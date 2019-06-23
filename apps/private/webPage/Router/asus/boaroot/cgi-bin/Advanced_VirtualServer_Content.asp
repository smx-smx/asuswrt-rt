<%
If Request_Form("editFlag") = "1" then
	tcWebApi_Unset("VirServer_PVC0_Entry1")
	tcWebApi_set("VirServer_PVC0_Entry0","VirServer_Enable","vts_enable_x")
	tcWebApi_set("VirServer_PVC0_Entry0","VirServer_RuleList","vts_rulelist")

	'new rule list
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList1","vts_rulelist1")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList2","vts_rulelist2")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList3","vts_rulelist3")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList4","vts_rulelist4")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList5","vts_rulelist5")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList6","vts_rulelist6")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList7","vts_rulelist7")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList8","vts_rulelist6")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList9","vts_rulelist9")
	tcWebApi_set("VirServer_PVC0_Entry1","VirServer_RuleList10","vts_rulelist10")

	tcWebApi_set("VirServer_PVC0_Entry0","vts_ftpport","vts_ftpport")
	tcWebApi_commit("VirServer_PVC_Entry")
end if

%>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_VirtualServer_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_3_4","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<style>
#ClientList_Block{
border:1px outset #999;
background-color:#576D73;
position:absolute;
*margin-top:27px;
margin-left:2px;
*margin-left:-223px;
width:340px;
text-align:left;	
height:auto;
overflow-y:auto;
z-index:200;
padding: 1px;
display:none;
}
#ClientList_Block div{
background-color:#576D73;
height:auto;
*height:20px;
width:auto;
font-size:12px;
line-height:20px;
text-decoration:none;
font-family: Lucida Console;
padding-left:2px;
}
#ClientList_Block a{
background-color:#EFEFEF;
color:#FFF;
font-size:12px;
font-family:Arial, Helvetica, sans-serif;
text-decoration:none;
}
#ClientList_Block div:hover, #ClientList_Block a:hover{
background-color:#3366FF;
color:#FFFFFF;
cursor:default;
}
</style>
<script>
var $j = jQuery.noConflict();
var wItem = new Array(new Array("", "", "TCP"),
new Array("FTP", "20,21", "TCP"),
new Array("TELNET", "23", "TCP"),
new Array("SMTP", "25", "TCP"),
new Array("DNS", "53", "UDP"),
new Array("FINGER", "79", "TCP"),
new Array("HTTP", "80", "TCP"),
new Array("POP3", "110", "TCP"),
new Array("SNMP", "161", "UDP"),
new Array("SNMP TRAP", "162", "UDP"),
new Array("GRE", "47", "OTHER"),
new Array("IPv6 Tunnel", "41", "OTHER"));

var wItem2 = new Array(new Array("", "", "TCP"),
new Array("Age of Empires", "2302:2400,6073", "BOTH"),
new Array("BitTorrent", "6881:6889", "TCP"),
new Array("Counter Strike(TCP)", "27030:27039", "TCP"),
new Array("Counter Strike(UDP)", "27000:27015,1200", "UDP"),
new Array("PlayStation2", "4658,4659", "BOTH"),
new Array("Warcraft III", "6112:6119,4000", "BOTH"),
new Array("WOW", "3724", "BOTH"),
new Array("Xbox Live", "3074", "BOTH"));


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var client_ip = login_ip_str();
var client_mac = login_mac_str();
var overlib_str0 = new Array(); //Viz add 2011.07 for record longer virtual srvr rule desc
var overlib_str = new Array(); //Viz add 2011.07 for record longer virtual srvr portrange value
var leases = []; // [[hostname, MAC, ip, lefttime], ...]
var arps = []; // [[ip, x, x, MAC, x, type], ...]
var arls = []; // [[MAC, port, x, x], ...]
var wireless = []; // [[MAC, associated, authorized], ...]
var ipmonitor = []; // [[IP, MAC, DeviceName, Type, http, printer, iTune], ...]
var networkmap_fullscan = 'done'; //2008.07.24 Add. 1 stands for complete, 0 stands for scanning.;

var vts_rulelist_array = "<%TCWebApi_get("VirServer_PVC0_Entry0","VirServer_RuleList","s")%>";	//javi
var ctf_disable = '';

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

