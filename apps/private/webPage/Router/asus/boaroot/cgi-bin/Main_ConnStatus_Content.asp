<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","Connections","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">

<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
</head>

<body onload="show_menu();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>

<form method="post" name="form" action="apply.cgi" target="hidden_frame">
<input type="hidden" name="current_page" value="Main_ConnStatus_Content.asp">
<input type="hidden" name="next_page" value="Main_ConnStatus_Content.asp">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">

<table class="content" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="17">&nbsp;</td>
    <td valign="top" width="202">
		<div id="mainMenu"></div>
		<div id="subMenu"></div>
	</td>
    <td valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
		<!--===================================Beginning of Main Content===========================================-->
		<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
			<tr>
				<td valign="top" >          
					<table width="760px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3"  class="FormTitle" id="FormTitle">
						<tr bgcolor="#4D595D">
							<td valign="top">
								<div>&nbsp;</div>
								<div class="formfonttitle"><%tcWebApi_get("String_Entry","System_Log","s")%> - <%tcWebApi_get("String_Entry","System_act_connections","s")%></div>
								<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
								<div class="formfontdesc"><%tcWebApi_get("String_Entry","System_log_connections","s")%></div>
								<div style="margin-top:8px">
									<iframe id="status_iframe" src="/cgi-bin/status_conn.cgi" width="100%" marginwidth="5" #marginheight="0" scrolling="no" frameborder="0" align="center"></iframe>
								</div>
								<div class="apply_gen" style="margin-top:8px;">
									<input type="button" onClick="location.href=location.href" value="<%tcWebApi_get("String_Entry","CTL_refresh","s")%>" class="button_gen">
								</div>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		<!--===================================Ending of Main Content===========================================-->
    </td>
    <td width="10" align="center" valign="top">&nbsp;</td>
  </tr>
</table>
<div id="footer"></div>
</form>
</body>
</html>
 
