<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_wireless.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link type="text/css" rel="stylesheet" href="/form_style.css">
<link type="text/css" rel="stylesheet" href="/qis/qis_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script>
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";

var wl0_ssid_orig = decodeURIComponent('<% tcWebApi_staticGet("WLan_Entry","wl0_ssid","s") %>');
var wl1_ssid_orig = decodeURIComponent('<% tcWebApi_staticGet("WLan_Entry","wl1_ssid","s") %>');
function QKWireless_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_wireless_webtitle","s")%>";
	parent.set_step("t3");

	if(!wl_info.band5g_support){
		$("wl_unit_field_1").style.display = "none";
		$("wl_unit_field_2").style.display = "none";
		$("wl_unit_field_3").style.display = "none";
	}
	document.form.wl0_ssid.focus();
}

function submitForm(){
	if(document.form.wl0_wpa_psk.value == "" && document.form.wl1_wpa_psk.value == ""){
		if(!confirm("<%tcWebApi_get("String_Entry","QIS_confirm_recommand_encryp","s")%>"))
		return false;
	}
	if(!validate_string_ssid(document.form.wl1_ssid))
		return false;
	if(!validate_string_ssid(document.form.wl0_ssid))
		return false;
	if(document.form.wl1_wpa_psk.value.length > 0){
		document.form.wl1_auth_mode_x.value = "WPA2PSK";
		if(!validate_psk(document.form.wl1_wpa_psk))
			return false;
		//confirm common string combination
                var is_common_string = check_common_string(document.form.wl1_wpa_psk.value, "wpa_key");
                if(is_common_string){
                        if(!confirm("<% tcWebApi_Get("String_Entry", "JS_common_passwd","s") %>")){
                                document.form.wl1_wpa_psk.focus();
                                document.form.wl1_wpa_psk.select();
                                return false;   
                        }       
                }
	}
	else
		document.form.wl1_auth_mode_x.value = "OPEN";

	if(document.form.wl0_wpa_psk.value.length > 0){
		document.form.wl0_auth_mode_x.value = "WPA2PSK";
		if(!validate_psk(document.form.wl0_wpa_psk))
			return false;
		//confirm common string combination
                var is_common_string = check_common_string(document.form.wl0_wpa_psk.value, "wpa_key");
                if(is_common_string){
                        if(!confirm("<% tcWebApi_Get("String_Entry", "JS_common_passwd","s") %>")){
                                document.form.wl0_wpa_psk.focus();
                                document.form.wl0_wpa_psk.select();
                                return false;   
                        }       
                }
	}
	else
		document.form.wl0_auth_mode_x.value = "OPEN";

	document.form.next_page.value = "QIS_finish.asp";
	document.form.submit();
}

function Sync_2ghz(band){
	if(band == 2){
		if(document.form.sync_with_2ghz.checked == true){			
			document.form.wl1_wpa_psk.value = document.form.wl0_wpa_psk.value;
			if(document.form.wl0_ssid.value != wl0_ssid_orig)
					document.form.wl1_ssid.value = document.form.wl0_ssid.value.substring(0,29) + "_5G";
		}
	}
	else
		document.form.sync_with_2ghz.checked = false;
}

