#ifndef TCDSPINTERFACE_H
#define TCDSPINTERFACE_H

#include "tcVoIPGenDef.h"
#include "tcDSPEnum.h"


/************************************************************************
 *				S T R U C T U R E   D E C L A R A T I O N S
 *************************************************************************
 */
typedef struct tcCallProgressTone_s
{
	/*VTSP_ToneTemplate
	 * typedef struct {
		vint    freq1; //ToneAFrequency
		vint    freq2; //ToneB_OrAmpModulationFrequency
		vint    power1; //toneALv
		vint    power2; //toneBLv
		vint    cadences; //supportedCadence
		vint    make1; //cad1OnDuration
		vint    break1; //cad1OffDuration
		vint    repeat1; //cad1Repeat
		vint    make2;
		vint    break2;
		vint    repeat2;
		vint    make3;
		vint    break3;
		vint    repeat3;
		} VTSP_ToneTemplate;
	 * */
	tcECallProgressSignalType    	type;  /*Call Progress Signal Type.*/
	int							toneAFreq;  /*F1 */
	int							toneBFreq;  /*F2 */ /*20101217_serena_modify for country setting*/

	int							toneALv;  /*P1 Output level of the low frequency tone, in call progress generation. Output levels, ranging from -63dBm to 0 dBm. Units are in (-dBm)*/
	int							toneBLv;  /*P2 Output level of the high frequency tone, in Call Progress generation. Output levels, ranging from -63 dBm to 0 dBm. Units are in (-dBm)*/
	int 						supportedCadence; /*cadences*/
	int							detectionTimeOrCadenceFirstOnOrBurstDuration;  /*!< The content of this field is according to the signal type: @li Continuous (1) = detection time. @li Cadence (2) = Ton state. Burst @li (3) = duration.*/
	int							cad1OffDuration;  /*break1 When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int 						cad1Repeat; /*repeat1 */

	int							cad2OnDuration; /*make2*/
	int							cad2OffDuration;  /*break2 When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int 						cad2Repeat; /*repeat2 */

	int							cad3OnDuration; /*make3*/
	int							cad3OffDuration;  /*break3 When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int 						cad3Repeat; /*repeat3 */

	/*int							cad4OnDuration; make3
	int							cad4OffDuration;  break3 When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.
	int 						cad4Repeat; repeat3 */
	tcEControl					cadenceVoiceAddedWhileFirstOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/
	tcEControl					cadenceVoiceAddedWhileSecondOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/
	tcEControl					cadenceVoiceAddedWhileThirdOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/
	tcEControl					cadenceVoiceAddedWhileFourthOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/
} tcCallProgressTone_t;

typedef struct acDSPDevInfor_s
{
	char     *pProg;
	int        progSize;
	int        devCount;

} acDSPDevInfor_t;

typedef struct acAdvChannelParam_s
{
	int faxModemJitter;
}acAdvChannelParam_t;

typedef struct tcDSPInitParam_s
{
	int 			numOfCallProgressTones;	/* Number of defined call progress tones */
// 	tcCallProgressTone_t	callProgressTones[MAX_TONE_NUM]; /*20101217_serena_modify remove it*/

	char		remoteRecordingIpAddress[MAX_STR_LEN]; /* IP Address to which the recording packet will be send to */
	int			remoteRecordingPort;/* Port to which the recording packet will be send to */
    /*VTSP_TaskConfig*/
	int    vtspAddStackSize;
    int    rtcpAddStackSize;
    uint16  rtcpInternalPort;
	/**** AC49x Gen 3 related parameters ****/
	acDSPDevInfor_t 	primDSPDevInfor;
    acDSPDevInfor_t 	secondDSPDevInfor;
	acAdvChannelParam_t advChannelParam;
	/**** AC48x Gen 2 related parameters ****/
	int 			dspVerNum;

	char countryName[4];	/*20101217_serena_added for country setting*/
	
} tcDSPInitParam_t;

typedef struct tcVQMonitorParam_s
{
	tcEControl rtcpXrEnable;         /*Enables RTCP-XR (Voice Quality Monitor - RFC3611) */
} tcVQMonitorParam_t;

