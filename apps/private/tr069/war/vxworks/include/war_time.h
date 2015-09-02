/*=======================================================================
  
       Copyright(c) 2009, Works Systems, Inc. All rights reserved.
  
       This software is supplied under the terms of a license agreement 
       with Works Systems, Inc, and may not be copied nor disclosed except 
       in accordance with the terms of that agreement.
  
  =======================================================================*/
#ifndef __WAR_TIME_H
#define __WAR_TIME_H

#include <time.h>
#include <sys/times.h>
#include "war_type.h"

size_t war_strftime( char *s, size_t max, const char *format, const struct tm *tm);
time_t war_time(time_t *t);
struct tm * war_localtime(const time_t *t);
struct tm * war_gmtime(const time_t *t); 
time_t war_mktime(struct tm *tm);
void war_sleep(int sec);
int war_gettimeofday(struct timeval *tv, struct timezone *tz);

#endif
