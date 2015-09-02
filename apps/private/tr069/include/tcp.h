/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file tcp.h
 *
 */
#ifndef __HEADER_TCP__
#define __HEADER_TCP__

#include "tr.h"

int set_tcp_config( const char *name, const char *value );

int launch_tcp_listener( void );

#endif
