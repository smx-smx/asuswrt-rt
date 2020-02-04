<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--index.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<link href="/images/map-iconRouter_iphone.png" rel="apple-touch-icon" />
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <% tcWebApi_Get("String_Entry", "menu1", "s") %></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/NM_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<style type="text/css">
.contentM_qis{
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:20000;
	background-color:#2B373B;
	display:block;
	margin-left: 23%;
	margin-top: 20px;
	width:420px;
	height:250px;
}
.contentM_qis_manual{
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:200;
	background-color:#2B373B;
	margin-left: -30px;
	margin-left: -100px \9; 
	margin-top:-400px;
	width:740px;
	box-shadow: 3px 3px 10px #000;
}
#client_image{
	cursor: pointer;
	width: 102px;
	height: 77px;
	background-image: url('/images/New_ui/networkmap/client-list.png');
	background-repeat: no-repeat;
	background-position:50% 61.10%;
}
#client_image:hover{
	background-image: url('/images/New_ui/networkmap/client-listover.png');
}
#custom_image table{
	border: 1px solid #000000;
	border-collapse: collapse;
}
#custom_image div{
	background-image:url('/images/New_ui/networkmap/client-list.png');
	background-repeat:no-repeat;
	height:55px;
	width:55px;
	cursor:pointer;
	background-position:-21px 0%;
}
#custom_image div:hover{
	background-image:url('/images/New_ui/networkmap/client-listover.png');
}
#custom_image td:hover{
	border-radius: 7px;
	background-color:#84C1FF;
}
.disabled{
	background:#475a5f;
	width:200px;
	height:15px;
	padding:5px 0px 5px 5px;
	margin-left:2px;
	border-radius:5px;
}
#ipLockIcon{
	cursor: pointer;
	margin-top:-2px;
	background-repeat:no-repeat;
	height: 25px;
	width: 25px;
}
.dhcp{
	background-image: url('/images/New_ui/networkmap/unlock.png');
}
.static{
	background-image: url('/images/New_ui/networkmap/lock.png');
}
</style>
<script type="text/javascript" src="/md5.js"></script>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/disk_functions.js"></script>
<script type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/httpApi.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/asus_eula.js"></script>
<script>
var $j = jQuery.noConflict();
	
if(location.pathname == "/"){
if('1' == '0')
location.href = 'QIS_wizard.asp?flag=welcome';
else if('1' == '0' && sw_mode != 2)
location.href = 'QIS_wizard.asp?flag=wireless';
}

<% wanlink(); %>
<% first_wanlink(); %>
<% secondary_wanlink(); %>

var wanstate = -1;
var wansbstate = -1;
var wanauxstate = -1;

var flag = '';
var usb_path1_index = '<% tcWebApi_Get("USB_Entry", "usb_path1", "s") %>';
var usb_path2_index = '<% tcWebApi_Get("USB_Entry", "usb_path2", "s") %>';

var wan_primary_pvcunit = '<% get_wan_primary_pvcunit() %>';
var wan_secondary_pvcunit = '<% get_wan_secondary_pvcunit() %>';
var wans_dualwan_orig = '<% tcWebApi_Get("Dualwan_Entry", "wans_dualwan", "s") %>';
var wans_mode = '<%tcWebApi_Get("Dualwan_Entry", "wans_mode", "s")%>';

var wanlink_status = wanlink_statusstr();
var wanlink_ipaddr = wanlink_ipaddr();
var first_wanlink_status = first_wanlink_statusstr();
var first_wanlink_ipaddr = first_wanlink_ipaddr();
var secondary_wanlink_status = secondary_wanlink_statusstr();
var secondary_wanlink_ipaddr = secondary_wanlink_ipaddr();


if(wans_dualwan_orig.search(" ") == -1)
	var wans_flag = 0;
else
	var wans_flag = (wans_dualwan_orig.search("none") == -1) ? 1:0;

<% get_printer_info() %>


var usb_path0_product = '<% tcWebApi_get("USB_Entry","usb_path1_product","s") %>';
var usb_path1_product = '<% tcWebApi_get("USB_Entry","usb_path2_product","s") %>';
var dongle_name;

var all_disks;
var all_disk_interface;
if(usb_support != -1){
	all_disks = foreign_disks().concat(blank_disks());
	all_disk_interface = foreign_disk_interface_names().concat(blank_disk_interface_names());
}
var leases = []; // [[hostname, MAC, ip, lefttime], ...]
var arps = []; // [[ip, x, x, MAC, x, type], ...]
var arls = []; // [[MAC, port, x, x], ...]
var wireless = []; // [[MAC, associated, authorized], ...]
var ipmonitor = []; // [[IP, MAC, DeviceName, Type, http, printer, iTune], ...]
var client_list_array = '';

