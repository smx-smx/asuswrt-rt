#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "trendchip-def.h"
#include "gb-constants.h"
#include "trendchip_load.h"

typedef struct _T_STATUS_STATISTICS_ETH_INFO{
char inOctets              [16];
char inUnicastPkts         [16];
char inMulticastPkts       [16];
char ReceiveFrames		   [16];
char outOctets             [16];
char outUnicastPkts        [16];
char outMulticastPkts      [16];
char TransmitFrames		   [16];
char outErrors             [16];
char txCollisionCnt        [16];
char txUnderRunCnt         [16];
char rxCrcErr              [16];
}T_STATUS_STATISTICS_ETH_INFO;
T_STATUS_STATISTICS_ETH_INFO status_statistics_eth_info;

typedef struct _T_STATUS_STATISTICS_ADSL_INFO{
char inPkts                [16];
char inDiscards            [16];
char outPkts       	       [16];
char outDiscards 	         [16];
}T_STATUS_STATISTICS_ADSL_INFO;
T_STATUS_STATISTICS_ADSL_INFO status_statistics_adsl_info;

typedef struct _T_STATUS_STATISTICS_WLAN_INFO{
char wlanTxFrames		 [16];
char wlanTxErrFrames	 [16];
char wlanTxDropFrames	 [16];
char wlanRxFrames	 	 [16];
char wlanRxErrFrames	 [16];
char wlanRxDropFrames	 [16];
}T_STATUS_STATISTICS_WLAN_INFO;
T_STATUS_STATISTICS_WLAN_INFO status_statistics_wlan_info;

void StatusStatistics_eth_init()
{
	char *buf = NULL;
	int  fd=0, ret=0;	
	unsigned long int var_1 =0;
	unsigned long int var_2=0;
	
	memset(&status_statistics_eth_info,0,sizeof(T_STATUS_STATISTICS_ETH_INFO));	
	
	buf = (char *)ASP_MALLOC(1024*4);
	if (NULL == buf)
			return;
	
	fd = open("/proc/tc3162/eth_stats",O_RDONLY);
	if (fd == -1)
	goto err;
	
	ret = read(fd,buf,1024*4-1);
	if (ret<=0)
	goto err;
	
	*(buf+ret)=0;
	
#if 0	
	pch_itemname = strstr(buf,"outUnicastPkts");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x1 = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.outUnicastPkts,"%ld",x1);	
		}
	}
	else
		sprintf(status_statistics_eth_info.outUnicastPkts,"0");	
		
	pch_itemname = strstr(buf,"outMulticastPkts");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x2 = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.outMulticastPkts,"%ld",x2);	
		}
	}
	else
		sprintf(status_statistics_eth_info.outMulticastPkts,"0");		
	
	x = x1+x2;
	sprintf(status_statistics_eth_info.TransmitFrames,"%ld",x);
	
	pch_itemname = strstr(buf,"inUnicastPkts");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x1 = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.inUnicastPkts,"%ld",x1);	
		}
	}
	else
		sprintf(status_statistics_eth_info.inUnicastPkts,"0");	
	
	pch_itemname = strstr(buf,"inMulticastPkts");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x2 = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.inMulticastPkts,"%ld",x2);	
		}
	}
	else
		sprintf(status_statistics_eth_info.inMulticastPkts,"0");	
	
		x = x1+x2;
	sprintf(status_statistics_eth_info.ReceiveFrames,"%ld",x);
	
	
	
	pch_itemname = strstr(buf,"outOctets");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.outOctets,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_eth_info.outOctets,"0");	
	
	pch_itemname = strstr(buf,"inOctets");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.inOctets,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_eth_info.inOctets,"0");
	
	pch_itemname = strstr(buf,"txCollisionCnt");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.txCollisionCnt,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_eth_info.txCollisionCnt,"0");
	
	pch_itemname = strstr(buf,"rxCrcErr");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.rxCrcErr,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_eth_info.rxCrcErr,"0");
	
	pch_itemname = strstr(buf,"outErrors");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.outErrors,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_eth_info.outErrors,"0");
	
	pch_itemname = strstr(buf,"txUnderRunCnt");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_eth_info.txUnderRunCnt,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_eth_info.txUnderRunCnt,"0");
