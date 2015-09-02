<%
	if request_Form("postflag") = "1" then
		tcWebApi_set("System_Entry","upgrade_fw","HTML_HEADER_TYPE")
		tcWebApi_Commit("System_Entry")
	end if	
%>		
<html>
<%if tcWebApi_get("System_Entry","upgrade_fw_status","h") <> "SUCCESS" then%>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css"  type="text/css">
<script>
function uiDoUpdate()
{
	var string3 = document.uiPostUpdateForm.tools_FW_UploadFile.value.search(/romfile/);
	var string4 = document.uiPostUpdateForm.tools_FW_UploadFile.value.search(/tclinux/);
	
	if(document.uiPostUpdateForm.tools_FW_UploadFile.value=="")
	{
		alert("You must select a file to update.");
	}
	else if((string3 >= 0 ) || (string4 >= 0 ))
	{
		document.uiPostUpdateForm.uiStatus.value = "Upgrade in progress, Please wait...";
		document.uiPostUpdateForm.postflag.value = "1";
		document.uiPostUpdateForm.submit();
	}
	else
	{
		alert("you put a wrong file.");
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
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main">
		<font color="#FFFFFF">Firmware Upgrade</font>
		</td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150">&nbsp;</td>
		<td width="10">&nbsp; </td>
		<td width="440">&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata">
		<div align=right><font color="#000000"></font></div>
		</td>
		<td class="tabdata">
		<div align=center></div>
		</td>
		<td class="tabdata">
		<br>
		<input name="upload_type" type="radio" value="1" CHECKED>romfile
		<input name="upload_type" type="radio" value="4">tclinux.bin
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata">
		<div align=right><font color="#000000">New Firmware Location</font></div>
		</td>
		<td class="tabdata">
		<div align=center>:</div>
		</td>
		<td class="tabdata">
		<INPUT TYPE="FILE" NAME="tools_FW_UploadFile" SIZE="30" MAXLENGTH="128">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata">
		<div align=right><font color="#000000">Romfile Backup</font></div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<input type="button" value="ROMFILE BACKUP" onClick='backup_settings()'>
		</td>
	</tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td class="tabdata">
		<div align=right><font color="#000000">Status</font></div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="uiStatus" SIZE="55" MAXLENGTH="55" VALUE="<% if tcWebApi_get("System_Entry","upgrade_fw_status","h") = "NONE" then asp_Write("") elseif tcWebApi_get("System_Entry","upgrade_fw_status","h") = "FAIL" then asp_Write("The file is illegel, please to upload again!!") end if%>" class="InputTextWarning">
		</td>
	</tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><img src="/exclamation.gif"></div></td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<font color="#000000">It might take several minutes, don't power off it during upgrading. Device will restart after the upgrade.</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td>
		<td width="160" class="orange">&nbsp;</td>
		<td width="440" class="orange">
		<INPUT TYPE="BUTTON" NAME="FW_apply" VALUE="UPGRADE" class="sbutton"onClick="uiDoUpdate()"> 
		</td>
	</tr>
</table>
</form>
</body>
<%else%>
<HEAD><meta http-equiv="Refresh" content="115"; url="tools_update.asp"></HEAD>
<BODY><font color=red>File upload succeeded, starting flash erasing and programming!!</font></BODY>
<%end if%>
</html>
