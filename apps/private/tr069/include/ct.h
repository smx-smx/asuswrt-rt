/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file ct.h
 *
 */
#ifndef __HEADER_CT__
#define __HEADER_CT__

#include "session.h"

/*!
 * \brief CPE RPC method callback function to process the CancelTransfer
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the method successed, METHOD_FAILED on failure.
 */
int ct_process( struct session *ss, char **msg );

#endif
