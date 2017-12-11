<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<% tcWebApi_set("Wan_Common","TransMode","USB") %>
<% tcWebApi_set("WebCurSet_Entry","wan_pvc","11") %>

<% disable_other_wan()%>
<%
If Request_Form("DvInfo_PVC") <> "" Then
	tcWebApi_set("WebCurSet_Entry","dev_pvc","DvInfo_PVC")
End If

If request_Form("wanTransFlag") = "1" Then
	tcWebApi_set("Wan_Common","TransMode","wan_TransMode")
	tcWebApi_set("WebCurSet_Entry","wan_pvc","ptm_VC")
End If

If Request_Form("ModemSaveFlag")="1" Then
	tcWebApi_set("Wan_PVC11","Active","PVC11_active")
	tcWebApi_set("Wan_PVC11","DEFAULTROUTE","PVC11_DefaultRoute")
	If should_wan_pvc_do_commit("DvInfo_PVC") = "1" Then
		tcWebApi_commit("Wan_PVC11")
	End If
	tcWebApi_set("USBModem_Entry","modem_enable","modem_enable")
	tcWebApi_set("USBModem_Entry","modem_android","modem_android")
	tcWebApi_set("USBModem_Entry","modem_autoapn","modem_autoapn")
	tcWebApi_set("USBModem_Entry","modem_country","modem_country")
	tcWebApi_set("USBModem_Entry","modem_isp","modem_isp")
	tcWebApi_set("USBModem_Entry","modem_apn","modem_apn")
	tcWebApi_set("USBModem_Entry","modem_dialnum","modem_dialnum")
	tcWebApi_set("USBModem_Entry","modem_pincode","modem_pincode")
	tcWebApi_set("USBModem_Entry","modem_user","modem_user")
	tcWebApi_set("USBModem_Entry","modem_pass","modem_pass")
	tcWebApi_set("USBModem_Entry","Dev3G","Dev3G")
	If should_wan_pvc_do_commit("DvInfo_PVC") = "1" Then
		tcWebApi_commit("USBModem_Entry")
	End If	
End If
%>
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_Modem_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_4_4","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<style>
#ClientList_Block_PC{
	border:1px outset #999;
	background-color:#576D73;
	position:absolute;
	*margin-top:26px;	
	margin-left:2px;
	*margin-left:-189px;
	width:181px;
	text-align:left;	
	height:auto;
	overflow-y:auto;
	z-index:200;
	padding: 1px;
	display:none;
}
#ClientList_Block_PC div{
	background-color:#576D73;
	height:auto;
	*height:20px;
	line-height:20px;
	text-decoration:none;
	font-family: Lucida Console;
	padding-left:2px;
}

#ClientList_Block_PC a{
	background-color:#EFEFEF;
	color:#FFF;
	font-size:12px;
	font-family:Arial, Helvetica, sans-serif;
	text-decoration:none;	
}
#ClientList_Block_PC div:hover, #ClientList_Block a:hover{
	background-color:#3366FF;
	color:#FFFFFF;
	cursor:default;
}	
</style>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/wcdma_list.js"></script>
<script type="text/javaScript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script>
var $j = jQuery.noConflict();

function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
var usb_modem_enable = 0;
var modem_android_orig = '<% tcWebApi_get("USBModem_Entry","modem_android","s")%>';
var modem_card = '<% tcWebApi_get("USBModem_Entry","Dev3G","s")%>';
var country = '<% tcWebApi_get("USBModem_Entry","modem_country","s")%>';
var isp = '<% tcWebApi_get("USBModem_Entry","modem_isp","s")%>';
var apn = '<% tcWebApi_get("USBModem_Entry","modem_apn","s")%>';
var dialnum = '<% tcWebApi_get("USBModem_Entry","modem_dialnum","s")%>';
var user = '<% tcWebApi_get("USBModem_Entry","modem_user","s")%>';
var pass = '<% tcWebApi_get("USBModem_Entry","modem_pass","s")%>';
var pin_opt = '<% tcWebApi_get("USBModem_Entry","modem_pincode_opt","s")%>';
var modemlist = new Array();
var countrylist = new Array();
var protolist = new Array();
var isplist = new Array();
var apnlist = new Array();
var daillist = new Array();
var userlist = new Array();
var passlist = new Array();
var wans_dualwan = '<% tcWebApi_get("Dualwan_Entry","wans_dualwan","s") %>';
var wans_flag = (wans_dualwan.search("none") == -1) ? 1:0;

usb_modem_enable = ('<% tcWebApi_get("USBModem_Entry","modem_enable","s")%>' != "0")? 1:0;

