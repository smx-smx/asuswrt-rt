<%
	if request_Form("postflag") = "1" then
		tcWebApi_set("System_Entry","upgrade_fw","HTML_HEADER_TYPE")
		tcWebApi_CommitWithoutSave("System_Entry")
	end if	
%>		
<html>
<%if tcWebApi_get("System_Entry","upgrade_fw_status","h") <> "SUCCESS" then%>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" href="/style.css"  type="text/css">
<script language="JavaScript" src="/general.js"></script>
<script>
function uiDoUpdate()
{
	var form=document.uiPostUpdateForm;
	var string3 = form.tools_FW_UploadFile.value.search(/romfile/);
	var string4 = form.tools_FW_UploadFile.value.search(/tclinux/);

	if(form.tools_FW_UploadFile.value=="") {
		alert("<%tcWebApi_get("String_Entry","UpdateJS0Text","s")%>");
	}
	else if(form.fwNameChk.value == "Yes") {
		if (form.upload_type[0].checked)
			form.UG_filetype.value = "romfile";
		else if (form.upload_type[1].checked)
			form.UG_filetype.value = "tclinux";
		else {
			/* it will not come to here */
			alert("<%tcWebApi_get("String_Entry","UpdateJS1Text","s")%>");
			return;
		}
		form.uiStatus.value = "<%tcWebApi_get("String_Entry","UpdateJS2Text","s")%>";
		form.postflag.value = "1";
		form.submit();
	}
	else{
		if (((form.upload_type[0].checked) && (string3 >= 0)) || ((form.upload_type[1].checked) && (string4 >= 0))) {
			form.uiStatus.value = "<%tcWebApi_get("String_Entry","UpdateJS2Text","s")%>";
			form.postflag.value = "1";
			form.submit();
		}
		else
			alert("<%tcWebApi_get("String_Entry","UpdateJS1Text","s")%>");
	}
}

function backup_settings()
{
	var cfg = '/romfile.cfg';
	var code = 'location.assign("' + cfg + '")';
	eval(code);
}
</script>
</head>
<body>
<FORM ENCTYPE="multipart/form-data" METHOD="POST" name="uiPostUpdateForm">
<INPUT TYPE="HIDDEN" NAME="postflag" VALUE="1">
<INPUT TYPE="HIDDEN" NAME="HTML_HEADER_TYPE" VALUE="2">
<%if tcWebApi_get("WebCustom_Entry", "isFwNameNoChk", "h") = "Yes" then%>
<input type="hidden" name="UG_filetype" value="tclinux">
<%end if%>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<tr>
<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF"><%tcWebApi_get("String_Entry","UpdateUpgradeText","s")%></font></td><td width="10" class="black">&nbsp;</td><td width="150">&nbsp;</td><td width="10">&nbsp; </td><td width="440">&nbsp;</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000"></font></div></td><td class="tabdata"><div align=center></div></td><td class="tabdata">
<br>
<input name="upload_type" type="radio" value="1" CHECKED><%tcWebApi_get("String_Entry","UpdateRomfileText","s")%>
<input name="upload_type" type="radio" value="4"><%tcWebApi_get("String_Entry","UpdatebinText","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000"><%tcWebApi_get("String_Entry","UpdateNewFirmwareText","s")%></font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<INPUT TYPE="FILE" NAME="tools_FW_UploadFile" SIZE="30" MAXLENGTH="128"></td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000"><%tcWebApi_get("String_Entry","ButtonRomfileBakText","s")%></font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<input type="button" value="<%tcWebApi_get("String_Entry","ButtonRomfileBakText","s")%>" onClick='backup_settings()'></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000"><%tcWebApi_get("String_Entry","UpdateStatusText","s")%></font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<INPUT TYPE="TEXT" NAME="uiStatus" SIZE="55" MAXLENGTH="55" disabled VALUE="<% if tcWebApi_get("System_Entry","upgrade_fw_status","h") = "NONE" then asp_Write("") elseif tcWebApi_get("System_Entry","upgrade_fw_status","h") = "FAIL" then asp_Write("The file is illegel, please to upload again!!") end if%>" class="InputTextWarning">
</td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right><img src="/exclamation.gif"></div></td><td>&nbsp;</td><td class="tabdata">
<font color="#000000"><%tcWebApi_get("String_Entry","UpdateCommentText","s")%></font></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<tr><td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
<INPUT TYPE="BUTTON" NAME="FW_apply" VALUE="<%tcWebApi_get("String_Entry","ButtonUpgradeText","s")%>" class="sbutton"onClick="uiDoUpdate()"> 
<INPUT type=hidden name="fwNameChk" VALUE="<%tcWebApi_get("WebCustom_Entry", "isFwNameNoChk", "s")%>"></td></tr></table></form>
</body>
<%else%>
<%if tcwebApi_get("WebCustom_Entry", "isC2TrueSupported","h")="Yes" then%>
<head>
<meta http-equiv="Refresh" content="115"; url="tools_update.asp">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css"  type="text/css">
<style type="text/css">
.num 
{
	color: #FF9933;
	font-size: 8.0pt;
	font-weight: bold;
    font-family: Arial,Helvetica, sans-serif;
}
</style>
<script>
var pchar = "|";
var maxchars = 110;
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
<BODY onLoad="Init()">
<form METHOD="POST" name="UpgradingForm">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","UpdateUpgradeText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="600">&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=center><font color=red><%tcWebApi_get("String_Entry","UpdateSuccessfulMsgText","s")%></font></div></td>
	</tr>
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="15" class="light-orange"></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="600">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" class="light-orange"></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="600"><div align=center><input type="text" class="num" name="progress" size="61" value=""></div></td>
	</tr>
	<tr>
		<td width="150" height="2" class="light-orange"></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="600">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" class="light-orange"></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="600"><div align=center><input type="text" name="percent" size="2" value="">&nbsp;%</div></td>
	</tr>
	<tr>
		<td width="150" height="15" class="light-orange"></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="600">&nbsp;</td>
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
</BODY>
<%else%>
<HEAD><meta http-equiv="Refresh" content="115"; url="tools_update.asp">
<script language="JavaScript" src="/general.js"></script>
</HEAD>
<BODY><font color=red>
<script language="javascript">
document.writeln(_("UpdateSucc"));
</script>
</font></BODY>
<%end if%>
<%end if%>
</html>
