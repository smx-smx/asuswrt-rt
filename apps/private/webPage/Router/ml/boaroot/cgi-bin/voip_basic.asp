<%
If request_Form("VoIP_basic_flag")="1" then
	tcWebApi_set("VoIPBasic_Entry","SIPEnable","VoIPSIPEnable")
	tcWebApi_set("VoIPBasic_Entry","SIPDisplayName","VoIPSIPDisplayName")
	tcWebApi_set("VoIPBasic_Entry","SIPAuthenticationName","VoIPSIPAuthenticationName")
	tcWebApi_set("VoIPBasic_Entry","SIPPassword","VoIPSIPPassword")
	

	
	tcWebApi_set("VoIPBasic_Common","SIPProxyEnable","VoIPSIPProxyEnable")
	tcWebApi_set("VoIPBasic_Common","SIPProxyAddr","VoIPSIPProxyAddr")
	tcWebApi_set("VoIPBasic_Common","SIPProxyPort","VoIPSIPProxyPort")
	tcWebApi_set("VoIPBasic_Common","SIPOutboundProxyEnable","VoIPSIPOutboundProxyEnable")
	tcWebApi_set("VoIPBasic_Common","SIPOutboundProxyAddr","VoIPSIPOutboundProxyAddr")
	tcWebApi_set("VoIPBasic_Common","SIPOutboundProxyPort","VoIPSIPOutboundProxyPort")
	tcWebApi_set("VoIPBasic_Common","SIPTransportProtocol","VoIPSIPTransportProtocol")
	tcWebApi_set("VoIPBasic_Common","LocalSIPPort","VoIPLocalSIPPort")
	tcWebApi_set("VoIPBasic_Common","LocalRTPPort","VoIPLocalRTPPort")
	tcWebApi_set("VoIPBasic_Common","PRACKEnable","VoIPPRACKEnable")
	tcWebApi_commit("VoIPBasic_Entry")
Elseif	request_Form("VoIP_basic_flag")="2" then
	TCWebAPI_set("WebCurSet_Entry","SIPIndexBasic","VoIPBasicIndex")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" src="/wanfunc.js"></script>
<script language="JavaScript" src="/chksel.js"></script>
<script language="JavaScript">
function check_port(port){
	if(valDoValidateIntegerWAN(port) == false){
		return false;	
	}
	if((Number(port) > 65535 ) || (Number(port) < 1 )){
		alert('<%tcWebApi_get("String_Entry","VoIPBasicJS0Text","s")%>' + port);
		return false;
	}

	return true;
}

function check_siproxdPort(port){
	if(Number(port) == 9876){
		alert('<%tcWebApi_get("String_Entry","VoIPBasicJS10Text","s")%>' + port + '<%tcWebApi_get("String_Entry","VoIPBasicJS11Text","s")%>');
		return false;
	}
	return true;
}


function checkDisplayName(digit) {
	if(((digit >= 48) && (digit <= 57)) || ((digit >= 65) && (digit <= 90)) || ((digit >= 97) && (digit <= 122))) {
		return 0;
	} else {
		return -1;
	}
}
function checkAuthName(digit) {
	if((digit >= 48) && (digit <= 57)) {
		return 0;
	} else {
		return -1;
	}
}
function checkValidateIP(Address)
{
  var address = Address.match("^[0-9]{1,32}\.[0-9]{1,32}\.[0-9]{1,32}\.[0-9]{1,32}$");
  var digits;
  var i;
 
  if(address == null) { //domain name
  	return true;
  }
  else
  {
    digits = address[0].split(".");
    for(i=0; i < 4; i++)
    {
      if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
      { 
      	alert("<%tcWebApi_get("String_Entry","JSInvalidIPAddrText","s")%>  " + Address); 
      	return false;
      	break; 
      }
    }
	if( (digits[0] == 0) || (digits[3] == 0) || (digits[0] == 127) )	
    {
    	alert("<%tcWebApi_get("String_Entry","JSInvalidIPAddrText","s")%>  " + Address);
    	return false;
    }
  }
  return true;;
}