function initial(){
	show_menu();
	if(dualWAN_support != "-1" && wans_flag)
		genWANSoption();				

	reloadProfile();

	document.form.modem_android.value = modem_android_orig;
	if(usb_modem_enable){
		document.getElementById("modem_android_tr").style.display="";
		if(modem_android_orig == "0"){
			switch_modem_mode('<% tcWebApi_get("USBModem_Entry","modem_enable","s")%>');
			gen_country_list();
			reloadProfile();
			inputCtrl(document.form.modem_autoapn, 1);
			change_apn_mode();
		}
		else{
			hide_usb_settings(1);
			document.getElementById("android_desc").style.display="";
		}
	}
	else{
		hide_usb_settings();
	}

	$j('#usb_modem_switch').iphoneSwitch(usb_modem_enable,
		function() {
			if(dualWAN_support)
				document.form.wans_dualwan.value = wans_dualwan_array[0]+" usb";
			document.getElementById("modem_android_tr").style.display="";
			document.form.modem_enable.value = "1";
			if(document.form.modem_android.value == "0"){
				switch_modem_mode(document.form.modem_enable.value);
				document.getElementById("android_desc").style.display="none";
				gen_country_list();
				reloadProfile();
				inputCtrl(document.form.modem_autoapn, 1);
				//inputCtrl(document.form.modem_android, 1);
				change_apn_mode();
			}
			else{
				document.getElementById("android_desc").style.display="";
				hide_usb_settings(1);
			}
		},
		function() {
			document.form.modem_enable.value = "0";
			if(dualWAN_support){
				if(usb_index == 0)
						document.form.wans_dualwan.value = wans_dualwan_array[1]+" none";
					else
						document.form.wans_dualwan.value = wans_dualwan_array[0]+" none";
			}
			document.getElementById("modem_android_tr").style.display="none";
			hide_usb_settings();
		}
	);

		if(wimax_support == -1){
			for (var i = 0; i < document.form.modem_enable_option.options.length; i++) {
					if (document.form.modem_enable_option.options[i].value == "4") {
							document.form.modem_enable_option.options.remove(i);
							break;
					}
			}
		}		
		
		check_dongle_status();
}

function patchWANSoption(){
  var opt4 = document.createElement('option');
  opt4.value = "USB";
  opt4.innerHTML = "USB Modem";
  opt4.selected = true;
  document.form.wan_TransMode.appendChild(opt4);	
}

// "ATM"	ADSL WAN (ATM)
// "PTM"	VDSL WAN (PTM)
// "Ethernet"		Ethernet WAN
// "USB"	USB Modem	selected>
function genWANSoption(){
		free_options(document.form.wan_TransMode);
		for(i=0; i<wans_dualwan.split(" ").length; i++){
			if(wans_dualwan.split(" ")[i].toUpperCase() == "DSL"){
			<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
				var opt = document.createElement('option');
    				opt.value = "ATM";
    				opt.innerHTML = "ADSL WAN (ATM)";
    				document.form.wan_TransMode.appendChild(opt);
    			<%end if%>
    			<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
    				var opt1 = document.createElement('option');
    				opt1.value = "PTM";
    				opt1.innerHTML = "VDSL WAN (PTM)";
				document.form.wan_TransMode.appendChild(opt1);
			<%end if%>
    			}

	    		if(wans_dualwan.split(" ")[i].toUpperCase() == "WAN"){
    				var opt2 = document.createElement('option');
    				opt2.value = "Ethernet";
    				opt2.innerHTML = "Ethernet WAN";
    				document.form.wan_TransMode.appendChild(opt2);
    			}

	    		if(wans_dualwan.split(" ")[i].toUpperCase() == "USB"){
    				var opt3 = document.createElement('option');
    				opt3.value = "USB";
    				opt3.innerHTML = "USB Modem";
    				opt3.selected = true;
    				document.form.wan_TransMode.appendChild(opt3);    			
    			}
			
			if(wans_dualwan.split(" ")[i].toUpperCase() == "LAN"){
				var opt4 = document.createElement('option');
				opt4.value = "LAN";
				opt4.innerHTML = "Ethernet WAN";
				document.form.wan_TransMode.appendChild(opt4);
			}
		}

		if(wans_dualwan.split(" ")[0].toUpperCase() != "USB" && wans_dualwan.split(" ")[1].toUpperCase() != "USB"){
			patchWANSoption();	
		}
}

function reloadProfile(){
	if(document.form.modem_enable.value == 0)
		return 0;

	gen_list();
	show_ISP_list();
	show_APN_list();
}

function show_modem_list(mode){
		if(mode == "4")
			show_4G_modem_list();
		else
			show_3G_modem_list();
}

