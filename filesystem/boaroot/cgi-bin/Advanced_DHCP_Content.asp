<%
If Request_Form("lanFlag") <> "" Then
	tcWebApi_set("WebCurSet_Entry","lan_id","lan_VC")
	TCWebApi_set("Lan_Dhcp","type","dhcpTypeRadio")

  If Request_Form("dhcpFlag") ="0" Then
		tcWebApi_CommitWithoutSave("Lan")
  End If

  If Request_Form("dhcpTypeRadio")="0" Then
    TCWebApi_unset("Dproxy_Entry")
    If Request_Form("dhcpFlag") ="0" Then
      tcWebApi_CommitWithoutSave("Dhcpd_Common")
      tcWebApi_CommitWithoutSave("Dproxy_Entry")
    End If
  End If

  If Request_Form("dhcpTypeRadio")="1" Then
    'TCWebApi_unset("Dhcpd_Common")
    TCWebApi_set("Dhcpd_Common","start","StartIp")
    TCWebApi_set("Dhcpd_Common","end","EndIp")
    TCWebApi_set("Dhcpd_Common","lease","dhcp_LeaseTime")
    TCWebApi_set("Dhcpd_Common","router","dhcp_gateway_x")
    TCWebApi_set("Dhcpd_Common","wins_IP","winsIP")
    TCWebApi_set("Dhcpd_Common", "elan.1","DHCPPhyPortEth0")

    tcWebApi_set("Dproxy_Entry","Active","DNSproxy")
    TCWebApi_set("Dproxy_Entry","type","dnsTypeRadio")
    TCWebApi_set("Dproxy_Entry","Primary_DNS","PrimaryDns")
    TCWebApi_set("Dproxy_Entry","Secondary_DNS","SecondDns")

    If Request_form("delnum") <> "" Then
      tcWebApi_set("WebCurSet_Entry","dhcpd_id","delnum")
      tcWebApi_unset("Dhcpd_Entry")
    elseif Request_Form("addFlag")="1" Then
      if Request_form("emptyEntry") <> "" then
	      tcWebApi_set("WebCurSet_Entry","dhcpd_id","emptyEntry")
	      tcWebApi_set("Dhcpd_Entry","IP","IpAddr")
	      tcWebApi_set("Dhcpd_Entry","MAC","MACAddr")
      end if
    end if

    If Request_Form("dhcpFlag") ="0" Then
      tcWebApi_CommitWithoutSave("Dhcpd_Common")
      tcWebApi_CommitWithoutSave("Dproxy_Entry")
    End If
  End If

  update_variables()
  tcWebApi_Save()
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_DHCP_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_2_2","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type='text/javascript' src="/js/ip_new.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<style>
#ClientList_Block_PC{
border:1px outset #999;
background-color:#576D73;
position:absolute;
*margin-top:27px;
margin-left:10px;
*margin-left:-263px;
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
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';

var dhcp_enable = '1';
var pool_start = '192.168.1.2';
var pool_end = '192.168.1.254';
var pool_subnet = pool_start.split(".")[0]+"."+pool_start.split(".")[1]+"."+pool_start.split(".")[2]+".";
var pool_start_end = parseInt(pool_start.split(".")[3]);
var pool_end_end = parseInt(pool_end.split(".")[3]);
var static_enable = '0';

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
	showLANIPList();	
	setTimeout("update_FAQ();", 300);
}
function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){
		//id="faq" href="https://www.asus.com/US/support/FAQ/1036677"
		faqURL("faq", "https://www.asus.com", "/support/FAQ/", "1036677");
	}
}

function doValidateRange(startIP,endIP) {

	var dhcp_start_num = inet_network(startIP);
	var dhcp_end_num = inet_network(endIP);
	
	if(dhcp_start_num > dhcp_end_num){
			var tmp = startIP;
			document.uiViewLanForm.StartIp.value = endIP;
			document.uiViewLanForm.EndIp.value = tmp;
	}

    return true;
}

function StringCheck(val)
{
    re = /^[^\s]+$/;
    if( re.test(val) )
        return true;
    else
        return false;
}

function doValidateServer() {
    var Element;
    var ElementValue1,ElementValue2;

    Element = document.uiViewLanForm.dhcp_LeaseTime;

	//validate range
	if(!validate_range(Element, 120, 604800)) {
		return true;
	}

    Element = document.uiViewLanForm.StartIp;
    ElementValue1 = Element.value;
    if(inValidIPAddr(ElementValue1)) return true;
    Element = document.uiViewLanForm.EndIp;
    ElementValue2 = Element.value;
    if(inValidIPAddr(ElementValue2)) return true;
    if(doValidateRange(ElementValue1,ElementValue2)!=true) return true;
	return false;
}