function doIndexChange(){
	var form=document.VOIP_BASIC_form;

	form.VoIP_basic_flag.value=2;
	form.submit();
}
function dosave(){
	var form=document.VOIP_BASIC_form;
	var i;
	var tmp;

	if(form.VoIPSIPEnable[0].checked){
		if(form.VoIPSIPDisplayName.value.length > 0){
			for(i = 0; i < form.VoIPSIPDisplayName.value.length; i++) {
				tmp = form.VoIPSIPDisplayName.value.charCodeAt(i);
				if(checkDisplayName(tmp) != 0) {
					alert("<%tcWebApi_get("String_Entry","VoIPBasicJS2Text","s")%>");
					return;
				}
			}
		}
		if(form.VoIPSIPAuthenticationName.value.length <= 0){
			alert("<%tcWebApi_get("String_Entry","VoIPBasicJS2Text","s")%>");
			return;
		}
		else{
			for(i = 0; i < form.VoIPSIPAuthenticationName.value.length; i++) {
				tmp = form.VoIPSIPAuthenticationName.value.charCodeAt(i);
				if(checkAuthName(tmp) != 0) {
					alert("<%tcWebApi_get("String_Entry","VoIPBasicJS3Text","s")%>");
					return;
				}
		
			}
		}
	}
		
	if(form.VoIPSIPProxyEnable[0].checked){
		if(form.VoIPSIPProxyAddr.value.length <= 0){
			alert("<%tcWebApi_get("String_Entry","VoIPBasicJS4Text","s")%>");
			return;
		}
		else{
			if(checkValidateIP(form.VoIPSIPProxyAddr.value)!= true ){
					return;
				}
		}
		if(form.VoIPSIPProxyPort.value.length <= 0){
			alert("<%tcWebApi_get("String_Entry","VoIPBasicJS6Text","s")%>");
			return;
		}
		if(check_port(form.VoIPSIPProxyPort.value) == false){
			return;	
		}
	}
	if(form.VoIPSIPOutboundProxyEnable[0].checked){
		if(form.VoIPSIPOutboundProxyAddr.value.length <= 0){
			alert("<%tcWebApi_get("String_Entry","VoIPBasicJS7Text","s")%>");
			return;
		}
		else{
			if(checkValidateIP(form.VoIPSIPOutboundProxyAddr.value)!= true ){
				return;
			}

		}
		if(form.VoIPSIPOutboundProxyPort.value.length <= 0){
			alert("<%tcWebApi_get("String_Entry","VoIPBasicJS9Text","s")%>");
			return;
		}
		if(check_port(form.VoIPSIPOutboundProxyPort.value) == false){
			return;	
		}
	}

	if((check_port(form.VoIPLocalSIPPort.value) == false)
		|| (check_port(form.VoIPLocalRTPPort.value) == false)){
			return;
	}
	
<% if TCWebAPI_get("Siproxd_Entry", "SiproxdEnable", "h") = "Yes" then%>
	if((check_siproxdPort(form.VoIPLocalSIPPort.value) == false)
		|| (check_siproxdPort(form.VoIPLocalRTPPort.value) == false)){
			return;
	}
<%end if%>



  	form.VoIP_basic_flag.value=1;
	form.submit();
}
function LineOn(on){
	var form=document.VOIP_BASIC_form;

	form.VoIPSIPDisplayName.disabled=on;
	form.VoIPSIPAuthenticationName.disabled=on;
	form.VoIPSIPPassword.disabled=on;
}
function SIPProxyOn(on){
	var form=document.VOIP_BASIC_form;

	form.VoIPSIPProxyAddr.disabled=on;
	form.VoIPSIPProxyPort.disabled=on;
}
function SIPOutboundProxyOn(on){
	var form=document.VOIP_BASIC_form;

	form.VoIPSIPOutboundProxyAddr.disabled=on;
	form.VoIPSIPOutboundProxyPort.disabled=on;
}
function OnLoadCheck(){
	var form=document.VOIP_BASIC_form;

	if(form.VoIPSIPEnable[0].checked)
		LineOn(false)
	else
		LineOn(true)
	if(form.VoIPSIPProxyEnable[0].checked)
		SIPProxyOn(false)
	else
		SIPProxyOn(true)
	if(form.VoIPSIPOutboundProxyEnable[0].checked)
		SIPOutboundProxyOn(false)
	else
		SIPOutboundProxyOn(true)
}
</script>
</head><body onLoad="OnLoadCheck()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_basic.asp" name="VOIP_BASIC_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","VoIPBasicText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicLineText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="VoIPBasicIndex" SIZE="1" onChange="doIndexChange()">
				<OPTION VALUE="0" <%if TCWebAPI_get("WebCurSet_Entry", "SIPIndexBasic", "h") = "0" then asp_Write("selected") end if %>>1
<% if TCWebAPI_get("VoIPBasic_Common", "VoIPLine2Enable", "h") = "Yes" then%>
				<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "SIPIndexBasic", "h") = "1" then asp_Write("selected") end if %>>2
