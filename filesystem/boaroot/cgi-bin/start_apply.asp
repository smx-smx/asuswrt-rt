<%
If Request_Form("refresh_networkmap_Flag") = "1" then
	ClientList_Refresh()	
end if
If Request_Form("update_networkmap_Flag") = "1" then
	ClientList_Update()
end if
If Request_Form("experience_DSL_Flag") = "1" then
	tcWebApi_Set("Misc_Entry","experience_DSL_fb","experience_DSL_Feedback")
End if
If Request_Form("qis_notification_iptv_Flag") = "1" then
	tcWebApi_Set("Misc_Entry","qis_notification_iptv","qis_notification_iptv")
End if
if Request_Form("action_script") = "aidisk_asusddns_register" then
	tcWebApi_Set("Ddns_Entry", "Active", "ddns_enable_x")
	tcWebApi_Set("Ddns_Entry", "MYHOST", "ddns_hostname_x")
	tcWebApi_Set("Ddns_Entry", "SERVERNAME", "ddns_server_x")
	tcWebApi_Commit("Ddns_Entry")
elseif Request_Form("action_script") = "restart_ddns" then
	tcWebApi_Set("Ddns_Entry", "Active", "ddns_enable_x")
	tcWebApi_Set("Ddns_Entry", "MYHOST", "ddns_hostname_x")
	tcWebApi_Set("Ddns_Entry", "SERVERNAME", "ddns_server_x")
	tcWebApi_Set("Ddns_Entry", "USERNAME", "ddns_username_x")
	tcWebApi_Set("Ddns_Entry", "PASSWORD", "ddns_passwd_x")
	tcWebApi_Set("Ddns_Entry", "WILDCARD", "ddns_wildcard_x")
	tcWebApi_Set("Ddns_Entry", "ddns_wan_unit", "ddns_wan_unit")
	tcWebApi_Commit("Ddns_Entry")
elseif Request_Form("action_script") = "notif_hint" then
	tcWebApi_Set("WebCustom_Entry", "webs_notif_flag", "noti_notif_Flag")
	tcWebApi_Commit("WebCustom_Entry")	
elseif Request_Form("action_script") = "disable_aidisk_ddns" then
	tcWebApi_Set("Ddns_Entry", "Active", "ddns_enable_x")
	tcWebApi_Commit("Ddns_Entry")
elseif Request_Form("current_page") = "Advanced_VPN_PPTP.asp" then
	tcWebApi_Set("VPNControl_Entry", "VPNServer_enable", "VPNServer_enable")
	tcWebApi_Set("VPNControl_Entry", "VPNServer_mode", "VPNServer_mode")
	tcWebApi_Set("PPTP_Entry", "pptpd_enable", "pptpd_enable")
	tcWebApi_Set("PPTP_Entry", "pptpd_ms_network", "pptpd_ms_network")
	tcWebApi_Set("PPTP_Entry", "pptpd_broadcast", "pptpd_broadcast")
	tcWebApi_Set("PPTP_Entry", "pptpd_clientlist", "pptpd_clientlist")
	tcWebApi_Set("PPTP_Entry", "pptpd_sr_rulelist", "pptpd_sr_rulelist")
	tcWebApi_Set("PPTP_Entry", "pptpd_clients", "pptpd_clients")
	tcWebApi_Set("PPTP_Entry", "pptpd_chap", "pptpd_chap")
	tcWebApi_Set("PPTP_Entry", "pptpd_mppe", "pptpd_mppe")
	tcWebApi_Set("PPTP_Entry", "pptpd_dns1", "pptpd_dns1")
	tcWebApi_Set("PPTP_Entry", "pptpd_dns2", "pptpd_dns2")
	tcWebApi_Set("PPTP_Entry", "pptpd_wins1", "pptpd_wins1")
	tcWebApi_Set("PPTP_Entry", "pptpd_wins2", "pptpd_wins2")
	tcWebApi_Set("PPTP_Entry", "pptpd_mru", "pptpd_mru")
	tcWebApi_Set("PPTP_Entry", "pptpd_mtu", "pptpd_mtu")
	update_variables()
	tcWebApi_commit("PPTP_Entry")
