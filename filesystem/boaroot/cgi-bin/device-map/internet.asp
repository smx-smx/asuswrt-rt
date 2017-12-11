<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="images/favicon.png">
<link rel="icon" href="images/favicon.png">
<title>Untitled Document</title>
<link rel="stylesheet" type="text/css" href="/NM_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script>
var $j = jQuery.noConflict();

<% wanlink(); %>
<% first_wanlink(); %>
<% secondary_wanlink(); %>

var wanip = wanlink_ipaddr();
var wannetmask = wanlink_netmask();
var wandns = wanlink_dns();
var wangateway = wanlink_gateway();
var wanxip = wanlink_xipaddr();
var wanxnetmask = wanlink_xnetmask();
var wanxdns = wanlink_xdns();
var wanxgateway = wanlink_xgateway();
var wan_lease = wanlink_lease();
var wan_expires = wanlink_expires();

var first_wanip = first_wanlink_ipaddr();
var first_wannetmask = first_wanlink_netmask();
var first_wandns = first_wanlink_dns();
var first_wangateway = first_wanlink_gateway();
var first_wanxip = first_wanlink_xipaddr();
var first_wanxnetmask = first_wanlink_xnetmask();
var first_wanxdns = first_wanlink_xdns();
var first_wanxgateway = first_wanlink_xgateway();
var first_lease = first_wanlink_lease();
var first_expires = first_wanlink_expires();

var secondary_wanip = secondary_wanlink_ipaddr();
var secondary_wannetmask = secondary_wanlink_netmask();
var secondary_wandns = secondary_wanlink_dns();
var secondary_wangateway = secondary_wanlink_gateway();
var secondary_wanxip = secondary_wanlink_xipaddr();
var secondary_wanxnetmask = secondary_wanlink_xnetmask();
var secondary_wanxdns = secondary_wanlink_xdns();
var secondary_wanxgateway = secondary_wanlink_xgateway();
var secondary_lease = secondary_wanlink_lease();
var secondary_expires = secondary_wanlink_expires();

var wanstate = -1;
var wansbstate = -1;
var wanauxstate = -1;
var old_link_internet = -1;

var wans_cap = '<% tcWebApi_Get("Dualwan_Entry", "wans_cap", "s") %>';
var wans_dualwan = '<% tcWebApi_Get("Dualwan_Entry", "wans_dualwan", "s") %>';
var pri_if = wans_dualwan.split(" ")[0];
var sec_if = wans_dualwan.split(" ")[1];
var wans_lanport = '<% tcWebApi_Get("Dualwan_Entry", "wans_lanport", "s") %>';
var wan0_primary = '<% nvram_get("wan0_primary"); %>';
var wans_mode = '<%tcWebApi_Get("Dualwan_Entry", "wans_mode", "s")%>';
var loadBalance_Ratio = '<%tcWebApi_Get("Dualwan_Entry", "wans_lb_ratio", "s")%>';
var wan_common_transmode = '<%tcWebApi_Get("Wan_Common", "TransMode", "s")%>';

var wan_unit = '<% tcWebApi_Get("WebCurSet_Entry", "wan_unit", "s") %>';
if('<% tcWebApi_Get("Wan_PVC", "Active", "s") %>' == 'Yes')
	var wan_enable = 1;
else
	var wan_enable = 0;

function add_lanport_number(if_name)
{
	if (if_name == "lan") {
		return "lan" + wans_lanport;
	}
	return if_name;
}

function format_time(seconds, error)
{
	if (seconds <= 0)
		return error;
	var total = seconds;
	var value = "";
	var Seconds = total % 60; total = ~~(total / 60);
	var Minutes = total % 60; total = ~~(total / 60);
	var Hours   = total % 24;
	var Days = ~~(total / 24);
	if (Days != 0)
		value += Days.toString() + " <%tcWebApi_Get("String_Entry", "Day", "s")%> ";
	if (Hours != 0)
		value += Hours.toString() + " <%tcWebApi_Get("String_Entry", "Hour", "s")%> ";
	if (Minutes != 0)
		value += Minutes.toString() + " <%tcWebApi_Get("String_Entry", "Minute", "s")%> ";
	if (Seconds != 0)
		value += Seconds.toString() + " <%tcWebApi_Get("String_Entry", "Second", "s")%>";
	return value;
}

