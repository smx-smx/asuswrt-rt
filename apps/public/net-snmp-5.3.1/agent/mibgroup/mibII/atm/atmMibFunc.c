/* Portions of this file are subject to the following copyright(s).  See
 * the Net-SNMP's COPYING file for more details and other copyrights
 * that may apply:
 */
/*
 * Portions of this file are copyrighted by:
 * Copyright © 2003 Sun Microsystems, Inc. All rights reserved.
 * Use is subject to license terms specified in the COPYING file
 * distributed with the Net-SNMP package.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/auto_nlist.h>

#include "../interfaces.h"
#include "atmMibFunc.h"


#define ATMMIB_STATUS_OK         0
#define ATMMIB_STATUS_ERR        1


static oid defAtmTrafficDescriptor[ATM_TRAFDESC_OIDLEN]  = { 1, 3, 6, 1, 2, 1, 37, 1, 1, 2 }   ;
static int     getConfVccCount( void );

/*_____________________________________________________________________________
**      function name: tcdbg_printf
**      descriptions:
**            Show debug message in kernel mode.
**
**      parameters:
**           N/A
**
**      global:
**           N/A
**
**      return:
**	     	 N/A
**      call:
**      	 N/A
**      revision:
**      1. JasonLin 2008/11/25
**____________________________________________________________________________
*/
void
tcdbg_printf(char *fmt,...){

	FILE *proc_file;
	char msg[256];
	va_list args;

	va_start(args, fmt);
	/* jrchen modified 20100310*/
	//	vsprintf(msg, fmt, args);
	vsnprintf(msg, 256, fmt, args);	

    proc_file = fopen("/proc/tc3162/dbg_msg", "w");
	if (!proc_file) {
		printf("open /proc/tc3162/dbg_msg fail\n");
		return;
	}
	fprintf(proc_file, "%s", msg);
	fclose(proc_file);
	va_end(args);
}/*end tcapi_save*/


#ifdef ATMIFCONFTBL
/* Fill out the ATM interface configuration table */
void  atmMibHelper_fillAtmIfConfTable( ATM_INTERFACE_CONF_TABLE* pTable )
{
	/*Add code to handle*/
}
#endif /* ATMIFCONFTBL */


#ifdef ATMIFTCTBL
/* Fill out the ATM interface TC table */
void  atmMibHelper_fillAtmIfTCTable( ATM_INTERFACE_TC_TABLE* pTable )
{
	/*Add code to handle*/
}
#endif /* ATMIFTCTBL */


#ifdef ATMTRAFFDESCTBL
/* Fill out the ATM interface traffic descriptor table */
int   atmMibHelper_fillAtmTrafDescTable( ATM_TRAFDESC_TABLE* pTable )
{
	/*Add code to handle*/

   	return 0;
}
#endif /* ATMTRAFFDESCTBL */

#ifdef ATMVCLTBL
/* Fill out the ATM VCL table */
int atmMibHelper_fillAtmVclTable( ATM_VCL_TABLE* pTable )
{
	/*Add code to handle*/

   	return 0;
}
#endif /* ATMVCLTBL */


#ifdef AAL5VCCTBL

