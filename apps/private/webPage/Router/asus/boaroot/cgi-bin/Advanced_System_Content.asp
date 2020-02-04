<%
If Request_Form("accountFlag") = "1" Then
	TCWebApi_set("Account_Entry0","username","uiViewTools_username")
	If Request_Form("passwdFlag") = "1" Then
		TCWebApi_set("Account_Entry0","web_passwd","uiViewTools_Password")
		TCWebApi_set("Account_Entry0","console_passwd","uiViewTools_Password")
	End If
	tcWebApi_CommitWithoutSave("Account_Entry0")
	modify_aidisk_account()
End If

If Request_Form("RadioButtonFlag") = "1" Then
	TCWebApi_set("SysInfo_Entry","btn_ez_radiotoggle","btn_ez_radiotoggle")
	tcWebApi_CommitWithoutSave("SysInfo_Entry")
End If

If request_Form("logFlag") = "1" then
	TCWebApi_set("Syslog_Entry","LogEnable","syslogEnable")
	TCWebApi_set("Syslog_Entry","WriteLevel","logLevelSelect")
	TCWebApi_set("Syslog_Entry","DisplayLevel","DisplayLevelSelect")
	If request_Form("remoteSyslog") = "Yes" then
		TCWebApi_set("Syslog_Entry","remoteSyslogEnable","RemotelogEnable")
		TCWebApi_set("Syslog_Entry","remoteHost","syslogServerAddr")
		TCWebApi_set("Syslog_Entry","remotePort","serverPort")
	End if

	tcWebApi_CommitWithoutSave("Syslog_Entry")
End if

If Request_Form("SaveTime") = "1" Then
	TCWebApi_set("Timezone_Entry","TZ","uiViewdateToolsTZ")
	TCWebApi_set("Timezone_Entry","DAYLIGHT","uiViewdateDS")
	TCWebApi_set("Timezone_Entry","SERVER","uiViewSNTPServer")
	TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
	TCWebApi_set("Timezone_Entry","TZ_second","uiTimezoneSecond")

	tcWebApi_CommitWithoutSave("Timezone_Entry")
End if

If Request_Form("uiViewSyncWith") <> "" Then
	If Request_Form("uiViewSyncWith") = "0" Then
		TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
		TCWebApi_set("Timezone_Entry","PCSyncFlag","uiClearPCSyncFlag")
	End if
End if

If Request_Form("SaveSSHd") = "1" Then
	TCWebApi_set("SSH_Entry","Enable","sshd_enable")
	TCWebApi_set("SSH_Entry","sshport","sshd_port")
	TCWebApi_set("SSH_Entry","Need_Pass","sshd_pass")
	TCWebApi_set("SSH_Entry","Authkeys","sshd_authkeys")
	TCWebApi_set("SSH_Entry","timeout","shell_timeout_x")
	tcWebApi_CommitWithoutSave("SSH_Entry")
End if

If Request_Form("SaveTelnetd") = "1" Then
	TCWebApi_set("Misc_Entry","telnetd_enable","telnetd_enable")
	TCWebApi_set("Misc_Entry","telnetd_timeout","shell_timeout_x")
	TCWebApi_set("Misc_Entry","http_autologout","http_autologout")	
	tcWebApi_CommitWithoutSave("Misc_Entry")
End if

If Request_Form("SaveRebootScheduler") = "1" Then
	TCWebApi_set("RebootSchedule_Entry","reboot_schedule_enable","reboot_schedule_enable")
	TCWebApi_set("RebootSchedule_Entry","reboot_schedule","reboot_schedule")
	tcWebApi_CommitWithoutSave("RebootSchedule_Entry")
End if

If Request_Form("SaveRewanScheduler") = "1" Then
	TCWebApi_set("RebootSchedule_Entry","rewan_schedule_enable","rewan_schedule_enable")
	TCWebApi_set("RebootSchedule_Entry","rewan_schedule","rewan_schedule")
	tcWebApi_CommitWithoutSave("RebootSchedule_Entry")
End if

If Request_Form("SaveFirewall") = "1" Then
	TCWebApi_set("Firewall_Entry","misc_http_x","misc_http_x")
	TCWebApi_set("Firewall_Entry","misc_httpport_x","misc_httpport_x")
	TCWebApi_set("Firewall_Entry","misc_httpsport_x","misc_httpsport_x")
	TCWebApi_set("SysInfo_Entry","nat_redirect_enable","nat_redirect_enable")
	TCWebApi_set("Firewall_Entry","enable_acc_restriction","http_client")
	TCWebApi_set("Firewall_Entry","restrict_rulelist","http_clientlist")
	tcWebApi_CommitWithoutSave("Firewall_Entry")
End if

If tcWebApi_Get("WebCustom_Entry", "isSwapFileSupport", "h") = "Yes" then
	If Request_Form("SaveSwap") = "1" Then
		TCWebApi_set("SysInfo_Entry","swap_enable","swap_enable")
		do_swap()
	End if
End if

If Request_Form("tcWebApi_Save_Flag") = "1" Then
	tcWebApi_Save()
End if

If Request_Form("SaveHttps") = "1" Then
	TCWebApi_set("Https_Entry","http_enable","http_enable")
	TCWebApi_set("Https_Entry","https_lanport","https_lanport")
	tcWebApi_Commit("Https_Entry")
	do_aiprotection()
End if

If Request_Form("SaveSSHd") = "1" Then
	update_variables()
End if

%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_System_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_6_2","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/pwdmeter.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/jquery.js"></script>
<script language="JavaScript" src="/jsl.js"></script>
<script language="JavaScript" src="/ip.js"></script>
<style>
#ClientList_Block_PC{
border:1px outset #999;
background-color:#576D73;
position:absolute;
margin-top:0px;
margin-left:122px;
width:345px;
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
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';

var orig_restrict_rulelist_array = [];
var restrict_rulelist_array = [];
var accounts = [<% get_all_accounts(); %>];
var is_gen_key = "<% get_ssh_first_init() %>";

var ddns_enable_x = '<% tcWebApi_get("Ddns_Entry","Active","s"); %>';
var ddns_hostname_x_t = '<% tcWebApi_get("Ddns_Entry","MYHOST","s"); %>';

<% first_wanlink(); %>
<% secondary_wanlink(); %>
var first_wanlink_status = first_wanlink_status();
var first_wanlink_ipaddr = first_wanlink_ipaddr();
var secondary_wanlink_status = secondary_wanlink_status();
var secondary_wanlink_ipaddr = secondary_wanlink_ipaddr();
var wan_ipaddr = "";
if(first_wanlink_status == 1)
	wan_ipaddr = first_wanlink_ipaddr;
else if(secondary_wanlink_status == 1)
	wan_ipaddr = secondary_wanlink_ipaddr;
else
	wan_ipaddr = first_wanlink_ipaddr;


var isFromHTTPS = false;
if((location.href.search('https://') >= 0) || (location.href.search('HTTPS://') >= 0)){
        isFromHTTPS = true;
}

var http_clientlist_array = "<% TCWebApi_get("Firewall_Entry","restrict_rulelist","s") %>";
var set_shell_timeout = "";
var set_http_autologout = "";

