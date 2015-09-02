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
 *	File:	evcom_global.h
 *	 
 *	Author: PT Chen, Quark Li
 * 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	EVA Console Manager CLI Commands definition.
 *
 */

#ifndef _EVCOM_GLOBAL_H_
#define _EVCOM_GLOBAL_H_
#include "evcom.h"

command_t gCmdSet[NUM_OF_COMMAND] = {
    { .code = CMD_DSP_INVOKE,
      .name = "DspInvoke",
      .opcode = "di",
      .desc = "Initialize and start the DSP process.",
      .usage = "<dspInvoke | di>",
      .example = "EVCom >> dspInvoke",
      .func = cmdDspInvoke},   
      
    { .code = CMD_DSP_REVOKE,
      .name = "DspRevoke",
      .opcode = "dr",
      .desc = "Terminate DSP process.",
      .usage = "<dsprevoke | dr>",
      .example = "EVCom >> dsprevoke",
      .func = cmdDspRevoke},   
      
    { .code = CMD_DSP_QUERY,
      .name = "DspQuery",
      .opcode = "dq",
      .desc = "Query DSP configuration.",
      .usage = "<dspquery | dq>",
      .example = "EVCom >> dspquery",
      .func = cmdDspQuery},   
                  
    { .code = CMD_DSP_CONFIG_TONE,
      .name = "DspConfigTone",
      .opcode = "dct",
      .desc = "Change DSP tone configuration.",
      .usage = "<dspconfigtone |dct> tone=[tone_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: type=<reg | mod>, Note: Regular or Modulated tone. \n" \
      			"attr: For type=reg, <f0 | f1 | f2 | f3>=[freqency,power] \n" \
      			"attr: For type=mod, mf=[base_freq,mod_freq,mod_db,mod_depth] \n"\
      			"Note: Frequency=0~4000(Hz), Power=-40~0(db) \n" \
      			"attr: <cad0 | cad1 | cad2>=[ontime(ms),offtime(ms),cad_repeat]\n",
      .example = "EVCom >> dspconfigtone tone=0 type=reg f0=350,-20 f1=440,-16 cad0=1000,0,10",
      .func = cmdDspConfigTone},   

	{ .code = CMD_DSP_CONFIG_CPT,
      .name = "DspConfigCpt",
      .opcode = "dcc",
      .desc = "Change DSP call progress tone detection configuration.",
      .usage = "<dspconfigcpt | dcc> cpt=[tone_type] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: cpt = <dial | ringback | busy | reorder | sit | c1 | c2 | c3 | c4>\n" \
      			"attr: <f0 | f1 >=[freqency,deviation] Note: Frequency=0~4000(Hz)" \
      			" Deviation=Tolerated frequency deviation from base frequency\n" \
      			"attr: <cad0 | cad1 | cad2>=[Min_ontime(ms),Max_ontime(ms),Min_offtime(ms),Max_offtime(ms)]\n" \
      			"attr: pwr = [power] Note: Power=-40~0(db)\n", 
      .example = "EVCom >> dspconfigcpt cpt=busy f0=480,50 f1=620,50 cad0=450,550,450,550 pwr=-20",
      .func = cmdDspConfigCpt},
                       
    { .code = CMD_CHAN_CONFIG,
      .name = "ChanConfig",
      .opcode = "cc",
      .desc = "Change channel configuration.",
      .usage = "<chanconfig | cc> ch=[channel_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: det=<dtmf | mdm | cpt | cid>[+|-], i.e. To enable (Call Progress Tone) detection, det=cpt+ \n" \
      			"attr: ec=<on | off>, i.e. To disable EC, ec=off \n" \
      			"attr: <tx | rx>=[-20~20], Note: Adjust Tx/Rx gain level between +/-20db, default at 0db",
      .example = "EVCom >> chanconfig ch=1 ec=on det=mdm- tx=-3 rx=3",
      .func = cmdChanConfig},   

    { .code = CMD_CHAN_PLAY_TONE,
      .name = "ChanPlayTone",
      .opcode = "cpt",
      .desc = "Generate a tone to the channel.",
      .usage = "<chanplaytone | cpt> ch=[channel_id] tone=[tone_id<,tone_id,...>] rpt=[repeat]",
      .example = "EVCom >> chanplaytone ch=0 tone=0 rpt=5",
      .func = cmdChanPlayTone},   

    { .code = CMD_CHAN_PLAY_CID,
      .name = "ChanPlayCid",
      .opcode = "cpc",
      .desc = "Generate a caller ID to the channel.",
      .usage = "<chanplaycid | cpc> ch=[channel_id] cid=[number]" \
      		      " cidf=<us | dtmf | jp>, Note: us-BELLCORE_FSK, dtmf-ETSI_DTMF, jp-NTT\n",
      .example = "EVCom >> chanplaycid ch=0 cid=1234 cidf=us",
      .func = cmdChanPlayCid},   

    { .code = CMD_CHAN_PLAY_CID_T2,
      .name = "ChanPlayCid2",
      .opcode = "cpc2",
      .desc = "Generate a type 2 caller ID with SAS signal to the channel.",
      .usage = "<chanplaycid2 | cpc2> ch=[channel_id] cid=[number]" \
      		      " cidf=<us | dtmf | jp>, Note: us-BELLCORE_FSK, dtmf-ETSI_DTMF, jp-NTT\n",
      .example = "EVCom >> chanplaycid2 ch=0 cid=1234 cidf=us",
      .func = cmdChanPlayCid2},   

    { .code = CMD_CHAN_PCM_DUMP,
      .name = "ChanPcmDump",
      .opcode = "cpd",
      .desc = "Enable PCM dump for debug.",
      .usage = "<chanpcmdump | cpd> ch=[channel_id] ip=[ip], Note: Set IP as 0.0.0.0 to disable.",
      .example = "EVCom >> chanpcmdump ch=0 ip=192.168.1.2",
      .func = cmdChanPcmDump},   

    { .code = CMD_STRM_CONFIG,
      .name = "StrmConfig",
      .opcode = "sc",
      .desc = "Change a stream configuration.",
      .usage = "<strmconfig | sc> st=[channel_id:stream_id] $attr0=[value] $attr1=[value] ...\n" \
                  "attr: src=[ip_address:rtp_port:rtcp_port] \n" \
                  "attr: dst=[ip_address:rtp_port:rtcp_port] \n" \
                  "attr: codec=<g711a | g711u | g722 | g726 | g729 | t38> \n" \
                  "attr: ptime=<10 | 20 | 30 | 40 | 50 | 60> \n" \
                  "attr: dtmfr=<on | off> \n" \
                  "attr: scomp=<on | off> \n" \
                  "attr: dir=<sr | so | ro | in> Note: sr=send+receive, so=send only, ro=receive only, in=inactive\n"\
                  "attr: jb=[<a | f>, jb_init_size(ms), jb_max_size(ms)] Note: a=adaptive, f=fixed, JB size is between 0~800 ms",
      .example = "EVCom >> strmconfig st=0:0 src=192.168.1.1:5566:5567 dst=192.168.1.2:5566:5567 " \
      			"codec=g711a ptime=20 dtmfr=on scomp=on dir=sr jb=f,500,700",
      .func = cmdStrmConfig},   
      
    { .code = CMD_STRM_START,
      .name = "StrmStart",
      .opcode = "ss",
      .desc = "Start a streaming process.",
      .usage = "<strmstart | ss> st=[channel_id:stream_id]",
      .example = "EVCom >> strmstart st=0:0",
      .func = cmdStrmStart},   
      
    { .code = CMD_STRM_STOP,
      .name = "StrmstoP",
      .opcode = "sp",
      .desc = "Stop a streaming process.",
      .usage = "<strmstop | sp> st=[hannel_id:stream_id]",
      .example = "EVCom >> strmstop st=0:0",
      .func = cmdStrmStop},   
      
    { .code = CMD_STRM_SEND_DTMFR,
      .name = "StrmSendDtmfr",
      .opcode = "ssd",
      .desc = "Send DTMF relay (RFC2833/4733) packet to the stream.",
      .usage = "<strmsenddtmfr | ssd> st=[channel_id:stream_id] " \
		      "dtmf=<[0~9] | * | # | [a~d]> dur=[duration(ms), dur >= 10ms]",
      .example = "EVCom >> strmsenddtmfr st=0:0 dtmf=1 dur=500",
      .func = cmdStrmSendDtmfr},   
      
    { .code = CMD_STRM_PLAY_TONE,
      .name = "StrmPlayTone",
      .opcode = "spt",
      .desc = "Generate tone to the stream.",
      .usage = "<strmplaytone | spt> st=[channel_id:stream_id] tone=[tone_id<,tone_id,...>] rpt=[repeat]",
      .example = "EVCom >> strmplaytone st=0:0 tone=1 rpt=5",
      .func = cmdStrmPlayTone},   
      
    { .code = CMD_INFC_QUERY,
      .name = "InfcQuery",
      .opcode = "iq",
      .desc = "Query interface configuration.",
      .usage = "<infcquery | iq> if=[interface_id]",
      .example = "EVCom >> infcquery if=0",
      .func = cmdInfcQuery},   
  
    { .code = CMD_INFC_CONFIG_LINE,
      .name = "InfcConfigLine",
      .opcode = "icl",
      .desc = "Change interface line state, only works for FXS interface.",
      .usage = "<infcconfigline | icl> if=[interface_id] line=<down | fwd | rev | ring | sleep>",
      .example = "EVCom >> infcconfigline if=0 line=active",
      .func = cmdInfcConfigLine},   
  
    { .code = CMD_INFC_CONFIG_HOOK,
      .name = "InfcConfigHook",
      .opcode = "ich",
      .desc = "Change interface hook state, only works for FXO interface.",
      .usage = "<infcconfighook | ich> if=[interface_id] hook=<on | off | flash>",
      .example = "EVCom >> infcconfighook if=0 hook=flash",
      .func = cmdInfcConfigHook},   

    { .code = CMD_INFC_CONFIG_HOOKTS,
      .name = "InfcConfighookTs",
      .opcode = "ict",
      .desc = "Change interface hook time detection threshold, only works for FXS interface.",
      .usage = "<infcconfighookts | ict> if=[interface_id]" \
      			"ts=[Min_flashTs,Max_flashTs,Min_releaseTs], Note: Hook release time(ms) to meet each hook state",
      .example = "EVCom >> infcconfighookts if=0 ts=250,600,700",
      .func = cmdInfcConfigHookTs},   
  
    { .code = CMD_INFC_CONFIG_RING,
      .name = "InfcConfigRing",
      .opcode = "icr",
      .desc = "Change interface ring profile configuration.",
      .usage = "<infcconfigring | icr> if=[interface_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: <cad0 | cad1 | cad2>=[ontime(ms),offtime(ms)], i.e. cad0=500,200\n" \
      			"attr: dur=[duration(ms)]," \
      			"attr: cid=[number], i.e. cid=1234\n" \
      			"attr: cidf=<us | dtmf | jp>, i.e.cidf=us Note: us-BELLCORE_FSK, dtmf-ETSI_DTMF, jp-NTT\n" \
      			"attr: cidb=[ring break], i.e. cidb=1 Note: Send CID at 1st ring break - between 1st and 2nd ring",
      .example = "EVCom >> infcconfigring if=0 dur=500 cad0=500,1000 cad1=0,0 cad2=0,0 cid=1234 cidf=us cidb=0",
      .func = cmdInfcConfigRing},   
  
    { .code = CMD_INFC_RING,
      .name = "InfcRing",
      .opcode = "ir",
      .desc = "Control interface ringing, only work for FXS type interface.",
      .usage = "<infcring | ir> if=[interface_id] dur=[duration(ms)], Note: duration is optional.",
      .example = "EVCom >> infcring if=0",
      .func = cmdInfcRing},   
  
    { .code = CMD_QUIT,
      .name = "Quit",
      .opcode = "q",
      .desc = "Shutdown DSP process and leave this application.",
      .usage = "<quit | q>",
      .example = "EVCom >> quit",
      .func = cmdQuit},   
};

#endif

