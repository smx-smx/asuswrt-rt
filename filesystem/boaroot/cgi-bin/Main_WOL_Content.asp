<%
If Request_Form("applyFlag") = "1" Then
	WOL_Invoke()
End If
If Request_Form("wollistFlag") = "1" Then
	tcWebApi_set("Wol_Entry", "List_1", "wollist1")
	tcWebApi_set("Wol_Entry", "List_2", "wollist2")
	tcWebApi_set("Wol_Entry", "List_3", "wollist3")
	tcWebApi_set("Wol_Entry", "List_4", "wollist4")
	tcWebApi_commit("Wol")
End If
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title><%tcWebApi_get("String_Entry","Network_Tools","s")%> - <%tcWebApi_get("String_Entry","NetworkTools_WOL","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<style>
#ClientList_Block_PC{
	border:1px outset #999;
	background-color:#576D73;
	position:absolute;
	*margin-top:26px;	
	margin-left:53px;
	*margin-left:-189px;
	width:340px;
	text-align:left;	
	height:auto;
	overflow-y:auto;
	z-index:200;
	padding: 1px;
	display:none;
}
#ClientList_Block_PC div{
	background-color:#576D73;
	height:auto;
	*height:20px;
	line-height:20px;
	text-decoration:none;
	font-family: Lucida Console;
	padding-left:2px;
}

#ClientList_Block_PC a{
	background-color:#EFEFEF;
	color:#FFF;
	font-size:12px;
	font-family:Arial, Helvetica, sans-serif;
	text-decoration:none;	
}
#ClientList_Block_PC div:hover, #ClientList_Block a:hover{
	background-color:#3366FF;
	color:#FFFFFF;
	cursor:default;
}	
</style>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script>
<!--[if !IE]>-->
	jQuery.noConflict();	
<!--<![endif]-->

