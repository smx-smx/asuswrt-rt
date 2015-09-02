<%
If request_Form("VoIP_log_flag")="1" then
	tcWebApi_set("VoIPLog_Entry","LogEnable","VoIPLogEnable")
	tcWebApi_set("VoIPLog_Entry","LogServerIP","VoIPLogServerIP")
	tcWebApi_set("VoIPLog_Entry","LogServerPort","VoIPLogServerPort")
	tcWebApi_set("VoIPLog_Entry","LogDisplay","DisplaySetting")
	tcWebApi_set("VoIPLog_Entry","VoiceNetLogEnable","NetLogEnable")
	tcWebApi_set("VoIPLog_Entry","VoiceNetLogServerIP","NetLogServerIP")
	tcWebApi_commit("VoIPLog_Entry")

elseif request_Form("VoIP_log_flag")="2" then	
	tcWebApi_set("VoIPLog_Entry","LogEnable","VoIPLogEnable")	
	tcWebApi_set("VoIPLog_Entry","LogDisplay","DisplaySetting")
	tcWebApi_set("VoIPLog_Entry","VoiceNetLogEnable","NetLogEnable")
	tcWebApi_set("VoIPLog_Entry","VoiceNetLogServerIP","NetLogServerIP")
	tcWebApi_commit("VoIPLog_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script 
language="JavaScript" src="/wanfunc.js"></script>
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

function dosave(){
	var form=document.VOIP_LOG_form;

	if(form.VoIPLogEnable[0].checked){
		if(form.isSIPEnable.value == "No"){
			alert("please enable SIP Line first");
			return;
		}

		if(form.VoIPLogDisplayU.checked){
			if(form.VoIPLogDisplayC.checked){
		if(form.VoIPLogServerIP.value.length <= 0){
			alert("please input Server IP");
			return;
		}
		
		if(form.VoIPLogServerPort.value.length <= 0){
			alert("please input Server Port");
			return;
		}

		if(check_port(form.VoIPLogServerPort.value) == false){
			return;	
		}
				form.DisplaySetting.value = "Both";
			}
			else{
				if(form.VoIPLogServerIP.value.length <= 0){
					alert("please input Server IP");
					return;
				}

				if(form.VoIPLogServerPort.value.length <= 0){
					alert("please input Server Port");
					return;
				}

				if(check_port(form.VoIPLogServerPort.value) == false){
					return;	
				}
				form.DisplaySetting.value = "UDP";
			}
		}
		else{
			form.DisplaySetting.value = "Console";		
		}
  		form.VoIP_log_flag.value = 1;
	}
	else if(form.NetLogEnable[0].checked){
		if(form.isSIPEnable.value == "No"){
			alert("please enable SIP Line first");
			return;
		}
		if(form.NetLogServerIP.value.length <= 0){
			alert("please input Voice NetLog Server IP");
			return;
		}
		form.VoIP_log_flag.value = 1;
	}
  	else {
		form.VoIP_log_flag.value = 2;
		
	}
	
	form.submit();
}

function LineOn(on){
	var form=document.VOIP_LOG_form;

	form.VoIPLogServerIP.disabled=on;
	form.VoIPLogServerPort.disabled=on;
	form.VoIPLogDisplayU.disabled=on;	
	form.VoIPLogDisplayC.disabled=on;
}

function NetLineOn(on){
	var form=document.VOIP_LOG_form;
	form.NetLogServerIP.disabled=on;
}
function OnLoadCheck(){
	var form=document.VOIP_LOG_form;

	if(form.VoIPLogEnable[0].checked)
		LineOn(false)
	else
		LineOn(true)
	if(form.NetLogEnable[0].checked)
		NetLineOn(false)
	else
		NetLineOn(true)
}

</script>
</head><body onLoad="OnLoadCheck()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_log.asp" name="VOIP_LOG_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">VoIP Log</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Log</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPLogEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPLog_Entry", "LogEnable", "h") = "Yes" then asp_write("checked") end if %> onclick="LineOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPLogEnable" VALUE="No" <%if TCWebAPI_get("VoIPLog_Entry", "LogEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="LineOn(true)"><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Log Server IP</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPLogServerIP" SIZE="15" MAXLENGTH="15" VALUE="<%if TCWebAPI_get("VoIPLog_Entry", "LogServerIP", "h") <> "N/A" then TCWebAPI_get("VoIPLog_Entry", "LogServerIP", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Log Server Port</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPLogServerPort" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPLog_Entry", "LogServerPort", "h") <> "N/A" then TCWebAPI_get("VoIPLog_Entry", "LogServerPort", "s") else asp_Write("") end if %>">
		</td></tr>	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Display To</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<% if TCWebAPI_get("VoIPLog_Entry", "LogDisplay", "h") = "Both" then %><INPUT TYPE='CHECKBOX' NAME='VoIPLogDisplayC' VALUE='Console' checked><font color='#000000'>Console</font> <INPUT TYPE='CHECKBOX' NAME='VoIPLogDisplayU' VALUE='UDP' checked><font color='#000000'>UDP</font><INPUT TYPE="HIDDEN" NAME="DisplaySetting" VALUE="Both">
			<% elseif TCWebAPI_get("VoIPLog_Entry", "LogDisplay", "h") = "UDP" then %><INPUT TYPE='CHECKBOX' NAME='VoIPLogDisplayC' VALUE='Console'><font color='#000000'>Console</font> <INPUT TYPE='CHECKBOX' NAME='VoIPLogDisplayU' VALUE='UDP' checked><font color='#000000'>UDP</font><INPUT TYPE="HIDDEN" NAME="DisplaySetting" VALUE="UDP">
			<% else %>
			<INPUT TYPE='CHECKBOX' NAME='VoIPLogDisplayC' VALUE='Console' checked><font color='#000000'>Console</font> 
			<INPUT TYPE='CHECKBOX' NAME='VoIPLogDisplayU' VALUE='UDP'><font color='#000000'>UDP</font><INPUT TYPE="HIDDEN" NAME="DisplaySetting" VALUE="Console">
			<% end if %>
<!--
			<INPUT TYPE="CHECKBOX" NAME="VoIPLogDisplayUDP" VALUE="Console" <%if TCWebAPI_get("VoIPLog_Entry", "LogDisplay", "h") = "Console" then asp_write("checked") end if %> "><font color="#000000">Console</font>
			<INPUT TYPE="CHECKBOX" NAME="VoIPLogDisplay" VALUE="UDP" <%if TCWebAPI_get("VoIPLog_Entry", "LogDisplay", "h") = "Console" then asp_write("checked") end if %> "><font color="#000000">UDP</font>
-->		
		</td></tr>
	<tr>
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>


	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Voice NetLog</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="NetLogEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPLog_Entry", "VoiceNetLogEnable", "h") = "Yes" then asp_write("checked") end if %> onclick="NetLineOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="NetLogEnable" VALUE="No" <%if TCWebAPI_get("VoIPLog_Entry", "VoiceNetLogEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="NetLineOn(true)"><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Voice NetLog Server IP</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="NetLogServerIP" SIZE="15" MAXLENGTH="15" VALUE="<%if TCWebAPI_get("VoIPLog_Entry", "VoiceNetLogServerIP", "h") <> "N/A" then TCWebAPI_get("VoIPLog_Entry", "VoiceNetLogServerIP", "s") else asp_Write("") end if %>">
		</td></tr>



		
	<tr>	
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="VoIP_log_flag" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="isSIPEnable" VALUE="<% if TCWebAPI_get("VoIPBasic_Entry", "SIPEnable", "h") = "Yes" then asp_write("Yes") else asp_write("No") end if %>">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="Save" onClick="dosave();"></td></tr></table>
</form></body></html>
