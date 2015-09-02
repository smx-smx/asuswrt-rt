/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file network.h
 *
 */
#ifndef __HEADER_NETWORK__
#define __HEADER_NETWORK__

//#include "war_os.h"
#include "war_type.h"

#include "tr.h"

/*!
 * \brief A wrapper of connect() system API
 *
 * \param socket The socket file descriptor
 * \param address The server address
 * \param address_len The length of the server address(\a address)
 *
 * \return 0 on success, -1 on failure
 */
int tr_connect( int socket, const struct sockaddr *address, socklen_t address_len );

/*!
 * \brief Set a socket file descriptor to non-block mode
 *
 * \param fd The socket file descriptor
 *
 * \note Any socket that will be maintained by scheduler MUST be in non-mode,
 * or less the application will be blocked.
 */
void nonblock_socket( int fd );
/*!
 * \brief Set a socket file descriptor to block mode
 *
 * \param fd The socket file descriptor
 *
 * \note use ssl conection must set socket block mode
 */
void block_socket( int fd );


/*!
 * \brief Create a server socket to listen a address + port
 *
 * \param addr The local address to be binded to the server socket, if it is NULL,
 * the INADDR_ANY(0.0.0.0) will be used
 * \param port The port will be listened to
 * \param type The type(SOCK_DGRAM or SOCK_STREAM) of the socket
 * \param backlog Only used for SOCK_STREAM socket, it defines the maxnium length
 * to which the queue of pending connections for the socket may grows.
 *
 * \return 0 on success, -1 on failure
 */
int tr_listen( const char *addr, uint16_t port, int type, int backlog );

#endif