typedef struct tcEchoCancellerParam_s
{
	tcEControl echoCanceler;         /*!< Echo canceler operation mode. Default = CONTROL__ENABLE.*/
	tcEEchoCancelerNonLinearProcessor nonLinearProcessor;  /*!< Echo canceler non-linear processor mode. @n Default = ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__MEDIUM_SENSITIVITY. @n Refer to ::Tac49xEchoCancelerNonLinearProcessor.*/
} tcEchoCancellerParam_t;

typedef struct tcJitterBufferParam_s
{
	tcEDynamicJitterFactor	dynamicJitterFactor;  /*!< Dynamic Jitter Buffer Optimization Factor. Default = 7. @li 0 = Static Jitter Buffer. @li 1-12 = This an empirical, user-defined factor, where: 1 is optimal for delay and 12 is optimal for voice quality. @li 13 = Optimal for fax and data transfer. @li 14-15 = Reserved.*/
	int		jitterMinDelay;				/*!< Minimum delay of Dynamic Jitter Buffer, or fixed delay in Static Jitter Buffer, in msec resolution. @n Range: 0 - 280 msec. @n Default = 35 msec. */
	int		jitterMaxDelay;				/*!< Maximum delay of Dynamic Jitter Buffer in msec resolution. @n Range: 0 - 300 msec. @n Default = 150 msec. @n @b Note: The actual maximum size limit varies from product to product.*/
} tcJitterParam_t;

typedef struct tcGainParam_s
{
	tcEVoiceGain		voiceOutputGain;		/*!< Voice decoder output gain. Default = VOICE_GAIN__0_DB.*/
	tcEVoiceGain		inputGain;			/*!< Input gain. Default = VOICE_GAIN__0_DB.*/
	tcEControl		agcEnable;			/*!< Enables Automatic Gain Control. Default = CONTROL__DISABLE.*/
	tcEAGCLocation   	agcLocation;              	/*!< @li ENCODER_INPUT = AGC is placed at encoder input (default). @li DECODER_OUTPUT = AGC is placed at decoder output.*/
	int             		agcTargetEnergy;	    	/*!< The desired output energy in -dBm. @n Default = -19 dBm.                               */
} tcGainParam_t;

typedef struct tcCIdParam_s
{
	tcECIDStandard	cIdStandard;			/*!< Caller ID standard. Default = CALLER_ID_STANDARD__TELCORDIA_BELLCORE.*/
	/* Currently not supported */
	tcEControl			cIdOnHookDetection;	/*!< Enables the Call ID "on hook" service (Type 1) detection. Default = CONTROL__DISABLE.*/
	tcEControl			cIdOffHookDetection;	/*!< Enables the Caller ID "off hook" service (Type 2, CID on Call Waiting) detection. Default = CONTROL__DISABLE.*/
} tcCIdParam_t;

typedef struct tcFaxModemParam_s
{
	tcEFaxTxMethod	faxTransferMethod;		/*!< Fax transport method. Default = FAX_MODEM_TRANSPORT__DISABLE.*/
	tcEFaxTxMethod	modemTransferMethod;	/*!< Modem transport method. Default = FAX_MODEM_TRANSPORT__DISABLE.*/
	tcEFaxMaxRate				faxMaxRate;				/*!< Fax Relay Max Rate. Default = FAX_MAX_RATE__14_4_KBPS.*/
	tcEControl					faxErrorCorrection;		/*!< Enable the T.30 fax relay error correction mode Default = CONTROL__ENABLE.*/
	tcEControl					cngRelay;				/*!< CNG Relay on the detection side.  ACG_CONTROL__ENABLE = CNG signal is relayed using T.38 protocol */
	tcECoder					bypassCoder;			/*!< The codec used for fax or modem bypass */
	tcEImageDataLevel imageDataRedundancyLevel; /*< Redundancy for output Image Data (2400?4400 bps). @li 0 = No Redundancy (Default). @li 1-3 = Redundancy level 1-3.*/
	tcET30ControlLevel t30ControlDataRedundancyLevel;/*!< Redundancy for output T.30 Control Data (300 bps). @n (Default = 0). @li 0 = No Redundancy. @li 1-7 = Redundancy levels 1-7.*/
} tcFaxModemParam_t;

