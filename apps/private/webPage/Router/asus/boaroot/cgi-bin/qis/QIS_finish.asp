<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_finish.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<style>
span{
border:0px solid #FFFFFF;
color:#FFFFFF;
font-size:12px;
font-family:Arial, Helvetica, sans-serif;
/*width:27px;*/
text-align:right;
margin:0px auto;
ime-mode:disabled;
}
</style>
<script type="text/Javascript" src="/help.js"></script>
<script type="text/Javascript" src="/jquery.js"></script>
<script type="text/javascript">
var auth_mode2 = '<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_wl0_auth_mode_x","s"); %>';
var auth_mode = '<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_wl1_auth_mode_x","s"); %>';
var reIP = '<%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%>';
var band5g_support = "<% tcWebApi_get("SysInfo_Entry", "rc_support", "s") %>".search("5G");
var transfer_mode = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","s") %>";
var dsltmp_dsl_restart = "<%tcWebApi_get("GUITemp_Entry0","dsltmp_dsl_restart","s")%>";
var dsltmp_dsl_early_restart = "<%tcWebApi_get("GUITemp_Entry0","dsltmp_dsl_early_restart","s")%>";
var with_wan_setting = "<% tcWebApi_get("GUITemp_Entry0","with_wan_setting","s") %>";
var model_name = "<%tcWebApi_get("String_Entry","Web_Title2","s")%>";

var iptv_atm_pvc_str_title = "";
var iptv_atm_pvc_str = "";
var iptv_ptm_pvc_str_title = "";
var iptv_ptm_pvc_str = "";
var dsltmp_cfg_iptv_pvclist = decodeURIComponent('<%tcWebApi_char_to_ascii("GUITemp_Entry0","dsltmp_cfg_iptv_pvclist", "s")%>');
var iptv_num_pvc_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","s") %>";


