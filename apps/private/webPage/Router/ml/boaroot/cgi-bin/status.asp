<%
if request_Form("StatusActionFlag") <> "" then
	tcWebApi_set("LanguageSwitch_Entry","Type","StatusActionFlag")
	tcWebApi_commit("LanguageSwitch_Entry")	
end if
%>
<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="http://192.168.1.1/css/style.css">
<script language="javascript">
<%if request_Form("StatusActionFlag") <> "" then %>
	window.top.location='index.asp';
<%end if%>
function doSubmit(index)
{
	document.forms[0].StatusActionFlag.value = index;
	document.forms[0].submit();
}
</script>
</head><body>
<form METHOD="POST" action="/cgi-bin/status.asp" name="status_form">
<div align=center>
<table width="760" height="5" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td></td></tr></table><table border="0" width="760" cellspacing="0" cellpadding="0" bgcolor="#FFFFFF">
  <tr>
    <td width="200" height="50" align=center valign=middle bgcolor="#FFFFFF"><div align=left>
	<img src="/logo.gif" tppabs="logo.gif" width="200" height="50">
	</div></td><td width="560" align=right valign="bottom" bgcolor="#FFFFFF" class="model">	
	<INPUT TYPE="HIDDEN" NAME="StatusActionFlag" VALUE="">
<%if tcwebApi_get("WebCustom_Entry","isMultiLanguageSupport","h")="Yes" then%>	
	  <%tcWebApi_get("String_Entry","STATUSLanguageText","s")%>
	  <INPUT TYPE="submit" NAME="StatusEngBTN" VALUE="<%tcWebApi_get("String_Entry","ButtonEnglishText","s")%>" onClick="doSubmit(1);">
	  <INPUT TYPE="submit" NAME="StatusTurBTN" VALUE="<%tcWebApi_get("String_Entry","ButtonTurkeyText","s")%>" onClick="doSubmit(2);"> 
<%end if%>

<font color="#000000">
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%><%tcWebApi_get("String_Entry","AdslVdslInfoText","s")%>/<%end if%><%tcWebApi_get("String_Entry","AdslInfoText","s")%>
</font>

	</td></tr></table>
<table width="760" height="1" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#CCCCCC">
  <tr>
    <td></td></tr></table></div></form>	
</body></html>
