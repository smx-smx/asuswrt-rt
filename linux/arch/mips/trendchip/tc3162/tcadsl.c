/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/arch/mips/trendchip/tc3162/tcadsl.c#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
/*
** $Log: tcadsl.c,v $
** Revision 1.1.1.1  2009/12/17 01:43:39  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.1.1.1  2007/04/12 09:42:02  ian
** TCLinuxTurnkey2007
**
** Revision 1.5  2006/07/06 07:24:23  lino
** update copyright year
**
** Revision 1.4  2006/07/06 04:38:28  lino
** add kernel module support
**
** Revision 1.3  2006/01/04 06:10:18  lino
** add diag, neatituid and farituid ci command
**
** Revision 1.2  2005/11/03 11:18:03  lino
** show adsl uptime
**
** Revision 1.1.1.1  2005/11/02 05:45:19  lino
** no message
**
*/
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <asm/addrspace.h>
#include <asm/tc3162/tc3162.h>	
#include <asm/tc3162/TCIfSetQuery_os.h>	
#include <asm/tc3162/ledcetrl.h>	
#include <asm/tc3162/cmdparse.h>	

#ifdef CONFIG_TC3162_ADSL

#define MAX_ROOT_CMD	16

unsigned int adslUptime = 0;

#define NARG		40

#define FALSE		0
#define TRUE		1
#if 0 /*move into cmdparse.h*/
typedef	struct {
	char *name;		
	int	(*func)(int argc,char *argv[],void *p);
	int	flags;	
	int	argcmin;
	char *argc_errmsg;	
} cmds_t;
#endif

extern int RtsCiCmd(int argc, char *argv[], void *p);
extern int dmt_ci_cmd(int argc, char *argv[], void *p);
extern int dmt2_ci_cmd(int argc, char *argv[], void *p);
extern int hw_ci_cmd(int argc, char *argv[], void *p);
extern int SwCiCmd(int argc, char *argv[], void *p);
extern int ghs_ci_cmd(int argc, char *argv[], void *p);
extern int TCIfCiCmd(int argc, char *argv[], void *p);

static int doWan(int argc, char *argv[], void *p);

static int doAdsl(int argc, char *argv[], void *p);
static int doAdslReset(int argc, char *argv[], void *p); 
static int doAdslOpenCmd(int argc, char *argv[], void *p); 
static int doAdslOpen(int argc, char *argv[], void *p); 
static int doAdslClose(int argc, char *argv[], void *p); 
static int doAdslGlite(int argc, char *argv[], void *p);
static int doAdslT1413(int argc, char *argv[], void *p);
static int doAdslGdmt(int argc, char *argv[], void *p);
static int doAdslMultimode(int argc, char *argv[], void *p);
static int doAdslAdsl2(int argc, char *argv[], void *p);//Jason_931110
static int doAdslAdsl2plus(int argc, char *argv[], void *p);//Jason_931110
static int doAdslFwVersion(int argc, char *argv[], void *p);
static int doAdslStatus(int argc, char *argv[], void *p);
static int doAdslChanndata(int argc, char *argv[], void *p);
static int doAdslPerfdata(int argc, char *argv[], void *p);
static int doAdslLinedata(int argc, char *argv[], void *p);
static int doAdslLinedataFar(int argc, char *argv[], void *p);
static int doAdslLinedataNear(int argc, char *argv[], void *p);
static int doAdslOpmode(int argc, char *argv[], void *p);
static int doAdslDiag(int argc, char *argv[], void *p);
static int doAdslNearItuId(int argc, char *argv[], void *p);
static int doAdslFarItuId(int argc, char *argv[], void *p);

static int doSys(int argc, char *argv[], void *p);
static int doSysMemrl(int argc, char *argv[], void *p);
static int doSysMemwl(int argc, char *argv[], void *p);
static int doSysMemrw(int argc, char *argv[], void *p);
static int doSysMemww(int argc, char *argv[], void *p);
static int doSysMemory(int argc, char *argv[], void *p);
static int doSysMemcpy(int argc, char *argv[], void *p);
static int doSysFillMem(int argc, char *argv[], void *p);

static int doTask(int argc, char *argv[], void *p);
static int doTaskDisp(int argc, char *argv[], void *p);

int cmd_reg_add(char *cmd_name, cmds_t *cmds_p);
int cmd_register(cmds_t *cmds_p);

int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);

/* Command lookup and branch tables */
cmds_t Cmds[MAX_ROOT_CMD];
#if 0 /*Support register root ci-cmd by cmd_register*/
const cmds_t Cmds[] = {
	{"wan",			doWan,			0x12, 	0,	NULL},
	{"sys",			doSys,			0x12, 	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},	
};
#endif
EXPORT_SYMBOL(Cmds);

