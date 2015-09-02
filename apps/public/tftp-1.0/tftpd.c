/**********************************************************
Date: 		OCT 28th, 2006
Project :	NET4900 Project: tftpd.c TFTP Server

Programers:	
Craig Holmes
Reza Rahmanian 


File:		TFTP Server (main)
Purpose:	A TFTP server that will accept a connections from
		a client and transefet files.
Notes:		Here we are using the sendto and recvfrom
		functions so the server and client can exchange data.
***********************************************************************/

/* Include our header which contains libaries and defines */
#include "tftp.h"

#ifdef TRENDCHIP
#include "../lib/libtcapi.h"

#if !defined(TCSUPPORT_CT) 
#include "../../private/cfg_manager/cfg_manager.h"

#endif
//#define UG_ROMFILE 	"1"
//#define UG_KERNEL	"2"
//#define UG_FS		"3"

#endif

#define KERNEL_2_6_36 		(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))
#if 0
//Marfada 07.12.06
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>

#include <linux/version.h>

#if KERNEL_VERSION(2,6,0) <= LINUX_VERSION_CODE
#define IS_KERNEL26 1
#else
#define IS_KERNEL26 0
#endif

#if IS_KERNEL26
#include <mtd/mtd-user.h>
#else
#include <linux/mtd/mtd.h>
#endif

#define TRX_MAGIC       0x30524448      /* "HDR0" */
#define TRX_MAGIC1      0x31524448      /* "HDR1" */
#define BUFSIZE (16 * 1024)
char img_buf[BUFSIZE];
ssize_t buflen;
struct trx_header{
	uint32_t magic;			/* "HDR0" */
	uint32_t len;				/* Length of file including header */
	uint32_t crc32;			/* 32-bit CRC from flag_version to end of file */
	uint32_t flag_version;	/* 0:15 flags, 16:31 version */
	uint32_t offsets[3];    	/* Offsets of partitions from start of header */
};
int mtd_open(const char *, int);
int image_check_tc(int, const char *);
#endif
//Marfada end


/* Function prototypes */
#if defined(TCSUPPORT_IPV6_TFTP)
void tsend (char *, struct sockaddr_in6, char *, int);
void tget (char *, struct sockaddr_in6, char *, int);
#else
void tsend (char *, struct sockaddr_in, char *, int);
void tget (char *, struct sockaddr_in, char *, int);
#endif
int isnotvaliddir (char *);
void usage (void);
#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT 
// frank added 20110331 for bug 9609
#if defined(TCSUPPORT_IPV6_TFTP)
int isvalidip(struct sockaddr_in6 client);
#else
int isvalidip(struct sockaddr_in client);
#endif
#endif

#ifdef TCSUPPORT_FW_UPGRADE_16M
unsigned int mark;
int marklen;
#if defined(TCSUPPORT_IPV6_TFTP)
#if KERNEL_2_6_36
#define 	IPV6_SKB_MARK 	74
#else
#define 	IPV6_SKB_MARK 	50
#endif
#endif
#define 	IP_SKB_MARK 	50
#define SSID1_PORT_MASK 5
#define SSID4_PORT_MASK 8
#endif

/* default values which can be controlled by command line */
int debug = 0;
char path[64] = "/var/tmp/";
int port = 69;
unsigned short int ackfreq = 1;
int datasize = DEFBLOCKSIZE;
#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT 
#if defined(TCSUPPORT_IPV6_TFTP)
#define PATH_PROCNET_TCP "/proc/net/tcp6"
#else   
#define PATH_PROCNET_TCP "/proc/net/tcp"
#endif
#endif

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)
#define MAX_RECV_FILE_SIZE	0xc00000 					//12MB
#else
#define MAX_RECV_FILE_SIZE	0x800000 					//5.5MB
#endif