function show_3G_modem_list(){
	modemlist = new Array(
			"AUTO"
			, "ASUS-T500"
			, "BandLuxe-C120"
			, "BandLuxe-C170"
			, "BandLuxe-C339"
			, "Huawei-E1550"
			, "Huawei-E160G"
			, "Huawei-E161"
			, "Huawei-E169"
			, "Huawei-E176"
			, "Huawei-E220"
			, "Huawei-K3520"
			, "Huawei-ET128"
			, "Huawei-E1800"
			, "Huawei-K4505"
			, "Huawei-E172"
			, "Huawei-E372"
			, "Huawei-E122"
			, "Huawei-E160E"
			, "Huawei-E1552"
			, "Huawei-E173"
			, "Huawei-E1823"
			, "Huawei-E1762"
			, "Huawei-E1750C"
			, "Huawei-E1752Cu"
			//, "MU-Q101"
			, "Alcatel-X200"
			, "Alcatel-Oune-touch-X220S"
			, "AnyData-ADU-510A"
			, "AnyData-ADU-500A"
			, "Onda-MT833UP"
			, "Onda-MW833UP"
			, "MTS-AC2746"
			, "ZTE-AC5710"
			, "ZTE-MU351"
			, "ZTE-MF100"
			, "ZTE-MF636"
			, "ZTE-MF622"
			, "ZTE-MF626"
			, "ZTE-MF632"
			, "ZTE-MF112"
			, "ZTE-MFK3570-Z"
			, "CS15"
			, "CS17"
			, "ICON401"
			);

	free_options($("shown_modems"));
	for(var i = 0; i < modemlist.length; i++){
		if(modemlist[i] == "AUTO")
			$("shown_modems").options[i] = new Option("<%tcWebApi_get("String_Entry","Auto","s")%>", modemlist[i]);
		else	
			$("shown_modems").options[i] = new Option(modemlist[i], modemlist[i]);
			
		if(modemlist[i] == modem_card)
			$("shown_modems").options[i].selected = "1";
	}
}

function show_4G_modem_list(){
	modemlist = new Array(
		"AUTO", 
		"Samsung U200"
	);

	free_options($("shown_modems"));
	for(var i = 0; i < modemlist.length; i++){
		if(modemlist[i] == "AUTO")
			$("shown_modems").options[i] = new Option("<%tcWebApi_get("String_Entry","Auto","s")%>", modemlist[i]);
		else			
			$("shown_modems").options[i] = new Option(modemlist[i], modemlist[i]);

		if(modemlist[i] == modem)
			$("shown_modems").options[i].selected = "1";
	}
}
function switch_modem_mode(mode){
	document.form.modem_enable.value = mode;
	show_modem_list(mode);

	if(mode == "1"){ // WCDMA
		inputCtrl(document.form.Dev3G, 1);
		inputCtrl(document.form.modem_country, 1);
		inputCtrl(document.form.modem_isp, 1);
		inputCtrl(document.form.modem_apn, 1);
		if(pin_opt) inputCtrl(document.form.modem_pincode, 1);
		inputCtrl(document.form.modem_dialnum, 1);
		inputCtrl(document.form.modem_user, 1);
		inputCtrl(document.form.modem_pass, 1);
		inputCtrl(document.form.modem_ttlsid, 0);
		//$("hsdpa_hint").style.display = "";
	}
	else if(mode == "2"){ // CDMA2000
		inputCtrl(document.form.Dev3G, 1);
		inputCtrl(document.form.modem_country, 1);
		inputCtrl(document.form.modem_isp, 1);
		inputCtrl(document.form.modem_apn, 1);
		if(pin_opt) inputCtrl(document.form.modem_pincode, 1);
		inputCtrl(document.form.modem_dialnum, 1);
		inputCtrl(document.form.modem_user, 1);
		inputCtrl(document.form.modem_pass, 1);
		inputCtrl(document.form.modem_ttlsid, 0);
		//$("hsdpa_hint").style.display = "";
	}
	else if(mode == "3"){ // TD-SCDMA
		inputCtrl(document.form.Dev3G, 1);
		inputCtrl(document.form.modem_country, 1);
		inputCtrl(document.form.modem_isp, 1);
		inputCtrl(document.form.modem_apn, 1);
		if(pin_opt) inputCtrl(document.form.modem_pincode, 1);
		inputCtrl(document.form.modem_dialnum, 1);
		inputCtrl(document.form.modem_user, 1);
		inputCtrl(document.form.modem_pass, 1);
		inputCtrl(document.form.modem_ttlsid, 0);
		//$("hsdpa_hint").style.display = "";
	}
	else if(mode == "4"){	// WiMAX
		inputCtrl(document.form.Dev3G, 1);
		inputCtrl(document.form.modem_country, 1);
		inputCtrl(document.form.modem_isp, 1);
		inputCtrl(document.form.modem_apn, 0);
		if(pin_opt) inputCtrl(document.form.modem_pincode, 1);
		inputCtrl(document.form.modem_dialnum, 0);
		inputCtrl(document.form.modem_user, 1);
		inputCtrl(document.form.modem_pass, 1);
		inputCtrl(document.form.modem_ttlsid, 1);
		//$("hsdpa_hint").style.display = "";
	}
	else{	// Disable (mode == 0)
		inputCtrl(document.form.Dev3G, 0);
		inputCtrl(document.form.modem_enable_option, 0);
		inputCtrl(document.form.modem_country, 0);
		inputCtrl(document.form.modem_isp, 0);
		inputCtrl(document.form.modem_apn, 0);
		if(pin_opt) inputCtrl(document.form.modem_pincode, 0);
		inputCtrl(document.form.modem_dialnum, 0);
		inputCtrl(document.form.modem_user, 0);
		inputCtrl(document.form.modem_pass, 0);
		inputCtrl(document.form.modem_ttlsid, 0);
		//$("hsdpa_hint").style.display = "none";
		document.form.modem_enable.value = "0";
	}
}

