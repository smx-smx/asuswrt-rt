<%
tcWebApi_set("WLan_Common","editFlag","editFlag")
tcWebApi_set("WLan_Common","MBSSID_changeFlag","MBSSID_changeFlag")
tcWebApi_set("WLan_Common","MBSSID_able_Flag","MBSSID_able_Flag")
If Request_Form("editFlag") = "1" then
	tcWebApi_Set("SysInfo_Entry","w_Setting","w_Setting")
	tcWebApi_commit("SysInfo_Entry")

	tcWebApi_Set("WLan_Common","wl_unit","wl_unit")
	tcWebApi_Set("WLan_Entry","ssid","wl_ssid")
	tcWebApi_Set("WLan_Entry","HideSSID","wl_closed")
	tcWebApi_Set("WLan_Common","WirelessMode","wl_nmode_x")
	tcWebApi_Set("WLan_Common","BGProtection","wl_gmode_protection")
	tcWebApi_Set("WLan_Common","Channel","wl_channel")
	tcWebApi_Set("WLan_Common","acs_ch13","acs_ch13")
	tcWebApi_Set("WLan_Common","acs_dfs","acs_dfs")
	tcWebApi_Set("WLan_Common","HT_BW","wl_bw")
	tcWebApi_Set("WLan_Common","HT_EXTCHA","wl_nctrlsb")
	tcWebApi_Set("WLan_Entry","auth_mode_x","wl_auth_mode_save")
	tcWebApi_Set("WLan_Entry","crypto","wl_crypto_save")
	tcWebApi_Set("WLan_Entry","wpa_psk","wl_wpa_psk")
	tcWebApi_Set("WLan_Entry","wep_x","wl_wep_x")
	tcWebApi_Set("WLan_Entry","key","wl_key")
	tcWebApi_Set("WLan_Entry","key1","wl_key1")
	tcWebApi_Set("WLan_Entry","key2","wl_key2")
	tcWebApi_Set("WLan_Entry","key3","wl_key3")
	tcWebApi_Set("WLan_Entry","key4","wl_key4")
	tcWebApi_Set("WLan_Entry","phrase_x","wl_phrase_x")
	tcWebApi_Set("WLan_Entry","RekeyInterval","wl_wpa_gtk_rekey")
	tcWebApi_Set("WLan_Entry","WPSConfStatus","WPSConfigured")
	load_parameters_from_generic()
	tcWebApi_commit("WLan_Entry")
end if

load_parameters_to_generic()
%>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_Wireless_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <% tcWebApi_Get("String_Entry", "menu5_1_1", "s") %></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<link href="/other.css" rel="stylesheet" type="text/css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/md5.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var wireless = []; // [[MAC, associated, authorized], ...]
var cur_control_channel = ["<% tcWebApi_get("Info_WLan","wlanCurChannel_2G","s"); %>", "<% tcWebApi_get("Info_WLan","wlanCurChannel_5G","s"); %>"];

var wl_unit = "<% tcWebApi_get("WLan_Common","wl_unit","s") %>";

function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }


