<%
If Request_Form("SaveFlag")="1" Then
	TCWebApi_set("WLan_Common","BGProtection","bg_protection")
	TCWebApi_set("WLan_Common","TxPower","tx_power")
	TCWebApi_set("WLan_Common","TxPreamble","short_preamble")
	TCWebApi_set("WLan_Common","ShortSlot","short_slot")
	TCWebApi_set("WLan_Common","TxBurst","tx_burst")
	TCWebApi_set("WLan_Common","PktAggregate","pkt_aggregate")
	TCWebApi_set("WLan_Common","IEEE80211H","ieee_80211h")
	TCWebApi_set("WLan_Entry","WMM","wmm_capable")
	TCWebApi_set("WLan_Common","APSDCapable","apsd_capable")
	If Request_Form("Is53Mode") = "1" then
		TCWebApi_set("WLan_Common","IgmpSnEn","m2u_enable")
	else
		TCWebApi_set("WLan_Entry","IgmpSnEn","m2u_enable")
	end IF
	tcWebApi_commit("WLan_Entry")
end if
%>
<html>
<head>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<title>Advanced Wireless Settings</title>

<script language="JavaScript" type="text/javascript">
var bgProtection = '<% tcWebApi_get("WLan_Common","BGProtection","s") %>';
var beaconInterval = '<% tcWebApi_get("WLan_Common","BeaconPeriod","s") %>';
var dtimValue = '<% tcWebApi_get("WLan_Common","DtimPeriod","s") %>';
var fragmentThreshold = '<% tcWebApi_get("WLan_Common","FragThreshold","s") %>';
var rtsThreshold = '<% tcWebApi_get("WLan_Common","RTSThreshold","s") %>';
var shortPreamble = '<% tcWebApi_get("WLan_Common","TxPreamble","s") %>';
var shortSlot = '<% tcWebApi_get("WLan_Common","ShortSlot","s") %>';
var txBurst = '<% tcWebApi_get("WLan_Common","TxBurst","s") %>';
var pktAggregate = '<% tcWebApi_get("WLan_Common","PktAggregate","s") %>';
var autoprovision = '<% tcWebApi_get("WLan_Common","AutoProvisionEn","s") %>';
var wmmCapable = '<% tcWebApi_get("WLan_Entry","WMM","s") %>';
var APSDCapable = '<% tcWebApi_get("WLan_Common","APSDCapable","s") %>';
var DLSCapable = '<% tcWebApi_get("WLan_Common","DLSCapable","s") %>';
var wirelessMode = '<% tcWebApi_get("WLan_Common","WirelessMode","s") %>';
var ieee80211h  = '<% tcWebApi_get("WLan_Common","IEEE80211H","s") %>';
var countrycode = '<% tcWebApi_get("WLan_Common","Country","s") %>';
var txPower = '<% tcWebApi_get("WLan_Common","TxPower","s") %>';
var DLSBuilt = '<% tcWebApi_get("Info_WLan","DLS","s") %>';
var m2uBuilt = '<% tcWebApi_get("Info_WLan","M2U","s") %>';
var autoproBuilt = '0';//' getAutoProvisionBuilt(); ';
var m2uEnabled = '<% tcWebApi_get("WLan_Entry","IgmpSnEn","s") %>';
var rt_device = '<% tcWebApi_get("WLan_Common","rt_device","s") %>'; 
if (rt_device.indexOf("53") >= 0)
	m2uEnabled = '<% tcWebApi_get("WLan_Common","IgmpSnEn","s") %>';
var dfsb = '<% tcWebApi_get("Info_WLan","DFS","s") %>';
var carrierib = '<% tcWebApi_get("Info_WLan","Carrier","s") %>';

if (bgProtection == 'N/A')
	bgProtection='0';
if (shortPreamble == 'N/A')
	shortPreamble='0';
if (txBurst == 'N/A')
	txBurst='1';
if (pktAggregate == 'N/A')
	pktAggregate='0';
if (shortSlot == 'N/A')
	shortSlot='1';
if (APSDCapable == 'N/A')
	APSDCapable='0';