function poolcheck(st,pool,value,Mvalue){
	if( (pool > 253) || (st+pool) > ((Mvalue & st) + value - 1) )
	{
		//mtk04880: for resolving bug 12324 : showing alert twice
	  	//alert("DHCP IP Pool Range exceed limit!!");
		return false;
	}else
	{
		return true;
	}
}

function doPoolRange()
{
	var sIP = document.uiViewLanForm.StartIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var eIP = document.uiViewLanForm.EndIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits = sIP[0].split(".");
  var digits4 = eIP[0].split(".");

	var mask = document.uiViewLanForm.uiViewNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");
	var lanip = document.uiViewLanForm.uiViewIPAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits3 = lanip[0].split(".");

  var stIP = parseInt(digits[3]);//stIP last ipaddr Byte
  var edIP = parseInt(digits4[3]);//edIP last ipaddr Byte

	var Mask = parseInt(digits2[3]);

  if(document.uiViewLanForm.uiViewIPAddr.value == document.uiViewLanForm.StartIp.value)
	{
		alert("DHCP Start IP and Router Local IP are not allowed to be the same");
		return false;
	}
  if(document.uiViewLanForm.uiViewIPAddr.value == document.uiViewLanForm.EndIp.value)
	{
		alert("DHCP End IP and Router Local IP are not allowed to be the same");
		return false;
	}

	for(i=0;i<4;i++)
	{
	  if(((digits2[i] & digits3[i]) != (digits2[i] & digits[i]))
	     ||((digits2[i] & digits3[i]) != (digits2[i] & digits4[i])))
		{
			alert("DHCP pool and Router Local IP are not in the same subnet");
			return false;
		}
	}

	return true;
}

function doCheckmacAddr(){
	var macstr = document.uiViewLanForm.MACAddr.value;
var maclen = macstr.length;
	var tmp = macstr.toUpperCase();
	document.uiViewLanForm.MACAddr.value = tmp;
if(maclen != 0){
		var findpos = macstr.search("^([0-9A-Fa-f]{2})(:[0-9A-Fa-f]{2}){5}$");
		if( findpos != 0 )
		{
			alert("Invalid MAC address:" + macstr);
		}
		return findpos;
	}
	return 0;
}

function doStaticTableRange()
{
	var sIP = document.uiViewLanForm.StartIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var eIP = document.uiViewLanForm.EndIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits = sIP[0].split(".");
  var digits4 = eIP[0].split(".");

	var mask = document.uiViewLanForm.uiViewNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");

	var staticIP = document.uiViewLanForm.IpAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var staticdigits = staticIP[0].split(".");

  var stIP = parseInt(digits[3]);//stIP last ipaddr Byte
  var edIP = parseInt(digits4[3]);//edIP last ipaddr Byte

	var staticIP = parseInt(staticdigits[3]);
	var isIpValid = 1;
	var total = parseInt(document.uiViewLanForm.staticNum.value);
	var leaseTotal = parseInt(document.uiViewLanForm.LeaseNum.value);

	for(i=0;i<4;i++)
	{
	  if((digits2[i] & digits[i]) != (digits2[i] & staticdigits[i])){
			isIpValid = 0;
			break;
		}
	}

	if(isIpValid == 0){
		alert("DHCP Start IP and DHCP Static IP are not in the same subnet");
		return false;
	}

   	/* remove this restriction 2017.05
	if( staticIP <  stIP || staticIP > edIP){
		alert("DHCP Static IP is out of DHCP pool range!");
		return false;
	}*/

	var table=document.getElementById("static_list");
	for(j=0; j<total; j++)
	{
		var str1 = table.rows[j].cells[0].innerHTML.replace(" ","");
		var str2 = table.rows[j].cells[1].innerHTML.replace(" ","");

		if(str1 == document.uiViewLanForm.MACAddr.value)
		{
			alert("DHCP Static MAC address already exist in the list!");
			return false;
		}
		if( str2 == document.uiViewLanForm.IpAddr.value)
		{
			alert("DHCP Static IP already exist in the list!");
			return false;
		}
	}

	return true;
}

