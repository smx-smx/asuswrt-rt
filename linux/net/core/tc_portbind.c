#include <net/sock.h>
#include "tc_portbind.h"

MODULE_DESCRIPTION("portbind");
MODULE_LICENSE("GPL");
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(char *inIf, char *outIf);

static struct groupLst gpLst[MAX_GROUP_NUM];
static int g_portbind_enable = 0;

int portbind_open(struct inode *inode, struct file *filp);

#if 0
int
portbind_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, void __user *arg);
#else
int
portbind_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg);

#endif

int portbind_open(struct inode *inode, struct file *filp)
{
	DPRINTK("portbind_open entered\n");
	return 0;
}


/* return port bind switch info */
int getPortBindEnable(void) 
{
	return g_portbind_enable;
}

/*____________________________________________________________
** Description:
** 	   check if in interface and out interface are in the same 
** 	   grooup.
**	   1. vlan device and smux interface will only check one 
**		  time, it's controled by MASK_OUT_DEV bit in 
**		  skb->portbind_mark.
**	   2. There is special handle for e8b requirement.
** Returns:
**	   1: success
**	   0: fail
**____________________________________________________________
*/

int checkGroup(char *inIf, char *outIf)
{
	char inIfName[IFNAMSIZ], outIfName[IFNAMSIZ];
	int i, j, inId = -1, outId = -1;
#ifdef TCSUPPORT_E8B
	char inType[4], outType[4];
#endif
	struct groupif *pIf = NULL;


	if (inIf == NULL || outIf == NULL) {
		return 1;
	}

	strcpy(inIfName, inIf);
	strcpy(outIfName, outIf);

#ifdef TCSUPPORT_E8B
	memset(inType, 0, sizeof(inType));
	memset(outType, 0, sizeof(outType));
#endif
	
	/* find group id of in and out interface in group */
	for (i = 0; i < MAX_GROUP_NUM; i++) {
		pIf = gpLst[i].pIf;
		if (pIf == NULL) {
			continue;
		}
		else {
			for (j = 0; j < gpLst[i].ifNum; j++) {
				if (pIf == NULL) {
					break;
				}
				if (!strcmp(inIfName, pIf->ifName)) {
					inId = gpLst[i].groupid;
				#ifdef TCSUPPORT_E8B
					strcpy(inType, pIf->ifType);
				#endif
				}
				else if (!strcmp(outIfName, pIf->ifName)) {
					outId = gpLst[i].groupid;
				#ifdef TCSUPPORT_E8B
					strcpy(outType, pIf->ifType);
				#endif
				}
				pIf++;
			}
		}
	}

	/* in device and out device are in the same group */
	if (inId == outId) {
		return 1;
	}

#ifdef TCSUPPORT_E8B
	/*_______________________________________________________
	**
	**  If in dev is lan device and not in some group, 
	**  and out dev is INTERNET type wan device, let it go.
	**  There are two kinds of interface, INTERNET type for 
	**  wan interface and OTHER type for all other interface.
	**	lan -> wan, do you konwn why inIfname is empty ???, 
	** 	strlen(inIfName) == 0 is essential for packet from cpe.
	**_______________________________________________________
	*/

	/* packet from cpe, arp etc. */
	if (strlen(inIfName) == 0) {
		/* packet from cpe */
		return 1;
	}
#if 0
	/* lan -> imq, lan is in some group */
	if ((inId != -1) && (inIfName[0] == 'e' || inIfName[0] == 'r' || inIfName[0] == 'u') && (outIfName[0] == 'i')) {
		return 1;
	}
#endif

	/* lan -> wan, 'br0' and 'imq0' seems unnessary. */
	if ((inId == -1) && (inIfName[0] == 'e' || inIfName[0] == 'r' || inIfName[0] == 'u') 
					 && ((outIfName[0] == 'b' )		/* br0 interface */
						 || (outIfName[0] == 'i')	/* imq0 interface */
						 || (outIfName[0] == 'n' && !strcmp(outType, "I")))) {
		return 1;
	}

	/*____________________________________________________________
	**
	** wan -> lan data stream for bridge smux interface problem.
	** If there are more than one smux bridge interface, only the
	** the first smux interface will receive the packet.
	** wan -> wan or wan -> lan
	**____________________________________________________________
	*/
	if (inIfName[0] == 'n') {
		return 1;
	}
#else
	/* if the interface is not in group, including packet from cpe. */
	if (inId == -1 || outId == -1) {
		return 1;
	}
#endif

	/* check failed */
	return 0;
}

