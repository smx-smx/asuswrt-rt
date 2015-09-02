/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file method.h
 *
 */
#ifndef __HEADER_METHOD__
#define __HEADER_METHOD__

#include "session.h"
#include "buffer.h"

enum {
    FAULT_METHOD_NOT_SUPPORTED,
    FAULT_REQUEST_DENIED,
    FAULT_INTERNAL_ERROR,
    FAULT_INVALID_ARGUMENT,
    FAULT_RESOURCE_EXCEEDED,
    FAULT_INVALID_PARAMETER_NAME
};

enum {
    METHOD_SUCCESSED,
    METHOD_FAILED,
    METHOD_MORE_DATA,
    METHOD_COMPLETE,
    METHOD_RETRY_LATER,
    METHOD_END_SESSION,
    METHOD_RETRANSMISSION
};

struct session;

struct acs_method {
    const char *name;
    int supported; /*!<If or not the server supports it*/
    int ( *process )( struct session *s );
    int ( *body )( struct session *s );
    int ( *success_handler )( struct session *s, char **msg ); /*!<Process the successful response of the method*/
    int ( *fault_handler )( struct session *s, char **msg ); /*!<Process the fault response of the method*/
    void ( *rewind )( struct session *s );
    void ( *destroy )( struct session *s ); /*!<Destroy the private data of the ACS method*/
};

struct cpe_method {
    const char *name;
    int ( *process )( struct session *s, char **msg ); /*!<Process the CPE request from server*/
    int ( *body )( struct session *s );
    void ( *rewind )( struct session *s );
    void ( *destroy )( struct session *s ); /*!<Destroy the private data of the CPE method*/
};

/*!
 * \brief Convert a CPE fault code to string
 *
 * \param code The fault code
 *
 * \return The fault string
 */
const char *fault_code2string( int code );

struct acs_method *get_acs_method_by_name( const char * );
const struct cpe_method *get_cpe_method_by_name( const char * );
const struct cpe_method *get_cpe_method_by_index( int index );
int cpe_methods_count( void );

/*!
 * \brief Reset the ACS supported methods so that we can detecte it once again
 */
void reset_supported_acs_methods( void );

//joyce
int crypt_cpe_method( struct session *ss );
#endif
