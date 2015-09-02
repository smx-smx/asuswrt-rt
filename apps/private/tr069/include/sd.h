/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file sd.h
 *
 */
#ifndef __HEADER_SD__
#define __HEADER_SD__

#include "session.h"

/*!
 * \brief CPE RPC method callback function to process the ScheduleDownload
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the method successed, METHOD_FAILED on failure.
 */
int sd_process( struct session *ss, char **msg );

/*!
 * \brief CPE RPC method callback function to generate the response of ScheduleDownload
 *
 * \param ss The session structure
 *
 */
int sd_body( struct session *ss );

/*!
 * \brief CPE RPC method callback function to destroy the ScheduleDownload processor's
 * private data
 *
 * \param ss The session structure
 */
void sd_destroy( struct session *ss );

#endif
