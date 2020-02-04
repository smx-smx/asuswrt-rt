<%
If Request_Form("editFlag") = "1" then
do_HomeProtection()
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<!--AiProtection_HomeProtection.asp-->

<head>
    <meta name="generator" content="HTML Tidy for Linux/x86 (vers 25 March 2009), see www.w3.org">
    <meta http-equiv="X-UA-Compatible" content="IE=Edge">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Expires" content="-1">

    <title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","AiProtection_title","s")%></title>
    <link rel="shortcut icon" href="/images/favicon.png">
    <link rel="icon" href="/images/favicon.png">
    <link rel="stylesheet" type="text/css" href="/index_style.css"> 
    <link rel="stylesheet" type="text/css" href="/form_style.css">
    <script type="text/javascript" src="/state.js"></script>
    <script type="text/javascript" src="/popup.js"></script>
    <script type="text/javascript" src="/general.js"></script>
    <script type="text/javascript" src="/help.js"></script>
    <script type="text/javascript" src="/js/jquery.js"></script>
    <script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
    <script type="text/javascript" src="/form.js"></script>
	<script type="text/javascript" src="/js/asus_eula.js"></script>
<style>
.weakness{
	width:650px;
	height:590px;
	position:absolute;
	background: rgba(0,0,0,0.8);
	z-index:10;
	margin-left:300px;
	border-radius:10px;
	display: none;
}

.weakness_router_status{
	text-align:center;
	font-size:18px;
	padding:10px;
	font-weight:bold;
}

.weakness_status{
	width:90%;
	background-color:#BEBEBE;
	color:#000;border-collapse:separate !important;
	border-radius:10px;

}

.weakness_status th{
	text-align:left;
	border-bottom:1px solid #4D595D;
	padding-left:15px;
	font-size:14px;
}

.weakness_status td{
	border-bottom:1px solid #4D595D;
	width:100px;
}

.weakness_status td>div{
	/*background-color:#FF7575;*/ /*#1CFE16 for Yes button*/
	border-radius:10px;
	text-align:center;
	padding:3px 0px;
	width:100px;

}

.status_no{
	background-color:#FF7575;
}
.status_no a{
	text-decoration:underline;}
.status_yes{
	background-color:#1CFE16;
}

.alertpreference{
	width:650px;
	height:290px;
	position:absolute;
	background: rgba(0,0,0,0.8);
	z-index:10;
	margin-left:260px;
	border-radius:10px;
	padding:15px 10px 20px 10px;
	display: none;
}
</style>
<script>
var $j = jQuery.noConflict();
var button_flag = 0;

window.onresize = function() {
	if(document.getElementById("weakness_div").style.display == "block") {
		cal_panel_block("weakness_div", 0.25);
	}
//	if(document.getElementById("alert_preference").style.display == "block") {
//		cal_panel_block("alert_preference", 0.25);
//	}
} 

function initial(){
	show_menu();
	setTimeout("update_FAQ();", 300);
	$j("#all_security_btn").hide();
}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq", "https://www.asus.com", "/support/FAQ/", "1008719");
	}
}
function applyRule(){
	document.form.editFlag.value = "1";
	showLoading();
	setTimeout("redirect();", 3000);
	document.form.submit();
}

function redirect(){
	document.location.href = "/cgi-bin/AiProtection_HomeProtection.asp";
}

function check_weakness(){
	cal_panel_block("weakness_div", 0.25);
	$j('#weakness_div').fadeIn();
	check_login_name_password();
	check_wireless_password();
	check_wireless_encryption();
	check_WPS();
	check_upnp();
	check_wan_access();
	check_ping_form_wan();
	check_dmz();
	check_port_trigger();
	check_port_forwarding();
	check_ftp_anonymous();
	check_samba_anonymous();
	check_TM_feature();
}

function close_weakness_status(){
	$j('#weakness_div').fadeOut(100);
}

