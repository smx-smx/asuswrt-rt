#ifndef _TRENDCHIP_API_H
#define _TRENDCHIP_API_H


/*wan*/
int TcWebApiWan_set(int id,char*value);
char *TcWebApiWan_get(int id);
int TcWebApiWan_execute(int id);

/*lan*/
char *TcWebApiLan_get(int id);
int TcWebApiLan_set(int id,char *value);
int TcWebApiLan_execute();

/*passwd*/
int TcWebApiTools_set(int id,char*value);
int TcWebApiAdmin_execute();

/*asscee_anmp*/
int TcWebApiAccess_snmp_set(int id,char*value);
char *TcWebApiAccess_snmp_get(int id);
int TcWebApiAccess_snmp_execute();

/*system log*/
char *TcWebApiSyslog_get();

/*stat*/
char *TcWebApiStat_get(int id);
int TcWebApiStat_set(int id,char *value);
/* release and renew ip  shnwind add 2008.4.17 */
int tcWebApiStat_execute(int id);

/*tools test*/
char *TcWebApiDiagnostic_get(int id);

/*system restart*/
int TcWebApiSysRestart_set(int id,char *value);

/*adsl*/
int TcWebApiAdv_adsl_set(int id,char *value);
char *TcWebApiAdv_adsl_get(int id);

/*nat*/
char *TcWebApiNat_get(int id);
int TcWebApiNat_set(int id,char *value);
int TcWebApiNat_execute(int id);

/*route*/
char *TcWebApiRoute_get(int id);
int TcWebApiRoute_set(int id,char *value);
int TcWebApiRoute_execute(int id);

/*timezone*/ 
 int TcWebApiTimeZone_set(int id,char *value);   
 char *TcWebApiTimeZone_get(int id); 
 int TcWebApiTimeZone_execute(int id);
 int TcWebApiTimeZone_write();	 
 
/*vlan group*/
char *TcWebApiAdv_vlan_group_get(int id);
int TcWebApiAdv_vlan_group_set(int id,char *value);
int TcWebApiAdv_vlan_group_execute();

/*vlan pvid*/
char *TcWebApiAdv_vlan_pvid_get(int id);
int TcWebApiAdv_vlan_pvid_set(int id,char *value);

/*adv_qos*/	
int TcWebApiAdvQos_set(int id,char *value);		 
char *TcWebApiAdvQos_get(int id);	
int TcWebApiAdvQos_execute(int id);	
int TcWebApiAdvQos_write();	
			
/*adv_vlan_top*/	
int TcWebApiAdvVlanTop_set(int id,char *value);		 
char *TcWebApiAdvVlanTop_get(int id);	
int TcWebApiAdvVlanTop_write();
	
	/*asscee_acl*/
int TcWebApiAccess_acl_set(int id,char*value);
char *TcWebApiAccess_acl_get(int id);
int TcWebApiAccess_acl_execute();

/*asscee_ipfilter*/
int TcWebApiAccess_ipfilter_set(int id,char*value);
char *TcWebApiAccess_ipfilter_get(int id);
int TcWebApiAccess_ipfilter_execute();

	
/*upnp*/	
int TcWebApiAccess_upnp_set(int id,char*value);
int TcWebApiAccess_upnp_execute();
char *TcWebApiAccess_upnp_get(int id);

/*ddns*/
int TcWebApiAccess_ddns_set(int id,char*value);

/*test*/
int TcWebApiDiagnostic_set(int id,char *value);

/*status*/
int TcWebApiDeviceinfo_set(int id,char *value);
char *TcWebApiDeviceinfo_get(int id);

/*ddns*/
char *TcWebApiAccess_ddns_get(int id);
int TcWebApiAccess_ddns_set(int id,char*value);
int TcWebApiAccess_ddns_execute();

/*ftp*/
char *TcWebApiFtp_get(int id);
int TcWebApiFtp_set(int id,char *value);
int TcWebApiFtp_execute();

/*wireless wkw 07.3.1*/
char *TcWebApiWireless_get(int id);  
int TcWebApiWireless_set(int id,char *value);
int TcWebApiWireless_execute(int id);

/*adv_firewall*/                      //wjx 2007.3.5
char *TcWebApiAdv_firewall_get(int id);
int   TcWebApiAdv_firewall_set(int id,char *value);
int   TcWebApiAdv_firewall_execute(int id);

#endif
