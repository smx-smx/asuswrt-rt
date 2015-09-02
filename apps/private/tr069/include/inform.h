/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file inform.h
 *
 */
#ifndef __HEADER_INFORM__
#define __HEADER_INFORM__
#include "session.h"

const char *get_serial_number( void );
/*!
 * \brief Add a parameter to a list, which will be send to ACS in the next session
 *
 * \param path The parameter's path
 * \param type 0 means static parameter, 1 means dynamic(value change) parameter
 *
 * \return 0 on success, -1 on failure
 */
int add_inform_parameter( const char *path, int type );
int __add_inform_parameter( const char *path, int type, const char *value );

/*!
 * \brief A config callback function to add a parameter which will be send in Inform
 * in each session
 *
 * \param name The config item name
 * \param path The parameter's path
 *
 * \return 0 on success, -1 on failure
 */
int add_static_inform_parameter( const char *name, const char *path );

/*!
 * \brief ACS RPC method callback function to do some preprocessing
 *
 * \param ss The TR session structure
 *
 * \return METHOD_SUCCESSED if the method successed, METHOD_RETRY_LATER on failure.
 */
int inform_process( struct session *ss );


/*!
 * \brief ACS RPC method callback function to generate the Inform request
 *
 * \param ss The session structure
 *
 * \return  METHOD_COMPLETE if the body is completed, or less return METHOD_MORE_DATA
 */
int inform_body( struct session *ss );

/*!
 * \brief ACS RPC method callback function to process the Inform fault response
 *
 * \param ss The session structure
 * \param msg The left part of the SOAP message received from ACS
 *
 * \return  METHOD_RETRANSMISSION if received a 8005 fault code, or less return METHOD_RETRY_LATER
 */
int inform_fault_handler( struct session *ss, char **msg );

/*!
 * \brief ACS RPC method callback function to process the Inform response. It will delete all
 * committed events and dynamic parameters
 *
 * \param ss The session structure
 * \param msg The left part of the SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED
 */
int inform_success_handler( struct session *ss, char **msg );

/*!
 * \brief ACS RPC method callback function to rewind the Inform request so that the agent can
 * restruct it again
 *
 * \param ss The session structure
 */
void inform_rewind( struct session *ss );
#endif
