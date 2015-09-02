/*
 * TDI.h
 *
 *  Created on: Nov 10, 2010
 *      Author: pork
 */

/*
 * Interface types
 */
#ifndef _TDI_H_
#define _TDI_H_

//#include "osal_types.h"
#define TIC_INFCTYPE_FXS              (0x0000d200)
#define TIC_INFCTYPE_FXO              (0x0000d201)
#define TIC_INFCTYPE_AUDIO            (0x0000d202)

/*20110714 PTChen modified. define from makefile*/
/*pork commented: number of slic device*/
#define TIC_DEVICE_NUM 	        (VTSP_USER_FXS_NUM)
#define TIC_INFC_FXS_NUM        (VTSP_USER_FXS_NUM)
#define TIC_INFC_FXO_NUM      	(VTSP_USER_FXO_NUM)

/*
 * Number of interfaces
 */
#define TIC_INFC_CHAN_NUM            (2)
#define TIC_INFC_AUDIO_NUM            (0)
#define TIC_INFC_NUM  (TIC_INFC_FXO_NUM + TIC_INFC_FXS_NUM + TIC_INFC_AUDIO_NUM)

#define TIC_CONTROL_ARG_ENABLE        (1) /* "on" */
#define TIC_CONTROL_ARG_DISABLE       (0) /* "off" */

#define TIC_PULSE_DIALSTR_SZ          (35)

typedef enum {
    TIC_CONTROL_POWER_OPEN            = 1,
    TIC_CONTROL_POWER_TEST            = 2,
    TIC_CONTROL_POWER_ACTIVE          = 3,
    TIC_CONTROL_POWER_DOWN            = 4,
    TIC_CONTROL_POLARITY_FWD          = 5,
    TIC_CONTROL_POLARITY_REV          = 6,
    TIC_CONTROL_ONHOOK                = 7,
    TIC_CONTROL_OFFHOOK               = 8,
    TIC_CONTROL_HOOK_FLASH            = 9,
    TIC_CONTROL_STOP_RING             = 10,
    TIC_CONTROL_FLASH_TIME_MIN        = 11,
    TIC_CONTROL_FLASH_TIME_MAX        = 12,
    TIC_CONTROL_RELEASE_TIME_MIN      = 13,
    /* TIC_CONTROL_RELAY_FXS_TO_CODEC: "enable" = codec, "disable" = pstn */
    TIC_CONTROL_RELAY_FXS_TO_PSTN     = 14,
    TIC_CONTROL_LED                   = 15, /* ARG_ENABLE or ARG_DISABLE */
    TIC_CONTROL_GAIN_TX               = 16, /* units in 0.5 dB XXX? */
    TIC_CONTROL_GAIN_RX               = 17, /* units in 0.5 dB XXX? */
    TIC_CONTROL_MUTE                  = 18, /* ARG_ENABLE or ARG_DISABLE */
    TIC_CONTROL_LOUDSPEAKER           = 19, /* ARG_ENABLE or ARG_DISABLE */
    TIC_CONTROL_HANDSET               = 20, /* ARG_ENABLE or ARG_DISABLE */
    TIC_CONTROL_LOOPBACK_DIGITAL      = 21, /* ARG_ENABLE or ARG_DISABLE */
    TIC_CONTROL_LOOPBACK_ANALOG       = 22, /* ARG_ENABLE or ARG_DISABLE */
    TIC_CONTROL_LINE_STATE            = 23,
    TIC_CONTROL_PULSE_GENERATE        = 24,
    TIC_CONTROL_PDD_MAKE_MIN          = 25, /* PDD Make Minimum */
    TIC_CONTROL_PDD_MAKE_MAX          = 26, /* PDD Make Maximum */
    TIC_CONTROL_PDD_BREAK_MIN         = 27, /* PDD Break Minimum */
    TIC_CONTROL_PDD_BREAK_MAX         = 28, /* PDD Break Maximum */
    TIC_CONTROL_PDD_INTERDIGIT_MIN    = 29, /* PDD Digit Minimum */
    TIC_CONTROL_AUDIO_ATTACH          = 30,
    TIC_CONTROL_GR909_DIAG            = 31,
    TIC_CONTROL_BALANCED_RINGING      = 32,
    TIC_CONTROL_GENERIC               = 33,
    TIC_CONTROL_UNKNOWN_CODE          = 34  /* Last Entry*/
} TIC_ControlCode;

