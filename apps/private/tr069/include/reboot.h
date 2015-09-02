/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file reboot.h
 *
 */
#ifndef __HEADER_REBOOT__
#define __HEADER_REBOOT__

#include "session.h"

/*!
 * \brief CPE RPC method callback function to process the Reboot
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the method successed, METHOD_FAILED on failure.
 */
int reboot_process( struct session *ss, char **msg );


/*!
 * \brief CPE RPC method callback function to destroy the Reboot processor's
 * private data
 *
 * \param ss The session structure
 */
void reboot_destroy( struct session *ss );

#endif
