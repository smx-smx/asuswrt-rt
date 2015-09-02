/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file spv.h
 *
 */
#ifndef __HEADER_SPV__
#define __HEADER_SPV__

#include "tr_lib.h"
#include "session.h"

#ifdef ALIAS
struct spv_ao_record {
    char path[NODE_PATH_LEN + 1];
    struct spv_ao_record *next;
};
#endif //ALIAS

struct status_1 {
    char path[NODE_PATH_LEN+1];
    struct status_1 *next;
};

int set_status_1( const char *name, const char *path );
struct status_1* get_status_1_head();
int __backup_set_parameter_value( const char *_path, const char *value );
int __set_parameter_value( const char *_path, const char *value );
int set_parameter_value( const char *path, const char *value );
int spv_process( struct session *ss, char **msg );
int spv_body( struct session *ss );
void spv_rewind( struct session *ss );
void spv_destroy( struct session *ss );
char *__get_parameter_value(char *path, char *value);

#ifdef TR196
int check_parameter_value( const char *value, const char *type, char *limitation );
#endif

#endif