//udpate iptv information
if (iptv_num_pvc_val != "0" && iptv_num_pvc_val != "") {
	var dsltmp_cfg_iptv_pvclist_row;
	var dsltmp_cfg_iptv_pvclist_col;
	var i, j;

	iptv_atm_pvc_str_title = iptv_num_pvc_val + " IPTV PVC(s)";
	iptv_ptm_pvc_str_title = iptv_num_pvc_val + " IPTV service(s)";

	//<vpi>vci>proto>encap>vlanid
	if(dsltmp_cfg_iptv_pvclist.charAt(0) == "<")	//rempve the 1st "<"
		dsltmp_cfg_iptv_pvclist = dsltmp_cfg_iptv_pvclist.substr(1,dsltmp_cfg_iptv_pvclist.length);
	dsltmp_cfg_iptv_pvclist_row = dsltmp_cfg_iptv_pvclist.split('<');

	for(i=0; i<dsltmp_cfg_iptv_pvclist_row.length; i++) {
		if(i > 0) {
			iptv_atm_pvc_str += " + ";
			iptv_ptm_pvc_str += " + ";
		}
		dsltmp_cfg_iptv_pvclist_col = dsltmp_cfg_iptv_pvclist_row[i].split('>');
		for(j=0; j<dsltmp_cfg_iptv_pvclist_col.length; j++){
			switch(j) {
				case 0:	//vpi
					iptv_atm_pvc_str += dsltmp_cfg_iptv_pvclist_col[j] + "/";
					break;
				case 1:	//vci
					iptv_atm_pvc_str += dsltmp_cfg_iptv_pvclist_col[j] + ",";
					break;
				case 2:	//proto
					if(dsltmp_cfg_iptv_pvclist_col[j] == 3) {
						iptv_atm_pvc_str += "Bridged, ";
						iptv_ptm_pvc_str += "Bridged, ";
					}
					else {
						iptv_atm_pvc_str += ", ";
						iptv_ptm_pvc_str += ", ";
					}
					break;
				case 3:	//encap
					if(dsltmp_cfg_iptv_pvclist_col[j] == 1)
						iptv_atm_pvc_str += "VC-Mux";
					else
						iptv_atm_pvc_str += "LLC";
					break;
				case 4:	//vlan id
					if(dsltmp_cfg_iptv_pvclist_col[j] != "") {
						iptv_atm_pvc_str += "VLAN ID " + dsltmp_cfg_iptv_pvclist_col[j];
						iptv_ptm_pvc_str += "VLAN ID " + dsltmp_cfg_iptv_pvclist_col[j];
					}
					break;
			}
		}
	}
}

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	parent.set_step("t3");
	parent.$("LoadingBar").style.visibility = 'hidden';
	parent.document.getElementById("btn_gotoindex").style.visibility = "hidden";

	$("#ssid_2_item")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "QIS_finish_wireless_item1", "s") %>";
	$("#ssid_2")[0].innerHTML = handle_show_str("<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_wl0_ssid","s"); %>");
	$("#ssid_5_item")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "QIS_finish_wireless_item1", "s") %>";
	$("#ssid_5")[0].innerHTML = handle_show_str("<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_wl1_ssid","s"); %>");
	$("#security_item2")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "QIS_finish_wireless_item2", "s") %>";
	$("#security_item5")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "QIS_finish_wireless_item2", "s") %>";
	if(auth_mode2 == "OPEN"){
		$("#security2")[0].innerHTML = "Open System";
		$("#key2_block")[0].style.display = "none";
	}
	else if(auth_mode2 == "WPA2PSK"){
		$("#security2")[0].innerHTML = "WPA2–Personal - AES";
		$("#key2_item")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "Network_key", "s") %>";
		$("#key2")[0].innerHTML = handle_show_str("<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_wl0_wpa_psk","s"); %>");
	}
	else{
		$("#freqtitle2")[0].style.display = "none";
		$("#ssid2_block")[0].style.display = "none";
		$("#key2_block")[0].style.display = "none";
		$("#security2_block")[0].style.display = "none";
	}

	if(band5g_support == -1){
		$("#desc2g")[0].style.display = "none";
		$("#freqtitle5")[0].style.display = "none";
		$("#ssid5_block")[0].style.display = "none";
		$("#key5_block")[0].style.display = "none";
		$("#security5_block")[0].style.display = "none";
	}
	else{
		if(auth_mode == "OPEN"){
			$("#security5")[0].innerHTML = "Open System";
			$("#key5_block")[0].style.display = "none";
		}
		else if(auth_mode == "WPA2PSK"){
			$("#security5")[0].innerHTML = "WPA2–Personal - AES";
			$("#key5_item")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "Network_key", "s") %>";
			$("#key5")[0].innerHTML = handle_show_str("<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_wl1_wpa_psk","s"); %>");
		}
		else{
			$("#freqtitle5")[0].style.display = "none";
			$("#ssid5_block")[0].style.display = "none";
			$("#key5_block")[0].style.display = "none";
			$("#security5_block")[0].style.display = "none";
		}
	}

	$("#mac_item")[0].innerHTML = "MAC";
	$("#mac")[0].innerHTML = '<%If tcWebApi_get("Info_Ether","mac","h") <> "" Then tcWebApi_staticGet("Info_Ether","mac","s") end if%>';
	$("#wantype_item")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>";

	var pvc_type = "<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_prctl","s") %>";
	var wanlink_type_conv = "";
	if (pvc_type == "0")
		wanlink_type_conv = "PPPoE";
	else if (pvc_type == "1")
		wanlink_type_conv = "PPPoA";
	else if (pvc_type == "2")
	{
		if(transfer_mode == "ATM")
			wanlink_type_conv = "MER";
		else
			wanlink_type_conv = "Automatic IP";
	}
	else if (pvc_type == "3")
		wanlink_type_conv = "BRIDGE";
	else if (pvc_type == "4")
	{
		if(transfer_mode == "ATM")
			wanlink_type_conv = "IPoA";
		else
			wanlink_type_conv = "Static IP";
	}

	if(with_wan_setting == "")
		$("#wantype")[0].innerHTML = "";
	else
		$("#wantype")[0].innerHTML = wanlink_type_conv;

	if(transfer_mode == "ATM")
	{
		$("#wanvpivci_item")[0].innerHTML = "VPI / VCI";

		if(with_wan_setting == "")
			$("#wanvpivci")[0].innerHTML = "";
		else
			$("#wanvpivci")[0].innerHTML = "<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_vpi","s") %> / <% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_vci","s") %>";

		$("#wanencap_item")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "prtcl_JS_encmode", "s") %>";		
		
		var encap_val = "<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_encap","s") %>";
		if (encap_val == "1")
			$("#wanencap")[0].innerHTML = "VC-Mux";
		else
			$("#wanencap")[0].innerHTML = "LLC";

		if(with_wan_setting == "")
			$("#wanencap")[0].innerHTML = "";
						
		if(iptv_num_pvc_val >0){	//iptv information
			document.getElementById("iptv_service").style.display = "";
			document.getElementById("iptv_pvc_item").innerHTML = iptv_atm_pvc_str_title;
			document.getElementById("iptv_pvc").innerHTML = iptv_atm_pvc_str;
		}
	}
	else //PTM
	{
		if("<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_vlanid","s") %>" != "")
		{
			$("#wanvpivci_item")[0].innerHTML = "<%tcWebApi_get("String_Entry","WANVLANIDText","s")%>";

			if(with_wan_setting == "")
				$("#wanvpivci")[0].innerHTML = "";
			else
				$("#wanvpivci")[0].innerHTML = "<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_vlanid","s") %>";
		}
				
		if(iptv_num_pvc_val >0){	//iptv information
			document.getElementById("iptv_service").style.display = "";
			document.getElementById("iptv_pvc_item").innerHTML = iptv_ptm_pvc_str_title;
			document.getElementById("iptv_pvc").innerHTML = iptv_ptm_pvc_str;
		}
	}

	$("#lanip_item")[0].innerHTML = "<% tcWebApi_Get("String_Entry", "LAN_IP", "s") %>";
	$("#lanip")[0].innerHTML = reIP;
}

