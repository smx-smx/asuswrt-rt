<%
If Request_Form("SaveFlag")="1" Then
	TCWebApi_set("WLan_Entry","NoForwarding","apisolated")
	If Request_Form("Is11nMode") = "0" then
		TCWebApi_set("WLan_Entry","HT_RATE","abg_rate")
	end IF
	TCWebApi_set("WLan_Common","HT_RDG","n_rdg")
	TCWebApi_set("WLan_Common","HT_STBC","n_stbc")
	TCWebApi_set("WLan_Common","HT_AMSDU","n_amsdu")
	TCWebApi_set("WLan_Common","HT_AutoBA","n_autoba")
	TCWebApi_set("WLan_Common","HT_BADecline","n_badecline")
	TCWebApi_set("WLan_Common","HT_DisallowTKIP","n_disallow_tkip")
	TCWebApi_set("WLan_Common","HT_TxStream","tx_stream")
	TCWebApi_set("WLan_Common","HT_RxStream","rx_stream")
	tcWebApi_commit("WLan_Entry")
end if
%>
<html>
<head>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<META HTTP-EQUIV="Expires" CONTENT="-1">
<META http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<title>Basic Wireless Settings</title>

<script language="JavaScript" type="text/javascript">

var PhyMode  = '<% tcWebApi_get("WLan_Common","WirelessMode","s") %>';
var HiddenSSID  = '<% tcWebApi_get("WLan_Entry","HideSSID","s") %>';
var APIsolated = '<% tcWebApi_get("WLan_Entry","NoForwarding","s") %>';
var mbssidapisolated = '<% tcWebApi_get("WLan_Common","NoForwardingBTNBSSID","s") %>';
var ChIdx  = '<% tcWebApi_get("WLan_Common","Channel","s") %>';
var fxtxmode = '<% tcWebApi_get("WLan_Common","FixedTxMode","s") %>';
var CntyCd = '<% tcWebApi_get("WLan_Common","Country","s") %>';
var ht_mode = '<% tcWebApi_get("WLan_Common","HT_OpMode","s") %>';
var ht_bw = '<% tcWebApi_get("WLan_Common","HT_BW","s") %>';
var ht_gi = '<% tcWebApi_get("WLan_Common","HT_GI","s") %>';
var ht_stbc = '<% tcWebApi_get("WLan_Common","HT_STBC","s") %>';
var ht_mcs = '<% tcWebApi_get("WLan_Entry","HT_MCS","s") %>';
var ht_rdg = '<% tcWebApi_get("WLan_Common","HT_RDG","s") %>';
var ht_extcha = '<% tcWebApi_get("WLan_Common","HT_EXTCHA","s") %>';
var ht_amsdu = '<% tcWebApi_get("WLan_Common","HT_AMSDU","s") %>';
var ht_autoba = '<% tcWebApi_get("WLan_Common","HT_AutoBA","s") %>';
var ht_badecline = '<% tcWebApi_get("WLan_Common","HT_BADecline","s") %>';
var ht_disallow_tkip = '<% tcWebApi_get("WLan_Common","HT_DisallowTKIP","s") %>';
var apcli_include = '0';//' getWlanApcliBuilt(); ';
var mesh_include = '0';//' getMeshBuilt(); ';
var tx_stream_idx = '<% tcWebApi_get("WLan_Common","HT_TxStream","s") %>';
var rx_stream_idx = '<% tcWebApi_get("WLan_Common","HT_RxStream","s") %>';
var is3t3r = '0';//' is3t3r(); ';
var max_bssid_num = '<% tcWebApi_get("WLan_Common","BssidNum","s") %>';

if (APIsolated == 'N/A')
	APIsolated='0';
if (mbssidapisolated == 'N/A')
	mbssidapisolated='0';
if (ht_mode == 'N/A')
	ht_mode='0';
if (ht_stbc == 'N/A')
	ht_stbc='1';
if (ht_rdg == 'N/A')
	ht_rdg='1';
if (ht_amsdu == 'N/A')
	ht_amsdu='0';
if (ht_autoba == 'N/A')
	ht_autoba='1';
if (ht_badecline == 'N/A')
	ht_badecline='1';
if (ht_disallow_tkip == 'N/A')
	ht_disallow_tkip='0';
if (tx_stream_idx == 'N/A')
	tx_stream_idx='1';
if (rx_stream_idx == 'N/A')
	rx_stream_idx='1';		
	
ChLst_24G = new Array(14);
ChLst_24G[0] = "2412MHz (Channel 1)";
ChLst_24G[1] = "2417MHz (Channel 2)";
ChLst_24G[2] = "2422MHz (Channel 3)";
ChLst_24G[3] = "2427MHz (Channel 4)";
ChLst_24G[4] = "2432MHz (Channel 5)";
ChLst_24G[5] = "2437MHz (Channel 6)";
ChLst_24G[6] = "2442MHz (Channel 7)";
ChLst_24G[7] = "2447MHz (Channel 8)";
ChLst_24G[8] = "2452MHz (Channel 9)";
ChLst_24G[9] = "2457MHz (Channel 10)";
ChLst_24G[10] = "2462MHz (Channel 11)";
ChLst_24G[11] = "2467MHz (Channel 12)";
ChLst_24G[12] = "2472MHz (Channel 13)";
ChLst_24G[13] = "2484MHz (Channel 14)";

ChLst_5G = new Array(33);
ChLst_5G[0] = "5180MHz (Channel 36)";
ChLst_5G[1] = "5200MHz (Channel 40)";
ChLst_5G[2] = "5220MHz (Channel 44)";
ChLst_5G[3] = "5240MHz (Channel 48)";
ChLst_5G[4] = "5260MHz (Channel 52)";
ChLst_5G[5] = "5280MHz (Channel 56)";
ChLst_5G[6] = "5300MHz (Channel 60)";
ChLst_5G[7] = "5320MHz (Channel 64)";
ChLst_5G[16] = "5500MHz (Channel 100)";
ChLst_5G[17] = "5520MHz (Channel 104)";
ChLst_5G[18] = "5540MHz (Channel 108)";
ChLst_5G[19] = "5560MHz (Channel 112)";
ChLst_5G[20] = "5580MHz (Channel 116)";
ChLst_5G[21] = "5600MHz (Channel 120)";
ChLst_5G[22] = "5620MHz (Channel 124)";
ChLst_5G[23] = "5640MHz (Channel 128)";
ChLst_5G[24] = "5660MHz (Channel 132)";
ChLst_5G[25] = "5680MHz (Channel 136)";
ChLst_5G[26] = "5700MHz (Channel 140)";
ChLst_5G[28] = "5745MHz (Channel 149)";
ChLst_5G[29] = "5765MHz (Channel 153)";
ChLst_5G[30] = "5785MHz (Channel 157)";
ChLst_5G[31] = "5805MHz (Channel 161)";
ChLst_5G[32] = "5825MHz (Channel 165)";

HT5GExtCh = new Array(22);
HT5GExtCh[0] = new Array(1, "5200MHz (Channel 40)"); //36's extension channel
HT5GExtCh[1] = new Array(0, "5180MHz (Channel 36)"); //40's extension channel
HT5GExtCh[2] = new Array(1, "5240MHz (Channel 48)"); //44's
HT5GExtCh[3] = new Array(0, "5220MHz (Channel 44)"); //48's
HT5GExtCh[4] = new Array(1, "5280MHz (Channel 56)"); //52's
HT5GExtCh[5] = new Array(0, "5260MHz (Channel 52)"); //56's
HT5GExtCh[6] = new Array(1, "5320MHz (Channel 64)"); //60's
HT5GExtCh[7] = new Array(0, "5300MHz (Channel 60)"); //64's
HT5GExtCh[8] = new Array(1, "5520MHz (Channel 104)"); //100's
HT5GExtCh[9] = new Array(0, "5500MHz (Channel 100)"); //104's
HT5GExtCh[10] = new Array(1, "5560MHz (Channel 112)"); //108's
HT5GExtCh[11] = new Array(0, "5540MHz (Channel 108)"); //112's
HT5GExtCh[12] = new Array(1, "5600MHz (Channel 120)"); //116's
HT5GExtCh[13] = new Array(0, "5580MHz (Channel 116)"); //120's
HT5GExtCh[14] = new Array(1, "5640MHz (Channel 128)"); //124's
HT5GExtCh[15] = new Array(0, "5620MHz (Channel 124)"); //128's
HT5GExtCh[16] = new Array(1, "5680MHz (Channel 136)"); //132's
HT5GExtCh[17] = new Array(0, "5660MHz (Channel 132)"); //136's
HT5GExtCh[18] = new Array(1, "5765MHz (Channel 153)"); //149's
HT5GExtCh[19] = new Array(0, "5745MHz (Channel 149)"); //153's
HT5GExtCh[20] = new Array(1, "5805MHz (Channel 161)"); //157's
HT5GExtCh[21] = new Array(0, "5785MHz (Channel 157)"); //161's


function StylDispOn()
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

function InsChOpt(vChannel, band)
{
	var y = document.createElement('option');

	if (1*band == 24)
	{
		y.text = ChLst_24G[1*vChannel - 1];
		y.value = 1*vChannel;
	}
	else if (1*band == 5)
	{
		y.value = 1*vChannel;
		if (1*vChannel <= 140)
			y.text = ChLst_5G[((1*vChannel) - 36) / 4];
		else
			y.text = ChLst_5G[((1*vChannel) - 36 - 1) / 4];
	}

	if (1*band == 24)
		var x=document.getElementById("sz11gChannel");
	else if (1*band == 5)
		var x=document.getElementById("sz11aChannel");

	try
	{
		x.add(y,null); // standards compliant
	}
	catch(ex)
	{
		x.add(y); // IE only
	}
}