function showtext2(obj, str, visible)
{
	if (obj) {
		obj.innerHTML = str;
		obj.style.display = (visible) ? "" : "none";
	}
}

function initial(){	
	// if dualwan enabled , show dualwan status
	if(parent.wans_flag){
		var unit = parent.document.form.dual_wan_flag.value; // 0: Priamry WAN, 1: Secondary WAN		
		pri_if = add_lanport_number(pri_if);
		sec_if = add_lanport_number(sec_if);
		pri_if = pri_if.toUpperCase();
		sec_if = sec_if.toUpperCase();

		if(sec_if != 'NONE'){
			$("dualwan_row_main").style.display = "";
			// DSLTODO, need ajax to update failover status
			$('dualwan_mode_ctrl').style.display = "";
			$('wan_enable_button').style.display = "none";

			if(wans_mode == "lb"){
				//document.getElementById("wansMode").value = 1;
				$('dualwan_row_main').style.display = "none";
				//$('loadbalance_config_ctrl').style.display = "";
				showtext($("loadbalance_config"), loadBalance_Ratio);

				if (wan0_primary == '1') {
					showtext($j("#dualwan_current")[0], pri_if);
				}
				else {
					showtext($j("#dualwan_current")[0], sec_if);
				}
				showtext($("dualwan_mode"), "<%tcWebApi_Get("String_Entry", "dualwan_mode_lb", "s")%>");
				loadBalance_form(unit);
			}
			else{
				//document.getElementById("wansMode").value = 2;
				showtext($("dualwan_mode"), "<%tcWebApi_Get("String_Entry", "dualwan_mode_fo", "s")%>");
				failover_form(unit, pri_if, sec_if);
			}
		}
	}
	else{
		var unit = 0;
		pri_if = pri_if.toUpperCase();
		if(pri_if== "DSL")
                        document.getElementById("DSL_tab").style.display = "";
                else
                        document.getElementById("DSL_tab").style.display = "none";
		if ((wanlink_type() == "dhcp" || wanlink_xtype() == "dhcp") && wans_dualwan.split(" ")[0]!="usb") {
			document.getElementById('primary_lease_ctrl').style.display = "";
			document.getElementById('primary_expires_ctrl').style.display = "";
		}
	}

	if(sw_mode == 1){
		setTimeout("update_wanip();", 1);
		$('goSetting').style.display = "";

		if(dualWAN_support){
			if(dsl_support && wans_dualwan.split(" ")[0] == "usb" && parent.document.form.dual_wan_flag.value == 0){
				$("goDualWANSetting").style.display = "none";
				$("dualwan_enable_button").style.display = "none";
			}
			else if(parent.document.form.dual_wan_flag.value == 0){
				$("goDualWANSetting").style.display = "none";
				$("dualwan_enable_button").style.display = "";
			}
			else{
				$("goDualWANSetting").style.display = "";
				$("dualwan_enable_button").style.display = "none";
			}
		}
		else{
			$("goDualWANSetting").style.display = "none";
			$("dualwan_enable_button").style.display = "none";
		}

	}
	else{
		$("rt_table").style.display = "none";
		$("ap_table").style.display = "";
		if(sw_mode == 3)
			$('RemoteAPtd').style.display = "none";

		if((sw_mode == 2 || sw_mode == 3 || sw_mode == 4) && decodeURIComponent("<% nvram_char_to_ascii("WLANConfig11b", "wlc_ssid"); %>").length >= 28){
			showtext($("RemoteAP"), decodeURIComponent("<% nvram_char_to_ascii("WLANConfig11b", "wlc_ssid"); %>").substring(0, 26)+"...");
			$("RemoteAPtd").title = decodeURIComponent("<% nvram_char_to_ascii("WLANConfig11b", "wlc_ssid"); %>");
		}else
			showtext($("RemoteAP"), decodeURIComponent("<% nvram_char_to_ascii("WLANConfig11b", "wlc_ssid"); %>"));

		if(lanproto == "static")
			showtext($("LanProto"), "<%tcWebApi_Get("String_Entry", "BOP_ctype_title5", "s")%>");
		else
			showtext($("LanProto"), "<%tcWebApi_Get("String_Entry", "BOP_ctype_title1", "s")%>");

		if(sw_mode == 2 || sw_mode == 4)
			$('sitesurvey_tr').style.display = "";
	}

	update_connection_type(unit);
}

