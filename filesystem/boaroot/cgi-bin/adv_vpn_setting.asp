<%
if Request_form("delnum") <> "" then
		tcWebApi_set("WebCurSet_Entry","vpn_id","delnum")
		tcWebApi_unset("VPN_Entry")
		tcWebApi_commit("VPN_Entry")
end if
%>

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
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script>
function initial(){
	show_menu();
}

function totalConnCheck()
{
	if(document.VpnTable_form.TotalConnNum.value >= 8)
	{
		alert("Users can only configure 8 VPN connections!\n");
		return false;
	}
	
	window.location='/cgi-bin/adv_vpn_connection.asp?add_num='+document.VpnTable_form.add_num.value
	return true;
}

function doDelete(i)
{
 	document.VpnTable_form.delnum.value=i;
	document.VpnTable_form.submit();
}

function doedit(i)
{
 	document.VpnTable_form.action="/cgi-bin/adv_vpn_connection.asp";
	document.VpnTable_form.editnum.value=i;
	document.VpnTable_form.submit();
}
</script>

</head>

<body onload="initial();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<FORM METHOD="POST" ACTION="/cgi-bin/adv_vpn_setting.asp" name="VpnTable_form">
<div align="center">
<INPUT TYPE="HIDDEN" NAME="delnum">
<INPUT TYPE="HIDDEN" NAME="editnum">
<INPUT TYPE="HIDDEN" NAME="TotalConnNum" value="<%TcWebApi_get("VPN","Total_Conn_Num","s")%>">
<INPUT TYPE="HIDDEN" NAME="add_num" value="<%TcWebApi_get("VPN","add_num","s")%>">
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
				<tr height="10px">
					<td bgcolor="#4D595D" valign="top">
						<div>&nbsp;</div>
						<div class="formfonttitle">IPSec VPN - Setting</div>
						<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
						<div class="formfontdesc">XXXXXXXXXX</div>
					</td>
				</tr>
				<tr>
					<td bgcolor="#4D595D" valign="top">
					<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
							<tr>
								<td colspan="7">VPN Connection List</td>
							</tr>
						</thead>
						<tr>
							<th style="width: 6%;">Unit</th>
							<th style="width: 20%;">Connection Name</th>
							<th style="width: 20%;">Remote Gateway</th>
							<th style="width: 20%;">Local Address</th>
							<th style="width: 20%;">Remote Address</th>
							<th style="width: 7%;">Edit</th>
							<th style="width: 7%;">Delete</th>
						</tr>
					<%if tcWebApi_get("GUITemp_Entry0","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry0","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry0","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry0","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry0","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry0","Drop","s") %></td>
						</tr>
					<%end if%>
					<%if tcWebApi_staticGet("GUITemp_Entry1","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry1","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry1","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry1","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry1","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry1","Drop","s") %></td>
						</tr>
					<%end if%>
					<%if tcWebApi_staticGet("GUITemp_Entry2","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry2","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry2","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry2","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry2","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry2","Drop","s") %></td>
						</tr>
					<%end if%>
					<%if tcWebApi_staticGet("GUITemp_Entry3","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry3","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry3","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry3","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry3","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry3","Drop","s") %></td>
							</tr>
					<%end if%>
					<%if tcWebApi_staticGet("GUITemp_Entry4","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry4","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry4","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry4","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry4","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry4","Drop","s") %></td>
						</tr>
					<%end if%>
					<%if tcWebApi_staticGet("GUITemp_Entry5","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry5","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry5","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry5","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry5","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry5","Drop","s") %></td>
						</tr>
					<%end if%>
					<%if tcWebApi_staticGet("GUITemp_Entry6","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry6","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry6","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry6","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry6","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry6","Drop","s") %></td>
						</tr>
					<%end if%>
					<%if tcWebApi_staticGet("GUITemp_Entry7","vpnTRLine","h") <> "" then%>
						<tr>
							<%tcWebApi_staticGet("GUITemp_Entry7","Index_Name","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry7","GW_Local","s")%>
							<%tcWebApi_staticGet("GUITemp_Entry7","Remote","s") %>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry7","Edit","s") %></td>
							<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry7","Drop","s") %></td>
						</tr>
					<%end if%>
					</table>
					<div class="apply_gen">
						<input name="Add New Connection" type="button" class="button_gen" value="Add" onClick="totalConnCheck()">
					</div>
					</td>
				</tr>
				</tbody>
				</table>
			</td>
			</tr>
		</table>
	</td>
</tr>
</table>
</div>
</form>
<div id="footer"></div>
</body>
</html>