<%end if%>
			</SELECT>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicStatusText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPSIPEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPBasic_Entry", "SIPEnable", "h") = "Yes" then asp_write("checked") end if %> onclick="LineOn(false)"><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicStatus0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPSIPEnable" VALUE="No" <%if TCWebAPI_get("VoIPBasic_Entry", "SIPEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="LineOn(true)"><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicStatus1Text","s")%></font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicDisplayNameText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPSIPDisplayName" SIZE="14" MAXLENGTH="12" VALUE="<%if TCWebAPI_get("VoIPBasic_Entry", "SIPDisplayName", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Entry", "SIPDisplayName", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicAuthNameText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPSIPAuthenticationName" SIZE="14" MAXLENGTH="12" VALUE="<%if TCWebAPI_get("VoIPBasic_Entry", "SIPAuthenticationName", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Entry", "SIPAuthenticationName", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicPasswordText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="VoIPSIPPassword" SIZE="14" MAXLENGTH="12" VALUE="<%if TCWebAPI_get("VoIPBasic_Entry", "SIPPassword", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Entry", "SIPPassword", "s") else asp_Write("") end if %>">
		</td></tr>
				
			
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicProxyStatusText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
<%if TCWebAPI_get("WebCurSet_Entry", "SIPIndexBasic", "h") = "0" then%>
		<td class="tabdata"><font color="#000000"><%if TCWebAPI_get("DeviceInfo", "ConnStat0", "h") <> "N/A" then TCWebAPI_get("DeviceInfo", "ConnStat0", "s") else asp_Write("Not Connected") end if %></font></td></tr>
<%end if%>
<%if TCWebAPI_get("WebCurSet_Entry", "SIPIndexBasic", "h") = "1" then%>
		<td class="tabdata"><font color="#000000"><%if TCWebAPI_get("DeviceInfo", "ConnStat1", "h") <> "N/A" then TCWebAPI_get("DeviceInfo", "ConnStat1", "s") else asp_Write("Not Connected") end if %></font></td></tr>
<%end if%>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","VoIPBasicSIPSettingsText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade class="light-orange-line"></td><td width="10"><hr noshade class="light-orange-line"></td><td width="440"><hr noshade class="light-orange-line"></td></tr>
	<tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicSIPProxyText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPSIPProxyEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPBasic_Common", "SIPProxyEnable", "h") = "Yes" then asp_write("checked") end if %> onclick="SIPProxyOn(false)"><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicSIPProxy0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPSIPProxyEnable" VALUE="No" <%if TCWebAPI_get("VoIPBasic_Common", "SIPProxyEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="SIPProxyOn(true)"><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicSIPProxy1Text","s")%></font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicProxyAddressText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPSIPProxyAddr" SIZE="32" MAXLENGTH="32" VALUE="<%if TCWebAPI_get("VoIPBasic_Common", "SIPProxyAddr", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Common", "SIPProxyAddr", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicProxyPortText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPSIPProxyPort" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPBasic_Common", "SIPProxyPort", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Common", "SIPProxyPort", "s") else asp_Write("5060") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicOutboundProxyText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPSIPOutboundProxyEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPBasic_Common", "SIPOutboundProxyEnable", "h") = "Yes" then asp_write("checked") end if %> onclick="SIPOutboundProxyOn(false)"><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicOutboundProxy0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPSIPOutboundProxyEnable" VALUE="No" <%if TCWebAPI_get("VoIPBasic_Common", "SIPOutboundProxyEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="SIPOutboundProxyOn(true)"><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicOutboundProxy1Text","s")%></font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicOutProxyAddText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPSIPOutboundProxyAddr" SIZE="32" MAXLENGTH="32" VALUE="<%if TCWebAPI_get("VoIPBasic_Common", "SIPOutboundProxyAddr", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Common", "SIPOutboundProxyAddr", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicOutProxyPortText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPSIPOutboundProxyPort" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPBasic_Common", "SIPOutboundProxyPort", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Common", "SIPOutboundProxyPort", "s") else asp_Write("5060") end if %>">
		</td></tr>
	<tr>
		<td class="title-sub"><font color="#000000">&nbsp;</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicSIPTranProtocolText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="VoIPSIPTransportProtocol" SIZE="1">
				<OPTION <%if TCWebAPI_get("VoIPBasic_Common", "SIPTransportProtocol", "h") = "UDP" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPBasicSIPTranProtocol0Text","s")%>
				<OPTION <%if TCWebAPI_get("VoIPBasic_Common", "SIPTransportProtocol", "h") = "TCP" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPBasicSIPTranProtocol1Text","s")%>
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicLocalSIPPortText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPLocalSIPPort" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPBasic_Common", "LocalSIPPort", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Common", "LocalSIPPort", "s") else asp_Write("5060") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicLocalRTPPortText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPLocalRTPPort" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPBasic_Common", "LocalRTPPort", "h") <> "N/A" then TCWebAPI_get("VoIPBasic_Common", "LocalRTPPort", "s") else asp_Write("4000") end if %>">
		</td></tr>
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicPRACKText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPPRACKEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPBasic_Common", "PRACKEnable", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicSIPProxy0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPPRACKEnable" VALUE="No" <%if TCWebAPI_get("VoIPBasic_Common", "PRACKEnable", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPBasicSIPProxy1Text","s")%></font>
		</td></tr>
<%end if%>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="VoIP_basic_flag" VALUE="0">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="dosave();"></td></tr></table>
</form></body></html>