typedef enum {
    TIC_GR909_HEMF                    = 0,
    TIC_GR909_FEMF                    = 1,
    TIC_GR909_RFAULT                  = 2,
    TIC_GR909_ROFFHOOK                = 3,
    TIC_GR909_REN                     = 4,
    TIC_GR909_ALL                     = 5,
    TIC_GR909_ALLMASK                 = 0x1F
} TIC_Gr909Code;


typedef enum {
    TIC_GET_STATUS_HOOK                 = 1,
    TIC_GET_STATUS_HOOK_IMMEDIATE       = 2,
    TIC_GET_STATUS_HOOK_FLASH           = 3,
    TIC_GET_STATUS_LINE_STATE           = 4,
    TIC_GET_STATUS_POLARITY             = 5,
    TIC_GET_STATUS_RING_NUM             = 6,
    TIC_GET_STATUS_PULSE_DIGIT          = 7,
    TIC_GET_STATUS_PARALLEL_PHONE       = 8,
    TIC_GET_STATUS_PARALLEL_PHONE_START = 9,
    TIC_GET_STATUS_PARALLEL_PHONE_END   = 10,
    TIC_GET_STATUS_GAIN_TX              = 11, /* units in 0.5 dB XXX? */
    TIC_GET_STATUS_GAIN_RX              = 12, /* units in 0.5 dB XXX? */
    TIC_GET_STATUS_MUTE                 = 13, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_LOUDSPEAKER          = 14, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_HANDSET_ID           = 15, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_LOOPBACK_DIGITAL     = 16, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_LOOPBACK_ANALOG      = 17, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_LED                  = 18, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_RELAY_FXS_TO_PSTN    = 19, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_FLASH_TIME_MIN       = 20, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_FLASH_TIME_MAX       = 21, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_RELEASE_TIME_MIN     = 22, /* TIC_ENABLED or TIC_DISABLED */
    TIC_GET_STATUS_POLARITY_REVERSED    = 23,
    TIC_GET_STATUS_DISCONNECT           = 24,
    TIC_GET_STATUS_RING_EDGE            = 25,
    TIC_GET_STATUS_KEYPAD_EVENT         = 26, /* DECT key digit */
    TIC_GET_STATUS_KEYPAD_VALUE         = 27, /* DECT key digit */
    TIC_GET_STATUS_BYTE_EVENT           = 28,  /* Last Entry */
    TIC_GET_STATUS_PULSE_GENERATE_EVENT = 29, /* for pulse digit generation on fxo */
    TIC_GET_STATUS_NSAMPLES_10MS        = 30,
    TIC_GET_STATUS_GR909_HEMF           = 31,
    TIC_GET_STATUS_GR909_FEMF           = 32,
    TIC_GET_STATUS_GR909_RFAULT         = 33,
    TIC_GET_STATUS_GR909_ROFFHOOK       = 34,
    TIC_GET_STATUS_GR909_REN            = 35,
    TIC_GET_STATUS_NUM_INFC_FXS         = 36,
    TIC_GET_STATUS_NUM_INFC_FXO         = 37,
    TIC_GET_STATUS_NUM_INFC_AUDIO       = 38,
    TIC_GET_STATUS_UNKNOWN_CODE         = 39  /* Last Entry */
} TIC_GetStatusCode;

/*
 * TIC status codes
 */
