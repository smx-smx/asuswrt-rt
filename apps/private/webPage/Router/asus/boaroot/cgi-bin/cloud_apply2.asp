<%
if Request_Form("action_mode") = "get_sharelink" then
	tcWebApi_set("AiCloud_Entry", "share_link_host", "share_link_host")
	tcWebApi_set("AiCloud_Entry", "share_link_param", "share_link_param")
	tcWebApi_Save()
	do_apply_cgi()
end if
%>
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
</head>
</html>