function initial(){
	show_menu();
	loadAppOptions();
	loadGameOptions();
	showLANIPList();
	loadNewvts_rulelist();
	showvts_rulelist();
	setTimeout("update_FAQ();", 300);

	if(parent.usb_support == -1){
		$('FTP_desc').style.display = "none";
		$('ori_ftpport').style.display = "none";
	}

}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq", "https://www.asus.com", "/support/FAQ/", "114093");
	}
}
function applyRule(){

	if(parent.usb_support != -1){
		if(!validate_number_range(document.form.vts_ftpport, 1, 65535)){
			return false;
		}
	}

	var rule_num = $('vts_rulelist_table').rows.length;
	var item_num = $('vts_rulelist_table').rows[0].cells.length;
	var tmp_value = "";
	var attrIndex = "";
	for(i=0; i<rule_num; i++){
		tmp_value += "<"
		for(j=0; j<item_num-1; j++){
			if($('vts_rulelist_table').rows[i].cells[j].innerHTML.lastIndexOf("...")<0){
				tmp_value += $('vts_rulelist_table').rows[i].cells[j].innerHTML;
			}else{
				tmp_value += $('vts_rulelist_table').rows[i].cells[j].title;
			}
			
			if(j == 1 && $('vts_rulelist_table').rows[i].cells[1].innerHTML != "")
				tmp_value += "@";
			else if(j == 1)	
				tmp_value += "";
			else if(j != item_num-2)
				tmp_value += ">";
		}

		if(( i == rule_num -1 ) || (i%6 == 5) )
		{
			if(tmp_value == "<")
			        tmp_value = "";
			attrIndex = Math.floor(i/6);
			switch(attrIndex)
			{
				case 0:
					document.form.vts_rulelist.value = tmp_value;
					break;
				case 1:
					document.form.vts_rulelist1.value = tmp_value;
					break;
				case 2:
					document.form.vts_rulelist2.value = tmp_value;
					break;
				case 3:
					document.form.vts_rulelist3.value = tmp_value;
					break;
				case 4:
					document.form.vts_rulelist4.value = tmp_value;
					break;
				case 5:
					document.form.vts_rulelist5.value = tmp_value;
					break;
				case 6:
					document.form.vts_rulelist6.value = tmp_value;
					break;
				case 7:
					document.form.vts_rulelist7.value = tmp_value;
					break;
				case 8:
					document.form.vts_rulelist8.value = tmp_value;
					break;
				case 9:
					document.form.vts_rulelist9.value = tmp_value;
					break;
				case 10:
					document.form.vts_rulelist10.value = tmp_value;
					break;
				default:
					document.form.vts_rulelist1.value = "Out of Limitation";
					break;
			}
			tmp_value = "";
		}
	}
	
	if(ctf_disable != 1){
		document.form.action_script.value = "reboot";
		document.form.action_wait.value = "60";
	}
	
	showLoading(5);
	setTimeout("redirect();", 5000);
	document.form.action = "/cgi-bin/Advanced_VirtualServer_Content.asp";	//javi
	document.form.editFlag.value = "1" ;
	document.form.submit();
}
function loadAppOptions(){
	free_options(document.form.KnownApps);
	add_option(document.form.KnownApps, "<% tcWebApi_Get("String_Entry", "Select_menu_default", "s") %>", 0, 1);
	for(var i = 1; i < wItem.length; i++)
	add_option(document.form.KnownApps, wItem[i][0], i, 0);
}
function loadGameOptions(){
	free_options(document.form.KnownGames);
	add_option(document.form.KnownGames, "<% tcWebApi_Get("String_Entry", "Select_menu_default", "s") %>", 0, 1);
	for(var i = 1; i < wItem2.length; i++)
	add_option(document.form.KnownGames, wItem2[i][0], i, 0);
}
function change_wizard(o, id){
if(id == "KnownApps"){
$("KnownGames").value = 0;
for(var i = 0; i < wItem.length; ++i){
if(wItem[i][0] != null && o.value == i){
if(wItem[i][2] == "TCP")
document.form.vts_proto_x_0.options[0].selected = 1;
else if(wItem[i][2] == "UDP")
document.form.vts_proto_x_0.options[1].selected = 1;
else if(wItem[i][2] == "BOTH")
document.form.vts_proto_x_0.options[2].selected = 1;
document.form.vts_ipaddr_x_0.value = client_ip;
document.form.vts_port_x_0.value = wItem[i][1];
document.form.vts_desc_x_0.value = wItem[i][0]+" Server";
break;
}
}
if(document.form.KnownApps.options[1].selected == 1){
$('ori_ftpport').style.display = "";
document.form.vts_lport_x_0.value = "21";
}else{
$('ori_ftpport').style.display = "none";
document.form.vts_lport_x_0.value = "";
}
}
else if(id == "KnownGames"){
$("KnownApps").value = 0;
for(var i = 0; i < wItem2.length; ++i){
if(wItem2[i][0] != null && o.value == i){
if(wItem2[i][2] == "TCP")
document.form.vts_proto_x_0.options[0].selected = 1;
else if(wItem2[i][2] == "UDP")
document.form.vts_proto_x_0.options[1].selected = 1;
else if(wItem2[i][2] == "BOTH")
document.form.vts_proto_x_0.options[2].selected = 1;
document.form.vts_ipaddr_x_0.value = client_ip;
document.form.vts_port_x_0.value = wItem2[i][1];
document.form.vts_desc_x_0.value = wItem2[i][0];
break;
}
}
}
}
/*------------ Mouse event of fake LAN IP select menu {-----------------*/
function setClientIP(num){
document.form.vts_ipaddr_x_0.value = num;
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

		if(clientObj.ip == "offline") clientObj.ip = "";
		if(clientObj.Name.length > 30) clientObj.Name = clientObj.Name.substring(0, 27) + "...";

		htmlCode += '<a><div onmouseover="over_var=1;" onmouseout="over_var=0;" onclick="setClientIP(\'';
		htmlCode += clientObj.IP;
		htmlCode += '\');"><strong>';
		htmlCode += clientObj.IP;
		htmlCode += '</strong> ( ';
		htmlCode += clientObj.Name;
		htmlCode += ' )</div></a><!--[if lte IE 6.5]><iframe class="hackiframe2"></iframe><![endif]-->';	
	}

	$("ClientList_Block").innerHTML = htmlCode;
}