function show_ISP_list(){
	var removeItem = 0;
	free_options(document.form.modem_isp);
	document.form.modem_isp.options.length = isplist.length;

	for(var i = 0; i < isplist.length; i++){
	  if(protolist[i] == 4 && wimax_support == -1){
			document.form.modem_isp.options.length = document.form.modem_isp.options.length - 1;

			if(document.form.modem_isp.options.length > 0)
				continue;
			else{
				alert(Untranslated.ISP_not_support);
				document.form.modem_country.focus();
				document.form.modem_country.selectedIndex = countrylist.length-1;
				break;
			}
		}
		else
			document.form.modem_isp.options[i] = new Option(isplist[i], isplist[i]);

		if(isplist[i] == isp)
			document.form.modem_isp.options[i].selected = 1;
	}
}

function show_APN_list(){
	var ISPlist = document.form.modem_isp.value;
	var Countrylist = document.form.modem_country.value;

	var isp_order = -1;
	for(isp_order = 0; isp_order < isplist.length; ++isp_order){
		if(isplist[isp_order] == ISPlist)
			break;
		else if(isp_order == isplist.length-1){
			isp_order = -1;
			break;
		}
	}

	if(isp_order == -1){
		alert("system error");
		return;
	}
	
	/* use manual or location */
	if(document.form.modem_country.value == ""){
		inputCtrl(document.form.modem_isp, 0);
		inputCtrl(document.form.modem_enable_option, 1);
	}
	else{
		inputHideCtrl(document.form.modem_isp, 1);
		inputHideCtrl(document.form.modem_enable_option, 0);
		if(protolist[isp_order] == "")
			protolist[isp_order] = 1;
	}

	if(Countrylist == ""){
		if('<% tcWebApi_get("USBModem_Entry","modem_enable","s") %>' == $('modem_enable_option').value){
			$("modem_apn").value = apn;
			$("modem_dialnum").value = dialnum;
			$("modem_user").value = user;
			$("modem_pass").value = pass;
		}
		else{
			$("modem_apn").value = apnlist[isp_order];
			$("modem_dialnum").value = daillist[isp_order];
			$("modem_user").value = userlist[isp_order];
			$("modem_pass").value = passlist[isp_order];
		}
	}
	else if(protolist[isp_order] != "4"){
		if(ISPlist == isp && Countrylist == country && (apn != "" || dialnum != "" || user != "" || pass != "")){
			if(typeof(apnlist[isp_order]) == 'object' && apnlist[isp_order].constructor == Array){
				//Viz temp $("pull_arrow").style.display = '';
				showLANIPList(isp_order);
			}
			else{
				//Viz temp $("pull_arrow").style.display = 'none';
				//Viz temp $('ClientList_Block_PC').style.display = 'none';
			}

			$("modem_apn").value = apn;
			$("modem_dialnum").value = dialnum;
			$("modem_user").value = user;
			$("modem_pass").value = pass;
		}
		else{
			if(typeof(apnlist[isp_order]) == 'object' && apnlist[isp_order].constructor == Array){
				//Viz temp $("pull_arrow").style.display = '';
				showLANIPList(isp_order);
			}
			else{
				//Viz temp $("pull_arrow").style.display = 'none';
				//Viz temp $('ClientList_Block_PC').style.display = 'none';
				$("modem_apn").value = apnlist[isp_order];
			}

			$("modem_dialnum").value = daillist[isp_order];
			$("modem_user").value = userlist[isp_order];
			$("modem_pass").value = passlist[isp_order];
		}
	}
	else{
		$("modem_apn").value = "";
		$("modem_dialnum").value = "";

		if(ISPlist == isp	&& (user != "" || pass != "")){
			$("modem_user").value = user;
			$("modem_pass").value = pass;
		}
		else{
			$("modem_user").value = userlist[isp_order];
			$("modem_pass").value = passlist[isp_order];
		}
	}

	if(document.form.modem_country.value != ""){
		document.form.modem_enable.value = protolist[isp_order];
		switch_modem_mode(document.form.modem_enable.value);
	}
}