function CreateExtChOpt(vChannel)
{
	var y = document.createElement('option');

	y.text = ChLst_24G[1*vChannel - 1];
//	y.value = 1*vChannel;
	y.value = 1;

	var x = document.getElementById("n_extcha");

	try
	{
		x.add(y,null); // standards compliant
	}
	catch(ex)
	{
		x.add(y); // IE only
	}
}

function InsExtChOpt()
{
	var wmode = document.wireless_basic.wirelessmode.options.selectedIndex;
	var OptLen; 
	var CurrCh;

	if ((1*wmode == 6) || (1*wmode == 3) || (1*wmode == 4) || (1*wmode == 7))
	{
		var x = document.getElementById("n_extcha");
		var length = document.wireless_basic.n_extcha.options.length;

		if (length > 1)
		{
			x.selectedIndex = 1;
			x.remove(x.selectedIndex);
		}

		if ((1*wmode == 6) || (1*wmode == 7))
		{
			CurrCh = document.wireless_basic.sz11aChannel.value;

			if ((1*CurrCh >= 36) && (1*CurrCh <= 64))
			{
				CurrCh = 1*CurrCh;
				CurrCh /= 4;
				CurrCh -= 9;

				x.options[0].text = HT5GExtCh[CurrCh][1];
				x.options[0].value = HT5GExtCh[CurrCh][0];
			}
			else if ((1*CurrCh >= 100) && (1*CurrCh <= 136))
			{
				CurrCh = 1*CurrCh;
				CurrCh /= 4;
				CurrCh -= 17;

				x.options[0].text = HT5GExtCh[CurrCh][1];
				x.options[0].value = HT5GExtCh[CurrCh][0];
			}
			else if ((1*CurrCh >= 149) && (1*CurrCh <= 161))
			{
				CurrCh = 1*CurrCh;
				CurrCh -= 1;
				CurrCh /= 4;
				CurrCh -= 19;

				x.options[0].text = HT5GExtCh[CurrCh][1];
				x.options[0].value = HT5GExtCh[CurrCh][0];
			}
			else
			{
				x.options[0].text = "Auto Select";
				x.options[0].value = 0;
			}
		}
		else if ((1*wmode == 3) || (1*wmode == 4))
		{
			CurrCh = document.wireless_basic.sz11gChannel.value;
			OptLen = document.wireless_basic.sz11gChannel.options.length;

			if ((CurrCh >=1) && (CurrCh <= 4))
			{
				x.options[0].text = ChLst_24G[1*CurrCh + 4 - 1];
				x.options[0].value = 1*CurrCh + 4;
			}
			else if ((CurrCh >= 5) && (CurrCh <= 7))
			{
				x.options[0].text = ChLst_24G[1*CurrCh - 4 - 1];
				x.options[0].value = 0; //1*CurrCh - 4;
				CurrCh = 1*CurrCh;
				CurrCh += 4;
				CreateExtChOpt(CurrCh);
			}
			else if ((CurrCh >= 8) && (CurrCh <= 9))
			{
				x.options[0].text = ChLst_24G[1*CurrCh - 4 - 1];
				x.options[0].value = 0; //1*CurrCh - 4;

				if (OptLen >=14)
				{
					CurrCh = 1*CurrCh;
					CurrCh += 4;
					CreateExtChOpt(CurrCh);
				}
			}
			else if (CurrCh == 10)
			{
				x.options[0].text = ChLst_24G[1*CurrCh - 4 - 1];
				x.options[0].value = 0; //1*CurrCh - 4;

				if (OptLen > 14)
				{
					CurrCh = 1*CurrCh;
					CurrCh += 4;
					CreateExtChOpt(CurrCh);
				}
			}
			else if (CurrCh >= 11)
			{
				x.options[0].text = ChLst_24G[1*CurrCh - 4 - 1];
				x.options[0].value = 0; //1*CurrCh - 4;
			}
			else
			{
				x.options[0].text = "Auto Select";
				x.options[0].value = 0;
			}
		}
	}
}

function ChOnChange()
{
	if (document.wireless_basic.n_bandwidth[1].checked == true)
	{
		var w_mode = document.wireless_basic.wirelessmode.options.selectedIndex;

		if ((1*w_mode == 6) || (1*w_mode == 7))
		{
			if (document.wireless_basic.n_bandwidth[1].checked == true)
			{
				document.getElementById("extension_channel").style.visibility = "visible";
				document.getElementById("extension_channel").style.display = StylDispOn();
				document.wireless_basic.n_extcha.disabled = false;
			}

			if (document.wireless_basic.sz11aChannel.options.selectedIndex == 0)
			{
				document.getElementById("extension_channel").style.visibility = "hidden";
				document.getElementById("extension_channel").style.display = "none";
				document.wireless_basic.n_extcha.disabled = true;
			}
		}
		else if ((1*w_mode == 3) || (1*w_mode == 4))
		{
			if (document.wireless_basic.n_bandwidth[1].checked == true)
			{
				document.getElementById("extension_channel").style.visibility = "visible";
				document.getElementById("extension_channel").style.display = StylDispOn();
				document.wireless_basic.n_extcha.disabled = false;
			}

			if (document.wireless_basic.sz11gChannel.options.selectedIndex == 0)
			{
				document.getElementById("extension_channel").style.visibility = "hidden";
				document.getElementById("extension_channel").style.display = "none";
				document.wireless_basic.n_extcha.disabled = true;
			}
		}
	}

	InsExtChOpt();
}

function ChBwOnClick()
{
	var w_mode = document.wireless_basic.wirelessmode.options.selectedIndex;

	if (document.wireless_basic.n_bandwidth[0].checked == true)
	{
		document.getElementById("extension_channel").style.visibility = "hidden";
		document.getElementById("extension_channel").style.display = "none";
		document.wireless_basic.n_extcha.disabled = true;
		if ((1*w_mode == 6) || (1*w_mode == 7))
			Chk5GBandChExcep();
	}
	else
	{
		document.getElementById("extension_channel").style.visibility = "visible";
		document.getElementById("extension_channel").style.display = StylDispOn();
		document.wireless_basic.n_extcha.disabled = false;

		if ((1*w_mode == 6) || (1*w_mode == 7))
		{
			Chk5GBandChExcep();

			if (document.wireless_basic.sz11aChannel.options.selectedIndex == 0)
			{
				document.getElementById("extension_channel").style.visibility = "hidden";
				document.getElementById("extension_channel").style.display = "none";
				document.wireless_basic.n_extcha.disabled = true;
			}
		}
	}
}

function Chk5GBandChExcep()
{
	var w_mode = document.wireless_basic.wirelessmode.options.selectedIndex;

	if ((1*w_mode == 6) || (1*w_mode == 7))
	{
		var x = document.getElementById("sz11aChannel")
		var CurrLen = document.wireless_basic.sz11aChannel.options.length;
		var CurrIdx = document.wireless_basic.sz11aChannel.options.selectedIndex;
		var CurrCh = document.wireless_basic.sz11aChannel.value;
		 
		if (1*CurrIdx == 0)
		{
			if (1*ChIdx != 0)
				CurrIdx = 1;
		}

		for (ch_idx = CurrLen - 1; ch_idx > 0; ch_idx--)
		{
			x.remove(ch_idx);
		}

		if (document.wireless_basic.n_bandwidth[1].checked == true)
		{
			if ((CntyCd == 'NONE') || (CntyCd == 'FR') || (CntyCd == 'US') ||
				(CntyCd == 'IE') || (CntyCd == 'JP') || (CntyCd == 'HK'))
			{
				for(ch = 36; ch <= 48; ch+=4)
					InsChOpt(ch, 5);
			}

			if ((CntyCd == 'NONE') || (CntyCd == 'FR') || (CntyCd == 'US') ||
				(CntyCd == 'IE') || (CntyCd == 'TW') || (CntyCd == 'HK'))
			{
				for(ch = 52; ch <= 64; ch+=4)
					InsChOpt(ch, 5);
			}

			if (CntyCd == 'NONE')
			{
				for(ch = 100; ch <= 136; ch+=4)
					InsChOpt(ch, 5);
			}

			if ((CntyCd == 'NONE') || (CntyCd == 'US') || (CntyCd == 'TW') ||
				(CntyCd == 'CN') || (CntyCd == 'HK'))
			{
				for(ch = 149; ch <= 161; ch+=4)
					InsChOpt(ch, 5);
			}

			if ((1*CurrCh == 140) || (1*CurrCh == 165))
			{
				document.wireless_basic.sz11aChannel.options.selectedIndex = (1*CurrIdx) -1;
			}
			else
			{
				document.wireless_basic.sz11aChannel.options.selectedIndex = (1*CurrIdx);
			}
		}
		else
		{
			if ((CntyCd == 'NONE') || (CntyCd == 'FR') || (CntyCd == 'US') ||
				(CntyCd == 'IE') || (CntyCd == 'JP') || (CntyCd == 'HK'))
			{
				for(ch = 36; ch <= 48; ch+=4)
					InsChOpt(ch, 5);
			}

			if ((CntyCd == 'NONE') || (CntyCd == 'FR') || (CntyCd == 'US') ||
				(CntyCd == 'IE') || (CntyCd == 'TW') || (CntyCd == 'HK'))
			{
				for(ch = 52; ch <= 64; ch+=4)
					InsChOpt(ch, 5);
			}

			if (CntyCd == 'NONE')
			{
				for(ch = 100; ch <= 140; ch+=4)
					InsChOpt(ch, 5);
			}

			if ((CntyCd == 'NONE') || (CntyCd == 'US') || (CntyCd == 'TW') ||
				(CntyCd == 'CN') || (CntyCd == 'HK'))
			{
				for(ch = 149; ch <= 161; ch+=4)
					InsChOpt(ch, 5);
			}

			if ((CntyCd == 'NONE') || (CntyCd == 'US') ||
				(CntyCd == 'CN') || (CntyCd == 'HK'))
			{
					InsChOpt(165, 5);
			}

			document.wireless_basic.sz11aChannel.options.selectedIndex = (1*CurrIdx);
		}
	}
	else if (1*w_mode == 5)
	{
		var x = document.getElementById("sz11aChannel")
		var CurrLen = document.wireless_basic.sz11aChannel.options.length;
		var CurrIdx = document.wireless_basic.sz11aChannel.options.selectedIndex;

		for (ch_idx = CurrLen - 1; ch_idx > 0; ch_idx--)
		{
			x.remove(ch_idx);
		}

		if ((CntyCd == 'NONE') || (CntyCd == 'FR') || (CntyCd == 'US') ||
			(CntyCd == 'IE') || (CntyCd == 'JP') || (CntyCd == 'HK'))
		{
			for(ch = 36; ch <= 48; ch+=4)
				InsChOpt(ch, 5);
		}

		if ((CntyCd == 'NONE') || (CntyCd == 'FR') || (CntyCd == 'US') ||
			(CntyCd == 'IE') || (CntyCd == 'TW') || (CntyCd == 'HK'))
		{
			for(ch = 52; ch <= 64; ch+=4)
				InsChOpt(ch, 5);
		}

		if (CntyCd == 'NONE')
		{
			for(ch = 100; ch <= 140; ch+=4)
				InsChOpt(ch, 5);
		}

		if ((CntyCd == 'NONE') || (CntyCd == 'US') || (CntyCd == 'TW') ||
			(CntyCd == 'CN') || (CntyCd == 'HK'))
		{
			for(ch = 149; ch <= 161; ch+=4)
				InsChOpt(ch, 5);
		}

		if ((CntyCd == 'NONE') || (CntyCd == 'US') ||
			(CntyCd == 'CN') || (CntyCd == 'HK'))
		{
				InsChOpt(165, 5);
		}

		document.wireless_basic.sz11aChannel.options.selectedIndex = (1*CurrIdx);
	}
}

