/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file cli.h
 */
#ifndef __HEADER_CLI__
#define __HEADER_CLI__

#include "session.h"

struct listener {
    char addr[32];
    char uri[128];
};


#ifdef TR232
/*!
 * \brief Set IPDRDoc file path
 *
 * \param name The config item name
 * \param value The config item value
 *
 * \return Always return 0
 */
int set_ipdrdoc_path( const char *name, const char *value );

#ifdef __ASUS
/*!
 * \brief Generate IPDRDoc for profile
 *
 * \param name The path of profile
 * \param file_format The file format for profile
 * \param count The count of doc for profile
 *
 * \return 0 when success, -1 when any error
 */
int generate_ipdrdoc( char *name, char *doc_format, int doc_count );
#endif
#endif

/*!
 * \brief Eat the CLI configs
 *
 * \param name The config item name
 * \param value The config item value
 *
 * \return Always return 0
 */
int set_cli_config( const char *name, const char *value );
/*!
 * \brief Notify a parameter's value changed
 *
 * \param path The parameter's path
 * \param new The new value
 *
 * \return N/A
 */
void value_change( const char *path, const char *new );

/*!
 * \brief Launch the CLI listener to process the incoming CLI command
 *
 * \return 0 when success, -1 when any error
 */
int launch_cli_listener( void );

/*!
 * \brief Register a value change trigger
 *
 * \param pattern The parameter's path pattern
 * \param trigger The callback function to be called when the parameter's value changed
 *
 * \return 0 when success, -1 when any error
 */
int register_vct( const char *pattern, void ( *trigger )( const char *path, const char *new_value ) );
#endif
