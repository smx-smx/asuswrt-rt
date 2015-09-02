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
<style type="text/css">

</style>
<script>
function ExitWizard() {
    if (confirm("<% tcWebApi_get("String_Entry","LoginFailJS0Text","s") %>")) {
		window.parent.close();
	}
}

function uiSave() {
//	if(!pppDoValidatePage() || !pvcDoValidatePage())
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if(!pvcDoValidatePage())
		return ;
<%end if%>
  if(quotationCheck(document.forms[0].uiViewUserNameMark) ) 
		return;
	if(quotationCheck(document.forms[0].uiViewPasswordMark) ) 
		return;
	
		document.wzPPPform.submit();

}

function pvcDoValidatePage() {
	var value;
	var message;

	value = document.wzPPPform.uiViewPvcVpi.value;
	if(!isNumeric(value)){
		alert("<% tcWebApi_get("String_Entry","WizardWanDHCPJS0Text","s") %>");
		return false;
	}
	else if(Number(value) > 255 || Number(value)<0) {
		alert("<% tcWebApi_get("String_Entry","WizardWanDHCPJS1Text","s") %>");
		return false;
	}
	value = document.wzPPPform.uiViewPvcVci.value;
	if(!isNumeric(value)) {
		alert("<% tcWebApi_get("String_Entry","WizardWanDHCPJS2Text","s") %>");
		return false;
	}
	else if(Number(value) > 65535 || Number(value)<32) {
		alert("<% tcWebApi_get("String_Entry","WizardWanDHCPJS3Text","s") %>");
		return false;
	}
	return true;
}

function pppDoValidatePage() {
    var message;
    var value;
    var count=0;

    value = document.wzPPPform.Al_PPPUsername.value;
    message = valDoValidateUsrPwd(value,'1');
    if(message!=null) { return false; erlDoAddError("bad","Al_PPPUsername","",value,message); count++; }

    value = document.wzPPPform.Al_PPPPassword.value;
    message = valDoValidateUsrPwd(value,'0');
    if(message!=null) { return false; erlDoAddError("bad","Al_PPPPassword","",value,message);  count++; }
    if(count!=0) message="PPP";
    else message="";
    return true;
}

function quotationCheck(object) {
	var len = object.value.length;
	var c;
	var i;
    for (i = 0; i < len; i++)
    {
	 	 var c = object.value.charAt(i);
      
	 	 if (c == '"' || c == '$' || c == '\\' || c == '`')
		 {
				alert('<% tcWebApi_get("String_Entry","WANJS14Text","s") %>');      	  								    	    	   		
		 		return true;
		 }
    }
    
	return false;
}

</script>
</head>

<body topmargin="10" leftmargin="0">

<form name="wzPPPform" method="post" ACTION="/cgi-bin/wizardcomp.asp">
<INPUT TYPE="HIDDEN" NAME="wan_VCStatus" VALUE="Yes">

<INPUT TYPE="HIDDEN" NAME="TypeFlag" VALUE="2">
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
<INPUT TYPE="HIDDEN" NAME="wan_ConnectSelect" VALUE="Connect_Keep_Alive">
<INPUT TYPE="HIDDEN" NAME="wan_IdleTimeT" VALUE="">
<INPUT TYPE="HIDDEN" NAME="wan_TCPMSS" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wan_PPPGetIP" VALUE="Dynamic">
<INPUT TYPE="HIDDEN" NAME="wan_StaticIPaddr" VALUE="">
<INPUT TYPE="HIDDEN" NAME="wan_StaticIPSubMask" VALUE="">
<INPUT TYPE="HIDDEN" NAME="wan_StaticIpGateway" VALUE="">
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
	<% tcWebApi_get("String_Entry","HelpIndexQuickText","s") %> - <% tcWebApi_get("String_Entry","PVCListCgiEncap2Text","s") if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>/<% tcWebApi_get("String_Entry","PVCListCgiEncap3Text","s") end if%>
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
	<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
    		<% tcWebApi_get("String_Entry","WizardWanPPPInfoText","s") %>
	<%else%>
		<% tcWebApi_get("String_Entry","WizardWanPPPInfo1Text","s") %>
	<%end if%>
    </td>
  </tr>
  <tr>
    <td height="10"></td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WANUsernameText","s") %>:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="text" value="<%If tcWebApi_get("Wan_PVC","USERNAME","h") <> "N/A" then tcWebApi_get("Wan_PVC","USERNAME","s") end if%>" size="14" maxlength="32" class="" name="uiViewUserNameMark" id="uiViewUserName">
        
    </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WANPasswardText","s") %>:
    
    </td>
    <td class="tabdata" align="left">
        
        <input type="password" value="<%If tcWebApi_get("Wan_PVC","PASSWORD","h") <> "N/A" then tcWebApi_get("Wan_PVC","PASSWORD","s") end if%>" size="32" maxlength="64" class="" name="uiViewPasswordMark" id="uiViewPassword">
        
    </td>
  </tr>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
  <tr>
    <td>&#12288;</td>
    <td class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WANVPIText","s") %>:
    
    </td>
    <td class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<%If tcWebApi_get("Wan_PVC","VPI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VPI","s") end if%>" name="wzDHCP_VPI" id="uiViewPvcVpi" title="VPI parameter 0..255">
        
        <%tcWebApi_get("String_Entry","WANVPIrangeText","s")%>
    </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="150" class="tabdata" align="right">
    
    <% tcWebApi_get("String_Entry","WANVCIText","s") %>:
    
    </td>
    <td width="310" class="tabdata" align="left">
        
        <input class="uiTextInput" type="text" size="5" maxlength="5" value="<%If tcWebApi_get("Wan_PVC","VCI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VCI","s") end if%>" name="wzDHCP_VCI" id="uiViewPvcVci" title="VCI parameter 6..65535">
        
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
			<option value="PPPoE LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE LLC" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation4Text","s")%>
			<option value="PPPoE VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE VC-Mux" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation5Text","s")%>
			<option value="PPPoA LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA LLC" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation6Text","s")%>
			<option value="PPPoA VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA VC-Mux" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation7Text","s")%>
        </select>
        
    </td>
  </tr>
<%end if%>

  <tr>
    <td height="25">
        
        <input type="hidden" name="wzPPPExitFlag">
        
    </td>
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