function enable_whole_security(){
	if(!confirm("<%tcWebApi_get("String_Entry","AiProtection_Vulnerability_confirm","s")%>")){
		return false;
	}

	var wps_enable_2g = document.form.wps_enable_2g.value;
	var wps_enable_5g = document.form.wps_enable_5g.value;
	var wan_upnp_enable = document.form.wan_upnp_enable.value;
	var wan_access_enable = document.form.misc_http_x.value;
	var wan_ping_enable = document.form.misc_ping_x.value;
	var port_trigger_enable = document.form.autofw_enable_x.value;
	var port_forwarding_enable = document.form.vts_enable_x.value;
	var ftp_account_mode = document.form.st_ftp_mode.value;
	var samba_account_mode = document.form.st_samba_mode.value;
	var wrs_cc_enable = document.form.wrs_cc_enable.value;
	var wrs_vp_enable = document.form.wrs_vp_enable.value;
	var wrs_mals_enable = document.form.wrs_mals_enable.value;
	var wait_time = 3;

	if (wps_enable_2g == 1 || wps_enable_5g == 1)
	{
		document.form.wps_enable_2g.value = 0;
		document.form.wps_enable_5g.value = 0;
		document.form.wps_enable_2g.disabled = false;
		document.form.wps_enable_5g.disabled = false;
		wait_time += 10;
		document.form.editFlag.value = "1";
	}

	if(wan_upnp_enable == "Yes"){		
		document.form.wan_upnp_enable.value = "No";
		document.form.wan_upnp_enable.disabled = false;
		document.form.editFlag.value = "1";
		wait_time += 1;
	}

	if(wan_access_enable == 1){
		document.form.misc_http_x.value = 0;
		document.form.misc_http_x.disabled = false;
		document.form.editFlag.value = "1";
		wait_time += 1;
	}

	if(wan_ping_enable == 1){
		document.form.misc_ping_x.value = 0;
		document.form.misc_ping_x.disabled = false;
		document.form.editFlag.value = "1";
		wait_time += 1;
	}
	
	if(document.form.dmz_ip.value != ""){
		document.form.dmz_ip.value = "";
		document.form.dmz_ip.disabled = false;
		document.form.editFlag.value = "1";
	}

	if(port_trigger_enable == 1){
		document.form.autofw_enable_x.value = 0;
		document.form.autofw_enable_x.disabled = false;
		document.form.editFlag.value = "1";
	}
	
	if(port_forwarding_enable == 1){
		document.form.vts_enable_x.value = 0;
		document.form.vts_enable_x.disabled = false;
		document.form.editFlag.value = "1";
	}
	
	if(ftp_account_mode == 1){	
		document.form.st_ftp_mode.value = 2;
		document.form.st_ftp_force_mode.value = 2;
		document.form.st_ftp_mode.disabled = false;
		document.form.st_ftp_force_mode.disabled = false;
		document.form.editFlag.value = "1";
	}
	
	if(samba_account_mode == 1){
		document.form.st_samba_mode.value = 4;
		document.form.st_samba_force_mode.value = 4;
		document.form.st_samba_mode.disabled = false;
		document.form.st_samba_force_mode.disabled = false;
		document.form.editFlag.value = "1";
	}

	if(wrs_cc_enable == 0 || wrs_vp_enable == 0 || wrs_mals_enable == 0){
		document.form.wrs_cc_enable.value = 1;
		document.form.wrs_vp_enable.value = 1;
		document.form.wrs_mals_enable.value = 1;
		document.form.editFlag.value = "1";
		wait_time += 5;
	}

	showLoading();
	setTimeout("redirect();", wait_time*1000);
	document.form.submit();
}

function check_login_name_password(){

	if(<%check_acorpw()%> == 1){
		document.getElementById('login_password').innerHTML = "<a href='Advanced_System_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('login_password').className = "status_no";	
		document.getElementById('login_password').onmouseover = function(){overHint(10);}
		document.getElementById('login_password').onmouseout = function(){nd();}
	}
	else{
		document.getElementById('login_password').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('login_password').className = "status_yes";
	}
}

