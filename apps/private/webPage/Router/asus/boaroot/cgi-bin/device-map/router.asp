<%
tcWebApi_set("WLan_Common","editFlag","editFlag")
If Request_Form("editFlag") = "1" then
	tcWebApi_Set("SysInfo_Entry","w_Setting","w_Setting")
	tcWebApi_commit("SysInfo_Entry")

	tcWebApi_Set("WLan_Common","wl_unit","wl_unit")
	tcWebApi_Set("WLan_Entry","ssid","wl_ssid")
	tcWebApi_Set("WLan_Entry","HideSSID","wl_closed")
	tcWebApi_Set("WLan_Common","WirelessMode","wl_nmode_x")
	tcWebApi_Set("WLan_Common","BGProtection","wl_gmode_protection")
	tcWebApi_Set("WLan_Common","Channel","wl_channel")
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
	
	tcWebApi_commit("WLan_Entry")
end if

If Request_Form("editFlag") = "1" then
load_parameters_from_generic()
end if
load_parameters_to_generic()
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--device-map/router.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title></title>
<link href="/NM_style.css" rel="stylesheet" type="text/css" />
<link href="/form_style.css" rel="stylesheet" type="text/css" />
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/md5.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script>
function redirect(){
	document.location.href = "/cgi-bin/device-map/router.asp";
}

function redirectParent(){
	parent.document.location.href = document.form.next_page.value;
}

function tabclickhandler(wl_unit){
	if(wl_unit == 2){
		location.href = "router_status.asp";
	}
	else{
		if(!wl_info.band5g_support){
			redirect();
		}
		else{
			document.form_band.wl_unit.value = wl_unit;			
			document.form_band.submit();
			setTimeout("redirect();", 1000);
		}
	}
}