if (DLSCapable == 'N/A')
	DLSCapable='0';
if (ieee80211h == 'N/A')
	ieee80211h='0';
if (txPower == 'N/A')
	txPower='100';
if (m2uEnabled == 'N/A')
	m2uEnabled='0';
					
function style_display_on()
{
	if (window.ActiveXObject)
	{ // IE
		return "block";
	}
	else if (window.XMLHttpRequest)
	{ // Mozilla, Safari,...
		return "table-row";
	}
}

function initTranslation()
{
	/*
	var e = document.getElementById("advTitle");
	e.innerHTML = "Advanced Wireless Settings";
	e = document.getElementById("advIntroduction");
	e.innerHTML = "Use the Advanced Setup page to make detailed settings for the Wireless. Advanced Setup includes items that are not available from the Basic Setup page.";
	e = document.getElementById("advWireless");
	e.innerHTML = "Advanced Wireless";
	e = document.getElementById("advBGProtect");
	e.innerHTML = "BG Protection Mode";
	e = document.getElementById("advBGProAuto");
	e.innerHTML = "Auto";
	e = document.getElementById("advBGProOn");
	e.innerHTML = "On";
	e = document.getElementById("advBGProOff");
	e.innerHTML = "Off";
	e = document.getElementById("advBeaconInterval");
	e.innerHTML = "Beacon Interval";
	e = document.getElementById("advBeaconIntervalRange");
	e.innerHTML = "(range 20 - 999, default 100)";
	e = document.getElementById("advDTIM");
	e.innerHTML = "Data Beacon Rate (DTIM)";
	e = document.getElementById("advDTIMRange");
	e.innerHTML = "(range 1 - 255, default 1)";
	e = document.getElementById("advFrag");
	e.innerHTML = "Fragment Threshold";
	e = document.getElementById("advFragRange");
	e.innerHTML = "(range 256 - 2346, default 2346)";
	e = document.getElementById("advRTS");
	e.innerHTML = "RTS Threshold";
	e = document.getElementById("advRTSRange");
	e.innerHTML = "(range 1 - 2347, default 2347)";
	e = document.getElementById("advTxPW");
	e.innerHTML = "TX Power";
	e = document.getElementById("advTxPWRange");
	e.innerHTML = "(range 1 - 100, default 100)";
	e = document.getElementById("advShortPre");
	e.innerHTML = "Short Preamble";
	e = document.getElementById("advShortPreEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advShortPreDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("advShortSlot");
	e.innerHTML = "Short Slot";
	e = document.getElementById("advShortSlotEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advShortSlotDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("advTxBurst");
	e.innerHTML = "Tx Burst";
	e = document.getElementById("advTxBurstEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advTxBurstDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("advPktAggr");
	e.innerHTML = "Pkt_Aggregate";
	e = document.getElementById("advAutoPro");
	e.innerHTML = "Auto Provision";
	e = document.getElementById("advPktAggrEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advPktAggrDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("advAutoProEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advAutoProDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("adv80211H");
	e.innerHTML = "IEEE 802.11H Support";
	e = document.getElementById("adv80211HEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("adv80211HDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("adv80211HDisableDescribe");
	e.innerHTML = "(only in A band)";
	e = document.getElementById("advRDRegion");
	e.innerHTML = "DFS RDRegion";
	e = document.getElementById("advCountryCode");
	e.innerHTML = "Country Code";
	e = document.getElementById("advCountryCodeUS");
	e.innerHTML = "US (United States)";
	e = document.getElementById("advCountryCodeJP");
	e.innerHTML = "JP (Japan)";
	e = document.getElementById("advCountryCodeFR");
	e.innerHTML = "FR (France)";
	e = document.getElementById("advCountryCodeTW");
	e.innerHTML = "TW (Taiwan)";
	e = document.getElementById("advCountryCodeIE");
	e.innerHTML = "IE (Ireland)";
	e = document.getElementById("advCountryCodeHK");
	e.innerHTML = "HK (Hong Kong)";
	e = document.getElementById("advCountryCodeNONE");
	e.innerHTML = "None";
	e = document.getElementById("advCarrierDetect");
	e.innerHTML = "Carrier Detect";
	e = document.getElementById("advCarrierDetectEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advCarrierDetectDisable");
	e.innerHTML = "Disable";

	e = document.getElementById("advWiFiMM");
	e.innerHTML = "Wi-Fi Multimedia";
	e = document.getElementById("advWMM");
	e.innerHTML = "WMM Capable";
	e = document.getElementById("advWMMEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advWMMDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("advAPDS");
	e.innerHTML = "APSD Capable";
	e = document.getElementById("advAPDSEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advAPDSDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("advDLS");
	e.innerHTML = "DLS Capable";
	e = document.getElementById("advDLSEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advDLSDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("advWMMParameter");
	e.innerHTML = "WMM Parameters";
	e = document.getElementById("advWMMConf");
	e.value = "WMM Configuration";
	e = document.getElementById("advMul2UniConver");
	e.innerHTML = "Multicast-to-Unicast Converter";
	e = document.getElementById("advMul2Uni");
	e.innerHTML = "Multicast-to-Unicast";
	e = document.getElementById("advMul2UniEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("advMul2UniDisable");
	e.innerHTML = "Disable";

	e = document.getElementById("advApply");
	e.value = "Apply";
	e = document.getElementById("advCancel");
	e.value = "Cancel";
	*/
}

