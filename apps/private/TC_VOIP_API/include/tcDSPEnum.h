/*
 * tcDSPEnum.h
 *
 *  Created on: 2010/1/29
 *      Author: user
 */

#ifndef TCDSPENUM_H_
#define TCDSPENUM_H_

#define MAX_STR_LEN				128
#define MAX_DIAL_STRING_LEN 	16
#define MAX_CALLER_ID_LEN		16
#define TIME_STRING_LEN 		9
/* Generic enable/disable enum definition */
typedef enum
{
	CONTROL_OFF,
	CONTROL_ON
} tcEControl;

/* Echo canceler non-linear processor (NLP) mode enum definition */
typedef enum
{
	ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE_DISABLE,
	ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE_ENABLE,
} tcEEchoCancelerNonLinearProcessor;

typedef enum
{
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_0_NONE,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_1,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_2,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_3,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_4,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_5,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_6,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_7,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_8,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_9,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_10,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_11,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_12_MAX,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR_STATIC_DELAY_FOR_BYPASS_MODE
} tcEDynamicJitterFactor; /* Compatible with Tac49xDynamicJitterBufferOptimizationFactor */

typedef enum
{
	AGC_LOCATION_AT_ENCODER_INPUT,
	AGC_LOCATION_AT_DECODER_OUTPUT
} tcEAGCLocation;

typedef enum
{
	CALL_PROGRESS_CONTINUOUS,
	CALL_PROGRESS_CADENCE,
	CALL_PROGRESS_BURST,
	CALL_PROGRESS_SPECIAL_INFORMATION_TONE
} tcECallProgressSignalType;  /* Compatible with Tac49xCallProgressSignalType */


/* Voice input and output gain value enum definition */
typedef enum
{
    VOICE_GAIN_MUTE = -32, /*serena_modify*/
	VOICE_GAIN_minus31_DB,
	VOICE_GAIN_minus30_DB,
	VOICE_GAIN_minus29_DB,
	VOICE_GAIN_minus28_DB,
	VOICE_GAIN_minus27_DB,
	VOICE_GAIN_minus26_DB,
	VOICE_GAIN_minus25_DB,
	VOICE_GAIN_minus24_DB,
	VOICE_GAIN_minus23_DB,
	VOICE_GAIN_minus22_DB,
	VOICE_GAIN_minus21_DB,
	VOICE_GAIN_minus20_DB,
	VOICE_GAIN_minus19_DB,
	VOICE_GAIN_minus18_DB,
	VOICE_GAIN_minus17_DB,
	VOICE_GAIN_minus16_DB,
	VOICE_GAIN_minus15_DB,
	VOICE_GAIN_minus14_DB,
	VOICE_GAIN_minus13_DB,
	VOICE_GAIN_minus12_DB,
	VOICE_GAIN_minus11_DB,
	VOICE_GAIN_minus10_DB,
	VOICE_GAIN_minus9_DB,
	VOICE_GAIN_minus8_DB,
	VOICE_GAIN_minus7_DB,
	VOICE_GAIN_minus6_DB,
	VOICE_GAIN_minus5_DB,
	VOICE_GAIN_minus4_DB,
	VOICE_GAIN_minus3_DB,
	VOICE_GAIN_minus2_DB,
	VOICE_GAIN_minus1_DB,
	VOICE_GAIN_0_DB = 0, /*serena_modify*/
	VOICE_GAIN_plus1_DB,
	VOICE_GAIN_plus2_DB,
	VOICE_GAIN_plus3_DB,
	VOICE_GAIN_plus4_DB,
	VOICE_GAIN_plus5_DB,
	VOICE_GAIN_plus6_DB,
	VOICE_GAIN_plus7_DB,
	VOICE_GAIN_plus8_DB,
	VOICE_GAIN_plus9_DB,
	VOICE_GAIN_plus10_DB,
	VOICE_GAIN_plus11_DB,
	VOICE_GAIN_plus12_DB,
	VOICE_GAIN_plus13_DB,
	VOICE_GAIN_plus14_DB,
	VOICE_GAIN_plus15_DB,
	VOICE_GAIN_plus16_DB,
	VOICE_GAIN_plus17_DB,
	VOICE_GAIN_plus18_DB,
	VOICE_GAIN_plus19_DB,
	VOICE_GAIN_plus20_DB,
	VOICE_GAIN_plus21_DB,
	VOICE_GAIN_plus22_DB,
	VOICE_GAIN_plus23_DB,
	VOICE_GAIN_plus24_DB,
	VOICE_GAIN_plus25_DB,
	VOICE_GAIN_plus26_DB,
	VOICE_GAIN_plus27_DB,
	VOICE_GAIN_plus28_DB,
	VOICE_GAIN_plus29_DB,
	VOICE_GAIN_plus30_DB,
	VOICE_GAIN_plus31_DB
} tcEVoiceGain; /* Compatible with Tac49xVoiceGain */

