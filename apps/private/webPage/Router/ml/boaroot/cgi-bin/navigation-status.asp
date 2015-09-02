<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" type="text/css" href="/style.css">
</head><body>
<div align=center>
<table border="0" width="760" cellspacing="0" cellpadding="0">
	<tr><td width="160" class="header"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationStatusText","s")%></font></td><td width="600">
	<table width="600" height="40" border="0" cellpadding="0" cellspacing="0" class="nav-main">
	<tr align=center valign=middle>
	  
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit0","h") = "1" THEN%>
	        <td width="65" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation.asp" tppabs="navigation.html" onClick='javascript:top.main.location="/cgi-bin/home_wizard.asp"'><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusQuickText","s")%><br><%tcWebApi_get("String_Entry","NavigationStatusStartText","s")%></font></a></td>
	<%End if%>     
	   
	<%IF tcWebApi_get("dynDisp_Entry","MainMaskBit1","h") = "1" THEN %>
		<td width="85" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-basic.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "1")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/home_wan.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/home_lan.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/home_wireless.asp"'
		<% End if%>
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusInterfaceText","s")%><br><%tcWebApi_get("String_Entry","NavigationStatusSetupText","s")%></font></a></td>
	<%End if %>
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<%IF tcWebApi_get("dynDisp_Entry","MainMaskBit2","h") = "1" THEN %>     
		<td width="85" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-advanced.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/adv_firewall.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/adv_routing_table.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/adv_nat_top.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit3","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/adv_adsl.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit4","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/adv_qos.asp"'
		<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit5","h") = "1" THEN%>
		onClick='javascript:top.main.location="/cgi-bin/adv_portbinding.asp"'
		<% End if%>
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusAdvancedText","s")%><br><%tcWebApi_get("String_Entry","NavigationStatusSetupText","s")%></font></a></td>
	<%End if %> 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<%IF tcWebApi_get("dynDisp_Entry","MainMaskBit3","h") = "1" THEN %>
		<td width="110" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-access.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "3")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		<% If tcWebApi_get("WebCustom_Entry","isSNMPFunSupported","h") = "Yes" then %>
		onClick='javascript:top.main.location="/cgi-bin/access_snmp.asp"'
		<%Else %> 
		onClick='javascript:top.main.location="/cgi-bin/access_upnp.asp"'
		<%End if %> 
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
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusAccessText","s")%><br><%tcWebApi_get("String_Entry","NavigationStatusManagementText","s")%></font></a></td>
	<%End if %> 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<%IF tcWebApi_get("dynDisp_Entry","MainMaskBit4","h") = "1" THEN %>
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
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusMaintenanceText","s")%></font></a></td>
	<%End if%>
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit5","h") = "1" THEN%>
		<td width="65" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-voip.asp" 
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
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusVoIPText","s")%></font></a></td>
	<%End if%>  
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit6","h") = "1" THEN%>
	        <td width="60" class="nav-main-sel"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationStatusText","s")%></font></a></td>
	<%End if%>
	
	        <td width="40" bgcolor="#FFFFFF"></td>	 

	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit7","h") = "1" THEN%>
	        <td width="45" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-help.asp" onClick='javascript:top.main.location="/cgi-bin/help_index.asp" '><class="menu-main"><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusHelpText","s")%></font></a></td>
	<%End if%>
	</tr></table>

	<table width="600" height="5" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
	<tr>
		<td width="600" class="orange"></td></tr></table>
	<table width="600" height="22" border="0" cellpadding="0" cellspacing="0" class="nav-sub">
	<tr align=center valign=middle>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		<td width="105" class="orange"><a href="/cgi-bin/status_deviceinfo.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationStatusDeviceText","s")%></font></div></a></td>
		<td width="1" bgcolor="#FFFFFF">&nbsp;</td>
	<%End if%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
	<%if tcWebApi_get("WebCustom_Entry","isSyslogEnhance","h") = "Yes" then%>
		<td width="100" class="orange"><a href="/cgi-bin/status_log2.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationStatusSystemText","s")%></font></div></a></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
	<%else%>
		<td width="100" class="orange"><a href="/cgi-bin/status_log.cgi" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationStatusSystemText","s")%></font></div></a></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
	<%end if%>
	<%End if%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		<td width="90" class="orange"><a href="/cgi-bin/status_statistics.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationStatusStatisticsText","s")%></font></div></a></td>
	<%End if%>
	<%if tcwebApi_get("WebCustom_Entry","isCZOTESupported","h")="Yes" then%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		<td width="1" bgcolor="#FFFFFF">&nbsp;</td>
	<%End if%>
		<td width="90" class="orange"><a href="/cgi-bin/wizardPPP_OTE.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationStatusInforText","s")%></font></div></a></td>
	<%End if%>	
		<td width="1" class="orange">&nbsp;</td><td class="orange">&nbsp;</td>
        </tr></table></td></tr>
</table></div>
</body></html>
