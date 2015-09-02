

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

// MIMO Tx parameter, ShortGI, MCS, STBC, etc.  these are fields in TXWI. Don't change this definition!!!
typedef union  _MACHTTRANSMIT_SETTING {
	struct	{
	unsigned short   	MCS:7;                 // MCS
	unsigned short		BW:1;	//channel bandwidth 20MHz or 40 MHz
	unsigned short		ShortGI:1;
	unsigned short		STBC:2;	//SPACE 
	unsigned short		rsv:3;	 
	unsigned short		MODE:2;	// Use definition MODE_xxx.  
	}	field;
	unsigned short		word;
 } MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
#if defined(RT5392)||defined(MT7592)
		unsigned char       ApIdx;
#endif
     	unsigned char       Addr[MAC_ADDR_LENGTH];
     	unsigned char       Aid;
     	unsigned char       Psm;     //used for ssidindex
     	unsigned char		MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
   	signed char		AvgRssi0;
	signed char		AvgRssi1;
	signed char		AvgRssi2;
	unsigned int		ConnectedTime;
    MACHTTRANSMIT_SETTING	TxRate;
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;

typedef struct _RT_802_11_MAC_TABLE {
    unsigned long       Num;
    RT_802_11_MAC_ENTRY Entry[MAX_LEN_OF_MAC_TABLE];
} RT_802_11_MAC_TABLE, *PRT_802_11_MAC_TABLE;
#endif
