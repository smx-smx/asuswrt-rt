<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_IPTV_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - IPTV</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>

var original_switch_stb_x = '<% tcWebApi_get("IPTV_Entry", "switch_stb_x", "s") %>';
var original_switch_wantag = 'none';
var wans_lanport = '<% tcWebApi_get("Dualwan_Entry", "wans_lanport", "s") %>';
var wans_dualwan_orig = '<% tcWebApi_get("Dualwan_Entry", "wans_dualwan", "s") %>';
var haveLanWan_support = '<%tcWebApi_get("WebCustom_Entry","haveLanWan","s")%>'
var haveWan0_support = '<%tcWebApi_get("WebCustom_Entry","haveWan0","s")%>';


function initial(){
	show_menu();
	if(dsl_support == -1) {
		ISP_Profile_Selection(original_switch_wantag);
	}
	document.form.switch_stb_x.value = original_switch_stb_x;

	disable_udpxy();

	if(Rawifi_support == -1) //rawifi platform without this item, by Viz 2012.01
		$('enable_eff_multicast_forward').style.display="";
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	if(document.form.rmvlan.value == "1")
		document.form.rmvlan_check.checked = true;
	else
		document.form.rmvlan_check.checked = false;
<%end if%>
}

function load_ISP_profile() {
	if(document.form.switch_wantag.value == "unifi_home") {
		document.form.switch_stb_x.value = "4";
		document.form.switch_wan0tagid.value = "500";
		document.form.switch_wan0prio.value = "0";
		document.form.switch_wan1tagid.value = "600";
		document.form.switch_wan1prio.value = "0";
		document.form.switch_wan2tagid.value = "0";
		document.form.switch_wan2prio.value = "0";
	}
	else if(document.form.switch_wantag.value == "unifi_biz") {
		document.form.switch_stb_x.value = "0";
		document.form.switch_wan0tagid.value = "500";
		document.form.switch_wan0prio.value = "0";
		document.form.switch_wan1tagid.value = "0";
		document.form.switch_wan1prio.value = "0";
		document.form.switch_wan2tagid.value = "0";
		document.form.switch_wan2prio.value = "0";
	}
	else if(document.form.switch_wantag.value == "singtel_mio") {
		document.form.switch_stb_x.value = "6";
		document.form.switch_wan0tagid.value = "10";
		document.form.switch_wan0prio.value = "0";
		document.form.switch_wan1tagid.value = "20";
		document.form.switch_wan1prio.value = "4";
		document.form.switch_wan2tagid.value = "30";
		document.form.switch_wan2prio.value = "4";
	}
	else if(document.form.switch_wantag.value == "singtel_others") {
		document.form.switch_stb_x.value = "4";
		document.form.switch_wan0tagid.value = "10";
		document.form.switch_wan0prio.value = "0";
		document.form.switch_wan1tagid.value = "20";
		document.form.switch_wan1prio.value = "4";
		document.form.switch_wan2tagid.value = "0";
		document.form.switch_wan2prio.value = "0";
	}
	else if(document.form.switch_wantag.value == "m1_fiber") {
		document.form.switch_stb_x.value = "3";
		document.form.switch_wan0tagid.value = "1103";
		document.form.switch_wan0prio.value = "1";
		document.form.switch_wan1tagid.value = "0";
		document.form.switch_wan1prio.value = "0";
		document.form.switch_wan2tagid.value = "1107";
		document.form.switch_wan2prio.value = "1";
	}else{}
}

