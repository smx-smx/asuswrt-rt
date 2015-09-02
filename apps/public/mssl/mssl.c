/*

	Minimal CyaSSL/OpenSSL Helper
	Copyright (C) 2006-2009 Jonathan Zarate
	Copyright (C) 2010 Fedor Kozhevnikov

	Licensed under GNU GPL v2 or later

*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include "mssl.h"

//#define ssl_dbgprintf(args...)	while (0) {}

//static SSL_CTX* ctx;
/*******************************************************************
* NAME: ssl_dbgprintf
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: print debug message.
* INPUT:  
* OUTPUT:  
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
void ssl_dbgprintf (const char * format, ...)
{
	//return;
	FILE *dbg = fopen("/dev/console", "w");
	if(dbg)
	{
		va_list args;
		va_start (args, format);
		vfprintf (dbg, format, args);
		va_end (args);
		fclose(dbg);
	}
}

/*******************************************************************
* NAME: 
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: 
* INPUT:  
* OUTPUT:  
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
static inline void mssl_print_err(SSL* ssl)
{
	ERR_print_errors_fp(stderr);
}

/*******************************************************************
* NAME: mssl_cleanup
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: free ctx
* INPUT:  err: integer.errno.
*             ctx: the pointer of SSL_CTX.
* OUTPUT:  
* RETURN:
* NOTE:
*******************************************************************/
static inline void mssl_cleanup(const int err, SSL_CTX* ctx)
{
	if (err) mssl_print_err(NULL);
	SSL_CTX_free(ctx);
//	ctx = NULL;
}

