<%
If Request_Form("editFlag") = "1" then
	tcWebApi_set("WLan_Common","editFlag","editFlag")
	tcWebApi_set("WLan_Common","MBSSID_changeFlag","MBSSID_changeFlag")
	tcWebApi_set("WLan_Common","MBSSID_able_Flag","MBSSID_able_Flag")

	tcWebApi_Set("WLan_Common","wl_unit","wl_unit")
	tcWebApi_Set("WLan_Common","wps_enable","wps_enable")

	If Request_Form("WpsSwitch")="1" Then
		TCWebApi_set("Info_WLan","WPSSwitchStatus","WpsSwitch")
		tcWebApi_commit("WLan_Entry")
	end if
end if

If Request_Form("WpsStart")="1" Then
	if Request_Form("wps_enable") = "1" then
		TCWebApi_set("Info_WLan","WPSActiveStatus","WpsStart")
		TCWebApi_set("WLan_Entry","WPSConfMode","WpsConfModeAll")
		TCWebApi_set("WLan_Entry","WPSMode","WPSMode_Selection")
		if Request_form("WPSMode_Selection") = "1" then
			TCWebApi_set("WLan_Entry","enrolleePinCode","WPSEnrolleePINCode")
		end if
	else
		TCWebApi_set("WLan_Entry","WPSConfMode","WpsConfModeNone")
	end if
	tcWebApi_commit("WLan_Entry")
End If

If Request_Form("WpsOOB")="1" Then
	TCWebApi_set("Info_WLan","WPSOOBActive","WpsOOB")
	TCWebApi_set("WLan_Entry","WPSConfStatus","WPSunConfigured")
	tcWebApi_commit("WLan_Entry")
end if

If Request_Form("WpsGenerate") = "1" then
	TCWebApi_set("Info_WLan","WPSGenPinCode","WpsGenerate")
	TCWebApi_commit("WLan_Entry")
end if

If Request_Form("editFlag") = "1" then
load_parameters_from_generic()
end if
load_parameters_to_generic()
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<!--Advanced_WWPS_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_1_2","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/ajax.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script>
var $j = jQuery.noConflict();
</script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';

function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }


var wireless = []; // [[MAC, associated, authorized], ...]
var wsc_config_state_old = '';	//should be useless in RT-65168. javi
var wps_enable_old = '<% tcWebApi_get("WLan_Common","wps_enable","s"); %>';	//should be useless in RT-65168. javi
var wl_wps_mode_old = 'disabled';	//should be useless in RT-65168. javi
var secs;
var timerID = null;
var timerRunning = false;
var timeout = 2000;
var delay = 1000;
var radio_2 = '<% tcWebApi_get("WLan_Entry","wl0_radio_on","s"); %>';
var radio_5 = '<% tcWebApi_get("WLan_Entry","wl1_radio_on","s"); %>';

function initial(){
	show_menu();

	if(!wl_info.band5g_support){	//single band
		document.getElementById("wps_band_tr").style.display = "none";

	}else{										//Dual band
		document.getElementById("wps_band_tr").style.display = "";
		if(document.form.wl_unit.value == 1){
			document.getElementById("wps_band_word").innerHTML = "5GHz";
		}
		else if(document.form.wl_unit.value == 0){
			document.getElementById("wps_band_word").innerHTML = "2.4GHz";
		}
	}

	if(!ValidateChecksum(document.form.wps_sta_pin.value) || document.form.wps_sta_pin.value == "00000000" || document.form.wps_sta_pin.value == ""){
		document.form.WPSMode_Selection[0].checked = true;
		changemethod(0);
	}
	else{
		document.form.WPSMode_Selection[1].checked = true;
		changemethod(1);
	}

	if((document.form.APOn.value == "0") || (document.form.radio_on.value == "0")){
		document.getElementById("wpsDesc").style.display = "none";
		document.getElementById("wpsmethod_tr").style.display = "none";
		document.getElementById("wps_conf_tr").style.display = "none";
		wps_enable_old = "0";
	}
	
	//Hide button WPS_reset or not
	if(wps_reset_support == -1){
		document.getElementById("wps_conf_tr").style.display = "none";
		document.getElementById("Reset_OOB").disabled = true;
	}
	
	loadXML();	
	if("<% tcWebApi_get("WLan_Entry","HideSSID","s"); %>" == "1"){		
		document.getElementById('WPS_hideSSID_hint').innerHTML = "<% tcWebApi_get("String_Entry","FW_note","s") %> <% tcWebApi_get("String_Entry","WPS_hideSSID_hint","s") %>";
		document.getElementById('WPS_hideSSID_hint').style.display = "";
	}

}