function initial(){
	show_menu();
	load_body();
	autoFocus('<% get_parameter("af"); %>');
	
	if(wl_info.band5g_support && band5g_11ac_support != -1 && document.form.wl_unit[1].selected == true){
		document.getElementById('wl_mode_desc').onclick=function(){return openHint(1, 5)};		
	}else if(wl_info.band5g_support && document.form.wl_unit[1].selected == true){
		document.getElementById('wl_mode_desc').onclick=function(){return openHint(1, 4)};
	}	

	insertExtChannelOption();
	wl_auth_mode_change(1);	
	
	document.form.wl_ssid.value =  decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","ssid","s") %>');
	document.form.wl_wpa_psk.value =  decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","wpa_psk","s") %>');
	document.form.wl_key1.value =  decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","key1","s") %>');
	document.form.wl_key2.value =  decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","key2","s") %>');
	document.form.wl_key3.value =  decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","key3","s") %>');
	document.form.wl_key4.value =  decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","key4","s") %>');
	document.form.wl_phrase_x.value =  decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","phrase_x","s") %>');
	document.form.wl_channel.value = document.form.wl_channel_orig.value;
		
	if(document.form.wl_unit[0].selected == true)
	{
		document.getElementById("wl_gmode_checkbox").style.display = "";
		if(document.form.wl_nmode_x.value=='6'){
			document.form.wl_gmode_check.checked = false;
			document.getElementById("wl_gmode_check").disabled = true;
		}
		else{
			document.form.wl_gmode_check.checked = true;
			document.getElementById("wl_gmode_check").disabled = false;
		}
	}
	else{	//5G uses the different Wireless Mode, added by Javier.
		var algos;
		algos = new Array("<% tcWebApi_Get("String_Entry", "Auto", "s") %>", "N Only", "Legacy", "N/AC mixed");
		/* Reconstruct algorithm array from new crypto algorithms */
		document.form.wl_nmode_x.length = algos.length;
		for(i=0; i<algos.length; i++){
			switch(i) {	//Viz Mod 2014.09.30 for 802.11ac MTK
				case 0:
					document.form.wl_nmode_x[i] = new Option(algos[i], "14");
					break;
				case 1:
					document.form.wl_nmode_x[i] = new Option(algos[i], "11");
					break;
				case 2:
					document.form.wl_nmode_x[i] = new Option(algos[i], "2");
					break;
				case 3:
					document.form.wl_nmode_x[i] = new Option(algos[i], "15");
					break;	
				default:
					document.form.wl_nmode_x[i] = new Option(algos[i], "14");
			}

			if(document.form.wl_nmode_x[i].value == '<%tcWebApi_get("WLan_Common","WirelessMode","s")%>')
				document.form.wl_nmode_x[i].selected = true;
		}
	}

	if((document.form.wl_gmode_protection.value == "0") || (document.form.wl_gmode_protection.value == "1"))
		document.form.wl_gmode_check.checked = true;
	else
		document.form.wl_gmode_check.checked = false;
		
	if('<% tcWebApi_get("WLan_Entry","HideSSID","s") %>' == '1'){
		document.getElementById('WPS_hideSSID_hint').style.display = "";	
	}		
		
	if(!wl_info.band5g_support)
		document.getElementById("wl_unit_field").style.display = "none";			
		
	change_wl_nmode(document.form.wl_nmode_x);

	if(support_dfs == 1 && document.form.wl_channel.value  == '0' && wl_unit == '1'){	//support_dfs, 5GHz, Auto channel
		document.getElementById('dfs_checkbox').style.display = "";
		check_DFS_support(document.form.acs_dfs_checkbox);
	}

	handle_11ac_80MHz();	
		
	limit_auth_method();

	if(document.form.wl_channel.value  == '0'){
		document.getElementById("auto_channel").style.display = "";
		document.getElementById("auto_channel").innerHTML = "Current control channel: "+cur_control_channel[document.form.wl_unit.value];

		if (wl_unit == '0' && document.getElementById("wl_channel").length > 12) {	//with channel 12, 13
			document.getElementById('acs_ch13_checkbox').style.display = "";				
		}
	}
}

function add_options_value(o, arr, orig){
if(orig == arr)
add_option(o, "mbss_"+arr, arr, 1);
else
add_option(o, "mbss_"+arr, arr, 0);
}