var timezones = [
	["GMT+12:00",	"(GMT-12:00) <% tcWebApi_get("String_Entry","TZ01","s")%>"],
	["GMT+11:00",	"(GMT-11:00) <% tcWebApi_get("String_Entry","TZ02","s")%>"],
	["GMT+10:00",	"(GMT-10:00) <% tcWebApi_get("String_Entry","TZ03","s")%>"],
	["GMT+09:00",	"(GMT-09:00) <% tcWebApi_get("String_Entry","TZ04","s")%>"],
	["GMT+08:00",	"(GMT-08:00) <% tcWebApi_get("String_Entry","TZ05","s")%>"],
	["GMT+07:00",	"(GMT-07:00) <% tcWebApi_get("String_Entry","TZ06","s")%>, <% tcWebApi_get("String_Entry","TZ07","s")%>, <% tcWebApi_get("String_Entry","TZ08","s")%>"],
	["GMT+06:00",	"(GMT-06:00) <% tcWebApi_get("String_Entry","TZ10","s")%>, <% tcWebApi_get("String_Entry","TZ11","s")%>, <% tcWebApi_get("String_Entry","TZ12","s")%>, <% tcWebApi_get("String_Entry","TZ13","s")%>"],
	["GMT+05:00",	"(GMT-05:00) <% tcWebApi_get("String_Entry","TZ14","s")%>, <% tcWebApi_get("String_Entry","TZ15","s")%>, <% tcWebApi_get("String_Entry","TZ16","s")%>"],
	["GMT+04:30",   "(GMT-04:30) <% tcWebApi_get("String_Entry","TZ18_1","s")%>"],
	["GMT+04:00",	"(GMT-04:00) <% tcWebApi_get("String_Entry","TZ17","s")%>, <% tcWebApi_get("String_Entry","TZ18","s")%>, <% tcWebApi_get("String_Entry","TZ19","s")%>"],
	["GMT+03:30",	"(GMT-03:30) <% tcWebApi_get("String_Entry","TZ20","s")%>"],
	["GMT+03:00",	"(GMT-03:00) <% tcWebApi_get("String_Entry","TZ21","s")%>, <% tcWebApi_get("String_Entry","TZ22","s")%>, <% tcWebApi_get("String_Entry","TZ23","s")%>"],
	["GMT+02:00",	"(GMT-02:00) <% tcWebApi_get("String_Entry","TZ24","s")%>"],
	["GMT+01:00",	"(GMT-01:00) <% tcWebApi_get("String_Entry","TZ25","s")%>, <% tcWebApi_get("String_Entry","TZ26","s")%>"],
	["GMT",	"(GMT) <% tcWebApi_get("String_Entry","TZ27","s")%>, <% tcWebApi_get("String_Entry","TZ27_2","s")%>, <% tcWebApi_get("String_Entry","TZ28","s")%>, <% tcWebApi_get("String_Entry","TZ28_1","s")%>"],
	["GMT-01:00",	"(GMT+01:00) <% tcWebApi_get("String_Entry","TZ33","s")%>, <% tcWebApi_get("String_Entry","TZ31","s")%>, <% tcWebApi_get("String_Entry","TZ32","s")%>, <% tcWebApi_get("String_Entry","TZ37","s")%>, <% tcWebApi_get("String_Entry","TZ34","s")%>"],
	["GMT-01:00",	"(GMT+01:00) <% tcWebApi_get("String_Entry","TZ35","s")%>, <% tcWebApi_get("String_Entry","TZ36","s")%>, <% tcWebApi_get("String_Entry","TZ29","s")%>, <% tcWebApi_get("String_Entry","TZ30","s")%>"],
	["GMT-02:00",	"(GMT+02:00) <% tcWebApi_get("String_Entry","TZ41","s")%>, <% tcWebApi_get("String_Entry","TZ40","s")%>, <% tcWebApi_get("String_Entry","TZ40_2","s")%>, <% tcWebApi_get("String_Entry","TZ38","s")%>, <% tcWebApi_get("String_Entry","TZ33_1","s")%>, <% tcWebApi_get("String_Entry","TZ87","s")%>"],
	["GMT-02:00",	"(GMT+02:00) <% tcWebApi_get("String_Entry","TZ43_2","s")%>, <% tcWebApi_get("String_Entry","TZ39","s")%>, <% tcWebApi_get("String_Entry","TZ42","s")%>, <% tcWebApi_get("String_Entry","TZ43","s")%>, <% tcWebApi_get("String_Entry","TZ88","s")%>"],
	["GMT-03:00",	"(GMT+03:00) <% tcWebApi_get("String_Entry","TZ48","s")%>, <% tcWebApi_get("String_Entry","TZ48_1","s")%>, <% tcWebApi_get("String_Entry","TZ40_1","s")%>, <% tcWebApi_get("String_Entry","TZ47","s")%>, <% tcWebApi_get("String_Entry","TZ46","s")%>, <% tcWebApi_get("String_Entry","TZ44","s")%>, <% tcWebApi_get("String_Entry","TZ45","s")%>"],
	["GMT-03:30",	"(GMT+03:30) <% tcWebApi_get("String_Entry","TZ49","s")%>"],
	["GMT-04:00",	"(GMT+04:00) <% tcWebApi_get("String_Entry","TZ50","s")%>, <% tcWebApi_get("String_Entry","TZ50_1","s")%>, <% tcWebApi_get("String_Entry","TZ50_2","s")%>, <% tcWebApi_get("String_Entry","TZ51","s")%>"],
	["GMT-04:30",	"(GMT+04:30) <% tcWebApi_get("String_Entry","TZ52","s")%>"],
	["GMT-05:00",	"(GMT+05:00) <% tcWebApi_get("String_Entry","TZ54","s")%>,  <% tcWebApi_get("String_Entry","TZ53","s")%>"],
	["GMT-05:30",	"(GMT+05:30) <% tcWebApi_get("String_Entry","TZ55","s")%>, <% tcWebApi_get("String_Entry","TZ56","s")%>, <% tcWebApi_get("String_Entry","TZ59","s")%>"],
	["GMT-05:45",   "(GMT+05:45) <% tcWebApi_get("String_Entry","TZ57","s")%>"],
	["GMT-06:00",	"(GMT+06:00) <% tcWebApi_get("String_Entry","TZ60","s")%>, <% tcWebApi_get("String_Entry","TZ62_1","s")%>, <% tcWebApi_get("String_Entry","TZ58","s")%>"],
	["GMT-06:30",	"(GMT+06:30) <% tcWebApi_get("String_Entry","TZ61","s")%>"],
	["GMT-07:00",	"(GMT+07:00) <% tcWebApi_get("String_Entry","TZ62","s")%>, <% tcWebApi_get("String_Entry","TZ63","s")%>"],
	["GMT-08:00",	"(GMT+08:00) <% tcWebApi_get("String_Entry","TZ64","s")%>, <% tcWebApi_get("String_Entry","TZ67","s")%>, <% tcWebApi_get("String_Entry","TZ66","s")%>, <% tcWebApi_get("String_Entry","TZ68","s")%>, <% tcWebApi_get("String_Entry","TZ65","s")%>, <% tcWebApi_get("String_Entry","TZ69","s")%>, <% tcWebApi_get("String_Entry","TZ70","s")%>"],
	["GMT-09:00",	"(GMT+09:00) <% tcWebApi_get("String_Entry","TZ70_1","s")%>, <% tcWebApi_get("String_Entry","TZ71","s")%>, <% tcWebApi_get("String_Entry","TZ72","s")%>"],
	["GMT-09:30",	"(GMT+09:30) <% tcWebApi_get("String_Entry","TZ73","s")%>, <% tcWebApi_get("String_Entry","TZ74","s")%>"],
	["GMT-10:00",	"(GMT+10:00) <% tcWebApi_get("String_Entry","TZ76","s")%>, <% tcWebApi_get("String_Entry","TZ75","s")%>, <% tcWebApi_get("String_Entry","TZ77","s")%>, <% tcWebApi_get("String_Entry","TZ79","s")%>, <% tcWebApi_get("String_Entry","TZ82_1","s")%>, <% tcWebApi_get("String_Entry","TZ78","s")%>"],
	["GMT-11:00",	"(GMT+11:00) <% tcWebApi_get("String_Entry","TZ80","s")%>, <% tcWebApi_get("String_Entry","TZ81","s")%>, <% tcWebApi_get("String_Entry","TZ78","s")%>, <% tcWebApi_get("String_Entry","TZ86","s")%>"],
	["GMT-12:00",	"(GMT+12:00) <% tcWebApi_get("String_Entry","TZ82","s")%>, <% tcWebApi_get("String_Entry","TZ83","s")%>, <% tcWebApi_get("String_Entry","TZ85","s")%>"],
	["GMT-13:00",   "(GMT+13:00) <% tcWebApi_get("String_Entry","TZ84","s")%>"]
	];

function load_timezones(){
	free_options(document.form.uiViewdateToolsTZ);
	for(var i = 0; i < timezones.length; i++){
		if( (i==16 || i==18) && document.form.uiTimezoneSecond.value != 1){
			add_option(document.form.uiViewdateToolsTZ,
							timezones[i][1],
							timezones[i][0],
							false
						  );
		}else{
			add_option(document.form.uiViewdateToolsTZ,
							timezones[i][1],
							timezones[i][0],
							("<% tcWebApi_get("Timezone_Entry","TZ","s")%>" == timezones[i][0])
						  );
		}
	}
}

function remove_btn_ez_radiotoggle(){
	if((model_name == "DSL-N10-C1")||(model_name == "DSL-N10P-C1")||(model_name == "DSL-N12E-C1")||(model_name == "DSL-N10-D1")||(model_name == "DSL-N12U-C1")||(model_name == "DSL-N12U-D1")||(model_name == "DSL-N14U")||(model_name == "DSL-N14U-B1"))
	{
		document.getElementById('radio2').style.display = "none";
		document.getElementById('radio_text2').style.display = "none";
	}

	if((model_name == "DSL-N66U")||(model_name == "DSL-AC56U")||(model_name == "DSL-N17U"))
	{
		//DSL-N66U has an individual WiFi on/off button
		document.getElementById('radio1').style.display = "none";
		document.getElementById('radio_text1').style.display = "none";
	}
}

//parse nvram to array
var parseNvramToArray = function(oriNvram) {
	var parseArray = [];
	var oriNvramRow = decodeURIComponent(oriNvram).split('<');
	for(var i = 1; i < oriNvramRow.length; i++) {
		if(oriNvramRow[i] != "") {
			var oriNvramCol = oriNvramRow[i].split('>');
			var eachRuleArray = new Array();
			for(var j = 0; j < oriNvramCol.length; j++) {
				eachRuleArray.push(oriNvramCol[j]);
			}
			parseArray.push(eachRuleArray);
		}
	}
	return parseArray;
};

//parse type value to strings
var transformNumToText = function(restrict_type) {
	var bit_text_array = ["", "Web UI", "SSH", "Telnet"];
	var type_text = "";
	for(var i = 1; restrict_type != 0 && i <= 4; i += 1) {
		if(restrict_type & 1) {
			if(type_text == "")
				type_text += bit_text_array[i];
			else 
				type_text += ", " + bit_text_array[i];
		}
		restrict_type = restrict_type >> 1;
	}
	return type_text;
};