function ISP_Profile_Selection(isp){
	if(isp == "none"){
		$("wan_stb_x").style.display = "";
		$("wan_iptv_x").style.display = "none";
		$("wan_voip_x").style.display = "none";
		$("wan_internet_x").style.display = "none";
		$("wan_iptv_port4_x").style.display = "none";
		$("wan_voip_port3_x").style.display = "none";
		document.form.switch_wantag.value = "none";
		document.form.switch_stb_x.value = "0";
	}
	else if(isp == "unifi_home"){
		$("wan_stb_x").style.display = "none";
		$("wan_iptv_x").style.display = "";
		$("wan_voip_x").style.display = "none";
		$("wan_internet_x").style.display = "none";
		$("wan_iptv_port4_x").style.display = "none";
		$("wan_voip_port3_x").style.display = "none";
		document.form.switch_wantag.value = "unifi_home";
		document.form.switch_stb_x.value = "4";
	}
	else if(isp == "unifi_biz"){
		$("wan_stb_x").style.display = "none";
		$("wan_iptv_x").style.display = "none";
		$("wan_voip_x").style.display = "none";
		$("wan_internet_x").style.display = "none";
		$("wan_iptv_port4_x").style.display = "none";
		$("wan_voip_port3_x").style.display = "none";
		document.form.switch_wantag.value = "unifi_biz";
		document.form.switch_stb_x.value = "0";
	}
	else if(isp == "singtel_mio"){
		$("wan_stb_x").style.display = "none";
		$("wan_iptv_x").style.display = "";
		$("wan_voip_x").style.display = "";
		$("wan_internet_x").style.display = "none";
		$("wan_iptv_port4_x").style.display = "none";
		$("wan_voip_port3_x").style.display = "none";
		document.form.switch_wantag.value = "singtel_mio";
		document.form.switch_stb_x.value = "6";
	}
	else if(isp == "singtel_others"){
		$("wan_stb_x").style.display = "none";
		$("wan_iptv_x").style.display = "";
		$("wan_voip_x").style.display = "none";
		$("wan_internet_x").style.display = "none";
		$("wan_iptv_port4_x").style.display = "none";
		$("wan_voip_port3_x").style.display = "none";
		document.form.switch_wantag.value = "singtel_others";
		document.form.switch_stb_x.value = "4";
	}
	else if(isp == "m1_fiber"){
		$("wan_stb_x").style.display = "none";
		$("wan_iptv_x").style.display = "none";
		$("wan_voip_x").style.display = "";
		$("wan_internet_x").style.display = "none";
		$("wan_iptv_port4_x").style.display = "none";
		$("wan_voip_port3_x").style.display = "none";
		document.form.switch_wantag.value = "m1_fiber";
		document.form.switch_stb_x.value = "3";
	}
	else if(isp == "manual"){
		$("wan_stb_x").style.display = "none";
		$("wan_iptv_x").style.display = "";
		$("wan_voip_x").style.display = "";
		$("wan_internet_x").style.display = "";
		$("wan_iptv_port4_x").style.display = "";
		$("wan_voip_port3_x").style.display = "";
		document.form.switch_wantag.value = "manual";
		document.form.switch_stb_x.value = "6";
	}
}

function validForm(){
	if (dsl_support == -1) {
		if(document.form.switch_wantag.value == "manual")
		{
			if(document.form.switch_wan0tagid.value.length > 0)
			{
				if(!validate_range(document.form.switch_wan0tagid, 2, 4094))
					return false;
			}
			if(document.form.switch_wan1tagid.value.length > 0)
			{
				if(!validate_range(document.form.switch_wan1tagid, 2, 4094))
					return false;
			}
			if(document.form.switch_wan2tagid.value.length > 0)
			{
				if(!validate_range(document.form.switch_wan2tagid, 2, 4094))
					return false;
			}
			if(document.form.switch_wan0prio.value.length > 0 && !validate_range(document.form.switch_wan0prio, 0, 7))
				return false;
			if(document.form.switch_wan1prio.value.length > 0 && !validate_range(document.form.switch_wan1prio, 0, 7))
				return false;
			if(document.form.switch_wan2prio.value.length > 0 && !validate_range(document.form.switch_wan2prio, 0, 7))
				return false;
		}
	}
	return true;
}