//javi
//For using the format of ASUS WRT, we follow the JAVA script.
//However, the parameter wl_auth_mode_x has many JAVA code in general.js, it is default to change the value of option type.
//We must add the function to translate the value of parameters between ASUS WRT and RT-65168.
function TranslateWRTtoMTK(){
	var auth_mode = document.form.wl_auth_mode_x.value;
	var EncrypType = document.form.wl_crypto.value
	if(auth_mode == "open")
		document.form.wl_auth_mode_save.value = "OPEN";
	else if(auth_mode == "shared")
		document.form.wl_auth_mode_save.value = "SHARED";
	else if(auth_mode == "psk")
		document.form.wl_auth_mode_save.value = "WPAPSK";
	else if(auth_mode == "psk2")
		document.form.wl_auth_mode_save.value = "WPA2PSK";
	else if(auth_mode == "pskpsk2")
		document.form.wl_auth_mode_save.value = "WPAPSKWPA2PSK";
	else if(auth_mode == "wpa")
		document.form.wl_auth_mode_save.value = "WPA";
	else if(auth_mode == "wpa2")
		document.form.wl_auth_mode_save.value = "WPA2";
	else if(auth_mode == "wpawpa2")
		document.form.wl_auth_mode_save.value = "WPA1WPA2";
	else if(auth_mode == "radius")
		document.form.wl_auth_mode_save.value = "Radius";
	else
		document.form.wl_auth_mode_save.value = "OPEN";

	if(auth_mode == "open")	//avoid to misunderstand that had been encrypt.
		document.form.wl_crypto_save.value = "NONE";
	else
		document.form.wl_crypto_save.value = document.form.wl_crypto.value;
}

function check_acs_ch13_support(obj){
	if(obj.checked)
	{
		document.form.acs_ch13.value = 1;
	}
	else
	{
		document.form.acs_ch13.value = 0;
	}
}

function redirect(flag){
	if(flag=="0")
		document.location.href = "/cgi-bin/Advanced_WSecurity_Content.asp";
	else
		document.location.href = "/cgi-bin/Advanced_Wireless_Content.asp";
}

function applyRule(){
	var auth_mode = document.form.wl_auth_mode_x.value;	
	if(validForm()){
		document.form.wps_config_state.value = "1";
		if((auth_mode == "shared" || auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "wpawpa2" || auth_mode == "radius" ||
		((auth_mode == "open") && !(document.form.wl_wep_x.value == "0")))
		&& document.form.wps_mode.value == "enabled")
			document.form.wps_mode.value = "disabled";

		if(document.form.wl_nmode_x.value == "6" || document.form.wl_nmode_x.value == "11")
			document.form.wl_gmode_protection.value = "2";

		TranslateWRTtoMTK();
		document.form.action = "/cgi-bin/Advanced_Wireless_Content.asp";
		document.form.editFlag.value = "1" ;

		if(model_name == "DSL-N66U")
		{
			showLoading(28);
			if(auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "wpawpa2" || auth_mode == "radius")
				setTimeout("redirect(0);", 28000);
			else
				setTimeout("redirect(1);", 28000);
		}
		else
		{
			showLoading(23);
			if(auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "wpawpa2" || auth_mode == "radius")
				setTimeout("redirect(0);", 23000);
			else
				setTimeout("redirect(1);", 23000);
		}

		if(navigator.appName.indexOf("Microsoft") >= 0){		// Jieming added at 2013/05/21, to avoid browser freeze when submitting form on IE
			stopFlag = 1;
		}
		
		document.form.submit();
	}
}

function validForm(){
	var auth_mode = document.form.wl_auth_mode_x.value;
	if(!validate_string_ssid(document.form.wl_ssid))
		return false;

	if(!check_NOnly_to_GN()){
		autoFocus('wl_nmode_x');
		return false;
	}

	if(document.form.wl_wep_x.value != "0")
		if(!validate_wlphrase('WLANConfig11b', 'wl_phrase_x', document.form.wl_phrase_x))
				return false;
	if(auth_mode == "psk" || auth_mode == "psk2" || auth_mode == "pskpsk2"){ //2008.08.04 lock modified
		if(!validate_psk(document.form.wl_wpa_psk))
				return false;
		
		//confirm common string combination     #JS_common_passwd#
                var is_common_string = check_common_string(document.form.wl_wpa_psk.value, "wpa_key");
                if(is_common_string){
                        if(!confirm("<% tcWebApi_Get("String_Entry", "JS_common_passwd","s") %>")){
                                document.form.wl_wpa_psk.focus();
                                document.form.wl_wpa_psk.select();
                                return false;   
                        }       
                }	

		if(!validate_range(document.form.wl_wpa_gtk_rekey, 0, 2592000))
				return false;
	}
	else if(auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "wpawpa2"){
		if(!validate_range(document.form.wl_wpa_gtk_rekey, 0, 2592000))
				return false;
	}
	else{
		var cur_wep_key = eval('document.form.wl_key'+document.form.wl_key.value);
		if(auth_mode != "radius" && !validate_wlkey(cur_wep_key))
				return false;
	}
	return true;
}

