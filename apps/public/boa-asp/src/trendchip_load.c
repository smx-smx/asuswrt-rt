#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "trendchip_load.h"

#include <linux/version.h>
#define KERNEL_2_6_36 	(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))

extern unsigned char StartChannelNumber;
extern unsigned char EndChannelNumber;

#define UPNPD_PATH "/etc/igd/igd.conf" 

/*move all function definition to trendchip_load.h  
   because these function may be called by other function */
#if 0
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
#endif

int load_config_file()
{

	if(read_value_define()==-1)
		return -1;
	
	web_api = (WEB_API*)malloc(sizeof(WEB_API));
	memset(web_api,0,sizeof(WEB_API));
	
	read_internet_settings();
	read_lan_settings();
	read_snmp_commu();
	read_adsl_conf();
	read_nat_conf();
	read_route_conf();
	read_deviceinfo();
	
	read_acl_conf();
	read_ipfilter_conf();
	read_upnp_conf();
	read_ddns_conf();
	read_diag_dns(0);
	read_time_settings();

	read_vlan_pvid_conf();
	read_vlan_group_conf();
	
 read_adv_qos_settings();
 read_adv_vlan_top_settings();	
 //wkw 07.3.2
  read_wireless_settings();	

  read_adv_firewall_settings();   //wjx 2007.3.5
  web_api->nat_state = -1;             //use -1 after boot 
  web_api->nat_state = check_nat_enable(web_api->nat_state);  //shnwind 2008.4.17
  web_api->ip_filter_on = 0;//does not load ip_filter at boot
  web_api->ip_filter_on = check_MTU_change(web_api->ip_filter_on);
  
  return 0;
}
int get_profile_str(char *keyname,char *str_return,int fd)
{
	char *str_key,*str_enter;
	char stream[200];
	int len,ret,key_len;
	off_t pos;
	
	lseek(fd,0L,SEEK_SET);
	while((len=read(fd,stream,200))>0)
	{
		pos = lseek(fd,0,SEEK_CUR);
		if(pos == -1L)
			return 1;
		pos = pos-len;
		
		str_key = strstr(stream,keyname);
		str_enter = strrchr(stream,'\n');

		if(str_enter ==NULL)
			return 1;
		if(str_key ==NULL)
		{
			pos = pos + (str_enter-stream) +1;
			if(lseek(fd,pos,SEEK_SET)== -1L)
				return 1;
			else
				continue;
		}
		else
		{
			str_enter = strchr(str_key,'\n');
			if(str_enter == NULL)
			{
				pos = pos + (str_key-stream);
				if(lseek(fd,pos,SEEK_SET)== -1L)
					return 1;
				else
					continue;
			}
			else
			{			
				key_len = strlen(keyname);
				ret = str_enter-(str_key+key_len);
				memcpy(str_return,str_key+key_len,ret);
				str_return[ret]= '\0';
//fprintf(stderr,"----%s%s\n",keyname,str_return);				
				return ret+1;
			}
						
		}
		
		
	}
	return 1;
}

