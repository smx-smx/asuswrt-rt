<%
If Request_Form("editFlag") = "1" then
	tcWebApi_Set("Parental_Entry","MULTIFILTER_ALL","MULTIFILTER_ALL")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_ENABLE","MULTIFILTER_ENABLE")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_MAC","MULTIFILTER_MAC")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_DEVICENAME","MULTIFILTER_DEVICENAME")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_MACFILTER_DAYTIME","MULTIFILTER_MACFILTER_DAYTIME")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_LANTOWAN_ENABLE","MULTIFILTER_LANTOWAN_ENABLE")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_LANTOWAN_DESC","MULTIFILTER_LANTOWAN_DESC")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_LANTOWAN_PORT","MULTIFILTER_LANTOWAN_PORT")
	tcWebApi_Set("Parental_Entry","MULTIFILTER_LANTOWAN_PROTO","MULTIFILTER_LANTOWAN_PROTO")
  	tcWebApi_commit("Parental_Entry")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<!--ParentalControl.asp-->

<head>
    <meta name="generator" content="HTML Tidy for Linux/x86 (vers 25 March 2009), see www.w3.org">
    <meta http-equiv="X-UA-Compatible" content="IE=Edge">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Expires" content="-1">

    <title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","Parental_Control","s")%></title>
    <link rel="shortcut icon" href="/images/favicon.png">
    <link rel="icon" href="/images/favicon.png">
    <link rel="stylesheet" type="text/css" href="/ParentalControl.css">
    <link rel="stylesheet" type="text/css" href="/index_style.css">
    <link rel="stylesheet" type="text/css" href="/form_style.css">
    <link rel="stylesheet" type="text/css" href="/usp_style.css">
    <link rel="stylesheet" type="text/css" href="/calendar/fullcalendar.css">
    <script type="text/javascript" src="/state.js"></script>
    <script type="text/javascript" src="/popup.js"></script>
    <script type="text/javascript" src="/help.js"></script>
    <script type="text/javascript" src="/detect.js"></script>
    <script type="text/javascript" src="/general.js"></script>
    <script type="text/javascript" src="/client_function.js"></script>
    <script type="text/javascript" src="/calendar/jquery.js"></script>
		<script type="text/javascript" src="/calendar/fullcalendar.js"></script>
		<script type="text/javascript" src="/calendar/jquery-ui-1.8.11.custom.min.js"></script>

		<script>
		if(parental2_support != -1){
			addNewScript("/calendar/fullcalendar.js");
			addNewScript("/calendar/jquery-ui-1.8.11.custom.min.js");
		}

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

    var $j = jQuery.noConflict();
    var jData;
    wan_route_x = '';
    wan_nat_x = '1';
    wan_proto = 'pppoe';


    function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

    function login_mac_str() { return ''; }

    var wireless = []; // [[MAC, associated, authorized], ...]
    var client_ip = login_ip_str();
    var client_mac = login_mac_str();
    var leases = []; // [[hostname, MAC, ip, lefttime], ...]
    var arps = []; // [[ip, x, x, MAC, x, type], ...]
    var arls = []; // [[MAC, port, x, x], ...]
    var wireless = []; // [[MAC, associated, authorized], ...]
    var ipmonitor = []; // [[IP, MAC, DeviceName, Type, http, printer, iTune], ...]
    var networkmap_fullscan = 'done'; //2008.07.24 Add. 1 stands for complete, 0 stands for scanning.;
    var parental2_support = rc_support.search("PARENTAL2");

    var MULTIFILTER_ENABLE = '<%TCWebApi_get("Parental_Entry","MULTIFILTER_ENABLE","s")%>'.replace(/&#62/g, ">");
    var MULTIFILTER_MAC = '<%TCWebApi_get("Parental_Entry","MULTIFILTER_MAC","s")%>'.replace(/&#62/g, ">");
    var MULTIFILTER_DEVICENAME = decodeURIComponent('<%tcWebApi_char_to_ascii("Parental_Entry","MULTIFILTER_DEVICENAME","s")%>').replace(/&#62/g, ">");
    var MULTIFILTER_MACFILTER_DAYTIME = '<%TCWebApi_get("Parental_Entry","MULTIFILTER_MACFILTER_DAYTIME","s")%>'.replace(/&#62/g, ">").replace(/&#60/g, "<");
    var MULTIFILTER_LANTOWAN_ENABLE = '<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_ENABLE","s")%>'.replace(/&#62/g, ">").replace(/&#60/g, "<");
    var MULTIFILTER_LANTOWAN_DESC = '<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_DESC","s")%>'.replace(/&#62/g, ">").replace(/&#60/g, "<");
    var MULTIFILTER_LANTOWAN_PORT = '<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_PORT","s")%>'.replace(/&#62/g, ">").replace(/&#60/g, "<");
    var MULTIFILTER_LANTOWAN_PROTO = '<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_PROTO","s")%>'.replace(/&#62/g, ">").replace(/&#60/g, "<");

    var MULTIFILTER_ENABLE_row = MULTIFILTER_ENABLE.split('>');
    var MULTIFILTER_DEVICENAME_row = MULTIFILTER_DEVICENAME.split('>');
    var MULTIFILTER_MAC_row = MULTIFILTER_MAC.split('>');
    var MULTIFILTER_LANTOWAN_ENABLE_row = MULTIFILTER_LANTOWAN_ENABLE.split('>');
    var MULTIFILTER_LANTOWAN_DESC_row = MULTIFILTER_LANTOWAN_DESC.split('>');
    var MULTIFILTER_LANTOWAN_PORT_row = MULTIFILTER_LANTOWAN_PORT.split('>');
    var MULTIFILTER_LANTOWAN_PROTO_row = MULTIFILTER_LANTOWAN_PROTO.split('>');
    var MULTIFILTER_MACFILTER_DAYTIME_row = MULTIFILTER_MACFILTER_DAYTIME.split('>');

    var _client;
    var StopTimeCount;
    function initial(){
    		show_menu();
    		show_footer();
    		gen_mainTable();
    		showLANIPList();
    }
    /*------------ Mouse event of fake LAN IP select menu {-----------------*/
    function setClientIP(devname, macaddr){
		document.form.PC_devicename.value = devname;
		document.form.PC_mac.value = macaddr;
		hideClients_Block();
		over_var = 0;
    }
function showLANIPList(){
	if(clientList.length == 0){
		setTimeout(function() {
			genClientList();
			showLANIPList();
		}, 500);
		return false;
	}
	
	var htmlCode = "";
	for(var i=0; i<clientList.length;i++){
		var clientObj = clientList[clientList[i]];

		if(clientObj.IP == "offline") clientObj.IP = "";
		if(clientObj.Name.length > 30) clientObj.Name = clientObj.Name.substring(0, 27) + "...";

		htmlCode += '<a><div onmouseover="over_var=1;" onmouseout="over_var=0;" onclick="setClientIP(\'';
		htmlCode += clientObj.Name;
		htmlCode += '\', \'';
		htmlCode += clientObj.MacAddr;
		htmlCode += '\');"><strong>';
		htmlCode += clientObj.Name;
		htmlCode += "</strong> (";
		htmlCode += clientObj.MacAddr;
		htmlCode += " )</div></a><!--[if lte IE 6.5]><script>alert(\"<#ALERT_TO_CHANGE_BROWSER#>\");</script><![endif]-->";	
	}

	$("ClientList_Block_PC").innerHTML = htmlCode;
}

    function pullLANIPList(obj){
		if(isMenuopen == 0){
			obj.src = "/images/arrow-top.gif"
			$("ClientList_Block_PC").style.display = 'block';
			document.form.PC_devicename.focus();
			isMenuopen = 1;
		}
		else
			hideClients_Block();
    }

    var over_var = 0;
    var isMenuopen = 0;
    function hideClients_Block(){
			$("pull_arrow").src = "/images/arrow-down.gif";
			$('ClientList_Block_PC').style.display='none';
			isMenuopen = 0;
    }

    /*----------} Mouse event of fake LAN IP select menu-----------------*/
    function gen_mainTable(){
		var code = "";
		code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="FormTable_table" id="mainTable_table">';
		code +='<thead><tr><td colspan="5"><%tcWebApi_get("String_Entry","ConnectedClient","s")%>&nbsp;(<%tcWebApi_get("String_Entry","List_limit","s")%>&nbsp;7)</td></tr></thead>';
		code +='<tr><th width="5%" height="30px" title="<%tcWebApi_get("String_Entry","select_all","s")%>"><input type=\"checkbox\" onclick=\"selectAll(this, 0);\" value=\"\"/><\/th>';
		code +='<th width="40%"><% tcWebApi_Get("String_Entry", "ParentalCtrl_username", "s") %><\/th>';
		code +='<th width="25%"><% tcWebApi_Get("String_Entry", "ParentalCtrl_hwaddr", "s") %><\/th>';
		code +='<th width="10%"><% tcWebApi_Get("String_Entry", "ParentalCtrl_time", "s") %><\/th>';
		code +='<th width="10%"><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %><\/th><\/tr>';
		code +='<tr><td style="border-bottom:2px solid #000;" title="<%tcWebApi_get("String_Entry","btn_Enabled","s")%>/<%tcWebApi_get("String_Entry","btn_disable","s")%>"><input type=\"checkbox\" id="newrule_Enable" checked><\/td>';
		code +='<td style="border-bottom:2px solid #000;"><input type="text" maxlength="32" style="margin-left:10px;float:left;width:255px;" class="input_20_table" name="PC_devicename" onKeyPress="" onClick="hideClients_Block();" onblur="if(!over_var){hideClients_Block();}">';
		code +='<img id="pull_arrow" height="14px;" src="/images/arrow-down.gif" onclick="pullLANIPList(this);" title="<% tcWebApi_Get("String_Entry", "select_client", "s") %>" onmouseover="over_var=1;" onmouseout="over_var=0;"><\/td>';
		code +='<td style="border-bottom:2px solid #000;"><input type="text" maxlength="17" class="input_macaddr_table" name="PC_mac" onKeyPress="return is_hwaddr(this,event)"><\/td>';
		code +='<td style="border-bottom:2px solid #000;">--<\/td>';
		code +='<td style="border-bottom:2px solid #000;"><input class="url_btn" type="button" onClick="addRow_main(10)" value=""><\/td><\/tr>';
		if(MULTIFILTER_DEVICENAME == "" && MULTIFILTER_MAC == "")
		code +='<tr><td style="color:#FFCC00;" colspan="10"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td>';
		else{
			var Ctrl_enable= "";
			for(var i=0; i<MULTIFILTER_DEVICENAME_row.length; i++){
				code +='<tr id="row'+i+'">';
				if(MULTIFILTER_ENABLE_row[i] == 1)
						Ctrl_enable= "<%tcWebApi_get("String_Entry","btn_Enabled","s")%>";
				else if(MULTIFILTER_ENABLE_row[i] == 0)
						Ctrl_enable= "<%tcWebApi_get("String_Entry","btn_Disabled","s")%>";

			/*code +='<td title="'+MULTIFILTER_ENABLE_row[i]+'"><input type=\"checkbox\" onclick=\"genEnableArray_main('+i+',this);\" '+genChecked(MULTIFILTER_ENABLE_row[i])+'/></td>';*/
			code +='<td title="'+ Ctrl_enable +'"><input type=\"checkbox\" onclick=\"genEnableArray_main('+i+',this);\" '+genChecked(MULTIFILTER_ENABLE_row[i])+'/></td>';
			code +='<td title="'+MULTIFILTER_DEVICENAME_row[i]+'">'+ MULTIFILTER_DEVICENAME_row[i] +'</td>';
			code +='<td title="'+MULTIFILTER_MAC_row[i]+'">'+ MULTIFILTER_MAC_row[i] +'</td>';
			code +='<td><input class=\"service_btn\" type=\"button\" onclick=\"gen_lantowanTable('+i+');" value=\"\"/></td>';
			code +='<td><input class=\"remove_btn\" type=\"button\" onclick=\"deleteRow_main(this);\" value=\"\"/></td>';
			}
		}
		code +='<\/tr><\/table>';
		$("mainTable").innerHTML = code;
	$("ctrlBtn").innerHTML = '<input class="button_gen" type="button" onClick="applyRule(0);" value="<%tcWebApi_get("String_Entry","btn_disable","s")%>"><input class="button_gen" type="button" onClick="applyRule(1);" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>">';

		if(document.form.MULTIFILTER_ALL.value == 0){
		$("mainTable").style.display = "none";
		$("ctrlBtn").innerHTML = '<input class="button_gen" type="button" onClick="applyRule(1);" value="<%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>">'
		}
    }
    function selectAll(obj, tab){
    var tag_name= document.getElementsByTagName('input');
    var tag = 0;
    for(var i=0;i<tag_name.length;i++){
    if(tag_name[i].type == "checkbox"){
    if(tab == 1){
    tag++;
    if(tag > 7)
    tag_name[i].checked = obj.checked;
    }
    else
    tag_name[i].checked = obj.checked;
    }
    }
    }
    function genEnableArray_main(j, obj){
		MULTIFILTER_ENABLE_row = MULTIFILTER_ENABLE.split('>');
		if(obj.checked){
		obj.parentNode.title = "<%tcWebApi_get("String_Entry","btn_Enabled","s")%>";
			MULTIFILTER_ENABLE_row[j] = "1";
		}
		else{
		obj.parentNode.title = "<%tcWebApi_get("String_Entry","btn_Disabled","s")%>";
			MULTIFILTER_ENABLE_row[j] = "0";
		}
		MULTIFILTER_ENABLE = "";
		for(i=0; i<MULTIFILTER_ENABLE_row.length; i++){
			MULTIFILTER_ENABLE += MULTIFILTER_ENABLE_row[i];
			if(i<MULTIFILTER_ENABLE_row.length-1)
				MULTIFILTER_ENABLE += ">";
		}
    }
    function genEnableArray_lantowan(j, client, obj){
    MULTIFILTER_LANTOWAN_ENABLE_col = MULTIFILTER_LANTOWAN_ENABLE_row[client].split('<');
    if(obj.checked){
    obj.parentNode.title = "1";
    MULTIFILTER_LANTOWAN_ENABLE_col[j] = "1";
    }
    else{
    obj.parentNode.title = "0";
    MULTIFILTER_LANTOWAN_ENABLE_col[j] = "0";
    }
    MULTIFILTER_LANTOWAN_ENABLE_row[client] = "";
    for(i=0;i<MULTIFILTER_LANTOWAN_ENABLE_col.length;i++){
    MULTIFILTER_LANTOWAN_ENABLE_row[client] += MULTIFILTER_LANTOWAN_ENABLE_col[i];
    if(i<MULTIFILTER_LANTOWAN_ENABLE_col.length-1)
    MULTIFILTER_LANTOWAN_ENABLE_row[client] += "<";
    }
    }
    function applyRule(_on){
    document.form.MULTIFILTER_ALL.value = _on;
    document.form.MULTIFILTER_ENABLE.value = MULTIFILTER_ENABLE;
    document.form.MULTIFILTER_MAC.value = MULTIFILTER_MAC;
    document.form.MULTIFILTER_DEVICENAME.value = MULTIFILTER_DEVICENAME;
    MULTIFILTER_MACFILTER_DAYTIME = Add_split_symbol(MULTIFILTER_MACFILTER_DAYTIME);
    document.form.MULTIFILTER_MACFILTER_DAYTIME.value = MULTIFILTER_MACFILTER_DAYTIME;
    document.form.MULTIFILTER_LANTOWAN_ENABLE.value = MULTIFILTER_LANTOWAN_ENABLE;
		document.form.MULTIFILTER_LANTOWAN_DESC.value = MULTIFILTER_LANTOWAN_DESC;
    document.form.MULTIFILTER_LANTOWAN_PORT.value = MULTIFILTER_LANTOWAN_PORT;
    document.form.MULTIFILTER_LANTOWAN_PROTO.value = MULTIFILTER_LANTOWAN_PROTO;

		var restart_time = 2;
		showLoading(restart_time, "waiting");
		document.form.editFlag.value = "1" ;
		setTimeout('test_delay_redirect("/cgi-bin/ParentalControl.asp");', restart_time*1000);
	}

	function Add_split_symbol(list){
		var newlist=list;
		if(list.length>500){
			var split_counts = Math.floor(list.length/500);
			for(var i=split_counts;i>0;i--){
				newlist = newlist.splice(i*500,0,"#");
			}
			return newlist;
		}
		else
			return list;
	}

	function test_delay_redirect(next_url){
		document.form.action = next_url;
		document.form.target = "_parent";
		document.form.SaveFlag.value = 1;
		document.form.submit();
	}
    function genChecked(_flag){
		if(_flag == 1)
		return "checked";
		else
		return "";
    }
    function showclock(){
    if(StopTimeCount == 1)
    return false;
    JS_timeObj.setTime(systime_millsec);
    systime_millsec += 1000;
    JS_timeObj2 = JS_timeObj.toString();
    JS_timeObj2 = JS_timeObj2.substring(0,3) + ", " +
    JS_timeObj2.substring(4,10) + " " +
    checkTime(JS_timeObj.getHours()) + ":" +
    checkTime(JS_timeObj.getMinutes()) + ":" +
    checkTime(JS_timeObj.getSeconds()) + " " +
    JS_timeObj.getFullYear();
    $("system_time").value = JS_timeObj2;
    setTimeout("showclock()", 1000);
    corrected_timezone(DAYLIGHT_orig, TZ_orig);
    }
   
    function gen_lantowanTable(client){
		_client = client;
		var code = "";
		code +='<div style="margin-bottom:10px;color: #003399;font-family: Verdana;" align="left">';
		if(parental2_support != -1){
			code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="FormTable">';
			code +='<thead><tr><td colspan="6" id="LWFilterList"><%tcWebApi_get("String_Entry","ParentalCtrl_Act_schedule","s")%><\/td><\/tr><\/thead>';
			code +='<tr><th width="20%"><%tcWebApi_get("String_Entry","General_x_SystemTime_in","s")%><\/th><td>';
			code +='<input type="text" id="system_time" name="system_time" class="devicepin" value="" readonly="1" style="font-size:12px;width:200px;">';
			code +='<br><span id="timezone_hint" onclick="location.href=\'Advanced_System_Content.asp?af=uiViewdateToolsTZ\'" style="display:none;text-decoration:underline;cursor:pointer;"><\/span><\/td><\/tr>';
			code +='<tr>';
			code +='<th style="width:40%;height:20px;" align="right">Client<\/th>';
			if(MULTIFILTER_DEVICENAME_row[client] != "")
				code +='<td align="left" style="color:#FFF">'+ MULTIFILTER_DEVICENAME_row[client] + '<\/td><\/tr>';
			else
				code +='<td align="left" style="color:#FFF">'+ MULTIFILTER_MAC_row[client] + '<\/td><\/tr>';
			code +='<\/table>';
			code +='<\/div><div id="calendar" style="margin:0;font-size:13px;margin-top:-10px;"><\/div>';
			code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="FormTable" style="display:none;">';
		}
		else
			code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="FormTable">';
		code +='<thead><tr><td colspan="6" id="LWFilterList"><%tcWebApi_get("String_Entry","ParentalCtrl_Act_schedule","s")%><\/td><\/tr><\/thead>';
		code +='<tr>';
		code +='<th width="40%" height="30px;" align="right">Client<\/th>';
		if(MULTIFILTER_DEVICENAME_row[client] != "")
			code +='<td align="left" style="color:#FFF">'+ MULTIFILTER_DEVICENAME_row[client] + '<\/td><\/tr>';
		else
			code +='<td align="left" style="color:#FFF">'+ MULTIFILTER_MAC_row[client] + '<\/td><\/tr>';
		code +='<tr id="url_time">';
		code +='<th width="40%" height="30px;" align="right">Allowed access time<\/th>';
		code +='<td align="left" style="color:#FFF">';
		code +='<input type="text" maxlength="2" class="input_3_table" name="url_time_x_starthour" onKeyPress="return is_number(this,event)" value='+MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(7,2)+'>:';
		code +='<input type="text" maxlength="2" class="input_3_table" name="url_time_x_startmin" onKeyPress="return is_number(this,event);" onblur="validate_timerange(this, 1);" value='+MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(9,2)+'>-';
		code +='<input type="text" maxlength="2" class="input_3_table" name="url_time_x_endhour" onKeyPress="return is_number(this,event);" onblur="validate_timerange(this, 2);" value='+MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(11,2)+'>:';
		code +='<input type="text" maxlength="2" class="input_3_table" name="url_time_x_endmin" onKeyPress="return is_number(this,event);" onblur="validate_timerange(this, 3);" value='+MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(13,2)+'>';
		code +='<\/td><\/tr><tr>';
		code +='<th width="40%" height="25px;" align="right">Allowed access date<\/th>';
		code +='<td align="left" style="color:#FFF">';
		code +='<input type="checkbox" id="url_date_x_Sun" class="input" onChange="return changeDate();" '+genChecked(MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(0,1))+'>Sun';
		code +='<input type="checkbox" id="url_date_x_Mon" class="input" onChange="return changeDate();" '+genChecked(MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(1,1))+'>Mon';
		code +='<input type="checkbox" id="url_date_x_Tue" class="input" onChange="return changeDate();" '+genChecked(MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(2,1))+'>Tue';
		code +='<input type="checkbox" id="url_date_x_Wed" class="input" onChange="return changeDate();" '+genChecked(MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(3,1))+'>Wed';
		code +='<input type="checkbox" id="url_date_x_Thu" class="input" onChange="return changeDate();" '+genChecked(MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(4,1))+'>Thu';
		code +='<input type="checkbox" id="url_date_x_Fri" class="input" onChange="return changeDate();" '+genChecked(MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(5,1))+'>Fri';
		code +='<input type="checkbox" id="url_date_x_Sat" class="input" onChange="return changeDate();" '+genChecked(MULTIFILTER_MACFILTER_DAYTIME_row[client].substr(6,1))+'>Sat';
		code +='<\/td><\/tr><\/table>';
		code +='<table width="100%" style="margin-top:10px;display:none;" border="1" cellspacing="0" cellpadding="4" align="center" class="PC_table" id="lantowanTable_table">';
		code +='<thead><tr><td colspan="6" id="LWFilterList">LAN to WAN Filter Table<\/td><\/tr><\/thead>';
		code +='<tr><th width="5%" height="30px;"><input type=\"checkbox\" onclick=\"selectAll(this, 1);\"/><\/th>';
	code +='<th width="35%"><%tcWebApi_get("String_Entry","BM_UserList1","s")%></th>';
	code +='<th width="30%"><%tcWebApi_get("String_Entry","FC_LanWanSrcPort_in","s")%></th>';
	code +='<th width="20%"><%tcWebApi_get("String_Entry","IPC_VServerProto_in","s")%></th>';
		code +='<th width="10%"><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %><\/th><\/tr>';
		code +='<tr><td style="border-bottom:2px solid #666;"><input type=\"checkbox\" id="newrule_lantowan_Enable" checked><\/td>';
		code +='<td style="border-bottom:2px solid #666;"><input type="text" maxlength="32" name="lantowan_service" onKeyPress="return is_string(this, event)"><\/td>';
		code +='<td style="border-bottom:2px solid #666;"><input type="text" maxlength="32" name="lantowan_port" onKeyPress="return is_string(this, event)"><\/td>';
		code +='<td style="border-bottom:2px solid #666;"><select name="lantowan_proto" class="input"><option value="TCP">TCP<\/option><option value="TCP ALL">TCP ALL<\/option><option value="TCP SYN">TCP SYN<\/option><option value="TCP ACK">TCP ACK<\/option><option value="TCP FIN">TCP FIN<\/option><option value="TCP RST">TCP RST<\/option><option value="TCP URG">TCP URG<\/option><option value="TCP PSH">TCP PSH<\/option><option value="UDP">UDP<\/option><\/select><\/td>';
		code +='<td style="border-bottom:2px solid #666;"><input class="url_btn" type="button" onClick="addRow_lantowan('+client+');" value=""><\/td><\/tr>';
		if(MULTIFILTER_LANTOWAN_DESC_row[client] == "" && MULTIFILTER_LANTOWAN_PORT_row[client] == "" && MULTIFILTER_LANTOWAN_PROTO_row[client] == "")
		code +='<tr><td style="color:#FFCC00;" colspan="10"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td>';
		else{
			var MULTIFILTER_LANTOWAN_ENABLE_col = MULTIFILTER_LANTOWAN_ENABLE_row[client].split('<');
			var MULTIFILTER_LANTOWAN_DESC_col = MULTIFILTER_LANTOWAN_DESC_row[client].split('<');
			var MULTIFILTER_LANTOWAN_PORT_col = MULTIFILTER_LANTOWAN_PORT_row[client].split('<');
			var MULTIFILTER_LANTOWAN_PROTO_col = MULTIFILTER_LANTOWAN_PROTO_row[client].split('<');
			for(var i=0; i<MULTIFILTER_LANTOWAN_DESC_col.length; i++){
				code +='<tr id="row'+i+'">';
				code +='<td title="'+MULTIFILTER_LANTOWAN_ENABLE_col[i]+'"><input type=\"checkbox\" onclick=\"genEnableArray_lantowan('+i+','+client+',this);\" '+genChecked(MULTIFILTER_LANTOWAN_ENABLE_col[i])+'><\/td>';
				code +='<td title="'+MULTIFILTER_LANTOWAN_DESC_col[i]+'">'+ MULTIFILTER_LANTOWAN_DESC_col[i] +'<\/td>';
				code +='<td title="'+MULTIFILTER_LANTOWAN_PORT_col[i]+'">'+ MULTIFILTER_LANTOWAN_PORT_col[i] +'<\/td>';
				code +='<td title="'+MULTIFILTER_LANTOWAN_PROTO_col[i]+'">'+ MULTIFILTER_LANTOWAN_PROTO_col[i] +'<\/td>';
				code +='<td><input class=\"remove_btn\" type=\"button\" onclick=\"deleteRow_lantowan(this,'+client+');\" value=\"\"/><\/td>';
			}
		}
		code +='<\/tr><\/table>';
		$("mainTable").innerHTML = code;
		$("ctrlBtn").innerHTML = '<input class="button_gen" type="button" onClick="cancel_lantowan('+client+');" value="<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>">';
		$("ctrlBtn").innerHTML += '<input class="button_gen" type="button" onClick="saveto_lantowan('+client+');" value="<%tcWebApi_get("String_Entry","CTL_ok","s")%>">';
		if(parental2_support != -1)
		generateCalendar(client);
		StopTimeCount = 0;
		showclock();
    }
    function regen_lantowan(){
		MULTIFILTER_LANTOWAN_ENABLE = "";
		MULTIFILTER_LANTOWAN_DESC = "";
		MULTIFILTER_LANTOWAN_PORT = "";
		MULTIFILTER_LANTOWAN_PROTO = "";
		MULTIFILTER_MACFILTER_DAYTIME = "";

		for(i=0;i<MULTIFILTER_LANTOWAN_DESC_row.length;i++){
			MULTIFILTER_LANTOWAN_ENABLE += MULTIFILTER_LANTOWAN_ENABLE_row[i];
			MULTIFILTER_LANTOWAN_DESC += MULTIFILTER_LANTOWAN_DESC_row[i];
			MULTIFILTER_LANTOWAN_PORT += MULTIFILTER_LANTOWAN_PORT_row[i];
			MULTIFILTER_LANTOWAN_PROTO += MULTIFILTER_LANTOWAN_PROTO_row[i];
			MULTIFILTER_MACFILTER_DAYTIME += MULTIFILTER_MACFILTER_DAYTIME_row[i];
			if(i<MULTIFILTER_LANTOWAN_DESC_row.length-1){
				MULTIFILTER_LANTOWAN_ENABLE += ">";
				MULTIFILTER_LANTOWAN_DESC += ">";
				MULTIFILTER_LANTOWAN_PORT += ">";
				MULTIFILTER_LANTOWAN_PROTO += ">";
				MULTIFILTER_MACFILTER_DAYTIME += ">";
			}
		}
	}
    function saveto_lantowan(client){
		if(parental2_support == -1){
			var starttime = eval(document.form.url_time_x_starthour.value + document.form.url_time_x_startmin.value);
			var endtime = eval(document.form.url_time_x_endhour.value + document.form.url_time_x_endmin.value);
			if(!validate_timerange(document.form.url_time_x_starthour, 0)
			|| !validate_timerange(document.form.url_time_x_startmin, 1)
			|| !validate_timerange(document.form.url_time_x_endhour, 2)
			|| !validate_timerange(document.form.url_time_x_endmin, 3)
			)
				return false;
			if(starttime > endtime || starttime == endtime){
				alert("<%tcWebApi_get("String_Entry","FC_URLActiveTime_itemhint","s")%>");
				return false;
			}
			MULTIFILTER_MACFILTER_DAYTIME_row[client] = "";
			if($("url_date_x_Sun").checked)
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "1";
			else
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "0";
			if($("url_date_x_Mon").checked)
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "1";
			else
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "0";
			if($("url_date_x_Tue").checked)
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "1";
			else
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "0";
			if($("url_date_x_Wed").checked)
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "1";
			else
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "0";
			if($("url_date_x_Thu").checked)
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "1";
			else
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "0";
			if($("url_date_x_Fri").checked)
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "1";
			else
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "0";
			if($("url_date_x_Sat").checked)
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "1";
			else
				MULTIFILTER_MACFILTER_DAYTIME_row[client] += "0";
			MULTIFILTER_MACFILTER_DAYTIME_row[client] += document.form.url_time_x_starthour.value;
			MULTIFILTER_MACFILTER_DAYTIME_row[client] += document.form.url_time_x_startmin.value;
			MULTIFILTER_MACFILTER_DAYTIME_row[client] += document.form.url_time_x_endhour.value;
			MULTIFILTER_MACFILTER_DAYTIME_row[client] += document.form.url_time_x_endmin.value;
		}
		StopTimeCount = 1; //Jieming added, to avoid a JavaScript error
		regen_lantowan();
		gen_mainTable();
    }
    function cancel_lantowan(client){
    StopTimeCount = 1;
    MULTIFILTER_LANTOWAN_ENABLE_row_tmp = MULTIFILTER_LANTOWAN_ENABLE.split('>');
    MULTIFILTER_LANTOWAN_DESC_row_tmp = MULTIFILTER_LANTOWAN_DESC.split('>');
    MULTIFILTER_LANTOWAN_PORT_row_tmp = MULTIFILTER_LANTOWAN_PORT.split('>');
    MULTIFILTER_LANTOWAN_PROTO_row_tmp = MULTIFILTER_LANTOWAN_PROTO.split('>');
    MULTIFILTER_MACFILTER_DAYTIME_row_tmp = MULTIFILTER_MACFILTER_DAYTIME.split('>');
    MULTIFILTER_LANTOWAN_ENABLE_row[client] = MULTIFILTER_LANTOWAN_ENABLE_row_tmp[client];
    MULTIFILTER_LANTOWAN_DESC_row[client] = MULTIFILTER_LANTOWAN_DESC_row_tmp[client];
    MULTIFILTER_LANTOWAN_PORT_row[client] = MULTIFILTER_LANTOWAN_PORT_row_tmp[client];
    MULTIFILTER_LANTOWAN_PROTO_row[client] = MULTIFILTER_LANTOWAN_PROTO_row_tmp[client];
    MULTIFILTER_MACFILTER_DAYTIME_row[client] = MULTIFILTER_MACFILTER_DAYTIME_row_tmp[client];
    gen_mainTable();
    }
    function addRow_main(upper){
		var rule_num = $('mainTable_table').rows.length;
		var item_num = $('mainTable_table').rows[0].cells.length;

		if(rule_num >= upper){	//minus 3 rows for editing
			alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> 7 <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
			return false;
		}
		if(!validate_string(document.form.PC_devicename))
		return false;
		if(document.form.PC_devicename.value == ""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			document.form.PC_devicename.focus();
			return false;
		}
		
		for(var i = 0; i < document.form.PC_devicename.value.length; ++i){
			if(document.form.PC_devicename.value.charAt(i) == '<' || document.form.PC_devicename.value.charAt(i) == '>'){
				invalid_char += document.form.PC_devicename.value.charAt(i);
				document.form.PC_devicename.focus();
				alert("<%tcWebApi_get("String_Entry","JS_validstr2","s")%> ' "+invalid_char + " '");
				return false;			
			}
		}
		
		if(document.form.PC_mac.value == ""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			document.form.PC_mac.focus();
			return false;
		}
		if(MULTIFILTER_MAC.search(document.form.PC_mac.value) > -1){
		alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");
			document.form.PC_mac.focus();
			return false;
		}
		if(!check_hwaddr(document.form.PC_mac))
			return false;
		if(MULTIFILTER_DEVICENAME != "" || MULTIFILTER_MAC != ""){
			MULTIFILTER_ENABLE += ">";
			MULTIFILTER_DEVICENAME += ">";
			MULTIFILTER_MAC += ">";
		}
		if($("newrule_Enable").checked)
			MULTIFILTER_ENABLE += "1";
		else
			MULTIFILTER_ENABLE += "0";
		MULTIFILTER_DEVICENAME += document.form.PC_devicename.value;
		MULTIFILTER_MAC += document.form.PC_mac.value;
		if(MULTIFILTER_MACFILTER_DAYTIME != "")
		MULTIFILTER_MACFILTER_DAYTIME += ">";
		if(parental2_support != -1)
			MULTIFILTER_MACFILTER_DAYTIME += "<";
		else
			MULTIFILTER_MACFILTER_DAYTIME += "111111100002359";
		if(MULTIFILTER_LANTOWAN_ENABLE != "")
			MULTIFILTER_LANTOWAN_ENABLE += ">"
		MULTIFILTER_LANTOWAN_ENABLE += "0";
		if(MULTIFILTER_LANTOWAN_DESC != "")
			MULTIFILTER_LANTOWAN_DESC += ">"
		MULTIFILTER_LANTOWAN_DESC += " ";
		if(MULTIFILTER_LANTOWAN_PORT != "")
			MULTIFILTER_LANTOWAN_PORT += ">"
		MULTIFILTER_LANTOWAN_PORT += " ";
		if(MULTIFILTER_LANTOWAN_PROTO != "")
			MULTIFILTER_LANTOWAN_PROTO += ">"
		MULTIFILTER_LANTOWAN_PROTO += " ";
		MULTIFILTER_ENABLE_row = MULTIFILTER_ENABLE.split('>');
		MULTIFILTER_DEVICENAME_row = MULTIFILTER_DEVICENAME.split('>');
		MULTIFILTER_MAC_row = MULTIFILTER_MAC.split('>');
		MULTIFILTER_LANTOWAN_ENABLE_row = MULTIFILTER_LANTOWAN_ENABLE.split('>');
		MULTIFILTER_LANTOWAN_DESC_row = MULTIFILTER_LANTOWAN_DESC.split('>');
		MULTIFILTER_LANTOWAN_PORT_row = MULTIFILTER_LANTOWAN_PORT.split('>');
		MULTIFILTER_LANTOWAN_PROTO_row = MULTIFILTER_LANTOWAN_PROTO.split('>');
		MULTIFILTER_MACFILTER_DAYTIME_row = MULTIFILTER_MACFILTER_DAYTIME.split('>');
		document.form.PC_devicename.value = "";
		document.form.PC_mac.value = "";
		gen_mainTable();
    }
    function deleteRow_main(r) {
		var j = r.parentNode.parentNode.rowIndex;
		$(r.parentNode.parentNode.parentNode.parentNode.id).deleteRow(j);

		var MULTIFILTER_ENABLE_tmp = "";
		var MULTIFILTER_MAC_tmp = "";
		var MULTIFILTER_DEVICENAME_tmp = "";
		for(i = 3; i < $('mainTable_table').rows.length; i++) {
			MULTIFILTER_ENABLE_tmp += $('mainTable_table').rows[i].cells[0].title;
			MULTIFILTER_DEVICENAME_tmp += $('mainTable_table').rows[i].cells[1].title;
			MULTIFILTER_MAC_tmp += $('mainTable_table').rows[i].cells[2].title;

			if(i != $('mainTable_table').rows.length-1) {
				MULTIFILTER_ENABLE_tmp += ">";
				MULTIFILTER_DEVICENAME_tmp += ">";
				MULTIFILTER_MAC_tmp += ">";
			}
		}

		MULTIFILTER_ENABLE = MULTIFILTER_ENABLE_tmp;
		MULTIFILTER_MAC = MULTIFILTER_MAC_tmp;
		MULTIFILTER_DEVICENAME = MULTIFILTER_DEVICENAME_tmp;

		MULTIFILTER_ENABLE_row = MULTIFILTER_ENABLE.split('>');
		MULTIFILTER_MAC_row = MULTIFILTER_MAC.split('>');
		MULTIFILTER_DEVICENAME_row = MULTIFILTER_DEVICENAME.split('>');

		MULTIFILTER_LANTOWAN_ENABLE_row.splice(j-3,1);
		MULTIFILTER_LANTOWAN_DESC_row.splice(j-3,1);
		MULTIFILTER_LANTOWAN_PORT_row.splice(j-3,1);
		MULTIFILTER_LANTOWAN_PROTO_row.splice(j-3,1);
		MULTIFILTER_MACFILTER_DAYTIME_row.splice(j-3,1);

		regen_lantowan();
		gen_mainTable();
	}
    function addRow_lantowan(client){
		var invalid_char = "";
		if(MULTIFILTER_LANTOWAN_DESC_row[client] != "" || MULTIFILTER_LANTOWAN_PROTO_row[client] != "" || MULTIFILTER_LANTOWAN_PORT_row[client] != ""){
			MULTIFILTER_LANTOWAN_ENABLE_row[client] += "<";
			MULTIFILTER_LANTOWAN_DESC_row[client] += "<";
			MULTIFILTER_LANTOWAN_PORT_row[client] += "<";
			MULTIFILTER_LANTOWAN_PROTO_row[client] += "<";
		}
		if($("newrule_lantowan_Enable").checked)
			MULTIFILTER_LANTOWAN_ENABLE_row[client] += "1";
		else
			MULTIFILTER_LANTOWAN_ENABLE_row[client] += "0";
		MULTIFILTER_LANTOWAN_DESC_row[client] += document.form.lantowan_service.value;
		MULTIFILTER_LANTOWAN_PROTO_row[client] += document.form.lantowan_proto.value;
		MULTIFILTER_LANTOWAN_PORT_row[client] += document.form.lantowan_port.value;
		document.form.lantowan_service.value = "";
		document.form.lantowan_port.value = "";
		document.form.lantowan_proto.value = "";
		gen_lantowanTable(client);
    }
    function deleteRow_lantowan(r, client){
    var i=r.parentNode.parentNode.rowIndex;
    $(r.parentNode.parentNode.parentNode.parentNode.id).deleteRow(i);
    var MULTIFILTER_LANTOWAN_ENABLE_tmp = "";
    var MULTIFILTER_LANTOWAN_DESC_tmp = "";
    var MULTIFILTER_LANTOWAN_PORT_tmp = "";
    var MULTIFILTER_LANTOWAN_PROTO_tmp = "";
    for(i=2; i<$('lantowanTable_table').rows.length; i++){
    MULTIFILTER_LANTOWAN_ENABLE_tmp += $('lantowanTable_table').rows[i].cells[0].title;
    MULTIFILTER_LANTOWAN_DESC_tmp += $('lantowanTable_table').rows[i].cells[1].title;
    MULTIFILTER_LANTOWAN_PORT_tmp += $('lantowanTable_table').rows[i].cells[2].title;
    MULTIFILTER_LANTOWAN_PROTO_tmp += $('lantowanTable_table').rows[i].cells[3].title;
    if(i != $('lantowanTable_table').rows.length-1){
    MULTIFILTER_LANTOWAN_ENABLE_tmp += "<";
    MULTIFILTER_LANTOWAN_DESC_tmp += "<";
    MULTIFILTER_LANTOWAN_PORT_tmp += "<";
    MULTIFILTER_LANTOWAN_PROTO_tmp += "<";
    }
    }
    MULTIFILTER_LANTOWAN_ENABLE_row[client] = MULTIFILTER_LANTOWAN_ENABLE_tmp;
    MULTIFILTER_LANTOWAN_DESC_row[client] = MULTIFILTER_LANTOWAN_DESC_tmp;
    MULTIFILTER_LANTOWAN_PORT_row[client] = MULTIFILTER_LANTOWAN_PORT_tmp;
    MULTIFILTER_LANTOWAN_PROTO_row[client] = MULTIFILTER_LANTOWAN_PROTO_tmp;
    if(MULTIFILTER_LANTOWAN_DESC_row[client] == "")
    gen_lantowanTable(client);
    }
    </script>
</head>

<body onload="initial();" onunload="unload_body();">
    <div id="TopBanner"></div>
    <div id="Loading" class="popup_bg"></div>
    <iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>

    <form method="post" name="form" action="/cgi-bin/ParentalControl.asp" target="hidden_frame">
    <input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
		<input type="hidden" name="current_page" value="/cgi-bin/ParentalControl.asp">
		<input type="hidden" name="next_page" value="">
		<input type="hidden" name="next_host" value="">
		<input type="hidden" name="modified" value="0">
		<input type="hidden" name="action_wait" value="5">
		<input type="hidden" name="action_mode" value="apply">
		<input type="hidden" name="action_script" value="restart_firewall">
		<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN"><!--TODO: get the preffered_lang-->
		<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
		<input type="hidden" name="MULTIFILTER_ALL" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_ALL","s")%>">
		<input type="hidden" name="MULTIFILTER_ENABLE" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_ENABLE","s")%>">
		<input type="hidden" name="MULTIFILTER_MAC" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_MAC","s")%>">
		<input type="hidden" name="MULTIFILTER_DEVICENAME" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_DEVICENAME","s")%>">
		<input type="hidden" name="MULTIFILTER_MACFILTER_DAYTIME" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_MACFILTER_DAYTIME","s")%>">
		<input type="hidden" name="MULTIFILTER_LANTOWAN_ENABLE" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_ENABLE","s")%>">
		<input type="hidden" name="MULTIFILTER_LANTOWAN_DESC" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_DESC","s")%>">
		<input type="hidden" name="MULTIFILTER_LANTOWAN_PORT" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_PORT","s")%>">
		<input type="hidden" name="MULTIFILTER_LANTOWAN_PROTO" value="<%TCWebApi_get("Parental_Entry","MULTIFILTER_LANTOWAN_PROTO","s")%>">
		<input type="hidden" name="editFlag" value="0">
        <table class="content" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td width="17">&#160;</td>

                <td valign="top" width="202">
                    <div id="mainMenu"></div>

                    <div id="subMenu"></div>
                </td>

                <td valign="top">
                    <div id="tabMenu" class="submenuBlock" style="*margin-top:-155px;"></div>

                    <table width="98%" border="0" align="left" cellpadding="0" cellspacing="0" style="margin-top:5px;">
                        <tr>
                            <td valign="top">
                                <table width="730px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle" style="-webkit-border-radius:3px;-moz-border-radius:3px;border-radius:3px;">
                                    <tbody>
                                        <tr>
                                            <td bgcolor="#4D595D" valign="top">
                                                <div>
                                                    &#160;
                                                </div>

                                                <div class="formfonttitle">
                                                    <%tcWebApi_get("String_Entry","Parental_Control","s")%>
                                                </div>

                                                <div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div><!--div class="formfontdesc">Parental control allows you to set the time limit for a client's network usage. To use parental control:</div-->

                                                <div id="PC_desc">
                                                    <table width="700px" style="margin-left:25px;">
                                                        <tr>
                                                            <td><img id="guest_image" src="/images/New_ui/parental-control.png"></td>

                                                            <td>&#160;&#160;</td>

                                                            <td>
                                                                <span><%tcWebApi_get("String_Entry","ParentalCtrl_Desc","s")%></span>

                                                                <ol>
																	<li><%tcWebApi_get("String_Entry","ParentalCtrl_Desc1","s")%></li>
																	<li><%tcWebApi_get("String_Entry","ParentalCtrl_Desc2","s")%></li>
																	<li><%tcWebApi_get("String_Entry","ParentalCtrl_Desc3","s")%></li>
																	<li><%tcWebApi_get("String_Entry","ParentalCtrl_Desc4","s")%></li>
                                                                </ol>
																<ol style="color:#FC0;margin:-5px 0px 3px -18px;*margin-left:18px;"><%tcWebApi_get("String_Entry","ParentalCtrl_default","s")%></ol>

                                                            </td>
                                                        </tr>
                                                    </table>
                                                </div>

												<!--=====Beginning of Main Content=====-->
                                                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0" style="margin-left:30px;">
                                                    <tr>
                                                        <td valign="top" align="center">
															<!-- client info -->
                                                            <div id="ClientList_Block_PC" class="ClientList_Block_PC"></div>

                                                            <div id="VSList_Block"></div>
															<!-- Content -->
                                                            <div id="mainTable"></div>
                                                            <br>

                                                            <div id="ctrlBtn"></div>
															<INPUT TYPE="HIDDEN" NAME="SaveFlag" VALUE="0">
															<!-- Content -->
                                                        </td>
                                                    </tr>
                                                </table>
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </td>
                        </tr>
                    </table>
                </td>

                <td width="10" align="center" valign="top">&#160;</td>
            </tr>
        </table>

<!--copy from tools_times.asp of MTK.B-->

<!--copy from tools_times.asp of MTK.E-->

        <div id="footer"></div>
    </form><script type="text/javascript">
</script><!--ParentalControl.asp-->
</body>
</html>

