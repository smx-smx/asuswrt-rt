

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#define MAX_LEN_OF_MAC_TABLE 32
#define MAC_ADDR_LENGTH                 6

#define INIC_INTERFACE "ra"
//#define INIC_MAINIFNAME "ra0"	//javi
#define INIC_MAINIFNAME "ra00_0"
#define WLAN_SYS_LEN                    256
#define WLAN_8_LEN                     8
#define WLAN_16_LEN                    16
#define WLAN_32_LEN                    32
#define WLAN_64_LEN                    64


#define SUCCESS 0
#define FAIL -1

#define TCWlancmdVer  "wlancmd_v1.0\n"
#define TCWLANDriverVer "driver_v1.0\n"
//#define WLANDATPATH "/var/Wireless/RT2860AP.dat"
#define WLANDATPATH "/etc/Wireless/RT2860AP/RT2860AP.dat"

typedef struct
{
        char*           cWirelessMode;
        int             iWirelessModeNumber;
}TcWlanMode;

typedef struct
{
        char*           cCountryString;
        char            iRegionChannel;
}TcWlanCountry;

typedef struct
{
        char*           cChannelRate;
        int             iRateCount;
}TcWlanChannelMode;

typedef signed char CHAR;
typedef signed short SHORT;
typedef signed int INT;
typedef signed long LONG;

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT32;
typedef unsigned long ULONG;

#define UCOS
#define PACKED
#define UINT32 	unsigned int
#define USHORT 	unsigned short
#define UCHAR	unsigned char
#define CHAR 	char
#define ULONG 	unsigned long

#define MAX_NUMBER_OF_MAC	64

// MIMO Tx parameter, ShortGI, MCS, STBC, etc.  these are fields in TXWI. Don't change this definition!!!
//Remember to sync with WiFi_driver/include/oid.h if WiFi driver is updated.
//Andy Chiu, 2015/06/30. collect these.
#if defined(TCSUPPORT_WLAN_RT6856)
typedef union  _MACHTTRANSMIT_SETTING {
	struct  {
	unsigned short	BW:1;	//channel bandwidth 20MHz or 40 MHz
	unsigned short	MCS:7;	// MCS
	unsigned short	MODE:2;	// Use definition MODE_xxx.
	unsigned short	rsv:3;
	unsigned short	STBC:2;	//SPACE
	unsigned short	ShortGI:1;
	} field;
	unsigned short	word;
 } MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;	/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;	/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	MACHTTRANSMIT_SETTING MaxRate;
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;
#elif defined(MT7592)
typedef union _MACHTTRANSMIT_SETTING {
	//Andy Chiu, 2015/08/25. update date structure.
	struct {
		USHORT MODE:3;	/* Use definition MODE_xxx. */
		USHORT iTxBF:1;
		USHORT eTxBF:1;
		USHORT STBC:1;	/* only support in HT/VHT mode with MCS0~7 */
		USHORT ShortGI:1;
		USHORT BW:2;	/* channel bandwidth 20MHz/40/80 MHz */
		USHORT ldpc:1;
		USHORT MCS:6;	/* MCS */
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	UINT32 LastRxRate;
/*sync with MT7603E_DPA_LinuxAP_4.0.1.0_rev2.P1_20160707*/
//#ifdef RTMP_RBUS_SUPPORT
	SHORT StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	SHORT SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT reserved;*/
//#endif /* RTMP_RBUS_SUPPORT */
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;
#elif defined(RT5392) || defined(RT3090) || defined(TCSUPPORT_DUAL_WLAN_RT5592_RT3593)
typedef union _MACHTTRANSMIT_SETTING {
	struct {
		USHORT	MODE:2;	// Use definition MODE_xxx.
		USHORT	rsv:3;
		USHORT	STBC:2;	//SPACE
		USHORT	ShortGI:1;
		USHORT	BW:1;	//channel bandwidth 20MHz or 40 MHz
		USHORT	MCS:7;	// MCS
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
//#ifdef RTMP_RBUS_SUPPORT
	UINT32		LastRxRate;
	SHORT		StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	SHORT		SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT		TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT		reserved;*/
//#endif /* RTMP_RBUS_SUPPORT */
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;
#endif

typedef struct _RT_802_11_MAC_TABLE {
	ULONG Num;
	RT_802_11_MAC_ENTRY Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_MAC_TABLE, *PRT_802_11_MAC_TABLE;

//Andy Chiu, 2015/06/30. Add 5G mac table
#if defined(MT7612E) || defined(MT7610E)
typedef union _MACHTTRANSMIT_SETTING_5G{
	//Andy Chiu, 2015/08/25. update date structure.
	struct {
		USHORT MODE:3;	/* Use definition MODE_xxx. */
		USHORT iTxBF:1;
		USHORT eTxBF:1;
		USHORT STBC:1;	/* only support in HT/VHT mode with MCS0~7 */
		USHORT ShortGI:1;
		USHORT BW:2;	/* channel bandwidth 20MHz/40/80 MHz */
		USHORT ldpc:1;
		USHORT MCS:6;	/* MCS */
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING_5G, *PMACHTTRANSMIT_SETTING_5G;

typedef struct _RT_802_11_MAC_ENTRY_5G {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	UINT32 LastRxRate;

//sync with MT7620
	int StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	int SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT reserved;*/
} RT_802_11_MAC_ENTRY_5G, *PRT_802_11_MAC_ENTRY_5G;

typedef struct _RT_802_11_MAC_TABLE_5G {
	ULONG Num;
	RT_802_11_MAC_ENTRY_5G Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_MAC_TABLE_5G, *PRT_802_11_MAC_TABLE_5G;
#elif defined(RT5592)
typedef union _MACHTTRANSMIT_SETTING_5G{
	struct {
		USHORT	MODE:2;	// Use definition MODE_xxx.
		USHORT	rsv:3;
		USHORT	STBC:2;	//SPACE
		USHORT	ShortGI:1;
		USHORT	BW:1;	//channel bandwidth 20MHz or 40 MHz
		USHORT	MCS:7;	// MCS
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING_5G, *PMACHTTRANSMIT_SETTING_5G;

typedef struct _RT_802_11_MAC_ENTRY_5G {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	UINT32 LastRxRate;

//sync with MT7620
	SHORT StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	SHORT SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT reserved;*/
} RT_802_11_MAC_ENTRY_5G, *PRT_802_11_MAC_ENTRY_5G;

typedef struct _RT_802_11_MAC_TABLE_5G {
	ULONG Num;
	RT_802_11_MAC_ENTRY_5G Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_MAC_TABLE_5G, *PRT_802_11_MAC_TABLE_5G;
#endif

#endif