var tableData = [
		//["1", "BVA-NB","192.168.177.168","00:22:15:A5:03:68"],
		["1", "<%tcWebApi_staticGet("DhcpLease_Entry0", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry0", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry0", "MAC","s")%>"],
		["2", "<%tcWebApi_staticGet("DhcpLease_Entry1", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry1", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry1", "MAC","s")%>"],
		["3", "<%tcWebApi_staticGet("DhcpLease_Entry2", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry2", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry2", "MAC","s")%>"],
		["4", "<%tcWebApi_staticGet("DhcpLease_Entry3", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry3", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry3", "MAC","s")%>"],
		["5", "<%tcWebApi_staticGet("DhcpLease_Entry4", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry4", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry4", "MAC","s")%>"],
		["6", "<%tcWebApi_staticGet("DhcpLease_Entry5", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry5", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry5", "MAC","s")%>"],
		["7", "<%tcWebApi_staticGet("DhcpLease_Entry6", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry6", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry6", "MAC","s")%>"],
		["8", "<%tcWebApi_staticGet("DhcpLease_Entry7", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry7", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry7", "MAC","s")%>"],
		["9", "<%tcWebApi_staticGet("DhcpLease_Entry8", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry8", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry8", "MAC","s")%>"],
		["10", "<%tcWebApi_staticGet("DhcpLease_Entry9", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry9", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry9", "MAC","s")%>"],
		["11", "<%tcWebApi_staticGet("DhcpLease_Entry10", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry10", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry10", "MAC","s")%>"],
		["12", "<%tcWebApi_staticGet("DhcpLease_Entry11", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry11", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry11", "MAC","s")%>"],
		["13", "<%tcWebApi_staticGet("DhcpLease_Entry12", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry12", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry12", "MAC","s")%>"],
		["14", "<%tcWebApi_staticGet("DhcpLease_Entry13", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry13", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry13", "MAC","s")%>"],
		["15", "<%tcWebApi_staticGet("DhcpLease_Entry14", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry14", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry14", "MAC","s")%>"],
		["16", "<%tcWebApi_staticGet("DhcpLease_Entry15", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry15", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry15", "MAC","s")%>"],
		["17", "<%tcWebApi_staticGet("DhcpLease_Entry16", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry16", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry16", "MAC","s")%>"],
		["18", "<%tcWebApi_staticGet("DhcpLease_Entry17", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry17", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry17", "MAC","s")%>"],
		["19", "<%tcWebApi_staticGet("DhcpLease_Entry18", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry18", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry18", "MAC","s")%>"],
		["20", "<%tcWebApi_staticGet("DhcpLease_Entry19", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry19", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry19", "MAC","s")%>"],
		["21", "<%tcWebApi_staticGet("DhcpLease_Entry20", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry20", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry20", "MAC","s")%>"],
		["22", "<%tcWebApi_staticGet("DhcpLease_Entry21", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry21", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry21", "MAC","s")%>"],
		["23", "<%tcWebApi_staticGet("DhcpLease_Entry22", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry22", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry22", "MAC","s")%>"],
		["24", "<%tcWebApi_staticGet("DhcpLease_Entry23", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry23", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry23", "MAC","s")%>"],
		["25", "<%tcWebApi_staticGet("DhcpLease_Entry24", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry24", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry24", "MAC","s")%>"],
		["26", "<%tcWebApi_staticGet("DhcpLease_Entry25", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry25", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry25", "MAC","s")%>"],
		["27", "<%tcWebApi_staticGet("DhcpLease_Entry26", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry26", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry26", "MAC","s")%>"],
		["28", "<%tcWebApi_staticGet("DhcpLease_Entry27", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry27", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry27", "MAC","s")%>"],
		["29", "<%tcWebApi_staticGet("DhcpLease_Entry28", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry28", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry28", "MAC","s")%>"],
		["30", "<%tcWebApi_staticGet("DhcpLease_Entry29", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry29", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry29", "MAC","s")%>"],
		["31", "<%tcWebApi_staticGet("DhcpLease_Entry30", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry30", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry30", "MAC","s")%>"],
		["32", "<%tcWebApi_staticGet("DhcpLease_Entry31", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry31", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry31", "MAC","s")%>"],
		["33", "<%tcWebApi_staticGet("DhcpLease_Entry32", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry32", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry32", "MAC","s")%>"],
		["34", "<%tcWebApi_staticGet("DhcpLease_Entry33", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry33", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry33", "MAC","s")%>"],
		["35", "<%tcWebApi_staticGet("DhcpLease_Entry34", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry34", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry34", "MAC","s")%>"],
		["36", "<%tcWebApi_staticGet("DhcpLease_Entry35", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry35", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry35", "MAC","s")%>"],
		["37", "<%tcWebApi_staticGet("DhcpLease_Entry36", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry36", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry36", "MAC","s")%>"],
		["38", "<%tcWebApi_staticGet("DhcpLease_Entry37", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry37", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry37", "MAC","s")%>"],
		["39", "<%tcWebApi_staticGet("DhcpLease_Entry38", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry38", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry38", "MAC","s")%>"],
		["40", "<%tcWebApi_staticGet("DhcpLease_Entry39", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry39", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry39", "MAC","s")%>"],
		["41", "<%tcWebApi_staticGet("DhcpLease_Entry40", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry40", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry40", "MAC","s")%>"],
		["42", "<%tcWebApi_staticGet("DhcpLease_Entry41", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry41", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry41", "MAC","s")%>"],
		["43", "<%tcWebApi_staticGet("DhcpLease_Entry42", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry42", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry42", "MAC","s")%>"],
		["44", "<%tcWebApi_staticGet("DhcpLease_Entry43", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry43", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry43", "MAC","s")%>"],
		["45", "<%tcWebApi_staticGet("DhcpLease_Entry44", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry44", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry44", "MAC","s")%>"],
		["46", "<%tcWebApi_staticGet("DhcpLease_Entry45", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry45", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry45", "MAC","s")%>"],
		["47", "<%tcWebApi_staticGet("DhcpLease_Entry46", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry46", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry46", "MAC","s")%>"],
		["48", "<%tcWebApi_staticGet("DhcpLease_Entry47", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry47", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry47", "MAC","s")%>"],
		["49", "<%tcWebApi_staticGet("DhcpLease_Entry48", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry48", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry48", "MAC","s")%>"],
		["50", "<%tcWebApi_staticGet("DhcpLease_Entry49", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry49", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry49", "MAC","s")%>"],
		["51", "<%tcWebApi_staticGet("DhcpLease_Entry50", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry50", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry50", "MAC","s")%>"],
		["52", "<%tcWebApi_staticGet("DhcpLease_Entry51", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry51", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry51", "MAC","s")%>"],
		["53", "<%tcWebApi_staticGet("DhcpLease_Entry52", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry52", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry52", "MAC","s")%>"],
		["54", "<%tcWebApi_staticGet("DhcpLease_Entry53", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry53", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry53", "MAC","s")%>"],
		["55", "<%tcWebApi_staticGet("DhcpLease_Entry54", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry54", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry54", "MAC","s")%>"],
		["56", "<%tcWebApi_staticGet("DhcpLease_Entry55", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry55", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry55", "MAC","s")%>"],
		["57", "<%tcWebApi_staticGet("DhcpLease_Entry56", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry56", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry56", "MAC","s")%>"],
		["58", "<%tcWebApi_staticGet("DhcpLease_Entry57", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry57", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry57", "MAC","s")%>"],
		["59", "<%tcWebApi_staticGet("DhcpLease_Entry58", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry58", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry58", "MAC","s")%>"],
		["60", "<%tcWebApi_staticGet("DhcpLease_Entry59", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry59", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry59", "MAC","s")%>"],
		["61", "<%tcWebApi_staticGet("DhcpLease_Entry60", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry60", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry60", "MAC","s")%>"],
		["62", "<%tcWebApi_staticGet("DhcpLease_Entry61", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry61", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry61", "MAC","s")%>"],
		["63", "<%tcWebApi_staticGet("DhcpLease_Entry62", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry62", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry62", "MAC","s")%>"],
		["64", "<%tcWebApi_staticGet("DhcpLease_Entry63", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry63", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry63", "MAC","s")%>"],
		["65", "<%tcWebApi_staticGet("DhcpLease_Entry64", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry64", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry64", "MAC","s")%>"],
		["66", "<%tcWebApi_staticGet("DhcpLease_Entry65", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry65", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry65", "MAC","s")%>"],
		["67", "<%tcWebApi_staticGet("DhcpLease_Entry66", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry66", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry66", "MAC","s")%>"],
		["68", "<%tcWebApi_staticGet("DhcpLease_Entry67", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry67", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry67", "MAC","s")%>"],
		["69", "<%tcWebApi_staticGet("DhcpLease_Entry68", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry68", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry68", "MAC","s")%>"],
		["70", "<%tcWebApi_staticGet("DhcpLease_Entry69", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry69", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry69", "MAC","s")%>"],
		["71", "<%tcWebApi_staticGet("DhcpLease_Entry70", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry70", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry70", "MAC","s")%>"],
		["72", "<%tcWebApi_staticGet("DhcpLease_Entry71", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry71", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry71", "MAC","s")%>"],
		["73", "<%tcWebApi_staticGet("DhcpLease_Entry72", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry72", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry72", "MAC","s")%>"],
		["74", "<%tcWebApi_staticGet("DhcpLease_Entry73", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry73", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry73", "MAC","s")%>"],
		["75", "<%tcWebApi_staticGet("DhcpLease_Entry74", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry74", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry74", "MAC","s")%>"],
		["76", "<%tcWebApi_staticGet("DhcpLease_Entry75", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry75", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry75", "MAC","s")%>"],
		["77", "<%tcWebApi_staticGet("DhcpLease_Entry76", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry76", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry76", "MAC","s")%>"],
		["78", "<%tcWebApi_staticGet("DhcpLease_Entry77", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry77", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry77", "MAC","s")%>"],
		["79", "<%tcWebApi_staticGet("DhcpLease_Entry78", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry78", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry78", "MAC","s")%>"],
		["80", "<%tcWebApi_staticGet("DhcpLease_Entry79", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry79", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry79", "MAC","s")%>"],
		["81", "<%tcWebApi_staticGet("DhcpLease_Entry80", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry80", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry80", "MAC","s")%>"],
		["82", "<%tcWebApi_staticGet("DhcpLease_Entry81", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry81", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry81", "MAC","s")%>"],
		["83", "<%tcWebApi_staticGet("DhcpLease_Entry82", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry82", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry82", "MAC","s")%>"],
		["84", "<%tcWebApi_staticGet("DhcpLease_Entry83", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry83", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry83", "MAC","s")%>"],
		["85", "<%tcWebApi_staticGet("DhcpLease_Entry84", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry84", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry84", "MAC","s")%>"],
		["86", "<%tcWebApi_staticGet("DhcpLease_Entry85", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry85", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry85", "MAC","s")%>"],
		["87", "<%tcWebApi_staticGet("DhcpLease_Entry86", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry86", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry86", "MAC","s")%>"],
		["88", "<%tcWebApi_staticGet("DhcpLease_Entry87", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry87", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry87", "MAC","s")%>"],
		["89", "<%tcWebApi_staticGet("DhcpLease_Entry88", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry88", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry88", "MAC","s")%>"],
		["90", "<%tcWebApi_staticGet("DhcpLease_Entry89", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry89", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry89", "MAC","s")%>"],
		["91", "<%tcWebApi_staticGet("DhcpLease_Entry90", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry90", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry90", "MAC","s")%>"],
		["92", "<%tcWebApi_staticGet("DhcpLease_Entry91", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry91", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry91", "MAC","s")%>"],
		["93", "<%tcWebApi_staticGet("DhcpLease_Entry92", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry92", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry92", "MAC","s")%>"],
		["94", "<%tcWebApi_staticGet("DhcpLease_Entry93", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry93", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry93", "MAC","s")%>"],
		["95", "<%tcWebApi_staticGet("DhcpLease_Entry94", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry94", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry94", "MAC","s")%>"],
		["96", "<%tcWebApi_staticGet("DhcpLease_Entry95", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry95", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry95", "MAC","s")%>"],
		["97", "<%tcWebApi_staticGet("DhcpLease_Entry96", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry96", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry96", "MAC","s")%>"],
		["98", "<%tcWebApi_staticGet("DhcpLease_Entry97", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry97", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry97", "MAC","s")%>"],
		["99", "<%tcWebApi_staticGet("DhcpLease_Entry98", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry98", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry98", "MAC","s")%>"],
		["100", "<%tcWebApi_staticGet("DhcpLease_Entry99", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry99", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry99", "MAC","s")%>"]
	];

