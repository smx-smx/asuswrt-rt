/*=======================================================================
  
       Copyright(c) 2009, Works Systems, Inc. All rights reserved.
  
       This software is supplied under the terms of a license agreement 
       with Works Systems, Inc, and may not be copied nor disclosed except 
       in accordance with the terms of that agreement.
  
  =======================================================================*/
#ifndef __WAR_SOCKET_H
#define __WAR_SOCKET_H

#include "war_type.h"

int war_loadsock ();
int war_getsockopt(int s, int level, int optname, void *optval, int *optlen);
int war_setsockopt( int s, int level, int optname, const void *optval, int optlen);
int war_ioctl( int d, ioctl_req_t request, ioctl_argp_t *argp);
int war_socket( int domain, int type, int protocol);
int war_sockclose(int fd);
struct hostent * war_gethostbyname (const char *name);
ssize_t war_recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
int war_connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);
int war_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
ssize_t war_send(int s, const void *buf, size_t len, int flags);
int war_getsockname(int s, struct sockaddr *name, socklen_t *namelen);

#endif