elseif Request_Form("current_page") = "Advanced_VPN_OpenVPN.asp" then
	if Request_Form("formname") = "form" then
		tcWebApi_Set("VPNControl_Entry", "VPNServer_enable", "VPNServer_enable")
		tcWebApi_Set("VPNControl_Entry", "VPNServer_mode", "VPNServer_mode")
		tcWebApi_Set("OpenVPN_Common", "vpn_server_clientlist", "vpn_server_clientlist")
		tcWebApi_Set("OpenVPN_Common", "vpn_serverx_eas", "vpn_serverx_eas")
		tcWebApi_Set("OpenVPN_Common", "vpn_serverx_dns", "vpn_serverx_dns")
		tcWebApi_Set("OpenVPN_Common", "vpn_server_unit", "vpn_server_unit")
		tcWebApi_Set("OpenVPN_Entry", "iface", "vpn_server_if")
		tcWebApi_Set("OpenVPN_Entry", "proto", "vpn_server_proto")
		tcWebApi_Set("OpenVPN_Entry", "port", "vpn_server_port")
		tcWebApi_Set("OpenVPN_Entry", "firewall", "vpn_server_firewall")
		tcWebApi_Set("OpenVPN_Entry", "crypt", "vpn_server_crypt")
		tcWebApi_Set("OpenVPN_Entry", "igncrt", "vpn_server_igncrt")
		tcWebApi_Set("OpenVPN_Entry", "hmac", "vpn_server_hmac")
		tcWebApi_Set("OpenVPN_Entry", "reneg", "vpn_server_reneg")
		tcWebApi_Set("OpenVPN_Entry", "comp", "vpn_server_comp")
		tcWebApi_Set("OpenVPN_Entry", "cipher", "vpn_server_cipher")
		tcWebApi_Set("OpenVPN_Entry", "digest", "vpn_server_digest")
		tcWebApi_Set("OpenVPN_Entry", "dhcp", "vpn_server_dhcp")
		tcWebApi_Set("OpenVPN_Entry", "pool_start", "vpn_server_r1")
		tcWebApi_Set("OpenVPN_Entry", "pool_end", "vpn_server_r2")
		tcWebApi_Set("OpenVPN_Entry", "subnet", "vpn_server_sn")
		tcWebApi_Set("OpenVPN_Entry", "netmask", "vpn_server_nm")
		tcWebApi_Set("OpenVPN_Entry", "local", "vpn_server_local")
		tcWebApi_Set("OpenVPN_Entry", "remote", "vpn_server_remote")
		tcWebApi_Set("OpenVPN_Entry", "poll", "vpn_server_poll")
		tcWebApi_Set("OpenVPN_Entry", "redirect_gateway", "vpn_server_rgw")
		tcWebApi_Set("OpenVPN_Entry", "push_lan", "vpn_server_plan")
		tcWebApi_Set("OpenVPN_Entry", "push_dns", "vpn_server_pdns")
		tcWebApi_Set("OpenVPN_Entry", "ccd", "vpn_server_ccd")
		tcWebApi_Set("OpenVPN_Entry", "c2c", "vpn_server_c2c")
		tcWebApi_Set("OpenVPN_Entry", "ccd_excl", "vpn_server_ccd_excl")
		tcWebApi_Set("OpenVPN_Entry", "ccd_val", "vpn_server_ccd_val")
		tcWebApi_Set("OpenVPN_Entry", "tls_keysize", "vpn_server_tls_keysize")
		tcWebApi_Set("OpenVPN_Entry", "custom", "vpn_server_custom")
		update_variables()
	end if
	tcWebApi_Save()