function applyRule(){
	if(validForm()){
		if(document.form.modem_enable.value == "0")
			document.form.PVC11_active.value = "No";
		else
			document.form.PVC11_active.value = "Yes";
		
		if(document.form.modem_pincode.value != "") {
			if(document.form.modem_pincode.value.search(/^\d{4,8}$/)==-1) {
				alert("<%tcWebApi_get("String_Entry","JS_InvalidPIN","s")%>");
				return;
			}
		}
		document.form.wanTransFlag.value = 1;
		document.form.ModemSaveFlag.value = 1;
		document.form.wanVCFlag.value = 3;
		showLoading(10);
		setTimeout("redirect();", 10000);
		document.form.submit();
	}
	else
		return;
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_Modem_Content.asp";
}

function validForm(){
	return true;
}

function done_validating(action){
	refreshpage();
}

function check_dongle_status(){
	$j.ajax({
    	url: '/ajax_ddnscode.asp',
    	dataType: 'script', 

    	error: function(xhr){
      		check_dongle_status();
    	},
    	success: function(response){
			if(pin_status != "" && pin_status != "0")
				$("pincode_status").style.display = "";
			else	
				$("pincode_status").style.display = "none";
				
			setTimeout("check_dongle_status();",5000);
       }
   });
}

function doTransChange() {
	var pvc, transModeSupport = 0;

	pvc=document.form.wan_TransMode.selectedIndex;

<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	transModeSupport += 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	transModeSupport += 2;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	transModeSupport += 4;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveUSBModem","h") = "Yes" then%>
	transModeSupport += 8;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
	transModeSupport += 16;
<%end if%>

	switch(transModeSupport) {
		case 5:	//ATM + ETHER, ex. DSL-N17VPN
			if(pvc==1) //ether
				pvc = 10;
			break;
		case 7:	//ATM + PTM + ETHER, ex. DSL-N66U
			if(pvc==1) //ptm
				pvc = 8;
			else if(pvc==2) //ether
				pvc = 10;
			break;
		case 9: //ATM + USB, ex. DSL-N12U-C1
			if(pvc==1) //USB
				pvc = 11;
			break;	
		case 13:	//ATM + ETHER + USB, ex.
			if(pvc==1) //ether
				pvc = 10;
			else if(pvc==2) //USB
				pvc = 11;				
			break;	
		case 15:	//ATM + PTM + ETHER + USB, ex.
			if(pvc==1) //ptm
				pvc = 8;
			else if(pvc==2) //ether
				pvc = 10;
			else if(pvc==3) //USB
				pvc = 11;				
			break;
		case 17:	//ATM + ETHER_LAN
			if(pvc==1) //ETHER_LAN
				pvc = 12;
			break;
		case 19:	//ATM + PTM + ETHER_LAN
			if(pvc==1) //PTM
				pvc = 8;
			else if(pvc==2) //ETHER_LAN
				pvc = 12;
			break;
		case 25:	//ATM + USB + ETHER_LAN
			if(pvc==1) //USB
				pvc = 11;
			else if(pvc==2) //ETHER_LAN
				pvc = 12;
			break;
		case 26:	//ATM + PTM + USB + ETHER_LAN
			if(pvc==1) //PTM
				pvc = 8;
			else if(pvc==2) //USB
				pvc = 11;
			else if(pvc==3) //ETHER_LAN
				pvc = 12;
			break;
		//add other case for other product if necessary
	}
	if(document.form.wan_TransMode.value == "ATM"){
		document.formUSB.ModemInfo_PVC.value = "0";
		document.formUSB.ModemTransMode.value = document.form.wan_TransMode.value;
	}
	else if(document.form.wan_TransMode.value == "PTM"){
		document.formUSB.ModemInfo_PVC.value = "8";
		document.formUSB.ModemTransMode.value = document.form.wan_TransMode.value;
	}
	else if(document.form.wan_TransMode.value == "USB"){
		document.formUSB.ModemInfo_PVC.value = "11";
		document.formUSB.ModemTransMode.value = document.form.wan_TransMode.value;
		return;
	}
	else if(document.form.wan_TransMode.value == "LAN"){
		document.formUSB.ModemInfo_PVC.value = "12";
		document.formUSB.ModemTransMode.value = document.form.wan_TransMode.value;
	}
	else{
		document.formUSB.ModemInfo_PVC.value = "10";
		document.formUSB.ModemTransMode.value = document.form.wan_TransMode.value;
	}

	document.formUSB.submit();
	return;
}

function redirect_DSL(){
	document.location.href = "/cgi-bin/Advanced_DSL_Content.asp";
}