function doAdminUiMgmtIpValidate()
{
	var value;
	if(document.uiViewLanForm.dhcpTypeRadio[0].checked)
	{
		value = document.uiViewLanForm.StartIp.value;
		if(inValidIPAddr(value))
		return false;

		if(!doPoolRange())
		return false;

		if(doValidateServer())
		return false;

		if(StringCheck(document.uiViewLanForm.IpAddr.value) || StringCheck(document.uiViewLanForm.MACAddr.value))
		{
			if(document.uiViewLanForm.emptyEntry.value == "N/A")
			{
				alert("DHCP reservation table is full!" );
				return false;
			}
			value = document.uiViewLanForm.IpAddr.value;
			if(inValidIPAddr(value))
				return false;

			if(doCheckmacAddr())
				return false;

			if(!doStaticTableRange())
				return false;

			document.uiViewLanForm.addFlag.value = "1";
		}
		if(document.uiViewLanForm.staticNum.value != "0"){
			if(document.uiViewLanForm.tmpStartIp.value != document.uiViewLanForm.StartIp.value)
			{
				if(confirm("Change Start IP may lead to reservation item be deleted!\nContinue?") == false)
						return false;
			}			
		}
	}
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_DHCP_Content.asp";
}

function applyRule(){
	var form = document.uiViewLanForm;

	if(doAdminUiMgmtIpValidate()==false)
		return;

	if (form.PrimaryDns.value == "" && form.SecondDns.value == "")
		form.dnsTypeRadio.value = "0";
	else
	{
		form.dnsTypeRadio.value = "1";
		if(form.PrimaryDns.value != "") {
  			if(inValidIPAddr(form.PrimaryDns.value))
		      		return false;
		}
		if(form.SecondDns.value != "") {
  			if(inValidIPAddr(form.SecondDns.value))
		      		return false;
		}
	}

	if (form.winsIP.value != "")
	{
	  	if(inValidIPAddr(form.winsIP.value))
      			return false;
	}

	document.uiViewLanForm.dhcpFlag.value = 0;
	document.uiViewLanForm.action_script.value = "restart_boa";
	document.uiViewLanForm.action_mode.value = "apply";
	originData.onlinelist_cache = "";
	showLoading(42);	//Extend from 17 to 42 to restore_webtype
	setTimeout("redirect();", 42000);
	if(navigator.appName.indexOf("Microsoft") >= 0){ 		// Jieming added at 2013/05/21, to avoid browser freeze when submitting form on IE
		stopFlag = 1;
	}
	document.uiViewLanForm.submit();

}

function doAdd()
{
	if(document.uiViewLanForm.IpAddr.value == "" || document.uiViewLanForm.MACAddr.value == "")
		return;

	applyRule();
}

function doDelete(i)
{
	document.uiViewLanForm.delnum.value=i;
	document.uiViewLanForm.MACAddr.value = "";
	document.uiViewLanForm.IpAddr.value = "";
	applyRule();
}

function validate_dhcp_range(ip_obj){
	var ip_num = inet_network(ip_obj.value);
	var subnet_head, subnet_end;
	if(ip_num <= 0){
		alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
	ip_obj.value = "";
	ip_obj.focus();
	ip_obj.select();
	return 0;
	}
	subnet_head = getSubnet(document.form.lan_ipaddr.value, document.form.lan_netmask.value, "head");
	subnet_end = getSubnet(document.form.lan_ipaddr.value, document.form.lan_netmask.value, "end");
	if(ip_num <= subnet_head || ip_num >= subnet_end){
		alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
	ip_obj.value = "";
	ip_obj.focus();
	ip_obj.select();
	return 0;
	}
	return 1;
}

function get_default_pool(ip, netmask){
	z=0;
	tmp_ip=0;
	for(i=0;i<document.form.lan_ipaddr.value.length;i++){
	if (document.form.lan_ipaddr.value.charAt(i) == '.') z++;
	if (z==3){ tmp_ip=i+1; break;}
	}
	post_lan_ipaddr = document.form.lan_ipaddr.value.substr(tmp_ip,3);
	c=0;
	tmp_nm=0;
	for(i=0;i<document.form.lan_netmask.value.length;i++){
	if (document.form.lan_netmask.value.charAt(i) == '.') c++;
	if (c==3){ tmp_nm=i+1; break;}
	}
	var post_lan_netmask = document.form.lan_netmask.value.substr(tmp_nm,3);
	var nm = new Array("0", "128", "192", "224", "240", "248", "252");
	for(i=0;i<nm.length;i++){
	if(post_lan_netmask==nm[i]){
	gap=256-Number(nm[i]);
	subnet_set = 256/gap;
	for(j=1;j<=subnet_set;j++){
	if(post_lan_ipaddr < j*gap){
	pool_start=(j-1)*gap+1;
	pool_end=j*gap-2;
	break;
	}
	}
	var default_pool_start = subnetPostfix(document.form.dhcp_start.value, pool_start, 3);
	var default_pool_end = subnetPostfix(document.form.dhcp_end.value, pool_end, 3);
	var default_pool = new Array(default_pool_start, default_pool_end);
	return default_pool;
	break;
	}
	}
}

