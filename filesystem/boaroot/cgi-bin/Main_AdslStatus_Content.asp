<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--Main_AdslStatus_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu_dsl_log","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<style>
.FormTable_log{
	font-size:12px;
	font-family:Arial, Helvetica, sans-serif;
	border: 1px solid #000000;
	border-collapse: collapse;
}
.FormTable_log thead td{
	color: #FFFFFF;
	font-size:12px;
	background-color:#405054;
}
.FormTable_log thead th{
	color: #FFFFFF;
	font-size:12px;
	background-color:#4D595D;
	text-align:left;
	font-weight:bolder;
	border: 1px solid #222;
	padding: 3px;
	padding-left: 10px;
	height: 15px;
	border-collapse: collapse;
	background: #92A0A5; /* Old browsers */
	background: -moz-linear-gradient(top, #92A0A5  0%, #66757C 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#92A0A5 ), color-stop(100%,#66757C)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(top, #92A0A5  0%, #66757C 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(top,  #92A0A5 0%, #66757C 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(top,  #92A0A5  0%, #66757C 100%); /* IE10+ */
	background: linear-gradient(to bottom, #92A0A5  0%, #66757C 100%); /* W3C */
	filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#92A0A5', endColorstr='#66757C',GradientType=0 ); /* IE6-9 */
}
.FormTable_log th{
	font-family:Arial, Helvetica, sans-serif;
	background-color:#1F2D35;
	color:#FFFFFF;	/*Viz add*/
	font-weight:normal;
	line-height:15px;
	height: 21px;
	text-align:left;
	font-size:12px;
	padding-left: 10px;
	border: 1px solid #222;
	border-collapse: collapse;
	background:#2F3A3E;
}
.FormTable_log td{
	padding-left: 10px;
	background-color:#475A5F;
	border: 1px solid #222;
	border-collapse: collapse;
}
.FormTable_log td span{
	background-color:#475a5f;
	color:#FFCC00;
}
.color_ch{
	color:#CC2200;
}
</style>
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var sync_status = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
var adsl_timestamp = parseInt("<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>");
var sync_status_update = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
var adsl_timestamp_update = parseInt("<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>");
var status_isVDSLmode = "<%tcWebApi_staticGet("Info_Adsl","xDSLmode","s")%>";
var adsl_boottime = boottime - adsl_timestamp;
var dsl_type = "<% tcWebApi_get("Info_Adsl","AdslType","s") %>".replace("_", " ");
if(status_isVDSLmode == "VDSL"){
	dsl_type = dsl_type.replace("ANNEX ", "");
}
var isMT7510 = "<%tcWebApi_get("WebCustom_Entry","isMT7510","s")%>";
var log_lineState;
var log_Opmode;
var log_AdslType;
var log_SNRMarginDown;
var log_SNRMarginUp;
var log_AttenDown;
var log_AttenUp;
var log_TCM;
var log_PathModeDown;
var log_IntDepthDown;
var log_PathModeUp;
var log_IntDepthUp;
var log_DataRateDown;
var log_DataRateUp;
var log_AttainDown;
var log_AttainUp;
var log_PowerDown;
var log_PowerUp;
var log_INPDown;
var log_INPUp;
var log_INPSHINEDown;
var log_INPSHINEUp;
var log_INPREINDown;
var log_INPREINUp;
var log_CRCDown;
var log_CRCUp;
var log_VDSL_CurrentProfile;
var log_FarEndVendorID;
<%if tcWebApi_get("WebCustom_Entry","isMT7510","h") = "Yes" then%>
var log_VDSLBAND_SNRMarginDown;
var log_VDSLBAND_SNRMarginUp;
var log_VDSLBAND_LATNDown;
var log_VDSLBAND_LATNUp;
var log_VDSLBAND_SATNDown;
var log_VDSLBAND_SATNUp;
<%end if%>

function display_line_stats(){
	if(sync_status_update == "up")
	{
		document.getElementById("line_stats").style.display = "";
	}
	else
	{
		document.getElementById("line_stats").style.display = "none";
	}
}

function display_vdsl_band_status(){
	if(isMT7510 != "Yes")
		return;

	if(status_isVDSLmode == "VDSL")
	{
		document.getElementById("th_AdslType").innerHTML = "<%tcWebApi_get("String_Entry","dslsetting_disc2_vdsl","s")%>";	/*untranslated*/
		document.getElementById("vdsl_band_status").style.display = "";
	}
	else
	{
		document.getElementById("th_AdslType").innerHTML = "<%tcWebApi_get("String_Entry","dslsetting_disc2","s")%>";
		document.getElementById("vdsl_band_status").style.display = "none";
	}
}

function update_log(){
	$j.ajax({
		url: '/cgi-bin/ajax_AdslStatus.asp',
		dataType: 'script',
		error: function(xhr){
				setTimeout("update_log();", 1000);
			},
 	
		success: function(){
				if(adsl_timestamp_update != "" && sync_status != sync_status_update){
					adsl_boottime = boottime - adsl_timestamp_update;
					showadslbootTime();
				}
				sync_status = sync_status_update;

				document.getElementById("div_VDSL_CurrentProfile").innerHTML = log_VDSL_CurrentProfile;
				document.getElementById("div_FarEndVendorID").innerHTML = log_FarEndVendorID;
				document.getElementById("div_lineState").innerHTML = log_lineState;
				document.getElementById("div_Opmode").innerHTML = log_Opmode;
				if(status_isVDSLmode == "VDSL"){
					log_AdslType = log_AdslType.replace("ANNEX ", "");
				}
				document.getElementById("div_AdslType").innerHTML = log_AdslType;

				display_line_stats();
				if(sync_status_update == "up")
				{
					document.getElementById("div_SNRMarginDown").innerHTML = log_SNRMarginDown;
					document.getElementById("div_SNRMarginUp").innerHTML = log_SNRMarginUp;
					document.getElementById("div_AttenDown").innerHTML = log_AttenDown;
					document.getElementById("div_AttenUp").innerHTML = log_AttenUp;
					document.getElementById("div_TCMDown").innerHTML = log_TCM;
					document.getElementById("div_TCMUp").innerHTML = log_TCM;
					document.getElementById("div_PathModeDown").innerHTML = log_PathModeDown;
					document.getElementById("div_IntDepthDown").innerHTML = log_IntDepthDown;
					document.getElementById("div_PathModeUp").innerHTML = log_PathModeUp;
					document.getElementById("div_IntDepthUp").innerHTML = log_IntDepthUp;
					document.getElementById("div_DataRateDown").innerHTML = log_DataRateDown;
					document.getElementById("div_DataRateUp").innerHTML = log_DataRateUp;
					document.getElementById("div_AttainDown").innerHTML = log_AttainDown;
					document.getElementById("div_AttainUp").innerHTML = log_AttainUp;
					document.getElementById("div_PowerDown").innerHTML = log_PowerDown;
					document.getElementById("div_PowerUp").innerHTML = log_PowerUp;
					document.getElementById("div_INPDown").innerHTML = log_INPDown;
					document.getElementById("div_INPUp").innerHTML = log_INPUp;
					document.getElementById("div_INPSHINEDown").innerHTML = log_INPSHINEDown;
					document.getElementById("div_INPSHINEUp").innerHTML = log_INPSHINEUp;
					document.getElementById("div_INPREINDown").innerHTML = log_INPREINDown;
					document.getElementById("div_INPREINUp").innerHTML = log_INPREINUp;
					document.getElementById("div_CRCDown").innerHTML = log_CRCDown;
					document.getElementById("div_CRCUp").innerHTML = log_CRCUp;
				}

				<%if tcWebApi_get("WebCustom_Entry","isMT7510","h") = "Yes" then%>
				display_vdsl_band_status();
				if(status_isVDSLmode == "VDSL")
				{
					//VDSL Band Status:SNR Margin
					document.getElementById("div_VDSLBAND_SNRMarginDown1").innerHTML = log_VDSLBAND_SNRMarginDown[0].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginDown2").innerHTML = log_VDSLBAND_SNRMarginDown[1].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginDown3").innerHTML = log_VDSLBAND_SNRMarginDown[2].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginDown4").innerHTML = log_VDSLBAND_SNRMarginDown[3].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginUp0").innerHTML = log_VDSLBAND_SNRMarginUp[0].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginUp1").innerHTML = log_VDSLBAND_SNRMarginUp[1].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginUp2").innerHTML = log_VDSLBAND_SNRMarginUp[2].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginUp3").innerHTML = log_VDSLBAND_SNRMarginUp[3].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SNRMarginUp4").innerHTML = log_VDSLBAND_SNRMarginUp[4].replace("N/A","-");

					//VDSL Band Status:Line Attenuation
					document.getElementById("div_VDSLBAND_LATNDown1").innerHTML = log_VDSLBAND_LATNDown[0].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNDown2").innerHTML = log_VDSLBAND_LATNDown[1].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNDown3").innerHTML = log_VDSLBAND_LATNDown[2].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNDown4").innerHTML = log_VDSLBAND_LATNDown[3].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNUp0").innerHTML = log_VDSLBAND_LATNUp[0].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNUp1").innerHTML = log_VDSLBAND_LATNUp[1].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNUp2").innerHTML = log_VDSLBAND_LATNUp[2].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNUp3").innerHTML = log_VDSLBAND_LATNUp[3].replace("N/A","-");
					document.getElementById("div_VDSLBAND_LATNUp4").innerHTML = log_VDSLBAND_LATNUp[4].replace("N/A","-");

					//VDSL Band Status:Signal Attenuation
					document.getElementById("div_VDSLBAND_SATNDown1").innerHTML = log_VDSLBAND_SATNDown[0].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNDown2").innerHTML = log_VDSLBAND_SATNDown[1].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNDown3").innerHTML = log_VDSLBAND_SATNDown[2].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNDown4").innerHTML = log_VDSLBAND_SATNDown[3].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNUp0").innerHTML = log_VDSLBAND_SATNUp[0].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNUp1").innerHTML = log_VDSLBAND_SATNUp[1].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNUp2").innerHTML = log_VDSLBAND_SATNUp[2].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNUp3").innerHTML = log_VDSLBAND_SATNUp[3].replace("N/A","-");
					document.getElementById("div_VDSLBAND_SATNUp4").innerHTML = log_VDSLBAND_SATNUp[4].replace("N/A","-");
				}
				<%end if%>

				setTimeout("update_log();", 5000);
			}
	});
}