static cmds_t wanCmds[] = {
	{"adsl",		doAdsl, 		0x10,	0,	NULL},
	/* reserved space for adsl ci commands */
#if 1
	{NULL,			NULL, 			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL, 			NULL,			0x10,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
#else
	{"rts",			RtsCiCmd, 		0x10,	0,	NULL},
	{"dmt", 		dmt_ci_cmd,		0x10,	0,	NULL},
	{"dmt2", 		dmt2_ci_cmd,	0x10,	0,	NULL},
	{"hwdmt", 		hw_ci_cmd,		0x10,	0,	NULL},
	{"swdmt", 		SwCiCmd,		0x10,	0,	NULL},
	{"ghs",			ghs_ci_cmd,		0x10,	0,	NULL},
	{"tcif",		TCIfCiCmd,		0x10,	0,	NULL},	
#endif
	{NULL,			NULL,			0x10,	0,	NULL},	
}; 

static const cmds_t adslCmds[] = {
	{"reset",		doAdslReset,	0x02,  	0,  NULL},
	{"open",		doAdslOpen,		0x02,  	0,  NULL},
	{"opencmd",		doAdslOpenCmd,	0x12,  	0,  NULL},
	{"fwversion",	doAdslFwVersion,0x02,  	0,  NULL},
	{"status",		doAdslStatus,	0x02,  	0,  NULL},
	{"chandata", 	doAdslChanndata,0x02,  	0,  NULL},
	{"perfdata", 	doAdslPerfdata,	0x02,  	0,  NULL},
	{"linedata", 	doAdslLinedata,	0x12,  	0,  NULL},
	{"opmode", 		doAdslOpmode,	0x02,  	0,  NULL},
	{"close",		doAdslClose,	0x02,  	0,  NULL},
	{"diag",		doAdslDiag,		0x02,  	0,  NULL},
	{"nearituid",	doAdslNearItuId,0x02,  	0,  NULL},
	{"farituid",	doAdslFarItuId,	0x02,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},	
};

static const cmds_t adslOpenCmds[] =
{
	{"glite",		doAdslGlite,	0x02,   0,  NULL},
	{"t1.413",		doAdslT1413,	0x02,	0,  NULL},
   	{"gdmt",		doAdslGdmt,		0x02,   0,  NULL},
   	{"multimode",	doAdslMultimode,0x02,  	0,  NULL},
   	{"adsl2",		doAdslAdsl2,	0x02,   0,  NULL},//Jason_931110
   	{"adsl2plus",	doAdslAdsl2plus,0x02, 	0,  NULL},//Jason_931110
	{NULL,			NULL,			0x10,	0,	NULL},	
};

static const cmds_t adslLinedataCmds[] =
{
	{"far",			doAdslLinedataFar,	0x02,   0,  NULL},
	{"near",		doAdslLinedataNear,	0x02,   0,  NULL},
	{NULL,			NULL,				0x10,	0,	NULL},	
};

static const cmds_t sysCmds[] = {
	{"memrl",		doSysMemrl,		0x02,  	1,  NULL},
	{"memwl",		doSysMemwl,		0x02,  	2,  NULL},
	{"memrw",		doSysMemrw,		0x02,  	1,  NULL},
	{"memww",		doSysMemww,		0x02,  	2,  NULL},
	{"memory",		doSysMemory,	0x02,  	2,  NULL},
	{"memcpy",		doSysMemcpy,	0x02,  	3,  NULL},
	{"fillmem",		doSysFillMem,	0x02,  	3,  NULL},
	{"task",		doTask,			0x12,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},	
};
static const cmds_t taskCmds[] = {
	{"disp",		doTaskDisp,		0x02,  	0,  NULL},
	{NULL,			NULL,			0x10,	0,	NULL},
};


int cmd_reg_add(char *cmd_name, cmds_t *cmds_p)
{
	return 0;
}
EXPORT_SYMBOL(cmd_reg_add);

/*_____________________________________________________________________________
**      function name: cmd_register
**      descriptions:
**         Register a new ci-cmd into top ci-cmd tables.
**          
**      parameters:
**             cmds_p: Specify you want to register ci-cmd
**            	
**      global:
**             Cmds
**             
**      return:
**              Success:        0
**              Otherwise:     -1
**	     
**      call:
**   	None
**      		
**      revision:
**      1. Here 2008/8/13
**____________________________________________________________________________
*/
int 
cmd_register(cmds_t *cmds_p)
{
	uint8 i=0;
	for(i=0; i < MAX_ROOT_CMD; i++){
		if(Cmds[i].name == NULL){
			memcpy(&Cmds[i], cmds_p, sizeof(cmds_t));
			return 0;
		}
	}
	
	printk("Can't register ci-cmd, Cmds is full\n");
	return -1;
}/*end cmd_register*/

EXPORT_SYMBOL(cmd_register);

int
cmd_unregister(cmds_t *cmds_p)
{
	uint8 i=0;
	for(i=0; i < MAX_ROOT_CMD; i++){
		if(strcmp(Cmds[i].name,cmds_p->name) == 0){
			Cmds[i].name = NULL;
			return 0;
		}
	}

	printk("Can't un-register ci-cmd, Cmds is incorrect\n");
	return -1;
}/*end cmd_register*/

EXPORT_SYMBOL(cmd_unregister);

/* Call a subcommand based on the first token in an already-parsed line */
int subcmd	(
	const cmds_t tab[], int argc, char *argv[], void *p
)
{
	register const cmds_t *cmdp;
	int found = 0;
	int i;

	/* Strip off first token and pass rest of line to subcommand */
	if (argc < 2) {
		if (argc < 1)
			printk("SUBCMD - Don't know what to do?\n");
		else 
			goto print_out_cmds;
			//printk("\"%s\" - takes at least one argument\n",argv[0]);
		return -1;
	}
	argc--;
	argv++;
	for(cmdp = tab;cmdp->name != NULL;cmdp++){
		if(strncmp(argv[0],cmdp->name,strlen(argv[0])) == 0){
			found = 1;
			break;
		}
	}
	if(!found){
		char buf[66];
		
print_out_cmds:
		printk("valid subcommands:\n");
		memset(buf,' ',sizeof(buf));
		buf[64] = '\n';
		buf[65] = '\0';
		for(i=0,cmdp = tab;cmdp->name != NULL;cmdp++){
			strncpy(&buf[i*16],cmdp->name,strlen(cmdp->name));
			if(i == 3){
				printk(buf);
				memset(buf,' ',sizeof(buf));
				buf[64] = '\n';
				buf[65] = '\0';
			}
			i = (i+1)%4;
		}
		if(i != 0)
			printk(buf);
		return -1;
	}
	if(argc <= cmdp->argcmin){
		if(cmdp->argc_errmsg != NULL)
			printk("Usage: %s\n",cmdp->argc_errmsg);
		return -1;
	}
	return (*cmdp->func)(argc,argv,p);
}
EXPORT_SYMBOL(subcmd);

static char *
stringparse(char *line)
{
	register char *cp = line;
	unsigned long num;

	while ( *line != '\0' && *line != '\"' ) {
		if ( *line == '\\' ) {
			line++;
			switch ( *line++ ) {
			case 'n':
				*cp++ = '\n';
				break;
			case 't':
				*cp++ = '\t';
				break;
			case 'v':
				*cp++ = '\v';
				break;
			case 'b':
				*cp++ = '\b';
				break;
			case 'r':
				*cp++ = '\r';
				break;
			case 'f':
				*cp++ = '\f';
				break;
			case 'a':
				*cp++ = '\a';
				break;
			case '\\':
				*cp++ = '\\';
				break;
			case '\?':
				*cp++ = '\?';
				break;
			case '\'':
				*cp++ = '\'';
				break;
			case '\"':
				*cp++ = '\"';
				break;
			case 'x':
				num = simple_strtoul( --line, &line, 16 );
				*cp++ = (char) num;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				num = simple_strtoul( --line, &line, 8 );
				*cp++ = (char) num;
				break;
			case '\0':
				return NULL;
			default:
				*cp++ = *(line - 1);
				break;
			};
		} else {
			*cp++ = *line++;
		}
	}

	if ( *line == '\"' )
		line++; 	/* skip final quote */
	*cp = '\0';		/* terminate string */
	return line;
}

/* replace terminating end of line marker(s) with null */
void 
rip(register char *s)
{
	register char *cp;

	if((cp = strchr(s,'\n')) != NULL)
		*cp = '\0';
	if((cp = strchr(s,'\r')) != NULL)
		*cp = '\0';
}

int 
cmdparse(
	const cmds_t cmds[], char *line, void *p
)
{
	const cmds_t *cmdp;
	char *argv[NARG],*cp;
	int argc;

	/* Remove cr/lf */
	rip(line);

	for(argc = 0;argc < NARG;argc++)
		argv[argc] = NULL;

	for(argc = 0;argc < NARG;){
		register int qflag = FALSE;

		/* Skip leading white space */
		while(*line == ' ' || *line == '\t')
			line++;
		if(*line == '\0')
			break;
		/* return if comment character first non-white */
		if ( argc == 0  &&  *line == '#' )
			return 0;
		/* Check for quoted token */
		if(*line == '"'){
			line++;	/* Suppress quote */
			qflag = TRUE;
		}
		argv[argc++] = line;	/* Beginning of token */

		if(qflag){
			/* Find terminating delimiter */
			if((line = stringparse(line)) == NULL){
				return -1;
			}
		} else {
			/* Find space or tab. If not present,
			 * then we've already found the last
			 * token.
			 */
			if((cp = strchr(line,' ')) == NULL
			 && (cp = strchr(line,'\t')) == NULL){
				break;
			}
			*cp++ = '\0';
			line = cp;
		}
	}
	if (argc < 1) {		/* empty command line */
		argc = 1;
		argv[0] = "";
	}
	/* Look up command in table; prefix matches are OK */
	for(cmdp = cmds;cmdp->name != NULL;cmdp++){
		if(strncmp(argv[0],cmdp->name,strlen(argv[0])) == 0) {
			break;
		}
	}
	if(cmdp->name == NULL) {
		if(cmdp->argc_errmsg != NULL)
			printk("%s\n",cmdp->argc_errmsg);
		return -1;
	}
	if(argc < cmdp->argcmin) {
		/* Insufficient arguments */
		printk("Usage: %s\n",cmdp->argc_errmsg);
		return -1;
	}
	if(cmdp->func == NULL)
		return 0;
	return (*cmdp->func)(argc,argv,p);
}
EXPORT_SYMBOL(cmdparse);

int doWan(int argc, char *argv[], void *p)
{
	return subcmd(wanCmds, argc, argv, p);
}

/************************************************************************
*		     A D S L   D A T A   S T R U C T U R E
*************************************************************************
*/

static struct timer_list adsl_timer;

typedef struct {
	uint16          actualBitrateNearInt;
	uint16          actualBitrateNearFast;
	uint16          actualBitrateFarInt;
	uint16          actualBitrateFarFast;
} T_adslChannelOperData;

typedef struct {
    uint16          FecNotInterleaved;
    uint16          FecInterleaved;
    uint16          CrcNotInterleaved;
    uint16          CrcInterleaved;
    uint16          HecNotInterleaved;
    uint16          HecInterleaved;
} T_adsl_def_counters;

typedef struct {
    T_adsl_def_counters     near_end;
    T_adsl_def_counters     far_end;
} T_adsl_def_counter_set;

typedef struct {
        uint16          relCapacityOccupationDnstr;
        signed short     noiseMarginDnstr;//Jason_931028
        signed short     outputPowerUpstr;//Jason_931028
        uint16           attenuationDnstr;//Jason_931028
} T_NTadslNearEndLineOperData;

typedef struct {
        uint16          relCapacityOccupationUpstr;
        signed short     noiseMarginUpstr;//Jason_931028
        signed short     outputPowerDnstr;//Jason_931028
        uint16           attenuationUpstr;//Jason_931028
        uint8           carrierLoad[256]; //Jason_931130
} T_NTadslFarEndLineOperData;

int 
getXdslSpeed(void)
{
	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;

	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL);		   //Jason_920527
	if (modemst != ADSL_MODEM_STATE_UP)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL); 		 //Jason_920527
	return (adslChannelOperData.actualBitrateFarFast+adslChannelOperData.actualBitrateFarInt);
}    

