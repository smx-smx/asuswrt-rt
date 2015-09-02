<%
tcWebApi_set("WLan_Common","editFlag","editFlag")
tcWebApi_set("WLan_Common","MBSSID_changeFlag","MBSSID_changeFlag")
tcWebApi_set("WLan_Common","MBSSID_able_Flag","MBSSID_able_Flag")
If Request_Form("editFlag") = "1" then
	tcWebApi_Set("SysInfo_Entry","w_Setting","w_Setting")
	tcWebApi_commit("SysInfo_Entry")

	tcWebApi_Set("WLan_Common","wl_unit","wl_unit")
	tcWebApi_Set("WLan_Entry","radio_on","radio_on")
	tcWebApi_Set("WLan_Entry","wl0_radio_on","wl0_radio_on")
	tcWebApi_Set("WLan_Entry","wl1_radio_on","wl1_radio_on")
	tcWebApi_Set("WLan_Entry","radio_flag","radio_flag")
	tcWebApi_Set("WLan_Entry","wl_schedule_radio","wl_schedule_radio")
	tcWebApi_Set("WLan_Entry","wl_radio_date","wl_radio_date_x")
	tcWebApi_Set("WLan_Entry","wl_radio_time","wl_radio_time_x")
	'tcWebApi_Set("WLan_Entry","wl_radio_time2","wl_radio_time2_x")
	tcWebApi_Set("WLan_Common","BasicRate","wl_rateset")
	tcWebApi_Set("WLan_Common","FragThreshold","wl_frag")
	tcWebApi_Set("WLan_Common","RTSThreshold","wl_rts")
	tcWebApi_Set("WLan_Common","DtimPeriod","wl_dtim")
	tcWebApi_Set("WLan_Common","BeaconPeriod","wl_bcn")
	tcWebApi_Set("WLan_Common","TxBurst","wl_frameburst")
	tcWebApi_Set("WLan_Common","PktAggregate","wl_PktAggregate")
	tcWebApi_Set("WLan_Entry","WmmCapable","wl_wme")
	tcWebApi_Set("WLan_Common","APSDCapable","wl_wme_apsd")
	tcWebApi_Set("WLan_Common","DLSCapable","wl_DLSCapable")
	tcWebApi_Set("WLan_Common","wl_wme_no_ack","wl_wme_no_ack")
	tcWebApi_Set("WLan_Common","TxPower","wl_txpower")
	tcWebApi_Set("WLan_Common","wl_ap_isolate","wl_ap_isolate")
	tcWebApi_Set("WLan_Entry","wl_user_rssi","wl_user_rssi")
	tcWebApi_Set("WLan_Common","IgmpSnEnable","wl_igs")
	tcWebApi_Set("WLan_Common","wl_mrate_x","wl_mrate_x")

	load_parameters_from_generic()

	tcWebApi_commit("WLan_Entry")
end if

load_parameters_to_generic()
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_WAdvanced_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_1_6","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/calendar/jquery-ui.js"></script> 
<style>
.ui-slider {
	position: relative;
	text-align: left;
}
.ui-slider .ui-slider-handle {
	position: absolute;
	width: 12px;
	height: 12px;
}
.ui-slider .ui-slider-range {
	position: absolute;
}
.ui-slider-horizontal {
	height: 6px;
}

.ui-widget-content {
	border: 2px solid #000;
	background-color:#000;
}
.ui-state-default,
.ui-widget-content .ui-state-default,
.ui-widget-header .ui-state-default {
	border: 1px solid ;
	background: #e6e6e6;
	margin-top:-4px;
	margin-left:-6px;
}

/* Corner radius */
.ui-corner-all,
.ui-corner-top,
.ui-corner-left,
.ui-corner-tl {
	border-top-left-radius: 4px;
}
.ui-corner-all,
.ui-corner-top,
.ui-corner-right,
.ui-corner-tr {
	border-top-right-radius: 4px;
}
.ui-corner-all,
.ui-corner-bottom,
.ui-corner-left,
.ui-corner-bl {
	border-bottom-left-radius: 4px;
}
.ui-corner-all,
.ui-corner-bottom,
.ui-corner-right,
.ui-corner-br {
	border-bottom-right-radius: 4px;
}

.ui-slider-horizontal .ui-slider-range {
	top: 0;
	height: 100%;
}

#slider .ui-slider-range {
	background: #93E7FF; 
	border-top-left-radius: 3px;
	border-top-right-radius: 1px;
	border-bottom-left-radius: 3px;
	border-bottom-right-radius: 1px;
}
#slider .ui-slider-handle { border-color: #93E7FF; }


