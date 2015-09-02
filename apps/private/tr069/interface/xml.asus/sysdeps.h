/*
 * Copyright(c) 2013, ASUSTeK Inc.
 * All rights reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTeK Inc.;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#ifndef __SYSDEPS_H
#define __SYSDEPS_H

extern int wl_control_channel(int unit);
extern int get_wireless_totalassociations(char *path);
extern int renew_associatation(char *path);

#endif /*__SYSDEPS_H*/