function done_validating(action){
refreshpage();
}

function change_key_des(){
	var objs = getElementsByName_iefix("span", "key_des");
	var wep_type = document.form.wl_wep_x.value;
	var str = "";
	if(wep_type == "1")
		str = "(5 ASCII digits or 10 hex digits)";
	else if(wep_type == "2")
		str = "(13 ASCII digits or 26 hex digits)";
	
	for(var i = 0; i < objs.length; ++i)
		showtext(objs[i], str);
}

function validate_wlphrase(s, v, obj){
if(!validate_string(obj)){
is_wlphrase(s, v, obj);
return(false);
}
return true;
}
function disableAdvFn(){
for(var i=18; i>=3; i--)
document.getElementById("WLgeneral").deleteRow(i);
}

function _change_wl_unit(wl_unit){
	document.form_band.wl_unit.value = wl_unit;
	document.form_band.action = "/cgi-bin/Advanced_Wireless_Content.asp";
	showLoading(2);
	setTimeout("redirect(1);", 2000);
	document.form_band.submit();
}

function check_NOnly_to_GN(){
	// Viz add 2012.11.05 restriction for 'N Only' mode  ( start
	if((document.form.wl_nmode_x.value == "11" || document.form.wl_nmode_x.value == "6") && document.form.wl_unit[1].selected == true){		//5G: NOnly
			for(var i=0;i<gn_array_5g.length;i++){
					if(gn_array_5g[i][0] == "1"
							&& (gn_array_5g[i][3] == "tkip" || gn_array_5g[i][5] == "1" || gn_array_5g[i][5] == "2")){
								document.getElementById('wl_NOnly_note').style.display = "";
								return false;
					}
			}
	}else if(document.form.wl_nmode_x.value == "6" && document.form.wl_unit[0].selected == true){		//2.4G: NOnly
			for(var i=0;i<gn_array_2g.length;i++){
					if(gn_array_2g[i][0] == "1"
							&& (gn_array_2g[i][3] == "tkip" || gn_array_2g[i][5] == "1" || gn_array_2g[i][5] == "2")){
								document.getElementById('wl_NOnly_note').style.display = "";
								return false;
					}
			}
	}
	document.getElementById('wl_NOnly_note').style.display = "none";
	return true;
//  Viz add 2012.11.05 restriction for 'N Only' mode  ) end
}

/* Handle wireless mode changed */
function change_wl_nmode(obj){	
	if(obj.value=='9' || obj.value=='0')	//2.4GHZ: auto, legacy
		inputCtrl(document.form.wl_gmode_check, 1);
	else
		inputCtrl(document.form.wl_gmode_check, 0);
	
	free_options(document.form.wl_bw);
	var bws = new Array();
	var bwsDesc = new Array();
	var cur = "<%tcWebApi_get("WLan_Common","HT_BW","s")%>";		
	if(obj.value == '0' || obj.value == '2') { //legacy: 0 for 2.4GHz, 2 for 5GHz		
		bws = [0];
		bwsDesc = ["20 MHz"];
		add_options_x2(document.form.wl_bw, bwsDesc, bws, cur);
	}
	else {
		bws = [1, 0, 2, 3];
		bwsDesc = ["20/40/80 MHz", "20 MHz", "40 MHz", "80 MHz"];
		add_options_x2(document.form.wl_bw, bwsDesc, bws, cur);
		handle_11ac_80MHz();
	}

	insertExtChannelOption();	
	limit_auth_method();
	automode_hint();
	check_NOnly_to_GN();
}

