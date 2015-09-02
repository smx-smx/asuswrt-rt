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
 *	File:	eva_struct.h
 *	 
 *	Author: Serena Huang, Quark Li
 *	 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	Enhance VoIP Architecture (EVA) generic data structure definitions.
 *
 */

#ifndef _EVA_STRUCT_H_
#define _EVA_STRUCT_H_

#include "eva_constant.h"

/* ========================================
 *	Generic data context section 
 * ======================================== */
#define MAX_CID_CHAR_LEN		(32)

typedef struct {
	uint32 	addrV4;
	uint16 	addrV6[8];
	ipVer_e 	ver; 	/* ipv4/ipv6 */
	uint16 	port; /* 1024~65535, 0 for any */
	uint16	rtcpPort;
}netAddr_t;

typedef struct {
	cidFormat_e format;
	char number[MAX_CID_CHAR_LEN];
	char name[MAX_CID_CHAR_LEN];
	char dateTime[MAX_CID_CHAR_LEN];
}cid_t;

typedef union {
	struct{
		lineState_e status;
		int ringCount;
	}line; /* EVENT_CODE_LINE */

	struct{
		hookState_e status;
		int pulseCount;
	}hook; /*EVENT_CODE_HOOK*/
	
	struct{
		uint32 dspExecTimes;
		uint32 averageMhz;
	}performanceIdx; /*EVENT_CODE_PERFORMANCE*/
	
	struct{
		int8 number[MAX_CID_CHAR_LEN];
		int8 name[MAX_CID_CHAR_LEN];
		int8 dateTime[MAX_CID_CHAR_LEN];
	}cidData;/*EVENT_CODE_CID_DETECTED*/
	
	struct{
		toneCode_e code;
		toneDir_e dir;
	}tone; /*EVENT_CODE_TONE_DETECTED*/
	
	struct{
		uint8 streamId;
		t38State_e status;
	}t38; /*EVENT_CODE_T38*/
	
	struct{
		uint8 streamId;
		uint32 total;
		uint32 drop;
		uint32 plc;
		uint32 jbSize;
		uint32 avgJitter;
	}jb; /*EVENT_CODE_JB_UPDATE*/
	
	struct{
		netAddr_t srcAddr;
		netAddr_t dstAddr;
		uint8 payload[MAX_PACKET_SZ];
	}packet; /*EVENT_CODE_NON_RTP_RECVD*/
}eventContext_u;

typedef struct {
	eventEdge_e 		edge;
	eventCode_e 		evtCode;
	uint32 			dspTick; /*dspTicks*/
	infcId_e			infcId;
	chanId_e			chanId;
	eventContext_u 	context;
} event_t;

typedef struct {
	uint16 onTime;
	uint16 offTime;
} cadence_t;

/* ========================================
 *	Interface Section 
 * ======================================== */
typedef struct {
	cadence_t cad[MAX_CADENCE];
	uint32	dur;
	cid_t	cid;
	uint8	cidAt;
} ringProfile_t;

typedef struct {
    uint32	flashMin;
    uint32	flashMax;
    uint32	releaseMin;
} hookThreshold_t;

typedef struct {
	infcType_e	type;
	lineState_e	lineState;
	hookState_e hookState;
	polDir_e		pol;
	ringProfile_t	ring;
	hookThreshold_t hookTs;
}infcConfig_t;

/* ========================================
 *	Stream Section 
 * ======================================== */
typedef struct {
	ipVer_e		ver;
	netAddr_t 	srcAddr;
	netAddr_t 	dstAddr;
	uint8 		encrypt; /*T/F*/ /*reserved for user to select the encypt type*/
	uint32 		dur; /*session time*/ /*reserved for update the session duration*/
}session_t;

typedef struct {
	codec_e			payloadSelect;
	pTime_e			ptimeSelect;
	enableControl_e	dtmfRelay; /*enable/disable*/
	enableControl_e	silenceComp; /*enable/disable*/
	strmDir_e 		direction;
	uint32 			jbUpdateTime; /*duration of JB_EVENT update, 0 to turn off*/
}strmAttr_t;

typedef struct {
	jbMode_e mode;
	uint32 szJbMax;
	uint32 szJbInit;
}jbConfig_t;

typedef struct {
	session_t 		session;
	strmAttr_t 		strmAttr;
	uint8 			payloadType[MAX_CODEC_NUM];
	jbConfig_t		jbConf;
}strmConfig_t;

/* ========================================
 *	Channel Section 
 * ======================================== */
