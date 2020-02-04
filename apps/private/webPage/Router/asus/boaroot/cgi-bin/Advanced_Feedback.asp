<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_Feedback.asp-->
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
<script type="text/javaScript" src="/jquery.js"></script>
<script type="text/javascript" src="/js/httpApi.js"></script>
<script>

function initial(){
	show_menu();
	if(modem_support == -1){
		document.adv_adsl.attach_modemlog.checked = false;
		document.getElementById("attach_modem_span").style.display = "none";
	}

	if(dsl_diag_support >= 0 && (Diag2jffs_support || usb_support >= 0) ){
		change_dsl_diag_enable(0);
		check_dsl_diag_state();
		if(!wl_info.band5g_support){		//single band
			inputCtrl(document.adv_adsl.dslx_diag_enable[3], 0);
			document.adv_adsl.dslx_diag_enable[3].style.display = "none";
			document.getElementById("5GHz_text").style.display = "none";
		}
	}
	else{
		inputCtrl(document.adv_adsl.dslx_diag_enable[0], 0);
		inputCtrl(document.adv_adsl.dslx_diag_enable[1], 0);
		inputCtrl(document.adv_adsl.dslx_diag_enable[2], 0);
		inputCtrl(document.adv_adsl.dslx_diag_enable[3], 0);
		inputCtrl(document.adv_adsl.dslx_diag_duration, 0);	
		document.getElementById("dslx_diag_enable_tr").style.display = "none";
		document.getElementById("dslx_diag_duration_tr").style.display = "none";
	}

	//Renjie: do not check WAN connection.
	//setTimeout("check_wan_state();", 300);
}

function check_dsl_diag_state(){
	if(wan_diag_state == 1){	//diagnostic log capturing
		document.adv_adsl.dslx_diag_enable[0].disabled = "true";
		document.adv_adsl.dslx_diag_enable[1].disabled = "true";
		document.adv_adsl.dslx_diag_enable[2].disabled = "true";
		if(wl_info.band5g_support){
			document.adv_adsl.dslx_diag_enable[3].disabled = "true";
		}
		else{
			document.adv_adsl.dslx_diag_enable[3].style.display = "none";
			document.getElementById("5GHz_text").style.display = "none";
		}
		document.adv_adsl.dslx_diag_duration.disabled = "true";
		document.getElementById("diag_proceeding").style.display = "";
		document.getElementById("diag_proceeding").innerHTML = "* Diagnostic debug log capture in progress. " +show_diagTime(boottime_update)+"<br><span onclick=\"cancel_diag();\" style=\"cursor:pointer;text-decoration:underline;\">Cancel debug capture</span>";
	}
	else{
		document.adv_adsl.dslx_diag_enable[0].disabled = "";
		document.adv_adsl.dslx_diag_enable[1].disabled = "";
		document.adv_adsl.dslx_diag_enable[2].disabled = "";
		if(wl_info.band5g_support){
			document.adv_adsl.dslx_diag_enable[3].disabled = "";
		}
		else{
			document.adv_adsl.dslx_diag_enable[3].style.display = "none";
			document.getElementById("5GHz_text").style.display = "none";
		}
		document.adv_adsl.dslx_diag_duration.disabled = "";
		document.getElementById("diag_proceeding").style.display = "none";
	}

	setTimeout("check_dsl_diag_state();", 3000);
}