static char*string_buf;
int read_value_define()
{
	int fd;
	struct stat buf;
	int ret,err=-1;
	
	val_def = (VALUE_DEFINE*)malloc(sizeof(VALUE_DEFINE));

	string_buf = (char*)malloc(1024*8);
	memset(string_buf,0,1024*8);
	
	fd = open(VALUE_INI,O_RDONLY);
	if(fd == -1)
		return err;

	ret = fstat(fd,&buf);
	if(ret == -1 || buf.st_size == 0)  
		goto read_value_define_err;

	ret = 0;
	/*wan*/
	val_def->wan_vc = string_buf+ret;
	ret += get_profile_str("wan_vc=",val_def->wan_vc,fd);
	
	val_def->wan_activated = string_buf+ret;
	ret += get_profile_str("wan_activated=",val_def->wan_activated,fd);

	val_def->wan_deactivated = string_buf+ret;
	ret += get_profile_str("wan_deactivated=",val_def->wan_deactivated,fd);
	
	val_def->wan_ubr = string_buf+ret;
	ret += get_profile_str("wan_ubr=",val_def->wan_ubr,fd);

	val_def->wan_cbr = string_buf+ret;
	ret += get_profile_str("wan_cbr=",val_def->wan_cbr,fd);

	val_def->wan_vbr = string_buf+ret;
	ret += get_profile_str("wan_vbr=",val_def->wan_vbr,fd);

	val_def->wan_nrtvbr = string_buf+ret;
	ret += get_profile_str("wan_nrtvbr=",val_def->wan_nrtvbr,fd);

	val_def->wan_dynamic_ip = string_buf+ret;
	ret += get_profile_str("wan_dynamic_ip=",val_def->wan_dynamic_ip,fd);

	val_def->wan_static_ip = string_buf+ret;
	ret += get_profile_str("wan_static_ip=",val_def->wan_static_ip,fd);
	
	val_def->wan_pppoe = string_buf+ret;
	ret += get_profile_str("wan_pppoe=",val_def->wan_pppoe,fd);

	val_def->wan_bridge = string_buf+ret;
	ret += get_profile_str("wan_bridge=",val_def->wan_bridge,fd);
	
	val_def->wan_encap_bridgellc = string_buf+ret;
	ret += get_profile_str("wan_encap_bridgellc=",val_def->wan_encap_bridgellc,fd);

	val_def->wan_encap_bridgevc = string_buf+ret;
	ret += get_profile_str("wan_encap_bridgevc=",val_def->wan_encap_bridgevc,fd);

	val_def->wan_encap_routellc = string_buf+ret;
	ret += get_profile_str("wan_encap_routellc=",val_def->wan_encap_routellc,fd);

	val_def->wan_encap_routevc = string_buf+ret;
	ret += get_profile_str("wan_encap_routevc=",val_def->wan_encap_routevc,fd);

	val_def->wan_encap_pppoellc = string_buf+ret;
	ret += get_profile_str("wan_encap_pppoellc=",val_def->wan_encap_pppoellc,fd);
	
	val_def->wan_encap_pppoevc = string_buf+ret;
	ret += get_profile_str("wan_encap_pppoevc=",val_def->wan_encap_pppoevc,fd);

	val_def->wan_encap_pppoallc = string_buf+ret;
	ret += get_profile_str("wan_encap_pppoallc=",val_def->wan_encap_pppoallc,fd);

	val_def->wan_encap_pppoavc = string_buf+ret;
	ret += get_profile_str("wan_encap_pppoavc=",val_def->wan_encap_pppoavc,fd);

	val_def->wan_natenable = string_buf+ret;
	ret += get_profile_str("wan_natenable=",val_def->wan_natenable,fd);
	
	val_def->wan_natdisable = string_buf+ret;
	ret += get_profile_str("wan_natdisable=",val_def->wan_natdisable,fd);

	val_def->wan_defroute_check = string_buf+ret;
	ret += get_profile_str("wan_defroute_check=",val_def->wan_defroute_check,fd);

	val_def->wan_defroute_uncheck = string_buf+ret;
	ret += get_profile_str("wan_defroute_uncheck=",val_def->wan_defroute_uncheck,fd);

	val_def->wan_rip_disable = string_buf+ret;
	ret += get_profile_str("wan_rip_disable=",val_def->wan_rip_disable,fd);
	
	val_def->wan_rip1 = string_buf+ret;
	ret += get_profile_str("wan_rip1=",val_def->wan_rip1,fd);

	val_def->wan_rip2b = string_buf+ret;
	ret += get_profile_str("wan_rip2b=",val_def->wan_rip2b,fd);

	val_def->wan_rip2m = string_buf+ret;
	ret += get_profile_str("wan_rip2m=",val_def->wan_rip2m,fd);

	val_def->wan_direction_none = string_buf+ret;
	ret += get_profile_str("wan_direction_none=",val_def->wan_direction_none,fd);

	val_def->wan_direction_both = string_buf+ret;
	ret += get_profile_str("wan_direction_both=",val_def->wan_direction_both,fd);

	val_def->wan_direction_in = string_buf+ret;
	ret += get_profile_str("wan_direction_in=",val_def->wan_direction_in,fd);

	val_def->wan_direction_out = string_buf+ret;
	ret += get_profile_str("wan_direction_out=",val_def->wan_direction_out,fd);

	val_def->wan_igmpdisable = string_buf+ret;
	ret += get_profile_str("wan_igmpdisable=",val_def->wan_igmpdisable,fd);

	val_def->wan_igmpv1 = string_buf+ret;
	ret += get_profile_str("wan_igmpv1=",val_def->wan_igmpv1,fd);

	val_def->wan_igmpv2 = string_buf+ret;
	ret += get_profile_str("wan_igmpv2=",val_def->wan_igmpv2,fd);

	val_def->wan_connect_keepalive = string_buf+ret;
	ret += get_profile_str("wan_connect_keepalive=",val_def->wan_connect_keepalive,fd);

	val_def->wan_connect_ondemand = string_buf+ret;
	ret += get_profile_str("wan_connect_ondemand=",val_def->wan_connect_ondemand,fd);

	val_def->wan_pppgetip_static = string_buf+ret;
	ret += get_profile_str("wan_pppgetip_static=",val_def->wan_pppgetip_static,fd);

	val_def->wan_pppgetip_dynamic = string_buf+ret;
	ret += get_profile_str("wan_pppgetip_dynamic=",val_def->wan_pppgetip_dynamic,fd);

	/*lan*/
	val_def->lan_dhcp_disable = string_buf+ret;
	ret += get_profile_str("lan_dhcp_disable=",val_def->lan_dhcp_disable,fd);
	
	val_def->lan_dhcp_enable = string_buf+ret;
	ret += get_profile_str("lan_dhcp_enable=",val_def->lan_dhcp_enable,fd);

	val_def->lan_dhcp_relay = string_buf+ret;
	ret += get_profile_str("lan_dhcp_relay=",val_def->lan_dhcp_relay,fd);

	val_def->lan_dns_directory = string_buf+ret;
	ret += get_profile_str("lan_dns_directory=",val_def->lan_dns_directory,fd);

	val_def->lan_dns_relay = string_buf+ret;
	ret += get_profile_str("lan_dns_relay=",val_def->lan_dns_relay,fd);
	
	/*stat*/
	val_def->stat_ethernet = string_buf+ret;
	ret += get_profile_str("stat_ethernet=",val_def->stat_ethernet,fd);

	val_def->stat_adsl = string_buf+ret;
	ret += get_profile_str("stat_adsl=",val_def->stat_adsl,fd);

	val_def->stat_wlan = string_buf+ret;
	ret += get_profile_str("stat_wlan=", val_def->stat_wlan,fd);

	/*system*/
 	val_def->sys_curr_set = string_buf+ret;
 	ret += get_profile_str("sys_curr_set=",val_def->sys_curr_set,fd);

 	val_def->sys_def_set = string_buf+ret;
 	ret += get_profile_str("sys_def_set=",val_def->sys_def_set,fd);

	/*nat*/
	val_def->nat_status_activated = string_buf+ret;
 	ret += get_profile_str("nat_status_activated=",val_def->nat_status_activated,fd);

	val_def->nat_status_deactivated = string_buf+ret;
 	ret += get_profile_str("nat_status_deactivated=",val_def->nat_status_deactivated,fd);

	val_def->nat_ip_single = string_buf+ret;
 	ret += get_profile_str("nat_ip_single=",val_def->nat_ip_single,fd);

 	val_def->nat_ip_multiple = string_buf+ret;
 	ret += get_profile_str("nat_ip_multiple=",val_def->nat_ip_multiple,fd);

 	val_def->nat_dmz_enable = string_buf+ret;
 	ret += get_profile_str("nat_dmz_enable=",val_def->nat_dmz_enable,fd);
 	
 	val_def->nat_dmz_disable = string_buf+ret;
 	ret += get_profile_str("nat_dmz_disable=",val_def->nat_dmz_disable,fd);

 	val_def->nat_ipmap_rule_one_to_one = string_buf+ret;
 	ret += get_profile_str("nat_ipmap_rule_one_to_one=",val_def->nat_ipmap_rule_one_to_one,fd);

	val_def->nat_ipmap_rule_many_to_one = string_buf+ret;
 	ret += get_profile_str("nat_ipmap_rule_many_to_one=",val_def->nat_ipmap_rule_many_to_one,fd);

 	val_def->nat_ipmap_rule_many_to_many_overload = string_buf+ret;
 	ret += get_profile_str("nat_ipmap_rule_many_to_many_overload=",val_def->nat_ipmap_rule_many_to_many_overload,fd);

 	val_def->nat_ipmap_rule_many_to_many_no_overload = string_buf+ret;
 	ret += get_profile_str("nat_ipmap_rule_many_to_many_no_overload=",val_def->nat_ipmap_rule_many_to_many_no_overload,fd);

	val_def->nat_ipmap_rule_server = string_buf+ret;
 	ret += get_profile_str("nat_ipmap_rule_server=",val_def->nat_ipmap_rule_server,fd);

	/*adsl*/
	val_def->adsl_mode_auto = string_buf+ret;
	ret += get_profile_str("adsl_mode_auto=",val_def->adsl_mode_auto,fd);	
	
	val_def->adsl_mode_adsl2_add = string_buf+ret;
	ret += get_profile_str("adsl_mode_adsl2_add=",val_def->adsl_mode_adsl2_add,fd);	

	val_def->adsl_mode_adsl2 = string_buf+ret;
	ret += get_profile_str("adsl_mode_adsl2=",val_def->adsl_mode_adsl2,fd);	
	
	val_def->adsl_mode_gdmt = string_buf+ret;
	ret += get_profile_str("adsl_mode_gdmt=",val_def->adsl_mode_gdmt,fd);		
	
	val_def->adsl_mode_t1413 = string_buf+ret;
	ret += get_profile_str("adsl_mode_t1413=",val_def->adsl_mode_t1413,fd);	
	
	val_def->adsl_mode_glite = string_buf+ret;
	ret += get_profile_str("adsl_mode_glite=",val_def->adsl_mode_glite,fd);		
	
	val_def->adsl_type_annex_a = string_buf+ret;
	ret += get_profile_str("adsl_type_annex_a=",val_def->adsl_type_annex_a,fd);	
	
	val_def->adsl_type_annex_i = string_buf+ret;
	ret += get_profile_str("adsl_type_annex_i=",val_def->adsl_type_annex_i,fd);	
	
	val_def->adsl_type_annex_a_l = string_buf+ret;
	ret += get_profile_str("adsl_type_annex_a_l=",val_def->adsl_type_annex_a_l,fd);		
	
	val_def->adsl_type_annex_m = string_buf+ret;
	ret += get_profile_str("adsl_type_annex_m=",val_def->adsl_type_annex_m,fd);	
	
	val_def->adsl_type_annex_all = string_buf+ret;
	ret += get_profile_str("adsl_type_annex_all=",val_def->adsl_type_annex_all,fd);	

	/*access acl*/
	val_def->acl_status_activated = string_buf+ret;
	ret += get_profile_str("acl_status_activated=",val_def->acl_status_activated,fd);	
	
	val_def->acl_status_deactivated = string_buf+ret;
	ret += get_profile_str("acl_status_deactivated=",val_def->acl_status_deactivated,fd);		
	
	val_def->acl_rule_activated= string_buf+ret;
	ret += get_profile_str("acl_rule_activated=",val_def->acl_rule_activated,fd);	
	
	val_def->acl_rule_deactivated = string_buf+ret;
	ret += get_profile_str("acl_rule_deactivated=",val_def->acl_rule_deactivated,fd);	
	
	val_def->acl_application_web = string_buf+ret;
	ret += get_profile_str("acl_application_web=",val_def->acl_application_web,fd);	
	
	val_def->acl_application_ftp = string_buf+ret;
	ret += get_profile_str("acl_application_ftp=",val_def->acl_application_ftp,fd);		
	
	val_def->acl_application_telnet = string_buf+ret;
	ret += get_profile_str("acl_application_telnet=",val_def->acl_application_telnet,fd);	
	
	val_def->acl_application_snmp = string_buf+ret;
	ret += get_profile_str("acl_application_snmp=",val_def->acl_application_snmp,fd);				
	
	val_def->acl_application_ping = string_buf+ret;
	ret += get_profile_str("acl_application_ping=",val_def->acl_application_ping,fd);		
	
	val_def->acl_application_all = string_buf+ret;
	ret += get_profile_str("acl_application_all=",val_def->acl_application_all,fd);	
	
	val_def->acl_application_wan = string_buf+ret;
	ret += get_profile_str("acl_application_wan=",val_def->acl_application_wan,fd);	
	
	val_def->acl_application_lan = string_buf+ret;
	ret += get_profile_str("acl_application_lan=",val_def->acl_application_lan,fd);	
	
	val_def->acl_application_both = string_buf+ret;
	ret += get_profile_str("acl_application_both=",val_def->acl_application_both,fd);		

	/*ip_filter*/
	val_def->ipfilter_interface_pvc = string_buf+ret;
	ret += get_profile_str("ipfilter_interface_pvc=",val_def->ipfilter_interface_pvc,fd);	
	
	val_def->ipfilter_interface_lan = string_buf+ret;
	ret += get_profile_str("ipfilter_interface_lan=",val_def->ipfilter_interface_lan,fd);		
	
	val_def->ipfilter_direction_both= string_buf+ret;
	ret += get_profile_str("ipfilter_direction_both=",val_def->ipfilter_direction_both,fd);	
	
	val_def->ipfilter_direction_incoming = string_buf+ret;
	ret += get_profile_str("ipfilter_direction_incoming=",val_def->ipfilter_direction_incoming,fd);	
	
	val_def->ipfilter_direction_outgoing = string_buf+ret;
	ret += get_profile_str("ipfilter_direction_outgoing=",val_def->ipfilter_direction_outgoing,fd);	
	
	val_def->ipfilter_active_yes = string_buf+ret;
	ret += get_profile_str("ipfilter_active_yes=",val_def->ipfilter_active_yes,fd);		
	
	val_def->ipfilter_active_no = string_buf+ret;
	ret += get_profile_str("ipfilter_active_no=",val_def->ipfilter_active_no,fd);	
	
	val_def->ipfilter_protocol_tcp = string_buf+ret;
	ret += get_profile_str("ipfilter_protocol_tcp=",val_def->ipfilter_protocol_tcp,fd);				
	
	val_def->ipfilter_protocol_udp = string_buf+ret;
	ret += get_profile_str("ipfilter_protocol_udp=",val_def->ipfilter_protocol_udp,fd);		
	
	val_def->ipfilter_protocol_icmp = string_buf+ret;
	ret += get_profile_str("ipfilter_protocol_icmp=",val_def->ipfilter_protocol_icmp,fd);	
	
	/*wtw del val_def->ipfilter_unmatched_forward = string_buf+ret;
	ret += get_profile_str("ipfilter_unmatched_forward=",val_def->ipfilter_unmatched_forward,fd);	
	
	val_def->ipfilter_unmatched_next = string_buf+ret;
	ret += get_profile_str("ipfilter_unmatched_next=",val_def->ipfilter_unmatched_next,fd);	*/
	
	/*upnp*/
	val_def->upnp_status_activated = string_buf+ret;
	ret += get_profile_str("upnp_status_activated=",val_def->upnp_status_activated,fd);	
	
	val_def->upnp_status_deactivated = string_buf+ret;
	ret += get_profile_str("upnp_status_deactivated=",val_def->upnp_status_deactivated,fd);		
	
	val_def->adsl_auto_activated = string_buf+ret;
	ret += get_profile_str("adsl_auto_activated=",val_def->adsl_auto_activated,fd);	
	
	val_def->adsl_auto_deactivated = string_buf+ret;
	ret += get_profile_str("adsl_auto_deactivated=",val_def->adsl_auto_deactivated,fd);	

	/*ddns*/
	val_def->ddns_status_activated = string_buf+ret;
	ret += get_profile_str("ddns_status_activated=",val_def->ddns_status_activated,fd);	
	
	val_def->ddns_status_deactivated = string_buf+ret;
	ret += get_profile_str("ddns_status_deactivated=",val_def->ddns_status_deactivated,fd);		
	
	val_def->ddns_provider_dyndns = string_buf+ret;
	ret += get_profile_str("ddns_provider_dyndns=",val_def->ddns_provider_dyndns,fd);	
	
	val_def->ddns_provider_justlinux = string_buf+ret;
	ret += get_profile_str("ddns_provider_justlinux=",val_def->ddns_provider_justlinux,fd);	
	
	val_def->ddns_provider_dhs = string_buf+ret;
	ret += get_profile_str("ddns_provider_dhs=",val_def->ddns_provider_dhs,fd);	
	
	val_def->ddns_provider_ods = string_buf+ret;
	ret += get_profile_str("ddns_provider_ods=",val_def->ddns_provider_ods,fd);		
	
	val_def->ddns_provider_gnudip = string_buf+ret;
	ret += get_profile_str("ddns_provider_gnudip=",val_def->ddns_provider_gnudip,fd);	
	
	val_def->ddns_provider_tzo = string_buf+ret;
	ret += get_profile_str("ddns_provider_tzo=",val_def->ddns_provider_tzo,fd);				
	
	val_def->ddns_provider_easydns = string_buf+ret;
	ret += get_profile_str("ddns_provider_easydns=",val_def->ddns_provider_easydns,fd);		
	
	val_def->ddns_wildcard_yes = string_buf+ret;
	ret += get_profile_str("ddns_wildcard_yes=",val_def->ddns_wildcard_yes,fd);	
	
	val_def->ddns_wildcard_no = string_buf+ret;
	ret += get_profile_str("ddns_wildcard_no=",val_def->ddns_wildcard_no,fd);		

	val_def->diag_vc = string_buf+ret;
	ret += get_profile_str("diag_vc=",val_def->diag_vc,fd);	

	/*diagnositics*/
	val_def->diag_vc = string_buf+ret;
	ret += get_profile_str("diag_vc=",val_def->diag_vc,fd);		
	
	/*time_zone*/
	val_def->time_syn_ntp = string_buf+ret;
	ret += get_profile_str("time_syn_ntp=",val_def->time_syn_ntp,fd);		
		
	val_def->time_syn_pc = string_buf+ret;
	ret += get_profile_str("time_syn_pc=",val_def->time_syn_pc,fd);		
	
	val_def->time_syn_manu = string_buf+ret;
	ret += get_profile_str("time_syn_manu=",val_def->time_syn_manu,fd);		
	
	val_def->time_zone1 = string_buf+ret;
	ret += get_profile_str("time_zone1=",val_def->time_zone1,fd);		
	
	val_def->time_zone2 = string_buf+ret;
	ret += get_profile_str("time_zone2=",val_def->time_zone2,fd);
	
	val_def->time_zone3 = string_buf+ret;
	ret += get_profile_str("time_zone3=",val_def->time_zone3,fd);		
	
	val_def->time_zone4 = string_buf+ret;
	ret += get_profile_str("time_zone4=",val_def->time_zone4,fd);		
	
	val_def->time_zone5 = string_buf+ret;
	ret += get_profile_str("time_zone5=",val_def->time_zone5,fd);		
	
	val_def->time_zone6 = string_buf+ret;
	ret += get_profile_str("time_zone6=",val_def->time_zone6,fd);	
	
	val_def->time_zone7 = string_buf+ret;
	ret += get_profile_str("time_zone7=",val_def->time_zone7,fd);		
	
	val_def->time_zone8 = string_buf+ret;
	ret += get_profile_str("time_zone8=",val_def->time_zone8,fd);	
	
	val_def->time_zone9 = string_buf+ret;
	ret += get_profile_str("time_zone9=",val_def->time_zone9,fd);		
	
	val_def->time_zone10 = string_buf+ret;
	ret += get_profile_str("time_zone10=",val_def->time_zone10,fd);	
	
	val_def->time_zone11 = string_buf+ret;
	ret += get_profile_str("time_zone11=",val_def->time_zone11,fd);		
	
	val_def->time_zone12 = string_buf+ret;
	ret += get_profile_str("time_zone12=",val_def->time_zone12,fd);	
	
	val_def->time_zone13 = string_buf+ret;
	ret += get_profile_str("time_zone13=",val_def->time_zone13,fd);	
	
	val_def->time_zone14 = string_buf+ret;
	ret += get_profile_str("time_zone14=",val_def->time_zone14,fd);	
	
	val_def->time_zone15 = string_buf+ret;
	ret += get_profile_str("time_zone15=",val_def->time_zone15,fd);	
	
	val_def->time_zone16 = string_buf+ret;
	ret += get_profile_str("time_zone16=",val_def->time_zone16,fd);	
	
	val_def->time_zone17 = string_buf+ret;
	ret += get_profile_str("time_zone17=",val_def->time_zone17,fd);	
	
	val_def->time_zone18 = string_buf+ret;
	ret += get_profile_str("time_zone18=",val_def->time_zone18,fd);	
	
	val_def->time_zone19 = string_buf+ret;
	ret += get_profile_str("time_zone19=",val_def->time_zone19,fd);	
	
	val_def->time_zone20 = string_buf+ret;
	ret += get_profile_str("time_zone20=",val_def->time_zone20,fd);	
	
	val_def->time_zone21 = string_buf+ret;
	ret += get_profile_str("time_zone21=",val_def->time_zone21,fd);	
	
	val_def->time_zone22 = string_buf+ret;
	ret += get_profile_str("time_zone22=",val_def->time_zone22,fd);
	
	val_def->time_zone23 = string_buf+ret;
	ret += get_profile_str("time_zone23=",val_def->time_zone23,fd);		
	
	val_def->time_zone24 = string_buf+ret;
	ret += get_profile_str("time_zone24=",val_def->time_zone24,fd);		
	
	val_def->time_zone25 = string_buf+ret;
	ret += get_profile_str("time_zone25=",val_def->time_zone25,fd);		
	
	val_def->time_zone26 = string_buf+ret;
	ret += get_profile_str("time_zone26=",val_def->time_zone26,fd);	
	
	val_def->time_zone27 = string_buf+ret;
	ret += get_profile_str("time_zone27=",val_def->time_zone27,fd);		
		
	val_def->time_zone28 = string_buf+ret;
	ret += get_profile_str("time_zone28=",val_def->time_zone28,fd);		
	
	val_def->time_daysave_enable = string_buf+ret;
	ret += get_profile_str("time_daysave_enable=",val_def->time_daysave_enable,fd);		
	
	val_def->time_daysave_disable = string_buf+ret;
	ret += get_profile_str("time_daysave_disable=",val_def->time_daysave_disable,fd);	
	
	/*route*/
	val_def->pvcgateway_no = string_buf+ret;
	ret += get_profile_str("pvcgateway_no=",val_def->pvcgateway_no,fd);

	val_def->pvcgateway_yes = string_buf+ret;
	ret += get_profile_str("pvcgateway_yes=",val_def->pvcgateway_yes,fd);
	
	val_def->announcedinrip_yes = string_buf+ret;
	ret += get_profile_str("announcedinrip_yes=",val_def->announcedinrip_yes,fd);	

	val_def->announcedinrip_no = string_buf+ret;
	ret += get_profile_str("announcedinrip_no=",val_def->announcedinrip_no,fd);	

	
	/*vlan_group*/
	val_def->vlan_group_active_yes = string_buf+ret;
	ret += get_profile_str("vlan_group_active_yes=",val_def->vlan_group_active_yes,fd);	
	
	val_def->vlan_group_active_no = string_buf+ret;
	ret += get_profile_str("vlan_group_active_no=",val_def->vlan_group_active_no,fd);			
		
	/*adv_qos*/
	val_def->qos_activated = string_buf+ret;
	ret += get_profile_str("qos_activated=",val_def->qos_activated,fd);		
		
	val_def->qos_deactivated = string_buf+ret;
	ret += get_profile_str("qos_deactivated=",val_def->qos_deactivated,fd);	
	
	val_def->qos_8021_activated = string_buf+ret;
	ret += get_profile_str("qos_8021_activated=",val_def->qos_8021_activated,fd);		
		
	val_def->qos_8021_deactivated = string_buf+ret;
	ret += get_profile_str("qos_8021_deactivated=",val_def->qos_8021_deactivated,fd);			
	
	val_def->qos_ip_activated = string_buf+ret;
	ret += get_profile_str("qos_ip_activated=",val_def->qos_ip_activated,fd);		
		
	val_def->qos_ip_deactivated = string_buf+ret;
	ret += get_profile_str("qos_ip_deactivated=",val_def->qos_ip_deactivated,fd);	
	
	val_def->qos_iptype_tos = string_buf+ret;
	ret += get_profile_str("qos_iptype_tos=",val_def->qos_iptype_tos,fd);		
		
	val_def->qos_iptype_diffserv = string_buf+ret;
	ret += get_profile_str("qos_iptype_diffserv=",val_def->qos_iptype_diffserv,fd);
	
	val_def->qos_app_activated = string_buf+ret;
	ret += get_profile_str("qos_app_activated=",val_def->qos_app_activated,fd);		
		
	val_def->qos_app_deactivated = string_buf+ret;
	ret += get_profile_str("qos_app_deactivated=",val_def->qos_app_deactivated,fd);	
	
	val_def->qos_vlan_activated = string_buf+ret;
	ret += get_profile_str("qos_vlan_activated=",val_def->qos_vlan_activated,fd);		
		
	val_def->qos_vlan_deactivated = string_buf+ret;
	ret += get_profile_str("qos_vlan_deactivated=",val_def->qos_vlan_deactivated,fd);	
	
	val_def->vlan_top_activated = string_buf+ret;
	ret += get_profile_str("vlan_top_activated=",val_def->vlan_top_activated,fd);		
		
	val_def->vlan_top_deactivated = string_buf+ret;
	ret += get_profile_str("vlan_top_deactivated=",val_def->vlan_top_deactivated,fd);	
	
	val_def->update_rootfs = string_buf+ret;
	ret += get_profile_str("update_rootfs=",val_def->update_rootfs,fd);		
		
	val_def->update_kernal = string_buf+ret;
	ret += get_profile_str("update_kernal=",val_def->update_kernal,fd);

	/*wireless wkw 07.3.1*/
	val_def->wireless_access_activated							 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_access_activated=",val_def->wireless_access_activated,fd);                        
	                                                                                                                   
	val_def->wireless_access_deactivated						 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_access_deactivated=",val_def->wireless_access_deactivated,fd); 
	                   
	val_def->wireless_channel_albania                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_albania=",val_def->wireless_channel_albania,fd); 	 //wkw 4.18
	                                                                                                                  
	val_def->wireless_channel_algeria                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_algeria=",val_def->wireless_channel_algeria,fd);                          
	                                                                                                                   
	val_def->wireless_channel_argentina              = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_argentina=",val_def->wireless_channel_argentina,fd);                      
	                                                                                                                   
	val_def->wireless_channel_armenia                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_armenia=",val_def->wireless_channel_armenia,fd);                          
	                                                                                                                   
	val_def->wireless_channel_australia              = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_australia=",val_def->wireless_channel_australia,fd);                      
	                                                                                                                   
	val_def->wireless_channel_austria                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_austria=",val_def->wireless_channel_austria,fd);                          
	                                                                                                                   
	val_def->wireless_channel_azerbaijan             = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_azerbaijan=",val_def->wireless_channel_azerbaijan,fd);                    
	                                                                                                                   
	val_def->wireless_channel_bahrain                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_bahrain=",val_def->wireless_channel_bahrain,fd);                          
	                                                                                                                   
	val_def->wireless_channel_belarus                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_belarus=",val_def->wireless_channel_belarus,fd);                          
	                                                                                                                   
	val_def->wireless_channel_belgium                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_belgium=",val_def->wireless_channel_belgium,fd);                          
	                                                                                                                   
	val_def->wireless_channel_belize                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_belize=",val_def->wireless_channel_belize,fd);                            
	                                                                                                                   
	val_def->wireless_channel_bolvia                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_bolvia=",val_def->wireless_channel_bolvia,fd);                            
	                                                                                                                   
	val_def->wireless_channel_brazil                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_brazil=",val_def->wireless_channel_brazil,fd);                            
	                                                                                                                   
	val_def->wireless_channel_brunei_darussalam      = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_brunei_darussalam=",val_def->wireless_channel_brunei_darussalam,fd);      
	                                                                                                                   
	val_def->wireless_channel_bulgaria               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_bulgaria=",val_def->wireless_channel_bulgaria,fd);                        
	                                                                                                                   
	val_def->wireless_channel_canada                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_canada=",val_def->wireless_channel_canada,fd);                            
	                                                                                                                   
	val_def->wireless_channel_chile                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_chile=",val_def->wireless_channel_chile,fd);                              
	                                                                                                                   
	val_def->wireless_channel_china                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_china=",val_def->wireless_channel_china,fd);                              
	                                                                                                                   
	val_def->wireless_channel_colombia               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_colombia=",val_def->wireless_channel_colombia,fd);                        
	                                                                                                                   
	val_def->wireless_channel_costa_rica             = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_costa_rica=",val_def->wireless_channel_costa_rica,fd);                    
	                                                                                                                   
	val_def->wireless_channel_croatia                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_croatia=",val_def->wireless_channel_croatia,fd);                          
	                                                                                                                   
	val_def->wireless_channel_cyprus                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_cyprus=",val_def->wireless_channel_cyprus,fd);                            
	                                                                                                                   
	val_def->wireless_channel_czch_republic          = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_czch_republic=",val_def->wireless_channel_czch_republic,fd);              
	                                                                                                                   
	val_def->wireless_channel_denmark                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_denmark=",val_def->wireless_channel_denmark,fd);                          
	                                                                                                                   
	val_def->wireless_channel_dominicanrepublic      = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_dominicanrepublic=",val_def->wireless_channel_dominicanrepublic,fd);      
	                                                                                                                   
	val_def->wireless_channel_ecuador                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_ecuador=",val_def->wireless_channel_ecuador,fd);                          
	                                                                                                                   
	val_def->wireless_channel_egypt                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_egypt=",val_def->wireless_channel_egypt,fd);                              
	                                                                                                                   
	val_def->wireless_channel_elsalvador             = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_elsalvador=",val_def->wireless_channel_elsalvador,fd);                    
	                                                                                                                   
	val_def->wireless_channel_estonia                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_estonia=",val_def->wireless_channel_estonia,fd);                          
	                                                                                                                   
	val_def->wireless_channel_finland                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_finland=",val_def->wireless_channel_finland,fd);                          
	                                                                                                                   
	val_def->wireless_channel_france                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_france=",val_def->wireless_channel_france,fd);                            
	                                                                                                                   
	val_def->wireless_channel_georgia                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_georgia=",val_def->wireless_channel_georgia,fd);                          
	                                                                                                                   
	val_def->wireless_channel_germany                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_germany=",val_def->wireless_channel_germany,fd);                          
	                                                                                                                   
	val_def->wireless_channel_greece                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_greece=",val_def->wireless_channel_greece,fd);                            
	                                                                                                                   
	val_def->wireless_channel_guatemala              = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_guatemala=",val_def->wireless_channel_guatemala,fd);                      
	                                                                                                                   
	val_def->wireless_channel_honduras               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_honduras=",val_def->wireless_channel_honduras,fd);                        
	                                                                                                                   
	val_def->wireless_channel_hongkong               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_hongkong=",val_def->wireless_channel_hongkong,fd);                        
	                                                                                                                   
	val_def->wireless_channel_hungary                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_hungary=",val_def->wireless_channel_hungary,fd);                          
	                                                                                                                   
	val_def->wireless_channel_iceland                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_iceland=",val_def->wireless_channel_iceland,fd);                          
	                                                                                                                   
	val_def->wireless_channel_india                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_india=",val_def->wireless_channel_india,fd);                              
	                                                                                                                   
	val_def->wireless_channel_indonesia              = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_indonesia=",val_def->wireless_channel_indonesia,fd);                      
	                                                                                                                   
	val_def->wireless_channel_iran                   = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_iran=",val_def->wireless_channel_iran,fd);                                
	                                                                                                                   
	val_def->wireless_channel_ireland                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_ireland=",val_def->wireless_channel_ireland,fd);                          
	                                                                                                                   
	val_def->wireless_channel_isarel                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_isarel=",val_def->wireless_channel_isarel,fd);                            
	                                                                                                                   
	val_def->wireless_channel_italy                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_italy=",val_def->wireless_channel_italy,fd);                              
	                                                                                                                   
	val_def->wireless_channel_japan                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_japan=",val_def->wireless_channel_japan,fd);                              
	                                                                                                                   
	val_def->wireless_channel_jordan                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_jordan=",val_def->wireless_channel_jordan,fd);                            
	                                                                                                                   
	val_def->wireless_channel_kazakhstan             = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_kazakhstan=",val_def->wireless_channel_kazakhstan,fd);                    
	                                                                                                                   
	val_def->wireless_channel_northkorea        	   = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_northkorea=",val_def->wireless_channel_northkorea,fd);                    
	                                                                                                                   
	val_def->wireless_channel_korearepublic     	   = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_korearepublic=",val_def->wireless_channel_korearepublic,fd);              
	                                                                                                                   
	val_def->wireless_channel_kuwait                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_kuwait=",val_def->wireless_channel_kuwait,fd);                            
	                                                                                                                   
	val_def->wireless_channel_latvia                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_latvia=",val_def->wireless_channel_latvia,fd);                            
	                                                                                                                   
	val_def->wireless_channel_lebanon                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_lebanon=",val_def->wireless_channel_lebanon,fd);                          
	                                                                                                                   
	val_def->wireless_channel_liechtenstein          = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_liechtenstein=",val_def->wireless_channel_liechtenstein,fd);              
	                                                                                                                   
	val_def->wireless_channel_lithuania              = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_lithuania=",val_def->wireless_channel_lithuania,fd);                      
	                                                                                                                   
	val_def->wireless_channel_luxembourg             = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_luxembourg=",val_def->wireless_channel_luxembourg,fd);                    
	                                                                                                                   
	val_def->wireless_channel_macau                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_macau=",val_def->wireless_channel_macau,fd);                              
	                                                                                                                   
	val_def->wireless_channel_macedonia              = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_macedonia=",val_def->wireless_channel_macedonia,fd);                      
	                                                                                                                   
	val_def->wireless_channel_malaysia               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_malaysia=",val_def->wireless_channel_malaysia,fd);                        
	                                                                                                                   
	val_def->wireless_channel_mexico                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_mexico=",val_def->wireless_channel_mexico,fd);                            
	                                                                                                                   
	val_def->wireless_channel_monaco                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_monaco=",val_def->wireless_channel_monaco,fd);                            
	                                                                                                                   
	val_def->wireless_channel_morocco                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_morocco=",val_def->wireless_channel_morocco,fd);                          
	                                                                                                                   
	val_def->wireless_channel_netherlands            = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_netherlands=",val_def->wireless_channel_netherlands,fd);                  
	                                                                                                                   
	val_def->wireless_channel_new_zealand            = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_new_zealand=",val_def->wireless_channel_new_zealand,fd);                  
	                                                                                                                   
	val_def->wireless_channel_norway                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_norway=",val_def->wireless_channel_norway,fd);                            
	                                                                                                                   
	val_def->wireless_channel_oman                   = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_oman=",val_def->wireless_channel_oman,fd);                                
	                                                                                                                   
	val_def->wireless_channel_pakistan               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_pakistan=",val_def->wireless_channel_pakistan,fd);                        
	                                                                                                                   
	val_def->wireless_channel_panama                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_panama=",val_def->wireless_channel_panama,fd);                            
	                                                                                                                   
	val_def->wireless_channel_peru                   = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_peru=",val_def->wireless_channel_peru,fd);                                
	                                                                                                                   
	val_def->wireless_channel_philippines            = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_philippines=",val_def->wireless_channel_philippines,fd);                  
	                                                                                                                   
	val_def->wireless_channel_poland                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_poland=",val_def->wireless_channel_poland,fd);                            
	                                                                                                                   
	val_def->wireless_channel_portugal               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_portugal=",val_def->wireless_channel_portugal,fd);                        
	                                                                                                                   
	val_def->wireless_channel_puerto_rico            = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_puerto_rico=",val_def->wireless_channel_puerto_rico,fd);                  
	                                                                                                                   
	val_def->wireless_channel_qatar                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_qatar=",val_def->wireless_channel_qatar,fd);                              
	                                                                                                                   
	val_def->wireless_channel_romania                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_romania=",val_def->wireless_channel_romania,fd);                          
	                                                                                                                   
	val_def->wireless_channel_russia                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_russia=",val_def->wireless_channel_russia,fd);                            
	                                                                                                                   
	val_def->wireless_channel_saudi_arabia           = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_saudi_arabia=",val_def->wireless_channel_saudi_arabia,fd);                
	                                                                                                                   
	val_def->wireless_channel_singapore              = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_singapore=",val_def->wireless_channel_singapore,fd);                      
	                                                                                                                   
	val_def->wireless_channel_slovakia               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_slovakia=",val_def->wireless_channel_slovakia,fd);    
	                    
	val_def->wireless_channel_slovenia               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_slovenia=",val_def->wireless_channel_slovenia,fd);                        
	                                                                                                                   
	val_def->wireless_channel_south_africa           = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_south_africa=",val_def->wireless_channel_south_africa,fd);                
	                                                                                                                   
	val_def->wireless_channel_spain                  = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_spain=",val_def->wireless_channel_spain,fd);                              
	                                                                                                                   
	val_def->wireless_channel_swedan                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_swedan=",val_def->wireless_channel_swedan,fd);                            
	                                                                                                                   
	val_def->wireless_channel_switzerland            = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_switzerland=",val_def->wireless_channel_switzerland,fd);                  
	                                                                                                                   
	val_def->wireless_channel_syriaarabrepublic  	   = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_syriaarabrepublic=",val_def->wireless_channel_syriaarabrepublic,fd);      

	val_def->wireless_channel_taiwan                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_taiwan=",val_def->wireless_channel_taiwan,fd);                            
	                                                                                                                   
	val_def->wireless_channel_thailand               = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_thailand=",val_def->wireless_channel_thailand,fd);                        
	                                                                                                                   
	val_def->wireless_channel_trianidadandtobago 	   = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_trianidadandtobago=",val_def->wireless_channel_trianidadandtobago,fd);    
	                                                                                                                   
	val_def->wireless_channel_tunisia                = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_tunisia=",val_def->wireless_channel_tunisia,fd);                          
	                                                                                                                   
	val_def->wireless_channel_turkey                 = string_buf+ret;                                                 
	ret += get_profile_str("wireless_channel_turkey=",val_def->wireless_channel_turkey,fd);                            
	                                                                                                                   
	  val_def->wireless_channel_ukraine                = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_ukraine=",val_def->wireless_channel_ukraine,fd);                          
	                                                                                                                     
	  val_def->wireless_channel_united_arab_emirates   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_united_arab_emirates=",val_def->wireless_channel_united_arab_emirates,fd);
	                                                                                                                     
	  val_def->wireless_channel_united_kingdom         = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_united_kingdom=",val_def->wireless_channel_united_kingdom,fd);            
	                                                                                                                     
	  val_def->wireless_channel_unitedstates        	 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_unitedstates=",val_def->wireless_channel_unitedstates,fd);                
	                                                                                                                     
	  val_def->wireless_channel_uruguay                = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_uruguay=",val_def->wireless_channel_uruguay,fd);                          
	                                                                                                                     
	  val_def->wireless_channel_uzbekistan             = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_uzbekistan=",val_def->wireless_channel_uzbekistan,fd);                    
	                                                                                                                     
	  val_def->wireless_channel_venezuela              = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_venezuela=",val_def->wireless_channel_venezuela,fd);                      
	                                                                                                                     
	  val_def->wireless_channel_vietnam                = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_vietnam=",val_def->wireless_channel_vietnam,fd);                          
	                                                                                                                     
	  val_def->wireless_channel_yemen                  = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_yemen=",val_def->wireless_channel_yemen,fd);                              
	                                                                                                                     
	  val_def->wireless_channel_zimbabwe               = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_zimbabwe=",val_def->wireless_channel_zimbabwe,fd);                        
	                                                                                                                     
	  val_def->wireless_channel_undefined              = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_undefined=",val_def->wireless_channel_undefined,fd);                      
	                                                                                                                     
	  val_def->wireless_channel_auto			             = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_channel_auto=",val_def->wireless_channel_auto,fd);                                
	                                                                                                                     
	  val_def->wireless_80211bg_80211b		             = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_80211bg_80211b=",val_def->wireless_80211bg_80211b,fd);                            
	                                                                                                                     
	  val_def->wireless_80211bg_80211g		             = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_80211bg_80211g=",val_def->wireless_80211bg_80211g,fd);                            
	                                                                                                                     
	  val_def->wireless_80211bg_80211bg	               = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_80211bg_80211bg=",val_def->wireless_80211bg_80211bg,fd);                          
	                                                                                                                     
	  val_def->wireless_broadcast_yes		               = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_broadcast_yes=",val_def->wireless_broadcast_yes,fd);                              
	                                                                                                                     
	  val_def->wireless_broadcast_no			             = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_broadcast_no=",val_def->wireless_broadcast_no,fd);                                
	
	  val_def->wireless_authentication_disabled			             = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_authentication_disabled=",val_def->wireless_authentication_disabled,fd);   
	                                                                                                                      
	  val_def->wireless_authentication_wep64bits   	   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_authentication_wep64bits=",val_def->wireless_authentication_wep64bits,fd);        
	                                                                                                                     
	  val_def->wireless_authentication_wep128bits  	   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_authentication_wep128bits=",val_def->wireless_authentication_wep128bits,fd);      
	                                                                                                                     
	  val_def->wireless_authentication_wpapsk      	   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_authentication_wpapsk=",val_def->wireless_authentication_wpapsk,fd);              
	                                                                                                                     
	  val_def->wireless_authentication_wpa2psk     	   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_authentication_wpa2psk=",val_def->wireless_authentication_wpa2psk,fd);            
	                                                                                                                     
	  val_def->wireless_active_activated      				 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_active_activated=",val_def->wireless_active_activated,fd);                        
	                                                                                                                     
	  val_def->wireless_active_deactivated      			 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_active_deactivated=",val_def->wireless_active_deactivated,fd);                    
	                                                                                                                     
	  val_def->wireless_action_allow 			    			   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_action_allow=",val_def->wireless_action_allow,fd);                                
	                                                                                                                     
	  val_def->wireless_action_deny	 			    			   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_action_deny=",val_def->wireless_action_deny,fd);                                  
	                                                                                                                     
	  val_def->wireless_key1	 			    							 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_key1=",val_def->wireless_key1,fd);                                                
	                                                                                                                     
	  val_def->wireless_key2	 			    							 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_key2=",val_def->wireless_key2,fd);                                                
	                                                                                                                     
	  val_def->wireless_key3	 			    							 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_key3=",val_def->wireless_key3,fd);                                                
	                                                                                                                     
	  val_def->wireless_key4	 			    							 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_key4=",val_def->wireless_key4,fd);                                                
	                                                                                                                     
	  val_def->wireless_encryption_tkip 			    		 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_encryption_tkip=",val_def->wireless_encryption_tkip,fd);                          
	                                                                                                                     
	  val_def->wireless_encryption_aes 	 			    	   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_encryption_aes=",val_def->wireless_encryption_aes,fd); 
	  
	  val_def->wireless_default_key64	 			    							 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_default_key64=",val_def->wireless_default_key64,fd);                                                
	                                                                                                                     
	  val_def->wireless_default_key128 			    		 = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_default_key128=",val_def->wireless_default_key128,fd);                          
	                                                                                                                     
	  val_def->wireless_default_mac 	 			    	   = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_default_mac=",val_def->wireless_default_mac,fd);  
	
	  val_def->wireless_default_SSID		               = string_buf+ret;                                                 
	  ret += get_profile_str("wireless_default_SSID=",val_def->wireless_default_SSID,fd);    
	                              
	//wireless end    
//wtw add begin
	/*ipfilter extend wkw 3.6*/
	val_def->ipfilter_type_ipmac = string_buf+ret;
	ret += get_profile_str("ipfilter_type_ipmac=",val_def->ipfilter_type_ipmac,fd);	
		
	val_def->ipfilter_type_application = string_buf+ret;
	ret += get_profile_str("ipfilter_type_application=",val_def->ipfilter_type_application,fd);	
	
	val_def->ipfilter_type_url = string_buf+ret;
	ret += get_profile_str("ipfilter_type_url=",val_def->ipfilter_type_url,fd);	
	
	val_def->ipfilter_rule_ip = string_buf+ret;
	ret += get_profile_str("ipfilter_rule_ip=",val_def->ipfilter_rule_ip,fd);	
	
	val_def->ipfilter_rule_mac = string_buf+ret;
	ret += get_profile_str("ipfilter_rule_mac=",val_def->ipfilter_rule_mac,fd);			
	
	val_def->urlfilter_activated = string_buf+ret;              //wjx 2007.3.6
	ret += get_profile_str("urlfilter_activated=",val_def->urlfilter_activated,fd);	
	
	val_def->urlfilter_deactivated = string_buf+ret;             //wjx 2007.3.6
	ret += get_profile_str("urlfilter_deactivated=",val_def->urlfilter_deactivated,fd);

/*  ipfilter_app begin wjx 2007.3.7 */	
	val_def->appfilter_activated = string_buf+ret;              
	ret += get_profile_str("appfilter_activated=",val_def->appfilter_activated,fd);	
		
	val_def->appfilter_deactivated = string_buf+ret;
	ret += get_profile_str("appfilter_deactivated=",val_def->appfilter_deactivated,fd);	
	
	val_def->appfilter_icq_allow= string_buf+ret;            
	ret += get_profile_str("appfilter_icq_allow=",val_def->appfilter_icq_allow,fd);	
		
	val_def->appfilter_icq_deny = string_buf+ret;
	ret += get_profile_str("appfilter_icq_deny=",val_def->appfilter_icq_deny,fd);		
	
	val_def->appfilter_msn_allow = string_buf+ret;           
	ret += get_profile_str("appfilter_msn_allow=",val_def->appfilter_msn_allow,fd);	
		
	val_def->appfilter_msn_deny = string_buf+ret;
	ret += get_profile_str("appfilter_msn_deny=",val_def->appfilter_msn_deny,fd);		
	
	val_def->appfilter_ymsn_allow = string_buf+ret;           
	ret += get_profile_str("appfilter_ymsn_allow=",val_def->appfilter_ymsn_allow,fd);	
		
	val_def->appfilter_ymsn_deny = string_buf+ret;
	ret += get_profile_str("appfilter_ymsn_deny=",val_def->appfilter_ymsn_deny,fd);		
	
	val_def->appfilter_ra_allow = string_buf+ret;            
	ret += get_profile_str("appfilter_ra_allow=",val_def->appfilter_ra_allow,fd);	
		
	val_def->appfilter_ra_deny = string_buf+ret;
	ret += get_profile_str("appfilter_ra_deny=",val_def->appfilter_ra_deny,fd);		
	
/*  ipfilter_app end wjx 2007.3.7 */				
	
	/*ipfilter extend end*/

//wtw add end	
/*adv_firewall begin wjx 2007.3.5*/
	val_def->fw_firewall_enable = string_buf+ret;
	ret += get_profile_str("fw_firewall_enable=",val_def->fw_firewall_enable,fd);
	
	val_def->fw_firewall_disable = string_buf+ret;
	ret += get_profile_str("fw_firewall_disable=",val_def->fw_firewall_disable,fd);	
	
	val_def->fw_spi_enable = string_buf+ret;
	ret += get_profile_str("fw_spi_enable=",val_def->fw_spi_enable,fd);		
	
	val_def->fw_spi_disable = string_buf+ret;
	ret += get_profile_str("fw_spi_disable=",val_def->fw_spi_disable,fd);		
			
/*adv_firewall end 2007.3.5*/		
	fprintf(stderr,"----total string buf use:%d\n",ret);
	err = 0;
	
read_value_define_err:	
	if(fd != -1)
		close(fd);	
	
	return err;
}