var wollist_array_1 = "<%tcWebApi_staticGet("Wol_Entry", "List_1","s")%>";
var wollist_array_2 = "<%tcWebApi_staticGet("Wol_Entry", "List_2","s")%>";
var wollist_array_3 = "<%tcWebApi_staticGet("Wol_Entry", "List_3","s")%>";
var wollist_array_4 = "<%tcWebApi_staticGet("Wol_Entry", "List_4","s")%>";

var wollist_array = wollist_array_1;
if(wollist_array_2.length > 0)
	wollist_array = wollist_array+"<"+wollist_array_2;
if(wollist_array_3.length > 0)
	wollist_array = wollist_array+"<"+wollist_array_3;
if(wollist_array_4.length > 0)
	wollist_array = wollist_array+"<"+wollist_array_4;	

function initial(){
	show_menu();
	setTimeout("update_FAQ();", 300);
	showwollist();
	showLANMacList();
}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq3", "https://www.asus.com", "/support/FAQ/", "1011268");	//id in string tag : smart_access3
	}
}
function showwollist(){
        var wollist_row = wollist_array.split('<');
        var code = "";

        code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="wollist_table">';
        if(wollist_array.length == 0)
                code +='<tr><td style="color:#FFCC00;" colspan="6"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td></tr>';
        else{
                for(var i = 0; i < wollist_row.length; i++){
                	if(wollist_row[i].search('>') >= 0){
                        code +='<tr id="row'+i+'">';
                        var wollist_col = wollist_row[i].split('>');
                                for(var j = 0; j < wollist_col.length; j++){
                                        if(j == 0)
                                                code +='<td width="40%">'+ wollist_col[j] +'</td>';
                                        else
                                                code +='<td width="40%" style="font-weight:bold;cursor:pointer;text-decoration:underline;font-family:Lucida Console;" onclick="document.form.destMac.value=this.innerHTML;">'+ wollist_col[j] +'</td>';
                                }
                                code +='<td width="20%">';
                                code +='<input class="remove_btn" onclick="del_Row(this);" value=""/></td></tr>';
			}
                }
        }

  code +='</table>';
	$("wollist_Block").innerHTML = code;
}