function initValue()
{
	var datarateArray;

	initTranslation();
	bgProtection = 1*bgProtection;
	document.wireless_advanced.bg_protection.options.selectedIndex = bgProtection;

	wirelessMode = 1*wirelessMode;

	if ((wirelessMode == 2) || (wirelessMode == 8))
		; //document.wireless_advanced.basic_rate.disabled = true;
	else
		document.wireless_advanced.ieee_80211h.disabled = true;

	beaconInterval = 1*beaconInterval;
	document.wireless_advanced.beacon.value = beaconInterval;
	dtimValue = 1*dtimValue;
	document.wireless_advanced.dtim.value = dtimValue;
	fragmentThreshold = 1*fragmentThreshold;
	document.wireless_advanced.fragment.value = fragmentThreshold;
	rtsThreshold = 1*rtsThreshold;
	document.wireless_advanced.rts.value = rtsThreshold;

	shortPreamble = 1*shortPreamble;
	if (shortPreamble == 1)
	{
		document.wireless_advanced.short_preamble[0].checked = true;
		document.wireless_advanced.short_preamble[1].checked = false;
	}
	else
	{
		document.wireless_advanced.short_preamble[0].checked = false;
		document.wireless_advanced.short_preamble[1].checked = true;
	}

	shortSlot = 1*shortSlot;
	if (shortSlot == 1)
	{
		document.wireless_advanced.short_slot[0].checked = true;
	}
	else
	{
		document.wireless_advanced.short_slot[1].checked = true;
	}

	txBurst = 1*txBurst;
	if (txBurst == 1)
	{
		document.wireless_advanced.tx_burst[0].checked = true;
		document.wireless_advanced.tx_burst[1].checked = false;
	}
	else
	{
		document.wireless_advanced.tx_burst[0].checked = false;
		document.wireless_advanced.tx_burst[1].checked = true;
	}

	pktAggregate = 1*pktAggregate;
	if (pktAggregate == 1)
	{
		document.wireless_advanced.pkt_aggregate[0].checked = true;
		document.wireless_advanced.pkt_aggregate[1].checked = false;
	}
	else
	{
		document.wireless_advanced.pkt_aggregate[0].checked = false;
		document.wireless_advanced.pkt_aggregate[1].checked = true;
	}
	if (autoproBuilt == "1") {
		document.getElementById("div_AutoPro").style.visibility = "visible";
		document.getElementById("div_AutoPro").style.display = style_display_on();
		document.wireless_advanced.auto_provision.disabled = false;
		if (autoprovision == "1") {
			document.wireless_advanced.auto_provision[0].checked = true;
			document.wireless_advanced.auto_provision[1].checked = false;
		} else {
			document.wireless_advanced.auto_provision[0].checked = false;
			document.wireless_advanced.auto_provision[1].checked = true;
		}
	} else {
		document.getElementById("div_AutoPro").style.visibility = "hidden";
		document.getElementById("div_AutoPro").style.display = "none";
		document.wireless_advanced.auto_provision.disabled = true;
	}

	ieee80211h = 1*ieee80211h;
	if (ieee80211h == 1)
		document.wireless_advanced.ieee_80211h[0].checked = true;
	else
		document.wireless_advanced.ieee_80211h[1].checked = true;

	ieee80211h_switch();
	if (dfsb == "1" && ieee80211h == 1)
	{
		var rdregion = '<% tcWebApi_get("WLan_Common","RDRegion","s") %>';
		if (rdregion == "CE")
			document.wireless_advanced.rd_region.options.selectedIndex = 2;
		else if (rdregion == "JAP")
			document.wireless_advanced.rd_region.options.selectedIndex = 1;
		else
			document.wireless_advanced.rd_region.options.selectedIndex = 0;
	}

	carrierib = 1*carrierib;
	if (carrierib == 1)
	{
		document.getElementById("div_carrier_detect").style.visibility = "visible";
		document.getElementById("div_carrier_detect").style.display = style_display_on();
		document.wireless_advanced.carrier_detect.disabled = false;
		var carrierebl = '<% tcWebApi_get("WLan_Common","CarrierDetect","s") %>';
		if (carrierebl == "1")
		{
			document.wireless_advanced.carrier_detect[0].checked = true;
			document.wireless_advanced.carrier_detect[1].checked = false;
		}
		else
		{
			document.wireless_advanced.carrier_detect[0].checked = false;
			document.wireless_advanced.carrier_detect[1].checked = true;
		}
	}
	else
	{
		document.getElementById("div_carrier_detect").style.visibility = "hidden";
		document.getElementById("div_carrier_detect").style.display = "none";
		document.wireless_advanced.carrier_detect.disabled = true;
	}

	if (wmmCapable.indexOf("1") >= 0)
	{
		document.wireless_advanced.wmm_capable[0].checked = true;
		document.wireless_advanced.wmm_capable[1].checked = false;
	}
	else
	{
		document.wireless_advanced.wmm_capable[0].checked = false;
		document.wireless_advanced.wmm_capable[1].checked = true;
	}

	wmm_capable_enable_switch();

	APSDCapable = 1*APSDCapable;
	if (APSDCapable == 1)
	{
		document.wireless_advanced.apsd_capable[0].checked = true;
		document.wireless_advanced.apsd_capable[1].checked = false;
	}
	else
	{
		document.wireless_advanced.apsd_capable[0].checked = false;
		document.wireless_advanced.apsd_capable[1].checked = true;
	}

	if (DLSBuilt == 1)
	{
		DLSCapable = 1*DLSCapable;
		if (DLSCapable == 1)
		{
			document.wireless_advanced.dls_capable[0].checked = true;
			document.wireless_advanced.dls_capable[1].checked = false;
		}
		else
		{
			document.wireless_advanced.dls_capable[0].checked = false;
			document.wireless_advanced.dls_capable[1].checked = true;
		}
	}
	document.wireless_advanced.tx_power.value = txPower;

	if (countrycode == "US")
		document.wireless_advanced.country_code.options.selectedIndex = 0;
	else if (countrycode == "JP")
		document.wireless_advanced.country_code.options.selectedIndex = 1;
	else if (countrycode == "FR")
		document.wireless_advanced.country_code.options.selectedIndex = 2;
	else if (countrycode == "TW")
		document.wireless_advanced.country_code.options.selectedIndex = 3;
	else if (countrycode == "IE")
		document.wireless_advanced.country_code.options.selectedIndex = 4;
	else if (countrycode == "HK")
		document.wireless_advanced.country_code.options.selectedIndex = 5;
	else if (countrycode == "NONE")
		document.wireless_advanced.country_code.options.selectedIndex = 6;
	else
		document.wireless_advanced.country_code.options.selectedIndex = 6;

	//multicase to unicast converter
	m2uBuilt = 1*m2uBuilt;
	document.getElementById("div_m2u").style.display = "none";
	if (m2uBuilt == 1) {
		if (window.ActiveXObject) // IE
			document.getElementById("div_m2u").style.display = "block";
		else
			document.getElementById("div_m2u").style.display = "table";

		m2uEnabled = 1*m2uEnabled;
		if (m2uEnabled == 1)
		{
			document.wireless_advanced.m2u_enable[0].checked = true;
			document.wireless_advanced.m2u_enable[1].checked = false;
		}
		else
		{
			document.wireless_advanced.m2u_enable[0].checked = false;
			document.wireless_advanced.m2u_enable[1].checked = true;
		}
	}
	
	//hide
	document.getElementById("div_beacon").style.visibility = "hidden";
	document.getElementById("div_beacon").style.display = "none";
	document.getElementById("div_dtim").style.visibility = "hidden";
	document.getElementById("div_dtim").style.display = "none";
	document.getElementById("div_fragment").style.visibility = "hidden";
	document.getElementById("div_fragment").style.display = "none";	
	document.getElementById("div_rts").style.visibility = "hidden";
	document.getElementById("div_rts").style.display = "none";
	
	document.getElementById("div_country_code").style.visibility = "hidden";
	document.getElementById("div_country_code").style.display = "none";							
	//~hide
	
}

