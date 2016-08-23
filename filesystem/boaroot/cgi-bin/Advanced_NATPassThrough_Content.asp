<%
If Request_Form("SaveALGSwitch") = "1" Then
	TCWebApi_set("ALGSwitch_Entry","SIPSW","SIPSW")
	TCWebApi_set("ALGSwitch_Entry","H323SW","H323SW")

	tcWebApi_commit("ALGSwitch_Entry")
End If

If Request_Form("SaveFirewall") = "1" Then
	TCWebApi_set("Firewall_Entry","fw_pt_pptp","fw_pt_pptp")
	TCWebApi_set("Firewall_Entry","fw_pt_l2tp","fw_pt_l2tp")
	TCWebApi_set("Firewall_Entry","fw_pt_ipsec","fw_pt_ipsec")
	TCWebApi_set("Firewall_Entry","fw_pt_rtsp","fw_pt_rtsp")
	TCWebApi_set("Firewall_Entry","fw_pt_pppoerelay","fw_pt_pppoerelay")
	update_variables()

	tcWebApi_commit("Firewall_Entry")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_NATPassThrough_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - NAT Pass-Through</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
function initial(){
show_menu();
}
function applyRule(){

document.form.SaveFirewall.value = 1;
document.form.SaveALGSwitch.value = 1;

document.form.action = "/cgi-bin/Advanced_NATPassThrough_Content.asp";
showLoading(5);
setTimeout("redirect();", 5000);
document.form.submit();
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_NATPassThrough_Content.asp";
}
</script>
</head>
<body onload="initial();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<input type="hidden" name="current_page" value="Advanced_NATPassThrough_Content.asp">
<input type="hidden" name="next_page" value="Advanced_NATPassThrough_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_script" value="restart_pppoe_relay">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wl_ssid" value="IBIZA_2.4G">
<input type="hidden" name="SaveFirewall" value="0">
<input type="hidden" name="SaveALGSwitch" value="0">
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
<td valign="top" >
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
								  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_3","s")%> - <%tcWebApi_get("String_Entry","NAT_passthrough_in","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
								  <div class="formfontdesc"><%tcWebApi_get("String_Entry","NAT_passthrough_desc","s")%></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
											<th><%tcWebApi_get("String_Entry","NAT_PPTP_Passthrough","s")%></th>
<td>
<select name="fw_pt_pptp" class="input_option">
<option class="content_input_fd" value="0" <% if tcWebApi_get("Firewall_Entry","fw_pt_pptp","h") = "0" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
<option class="content_input_fd" value="1" <% if tcWebApi_get("Firewall_Entry","fw_pt_pptp","h") = "1" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
</select>
</td>
</tr>
<tr>
											<th><%tcWebApi_get("String_Entry","NAT_L2TP_Passthrough","s")%></th>
<td>
<select name="fw_pt_l2tp" class="input_option">
<option class="content_input_fd" value="0" <% if tcWebApi_get("Firewall_Entry","fw_pt_l2tp","h") = "0" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
<option class="content_input_fd" value="1" <% if tcWebApi_get("Firewall_Entry","fw_pt_l2tp","h") = "1" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
</select>
</td>
</tr>
<tr>
											<th><%tcWebApi_get("String_Entry","NAT_IPSec_Passthrough","s")%></th>
<td>
<select name="fw_pt_ipsec" class="input_option">
<option class="content_input_fd" value="0" <% if tcWebApi_get("Firewall_Entry","fw_pt_ipsec","h") = "0" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
<option class="content_input_fd" value="1" <% if tcWebApi_get("Firewall_Entry","fw_pt_ipsec","h") = "1" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
</select>
</td>
</tr>
<tr>
  	         					<th><%tcWebApi_get("String_Entry","NAT_RTSP_Passthrough","s")%></th>
<td>
<select name="fw_pt_rtsp" class="input_option">
<option class="content_input_fd" value="0" <% if tcWebApi_get("Firewall_Entry","fw_pt_rtsp","h") = "0" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
<option class="content_input_fd" value="1" <% if tcWebApi_get("Firewall_Entry","fw_pt_rtsp","h") = "1" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
</select>
</td>
</tr>

<tr>
  	         					<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,31);">H.323 Passthrough</a></th>
<td>
<select name="H323SW" class="input_option">
<option class="content_input_fd" value="on" <% if tcWebApi_get("ALGSwitch_Entry","H323SW","h") = "on" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
<option class="content_input_fd" value="off" <% if tcWebApi_get("ALGSwitch_Entry","H323SW","h") = "off" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
</select>
</td>
</tr>

<tr>
  	         					<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,30);">SIP Passthrough</a></th>
<td>
<select name="SIPSW" class="input_option">
<option class="content_input_fd" value="on" <% if tcWebApi_get("ALGSwitch_Entry","SIPSW","h") = "on" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
<option class="content_input_fd" value="off" <% if tcWebApi_get("ALGSwitch_Entry","SIPSW","h") = "off" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
</select>
</td>
</tr>

<tr>
  	         					<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,11);"><%tcWebApi_get("String_Entry","PPPC_x_PPPoERelay_in","s")%></a></th>
<td>
<select name="fw_pt_pppoerelay" class="input_option">
<option class="content_input_fd" value="0" <% if tcWebApi_get("Firewall_Entry","fw_pt_pppoerelay","h") = "0" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
<option class="content_input_fd" value="1" <% if tcWebApi_get("Firewall_Entry","fw_pt_pppoerelay","h") = "1" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
</select>
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
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
</form>
<div id="footer"></div>
</body>

<!--Advanced_NATPassThrough_Content.asp-->
</html>

