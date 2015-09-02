#define VOIP_LINE_NUM 2
#define SUPPORT_CODEC_NUM 4
/* serena_modify */
#define PHONE_BOOK_NUM 10
/* serena_modify end*/
#define VOIPADVANCED "VoIPAdvanced"
#define VOIPBASIC "VoIPBasic"
#define VOIPCALLCTRL "VoIPCallCtrl"
#define VOIPMEDIA "VoIPMedia"
#define VOIPSPEED "VoIPSpeed"
#define VOIPCODECS "VoIPCodecs"
#define VOIPDIGITMAP "VoIPDigitMap"
#define VOIPENTRY "Entry"
/* jrchen add 20100222*/
#define VOIPLOG		"VoIPLog"
#define VOIPLOGSUB 	"Entry"
#define LOGENABLE 	"LogEnable"
#define LOGSERVERIP	"LogServerIP"
#define LOGSERVERPORT	"LogServerPort"
#define LOGDISPLAY	"LogDisplay"


#define SIPCONFPATH "/etc/SIPPhone.cfg"
#define MEDIAENTRY_NUM 5
#define MAXSPEED_NUM 10
#define RETRY_TIME 3
#define MAXCODEC_NUM 9

/*string define*/
#define REGISTRARADDR 		"RegistrarAddress"
#define REGISTRARPORT 		"RegistrarPort"
#define OUTBOUNDPROXYADDR 	"OutboundProxyAddress"
#define OUTBOUNDPROXYPORT 	"OutboundProxyPort"
#define SIPTRANSPORTTYPE 	"TransportType"
#define SIPTCPENABLED 		"TcpEnabled"
#define SIPTCPPORT 		"TcpPort"
#define SIPUDPPORT 		"UdpPort"
#define PRACKENABLE 		"PRACKEnable"
#define CALLERIDENABLE 		"CallerIdEnable"
// #define CALLERIDTYPE 		"CallerIdType"
#define CALLWAITINGENABLE 	"CallWaitingEnable"
#define CALLWAITINGREPLY 	"CallWaitingReply"
#define CALLFORWARDENABLE 	"CallForwardEnable"
#define CFUNUMBER		"CFUNumber"
#define CFBNUMBER		"CFBNumber"
#define CFNRNUMBER		"CFNRNumber"
#define CALLTRANSFER		"CallTransfer"
#define BLINDTRANSFERNUM	"BlindTransferNumber"
#define ATTENDEDTRANSFERNUM	"AttendedTransferNumber"
#define CALLHOLD		"CallHold"
#define CALLRETURN		"CallReturn"
/* 20110110 PTChen add for call return */
#define CALLRETURNNUMBER	"CallReturnNumber"
/* 20110110 PTChen add for DND */
#define CALLDNDENABLE		"CallDNDEnable"
#define CALLDNDENABLENUMBER 	"CallDNDEnableNumber"
#define CALLDNDDISABLENUMBER 	"CallDNDDisableNumber"
/* 20110321 Pork add for DOD */
#define CALLDODENABLE		"CallDODEnable"
#define CALLDODNUMBER 		"CallDODNumber"




#define	THREEWAYCONF		"ThreeWayConf"
#define	THREEWAYCONFNUMBER	"ThreeWayConfNumber"
#define	MWIENABLE		"MWIEnable"
#define SIP_YES			"Yes"
/* 2010/12/20 Enhance by Sam */
#define REGISTRATION_EXPIRE	"RegistrationExpire"
#define RETRANSMIT_INTERVAL_T1	"retransmissionT1"
#define RETRANSMIT_INTERVAL_T2	"retransmissionT2"

/* 20101220 PTchen add  */
/* 2011/1/12 Enhance by Sam */
#define	ANONY_CALLBLOCK	        "AnonyCallBlock"
#define	ANONY_CALL		"AnonyCall"
#define	VoIP_DND		"DND"
/* 20101220 PTchen add end */


#define DIGITMAPENABLE	"DigitMapEnable"
#define DIGITMAP	"DigitMap"
#define DIALTIMEOUT	"DialTimeout"
#define DIGITTIMEOUT	"DigitTimeout"


int voip_advanced_init(void);
int voip_advanced_boot(mxml_node_t *top);
int voip_advanced_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int voip_advanced_write(mxml_node_t *top, mxml_node_t *parant);
int voip_advanced_verify(mxml_node_t *node);
int voip_advanced_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);




int voip_basic_init(void);
int voip_basic_boot(mxml_node_t *top);
int voip_basic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int voip_basic_write(mxml_node_t *top, mxml_node_t *parant);
int voip_basic_verify(mxml_node_t *node);
int voip_basic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_call_ctrl_init(void);      
int voip_call_ctrl_write(mxml_node_t *top, mxml_node_t *parant);
int voip_call_ctrl_verify(mxml_node_t *node);
int voip_call_ctrl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_media_init(void);
int voip_media_write(mxml_node_t *top, mxml_node_t *parant);
int voip_media_verify(mxml_node_t *node);
int voip_media_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_speed_dial_init(void);
int voip_speed_dial_write(mxml_node_t *top, mxml_node_t *parant);
int voip_speed_dial_verify(mxml_node_t *node);
int voip_speed_dial_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_real_write(mxml_node_t *top);
int voip_real_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

void voip_put_str(FILE *fp, char *name, char *value);
int voip_get_payload_id(char *name);
void put_basic(FILE *fp,mxml_node_t *top);
/* PT Chen add 20101028 */
void put_advanced(FILE *fp,mxml_node_t *top);
void put_call_ctrl(FILE *fp,mxml_node_t *top);
void put_media(FILE *fp,mxml_node_t *top,char *local_addr,char *local_rtp_port);
void put_phone_number(FILE *fp,mxml_node_t *top);
void put_dial_plan(FILE *fp,mxml_node_t *top);
/* Enhance by sam 2010/12/20 */
void put_SIPStackParameters(FILE *fp,mxml_node_t *top);

int voip_codec_init(void);
/* jrchen add 20100222 */
int voip_log_init(void);
int voip_log_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_log_write(mxml_node_t *top, mxml_node_t *parant);
void put_log(FILE *fp,mxml_node_t *top);

int voip_digitmap_init(void);
int voip_digitmap_write(mxml_node_t *top, mxml_node_t *parant);
int voip_digitmap_verify(mxml_node_t *node);
int voip_digitmap_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
void put_digitmap(FILE *fp,mxml_node_t *top);
