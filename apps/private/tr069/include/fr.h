/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file fr.h
 */
#ifndef __HEADER_FR__
#define __HEADER_FR__

#include "session.h"

/*!
 * \brief Process the FactoryReset RPC method from ACS
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the FactoryReset operation successed, METHOD_FAILED if the
 * AddObject operation if failed.
 */
int fr_process( struct session *ss, char **msg );

/*!
 * \brief callback function to destroy the factory reset method related private data
 * \param sc The TR sesion structure
 */
void fr_destroy( struct session *ss );

#endif