function initial(){
	
	show_menu();
	var isIE6 = navigator.userAgent.search("MSIE 6") > -1;
	if(isIE6)
		alert("<% tcWebApi_get("String_Entry","ALERT_TO_CHANGE_BROWSER","s") %>");

	if(dualWAN_support != -1 && sw_mode == 1){
		check_dualwan(wans_flag);	//Enable/DisableDual WAN
	}

	show_middle_status(document.form.wl_auth_mode_x.value, document.form.wl_wep_x.value);
	setTimeout('update_wan_status2();', 1000);

	set_default_choice();

	show_client_status(originData.size);

	if(parent.usb_support == -1){
		document.getElementById("line3_td").height = '20px';
		document.getElementById("line3_img").src = '/images/New_ui/networkmap/line_one.png';
		document.getElementById("clients_tr").colSpan = "3";
		document.getElementById("clients_tr").className = 'NM_radius';
		document.getElementById("clients_tr").width = '350';
		document.getElementById("clientspace_td").style.display = "none";
		document.getElementById("usb1_tr").style.display = "none";
		document.getElementById("usb2_tr").style.display = "none";
		document.getElementById("bottomspace_tr").style.display = "";
	}
	if(rc_support.search("usbX") == -1 || rc_support.search("usbX1") > -1){
		document.getElementById("deviceIcon_1").style.display = "none";
		document.getElementById("deviceDec_1").style.display = "none";
		document.getElementById("usb1_text").style.display = "none";
	}
	show_USBDevice();
	//showMapWANStatus();

	if(sw_mode == "1"){
		document.getElementById("wanIP_status").innerHTML = '<span style="word-break:break-all;">' + wanlink_ipaddr + '</span>'
		setTimeout("show_ddns_status();", 1);

		if(wanlink_ipaddr == '0.0.0.0' || wanlink_ipaddr == '')
			document.getElementById("wanIP_div").style.display = "none";
	}

	var NM_table_img = getCookie("NM_table_img");
	if(NM_table_img != "" && NM_table_img != null){
		customize_NM_table(NM_table_img);
		document.getElementById("bgimg").options[NM_table_img[4]].selected = 1;
	}
	update_wan_status();

	if(wans_flag == 0)
		change_wan_pvc(wan_primary_pvcunit);

	setTimeout(check_eula, 100);
}

function check_eula(){
    ASUS_EULA.config(check_eula, check_eula);

    var asus_status = httpApi.nvramGet(["ASUS_EULA", "ASUS_EULA_time", "ddns_enable_x", "ddns_server_x"], true);
    if( (asus_status.ASUS_EULA == "1" && asus_status.ASUS_EULA_time == "") ||
        (asus_status.ASUS_EULA != "1" && asus_status.ddns_enable_x == "1" && asus_status.ddns_server_x == "WWW.ASUS.COM") ){
        ASUS_EULA.check("asus");
        return false;
    }

    var tm_status = httpApi.nvramGet(["TM_EULA", "TM_EULA_time"], true);
    if(tm_status.TM_EULA == "1" &&  tm_status.TM_EULA_time == ""){
        ASUS_EULA.check("tm");
        return false;
    }
}

function update_wan_status2(){
	$j.ajax({
		url: '/cgi-bin/query_wan_status2.asp',
		dataType: 'script',

		error: function(xhr){
			setTimeout("update_wan_status2();", 2000);
		},
		success: function(response){
			show_middle_status(Auth_Mode2, Wep2);
			setTimeout("update_wan_status2();", 2000);
		}
	});
}

function customize_NM_table(img){
	document.getElementById("NM_table").style.background = "url('/images/" + img +"')";
	setCookie(img);
}
function setCookie(color){
	document.cookie = "NM_table_img=" + color;
}
function getCookie(c_name)
{
if (document.cookie.length>0){
c_start=document.cookie.indexOf(c_name + "=")
if (c_start!=-1){
c_start=c_start + c_name.length+1
c_end=document.cookie.indexOf(";",c_start)
if (c_end==-1) c_end=document.cookie.length
return unescape(document.cookie.substring(c_start,c_end))
}
}
return null
}

function set_default_choice(){
	var icon_name;
	if(flag && flag.length > 0 && wan_route_x != "IP_Bridged"){
		if(flag == "Internet")
			document.getElementById("statusframe").src = "/cgi-bin/device-map/internet.asp";
		else if(flag == "Client")
			document.getElementById("statusframe").src = "/cgi-bin/device-map/clients.asp";
		else if(flag == "Router2g")
			document.getElementById("statusframe").src = "/cgi-bin/device-map/router.asp";
		else{
			clickEvent(document.getElementById("iconRouter"));
			return;
		}

		if(flag == "Router2g")
			icon_name = "iconRouter";
		else
			icon_name = "icon"+flag;

		clickEvent(document.getElementById(icon_name));
	}
	else
		clickEvent(document.getElementById("iconRouter"));
}

function showMapWANStatus(){
showtext(document.getElementById("NM_connect_status"), "AP Mode");
}

function show_middle_status(auth_mode, wep){
	var security_mode;
	switch (auth_mode){
		case "OPEN":
		security_mode = "Open System";
		break;
		case "SHARED":
		security_mode = "Shared Key";
		break;
		case "WPAPSK":
		security_mode = "WPA-Personal";
		break;
		case "WPA2PSK":
		security_mode = "WPA2-Personal";
		break;
		case "WPAPSKWPA2PSK":
		security_mode = "WPA-Auto-Personal";
		document.getElementById("wl_securitylevel_span").style.fontSize = "16px";
		break;
		case "WPA":
		security_mode = "WPA-Enterprise";
		break;
		case "WPA2":
		security_mode = "WPA2-Enterprise";
		break;
		case "WPA1WPA2":
		security_mode = "WPA-Auto-Enterprise";
		document.getElementById("wl_securitylevel_span").style.fontSize = "16px";
		break;
		case "Radius":
		security_mode = "Radius WEP-64";
		document.getElementById("wl_securitylevel_span").style.fontSize = "16px";
		break;
		default:
		//alert("case default");
		security_mode = "Unknown Auth";
	}
	document.getElementById("wl_securitylevel_span").innerHTML = security_mode;
	if(auth_mode == "OPEN")
	{
		if(wep == "1" || wep == "2")
			document.getElementById("iflock").src = "/images/New_ui/networkmap/lock.png"
		else
			document.getElementById("iflock").src = "/images/New_ui/networkmap/unlock.png";
	}
	else
		document.getElementById("iflock").src = "/images/New_ui/networkmap/lock.png"
}

