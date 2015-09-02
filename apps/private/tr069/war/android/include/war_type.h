/*=======================================================================
  
       Copyright(c) 2009, Works Systems, Inc. All rights reserved.
  
       This software is supplied under the terms of a license agreement 
       with Works Systems, Inc, and may not be copied nor disclosed except 
       in accordance with the terms of that agreement.
  
  =======================================================================*/
#ifndef __WAR_TYPE_H
#define __WAR_TYPE_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
//#include <sys/time.h>

#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>



#ifndef __PRETTY_FUNCTION__
#   if defined(_MSC_VER) && (defined(__INTEL_COMPILER) || _MSC_VER >= 1300)
#       define __PRETTY_FUNCTION__ __FUNCSIG__
#   else
#       define __PRETTY_FUNCTION__ __func__
#   endif
#endif

#define FILE_PATH_SEP '/'
#define WORK_DIRECTORY "/etc/tr/"


#define WAR_EINPROGRESS EINPROGRESS
#define WAR_EAGAIN   EAGAIN

typedef int ioctl_req_t;//todo
typedef int ioctl_argp_t;//todo
typedef unsigned long long ulonglong;

#endif