function initTranslation()
{
	/*
	var e = document.getElementById("basicTitle");
	e.innerHTML = "Basic Wireless Settings ";
	e = document.getElementById("basicIntroduction");
	e.innerHTML = "You could configure the minimum number of Wireless settings for communication.";

	e = document.getElementById("basicWirelessNet");
	e.innerHTML = "Wireless Network";
	e = document.getElementById("basicRadioButton");
	e.innerHTML = "Radio On/Off";
	e = document.getElementById("basicNetMode");
	e.innerHTML = "Network Mode";
	e = document.getElementById("basicSSID");
	e.innerHTML = "Network Name(SSID)";
	e = document.getElementById("basicHSSID0");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID1");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID2");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID3");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID4");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID5");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID6");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID7");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID8");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID9");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID10");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID11");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID12");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID13");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID14");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicHSSID15");
	e.innerHTML = "Hidden";
	e = document.getElementById("basicMSSID1");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID2");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID3");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID4");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID5");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID6");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID7");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID8");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID9");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID10");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID11");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID12");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID13");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID14");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicMSSID15");
	e.innerHTML = "Multiple SSID";
	e = document.getElementById("basicBroadcastSSIDEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("basicBroadcastSSIDDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("basicBroadcastSSID");
	e.innerHTML = "Broadcast Network Name (SSID)";
	e = document.getElementById("basicApIsolatedEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("basicApIsolatedDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("basicApIsolated");
	e.innerHTML = "AP Isolation";
	e = document.getElementById("basicIsolatedSSID0");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID1");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID2");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID3");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID4");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID5");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID6");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID7");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID8");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID9");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID10");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID11");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID12");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID13");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID14");
	e.innerHTML = "Isolated";
	e = document.getElementById("basicIsolatedSSID15");
	e.innerHTML = "Isolated";

	e = document.getElementById("basicMBSSIDApIsolatedEnable");
	e.innerHTML = "Enable";
	e = document.getElementById("basicMBSSIDApIsolatedDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("basicMBSSIDApIsolated");
	e.innerHTML = "MBSSID AP Isolation";
	e = document.getElementById("basicBSSID");
	e.innerHTML = "BSSID";
	e = document.getElementById("basicFreqA");
	e.innerHTML = "Frequency (Channel)";
	e = document.getElementById("basicFreqAAuto");
	e.innerHTML = "AutoSelect";
	e = document.getElementById("basicFreqB");
	e.innerHTML = "Frequency (Channel)";
	e = document.getElementById("basicFreqBAuto");
	e.innerHTML = "AutoSelect";
	e = document.getElementById("basicFreqG");
	e.innerHTML = "Frequency (Channel)";
	e = document.getElementById("basicFreqGAuto");
	e.innerHTML = "AutoSelect";
	e = document.getElementById("basicRate");
	e.innerHTML = "Rate";

	e = document.getElementById("basicHTPhyMode");
	e.innerHTML = "HT Physical Mode";
	e = document.getElementById("basicHTOPMode");
	e.innerHTML = "Operating Mode";
      	e = document.getElementById("basicHTMixed");
	e.innerHTML = "Mixed Mode";
    	e = document.getElementById("basicHTChannelBW");
	e.innerHTML = "Channel BandWidth";
    	e = document.getElementById("basicHTGI");
	e.innerHTML = "Guard Interval";
      	e = document.getElementById("basicHTLongGI");
	e.innerHTML = "Long";
      	e = document.getElementById("basicHTAutoGI");
	e.innerHTML = "Auto";
    	e = document.getElementById("basicHTAutoMCS");
	e.innerHTML = "Auto";
    	e = document.getElementById("basicHTRDG");
	e.innerHTML = "Reverse Direction Grant(RDG)";
    	e = document.getElementById("basicHTRDGDisable");
	e.innerHTML = "Disable";
    	e = document.getElementById("basicHTRDGEnable");
	e.innerHTML = "Enable";
    	e = document.getElementById("basicHTExtChannel");
	e.innerHTML = "Extension Channel";
	e = document.getElementById("basicHTSTBC");
	e.innerHTML = "Space Time Block Coding(STBC)";
	e = document.getElementById("basicHTSTBCDisable");
	e.innerHTML = "Disable";
	e = document.getElementById("basicHTSTBCEnable");
	e.innerHTML = "Enable";
    	e = document.getElementById("basicHTAMSDU");
	e.innerHTML = "Aggregation MSDU(A-MSDU)";
    	e = document.getElementById("basicHTAMSDUDisable");
	e.innerHTML = "Disable";
    	e = document.getElementById("basicHTAMSDUEnable");
	e.innerHTML = "Enable";
    	e = document.getElementById("basicHTAddBA");
	e.innerHTML = "Auto Block ACK";
    	e = document.getElementById("basicHTAddBADisable");
	e.innerHTML = "Disable";
    	e = document.getElementById("basicHTAddBAEnable");
	e.innerHTML = "Enable";
    	e = document.getElementById("basicHTDelBA");
	e.innerHTML = "Decline BA Request";
    	e = document.getElementById("basicHTDelBADisable");
	e.innerHTML = "Disable";
    	e = document.getElementById("basicHTDelBAEnable");
	e.innerHTML = "Enable";
    	e = document.getElementById("basicHTAllowTKIPDisable");
	e.innerHTML = "Disable";
    	e = document.getElementById("basicHTAllowTKIPEnable");
	e.innerHTML = "Enable";
	
    	e = document.getElementById("basicOther");
	e.innerHTML = "Other";
    	e = document.getElementById("basicHTTxStream");
	e.innerHTML = "HT TxStream";
    	e = document.getElementById("basicHTRxStream");
	e.innerHTML = "HT RxStream";

	e = document.getElementById("basicApply");
	e.value = "Apply";
	e = document.getElementById("basicCancel");
	e.value = "Cancel";
	*/
}

