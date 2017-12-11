<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_OperationMode_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_6_1_title","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" language="JavaScript" src="/help.js"></script>
<script type="text/javascript" language="JavaScript" src="/detect.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
function initial(){
show_menu();
setScenerion(sw_mode);
Senario_shift();
if(repeater_support < 0)
$("repeaterMode").style.display = "none";
}
function Senario_shift(){
var isIE = navigator.userAgent.search("MSIE") > -1;
if(isIE)
$("Senario").style.width = "700px";
$("Senario").style.marginLeft = "5px";
}
function saveMode(){
if('1' == document.form.sw_mode.value){
alert("ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> <%tcWebApi_get("String_Entry","op_already_configured","s")%>");
return false;
}
if(document.form.sw_mode.value == 3){
parent.location.href = 'QIS_wizard.asp?flag=lanip';
return false;
}
else if(document.form.sw_mode.value == 2){
parent.location.href = 'http://router.asus.com/QIS_wizard.asp?flag=sitesurvey';
return false;
}
else{ // default router
document.form.lan_proto.value = 'static';
document.form.lan_ipaddr.value = document.form.lan_ipaddr_rt.value;
document.form.lan_netmask.value = document.form.lan_netmask_rt.value;
document.form.lan_gateway.value = document.form.lan_ipaddr_rt.value;
if('1' == '2'){
close_guest_unit(0,1);
if(wl_info.band5g_support)
close_guest_unit(1,1);
}
}
showLoading();
document.form.target="hidden_frame";
document.form.submit();
}
function close_guest_unit(_unit, _subunit){
var NewInput = document.createElement("input");
NewInput.type = "hidden";
NewInput.name = "wl"+ _unit + "." + _subunit +"_bss_enabled";
NewInput.value = "0";
document.form.appendChild(NewInput);
}
function done_validating(action){
refreshpage();
}
var $j = jQuery.noConflict();
var id_WANunplungHint;
function setScenerion(mode){
if(mode == '1'){
document.form.sw_mode.value = 1;
$j("#Senario").css("background","url(/images/New_ui/rt.jpg) center");
$j("#radio2").hide();
$j("#Internet_span").hide();
$j("#ap-line").css("display", "none");
		$j("#AP").html("<%tcWebApi_get("String_Entry","Internet","s")%>");
		$j("#mode_desc").html("<%tcWebApi_get("String_Entry","OP_GW_desc","s")%>");
		$j("#nextButton").attr("value","<%tcWebApi_get("String_Entry","CTL_next","s")%>");
}
else if(mode == '2'){
document.form.sw_mode.value = 2;
$j("#Senario").css("background", "url(/images/New_ui/re.jpg) center");
$j("#radio2").css("display", "none");
$j("#Internet_span").css("display", "block");
$j("#ap-line").css("display", "none");
		$j("#AP").html("<%tcWebApi_get("String_Entry","Device_type_02_RT","s")%>");
		$j("#mode_desc").html("<%tcWebApi_get("String_Entry","OP_RE_desc","s")%>");
		$j("#nextButton").attr("value","<%tcWebApi_get("String_Entry","CTL_next","s")%>");
clearTimeout(id_WANunplungHint);
$j("#Unplug-hint").css("display", "none");
}
else if(mode == '3'){
document.form.sw_mode.value = 3;
$j("#Senario").css("background", "url(/images/New_ui/ap.jpg) center");
$j("#radio2").css("display", "none");
$j("#Internet_span").css("display", "block");
$j("#ap-line").css("display", "none");
		$j("#AP").html("<%tcWebApi_get("String_Entry","Device_type_02_RT","s")%>");
		$j("#mode_desc").html("<%tcWebApi_get("String_Entry","OP_AP_desc","s")%>");
		$j("#nextButton").attr("value","<%tcWebApi_get("String_Entry","CTL_next","s")%>");
clearTimeout(id_WANunplungHint);
$j("#Unplug-hint").css("display", "none");
}
else{
document.form.sw_mode.value = 1;
$j("#Senario").css("background","url(/images/New_ui/gw.png) center");
$j("#radio2").hide();
$j("#Internet_span").hide();
$j("#ap-line").css("display", "none");
		$j("#AP").html("<%tcWebApi_get("String_Entry","Internet","s")%>");
		$j("#mode_desc").html("<%tcWebApi_get("String_Entry","OP_GW_desc","s")%>");
		$j("#nextButton").attr("value","<%tcWebApi_get("String_Entry","CTL_next","s")%>");
}
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="hiddenMask" class="popup_bg">
<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
<tr>
<td>
<div class="drword" id="drword" style="height:110px;"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
<br/>
<br/>
</div>
<div class="drImg"><img src="/images/alertImg.png"></div>
<div style="height:70px;"></div>
</td>
</tr>
</table>
</div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_all">
<input type="hidden" name="action_wait" value="60">
<input type="hidden" name="prev_page" value="Advanced_OperationMode_Content.asp">
<input type="hidden" name="current_page" value="Advanced_OperationMode_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="flag" value="">
<input type="hidden" name="sw_mode" value="1">
<input type="hidden" name="lan_proto" value="static">
<input type="hidden" name="lan_ipaddr" value="192.168.1.1">
<input type="hidden" name="lan_netmask" value="255.255.255.0">
<input type="hidden" name="lan_gateway" value="0.0.0.0">
<input type="hidden" name="lan_ipaddr_rt" value="192.168.1.1">
<input type="hidden" name="lan_netmask_rt" value="255.255.255.0">
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
<td valign="top" >
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tr bgcolor="#4D595D" valign="top" height="100px">
<td>
<div>&nbsp;</div>
	  		<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6_adv","s")%> - <%tcWebApi_get("String_Entry","menu5_6_1","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  	<div class="formfontdesc"><%tcWebApi_get("String_Entry","OP_desc1","s")%></div>

</td>
</tr>
<tr>
<td bgcolor="#4D595D" valign="top" height="500px">
<div style="width:95%; margin:0 auto; padding-bottom:3px;">
<span style="font-size:16px; font-weight:bold;color:white;text-shadow:1px 1px 1px black">
<input type="radio" name="sw_mode_radio" class="input" value="1" onclick="setScenerion(1);" checked><%tcWebApi_get("String_Entry","OP_GW_item","s")%>
&nbsp;&nbsp;
<span id="repeaterMode"><input type="radio" name="sw_mode_radio" class="input" value="2" onclick="setScenerion(2);" ><%tcWebApi_get("String_Entry","OP_RE_item","s")%></span>
&nbsp;&nbsp;
<input type="radio" name="sw_mode_radio" class="input" value="3" onclick="setScenerion(3);" ><%tcWebApi_get("String_Entry","OP_AP_item","s")%>
</span>
<div id="mode_desc" style="position:relative;display:block;margin-top:10px;margin-left:5px;height:60px;z-index:90;">
		<%tcWebApi_get("String_Entry","OP_GW_desc","s")%>
</div>
<br/><br/>
<div id="Senario" style="margin-top:40px; margin-bottom:60px;">
<!--div id="ap-line" style="border:0px solid #333; width:133px; height:41px; position:absolute; background:url(/images/wanlink.gif) no-repeat;"></div-->
		<div id="Unplug-hint" style="border:2px solid red; background-color:#FFF; padding:3px;margin:0px 0px 0px 150px;width:250px; position:absolute; display:block; display:none;"><%tcWebApi_get("String_Entry","w_r_suggestion1","s")%></div>
</div>
</div>
<div class="apply_gen">
<input name="button" type="button" class="button_gen" onClick="saveMode();" value="Save">
</div>
</td>
</tr>
</table>
</td>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
</form>
<form name="hint_form"></form>
<div id="footer"></div>
</body>

<!--Advanced_OperationMode_Content.asp-->
</html>