/* Fill out the AAL5 VCC table */
int atmMibHelper_fillAal5VccTable( AAL5_VCC_TABLE* pTable )
{
   /* Assuming only one AAL5 CPCS interface */
   	int errStatus = ATMMIB_STATUS_ERR;
   	AAL5_VCC_TABLE* p = pTable;
   	int vccCount = 0;
   	int i;

	char nodeName[32], activeStatus[8], temp[32];
	int vpi = 0, vci = 0;
	int activePVCIndex[MAX_PVC_NUM+1];
	int crcErrors = 0, overSizedSDUs = 0, sarTimeOuts = 0;
	
   	for ( i = 0; i < MAX_PVC_NUM; i++ ) {
		memset(nodeName, 0, sizeof(nodeName));
		memset(activeStatus, 0, sizeof(activeStatus));
		sprintf(nodeName, PVC_ENTRY, i);

		tcapi_get(nodeName, "Active", activeStatus);
		if(strcmp(activeStatus, "Yes") ==  0){
			activePVCIndex[vccCount] = i;
			vccCount++;			
		}
	}

   	if ( vccCount > 0 ) {
		for ( i=1; i<=vccCount; i++ ) {
			memset(nodeName, 0, sizeof(nodeName));
			sprintf(nodeName, PVC_ENTRY, activePVCIndex[i-1]);
			memset(temp, 0, sizeof(temp));

			tcapi_get(nodeName, "VPI", temp);
			vpi = atoi(temp);
			memset(temp, 0, sizeof(temp));
			tcapi_get(nodeName, "VCI", temp);
			vci = atoi(temp);
			
			if(vpi >= 0 && vci > 0 ){
				p->pTableBeginning = pTable;
	           		p->aal5ifIndex = AAL5_IF_INDEX;
	           		p->aal5VccVpi = vpi;
		    		p->aal5VccVci = vci;
					
			    	p->aal5VccCrcErrors = 0;
		    		p->aal5VccOverSizedSDUs = 0;
		    		p->aal5VccSarTimeOuts = 0;				

				memset(nodeName, 0, sizeof(nodeName));
				sprintf(nodeName, INFO_NAS, activePVCIndex[i-1]);

				memset(temp, 0, sizeof(temp));
				tcapi_get(nodeName, "crcError", temp);
				p->aal5VccCrcErrors = atoi(temp);

				memset(temp, 0, sizeof(temp));
				tcapi_get(nodeName, "overSizedSDUS", temp);
				p->aal5VccOverSizedSDUs = atoi(temp);
				
				memset(temp, 0, sizeof(temp));
				tcapi_get(nodeName, "sarTimeouts", temp);
				p->aal5VccOverSizedSDUs = atoi(temp);

			//	tcdbg_printf("\n==>atmMibHelper_fillAal5VccTable, vpi=%d, vci=%d, crcErrors=%d, overSizedSDUs=%d, sarTimeOuts=%d\n", vpi, vci, crcErrors, overSizedSDUs, sarTimeOuts);
			       if ( i == vccCount ){
					/* The last row */
                     		p->next = NULL;
                     		errStatus = ATMMIB_STATUS_OK;
                  		}
                  		else{
                  			p->next = (p+1);
                  			p++;
               		}	
			}	
               	else {
		 		tcdbg_printf(("atmMibHelper_fillAal5VccTable: VPI or VCI Invaild"));
               		break;
                    }
		}
       }

   	return( errStatus );
}
#endif /* AAL5VCCTBL */

/* Get the number of traffic indexes */
int atmMibHelper_getTrafDescIndexCount( void )
{
   	return 0;
}

/* Get the number of ATM interfaces */
int atmMibHelper_getAtmIfCount( void )
{
   	return 1;
}

int atmMibHelper_getTotalVccCount( void )
{
	return (getConfVccCount());
}

/* Get the number of configured VCCs */
int getConfVccCount( void )
{
	char nodeName[32] = {0}, activeStatus[8] = {0};
	int i = 0, vccCount = 0;

	for(i=0; i<MAX_PVC_NUM; i++) {
		memset(nodeName, 0, sizeof(nodeName));
		memset(activeStatus, 0, sizeof(activeStatus));
		sprintf(nodeName, PVC_ENTRY, i);

		tcapi_get(nodeName, "Active", activeStatus);
		if(strcmp(activeStatus, "Yes") ==  0)
			vccCount ++;
	}
   
   	return vccCount;
}


#ifdef ATMTRAFFDESCTBL
/* Map the traffic descriptor type from a value provided by the ATM driver (0-15)
** to a oid value for the traffic descriptor type (e.g. { 1, 3, 6, 1, 2, 1, 37, 1, 1, 2 } ) */
void trafDescMapType( ATM_TRAFDESC_TABLE* tableRow, ATM_TRAFFIC_DESCR_PARM_ENTRY* driverTableRow )
{
   /* Set the default traffic descriptor */
   memcpy(  tableRow->atmTrafficDescrType, defAtmTrafficDescriptor, sizeof(defAtmTrafficDescriptor) );

   /* The ATM driver returns values from 1 to 15 for the traffic descriptor type.
   ** Modify only the last entry in the oid array. */
   tableRow->atmTrafficDescrType[ATM_TRAFDESC_OIDLEN-1] = driverTableRow->ulTrafficDescrType;
}