function update_connection_type(dualwan_unit){
	if(parent.wans_flag){
		if(dualwan_unit == 0)
			var wanlink_type_conv = first_wanlink_type();
		else
			var wanlink_type_conv = secondary_wanlink_type();
	}
	else
		var wanlink_type_conv = wanlink_type();

/*
	if (dsl_support) {
		if( wans_dualwan.split(" ")[dualwan_unit] == "dsl" ) {
			if(dslx_transmode == "atm") {
				if (dslproto == "pppoa" || dslproto == "ipoa")
					wanlink_type_conv = dslproto;
			}
		}
	}
*/
	if(wanlink_type_conv == "dhcp")
		wanlink_type_conv = "<%tcWebApi_Get("String_Entry", "BOP_ctype_title1", "s")%>";
	else if(wanlink_type_conv == "static")
		wanlink_type_conv = "<%tcWebApi_Get("String_Entry", "BOP_ctype_title5", "s")%>";
	else if(wanlink_type_conv == "pppoe" || wanlink_type_conv == "PPPOE")
		//wanlink_type_conv = "PPPoE";
		wanlink_type_conv = "PPPoE/PPPoA";
	else if(wanlink_type_conv == "pptp")
		wanlink_type_conv = "PPTP";
	else if(wanlink_type_conv == "l2tp")
		wanlink_type_conv = "L2TP";
	else if(wanlink_type_conv == "pppoa")
		wanlink_type_conv = "PPPoA";
	else if(wanlink_type_conv == "ipoa")
		wanlink_type_conv = "IPoA";

	showtext($j("#connectionType")[0], wanlink_type_conv);
}

function loadBalance_form(lb_unit){
	if(!parent.wans_flag)
		return 0;

	var pri_if = wans_dualwan.split(" ")[0];
	var sec_if = wans_dualwan.split(" ")[1];
	pri_if = add_lanport_number(pri_if);
	sec_if = add_lanport_number(sec_if);
	pri_if = pri_if.toUpperCase();
	sec_if = sec_if.toUpperCase();

	if(lb_unit == 0){
		if(pri_if== "DSL")
			document.getElementById("DSL_tab").style.display = "";
		else
			document.getElementById("DSL_tab").style.display = "none";
		have_lease = (first_wanlink_type() == "dhcp" || first_wanlink_type() == "dhcp");
		$("dualwan_row_primary").style.display = "";
		showtext($j("#dualwan_primary_if")[0], pri_if);
		$("dualwan_row_secondary").style.display = "none";
		update_connection_type(0);
		$('primary_WANIP_ctrl').style.display = "";
		$('primary_netmask_ctrl').style.display = "";
		$('secondary_WANIP_ctrl').style.display = "none";
		$('secondary_netmask_ctrl').style.display = "none";
		$('primary_DNS_ctrl').style.display = "";
		$('secondary_DNS_ctrl').style.display = "none";
		$('primary_gateway_ctrl').style.display = "";
		$('secondary_gateway_ctrl').style.display = "none";
		$('primary_lease_ctrl').style.display = (have_lease) ? "" : "none";
		$('primary_expires_ctrl').style.display = (have_lease) ? "" : "none";
		$('secondary_lease_ctrl').style.display = "none";
		$('secondary_expires_ctrl').style.display = "none";
	}
	else{
		if(sec_if== "DSL")
			document.getElementById("DSL_tab").style.display = "";
		else
			document.getElementById("DSL_tab").style.display = "none";
		have_lease = (secondary_wanlink_type() == "dhcp" || secondary_wanlink_xtype() == "dhcp");
		$("dualwan_row_primary").style.display = "none";
		$("dualwan_row_secondary").style.display = "";
		showtext($j("#dualwan_secondary_if")[0], sec_if);
		update_connection_type(1);
		$('primary_WANIP_ctrl').style.display = "none";
		$('primary_netmask_ctrl').style.display = "none";
		$('secondary_WANIP_ctrl').style.display = "";
		$('secondary_netmask_ctrl').style.display = "";
		$('primary_DNS_ctrl').style.display = "none";
		$('secondary_DNS_ctrl').style.display = "";
		$('primary_gateway_ctrl').style.display = "none";
		$('secondary_gateway_ctrl').style.display = "";
		$('primary_lease_ctrl').style.display = "none";
		$('primary_expires_ctrl').style.display = "none";
		$('secondary_lease_ctrl').style.display = (have_lease) ? "" : "none";
		$('secondary_expires_ctrl').style.display = (have_lease) ? "" : "none";
	}
}