function CheckValue()
{

	if (document.wireless_advanced.beacon.value == "" )
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS0Text","s")%>');
		document.wireless_advanced.beacon.focus();
		document.wireless_advanced.beacon.select();
		return false;
	}

	if (isNaN(document.wireless_advanced.beacon.value) || document.wireless_advanced.beacon.value < 20 || document.wireless_advanced.beacon.value > 999)
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS1Text","s")%>');
		document.wireless_advanced.beacon.focus();
		document.wireless_advanced.beacon.select();
		return false;
	}

	if (document.wireless_advanced.dtim.value == "" )
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS2Text","s")%>');
		document.wireless_advanced.dtim.focus();
		document.wireless_advanced.dtim.select();
		return false;
	}

	if (isNaN(document.wireless_advanced.dtim.value) || document.wireless_advanced.dtim.value < 1 || document.wireless_advanced.dtim.value > 255)
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS3Text","s")%>');
		document.wireless_advanced.dtim.focus();
		document.wireless_advanced.dtim.select();
		return false;
	}

	if (document.wireless_advanced.fragment.value == "" )
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS4Text","s")%>');
		document.wireless_advanced.fragment.focus();
		document.wireless_advanced.fragment.select();
		return false;
	}

	if (isNaN(document.wireless_advanced.fragment.value) || document.wireless_advanced.fragment.value < 1 || document.wireless_advanced.fragment.value > 2346)
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS5Text","s")%>');
		document.wireless_advanced.fragment.focus();
		document.wireless_advanced.fragment.select();
		return false;
	}

	if (document.wireless_advanced.rts.value == "" )
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS6Text","s")%>');
		document.wireless_advanced.rts.focus();
		document.wireless_advanced.rts.select();
		return false;
	}

	if (isNaN(document.wireless_advanced.rts.value) || document.wireless_advanced.rts.value < 1 || document.wireless_advanced.rts.value > 2347)
	{
		alert('<%tcWebApi_get("String_Entry","WirelessAdvJS7Text","s")%>');
		document.wireless_advanced.rts.focus();
		document.wireless_advanced.rts.select();
		return false;
	}

	if (document.wireless_advanced.ieee_80211h[0].checked == true)
	{
		if (1*ieee80211h == 0)
			document.wireless_advanced.rebootAP.value = 1;
	}
	else
	{
		if (1*ieee80211h == 1)
			document.wireless_advanced.rebootAP.value = 1;
	}

	DLSBuilt = 1*DLSBuilt;
	if (document.wireless_advanced.wmm_capable[0].checked == true)
	{
		if (1*wmmCapable == 0)
			document.wireless_advanced.rebootAP.value = 1;
	}
	else
	{
		document.wireless_advanced.apsd_capable[1].checked = true;
		if (DLSBuilt == 1)
		{
			document.wireless_advanced.dls_capable[1].checked = true;
		}

		if (1*wmmCapable == 1)
			document.wireless_advanced.rebootAP.value = 1;
	}

	if (document.wireless_advanced.apsd_capable[0].checked == true)
	{
		if (1*APSDCapable == 0)
			document.wireless_advanced.rebootAP.value = 1;
	}
	else
	{
		if (1*APSDCapable == 1)
			document.wireless_advanced.rebootAP.value = 1;
	}

	if (DLSBuilt == 1)
	{
		if (document.wireless_advanced.dls_capable[0].checked == true)
		{
			if (1*DLSCapable == 0)
				document.wireless_advanced.rebootAP.value = 1;
		}
		else
		{
			if (1*DLSCapable == 1)
				document.wireless_advanced.rebootAP.value = 1;
		}
	}
	
	document.wireless_advanced.SaveFlag.value = "1";
	var rt_device = '<% tcWebApi_get("WLan_Common","rt_device","s") %>'; 
	if (rt_device.indexOf("53") >= 0)
		document.wireless_advanced.Is53Mode.value = "1";
	return true;
}