function check_wan_state(){
	
	if(sw_mode != 3 && document.getElementById("connect_status").className == "connectstatusoff"){
		document.getElementById("fb_desc_disconnect").style.display = "";
		document.adv_adsl.fb_country.disabled = "true";
		document.adv_adsl.fb_ISP.disabled = "true";
		document.adv_adsl.fb_Subscribed_Info.disabled = "true";
		document.adv_adsl.fb_email.disabled = "true";		
		document.adv_adsl.fb_serviceno.disabled = "true";
		document.adv_adsl.attach_syslog.disabled = "true";
		document.adv_adsl.attach_cfgfile.disabled = "true";
		document.adv_adsl.attach_iptables.disabled = "true";
		document.adv_adsl.attach_modemlog.disabled = "true";
		if(dsl_diag_support >= 0){
			document.adv_adsl.dslx_diag_enable[0].disabled = "true";
			document.adv_adsl.dslx_diag_enable[1].disabled = "true";
			document.adv_adsl.dslx_diag_enable[2].disabled = "true";
			document.adv_adsl.dslx_diag_enable[3].disabled = "true";
			document.adv_adsl.dslx_diag_duration.disabled = "true";
		}
		document.adv_adsl.fb_availability.disabled = "true";		
		document.adv_adsl.fb_comment.disabled = "true";		
		document.adv_adsl.btn_send.disabled = "true";
		
	}
	else{
		document.getElementById("fb_desc_disconnect").style.display = "none";
		document.adv_adsl.fb_country.disabled = "";
		document.adv_adsl.fb_ISP.disabled = "";
		document.adv_adsl.fb_Subscribed_Info.disabled = "";
		document.adv_adsl.fb_email.disabled = "";
		document.adv_adsl.fb_serviceno.disabled = "";
		document.adv_adsl.attach_syslog.disabled = "";
		document.adv_adsl.attach_cfgfile.disabled = "";
		document.adv_adsl.attach_iptables.disabled = "";
		document.adv_adsl.attach_modemlog.disabled = "";
		if(dsl_diag_support >= 0 && wan_diag_state == 1){
			document.adv_adsl.dslx_diag_enable[0].disabled = "true";
			document.adv_adsl.dslx_diag_enable[1].disabled = "true";
			document.adv_adsl.dslx_diag_enable[2].disabled = "true";
			document.adv_adsl.dslx_diag_enable[3].disabled = "true";
			document.adv_adsl.dslx_diag_duration.disabled = "true";
			document.getElementById("diag_proceeding").style.display = "";
			document.getElementById("diag_proceeding").innerHTML = "* Diagnostic debug log capture in progress. " +show_diagTime(boottime_update)+"<br><span onclick=\"cancel_diag();\" style=\"cursor:pointer;text-decoration:underline;\">Cancel debug capture</span>";
		}
		else if(dsl_diag_support >= 0){
			document.adv_adsl.dslx_diag_enable[0].disabled = "";
			document.adv_adsl.dslx_diag_enable[1].disabled = "";
			document.adv_adsl.dslx_diag_enable[2].disabled = "";
			if(wl_info.band5g_support){
				document.adv_adsl.dslx_diag_enable[3].disabled = "";
			}
			document.adv_adsl.dslx_diag_duration.disabled = "";
			document.getElementById("diag_proceeding").style.display = "none";
		}
		document.adv_adsl.fb_availability.disabled = "";
		document.adv_adsl.fb_comment.disabled = "";		
		document.adv_adsl.btn_send.disabled = "";
			
	}		
		
	setTimeout("check_wan_state();", 3000);
}

function updateUSBStatus(){

	if(Diag2jffs_support){
		return;
	}

	if((rc_support.search("usbX") == -1 || rc_support.search("usbX1") >= 0) && usb_path1.search("storage") == -1){	//single USB port		
		document.getElementById("be_lack_storage").style.display = "";
	}
	else if(rc_support.search("usbX2") >= 0 && (usb_path1.search("storage") == -1 && usb_path2.search("storage") == -1)){	//two USBport
		document.getElementById("be_lack_storage").style.display = "";
	} 
	else{		
		document.getElementById("be_lack_storage").style.display = "none";
	}					
}

function redirect(){
	document.location.href = "/cgi-bin/Feedback_Info.asp";
}