function del_Row(r){
  var i=r.parentNode.parentNode.rowIndex;
  $('wollist_table').deleteRow(i);
  
  var wollist_value = "";
	for(k=0; k<$('wollist_table').rows.length; k++){
		for(j=0; j<$('wollist_table').rows[k].cells.length-1; j++){
			if(j == 0)	
				wollist_value += "<";
			else{
				wollist_value += $('wollist_table').rows[k].cells[0].innerHTML;
				wollist_value += ">";
				wollist_value += $('wollist_table').rows[k].cells[1].innerHTML;
			}
		}
	}

	wollist_array = wollist_value;
	if(wollist_array == "")
		showwollist();
}

function addRow(obj, head){	
	if(head == 1){
		if(wollist_array.length > 0)
			wollist_array += "<";
	}else
		wollist_array += ">";
			
	wollist_array += obj.value;

	obj.value = "";
}

function showLANMacList(){
	if(clientList.length == 0){
		setTimeout(function() {
			genClientList();
			showLANMacList();
		}, 500);
		return false;
	}
	
	var htmlCode = "";
	for(var i=0; i<clientList.length;i++){
		var clientObj = clientList[clientList[i]];

		if(clientObj.IP == "offline") clientObj.IP = "";
		if(clientObj.Name.length > 30) clientObj.Name = clientObj.Name.substring(0, 27) + "...";

		htmlCode += '<a><div onmouseover="over_var=1;" onmouseout="over_var=0;" onclick="setClientIP(\'';
		htmlCode += clientObj.MacAddr;
		htmlCode += '\', \'';
		htmlCode += clientObj.Name;
		htmlCode += '\');"><strong>';
		htmlCode += clientObj.Name;
		htmlCode += '</strong> ( ';
		htmlCode += clientObj.MacAddr;
		htmlCode += ' )</div></a><!--[if lte IE 6.5]><iframe class="hackiframe2"></iframe><![endif]-->';	
	}

	$("ClientList_Block_PC").innerHTML = htmlCode;
}

