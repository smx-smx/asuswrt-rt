/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file connection.h
 */
#ifndef __HEADER_CONNECTION__
#define __HEADER_CONNECTION__

#ifdef __ENABLE_SSL__
#include <openssl/ssl.h>
#endif

/*!
 * \struct connection
 * \brief TCP connection abstraction used for HTTP session
 */
struct connection {
    char path[257]; /*!<HTTP request URL path*/
    char host[257]; /*!<The HTTP server host address*/
    char port[10]; /*!<The HTTP server port*/
    int secure; /*!<If or not useing SSL/TLS*/

    int fd; /*!<The connection's socket file descriptor*/
#ifdef __ENABLE_SSL__
    SSL_CTX *ctx;
    SSL *ssl;
#endif
};


/*!
 * \brief Connect to a URL
 *
 * \param conn The connection
 * \param u The URL which can be HTTP or HTTPS, default is HTTP
 *
 * \return 0 when success, -1 when any error
 */
int tr_conn( struct connection *conn, const char *u );

/*!
 * \brief Disconnect the connection
 *
 * \param conn The connection
 */
void tr_disconn( struct connection *conn );

/*!
 * \brief Receive data from a connection
 *
 * \param conn The connection
 * \param buffer The buffer to store the received data
 * \param len How long data to be received
 *
 * \return The bytes number received from peer when success, or -1 when any
 * error occured. If the peer shutdown the connection then return 0
 */
int tr_conn_recv( struct connection *conn, void *buf, int len );

/*!
 * \brief Send data to peer through the connection
 *
 * \param conn The connection
 * \param buf The data buffer
 * \param len The data length to send
 *
 * \return byte number sent to peer when success, or -1 when any error
 * occured.
 */
int tr_conn_send( struct connection *conn, const void *buf, int len );
#endif