void os_TCIfQuery(unsigned short query_id, void *result1, void *result2)
{
	if (adsl_dev_ops == NULL)
		return;

	adsl_dev_ops->query(query_id, result1, result2);	
}    
EXPORT_SYMBOL(os_TCIfQuery);

void os_TCIfSet(unsigned short set_id, void *value1, void *value2)
{
	if (adsl_dev_ops == NULL)
		return;

	adsl_dev_ops->set(set_id, value1, value2);	
}
EXPORT_SYMBOL(os_TCIfSet);

void adslTimer(unsigned long data)
{
	static uint8 doAdslInit = 0;
	uint8 modemst;

	if (adsl_dev_ops == NULL)
		goto down_proc;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL);		  
	switch (modemst) {
		case ADSL_MODEM_STATE_DOWN:
down_proc:
			doAdslInit = 0;
			ledTurnOn(LED_DSL_IDLE_STATUS);
			if (ledGetMode(LED_DSL_IDLE_STATUS) == LED_MODE_NOT_USED)
				ledTurnOff(LED_DSL_STATUS);
			ledTurnOff(LED_DSL_ACT_STATUS);
			ledTurnOff(LED_DSL_NOACT_STATUS);
			ledTurnOff(LED_PPP_STATUS);
			ledTurnOff(LED_PPP_ACT_STATUS);
			ledTurnOff(LED_PPP_NOACT_STATUS);
			break;
		case ADSL_MODEM_STATE_WAIT_INIT:
			doAdslInit = 0;
			ledTurnOn(LED_DSL_IDLE_STATUS);
			if (ledGetMode(LED_DSL_IDLE_STATUS) == LED_MODE_NOT_USED)
				ledTurnOff(LED_DSL_STATUS);
			ledTurnOff(LED_DSL_ACT_STATUS);
			ledTurnOff(LED_DSL_NOACT_STATUS);
			ledTurnOff(LED_PPP_STATUS);
			ledTurnOff(LED_PPP_ACT_STATUS);
			ledTurnOff(LED_PPP_NOACT_STATUS);

			/* polling ethernet */
			//if (!(VPint(CR_INTC_IMR) & (1 << MAC_INT)))
			//		tc3162_emac_poll();
			break;
		case ADSL_MODEM_STATE_INIT:
			doAdslInit = 0;
			ledTurnOn(LED_DSL_TRAINING_STATUS);

			/* polling ethernet */
			//if (!(VPint(CR_INTC_IMR) & (1 << MAC_INT)))
			//	tc3162_emac_poll();
			break;
		case ADSL_MODEM_STATE_UP:
			if (!doAdslInit) {
				adslUptime = jiffies_to_msecs(jiffies);
				doAdslInit = 1;
				ledTurnOn(LED_DSL_SHOWTIME_STATUS);
				ledTurnOn(LED_DSL_NOACT_STATUS);
			}
			break;
	}
	mod_timer(&adsl_timer, jiffies + (HZ / 2)); 
}