function initial(){
	
	orig_restrict_rulelist_array = parseNvramToArray('<% TCWebApi_get("Firewall_Entry","restrict_rulelist","s") %>');
	restrict_rulelist_array = JSON.parse(JSON.stringify(orig_restrict_rulelist_array));	

	show_menu();
	autoFocus('<% get_parameter("af"); %>');
	show_http_clientlist();
	remove_btn_ez_radiotoggle();	
	load_timezones();
	corrected_timezone(DAYLIGHT_orig, TZ_orig);

	init_reboot_schedule_setting();
	init_rewan_schedule_setting();

	hideport(document.form.misc_http_x[0].checked);
	if("<% TCWebApi_get("Firewall_Entry","enable_acc_restriction","s") %>" != "1")	{
		display_spec_IP(0);
	}
	
	if(HTTPS_support == -1){
		document.getElementById("http_auth_table").style.display = "none";		
	}
	else{
		hide_https_lanport(document.form.http_enable.value);		
	}
	showLANIPList();
	document.getElementById("accessfromwan_port").style.display = (document.form.misc_http_x[0].checked == 1) ? "" : "none";

	if(HTTPS_support  == -1 || '<% tcWebApi_get("Https_Entry","http_enable","s") %>' == 1){
		document.getElementById("https_port_span").style.display = "none";
	}
	
	//Set default http_autologout if value null
	if("<% tcWebApi_Get("Misc_Entry", "http_autologout", "s") %>" != ""){
        	set_http_autologout = "<% tcWebApi_Get("Misc_Entry", "http_autologout", "s") %>";
	}
	else{
        	set_http_autologout = 30;
	}
	document.form.http_autologout.value = set_http_autologout;
	

	//Hide enable_telnet or not
	if(telnet_support == -1){
		document.getElementById("telnet_tr").style.display = "none";
		document.form.telnetd_enable[0].disabled = true;
		document.form.telnetd_enable[1].disabled = true;
	}
	else{
		document.getElementById("telnet_tr").style.display = "";
		document.form.telnetd_enable[0].disabled = false;
		document.form.telnetd_enable[1].disabled = false;
		document.form.shell_timeout_x.value = "<%tcWebApi_get("Misc_Entry","telnetd_timeout","s")%>";
	}

	//Hide SSH or not
	if(ssh_support == -1){
		document.getElementById('sshd_enable_tr').style.display = "none";
		document.getElementById('sshd_port_tr').style.display = "none";
		document.getElementById('sshd_password_tr').style.display = "none";
		document.getElementById('auth_keys_tr').style.display = "none";
	}
	else{
		check_sshd_enable('<% tcWebApi_get("SSH_Entry","Enable","s") %>');
		document.form.shell_timeout_x.value = "<%tcWebApi_get("SSH_Entry","timeout","s")%>";		
	}
	
	//Set default shell_timeout_x if value null
	if("<% tcWebApi_Get("Misc_Entry", "telnetd_timeout", "s") %>" != ""){
		set_shell_timeout = "<% tcWebApi_Get("Misc_Entry", "telnetd_timeout", "s") %>";
	}
	else if("<% tcWebApi_Get("SSH_Entry", "timeout" , "s") %>" != ""){
		set_shell_timeout = "<% tcWebApi_Get("SSH_Entry", "timeout", "s") %>";
	}
	else{
		set_shell_timeout = 20;
	}

	setTimeout("update_FAQ();", 300);
}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq", "https://www.asus.com", "/support/FAQ/", "1034294");
	}
}
function init_reboot_schedule_setting(){
	document.form.reboot_date_x_Sun.checked = getDateCheck(document.form.reboot_schedule.value, 0);
	document.form.reboot_date_x_Mon.checked = getDateCheck(document.form.reboot_schedule.value, 1);
	document.form.reboot_date_x_Tue.checked = getDateCheck(document.form.reboot_schedule.value, 2);
	document.form.reboot_date_x_Wed.checked = getDateCheck(document.form.reboot_schedule.value, 3);
	document.form.reboot_date_x_Thu.checked = getDateCheck(document.form.reboot_schedule.value, 4);
	document.form.reboot_date_x_Fri.checked = getDateCheck(document.form.reboot_schedule.value, 5);
	document.form.reboot_date_x_Sat.checked = getDateCheck(document.form.reboot_schedule.value, 6);
	document.form.reboot_time_x_hour.value = getrebootTimeRange(document.form.reboot_schedule.value, 0);
	document.form.reboot_time_x_min.value = getrebootTimeRange(document.form.reboot_schedule.value, 1);
	document.getElementById('reboot_schedule_enable_tr').style.display = "";
	hide_reboot_option("<%tcWebApi_Get("RebootSchedule_Entry","reboot_schedule_enable","s")%>");	
}

function init_rewan_schedule_setting(){
	document.form.rewan_date_x_Sun.checked = getDateCheck(document.form.rewan_schedule.value, 0);
	document.form.rewan_date_x_Mon.checked = getDateCheck(document.form.rewan_schedule.value, 1);
	document.form.rewan_date_x_Tue.checked = getDateCheck(document.form.rewan_schedule.value, 2);
	document.form.rewan_date_x_Wed.checked = getDateCheck(document.form.rewan_schedule.value, 3);
	document.form.rewan_date_x_Thu.checked = getDateCheck(document.form.rewan_schedule.value, 4);
	document.form.rewan_date_x_Fri.checked = getDateCheck(document.form.rewan_schedule.value, 5);
	document.form.rewan_date_x_Sat.checked = getDateCheck(document.form.rewan_schedule.value, 6);
	document.form.rewan_time_x_hour.value = getrebootTimeRange(document.form.rewan_schedule.value, 0);
	document.form.rewan_time_x_min.value = getrebootTimeRange(document.form.rewan_schedule.value, 1);
	document.getElementById('rewan_schedule_enable_tr').style.display = "";
	hide_rewan_option("<%tcWebApi_Get("RebootSchedule_Entry","rewan_schedule_enable","s")%>");	
}

function hideport(flag){
	document.getElementById("accessfromwan_port").style.display = (flag == 1) ? "" : "none";
	if(HTTPS_support == -1){
		document.getElementById("NSlookup_help_for_WAN_access").style.display = (flag == 1) ? "" : "none";
		var orig_str = document.getElementById("access_port_title").innerHTML;
		document.getElementById("access_port_title").innerHTML = orig_str.replace(/HTTPS/, "HTTP");
		document.getElementById("http_port_span").style.display = (flag == 1) ? "" : "none";
	}
	else{
		document.getElementById("WAN_access_hint").style.display = (flag == 1) ? "" : "none";
		document.getElementById("wan_access_url").style.display = (flag == 1) ? "" : "none";
		change_url(document.form.misc_httpsport_x.value, "https_wan");
		document.getElementById("https_port_span").style.display = (flag == 1) ? "" : "none";
	}	
}

var autoChange = false;
function enable_wan_access(flag){
	if(HTTPS_support != -1){
		if(flag == 1){
			if(document.form.http_enable.value == "1"){
				document.form.http_enable.selectedIndex = 2;
				autoChange = true;
				hide_https_lanport(document.form.http_enable.value);
			}
		}
		else{
			if(autoChange){
				document.form.http_enable.selectedIndex = 0;
				autoChange = false;
				hide_https_lanport(document.form.http_enable.value);
			}
		}
	}
}
function redirect(){
	if(document.form.http_enable.value == "1"){
		if(isFromWAN)
			document.location.href = "http://" + location.hostname + ":" + document.form.misc_httpport_x.value + "/cgi-bin/Advanced_System_Content.asp";
		else
			document.location.href = "http://" + location.hostname + "/cgi-bin/Advanced_System_Content.asp";

	}else if(document.form.http_enable.value == "2"){
		if(isFromWAN)
			document.location.href = "https://" + location.hostname + ":" + document.form.misc_httpsport_x.value + "/cgi-bin/Advanced_System_Content.asp";
		else
			document.location.href = "https://" + location.hostname + ":" + document.form.https_lanport.value + "/cgi-bin/Advanced_System_Content.asp";

	}else{
		if(isFromHTTPS){
			if(isFromWAN)
					document.location.href = "https://" + location.hostname + ":" + document.form.misc_httpsport_x.value + "/cgi-bin/Advanced_System_Content.asp";
			else
					document.location.href = "https://" + location.hostname + ":" + document.form.https_lanport.value + "/cgi-bin/Advanced_System_Content.asp";
		}else{
			if(isFromWAN)
					document.location.href = "http://" + location.hostname + ":" + document.form.misc_httpport_x.value + "/cgi-bin/Advanced_System_Content.asp";
			else
					document.location.href = "http://" + location.hostname + "/cgi-bin/Advanced_System_Content.asp";
		}
	}
	//document.location.href = "/cgi-bin/Advanced_System_Content.asp";
}

function uiSave() {
	if(validForm())
	{
		var rule_num = document.getElementById('http_clientlist_table').rows.length;
		var item_num = document.getElementById('http_clientlist_table').rows[0].cells.length;
		var tmp_value = "";

		//parse array to nvram
		var tmp_value = "";
		for(var i = 0; i < restrict_rulelist_array.length; i += 1) {
			if(restrict_rulelist_array[i].length != 0) {
				tmp_value += "<";
				for(var j = 0; j < restrict_rulelist_array[i].length; j += 1) {
					tmp_value += restrict_rulelist_array[i][j];
					if( (j + 1) != restrict_rulelist_array[i].length)
						tmp_value += ">";
				}
			}
		}
		if(tmp_value == "<"+"<% tcWebApi_Get("String_Entry", "IPC_VSList_Norule", "s") %>" || tmp_value == "<")
			tmp_value = "";	
		document.form.http_clientlist.value = tmp_value;

		if(document.form.http_clientlist.value == "" && document.form.http_client[0].checked == 1){
			alert("<% tcWebApi_Get("String_Entry", "JS_fieldblank", "s") %>");
			document.form.http_client_ip_x_0.focus();
			return false;
		}

		document.form.SaveRebootScheduler.value = 1;
		updateDateTime();

		document.form.SaveRewanScheduler.value = 1;
		update_rewanDateTime();

		document.form.SaveFirewall.value = 1;
		if(document.form.misc_http_x[0].checked == true)
				document.form.misc_http_x.value = "1";
		else
				document.form.misc_http_x.value = "0";

		if(HTTPS_support != -1){
			if((document.form.http_enable.value != document.form.prev_http_enable.value) || (document.form.https_lanport.value != document.form.prev_https_lanport.value)
				|| (document.form.adminFlag.value == 1) || (document.form.passwdFlag.value == 1)
			)
			{
				document.form.SaveHttps.value = 1;
			}
			else
			{
				document.form.SaveHttps.value = 0;
			}
		}
		
		if(ssh_support != -1){
			if(document.form.sshd_enable[0].checked){
				if(!validate_range(document.form.sshd_port, 1, 65535))
					return false;
			}
			else{
				document.form.sshd_port.disabled = true;
				document.form.sshd_pass[0].disabled = true;
				document.form.sshd_pass[1].disabled = true;
				document.form.sshd_authkeys.disabled = true;
			}  
    	
    			document.form.action_script.value = "restart_ssh";
			document.form.action_mode.value = "apply";
			document.form.SaveSSHd.value = 1;
		}	
		
	}
	else
	{
		//port range is not correct.
		return false;
	}

	if(document.form.uiViewTools_Password.value.length > 0)
		alert("<% tcWebApi_get("String_Entry","File_Pop_content_alert_desc10","s") %>");

        if(document.form.syslogServerAddr.value == "")
        {
                document.form.logFlag.value = 1;
                document.form.RemotelogEnable.value = 0;
                //document.form.syslogServerAddr.value = "N/A";
        }
        else
        {
                document.form.logFlag.value = 1;
                document.form.RemotelogEnable.value = 1;
        }

        document.form.RadioButtonFlag.value = 1;
        document.form.SaveTime.value = 1;
        
        if(telnet_support != -1)
        	document.form.SaveTelnetd.value = 1;
        <%if tcWebApi_Get("WebCustom_Entry", "isSwapFileSupport", "h") = "Yes" then%>
        if(document.form.swap_enable.value != "<%TCWebApi_get("SysInfo_Entry","swap_enable","s")%>")
                document.form.SaveSwap.value = 1;
        <%end if%>

        if(document.form.uiViewSNTPServer.value == "")
        {
                document.form.uiViewSNTPServer.value = "0.0.0.0";
        }

	if(document.form.uiViewdateToolsTZ[16].selected == true || document.form.uiViewdateToolsTZ[18].selected == true)
		document.form.uiTimezoneSecond.value = "1";
	else
		document.form.uiTimezoneSecond.value = "";

	if(ssh_support != -1 && is_gen_key == "0" && document.form.sshd_enable[0].checked){	//enable ssh at first time without rsa/dss key generated
		showLoading();
		document.getElementById("proceeding_txt").innerHTML = "<br><br>SSH key generation in progress, please wait for few minutes."; /* Untranslated */
		detect_gen_key();
	}
	else{
		showLoading(20);	//Extend from 10 to 20 to restore_webtype 
		setTimeout("redirect();", 20000);
	}
	document.form.submit();
}

