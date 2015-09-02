<%
If Request_Form("SaveFlag")="1" Then
	TCWebApi_set("WLan_Common","APAifsn","ap_aifsn_all")
	TCWebApi_set("WLan_Common","APCwmin","ap_cwmin_all")
	TCWebApi_set("WLan_Common","APCwmax","ap_cwmax_all")
	TCWebApi_set("WLan_Common","APTxop","ap_txop_all")
	TCWebApi_set("WLan_Common","APACM","ap_acm_all")
	TCWebApi_set("WLan_Common","BSSAifsn","sta_aifsn_all")
	TCWebApi_set("WLan_Common","BSSCwmin","sta_cwmin_all")
	TCWebApi_set("WLan_Common","BSSCwmax","sta_cwmax_all")
	TCWebApi_set("WLan_Common","BSSTxop","sta_txop_all")
	TCWebApi_set("WLan_Common","BSSACM","sta_acm_all")
	TCWebApi_set("WLan_Common","AckPolicy","ap_ackpolicy_all")		
	tcWebApi_commit("WLan_Entry")
end if
%>
<html>
<head>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<title>Wireless WMM Settings</title>
<script language="JavaScript" type="text/javascript">
function CheckValue()
{
   var APAifsn_s;
   var APCwmin_s;
   var APCwmax_s;
   var APTxop_s;
   var APACM_s;
   var BSSAifsn_s;
   var BSSCwmin_s;
   var BSSCwmax_s;
   var BSSTxop_s;
   var BSSACM_s;
   var AckPolicy_s;

   if (1*document.wmm.ap_aifsn_acbe.value < 1 || document.wmm.ap_aifsn_acbe.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert0Text","s")%>');
     return false;
   }
   if (1*document.wmm.ap_aifsn_acbk.value < 1 || document.wmm.ap_aifsn_acbk.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert1Text","s")%>');
     return false;
   }
   if (1*document.wmm.ap_aifsn_acvi.value < 1 || document.wmm.ap_aifsn_acvi.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert2Text","s")%>');
     return false;
   }
   if (1*document.wmm.ap_aifsn_acvo.value < 1 || document.wmm.ap_aifsn_acvo.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert3Text","s")%>');
     return false;
   }
   APAifsn_s = '';
   APAifsn_s = APAifsn_s + document.wmm.ap_aifsn_acbe.value;
   APAifsn_s = APAifsn_s + ';';
   APAifsn_s = APAifsn_s + document.wmm.ap_aifsn_acbk.value;
   APAifsn_s = APAifsn_s + ';';
   APAifsn_s = APAifsn_s + document.wmm.ap_aifsn_acvi.value;
   APAifsn_s = APAifsn_s + ';';
   APAifsn_s = APAifsn_s + document.wmm.ap_aifsn_acvo.value;
   document.wmm.ap_aifsn_all.value = APAifsn_s;
   document.wmm.ap_aifsn_acbe.disabled = true;
   document.wmm.ap_aifsn_acbk.disabled = true;
   document.wmm.ap_aifsn_acvi.disabled = true;
   document.wmm.ap_aifsn_acvo.disabled = true;

   APCwmin_s = '';
   APCwmin_s = APCwmin_s + document.wmm.ap_cwmin_acbe.value;
   APCwmin_s = APCwmin_s + ';';
   APCwmin_s = APCwmin_s + document.wmm.ap_cwmin_acbk.value;
   APCwmin_s = APCwmin_s + ';';
   APCwmin_s = APCwmin_s + document.wmm.ap_cwmin_acvi.value;
   APCwmin_s = APCwmin_s + ';';
   APCwmin_s = APCwmin_s + document.wmm.ap_cwmin_acvo.value;
   document.wmm.ap_cwmin_all.value = APCwmin_s;
   document.wmm.ap_cwmin_acbe.disabled = true;
   document.wmm.ap_cwmin_acbk.disabled = true;
   document.wmm.ap_cwmin_acvi.disabled = true;
   document.wmm.ap_cwmin_acvo.disabled = true;

   APCwmax_s = '';
   APCwmax_s = APCwmax_s + document.wmm.ap_cwmax_acbe.value;
   APCwmax_s = APCwmax_s + ';';
   APCwmax_s = APCwmax_s + document.wmm.ap_cwmax_acbk.value;
   APCwmax_s = APCwmax_s + ';';
   APCwmax_s = APCwmax_s + document.wmm.ap_cwmax_acvi.value;
   APCwmax_s = APCwmax_s + ';';
   APCwmax_s = APCwmax_s + document.wmm.ap_cwmax_acvo.value;
   document.wmm.ap_cwmax_all.value = APCwmax_s;
   document.wmm.ap_cwmax_acbe.disabled = true;
   document.wmm.ap_cwmax_acbk.disabled = true;
   document.wmm.ap_cwmax_acvi.disabled = true;
   document.wmm.ap_cwmax_acvo.disabled = true;

   APTxop_s = '';
   APTxop_s = APTxop_s + document.wmm.ap_txop_acbe.value;
   APTxop_s = APTxop_s + ';';
   APTxop_s = APTxop_s + document.wmm.ap_txop_acbk.value;
   APTxop_s = APTxop_s + ';';
   APTxop_s = APTxop_s + document.wmm.ap_txop_acvi.value;
   APTxop_s = APTxop_s + ';';
   APTxop_s = APTxop_s + document.wmm.ap_txop_acvo.value;
   document.wmm.ap_txop_all.value = APTxop_s;
   document.wmm.ap_txop_acbe.disabled = true;
   document.wmm.ap_txop_acbk.disabled = true;
   document.wmm.ap_txop_acvi.disabled = true;
   document.wmm.ap_txop_acvo.disabled = true;

   APACM_s = '';
   if (document.wmm.ap_acm_acbe.checked)
      APACM_s = APACM_s + '1';
   else
      APACM_s = APACM_s + '0';
   APACM_s = APACM_s + ';';
   if (document.wmm.ap_acm_acbk.checked)
      APACM_s = APACM_s + '1';
   else
      APACM_s = APACM_s + '0';
   APACM_s = APACM_s + ';';
   if (document.wmm.ap_acm_acvi.checked)
      APACM_s = APACM_s + '1';
   else
      APACM_s = APACM_s + '0';
   APACM_s = APACM_s + ';';
   if (document.wmm.ap_acm_acvo.checked)
      APACM_s = APACM_s + '1';
   else
      APACM_s = APACM_s + '0';
   document.wmm.ap_acm_acbe.disabled = true;
   document.wmm.ap_acm_acbk.disabled = true;
   document.wmm.ap_acm_acvi.disabled = true;
   document.wmm.ap_acm_acvo.disabled = true;
   document.wmm.ap_acm_all.value = APACM_s;

   AckPolicy_s = '';
   if (document.wmm.ap_ackpolicy_acbe.checked)
      AckPolicy_s = AckPolicy_s + '1';
   else
      AckPolicy_s = AckPolicy_s + '0';
   AckPolicy_s = AckPolicy_s + ';';
   if (document.wmm.ap_ackpolicy_acbk.checked)
      AckPolicy_s = AckPolicy_s + '1';
   else
      AckPolicy_s = AckPolicy_s + '0';
   AckPolicy_s = AckPolicy_s + ';';
   if (document.wmm.ap_ackpolicy_acvi.checked)
      AckPolicy_s = AckPolicy_s + '1';
   else
      AckPolicy_s = AckPolicy_s + '0';
   AckPolicy_s = AckPolicy_s + ';';
   if (document.wmm.ap_ackpolicy_acvo.checked)
      AckPolicy_s = AckPolicy_s + '1';
   else
      AckPolicy_s = AckPolicy_s + '0';
   document.wmm.ap_ackpolicy_acbe.disabled = true;
   document.wmm.ap_ackpolicy_acbk.disabled = true;
   document.wmm.ap_ackpolicy_acvi.disabled = true;
   document.wmm.ap_ackpolicy_acvo.disabled = true;
   document.wmm.ap_ackpolicy_all.value = AckPolicy_s;

   if (1*document.wmm.sta_aifsn_acbe.value < 1 || document.wmm.sta_aifsn_acbe.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert4Text","s")%>');
     return false;
   }
   if (1*document.wmm.sta_aifsn_acbk.value < 1 || document.wmm.sta_aifsn_acbk.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert5Text","s")%>');
     return false;
   }
   if (1*document.wmm.sta_aifsn_acvi.value < 1 || document.wmm.sta_aifsn_acvi.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert6Text","s")%>');
     return false;
   }
   if (1*document.wmm.sta_aifsn_acvo.value < 1 || document.wmm.sta_aifsn_acvo.value > 15) {
     alert('<%tcWebApi_get("String_Entry","WirelessWMMAlert7Text","s")%>');
     return false;
   }
   BSSAifsn_s = '';
   BSSAifsn_s = BSSAifsn_s + document.wmm.sta_aifsn_acbe.value;
   BSSAifsn_s = BSSAifsn_s + ';';
   BSSAifsn_s = BSSAifsn_s + document.wmm.sta_aifsn_acbk.value;
   BSSAifsn_s = BSSAifsn_s + ';';
   BSSAifsn_s = BSSAifsn_s + document.wmm.sta_aifsn_acvi.value;
   BSSAifsn_s = BSSAifsn_s + ';';
   BSSAifsn_s = BSSAifsn_s + document.wmm.sta_aifsn_acvo.value;
   document.wmm.sta_aifsn_all.value = BSSAifsn_s;
   document.wmm.sta_aifsn_acbe.disabled = true;
   document.wmm.sta_aifsn_acbk.disabled = true;
   document.wmm.sta_aifsn_acvi.disabled = true;
   document.wmm.sta_aifsn_acvo.disabled = true;

   BSSCwmin_s = '';
   BSSCwmin_s = BSSCwmin_s + document.wmm.sta_cwmin_acbe.value;
   BSSCwmin_s = BSSCwmin_s + ';';
   BSSCwmin_s = BSSCwmin_s + document.wmm.sta_cwmin_acbk.value;
   BSSCwmin_s = BSSCwmin_s + ';';
   BSSCwmin_s = BSSCwmin_s + document.wmm.sta_cwmin_acvi.value;
   BSSCwmin_s = BSSCwmin_s + ';';
   BSSCwmin_s = BSSCwmin_s + document.wmm.sta_cwmin_acvo.value;
   document.wmm.sta_cwmin_all.value = BSSCwmin_s;
   document.wmm.sta_cwmin_acbe.disabled = true;
   document.wmm.sta_cwmin_acbk.disabled = true;
   document.wmm.sta_cwmin_acvi.disabled = true;
   document.wmm.sta_cwmin_acvo.disabled = true;

   BSSCwmax_s = '';
   BSSCwmax_s = BSSCwmax_s + document.wmm.sta_cwmax_acbe.value;
   BSSCwmax_s = BSSCwmax_s + ';';
   BSSCwmax_s = BSSCwmax_s + document.wmm.sta_cwmax_acbk.value;
   BSSCwmax_s = BSSCwmax_s + ';';
   BSSCwmax_s = BSSCwmax_s + document.wmm.sta_cwmax_acvi.value;
   BSSCwmax_s = BSSCwmax_s + ';';
   BSSCwmax_s = BSSCwmax_s + document.wmm.sta_cwmax_acvo.value;
   document.wmm.sta_cwmax_all.value = BSSCwmax_s;
   document.wmm.sta_cwmax_acbe.disabled = true;
   document.wmm.sta_cwmax_acbk.disabled = true;
   document.wmm.sta_cwmax_acvi.disabled = true;
   document.wmm.sta_cwmax_acvo.disabled = true;

   BSSTxop_s = '';
   BSSTxop_s = BSSTxop_s + document.wmm.sta_txop_acbe.value;
   BSSTxop_s = BSSTxop_s + ';';
   BSSTxop_s = BSSTxop_s + document.wmm.sta_txop_acbk.value;
   BSSTxop_s = BSSTxop_s + ';';
   BSSTxop_s = BSSTxop_s + document.wmm.sta_txop_acvi.value;
   BSSTxop_s = BSSTxop_s + ';';
   BSSTxop_s = BSSTxop_s + document.wmm.sta_txop_acvo.value;
   document.wmm.sta_txop_all.value = BSSTxop_s;
   document.wmm.sta_txop_acbe.disabled = true;
   document.wmm.sta_txop_acbk.disabled = true;
   document.wmm.sta_txop_acvi.disabled = true;
   document.wmm.sta_txop_acvo.disabled = true;

   BSSACM_s = '';
   if (document.wmm.sta_acm_acbe.checked)
      BSSACM_s = BSSACM_s + '1';
   else
      BSSACM_s = BSSACM_s + '0';
   BSSACM_s = BSSACM_s + ';';
   if (document.wmm.sta_acm_acbk.checked)
      BSSACM_s = BSSACM_s + '1';
   else
      BSSACM_s = BSSACM_s + '0';
   BSSACM_s = BSSACM_s + ';';
   if (document.wmm.sta_acm_acvi.checked)
      BSSACM_s = BSSACM_s + '1';
   else
      BSSACM_s = BSSACM_s + '0';
   BSSACM_s = BSSACM_s + ';';
   if (document.wmm.sta_acm_acvo.checked)
      BSSACM_s = BSSACM_s + '1';
   else
      BSSACM_s = BSSACM_s + '0';
   document.wmm.sta_acm_acbe.disabled = true;
   document.wmm.sta_acm_acbk.disabled = true;
   document.wmm.sta_acm_acvi.disabled = true;
   document.wmm.sta_acm_acvo.disabled = true;
   document.wmm.sta_acm_all.value = BSSACM_s;

   document.wmm.SaveFlag.value = "1";
   return true;
}