function show_client_status(num){
	var client_str = "";
	client_str += "<% tcWebApi_Get("String_Entry", "Full_Clients", "s") %>";
	client_str += ": <span id='_clientNumber'>"+num+"</span>";
	document.getElementById("clientNumber").innerHTML = client_str;
}
function show_USBDevice(){
	if(usb_support == -1){
		usb_path1_index = "";
		usb_path2_index = "";
	}
	else
		all_disks = foreign_disks().concat(blank_disks());
	switch(usb_path1_index){
		case "storage":
			for(var i = 0; i < all_disks.length; ++i)
				if(foreign_disk_interface_names()[i] == "1"){
					disk_html(0, i);
					break;
				}
			if(all_disk_interface.getIndexByValue("1") == -1)
				no_device_html(0);
			break;
		case "printer":
			printer_html(0, 0);
			break;
		case "audio":
		case "webcam":
		case "modem":
			modem_html(0, 0);
			break;
		default:
			no_device_html(0);
	}
	switch(usb_path2_index){
		case "storage":
			for(var i = 0; i < all_disks.length; ++i)
				if(foreign_disk_interface_names()[i] == "2"){
					disk_html(1, i);
					break;
				}
			if(all_disk_interface.getIndexByValue("2") == -1)
				no_device_html(0);
			break;
		case "printer":
			printer_html(1, 1);
			break;
		case "audio":
		case "webcam":
		case "modem":
			modem_html(1, 1);
			break;
		default:
			no_device_html(1);
	}
}

function disk_html(device_order, all_disk_order){
	var device_icon = document.getElementById("deviceIcon_"+device_order);
	var device_dec = document.getElementById("deviceDec_"+device_order);
	var icon_html_code = '';
	var dec_html_code = '';
	var disk_model_name = "";
	var TotalSize;
	var mount_num = getDiskMountedNum(all_disk_order);
	var all_accessable_size;
	var percentbar = 0;
	if(all_disk_order < foreign_disks().length)
		disk_model_name = foreign_disk_model_info()[all_disk_order];
	else
		disk_model_name = blank_disks()[all_disk_order-foreign_disks().length];
	icon_html_code += '<a href="/cgi-bin/device-map/disk.asp" target="statusframe">\n';
	if(device_order == 0)
		icon_html_code += '<div id="iconUSBdisk_'+all_disk_order+'" style="margin-top:20px;" class="iconUSBdisk" onclick="setSelectedDiskOrder(this.id);clickEvent(this);"></div>\n';
	else
		icon_html_code += '<div id="iconUSBdisk_'+all_disk_order+'" class="iconUSBdisk" onclick="setSelectedDiskOrder(this.id);clickEvent(this);"></div>\n';
	icon_html_code += '</a>\n';
	dec_html_code += '<div class="formfonttitle_nwm" style="text-align:center;margin-top:10px;">'+disk_model_name+'</div>\n';
	if(mount_num > 0){
		if(all_disk_order < foreign_disks().length)
			TotalSize = simpleNum(foreign_disk_total_size()[all_disk_order]);
		else
			TotalSize = simpleNum(blank_disk_total_size()[all_disk_order-foreign_disks().length]);
		all_accessable_size = simpleNum2(computeallpools(all_disk_order, "size")-computeallpools(all_disk_order, "size_in_use"));
		percentbar = simpleNum2((all_accessable_size)/TotalSize*100);
		percentbar = Math.round(100-percentbar);
		dec_html_code += '<p id="diskDesc'+ foreign_disk_interface_names()[all_disk_order] +'" style="margin-top:5px;"><% tcWebApi_Get("String_Entry", "Availablespace", "s") %>:</p><div id="diskquota" align="left" style="margin-top:5px;margin-bottom:10px;">\n';
		dec_html_code += '<img src="/images/quotabar.gif" width="'+percentbar+'" height="13">';
		dec_html_code += '</div>\n';
	}
	else{
		all_disk_order++;
		dec_html_code += '<span class="style1"><strong id="diskUnmount'+ all_disk_order +'"><% tcWebApi_Get("String_Entry", "DISK_UNMOUNTED", "s") %></strong></span>\n';
	}
	device_icon.innerHTML = icon_html_code;
	device_dec.innerHTML = dec_html_code;
}

function printer_html(device_seat, printer_order){
	var printer_name = printer_manufacturers()[printer_order]+" "+printer_models()[printer_order];
	var printer_status = "";
	var device_icon = document.getElementById("deviceIcon_"+device_seat);
	var device_dec = document.getElementById("deviceDec_"+device_seat);
	var icon_html_code = '';
	var dec_html_code = '';
	if(printer_pool()[printer_order] != "")
		printer_status = '<% tcWebApi_Get("String_Entry", "CTL_Enabled", "s") %>';
	else
		printer_status = '<% tcWebApi_Get("String_Entry", "CTL_Disabled", "s") %>';
	icon_html_code += '<a href="/cgi-bin/device-map/printer.asp" target="statusframe">\n';
	icon_html_code += ' <div id="iconPrinter'+printer_order+'" class="iconPrinter" onclick="clickEvent(this);"></div>\n';
	icon_html_code += '</a>\n';
	dec_html_code += '<div class="formfonttitle_nwm" style="text-align:center;margin-top:10px;"><span id="printerName'+device_seat+'">'+ printer_name +'</span></div>\n';
	dec_html_code += '<div style="margin:10px;">'+printer_status+'</div>';
	device_icon.innerHTML = icon_html_code;
	device_dec.innerHTML = dec_html_code;
}