</style>
<script>	
var $j = jQuery.noConflict();//var flag = 0;
	
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var flag = 0;
var enable_radio = "<% tcWebApi_get("WLan_Entry","radio_on","s") %>";
var wl_user_rssi_onload = "<% tcWebApi_get("WLan_Entry","wl_user_rssi","s") %>";

function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }


var wireless = []; // [[MAC, associated, authorized], ...]
var mcast_rates = [
	["HTMIX 6.5/15", "14", 0, 1],
	["HTMIX 13/30",	 "15", 0, 1],
	["HTMIX 19.5/45","16", 0, 1],
	["HTMIX 13/30",	 "17", 0, 1],
	["HTMIX 26/60",	 "18", 0, 1],
	["HTMIX 130/144","13", 0, 1],
	["OFDM 6",	 "4",  0, 0],
	["OFDM 9",	 "5",  0, 0],
	["OFDM 12",	 "7",  0, 0],
	["OFDM 18",	 "8",  0, 0],
	["OFDM 24",	 "9",  0, 0],
	["OFDM 36",	 "10", 0, 0],
	["OFDM 48",	 "11", 0, 0],
	["OFDM 54",	 "12", 0, 0],
	["CCK 1",	 "1",  1, 0],
	["CCK 2",	 "2",  1, 0],
	["CCK 5.5",	 "3",  1, 0],
	["CCK 11",	 "6",  1, 0]
];

function initial(){
	show_menu();
	autoFocus('<% get_parameter("af"); %>');
	
	register_event();
	load_body();	

	if(userRSSI_support > 0)
		changeRSSI(wl_user_rssi_onload);
	else
		document.getElementById("rssiTr").style.display = "none";

	if(sw_mode == "2"){
		disableAdvFn(17);
		change_common(document.form.wl_wme, "WLANConfig11b", "wl_wme");
	}
	$("wl_rate").style.display = "none";
	/*if(wifi_hw_sw_support != -1) {
		$("wl_rf_enable").style.display = "none";
	}*/
	if(band5g_support == -1)
		$("wl_unit_field").style.display = "none";

/*
	if(Rawifi_support == -1){ //without rawifi
		$("DLSCapable").style.display = "none";
		$("PktAggregate").style.display = "none";
		$("enable_wl_multicast_forward").style.display = "";
	}
	else{
		free_options(document.form.wl_mrate_x);
		add_option(document.form.wl_mrate_x, "Disabled", 0, 0 == 0);	//need get the wl_mrate_x value by function
		add_option(document.form.wl_mrate_x, "Auto", 13, 0 == 13);	//need get the wl_mrate_x value by function
	}
	if(repeater_support != -1){ //with RE mode
		$("DLSCapable").style.display = "none";
	}
*/

	var mcast_rate = '<% tcWebApi_get("WLan_Common","wl_mrate_x","s") %>';
	var mcast_unit = '<% tcWebApi_get("WLan_Common","wl_unit","s") %>';
	for (var i = 0; i < mcast_rates.length; i++) {
		if (mcast_unit == '1' && mcast_rates[i][2]) // 5Ghz && CCK
			continue;
		if (!Rawifi_support && mcast_rates[i][3]) // BCM && HTMIX
			continue;
		add_option(document.form.wl_mrate_x,
			mcast_rates[i][0], mcast_rates[i][1],
			(mcast_rate == mcast_rates[i][1]) ? 1 : 0);
	}

	//Note: the value of wl_nmode_x
	//2G=> 0:Legacy, 6:N only, 9:Auto
	//5G=> 2:Legacy, 11: N only, 8:Auto
	if(document.form.wl_nmode_x.value == "0" || document.form.wl_nmode_x.value == "2"){ //Legacy
		//Before MTK fix Fragmentation Threshold not work issue, we hide the Fragmentation Threshold option temporarily.
		inputCtrl(document.form.wl_frag, 1);
		inputCtrl(document.form.wl_wme, 1);
		inputCtrl(document.form.wl_wme_no_ack, 1);
	}
	else{
		inputCtrl(document.form.wl_frag, 0);
		inputCtrl(document.form.wl_wme, 0);
		inputCtrl(document.form.wl_wme_no_ack, 0);
	}

	adjust_tx_power();
	loadDateTime();
	corrected_timezone(DAYLIGHT_orig, TZ_orig);

	if(enable_radio == "1"){
		$("wl_rf_schedule_enable").style.display = "";
		$("enable_day_week_tr").style.display = "";
		$("enable_time_week_tr").style.display = "";
	}
	else{
		$("wl_rf_schedule_enable").style.display = "none";
		$("enable_day_week_tr").style.display = "none";
		$("enable_time_week_tr").style.display = "none";
	}

	control_TimeField();
	//RT65168 use APOn to record enable/disable
}