function open_wmm_window()
{
	window.open("/cgi-bin/wifiqa_wmm.asp","WMM_Parameters_List","toolbar=no,menubar=no,location=0, scrollbars=yes, resizable=no, width=640, height=480")
}

function wmm_capable_enable_switch()
{
	document.getElementById("div_apsd_capable").style.visibility = "hidden";
	document.getElementById("div_apsd_capable").style.display = "none";
	document.wireless_advanced.apsd_capable.disabled = true;
	document.getElementById("div_dls_capable").style.visibility = "hidden";
	document.getElementById("div_dls_capable").style.display = "none";
	document.wireless_advanced.dls_capable.disabled = true;

	DLSBuilt = 1*DLSBuilt;
	if (document.wireless_advanced.wmm_capable[0].checked == true)
	{
		document.getElementById("div_apsd_capable").style.visibility = "visible";
		document.getElementById("div_apsd_capable").style.display = style_display_on();
		document.wireless_advanced.apsd_capable.disabled = false;
		if (DLSBuilt == 1)
		{
			document.getElementById("div_dls_capable").style.visibility = "visible";
			document.getElementById("div_dls_capable").style.display = style_display_on();
			document.wireless_advanced.dls_capable.disabled = false;
		}
	}
}

function ieee80211h_switch()
{
	if (dfsb == "1" && document.wireless_advanced.ieee_80211h[0].checked == true)
	{
		document.getElementById("div_dfs_rdregion").style.visibility = "visible";
		document.getElementById("div_dfs_rdregion").style.display = style_display_on();
		document.wireless_advanced.rd_region.disabled = false;
	}
	else
	{
		document.getElementById("div_dfs_rdregion").style.visibility = "hidden";
		document.getElementById("div_dfs_rdregion").style.display = "none";
		document.wireless_advanced.rd_region.disabled = true;
	}
}
</script>
</head>