typedef struct tcSignalingParam_s
{
	tcESignalingTransportMethod	dtmfTransportMethod;	/* The mothod for DTMF signals transport */
	tcEIbsDirection redirection;             /* Configures the IBS detection direction */
    tcEControl detectCallProgress;                                    /*Enables Call Progress Tones detection*/
    tcEControl detectDTMFRegister;                                  /*Enables DTMF detection*/
} tcSignalingParam_t;

typedef struct tcRTPStreamParam_s
{
	/*
	 * Typedef struct {
    vint            streamId; //ch ID
    VTSP_StreamDir  dir;
    uint16          peer;
    vint            encoder;//coder
    vint            encodeTime[VTSP_ENCODER_NUM];
    vint            encodeType[VTSP_ENCODER_NUM];
    vint            decodeType[VTSP_DECODER_NUM];
    uint16          extension;
    uint16          dtmfRelay;
    uint16          silenceComp;
    uint32          confMask; // for conference mask
    uint32          remoteIpAddr;
    uint16          remoteDataPort;
    uint16          remoteControlPort;
    uint32          localIpAddr;
    uint16          localDataPort;
    uint16          localControlPort;
    uint16          srtpSecurityType;
    char            srtpSendKey[VTSP_SRTP_KEY_STRING_MAX_LEN];
    char            srtpRecvKey[VTSP_SRTP_KEY_STRING_MAX_LEN];

	} VTSP_Stream;
	 *
	 *
	 * */
	int chId;
	int direction;
	int peer;
	tcECoder	coder;	/* The channel encoder and decoder codec type */
	tcESignalingTransportMethod	dtmfMethod;	/* The mothod for DTMF signals transport */ /*20101119_serena_added for dtmf*/
	int			pTime;	/* The tx packetization time in ms */
	int		txVoicePayloadType;				/* RTP payload type for transmitted voice packets */
	int		rxVoicePayloadType;				/* RTP payload type for received voice packets */
	int		txDTMFRelayPayloadType;			/* RTP payload type for transmitted RFC2833 packets */
	int		rxDTMFRelayPayloadType;			/* RTP payload type for received RFC2833 packets */
	int		faxBypassPayloadType;			/* RTP payload type for fax bypass packets */
	int		modemByassPayloadType;			/* RTP payload type for modem bypass packets */
    char          remoteIpAddr[128];
    uint16          remoteDataPort;
    uint16          remoteControlPort;
    char          localIpAddr[128];
    uint16          localDataPort;
    uint16          srtpSecurityType;
    uint32          confMask;
    uint16          extension;
    char            srtpSendKey[MAX_STR_LEN];
    char            srtpRecvKey[MAX_STR_LEN];
	tcESilenceCompressionMode		silenceCompressionMode;	    /* Encoder Silence Compression Mode. @n Default = SILENCE_COMPRESSION_MODE__DISABLE.*/
} tcRTPStreamParam_t;

typedef struct tcChannelParam_s
{
	int channel;
	tcRTPStreamParam_t		rtpStreamParam;	/* The RTP media stream parameters */
	tcSignalingParam_t			signalingParam;	/* In-band Signaling parameetrs */
	tcFaxModemParam_t		faxModemParam;		/* Fax and Modem configuration Param */
	tcCIdParam_t			cidParam;		/* Caller ID Parameters */
	tcGainParam_t			gainParam;			/* Caller ID Parameters */
	tcJitterParam_t		jitterBufferParam;	/* Jitter Buffer Parameters */
	tcEchoCancellerParam_t		echoCancellerParam;/* Echo Canceller Parameters */
	tcVQMonitorParam_t		vqMonitorParam;/* RTCP-XR Parameters */
	
	/* jrchen modify for multifunctional update channel*/
	int channelMode;
/*20100821 serena_modify*/
	tcEUpdateChannelCmdType cmdType;	
	/* end jrchen modify*/
} tcChannelParam_t;

typedef struct tcToneElement_s
{
	tcEToneType	type;			/* The type of tone (DTMF, call progress tone, ...) */
	int			toneIdx;		/* The tone to play - 0-15 for DTMF or Call Progress Tone Index */
} tcToneElement_t;