function applyRule(){		
		/*if(document.adv_adsl.fb_response.value == "3"){
			alert("Feedback report daily maximum(10) send limit reached.");
			return false;
		}*/

		if(!document.adv_adsl.eula_checkbox.checked){
			alert('<%tcWebApi_get("String_Entry","feedback_eula_notice","s")%>');
			return false;
		}

		if(document.adv_adsl.attach_syslog.checked == true)
			document.adv_adsl.PM_attach_syslog.value = 1;
		else
			document.adv_adsl.PM_attach_syslog.value = 0;
		if(document.adv_adsl.attach_cfgfile.checked == true)
			document.adv_adsl.PM_attach_cfgfile.value = 1;
		else
			document.adv_adsl.PM_attach_cfgfile.value = 0;
		if(document.adv_adsl.attach_iptables.checked == true)
			document.adv_adsl.PM_attach_iptables.value = 1;
		else
			document.adv_adsl.PM_attach_iptables.value = 0;
		if(document.adv_adsl.attach_modemlog.checked == true)
			document.adv_adsl.PM_attach_modemlog.value = 1;
		else
			document.adv_adsl.PM_attach_modemlog.value = 0;

		split_fb_comment();
		
		if(document.adv_adsl.fb_email.value == ""){
				if(!confirm("<%tcWebApi_get("String_Entry","feedback_email_confirm","s")%>")){
						document.adv_adsl.fb_email.focus();
						return false;
				}
		}
		else{   //validate email                        
				if(!isEmail(document.adv_adsl.fb_email.value)){
					alert("<%tcWebApi_get("String_Entry","feedback_email_alert","s")%>");
					document.adv_adsl.fb_email.focus();
					return false;
				}
		}

		var re = new RegExp("^[a-zA-Z][0-9]{10}","gi");
		if(!re.test(document.adv_adsl.fb_serviceno.value) && document.adv_adsl.fb_serviceno.value != ""){
			alert("<%tcWebApi_get("String_Entry","JS_validchar","s")%>");
			document.adv_adsl.fb_serviceno.focus();
			return false;
		}
		
		document.adv_adsl.saveFlag.value = 1;
		if(document.adv_adsl.dslx_diag_enable[1].checked == true ||
			document.adv_adsl.dslx_diag_enable[2].checked == true ||
			document.adv_adsl.dslx_diag_enable[3].checked == true)
		{
			document.adv_adsl.DslDiagFlag.value = 1;
			if(Diag2jffs_support){
				document.adv_adsl.chg2jffsFlag.value = 1;
			}
			showLoading(120);
			setTimeout("redirect();", 120*1000);
		}
		else{
			showLoading(60);
			setTimeout("redirect();", 60*1000);
		}
		document.adv_adsl.browserInfo.value = navigator.userAgent;
		if(document.getElementById("connect_status").className == "connectstatuson"){
			document.adv_adsl.WANConnCurState.value = 1;	//Submit form with WAN connected state
		}	
		document.adv_adsl.submit();
}

