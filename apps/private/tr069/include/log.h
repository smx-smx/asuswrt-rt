/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file log.h
 */


#ifndef __HEADER_LOG__
#define __HEADER_LOG__


#include <stdio.h>
#include <stdarg.h>

#include "war_type.h"

#define _A_ __FILE__, __LINE__, __PRETTY_FUNCTION__

#ifdef LOG_DEBUG
#undef LOG_DEBUG
#endif
#define __LOG_DEBUG    0
#define LOG_DEBUG      __LOG_DEBUG, _A_

#ifdef LOG_NOTICE
#undef LOG_NOTICE
#endif
#define __LOG_NOTICE   1
#define LOG_NOTICE     __LOG_NOTICE, _A_

#ifdef LOG_WARNING
#undef LOG_WARNING
#endif
#define __LOG_WARNING  2
#define LOG_WARNING    __LOG_WARNING, _A_

#ifdef LOG_ERROR
#undef LOG_ERROR
#endif
#define __LOG_ERROR    3
#define LOG_ERROR      __LOG_ERROR, _A_


/*!
 * \brief A config callback function to setup log's configuration
 *
 * \param name The config item name
 * \param value The config item value
 *
 * \return 0 on success, -1 on failure
 */
int set_log_config( const char *name, const char *value );

/*!
 * \brief Start the logger - open the log file if the configuration require log
 * it to file, if open the file failed, agent will switch to TO_SCREEN mode
 */
int start_logger( void );

/*!
 * \brief Record a piece of log
 *
 * \param level The log message level
 * \param file The source code file name of the log message comes from
 * \param line The line number of the source code of the log message comes from
 * \param function The function name which generates this log message
 * \param fmt The log message's format
 */
void tr_log( unsigned int level, const char *file, int line, const char *function, const char *fmt, ... );

#endif