/************************************************************************
*		     A D S L   C I   C O M M A N D
*************************************************************************
*/
static void adslReset(void)
{
	uint32 mask = 0xFFFFFFFE;               
	uint16 flag = 1;

	if (adsl_dev_ops == NULL)
		return;

	adsl_dev_ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL);
	adsl_dev_ops->set(ADSL_SET_R_SOFT_RESET, &flag, NULL); 
   	adsl_dev_ops->set(ADSL_SET_INT_MASK0, &mask, NULL );       
}

int doAdsl(int argc, char *argv[], void *p)
{
	return subcmd(adslCmds, argc, argv, p);
}

int doAdslReset(int argc, char *argv[], void *p)
{
	adslReset();
	return 0;
}

int doAdslOpenCmd(int argc, char *argv[], void *p)
{
	return subcmd(adslOpenCmds, argc, argv, p);
}

int doAdslOpen(int argc, char *argv[], void *p)
{
	uint8 modemst;
	uint32 mask = 0xFFFFFFFE;               
	uint16 flag = 1;

	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst == ADSL_MODEM_STATE_UP) {
		printk("ADSL line is up.\n");
		return 0;
	}

	adsl_dev_ops->set(ADSL_SET_R_SOFT_RESET, &flag, NULL); 
   	adsl_dev_ops->set(ADSL_SET_INT_MASK0, &mask, NULL );       
	return 0;
}

int doAdslClose(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL);
	return 0;
}

int doAdslGlite(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_MODE_LITE, NULL, NULL);
	adslReset();
	return 0;
}

int doAdslT1413(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_MODE_ANSI, NULL, NULL);
	adslReset();
	return 0;
}

int doAdslGdmt(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_MODE_A43, NULL, NULL);
	adslReset();
	return 0;
}

int doAdslMultimode(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_MODE_MULTIMODE, NULL, NULL);
	adslReset();
	return 0;
}

int doAdslAdsl2(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_MODE_ADSL2, NULL, NULL);
	adslReset();
	return 0;
}

int doAdslAdsl2plus(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->set(ADSL_SET_MODE_ADSL2PLUS, NULL, NULL);
	adslReset();
	return 0;
}

int
getADSLFwVer(char *modemCode)
{
	uint16	index=0;

	uint8 fw_ver_r, fw_ver_p, fw_rev, tdm_ver, fdm_ver, annex_afe, b_ver;
	uint8   major;
	uint8   minor;

	if (adsl_dev_ops == NULL)
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_HW_VER, &major, &minor);           //Jason_920529
   	adsl_dev_ops->query(ADSL_QUERY_FW_VER, &fw_ver_r, &fw_ver_p);     //Jason_920527
   	adsl_dev_ops->query(ADSL_QUERY_FW_REV, &fw_rev, &annex_afe);      //Jason_920527
   	adsl_dev_ops->query(ADSL_QUERY_TDM_FDM_VER, &tdm_ver, &fdm_ver);  //Jason_920527
   	adsl_dev_ops->query(ADSL_QUERY_BUILD_VERSION, &b_ver, NULL ); //Jason_931108
        
	index = sprintf( modemCode," FwVer:");
	index += sprintf( modemCode+index, "%d.", fw_ver_r);
	index += sprintf( modemCode+index, "%d.", fw_ver_p);
	index += sprintf( modemCode+index, "%d.", fw_rev);		
	index += sprintf( modemCode+index, "%d_", b_ver);
	index += sprintf( modemCode+index, "%c_", (annex_afe & 0x02)?'A':'B');

	annex_afe = annex_afe & 0xe0;	
	if (annex_afe == 0x20)
		index += sprintf( modemCode+index, "%s", "TC3084");
	else if (annex_afe == 0x40)
		index += sprintf( modemCode+index, "%s", "TC3085");
	else
		index += sprintf( modemCode+index, "%s", "TC3084");
	index += sprintf( modemCode+index, " HwVer:T%d.F%d_%d.%d\n", tdm_ver, fdm_ver, major, minor);

	return index;
}

int doAdslFwVersion(int argc, char *argv[], void *p)
{
	char fwver[80];

	if (getADSLFwVer(fwver))
	printk("%s", fwver);
	return 0;
}

int doAdslStatus(int argc, char *argv[], void *p)
{
	uint8 modemst;

	if (adsl_dev_ops == NULL) {
		printk("ADSL link status: down\n");
		return 0;
	}

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst == ADSL_MODEM_STATE_DOWN) 
		printk("ADSL link status: down\n");
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT) 
		printk("ADSL link status: wait for init\n");
	else if (modemst == ADSL_MODEM_STATE_INIT) 
		printk("ADSL link status: initializing\n");
	else if (modemst == ADSL_MODEM_STATE_UP) 
		printk("ADSL link status: up\n");
	else 
		printk("ADSL link status: unknown\n");
	return 0;
}