typedef struct tcToneParam_s
{
	/*
	 * D2
	 * VTSP_Return VTSP_toneLocal(
        uvint  infc, // replace by channel number
        uvint  template, //toneElement.ToneIndex
        uvint  repeat,
        uint32 maxTime);
	 * */
	int channel;
	tcToneElement_t	toneElement;
	tcEDirection		direction;		/* The generation direction - towards local TDM or remote network */
	int				onDuration;		/*On duration in msec.*/
	int				offDuration;		/* Off duration in msec.*/
	int repeat;
	uint32 maxtime;

} tcToneParam_t;


typedef struct tcRegularRTCPParam_s
{
	tcERTCPRegularParam	RTCPParamIndex;       /* Index of the modified parameter */
	int						RTCPParamValue;       /* The value of the parameter*/
} tcRegularRTCPParam_t;

typedef struct tcMediaPut_s
{
	int channel;
	char* 		mediaBuf;			/* Pointer to the transmitted packet */
	int 			bufLen;				/* Size of the transmitted packet in bytes */
	tcEProtocol	mediaProtocol;	/* Media type to be decoded */
} tcMediaPut_t;

typedef struct tcMediaGet_s
{
	int 			channel;				/* [Out] - The channel from which the media packet is recevied */
	char* 		mediaBuf;				/* [Out] - Pointer to the received packet */
	int 			bufLen;					/* [Out] - Size of the received packet in bytes */
	tcEProtocol	mediaProtocol;		/* [Out] - Media type of the received packet */
	tcEPollingIO	pollingIOType;	/* [In] - Method of acgGetMedia() - Blocking / Non Blocking */
} tcMediaGet_t;


typedef union rtcpInfo{
	struct
	{
		tcERTCPEventType  reportType;    	/*According to enum acgERTCPEventType*/
		unsigned int  pktCnt;   			/*Number of packets transmitted*/
		unsigned int  OctetCnt;   			/*Number of Octets transmitted*/
		unsigned int  Jitter;        				/*Delay Jitter [msec]*/
		unsigned int  roundTrip;     			/*Local<->Remote round trip delay [msec]*/
		unsigned int  fractionLost;  			/*Fraction of packets lost */
		unsigned int  cumLost;       			/*Number of packets lost */
		unsigned int  extHighSeq;    			/*Extended highest sequence number recieved*/
		unsigned int  ssrcSender;    			/*Synchronization Source of sender*/
		unsigned int  rtpTimeStamp;  		/*RTP TimeStamp of the RTCP report*/
		unsigned int  pktsReceived; 		/*Number of received packets*/
		unsigned int  OctetsReceived; 		/*Number of transmitted octets */
	}rtcp;

	struct
	{
		tcERTCPXREventType ReportType;	/*According to enum acgERTCPXREventType*/
		unsigned char MOS_LQ;				/*The estimated mean opinion score for listening quality */
		unsigned char MOS_CQ;				/*The estimated mean opinion score for conversational quality*/
		unsigned char RFactor; 		  		/*A voice quality metric describing the segment of the call that is carried over this RTP session*/
	}rtcp_xr;

} rtcpInfo_u;	/* Caller ID event information */

typedef union eventInfo{
	struct
	{
		tcEToneType	type;		/* The type of tone (DTMF, call progress tone, ...) */
		int			toneIdx;	/* The tone to play - 0-15 for DTMF or Call Progress Tone Index */
		tcEDirection	direction;	/* The generation direction - towards local TDM or remote network */
		int			duration;		/* Tone duration in [ms] - relevant for Call Progress Tone */
	} tone;	/* DTMF or Call Progress Tone start or end event information */

	/*** Currently Caller ID detection is not supported ***/
	struct
	{
		int tbd;	/* Need to be define */
	} cId;	/* Caller ID event information */

	struct
	{
		tcEInitiatingFlag	initFlag;		/* Indicating whether the local or remote initiated the signal */
		tcEReasonIdentifierCode reasonIdentifierCode;
	} faxModem;	/* Fax or Modem detection event information */

	rtcpInfo_u rtcpInfo;	/* Caller ID event information */

}eventInfo_u;

