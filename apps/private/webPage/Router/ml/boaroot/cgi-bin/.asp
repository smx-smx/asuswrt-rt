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

var	ary_langInfo = [
	["Aim Talk", "<%tcWebApi_get("String_Entry","PortTriggerApplication0Text","s")%>"],
	["Asheron's Call", "<%tcWebApi_get("String_Entry","PortTriggerApplication1Text","s")%>"],
	["CalistaIPPhone", "<%tcWebApi_get("String_Entry","PortTriggerApplication2Text","s")%>"],
	["DeltaForce", "<%tcWebApi_get("String_Entry","PortTriggerApplication3Text","s")%>"],
	["ICQ", "<%tcWebApi_get("String_Entry","PortTriggerApplication4Text","s")%>"],
	["Napster", "<%tcWebApi_get("String_Entry","PortTriggerApplication5Text","s")%>"],
	["Net2Phone", "<%tcWebApi_get("String_Entry","PortTriggerApplication6Text","s")%>"],
	["QuickTimeClient", "<%tcWebApi_get("String_Entry","PortTriggerApplication7Text","s")%>"],
	["RainBow 6 Game", "<%tcWebApi_get("String_Entry","PortTriggerApplication8Text","s")%>"]
];
var	ary_triggerInfo = [
	["<% tcWebApi_get("portTriggering_Entry0","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry0","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry0","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry0","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry0","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry0","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry0","OProtocol","s") %>"],
	["<% tcWebApi_get("portTriggering_Entry1","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry1","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry1","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry1","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry1","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry1","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry1","OProtocol","s") %>"],
	["<% tcWebApi_get("portTriggering_Entry2","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry2","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry2","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry2","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry2","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry2","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry2","OProtocol","s") %>"],
	["<% tcWebApi_get("portTriggering_Entry3","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry3","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry3","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry3","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry3","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry3","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry3","OProtocol","s") %>"],
	["<% tcWebApi_get("portTriggering_Entry4","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry4","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry4","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry4","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry4","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry4","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry4","OProtocol","s") %>"],
	["<% tcWebApi_get("portTriggering_Entry5","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry5","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry5","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry5","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry5","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry5","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry5","OProtocol","s") %>"],
	["<% tcWebApi_get("portTriggering_Entry6","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry6","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry6","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry6","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry6","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry6","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry6","OProtocol","s") %>"],
	["<% tcWebApi_get("portTriggering_Entry7","Name","s") %>", "<% tcWebApi_get("portTriggering_Entry7","TSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry7","TEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry7","TProtocol","s") %>", "<% tcWebApi_get("portTriggering_Entry7","OSPort","s") %>", "<% tcWebApi_get("portTriggering_Entry7","OEPort","s") %>", "<% tcWebApi_get("portTriggering_Entry7","OProtocol","s") %>"]
];
function stPorttrigger(name, tsport, teport, tprotl, osport, oeport, oproto)
{
	this.name = name;
	if ("N/A" == name)
		this.name = "-";
	else {
		this.name = name;
		for(var i=0; i<ary_langInfo.length; i++)
		{
			if (name == ary_langInfo[i][0]){
				this.name = ary_langInfo[i][1];
				break;
			}
		}
	}
	
	this.tsport = tsport;
	if ("N/A" == tsport)
		this.tsport = 0;
		
	this.teport = teport;
	if ("N/A" == teport)
		this.teport = 0;
		
	this.tprotl = tprotl;
	if ("N/A" == tprotl)
		this.tprotl = "-";
		
	this.osport = osport;
	if ("N/A" == osport)
		this.osport = 0;
		
	this.oeport = oeport;
	if ("N/A" == oeport)
		this.oeport = 0;
		
	this.oproto = oproto;
	if ("N/A" == oproto)
		this.oproto = "-";
}
var trigger_obj = new Array(8);
for (var i=0; i<ary_triggerInfo.length; i++) {
	trigger_obj[i] = new stPorttrigger(ary_triggerInfo[i][0], ary_triggerInfo[i][1], ary_triggerInfo[i][2], ary_triggerInfo[i][3], ary_triggerInfo[i][4], ary_triggerInfo[i][5], ary_triggerInfo[i][6]);
}