elseif Request_Form("current_page") = "Advanced_VPNClient_Content.asp" then	
	if Request_form("listFlag") = "1" Then
		tcWebApi_Set("VPNC_Entry", "clientlist", "vpnc_clientlist")
		tcWebApi_Set("VPNC_Entry", "pptp_options_list", "vpnc_pptp_options_x_list")
	end if
	if Request_form("connFlag") = "1" Then
		tcWebApi_Set("VPNC_Entry", "proto", "vpnc_proto")
		tcWebApi_Set("VPNC_Entry", "heartbeat", "vpnc_heartbeat_x")
		tcWebApi_Set("VPNC_Entry", "username", "vpnc_pppoe_username")
		tcWebApi_Set("VPNC_Entry", "passwd", "vpnc_pppoe_passwd")	
		tcWebApi_Set("VPNC_Entry", "auto_conn", "vpnc_auto_conn")
		tcWebApi_Set("VPNC_Entry", "pptp_options", "vpnc_pptp_options_x")
		update_variables()
	elseif Request_form("connFlag") = "2" Then
		tcWebApi_Set("VPNC_Entry", "proto", "vpnc_proto")
		tcWebApi_Set("OpenVPN_Common", "vpn_client_unit", "vpn_client_unit")
		tcWebApi_Set("OpenVPN_Common", "vpn_clientx_eas", "vpn_clientx_eas")
		tcWebApi_Set("WebCurSet_Entry", "openvpn_id", "openvpn_id")
		tcWebApi_Set("OpenVPN_Entry", "username", "vpn_client_username")
		tcWebApi_Set("OpenVPN_Entry", "password", "vpn_client_password")
		update_variables()
	end if
	tcWebApi_commit("VPNC_Entry")
elseif Request_Form("current_page") = "Main_LogStatus_Content.asp" then	
	if Request_Form("clearLog_Flag") = "1" then
		tcWebApi_commit("LogAccess_ClearLog")
	end if
elseif Request_Form("current_page") = "Advanced_SettingBackup_Content.asp" then
	if request_Form("postflag") = "1" then
		tcWebApi_set("System_Entry","upgrade_fw_status","value_NONE")
		tcWebApi_set("System_Entry","upgrade_fw","HTML_HEADER_TYPE")
		tcWebApi_CommitWithoutSave("System_Entry")
	elseif request_Form("postflag") = "5" then
		tcWebApi_set("System_Entry","upgrade_fw_status","value_NONE")
	end if
elseif Request_Form("current_page") = "Advanced_Feedback.asp" then
	If Request_Form("DslDiagFlag") = "1" Then
		tcWebApi_Set("DslDiag_Entry","dslx_diag_enable","dslx_diag_enable")
		tcWebApi_Set("DslDiag_Entry","dslx_diag_duration","dslx_diag_duration")
		If Request_Form("chg2jffsFlag") = "1" Then
			tcWebApi_Set("DslDiag_Entry","dslx_diag_log_path","dslx_diag_log_path")
		end if
	end if
	If Request_Form("saveFlag") = "1" Then
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_country","fb_country")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_ISP","fb_ISP")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_Subscribed_Info","fb_Subscribed_Info")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_email","fb_email")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_serviceno","fb_serviceno")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_availability","fb_availability")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment0","fb_comment0")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment1","fb_comment1")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment2","fb_comment2")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment3","fb_comment3")
		tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment4","fb_comment4")
		tcWebApi_Set("GUITemp_Entry0","browserInfo","browserInfo")
		tcWebApi_Set("GUITemp_Entry0","WANConnCurState","WANConnCurState")
		tcWebApi_Set("PushMail_Entry","PM_attach_syslog","PM_attach_syslog")
		tcWebApi_Set("PushMail_Entry","PM_attach_cfgfile","PM_attach_cfgfile")
		tcWebApi_Set("PushMail_Entry","PM_attach_iptables","PM_attach_iptables")
		tcWebApi_Set("PushMail_Entry","PM_attach_modemlog","PM_attach_modemlog")
		gen_modemlog()
		tcWebApi_commit("PushMail")		
	end if