/*20101217_serena_modify for country setting*/
typedef enum{
	SILENCE = 0,
	DIAL_TONE,
	RINGBACK_TONE, /*TODO check the idx of ring back tone*/
	ERROR_TONE ,
	CALL_WAITING_TONE,
	SECOND_DAIL_TONE,
	BUSY_TONE,
	DTMF_TONE_0,
	DTMF_TONE_1,
	DTMF_TONE_2,
	DTMF_TONE_3,
	DTMF_TONE_4,
	DTMF_TONE_5,
	DTMF_TONE_6,
	DTMF_TONE_7,
	DTMF_TONE_8,
	DTMF_TONE_9,
	DTMF_TONE_STAR,//*
	DTMF_TONE_HASH,//#
	DTMF_TONE_A,
	DTMF_TONE_B,
	DTMF_TONE_C,
	DTMF_TONE_D,
	TOTAL_TONE_NUM
}tcECallProgressToneType;
#define MAX_TONE_NUM TOTAL_TONE_NUM
/*End 20101217_serena_modify for country setting*/


/*T30 control data redundancy level enum definition*/
typedef enum
{
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_NONE,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_1,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_2,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_3,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_4,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_5,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_6,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL_7
} tcET30ControlLevel; /* Compatible with Tac49xT30ControlDataRedundancyLevel */

/* Caller ID standard enum definition */
typedef enum
{
    CALLER_ID_STANDARD_TELCORDIA_BELLCORE,
	CALLER_ID_STANDARD_ETSI,
	CALLER_ID_STANDARD_NTT,
	CALLER_ID_STANDARD_DTMF_CLIP_ETSI=8,
	CALLER_ID_STANDARD_DTMF_CLIP_DANISH,
	CALLER_ID_STANDARD_DTMF_CLIP_INDIAN,
	CALLER_ID_STANDARD_DTMF_CLIP_BRAZILIAN,
	CALLER_ID_STANDARD_DTMF_CLIP_NTT_DID
} tcECIDStandard; /* Compatible with Tac49xCallerIdStandard */

/*Image data redundancy enum definition*/
typedef enum
{
    IMAGE_DATA_REDUNDANCY_LEVEL_NONE,
    IMAGE_DATA_REDUNDANCY_LEVEL_1,
    IMAGE_DATA_REDUNDANCY_LEVEL_2,
    IMAGE_DATA_REDUNDANCY_LEVEL_3
} tcEImageDataLevel; /* Compatible with Tac49xImageDataRedundancyLevel */

/* Fax relay max rate enum definition */
typedef enum
{
	FAX_MAX_RATE_2_4_KBPS,
	FAX_MAX_RATE_4_8_KBPS,
	FAX_MAX_RATE_7_2_KBPS,
	FAX_MAX_RATE_9_6_KBPS,
	FAX_MAX_RATE_12_KBPS,
	FAX_MAX_RATE_14_4_KBPS
} tcEFaxMaxRate; /* Compatible with Tac49xMaxRate */

/* Fax and Modem transport enum definition */
typedef enum
{
	FAX_MODEM_TRANSPORT_DISABLE,
	FAX_MODEM_TRANSPORT_RELAY,		/* T.38 */
	FAX_MODEM_TRANSPORT_BYPASS,
	FAX_MODEM_TRANSPORT_VBD,	/* Not compatible with DSP drivers */

} tcEFaxTxMethod; /* Compatible with Tac49xFaxModemTransferMethod */
/* In-band signaling transport method enum definition */
typedef enum
{
	SIG_TRANSPORT_VOICE_MUTE,
	SIG_TRANSPORT_RELAY,			/* RFC 2833 */
	SIG_TRANSPORT_TRANSPARENT,
	SIG_TRANSPORT_RELAY_ENABLE_VOICE_MUTE
} tcESignalingTransportMethod; /* Compatible with Tac49xIbsTransferMode */
/* channel coder type enum definition */

