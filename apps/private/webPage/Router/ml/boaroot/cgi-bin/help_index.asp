<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
</head>

<body>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
                            
  <tr>
    <td width="150" height="30" class="title-main">
    
    <%tcWebApi_get("String_Entry","HelpIndexQuickText","s")%> 
    
    </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="150">&nbsp;</td>
    <td width="10" >&nbsp;</td>
    <td width="440">&nbsp;</td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_quickstart.asp#quick_start" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexQuickText","s")%>
    
    </a></td>
  </tr>
         
  <tr>
    <td width="150" height="30" class="title-main">
    
    <%tcWebApi_get("String_Entry","HelpIndexInteSetupText","s")%> 
    
    </td>
    <td class="black">&nbsp;</td>
    <td><hr noshade></td>
    <td><hr noshade></td>
    <td><hr noshade></td>
  </tr>
                                            
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_interface.asp#Internet_Settings" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexInteSetText","s")%>
    
    </a></td>
  </tr>
                                		               
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="help_interface.asp#LAN_Settings" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexLANSetText","s")%>
    
    </a></td>
  </tr>
  
  <% if tcWebApi_get("Info_WLan","isExist","h") = "N/A" then%>
  <!--tr></tr-->
  <%else%>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="help_interface.asp#Wireless_LAN_Settings" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexWLANSetText","s")%>
    
    </a></td>
  </tr>
  <%end if%>
      
  <tr>
    <td width="150" height="30" class="title-main">
    
    <%tcWebApi_get("String_Entry","HelpIndexAdvSetupText","s")%>
    
    </td>
    <td class="black">&nbsp;</td>
    <td><hr noshade></td>
    <td><hr noshade></td>
    <td><hr noshade></td>
  </tr>
                                                                             
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_advanced.asp#Firewall" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexFirewallText","s")%> 
    
    </a></td>
  </tr>
                      
                                                                             
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_advanced.asp#Routing" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexRoutingText","s")%> 
    
    </a></td>
  </tr>
                    
                                                                                             
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_advanced.asp#NAT" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexNATText","s")%>
    
    </a></td>
  </tr>

                      
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_advanced.asp#QoS" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexQoSText","s")%>
    
    </a></td>
  </tr>
  

  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_advanced.asp#VLAN" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexVLANText","s")%>
    
    </a></td>
  </tr>
  

  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_advanced.asp#ADSL" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexADSLText","s")%>
    
    </a></td>
  </tr>
  
<!--  
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="help_advanced.htm" target=_blank>ADSL</a></td>
  </tr>
-->
                           
        
  <tr>
    <td width="150" height="30" class="title-main">
    
    <%tcWebApi_get("String_Entry","HelpIndexAccManageText","s")%>
    
    </td>
    <td class="black">&nbsp;</td>
    <td><hr noshade></td>
    <td><hr noshade></td>
    <td><hr noshade></td>
  </tr>
  
                                        
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_access.asp#ACL" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexACLText","s")%>
    
    </a></td>
  </tr>
  
                                                    
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_access.asp#IP_Filter" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexIPFilterText","s")%>
    
    </a></td>
  </tr>
    
                                                    
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_access.asp#SNMP" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexSNMPText","s")%>
    
    </a></td>
  </tr>
    
                                                                
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_access.asp#UPnP" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexUPnPText","s")%>
    
    </a></td>
  </tr>
      
                                                                            
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_access.asp#DDNS" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexDDNSText","s")%>
    
    </a></td>
  </tr>
                                    
                                    

              
  <tr>
    <td width="150" height="30" class="title-main">
    
    <%tcWebApi_get("String_Entry","HelpIndexMaintenText","s")%>
    
    </td>
    <td class="black">&nbsp;</td>
    <td><hr noshade></td>
    <td><hr noshade></td>
    <td><hr noshade></td>
  </tr>
                                                                                             
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_maintenance.asp#Administration" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexAdminText","s")%>
    
    </a></td>
  </tr>
                                                                       
                                                                                                    
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_maintenance.asp#Time_Zone" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexTimeZoneText","s")%></a> 
    
    </td>
  </tr>
                                                                       
                                                                                                               
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_maintenance.asp#Firmware" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexFirmwareText","s")%>
    
    </a></td>
  </tr>
                                                                       
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_maintenance.asp#SysRestart" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexSysRestartText","s")%>
    
    </a></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_maintenance.asp#Diagnostics" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexDiagnText","s")%>
    
    </a></td>
  </tr>
                                 
<% IF tcWebApi_get("dynDisp_Entry","MainMaskBit5","h") = "1" THEN%>
    <tr>
    <td width="150" height="30" class="title-main">
    
    <%tcWebApi_get("String_Entry","NavigationVoipStatusText","s")%>
    
    </td>
    <td class="black">&nbsp;</td>
    <td><hr noshade></td>
    <td><hr noshade></td>
    <td><hr noshade></td>
  </tr>
                                                                                             
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_voip.asp#Basic" target=_blank>
    
    <%tcWebApi_get("String_Entry","NavigationVoipBasicText","s")%>
    
    </a></td>
  </tr>
                                                                       
                                                                                                    
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_voip.asp#Call_Control" target=_blank>
    
    <%tcWebApi_get("String_Entry","NavigationVoipCallText","s")%>
    
    </a></td>
  </tr>
                                                                       
                                                                                                               
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_voip.asp#Media" target=_blank>
    
    <%tcWebApi_get("String_Entry","NavigationVoipMediaText","s")%>
    
    </a></td>
  </tr>
                                                                       
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_voip.asp#Speed_Dial" target=_blank>
    
    <%tcWebApi_get("String_Entry","NavigationVoipDialTex","s")%>
    
    </a></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_voip.asp#Advanced" target=_blank>
    
    <%tcWebApi_get("String_Entry","NavigationVoipAdvancedText","s")%>
    
    </a></td>
  </tr>
<%End if%> 


  <tr>
    <td width="150" height="30" class="title-main">
    
    <%tcWebApi_get("String_Entry","HelpIndexStatusText","s")%>
    
    </td>
    <td class="black">&nbsp;</td>
    <td><hr noshade></td>
    <td><hr noshade></td>
    <td><hr noshade></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_status.asp#Device_Info" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexDevInfoText","s")%>
    
    </a></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_status.asp#System_Log" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexSysLogText","s")%>
    
    </a></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"><a href="/cgi-bin/help_status.asp#Statistics" target=_blank>
    
    <%tcWebApi_get("String_Entry","HelpIndexStatsText","s")%>
    
    </a></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>

<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">&nbsp;</td>
  </tr>
</table>
</body>
</html>        
