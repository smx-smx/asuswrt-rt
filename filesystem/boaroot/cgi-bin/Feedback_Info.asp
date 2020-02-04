<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Feedback_Info.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu_dsl_feedback","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
var fb_response = "<% tcWebApi_staticGet("PushMail_Entry","fb_state","s") %>";
var diag_log_exist = "<% tcWebApi_staticGet("DslDiag_Entry","dslx_diag_log_link","s") %>";
var WANConnCurState = "<% tcWebApi_staticGet("GUITemp_Entry0","WANConnCurState","s") %>";
	
function initial(){	
	show_menu();	
	check_info();	
}

function check_info(){
	//0:initial  1:Success  2.Failed  3.Limit?  4.dla
	if(dsl_diag_support >= 0 && wan_diag_state == "4"){
		document.getElementById("fb_send_debug_log").style.display = "";		
		document.getElementById("Email_subject").href = "mailto:broadband_feedback@asus.com?Subject="+productid;
		get_debug_log_info();
		gen_dl_diag_log();
	}
	else
		document.getElementById("fb_success").style.display = "";
	/*
	if(fb_response == "1"){
		document.getElementById("fb_success").style.display = "";
		
	}else{	// 0:default 2:Fail 3:limit reached
		if(WANConnCurState == 1){	//WAN connected state
			document.getElementById("fb_fail_WAN_connected").style.display = "";
		}
		else{	//WAN disconnected state
			document.getElementById("fb_fail_WAN_disconnected").style.display = "";
		}	
	}
	*/
}

function gen_dl_diag_log(){
	if(diag_log_exist == "" || typeof(diag_log_exist) == "undefined")
		return;
	var diag_log_exist_array = diag_log_exist.split('');	//string to characte array
	for(var i=0;i<diag_log_exist_array.length;i++){
		if(diag_log_exist_array[i] == 1){
			document.getElementById("dl_diag_log_"+i).style.display = "";
			if(i > 0)
				document.getElementById("dl_diag_log_multiple_desc").style.display = "";
		}
	}
}

function get_debug_log_info(){

	var desc = "DSL / Wi-Fi DIAGNOSTIC LOG\n";
	desc += "----------------------------------------------------------------------\n";

	desc += "Model: "+productid+"\n";
	desc += "Firmware Version: <% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>\n";
	desc += "Inner Version: <% tcWebApi_staticGet("SysInfo_Entry","InnerVersion","s") %>\n";	
	desc += "DSL Driver Version:  <% tcWebApi_staticGet("Info_Adsl","fwVer","s") %>\n\n";

	desc += "PIN Code: <% tcWebApi_get("WLan_Entry0", "WscVendorPinCode", "s") %>\n";
	desc += "MAC Address: <% tcWebApi_Get("Info_Ether","mac","s") %>\n\n";

	desc += "Diagnostic debug log capture duration: <% tcWebApi_staticGet("DslDiag_Entry","dslx_diag_duration","s") %> sec.\n";
	desc += "DSL connection: <% tcWebApi_staticGet("GUITemp_Entry0","fb_tmp_availability","s") %>\n";

	document.uiForm.fb_send_debug_log_content.value = desc;
	
}

function applyRule(){
	if(document.diagform.dslx_diag_state.value == 4){
		document.diagform.DslDiagFlag.value = 1;
		document.diagform.dslx_diag_state.value = 0;
	}
	document.diagform.submit();
}

function reset_diag_state(unit){
	if(unit == 0)
		var cfg = '/TCC.log.gz';
	else
		var cfg = '/TCC.log.'+unit+'.gz';
	var code = 'location.assign(\"' + cfg + '\")';
	eval(code);
}

function get_feedback_tarball(){
	var cfg = '/feedback.bin.gz';
	var code = 'location.assign(\"' + cfg + '\")';
	eval(code);
}
</script>
<style>
.feedback_info_0{
	font-size:20px;
	margin-left:25px;
	text-shadow: 1px 1px 0px black;
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bolder;
}

.feedback_info_1{
	font-size:13px;
	margin-left:30px;
	font-family: Arial, Helvetica, sans-serif;
}
</style>	
</head>

<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="diagform" action="/start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Feedback_Info.asp">
<input type="hidden" name="next_page" value="Advanced_Feedback.asp">
<input TYPE="hidden" name="DslDiagFlag" value="0">
<input type="hidden" name="dslx_diag_state" value="<% tcWebApi_staticGet("DslDiag_Entry", "dslx_diag_state", "s") %>">
</form>
<form method="post" name="form" action="" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value=""></form>
<FORM METHOD="POST" ACTION="" name="uiForm" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<INPUT TYPE="HIDDEN" NAME="dhcppoolFlag" VALUE="0">
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
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","menu_dsl_feedback","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>