typedef enum
{
    IBS_DETECTION_TO_LOCAL_TDM,
    IBS_DETECTION_TO_NETWORK
}tcEIbsDirection;

typedef enum
{
/*20100821 serena_modify: the value is PT for audio encodings*/
	CODER_G711MULAW				= 0,
	CODER_G726_32				= 2, 
	CODER_G723					= 4, 
	CODER_G711ALAW  			= 8, 
	CODER_G22  					= 9,
	CODER_G729			    	= 18,
	CODER_G726_40				= 21, 
	CODER_G726_24				= 22, 
	CODER_G726_16				= 23,
	CODER_T38			    	= 24, 

/*
	CODER_CHANNEL_DISABLED		= 0,
	CODER_TRANSPARENT			= 1,
	CODER_G711ALAW  			= 3,
	CODER_G711MULAW				= 4,
	CODER_G726_16				= 5,
	CODER_G726_24				= 6,
	CODER_G726_32				= 7,
	CODER_G726_40				= 8,
	CODER_G727_16				= 9,
	CODER_G727_24_16			= 10,
	CODER_G727_24				= 11,
	CODER_G727_32_16			= 12,
	CODER_G727_32_24			= 13,
	CODER_G727_32				= 14,
	CODER_G727_40_16			= 15,
	CODER_G727_40_24			= 16,
	CODER_G727_40_32			= 17,
	CODER_G728_16	        	= 18,
	CODER_LINEAR_16_BIT_PCM		= 19,
	CODER_G723LOW				= 25,
	CODER_G723HIGH				= 26,
	CODER_G729			    	= 27,
	CODER_GSM_FULL_RATE_13_2	= 28,
	CODER_MICROSOFT_GSM_13_2	= 29,
	CODER_EVRC_8_8K_0_8_K   	= 30,
	CODER_QCELP_8           	= 31,
	CODER_QCELP_13          	= 32,
	CODER_LINEAR_8_BIT_PCM  	= 33,
	CODER_G729E_11_8K      		= 34,
	CODER_GSM_EFR          		= 35,
	CODER_ILBC_15_2K			= 36,
	CODER_ILBC_13_33K			= 37,
	CODER_LINEAR_16_BIT_PCM_SAMPLE_RATE_16KHZ = 38,
	CODER_G722_48K				= 39,
	CODER_G722_56K				= 40,
	CODER_G722_64K				= 41,
	CODER_EVRC_B				= 42,
	CODER_G729_EV				= 43
	*/
} tcECoder; /* Compatible with Tac49xCoder */

/* Event polling method enum definition */
typedef enum
{
	POLLING_BLOCKING,		/* Blocking mode - function will not return until an event is received */
	POLLING_NON_BLOCKING	/* Non-blocking mode - funciton will return immediately with event type or
									   EVENT_TYPE_NO_EVENT */
} tcEPollingIO;

typedef enum
{
	PROTOCOL_PROPRIETARY,
	PROTOCOL_RTP,
	PROTOCOL_RTCP,
	PROTOCOL_FAX,
} tcEProtocol; /* Compatible with Tac49xProtocol */

typedef enum
{
	SILENCE_COMPRESSION_MODE_DISABLE,
	SILENCE_COMPRESSION_MODE_ENABLE_NOISE_ADAPTATION_ENABLE
} tcESilenceCompressionMode;

typedef enum
{
	CALLER_ID_SERVICE_TYPE_1_ON_HOOK,
	CALLER_ID_SERVICE_TYPE_2_OFF_HOOK
} tcECIdType; /* Compatible with Tac49xCallerIdType */

/* Tone generation type enum definition */
typedef enum
{
	TONE_TYPE_DTMF,
	TONE_TYPE_CALL_PROGRESS_TONE
} tcEToneType;
/* Signal direction enum definition */
typedef enum
{
	DIRECTION_TO_LOCAL_TDM = 0,
	DIRECTION_TO_NETWORK = 1,
	DIRECTION_TO_LOCAL_TDM_AND_NETWORK = 3,	/* Not support on AC48x Drivers */
} tcEDirection; /* Compatible with Tac49xIbsStringGeneratorRedirection; */