function setClientIP(macaddr, nameIP){
	document.form.wollist_macAddr.value = macaddr;
	document.form.wollist_deviceName.value = nameIP;
	hideClients_Block();
	over_var = 0;
}

function onSubmitCtrl(o, s) {
	if(document.form.destMac.value == ""){
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
		document.form.destMac.focus();
		return false;
	}

	if(check_hwaddr_flag(document.form.destMac) != 0){
		alert("<%tcWebApi_get("String_Entry","IPC_x_illegal_mac","s")%>");
		document.form.destMac.focus();
		return false;
	}

	document.form.action_mode.value = s;
	updateOptions();
}
	
function updateOptions(){
	document.form.applyFlag.value="1";	
	document.form.submit();
	document.getElementById("cmdBtn").disabled = true;
	document.getElementById("cmdBtn").style.color = "#666";
	document.getElementById("loadingIcon").style.display = "";
	setTimeout("refresh_btn()", "1000");
}	

function refresh_btn(){
	document.getElementById("loadingIcon").style.display = "none";
	document.getElementById("cmdBtn").disabled = false;
	document.getElementById("cmdBtn").style.color = "#FFF";	
	
}

var over_var = 0;
var isMenuopen = 0;

function hideClients_Block(){
	$("pull_arrow").src = "/images/arrow-down.gif";
	$('ClientList_Block_PC').style.display='none';
	isMenuopen = 0;
}

function pullLANIPList(obj){
	if(isMenuopen == 0){
		obj.src = "/images/arrow-top.gif"
		$("ClientList_Block_PC").style.display = 'block';
		document.form.wollist_macAddr.focus();
		isMenuopen = 1;
	}
	else
		hideClients_Block();
}

function addRow_Group(upper){
	var rule_num = $('wollist_table').rows.length;
	var item_num = $('wollist_table').rows[0].cells.length;		
	if(rule_num >= upper){
		alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
		return false;	
	}			

	if(document.form.wollist_macAddr.value == ""){
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
		document.form.wollist_macAddr.focus();
		document.form.wollist_macAddr.select();			
		return false;
	}else if(!check_macaddr(document.form.wollist_macAddr, check_hwaddr_flag(document.form.wollist_macAddr))){
		document.form.wollist_macAddr.focus();
		document.form.wollist_macAddr.select();	
		return false;	
	}
	
	if(item_num >=2){
		for(i=0; i<rule_num; i++){	
				if(document.form.wollist_macAddr.value.toLowerCase() == $('wollist_table').rows[i].cells[1].innerHTML.toLowerCase()){
					alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");
					document.form.wollist_macAddr.focus();
					document.form.wollist_macAddr.select();
					return false;
				}	
		}
	}
		
	addRow(document.form.wollist_deviceName ,1);
	addRow(document.form.wollist_macAddr, 0);
	showwollist();
}


