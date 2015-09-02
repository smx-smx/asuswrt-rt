#include <stdio.h>
#include <stdlib.h>

#include "trendchip_api.h"
#include "trendchip-def.h"
#include "gb-constants.h"

int TcWebApiSave(int id,char *value);

int asp_TCWebApi_set(int id,char *value)
{
	if(id==GB_C_SAVE_CONFIG)
		return TcWebApiSave(id,value);
	
	if(id >GB_C_WAN_SETTING && id<GB_C_WAN_END)
		return TcWebApiWan_set(id,value);
	
	if(id>GB_C_LAN_SETTING && id<GB_C_LAN_END)
		return TcWebApiLan_set(id,value);
		
	if(id== GB_C_ADMIN_PASSWORD )
    	return TcWebApiTools_set(id,value);
	
  	if(id>GB_C_SNMP_SETTING && id<GB_C_SNMP_END)  
    	return TcWebApiAccess_snmp_set(id,value);

  	if(id ==GB_C_STATISTICS_INTERFACE )
		return TcWebApiStat_set(id,value);
		
	/*system restart*/
  	if(id ==GB_C_SYSTEM_RESTART )
  		return TcWebApiSysRestart_set(id,value);	
  
  	/*adsl*/	
  	if(id>GB_C_ADSL_SETTING && id<GB_C_ADSL_END)  
     	return TcWebApiAdv_adsl_set(id,value);	

  	/*nat*/
  	if(id >GB_C_NAT_SETTING && id<GB_C_NAT_END)
  		return TcWebApiNat_set(id,value);
  	
  	/*route*/
  	if(id >GB_C_ROUTING_SETTING && id<GB_C_ROUTING_END)
  		return TcWebApiRoute_set(id,value);
  	
  	/*acl*/
  	if(id>=GB_C_ACL_STATUS && id<=GB_C_ACL_END)  
    	return TcWebApiAccess_acl_set(id,value);

  	/*ipfliter*/
  	if(id>=GB_C_IPFILTER && id<=GB_C_IPFILTER_END)  
    	return TcWebApiAccess_ipfilter_set(id,value);
	/*upnp*/
  	if(id>=GB_C_UPNP_SETTING && id<=GB_C_UPNP_AUTODEACTIVATED)  
    	return TcWebApiAccess_upnp_set(id,value);

  	/*ddns*/
  	if(id>GB_C_DDNS_SETTING && id<GB_C_DDNS_END)  
    	return TcWebApiAccess_ddns_set(id,value); 

  	/*Diagnostic*/
	if(id ==GB_C_DIAGNOSTIC_VC )
		return TcWebApiDiagnostic_set(id,value);

	/*time*/
	if(id >=GB_C_TIME_NOW && id<=GB_C_TIME_SETTING)
  		return TcWebApiTimeZone_set(id,value); 

	if(id == GB_C_DEVICEINFO_PVC)
		return TcWebApiDeviceinfo_set(id,value);
	
	if(id>=GB_C_VLAN_ATMPVID0 && id<=GB_C_VLAN_PVIDEND)  
    	return TcWebApiAdv_vlan_pvid_set(id,value);          	
    	
  	if(id>=GB_C_VLAN_INDEX && id<=GB_C_VLAN_SUMMARY)  
    	return TcWebApiAdv_vlan_group_set(id,value);  
    	
 		/*adv_qos*/		
	if(id >=GB_C_QOS_SETTING && id<=GB_C_QOS_VID7)
		return TcWebApiAdvQos_set(id,value);	
		
 		/*adv_vlan_top*/		
	if(id >=GB_C_VLAN_SETTING && id<=GB_C_VLAN_DEACTIVATED)
		return TcWebApiAdvVlanTop_set(id,value);	
	
	/*wireless*/	
	if(id >=GB_C_WIRELESS_BEGIN && id<=GB_C_WIRELESS_END)
		return TcWebApiWireless_set(id,value);
// wtw add begin
	if(id>=GB_C_IPFILTER_EXTEND&&id<=GB_C_IPFILTER_EXTEND_END) //wkw 3.6
		return TcWebApiAccess_ipfilter_set(id,value);		
// wtw add end
		
	/*adv_firewall*/
	if(id >=GB_C_FW_FIREWALL && id<=GB_C_FW_SAVE)          //wjx 2007.3.5
		return TcWebApiAdv_firewall_set(id,value);		

#if 0
	if(id == GB_C_UPDATE_TYPE)
		return TcWebApiFtp_set(id,value);
#endif	
	return 0;
}