/* DSP and FXS/FXO event types enum definiton */
typedef enum
{
	/* DSP events : */
	EVENT_TYPE_NO_EVENT=0,				/* Indicates that no event is currently avilable */
	EVENT_TYPE_DTMF_START,				/* Beginning of DTMF digit (0-9,*,#,A-D). More info in Tone struct. */
	EVENT_TYPE_DTMF_END,					/* End of DTMF digit (0-9,*,#,A-D). More info in Tone struc.  */
	EVENT_TYPE_CALL_PROGRESS_TONE_START,/* Beginning of call progress tone (dial tone, ...). More info in Tone struct. */
	EVENT_TYPE_CALL_PROGRESS_TONE_END,	/* End of call progress tone (dial tone, ...). More info in Tone struct. */
	EVENT_TYPE_CALLER_ID_END,			/* Caller ID end message . More info in CallerID struct. */
	EVENT_TYPE_FAX_START,				/* Fax signal detected. More info in FaxModem struct. */
	EVENT_TYPE_FAX_SIGNAL,
	EVENT_TYPE_MODEM_START,				/* Modem signal detected. More info in FaxModem struct. */
	EVENT_TYPE_FAX_OR_MODEM_START,		/* Ambigous fax or modem signal detected (or switch to bypass). More info in FaxModem struct. */
	EVENT_TYPE_FAX_OR_MODEM_END,		/* End of fax or modem - back to voice */
	EVENT_TYPE_RTCP_INFO,				/* RTCP information */
	EVENT_TYPE_RTCP_XR_INFO,				/* RTCP-XR information */
	/* FXS/FXO events : */
	EVENT_TYPE_ON_HOOK=100,			/* Receiver On-hook */
	EVENT_TYPE_OFF_HOOK,				/* Receiver Off-hook */
	EVENT_TYPE_FLASH_HOOK,			/* Flash hook signal */
	EVENT_TYPE_RING_START,			/* Start of ring signal */
	EVENT_TYPE_RING_END,				/* Start of ring signal */
	EVENT_TYPE_POLARITY_REVERSAL,	/* Tip/Ring polarity reversal */
} tcEEventType;

typedef enum
{
	INITIATING_FLAG_MEDIA_GATEWAY_STATE_IS_INITIATED_BY_REMOTE_MEDIA_GATEWAY,
	INITIATING_FLAG_MEDIA_GATEWAY_STATE_IS_INITIATED_BY_DSP
} tcEInitiatingFlag;

typedef enum
{
	REASON_IDENTIFIER_CODE_Reason_NULL = 0,
	REASON_IDENTIFIER_CODE_V8CM,
	REASON_IDENTIFIER_CODE_V8JM,
	REASON_IDENTIFIER_CODE_V32AA_1800HZ,
	REASON_IDENTIFIER_CODE_V32AC,
	REASON_IDENTIFIER_CODE_V22USB1_2250HZ,
	REASON_IDENTIFIER_CODE_V22SB1,
	REASON_IDENTIFIER_CODE_V22bisS1,
	REASON_IDENTIFIER_CODE_V21Channel2,
	REASON_IDENTIFIER_CODE_V21Channel1,
	REASON_IDENTIFIER_CODE_V21Preamp,
	REASON_IDENTIFIER_CODE_V23ForwardChannel_1300Hz,
	REASON_IDENTIFIER_CODE_V23LowChannel,
	REASON_IDENTIFIER_CODE_BELL103ANS_2225Hz,
	REASON_IDENTIFIER_CODE_V21Channel2HDLCFlag,
	REASON_IDENTIFIER_CODE_Indeterminatesignal,
	REASON_IDENTIFIER_CODE_Silence,
	REASON_IDENTIFIER_CODE_CNG,
	REASON_IDENTIFIER_CODE_Voice,
	REASON_IDENTIFIER_CODE_TimeOut,
	REASON_IDENTIFIER_CODE_PStateTransition,
	REASON_IDENTIFIER_CODE_Cleardown,
	REASON_IDENTIFIER_CODE_CED_ANS,
	REASON_IDENTIFIER_CODE_ANSam,
	REASON_IDENTIFIER_CODE_ANS_Phasereversal,
	REASON_IDENTIFIER_CODE_ANSam_Phasereversal,
	REASON_IDENTIFIER_CODE_Reserve1,
	REASON_IDENTIFIER_CODE_Reserve2,
	REASON_IDENTIFIER_CODE_Reserve3,
	REASON_IDENTIFIER_CODE_Reserve4,
	REASON_IDENTIFIER_CODE_V8bisCRe,
	REASON_IDENTIFIER_CODE_V8bisCRd,
	REASON_IDENTIFIER_CODE_TIA825A45bits,
	REASON_IDENTIFIER_CODE_TIA825A50bits,
	REASON_IDENTIFIER_CODE_EDT,
	REASON_IDENTIFIER_CODE_Bell103modem,
	REASON_IDENTIFIER_CODE_V21TextTelephone,
	REASON_IDENTIFIER_CODE_V23Text,
	REASON_IDENTIFIER_CODE_V18TextTelephone,
}tcEReasonIdentifierCode;