function failover_form(fo_unit, primary_if, secondary_if){	
	if(fo_unit == 0){
		have_lease = (first_wanlink_type() == "dhcp" || first_wanlink_xtype() == "dhcp");
		showtext($j("#dualwan_current")[0], primary_if);
		if(primary_if == "DSL")
			document.getElementById("DSL_tab").style.display = "";
		else
			document.getElementById("DSL_tab").style.display = "none";
		update_connection_type(0);
		$('primary_WANIP_ctrl').style.display = "";
		$('primary_netmask_ctrl').style.display = "";
		$('secondary_WANIP_ctrl').style.display = "none";
		$('secondary_netmask_ctrl').style.display = "none";
		$('primary_DNS_ctrl').style.display = "";
		$('secondary_DNS_ctrl').style.display = "none";
		$('primary_gateway_ctrl').style.display = "";
		$('secondary_gateway_ctrl').style.display = "none";
		$('primary_lease_ctrl').style.display = (have_lease) ? "" : "none";
		$('primary_expires_ctrl').style.display = (have_lease) ? "" : "none";
		$('secondary_lease_ctrl').style.display = "none";
		$('secondary_expires_ctrl').style.display = "none";
	}
	else{
		have_lease = (secondary_wanlink_type() == "dhcp" || secondary_wanlink_xtype() == "dhcp");
		showtext($j("#dualwan_current")[0], secondary_if);
		if(secondary_if == "DSL")
			document.getElementById("DSL_tab").style.display = "";
		else
			document.getElementById("DSL_tab").style.display = "none";
		update_connection_type(1);
		$('primary_WANIP_ctrl').style.display = "none";
		$('primary_netmask_ctrl').style.display = "none";
		$('secondary_WANIP_ctrl').style.display = "";
		$('secondary_netmask_ctrl').style.display = "";
		$('primary_DNS_ctrl').style.display = "none";
		$('secondary_DNS_ctrl').style.display = "";
		$('primary_gateway_ctrl').style.display = "none";
		$('secondary_gateway_ctrl').style.display = "";
		$('primary_lease_ctrl').style.display = "none";
		$('primary_expires_ctrl').style.display = "none";
		$('secondary_lease_ctrl').style.display = (have_lease) ? "" : "none";
		$('secondary_expires_ctrl').style.display = (have_lease) ? "" : "none";
	}
}

