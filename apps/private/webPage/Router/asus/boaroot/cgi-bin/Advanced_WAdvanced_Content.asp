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
	tcWebApi_Set("WLan_Entry","wl_sched0","wl_sched0")
	tcWebApi_Set("WLan_Entry","wl_sched1","wl_sched1")
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
	tcWebApi_Set("WLan_Common","location_code","location_code")
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
.parental_th{
	color:white;
	background:#2F3A3E;
	cursor: pointer;
	width:160px;
	height:22px;
	border-bottom:solid 1px black;
	border-right:solid 1px black;
} 
.parental_th:hover{
	background:rgb(94, 116, 124);
	cursor: pointer;
}

.checked{
	background-color:#9CB2BA;
	width:82px;
	border-bottom:solid 1px black;
	border-right:solid 1px black;
}

.disabled{
	width:82px;
	border-bottom:solid 1px black;
	border-right:solid 1px black;
	background-color:#475A5F;
}

  #selectable .ui-selecting { background: #FECA40; }
  #selectable .ui-selected { background: #F39814; color: white; }
  #selectable .ui-unselected { background: gray; color: green; }
  #selectable .ui-unselecting { background: green; color: black; }
  #selectable { border-spacing:0px; margin-left:0px;margin-top:0px; padding: 0px; width:100%;}
  #selectable td { height: 22px; }


</style>
<script>	
var $j = jQuery.noConflict();//var flag = 0;
	
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var flag = 0;
var enable_radio = "<% tcWebApi_get("WLan_Entry","radio_on","s") %>";
var wl_user_rssi_onload = "<% tcWebApi_get("WLan_Entry","wl_user_rssi","s") %>";

var array = new Array(7);
var clock_type = "";
var wifi_schedule_value = "<% tcWebApi_get("WLan_Entry","wl_sched0","s") %>";
wifi_schedule_value += "<% tcWebApi_get("WLan_Entry","wl_sched1","s") %>";

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
	init_array(array);
	init_cookie();
	count_time();
	load_body();	

	if(userRSSI_support > 0)
		changeRSSI(wl_user_rssi_onload);
	else
		document.getElementById("rssiTr").style.display = "none";

	if(!wl_info.band5g_support)
		document.getElementById("wl_unit_field").style.display = "none";

	document.getElementById("wl_rate").style.display = "none";

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
	corrected_timezone(DAYLIGHT_orig, TZ_orig);

	if(enable_radio == "1"){
		document.getElementById("wl_sched_enable").style.display = "";		
	}
	else{
		document.getElementById("wl_sched_enable").style.display = "none";
	}

	/*location_code Setting*/		
	if(location_list_support != -1){
		document.getElementById('region_tr').style.display = "";
		generate_region();
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
		
		split_wl_sched();	

		document.form.submit();
	}
}

function split_wl_sched(){
	document.form.wl_sched0.value = wifi_schedule_value.substring(0,500);
	document.form.wl_sched1.value = wifi_schedule_value.substring(500,1000);
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
document.getElementById("WAdvTable").deleteRow(i);
}
}

/* MODELDEP by Territory Code */
function generate_region(){
	
	var region_name = {
		"AA":"Asia",
		"AU":"Australia",
		"EU":"Europe"
	};	//Viz 2017.07.17	
	//var region_value = "AA/AU/EU";
	var region_value = "<% wl_support_region() %>"; //Viz mod 2017.07.17 
	var region_array = region_value.split("/");
	var current_region = '<% tcWebApi_get("WLan_Common","location_code","s") %>';
	if(current_region == '')
		current_region = ttc.split("/")[0];	

	free_options(document.form.location_code);
	for(var i=0;i<region_array.length;i++){
		if(current_region == region_array[i])
			add_option(document.form.location_code, region_name[region_array[i]], region_array[i], 1);
		else	
			add_option(document.form.location_code, region_name[region_array[i]], region_array[i], 0);
	}	
}