var SystemVendorID_orig = "<%tcWebApi_get("SysInfo_Entry","SystemVendorID","s")%>";
var SystemVendorModelID_orig = "<%tcWebApi_get("SysInfo_Entry","SystemVendorModelID","s")%>";
var ModemVendorID_orig = "<%tcWebApi_get("SysInfo_Entry","ModemVendorID","s")%>";

function initial(){
	show_menu();
	load_body();
	display_line_stats();
	display_vdsl_band_status();
	showadslbootTime();
	document.getElementById("div_AdslType").innerHTML = dsl_type;

	document.getElementById("tr_SystemVendorID").style.display = (SystemVendorID_orig != "")? "":"none";
	document.getElementById("tr_SystemVendorModelID").style.display = (SystemVendorModelID_orig != "")? "":"none";
	document.getElementById("tr_ModemVendorID").style.display = (ModemVendorID_orig != "")? "":"none";

	setTimeout("update_log();", 5000);
}

function showadslbootTime(){
	if(adsl_timestamp_update != "" && sync_status_update == "up")
	{
		if(adsl_boottime < 0)
			adsl_boottime = boottime - adsl_timestamp_update;
		Days = Math.floor(adsl_boottime / (60*60*24));
		Hours = Math.floor((adsl_boottime / 3600) % 24);
		Minutes = Math.floor(adsl_boottime % 3600 / 60);
		Seconds = Math.floor(adsl_boottime % 60);

		document.getElementById("boot_days").innerHTML = Days;
		document.getElementById("boot_hours").innerHTML = Hours;
		document.getElementById("boot_minutes").innerHTML = Minutes;
		document.getElementById("boot_seconds").innerHTML = Seconds;
		adsl_boottime += 1;
		setTimeout("showadslbootTime()", 1000);
	}
	else
	{
		document.getElementById("boot_days").innerHTML = "0";
		document.getElementById("boot_hours").innerHTML = "0";
		document.getElementById("boot_minutes").innerHTML = "0";
		document.getElementById("boot_seconds").innerHTML = "0";
	}
}

