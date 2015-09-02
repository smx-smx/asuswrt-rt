<%
if Request_form("Stat_Radio") <> "" then
	tcWebApi_set("WebCurSet_Entry","statis_type","Stat_Radio")
end if
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script type='text/javascript' src="/jsl.js" tppabs="http://192.168.1.1/js/jsl.js"></script>
<style type="text/css">
</style>
</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/status_statistics.asp" name="Stat_Form">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
		<td width="150" height="30" class="title-main">
		<font color="#FFFFFF">Traffic Statistics</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"></td><td width="10"></td>
		<td width="440"></td></tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000">Interface</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="0" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "0" then asp_Write("checked") end if%> onClick="document.Stat_Form.submit()">
		<font color="#000000">Ethernet</font>

		<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="1" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "1" then asp_Write("checked") end if%>  onClick="document.Stat_Form.submit()">
		<font color="#000000">xDSL</font>
		<%elseif tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="1" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "1" then asp_Write("checked") end if%>  onClick="document.Stat_Form.submit()">
		<font color="#000000">ADSL</font>
		<%end if%>

		<%If tcWebApi_get("Info_WLan","isExist","h") = "On" Then%>
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="2" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "2" then asp_Write("checked") end if%> onClick="document.Stat_Form.submit()">
		<font color="#000000">WLAN</font>
		<%end If%>


		<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="3" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "3" then asp_Write("checked") end if%>  onClick="document.Stat_Form.submit()">
		<font color="#000000">WAN0</font>
		<%end if%>

		</td></tr>
	<tr>
		<td class="light-orange"></td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr><tr>
		<td class="light-orange"></td><td class="light-orange"></td><td colspan="3">
		<table width="570" border="1" align=center cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
	<tr class="tabdata">
		<td width="180" class="model"><div align=center>
		<font color="#000000">Transmit Statistics</font></div></td>
		<td width="105">&nbsp;</td>
		<td width="180" class="model"><div align=center>
		<font color="#000000">Receive Statistics</font></div></td>
		<td>&nbsp;</td></tr>  

<% If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "1" Then %>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Total PDUs</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","outPkts","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Total PDUs</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","inPkts","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Total Error Counts</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","outDiscards","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Total Error Counts</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","inDiscards","s") %></div></td></tr>
		<%if tcwebApi_get("WebCustom_Entry", "isCZTMSupported", "h")="Yes" Then %>
		<tr class="tabdata">
		<td width="180" class="model"><div align=center>
		<font color="#000000">Downstream</font></div></td>
		<td width="105">&nbsp;</td>
		<td width="180" class="model"><div align=center>
		<font color="#000000">Upstream</font></div></td>
		<td>&nbsp;</td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Downstream Rate (Kbps):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","DataRateDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Upstream Rate (Kbps):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","DataRateUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Downstream SNR Margin (dB):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","SNRMarginDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Upstream SNR Margin (dB):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","SNRMarginUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Downstream Attenuation (dB):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","AttenDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Upstream Attenuation (dB):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","AttenUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Downstream Output Power (dBm):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","PowerDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Upstream Output Power (dBm):</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","PowerUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Downstream CRC Errors:</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","CRCDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Upstream CRC Errors:</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","CRCUp","s") %></div></td></tr>
			<%end if%>
<% Elseif tcWebApi_get("WebCurSet_Entry","statis_type","h") = "2" Then %>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Tx Frames Count</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanTxFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Rx Frames Count</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanRxFrames","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Tx Errors Count</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanTxErrFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Rx Errors Count</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanRxErrFrames","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Tx Drops Count</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanTxDropFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Rx Drops Count</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanRxDropFrames","s") %></div></td></tr>

 <% ElseIf tcWebApi_get("WebCurSet_Entry","statis_type","h") = "3" Then %>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","txFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","rxFrames","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Multicast Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","outMulticastPkts","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Multicast Frame</td><td class='tabdata'><div align='center'><%tcWebApi_get("Info_Wan0","inMulticastPkts","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit total Bytes</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","outOctets","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive total Bytes</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","inOctets","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Collision</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","txCollisionCnt","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive CRC Errors</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","rxCrcErr","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Error Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","outErrors","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Under-size Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Wan0","txUnderRunCnt","s") %></div></td></tr>

<% Else %>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","txFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","rxFrames","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Multicast Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","outMulticastPkts","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Multicast Frame</td><td class='tabdata'><div align='center'><%tcWebApi_get("Info_Ether","inMulticastPkts","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit total Bytes</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","outOctets","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive total Bytes</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","inOctets","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Collision</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","txCollisionCnt","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive CRC Errors</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","rxCrcErr","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;Transmit Error Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","outErrors","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;Receive Under-size Frames</td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","txUnderRunCnt","s") %></div></td></tr>

<% End If %>
		</table></td></tr>
	<tr>
		<td class="light-orange"></td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="SUBMIT" NAME="StatRefresh" VALUE="REFRESH"></td></tr></table>
</form></body></html>