function check_macaddr(obj,flag){ //control hint of input mac address
	if(flag == 1){
		var childsel=document.createElement("div");
		childsel.setAttribute("id","check_mac");
		childsel.style.color="#FFCC00";
		obj.parentNode.appendChild(childsel);
		$("check_mac").innerHTML="<%tcWebApi_get("String_Entry","LHC_MnlDHCPMacaddr_id","s")%>";
		$("check_mac").style.display = "";
		return false;
	}else if(flag ==2){
		var childsel=document.createElement("div");
		childsel.setAttribute("id","check_mac");
		childsel.style.color="#FFCC00";
		obj.parentNode.appendChild(childsel);
		$("check_mac").innerHTML="<%tcWebApi_get("String_Entry","IPC_x_illegal_mac","s")%>";
		$("check_mac").style.display = "";
		return false;		
	}else{	
		$("check_mac") ? $("check_mac").style.display="none" : true;
		return true;
	}	
}

function applyRule(){
	var rule_num = $('wollist_table').rows.length;
	var item_num = $('wollist_table').rows[0].cells.length;
	var tmp_value1 = "";
	var tmp_value2 = "";
	var tmp_value3 = "";
	var tmp_value4 = "";

	for(i=0; i<rule_num; i++){		
		if(i >= 24){
			for(j=0; j<item_num-1; j++){
				if(i != 24 && j == 0)
					tmp_value4 += "<";
				tmp_value4 += $('wollist_table').rows[i].cells[j].innerHTML;
				if(j != item_num-2)	
					tmp_value4 += ">";					
			}
		}
		else if(i >= 16){
			for(j=0; j<item_num-1; j++){	
				if(i != 16 && j == 0)
					tmp_value3 += "<";
				tmp_value3 += $('wollist_table').rows[i].cells[j].innerHTML;
				if(j != item_num-2)	
					tmp_value3 += ">";
			}
		}
		else if(i >= 8){
			for(j=0; j<item_num-1; j++){	
				if(i != 8 && j == 0)
					tmp_value2 += "<";
				tmp_value2 += $('wollist_table').rows[i].cells[j].innerHTML;
				if(j != item_num-2)	
					tmp_value2 += ">";
			}
		}
		else{				
			for(j=0; j<item_num-1; j++){	
				if(i != 0 && j == 0)
					tmp_value1 += "<";
				tmp_value1 += $('wollist_table').rows[i].cells[j].innerHTML;
				if(j != item_num-2)	
					tmp_value1 += ">";
			}
		}		
	}
	
	if(tmp_value1.search("<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>") >= 0 || tmp_value1 == "<")
		tmp_value1 = "";
	if(tmp_value2.search("<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>") >= 0 || tmp_value2 == "<")
		tmp_value2 = "";
	if(tmp_value3.search("<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>") >= 0 || tmp_value3 == "<")
		tmp_value3 = "";
	if(tmp_value4.search("<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>") >= 0 || tmp_value4 == "<")
		tmp_value4 = "";				

	document.wolform.wollist1.value = tmp_value1;
	document.wolform.wollist2.value = tmp_value2;
	document.wolform.wollist3.value = tmp_value3;
	document.wolform.wollist4.value = tmp_value4;	
	
	document.wolform.wollistFlag.value = "1";
	showLoading(3);
	setTimeout("redirect();", 3000);
	document.wolform.submit();
}

function redirect(){
	document.location.href = "/cgi-bin/Main_WOL_Content.asp";
}
</script>
</head>
<body onload="initial();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="GET" name="form" action="Main_WOL_Content.asp" target="hidden_frame"> 
<input type="hidden" name="current_page" value="Main_WOL_Content.asp">
<input type="hidden" name="next_page" value="Main_WOL_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="applyFlag" value="0">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">