</script>
</head>
<body onLoad="QKWireless_load_body();" onunload="">
<form method="post" name="form" action="QIS_process.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_wireless.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_finish.asp">
<input type="hidden" name="prev_page" value="/cgi-bin/qis/QIS_manual_setting.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="wl0_auth_mode_x" value="">
<input type="hidden" name="wl1_auth_mode_x" value="">
<input type="hidden" name="w_Setting" value="1">
<input type="hidden" name="WPSConfigured" value="2">
<div class="QISmain">
<div class="formfonttitle" style="padding:6 0 0 10;">
<div>
<table width="730px">
<tr>
<td align="left">
<span class="description_down"><%tcWebApi_get("String_Entry","QKS_wireless_webtitle","s")%></span>
</td>
<!--td align="right">
<img onclick="gotoIndex();" style="cursor:pointer;" align="right" title="Go to Home" src="/images/backtohome.png" onMouseOver="this.src='/images/backtohomeclick.png'" onMouseOut="this.src='/images/backtohome.png'">
</td-->
</tr>
</table>
</div>
<div style="margin:5px;"><img style="width: 720px; *width: 710px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
</div>
<div class="QISGeneralFont"><%tcWebApi_get("String_Entry","qis_wireless_setting","s")%></div>
<br/>
<table class="QISform_wireless" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
<tr>
<th width="180">2.4GHz - <%tcWebApi_get("String_Entry","Security","s")%> </th>
<td class="QISformtd">
</td>
</tr>
<tr>
<th width="180"><span onmouseout="return nd();" onclick="openHint(0, 22);" style="cursor:help;"><% tcWebApi_Get("String_Entry", "QIS_finish_wireless_item1", "s") %><img align="right" style="cursor:pointer;margin-top:-14px\9;" src="/images/New_ui/helpicon.png"></span></th>
<td class="QISformtd">
<input type="text" id="wl0_ssid" name="wl0_ssid"  tabindex="1" onkeyup="Sync_2ghz(2);" style="height:25px;" class="input_32_table" maxlength="32" value="<% tcWebApi_staticGet("WLan_Entry","wl0_ssid","s") %>" autocapitalization="off" autocomplete="off"/>
</td>
</tr>
<tr id="wl_unit_field_0">
<th width="180"><span onmouseout="return nd();" onclick="openHint(0, 23);" style="cursor:help;"><% tcWebApi_Get("String_Entry", "Network_key", "s") %><img align="right" style="cursor:pointer;margin-top:-14px\9;" src="/images/New_ui/helpicon.png"></span></th>
<td class="QISformtd">
<input type="password" name="wl0_wpa_psk" value="<% tcWebApi_staticGet("WLan_Entry","wl0_wpa_psk","s") %>"  tabindex="2" onBlur="switchType(this, false);" onFocus="switchType(this, true);" onkeyup="Sync_2ghz(2);" style="height:25px;" class="input_32_table" maxlength="64" autocapitalization="off" autocomplete="off">
</td>
</tr>
<tr id="wl_unit_field_1">
<th width="180">5GHz - <%tcWebApi_get("String_Entry","Security","s")%> </th>
<td class="QISformtd">
<span id="syncCheckbox"><input type="checkbox" name="sync_with_2ghz" class="input"  tabindex="3" onclick="setTimeout('Sync_2ghz(2);',0);"><% tcWebApi_Get("String_Entry", "qis_ssid_desc", "s") %></span>
</td>
</tr>
<tr id="wl_unit_field_2">
<th width="180"><span onmouseout="return nd();" onclick="openHint(0, 22);" style="cursor:help;"><% tcWebApi_Get("String_Entry", "QIS_finish_wireless_item1", "s") %><img align="right" style="cursor:pointer;margin-top:-14px\9;" src="/images/New_ui/helpicon.png"></span></th>
<td class="QISformtd">
<input type="text" id="wl1_ssid" name="wl1_ssid"  tabindex="4" onkeyup="Sync_2ghz(5);" style="height:25px;" class="input_32_table" maxlength="32" value="<% tcWebApi_staticGet("WLan_Entry","wl1_ssid","s") %>" autocapitalization="off" autocomplete="off"/>
</td>
</tr>
<tr id="wl_unit_field_3">
<th width="180"><span onmouseout="return nd();" onclick="openHint(0, 23);" style="cursor:help;"><% tcWebApi_Get("String_Entry", "Network_key", "s") %><img align="right" style="cursor:pointer;margin-top:-14px\9;" src="/images/New_ui/helpicon.png"></span></th>
<td class="QISformtd">
<input type="password" name="wl1_wpa_psk" value="<% tcWebApi_staticGet("WLan_Entry","wl1_wpa_psk","s") %>"  tabindex="5" onBlur="switchType(this, false);" onFocus="switchType(this, true);" onkeyup="Sync_2ghz(5);" style="height:25px;" class="input_32_table" maxlength="64" autocapitalization="off" autocomplete="off">
</td>
</tr>
</table>
<br/>
<div class="QISGeneralFont"><% tcWebApi_Get("String_Entry", "qis_wireless_setting_skdesc", "s") %></div>
<br/>
<div class="apply_gen" style="margin-top:10px">
<input type="button" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>"  tabindex="6" onclick="submitForm();" class="button_gen">
</div>
</form>
</body>

<!--qis/QIS_wireless.asp-->
</html>
