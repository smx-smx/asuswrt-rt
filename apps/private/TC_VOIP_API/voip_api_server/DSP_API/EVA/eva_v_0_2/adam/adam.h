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
 *	File:	adam.h
 *	 
 *	Author: Serena Huang, Quark Li
 * 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	Adaptive DSP Access Manager API prototype definition
 *
 */
 
#include "eva_constant.h"
#include "eva_struct.h"

#ifndef _ADAM_H_
#define _ADAM_H_

#define ADAM_VERSION	"0.1.1"
#define ADAM_DSP_NUM	(1)
#define ADAM_INFC_NUM	3

typedef struct {
	char* 			version;
	uint8			dspNum;
	uint8			infcNum;
} adamConfig_t;

/* Allocate ADAM resource and activate ADAM process. */
exCode_e	adamInit(void);

/*Deactivate ADAM process and release resource. */
exCode_e	adamExit(void);

/* Get ADAM's configuration (adamConfig) informaiton */
exCode_e	adamQuery(adamConfig_t *pAdamConf);

exCode_e adamPollEvent(blockMode_e mode, event_t *pEvent);


/* Invoke (load and initialize) DSP by DSP ID*/
exCode_e adamDspInvoke(dspId_e dsp);

/* Revoke (terminate and unload) DSP by DSP ID */
exCode_e adamDspRevoke(dspId_e dsp);

/* Query DSP support feature by DSP ID */
exCode_e adamDspQuery(dspId_e dsp, activeState_e *dspActive, dspFeature_t *pFeature);

/* Configure Tone profile by DSP ID */
exCode_e adamDspConfigTone(dspId_e dsp, uint16 toneId, tone_t *pTone);

/* Configure CPT profile by DSP ID */
exCode_e adamDspConfigCpt(dspId_e dsp, cpt_t *pCpt);


/* Query Channel configuration by DSP ID and Channel ID */
exCode_e adamChanQuery(dspId_e dsp, chanId_e ch, chanConfig_t *pChanConf);

/* Configure Channel by DSP ID and Channel ID */
exCode_e adamChanConfig(dspId_e dsp, chanId_e ch, chanConfig_t *pChanConf);

/* Play (generate) tone to specific channel by DSP ID and Channel ID */
exCode_e adamChanPlayTone(dspId_e dsp, chanId_e ch, toneSeq_t *pToneSeq, uint32 repeat);

/* Play (generate) tone sequence to specific channel by DSP ID and Channel ID */
exCode_e adamChanStopTone(dspId_e dsp, chanId_e ch);

/* Play (generate) Caller ID to specific channel by DSP ID and Channel ID */
exCode_e adamChanPlayCid(dspId_e dsp, chanId_e ch, cid_t *pCid);

exCode_e adamChanPlayType2Cid(dspId_e dsp, chanId_e ch, cid_t *pCid);

exCode_e adamChanDumpPcm(dspId_e dsp, chanId_e ch, netAddr_t *pDstAddr);



/* Query Stream configuration by DSP ID, Channel ID and Stream ID */
exCode_e adamStrmQuery(dspId_e dsp, chanId_e ch, strmId_e strm, activeState_e *pStrmActive, strmConfig_t *pStrmConf);

/* Configure Channel by DSP ID, Channel ID and Stream ID */
exCode_e adamStrmConfig(dspId_e dsp, chanId_e ch, strmId_e strm, strmConfig_t *pStrmConf);

/* Enable a streamming process by the configuration of the Stream */
exCode_e adamStrmStart(dspId_e dsp, chanId_e ch, strmId_e strm);

/* Disable a streamming process */
exCode_e adamStrmStop(dspId_e dsp, chanId_e ch, strmId_e strm);

/* Send DTMF relay packet (RFC2833/4733) to the session destination */
exCode_e adamStrmSendDtmfr(dspId_e dsp, chanId_e ch, strmId_e strm, dtmf_e dtmf, uint32 dur);

exCode_e adamStrmPlayTone(dspId_e dsp, chanId_e ch, strmId_e strm, toneSeq_t *pToneSeq, uint32 repeat);

exCode_e adamStrmStopTone(dspId_e dsp, chanId_e ch, strmId_e strm);

/* Query Interface configuration by Interface ID */
exCode_e adamInfcQuery(infcId_e infc, infcConfig_t *pInfcConf);

/* Configure Interface Line state by Interface ID */
exCode_e adamInfcConfigLine(infcId_e infc, lineState_e state);

/* Configure Interface Hook state by Interface ID */
exCode_e adamInfcConfigHook(infcId_e infc, hookState_e state);

/* Configure Interface Ring cadence by Interface ID */
exCode_e adamInfcConfigRing(infcId_e infc, ringProfile_t *pRingProf);

exCode_e adamInfcRing(infcId_e infc, uint32 dur, cid_t *pCid);

exCode_e adamInfcStopRing(infcId_e infc);

/* Configure Interface Hook threshold by Interface ID */
exCode_e adamInfcConfigHookTs(infcId_e infc, hookThreshold_t *pHookTs);
#endif 