<table class="content" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td width="17">&nbsp;</td>
		<td valign="top" width="202">
			<div id="mainMenu"></div>
			<div id="subMenu"></div>
		</td>	
		<td valign="top">
			<div id="tabMenu" class="submenuBlock"></div>		
			<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
				<tr>
					<td align="left" valign="top">				
						<table width="760px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3"  class="FormTitle" id="FormTitle">		
							<tr>
								<td bgcolor="#4D595D" colspan="3" valign="top">
									<div>&nbsp;</div>
									<div class="formfonttitle"><%tcWebApi_get("String_Entry","Network_Tools","s")%> - <%tcWebApi_get("String_Entry","NetworkTools_WOL","s")%></div>
									<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
									<div class="formfontdesc"><%tcWebApi_get("String_Entry","smart_access2","s")%></div>	
									<div class="formfontdesc"><%tcWebApi_get("String_Entry","smart_access3","s")%></div>	
									<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
										<tr>
											<th width="20%"><%tcWebApi_get("String_Entry","NetworkTools_target","s")%></th>
											<td>
												<input type="text" class="input_20_table" maxlength="17" name="destMac" value="" placeholder="ex: <% tcWebApi_staticGet("WLan_Common","wl0_MacAddress","s") %>" onKeyPress="return validator.isHWAddr(this,event);">
												<input class="button_gen" id="cmdBtn" onClick="onSubmitCtrl(this, ' Refresh ')" type="button" value="<%tcWebApi_get("String_Entry","NetworkTools_WOL_btn","s")%>">
												<img id="loadingIcon" style="display:none;" src="/images/InternetScan.gif"></span>
											</td>										
										</tr>
									</table>

									<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" style="margin-top:8px;">
									  	<thead>
									  		<tr>
												<td colspan="3" id="GWStatic"><%tcWebApi_get("String_Entry","NetworkTools_Offline","s")%>&nbsp;(<%tcWebApi_get("String_Entry","List_limit","s")%>&nbsp;32)</td>
									  		</tr>
									  	</thead>
						
									  	<tr>
						        		<th><%tcWebApi_get("String_Entry","ShareNode_DeviceName_in","s")%></th>
								  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,10);"><%tcWebApi_get("String_Entry","MAC_Address","s")%></a></th>
						        		<th><%tcWebApi_get("String_Entry","list_add_delete","s")%></th>
									  	</tr>			  
									  	<tr>
									  			<!-- client info -->
																					  		
				            			<td width="40%">
				            				<input type="text" class="input_20_table" maxlength="15" name="wollist_deviceName" onClick="hideClients_Block();">
														<img id="pull_arrow" height="14px;" src="/images/arrow-down.gif" style="position:absolute;*margin-left:-3px;*margin-top:1px;" onclick="pullLANIPList(this);" title="<%tcWebApi_get("String_Entry","select_device_name","s")%>" onmouseover="over_var=1;" onmouseout="over_var=0;">
														<div id="ClientList_Block_PC" class="ClientList_Block_PC"></div>	
				            			</td>
				            			<td width="40%">
				                		<input type="text" class="input_20_table" maxlength="17" name="wollist_macAddr" style="" onKeyPress="return validator.isHWAddr(this,event)">
		                			</td>
				            			<td width="20%">
														<div> 
															<input type="button" class="add_btn" onClick="addRow_Group(32);" value="">
														</div>
				            			</td>
									  	</tr>	 			  
									  </table>
								  <div id="wollist_Block"></div>

			           	<div class="apply_gen">
			           		<input type="button" name="button" class="button_gen" onclick="applyRule();" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
		            	</div>

								</td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
			<!--===================================Ending of Main Content===========================================-->
		</td>
		<td width="10" align="center" valign="top"></td>
	</tr>
</table>
</form>

<form method="post" name="wolform" action="/cgi-bin/Main_WOL_Content.asp" target="hidden_frame">
<input type="hidden" name="wollistFlag" value="0">
<input type="hidden" name="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wollist1" value="<%tcWebApi_staticGet("Wol_Entry", "List_1","s")%>">
<input type="hidden" name="wollist2" value="<%tcWebApi_staticGet("Wol_Entry", "List_2","s")%>">
<input type="hidden" name="wollist3" value="<%tcWebApi_staticGet("Wol_Entry", "List_3","s")%>">
<input type="hidden" name="wollist4" value="<%tcWebApi_staticGet("Wol_Entry", "List_4","s")%>">
</form>


<div id="footer"></div>
</body>
</html>