function modem_html(device_seat, modem_order){

if(device_seat)
	dongle_name = usb_path1_product;
else
	dongle_name = usb_path0_product;	
	
var modem_name = dongle_name;
var modem_status = "<% tcWebApi_Get("String_Entry", "Connected", "s") %>";
var device_icon = document.getElementById("deviceIcon_"+device_seat);
var device_dec = document.getElementById("deviceDec_"+device_seat);
var icon_html_code = '';
var dec_html_code = '';
icon_html_code += '<a href="/cgi-bin/device-map/modem.asp" target="statusframe">\n';
icon_html_code += ' <div id="iconModem'+modem_order+'" class="iconmodem" onclick="clickEvent(this);"></div>\n';
icon_html_code += '</a>\n';
dec_html_code += modem_name+'<br>\n';
device_dec.className = "clients";
device_icon.innerHTML = icon_html_code;
device_dec.innerHTML = dec_html_code;
}
function no_device_html(device_seat){
var device_icon = document.getElementById("deviceIcon_"+device_seat);
var device_dec = document.getElementById("deviceDec_"+device_seat);
var icon_html_code = '';
var dec_html_code = '';
icon_html_code += '<div class="iconNo"></div>';
dec_html_code += '<br/><span id="noUSB'+ device_seat +'">';
if(rc_support.search("usbX") > -1)
dec_html_code += '<% tcWebApi_Get("String_Entry", "NoDevice", "s") %>';
else dec_html_code += 'Nonsupported';
dec_html_code += '</span>\n';
device_icon.innerHTML = icon_html_code;
device_dec.innerHTML = dec_html_code;
}
var avoidkey;
var lastClicked;
var lastName;
var clicked_device_order;
function get_clicked_device_order(){
	return clicked_device_order;
}
function clickEvent(obj){
	var icon;
	var stitle;
	var seat;
	clicked_device_order = -1;

	if(obj.id.indexOf("Internet") > 0){
		if(dualWAN_support == -1){
			check_wan_unit();
		}

		icon = "iconInternet";
		stitle = "<% tcWebApi_Get("String_Entry", "statusTitle_Internet", "s") %>";

		document.getElementById("statusframe").src = "/cgi-bin/device-map/internet.asp";

		if(parent.wans_flag){
			if(obj.id.indexOf("primary") != -1) {
				stitle = "Primary WAN status";
				change_wan_unit(0);
			}
			else {
				stitle = "Secondary WAN status";
				change_wan_unit(1);
			}
		}
	}
	else if(obj.id.indexOf("Router") > 0){
		icon = "iconRouter";
		stitle = "<%tcWebApi_get("String_Entry","menu5_7_1","s")%>";
	}
	else if(obj.id.indexOf("Client") > 0){
		icon = "iconClient";
		stitle = "<% tcWebApi_Get("String_Entry", "statusTitle_Client", "s") %>";
	}
	else if(obj.id.indexOf("USBdisk") > 0){
		icon = "iconUSBdisk";
		stitle = "<% tcWebApi_Get("String_Entry", "statusTitle_USB_Disk", "s") %>";
		document.getElementById("statusframe").src = "/device-map/disk.asp";
	}
	else if(obj.id.indexOf("Modem") > 0){
		seat = obj.id.indexOf("Modem")+5;
		clicked_device_order = parseInt(obj.id.substring(seat, seat+1));
		icon = "iconmodem";
		stitle = "USB Modem";
		document.getElementById("statusframe").src = "/device-map/modem.asp";
	}
	else if(obj.id.indexOf("Printer") > 0){
		seat = obj.id.indexOf("Printer")+7;
		clicked_device_order = parseInt(obj.id.substring(seat, seat+1));
		icon = "iconPrinter";
		stitle = "<% tcWebApi_Get("String_Entry", "statusTitle_Printer", "s") %>";
	}
	else if(obj.id.indexOf("Remote") > 0){
		icon = "iconRemote";
		stitle = "Access Point status";
		document.getElementById("statusframe").src = "/device-map/remote.asp";
	}
	else if(obj.id.indexOf("No") > 0){
		icon = "iconNo";
	}
	else
		alert("mouse over on wrong place!");

	if(lastClicked){
		lastClicked.style.background = 'url(/images/New_ui/networkmap/map-'+lastName+'.png) no-repeat 0% 0%';
	}
	obj.style.background = 'url(/images/New_ui/networkmap/map-'+icon+'.png) no-repeat 0% 101%';

	document.getElementById("helpname").innerHTML = stitle;
	avoidkey = icon;
	lastClicked = obj;
	lastName = icon;
}

