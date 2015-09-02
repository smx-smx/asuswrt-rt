<%
If request_Form("VoIP_speed_dial_flag")="1" then
	TCWebAPI_set("WebCurSet_Entry","SIPIndexSpeed","editnum")
	tcWebApi_set("VoIPSpeed_Entry","Destination","VoIPDestination")
	tcWebApi_set("VoIPSpeed_Entry","SpeedDialNumber","VoIPSpeedDialNumber")
	tcWebApi_set("VoIPSpeed_Entry","UserId","VoIPUserId")
	tcWebApi_set("VoIPSpeed_Entry","IpAddressOrHostName","VoIPIpAddressOrHostName")
	tcWebApi_set("VoIPSpeed_Entry","Port","VoIPPort")
	tcWebApi_set("VoIPSpeed_Entry","SIPSpeedLine","VoIPSIPSpeedLine")
	tcWebApi_commit("VoIPSpeed_Entry")
elseif request_Form("delFlag")="1" then
	tcWebApi_set("WebCurSet_Entry","SIPIndexSpeed","editnum")
	tcWebApi_unset("VoIPSpeed_Entry")
	tcWebApi_commit("VoIPSpeed_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" type='text/javascript' src="/chksel.js"></script>
<script language="JavaScript" src="/wanfunc.js"></script>
<script language="JavaScript">
function check_port(port){
	if(valDoValidateIntegerWAN(port) == false){
		return false;
	}
	if((Number(port) > 65535 ) || (Number(port) < 1 )){
		alert('Invalid Port ' + port);
		return false;
	}
	return true;
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
      	alert("Invalid IP address  " + Address); 
      	return false;
      	break; 
      }
    }
	if( (digits[0] == 0) || (digits[3] == 0) || (digits[0] == 127) )	
    {
    	alert("Invalid IP address  " + Address);
    	return false;
    }
  }
  return true;;
}
function checkAuthName(digit) {
	if((digit >= 48) && (digit <= 57)) {
		return 0;
	} else {
		return -1;
	}
}
function chkRepeatSpeedDial(){
	var HaveSettingDialNumber = new Array(10);
	HaveSettingDialNumber[0] = "<% TCWebAPI_get("VoIPSpeed_Entry0", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[1] = "<% TCWebAPI_get("VoIPSpeed_Entry1", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[2] = "<% TCWebAPI_get("VoIPSpeed_Entry2", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[3] = "<% TCWebAPI_get("VoIPSpeed_Entry3", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[4] = "<% TCWebAPI_get("VoIPSpeed_Entry4", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[5] = "<% TCWebAPI_get("VoIPSpeed_Entry5", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[6] = "<% TCWebAPI_get("VoIPSpeed_Entry6", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[7] = "<% TCWebAPI_get("VoIPSpeed_Entry7", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[8] = "<% TCWebAPI_get("VoIPSpeed_Entry8", "SpeedDialNumber", "s") %>";
	HaveSettingDialNumber[9] = "<% TCWebAPI_get("VoIPSpeed_Entry9", "SpeedDialNumber", "s") %>";
	var f=top.main.document.VOIP_SPEED_DIAL_form;
	var now=f.editnum.value;
	var dialnumber=f.VoIPSpeedDialNumber.value;
	for(i=0; i<10; i++){
		if(i == now){
			continue;
		}
		else{
			if(dialnumber == HaveSettingDialNumber[i]){
				alert("Can't set a repeat speed dial number!");
				return 1;
			}
		}
	}
	return 0;
}
function doNonSympolCheck(c)
{
	if ((c >= "0")&&(c <= "9"))
	{
		return 0;
	}
	else{
		return 1;
	}
}
function NotAValidDialNumber(){
	var f=top.main.document.VOIP_SPEED_DIAL_form;
	var dialnumber=f.VoIPSpeedDialNumber.value;
	len=dialnumber.length;
	for(i=0;i<len;i++)
	{
		var c = dialnumber.charAt(i);	
		if(doNonSympolCheck(c))
		{
			alert("Invalid Dial Number!");
			return 1;
		}
	}

}
function dosave(){
	var f=document.VOIP_SPEED_DIAL_form
	var index=f.VoIPDestination.selectedIndex
	var i;
	var tmp;

	if( chkRepeatSpeedDial() ){
		return;
	}

	if(f.VoIPUserId.value.length <= 0){
		alert("Please input UserId");
		return;
	}else{
		for(i = 0; i < f.VoIPUserId.value.length; i++) {
			tmp = f.VoIPUserId.value.charCodeAt(i);
			if(checkAuthName(tmp) != 0) {
				alert("VoIP User ID is invalid (only digits)");
				return;
			}
		}
	}

	if(f.VoIPSpeedDialNumber.value.length <= 0){
		alert("Please input SpeedDialNumber");
		return;
	}else{
		if( NotAValidDialNumber() ){
			return;	
		}	
	}

	if(f.VoIPDestination.options[index].value=="Direct Call"){
		if(f.VoIPIpAddressOrHostName.value.length <= 0){
			alert("Please input IpAddressOrHostName");
			return;
		}else{
			if(checkValidateIP(f.VoIPIpAddressOrHostName.value)!= true ){
				return;
		}
	}
		if(f.VoIPPort.value.length <= 0){
			alert("Please input Port");
			return;
		}
		if(check_port(f.VoIPPort.value) == false){
			return;
		}
	}
	
	
	f.VoIP_speed_dial_flag.value=1;
	f.submit();
}
function checksel(){
	var f=top.main.document.VOIP_SPEED_DIAL_form;
	TcCheckSel(f.VoIPDestination, f.VoIPDestinationTmp.value);
	
	if(f.VoIPDestinationTmp.value=="Proxy"){
		f.VoIPIpAddressOrHostName.disabled=true;
		f.VoIPUserId.disabled=false;
		f.VoIPPort.disabled=true;
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
	}else if(f.VoIPDestinationTmp.value=="Local Line"){
		f.VoIPIpAddressOrHostName.disabled=true;
		f.VoIPUserId.disabled=true;
		f.VoIPPort.disabled=true;
		f.VoIPSIPSpeedLine[0].disabled=false;
		f.VoIPSIPSpeedLine[1].disabled=false;		
	}else if(f.VoIPDestinationTmp.value=="Direct Call"){
		f.VoIPIpAddressOrHostName.disabled=false;
		f.VoIPUserId.disabled=false;
		f.VoIPPort.disabled=false;
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
	}else{
		f.VoIPIpAddressOrHostName.disabled=true;
		f.VoIPUserId.disabled=false;
		f.VoIPPort.disabled=true;
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
	}

}
function directcheck(){
	var f=top.main.document.VOIP_SPEED_DIAL_form;
	var index=f.VoIPDestination.selectedIndex
	
	if(f.VoIPDestination.options[index].value=="Proxy"){
		f.VoIPIpAddressOrHostName.disabled=true;
		f.VoIPIpAddressOrHostName.value="";
		f.VoIPUserId.disabled=false;
		f.VoIPPort.disabled=true;
		f.VoIPPort.value="";
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
	}else if(f.VoIPDestination.options[index].value=="Local Line"){
		f.VoIPIpAddressOrHostName.disabled=true;
		f.VoIPIpAddressOrHostName.value="";
		f.VoIPUserId.disabled=true;
		f.VoIPUserId.value="";
		f.VoIPPort.disabled=true;
		f.VoIPPort.value="";
		f.VoIPSIPSpeedLine[0].disabled=false;
		f.VoIPSIPSpeedLine[1].disabled=false;		
	}else if(f.VoIPDestination.options[index].value=="Direct Call"){
		f.VoIPIpAddressOrHostName.disabled=false;
		f.VoIPUserId.disabled=false;
		f.VoIPPort.disabled=false;
		f.VoIPSIPSpeedLine[0].disabled=true;
		f.VoIPSIPSpeedLine[1].disabled=true;
	}	
}
</script>
</head><body onload="checksel()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_speed_dial.asp" name="VOIP_SPEED_DIAL_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
	<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
	<td width="150" height="30" class="title-main">
	<font color="#FFFFFF">VoIP Speed Dial</font></td>
	<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Index</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata" id="EditIndex"><%tcWebApi_get("WebCurSet_Entry","SIPIndexSpeed", "s")%>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Destination</font></td>
		<td class="tabdata" align=center>:</td>
		<INPUT TYPE="HIDDEN" NAME="VoIPDestinationTmp" VALUE="<% TCWebAPI_get("VoIPSpeed_Entry", "Destination", "s") %>">
		<td class="tabdata">
			<SELECT NAME="VoIPDestination" SIZE="1" onchange="directcheck()">
				<OPTION value="Proxy" >Proxy
			<!--	<OPTION value="Local Line" >Local Line -->
				<OPTION value="Direct Call" >Direct Call
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Speed Dial Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPSpeedDialNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPSpeed_Entry", "SpeedDialNumber", "h") <> "N/A" then TCWebAPI_get("VoIPSpeed_Entry", "SpeedDialNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">User Id</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPUserId" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPSpeed_Entry", "UserId", "h") <> "N/A" then TCWebAPI_get("VoIPSpeed_Entry", "UserId", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">IP Address / Host Name</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPIpAddressOrHostName" SIZE="32" MAXLENGTH="32" VALUE="<%if TCWebAPI_get("VoIPSpeed_Entry", "IpAddressOrHostName", "h") <> "N/A" then TCWebAPI_get("VoIPSpeed_Entry", "IpAddressOrHostName", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Port</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPPort" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPSpeed_Entry", "Port", "h") <> "N/A" then TCWebAPI_get("VoIPSpeed_Entry", "Port", "s") else asp_Write("") end if %>">
		</td></tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
	<TR>
		<TD class=light-orange width=150>&nbsp;</TD>
		<TD class=light-orange width=10></TD>
		<TD class=tabdata><iframe src="/cgi-bin/voip_speed_dial.cgi" id="cgi_frame" frameborder="0" width="550" height="250"></iframe></TD>
		</TR></TBODY></TABLE>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="VoIP_speed_dial_flag" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="editnum" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="delFlag" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="VoIPSIPSpeedLine" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="VoIPSIPSpeedLine" VALUE="0">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="dosave();"></td></tr></table>
</form></body></html>
