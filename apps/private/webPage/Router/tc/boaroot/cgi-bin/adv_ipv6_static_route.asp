<%
	if request_form("Rule_Index") <> "" Then
		tcWebApi_set("WebCurSet_Entry","ipv6_route_id","Rule_Index")
		
		if Request_form("Save_or_Delete") = "1" then
			tcWebApi_unset("Route6_Entry")
			tcWebApi_commit("Route6_Entry")
			tcWebApi_set("Route6_Entry","DST_IP","staticDestIP")
			tcWebApi_set("Route6_Entry","Prefix","DstIPv6Prefix")
			tcWebApi_set("Route6_Entry","Device","Route_PVC_Index")
			tcWebApi_commit("Route6_Entry")
		elseif request_form("Save_or_Delete") = "2" Then 
			tcWebApi_unset("Route6_Entry")
			tcWebApi_commit("Route6_Entry")
		End If
		tcWebApi_commit("WebCurSet_Entry")
	end if
%>
<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<LINK href="/style.css" type=text/css rel=stylesheet>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<SCRIPT language=javascript>
function doCheckInterface()
{
	var value;
	value = document.forms[0].Route_PVC_Index.selectedIndex;
	var devName = "PVC_"+value+"_dev";
	if(document.getElementById(devName).value == "0")
	{
		alert("invalid interface: PVC" + value);
		return false;
	}
	return true;
}
function doSubmit()
{

	if(inValidIPv6Addr(document.forms[0].staticDestIP.value)) 
	{
		return false;
	}
	
	if(inValidIPv6Prefix(document.forms[0].DstIPv6Prefix.value))
	{
		return false;
	}
	if(doCheckInterface() == false)
	{
		return false;
	}
	document.forms[0].Save_or_Delete.value = 1;
	document.forms[0].submit();	
}
function doDelete()
{
	document.forms[0].Save_or_Delete.value = 2;
	document.forms[0].submit();
}
</SCRIPT>