#endif 
	
	var_1 = get_key_vlaue(buf,"outUnicastPkts",16);
	sprintf(status_statistics_eth_info.outUnicastPkts,"%lu", var_1);
	var_2 = get_key_vlaue(buf,"outMulticastPkts",16);
	sprintf(status_statistics_eth_info.outMulticastPkts,"%lu", var_2);
	sprintf(status_statistics_eth_info.TransmitFrames,"%lu", var_1 + var_2 );

	var_1 = get_key_vlaue(buf,"inUnicastPkts",16);
	sprintf(status_statistics_eth_info.inUnicastPkts,"%lu", var_1);
	var_2 = get_key_vlaue(buf,"inMulticastPkts",16);
	sprintf(status_statistics_eth_info.inMulticastPkts,"%lu", var_2);
	sprintf(status_statistics_eth_info.ReceiveFrames,"%lu", var_1 + var_2 );

	sprintf(status_statistics_eth_info.outOctets,"%lu", get_key_vlaue(buf,"outOctets",16));
	sprintf(status_statistics_eth_info.inOctets,"%lu", get_key_vlaue(buf,"inOctets",16));
	sprintf(status_statistics_eth_info.txCollisionCnt,"%u", get_key_vlaue(buf,"txCollisionCnt",16));
	sprintf(status_statistics_eth_info.rxCrcErr,"%lu", get_key_vlaue(buf,"rxCrcErr",16));
	sprintf(status_statistics_eth_info.outErrors,"%lu", get_key_vlaue(buf,"outErrors",16));
	sprintf(status_statistics_eth_info.txUnderRunCnt,"%lu", get_key_vlaue(buf,"txUnderRunCnt",16));
		
err:
  if (buf != NULL)
  {
		ASP_FREE(buf);
	}
		
	if(fd != -1)
	{
		close(fd);	
	}	
	return; 
}		
		
		
		
void StatusStatistics_adsl_init()
{
	char *buf = NULL;
	int  fd=0, ret=0;	
	
	memset(&status_statistics_adsl_info,0,sizeof(T_STATUS_STATISTICS_ADSL_INFO));	
	
	buf = (char *)ASP_MALLOC(1024*2);
	if (NULL == buf)
			return;
			

	fd = open("/proc/tc3162/tsarm_stats",O_RDONLY);
	if (fd == -1)
	goto err;
	
	ret = read(fd,buf,1024*2-1);
	if (ret<=0)
	goto err;
	*(buf+ret)=0;
	
#if 0
	pch_itemname = strstr(buf,"outPkts");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_adsl_info.outPkts,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_adsl_info.outPkts,"0");
	
	pch_itemname = strstr(buf,"inPkts");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_adsl_info.inPkts,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_adsl_info.inPkts,"0");
	
		pch_itemname = strstr(buf,"outDiscards");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_adsl_info.outDiscards,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_adsl_info.outDiscards,"0");	
	
	pch_itemname = strstr(buf,"inDiscards");	
	if (pch_itemname != NULL)
	{
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2,10);
			x = strtoul(value,NULL,16);
			sprintf(status_statistics_adsl_info.inDiscards,"%ld",x);	
		}
	}
	else
		sprintf(status_statistics_adsl_info.inDiscards,"0");
#endif 

	sprintf(status_statistics_adsl_info.outPkts,"%lu", get_key_vlaue(buf,"outPkts",16));
	sprintf(status_statistics_adsl_info.inPkts,"%lu", get_key_vlaue(buf,"inPkts",16));
	sprintf(status_statistics_adsl_info.outDiscards,"%lu", get_key_vlaue(buf,"outDiscards",16));
	sprintf(status_statistics_adsl_info.inDiscards,"%lu", get_key_vlaue(buf,"inDiscards",16));

err:
  if (buf != NULL)
		ASP_FREE(buf);
	 
	if(fd != -1)
		close(fd);	

	return; 
}