<body onLoad="initValue()">

<form method=post name=wireless_advanced action="/cgi-bin/wifiqa_advanced.asp" onSubmit="return CheckValue()">
<INPUT TYPE="HIDDEN" NAME="SaveFlag" value="0">
<INPUT TYPE="HIDDEN" NAME="Is53Mode"  value="0">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bordercolor="#9BABBD" bgcolor="#FFFFFF">
<tr> 
<td height="5" class="light-orange" colspan="4">&nbsp;</td>
</tr>
  <tr>
	<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessAdvWirelessText","s")%></font></td>
	<td width="10" class="black">&nbsp;</td>
	<td width="200"></td> 
	<td width="400" id="advWireless"></td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvIndexText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
	<%If tcWebApi_get("WebCurSet_Entry","wlan_id","h") <> "N/A" then tcWebApi_get("WebCurSet_Entry","wlan_id","s") end if%>
    </td>
  </tr>  
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvIndex1Text","s")%>:&nbsp;</div></td>
    <td class="tabdata">
	<%If tcWebApi_get("WLan_Entry","SSID","h") <> "N/A" then tcWebApi_get("WLan_Entry","SSID","s") end if%>
    </td>
  </tr>     
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advBGProtect"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvBGPMText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <select name="bg_protection" size="1">
	<option value=0 selected id="advBGProAuto"><%tcWebApi_get("String_Entry","WirelessAdvOptionBG0Text","s")%></option>
	<option value=1 id="advBGProOn"><%tcWebApi_get("String_Entry","WirelessAdvOptionBG1Text","s")%></option>
	<option value=2 id="advBGProOff"><%tcWebApi_get("String_Entry","WirelessAdvOptionBG2Text","s")%></option>
      </select>
    </td>
  </tr>
  <tr id="div_beacon">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advBeaconInterval"><div align=right><%tcWebApi_get("String_Entry","WirelessBeaconIntervalText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=text name=beacon size=5 maxlength=3 value="100"> ms <font color="#808080" id="advBeaconIntervalRange"><%tcWebApi_get("String_Entry","WirelessAdvRange0Text","s")%></font>
    </td>
  </tr>
  <tr id="div_dtim">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advDTIM"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvDTIMText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=text name=dtim size=5 maxlength=3 value="1"> ms <font color="#808080" id="advDTIMRange"><%tcWebApi_get("String_Entry","WirelessAdvRange1Text","s")%></font>
    </td>
  </tr>
  <tr id="div_fragment">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advFrag"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvFragText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=text name=fragment size=5 maxlength=4 value=""> <font color="#808080" id="advFragRange"><%tcWebApi_get("String_Entry","WirelessAdvRange2Text","s")%></font>
    </td>
  </tr>
  <tr id="div_rts">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advRTS"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvRTSText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=text name=rts size=5 maxlength=4 value=""> <font color="#808080" id="advRTSRange"><%tcWebApi_get("String_Entry","WirelessAdvRange3Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advTxPW"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvTXText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=text name=tx_power size=5 maxlength=3 value="100"> <font color="#808080" id="advTxPWRange"><%tcWebApi_get("String_Entry","WirelessAdvRange4Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advShortPre"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvShortPText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=short_preamble value="1" checked><font id="advShortPreEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=short_preamble value="0"><font id="advShortPreDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advShortSlot"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvShortSText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=short_slot value="1" checked><font id="advShortSlotEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=short_slot value="0"><font id="advShortSlotDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advTxBurst"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvTXBText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=tx_burst value="1" checked><font id="advTxBurstEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=tx_burst value="0"><font id="advTxBurstDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advPktAggr"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvPKTText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=pkt_aggregate value="1"><font id="advPktAggrEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=pkt_aggregate value="0" checked><font id="advPktAggrDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr id="div_AutoPro">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advAutoPro"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvProvText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=auto_provision value="1"><font id="advAutoProEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=auto_provision value="0" checked><font id="advAutoProDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="adv80211H"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvIEEEText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=ieee_80211h value="1" onClick="ieee80211h_switch()"><font id="adv80211HEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=ieee_80211h value="0" checked onClick="ieee80211h_switch()"><font id="adv80211HDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%> </font><font color="#808080" id="adv80211HDisableDescribe"><%tcWebApi_get("String_Entry","WirelessAdvRange5Text","s")%></font>
    </td>
  </tr>
  <tr id="div_dfs_rdregion" name="div_dfs_rdregion">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>    
    <td class="tabdata" id="advRDRegion"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvDFSText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
    	<select name="rd_region">
	  <option value="FCC"><%tcWebApi_get("String_Entry","WirelessAdvOptionRdreg0Text","s")%></option>
	  <option value="JAP"><%tcWebApi_get("String_Entry","WirelessAdvOptionRdreg1Text","s")%></option>
	  <option value="CE"><%tcWebApi_get("String_Entry","WirelessAdvOptionRdreg2Text","s")%></option>
	</select>
    </td>
  </tr>
  <tr id="div_country_code">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advCountryCode"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvCountryText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <select name="country_code">
        <option value="US" id="advCountryCodeUS"><%tcWebApi_get("String_Entry","WirelessAdvOptionCoun0Text","s")%></option>
        <option value="JP" id="advCountryCodeJP"><%tcWebApi_get("String_Entry","WirelessAdvOptionCoun1Text","s")%></option>
        <option value="FR" id="advCountryCodeFR"><%tcWebApi_get("String_Entry","WirelessAdvOptionCoun2Text","s")%></option>
        <option value="TW" id="advCountryCodeTW"><%tcWebApi_get("String_Entry","WirelessAdvOptionCoun3Text","s")%></option>
        <option value="IE" id="advCountryCodeIE"><%tcWebApi_get("String_Entry","WirelessAdvOptionCoun4Text","s")%></option>
        <option value="HK" id="advCountryCodeHK"><%tcWebApi_get("String_Entry","WirelessAdvOptionCoun5Text","s")%></option>
        <option value="NONE" selected id="advCountryCodeNONE"><%tcWebApi_get("String_Entry","WirelessAdvOptionCoun6Text","s")%></option>
      </select>
    </td>
  </tr>
  <tr id="div_carrier_detect" name="div_carrier_detect">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>    
    <td class="tabdata" id="advCarrierDetect"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvCarrText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type="radio" name="carrier_detect" value="1"><font id="advCarrierDetectEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>&nbsp;&nbsp;
      <input type="radio" name="carrier_detect" value="0" checked><font id="advCarrierDetectDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