function loadNewvts_rulelist(){
//to load new rule list
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList1","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList2","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList3","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList4","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList5","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList6","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList7","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList8","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList9","s")%>";
vts_rulelist_array += "<%TCWebApi_get("VirServer_PVC0_Entry1","VirServer_RuleList10","s")%>";
}

function pullLANIPList(obj){
	if(isMenuopen == 0){
		obj.src = "/images/arrow-top.gif"
		$("ClientList_Block").style.display = 'block';
		document.form.vts_ipaddr_x_0.focus();
		isMenuopen = 1;
	}
	else
		hideClients_Block();
}

var over_var = 0;
var isMenuopen = 0;

function hideClients_Block(){
	$("pull_arrow").src = "/images/arrow-down.gif";
	$('ClientList_Block').style.display='none';
	isMenuopen = 0;
	valid_IP_form(document.form.vts_ipaddr_x_0, 0);
}

/*----------} Mouse event of fake LAN IP select menu-----------------*/
function addRow(obj, head){
	if(head == 1)
		vts_rulelist_array += "<"
	else
		vts_rulelist_array += ">"

	vts_rulelist_array += obj.value;
	obj.value = "";
}
function addRow_Group(upper){
	if(validForm()){
		if("<%tcWebApi_get("VirServer_PVC0_Entry0","VirServer_Enable","s")%>" != "1")
				document.form.vts_enable_x[0].checked = true;

		var rule_num = $('vts_rulelist_table').rows.length;
		var item_num = $('vts_rulelist_table').rows[0].cells.length;
		if(rule_num >= upper){
				alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
				return false;
		}
	}else{
		return false;	
	}
		//Viz check same rule  //match(out port+out_proto) is not accepted
		if(item_num >=2){
			for(i=0; i<rule_num; i++){
				if(entry_cmp($('vts_rulelist_table').rows[i].cells[4].innerHTML.toLowerCase(), document.form.vts_proto_x_0.value.toLowerCase(), 3)==0
				|| document.form.vts_proto_x_0.value == 'BOTH'
				|| $('vts_rulelist_table').rows[i].cells[4].innerHTML == 'BOTH'){
					if(overlib_str[i]){
						if(document.form.vts_port_x_0.value == overlib_str[i]){
							alert('<%tcWebApi_get("String_Entry","JS_duplicate","s")%>');
							document.form.vts_port_x_0.value =="";
							document.form.vts_port_x_0.focus();
							document.form.vts_port_x_0.select();
							return false;
						}
					}else{
						if(document.form.vts_port_x_0.value == $('vts_rulelist_table').rows[i].cells[1].innerHTML){
							alert('<%tcWebApi_get("String_Entry","JS_duplicate","s")%>');
							document.form.vts_port_x_0.value =="";
							document.form.vts_port_x_0.focus();
							document.form.vts_port_x_0.select();
							return false;
						}
					}
				}
			}
		}
		addRow(document.form.vts_desc_x_0 ,1);
		if(document.form.vts_srcipaddr_x_0.value.length > 0){
			addRow(document.form.vts_srcipaddr_x_0, 0);
			vts_rulelist_array += "@"+document.form.vts_port_x_0.value;
		}
		else{
			addRow(document.form.vts_srcipaddr_x_0, 0);
			vts_rulelist_array += document.form.vts_port_x_0.value;
		}
		document.form.vts_port_x_0.value = "";
		addRow(document.form.vts_ipaddr_x_0, 0);
		addRow(document.form.vts_lport_x_0, 0);
		addRow(document.form.vts_proto_x_0, 0);		
		document.form.vts_proto_x_0.value= "TCP";
		showvts_rulelist();
}

