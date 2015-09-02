
#ifndef CWMP_EXTRA_H_
#define CWMP_EXTRA_H_
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed long int int32;
typedef unsigned long uint32;
typedef unsigned char boolean;
typedef unsigned long ip4a;
typedef unsigned char bool;
//typedef unsigned char Boolean;
typedef int tm_id_t;

#define CWMP_MAGIC 0xFFEEAABB
typedef struct
{
	uint8  vUpgradesManaged;	
	uint8  vRebootFlag;
	uint8  vBootTrapFlag;//for store the BootTrap flag.hqfeng,2006.06.14
	uint8  vGetFormBootbaseflag;//added by hqfeng,2006.09.14
	
	uint8 flagMAC2Name;//add by JQZhu 20080110	
	uint8 flagLoadDefaultRom;
	uint8  downloadType;        // 0: immediate-download; 1: delayed-download
	uint8  downloadComplete; //0: no download; 1: sucess; 2: 9001 error; 3: 9002 error; 4: 9010 error; 5: 9011 error; 6: 9012 error;
		
	uint32  startTime;
	uint32  endTime;

	uint32	vHostAddressTable[20];//for store the ACS IP	
	char  commandKey[4][33];//0:ScheduleInform, 1:Reboot, 2:download, 3:Upload. hqfeng,2007.01.09
	/***********************************End of Download & SSL*************************************/
	uint32  flag; //added for store some flags of cwmp, by hqfeng,2006.10.30	
	
	char  vParameterKey[34];	
	uint8  https;
	uint8  ca;  //0: TCCA, 1:UNHCA
//added by xyzhu_nj_20091211
	uint8 vWANLinkTypeFlag[64];
	uint8 downloadErr;
	uint8 downloadStart;
	uint8 uploadStart;
	uint8 uploadErr;

    	uint8 Reserved[52];		
	uint32  magic;  //This variable should be at the end of the structure.

}cwmpSaveFlag;

typedef struct spSysParam_s 
{	
	char				rootPassword[64];
	cwmpSaveFlag			*tcCwmpAttr;	/* For CWMP store parameters'*/
} spSysParam_t;

//add by brian
enum cwmp_attr_info
{
	acsUrl=0,
	CRUsername,
	CRPassword,
	CRPath,
	CRPort,
	CPUsername,
	CPPassword,
	CPPath,
	CPPort,
	CPHostName,
	CPSwh,
	PrdInterval,
	PrdEnable,
	PrvCode,
	ManufacturerName,
	ModelName,
	ProductClass,
	Description,
	CusHWVersion,
	ManufacturerOUI,
	prdInformTime,
	Persistent_Data,
	Warranty_Date,
};

#endif