function showLANIPList(){

	if(clientList.length == 0){
		setTimeout(function() {
			genClientList();
			showLANIPList();
		}, 500);
		$("ClientList_Block_PC").innerHTML = "";
		
	}
	else{
		var htmlCode = "";		
		for(var i=0; i<clientList.length;i++){
			var clientObj = clientList[clientList[i]];

			if(clientObj.IP == "offline") clientObj.IP = "";
			if(clientObj.Name.length > 30) clientObj.Name = clientObj.Name.substring(0, 27) + "...";

			htmlCode += '<a><div onmouseover="over_var=1;" onmouseout="over_var=0;" onclick="setClientIP(\'';
			htmlCode += clientObj.MacAddr;
			htmlCode += '\', \'';
			htmlCode += clientObj.IP;
			htmlCode += '\');"><strong>';
			htmlCode += clientObj.MacAddr;
			htmlCode += '</strong> ( ';
			htmlCode += clientObj.Name;
			htmlCode += ' )</div></a><!--[if lte IE 6.5]><iframe class="hackiframe2"></iframe><![endif]-->';	
		}

		$("ClientList_Block_PC").innerHTML = htmlCode;
	}
}

function setClientIP(macaddr, ipaddr){
	document.uiViewLanForm.MACAddr.value = macaddr;
	document.uiViewLanForm.IpAddr.value = ipaddr;
	hideClients_Block();
	over_var = 0;
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
		document.uiViewLanForm.MACAddr.focus();
		isMenuopen = 1;
	}
	else
		hideClients_Block();
}

function check_macaddr(obj,flag){ //control hint of input mac address
if (flag){
$("check_mac") ? $("check_mac").style.display="none" : true;
}
else{
var childsel=document.createElement("div");
childsel.setAttribute("id","check_mac");
childsel.style.color="#FFCC00";
obj.parentNode.appendChild(childsel);
$("check_mac").innerHTML="The format for the MAC address is six groups of two hexadecimal digits, separated by colons (:), in transmission order (e.g. 12:34:56:aa:bc:ef)";
}
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value=""></form>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_DHCP_Content.asp" name="uiViewLanForm" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<INPUT TYPE="HIDDEN" NAME="lan_VC" value="0">
<INPUT type="HIDDEN" name="staticNum" value="<%TcWebApi_get("Dhcpd","Static_Num","s")%>">
<INPUT type="HIDDEN" name="LeaseNum" value="<%TcWebApi_get("DhcpLease","LeaseNum","s")%>">
<INPUT type="HIDDEN" name="emptyEntry" value="<%TcWebApi_get("Dhcpd","Empty_Entry","s")%>">
<INPUT TYPE="HIDDEN" NAME="addFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="delnum">
<INPUT TYPE="HIDDEN" NAME="tmpStartIp" value="<%tcWebApi_get("Dhcpd_Common","start","s")%>">
<INPUT type="HIDDEN" NAME="tmpEndIp" value="<%tcWebApi_get("Dhcpd_Common","end","s")%>">
<INPUT type="hidden" name="userMode" value="<%if tcwebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then asp_write("0") else asp_write("1") end if%>">
<INPUT TYPE="HIDDEN" NAME="dhcpFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="lanFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="DNSproxy" VALUE="Yes">
<INPUT TYPE="HIDDEN" NAME="dnsTypeRadio" VALUE='0'>
<INPUT TYPE="HIDDEN" NAME="DHCPPhyPortEth0" VALUE="Yes">
<INPUT TYPE="HIDDEN" NAME="uiViewIPAddr" VALUE="<%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%>" disabled>
<INPUT TYPE="HIDDEN" NAME="uiViewNetMask" VALUE="<%If tcWebApi_get("Lan_Entry","netmask","h") <> "" then tcWebApi_get("Lan_Entry","netmask","s") end if%>" disabled>
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
<table width="760" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_2","s")%> - <%tcWebApi_get("String_Entry","menu5_2_2","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","LHC_DHCPServerConfigurable_sd","s")%></div>
<div id="router_in_pool" class="formfontdesc" style="color:#FFCC00;display:none;">#LHC_DHCPServerConfigurable_sd2","s")%>  <span id="LANIP"></span></div>
<div id="VPN_conflict" class="formfontdesc" style="color:#FFCC00;display:none;"><span id="VPN_conflict_span"></span></div>
<div class="formfontdesc" style="margin-top:-10px;">
<a id="faq" href="https://www.asus.com/support/FAQ/1036677" target="_blank" style="font-family:Lucida Console;text-decoration:underline;"><% tcWebApi_Get("String_Entry", "LHC_MnlDHCPList_groupitemdesc", "s") %> FAQ</a>
</div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
				<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
				<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,1);"><%tcWebApi_get("String_Entry","LHC_DHCPServerConfigurable_in","s")%></a></th>
