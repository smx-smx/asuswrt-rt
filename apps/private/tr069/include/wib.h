/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file wib.h
 *
 */

#ifndef __HEADER_WIB__
#define __HEADER_WIB__

#ifdef TR069_WIB

int set_wib_config( const char *name, const char *value );
int launch_wib_sched();
void wib_retry();

#endif  /*TR069_WIB*/
#endif  /*__HEADER_WIB__*/