/* Copy the data from the driver table to the ATM traffic descriptor table */
void trafDescCopyDriverData( ATM_TRAFDESC_TABLE* tableRow, ATM_TRAFFIC_DESCR_PARM_ENTRY* driverTableRow )
{
   /* First, set the default values */
   trafDescSetDefValues( tableRow );

   /* Change only the values that can be obtained from the ATM driver.
   ** Don't change the other values (i.e. keep the defaults) */
   tableRow->atmTrafficDescrParamIndex = driverTableRow->ulTrafficDescrIndex;
   trafDescMapType( tableRow, driverTableRow );
   tableRow->atmTrafficDescrParam1   = driverTableRow->ulTrafficDescrParm1;
   tableRow->atmTrafficDescrParam2   = driverTableRow->ulTrafficDescrParm2;
   tableRow->atmTrafficDescrParam3   = driverTableRow->ulTrafficDescrParm3;
   tableRow->atmTrafficDescrParam4   = driverTableRow->ulTrafficDescrParm4;
   tableRow->atmTrafficDescrParam5   = driverTableRow->ulTrafficDescrParm5;
   tableRow->atmServiceCategory      = driverTableRow->ulServiceCategory;
   tableRow->atmTrafficFrameDiscard  = driverTableRow->ulTrafficFrameDiscard;
}

/* Set the default values for the ATM traffic descriptor table */
void trafDescSetDefValues( ATM_TRAFDESC_TABLE* tableRow )
{
   memcpy(  tableRow->atmTrafficDescrType, defAtmTrafficDescriptor, sizeof(defAtmTrafficDescriptor) );
   tableRow->atmTrafficDescrParam1   = 0;
   tableRow->atmTrafficDescrParam2   = 0;
   tableRow->atmTrafficDescrParam3   = 0;
   tableRow->atmTrafficDescrParam4   = 0;
   tableRow->atmTrafficDescrParam5   = 0;
   tableRow->atmTrafficQoSClass      = 0;
   tableRow->atmTrafficDescrRowStatus= 0;
   tableRow->atmServiceCategory      = 0;
   tableRow->atmTrafficFrameDiscard = 1;
}
#endif /* ATMTRAFFDESCTBL */

#ifdef ATMVCLTBL
/* Copy the driver data to the user structure for the ATM VCL table */
void vclCopyDriverData( ATM_VCL_TABLE* tableRow, PATM_VCC_ADDR pAddrs, ATM_VCC_CFG* vccCfg )
{
	/* First, set the default values */
	vclSetDefValues( tableRow );

   /* Change only the values that can be obtained from the ATM driver.
   ** Don't change the other values (i.e. keep the defaults) */
	tableRow->atmVclVpi						= pAddrs->usVpi;
	tableRow->atmVclVci						= pAddrs->usVci;
	tableRow->atmVclAdminStatus			      = vccCfg->ulAtmVclAdminStatus;
	tableRow->atmVclOperStatus				      = vccCfg->ulAtmVclOperStatus;
	tableRow->atmVclLastChange				      = vccCfg->ulAtmVclLastChange;
	tableRow->atmVccAalType					      = mapAalType( vccCfg->ulAalType );
	tableRow->atmVclTransmitTrafficDescrIndex = vccCfg->ulAtmVclTransmitTrafficDescrIndex;
	tableRow->atmVclReceiveTrafficDescrIndex  = tableRow->atmVclTransmitTrafficDescrIndex;
   	tableRow->atmVccAal5EncapsType            = vccCfg->u.Aal5Cfg.ulAtmVccEncapsType;
}

/* Set the default values for the ATM VCL table */
void vclSetDefValues( ATM_VCL_TABLE* tableRow )
{
   	tableRow->atmVccAalType					      = 3;
	tableRow->atmVccAal5CpcsTransmitSduSize   = 0xFFFF;
   	tableRow->atmVccAal5CpcsReceiveSduSize    = 0xFFFF;
	tableRow->atmVccAalType					      = 3;
   	tableRow->atmVclCrossConnectIdentifier    = 0;
   	tableRow->atmVclRowStatus                 = 1;
   	tableRow->atmVclCastType                  = 1;
   	tableRow->atmVclConnKind                  = 1;
}

/* Map the AAL type provided by the driver to a value understood by the MIB
** (used for the ATM VCL table) */
int mapAalType( UINT32 driverAalType )
{
   int aalType = 3;

   switch ( driverAalType )
   {
      case AAL_2:
         aalType = 6;
         break;
      case AAL_5:
         aalType = 3;
         break;
      default:
         aalType = 5;
         break;
   }

   return( aalType );
}
#endif /* ATMVCLTBL */