char* config_get_value(char *src,char*dst,int size,char *item)
{
	char *start,*enter;
	int len;

	dst[0] = '\0';
	start = strstr(src,item);
	if (start == NULL)
		return dst;
	
	start += strlen(item) + 1;
	enter = strchr(start,'\n');
#if 0	
	if (enter == NULL)
	{
		strncpy(dst,start,size-1);
		dst[size-1] = '\0';
		fprintf(stderr,"---- %s %s%s\n",__FUNCTION__,item,dst);
		return dst;
	}
#endif	
	enter--;
	
	len = enter-start;
	if(len >= size)
		len = size-1;
	strncpy(dst,start,len);
	dst[len] = '\0';
	fprintf(stderr,"---- %s %s%s\n",__FUNCTION__,item,dst);
	return dst;
	
}

char *readall(char *arg)
{
	int fd;
	char *stream=NULL;
	struct stat buf;
	int ret;
	
	fd = open(arg,O_RDONLY);
	if ( fd!= -1)
	{
		ret = fstat(fd,&buf);
		if(ret == 0 && buf.st_size)
		{
			stream = (char*)malloc(buf.st_size);
			read(fd,stream,buf.st_size);
		}
		close(fd);
	}	
	return stream;
}

char *boa_readline(int fd)
{

	char *str_enter;
	char stream[128];
	char *str_return=NULL;
	int len,sub_len,offset=0,ret;
	off_t pos;
	struct stat st;

	ret = fstat(fd,&st);
	if(ret == -1)
		return NULL;

	pos = lseek(fd,0,SEEK_CUR);
	if(pos == -1L)
			return NULL;
	memset(stream,0,128);
	while((len=read(fd,stream,16))>0)
	{
		str_enter = strchr(stream,'\n');
		if(str_enter ==NULL)
		{
			offset +=len;
			continue;
		}
		else
		{
			sub_len = str_enter-stream;
			offset +=sub_len;
			break;
		}
				
	}
	if(!offset)
		return NULL;
		
	lseek(fd,pos,SEEK_SET);
#if 0
	if(pos == -1L)
		return NULL;
#endif	
	str_return = (char *)malloc(offset+1);
	memset(str_return,0,offset+1);
	ret = read(fd,str_return,offset);
	
	if(ret<0)
	{
		free(str_return);
		return NULL;
	}
	else
		lseek(fd,pos+offset+1,SEEK_SET);
#if 0	
	if((pos+offset+1) >= st.st_size )
	{
		free(str_return);
		return NULL;
	}
#endif	
	return str_return;
	
}

