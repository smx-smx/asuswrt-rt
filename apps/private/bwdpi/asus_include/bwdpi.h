#ifndef _BWDPI_H
#define _BWDPI_H

// command
#define CMD_WRED		"wred"
#define CMD_DATACOLLD	"dcd"
#define CMD_WRED_SET		"wred_set_conf"
#define CMD_RULE_AGENT	"tdts_rule_agent"
#define CMD_SHN_CTRL		"shn_ctrl"
#define CMD_GEN_LIC		"gen_lic"
#define CMD_LIC_CTRL		"lic_ctrl"
#define CMD_LIC_SETUP	"lic-setup.sh"

#define PATH_CMD_BWDPI	"/usr/bin/bwdpi/"
#define PATH_CMD_WRED	PATH_CMD_BWDPI	CMD_WRED
#define PATH_CMD_DATACOLLD	PATH_CMD_BWDPI	CMD_DATACOLLD
#define PATH_CMD_WRED_SET	PATH_CMD_BWDPI	CMD_WRED_SET
#define PATH_CMD_RULE_AGENT	PATH_CMD_BWDPI	CMD_RULE_AGENT
#define PATH_CMD_SHN_CTRL	PATH_CMD_BWDPI	CMD_SHN_CTRL

// config and folder path
#define PATH_TMP_BWDPI	"/tmp/bwdpi/"
#define PATH_DATABASE		PATH_TMP_BWDPI "rule.trf"
#define PATH_WRS_CONF	PATH_TMP_BWDPI "wred.conf"
#define PATH_APP_PATROL_CONF	PATH_TMP_BWDPI "app_patrol.conf"
#define PATH_DPI_CERT		PATH_TMP_BWDPI "ntdasus2014.cert"
#define PATH_CMD_LIC_CTRL	PATH_TMP_BWDPI	CMD_LIC_CTRL
#define PATH_CMD_LIC_SETUP	PATH_TMP_BWDPI	CMD_LIC_SETUP

// kernel module
#define PATH_TDTS	"/lib/modules/bwdpi/tdts.ko"
#define PATH_UDB		"/lib/modules/bwdpi/tdts_udb.ko"
#define PATH_UDBFW	"/lib/modules/bwdpi/tdts_udbfw.ko"

// module setting
#define PATH_TDTSFW_PARA	"/sys/module/tdts_udb/parameters/"
#define PATH_DEV_WAN		PATH_TDTSFW_PARA"dev_wan"
#define PATH_DEV_LAN		PATH_TDTSFW_PARA"dev_lan"
#define PATH_QOS_WAN		PATH_TDTSFW_PARA"qos_wan"
#define PATH_QOS_LAN		PATH_TDTSFW_PARA"qos_lan"
#define PATH_BW_DPI_SET	"/proc/bw_dpi_conf"

// TM rule DB
#define PATH_DEFAULT_RULE	PATH_CMD_BWDPI "rule.trf"
#define PATH_APPDB	PATH_TMP_BWDPI "bwdpi.app.db"
#define PATH_CATDB	PATH_TMP_BWDPI "bwdpi.cat.db"
#define PATH_RULEVER	PATH_TMP_BWDPI "rule.version"
#define PATH_SIG_VER	"/proc/nk_policy"
#define PATH_ENGINE_INFO	"/proc/ips_info"

#ifdef TCSUPPORT_ADD_JFFS
#define PATH_UPGRADE_RULE	"/jffs/signature/rule.trf"
#else
#ifdef TCSUPPORT_SQUASHFS_ADD_YAFFS
#define PATH_UPGRADE_RULE	"/yaffs/signature/rule.trf"
#endif
#endif


/*
	To turn on/off DPI features, echo with these HEX values
	APP_ID      : 0x001
	DEV_ID      : 0x002
	VIRT_PATCH  : 0x004
	WRS_APP     : 0x008
	WRS_CC      : 0x010
	WRS_SEC     : 0x020
	ANOMALY     : 0x040
	QOS         : 0x080
	APP_PATROL  : 0x100
	PATROL_TQ   : 0x200
*/
#define APP_ID     0
#define DEV_ID     1
#define VP_ID      2
#define WRS_APP    3
#define WRS_CC     4
#define WRS_SEC    5
#define ANOMALY    6
#define QOS_ID     7
#define APP_PATROL 8
#define TIME_QUOTA 9

/* DEBUG DEFINE */
#define BWDPI_DEBUG             "/tmp/BWDPI_DEBUG"
#define BWDPI_MON_DEBUG         "/tmp/BWMON_DEBUG"
#define BWDPI_MON_DELOG         "/tmp/BWMON_LOG"

/* DEBUG FUNCTION */
#define BWDPI_DBG(fmt,args...) \
	if(f_exists(BWDPI_DEBUG) > 0) { \
		tcdbg_printf("[BWDPI][%s:(%d)]"fmt, __FUNCTION__, __LINE__, ##args); \
	}

#define BWSQL_LOG(fmt,args...) \
	if(f_exists(BWDPI_SQLITE_DELOG) > 0) { \
		char info[200]; \
		snprintf(info, sizeof(info), "echo \"[BWDPI_SQLITE]"fmt"\" >> /tmp/BWSQL.log", ##args); \
		system(info); \
	}

#define BWMON_DBG(fmt,args...) \
	if(f_exists(BWDPI_MON_DEBUG) > 0) { \
		dbg("[BWMON][%s:(%d)]"fmt, __FUNCTION__, __LINE__, ##args); \
	}

#define BWMON_LOG(fmt,args...) \
	if(f_exists(BWDPI_MON_DELOG) > 0) { \
		char info[200]; \
		snprintf(info, sizeof(info), "echo \"[BWMON]"fmt"\" >> /tmp/BWMON.log", ##args); \
		system(info); \
	}

void start_dpi_engine_service();
void stop_dpi_engine_service();

#endif