function hide_usb_settings(_flag){
	inputCtrl(document.form.modem_autoapn, 0);
	inputCtrl(document.form.modem_enable_option, 0);
	inputCtrl(document.form.modem_country, 0);
	inputCtrl(document.form.modem_isp, 0);
	inputCtrl(document.form.modem_apn, 0);
	if(pin_opt) inputCtrl(document.form.modem_pincode, 0);
	inputCtrl(document.form.modem_dialnum, 0);
	inputCtrl(document.form.modem_user, 0);
	inputCtrl(document.form.modem_pass, 0);
	inputCtrl(document.form.modem_ttlsid, 0);
	inputCtrl(document.form.Dev3G, 0);
	document.getElementById("modem_enable_div_tr").style.display = "none";
	document.getElementById("modem_apn_div_tr").style.display = "none";
	document.getElementById("modem_dialnum_div_tr").style.display = "none";
	document.getElementById("modem_user_div_tr").style.display = "none";
	document.getElementById("modem_pass_div_tr").style.display = "none";
}

function select_usb_device(obj){
	if(obj.selectedIndex == 0){
		inputCtrl(document.form.modem_autoapn, 1);
		switch_modem_mode(document.form.modem_enable_option.value);
		gen_country_list();
		reloadProfile();
		change_apn_mode();
		document.getElementById("android_desc").style.display="none";
	}
	else{
		document.getElementById("android_desc").style.display="";
		hide_usb_settings(1);
	}

}

