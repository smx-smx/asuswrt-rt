<%
If Request_Form("PortTriggering_SubmitFlag") = "1" Then
	tcWebApi_set("GUITemp_Entry0","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry0","TSPortTmp","PortTriggering_StartPort_0")
	tcWebApi_set("GUITemp_Entry0","TEPortTmp","PortTriggering_EndPort_0")
	tcWebApi_set("GUITemp_Entry0","TProtocolTmp","PortTriggering_Protocol_0")
	tcWebApi_set("GUITemp_Entry0","OSPortTmp","PortOpening_StartPort_0")
	tcWebApi_set("GUITemp_Entry0","OEPortTmp","PortOpening_EndPort_0")
	tcWebApi_set("GUITemp_Entry0","OProtocolTmp","PortOpening_Protocol_0")
	
	tcWebApi_set("GUITemp_Entry1","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry1","TSPortTmp","PortTriggering_StartPort_1")
	tcWebApi_set("GUITemp_Entry1","TEPortTmp","PortTriggering_EndPort_1")
	tcWebApi_set("GUITemp_Entry1","TProtocolTmp","PortTriggering_Protocol_1")
	tcWebApi_set("GUITemp_Entry1","OSPortTmp","PortOpening_StartPort_1")
	tcWebApi_set("GUITemp_Entry1","OEPortTmp","PortOpening_EndPort_1")
	tcWebApi_set("GUITemp_Entry1","OProtocolTmp","PortOpening_Protocol_1")
	
	tcWebApi_set("GUITemp_Entry2","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry2","TSPortTmp","PortTriggering_StartPort_2")
	tcWebApi_set("GUITemp_Entry2","TEPortTmp","PortTriggering_EndPort_2")
	tcWebApi_set("GUITemp_Entry2","TProtocolTmp","PortTriggering_Protocol_2")
	tcWebApi_set("GUITemp_Entry2","OSPortTmp","PortOpening_StartPort_2")
	tcWebApi_set("GUITemp_Entry2","OEPortTmp","PortOpening_EndPort_2")
	tcWebApi_set("GUITemp_Entry2","OProtocolTmp","PortOpening_Protocol_2")
	
	tcWebApi_set("GUITemp_Entry3","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry3","TSPortTmp","PortTriggering_StartPort_3")
	tcWebApi_set("GUITemp_Entry3","TEPortTmp","PortTriggering_EndPort_3")
	tcWebApi_set("GUITemp_Entry3","TProtocolTmp","PortTriggering_Protocol_3")
	tcWebApi_set("GUITemp_Entry3","OSPortTmp","PortOpening_StartPort_3")
	tcWebApi_set("GUITemp_Entry3","OEPortTmp","PortOpening_EndPort_3")
	tcWebApi_set("GUITemp_Entry3","OProtocolTmp","PortOpening_Protocol_3")
	
	tcWebApi_set("GUITemp_Entry4","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry4","TSPortTmp","PortTriggering_StartPort_4")
	tcWebApi_set("GUITemp_Entry4","TEPortTmp","PortTriggering_EndPort_4")
	tcWebApi_set("GUITemp_Entry4","TProtocolTmp","PortTriggering_Protocol_4")
	tcWebApi_set("GUITemp_Entry4","OSPortTmp","PortOpening_StartPort_4")
	tcWebApi_set("GUITemp_Entry4","OEPortTmp","PortOpening_EndPort_4")
	tcWebApi_set("GUITemp_Entry4","OProtocolTmp","PortOpening_Protocol_4")
	
	tcWebApi_set("GUITemp_Entry5","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry5","TSPortTmp","PortTriggering_StartPort_5")
	tcWebApi_set("GUITemp_Entry5","TEPortTmp","PortTriggering_EndPort_5")
	tcWebApi_set("GUITemp_Entry5","TProtocolTmp","PortTriggering_Protocol_5")
	tcWebApi_set("GUITemp_Entry5","OSPortTmp","PortOpening_StartPort_5")
	tcWebApi_set("GUITemp_Entry5","OEPortTmp","PortOpening_EndPort_5")
	tcWebApi_set("GUITemp_Entry5","OProtocolTmp","PortOpening_Protocol_5")
	
	tcWebApi_set("GUITemp_Entry6","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry6","TSPortTmp","PortTriggering_StartPort_6")
	tcWebApi_set("GUITemp_Entry6","TEPortTmp","PortTriggering_EndPort_6")
	tcWebApi_set("GUITemp_Entry6","TProtocolTmp","PortTriggering_Protocol_6")
	tcWebApi_set("GUITemp_Entry6","OSPortTmp","PortOpening_StartPort_6")
	tcWebApi_set("GUITemp_Entry6","OEPortTmp","PortOpening_EndPort_6")
	tcWebApi_set("GUITemp_Entry6","OProtocolTmp","PortOpening_Protocol_6")
	
	tcWebApi_set("GUITemp_Entry7","NameTmp","PortTriggering_Applications")
	tcWebApi_set("GUITemp_Entry7","TSPortTmp","PortTriggering_StartPort_7")
	tcWebApi_set("GUITemp_Entry7","TEPortTmp","PortTriggering_EndPort_7")
	tcWebApi_set("GUITemp_Entry7","TProtocolTmp","PortTriggering_Protocol_7")
	tcWebApi_set("GUITemp_Entry7","OSPortTmp","PortOpening_StartPort_7")
	tcWebApi_set("GUITemp_Entry7","OEPortTmp","PortOpening_EndPort_7")
	tcWebApi_set("GUITemp_Entry7","OProtocolTmp","PortOpening_Protocol_7")
	tcWebApi_commit("portTriggering_Entry")