typedef enum {
    /* TIC status error, or not supported */
    TIC_STATUS_ERROR              = -1,

    /*
     * TIC Physical Interface states
     */
    TIC_STATE_POWER_DOWN        = 10,
    TIC_STATE_RINGING           = 11,
    TIC_STATE_RING_PAUSE        = 12,
    TIC_STATE_ACTIVE            = 13,
    TIC_STATE_ACTIVE_MONITOR    = 14,
    TIC_STATE_GROUND_START      = 15,
    TIC_STATE_LOOP_START        = 16,
    /*
     * Immediate hook status flags
     */
    TIC_IMMEDIATE_ONHOOK          = 30,
    TIC_IMMEDIATE_OFFHOOK         = 31,
    /*
     * On hook monitor modes
     */
    TIC_NOMONITOR                 = 40,
    TIC_MONITOR                   = 41,
    /*
     * Seize/Release flags, if not in seize or not in release then in inflash.
     */
    TIC_RELEASE                   = 50,
    TIC_SEIZE                     = 51,
    TIC_UNKNOWN_HOOK              = 52,
    /*
     * Flash detection flags
     */
    TIC_NO_FLASH                  = 63,
    TIC_FLASH                     = 64,
    /*
     * * Ring cadences.
     */
    TIC_RING_MAKE                 = 70,
    TIC_RING_BREAK                = 71,
    /*
     * If parallel phone is offhook.
     */
    TIC_PARALLEL_NONE             = 80,
    TIC_PARALLEL_INPROGRESS       = 81,
    TIC_PARALLEL_OFFHOOK          = 82,
    /*
     * Define TIC battery ploarity flags:
     */
    TIC_BATT_POL_FORWARD          = 90,
    TIC_BATT_POL_REVERSE          = 91,
    TIC_BATT_POL_REVERSED         = 92,
    TIC_BATT_POL_NOT_REVERSED     = 93,
    /*
     * Genric TIC Status
     */
    TIC_ENABLED                   = 100,
    TIC_DISABLED                  = 101,
    /*
     * Disconnect Status
     */
    TIC_DISCONNECT_DISC           = 110,
    TIC_DISCONNECT_RECON          = 111,
    /*
     * Keypad Event Status
     */
    TIC_KEYPAD_KEY_FLAG           = 120,
    TIC_KEYPAD_KEY_LEADING        = 121,
    TIC_KEYPAD_KEY_TRAILING       = 122,
    /*
     * Byte Event Status
     */
    TIC_BYTE_EVENT_FALSE           = 130,
    TIC_BYTE_EVENT_TRUE            = 131,
    /*
     * Pulse Generation Status
     */
    TIC_PULSE_GENERATE_ACTIVE      = 140,
    TIC_PULSE_GENERATE_COMPLETE    = 141,
    /*
     * nsamples per 10ms rate
     */
    TIC_NSAMPLES_10MS_8K           = 150,
    TIC_NSAMPLES_10MS_16K          = 151,
    TIC_NSAMPLES_10MS_32K          = 152,
    TIC_NSAMPLES_10MS_48K          = 153,
    /*
     * Used for easy math in VTSPR
     */
    TIC_NUM_INFC_0                 = 159,
    TIC_NUM_INFC_1                 = 160,
    TIC_NUM_INFC_2                 = 161,
    TIC_NUM_INFC_3                 = 162,
    TIC_NUM_INFC_4                 = 163,
    TIC_NUM_INFC_5                 = 164,
    TIC_NUM_INFC_6                 = 165,
    TIC_NUM_INFC_7                 = 166,
    TIC_NUM_INFC_8                 = 167,
    TIC_NUM_INFC_9                 = 168,
    TIC_NUM_INFC_10                = 169,
    TIC_NUM_INFC_11                = 170,
    TIC_NUM_INFC_12                = 171,
    TIC_NUM_INFC_13                = 172,
    TIC_NUM_INFC_14                = 173,
    TIC_NUM_INFC_15                = 174,
    TIC_NUM_INFC_16                = 175,
    TIC_NUM_INFC_17                = 176,
    TIC_NUM_INFC_18                = 177,
    TIC_NUM_INFC_19                = 178,
    TIC_NUM_INFC_20                = 179

} TIC_Status;

/*
 * TIC return codes
 */