function initValue()
{
	var Ch11aIdx;
	var CurrChLen;
	var radio_off = '<% tcWebApi_get("WLan_Common","APOn","s") %>';
	var mssidb = "0";//" getMBSSIDBuilt(); ";
	var BssidNum = 1*'<% tcWebApi_get("WLan_Common","BssidNum","s") %>';
	var i = 0;

	initTranslation();

	if (CntyCd == '')
		CntyCd = 'NONE';

	document.getElementById("div_11a_channel").style.visibility = "hidden";
	document.getElementById("div_11a_channel").style.display = "none";
	document.wireless_basic.sz11aChannel.disabled = true;
	document.getElementById("div_11b_channel").style.visibility = "hidden";
	document.getElementById("div_11b_channel").style.display = "none";
	document.wireless_basic.sz11bChannel.disabled = true;
	document.getElementById("div_11g_channel").style.visibility = "hidden";
	document.getElementById("div_11g_channel").style.display = "none";
	document.wireless_basic.sz11gChannel.disabled = true;
	document.getElementById("div_11n").style.display = "none";
	document.wireless_basic.n_bandwidth.disabled = true;
	document.wireless_basic.n_rdg.disabled = true;
	document.wireless_basic.n_gi.disabled = true;
	document.wireless_basic.n_mcs.disabled = true;
	
	PhyMode = 1*PhyMode;

	if ((PhyMode >= 8) || (PhyMode == 6))
	{
		if (window.ActiveXObject) // IE
			document.getElementById("div_11n").style.display = "block";
		else if (window.XMLHttpRequest)  // Mozilla, Safari,...
			document.getElementById("div_11n").style.display = "table";
		document.wireless_basic.n_bandwidth.disabled = false;
		document.wireless_basic.n_rdg.disabled = false;
		document.wireless_basic.n_gi.disabled = false;
		document.wireless_basic.n_mcs.disabled = false;
	}

	var rfic = '<% tcWebApi_get("WLan_Common","RFICType","s") %>';
	if ((rfic == "2") || (rfic == "4") || (rfic == "a") || (rfic == "d"))
	{
		document.wireless_basic.wirelessmode.options[5] = new Option("11a only", "2");
                document.wireless_basic.wirelessmode.options[6] = new Option("11a/n mixed mode", "8");
                document.wireless_basic.wirelessmode.options[7] = new Option("11n only(5G)", "11");
	}
	if ((PhyMode == 0) || (PhyMode == 4) || (PhyMode == 9) || (PhyMode == 6))
	{
		if (PhyMode == 0)
			document.wireless_basic.wirelessmode.options.selectedIndex = 0;
		else if (PhyMode == 4)
			document.wireless_basic.wirelessmode.options.selectedIndex = 2;
		else if (PhyMode == 9)
			document.wireless_basic.wirelessmode.options.selectedIndex = 3;
		else if (PhyMode == 6)
			document.wireless_basic.wirelessmode.options.selectedIndex = 4;

		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = StylDispOn();
		document.wireless_basic.sz11gChannel.disabled = false;
	}
	else if (PhyMode == 1)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 1;
		document.getElementById("div_11b_channel").style.visibility = "visible";
		document.getElementById("div_11b_channel").style.display = StylDispOn();
		document.wireless_basic.sz11bChannel.disabled = false;
	}
	else if ((PhyMode == 2) || (PhyMode == 8) || (PhyMode == 11))
	{
		if (PhyMode == 2)
			document.wireless_basic.wirelessmode.options.selectedIndex = 5;
		else if (PhyMode == 8)
			document.wireless_basic.wirelessmode.options.selectedIndex = 6;
		else if (PhyMode == 11)
			document.wireless_basic.wirelessmode.options.selectedIndex = 7;
		document.getElementById("div_11a_channel").style.visibility = "visible";
		document.getElementById("div_11a_channel").style.display = StylDispOn();
		document.wireless_basic.sz11aChannel.disabled = false;
	}

	for (i=1;i<16;i++)
	{
		var ssid = eval("document.wireless_basic.mssid_"+i+".disabled");
		var div = eval("document.getElementById(\"div_hssid"+i+"\").style");

		div.visibility = "hidden";
		div.display = "none";
		ssid = true;
		document.wireless_basic.hssid[i].disabled = true;
	}
	if (mssidb == "1")
	{
		var mssid16b = "0";//" get16MBSSIDBuilt(); ";
		var num = 8;
		if (mssid16b == "1")
			num = 16;
		for (i=1;i<num;i++)
		{
			var ssid = eval("document.wireless_basic.mssid_"+i+".disabled");
			var div = eval("document.getElementById(\"div_hssid"+i+"\").style");

			div.visibility = "visible";
			div.display = StylDispOn();
			ssid = false;
			document.wireless_basic.hssid[i].disabled = false;
		}
	}

	if (HiddenSSID.indexOf("0") >= 0)
	{
		document.wireless_basic.broadcastssid[0].checked = true;
		var HiddenSSIDArray = HiddenSSID.split(";");
		for (i=0;i<BssidNum;i++)
		{
			if (HiddenSSIDArray[i] == "1")
				document.wireless_basic.hssid[i].checked = true;
			else
				document.wireless_basic.hssid[i].checked = false;
		}
	}
	else
	{
		document.wireless_basic.broadcastssid[1].checked = true;
		for (i=0;i<BssidNum;i++)
			document.wireless_basic.hssid[i].checked = true;
	}

	if (APIsolated.indexOf("1") >= 0)
	{
		document.wireless_basic.apisolated[0].checked = true;
		/*
		var APIsolatedArray = APIsolated.split(";");
		for (i=0;i<BssidNum;i++)
		{
			if (APIsolatedArray[i] == "1")
				document.wireless_basic.isolated_ssid[i].checked = true;
			else
				document.wireless_basic.isolated_ssid[i].checked = false;
		}
		*/
		document.wireless_basic.isolated_ssid[0].checked = true;
	}
	else
	{
		document.wireless_basic.apisolated[1].checked = true;
		/*
		for (i=0;i<BssidNum;i++)
			document.wireless_basic.isolated_ssid[i].checked = false;
		*/
		document.wireless_basic.isolated_ssid[0].checked = false;
	}

	if (1*ht_bw == 0)
	{
		document.wireless_basic.n_bandwidth[0].checked = true;
		document.getElementById("extension_channel").style.visibility = "hidden";
		document.getElementById("extension_channel").style.display = "none";
		document.wireless_basic.n_extcha.disabled = true;
	}
	else
	{
		document.wireless_basic.n_bandwidth[1].checked = true;
		document.getElementById("extension_channel").style.visibility = "visible";
		document.getElementById("extension_channel").style.display = StylDispOn();
		document.wireless_basic.n_extcha.disabled = false;
	}

	ChIdx = 1*ChIdx;

	if ((PhyMode == 0) || (PhyMode == 4) || (PhyMode == 9) || (PhyMode == 6))
	{
		document.wireless_basic.sz11gChannel.options.selectedIndex = ChIdx;

		CurrChLen = document.wireless_basic.sz11gChannel.options.length;

		if ((ChIdx + 1) > CurrChLen)
			document.wireless_basic.sz11gChannel.options.selectedIndex = 0;
	}
	else if (PhyMode == 1)
	{
		document.wireless_basic.sz11bChannel.options.selectedIndex = ChIdx;

		CurrChLen = document.wireless_basic.sz11bChannel.options.length;

		if ((ChIdx + 1) > CurrChLen)
			document.wireless_basic.sz11bChannel.options.selectedIndex = 0;
	}
	else if ((PhyMode == 2) || (PhyMode == 8) || (PhyMode == 11))
	{
		if (CntyCd == 'NONE')
		{
			if (ChIdx <= 64)
			{
				Ch11aIdx = ChIdx;
				Ch11aIdx = Ch11aIdx / 4;
				if (Ch11aIdx != 0)
					Ch11aIdx = Ch11aIdx - 8;
			}
			else if ((ChIdx >= 100) && (ChIdx <= 140))
			{
				Ch11aIdx = ChIdx;
				Ch11aIdx = Ch11aIdx / 4;
				Ch11aIdx = Ch11aIdx - 16;
			}
			else if (ChIdx >= 149)
			{
				Ch11aIdx = ChIdx - 1;
				Ch11aIdx = Ch11aIdx / 4;
				Ch11aIdx = Ch11aIdx - 17;
			}
			else
			{
				Ch11aIdx = 0;
			}
		}
		else if ((CntyCd == 'US') || (CntyCd == 'HK') || (CntyCd == 'FR') || (CntyCd == 'IE'))
		{
			if (ChIdx <= 64)
			{
				Ch11aIdx = ChIdx;
				Ch11aIdx = Ch11aIdx / 4;
				if (Ch11aIdx != 0)
					Ch11aIdx = Ch11aIdx - 8;
			}
			else if (ChIdx >= 149)
			{
				Ch11aIdx = ChIdx - 1;
				Ch11aIdx = Ch11aIdx / 4;
				Ch11aIdx = Ch11aIdx - 28;
			}
			else
			{
				Ch11aIdx = 0;
			}
		}
		else if (CntyCd == 'JP')
		{
			if (ChIdx <= 48)
			{
				Ch11aIdx = ChIdx;
				Ch11aIdx = Ch11aIdx / 4;
				if (Ch11aIdx != 0)
					Ch11aIdx = Ch11aIdx - 8;
			}
			else
			{
				Ch11aIdx = 0;
			}
		}
		else if (CntyCd == 'TW')
		{
			if (ChIdx <= 64)
			{
				Ch11aIdx = ChIdx;
				Ch11aIdx = Ch11aIdx / 4;
				if (Ch11aIdx != 0)
					Ch11aIdx = Ch11aIdx - 12;
			}
			else if (ChIdx >= 149)
			{
				Ch11aIdx = ChIdx - 1;
				Ch11aIdx = Ch11aIdx / 4;
				Ch11aIdx = Ch11aIdx - 32;
			}
			else
			{
				Ch11aIdx = 0;
			}
		}
		else if (CntyCd == 'CN')
		{
			if (ChIdx >= 149)
			{
				Ch11aIdx = ChIdx - 1;
				Ch11aIdx = Ch11aIdx / 4;
				Ch11aIdx = Ch11aIdx - 36;
			}
			else
			{
				Ch11aIdx = 0;
			}
		}
		else
		{
			Ch11aIdx = 0;
		}

		Chk5GBandChExcep();

		if (ChIdx > 0)
			document.wireless_basic.sz11aChannel.options.selectedIndex = Ch11aIdx;
		else
			document.wireless_basic.sz11aChannel.options.selectedIndex = ChIdx;
	}

	//ABG Rate
	if ((PhyMode == 0) || (PhyMode == 2) || (PhyMode == 4))
	{
		document.wireless_basic.abg_rate.options.length = 0;
		document.wireless_basic.abg_rate.options[0] = new Option("Auto", "0");
		document.wireless_basic.abg_rate.options[1] = new Option("1 Mbps", "1");
		document.wireless_basic.abg_rate.options[2] = new Option("2 Mbps", "2");
		document.wireless_basic.abg_rate.options[3] = new Option("5.5 Mbps", "5");
		document.wireless_basic.abg_rate.options[4] = new Option("6 Mbps", "6");
		document.wireless_basic.abg_rate.options[5] = new Option("9 Mbps", "9");
		document.wireless_basic.abg_rate.options[6] = new Option("11 Mbps", "11");
		document.wireless_basic.abg_rate.options[7] = new Option("12 Mbps", "12");
		document.wireless_basic.abg_rate.options[8] = new Option("18 Mbps", "18");
		document.wireless_basic.abg_rate.options[9] = new Option("24 Mbps", "24");
		document.wireless_basic.abg_rate.options[10] = new Option("36 Mbps", "36");
		document.wireless_basic.abg_rate.options[11] = new Option("48 Mbps", "48");
		document.wireless_basic.abg_rate.options[12] = new Option("54 Mbps", "54");
		if (fxtxmode == "CCK" || fxtxmode == "cck") {
			if (ht_mcs.indexOf("33") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 0;
			else if (ht_mcs.indexOf("0") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 1;
			else if (ht_mcs.indexOf("1") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 2;
			else if (ht_mcs.indexOf("2") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 3;
			else if (ht_mcs.indexOf("3") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 6;
		}
		else {
			if (ht_mcs.indexOf("33") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 0;
			else if (ht_mcs.indexOf("0") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 4;
			else if (ht_mcs.indexOf("1") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 5;
			else if (ht_mcs.indexOf("2") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 7;
			else if (ht_mcs.indexOf("3") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 8;
			else if (ht_mcs.indexOf("4") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 9;
			else if (ht_mcs.indexOf("5") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 10;
			else if (ht_mcs.indexOf("6") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 11;
			else if (ht_mcs.indexOf("7") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 12;
		}
	}
	else if (PhyMode == 1)
	{
		document.wireless_basic.abg_rate.options.length = 0;
		document.wireless_basic.abg_rate.options[0] = new Option("Auto", "0");
		document.wireless_basic.abg_rate.options[1] = new Option("1 Mbps", "1");
		document.wireless_basic.abg_rate.options[2] = new Option("2 Mbps", "2");
		document.wireless_basic.abg_rate.options[3] = new Option("5.5 Mbps", "5");
		document.wireless_basic.abg_rate.options[4] = new Option("11 Mbps", "11");
		if (ht_mcs.indexOf("33") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 0;
		else if (ht_mcs.indexOf("0") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 1;
		else if (ht_mcs.indexOf("1") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 2;
		else if (ht_mcs.indexOf("2") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 3;
		else if (ht_mcs.indexOf("3") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 4;
	}
	else
	{
		document.getElementById("div_abg_rate").style.visibility = "hidden";
		document.getElementById("div_abg_rate").style.display = "none";
		document.wireless_basic.abg_rate.disabled = true;
	}

	document.getElementById("div_mbssidapisolated").style.visibility = "hidden";
	document.getElementById("div_mbssidapisolated").style.display = "none";
	document.wireless_basic.mbssidapisolated.disabled = true;
	if (mssidb == "1")
	{
		document.getElementById("div_mbssidapisolated").style.visibility = "visible";
		document.getElementById("div_mbssidapisolated").style.display = StylDispOn();
		document.wireless_basic.mbssidapisolated.disabled = false;
		if (mbssidapisolated == "1")
			document.wireless_basic.mbssidapisolated[0].checked = true;
		else
			document.wireless_basic.mbssidapisolated[1].checked = true;
	}

	InsExtChOpt();

	if (1*ht_stbc == 0)
	{
		document.wireless_basic.n_stbc[0].checked = true;
	}
	else
	{
		document.wireless_basic.n_stbc[1].checked = true;
	}

	if (1*ht_gi == 0)
	{
		document.wireless_basic.n_gi[0].checked = true;
	}
	else if (1*ht_gi == 1)
	{
		document.wireless_basic.n_gi[1].checked = true;
	}
	else if (1*ht_gi == 2)
	{
		document.wireless_basic.n_gi[2].checked = true;
	}

	if (1*is3t3r == 1) {
		for (i = 16; i < 24; i++) {
			document.wireless_basic.n_mcs.options[i] = new Option(i, i);
		}
	}
	var mcs_length = document.wireless_basic.n_mcs.options.length;
	if (1*is3t3r == 1) {
		document.wireless_basic.n_mcs.options[mcs_length] = new Option("32", "32");
		mcs_length++;
		document.wireless_basic.n_mcs.options[mcs_length] = new Option("Auto", "33");
		mcs_length++;
	}

	var ht_mcs_nm = 1*ht_mcs.split(";", 1);
        if (ht_mcs_nm <= 23)
                document.wireless_basic.n_mcs.options.selectedIndex = ht_mcs_nm;
        else if (ht_mcs_nm == 32)
		document.wireless_basic.n_mcs.options.selectedIndex = mcs_length-2;
        else if (ht_mcs_nm == 33)
		document.wireless_basic.n_mcs.options.selectedIndex = mcs_length-1;

	if (1*ht_rdg == 0)
		document.wireless_basic.n_rdg[0].checked = true;
	else
		document.wireless_basic.n_rdg[1].checked = true;

	var OptLen = document.wireless_basic.n_extcha.options.length;

	if (1*ht_extcha == 0)
	{
		if (OptLen > 1)
			document.wireless_basic.n_extcha.options.selectedIndex = 0;
	}
	else if (1*ht_extcha == 1)
	{
		if (OptLen > 1)
			document.wireless_basic.n_extcha.options.selectedIndex = 1;
	}
	else
	{
		document.wireless_basic.n_extcha.options.selectedIndex = 0;
	}

	if ((1*PhyMode == 8) || (1*PhyMode == 11))
	{
		if (document.wireless_basic.sz11aChannel.options.selectedIndex == 0)
		{
			document.getElementById("extension_channel").style.visibility = "hidden";
			document.getElementById("extension_channel").style.display = "none";
			document.wireless_basic.n_extcha.disabled = true;
		}
	}
	else if ((1*PhyMode == 9) || (1*PhyMode == 6))
	{
		if (document.wireless_basic.sz11gChannel.options.selectedIndex == 0)
		{
			document.getElementById("extension_channel").style.visibility = "hidden";
			document.getElementById("extension_channel").style.display = "none";
			document.wireless_basic.n_extcha.disabled = true;
		}
	}

	if (1*ht_amsdu == 0)
		document.wireless_basic.n_amsdu[0].checked = true;
	else
		document.wireless_basic.n_amsdu[1].checked = true;

	if (1*ht_autoba == 0)
		document.wireless_basic.n_autoba[0].checked = true;
	else
		document.wireless_basic.n_autoba[1].checked = true;

	if (1*ht_badecline == 0)
		document.wireless_basic.n_badecline[0].checked = true;
	else
		document.wireless_basic.n_badecline[1].checked = true;

	if (1*ht_disallow_tkip == 1)
		document.wireless_basic.n_disallow_tkip[1].checked = true;
	else
		document.wireless_basic.n_disallow_tkip[0].checked = true;

	if (1*mesh_include == 1 && 1*apcli_include == 1)
	{
		document.wireless_basic.mssid_6.disabled = true;
		document.wireless_basic.mssid_7.disabled = true;
	}
	else if (1*mesh_include == 1 || 1*apcli_include == 1)
		document.wireless_basic.mssid_7.disabled = true;

	var txpath = '<% tcWebApi_get("Info_WLan","TXPath","s") %>';
	var rxpath = '<% tcWebApi_get("Info_WLan","RXPath","s") %>';
	if ((txpath == "1")||(document.wireless_basic.Is11nMode.value==0))
	{
		//document.getElementById("div_HtTx2Stream").style.visibility = "hidden";
		//document.getElementById("div_HtTx2Stream").style.display = "none";
		tx_stream_idx = 1;
	}
	else
	{
		//document.getElementById("div_HtTx2Stream").style.visibility = "visible";
		//document.getElementById("div_HtTx2Stream").style.display = StylDispOn();
		document.wireless_basic.tx_stream.options[1] = new Option("2", "2");
	}
	if ((rxpath == "1")||(document.wireless_basic.Is11nMode.value==0))
	{
		//document.getElementById("div_HtRx2Stream").style.visibility = "hidden";
		//document.getElementById("div_HtRx2Stream").style.display = "none";
		rx_stream_idx = 1;
	}
	else
	{
		//document.getElementById("div_HtRx2Stream").style.visibility = "visible";
		//document.getElementById("div_HtRx2Stream").style.display = StylDispOn();
		document.wireless_basic.rx_stream.options[1] = new Option("2", "2");
	}
	if (1*is3t3r == 1) {
		if (rxpath == "3")
			document.wireless_basic.rx_stream.options[2] = new Option("3", "3");
		if (txpath == "3")
			document.wireless_basic.tx_stream.options[2] = new Option("3", "3");
	}
	document.wireless_basic.rx_stream.options.selectedIndex = rx_stream_idx - 1;
	document.wireless_basic.tx_stream.options.selectedIndex = tx_stream_idx - 1;

	if (1*radio_off == 1)
		document.wireless_basic.radioButton.value = "RADIO ON";
	else
		document.wireless_basic.radioButton.value = "RADIO OFF";
		
	var ra0mac = '<% tcWebApi_get("Info_ra0","hwaddr","s") %>';
	var ra1mac = '<% tcWebApi_get("Info_ra1","hwaddr","s") %>';
	var ra2mac = '<% tcWebApi_get("Info_ra2","hwaddr","s") %>';
	var ra3mac = '<% tcWebApi_get("Info_ra3","hwaddr","s") %>';
	var wlan_id = '<%tcWebApi_get("WebCurSet_Entry","wlan_id","s")%>';
	var e = document.getElementById("BssidMAC");
	if (wlan_id == '1')
		e.innerHTML = ra1mac;
	else if (wlan_id == '2')
		e.innerHTML = ra2mac;		
	else if (wlan_id == '3')
		e.innerHTML = ra3mac;
	else
		e.innerHTML = ra0mac;	
						
	//hide
	document.getElementById("div_radioButton").style.visibility = "hidden";
	document.getElementById("div_radioButton").style.display = "none";
	document.getElementById("div_wirelessmode").style.visibility = "hidden";
	document.getElementById("div_wirelessmode").style.display = "none";
	document.getElementById("div_broadcastssid").style.visibility = "hidden";
	document.getElementById("div_broadcastssid").style.display = "none";
	
	document.getElementById("div_11a_channel").style.visibility = "hidden";
	document.getElementById("div_11a_channel").style.display = "none";
	document.wireless_basic.sz11aChannel.disabled = true;
	document.getElementById("div_11b_channel").style.visibility = "hidden";
	document.getElementById("div_11b_channel").style.display = "none";
	document.wireless_basic.sz11bChannel.disabled = true;
	document.getElementById("div_11g_channel").style.visibility = "hidden";
	document.getElementById("div_11g_channel").style.display = "none";
	document.wireless_basic.sz11gChannel.disabled = true;	

	//document.getElementById("div_n_mode").style.visibility = "hidden";
	//document.getElementById("div_n_mode").style.display = "none";
	
	document.getElementById("div_n_bandwidth").style.visibility = "hidden";
	document.getElementById("div_n_bandwidth").style.display = "none";
	document.getElementById("div_n_gi").style.visibility = "hidden";
	document.getElementById("div_n_gi").style.display = "none";
	document.getElementById("div_n_mcs").style.visibility = "hidden";
	document.getElementById("div_n_mcs").style.display = "none";	
	
	document.getElementById("div_hssid0").style.visibility = "hidden";
	document.getElementById("div_hssid0").style.display = "none";			
	//~hide
}

function wirelessModeChange()
{
	var wmode;
   
	document.getElementById("div_11a_channel").style.visibility = "hidden";
	document.getElementById("div_11a_channel").style.display = "none";
	document.wireless_basic.sz11aChannel.disabled = true;
	document.getElementById("div_11b_channel").style.visibility = "hidden";
	document.getElementById("div_11b_channel").style.display = "none";
	document.wireless_basic.sz11bChannel.disabled = true;
	document.getElementById("div_11g_channel").style.visibility = "hidden";
	document.getElementById("div_11g_channel").style.display = "none";
	document.wireless_basic.sz11gChannel.disabled = true;
	document.getElementById("div_abg_rate").style.visibility = "hidden";
	document.getElementById("div_abg_rate").style.display = "none";
	document.wireless_basic.abg_rate.disabled = true;
	document.getElementById("div_11n").style.display = "none";
	document.wireless_basic.n_bandwidth.disabled = true;
	document.wireless_basic.n_rdg.disabled = true;
	document.wireless_basic.n_gi.disabled = true;
	document.wireless_basic.n_mcs.disabled = true;

	wmode = document.wireless_basic.wirelessmode.options.selectedIndex;

	wmode = 1*wmode;
	if (wmode == 0)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 0;
		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = StylDispOn();
		document.wireless_basic.sz11gChannel.disabled = false;
	}
	else if (wmode == 1)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 1;
		document.getElementById("div_11b_channel").style.visibility = "visible";
		document.getElementById("div_11b_channel").style.display = StylDispOn();
		document.wireless_basic.sz11bChannel.disabled = false;
	}
	else if (wmode == 2)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 2;
		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = StylDispOn();
		document.wireless_basic.sz11gChannel.disabled = false;
	}
	else if (wmode == 5)
	{
		document.wireless_basic.wirelessmode.options.selectedIndex = 5;
		document.getElementById("div_11a_channel").style.visibility = "visible";
		document.getElementById("div_11a_channel").style.display = StylDispOn();
		document.wireless_basic.sz11aChannel.disabled = false;

		Chk5GBandChExcep();
	}
	else if ((wmode == 6) || (wmode == 7))
	{
		if (wmode == 7)
			document.wireless_basic.wirelessmode.options.selectedIndex = 7;
		else
			document.wireless_basic.wirelessmode.options.selectedIndex = 6;
		document.getElementById("div_11a_channel").style.visibility = "visible";
		document.getElementById("div_11a_channel").style.display = StylDispOn();
		document.wireless_basic.sz11aChannel.disabled = false;
		if (window.ActiveXObject) // IE
			document.getElementById("div_11n").style.display = "block";
		else if (window.XMLHttpRequest)  // Mozilla, Safari,...
			document.getElementById("div_11n").style.display = "table";
		document.wireless_basic.n_bandwidth.disabled = false;
		document.wireless_basic.n_rdg.disabled = false;
		document.wireless_basic.n_gi.disabled = false;
		document.wireless_basic.n_mcs.disabled = false;

		Chk5GBandChExcep();

		if (document.wireless_basic.sz11aChannel.options.selectedIndex == 0)
		{
			document.getElementById("extension_channel").style.visibility = "hidden";
			document.getElementById("extension_channel").style.display = "none";
			document.wireless_basic.n_extcha.disabled = true;
		}

		InsExtChOpt();
	}
	else if ((wmode == 3) || (wmode == 4))
	{
		if (wmode == 4)
			document.wireless_basic.wirelessmode.options.selectedIndex = 4;
		else
			document.wireless_basic.wirelessmode.options.selectedIndex = 3;
		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = StylDispOn();
		document.wireless_basic.sz11gChannel.disabled = false;
		if (window.ActiveXObject) // IE
			document.getElementById("div_11n").style.display = "block";
		else if (window.XMLHttpRequest)  // Mozilla, Safari,...
			document.getElementById("div_11n").style.display = "table";
		document.wireless_basic.n_bandwidth.disabled = false;
		document.wireless_basic.n_rdg.disabled = false;
		document.wireless_basic.n_gi.disabled = false;
		document.wireless_basic.n_mcs.disabled = false;

		if (document.wireless_basic.sz11gChannel.options.selectedIndex == 0)
		{
			document.getElementById("extension_channel").style.visibility = "hidden";
			document.getElementById("extension_channel").style.display = "none";
			document.wireless_basic.n_extcha.disabled = true;
		}

		InsExtChOpt();
	}

	//ABG Rate
	if ((wmode == 0) || (wmode == 2) || (wmode == 5))
	{
		document.wireless_basic.abg_rate.options.length = 0;
		document.wireless_basic.abg_rate.options[0] = new Option("Auto", "0");
		document.wireless_basic.abg_rate.options[1] = new Option("1 Mbps", "1");
		document.wireless_basic.abg_rate.options[2] = new Option("2 Mbps", "2");
		document.wireless_basic.abg_rate.options[3] = new Option("5.5 Mbps", "5");
		document.wireless_basic.abg_rate.options[4] = new Option("6 Mbps", "6");
		document.wireless_basic.abg_rate.options[5] = new Option("9 Mbps", "9");
		document.wireless_basic.abg_rate.options[6] = new Option("11 Mbps", "11");
		document.wireless_basic.abg_rate.options[7] = new Option("12 Mbps", "12");
		document.wireless_basic.abg_rate.options[8] = new Option("18 Mbps", "18");
		document.wireless_basic.abg_rate.options[9] = new Option("24 Mbps", "24");
		document.wireless_basic.abg_rate.options[10] = new Option("36 Mbps", "36");
		document.wireless_basic.abg_rate.options[11] = new Option("48 Mbps", "48");
		document.wireless_basic.abg_rate.options[12] = new Option("54 Mbps", "54");
		if (fxtxmode == "CCK" || fxtxmode == "cck") {
			if (ht_mcs.indexOf("33") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 0;
			else if (ht_mcs.indexOf("0") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 1;
			else if (ht_mcs.indexOf("1") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 2;
			else if (ht_mcs.indexOf("2") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 3;
			else if (ht_mcs.indexOf("3") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 6;
		}
		else {
			if (ht_mcs.indexOf("33") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 0;
			else if (ht_mcs.indexOf("0") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 4;
			else if (ht_mcs.indexOf("1") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 5;
			else if (ht_mcs.indexOf("2") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 7;
			else if (ht_mcs.indexOf("3") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 8;
			else if (ht_mcs.indexOf("4") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 9;
			else if (ht_mcs.indexOf("5") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 10;
			else if (ht_mcs.indexOf("6") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 11;
			else if (ht_mcs.indexOf("7") == 0)
				document.wireless_basic.abg_rate.options.selectedIndex = 12;
		}

		document.getElementById("div_abg_rate").style.visibility = "visible";
		document.getElementById("div_abg_rate").style.display = StylDispOn();
		document.wireless_basic.abg_rate.disabled = false;
	}
	else if (wmode == 1)
	{
		document.wireless_basic.abg_rate.options.length = 0;
		document.wireless_basic.abg_rate.options[0] = new Option("Auto", "0");
		document.wireless_basic.abg_rate.options[1] = new Option("1 Mbps", "1");
		document.wireless_basic.abg_rate.options[2] = new Option("2 Mbps", "2");
		document.wireless_basic.abg_rate.options[3] = new Option("5.5 Mbps", "5");
		document.wireless_basic.abg_rate.options[4] = new Option("11 Mbps", "11");
		if (ht_mcs.indexOf("33") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 0;
		else if (ht_mcs.indexOf("0") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 1;
		else if (ht_mcs.indexOf("1") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 2;
		else if (ht_mcs.indexOf("2") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 3;
		else if (ht_mcs.indexOf("3") == 0)
			document.wireless_basic.abg_rate.options.selectedIndex = 4;

		document.getElementById("div_abg_rate").style.visibility = "visible";
		document.getElementById("div_abg_rate").style.display = StylDispOn();
		document.wireless_basic.abg_rate.disabled = false;
	}
}

function switch_hidden_ssid()
{
	var BssidNum = 1*'<% tcWebApi_get("WLan_Common","BssidNum","s") %>';

	if (document.wireless_basic.broadcastssid[0].checked == true)
		for (i=0;i<BssidNum;i++)
			document.wireless_basic.hssid[i].checked = false;
	else
		for (i=0;i<BssidNum;i++)
			document.wireless_basic.hssid[i].checked = true;
}

function switch_isolated_ssid()
{
	var BssidNum = 1*'<% tcWebApi_get("WLan_Common","BssidNum","s") %>';

	if (document.wireless_basic.apisolated[0].checked == true)
		for (i=0;i<BssidNum;i++)
			document.wireless_basic.isolated_ssid[i].checked = true;
	else
		for (i=0;i<BssidNum;i++)
			document.wireless_basic.isolated_ssid[i].checked = false;
}

function CheckValue()
{
	var wireless_mode;
	var submit_ssid_num;
	var Ch11aIdx;

	submit_ssid_num = 1;

	for (i = 1; i < max_bssid_num; i++)
	{
		if (eval("document.wireless_basic.mssid_"+i).value != "")
		{
			if (i > 5)
			{
				if (1*mesh_include == 1 && 1*apcli_include)
					break;
				else if (i == (max_bssid_num - 1))
					if (1*mesh_include == 1 || 1*apcli_include)
						break;
			}
			submit_ssid_num++;
		}
	}

	document.wireless_basic.bssid_num.value = submit_ssid_num;
	document.wireless_basic.SaveFlag.value = "1";
	return true;
}

function RadioStatusChange(rs)
{
	if (rs == 1) {
		document.wireless_basic.radioButton.value = "RADIO OFF";
		document.wireless_basic.radiohiddenButton.value = 0;
	}
	else {
		document.wireless_basic.radioButton.value = "RADIO ON";
		document.wireless_basic.radiohiddenButton.value = 1;
	}
}
</script>
</head>


<body onLoad="initValue()">

<form method=post name=wireless_basic action="/cgi-bin/wifiqa_basic.asp" onSubmit="return CheckValue()">
<INPUT TYPE="HIDDEN" NAME="SaveFlag" value="0">
<INPUT TYPE="HIDDEN" NAME="Is11nMode"  value="<%tcWebApi_get("WLan_Common","11nMode","s")%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bordercolor="#9BABBD" bgcolor="#FFFFFF">
<tr> 
<td height="5" class="light-orange" colspan="4">&nbsp;</td>
</tr>
  <tr>
	<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessBasicMenu0Text","s")%></font></td>
	<td width="10" class="black">&nbsp;</td>
	<td width="200"></td> 
	<td width="400" id="basicWirelessNet"></td>
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
  <tr id="div_radioButton">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata" id="basicRadioButton"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicRadioText","s")%></div></td>
    <td class="tabdata">
      <input type="button" name="radioButton" style="{width:120px;}" value="<%tcWebApi_get("String_Entry","WirelessBasicRadioButText","s")%>"
      onClick="if (this.value.indexOf('OFF') >= 0) RadioStatusChange(1); else RadioStatusChange(0); document.wireless_basic.submit();"> &nbsp; &nbsp;
      <input type=hidden name=radiohiddenButton value="2">
    </td>
  </tr>
  <tr id="div_wirelessmode"> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
    <td class="tabdata" id="basicNetMode"><%tcWebApi_get("String_Entry","WirelessBasicNetModeText","s")%></td>
    <td class="tabdata">
      <select name="wirelessmode" id="wirelessmode" size="1" onChange="wirelessModeChange()">
        <option value=0><%tcWebApi_get("String_Entry","WirelessModeOption0Text","s")%></option>
        <option value=1><%tcWebApi_get("String_Entry","WirelessModeOption1Text","s")%></option>
        <option value=4><%tcWebApi_get("String_Entry","WirelessModeOption2Text","s")%></option>
        <option value=9><%tcWebApi_get("String_Entry","WirelessModeOption3Text","s")%></option>
	<option value=6><%tcWebApi_get("String_Entry","WirelessModeOption4Text","s")%></option>
      </select>
    </td>
  </tr>
  <input type="hidden" name="bssid_num" value="1">
  <tr id="div_hssid0">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
    <td class="tabdata" id="basicSSID"><%tcWebApi_get("String_Entry","WirelessBasicNetNameText","s")%></td>
    <td>
      <input type=text name=mssid_0 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID0"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="0">
      &nbsp;&nbsp;<font id="basicIsolatedSSID0"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="0">
    </td>
  </tr>
  <tr id="div_hssid1">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td> 
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID1"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>1</td>
    <td>
      <input type=text name=mssid_1 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID1"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="1">
      &nbsp;&nbsp;<font id="basicIsolatedSSID1"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="1">
    </td>
  </tr>
  <tr id="div_hssid2">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID2"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>2</td>
    <td>
      <input type=text name=mssid_2 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID2"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="2">
      &nbsp;&nbsp;<font id="basicIsolatedSSID2"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="2">
    </td>
  </tr>
  <tr id="div_hssid3">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID3"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>3</td>
    <td><input type=text name=mssid_3 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID3"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="3">
      &nbsp;&nbsp;<font id="basicIsolatedSSID3"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="3">
    </td>
  </tr>
  <tr id="div_hssid4">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID4"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>4</td>
    <td><input type=text name=mssid_4 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID4"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="4">
      &nbsp;&nbsp;<font id="basicIsolatedSSID4"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="4">
    </td>
  </tr>
  <tr id="div_hssid5">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID5"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>5</td>
    <td><input type=text name=mssid_5 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID5"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="5">
      &nbsp;&nbsp;<font id="basicIsolatedSSID5"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="5">
    </td>
  </tr>
  <tr id="div_hssid6">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID6"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>6</td>
    <td><input type=text name=mssid_6 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID6"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="6">
      &nbsp;&nbsp;<font id="basicIsolatedSSID6"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="6">
    </td>
  </tr>
  <tr id="div_hssid7">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID7"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>7</td>
    <td class="tabdata"><input type=text name=mssid_7 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID7"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="7">
      &nbsp;&nbsp;<font id="basicIsolatedSSID7"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="7">
    </td>
  </tr>
  <tr id="div_hssid8">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID8"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>8</td>
    <td class="tabdata"><input type=text name=mssid_8 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID8"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="8">
      &nbsp;&nbsp;<font id="basicIsolatedSSID8"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="8">
    </td>
  </tr>
  <tr id="div_hssid9">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID9"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>9</td>
    <td class="tabdata"><input type=text name=mssid_9 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID9"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="9">
      &nbsp;&nbsp;<font id="basicIsolatedSSID9"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="9">
    </td>
  </tr>
  <tr id="div_hssid10">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID10"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>10</td>
    <td class="tabdata"><input type=text name=mssid_10 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID10"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="10">
      &nbsp;&nbsp;<font id="basicIsolatedSSID10"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="10">
    </td>
  </tr>
  <tr id="div_hssid11">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID11"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>11</td>
    <td class="tabdata"><input type=text name=mssid_11 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID11"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="11">
      &nbsp;&nbsp;<font id="basicIsolatedSSID11"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="11">
    </td>
  </tr>
  <tr id="div_hssid12">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID12"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>12</td>
    <td class="tabdata"><input type=text name=mssid_12 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID12"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="12">
      &nbsp;&nbsp;<font id="basicIsolatedSSID12"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="12">
    </td>
  </tr>
  <tr id="div_hssid13">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID13"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>13</td>
    <td class="tabdata"><input type=text name=mssid_13 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID13"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="13">
      &nbsp;&nbsp;<font id="basicIsolatedSSID13"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="13">
    </td>
  </tr>
  <tr id="div_hssid14">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID14"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>14</td>
    <td class="tabdata"><input type=text name=mssid_14 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID14"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="14">
      &nbsp;&nbsp;<font id="basicIsolatedSSID14"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="14">
    </td>
  </tr>
  <tr id="div_hssid15">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="basicMSSID15"><%tcWebApi_get("String_Entry","WirelessBasicMultiSDText","s")%></font>15</td>
    <td class="tabdata"><input type=text name=mssid_15 size=20 maxlength=32 value="">
      &nbsp;&nbsp;<font id="basicHSSID15"><%tcWebApi_get("String_Entry","WirelessBasicHiddenText","s")%></font>
      <input type=checkbox name=hssid value="15">
      &nbsp;&nbsp;<font id="basicIsolatedSSID15"><%tcWebApi_get("String_Entry","WirelessBasicIsoloText","s")%></font>
      <input type=checkbox name=isolated_ssid value="15">
    </td>
  </tr>
  <tr id="div_broadcastssid">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata" id="basicBroadcastSSID"><%tcWebApi_get("String_Entry","WirelessBasicBrodText","s")%></td>
    <td class="tabdata">
      <input type=radio name=broadcastssid value="1" checked onClick="switch_hidden_ssid()"><font id="basicBroadcastSSIDEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%>&nbsp;</font>
      <input type=radio name=broadcastssid value="0" onClick="switch_hidden_ssid()"><font id="basicBroadcastSSIDDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicApIsolated"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicAPIText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=apisolated value="1" onClick="switch_isolated_ssid()"><font id="basicApIsolatedEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%>&nbsp;</font>
      <input type=radio name=apisolated value="0" checked onClick="switch_isolated_ssid()"><font id="basicApIsolatedDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr id="div_mbssidapisolated">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicMBSSIDApIsolated"><%tcWebApi_get("String_Entry","WirelessBasicMDAPText","s")%></td>
    <td class="tabdata">
      <input type=radio name=mbssidapisolated value="1"><font id="basicMBSSIDApIsolatedEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%>&nbsp;</font>
      <input type=radio name=mbssidapisolated value="0" checked><font id="basicMBSSIDApIsolatedDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicBSSID"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicBssidText","s")%>:&nbsp;</div></td>    
    <td class="tabdata" id="BssidMAC">&nbsp;&nbsp;</td>
  </tr>
  <tr id="div_11a_channel" name="div_11a_channel">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata"><font id="basicFreqA"><%tcWebApi_get("String_Entry","WirelessBasicFREQText","s")%></font></td>
    <td class="tabdata">
      <select id="sz11aChannel" name="sz11aChannel" size="1" onChange="ChOnChange()">
	<option value=0 id="basicFreqAAuto"><%tcWebApi_get("String_Entry","WirelessFreqOptionText","s")%></option>
	<% getWlan11aChannels(); %>
      </select>
    </td>
  </tr>
  <tr id="div_11b_channel" name="div_11b_channel">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata"><font id="basicFreqB"><%tcWebApi_get("String_Entry","WirelessBasicFREQText","s")%></font></td>
    <td class="tabdata">
      <select id="sz11bChannel" name="sz11bChannel" size="1" onChange="ChOnChange()">
	<option value=0 id="basicFreqBAuto"><%tcWebApi_get("String_Entry","WirelessFreqOptionText","s")%></option>
	<% getWlan11bChannels(); %>
      </select>
    </td>
  </tr>
  <tr id="div_11g_channel" name="div_11g_channel">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata"><font id="basicFreqG"><%tcWebApi_get("String_Entry","WirelessBasicFREQText","s")%></font></td>
    <td class="tabdata">
      <select id="sz11gChannel" name="sz11gChannel" size="1" onChange="ChOnChange()">
	<option value=0 id="basicFreqGAuto"><%tcWebApi_get("String_Entry","WirelessFreqOptionText","s")%></option>
	<% getWlan11gChannels(); %>
      </select>
    </td>
  </tr>
  <tr id="div_abg_rate">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>  
    <td class="tabdata"><div align=right><font id="basicRate"><%tcWebApi_get("String_Entry","WirelessBasicRateText","s")%>:&nbsp;</font></div></td>
    <td class="tabdata">
      <select name="abg_rate" size="1">
      </select>
    </td>
  </tr>
</table>

<table id="div_11n" name="div_11n" width="760" border="0" align=center cellpadding="0" cellspacing="0" bordercolor="#9BABBD" bgcolor="#FFFFFF">
  <tr>
      <td width="150" height="30" class="title-main"> <font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessBasicMenu1Text","s")%></font> </td>
      <td width="10" class="black">&nbsp;</td>
      <td width="200"><hr noshade></td>
      <td width="400" id="basicHTPhyMode"><hr noshade></td>
  </tr>
  <tr id="div_n_mode">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTOPMode"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicOPMoText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
	  <script language="JavaScript" type="text/JavaScript">
	  	if(1*ht_mode == 0)
		{
			document.writeln("Mixed Mode");
		}
		else if (1*ht_mode == 1)
		{
			document.writeln("Green Field");
		}
	  </script>
    </td>
  </tr>
  <tr id="div_n_bandwidth">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTChannelBW"><%tcWebApi_get("String_Entry","WirelessBasicChanBText","s")%></td>
    <td class="tabdata"> 
      <input type=radio name=n_bandwidth value="0" onClick="ChBwOnClick()" checked>20&nbsp;
      <input type=radio name=n_bandwidth value="1" onClick="ChBwOnClick()">20/40
    </td>
  </tr>
  <tr id="div_n_gi">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTGI"><%tcWebApi_get("String_Entry","WirelessBasicGuardText","s")%></td>
    <td class="tabdata">
      <input type=radio name=n_gi value="0" checked><font id="basicHTLongGI"><%tcWebApi_get("String_Entry","WirelessBasicLongText","s")%>&nbsp;</font>
      <input type=radio name=n_gi value="1"><font id="basicHTAutoGI"><%tcWebApi_get("String_Entry","WirelessAutoText","s")%></font>
    </td>
  </tr>
  <tr id="div_n_mcs">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata"><%tcWebApi_get("String_Entry","WirelessBasicMCSText","s")%></td>
    <td class="tabdata">
      <select name="n_mcs" size="1">
	<option value="0">0</option>
	<option value="1">1</option>
	<option value="2">2</option>
	<option value="3">3</option>
	<option value="4">4</option>
	<option value="5">5</option>
	<option value="6">6</option>
	<option value="7">7</option>
	<option value="8">8</option>
	<option value="9">9</option>
	<option value="10">10</option>
	<option value="11">11</option>
	<option value="12">12</option>
	<option value="13">13</option>
	<option value="14">14</option>
	<option value="15">15</option>
	<option value="32">32</option>
	<option value="33" selected id="basicHTAutoMCS"><%tcWebApi_get("String_Entry","WirelessAutoText","s")%></option>
      </select>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTRDG"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicFREQText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=n_rdg value="0" checked><font id="basicHTRDGDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%>&nbsp;</font>
      <input type=radio name=n_rdg value="1"><font id="basicHTRDGEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>
    </td>
  </tr>
  <tr name="extension_channel" id="extension_channel">
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTExtChannel"><%tcWebApi_get("String_Entry","WirelessBasicRDGText","s")%></td>
    <td class="tabdata">
      <select id="n_extcha" name="n_extcha" size="1">
	<option value=1 selected><%tcWebApi_get("String_Entry","WirelessRDGOptionText","s")%></option>
      </select>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTSTBC"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicRDGText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=n_stbc value="0" checked><font id="basicHTSTBCDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%>&nbsp;</font>
      <input type=radio name=n_stbc value="1"><font id="basicHTSTBCEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTAMSDU"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicSpaceText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=n_amsdu value="0" checked><font id="basicHTAMSDUDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%>&nbsp;</font>
      <input type=radio name=n_amsdu value="1"><font id="basicHTAMSDUEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTAddBA"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicMSDUText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=n_autoba value="0" checked><font id="basicHTAddBADisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%>&nbsp;</font>
      <input type=radio name=n_autoba value="1"><font id="basicHTAddBAEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTDelBA"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicABAText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=n_badecline value="0" checked><font id="basicHTDelBADisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%>&nbsp;</font>
      <input type=radio name=n_badecline value="1"><font id="basicHTDelBAEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTAllowTKIP"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicHTKIPText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <input type=radio name=n_disallow_tkip value="0" checked><font id="basicHTAllowTKIPDisable"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%>&nbsp;</font>
      <input type=radio name=n_disallow_tkip value="1"><font id="basicHTAllowTKIPEnable"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>
    </td>
  </tr>
</table>

<table id="div_11n_plugfest" name="div_11n_plugfest" width="760" border="0" align=center cellpadding="0" cellspacing="0" bordercolor="#9BABBD" bgcolor="#FFFFFF">
  <tr>
      <td width="150" height="30" class="title-main"> <font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessBasicMenu2Text","s")%></font> </td>
      <td width="10" class="black">&nbsp;</td>
      <td width="200"><hr noshade></td>
      <td width="400" id="basicOther"><hr noshade></td>    
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTTxStream"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicHTTXText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <select name="tx_stream" size="1">
	<option value = 1>1</option>
      </select>
    </td>
  </tr>
  <tr>
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>   
    <td class="tabdata" id="basicHTRxStream"><div align=right><%tcWebApi_get("String_Entry","WirelessBasicHTRXText","s")%>:&nbsp;</div></td>
    <td class="tabdata">
      <select name="rx_stream" size="1">
	<option value = 1>1</option>
      </select>
    </td>
  </tr>
</table>

<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr height="42" >
    <td width="160" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
	<input type=submit value="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" id="basicApply"> &nbsp; &nbsp;
	<input type=button value="<%tcWebApi_get("String_Entry","ButtonBackText","s")%>" id="basicBack" onClick="location.replace('/cgi-bin/home_advancedwireless.asp');">
    </td></tr>
</table>
</form>

</body>
</html>