function validForm(){
		if(!Block_chars(document.form.vts_desc_x_0, ["<" ,">" ,"'" ,"%"])){
				return false;
		}
		if(!Block_chars(document.form.vts_port_x_0, ["<" ,">"])){
				return false;
		}

		/*if(document.form.vts_proto_x_0.value=="OTHER"){
				document.form.vts_lport_x_0.value = "";
				if (!check_multi_range(document.form.vts_port_x_0, 1, 255, false))
						return false;
		}*/
		
		if(document.form.vts_srcipaddr_x_0.value.split("*").length >= 2){
					if(!valid_IP_subnet(document.form.vts_srcipaddr_x_0))
							return false;
		}
		else if(!valid_IP_form(document.form.vts_srcipaddr_x_0, 0))
					return false;
					
		if(!check_multi_range(document.form.vts_port_x_0, 1, 65535, true)){
				return false;
		}

		if(document.form.vts_lport_x_0.value.length > 0
			&& !validate_number_range(document.form.vts_lport_x_0, 1, 65535)){
				return false;
		}

		if(document.form.vts_ipaddr_x_0.value==""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			document.form.vts_ipaddr_x_0.focus();
			document.form.vts_ipaddr_x_0.select();
			return false;
		}
		if(document.form.vts_port_x_0.value==""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			document.form.vts_port_x_0.focus();
			document.form.vts_port_x_0.select();
			return false;
		}
		if(!validate_multi_range(document.form.vts_port_x_0, 1, 65535)
				|| !valid_IP_form(document.form.vts_ipaddr_x_0, 0)){
				return false;
		}

		return true;
}

var parse_port="";
function check_multi_range(obj, mini, maxi, allow_range){
	obj.value = document.form.vts_port_x_0.value.replace(/[-~]/gi,":");	// "~-" to ":"
	var PortSplit = obj.value.split(",");
	for(i=0;i<PortSplit.length;i++){
		PortSplit[i] = PortSplit[i].replace(/(^\s*)|(\s*$)/g, ""); 		// "\space" to ""
		PortSplit[i] = PortSplit[i].replace(/(^0*)/g, ""); 		// "^0" to ""	

		if(PortSplit[i] == "" ||PortSplit[i] == 0){
			alert("<%tcWebApi_get("String_Entry","JS_ipblank1","s")%>");
			obj.focus();
			obj.select();
			return false;
		}
		if(allow_range)
			res = validate_multi_range(PortSplit[i], mini, maxi);
		else	res = validate_single_range(PortSplit[i], mini, maxi);
		if(!res){
			obj.focus();
			obj.select();
			return false;
		}

		if(i ==PortSplit.length -1)
			parse_port = parse_port + PortSplit[i];
		else
			parse_port = parse_port + PortSplit[i] + ",";

	}
	document.form.vts_port_x_0.value = parse_port;
	parse_port ="";
	return true;
}

