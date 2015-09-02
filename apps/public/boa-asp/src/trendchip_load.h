#ifndef TRENDCHIP_LOAD_H
#define TRENDCHIP_LOAD_H

#define SNMPD_STAT_CONF "/etc/snmp/snmpd_stat.conf"

int get_profile_str(char *keyname,char *str_return,int fd);
int read_value_define();
char* config_get_value(char *src,char*dst,int size,char *item);

void read_internet_settings();
void read_lan_settings();
void read_eth_info();
void read_snmp_commu();
void read_adsl_conf();
void read_nat_conf();
void read_route_conf();
void read_acl_conf();
void read_ipfilter_conf();
void read_upnp_conf();
void read_ddns_conf();
void read_diag_dns(int cur_vc);
void read_time_settings();
void read_deviceinfo();

void read_vlan_pvid_conf();
void read_vlan_group_conf();

void read_adv_qos_settings();
void read_adv_vlan_top_settings();

//wkw 07.3.2
void read_wireless_settings();

void read_adv_firewall_settings();       //wjx 2007.3.5
//wtw add begin
void read_ipfilter_url_conf();								//wjx 2007.3.6
void read_ipfilter_app_conf();								//wjx 2007.3.7
//wtw add end
int check_nat_enable(int state);//shnwind add 2008.4.17
int check_MTU_change(int filter_state);
/*lee*/
typedef struct _INTERNET_SETTINGS{
	char status[16];
		
	long vpi;
	long vci;
	char qos[16];
	long pcr;
	long scr;
	long mbs;

	char isp[16];
	
	char encap[32];

	char nat_enable[16];
	char def_route[16];
	long mtu;
	char rip_ver[16];
	char direction[16];
	char igmp[16];

	char ip_addr[16];
	char netmask[16];
	char gateway[16];

	char usr_name[128];
	char passwd[128];
	char connection[32];
	long closeifidle;
	long last_mss;
	long mss;
	char ppp_get_ip[16];	

}INTERNET_SETTINGS;

typedef struct _LAN_SETTINGS{
	char mac_addr[18];
	char ip_addr[16];
	char netmask[16];
	char rip_ver[16];
	char direction[16];
	char igmp[16];
	char dhcp_type[16];
	char dhcp_startip[16];
	char dhcp_poolsize[16];
	char dhcp_leasetime[16];//here 20070201
	char dhcp_netmask[16];
	char dhcp_router[16];
	char dhcp_wins[16];
	char dhcp_serverip[16];
	char dns_type[16];
	char dns_primary[16];
	char dns_second[16];
}LAN_SETTINGS;

typedef struct _DMZ_SETUP{
	char 			status[16];
	char 			hostip[16];
	char			numberofips[16];
	char			serverip[16];
}DMZ_SETUP;

typedef struct _VSRV_SETUP{
	char start_port[8];
	char end_port[8];
	char local_ip[16];
	char numberofips[16];
	char serverip[16];
}VSRV_SETUP;

typedef struct _IPMAP_SETUP{
	char rule[32];
	char local_startip[16];
	char local_endip[16];
	char public_startip[16];
	char public_endip[16];
}IPMAP_SETUP;

typedef struct _NAT_SETUP{
	char 			status[16];
	char 			numberofips[16];
	char			serverip[16];	
	DMZ_SETUP		*dmz;
	VSRV_SETUP 		*vsrv[11];
	IPMAP_SETUP 	*ipmap[17];	
}NAT_SETUP;

typedef struct _ROUTE_SETUP{
	unsigned char  isdefault;
	char dest_ip[16];
	char netmask[16];
	char use[8];
	char gw_type[16];
	char gw_ip[16];
	char gw_interface[16];
	char metric[8];
	char announced_in_rip[16];
}ROUTE_SETUP;

typedef struct _DEVICE_INFO{
	char device_firmwarever[128];
}DEVICE_INFO;

/*wkw*/
typedef struct _ACCESS_SNMP{
   char rocommunity[32];
   char rwcommunity[32];
}ACCESS_SNMP;

typedef struct _ADV_ADSL{
   char mode[16];
   char type[16];
}ADV_ADSL;

typedef struct _ACL_RULE{
	char rule_status[8];
	char secure_ip[16];
	char application[8];
	char interface[8];  
}ACL_RULE;

