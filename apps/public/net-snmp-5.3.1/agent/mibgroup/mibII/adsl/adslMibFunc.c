#include <stddef.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/auto_nlist.h>

#include "../interfaces.h"
#include "adslMibFunc.h"

static int adslMibCurrAdminStatus=IF_ADMIN_STATUS_UP;

int
getAdslIfIndex(void)
{
	return ADSL_IF_INDEX;
}

/* code copied from adslctl.c */
unsigned long
getAdslIfSpeed(void)
{
    return 0;
}

int
getAdslOperStatus(void)
{
    return 0;
}

int
getAdslAdminStatus(void)
{
  return (adslMibCurrAdminStatus);
}

int
setAdslAdminStatus(int status)
{
  int ret=0;

  return (ret);
}


int
adslMibHelper_getIfCount()
{
    return (1);
}

unsigned long getAR5211AdslInfo(char *attr_total, char *attr_curr)
{
    	char tmp[32] = {0};
    	char status[8] = {0};
    	char *end;
    	unsigned long total= 0;

	/* 0. get adsl status */
	tcapi_get(INFOADSL_NODE, ADSL_STATUS_ATTR, status);

	/* 1. get total Value */
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOTOTAL_NODE, attr_total, tmp);
	total  = strtoul(tmp, &end, 0);
	//tcdbg_printf("\n==>fillctAR5211TxInfoTable, 11 %s = %lu, %s\n", attr_total, total, tmp);

	if(0 == strcmp(status, ADSL_STATUS_UP)) {
		memset(tmp, 0, sizeof(tmp));
		tcapi_get(INFOSHOW_NODE, attr_curr, tmp);
		total += strtoul(tmp, &end, 0);
		//tcdbg_printf("\n==>fillctAR5211TxInfoTable, 22 %s = %lu, %s\n", attr_total, total, tmp);
	}

	return total;
}

#ifdef DO_CTAR5211TXINFOTABLE
//
 //Fill out the ADSL ctAR5211TxInfoTable
 //
void
adslMibHelper_fillctAR5211TxInfoTable(pADSL_MIB_CTAR5211TXINFOTABLE
                                        pTable)
{
    	pADSL_MIB_CTAR5211TXINFOTABLE p = pTable;

    	p->pTableBeginning = pTable;
    	p->ifIndex = getAdslIfIndex();

    	/*Init the table value*/
    	p->ctTxTotalES = 0;
    	p->ctTxTotalSES = 0;
    	p->ctTxTotalUAS = 0;
	/*
    	p->ctTx15MinES = 0;
    	p->ctTx15MinSES = 0;
    	p->ctTx15MinUAS = 0;
    	p->ctTx1DayES = 0;
    	p->ctTx1DaySES = 0;
    	p->ctTx1DayUAS = 0;   */

	/* 1. get total ES */
	p->ctTxTotalES  = getAR5211AdslInfo(TOTAL_AR5211_ES_UP, AR5211_ES_UP);

	/* 2. get total SES */
	p->ctTxTotalSES  = getAR5211AdslInfo(TOTAL_AR5211_SES_UP, AR5211_SES_UP);

	/* 3. get total UAS */
	p->ctTxTotalUAS  = getAR5211AdslInfo(TOTAL_AR5211_UAS_UP, AR5211_UAS_UP);

	p->next = NULL;
}
#endif                          //DO_CTAR5211TXINFOTABLE //

#ifdef DO_CTAR5211RXINFOTABLE
//
 //Fill out the ADSL ctAR5211RxInfoTable
 //
