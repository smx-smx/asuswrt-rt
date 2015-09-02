
#ifndef TC_PORTBIND_H_
#define TC_PORTBIND_H_

#if 0
#define DPRINTK(x, args...) printk("PORT-BIND: " x, ##args)
#else
#define DPRINTK(x, args...)
#endif

#define PORTBIND_IOC_MAGIC 'g'
#define PORTBIND_MAJOR 200
#define PORTBIND_IOC_ENABLE _IOW(PORTBIND_IOC_MAGIC,0,unsigned long)
#define PORTBIND_IOC_DISABLE _IOR(PORTBIND_IOC_MAGIC,1,unsigned long)
#define PORTBIND_IOC_ADD_INTERFACE _IOR(PORTBIND_IOC_MAGIC,2,unsigned long)
#define PORTBIND_IOC_DEL_INTERFACE _IOR(PORTBIND_IOC_MAGIC,3,unsigned long)
#define PORTBIND_IOC_ADD_GROUP _IOR(PORTBIND_IOC_MAGIC,4,unsigned long)
#define PORTBIND_IOC_DEL_GROUP _IOR(PORTBIND_IOC_MAGIC,5,unsigned long)
#define PORTBIND_IOC_GET_FREE_GROUPID _IOR(PORTBIND_IOC_MAGIC,6,unsigned long)
#define PORTBIND_IOC_SHOW_GROUP _IOR(PORTBIND_IOC_MAGIC,7,unsigned long)
#define PORTBIND_IOC_MAX_NR 20

//#define		PORTBIND_DEUBG
#ifdef RELEASE_TEST
#define VERSION_TYPE " #"
#else
#ifdef RELEASE_VERIFICATION
#define VERSION_TYPE " $"
#else
#define VERSION_TYPE " "
#endif
#endif

#define MODULE_NAME "tcportbind"
#define VERSION_NUM " V1.0.0"
#define TRUNK_TIME ""
#define BUILD_NUM __DATE__"-"__TIME__
#define MODULE_VERSION MODULE_NAME VERSION_NUM VERSION_TYPE TRUNK_TIME "("BUILD_NUM")"


#define     IFNAMSIZ					16
#ifdef TCSUPPORT_E8B
#define 	MAX_GROUP_NUM			64
#else
#define 	MAX_GROUP_NUM			16
#endif
#define 	MAX_IFNAME_LST_NUM		128
#define		MAX_IFTYPE_LST_NUM		64

#define 	TMP_IF_PATH	"/tmp/lanif.conf"

struct ifPolicyRoute
{
	char *ifName;
	char *mark;
	char *tabid;
};

/* for receive ioctl info from user space */
struct portbindcmdif
{
	int groupid;
	char ifNameLst[MAX_IFNAME_LST_NUM];
	char ifTypeLst[MAX_IFTYPE_LST_NUM];
	int ifNum;
};

/* for a interface that is bind to a group */
struct groupif
{
	char ifName[IFNAMSIZ];
	char ifType[2];
};

/* a serial of interface that in one group */
struct groupLst
{
   struct groupif *pIf;
   int 	groupid;
   int ifNum;
};

#endif

