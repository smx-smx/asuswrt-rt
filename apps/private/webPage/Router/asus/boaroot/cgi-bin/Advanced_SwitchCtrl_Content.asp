<%
If Request_Form("hwnat_flag") = "1" Then
	TCWebApi_set("Misc_Entry","hwnat_enable","hwnat")
	tcWebApi_commit("Misc_Entry")
End If
If Request_Form("hwnat_flag") = "2" Then
	TCWebApi_set("Misc_Entry","hwnat_enable","hwnat")  
	TCWebApi_set("Misc_Entry","jumbo_frame_enable","jumbo_frame_enable")
	tcWebApi_commit("Misc_Entry")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_SwitchCtrl_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","Switch_itemname","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
var wireless = []; // [[MAC, associated, authorized], ...]
function initial(){
	show_menu();
	
	if(Jumbo_frame_support >= 0)	//Jumbo_frame_support
		inputCtrl(document.form.jumbo_frame_enable, 1);
	else
		inputCtrl(document.form.jumbo_frame_enable, 0);
			
}
function applyRule(){
	if(valid_form()){
				
		if(Jumbo_frame_support >= 0)
			document.form.hwnat_flag.value = "2";
		else
			document.form.hwnat_flag.value = "1";	
			
		showLoading(5);
		setTimeout("redirect();", 5000);
		document.form.submit();
	}
}
function redirect(){
	document.location.href = "/cgi-bin/Advanced_SwitchCtrl_Content.asp";
}
function valid_form(){
	return true;
}
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
<form method="post" name="form" id="ruleForm" action="Advanced_SwitchCtrl_Content.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_SwitchCtrl_Content.asp">
<input type="hidden" name="next_page" value="Advanced_SwitchCtrl_Content.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="20">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="hwnat_flag" value="0">
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
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_2","s")%> - <%tcWebApi_get("String_Entry","Switch_itemname","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","SwitchCtrl_desc","s")%></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
	<th><%tcWebApi_get("String_Entry","jumbo_frame","s")%></th>
<td>
	<select name="jumbo_frame_enable" class="input_option">
		<option class="content_input_fd" value="0" <% if tcWebApi_get("Misc_Entry","jumbo_frame_enable","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
		<option class="content_input_fd" value="1" <% if tcWebApi_get("Misc_Entry","jumbo_frame_enable","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
	</select>
</td>
</tr>
<tr>
      <th><%tcWebApi_get("String_Entry","NAT_Acceleration","s")%></th>
<td>
	<select name="hwnat" class="input_option">
		<option class="content_input_fd" value="0" <% if tcWebApi_get("Misc_Entry","hwnat_enable","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
		<option class="content_input_fd" value="1" <% if tcWebApi_get("Misc_Entry","hwnat_enable","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Auto","s")%></option>
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
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_SwitchCtrl_Content.asp-->
</html>

