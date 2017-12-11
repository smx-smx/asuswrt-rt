/*
 * Copyright 2014 Trend Micro Incorporated
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 */

#ifndef __FORWARD_CONFIG_H__
#define __FORWARD_CONFIG_H__

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/version.h>
#include <linux/vmalloc.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
#include <generated/autoconf.h>
#else
#include <linux/autoconf.h>
#endif
#endif // __KERNEL__

#include "tdts_udb.h"

#if defined(CONFIG_NF_CONNTRACK_MARK)
#define CONFIG_USE_CONNTRACK_MARK 1
#else
#define CONFIG_USE_CONNTRACK_MARK 0
#endif

#ifndef TMCFG_APP_K_TDTS_UDBFW_FAST_PATH
#define TMCFG_APP_K_TDTS_UDBFW_FAST_PATH 0
#endif

#if (TMCFG_E_UDB_CORE_URL_QUERY || TMCFG_E_UDB_CORE_VIRTUAL_PATCH || TMCFG_E_UDB_CORE_ANOMALY_PREVENT || TMCFG_E_UDB_CORE_IQOS_SUPPORT)
#define UDB_IP_HLIST 1
#else
#define UDB_IP_HLIST 0
#endif

#if 0
/* Adjust vendor reserved bits for conntrack mark */
#if TMCFG_APP_K_TDTS_UDBFW_CT_MARK_RSV
#else
#define TMCFG_APP_K_TDTS_UDBFW_CT_MARK_RSV_BITS 0
#endif
#endif

////////////////////////////////////////////////////////////////////////////////

#define CONFIG_DEBUG 		(0)	//!< Say 1 to debug
#define CONFIG_DEBUG_SKB 	(0) //!< Say 1 to dump skb.

#define FWMOD_NAME "IDPfw"

////////////////////////////////////////////////////////////////////////////////
#ifdef __KERNEL__

#define ERR(fmt, args...) printk(KERN_ERR " *** ERROR: [%s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##args)
#define PRT(fmt, args...) printk(KERN_INFO FWMOD_NAME ": " fmt "\n", ##args)

#if CONFIG_DEBUG
#define DBG(fmt, args...) printk(KERN_DEBUG "[%s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##args)
#define assert(_condition) \
	do \
	{ \
		if (unlikely(!(_condition))) \
		{ \
			printk(KERN_ERR "\n" FWMOD_NAME  ": Assertion failed at %s:%d\n", __FUNCTION__, __LINE__); \
			BUG(); \
		} \
	} while(0)
#else
#define DBG(fmt, args...) do { } while (0)
#define assert(_condition) do { } while (0)
#endif

#define MEM_DBG(fmt, args...) printk(KERN_DEBUG "[%s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##args)
#endif // __KERNEL__

#endif /* FORWARD_CONFIG_H_ */
