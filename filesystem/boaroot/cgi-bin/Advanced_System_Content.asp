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

If Request_Form("SaveTelnetd") = "1" Then
	TCWebApi_set("Misc_Entry","telnetd_enable","telnetd_enable")
	TCWebApi_set("Misc_Entry","http_autologout","http_autologout")
	tcWebApi_CommitWithoutSave("Misc_Entry")
End if

If Request_Form("SaveFirewall") = "1" Then
	TCWebApi_set("Firewall_Entry","misc_http_x","misc_http_x")
	TCWebApi_set("Firewall_Entry","misc_httpport_x","misc_httpport_x")
	TCWebApi_set("Firewall_Entry","misc_httpsport_x","misc_httpsport_x")
	TCWebApi_set("SysInfo_Entry","nat_redirect_enable","nat_redirect_enable")
	tcWebApi_CommitWithoutSave("Firewall_Entry")
End if

If tcWebApi_Get("WebCustom_Entry", "isSwapFileSupport", "h") = "Yes" then
	If Request_Form("SaveSwap") = "1" Then
		TCWebApi_set("SysInfo_Entry","swap_enable","swap_enable")
		do_swap()
	End if
End if

If Request_Form("http_client") <> "" Then
	TCWebApi_set("SysInfo_Entry","http_restrict_client","http_client")
	TCWebApi_set("SysInfo_Entry","http_clientlist","http_clientlist")
	update_http_clientlist()
End if

If Request_Form("tcWebApi_Save_Flag") = "1" Then
	tcWebApi_Save()
End if

If Request_Form("SaveHttps") = "1" Then
	TCWebApi_set("Https_Entry","http_enable","http_enable")
	TCWebApi_set("Https_Entry","https_lanport","https_lanport")
	tcWebApi_Commit("Https_Entry")
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
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var accounts = [<% get_all_accounts(); %>];

var isFromHTTPS = false;
if((location.href.search('https://') >= 0) || (location.href.search('HTTPS://') >= 0)){
        isFromHTTPS = true;
}

var http_clientlist_array = "<% TCWebApi_get("SysInfo_Entry","http_clientlist","s") %>";
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
	["GMT-03:00",	"(GMT+03:00) <% tcWebApi_get("String_Entry","TZ48","s")%>, <% tcWebApi_get("String_Entry","TZ40_1","s")%>, <% tcWebApi_get("String_Entry","TZ47","s")%>, <% tcWebApi_get("String_Entry","TZ46","s")%>, <% tcWebApi_get("String_Entry","TZ44","s")%>, <% tcWebApi_get("String_Entry","TZ45","s")%>"],
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
		if( (i==15 || i==17) && document.form.uiTimezoneSecond.value != 1){
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
	if((model_name == "DSL-N10-C1")||(model_name == "DSL-N10P-C1")||(model_name == "DSL-N12E-C1")||(model_name == "DSL-N12U-C1")||(model_name == "DSL-N14U"))
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

function initial(){
	show_menu();
	autoFocus('<% get_parameter("af"); %>');
	show_http_clientlist();
	remove_btn_ez_radiotoggle();	
	load_timezones();
	corrected_timezone(DAYLIGHT_orig, TZ_orig);
	hideport("<% tcWebApi_get("Firewall_Entry","misc_http_x", "s") %>");
	if("<% TCWebApi_get("SysInfo_Entry","http_restrict_client","s") %>" != "1")	{
		display_spec_IP(0);
	}
	
	if(HTTPS_support == -1 || model_name == "DSL-N10-C1" || model_name == "DSL-N10P-C1" || model_name == "DSL-N12E-C1"){       //MODELDEP: DSL-N10-C1, DSL-N12E-C1
		$("https_tr").style.display = "none";
		$("https_lanport").style.display = "none";
	}
	else{
		hide_https_lanport(document.form.http_enable.value);
		hide_https_wanport(document.form.http_enable.value);
	}
	showLANIPList();
	$("accessfromwan_port").style.display = (document.form.misc_http_x[0].checked == 1) ? "" : "none";

	if(HTTPS_support  == -1 || '<% tcWebApi_get("Https_Entry","http_enable","s") %>' == 1)
		$("https_port").style.display = "none";
	else if(model_name == "DSL-N10-C1" || model_name == "DSL-N10P-C1" || model_name == "DSL-N12E-C1"){       //MODELDEP: DSL-N10-C1, DSL-N12E-C1
		$("https_port").style.display = "none";
	}	
	else if('<% tcWebApi_get("Https_Entry","http_enable","s") %>' == 2)
		$("http_port").style.display = "none";

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
	}
}

