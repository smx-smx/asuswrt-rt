<html>
<head>
<title>serv list</title>
<meta http-equiv="Content-Type" content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" type="text/JavaScript">
var str_serv = [
				["yes", "<%tcWebApi_get("String_Entry", "WANDefaultRoute0Text", "s")%>"],
				["no", "<%tcWebApi_get("String_Entry", "WANDefaultRoute1Text", "s")%>"],
				["up", "<%tcWebApi_get("String_Entry", "ServListCgiUpText", "s")%>"],
				["Dynamic", "<%tcWebApi_get("String_Entry", "ServListCgiISP0Text", "s")%>"],
				["Static", "<%tcWebApi_get("String_Entry", "ServListCgiISP1Text", "s")%>"],
				["PPPoE", "<%tcWebApi_get("String_Entry", "ServListCgiISP2Text", "s")%>"],
				["Bridge", "<%tcWebApi_get("String_Entry", "ServListCgiISP3Text", "s")%>"]
			];
var strobj = {};
for (var i=0; i<str_serv.length; i++)
	strobj[str_serv[i][0]] = str_serv[i][1];

var pvc8_serv_info = [
				["<%tcWebApi_get("WanExt_PVC8e0", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e0", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e0", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e0", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e0", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e0", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC8e1", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e1", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e1", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e1", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e1", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e1", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC8e2", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e2", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e2", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e2", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e2", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e2", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC8e3", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e3", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e3", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e3", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e3", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e3", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC8e4", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e4", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e4", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e4", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e4", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e4", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC8e5", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e5", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e5", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e5", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e5", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e5", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC8e6", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e6", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e6", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e6", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e6", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e6", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC8e7", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC8e7", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC8e7", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC8e7", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e7", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC8e7", "IPVERSION", "s")%>"]
				];

var pvc9_serv_info = [				
				["<%tcWebApi_get("WanExt_PVC9e0", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e0", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e0", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e0", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e0", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e0", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC9e1", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e1", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e1", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e1", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e1", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e1", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC9e2", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e2", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e2", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e2", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e2", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e2", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC9e3", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e3", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e3", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e3", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e3", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e3", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC9e4", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e4", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e4", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e4", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e4", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e4", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC9e5", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e5", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e5", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e5", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e5", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e5", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC9e6", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e6", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e6", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e6", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e6", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e6", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC9e7", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC9e7", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC9e7", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC9e7", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e7", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC9e7", "IPVERSION", "s")%>"]
			];

