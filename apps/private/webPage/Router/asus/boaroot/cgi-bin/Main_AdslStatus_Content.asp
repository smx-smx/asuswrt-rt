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
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var sync_status = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
var adsl_timestamp = parseInt("<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>");
var sync_status_update = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
var adsl_timestamp_update = parseInt("<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>");
var adsl_boottime = boottime - adsl_timestamp;
var dsl_type = "<% tcWebApi_get("Info_Adsl","AdslType","s") %>".replace("_", " ");
var log_lineState;
var log_Opmode;
var log_AdslType;
var log_SNRMarginDown;
var log_SNRMarginUp;
var log_AttenDown;
var log_AttenUp;
var log_WanListMode;
var log_DataRateDown;
var log_DataRateUp;
var log_AttainDown;
var log_AttainUp;
var log_PowerDown;
var log_PowerUp;
var log_CRCDown;
var log_CRCUp;

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
				document.getElementById("div_lineState").innerHTML = log_lineState;
				document.getElementById("div_Opmode").innerHTML = log_Opmode;
				document.getElementById("div_AdslType").innerHTML = log_AdslType;
				document.getElementById("div_SNRMarginDown").innerHTML = log_SNRMarginDown;
				document.getElementById("div_SNRMarginUp").innerHTML = log_SNRMarginUp;
				document.getElementById("div_AttenDown").innerHTML = log_AttenDown;
				document.getElementById("div_AttenUp").innerHTML = log_AttenUp;
				if(log_WanListMode=="0")
					document.getElementById("div_WanListMode").innerHTML = "Interleaved";
				else if(log_WanListMode=="1")
					document.getElementById("div_WanListMode").innerHTML = "FastPath";
				else
					document.getElementById("div_WanListMode").innerHTML = "";
				document.getElementById("div_DataRateDown").innerHTML = log_DataRateDown;
				document.getElementById("div_DataRateUp").innerHTML = log_DataRateUp;
				document.getElementById("div_AttainDown").innerHTML = log_AttainDown;
				document.getElementById("div_AttainUp").innerHTML = log_AttainUp;
				document.getElementById("div_PowerDown").innerHTML = log_PowerDown;
				document.getElementById("div_PowerUp").innerHTML = log_PowerUp;
				document.getElementById("div_CRCDown").innerHTML = log_CRCDown;
				document.getElementById("div_CRCUp").innerHTML = log_CRCUp;
					
				setTimeout("update_log();", 5000);
			}	
	});		
}

function initial(){

	show_menu();
	load_body();
	showadslbootTime();
	document.getElementById("div_AdslType").innerHTML = dsl_type;
	setTimeout("update_log();", 5000);
} 

