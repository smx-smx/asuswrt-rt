<%
	if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "N/A" then  
			tcWebApi_constSet("WebCurSet_Entry", "timeOfDay_entry", "0")
	end if
	If Request_Form("submitFlag") = "1" Then
		tcWebApi_set("WebCurSet_Entry", "timeOfDay_entry", "Sel_Rule")
		tcWebApi_set("timeOfDay_setting", "Active", "TimeofDay_status")
		tcWebApi_set("timeOfDay_Entry","all","allFlag")
		tcWebApi_set("timeOfDay_Entry","msn","msnFlag")
		tcWebApi_set("timeOfDay_Entry","http","httpFlag")
		tcWebApi_set("timeOfDay_Entry","ftp","ftpFlag")
		tcWebApi_set("timeOfDay_Entry","url","urlFlag")
		tcWebApi_set("timeOfDay_Entry","clock","clockFlag")
		tcWebApi_commit("timeOfDay_Entry")
	elseif Request_Form("submitFlag") = "2" Then
		tcWebApi_set("WebCurSet_Entry", "timeOfDay_entry", "Sel_Rule")	
	End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<SCRIPT language=JavaScript>
function doRuleChange(index)
{
	document.timeofDayForm.submitFlag.value = 2;
	document.timeofDayForm.submit();
	return;
}

function cleckTheClockFromNode()
{
	var i =0;
	var cleckedFlag = 0;
	var TimeofDayClock = "TimeofDay_0_clock";
	var clockFlag = 0x<% tcWebApi_get("timeOfDay_Entry","clock","s") %>;
	
	for(i = 0; i < 24; i++)
	{
		cleckedFlag = clockFlag&(1 << i);
		TimeofDayClock = "TimeofDay_" + i + "_clock";
		if(0 != cleckedFlag)
		{
			document.getElementById(TimeofDayClock).checked = true;			
		}	
	}
	return;
}

function setClockFlag2Node()
{
	var i =0;
	var clockFlag = 0;
	var TimeofDayClock = "TimeofDay_0_clock";

	for(i = 0; i < 24; i++)
	{
		TimeofDayClock = "TimeofDay_" + i + "_clock";		
		if(document.getElementById(TimeofDayClock).checked)
		{
			clockFlag = clockFlag|(1 << i);
		}	
	}
	
	document.timeofDayForm.clockFlag.value = clockFlag.toString(16);
	return;
}

function applicationSet()
{		
	if(document.timeofDayForm.TimeofDay_App_all_check.checked)
		document.timeofDayForm.allFlag.value = "on";
	if(document.timeofDayForm.TimeofDay_App_Msn_check.checked)
		document.timeofDayForm.msnFlag.value = "on";
	if(document.timeofDayForm.TimeofDay_App_Http_check.checked)
		document.timeofDayForm.httpFlag.value = "on";
	if(document.timeofDayForm.TimeofDay_App_Ftp_check.checked)
		document.timeofDayForm.ftpFlag.value = "on";
	if(document.timeofDayForm.TimeofDay_URL_Filter_check.checked)
		document.timeofDayForm.urlFlag.value = "on";
			
	return;
}

function SaveTimeofDay()
{
	applicationSet();
	setClockFlag2Node();
	document.timeofDayForm.submitFlag.value = 1;
	document.timeofDayForm.submit();
}

function RunTimeofDay_KeywordList()
{
	window.open("/cgi-bin/keyword.asp","url_filter_keywords","scrollbars=yes,resizable=yes,toolbar=no,menubar=no,height=305, width=620,location=0,left=200,top=200");
	return false;
}

function doLoad()
{
	cleckTheClockFromNode();
	return;
}
</SCRIPT>

