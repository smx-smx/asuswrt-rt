<%
If Request_Form("experience_DSL_Flag") = "1" Then
tcWebApi_Set("Misc_Entry","experience_DSL_fb","experience_DSL_Feedback")	
tcWebApi_commit("Misc_Entry")
end if	

If Request_Form("saveFlag") = "1" Then
tcWebApi_Set("GUITemp_Entry0","fb_tmp_country","fb_country")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_ISP","fb_ISP")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_Subscribed_Info","fb_Subscribed_Info")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_email","fb_email")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_availability","fb_availability")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment0","fb_comment0")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment1","fb_comment1")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment2","fb_comment2")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment3","fb_comment3")
tcWebApi_Set("GUITemp_Entry0","fb_tmp_comment4","fb_comment4")
tcWebApi_Set("PushMail_Entry","PM_attach_syslog","PM_attach_syslog")
tcWebApi_Set("PushMail_Entry","PM_attach_cfgfile","PM_attach_cfgfile")
tcWebApi_Set("PushMail_Entry","PM_attach_iptables","PM_attach_iptables")
tcWebApi_commit("PushMail")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_DSL_Feedback.asp-->
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
function initial(){
	show_menu();
}

function redirect(){
	document.location.href = "/cgi-bin/Feedback_Info.asp";
}

function applyRule(){
		var wait_time = 60;
		document.adv_adsl.browserInfo.value = navigator.userAgent;
		if((link_status == "2" && (link_auxstatus == "0" || link_auxstatus == "2")) || (secondary_link_status == "2" && (secondary_link_auxstatus == "0" || secondary_link_auxstatus == "2"))){

			/*if(document.adv_adsl.fb_response.value == "3"){
				alert("Feedback report daily maximum(10) send limit reached.");
				return false;
			}*/
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

			split_fb_comment();
			
			if(document.adv_adsl.fb_email.value == ""){
					if(!confirm("E-mail address field is empty. Are you sure you want to proceed?")){
							document.adv_adsl.fb_email.focus();
							return false;
					}	
			}
			else{   //validate email
                        
                                if(!isEmail(document.adv_adsl.fb_email.value)){
                                                alert(Untranslated.Email_validation);
                                                document.adv_adsl.fb_email.focus();
                                                return false;
                                }
                }
			
			document.adv_adsl.saveFlag.value = 1;
			showLoading(wait_time);
			setTimeout("redirect();", wait_time*1000);
			document.adv_adsl.submit();
		}
		else{
			alert("<%tcWebApi_get("String_Entry","USB_App_No_Internet","s")%>");
			return false;
		}
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
<form method="post" name="form" action="" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="experience_DSL_Flag" value="0">
<input type="hidden" name="experience_DSL_Feedback" value="0">
</form>
<FORM METHOD="POST" name="adv_adsl" ACTION="/cgi-bin/start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_DSL_Feedback.asp">
<input type="hidden" name="PM_attach_syslog" value="<% tcWebApi_Get("PushMail_Entry", "PM_attach_syslog", "s") %>">
<input type="hidden" name="PM_attach_cfgfile" value="<% tcWebApi_Get("PushMail_Entry", "PM_attach_cfgfile", "s") %>">
<input type="hidden" name="PM_attach_iptables" value="<% tcWebApi_Get("PushMail_Entry", "PM_attach_iptables", "s") %>">	
<input type="hidden" name="fb_comment0" value="">
<input type="hidden" name="fb_comment1" value="">
<input type="hidden" name="fb_comment2" value="">
<input type="hidden" name="fb_comment3" value="">
<input type="hidden" name="fb_comment4" value="">
<input type="hidden" name="fb_response" value="<% tcWebApi_Get("WebCurSet_Entry", "feedbackresponse", "s") %>">
<input type="hidden" name="browserInfo" value="">
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
<th>Name of the Subscribed Plan/Service/Package *</th>
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
<th>Extra information for debugging *</th>
<td>
	<input type="checkbox" class="input" name="attach_syslog">Syslog&nbsp;&nbsp;&nbsp;
	<input type="checkbox" class="input" name="attach_cfgfile">Setting file&nbsp;&nbsp;&nbsp;
	<input type="checkbox" class="input" name="attach_iptables">Iptable setting
</td>
</tr>

<tr>
<th><%tcWebApi_get("String_Entry","feedback_connection_type","s")%></th>
<td>
	<select id="" class="input_option" name="fb_availability">
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
		<i>Maximum of 2000 characters - characters left : <input type="text" class="input_6_table" name="msglength" id="msglength" maxlength="4" value="2000" readonly></i>
	</td>
</tr>

<tr align="center">
	<td colspan="2">	
		<div style="margin-left:-680px;">
		<%tcWebApi_get("String_Entry","feedback_optional","s")%>	
		</div>
		<input class="button_gen" onclick="applyRule()" type="button" value="Send"/>
	</td>	
</tr>

<tr>
	<td colspan="2">
		<strong><%tcWebApi_get("String_Entry","FW_note","s")%></strong>
		<ul>
			<li>The Firmware and DSL Driver Version will be submitted in addition to any info you choose to include above.</li>
			<li>DSL feedback will be used to diagnose problems and help to improve the firmware of <%tcWebApi_get("String_Entry","Web_Title2","s")%>, any personal information you submitted, whether explicitly or incidentally will be protected in accordance with our <a style='font-weight: bolder;text-decoration:underline;cursor:pointer;' href='http://www.asus.com/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy/' target='_blank'>privacy policy</a>.</li>
			<li>By submitting this DSL Feedback, you agree that ASUS may use feedback that you provided to improve ASUS xDSL modem router product.</li>
		</ul>
	</td>
</tr>	
</table>
<INPUT TYPE="HIDDEN" NAME="saveFlag" VALUE="0">
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

<!--Advanced_DSL_Feedback.asp-->
</html>

