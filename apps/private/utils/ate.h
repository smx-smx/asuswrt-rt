


#if defined(TCSUPPORT_BB_NAND) || defined(TCSUPPORT_BOOTROM_LARGE_SIZE)
#define MTD_DATA_AREA_OFFSET 0x0001ffa0L
#define TCBOOT_MODEL_NAME_OFFSET 0x0001ff24L
#ifdef TCSUPPORT_ATE11 /* ATE Command v1.1 */
#define TCBOOT_ATE11_OFFSET						0x0001fad0L
#endif
#define TCBOOT_CALIBRATION_FIRST_OFFSET 		0x0001fb00L
#define TCBOOT_CALIBRATION_SECOND_OFFSET 		0x0001fd00L
#define TCBOOT_BOOTLOADER_VERSION_OFFSET		0x0001FF9DL
#else
#define MTD_DATA_AREA_OFFSET 0x0000ffa0L
#define TCBOOT_MODEL_NAME_OFFSET 0x0000ff24L
#ifdef TCSUPPORT_ATE11 /* ATE Command v1.1 */
#define TCBOOT_ATE11_OFFSET						0x0000fad0L
#endif
#define TCBOOT_CALIBRATION_FIRST_OFFSET 		0x0000fb00L
#define TCBOOT_CALIBRATION_SECOND_OFFSET 		0x0000fd00L
#define TCBOOT_BOOTLOADER_VERSION_OFFSET		0x0000FF9DL
#endif

#if defined(TCSUPPORT_ATE11) && defined(TCSUPPORT_WLAN_RT6856) /* ATE Command v1.1 in reservearea */
#define MTD_DATA_AREA_LEN 130 /* The total size of all parameters */
#else
#define MTD_DATA_AREA_LEN 92
#endif
#ifdef TCSUPPORT_ATE11 /* ATE Command v1.1 for GetATEParamFromBootloader */
#define TCBOOT_ATE11_LEN 48
#endif

#define ATE_2GCOUNTRY_LEN 2
#define ATE_5GCOUNTRY_LEN 2
#define ATE_PIN_LEN 8
#define ATE_2GMAC_LEN 17
#define ATE_5GMAC_LEN 17
#define ATE_TCODE_LEN 5
#define ATE_MODELNAME_LEN 24
#ifdef TCSUPPORT_ATE11 /* ATE Command v1.1 */
#define ATE_HWID_LEN 1
#define ATE_HWVERSION_LEN 3
#define ATE_DATECODE_LEN 8
#define ATE_HWBOM_LEN 19
#define ATE_SN_LEN 12
#endif

#if defined(TCSUPPORT_DUAL_WLAN)
#define ATE_2GCOUNTRY_OFFSET 0
#define ATE_5GCOUNTRY_OFFSET (ATE_2GCOUNTRY_OFFSET + ATE_2GCOUNTRY_LEN + 1)
#define ATE_PIN_OFFSET (ATE_5GCOUNTRY_OFFSET + ATE_5GCOUNTRY_LEN + 1)
#define ATE_2GMAC_OFFSET (ATE_PIN_OFFSET + ATE_PIN_LEN + 1)
#define ATE_5GMAC_OFFSET (ATE_2GMAC_OFFSET + ATE_2GMAC_LEN + 1)
#define ATE_TCODE_OFFSET (ATE_5GMAC_OFFSET + ATE_5GMAC_LEN + 1)
#define ATE_MODELNAME_OFFSET (ATE_TCODE_OFFSET + ATE_TCODE_LEN + 1)
#else
#define ATE_2GCOUNTRY_OFFSET 0
#define ATE_PIN_OFFSET (ATE_2GCOUNTRY_OFFSET + ATE_2GCOUNTRY_LEN + 1)
#define ATE_2GMAC_OFFSET (ATE_PIN_OFFSET + ATE_PIN_LEN + 1)
#define ATE_TCODE_OFFSET (ATE_2GMAC_OFFSET + ATE_2GMAC_LEN + 1)
#define ATE_MODELNAME_OFFSET (ATE_TCODE_OFFSET + ATE_TCODE_LEN + 1)
#endif
#ifdef TCSUPPORT_ATE11 /* ATE Command v1.1 */
#if defined(TCSUPPORT_WLAN_RT6856) /* in reservearea */
#define ATE_HWID_OFFSET (ATE_MODELNAME_OFFSET + ATE_MODELNAME_LEN + 1)
#else
#define ATE_HWID_OFFSET 0
#endif
#define ATE_HWVERSION_OFFSET (ATE_HWID_OFFSET + ATE_HWID_LEN + 1)
#define ATE_DATECODE_OFFSET (ATE_HWVERSION_OFFSET + ATE_HWVERSION_LEN + 1)
#define ATE_HWBOM_OFFSET (ATE_DATECODE_OFFSET + ATE_DATECODE_LEN + 1)
#define ATE_SN_OFFSET (ATE_HWBOM_OFFSET + ATE_HWBOM_LEN + 1)
#endif

#define ATE_FLASH_QUIETREAD_CMD		"/userfs/bin/mtd -q -q readflash %s %lu %lu %s"
#define ATE_FLASH_QUIETWRITE_CMD	"/userfs/bin/mtd -q -q writeflash %s %lu %lu %s"

typedef enum ate_param_id{
	ATE_PARAM_MAC2G=1,
	ATE_PARAM_MAC5G,
	ATE_PARAM_COUNTRY2G,
	ATE_PARAM_COUNTRY5G,
	ATE_PARAM_PINCODE,
	ATE_PARAM_TERRITORY,
	ATE_PARAM_MODELNAME,
#ifdef TCSUPPORT_ATE11 /* ATE Command v1.1 */
	ATE_PARAM_HWID,
	ATE_PARAM_HWVERSION,
	ATE_PARAM_DATECODE,
	ATE_PARAM_HWBOM,
	ATE_PARAM_SN,
#endif
	ATE_PARAM_MAX
} ate_param_id_t;

typedef struct ate_param{
	char code0[3];
	char code1[3]; 
	char pincode[9];
	char mac0[18]; 
	char mac1[18];
	char territorycode[6];
	char modelname[25];
#ifdef TCSUPPORT_ATE11 /* ATE Command v1.1 */
	char hwid[2];
	char hwversion[4];
	char datecode[9];
	char hwbom[20];
	char sn[13];
#endif
} ate_param_t;

void GetDataAreaFromBootloader(char *valbuf, int vallen);
void GetATEParamFromBootloader(ate_param_t *param);
int WriteATEParamToBootLoader(ate_param_id_t id, char *value);

#ifdef TCSUPPORT_MTD_ENCHANCEMENT

/* Note : Don't call this API in cfg_manager */
int WriteATEParamToFlash(ate_param_id_t id, char *value);
void GetDataFromFlash(char *valbuf, int vallen);
void GetATEParamFromFlash(ate_param_t *param);
int WritePLDataToFlash(int mode);
void GetPLDataFromFlash(char *str);
#endif