function hideport(flag){
$("accessfromwan_port").style.display = (flag == 1) ? "" : "none";
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
		var rule_num = $('http_clientlist_table').rows.length;
		var item_num = $('http_clientlist_table').rows[0].cells.length;
		var tmp_value = "";

		for(i=0; i<rule_num; i++){
			tmp_value += ":"		
			for(j=0; j<item_num-1; j++){	
				tmp_value += $('http_clientlist_table').rows[i].cells[j].innerHTML;
				if(j != item_num-2)	
					tmp_value += ":";
			}
		}
		if(tmp_value == ":"+"<% tcWebApi_Get("String_Entry", "IPC_VSList_Norule", "s") %>" || tmp_value == ":")
			tmp_value = "";	
		document.form.http_clientlist.value = tmp_value;

		if(document.form.http_clientlist.value == "" && document.form.http_client[0].checked == 1){
			alert("<% tcWebApi_Get("String_Entry", "JS_fieldblank", "s") %>");
			document.form.http_client_ip_x_0.focus();
			return false;
		}
		
		document.form.SaveFirewall.value = 1;
		if(document.form.misc_http_x[0].checked == true)
				document.form.misc_http_x.value = "1";
		else
				document.form.misc_http_x.value = "0";

		if(HTTPS_support != -1 && model_name != "DSL-N10-C1"  && model_name != "DSL-N10P-C1" && model_name != "DSL-N12E-C1"){	//MODELDEP : exclude DSL-N10-C1, DSL-N12E-C1
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
        document.form.SaveTelnetd.value = 1;
        <%if tcWebApi_Get("WebCustom_Entry", "isSwapFileSupport", "h") = "Yes" then%>
        if(document.form.swap_enable.value != "<%TCWebApi_get("SysInfo_Entry","swap_enable","s")%>")
                document.form.SaveSwap.value = 1;
        <%end if%>

        if(document.form.uiViewSNTPServer.value == "")
        {
                document.form.uiViewSNTPServer.value = "0.0.0.0";
        }

	if(document.form.uiViewdateToolsTZ[15].selected == true || document.form.uiViewdateToolsTZ[17].selected == true)
		document.form.uiTimezoneSecond.value = "1";
	else
		document.form.uiTimezoneSecond.value = "";

	showLoading(20);	//Extend from 10 to 20 to restore_webtype 
	setTimeout("redirect();", 20000);
	document.form.submit();
	return;
}

function validForm(){

        showtext($("alert_msg1"), "");
        showtext($("alert_msg2"), "");

	//account
        if(document.form.uiViewTools_username.value.length == 0){
                showtext($("alert_msg1"), "<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc1","s")%>");
                document.form.uiViewTools_username.focus();
                document.form.uiViewTools_username.select();
                return false;
        }
        else{
                var alert_str = validate_hostname(document.form.uiViewTools_username);
                if(alert_str != ""){
                        showtext($("alert_msg1"), alert_str);
                        $("alert_msg1").style.display = "";
                        document.form.uiViewTools_username.focus();
                        document.form.uiViewTools_username.select();
                        changeiuiBackground(0);
                        return false;
                }else{
                        $("alert_msg1").style.display = "none";
                }

                document.form.uiViewTools_username.value = trim(document.form.uiViewTools_username.value);

		if(document.form.uiViewTools_username.value == "root"
                                || document.form.uiViewTools_username.value == "guest"
                                || document.form.uiViewTools_username.value == "anonymous"
                ){
                                showtext($("alert_msg1"), "<% tcWebApi_get("String_Entry","USB_App_account_alert","s") %>");
                                $("alert_msg1").style.display = "";
                                document.form.uiViewTools_username.focus();
                                document.form.uiViewTools_username.select();
                                changeiuiBackground(0);
                                return false;
                }
                else if(accounts.getIndexByValue(document.form.uiViewTools_username.value) > 0
                                && document.form.uiViewTools_username.value != accounts[0]){
                                showtext($("alert_msg1"), "<% tcWebApi_get("String_Entry","File_Pop_content_alert_desc5","s") %>");
                                $("alert_msg1").style.display = "";
                                document.form.uiViewTools_username.focus();
                                document.form.uiViewTools_username.select();
                                changeiuiBackground(0);
                                return false;
                }else{
                                $("alert_msg1").style.display = "none";
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

	        if(document.form.uiViewTools_Password.value != document.form.uiViewTools_PasswordConfirm.value){
        	        showtext($("alert_msg2"),"*<% tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s") %>");
	                if(document.form.uiViewTools_Password.value.length <= 0){
                	        document.form.uiViewTools_Password.focus();
        	                document.form.uiViewTools_Password.select();
	                }else{
                	        document.form.uiViewTools_PasswordConfirm.focus();
        	                document.form.uiViewTools_PasswordConfirm.select();
	                }

                	return false;
        	}

	        if(!validate_string(document.form.uiViewTools_Password)){
        	        document.form.uiViewTools_Password.focus();
                	document.form.uiViewTools_Password.select();
	                return false;
        	}
		if(document.form.uiViewTools_Password.value != document.form.prev_web_passwd.value)
		{
			document.form.passwdFlag.value = 1;
		}
	}

	if((document.form.adminFlag.value == 1) || (document.form.passwdFlag.value == 1))
	{
		document.form.accountFlag.value = 1;
	}

	if (document.form.misc_http_x[0].checked) {
		if (!validate_range(document.form.misc_httpport_x, 1024, 65535))
			return false;
		if(HTTPS_support != -1 && model_name != "DSL-N10-C1" && model_name != "DSL-N10P-C1" && model_name != "DSL-N12E-C1"){	//MODELDEP: exclude DSL-N10-C1, DSL-N12E-C1
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
	
	if(HTTPS_support != -1 &&  model_name != "DSL-N10-C1" &&  model_name != "DSL-N10P-C1" && model_name != "DSL-N12E-C1"){     //MODELDEP: exclude DSL-N10-C1, DSL-N12E-C1
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
		if(document.form.misc_httpsport_x.value == document.form.misc_httpport_x.value){
			alert("<%tcWebApi_get("String_Entry","https_port_conflict","s")%>");
			document.form.misc_httpsport_x.focus();
			return false;
		}
	}
	
	if(!validate_range_sp(document.form.http_autologout, 10, 999, set_http_autologout))
		return false;

	return true;
}

/* password item show or not */
function pass_checked(obj){
	switchType(obj, document.form.show_pass_1.checked, true);
}

var theUrl = "router.asus.com";
function hide_https_lanport(_value){
	if(sw_mode == '1' || sw_mode == '2'){
		var https_lanport_num = "<% TCWebApi_get("Https_Entry","https_lanport","s") %>";
		$("https_lanport").style.display = (_value == "1") ? "none" : "";
		document.form.https_lanport.value = "<% TCWebApi_get("Https_Entry","https_lanport","s") %>";
		$("https_access_page").innerHTML = "<%tcWebApi_get("String_Entry","https_access_url","s")%> ";
		$("https_access_page").innerHTML += "<a href=\"https://"+theUrl+":"+https_lanport_num+"\" target=\"_blank\" style=\"color:#FC0;text-decoration: underline; font-family:Lucida Console;\">http<span>s</span>://"+theUrl+"<span>:"+https_lanport_num+"</span></a>";
		$("https_access_page").style.display = (_value == "1") ? "none" : "";
	}
	else{
		$("https_access_page").style.display = 'none';
	}
}

function hide_https_wanport(_value){
	$("http_port").style.display = (_value == "2") ? "none" : "";
	$("https_port").style.display = (_value == "1") ? "none" : "";
}

// show clientlist
function show_http_clientlist(){
	var http_clientlist_row = http_clientlist_array.split(":");
	var code = "";
	code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="list_table" id="http_clientlist_table">'; 
	if(http_clientlist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;"><% tcWebApi_Get("String_Entry", "IPC_VSList_Norule", "s") %></td>';
	else{
		for(var i =1; i < http_clientlist_row.length; i++){
		code +='<tr id="row'+i+'">';
		code +='<td width="80%">'+ http_clientlist_row[i] +'</td>';		//Url keyword
		code +='<td width="20%">';
		code +="<input class=\"remove_btn\" type=\"button\" onclick=\"deleteRow(this);\" value=\"\"/></td>";
		}
	}
  	code +='</tr></table>';
	
	$("http_clientlist_Block").innerHTML = code;
}

function setClientIP(ipaddr){
	document.form.http_client_ip_x_0.value = ipaddr;
	hideClients_Block();
	over_var = 0;
}

var over_var = 0;
var isMenuopen = 0;

function hideClients_Block(){
	$("pull_arrow").src = "/images/arrow-down.gif";
	$('ClientList_Block_PC').style.display='none';
	isMenuopen = 0;
}

function pullLANIPList(obj){
	if(isMenuopen == 0){		
		obj.src = "/images/arrow-top.gif"
		$("ClientList_Block_PC").style.display = 'block';		
		document.form.http_client_ip_x_0.focus();		
		isMenuopen = 1;
	}
	else
		hideClients_Block();
}

function deleteRow(r){
	var i=r.parentNode.parentNode.rowIndex;
	$('http_clientlist_table').deleteRow(i);
  
	var http_clientlist_value = "";
	for(i=0; i<$('http_clientlist_table').rows.length; i++){
		http_clientlist_value += ":";
		http_clientlist_value += $('http_clientlist_table').rows[i].cells[0].innerHTML;
	}
	
	http_clientlist_array = http_clientlist_value;
	if(http_clientlist_array == "")
		show_http_clientlist();
}

function addRow(obj, upper){
	if('<% TCWebApi_get("SysInfo_Entry","http_restrict_client","h") %>' != "1")
		document.form.http_client[0].checked = true;
		
	//Viz check max-limit 
	var rule_num = $('http_clientlist_table').rows.length;
	var item_num = $('http_clientlist_table').rows[0].cells.length;		
	if(rule_num >= upper){
		alert("<% tcWebApi_Get("String_Entry", "JS_itemlimit1", "s") %> " + upper + " <% tcWebApi_Get("String_Entry", "JS_itemlimit2", "s") %>");
		return false;	
	}
			
	if(obj.value == ""){
		alert(" <% tcWebApi_Get("String_Entry", "JS_fieldblank", "s") %>");
		obj.focus();
		obj.select();			
		return false;
	}
	else if(valid_IP_form(obj, 0) != true){
		return false;
	}
	else{		
		//Viz check same rule
		for(i=0; i<rule_num; i++){
			for(j=0; j<item_num-1; j++){		//only 1 value column
				if(obj.value == $('http_clientlist_table').rows[i].cells[j].innerHTML){
					alert(" <% tcWebApi_Get("String_Entry", "JS_duplicate", "s") %>");
					return false;
				}	
			}
		}
		
		http_clientlist_array += ":";
		http_clientlist_array += obj.value;
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
		$("https_access_page").innerHTML = "<%tcWebApi_get("String_Entry","https_access_url","s")%> ";
		$("https_access_page").innerHTML += "<a href=\"https://"+theUrl+":"+https_lanport_num_new+"\" target=\"_blank\" style=\"color:#FC0;text-decoration: underline; font-family:Lucida Console;\">http<span>s</span>://"+theUrl+"<span>:"+https_lanport_num_new+"</span></a>";
	}else{

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

	$("ClientList_Block_PC").innerHTML = htmlCode;
}

function clean_scorebar(obj){
	if(obj.value == "")
		document.getElementById("scorebarBorder").style.display = "none";
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_System_Content.asp" name="form" target="hidden_frame">
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
<INPUT TYPE="HIDDEN" name="SaveTelnetd" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveFirewall" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveHttps" VALUE="0">
<INPUT TYPE="HIDDEN" name="SaveSwap" VALUE="0">
<INPUT TYPE="HIDDEN" name="tcWebApi_Save_Flag" VALUE="1">
<INPUT TYPE="HIDDEN" name="prev_http_enable" VALUE="<%tcWebApi_get("Https_Entry","http_enable","s")%>">
<INPUT TYPE="HIDDEN" name="prev_https_lanport" VALUE="<%tcWebApi_get("Https_Entry","https_lanport","s")%>">
<INPUT TYPE="HIDDEN" name="prev_username" VALUE="<%tcWebApi_get("Account_Entry0","username","s")%>">
<INPUT TYPE="HIDDEN" name="prev_web_passwd" VALUE="<%tcWebApi_get("Account_Entry0","web_passwd","s")%>">
<INPUT TYPE="HIDDEN" name="http_clientlist" value="<% TCWebApi_get("SysInfo_Entry","http_clientlist","s") %>">

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
						<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6_adv","s")%> - <%tcWebApi_get("String_Entry","menu5_6_2","s")%></div>
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
									<input type="text" name="uiViewTools_username" maxlength="20" value="<% tcWebApi_Get("Account_Entry0","username","s") %>" onKeyPress="return is_string(this, event);"" class="input_15_table" autocapitalization="off" autocomplete="off">
									<br/><span id="alert_msg1" style="color:#FFCC00;"></span>
								</td>
							</tr>
							<tr>
								<th width="40%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,4)"><%tcWebApi_get("String_Entry","PASS_new","s")%></a></th>
								<td>
									<input type="password" autocapitalization="off" autocomplete="off" name="uiViewTools_Password" maxlength="16" value="" onKeyPress="return is_string(this, event);" onkeyup="chkPass(this.value, 'http_passwd');" onpaste="return false;" class="input_15_table" onBlur="clean_scorebar(this);">
										&nbsp;&nbsp;
									<div id="scorebarBorder" name="scorebarBorder" style="margin-left:140px; margin-top:-25px; display:none;" title="<%tcWebApi_get("String_Entry","LHC_x_Password_itemSecur","s")%>">
										<div id="score" name="score"></div>
										<div id="scorebar" name="scorebar">&nbsp;</div>
									</div>									
								</td>
							</tr>
							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,4)"><%tcWebApi_get("String_Entry","PASS_retype","s")%></a></th>
								<td>
									<INPUT TYPE="PASSWORD" autocapitalization="off" autocomplete="off" NAME="uiViewTools_PasswordConfirm" MAXLENGTH="16" VALUE="" onKeyPress="return is_string(this, event);" onpaste="return false;" class="input_15_table">
									<div style="margin:-25px 0px 5px 135px;"><input type="checkbox" name="show_pass_1" onclick="pass_checked(document.form.uiViewTools_Password);pass_checked(document.form.uiViewTools_PasswordConfirm);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
									<br/><span id="alert_msg2" style="color:#FC0;margin-left:8px;"></span>
								</td>
							</tr>
						</table>
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:8px;">
							<thead>
							<tr>
								<td colspan="2"><%tcWebApi_get("String_Entry","t2Misc","s")%></td>
							</tr>
							</thead>
							<tr id="btn_ez_radiotoggle_tr">
								<th><%tcWebApi_get("String_Entry","WPS_btn_behavior","s")%></th>
								<td>
									<input type="radio" id="radio2" name="btn_ez_radiotoggle" class="input" value="2" <% if tcWebApi_get("SysInfo_Entry","btn_ez_radiotoggle","h") = "2" then asp_Write("checked") end if %>><label id="radio_text2">Turn LED On/Off</label>
									<input type="radio" id="radio1" name="btn_ez_radiotoggle" class="input" value="1" <% if tcWebApi_get("SysInfo_Entry","btn_ez_radiotoggle","h") = "1" then asp_Write("checked") end if %>><label id="radio_text1"><%tcWebApi_get("String_Entry","WPS_btn_toggle","s")%></label>
									<input type="radio" id="radio0" name="btn_ez_radiotoggle" class="input" value="0" <% if tcWebApi_get("SysInfo_Entry","btn_ez_radiotoggle","h") = "0" then asp_Write("checked") end if %>><label id="radio_text0"><%tcWebApi_get("String_Entry","WPS_btn_actWPS","s")%></label>
								</td>
							</tr>

							<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,1)"><%tcWebApi_get("String_Entry","LHC_x_ServerLogEnable_in","s")%></a></th>
								<td>
									<INPUT TYPE="TEXT" class="input_15_table" NAME="syslogServerAddr" SIZE="25" MAXLENGTH="15" VALUE="<%if tcWebApi_get("Syslog_Entry","remoteHost","h") <> "N/A" then tcWebApi_get("Syslog_Entry","remoteHost","s") else asp_Write("") end if%>" onKeyPress="return is_ipaddr(this, event)">
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
									<INPUT TYPE="TEXT" NAME="uiViewSNTPServer" SIZE="24" MAXLENGTH="48" VALUE="<%if tcWebApi_get("Timezone_Entry","SERVER","h") <> "0.0.0.0" then tcWebApi_get("Timezone_Entry","SERVER","s") else asp_Write("") end if%>" class="input_32_table" onKeyPress="return is_string(this, event);">
									<a href="javascript:openLink('x_NTPServer1')" name="x_NTPServer1_link" style=" margin-left:5px; text-decoration: underline;"><%tcWebApi_get("String_Entry","LHC_x_NTPServer1_linkname","s")%></a>
								</td>
							</tr>
							<tr id="telnet_tr">
								<th><%tcWebApi_get("String_Entry","Enable_Telnet","s")%></th>
								<td>
									<input type="radio" name="telnetd_enable" class="input" value="1" <% If TCWebApi_get("Misc_Entry","telnetd_enable","h") = "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" name="telnetd_enable" class="input" value="0" <% If TCWebApi_get("Misc_Entry","telnetd_enable","h") = "0" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr>

							<tr id="https_tr">
								<th><% tcWebApi_Get("String_Entry", "WC11b_AuthenticationMethod_in", "s") %></th>
								<td>
									<select name="http_enable" class="input_option" onchange="hide_https_lanport(this.value);hide_https_wanport(this.value);">
											<option value="1" <% if tcWebApi_get("Https_Entry","http_enable","h") = "1" then asp_Write("selected") end if %>>HTTP</option>
											<option value="2" <% if tcWebApi_get("Https_Entry","http_enable","h") = "2" then asp_Write("selected") end if %>>HTTPS</option>
											<option value="3" <% if tcWebApi_get("Https_Entry","http_enable","h") = "3" then asp_Write("selected") end if %>>BOTH</option>
									</select>
								</td>
							</tr>

							<tr id="https_lanport">
								<th>HTTPS Lan port</th>
								<td>
									<input type="text" maxlength="5" class="input_6_table" name="https_lanport" value="<%TCWebApi_get("Https_Entry","https_lanport","s")%>" onKeyPress="return is_number(this,event);" onBlur="change_url(this.value, 'https_lan');">
									<span id="https_access_page"></span>
								</td>
							</tr>

							<tr id="misc_http_x_tr">
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,2);"><% tcWebApi_Get("String_Entry", "FC_x_WanWebEnable_in", "s") %></a></th>
								<td>
									<input type="radio" value="1" name="misc_http_x" class="input" onClick="hideport(1);return change_common_radio(this, 'FirewallConfig', 'misc_http_x', '1')" <% if tcWebApi_get("Firewall_Entry","misc_http_x","h") = "1" then asp_Write("checked") end if %>><% tcWebApi_Get("String_Entry", "checkbox_Yes", "s") %>
									<input type="radio" value="0" name="misc_http_x" class="input" onClick="hideport(0);return change_common_radio(this, 'FirewallConfig', 'misc_http_x', '0')" <% if tcWebApi_get("Firewall_Entry","misc_http_x","h") = "0" then asp_Write("checked") end if %>><% tcWebApi_Get("String_Entry", "checkbox_No", "s") %>
								</td>
							</tr>

							<tr id="accessfromwan_port">
								<th align="right"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,3);"><% tcWebApi_Get("String_Entry", "FC_x_WanWebPort_in", "s") %></a></th>
								<td>
									<span style="margin-left:5px;" id="http_port">HTTP: <input type="text" maxlength="5" name="misc_httpport_x" class="input_6_table" value="<% tcWebApi_get("Firewall_Entry","misc_httpport_x", "s") %>" onKeyPress="return is_number(this,event);"/>&nbsp;&nbsp;</span>
									<span style="margin-left:5px;" id="https_port">HTTPS: <input type="text" maxlength="5" name="misc_httpsport_x" class="input_6_table" value="<% tcWebApi_get("Firewall_Entry","misc_httpsport_x", "s") %>" onKeyPress="return is_number(this,event);"/></span>
								</td>
							</tr>

							<tr>
								<th><% tcWebApi_Get("String_Entry", "System_AutoLogout", "s") %></th>
								<td>
									<input type="text" class="input_3_table" maxlength="3" name="http_autologout" value="<% tcWebApi_Get("Misc_Entry", "http_autologout", "s") %>" onKeyPress="return is_number(this,event);">
									min<span> ( <% tcWebApi_Get("String_Entry", "zero_disable", "s") %> )</span>
								</td>
							</tr>

							<tr>
								<th align="right"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(8,9);">Enable WAN down browser redirect notice</a></th>
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
							<tr id="http_client_tr">
								<th><% tcWebApi_Get("String_Entry", "System_login_specified_IP", "s") %></th>
								<td>
									<input type="radio" name="http_client" class="input" value="1" onclick="display_spec_IP(1);" <% If TCWebApi_get("SysInfo_Entry","http_restrict_client","h") = "1" then asp_Write("checked") end if%>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" name="http_client" class="input" value="0" onclick="display_spec_IP(0);" <% If TCWebApi_get("SysInfo_Entry","http_restrict_client","h") <> "1" then asp_Write("checked") end if%>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
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
								<th width="80%"><% tcWebApi_Get("String_Entry", "ConnectedClient", "s") %></th>
								<th width="20%"><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
							</tr>

							<tr>
									<!-- client info -->
								<td width="80%">
									<input type="text" class="input_32_table" maxlength="15" name="http_client_ip_x_0"  onKeyPress="" onClick="hideClients_Block();" onblur="if(!over_var){hideClients_Block();}">
									<img id="pull_arrow" height="14px;" src="/images/arrow-down.gif" style="position:absolute;*margin-left:-3px;*margin-top:1px;" onclick="pullLANIPList(this);" title=<% tcWebApi_Get("String_Entry", "select_client", "s") %> onmouseover="over_var=1;" onmouseout="over_var=0;">	
									<div id="ClientList_Block_PC" class="ClientList_Block_PC"></div>	
								 </td>
								 <td width="20%">	
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

