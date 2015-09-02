<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
</head><frameset rows="61,67,*" framespacing="0" border="0"frameborder="0">
<frame name="header" src="/cgi-bin/status.asp" marginwidth="0" marginheight="0">
    <frame name="navigation" src="/cgi-bin/navigation-status.asp" marginwidth="0" marginheight="0">
<frame name="main" 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<%if tcwebApi_get("WebCustom_Entry","isCZOTESupported","h")="Yes" then%>
	src="/cgi-bin/wizardPPP_OTE.asp"
	<%else%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
	src="/cgi-bin/status_deviceinfo.asp"
	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
	src="/cgi-bin/status_log.cgi"
	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
	src="/cgi-bin/status_statistics.asp"	
	<% End if%>
	<%end if%>
marginwidth="0" marginheight="0">

</frameset><noframes>
</noframes>
</html>
