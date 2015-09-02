/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/

/*
 *	File:	evcom.h
 *	 
 *	Author: PT Chen, Quark Li
 *	 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	EVA Console Manager CLI tool Constant and function prototypes. 
 *
 */

#include "eva_constant.h"

#ifndef _EVCOM_H_
#define _EVCOM_H_

#ifndef NULL                                
#define NULL (void*)(0)
#endif

#define MAX_CMD_LEN 			(128)
#define MAX_TONE_PROFILE 		(5)
#define MAX_TONELIST_PROFILE 	(10)
#define MAX_TOKEN				(20)

#define CHAN_FIRST 	0
#define CHAN_LAST 	2
#define STRM_FIRST	0
#define STRM_LAST	1
#define INFC_FIRST	CHAN_FIRST
#define INFC_LAST	CHAN_LAST
#define TONECFG_FIRST 0
#define TONECFG_LAST 9


typedef enum {         
    CMD_DSP_INVOKE,   
    CMD_DSP_REVOKE,
    CMD_DSP_QUERY,
    CMD_DSP_CONFIG_TONE,
    CMD_DSP_CONFIG_CPT,

    CMD_CHAN_CONFIG,
    CMD_CHAN_PLAY_TONE,
    CMD_CHAN_PLAY_CID,
    CMD_CHAN_PLAY_CID_T2,
    CMD_CHAN_PCM_DUMP,

    CMD_STRM_CONFIG,
    CMD_STRM_START,
    CMD_STRM_STOP,
    CMD_STRM_SEND_DTMFR,
    CMD_STRM_PLAY_TONE,

    CMD_INFC_QUERY,
    CMD_INFC_CONFIG_LINE,
    CMD_INFC_CONFIG_HOOK,
    CMD_INFC_CONFIG_HOOKTS,
    CMD_INFC_CONFIG_RING,
    CMD_INFC_RING,

    CMD_QUIT,
    CMD_INVALID
} command_e;

#define NUM_OF_COMMAND (CMD_INVALID)

typedef struct {
    command_e code;
    char*	name;
    char* opcode;
    char*	desc;
    char*	usage;
    char*	example;

    void (*func)(const char *inStr);
} command_t;

void cmdDspInvoke(const char *inStr);
void cmdDspRevoke(const char *inStr);
void cmdDspQuery(const char *inStr);
void cmdDspConfigTone(const char *inStr);
void cmdDspConfigCpt(const char *inStr);

void cmdChanConfig(const char *inStr);
void cmdChanPlayTone(const char *inStr);
void cmdChanPlayCid(const char *inStr);
void cmdChanPlayCid2(const char *inStr);
void cmdChanPcmDump(const char *inStr);

void cmdStrmConfig(const char *inStr);
void cmdStrmStart(const char *inStr);
void cmdStrmStop(const char *inStr);
void cmdStrmSendDtmfr(const char *inStr);
void cmdStrmPlayTone(const char *inStr);

void cmdInfcQuery(const char *inStr);;
void cmdInfcConfigLine(const char *inStr);
void cmdInfcConfigHook(const char *inStr);
void cmdInfcConfigHookTs(const char *inStr);
void cmdInfcConfigRing(const char *inStr);
void cmdInfcRing(const char *inStr);

void cmdQuit(const char *inStr);;

#endif /* End of _EVCOM_H_ */