function check_DFS_support(obj){
	if(obj.checked)
		document.form.acs_dfs.value = 1;
	else
		document.form.acs_dfs.value = 0;
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>
<div id="hiddenMask" class="popup_bg">
<table cellpadding="4" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
<tr>
<td>
<div class="drword" id="drword">Please wait, Proceeding...
<br/>
<br/>
</div>
<div class="drImg"><img src="/images/alertImg.png"></div>
<div style="height:70px; "></div>
</td>
</tr>
</table>
</div>

<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form_band" action="Advanced_Wireless_Content.asp" target="hidden_frame">
<input type="hidden" name="wl_unit">
<input type="hidden" name="editFlag" value="0">
</form>
<form method="post" name="form" action="Advanced_Wireless_Content.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="wan_route_x" value="">
<input type="hidden" name="wan_nat_x" value="1">
<input type="hidden" name="current_page" value="Advanced_Wireless_Content.asp">
<input type="hidden" name="next_page" value="Advanced_Wireless_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_wireless">
<input type="hidden" name="action_wait" value="3">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="wl_country_code" value="TW" disabled>
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wps_mode" value="1">
<input type="hidden" name="wps_config_state" value="">
<input type="hidden" name="wl_ssid_org" value="IBIZA_2.4G">
<input type="hidden" name="wlc_ure_ssid_org" value="" disabled>
<input type="hidden" name="wl_wpa_mode" value="0">
<input type="hidden" name="wl_wpa_psk_org" value="12345678">
<input type="hidden" maxlength="15" size="15" name="x_RegulatoryDomain" value="" readonly="1">
<input type="hidden" name="wl_gmode_protection" value='<% tcWebApi_get("WLan_Common","BGProtection","s"); %>'>
<input type="hidden" name="wl_mode_x" value="0">
<input type="hidden" name="wl_nmode" value="">
<input type="hidden" name="wl_nctrlsb_old" value="<% tcWebApi_get("WLan_Common","HT_EXTCHA","s"); %>">
<input type="hidden" name="wl_key_type" value='0'>
<input type="hidden" name="wl_channel_orig" value='<% tcWebApi_get("WLan_Common","Channel","s"); %>'>
<input type="hidden" name="acs_ch13" value='<% tcWebApi_get("WLan_Common","acs_ch13","s"); %>'>
<input type="hidden" name="wl_wep_x_orig" value='0'>
<input type="hidden" name="wl_subunit" value='-1'>
<input type="hidden" name="acs_dfs" value='<% tcWebApi_get("WLan_Common","acs_dfs","s"); %>'>
<input type="hidden" name="editFlag" value="0">
<input type="hidden" name="wl_auth_mode_save" value="OPEN">
<input type="hidden" name="wl_crypto_save" value="NONE">
<input type="hidden" name="MBSSID_changeFlag" value="0">
<input type="hidden" name="MBSSID_able_Flag" value="0">
<input type="hidden" name="w_Setting" value="1">
<input type="hidden" name="WPSConfigured" value="2">
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
<td align="left" valign="top" >
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_1","s")%> - <% tcWebApi_Get("String_Entry", "menu5_1_1", "s") %></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><% tcWebApi_Get("String_Entry", "adv_wl_desc", "s") %></div>


<table width="99%" border="1" align="center" cellpadding="4" cellspacing="0" id="WLgeneral" class="FormTable">
	<tr id="wl_unit_field">
		<th><% tcWebApi_Get("String_Entry", "Interface", "s") %></th>
		<td>
			<select name="wl_unit" class="input_option" onChange="_change_wl_unit(this.value);">
				<option class="content_input_fd" value="0" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "0" then asp_Write("selected") end if %>>2.4GHz</option>
				<option class="content_input_fd" value="1" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "1" then asp_Write("selected") end if %>>5GHz</option>
			</select>
		</td>
	</tr>
<!--tr id="wl_subunit_field" style="display:none">
<th>Multiple SSID index</th>
<td>
<select name="wl_subunit" class="input_option" onChange="change_wl_unit();">
<option class="content_input_fd" value="0" >Primary</option>
</select>
<select id="wl_bss_enabled_field" name="wl_bss_enabled" class="input_option" onChange="mbss_switch();">
<option class="content_input_fd" value="0" >Disable</option>
<option class="content_input_fd" value="1" selected>Enable</option>
</select>
</td>
</tr-->
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 1);">SSID</a></th>
		<td>
			<input type="text" maxlength="32" class="input_32_table" id="wl_ssid" name="wl_ssid" value="<% If tcWebApi_get("WLan_Entry","ssid","h") <> "" then  tcWebApi_get("WLan_Entry","ssid","s") else asp_Write("ASUS_VSL_N66U") end if %>" onkeypress="return validator.isString(this, event)">
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 2);"><% tcWebApi_Get("String_Entry", "WC11b_x_BlockBCSSID_in", "s") %></a></th>
		<td>
			<input type="radio" value="1" name="wl_closed" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '1')" <% if tcWebApi_get("WLan_Entry","HideSSID","h") = "1" then asp_Write("checked") end if %>>Yes
			<input type="radio" value="0" name="wl_closed" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_closed', '0')" <% if tcWebApi_get("WLan_Entry","HideSSID","h") = "0" then asp_Write("checked") end if %>>No
			<span id="WPS_hideSSID_hint" style="display:none;"><%tcWebApi_get("String_Entry","WPS_hideSSID_hint","s")%></span>	
		</td>
	</tr>
	<tr>
		<th><a id="wl_mode_desc" class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 4);"><% tcWebApi_Get("String_Entry", "WC11b_x_Mode11g_in", "s") %></a></th>
		<td>
			<select name="wl_nmode_x" class="input_option" onChange="change_wl_nmode(this);">
				<option value="9" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "9" then asp_Write("selected") end if %>><% tcWebApi_Get("String_Entry", "Auto", "s") %></option>
				<option value="6" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "6" then asp_Write("selected") end if %>>N Only</option>
				<option value="0" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "0" then asp_Write("selected") end if %>>Legacy</option>
				<!-- <option value="7" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "7" then asp_Write("selected") end if %>>802.11g+n</option> -->
			</select>
			<!-- need to make sure the meanings of WirelessMode and verify the checkbox behavior //javi-->
			<span id="wl_gmode_checkbox" style="display:none;"><input type="checkbox" name="wl_gmode_check" id="wl_gmode_check" value="" onClick="return change_common(this, 'WLANConfig11b', 'wl_gmode_check', '1')"> <%tcWebApi_get("String_Entry","WC11b_x_Mode11g_protectbg","s")%></input></span>
			<span id="wl_nmode_x_hint" style="display:none;"><br><%tcWebApi_get("String_Entry","WC11n_automode_limition_hint","s")%></span>
			<span id="wl_NOnly_note" style="display:none;"><br>* <%tcWebApi_get("String_Entry","WC11n_NOnly_note","s")%></span>
		</td>
	</tr>
	<tr id="wl_bw_field">
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 14);"><% tcWebApi_Get("String_Entry", "WC11b_ChannelBW_in", "s") %></a></th>
		<td>
			<select name="wl_bw" class="input_option" onChange="insertExtChannelOption();">
				<option class="content_input_fd" value="1" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "1" then asp_Write("selected") end if %>>20/40/80 MHz</option>
				<option class="content_input_fd" value="0" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "0" then asp_Write("selected") end if %>>20 MHz</option>				
				<option class="content_input_fd" value="2" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "2" then asp_Write("selected") end if %>>40 MHz</option>
				<option class="content_input_fd" value="3" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "3" then asp_Write("selected") end if %>>80 MHz</option>
			</select>
		</td>
	</tr>
	<tr id="wl_channel_field">
		<th><a id="wl_channel_select" class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 3);"><% tcWebApi_Get("String_Entry", "WC11b_Channel_in", "s") %></a></th>
		<td>
			<select id="wl_channel" name="wl_channel" class="input_option" onChange="change_channel();">
				<option value="0" <% if tcWebApi_get("WLan_Common","Channel","h") = "0" then asp_Write("selected") end if %>><% tcWebApi_Get("String_Entry", "Auto", "s") %></option>
				<option value="1" <% if tcWebApi_get("WLan_Common","Channel","h") = "1" then asp_Write("selected") end if %>>1</option>
				<option value="2" <% if tcWebApi_get("WLan_Common","Channel","h") = "2" then asp_Write("selected") end if %>>2</option>
				<option value="3" <% if tcWebApi_get("WLan_Common","Channel","h") = "3" then asp_Write("selected") end if %>>3</option>
				<option value="4" <% if tcWebApi_get("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>4</option>
				<option value="5" <% if tcWebApi_get("WLan_Common","Channel","h") = "5" then asp_Write("selected") end if %>>5</option>
				<option value="6" <% if tcWebApi_get("WLan_Common","Channel","h") = "6" then asp_Write("selected") end if %>>6</option>
				<option value="7" <% if tcWebApi_get("WLan_Common","Channel","h") = "7" then asp_Write("selected") end if %>>7</option>
				<option value="8" <% if tcWebApi_get("WLan_Common","Channel","h") = "8" then asp_Write("selected") end if %>>8</option>
				<option value="9" <% if tcWebApi_get("WLan_Common","Channel","h") = "9" then asp_Write("selected") end if %>>9</option>
				<option value="10" <% if tcWebApi_get("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10</option>
				<option value="11" <% if tcWebApi_get("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11</option>
			</select>
			<span id="auto_channel" style="display:none;margin-left:10px;"></span><br>
			<span id="dfs_checkbox" style="display:none"><input type="checkbox" onClick="check_DFS_support(this);" name="acs_dfs_checkbox" <% if tcWebApi_get("WLan_Common","acs_dfs","h") = "1" then asp_Write("checked") end if %>><% tcWebApi_Get("String_Entry", "WC11b_EChannel_dfs", "s") %></input></span>
			<span id="acs_ch13_checkbox" style="display:none;"><input type="checkbox" onClick="check_acs_ch13_support(this);"  <% tcWebApi_MatchThenWrite("WLan_Common","acs_ch13","1","checked") %>>Auto select channel including channel 12, 13</input></span>
		</td>
	</tr>
	<tr id="wl_nctrlsb_field">
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 15);"><% tcWebApi_Get("String_Entry", "WC11b_EChannel_in", "s") %></a></th>
		<td>
			<select name="wl_nctrlsb" class="input_option">
				<option value="0" <% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "0" then asp_Write("selected") end if %>>lower</option>
				<option value="1" <% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "1" then asp_Write("selected") end if %>>upper</option>
			</select>
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 5);"><% tcWebApi_Get("String_Entry", "WC11b_AuthenticationMethod_in", "s") %></a></th>
		<td>
			<select name="wl_auth_mode_x" class="input_option" onChange="authentication_method_change(this);">
				<option value="open" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "OPEN" then asp_Write("selected") end if %>>Open System</option>
				<option value="shared" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "SHARED" then asp_Write("selected") end if %>>Shared Key</option>
				<option value="psk" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "WPAPSK" then asp_Write("selected") end if %>>WPA-Personal</option>
				<option value="psk2" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "WPA2PSK" then asp_Write("selected") end if %>>WPA2-Personal</option>
				<option value="pskpsk2" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "WPAPSKWPA2PSK" then asp_Write("selected") end if %>>WPA-Auto-Personal</option>
				<option value="wpa" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "WPA" then asp_Write("selected") end if %>>WPA-Enterprise</option>
				<option value="wpa2" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "WPA2" then asp_Write("selected") end if %>>WPA2-Enterprise</option>
				<option value="wpawpa2" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "WPA1WPA2" then asp_Write("selected") end if %>>WPA-Auto-Enterprise</option>
				<option value="radius" <% if tcWebApi_get("WLan_Entry","auth_mode_x","h") = "Radius" then asp_Write("selected") end if %>>Radius with 802.1x</option>
			</select>
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 6);"><% tcWebApi_Get("String_Entry", "WC11b_WPAType_in", "s") %></a></th>
		<td>
			<select name="wl_crypto" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_crypto')">
				<option value="AES" <% if tcWebApi_get("WLan_Entry","crypto","h") = "AES" then asp_Write("selected") end if %>>AES</option>
				<option value="TKIP+AES" <% if tcWebApi_get("WLan_Entry","crypto","h") = "TKIP+AES" then asp_Write("selected") end if %>>TKIP+AES</option>
			</select>
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 7);">WPA Pre-Shared Key</a></th>
		<td>
			<input type="password" name="wl_wpa_psk" maxlength="65" class="input_32_table" value="<% If tcWebApi_get("WLan_Entry","wpa_psk","h") <> "" then tcWebApi_get("WLan_Entry","wpa_psk","s") end if %>" onBlur="switchType(this, false);" onFocus="switchType(this, true);">
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 9);">WEP Encryption</a></th>
		<td>
			<select name="wl_wep_x" class="input_option" onChange="wep_encryption_change(this);">
				<option value="0" <% if tcWebApi_get("WLan_Entry","wep_x","h") = "0" then asp_Write("selected") end if %>>None</option>
				<option value="1" <% if tcWebApi_get("WLan_Entry","wep_x","h") = "1" then asp_Write("selected") end if %>>WEP-64bits</option>
				<option value="2" <% if tcWebApi_get("WLan_Entry","wep_x","h") = "2" then asp_Write("selected") end if %>>WEP-128bits</option>
			</select>
			<span name="key_des"></span>
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 10);">Key Index</a></th>
		<td>
			<select name="wl_key" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_key');">
				<option value="1" <% if tcWebApi_get("WLan_Entry","key","h") = "1" then asp_Write("selected") end if %>>1</option>
				<option value="2" <% if tcWebApi_get("WLan_Entry","key","h") = "2" then asp_Write("selected") end if %>>2</option>
				<option value="3" <% if tcWebApi_get("WLan_Entry","key","h") = "3" then asp_Write("selected") end if %>>3</option>
				<option value="4" <% if tcWebApi_get("WLan_Entry","key","h") = "4" then asp_Write("selected") end if %>>4</option>
			</select>
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 18);">WEP Key 1:</th>
		<td><input type="text" name="wl_key1" id="wl_key1" maxlength="32" class="input_32_table" value="<% If tcWebApi_get("WLan_Entry","key1","h") <> "" then  tcWebApi_get("WLan_Entry","key1","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"></td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 18);">WEP Key 2:</th>
		<td><input type="text" name="wl_key2" id="wl_key2" maxlength="32" class="input_32_table" value="<% If tcWebApi_get("WLan_Entry","key2","h") <> "" then  tcWebApi_get("WLan_Entry","key2","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"></td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 18);">WEP Key 3:</th>
		<td><input type="text" name="wl_key3" id="wl_key3" maxlength="32" class="input_32_table" value="<% If tcWebApi_get("WLan_Entry","key3","h") <> "" then  tcWebApi_get("WLan_Entry","key3","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"></td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 18);">WEP Key 4:</th>
		<td><input type="text" name="wl_key4" id="wl_key4" maxlength="32" class="input_32_table" value="<% If tcWebApi_get("WLan_Entry","key4","h") <> "" then  tcWebApi_get("WLan_Entry","key4","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"></td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 8);">ASUS Passphrase:</a></th>
		<td>
			<input type="text" name="wl_phrase_x" maxlength="64" class="input_32_table" value="<% If tcWebApi_get("WLan_Entry","phrase_x","h") <> "" then  tcWebApi_get("WLan_Entry","phrase_x","s") end if %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_phrase_x', this);">
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 11);"><% tcWebApi_Get("String_Entry", "WC11b_x_Rekey_in", "s") %></a></th>
		<td><input type="text" maxlength="7" name="wl_wpa_gtk_rekey" class="input_6_table" value="<% If tcWebApi_get("WLan_Entry","RekeyInterval","h") <> "" then  tcWebApi_get("WLan_Entry","RekeyInterval","s") else asp_Write("3600") end if %>" onKeyPress="return validator.isNumber(this,event);" onblur="validate_number_range(this, 0, 2592000)"></td>
	</tr>
</table>


<div class="apply_gen">
<input type="button" id="applyButton" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>" onclick="applyRule();">
</div>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>

</td>
<td width="10" align="center" valign="top"></td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_Wireless_Content.asp-->
</html>