function SwitchBand(){
	if(wps_enable_old == "0"){
		if(document.form.wl_unit.value == "1")
			document.form.wl_unit.value = "0";
		else
			document.form.wl_unit.value = "1";
	}
	else{
		document.getElementById("wps_band_hint").innerHTML = "* <%tcWebApi_get("String_Entry","WC11b_x_WPSband_hint","s")%>";
		return false;
	}
	showLoading(2);
	setTimeout("redirect();", 2000);
	document.form.submit();
}

function ValidateChecksum(PIN){
var accum = 0;
accum += 3 * (parseInt(PIN / 10000000) % 10);
accum += 1 * (parseInt(PIN / 1000000) % 10);
accum += 3 * (parseInt(PIN / 100000) % 10);
accum += 1 * (parseInt(PIN / 10000) % 10);
accum += 3 * (parseInt(PIN / 1000) % 10);
accum += 1 * (parseInt(PIN / 100) % 10);
accum += 3 * (parseInt(PIN / 10) % 10);
accum += 1 * (parseInt(PIN / 1) % 10);
return ((accum % 10) == 0);
}
function PIN_PBC_Check(){
	var array_temp = new Array();
    if(document.form.wps_sta_pin.value != ""){
		if(document.form.wps_sta_pin.value.indexOf(' ')!= -1){
			array_temp = document.form.wps_sta_pin.value.split(" ");
			document.form.wps_sta_pin.value = array_temp[0] + array_temp[1];
		}
		else if(document.form.wps_sta_pin.value.indexOf('-')!= -1){
			array_temp = document.form.wps_sta_pin.value.split("-");
			document.form.wps_sta_pin.value = array_temp[0] + array_temp[1];
		}
    
		if(document.form.wps_sta_pin.value.length != 4 || isNaN(document.form.wps_sta_pin.value/1)){        //new format, 4 digits and doesn't need to checksum
			if(document.form.wps_sta_pin.value.length != 8 || !ValidateChecksum(document.form.wps_sta_pin.value)){
				alert("<%tcWebApi_get("String_Entry","JS_InvalidPIN","s")%>");
				document.form.wps_sta_pin.focus();
				return false;
			}
		}
	}
	return true;
}

function changemethod(wpsmethod){
	if(wpsmethod == 0){
		document.getElementById("starBtn").style.marginTop = "9px";
		document.getElementById("wps_sta_pin").style.display = "none";
	}
	else{
		document.getElementById("starBtn").style.marginTop = "5px";
		document.getElementById("wps_sta_pin").style.display = "";
	}
}

function doStartWPS(){
	if((document.form.WPSMode_Selection[1].checked)&&(document.form.isInWPSing.value==0)){
		if(PIN_PBC_Check() == false)
		{
			return ;
		}

		<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") <> "1"  then %>
		<%if tcWebApi_get("WLan_Entry", "WPSConfStatus", "h") = "2" then %>
	 	var pincode = document.form.WPSEnrolleePINCode;
        var len = pincode.value.length;
		if(len <= 0){
			alert("<%tcWebApi_get("String_Entry","WirelessJS32Text","s")%>");
			return;
		}
		<%end if%>
		<%end if%>
	}

	document.form.WpsStart.value = 1;
	document.form.WpsOOB.value = 0;
	document.form.submit();
}

function doResetWPS(){
	document.form.WpsOOB.value = 1;
	document.form.WpsStart.value = 0;
	showLoading(28);
	setTimeout("redirect();", 28000);
	document.form.submit();
}

