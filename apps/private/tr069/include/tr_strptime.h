/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file strptime.h
 *
 */
#ifndef __HEADER_STRPTIME__
#define __HEADER_STRPTIME__
#ifdef WITH_STRPTIME
#include <time.h>
char *strptime( const char *buf, const char *fmt, struct tm *tm );
#else
#define _XOPEN_SOURCE
#include <time.h>
#endif
int string_time2second( const char *str_time, time_t *output );
#endif