function check_wireless_password(){
	var nScore = <%check_passwd_strength("wl_key")%>;
	var oScore = document.getElementById("score");
	if (nScore >= 0 && nScore < 20) { sComplexity = "<%tcWebApi_get("String_Entry","PASS_score0","s")%>"; }
	else if (nScore >= 20 && nScore < 40) { sComplexity = "<%tcWebApi_get("String_Entry","PASS_score1","s")%>"; }
	else if (nScore >= 40 && nScore < 60) { sComplexity = "<%tcWebApi_get("String_Entry","PASS_score2","s")%>"; }
	else if (nScore >= 60 && nScore < 80) { sComplexity = "<%tcWebApi_get("String_Entry","PASS_score3","s")%>"; }
	else if (nScore >= 80 && nScore <= 100) { sComplexity = "<%tcWebApi_get("String_Entry","PASS_score4","s")%>"; }

	if(nScore >= 0 && nScore < 40){
		document.getElementById('score').className = "status_no";			
	}
	else if(nScore >= 40 && nScore <= 100){
		document.getElementById('score').className = "status_yes";		
	}	
	oScore.innerHTML = sComplexity;
	if(document.getElementById('score').className == "status_no")
	{
		document.getElementById('score').onmouseover = function(){overHint(11);}
		document.getElementById('score').onmouseout = function(){nd();}
	}
}

function check_wireless_encryption(){
	if(<% check_wireless_encryption()%> == "1"){		
		document.getElementById('wireless_encryption').innerHTML = "<%tcWebApi_get("String_Entry","PASS_score3","s")%>";
		document.getElementById('wireless_encryption').className = "status_yes";
	}
	else{
		document.getElementById('wireless_encryption').innerHTML = "<a href='Advanced_Wireless_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","PASS_score1","s")%></a>";
		document.getElementById('wireless_encryption').className = "status_no";	
		document.getElementById('wireless_encryption').onmouseover = function(){overHint(12);}
		document.getElementById('wireless_encryption').onmouseout = function(){nd();}
	}
}

function check_WPS(){
	var wps_enable_2g = document.form.wps_enable_2g.value;
	var wps_enable_5g = document.form.wps_enable_5g.value;

	if(wps_enable_2g == 0 && wps_enable_2g == 0){
		document.getElementById('wps_status').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('wps_status').className = "status_yes";
	}
	else{
		document.getElementById('wps_status').innerHTML = "<a href='Advanced_WWPS_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('wps_status').className = "status_no";	
		document.getElementById('wps_status').onmouseover = function(){overHint(25);}
		document.getElementById('wps_status').onmouseout = function(){nd();}
	}
}

function check_upnp(){
	var wan_upnp_enable = document.form.wan_upnp_enable.value;
	
	if(wan_upnp_enable == "No"){
		document.getElementById('upnp_service').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('upnp_service').className = "status_yes";
	}
	else{
		document.getElementById('upnp_service').innerHTML = "<a href='Advanced_DSL_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('upnp_service').className = "status_no";
		document.getElementById('upnp_service').onmouseover = function(){overHint(13);}
		document.getElementById('upnp_service').onmouseout = function(){nd();}
	}
}

function check_wan_access(){
	var wan_access_enable = document.form.misc_http_x.value;

	if(wan_access_enable == 0){
		document.getElementById('access_from_wan').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('access_from_wan').className = "status_yes";
	}
	else{
		document.getElementById('access_from_wan').innerHTML = "<a href='Advanced_System_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('access_from_wan').className = "status_no";
		document.getElementById('access_from_wan').onmouseover = function(){overHint(14);}
		document.getElementById('access_from_wan').onmouseout = function(){nd();}
	}
}

function check_ping_form_wan(){
	var wan_ping_enable = document.form.misc_ping_x.value;

	if(wan_ping_enable == 0){
		document.getElementById('ping_from_wan').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('ping_from_wan').className = "status_yes";
	}
	else{
		document.getElementById('ping_from_wan').innerHTML = "<a href='Advanced_BasicFirewall_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('ping_from_wan').className = "status_no";
		document.getElementById('ping_from_wan').onmouseover = function(){overHint(15);}
		document.getElementById('ping_from_wan').onmouseout = function(){nd();}
	}
}

function check_dmz(){
	if(document.form.dmz_ip.value == ""){
		document.getElementById('dmz_service').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('dmz_service').className = "status_yes";
	}
	else{
		document.getElementById('dmz_service').innerHTML = "<a href='Advanced_Exposed_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('dmz_service').className = "status_no";
		document.getElementById('dmz_service').onmouseover = function(){overHint(16);}
		document.getElementById('dmz_service').onmouseout = function(){nd();}
	}
}