function doGenerate(){
	document.form.WpsGenerate.value = "1";
	showLoading(28);
	setTimeout("redirect();", 28000);
	document.form.submit();
}
function doWPSUseChange(flag){
	if((document.form.APOn.value == "0") || (document.form.radio_on.value == "0")){
		flag = 0;
		//alert("the interface is disable.");
	}
	
	if(flag == 0){
		document.getElementById("wpsDesc").style.display = "none";
		document.getElementById("wpsmethod_tr").style.display = "none";
		document.form.wps_enable.value = "0";
	}
	else{
		document.getElementById("wpsDesc").style.display = "";
		document.getElementById("wpsmethod_tr").style.display = "";
		document.form.wps_enable.value = "1";
	}

	document.form.WpsSwitch.value = "1";
	showLoading(2);
	setTimeout("redirect();", 2000);
	document.form.editFlag.value = "1";
	document.form.submit();
	//show_wsc_status(wps_infos);//Viz add 2013.02 WPS 2.0 restrict by wifi auth "open-none, psk2, pskpsk2"
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_WWPS_Content.asp";
}

function InitializeTimer()
{
	if(document.form.wl_auth_mode_x.value == "SHARED"
			|| document.form.wl_auth_mode_x.value == "WPA"
			|| document.form.wl_auth_mode_x.value == "WPA1WPA2"
			|| document.form.wl_auth_mode_x.value == "Radius")
		return;

	msecs = timeout;
	StopTheClock();
	StartTheTimer();
}

function StopTheClock()
{
	if(timerRunning)
		clearTimeout(timerID);
	timerRunning = false;
}

function StartTheTimer(){
	if(msecs == 0){
		StopTheClock();

		if(stopFlag == 1)
			return;

		updateWPS();
		msecs = timeout;
		StartTheTimer();
	}
	else{
		msecs = msecs-500;
		timerRunning = true;
		timerID = setTimeout("StartTheTimer();", delay);
	}
}

function updateWPS(){
	var ie = window.ActiveXObject;

	if (ie)
		makeRequest_ie('/cgi-bin/WPS_info.xml?hash=' + Math.random().toString());
	else
		makeRequest('/cgi-bin/WPS_info.xml?hash=' + Math.random().toString());
}

function loadXML(){
	updateWPS();
	InitializeTimer();
}

function refresh_wpsinfo(xmldoc){
	var wpss = xmldoc.getElementsByTagName("wps");

	if(wpss == null || wpss[0] == null){
		if (confirm('<%tcWebApi_get("String_Entry","JS_badconnection","s")%>'))
			;
		else
			stopFlag=1;

		return;
	}

	var wps_infos = wpss[0].getElementsByTagName("wps_info");
	show_wsc_status(wps_infos);
}