</script>
</head>

<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="apply.cgi" target="hidden_frame">
<input type="hidden" name="current_page" value="Main_LogStatus_Content.asp">
<input type="hidden" name="next_page" value="Main_LogStatus_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
</form>
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
<table width="760px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3" class="FormTitle" id="FormTitle">
<tr>
<td bgcolor="#4D595D" colspan="3" valign="top">
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","System_Log","s")%> - <%tcWebApi_get("String_Entry","menu_dsl_log","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","DSL_Log_Desc","s")%></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable_log">
<thead>
<tr>
	<th colspan="2"><%tcWebApi_get("String_Entry","DSL_info","s")%></th>
</tr>
</thead>
<tr>
<th width="36%"><%tcWebApi_get("String_Entry","adsl_fw_ver_in","s")%></th>
<td colspan="2">
	<%If tcWebApi_get("Info_Adsl","fwVer","h") <> "" then tcWebApi_staticGet("Info_Adsl","fwVer","s") end if%>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%></th>
<td colspan="2">
	<div id="div_lineState"><%tcWebApi_staticGet("Info_Adsl","lineState","s")%></div>
</td>
</tr>
<tr>
<th>DSL <%tcWebApi_get("String_Entry","General_x_SystemUpTime_in","s")%>
<td colspan="2">
	<span id="boot_days"></span> <%tcWebApi_get("String_Entry","Day","s")%> <span id="boot_hours"></span> <%tcWebApi_get("String_Entry","Hour","s")%> <span id="boot_minutes"></span> <%tcWebApi_get("String_Entry","Minute","s")%> <span id="boot_seconds"></span> <%tcWebApi_get("String_Entry","Second","s")%>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","dslsetting_disc1","s")%></th>
