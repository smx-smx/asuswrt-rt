/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file spa.h
 *
 */
#ifndef __HEADER_SPA__
#define __HEADER_SPA__

#include "session.h"


/*!
 * \brief CPE RPC method callback function to process the SetParameterAttributes
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the method successed, METHOD_FAILED on failure.
 */
int spa_process( struct session *ss, char **msg );


/*!
 * \brief CPE RPC method callback function to generate the response of SetParameterAttributes
 *
 * \param ss The session structure
 *
 * \return  METHOD_COMPLETE if the body is completed, or less return METHOD_MORE_DATA
 */
int spa_body( struct session *ss );


/*!
 * \brief CPE RPC method callback function to destroy the SetParameterAttributes processor's
 * private data
 *
 * \param ss The session structure
 */
void spa_destroy( struct session *ss );

#endif
