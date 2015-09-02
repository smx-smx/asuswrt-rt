/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file udp.h
 *
 */
#ifdef TR111

#ifndef __DEVICE_IPV6__

#ifndef __HEADER_UDP__
#define __HEADER_UDP__

int set_udp_config( const char *name, const char *value );
int launch_udp_listener( void );

#endif

#endif /*__DEVICE_IPV6__*/

#endif /*TR111*/
