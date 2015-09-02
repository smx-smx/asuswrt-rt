/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file request.h
 *
 */
#ifndef __HEADER_REQUEST__
#define __HEADER_REQUEST__

#include "method.h"

/*!
 * \brief The ACS request structure
 */
struct request {
    char method_name[32]; /*!<The method name*/
    struct {
        int event_code;
        char cmd_key[33];
    } event; /*!<The method related event*/
    int data_len; /*!<The request body*/
    struct request *next;
};


/*!
 * \brief Load the request from file
 *
 * \return Always return 0
 */
int load_request( void );

/*!
 * \brief Fetch the next pending ACS request's processor
 *
 * \return The first ACS request's processor if find any one, or less return NULL
 */
const struct acs_method *next_acs_method( int reboot );

/*!
 * \brief Fetch a request by name and related event
 *
 * \param name The ACS request name
 * \param event_code The event code, a minus number means the request has no related
 * event.
 * \param cmd_key The command key, a NULL or EMPTY string means the request's related
 * event has no command key.
 *
 * \return The found request or NULL
 */
struct request *get_request( const char *name, int event_code, const char *cmd_key );

/*!
 * \brief Add an ACS to the requests list wich will be sent to ACS in the next session
 *
 * \param method_name The ACS request's processor name
 * \param event_code The event code
 * \param cmd_key The event's command key
 * \param data The request's body
 *
 * \return 0 on success, -1 on failure
 */
int add_request( const char *method_name, int event_code, const char *cmd_key, const char *data );

/*!
 * \brief Delete a request from the requests list
 *
 * \param r The request to be deleted
 *
 * \return Always return 0
 */
int del_request( struct request *r );

#endif
