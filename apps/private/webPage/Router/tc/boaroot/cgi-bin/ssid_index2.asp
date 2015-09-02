<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
</head>
<frameset rows="65,75,*" framespacing="0" border="0" frameborder="0">
<frame name="header_ssid2" src="/status.html" marginwidth="0" marginheight="0">
<frame name="navigation_ssid2" src="/cgi-bin/navigation-status_ssid2.asp" marginwidth="0" marginheight="0">
<frame name="main_ssid2" 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
	src="/cgi-bin/status_deviceinfo.asp"
	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
	src="/cgi-bin/status_log.cgi"
	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
	src="/cgi-bin/status_statistics.asp"
	<% End if%>
	marginwidth="0" marginheight="0">
</frameset>
<noframes>
</noframes>
</html>