function mouseEvent(obj, key){
	var icon;

	if(obj.id.indexOf("Internet") > 0)
		icon = "iconInternet";
	else if(obj.id.indexOf("Router") > 0)
		icon = "iconRouter";
	else if(obj.id.indexOf("Client") > 0){
		if(wan_route_x == "IP_Bridged")
			return;
		icon = "iconClient";
	}
	else if(obj.id.indexOf("USBdisk") > 0)
		icon = "iconUSBdisk";
	else if(obj.id.indexOf("Printer") > 0)
		icon = "iconPrinter";
	else if(obj.id.indexOf("No") > 0)
		icon = "iconNo";
	else
		alert("mouse over on wrong place!");

	if(avoidkey != icon){
		if(key){ //when mouseover
			obj.style.background = 'url("/images/map-'+icon+'_r.gif") no-repeat';
		}
		else { //when mouseout
			obj.style.background = 'url("/images/map-'+icon+'.gif") no-repeat';
		}
	}
}//end of mouseEvent

function MapUnderAPmode(){// if under AP mode, disable the Internet icon and show hint when mouseover.
document.getElementById("iconInternet").style.background = "url(/images/New_ui/networkmap/map-iconInternet-d.png) no-repeat";
document.getElementById("iconInternet").style.cursor = "default";
document.getElementById("iconInternet").onmouseover = function(){
writetxt("<%tcWebApi_get("String_Entry","underAPmode","s")%>");
}
document.getElementById("iconInternet").onmouseout = function(){
writetxt(0);
}
document.getElementById("iconInternet").onclick = function(){
return false;
}
document.getElementById("clientStatusLink").href = "javascript:void(0)";
document.getElementById("clientStatusLink").style.cursor = "default";
document.getElementById("iconClient").style.background = "url(/images/New_ui/networkmap/map-iconClient-d.png) no-repeat";
document.getElementById("iconClient").style.cursor = "default";
}

function showstausframe(page){
	clickEvent(document.getElementById("icon"+page));
	if(page == "Client")
		page = "clients";
	else if(page.indexOf('Internet') == 0){
		if(page == "Internet_secondary")
			document.form.dual_wan_flag.value = 1;
		else
			document.form.dual_wan_flag.value = 0;
		page = "Internet";
	}

	window.open("/cgi-bin/device-map/"+page.toLowerCase()+".asp","statusframe");
}


function check_wan_unit(){   //To check wan_unit, if USB Modem plug in change wan_unit to 1
	if(wan0_primary == 1 && document.form.wan_unit.value == 1)
		change_wan_unit(0);
	else if(wan1_primary == 1 && document.form.wan_unit.value == 0)
		change_wan_unit(1);
}

function change_wan_unit(wan_unit_flag){
	document.form.wan_unit.value = wan_unit_flag;
//	document.form.wl_auth_mode_x.disabled = true;
//	document.form.wl_wep_x.disabled = true;
	FormActions("/cgi-bin/apply.asp", "change_wan_unit", "", "");
	document.form.submit();
}

function change_wan_pvc(wan_pvc){
	document.form.wan_pvc.value = wan_pvc;
	FormActions("/cgi-bin/apply.asp", "change_wan_pvc", "", "");
	document.form.submit();
}

function show_ddns_fail_hint() {
	var str="";
	if(sw_mode != 3 && document.getElementById("connect_status").className == "connectstatusoff")	//Both link down
		str = "<%tcWebApi_get("String_Entry","Disconnected","s")%>";
	else if(ddns_server = 'WWW.ASUS.COM') {
		if(ddns_return_code.indexOf('203')!=-1)
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_hostname","s") %> <% tcWebApi_get("Ddns_Entry","MYHOST","s") %> <% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_registered","s") %>";
		else if(ddns_return_code.indexOf('233')!=-1)
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_hostname", "s") %> <% tcWebApi_get("Ddns_Entry","MYHOST","s") %> <% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_registered_2", "s") %> <% tcWebApi_get("Vram_Entry","ddns_old_name","s"); %>";
		else if(ddns_return_code.indexOf('296')!=-1)
			str = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_6", "s")%>";
	  	else if(ddns_return_code.indexOf('297')!=-1)
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_7", "s") %>";
	  	else if(ddns_return_code.indexOf('298')!=-1)
    			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_8", "s") %>";
	  	else if(ddns_return_code.indexOf('299')!=-1)
    			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_9", "s") %>";
  		else if(ddns_return_code.indexOf('401')!=-1)
	    		str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_10", "s") %>";
	  	else if(ddns_return_code.indexOf('407')!=-1)
    			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_11", "s") %>";
		else if(ddns_return_code.indexOf('-1')!=-1)
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2", "s") %>";
	  	else if(ddns_return_code =='no_change')
    			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_nochange", "s") %>";
		else if(ddns_return_code == 'Time-out')
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_1", "s") %>";
		else if(ddns_return_code =='unknown_error')
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2", "s") %>";
		else if(ddns_return_code =='')
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2", "s") %>";
		else if(ddns_return_code =='connect_fail')
			str = "<% tcWebApi_get("String_Entry","qis_fail_desc7", "s") %>";
		else if(ddns_return_code =='auth_fail')
			str = "<% tcWebApi_get("String_Entry","qis_fail_desc1", "s") %>";
		else if(ddns_return_code =='Updating' || ddns_return_code =='ddns_query')
			str = "Still query ASUS DDNS registration status. Please wait.";
		else
			str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2", "s") %>";
	}
	else
		str = "<% tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2", "s") %>";

	overlib(str);
}

