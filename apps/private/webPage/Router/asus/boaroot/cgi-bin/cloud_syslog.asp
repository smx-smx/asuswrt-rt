<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - AiCloud Log</title>
<link rel="stylesheet" type="text/css" href="/index_style.css"> 
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
<% login_state_hook(); %>

//var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]
//wan_route_x = '<% nvram_get("wan_route_x"); %>';
//wan_nat_x = '<% nvram_get("wan_nat_x"); %>';
//wan_proto = '<% nvram_get("wan_proto"); %>';

function initial(){
	show_menu();

	if(!smart_sync_support){
		document.getElementById("ssLink").style.display = "none";
	}
	if(rrsut_support == -1)
                document.getElementById("rrsLink").style.display = "none";
}
</script>
</head>
<body onload="initial();" onunload="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>
<form method="post" name="form" action="cloud_apply.asp" target="hidden_frame">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% tcWebApi_get("SysInfo_Entry", "preferred_lang", "s") %>">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="current_page" value="cloud_settings.asp">
<input type="hidden" name="next_page" value="cloud_settings.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_webdav">
<input type="hidden" name="action_wait" value="3">
<table border="0" align="center" cellpadding="0" cellspacing="0" class="content">
	<tr>
		<td valign="top" width="17">&nbsp;</td>
		<!--=====Beginning of Main Menu=====-->
		<td valign="top" width="202">
			<div id="mainMenu"></div>
			<div id="subMenu"></div>
		</td>
		<td valign="top">
			<div id="tabMenu" class="submenuBlock">
				<table border="0" cellspacing="0" cellpadding="0">
					<tbody>
					<tr>
						<td>
							<a href="cloud_main.asp"><div class="tab"><span><%tcWebApi_Get("String_Entry", "AiCloud_Title","s")%></span></div></a>
						</td>
						<td>
							<a id="ssLink" href="cloud_sync.asp"><div class="tab"><span><% tcWebApi_Get("String_Entry","smart_sync","s") %></span></div></a>
						</td>
						<td>
							<a id="rrsLink" href="cloud_router_sync.asp"><div class="tab"><span><% tcWebApi_Get("String_Entry","Server_Sync","s") %></span></div></a>
						</td>
						<td>
							<a href="cloud_settings.asp"><div class="tab"><span><% tcWebApi_Get("String_Entry","Settings","s") %></span></div></a>
						</td>
						<td>
							<div class="tabclick"><span><% tcWebApi_Get("String_Entry","Log","s") %></span></div>
						</td>
					</tr>
					</tbody>
				</table>
			</div>
<!--==============Beginning of hint content=============-->
			<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
			  <tr>
					<td align="left" valign="top">
					  <table width="100%" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
							<tbody>
							<tr>
							  <td bgcolor="#4D595D" valign="top">
									<div>&nbsp;</div>
									<div class="formfonttitle">AiCloud - <% tcWebApi_Get("String_Entry","Log","s") %></div>
									<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>

								  <div class="formfontdesc" style="font-style: italic;font-size: 14px;"><%tcWebApi_get("String_Entry","AiCloud_Log_desc","s")%></div>

									<table width="100%" style="border-collapse:collapse;">

									  <tr>
									    <td>
												<div style="margin-top:8px">
													<iframe src="/cgi-bin/cloud_log.cgi" width="100%" height="450" marginwidth="5" #marginheight="0" scrolling="no" frameborder="0" align="center"></iframe>
												</div>
											</td>
									  </tr>

									</table>

							  </td>
							</tr>				
							</tbody>	
				  	</table>			
					</td>
				</tr>
			</table>
		</td>
		<td width="20"></td>
	</tr>
</table>
<div id="footer"></div>
</form>

</body>
</html>
