<%
if request_Form("preferred_lang") <> "" then
           tcWebApi_set("LanguageSwitch_Entry","Type","preferred_lang")
           tcWebApi_commit("LanguageSwitch_Entry")
end if
%>
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
</head>
<body></body>
</html>