function redirect(){
	parent.document.location.href =  "/cgi-bin/index2.asp";
}

function goHome(){
	var wait_time = 40;

	if(document.redirectForm.dsltmp_cfg_iptv_enable.value == "1")
		wait_time += 15;
	if(model_name == "DSL-N66U")
		wait_time += 5;

	if(dsltmp_dsl_restart == "1")
	{
		wait_time += 60;
	}
	else if(dsltmp_dsl_early_restart == "1")
	{
		wait_time += 30;
	}

	parent.showLoading(wait_time);
	setTimeout("redirect();", wait_time*1000);
	document.redirectForm.submit();
}
</script>
</head>
<body onLoad="QKfinish_load_body();" >
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="redirectForm" action="QIS_process.asp" target="hidden_frame">
<input type="hidden" name="prev_page" value="/cgi-bin/qis/QIS_wireless.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_finish.asp">
<input type="hidden" name="next_page" value="none">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<!-- pwd -->
<input type="hidden" name="uiViewPassword" value="<% tcWebApi_staticGet("GUITemp_Entry0","web_passwd","s") %>">
<input type="hidden" name="uiViewUsername" value="<% tcWebApi_staticGet("GUITemp_Entry0","username","s") %>">
<input type="hidden" name="uiViewResetValue" value="">
<!-- wifi -->
<input type="hidden" name="wl0_ssid" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl0_ssid","s") %>">
<input type="hidden" name="wl0_wpa_psk" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl0_wpa_psk","s") %>">
<input type="hidden" name="wl0_auth_mode_x" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl0_auth_mode_x","s") %>">
<input type="hidden" name="wl1_ssid" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl1_ssid","s") %>">
<input type="hidden" name="wl1_wpa_psk" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl1_wpa_psk","s") %>">
<input type="hidden" name="wl1_auth_mode_x" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl1_auth_mode_x","s") %>">
<input type="hidden" name="w_Setting" value="1">

<script>
if("<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl0_auth_mode_x","s") %>" == "WPA2PSK")
	document.write('<INPUT TYPE="hidden" NAME="wl0_crypto" value="AES">');
else
	document.write('<INPUT TYPE="hidden" NAME="wl0_crypto" value="NONE">');

if("<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_wl1_auth_mode_x","s") %>" == "WPA2PSK")
	document.write('<INPUT TYPE="hidden" NAME="wl1_crypto" value="AES">');
else
	document.write('<INPUT TYPE="hidden" NAME="wl1_crypto" value="NONE">');
</script>
<!-- wan -->
<INPUT TYPE="hidden" NAME="wanVCFlag" value="3">
<input type="hidden" name="dsltmp_wanTypeOption" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_wanTypeOption","s") %>">
<input type="hidden" name="dsltmp_transfer_mode" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_transfer_mode","s") %>">
<input type="hidden" name="dsltmp_cfg_vlanid" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_vlanid","s") %>">
<input type="hidden" name="dsltmp_cfg_vpi" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_vpi","s") %>">
<input type="hidden" name="dsltmp_cfg_vci" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_vci","s") %>">
<input type="hidden" name="dsltmp_cfg_prctl" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_prctl","s") %>">
<input type="hidden" name="dsltmp_cfg_encap" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_encap","s") %>">