function check_port_trigger(){
	var port_trigger_enable = document.form.autofw_enable_x.value;

	if(port_trigger_enable == 0){
		document.getElementById('port_tirgger').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('port_tirgger').className = "status_yes";
	}
	else{
		document.getElementById('port_tirgger').innerHTML = "<a href='Advanced_PortTrigger_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('port_tirgger').className = "status_no";
		document.getElementById('port_tirgger').onmouseover = function(){overHint(17);}
		document.getElementById('port_tirgger').onmouseout = function(){nd();}
	}

}

function check_port_forwarding(){
	var port_forwarding_enable = document.form.vts_enable_x.value;

	if(port_forwarding_enable == 0){
		document.getElementById('port_forwarding').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('port_forwarding').className = "status_yes";
	}
	else{
		document.getElementById('port_forwarding').innerHTML = "<a href='Advanced_VirtualServer_Content.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('port_forwarding').className = "status_no";
		document.getElementById('port_forwarding').onmouseover = function(){overHint(18);}
		document.getElementById('port_forwarding').onmouseout = function(){nd();}
	}
}

function check_ftp_anonymous(){
	var ftp_account_mode = document.form.st_ftp_mode.value;		//1: shared mode, 2: account mode
	
	if(ftp_account_mode == 1){
		document.getElementById('ftp_account').innerHTML = "<a href='Advanced_AiDisk_ftp.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('ftp_account').className = "status_no";
		document.getElementById('ftp_account').onmouseover = function(){overHint(19);}
		document.getElementById('ftp_account').onmouseout = function(){nd();}
	}
	else{
		document.getElementById('ftp_account').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('ftp_account').className = "status_yes";
	}
}

function check_samba_anonymous(){
	var samba_account_mode = document.form.st_samba_mode.value;		//1: shared mode, 4: account mode
	
	if(samba_account_mode == 1){
		document.getElementById('samba_account').innerHTML = "<a href='Advanced_AiDisk_samba.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('samba_account').className = "status_no";
		document.getElementById('samba_account').onmouseover = function(){overHint(20);}
		document.getElementById('samba_account').onmouseout = function(){nd();}
	}
	else{
		document.getElementById('samba_account').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('samba_account').className = "status_yes";
	}
}

function check_TM_feature(){
	var wrs_cc_enable = document.form.wrs_cc_enable.value;
	var wrs_vp_enable = document.form.wrs_vp_enable.value;
	var wrs_mals_enable = document.form.wrs_mals_enable.value;

	if(wrs_mals_enable == 1){
		document.getElementById('wrs_service').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('wrs_service').className = "status_yes";
	}
	else{
		document.getElementById('wrs_service').innerHTML = "<a href='AiProtection_HomeProtection.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('wrs_service').className = "status_no";
		document.getElementById('wrs_service').onmouseover = function(){overHint(21);}
		document.getElementById('wrs_service').onmouseout = function(){nd();}
	}

	if(wrs_vp_enable == 1){
		document.getElementById('vp_service').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('vp_service').className = "status_yes";
	}
	else{
		document.getElementById('vp_service').innerHTML = "<a href='AiProtection_HomeProtection.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('vp_service').className = "status_no";
		document.getElementById('vp_service').onmouseover = function(){overHint(22);}
		document.getElementById('vp_service').onmouseout = function(){nd();}
	}
	
	if(wrs_cc_enable == 1){
		document.getElementById('cc_service').innerHTML = "<%tcWebApi_get("String_Entry","checkbox_Yes","s")%>";
		document.getElementById('cc_service').className = "status_yes";
	}
	else{
		document.getElementById('cc_service').innerHTML = "<a href='AiProtection_HomeProtection.asp' target='_blank'><%tcWebApi_get("String_Entry","checkbox_No","s")%></a>";
		document.getElementById('cc_service').className = "status_no";
		document.getElementById('cc_service').onmouseover = function(){overHint(23);}
		document.getElementById('cc_service').onmouseout = function(){nd();}
	}
}