function update_all_ip(wanip, wannetmask, wandns, wangateway, unit){
	var dnsArray = wandns.split(" ");
	if(unit == 0){
		showtext($j("#WANIP")[0], wanip);
		showtext($j("#netmask")[0], wannetmask);
		showtext2($j("#DNS1")[0], dnsArray[0], dnsArray[0]);
		showtext2($j("#DNS2")[0], dnsArray[1], dnsArray[1]);
		showtext($j("#gateway")[0], wangateway);
		if(parent.wans_flag){
			if (first_wanlink_type() == "dhcp") {
				showtext($j("#lease")[0], format_time(first_lease, "Renewing..."));
				showtext($j("#expires")[0], format_time(first_expires, "Expired"));
			}
		}
		else{		
			if (wanlink_type() == "dhcp") {
				showtext($j("#lease")[0], format_time(wan_lease, "Renewing..."));
				showtext($j("#expires")[0], format_time(wan_expires, "Expired"));
			}
		}
	}
	else{
		showtext($j("#secondary_WANIP")[0], wanip);
		showtext($j("#secondary_netmask")[0], wannetmask);
		showtext2($j("#secondary_DNS1")[0], dnsArray[0], dnsArray[0]);
		showtext2($j("#secondary_DNS2")[0], dnsArray[1], dnsArray[1]);
		showtext($j("#secondary_gateway")[0], wangateway);
		if (secondary_wanlink_type() == "dhcp") {
			showtext($j("#secondary_lease")[0], format_time(secondary_lease, "Renewing..."));
			showtext($j("#secondary_expires")[0], format_time(secondary_expires, "Expired"));
		}
	}
}
function update_all_xip(wanxip, wanxnetmask, wanxdns, wanxgateway, unit) {	
	var dnsArray = wandns.split(" ");
	var have_dns = !(dnsArray[0] || dnsArray[1]);
	var dnsArray = wanxdns.split(" ");
	var have_ip = false;
	var have_gateway = false;
	var have_lease = false;
	var lease = 0;
	var expires = 0;

	var type = "";

	if(parent.wans_flag){
		type = (unit == 0) ? first_wanlink_xtype() : secondary_wanlink_xtype();
	}
	else
		type = wanlink_xtype();
			
	if (type == "dhcp" || type == "static") {
		have_ip = true;
		have_gateway = !(wanxgateway == "" || wanxgateway == "0.0.0.0");
		if (type == "dhcp") {
			have_lease = true;
			if(parent.wans_flag)
				lease = (unit == 0) ? first_wanlink_xlease() : secondary_wanlink_xlease();
			else
				lease = wanlink_xlease();

			if(parent.wans_flag)
				expires = (unit == 0) ? first_wanlink_xexpires() : secondary_wanlink_xexpires();
			else
				expires = wanlink_xexpires();
		}
	}

	if (unit == 0) {
		showtext2($j("#xWANIP")[0], wanxip, have_ip);
		showtext2($j("#xnetmask")[0], wanxnetmask, have_ip);
		showtext2($j("#xDNS1")[0], dnsArray[0], have_dns && dnsArray[0]);
		showtext2($j("#xDNS2")[0], dnsArray[1], have_dns && dnsArray[1]);
		showtext2($j("#xgateway")[0], wanxgateway, have_gateway);
		showtext2($j("#xlease")[0], format_time(lease, "Renewing..."), have_lease);
		showtext2($j("#xexpires")[0], format_time(expires, "Expired"), have_lease);
	}
	else {
		showtext2($j("#secondary_xWANIP")[0], wanxip, have_ip);
		showtext2($j("#secondary_xnetmask")[0], wanxnetmask, have_ip);
		showtext2($j("#secondary_xDNS1")[0], dnsArray[0], have_dns && dnsArray[0]);
		showtext2($j("#secondary_xDNS2")[0], dnsArray[1], have_dns && dnsArray[1]);
		showtext2($j("#secondary_xgateway")[0], wanxgateway, have_gateway);
		showtext2($j("#secondary_xlease")[0], format_time(lease, "Renewing..."), have_lease);
		showtext2($j("#secondary_xexpires")[0], format_time(expires, "Expired"), have_lease);
	}
}

function update_wan_state(state, auxstate){
	if(state == "2" && auxstate == "0")
		link_internet = 1;
	else
		link_internet = 0;

	return link_internet;
}