<td colspan="2">
	<div id="div_Opmode"><% if tcWebApi_get("Info_Adsl","Opmode","h") <> "" then tcWebApi_get("Info_Adsl","Opmode","s") end if%></div>
</td>
</tr>
<tr>
<th id="th_AdslType"><%tcWebApi_get("String_Entry","dslsetting_disc2","s")%></th>
<td colspan="2">
	<div id="div_AdslType"></div>
</td>
</tr>
<tr>
<th>DSL Exchange(DSLAM)</th>
<td colspan="2">
	<div id="div_FarEndVendorID"><% if tcWebApi_get("Info_Adsl","FarEndVendorID","h") <> "" then tcWebApi_get("Info_Adsl","FarEndVendorID","s") end if%></div>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","DSL_profile","s")%></th>
<td colspan="2">
	<div id="div_VDSL_CurrentProfile"><% if tcWebApi_get("Info_Adsl","CurrentProfiles","h") <> "" then tcWebApi_get("Info_Adsl","CurrentProfiles","s") end if%></div>
</td>
</tr>

<tr id="tr_SystemVendorID" style="display:none;">
<th>System Vendor ID</th>
<td colspan="2">
	<div><%tcWebApi_get("SysInfo_Entry","SystemVendorID","s")%></div>
</td>
</tr>
<tr id="tr_SystemVendorModelID" style="display:none;">
<th>System Vendor Model ID</th>
<td colspan="2">
	<div><%tcWebApi_get("SysInfo_Entry","SystemVendorModelID","s")%></div>
</td>
</tr>
<tr id="tr_ModemVendorID" style="display:none;">
<th>Modem Vendor ID</th>
<td colspan="2">
	<div><%tcWebApi_get("SysInfo_Entry","ModemVendorID","s")%></div>
</td>
</tr>

</table>
</td>
</tr>

<!-- Line Stats - Begin -->
<tr id="line_stats">
<td bgcolor="#4D595D" colspan="3" valign="top">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable_log">
<thead>
<tr>
	<th colspan="3">Line Stats</th>
</tr>
<tr>
<td width="36%" style="background-color:#2F3A3E"></td>
<td width="32%">Downstream</td>
<td width="32%">Upstream</td>
</tr>
</thead>
<tr>
	<!-- Accroding to Spec., TCM will be the same status on upstream/downstream. So we show it on both direction. -->
<th>TCM(Trellis Coded Modulation)</th>
<td>
	<div id="div_TCMDown"><% if tcWebApi_get("Info_Adsl","TCM","h") <> "" then tcWebApi_get("Info_Adsl","TCM","s") end if%></div>
