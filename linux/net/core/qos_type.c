#include <net/sock.h>
#include <linux/qos_type.h>

static struct qos_action qa[MAX_RULE_NUM];

int qostype_chk(int chk_type, int rule_no, char *wan_if, int rtp_match)
{
	struct qos_action *pqa;
	char value[MAX_BUF_LEN];
	char *pval;
	char *p = NULL;
	
	if (rule_no < 0 || rule_no > MAX_RULE_NUM - 1) {
		printk("%s:rule no should be between 0 and %d.\n", __FUNCTION__, MAX_RULE_NUM - 1);
		return -1;
	}

	pqa = qa + rule_no;
	
	if (chk_type == EBT_CHK_TYPE) {
		if (!strcmp(pqa->qm[0].type_name, "wan_if") || !strcmp(pqa->qm[1].type_name, "rtp_proto")) {
			return 0;
		}
		else {
			return -1;
		}
	}
	
	if (wan_if == NULL) {
		return -1;
	}

	if (!strcmp(pqa->qm[0].type_name, "wan_if")) {
		strcpy(value, pqa->qm[0].type_value);
		/* check if wan interface is in group */
		pval = value;
		while ((p = strsep(&pval, ",")) != NULL) {
			if (!strcmp(p, wan_if)) {
				goto rtp_proto_handle;
			}
		}
		return -1;
	}

rtp_proto_handle:
	if ( (strcmp(pqa->qm[1].type_name, "rtp_proto") != 0) || 
		 (!strcmp(pqa->qm[1].type_name, "rtp_proto") && (1 == rtp_match)) ) {
		return 0;
	}

	return -1;
}

int set_tos(int rule_no, unsigned int tos)
{
	struct qos_action *pqa;
	if (rule_no < 0 || rule_no > MAX_RULE_NUM) {
		printk("%s:rule no should be between 0 and %d.\n", __FUNCTION__, MAX_RULE_NUM - 1);
		return -1;
	}

	pqa = &qa[rule_no];

	pqa->dscp_flag = 1;
	pqa->dscp = tos;
	return 0;
}

int unset_tos(int rule_no)
{
	struct qos_action *pqa;
	if (rule_no < 0 || rule_no > MAX_RULE_NUM) {
		printk("%s:rule no should be between 0 and %d.\n", __FUNCTION__, MAX_RULE_NUM - 1);
		return -1;
	}

	pqa = &qa[rule_no];

	pqa->dscp_flag = 0;

	return 0;
}

int get_tos(int rule_no, unsigned int *tos)
{
	struct qos_action *pqa;
	if (rule_no < 0 || rule_no > MAX_RULE_NUM) {
		printk("%s:rule no should be between 0 and %d.\n", __FUNCTION__, MAX_RULE_NUM - 1);
		return -1;
	}

	pqa = &qa[rule_no];

	if (0 == pqa->dscp_flag) {
		return -1;
	}

	*tos = pqa->dscp;

	return 0;
}

static int qostype_settype(struct qos_type *pqt)
{
	int rule_no = pqt->rule_no;
	if (rule_no < 0 || rule_no > MAX_RULE_NUM - 1) {
		printk("%s:rule no should be between 0 and %d.\n", __FUNCTION__, MAX_RULE_NUM - 1);
		return -1;
	}

	memcpy(qa[pqt->rule_no].qm, pqt->qm, sizeof(struct qos_match)*2);

	return 0;
}

int
qostype_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg)
{
	struct qos_type qt;
	
	switch (cmd) {
	case QOSTYPE_IOC_SET_TYPE:
		if (copy_from_user(&qt, (struct qos_type*)arg, sizeof(qt))) {
			return -EFAULT;
		}
		qostype_settype(&qt);
		break;
	default:
		break;
	}

	return 0;
}

int qostype_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static struct file_operations qostype_fops = {
	.owner =		THIS_MODULE,
	.write =		NULL,
	.read =			NULL,
	.ioctl =		qostype_ioctl,
	.open =			qostype_open,
	.release =		NULL,
};

/***************************************************************************
 * Function Name: qostype_drv_init
 * Description  : Initialization of qostype driver
 * Returns      : 0
 ***************************************************************************/
static int __init qostype_drv_init(void)
{
	int status;
	
	/* qostype ioctl */
	status = register_chrdev(QOSTYPE_MAJOR, "qostype", &qostype_fops);
	if (status < 0)
		return status;
	
	memset(qa, 0, sizeof(qa));

  	return 0;
}


/*_______________________________________________________________________
** Function Name: qostype_drv_exit
** Description  : qostype module clean routine
** Returns      : None
**_______________________________________________________________________
*/
static void __exit qostype_drv_exit(void)
{
	unregister_chrdev(QOSTYPE_MAJOR, "qostype");
}

EXPORT_SYMBOL(qostype_chk);
EXPORT_SYMBOL(set_tos);
EXPORT_SYMBOL(unset_tos);

module_init(qostype_drv_init);
module_exit(qostype_drv_exit);