function show_wsc_status(wps_infos){

	var controlDisplayItem = function () {
		document.getElementById("wps_state_tr").style.display = "none";
		document.getElementById("devicePIN_tr").style.display = "none";
		document.getElementById("wpsmethod_tr").style.display = "none";
	};

	// First filter whether turn on Wi-Fi or not
	if(document.form.wl_unit.value == 0 && radio_2 != "1") {        //2.4GHz
			document.getElementById("wps_enable_hint").innerHTML = "* <%tcWebApi_get("String_Entry","note_turn_wifi_on_WPS","s")%> <a style='color:#FC0; text-decoration: underline; font-family:Lucida Console;cursor:pointer;' href=\"Advanced_WAdvanced_Content.asp?af=radio_on\"><%tcWebApi_get("String_Entry","btn_go","s")%></a>";
			controlDisplayItem();
			return;
	}
	else if(document.form.wl_unit.value == 1 && radio_5 != "1") {   //5GHz
			document.getElementById("wps_enable_hint").innerHTML = "* <%tcWebApi_get("String_Entry","note_turn_wifi_on_WPS","s")%> <a style='color:#FC0; text-decoration: underline; font-family:Lucida Console;cursor:pointer;' href=\"Advanced_WAdvanced_Content.asp?af=radio_on\"><%tcWebApi_get("String_Entry","btn_go","s")%></a>"
			controlDisplayItem();
			return;
	}
	else if( (wps_infos[11].firstChild.nodeValue == "OPEN" && document.form.wl_wep_x.value != "0")
		  || wps_infos[11].firstChild.nodeValue == "SHARED"
		  || wps_infos[11].firstChild.nodeValue == "WPAPSK"
			|| wps_infos[11].firstChild.nodeValue == "WPA"
			|| wps_infos[11].firstChild.nodeValue == "WPA2"
			|| wps_infos[11].firstChild.nodeValue == "WPA1WPA2"
			|| wps_infos[11].firstChild.nodeValue == "Radius"){
		document.getElementById("wps_enable_hint").innerHTML = "<%tcWebApi_get("String_Entry","WPS_weptkip_hint","s")%><br><%tcWebApi_get("String_Entry","wsc_mode_hint1","s")%> <a style='color:#FC0; text-decoration: underline; font-family:Lucida Console;' href=\"Advanced_Wireless_Content.asp?af=wl_auth_mode_x\"><%tcWebApi_get("String_Entry","menu5_1_1","s")%></a> <%tcWebApi_get("String_Entry","wsc_mode_hint2","s")%>";
		controlDisplayItem();
		document.getElementById("wps_band_tr").style.display = "none";

		return;
	}

	// enable button
	if(wps_enable_old == "1"){
		document.getElementById("wps_enable_word").innerHTML = "<%tcWebApi_get("String_Entry","btn_Enabled","s")%>";
		document.getElementById("enableWPSbtn").value = "<%tcWebApi_get("String_Entry","btn_disable","s")%>";
		document.getElementById("switchWPSbtn").style.display = "none";
	}
	else{
		document.getElementById("wps_enable_word").innerHTML = "<%tcWebApi_get("String_Entry","btn_Disabled","s")%>"
		document.getElementById("enableWPSbtn").value = "<%tcWebApi_get("String_Entry","btn_Enabled","s")%>";

		if(wps_infos[12].firstChild.nodeValue == 0){
			document.getElementById("wps_band_word").innerHTML = "2.4GHz";
		}
		else if(wps_infos[12].firstChild.nodeValue == 1){
			document.getElementById("wps_band_word").innerHTML = "5GHz";
		}
		document.getElementById("switchWPSbtn").style.display = "";
	}	

	// WPS status
	if(wps_enable_old == "0"){
		document.getElementById("wps_state_tr").style.display = "";
		document.getElementById("wps_state_td").innerHTML = "Not used";
		document.getElementById("WPSConnTble").style.display = "none";
		document.getElementById("wpsDesc").style.display = "none";
	}
	else{
		document.getElementById("wps_state_tr").style.display = "";
		document.getElementById("wps_state_td").innerHTML = wps_infos[0].firstChild.nodeValue;
		document.getElementById("WPSConnTble").style.display = "";
		document.getElementById("wpsDesc").style.display = "";
	}

	// device's PIN code
	document.getElementById("devicePIN_tr").style.display = "";
	document.getElementById("devicePIN").value = wps_infos[7].firstChild.nodeValue;

	// the input of the client's PIN code
	document.getElementById("wpsmethod_tr").style.display = "";
	if(wps_enable_old == "1"){
		inputCtrl(document.form.wps_sta_pin, 1);
		if(wps_infos[1].firstChild.nodeValue == "Yes")
			document.getElementById("Reset_OOB").style.display = "";
		else
			document.getElementById("Reset_OOB").style.display = "none";
	}
	else{
		inputCtrl(document.form.wps_sta_pin, 0);
		document.getElementById("Reset_OOB").style.display = "none";
	}

	// show connecting btn
	/*
	if(wps_infos[0].firstChild.nodeValue == "Idle" || wps_infos[0].firstChild.nodeValue == "Configured"){
		show_method = 1;
	}
	else if(Rawifi_support != -1){ //ralink solutions
		var wpsState = wps_infos[0].firstChild.nodeValue;
		if(wpsState.search("Received M") != -1 || wpsState.search("Send M") != -1 || wpsState == "Success")
			show_method = 1;
	}

	if(show_method == 1) {
		document.getElementById("addEnrolleebtn_client").style.display = "";
		document.getElementById("WPSConnTble").style.display = "";
		document.getElementById("wpsDesc").style.display = "";
		document.form.wps_sta_pin.focus();
	}
	else{
		document.getElementById("addEnrolleebtn_client").style.display = "none";
		document.getElementById("WPSConnTble").style.display = "none";
		document.getElementById("wpsDesc").style.display = "none";
	}
	*/

	if(wps_infos[0].firstChild.nodeValue == "Start WPS Process")
		document.getElementById("wps_pin_hint").style.display = "inline";
	else
		document.getElementById("wps_pin_hint").style.display = "none";


	if(wps_infos[1].firstChild.nodeValue == "No")
			document.getElementById("wps_config_td").innerHTML = "No";
	else
			document.getElementById("wps_config_td").innerHTML = "Yes";
}