</td>
<td>
	<div id="div_TCMUp"><% if tcWebApi_get("Info_Adsl","TCM","h") <> "" then tcWebApi_get("Info_Adsl","TCM","s") end if%></div>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","DSL_SNR_Margin","s")%></th>
<td>
	<div id="div_SNRMarginDown"><% if tcWebApi_get("Info_Adsl","SNRMarginDown","h") <> "" then tcWebApi_get("Info_Adsl","SNRMarginDown","s") end if%></div>
</td>
<td>
	<div id="div_SNRMarginUp"><% if tcWebApi_get("Info_Adsl","SNRMarginUp","h") <> "" then tcWebApi_get("Info_Adsl","SNRMarginUp","s") end if%></div>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","DSL_Line_Attenuation","s")%></th>
<td>
	<div id="div_AttenDown"><% if tcWebApi_get("Info_Adsl","AttenDown","h") <> "" then tcWebApi_get("Info_Adsl","AttenDown","s") end if%></div>
</td>
<td>
	<div id="div_AttenUp"><% if tcWebApi_get("Info_Adsl","AttenUp","h") <> "" then tcWebApi_get("Info_Adsl","AttenUp","s") end if%></div>
</td>
</tr>
<tr>
<th>Path Mode</th>
<td>
	<div id="div_PathModeDown"><% if tcWebApi_get("Info_Adsl","PathModeDown","h") <> "" then tcWebApi_get("Info_Adsl","PathModeDown","s") end if%></div>
</td>
<td>
	<div id="div_PathModeUp"><% if tcWebApi_get("Info_Adsl","PathModeUp","h") <> "" then tcWebApi_get("Info_Adsl","PathModeUp","s") end if%></div>
</td>
</tr>
<tr>
<th>Interleave Depth</th>
<td>
	<div id="div_IntDepthDown"><% if tcWebApi_get("Info_Adsl","InterleaveDepthDown","h") <> "" then tcWebApi_get("Info_Adsl","InterleaveDepthDown","s") end if%></div>
</td>
<td>
	<div id="div_IntDepthUp"><% if tcWebApi_get("Info_Adsl","InterleaveDepthUp","h") <> "" then tcWebApi_get("Info_Adsl","InterleaveDepthUp","s") end if%></div>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","DSL_Data_Rate","s")%></th>
<td>
	<div id="div_DataRateDown"><% if tcWebApi_get("Info_Adsl","DataRateDown","h") <> "" then tcWebApi_get("Info_Adsl","DataRateDown","s") end if%></div>
</td>
<td>
	<div id="div_DataRateUp"><% if tcWebApi_get("Info_Adsl","DataRateUp","h") <> "" then tcWebApi_get("Info_Adsl","DataRateUp","s") end if%></div>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","DSL_Data_Rate_Max","s")%></th>
<td>
	<div id="div_AttainDown"><% if tcWebApi_get("Info_Adsl","AttainDown","h") <> "" then tcWebApi_get("Info_Adsl","AttainDown","s") end if%></div>
</td>
<td>
	<div id="div_AttainUp"><% if tcWebApi_get("Info_Adsl","AttainUp","h") <> "" then tcWebApi_get("Info_Adsl","AttainUp","s") end if%></div>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","DSL_Power","s")%></th>
<td>
	<div id="div_PowerDown"><% if tcWebApi_get("Info_Adsl","PowerDown","h") <> "" then tcWebApi_get("Info_Adsl","PowerDown","s") end if%></div>
</td>
<td>
	<div id="div_PowerUp"><% if tcWebApi_get("Info_Adsl","PowerUp","h") <> "" then tcWebApi_get("Info_Adsl","PowerUp","s") end if%></div>
</td>
</tr>
<tr>
<th>INP</th>
<td>
	<div id="div_INPDown"><% if tcWebApi_get("Info_Adsl","INPDown","h") <> "" then tcWebApi_get("Info_Adsl","INPDown","s") end if%></div>
</td>
<td>
	<div id="div_INPUp"><% if tcWebApi_get("Info_Adsl","INPUp","h") <> "" then tcWebApi_get("Info_Adsl","INPUp","s") end if%></div>
</td>
</tr>
<tr>
<th>INP-SHINE</th>
<td>
	<div id="div_INPSHINEDown"><% if tcWebApi_get("Info_Adsl","INPSHINEDown","h") <> "" then tcWebApi_get("Info_Adsl","INPSHINEDown","s") end if%></div>