<META content="MSHTML 6.00.2900.5726" name=GENERATOR></HEAD>
<BODY onLoad="doLoad();">
<FORM name=timeofDayForm  ACTION="/cgi-bin/access_timeofday.asp" method=post>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD class=light-orange colSpan=5 height=5>&nbsp;</TD>
		</TR>
		<TR>
			<TD class=title-main width=150 height=30><FONT color=#ffffff><FONT color=#ffffff><%tcWebApi_get("String_Entry","TimeofdayControlText","s")%></FONT></FONT></TD>
			<TD class=black width=10>&nbsp;</TD>
			<TD width=150></TD>
			<TD width=11></TD>
			<TD width=439></TD>
		</TR>
		<TR>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=light-orange></TD>
			<TD class=tabdata><DIV align=right><FONT color=#000000><FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayStatusText","s")%></FONT></FONT></DIV></TD>
			<TD class=tabdata><DIV align=center>:</DIV></TD>
			<TD class=tabdata>
			<INPUT type=radio value=1 name=TimeofDay_status <%if tcWebApi_get("timeOfDay_setting", "Active", "h") = "1" then asp_Write("checked") end if%>><FONT color=#000000> <FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayActivatedText","s")%></FONT></FONT> 
			<INPUT type=radio value=0 name=TimeofDay_status <%if tcWebApi_get("timeOfDay_setting", "Active", "h") = "0" then asp_Write("checked") elseif tcWebApi_get("timeOfDay_setting", "Active", "h") = "N/A" then asp_Write("checked") end if%>><FONT color=#000000> <FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayDeactivatedText","s")%></FONT></FONT></TD>
		</TR>
		<TR>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=light-orange>&nbsp;</TD>
			<TD class=tabdata><DIV align=right><P><FONT color=#000000><FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayRuleText","s")%></FONT></FONT></P></DIV></TD>
			<TD class=tabdata><DIV align=center>:</DIV></TD>
			<TD class=tabdata>
			<SELECT onchange=doRuleChange(this.selectedIndex) size=1 name=Sel_Rule>
				<OPTION value="0" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "0" then  asp_Write("selected") end if%>>0
				<OPTION value="1" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "1" then  asp_Write("selected") end if%>>1
				<OPTION value="2" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "2" then  asp_Write("selected") end if%>>2
				<OPTION value="3" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "3" then  asp_Write("selected") end if%>>3
				<OPTION value="4" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "4" then  asp_Write("selected") end if%>>4
				<OPTION value="5" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "5" then  asp_Write("selected") end if%>>5
				<OPTION value="6" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "6" then  asp_Write("selected") end if%>>6
				<OPTION value="7" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "7" then  asp_Write("selected") end if%>>7
				<OPTION value="8" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "8" then  asp_Write("selected") end if%>>8
				<OPTION value="9" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "9" then  asp_Write("selected") end if%>>9
				<OPTION value="10" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "10" then  asp_Write("selected") end if%>>10
				<OPTION value="11" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "11" then  asp_Write("selected") end if%>>11
				<OPTION value="12" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "12" then  asp_Write("selected") end if%>>12
				<OPTION value="13" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "13" then  asp_Write("selected") end if%>>13
				<OPTION value="14" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "14" then  asp_Write("selected") end if%>>14
				<OPTION value="15" <%if tcWebApi_get("WebCurSet_Entry","timeOfDay_entry","h") = "15" then  asp_Write("selected") end if%>>15
				</OPTION>
			</SELECT>
			<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayCommentText","s")%></FONT>
			</TD>
		</TR>
		<TR>
			<TD class=title-sub><FONT color=#000000><FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayApplicationText","s")%></FONT></FONT></TD>
			<TD class=light-orange>&nbsp;</TD>
			<TD><HR noShade></TD>
			<TD><HR noShade></TD>
			<TD><HR noShade></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0 dwcopytype="CopyTableRow">
	<TBODY>
		<TR>
			<TD class=light-orange width=146><DIV align=center></DIV></TD>
			<TD class=light-orange width=14><DIV align=center></DIV></TD>
			<TD class=tabdata width=81>
				<INPUT type=checkbox value="No" name=TimeofDay_App_all_check <%if tcWebApi_get("timeOfDay_Entry","all","h") = "on" then  asp_Write("checked") end if%>>
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayAllText","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox value="No" name=TimeofDay_App_Msn_check <%if tcWebApi_get("timeOfDay_Entry","msn","h") = "on" then  asp_Write("checked") end if%>>
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdaymsnText","s")%></FONT>
			</TD>
			<TD class=tabdata width=73>
				<INPUT type=checkbox value="No" name=TimeofDay_App_Http_check <%if tcWebApi_get("timeOfDay_Entry","http","h") = "on" then  asp_Write("checked") end if%>>
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayhttpText","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox value="No" name=TimeofDay_App_Ftp_check <%if tcWebApi_get("timeOfDay_Entry","ftp","h") = "on" then  asp_Write("checked") end if%>>
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayftpText","s")%></FONT>
			</TD>
			<TD class=tabdata width=82>&nbsp; </TD>
			<TD class=tabdata width=131>&nbsp; </TD>
			<TD width=73></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0 dwcopytype="CopyTableRow">
	<TBODY>
		<TR>
			<TD class=light-orange width=146><DIV align=center></DIV></TD>
			<TD class=light-orange width=14><DIV align=center></DIV></TD>
			<TD class=tabdata width=81>
				<INPUT type=checkbox value="No" name=TimeofDay_URL_Filter_check <%if tcWebApi_get("timeOfDay_Entry","url","h") = "on" then  asp_Write("checked") end if%>>
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayURLFilterText","s")%></FONT>
			</TD>
			<TD class=tabdata width=153>
				<INPUT onclick=RunTimeofDay_KeywordList() type=button value="<%tcWebApi_get("String_Entry","ButtonEditKeywordText","s")%>" name=TimeofDay_EditKeyword>
			</TD>
			<TD class=tabdata width=80></TD>
			<TD class=tabdata width=82></TD>
			<TD class=tabdata width=131></TD>
			<TD width=73></TD>
		</TR>
	</TBODY>
