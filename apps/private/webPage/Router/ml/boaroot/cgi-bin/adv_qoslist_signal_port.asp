<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" type="text/JavaScript">
var	ary_qosport = [
					["<%tcWebApi_get("QoS_Entry0","eth0","s")%>", "<%tcWebApi_get("QoS_Entry0","ra0","s")%>", "<%tcWebApi_get("QoS_Entry0","ra1","s")%>", "<%tcWebApi_get("QoS_Entry0","ra2","s")%>", "<%tcWebApi_get("QoS_Entry0","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry1","eth0","s")%>", "<%tcWebApi_get("QoS_Entry1","ra0","s")%>", "<%tcWebApi_get("QoS_Entry1","ra1","s")%>", "<%tcWebApi_get("QoS_Entry1","ra2","s")%>", "<%tcWebApi_get("QoS_Entry1","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry2","eth0","s")%>", "<%tcWebApi_get("QoS_Entry2","ra0","s")%>", "<%tcWebApi_get("QoS_Entry2","ra1","s")%>", "<%tcWebApi_get("QoS_Entry2","ra2","s")%>", "<%tcWebApi_get("QoS_Entry2","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry3","eth0","s")%>", "<%tcWebApi_get("QoS_Entry3","ra0","s")%>", "<%tcWebApi_get("QoS_Entry3","ra1","s")%>", "<%tcWebApi_get("QoS_Entry3","ra2","s")%>", "<%tcWebApi_get("QoS_Entry3","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry4","eth0","s")%>", "<%tcWebApi_get("QoS_Entry4","ra0","s")%>", "<%tcWebApi_get("QoS_Entry4","ra1","s")%>", "<%tcWebApi_get("QoS_Entry4","ra2","s")%>", "<%tcWebApi_get("QoS_Entry4","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry5","eth0","s")%>", "<%tcWebApi_get("QoS_Entry5","ra0","s")%>", "<%tcWebApi_get("QoS_Entry5","ra1","s")%>", "<%tcWebApi_get("QoS_Entry5","ra2","s")%>", "<%tcWebApi_get("QoS_Entry5","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry6","eth0","s")%>", "<%tcWebApi_get("QoS_Entry6","ra0","s")%>", "<%tcWebApi_get("QoS_Entry6","ra1","s")%>", "<%tcWebApi_get("QoS_Entry6","ra2","s")%>", "<%tcWebApi_get("QoS_Entry6","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry7","eth0","s")%>", "<%tcWebApi_get("QoS_Entry7","ra0","s")%>", "<%tcWebApi_get("QoS_Entry7","ra1","s")%>", "<%tcWebApi_get("QoS_Entry7","ra2","s")%>", "<%tcWebApi_get("QoS_Entry7","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry8","eth0","s")%>", "<%tcWebApi_get("QoS_Entry8","ra0","s")%>", "<%tcWebApi_get("QoS_Entry8","ra1","s")%>", "<%tcWebApi_get("QoS_Entry8","ra2","s")%>", "<%tcWebApi_get("QoS_Entry8","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry9","eth0","s")%>", "<%tcWebApi_get("QoS_Entry9","ra0","s")%>", "<%tcWebApi_get("QoS_Entry9","ra1","s")%>", "<%tcWebApi_get("QoS_Entry9","ra2","s")%>", "<%tcWebApi_get("QoS_Entry9","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry10","eth0","s")%>", "<%tcWebApi_get("QoS_Entry10","ra0","s")%>", "<%tcWebApi_get("QoS_Entry10","ra1","s")%>", "<%tcWebApi_get("QoS_Entry10","ra2","s")%>", "<%tcWebApi_get("QoS_Entry10","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry11","eth0","s")%>", "<%tcWebApi_get("QoS_Entry11","ra0","s")%>", "<%tcWebApi_get("QoS_Entry11","ra1","s")%>", "<%tcWebApi_get("QoS_Entry11","ra2","s")%>", "<%tcWebApi_get("QoS_Entry11","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry12","eth0","s")%>", "<%tcWebApi_get("QoS_Entry12","ra0","s")%>", "<%tcWebApi_get("QoS_Entry12","ra1","s")%>", "<%tcWebApi_get("QoS_Entry12","ra2","s")%>", "<%tcWebApi_get("QoS_Entry12","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry13","eth0","s")%>", "<%tcWebApi_get("QoS_Entry13","ra0","s")%>", "<%tcWebApi_get("QoS_Entry13","ra1","s")%>", "<%tcWebApi_get("QoS_Entry13","ra2","s")%>", "<%tcWebApi_get("QoS_Entry13","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry14","eth0","s")%>", "<%tcWebApi_get("QoS_Entry14","ra0","s")%>", "<%tcWebApi_get("QoS_Entry14","ra1","s")%>", "<%tcWebApi_get("QoS_Entry14","ra2","s")%>", "<%tcWebApi_get("QoS_Entry14","ra3","s")%>", ""],
					["<%tcWebApi_get("QoS_Entry15","eth0","s")%>", "<%tcWebApi_get("QoS_Entry15","ra0","s")%>", "<%tcWebApi_get("QoS_Entry15","ra1","s")%>", "<%tcWebApi_get("QoS_Entry15","ra2","s")%>", "<%tcWebApi_get("QoS_Entry15","ra3","s")%>", ""]	
];
var	str_portname = ["e0", "ra0", "ra1", "ra2", "ra3"];
function checkandsetphyPort(n)
{
	var strtemp = "";
	for(var i=0; ary_qosport[n][i] != ""; i++)
	{
		if (ary_qosport[n][i] == "Yes")
		{
			strtemp += ",";
			strtemp += str_portname[i];
		}
	}
	if (strtemp == "")
		return "&nbsp;";
	return strtemp.substring(1);
}
var str_langQos = [
				["Yes", "<%tcWebApi_get("String_Entry", "ACLYesText", "s")%>"],
				["No", "<%tcWebApi_get("String_Entry", "ACLNoText", "s")%>"]
			];