function show_ddns_status(){
	var ddns_enable = '<% tcWebApi_get("Ddns_Entry","Active","s") %>';
	var ddns_server_x = '<% tcWebApi_get("Ddns_Entry","SERVERNAME","s") %>';
	var ddnsName = '<% tcWebApi_get("Ddns_Entry","MYHOST","s") %>';

	document.getElementById("ddns_fail_hint").className = "notificationoff";
        if( ddns_enable == '0')
		document.getElementById("ddnsHostName").innerHTML = '<a style="color:#FFF;text-decoration:underline;" href="/cgi-bin/Advanced_ASUSDDNS_Content.asp?af=ddns_enable_x"><% tcWebApi_get("String_Entry","btn_go","s")%></a>';
	else if(ddnsName == '')
		document.getElementById("ddnsHostName").innerHTML = '<a style="color:#FFF;text-decoration:underline;" href="/cgi-bin/Advanced_ASUSDDNS_Content.asp?af=DDNSName"><% tcWebApi_get("String_Entry","sign_up", "s") %></a>';
	else if(ddnsName == isMD5DDNSName())
		document.getElementById("ddnsHostName").innerHTML = '<a style="color:#FFF;text-decoration:underline;" href="/cgi-bin/Advanced_ASUSDDNS_Content.asp?af=DDNSName"><% tcWebApi_get("String_Entry","sign_up", "s") %></a>';
	else{
		document.getElementById("ddnsHostName").innerHTML = '<span>'+ ddnsName +'</span>';
		if( ddns_enable == '1' ) {
			if(!((link_status == "2" && (link_auxstatus == "0" || link_auxstatus == "2")) || (secondary_link_status == "2" && (secondary_link_auxstatus == "0" || secondary_link_auxstatus == "2")))) //Both link down
				document.getElementById("ddns_fail_hint").className = "notificationon";
				if( ddns_server_x == 'WWW.ASUS.COM' ) { //ASUS DDNS
					if( (ddns_return_code.indexOf('200')==-1) && (ddns_return_code.indexOf('220')==-1) && (ddns_return_code.indexOf('230')==-1))
						document.getElementById("ddns_fail_hint").className = "notificationon";
				}
				else { //Other ddns service
			    		if(ddns_updated != '1' || ddns_return_code=='unknown_error' || ddns_return_code=="auth_fail")
		                        	document.getElementById("ddns_fail_hint").className = "notificationon";
				}
                }
	}
	setTimeout("show_ddns_status();", 2000);
}

var isMD5DDNSName = function(){
	var macAddr = '<% tcWebApi_Get("Info_Ether","mac","s") %>'.toUpperCase().replace(/:/g, "");
	return "A"+hexMD5(macAddr).toUpperCase()+".asuscomm.com";
}


function check_dualwan(flag){
	if(flag == 0){		//single wan
		document.getElementById('single_wan_icon').style.display = "";
		document.getElementById('single_wan_status').style.display = "";
		document.getElementById('single_wan_line').style.display = "";
		document.getElementById('primary_wan_icon').style.display = "none";
		document.getElementById('secondary_wan_icon').style.display = "none";
		document.getElementById('primary_wan_line').style.display = "none";
		document.getElementById('secondary_wan_line').style.display = "none";
		document.getElementById('dual_wan_gap').style.display = "none";
	}
	else{
		document.getElementById('single_wan_icon').style.display = "none";
		document.getElementById('single_wan_status').style.display = "none";
		document.getElementById('single_wan_line').style.display = "none";
		document.getElementById('primary_wan_icon').style.display = "";
		document.getElementById('secondary_wan_icon').style.display = "";
		document.getElementById('primary_wan_line').style.display = "";
		document.getElementById('secondary_wan_line').style.display = "";
		document.getElementById('dual_wan_gap').style.display = "";
	}
}

function update_wan_status(e) {
	if(stopFlag == 1) return false;

	$j.ajax({
		url: '/cgi-bin/status.asp',
		dataType: 'script',

		error: function(xhr) {
			setTimeout("update_wan_status();", 3000);
		},
		success: function(response) {
			wanlink_status = wanlink_statusstr();
			wanlink_ipaddr = wanlink_ipaddr();
			first_wanlink_status = first_wanlink_statusstr();
			first_wanlink_ipaddr = first_wanlink_ipaddr();
			secondary_wanlink_status = secondary_wanlink_statusstr();
			secondary_wanlink_ipaddr = secondary_wanlink_ipaddr();
			change_wan_state(first_wanlink_status,secondary_wanlink_status);
			setTimeout("update_wan_status();", 3000);
		}
	});
}

function change_wan_state(primary_status, secondary_status){
	if (dualWAN_support == -1)
		return true;

	if(wans_mode == "fo" || wans_mode == "fb"){
		if(first_wanlink_ipaddr != '0.0.0.0'
			&& first_wanlink_ipaddr != wanlink_ipaddr
			&& primary_status != 'Disconnected'
			)
			primary_status = "Standby";

		if(secondary_wanlink_ipaddr != '0.0.0.0'
			&& secondary_wanlink_ipaddr != wanlink_ipaddr
			&& secondary_status != 'Disconnected'
			)
			secondary_status = "Standby";

		if(primary_status == 'Disconnected'){
			document.getElementById('primary_line').className = "primary_wan_disconnected";
		}
		else if(primary_status == 'Standby'){
			document.getElementById('primary_line').className = "primary_wan_standby";
		}
		else{
			document.getElementById('primary_line').className = "primary_wan_connected";
		}

		if(secondary_status == 'Disconnected'){
			document.getElementById('secondary_line').className = "secondary_wan_disconnected";
		}
		else if(secondary_status == 'Standby'){
			document.getElementById('secondary_line').className = "secondary_wan_standby";
		}
		else{
			document.getElementById('secondary_line').className = "secondary_wan_connected";
		}

		document.getElementById('primary_status').innerHTML = primary_status;
		document.getElementById('seconday_status').innerHTML = secondary_status;

	}
	else{	//lb
		document.getElementById('primary_status').innerHTML = primary_status;
		document.getElementById('seconday_status').innerHTML = secondary_status;
		if(primary_status == "Disconnected"){
			document.getElementById('primary_line').className = "primary_wan_disconnected";
		}
		else{
			document.getElementById('primary_line').className = "primary_wan_connected";
		}

		if(secondary_status == "Disconnected"){
			document.getElementById('secondary_line').className = "secondary_wan_disconnected";
		}
		else{
			document.getElementById('secondary_line').className = "secondary_wan_connected";
		}
	}
}

