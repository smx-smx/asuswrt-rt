<%
If Request_Form("landingPageSubmitFlag") = "1" Then
	TCWebApi_set("GUI_Entry0","landingPageEnable","landingPageRadio")
	tcWebApi_commit("GUI_Entry0")
End If
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" href="/style.css" type="text/css">

<script language="JavaScript">
function uiSave() 
{
	document.landingPage.landingPageSubmitFlag.value = 1;
    document.landingPage.submit();
    return;
}
</script>
</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/tools_landingPage.asp" name="landingPage">
<INPUT TYPE="HIDDEN" NAME="landingPageSubmitFlag" VALUE="0">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main">
		<font color="#FFFFFF"><%tcWebApi_get("String_Entry","LandingPageText","s")%></font>
		</td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"></td>
		<td width="10"> </td>
		<td width="440"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"></td>
		<td class="tabdata"></td>
		<td class="tabdata"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right><font color="#000000"><%tcWebApi_get("String_Entry","LandingPageText","s")%></font></div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT NAME="landingPageRadio" TYPE="RADIO" VALUE="Yes" <% If TCWebApi_get("GUI_Entry0","landingPageEnable","h") = "Yes" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","LandingPage0Text","s")%></font>		
			<INPUT NAME="landingPageRadio" TYPE="RADIO" VALUE="No" <% If TCWebApi_get("GUI_Entry0","landingPageEnable","h") = "No" then asp_Write("checked") end if%>><font color="#000000"><%tcWebApi_get("String_Entry","LandingPage1Text","s")%></font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
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
			<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="uiSave()">        
			<INPUT TYPE="BUTTON" NAME="CancelBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>" onClick="javascript:window.location='/cgi-bin/tools_landingPage.asp'">        
		</td>
	</tr>
</table>
</div>
</form>
</body>
</html>

