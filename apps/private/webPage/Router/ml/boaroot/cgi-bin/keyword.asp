<%
	if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "N/A" then
			tcWebApi_constSet("WebCurSet_Entry", "keywordTime_entry", "0")
	end if
	
	If Request_Form("TimeofDay_KeywordIndexChangeFlag") = "1" Then
		tcWebApi_set("WebCurSet_Entry", "keywordTime_entry", "TimeofDay_FilterKeyword_Index")
	end if
	If Request_Form("TimeofDay_URLFilterKeywordSave_Flag") = "1" Then
		tcWebApi_set("WebCurSet_Entry", "keywordTime_entry", "TimeofDay_FilterKeyword_Index")
		tcWebApi_set("keywordTime_Entry", "keyword", "TimeofDay_URLFilterKeyword_Input")
		tcWebApi_commit("keywordTime_Entry")
	elseif Request_Form("TimeofDay_URLFilterKeywordSave_Flag") = "2" Then
		tcWebApi_unset("keywordTime_Entry")
		tcWebApi_commit("keywordTime_Entry")	
	End If
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<script language="javascript" src="/general.js"></script>
<script language='javascript'>
function TimeofDay_URLFilterKeywordSave( FlagValue )
{
	var URLKeyword = document.forms[0].TimeofDay_URLFilterKeyword_Input.value;
	if (FlagValue == 1)
	{
		if (!isValidKeyword(URLKeyword))
		{
			alert("<%tcWebApi_get("String_Entry","KeywordJS0Text","s")%>");
			return false;
		}
	}
	document.forms[0].TimeofDay_URLFilterKeywordSave_Flag.value=FlagValue;
	document.forms[0].submit();
}

function TimeofDay_KeywordIndexChange()
{
	document.TimeofDay_URLFilterKeyword_form.TimeofDay_KeywordIndexChangeFlag.value=1;
	document.TimeofDay_URLFilterKeyword_form.submit();
}

