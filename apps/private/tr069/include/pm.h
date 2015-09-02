/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file cm.h
 *
 */
#ifndef __HEADER_PM__
#define __HEADER_PM__

int gen_pm_file( const char *path );
int launch_periodic_upload_sched();
int set_pm_config( const char *name, const char *value );

#endif