//javi
//For using the format of ASUS WRT, we follow the JAVA script.
//However, the parameter wl_auth_mode_x has many JAVA code in general.js, it is default to change the value of option type.
//We must add the function to translate the value of parameters between ASUS WRT and RT-65168.
function TranslateWRTtoMTK(){
	var auth_mode = document.form.wl_auth_mode_x.value;
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

function doSave(){
	var auth_mode = document.form.wl_auth_mode_x.value;	
	if(validForm()){
		document.form.wps_config_state.value = "1";
		if((auth_mode == "shared" || auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "wpawpa2" || auth_mode == "radius" ||
		((auth_mode == "open") && !(document.form.wl_wep_x.value == "0")))
		&& document.form.wps_mode.value == "enabled")
			document.form.wps_mode.value = "disabled";
		if(auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "wpawpa2" || auth_mode == "radius"){
			document.form.next_page.value = "/cgi-bin/Advanced_WSecurity_Content.asp";
		}
		else{
			document.form.next_page.value = "/cgi-bin/index2.asp";
		}

		TranslateWRTtoMTK();

		document.form.action = "/cgi-bin/device-map/router.asp";
		document.form.editFlag.value = "1" ;
		if(model_name == "DSL-N66U")
		{
			parent.showLoading(28);
			setTimeout("redirectParent();", 28000);
		}
		else
		{
			parent.showLoading(23);
			setTimeout("redirectParent();", 23000);
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
	if(document.form.wl_wep_x.value != "0")
		if(!validate_wlphrase('WLANConfig11b', 'wl_phrase_x', document.form.wl_phrase_x))
			return false;
	if(auth_mode == "psk" || auth_mode == "psk2" || auth_mode == "pskpsk2"){ //2008.08.04 lock modified
		if(!validate_psk(document.form.wl_wpa_psk))
			return false;
		//confirm common string combination
                var is_common_string = check_common_string(document.form.wl_wpa_psk.value, "wpa_key");
                if(is_common_string){
                        if(!confirm("<% tcWebApi_Get("String_Entry", "JS_common_passwd","s") %>")){
                                document.form.wl_wpa_psk.focus();
                                document.form.wl_wpa_psk.select();
                                return false;   
                        }       
                }
	}
	else if(auth_mode == "wpa" || auth_mode == "wpa2" || auth_mode == "wpawpa2"){

	}
	else{
		var cur_wep_key = eval('document.form.wl_key'+document.form.wl_key.value);
		if(auth_mode != "radius" && !validate_wlkey(cur_wep_key))
			return false;
	}
	return true;
}

function change_key_des(){
var objs = getElementsByName_iefix("span", "key_des");
var wep_type = document.form.wl_wep_x.value;
var str = "";
if(wep_type == "1")
		str = " (<%tcWebApi_get("String_Entry","WC11b_WEPKey_itemtype1","s")%>)";
else if(wep_type == "2")
		str = " (<%tcWebApi_get("String_Entry","WC11b_WEPKey_itemtype2","s")%>)";
		
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

function check_state(){
	if(document.form.wl_auth_mode_x.value == "open")
	{
		showhide("div_wpaencrypt", 0);
		showhide("div_wpapreshared", 0);
		if(document.form.wl_wep_x.value != "0")
		{
			showhide("div_wlkey", 1);
			showhide("div_wepencrypt", 1);
			showhide("div_passphrase", 1);
		}
		else //WEP None
		{
			showhide("div_wlkey", 0);
			showhide("div_wepencrypt", 1);
			showhide("div_passphrase", 0);
		}
	}
	else if(document.form.wl_auth_mode_x.value == "shared")
	{
		showhide("div_wpaencrypt", 0);
		showhide("div_wpapreshared", 0);
		showhide("div_wlkey", 1);
		showhide("div_wepencrypt", 1);
		showhide("div_passphrase", 1);
	}
	else
	{
		showhide("div_wlkey", 0);
		showhide("div_wepencrypt", 0);
		showhide("div_passphrase", 0);
		if(document.form.wl_auth_mode_x.value == "radius")
		{
			showhide("div_wpaencrypt", 0);
			showhide("div_wpapreshared", 0);
 		}
 		else if(document.form.wl_auth_mode_x.value == "wpa" || document.form.wl_auth_mode_x.value == "wpa2" || document.form.wl_auth_mode_x.value == "wpawpa2")
		{
			showhide("div_wpaencrypt", 1);
			showhide("div_wpapreshared", 0);
		}
		else
		{
			showhide("div_wpaencrypt", 1);
			showhide("div_wpapreshared", 1);
		}
	}
}

function doLoad(){
	limit_auth_method();
	wl_auth_mode_change(1);
	check_state();
	automode_hint();

	document.getElementById("MAC").innerHTML = document.getElementById("MAC").innerHTML.toString().toUpperCase();
	document.getElementById("BandMAC").innerHTML = document.getElementById("BandMAC").innerHTML.toString().toUpperCase();

	if(wl_info.band5g_support){
		document.getElementById("t0").style.display = "";
		document.getElementById("t1").style.display = "";
	}
	else //DSL-N16U ..
	{
		document.getElementById("t0").style.display = "";
		document.getElementById("t1").style.display = "none";
	}
	document.getElementById("t0").className = <% tcWebApi_get("WLan_Common","wl_unit","s"); %> ? "tab_NW" : "tabclick_NW";
	document.getElementById("t1").className = <% tcWebApi_get("WLan_Common","wl_unit","s"); %> ? "tabclick_NW" : "tab_NW";
}

</script>
</head>
<body class="statusbody" onLoad="doLoad()">
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form_band" action="/cgi-bin/device-map/router.asp" target="hidden_frame">
<input type="hidden" name="wl_unit">
<input type="hidden" name="editFlag" value="0">
</form>
<FORM METHOD="POST" ACTION="/cgi-bin/device-map/router.asp" name="form" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="wan_route_x" value="">
<input type="hidden" name="wan_nat_x" value="1">
<input type="hidden" name="current_page" value="device-map/router.asp">
<input type="hidden" name="next_page" value="">
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
<input type="hidden" name="wlc_ure_ssid_org" value="" disabled>
<input type="hidden" name="wl_wpa_mode" value="0">
<input type="hidden" name="x_RegulatoryDomain" value="" readonly="1">
<input type="hidden" name="wl_gmode_protection" value='<% tcWebApi_get("WLan_Common","BGProtection","s"); %>'>
<input type="hidden" name="wl_wme" value="on">
<input type="hidden" name="wl_mode_x" value="0">
<input type="hidden" name="wl_nmode" value="">
<input type="hidden" name="wl_key_type" value='0'>
<input type="hidden" name="wl_wep_x_orig" value='0'>
<input type="hidden" name="wl_subunit" value='-1'>
<input type="hidden" name="editFlag" value="0">
<input type="hidden" name="wl_auth_mode_save" value="OPEN">
<input type="hidden" name="wl_crypto_save" value="NONE">
<input type="hidden" name="wl_unit" value='<% tcWebApi_get("WLan_Common","wl_unit","s"); %>'>
<input type="hidden" name="wl_nmode_x" value="<%tcWebApi_get("WLan_Common","WirelessMode","s")%>">
<input type="hidden" name="wl_closed" value="<%tcWebApi_get("WLan_Entry","HideSSID","s")%>">
<input type="hidden" name="wl_channel" value="<%tcWebApi_get("WLan_Common","Channel","s")%>">
<input type="hidden" name="wl_bw" value="<%tcWebApi_get("WLan_Common","HT_BW","s")%>">
<input type="hidden" name="wl_nctrlsb" value="<%tcWebApi_get("WLan_Common","HT_EXTCHA","s")%>">
<input type="hidden" name="w_Setting" value="1">
<table border="0" cellpadding="0" cellspacing="0">
<tr>
	<td>
	<table width="100px" border="0" align="left" style="margin-left:8px;" cellpadding="0" cellspacing="0">
	<td>
		<div id="t0" class="tabclick_NW" align="center" style="display:none; margin-right:2px; width:90px;" onclick="tabclickhandler(0)">
			<span id="span1" style="cursor:pointer;">2.4GHz</span>
		</div>
	</td>
	<td>
		<div id="t1" class="tab_NW" align="center" style="display:none; margin-right:2px; width:90px;" onclick="tabclickhandler(1)">
			<span id="span1" style="cursor:pointer;">5GHz</span>
		</div>
	</td>
	<td>
		<div id="t2" class="tab_NW" align="center" style="font-weight: bolder; margin-right:2px; width:90px;" onclick="tabclickhandler(2)">
			<span id="span1" style="cursor:pointer;font-weight: bolder;">Status</span>
		</div>
	</td>
	</table>
	</td>
</tr>
<tr>
<td>
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="WLnetworkmap">
<tr>
<td style="padding:5px 10px 0px 10px; ">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","Wireless_name","s")%>(SSID)</p>
<input style="*margin-top:-7px; width:260px;" type="text" maxlength="32" class="input_20_table" id="wl_ssid" name="wl_ssid" value="<% If tcWebApi_get("WLan_Entry","ssid","h") <> "" then  tcWebApi_get("WLan_Entry","ssid","s") else asp_Write("ASUS") end if %>" onkeypress="return validator.isString(this, event)">
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 0px 10px; *padding:1px 10px 0px 10px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","WC11b_AuthenticationMethod_in","s")%></p>
	<select name="wl_auth_mode_x" class="input_option" onChange="authentication_method_change(this);" onClick="check_state();">
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
<img style="display:none;margin-top:-30px;margin-left:185px;cursor:pointer;" id="wl_nmode_x_hint" src="/images/alertImg.png" width="30px" onClick="parent.overlib(parent.helpcontent[0][24], parent.FIXX, 870, parent.FIXY, 350);" onMouseOut="parent.nd();">
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
<!-- start -->
<div id="div_wpaencrypt">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="WLnetworkmap">
<tr>
<td style="padding:5px 10px 0px 10px; ">
<p class="formfonttitle_nwm" >WPA Encryption</p>
	<select name="wl_crypto" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_crypto')">
		<option value="AES" <% if tcWebApi_get("WLan_Entry","crypto","h") = "AES" then asp_Write("selected") end if %>>AES</option>
		<option value="TKIP+AES" <% if tcWebApi_get("WLan_Entry","crypto","h") = "TKIP+AES" then asp_Write("selected") end if %>>TKIP+AES</option>
	</select>
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
</div>
<div id="div_wpapreshared">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="WLnetworkmap">
<tr>
<td style="padding:5px 10px 0px 10px; *padding:1px 10px 0px 10px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","WPA-PSKKey","s")%></p>
	<input type="password" name="wl_wpa_psk" maxlength="65" class="input_20_table" value="<% If tcWebApi_get("WLan_Entry","wpa_psk","h") <> "" then tcWebApi_get("WLan_Entry","wpa_psk","s") end if %>" onBlur="switchType(this, false);" onFocus="switchType(this, true);">
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
</div>
<div id="div_wepencrypt">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="WLnetworkmap">
<tr>
<td style="padding:5px 10px 0px 10px; ">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","WC11b_WEPType_in","s")%></p>
	<select name="wl_wep_x" class="input_option" onChange="wep_encryption_change(this);" onClick="check_state();"><!-- return change_common(this, 'WLANConfig11b', 'wl_wep_x'); -->
		<option value="0" <% if tcWebApi_get("WLan_Entry","wep_x","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","wl_securitylevel_0","s")%></option>
		<option value="1" <% if tcWebApi_get("WLan_Entry","wep_x","h") = "1" then asp_Write("selected") end if %>>WEP-64bits</option>
		<option value="2" <% if tcWebApi_get("WLan_Entry","wep_x","h") = "2" then asp_Write("selected") end if %>>WEP-128bits</option>
	</select><br>
	<span name="key_des"></span>
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
</div>
<div id="div_wlkey">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="WLnetworkmap">
<tr>
<td style="padding:5px 10px 0px 10px; *padding:1px 10px 0px 10px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","WC11b_WEPDefaultKey_in","s")%></p>
	<select name="wl_key" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_key');">
		<option value="1" <% if tcWebApi_get("WLan_Entry","key","h") = "1" then asp_Write("selected") end if %>>1</option>
		<option value="2" <% if tcWebApi_get("WLan_Entry","key","h") = "2" then asp_Write("selected") end if %>>2</option>
		<option value="3" <% if tcWebApi_get("WLan_Entry","key","h") = "3" then asp_Write("selected") end if %>>3</option>
		<option value="4" <% if tcWebApi_get("WLan_Entry","key","h") = "4" then asp_Write("selected") end if %>>4</option>
	</select><br><br>
	Key 1:<br>
	<input type="text" name="wl_key1" id="wl_key1" maxlength="32" class="input_20_table" value="<% If tcWebApi_get("WLan_Entry","key1","h") <> "" then  tcWebApi_get("WLan_Entry","key1","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"><br>
	Key 2:<br>
	<input type="text" name="wl_key2" id="wl_key2" maxlength="32" class="input_20_table" value="<% If tcWebApi_get("WLan_Entry","key2","h") <> "" then  tcWebApi_get("WLan_Entry","key2","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"><br>
	Key 3:<br>
	<input type="text" name="wl_key3" id="wl_key3" maxlength="32" class="input_20_table" value="<% If tcWebApi_get("WLan_Entry","key3","h") <> "" then  tcWebApi_get("WLan_Entry","key3","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"><br>
	Key 4:<br>
	<input type="text" name="wl_key4" id="wl_key4" maxlength="32" class="input_20_table" value="<% If tcWebApi_get("WLan_Entry","key4","h") <> "" then  tcWebApi_get("WLan_Entry","key4","s") end if %>" onKeyUp="return change_wlkey(this, 'WLANConfig11b');"><br>
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
</div>
<div id="div_passphrase">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="WLnetworkmap">
<tr>
<td style="padding:5px 10px 0px 10px; ">
<p class="formfonttitle_nwm" >ASUS Passphrase</p>
	<input type="text" name="wl_phrase_x" maxlength="64" class="input_20_table" value="<% If tcWebApi_get("WLan_Entry","phrase_x","h") <> "" then  tcWebApi_get("WLan_Entry","phrase_x","s") end if %>" onKeyUp="return is_wlphrase('WLANConfig11b', 'wl_phrase_x', this);">
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
</div>
<div id="div_keyrotate" style="display:none;">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="WLnetworkmap">
<tr>
<td style="padding:5px 10px 0px 10px; *padding:1px 10px 0px 10px;">
<p class="formfonttitle_nwm" ><% tcWebApi_Get("String_Entry", "WC11b_x_Rekey_in", "s") %></p>
	<input type="text" maxlength="7" name="wl_wpa_gtk_rekey" class="input_6_table" value="<% If tcWebApi_get("WLan_Entry","RekeyInterval","h") <> "" then  tcWebApi_get("WLan_Entry","RekeyInterval","s") else asp_Write("3600") end if %>" onKeyPress="return validator.isNumber(this,event);" onblur="validate_number_range(this, 0, 259200)">
<img style="margin-top:5px; *margin-top:-10px;"src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
</div>
<!-- end -->
</td>
</tr>
<tr>
<td>
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
<tr id="apply_tr">
<td style="border-bottom:3px #15191b solid;padding:0px 10px 5px 10px;">
	<INPUT TYPE="SUBMIT" class="button_gen" NAME="BUTTON" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>" onClick="return doSave();" style="margin-left:90px;">
	<INPUT TYPE="HIDDEN" NAME="CountryChange" VALUE="0">
</td>
</tr>
<tr>
<td style="padding:5px 10px 0px 10px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","LAN_IP","s")%></p>
<p style="padding-left:10px; margin-top:3px; *margin-top:-5px; margin-right:10px; background-color:#444f53; line-height:20px;" id="LANIP">
<%If tcWebApi_staticGet("Info_Ether","ip","h") <> "" Then tcWebApi_staticGet("Info_Ether","ip","s") end if%>
</p>
<img style="margin-top:5px; *margin-top:-10px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 0px 10px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","PIN_code","s")%></p>
<p style="padding-left:10px; margin-top:3px; *margin-top:-5px; margin-right:10px; background-color:#444f53; line-height:20px;" id="PINCode">
<% tcWebApi_get("WLan_Entry0", "WscVendorPinCode", "s") %>
</p>
<img style="margin-top:5px; *margin-top:-10px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 0px 10px;">
<p class="formfonttitle_nwm" >LAN <%tcWebApi_get("String_Entry","MAC_Address","s")%></p>
<p style="padding-left:10px; margin-top:3px; *margin-top:-5px; padding-bottom:3px; margin-right:10px; background-color:#444f53; line-height:20px;" id="MAC">
<%If tcWebApi_get("Info_Ether","mac","h") <> "" Then tcWebApi_staticGet("Info_Ether","mac","s") end if%>
</p>
<img style="margin-top:5px; *margin-top:-10px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr id="macaddr_wl2">
<td style="padding:5px 10px 0px 10px;">
<p class="formfonttitle_nwm" >
<script>
	if("<% tcWebApi_get("WLan_Common","wl_unit","s"); %>" == "1")
		document.write("<% tcWebApi_Get("String_Entry", "menu5_1", "s") %> 5GHz <%tcWebApi_get("String_Entry","MAC_Address","s")%>");
	else
	{
		if(wl_info.band5g_support)
			document.write("<% tcWebApi_Get("String_Entry", "menu5_1", "s") %> 2.4GHz <%tcWebApi_get("String_Entry","MAC_Address","s")%>");
		else //DSL-N16U ..
			document.write("<% tcWebApi_Get("String_Entry", "menu5_1", "s") %> <%tcWebApi_get("String_Entry","MAC_Address","s")%>");
	}
</script>
</p>
<p style="padding-left:10px; margin-top:3px; *margin-top:-5px; margin-right:10px; background-color:#444f53; line-height:20px;" id="BandMAC">
<script>
	if("<% tcWebApi_get("WLan_Common","wl_unit","s"); %>" == "1")
		document.write("<% tcWebApi_get("WLan_Common", "wl1_MacAddress", "s") %>");
	else
		document.write("<% tcWebApi_get("WLan_Common", "wl0_MacAddress", "s") %>");
</script>
</p>
<img style="margin-top:5px; *margin-top:-10px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
</table>
</td>
</tr>
</table>
</form>
</body>

<!--device-map/router.asp-->
</html>