static int portbind_addgroup(struct portbindcmdif *pbif)
{
	struct groupif *pIf = NULL, *pIftmp = NULL;
	char ifName[MAX_IFNAME_LST_NUM], ifType[MAX_IFTYPE_LST_NUM];
	char *p = NULL;
	int ifNum = pbif->ifNum, i = 0;
	char *pName = ifName;
#ifdef TCSUPPORT_E8B
	char *pType = ifType;
#endif
	int find = 0;

	if (pbif == NULL) {
		return -1;
	}
	strcpy(ifName, pbif->ifNameLst);
	strcpy(ifType, pbif->ifTypeLst);
	DPRINTK("%s:group id is %d, ifNum is %d\r\nifName list is %s, ifType is %s.\r\n", __FUNCTION__, pbif->groupid, ifNum, pbif->ifNameLst, pbif->ifTypeLst);
	
	pIftmp = pIf = (struct groupif *)kmalloc(ifNum * sizeof(struct groupif), GFP_KERNEL);
	if (pIf == NULL) {
		printk("Allocate memory for group interface failed. \n");
		return -1;
	}

	memset(pIf, 0, sizeof(ifNum * sizeof(struct groupif)));
	
	/* get interface name and store it into groupif struct */
	while ((p = strsep(&pName, ",")) && (i++ < ifNum)) {
		strcpy(pIf->ifName, p);
		pIf++;
	}

#ifdef TCSUPPORT_E8B
	/* get interface type and store it into groupif struct */
	i = 0;
	pIf = pIftmp;
	while ((p = strsep(&pType, ",")) && (i++ < ifNum)) {
		if ((strcmp(p, (char*)"I") != 0) && (strcmp(p, (char*)"O") != 0)) {
			kfree(pIftmp);
			return -1;
		}
		strcpy(pIf->ifType, p);
		pIf++;
	}
#endif

	for (i = 0; i < MAX_GROUP_NUM; i++) {
		if (gpLst[i].groupid == pbif->groupid) {
			find = 1;
			break;
		}
	}
	if (find == 1) {
		/* There is the same group in group list, update it. */
		kfree(gpLst[i].pIf);
		gpLst[i].pIf = pIftmp;
		gpLst[i].ifNum = ifNum;
		return 0;
	}
	
	/* search group list and find an empty entry to this group info */
	for (i = 0; i < MAX_GROUP_NUM; i++) {
		if (gpLst[i].pIf == NULL) {
			break;
		}
	}

	if (i == MAX_GROUP_NUM) {
		printk("%s:There is no empty entry to store group info.\r\n", __FUNCTION__);
		kfree(pIftmp);
		return -1;
	}

	gpLst[i].groupid = pbif->groupid;
	gpLst[i].pIf = pIftmp;
	gpLst[i].ifNum = ifNum;

	return 0;
}

static int portbind_delgroup(struct portbindcmdif *pbif)
{
	int i, find = 0;

	if (pbif == NULL) {
		return -1;
	}
	DPRINTK("%s:group id is %d, ifName list is %s, ifType is %s.\r\n", pbif->groupid, pbif->ifNameLst, pbif->ifTypeLst);
	for (i = 0; i < MAX_GROUP_NUM; i++) {
		if (gpLst[i].groupid == pbif->groupid) {
			find = 1;
			break;
		}
	}

	/* delete this group, but if it has been deleted, just return */
	if (find == 1 && gpLst[i].pIf) {
		kfree(gpLst[i].pIf);
		gpLst[i].pIf = NULL;
		gpLst[i].groupid = -1;
	}

	return 0;
}

