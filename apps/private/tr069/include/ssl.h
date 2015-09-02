/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file ssl.h
 *
 */
#ifndef __HEADER_SSL__
#define __HEADER_SSL__
#ifdef __ENABLE_SSL__

#include "connection.h"

int set_ssl_config( const char *name, const char *value );
void destroy_ssl_connection( struct connection *conn );
int setup_ssl_connection( struct connection *conn );

#endif
#endif