function changeRSSI(_switch){
	if(_switch == 0){
		document.getElementById("rssiDbm").style.display = "none";
		document.form.wl_user_rssi.value = 0;
	}
	else{
		document.getElementById("rssiDbm").style.display = "";
		if(wl_user_rssi_onload == 0)
			document.form.wl_user_rssi.value = "-70";
		else
			document.form.wl_user_rssi.value = wl_user_rssi_onload;
	}
}

function applyRule(){

	if(document.form.radio_on[0].checked){	//if software radio on
		if(document.form.wl_schedule_radio[1].checked){	//if schedule off
			document.form.radio_flag.value = 1;	//physical radio status on
			if(document.form.wl_unit[0].selected){
				document.form.wl0_radio_on.value = 1;
			}
			else{
				document.form.wl1_radio_on.value = 1;
			}
		}
	}
	else{	//if software radio off
		document.form.wl_schedule_radio[1].checked = true;	//schedule off
		document.form.radio_flag.value = 0;	//physical radio status on
			if(document.form.wl_unit[0].selected){
				document.form.wl0_radio_on.value = 0;
			}
			else{
				document.form.wl1_radio_on.value = 0;
			}
	}
	
	if(validForm()){
		updateDateTime(document.form.current_page.value);
		if(model_name == "DSL-N66U")
		{
			showLoading(28);
			setTimeout("redirect();", 28000);
		}
		else
		{
			showLoading(23);
			setTimeout("redirect();", 23000);
		}
		document.form.action = "/cgi-bin/Advanced_WAdvanced_Content.asp";
		document.form.editFlag.value = "1" ;
		
		if(navigator.appName.indexOf("Microsoft") >= 0){ 		// Jieming added at 2013/05/21, to avoid browser freeze when submitting form on IE
			stopFlag = 1;
		}
		document.form.submit();
	}
}
function validForm(){
	if(sw_mode != "2"){
		if(!validate_range(document.form.wl_frag, 256, 2346)
		|| !validate_range(document.form.wl_rts, 0, 2347)
		|| !validate_range(document.form.wl_dtim, 1, 255)
		|| !validate_range(document.form.wl_bcn, 20, 1000)
		){
			return false;
		}
	}
//	if(!validate_timerange(document.form.wl_radio_time_x_starthour, 0) || !validate_timerange(document.form.wl_radio_time2_x_starthour, 0)
//			|| !validate_timerange(document.form.wl_radio_time_x_startmin, 1) || !validate_timerange(document.form.wl_radio_time2_x_startmin, 1)
//			|| !validate_timerange(document.form.wl_radio_time_x_endhour, 2) || !validate_timerange(document.form.wl_radio_time2_x_endhour, 2)
//			|| !validate_timerange(document.form.wl_radio_time_x_endmin, 3) || !validate_timerange(document.form.wl_radio_time2_x_endmin, 3)
	if(!validate_timerange(document.form.wl_radio_time_x_starthour, 0)
			|| !validate_timerange(document.form.wl_radio_time_x_startmin, 1)
			|| !validate_timerange(document.form.wl_radio_time_x_endhour, 2)
			|| !validate_timerange(document.form.wl_radio_time_x_endmin, 3)
	)
		return false;
	
	if(document.form.wl_radio_time_x_starthour.value == document.form.wl_radio_time_x_endhour.value && document.form.wl_radio_time_x_startmin.value == document.form.wl_radio_time_x_endmin.value){
			alert("<%tcWebApi_get("String_Entry","FC_URLActiveTime_itemhint2","s")%>");
			document.form.wl_radio_time_x_startmin.focus();
			document.form.wl_radio_time_x_startmin.select;
			return false;
	}
	if(document.form.wl_radio_date_x_Sun.checked == false
	&& document.form.wl_radio_date_x_Mon.checked == false
	&& document.form.wl_radio_date_x_Tue.checked == false
	&& document.form.wl_radio_date_x_Wed.checked == false
	&& document.form.wl_radio_date_x_Thu.checked == false
	&& document.form.wl_radio_date_x_Fri.checked == false
	&& document.form.wl_radio_date_x_Sat.checked == false)
	//document.form.wl_radio[1].checked=true;
	{
		document.form.wl_radio_date_x_Sun.focus();
		$('blank_warn').style.display = "";
		return false;
	}
			
	if(userRSSI_support){
		if(document.form.wl_user_rssi.value != 0){
			if(!validator.range(document.form.wl_user_rssi, -90, -70)){
				document.form.wl_user_rssi.focus();
				return false;
			}
		}
	}
	return true;
}
function done_validating(action){
refreshpage();
}
function disableAdvFn(row){
for(var i=row; i>=3; i--){
$("WAdvTable").deleteRow(i);
}
}