function update_wanip(e) {
  $j.ajax({
    url: '/status.asp',
    dataType: 'script', 
	
    error: function(xhr) {
      setTimeout("update_wanip();", 3000);
    },
    success: function(response) {
		if(parent.wans_flag){
			first_wanip = first_wanlink_ipaddr();
			first_wannetmask = first_wanlink_netmask();
			first_wandns = first_wanlink_dns();
			first_wangateway = first_wanlink_gateway();
			first_wanxip = first_wanlink_xipaddr();
			first_wanxnetmask = first_wanlink_xnetmask();
			first_wanxdns = first_wanlink_xdns();
			first_wanxgateway = first_wanlink_xgateway();
			first_lease = first_wanlink_lease();
			firset_expires = first_wanlink_expires();

			secondary_wanip = secondary_wanlink_ipaddr();
			secondary_wannetmask = secondary_wanlink_netmask();
			secondary_wandns = secondary_wanlink_dns();
			secondary_wangateway = secondary_wanlink_gateway();
			secondary_wanxip = secondary_wanlink_xipaddr();
			secondary_wanxnetmask = secondary_wanlink_xnetmask();
			secondary_wanxdns = secondary_wanlink_xdns();
			secondary_wanxgateway = secondary_wanlink_xgateway();
			secondary_lease = secondary_wanlink_lease();
			secondary_expires = secondary_wanlink_expires();
		}
		else{
			wanip = wanlink_ipaddr();
			wannetmask = wanlink_netmask();
			wandns = wanlink_dns();
			wangateway = wanlink_gateway();
			wanxip = wanlink_xipaddr();
			wanxnetmask = wanlink_xnetmask();
			wanxdns = wanlink_xdns();
			wanxgateway = wanlink_xgateway();
			wan_lease = wanlink_lease();
			wan_expires = wanlink_expires();
		}

		if(old_link_internet == -1)
			old_link_internet = update_wan_state(wanstate, wanauxstate);

		if(update_wan_state(wanstate, wanauxstate) != old_link_internet){
			refreshpage();
		}
		else{
			if(parent.wans_flag){
				update_all_ip(first_wanip, first_wannetmask, first_wandns, first_wangateway, 0);
				update_all_xip(first_wanxip, first_wanxnetmask, first_wanxdns, first_wanxgateway, 0);
				update_all_ip(secondary_wanip, secondary_wannetmask, secondary_wandns, secondary_wangateway, 1);
				update_all_xip(secondary_wanxip, secondary_wanxnetmask, secondary_wanxdns, secondary_wanxgateway, 1);
			}
			else{				
				update_all_ip(wanip, wannetmask, wandns, wangateway, 0);
				update_all_xip(wanxip, wanxnetmask, wanxdns, wanxgateway, 0);
			}
			setTimeout("update_wanip();", 3000);
		}
    }
  });
}

function goQIS(){
	parent.location.href = '/QIS_wizard.htm';
}

function goToWAN(){	
	if(parent.wans_flag){
		if(wans_dualwan.split(" ")[wan_unit] == "usb")
			parent.location.href = '/cgi-bin/Advanced_Modem_Content.asp';
		else
			parent.location.href = '/cgi-bin/Advanced_DSL_Content.asp';
	}
	else {
		if(wan_common_transmode == "USB")
			parent.location.href = '/cgi-bin/Advanced_Modem_Content.asp';
		else
			parent.location.href = '/cgi-bin/Advanced_DSL_Content.asp';
	}
}

function goToDualWAN(){
	parent.location.href = '/cgi-bin/Advanced_WANPort_Content.asp';
}

function gotoSiteSurvey(){
	if(sw_mode == 2)
		parent.location.href = '/QIS_wizard.htm?flag=sitesurvey&band='+'<% nvram_get("wl_unit"); %>';
	else
		parent.location.href = '/QIS_wizard.htm?flag=sitesurvey_mb';
}

function manualSetup(){
	return 0;
}