int asp_TCWebApi_execute(int id)
{
	switch (id)
	{
		case GB_C_WAN_SETTING:
		case GB_C_WAN_DELETE:
			return TcWebApiWan_execute(id);

		case GB_C_LAN_SETTING:
			return TcWebApiLan_execute();
			
		case GB_C_DMZ_SETTING:	
		case GB_C_VSERVER_ADD:
		case GB_C_VSERVER_DELETE:
		case GB_C_IPMAPPING_ADD:
		case GB_C_IPMAPPING_DELETE:	
			return TcWebApiNat_execute(id);

		case GB_C_ROUTING_ADD:
		case GB_C_ROUTING_DELETE:
			return TcWebApiRoute_execute(id);

		case GB_C_ACL_SETTING:
  		case GB_C_ACL_DELETE:
		    return TcWebApiAccess_acl_execute();	

		case GB_C_UPNP_SETTING:
		    return TcWebApiAccess_upnp_execute();

		case GB_C_TIME_SETTING:
		   return TcWebApiTimeZone_execute(id);   
		   
		  //ipfilte exten id   wkw 3.6
		case GB_C_IPFILTER_SETTING: //wtw add
  		case GB_C_IPFILTER_DELETE:
//wtw add begin
			case GB_C_URLFILTER_SAVE:   //wjx 2007.3.6
  		case GB_C_URLFILTER_DELETE:  
  		case GB_C_APPFILTER_SAVE:   //wjx 2007.3.7				
//wtw add end
			return TcWebApiAccess_ipfilter_execute(id);

		case GB_C_QOS_SETTING:
			return TcWebApiAdvQos_execute(id);		

		case GB_C_SNMP_SETTING:
			return TcWebApiAccess_snmp_execute();

		case GB_C_DDNS_SETTING:
			return TcWebApiAccess_ddns_execute();

		case GB_C_ADMIN_SETTING:
			return TcWebApiAdmin_execute();

		case GB_C_VLAN_GROUPSETTING:
  		case GB_C_VLAN_GROUPDELETE:
   			return TcWebApiAdv_vlan_group_execute(id);
   		case GB_C_WIRELESS_SETTING:
   			return TcWebApiWireless_execute(id);
   	
   		case GB_C_FW_SAVE:               //wjx 2007.3.6
   			return TcWebApiAdv_firewall_execute(id); 
   	        /* for renew and release ip. shnwind 2008.4.17*/
                case GB_C_DEVICEINFO_RENEW_IP:    
   	        case GB_C_DEVICEINFO_RELEASE_IP:
   	           return tcWebApiStat_execute(id);
#if 0
   		case GB_C_FTP_UPDATE:
   			return TcWebApiFtp_execute();
#endif   			
	}
	return 0;
}

