/*

	Minimal MatrixSSL Helper
	Copyright (C) 2006-2009 Jonathan Zarate

	Licensed under GNU GPL v2 or later.

*/
#ifndef __MSSL_H__
#define __MSSL_H__

#include <openssl/ssl.h>

typedef struct {
	SSL* ssl;	//ssl connection.
	int sd;	//socket id
} mssl_conn_t;


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
extern int mssl_server_fopen(const int sd, mssl_conn_t **conn, SSL_CTX* ctx);

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
extern int mssl_client_fopen(const int sd, mssl_conn_t **conn, SSL_CTX* ctx);

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
extern int mssl_init(const char *cert, const char *priv, SSL_CTX** ctx);

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
extern void mssl_deinit(SSL_CTX* ctx);

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
extern ssize_t mssl_read(const mssl_conn_t *conn, char *buf, const size_t len);

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
extern ssize_t mssl_write(const mssl_conn_t *conn, const char *buf, const size_t len);

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
extern int mssl_close(const mssl_conn_t *conn);


#endif