function applyRule(){
	if(haveLanWan_support == "Yes"){
	if (dualWAN_support != -1) {
		var port_conflict = false;
		var iptv_port = document.form.switch_stb_x.value;
		if (wans_lanport == "1") {
			if (iptv_port == "1" || iptv_port == "5") {
				port_conflict = true;
			}
		}
		else if (wans_lanport == "2") {
			if (iptv_port == "2" || iptv_port == "5") {
				port_conflict = true;
			}
		}
		else if (wans_lanport == "3") {
			if (iptv_port == "3" || iptv_port == "6") {
				port_conflict = true;
			}
		}
		else if (wans_lanport == "4") {
			if (iptv_port == "4" || iptv_port == "6") {
				port_conflict = true;
			}
		}
		if (port_conflict) {
			alert("<%tcWebApi_get("String_Entry","RC_IPTV_conflict","s")%>");
			document.form.switch_stb_x.focus();
			return;
		}
	}
	}

	if(haveWan0_support == "Yes"){
		var port_conflict = false;
		var iptv_port = document.form.switch_stb_x.value;
		if (iptv_port == "7" && wans_dualwan_orig.search("wan") >= 0)
			port_conflict = true;
		
		if (port_conflict) {
			alert("<%tcWebApi_get("String_Entry","RC_IPTV_conflict_WAN","s")%>");
			document.form.switch_stb_x.focus();
			return;
		}
	}

	if(dsl_support == -1) {
		if( (original_switch_stb_x != document.form.switch_stb_x.value)
		|| (original_switch_wantag != document.form.switch_wantag.value)){
			FormActions("start_apply.asp", "apply", "reboot", "30");
		}
		load_ISP_profile();
	}
	if(original_switch_stb_x != document.form.switch_stb_x.value)
		document.form.modified.value = "1";
	if(validForm()){
		if(document.form.udpxy_enable_x.value != 0 && document.form.udpxy_enable_x.value != ""){
			if(!validate_range(document.form.udpxy_enable_x, 1024, 65535)){
				document.form.udpxy_enable_x.focus();
				document.form.udpxy_enable_x.select();
				return false;
			}else{
				showLoading();
				document.form.submit();
			}
		}else{
			showLoading();
			document.form.submit();
		}
	}
}

function valid_udpxy(){
	if(document.form.udpxy_enable_x.value != 0 && document.form.udpxy_enable_x.value != ""){
		if(!validate_range(document.form.udpxy_enable_x, 1024, 65535)){
			document.form.udpxy_enable_x.focus();
			document.form.udpxy_enable_x.select();
			return false;
		}else
			return true;
	}
}

function disable_udpxy(){
	if(document.form.mr_enable_x.value == 1){
		return change_common_radio(document.form.mr_enable_x, 'RouterConfig', 'mr_enable_x', '1');
	}
	else{
		return change_common_radio(document.form.mr_enable_x, 'RouterConfig', 'mr_enable_x', '0');
	}
}// The input fieldof UDP proxy does not relate to Mutlicast Routing.

function add_options_value(o, str, arr, orig){
	if(orig == arr)
		add_option(o, str, arr, 1);
	else
		add_option(o, str, arr, 0);
}
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
function change_rmvlan(){
	if(document.form.rmvlan_check.checked == true)
		document.form.rmvlan.value = 1;
	else
		document.form.rmvlan.value = 0;
}
<%end if%>
</script>
</head>

<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="hiddenMask" class="popup_bg">
	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
	<tr>
	<td>
		<div class="drword" id="drword" style="height:110px;"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
		<br/>
		<br/>
		</div>
		<div class="drImg"><img src="/images/alertImg.png"></div>
		<div style="height:70px;"></div>
	</td>
	</tr>
	</table>