function adjust_tx_power(){
	var power_value_old = document.form.wl_TxPower_orig.value;
	var power_value_new = document.form.wl_TxPower_orig.value;
	var translated_value = 0;
	
	if(!power_support){
		$("wl_txPower_field").style.display = "none";
	}
	else{
		if(power_value_old != ""){
			translated_value = parseInt(power_value_old/80*100);
			translated_value = parseInt(power_value_old);
			//alert(translated_value);
			if(translated_value >=100){
				translated_value = 100;
			}
			else if(translated_value <= 1){
				translated_value = 1;			
			}

			$('slider').children[0].style.width = translated_value + "%";
			$('slider').children[1].style.left = translated_value + "%";
			document.form.wl_txpower.value = translated_value;
		}
		else{
			$('slider').children[0].style.width = power_value_new + "%";
			$('slider').children[1].style.left = power_value_new + "%";
			document.form.wl_txpower.value = power_value_new;
		}
	}
}

function loadDateTime(){
	document.form.wl_radio_date_x_Sun.checked = getDateCheck(document.form.wl_radio_date_x.value, 0);
	document.form.wl_radio_date_x_Mon.checked = getDateCheck(document.form.wl_radio_date_x.value, 1);
	document.form.wl_radio_date_x_Tue.checked = getDateCheck(document.form.wl_radio_date_x.value, 2);
	document.form.wl_radio_date_x_Wed.checked = getDateCheck(document.form.wl_radio_date_x.value, 3);
	document.form.wl_radio_date_x_Thu.checked = getDateCheck(document.form.wl_radio_date_x.value, 4);
	document.form.wl_radio_date_x_Fri.checked = getDateCheck(document.form.wl_radio_date_x.value, 5);
	document.form.wl_radio_date_x_Sat.checked = getDateCheck(document.form.wl_radio_date_x.value, 6);
	document.form.wl_radio_time_x_starthour.value = getTimeRange(document.form.wl_radio_time_x.value, 0);
	document.form.wl_radio_time_x_startmin.value = getTimeRange(document.form.wl_radio_time_x.value, 1);
	document.form.wl_radio_time_x_endhour.value = getTimeRange(document.form.wl_radio_time_x.value, 2);
	document.form.wl_radio_time_x_endmin.value = getTimeRange(document.form.wl_radio_time_x.value, 3);
/*
	document.form.wl_radio_time2_x_starthour.value = getTimeRange(document.form.wl_radio_time2_x.value, 0);
	document.form.wl_radio_time2_x_startmin.value = getTimeRange(document.form.wl_radio_time2_x.value, 1);
	document.form.wl_radio_time2_x_endhour.value = getTimeRange(document.form.wl_radio_time2_x.value, 2);
	document.form.wl_radio_time2_x_endmin.value = getTimeRange(document.form.wl_radio_time2_x.value, 3);
*/
}
function control_Schedule(){
	if(document.form.radio_on[0].checked){
		$("wl_rf_schedule_enable").style.display = "";
		$("enable_day_week_tr").style.display = "";
		$("enable_time_week_tr").style.display = "";
		document.form.wl_schedule_radio[0].checked = true;
	}
	else{
		$("wl_rf_schedule_enable").style.display = "none";
		$("enable_day_week_tr").style.display = "none";
		$("enable_time_week_tr").style.display = "none";
		document.form.wl_schedule_radio[1].checked = true;
	}

	control_TimeField();
}
function control_TimeField(){		//control time of week & weekend field when wireless radio is down , Jieming added 2012/08/22
	if(!document.form.wl_schedule_radio[0].checked){
/*
		//ASUS WRT hide date and time and separate week and weekend.
		$('enable_date_week_tr').style.display="none";
		$('enable_time_week_tr').style.display="none";
		$('enable_date_weekend_tr').style.display="none";
		$('enable_time_weekend_tr').style.display="none";
*/
		document.form.wl_radio_date_x_Sun.disabled= true;
		document.form.wl_radio_date_x_Mon.disabled= true;
		document.form.wl_radio_date_x_Tue.disabled= true;
		document.form.wl_radio_date_x_Wed.disabled= true;
		document.form.wl_radio_date_x_Thu.disabled= true;
		document.form.wl_radio_date_x_Fri.disabled= true;
		document.form.wl_radio_date_x_Sat.disabled= true;
		document.form.wl_radio_time_x_starthour.disabled= true;
		document.form.wl_radio_time_x_startmin.disabled= true;
		document.form.wl_radio_time_x_endhour.disabled= true;
		document.form.wl_radio_time_x_endmin.disabled= true;
	}
	else{
/*
		//ASUS WRT hide date and time and separate week and weekend.
		$('enable_date_week_tr').style.display="";
		$('enable_time_week_tr').style.display="";
		$('enable_date_weekend_tr').style.display="";
		$('enable_time_weekend_tr').style.display="";
*/
		document.form.wl_radio_date_x_Sun.disabled= false;
		document.form.wl_radio_date_x_Mon.disabled= false;
		document.form.wl_radio_date_x_Tue.disabled= false;
		document.form.wl_radio_date_x_Wed.disabled= false;
		document.form.wl_radio_date_x_Thu.disabled= false;
		document.form.wl_radio_date_x_Fri.disabled= false;
		document.form.wl_radio_date_x_Sat.disabled= false;
		document.form.wl_radio_time_x_starthour.disabled= false;
		document.form.wl_radio_time_x_startmin.disabled= false;
		document.form.wl_radio_time_x_endhour.disabled= false;
		document.form.wl_radio_time_x_endmin.disabled= false;
	}
}