<META content="MSHTML 6.00.2800.1400" name=GENERATOR></HEAD>
<BODY onLoad="">
<FORM name="ipv6_route_table" action="/cgi-bin/adv_ipv6_static_route.asp" method="post">
<DIV align=center>
<input type="hidden" name="Save_or_Delete" value="0">
<input type="hidden" name="PVC_0_dev" id="PVC_0_dev" value="<% if tcWebApi_get("Wan_PVC0","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="PVC_1_dev" id="PVC_1_dev" value="<% if tcWebApi_get("Wan_PVC1","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="PVC_2_dev" id="PVC_2_dev" value="<% if tcWebApi_get("Wan_PVC2","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="PVC_3_dev" id="PVC_3_dev" value="<% if tcWebApi_get("Wan_PVC3","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="PVC_4_dev" id="PVC_4_dev" value="<% if tcWebApi_get("Wan_PVC4","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="PVC_5_dev" id="PVC_5_dev" value="<% if tcWebApi_get("Wan_PVC5","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="PVC_6_dev" id="PVC_6_dev" value="<% if tcWebApi_get("Wan_PVC6","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="PVC_7_dev" id="PVC_7_dev" value="<% if tcWebApi_get("Wan_PVC7","Active","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TD class=light-orange colSpan=5 height=5>&nbsp;</TD></TR>
  <TR>
    <TD class=title-main width=150 height=30><FONT color=#ffffff>Static 
      Route</FONT> </TD>
    <TD class=black width=10>&nbsp;</TD>
        <TD width=180> <hr noshade></TD>
    <TD width=10>
      <HR noShade>
    </TD>
    <TD width=410>
      <HR noShade>
    </TD></TR>
	<TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Rule Index</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Rule_Index" SIZE="1" onChange="document.forms[0].submit();">
    		<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "0" then asp_Write("selected") end if %> value="0">1
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "1" then asp_Write("selected") end if %> value="1">2
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "2" then asp_Write("selected") end if %> value="2">3
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "3" then asp_Write("selected") end if %> value="3">4
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "4" then asp_Write("selected") end if %> value="4">5
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "5" then asp_Write("selected") end if %> value="5">6
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "6" then asp_Write("selected") end if %> value="6">7
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "7" then asp_Write("selected") end if %> value="7">8
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "8" then asp_Write("selected") end if %> value="8">9
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "9" then asp_Write("selected") end if %> value="9">10
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "10" then asp_Write("selected") end if %> value="10">11
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "11" then asp_Write("selected") end if %> value="11">12
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "12" then asp_Write("selected") end if %> value="12">13
      		<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "13" then asp_Write("selected") end if %> value="13">14
			<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "14" then asp_Write("selected") end if %> value="14">15
    		<option <% if tcWebApi_get("WebCurSet_Entry","ipv6_route_id","h") = "15" then asp_Write("selected") end if %> value="15">16
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Destination IPv6 Address / Prefix</FONT> 
          </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT name="staticDestIP" maxLength=39 size=39 value="<%If TcWebApi_get("Route6_Entry","DST_IP","h") <> "N/A" then TcWebApi_get("Route6_Entry","DST_IP","s") else asp_Write("::1") end if %>" ><font size=+1>&nbsp;/&nbsp;</font>
	<INPUT TYPE="TEXT" NAME="DstIPv6Prefix" SIZE="3" MAXLENGTH="3" VALUE="<%if tcwebapi_get("Route6_Entry","Prefix","h")<> "N/A" then tcwebApi_get("Route6_Entry","Prefix","s") else asp_write("128") end if%>"></TD></TR>
 
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Gateway IP Address</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Route_PVC_Index" SIZE="1" >
    	<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC0" then asp_Write("selected") end if %>>PVC0
		<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC1" then asp_Write("selected") end if %>>PVC1
		<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC2" then asp_Write("selected") end if %>>PVC2
		<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC3" then asp_Write("selected") end if %>>PVC3
		<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC4" then asp_Write("selected") end if %>>PVC4
		<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC5" then asp_Write("selected") end if %>>PVC5
		<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC6" then asp_Write("selected") end if %>>PVC6
		<option <% if tcWebApi_get("Route6_Entry","Device","h") = "PVC7" then asp_Write("selected") end if %>>PVC7
    	</SELECT> 
    </TD></TR>
  
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD>&nbsp;</TD>
    <TD>&nbsp;</TD>
    <TD>&nbsp;</TD></TR></TBODY></TABLE>
	
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TR>
		<TD class=title-main width=150 height=30><FONT color=#ffffff>IPv6 Static Route Table Listing</FONT> </TD>
		<TD class=black width=10>&nbsp;</TD>
        <TD width=600> <hr noshade></TD>
	</TR>
	<TR>
		<TD class=light-orange width=150 >&nbsp;</TD>
		<TD class=light-orange width=10 >&nbsp;</TD>
		<TD>
		<TABLE cellSpacing=0 cellPadding=0 width=600 align=center bgColor=#ffffff border=1>
			<tr>
				<TD width=80 align="center" class="tabdata"><strong>Index</strong></TD>
				<TD width=400 align="center" class="tabdata"><strong>Destination IPv6 Address/Prefix</strong></TD>
				<TD width=120 align="center" class="tabdata"><strong>Gateway</strong></TD>
			</tr>
			<% tcWebApi_get("GUITemp_Entry0","ipv6RouteTRLine","s") %>
			<% tcWebApi_Get("GUITemp_Entry1","ipv6RouteTRLine","s") %>
			<% tcWebApi_Get("GUITemp_Entry2","ipv6RouteTRLine","s") %>
			<% tcWebApi_Get("GUITemp_Entry3","ipv6RouteTRLine","s") %>
			<% tcWebApi_Get("GUITemp_Entry4","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry5","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry6","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry7","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry8","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry9","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry10","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry11","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry12","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry13","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry14","ipv6RouteTRLine","s") %>

			<% tcWebApi_Get("GUITemp_Entry15","ipv6RouteTRLine","s") %>		

		</TABLE></TD>

	</TR>

	</TABLE>

<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 

border=0>

  <TBODY>

  

  <TR height=42>

    <TD class=black width=160>&nbsp;</TD>

    <TD class=orange width=100>&nbsp;</TD>

          <TD class=orange width=500> <input type="button" value="  ADD  " name="StaticAdd" onClick="doSubmit();">

            &nbsp;&nbsp; 

            <INPUT type="button" value="DELETE" name="StaticDelete" onClick="doDelete();">&nbsp;&nbsp;&nbsp; 

	<INPUT type="reset" value="CANCEL" name="StaticCancel" onClick="javascript:window.location='/cgi-bin/adv_ipv6_static_route.asp'">

    </TD></TR></TBODY>

  </TABLE>

</DIV>

</FORM></BODY></HTML>

