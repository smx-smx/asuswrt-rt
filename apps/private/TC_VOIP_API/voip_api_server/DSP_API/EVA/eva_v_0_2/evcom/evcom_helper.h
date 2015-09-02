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
 *	File:	evcom_helper.h
 *	 
 *	Author: PT Chen, Quark Li
 * 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	EVA Console Manager CLI Tool. Data validation and converstion function
 *	prototypes. 
 *
 */

#include "eva_constant.h"
#include "evcom.h"

#ifndef _EVCOM_HELPER_H_
#define _EVCOM_HELPER_H_

evaBool_e isValidNum(const char* inStr, int min, int max);
evaBool_e isValidIp(const char* inStr);
evaBool_e isValidCodec(const char* inStr); 
evaBool_e isValidPtime(const char* inStr); 
evaBool_e isValidStrmdir(const char* inStr);
evaBool_e isValidOnOff(const char* inStr);
evaBool_e isValidDtmf(const char* inStr);  
evaBool_e isValidChanId(const char* inStr);
evaBool_e isValidStrmId(const char* inStr, chanId_e *pCh, strmId_e *pSt);
evaBool_e isValidInfcId(const char* inStr);
evaBool_e isValidToneid(const char* inStr);
evaBool_e isValidCptType(const char* inStr);
evaBool_e isValidToneType(const char* inStr);
evaBool_e isValidGain(const char* inStr);
evaBool_e isEnabled(const char* inStr, const char* pAttr);
evaBool_e isValidJbMode(const char* inStr);
evaBool_e isValidJbSize(const char* inStr);
evaBool_e isValidCidFormat(const char* inStr);

void 	replaceKeyToBlank(char* inStr, char key);
void 	adjustBackSpace(char* inStr);

codec_e 	convertCodec(const char* inStr);
pTime_e 	convertPtime(const char* inStr);
strmDir_e	convertStrmdir(const char* inStr);
dtmf_e 	convertDtmf(const char* inStr);
lineState_e convertLineState(const char* inStr);
hookState_e convertHookState(const char* inStr);
int convertToneType(const char* inStr);
int convertCptType(const char* inStr);
enableControl_e convertControl(const char* inStr);
int convertJbMode(const char* inStr);
int convertCidFormat(const char* inStr);
char *	etosActive(int32 e);
char *	etosBool(int32 e);
char *	etosControl(int32 e);
char *	etosInfctype(int32 e);
char *	etosTonetype(int32 e);
char *	etosIpver(int32 e);
char *	etosStrmdir(int32 e);
char* 	etosDtmf(int32 e);
char *	etosTone(int32 e);
char *     etosToneDir(int32 e);
char *	etosEvent(int32 e);
char *	etosEdge(int32 e);
char* 	etosExec(int32 e);
char*	etosCodec(int32 e) ;
char* 	etosPtime(int32 e) ;
char*	etosHook(int32 e);
char*	etosLine(int32 e);
char* 	etosJbMode(int32 e);
char* 	etosT38State(int32 e) ;
char* 	etosCidFormat(int32 e);
char*   	ipToStr(uint32 ip);
int 			getTokens(const char *inStr, const char* delim, char *pArgv[]);
void 		getAttrVal(const char *inStr, const char *pAttr, const char *delim, char *pVal);
command_e 	getCmdCode(const command_t *cmdSet, const char *inStr);

#endif