elseif Request_Form("current_page") = "Feedback_Info.asp" then
	If Request_Form("DslDiagFlag") = "1" Then
		tcWebApi_Set("DslDiag_Entry","dslx_diag_state","dslx_diag_state")
		tcWebApi_commit("DslDiag")
	end if	
elseif Request_Form("current_page") = "Advanced_WANPort_Content.asp" then
	tcWebApi_Set("WebCurSet_Entry","wandog_pingfail","wandog_pingfail")
	tcWebApi_Set("Dualwan_Entry","wans_mode","wans_mode")
	tcWebApi_Set("Dualwan_Entry","wans_dualwan","wans_dualwan")
	tcWebApi_Set("Dualwan_Entry","wans_lanport","wans_lanport")
	tcWebApi_Set("Dualwan_Entry","wans_lb_ratio","wans_lb_ratio")
	tcWebApi_Set("Dualwan_Entry","wans_routing_enable","wans_routing_enable")
	tcWebApi_Set("Dualwan_Entry","wans_routing_rulelist","wans_routing_rulelist")
	tcWebApi_Set("Dualwan_Entry","wan0_routing_isp_enable","wan0_routing_isp_enable")
	tcWebApi_Set("Dualwan_Entry","wan0_routing_isp","wan0_routing_isp")
	tcWebApi_Set("Dualwan_Entry","wan1_routing_isp_enable","wan1_routing_isp_enable")
	tcWebApi_Set("Dualwan_Entry","wan1_routing_isp","wan1_routing_isp")
	tcWebApi_Set("Dualwan_Entry","wandog_enable","wandog_enable")
	tcWebApi_Set("Dualwan_Entry","wandog_target","wandog_target")
	tcWebApi_Set("Dualwan_Entry","wandog_interval","wandog_interval")
	tcWebApi_Set("Dualwan_Entry","wandog_maxfail","wandog_maxfail")
	tcWebApi_Set("Dualwan_Entry","wandog_delay","wandog_delay")
	tcWebApi_Set("Dualwan_Entry","wandog_fb_count","wandog_fb_count")
	tcWebApi_Set("Dualwan_Entry","wandog_off_guestwl","off_guestwl")
	tcWebApi_Set("Dualwan_Entry","wandog_resume_guestwl","resume_guestwl")
	tcWebApi_commit("Dualwan")
elseif Request_Form("current_page") = "Advanced_DSL_Content.asp" then
	If Request_Form("next_page") = "Advanced_Modem_Content.asp" Then
		tcWebApi_Set("WebCurSet_Entry","dev_pvc","USBInfo_PVC")
		tcWebApi_Set("Wan_Common","TransMode","USB_TransMode")
		tcWebApi_Set("WebCurSet_Entry","wan_pvc","USBInfo_PVC")
	end if
elseif Request_Form("current_page") = "Advanced_Modem_Content.asp" then
	If Request_Form("next_page") = "Advanced_DSL_Content.asp" Then
		tcWebApi_Set("WebCurSet_Entry","dev_pvc","ModemInfo_PVC")
		tcWebApi_Set("Wan_Common","TransMode","ModemTransMode")
		tcWebApi_Set("WebCurSet_Entry","wan_pvc","ModemInfo_PVC")
	end if
elseif Request_Form("current_page") = "index.asp" then
	If Request_Form("flag") = "dualwan" Then
		tcWebApi_Set("Dualwan_Entry","wans_dualwan","wans_dualwan")
		tcWebApi_commit("Dualwan")
	end if
	If Request_Form("flag") = "wan_enable" Then
		tcWebApi_Set("Wan_PVC","Active","wan_active")
		tcWebApi_commit("Wan_PVC")
	end if
end if
%>
<html>