void
adslMibHelper_fillctAR5211RxInfoTable(pADSL_MIB_CTAR5211RXINFOTABLE
                                        pTable)
{
    	pADSL_MIB_CTAR5211RXINFOTABLE p = pTable;

    	p->pTableBeginning = pTable;
    	p->ifIndex = getAdslIfIndex();
	
    	/*Init the table value*/
    	p->ctRxTotalES = 0;
    	p->ctRxTotalSES = 0;
    	p->ctRxTotalUAS = 0;
	/*
    	p->ctRx15MinES = 0;
    	p->ctRx15MinSES = 0;
    	p->ctRx15MinUAS = 0;
    	p->ctRx1DayES = 0;
    	p->ctRx1DaySES = 0;
    	p->ctRx1DayUAS = 0;   */

	/* 1. get total ES */
	p->ctRxTotalES  = getAR5211AdslInfo(TOTAL_AR5211_ES_DOWN, AR5211_ES_DOWN);

	/* 2. get total SES */
	p->ctRxTotalSES  = getAR5211AdslInfo(TOTAL_AR5211_SES_DOWN, AR5211_SES_DOWN);

	/* 3. get total UAS */
	p->ctRxTotalUAS  = getAR5211AdslInfo(TOTAL_AR5211_UAS_DOWN, AR5211_UAS_DOWN);

	p->next = NULL;
}
#endif 

#ifdef DO_CTAR5211BITERRORTABLE
//
 //Fill out the ADSL ctAR5211BitErrorTable
 //
void
adslMibHelper_fillctAR5211BitErrorTable(pADSL_MIB_CTAR5211BITERRORTABLE
                                        pTable)
{
    	char tmp[32] = {0};
    	char *end;

    	pADSL_MIB_CTAR5211BITERRORTABLE p = pTable;

   	p->pTableBeginning = pTable;
    	p->ifIndex = getAdslIfIndex();

    	/*Init the table value*/
    	p->ctTxBiterror = 0;
    	p->ctRxBiterror = 0;

	/* 0. get TX Bit Error Count */
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, AR5211_TX_BIRERR_CNT, tmp);
	p->ctTxBiterror = strtoul(tmp, &end, 0);

	/* 1. get RX Bit Error Count */
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, AR5211_RX_BIRERR_CNT, tmp);
	p->ctRxBiterror = strtoul(tmp, &end, 0);

    	p->next = NULL;
}
#endif                          //DO_CTAR5211BITERRORTABLE //


/** free context of adslAturPhysTable table */
#ifdef DO_ADSLATURPHYSTABLE
/*
 * Fill out the ADSL adslAturPhysTable 
 */
void
adslMibHelper_filladslAturPhysTable(pADSL_MIB_ADSLATURPHYSTABLE pTable)
{

}
#endif                          /* DO_ADSLATURPHYSTABLE */
/** free context of adslLineConfProfileTable table */
#ifdef DO_ADSLLINECONFPROFILETABLE
/*
 * Fill out the ADSL adslLineConfProfileTable 
 */
void
adslMibHelper_filladslLineConfProfileTable
    (pADSL_MIB_ADSLLINECONFPROFILETABLE pTable)
{

}
#endif                          /* DO_ADSLLINECONFPROFILETABLE */
/** free context of adslAturChanTable table */
#ifdef DO_ADSLATURCHANTABLE
/*
 * Fill out the ADSL adslAturChanTable 
 */
void
adslMibHelper_filladslAturChanTable(pADSL_MIB_ADSLATURCHANTABLE pTable)
{
    	pADSL_MIB_ADSLATURCHANTABLE p = pTable;
    	char tmp[32] = {0};
	char *end;

    	p->pTableBeginning = pTable;
    	p->ifIndex = getAdslIfIndex();

    	/*Init the table value*/
    	p->adslAturChanInterleaveDelay = 0;
    	p->adslAturChanCurrTxRate = 0;
    	p->adslAturChanPrevTxRate = 0;
    	p->adslAturChanCrcBlockLength= 0;

	tcapi_get(INFOADSL_NODE, ADSL_STATUS_ATTR, tmp);
	if(0 == strcmp(tmp, ADSL_STATUS_UP)) {
		memset(tmp, 0, sizeof(tmp));
		tcapi_get(INFOADSL_NODE, ATUR_CHAN_ITL_DELAY, tmp);
		p->adslAturChanInterleaveDelay  = strtoul(tmp, &end, 0);

		memset(tmp, 0, sizeof(tmp));
		tcapi_get(INFOADSL_NODE, ATUR_CHAN_PREV_TXRATE, tmp);
		p->adslAturChanPrevTxRate = strtoul(tmp, &end, 0);

		memset(tmp, 0, sizeof(tmp));
		tcapi_get(INFOADSL_NODE, ATUR_CHAN_CURR_TXRATE, tmp);
		p->adslAturChanCurrTxRate  = strtoul(tmp, &end, 0);

		memset(tmp, 0, sizeof(tmp));
		tcapi_get(INFOADSL_NODE, ATUR_CHAN_CRC_BLK_LEN, tmp);
		p->adslAturChanCrcBlockLength  = strtoul(tmp, &end, 0);	
	}

    	p->next = NULL;
}
#endif                          /* DO_ADSLATURCHANTABLE */
/** free context of adslAtucPerfDataTable table */
#ifdef DO_ADSLATUCPERFDATATABLE
/*
 * Fill out the ADSL adslAtucPerfDataTable 
 */