void boa_execute(char *arg)
{
	int pid,status,child_pid;
	
	pid = vfork();
	if(pid == -1)
		fprintf(stderr,"boa_execute:%s\n",arg);
	else if(pid == 0)
	{	
		execlp("/bin/sh","/bin/sh","-c",arg,(char*)0);
		fprintf(stderr,"boa_execute:%s\n",arg);
		_exit(1);
	}
	else
	{
		child_pid = wait(&status);
	}

}

void read_internet_settings()
{
	char filename[128],tmp[128];
	int fd;
	struct stat buf;
	int i,ret;
	
	for(i = 0; i < 9; i++)
	{
		char *stream = NULL;

		web_api->wan[i] = (INTERNET_SETTINGS *)malloc(sizeof(INTERNET_SETTINGS));
		memset(web_api->wan[i],0,sizeof(INTERNET_SETTINGS));
		
		sprintf(filename,"/etc/isp%d.conf",i);
		fd = open(filename,O_RDONLY);
		if (fd == -1)
		{
			strncpy(web_api->wan[i]->status,val_def->wan_deactivated,16);
			goto read_internet_settings_err;
		}
		
		strncpy(web_api->wan[i]->status,val_def->wan_activated,16);
		ret = fstat(fd,&buf);
		if((ret == -1) || (buf.st_size==0))
			goto read_internet_settings_err;
		
		stream = (char*)malloc(buf.st_size);
		ret = read(fd,stream,buf.st_size);
		if(ret <=0)
			goto read_internet_settings_err;

		if(strncmp(config_get_value(stream,tmp,16,"ACTIVE="),"No",2) == 0)
		{
			strncpy(web_api->wan[i]->status,val_def->wan_deactivated,16);
		}
		else
		{
			strncpy(web_api->wan[i]->status,val_def->wan_activated,16);
		}
		web_api->wan[i]->vpi = atol(config_get_value(stream,tmp,128,"VPI="));
		web_api->wan[i]->vci = atol(config_get_value(stream,tmp,128,"VCI="));
		config_get_value(stream,web_api->wan[i]->qos,16,"QOS=");
		web_api->wan[i]->pcr = atol(config_get_value(stream,tmp,128,"PCR="));
		web_api->wan[i]->scr = atol(config_get_value(stream,tmp,128,"SCR="));
		web_api->wan[i]->mbs = atol(config_get_value(stream,tmp,128,"MBS="));
		
		config_get_value(stream,web_api->wan[i]->isp,16,"ISP=");
		config_get_value(stream,web_api->wan[i]->encap,32,"ENCAP=");

		config_get_value(stream,web_api->wan[i]->nat_enable,16,"NATENABLE=");
		config_get_value(stream,web_api->wan[i]->def_route,16,"DEFAULTROUTE=");
		web_api->wan[i]->mtu = atol(config_get_value(stream,tmp,128,"MTU="));
		config_get_value(stream,web_api->wan[i]->rip_ver,16,"RIPVERSION=");
		config_get_value(stream,web_api->wan[i]->direction,16,"DIRECTION=");			
		config_get_value(stream,web_api->wan[i]->igmp,16,"IGMP=");
		
		config_get_value(stream,web_api->wan[i]->ip_addr,16,"IPADDR=");
		config_get_value(stream,web_api->wan[i]->netmask,16,"NETMASK=");
		config_get_value(stream,web_api->wan[i]->gateway,16,"GATEWAY=");
		
		config_get_value(stream,web_api->wan[i]->usr_name,128,"USERNAME=");
		config_get_value(stream,web_api->wan[i]->passwd,128,"PASSWORD=");
		config_get_value(stream,web_api->wan[i]->connection,32,"CONNECTION=");
		web_api->wan[i]->closeifidle = atol(config_get_value(stream,tmp,128,"CLOSEIFIDLE="));
		web_api->wan[i]->mss = atol(config_get_value(stream,tmp,128,"MSS="));
		web_api->wan[i]->last_mss = web_api->wan[i]->mss;
		config_get_value(stream,web_api->wan[i]->ppp_get_ip,16,"PPPGETIP=");
		
read_internet_settings_err:	
		if(fd!=-1)
			close(fd);
		if(stream)
			free(stream);

		
	}
	

}

void read_lan_settings()
{
	char *stream=NULL;
	struct stat buf;
	int i;
	FILE *fd=NULL;
	char dhcpRelayInfo[4][32];
	
	for(i=0; i<2; i++)
	{
		web_api->lan[i] = (LAN_SETTINGS*)malloc(sizeof(LAN_SETTINGS));
		//memset(web_api->lan[i],0,sizeof(LAN_SETTINGS));
	}
	
	read_eth_info();
#if 0
	/*ip & mask*/
	fd = fopen("/etc/lanconfig.sh", "r");
	if(fd)
	{
		char line[100], *sub3;

		fgets(line, 100, fd);
		fclose(fd);

		sub1 = strstr(line, "br0");
		sub2 = strstr(sub1, "netmask");
		sub3 = strstr(sub2, "up");
		strncpy(web_api->lan[0]->ip_addr, sub1 + 4, sub2 - sub1 - 5);
		strncpy(web_api->lan[0]->netmask, sub2 + 8, sub3 - sub2 - 9);
		
	}
	else
	{
		sprintf(arg,"/sbin/ifconfig br0 >/tmp/br0.log");
		boa_execute(arg);
		stream = readall("/tmp/br0.log");
		if(stream)
		{
			sub1 = strstr(stream,"inet addr:");
			sub2 = strstr(stream,"Bcast:");
			if(sub1!=NULL || sub2!=NULL)
				strncpy(web_api->lan[0]->ip_addr,sub1+10,sub2-sub1-12);
			
			sub1 = strstr(stream,"Mask:");
			if(sub1)
				sub2 = strchr(sub1,'\n');
			if(sub1 && sub2)
				strncpy(web_api->lan[0]->netmask,sub1+5,sub2-sub1-5);
			
			sub1 = strstr(stream,"HWaddr");
			if(sub1)
				sub2 = strchr(sub1,'\n');
			if(sub1 && sub2)
				strncpy(web_api->lan[0]->mac_addr,sub1+7,17);
			
			free(stream);
			fprintf(stderr,"----%s,%s,%s\n",web_api->lan[0]->ip_addr,web_api->lan[0]->netmask,web_api->lan[0]->mac_addr);
		}
	}
#endif
#if 0
	/*rip ver & direction & igmp*/
	strncpy(web_api->lan[0]->rip_ver,val_def->wan_rip_disable,16);
	strncpy(web_api->lan[0]->direction,val_def->wan_direction_none,16);
	strncpy(web_api->lan[0]->igmp,val_def->wan_igmpdisable,16);
	
	stream = readall("/etc/gated.conf");
	if(stream)
	{
	  /*rip*/
		if(strstr(stream,"rip yes"))
			strncpy(web_api->lan[0]->rip_ver,val_def->wan_rip1,16);
		else if(strstr(stream,"version 2") && strstr(stream,"multicast")) 
			strncpy(web_api->lan[0]->rip_ver,val_def->wan_rip2m,16);
		else if(strstr(stream,"version 2"))
			strncpy(web_api->lan[0]->rip_ver,val_def->wan_rip2b,16);
	  /*direction*/	
		if(strstr(stream,"ripin") && strstr(stream,"ripout"))
			strncpy(web_api->lan[0]->direction,val_def->wan_direction_both,16);
		else if(strstr(stream,"noripin") && strstr(stream,"noripout"))
			strncpy(web_api->lan[0]->direction,val_def->wan_direction_none,16);
		else if(strstr(stream,"ripin"))
			strncpy(web_api->lan[0]->direction,val_def->wan_direction_in,16);
		else if(strstr(stream,"ripout"))
			strncpy(web_api->lan[0]->direction,val_def->wan_direction_out,16);
		/*igmp*/
		if(strstr(stream,"IGMP on"))
			strncpy(web_api->lan[0]->igmp,val_def->wan_igmpv1,16);
		else if(strstr(stream,"IGMP yes"))
			strncpy(web_api->lan[0]->igmp,val_def->wan_igmpv2,16);
		
		free(stream);
	}
#endif

	/*rip ver & direction & igmp*/
	stream = readall("/etc/lan_rip.conf");
	if(stream)
	{
		/*rip*/
		config_get_value(stream,web_api->lan[0]->rip_ver,16,"RIPVERSION=");
		/*direction*/
		config_get_value(stream,web_api->lan[0]->direction,16,"DIRECTION=");
	  
		free(stream);
	}
	else
	{
		strncpy(web_api->lan[0]->rip_ver, val_def->wan_rip1, 16);
		strncpy(web_api->lan[0]->direction, val_def->wan_direction_none, 16);
	}

	/*dhcp & dns*/
	if(stat("/etc/udhcpd.conf",&buf)== 0)
		strncpy(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_enable,16);
	else if(stat("/etc/config/dhcprelay.sh",&buf)== 0)
		strncpy(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_relay,16);
	else
		strncpy(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_disable,16);

	fd = fopen("/etc/udhcpd.conf", "r");
	if(fd)
	{
		#if 0 /*here_20080129*/
		char starttemp[32], endtemp[32], temp[16];
		unsigned char startip, endip;
		
		fgets(starttemp, 100, fd);
		fgets(endtemp, 100, fd);
		sprintf(web_api->lan[0]->dhcp_startip, "%s", starttemp + 6);
		web_api->lan[0]->dhcp_startip[strlen(web_api->lan[0]->dhcp_startip) - 2] = '\0';
		sprintf(temp, "%s", endtemp + 4);
		startip = atoi(rindex(web_api->lan[0]->dhcp_startip, '.') + 1);
		endip = atoi(rindex(temp, '.') + 1);
		sprintf(web_api->lan[0]->dhcp_poolsize, "%d", endip - startip + 1);
		for(i = 0; i < 7; i ++)
			fgets(starttemp, 100, fd);
		fclose(fd);
		sprintf(web_api->lan[0]->dhcp_leasetime, "%s", starttemp + 13);
		web_api->lan[0]->dhcp_leasetime[strlen(web_api->lan[0]->dhcp_leasetime) - 2] = '\0';
		#endif
		char starttemp[32];
		char dhcp_info[3][32];
		char conf_endIp[16];
		int startIp=0, endIp=0;
		
		char temp[64];
		
		while(fgets(temp,64,fd)){
			memset(dhcp_info,0,sizeof(dhcp_info));
			sscanf(temp,"%s %s %s",dhcp_info[0],dhcp_info[1],dhcp_info[2]);
			if(strcmp(dhcp_info[0],"start") == 0){
				strcpy(web_api->lan[0]->dhcp_startip,dhcp_info[1]);
			}
			else if(strcmp(dhcp_info[0],"end") == 0){
				strcpy(conf_endIp,dhcp_info[1]);
			}else if(strcmp(dhcp_info[0],"option") == 0){
				strcpy(web_api->lan[0]->dhcp_leasetime,dhcp_info[2]);
			}
		}
		
		startIp = atoi(rindex(web_api->lan[0]->dhcp_startip, '.') + 1);
		endIp = atoi(rindex(conf_endIp, '.') + 1);
		sprintf(web_api->lan[0]->dhcp_poolsize,"%d", (endIp-startIp+1));
		fclose(fd);
		fd = fopen("/etc/dproxy.auto", "r");
		if(fd)
		{
			fgets(temp, 64, fd);
			if(atoi(temp))
				sprintf(web_api->lan[0]->dns_type, "%s", val_def->lan_dns_directory);
			else
				sprintf(web_api->lan[0]->dns_type, "%s", val_def->lan_dns_relay);
			fclose(fd);
			fd = fopen("/etc/dproxy.conf", "r");
			if(fd)
			{
				for(i = 0; i < 13; i++)
				{
					fgets(starttemp, 100, fd);
					if(i == 5)
						sprintf(web_api->lan[0]->dns_primary, "%s", starttemp + 14);
					else if(i == 12)
						sprintf(web_api->lan[0]->dns_second, "%s", starttemp + 14);
				}
				fclose(fd);
			}
		}
	}

	/*Read dhcp realy information*/
	stream = readall("/etc/config/dhcprelay.sh");
	if(stream){
		sscanf(stream, "%s %s %s %s", 
		dhcpRelayInfo[0], dhcpRelayInfo[1], dhcpRelayInfo[2], dhcpRelayInfo[3]);
		/*server ip  stream =/bin/dhcrelay -pf /var/log/dhcrelay.pid 10.10.10.100 */
		strcpy(web_api->lan[0]->dhcp_serverip, dhcpRelayInfo[3]);
	}

	#if 0
	/*server ip*/
	stream = readall("/etc/config/dhcprelay.sh");
	if(stream){
		if((sub1 = strstr(stream,"/bin/dhcrelay"))){
			if((sub2 = strrchr(sub1,0x20))){
					strncpy(web_api->lan[0]->dhcp_serverip,sub2+1,16);
			}	
		}
}
	#endif
}/*end read_lan_settings*/
/*IP, MASK, MAC*/
void read_eth_info()
{
	char arg[64];
	char *stream=NULL, *sub1, *sub2;;
	int i = 0;
	
	for(i=0; i < 2; i++)
	{	
		memset(web_api->lan[i]->ip_addr, 0, 16);
		memset(web_api->lan[i]->netmask, 0, 16);
		memset(web_api->lan[i]->mac_addr, 0, 18);
	}	
	
	sprintf(arg,"/sbin/ifconfig br0 > /tmp/br0.log");
	boa_execute(arg);
	stream = readall("/tmp/br0.log");
	if(stream)
	{
		sub1 = strstr(stream,"inet addr:");
		sub2 = strstr(stream,"Bcast:");
		if(sub1!=NULL || sub2!=NULL)
			strncpy(web_api->lan[0]->ip_addr,sub1+10,sub2-sub1-12);
		
		sub1 = strstr(stream,"Mask:");
		if(sub1)
			sub2 = strchr(sub1,'\n');
		if(sub1 && sub2)
			strncpy(web_api->lan[0]->netmask,sub1+5,sub2-sub1-5);
		
		sub1 = strstr(stream,"HWaddr");
		if(sub1)
			sub2 = strchr(sub1,'\n');
		if(sub1 && sub2)
			strncpy(web_api->lan[0]->mac_addr,sub1+7,17);
		
		free(stream);
	}
	
}