var dead = 0;
function detect_gen_key(){
	$j.ajax({
		url: '/cgi-bin/update_gen_key.asp',
		dataType: 'script',
		error: function(xhr){
			dead++;
			if(dead < 20){
				setTimeout("detect_gen_key()", 1000);
			}
			else{
				setTimeout("redirect();", 1000);
			}
		},
		success: function(){
			if(is_gen_key == "0"){
				setTimeout("detect_gen_key()", 1000);
			}
			else{
				setTimeout("redirect();", 1000);
			}
		}
	});
}

function validForm(){

		showtext(document.getElementById("alert_msg1"), "");
		showtext(document.getElementById("alert_msg2"), "");
	//account
        if(document.form.uiViewTools_username.value.length == 0){
                showtext(document.getElementById("alert_msg1"), "<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc1","s")%>");
                document.form.uiViewTools_username.focus();
                document.form.uiViewTools_username.select();
                return false;
        }
        else{
                var alert_str = validate_hostname(document.form.uiViewTools_username);
                if(alert_str != ""){
                        showtext(document.getElementById("alert_msg1"), alert_str);
                        document.getElementById("alert_msg1").style.display = "";
                        document.form.uiViewTools_username.focus();
                        document.form.uiViewTools_username.select();
                        changeiuiBackground(0);
                        return false;
                }else{
                        document.getElementById("alert_msg1").style.display = "none";
                }

                document.form.uiViewTools_username.value = trim(document.form.uiViewTools_username.value);

				if(document.form.uiViewTools_username.value == "root"
                                || document.form.uiViewTools_username.value == "guest"
                                || document.form.uiViewTools_username.value == "anonymous"
                ){
                                showtext(document.getElementById("alert_msg1"), "<% tcWebApi_get("String_Entry","USB_App_account_alert","s") %>");
                                document.getElementById("alert_msg1").style.display = "";
                                document.form.uiViewTools_username.focus();
                                document.form.uiViewTools_username.select();
                                changeiuiBackground(0);
                                return false;
                }
                else if(accounts.getIndexByValue(document.form.uiViewTools_username.value) > 0
                                && document.form.uiViewTools_username.value != accounts[0]){
                                showtext(document.getElementById("alert_msg1"), "<% tcWebApi_get("String_Entry","File_Pop_content_alert_desc5","s") %>");
                                document.getElementById("alert_msg1").style.display = "";
                                document.form.uiViewTools_username.focus();
                                document.form.uiViewTools_username.select();
                                changeiuiBackground(0);
                                return false;
                }else{
                                document.getElementById("alert_msg1").style.display = "none";
                }

		if(document.form.uiViewTools_username.value != document.form.prev_username.value)
		{
			document.form.adminFlag.value = 1;
		}
	}

	//password
	if (document.form.uiViewTools_Password.value.length == 0 && document.form.uiViewTools_PasswordConfirm.value.length == 0){
                document.form.passwdFlag.value = 0;
        }
        else{
		if(document.form.uiViewTools_Password.value.length > 0 && document.form.uiViewTools_Password.value.length < 5){
                	showtext(document.getElementById("alert_msg2"),"* <% tcWebApi_get("String_Entry","JS_short_password","s") %>");
	                document.form.uiViewTools_Password.focus();
        	        document.form.uiViewTools_Password.select();
                	return false;
        	}

	        if(document.form.uiViewTools_Password.value != document.form.uiViewTools_PasswordConfirm.value){
        	        showtext(document.getElementById("alert_msg2"),"* <% tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s") %>");
	                if(document.form.uiViewTools_Password.value.length <= 0){
                	        document.form.uiViewTools_Password.focus();
        	                document.form.uiViewTools_Password.select();
	                }else{
                	        document.form.uiViewTools_PasswordConfirm.focus();
        	                document.form.uiViewTools_PasswordConfirm.select();
	                }

                	return false;
        	}

		if(document.form.uiViewTools_Password.value == '<% tcWebApi_get("Account_Entry0","default_passwd","s") %>'){
                        showtext(document.getElementById("alert_msg2"),"* <% tcWebApi_get("String_Entry","QIS_adminpass_confirm0","s") %>");
                        document.form.uiViewTools_Password.focus();
                        document.form.uiViewTools_Password.select();    
                        return false;
                }
		
	        if(!validate_string_webpassword(document.form.uiViewTools_Password)){
        	        document.form.uiViewTools_Password.focus();
                	document.form.uiViewTools_Password.select();
	                return false;
        	}

		//confirm common string combination
		var is_common_string = check_common_string(document.form.uiViewTools_Password.value, "httpd_password");
		if(document.form.uiViewTools_Password.value.length > 0 && is_common_string){
			if(!confirm("<% tcWebApi_get("String_Entry","JS_common_passwd","s") %>")){
				document.form.uiViewTools_Password.focus();
				document.form.uiViewTools_Password.select();
				return false;   
			}
		}

		if(document.form.uiViewTools_Password.value.length > 0) //setting web_passwd
		{
			document.form.passwdFlag.value = 1;
		}
	}

	if(ssh_support != -1){	//support SSH Daemon setting
		if(document.form.sshd_enable[0].checked && document.form.sshd_pass[1].checked){
			if(document.form.sshd_authkeys.value <= 0){
				alert("If Allow Password Login option change to [<%tcWebApi_get("String_Entry","checkbox_No","s")%>], Authorized Keys cannot be Null.");	/* Untranslated */
				document.form.sshd_authkeys.focus();
				return false;
			}
		}
	}

	if(!validate_range_sp(document.form.shell_timeout_x, 10, 999, set_shell_timeout))
                return false;

	if((document.form.adminFlag.value == 1) || (document.form.passwdFlag.value == 1))
	{
		document.form.accountFlag.value = 1;
	}

	if (document.form.misc_http_x[0].checked) {
		if (!validate_range(document.form.misc_httpport_x, 1024, 65535))
			return false;
		if(HTTPS_support != -1){
			if (!validate_range(document.form.https_lanport, 1024, 65535))
				return false;

			if (!validate_range(document.form.misc_httpsport_x, 1024, 65535))
				return false;
		}
	}
	else{
		document.form.misc_httpport_x.value = '<% tcWebApi_get("Firewall_Entry","misc_httpport_x", "s") %>';
		document.form.misc_httpsport_x.value = '<% tcWebApi_get("Firewall_Entry","misc_httpsport_x", "s") %>';
	}

	if(isPortConflict(document.form.misc_httpport_x.value)){
		alert(isPortConflict(document.form.misc_httpport_x.value));
		document.form.misc_httpport_x.focus();
		return false;
	}
	
	if(HTTPS_support != -1){
		if(isPortConflict(document.form.misc_httpsport_x.value)){
			alert(isPortConflict(document.form.misc_httpsport_x.value));
			document.form.misc_httpsport_x.focus();
			return false;
		}
		if(isPortConflict(document.form.https_lanport.value)){
			alert(isPortConflict(document.form.https_lanport.value));
			document.form.https_lanport.focus();
			return false;
		}
		/* misc_httpport_x not support while HTTPS_support
		if(document.form.misc_httpsport_x.value == document.form.misc_httpport_x.value){
			alert("<%tcWebApi_get("String_Entry","https_port_conflict","s")%>");
			document.form.misc_httpsport_x.focus();
			return false;
		}
		*/
	}
		
	if(!document.form.reboot_date_x_Sun.checked && !document.form.reboot_date_x_Mon.checked &&
		!document.form.reboot_date_x_Tue.checked && !document.form.reboot_date_x_Wed.checked &&
		!document.form.reboot_date_x_Thu.checked && !document.form.reboot_date_x_Fri.checked &&
		!document.form.reboot_date_x_Sat.checked && document.form.reboot_schedule_enable_x[0].checked)
	{
		alert(Untranslated.filter_lw_date_valid);
		document.form.reboot_date_x_Sun.focus();
		return false;
	}

	if(!document.form.rewan_date_x_Sun.checked && !document.form.rewan_date_x_Mon.checked &&
		!document.form.rewan_date_x_Tue.checked && !document.form.rewan_date_x_Wed.checked &&
		!document.form.rewan_date_x_Thu.checked && !document.form.rewan_date_x_Fri.checked &&
		!document.form.rewan_date_x_Sat.checked && document.form.rewan_schedule_enable_x[0].checked)
	{
		alert(Untranslated.filter_lw_date_valid);
		document.form.rewan_date_x_Sun.focus();
		return false;
	}
	
	if(!validate_range_sp(document.form.http_autologout, 10, 999, set_http_autologout))
		return false;

	//Not allowed no Web UI in restrict_rulelist_array
	var WebUI_selected=0
	if(document.form.http_client[0].checked && restrict_rulelist_array.length >0){	//Allow only specified IP address
		for(var x=0;x<restrict_rulelist_array.length;x++){
			if(restrict_rulelist_array[x][0] == 1 && 	//enabled rule && Web UI included
				(restrict_rulelist_array[x][2] == 1 || restrict_rulelist_array[x][2] == 3 || restrict_rulelist_array[x][2] == 5 || restrict_rulelist_array[x][2] == 7)){
				WebUI_selected++;
			}
		}

		if(WebUI_selected <= 0){
			alert("Please select at least one Web UI of Access Type and enable it in [Allow only specified IP address]");	//Untranslated 2017/08
			document.form.http_client_ip_x_0.focus();
			return false;
		}
	}

	return true;
}

/* password item show or not */
function pass_checked(obj){
	switchType(obj, document.form.show_pass_1.checked, true);
}

var header_info = [<% get_header_info(); %>];
var host_name = header_info[0].host;
if(host_name.split(":").length > 1)
	host_name = host_name.split(":")[0];
