<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" type="text/css" href="/style.css">
</head><body>
<div align=center>
<table border="0" width="760" cellspacing="0" cellpadding="0">
	<tr><td width="160" class="header"><font color="#FFFFFF">Advanced</font></td><td width="600">
	<table width="600" height="40" border="0" cellpadding="0" cellspacing="0" class="nav-main">
	<tr align=center valign=middle>
	  
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit0","h") = "1" THEN%>
	<td width="65" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation.asp" tppabs="navigation.html" onClick='javascript:top.main.location="home_wizard.asp"'><font color="#666666">Quick<br>Start</font></a></td>
	<%End if%>     
	  
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit1","h") = "1" THEN%>
		<td width="85" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-basic.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "1")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/home_wan.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/home_lan.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/home_wireless.asp"'
		<% End if%>
		><font color="#666666">Interface<br>Setup</font></a></td>
	<%End if %>

	<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit2","h") = "1" THEN%>
	        <td width="85" class="nav-main-sel"><font color="#FFFFFF">Advanced<br>Setup</font></a></td> 
	<% End if%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit3","h") = "1" THEN%>
		<td width="110" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-access.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "3")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		<% If tcWebApi_get("WebCustom_Entry","isSNMPFunSupported","h") = "Yes" then %>
		onClick='javascript:top.main.location="/cgi-bin/access_snmp.asp"'
		<% else %>
		onClick='javascript:top.main.location="/cgi-bin/access_upnp.asp"'
		<% End if%>
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/access_upnp.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/access_ddns.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit3","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/access_acl.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit4","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/access_ipfilter.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit5","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/access_cwmp.asp"'
                <% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit6","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/access_parental.asp"'
		<% End if%>
		><font color="#666666">Access<br>Management</font></a></td>
	<%End if %> 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit4","h") = "1" THEN%>
		<td width="110" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-maintenance.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/tools_admin.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/tools_time.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/tools_update.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit3","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/tools_system.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit4","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/tools_test.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit5","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/tools_save.asp"'
		<% End if%>
		><font color="#666666">Maintenance</font></a></td>
	<%End if%>

	<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit5","h") = "1" THEN%>
		<td width="60" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-voip.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "5")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/voip_basic.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/voip_media.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/voip_call_ctrl.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit3","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/voip_speed_dial.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit4","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/voip_advanced.asp"'
		<% End if%>
		><font color="#666666">VoIP</font></a></td>
	<%End if%>
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit6","h") = "1" THEN%>
	        <td width="60" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-status.asp" 
        	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
        	<%if tcwebApi_get("WebCustom_Entry","isCZOTESupported","h")="Yes" then%>        	   	
        	onClick='javascript:top.main.location="/cgi-bin/wizardPPP_OTE.asp"' 
        	<%else%>        	
        	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
        	onClick='javascript:top.main.location="/cgi-bin/status_deviceinfo.asp"'
        	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
        	onClick='javascript:top.main.location="/cgi-bin/status_log.cgi"'
        	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
        	onClick='javascript:top.main.location="/cgi-bin/status_statistics.asp"'
        	<% End if%>
        	<% End if%>
        	><font color="#666666">Status</font></a></td> 
	<% End if%>

	        <td width="40" bgcolor="#FFFFFF"></td>

	<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
	        <% IF tcWebApi_get("dynDisp_Entry","MainMaskBit7","h") = "1" THEN%>
	<td width="45" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-help.asp" onClick='javascript:top.main.location="/cgi-bin/help_index.asp"'><class="menu-main"><font color="#666666">Help</font></a></td>
	<% End if%>
	</tr></table>
	<table width="600" height="5" border="0" align=center cellpadding="0" cellspacing="0">
	<tr>
		<td width="600" class="orange"></td></tr>
	<table width="600" height="22" border="0" cellpadding="0" cellspacing="0" class="nav-sub">
	<tr align=center valign=middle>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
	        <td width="65" class="orange"><a href="/cgi-bin/adv_firewall.asp" target="main"><div align=center><font color="#FFFFFF">Firewall</font></div></a></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
	<% End if%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
	        <td width="65" class="orange"><a href="/cgi-bin/adv_routing_table.asp" target="main"><div align=center><font color="#FFFFFF">Routing</font></div></a></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
	<% End if%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
	        <td width="65" class="orange"><a href="/cgi-bin/adv_nat_top.asp" target="main"><div align=center><font color="#FFFFFF">NAT</font></div></a></td><td width="1"  bgcolor="#FFFFFF">&nbsp;</td>
	<% End if%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit3","h") = "1" THEN%>
	        <td width="65" class="orange"><a href="/cgi-bin/adv_adsl.asp" target="main"><div align=center><font color="#FFFFFF"><%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>x<%else%>A<%end if%>DSL</font></div></a></td><td width="1"  bgcolor="#FFFFFF">&nbsp;</td>
	<% End if%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit4","h") = "1" THEN%>
	        <td width="65" class="orange"><a href="/cgi-bin/adv_qos.asp" target="main"><div align=center><font color="#FFFFFF">QoS</font></div></a></td><td width="1"  bgcolor="#FFFFFF">&nbsp;</td>
	<% End if%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit5","h") = "1" THEN%>
	        <td width="70" class="orange"><a href="/cgi-bin/adv_portbinding.asp" target="main"><div align=center><font color="#FFFFFF">PortBinding</font></div></a></td><td width="1"  bgcolor="#FFFFFF">&nbsp;</td>
	<% End if%>
	<% if tcWebApi_get("Info_Wan", "TTNETGuiSupported", "h") = "Yes" then%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit6","h") = "1" THEN%>
	        <td width="1"  bgcolor="#FFFFFF">&nbsp;</td><td width="70" class="orange"><a href="/cgi-bin/adv_routepolicy.asp" target="main"><div align=center><font color="#FFFFFF">RoutePolicy</font></div></a></td>
	<% End if%>
	<% End if%>
	<% IF tcWebApi_get("GUI_Entry0","vlanTag","h") = "support" THEN%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit6","h") = "1" THEN%>
		<td width="70" class="orange"><a href="/cgi-bin/adv_pvidsetting.asp" target="main"><div align=center><font color="#FFFFFF">Pvid Setting</font></div></a></td>
	<% End if%>
	<% End if%>
	<%IF tcwebApi_get("GUI_Entry0","staticRoute6", "h")="support" then%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit7","h") = "1" THEN%>
		<td width="1"  bgcolor="#FFFFFF">&nbsp;</td>
	        <td width="75" class="orange"><a href="/cgi-bin/adv_ipv6_static_route.asp" target="main"><div align=center><font color="#FFFFFF">IPv6 Routing</font></div></a></td>
	<% End if%>
	<% End if%>	
	        <td width="1"   class="orange">&nbsp;</td><td class="orange">&nbsp;</td>
        </tr></table></td></tr>
</table></div>
</body></html>