char *asp_TCWebApi_get(int id)
{
	if(id >GB_C_WAN_SETTING && id<GB_C_WAN_END)
		return TcWebApiWan_get(id);

	if((id>GB_C_LAN_SETTING && id < 0x56) || ( id > 0x69 && id < GB_C_LAN_END))
		return TcWebApiLan_get(id);
	
	if(id>GB_C_SNMP_SETTING && id<GB_C_SNMP_END)  
	  	return TcWebApiAccess_snmp_get(id);
	
	if(id == GB_C_SYSTEM_LOG)
		return TcWebApiSyslog_get();		
	
	if(id >=GB_C_STATISTICS_INTERFACE && id<GB_C_STATISTICS_END)
		return TcWebApiStat_get(id);
	
	if(id>GB_C_ADSL_SETTING && id<GB_C_ADSL_END)  
     	return TcWebApiAdv_adsl_get(id);

     if((id >GB_C_NAT_SETTING && id<GB_C_NAT_END) || (id >= 0x56 && id <= 0x69))
  		return TcWebApiNat_get(id);

     /*route*/
  	if(id >GB_C_ROUTING_SETTING && id<GB_C_ROUTING_END)
  		return TcWebApiRoute_get(id);

  	/*acl*/
  	 if(id>=GB_C_ACL_STATUS && id<=GB_C_ACL_END)  
    	return TcWebApiAccess_acl_get(id);
	/*ipfilter*/
  	if(id>=GB_C_IPFILTER && id<=GB_C_IPFILTER_END)  
    	return TcWebApiAccess_ipfilter_get(id);
	/*upnp*/
  	if(id>=GB_C_UPNP_SETTING && id<=GB_C_UPNP_AUTODEACTIVATED)  
    	return TcWebApiAccess_upnp_get(id);

  	/*Diagnostic*/   
  	if(id >=GB_C_DIAGNOSTIC && id<=GB_C_DIAGNOSTIC_TEST_PING_YAHOO)
  		return TcWebApiDiagnostic_get(id);
  	
	 /*timezone*/
	if(id >=GB_C_TIME_NOW && id<=GB_C_TIME_SETTING)
		return TcWebApiTimeZone_get(id);

	/*deviceinfo*/
	if(id >GB_C_DEVICEINFO_SETTING && id<GB_C_DEVICEINFO_END)
		return TcWebApiDeviceinfo_get(id);

	if(id>=GB_C_VLAN_ATMPVID0 && id<=GB_C_VLAN_PVIDEND)  
    	return TcWebApiAdv_vlan_pvid_get(id);   

  	if(id>=GB_C_VLAN_INDEX && id<=GB_C_VLAN_SUMMARY)  
    	return TcWebApiAdv_vlan_group_get(id); 

 		/*adv_qos*/		
	if(id >=GB_C_QOS_SETTING && id<=GB_C_QOS_VID7)
		return TcWebApiAdvQos_get(id);		
		
 		/*adv_vlan_top*/		
	if(id >=GB_C_VLAN_SETTING && id<=GB_C_VLAN_DEACTIVATED)
		return TcWebApiAdvVlanTop_get(id);	
    	
    	/*ddns*/
    if(id>GB_C_DDNS_SETTING && id<GB_C_DDNS_END)  
    	return TcWebApiAccess_ddns_get(id); 	
    		
    	/*wireless*/ 	
  	if(id >=GB_C_WIRELESS_BEGIN && id<=GB_C_WIRELESS_END) 
  		return TcWebApiWireless_get(id); 	
  	//wtw add begin
	if(id>=GB_C_IPFILTER_EXTEND && id<=GB_C_IPFILTER_EXTEND_END) //wkw 3.6
		return TcWebApiAccess_ipfilter_get(id);
//wtw add end

    	/*adv_firewall*/
 	if(id >=GB_C_FW_FIREWALL && id<=GB_C_FW_SAVE)          //wjx 2007.3.5
		return TcWebApiAdv_firewall_get(id);	   	

#if 0
	if(id == GB_C_UPDATE_STATUS)
		return TcWebApiFtp_get(id);
#endif	
	return NULL;
}

int TcWebApiSave(int id,char *value)
{
	if(ASP_FORM(value) == NULL)
		return 0;
	
	
	/*write to flash*/
/* wtw del begin	asp_shell("/bin/tar cvf /tmp/romfile.tar	/etc/adsl.conf /etc/adsl.sh /etc/snmp/snmpd.conf \
	/etc/upnp.conf /etc/ddns.conf /etc/passwd /etc/led.conf /etc/ntp.sh /etc/timezone.conf  /etc/isp*  \
	/etc/udhcpd.conf /etc/nat_pvc* /etc/dproxy.* /etc/ipupdate.conf /etc/route* \
	/etc/lanconfig.sh /etc/config/dhcprelay.sh /etc/Wireless/RT61AP/RT61AP.dat"); wtw del end*/
	//wtw add begin
	//asp_shell("/bin/cfg_parser -c");
	system("/userfs/bin/cfg_parser -c");
	#if 0
	asp_shell("/bin/tar cvf /tmp/var/romfile.tar	/etc/adsl.conf /etc/adsl.sh /etc/snmp/snmpd.conf \
	/etc/upnp.conf /etc/ddns.conf /etc/passwd /etc/led.conf /etc/ntp.sh /etc/timezone.conf  /etc/isp*  \
	/etc/udhcpd.conf /etc/nat_pvc* /etc/dproxy.* /etc/ipupdate.conf /etc/route* \
	/etc/lanconfig.sh /etc/config/dhcprelay.sh /etc/Wireless/RT61AP/RT61AP.dat \
	/etc/mac.conf /etc/ipfilter/ipfilter* /etc/lan_rip.conf /etc/zebra.conf /etc/ripd.conf"); 
	#endif
	//wtw add end
	sleep(1);
	sync();
	system("/userfs/bin/mtd write /tmp/var/romfile.cfg romfile");
	return 0;
}