int doAdslChanndata(int argc, char *argv[], void *p)
{
	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;

	if (adsl_dev_ops == NULL) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst != ADSL_MODEM_STATE_UP) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL); 		 

	printk("near-end interleaved channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateNearInt);
	printk("near-end fast channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateNearFast);
	printk("far-end interleaved channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateFarInt);
	printk("far-end fast channel bit rate: %u kbps\n",
				adslChannelOperData.actualBitrateFarFast);
	return 0;
}

int doAdslPerfdata(int argc, char *argv[], void *p)
{
	uint8 modemst;
    T_adsl_def_counter_set adsl_def_counter_set;
	unsigned int now = jiffies_to_msecs(jiffies);
	unsigned int updays, uphours, upminutes;	//andy chiu 2014/10/16, change the data struct type form 'int' to 'unsigned int'

	if (adsl_dev_ops == NULL) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst != ADSL_MODEM_STATE_UP) 
		return 0;

    adsl_dev_ops->query(ADSL_QUERY_DEFECT_CNT, &adsl_def_counter_set, NULL);

	printk("near-end FEC error fast: %u\n",
						adsl_def_counter_set.near_end.FecNotInterleaved);
	printk("near-end FEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.FecInterleaved);
	printk("near-end CRC error fast: %u\n",
						adsl_def_counter_set.near_end.CrcNotInterleaved);
	printk("near-end CRC error interleaved: %u\n",
						adsl_def_counter_set.near_end.CrcInterleaved);
	printk("near-end HEC error fast: %u\n",
						adsl_def_counter_set.near_end.HecNotInterleaved);
	printk("near-end HEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.HecInterleaved);
	printk("far-end FEC error fast: %u\n",
						adsl_def_counter_set.far_end.FecNotInterleaved);
	printk("far-end FEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.FecInterleaved);
	printk("far-end CRC error fast: %u\n",
						adsl_def_counter_set.far_end.CrcNotInterleaved);
	printk("far-end CRC error interleaved: %u\n",
						adsl_def_counter_set.far_end.CrcInterleaved);
	printk("far-end HEC error fast: %u\n",
						adsl_def_counter_set.far_end.HecNotInterleaved);
	printk("far-end HEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.HecInterleaved);

	now = now - adslUptime;
	now = now / MSEC_PER_SEC;

	printk("ADSL uptime : ");

	// >>> Andy Chiu 2014/10/16, modify the data structure type. Do NOT convert 'unsigned int' to 'int'.
	updays = now / (60*60*24);
	if (updays)
		printk("%u day%s, ", updays, (updays != 1) ? "s" : "");
	upminutes = now / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	if(uphours)
		printk("%2u:%02u, ", uphours, upminutes);
	else
		printk("%u min, ", upminutes);
	printk("%u secs\n", now % 60);
	// <<< End of Andy Chiu modify. 2014/10/16
	return 0;
}

int doAdslLinedata(int argc, char *argv[], void *p)
{
	return subcmd(adslLinedataCmds, argc, argv, p);
}

int doAdslLinedataFar(int argc, char *argv[], void *p)
{
	uint8 modemst;
	T_NTadslFarEndLineOperData adslFarEndLineOperData;
	uint8 link_mode;
	int i, j, line_num;

	if (adsl_dev_ops == NULL) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst != ADSL_MODEM_STATE_UP) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_FAR_OP_DATA, &adslFarEndLineOperData, NULL);    

	printk("relative capacity occupation: %u%% \n",
			adslFarEndLineOperData.relCapacityOccupationUpstr);
	printk("noise margin upstream: %d.%d db\n", 
			(adslFarEndLineOperData.noiseMarginUpstr/10),(adslFarEndLineOperData.noiseMarginUpstr%10)); //Jason_931028
	printk("output power downstream: %d.%d dbm\n", 
			(adslFarEndLineOperData.outputPowerDnstr/10),(adslFarEndLineOperData.outputPowerDnstr%10)); //Jason_931028
	printk("attenuation upstream: %d.%d db\n", 
			(adslFarEndLineOperData.attenuationUpstr/10),(adslFarEndLineOperData.attenuationUpstr%10));//Jason_931028

	printk("\nBit table:\n");
	adsl_dev_ops->query(ADSL_QUERY_MODE, &link_mode, NULL);//Jason_931130
	if( link_mode == ME_CMD_ADSL2PLUS )    		 //Jason_931130
		line_num = 16;
	else
		line_num = 8;
	printk("carrier load: number of bits per tone\n");
	for(i=0; i<line_num; i++) {	//Jason_931130
		printk("tone %3d-%3d: ",i*32, i*32+31);
		for (j=0; j<16; j++) {
			printk("%1x", adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF);
			printk("%1x ", (adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF0) >> 4);
		}
		printk("\n");
	}

	return 0;
}

int doAdslLinedataNear(int argc, char *argv[], void *p)
{
	uint8 modemst;
	T_NTadslNearEndLineOperData adslNearEndLineOperData;

	if (adsl_dev_ops == NULL) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst != ADSL_MODEM_STATE_UP) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_NEAR_OP_DATA, &adslNearEndLineOperData, NULL);    

	printk("relative capacity occupation: %u%% \n",
			adslNearEndLineOperData.relCapacityOccupationDnstr);
	printk("noise margin downstream: %d.%d db\n", 
			adslNearEndLineOperData.noiseMarginDnstr/10, adslNearEndLineOperData.noiseMarginDnstr%10);
	printk("output power upstream: %d.%01d dbm\n", 
			adslNearEndLineOperData.outputPowerUpstr/10, adslNearEndLineOperData.outputPowerUpstr%10);
	printk("attenuation downstream: %d.%01d db\n", 
			adslNearEndLineOperData.attenuationDnstr/10, adslNearEndLineOperData.attenuationDnstr%10);

	return 0;
}

int doAdslOpmode(int argc, char *argv[], void *p)
{
	uint8 modemst;
	uint8 mode;

	if (adsl_dev_ops == NULL) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst != ADSL_MODEM_STATE_UP) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_MODE, &mode, NULL);
	printk("Opmode: ");
	switch(mode) {
		case ME_CMD_ADSL_ANSI:
	    	printk("ANSI T1.413\n");
			break;
		case ME_CMD_ADSL_OPEN_GDMT:
	   		printk("ITU G.992.1(G.DMT)\n");
			break;
		case ME_CMD_ADSL_OPEN_GLITE:
       		printk("ITU G.992.2(G.Lite)\n");
			break;
		case ME_CMD_ADSL2:
       		printk("ITU G.992.3(ADSL2)\n");
			break;
		case ME_CMD_ADSL2PLUS:
       		printk("ITU G.992.5(ADSL2PLUS)\n");
		break;
	}
	return 0;
}

