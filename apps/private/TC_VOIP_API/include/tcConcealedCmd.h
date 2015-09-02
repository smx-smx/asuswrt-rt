/*typedef enum{
	CHANNEL_UPDATE,
	TONE_SEQ_PLAY,
	MWI_PLAY,
	MWI_STOP,
	THREE_WAY_START,
	THREE_WAY_STOP,
	CID_PLAY,
	CID_STOP,
}TCEVOIPCMD;*/

/*typedef struct tcDSPInterface_s{
	int (*tcvChannelUpdate)(tcChannelParam_t* pChanelParam);
	int (*tcvTonePlaySeq)(tcToneSeqParam_t *pTones);
	int (*tcvMWIPlay)(tcMWIParam_t* pMWIParam);
	int (*tcvMWIStop)(tcMWIParam_t* cIdServiceType);
	int (*tcv3WayStart)(tc3WayParam_t* tc3WayParam);
	int (*tcv3WayStop)(tc3WayParam_t* tc3WayParam);
	int (*tcvCIdPlay)(tcCId_t *pCId);
	int (*tcvCIdStop)(int channel);
}tcDSPInterface_t;*/


typedef struct tcMWIParam_s{
	int channel;
	tcECIdType cIdServiceType;
}tcMWIParam_t;

//jrchen modify
#if 0
typedef struct tc3WayParam_s{
	int channel;
	int channelMate;
}tc3WayParam_t;
#endif
/* end jrchen modify */

typedef struct tcToneSeqParam_s
{
	/*
	 * VTSP_Return VTSP_toneLocalSequence(
		uvint   infc,
		uvint  *toneId_ptr,
		uvint   seqLength,
		uint32  control,
		uint32  repeat);
	 *
	 * */
	int channel;
	tcEDirection	direction;		/* The gene ration direction - towards local TDM or remote network */
	int		        onDuration;            /* On duration (identical to all digits) in msec.*/
    int			offDuration;           /* Off duration (identical to all digits) in msec.*/

	int			numOfTones;		/* Number of tones to play */
	tcToneElement_t	tones[MAX_TONE_NUM];	/* Array of indices to play (0-15 for DTMF or Call Progress Tone Index) */
	int control;
	int repeat;
} tcToneSeqParam_t;//acgTToneSequence