typedef struct {
	uint16 	detectMask; //bitmask
	int8 		ampTx; // db, step by 0.5
	int8 		ampRx; // db, step by 0.5
	enableControl_e ecEnable;
}chanConfig_t;

/* ========================================
 *	DSP Section 
 * ======================================== */
typedef struct {
	toneType_e toneType;
	
    struct{
		uint16 	toneFreq[MAX_TONE_FREQ];
		int16	tonePwr[MAX_TONE_FREQ];  
	}regular;
		
	struct{
		int16 	baseFreq;
		int16 	modFreq;
		int16 	modPwr;
		int16 	modDepth;
	}modulate;
	
	int16 makeTime[MAX_CADENCE];
	int16 breakTime[MAX_CADENCE];
	int16 repeat[MAX_CADENCE];	
}tone_t;

typedef struct {
	uint8 *toneIdSeq;
	uint8 numOfTone;
}toneSeq_t;

typedef struct {
    toneCode_e	type;
    struct {
		int16	toneFreq[DUAL_TONE_FREQ];
		int16	toneDev[DUAL_TONE_FREQ];
		int16	minMake[MAX_CADENCE];
		int16	maxMake[MAX_CADENCE];
		int16	minBreak[MAX_CADENCE];
		int16	maxBreak[MAX_CADENCE];
		int16	power;
	} dual;	
}cpt_t;

typedef struct {
	uint16 	dspId;
	uint8 	numOfChan;
	uint8 	strmsPerChan;
	uint32 	codecSupport; /*bitmask*/
	uint32 	ptimeSupport; 	/*bitmask*/
	uint8 	rtpRedundancy; /*T/F*//*not support currently*/
	uint8 	maxEcTailLength;
}dspFeature_t;

typedef struct {
	/*dsp Function*/
	exCode_e (*dspInvoke)		(void);
	exCode_e (*dspRevoke)		(void);
	exCode_e (*dspQuery)		(activeState_e *pDspActive, dspFeature_t *pFeature);
	exCode_e (*dspPollEvent)	(blockMode_e mode, event_t *pEvent);
	exCode_e (*dspConfigTone)	(uint16 toneId, tone_t *pTone);
	exCode_e (*dspConfigCpt)	(cpt_t *pCpt);

	/*channel Function*/
	exCode_e (*chanQuery)		(chanId_e ch, chanConfig_t *pChanConf);
	exCode_e (*chanConfig)		(chanId_e ch, chanConfig_t *pChanConf);
	exCode_e (*chanPlayTone)		(chanId_e ch, toneSeq_t *pToneSeq, uint32 repeat);
	exCode_e (*chanStopTone)	(chanId_e ch);
	exCode_e (*chanPlayCid)		(chanId_e ch, cid_t *pCid);
	exCode_e (*chanPlayType2Cid)(chanId_e ch, cid_t *pCid);
	exCode_e (*chanDumpPcm)	(chanId_e ch, netAddr_t *pAddr);
	
	/*stream Function*/
	exCode_e (*strmQuery)		(chanId_e ch, strmId_e strm, activeState_e *pStrmActive, strmConfig_t *pStrmConf);
	exCode_e (*strmConfig)		(chanId_e ch, strmId_e strm, strmConfig_t *pStrmConf);
	exCode_e (*strmStart)		(chanId_e ch, strmId_e strm);
	exCode_e (*strmStop)			(chanId_e ch, strmId_e strm);
	exCode_e (*strmSendDtmfr)	(chanId_e ch, strmId_e strm, dtmf_e dtmf, uint32 dur);
	exCode_e (*strmPlayTone)		(chanId_e ch, strmId_e strm, toneSeq_t *pToneSeq, uint32 repeat);
	exCode_e (*strmStopTone)	(chanId_e ch, strmId_e strm);
	
	/*infc Function*/
	exCode_e (*infcQuery)		(infcId_e infc, infcConfig_t *pInfcConf);
	exCode_e (*infcConfigLine)	(infcId_e infc, lineState_e state);
	exCode_e (*infcConfigHook)	(infcId_e infc, hookState_e state);
	exCode_e (*infcConfigRing)	(infcId_e infc, ringProfile_t *pRingProf);
	exCode_e (*infcRing)			(infcId_e infc, uint32 dur, cid_t *pCid);
	exCode_e (*infcStopRing)		(infcId_e infc);
	exCode_e (*infcConfigHookTs)	(infcId_e infc, hookThreshold_t *pHookTs);
}dspObject_t;

#endif /* End of _EVA_STRUCT_H_ */