</div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_IPTV_Content.asp">
<input type="hidden" name="next_page" value="Advanced_IPTV_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_net">
<input type="hidden" name="action_wait" value="10">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<input type="hidden" name="rmvlan" value='<% tcWebApi_get("IPTV_Entry","rmvlan","s"); %>'>
<%end if%>
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
	<td width="17">&nbsp;</td>
	<td valign="top" width="202">
		<div id="mainMenu"></div>
		<div id="subMenu"></div>
	</td>
	<td valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
		<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
			<tr>
			<td align="left" valign="top">
				<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
				<tbody>
					<tr>
					<td bgcolor="#4D595D" valign="top" >
						<div>&nbsp;</div>
						<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_2","s")%> - IPTV</div>
						<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
						<div class="formfontdesc"><%tcWebApi_get("String_Entry","LHC_displayIPTV_sd","s")%></div>
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
							<thead>
								<tr>
								<td colspan="2">Port</td>
								</tr>
							</thead>
							<tr id="wan_stb_x">
								<th width="30%">Choose IPTV STB Port</th>
								<td align="left">
									<select name="switch_stb_x" class="input_option">
									<option value="0" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "0" then asp_Write("selected") end if%>><%tcWebApi_get("String_Entry","checkbox_No","s")%></option>
									<option value="1" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "1" then asp_Write("selected") end if%>>LAN1</option>
									<option value="2" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "2" then asp_Write("selected") end if%>>LAN2</option>
									<option value="3" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "3" then asp_Write("selected") end if%>>LAN3</option>
									<option value="4" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "4" then asp_Write("selected") end if%>>LAN4</option>
									<option value="5" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "5" then asp_Write("selected") end if%>>LAN1 & LAN2</option>
									<option value="6" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "6" then asp_Write("selected") end if%>>LAN3 & LAN4</option>
									<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
									<option value="7" <%if tcWebApi_get("IPTV_Entry","switch_stb_x","h") = "7" then asp_Write("selected") end if%>>Ethernet WAN</option>
									<%end if%>
									</select>
									<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
									<input type="checkbox" name="rmvlan_check" id="rmvlan_check" value="" onClick="change_rmvlan()"> Remove VLAN TAG from WAN</input>
									<%end if%>
								</td>
							</tr>
						</table>
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:10px;">
							<thead>
								<tr>
								<td colspan="2">Special Application</td>
								</tr>
							</thead>
							<!--tr>
								<th><%tcWebApi_get("String_Entry","RC_GWDHCPEnable_in","s")%></th>
								<td>
									<select name="dr_enable_x" class="input_option">
									<option value="0" <%if tcWebApi_get("IPTV_Entry","dr_enable_x","h") = "0" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
									<option value="1" <%if tcWebApi_get("IPTV_Entry","dr_enable_x","h") = "1" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
									</select>
								</td>
							</tr-->
							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,11);"><%tcWebApi_get("String_Entry","RC_GWMulticastEnable_in","s")%></a></th>
								<td>
									<select name="mr_enable_x" class="input_option">
									<option value="0" <%if tcWebApi_get("IPTV_Entry","mr_enable_x","h") = "0" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
									<option value="1" <%if tcWebApi_get("IPTV_Entry","mr_enable_x","h") = "1" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
									</select>
								</td>
							</tr>
							<tr id="enable_eff_multicast_forward" style="display:none;">
								<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> efficient multicast forwarding</th>
								<td>
									<select name="emf_enable" class="input_option">
									<option value="0" selected ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
									<option value="1"  ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
									</select>
								</td>
							</tr>
							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(6, 6);"><%tcWebApi_get("String_Entry","RC_IPTV_in","s")%></a></th>
								<td>
									<input id="udpxy_enable_x" type="text" maxlength="5" class="input_6_table" name="udpxy_enable_x" value="<%tcWebApi_get("IPTV_Entry", "udpxy_enable_x", "s")%>" onkeypress="return is_number(this,event);">
								</td>
							</tr>
						</table>
						<div class="apply_gen">
							<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
						</div>
					</td>
					</tr>
				</tbody>
				</table>
			</td>
			</tr>
		</table>
	</td>
	<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</form>

</body>

<!--Advanced_IPTV_Content.asp-->
</html>

