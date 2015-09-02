/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file grm.h
 *
 */
#ifndef __HEADER_GRM__
#define __HEADER_GRM__

#include "session.h"

struct xml;
int get_num_from_attrs( struct xml *tag );
/*!
 * \brief CPE RPC method callback function to generate the response of GetRPCMethods
 *
 * \param ss The session structure
 *
 * \return  METHOD_COMPLETE if the body is completed, or less return METHOD_MORE_DATA
 */
int cpe_grm_body( struct session *ss );

#endif
