<%
tcWebApi_set("WLan_Common","editFlag","editFlag")
tcWebApi_set("WLan_Common","MBSSID_changeFlag","MBSSID_changeFlag")
tcWebApi_set("WLan_Common","MBSSID_able_Flag","MBSSID_able_Flag")
If Request_Form("editFlag") = "1" then
	tcWebApi_Set("SysInfo_Entry","w_Setting","w_Setting")
	tcWebApi_commit("SysInfo_Entry")

	tcWebApi_Set("WLan_Common","wl_unit","wl_unit")
	tcWebApi_Set("WLan_Entry","RADIUS_Server","wl_radius_ipaddr")
	tcWebApi_Set("WLan_Entry","RADIUS_Port","wl_radius_port")
	tcWebApi_Set("WLan_Entry","RADIUS_Key1","wl_radius_key")
	tcWebApi_commit("WLan_Entry")
end if

If Request_Form("editFlag") = "1" then
load_parameters_from_generic()
end if
load_parameters_to_generic()
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_WSecurity_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_1_5","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }


var wireless = []; // [[MAC, associated, authorized], ...]
function initial(){
	show_menu();

	if(!wl_info.band5g_support)
		$("wl_unit_field").style.display = "none";

	if(sw_mode == 2 && '<% tcWebApi_get("WLan_Common","wl_unit","s"); %>' == ''){
		for(var i=4; i>=2; i--)
		$("MainTable1").deleteRow(i);
		$("repeaterModeHint").style.display = "";
		$("submitBtn").style.display = "none";
	}
}
function applyRule(){
	if(validForm()){
		document.form.action = "/cgi-bin/Advanced_WSecurity_Content.asp";
		document.form.next_page.value = "/cgi-bin/Advanced_WSecurity_Content.asp";
		document.form.editFlag.value = "1" ;
		if(model_name == "DSL-N66U")
		{
			showLoading(28);
			setTimeout("redirect();", 28000);
		}
		else
		{
			showLoading(23);
			setTimeout("redirect();", 23000);
		}
		if(navigator.appName.indexOf("Microsoft") >= 0){ 		// Jieming added at 2013/05/21, to avoid browser freeze when submitting form on IE
			stopFlag = 1;
		}
		document.form.submit();
	}
}
function validForm(){
if(!validate_ipaddr(document.form.wl_radius_ipaddr, 'wl_radius_ipaddr'))
return false;
if(!validate_range(document.form.wl_radius_port, 0, 65535))
return false;
if(!validate_string(document.form.wl_radius_key))
return false;
return true;
}
function done_validating(action){
refreshpage();
}

function redirect(){
	document.location.href = document.form.next_page.value;
}

function _change_wl_unit(wl_unit){
	document.form_band.wl_unit.value = wl_unit;
	document.form_band.action = "/cgi-bin/Advanced_WSecurity_Content.asp";
	showLoading(2);
	setTimeout("redirect();", 2000);	
	document.form_band.submit();
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form_band" action="Advanced_WSecurity_Content.asp" target="hidden_frame">
<input type="hidden" name="wl_unit">
<input type="hidden" name="editFlag" value="0">
</form>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_WSecurity_Content.asp">
<input type="hidden" name="next_page" value="Advanced_WSecurity_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="restart_wireless">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="3">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wl_subunit" value="-1">
<input type="hidden" name="editFlag" value="0">
<input type="hidden" name="MBSSID_changeFlag" value="0">
<input type="hidden" name="MBSSID_able_Flag" value="0">
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
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_1","s")%> - <%tcWebApi_get("String_Entry","menu5_1_5","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc"><%tcWebApi_get("String_Entry","WA11a_display1_sd","s")%></div>
		  
<table id="MainTable1" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
	<tr id="wl_unit_field">
			<th><%tcWebApi_get("String_Entry","Interface","s")%></th>
		<td>
			<select name="wl_unit" class="input_option" onChange="_change_wl_unit(this.value);">
				<option class="content_input_fd" value="0" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "0" then asp_Write("selected") end if %>>2.4GHz</option>
				<option class="content_input_fd" value="1" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "1" then asp_Write("selected") end if %>>5GHz</option>
			</select>
		</td>
	</tr>
	<tr id="repeaterModeHint" style="display:none;">
			<td colspan="2" style="color:#FFCC00;height:30px;" align="center"><%tcWebApi_get("String_Entry","page_not_support_mode_hint","s")%></td>
	</tr>
	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(2,1);">
			  	<%tcWebApi_get("String_Entry","WA11a_ExAuthDBIPAddr_in","s")%></a>			  
		</th>
		<td>
			<input type="text" maxlength="15" class="input_15_table" name="wl_radius_ipaddr" value="<% If tcWebApi_get("WLan_Entry","RADIUS_Server","h") <> "" then  tcWebApi_get("WLan_Entry","RADIUS_Server","s") end if %>" onKeyPress="return validator.isIPAddr(this, event)" onblur="valid_IP_form(this, 0)">
		</td>
	</tr>
	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(2,2);">
			  	<%tcWebApi_get("String_Entry","WA11a_ExAuthDBPortNumber_in","s")%></a>
		</th>
		<td>
			<input type="text" maxlength="5" class="input_6_table" name="wl_radius_port" value="<% If tcWebApi_get("WLan_Entry","RADIUS_Port","h") <> "" then  tcWebApi_get("WLan_Entry","RADIUS_Port","s") end if %>" onkeypress="return validator.isNumber(this,event)" onblur="validate_number_range(this, 0, 65535)"/>
		</td>
	</tr>
	<tr>
		<th >
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(2,3);">
				<%tcWebApi_get("String_Entry","WA11a_ExAuthDBPassword_in","s")%></a>
		</th>
		<td>
			<input type="password" maxlength="64" class="input_32_table" name="wl_radius_key" autocapitalization="off" autocomplete="off" value="<% If tcWebApi_get("WLan_Entry","RADIUS_Key1","h") <> "" then  tcWebApi_get("WLan_Entry","RADIUS_Key1","s") end if %>">
		</td>
	</tr>
</table>
<div id="submitBtn" class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_WSecurity_Content.asp-->
</html>