<td>
	<INPUT TYPE="RADIO" NAME="dhcpTypeRadio" class="content_input_fd" VALUE="1" <%If tcWebApi_get("Lan_Dhcp","type","h") = "1" then asp_Write("checked") end if%>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%><!-- onClick="doReload()"-->
	<INPUT TYPE="RADIO" NAME="dhcpTypeRadio" class="content_input_fd" VALUE="0" <%If tcWebApi_get("Lan_Dhcp","type","h") = "0" then asp_Write("checked") elseIf tcWebApi_get("Lan_Dhcp","type","h") = "" then asp_Write("checked") end if%>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<!--<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,2);"><% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>'s Domain Name:</a></th>
<td>
<input type="text" maxlength="32" class="input_25_table" name="lan_domain" value="">
</td>
</tr>-->
<tr>
			  <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,3);"><%tcWebApi_get("String_Entry","LHC_MinAddress_in","s")%></a></th>
<td>
	<INPUT TYPE="TEXT" NAME="StartIp" class="input_15_table" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Dhcpd_Common","start","h") <> "" then tcWebApi_get("Dhcpd_Common","start","s") end if%>" onKeyPress="return validator.isIPAddr(this, event);">
</td>
</tr>
<tr>
            <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,4);"><%tcWebApi_get("String_Entry","LHC_MaxAddress_in","s")%></a></th>
<td>
	<INPUT TYPE="TEXT" NAME="EndIp" class="input_15_table" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Dhcpd_Common","end","h") <> "" then tcWebApi_get("Dhcpd_Common","end","s") end if%>" onKeyPress="return validator.isIPAddr(this, event);">
</td>
</tr>
<tr>
            <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,5);"><%tcWebApi_get("String_Entry","LHC_LeaseTime_in","s")%></a></th>
<td>
	<INPUT TYPE="TEXT" NAME="dhcp_LeaseTime" class="input_15_table" SIZE="6" MAXLENGTH="6" VALUE="<%If tcWebApi_get("Dhcpd_Common","lease","h") <> "" then tcWebApi_get("Dhcpd_Common","lease","s") else asp_Write("0") end if%>"  onKeyPress="return validator.isNumber(this,event)">
	 <%tcWebApi_get("String_Entry","Second","s")%>
</td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,6);"><%tcWebApi_get("String_Entry","IPC_x_ExternalGateway_in","s")%></a></th>
<td>
<input type="text" maxlength="15" class="input_15_table" name="dhcp_gateway_x" value="<%tcWebApi_get("Dhcpd_Common", "router", "s")%>" onKeyPress="return validator.isIPAddr(this,event)" onBlur="valid_IP_form(this,0)">
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:8px">
<thead>
<tr>
				<td colspan="2"><%tcWebApi_get("String_Entry","LHC_x_LDNSServer1_sn","s")%></td>
</tr>
</thead>
<tr>
				<th width="200"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,7);"><%tcWebApi_get("String_Entry","LHC_x_LDNSServer1_in","s")%></a></th>
<td>
<INPUT TYPE="TEXT" NAME="PrimaryDns" class="input_15_table" SIZE="15" MAXLENGTH="15" VALUE="<% If tcWebApi_get("Dproxy_Entry","type","h") = "1" then tcWebApi_get("Dproxy_Entry","Primary_DNS","s") else asp_Write("") end if %>" onKeyPress="return validator.isIPAddr(this, event);">
</td>
</tr>
<tr>
<th width="200"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,7);"><%tcWebApi_get("String_Entry","LHC_x_LDNSServer2_in","s")%></a></th>
<td>
<INPUT TYPE="TEXT" NAME="SecondDns" class="input_15_table" SIZE="15" MAXLENGTH="15" VALUE="<% If tcWebApi_get("Dproxy_Entry","type","h") = "1" then tcWebApi_get("Dproxy_Entry","Secondary_DNS","s") else asp_Write("") end if %>" onKeyPress="return validator.isIPAddr(this, event);">
</td>
</tr>
<tr>
				<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,8);"><%tcWebApi_get("String_Entry","LHC_x_WINSServer_in","s")%></a></th>