function cancel(){
	button_flag = 0;
	refreshpage();
}

function eula_confirm(){
	document.form.TM_EULA.value = 1;
	if(button_flag == 1){
		document.form.wrs_mals_enable.value = 1;
	}
	else if(button_flag == 2){
		document.form.wrs_vp_enable.value = 1;
	}
	else{
		document.form.wrs_cc_enable.value = 1;
	}
	
	applyRule();
}

</script>
</head>

<body onload="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<div id="agreement_panel" class="eula_panel_container" style="margin-top: -100px;"></div>
<div id="hiddenMask" class="popup_bg" style="z-index:999;">
	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center"></table>
	<!--[if lte IE 6.5.]><script>alert("<#ALERT_TO_CHANGE_BROWSER#>");</script><![endif]-->
</div>
<div id="weakness_div" class="weakness">
	<table style="width:99%;">
		<tr>
			<td>
				<div class="weakness_router_status"><%tcWebApi_get("String_Entry","AiProtection_scan","s")%></div>
			</td>
		</tr>	
		<tr>
			<td>
				<div>
					<table class="weakness_status" cellspacing="0" cellpadding="4" align="center">
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item1","s")%> -</th>
							<td>
								<div id="login_password"></div>
							</td>					
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item2","s")%> -</th>
							<td>
								<div id="score"></div>
							</td>			
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item3","s")%> -</th>
							<td>
								<div id="wireless_encryption"></div>
							</td>			
						</tr>
						<tr>
							<th>WPS disabled -</th>
							<td>
								<div id="wps_status"></div>
							</td>			
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item4","s")%> -</th>
							<td>
								<div id="upnp_service"></div>
							</td>	
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item5","s")%> -</th>
							<td>
								<div id="access_from_wan"></div>
							</td>	
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item6","s")%> -</th>
							<td>
								<div id="ping_from_wan"></div>
							</td>	
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item7","s")%> -</th>
							<td>
								<div id="dmz_service"></div>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item8","s")%> -</th>
							<td>
								<div id="port_tirgger"></div>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item9","s")%> -</th>
							<td>
								<div id="port_forwarding"></div>
							</td>		
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item10","s")%> -</th>
							<td>
								<div id="ftp_account"></div>
							</td>		
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item11","s")%> -</th>
							<td>
								<div id="samba_account"></div>
							</td>		
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item12","s")%> -</th>
							<td>
								<div id="wrs_service"></div>
							</td>	
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_scan_item13","s")%> -</th>
							<td>
								<div id="vp_service"></div>
							</td>						
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","AiProtection_detection_blocking","s")%> -</th>
							<td>
								<div id="cc_service"></div>
							</td>						
						</tr>
					</table>
				</div>
			</td>
		</tr>	
		<tr>
			<td>
				<table style="margin-top:10px;margin-left:auto;margin-right:auto;">
					<tr>
						<td>
							<input class="button_gen" type="button" onclick="close_weakness_status();" value="<%tcWebApi_get("String_Entry","CTL_close","s")%>">
						</td>
						<td>
							<input id="all_security_btn" class="button_gen_long" type="button" onclick="enable_whole_security();" value="<%tcWebApi_get("String_Entry","CTL_secure","s")%>">
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>

</div>

<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="/cgi-bin/AiProtection_HomeProtection.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="AiProtection_HomeProtection.asp">
<input type="hidden" name="next_page" value="AiProtection_HomeProtection.asp">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% tcWebApi_get("SysInfo_Entry","preferred_lang","s") %>" disabled>
<input type="hidden" name="wrs_mals_enable" value="<% tcWebApi_get("AiProtection_Entry","wrs_mals_enable","s") %>">
<input type="hidden" name="wrs_cc_enable" value="<% tcWebApi_get("AiProtection_Entry","wrs_cc_enable","s") %>">
<input type="hidden" name="wrs_vp_enable" value="<% tcWebApi_get("AiProtection_Entry","wrs_vp_enable","s") %>">
<input type="hidden" name="TM_EULA" value="<% tcWebApi_get("AiProtection_Entry","tm_eula","s") %>">
<input type="hidden" name="editFlag" value="0">

