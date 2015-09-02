/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file adconfig.h
 */
#ifndef __HEADER_ADCONFIG__
#define __HEADER_ADCONFIG__

/*!
 * \brief Read the config file
 *
 * \return 0 when success, -1 when any error
 * \remark If any error occurs, the agent will exit abnormally
 */
int read_config_file( void );

typedef int ( *config_callback_func )( const char *name, const char *value );

#endif