</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_WWPS_Content.asp" name="form" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_WWPS_Content.asp">
<input type="hidden" name="next_page" value="Advanced_WWPS_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="3">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wps_enable" value="<% if tcWebApi_get("WLan_Common","APOn","h") = "0" then asp_Write("0") elseif tcWebApi_get("WLan_Entry","radio_on","h") = "0" then asp_Write("0") else tcWebApi_get("WLan_Common","wps_enable","s") end if %>">
<input type="hidden" name="wl_wps_mode" value="disabled" disabled>   <!--wait to add function //javi-->
<input type="hidden" name="wl_auth_mode_x" value="<% tcWebApi_get("WLan_Entry","auth_mode_x","s"); %>">
<input type="hidden" name="wl_wep_x" value="<% tcWebApi_get("WLan_Entry","wep_x","s"); %>">
<input type="hidden" name="wl_unit" value="<% tcWebApi_get("WLan_Common","wl_unit","s"); %>">
<input type="hidden" name="wl_crypto" value="<% tcWebApi_get("WLan_Entry","wl_wep_type","s"); %>">
<input type="hidden" name="editFlag" value="0">
<input type="hidden" name="MBSSID_changeFlag" value="0">
<input type="hidden" name="MBSSID_able_Flag" value="0">
<INPUT TYPE="HIDDEN" NAME="WPSConfigured" value="2">
<INPUT TYPE="HIDDEN" NAME="WPSunConfigured" value="1">
<INPUT TYPE="HIDDEN" NAME="WpsConfModeAll" value="7">
<INPUT TYPE="HIDDEN" NAME="WpsConfModeNone" value="0">
<INPUT TYPE="HIDDEN" name="WpsSwitch" value="0">
<INPUT TYPE="HIDDEN" NAME="WpsStart" value="0">
<INPUT TYPE="HIDDEN" NAME="WpsOOB" value="0">
<INPUT TYPE="HIDDEN" NAME="isInWPSing" value="<%tcWebApi_get("Info_WLan","wlanWPStimerRunning","s")%>">
<INPUT TYPE="HIDDEN" NAME="WpsGenerate" value="0">
<INPUT TYPE="HIDDEN" NAME="APOn" value="<%tcWebApi_get("WLan_Common","APOn","s")%>">
<INPUT TYPE="HIDDEN" NAME="radio_on" value="<%tcWebApi_get("WLan_Entry","radio_on","s")%>">
<input type="hidden" name="w_Setting" value="1">
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
<td valign="top" >
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_1","s")%> - <%tcWebApi_get("String_Entry","menu5_1_2","s")%></div>
		  <div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc"><%tcWebApi_get("String_Entry","WC11b_display6_sd","s")%></div>
		  <div id="WPS_hideSSID_hint" class="formfontdesc" style="display:none;color:#FFCC00;"></div>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">

	<tr>
		<th width="30%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(13,1);"><%tcWebApi_get("String_Entry","WC11b_x_WPS_in","s")%></a></th>
		<td>
			<div id="wps_enable_block" style="display:none;">
				<span style="color:#FFF;" id="wps_enable_word">&nbsp;&nbsp;</span>
				<input type="button" name="enableWPSbtn" id="enableWPSbtn" value="" class="button_gen" onClick="enableWPS();">
				<br>
			</div>

			<div class="left" style="width: 94px;" id="radio_wps_enable"></div>
				<div class="clear"></div>
				<script type="text/javascript">
					$j('#radio_wps_enable').iphoneSwitch( document.form.wps_enable.value,
						 function() {
							doWPSUseChange(1);
						 },
						 function() {
							doWPSUseChange(0);			
						 },
						 {
							switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
						 }
					);
				</script>
				<span id="wps_enable_hint"></span>
	  </td>
	</tr>

	<tr id="wps_band_tr">
		<th width="30%"><a class="hintstyle" href="javascript:void(0);" onclick="openHint(13,5);"><%tcWebApi_get("String_Entry","Current_band","s")%></th>

		<td>
				<span class="devicepin" style="color:#FFF;" id="wps_band_word"></span>&nbsp;&nbsp;
				<input type="button" class="button_gen_long" name="switchWPSbtn" id="switchWPSbtn" value="<%tcWebApi_get("String_Entry","Switch_band","s")%>" class="button" onClick="SwitchBand();">
				<br><span id="wps_band_hint"></span>
	</td>
	</tr>

	<tr id="wps_state_tr">
		<th><% tcWebApi_Get("String_Entry", "PPPC_x_WANLink_in", "s") %></th>
		<td width="300">
			<span id="wps_state_td"></span>
			<img id="wps_pin_hint" style="display:none;" src="/images/InternetScan.gif" />
		</td>
	</tr>
	<tr id="wps_conf_tr">
		<th><% tcWebApi_Get("String_Entry", "WPSConfStatus1Text", "s") %></th>
		<td>
			<div style="margin-left:-10px">
				<table><tr>
					<td style="border:0px;" >
						<div class="devicepin" style="color:#FFF;" id="wps_config_td"></div>
					</td>
					<td style="border:0px">
						<input class="button_gen" type="button" onClick="doResetWPS();" id="Reset_OOB" name="Reset_OOB" value="Reset" style="padding:0 0.3em 0 0.3em;" >
					</td>
				</tr></table>
			</div>
		</td>
	</tr>

	<tr id="devicePIN_tr">
	  <th>
		<span id="devicePIN_name"><a class="hintstyle" href="javascript:void(0);" onclick="openHint(13,4);"><% tcWebApi_Get("String_Entry", "WC11b_x_DevicePIN_in", "s") %></a></span>
	  </th>
	  <td>
		<input type="text" name="devicePIN" id="devicePIN" value="" class="input_15_table" readonly="1" style="float:left;"></input>
	  </td>
	</tr>