typedef struct tcEventInfo_s
{
	/*
	 * typedef struct {
		VTSP_EventMsgCodes code;//EventType
		uvint              infc;//channel
		uvint32            tick;
		union {
            struct {
			  uint32 reason;
			 uint32 arg1;
			  } echoCanc;
			} msg;
		} VTSP_EventMsg;
	 *
	 *
	 * */
	int 				channel;		/* The channel from which the event is recevied */

	tcEEventType	eventType; 	/* The event type code */
	uint32            tick;
	tcEPollingIO pollingIOType;
	eventInfo_u eventInfo;
} tcEventInfo_t;

typedef struct tcFax_s{
	int channel;
	tcEFaxTxMethod faxTxMethod;
/*20100821 serena_modify*/
	uint8 faxCodec;
}tcFax_t;

typedef struct tcFXSInitParam_s
{
	void     *coefficients;

} tcFXSInitParam_t;

typedef struct tcRingInitParam_s
{
	 int channel;
	 uint8        numRings;
	 uint8         maxTime;
	 void *cid_ptr;

} tcRingInitParam_t;

/* jrchen modify */
typedef struct tc3WayParam_s{
	int channel;
	int channelMate;
}tc3WayParam_t;
/* end jrchen modify */
/*20100821 serena_modify*/
typedef struct tcCId_s
{
	int channel;
	tcECIdType			cIdServiceType;	/* The caller Id service type (type 1 or type 2) */
	char 							num[MAX_DIAL_STRING_LEN]; 			/* Caller's number */
	char 							name[MAX_CALLER_ID_LEN]; 				/* Caller's user-id */

	/*** Currently Time and addr are not supported ***/
	char 							time[TIME_STRING_LEN];					 /* System time, the format is MMDDhhmm */
	char 							addr[MAX_STR_LEN]; 					/* Caller's IP address */

	/*** Currently NumberStatus and NameStatus are not supported ***/
	tcEPhoneCallerNumberStatus 	numStat; /* is 'number' valid? */
	tcEPhoneCallerNameStatus		nameStat; /* is 'name' valid? */
} tcCId_t;

typedef struct tcFXOCtrl_s
{
	int fxoInfc;
	int fxsInfc;
	tcECtrlLine infcCtrl;
} tcFXOCtrl_t;

typedef struct tcFXOPSTNStat_s
{
	int fxoInfc;
	int pstnStat;
} tcFXOPSTNStat_t;

typedef struct tcDSPInterface_s{

	int venderId;
	int (*tcvDSPParaminit) (tcDSPInitParam_t* pInitParam);
	int (*tcvDSPStart)(tcDSPInitParam_t* pInitParam);
	int (*tcvDSPClose)(void);
	int (*tcvChannelParamInit)(tcChannelParam_t* pChanelParam);
	int (*tcvChannelOpen)(tcChannelParam_t* pChanelParam);
	int (*tcvChannelClose)(int channel);
	int (*tcvRTPActive)(tcRTPStreamParam_t* rtpStream);
	int (*tcvRTPDeactive)(int channel);
	int (*tcvTonePlay)(tcToneParam_t *pTone);
	int (*tcvToneStop)(int channel);
	int (*tcvMediaPut)(tcMediaPut_t *pDecodeMedia);
	int (*tcvMediaGet)(tcMediaGet_t *pMediaGet);
	int (*tcvEventPut) (tcEventInfo_t *pEvent);
	int (*tcvEventGet) (tcEventInfo_t *pEvent);
	int (*tcvTelEventGet) (tcEventInfo_t *pEvent);
	int (*tcvSlicInit)(tcFXSInitParam_t *pFXSInitParam);
	int (*tcvSlicClose)(void);
	int (*tcvRingStart)(tcRingInitParam_t* tcRingInitParam);
	int (*tcvRingStop)(int channel);
	int (*tcvFaxStart)(tcFax_t* faxTxMethod);
	int (*tcvFaxStop)(tcFax_t* faxTxMethod);
}tcDSPInterface_t;

/************************************************************************
 *				F U N C T I O N   D E C L A R A T I O N S
 *************************************************************************
 */
int tcDSPInterfaceReg(tcDSPInterface_t *pTCDSPInterface);
void dspCmdOperate(cmdNode_t* cmdNode);
#endif