void StatusStatistics_wlan_init()
  {
	char *buf = NULL;
	int  fd=0, ret=0;	
	unsigned long int var_1 =0;

	memset(&status_statistics_wlan_info, 0, sizeof(T_STATUS_STATISTICS_WLAN_INFO));

	system("/userfs/bin/iwpriv ra0 stat  > /tmp/wlan_stats");

	buf = (char *)ASP_MALLOC(1024*2);
	
	fd = open("/tmp/wlan_stats",O_RDONLY);
	if (fd == -1)
		goto err;

	ret = read(fd,buf,1024*2-1);
	if (ret <= 0)
		goto err;

	*(buf+ret)=0;
	
	sprintf(status_statistics_wlan_info.wlanTxFrames,"%lu", get_key_vlaue(buf,"Tx success    ", 10));	
	var_1 = get_key_vlaue(buf,"Tx fail to Rcv ACK after retry", 10);
	sprintf(status_statistics_wlan_info.wlanTxErrFrames,"%lu", var_1);
	sprintf(status_statistics_wlan_info.wlanTxDropFrames,"%lu", var_1);	
	sprintf(status_statistics_wlan_info.wlanRxFrames,"%lu", get_key_vlaue(buf,"Rx success"), 10);	
	var_1 = get_key_vlaue(buf,"Rx with CRC", 10);
	sprintf(status_statistics_wlan_info.wlanRxErrFrames,"%lu", var_1);
	sprintf(status_statistics_wlan_info.wlanRxDropFrames,"%lu", var_1 + get_key_vlaue(buf,"Rx drop due to out of resource", 10));	
	
err:
	unlink("/tmp/wlan_stats");

	if (buf != NULL)
		ASP_FREE(buf);
	
	if(fd != -1)
		close(fd);	

	return; 
}	
	
unsigned long int get_key_vlaue(char *str, char *keyword, int base)
{
	char *pch_itemname = NULL;
	char *pch_temp = NULL;
	char value[11]={0};

	pch_itemname = strstr(str, keyword);
	if (pch_itemname != NULL)
	{	
		pch_temp = strstr(pch_itemname,"= ");
		if (pch_temp != NULL)	
		{
			strncpy(value,pch_temp+2, 10);
			return strtoul(value,NULL, base);
		}
		else
			return 0;
}
	else
		return 0;

}

