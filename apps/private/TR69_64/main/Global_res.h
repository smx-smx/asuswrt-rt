#ifndef GLOBAL_RES_H_
#define GLOBAL_RES_H_

#include <pthread.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <assert.h>
#include <sys/time.h>
#include <fcntl.h> //for message queue 
//#include <sys/types.h>
//#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <netinet/in.h>

//#include "../../../lib/libtcapi.h"
#include "cwmp_extra.h"
#include "messageHandle.h"
#include "tctimer.h"
#include "cwmp_dbg.h"
#include "mbuf.h"

#include "cwmpXmlmembuf.h"
#include "cwmpXml.h"
#include "cwmpXmlparser.h"

#include "cwmpRpc.h"
#include "cwmpSoap.h"
#include "cwmp.h"
#include "cwmpRpc.h"
#include "cwmpParamApi.h"
#include "cwmpNode.h"
//#include "cwmpParameter.h"
#include "cwmpParameter_table.h"

#ifdef TCSUPPORT_BACKUPROMFILE
#include "../../../public/mtd/tc_partition.h"
#endif
//extern void tcdbg_printf(char *fmt,...);
/******************************************
 * 
 *global var define
 *
 *******************************************/ 

/*use for semaphore*/
extern sem_t SEM_IMFORM;
#ifndef THREAD_REDUCE
extern sem_t SEM_VALCHANGE;
extern sem_t CWMP_PING;//for ip ping diagnostic
#else
extern uint8 ValueChangeFlag;
extern int message_handle();
extern void ThreadedTimerCheck(void);
#endif

/*use for tr069 rpc*/
extern /*volatile*/ cwmp_rpc_t       rpc;
extern spSysParam_t spSysParam;


/***************************
*
*global function define
*
***************************/
//void *instantiate_task();

void getdbgflag(void);
void resetdbgflag(void);
void tc_printf(char *fmt,...);
void SetCwmpPswModFlag(uint8);
void SignalInform(void);
int httpTaskStart(void);

/*for UPnP Port mapping*/
int PortMapInit();

extern void SaveCwmpFlags(void);
extern void InitCWMPFlags(void);
/*below is for session retry---add by brian*/
//extern void cwmpSetSessionRetryFlag(int value);
//extern void cwmpResetSessionRetryWaitInterval(void);
/*end*/
extern volatile void *theAllegroServerDataPtr;
extern pthread_mutex_t value_change_lock;

extern uint8 inform_task_start;
/*For TR064 State*/
#define kUpnpStateOff       0
#define kUpnpStateRunning   1
#define kUpnpStateShutdown  2

/*For CWMP Debug messsage in rompager*/
#define dbg_pline_1(buf) tc_printf(buf)
#define dbg_plineb_1(buf, byte) tc_printf(buf); tc_printf("%d", byte)
#define dbg_plinew_1(buf, word) tc_printf(buf); tc_printf("%d", word)
#define dbg_plinel_1(buf, long) tc_printf(buf); tc_printf("%d", long)

#define DBG_pline_1 dbg_pline_1
#define DBG_plinel_1 dbg_plinel_1

#define pause(x) usleep((x)*1000)


/*For CWMP Download UpLoad Reboot Factory Reset RPC Method*/
#define CLMP_REBOOT (1 << 0)
#define CLMP_RESET_DEF (1 << 1)

#define NO_OPERATION  	0
#define NO_HEADER	1
#define HTML_HEADER	2

extern int FirmWareUpdate(void);
extern int getCwmpAttrValue(int attrType,char *buf,uint32 size);
extern int setCwmpAttrValue(int attrType,char *buf);

#define UPLOAD_ROMFILE_PATH	"/var/tmp/up_romfile.cfg"
#define DOWNLOAD_CONFIG_FILE_NAME "ROM-0"
#define DOWNLOAD_CONFIG_FILE_PATH UPLOAD_ROMFILE_PATH

#define TCLINUX_PATH "/var/tmp/tclinux.bin"
#define DOWNLOAD_FIRMWARE_FILE_NAME "FIRMWARE"
#define DOWNLOAD_FIRMWARE_FILE_PATH TCLINUX_PATH


#define UPLOAD_CONFIG_FILE_NAME "/rom-0"
#define UPLOAD_CONFIG_FILE_PATH "/tmp/var/romfile.cfg"

#define UPLOAD_LOG_FILE_NAME "/device_log"
#if !defined(TCSUPPORT_CT) 
#ifdef TCSUPPORT_SYSLOG
#ifdef TCSUPPORT_SYSLOG_ENHANCE
#define DEFAULT_LOG_FILE 	 "/var/log/currLogFile"
#else
#define DEFAULT_LOG_FILE 	 "/tmp/var/log/messages"
#endif
#endif
#define UPLOAD_LOG_FILE_PATH "/tmp/log"
#endif

#define REBOOT_CMD	"/userfs/bin/mtd -r write %s romfile"
#define NOREBOOT_CMD	"/userfs/bin/mtd write %s romfile"
#if !defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT) 
#define DEF_ROMFILE_PATH 	"/userfs/romfile.cfg"
#endif

#define FAIL -1

/*Add for VendorConfigFile TR069 Nodes*/
#define VENDORCFGFILE_COMMON	"VendorCfgFile_Common"
#define VENDORCFGFILE_ENTRY	"VendorCfgFile_Entry%d"
#define CFG_FILE_ENTRY		0

#define LAN_ARP_INFO_PATH	"/tmp/lan_host_arp.info"
#define MAX_BUF_SIZE		256

/*MIB info for TR069 & TR064 memory allocate*/
extern void showMIB(void);
extern uint32 tctimer_free_mib;
extern uint32 tctimer_malloc_mib;
extern uint32 rompager_free_mib;
extern uint32 rompager_malloc_mib;
extern uint32 os_free_mib;
extern uint32 os_malloc_mib;



#endif
