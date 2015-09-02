/*=======================================================================
  
       Copyright(c) 2009, Works Systems, Inc. All rights reserved.
  
       This software is supplied under the terms of a license agreement 
       with Works Systems, Inc, and may not be copied nor disclosed except 
       in accordance with the terms of that agreement.
  
  =======================================================================*/
#ifndef __WAR_ERRORCODE_H
#define __WAR_ERRORCODE_H

#include <errno.h>

int war_geterror();
int war_getsockerror();
char * war_sockstrerror(int errnum);
char * war_strerror(int errnum);

#endif