</table>

<table id="WPSConnTble" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
	<div class="formfontdesc" style="padding-bottom:10px;padding-top:10px;display;" id="wpsDesc">
	<% tcWebApi_Get("String_Entry", "WPS_add_client", "s") %>
	</div>
	<tr id="wpsmethod_tr">
		<th>
		<span id="wps_method"><a class="hintstyle" href="javascript:void(0);" onclick="openHint(13,2);"><% tcWebApi_Get("String_Entry", "WC11b_x_WPSMode_in", "s") %></a></span>
		</th>
		<td>
			<input type="radio" value="0" name="WPSMode_Selection" class="input" onClick="changemethod(0);" <%If tcWebApi_get("WLan_Entry","WPSMode","h") = "0" then asp_Write("checked") end if%> >PBC
			<input type="radio" value="1" name="WPSMode_Selection" class="input" onClick="changemethod(1);" <%If tcWebApi_get("WLan_Entry","WPSMode","h") = "1" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","WPSMode","h") = "" then asp_Write("checked") end if%> >PIN Code
			<input type="text" id="wps_sta_pin" size="9" maxlength="9" class="input_12_table" name="WPSEnrolleePINCode" value="" onKeyPress="return validator.isString(this, event)">
			<div id="starBtn" style="margin-top:10px;">
				<input class="button_gen" type="button" id="addEnrolleebtn_client" style="margin-left:5px;" name="StartWPS" onClick="doStartWPS()" value="<% tcWebApi_Get("String_Entry", "WlanWPStimerRunning0Text", "s") %>">
			</div>
		</td>
	</tr>
</table>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_WWPS_Content.asp-->
</html>

