#ifndef _QOS_TYPE_H_
#define _QOS_TYPE_H_

#define	 MAX_BUF_LEN				64
#define  MAX_RULE_NUM				16

#define  EBT_CHK_TYPE				1
#define  DEV_XMIT_CHK_TYPE			2


#define QOSTYPE_IOC_MAGIC 			'g'
#define QOSTYPE_MAJOR 				111               
#define QOSTYPE_IOC_SET_TYPE _IOR(QOSTYPE_IOC_MAGIC,10,unsigned long)

/* type_name: wan_if, rtp_proto */
struct qos_match
{
	char type_name[MAX_BUF_LEN];
	char type_value[MAX_BUF_LEN];
};

/* qm[0]: wan_if, qm[1]: rtp_proto */
struct qos_action
{
	struct qos_match qm[2];
	/* if match, set dscp here */
	int dscp_flag;
	unsigned int dscp;
};

struct qos_type
{
	int rule_no;
	struct qos_match qm[2];
};

int qostype_chk(int chkType, int rule_no, char *wan_if, int rtp_match);
int set_tos(int rule_no, unsigned int tos);
int get_tos(int rule_no, unsigned int *tos);
int unset_tos(int rule_no);

#endif

