<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--aidisk/Aidisk-1.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>AiDisk Wizard</title>
<link rel="stylesheet" type="text/css" href="/NM_style.css">
<link rel="stylesheet" type="text/css" href="/aidisk/aidisk.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script>
var dummyShareway = '<% tcWebApi_get("Samba_Entry", "dummyShareway", "s") %>';
var FTP_status = parent.get_ftp_status(); // FTP 0=disable 1=enable
var FTP_mode = parent.get_share_management_status("ftp"); // if share by account. 1=no 2=yes
var accounts = [<% get_all_accounts(); %>];
var ddns_enable = '<% tcWebApi_get("Ddns_Entry","Active","s") %>';
var ddns_server = '<%tcWebApi_get("Ddns_Entry","SERVERNAME","s")%>';
var ddns_hostname = '<%tcWebApi_get("Ddns_Entry","MYHOST","s")%>';
var format_of_first_partition = parent.pool_types()[0]; //"ntfs";
function initial(){
	parent.hideLoading();
	showdisklink();
}
function showdisklink(){
	if(detect_mount_status() == 0){ // No USB disk plug.
		$("AiDiskWelcome_desp").style.display = 'none';
		$("linkdiskbox").style.display = 'none';
		$("Nodisk_hint").style.display = 'block';
		$("gotonext").style.display = 'none';
		return;
	}
	else if(dummyShareway != ""){ // Ever config aidisk wizard
		$("AiDiskWelcome_desp").style.display = 'none';
		$("linkdiskbox").style.display = 'block';
		$("settingBtn").innerHTML = "<%tcWebApi_get("String_Entry","CTL_Reset_OOB","s")%>";

		show_share_link();
	}
	else{ // Never config aidisk wizard
		$("linkdiskbox").style.display = 'none';
	}
}
function show_share_link(){
/*if(NN_status == 1 && nav == false)
$("ie_link").style.display = "block";
if(FTP_status == 1 && FTP_mode == 1)
$("notie_link").style.display = "block";
else{
$("noLAN_link").style.display = "block";
if(NN_status != 1)
			showtext($("noLAN_link"), "<%tcWebApi_get("String_Entry","linktodisk_no_1","s")%>");
else
			showtext($("noLAN_link"), "<%tcWebApi_get("String_Entry","linktodisk_no_2","s")%>");
}*/
	if(FTP_status == 1 && ddns_enable == 1 && ddns_server.length > 0 && ddns_hostname.length > 0){
		if(FTP_mode == 1 || dummyShareway == 0){
			$("ddnslink1").style.display = "";
			$("desc_2").style.display = "";
			$("ddnslink1_LAN").style.display = "";
		}
		else if(FTP_mode == 2){
			$("ddnslink2").style.display = "";
			$("desc_2").style.display = "";
			$("ddnslink2_LAN").style.display = "";
		}
	}
	else{
		$("noWAN_link").style.display = "";
		if(FTP_status != 1)
			showtext($("noWAN_link"), '<%tcWebApi_get("String_Entry","linktoFTP_no_1","s")%>');
		else if(ddns_enable != 1){ //Viz 2011.10{
			showtext($("noWAN_link"), "<%tcWebApi_get("String_Entry","linktoFTP_no_2","s")%>");
			$("desc_2").style.display = "";
			$("ddnslink1_LAN").style.display = ""; //Viz 2011.10}
		}else if(ddns_hostname.length <= 0)
			showtext($("noWAN_link"), "<%tcWebApi_get("String_Entry","linktoFTP_no_3","s")%>");
		else
			alert("FTP and ddns exception");
	}
}
function detect_mount_status(){
	var mount_num = 0;
	for(var i = 0; i < parent.foreign_disk_total_mounted_number().length; ++i)
		mount_num += parent.foreign_disk_total_mounted_number()[i];
	return mount_num;
}
function go_next_page(){
	document.redirectForm.action = "Aidisk-2.asp";
	document.redirectForm.submit();
}
</script>
</head>
<body onload="initial();">
<form method="GET" name="redirectForm" action="#">
<input type="hidden" name="flag" value="">
</form>
<div class="aidisk1_table">
<table>
<tr>
<td valign="top" bgcolor="#4d595d" style="padding-top:25px;">
<table width="740" height="125" border="0">
	<tr>
		<td class="formfonttitle">
			<span style="margin-left:3px;"><%tcWebApi_get("String_Entry","AiDiskWelcome_title","s")%></span>
			<img onclick="go_setting_parent('/cgi-bin/APP_Installation.asp')" align="right" style="cursor:pointer;margin-right:20px;margin-top:-20px;" title="Go to APP Gallery" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'">
		</td>
	</tr>
	<tr>
		<td height="20">
			<img src="/images/New_ui/export/line_export.png" />
		</td>
	</tr>
	<tr>
		<td>
			<div style="width:660px; line-height:180%;">
	  		<div id="Nodisk_hint" class="alert_string" style="display:none;"><%tcWebApi_get("String_Entry","no_usb_found","s")%></div>

			<div id="AiDiskWelcome_desp">
			  	<%tcWebApi_get("String_Entry","AiDiskWelcome_desp","s")%>
				<ul>
			  		<li><%tcWebApi_get("String_Entry","AiDiskWelcome_desp1","s")%></li>
			  		<li><%tcWebApi_get("String_Entry","AiDiskWelcome_desp2","s")%></li>
					<li><%tcWebApi_get("String_Entry","AiDisk_moreconfig","s")%></li>
					<li>
							<a id="tosLink" style="cursor:pointer;font-family:Lucida Console;text-decoration:underline;" target="_blank" href="">
								<%tcWebApi_get("String_Entry","DDNS_termofservice_Title","s")%>
							</a>
							<script>
								(function(){
									var preferLang = "<%tcWebApi_get("LanguageSwitch_Entry","Type","s")%>";
									
									if(preferLang == "1")
										document.getElementById("tosLink").href = "http://www.asus.com/us";
									else if(preferLang == "2")
										document.getElementById("tosLink").href = "http://www.asus.com/br";
									else if(preferLang == "3")
										document.getElementById("tosLink").href = "http://www.asus.com.cn";
									else if(preferLang == "4")
										document.getElementById("tosLink").href = "http://www.asus.com/cz";
									else if(preferLang == "5")
										document.getElementById("tosLink").href = "http://www.asus.com/dk";
									else if(preferLang == "6")
										document.getElementById("tosLink").href = "http://www.asus.com/de";
									else if(preferLang == "7")
										document.getElementById("tosLink").href = "http://www.asus.com/es";
									else if(preferLang == "8")
										document.getElementById("tosLink").href = "http://www.asus.com/fi";
									else if(preferLang == "9")
										document.getElementById("tosLink").href = "http://www.asus.com/fr";
									else if(preferLang == "10")
										document.getElementById("tosLink").href = "http://www.asus.com/it";
									else if(preferLang == "11")
										document.getElementById("tosLink").href = "http://www.asus.com/my";
									else if(preferLang == "12")
										document.getElementById("tosLink").href = "http://www.asus.com/no";
									else if(preferLang == "13")
										document.getElementById("tosLink").href = "http://www.asus.com/pl";
									else if(preferLang == "14")
										document.getElementById("tosLink").href = "http://www.asus.com/ru";
									else if(preferLang == "15")
										document.getElementById("tosLink").href = "http://www.asus.com/se";
									else if(preferLang == "16")
										document.getElementById("tosLink").href = "http://www.asus.com/th";	
									else if(preferLang == "17")
										document.getElementById("tosLink").href = "http://www.asus.com/tr";
									else if(preferLang == "18")
										document.getElementById("tosLink").href = "http://www.asus.com/tw";
									else if(preferLang == "19")
										document.getElementById("tosLink").href = "http://www.asus.com/ua";
									else if(
									 preferLang == "ro" || preferLang == "hu"  || preferLang == "jp"
									)
										document.getElementById("tosLink").href = "http://www.asus.com/" + preferLang;
									else
										document.getElementById("tosLink").href = "http://www.asus.com/us";

									document.getElementById("tosLink").href += "/Terms_of_Use_Notice_Privacy_Policy/Official-Site";
								})()
							</script>
						</li>						
				</ul>
			</div>
			<div id="linkdiskbox" >
		  			<span style="margin-left:5px;"><%tcWebApi_get("String_Entry","AiDisk_wizard_text_box_title3","s")%></span><br/>
				<ul>
					<li>
						<span id="noWAN_link" style="display:none;"></span>
						<span id="ddnslink1" style="display:none;">
							Internet FTP address: <a target="_blank" style="text-decoration: underline; font-family:Lucida Console;">ftp://<%tcWebApi_get("Ddns_Entry","MYHOST","s")%></a>
						</span>
						<span id="ddnslink2" style="display:none;">
							Internet FTP address: <a target="_blank" style="text-decoration: underline; font-family:Lucida Console;">ftp://<%tcWebApi_get("Ddns_Entry","MYHOST","s")%></a>
						</span>
					</li>
					<li id="desc_2" style="display:none;margin-top:8px;">
						<span id="ddnslink1_LAN" style="display:none;">
							LAN FTP address: <a target="_blank" style="text-decoration: underline; font-family:Lucida Console;">ftp://<% tcWebApi_Get("Lan_Entry0", "IP", "s") %></a>
						</span>
						<span id="ddnslink2_LAN" style="display:none;">
							LAN FTP address: <a target="_blank" style="text-decoration: underline; font-family:Lucida Console;">ftp://<% tcWebApi_Get("Lan_Entry0", "IP", "s") %></a>
						</span>
					</li>
					<li><%tcWebApi_get("String_Entry","AiDisk_moreconfig","s")%></li>
					<li><%tcWebApi_get("String_Entry","Aidisk_authority_hint","s")%></li>
					<li>
							<a id="tosLink2" style="cursor:pointer;font-family:Lucida Console;text-decoration:underline;" target="_blank" href="">
								<%tcWebApi_get("String_Entry","DDNS_termofservice_Title","s")%>
							</a>
							<script>
								(function(){
									var preferLang = "<%tcWebApi_get("LanguageSwitch_Entry","Type","s")%>";
									
									if(preferLang == "1")
										document.getElementById("tosLink2").href = "http://www.asus.com/us";
									else if(preferLang == "2")
										document.getElementById("tosLink2").href = "http://www.asus.com/br";
									else if(preferLang == "3")
										document.getElementById("tosLink2").href = "http://www.asus.com.cn";
									else if(preferLang == "4")
										document.getElementById("tosLink2").href = "http://www.asus.com/cz";
									else if(preferLang == "5")
										document.getElementById("tosLink2").href = "http://www.asus.com/dk";
									else if(preferLang == "6")
										document.getElementById("tosLink2").href = "http://www.asus.com/de";
									else if(preferLang == "7")	
										document.getElementById("tosLink2").href = "http://www.asus.com/es";
									else if(preferLang == "8")
										document.getElementById("tosLink2").href = "http://www.asus.com/fi";
									else if(preferLang == "9")
										document.getElementById("tosLink2").href = "http://www.asus.com/fr";
									else if(preferLang == "10")
										document.getElementById("tosLink2").href = "http://www.asus.com/it";
									else if(preferLang == "11")
										document.getElementById("tosLink2").href = "http://www.asus.com/my";
									else if(preferLang == "12")
										document.getElementById("tosLink2").href = "http://www.asus.com/no";
									else if(preferLang == "13")
										document.getElementById("tosLink2").href = "http://www.asus.com/pl";
									else if(preferLang == "14")
										document.getElementById("tosLink2").href = "http://www.asus.com/ru";
									else if(preferLang == "15")
										document.getElementById("tosLink2").href = "http://www.asus.com/se";
									else if(preferLang == "16")
										document.getElementById("tosLink2").href = "http://www.asus.com/th";	
									else if(preferLang == "17")
										document.getElementById("tosLink2").href = "http://www.asus.com/tr";
									else if(preferLang == "18")
										document.getElementById("tosLink2").href = "http://www.asus.com/tw";
									else if(preferLang == "19")
										document.getElementById("tosLink2").href = "http://www.asus.com/ua";
									else if(
									 preferLang == "ro" || preferLang == "hu"  || preferLang == "jp"
									)
										document.getElementById("tosLink2").href = "http://www.asus.com/" + preferLang;
									else
										document.getElementById("tosLink2").href = "http://www.asus.com/us";

									document.getElementById("tosLink2").href += "/Terms_of_Use_Notice_Privacy_Policy/Official-Site";
								})()
							</script>
						</li>					
				</ul>
			</div>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center" width="740px" height="60px">
			<div id="gotonext">
			<a href="javascript:go_next_page();"><div class="titlebtn" style="margin-left:300px;_margin-left:150px;" align="center"><span id="settingBtn" style="*width:190px;"><%tcWebApi_get("String_Entry","btn_go","s")%></span></div></a>
			</div>
		</td>
	</tr>
</table>
</td>
</tr>
</table>
</div> </body>

<!--aidisk/Aidisk-1.asp-->
</html>