int doAdslDiag(int argc, char *argv[], void *p)
{
	if (adsl_dev_ops == NULL) 
		return 0;

	adsl_dev_ops->query(ADSL_QUERY_SHOW_DIAG, NULL, NULL);		   
	return 0;
}

int doAdslNearItuId(int argc, char *argv[], void *p) 
{
	int i;
	uint8 *r_vendor_id;

	if (adsl_dev_ops == NULL) 
		return 0;

    adsl_dev_ops->query(ADSL_QUERY_NEAR_ITUID, &r_vendor_id, NULL);
	
	printk("near end itu identification: ");
    for (i = 0; i < 8; i++) {
		printk("%x ", r_vendor_id[i]);
    }
    printk("\n");
	return 0;
}

int doAdslFarItuId(int argc, char *argv[], void *p) 
{
	int i;
	uint8 *c_vendor_id;

	if (adsl_dev_ops == NULL) 
		return 0;

    adsl_dev_ops->query(ADSL_QUERY_FAR_ITUID, &c_vendor_id, NULL);
	
	printk("far end itu identification: ");
    for (i = 0; i < 8; i++) {
		printk("%x ", c_vendor_id[i]);
    }
    printk("\n");
	return 0;
}

int doSys(int argc, char *argv[], void *p)
{
	return subcmd(sysCmds, argc, argv, p);
}

int doSysMemrl(int argc, char *argv[], void *p)
{
	unsigned long *ptr;

	ptr = (unsigned long *)simple_strtoul(argv[1], NULL, 16);
	printk("\r\n<Address>\t<Value>\r\n");
	printk("0x%08lx\t0x%08lx\r\n", ptr, *ptr);

	return 0;
}

int doSysMemwl(int argc, char *argv[], void *p)
{
	unsigned long *ptr;
	unsigned long value;

	ptr = (unsigned long *)simple_strtoul(argv[1], NULL, 16);
	value = (unsigned long)simple_strtoul(argv[2], NULL, 16);
	*ptr = value;

	return 0;
}

int doSysMemrw(int argc, char *argv[], void *p)
{
	unsigned short *ptr;

	ptr = (unsigned short *)simple_strtoul(argv[1], NULL, 16);
	printk("\r\n<Address>\t<Value>\r\n");
	printk("0x%08lx\t0x%04x\r\n", (unsigned long) ptr, (unsigned int) (*ptr));

	return 0;
}

int doSysMemww(int argc, char *argv[], void *p)
{
	unsigned short *ptr;
	unsigned short value;

	ptr = (unsigned short *)simple_strtoul(argv[1], NULL, 16);
	value = (unsigned short)simple_strtoul(argv[2], NULL, 16);
	*ptr = value;

	return 0;
}

static int dump(unsigned long addr, unsigned long len)
{
	register int n, m, c, r;
	unsigned char temp[16];
		
	for( n = len; n > 0; ){
			printk("%.8lx ", addr);
			r = n < 16? n: 16;
			memcpy((void *) temp, (void *) addr, r);
			addr += r;
			for( m = 0; m < r; ++m ){
					printk("%c", (m & 3) == 0 && m > 0? '.': ' ');
					printk("%.2x", temp[m]);
			}
			for(; m < 16; ++m )
					printk("   ");
			printk("  |");
			for( m = 0; m < r; ++m ){
				c = temp[m];
				printk("%c", ' ' <= c && c <= '~'? c: '.');
			}
			n -= r;
			for(; m < 16; ++m )
					printk(" ");
			printk("|\n");
	}
	
	return 0;
}

int doSysMemory(int argc, char *argv[], void *p)
{
	unsigned long addr, len;

	addr = simple_strtoul(argv[1], NULL, 16);
	len = simple_strtoul(argv[2], NULL, 16);
	dump(addr, len);

	return 0;
}

int doSysMemcpy(int argc, char *argv[], void *p)
{
	unsigned long src, dst, len;

	src = simple_strtoul(argv[1], NULL, 16);
	dst = simple_strtoul(argv[2], NULL, 16);
	len = simple_strtoul(argv[3], NULL, 16);
	memcpy((void *)dst, (void *)src, len);

	return 0;
}

int doSysFillMem(int argc, char *argv[], void *p)
{
	unsigned long addr, len, pattern;

	addr = simple_strtoul(argv[1], NULL, 16);
	len = simple_strtoul(argv[2], NULL, 16);
	pattern = simple_strtoul(argv[3], NULL, 16);
	memset((void *)addr, pattern, len);

	return 0;
}

int doTask(int argc, char *argv[], void *p)
{
	return subcmd(taskCmds, argc, argv, p);
}

int doTaskDisp(int argc, char *argv[], void *p)
{
	show_state();
	return 0;
}

/************************************************************************
*		     A D S L   E V E N T   C A L L   F U N C T I O N  
*************************************************************************
*/

#define DSL_EVENT_UP			0x1
#define DSL_EVENT_DOWN			0x2
#define DSL_EVENT_TRAIN			0x3

uint32 dslEventNotify ( uint32 event, uint32 param1, uint32 param2)
{
#ifndef CONFIG_MIPS_TC3262
	switch (event) {
		case DSL_EVENT_DOWN:
			break;
		case DSL_EVENT_UP:
			break;
		case DSL_EVENT_TRAIN:
			break;
	}
#endif
	return 0;
}
EXPORT_SYMBOL(dslEventNotify);

/************************************************************************
*		     A D S L   P R O C  D E F I N I T I O N S
*************************************************************************
*/

extern struct proc_dir_entry *proc_tc3162;

