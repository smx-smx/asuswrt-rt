<%
 tcWebApi_set("Wan_PVC","ISP","TypeFlag")
if request_Form("wanMultiService") = "1" then
	tcWebApi_set("WebCurSet_Entry", "wan_pvc_ext", "WAN_PVC_EXT")
	tcWebApi_set("WebCurSet_Entry", "nat_pvc_ext", "WAN_PVC_EXT")
end if	

%>
<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/jsl.js"></script>
<script language="JavaScript" type="text/javascript" src="/val.js"></script>
<script language="JavaScript" type="text/javascript" src="/pvc.js"></script>
<script language="JavaScript" type="text/javascript" src="/ip_new.js"></script>
<style type="text/css">

</style>
<script>
function doSubnetCheck()
{
	var sIP = document.wzStaticform.wzStatic_IPaddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits = sIP[0].split(".");
	var mask = document.wzStaticform.wzStatic_SubMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");
	var gatewayIP = document.wzStaticform.wzStatic_GatewayAdd.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits3 = gatewayIP[0].split(".");
	for(i=0;i<4;i++)
	{
	  if((digits2[i] & digits3[i]) != (digits2[i] & digits[i]))
		{
			alert("<% tcWebApi_get("String_Entry","WizardWanStaticJS0Text","s") %>");                                   						
			return false;
		}
	}
	return true;
}
function ExitWizard()
{
    if (confirm("<% tcWebApi_get("String_Entry","LoginFailJS0Text","s") %>")) {
		window.parent.close();
	}
}

function IPoACheck() {
	if (document.wzStaticform.uiViewEncaps.selectedIndex == 2) {
		document.wzStaticform.wzStatic_SubMask.disabled = true;
		document.wzStaticform.wzStatic_SubMask.value = "0.0.0.0";
		document.wzStaticform.wzStatic_GatewayAdd.disabled = true;
		document.wzStaticform.wzStatic_GatewayAdd.value = "0.0.0.0";
	}  else {
		document.wzStaticform.wzStatic_SubMask.disabled = false;
		document.wzStaticform.wzStatic_GatewayAdd.disabled = false;
	}
	return;
}


function vpivcicheck() {
	var value;
	
	value = document.forms[0].uiViewPvcVpi.value;
	if (!isNumeric(value)) {
	   alert('<% tcWebApi_get("String_Entry","WizardWanDHCPJS0Text","s") %>');
		return false;
	}
	if(Number(value) > 255 || Number(value)<0) 
	{ 
		alert("<% tcWebApi_get("String_Entry","WizardWanDHCPJS1Text","s") %>"); 
		return false; 
  	}

	value = document.forms[0].uiViewPvcVci.value;
	if (!isNumeric(value)) {
	   alert('<% tcWebApi_get("String_Entry","WizardWanDHCPJS2Text","s") %>');
		return false;
	}
  	if(Number(value) > 65535 || Number(value)<32) 
  	{ 
		alert("<% tcWebApi_get("String_Entry","WizardWanDHCPJS3Text","s") %>"); 
		return false; 
  	}  
	return true;
}
function uiSave() {
	var value;
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>	
	if (!vpivcicheck())
	  	return;
<%end if%>
	value = document.forms[0].wzStatic_IPaddr.value;
	if (inValidIPAddr(value))
		return;
			
	value = document.forms[0].wzStatic_SubMask.value;
	if (inValidSubnetMask(value))
		return;
		
	value = document.forms[0].wzStatic_GatewayAdd.value;
	if (inValidIPAddr(value))
		return;
  	if(!doSubnetCheck()){
  		return;
  	}
	document.wzStaticform.submit();
}
</script>
</head>

<body topmargin="10" leftmargin="0">
<body topmargin="10" leftmargin="0">

<form name="wzStaticform" method="post" ACTION="/cgi-bin/wizardcomp.asp">
<INPUT TYPE="HIDDEN" NAME="wan_VCStatus" VALUE="Yes">