var theUrl = host_name;
function hide_https_lanport(_value){
	if(sw_mode == '1' || sw_mode == '2'){
		var https_lanport_num = "<% TCWebApi_get("Https_Entry","https_lanport","s") %>";
		document.getElementById("https_lanport_tr").style.display = (_value == "1") ? "none" : "";
		document.form.https_lanport.value = "<% TCWebApi_get("Https_Entry","https_lanport","s") %>";
		document.getElementById("https_access_page").innerHTML = "<%tcWebApi_get("String_Entry","https_access_url","s")%> ";
		document.getElementById("https_access_page").innerHTML += "<a href=\"https://"+theUrl+":"+https_lanport_num+"\" target=\"_blank\" style=\"color:#FC0;text-decoration: underline; font-family:Lucida Console;\">http<span>s</span>://"+theUrl+"<span>:"+https_lanport_num+"</span></a>";
		document.getElementById("https_access_page").style.display = (_value == "1") ? "none" : "";
	}
	else{
		document.getElementById("https_access_page").style.display = 'none';
	}
}

function check_wan_access(http_enable){
	if(http_enable == "1" && document.form.misc_http_x[0].checked == true){	//While Accesss from WAN enabled, not allow to set HTTP only
		alert("When \"Web Access from WAN\" is enabled, HTTPS must be enabled.");
		document.form.http_enable.selectedIndex = 2;
		hide_https_lanport(document.form.http_enable.value);
	}
}

// show clientlist
function show_http_clientlist(){
	
	var code = "";
	code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="list_table" id="http_clientlist_table">';
	if(restrict_rulelist_array.length == 0)
		code +='<tr><td style="color:#FFCC00;"><% tcWebApi_Get("String_Entry", "IPC_VSList_Norule", "s") %></td>';
	else{

		var Ctrl_enable= "";
		for(var i = 0; i < restrict_rulelist_array.length; i += 1) {
			code +='<tr id="row'+i+'">';
				if(restrict_rulelist_array[i][0] == "1")
					Ctrl_enable= "<%tcWebApi_get("String_Entry","btn_Enabled","s")%>";
				else
					Ctrl_enable= "<%tcWebApi_get("String_Entry","btn_Disabled","s")%>";

			code +='<td width="10%" title="'+ Ctrl_enable +'"><input type="checkbox" onclick="control_rule_status(this);" '+genChecked(restrict_rulelist_array[i][0])+'/></td>';
			code += '<td width="40%">';
			code += restrict_rulelist_array[i][1];
			code += "</td>";
			code += '<td width="40%">';
			code += transformNumToText(restrict_rulelist_array[i][2]);
			code += "</td>";
			code += '<td width="10%">';
			code += '<input class="remove_btn" type="button" onclick="deleteRow(this);"></td>';
			code += "</td>";
			code += '</tr>';
		}
	}
  	code +='</tr></table>';
	
	document.getElementById("http_clientlist_Block").innerHTML = code;
}

function control_rule_status(obj) {
	var $itemObj = $j(obj);
	var $trObj = $itemObj.closest('tr');
	var row_idx = $trObj.index();
	if($itemObj.is(":checked")) {
		restrict_rulelist_array[row_idx][0] = "1";
	}
	else {
		restrict_rulelist_array[row_idx][0] = "0";
	}

	if($j("#selAll").is(":checked"))
		$j("#selAll").attr('checked', false);
}

function genChecked(_flag){
	if(_flag == 1)
		return "checked";
	else
		return "";
}

function control_all_rule_status(obj) {
	var set_all_rule_status = function(stauts) {
		for(var i = 0; i < restrict_rulelist_array.length; i += 1) {
			restrict_rulelist_array[i][0] = stauts;
		}
	};

	var $itemObj = $j(obj);
	if($itemObj.is(":checked")) {		
		set_all_rule_status("1");
	}
	else {
		set_all_rule_status("0");
	}

	show_http_clientlist();
}

function check_Timefield_checkbox(){	// To check Date checkbox checked or not and control Time field disabled or not
	if( document.form.reboot_date_x_Sun.checked == true 
		|| document.form.reboot_date_x_Mon.checked == true 
		|| document.form.reboot_date_x_Tue.checked == true
		|| document.form.reboot_date_x_Wed.checked == true
		|| document.form.reboot_date_x_Thu.checked == true
		|| document.form.reboot_date_x_Fri.checked == true	
		|| document.form.reboot_date_x_Sat.checked == true	){
			inputCtrl(document.form.reboot_time_x_hour,1);
			inputCtrl(document.form.reboot_time_x_min,1);
			document.form.reboot_schedule.disabled = false;
	}
	else{
			inputCtrl(document.form.reboot_time_x_hour,0);
			inputCtrl(document.form.reboot_time_x_min,0);
			document.form.reboot_schedule.disabled = true;
			document.getElementById('reboot_schedule_time_tr').style.display ="";
	}		
}

function check_rewanTimefield_checkbox(){	// To check Date checkbox checked or not and control Time field disabled or not
	if( document.form.rewan_date_x_Sun.checked == true 
		|| document.form.rewan_date_x_Mon.checked == true 
		|| document.form.rewan_date_x_Tue.checked == true
		|| document.form.rewan_date_x_Wed.checked == true
		|| document.form.rewan_date_x_Thu.checked == true
		|| document.form.rewan_date_x_Fri.checked == true	
		|| document.form.rewan_date_x_Sat.checked == true	){
			inputCtrl(document.form.rewan_time_x_hour,1);
			inputCtrl(document.form.rewan_time_x_min,1);
			document.form.rewan_schedule.disabled = false;
	}
	else{
			inputCtrl(document.form.rewan_time_x_hour,0);
			inputCtrl(document.form.rewan_time_x_min,0);
			document.form.rewan_schedule.disabled = true;
			document.getElementById('rewan_schedule_time_tr').style.display ="";
	}		
}

function setClientIP(ipaddr){
	document.form.http_client_ip_x_0.value = ipaddr;
	hideClients_Block();
	over_var = 0;
}

var over_var = 0;
var isMenuopen = 0;

function hideClients_Block(){
	document.getElementById("pull_arrow").src = "/images/arrow-down.gif";
	document.getElementById('ClientList_Block_PC').style.display='none';
	isMenuopen = 0;
}

function pullLANIPList(obj){
	if(isMenuopen == 0){		
		obj.src = "/images/arrow-top.gif"
		document.getElementById("ClientList_Block_PC").style.display = 'block';		
		document.form.http_client_ip_x_0.focus();		
		isMenuopen = 1;
	}
	else
		hideClients_Block();
}

function deleteRow(r){
	var i=r.parentNode.parentNode.rowIndex;
	document.getElementById('http_clientlist_table').deleteRow(i);
  
	restrict_rulelist_array.splice(i,1);

	if(restrict_rulelist_array.length == 0)
		show_http_clientlist();
}

function addRow(obj, upper){
	if('<% TCWebApi_get("Firewall_Entry","enable_acc_restriction","h") %>' != "1")
		document.form.http_client[0].checked = true;
		
	//Viz check max-limit 
	var rule_num = restrict_rulelist_array.length;
	if(rule_num >= upper){
		alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
		return false;	
	}
			
	if(obj.value == ""){
		alert(" <% tcWebApi_Get("String_Entry", "JS_fieldblank", "s") %>");
		obj.focus();
		obj.select();			
		return false;
	}
	else if(validator.validIPForm(obj, 2) != true){
		return false;
	}
	
	var access_type_value = 0;
	$j(".access_type").each(function() {
		if(this.checked)
			access_type_value += parseInt($j(this).val());
	});	

	if(access_type_value == 0) {
		alert("Please select at least one Access Type.");/*untranslated*/
		return false;
	}
	else{		

		var newRuleArray = new Array();
		if(document.getElementById("newrule_Enable").checked)
			newRuleArray.push("1");
		else
			newRuleArray.push("0");		
		newRuleArray.push(obj.value.trim());
		newRuleArray.push(access_type_value.toString());
		var newRuleArray_tmp = new Array();
		newRuleArray_tmp = newRuleArray.slice();
		newRuleArray_tmp.splice(0, 1);
		newRuleArray_tmp.splice(1, 1);
		if(restrict_rulelist_array.length > 0) {
			for(var i = 0; i < restrict_rulelist_array.length; i += 1) {
				var restrict_rulelist_array_tmp = restrict_rulelist_array[i].slice();
				restrict_rulelist_array_tmp.splice(0, 1);
				restrict_rulelist_array_tmp.splice(1, 1);
				if(newRuleArray_tmp.toString() == restrict_rulelist_array_tmp.toString()) { //compare IP
					alert("<% tcWebApi_Get("String_Entry", "JS_duplicate", "s") %>");
					return false;
				}
			}
		}
		restrict_rulelist_array.push(newRuleArray);
		
		$j(".access_type").each(function() {
			if(this.checked)
				$j(this).prop('checked', false);
		});
		obj.value = "";
		show_http_clientlist();
	}	
}

function display_spec_IP(flag){
	if(flag == 0){
			document.getElementById("http_client_table").style.display = "none";
			document.getElementById("http_clientlist_Block").style.display = "none";
	}
	else{
			document.getElementById("http_client_table").style.display = "";
			document.getElementById("http_clientlist_Block").style.display = "";
	}
}

//Viz add 2012.12 show url for https [start]
function change_url(num, flag){
	if(flag == 'https_lan'){
		var https_lanport_num_new = num;
		document.getElementById("https_access_page").innerHTML = "<%tcWebApi_get("String_Entry","https_access_url","s")%> ";
		document.getElementById("https_access_page").innerHTML += "<a href=\"https://"+theUrl+":"+https_lanport_num_new+"\" target=\"_blank\" style=\"color:#FC0;text-decoration: underline; font-family:Lucida Console;\">http<span>s</span>://"+theUrl+"<span>:"+https_lanport_num_new+"</span></a>";
	}
	else if(flag == 'https_wan'){
		var https_wanport = num;
		var host_addr = "";
		if(ddns_enable_x == "1" && ddns_hostname_x_t.length != 0)
			host_addr = ddns_hostname_x_t;	
		else
			host_addr = wan_ipaddr;

		document.getElementById("wan_access_url").innerHTML = "<%tcWebApi_get("String_Entry","https_access_url","s")%> ";
		document.getElementById("wan_access_url").innerHTML += "<a href=\"https://"+host_addr+":"+https_wanport+"\" target=\"_blank\" style=\"color:#FC0;text-decoration: underline; font-family:Lucida Console;\">http<span>s</span>://"+host_addr+"<span>:"+https_wanport+"</span></a>";

	}
}
//Viz add 2012.12 show url for https [end]