</script>
</head>
<body onunload="return unload_body();">
<noscript>
	<div class="popup_bg" style="visibility:visible; z-index:999;">
	<div style="margin:200px auto; width:300px; background-color:#006699; color:#FFFFFF; line-height:150%; border:3px solid #FFF; padding:5px;"><%tcWebApi_get("String_Entry","not_support_script","s")%></p></div>
	</div>
</noscript>
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<div id="hiddenMask" class="popup_bg">
	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
		<tr>
			<td>
				<div class="drword" id="drword"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>
					<br>
					<br>
				</div>
				<div class="drImg"><img src="/images/alertImg.png"></div>
				<div style="height:70px; "></div>
			</td>
		</tr>
	</table>
</div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>
<form method="POST" name="update_form" action="/start_apply.asp" target="hidden_frame">
<input type="hidden" name="update_networkmap_Flag" value="0">	
</form>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="index.asp">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wl_auth_mode_x" value="<% tcWebApi_get("WLan_Entry","auth_mode_x","s") %>">
<input type="hidden" name="wl_wep_x" value="<% tcWebApi_get("WLan_Entry","wep_x","s") %>">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="apps_action" value="">
<input type="hidden" name="apps_path" value="">
<input type="hidden" name="apps_name" value="">
<input type="hidden" name="apps_flag" value="">
<input type="hidden" name="wan_unit" value="0">
<input type="hidden" name="wan_pvc" value="0">
<input type="hidden" name="dual_wan_flag" value="">
</form>
<table class="content" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" width="17">&nbsp;</td>
		<td valign="top" width="204">
			<div id="mainMenu"></div>
			<div id="subMenu"></div>
		</td>
		<td align="left" valign="top" class="bgarrow">
			<div id="tabMenu"></div><br>
			<div id="NM_shift" style="margin-top:-160px;"></div>
			<div id="NM_table" class="NM_table">
				<table cellspacing="0"><!--  id="_NM_table" -->
					<tr>
						<td width="40" rowspan="11" valign="center">
						</td>
						<!--== Dual WAN ==-->
						<td id="primary_wan_icon" width="160px;" height="155" align="center" class="NM_radius" valign="middle" bgcolor="#444f53" onclick="showstausframe('Internet_primary');" style="display:none">
							<a href="/device-map/internet.asp" target="statusframe"><div id="iconInternet_primary" onclick="clickEvent(this);"></div></a>
							<div><%tcWebApi_Get("String_Entry", "dualwan_primary", "s")%>:</div>
							<div><strong id="primary_status"></strong></div>
						</td>
						<td id="dual_wan_gap" width="40px" style="display:none">
						</td>
						<td id="secondary_wan_icon" width="160px;" height="155" align="center" class="NM_radius" valign="middle" bgcolor="#444f53" onclick="showstausframe('Internet_secondary');" style="display:none">
							<a href="/device-map/internet.asp" target="statusframe"><div id="iconInternet_secondary" onclick="clickEvent(this);"></div></a>
							<div><%tcWebApi_Get("String_Entry", "dualwan_secondary", "s")%>:</div>
							<div><strong id="seconday_status"></strong></div>
						</td>
						<!--== single WAN ==-->
						<td id="single_wan_icon" height="115" align="right" class="NM_radius_left" valign="middle" bgcolor="#444f53" onclick="showstausframe('Internet');" >
							<a href="/cgi-bin/device-map/internet.asp" target="statusframe"><div id="iconInternet" onclick="clickEvent(this);"></div></a>
						</td>
						<td id="single_wan_status" colspan="2" valign="middle" bgcolor="#444f53" class="NM_radius_right" onclick="showstausframe('Internet');">
							<div>
								<span id="NM_connect_title" style="font-size:12px;font-family: Verdana, Arial, Helvetica, sans-serif;"><% tcWebApi_Get("String_Entry", "statusTitle_Internet", "s") %>:</span>
								<strong id="NM_connect_status" class="index_status" style="font-size:14px;"><% tcWebApi_Get("String_Entry", "QKS_Internet_Setup_fail_method1", "s") %>...</strong>
							</div>
							<div id="wanIP_div" style="margin-top:5px;">
								<span style="font-size:12px;font-family: Verdana, Arial, Helvetica, sans-serif;">WAN IP:</span>
								<strong id="wanIP_status" class="index_status" style="font-size:14px;"></strong>
							</div>
							<div id="ddnsHostName_div" style="margin-top:5px;word-break:break-all;word-wrap:break-word;">
								<span style="font-size:12px;font-family: Verdana, Arial, Helvetica, sans-serif;">DDNS:</span>
								<strong id="ddnsHostName" class="index_status" style="font-size:14px;"><% tcWebApi_Get("String_Entry", "QIS_detectWAN_desc2", "s") %></strong>
								<span id="ddns_fail_hint" class="notificationoff" style="position: absolute;margin-top:-5px;" onClick="show_ddns_fail_hint();" onMouseOut="nd();"></span>
							</div>
						</td>
						<td width="40" rowspan="11" valign="center">
							<div class="statusTitle">
							<div id="helpname" style="padding-top:10px;font-size:16px;"></div>
							</div>
							<div>
								<iframe id="statusframe" class="NM_radius_bottom" style="margin-left:45px;margin-top:-2px;" name="statusframe" width="320" height="735" frameborder="0" allowtransparency="true" style="background-color:transparent; margin-left:10px;" src="/cgi-bin/device-map/router.asp"></iframe>
							</div>
						</td>
					</tr>
					<tr>
						<!--==line of dual wan==-->
						<td id="primary_wan_line"  height="40px" style="display:none;">
							<div id="primary_line" class="primary_wan_connected"></div>
						</td>
						<td id="secondary_wan_line" colspan="2" height="40px"  style="display:none;">
							<div id="secondary_line" class="secondary_wan_connected"></div>
						</td>
						<!--==line of single wan==-->
						<td id="single_wan_line" colspan="5" height="19px">
							<div id="single_wan" class="single_wan_connected"></div>
						</td>
					</tr>
					<tr>
						<td height="115" align="right" bgcolor="#444f53" class="NM_radius_left" onclick="showstausframe('Router');">
							<a href="/cgi-bin/device-map/router.asp" target="statusframe"><div id="iconRouter" onclick="clickEvent(this);"></div></a>
						</td>
						<td colspan="2" valign="middle" bgcolor="#444f53" class="NM_radius_right" onclick="showstausframe('Router');">
							ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%><br/><%tcWebApi_get("String_Entry","Security_Level","s")%>: <br/><br/><strong id="wl_securitylevel_span" class="index_status"></strong>
							<img id="iflock">
						</td>
					</tr>
					<tr>
						<td id="line3_td" colspan="3" align="center" height="52px">
							<img id="line3_img" src="/images/New_ui/networkmap/line_two.png">
						</td>
						<!--td id="line3_td" colspan="3" height="55" align="center" background="/images/New_ui/networkmap/line_two.png" style="background-repeat: no-repeat;"></td-->
					</tr>
					<tr>
						<td id="clients_tr" width="150" height="170" bgcolor="#444f53" align="center" valign="top" class="NM_radius_top" onclick="showstausframe('Client');">
							<a id="clientStatusLink" href="/cgi-bin/device-map/clients.asp" target="statusframe"><div id="iconClient" style="margin-top:20px;" onclick="clickEvent(this);"></div>
							</a>
							<div class="clients" id="clientNumber" style="cursor:pointer;"></div>
						</td>
						<td width="36" rowspan="6" id="clientspace_td"></td>
						<td id="usb1_tr" width="160" bgcolor="#444f53" align="center" valign="top" class="NM_radius_top">
							<div style="margin-top:20px;" id="deviceIcon_0"></div>
							<div id="usb0_text" class="usb_text">USB 2.0</div>
							<div style="margin:10px 0px;">
								<span id="deviceText_0"></span>
								<select id="deviceOption_0" class="input_option" style="display:none;height:20px;width:130px;font-size:12px;"></select>	
							</div>
							<div id="deviceDec_0"></div>
						</td>
					</tr>
					<tr id="usb2_tr">
						<td bgcolor="#444f53" align="center" valign="top" class="NM_radius_bottom">
						</td>
						<td height="150" bgcolor="#444f53" align="center" valign="top" class="NM_radius_bottom">
							<div style="margin-top:10px;" id="deviceIcon_1"></div>
							<div id="usb1_text" class="usb_text">USB 2.0</div>
							<div style="margin:10px 0px;">
								<span id="deviceText_1"></span>
								<select id="deviceOption_1" class="input_option" style="display:none;height:20px;width:130px;font-size:12px;"></select>	
							</div>
							<div id="deviceDec_1"></div>
						</td>
					</tr>
					<tr id="bottomspace_tr" style="display:none">
						<td colspan="3" height="200px"></td>
					</tr>
					</table>
			</div>
	</tr>
</table>
<div id="navtxt" class="navtext" style="position:absolute; top:50px; left:-100px; visibility:hidden; font-family:Arial, Verdana"></div>
<div id="footer"></div>
<select id="bgimg" onChange="customize_NM_table(this.value);" class="input_option_left" style="display:none;">
	<option value="wall0.gif">dark</option>
	<option value="wall1.gif">light</option>
</select>
<script>
	if(flag == "Internet" || flag == "Client")
		document.getElementById("statusframe").src = "";

	initial();			
	var manualUpdate = false;
	
	if(parseInt((JS_timeObj.getTime()-cookie.get("nwmapRefreshTime"))/60000) > 1){								
                setTimeout(function(){
                		 manualUpdate = true;
                     document.update_form.update_networkmap_Flag.value = 1;										 
										 document.update_form.submit();
                }, 3500);
	}
	cookie.set("nwmapRefreshTime", JS_timeObj.getTime(), 1);
</script>
</body>

<!--index.asp-->
</html>