function isEmail(strE) {
        if (strE.search(/^[A-Za-z0-9]+((-[A-Za-z0-9]+)|(\.[A-Za-z0-9]+)|(_[A-Za-z0-9]+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z]+$/) != -1)
                return true;
        else
                return false;
}

function split_fb_comment(){
	document.adv_adsl.fb_comment0.value = document.adv_adsl.fb_comment.value.substring(0,500);
	document.adv_adsl.fb_comment1.value = document.adv_adsl.fb_comment.value.substring(500,1000);
	document.adv_adsl.fb_comment2.value = document.adv_adsl.fb_comment.value.substring(1000,1500);
	document.adv_adsl.fb_comment3.value = document.adv_adsl.fb_comment.value.substring(1500,2000);
	document.adv_adsl.fb_comment4.value = document.adv_adsl.fb_comment.value.substring(2000,2500);
}

function textCounter(field, cnt, upper) {
	if (field.value.length > upper)
		field.value = field.value.substring(0, upper);
	else
		cnt.value = upper - field.value.length;
}

function change_dsl_diag_enable(value) {
	if(value > 0) {
		if(Diag2jffs_support){
			document.adv_adsl.dslx_diag_duration[0].style.display = "none";
			document.adv_adsl.dslx_diag_duration[1].selected = true;
			document.adv_adsl.dslx_diag_duration[2].style.display = "none";
			document.adv_adsl.dslx_diag_duration[3].style.display = "none";
			document.adv_adsl.dslx_diag_duration[4].style.display = "none";
			document.adv_adsl.dslx_diag_duration[5].style.display = "none";
			showhide("dslx_diag_duration_tr",1);
		}	
		else if(rc_support.search("usbX") == -1 || rc_support.search("usbX1") >= 0){	//single USB port
			if(usb_path1.search("storage") == -1){
				alert("USB disk required in order to store the debug log, please plug-in a USB disk to <%tcWebApi_get("String_Entry","Web_Title2","s")%> and Enable DSL Line / Wi-Fi Diagnostic again.");
				document.adv_adsl.dslx_diag_enable[0].checked = true;
				return;
			}
			else{
				alert("While debug log capture in progress, please do not unplug the USB disk as the debug log would be stored in the disk. UI top right globe icon flashing in yellow indicating that debug log capture in progress. Click on the yellow globe icon could cancel the debug log capture.");	/* Please note that xDSL line would resync in one minute after Feedback form submitted.*/
				showhide("dslx_diag_duration_tr",1);
			}	
		}
		else if(rc_support.search("usbX2") >= 0 ){	//two USB port
			if(usb_path1.search("storage") == -1 && usb_path2.search("storage") == -1){
				alert("USB disk required in order to store the debug log, please plug-in a USB disk to <%tcWebApi_get("String_Entry","Web_Title2","s")%> and Enable DSL Line / Wi-Fi Diagnostic again.");
				document.adv_adsl.dslx_diag_enable[0].checked = true;
				return;
			}
			else{
				alert("While debug log capture in progress, please do not unplug the USB disk as the debug log would be stored in the disk. UI top right globe icon flashing in yellow indicating that debug log capture in progress. Click on the yellow globe icon could cancel the debug log capture.");	/* Please note that xDSL line would resync in one minute after Feedback form submitted.*/
				showhide("dslx_diag_duration_tr",1);
			}
		}
				
	}
	else {
		showhide("dslx_diag_duration_tr",0);
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
<form method="POST" name="adv_adsl" action="/start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_Feedback.asp">
<input type="hidden" name="PM_attach_syslog" value="<% tcWebApi_Get("PushMail_Entry", "PM_attach_syslog", "s") %>">
<input type="hidden" name="PM_attach_cfgfile" value="<% tcWebApi_Get("PushMail_Entry", "PM_attach_cfgfile", "s") %>">
<input type="hidden" name="PM_attach_iptables" value="<% tcWebApi_Get("PushMail_Entry", "PM_attach_iptables", "s") %>">
<input type="hidden" name="PM_attach_modemlog" value="<% tcWebApi_Get("PushMail_Entry", "PM_attach_modemlog", "s") %>">
<input type="hidden" name="fb_comment0" value="">
<input type="hidden" name="fb_comment1" value="">
<input type="hidden" name="fb_comment2" value="">
<input type="hidden" name="fb_comment3" value="">
<input type="hidden" name="fb_comment4" value="">
<input type="hidden" name="fb_response" value="<% tcWebApi_Get("PushMail_Entry", "fb_state", "s") %>">
<input type="hidden" name="browserInfo" value="">
<input type="hidden" name="WANConnCurState" value="0">
<input type="hidden" name="saveFlag" value="0">
<input type="hidden" name="DslDiagFlag" value="0">
<input type="hidden" name="chg2jffsFlag" value="0">
<input type="hidden" name="dslx_diag_log_path" value="/jffs">
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
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","menu_dsl_feedback","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","Feedback_desc","s")%></div>
<div id="fb_desc_disconnect" class="formfontdesc" style="display:none;color:#FC0;">Now this function can't work, because your ASUS Router isn't connected to the Internet. Please send your Feedback to this email address : <a href="mailto:broadband_feedback@asus.com?Subject=<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>" target="_top" style="color:#FFCC00;">broadband_feedback@asus.com </a></div><!-- untranslated -->
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
<th width="30%"><%tcWebApi_get("String_Entry","feedback_country","s")%> *</th>
<td>
	<input type="text" name="fb_country" maxlength="30" class="input_25_table" value="">
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","feedback_isp","s")%> *</th>
<td>
	<input type="text" name="fb_ISP" maxlength="40" class="input_25_table" value="">
</td>
</tr>
<tr>
<th>Subscribed Plan/Service/Package *</th>
<td>
	<input type="text" name="fb_Subscribed_Info" maxlength="50" class="input_25_table" value="">
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","feedback_email","s")%> *</th>
<td>
	<input type="text" name="fb_email" maxlength="50" class="input_25_table" value="">
</td>
</tr>

<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(34,2);"><%tcWebApi_get("String_Entry","ASUS_Service_No","s")%></a></th>
<td>
	<input type="text" name="fb_serviceno" maxlength="11" class="input_15_table" value="">
</td>
</tr>
<tr>

<th>Extra information for debugging *</th>
<td>
	<input type="checkbox" class="input" name="attach_syslog">Syslog&nbsp;&nbsp;&nbsp;
	<input type="checkbox" class="input" name="attach_cfgfile">Setting file&nbsp;&nbsp;&nbsp;
	<input type="checkbox" class="input" name="attach_iptables">Iptable setting&nbsp;&nbsp;&nbsp;
	<span id="attach_modem_span" style="color:#FFFFFF;"><input type="checkbox" class="input" name="attach_modemlog" id="attach_modemlog_id"><label for="attach_modemlog_id">3G/4G log</label></span>
</td>
</tr>

<tr id="dslx_diag_enable_tr">
	<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,19);">DSL Line / Wi-Fi Diagnostic *</a></th>
	<td>
		<input type="radio" name="dslx_diag_enable" class="input" value="0" onclick="change_dsl_diag_enable(0);" checked><%tcWebApi_get("String_Entry","btn_disable","s")%>
		<input type="radio" name="dslx_diag_enable" class="input" value="1" onclick="change_dsl_diag_enable(1);">xDSL line
		<input type="radio" name="dslx_diag_enable" class="input" value="2" onclick="change_dsl_diag_enable(2);">Wi-Fi 2.4GHz
		<input type="radio" name="dslx_diag_enable" class="input" value="3" onclick="change_dsl_diag_enable(3);"><span id="5GHz_text" style="color:white;">Wi-Fi 5GHz</span>
		<br>
		<span id="be_lack_storage" style="display:none;color:#FC0">* No USB disk plug-in.</span>
		<span id="diag_proceeding" style="display:none;color:#FC0"></span>
	</td>
</tr>

<tr id="dslx_diag_duration_tr">
	<th>Diagnostic debug log capture duration *</th>
	<td>
		<select class="input_option" name="dslx_diag_duration">
			<option value="0" selected><%tcWebApi_get("String_Entry","Auto","s")%></option>
			<option value="3600">1 <%tcWebApi_get("String_Entry","Hour","s")%></option>
			<option value="18000">5 <%tcWebApi_get("String_Entry","Hour","s")%></option>
			<option value="43200">12 <%tcWebApi_get("String_Entry","Hour","s")%></option>
			<option value="86400">24 <%tcWebApi_get("String_Entry","Hour","s")%></option>
			<option value="172800">48 <%tcWebApi_get("String_Entry","Hour","s")%></option>
		</select>
	</td>
</tr>

<tr>
<th><%tcWebApi_get("String_Entry","feedback_connection_type","s")%></th>
<td>
	<select class="input_option" name="fb_availability">
		<option value="Not_selected"><%tcWebApi_get("String_Entry","Select_menu_default","s")%> ...</option>
		<option value="Stable_connection"><%tcWebApi_get("String_Entry","feedback_stable","s")%></option>
		<option value="Occasional_interruptions"><%tcWebApi_get("String_Entry","feedback_Occasion_interrupt","s")%></option>
		<option value="Frequent_interruptions"><%tcWebApi_get("String_Entry","feedback_Frequent_interrupt","s")%></option>
		<option value="Unavailable"><%tcWebApi_get("String_Entry","feedback_usaul_unavailable","s")%></option>
	</select>
</td>
</tr>
<tr>
	<th>
		<%tcWebApi_get("String_Entry","feedback_comments","s")%> *
	</th>
	<td>		
		<textarea name="fb_comment" maxlength="2000" cols="55" rows="8" style="font-family:'Courier New', Courier, mono; font-size:13px;background:#475A5F;color:#FFFFFF;" onKeyDown="textCounter(this,document.adv_adsl.msglength,2000);" onKeyUp="textCounter(this,document.adv_adsl.msglength,2000)"></textarea>
		<i><%tcWebApi_get("String_Entry","feedback_max_counts","s")%> : <input type="text" class="input_6_table" name="msglength" id="msglength" maxlength="4" value="2000" readonly></i>
	</td>
</tr>

<tr>
	<td colspan="2">
        <div>
            <div style="float: left;"><input type="checkbox" name="eula_checkbox"/></div>
            <div id="eula_content" style="margin-left: 20px;"><%tcWebApi_get("String_Entry","feedback_eula","s")%></div>
        </div>
        <input class="button_gen" style="margin-left: 305px; margin-top:5px;" name="btn_send" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","btn_send","s")%>"/>
    </td>
</tr>

<tr>
	<td colspan="2">
		<strong><%tcWebApi_get("String_Entry","FW_note","s")%></strong>
		<ul>
			<li><%tcWebApi_get("String_Entry","feedback_note4","s")%><br><a style="font-weight: bolder;text-decoration:underline;cursor:pointer;" href="https://www.asus.com/support/CallUs/" target="_blank">https://www.asus.com/support/CallUs/</a></li>
		</ul>
	</td>
</tr>	
</table>
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

<!--Advanced_Feedback.asp-->
</html>

