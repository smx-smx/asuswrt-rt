/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file retry.h
 *
 */
#ifndef __HEADER_RETRY__
#define __HEADER_RETRY__

int in_retry();
int get_retry_count( void );
void reset_retry_count( void );
void retry_later();

#endif