function initTranslation()
{
	var e = document.getElementById("wmmAP");
	e.innerHTML = "WMM Parameters of Access Point";
	e = document.getElementById("wmmSTA");
	e.innerHTML = "WMM Parameters of Station";

	e = document.getElementById("wmmApply");
	e.value = "Apply";
	e = document.getElementById("wmmCancel");
	e.value = "Cancel";
	e = document.getElementById("wmmClose");
	e.value = "Close";
}

function initValue()
{
   var APAifsn    = '<% tcWebApi_get("WLan_Common","APAifsn","s") %>';
   var APCwmin    = '<% tcWebApi_get("WLan_Common","APCwmin","s") %>';
   var APCwmax    = '<% tcWebApi_get("WLan_Common","APCwmax","s") %>';
   var APTxop     = '<% tcWebApi_get("WLan_Common","APTxop","s") %>';
   var APACM      = '<% tcWebApi_get("WLan_Common","APACM","s") %>';
   var BSSAifsn   = '<% tcWebApi_get("WLan_Common","BSSAifsn","s") %>';
   var BSSCwmin   = '<% tcWebApi_get("WLan_Common","BSSCwmin","s") %>';
   var BSSCwmax   = '<% tcWebApi_get("WLan_Common","BSSCwmax","s") %>';
   var BSSTxop    = '<% tcWebApi_get("WLan_Common","BSSTxop","s") %>';
   var BSSACM     = '<% tcWebApi_get("WLan_Common","BSSACM","s") %>';
   var AckPolicy  = '<% tcWebApi_get("WLan_Common","AckPolicy","s") %>';

   initTranslation();
   if (APAifsn   == "N/A") APAifsn = "3;7;1;1";
   if (APCwmin   == "N/A") APCwmin = "4;4;3;2";
   if (APCwmax   == "N/A") APCwmax = "6;10;4;3";
   if (APTxop    == "N/A") APTxop = "0;0;94;47";
   if (APACM     == "N/A") APACM = "0;0;0;0";
   if (BSSAifsn  == "N/A") BSSAifsn = "3;7;2;2";
   if (BSSCwmin  == "N/A") BSSCwmin = "4;4;3;2";
   if (BSSCwmax  == "N/A") BSSCwmax = "10;10;4;3";
   if (BSSTxop   == "N/A") BSSTxop = "0;0;94;47";
   if (BSSACM    == "N/A") BSSACM = "0;0;0;0";
   if (AckPolicy == "N/A") AckPolicy = "0;0;0;0";

   var APAifsnArray   = APAifsn.split(";");
   var APCwminArray   = APCwmin.split(";");
   var APCwmaxArray   = APCwmax.split(";");
   var APTxopArray    = APTxop.split(";");
   var APACMArray     = APACM.split(";");
   var BSSAifsnArray  = BSSAifsn.split(";");
   var BSSCwminArray  = BSSCwmin.split(";");
   var BSSCwmaxArray  = BSSCwmax.split(";");
   var BSSTxopArray   = BSSTxop.split(";");
   var BSSACMArray    = BSSACM.split(";");
   var AckPolicyArray = AckPolicy.split(";");

   document.wmm.ap_aifsn_acbe.value = APAifsnArray[0];
   document.wmm.ap_aifsn_acbk.value = APAifsnArray[1];
   document.wmm.ap_aifsn_acvi.value = APAifsnArray[2];
   document.wmm.ap_aifsn_acvo.value = APAifsnArray[3];

   document.wmm.ap_cwmin_acbe.options.selectedIndex = 1*APCwminArray[0] - 1;
   document.wmm.ap_cwmin_acbk.options.selectedIndex = 1*APCwminArray[1] - 1;
   document.wmm.ap_cwmin_acvi.options.selectedIndex = 1*APCwminArray[2] - 1;
   document.wmm.ap_cwmin_acvo.options.selectedIndex = 1*APCwminArray[3] - 1;

   document.wmm.ap_cwmax_acbe.options.selectedIndex = 1*APCwmaxArray[0] - 1;
   document.wmm.ap_cwmax_acbk.options.selectedIndex = 1*APCwmaxArray[1] - 1;
   document.wmm.ap_cwmax_acvi.options.selectedIndex = 1*APCwmaxArray[2] - 1;
   document.wmm.ap_cwmax_acvo.options.selectedIndex = 1*APCwmaxArray[3] - 1;

   document.wmm.ap_txop_acbe.value = APTxopArray[0];
   document.wmm.ap_txop_acbk.value = APTxopArray[1];
   document.wmm.ap_txop_acvi.value = APTxopArray[2];
   document.wmm.ap_txop_acvo.value = APTxopArray[3];

   if (1*APACMArray[0] == 1)
      document.wmm.ap_acm_acbe.checked = true;
   else
      document.wmm.ap_acm_acbe.checked = false;

   if (1*APACMArray[1] == 1)
      document.wmm.ap_acm_acbk.checked = true;
   else
      document.wmm.ap_acm_acbk.checked = false;

   if (1*APACMArray[2] == 1)
      document.wmm.ap_acm_acvi.checked = true;
   else
      document.wmm.ap_acm_acvi.checked = false;

   if (1*APACMArray[3] == 1)
      document.wmm.ap_acm_acvo.checked = true;
   else
      document.wmm.ap_acm_acvo.checked = false;

   if (1*AckPolicyArray[0] == 1)
      document.wmm.ap_ackpolicy_acbe.checked = true;
   else
      document.wmm.ap_ackpolicy_acbe.checked = false;

   if (1*AckPolicyArray[1] == 1)
      document.wmm.ap_ackpolicy_acbk.checked = true;
   else
      document.wmm.ap_ackpolicy_acbk.checked = false;

   if (1*AckPolicyArray[2] == 1)
      document.wmm.ap_ackpolicy_acvi.checked = true;
   else
      document.wmm.ap_ackpolicy_acvi.checked = false;

   if (1*AckPolicyArray[3] == 1)
      document.wmm.ap_ackpolicy_acvo.checked = true;
   else
      document.wmm.ap_ackpolicy_acvo.checked = false;

   document.wmm.sta_aifsn_acbe.value = BSSAifsnArray[0];
   document.wmm.sta_aifsn_acbk.value = BSSAifsnArray[1];
   document.wmm.sta_aifsn_acvi.value = BSSAifsnArray[2];
   document.wmm.sta_aifsn_acvo.value = BSSAifsnArray[3];

   document.wmm.sta_cwmin_acbe.options.selectedIndex = 1*BSSCwminArray[0] - 1;
   document.wmm.sta_cwmin_acbk.options.selectedIndex = 1*BSSCwminArray[1] - 1;
   document.wmm.sta_cwmin_acvi.options.selectedIndex = 1*BSSCwminArray[2] - 1;
   document.wmm.sta_cwmin_acvo.options.selectedIndex = 1*BSSCwminArray[3] - 1;

   document.wmm.sta_cwmax_acbe.options.selectedIndex = 1*BSSCwmaxArray[0] - 1;
   document.wmm.sta_cwmax_acbk.options.selectedIndex = 1*BSSCwmaxArray[1] - 1;
   document.wmm.sta_cwmax_acvi.options.selectedIndex = 1*BSSCwmaxArray[2] - 1;
   document.wmm.sta_cwmax_acvo.options.selectedIndex = 1*BSSCwmaxArray[3] - 1;

   document.wmm.sta_txop_acbe.value = BSSTxopArray[0];
   document.wmm.sta_txop_acbk.value = BSSTxopArray[1];
   document.wmm.sta_txop_acvi.value = BSSTxopArray[2];
   document.wmm.sta_txop_acvo.value = BSSTxopArray[3];

   if (1*BSSACMArray[0] == 1)
      document.wmm.sta_acm_acbe.checked = true;
   else
      document.wmm.sta_acm_acbe.checked = false;

   if (1*BSSACMArray[1] == 1)
      document.wmm.sta_acm_acbk.checked = true;
   else
      document.wmm.sta_acm_acbk.checked = false;

   if (1*BSSACMArray[2] == 1)
      document.wmm.sta_acm_acvi.checked = true;
   else
      document.wmm.sta_acm_acvi.checked = false;

   if (1*BSSACMArray[3] == 1)
      document.wmm.sta_acm_acvo.checked = true;
   else
      document.wmm.sta_acm_acvo.checked = false;
}

