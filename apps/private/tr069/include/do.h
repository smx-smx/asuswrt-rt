/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file do.h
 */
#ifndef __HEADER_DO__
#define __HEADER_DO__

#include "session.h"
#include "tr_lib.h"

/*!
 * \brief Ckeck if or not a object(interior node) is writable.
 * For an object if it is multi-instances capable or it is an instance then it is writable
 *
 * \param node The object node
 *
 * \return 0 if is not writable, or return 1
 */
int node_is_writable( node_t node );


/*!
 * \brief Remove a particular instance of an object
 *
 * \param path The absolute path of the instance, ended with "{instance number}."
 * \param path_len The length of the path string
 *
 * \return 0 when success or relative errno when failed, which is the return value of __do__ function, if 9002, it could also be caused by start_transaction failed
 */
int delete_object( char *path, int path_len );

/*!
 * \brief Process the DeleteObject command from ACS
 *
 * \param ss The DM session
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED when success, or METHOD_FAILED when any error
 */
int do_process( struct session *ss, char **msg );

/*!
 * \brief Generate the DeleteObjectResponse or Fault response for the DeleteObject command
 *
 * \param ss The DM session
 *
 * \return  METHOD_COMPLETE
 */
int do_body( struct session *ss );

#ifdef TR196
int backup_subtree( node_t node );
#endif
int node_is_instance( node_t node );
int node_is_object( node_t node );
#endif

#ifdef __ASUS
int __do__( char *path, int path_len );
#endif