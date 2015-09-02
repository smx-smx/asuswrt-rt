<%
If Request_Form("rebootFlag") = "1"  Then 
	TCWebApi_set("System_Entry","reboot_type","restoreFlag") 
	tcWebApi_CommitWithoutSave("System_Entry")
End If	
%>

<!--
If you execute doRefresh() and then reload webpage, doRefresh() will be disabled.
Therefore, execute doRefresh() after webpage reloads.
-->
<%if tcwebApi_get("WebCustom_Entry","isC2TrueSupported","h")<>"Yes" then%>
<%If Request_Form("RestartBtn") = "RESTART"  Then%>
<script language="JavaScript">
	setTimeout("doRefresh()",70000);
</script>
<%End If%>
<%End If%>

<html>
<%if tcwebApi_get("WebCustom_Entry","isC2TrueSupported","h")<>"Yes" then%>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript">

function restart()
{
	if(document.tools_System_Restore.restoreFlag[0].checked)
		alert("Restart with current settings! Please wait for reboot complete!");
	if(document.tools_System_Restore.restoreFlag[1].checked)
		alert("Restart with factory default settings! Please wait for reboot complete!");
	document.tools_System_Restore.rebootFlag.value=1;
	document.tools_System_Restore.submit();
}
	
function doRefresh()
{
	alert("Reboot Complete");
	var mydate = new Date();
	mydate.setTime(mydate.getTime() - 1);
	document.cookie = "SESSIONID=; expires=" + mydate.toGMTString(); 
	window.top.location.href = "../";    
}

</script>
</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/tools_system.asp" name="tools_System_Restore">
<INPUT TYPE="HIDDEN" NAME="testFlag" VALUE="0">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">System Restart</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"> </td><td width="440"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">System Restart with</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT TYPE="RADIO" NAME="restoreFlag" VALUE="1" CHECKED>      	<font color="#000000">Current Settings</font>		</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata">&nbsp;</td><td class="tabdata">&nbsp;</td><td class="tabdata">
		<INPUT TYPE="RADIO" NAME="restoreFlag" VALUE="2">		<font color="#000000">Factory Default Settings</font>		</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150">&nbsp;</td><td width="10">&nbsp;</td><td width="440">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="rebootFlag" value="0">  
		<INPUT TYPE="SUBMIT" NAME="RestartBtn" VALUE="RESTART" onClick="restart();">	</td></tr></table></div></form>
</body>

<%end if%>

<%if tcwebApi_get("WebCustom_Entry","isC2TrueSupported","h")="Yes" then%>
<%if TCWebApi_get("System_Entry","reboot_type","h") ="0"  then%>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript">

function restart()
{
	if(document.tools_System_Restore.C2UserMode.value ==0)
	{
	if(document.tools_System_Restore.restoreFlag[0].checked)
		alert("Restart with current settings! Please wait for reboot complete!");
	if(document.tools_System_Restore.restoreFlag[1].checked)
		alert("Restart with factory default settings! Please wait for reboot complete!");
	}
	else
	{
		if(document.tools_System_Restore.restoreFlag.value == 1)
			alert("Restart with current settings! Please wait for reboot complete!");
	}
	document.tools_System_Restore.rebootFlag.value=1;
	document.tools_System_Restore.submit();
}
</script>
</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/tools_system.asp" name="tools_System_Restore">
<INPUT TYPE="HIDDEN" NAME="testFlag" VALUE="0">
<Input type="hidden" name="C2UserMode" value="<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then asp_write("0") else asp_write("1") end if%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">System Restart</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"> </td><td width="440"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">System Restart with</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT TYPE="RADIO" NAME="restoreFlag" VALUE="1" CHECKED>      	<font color="#000000">Current Settings</font>		</td></tr>
	<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
	<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td>
	<td class="tabdata">&nbsp;</td><td class="tabdata">&nbsp;</td>
	<td class="tabdata"><INPUT TYPE="RADIO" NAME="restoreFlag" VALUE="2"><font color="#000000">Factory Default Settings</font></td>
	</tr>
	<%end if%>
	<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150">&nbsp;</td><td width="10">&nbsp;</td><td width="440">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<INPUT TYPE="HIDDEN" NAME="rebootFlag" value="0">
	    <INPUT TYPE="SUBMIT" NAME="RestartBtn" VALUE="RESTART" onClick="restart();">	</td></tr></table></div></form>
</body>

<%else%>

<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<style type="text/css">
.num 
{
	color: #FF9933;
	font-size: 8.0pt;
	font-weight: bold;
    font-family: Arial,Helvetica, sans-serif;
}
</style>
<script language="JavaScript">
var pchar = "|";
var maxchars = 100;
var delay_time = 1000;
var charcount = 0;  
function Init()
{
	var formName = document.forms[0];
	if (charcount < maxchars)
	{
		charcount ++;
		formName.progress.value = makeStr(charcount,pchar);
		formName.percent.value=Math.floor(charcount/maxchars*100);
		setTimeout("Init()",delay_time);		
	}
	else
	{
		window.top.location.href = "../"; 
	} 
}
function makeStr(strSize, fillChar)
{
	var temp = "";
	for (i=0; i < strSize ; i ++)
		temp = temp + fillChar;
	return temp;
}

</script>
</head>
<body onLoad="Init();">
<FORM METHOD="POST" ACTION="/cgi-bin/tools_system.asp" name="tools_System_Restore">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		 <tr>
			  <td height="5" class="light-orange" colspan="5">&nbsp;</td>
		 </tr>
		 <tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF">System Restart</font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="600"></td>
		</tr>
		<tr>
			 <td class="light-orange">&nbsp;</td>
			 <td class="light-orange"></td>
			 <td class="tabdata"><div align=center><font color=red>System is rebooting!!</font></div></td>
			 </tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=center><input type="text" class="num" name="progress" size="55" value=""></div></td>
		</tr>
		<tr>
			<td height="2" class="light-orange"></td>
			<td class="light-orange">&nbsp;</td>
			<td class="tabdata">&nbsp;</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=center><input type="text" name="percent" size="2" value="">&nbsp;%</div></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange">&nbsp;</td>
			<td class="tabdata">&nbsp;</td>
		</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	  <tr>	
		<td width="160" class="black" height="42" >&nbsp;</td>
		<td width="160" class="orange">&nbsp;</td>
		<td width="440" class="orange"></td>
	  </tr>
	</table>
</form>

</body>
<%end if%>
<%end if%>
</html>