void read_snmp_commu()
{
  	int fd;
	int ret, i;
	char *content;
	struct stat buf;
	
	web_api->access_snmp=(ACCESS_SNMP*)malloc(sizeof(ACCESS_SNMP));
  	memset(web_api->access_snmp,0,sizeof(ACCESS_SNMP));
  	
	fd = open(SNMPD_STAT_CONF,O_RDONLY);
	if(fd == -1){
		goto err;
	}
	close(fd);
	
	fd = open("/etc/snmp/snmpd.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);	
	ret = read(fd,content,buf.st_size);
	if (ret <= 0)
	{	
		free(content);
		goto err;
	}
	config_get_value(content,web_api->access_snmp->rocommunity,32,"rocommunity ");	
	config_get_value(content,web_api->access_snmp->rwcommunity,32,"rwcommunity ");	
	close(fd);
	free(content);
	for(i = 0; i < 16; i ++)
	{
		if(web_api->access_snmp->rocommunity[i] == ' ')
		{
			web_api->access_snmp->rocommunity[i] = '\0';
			break;
		}
	}	
	for(i = 0; i < 16; i ++)
	{
		if(web_api->access_snmp->rwcommunity[i] == ' ')
		{
			web_api->access_snmp->rwcommunity[i] = '\0';	
			break;
		}
	}	
	return;
	
	err:
	 close(fd);
//	 fprintf(stderr,"load /etc/snmp/snmpd.conf error\n");	
}

void read_adsl_conf()
{
  int fd;
  int ret;
  char *content;
  struct stat buf;
  web_api->adv_adsl=(ADV_ADSL*)malloc(sizeof(ADV_ADSL));
  memset(web_api->adv_adsl,0,sizeof(ADV_ADSL));
  sprintf(web_api->adv_adsl->mode,"Auto Sync-Up");
  sprintf(web_api->adv_adsl->type,"ANNEX A");
	fd = open("/etc/adsl.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);	
	ret = read(fd,content,buf.st_size);
	if (ret <= 0)
	{	
		free(content);
		goto err;
	}
	config_get_value(content,web_api->adv_adsl->mode,12,"MODULATIONTYPE=");
	config_get_value(content,web_api->adv_adsl->type,16,"ANNEXTYPEA=");	
	close(fd);
	free(content);
	return;
	
	err:
	 close(fd);
//	 fprintf(stderr,"load /etc/adsl.conf error\n");
			
}
void read_nat_conf()
{
	int i,j,fd;
	char path[128];
	
	for(i=0;i<8;i++)
	{
		web_api->nat[i] = (NAT_SETUP*)malloc(sizeof(NAT_SETUP));
		memset(web_api->nat[i],0,sizeof(NAT_SETUP));

		//ipnumbers
		sprintf(web_api->nat[i]->numberofips,val_def->nat_ip_single);
		sprintf(path,"/etc/nat_pvc%d/ipmode",i);
		fd = open(path,O_RDONLY);
		if(fd != -1)
		{
			get_profile_str("IPMODE=",web_api->nat[i]->numberofips,fd);
			close(fd);
			fprintf(stderr,"----load:%s,%s\n",web_api->nat[i]->numberofips,web_api->nat[i]->serverip);
		}
		
		//dmz
		web_api->nat[i]->dmz = (DMZ_SETUP *)malloc(sizeof(DMZ_SETUP));
		memset(web_api->nat[i]->dmz,0,sizeof(DMZ_SETUP));
		
		sprintf(web_api->nat[i]->dmz->status,val_def->nat_dmz_disable);
		sprintf(path,"/etc/nat_pvc%d/dmz",i);
		fd = open(path,O_RDONLY);
		if(fd !=-1)
		{
			get_profile_str("DMZ_ACTIVE=", web_api->nat[i]->dmz->status, fd);
			get_profile_str("DMZ_IP=", web_api->nat[i]->dmz->hostip,fd);
			if(!strcmp(web_api->nat[i]->dmz->status, "Yes"))
				sprintf(web_api->nat[i]->dmz->status,val_def->nat_dmz_enable);
			else
				sprintf(web_api->nat[i]->dmz->status,val_def->nat_dmz_disable);	
			close(fd);
		}
		//vsrv
		for(j = 0;j<10;j++)
		{
			web_api->nat[i]->vsrv[j]=(VSRV_SETUP *)malloc(sizeof(VSRV_SETUP));
			memset(web_api->nat[i]->vsrv[j],0,sizeof(VSRV_SETUP));
			
			sprintf(path,"/etc/nat_pvc%d/vserver%d", i, j);
			fd = open(path, O_RDONLY);
			if(fd ==-1)
				continue;
			get_profile_str("STARTPORT=",web_api->nat[i]->vsrv[j]->start_port,fd);
			get_profile_str("ENDPORT=",web_api->nat[i]->vsrv[j]->end_port,fd);
			get_profile_str("LOCALIP=",web_api->nat[i]->vsrv[j]->local_ip,fd);
			close(fd);
		}

		//ipmapping
		for(j = 0; j<16; j++)
		{
			char temp[32];
			
			web_api->nat[i]->ipmap[j] = (IPMAP_SETUP*)malloc(sizeof(IPMAP_SETUP));
			memset(web_api->nat[i]->ipmap[j],0,sizeof(IPMAP_SETUP));
			
			sprintf(path, "/etc/nat_pvc%d/ipaddr_mapping%d", i, j);
			fd = open(path, O_RDONLY);
			if(fd ==-1)
				continue;
			get_profile_str("RULETYPE=", temp, fd);
			temp[strlen(temp) - 1] = '\0';
			strcpy(web_api->nat[i]->ipmap[j]->rule, temp + 1);
			get_profile_str("LOCALSTARTIP=", temp, fd);			
			temp[strlen(temp) - 1] = '\0';
			strcpy(web_api->nat[i]->ipmap[j]->local_startip, temp + 1);
			get_profile_str("LOCALENDIP=", temp, fd);
			temp[strlen(temp) - 1] = '\0';
			strcpy(web_api->nat[i]->ipmap[j]->local_endip, temp + 1);
			get_profile_str("PUBLICSTARTIP=", temp, fd);
			temp[strlen(temp) - 1] = '\0';
			strcpy(web_api->nat[i]->ipmap[j]->public_startip, temp + 1);
			get_profile_str("PUBLICENDIP=", temp, fd);
			temp[strlen(temp) - 1] = '\0';
			strcpy(web_api->nat[i]->ipmap[j]->public_endip, temp + 1);
			close(fd);
		}
		sprintf(path, "/etc/nat_pvc%d/server_ip", i);
		fd = open(path, O_RDONLY);
		if(fd != -1)
			get_profile_str("SERVER_IP=", web_api->nat[i]->serverip, fd);
	}
	
}

/*read active route*/
int check_route(char *rule)
{
	int fd,ret =1;
	char *line;

	fd = open("/etc/route.sh",O_RDONLY);
	if(fd ==-1)
		return ret;
	while((line = boa_readline(fd)))
	{
		if(!strcmp(line,rule))
		{
			free(line);
			close(fd);
			return 0;
		}
		free(line);
	}
	
	close(fd);
	return ret;
}

void read_route_conf()
{
	int fd,i;
	char *line;
	
	for(i = 0;i<50;i++)
	{
		web_api->route[i] = (ROUTE_SETUP*)malloc(sizeof(ROUTE_SETUP));
		memset(web_api->route[i],0,sizeof(ROUTE_SETUP));
	}
	boa_execute("/sbin/route -n > /tmp/route.log");
	
	fd = open("/tmp/route.log",O_RDONLY);
	if(fd ==-1)
		return ;
	i = 0;
	
	line = boa_readline(fd);
	free(line);
	line = boa_readline(fd);
	free(line);
	while((line = boa_readline(fd)) && i<50)
	{
		char tmp[256];
		int len=0;
		sscanf(line,"%s %s %s %s %s %s %s %s",web_api->route[i]->dest_ip,web_api->route[i]->gw_ip,web_api->route[i]->netmask,tmp,web_api->route[i]->metric,tmp,web_api->route[i]->use,web_api->route[i]->gw_interface);
		if(!strcmp(web_api->route[i]->dest_ip, "0.0.0.0"))
			sprintf(web_api->route[i]->dest_ip, "default");
#if 1
		/*check default or user edit*/
		len +=sprintf(tmp+len,"/sbin/route add -net %s netmask %s ",web_api->route[i]->dest_ip,web_api->route[i]->netmask);
		if(strcmp(web_api->route[i]->gw_ip,"0.0.0.0"))
			len +=sprintf(tmp+len,"gw %s",web_api->route[i]->gw_ip);
		else
			len +=sprintf(tmp+len,"dev %s",web_api->route[i]->gw_interface);
		if((atoi(web_api->route[i]->metric))>0)
			len +=sprintf(tmp+len," metric %s",web_api->route[i]->metric);

		web_api->route[i]->isdefault = check_route(tmp);
		fprintf(stderr,"----route:%s,%d\n",tmp,web_api->route[i]->isdefault);
#endif		
		if(!strcmp(web_api->route[i]->gw_ip,"0.0.0.0"))
		{
			strncpy(web_api->route[i]->gw_type,"dev",16);
			memset(web_api->route[i]->gw_ip,0,16);
		}
		else
			strncpy(web_api->route[i]->gw_type,"gw",16);

		fprintf(stderr,"1.%s 2.%s 3.%s 4.%s 5.%s 6.%s 7.%s\n",web_api->route[i]->dest_ip,web_api->route[i]->netmask,web_api->route[i]->gw_type,web_api->route[i]->gw_ip,web_api->route[i]->gw_interface,web_api->route[i]->metric,web_api->route[i]->use);
		free(line);
		i++;
		
	}
	close(fd);
	
}

void read_acl_conf()
{
	int rule_index;
	int fd;
	int ret;
	char *content,tmp[16];
 	struct stat buf;
 	
	web_api->access_acl=(ACCESS_ACL*)malloc(sizeof(ACCESS_ACL));
  	memset(web_api->access_acl,0,sizeof(ACCESS_ACL));
 	for(rule_index=0;rule_index<16;rule_index++)
 	{
 		web_api->access_acl->acl_rule[rule_index] = (ACL_RULE *)malloc(sizeof(ACL_RULE));
		memset(web_api->access_acl->acl_rule[rule_index],0,sizeof(ACL_RULE));
 	}
	strncpy(web_api->access_acl->status,val_def->acl_status_deactivated,8); 	
	
 	fd=open("/etc/acl.conf",O_RDONLY);
	if(fd!=-1)
	{
		strncpy(web_api->access_acl->status,val_def->acl_status_activated,8);
		ret = fstat(fd,&buf);
		if (ret == -1)
		{
			close(fd);
			return ;
		}
		content = (char*)malloc(buf.st_size+1);	
		ret = read(fd,content,buf.st_size);
		if (ret <= 0)
		{	
			free(content);
			close(fd);
			return ;
		}	
		for(rule_index=0;rule_index<16;rule_index++)
		{
			sprintf(tmp,"ACTIVE%d=",rule_index+1);
			config_get_value(content,web_api->access_acl->acl_rule[rule_index]->rule_status,8,tmp);
			sprintf(tmp,"SECUREIP%d=",rule_index+1);
			config_get_value(content,web_api->access_acl->acl_rule[rule_index]->secure_ip,16,tmp);			
			sprintf(tmp,"APPLICATION%d=",rule_index+1);
			config_get_value(content,web_api->access_acl->acl_rule[rule_index]->application,8,tmp);
			sprintf(tmp,"INTERFACE%d=",rule_index+1);
			config_get_value(content,web_api->access_acl->acl_rule[rule_index]->interface,8,tmp);
		}	
		free(content);	
		
	}
	close(fd); 	
}

void read_ipfilter_ipmac() //wtw add
//wtw del void read_ipfilter_conf()
{
	int set_index,rule_index;
	int fd;
	int ret;
	char *content,path[64];
 	struct stat buf;
 	
	for(set_index=0;set_index<12;set_index++)
	{
		web_api->access_ipfilter[set_index] = (ACCESS_IPFILTER *)malloc(sizeof(ACCESS_IPFILTER));
		memset(web_api->access_ipfilter[set_index],0,sizeof(ACCESS_IPFILTER));
		sprintf(path,"/etc/ipfilter/ipfilter_%d_0.conf",set_index+1);
		fd=open(path,O_RDONLY);
		if(fd!=-1)
		{
			ret = fstat(fd,&buf);
			if (ret == -1)
				 break;
			content = (char*)malloc(buf.st_size+1);	
			ret = read(fd,content,buf.st_size);
			if (ret <= 0)
			{	
				free(content);
				 break;
			}	
//wtw del			config_get_value(content,web_api->access_ipfilter[set_index]->interface,8,"INTERFACE=");	
//wtw del			config_get_value(content,web_api->access_ipfilter[set_index]->direction,8,"DIRECTION=");
//wtw add begin
			config_get_value(content,web_api->access_ipfilter[set_index]->interface,16,"INTERFACE=");	//wkw 3.6
			config_get_value(content,web_api->access_ipfilter[set_index]->direction,16,"DIRECTION=");//wkw 3.6
//wtw add end
			free(content);
		}			
		close(fd);
		
		for(rule_index=0;rule_index<6;rule_index++)	
		{
			web_api->access_ipfilter[set_index]->rule[rule_index] = (IPFILTER_RULE *)malloc(sizeof(IPFILTER_RULE));
			memset(web_api->access_ipfilter[set_index]->rule[rule_index],0,sizeof(IPFILTER_RULE));
//wtw add begin						
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->active,"%s",val_def->ipfilter_active_no);	
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->mac,"00:00:00:00:00:00");				
//wtw add end			
			sprintf(path,"/etc/ipfilter/ipfilter_%d_%d.conf",set_index+1,rule_index+1);
			fd=open(path,O_RDONLY);
			if(fd!=-1)
			{
				ret = fstat(fd,&buf);
				if (ret == -1)
					 continue;
				content = (char*)malloc(buf.st_size+1);	
				ret = read(fd,content,buf.st_size);
				if (ret <= 0)
				{	
					free(content);
					continue;
				}	
//wtw add begin
				/***********ipfilter extend*********/
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type,4,"RULE_TYPE=");	
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->mac,20,"MAC=");	
				/***********************************/					
//wtw add end
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->active,8,"ACTIVE=");	
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->source_ip,15,"SRCIP=");	
				//wtw del config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask,15,"SRCMASK=");	
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask,16,"SRCMASK=");	//wtw add
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->source_port,8,"SRCPORT=");	
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->des_ip,15,"DESIP=");	
				//wtw del config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask,15,"DESMASK=");	
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask,16,"DESMASK=");	//wtw add
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->des_port,8,"DESPORT=");
				//wtw del config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->protocol,4,"PROTOCOL=");
				config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->protocol,5,"PROTOCOL="); //wtw add
				//wtw del config_get_value(content,web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched,8,"UNMATCHED=");
				free(content);
			}
			close(fd);
		}
	}
}
//wtw add begin
void read_ipfilter_conf()
{	
	web_api->ipfilter_extend=(IPFILTER_EXTEND*)malloc(sizeof(IPFILTER_EXTEND));
  memset(web_api->ipfilter_extend,0,sizeof(IPFILTER_EXTEND));
  
  read_ipfilter_url_conf();   // 2007.3.6
  read_ipfilter_app_conf();   // 2007.3.6
	read_ipfilter_ipmac();	//old
}
//wtw add end
void read_upnp_conf()
{
	int fd=0;
	char value[16];

	web_api->access_upnp=(ACCESS_UPNP*)malloc(sizeof(ACCESS_UPNP));
	memset(web_api->access_upnp,0,sizeof(ACCESS_UPNP));
	
	fd = open(UPNPD_PATH,O_RDONLY);
	if(fd ==-1){
		return;
	}else{
		get_profile_str("igd=", value, fd);
		if(strcmp(value,"enable") == 0){
			web_api->access_upnp->status = 1;
		}else{
			web_api->access_upnp->status = 0;
		}
		
		get_profile_str("autoconf=", value, fd);
		web_api->access_upnp->auto_conf=atoi(value);
		close(fd);
	}
#if 0 //here_20080112
    int fd;
	int ret;
	char *content,tmp[32];
	struct stat buf;
	
	web_api->access_upnp=(ACCESS_UPNP*)malloc(sizeof(ACCESS_UPNP));
    memset(web_api->access_upnp,0,sizeof(ACCESS_UPNP));
	fd = open("/etc/upnp.conf",O_RDONLY);
	if (fd == -1)
		return ;
		
	web_api->access_upnp->status=1;
	
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);	
	ret = read(fd,content,buf.st_size);
	if (ret <= 0)
	{	
		free(content);
		goto err;
	}
	web_api->access_upnp->auto_conf=atoi(config_get_value(content,tmp,16,"AUTOCONFIG="));
	
	close(fd);
	free(content);
	return;
	
	err:
	 close(fd);