<input type="hidden" name="dsltmp_cfg_pppoe_username" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_pppoe_username","s") %>">
<input type="hidden" name="dsltmp_cfg_pppoe_passwd" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_pppoe_passwd","s") %>">

<input type="hidden" name="dsltmp_cfg_DHCPClient" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_DHCPClient","s") %>">
<input type="hidden" name="dsltmp_cfg_dnsenable" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_dnsenable","s") %>">
<input type="hidden" name="dsltmp_cfg_ipaddr" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_ipaddr","s") %>">
<input type="hidden" name="dsltmp_cfg_netmask" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_netmask","s") %>">
<input type="hidden" name="dsltmp_cfg_gateway" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_gateway","s") %>">
<input type="hidden" name="dsltmp_cfg_dns1" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_dns1","s") %>">
<input type="hidden" name="dsltmp_cfg_dns2" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_dns2","s") %>">
<input type="hidden" name="dsltmp_dhcp_clientid" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_dhcp_clientid","s") %>">

<input type="hidden" name="dsltmp_cfg_iptv_rmvlan" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_iptv_rmvlan","s") %>">
<input type="hidden" name="dsltmp_cfg_iptv_mr" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_iptv_mr","s") %>">
<input type="hidden" name="dsltmp_cfg_iptv_num_pvc" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","s") %>">
<input type="hidden" name="dsltmp_cfg_iptv_pvclist" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_iptv_pvclist","s") %>">
<input type="hidden" name="dsltmp_cfg_iptv_enable" value="<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_iptv_enable","s") %>">
<!-- dsl -->
<input type="hidden" name="dsltmp_cfg_ginp" value="<%tcWebApi_Get("GUITemp_Entry0","dsltmp_cfg_ginp","s")%>">

<!-- default value -->
<INPUT TYPE="hidden" NAME="value_empty" value="">
<INPUT TYPE="hidden" NAME="value_yes" value="Yes">
<INPUT TYPE="hidden" NAME="value_no" value="No">
<INPUT TYPE="hidden" NAME="value_one" value="1">
<INPUT TYPE="hidden" NAME="value_zero" value="0">
<INPUT TYPE="hidden" NAME="value_30" value="30">
<INPUT TYPE="hidden" NAME="value_none" value="None">
<INPUT TYPE="hidden" NAME="value_ubr" value="ubr">
<INPUT TYPE="hidden" NAME="ipVerRadio" value="IPv4">
<INPUT TYPE="hidden" NAME="wan_RIP" value="RIP1">
<INPUT TYPE="hidden" NAME="wan_status" value="Disabled">
<INPUT TYPE="hidden" NAME="wan_NAT" value="Enable">
<INPUT TYPE="hidden" NAME="PPPAuthen" value="AUTO">
<INPUT TYPE="hidden" NAME="wan_ConnectSelect" value="Connect_Keep_Alive">
<INPUT TYPE="hidden" NAME="wan_ConnectSelect_POL" value="Connect_on_Demand">
<INPUT TYPE="hidden" NAME="wan_PPPGetIP" value="Dynamic">

<script>
	if(transfer_mode == "ATM")
		document.write('<INPUT TYPE="hidden" NAME="wan_pvc" value="0">');
	else
		document.write('<INPUT TYPE="hidden" NAME="wan_pvc" value="8">');
</script>

<INPUT TYPE="hidden" NAME="Bridged_IP_VC" value="1483 Bridged IP VC-Mux">
<INPUT TYPE="hidden" NAME="Bridged_IP_LLC" value="1483 Bridged IP LLC">
<INPUT TYPE="hidden" NAME="Routed_IP_VC" value="1483 Routed IP VC-Mux">
<INPUT TYPE="hidden" NAME="Routed_IP_LLC" value="1483 Routed IP LLC(IPoA)">
<INPUT TYPE="hidden" NAME="PPPoE_VC" value="PPPoE VC-Mux">
<INPUT TYPE="hidden" NAME="PPPoE_LLC" value="PPPoE LLC">
<INPUT TYPE="hidden" NAME="PPPoA_VC" value="PPPoA VC-Mux">
<INPUT TYPE="hidden" NAME="PPPoA_LLC" value="PPPoA LLC">
<INPUT TYPE="hidden" NAME="Bridged_Only_VC" value="1483 Bridged Only VC-Mux">
<INPUT TYPE="hidden" NAME="Bridged_Only_LLC" value="1483 Bridged Only LLC">
<input type="hidden" name="QIS_Flag" value="1">
<input type="hidden" name="detected_lang_type" value="<% tcWebApi_staticGet("WebCurSet_Entry","detected_lang_type","s") %>">
</form>
<div class="QISmain">
<div>
<table width="730px">
	<tr>
		<td align="left">
			<span class="description_down"><% tcWebApi_Get("String_Entry", "qis_finish_title1", "s") %></script></span>
		</td>
	</tr>