function showadslbootTime(){
	if(adsl_timestamp_update != "" && sync_status_update == "up")
	{
		if(adsl_boottime < 0)
			dsl_boottime = boottime - adsl_timestamp_update;
		Days = Math.floor(adsl_boottime / (60*60*24));
		Hours = Math.floor((adsl_boottime / 3600) % 24);
		Minutes = Math.floor(adsl_boottime % 3600 / 60);
		Seconds = Math.floor(adsl_boottime % 60);

		$("boot_days").innerHTML = Days;
		$("boot_hours").innerHTML = Hours;
		$("boot_minutes").innerHTML = Minutes;
		$("boot_seconds").innerHTML = Seconds;
		adsl_boottime += 1;
		setTimeout("showadslbootTime()", 1000);
	}
	else
	{
		$("boot_days").innerHTML = "0";
		$("boot_hours").innerHTML = "0";
		$("boot_minutes").innerHTML = "0";
		$("boot_seconds").innerHTML = "0";
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
<div class="formfontdesc"><% tcWebApi_Get("String_Entry", "GeneralLog_title", "s") %></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","adsl_fw_ver_in","s")%></th>
<td>
	<%If tcWebApi_get("Info_Adsl","fwVer","h") <> "" then tcWebApi_staticGet("Info_Adsl","fwVer","s") end if%>
</td>
</tr>
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%></th>
<td>
	<div id="div_lineState"><%tcWebApi_staticGet("Info_Adsl","lineState","s")%></div>
</td>
</tr>
<tr>
<th width="20%">DSL <%tcWebApi_get("String_Entry","General_x_SystemUpTime_in","s")%>
<td>
	<span id="boot_days"></span> <%tcWebApi_get("String_Entry","Day","s")%> <span id="boot_hours"></span> <%tcWebApi_get("String_Entry","Hour","s")%> <span id="boot_minutes"></span> <%tcWebApi_get("String_Entry","Minute","s")%> <span id="boot_seconds"></span> <%tcWebApi_get("String_Entry","Second","s")%>
</td>
</tr>
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","dslsetting_disc1","s")%></th>
<td>
	<div id="div_Opmode"><% if tcWebApi_get("Info_Adsl","Opmode","h") <> "" then tcWebApi_get("Info_Adsl","Opmode","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","dslsetting_disc2","s")%></th>
<td>
	<div id="div_AdslType"></div>
</td>
</tr>
<tr>
<th width="20%">SNR Down</th>
<td>
	<div id="div_SNRMarginDown"><% if tcWebApi_get("Info_Adsl","SNRMarginDown","h") <> "" then tcWebApi_get("Info_Adsl","SNRMarginDown","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">SNR Up</th>
<td>
	<div id="div_SNRMarginUp"><% if tcWebApi_get("Info_Adsl","SNRMarginUp","h") <> "" then tcWebApi_get("Info_Adsl","SNRMarginUp","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">Line Attenuation Down</th>
<td>
	<div id="div_AttenDown"><% if tcWebApi_get("Info_Adsl","AttenDown","h") <> "" then tcWebApi_get("Info_Adsl","AttenDown","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">Line Attenuation Up</th>
<td>
	<div id="div_AttenUp"><% if tcWebApi_get("Info_Adsl","AttenUp","h") <> "" then tcWebApi_get("Info_Adsl","AttenUp","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">Path Mode</th>
<td>
	<div id="div_WanListMode">
		<% if tcWebApi_get("Info_Adsl","WanListMode","h") = "0" then
			asp_Write("Interleaved")
		elseif tcWebApi_get("Info_Adsl","WanListMode","h") = "1" then
			asp_Write("FastPath")
		else asp_Write("")
		end if %>
	</div>
</td>
</tr>
<tr>
<th width="20%">Data Rate Down</th>
<td>
	<div id="div_DataRateDown"><% if tcWebApi_get("Info_Adsl","DataRateDown","h") <> "" then tcWebApi_get("Info_Adsl","DataRateDown","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">Data Rate Up</th>
<td>
	<div id="div_DataRateUp"><% if tcWebApi_get("Info_Adsl","DataRateUp","h") <> "" then tcWebApi_get("Info_Adsl","DataRateUp","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">MAX Rate Down</th>
<td>
	<div id="div_AttainDown"><% if tcWebApi_get("Info_Adsl","AttainDown","h") <> "" then tcWebApi_get("Info_Adsl","AttainDown","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">MAX Rate Up</th>
<td>
	<div id="div_AttainUp"><% if tcWebApi_get("Info_Adsl","AttainUp","h") <> "" then tcWebApi_get("Info_Adsl","AttainUp","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">POWER Down</th>
<td>
	<div id="div_PowerDown"><% if tcWebApi_get("Info_Adsl","PowerDown","h") <> "" then tcWebApi_get("Info_Adsl","PowerDown","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">POWER Up</th>
<td>
	<div id="div_PowerUp"><% if tcWebApi_get("Info_Adsl","PowerUp","h") <> "" then tcWebApi_get("Info_Adsl","PowerUp","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">CRC Down</th>
<td>
	<div id="div_CRCDown"><% if tcWebApi_get("Info_Adsl","CRCDown","h") <> "" then tcWebApi_get("Info_Adsl","CRCDown","s") end if%></div>
</td>
</tr>
<tr>
<th width="20%">CRC Up</th>
<td>
	<div id="div_CRCUp"><% if tcWebApi_get("Info_Adsl","CRCUp","h") <> "" then tcWebApi_get("Info_Adsl","CRCUp","s") end if%></div>
</td>
</tr>
</table>
</td>
</tr>
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