<input type="hidden" name="wan_upnp_enable" value="<% tcWebApi_get("Upnpd_Entry","Active","s") %>" disabled>
<input type="hidden" name="misc_http_x" value="<% tcWebApi_get("Firewall_Entry","misc_http_x","s") %>" disabled>
<input type="hidden" name="misc_ping_x" value="<% tcWebApi_get("Firewall_Entry","misc_ping_x","s") %>" disabled>
<input type="hidden" name="dmz_ip" value="<% tcWebApi_get("Dmz_PVC","DMZ_IP","s") %>" disabled>
<input type="hidden" name="autofw_enable_x" value="<% tcWebApi_get("portTriggering_Entry0","AutoFW_Enable","s") %>" disabled>
<input type="hidden" name="vts_enable_x" value="<% tcWebApi_get("VirServer_PVC0_Entry0","VirServer_Enable","s") %>" disabled>
<input type="hidden" name="wps_enable_2g" value="<% If tcWebApi_get("WLan_Common","wl0_wps_enable","h") <> "" then  tcWebApi_get("WLan_Common","wl0_wps_enable","s") else asp_Write("1") end if %>" disabled>
<input type="hidden" name="wps_enable_5g" value="<% If tcWebApi_get("WLan_Common","wl1_wps_enable","h") <> "" then  tcWebApi_get("WLan_Common","wl1_wps_enable","s") else asp_Write("1") end if %>" disabled>
<!-- not support 
<input type="hidden" name="wps_sta_pin" value="<% nvram_get("wps_sta_pin"); %>" disabled>
-->
<input type="hidden" name="st_ftp_force_mode" value="<% tcWebApi_get("Samba_Entry","st_ftp_force_mode","s") %>" disabled>
<input type="hidden" name="st_ftp_mode" value="<% tcWebApi_get("Samba_Entry","st_ftp_mode","s") %>" disabled>
<input type="hidden" name="st_samba_force_mode" value="<% tcWebApi_get("Samba_Entry","st_samba_force_mode","s") %>" disabled>
<input type="hidden" name="st_samba_mode" value="<% tcWebApi_get("Samba_Entry","st_samba_mode","s") %>" disabled>

<!-- mail alert
<input type="hidden" name="wrs_mail_bit" value="<% nvram_get("wrs_mail_bit"); %>">
<input type="hidden" name="PM_SMTP_SERVER" value="<% nvram_get("PM_SMTP_SERVER"); %>">
<input type="hidden" name="PM_SMTP_PORT" value="<% nvram_get("PM_SMTP_PORT"); %>">
<input type="hidden" name="PM_MY_EMAIL" value="<% nvram_get("PM_MY_EMAIL"); %>">
<input type="hidden" name="PM_SMTP_AUTH_USER" value="<% nvram_get("PM_SMTP_AUTH_USER"); %>">
<input type="hidden" name="PM_SMTP_AUTH_PASS" value="">
-->