typedef enum {
    TIC_ERROR_NOMEM               = -5,
    TIC_ERROR_OVERFLOW            = -4,
    TIC_ERROR_INIT                = -3,
    TIC_ERROR_NOT_SUPPORTED       = -2,
    TIC_ERROR_UNKNOWN_INTERFACE   = -1,
    /* TIC Control Success code */
    TIC_OK                        =  0,
} TIC_Return;

#ifdef VTSP_ENABLE_PULSE_DETECT
typedef enum {
    TIC_PDD_READY_BIT             = 7,
    TIC_PDD_EARLY_BIT             = 6,
    TIC_PDD_DIGIT_MASK            = 0x0f
} TIC_PDD_STATUS;
#endif

/*
 * Ring objct.
 */
typedef struct {
    int state;
    int count;
    int numCads;
    int make1s;
    int make2s;
    int make3s;
    int break1s;
    int break2s;
    int break3s;
    int rep;
    int numRings;
} TIC_RingObj;

typedef struct {
    int numCads; /* Number of ring cadences */
    int make1;   /* Make time of 1st cadence, -1 for infinite */
    int break1;  /* Break time of 1st cadence, -1 for infinite */
    int make2;   /* Make time of 2nd cadence */
    int break2;  /* Break time of 2nd cadence */
    int make3;   /* Make time of 3rd cadence */
    int break3;  /* Break time of 3rd cadence */
    int repeat;  /* Repeats, -1 for infinite */
} TIC_RingParams;
/*
 * TIC FXS Object
 */
typedef struct {
    unsigned char       state;
    int         intrs;
    /*
     * Seize/Release/Hook-flash detection and battery polarity
     */
    int         hookFlag;
    int         onhookTimer;
    int         offhookTimer;
    int         flashFlag;
    int         sezRelFlag;
    int         polarity;
    int         pulseStatus;
     /*
     * Flash_hook/Release parameter
     * These are adjustable
     */
    int         releaseTimeMin;
    int         flashTimeMin;
    int         flashTimeMax;
    /*
     * Commands execution status
     */
    int         stateCmd;
    int         polarityCmd;
    unsigned int      gr909Mask;
    unsigned int      gr909Status;
    /*
     * Ringing table parameters
     */
    TIC_RingObj ringObj;
#ifdef VTSP_ENABLE_PULSE_DETECT
    TIC_PddObj  pddObj;
#endif
} TIC_FxsObj;
/*
 * TIC FXO Object
 */
typedef struct {
    unsigned char state;
    int   intrs;
    /*
     * Seize/Release/Hook-flash detection and battery polarity
     */
    int   onhookTimer;
    int   offhookTimer;
    int   hookFlag;
    int   sezRelFlag;
    int   parallelFlag;
    int   polarity;
    int   polarityRev;
     /*
     * Flash_hook/Release parameter
     * These are adjustable
     */
    int   releaseTimeMin;
    int   flashTimeMin;
    int   flashTimeMax;
    /*
     * Commands execution status
     */
    int   flashCmd;
    int   hookStateCmd;
    /*
     * Misc
     */
    int   flashCount;
    int   ringCount;
    int   ringNum;
    int   ringBreak;
    int   zivCount;
    int   pvCount;
    int   nvCount;
    int   count;
    unsigned char pulse[35];
    unsigned char pulseDTMFMode;
    int   pauseTime;

    /*
     * Disconnect Status
     */
    int   disconnectStatus;

#ifdef VTSP_ENABLE_PULSE_GENERATE
    int  pulseStatus;
    int  pulseEvent;
#endif
    int  flashEvent;
} TIC_FxoObj;
/*
 * TIC Audio / DECT Object
 */
typedef struct {
    int   futureSupport;
} TIC_AudioObj;

/*
 * TIC Object
 */
typedef struct {
    unsigned char chan;
    int   infcType;
    TIC_Status       nSamples10ms;
    union {
        TIC_FxsObj   fxs;
        TIC_FxoObj   fxo;
        TIC_AudioObj audio;
    };
} TIC_Obj;

#endif

