/*
 * tcDSPGenDef.h
 *
 *  Created on: 2010/1/29
 *      Author: user
 */

#ifndef TCDSPGENDEF_H_
#define TCDSPGENDEF_H_
/******************Macros*****************************************/
#define TC_VOIP_SUCCESS 1
#define TC_VOIP_FAIL 0
#define AUDIOCODES 1
#define D2 2
#define SOCK_PATH "/tmp/tcVoIPAPI_sock"
#define CMDNODE_BUF_SIZE 1024

/****************** typedefs ***********************************/
#ifndef UINT8	/*20100520 serena_modify*/
#define UINT8
typedef unsigned char uint8;
#endif
#ifndef UINT16
#define UINT16
typedef unsigned short int uint16;
#endif
#ifndef UINT32
#define UINT32
typedef unsigned long int uint32;
#endif

/*
 * 20100412 pork commented:
 * After adding any additional cmd,
 * please follow the cmd sequence to add the corresponding function
 * in function pointer array table.
 */
typedef enum{
	DSP_DEFAULT_PARAM,
	DSP_START,
	DSP_CLOSE,
	CHANNEL_DEFAULT_PARAM,
	CHANNEL_OPEN,
	CHANNEL_CLOSE,
	RTP_ACTIVE,
	RTP_DEACTIVE,
	TONE_PLAY,
	TONE_STOP,
	MEDIA_PUT,
	MEDIA_GET,
	EVENT_PUT,
	EVENT_GET,
	TEL_EVENT_GET,
	SLIC_INIT,
	SLIC_CLOSE,
	RING_START,
	RING_STOP,
	FAX_START,
	FAX_STOP,
/* jrchen modify for 3way conference */
	THREE_WAY_START,
	THREE_WAY_STOP,
	CHANNEL_UPDATE,	
/* end jrchen modify */
/*20100821 serena_modify*/
	CID_PLAY,
/*20101206 pork added for FXO support*/
	FXO_CTRL,
	FXO_PSTNSTAT_GET
}TCEVOIPCMD;

typedef struct cmdNode_s{
	TCEVOIPCMD cmd;
	int retVal;
	char buf[CMDNODE_BUF_SIZE];
}cmdNode_t;

/**************Function declarion*********/
int _tcVoIPAPIGet(TCEVOIPCMD cmd,void* buf,int len);
int _tcVoIPAPISet(TCEVOIPCMD cmd,void* buf,int len);

#define tcVoIPAPIGet(a,b) _tcVoIPAPIGet(a,b,sizeof(*b))
#define tcVoIPAPISet(a,b) _tcVoIPAPISet(a,b,sizeof(*b))

#endif /* TCDSPGENDEF_H_ */
