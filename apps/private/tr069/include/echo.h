/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file echo.h
 *
 */
#ifndef __HEADER_ECHO__
#define __HEADER_ECHO__

#ifdef TR143


/*!
 * \brief Launch the echo server to ready receive data from any address
 */
int launch_echo_server( void );

/*!
 * \brief Convert time into a special format required by TR143
 *
 * \param tv The time to be converted
 *
 * \return Return a static string.
 */
const char *echo_current_time( struct timeval *tv );
#endif /*TR143*/

#endif/*__HEADER_ECHO__*/