var pvc10_serv_info = [				
				["<%tcWebApi_get("WanExt_PVC10e0", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e0", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e0", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e0", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e0", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e0", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC10e1", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e1", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e1", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e1", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e1", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e1", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC10e2", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e2", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e2", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e2", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e2", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e2", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC10e3", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e3", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e3", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e3", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e3", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e3", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC10e4", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e4", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e4", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e4", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e4", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e4", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC10e5", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e5", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e5", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e5", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e5", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e5", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC10e6", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e6", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e6", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e6", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e6", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e6", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("WanExt_PVC10e7", "Active", "s")%>", "<%tcWebApi_get("WanExt_PVC10e7", "IPADDR", "s")%>", "<%tcWebApi_get("WanExt_PVC10e7", "IPADDR6", "s")%>", "<%tcWebApi_get("WanExt_PVC10e7", "ISP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e7", "PPPGETIP", "s")%>", "<%tcWebApi_get("WanExt_PVC10e7", "IPVERSION", "s")%>"]
			];

function stWanservlist(active,  ipaddr4, ipaddr6, isp, getip, ipversion)
{
	var line = "<%tcWebApi_get("Info_Adsl", "lineState", "s")%>";
	if (("up" == line) && ("Yes" == active))
		this.linestate = strobj["up"];
	else
		this.linestate = "N/A";
	if ("Yes" == active) 
		this.active = strobj["yes"];
	else if("No" == active) 
		this.active = strobj["no"];
//ip address
	if ("0" == isp)
	{
		this.isp = strobj["Dynamic"];
		this.ipaddr = strobj["Dynamic"];
	}
	else if ("1" == isp)
	{
		this.isp = strobj["Static"];
		if ("IPv6" == ipversion)
			this.ipaddr = strobj["Dynamic"];
		else if ("IPv4/IPv6" == ipversion)
		{
			this.ipaddr = ipaddr4 + " / " + ipaddr6;
		}
		else
			this.ipaddr = ipaddr4;
	}
	else if ("2" == isp)
	{
		this.isp = strobj["PPPoE"];
		if ("Dynamic" == getip)
			this.ipaddr = strobj["Dynamic"];
		else if("Static" == getip)
		{
			if ("IPv6" == ipversion)
				this.ipaddr = strobj["Dynamic"];
			else if ("IPv4/IPv6" == ipversion)
			{
				this.ipaddr = ipaddr4 + " / " + ipaddr6;
			}
			else
				this.ipaddr = ipaddr4;
		}
		else
			this.ipaddr = "N/A";
	}
	else if ("3" == isp)
	{
		this.isp = strobj["Bridge"];
		this.ipaddr = "N/A";
	}
	else {
		this.isp = "N/A";
		this.ipaddr = "N/A";	
	}	
}
var ary_wan_pvc8 = new Array(6);
var ary_wan_pvc9 = new Array(6);
var ary_wan_pvc10 = new Array(6);
for (var i=0; i<pvc8_serv_info.length; i++)
{
	ary_wan_pvc8[i] = new stWanservlist(pvc8_serv_info[i][0], pvc8_serv_info[i][1], pvc8_serv_info[i][2], pvc8_serv_info[i][3], pvc8_serv_info[i][4], pvc8_serv_info[i][5]);
	ary_wan_pvc9[i] = new stWanservlist(pvc9_serv_info[i][0], pvc9_serv_info[i][1], pvc9_serv_info[i][2], pvc9_serv_info[i][3], pvc9_serv_info[i][4], pvc9_serv_info[i][5]);
	ary_wan_pvc10[i] = new stWanservlist(pvc10_serv_info[i][0], pvc10_serv_info[i][1], pvc10_serv_info[i][2], pvc10_serv_info[i][3], pvc10_serv_info[i][4], pvc10_serv_info[i][5]);
}
function writeInfototable()
{
	var strtemp = "";
	var vc = "<%tcWebApi_get("WebCurSet_Entry", "wan_pvc", "s")%>";

	if (vc == "8"){
		for (var i=0; i<ary_wan_pvc8.length; i++) {
			strtemp += '<tr><td align="center" class="tabdata">' + i + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc8[i].active + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc8[i].isp + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc8[i].ipaddr + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc8[i].linestate + '</td></tr>\n';
		}
	} else if (vc == "9"){
		for (var i=0; i<ary_wan_pvc9.length; i++) {
			strtemp += '<tr><td align="center" class="tabdata">' + i + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc9[i].active + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc9[i].isp + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc9[i].ipaddr + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc9[i].linestate + '</td></tr>\n';
		}
	} else if (vc == "10"){
		for (var i=0; i<ary_wan_pvc10.length; i++) {
			strtemp += '<tr><td align="center" class="tabdata">' + i + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc10[i].active + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc10[i].isp + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc10[i].ipaddr + '</td>\n';
			strtemp += '<td align="center" class="tabdata">' + ary_wan_pvc10[i].linestate + '</td></tr>\n';
		}
	}
	document.write(strtemp);
}
</script>
</head>
<body topmargin="10" leftmargin="0">
<form id="pvclist_form" method="post">
<table width="590" border="0" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td width="590" height="5" valign="baseline" class="orange"></td>
	</tr>
</table>
<table border="0" width="590" align="center" cellspacing="0" cellpadding="0">
	<tr>
		<td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
		<td width="500" align="right" valign="bottom" bgcolor="#FFFFFF" class="model"><%tcWebApi_get("String_Entry", "ServListCgiADSL", "s")%></td>
	</tr>
</table>
<table width="590" height="2" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
	<tr>
		<td width="590" valign="baseline" class="orange"></td>
	</tr>
</table>
<table width="590" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
	<tr>
		<td width="590" height="35"  bgcolor="#FFFFFF" class="headline"><%tcWebApi_get("String_Entry", "ServListCgiSis", "s")%></td>
	</tr>
	<tr><td>
</table>
<table width="590" border="1" align="center" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
	<tr height="30">
		<% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "8" then%>
		<td width="55"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ServListCgiBarrier0", "s")%></strong></div></td>
		<%elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "9" then%>
		<td width="55"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ServListCgiBarrier1", "s")%></strong></div></td>
		<%elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "10" then%>
		<td width="55"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ServListCgiWan0", "s")%></strong></div></td>
		<%end if%>
		<td width="70"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ServListCgiActive", "s")%></strong></td>
		<td width="50"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ServListCgiISP", "s")%></strong></td>
		<td width="120"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ServListCgiIPAddr", "s")%></strong></td>
		<td width="70"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ServListCgiStatus", "s")%></strong></td>
	</tr>
	<script language="JavaScript" type="text/JavaScript">
			writeInfototable();
	</script>
</table>
<table width="590" height="15" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
	<tr>
		<td width="590" height="10" valign="baseline" bgcolor="#FFFFFF"></td>
	</tr>
	<tr>
		<td width="590" valign="baseline" class="orange"></td>
	</tr>
</table>
</form>
</body>
</html>