typedef struct _ADV_VLAN_PVID{
   char atmvc0[2];
   char atmvc1[2];
   char atmvc2[2];
   char atmvc3[2];
   char atmvc4[2];
   char atmvc5[2];
   char atmvc6[2];
   char atmvc7[2];         
   char ethport0[2];
   char ethport1[2];
   char ethport2[2];
   char ethport3[2];
   char usbport[2];
}ADV_VLAN_PVID;

typedef struct _ADV_VLAN_GROUP{
   char active[8];
   char vlan_id[8];
   unsigned char atmp[8];
   unsigned char atmt[8];
   unsigned char ethernetp[4];
   unsigned char ethernett[4];   
   unsigned char usbp;
   unsigned char usbt;      
}ADV_VLAN_GROUP;

typedef struct _ACCESS_ACL{
	char status[8];
	ACL_RULE  *acl_rule[16];
}ACCESS_ACL;

typedef struct _IPFILTER_RULE{
   char active[8];
   char source_ip[16];
   char source_mask[16];
   char source_port[8];
   char des_ip[16];
   char des_mask[16];
   char des_port[8];
   char protocol[8];
//wtw del   char unmatched[8];
//wtw add begin
   char rule_type[8];//wkw 3.6 filter extend
   char mac[20];   	 //wkw 3.6 filter extend   
//wtw add end
}IPFILTER_RULE;

typedef struct _ACCESS_IPFILTER{
   char interface[16];
   char direction[16];
   IPFILTER_RULE *rule[6];
}ACCESS_IPFILTER;

typedef struct _ACCESS_UPNP{
   unsigned char status;
   unsigned char auto_conf;
}ACCESS_UPNP;

typedef struct _ACCESS_DDNS{
   unsigned char status;
   char provider[32];
   char host[128];
   char email[128];
   char username[128];
   char password[128];
   char wildcard [4];
}ACCESS_DDNS;

typedef struct _T_MAIN_DIAGNOSTICS{
		char dns_server       [16];
}T_MAIN_DIAGNOSTICS;

/*time_noze*/
typedef struct _T_MAIN_TIEMZONE{
		char Syn_with					[16];
		char time_month				[8];	
		char time_date				[8];			
		char time_year				[8];	
		char time_hour				[8];		
		char time_minute		 	[8];	
		char time_second			[8];							
		char time_zone_conf   [16];		
		char daylight_status  [16];		
		char ntp_server       [16];	
		char time_pc					[32];
		char *timezone[28]				;
		int  curr_timezone				;			
}T_MAIN_TIEMZONE;

/*adv_qos*/
typedef struct _T_ADV_QOS{
		char qos_status						[16];
		char qos_8021_status			[16];		
		char qos_ip_status				[16];
		char qos_iptype						[16];		
		char qos_app_status				[16];		
		char qos_vlan_status			[16];
		char qos_p8021_value0			[8];				
		char qos_p8021_value1			[8];
		char qos_p8021_value2 		[8];			
		char qos_p8021_value3 		[8];		
		char qos_p8021_value4			[8];		
		char qos_p8021_value5			[8];		
		char qos_p8021_value6			[8];		
		char qos_p8021_value7			[8];
		char qos_ipqos_value0			[8];				
		char qos_ipqos_value1			[8];	
		char qos_ipqos_value2			[8];
		char qos_ipqos_value3			[8];			
		char qos_ipqos_value4			[8];
		char qos_ipqos_value5			[8];			
		char qos_ipqos_value6			[8];
		char qos_ipqos_value7			[8];		
		char qos_voicequeueindex	[8];
		char qos_voicertprange1	 	[8];
		char qos_voicertprange2		[8];		
		char qos_videoqueueindex	[8];
		char qos_videortprange1		[8];
		char qos_videortprange2		[8];	
		char qos_igmpqueueindex		[8];
		char qos_dataqueueindex		[8];
		char qos_vlanqos_value0		[8];								
		char qos_vlanqos_value1		[8];
		char qos_vlanqos_value2		[8];
		char qos_vlanqos_value3		[8];
		char qos_vlanqos_value4		[8];
		char qos_vlanqos_value5		[8];
		char qos_vlanqos_value6		[8];
		char qos_vlanqos_value7		[8];		
}T_ADV_QOS;

/*home_wireless wkw 07.3.1*/
#define SSID_COUNT 4

