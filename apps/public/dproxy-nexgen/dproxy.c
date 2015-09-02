#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include <syslog.h>

#include "dproxy.h"
#include "dns_decode.h"
#include "cache.h"
#include "conf.h"
#include "dns_list.h"
#include "dns_construct.h"
#include "dns_io.h"
/*****************************************************************************/
/*****************************************************************************/
int dns_main_quit;
int dns_sock;
fd_set rfds;
dns_request_t *dns_request_list;
/*****************************************************************************/
int is_connected()
{
  FILE *fp;

  if(!config.ppp_detect)return 1;

  fp = fopen( config.ppp_device_file, "r" );
  if(!fp)return 0;
  fclose(fp);
  return 1;
}
/*****************************************************************************/
int dns_init()
{
  struct sockaddr_in sa;
  struct in_addr ip;

  /* Clear it out */
  memset((void *)&sa, 0, sizeof(sa));
  
  dns_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  /* Error */
  if( dns_sock < 0 ){
	 debug_perror("Could not create socket");
	 exit(1);
  } 

  ip.s_addr = INADDR_ANY;
  sa.sin_family = AF_INET;
  memcpy((void *)&sa.sin_addr, (void *)&ip, sizeof(struct in_addr));
  sa.sin_port = htons(PORT);
  
  /* bind() the socket to the interface */
  if (bind(dns_sock, (struct sockaddr *)&sa, sizeof(struct sockaddr)) < 0){
	 debug_perror("dns_init: bind: Could not bind to port");
	 exit(1);
  }

  dns_main_quit = 0;

  FD_ZERO( &rfds );
  FD_SET( dns_sock, &rfds );

  dns_request_list = NULL;

  cache_purge( config.purge_time );
  
  return 1;
}
/*****************************************************************************/
void dns_handle_new_query(dns_request_t *m)
{
  struct in_addr in;
  int retval = -1;

  if( m->message.question[0].type == A || m->message.question[0].type == AAA){
    /* standard query */
    retval = cache_lookup_name( m->cname, m->ip );
  }else if( m->message.question[0].type == PTR ){
    /* reverse lookup */
    retval = cache_lookup_ip( m->ip, m->cname );
  }

  debug(".......... %s ---- %s\n", m->cname, m->ip );
  
  switch( retval )
    {
    case 0:
      if( is_connected() ){
	debug("Adding to list-> id: %d\n", m->message.header.id);
	dns_request_list = dns_list_add( dns_request_list, m );
	/*!!! relay the query untouched */
		
	inet_aton( config.name_server[0], &in );
	debug("Sent Request To %s\n",config.name_server[0]);
        dns_write_packet( dns_sock, in, PORT, m ); 
      }else{
	debug("Not connected **\n");
	dns_construct_error_reply(m);
	dns_write_packet( dns_sock, m->src_addr, m->src_port, m );
      }
      break;
    case 1:
      dns_construct_reply( m );
      dns_write_packet( dns_sock, m->src_addr, m->src_port, m );
      debug("Cache hit\n");
      break;
    default:
      debug("Unknown query type: %d\n", m->message.question[0].type );
    }

}
/*****************************************************************************/
void dns_handle_request(dns_request_t *m)
{
  dns_request_t *ptr = NULL;

  /* request may be a new query or a answer from the upstream server */
  ptr = dns_list_find_by_id( dns_request_list, m );

  if( ptr != NULL ){
    debug("Found query in list\n");
    /* message may be a response */
    if( m->message.header.flags.f.question == 1 ){
      if( m->message.header.flags.f.rcode == 0 ){ // lookup was succesful
        dns_write_packet( dns_sock, ptr->src_addr, ptr->src_port, m );
        debug("Replying with answer from %s\n", inet_ntoa( m->src_addr ));
        dns_request_list = dns_list_remove( dns_request_list, ptr );
	debug("Cache append: %s ----> %s\n", m->cname, m->ip );
	cache_name_append( m->cname, m->ip );
      }

    /*if( m->message.header.flags.f.question == 1 ){
      dns_write_packet( dns_sock, ptr->src_addr, ptr->src_port, m );
      debug("Replying with answer from %s\n", inet_ntoa( m->src_addr ));
      dns_request_list = dns_list_remove( dns_request_list, ptr );
      if( m->message.header.flags.f.rcode == 0 ){ // lookup was succesful
	debug("Cache append: %s ----> %s\n", m->cname, m->ip );
	cache_name_append( m->cname, m->ip );
      }*/

    }else{
      debug("Duplicate query\n");
    }
  }else{
    dns_handle_new_query( m );
  }

}
/*****************************************************************************/
int dns_main_loop()
{
  struct timeval tv;
  fd_set active_rfds;
  int retval;
  dns_request_t m;
  dns_request_t *ptr, *next;
  int purge_time = config.purge_time / 60;

  int next_server_index = 0;
  struct in_addr in;

  while( !dns_main_quit ){

    /* set the one second time out */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    /* now copy the main rfds in the active one as it gets modified by select*/
    active_rfds = rfds;

    retval = select( FD_SETSIZE, &active_rfds, NULL, NULL, &tv );

    if (retval){
      /* data is now available */
      dns_read_packet( dns_sock, &m );
      m.time_pending = 0;
      dns_handle_request( &m );
    }else{
      /* select time out */
      ptr = dns_request_list;
      while( ptr ){
	next = ptr->next;
	/* Resend query to a new nameserver if response
	 * has not been received from the current nameserver */
	
	ptr->time_pending++;
	if( ptr->time_pending > DNS_TIMEOUT ){
	  debug("Request timed out\n");
	  /* send error back */
	  dns_construct_error_reply(ptr);
	  dns_write_packet( dns_sock, ptr->src_addr, ptr->src_port, ptr );
	  dns_request_list = dns_list_remove( dns_request_list, ptr );
	}
	if( ( ptr->time_pending % DNS_SERVER_TIMEOUT == 0) && 
	    ( ptr->time_pending < DNS_TIMEOUT) )
	{
          /* Send a request to the next nameserver */
          next_server_index = ptr->time_pending/DNS_SERVER_TIMEOUT;
	  /* If already the maximum number of supported servers have
	    been tried out then remove the request from the list and
	    send a response  back */
            if(next_server_index >= MAX_NAME_SERVER || 
			(config.name_server[next_server_index]  == NULL ))
            {
	  	/* send error back */
		debug("All Servers Tried: No Response\n");
	  	dns_construct_error_reply(ptr);
	  	dns_write_packet( dns_sock, ptr->src_addr, ptr->src_port, ptr );
	  	dns_request_list = dns_list_remove( dns_request_list, ptr );
                debug("Sent a DNS error message to Client \n");		
	    }
	    else
	    {
	      /* Create the new server IP address and relay the query untouched */
              inet_aton( config.name_server[next_server_index], &in );
	      /*!!!*/
	      debug("Didn't get a response from last server\n");
	      debug("Sending a new request to %s\n", config.name_server[next_server_index]);
	      /* Send the new request to the next name server */
	      dns_write_packet( dns_sock, in, PORT, ptr );	
	    }
		
	}
	ptr = next;
      } /* while(ptr) */

      /* purge cache */
      purge_time--;
      if( !purge_time ){
	cache_purge( config.purge_time );
	purge_time = config.purge_time / 60;
      } 

    } /* if (retval) */
  }  
  return 0;
}
/*****************************************************************************/
void debug_perror( char * msg ) {
	debug( "%s : %s\n" , msg , strerror(errno) );
}
/*****************************************************************************/
void debug(char *fmt, ...)
{
#define MAX_MESG_LEN 1024
  
  va_list args;
  char text[ MAX_MESG_LEN ];
  
  sprintf( text, "[ %d ]: ", getpid());
  va_start (args, fmt);
  vsnprintf( &text[strlen(text)], MAX_MESG_LEN - strlen(text), fmt, args);
  va_end (args);
  
  if( config.debug_file[0] ){
    FILE *fp;
    fp = fopen( config.debug_file, "a");
    if(!fp){
      syslog( LOG_ERR, "could not open log file %m" );
      return;
    }
    fprintf( fp, "%s", text);
    fclose(fp);
  }
  
  /** if not in daemon-mode stderr was not closed, use it. */
  if( ! config.daemon_mode ) {
    fprintf( stderr, "%s", text);
  }
}
/*****************************************************************************
 * print usage informations to stderr.
 * 
 *****************************************************************************/