function showLANIPList(){	
	if(clientList.length == 0){
		setTimeout(function() {
			genClientList();
			showLANIPList();
		}, 500);
		return false;
	}
	
	var htmlCode = "";		
	for(var i=0; i<clientList.length;i++){
		var clientObj = clientList[clientList[i]];

		if(clientObj.IP == "offline") clientObj.IP = "";
		if(clientObj.Name.length > 30) clientObj.Name = clientObj.Name.substring(0, 27) + "...";

		htmlCode += '<a><div onmouseover="over_var=1;" onmouseout="over_var=0;" onclick="setClientIP(\'';
		htmlCode += clientObj.IP;		
		htmlCode += '\');"><strong>';
		htmlCode += clientObj.IP;
		htmlCode += '</strong> ( ';
		htmlCode += clientObj.Name;
		htmlCode += ' )</div></a><!--[if lte IE 6.5]><iframe class="hackiframe2"></iframe><![endif]-->';	
	}

	document.getElementById("ClientList_Block_PC").innerHTML = htmlCode;
}

function clean_scorebar(obj){
	if(obj.value == "")
		document.getElementById("scorebarBorder").style.display = "none";
}

function check_sshd_enable(obj_value){
	if(obj_value == 'Yes'){
		document.getElementById('sshd_port_tr').style.display = "";		
		document.getElementById('sshd_password_tr').style.display = "";
		document.getElementById('auth_keys_tr').style.display = "";
	}
	else{
		document.getElementById('sshd_port_tr').style.display = "none";
		document.getElementById('sshd_password_tr').style.display = "none";
		document.getElementById('auth_keys_tr').style.display = "none";	
	}
}

function hide_reboot_option(flag){
	document.getElementById("reboot_schedule_date_tr").style.display = (flag == 1) ? "" : "none";
	document.getElementById("reboot_schedule_time_tr").style.display = (flag == 1) ? "" : "none";
	if(flag == 1)
		check_Timefield_checkbox();
}

function getrebootTimeRange(str, pos)
{
	if (pos == 0)
		return str.substring(7,9);
	else if (pos == 1)
		return str.substring(9,11);
}

function setrebootTimeRange(rd, rh, rm)
{
	return(rd.value+rh.value+rm.value);
}

function updateDateTime()
{
	if(document.form.reboot_schedule_enable_x[0].checked == true){
		document.form.reboot_schedule_enable.value = "1";
		document.form.reboot_schedule.value = setDateCheck(
			document.form.reboot_date_x_Sun,
			document.form.reboot_date_x_Mon,
			document.form.reboot_date_x_Tue,
			document.form.reboot_date_x_Wed,
			document.form.reboot_date_x_Thu,
			document.form.reboot_date_x_Fri,
			document.form.reboot_date_x_Sat);
		document.form.reboot_schedule.value = setrebootTimeRange(
			document.form.reboot_schedule,
			document.form.reboot_time_x_hour,
			document.form.reboot_time_x_min);
	}
	else{
		document.form.reboot_schedule_enable.value = "0";
		document.form.reboot_schedule.value = "<% TCWebApi_get("RebootSchedule_Entry","reboot_schedule","s") %>";
	}
}

function hide_rewan_option(flag){
	document.getElementById("rewan_schedule_date_tr").style.display = (flag == 1) ? "" : "none";
	document.getElementById("rewan_schedule_time_tr").style.display = (flag == 1) ? "" : "none";
	if(flag == 1)
		check_rewanTimefield_checkbox();
}

function getrewanTimeRange(str, pos)
{
	if (pos == 0)
		return str.substring(7,9);
	else if (pos == 1)
		return str.substring(9,11);
}

function setrewanTimeRange(rd, rh, rm)
{
	return(rd.value+rh.value+rm.value);
}

