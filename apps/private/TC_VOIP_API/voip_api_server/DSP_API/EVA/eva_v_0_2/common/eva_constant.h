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
 *	File:	eva_constant.h
 *	 
 *	Author: Serena Huang, Quark Li
 *	 
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	Enhance VoIP Architecture (EVA) generic Constant and Enumeration
 *	difinitions. 
 *
 */
 
#ifndef _EVA_CONST_H_
#define _EVA_CONST_H_

/* Primitive Types */
#ifndef UINT8
#define UINT8
typedef unsigned char   uint8;
#endif

#ifndef UINT16
#define UINT16
typedef unsigned short  uint16;
#endif

#ifndef UINT32
#define UINT32
typedef unsigned long   uint32;
#endif

#ifndef INT8
#define INT8
typedef signed char    int8;
#endif

#ifndef INT16
#define INT16
typedef signed short int int16;
#endif

#ifndef INT32
#define INT32
typedef signed long  int32;
#endif

#ifndef NULL                                
#define NULL (void*)(0)
#endif

#define MAX8				(0xff)
#define MAX16				(0xffff)
#define MAX32				(0xffffffff)
#define MAX_CID_CHAR_LEN	(32)
#define MAX_CADENCE		(3)
#define MAX_TONE_FREQ		(4)	
#define DUAL_TONE_FREQ		(2)
#define MAX_PACKET_SZ		(1024)
#define MAX_GAIN_AMP		(40)
#define MIN_GAIN_AMP		(-40)
#define JB_SZ_MAX			(800)
#define JB_SZ_MIN			(0)

/* Generic Conditions */
typedef enum {
	EVA_FALSE,
	EVA_TRUE
} evaBool_e;

typedef enum {
	CONTROL_DISABLE,	
	CONTROL_ENABLE
} enableControl_e;

/* DSP Identification */
typedef enum {
	DSP_VIKING
} dspId_e;

/* Channel ID Enumeration */
typedef enum {
	CH0,
	CH1,
	CH2,
	CH3,
	CH4,
	CH5,
	CH6,
	CH7
} chanId_e;

/* Stream ID Enumeration */
typedef enum {
	STRM0,
	STRM1,
	STRM2,
	STRM3
} strmId_e;

/* Interface ID Enumeration */
typedef enum {
	INFC0,
	INFC1,
	INFC2,
	INFC3,
	INFC4,
	INFC5,
	INFC6,
	INFC7
} infcId_e;


/* Execution Return Code */
typedef enum { 
	EXEC_SUCCESS = 1,
	EXEC_FAIL = -1,
	INVALID_PARAM = -2,
	FUNC_UNSUPPORT = -3,
	DEVICE_BUSY = -4,
	UNKNOWN_ERROR = -5
} exCode_e;

typedef enum {
	BLOCKING,
	NON_BLOCKING
} blockMode_e;

/* Generic Active State */
typedef enum {
	STATE_INACTIVE,
	STATE_ACTIVE
} activeState_e;

/* Telephony Device Interface Type */
typedef enum {
	INFC_FXS,
	INFC_FXO,
	INFC_AUDIO,
	INFC_OTHER
} infcType_e;

#define INFCTYPE_NUM	(INFC_OTHER + 1)

/* Polarity Direction */
typedef enum {
	POL_FWD,
	POL_REV
} polDir_e;

/* Line State */
typedef enum {
	LINE_DOWN,
	LINE_ACTIVE_FWD,
	LINE_ACTIVE_REV,
	LINE_RING, 
	LINE_RING_PAUSE,
	LINE_BUSY, 
	LINE_SLEEP, 
	LINE_ERROR	
} lineState_e;

/* Hook State */
typedef enum {
	HOOK_FLASH,
	HOOK_RELEASE,
	HOOK_SEIZE,
	HOOK_PULSE1,
	HOOK_PULSE2,	
	HOOK_PULSE3,	
	HOOK_PULSE4,	
	HOOK_PULSE5,	
	HOOK_PULSE6,	
	HOOK_PULSE7,	
	HOOK_PULSE8,	
	HOOK_PULSE9,	
	HOOK_PULSE10,	
	HOOK_PULSE11,	
	HOOK_PULSE12,	
	HOOK_PULSE13,	
	HOOK_PULSE14,	
	HOOK_PULSE15,	
	HOOK_PULSE16,	
	HOOK_PULSE17,	
	HOOK_PULSE18,	
	HOOK_PULSE19,	
	HOOK_PULSE20,
	HOOK_ERROR
} hookState_e;

/* CODEC Enumerations */
typedef enum { 
	CODEC_G711A,
	CODEC_G711U,
	CODEC_G722,
	CODEC_G723,
	CODEC_G726,
	CODEC_G729,
	CODEC_SILCOMP,
	CODEC_DTMFR,
	CODEC_T38,
	CODEC_INVALID
} codec_e;

#define MAX_CODEC_NUM            (CODEC_T38+1)