int
getADSLState(char *stateADSL, uint8 long_fmt)
{
	uint16	index=0;

	uint8 modemst;
	T_adslChannelOperData adslChannelOperData;
    T_adsl_def_counter_set adsl_def_counter_set;
	T_NTadslNearEndLineOperData adslNearEndLineOperData;
	T_NTadslFarEndLineOperData adslFarEndLineOperData;
	uint8 link_mode;
	uint8 mode;
	int i, j, line_num;

	if (adsl_dev_ops == NULL) {
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
		return index;
	}

	adsl_dev_ops->query(ADSL_QUERY_STATUS, &modemst, NULL );		   
	if (modemst == ADSL_MODEM_STATE_DOWN) 
		index += sprintf( stateADSL+index, "ADSL link status: down\n");
	else if (modemst == ADSL_MODEM_STATE_WAIT_INIT) 
		index += sprintf( stateADSL+index, "ADSL link status: wait for init\n");
	else if (modemst == ADSL_MODEM_STATE_INIT) 
		index += sprintf( stateADSL+index, "ADSL link status: initializing\n");
	else if (modemst == ADSL_MODEM_STATE_UP) 
		index += sprintf( stateADSL+index, "ADSL link status: up\n");
	else 
		index += sprintf( stateADSL+index, "ADSL link status: unknown\n");

	if (modemst != ADSL_MODEM_STATE_UP) {
		return index;
	}

	adsl_dev_ops->query(ADSL_QUERY_CH_OP_DATA, &adslChannelOperData, NULL); 		 
	adsl_dev_ops->query(ADSL_QUERY_MODE, &mode, NULL);
    adsl_dev_ops->query(ADSL_QUERY_DEFECT_CNT, &adsl_def_counter_set, NULL);
	adsl_dev_ops->query(ADSL_QUERY_NEAR_OP_DATA, &adslNearEndLineOperData, NULL);    
	adsl_dev_ops->query(ADSL_QUERY_FAR_OP_DATA, &adslFarEndLineOperData, NULL);    

	index += sprintf(stateADSL+index, "\nOpmode: ");
	switch(mode) {
		case ME_CMD_ADSL_ANSI:
	    	index += sprintf(stateADSL+index, "ANSI T1.413\n");
			break;
		case ME_CMD_ADSL_OPEN_GDMT:
	   		index += sprintf(stateADSL+index, "ITU G.992.1(G.DMT)\n");
			break;
		case ME_CMD_ADSL_OPEN_GLITE:
       		index += sprintf(stateADSL+index, "ITU G.992.2(G.Lite)\n");
			break;
		case ME_CMD_ADSL2:
       		index += sprintf(stateADSL+index, "ITU G.992.3(ADSL2)\n");
			break;
		case ME_CMD_ADSL2PLUS:
       		index += sprintf(stateADSL+index, "ITU G.992.5(ADSL2PLUS)\n");
		break;
	}

	index += sprintf( stateADSL+index, "\n");
	index += sprintf( stateADSL+index, "near-end interleaved channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateNearInt);
	index += sprintf( stateADSL+index, "near-end fast channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateNearFast);
	index += sprintf( stateADSL+index, "far-end interleaved channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateFarInt);
	index += sprintf( stateADSL+index, "far-end fast channel bit rate: %u kbps\n",
									adslChannelOperData.actualBitrateFarFast);

	index += sprintf( stateADSL+index, "\n");
	index += sprintf( stateADSL+index, "near-end FEC error fast: %u\n",
						adsl_def_counter_set.near_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "near-end FEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.FecInterleaved);
	index += sprintf( stateADSL+index, "near-end CRC error fast: %u\n",
						adsl_def_counter_set.near_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "near-end CRC error interleaved: %u\n",
						adsl_def_counter_set.near_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "near-end HEC error fast: %u\n",
						adsl_def_counter_set.near_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "near-end HEC error interleaved: %u\n",
						adsl_def_counter_set.near_end.HecInterleaved);
	index += sprintf( stateADSL+index, "far-end FEC error fast: %u\n",
						adsl_def_counter_set.far_end.FecNotInterleaved);
	index += sprintf( stateADSL+index, "far-end FEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.FecInterleaved);
	index += sprintf( stateADSL+index, "far-end CRC error fast: %u\n",
						adsl_def_counter_set.far_end.CrcNotInterleaved);
	index += sprintf( stateADSL+index, "far-end CRC error interleaved: %u\n",
						adsl_def_counter_set.far_end.CrcInterleaved);
	index += sprintf( stateADSL+index, "far-end HEC error fast: %u\n",
						adsl_def_counter_set.far_end.HecNotInterleaved);
	index += sprintf( stateADSL+index, "far-end HEC error interleaved: %u\n",
						adsl_def_counter_set.far_end.HecInterleaved);
	
	if (!long_fmt)
		return index;

	index += sprintf( stateADSL+index, "\nDownstream:\n");
	index += sprintf( stateADSL+index, "relative capacity occupation: %u%% \n",
			adslNearEndLineOperData.relCapacityOccupationDnstr);
	index += sprintf( stateADSL+index, "noise margin downstream: %d.%d db\n", 
			adslNearEndLineOperData.noiseMarginDnstr/10, adslNearEndLineOperData.noiseMarginDnstr%10);
	index += sprintf( stateADSL+index, "output power upstream: %d.%01d dbm\n", 
			adslNearEndLineOperData.outputPowerUpstr/10, adslNearEndLineOperData.outputPowerUpstr%10);
	index += sprintf( stateADSL+index, "attenuation downstream: %d.%01d db\n", 
			adslNearEndLineOperData.attenuationDnstr/10, adslNearEndLineOperData.attenuationDnstr%10);

	index += sprintf( stateADSL+index, "\nUpstream:\n");
	index += sprintf( stateADSL+index , "relative capacity occupation: %u%% \n",
			adslFarEndLineOperData.relCapacityOccupationUpstr);
	index += sprintf( stateADSL+index, "noise margin upstream: %d.%d db\n", 
			(adslFarEndLineOperData.noiseMarginUpstr/10),(adslFarEndLineOperData.noiseMarginUpstr%10)); //Jason_931028
	index += sprintf( stateADSL+index, "output power downstream: %d.%d dbm\n", 
			(adslFarEndLineOperData.outputPowerDnstr/10),(adslFarEndLineOperData.outputPowerDnstr%10)); //Jason_931028
	index += sprintf( stateADSL+index, "attenuation upstream: %d.%d db\n", 
			(adslFarEndLineOperData.attenuationUpstr/10),(adslFarEndLineOperData.attenuationUpstr%10));//Jason_931028

	index += sprintf( stateADSL+index, "\nBit table:\n");
	adsl_dev_ops->query(ADSL_QUERY_MODE, &link_mode, NULL);//Jason_931130
	if( link_mode == ME_CMD_ADSL2PLUS )    		 //Jason_931130
		line_num = 16;
	else
		line_num = 8;
	index += sprintf( stateADSL+index, "carrier load: number of bits per tone\n");
	for(i=0; i<line_num; i++) {	//Jason_931130
		index += sprintf( stateADSL+index, "tone %3d-%3d: ",i*32, i*32+31);
		for (j=0; j<16; j++) {
			index += sprintf( stateADSL+index, "%1x", adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF);
			index += sprintf( stateADSL+index, "%1x ", (adslFarEndLineOperData.carrierLoad[i*16+j] & 0xF0) >> 4);
		}
		index += sprintf( stateADSL+index, "\n");
	}

	return index;
}

int adsl_proc_stats(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLState(buf, 1);
	if (len <= off+count) 
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}

int adsl_proc_perf(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLState(buf, 0);
	if (len <= off+count) 
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}

int adsl_proc_fwver(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int len = getADSLFwVer(buf);
	if (len <= off+count) 
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}

static int adsl_cmd_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "0\n");

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

DEFINE_MUTEX(adsl_mutex);

static int adsl_cmd_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[80];

	mutex_lock(&adsl_mutex);
	if (count > sizeof(val_string) - 1) {
		mutex_unlock(&adsl_mutex);
		return -EINVAL;
	}

	if (copy_from_user(val_string, buffer, count)) {
		mutex_unlock(&adsl_mutex);
		return -EFAULT;
	}

	val_string[count] = '\0';
	cmdparse(Cmds, val_string, NULL);
	mutex_unlock(&adsl_mutex);

	return count;
}

static int __init tc3162_adsl_init(void)
{
	struct proc_dir_entry *adsl_cmd_proc;
	cmds_t wan_cmd, sys_cmd;
	
	/* start adsl timer */
	init_timer(&adsl_timer);
	adsl_timer.expires = jiffies + (HZ * 2);
	adsl_timer.function = adslTimer;
	adsl_timer.data = 0x0;
	add_timer(&adsl_timer);

	/* adsl related stats */
	create_proc_read_entry("tc3162/adsl_stats", 0, NULL, adsl_proc_stats, NULL);
	/* adsl performance related stats */
	create_proc_read_entry("tc3162/adsl_perf", 0, NULL, adsl_proc_perf, NULL);
	/* adsl firmware version */
	create_proc_read_entry("tc3162/adsl_fwver", 0, NULL, adsl_proc_fwver, NULL);

	/* adsl commands */
	adsl_cmd_proc = create_proc_entry("tc3162/adsl_cmd", 0, NULL);
	adsl_cmd_proc->read_proc = adsl_cmd_read_proc;
	adsl_cmd_proc->write_proc = adsl_cmd_write_proc;

	/*Init cmd*/
	memset(Cmds, 0, (sizeof(cmds_t)*MAX_ROOT_CMD));
	
	/*Init wan  root ci-cmd*/
	wan_cmd.name= "wan";
	wan_cmd.func=doWan;
	wan_cmd.flags=0x12;
	wan_cmd.argcmin=0;
	wan_cmd.argc_errmsg=NULL;

	/*Init sys root ci-cmd*/
	sys_cmd.name= "sys";
	sys_cmd.func=doSys;
	sys_cmd.flags=0x12;
	sys_cmd.argcmin=0;
	sys_cmd.argc_errmsg=NULL;
	
	/*Register wan & sys ci-cmd*/
	cmd_register(&wan_cmd);
	cmd_register(&sys_cmd);
	
	return 0;
}

static void __exit tc3162_adsl_exit(void)
{
	remove_proc_entry("tc3162/adsl_stats", NULL);
	remove_proc_entry("tc3162/adsl_perf", NULL);
	remove_proc_entry("tc3162/adsl_fwver", NULL);
	remove_proc_entry("tc3162/adsl_cmd", NULL);
}

EXPORT_SYMBOL(getXdslSpeed);

module_init (tc3162_adsl_init);
module_exit (tc3162_adsl_exit);

#else

int 
getXdslSpeed(void)
{
	return 0;
}

EXPORT_SYMBOL(getXdslSpeed);

#endif

adsldev_ops *adsl_dev_ops = NULL;

void adsl_dev_ops_register(adsldev_ops *ops)
{
	adsl_dev_ops = ops;

#ifdef CONFIG_TC3162_ADSL
	wanCmds[1].name = "rts";  	wanCmds[1].func = ops->rts_cmd;
	wanCmds[2].name = "dmt";  	wanCmds[2].func = ops->dmt_cmd;
	wanCmds[3].name = "dmt2";  	wanCmds[3].func = ops->dmt2_cmd;
	wanCmds[4].name = "hwdmt"; 	wanCmds[4].func = ops->hw_cmd;
	wanCmds[5].name = "swdmt"; 	wanCmds[5].func = ops->sw_cmd;
	wanCmds[6].name = "ghs";  	wanCmds[6].func = ops->ghs_cmd;
	wanCmds[7].name = "tcif";  	wanCmds[7].func = ops->tcif_cmd;
#endif
}

void adsl_dev_ops_deregister(void)
{
	adsl_dev_ops = NULL;

#ifdef CONFIG_TC3162_ADSL
	wanCmds[1].name = NULL;  	wanCmds[1].func = NULL;
	wanCmds[2].name = NULL;  	wanCmds[2].func = NULL;
	wanCmds[3].name = NULL;  	wanCmds[3].func = NULL;
	wanCmds[4].name = NULL; 	wanCmds[4].func = NULL;
	wanCmds[5].name = NULL; 	wanCmds[5].func = NULL;
	wanCmds[6].name = NULL;  	wanCmds[6].func = NULL;
	wanCmds[7].name = NULL;  	wanCmds[7].func = NULL;
#endif
}

EXPORT_SYMBOL(adsl_dev_ops);
EXPORT_SYMBOL(adsl_dev_ops_register);
EXPORT_SYMBOL(adsl_dev_ops_deregister);