<table class="content" align="center" cellpadding="0" cellspacing="0" >
	<tr>
		<td width="17">&nbsp;</td>		
		<td valign="top" width="202">				
			<div  id="mainMenu"></div>	
			<div  id="subMenu"></div>		
		</td>					
		<td valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
		<!--===================================Beginning of Main Content===========================================-->		
			<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0" >
				<tr>
					<td valign="top" >		
						<table width="730px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
							<tbody>
							<tr>
								<td style="background:#4D595D" valign="top">
									<div>&nbsp;</div>
									<div>
										<table width="730px">
											<tr>
												<td align="left">
													<span class="formfonttitle"><%tcWebApi_get("String_Entry","AiProtection_title","s")%> - <%tcWebApi_get("String_Entry","AiProtection_Home","s")%></span>
												</td>
											</tr>
										</table>
									</div>									
									<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
									<div id="PC_desc">
										<table width="700px" style="margin-left:25px;">
											<tr>
												<td>
													<img id="guest_image" src="/images/New_ui/HomeProtection.png">
												</td>
												<td>&nbsp;&nbsp;</td>
												<td style="font-style:italic;font-size:14px;">
													<table>
														<tr>
															<td>
																<div style="width:430px"><%tcWebApi_get("String_Entry","AiProtection_HomeDesc2","s")%></div>
																<div style="width:430px"><a id="faq" style="text-decoration:underline;" href="https://www.asus.com/support/FAQ/1008719/" target="_blank"><%tcWebApi_get("String_Entry","AiProtection_Home","s")%> FAQ</a></div>
															</td>
															<td>
																<div style="width:100px;height:48px;margin-left:-40px;background-image:url('/images/New_ui/tm_logo.png');"></div>
															</td>
														</tr>
														<tr>
															<td rowspan="2">
																<div>
																	<img src="/images/New_ui/Home_Protection_Scenario.png">
																</div>
															</td>
														</tr>												
													</table>
												</td>
											</tr>									
										</table>
									</div>
									<!--=====Beginning of Main Content=====-->
									<div style="margin-top:5px;">
										<table style="width:99%;border-collapse:collapse;">
											<tr style="background-color:#444f53;height:120px;">
												<td style="width:25%;border-radius:10px 0px 0px 10px;background-color:#444f53;">
													<div style="text-align:center;background: url('/images/New_ui/AiProtection.png');width:130px;height:130px;margin-left:30px;"></div>	
												</td>
												 <td width="6px">
													<div><img src="/images/cloudsync/line.png"></div>
												</td>
												<td style="padding:10px;">
													<div style="font-size:18px;text-shadow:1px 1px 0px black;"><%tcWebApi_get("String_Entry","AiProtection_scan","s")%></div>
													<div style="font-style: italic;font-size: 14px;color:#FC0;height:auto;padding-top:5px;"><%tcWebApi_get("String_Entry","AiProtection_scan_desc","s")%></div>
												</td>
												 <td width="6px">
													<div><img src="/images/cloudsync/line.png"></div>
												</td>
												<td style="width:20%;border-radius:0px 10px 10px 0px;">
													<div>
														<input class="button_gen" type="button" onclick="check_weakness();" value="<%tcWebApi_get("String_Entry","CTL_scan","s")%>">
													</div>
												</td>										
											</tr>
											
											<tr style="height:10px;"></tr>
											<tr style="background-color:#444f53;height:120px;">
												<td style="width:25%;border-radius:10px 0px 0px 10px;background-color:#444f53;">
													<div style="text-align:center;background: url('/images/New_ui/AiProtection.png');width:130px;height:130px;margin-left:30px;background-position:0px -187px;"></div>	
												</td>
												 <td width="6px">
													<div style="background:url('/images/cloudsync/line.png') no-repeat;width:6px;height:185px;background-size:4px 185px;";></div>
												</td>
												<td style="padding:10px;">
													<div>
														<table>
															<tr>
																<td style="padding-bottom:10px;">
																	<div style="font-size:18px;text-shadow:1px 1px 0px black;"><%tcWebApi_get("String_Entry","AiProtection_sites_blocking","s")%></div>
																	<div style="font-style: italic;font-size: 14px;color:#FC0;height:auto;padding-top:5px;"><%tcWebApi_get("String_Entry","AiProtection_sites_block_desc","s")%></div>
																</td>
															</tr>										
															<tr>
																<td>
																	<div style="font-size:18px;text-shadow:1px 1px 0px black;"><%tcWebApi_get("String_Entry","AiProtection_Vulnerability","s")%></div>
																	<div style="font-style: italic;font-size: 14px;color:#FC0;height:auto;padding-top:5px;"><%tcWebApi_get("String_Entry","AiProtection_Vulnerability_desc","s")%></div>
																</td>
															</tr>
														</table>
													</div>								
												</td>
												 <td width="6px">
													<div style="background:url('/images/cloudsync/line.png') no-repeat;width:6px;height:185px;background-size:4px 185px;";></div>
												</td>
												<td style="width:20%;border-radius:0px 10px 10px 0px;">
													<div>
														<table>
															<tr>
																<td>
																	<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_mals_enable"></div>
																	<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
																		<script type="text/javascript">
																			$j('#radio_mals_enable').iphoneSwitch('<% tcWebApi_get("AiProtection_Entry","wrs_mals_enable","s") %>',
																				function(){
																					if(document.form.TM_EULA.value == 0){
																						button_flag = 1;
																						ASUS_EULA.config(eula_confirm, cancel);
																						if(ASUS_EULA.check("tm")){
																							document.form.wrs_mals_enable.value = 1;
																							applyRule();
																						}
																					}
																					else{
																						document.form.wrs_mals_enable.value = 1;
																						applyRule();
																					}
																				},
																				function(){
																					document.form.wrs_mals_enable.value = 0;
																					applyRule();
																				},
																				{
																					switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
																				}
																			);
																		</script>			
																	</div>
																</td>													
															</tr>
															<tr>
																<td>
																	<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_vp_enable"></div>
																	<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
																		<script type="text/javascript">
																			$j('#radio_vp_enable').iphoneSwitch('<% tcWebApi_get("AiProtection_Entry","wrs_vp_enable","s") %>',
																				function(){
																					if(document.form.TM_EULA.value == 0){
																						button_flag = 2;
																						ASUS_EULA.config(eula_confirm, cancel);
																						if(ASUS_EULA.check("tm")){
																							document.form.wrs_vp_enable.value = 1;
																							applyRule();
																						}
																					}
																					else{
																						document.form.wrs_vp_enable.value = 1;
																						applyRule();
																					}
																				},
																				function(){
																					document.form.wrs_vp_enable.value = 0;	
																					applyRule();
																				},
																				{
																					switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
																				}
																			);
																		</script>			
																	</div>
																</td>													
															</tr>
														</table>
													</div>
												</td>										
											</tr>
											<tr style="height:10px;"></tr>										
											<tr style="background-color:#444f53;height:120px;">
												<td style="width:25%;border-radius:10px 0px 0px 10px;background-color:#444f53;">
													<div style="text-align:center;background: url('/images/New_ui/AiProtection.png');width:130px;height:140px;margin-left:30px;background-position:0px -385px";></div>	
												</td>
												 <td width="6px">
													<div><img src="/images/cloudsync/line.png"></div>
												</td>
												<td style="padding:10px;">
													<div style="font-size:18px;text-shadow:1px 1px 0px black;"><%tcWebApi_get("String_Entry","AiProtection_detection_blocking","s")%></div>
													<div style="font-style: italic;font-size: 14px;color:#FC0;height:auto;;padding-top:5px;"><%tcWebApi_get("String_Entry","AiProtection_detection_block_desc","s")%></div>
												</td>
												 <td width="6px">
													<div><img src="/images/cloudsync/line.png"></div>
												</td>
												<td style="width:20%;border-radius:0px 10px 10px 0px;">
													<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_cc_enable"></div>
													<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
														<script type="text/javascript">
															$j('#radio_cc_enable').iphoneSwitch('<% tcWebApi_get("AiProtection_Entry","wrs_cc_enable","s") %>',
																function(){
																	if(document.form.TM_EULA.value == 0){
																		button_flag = 3;
																		ASUS_EULA.config(eula_confirm, cancel);
																		if(ASUS_EULA.check("tm")){
																			document.form.wrs_cc_enable.value = 1;
																			applyRule();
																		}
																	}
																	else{
																		document.form.wrs_cc_enable.value = 1;
																		applyRule();
																	}
																},
																function(){
																	document.form.wrs_cc_enable.value = 0;
																	applyRule();
																},
																{
																	switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
																}
															);
														</script>			
													</div>
<!--
													<div style="margin-top:-15px;">
														<input class="button_gen_long" type="button" onclick="show_alert_preference();" value="<%tcWebApi_get("String_Entry","AiProtection_alert_pref","s")%>">
													</div>
-->
												</td>										
											</tr>
										</table>
									</div>
									<div style="width:135px;height:55px;margin: 10px 0 0 600px;background-image:url('/images/New_ui/tm_logo_power.png');"></div>
								</td>
							</tr>
							</tbody>	
						</table>
					</td>         
				</tr>
			</table>				
		<!--===================================Ending of Main Content===========================================-->		
		</td>		
		<td width="10" align="center" valign="top">&nbsp;</td>
	</tr>
</table>
<div id="footer"></div>
</form>
</body>
</html>