function	getStr(key)
{
	for (var i=0; i<str_langQos.length; i++) {
		if (key == str_langQos[i][0])
		{
			return str_langQos[i][1];
		}
	}
	return "";
}
</script>
</head>
<body topmargin="10" leftmargin="0">
<table width="820" align=center cellpadding="0" cellspacing="0">
<tr>
	<td width="820" height="5" valign="baseline" class="orange"></td>
</tr>
</table>
<table width="820" align=center cellpadding="0" cellspacing="0">
<tr>
	<td width="820" height="35" class="headline"><font color="#666666"><%tcWebApi_get("String_Entry","QosCgiSettingsSummary","s")%></font></td>
</tr>
<tr>
	<td>
	<table cellspacing=1 cellpadding=1 border=1 width="820" align=center>
	<tr height="30">
		<td height="35" colspan="9" bgcolor="ffff66" class="tabdata" align=center><strong><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiRules","s")%></font></strong></td>
		<td height="35" colspan="3" bgcolor="ffff99" class="tabdata" align=center><strong><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiActions","s")%></font></strong></td>
		</tr>
		<tr>
			<td class="tabdata" rowspan=2 align=center bgcolor="ffff66">#</td>
			<td class="tabdata" rowspan=2 align=center bgcolor="ffff66"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiActive","s")%></font></td>
			<td class="tabdata" rowspan=2 align=center bgcolor="ffff66"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiPhysicalPorts","s")%></font></td>
			<td class="tabdata" align=center bgcolor="ffff66"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiDestination","s")%></font></td>
			<td class="tabdata" align=center bgcolor="ffff66"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiSource","s")%></font></td>
			<td class="tabdata" rowspan=2 align=center bgcolor="ffff66"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiProtocolID","s")%></font></td>
			<td class="tabdata" rowspan=2 align=center bgcolor="ffff66"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiVLANID","s")%></font></td>
			<td class="tabdata" rowspan=2 align=center bgcolor="ffff66"><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiIPPTOS","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiDSCP","s")%></font></div></td>
			<td class="tabdata" rowspan=2 align=center bgcolor="ffff66"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgi8021p","s")%></font></td>
			<td rowspan=2 align=center class="tabdata" bgcolor="ffff99"><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiIPPTOS","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiDSCP","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiRemarking","s")%></font></div></td>
			<td rowspan=2 class="tabdata" align=center bgcolor="ffff99"><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgi8021p","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiRemarking","s")%></font></div></td>
			<td rowspan=2 class="tabdata" align=center bgcolor="ffff99"><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiQueue","s")%></font></td>			
		</tr>
		<tr>
			<td class="tabdata" align=center bgcolor="ffff66"><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiMAC","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiIPMask","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiPortRange","s")%></font></div></td>
			<td class="tabdata" align=center bgcolor="ffff66"><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiMAC","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiIPMask","s")%></font></div><div><font color="#000000"><%tcWebApi_get("String_Entry","QosCgiPortRange","s")%></font></div></td>
		</tr>
		<%tcWebApi_Get("GUITemp_Entry0","qosTempAttr","h")%>
		<%if tcWebApi_staticGet("GUITemp_Entry0","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">0</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry0","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(0))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry0","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry0","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry0","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry0","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry0","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry0","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry0","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry0","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry0","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry1","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">1</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry1","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(1))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry1","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry1","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry1","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry1","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry1","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry1","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry1","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry1","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry1","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry2","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">2</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry2","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(2))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry2","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry2","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry2","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry2","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry2","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry2","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry2","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry2","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry2","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry3","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">3</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry4","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(3))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry3","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry3","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry3","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry3","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry3","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry3","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry3","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry3","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry3","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry4","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">4</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry5","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(4))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry4","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry4","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry4","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry4","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry4","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry4","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry4","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry4","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry4","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry5","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">5</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry5","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(5))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry5","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry5","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry5","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry5","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry5","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry5","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry5","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry5","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry5","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry6","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">6</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry6","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(6))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry6","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry6","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry6","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry6","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry6","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry6","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry6","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry6","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry6","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry7","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">7</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry7","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(7))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry7","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry7","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry7","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry7","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry7","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry7","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry7","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry7","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry7","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry8","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">8</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry8","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(8))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry8","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry8","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry8","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry8","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry8","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry8","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry8","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry8","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry8","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry9","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">9</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry9","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(9))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry9","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry9","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry9","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry9","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry9","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry9","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry9","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry9","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry9","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry10","Active","h") <> "N/A" then%>
		<tr>
		<td height="24" align="center" class="tabdata" bgcolor="ffff66">10</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry10","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(10))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry10","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry10","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry10","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry10","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry10","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry10","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry10","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry10","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry10","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry11","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">11</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry11","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(11))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry11","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry11","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry11","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry11","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry11","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry11","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry11","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry11","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry11","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry12","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">12</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry12","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(12))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry12","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry12","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry12","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry12","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry12","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry12","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry12","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry12","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry12","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry13","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">13</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry13","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(13))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry13","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry13","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry13","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry13","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry13","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry13","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry13","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry13","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry13","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry14","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">14</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry14","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(14))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry14","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry14","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry14","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry14","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry14","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry14","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry14","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry14","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry14","queue","s")%></td>		
		</tr>
		<%end if%>
		<%if tcWebApi_staticGet("GUITemp_Entry15","Active","h") <> "N/A" then%>
		<tr>
		<td align="center" class="tabdata" bgcolor="ffff66">15</td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(getStr("<%tcWebApi_staticGet("GUITemp_Entry15","Active","s")%>"))</script></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><script>document.writeln(checkandsetphyPort(15))</script> </td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry15","Des","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry15","Src","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry15","proID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry15","vlanID","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry15","dscp","s")%></td>
	 	<td align="center" class="tabdata" bgcolor="ffff66"><%tcWebApi_staticGet("GUITemp_Entry15","dotp","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry15","dscp_remark","s")%></td>	
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry15","dotp_remark","s")%></td>
		<td align="center" class="tabdata" bgcolor="ffff99"><%tcWebApi_staticGet("GUITemp_Entry15","queue","s")%></td>		
		</tr>
		<%end if%>
		</table>
		</td>
		
	</table>
	<table width="820" height="15" align=center class="orange" cellpadding="0" cellspacing="0">
	<tr><td height="10" valign="baseline" class="tabdata"><%tcWebApi_get("String_Entry","QosListNote","s")%></td>
	</tr>
	<tr><td valign="baseline" class="orange"> </td>
	</tr>
	</table>
	</td></tr>
</table>
</body>
</html>
