<%
If Request_Form("SaveFirewall") = "1" Then
	TCWebApi_set("Firewall_Entry","fw_enable_x","fw_enable_x")
	TCWebApi_set("Firewall_Entry","fw_dos_x","fw_dos_x")
	TCWebApi_set("Firewall_Entry","fw_log_x","fw_log_x")
	TCWebApi_set("Firewall_Entry","misc_ping_x","misc_ping_x")

	tcWebApi_commit("Firewall_Entry")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_BasicFirewall_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_1_1","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" language="JavaScript" src="/help.js"></script>
<script type="text/javascript" language="JavaScript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
function initial(){
show_menu();
setTimeout("update_FAQ();", 300);
load_body();
change_firewall('<% tcWebApi_get("Firewall_Entry","fw_enable_x","s") %>');
/* Ren: move to Administration page(Advanced_System_Content.asp)
hideport(document.form.misc_http_x.value);
if(HTTPS_support == -1)
$("https_port").style.display = "none";
$("accessfromwan_port").style.display = (document.form.misc_http_x[0].checked == 1) ? "" : "none";
*/
}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){
		faqURL("faq", "https://www.asus.com", "/support/FAQ/", "1031610");
	}
}

function applyRule(){
if(validForm()){
//inputRCtrl1(document.form.misc_http_x, 1); //Ren: move to Administration page
inputRCtrl1(document.form.misc_ping_x, 1);

document.form.SaveFirewall.value = 1;

document.form.action = "/cgi-bin/Advanced_BasicFirewall_Content.asp";
showLoading(5);
setTimeout("redirect();", 5000);
document.form.submit();
}
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_BasicFirewall_Content.asp";
}

function validForm(){
//if(!validate_range(document.form.misc_httpport_x, 1024, 65535)) //Ren: move to Administration page
//return false; //Ren: move to Administration page
return true;
}
function hideport(flag){
$("accessfromwan_port").style.display = (flag == 1) ? "" : "none";
}
function done_validating(action){
refreshpage();
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_BasicFirewall_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_firewall">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="SaveFirewall" value="0">
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
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_5","s")%> - <%tcWebApi_get("String_Entry","menu5_1_1","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","FC_display2_sd","s")%></div>
<div class="formfontdesc" style="margin-top:-10px;">
	<a id="faq" href="https://www.asus.com/support/FAQ/1031610/" target="_blank" style="font-family:Lucida Console;text-decoration:underline;">DoS Protection FAQ</a>       <!-- untranslated -->
</div>		

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
            	<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,6);"><%tcWebApi_get("String_Entry","FC_FirewallEnable_in","s")%></a></th>
<td>
<input type="radio" value="1" name="fw_enable_x" onClick="return change_common_radio(this, 'FirewallConfig', 'fw_enable_x', '1')" <% if tcWebApi_get("Firewall_Entry","fw_enable_x","h") = "1" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" value="0" name="fw_enable_x" onClick="return change_common_radio(this, 'FirewallConfig', 'fw_enable_x', '0')" <% if tcWebApi_get("Firewall_Entry","fw_enable_x","h") = "0" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,7);"><%tcWebApi_get("String_Entry","FC_DoSEnable_in","s")%></a></th>
<td>
<input type="radio" value="1" name="fw_dos_x" class="input" onClick="return change_common_radio(this, 'FirewallConfig', 'fw_dos_x', '1')" <% if tcWebApi_get("Firewall_Entry","fw_dos_x","h") = "1" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" value="0" name="fw_dos_x" class="input" onClick="return change_common_radio(this, 'FirewallConfig', 'fw_dos_x', '0')" <% if tcWebApi_get("Firewall_Entry","fw_dos_x","h") = "0" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
          		<th align="right"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,1);"><%tcWebApi_get("String_Entry","FC_WanLanLog_in","s")%></a></th>
<td>
<select name="fw_log_x" class="input_option" onchange="return change_common(this, 'FirewallConfig', 'fw_log_x')">
<option value="none" <% if tcWebApi_get("Firewall_Entry","fw_log_x","h") = "none" then asp_Write("selected") end if %> ><% tcWebApi_Get("String_Entry", "wl_securitylevel_0", "s") %></option>
<option value="drop" <% if tcWebApi_get("Firewall_Entry","fw_log_x","h") = "drop" then asp_Write("selected") end if %> >Dropped</option>
<option value="accept" <% if tcWebApi_get("Firewall_Entry","fw_log_x","h") = "accept" then asp_Write("selected") end if %> >Accepted</option>
<option value="both" <% if tcWebApi_get("Firewall_Entry","fw_log_x","h") = "both" then asp_Write("selected") end if %> >Both</option>
</select>
</td>
</tr>
<tr>
          		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,5);"><%tcWebApi_get("String_Entry","FC_x_WanPingEnable_in","s")%></a></th>
<td>
<input type="radio" value="1" name="misc_ping_x" class="input" onClick="return change_common_radio(this, 'FirewallConfig', 'misc_ping_x', '1')" <% if tcWebApi_get("Firewall_Entry","misc_ping_x","h") = "1" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" value="0" name="misc_ping_x" class="input" onClick="return change_common_radio(this, 'FirewallConfig', 'misc_ping_x', '0')" <% if tcWebApi_get("Firewall_Entry","misc_ping_x","h") = "0" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
</table>
<div class="apply_gen">
            	<input name="button" type="button" class="button_gen" onclick="applyRule();" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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

<!--Advanced_BasicFirewall_Content.asp-->
</html>