void get_eth_stats()
{ 
	StatusStatistics_eth_init();
	
	ASP_OUTPUT("<tr><td class=\"tabdata\">&nbsp;&nbsp;Transmit Frames </td> <td class=\"tabdata\"><div align=\"center\">");
	ASP_OUTPUT (status_statistics_eth_info.TransmitFrames);
	
	ASP_OUTPUT ("</div></td> <td class=\"tabdata\">&nbsp;&nbsp;Receive Frames  </td> <td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_eth_info.ReceiveFrames );
	
	ASP_OUTPUT ("</div></td> </tr> <tr> <td class=\"tabdata\">&nbsp;&nbsp;Transmit Multicast Frames </td> <td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_eth_info.outMulticastPkts);
	
	ASP_OUTPUT ("</div></td> <td class=\"tabdata\">&nbsp;&nbsp;Receive Multicast Frames  </td> <td class=\"tabdata\"><div align=\"center\">");
	ASP_OUTPUT (status_statistics_eth_info.inMulticastPkts );
	
	ASP_OUTPUT ("</div></td> </tr> <tr> <td class=\"tabdata\">&nbsp;&nbsp;Transmit total Bytes  </td> <td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_eth_info.outOctets );
	
	ASP_OUTPUT ("</div></td> <td class=\"tabdata\">&nbsp;&nbsp;Receive total Bytes   </td> <td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_eth_info.inOctets );
	
	ASP_OUTPUT ("</div></td> </tr> <tr> <td class=\"tabdata\">&nbsp;&nbsp;Transmit Collision  </td> <td class=\"tabdata\"><div align=\"center\">" );	
	ASP_OUTPUT (status_statistics_eth_info.txCollisionCnt );
	
	ASP_OUTPUT ("</div></td> <td class=\"tabdata\">&nbsp;&nbsp;Receive CRC Errors   </td> <td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_eth_info.rxCrcErr );
	
	ASP_OUTPUT ("</div></td> </tr> <td class=\"tabdata\">&nbsp;&nbsp;Transmit Error Frames   </td> <td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_eth_info.outErrors );
	
	ASP_OUTPUT ("</div></td> <td class=\"tabdata\">&nbsp;&nbsp;Receive Under-size Frames   </td> <td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_eth_info.txUnderRunCnt );
	ASP_OUTPUT ("</div></td> </tr>" );       
}

void get_adsl_stats()
{
	StatusStatistics_adsl_init();
	
	ASP_OUTPUT ("<tr><td class=\"tabdata\">&nbsp;&nbsp;Transmit Total PDUs</td> <td class=\"tabdata\"><div align=\"center\">" );	
	ASP_OUTPUT (status_statistics_adsl_info.outPkts );

	ASP_OUTPUT ("</div></td><td class=\"tabdata\">&nbsp;&nbsp;Receive Total PDUs</td> <td class=\"tabdata\"><div align=\"center\">" );	
	ASP_OUTPUT (status_statistics_adsl_info.inPkts );
	
	ASP_OUTPUT ("</div></td></tr> <tr><td class=\"tabdata\">&nbsp;&nbsp;Transmit Total Error Counts</td><td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_adsl_info.outDiscards );
	
	ASP_OUTPUT ("</div></td><td class=\"tabdata\">&nbsp;&nbsp;Receive Total Error Counts</td><td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_adsl_info.inDiscards );
	
	ASP_OUTPUT ("</div></td></tr>" );
}

void get_wlan_stats()
{

	StatusStatistics_wlan_init();
	
	ASP_OUTPUT ("<tr><td class=\"tabdata\">&nbsp;&nbsp;Tx Frames Count</td> <td class=\"tabdata\"><div align=\"center\">" );	
	ASP_OUTPUT (status_statistics_wlan_info.wlanTxFrames);

	ASP_OUTPUT ("</div></td><td class=\"tabdata\">&nbsp;&nbsp;Rx Frames Count</td> <td class=\"tabdata\"><div align=\"center\">" );	
	ASP_OUTPUT (status_statistics_wlan_info.wlanRxFrames);
	
	ASP_OUTPUT ("</div></td></tr> <tr><td class=\"tabdata\">&nbsp;&nbsp;Tx Errors Count</td><td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_wlan_info.wlanTxErrFrames);
	
	ASP_OUTPUT ("</div></td><td class=\"tabdata\">&nbsp;&nbsp;Rx Errors Count</td><td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_wlan_info.wlanRxErrFrames);

	ASP_OUTPUT ("</div></td></tr> <tr><td class=\"tabdata\">&nbsp;&nbsp;Tx Drops Count</td><td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_wlan_info.wlanTxDropFrames);
	
	ASP_OUTPUT ("</div></td><td class=\"tabdata\">&nbsp;&nbsp;Rx Drops Count</td><td class=\"tabdata\"><div align=\"center\">" );
	ASP_OUTPUT (status_statistics_wlan_info.wlanRxDropFrames);
	
	ASP_OUTPUT ("</div></td></tr>" );
}

int TcWebApiStat_set(int id,char *value)
{
	char *retval;
	
	retval = ASP_FORM(value);
	if(retval == NULL)
		return -1;
	if(id == GB_C_STATISTICS_INTERFACE)
		strncpy(web_api->stat_interface,retval,16);			
	return 0;	
}

char *TcWebApiStat_get(int id)
{
	switch(id)
	{
		case GB_C_STATISTICS_INTERFACE:
			if(!ASP_ISPOST)
				return val_def->stat_ethernet;
			else
				return web_api->stat_interface;
		
		case GB_C_STATISTICS_INTERFACEETHERNET://radio ethernet    
			if(!strcmp(web_api->stat_interface,val_def->stat_ethernet) || (!ASP_ISPOST))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_STATISTICS_INTERFACEADSL://radio adsl   
			if(!strcmp(web_api->stat_interface,val_def->stat_adsl))
				ASP_OUTPUT("CHECKED");
			break;		
		case GB_C_STATISTICS_INTERFACEWLAN://radio wlan   
			if(!strcmp(web_api->stat_interface, val_def->stat_wlan))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_STATISTICS_ETHERNET:
	  		get_eth_stats();
			break;	
		case GB_C_STATISTICS_ADSL:
			get_adsl_stats();
			break;			
		case GB_C_STATISTICS_WLAN:
			get_wlan_stats();
			break;			
	}
	return ASP_OK;
}