<INPUT TYPE="HIDDEN" NAME="TypeFlag" VALUE="1">
<INPUT TYPE="HIDDEN" NAME="ATM_QoS" VALUE="ubr">
<INPUT TYPE="HIDDEN" NAME="ATM_PCR" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="ATM_SCR" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="ATM_MBS" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wan_NAT" VALUE="Enable">
<INPUT TYPE="HIDDEN" NAME="wan_DefaultRoute" VALUE="Yes">
<INPUT TYPE="HIDDEN" NAME="wan_TCPMTU" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wan_RIPVersion" VALUE="RIP1">
<INPUT TYPE="HIDDEN" NAME="wan_RIPDirection" VALUE="None">
<INPUT TYPE="HIDDEN" NAME="wan_IGMP" VALUE="Disabled">
<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then %>
<INPUT TYPE="HIDDEN" NAME="wan_IPVERSION" VALUE="IPv4/IPv6">	
<% end if %>
<table width="500" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="500" height="5" valign="baseline" class="orange"></td>
  </tr>
</table>

<table border="0" width="500" align="center" cellspacing="0" cellpadding="0">
  <tr>
    <td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
    <td width="300" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
	  
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then tcWebApi_get("String_Entry","AdslVdslInfoText","s")%>/<%end if tcWebApi_get("String_Entry","AdslInfoText","s")%>
	  
	</td>
  </tr>
  <tr>
    <td width="500" colspan="2" class="orange"> </td>
  </tr>
</table>

<table width="500" height="2" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td class="orange"> </td>
  </tr>
</table>

<table width="500" height="35" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
  <tr>
    <td width="500" bgcolor="#FFFFFF" class="headline">&nbsp; 
	<% tcWebApi_get("String_Entry","HelpIndexQuickText","s") %> -  <% tcWebApi_get("String_Entry","WANStaticIPText","s") %>
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
    <% tcWebApi_get("String_Entry","WizardWanStaticInfoText","s") %>
    
	 
	<input type="hidden" name="wanVCFlag">
	
	</td>
  </tr>
  <tr>
    <td height="10"></td>
    <td></td>
    <td></td>
  </tr>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WANVPIText","s") %>:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<% If tcWebApi_get("Wan_PVC","VPI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VPI","s") end if%>"  name="wzDHCP_VPI" id="uiViewPvcVpi" title="VPI parameter 0..255">
        
        <%tcWebApi_get("String_Entry","WANVPIrangeText","s")%> </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WANVCIText","s") %>:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<% If tcWebApi_get("Wan_PVC","VCI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VCI","s") end if%>"  name="wzDHCP_VCI" id="uiViewPvcVci" title="VCI parameter 6..65535">
        
        <%tcWebApi_get("String_Entry","WANVCIrangeText","s")%></td>
  </tr>
<%end if%>

  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WizardWanStaticIPText","s") %>:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="wzStatic_IPaddr" value="<%If tcWebApi_get("Wan_PVC","IPADDR","h") <> "N/A" then tcWebApi_get("Wan_PVC","IPADDR","s") end if%>" id="uiViewIpAddress">
        
        </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WizardWanStaticMaskText","s") %>:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="wzStatic_SubMask" value="<%If tcWebApi_get("Wan_PVC","NETMASK","h") <> "N/A" then tcWebApi_get("Wan_PVC","NETMASK","s") end if%>" id="uiViewNetMask">
        
        </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WizardWanStaticGwayText","s") %>:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="wzStatic_GatewayAdd" value="<%If tcWebApi_get("Wan_PVC","GATEWAY","h") <> "N/A" then tcWebApi_get("Wan_PVC","GATEWAY","s") end if%>" id="uiViewGateway">
        
        </td>
  </tr>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
  <tr>
    <td width="40">&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","DeviceConnectionTypeText","s") %>: 
    
    </td>
    <td class="tabdata" align="left">
    	
        <select id="uiViewEncaps" name="uiViewEncaps">
			<option value="1483 Bridged IP LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation0Text","s")%>
			<option value="1483 Bridged IP VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation1Text","s")%>
			<option value="1483 Routed IP LLC(IPoA)" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP LLC(IPoA)" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation2Text","s")%>
			<option value="1483 Routed IP VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP VC-Mux" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation3Text","s")%>
        </select>
        
        </td>
  </tr>
<%end if%>

  <tr>
    <td height="25"></td>
  </tr>
</table>

<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="right">
          <input name="BackBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonBackText","s") %>" class="tabdata" onClick="javascript:window.location='/cgi-bin/wizardConType.asp'">
      	  <input name="NextBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonNextText","s") %>" class="tabdata" onClick="uiSave()">
	    <input name="ExitBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonExitText","s") %>" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>


</form>
</body>
</html>        