function tabclickhandler(unit){
	if(unit == 0){
		return;
	}
	else{
		location.href = "DSL_dashboard.asp";
	}	
}
</script>
</head>

<body class="statusbody" onload="initial();">
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="internetForm" id="form" action="/cgi-bin/start_apply.asp">
<input type="hidden" name="current_page" value="index.asp">
<input type="hidden" name="next_page" value="index.asp">
<input type="hidden" name="flag" value="Internet">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="wans_dualwan" value="<% tcWebApi_Get("Dualwan_Entry","wans_dualwan","s") %>">
<input type="hidden" name="wan_active" value="<% tcWebApi_Get("Wan_PVC","Active","s") %>">
<table id="DSL_tab" border="0" cellpadding="0" cellspacing="0" style="display:none;">
<tr>
	<td>
	<table width="100px" border="0" align="left" style="margin-left:8px;" cellpadding="0" cellspacing="0">
	<td>
		<div id="t0" class="tabclick_NW" align="center" style="font-weight: bolder; margin-right:2px; width:90px;" onclick="tabclickhandler(0)">
			<span id="span0" style="cursor:pointer;">WAN</span>
		</div>
	</td>
	<td>
		<div id="t1" class="tab_NW" align="center" style="margin-right:2px; width:90px;" onclick="tabclickhandler(1)">
			<span id="span1" style="cursor:pointer;font-weight: bolder;">DSL</span>
		</div>
	</td>
	</table>
	</td>