function submit_apply()
{
  if (CheckValue() == true) {
    document.wmm.submit();
    window.close();
  }
}
</script>
<title>Ralink Wireless WMM Configuration</title>
<body onLoad="initValue()">
<form method=post name=wmm action="/cgi-bin/wifiqa_wmm.asp">
<INPUT TYPE="HIDDEN" NAME="SaveFlag" value="0">
  <table align="center" width="540" border="1" cellspacing="1" cellpadding="3" vspace="2" hspace="2" bordercolor="#9BABBD">
    <tr> 
      <td class="title" align="center" bgcolor="#2C5EA4" colspan="7"><font color="#FFFFFF" id="wmmAP"><b><%tcWebApi_get("String_Entry","WirelessWMMPOAPText","s")%></b></font></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap>&nbsp;</td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMAifsnText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMCWMinText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMCWMaxText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMTxopText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMACMText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMAckPolicyText","s")%></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_BEText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_aifsn_acbe size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmin_acbe" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4 selected>15</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmax_acbe" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4>15</option>
	  <option value=5>31</option>
	  <option value=6 selected>63</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_txop_acbe size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_acm_acbe value="1"></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_ackpolicy_acbe value="1"></td>
    </tr>
    <input type=hidden name=ap_aifsn_all>
    <input type=hidden name=ap_cwmin_all>
    <input type=hidden name=ap_cwmax_all>
    <input type=hidden name=ap_txop_all>
    <input type=hidden name=ap_acm_all>
    <input type=hidden name=ap_ackpolicy_all>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_BKText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_aifsn_acbk size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmin_acbk" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4 selected>15</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmax_acbk" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4>15</option>
	  <option value=5>31</option>
	  <option value=6>63</option>
	  <option value=7>127</option>
	  <option value=8>255</option>
	  <option value=9>511</option>
	  <option value=10 selected>1023</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_txop_acbk size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_acm_acbk value="1"></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_ackpolicy_acbk value="1"></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_VIText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_aifsn_acvi size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmin_acvi" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3 selected>7</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmax_acvi" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4 selected>15</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_txop_acvi size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_acm_acvi value="1"></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_ackpolicy_acvi value="1"></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_VOText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_aifsn_acvo size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmin_acvo" size="1">
	  <option value=1>1</option>
	  <option value=2 selected>3</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="ap_cwmax_acvo" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3 selected>7</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=ap_txop_acvo size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_acm_acvo value="1"></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=ap_ackpolicy_acvo value="1"></td>
    </tr>
  </table>
  <br>
  <table align="center" width="540" border="1" cellspacing="1" cellpadding="3" vspace="2" hspace="2" bordercolor="#9BABBD">
    <tr> 
      <td class="title" align="center" bgcolor="#2C5EA4" colspan="7"><font color="#FFFFFF" id="wmmSTA"><b><%tcWebApi_get("String_Entry","WirelessWMMPOSText","s")%></b></font></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap>&nbsp;</td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMAifsnText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMCWMinText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMCWMaxText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMTxopText","s")%></td>
      <td align="center" bgcolor="#E8F8FF" nowrap><%tcWebApi_get("String_Entry","WirelessWMMACMText","s")%></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_BEText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_aifsn_acbe size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmin_acbe" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4 selected>15</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmax_acbe" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4>15</option>
	  <option value=5>31</option>
	  <option value=6>63</option>
	  <option value=7>127</option>
	  <option value=8>255</option>
	  <option value=9>511</option>
	  <option value=10 selected>1023</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_txop_acbe size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=sta_acm_acbe value="1"></td>
    </tr>
    <input type=hidden name=sta_aifsn_all>
    <input type=hidden name=sta_cwmin_all>
    <input type=hidden name=sta_cwmax_all>
    <input type=hidden name=sta_txop_all>
    <input type=hidden name=sta_acm_all>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_BKText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_aifsn_acbk size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmin_acbk" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4 selected>15</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmax_acbk" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4>15</option>
	  <option value=5>31</option>
	  <option value=6>63</option>
	  <option value=7>127</option>
	  <option value=8>255</option>
	  <option value=9>511</option>
	  <option value=10 selected>1023</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_txop_acbk size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=sta_acm_acbk value="1"></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_VIText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_aifsn_acvi size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmin_acvi" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3 selected>7</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmax_acvi" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3>7</option>
	  <option value=4 selected>15</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_txop_acvi size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=sta_acm_acvi value="1"></td>
    </tr>
    <tr>
      <td align="center" bgcolor="#E8F8FF" nowrap><b><%tcWebApi_get("String_Entry","WirelessWMMAC_VOText","s")%></b></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_aifsn_acvo size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmin_acvo" size="1">
	  <option value=1>1</option>
	  <option value=2 selected>3</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap>
	<select name="sta_cwmax_acvo" size="1">
	  <option value=1>1</option>
	  <option value=2>3</option>
	  <option value=3 selected>7</option>
	</select>
      </td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=text name=sta_txop_acvo size=4 maxlength=4></td>
      <td align="center" bgcolor="#FFFFFF" nowrap><input type=checkbox name=sta_acm_acvo value="1"></td>
    </tr>
  </table>

  <table align="center" width = "540" border = "0" cellpadding = "2" cellspacing = "1">
    <tr align="center">
      <td>
        <input type=button style="{width:120px;}" value="<%tcWebApi_get("String_Entry","ButtonApplyText","s")%>" id="wmmApply" onClick="submit_apply()">&nbsp;&nbsp;
        <input type=reset  style="{width:120px;}" value="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>" id="wmmCancel"  onClick="window.location.reload()">
        <input type=button name="Close" value="<%tcWebApi_get("String_Entry","ButtonCloseText","s")%>" id="wmmClose" style="{width:120px;}" onClick="window.close()">
      </td>
    </tr>
  </table>
</form>
</body>
</html>