typedef struct _WIRELESS_SSID{
        /*increase size from 32 to 33 because the SSID have 32 characters.
          shnwind 2008.4.17*/
        char SSID[33];
	char broadcast[4];
	char authentication[16];
	
	char keyIndex_64[4];
	/*change to 16 because it not enough to put the WEP 64bit key whick have 10 
          characters. shnwind 2008.4.17*/
	char key1_64[16];
	char key2_64[16];
	char key3_64[16];
	char key4_64[16];

	char keyIndex_128[4];
	char key1_128[32];
	char key2_128[32];
	char key3_128[32];
	char key4_128[32];	
	
	char encryption[8];
	char pre_shared[64]; 

	char active[8];
	char action[20];		
	char mac1[20]; 
	char mac2[20]; 
	char mac3[20]; 
	char mac4[20]; 
	char mac5[20]; 
	char mac6[20]; 
	char mac7[20]; 
	char mac8[20]; 	

}WIRELESS_SSID;

typedef struct _WIRELESS_SETTING{
		char access[8];
		char channel[32];
		char channel_num[8];
		char current_channel[8];
		char beacon[8];
		char RTS_CTS[8]; 
		char Fragmentation[8];
		char DTIM[8];
		char bg80211[16];
		
		char ssid_index[4];
						
		WIRELESS_SSID *SSID[SSID_COUNT];
	
}WIRELESS_SETTING;
//wtw add begin
typedef struct _T_URL_FILTER{               //wjx 2007.3.6
//		char index_status[16];								 //wjx 2007.4.13
		char url[48];		
}T_URL_FILTER;


/*********** ipfilter extend ************/
typedef struct _IPFILTER_EXTEND{
/*ipfilter_app begin wjx 2007.3.7*/
	 char appfilter_status[16];
	 char appfilter_icq_allowance[16];	 
	 char appfilter_msn_allowance[16];
	 char appfilter_ymsn_allowance[16];	 
	 char appfilter_ra_allowance[16];	 	 
/*ipfilter_app end wjx 2007.3.7*/   
	 char urlfilter_index[8];
	 char urlfilter_status[16];
	 T_URL_FILTER *urlfilter[16];
	 
}IPFILTER_EXTEND;	 
/**************************************/
//wtw add end


typedef struct _T_ADV_FIREWALL{               //wjx 2007.3.5
		char firewall_status[16];
		char spi_status[16];		
}T_ADV_FIREWALL;

typedef struct _WEB_API{	
/*lee*/
unsigned char 		wan_typechange;	
INTERNET_SETTINGS 	*wan[9];
unsigned char		lan_typechange;
LAN_SETTINGS		*lan[2];
unsigned char		nat_vc;
NAT_SETUP			*nat[9];
unsigned char		route_index;
ROUTE_SETUP			*route[50];
DEVICE_INFO			*dev_info;

char				admin_passwd[128];
ACCESS_SNMP 		*access_snmp;
char 				stat_interface[16];
char 				vlan_top_status[16];	
ADV_ADSL			 *adv_adsl;
ACCESS_ACL 			*access_acl;
ACCESS_IPFILTER		*access_ipfilter[12];
ACCESS_UPNP 		*access_upnp;
ACCESS_DDNS 		*access_ddns;


ADV_VLAN_PVID 		*adv_vlan_pvid;
ADV_VLAN_GROUP		*adv_vlan_group[8];

T_MAIN_DIAGNOSTICS  *main_diagnostics;
T_MAIN_TIEMZONE 	*main_timezone;
T_ADV_QOS         *adv_qos;	
//wkw 07.3.2
WIRELESS_SETTING *home_wireless;	
IPFILTER_EXTEND *ipfilter_extend; //wtw add
T_ADV_FIREWALL *adv_firewall;  				//wjx 2007.3.5
int nat_state;//shnwind add
int ip_filter_on;
}WEB_API;

WEB_API *web_api;