function writeTriggerTable()
{
	var strtemp = "";
	for(var i=0;  i<trigger_obj.length; i++)
	{
		
	}
}
function CtrClickSetting(index,checked)
{ 
	var f = document.PORTTRIGGERINGList_form; 
	switch(index)
	{ 
		case 0:
			if(checked) f.PortTriggeringList_Cntr0.value = 1;
			else f.PortTriggeringList_Cntr0.value = 0; 
			break; 
		case 1: 
			if(checked) f.PortTriggeringList_Cntr1.value = 1; 
			else f.PortTriggeringList_Cntr1.value = 0; 
			break; 
		case 2: 
			if(checked) f.PortTriggeringList_Cntr2.value = 1; 
			else f.PortTriggeringList_Cntr2.value = 0; 
			break; 
		case 3: 
			if(checked) f.PortTriggeringList_Cntr3.value = 1; 
			else f.PortTriggeringList_Cntr3.value = 0; 
			break; 
		case 4: 
			if(checked) f.PortTriggeringList_Cntr4.value = 1; 
			else f.PortTriggeringList_Cntr4.value = 0; 
			break; 
		case 5: 
			if(checked) f.PortTriggeringList_Cntr5.value = 1; 
			else f.PortTriggeringList_Cntr5.value = 0;
			break; 
		case 6: 
			if(checked) f.PortTriggeringList_Cntr6.value = 1; 
			else f.PortTriggeringList_Cntr6.value = 0; 
			break; 
		case 7: 
			if(checked) f.PortTriggeringList_Cntr7.value = 1; 
			else f.PortTriggeringList_Cntr7.value = 0; 
			break; 
	}
}

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
		<!--iframe src="/cgi-bin/porttriggering_list.cgi" frameborder="0" width="100%" height="100%" align=left scrolling="no"></iframe-->
		<table width="100%" height="100%" border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
		<tr height="10">
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","CGIporttriggerClearText","s")%></font></strong><td>
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerApplicationText","s")%></font></strong></td>
			<td colspan="3" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerRangeText","s")%></font></strong></td>
			<td colspan="3" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerOpeningRangeText","s")%></font></strong></td>
		</tr>
		<tr>
			<td align=center class="tabdata">&nbsp;</td>
			<td align=center class="tabdata">&nbsp;</td>
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerStartPort0Text","s")%></font></strong></td>
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerStartPort1Text","s")%></font></strong></td>
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerStartPort2Text","s")%></font></strong></td>
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerStartPort0Text","s")%></font></strong></td>
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerStartPort1Text","s")%></font></strong></td>
			<td align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","PortTriggerStartPort2Text","s")%></font></strong></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear0" onClick="CtrClickSetting(0,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[0][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[0][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[0][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[0][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[0][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[0][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[0][6])</script></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear1" onClick="CtrClickSetting(1,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[1][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[1][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[1][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[1][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[1][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[1][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[1][6])</script></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear2" onClick="CtrClickSetting(2,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[2][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[2][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[2][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[2][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[2][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[2][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[2][6])</script></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear3" onClick="CtrClickSetting(3,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[3][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[3][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[3][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[3][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[3][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[3][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[3][6])</script></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear4" onClick="CtrClickSetting(4,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[4][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[4][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[4][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[4][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[4][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[4][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[4][6])</script></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear5" onClick="CtrClickSetting(5,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[5][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[5][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[5][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[5][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[5][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[5][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[5][6])</script></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear6" onClick="CtrClickSetting(6,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[6][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[6][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[6][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[6][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[6][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[6][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[6][6])</script></td>
		</tr>
		<tr>
			<td align="center" class="tabdata"><INPUT TYPE="CHECKBOX" NAME="PortTriggeringListClear7" onClick="CtrClickSetting(7,this.checked);"></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[7][0])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[7][1])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[7][2])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[7][3])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[7][4])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[7][5])</script></td>
			<td align="center" class="tabdata"><script>document.writeln(trigger_obj[7][6])</script></td>
		</tr>
		</table>
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