function validate_multi_range(val, mini, maxi){
	var rangere=new RegExp("^([0-9]{1,5})\:([0-9]{1,5})$", "gi");
	if(rangere.test(val)){
		if(!validate_each_port(document.form.vts_port_x_0, RegExp.$1, mini, maxi)
				|| !validate_each_port(document.form.vts_port_x_0, RegExp.$2, mini, maxi)){
					return false;
		}else if(parseInt(RegExp.$1) >= parseInt(RegExp.$2)){
					alert("<%tcWebApi_get("String_Entry","JS_validport","s")%>");
					return false;
		}else
					return true;
	}else{
		if(!validate_single_range(val, mini, maxi)){
					return false;
		}
		return true;
	}
}

function validate_single_range(val, min, max) {
	for(j=0; j<val.length; j++){ //is_number
		if (val.charAt(j)<'0' || val.charAt(j)>'9'){
				alert('<%tcWebApi_get("String_Entry","JS_validrange","s")%> ' + min + ' <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> ' + max);
				return false;
		}
	}
	if(val < min || val > max) { //is_in_range
		alert('<%tcWebApi_get("String_Entry","JS_validrange","s")%> ' + min + ' <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> ' + max);
		return false;
	}else
		return true;
}

function edit_Row(r){
	var i=r.parentNode.parentNode.rowIndex;
	document.form.vts_desc_x_0.value = $('vts_rulelist_table').rows[i].cells[0].innerHTML;
	document.form.vts_port_x_0.value = $('vts_rulelist_table').rows[i].cells[1].innerHTML;
	document.form.vts_ipaddr_x_0.value = $('vts_rulelist_table').rows[i].cells[2].innerHTML;
	document.form.vts_lport_x_0.value = $('vts_rulelist_table').rows[i].cells[3].innerHTML;
	document.form.vts_proto_x_0.value = $('vts_rulelist_table').rows[i].cells[4].innerHTML;
	del_Row(r);
}
function del_Row(r){
	var i=r.parentNode.parentNode.rowIndex;
	$('vts_rulelist_table').deleteRow(i);
	var vts_rulelist_value = "";
	for(k=0; k<$('vts_rulelist_table').rows.length; k++){
	for(j=0; j<$('vts_rulelist_table').rows[k].cells.length-1; j++){
	if(j == 0)
		vts_rulelist_value += "<";
	else if(j == 2 && $('vts_rulelist_table').rows[k].cells[1].innerHTML != "")
		vts_rulelist_value += "@";
	else if(j == 2)
		vts_rulelist_value += "";
	else
		vts_rulelist_value += ">";
	if($('vts_rulelist_table').rows[k].cells[j].innerHTML.lastIndexOf("...")<0){
	vts_rulelist_value += $('vts_rulelist_table').rows[k].cells[j].innerHTML;
	}else{
	vts_rulelist_value += $('vts_rulelist_table').rows[k].cells[j].title;
	}
	}
	}
	vts_rulelist_array = vts_rulelist_value;
	if(vts_rulelist_array == "")
	showvts_rulelist();
}
function showvts_rulelist(){
	var vts_rulelist_row = decodeURIComponent(vts_rulelist_array).split('<');
	var code = "";
	code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="vts_rulelist_table">';
	if(vts_rulelist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;" colspan="7"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td></tr>';
	else{
		for(var i = 1; i < vts_rulelist_row.length; i++){
			
			overlib_str0[i] ="";	//for service name overlib
			overlib_str[i] ="";		//for src port range overlib
			code +='<tr id="row'+i+'">';
			var vts_rulelist_col = vts_rulelist_row[i].split('>');
			var wid=[20, 17, 14, 22, 9, 10];
			for(var j = 0; j < vts_rulelist_col.length; j++){	
				if(j==0){
					if(vts_rulelist_col[0].length >23){
						overlib_str0[i] += vts_rulelist_col[0];
						vts_rulelist_col[0]=vts_rulelist_col[0].substring(0, 21)+"...";
						code +='<td width="'+wid[0]+'%" title="'+overlib_str0[i]+'">'+ vts_rulelist_col[0] +'</td>';
					}else
						code +='<td width="'+wid[0]+'%">'+ vts_rulelist_col[0] +'</td>';
				}
				else if(j==1){
					if(vts_rulelist_col[1].indexOf('@') >= 0){
						//add srcip@srcport 
						var vts_rulelist_srcipnport = vts_rulelist_col[1].split('@');
						code +='<td width="'+wid[1]+'%">'+ vts_rulelist_srcipnport[0] +'</td>';
					
						if(vts_rulelist_srcipnport[1].length >13){
							overlib_str[i] += vts_rulelist_srcipnport[1];
							vts_rulelist_srcipnport[1] = vts_rulelist_srcipnport[1].substring(0, 11)+"...";
							code +='<td width="'+wid[2]+'%" title='+overlib_str[i]+'>'+ vts_rulelist_srcipnport[1] +'</td>';
						}
						else
							code +='<td width="'+wid[2]+'%">'+ vts_rulelist_srcipnport[1] +'</td>';
					}
					else{
						
						code +='<td width="'+wid[1]+'%"></td>';	//src ip is null
						if(vts_rulelist_col[1].length >13){
							overlib_str[i] += vts_rulelist_col[1];
							vts_rulelist_col[1] = vts_rulelist_col[1].substring(0, 11)+"...";
							code +='<td width="'+wid[2]+'%" title='+overlib_str[i]+'>'+ vts_rulelist_col[1] +'</td>';	//src port range
						}
						else
							code +='<td width="'+wid[2]+'%">'+ vts_rulelist_col[1] +'</td>';		//src port range
					}
				}
				
				if(j > 1){//j push back cause add srcip
					var index = parseInt(j)+1;					
					code +='<td width="'+wid[index]+'%">'+ vts_rulelist_col[j] +'</td>';
				}
			}
			code +='<td width="8%"><input class="remove_btn" onclick="del_Row(this);" value=""/></td></tr>';
		}
	}
	code +='</table>';
	$("vts_rulelist_Block").innerHTML = code;
}
function changeBgColor(obj, num){
if(obj.checked)
$("row" + num).style.background='#FF9';
else
$("row" + num).style.background='#FFF';
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_VirtualServer_Content.asp";
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="start_apply.asp" target="hidden_frame" >
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_VirtualServer_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_firewall">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="vts_rulelist" value=''>
<input type="hidden" name="vts_rulelist1" value=''>
<input type="hidden" name="vts_rulelist2" value=''>
<input type="hidden" name="vts_rulelist3" value=''>
<input type="hidden" name="vts_rulelist4" value=''>
<input type="hidden" name="vts_rulelist5" value=''>
<input type="hidden" name="vts_rulelist6" value=''>
<input type="hidden" name="vts_rulelist7" value=''>
<input type="hidden" name="vts_rulelist8" value=''>
<input type="hidden" name="vts_rulelist9" value=''>
<input type="hidden" name="vts_rulelist10" value=''>
<input type="hidden" name="editFlag" value="0">
<table class="content" align="center" cellpadding="0" cellspacing="0" >
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
<td valign="top" >
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_3","s")%> - <%tcWebApi_get("String_Entry","menu5_3_4","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","IPC_VServerEnable_sd","s")%></div>
	<ul style="margin-left:-25px; *margin-left:10px;">
				<div class="formfontdesc"><li><%tcWebApi_get("String_Entry","FC_Port80_id","s")%></div>
				<div class="formfontdesc" id="FTP_desc"><li><%tcWebApi_get("String_Entry","FC_FTPPrompt_id","s")%></div>
	</ul>
</div>
<div class="formfontdesc" style="margin-top:-10px;">
<a id="faq" href="https://www.asus.com/support/FAQ/114093" target="_blank" style="font-family:Lucida Console;text-decoration:underline;"><%tcWebApi_get("String_Entry","menu5_3_4","s")%> FAQ</a>
</div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
<thead>
<tr>
<td colspan="4"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
<th><%tcWebApi_get("String_Entry","IPC_VServerEnable_in","s")%><input type="hidden" name="vts_num_x_0" value="" readonly="1" /></th>
<td>
<input type="radio" value="1" name="vts_enable_x" class="content_input_fd" onclick="return change_common_radio(this, 'IPConnection', 'vts_enable_x', '1')" <% if tcWebApi_get("VirServer_PVC0_Entry0","VirServer_Enable","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" value="0" name="vts_enable_x" class="content_input_fd" onclick="return change_common_radio(this, 'IPConnection', 'vts_enable_x', '0')" <% if tcWebApi_get("VirServer_PVC0_Entry0","VirServer_Enable","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","IPC_VSList_groupitemdesc","s")%></th>
<td id="vts_rulelist">
<select name="KnownApps" id="KnownApps" class="input_option" onchange="change_wizard(this, 'KnownApps');">
</select>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","IPC_VSList_gameitemdesc","s")%></th>
<td id="VSGameList">
<select name="KnownGames" id="KnownGames" class="input_option" onchange="change_wizard(this, 'KnownGames');">
</select>
</td>
</tr>
<tr id="ori_ftpport">
<th><%tcWebApi_get("String_Entry","IPC_VSList_ftpport","s")%></th>
<td>
<input type="text" maxlength="5" name="vts_ftpport" class="input_6_table" value='<%tcWebApi_get("VirServer_PVC0_Entry0","vts_ftpport","s")%>'>
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
<thead>
<tr>
<td colspan="7"><%tcWebApi_get("String_Entry","IPC_VSList_title","s")%> (<%tcWebApi_get("String_Entry","List_limit","s")%> 32)</td>
</tr>
</thead>
<tr>
<th><%tcWebApi_get("String_Entry","PPPC_x_ServiceName_in","s")%></th>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(18,7);"><%tcWebApi_get("String_Entry","FC_LanWanSrcIP_in","s")%></a></th>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,24);"><%tcWebApi_get("String_Entry","FC_LanWanSrcPort_in","s")%></a></th>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,25);"><%tcWebApi_get("String_Entry","IPC_VServerIP_in","s")%></a></th>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,26);"><%tcWebApi_get("String_Entry","IPC_VServerLPort_in","s")%></a></th>
<th><%tcWebApi_get("String_Entry","IPC_VServerProto_in","s")%></th>
<th><%tcWebApi_get("String_Entry","list_add_delete","s")%></th>
</tr>
<tr>
<td width="20%">
<input type="text" maxlength="30" class="input_15_table" name="vts_desc_x_0" onkeypress="return validator.isString(this, event)"/>
</td>
<td width="17%">
<input type="text" maxlength="15" class="input_15_table" name="vts_srcipaddr_x_0"  onKeyPress="return validator.isIPRange(this, event)"/>
</td>
<td width="14%">
<input type="text" maxlength="30" class="input_12_table" name="vts_port_x_0" onkeypress="return validator.isPortRange(this, event)"/>
</td>
<td width="22%">
<input type="text" maxlength="15" class="input_15_table" name="vts_ipaddr_x_0" align="left" onkeypress="return validator.isIPAddr(this, event)" style="float:left;"/ autocomplete="off" onblur="if(!over_var){hideClients_Block();}" onClick="hideClients_Block();">
<img id="pull_arrow" height="14px;" src="/images/arrow-down.gif" align="right" onclick="pullLANIPList(this);" title="<% tcWebApi_Get("String_Entry", "select_IP", "s") %>" onmouseover="over_var=1;" onmouseout="over_var=0;">
<div id="ClientList_Block" class="ClientList_Block"></div>
</td>
<td width="9%">
<input type="text" maxlength="5" class="input_6_table" name="vts_lport_x_0" onKeyPress="return validator.isNumber(this,event);"/>
</td>
<td width="10%">
<select name="vts_proto_x_0" class="input_option">
<option value="TCP">TCP</option>
<option value="UDP">UDP</option>
<option value="BOTH">BOTH</option>
</select>
</td>
<td width="8%">
<input type="button" class="add_btn" onClick="addRow_Group(32);" name="vts_rulelist_tableAddRow" value="">
</td>
</tr>
</table>
<div id="vts_rulelist_Block"></div>
<div class="apply_gen">
<input name="button" type="button" class="button_gen" onclick="applyRule();" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</form>
<script>
</script>
</body>

<!--Advanced_VirtualServer_Content.asp-->
</html>