function update_rewanDateTime()
{
	if(document.form.rewan_schedule_enable_x[0].checked == true){
		document.form.rewan_schedule_enable.value = "1";
		document.form.rewan_schedule.value = setDateCheck(
			document.form.rewan_date_x_Sun,
			document.form.rewan_date_x_Mon,
			document.form.rewan_date_x_Tue,
			document.form.rewan_date_x_Wed,
			document.form.rewan_date_x_Thu,
			document.form.rewan_date_x_Fri,
			document.form.rewan_date_x_Sat);
		document.form.rewan_schedule.value = setrewanTimeRange(
			document.form.rewan_schedule,
			document.form.rewan_time_x_hour,
			document.form.rewan_time_x_min);
	}
	else{
		document.form.rewan_schedule_enable.value = "0";
		document.form.rewan_schedule.value = "<% TCWebApi_get("RebootSchedule_Entry","rewan_schedule","s") %>";
	}
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_System_Content.asp" name="form" target="hidden_frame">
<INPUT TYPE="HIDDEN" NAME="action_script" VALUE="">
<INPUT TYPE="HIDDEN" NAME="action_mode" VALUE="">
<INPUT TYPE="HIDDEN" NAME="accountFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="adminFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="passwdFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="uiViewTools_empty" VALUE="">
<INPUT TYPE="HIDDEN" NAME="syslogEnable" VALUE="Yes">
<INPUT TYPE="HIDDEN" NAME="logLevelSelect" VALUE="7">
<INPUT TYPE="HIDDEN" NAME="DisplayLevelSelect" VALUE="<%tcWebApi_get("Syslog_Entry","DisplayLevel","s")%>">
<INPUT TYPE="HIDDEN" NAME="RemotelogEnable" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="serverPort" VALUE="514">
<INPUT TYPE="HIDDEN" NAME="logFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="remoteSyslog" VALUE="<%tcWebApi_get("WebCustom_Entry","isRemoteSyslog","s")%>">
<INPUT TYPE="HIDDEN" NAME="RadioButtonFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="radiotoggle_original" VALUE="<%tcWebApi_get("SysInfo_Entry","btn_ez_radiotoggle","s")%>">
<INPUT TYPE="HIDDEN" name="SaveTime" VALUE="0">
<INPUT TYPE="HIDDEN" name="uiViewSyncWith" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="uiClearPCSyncFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="uiTimezoneType" VALUE="<%TCWebApi_get("Timezone_Entry","TYPE","s")%>">
<INPUT TYPE="HIDDEN" name="uiTimezoneSecond" VALUE="<%tcWebApi_get("Timezone_Entry","TZ_second","s")%>">
<INPUT TYPE="HIDDEN" name="SaveSSHd" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveTelnetd" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveFirewall" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveHttps" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveSwap" VALUE="0">
<INPUT TYPE="HIDDEN" name="tcWebApi_Save_Flag" VALUE="1">

<INPUT TYPE="HIDDEN" name="prev_http_enable" VALUE="<%tcWebApi_get("Https_Entry","http_enable","s")%>">
<INPUT TYPE="HIDDEN" name="prev_https_lanport" VALUE="<%tcWebApi_get("Https_Entry","https_lanport","s")%>">
<INPUT TYPE="HIDDEN" name="prev_username" VALUE="<%tcWebApi_get("Account_Entry0","username","s")%>">
<INPUT TYPE="HIDDEN" name="http_clientlist" value="<% TCWebApi_get("Firewall_Entry","restrict_rulelist","s") %>">
<INPUT TYPE="HIDDEN" name="SaveRebootScheduler" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveRewanScheduler" VALUE="0">
<INPUT TYPE="HIDDEN" name="reboot_schedule_enable" value="<% TCWebApi_get("RebootSchedule_Entry","reboot_schedule_enable","s") %>">
<INPUT TYPE="HIDDEN" name="reboot_schedule" value="<% TCWebApi_get("RebootSchedule_Entry","reboot_schedule","s") %>">
<INPUT TYPE="HIDDEN" name="rewan_schedule_enable" value="<% TCWebApi_get("RebootSchedule_Entry","rewan_schedule_enable","s") %>">
<INPUT TYPE="HIDDEN" name="rewan_schedule" value="<% TCWebApi_get("RebootSchedule_Entry","rewan_schedule","s") %>">

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
			<td valign="top">
				<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
				<tbody>
				<tr>
					<td bgcolor="#4D595D" valign="top">
						<div>&nbsp;</div>
						<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","menu5_6_2","s")%></div>
						<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
						<div class="formfontdesc"><%tcWebApi_get("String_Entry","Syste_title","s")%></div>
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
							<thead>
								<tr>
									<td colspan="2"><%tcWebApi_get("String_Entry","PASS_changepasswd","s")%></td>
								</tr>
							</thead>
							<tr>
								<th width="40%"><% tcWebApi_Get("String_Entry", "Router_Login_Name", "s") %></th>
								<td>
									<input type="text" name="uiViewTools_username" maxlength="20" value="<% tcWebApi_Get("Account_Entry0","username","s") %>" onKeyPress="return validator.isString(this, event);"" class="input_18_table" autocapitalization="off" autocomplete="off">
									<span id="alert_msg1" style="color:#FFCC00;"></span>
								</td>
							</tr>
							<tr>
								<th width="40%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,4)"><%tcWebApi_get("String_Entry","PASS_new","s")%></a></th>
								<td>
									<input type="password" autocapitalization="off" autocomplete="off" name="uiViewTools_Password" maxlength="16" value="" onKeyPress="return validator.isString(this, event);" onKeyUp="chkPass(this.value, 'http_passwd', 16);" onPaste="return false;" class="input_18_table" onBlur="clean_scorebar(this);">
										&nbsp;&nbsp;
									<div id="scorebarBorder" name="scorebarBorder" style="margin-left:180px; margin-top:-25px; display:none;" title="<%tcWebApi_get("String_Entry","LHC_x_Password_itemSecur","s")%>">
										<div id="score" name="score"></div>
										<div id="scorebar" name="scorebar">&nbsp;</div>
									</div>									
								</td>
							</tr>
							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,4)"><%tcWebApi_get("String_Entry","PASS_retype","s")%></a></th>
								<td>
									<INPUT TYPE="PASSWORD" autocapitalization="off" autocomplete="off" NAME="uiViewTools_PasswordConfirm" MAXLENGTH="16" VALUE="" onKeyPress="return validator.isString(this, event);" onpaste="return false;" class="input_18_table">
									<div style="margin:-25px 0px 5px 175px;"><input type="checkbox" name="show_pass_1" onclick="pass_checked(document.form.uiViewTools_Password);pass_checked(document.form.uiViewTools_PasswordConfirm);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
									<span id="alert_msg2" style="color:#FC0;margin-left:8px;"></span>
								</td>
							</tr>
						</table>						
						
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:8px;">
							<thead>
							<tr>
								<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
							</tr>
							</thead>
							<tr id="btn_ez_radiotoggle_tr">
								<th><%tcWebApi_get("String_Entry","WPS_btn_behavior","s")%></th>
								<td>
									<input type="radio" id="radio2" name="btn_ez_radiotoggle" class="input" value="2" <% if tcWebApi_get("SysInfo_Entry","btn_ez_radiotoggle","h") = "2" then asp_Write("checked") end if %>><label id="radio_text2"><%tcWebApi_get("String_Entry","LED_switch","s")%></label>
									<input type="radio" id="radio1" name="btn_ez_radiotoggle" class="input" value="1" <% if tcWebApi_get("SysInfo_Entry","btn_ez_radiotoggle","h") = "1" then asp_Write("checked") end if %>><label id="radio_text1"><%tcWebApi_get("String_Entry","WPS_btn_toggle","s")%></label>
									<input type="radio" id="radio0" name="btn_ez_radiotoggle" class="input" value="0" <% if tcWebApi_get("SysInfo_Entry","btn_ez_radiotoggle","h") = "0" then asp_Write("checked") end if %>><label id="radio_text0"><%tcWebApi_get("String_Entry","WPS_btn_actWPS","s")%></label>
								</td>
							</tr>
							<tr id="reboot_schedule_enable_tr">
								<th>Enable Reboot Scheduler</th>
								<td>
									<input type="radio" value="1" name="reboot_schedule_enable_x" onClick="hide_reboot_option(1);" <% if tcWebApi_get("RebootSchedule_Entry","reboot_schedule_enable","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" value="0" name="reboot_schedule_enable_x" onClick="hide_reboot_option(0);" <% if tcWebApi_get("RebootSchedule_Entry","reboot_schedule_enable","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr>
							<tr id="reboot_schedule_date_tr">
								<th>Date to Reboot</th>
								<td>
									<input type="checkbox" name="reboot_date_x_Sun" class="input" onclick="check_Timefield_checkbox();"><%tcWebApi_get("String_Entry","date_Sun_id","s")%>
									<input type="checkbox" name="reboot_date_x_Mon" class="input" onclick="check_Timefield_checkbox();"><%tcWebApi_get("String_Entry","date_Mon_id","s")%>
									<input type="checkbox" name="reboot_date_x_Tue" class="input" onclick="check_Timefield_checkbox();"><%tcWebApi_get("String_Entry","date_Tue_id","s")%>
									<input type="checkbox" name="reboot_date_x_Wed" class="input" onclick="check_Timefield_checkbox();"><%tcWebApi_get("String_Entry","date_Wed_id","s")%>
									<input type="checkbox" name="reboot_date_x_Thu" class="input" onclick="check_Timefield_checkbox();"><%tcWebApi_get("String_Entry","date_Thu_id","s")%>
									<input type="checkbox" name="reboot_date_x_Fri" class="input" onclick="check_Timefield_checkbox();"><%tcWebApi_get("String_Entry","date_Fri_id","s")%>
									<input type="checkbox" name="reboot_date_x_Sat" class="input" onclick="check_Timefield_checkbox();"><%tcWebApi_get("String_Entry","date_Sat_id","s")%>
								</td>
							</tr>
							<tr id="reboot_schedule_time_tr">
								<th>Time of Day to Reboot</th>
								<td>
									<input type="text" maxlength="2" class="input_3_table" name="reboot_time_x_hour" onKeyPress="return validator.isNumber(this,event);" onblur="validator.timeRange(this, 0);" autocorrect="off" autocapitalize="off"> :
									<input type="text" maxlength="2" class="input_3_table" name="reboot_time_x_min" onKeyPress="return validator.isNumber(this,event);" onblur="validator.timeRange(this, 1);" autocorrect="off" autocapitalize="off">
								</td>
							</tr>

							<tr id="rewan_schedule_enable_tr">
								<th>Enable WAN Reconnect Scheduler</th>
								<td>
									<input type="radio" value="1" name="rewan_schedule_enable_x" onClick="hide_rewan_option(1);" <% if tcWebApi_get("RebootSchedule_Entry","rewan_schedule_enable","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" value="0" name="rewan_schedule_enable_x" onClick="hide_rewan_option(0);" <% if tcWebApi_get("RebootSchedule_Entry","rewan_schedule_enable","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr>
							<tr id="rewan_schedule_date_tr">
								<th>Date to do WAN Reconnect</th>
								<td>
									<input type="checkbox" name="rewan_date_x_Sun" class="input" onclick="check_rewanTimefield_checkbox();"><%tcWebApi_get("String_Entry","date_Sun_id","s")%>
									<input type="checkbox" name="rewan_date_x_Mon" class="input" onclick="check_rewanTimefield_checkbox();"><%tcWebApi_get("String_Entry","date_Mon_id","s")%>
									<input type="checkbox" name="rewan_date_x_Tue" class="input" onclick="check_rewanTimefield_checkbox();"><%tcWebApi_get("String_Entry","date_Tue_id","s")%>
									<input type="checkbox" name="rewan_date_x_Wed" class="input" onclick="check_rewanTimefield_checkbox();"><%tcWebApi_get("String_Entry","date_Wed_id","s")%>
									<input type="checkbox" name="rewan_date_x_Thu" class="input" onclick="check_rewanTimefield_checkbox();"><%tcWebApi_get("String_Entry","date_Thu_id","s")%>
									<input type="checkbox" name="rewan_date_x_Fri" class="input" onclick="check_rewanTimefield_checkbox();"><%tcWebApi_get("String_Entry","date_Fri_id","s")%>
									<input type="checkbox" name="rewan_date_x_Sat" class="input" onclick="check_rewanTimefield_checkbox();"><%tcWebApi_get("String_Entry","date_Sat_id","s")%>
								</td>
							</tr>
							<tr id="rewan_schedule_time_tr">
								<th>Time of Day to do WAN Reconnect</th>
								<td>
									<input type="text" maxlength="2" class="input_3_table" name="rewan_time_x_hour" onKeyPress="return validator.isNumber(this,event);" onblur="validator.timeRange(this, 0);" autocorrect="off" autocapitalize="off"> :
									<input type="text" maxlength="2" class="input_3_table" name="rewan_time_x_min" onKeyPress="return validator.isNumber(this,event);" onblur="validator.timeRange(this, 1);" autocorrect="off" autocapitalize="off">
								</td>
							</tr>

							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,1)"><%tcWebApi_get("String_Entry","LHC_x_ServerLogEnable_in","s")%></a></th>
								<td>
									<INPUT TYPE="TEXT" class="input_15_table" NAME="syslogServerAddr" SIZE="25" MAXLENGTH="15" VALUE="<%if tcWebApi_get("Syslog_Entry","remoteHost","h") <> "N/A" then tcWebApi_get("Syslog_Entry","remoteHost","s") else asp_Write("") end if%>" onKeyPress="return validator.isIPAddr(this, event)">
								</td>
							</tr>
							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,2)"><%tcWebApi_get("String_Entry","LHC_x_TimeZone_in","s")%></a></th>
								<td>
									<SELECT NAME="uiViewdateToolsTZ" class="input_option" style="width:400px;" onChange="corrected_timezone(document.form.uiViewdateDS.value, this.value);">
									</SELECT><br>
									<span id="timezone_hint" style="display:none;"></span>
									<div>
									</div>
								</td>
							</tr>
							<tr>
								<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> Daylight Saving</th>
								<td>
									<INPUT TYPE="RADIO" NAME="uiViewdateDS" VALUE="Enable" onClick="corrected_timezone(this.value, document.form.uiViewdateToolsTZ.value);" <% If TCWebApi_get("Timezone_Entry","DAYLIGHT","h") = "Enable" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<INPUT TYPE="RADIO" NAME="uiViewdateDS" VALUE="Disable" onClick="corrected_timezone(this.value, document.form.uiViewdateToolsTZ.value);" <% If TCWebApi_get("Timezone_Entry","DAYLIGHT","h") = "Disable" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr>
							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,3)"><%tcWebApi_get("String_Entry","LHC_x_NTPServer_in","s")%></a></th>
								<td>
									<INPUT TYPE="TEXT" NAME="uiViewSNTPServer" SIZE="24" MAXLENGTH="48" VALUE="<%if tcWebApi_get("Timezone_Entry","SERVER","h") <> "0.0.0.0" then tcWebApi_get("Timezone_Entry","SERVER","s") else asp_Write("") end if%>" class="input_32_table" onKeyPress="return validator.isString(this, event);">
									<a href="javascript:openLink('x_NTPServer1')" name="x_NTPServer1_link" style=" margin-left:5px; text-decoration: underline;"><%tcWebApi_get("String_Entry","LHC_x_NTPServer1_linkname","s")%></a>
								</td>
							</tr>							

							<tr>
								<th><% tcWebApi_Get("String_Entry", "System_AutoLogout", "s") %></th>
								<td>
									<input type="text" class="input_3_table" maxlength="3" name="http_autologout" value="<% tcWebApi_Get("Misc_Entry", "http_autologout", "s") %>" onKeyPress="return validator.isNumber(this,event);">
									min<span> ( <% tcWebApi_Get("String_Entry", "zero_disable", "s") %> )</span>
								</td>
							</tr>

							<tr>
								<th align="right"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,6);"><% tcWebApi_Get("String_Entry", "Enable_redirect_notice", "s") %></a></th>
								<td>
									<input type="radio" name="nat_redirect_enable" class="input" value="1" <% If TCWebApi_get("SysInfo_Entry","nat_redirect_enable","h") <> "0" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" name="nat_redirect_enable" class="input" value="0" <% If TCWebApi_get("SysInfo_Entry","nat_redirect_enable","h") = "0" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr>

						<%if tcWebApi_Get("WebCustom_Entry", "isSwapFileSupport", "h") = "Yes" then%>
							<tr>
								<th align="right"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,8);">Enable Virtual Memory (swap)</a></th>
								<td>
									<input type="radio" name="swap_enable" class="input" value="1" <% If TCWebApi_get("SysInfo_Entry","swap_enable","h") = "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" name="swap_enable" class="input" value="0" <% If TCWebApi_get("SysInfo_Entry","swap_enable","h") = "0" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr>
						<%end if%>
						</table>


						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:8px;">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","qis_service","s")%></td>
						</tr>
						</thead>
						<tr id="telnet_tr">
							<th><%tcWebApi_get("String_Entry","Enable_Telnet","s")%></th>
							<td>
								<input type="radio" name="telnetd_enable" class="input" value="1" <% If TCWebApi_get("Misc_Entry","telnetd_enable","h") = "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="radio" name="telnetd_enable" class="input" value="0" <% If TCWebApi_get("Misc_Entry","telnetd_enable","h") = "0" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
						
						<tr id="sshd_enable_tr">
							<th width="40%"><%tcWebApi_get("String_Entry","Enable_SSH","s")%></th>
							<td>
								<input type="radio" name="sshd_enable" class="input" value="Yes" onclick="check_sshd_enable(this.value);" <% if tcWebApi_get("SSH_Entry","Enable","h") = "Yes" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="radio" name="sshd_enable" class="input" value="No" onclick="check_sshd_enable(this.value);" <% if tcWebApi_get("SSH_Entry","Enable","h") = "No" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
						<tr id="sshd_port_tr">
							<th width="40%">SSH Port</th>
							<td>
								<input type="text" class="input_6_table" maxlength="5" name="sshd_port" onKeyPress="return validator.isNumber(this,event);" value='<%tcWebApi_get("SSH_Entry","sshport","s")%>' autocorrect="off" autocapitalize="off">
							</td>
						</tr>				
						<tr id="sshd_password_tr">
							<th><%tcWebApi_get("String_Entry","Allow_PWLogin","s")%></th>
							<td>
								<input type="radio" name="sshd_pass" class="input" value="1" <% if tcWebApi_get("SSH_Entry","Need_Pass","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="radio" name="sshd_pass" class="input" value="0" <% if tcWebApi_get("SSH_Entry","Need_Pass","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
						<tr id="auth_keys_tr">
							<th>Authorized Keys</th>
							<td>
								<textarea rows="5" cols="55" class="textarea_ssh_table" name="sshd_authkeys" maxlength="1023"><%tcWebApi_get("SSH_Entry","Authkeys","s")%></textarea>
							</td>
						</tr>
						
						<tr>
							<th width="40%"><%tcWebApi_get("String_Entry","FreeWiFi_Idle","s")%></th>
							<td>
								<input type="text" class="input_3_table" maxlength="3" name="shell_timeout_x" value="" onKeyPress="return validator.isNumber(this,event);" autocorrect="off" autocapitalize="off"> <%tcWebApi_get("String_Entry","Minute","s")%>
								<span>(<%tcWebApi_get("String_Entry","zero_disable","s")%>)</span>
							</td>
						</tr>
					</table>

						<table id="http_auth_table" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:8px;">
							<thead>
							<tr>
								<td colspan="2">Local Access Config</td><!--untranslated-->
							</tr>
							</thead>
							<tr>
								<th><% tcWebApi_Get("String_Entry", "WC11b_AuthenticationMethod_in", "s") %></th>
								<td>
									<select name="http_enable" class="input_option" onchange="hide_https_lanport(this.value);check_wan_access(this.value);">
									<option value="1" <% if tcWebApi_get("Https_Entry","http_enable","h") = "1" then asp_Write("selected") end if %>>HTTP</option>
									<option value="2" <% if tcWebApi_get("Https_Entry","http_enable","h") = "2" then asp_Write("selected") end if %>>HTTPS</option>
									<option value="3" <% if tcWebApi_get("Https_Entry","http_enable","h") = "3" then asp_Write("selected") end if %>>BOTH</option>
									</select>
								</td>
							</tr>

							<tr id="https_lanport_tr">
								<th>HTTPS Lan port</th>
								<td>
									<input type="text" maxlength="5" class="input_6_table" name="https_lanport" value="<%TCWebApi_get("Https_Entry","https_lanport","s")%>" onKeyPress="return validator.isNumber(this,event);" onBlur="change_url(this.value, 'https_lan');">
									<span id="https_access_page"></span>
								</td>
							</tr>
						</table>	

						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable" style="margin-top:8px;">
							<thead>
							<tr>
								<td colspan="2">Remote Access Config</td><!--untranslated-->
							</tr>
							</thead>
							
							<tr id="misc_http_x_tr">
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,2);"><% tcWebApi_Get("String_Entry", "FC_x_WanWebEnable_in", "s") %></a></th>
								<td>
									<input type="radio" value="1" name="misc_http_x" class="input" onClick="hideport(1);enable_wan_access(1);return change_common_radio(this, 'FirewallConfig', 'misc_http_x', '1')" <% if tcWebApi_get("Firewall_Entry","misc_http_x","h") = "1" then asp_Write("checked") end if %>><% tcWebApi_Get("String_Entry", "checkbox_Yes", "s") %>
									<input type="radio" value="0" name="misc_http_x" class="input" onClick="hideport(0);enable_wan_access(0);return change_common_radio(this, 'FirewallConfig', 'misc_http_x', '0')" <% if tcWebApi_get("Firewall_Entry","misc_http_x","h") = "0" then asp_Write("checked") end if %>><% tcWebApi_Get("String_Entry", "checkbox_No", "s") %>
									<span class="formfontdesc" id="WAN_access_hint" style="color:#FFCC00; display:none;">Only HTTPS is supported when accessing web from WAN.
										<a id="faq" href="https://www.asus.com/support/FAQ/1034294" target="_blank" style="margin-left: 5px; color:#FFCC00; text-decoration: underline;">FAQ</a>
									</span><!--untranslated-->
									<div class="formfontdesc" id="NSlookup_help_for_WAN_access" style="color:#FFCC00; display:none;"><% tcWebApi_Get("String_Entry", "NSlookup_help", "s") %></div>
								</td>
							</tr>
							<tr id="accessfromwan_port">
								<th align="right"><a id="access_port_title" class="hintstyle" href="javascript:void(0);" onClick="openHint(8,3);">HTTPS <% tcWebApi_Get("String_Entry", "FC_x_WanWebPort_in", "s") %></a></th>
								<td>
									<span style="margin-left:5px; display:none;" id="http_port_span"><input type="text" maxlength="5" name="misc_httpport_x" class="input_6_table" value="<% tcWebApi_get("Firewall_Entry","misc_httpport_x", "s") %>" onKeyPress="return validator.isNumber(this,event);" autocorrect="off" autocapitalize="off"/>&nbsp;&nbsp;</span>
									<span style="margin-left:5px;" id="https_port_span"><input type="text" maxlength="5" name="misc_httpsport_x" class="input_6_table" value="<% tcWebApi_get("Firewall_Entry","misc_httpsport_x", "s") %>" onKeyPress="return validator.isNumber(this,event);" onBlur="change_url(this.value, 'https_wan');" autocorrect="off" autocapitalize="off"/></span>
									<span id="wan_access_url"></span>
								</td>
							</tr>
							
							<tr id="http_client_tr">
								<th><% tcWebApi_Get("String_Entry", "System_login_specified_IP", "s") %></th>
								<td>
									<input type="radio" name="http_client" class="input" value="1" onclick="display_spec_IP(1);" <% If TCWebApi_get("Firewall_Entry","enable_acc_restriction","h") = "1" then asp_Write("checked") end if%>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" name="http_client" class="input" value="0" onclick="display_spec_IP(0);" <% If TCWebApi_get("Firewall_Entry","enable_acc_restriction","h") <> "1" then asp_Write("checked") end if%>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr>
						</table><br>
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" id="http_client_table">
							<thead>
								<tr>
									<td colspan="4"><% tcWebApi_Get("String_Entry", "System_login_specified_Iplist", "s") %>&nbsp;(<% tcWebApi_Get("String_Entry", "List_limit", "s") %>&nbsp;4)</td>
								</tr>
							</thead>
							
							<tr>
								<th width="10%" title="<%tcWebApi_get("String_Entry","select_all","s")%>"><input id="selAll" type="checkbox" onclick="control_all_rule_status(this);"></th>
								<th width="40%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,9);"><% tcWebApi_Get("String_Entry", "FC_LanWanDstIP_in", "s") %></a></th>
								<th width="40%">Access Type</th>	<!--untranslated-->
								<th width="10%"><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
							</tr>

							<tr>
								<!-- client info -->
								<td width="10%" title="<%tcWebApi_get("String_Entry","btn_Enabled","s")%>/<%tcWebApi_get("String_Entry","btn_disable","s")%>"><input type="checkbox" id="newrule_Enable" checked></td>
								<td width="40%">
									<input type="text" class="input_25_table" maxlength="18" name="http_client_ip_x_0"  onKeyPress="" onClick="hideClients_Block();" autocorrect="off" autocapitalize="off">
									<img id="pull_arrow" height="14px;" src="/images/arrow-down.gif" style="position:absolute;*margin-left:-3px;*margin-top:1px;" onclick="pullLANIPList(this);" title="<% tcWebApi_Get("String_Entry", "select_client", "s") %>">	
									<div id="ClientList_Block_PC" class="clientlist_dropdown" style="margin-left:27px;width:235px;"></div>
								</td>
								<td width="40%">
									<input type="checkbox" name="access_webui" class="input access_type" value="1">Web UI<!--untranslated-->
									<input type="checkbox" name="access_ssh" class="input access_type" value="2">SSH(LAN only)<!--untranslated-->
									<input type="checkbox" name="access_telnet" class="input access_type" value="4">Telnet(LAN only)<!--untranslated-->
								</td>
								<td width="10%">									
									<input class="add_btn" type="button" onClick="addRow(document.form.http_client_ip_x_0, 4);" value="">
								</td>	
							</tr>
						</table>
						<div id="http_clientlist_Block"></div>
						<center><input name="button" type="button" class="button_gen" onclick="uiSave();" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/></center>
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
<div id="footer"></div>
</body>

<!--Advanced_System_Content.asp-->
</html>