<!--start_apply.asp-->
<head>
<title></title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<script type="text/javascript">
var page_modified = 0;
var restart_time = 0;
var rc_support = "<% tcWebApi_get("SysInfo_Entry", "rc_support", "s") %>";
var dsl_support = rc_support.search("dsl");
var current_page = '<% get_parameter("current_page") %>';
var next_page = '<% get_parameter("next_page") %>';
var action_script = '<% get_parameter("action_script") %>';
var action_mode = '<% get_parameter("action_mode") %>';
var action_wait = '<% get_parameter("action_wait") %>';
var group_id = '<% get_parameter("group_id") %>';
var getflag = '<% get_parameter("flag") %>';
var postflag = '<% get_parameter("postflag") %>';
function started_validating(){
}
function done_validating(action){
	parent.done_validating(action, group_id);
}
function started_committing(){
}
function done_committing(){
	page_modified = 1;
}
function no_changes_and_no_committing(){
}
function invalid_variable_value(variable, bad_value, message){
}
function restart_needed_time(second){
	restart_time = second;
}
function delay_redirect(next_url){
document.redirectForm.action = next_url;
document.redirectForm.target = "";
document.redirectForm.submit();
}
function top_delay_redirect(next_url){
	document.redirectForm.action = next_url;
	document.redirectForm.target = "_parent";
	document.redirectForm.submit();
}
function qis_delay_redirect(next_url){
	parent.parent.document.redirectForm.action = next_url;
	parent.parent.document.redirectForm.target = "contentM";
	parent.parent.document.redirectForm.submit();
}
function redirect_page(next_url){
	parent.location.href = "/"+next_url;
}
function redirect(){
	parent.location.href = 'http://'+parent.document.form.lan_ipaddr.value+'/QIS_wizard.asp?flag=detect';
}
function redirect_qis(){
	parent.location.href = 'http://'+parent.document.form.lan_ipaddr.value+'/index.asp';
}
function initial(){
	if(getflag == "background") {
		return false;
	}
	else if(getflag == "set_language"){
		if(navigator.appName.indexOf("Microsoft") >= 0)
			parent.parent.location.reload();
		else
			parent.parent.location.href = parent.parent.location.href;
	}
	else if(next_page.indexOf("QIS_") >= 0){
		if(dsl_support != -1){
			setTimeout("delay_redirect('"+next_page+"');", restart_time*1000);
		}
		else {
			if(getflag.length > 0)
				document.redirectForm.flag.value = getflag;
			if(parent.$("hidden_frame")){
				setTimeout("qis_delay_redirect('qis/QIS_verify_detect.asp');", restart_time*1000);
			}
			else{
				if(getflag == "auto_way_vpn"){
					parent.showLoading(restart_time);
					setTimeout("delay_redirect('qis/QIS_verify_detect.asp');", restart_time*1000);
					return;
				}
				else if(getflag == "detect"){
					parent.showLoading(restart_time);
					setTimeout("delay_redirect('qis/QIS_detect.asp');", restart_time*1000);
					return;
				}
				else if(getflag == "verify_detect"){
					parent.showLoading(restart_time);
					setTimeout("delay_redirect('qis/QIS_verify_detect.asp');", restart_time*1000);
					return;
				}
				else{
					parent.showLoading(restart_time, "waiting");
					if(parent.parent.isWLclient())
						setTimeout("parent.parent.detectLANstatus('"+next_page+"', '"+getflag+"');", restart_time*1000);
					else
						setTimeout("top_delay_redirect('"+next_page+"');", restart_time*1000);
				}
			}
		}
	}
	else if(current_page.indexOf("Feedback_Info") >= 0){
		setTimeout("redirect_page(next_page)", 200);
	}
	else if(current_page.indexOf("Advanced_Feedback") >= 0){
		return;
	}	
	else if(getflag == "ddnscheck"){
		setTimeout("parent.go_index_page();", 5000);
	}
	else if(getflag == "aidisk"){
		parent.Webdav_status = (parent.Webdav_status == 1) ? 0 : 1;
		parent.resultOfSwitchWebdavStatus();
	}
	else if(action_script.indexOf("aidisk_asusddns_register") >= 0){
		setTimeout("parent.checkDDNSReturnCode();", 1000);
	}
	else if(current_page.indexOf("Advanced_VPNClient") >= 0) {		
			setTimeout("redirect_page(next_page)", 500);
	}
	else if(current_page.indexOf("Advanced_DSL_Content") >= 0) {
		if(next_page.length > 0){
			setTimeout("redirect_page(next_page)", 200);
		}
	}
	else if(current_page.indexOf("Advanced_Modem_Content") >= 0) {
		if(next_page.length > 0){
			setTimeout("redirect_page(next_page)", 200);
		}
	}
	else if(action_script.indexOf("restart_ddns") >= 0) {
		setTimeout("parent.checkDDNSReturnCode();", 1000);
	}
	else if(action_script.indexOf("start_webs_upgrade") >= 0) {
		setTimeout("parent.showLoadingBar(restart_time);", 1000);

	}else if(current_page.indexOf("Advanced_SettingBackup_Content.asp") >= 0){
		if(postflag == "5"){
				setTimeout("alert(\"<% tcWebApi_Get("String_Entry", "SET_fail_desc", "s") %>\");", 5000);
				setTimeout("parent.redirect_self();", 5000);
		}else if(postflag == "1"){
				parent.chk_upgrade();
				setTimeout("parent.redirect();", 120000); //90000
		}
	}
	else if(page_modified == 1){
		parent.showLoading(restart_time, "waiting");
		if(current_page.length > 0){
			if(current_page.indexOf("clients.asp") >= 0){
				document.redirectForm.flag.value = "Client";
				setTimeout("top_delay_redirect('/');", restart_time*1000);
			}
			else if(current_page == "Advanced_OperationMode_Content.asp"){
				parent.$('drword').innerHTML = "<%tcWebApi_get("String_Entry","LAN_IP_changed_suggedtion1","s")%>"+parent.document.form.lan_ipaddr.value+"<%tcWebApi_get("String_Entry","LAN_IP_changed_suggedtion2","s")%><br/>";
				setTimeout("hideLoading() ",20000);
				setTimeout("parent.dr_advise();",20000);
				setTimeout("redirect()",35000);
			}
			else if(current_page == "Advanced_LAN_Content.asp"){
				parent.$('drword').innerHTML = "<%tcWebApi_get("String_Entry","LAN_IP_changed_suggedtion1","s")%>"+parent.document.form.lan_ipaddr.value+"<%tcWebApi_get("String_Entry","LAN_IP_changed_suggedtion2","s")%><br/>";
				setTimeout("hideLoading() ",20000);
				setTimeout("parent.dr_advise();",20000);
				setTimeout("redirect_qis()",35000);
			}
			else if(getflag == "nodetect"){
				setTimeout("top_delay_redirect('"+current_page+"');", restart_time*1000);
			}
			else if(action_script == "restart_webdav"){
				setTimeout("top_delay_redirect('"+current_page+"');", restart_time*1000);
			}
			else{
				setTimeout("parent.parent.detectLANstatus('"+current_page+"', '');", restart_time*1000);
			}
		}
	}
	else if(action_script == "reboot"){
		parent.showLoading(restart_time);
		setTimeout("top_delay_redirect('"+next_page+"');", restart_time*1000);
	}
	else{
		if(current_page.length > 0){			
			setTimeout("top_delay_redirect('"+current_page+"');", restart_time*1000);
		}
		else if(next_page.length > 0){
			setTimeout("delay_redirect('"+next_page+"');", restart_time*1000);
		}
	}
}
function hideLoading(){
	parent.$("loadingBlock").style.visibility = "hidden";
	parent.$("Loading").style.width = "10px";
}
</script>
</head>
<body onload="initial();">
<form method="post" name="redirectForm" action="#" target="">
<input type="hidden" name="flag" value="">
<input type="hidden" name="prev_page" value="">
</form>




</body>

<!--start_apply.asp-->
</html>

