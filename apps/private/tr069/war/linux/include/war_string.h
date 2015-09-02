/*=======================================================================
  
       Copyright(c) 2009, Works Systems, Inc. All rights reserved.
  
       This software is supplied under the terms of a license agreement 
       with Works Systems, Inc, and may not be copied nor disclosed except 
       in accordance with the terms of that agreement.
  
  =======================================================================*/
#ifndef __WAR_STRING_H
#define __WAR_STRING_H

#include <stdio.h>
#include <stdarg.h>

int war_vsnprintf(char *str, size_t size, const char *format, va_list ap);
int war_snprintf(char *str, size_t size, const char *format, ...);
int war_strcasecmp(const char *s1, const char *s2);
int war_strncasecmp(const char *s1, const char *s2, size_t n);
int war_getline(char **buf, size_t *len, FILE *fp);
char *war_strdup(const char *s);
char *war_strndup( const char *s, size_t n);


#endif