function isValidKeyword(val)
{
	var i;
	for(i = 0 ; i < val.length ; i++)
	{
		if (val.charCodeAt(i) < 33 || val.charCodeAt(i) > 126)
			return false;
	}
	return true;
}
</script>
</head><body topmargin="10" leftmargin="0">
<FORM METHOD="POST" name="TimeofDay_URLFilterKeyword_form">
	<table width="560" border="0" align=center cellpadding="0" cellspacing="0">
		<tr>
			<td width="560" height="5" valign="baseline" class="orange">
			</td>
		</tr>
	</table>
	<table border="0" width="560" align=center cellspacing="0" cellpadding="0">
		<tr>
			<td width="200" height="50" align=center valign=middle bgcolor="#FFFFFF">
				<div align=left>
				<img src="/logo.gif" width="200" height="50"></div></td><td width="360" align=right valign="bottom" bgcolor="#FFFFFF" class="model">
				</div>
			</td>
		</tr>
	</table>
	<table width="560" height="2" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
		<tr>
			<td width="560" valign="baseline" class="orange"> 
			</td>
		</tr>
	</table>
	<table width="560"  height="35" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
		<tr>
			<td width="560" height="35" bgcolor="#FFFFFF" class="headline">
				<font color="#000000"><%tcWebApi_get("String_Entry","KeywordURLFilterText","s")%></font>
			</td>
		</tr>
	</table>
	<table width="560" border="0" align=center cellpadding="0" cellspacing="0" color="#FF9933">
		<tr>
			<td height="15" class="light-orange" colspan="5"> </td>
		</tr>
		<tr>
			<td width="120" height="30" class="title-main">
			<font color="#FFFFFF"><%tcWebApi_get("String_Entry","KeywordEditingText","s")%></font>
			</td>
			<td width="10" class="black"></td><td width="100"></td>
			<td width="10"></td><td width="320"></td>
		</tr>
		<tr>
			<td height="25" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td>
				<div align=right>
				<font color="#000000"><%tcWebApi_get("String_Entry","KeywordIndexText","s")%></font>
				</div>
			</td>
			<td><div align=center>:</div></td>
			<td>
				<SELECT NAME="TimeofDay_FilterKeyword_Index" SIZE="1" onchange="TimeofDay_KeywordIndexChange()">
					<OPTION value="0" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "0" then  asp_Write("selected") end if%>>0
					<OPTION value="1" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "1" then  asp_Write("selected") end if%>>1
					<OPTION value="2" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "2" then  asp_Write("selected") end if%>>2
					<OPTION value="3" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "3" then  asp_Write("selected") end if%>>3
					<OPTION value="4" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "4" then  asp_Write("selected") end if%>>4
					<OPTION value="5" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "5" then  asp_Write("selected") end if%>>5
					<OPTION value="6" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "6" then  asp_Write("selected") end if%>>6
					<OPTION value="7" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "7" then  asp_Write("selected") end if%>>7
					<OPTION value="8" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "8" then  asp_Write("selected") end if%>>8
					<OPTION value="9" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "9" then  asp_Write("selected") end if%>>9
					<OPTION value="10" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "10" then  asp_Write("selected") end if%>>10
					<OPTION value="11" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "11" then  asp_Write("selected") end if%>>11
					<OPTION value="12" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "12" then  asp_Write("selected") end if%>>12
					<OPTION value="13" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "13" then  asp_Write("selected") end if%>>13
					<OPTION value="14" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "14" then  asp_Write("selected") end if%>>14
					<OPTION value="15" <%if tcWebApi_get("WebCurSet_Entry","keywordTime_entry","h") = "15" then  asp_Write("selected") end if%>>15
				</SELECT>
				<INPUT TYPE="HIDDEN" NAME="TimeofDay_KeywordIndexChangeFlag" VALUE="0">
			</td>
		</tr>
		<tr>
			<td height="25" class="light-orange">&nbsp;</td><td class="light-orange"></td>
			<td>
				<div align=right>
				<font color="#000000"><%tcWebApi_get("String_Entry","KeywordText","s")%></font>
				</div>
			</td>
			<td><div align=center>:</div></td>
			<td>
				<INPUT TYPE="TEXT" NAME="TimeofDay_URLFilterKeyword_Input" SIZE="48" MAXLENGTH="49" VALUE="<%if tcWebApi_get("keywordTime_Entry", "keyword", "h") <> "N/A" then tcWebApi_get("keywordTime_Entry", "keyword", "s") else asp_Write("") end if %>">
			</td>
		</tr>
	</table>
	<table width="560" border="0" align=center cellpadding="0" cellspacing="0" color="#FF9933">
		<tr>
			<td width="21%" height="25" class="title-sub">
			<font color="#000000"><%tcWebApi_get("String_Entry","KeywordListText","s")%></font></td>
			<td width="2%" class="light-orange"></td>
			<td width="2%"><hr noshade class="light-orange-line"></td>
			<td width="73%"><hr noshade class="light-orange-line"></td>
			<td width="2%"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td class="light-orange"></td>
			<td class="light-orange"></td>
			<td >&nbsp;</td>
			<td>
					<iframe src="/cgi-bin/keywordTimeOfDay.cgi" frameborder="0" width="100%" height="300" align=left scrolling="no"></iframe>
			</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td height="15" class="light-orange"></td>
			<td class="light-orange"></td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
	</table>

	<table width="560" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="130" height="40" class="black">&nbsp;</td>
			<td width="100" class="orange">&nbsp;</td>
			<td class="orange">
				<INPUT TYPE="HIDDEN" NAME="TimeofDay_URLFilterKeywordSave_Flag" VALUE="0">
				<INPUT TYPE="BUTTON" NAME="Submit" VALUE="<%tcWebApi_get("String_Entry","ButtonApplyText","s")%>" onClick="TimeofDay_URLFilterKeywordSave(1)">
				<INPUT TYPE="BUTTON" NAME="Submit" VALUE="<%tcWebApi_get("String_Entry","ButtonDeleteText","s")%>" onClick="TimeofDay_URLFilterKeywordSave(2)">
				<INPUT TYPE="BUTTON" NAME="TimeofDay_URLFilterKeywordClose" VALUE="<%tcWebApi_get("String_Entry","ButtonCloseText","s")%>" onClick="window.close()">
			</td>
		</tr>
	</table>
</form>
</body>
</html>
