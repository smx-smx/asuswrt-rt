/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file gpn.h
 *
 */
#ifndef __HEADER_GPN__
#define __HEADER_GPN__

#include "session.h"


/*!
 * \brief CPE RPC method callback function to process the GetParameterNames
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the method successed, METHOD_FAILED on failure.
 */
int gpn_process( struct session *ss, char **msg );

/*!
 * \brief CPE RPC method callback function to generate the response of GetParameterNames
 *
 * \param ss The session structure
 *
 * \return  METHOD_COMPLETE if the body is completed, or less return METHOD_MORE_DATA
 */
int gpn_body( struct session *ss );

/*!
 * \brief CPE RPC method callback function to rewind the GetParameterNamesResponse body
 *
 * \param ss The session structure
 */
void gpn_rewind( struct session *ss );

/*!
 * \brief CPE RPC method callback function to destroy the GetParameterNames processor's
 * private data
 *
 * \param ss The session structure
 */
void gpn_destroy( struct session *ss );

#endif