</TABLE>
<DIV align=center>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD class=title-sub width=148><FONT color=#000000><FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTimeRulesText","s")%></FONT></FONT></TD>
			<TD class=light-orange width=12>&nbsp;</TD>
			<TD width=149><HR noShade></TD>
			<TD width=12><HR noShade></TD>
			<TD width=439><HR noShade></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD class=light-orange width=146>
			  <DIV align=center></DIV></TD>
			<TD class=light-orange width=14>
			  <DIV align=center></DIV></TD>
			<TD class=tabdata width=81>
				<INPUT type=checkbox name=TimeofDay_0_clock id="TimeofDay_0_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime0Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_1_clock id="TimeofDay_1_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime1Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=73>
				<INPUT type=checkbox name=TimeofDay_2_clock id="TimeofDay_2_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime2Text","s")%></FONT>
			  </TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_3_clock id="TimeofDay_3_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime3Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=82>
				<INPUT type=checkbox name=TimeofDay_4_clock id="TimeofDay_4_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime4Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=131>
				<INPUT type=checkbox name=TimeofDay_5_clock id="TimeofDay_5_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime5Text","s")%></FONT>
			</TD>
			<TD width=73> </TD>
		</TR>
	</TBODY>
	<TBODY>
		<TR>
			<TD class=light-orange width=146>
			  <DIV align=center></DIV></TD>
			<TD class=light-orange width=14>
			  <DIV align=center></DIV></TD>
			<TD class=tabdata width=81>
				<INPUT type=checkbox name=TimeofDay_6_clock id="TimeofDay_6_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime6Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_7_clock id="TimeofDay_7_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime7Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=73>
				<INPUT type=checkbox name=TimeofDay_8_clock id="TimeofDay_8_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime8Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_9_clock id="TimeofDay_9_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime9Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=82>
				<INPUT type=checkbox name=TimeofDay_10_clock id="TimeofDay_10_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime10Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=131>
				<INPUT type=checkbox name=TimeofDay_11_clock id="TimeofDay_11_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime11Text","s")%></FONT>
			</TD>
			<TD width=73> </TD>
		</TR>
	</TBODY>
	<TBODY>
		<TR>
			<TD class=light-orange width=146>
			  <DIV align=center></DIV></TD>
			<TD class=light-orange width=14>
			  <DIV align=center></DIV></TD>
			<TD class=tabdata width=81>
				<INPUT type=checkbox name=TimeofDay_12_clock id="TimeofDay_12_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime12Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_13_clock id="TimeofDay_13_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime13Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=73>
				<INPUT type=checkbox name=TimeofDay_14_clock id="TimeofDay_14_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime14Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_15_clock id="TimeofDay_15_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime15Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=82>
				<INPUT type=checkbox name=TimeofDay_16_clock id="TimeofDay_16_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime16Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=131>
				<INPUT type=checkbox name=TimeofDay_17_clock id="TimeofDay_17_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime17Text","s")%></FONT>
			</TD>
			<TD width=73> </TD>
		</TR>
	</TBODY>
	<TBODY>
		<TR>
			<TD class=light-orange width=146>
			  <DIV align=center></DIV></TD>
			<TD class=light-orange width=14>
			  <DIV align=center></DIV></TD>
			<TD class=tabdata width=81>
				<INPUT type=checkbox name=TimeofDay_18_clock id="TimeofDay_18_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime18Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_19_clock id="TimeofDay_19_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime19Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=73>
				<INPUT type=checkbox name=TimeofDay_20_clock id="TimeofDay_20_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime20Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=80>
				<INPUT type=checkbox name=TimeofDay_21_clock id="TimeofDay_21_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime21Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=82>
				<INPUT type=checkbox name=TimeofDay_22_clock id="TimeofDay_22_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime22Text","s")%></FONT>
			</TD>
			<TD class=tabdata width=131>
				<INPUT type=checkbox name=TimeofDay_23_clock id="TimeofDay_23_clock">
				<FONT color=#000000><%tcWebApi_get("String_Entry","TimeofdayTime23Text","s")%></FONT>
			</TD>
			<TD width=73> </TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD class=light-orange width=150>&nbsp;</TD>
			<TD class=light-orange width=10></TD>
			<TD class=tabdata width=150>&nbsp;</TD>
			<TD width=10>&nbsp;</TD>
			<TD class=tabdata width=440>&nbsp;</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD class=black width=160 height=42>&nbsp;</TD>
			<TD class=orange width=160>&nbsp;</TD>
			<TD class=orange width=440>
				<INPUT type=hidden value=0 name=submitFlag>
				<INPUT type=hidden value=0 name=clockFlag>
				<INPUT type=hidden value="off" name=allFlag>
				<INPUT type=hidden value="off" name=msnFlag>
				<INPUT type=hidden value="off" name=httpFlag>
				<INPUT type=hidden value="off" name=ftpFlag>
				<INPUT type=hidden value="off" name=urlFlag>
				<INPUT onclick=SaveTimeofDay(); type=button value=<%tcWebApi_get("String_Entry","ButtonSaveText","s")%> name=TimeOfDay_SaveBtn>
			</TD>
		</TR>
	</TBODY>
</TABLE>
</FORM>
</BODY>
</HTML>