</table>

<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bordercolor="#9BABBD" bgcolor="#FFFFFF">
  <tr>
      <td width="150" height="30" class="title-main"> <font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessAdvWifiMultiText","s")%></font> </td>
      <td width="10" class="black">&nbsp;</td>
      <td width="200"><hr noshade></td>
      <td width="400" id="advWiFiMM"><hr noshade></td>    
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advWMM"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvWMMText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=wmm_capable value="1" onClick="wmm_capable_enable_switch()" checked><font id="advWMMEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=wmm_capable value="0" onClick="wmm_capable_enable_switch()"><font id="advWMMDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr id="div_apsd_capable" name="div_apsd_capable">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>    
    <td class="tabdata" id="advAPDS"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvAPSDText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=apsd_capable value="1"><font id="advAPDSEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=apsd_capable value="0" checked><font id="advAPDSDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr id="div_dls_capable" name="div_dls_capable">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>    
    <td class="tabdata" id="advDLS"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvDLSText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=dls_capable value="1"><font id="advDLSEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name=dls_capable value="0" checked><font id="advDLSDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>     
    <td class="tabdata" id="advWMMParameter"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvWMMPText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=button name="wmm_list" value="<%tcWebApi_get("String_Entry","WirelessAdvWMMCText","s")%>" id="advWMMConf" onClick="open_wmm_window()">
    </td>
  </tr>
  <input type="hidden" name="rebootAP" value="0">
</table>

<table id="div_m2u" name="div_m2u" width="760" border="0" align=center cellpadding="0" cellspacing="0" bordercolor="#9BABBD" bgcolor="#FFFFFF">
  <tr>
      <td width="150" height="30" class="title-main"> <font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessAdvMultiConText","s")%></font> </td>
      <td width="10" class="black">&nbsp;</td>
      <td width="200"><hr noshade></td>
      <td width="400" id="advMul2UniConver"><hr noshade></td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
    <td class="tabdata" id="advMul2Uni"><div align=right><%tcWebApi_get("String_Entry","WirelessAdvMultiUnText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name="m2u_enable" value="1"><font id="advMul2UniEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%> &nbsp;</font>
      <input type=radio name="m2u_enable" value="0"><font id="advMul2UniDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
</table>

<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr height="42" >
    <td width="160" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
      <input type=submit value="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" id="advApply"> &nbsp; &nbsp;
      <input type=button value="<%tcWebApi_get("String_Entry","ButtonBackText","s")%>" id="advCancel" onClick="location.replace('/cgi-bin/home_advancedwireless.asp');">
    </td></tr>
</table>
</form>

</body>
</html>

