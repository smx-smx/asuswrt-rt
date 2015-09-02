/*!
 * *************************************************************
 *
 * Copyright(c) 2011, Works Systems, Inc. All rights reserved.
 *
 * This software is supplied under the terms of a license agreement
 * with Works Systems, Inc, and may not be copied nor disclosed except
 * in accordance with the terms of that agreement.
 *
 * *************************************************************
 */

/*!
 * \file download_upload.h
 *
 */
#ifndef __HEADER_TASK__
#define __HEADER_TASK__

#include "session.h"

int download_upload_process( struct session *ss, char **msg );
int download_upload_body( struct session *ss );

#endif