</td>
<td>
	<div id="div_INPSHINEUp"><% if tcWebApi_get("Info_Adsl","INPSHINEUp","h") <> "" then tcWebApi_get("Info_Adsl","INPSHINEUp","s") end if%></div>
</td>
</tr>
<tr>
<th>INP-REIN</th>
<td>
	<div id="div_INPREINDown"><% if tcWebApi_get("Info_Adsl","INPREINDown","h") <> "" then tcWebApi_get("Info_Adsl","INPREINDown","s") end if%></div>
</td>
<td>
	<div id="div_INPREINUp"><% if tcWebApi_get("Info_Adsl","INPREINUp","h") <> "" then tcWebApi_get("Info_Adsl","INPREINUp","s") end if%></div>
</td>
</tr>
<tr>
<th>CRC</th>
<td>
	<div id="div_CRCDown"><% if tcWebApi_get("Info_Adsl","CRCDown","h") <> "" then tcWebApi_get("Info_Adsl","CRCDown","s") end if%></div>
</td>
<td>
	<div id="div_CRCUp"><% if tcWebApi_get("Info_Adsl","CRCUp","h") <> "" then tcWebApi_get("Info_Adsl","CRCUp","s") end if%></div>
</td>
</tr>
</table>
</td>
</tr>
<!-- Line Stats - End -->

<!-- VDSL Band Status - Begin -->
<%if tcWebApi_get("WebCustom_Entry","isMT7510","h") = "Yes" then%>
<tr id="vdsl_band_status">
<td bgcolor="#4D595D" colspan="3" valign="top">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable_log">
<thead>
<tr>
	<th colspan="10"><%tcWebApi_get("String_Entry","DSL_VDSL_status","s")%></th>
</tr>

<tr>
<td width="36%" style="background-color:#2F3A3E"></td>
<td width="7%">U0</td>
<td width="7%">D1</td>
<td width="7%">U1</td>
<td width="7%">D2</td>
<td width="7%">U2</td>
<td width="7%">D3</td>
<td width="7%">U3</td>
<td width="7%">D4</td>
<td width="8%">U4</td>
</tr>
</thead>
<tr>
<th><%tcWebApi_get("String_Entry","DSL_Line_Attenuation","s")%> (dB)</th>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_LATNUp0"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_LATNDown1"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_LATNUp1"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_LATNDown2"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_LATNUp2"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_LATNDown3"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_LATNUp3"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_LATNDown4"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_LATNUp4"></div>
</td>
</tr>

<tr>
<th><%tcWebApi_get("String_Entry","DSL_Signal_Attenuation","s")%> (dB)</th>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SATNUp0"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SATNDown1"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SATNUp1"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SATNDown2"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SATNUp2"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SATNDown3"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SATNUp3"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SATNDown4"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SATNUp4"></div>
</td>
</tr>

<tr>
<th><%tcWebApi_get("String_Entry","DSL_SNR_Margin","s")%> (dB)</th>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SNRMarginUp0"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SNRMarginDown1"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SNRMarginUp1"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SNRMarginDown2"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SNRMarginUp2"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SNRMarginDown3"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SNRMarginUp3"></div>
</td>
<td style="color:#FF9000;">
	<div id="div_VDSLBAND_SNRMarginDown4"></div>
</td>
<td style="color:#3CF;">
	<div id="div_VDSLBAND_SNRMarginUp4"></div>
</td>
</tr>
</table>
</td>
</tr>
<%end if%>
<!-- VDSL Band Status - End -->

<tr class="apply_gen" valign="top">
<td width="100%" align="center" >
<form method="post" name="form3" action="apply.cgi">
<input type="hidden" name="current_page" value="Main_LogStatus_Content.asp">
<input type="hidden" name="action_mode" value=" Refresh ">
<input type="hidden" name="next_host" value="">
<input type="button" onClick="location.href=location.href" value="<%tcWebApi_get("String_Entry","CTL_refresh","s")%>" class="button_gen">
</form>
</td>
</tr>
</table>
</td>
</tr>
</table>
</td>
<td width="10" align="center" valign="top"></td>
</tr>
</table>
<div id="footer"></div>
</form>
</body>

<!--Main_AdslStatus_Content.asp-->
</html>

