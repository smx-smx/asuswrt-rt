/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file session.h
 *
 */
#ifndef __HEADER_SESSION__
#define __HEADER_SESSION__

#include "sched.h"
#include "buffer.h"
#include "http.h"
#include "method.h"
#include "connection.h"
#include "network.h"
#include "war_string.h"

enum {
    CHALLENGE_NONE,
    CHALLENGE_BASIC,
    CHALLENGE_DIGEST
};

enum {
    NEXT_STEP_HTTP_HEADER,
    NEXT_STEP_SOAP_HEADER,
    NEXT_STEP_SOAP_BODY,
    NEXT_STEP_SOAP_TAIL,
    NEXT_STEP_NEXT_MSG,
    NEXT_STEP_REAL_NEXT_MSG
};

struct bucket {
    int data_len;
    char *data;
    struct bucket *next;
};

struct session {
    struct http http;
    struct connection conn;

    char id[128];
    void *acs_pdata;
    const struct acs_method *acs;
    void *cpe_pdata;
    const struct cpe_method *cpe;

    unsigned int reboot: 1;
    unsigned int factory_reset: 1;
    unsigned int diagnostic: 1;

    unsigned int hold: 1;
    unsigned int cpe_result: 2;
    unsigned int version: 3;
    unsigned int received_empty: 1;
    unsigned int sent_empty: 1;
    unsigned int next_step: 3;
    unsigned int redirect_count: 3;
    //Used for HTTP authentication
    unsigned int challenged: 1;
    unsigned int continue_100: 1;
    unsigned int successed: 1;
    int retransmission_count;
    char url[257];

    int offset;
    struct buffer outbuf;
    int bucket_len;
    struct bucket *last_bucket;
    struct bucket *buckets;
};

int push_soap( struct session *s, const char *fmt, ... );

int cwmp_enabled( void );

/*!
 * \brief Notify the session that something happened and need to reboot the device
 *
 * \return 0 if the agent is in session at the moment, -1 if not.
 */
int need_reboot_device( void );
void need_factory_reset( void );

int create_session( void );

/*!
 * \brief The config callback function
 */
int set_session_config( const char *name, const char *value );

void ref_session_lock( void );
void unref_session_lock( void );

#ifdef GET_NODE_VALUE
#error The GET_NODE_VALUE redefined
#else
#define GET_NODE_VALUE(path, buf) do { \
    char *v = NULL; \
        node_t node; \
        res |= lib_resolve_node(path, &node); \
        if(res == 0) { \
            res = lib_get_value(node, &v); \
            if(res == 0) { \
                if(v) { \
                    snprintf(buf, sizeof(buf), "%s", v); \
        } \
            } \
        } \
    if(v) \
        lib_destroy_value(v); \
} while(0)
#endif

#define UNKNOWN_TIME    "0001-01-01T00:00:00Z"
#endif
