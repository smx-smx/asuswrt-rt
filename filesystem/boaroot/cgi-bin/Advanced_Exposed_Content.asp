<%
If Request_Form("editFlag") = "1" then
	tcWebApi_set("Dmz_PVC","DMZ_IP","dmz_ip")
	tcWebApi_commit("Dmz_PVC")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_Exposed_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - DMZ</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
function applyRule(){
if(validForm()){
showLoading(5);
setTimeout("redirect();", 5000);
document.form.action = "/cgi-bin/Advanced_Exposed_Content.asp";
document.form.editFlag.value = "1" ;
document.form.submit();
}
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_Exposed_Content.asp";
}
function validForm(){
if(!validate_ipaddr(document.form.dmz_ip, 'dmz_ip'))
return false;
return true;
}
function done_validating(action){
refreshpage();
}
function initial(){
show_menu();
setTimeout("update_FAQ();", 300);
load_body();
}
function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq", "https://www.asus.com", "/support/FAQ/", "1011722");
	}
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_Exposed_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_script" value="restart_firewall">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="editFlag" value="0">
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
<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_3","s")%> - DMZ</div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><% tcWebApi_Get("String_Entry", "IPC_ExposedIP_sd", "s") %><br/>Special Applications: Some applications require special handler against NAT. These special handlers are disabled in default.</div>
<div class="formfontdesc" style="margin-top:-10px;">
<a id="faq" href="https://www.asus.com/US/support/FAQ/1011722" target="_blank" style="font-family:Lucida Console;text-decoration:underline;">DMZ FAQ</a>
</div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
<th><% tcWebApi_Get("String_Entry", "IPC_ExposedIP_in", "s") %></th>
<td>
<input type="text" maxlength="15" class="input_15_table" name="dmz_ip" value="<% If tcWebApi_get("Dmz_PVC","DMZ_IP","h") <> "" then  tcWebApi_get("Dmz_PVC","DMZ_IP","s") end if %>" onkeypress="return validator.isIPAddr(this, event)" onblur="valid_IP_form(this, 0)"/>
</td>
</tr>
<!-- Viz 2011.04tr>
<th width="30%" align="right"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,20);">Starcraft(Battle.Net)</a></th>
<td>
<input type="radio" value="1" name="sp_battle_ips" class="content_input_fd" onClick="return change_common_radio(this, 'IPConnection', 'sp_battle_ips', '1')" >Yes
<input type="radio" value="0" name="sp_battle_ips" class="content_input_fd" onClick="return change_common_radio(this, 'IPConnection', 'sp_battle_ips', '0')" >No
</td>
</tr-->
</table>
<div class="apply_gen">
<input name="button" type="button" class="button_gen" onclick="applyRule()" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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

<!--Advanced_Exposed_Content.asp-->
</html>

