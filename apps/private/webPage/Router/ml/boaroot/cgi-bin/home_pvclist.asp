<html>
<head>
<title>pvc list</title>
<meta http-equiv="Content-Type" content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" type="text/JavaScript">
var str_pvc = [
				["yes", "<%tcWebApi_get("String_Entry", "WANDefaultRoute0Text", "s")%>"],
				["no", "<%tcWebApi_get("String_Entry", "WANDefaultRoute1Text", "s")%>"],
				["llc", "<%tcWebApi_get("String_Entry", "PVCListCgiLLCText", "s")%>"],
				["vc_mux", "<%tcWebApi_get("String_Entry", "PVCListCgiMuxText", "s")%>"],
				["up", "<%tcWebApi_get("String_Entry", "PVCListCgiUpText", "s")%>"],
				["Bridged_IP", "<%tcWebApi_get("String_Entry", "PVCListCgiEncap0Text", "s")%>"],
				["Routed_IP", "<%tcWebApi_get("String_Entry", "PVCListCgiEncap1Text", "s")%>"],
				["PPPoE", "<%tcWebApi_get("String_Entry", "PVCListCgiEncap2Text", "s")%>"],
				["PPPoA", "<%tcWebApi_get("String_Entry", "PVCListCgiEncap3Text", "s")%>"],
				["Bridged_Only", "<%tcWebApi_get("String_Entry", "PVCListCgiEncap4Text", "s")%>"],
				["Dynamic", "<%tcWebApi_get("String_Entry", "WANGetIPAddress2Text", "s")%>"]
			];
var strobj = {};
for (var i=0; i<str_pvc.length; i++)
	strobj[str_pvc[i][0]] = str_pvc[i][1];
var pvc_info = [
				["<%tcWebApi_get("Wan_PVC0", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC0", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC0", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC0", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC0", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC0", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC0", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC0", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC0", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("Wan_PVC1", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC1", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC1", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC1", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC1", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC1", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC1", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC1", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC1", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("Wan_PVC2", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC2", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC2", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC2", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC2", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC2", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC2", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC2", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC2", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("Wan_PVC3", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC3", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC3", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC3", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC3", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC3", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC3", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC3", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC3", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("Wan_PVC4", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC4", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC4", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC4", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC4", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC4", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC4", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC4", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC4", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("Wan_PVC5", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC5", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC5", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC5", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC5", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC5", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC5", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC5", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC5", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("Wan_PVC6", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC6", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC6", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC6", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC6", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC6", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC6", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC6", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC6", "IPVERSION", "s")%>"],
				["<%tcWebApi_get("Wan_PVC7", "Active", "s")%>", "<%tcWebApi_get("Wan_PVC7", "VPI", "s")%>", "<%tcWebApi_get("Wan_PVC7", "VCI", "s")%>", "<%tcWebApi_get("Wan_PVC7", "ENCAP", "s")%>", "<%tcWebApi_get("Wan_PVC7", "IPADDR", "s")%>", "<%tcWebApi_get("Wan_PVC7", "IPADDR6", "s")%>", "<%tcWebApi_get("Wan_PVC7", "ISP", "s")%>", "<%tcWebApi_get("Wan_PVC7", "PPPGETIP", "s")%>", "<%tcWebApi_get("Wan_PVC7", "IPVERSION", "s")%>"]
			];
function stWanpvclist(active, vpi, vci, encap, ipaddr4, ipaddr6, isp, getip, ipversion)
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
	this.vpi = vpi;
	this.vci = vci;
//mux
	var encaptmp = encap.split(' ');
	var encaptmp1 ="";
	for (var n in encaptmp)
	{
		if ((encaptmp[n] == "LLC") || (encaptmp[n] == "LLC\(IPoA\)") || (encaptmp[n] == "LLC(IPoA)"))
			this.mux = strobj["llc"];
		else if (encaptmp[n] == "VC-Mux")
			this.mux = strobj["vc_mux"];
		else if("N/A" == encaptmp[n])
			this.mux = "N/A";
		else
			encaptmp1 +=  ' ' + encaptmp[n];
	}
//encap
	if (" 1483 Bridged IP" == encaptmp1) this.encap = strobj["Bridged_IP"];
	else if (" 1483 Routed IP" == encaptmp1) this.encap = strobj["Routed_IP"];
	else if (" PPPoE" == encaptmp1) this.encap = strobj["PPPoE"];
	else if (" PPPoA" == encaptmp1) this.encap = strobj["PPPoA"];
	else if (" 1483 Bridged Only" == encaptmp1) this.encap = strobj["Bridged_Only"];
	else this.encap = "N/A";
//ip address
	if ("0" == isp)
	{
		this.ipaddr = strobj["Dynamic"];
	}
	else if ("1" == isp)
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
	else if ("2" == isp)
	{
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
	else
		this.ipaddr = "N/A";	
}
var ary_wan = new Array(8);
for (var i=0; i<pvc_info.length; i++)
{
	ary_wan[i] = new stWanpvclist(pvc_info[i][0], pvc_info[i][1], pvc_info[i][2], pvc_info[i][3], pvc_info[i][4], pvc_info[i][5], pvc_info[i][6], pvc_info[i][7], pvc_info[i][8]);
}
function writeInfototable()
{
	var strtemp = "";
	for (var i=0; i<ary_wan.length; i++) {
		strtemp += '<tr><td align="center" class="tabdata">' + i + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_wan[i].active + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_wan[i].vpi + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_wan[i].vci + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_wan[i].encap + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_wan[i].mux + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_wan[i].ipaddr + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_wan[i].linestate + '</td></tr>\n';
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
		<td width="500" align="right" valign="bottom" bgcolor="#FFFFFF" class="model"><%tcWebApi_get("String_Entry", "PVCListCgiADSL", "s")%></td>
	</tr>
</table>
<table width="590" height="2" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
	<tr>
		<td width="590" valign="baseline" class="orange"></td>
	</tr>
</table>
<table width="590" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
	<tr>
		<td width="590" height="35"  bgcolor="#FFFFFF" class="headline"><%tcWebApi_get("String_Entry", "PVCListCgiSis", "s")%></td>
	</tr>
	<tr><td>
</table>
<table width="590" border="1" align="center" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
	<tr height="30">
		<td width="55"  align="center" class="tabdata"><strong>#</strong></div></td>
		<td width="70"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "PVCListCgiActive", "s")%></strong></td>
		<td width="50"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "PVCListCgiVPI", "s")%></strong></td>
		<td width="50"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "PVCListCgiVCI", "s")%></strong></td>
		<td width="200"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "PVCListCgiENCAP", "s")%></strong></td>
		<td width="120"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "PVCListCgiMux", "s")%></strong></td>
		<td width="120"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "PVCListCgiIPAddr", "s")%></strong></td>
		<td width="70"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "PVCListCgiStatus", "s")%></strong></td>
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
