<%
if Request_form("delnum") <> "" then
		tcWebApi_set("WebCurSet_Entry","route_id","delnum")
		tcWebApi_unset("Route_Entry")
		tcWebApi_commit("Route_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<SCRIPT language=javascript>
function doUserDefNumCheck(){
	if(document.RoutingTable_form.user_def_num.value>=16){
		alert("Users can only define 16 routes!\n");
		return false;
	}
	window.location='/cgi-bin/adv_static_route.asp?add_num='+document.RoutingTable_form.add_num.value
	return true;
}
function doDelete(i)
 {
 	document.RoutingTable_form.delnum.value=i;
	document.RoutingTable_form.submit();
}
function doedit(i)
 {
 	document.RoutingTable_form.action="/cgi-bin/adv_static_route.asp";
	document.RoutingTable_form.editnum.value=i;
	document.RoutingTable_form.submit();
}
</SCRIPT>

</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/adv_routing_table.asp" name="RoutingTable_form">
<div align="center">
<input type="hidden" name="Route_num" value="<%TcWebApi_get("Route","Route_num","s")%>">
<INPUT TYPE="HIDDEN" NAME="delnum">
<INPUT TYPE="HIDDEN" NAME="editnum">
<INPUT TYPE="HIDDEN" NAME="user_def_num" value="<%TcWebApi_get("Route","User_def_num","s")%>">
<INPUT TYPE="HIDDEN" NAME="add_num" value="<%TcWebApi_get("Route","add_num","s")%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">Routing Table List</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td></tr>
        <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td colspan="3" align=center>
       	<table width="580" border="1" align="center" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
		<tr height="30"><td width="40" align=center class="tabdata"><strong>#</strong></td>
		<td width="110" align=center class="tabdata"><strong><font color="#000000">Dest IP</font></strong></td>
		<td width="100" align=center class="tabdata"><strong><font color="#000000">Mask</font></strong></td>
		<td width="110" align=center class="tabdata"><strong><font color="#000000">Gateway IP</font></strong></td>
		<td width="50"  align=center class="tabdata"><strong><font color="#000000">Metric</font></strong></td>
                <td width="70" align=center class="tabdata"><strong><font color="#000000">Device</font></strong></td>
		<td width="45" align=center class="tabdata"><strong><font color="#000000">Edit</font></strong></td>
		<td align=center class="tabdata"><strong><font color="#000000">Drop</font></strong></td>
                </tr>
	        <%if tcWebApi_get("GUITemp_Entry0","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry0","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry0","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry0","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry0","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry0","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry1","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry1","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry1","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry1","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry1","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry1","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry2","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry2","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry2","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry2","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry2","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry2","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry3","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry3","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry3","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry3","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry3","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry3","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry4","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry4","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry4","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry4","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry4","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry4","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry5","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry5","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry5","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry5","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry5","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry5","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry6","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry6","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry6","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry6","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry6","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry6","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry7","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry7","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry7","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry7","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry7","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry7","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry8","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry8","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry8","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry8","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry8","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry8","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry9","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry9","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry9","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry9","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry9","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry9","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry10","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry10","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry10","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry10","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry10","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry10","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry11","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry11","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry11","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry11","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry11","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry11","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry12","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry12","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry12","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry12","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry12","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry12","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry13","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry13","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry13","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry13","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry13","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry13","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry14","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry14","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry14","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry14","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry14","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry14","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry15","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry15","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry15","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry15","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry15","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry15","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry16","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry16","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry16","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry16","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry16","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry16","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry17","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry17","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry17","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry17","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry17","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry17","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry18","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry18","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry18","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry18","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry18","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry18","Drop","s") %></td>
			</tr>
		<%end if%>		
		<%if tcWebApi_staticGet("GUITemp_Entry19","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry19","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry19","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry19","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry19","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry19","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry20","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry20","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry20","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry20","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry20","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry20","Drop","s") %></td>
			</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry21","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry21","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry21","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry21","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry21","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry21","Drop","s") %></td>
			</tr>
		<%end if%>

		<%if tcWebApi_staticGet("GUITemp_Entry22","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry22","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry22","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry22","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry22","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry22","Drop","s") %></td>
			</tr>
		<%end if%>

		<%if tcWebApi_staticGet("GUITemp_Entry23","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry23","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry23","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry23","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry23","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry23","Drop","s") %></td>
			</tr>
		<%end if%>

		<%if tcWebApi_staticGet("GUITemp_Entry24","routeTRLine","h") <> "" then%>
		        <tr>
			<%tcWebApi_staticGet("GUITemp_Entry24","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry24","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry24","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry24","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry24","Drop","s") %></td>
			</tr>
		<%end if%>

		<%if tcWebApi_staticGet("GUITemp_Entry25","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry25","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry25","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry25","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry25","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry25","Drop","s") %></td>
			</tr>
		<%end if%>

		<%if tcWebApi_staticGet("GUITemp_Entry26","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry26","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry26","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry26","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry26","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry26","Drop","s") %></td>
			</tr>
		<%end if%>

		<%if tcWebApi_staticGet("GUITemp_Entry27","routeTRLine","h") <> "" then%>
			<tr>
			<%tcWebApi_staticGet("GUITemp_Entry27","Index_Des","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry27","Mask_Gate","s")%>
			<%tcWebApi_staticGet("GUITemp_Entry27","Metric_Use","s") %>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry27","Edit","s") %></td>
			<td align=center class=tabdata><%tcWebApi_staticGet("GUITemp_Entry27","Drop","s") %></td>
			</tr>
		<%end if%>
				</table>
				</td></tr>
        </div>
		<tr>
    <td class="light-orange"></td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr height="42">
    <td width="160" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<input name="AddRoute" type="button" value="ADD ROUTE" onClick="doUserDefNumCheck()">
  </tr></table>
		</form>
</body></html>
