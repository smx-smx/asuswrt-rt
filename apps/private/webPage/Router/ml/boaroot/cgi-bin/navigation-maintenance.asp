<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" type="text/css" href="/style.css">
</head><body>
<div align=center>
<table border="0" width="760" cellspacing="0" cellpadding="0">
	<tr><td width="160" class="header"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainStatusText","s")%></font></td><td width="600">
	<table width="600" height="40" border="0" cellpadding="0" cellspacing="0" class="nav-main">
	<tr align=center valign=middle>
	  
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit0","h") = "1" THEN%>
	        <td width="65" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation.asp" tppabs="navigation.html" onClick='javascript:top.main.location="/cgi-bin/home_wizard.asp"'><font color="#666666"><%tcWebApi_get("String_Entry","NavigationMainQuickText","s")%><br><%tcWebApi_get("String_Entry","NavigationMainStartText","s")%></font></a></td>
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
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationMainInterfaceText","s")%><br><%tcWebApi_get("String_Entry","NavigationMainSetupText","s")%></font></a></td>
	<%End if %>
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit2","h") = "1" THEN%>
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
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationMainAdvancedText","s")%><br><%tcWebApi_get("String_Entry","NavigationMainSetupText","s")%></font></a></td>
	<%End if %> 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit3","h") = "1" THEN%>
		<td width="110" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-access.asp" 
		<% tcWebApi_set("dynDisp_Entry", "CurPage", "3")%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		<% If tcWebApi_get("WebCustom_Entry","isSNMPFunSupported","h") = "Yes" then %>
		onClick='javascript:top.main.location="/cgi-bin/access_snmp.asp"'
		<% else %>
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
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationMainAccessText","s")%><br><%tcWebApi_get("String_Entry","NavigationMainManagementText","s")%></font></a></td>
	<%End if %> 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit4","h") = "1" THEN%>
	        <td width="110" class="nav-main-sel"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainMaintenanceText","s")%></font></a></td>
	<% End if%>
	
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
		><font color="#666666"><%tcWebApi_get("String_Entry","NavigationMainVoIPText","s")%></font></a></td>
	<%End if%>
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
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
        	><font color="#666666"><%tcWebApi_get("String_Entry","NavigationStatusText","s")%></font></a></td> 
	<% End if%>

	<td width="40" bgcolor="#FFFFFF"></td>

	<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
	<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit7","h") = "1" THEN%>
	        <td width="45" bgcolor="#FFFFFF"><a href="/cgi-bin/navigation-help.asp" onClick='javascript:top.main.location="/cgi-bin/help_index.asp"'><class="menu-main"><font color="#666666"><%tcWebApi_get("String_Entry","NavigationMainHelpText","s")%></font></a></td>
	<% End if%>
	</tr></table>
	<table width="600" height="5" border="0" align=center cellpadding="0" cellspacing="0">
	<tr>
		<td width="600" class="orange"></td></tr></table>
	<table width="600" height="22" border="0" cellpadding="0" cellspacing="0" class="nav-sub">
	<tr align=center valign=middle>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
		<td width="110" class="orange"><a href="/cgi-bin/tools_admin.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainAdminText","s")%></font>         </div></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
		<% End if%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
		<td width="90" class="orange"><a href="/cgi-bin/tools_time.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainTimeZoneText","s")%></font></div></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
		<% End if%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
		<td width="90" class="orange"><a href="/cgi-bin/tools_update.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainFirmwareText","s")%></font></div></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
		<% End if%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit3","h") = "1" THEN%>
		<td width="110" class="orange"><a href="/cgi-bin/tools_system.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainSysRestartText","s")%></font></div></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
		<% End if%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit4","h") = "1" THEN%>
		<td width="90" class="orange"><a href="/cgi-bin/tools_test.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainDiagnosticsText","s")%></font></div></td><td width="1" bgcolor="#FFFFFF">&nbsp;</td>
		<% End if%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit5","h") = "1" THEN%>
		<td width="90" class="orange"><a href="/cgi-bin/tools_save.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainSaveText","s")%></font></div></td>
		<% End if%>
		<% IF tcWebApi_get("GUI_Entry0","landingPage","h") = "support" THEN%>
		<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit6","h") = "1" THEN%>
		<td width="1" bgcolor="#FFFFFF">&nbsp;</td><td width="90" class="orange"><a href="/cgi-bin/tools_landingPage.asp" target="main"><div align=center><font color="#FFFFFF"><%tcWebApi_get("String_Entry","NavigationMainlandingPageText","s")%></font></div></td>
		<% End if%>
		<% End if%>
		<td width="1" class="orange">&nbsp;</td><td class="orange">&nbsp;</td>
        </tr></table></td></tr>
</table></div>
</body></html>
