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
<script>
function ExitWizard()
{
    if (confirm("<% tcWebApi_get("String_Entry","LoginFailJS0Text","s") %>")) {
		//document.wzBridgeform.wzBridgeExitFlag.value = 1;
		//document.wzBridgeform.submit();
		window.parent.close();
	}
}

function isNumeric(s)
{
  var len= s.length;
  var ch;
  if(len==0)
    return false;
  for( i=0; i< len; i++)
  {
    ch= s.charAt(i);
    if( ch > '9' || ch < '0')
    {
      return false;
    }
  }
  return true;
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
  	if(Number(value) > 65535 || Number(value)<1) 
  	{ 
		alert("<% tcWebApi_get("String_Entry","WizardWanDHCPJS3Text","s") %>"); 
		return false; 
  	}  
	return true;
}


function uiSave() {
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if (!vpivcicheck())
		return;
<%end if%>
	document.wzBridgeform.submit();
}
</script>
</head>

<body topmargin="10" leftmargin="0">

<form name="wzBridgeform" method="post" ACTION="/cgi-bin/wizardcomp.asp">
<INPUT TYPE="HIDDEN" NAME="wan_VCStatus" VALUE="Yes">

<INPUT TYPE="HIDDEN" NAME="TypeFlag" VALUE="3">
<INPUT TYPE="HIDDEN" NAME="ATM_QoS" VALUE="ubr">
<INPUT TYPE="HIDDEN" NAME="ATM_PCR" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="ATM_SCR" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="ATM_MBS" VALUE="0">
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
    <td width="300" height="50" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
	  
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
	<% tcWebApi_get("String_Entry","HelpIndexQuickText","s") %> -  <% tcWebApi_get("String_Entry","WANBridgeModeText","s") %>
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
        <% tcWebApi_get("String_Entry","WizardWanBridgeInfoText","s") %>
        
		 
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
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<% If tcWebApi_get("Wan_PVC","VPI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VPI","s") end if%>" name="wzDHCP_VPI" id="uiViewPvcVpi" title="VPI parameter 0..255">
        
        <%tcWebApi_get("String_Entry","WANVPIrangeText","s")%>
    </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WANVCIText","s") %>:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<% If tcWebApi_get("Wan_PVC","VCI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VCI","s") end if%>" name="wzDHCP_VCI" id="uiViewPvcVci" title="VCI parameter 1..65535">
        
        <%tcWebApi_get("String_Entry","WANVCIrangeText","s")%>
    </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","DeviceConnectionTypeText","s") %>: 
    
    </td>
    <td class="tabdata" align="left">
    	
        <select id="uiViewEncaps" name="select">
			<option value="1483 Bridged Only LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation8Text","s")%>
			<option value="1483 Bridged Only VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation9Text","s")%>
        </select>
        
      </td>
  </tr>
  <tr>
<%end if%>

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
