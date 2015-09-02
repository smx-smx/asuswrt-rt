<%
if Request_Form("reset_flag") = "1" then
	TCWebApi_set("Info_WLan","ReCounterActive","reset_flag")
	TCWebApi_commit("WLan_Entry")
end if
%>

<html><head>
<META HTTP-EQUIV="refresh" CONTENT="3; URL=./wifiqa_apstatistics.asp">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript">

function Doreset() {
	document.ap_statistics.reset_flag.value = "1";
	document.ap_statistics.submit();
}

function initTranslation()
{
}

function PageInit()
{
	//var txbf = "<% getTxBFBuilt(); %>";
	var txbf = "0";
	initTranslation();

	if (txbf != "1") {
		//document.getElementById("div_stats_txbf").style.visibility = "hidden";
		//document.getElementById("div_stats_txbf").style.display = "none";
	}
}

function Doreturn(){
	location.replace("/cgi-bin/home_advancedwireless.asp");
}
</script>
</head><body onLoad="PageInit()">
<form method=post name="ap_statistics" action="/cgi-bin/wifiqa_apstatistics.asp">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
  <tr>
    <td width="150" height="30" class="title-main">
   	<font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessAdvTxStaticText","s")%></font></td>
	<td width="10" class="black">&nbsp;</td>
    <td width="230">&nbsp;</td>
    <td width="10"> </td>
	<td width="360"></td>
  </tr>
  <tr>
	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvTXSucText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><%if tcWebApi_get("Info_WLan","wlanTxFrames","h") <> "N/A" then tcWebApi_get("Info_WLan","wlanTxFrames","s") end if%></td>
  </tr>
  <tr>
  	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvTXRetryText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><%if tcWebApi_get("Info_WLan","wlanTxRetryCount","h") <> "N/A" then tcWebApi_get("Info_WLan","wlanTxRetryCount","s") end if%></td>
  <tr>
  	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvTXFailText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><%if tcWebApi_get("Info_WLan","wlanTxErrFrames","h") <> "N/A" then tcWebApi_get("Info_WLan","wlanTxErrFrames","s") end if%></td>
  <tr>
  	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvRTSSucText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><%if tcWebApi_get("Info_WLan","wlanRTSSuccessRcvCTS","h") <> "N/A" then tcWebApi_get("Info_WLan","wlanRTSSuccessRcvCTS","s") end if%></td>
  </tr>
  <tr>
  	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvRTSFailText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><%if tcWebApi_get("Info_WLan","wlanRTSFailRcvCTS","h") <> "N/A" then tcWebApi_get("Info_WLan","wlanRTSFailRcvCTS","s") end if%></td>
  </tr>
  <tr>
	<td width="150" height="30" class="title-main">
   		<font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessAdvRcStaticText","s")%></font></td>
	<td width="10" class="black">&nbsp;</td>
	<td width="230"><hr noshade></td>
    <td width="10"><hr noshade></td>
	<td width="360"><hr noshade></td>
  </tr>
  <tr>
 	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvRecSucText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><%if tcWebApi_get("Info_WLan","wlanRxFrames","h") <> "N/A" then tcWebApi_get("Info_WLan","wlanRxFrames","s") end if%></td>
  </tr>

  <tr>
  	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvReccrcText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><%if tcWebApi_get("Info_WLan","wlanRxErrFrames","h") <> "N/A" then tcWebApi_get("Info_WLan","wlanRxErrFrames","s") end if%></td>
  </tr>
  <tr>
	<td width="150" height="30" class="title-main">
   		<font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessAdvSNRText","s")%></font></td>
	<td width="10" class="black">&nbsp;</td>
	<td width="230"><hr noshade></td>
    <td width="10"><hr noshade></td>
	<td width="360"><hr noshade></td>
  </tr>
  <tr>
  	<td class="light-orange">&nbsp;</td>
	<td class="light-orange"></td>
    <td class="tabdata" align="right"><%tcWebApi_get("String_Entry","WirelessAdvSNRText","s")%>:</td>
	<td class="tabdata">&nbsp;</td>
    <td class="tabdata"><% tcWebApi_get("Info_WLan","wlanSNR0","s") %>, <% tcWebApi_get("Info_WLan","wlanSNR1","s") %>, <% tcWebApi_get("Info_WLan","wlanSNR2","s") %></td>
  </tr>
  <tr>
   		 <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="230">&nbsp;</td><td width="10">&nbsp;</td><td width="360">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<input type=button style="width:90px" value="<%tcWebApi_get("String_Entry","WirelessAdvIndexText","s")%>" onClick="Doreset()">&nbsp;<input type=button value="<%tcWebApi_get("String_Entry","ButtonBackText","s")%>" onClick="Doreturn()"><input type="hidden" name="reset_flag" value="0"></td></tr></table></div></form>
</body></html>

