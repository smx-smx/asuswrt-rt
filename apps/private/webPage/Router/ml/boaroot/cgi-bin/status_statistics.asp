<%
if Request_form("Stat_Radio") <> "" then
	tcWebApi_set("WebCurSet_Entry","statis_type","Stat_Radio")
end if
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
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
		<font color="#FFFFFF"><%tcWebApi_get("String_Entry","StatisticsTrafficText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"></td><td width="10"></td>
		<td width="440"></td></tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticsInterfaceText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="0" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "0" then asp_Write("checked") end if%> onClick="document.Stat_Form.submit()">
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticsEthernetText","s")%></font>
		
		
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="1" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "1" then asp_Write("checked") end if%>  onClick="document.Stat_Form.submit()">
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticsXDSLText","s")%></font>
<%elseif tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="1" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "1" then asp_Write("checked") end if%>  onClick="document.Stat_Form.submit()">
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticsADSLText","s")%></font>
<%end if%>
			
<% If tcWebApi_get("Info_WLan","isExist","h") = "On" Then%>
		<INPUT TYPE="RADIO" NAME="Stat_Radio" VALUE="2" <%If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "2" then asp_Write("checked") end if%> onClick="document.Stat_Form.submit()">
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticsWLANText","s")%></font>
<% End If%>
		</td></tr>
	<tr>
		<td class="light-orange"></td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr><tr>
		<td class="light-orange"></td><td class="light-orange"></td><td colspan="3">
		<table width="570" border="1" align=center cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
	<tr class="tabdata">
		<td width="180" class="model"><div align=center>
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticsTransmitText","s")%></font></div></td>
		<td width="105">&nbsp;</td>
		<td width="180" class="model"><div align=center>
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticsReceiveText","s")%></font></div></td>
		<td>&nbsp;</td></tr>  
<% If tcWebApi_get("WebCurSet_Entry","statis_type","h") = "1" Then %>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsPDUsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","outPkts","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsRePDUsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","inPkts","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsErrCountsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","outDiscards","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsReErrCountsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","inDiscards","s") %></div></td></tr>
 		<%if tcwebApi_get("WebCustom_Entry", "isCZTMSupported", "h")="Yes" Then %>
		<tr class="tabdata">
		<td width="180" class="model"><div align=center>
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticAdslDownstreamText","s")%></font></div></td>
		<td width="105">&nbsp;</td>
		<td width="180" class="model"><div align=center>
		<font color="#000000"><%tcWebApi_get("String_Entry","StatisticAdslUpstreamText","s")%></font></div></td>
		<td>&nbsp;</td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslDownRateText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","DataRateDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslUpRateText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","DataRateUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslDownSNRMText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","SNRMarginDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslUpSNRMText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","SNRMarginUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslDownAttenText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","AttenDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslUpAttenText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","AttenUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslDownPowerText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","PowerDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslUpPowerText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","PowerUp","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslDownCRCText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","CRCDown","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticAdslUpCRCText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Adsl","CRCUp","s") %></div></td></tr>
		<%end if%>
<% Elseif tcWebApi_get("WebCurSet_Entry","statis_type","h") = "2" Then %>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTxPacketsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanTxPackets","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsRxPacketsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanRxPackets","s") %></div></td></tr>
		<tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTxBytesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanTxBytes","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsRxBytesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanRxBytes","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTxErrorsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanTxErrFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsRxErrorsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanRxErrFrames","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTxDropsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanTxDropFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsRxDropsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_WLan","wlanRxDropFrames","s") %></div></td></tr>

<% Else %>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTrFramesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","txFrames","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsReFramesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","rxFrames","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTrMulticastText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","outMulticastPkts","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsReMulticastText","s")%></td><td class='tabdata'><div align='center'><%tcWebApi_get("Info_Ether","inMulticastPkts","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTrtotaltBytesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","outOctets","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsRetotaltBytesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","inOctets","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTrCollisionText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","txCollisionCnt","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsCRCErrorsText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","rxCrcErr","s") %></div></td></tr>
		<tr>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsTrErrorFramesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","outErrors","s") %></div></td>
			<td class='tabdata'>&nbsp;&nbsp;<%tcWebApi_get("String_Entry","StatisticsReUDsizeFramesText","s")%></td><td class='tabdata'><div align='center'><% tcWebApi_get("Info_Ether","txUnderRunCnt","s") %></div></td></tr>

<% End If %>
		</table></td></tr>
	<tr>
		<td class="light-orange"></td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="SUBMIT" NAME="StatRefresh" VALUE="<%tcWebApi_get("String_Entry","ButtonRefreshText","s")%>"></td></tr></table>
</form></body></html>