/*codec bit mask*/
#define MASK_CODEC_G711A	(1 << CODEC_G711A)
#define MASK_CODEC_G711U	(1 << CODEC_G711U)
#define MASK_CODEC_G723	(1 << CODEC_G723)
#define MASK_CODEC_G722	(1 << CODEC_G722)
#define MASK_CODEC_G726	(1 << CODEC_G726)
#define MASK_CODEC_G729	(1 << CODEC_G729)
#define MASK_CODEC_SILCOMP (1 << CODEC_SILCOMP)
#define MASK_CODEC_DTMFR	(1 << CODEC_DTMFR)
#define MASK_CODEC_T38		(1 << CODEC_T38)

/*EC tail length*/
typedef enum { 
	TAIL_16MS,
	TAIL_32MS,
	TAIL_48MS,
	TAIL_64MS,
	TAIL_128MS
} ecTail_e;

/*Event Type*/
typedef enum {
	EDGE_ONCE,
	EDGE_BEGIN,
	EDGE_END
} eventEdge_e;

/*T38 Code*/
typedef enum {
	T38_DISCONN
} t38State_e;

/*Event Code*/
typedef enum {
	EVENT_CODE_INVALID,
	EVENT_CODE_TONE,
	EVENT_CODE_CID,
	EVENT_CODE_LINE,
	EVENT_CODE_HOOK,
	EVENT_CODE_T38,
	EVENT_CODE_JB_UPDATE,
	EVENT_CODE_NON_RTP_RECVD, 
	EVENT_CODE_RTCP_SEND, /*Not implement yet*/
	EVENT_CODE_RTCP_RECVD, /*Not implement yet*/
	EVENT_CODE_STREAM_UPDATE, /*Not implement yet*/
	EVENT_CODE_TIMER, /*Not implement yet*/
	EVENT_CODE_PERFORMANCE, /*Not implement yet*/
	EVENT_CODE_ERROR /*Not implement yet*/
} eventCode_e;

/*channel Detectors*/
#define DETECT_TONE_DTMF				(1 << 0)
#define DETECT_TONE_MODEM				(1 << 1)
#define DETECT_TONE_CPT				(1 << 2)	/* FXO only */
#define DETECT_CID    					(1 << 3)  /* FXO only */
#define DETECT_DTMFR					(1 << 4)

/*Ptime*/
typedef enum { 
	PTIME_10MS,
	PTIME_20MS,
	PTIME_30MS,
	PTIME_40MS,
	PTIME_50MS,
	PTIME_60MS
} pTime_e;


typedef enum {
	TONE_DTMF_1 = 1,
	TONE_DTMF_2,
	TONE_DTMF_3,
	TONE_DTMF_4,
	TONE_DTMF_5,
	TONE_DTMF_6,
	TONE_DTMF_7,
	TONE_DTMF_8,
	TONE_DTMF_9,
	TONE_DTMF_0,
	TONE_DTMF_STAR,
	TONE_DTMF_POUND,
	TONE_DTMF_A,
	TONE_DTMF_B,
	TONE_DTMF_C,
	TONE_DTMF_D,
	TONE_DIAL,
	TONE_RINGBACK,	
	TONE_BUSY,
	TONE_REORDER,
	TONE_SIT,
	TONE_CUSTOM_1,
	TONE_CUSTOM_2,
	TONE_CUSTOM_3,
	TONE_CUSTOM_4,
	TONE_CNG,
	TONE_CED,
	TONE_ANS,
	TONE_ANSAM,
	TONE_V21PREAMBLE,
	TONE_FAX_CM,
	TONE_FAX_TIMEOUT,
	TONE_INVALID
} toneCode_e;

#define NUM_OF_TONE (TONE_INVALID)

typedef enum {
	DTMF_0,
	DTMF_1,
	DTMF_2,
	DTMF_3,
	DTMF_4,
	DTMF_5,
	DTMF_6,
	DTMF_7,
	DTMF_8,
	DTMF_9,
	DTMF_STAR,
	DTMF_POUND,
	DTMF_A,
	DTMF_B,
	DTMF_C,
	DTMF_D,
} dtmf_e;

typedef enum {
	STRM_INACTIVE,
	STRM_SENDONLY,
	STRM_RECVONLY,
	STRM_SENDRECV
} strmDir_e;

typedef enum {
	IPV4,
	IPV6
} ipVer_e;


typedef enum {
	TONE_REGULAR,
	TONE_MODULATE
} toneType_e;

/*Jitter buffer mode*/
typedef enum {
	JB_ADAPT,
	JB_FIXED
} jbMode_e;

typedef enum {
    CID_FORMAT_BELLCORE_FSK,
    CID_FORMAT_ETSI_DTMF,
	CID_FORMAT_NTT
} cidFormat_e;

/*Tone event direction*/
typedef enum{
	TONE_DIR_LOCAL,
	TONE_DIR_REMOTE
} toneDir_e;

#endif /* End of _EVA_CONST_H_ */

