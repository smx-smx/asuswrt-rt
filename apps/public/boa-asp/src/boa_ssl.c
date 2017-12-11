#if defined(TCSUPPORT_WEBSERVER_SSL)
#include "boa.h"
#include <shutils.h>
#include "shared.h"
#include "libtcapi.h"
#define SSL_KEYF "/etc/key.pem"
#define SSL_CERTF "/etc/cert.pem"
#define SSL_CERT_GEN	"/usr/script/gencert.sh"
//#define SSL_FILETYPE_PEM 1

int server_ssl;				/*ssl socket */
SSL_CTX *ssl_ctx = NULL;	//Andy Chiu, 2015/02/09. for store key information.
int ssl_server_port = 8443;		/*The port that the server should listen on*/
int ssl_pending_requests = 0; 
#ifdef INET6
struct sockaddr_in6 ssl_server_sockaddr;		/* boa ssl socket address */
#else
struct sockaddr_in ssl_server_sockaddr;		/* boa ssl socket address */
#endif
static int sock_opt = 1;

int  boa_sslInit(void)
{
	tcdbg_printf( "[%s, %d]Generate new key.\n", __FUNCTION__, __LINE__);
	unsigned long long sn;
	char t[32];

	if(access(SSL_KEYF, F_OK) || access(SSL_CERTF, F_OK))
	{
		f_read("/dev/urandom", &sn, sizeof(sn));
		sprintf(t, "%llu", sn & 0x7FFFFFFFFFFFFFFFULL);
		eval(SSL_CERT_GEN, t);
	}

	tcdbg_printf( "[%s, %d]Enabling SSL security system\n", __FUNCTION__, __LINE__);
	if ((server_ssl = socket(SERVER_AF, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		tcdbg_printf("[%s, %d]Couldn't create socket for ssl\n", __FUNCTION__, __LINE__);
		DIE("Couldn't create socket for ssl");
	}
	/* server socket is nonblocking */
	
	if (fcntl(server_ssl, F_SETFL, NOBLOCK) == -1)
	{
		tcdbg_printf("[%s, %d]Couldn't fcnt\n", __FUNCTION__, __LINE__);
		DIE("Couldn't fcntl");
	}

	if ((setsockopt(server_ssl, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,sizeof(sock_opt))) == -1)
	{
		tcdbg_printf("[%s, %d]Couldn't sockopt\n", __FUNCTION__, __LINE__); 
		DIE("Couldn't sockopt");
	}
	/* internet socket */
#ifdef INET6
	ssl_server_sockaddr.sin6_family = AF_INET6;
	memcpy(&ssl_server_sockaddr.sin6_addr,&in6addr_any,sizeof(in6addr_any));
	ssl_server_sockaddr.sin6_port = htons(ssl_server_port);
#else
	ssl_server_sockaddr.sin_family = AF_INET;
	ssl_server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ssl_server_sockaddr.sin_port = htons(ssl_server_port);
#endif
	if (bind(server_ssl, (struct sockaddr *) &ssl_server_sockaddr,sizeof(ssl_server_sockaddr)) == -1)
	{
		tcdbg_printf("[%s, %d]Couldn't bind ssl to port\n", __FUNCTION__, __LINE__); 
		DIE("Couldn't bind ssl to port");
	}

	/* listen: large number just in case your kernel is nicely tweaked */
	if (listen(server_ssl, backlog) == -1)
	{
		tcdbg_printf("[%s, %d]Couldn't listen\n", __FUNCTION__, __LINE__); 	
		DIE("Couldn't listen");
	}

	if(mssl_init(SSL_CERTF, SSL_KEYF, &ssl_ctx) == -1)
	{
		tcdbg_printf("[%s, %d]Couldn't Init SSL Library!\n", __FUNCTION__, __LINE__);
		DIE("\nCouldn't Init SSL Library!");
	}

	/*load and check that the key files are appropriate.*/
	tcdbg_printf("[%s, %d]SSL security system enabled\n", __FUNCTION__, __LINE__);
	return 1;
}

void  boa_sslUninit()
{
	//tcdbg_printf("[%s, %d]\n", __FUNCTION__, __LINE__);
	mssl_deinit(ssl_ctx);
}

void boa_sslWait (int fd,int mode)
{
	fd_set tmp_write_fdset,tmp_read_fdset;
	struct timeval tmp_timeout={1,0};
	int ret=0;

	while (1)
	{
		FD_ZERO(&tmp_read_fdset);
		FD_ZERO(&tmp_write_fdset);
		if(mode == 1)
		{
			FD_SET (fd, &tmp_write_fdset);
			ret=select(fd + 1, NULL, &tmp_write_fdset, NULL, &tmp_timeout);
		}
		else
		{
			FD_SET (fd, &tmp_read_fdset);
			ret=select(fd + 1, &tmp_read_fdset, NULL, NULL, &tmp_timeout);
		}
            	if (ret <0) 
		{
            		if (errno == EINTR)
			{
              		  	continue;   
            		}
        	}
		if(ret==0)
		{
			break;
		}
		if (FD_ISSET(fd, &tmp_read_fdset))
		{
			break;
		}
		if (FD_ISSET(fd, &tmp_write_fdset))
		{
			break;
		}
	}
}


int boa_sslRead(mssl_conn_t *conn, char *buf, const int len)
{
	//tcdbg_printf("[%s, %d]len(%d)\n", __FUNCTION__, __LINE__, len);	
	int ret = 0;

	do{
		boa_sslWait(conn->sd, 0);
		ret = mssl_read(conn, buf, len);
	}while(ret == -2);
	//tcdbg_printf("[%s, %d]ret(%d)\n", __FUNCTION__, __LINE__, ret);
	return ret;
}

//#define BOA_SSL_MAXBUF 8192
int boa_sslAccept(mssl_conn_t **conn, const int fd, SSL_CTX *ctx)
{
	//tcdbg_printf("[%s, %d]\n", __FUNCTION__, __LINE__);
	int ret = mssl_server_fopen(fd, conn, ctx);
	//tcdbg_printf("[%s, %d]ret(%d)\n", __FUNCTION__, __LINE__, ret);
	return ret;
}

int boa_sslWrite(mssl_conn_t *conn, char *buf, const int buflen)
{
	int ret = 0;
	
	//tcdbg_printf("[%s, %d]\n", __FUNCTION__, __LINE__);
	do{
		boa_sslWait(conn->sd, 1);
		ret = mssl_write(conn, buf,  buflen);		
	}while(ret == -2);
	
	//tcdbg_printf("[%s, %d]sd(%d), ret(%d)\n", __FUNCTION__, __LINE__, conn->sd,  ret);
	return ret;
}

int boa_sslClose(mssl_conn_t *conn)
{
	//tcdbg_printf("[%s, %d]\n", __FUNCTION__, __LINE__);
	int ret = mssl_close(conn);
	//tcdbg_printf("[%s, %d]ret(%d)\n", __FUNCTION__, __LINE__, ret);
	return ret;
}
#endif