typedef enum
{
	RTCP_RR_Rx,	/* Rx Receiver Report */
	RTCP_SR_Rx,	/* Rx Sender Report */
	RTCP_RR_Tx,	/* Tx Receiver Report */
	RTCP_SR_Tx		/* Tx Sender Report */
}tcERTCPEventType;

typedef enum
{
	RTCP_XR_INFO,	/* RTCP XR Local Information */
	RTCP_XR_REMOTE_INFO,	/* RTCP XR Remote Information */
}tcERTCPXREventType;

typedef enum
{
    RTCP_PARAM_INDEX_TRANSMIT_PACKET_COUNT,
    RTCP_PARAM_INDEX_TRANSMIT_OCTETS_COUNT,
    RTCP_PARAM_INDEX_RECEIVE_PACKET_COUNT,
    RTCP_PARAM_INDEX_RECEIVE_OCTETS_COUNT,
    RTCP_PARAM_INDEX_JITTER,
    RTCP_PARAM_INDEX_PACKET_LOSS,
    RTCP_PARAM_INDEX_ROUND_TRIP_DELAY
}tcERTCPRegularParam;  /* Compatible with Tac49xSetRtcpParametersAttr */


typedef enum
{
	PHONE_CALLER_NUMBER_PRESENT,
	PHONE_CALLER_NUMBER_ABSENT,
	PHONE_CALLER_NUMBER_SUPPRESSED,
} tcEPhoneCallerNumberStatus;

typedef enum
{
	PHONE_CALLER_NAME_PRESENT,
	PHONE_CALLER_NAME_ABSENT,
	PHONE_CALLER_NAME_SUPPRESSED,
} tcEPhoneCallerNameStatus;

/* jrchen modify for multifunctional update channel*/
typedef enum{
	UPDATE_OPEN_SECOND_CHANNEL,
	UPDATE_SWITCH_CHANNEL,
	UPDATE_CHANGE_CHANNEL_MODE,
}tcEUpdateChannelCmdType;	/*20100821 serena_modify*/
/* end jrchen modify */

/*20100821 serena_modify*/
typedef enum
{
    SDPCONNECTMODE_NOTSET    = 0,
    SDPCONNECTMODE_SENDONLY,       /* "sendonly" */
    SDPCONNECTMODE_RECVONLY,       /* "recvonly" */
    SDPCONNECTMODE_SENDRECV,       /* "sendrecv" */
    SDPCONNECTMODE_INACTIVE        /* "inactive" */
} tcESdpConnectionMode;

/*20101206 pork added for FXO support*/
typedef enum{
    CTRL_INFC_POWER_OPEN   = 1,
    CTRL_INFC_POWER_TEST   = 2,
    CTRL_INFC_POWER_ACTIVE = 3,
    CTRL_INFC_POWER_DOWN   = 4,
    CTRL_INFC_POLARITY_FWD = 5,
    CTRL_INFC_POLARITY_REV = 6,
    CTRL_INFC_ONHOOK       = 7,
    CTRL_INFC_OFFHOOK      = 8,
    CTRL_INFC_FLASH        = 9
}tcECtrlLine;

#endif /* TCDSPENUM_H_ */