void
adslMibHelper_filladslAtucPerfDataTable(pADSL_MIB_ADSLATUCPERFDATATABLE
                                        pTable)
{

}
#endif                          /* DO_ADSLATUCPERFDATATABLE */
/** free context of adslAtucIntervalTable table */
#ifdef DO_ADSLATUCINTERVALTABLE
/*
 * Fill out the ADSL adslAtucIntervalTable 
 */
void
adslMibHelper_filladslAtucIntervalTable(pADSL_MIB_ADSLATUCINTERVALTABLE
                                        pTable)
{

}
#endif                          /* DO_ADSLATUCINTERVALTABLE */
/** free context of adslAturPerfDataTable table */
#ifdef DO_ADSLATURPERFDATATABLE
/*
 * Fill out the ADSL adslAturPerfDataTable 
 */
void
adslMibHelper_filladslAturPerfDataTable(pADSL_MIB_ADSLATURPERFDATATABLE
                                        pTable)
{

}
#endif                          /* DO_ADSLATURPERFDATATABLE */
/** free context of adslLineAlarmConfProfileTable table */
#ifdef DO_ADSLLINEALARMCONFPROFILETABLE
/*
 * Fill out the ADSL adslLineAlarmConfProfileTable 
 */
void
adslMibHelper_filladslLineAlarmConfProfileTable
    (pADSL_MIB_ADSLLINEALARMCONFPROFILETABLE pTable)
{

}
#endif                          /* DO_ADSLLINEALARMCONFPROFILETABLE */
/** free context of adslAturChanPerfDataTable table */
#ifdef DO_ADSLATURCHANPERFDATATABLE
/*
 * Fill out the ADSL adslAturChanPerfDataTable 
 */