static int portbind_showgroup(struct portbindcmdif *pbif)
{
	int i, j, find = 0;
	char ifNameLst[MAX_IFNAME_LST_NUM];
	char ifTypeLst[MAX_IFTYPE_LST_NUM];
	struct groupif *pIf = NULL;

	if (pbif == NULL) {
		return -1;
	}
	DPRINTK("%s:group id is %d, ifName list is %s, ifType is %s.\r\n", pbif->groupid, pbif->ifNameLst, pbif->ifTypeLst);
	if (pbif->groupid == -1) {
		/* show all group info, valid group id is from 0~15, -1 means all group. */
		printk("Group List info.Portbinding is %s\r\n", g_portbind_enable ? "enable" : "disable");
		for (i = 0; i < MAX_GROUP_NUM; i++) {
			if (gpLst[i].pIf != NULL) {
				memset(ifNameLst, 0, sizeof(ifNameLst));
				memset(ifTypeLst, 0, sizeof(ifTypeLst));

				pIf = gpLst[i].pIf;
				
				for (j = 0; j < gpLst[i].ifNum; j++) {
					strcat(ifNameLst, pIf->ifName);
					if (j != gpLst[i].ifNum - 1) {
						strcat(ifNameLst, ",");
					}
				#ifdef TCSUPPORT_E8B
					strcat(ifTypeLst, pIf->ifType);
					if (j != gpLst[i].ifNum - 1) {
						strcat(ifTypeLst, ",");
					}
				#endif
					pIf++;
				}
				printk("group id:[%d]\r\ninterface name list:[%s]\r\ninterface type list:[%s]\r\n", gpLst[i].groupid, ifNameLst, ifTypeLst);
			}
		}

		return 0;
	}

	/* show specific group */
	for (i = 0; i < MAX_GROUP_NUM; i++) {
		if (gpLst[i].groupid == pbif->groupid) {
			find = 1;
			break;
		}
	}

	if (find == 1) {
		memset(ifNameLst, 0, sizeof(ifNameLst));
		memset(ifTypeLst, 0, sizeof(ifTypeLst));
		pIf = gpLst[i].pIf;
		for (j = 0; j < gpLst[i].ifNum; j++) {
			strcat(ifNameLst, pIf->ifName);
			if (j != gpLst[i].ifNum - 1) {
				strcat(ifNameLst, ",");
			}
		#ifdef TCSUPPORT_E8B
			strcat(ifTypeLst, pIf->ifType);
			if (j != gpLst[i].ifNum - 1) {
				strcat(ifTypeLst, ",");
			}
		#endif
			pIf++;
		}

		printk("group id:[%d]\r\ninterface name list:[%s]\r\ninterface type list:[%s]\r\n", pbif->groupid, ifNameLst, ifTypeLst);
			   
		return 0;
	}

	return 0;
}

#if 0
int
portbind_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, void __user *arg)
#else
int
portbind_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg)

#endif
{
	struct portbindcmdif pbif;

	DPRINTK("portbind_ioctl entered\n");
	
	memset(&pbif, 0, sizeof(pbif));
	switch (cmd) {
	case PORTBIND_IOC_ENABLE:
		DPRINTK("portbind_ioctl: PORTBIND_IOC_ENABLE\n");
		g_portbind_enable = 1;
		break;
	case PORTBIND_IOC_DISABLE:
		DPRINTK("portbind_ioctl: PORTBIND_IOC_DISABLE\n");
		g_portbind_enable = 0;
		break;
	case PORTBIND_IOC_ADD_GROUP:
		DPRINTK("portbind_ioctl: PORTBIND_IOC_ADD_GROUP\n");
		if (copy_from_user(&pbif, (struct portbindcmdif*)arg, sizeof(pbif))) {
			return -EFAULT;
		}
		portbind_addgroup(&pbif);
		break;
	case PORTBIND_IOC_DEL_GROUP:
		DPRINTK("portbind_ioctl: PORTBIND_IOC_DEL_GROUP\n");
		if (copy_from_user(&pbif, (struct portbindcmdif*)arg, sizeof(pbif))) {
			return -EFAULT;
		}
		portbind_delgroup(&pbif);
		break;
	case PORTBIND_IOC_SHOW_GROUP:
		DPRINTK("PORTBIND_IOC_SHOW_GROUP\n");
		if (copy_from_user(&pbif, (struct portbindcmdif*)arg, sizeof(pbif))) {
			return -EFAULT;
		}
		portbind_showgroup(&pbif);
		break;
	default:
		break;
	}

	return 0;
}


static struct file_operations portbind_fops = {
	.owner =		THIS_MODULE,
	.write =		NULL,
	.read =			NULL,
	.ioctl =		portbind_ioctl,
	.open =			portbind_open,
	.release =		NULL,
};

/***************************************************************************
 * Function Name: portbind_drv_init
 * Description  : Initialization of portbind driver
 * Returns      : 0
 ***************************************************************************/
static int __init portbind_drv_init(void)
{
	int status;
	
	printk("\r\ntcportbind version: %s.\n", MODULE_VERSION);
	/* portbind ioctl */
	DPRINTK("drv init begin\n");
	status = register_chrdev(PORTBIND_MAJOR, "pmap", &portbind_fops);
	if (status < 0)
		return status;

	memset(gpLst, 0, sizeof(gpLst));

	rcu_assign_pointer(portbind_sw_hook, getPortBindEnable);
	rcu_assign_pointer(portbind_check_hook, checkGroup);

	DPRINTK("drv init ok\n");
  	return 0;
}

/*________________________________________________________________________
** Function Name: portbind_drv_exit
** Description  : portbind module clean routine
** Returns      : None
**_______________________________________________________________________
*/

static void __exit portbind_drv_exit(void)
{
	DPRINTK("drv exit begin\n");
	rcu_assign_pointer(portbind_sw_hook, NULL);
	rcu_assign_pointer(portbind_check_hook, NULL);
	unregister_chrdev(PORTBIND_MAJOR, "pmap");
	DPRINTK("drv exit end\n");
}

module_init(portbind_drv_init);
module_exit(portbind_drv_exit);