//	 fprintf(stderr,"load /etc/upnp.conf error");	 
#endif
	
}

void read_ddns_conf()
{
    int fd;

    web_api->access_ddns=(ACCESS_DDNS*)malloc(sizeof(ACCESS_DDNS));
    web_api->access_ddns->status=0;
	fd = open("/etc/ddns.conf",O_RDONLY);
	if (fd == -1)
		return ;
	web_api->access_ddns->status=1;

	get_profile_str("SERVERNAME=", web_api->access_ddns->provider, fd);	
	get_profile_str("MYHOST=", web_api->access_ddns->host, fd);	
	get_profile_str("EMAIL=", web_api->access_ddns->email, fd);	
	get_profile_str("USERNAME=", web_api->access_ddns->username, fd);	
	get_profile_str("PASSWORD=", web_api->access_ddns->password, fd);	
	get_profile_str("WILDCARD=", web_api->access_ddns->wildcard, fd);	

	close(fd);
	return ;
	
}

void read_diag_dns(int cur_vc)
{
#if 0	
	int  ret;		
#endif
//	int  fd ;
	FILE *fd;
#if 0
	char *content = NULL;		
	struct stat buf;
#endif
//	char *line;
	char line[100];

	web_api->main_diagnostics =(T_MAIN_DIAGNOSTICS*)malloc(sizeof(T_MAIN_DIAGNOSTICS));
  memset(web_api->main_diagnostics,0,sizeof(T_MAIN_DIAGNOSTICS));

  	if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_pppoe))
//		fd = open("/etc/ppp/resolv.conf",O_RDONLY);
		fd = fopen("/etc/ppp/resolv.conf", "r");
	else
//		fd = open("/etc/resolv.conf",O_RDONLY);
		fd = fopen("/etc/resolv.conf", "r");
//	if (fd == -1)
//		goto err;
	if(!fd)
		fprintf(stderr,"------ load  /etc/resolv.conf  err\n");
	else
	{
		fprintf(stderr,"------ load  /etc/resolv.conf  sucessfully\n");
		fgets(line, 100, fd);
		if(!strstr(line,"nameserver")){/*if it have domain information, we need to get next to get DNS server.shnwind 2008.4.17*/
		  fgets(line, 100, fd);
                }
		strcpy(web_api->main_diagnostics->dns_server, line + 11);
		strcpy(web_api->lan[1]->dns_primary, line + 11);
		fgets(line, 100, fd);
		if(strlen(line))
			strcpy(web_api->lan[1]->dns_second, line + 11);
	        fclose(fd);
	}
	
#if 0
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);			
	if (content == NULL)
		goto err; 
	ret = read(fd,content,buf.st_size);
	if (ret<=0)
		goto err;
	config_get_value(content,web_api->main_diagnostics->dns_server,16,"nameserver ");
#endif
//	line = boa_readline(fd);
//	config_get_value(line, web_api->main_diagnostics->dns_server, 16, "nameserver");
//	strncpy(web_api->lan[1]->dns_primary, web_api->main_diagnostics->dns_server, 16);
//	free(line);
//	line = boa_readline(fd);
//	if(line)
//	{
//		config_get_value(line, web_api->lan[1]->dns_second, 16, "nameserver");
//		free(line);
//	}
//	close(fd);
#if 0	
	free(content);	
#endif
	return;
	
//err:
//	if (fd != -1)
//	close(fd);
#if 0	
	if (content != NULL)
	free(content);
#endif	
//	fprintf(stderr,"------ load  /etc/resolv.conf  err\n");	
	return;
}

void read_time_settings()
{

	int  fd ;
	int i=0;
	char *content = NULL;		
	//	int  ret;		
	//	struct stat buf;
	
	web_api->main_timezone =( T_MAIN_TIEMZONE*)malloc(sizeof(T_MAIN_TIEMZONE));
  memset(web_api->main_timezone,0,sizeof(T_MAIN_TIEMZONE));
  
//	web_api->main_timezone->curr_timezone = 13;
	    
  web_api->main_timezone->timezone[0] = val_def->time_zone1;
  web_api->main_timezone->timezone[1] = val_def->time_zone2; 
  web_api->main_timezone->timezone[2] = val_def->time_zone3;
  web_api->main_timezone->timezone[3] = val_def->time_zone4;   
  web_api->main_timezone->timezone[4] = val_def->time_zone5;
  web_api->main_timezone->timezone[5] = val_def->time_zone6;   
  web_api->main_timezone->timezone[6] = val_def->time_zone7;
  web_api->main_timezone->timezone[7] = val_def->time_zone8;   
  web_api->main_timezone->timezone[8] = val_def->time_zone9;
  web_api->main_timezone->timezone[9] = val_def->time_zone10;
  web_api->main_timezone->timezone[10] = val_def->time_zone11;
  web_api->main_timezone->timezone[11] = val_def->time_zone12; 
  web_api->main_timezone->timezone[12] = val_def->time_zone13;
  web_api->main_timezone->timezone[13] = val_def->time_zone14;   
  web_api->main_timezone->timezone[14] = val_def->time_zone15;
  web_api->main_timezone->timezone[15] = val_def->time_zone16;   
  web_api->main_timezone->timezone[16] = val_def->time_zone17;
  web_api->main_timezone->timezone[17] = val_def->time_zone18;   
  web_api->main_timezone->timezone[18] = val_def->time_zone19;
  web_api->main_timezone->timezone[19] = val_def->time_zone20;  
  web_api->main_timezone->timezone[20] = val_def->time_zone21;    
  web_api->main_timezone->timezone[21] = val_def->time_zone22; 
  web_api->main_timezone->timezone[22] = val_def->time_zone23;
  web_api->main_timezone->timezone[23] = val_def->time_zone24;   
  web_api->main_timezone->timezone[24] = val_def->time_zone25;
  web_api->main_timezone->timezone[25] = val_def->time_zone26;   
  web_api->main_timezone->timezone[26] = val_def->time_zone27;
  web_api->main_timezone->timezone[27] = val_def->time_zone28;     
  
  
 #if 0      
	fd = open("/etc/ntp.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);			
	if (content == NULL)
		goto err; 
	ret = read(fd,content,buf.st_size);
	if (ret<=0)
		goto err;
	config_get_value(content,web_api->main_timezone->ntp_server,16,"server ");

	close(fd);
	free(content);	
#endif	
	goto next;
#if 0	
err:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
	content = NULL;
  sprintf(web_api->main_timezone->ntp_server,"0.0.0.0");	
	fprintf(stderr,"------ load  /etc/ntp.conf  err\n");	
#endif	
next:	
	fd = open("/etc/timezone.conf",O_RDONLY);
	if (fd == -1)
		goto err2;

	strcpy(web_api->main_timezone->Syn_with, val_def->time_syn_ntp);
	get_profile_str("DAYLIGHT=", web_api->main_timezone->daylight_status, fd);
	get_profile_str("TZ=", web_api->main_timezone->time_zone_conf, fd);
	get_profile_str("SERVER=", web_api->main_timezone->ntp_server, fd);
	if(web_api->main_timezone->time_zone_conf[3] == '+')
		web_api->main_timezone->time_zone_conf[3] = '-';
	else if(web_api->main_timezone->time_zone_conf[3] == '-')
		web_api->main_timezone->time_zone_conf[3] = '+';
	if (strlen(web_api->main_timezone->time_zone_conf) < 9)
	{
			web_api->main_timezone->curr_timezone = 13;	
	}
	else
	{
		for(;i<28;i++)
		{
			if (strstr(web_api->main_timezone->timezone[i],web_api->main_timezone->time_zone_conf) != NULL)
				web_api->main_timezone->curr_timezone = i;
		}
	}
	close(fd);
	return;
	
err2:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
	web_api->main_timezone->curr_timezone = 13;
//	fprintf(stderr,"------ load  /etc/timezone.conf  err\n");	
	return;	
	
}

void read_deviceinfo()
{
	int fd;
	char *line;
	web_api->dev_info =(DEVICE_INFO*) malloc(sizeof(DEVICE_INFO));
	memset(web_api->dev_info,0,sizeof(DEVICE_INFO));

	strcpy(web_api->dev_info->device_firmwarever,"N/A");
	fd= open("/etc/fwver.conf",O_RDONLY);
	if(fd !=-1)
	{
		line = boa_readline(fd);
		if(line)
		{
			strncpy(web_api->dev_info->device_firmwarever,line,128);
			free(line);
		}
		close(fd);
	}
	fprintf(stderr,"----firmware ver:%s\n",web_api->dev_info->device_firmwarever);
}

void read_vlan_pvid_conf()
{
	int fd;
	int ret;
	char *content;
	struct stat buf;
	
	web_api->adv_vlan_pvid=(ADV_VLAN_PVID*)malloc(sizeof(ADV_VLAN_PVID));
    memset(web_api->adv_vlan_pvid,0,sizeof(ADV_VLAN_PVID));
	fd = open("/etc/vlan_pvid.conf",O_RDONLY);
	if (fd != -1)
	{	
		ret = fstat(fd,&buf);
		if (ret == -1)
			goto err;
		content = (char*)malloc(buf.st_size+1);	
		ret = read(fd,content,buf.st_size);
		if (ret <= 0)
		{	
			free(content);
			goto err;
		}
		config_get_value(content,web_api->adv_vlan_pvid->atmvc0,2,"ATMVC0=");
		config_get_value(content,web_api->adv_vlan_pvid->atmvc1,2,"ATMVC1=");
		config_get_value(content,web_api->adv_vlan_pvid->atmvc2,2,"ATMVC2=");
		config_get_value(content,web_api->adv_vlan_pvid->atmvc3,2,"ATMVC3=");
		config_get_value(content,web_api->adv_vlan_pvid->atmvc4,2,"ATMVC4=");
		config_get_value(content,web_api->adv_vlan_pvid->atmvc5,2,"ATMVC5=");
		config_get_value(content,web_api->adv_vlan_pvid->atmvc6,2,"ATMVC6=");
		config_get_value(content,web_api->adv_vlan_pvid->atmvc7,2,"ATMVC7=");
		
		config_get_value(content,web_api->adv_vlan_pvid->ethport0,2,"ETHPORT0=");
		config_get_value(content,web_api->adv_vlan_pvid->ethport1,2,"ETHPORT1=");
		config_get_value(content,web_api->adv_vlan_pvid->ethport2,2,"ETHPORT2=");
		config_get_value(content,web_api->adv_vlan_pvid->ethport3,2,"ETHPORT3=");	
		
		config_get_value(content,web_api->adv_vlan_pvid->usbport,2,"USBPORT=");				
		
		free(content);
	}	
	close(fd);
	return;
err:
	close(fd);
//	fprintf(stderr,"read vlan pvid   err");
}

void read_vlan_group_conf()
{
	int fd,i,j;
	int ret;
	char *content,find[8],*aim,*group,*first,*second,*groupbuf;
	struct stat buf;
	for(i=0;i<8;i++)
	{
		web_api->adv_vlan_group[i]=(ADV_VLAN_GROUP*)malloc(sizeof(ADV_VLAN_GROUP));
   		memset(web_api->adv_vlan_group[i],0,sizeof(ADV_VLAN_GROUP));
    }
	fd = open("/etc/vlan_group.conf",O_RDONLY);
	if (fd != -1)
	{	
		ret = fstat(fd,&buf);
		if (ret == -1)
			goto err;
		content = (char*)malloc(buf.st_size+1);	
		ret = read(fd,content,buf.st_size);
		if (ret <= 0)
		{	
			free(content);
			goto err;
		}	
		for(i=0;i<8;i++)
		{
			sprintf(find,"GROUP%d=",i+1);
			group=strstr(content,find);
			if(group)
			{
				first=strchr(group,'\"');
				second=strchr(first+1,'\"');
				groupbuf=(char *)malloc(second-first+1);
				groupbuf[second-first+1]='\0';
				strncpy(groupbuf,first,second-first);
				if(strstr(groupbuf,"Yes"))
					strncpy(web_api->adv_vlan_group[i]->active,val_def->vlan_group_active_yes,8);
				else
					strncpy(web_api->adv_vlan_group[i]->active,val_def->vlan_group_active_no,8);
				aim=strstr(groupbuf,"ID");
				second=strchr(aim+1,' ');
				if(aim && second)
					strncpy(web_api->adv_vlan_group[i]->vlan_id,aim+2,second-aim-2);
				else
					memset(web_api->adv_vlan_group[i]->vlan_id,0,sizeof(web_api->adv_vlan_group[i]->vlan_id));
				for(j=0;j<8;j++)
				{	
					sprintf(find,"Pp%d",j);
					aim=strstr(groupbuf,find);
					if(aim)
						web_api->adv_vlan_group[i]->atmp[j]=1;
					else	
						web_api->adv_vlan_group[i]->atmp[j]=0;	
					sprintf(find,"Tp%d",j);
					aim=strstr(groupbuf,find);
					if(aim)
						web_api->adv_vlan_group[i]->atmt[j]=1;
					else	
						web_api->adv_vlan_group[i]->atmt[j]=0;							
				}	
				for(j=0;j<4;j++)
				{
					sprintf(find,"Pe%d",j+1);
					aim=strstr(groupbuf,find);
					if(aim)
						web_api->adv_vlan_group[i]->ethernetp[j]=1;
					else	
						web_api->adv_vlan_group[i]->ethernetp[j]=0;	
					sprintf(find,"Te%d",j+1);
					aim=strstr(groupbuf,find);
					if(aim)
						web_api->adv_vlan_group[i]->ethernett[j]=1;
					else	
						web_api->adv_vlan_group[i]->ethernett[j]=0;					
				}
				aim=strstr(groupbuf,"PU1");
				if(aim)
					web_api->adv_vlan_group[i]->usbp=1;
				else
					web_api->adv_vlan_group[i]->usbp=0;
				aim=strstr(groupbuf,"TU1");
				if(aim)
					web_api->adv_vlan_group[i]->usbt=1;
				else
					web_api->adv_vlan_group[i]->usbt=0;				
					
				free(groupbuf);	
			}
			
		}
		free(content);
	}	
	close(fd);
	return;
err:
	close(fd);
//	fprintf(stderr,"read vlan group   err");
}


void	read_adv_qos_settings()
{
	int  ret;		
	int  fd ;
	char *content = NULL;		
	struct stat buf;
	
	web_api->adv_qos =( T_ADV_QOS*)malloc(sizeof(T_ADV_QOS));
  memset(web_api->adv_qos,0,sizeof(T_ADV_QOS));
 
 	fd = open("/etc/qos.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);			
	if (content == NULL)
		goto err; 
	ret = read(fd,content,buf.st_size);
	if (ret<=0)
		goto err;
		
	strncpy(web_api->adv_qos->qos_status,val_def->qos_activated,16); 		
	config_get_value(content,web_api->adv_qos->qos_8021_status,16,"QOS8021P_ACTIVE=");	
	if (!strcmp(web_api->adv_qos->qos_8021_status,val_def->qos_8021_activated))
	{
		config_get_value(content,web_api->adv_qos->qos_p8021_value0,8,"P8021_VALUE0=");			
		config_get_value(content,web_api->adv_qos->qos_p8021_value1,8,"P8021_VALUE1=");
		config_get_value(content,web_api->adv_qos->qos_p8021_value2,8,"P8021_VALUE2=");
		config_get_value(content,web_api->adv_qos->qos_p8021_value3,8,"P8021_VALUE3=");	
		config_get_value(content,web_api->adv_qos->qos_p8021_value4,8,"P8021_VALUE4=");
		config_get_value(content,web_api->adv_qos->qos_p8021_value5,8,"P8021_VALUE5=");
		config_get_value(content,web_api->adv_qos->qos_p8021_value6,8,"P8021_VALUE6=");
		config_get_value(content,web_api->adv_qos->qos_p8021_value7,8,"P8021_VALUE7=");		
	}
	else
	{
		strncpy(web_api->adv_qos->qos_8021_status,val_def->qos_8021_deactivated,16); 		
	}
	
	config_get_value(content,web_api->adv_qos->qos_ip_status,16,"IPQOS_ACTIVE=");	
	if (!strcmp(web_api->adv_qos->qos_ip_status,val_def->qos_ip_activated))
	{
		config_get_value(content,web_api->adv_qos->qos_iptype,16,"IPQOS_TYPE=");	
		config_get_value(content,web_api->adv_qos->qos_ipqos_value0,8,"IPQOS_VALUE0=");				
		config_get_value(content,web_api->adv_qos->qos_ipqos_value1,8,"IPQOS_VALUE1=");	
		config_get_value(content,web_api->adv_qos->qos_ipqos_value2,8,"IPQOS_VALUE2=");		
		config_get_value(content,web_api->adv_qos->qos_ipqos_value3,8,"IPQOS_VALUE3=");	
		config_get_value(content,web_api->adv_qos->qos_ipqos_value4,8,"IPQOS_VALUE4=");				
		config_get_value(content,web_api->adv_qos->qos_ipqos_value5,8,"IPQOS_VALUE5=");	
		config_get_value(content,web_api->adv_qos->qos_ipqos_value6,8,"IPQOS_VALUE6=");		
		config_get_value(content,web_api->adv_qos->qos_ipqos_value7,8,"IPQOS_VALUE7=");		
	}
	else
	{
		strncpy(web_api->adv_qos->qos_ip_status,val_def->qos_ip_deactivated,16); 	
		strncpy(web_api->adv_qos->qos_iptype,val_def->qos_iptype_tos,16); 		
	}

	config_get_value(content,web_api->adv_qos->qos_app_status,16,"APPQOS_ACTIVE=");	
	if (!strcmp(web_api->adv_qos->qos_app_status,val_def->qos_app_activated))
	{	
		config_get_value(content,web_api->adv_qos->qos_voicequeueindex,8,"VOICEQUEUEINDEX=");	
		config_get_value(content,web_api->adv_qos->qos_voicertprange1,8,"VOICERTPRANGE1=");	
		config_get_value(content,web_api->adv_qos->qos_voicertprange2,8,"VOICERTPRANGE2=");		
		config_get_value(content,web_api->adv_qos->qos_videoqueueindex,8,"VIDEOQUEUEINDEX=");			
		config_get_value(content,web_api->adv_qos->qos_videortprange1,8,"VIDEORTPRANGE1=");		

		config_get_value(content,web_api->adv_qos->qos_videortprange2,8,"VIDEORTPRANGE2="); 
		config_get_value(content,web_api->adv_qos->qos_igmpqueueindex,8," IGMPQUEUEINDEX=");  
		config_get_value(content,web_api->adv_qos->qos_dataqueueindex,8,"DATAQUEUEINDEX="); 
	}
	else
	{
		strncpy(web_api->adv_qos->qos_app_status,val_def->qos_app_deactivated,16);  		
	}
		
	config_get_value(content,web_api->adv_qos->qos_vlan_status,16,"VLANQOS_ACTIVE=");
	if (!strcmp(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_activated))
	{
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value0,8,"VLANQOS_VALUE0=");			
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value1,8,"VLANQOS_VALUE1=");	
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value2,8,"VLANQOS_VALUE2=");		
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value3,8,"VLANQOS_VALUE3=");			
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value4,8,"VLANQOS_VALUE4=");		
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value5,8,"VLANQOS_VALUE5=");	
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value6,8,"VLANQOS_VALUE6=");		
		config_get_value(content,web_api->adv_qos->qos_vlanqos_value7,8,"VLANQOS_VALUE7=");				
	}
	else
	{
		strncpy(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_deactivated,16); 	
	}
									
	close(fd);
	free(content);	
	fprintf(stderr,"------ load  /etc/qos.conf  ok\n");		
	return; 
  
err:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
//	fprintf(stderr,"------ load  /etc/qos.conf  err\n");	
	strncpy(web_api->adv_qos->qos_status,val_def->qos_deactivated,16); 
	strncpy(web_api->adv_qos->qos_8021_status,val_def->qos_8021_deactivated,16); 	
	strncpy(web_api->adv_qos->qos_ip_status,val_def->qos_ip_deactivated,16); 	
	strncpy(web_api->adv_qos->qos_iptype,val_def->qos_iptype_tos,16); 		
	strncpy(web_api->adv_qos->qos_app_status,val_def->qos_app_deactivated,16); 
	sprintf(web_api->adv_qos->qos_voicequeueindex,"1");
	sprintf(web_api->adv_qos->qos_videoqueueindex,"1");
	sprintf(web_api->adv_qos->qos_igmpqueueindex,"1");
	sprintf(web_api->adv_qos->qos_dataqueueindex,"1");	
	strncpy(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_deactivated,16); 		
	return; 
}

void read_adv_vlan_top_settings()
{
	int  ret;		
	int  fd ;
	char *content = NULL;		
	struct stat buf;
	
 	fd = open("/etc/vlan_pvid.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);			
	if (content == NULL)
		goto err; 
	ret = read(fd,content,buf.st_size);
	if (ret<=0)
		goto err;		
	strncpy(web_api->vlan_top_status,content,16); 	
	close(fd);
	free(content);	
	fprintf(stderr,"------ load  /etc/vlan_pvid.conf  ok\n");		
	return; 
  
err:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
//	fprintf(stderr,"------ load  /etc/vlan_pvid.conf  err\n");	
	strncpy(web_api->vlan_top_status,val_def->vlan_top_deactivated,16); 		
	return; 			
	
}

void read_wireless_settings()
{
	int ssid_index,i;
	int fd;
	//int ret;
	char *content,tmp[20], allmac[160];
 	//struct stat buf;
	//char *tmp_64,*tmp_128,
	char *tmp_mac;	
  
	web_api->home_wireless=(WIRELESS_SETTING*)malloc(sizeof(WIRELESS_SETTING));
  memset(web_api->home_wireless,0,sizeof(WIRELESS_SETTING)); 
  strncpy(web_api->home_wireless->access,val_def->wireless_access_deactivated,8);
  //strncpy(web_api->home_wireless->active,val_def->wireless_active_deactivated,8);
  sprintf(web_api->home_wireless->ssid_index,"1"); 
  
// 	for(ssid_index=0;ssid_index<SSID_COUNT;ssid_index++)
	for(ssid_index=0;ssid_index<1;ssid_index++)
 	{
 		web_api->home_wireless->SSID[ssid_index] = (WIRELESS_SSID *)malloc(sizeof(WIRELESS_SSID));
		memset(web_api->home_wireless->SSID[ssid_index],0,sizeof(WIRELESS_SSID));
		
		sprintf(tmp,"%s%d",val_def->wireless_default_SSID,ssid_index+1);
		strncpy(web_api->home_wireless->SSID[ssid_index]->SSID,tmp,32);
		
		strncpy(web_api->home_wireless->SSID[ssid_index]->broadcast,val_def->wireless_broadcast_yes,8);
		strncpy(web_api->home_wireless->SSID[ssid_index]->active,val_def->wireless_active_deactivated,8);

		sprintf(web_api->home_wireless->SSID[ssid_index]->authentication,"%s",val_def->wireless_authentication_disabled);

		sprintf(web_api->home_wireless->SSID[ssid_index]->keyIndex_64,"1");
		sprintf(web_api->home_wireless->SSID[ssid_index]->keyIndex_128,"1");

		tmp_mac=web_api->home_wireless->SSID[ssid_index]->mac1;
		for(i=0;i<8;i++)
		{
			sprintf(tmp_mac,"%s",val_def->wireless_default_mac);
			tmp_mac+=20;
		}

	}		
/*
	system("insmod /lib/modules/rt61ap.o");
	system("ifconfig ra0 0.0.0.0");
	system("brctl addif br0 ra0");
*/

// 	fd = open("/etc/wireless.conf",O_RDONLY);
	fd = open("/etc/Wireless/RT61AP/RT61AP.dat",O_RDONLY);
	if(fd!=-1)
	{	
#if 0	
		ret = fstat(fd,&buf);
		if (ret == -1)
		{
			close(fd);
			return ;
		}
		content = (char*)malloc(buf.st_size+1);	
		ret = read(fd,content,buf.st_size);
		if (ret <= 0)
		{	
			free(content);
			close(fd);
			return ;
		}	

		config_get_value(content,web_api->home_wireless->access,8,"ACCESS=");	
		config_get_value(content,web_api->home_wireless->channel,32,"CHANNEL=");			
		config_get_value(content,web_api->home_wireless->channel_num,8,"CHANNELNUM=");			
		config_get_value(content,web_api->home_wireless->current_channel,8,"CURRENTCHANNEL=");	
		config_get_value(content,web_api->home_wireless->beacon,8,"BEACON=");			
		config_get_value(content,web_api->home_wireless->RTS_CTS,8,"RTS_CTS=");		
		config_get_value(content,web_api->home_wireless->Fragmentation,8,"FRAGMENTATION=");			
		config_get_value(content,web_api->home_wireless->DTIM,8,"DTIM=");		
		config_get_value(content,web_api->home_wireless->bg80211,16,"80211BG=");			
		
		config_get_value(content,web_api->home_wireless->ssid_index,4,"SSIDINDEX=");
		
		for(ssid_index=0;ssid_index<SSID_COUNT;ssid_index++)
	 	{		
	 		sprintf(tmp,"SSID_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->SSID,32,tmp);
	 		sprintf(tmp,"BROADCAST_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->broadcast,4,tmp);	
	 		sprintf(tmp,"AUTHENTICATION_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->authentication,16,tmp);
			
		 	sprintf(tmp,"KEYINDEX_64_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->keyIndex_64,4,tmp);
			sprintf(tmp,"KEYINDEX_128_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->keyIndex_128,4,tmp);

			tmp_64=web_api->home_wireless->SSID[ssid_index]->key1_64;
			tmp_128=web_api->home_wireless->SSID[ssid_index]->key1_128;
			for(i=0;i<4;i++)
			{
			 	sprintf(tmp,"KEY%d_64_%d=",i+1,ssid_index+1);
				config_get_value(content,tmp_64,8,tmp);
				tmp_64+=8;
				sprintf(tmp,"KEY%d_128_%d=",i+1,ssid_index+1);
				config_get_value(content,tmp_128,32,tmp);
				tmp_128+=32;
			}
			
	 		sprintf(tmp,"ENCRYPTION_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->encryption,4,tmp);
		 	sprintf(tmp,"PRE-SHARED_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->pre_shared,64,tmp);				

			sprintf(tmp,"ACTIVE_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->active,8,tmp);	
			sprintf(tmp,"ACTION_%d=",ssid_index+1);
			config_get_value(content,web_api->home_wireless->SSID[ssid_index]->action,20,tmp);	

			tmp_mac=web_api->home_wireless->SSID[ssid_index]->mac1;
			for(i=0;i<8;i++)
			{
				sprintf(tmp,"MAC%d_%d=",i+1,ssid_index+1);
				config_get_value(content,tmp_mac,20,tmp);	
				tmp_mac+=20;
			}
				
	 	}	
#endif	 	
		get_profile_str("APOn=",web_api->home_wireless->access, fd);	
		get_profile_str("Country=",web_api->home_wireless->channel, fd);	
		get_profile_str("CountryRegion=", tmp, fd);
		switch(atoi(tmp))
		{
			case 0:
				StartChannelNumber = 1;
				EndChannelNumber = 11;
				break;
			case 1:
				StartChannelNumber = 1;
				EndChannelNumber = 13;
				break;
			case 2:
				StartChannelNumber = 10;
				EndChannelNumber = 11;
				break;
			case 3:
				StartChannelNumber = 10;
				EndChannelNumber = 13;
				break;
			case 5:
				StartChannelNumber = 1;
				EndChannelNumber = 14;
				break;
			case 6:
				StartChannelNumber = 5;
				EndChannelNumber = 7;
				break;
		}
		get_profile_str("AutoChannelSelect=", web_api->home_wireless->channel_num, fd);
		get_profile_str("Channel=",web_api->home_wireless->current_channel, fd);
		if(atoi(web_api->home_wireless->channel_num))
			strcpy(web_api->home_wireless->channel_num, val_def->wireless_channel_auto);
		else
			strcpy(web_api->home_wireless->channel_num, web_api->home_wireless->current_channel);		
		
		get_profile_str("BeaconPeriod=",web_api->home_wireless->beacon, fd);			
		get_profile_str("RTSThreshold=",web_api->home_wireless->RTS_CTS, fd);		
		get_profile_str("FragThreshold=",web_api->home_wireless->Fragmentation, fd);			
		get_profile_str("DtimPeriod=",web_api->home_wireless->DTIM, fd);		
		get_profile_str("WirelessMode=", tmp, fd);	
		switch(atoi(tmp))
		{
			case 0:
				strcpy(web_api->home_wireless->bg80211, val_def->wireless_80211bg_80211bg);
				break;
			case 1:
				strcpy(web_api->home_wireless->bg80211, val_def->wireless_80211bg_80211b);
				break;
			case 2:
				strcpy(web_api->home_wireless->bg80211, val_def->wireless_80211bg_80211g);
				break;
		}
#if 0		
		get_profile_str("",web_api->home_wireless->ssid_index, fd);
#endif		
//		for(ssid_index=0;ssid_index<SSID_COUNT;ssid_index++)
		for(ssid_index=0;ssid_index<1;ssid_index++)
	 	{		
//	 		sprintf(tmp,"SSID_%d=",ssid_index+1);
			get_profile_str("SSID=",web_api->home_wireless->SSID[ssid_index]->SSID, fd);
//	 		sprintf(tmp,"BROADCAST_%d=",ssid_index+1);
			get_profile_str("HideSSID=", tmp, fd);
			switch(atoi(tmp))
			{
				case 0:
					strcpy(web_api->home_wireless->SSID[ssid_index]->broadcast, val_def->wireless_broadcast_yes);
					break;
				case 1:
					strcpy(web_api->home_wireless->SSID[ssid_index]->broadcast, val_def->wireless_broadcast_no);
					break;
			}
//	 		sprintf(tmp,"AUTHENTICATION_%d=",ssid_index+1);
			get_profile_str("AuthMode=", tmp, fd);
			if(!strcmp(tmp, "OPEN"))
				strcpy(web_api->home_wireless->SSID[ssid_index]->authentication, val_def->wireless_authentication_disabled);
			else if(!strcmp(tmp, "WEPAUTO"))
			{
				get_profile_str("Key1Str=", tmp, fd);
				switch(strlen(tmp))
				{
					case 5:
						strcpy(web_api->home_wireless->SSID[ssid_index]->authentication, val_def->wireless_authentication_wep64bits);
						strcpy(web_api->home_wireless->SSID[ssid_index]->key1_64, tmp);
						get_profile_str("Key2Str=", web_api->home_wireless->SSID[ssid_index]->key2_64, fd);
						get_profile_str("Key3Str=", web_api->home_wireless->SSID[ssid_index]->key3_64, fd);
						get_profile_str("Key4Str=", web_api->home_wireless->SSID[ssid_index]->key4_64, fd);
						get_profile_str("DefaultKeyID=", web_api->home_wireless->SSID[ssid_index]->keyIndex_64, fd);
						break;
					case 13:
						strcpy(web_api->home_wireless->SSID[ssid_index]->authentication, val_def->wireless_authentication_wep128bits);
						strcpy(web_api->home_wireless->SSID[ssid_index]->key1_128, tmp);
						get_profile_str("Key2Str=", web_api->home_wireless->SSID[ssid_index]->key2_128, fd);
						get_profile_str("Key3Str=", web_api->home_wireless->SSID[ssid_index]->key3_128, fd);
						get_profile_str("Key4Str=", web_api->home_wireless->SSID[ssid_index]->key4_128, fd);
						get_profile_str("DefaultKeyID=", web_api->home_wireless->SSID[ssid_index]->keyIndex_128, fd);
						break;
				}
			}
			else if(tmp[0] == 'W' && tmp[1] == 'P' && tmp[2] == 'A')
			{
				if(!strcmp(tmp, "WPAPSK"))
					strcpy(web_api->home_wireless->SSID[ssid_index]->authentication, val_def->wireless_authentication_wpapsk);
				else if(!strcmp(tmp, "WPA2PSK"))
					strcpy(web_api->home_wireless->SSID[ssid_index]->authentication, val_def->wireless_authentication_wpa2psk);
				get_profile_str("EncrypType=",web_api->home_wireless->SSID[ssid_index]->encryption, fd);
				get_profile_str("WPAPSK=",web_api->home_wireless->SSID[ssid_index]->pre_shared, fd);
			}
			get_profile_str("AccessPolicy0=", tmp, fd);
			switch(atoi(tmp))
			{
				case 0:
					strcpy(web_api->home_wireless->SSID[ssid_index]->active, "0");
					break;
				case 1:
					strcpy(web_api->home_wireless->SSID[ssid_index]->active, "1");
					strcpy(web_api->home_wireless->SSID[ssid_index]->action, val_def->wireless_action_allow);
					break;
				case 2:
					strcpy(web_api->home_wireless->SSID[ssid_index]->active, "1");
					strcpy(web_api->home_wireless->SSID[ssid_index]->action, val_def->wireless_action_deny);
					break;
			}
			tmp_mac = web_api->home_wireless->SSID[ssid_index]->mac1;
			get_profile_str("AccessControlList0=", allmac, fd);
			content = allmac;
			for(i = 0; i < 8; i++)
			{
				strncpy(tmp_mac, content, 17);
				if(content[17] == ';')
				{
					content = content + 18;
					tmp_mac += 20;
				}
				else
					break;
			}
			if(!atoi(web_api->home_wireless->access))
			{
				system("ifconfig ra0 down");
				fprintf(stderr, "ifconfig ra0 has been down\n");
			}
			else
				fprintf(stderr, "ifconfig ra0 has been up\n");
#if 0			
//		 	sprintf(tmp,"KEYINDEX_64_%d=",ssid_index+1);
			get_profile_str("",web_api->home_wireless->SSID[ssid_index]->keyIndex_64, fd);
//			sprintf(tmp,"KEYINDEX_128_%d=",ssid_index+1);
			get_profile_str("",web_api->home_wireless->SSID[ssid_index]->keyIndex_128, fd);

			tmp_64=web_api->home_wireless->SSID[ssid_index]->key1_64;
			tmp_128=web_api->home_wireless->SSID[ssid_index]->key1_128;
			for(i=0;i<4;i++)
			{
			 	sprintf(tmp,"KEY%d_64_%d=",i+1,ssid_index+1);
				get_profile_str("",tmp_64, fd);
				tmp_64+=8;
				sprintf(tmp,"KEY%d_128_%d=",i+1,ssid_index+1);
				get_profile_str("",tmp_128, fd);
				tmp_128+=32;
			}
		
//	 		sprintf(tmp,"ENCRYPTION_%d=",ssid_index+1);
			get_profile_str("EncrypType=",web_api->home_wireless->SSID[ssid_index]->encryption, fd);
//		 	sprintf(tmp,"PRE-SHARED_%d=",ssid_index+1);
			get_profile_str("WPAPSK=",web_api->home_wireless->SSID[ssid_index]->pre_shared, fd);				

//			sprintf(tmp,"ACTIVE_%d=",ssid_index+1);
			get_profile_str("",web_api->home_wireless->SSID[ssid_index]->active, fd);	
//			sprintf(tmp,"ACTION_%d=",ssid_index+1);
			get_profile_str("",web_api->home_wireless->SSID[ssid_index]->action, fd);	

			tmp_mac=web_api->home_wireless->SSID[ssid_index]->mac1;
			for(i=0;i<8;i++)
			{
				sprintf(tmp,"MAC%d_%d=",i+1,ssid_index+1);
				get_profile_str("",tmp_mac, fd);	
				tmp_mac+=20;
			}
#endif				
	 	}
	 	
//		free(content);	
		close(fd);				
	} 
	else        //wjx 2007.4.16
	{
		strncpy(web_api->home_wireless->access,val_def->wireless_access_activated,8);		
		strncpy(web_api->home_wireless->channel,val_def->wireless_channel_taiwan,32);				
		strncpy(web_api->home_wireless->channel_num,"6",8);			
		strncpy(web_api->home_wireless->current_channel,"6",8);		
		strncpy(web_api->home_wireless->beacon,"100",8);				
		strncpy(web_api->home_wireless->RTS_CTS,"2347",8);				
		strncpy(web_api->home_wireless->Fragmentation,"2346",8);				
		strncpy(web_api->home_wireless->DTIM,"1",8);				
		strncpy(web_api->home_wireless->bg80211,val_def->wireless_80211bg_80211bg,16);		
			
		fprintf(stderr,"------ load  /etc/Wireless/RT61AP/RT61AP.dat\n");				
	}
		
}
//wtw add begin
void read_ipfilter_url_conf()                     //wjx 2007.3.6
{
	int  ret;		
	int  fd ;
	char *content = NULL;		
	struct stat buf;
	int index;
	char tmp[32];
	for (index=0;index<16;index++)
	{
		web_api->ipfilter_extend->urlfilter[index] =(T_URL_FILTER*)malloc(sizeof(T_URL_FILTER));
	  memset(web_api->ipfilter_extend->urlfilter[index],0,sizeof(T_URL_FILTER));			
	}

	
 	fd = open("/etc/ipfilter/ipfilter_url.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);			
	if (content == NULL)
		goto err; 
	ret = read(fd,content,buf.st_size);
	if (ret<=0)
		goto err;		
		
	config_get_value(content,web_api->ipfilter_extend->urlfilter_index,8,"urlfilter_index=");	
	
	config_get_value(content,web_api->ipfilter_extend->urlfilter_status,16,"urlfilter_active=");  //wjx 2007.4.13
	for (index=0;index<16;index++)
	{
//		sprintf(tmp,"urlfilter_active%d=",index+1);  
//	  config_get_value(content,web_api->ipfilter_extend->urlfilter[index]->index_status,16,tmp);
		sprintf(tmp,"urlfilter_url%d=",index+1);	  
	  config_get_value(content,web_api->ipfilter_extend->urlfilter[index]->url,48,tmp);	
	}
	
	close(fd);
	free(content);	
	fprintf(stderr,"------ load  /etc/ipfilter/ipfilter_url.conf  ok\n");		
	return; 
  
err:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
	fprintf(stderr,"------ load  /etc/ipfilter/ipfilter_url.conf  err\n");		
	strncpy(web_api->ipfilter_extend->urlfilter_index,"1",8);	
/*
	for (index=0;index<16;index++)
	{ 
	  strncpy(web_api->ipfilter_extend->urlfilter[index]->index_status,val_def->urlfilter_deactivated,16);
	}	
*/

	strncpy(web_api->ipfilter_extend->urlfilter_status,val_def->urlfilter_deactivated,16);
	
	return; 			
}



void read_ipfilter_app_conf()                     //wjx 2007.3.7
{
	int  ret;		
	int  fd ;
	char *content = NULL;		
	struct stat buf;

 	fd = open("/etc/ipfilter/ipfilter_app.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);			
	if (content == NULL)
		goto err; 
	ret = read(fd,content,buf.st_size);
	if (ret<=0)
		goto err;		
	
	config_get_value(content,web_api->ipfilter_extend->appfilter_status,16,"appfilter_status=");	
	config_get_value(content,web_api->ipfilter_extend->appfilter_icq_allowance,16,"appfilter_icq_allowance=");	
	config_get_value(content,web_api->ipfilter_extend->appfilter_msn_allowance,16,"appfilter_msn_allowance=");		
	config_get_value(content,web_api->ipfilter_extend->appfilter_ymsn_allowance,16,"appfilter_ymsn_allowance=");	
	config_get_value(content,web_api->ipfilter_extend->appfilter_ra_allowance,16,"appfilter_ra_allowance=");		
	
	close(fd);
	free(content);	
	fprintf(stderr,"------ load  /etc/ipfilter/ipfilter_app.conf  ok\n");		
	return; 
  
err:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
	fprintf(stderr,"------ load  /etc/ipfilter/ipfilter_app.conf  err\n");		

	strncpy(web_api->ipfilter_extend->appfilter_status,val_def->appfilter_deactivated,16);
	strncpy(web_api->ipfilter_extend->appfilter_icq_allowance,val_def->appfilter_icq_allow,16);	
	strncpy(web_api->ipfilter_extend->appfilter_msn_allowance,val_def->appfilter_msn_allow,16);		
	strncpy(web_api->ipfilter_extend->appfilter_ymsn_allowance,val_def->appfilter_ymsn_allow,16);		
	strncpy(web_api->ipfilter_extend->appfilter_ra_allowance,val_def->appfilter_ra_allow,16);		
	return; 			
}

//wtw add end
void read_adv_firewall_settings()                     //wjx 2007.3.5
{
	int  ret;		
	int  fd ;
	char *content = NULL;		
	struct stat buf;

	web_api->adv_firewall =( T_ADV_FIREWALL*)malloc(sizeof(T_ADV_FIREWALL));
	memset(web_api->adv_firewall,0,sizeof(T_ADV_FIREWALL));
	
 	fd = open("/etc/firewall.conf",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)malloc(buf.st_size+1);			
	if (content == NULL)
		goto err; 
	ret = read(fd,content,buf.st_size);
	if (ret<=0)
		goto err;		
		
	config_get_value(content,web_api->adv_firewall->firewall_status,16,"firewall_status=");
	config_get_value(content,web_api->adv_firewall->spi_status,16,"spi_status=");	
	
	close(fd);
	free(content);	
	fprintf(stderr,"------ load  /etc/firewall.conf  ok\n");		
	return; 
  
err:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
	fprintf(stderr,"------ load  /etc/firewall.conf  err\n");		
	strncpy(web_api->adv_firewall->firewall_status,val_def->fw_firewall_disable,16);
	strncpy(web_api->adv_firewall->spi_status,val_def->fw_spi_disable,16);	
	return; 			
}
/*______________________________________________________________________________
**	function name:check_nat_enable
**
**	description: 
*     check nat status of all pvc. if any pvc set the nat enable, flush raw table.
*                                            if no pvc set the nat enable, set raw table to NOTRACK.
**	parameters: 
*     state:nat state now.-1:boot
*                         0:no rule in raw table.
*                         1:use NOTRACK module.
**	global:
*     none
**	return: 
*     0:no rule in raw table
*     1:use NOTRACK module
**	call:
*     none
**	revision:
*     1.shnwind 20080417
**____________________________________________________________________________*/
int check_nat_enable(int state){
  
  int i,nat_on=0;

  for(i=0;i<8;i++){//pvc0~pvc7
    if((!strcmp(web_api->wan[i]->status,val_def->wan_activated)) &&
        (strcmp(web_api->wan[i]->isp, val_def->wan_bridge)) &&
        (!strcmp(web_api->wan[i]->nat_enable,val_def->wan_natenable))){
      nat_on = 1;
      break;
    }     
  }
  if(nat_on != state){
    if(nat_on){
      system("iptables -t raw -F");
    }else{
    	//Andy Chiu, 2015/08/03. modify for NAT disable issue.
//      system("iptables -t raw -A PREROUTING -j NOTRACK");
//      system("iptables -t raw -A OUTPUT -j NOTRACK");
    }
  }
  return nat_on;
}/*End check_nat_enable*/

/*______________________________________________________________________________
**	function name:check_MTU_change
**
**	description: 
*     Return 1 if user change MTU.
**	parameters: 
*     filter_state:0:CPE does no load ip_filter module.
*                  1:use ip_filter module.
**	global:
*     none
**	return: 
*     0:CPE does no load ip_filter module.
*     1:use ip_filter module.
**	call:
*     none
**	revision:
*     1.shnwind 20080417
**____________________________________________________________________________*/
int check_MTU_change(int filter_state)
{
  int i,filter_on=0;
  
  for(i=0;i<8;i++){//pvc0~pvc7
    if((!strcmp(web_api->wan[i]->status,val_def->wan_activated)) &&
       (strcmp(web_api->wan[i]->isp, val_def->wan_bridge))){
       if((!strcmp(web_api->wan[i]->isp, val_def->wan_dynamic_ip)) || 
         (!strcmp(web_api->wan[i]->isp, val_def->wan_static_ip))){
         if(web_api->wan[i]->mtu != 0){
           filter_on = 1;
           break;
         }        
       }else if(!strcmp(web_api->wan[i]->isp, val_def->wan_pppoe)){
         if(web_api->wan[i]->mss != 0){
           filter_on = 1;
           break;
         }
       }       
    }
  }  

  if(filter_state != filter_on){
    if(filter_on){
#if KERNEL_2_6_36
			system("insmod /lib/modules/2.6.36/kernel/net/ipv4/netfilter/iptable_filter.ko");
			system("insmod /lib/modules/2.6.36/kernel/net/ipv4/netfilter/ipt_REJCT.ko");
#else
			system("insmod /lib/modules/2.6.22.15/kernel/net/ipv4/netfilter/iptable_filter.ko");			
			system("insmod /lib/modules/2.6.22.15/kernel/net/ipv4/netfilter/ipt_REJCT.ko");			
#endif
    }else{
      system("iptables -t filter -F");
      system("rmmod iptable_filter");
      system("rmmod ipt_REJECT");
    }
  }
  return filter_on;
}
