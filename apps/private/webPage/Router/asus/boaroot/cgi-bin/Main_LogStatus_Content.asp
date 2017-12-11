<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--Main_LogStatus_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_7_2","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
 
function showclock(){
	JS_timeObj.setTime(systime_millsec);
	systime_millsec += 1000;
	JS_timeObj2 = JS_timeObj.toString();
	JS_timeObj2 = JS_timeObj2.substring(0,3) + ", " +
					JS_timeObj2.substring(4,10) + " " +
					checkTime(JS_timeObj.getHours()) + ":" +
					checkTime(JS_timeObj.getMinutes()) + ":" +
					checkTime(JS_timeObj.getSeconds()) + " " +
					JS_timeObj.getFullYear();

	var uptimeStr_new = document.getElementById('time1').value;

	document.getElementById("system_time").value = JS_timeObj2;

	setTimeout("showclock()", 1000);
	corrected_timezone(DAYLIGHT_orig, TZ_orig);	
}

function showbootTime(){
	Days = Math.floor(boottime / (60*60*24));
	Hours = Math.floor((boottime / 3600) % 24);
	Minutes = Math.floor(boottime % 3600 / 60);
	Seconds = Math.floor(boottime % 60);
	document.getElementById("boot_days").innerHTML = Days;
	document.getElementById("boot_hours").innerHTML = Hours;
	document.getElementById("boot_minutes").innerHTML = Minutes;
	document.getElementById("boot_seconds").innerHTML = Seconds;
	boottime += 1;
	setTimeout("showbootTime()", 1000);
}

function back_syslog(){
	var cfg = '/syslog.xlog';
	var code = 'location.assign(\"' + cfg + '\")';
	eval(code);
}

function clearLog(){
	document.clearform.clearLog_Flag.value = "1";
	document.clearform.submit();
}
</script>
</head>
<body onload="show_menu();load_body();showclock(); showbootTime();" onunLoad="return unload_body();">
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
		  			<div class="formfonttitle"><%tcWebApi_get("String_Entry","System_Log","s")%> - <%tcWebApi_get("String_Entry","menu5_7_2","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  			<div class="formfontdesc"><%tcWebApi_get("String_Entry","GeneralLog_title","s")%></div>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
								<th width="20%"><%tcWebApi_get("String_Entry","General_x_SystemTime_in","s")%></th>
<td>
<input type="text" id="system_time" name="system_time" size="40" class="devicepin" value="" readonly="1" style="font-size:12px;">
<div><span id="timezone_hint" onclick="location.href='Advanced_System_Content.asp?af=time_zone_select'" style="color:#FFCC00;text-decoration:underline;cursor:pointer;display:none;"></span></div>
</form>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","General_x_SystemUpTime_in","s")%></a></th>
<td><span id="boot_days"></span> <%tcWebApi_get("String_Entry","Day","s")%> <span id="boot_hours"></span> <%tcWebApi_get("String_Entry","Hour","s")%> <span id="boot_minutes"></span> <%tcWebApi_get("String_Entry","Minute","s")%> <span id="boot_seconds"></span> <%tcWebApi_get("String_Entry","Second","s")%></td>
</tr>
</table>
<div style="margin-top:8px">
<iframe src="/cgi-bin/status_log2.cgi" width="100%" height="470" marginwidth="5" #marginheight="0" scrolling="no" frameborder="0" align="center"></iframe>
</div>
			<div style="margin-top:-10px;">
				<table class="apply_gen" >
				<tr class="apply_gen" valign="top">
					<td width="40%" align="right" >
								<input type="button" onClick="clearLog();" value="<%tcWebApi_get("String_Entry","CTL_clear","s")%>" class="button_gen">
					</td>
					<td width="20%" align="center">
								<input type="button" onClick="back_syslog();" value="<%tcWebApi_get("String_Entry","CTL_onlysave","s")%>" class="button_gen">
					</td>
					<td width="40%" align="left" >
								<input type="button" onClick="location.href=location.href" value="<%tcWebApi_get("String_Entry","CTL_refresh","s")%>" class="button_gen">
					</td>
				</tr>
				</table>
			</div>
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
<div id="timearea" style="display:none;">
<textarea id="time1"><%TCWebApi_get("DeviceInfo","cur_time","s")%></textarea>
</div>
</form>

<form method="post" name="clearform" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Main_LogStatus_Content.asp">	
<input type="hidden" name="clearLog_Flag" value="">	
</form>	
</body>

<!--Main_LogStatus_Content.asp-->
</html>

