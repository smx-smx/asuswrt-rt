/*=======================================================================
  
       Copyright(c) 2009, Works Systems, Inc. All rights reserved.
  
       This software is supplied under the terms of a license agreement 
       with Works Systems, Inc, and may not be copied nor disclosed except 
       in accordance with the terms of that agreement.
  
  =======================================================================*/
#ifndef __WAR_TYPE_H
#define __WAR_TYPE_H

#include <windows.h>
#include <winbase.h>
#include <winsock2.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include "arithmetic.h"

#define WAR_EINPROGRESS  WSAEWOULDBLOCK
#define WAR_EAGAIN    WSAEWOULDBLOCK
#define ENOENT      2

//socket options wince no support
#define SO_PRIORITY 0x0200 
#define SOL_IP   0x000f
#define IP_TOS   0x0400

#ifndef __PRETTY_FUNCTION__
#   if defined(_MSC_VER) && (defined(__INTEL_COMPILER) || _MSC_VER >= 1300)
#       define __PRETTY_FUNCTION__ __FUNCSIG__
#   else
#       define __PRETTY_FUNCTION__ "<unknown>"
#   endif
#endif

#define FILE_PATH_SEP '\\'	   //NO use in wce
#define WORK_DIRECTORY "\\conf\\"  //No use: indeed refer to init_argument()

typedef long ioctl_req_t;//for ioctlsocket()
typedef unsigned long ioctl_argp_t;//for ioctlsocket()
typedef ULONG64 ulonglong;

#ifndef _TM_DEFINED
struct tm {
  int tm_sec;     /* seconds after the minute - [0,59] */
  int tm_min;     /* minutes after the hour - [0,59] */
  int tm_hour;    /* hours since midnight - [0,23] */
  int tm_mday;    /* day of the month - [1,31] */
  int tm_mon;     /* months since January - [0,11] */
  int tm_year;    /* years since 1900 */
  int tm_wday;    /* days since Sunday - [0,6] */
  int tm_yday;    /* days since January 1 - [0,365] */
  int tm_isdst;   /* daylight savings time flag */
};
#define _TM_DEFINED
#endif

struct timezone
{
   int  tz_minuteswest; /* minutes W of Greenwich */
   int  tz_dsttime;     /* type of dst correction */
};
//tr.h

typedef signed char		int8_t;
typedef short int		int16_t;
typedef int			int32_t;

typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
typedef int 			ssize_t;

typedef uint32_t in_addr_t;
typedef int socklen_t;

#endif