void usage(char * program , char * message ) {
  fprintf(stderr,"%s\n" , message );
  fprintf(stderr,"usage : %s [-c <config-file>] [-d] [-h] [-P]\n", program );
  fprintf(stderr,"\t-c <config-file>\tread configuration from <config-file>\n");
  fprintf(stderr,"\t-d \t\trun in debug (=non-daemon) mode.\n");
  fprintf(stderr,"\t-P \t\tprint configuration on stdout and exit.\n");
  fprintf(stderr,"\t-h \t\tthis message.\n");
}
/*****************************************************************************
 * get commandline options.
 * 
 * @return 0 on success, < 0 on error.
 *****************************************************************************/
int get_options( int argc, char ** argv ) 
{
  char c = 0;
  int not_daemon = 0;
  int want_printout = 0;
  char * progname = argv[0];

  conf_defaults();

  while( (c = getopt( argc, argv, "c:dhP")) != EOF ) {
    switch(c) {
	 case 'c':
  		conf_load(optarg);
		break;
	 case 'd':
		not_daemon = 1;
		break;
	 case 'h':
		usage(progname,"");
		return -1;
	 case 'P':
		want_printout = 1;
		break;
	 default:
		usage(progname,"");
		return -1;
    }
  }
  
  /** unset daemon-mode if -d was given. */
  if( not_daemon ) {
	 config.daemon_mode = 0;
  }
  
  if( want_printout ) {
	 conf_print();
	 exit(0);
  }
  return 0;
}
/*****************************************************************************/
void sig_hup (int signo)
{
  signal(SIGHUP, sig_hup); /* set this for the next sighup */
  conf_load (config.config_file);
}
/*****************************************************************************/
int main(int argc, char **argv)
{

  /* get commandline options, load config if needed. */
  if(get_options( argc, argv ) < 0 ) {
	  exit(1);
  }

  signal(SIGHUP, sig_hup);

  dns_init();

  if (config.daemon_mode) {
    /* Standard fork and background code */
    switch (fork()) {
	 case -1:	/* Oh shit, something went wrong */
		debug_perror("fork");
		exit(-1);
	 case 0:	/* Child: close off stdout, stdin and stderr */
//		close(0);
//		close(1);
//		close(2);
		break;	 
	 default:	/* Parent: Just exit */
		exit(0);
    }
  }

  dns_main_loop();

  return 0;
}

