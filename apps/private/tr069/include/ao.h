/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file ao.h
 */
#ifndef __HEADER_AO__
#define __HEADER_AO__

#include "session.h"

/*!
 * \brief Process the Addobject command from ACS
 *
 *
 * \param ss The TR session structure
 * \param msg The left SOAP message received from ACS
 *
 * \return METHOD_SUCCESSED if the AddObject operation successed, METHOD_FAILED if the
 * AddObject operation if failed.
 */
int ao_process( struct session *ss, char **msg );

/*!
 * \brief Generate the add object response
 *
 * \param ss The session structure
 *
 * \return  METHOD_SUCCESS
 */
int ao_body( struct session *ss );

/*!
 * \brief Add object
 *
 * \param path The absolute path of the instance's parent, ended with "."
 * \param path_len The length of the path
 *
 * \return instance number when success or relative errno when failed, which is mostly the return value of __ao__ function, if 9002, it could also be caused by start_transaction failed
 */
int add_object( char *path, int path_len );

int __ao__( char *path, int path_len );

#endif