void
adslMibHelper_filladslAturChanPerfDataTable
    (pADSL_MIB_ADSLATURCHANPERFDATATABLE pTable)
{
	
    	pADSL_MIB_ADSLATURCHANPERFDATATABLE p = pTable;
    	char tmp[32] = {0};
    	char status[8] = {0};
    	char *end;
    	unsigned long total= 0, qutHourSt = 0, interval = 0;

    	p->pTableBeginning = pTable;
	p->ifIndex = getAdslIfIndex();

    	/*Init the table value*/
    	memcpy((void *)&p->adslAturChanReceivedBlks,"0", 1);
    	p->adslAturChanPerfValidIntervals = 0;
    	p->adslAturChanPerfInvalidIntervals = 0;
    	p->adslAturChanPerfCurr15MinTimeElapsed = 0;
    	memcpy((void *)&p->adslAturChanPerfCurr15MinReceivedBlks,"0",	1);
    	p->adslAturChanPerfCurr1DayTimeElapsed = 0;
    	memcpy((void *)&p->adslAturChanPerfCurr1DayReceivedBlks,"0",	1);
    	p->adslAturChanPerfPrev1DayMoniSecs = 0;
    	memcpy((void *)&p->adslAturChanPerfPrev1DayReceivedBlks,"0",	1);
	
	/* 0. get adsl status */
	tcapi_get(INFOADSL_NODE, ADSL_STATUS_ATTR, status);

	/* 1. get total rx blocks */
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOTOTAL_NODE, TOTAL_ATUR_CHAN_RXBLKS, tmp);
	total  = strtoul(tmp, &end, 0);
	
	/* 2. get current interval rx blocks */
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, ATUR_CHAN_RXBLKS, tmp);
	interval  = strtoul(tmp, &end, 0);
	
	//adslAturChanReceivedBlks
	p->adslAturChanReceivedBlks  = total;
	if(0 == strcmp(status, ADSL_STATUS_UP)) {
		p->adslAturChanReceivedBlks += interval;
	}						

	//adslAturChanPerfValidIntervals
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, ATUR_CHAN_PERF_VALID_ITLS, tmp);
	p->adslAturChanPerfValidIntervals  = strtoul(tmp, &end, 0);

	//ATURChanPerfInValidIntervals
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, ATUR_CHAN_PERF_INVALID_ITLS, tmp);
	p->adslAturChanPerfInvalidIntervals  = strtoul(tmp, &end, 0);

	//adslAturChanPerfCurr15MinTimeElapsed
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, QUT_HRS_ATUR_CHAN_TIMEELAPSED, tmp);
	p->adslAturChanPerfCurr15MinTimeElapsed  = strtoul(tmp, &end, 0);

	//adslAturChanPerfCurr1DayTimeElapsed
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, CURDAY_TIMEELAPSED, tmp);
	p->adslAturChanPerfCurr1DayTimeElapsed  = strtoul(tmp, &end, 0);

	//adslAturChanPerfPrev1DayMoniSecs
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOSHOW_NODE, PREVDAY_ATUR_CHAN_MONISECS, tmp);
	p->adslAturChanPerfPrev1DayMoniSecs  = strtoul(tmp, &end, 0);

	//adslAturChanPerfCurr15MinReceivedBlks
	p->adslAturChanPerfCurr15MinReceivedBlks  = total;
	
	if(0 == strcmp(status, ADSL_STATUS_UP)) {
		p->adslAturChanPerfCurr15MinReceivedBlks += interval;
	}

	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOQUTHOURST_NODE, QUTHRS_ATUR_CHAN_RXBLKS, tmp);
	p->adslAturChanPerfCurr15MinReceivedBlks -= strtoul(tmp, &end, 0);	

	//adslAturChanPerfCurr1DayReceivedBlks
	p->adslAturChanPerfCurr1DayReceivedBlks  = total;

	if(0 == strcmp(status, ADSL_STATUS_UP)) {
		p->adslAturChanPerfCurr1DayReceivedBlks += interval;
	}

	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOCURDAYST_NODE, CURDAY_ATUR_CHAN_RXBLKS, tmp);
	p->adslAturChanPerfCurr1DayReceivedBlks -= strtoul(tmp, &end, 0);	

	//adslAturChanPerfPrev1DayReceivedBlks
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(INFOCURDAYST_NODE, PREVDAY_ATUR_CHAN_RXBLKS, tmp);
	p->adslAturChanPerfPrev1DayReceivedBlks -= strtoul(tmp, &end, 0);	

    	p->next = NULL;   
}
#endif                          /* DO_ADSLATURCHANPERFDATATABLE */
/** free context of adslAturIntervalTable table */
#ifdef DO_ADSLATURINTERVALTABLE
/*
 * Fill out the ADSL adslAturIntervalTable 
 */
void
adslMibHelper_filladslAturIntervalTable(pADSL_MIB_ADSLATURINTERVALTABLE
                                        pTable)
{

}
#endif                          /* DO_ADSLATURINTERVALTABLE */
/** free context of adslAtucPhysTable table */
#ifdef DO_ADSLATUCPHYSTABLE
/*
 * Fill out the ADSL adslAtucPhysTable 
 */
