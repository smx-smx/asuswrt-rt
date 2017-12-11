<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--aidisk/Aidisk-4.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>AiDisk Wizard</title>
<link rel="stylesheet" type="text/css" href="/aidisk/aidisk.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/state.js"></script>
<script>
var ddns_enable_x = parent.getASUSDDNS_enable();
var ddns_server_x = '<%tcWebApi_get("Ddns_Entry","SERVERNAME","s")%>';
var ddns_hostname_x = '<%tcWebApi_get("Ddns_Entry","MYHOST","s")%>';
var ddns_hostname_title;
var SAFE_TIME = 30;
var FOLDER_WORK_TIME = 2;
<% wanlink(); %>

function initial(){
	parent.show_help_iframe(4);
	parent.hideLoading();
	show_dummyshareway();
	showDDNS();
	clickevent();
	parent.restore_help_td();
	$("finish").focus();
}
function show_dummyshareway(){
	switch(parent.$("dummyShareway").value){
		case "0":
			showtext($("dummyShareStr"), "\"<%tcWebApi_get("String_Entry","Step2_method1","s")%>\"");
			break;
		case "1":
			showtext($("dummyShareStr"), "\"<%tcWebApi_get("String_Entry","Step2_method2","s")%>\"");
			break;
		case "2":
			showtext($("dummyShareStr"), "\"<%tcWebApi_get("String_Entry","Step2_method3","s")%>\"");
			break;
		default:
			alert("System error!");
	}
}
function valid_wan_ip() {
	var A_class_start = inet_network("10.0.0.0");
	var A_class_end = inet_network("10.255.255.255");
	var B_class_start = inet_network("172.16.0.0");
	var B_class_end = inet_network("172.31.255.255");
	var C_class_start = inet_network("192.168.0.0");
	var C_class_end = inet_network("192.168.255.255");
	var ip_obj = wanlink_ipaddr();
	var ip_num = inet_network(ip_obj);
	var ip_class = "";
	if(ip_num > A_class_start && ip_num < A_class_end)
		ip_class = 'A';
	else if(ip_num > B_class_start && ip_num < B_class_end)
		ip_class = 'B';
	else if(ip_num > C_class_start && ip_num < C_class_end)
		ip_class = 'C';
	else if(ip_num != 0){
		$("priv_wan_ip").style.display = "none";
		return;
	}
	$("priv_wan_ip").style.display = "";
	return;
}
function showDDNS(){
	$("priv_wan_ip").style.display = "none";
	if('<% tcWebApi_Get("Sambe_Entry", "enable_ftp", "s") %>' == '1'){
		if(this.ddns_enable_x == "1"){
			$("haveDDNS").style.display = "";
			$("noFTP").style.display = "none";
			$("noDDNS").style.display = "none";
			valid_wan_ip();
		}
		else{
			$("haveDDNS").style.display = "none";
			$("noFTP").style.display = "none";
			$("noDDNS").style.display = "";
		}
	}
	else{
		$("haveDDNS").style.display = "none";
		$("noFTP").style.display = "none";
		$("noDDNS").style.display = "none";
	}
}
function go_pre_page(){
	document.redirectForm.action = "Aidisk-3.asp";
	document.redirectForm.submit();
}
function compute_work_time(){
	var total_folder_number = 0;
	for(var i = 0; i < parent.pool_devices().length; ++i){
		if(parent.pool_devices()[i].indexOf("part") < 0)
			continue;
		total_folder_number += parent.get_sharedfolder_in_pool(parent.pool_devices()[i]).length-1;
	}
	if(parent.$("dummyShareway").value == "1")
		return FOLDER_WORK_TIME*total_folder_number*2+SAFE_TIME;
	else if(parent.$("dummyShareway").value == "2")
		return FOLDER_WORK_TIME*total_folder_number+SAFE_TIME;
	else
		return SAFE_TIME;
}
function clickevent(){
	$("finish").onclick = function(){
		parent.showLoading();
		parent.document.parameterForm.next_page.value = "aidisk.asp";
		if(parent.$("dummyShareway").value == "0")
			parent.switchShareMode("ftp", "share");
		else
			parent.initialAccount();
	};
}
</script>
</head>
<body onload="initial();">
<form method="post" name="redirectForm" action="#"></form>
<div class="aidisk1_table">
<table> <!-- width="765" height="760" border="0" bgcolor="#4d595d" cellpadding="0" cellspacing="0" style="padding:10px; padding-top:20px;" //Viz-->
<tr>
<td height="780" align="center" valign="top" bgcolor="#4d595d" style="padding-top:25px;">
<table width="740" height="225" border="0">
	<tr>
		<td>
		<table width="740" border="0">
		<tr>
			<td width="15%" height="90px" style="background:url(/images/New_ui/aidisk/step1.png) 0% 0% no-repeat;"></td>
			<td width="15%"><img src="/images/New_ui/aidisk/steparrow.png" /></td>
			<td width="15%" height="90px" style="background:url(/images/New_ui/aidisk/step2.png) 0% 0% no-repeat;"></td>
			<td width="15%"><img src="/images/New_ui/aidisk/steparrow.png" /></td>
			<td width="15%" height="90px" style="background:url(/images/New_ui/aidisk/step3.png) 0% 95% no-repeat;"></td>
			<td width="25%">&nbsp;</td>
		</tr>
		</table>
		</td>
	</tr>
	<tr>
    	<td align="left" class="formfonttitle" style="padding-left:20px;" height="72"><%tcWebApi_get("String_Entry","Step3_desp","s")%></td>
	</tr>
	<tr>
		<td colspan="3" valign="top" align="left" class="">
		<div style="width:660px; line-height:180%;">
		<ul>
     	 <li><%tcWebApi_get("String_Entry","yoursharesetting","s")%> 
				<span id="dummyShareStr"></span>
			</li>
			<span id="haveDDNS">
				<li>Internet FTP address: <a target="_blank" style="text-decoration: underline; font-family:Lucida Console;">ftp://<%tcWebApi_get("Ddns_Entry","MYHOST","s")%></a></li>
			</span>
			<span id="priv_wan_ip" style="color:#FFCC00;">
       <li><%tcWebApi_get("String_Entry","LHC_x_DDNSEnable_sd2","s")%></li>
			</span>
			<span id="noFTP" class="ServerClose">
       <li><%tcWebApi_get("String_Entry","linktoFTP_no_1","s")%></li>
			</span>
			<span id="noDDNS" class="ServerClose">
       <li><%tcWebApi_get("String_Entry","linktoFTP_no_2","s")%></li>
			</span>
		</ul>
		</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-top:30px;"><img src="/images/New_ui/export/line_export.png" /></div>
		</td>
	</tr>
	<tr valign="bottom">
		<td width="20%">
	    	<a href="Aidisk-3.asp"><div class="titlebtn" align="center" style="margin-left:275px;_margin-left:137px;width:80px;"><span><%tcWebApi_get("String_Entry","btn_pre","s")%></span></div></a>
	    	<a href="javascript:;"><div id="finish" class="titlebtn" align="center" style="width:80px;"><span><%tcWebApi_get("String_Entry","CTL_finish","s")%></span></div></a>
		</td>
	</tr>
</table>
</td>
</tr>
</table>
</div> </body>

<!--aidisk/Aidisk-4.asp-->
</html>

