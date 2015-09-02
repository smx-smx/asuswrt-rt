/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file gpva.h
 *
 */
#ifndef __HEADER_GPVA__
#define __HEADER_GPVA__

#include "tr_lib.h"
#include "session.h"
#include "tr.h"

struct gpv_gpa_node {
    node_t node;
#ifdef ALIAS
    char request_name[NODE_PATH_LEN + 1];
#endif //ALIAS
    struct gpv_gpa_node *next;
};

struct gpv_gpa {
    int fault_code;
    int count; /*!Response items count*/
    int which; /*!< 0 means GetParameterValues, other means GetParameterAttributes*/
    struct gpv_gpa_node *nodes;
    struct gpv_gpa_node *next;
};


/*!
 * \brief CPE RPC method callback function to process the GetParameterValues and
 * GetParameterAttributes
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the method successed, METHOD_FAILED on failure.
 */
int gpv_gpa_process( struct session *ss, char **msg );

/*!
 * \brief CPE RPC method callback function to generate the response of GetParameterValues and
 * GetParameterAttributes
 *
 * \param ss The session structure
 *
 * \return  METHOD_COMPLETE if the body is completed, or less return METHOD_MORE_DATA
 */
int gpv_gpa_body( struct session *ss );

/*!
 * \brief CPE RPC method callback function to rewind the GetParameterValuesResponse and
 * GetParameterAttributes body
 *
 * \param ss The session structure
 */
void gpv_gpa_rewind( struct session *ss );

/*!
 * \brief CPE RPC method callback function to destroy the GetParameterValues and
 * GetParameterAttributes processor's private data
 *
 * \param ss The session structure
 */
void gpv_gpa_destroy( struct session *ss );

#endif