function change_apn_mode(){
	if(document.form.modem_autoapn.value == "1"){//Automatic
		var modem_enable_str = "";
		inputCtrl(document.form.modem_country, 0);
		inputCtrl(document.form.modem_isp, 0);
		inputCtrl(document.form.modem_enable_option, 0);
		inputCtrl(document.form.modem_apn, 0);
		inputCtrl(document.form.modem_dialnum, 0);
		inputCtrl(document.form.modem_user, 0);
		inputCtrl(document.form.modem_pass, 0);
		document.getElementById("modem_enable_div_tr").style.display = "";
		document.getElementById("modem_apn_div_tr").style.display = "";
		document.getElementById("modem_dialnum_div_tr").style.display = "";
		document.getElementById("modem_user_div_tr").style.display = "";
		document.getElementById("modem_pass_div_tr").style.display = "";	
		if(document.form.modem_enable.value == "1")
			modem_enable_str = "WCDMA (UMTS) / LTE";
		else if(document.form.modem_enable.value == "2")
			modem_enable_str = "CDMA2000 (EVDO)";
		else if(document.form.modem_enable.value == "3")
			modem_enable_str = "TD-SCDMA";
		else if(document.form.modem_enable.value == "4")
			modem_enable_str = "WiMAX";
		document.getElementById("modem_enable_div").innerHTML = modem_enable_str;
		document.getElementById("modem_apn_div").innerHTML = apn;
		document.getElementById("modem_dialnum_div").innerHTML = dialnum;
		document.getElementById("modem_user_div").innerHTML = user;
		document.getElementById("modem_pass_div").innerHTML = pass;
	
	}
	else{	//Manual
		inputCtrl(document.form.modem_country, 1);
		if(document.form.modem_country.value == ""){
			inputCtrl(document.form.modem_isp, 0);
			inputCtrl(document.form.modem_enable_option, 1);
		}
		else{
			inputCtrl(document.form.modem_isp, 1);
			inputCtrl(document.form.modem_enable_option, 0);
		}
		inputCtrl(document.form.modem_apn, 1);
		inputCtrl(document.form.modem_dialnum, 1);
		inputCtrl(document.form.modem_user, 1);
		inputCtrl(document.form.modem_pass, 1);
		document.getElementById("modem_enable_div_tr").style.display = "none";
		document.getElementById("modem_apn_div_tr").style.display = "none";
		document.getElementById("modem_dialnum_div_tr").style.display = "none";
		document.getElementById("modem_user_div_tr").style.display = "none";
		document.getElementById("modem_pass_div_tr").style.display = "none";
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
<form method="post" name="form" id="ruleForm" action="Advanced_Modem_Content.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_Modem_Content.asp">
<input type="hidden" name="next_page" value="Advanced_Modem_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_wan_if">
<input type="hidden" name="action_wait" value="10">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="modem_enable" value="<% tcWebApi_staticGet("USBModem_Entry","modem_enable","s") %>">
<input type="hidden" name="PVC11_active" value="<% tcWebApi_staticGet("Wan_PVC11","Active","s") %>">
<input type="hidden" name="PVC11_DefaultRoute" value="Yes">
<input type="hidden" name="DvInfo_PVC" value="11">
<input type="hidden" name="wanTransFlag" value="0">
<input type="hidden" name="wanVCFlag" value="0">
<input type="hidden" name="ModemSaveFlag" value="0">
<input type="hidden" name="wans_dualwan" value="<% tcWebApi_staticGet("Dualwan_Entry","wans_dualwan","s") %>">

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
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle" style="-webkit-border-radius: 3px;-moz-border-radius: 3px;border-radius:3px;">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" height="680px">
<div>&nbsp;</div>
<div style="width:730px">
<table width="730px">
<tr>
<td align="left">
<span class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_4_4","s")%></span>
</td>
<td align="right">
<img onclick="go_setting('APP_Installation.asp')" align="right" style="cursor:pointer;position:absolute;margin-left:-20px;margin-top:-30px;" title="Back to USB Extension" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'">
</td>
</tr>
</table>
</div>
<div style="margin:5px;"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","HSDPAC_hsdpa_enable_hint1","s")%></div>

						<table  width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","WAN_Transfer_Mode","s")%></td>
						</tr>
						</thead>
						<tr>
							<th><%tcWebApi_get("String_Entry","Transfer_Mode","s")%></th>
							<td align="left">
								<select class="input_option" name="wan_TransMode" onchange="doTransChange();">
								<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
									<option value="ATM">ADSL WAN (ATM)</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
									<option value="PTM">VDSL WAN (PTM)</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
									<option value="Ethernet">Ethernet WAN</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","haveUSBModem","h") = "Yes" then%>
									<option value="USB" selected>USB Modem</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
									<option value="LAN">Ethernet WAN</option>
								<%end if%>
								</select>
							</td>
						</tr>
						</table>
												
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:8px">
<thead>
<tr>
<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
		<th><%tcWebApi_get("String_Entry","HSDPAC_hsdpa_enable_in","s")%></th>
		<td>
			<div class="left" style="width:94px; float:left; cursor:pointer;" id="usb_modem_switch"></div>
			<div class="clear" style="height:32px; width:74px; position: relative; overflow: hidden"></div>
		</td>
</tr>
<tr id="modem_android_tr" style="display:none;">
		<th><%tcWebApi_get("String_Entry","select_usb_device","s")%></th>
		<td>
			<select id="modem_android" name="modem_android" class="input_option" onChange="select_usb_device(this);">
				<option value="0"><%tcWebApi_get("String_Entry","menu5_4_4","s")%></option>
				<option value="1"><%tcWebApi_get("String_Entry","Android_phone","s")%></option>
			</select>
			<div  class="formfontdesc" id="android_desc" style="display:none; color:#FFCC00;margin-top:5px;">
				<%tcWebApi_get("String_Entry","usb_tethering_hint0","s")%>
				<ol style="margin-top: 0px;">
				<li><%tcWebApi_get("String_Entry","usb_tethering_hint1","s")%></li>
				<li><%tcWebApi_get("String_Entry","usb_tethering_hint2","s")%></li>
				<li><%tcWebApi_get("String_Entry","usb_tethering_hint3","s")%></li>
				</ol>
			</div>
		</td>
</tr>

<tr>
	<th width="40%"><%tcWebApi_get("String_Entry","APN_configuration","s")%></th>
	<td>
		<select name="modem_autoapn" id="modem_autoapn" class="input_option" onchange="change_apn_mode();">
			<option value="1" <%if tcWebApi_get("USBModem_Entry","modem_autoapn","h") = "1" then asp_write("selected") end if%>><%tcWebApi_get("String_Entry","Auto","s")%></option>
			<option value="0" <%if tcWebApi_get("USBModem_Entry","modem_autoapn","h") = "0" then asp_write("selected") end if%>><%tcWebApi_get("String_Entry","Manual_Setting_btn","s")%></option>
		</select>
	</td>
</tr>

<tr>
	<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(21,9);"><%tcWebApi_get("String_Entry","HSDPAC_Country_in","s")%></a></th>
	<td>
		<select name="modem_country" class="input_option" onfocus="parent.showHelpofDrSurf(21,9);" onchange="switch_modem_mode(document.form.modem_enable_option.value);reloadProfile();"></select>
	</td>
</tr>
<tr>
	<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(21,8);"><%tcWebApi_get("String_Entry","HSDPAC_ISP_in","s")%></a></th>
	<td>
		<select name="modem_isp" id="modem_isp" class="input_option" onchange="show_APN_list();"></select>
	</td>
</tr>
<tr>
<th width="40%">
	<a class="hintstyle" href="javascript:void(0);" onclick="openHint(21,1);"><%tcWebApi_get("String_Entry","menu5_4_4","s")%></a>
</th>
<td>
<select name="modem_enable_option" id="modem_enable_option" class="input_option" onchange="switch_modem_mode(this.value);reloadProfile();">
<!--option value="0" ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option-->
<option value="1" selected>WCDMA (UMTS) / LTE</option>
<option value="2" >CDMA2000 (EVDO)</option>
<option value="3" >TD-SCDMA</option>
<option value="4" >WiMAX</option>
</select>							
<br/><span id="hsdpa_hint" style="display:none;"><%tcWebApi_get("String_Entry","HSDPAC_hsdpa_enable_hint2","s")%></span>
</td>
</tr>

<tr id="modem_enable_div_tr" style="display:none;">
	<th>Telecommunications Standards</th>
	<td>
		<div id="modem_enable_div" style="color:#FFFFFF; margin-left:1px;"></div>
	</td>
</tr>

<tr>
	<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(21,3);"><%tcWebApi_get("String_Entry","HSDPAC_private_apn_in","s")%></a></th>
	<td>
		<input id="modem_apn" name="modem_apn" class="input_20_table" maxlength="32" type="text" value="<%if tcWebApi_get("USBModem_Entry","modem_apn","h") <> "" then tcWebApi_get("USBModem_Entry","modem_apn","s") else asp_Write("internet") end if%>"/>
	</td>
</tr>

<tr id="modem_apn_div_tr" style="display:none;">
	<th><%tcWebApi_get("String_Entry","HSDPAC_private_apn_in","s")%></th>
	<td>
		<div id="modem_apn_div" style="color:#FFFFFF; margin-left:1px;"></div>
	</td>
</tr>

<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(21,10);"><%tcWebApi_get("String_Entry","HSDPAC_DialNum_in","s")%></a></th>
<td>
<input id="modem_dialnum" name="modem_dialnum" class="input_20_table" maxlength="32" type="text" value="<%if tcWebApi_get("USBModem_Entry","modem_dialnum","h") <> "" then tcWebApi_get("USBModem_Entry","modem_dialnum","s") else asp_Write("*99#") end if%>"/>
</td>
</tr>

<tr id="modem_dialnum_div_tr" style="display:none;">
	<th><%tcWebApi_get("String_Entry","HSDPAC_DialNum_in","s")%></th>
	<td>
		<div id="modem_dialnum_div" style="color:#FFFFFF; margin-left:1px;"></div>
	</td>
</tr>

<tr>
		<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(21,11);"><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%></a></th>
		<td>
			<input id="modem_user" name="modem_user" class="input_20_table" maxlength="32" type="text" autocapitalization="off" autocomplete="off" value="<%if tcWebApi_get("USBModem_Entry","modem_user","h") <> "" then tcWebApi_get("USBModem_Entry","modem_user","s") else asp_Write("") end if%>"/>
		</td>
</tr>

<tr id="modem_user_div_tr" style="display:none;">
	<th><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%></th>
	<td>
		<div id="modem_user_div" style="color:#FFFFFF; margin-left:1px;"></div>
	</td>
</tr>

<tr>
		<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(21,12);"><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></a></th>
		<td>
			<input id="modem_pass" name="modem_pass" class="input_20_table" maxlength="32" type="password" autocapitalization="off" autocomplete="off" value="<%if tcWebApi_get("USBModem_Entry","modem_pass","h") <> "" then tcWebApi_get("USBModem_Entry","modem_pass","s") else asp_Write("") end if%>"/>
		</td>
</tr>

<tr id="modem_pass_div_tr" style="display:none;">
	<th><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></th>
	<td>
		<div id="modem_pass_div" style="color:#FFFFFF; margin-left:1px;"></div>
	</td>
</tr>

<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(21,2);"><%tcWebApi_get("String_Entry","PIN_code","s")%></a></th>
		<td>
			<input id="modem_pincode" name="modem_pincode" class="input_20_table" type="password" maxLength="8" autocapitalization="off" autocomplete="off" value="<%if tcWebApi_get("USBModem_Entry","modem_pincode","h") <> "" then tcWebApi_get("USBModem_Entry","modem_pincode","s") else asp_Write("") end if%>"/>
			<br><span id="pincode_status" style="display:none;"><%tcWebApi_get("String_Entry","pincode_wrong","s")%></span>
		</td>
</tr>

<tr>
		<th>E-mail</th>
		<td>
			<input id="modem_ttlsid" name="modem_ttlsid" type="text" class="input_20_table" maxlength="64" value=""/>
		</td>
</tr>
<tr>
		<th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(21,13);"><%tcWebApi_get("String_Entry","HSDPAC_USBAdapter_in","s")%></a></th>
		<td>
			<select name="Dev3G" id="shown_modems" class="input_option" disabled="disabled"></select>
		</td>
</tr>
</table>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
</form>
<form method="post" name="formUSB" action="/cgi-bin/start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_Modem_Content.asp">
<input type="hidden" name="next_page" value="Advanced_DSL_Content.asp">
<input type="hidden" name="ModemInfo_PVC" value="0">
<input type="hidden" name="ModemTransMode" value="ATM">
</form>
<div id="footer"></div>
</body>

<!--Advanced_Modem_Content.asp-->
</html>