int
main (int argc, char **argv)
{
  /*local variables */
  extern char *optarg;
  int sock, n, client_len, pid, status, opt, tid;
  char opcode, *bufindex, filename[196], mode[12];

  #if defined(TCSUPPORT_IPV6_TFTP)
  struct sockaddr_in6 server, client;	/*the address structure for both the server and client */
  #else
  struct sockaddr_in server, client;	/*the address structure for both the server and client */
  #endif

/* All of the following deals with command line switches */
  while ((opt = getopt (argc, argv, "dh:p:P:a:s:")) != -1)	/* this function is handy */
    {
      switch (opt)
	{
	case 'p':		/* path (opt required) */
	  if (!isnotvaliddir (optarg))
	    {
	      printf
		("Sorry, you specified an invalid/non-existant directory. Make sure the directory exists.\n");
	      return 0;
	    }
	  strncpy (path, optarg, sizeof (path) - 1);


	  break;
	case 'd':		/* debug mode (no opts) */
	  debug = 1;
	  break;
	case 'P':		/* Port (opt required) */
	  port = atoi (optarg);
	  break;
	case 'a':		/* ack frequency (opt required) */
	  ackfreq = atoi (optarg);
	  if (ackfreq > MAXACKFREQ)
	    {
	      printf
		("Sorry, you specified an ack frequency higher than the maximum allowed (Requested: %d Max: %d)\n",
		 ackfreq, MAXACKFREQ);
	      return 0;
	    }
	  else if (ackfreq == 0)
	    {
	      printf ("Sorry, you have to ack sometime.\n");
	      return 0;
	    }
	  break;
	case 's':		/* File chunk size (opt required) */
	  datasize = atoi (optarg);

	  if (datasize > MAXDATASIZE)
	    {
	      printf
		("Sorry, you specified a data size higher than the maximum allowed (Requested: %d Max: %d)\n",
		 datasize, MAXDATASIZE);
	      return 0;
	    }

	  break;
	case 'h':		/* Help (no opts) */
	  usage ();
	  return (0);
	  break;
	default:		/* everything else */
	  usage ();
	  return (0);
	  break;

	}
    }
/* Done dealing with switches */

  /*Create the socket, a -1 will show us an error */
  #if defined(TCSUPPORT_IPV6_TFTP)
  if ((sock = socket (AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0)
  #else
  if ((sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
  #endif
  	{
      printf ("Server Socket could not be created");
      return 0;
    }
	memset(&server,0,sizeof(server));
  /*set the address values for the server */
  #if defined(TCSUPPORT_IPV6_TFTP)
  server.sin6_family = AF_INET6;	/*address family for TCP and UDP */
  server.sin6_addr = in6addr_any;   /* wildcard */
  server.sin6_port = htons (port);	/*pick a free port */
  #else
  server.sin_family = AF_INET;	/*address family for TCP and UDP */
  server.sin_addr.s_addr = htonl (INADDR_ANY);	/*use any address */
  server.sin_port = htons (port);	/*pick a free port */
  #endif

  /*Bind the socket */
  if (bind (sock, (struct sockaddr *) &server, sizeof (server)) < 0)
    {
      printf
	("Server bind failed. Server already running? Proper permissions?\n");
      return (2);
    }
  if (!debug)
    {
      pid = fork ();

      if (pid != 0)		/* if pid != 0 then we are the parent */
	{
	  if (pid == -1)
	    {
	      printf ("Error: Fork failed!\n");
	      return 0;
	    }
	  else
	    {
	      printf ("Daemon Successfully forked (pid: %d)\n", pid);
	      return 1;
	    }
	}


    }
  else
    {
      printf
	("tftpd server is running in debug mode and will not fork. \nMulti-threaded mode disabled.\nServer is bound to port %d and awaiting connections\nfile path: %s\n",
	#if defined(TCSUPPORT_IPV6_TFTP)
	 ntohs (server.sin6_port), path);
	 #else
	 ntohs (server.sin_port), path);
	 #endif
    }
  /*endless loop to get connections from the client */
  while (1)
    {
      client_len = sizeof (client);	/*get the length of the client */
      memset (buf, 0, BUFSIZ);	/*clear the buffer */
      /*the fs message */
      n = 0;
      while (errno == EAGAIN || n == 0)	/* This loop is required because on linux we have to acknowledge complete children with waitpid. Ugh. */
	{
	  waitpid (-1, &status, WNOHANG);
	  n =
	    recvfrom (sock, buf, BUFSIZ, MSG_DONTWAIT,
		      (struct sockaddr *) &client,
		      (socklen_t *) & client_len);
	  if (n < 0 && errno != EAGAIN)
	    {
	      printf ("The server could not receive from the client");
	      return 0;
	    }
#ifdef TCSUPPORT_FW_UPGRADE_16M
#if defined(TCSUPPORT_IPV6_TFTP)
	getsockopt(sock, SOL_IPV6, IPV6_SKB_MARK, &mark, &marklen);
#else
	getsockopt(sock, SOL_IP, IP_SKB_MARK, &mark, &marklen);
#endif
#endif
	/*speed up, shnwind modify.*/
	  usleep (1000);
	}

      if (debug){
#if defined(TCSUPPORT_IPV6_TFTP)
		char str[INET6_ADDRSTRLEN];
		printf ("Connection from %s, port %d\n",
			inet_ntop (AF_INET6,&client.sin6_addr,str,sizeof(str)), ntohs (client.sin6_port));
#else
		printf ("Connection from %s, port %d\n",
			inet_ntoa (client.sin_addr), ntohs (client.sin_port));
#endif
		}
      bufindex = buf;		//start our pointer going
      if (bufindex++[0] != 0x00)
	{			//first TFTP packet byte needs to be null. Once the value is taken increment it.
	  if (debug)
	    printf ("Malformed tftp packet.\n");

	  return 0;
	}
      #if defined(TCSUPPORT_IPV6_TFTP)
	  tid = ntohs (client.sin6_port);	/* record the tid */
	  #else
	  tid = ntohs (client.sin_port);	/* record the tid */
	  #endif
      opcode = *bufindex++;	//opcode is in the second byte.
      if (opcode == 1 || opcode == 2)	// RRQ or WRQ. The only two really valid packets on port 69
	{
	  strncpy (filename, bufindex, sizeof (filename) - 1);	/* Our pointer has been nudged along the recieved string so the first char is the beginning of the filename. This filename is null deliimited so we can use the str family of functions */
	  bufindex += strlen (filename) + 1;	/* move the pointer to after the filename + null byte in the string */
	  strncpy (mode, bufindex, sizeof (mode) - 1);	/* like the filename, we are at the beginning of the null delimited mode */
	  bufindex += strlen (mode) + 1;	/* move pointer... */
	  if (debug)
	    printf ("opcode: %x filename: %s packet size: %d mode: %s\n", opcode, filename, n, mode);	/*show the message to the server */

	}
      else
	{
	  if (debug)
	    printf ("opcode: %x size: %d \n", opcode, sizeof (n));	/*show the message to the server */

	}
      switch (opcode)		/* case one and two are valid on port 69 or server port... no other codes are */
	{
	case 1:
	  if (debug)
	    {
	      printf ("Opcode indicates file read request\n");
	      tsend (filename, client, mode, tid);
	    }
	  else
	    {
	      pid = fork ();

	      if (pid == 0)
		{		/* if we are pid != 0 then we are the parent */

		  tsend (filename, client, mode, tid);
		  exit (1);
		}
	    }
	  break;
	case 2:
	  if (debug)
	    {
	      printf ("Opcode indicates file write request\n");
	      tget (filename, client, mode, tid);
	    }
	  else
	    {
	      pid = fork ();

	      if (pid == 0)	/* if we are pid != 0 then we are the parent */
		{
		  tget (filename, client, mode, tid);
		  exit (1);
		}
	    }
	  break;
	default:
	  if (debug)
	    printf ("Invalid opcode detected. Ignoring packet.");
	  break;




	}			//end of while
    }
}

#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT
#define ESTABLISHED				0x1
#define TELNET_PORT				0x17
#define ADDR_BYTES				0x4
#define IPV4_ADDR_LEN			4
#define ACCEPT_TFTP_UPLOAD		1
#define SESSION_LOGIN_SUCCESS	"1"
#define TELNET_INFO_DIR			"/tmp/telnet_info"
/*_____________________________________________________________________________
**      function name: isvalidip
**      descriptions:
**		  Check if the client can do tftp upgrade
**
**      parameters:
**           client: client pc info
**
**      global:
**          
**
**      return:
**        1		:valid client
**        other :invalid client
**
**      call:
**   	
**
**      revision:
** 		1. frank added 20110331, bug 9609
**____________________________________________________________________________
*/
#if defined(TCSUPPORT_IPV6_TFTP)
int isvalidip(struct sockaddr_in6 client)
#else
int isvalidip(struct sockaddr_in client)
#endif
{
	FILE *procinfo, *loginpid, *logincheck;
	char buffer[128], tmpbuf[128], pidbuf[32];
	#if defined(TCSUPPORT_IPV6_TFTP)
	char local_addr_str[INET6_ADDRSTRLEN], rem_addr_str[INET6_ADDRSTRLEN];
	uint32_t rem_addr[4];
	#else
	char local_addr_str[INET_ADDRSTRLEN], rem_addr_str[INET_ADDRSTRLEN];
	uint32_t rem_addr;
	#endif
	
	char addr[ADDR_BYTES][16];
	uint32_t local_port, rem_port, d, state;

	procinfo = fopen(PATH_PROCNET_TCP, "r"); 
	
	if (procinfo == NULL) {
		tcdbg_printf("%s file open failed\n", PATH_PROCNET_TCP);
		fclose(procinfo);
		return -1;
	} else {
		do {
			if (fgets(buffer, sizeof(buffer), procinfo)) {
				tmpbuf[0] = '\0';
				#if defined(TCSUPPORT_IPV6_TFTP)
				sscanf(buffer, "%d: %32[0-9A-Fa-f]:%X %32[0-9A-Fa-f]:%X %X %128s\n",
						&d, local_addr_str, &local_port, rem_addr_str, &rem_port, &state, tmpbuf);
				sscanf(rem_addr_str, "%08X%08X%08X%08X", &rem_addr[0], &rem_addr[1], &rem_addr[2], &rem_addr[3]);
				
				if (local_port == TELNET_PORT && memcmp(&rem_addr,&client.sin6_addr,sizeof(struct in6_addr)) == 0
					&& state==ESTABLISHED) {
					if(IN6_IS_ADDR_V4MAPPED(&(client.sin6_addr))) {
						sprintf(tmpbuf, "%s/%08X_%X.pid", TELNET_INFO_DIR, rem_addr[3], rem_port);
					}
					else{
						sprintf(tmpbuf, "%s/%08X%08X%08X%08X_%X.pid", TELNET_INFO_DIR, rem_addr[0], rem_addr[1], rem_addr[2], rem_addr[3], rem_port);
					}
					loginpid = fopen(tmpbuf, "r");
					if (loginpid != NULL) { 
						fgets(pidbuf, sizeof(pidbuf), loginpid);
						fclose(loginpid);
						sprintf(tmpbuf, "%s/session%s", TELNET_INFO_DIR, pidbuf);
						logincheck = fopen(tmpbuf, "r");
						if (logincheck != NULL) {
							fgets(tmpbuf, sizeof(tmpbuf), logincheck);
							if (strcmp(tmpbuf, SESSION_LOGIN_SUCCESS)==0) {
								fclose(logincheck);
								return 1;
							}	
							fclose(logincheck);	
						}
					} 	
				}
				#else
				sscanf(buffer, "%d: %16[0-9A-Fa-f]:%X %16[0-9A-Fa-f]:%X %X %128s\n",
						&d, local_addr_str, &local_port, rem_addr_str, &rem_port, &state, tmpbuf);
				sscanf(rem_addr_str, "%X", &rem_addr);

				if (local_port == TELNET_PORT && rem_addr==client.sin_addr.s_addr && state==ESTABLISHED) {
					sprintf(tmpbuf, "%s/%08X_%X.pid", TELNET_INFO_DIR, rem_addr, rem_port);
					loginpid = fopen(tmpbuf, "r");
					if (loginpid != NULL) { 
						fgets(pidbuf, sizeof(pidbuf), loginpid);
						fclose(loginpid);
						sprintf(tmpbuf, "%s/session%s", TELNET_INFO_DIR, pidbuf);
						logincheck = fopen(tmpbuf, "r");
						if (logincheck != NULL) {
							fgets(tmpbuf, sizeof(tmpbuf), logincheck);
							if (strcmp(tmpbuf, SESSION_LOGIN_SUCCESS)==0) {
								fclose(logincheck);
								return 1;
							}	
							fclose(logincheck);	
						}
					} 
				}
			#endif
			}	
		} while (!feof(procinfo));
		fclose(procinfo);
	}
	return 0;
}
#endif

void
#if defined(TCSUPPORT_IPV6_TFTP)
tget (char *pFilename, struct sockaddr_in6 client, char *pMode, int tid)
#else
tget (char *pFilename, struct sockaddr_in client, char *pMode, int tid)
#endif
{
  /* local variables */
  int sock, len, client_len, opcode, i, j, n, flag = 1, fd, ret;
  unsigned int file_size;
  unsigned short int count = 0, rcount = 0;
  unsigned char filebuf[MAXDATASIZE + 1];
  unsigned char packetbuf[MAXDATASIZE + 12];
  extern int errno;
  char filename[128], mode[12], fullpath[196], *bufindex, ackbuf[512], string[64];
  #if defined(TCSUPPORT_IPV6_TFTP)
  struct sockaddr_in6 data;
  #else
  struct sockaddr_in data;
  #endif
 
  FILE *fp, *fp2, *fp3;			/* pointer to the file we will be getting */
  int flags = 0;
  fd_set readfd;
  struct timeval timeout;
	#ifdef TRENDCHIP
	char ug_type[2];	
	#endif


#if 0
  //Marfada 07.12.06
  int imagefd;
  char device[16];
  char tftp_path[32];
  int flag_write = 0;
  //Marfada end
 #endif 

  strcpy (filename, pFilename);	//copy the pointer to the filename into a real array
  strcpy (mode, pMode);		//same as above

#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT
  if (isvalidip(client) != ACCEPT_TFTP_UPLOAD) 
  	{
	  tcdbg_printf("Client PC don't be accepted to do tftp upgrade\n");
	  return ;
	}  
#endif

  if (debug)
    printf ("branched to file receive function\n");

	#if defined(TCSUPPORT_IPV6_TFTP)
	if ((sock = socket (PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0)	//startup a socket
	#else
	if ((sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)	//startup a socket
	#endif
	{
      printf ("Server reconnect for getting did not work correctly\n");
      return;
    }
  if (!strncasecmp (mode, "octet", 5) && !strncasecmp (mode, "netascii", 8))	/* these two are the only modes we accept */
    {
      if (!strncasecmp (mode, "mail", 4))
	len = sprintf ((char *) packetbuf,
		       "%c%c%c%cThis tftp server will not operate as a mail relay%c",
		       0x00, 0x05, 0x00, 0x04, 0x00);
      else
	len = sprintf ((char *) packetbuf,
		       "%c%c%c%cUnrecognized mode (%s)%c",
		       0x00, 0x05, 0x00, 0x04, mode, 0x00);
      if (sendto (sock, packetbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
	{
	  printf
	    ("Mismatch in number of sent bytes while trying to send mode error packet\n");
	}
      return;
    }
  if (strchr (filename, 0x5C) || strchr (filename, 0x2F))	//look for illegal characters in the filename string these are \ and /
    {
      if (debug)
	printf ("Client requested to upload bad file: forbidden name\n");
      len =
	sprintf ((char *) packetbuf,
		 "%c%c%c%cIllegal filename.(%s) You may not attempt to descend or ascend directories.%c",
		 0x00, 0x05, 0x00, 0x00, filename, 0x00);
      if (sendto (sock, packetbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
	{
	  printf
	    ("Mismatch in number of sent bytes while trying to send error packet\n");
	}
      return;

    }
  strcpy (fullpath, path);
  strncat (fullpath, filename, sizeof (fullpath) - 1);	//build the full file path by appending filename to path
  fp = fopen (fullpath, "w");	/* open the file for writing */
  if (fp == NULL)
    {				//if the pointer is null then the file can't be opened - Bad perms 
      if (debug)
	printf ("Server requested bad file: cannot open for writing (%s)\n",
		fullpath);
      len =
	sprintf ((char *) packetbuf,
		 "%c%c%c%cFile cannot be opened for writing (%s)%c", 0x00,
		 0x05, 0x00, 0x02, fullpath, 0x00);
      if (sendto (sock, packetbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
	{
	  printf
	    ("Mismatch in number of sent bytes while trying to send error packet\n");
	}
      return;
    }
  else				/* everything worked fine */
    {
      if (debug)
	printf ("Getting file... (destination: %s) \n", fullpath);

    }
  /* zero the buffer before we begin */
  memset (filebuf, 0, sizeof (filebuf));
  n = datasize + 4;

#ifdef WITHVOIP
	fp2 = fopen("/etc/restart_voip","w");
	if(fp2 != NULL){
		fputc('s',fp2);
		fclose(fp2);
	}
#endif

#if 1 // Enhance tftpd by Richard.Lai

/* frank added 20110407 for bug 9613 */
#ifdef TCSUPPORT_FW_UPGRADE_16M
#if !defined(TCSUPPORT_CD_NEW_GUI) 
	fp3 = fopen("/tmp/FW_UPGRADE", "w");
	if (fp3 != NULL) {
		fputs("1", fp3);
		fclose(fp3);
	}
	sleep(2);
	mark = (mark & 0xf0000000) >> 28;
	#if 1
		tcdbg_printf("%s, mark = %d\n", __FUNCTION__, mark);
	#endif
#if 1
		if (mark >= SSID1_PORT_MASK && mark <= SSID4_PORT_MASK)
			system("/usr/script/before_web_download.sh 2");
		else
			system("sh /usr/script/before_web_download_remove_wifi.sh 2");
#else	
		system("/usr/script/before_web_download.sh 2");
#endif

#endif
	
	fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
	if( fd <= 0 ){
		printf("adsl_stats file open fail\n");
		return;
	}

	ret = read(fd, string, 64);
	close(fd);
	if( ret <= 0 ){
		printf("adsl_stats file read fail\n");
		return;
	}

	if(!strstr(string, "up")) {
		system("w ad close");	
	}
#if 0 
	else {
		// frank added
		system("/usr/bin/killall -9 upnpd 2>/dev/null");
		system("/usr/bin/killall -9 igmpproxy 2>/dev/null");
		system("/usr/bin/killall -9 dnsmasq 2>/dev/null");
		system("/usr/bin/killall -9 rt2860apd 2>/dev/null");
		system("/usr/bin/killall -9 pppoe-relay 2>/dev/null");
	}
#endif
	sleep(1);	
	sync();
#endif
#if 0
	/* Release memory first */
	system("sh /usr/script/before_tftp_download.sh");
        sleep(1);
        sync();

#endif
	/* Send first ACK Packet (0x4) */
	if (debug)
		printf("Sending ack # %04d (length: 4)\n", count);
	memset(ackbuf, 0, sizeof (ackbuf));
	len = sprintf(ackbuf, "%c%c%c%c", NULLSTART, ACK, 0x00, 0x00);
	if (sendto(sock, ackbuf, len, 0, (struct sockaddr *) &client, sizeof(client)) != len) {
		if (debug)
			printf("Mismatch in number of sent bytes\n");
		fflush(fp);
		fclose(fp);
		#ifdef TCSUPPORT_FW_UPGRADE_16M
		system("reboot");
		#else
	    	unlink(fullpath);
		#endif
		return;
	}

	/* Set socket to O_NONBLOCK */
	flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);

	/* frank added 20110408 bug 9613*/
	count++;
	file_size = 0;
	/* Receive loop */
	while(n == datasize + HEADERSIZE) {
		// frank delete 20110407 bug 9613
//		count++;
		for (j = 0; j < RETRIES; j++) { /* Prepare retry mechanism */
			/* Setup timeout */
			timeout.tv_sec = TIMEOUT / 1000;
			timeout.tv_usec = 0;
			/* Setup fd_set */
			FD_ZERO(&readfd);
			FD_SET(sock, &readfd);
			/* Use select() */
			if(select(sock + 1, &readfd, NULL, NULL, &timeout) > 0) {
				if(FD_ISSET(sock, &readfd)) {
					/* Receive data packet */
					memset(packetbuf, 0, sizeof (packetbuf));
					n = recvfrom(sock, packetbuf, sizeof (packetbuf) - 1, 0, (struct sockaddr *) &data, (socklen_t *) &client_len);

					/* Check src ip and udp port */
					#if defined(TCSUPPORT_IPV6_TFTP)
					if ( memcmp(&client.sin6_addr,&data.sin6_addr,sizeof(struct in6_addr)) == 0 
						&&	tid == ntohs (client.sin6_port) ) {
					#else
					if ((client.sin_addr.s_addr == data.sin_addr.s_addr) &&
					(tid == ntohs (client.sin_port))) {
					#endif
						opcode = packetbuf[1];
						rcount = (packetbuf[2] << 8) | packetbuf[3];
						/* Check TFTP Data Packet (0x3) and BLOCK id */
						if(packetbuf[0] == NULLSTART && opcode == DATA && rcount == count) {
							/* Write data to local file */
							if(fwrite(packetbuf + HEADERSIZE, n - HEADERSIZE, 1, fp) != 1) {
								fflush(fp);
								fclose(fp);
								if (debug)
									printf("File failed to recieve properly\n");
								#ifdef TCSUPPORT_FW_UPGRADE_16M
								system("reboot");
								#else
								unlink(fullpath);
								#endif

								return;
							}

							// frank added 20110428
							file_size += (n - HEADERSIZE);

							if (file_size > MAX_RECV_FILE_SIZE) {
								tcdbg_printf("Upload file size is out of range\n");
								#ifdef TCSUPPORT_FW_UPGRADE_16M
								system("reboot");
								#else
								unlink(fullpath);
								#endif

								return;
							}

							/* Arrange datasize via client's packet size. */
							if (flag) {
								if (n > DEFBLOCKSIZE + HEADERSIZE)
									datasize = n - HEADERSIZE;
								flag = 0;
							}

							if (debug)
								printf("Remote host sent data packet #%d (Opcode: %d packetsize: %d filechunksize: %d)\n", rcount, opcode, n, n - 4);
							
							/* Send ACK Packet (0x4) */
							if (debug)
								printf("Sending ack # %04d (length: 4)\n", count);
							len = sprintf(ackbuf, "%c%c%c%c", NULLSTART, ACK, rcount >> 8, rcount & 0xff);
							if (sendto(sock, ackbuf, len, 0, (struct sockaddr *) &client, sizeof(client)) != len) {
								if (debug)
									printf("Mismatch in number of sent bytes\n");
								fflush(fp);
								fclose(fp);
								#ifdef TCSUPPORT_FW_UPGRADE_16M
								system("reboot");
								#else
								unlink(fullpath);
								#endif
								return;
							}
							// frank modify 20110407 bug 9613
							count++;

							break;
						}
						
						if (debug)
							printf("Badly ordered/invalid data packet (Got OP: %d Block: %d) (Wanted Op: 3 Block: %d)\n", opcode, rcount, count);

						/* Send error message */
						if (opcode > ERR) {
							len = sprintf((char *) packetbuf, "%c%c%c%cIllegal operation%c", NULLSTART, ERR, 0x00, 0x04, 0x00);
							if (sendto(sock, packetbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len) {
								if (debug)
									printf("Mismatch in number of sent bytes while trying to send mode error packet\n");
								fflush(fp);
								fclose(fp);
								#ifdef TCSUPPORT_FW_UPGRADE_16M
								system("reboot");
								#else
								unlink(fullpath);
								#endif
								return;
							}
						}
						
						break;
					} else {
						/* Send last ACK Packet again */
						if (sendto(sock, ackbuf, len, 0, (struct sockaddr *) &client, sizeof(client)) != len) {
							if (debug)
								printf("Send ACK packet fail.\n");
							fflush(fp);
							fclose(fp);
							#ifdef TCSUPPORT_FW_UPGRADE_16M
							system("reboot");
							#else
							unlink(fullpath);
							#endif
							return;
						}
						j--;
						continue;
					}
				}
			}
		}
		if (j == RETRIES) {
			if (debug)
				printf("Data recieve Timeout. Aborting transfer\n");
			fflush(fp);
			fclose(fp);
			#ifdef TCSUPPORT_FW_UPGRADE_16M
			system("reboot");
			#else
			unlink(fullpath);
			#endif
			return;
		}
	}

	tcdbg_printf("tftpd: Upload completed.\n");

#endif

#if 0
  do
    {
      /* zero buffers so if there are any errors only NULLs will be exposed */
      memset (packetbuf, 0, sizeof (packetbuf));
      memset (ackbuf, 0, sizeof (ackbuf));
      if (debug)
	printf ("== just entered do-while count: %d  n: %d\n", count, n);


      if (count == 0 || (count % ackfreq) == 0 || n != (datasize + 4))	/* ack the first packet, count % ackfreq will make it so we only ACK everyone ackfreq ACKs, ack the last packet */
	{
	  len = sprintf (ackbuf, "%c%c%c%c", 0x00, 0x04, 0x00, 0x00);
	  ackbuf[2] = (count & 0xFF00) >> 8;	//fill in the count (top number first)
	  ackbuf[3] = (count & 0x00FF);	//fill in the lower part of the count
	  if (debug)
	    printf ("Sending ack # %04d (length: %d)\n", count, len);

	  if (sendto
	      (sock, ackbuf, len, 0, (struct sockaddr *) &client,
	       sizeof (client)) != len)
	    {
	      if (debug)
		printf ("Mismatch in number of sent bytes\n");
	      return;
	    }
	}
      else if (debug)
	{
	  printf ("No ack required on packet count %d\n", count);
	}
      if (n != (datasize + 4))	/* remember if our datasize is less than a full packet this was the last packet to be received */
	{
	  if (debug)
	    printf
	      ("Last chunk detected (file chunk size: %d). exiting while loop\n",
	       n - 4);
	  goto done;		/* gotos are not optimal, but a good solution when exiting a multi-layer loop */
	}
      memset (filebuf, 0, sizeof (filebuf));
      count++;

      for (j = 0; j < RETRIES; j++)	/* this allows us to loop until we either break out by getting the correct ack OR time out because we've looped more than RETRIES times */
	{
	  client_len = sizeof (data);
	  errno = EAGAIN;	/* this allows us to enter the loop */
	  n = -1;
	  for (i = 0; errno == EAGAIN && i <= TIMEOUT && n < 0; i++)	/* this for loop will just keep checking the non-blocking socket until timeout */
	    {

	      n =
		recvfrom (sock, packetbuf, sizeof (packetbuf) - 1,
			  MSG_DONTWAIT, (struct sockaddr *) &data,
			  (socklen_t *) & client_len);
	      /*if (debug)
	         printf ("The value recieved is n: %d\n",n); */
	      /*speed up, shnwind modify.*/
		usleep (1000);
	    }

	  if (n < 0 && errno != EAGAIN)	/* this will be true when there is an error that isn't the WOULD BLOCK error */
	    {
	      if (debug)
		printf
		  ("The server could not receive from the client (errno: %d n: %d)\n",
		   errno, n);

	      //resend packet
	    }
	  else if (n < 0 && errno == EAGAIN)	/* this is true when the error IS would block. This means we timed out */
	    {
	      if (debug)
		printf ("Timeout waiting for data (errno: %d == %d n: %d)\n",
			errno, EAGAIN, n);
	      //resend packet

	    }
	  else
	    {
	      if (client.sin_addr.s_addr != data.sin_addr.s_addr)	/* checks to ensure get from ip is same from ACK IP */
		{
		  if (debug)
		    printf
		      ("Error recieving file (data from invalid address)\n");
		  j--;
		  continue;	/* we aren't going to let another connection spoil our first connection */
		}

	      if (tid != ntohs (client.sin_port))	/* checks to ensure get from the correct TID */
		{
		  if (debug)
		    printf ("Error recieving file (data from invalid tid)\n");
		  len = sprintf ((char *) packetbuf,
				 "%c%c%c%cBad/Unknown TID%c",
				 0x00, 0x05, 0x00, 0x05, 0x00);
		  if (sendto (sock, packetbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
		    {
		      printf
			("Mismatch in number of sent bytes while trying to send mode error packet\n");
		    }
		  j--;

		  continue;	/* we aren't going to let another connection spoil our first connection */
		}
/* this formatting code is just like the code in the main function */
	      bufindex = (char *) packetbuf;	//start our pointer going
	      if (bufindex++[0] != 0x00)
		printf ("bad first nullbyte!\n");
	      opcode = *bufindex++;
	      rcount = *bufindex++ << 8;
	      rcount &= 0xff00;
	      rcount += (*bufindex++ & 0x00ff);



	      memcpy ((char *) filebuf, bufindex, n - 4);	/* copy the rest of the packet (data portion) into our data array */
	      if (debug)
		printf
		  ("Remote host sent data packet #%d (Opcode: %d packetsize: %d filechunksize: %d)\n",
		   rcount, opcode, n, n - 4);
	      if (flag)
		{
		  if (n > 516)
		    datasize = n - 4;
		  flag = 0;
		}
	      if (opcode != 3 || rcount != count)	/* ack packet should have code 3 (data) and should be ack+1 the packet we just sent */
		{
		  if (debug)
		    printf
		      ("Badly ordered/invalid data packet (Got OP: %d Block: %d) (Wanted Op: 3 Block: %d)\n",
		       opcode, rcount, count);
/* sending error message */
		  if (opcode > 5)
		    {
		      len = sprintf ((char *) packetbuf,
				     "%c%c%c%cIllegal operation%c",
				     0x00, 0x05, 0x00, 0x04, 0x00);
		      if (sendto (sock, packetbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
			{
			  printf
			    ("Mismatch in number of sent bytes while trying to send mode error packet\n");
			}
		    }

		}
	      else
		{
		  break;
		}
	    }


	  if (sendto
	      (sock, ackbuf, len, 0, (struct sockaddr *) &client,
	       sizeof (client)) != len)
	    {
	      if (debug)
		printf ("Mismatch in number of sent bytes\n");
	      return;
	    }

	}
      if (j == RETRIES)
	{
	  if (debug)
	    printf ("Data recieve Timeout. Aborting transfer\n");
	  fclose (fp);

	  return;
	}

    }

  while (fwrite (filebuf, 1, n - 4, fp) == n - 4);	/* if it doesn't write the file the length of the packet received less 4 then it didn't work */
  fclose (fp);
  sync ();
  if (debug)
    printf ("fclose and sync successful. File failed to recieve properly\n");
  return;

done:
#endif	
	#if 0 //Marfada
  	printf("File uploaded\n");
	printf("Upload: %s\n", filename);
	if(strncmp(filename, "linux", 5) == 0)
		strcpy(device, "kernel");
	else if(strncmp(filename, "rootfs", 6) == 0)
		strcpy(device, "rootfs");
	#endif
	
	
	fclose (fp);
	#if 0
  	fopen(fullpath, "r");
	imagefd = fileno(fp);
	if(imagefd <= 0)
		printf("Couldn't open image file: %s!\n", fullpath);
	if(!image_check_tc(imagefd, device))
	{
		printf("The firmware is illegal!!\r\n");
		unlink(fullpath);
	}
	else
	{
		fflush(stdout);
		printf("WARNING:\nIt will take some minutes writing flash.\nDon't reboot modem within 5 minutes.\nThe modem will reboot itself after finishing writing flash.\r\n");
		flag_write = 1;
	}
	if(flag_write == 1)
		system("/usr/script/upgrade_firmware.sh");
	fclose (fp);
	#endif
  
  sync ();
  if (debug)
    printf ("fclose and sync successful. File received successfully\n");
	#ifdef TRENDCHIP
	memset(ug_type,0,sizeof(ug_type));
	/*if(strcmp(filename, "linux.7z.trx") == 0){
		strcpy(ug_type,UG_KERNEL);
	}
	else if(strcmp(filename, "rootfs.trx") == 0){
		strcpy(ug_type,UG_FS);
	}*/
#if 0	
	if((strcmp(filename, "linux.7z.trx") != 0)
		&& (strcmp(filename, "rootfs.trx") != 0)
		&&	(strcmp(filename, "tclinux.bin") != 0)){
		printf ("Not allowed to upload '%s'.\n", filename);
		unlink(fullpath);
		return;
	}
#else
	if((strcmp(filename, "tclinux.bin") != 0)){
	    printf ("Not allowed to upload '%s'.\n", filename);
	    unlink(fullpath);
//#ifdef WITHVOIP
		#if defined (TCSUPPORT_FW_UPGRADE_16M) || defined (WITHVOIP)
		system("reboot 1 &");
		#endif
//#endif		
	    return;
	}

#endif	
	/*shnwind modify new upgrade method. 2009.3.31*/
	 /*do firmware upgrade*/
	sprintf(ug_type,"%d",NO_HEADER);
	if(tcapi_set("System_Entry","upgrade_fw", ug_type)!=0){
	  	unlink(fullpath);
		return;
	}
	else{
		if(tcapi_commit("System_Entry")!=0){
			printf("The firmware is illegal!!\r\n");
			unlink(fullpath);
			return;
		}
		else{
			printf ("WARNING:\nIt will take some minutes writing flash.\nDon't reboot modem within 5 minutes.\nThe modem will reboot itself after finishing writing flash.\r\n");  
		}
	}
	#endif
  return;

}







void
#if defined(TCSUPPORT_IPV6_TFTP)
tsend (char *pFilename, struct sockaddr_in6 client, char *pMode, int tid)
#else
tsend (char *pFilename, struct sockaddr_in client, char *pMode, int tid)
#endif
{
  int sock, len, client_len, opcode, ssize = 0, n, i, j, bcount = 0;
  unsigned short int count = 0, rcount = 0, acked = 0;
  unsigned char filebuf[MAXDATASIZE + 1];
  unsigned char packetbuf[MAXACKFREQ][MAXDATASIZE + 12],
    recvbuf[MAXDATASIZE + 12];
  char filename[128], mode[12], fullpath[196], *bufindex;
  #if defined(TCSUPPORT_IPV6_TFTP)
  struct sockaddr_in6 ack;
  #else
  struct sockaddr_in ack;
  #endif

  FILE *fp;			/* pointer to the file we will be sending */

  strcpy (filename, pFilename);	//copy the pointer to the filename into a real array
  strcpy (mode, pMode);		//same as above

  if (debug)
    printf ("branched to file send function\n");


#if defined(TCSUPPORT_IPV6_TFTP)
   if ((sock = socket (PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0)	//startup a socket
#else
	if ((sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)	//startup a socket
#endif
    {
      printf ("Server reconnect for sending did not work correctly\n");
      return;
    }
  if (!strncasecmp (mode, "octet", 5) && !strncasecmp (mode, "netascii", 8))	/* these two are the only modes we accept */
    {
      if (!strncasecmp (mode, "mail", 4))
	len = sprintf ((char *) packetbuf[0],
		       "%c%c%c%cThis tftp server will not operate as a mail relay%c",
		       0x00, 0x05, 0x00, 0x04, 0x00);
      else
	len = sprintf ((char *) packetbuf[0],
		       "%c%c%c%cUnrecognized mode (%s)%c",
		       0x00, 0x05, 0x00, 0x04, mode, 0x00);
      if (sendto (sock, packetbuf[0], len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
	{
	  printf
	    ("Mismatch in number of sent bytes while trying to send mode error packet\n");
	}
      return;
    }
  if (strchr (filename, 0x5C) || strchr (filename, 0x2F))	//look for illegal characters in the filename string these are \ and /
    {
      if (debug)
	printf ("Server requested bad file: forbidden name\n");
      len =
	sprintf ((char *) packetbuf[0],
		 "%c%c%c%cIllegal filename.(%s) You may not attempt to descend or ascend directories.%c",
		 0x00, 0x05, 0x00, 0x00, filename, 0x00);
      if (sendto (sock, packetbuf[0], len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
	{
	  printf
	    ("Mismatch in number of sent bytes while trying to send error packet\n");
	}
      return;

    }
  strcpy (fullpath, path);
  strncat (fullpath, filename, sizeof (fullpath) - 1);	//build the full file path by appending filename to path
  fp = fopen (fullpath, "r");
  if (fp == NULL)
    {				//if the pointer is null then the file can't be opened - Bad perms OR no such file
      if (debug)
	printf ("Server requested bad file: file not found (%s)\n", fullpath);
      len =
	sprintf ((char *) packetbuf[0], "%c%c%c%cFile not found (%s)%c", 0x00,
		 0x05, 0x00, 0x01, fullpath, 0x00);
      if (sendto (sock, packetbuf[0], len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
	{
	  printf
	    ("Mismatch in number of sent bytes while trying to send error packet\n");
	}
      return;
    }
  else
    {
      if (debug)
	printf ("Sending file... (source: %s)\n", fullpath);

    }
  memset (filebuf, 0, sizeof (filebuf));
  while (1)			/* our break statement will escape us when we are done */
    {
      acked = 0;
      ssize = fread (filebuf, 1, datasize, fp);





      count++;			/* count number of datasize byte portions we read from the file */
      if (count == 1)		/* we always look for an ack on the FIRST packet */
	bcount = 0;
      else if (count == 2)	/* The second packet will always start our counter at zreo. This special case needs to exist to avoid a DBZ when count = 2 - 2 = 0 */
	bcount = 0;
      else
	bcount = (count - 2) % ackfreq;

      sprintf ((char *) packetbuf[bcount], "%c%c%c%c", 0x00, 0x03, 0x00, 0x00);	/* build data packet but write out the count as zero */
      memcpy ((char *) packetbuf[bcount] + 4, filebuf, ssize);
      len = 4 + ssize;
      packetbuf[bcount][2] = (count & 0xFF00) >> 8;	//fill in the count (top number first)
      packetbuf[bcount][3] = (count & 0x00FF);	//fill in the lower part of the count
      if (debug)
	printf ("Sending packet # %04d (length: %d file chunk: %d)\n",
		count, len, ssize);

      if (sendto (sock, packetbuf[bcount], len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
	{
	  if (debug)
	    printf ("Mismatch in number of sent bytes\n");
	  return;
	}


      if ((count - 1) == 0 || ((count - 1) % ackfreq) == 0
	  || ssize != datasize)
	{
/* The following 'for' loop is used to recieve/timeout ACKs */
	  for (j = 0; j < RETRIES; j++)
	    {
	      client_len = sizeof (ack);
	      errno = EAGAIN;
	      n = -1;
	      for (i = 0; errno == EAGAIN && i <= TIMEOUT && n < 0; i++)
		{
		  n =
		    recvfrom (sock, recvbuf, sizeof (recvbuf), MSG_DONTWAIT,
			      (struct sockaddr *) &ack,
			      (socklen_t *) & client_len);
		/*speed up, shnwind modify.*/
		  usleep (1000);
		}
	      if (n < 0 && errno != EAGAIN)
		{
		  if (debug)
		    printf
		      ("The server could not receive from the client (errno: %d n: %d)\n",
		       errno, n);
		  //resend packet
		}
	      else if (n < 0 && errno == EAGAIN)
		{
		  if (debug)
		    printf ("Timeout waiting for ack (errno: %d n: %d)\n",
			    errno, n);
		  //resend packet

		}
	      else
		{

		  #if defined(TCSUPPORT_IPV6_TFTP)
		  if (memcmp(&client.sin6_addr,&ack.sin6_addr,sizeof(struct in6_addr)))	/* checks to ensure send to ip is same from ACK IP */
		  #else
		  if (client.sin_addr.s_addr != ack.sin_addr.s_addr)	/* checks to ensure send to ip is same from ACK IP */
		  #endif
		    {
		      if (debug)
			printf
			  ("Error recieving ACK (ACK from invalid address)\n");
		      j--;	/* in this case someone else connected to our port. Ignore this fact and retry getting the ack */
		      continue;
		    }
			#if defined(TCSUPPORT_IPV6_TFTP)
			if (tid != ntohs (client.sin6_port))	/* checks to ensure get from the correct TID */
			#else
			if (tid != ntohs (client.sin_port))	/* checks to ensure get from the correct TID */
			#endif 
		    {
		      if (debug)
			printf
			  ("Error recieving file (data from invalid tid)\n");
		      len =
			sprintf ((char *) recvbuf,
				 "%c%c%c%cBad/Unknown TID%c", 0x00, 0x05,
				 0x00, 0x05, 0x00);
		      if (sendto (sock, recvbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
			{
			  printf
			    ("Mismatch in number of sent bytes while trying to send mode error packet\n");
			}
		      j--;

		      continue;	/* we aren't going to let another connection spoil our first connection */
		    }

/* this formatting code is just like the code in the main function */
		  bufindex = (char *) recvbuf;	//start our pointer going
		  if (bufindex++[0] != 0x00)
		    printf ("bad first nullbyte!\n");
		  opcode = *bufindex++;

		  rcount = *bufindex++ << 8;
		  rcount &= 0xff00;
		  rcount += (*bufindex++ & 0x00ff);
		  if (opcode != 4 || rcount != count)	/* ack packet should have code 4 (ack) and should be acking the packet we just sent */
		    {
		      if (debug)
			printf
			  ("Remote host failed to ACK proper data packet # %d (got OP: %d Block: %d)\n",
			   count, opcode, rcount);
/* sending error message */
		      if (opcode > 5)
			{
			  len = sprintf ((char *) recvbuf,
					 "%c%c%c%cIllegal operation%c",
					 0x00, 0x05, 0x00, 0x04, 0x00);
			  if (sendto (sock, recvbuf, len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* send the data packet */
			    {
			      printf
				("Mismatch in number of sent bytes while trying to send mode error packet\n");
			    }
			}
		      /* from here we will loop back and resend */
		    }
		  else
		    {
		      if (debug)
			printf ("Remote host successfully ACK'd (#%d)\n",
				rcount);
		      break;
		    }
		}
	      for (i = 0; i <= bcount; i++)
		{
		  if (sendto (sock, packetbuf[i], len, 0, (struct sockaddr *) &client, sizeof (client)) != len)	/* resend the data packet */
		    {
		      if (debug)
			printf ("Mismatch in number of sent bytes\n");
		      return;
		    }
		  if (debug)
		    printf ("Ack(s) lost. Resending: %d\n",
			    count - bcount + i);
		}
	      if (debug)
		printf ("Ack(s) lost. Resending complete.\n");

	    }
/* The ack sending 'for' loop ends here */

	}
      else if (debug)
	{
	  printf ("Not attempting to recieve ack. Not required. count: %d\n",
		  count);
	  n = recvfrom (sock, recvbuf, sizeof (recvbuf), MSG_DONTWAIT, (struct sockaddr *) &ack, (socklen_t *) & client_len);	/* just do a quick check incase the remote host is trying with ackfreq = 1 */

	}

      if (j == RETRIES)
	{
	  if (debug)
	    printf ("Ack Timeout. Aborting transfer\n");
	  fclose (fp);

	  return;
	}
      if (ssize != datasize)
	break;

      memset (filebuf, 0, sizeof (filebuf));	/* fill the filebuf with zeros so that when the fread fills it, it is a null terminated string */
    }

  fclose (fp);
  if (debug)
    printf ("File sent successfully\n");

  return;
}

int
isnotvaliddir (char *pPath)	/* this function just makes sure that the directory passed to the server is valid and adds a trailing slash if not present */
{
  DIR *dp;
  int len;
  dp = opendir (pPath);
  if (dp == NULL)
    {
      return (0);
    }
  else
    {
      len = strlen (pPath);
      closedir (dp);
      if (pPath[len - 1] != '/')
	{
	  pPath[len] = '/';
	  pPath[len + 1] = 0;
	}




      return (1);
    }
}

void
usage (void)			/* prints program usage */
{
  printf
    ("Usage: tftpd [options] [path]\nOptions:\n-d (debug mode)\n-h (help; this message)\n-P <port>\n-a <ack freqency. Default 1>\n-s <data chunk size in bytes. Default 512>\n");
  return;
}
#if 0
int
mtd_open(const char *mtd, int flags)
{
	FILE *fp;
	char dev[PATH_MAX];
	int i;

	if ((fp = fopen("/proc/mtd", "r"))) {
	while (fgets(dev, sizeof(dev), fp)) {
		if (sscanf(dev, "mtd%d:", &i) && strstr(dev, mtd)) {
			snprintf(dev, sizeof(dev), "/dev/mtd%d", i);
			fclose(fp);
			return open(dev, flags);
		}
	}
	fclose(fp);
										}
	return open(mtd, flags);
}

int
image_check_tc(int imagefd, const char *mtd)
{
	struct trx_header *trx = (struct trx_header *) img_buf;
	struct mtd_info_user mtdInfo;
	int fd;

	printf("size: %d\n", sizeof(struct trx_header));
	buflen = read(imagefd, img_buf, sizeof(struct trx_header));
	printf("buflen = %d\n", buflen);
	if (buflen < sizeof(struct trx_header)) {
		fprintf(stdout, "Could not get image header, file too small (%d bytes)\n", buflen);
		return 0;
	}
				
	if(strcmp(mtd, "kernel") == 0)
	{
		if (trx->magic != TRX_MAGIC || trx->len < sizeof(struct trx_header))
		{
			printf("trx->magic = %x\n", trx->magic);
			printf("trx->len = %u\n", trx->len);
			fprintf(stderr, "Bad trx header\n");
			fprintf(stderr, "If this is a firmware in bin format, like some of the\n"
			"original firmware files are, use following command to convert to trx:\n"
			"dd if=firmware.bin of=firmware.trx bs=32 skip=1\n");
			return 0;
												}
		}else 
		{
			if (trx->magic != TRX_MAGIC1 || trx->len < sizeof(struct trx_header))
			{
				fprintf(stderr, "Bad trx header\n");
				fprintf(stderr, "If this is a firmware in bin format, like some of the\n"
				"original firmware files are, use following command to convert to trx:\n"
				"dd if=firmware.bin of=firmware.trx bs=32 skip=1\n");
				return 0;
			}
		}
	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}
	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		exit(1);
	}
				
	if(mtdInfo.size < trx->len) {
		fprintf(stderr, "Image too big for partition: %s\n", mtd);
		close(fd);
		return 0;
	}

	close(fd);
	return 1;
}
#endif