</tr>
</table>
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="rt_table">

	<tr id="wan_enable_button">
		<td height="50" style="padding:10px 15px 0px 15px;">
			<p class="formfonttitle_nwm" style="float:left;width:98px;"><%tcWebApi_Get("String_Entry","menu5_3_1","s")%></p>
			<div class="left" style="width:94px; float:right;" id="radio_wan_enable"></div>
			<div class="clear"></div>
			<script type="text/javascript">
				$j('#radio_wan_enable').iphoneSwitch(wan_enable,
					function() {
						document.internetForm.wan_active.value = "Yes";
						document.internetForm.flag.value = "wan_enable";
						parent.showLoading();
						document.internetForm.submit();
						return true;
					},
					function() {
						document.internetForm.wan_active.value = "No";
						document.internetForm.flag.value = "wan_enable";
						parent.showLoading();
						document.internetForm.submit();
						return true;
					}
				);
			</script>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id="dualwan_enable_button">
		<td height="50" style="padding:10px 15px 0px 15px;">
			<p class="formfonttitle_nwm" style="float:left;width:98px;"><%tcWebApi_Get("String_Entry","dualwan_enable","s")%></p>
			<div class="left" style="width:94px; float:right;" id="radio_dualwan_enable"></div>
			<div class="clear"></div>
			<script type="text/javascript">
				$j('#radio_dualwan_enable').iphoneSwitch(parent.wans_flag, 
					function() {
						document.internetForm.wans_dualwan.value = wans_cap.split(" ")[0] + " " + wans_cap.split(" ")[1];
						document.internetForm.flag.value = "dualwan";
						parent.showLoading();
						document.internetForm.submit();
						return true;
					},
					function() {
						document.internetForm.wans_dualwan.value = wans_dualwan.split(" ")[0]+" none";
						document.internetForm.flag.value = "dualwan";
						parent.showLoading();
						document.internetForm.submit();
						return true;
					}
				);
			</script>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id=dualwan_row_main style="display:none">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm">WAN Port</p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="dualwan_current"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id=dualwan_row_primary style="display:none">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "wan_type", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="dualwan_primary_if"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id=dualwan_row_secondary style="display:none">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "wan_type", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="dualwan_secondary_if"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id="dualwan_mode_ctrl" style="display:none">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "dualwan_mode", "s")%></p>

			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="dualwan_mode"></p>
			<!--select style="*margin-top:-7px;" id="wansMode" class="input_option" onchange="">
				<option value="1">Load Balance</option>
				<option value="2">Fail Over</option>
			</select-->

			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr id="loadbalance_config_ctrl" style="display:none">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "dualwan_mode_lb_setting", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="loadbalance_config"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm" ><%tcWebApi_Get("String_Entry", "Connectiontype", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="connectionType"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id="primary_WANIP_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "WAN_IP", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="WANIP"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="xWANIP"></p>
			<span id="wan_status" style="display:none"></span>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr id="primary_netmask_ctrl">
		<td style="padding:5px 10px 5px 15px;">
				<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "IPC_x_ExternalSubnetMask_in", "s")%></p>
				<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="netmask"></p>
				<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="xnetmask"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="secondary_WANIP_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "WAN_IP", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_WANIP"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_xWANIP"></p>
			<span id="wan_status" style="display:none"></span>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="secondary_netmask_ctrl">
		<td style="padding:5px 10px 5px 15px;">
				<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "IPC_x_ExternalSubnetMask_in", "s")%></p>
				<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_netmask"></p>
				<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_xnetmask"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id="primary_DNS_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm">DNS</p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="DNS1"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="DNS2"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="xDNS1"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="xDNS2"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="secondary_DNS_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm">DNS</p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_DNS1"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_DNS2"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_xDNS1"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_xDNS2"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr id="primary_gateway_ctrl">
		<td style="padding:5px 10px 5px 15px;">
				<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "RC_GWStaticGW_in", "s")%></p>
				<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="gateway"></p>
				<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="xgateway"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="secondary_gateway_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "RC_GWStaticGW_in", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_gateway"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_xgateway"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="primary_lease_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "LHC_LeaseTime_in", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="lease"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="xlease"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="primary_expires_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "LeaseExpires", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="expires"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="xexpires"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="secondary_lease_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "LHC_LeaseTime_in", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_lease"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_xlease"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr style="display:none;" id="secondary_expires_ctrl">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "LeaseExpires", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_expires"></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="secondary_xexpires"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr id="goDualWANSetting">
		<td height="50" style="padding:10px 15px 0px 15px;">
			<p class="formfonttitle_nwm" style="float:left;width:116px;">Dual WAN setting</p>
			<input type="button" class="button_gen_long" onclick="goToDualWAN();" value="<%tcWebApi_Get("String_Entry", "btn_go", "s")%>" style="position:absolute;right:25px;margin-top:-10px;margin-left:115px;">
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr id="goSetting" style="display:none">
		<td height="30" style="padding:10px 15px 0px 15px;">
			<p class="formfonttitle_nwm" style="float:left;width:116px;"><%tcWebApi_Get("String_Entry", "btn_to_WAN", "s")%></p>
			<input type="button" class="button_gen_long" onclick="goToWAN();" value="<%tcWebApi_Get("String_Entry", "btn_go", "s")%>" style="position:absolute;right:25px;margin-top:-10px;margin-left:115px;">
		</td>
	</tr>

	<!--tr>
		<td height="50" style="padding:10px 15px 0px 15px;">
			<p class="formfonttitle_nwm" style="float:left;width:98px; "><%tcWebApi_Get("String_Entry", "QIS", "s")%></p>
			<input type="button" class="button_gen" value="<%tcWebApi_Get("String_Entry", "btn_go", "s")%>" onclick="javascript:goQIS();">
		</td>
	</tr-->
</table>

<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="ap_table" style="display:none">
	<tr>
		<td style="padding:5px 10px 5px 15px;" id="RemoteAPtd">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "statusTitle_AP", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="RemoteAP"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "Connectiontype", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="LanProto"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "LAN_IP", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;"><% nvram_get("lan_ipaddr"); %></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "IPC_x_ExternalSubnetMask_in", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;"><% nvram_get("lan_netmask"); %></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "IPC_x_ExternalGateway_in", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;"><% nvram_get("lan_gateway"); %></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_Get("String_Entry", "HSDPAC_DNSServers_in", "s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;"><% nvram_get("lan_dns"); %></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>

</table>

</form>
</body>
</html>