<div id="fb_success" style="display:none;">
	<br>
	<br>
	<div class="feedback_info_0"><%tcWebApi_get("String_Entry","feedback_thanks","s")%></div>
	<br>
	<br>
	<%if tcWebApi_get("PushMail_Entry","fb_state","h") = "2" then %>
		<%if tcWebApi_get("GUITemp_Entry0","WANConnCurState","h") = "0" then %>
		<!-- fb_fail_WAN_disconnected -->
		<div class="feedback_info_1">
			<%tcWebApi_get("String_Entry","feedback_fail_WAN_disconnected","s")%> <%tcWebApi_get("String_Entry","feedback_fail1","s")%>(<a href="mailto:broadband_feedback@asus.com?Subject=<%tcWebApi_get("String_Entry","Web_Title2","s")%>" style="color:#FFCC00;" target="_top">broadband_feedback@asus.com</a>). <%tcWebApi_get("String_Entry","feedback_fail2","s")%> And download <span onClick="get_feedback_tarball();" style="text-decoration: underline; color:#FFCC00; cursor:pointer;">this debug file</span> and add it as email attachment.
		<br>
		<br>	
		<textarea name="fb_comment" maxlength="2000" cols="55" rows="8" style="width:90%;margin-left:25px;font-family:'Courier New', Courier, mono; font-size:13px;background:#475A5F;color:#FFFFFF;" readonly><% nvram_dump("fb_fail_content", "") %></textarea>
		</div>
		<br>
		<br>
		<%end if%>

		<%if tcWebApi_get("GUITemp_Entry0","WANConnCurState","h") = "1" then %>
		<!-- fb_fail_WAN_connected -->
		<div class="feedback_info_1">			
			<%tcWebApi_get("String_Entry","feedback_fail_WAN_connected","s")%> <%tcWebApi_get("String_Entry","feedback_fail1","s")%>(<a href="mailto:broadband_feedback@asus.com?Subject=<%tcWebApi_get("String_Entry","Web_Title2","s")%>" style="color:#FFCC00;" target="_top">broadband_feedback@asus.com</a>). <%tcWebApi_get("String_Entry","feedback_fail2","s")%> And download <span onClick="get_feedback_tarball();" style="text-decoration: underline; color:#FFCC00; cursor:pointer;">this debug file</span> and add it as email attachment.
		<br>
		<br>	
		<textarea name="fb_comment" maxlength="2000" cols="55" rows="8" style="width:90%;margin-left:25px;font-family:'Courier New', Courier, mono; font-size:13px;background:#475A5F;color:#FFFFFF;" readonly><% nvram_dump("fb_fail_content", "") %></textarea>
		</div>
		<br>
		<br>
		<%end if%>
	<%end if%>
	<div class="feedback_info_1">We are working hard to improve the firmware of <%tcWebApi_get("String_Entry","Web_Title2","s") %> and your feedback is very important to us. We will use your feedbacks and comments to strive to improve your ASUS experience.</div>
	<br>
</div>	<!-- fb_success end -->

<div id="fb_fail_WAN_connected" style="display:none;">
</div>	<!-- fb_fail_WAN_connected end -->

<div id="fb_fail_WAN_disconnected" style="display:none;">
</div>	<!-- fb_fail_WAN_disconnected end -->

<div id="fb_send_debug_log" style="display:none;">
	<br>
	<br>
	<div class="feedback_info_0">The debug log of diagnostic DSL / Wi-Fi captured.</div>
	<br>
	<div class="feedback_info_1" id="dl_diag_log_0" onClick="reset_diag_state(0);" style="display:none; text-decoration: underline; font-family:Lucida Console; cursor:pointer;">Click here to download the debug log and add as mail attachment.<br></div>

	<div class="feedback_info_1" id="dl_diag_log_multiple_desc" style="display:none;">Click on each link to download the complete log files and add as email attachment. Please note that each file size is around 5 MByte, please send separate mails to us(each mail contain only one attachment). <br></div>
	<div class="feedback_info_1" id="dl_diag_log_1" onClick="reset_diag_state('a');" style="display:none; text-decoration: underline; font-family:Lucida Console; cursor:pointer;">Partial debug log (Part 1)<br></div>
	<div class="feedback_info_1" id="dl_diag_log_2" onClick="reset_diag_state('b');" style="display:none; text-decoration: underline; font-family:Lucida Console; cursor:pointer;">Partial debug log (Part 2)<br></div>
	<div class="feedback_info_1" id="dl_diag_log_3" onClick="reset_diag_state('c');" style="display:none; text-decoration: underline; font-family:Lucida Console; cursor:pointer;">Partial debug log (Part 3)<br></div>
	<div class="feedback_info_1" id="dl_diag_log_4" onClick="reset_diag_state('d');" style="display:none; text-decoration: underline; font-family:Lucida Console; cursor:pointer;">Partial debug log (Part 4)<br></div>
	<div class="feedback_info_1" id="dl_diag_log_5" onClick="reset_diag_state('e');" style="display:none; text-decoration: underline; font-family:Lucida Console; cursor:pointer;">Partial debug log (Part 5)<br></div>
	<br>
	<div class="feedback_info_1">Please send us an email directly ( <a id="Email_subject" href="" target="_top" style="color:#FFCC00;">broadband_feedback@asus.com</a> ). <%tcWebApi_get("String_Entry","feedback_fail2","s")%> <br></div>
	<textarea name="fb_send_debug_log_content" cols="70" rows="15" style="width:90%;margin-left:25px;font-family:'Courier New', Courier, mono; font-size:13px;background:#475A5F;color:#FFFFFF;" readonly></textarea>
	<br>	
</div>	<!-- fb_send_debug_log end -->

<div id="fb_deny" style="display:none;">
</div>	



<div class="apply_gen">
	<input class="button_gen" onclick="applyRule();" type="button" value="<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>"/>
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
</html>