#define VALUE_INI  "/boaroot/value.ini"
typedef struct _VALUE_DEFINE{
	/*wan:lee*/
	char *wan_vc;
	char *wan_activated;
	char *wan_deactivated;
	char *wan_ubr;
	char *wan_cbr;
	char *wan_vbr;
	char *wan_nrtvbr;
	char *wan_dynamic_ip;
	char *wan_static_ip;
	char *wan_pppoe;
	char *wan_bridge;
	char *wan_encap_bridgellc;
	char *wan_encap_bridgevc;
	char *wan_encap_routellc;
	char *wan_encap_routevc;
	char *wan_encap_pppoellc;
	char *wan_encap_pppoevc;
	char *wan_encap_pppoallc;
	char *wan_encap_pppoavc;
	char *wan_natenable;
	char *wan_natdisable;
	char *wan_defroute_check;
	char *wan_defroute_uncheck;
	char *wan_rip_disable;
	char *wan_rip1;
	char *wan_rip2b;
	char *wan_rip2m;
	char *wan_direction_none;
	char *wan_direction_both;
	char *wan_direction_in;
	char *wan_direction_out;
	char *wan_igmpdisable;
	char *wan_igmpv1;
	char *wan_igmpv2;
	char *wan_connect_keepalive;
	char *wan_connect_ondemand;
	char *wan_pppgetip_static;
	char *wan_pppgetip_dynamic;
	/*lan:lee*/
	char *lan_dhcp_enable;
	char *lan_dhcp_disable;
	char *lan_dhcp_relay;
	char *lan_dns_directory;
	char *lan_dns_relay;
	
	/*stat:wjx*/
	char *stat_ethernet;
	char *stat_adsl;
	char *stat_wlan;
	
	/*system:wjx*/
 	char *sys_curr_set;
 	char *sys_def_set;
 
	 /*nat:lee*/
	 char *nat_status_activated;
	 char *nat_status_deactivated;
	 char *nat_ip_single;
	 char *nat_ip_multiple;
	 char *nat_dmz_enable;
	 char *nat_dmz_disable;
	 char *nat_ipmap_rule_one_to_one;
	 char *nat_ipmap_rule_many_to_one;
	 char *nat_ipmap_rule_many_to_many_overload;
	 char *nat_ipmap_rule_many_to_many_no_overload;
	 char *nat_ipmap_rule_server;

	 /*adv adsl:wkw*/
	char *adsl_mode_auto;
	char *adsl_mode_adsl2_add;
	char *adsl_mode_adsl2;
	char *adsl_mode_gdmt;
	char *adsl_mode_t1413;
	char *adsl_mode_glite;	
	char *adsl_type_annex_a;
	char *adsl_type_annex_i;
	char *adsl_type_annex_a_l;
	char *adsl_type_annex_m;	
	char *adsl_type_annex_all;	

 	/*access acl:wkw*/
	char *acl_status_activated;
	char *acl_status_deactivated;
	char *acl_rule_activated;
	char *acl_rule_deactivated;	
	char *acl_application_web;
	char *acl_application_ftp;
	char *acl_application_telnet;
	char *acl_application_snmp;	
	char *acl_application_ping;
	char *acl_application_all;
	char *acl_application_wan;
	char *acl_application_lan;		
	char *acl_application_both;		

	/*access ip_filter*/
	char *ipfilter_interface_pvc;
	char *ipfilter_interface_lan;
	char *ipfilter_direction_both;
	char *ipfilter_direction_incoming;	
	char *ipfilter_direction_outgoing;
	char *ipfilter_active_yes;
	char *ipfilter_active_no;
	char *ipfilter_protocol_tcp;	
	char *ipfilter_protocol_udp;
	char *ipfilter_protocol_icmp;
//wtw del	char *ipfilter_unmatched_forward;
//wtw del	char *ipfilter_unmatched_next;	

	/*access upnp*/
 	char *upnp_status_activated;
 	char *upnp_status_deactivated ;
 	char *adsl_auto_activated; 
 	char *adsl_auto_deactivated;

 	/*access ddns*/
	 char *ddns_status_activated;
	 char *ddns_status_deactivated;
	 char *ddns_provider_dyndns; 
	 char *ddns_provider_justlinux;
	 char *ddns_provider_dhs;
	 char *ddns_provider_ods;
	 char *ddns_provider_gnudip; 
	 char *ddns_provider_tzo; 
	 char *ddns_provider_easydns;
	 char *ddns_wildcard_yes; 
	 char *ddns_wildcard_no;

	/*diagnositics*/
	char *diag_vc;
	
	/*timezone*/
	char *time_syn_ntp;	 
	char *time_syn_pc;
	char *time_syn_manu;
	char *time_zone1;
	char *time_zone2;
	char *time_zone3;
	char *time_zone4;
	char *time_zone5;
	char *time_zone6;
	char *time_zone7;
	char *time_zone8;
	char *time_zone9;
	char *time_zone10;
	char *time_zone11;
	char *time_zone12;	
	char *time_zone13;
	char *time_zone14;
	char *time_zone15;
	char *time_zone16;
	char *time_zone17;
	char *time_zone18;
	char *time_zone19;	
	char *time_zone20;		
	char *time_zone21;
	char *time_zone22;
	char *time_zone23;
	char *time_zone24;
	char *time_zone25;
	char *time_zone26;
	char *time_zone27;
	char *time_zone28;
	char *time_daysave_enable;
	char *time_daysave_disable;	

	/*route*/
	char *pvcgateway_no;
	char *pvcgateway_yes;
	char *announcedinrip_no;
	char *announcedinrip_yes;	

	/*vlan_group*/
	char *vlan_group_active_yes;
	char *vlan_group_active_no; 
	
	/*adv_qos*/
	char *qos_activated;	 
	char *qos_deactivated;
	char *qos_8021_activated;
	char *qos_8021_deactivated;
	char *qos_ip_activated;
	char *qos_ip_deactivated;
	char *qos_iptype_tos;
	char *qos_iptype_diffserv;
	char *qos_app_activated;	
	char *qos_app_deactivated;
	char *qos_vlan_activated;
	char *qos_vlan_deactivated;	

	/*adv_vlan_top*/
	char *vlan_top_activated;	 
	char *vlan_top_deactivated;	

	/*ftp_update*/	
	char *update_rootfs;
	char *update_kernal;
	
	/*home_wireless wkw 07.3.1*/	
	char *wireless_access_activated							;
	char *wireless_access_deactivated						;
	
	char *wireless_channel_albania              ;   	
	char *wireless_channel_algeria              ;        
	char *wireless_channel_argentina            ;        
	char *wireless_channel_armenia              ;        
	char *wireless_channel_australia            ;        
	char *wireless_channel_austria              ;        
	char *wireless_channel_azerbaijan           ;        
	char *wireless_channel_bahrain              ;        
	char *wireless_channel_belarus              ;        
	char *wireless_channel_belgium              ;        
	char *wireless_channel_belize               ;        
	char *wireless_channel_bolvia               ;        
	char *wireless_channel_brazil               ;        
	char *wireless_channel_brunei_darussalam    ;        
	char *wireless_channel_bulgaria             ;        
	char *wireless_channel_canada               ;        
	char *wireless_channel_chile                ;        
	char *wireless_channel_china                ;        
	char *wireless_channel_colombia             ;        
	char *wireless_channel_costa_rica           ;        
	char *wireless_channel_croatia              ;        
	char *wireless_channel_cyprus               ;        
	char *wireless_channel_czch_republic        ;        
	char *wireless_channel_denmark              ;        
	char *wireless_channel_dominicanrepublic  	;        
	char *wireless_channel_ecuador              ;        
	char *wireless_channel_egypt                ;        
	char *wireless_channel_elsalvador        	  ;        
	char *wireless_channel_estonia              ;        
	char *wireless_channel_finland              ;        
	char *wireless_channel_france               ;        
	char *wireless_channel_georgia              ; //32       
	char *wireless_channel_germany              ;        
	char *wireless_channel_greece               ;        
	char *wireless_channel_guatemala            ;        
	char *wireless_channel_honduras             ;        
	char *wireless_channel_hongkong             ;        
	char *wireless_channel_hungary              ;        
	char *wireless_channel_iceland              ;        
	char *wireless_channel_india                ;        
	char *wireless_channel_indonesia            ;        
	char *wireless_channel_iran                 ;        
	char *wireless_channel_ireland              ;        
	char *wireless_channel_isarel               ;        
	char *wireless_channel_italy                ;        
	char *wireless_channel_japan                ;        
	char *wireless_channel_jordan               ;        
	char *wireless_channel_kazakhstan           ;        
	char *wireless_channel_northkorea        	  ;        
	char *wireless_channel_korearepublic     	  ;        
	char *wireless_channel_kuwait               ;        
	char *wireless_channel_latvia               ;        
	char *wireless_channel_lebanon              ;        
	char *wireless_channel_liechtenstein        ;        
	char *wireless_channel_lithuania            ;        
	char *wireless_channel_luxembourg           ;        
	char *wireless_channel_macau                ;        
	char *wireless_channel_macedonia            ;        
	char *wireless_channel_malaysia             ;        
	char *wireless_channel_mexico               ;        
	char *wireless_channel_monaco               ;        
	char *wireless_channel_morocco              ;        
	char *wireless_channel_netherlands          ;        
	char *wireless_channel_new_zealand          ; //64       
	char *wireless_channel_norway               ;        
	char *wireless_channel_oman                 ;        
	char *wireless_channel_pakistan             ;        
	char *wireless_channel_panama               ;        
	char *wireless_channel_peru                 ;        
	char *wireless_channel_philippines          ;        
	char *wireless_channel_poland               ;        
	char *wireless_channel_portugal             ;        
	char *wireless_channel_puerto_rico          ;        
	char *wireless_channel_qatar                ;        
	char *wireless_channel_romania              ;        
	char *wireless_channel_russia               ;        
	char *wireless_channel_saudi_arabia         ;        
	char *wireless_channel_singapore            ;        
	char *wireless_channel_slovakia             ;        
	char *wireless_channel_slovenia             ;        
	char *wireless_channel_south_africa         ;        
	char *wireless_channel_spain                ;        //17
	char *wireless_channel_swedan               ;        
	char *wireless_channel_switzerland          ;        
	char *wireless_channel_syriaarabrepublic  	;        
	char *wireless_channel_taiwan               ;        
	char *wireless_channel_thailand             ;        
	char *wireless_channel_trianidadandtobago 	;        
	char *wireless_channel_tunisia              ;        
	char *wireless_channel_turkey               ;        
	char *wireless_channel_ukraine              ;        
	char *wireless_channel_united_arab_emirates ;        
	char *wireless_channel_united_kingdom       ;        
	char *wireless_channel_unitedstates        	;        
	char *wireless_channel_uruguay              ;        
	char *wireless_channel_uzbekistan           ;      //32  
	char *wireless_channel_venezuela            ;        
	char *wireless_channel_vietnam              ;        
	char *wireless_channel_yemen                ;        
	char *wireless_channel_zimbabwe             ;        
	char *wireless_channel_undefined            ;      
	char *wireless_channel_auto			            ; 
	
	char *wireless_80211bg_80211b		            ;  
	char *wireless_80211bg_80211g		            ;   	
	char *wireless_80211bg_80211bg	            ;  	
	char *wireless_broadcast_yes		            ;	
	char *wireless_broadcast_no			            ;
	char *wireless_authentication_disabled   		;	
	char *wireless_authentication_wep64bits   	;
	char *wireless_authentication_wep128bits  	;                              
	char *wireless_authentication_wpapsk      	;                              
	char *wireless_authentication_wpa2psk     	;  
	
	char *wireless_active_activated      				;	                            
	char *wireless_active_deactivated      			;		
	char *wireless_action_allow 			    			;	
	char *wireless_action_deny	 			    			;	
	
	char *wireless_key1	 			    							;	
	char *wireless_key2	 			    							;	
	char *wireless_key3	 			    							;		
	char *wireless_key4	 			    							;	
	
	char *wireless_encryption_tkip 			    		;			
	char *wireless_encryption_aes 	 			    	;		
	
	char *wireless_default_key64								;
	char *wireless_default_key128								;
	char *wireless_default_mac									;
	char *wireless_default_SSID		           		;

	/* adv_firewall*/	                   //wjx 2007.3.5
	char *fw_firewall_enable;
	char *fw_firewall_disable;	
	char *fw_spi_enable;
	char *fw_spi_disable;	
//wtw add begin
	/*********** ipfilter extend ************/ //wkw 3.6
	char *ipfilter_type_ipmac;
	char *ipfilter_type_application;	
	char *ipfilter_type_url;		
	char *ipfilter_rule_ip;
	char *ipfilter_rule_mac;	
	
	
	char *urlfilter_activated;    //wjx 2007.3.6
	char *urlfilter_deactivated;
	
	char *appfilter_activated;    //wjx 2007.3.7
	char *appfilter_deactivated;	
	char *appfilter_icq_allow;    
	char *appfilter_icq_deny;
	char *appfilter_msn_allow;    
	char *appfilter_msn_deny;	
	char *appfilter_ymsn_allow;    
	char *appfilter_ymsn_deny;			
	char *appfilter_ra_allow;    
	char *appfilter_ra_deny;	
	
	/**************************************/
//wtw add end

}VALUE_DEFINE;

VALUE_DEFINE *val_def;

#endif