function adjust_tx_power(){
	var power_value_old = document.form.wl_TxPower_orig.value;
	var power_value_new = document.form.wl_TxPower_orig.value;
	var translated_value = 0;
	
	if(!power_support){
		document.getElementById("wl_txPower_field").style.display = "none";
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

			document.getElementById("slider").children[0].style.width = translated_value + "%";
			document.getElementById("slider").children[1].style.left = translated_value + "%";
			document.form.wl_txpower.value = translated_value;
		}
		else{
			document.getElementById("slider").children[0].style.width = power_value_new + "%";
			document.getElementById("slider").children[1].style.left = power_value_new + "%";
			document.form.wl_txpower.value = power_value_new;
		}
	}
}

function control_TimeField(){
	if(document.form.radio_on[0].checked){
		document.getElementById("wl_sched_enable").style.display = "";
		document.form.wl_schedule_radio.disabled = false;
		if(document.form.wl_schedule_radio[0].checked){
			document.getElementById("time_setting").style.display = "";
			document.form.wl_schedule_radio.disabled = false;
		}
		else{
			document.getElementById("time_setting").style.display = "none";
			document.form.wl_schedule_radio.disabled = true;
		}
	}
	else{
		document.getElementById("wl_sched_enable").style.display = "none";
		document.form.wl_schedule_radio.disabled = true;
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
	
	var array_temp = new Array(7);
	var checked = 0
	var unchecked = 0;
	init_array(array_temp);

  $j(function() {
    $j( "#selectable" ).selectable({
		filter:'td',
		selecting: function(event, ui){
					
		},
		unselecting: function(event, ui){
			
		},
		selected: function(event, ui){	
			id = ui.selected.getAttribute('id');
			column = parseInt(id.substring(0,1), 10);
			row = parseInt(id.substring(1,3), 10);	

			array_temp[column][row] = 1;
			if(array[column][row] == 1){
				checked = 1;
			}
			else if(array[column][row] == 0){
				unchecked = 1;
			}
		},
		unselected: function(event, ui){

		},		
		stop: function(event, ui){
			if((checked == 1 && unchecked == 1) || (checked == 0 && unchecked == 1)){
				for(i=0;i<7;i++){
					for(j=0;j<24;j++){
						if(array_temp[i][j] == 1){
						array[i][j] = array_temp[i][j];					
						array_temp[i][j] = 0;		//initialize
						if(j < 10){
							j = "0" + j;						
						}		
							id = i.toString() + j.toString();					
							document.getElementById(id).className = "checked";					
						}
					}
				}									
			}
			else if(checked == 1 && unchecked == 0){
				for(i=0;i<7;i++){
					for(j=0;j<24;j++){
						if(array_temp[i][j] == 1){
						array[i][j] = 0;					
						array_temp[i][j] = 0;
						
						if(j < 10){
							j = "0" + j;						
						}
							id = i.toString() + j.toString();											
							document.getElementById(id).className = "disabled";												
						}
					}
				}			
			}
		
			checked = 0;
			unchecked = 0;
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
	
	document.getElementById("slider").children[0].style.width = power_value + "%";
	document.getElementById("slider").children[1].style.left = power_value + "%";
	document.form.wl_txpower.value = power_value;	
}

function init_array(arr){
	for(i=0;i<7;i++){
		arr[i] = new Array(24);

		for(j=0;j<24;j++){
			arr[i][j] = 0;
		}
	}
}

function init_cookie(){
	if(document.cookie.indexOf('clock_type') == -1)		//initialize
		document.cookie = "clock_type=1";		
			
	x = document.cookie.split(';');
	for(i=0;i<x.length;i++){
		if(x[i].indexOf('clock_type') != -1){
			clock_type = x[i].substring(x[i].length-1, x[i].length);			
		}	
	}
}

//draw time slot at first time
function redraw_selected_time(obj){
	var start_day = 0;
	var end_day = 0;
	var start_time = "";
	var end_time = "";
	var time_temp = "";
	var duration = "";
	var id = "";
	
	for(i=0;i<obj.length;i++){
		time_temp = obj[i];
		start_day = parseInt(time_temp.substring(0,1), 10);
		end_day =  parseInt(time_temp.substring(1,2), 10);
		start_time =  parseInt(time_temp.substring(2,4), 10);
		end_time =  parseInt(time_temp.substring(4,6), 10);
		if((start_day == end_day) && (end_time - start_time) < 0)	//for Sat 23 cross to Sun 00
			end_day = 7;

		if(start_day == end_day){			// non cross day
			duration = end_time - start_time;
			if(duration == 0)	//for whole selected
				duration = 7*24;
			
			while(duration >0){
				array[start_day][start_time] = 1;
				if(start_time < 10)
					start_time = "0" + start_time;
								
				id = start_day.toString() + start_time.toString();
				document.getElementById(id).className = "checked";
				start_time++;
				if(start_time == 24){
					start_time = 0;
					start_day++;
					if(start_day == 7)
						start_day = 0;
				}
	
				duration--;
				id = "";		
			}	
		}else{			// cross day
			var duration_day = 0;
			if(end_day - start_day < 0)
				duration_day = 7 - start_day;
			else
				duration_day = end_day - start_day;
		
			duration = (24 - start_time) + (duration_day - 1)*24 + end_time;
			while(duration > 0){
				array[start_day][start_time] = 1;
				if(start_time < 10)
					start_time = "0" + start_time;
				
				id = start_day.toString() + start_time.toString();
				document.getElementById(id).className = "checked";
				start_time++;
				if(start_time == 24){
					start_time = 0;
					start_day++;
					if(start_day == 7)
						start_day = 0;		
				}
				
				duration--;
				id = "";	
			}		
		}	
	}
}

function select_all(){
	var full_flag = 1;
	for(i=0;i<7;i++){
		for(j=0;j<24;j++){
			if(array[i][j] ==0){ 
				full_flag = 0;
				break;
			}
		}
		
		if(full_flag == 0){
			break;
		}
	}

	if(full_flag == 1){
		for(i=0;i<7;i++){
			for(j=0;j<24;j++){
				array[i][j] = 0;
				if(j<10){
					j = "0"+j;
				}
		
				id = i.toString() + j.toString();
				document.getElementById(id).className = "disabled";
			}
		}
	}
	else{
		for(i=0;i<7;i++){
			for(j=0;j<24;j++){
				if(array[i][j] == 1)
					continue;
				else{	
					array[i][j] = 1;
					if(j<10){
						j = "0"+j;
					}
			
					id = i.toString() + j.toString();
					document.getElementById(id).className = "checked";
				}
			}
		}
	}
}

function select_all_day(day){
	var check_flag = 0
	day = day.substring(4,5);
	for(i=0;i<24;i++){
		if(array[day][i] == 0){
			check_flag = 1;			
		}			
	}
	
	if(check_flag == 1){
		for(j=0;j<24;j++){
			array[day][j] = 1;
			if(j<10){
				j = "0"+j;
			}
		
			id = day + j;
			document.getElementById(id).className = "checked";	
		}
	}
	else{
		for(j=0;j<24;j++){
			array[day][j] = 0;
			if(j<10){
				j = "0"+j;
			}
		
			id = day + j;
			document.getElementById(id).className = "disabled";	
		}
	}
}

function select_all_time(time){
	var check_flag = 0;
	time_int = parseInt(time, 10);	
	for(i=0;i<7;i++){
		if(array[i][time] == 0){
			check_flag = 1;			
		}			
	}
	
	if(time<10){
		time = "0"+time;
	}

	if(check_flag == 1){
		for(i=0;i<7;i++){
			array[i][time_int] = 1;
			
			id = i + time;
			document.getElementById(id).className = "checked";
		}
	}
	else{
		for(i=0;i<7;i++){
			array[i][time_int] = 0;

			id = i + time;
			document.getElementById(id).className = "disabled";
		}
	}
}

function change_clock_type(type){
	document.cookie = "clock_type="+type;
	if(type == 1)
		var array_time = ["00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"];
	else
		var array_time = ["12", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"];

	for(i=0;i<24;i++){
		if(type == 1)
			document.getElementById(i).innerHTML = array_time[i] +" ~ "+ array_time[i+1];
		else{
			if(i<11 || i == 23)
				document.getElementById(i).innerHTML = array_time[i] +" ~ "+ array_time[i+1] + " AM";
			else
				document.getElementById(i).innerHTML = array_time[i] +" ~ "+ array_time[i+1] + " PM";
		}	
	}
}


function save_wifi_schedule(){
	var flag = 0;
	var start_day = 0;
	var end_day = 0;
	var start_time = 0;
	var end_time = 0;
	var time_temp = "";
	
	for(i=0;i<7;i++){
		for(j=0;j<24;j++){
			if(array[i][j] == 1){
				if(flag == 0){
					flag =1;
					start_day = i;
					if(j<10)
						j = "0" + j;
						
					start_time = j;				
				}
			}
			else{
				if(flag == 1){
					flag =0;
					end_day = i;
					if(j<10)
						j = "0" + j;
					
					end_time = j;		
					if(time_temp != "")
						time_temp += "<";
				
					time_temp += start_day.toString() + end_day.toString() + start_time.toString() + end_time.toString();
				}
			}
		}	
	}
	
	if(flag == 1){
		if(time_temp != "")
			time_temp += "<";
									
		time_temp += start_day.toString() + "0" + start_time.toString() + "00";	
	}

	if(time_temp == ""){
		alert("If you want to deny WiFi radio all time, you should check the '<%tcWebApi_get("String_Entry","WC11b_x_RadioEnable_in","s")%>' to '<%tcWebApi_get("String_Entry","checkbox_No","s")%>'");
		return false;
	}
	
	wifi_schedule_value = time_temp;
	document.getElementById("schedule_block").style.display = "none";
	document.getElementById("titl_desc").style.display = "";
	document.getElementById("WAdvTable").style.display = "";
	document.getElementById("apply_btn").style.display = "";
}

function cancel_wifi_schedule(client){
	init_array(array);
	document.getElementById("schedule_block").style.display = "none";
	document.getElementById("titl_desc").style.display = "";
	document.getElementById("WAdvTable").style.display = "";
	document.getElementById("apply_btn").style.display = "";
}

function count_time(){		// To count system time
	systime_millsec += 1000;
	setTimeout("count_time()", 1000);
}

function show_wifi_schedule(){
	document.getElementById("titl_desc").style.display = "none";
	document.getElementById("WAdvTable").style.display = "none";
	document.getElementById("apply_btn").style.display = "none";
	document.getElementById("schedule_block").style.display = "";

	var array_date = ["Select All", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
	var array_time_id = ["00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"];
	if(clock_type == "1")
		var array_time = ["00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"];
	else
		var array_time = ["12am", "1am", "2am", "3am", "4am", "5am", "6am", "7am", "8am", "9am", "10am", "11am", "12pm", "1pm", "2pm", "3pm", "4pm", "5pm", "6pm", "7pm", "8pm", "9pm", "10pm", "11pm", "12am"];
	
	var code = "";
	
	wifi_schedule_row = wifi_schedule_value.split('<');

	code +='<div style="margin-bottom:10px;color: #003399;font-family: Verdana;" align="left">';
	code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="FormTable">';
	code +='<thead><tr><td colspan="6" id="LWFilterList"><%tcWebApi_get("String_Entry","ParentalCtrl_Act_schedule","s")%></td></tr></thead>';
	code +='<tr><th style="width:40%;height:20px;" align="right"><%tcWebApi_get("String_Entry","General_x_SystemTime_in","s")%></th>';	
	code +='<td align="left" style="color:#FFF"><input type="text" id="system_time" name="system_time" class="devicepin" value="" readonly="1" style="font-size:12px;width:200px;"></td></tr>';		
	code +='</table><table id="main_select_table">';
	code +='<table  id="selectable" class="table_form" >';
	code += "<tr>";
	for(i=0;i<8;i++){
		if(i == 0)
			code +="<th class='parental_th' onclick='select_all();'>"+array_date[i]+"</th>";	
		else
			code +="<th id=col_"+(i-1)+" class='parental_th' onclick='select_all_day(this.id);'>"+array_date[i]+"</th>";			
	}
	
	code += "</tr>";
	for(i=0;i<24;i++){
		code += "<tr>";
		code +="<th id="+i+" class='parental_th' onclick='select_all_time(this.id)'>"+ array_time[i] + " ~ " + array_time[i+1] +"</th>";
		for(j=0;j<7;j++){
			code += "<td id="+ j + array_time_id[i] +" class='disabled' ></td>";		
		}
		
		code += "</tr>";			
	}
	
	code +='</table></table></div>';
	document.getElementById("mainTable").innerHTML = code;

	register_event();
	redraw_selected_time(wifi_schedule_row);
	
	var code_temp = "";
	code_temp = '<table style="width:350px;margin-left:20px;"><tr>';
	code_temp += "<td><div style=\"width:95px;font-family:Arial,sans-serif,Helvetica;font-size:18px;\"><%tcWebApi_get("String_Entry","Clock_Format","s")%></div></td>";
	code_temp += '<td><div>';
	code_temp += '<select id="clock_type_select" class="input_option" onchange="change_clock_type(this.value);">';
	code_temp += '<option value="0" >12-hour</option>';
	code_temp += '<option value="1" >24-hour</option>';
	code_temp += '</select>';
	code_temp += '</div></td>';
	code_temp += '<td><div align="left" style="font-family:Arial,sans-serif,Helvetica;font-size:18px;margin:0px 5px 0px 30px;">Allow</div></td>';
	code_temp += '<td><div style="width:90px;height:20px;background:#9CB2BA;"></div></td>';
	code_temp += '<td><div align="left" style="font-family:Arial,sans-serif,Helvetica;font-size:18px;margin:0px 5px 0px 30px;">Deny</div></td>';
	code_temp += '<td><div style="width:90px;height:20px;border:solid 1px #000"></div></td>';
	code_temp += '</tr></table>';
	document.getElementById("hintBlock").innerHTML = code_temp;
	document.getElementById("hintBlock").style.marginTop = "10px";
	document.getElementById("hintBlock").style.display = "";
	document.getElementById("ctrlBtn").innerHTML = '<input class="button_gen" type="button" onClick="cancel_wifi_schedule();" value="<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>">';
	document.getElementById("ctrlBtn").innerHTML += '<input class="button_gen" type="button" onClick="save_wifi_schedule();" value="<%tcWebApi_get("String_Entry","CTL_ok","s")%>">';  
	document.getElementById("clock_type_select")[clock_type].selected = true;		// set clock type by cookie
	
	document.getElementById("mainTable").style.display = "";
	$j("#mainTable").fadeIn();
	showclock();
}

function showclock(){
	JS_timeObj.setTime(systime_millsec);
	JS_timeObj2 = JS_timeObj.toString();	
	JS_timeObj2 = JS_timeObj2.substring(0,3) + ", " +
	              JS_timeObj2.substring(4,10) + "  " +
				  checkTime(JS_timeObj.getHours()) + ":" +
				  checkTime(JS_timeObj.getMinutes()) + ":" +
				  checkTime(JS_timeObj.getSeconds()) + "  " +
				  JS_timeObj.getFullYear();
	document.getElementById("system_time").value = JS_timeObj2;
	setTimeout("showclock()", 1000);
		
	corrected_timezone(DAYLIGHT_orig, TZ_orig);
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
<input type="hidden" name="wl_sched0" value="<% tcWebApi_staticGet("WLan_Entry","wl_sched0","s") %>">
<input type="hidden" name="wl_sched1" value="<% tcWebApi_staticGet("WLan_Entry","wl_sched1","s") %>">
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
		 				<div id="titl_desc" class="formfontdesc"><%tcWebApi_get("String_Entry","WC11b_display5_sd","s")%></div>
		 				<div id="timezone_hint" onclick="location.href='Advanced_System_Content.asp?af=uiViewdateToolsTZ'" style="display:none;margin-left:4px;color:#FFCC00;text-decoration:underline;cursor:pointer;"></div>
		 				
			<div id="schedule_block" style="display:none">
				<div id="hintBlock" style="width: 650px; margin-top: 10px;">
					<table style="width:350px;"><tbody><tr><td><div style="width:95px;font-family:Arial,sans-serif,Helvetica;font-size:18px;">Clock Format</div></td><td><div><select id="clock_type_select" class="input_option" onchange="change_clock_type(this.value);"><option value="0">12-hour</option><option value="1">24-hour</option></select></div></td><td><div align="left" style="font-family:Arial,sans-serif,Helvetica;font-size:18px;margin:0px 5px 0px 30px;">Allow</div></td><td><div style="width:90px;height:20px;background:#9CB2BA;"></div></td><td><div align="left" style="font-family:Arial,sans-serif,Helvetica;font-size:18px;margin:0px 5px 0px 30px;">Deny</div></td><td><div style="width:90px;height:20px;border:solid 1px #000"></div></td></tr></tbody></table>
				</div>
				<div id="mainTable" style="padding:0 20px 20px 20px;"></div>
				<div id="ctrlBtn" style="text-align:center;"></div>
			</div>
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
	<input type="radio" value="1" name="radio_on" class="input" onClick="control_TimeField();" <% if tcWebApi_get("WLan_Entry","radio_on","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
	<input type="radio" value="0" name="radio_on" class="input" onClick="control_TimeField();" <% if tcWebApi_get("WLan_Entry","radio_on","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
	</td>
	</tr>
	<tr id="wl_sched_enable">
	<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 22);"><%tcWebApi_get("String_Entry","WC11b_x_RadioEnable_scheduler","s")%></a></th>
	<td>
	<input type="radio" value="1" name="wl_schedule_radio" class="input" onClick="control_TimeField();" <% if tcWebApi_get("WLan_Entry","wl_schedule_radio","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
	<input type="radio" value="0" name="wl_schedule_radio" class="input" onClick="control_TimeField();" <% if tcWebApi_get("WLan_Entry","wl_schedule_radio","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
	<span id="time_setting" style="padding-left:20px;cursor:pointer;text-decoration:underline" onclick="show_wifi_schedule();">Time Setting</span>
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
			<input type="text" maxlength="4" name="wl_frag" id="wl_frag" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","FragThreshold","h") <> "" then  tcWebApi_get("WLan_Common","FragThreshold","s") else asp_Write("2346") end if %>" onKeyPress="return validator.isNumber(this,event)" onChange="page_changed()">
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 10);"><%tcWebApi_get("String_Entry","WC11b_x_RTS_in","s")%></a></th>
		<td>
			<input type="text" maxlength="4" name="wl_rts" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","RTSThreshold","h") <> "" then  tcWebApi_get("WLan_Common","RTSThreshold","s") else asp_Write("2347") end if %>" onKeyPress="return validator.isNumber(this,event)">
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(3, 11);"><%tcWebApi_get("String_Entry","WC11b_x_DTIM_in","s")%></a></th>
		<td>
			<input type="text" maxlength="3" name="wl_dtim" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","DtimPeriod","h") <> "" then  tcWebApi_get("WLan_Common","DtimPeriod","s") else asp_Write("1") end if %>" onKeyPress="return validator.isNumber(this,event)">
		</td>
	</tr>
	<tr>
			  			<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(3, 12);"><%tcWebApi_get("String_Entry","WC11b_x_Beacon_in","s")%></a></th>
		<td>
			<input type="text" maxlength="4" name="wl_bcn" class="input_6_table" value="<% If tcWebApi_get("WLan_Common","BeaconPeriod","h") <> "" then  tcWebApi_get("WLan_Common","BeaconPeriod","s") else asp_Write("100") end if %>" onKeyPress="return validator.isNumber(this,event)">
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
						<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 16);"><%tcWebApi_get("String_Entry","WC11b_TxPower_in","s")%></a></th>
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
					<tr id="region_tr" style="display:none">
						<th><a class="hintstyle" href="javascript:void(0);" onClick=""><%tcWebApi_get("String_Entry","WC11b_x_Region","s")%></a></th>
						<td><select name="location_code" class="input_option"></select></td>
					</tr>
</table>
<div class="apply_gen" id="apply_btn">
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
