<%
	If Request_Form("submitFlag") = "1" Then
		If Request_Form("PortTriggeringList_Cntr0") = "1" Then
			tcWebApi_unset("portTriggering_Entry0")
		End If
		If Request_Form("PortTriggeringList_Cntr1") = "1" Then
			tcWebApi_unset("portTriggering_Entry1")
		End If
		If Request_Form("PortTriggeringList_Cntr2") = "1" Then
			tcWebApi_unset("portTriggering_Entry2")
		End If
		If Request_Form("PortTriggeringList_Cntr3") = "1" Then
			tcWebApi_unset("portTriggering_Entry3")
		End If
		If Request_Form("PortTriggeringList_Cntr4") = "1" Then
			tcWebApi_unset("portTriggering_Entry4")
		End If
		If Request_Form("PortTriggeringList_Cntr5") = "1" Then
			tcWebApi_unset("portTriggering_Entry5")
		End If
		If Request_Form("PortTriggeringList_Cntr6") = "1" Then
			tcWebApi_unset("portTriggering_Entry6")
		End If
		If Request_Form("PortTriggeringList_Cntr7") = "1" Then
			tcWebApi_unset("portTriggering_Entry7")
		End If
		tcWebApi_commit("portTriggering")
	End If
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<script language='javascript'>
function doSave() 
{
	var ObjectNameStr = "PortTriggeringList_Cntr0";
	active = 0;
	for(i = 0; i < 8; i++)
	{
		ObjectNameStr = "PortTriggeringList_Cntr" + i;
		if(1 == document.getElementById(ObjectNameStr).value)
		{
			active++;		
		}
	}
	
	if(!active)
	{
		alert("<%tcWebApi_get("String_Entry","PortTriggerlistJS0Text","s")%>");
		return true;
	}
	document.forms[0].submitFlag.value = 1;
	document.forms[0].submit();
}
/******************For Multi-Language Strings Start*************************/
var	ary_strings = [			
				['porttriggerClear','<% tcWebApi_get("String_Entry", "CGIporttriggerClearText", "s")%>'],
				['porttriggerApp','<% tcWebApi_get("String_Entry", "QosApplicationText", "s")%>'],
				['porttriggerRag','<% tcWebApi_get("String_Entry", "PortTriggerRangeText", "s")%>'],																	
				['porttriggerOpen','<% tcWebApi_get("String_Entry", "PortTriggerOpeningRangeText", "s")%>'],
				['porttriggerSPort','<% tcWebApi_get("String_Entry", "PortTriggerStartPort3Text", "s")%>'],
				['porttriggerEPort','<% tcWebApi_get("String_Entry", "PortTriggerStartPort4Text", "s")%>'],
				['porttriggerProtocol','<% tcWebApi_get("String_Entry", "PortTriggerStartPort5Text", "s")%>'],									
				['porttriggerAim','<% tcWebApi_get("String_Entry", "PortTriggerApplication0Text", "s")%>'],
				['porttriggerAsh',"<% tcWebApi_get("String_Entry", "PortTriggerApplication1Text", "s")%>"],
				['porttriggerCal','<% tcWebApi_get("String_Entry", "PortTriggerApplication2Text", "s")%>'],
				['porttriggerDel','<% tcWebApi_get("String_Entry", "PortTriggerApplication3Text", "s")%>'],
				['porttriggerICQ','<% tcWebApi_get("String_Entry", "PortTriggerApplication4Text", "s")%>'],
				['porttriggerNap',"<% tcWebApi_get("String_Entry", "PortTriggerApplication5Text", "s")%>"],
				['porttriggerNet','<% tcWebApi_get("String_Entry", "PortTriggerApplication6Text", "s")%>'],
				['porttriggerQui','<% tcWebApi_get("String_Entry", "PortTriggerApplication7Text", "s")%>'],				
				['porttriggerRai','<% tcWebApi_get("String_Entry", "PortTriggerApplication8Text", "s")%>'],													
				['','']
			];
/******************End*************************/
</script>
</head>
<body>
<FORM METHOD="POST" name="PORTTRIGGERINGList_form">
<table width="590" border="0" align=center cellpadding="0" cellspacing="0">
	<tr>
		<td width="590" height="5" valign="baseline" class="orange">
		</td>
	</tr>
</table>
<table border="0" width="590" align=center cellspacing="0" cellpadding="0">
	<tr>
		<td width="200" height="50" align=center valign=middle bgcolor="#FFFFFF">
			<div align=left><img src="/logo.gif" width="200" height="50"></div>
		</td>
		<td width="500" align=right valign="bottom" bgcolor="#FFFFFF" class="model">
		</td>
	</tr>
</table>
<table width="590" height="2" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
	<tr>
		<td width="590" valign="baseline" class="orange">
		</td>
	</tr>
</table>
<table width="590" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="590" height="35"  bgcolor="#FFFFFF" class="headline">
			<%tcWebApi_get("String_Entry","PortTriggerlistSummaryText","s")%>
		</td>
	</tr>
</table>
<table width="590" border="0" height="250" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="590" align=center>
		<iframe src="/cgi-bin/porttriggering_list.cgi" frameborder="0" width="100%" height="100%" align=left scrolling="no"></iframe>
		</td>
	</tr>
</table>
<table width="590" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td class="orange" align=center>
			<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="doSave();">
			<INPUT TYPE="HIDDEN" NAME="submitFlag" id="submitFlag" VALUE="0">						
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr0" id="PortTriggeringList_Cntr0" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr1" id="PortTriggeringList_Cntr1" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr2" id="PortTriggeringList_Cntr2" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr3" id="PortTriggeringList_Cntr3" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr4" id="PortTriggeringList_Cntr4" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr5" id="PortTriggeringList_Cntr5" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr6" id="PortTriggeringList_Cntr6" VALUE="0">
			<INPUT TYPE="HIDDEN" NAME="PortTriggeringList_Cntr7" id="PortTriggeringList_Cntr7" VALUE="0">
		</td>
	</tr>
</table>
<p>&nbsp;</p>
</div>
</form>
</body>
</html>
