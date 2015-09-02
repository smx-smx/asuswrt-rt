/*
 * linux/include/linux/sunrpc/svcauth_gss.h
 *
 * Bruce Fields <bfields@umich.edu>
 * Copyright (c) 2002 The Regents of the Unviersity of Michigan
 *
 * $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/include/linux/sunrpc/svcauth_gss.h#1 $
 *
 */

#ifndef _LINUX_SUNRPC_SVCAUTH_GSS_H
#define _LINUX_SUNRPC_SVCAUTH_GSS_H

#ifdef __KERNEL__
#include <linux/sched.h>
#include <linux/sunrpc/types.h>
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/svcauth.h>
#include <linux/sunrpc/svcsock.h>
#include <linux/sunrpc/auth_gss.h>

int gss_svc_init(void);
void gss_svc_shutdown(void);
int svcauth_gss_register_pseudoflavor(u32 pseudoflavor, char * name);

#endif /* __KERNEL__ */
#endif /* _LINUX_SUNRPC_SVCAUTH_GSS_H */