End If
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<SCRIPT language=javascript>
var ApplicationName = ["-" , "AimTalk" , "Asheron's Call" , "CalistaIPPhone" , "DeltaForce" , "ICQ" , "Napster" , "Net2Phone" , "QuickTimeClient" , "RainBow 6 Game" ];
var defaultArray = [
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var AimTalkArray = [
					[1 , 4099 ,4099 , 1 , 5191 , 5191 , 1],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var ACallArray = [
					[1 , 9000 ,9013 , 2 , 9000 , 9013 , 2],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var CalistaIPPhoneArray = [
					[1 , 5190 ,5190 , 1 , 3000 , 3000 , 1],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var DeltaForceArray = [
					[1 , 3568 ,3568 , 2 , 3100 , 3999 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var ICQArray = [
					[1 , 4000 ,4000 , 2 , 20000 , 20059 , 1],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var NapsterArray = [
					[1 , 6699 ,6699 , 1 , 6699 , 6699 , 1],
					[1 , 6699 ,6699 , 1 , 6697 , 6697 , 1],
					[1 , 6699 ,6699 , 1 , 4444 , 4444 , 1],
					[1 , 6699 ,6699 , 1 , 5555 , 5555 , 1],
					[1 , 6699 ,6699 , 1 , 6666 , 6666 , 1],
					[1 , 6699 ,6699 , 1 , 7777 , 7777 , 1],
					[1 , 6699 ,6699 , 1 , 8888 , 8888 , 1],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var Net2PhoneArray = [
					[1 , 6801 ,6801 , 2 , 6801 , 6801 , 2],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var QuickTimeArray = [
					[1 , 554 ,554 , 1 , 6990 , 32000 , 1],
					[1 , 554 ,554 , 1 , 6970 , 7000 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
var RainBow6GameArray = [
					[1 , 2346 ,2346 , 1 , 2346 , 2438 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0],
					[0 , 0 ,0 , 0 , 0 , 0 , 0]					
];
function RuleTableSetting(ApplicationArray)
{
	var i = 0;
	for(i = 0; i < 8; i++)
	{
		if(1 == ApplicationArray[i][0])
		{
			document.forms[0].elements[i*7+3].checked = true;	
		}
		else
		{
			document.forms[0].elements[i*7+3].checked = false;
		}

		document.forms[0].elements[i*7+4].value = ApplicationArray[i][1];
		document.forms[0].elements[i*7+5].value = ApplicationArray[i][2];
		document.forms[0].elements[i*7+6].selectedIndex = ApplicationArray[i][3];
		document.forms[0].elements[i*7+7].value = ApplicationArray[i][4];	
		document.forms[0].elements[i*7+8].value = ApplicationArray[i][5];
		document.forms[0].elements[i*7+9].selectedIndex = ApplicationArray[i][6];
	}
}

function ruleSetting(selectIndex)
{
	document.forms[0].PortTriggering_Applications.value = ApplicationName[selectIndex];
	switch(selectIndex)
	{
		case 0:
			RuleTableSetting(defaultArray);
			break;
		case 1:
			RuleTableSetting(AimTalkArray);
			break;
		case 2:
			RuleTableSetting(ACallArray);
			break;
		case 3:
			RuleTableSetting(CalistaIPPhoneArray);
			break;
		case 4:
			RuleTableSetting(DeltaForceArray);
			break;
		case 5:
			RuleTableSetting(ICQArray);
			break;
		case 6:
			RuleTableSetting(NapsterArray);
			break;
		case 7:
			RuleTableSetting(Net2PhoneArray);
			break;
		case 8:
			RuleTableSetting(QuickTimeArray);
			break;
		case 9:
			RuleTableSetting(RainBow6GameArray);
			break;
	}
}

function doAppChange(selectIndex) 
{
	ruleSetting(selectIndex);
}

function chkPortRange()
{
	active = 0;
	for(i = 0; i < 8; i++)
	{
		if(document.forms[0].elements[i*7+3].checked)
			active++;
		else
			continue;
		
		port1 = parseInt(document.forms[0].elements[i*7+4].value);
		port2 = parseInt(document.forms[0].elements[i*7+5].value);
		port3 = parseInt(document.forms[0].elements[i*7+7].value);
		port4 = parseInt(document.forms[0].elements[i*7+8].value);
		
		if (port1 > 65535 || port1 < 1 || port2 > 65535 || port2 < 1 ||
		port3 > 65535 || port3 < 1 || port4 > 65535 || port4 < 1)
		{
			alert("The Port value must position between 1 to 65535.");return true;
		}
		if (port2 < port1 || port4 < port3)
		{
			alert("End port must greater than Start port.");return true;
		}
	}
	
	if(!active)
	{
		alert("No rules are selected!!");
		return true;
	}
	
	if(active > document.forms[0].PortTriggering_CanUseNumFlag.value)
	{
		alert("Not enough available rules!!!");
		return true;
	}
	return false;
}

function quotationCheck(object, limit_len) 
{
	var len = object.value.length;
	var c;
	var i, j = 0;
	if(!len)
	{
		alert("No Application Name!");
		return true;
	}
	for (i = 0; i < len; i++)
	{
		var c = object.value.charAt(i);
		if (c == '"')
		{
			j += 6;
		}
		else
		{
			j++;	
		}

	}
	
	if (j > limit_len)
	{
		alert('too many quotation marks!');return true;
	}
	return false;
}

function doSave() 
{
	if (chkPortRange())
		return;
	if (quotationCheck(document.forms[0].PortTriggering_Applications, 15) )
		return;
	document.forms[0].PortTriggering_SubmitFlag.value = 1;
	document.forms[0].submit();
}

function doLoad() 
{
<% if Request_Form("PortTriggering_SubmitFlag") = "1" then %>
	top.main.location.href = "/cgi-bin/adv_nat_top.asp";
<% End If %>
	ruleSetting(document.forms[0].PortTriggering_App_idx.selectedIndex);
	return;
}
</SCRIPT>

<META content="MSHTML 6.00.2900.5726" name=GENERATOR></HEAD>
<BODY onLoad="doLoad();">
<FORM name=PORTTRIGGERING_form method=post>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD class=light-orange colSpan=5 height=5>&nbsp;</TD>
		</TR>
		<TR>
			<TD class=title-main width=150 height=30>
				<FONT color=#ffffff>Port Triggering</FONT>
			</TD>
			<TD class=black width=10>&nbsp;</TD>
			<TD width=150></TD>
			<TD width=10></TD>
			<TD width=440></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD class=light-orange width=150>&nbsp;</TD>
			<TD class=light-orange width=10></TD>
			<TD width=10>&nbsp;</TD>
			<TD class=tabdata align=middle width=580>
			<FONT color="black">Maximal number of rules: 8; Available rules</FONT>: 8</TD>
			<TD width=10>&nbsp;</TD>
		</TR>
		<TR>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=light-orange></TD>
			<TD>&nbsp;</TD>
			<TD>&nbsp;</TD>
			<TD width=10>&nbsp;</TD>
		</TR>
		<TR>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=light-orange></TD>
			<TD>&nbsp;</TD>
			<TD align=middle>
				<TABLE cellSpacing=0 cellPadding=0 width=520 bgColor=#ffffff>
					<TBODY>
						<TR>
							<TD class=tabdata align=middle>
								<FONT color=#000000>Application</FONT>&nbsp;:&nbsp; 
								<INPUT maxLength=15 size=15 name=PortTriggering_Applications>
								<SELECT onchange=doAppChange(this.selectedIndex) size=1 name=PortTriggering_App_idx>
								<OPTION selected>-
								<OPTION>Aim Talk
								<OPTION>Asheron's Call
								<OPTION>Calista IPPhone
								<OPTION>Delta Force
								<OPTION>ICQ
								<OPTION>Napster
								<OPTION>Net2Phone
								<OPTION>QuickTimeClient
								<OPTION>Rainbow 6 Game
								</OPTION>
								</SELECT>
							</TD>
						</TR>
					</TBODY>
				</TABLE>
			</TD>
			<TD width=10>&nbsp;</TD>
		</TR>
		<TR>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=light-orange></TD>
			<TD>&nbsp;</TD>
			<TD>&nbsp;</TD>
			<TD width=10>&nbsp;</TD>
		</TR>
		<TR>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=light-orange></TD>
			<TD>&nbsp;</TD>
			<TD align=middle><INPUT type=hidden value=8 name=PortTriggering_SubmitFlag>
				<TABLE borderColor=#cccccc cellSpacing=0 cellPadding=0 width=520 bgColor=#ffffff border=1>
				<TBODY>
					<TR height=30>
						<TD class=tabdata align=middle><STRONG>Active</STRONG></TD>
						<TD class=tabdata align=middle colSpan=3><STRONG><FONT color=#000000>Triggering Port Range</FONT></STRONG></TD>
						<TD class=tabdata align=middle colSpan=3><STRONG><FONT color=#000000>Opening Port Range</FONT></STRONG></TD>
					</TR>
					<TR height=30>
						<TD class=tabdata align=middle>&nbsp;</TD>
						<TD class=tabdata align=middle><STRONG><FONT color=#000000>Start Port</FONT></STRONG></TD>
						<TD class=tabdata align=middle><STRONG><FONT color=#000000>End Port</FONT></STRONG></TD>
						<TD class=tabdata align=middle><STRONG><FONT color=#000000>Protocol</FONT></STRONG></TD>
						<TD class=tabdata align=middle><STRONG><FONT color=#000000>Start Port</FONT></STRONG></TD>
						<TD class=tabdata align=middle><STRONG><FONT color=#000000>End Port</FONT></STRONG> </TD>
						<TD class=tabdata align=middle><STRONG><FONT color=#000000>Protocol</FONT></STRONG> </TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_0>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_0>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_0>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_0>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_0>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_0>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_0>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_1>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_1>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_1>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_1>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_1>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_1>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_1>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_2>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_2>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_2>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_2>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_2>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_2>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_2>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_3>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_3>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_3>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_3>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_3>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_3>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_3>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_4>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_4>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_4>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_4>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_4>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_4>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_4>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_5>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_5>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_5>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_5>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_5>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_5>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_5>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_6>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_6>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_6>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_6>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_6>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_6>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_6>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
					<TR>
						<TD class=tabdata align=middle>
							<INPUT type=checkbox name=PortTriggeringActive_7>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_StartPort_7>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortTriggering_EndPort_7>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortTriggering_Protocol_7>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_StartPort_7>
						</TD>
						<TD class=tabdata align=middle>
							<INPUT maxLength=5 size=5 value=0 name=PortOpening_EndPort_7>
						</TD>
						<TD class=tabdata align=middle>
							<SELECT size=1 name=PortOpening_Protocol_7>
								<OPTION selected value="TCP/UDP">TCP/UDP
								<OPTION value="TCP">TCP
								<OPTION value="UDP">UDP
								</OPTION>
							</SELECT>
						</TD>
					</TR>
				</TBODY>
				</TABLE>
			</TD>
			<TD width=10>&nbsp;</TD>
		</TR>
		<TR>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=light-orange></TD>
			<TD>&nbsp;</TD>
			<TD>&nbsp;</TD>
			<TD width=10>&nbsp;</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
	<TR>
		<TD class=black width=160 height=42>&nbsp;</TD>
		<TD class=orange width=160>&nbsp;</TD>
		<TD class=orange width=440>
			<INPUT onclick=doSave(); type=button value=SAVE name=SaveBtn>
			<INPUT onclick="javascript:window.location='/cgi-bin/adv_nat_top.asp'" type=button value=BACK name=BackBtn>
			<input type="reset" name="Reset" value="Cancel">
			<input type=hidden value="<%if TCWebAPI_get("portTriggering_setting", "canUseNum", "h") <> "N/A" then TCWebAPI_staticGet("portTriggering_setting", "canUseNum", "s") else asp_Write("8") end if %>" name=PortTriggering_CanUseNumFlag>
		</TD>
	</TR>
	</TBODY>
</TABLE>
<P>&nbsp;</P>
<DIV></DIV>
</FORM>
</BODY>
</HTML>