void
adslMibHelper_filladslAtucPhysTable(pADSL_MIB_ADSLATUCPHYSTABLE pTable)
{

}
#endif                          /* DO_ADSLATUCPHYSTABLE */
/** free context of adslAtucChanPerfDataTable table */
#ifdef DO_ADSLATUCCHANPERFDATATABLE
/*
 * Fill out the ADSL adslAtucChanPerfDataTable 
 */
void
adslMibHelper_filladslAtucChanPerfDataTable
    (pADSL_MIB_ADSLATUCCHANPERFDATATABLE pTable)
{

}
#endif                          /* DO_ADSLATUCCHANPERFDATATABLE */
/** free context of adslAturChanIntervalTable table */
#ifdef DO_ADSLATURCHANINTERVALTABLE
/*
 * Fill out the ADSL adslAturChanIntervalTable 
 */
void
adslMibHelper_filladslAturChanIntervalTable
    (pADSL_MIB_ADSLATURCHANINTERVALTABLE pTable)
{

    	int numOfInterval, i;
    	int index;
    	int validInterval;
    	char tmp[32] = {0};
	char prev[32] = {0};
	unsigned long intervalValue = 0;
	char attrname[48] = {0};
	char *end;

    	pADSL_MIB_ADSLATURCHANINTERVALTABLE p = pTable;

    	index = getAdslIfIndex();

	/* the max interval we support */
    	numOfInterval = ADSLMIB_CHANPERFINTERVAL_MAXNUM;

	tcapi_get(INFOSHOW_NODE, ATUR_CHAN_PERF_VALID_ITLS, tmp);
	validInterval = atoi(tmp);

    	for (i=0; i<numOfInterval; i++) {
      		p->pTableBeginning = pTable;
      		p->ifIndex = index;
      		p->adslAturChanIntervalNumber = i+1;
      		if (validInterval > i)
      		{
      			memset(attrname, 0, sizeof(attrname));
			sprintf(attrname, ATURCHANRXBLKSITL_ATTR, i);
      			tcapi_get(INFOINTERVRLST_NODE, attrname, prev);
		     	memset(attrname, 0, sizeof(attrname));
			sprintf(attrname, ATURCHANRXBLKSITL_ATTR, i+1);
      			tcapi_get(INFOINTERVRLST_NODE, attrname, tmp);
	
			intervalValue = strtoul(tmp, &end, 0) - strtoul(prev, &end, 0);
			if(intervalValue > 0)
				p->adslAturChanIntervalReceivedBlks  = intervalValue;
			else
				p->adslAturChanIntervalReceivedBlks  = 0;
			p->adslAturChanIntervalValidData = 1;
      		}
      		else
			p->adslAturChanIntervalValidData = 0;
      		if (i == (numOfInterval-1))
			p->next=NULL;
      		else {
			p->next = (p+1);
			p++;
      		}
    	}

    	p->next = NULL;   
}
#endif                          /* DO_ADSLATURCHANINTERVALTABLE */
/** free context of adslAtucChanIntervalTable table */
#ifdef DO_ADSLATUCCHANINTERVALTABLE
/*
 * Fill out the ADSL adslAtucChanIntervalTable 
 */
void
adslMibHelper_filladslAtucChanIntervalTable
    (pADSL_MIB_ADSLATUCCHANINTERVALTABLE pTable)
{
}
#endif                          /* DO_ADSLATUCCHANINTERVALTABLE */
/** free context of adslLineTable table */
#ifdef DO_ADSLLINETABLE
/*
 * Fill out the ADSL adslLineTable 
 */
void
adslMibHelper_filladslLineTable(pADSL_MIB_ADSLLINETABLE pTable)
{

}
#endif                          /* DO_ADSLLINETABLE */
/** free context of adslAtucChanTable table */
#ifdef DO_ADSLATUCCHANTABLE
/*
 * Fill out the ADSL adslAtucChanTable 
 */
void
adslMibHelper_filladslAtucChanTable(pADSL_MIB_ADSLATUCCHANTABLE pTable)
{

}
#endif                          /* DO_ADSLATUCCHANTABLE */

