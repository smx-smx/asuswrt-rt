<%
tcWebApi_unset("Wan_PVC")
tcWebApi_set("Wan_PVC","ISP","TypeFlag")
tcWebApi_set("Wan_PVC","Active","wan_VCStatus")
if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
tcWebApi_set("Wan_PVC","QOS","ATM_QoS")
tcWebApi_set("Wan_PVC","PCR","ATM_PCR")
tcWebApi_set("Wan_PVC","SCR","ATM_SCR")
tcWebApi_set("Wan_PVC","MBS","ATM_MBS")
end if

tcWebApi_set("Wan_PVC","NATENABLE","wan_NAT")
tcWebApi_set("Wan_PVC","DEFAULTROUTE","wan_DefaultRoute")
tcWebApi_set("Wan_PVC","MTU","wan_TCPMTU")
tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP")
tcWebApi_set("Wan_PVC","DIRECTION","wan_RIP_Dir")
tcWebApi_set("Wan_PVC","IGMP","wan_IGMP")
tcWebApi_set("Wan_PVC","IPVERSION","wan_IPVERSION")

If Request_Form("TypeFlag")= "0" Then
if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
	tcWebApi_set("Wan_PVC","VPI","wzDHCP_VPI")
	tcWebApi_set("Wan_PVC","VCI","wzDHCP_VCI")
	tcWebApi_set("Wan_PVC","ENCAP","select")
end if
End If

If Request_Form("TypeFlag")= "1" Then
if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
	tcWebApi_set("Wan_PVC","VPI","wzDHCP_VPI")
	tcWebApi_set("Wan_PVC","VCI","wzDHCP_VCI")
	tcWebApi_set("Wan_PVC","ENCAP","uiViewEncaps")
end if
 	tcWebApi_set("Wan_PVC","IPADDR","wzStatic_IPaddr")
	tcWebApi_set("Wan_PVC","NETMASK","wzStatic_SubMask")
	tcWebApi_set("Wan_PVC","GATEWAY","wzStatic_GatewayAdd")
	tcWebApi_set("Wan_PVC","ENCAP","uiViewEncaps")
End If

If Request_Form("TypeFlag")= "2" Then

	tcWebApi_set("Wan_PVC","USERNAME","uiViewUserNameMark")
	tcWebApi_set("Wan_PVC","PASSWORD","uiViewPasswordMark")
if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
	tcWebApi_set("Wan_PVC","VPI","wzDHCP_VPI")
	tcWebApi_set("Wan_PVC","VCI","wzDHCP_VCI")
	tcWebApi_set("Wan_PVC","ENCAP","select")
end if
	tcWebApi_set("Wan_PVC","CONNECTION","wan_ConnectSelect")
	tcWebApi_set("Wan_PVC","CLOSEIFIDLE","wan_IdleTimeT")
	tcWebApi_set("Wan_PVC","MSS","wan_TCPMSS")
	tcWebApi_set("Wan_PVC","PPPGETIP","wan_PPPGetIP")
	tcWebApi_set("Wan_PVC","IPADDR","wan_StaticIPaddr")
	tcWebApi_set("Wan_PVC","NETMASK","wan_StaticIPSubMask")
	tcWebApi_set("Wan_PVC","GATEWAY","wan_StaticIpGateway")
End If

If Request_Form("TypeFlag")= "3" Then
if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
	tcWebApi_set("Wan_PVC","VPI","wzDHCP_VPI")
	tcWebApi_set("Wan_PVC","VCI","wzDHCP_VCI")
	tcWebApi_set("Wan_PVC","ENCAP","select")
end if
End If

%>

<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=

iso-8859-1

">
<link rel="stylesheet" href="/style.css" type="text/css">
<script type='text/javascript' src="/jsl.js"></script>
<style type="text/css">

</style>
<script>
function ExitWizard()
{
    if (confirm("<% tcWebApi_get("String_Entry","LoginFailJS0Text","s") %>")) {
		window.parent.close();
	}
}

function uiSave() {
	document.wzCompleteform.submit();
}
</script>
</head>

<body topmargin="10" leftmargin="0">

<form name="wzCompleteform" method="post" ACTION="/cgi-bin/wizardclose.asp">


<table width="500" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="500" height="5" valign="baseline" class="orange"></td>
  </tr>
</table>

<table border="0" width="500" align="center" cellspacing="0" cellpadding="0">
  <tr>
    <td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
    <td width="300" height="50" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
	  
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%><%tcWebApi_get("String_Entry","AdslVdslInfoText","s")%>/<%end if%><%tcWebApi_get("String_Entry","AdslInfoText","s")%>
	  
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
    <td width="500" bgcolor="#FFFFFF" class="headline"><div align="left">&nbsp;
    
    <%tcWebApi_get("String_Entry","WizardCompleteText","s")%>
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&nbsp;</td>
    <td colspan="2" class="tabdata">
        
        <%tcWebApi_get("String_Entry","WizardComQueryInfoText","s")%>
        
        
        <input type="hidden" name="wzExitFlag">
        
    </td>
  </tr>

  <tr>
    <td></td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td width="40">&nbsp;</td>
    <td width="150" class="tabdata" align="right">&nbsp;</td>
    <td width="310" class="tabdata" align="left">&nbsp;</td>
  </tr>
  <tr>
    <td width="40">&nbsp;</td>
    <td width="150" class="tabdata" align="right">&nbsp;</td>
    <td class="tabdata" align="left">&nbsp;</td>
  </tr>
  <tr>
    <td height="25"></td>
  </tr>
</table>

<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="right">
          <input name="BackBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonBackText","s") %>" class="tabdata" onClick="javascript:history.go(-1);">
        	  <input name="NextBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonNextText","s") %>" class="tabdata" onClick="uiSave()">
	  <input name="ExitBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonExitText","s") %>" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&nbsp;</td>
</tr>
</table>


</form>
</body>
</html>        