<td>
<INPUT TYPE="TEXT" NAME="winsIP" class="input_15_table" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Dhcpd_Common","wins_IP","h") <> "" then tcWebApi_get("Dhcpd_Common","wins_IP","s") end if%>" onKeyPress="return validator.isIPAddr(this, event);">
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" style="margin-top:8px;" >
<thead>
	<tr>
		<td colspan="3" id="GWStatic"><%tcWebApi_get("String_Entry","LHC_MnlDHCPList_groupitemdesc","s")%>  (<%tcWebApi_get("String_Entry","List_limit","s")%> 32)</td>
	</tr>
</thead>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,10);"><%tcWebApi_get("String_Entry","PPPC_x_MacAddressForISP_in","s")%></a></th>
		<th><%tcWebApi_get("String_Entry","IPC_ExternalIPAddress_in","s")%></th>
		<th><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
	</tr>
<tr>

<td width="40%">
<input type="hidden" name="dhcp_staticnum_x_0" value="" readonly="1" />
<input type="text" class="input_20_table" maxlength="17" name="MACAddr" style="margin-left:-12px;width:255px;" onKeyPress="return validator.isHWAddr(this,event)" onClick="hideClients_Block();">
<img id="pull_arrow" height="14px;" src="/images/arrow-down.gif" style="position:absolute;*margin-left:-3px;*margin-top:1px;" onclick="pullLANIPList(this);" title="<% tcWebApi_Get("String_Entry", "select_MAC", "s") %>" onmouseover="over_var=1;" onmouseout="over_var=0;">
<div id="ClientList_Block_PC" class="ClientList_Block_PC"></div>
</td>
<td width="40%">
<input type="text" class="input_15_table" maxlength="15" name="IpAddr" onkeypress="return validator.isIPAddr(this,event)">
</td>
<td width="20%">
<div>
<input type="button" class="add_btn" onClick="doAdd();" value="">
</div>
</td>
</tr>
</table>
<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="static_list">
	<%if tcwebApi_get("Dhcpd_Entry0","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry0","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry0","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(0);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry1","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry1","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry1","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(1);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry2","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry2","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry2","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(2);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry3","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry3","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry3","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(3);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry4","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry4","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry4","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(4);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry5","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry5","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry5","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(5);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry6","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry6","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry6","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(6);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry7","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry7","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry7","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(7);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry8","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry8","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry8","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(8);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry9","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry9","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry9","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(9);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry10","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry10","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry10","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(10);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry11","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry11","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry11","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(11);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry12","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry12","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry12","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(12);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry13","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry13","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry13","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(13);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry14","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry14","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry14","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(14);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry15","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry15","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry15","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(15);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry16","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry16","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry16","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(16);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry17","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry17","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry17","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(17);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry18","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry18","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry18","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(18);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry19","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry19","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry19","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(19);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry20","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry20","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry20","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(20);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry21","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry21","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry21","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(21);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry22","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry22","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry22","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(22);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry23","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry23","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry23","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(23);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry24","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry24","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry24","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(24);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry25","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry25","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry25","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(25);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry26","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry26","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry26","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(26);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry27","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry27","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry27","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(27);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry28","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry28","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry28","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(28);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry29","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry29","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry29","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(29);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry30","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry30","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry30","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(30);" value=""/></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry31","IP","h")<> "" then%>
	<tr>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry31","MAC","s") %></td>
	<td width="40%"><% tcwebApi_staticGet("Dhcpd_Entry31","IP","s") %></td>
	<td width="20%"><input class="remove_btn" onclick="doDelete(31);" value=""/></td>
	</tr>
	<%end if%>
	</table>
<div class="apply_gen">
<input type="button" name="button" class="button_gen" onclick="applyRule();" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_DHCP_Content.asp-->
</html>

