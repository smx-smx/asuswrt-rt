/*=======================================================================
  
       Copyright(c) 2009, Works Systems, Inc. All rights reserved.
  
       This software is supplied under the terms of a license agreement 
       with Works Systems, Inc, and may not be copied nor disclosed except 
       in accordance with the terms of that agreement.
  
  =======================================================================*/
#ifndef __WAR_FILE_H
#define __WAR_FILE_H


int war_unlink(const char *pathname);
int war_rename(const char *oldname, const char *newname);
int war_judge_absolutepath(const char *pathname);
int init_argument(int argc, char **argv, char *p_directory, int size);

#endif