function _change_wl_unit(wl_unit){
	document.form_band.wl_unit.value = wl_unit;
	document.form_band.action = "/cgi-bin/Advanced_WAdvanced_Content.asp";
	showLoading(2);
	setTimeout("redirect();", 2000);
	document.form_band.submit();
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_WAdvanced_Content.asp";
}

function register_event(){
	$j(function() {
		$j( "#slider" ).slider({
			orientation: "horizontal",
			range: "min",
			min:1,
			max: 100,
			value:100,
			slide:function(event, ui){
				document.getElementById('wl_txpower').value = ui.value; 
			},
			stop:function(event, ui){
				set_power(ui.value);	  
			}
		}); 
	});
}


function set_power(power_value){	
	if(power_value < 1){
		power_value = 1;
		alert("The minimun value of power is 1");
	}
	
	if(power_value > 100){
		power_value = 100;
		alert("The maximun value of power is 1");
	}
	
	$('slider').children[0].style.width = power_value + "%";
	$('slider').children[1].style.left = power_value + "%";
	document.form.wl_txpower.value = power_value;	
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form_band" action="Advanced_WAdvanced_Content.asp" target="hidden_frame">
<input type="hidden" name="wl_unit">
<input type="hidden" name="editFlag" value="0">
</form>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="wan_route_x" value="">
<input type="hidden" name="wan_nat_x" value="1">
<input type="hidden" name="wl_nmode_x" value="<% tcWebApi_get("WLan_Common","WirelessMode","s"); %>">
<input type="hidden" name="wl_gmode_protection_x" value="">
<input type="hidden" name="current_page" value="Advanced_WAdvanced_Content.asp">
<input type="hidden" name="next_page" value="SaveRestart.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_wireless">
<input type="hidden" name="action_wait" value="3">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">

<input type="hidden" name="wl_radio_date_x" value="<% If tcWebApi_get("WLan_Entry","wl_radio_date","h") <> "" then  tcWebApi_get("WLan_Entry","wl_radio_date","s") else asp_Write("1111111") end if %>">
<input type="hidden" name="wl_radio_time_x" value="<% If tcWebApi_get("WLan_Entry","wl_radio_time","h") <> "" then  tcWebApi_get("WLan_Entry","wl_radio_time","s") else asp_Write("00002359") end if %>">
<!--
<input type="hidden" name="wl_radio_time2_x" value="<% If tcWebApi_get("WLan_Entry","wl_radio_time2","h") <> "" then  tcWebApi_get("WLan_Entry","wl_radio_time2","s") else asp_Write("00002359") end if %>">
-->
<!--
<input type="hidden" name="wl_radio_date_x" value="1111111">
<input type="hidden" name="wl_radio_time_x" value="00002359">
-->
<input type="hidden" name="wl_subunit" value="-1">
<input type="hidden" name="wl_plcphdr" value="long">
<input type="hidden" name="wl_amsdu" value="auto">
<input type="hidden" name="wl_TxPower_orig" value="<% tcWebApi_get("WLan_Common","TxPower","s") %>">
<input type="hidden" name="editFlag" value="0">
<input type="hidden" name="MBSSID_changeFlag" value="0">
<input type="hidden" name="MBSSID_able_Flag" value="0">
<input type="hidden" name="radio_flag" value="<% tcWebApi_staticGet("WLan_Entry","radio_flag","s") %>">
<input type="hidden" name="wl0_radio_on" value="<% tcWebApi_staticGet("WLan_Entry","wl0_radio_on","s") %>">
<input type="hidden" name="wl1_radio_on" value="<% tcWebApi_staticGet("WLan_Entry","wl1_radio_on","s") %>">
<input type="hidden" name="w_Setting" value="1">
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
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
		  			<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_1","s")%> - <%tcWebApi_get("String_Entry","menu5_1_6","s")%></div>
						<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		 				<div class="formfontdesc"><%tcWebApi_get("String_Entry","WC11b_display5_sd","s")%></div>
		 				<div id="timezone_hint" onclick="location.href='Advanced_System_Content.asp?af=uiViewdateToolsTZ'" style="display:none;margin-left:4px;color:#FFCC00;text-decoration:underline;cursor:pointer;"></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable" id="WAdvTable">
	<tr id="wl_unit_field">
						<th><%tcWebApi_get("String_Entry","Interface","s")%></th>
		<td>
			<select name="wl_unit" class="input_option" onChange="_change_wl_unit(this.value);">
				<option class="content_input_fd" value="0" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "0" then asp_Write("selected") end if %>>2.4GHz</option>
				<option class="content_input_fd" value="1" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "1" then asp_Write("selected") end if %>>5GHz</option>
			</select>
		</td>
	</tr>
	<tr id="wl_rf_enable">
	<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 1);"><%tcWebApi_get("String_Entry","WC11b_x_RadioEnable_in","s")%></a></th>
	<td>
	<input type="radio" value="1" name="radio_on" class="input" onClick="control_Schedule();" <% if tcWebApi_get("WLan_Entry","radio_on","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
	<input type="radio" value="0" name="radio_on" class="input" onClick="control_Schedule();" <% if tcWebApi_get("WLan_Entry","radio_on","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
	</td>
	</tr>
	<tr id="wl_rf_schedule_enable">
	<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 22);"><%tcWebApi_get("String_Entry","WC11b_x_RadioEnable_scheduler","s")%></a></th>
	<td>
	<input type="radio" value="1" name="wl_schedule_radio" class="input" onClick="control_TimeField();return change_common_radio(this, 'WLANConfig11b', 'wl_schedule_radio', '1')" <% if tcWebApi_get("WLan_Entry","wl_schedule_radio","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
	<input type="radio" value="0" name="wl_schedule_radio" class="input" onClick="control_TimeField();return change_common_radio(this, 'WLANConfig11b', 'wl_schedule_radio', '0')" <% if tcWebApi_get("WLan_Entry","wl_schedule_radio","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
	</td>
	</tr>
	<tr id="enable_day_week_tr">
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 2);"><%tcWebApi_get("String_Entry","WC11b_x_RadioEnableDate_in","s")%></a></th>
		<td>
			<input type="checkbox" class="input" name="wl_radio_date_x_Sun" onChange="return changeDate();">Sun
			<input type="checkbox" class="input" name="wl_radio_date_x_Mon" onChange="return changeDate();">Mon
			<input type="checkbox" class="input" name="wl_radio_date_x_Tue" onChange="return changeDate();">Tue
			<input type="checkbox" class="input" name="wl_radio_date_x_Wed" onChange="return changeDate();">Wed
			<input type="checkbox" class="input" name="wl_radio_date_x_Thu" onChange="return changeDate();">Thu
			<input type="checkbox" class="input" name="wl_radio_date_x_Fri" onChange="return changeDate();">Fri
			<input type="checkbox" class="input" name="wl_radio_date_x_Sat" onChange="return changeDate();">Sat
			<span id="blank_warn" style="display:none;">You cannot leave this field blank!</span>
		</td>
	</tr>
	<tr id="enable_time_week_tr">
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 3);"><%tcWebApi_get("String_Entry","WC11b_x_RadioEnableTime_in","s")%></a></th>
		<td>
			<input type="text" maxlength="2" class="input_3_table" name="wl_radio_time_x_starthour" onKeyPress="return is_number(this,event)" onblur="validate_timerange(this, 0);">:
			<input type="text" maxlength="2" class="input_3_table" name="wl_radio_time_x_startmin" onKeyPress="return is_number(this,event)" onblur="validate_timerange(this, 1);">-
			<input type="text" maxlength="2" class="input_3_table" name="wl_radio_time_x_endhour" onKeyPress="return is_number(this,event)" onblur="validate_timerange(this, 2);">:
			<input type="text" maxlength="2" class="input_3_table" name="wl_radio_time_x_endmin" onKeyPress="return is_number(this,event)" onblur="validate_timerange(this, 3);">
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 5);"><%tcWebApi_get("String_Entry","WC11b_x_IsolateAP_in","s")%></a></th>
		<td>
			<input type="radio" value="1" name="wl_ap_isolate" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '1')" <% if tcWebApi_get("WLan_Common","wl_ap_isolate","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
			<input type="radio" value="0" name="wl_ap_isolate" class="input" onClick="return change_common_radio(this, 'WLANConfig11b', 'wl_ap_isolate', '0')" <% if tcWebApi_get("WLan_Common","wl_ap_isolate","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
		</td>
	</tr>
	<tr id="wl_rate">
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 6);"><%tcWebApi_get("String_Entry","WC11b_DataRateAll_in","s")%></a></th>
	<td>
	<select name="wl_rate" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_rate')">
	<option value="0" >auto</option>
	<option value="1000000" >1</option>
	<option value="2000000" >2</option>
	<option value="5500000" >5.5</option>
	<option value="6000000" >6</option>
	<option value="9000000" >9</option>
	<option value="11000000" >11</option>
	<option value="12000000" >12</option>
	<option value="18000000" >18</option>
	<option value="24000000" >24</option>
	<option value="36000000" >36</option>
	<option value="48000000" >48</option>
	<option value="54000000" >54</option>
	</div>
	</td>
	</tr>

	<tr id="rssiTr">
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 23);"><%tcWebApi_get("String_Entry","Roaming_assistant","s")%></a></th>
		<td>
			<select id="wl_user_rssi_option" class="input_option" onchange="changeRSSI(this.value);">
				<option value="1"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
				<option value="0" <% if tcWebApi_get("WLan_Entry","wl_user_rssi","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
			</select>
			<span id="rssiDbm" style="color:#FFF">
				Disconnect clients with RSSI lower than
			<input type="text" maxlength="3" name="wl_user_rssi" class="input_3_table" value="<% tcWebApi_get("WLan_Entry","wl_user_rssi","s"); %>" autocorrect="off" autocapitalize="off">
				dB
			</span>
		</td>
	</tr>

	<tr id="wl_igs_select">
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 21);"><%tcWebApi_get("String_Entry","WC11b_x_IgmpSnEnable_in","s")%></a></th>
		<td>
			<select name="wl_igs" class="input_option">
				<option value="0" <% if tcWebApi_get("WLan_Common","IgmpSnEnable","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
				<option value="1" <% if tcWebApi_get("WLan_Common","IgmpSnEnable","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
			</select>
		</td>
	</tr>
	<tr id="wl_mrate_select">
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 7);"><%tcWebApi_get("String_Entry","WC11b_MultiRateAll_in","s")%></a></th>
		<td>
			<select name="wl_mrate_x" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_mrate_x')">
				<option value="0" <% if tcWebApi_get("WLan_Common","wl_mrate_x","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Auto","s")%></option>
			</select>
		</td>
	</tr>
<!--
<tr id="wl_mrate_select">
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 7);"><%tcWebApi_get("String_Entry","WC11b_MultiRateAll_in","s")%></a></th>
<td>
<select name="wl_mrate_x" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_mrate_x')">
<option value="0" selected>111Disable</option>
<option value="1" >1</option>
<option value="2" >2</option>
<option value="3" >5.5</option>
<option value="4" >6</option>
<option value="5" >9</option>
<option value="6" >11</option>
<option value="7" >12</option>
<option value="8" >18</option>
<option value="9" >24</option>
<option value="10" >36</option>
<option value="11" >48</option>
<option value="12" >54</option>
</select>
</td>
</tr>
-->
	<tr style="display:none;">
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 8);"><%tcWebApi_get("String_Entry","WC11b_DataRate_in","s")%></a></th>
		<td>
			<select name="wl_rateset" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_rateset')">
				<option value="15" <% if tcWebApi_get("WLan_Common","BasicRate","h") = "15" then asp_Write("selected") end if %>>11B/G Mixed</option>
				<option value="3" <% if tcWebApi_get("WLan_Common","BasicRate","h") = "3" then asp_Write("selected") end if %>>11B</option>
				<option value="351" <% if tcWebApi_get("WLan_Common","BasicRate","h") = "351" then asp_Write("selected") end if %>>11G-Only</option>
			</select>
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 9);"><%tcWebApi_get("String_Entry","WC11b_x_Frag_in","s")%></a></th>
		<td>
			<input type="text" maxlength="4" name="wl_frag" id="wl_frag" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","FragThreshold","h") <> "" then  tcWebApi_get("WLan_Common","FragThreshold","s") else asp_Write("2346") end if %>" onKeyPress="return is_number(this,event)" onChange="page_changed()">
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 10);"><%tcWebApi_get("String_Entry","WC11b_x_RTS_in","s")%></a></th>
		<td>
			<input type="text" maxlength="4" name="wl_rts" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","RTSThreshold","h") <> "" then  tcWebApi_get("WLan_Common","RTSThreshold","s") else asp_Write("2347") end if %>" onKeyPress="return is_number(this,event)">
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(3, 11);"><%tcWebApi_get("String_Entry","WC11b_x_DTIM_in","s")%></a></th>
		<td>
			<input type="text" maxlength="3" name="wl_dtim" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","DtimPeriod","h") <> "" then  tcWebApi_get("WLan_Common","DtimPeriod","s") else asp_Write("1") end if %>" onKeyPress="return is_number(this,event)">
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 12);"><%tcWebApi_get("String_Entry","WC11b_x_Beacon_in","s")%></a></th>
		<td>
			<input type="text" maxlength="4" name="wl_bcn" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","BeaconPeriod","h") <> "" then  tcWebApi_get("WLan_Common","BeaconPeriod","s") else asp_Write("100") end if %>" onKeyPress="return is_number(this,event)">
		</td>
	</tr>
	<tr>
						<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 13);"><%tcWebApi_get("String_Entry","WC11b_x_TxBurst_in","s")%></a></th>
	<td>
	<select name="wl_frameburst" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_frameburst')">
	<option value="0" <% if tcWebApi_get("WLan_Common","TxBurst","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
	<option value="1" <% if tcWebApi_get("WLan_Common","TxBurst","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
	</select>
	</td>
	</tr>
	<tr id="PktAggregate"><th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 16);"><%tcWebApi_get("String_Entry","WC11b_x_PktAggregate_in","s")%></a></th>
	<td>
	<select name="wl_PktAggregate" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_PktAggregate')">
	<option value="0" <% if tcWebApi_get("WLan_Common","PktAggregate","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
	<option value="1" <% if tcWebApi_get("WLan_Common","PktAggregate","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
	</select>
	</td>
	</tr>
<!-- RaLink Only tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 19);"><%tcWebApi_get("String_Entry","WC11b_x_HT_OpMode_in","s")%></a></th>
<td>
<select id="wl_HT_OpMode" class="input_option" name="wl_HT_OpMode" onChange="return change_common(this, 'WLANConfig11b', 'wl_HT_OpMode')">
<option value="0" selected><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
<option value="1" ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
</select>
</div>
</td>
</tr-->
	<tr>
			  			<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(3, 14);"><%tcWebApi_get("String_Entry","WC11b_x_WMM_in","s")%></a></th>
		<td>
		<select name="wl_wme" id="wl_wme" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme')">
		<option value="1" <% if tcWebApi_get("WLan_Entry","WmmCapable","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
		<option value="0" <% if tcWebApi_get("WLan_Entry","WmmCapable","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
		</select>
		</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3,15);"><%tcWebApi_get("String_Entry","WC11b_x_NOACK_in","s")%></a></th>
	<td>
	<select name="wl_wme_no_ack" id="wl_wme_no_ack" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_wme_no_ack')">
	<option value="off" <% if tcWebApi_get("WLan_Common","wl_wme_no_ack","h") = "off" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
	<option value="on" <% if tcWebApi_get("WLan_Common","wl_wme_no_ack","h") = "on" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
	</select>
	</td>
	</tr>
<!--
<tr id="enable_wl_multicast_forward" style="display:none;">
<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> Wireless Multicast Forwarding</th>
<td>
<select name="wl_wmf_bss_enable" class="input_option">
<option value="0"  ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
<option value="1"  ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
</select>
</td>
</tr>
-->
	<tr>
						<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3,17);"><%tcWebApi_get("String_Entry","WC11b_x_APSD_in","s")%></a></th>
	<td>
	<select name="wl_wme_apsd" class="input_option" onchange="return change_common(this, 'WLANConfig11b', 'wl_wme_apsd')">
	<option value="0" <% if tcWebApi_get("WLan_Common","APSDCapable","h") = "0" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
	<option value="1" <% if tcWebApi_get("WLan_Common","APSDCapable","h") = "1" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
	</select>
	</td>
	</tr>

	<tr id="DLSCapable"> <th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3,18);"><%tcWebApi_get("String_Entry","WC11b_x_DLS_in","s")%></a></th>
	<td>
	<select name="wl_DLSCapable" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_DLSCapable')">
	<option value="0" <% if tcWebApi_get("WLan_Common","DLSCapable","h") = "0" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
	<option value="1" <% if tcWebApi_get("WLan_Common","DLSCapable","h") = "1" then asp_Write("selected") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
	</select>
	</td>
	</tr>

	<tr id="wl_txPower_field">
						<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 17);"><%tcWebApi_get("String_Entry","WC11b_TxPower_in","s")%></a></th>
						<td>
							<div>
								<table>
									<tr >
										<td style="border:0px;width:60px;">
											<input id="wl_txpower" name="wl_txpower" type="text" maxlength="3" class="input_3_table" value="<% If tcWebApi_get("WLan_Common","TxPower","h") <> "" then  tcWebApi_get("WLan_Common","TxPower","s") else asp_Write("100") end if %>" style="margin-left:-10px;" onkeyup="set_power(this.value);"> %
										</td>					
										<td style="border:0px;">
											<div id="slider" style="width:200px;"></div>
										</td>
									</tr>
								</table>
							</div>
						</td>
					</tr>
</table>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule();" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_WAdvanced_Content.asp-->
</html>