</table>
</div>
<div style="margin:5px;"><img style="width: 720px; *width: 710px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
<br/>
<p class="QISGeneralFont" style="margin-top:-5px;"><% tcWebApi_Get("String_Entry", "QIS_finish_Desc", "s") %></p>
<br/>
<table id="tblsetting_1" class="QISform_finish" width="400" border=0 align="center" cellpadding="5" cellspacing="0" style="margin-top:-10px;">
	<tr id="freqtitle2">
		<td align="left" colspan="4" style="color:#5AD;font-size:16px; border-bottom:1px dashed #AAA;"><% tcWebApi_Get("String_Entry", "menu5_1", "s") %> <span style="color:#5AD;" id="desc2g">(2.4GHz)</span></td>
	</tr>
	<tr id="ssid2_block">
		<th width="180"><span id="ssid_2_item"></span></th>
		<td class="QISformtd" width="300px">
			<span id="ssid_2" style="white-space: pre;"></span>
		</td>
	</tr>
	<tr id="key2_block">
		<th width="180"><span id="key2_item"></span></th>
		<td class="QISformtd">
			<span id="key2" style="white-space: pre;"></span>
		</td>
	</tr>
	<tr id="security2_block">
		<th width="180"><span id="security_item2"></span></th>
		<td class="QISformtd">
			<span id="security2"></span>
		</td>
	</tr>
	<tr id="freqtitle5">
		<td align="left" colspan="4" style="color:#5AD;font-size:16px; border-bottom:1px dashed #AAA;"><% tcWebApi_Get("String_Entry", "menu5_1", "s") %> <span style="color:#5AD;">(5GHz)</span></td>
	</tr>
	<tr id="ssid5_block">
		<th width="180"><span id="ssid_5_item"></span></th>
		<td class="QISformtd">
			<span id="ssid_5" style="white-space: pre;"></span>
		</td>
	</tr>
	<tr id="key5_block">
		<th width="180"><span id="key5_item"></span></th>
		<td class="QISformtd">
			<span id="key5" style="white-space: pre;"></span>
		</td>
	</tr>
	<tr id="security5_block">
		<th width="180"><span id="security_item5"></span></th>
		<td class="QISformtd">
			<span id="security5"></span>
		</td>
	</tr>
	<tr>
		<td align="left" colspan="4" style="color:#5AD;font-size:16px; border-bottom:1px dashed #AAA;">WAN <span style="color:#5AD;">(<% tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","s"); %>)</span>
		</td>
	</tr>
	<tr>
		<th width="180"><span id="wantype_item"></span></th>
		<td class="QISformtd">
			<span id="wantype"></span>
		</td>
	</tr>
	<tr>
		<th width="180"><span id="wanvpivci_item"></span></th>
		<td class="QISformtd">
			<span id="wanvpivci"></span>
		</td>
	</tr>
	<tr>
		<th width="180"><span id="wanencap_item"></span></th>
		<td class="QISformtd">
			<span id="wanencap"></span>
		</td>
	</tr>
	<tr id="iptv_service" style="display:none;">
		<th width="180"><span id="iptv_pvc_item"></span></th>
		<td class="QISformtd">
			<span id="iptv_pvc"></span>
		</td>
	</tr>
	<tr>
		<td align="left" colspan="4" style="color:#5AD;font-size:16px; border-bottom:1px dashed #AAA;">LAN</td>
	</tr>
	<tr>
		<th width="180"><span id="lanip_item"></span></th>
		<td class="QISformtd">
			<span id="lanip"></span>
		</td>
	</tr>
	<tr>
		<th width="180"><span id="mac_item"></span></th>
		<td class="QISformtd">
			<span id="mac"></span>
		</td>
	</tr>
</table>
<div class="apply_gen" style="margin-top:5px">
	<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" onclick="goHome();" class="button_gen">
</div>
</div>
</body>

<!--qis/QIS_finish.asp-->
</html>