/*******************************************************************
* NAME: mssl_read
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: read data in mssl connection
* INPUT:  conn: pointer of mssl_conn_t
*             buf: char array. data for writting.
*             len: length of buf
* OUTPUT:  
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
ssize_t mssl_read(const mssl_conn_t *conn, char *buf, const size_t len)
{
	//ssl_dbgprintf("%s()\n", __FUNCTION__);

	if(!conn || !buf || !len)
	{
		ssl_dbgprintf("[%s]parameter error!\n", __FUNCTION__);
		return -1;
	}
	
	const mssl_conn_t *kuki = conn;
	int total = 0;
	int n, err;

	//int cnt = 0;
	
	do{
		n = SSL_read(kuki->ssl, buf + total, len - total);
		//ssl_dbgprintf("SSL_read(max=%d) returned %d\n", len - total, n);

		//++cnt;
		
		err = SSL_get_error(kuki->ssl, n);
		switch (err) {
		case SSL_ERROR_NONE:
			total += n;
			break;
		case SSL_ERROR_ZERO_RETURN:
			total += n;
			goto OUT;
		case SSL_ERROR_WANT_WRITE:
			//ssl_dbgprintf("%s():SSL_ERROR_WANT_WRITE(%d)\n", __FUNCTION__, cnt);
			break;
		case SSL_ERROR_WANT_READ:
			//ssl_dbgprintf("%s():SSL_ERROR_WANT_READ(%d)\n", __FUNCTION__, cnt);
			total = -2;
			break;
			//continue;
		default:
			//ssl_dbgprintf("%s(): SSL error %d, %d, %d, %u\n", __FUNCTION__, err, errno, n, ERR_get_error());
			mssl_print_err(kuki->ssl);
			if (total == 0) total = -1;
			goto OUT;
		}
	} while ((len - total > 0) && SSL_pending(kuki->ssl));

OUT:
	//ssl_dbgprintf("%s() returns %d\n", __FUNCTION__, total);
	return total;
}

/*******************************************************************
* NAME: mssl_write
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: write data in mssl connection
* INPUT:  conn: pointer of mssl_conn_t
*             buf: char array. data for writting.
*             len: length of buf
* OUTPUT:  
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
ssize_t mssl_write(const mssl_conn_t *conn, const char *buf, const size_t len)
{
	//ssl_dbgprintf("%s()\n", __FUNCTION__);

	const mssl_conn_t *kuki = conn;
	int total = 0;
	int n, err;
	if(!conn || !buf || !len)
	{
		ssl_dbgprintf("[%s]parameter error!\n", __FUNCTION__);
		return -1;
	}

	//int cnt = 0;

	while (total < len) {
		n = SSL_write(kuki->ssl, &(buf[total]), len - total);
		//ssl_dbgprintf("SSL_write(max=%d) returned %d\n", len - total, n);
		err = SSL_get_error(kuki->ssl, n);
		//++cnt;
		switch (err) {
		case SSL_ERROR_NONE:
			total += n;
			break;
		case SSL_ERROR_ZERO_RETURN:
			total += n;
			goto OUT;
		case SSL_ERROR_WANT_WRITE:
			//ssl_dbgprintf("%s():SSL_ERROR_WANT_WRITE(%d)\n", __FUNCTION__, cnt);
			total = -2;
			break;
			//continue;
		case SSL_ERROR_WANT_READ:
			//ssl_dbgprintf("%s():SSL_ERROR_WANT_READ(%d)\n", __FUNCTION__, cnt);
			break;
			//continue;
		default:
			//ssl_dbgprintf("%s(): SSL error %d, <%d>\n", __FUNCTION__, err, total);
			mssl_print_err(kuki->ssl);
			total = -1;
			//if (total == 0) total = -1;
			goto OUT;
		}
	}

OUT:
	//ssl_dbgprintf("%s() returns %d\n", __FUNCTION__, total);
	return total;
}

/*******************************************************************
* NAME: mssl_close
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: close mssl connection
* INPUT:  conn: pointer of mssl_conn_t
* OUTPUT:  
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
int mssl_close(const mssl_conn_t *conn)
{
	//ssl_dbgprintf("%s()\n", __FUNCTION__);

	const mssl_conn_t *kuki = conn;
	if (!kuki) return -1;

	if (kuki->ssl) {
		SSL_shutdown(kuki->ssl);
		SSL_free(kuki->ssl);
	}
	close(kuki->sd);

	free(conn);
	return 0;
}


/*******************************************************************
* NAME: _ssl_fopen
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: open ssl comnnection
* INPUT:  sd: socket fd.
*             client: 0:server, 1:client
*             ctx: the pointer of SSL_CTX
* OUTPUT:  conn: mssl_conn_t 
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
static int _ssl_fopen(const int sd, const int client, mssl_conn_t *conn, SSL_CTX* ctx)
{
	int r = 0;
	int err;
	mssl_conn_t *kuki = conn;

	//ssl_dbgprintf("[%s, %d]<%d, %d, %x, %x>\n", __FUNCTION__, __LINE__, sd, client, conn, ctx);
	//fprintf(stderr,"[ssl_fopen] ssl_fopen start!\n"); // tmp test
	if(!conn)
	{
		ssl_dbgprintf("[%s]parameter error!\n", __FUNCTION__);
		return -1;
	}

	kuki->sd = sd;

	// Create new SSL object
	if ((kuki->ssl = SSL_new(ctx)) == NULL) {
		fprintf(stderr,"[ssl_fopen] SSL_new failed!\n"); // tmp test
		ssl_dbgprintf("%s: SSL_new failed\n", __FUNCTION__);
		goto ERROR;
	}

	//ssl_dbgprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
	// SSL structure for client authenticate after SSL_new()
	//SSL_set_verify(kuki->ssl, SSL_VERIFY_NONE, NULL);
	SSL_set_mode(kuki->ssl, SSL_MODE_AUTO_RETRY);

	//ssl_dbgprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
	// Bind the socket to SSL structure
	// kuki->ssl : SSL structure
	// kuki->sd  : socket_fd

	r = SSL_set_fd(kuki->ssl, kuki->sd);
	//fprintf(stderr,"[ssl_fopen] set_fd=%d\n", r); // tmp test

	if (!client){
		// Do the SSL Handshake
		//ssl_dbgprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
		r = SSL_accept(kuki->ssl);
		err = SSL_get_error(kuki->ssl, r);
	}
	else{
		// Connect to the server, SSL layer
		//ssl_dbgprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
		r = SSL_connect(kuki->ssl);
		err = SSL_get_error(kuki->ssl, r);
	}

	struct timeval tv_timeout;
	int maxfd;
	fd_set my_fdset;
	int sel_ret;
	
	while(r != 1 && 
		(err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE))
	{
		//TODO:select socket

		tv_timeout.tv_sec =  1;
		tv_timeout.tv_usec = 0;

		FD_ZERO(&my_fdset);
		FD_SET(sd, &my_fdset);
		maxfd = sd;
		
		sel_ret = select(maxfd + 1, &my_fdset, &my_fdset, NULL, &tv_timeout);
		
		if(sel_ret > 0)
		{
			//ssl_dbgprintf("[%s, %d]r(%d)\n", __FUNCTION__, __LINE__, r);
			if (!client){
				// Do the SSL Handshake
				//ssl_dbgprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
				r = SSL_accept(kuki->ssl);
				err = SSL_get_error(kuki->ssl, r);
			}
			else{
				// Connect to the server, SSL layer
				//ssl_dbgprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
				r = SSL_connect(kuki->ssl);
				err = SSL_get_error(kuki->ssl, r);
			}

			//ssl_dbgprintf("[%s, %d]r(%d)\n", __FUNCTION__, __LINE__, r);
			//fprintf(stderr,"[ssl_fopen] client=%d, r=%d\n", client, r); // tmp test
			// r = 0 show unknown CA, but we don't have any CA, so ignore.
			if (r < 0){
				// Check error in connect or accept
	//			fprintf(stderr,"[ssl_fopen] SSL error #%d in client=%d\n", err, client); // tmp test
	//			mssl_print_err(kuki->ssl);
				if(err != SSL_ERROR_WANT_READ && err != SSL_ERROR_WANT_WRITE)
					ssl_dbgprintf("[%s, %d]SSL error #%d in client=%d\n", __FUNCTION__, __LINE__, err, client);
	//			goto ERROR;
			}
		}
		else if(!sel_ret)
		{
			//ssl_dbgprintf("[%s, %d]select timeout.\n", __FUNCTION__, __LINE__);
			err = SSL_ERROR_WANT_READ;
		}
		else
			err = SSL_ERROR_SSL;
	}
	//ssl_dbgprintf("SSL connection using %s cipher\n", SSL_get_cipher(kuki->ssl));

	//fprintf(stderr,"[ssl_fopen] SUCCESS!\n", r); // tmp test
	//ssl_dbgprintf("%s() success\n", __FUNCTION__);
	
	return 0;

ERROR:
	fprintf(stderr,"[ssl_fopen] ERROR!\n"); // tmp test
	mssl_close(kuki);
	return -1;
}

/*******************************************************************
* NAME: mssl_server_fopen
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: open ssl server connection
* INPUT:  sd: socket fd.
*             ctx: the pointer of SSL_CTX
* OUTPUT:  conn: mssl_conn_t would be alloc and retuen
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
int mssl_server_fopen(const int sd, mssl_conn_t **conn, SSL_CTX* ctx)
{
	//ssl_dbgprintf("%s()\n", __FUNCTION__);
	if(!ctx)
		return -1;
	mssl_conn_t *new_conn = malloc(sizeof(mssl_conn_t));
	int ret = -1;
	if(!new_conn)
	{
		ssl_dbgprintf("%s()mssl_conn_t memory alloc failed\n", __FUNCTION__);
		return -1;
	}
	ret = _ssl_fopen(sd, 0, new_conn, ctx);
	if(!ret)
		*conn = new_conn;
	else
	{
		ssl_dbgprintf("%s()open socket failed.\n", __FUNCTION__);
		free(new_conn);
	}
	return ret;
}

/*******************************************************************
* NAME: mssl_client_fopen
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: open ssl client connection
* INPUT:  sd: socket fd.
*             ctx: the pointer of SSL_CTX
* OUTPUT:  conn: mssl_conn_t would be alloc and retuen
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
int mssl_client_fopen(const int sd, mssl_conn_t **conn, SSL_CTX* ctx)
{
	//ssl_dbgprintf("%s()\n", __FUNCTION__);
	if(!ctx)
		return -1;
	mssl_conn_t *new_conn = malloc(sizeof(mssl_conn_t));
	int ret = -1;
	if(!new_conn || !ctx)
		return -1;
	ret = _ssl_fopen(sd, 1, new_conn, ctx);
	if(!ret)
		*conn = new_conn;
	else
		free(new_conn);
	return ret;
}

/*******************************************************************
* NAME: mssl_init
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: init ssl ctx by cert and key.
* INPUT:  cert: the certificate file path. 
*             key: the private file path. 
* OUTPUT:  ctx: SSL_CTX would be alloc and return
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
int mssl_init(const char *cert, const char *priv, SSL_CTX** ctx)
{
	ssl_dbgprintf("%s()\n", __FUNCTION__);
	SSL_CTX *new_ctx = NULL;
	if(!cert || !priv)
	{
		ssl_dbgprintf("%s(), parameter invalid\n", __FUNCTION__);
		return -1;
	}	

	int server = (cert != NULL);
	
	//fprintf(stderr,"[ssl_init] server=%d\n", server); // tmp test

	OpenSSL_add_all_algorithms();
	// Register error strings for libcrypto and libssl functions
	SSL_load_error_strings();
	//SSLeay_add_ssl_algorithms();
	SSL_library_init();


	// Create the new CTX with the method 
	// If server=1, use TLSv1_server_method() or SSLv23_server_method()
	// else 	use TLSv1_client_method() or SSLv23_client_method()
	//new_ctx = SSL_CTX_new(server ? SSLv23_server_method() : SSLv23_client_method()); // SSLv23 for IE
	//new_ctx = SSL_CTX_new(TLSv1_server_method()); // SSLv23 for IE
	new_ctx = SSL_CTX_new(server ? TLSv1_server_method() : TLSv1_client_method()); // SSLv23 for IE
	
	if (!new_ctx) {
		fprintf(stderr,"[ssl_init] SSL_CTX_new() failed\n"); // tmp test
		ssl_dbgprintf("SSL_CTX_new() failed\n");
		mssl_print_err(NULL);
		return -1;
	}

	SSL_CTX_set_options(new_ctx, SSL_OP_NO_SSLv2 );
	SSL_CTX_set_verify(new_ctx, SSL_VERIFY_NONE, NULL);
	SSL_CTX_set_verify_depth(new_ctx, 1 );

	if (server) {
		// Set the certificate to be used
		//ssl_dbgprintf("SSL_CTX_use_certificate_file(%s)\n", cert);
		if (SSL_CTX_use_certificate_file(new_ctx, cert, SSL_FILETYPE_PEM) != 1) {
			ssl_dbgprintf("SSL_CTX_use_certificate_file() failed\n");
			mssl_cleanup(1, new_ctx);
			return -1;
		}
		// Indicate the key file to be used
		//ssl_dbgprintf("SSL_CTX_use_PrivateKey_file(%s)\n", priv);
		if (SSL_CTX_use_PrivateKey_file(new_ctx, priv, SSL_FILETYPE_PEM) != 1) {
			ssl_dbgprintf("SSL_CTX_use_PrivateKey_file() failed\n");
			mssl_cleanup(1, new_ctx);
			return -1;
		}
		// Make sure the key and certificate file match
		if (SSL_CTX_check_private_key(new_ctx) != 1) {
			ssl_dbgprintf("Private key does not match the certificate public key\n");
			mssl_cleanup(0, new_ctx);
			return -1;
		}
	}

	*ctx = new_ctx;
	fprintf(stderr,"[ssl_init] success!!\n"); // tmp test
	ssl_dbgprintf("%s() success\n", __FUNCTION__);
	return 0;
}

/*******************************************************************
* NAME: mssl_deinit
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/02/11
* DESCRIPTION: free ctx. after calling this function, DO NOT use ctx anymore.
* INPUT:  ctx: pointer of SSL_CTX
* OUTPUT:  
* RETURN:  
* NOTE:
*******************************************************************/
void mssl_deinit(SSL_CTX* ctx)
{
	mssl_cleanup(0, ctx);
}

