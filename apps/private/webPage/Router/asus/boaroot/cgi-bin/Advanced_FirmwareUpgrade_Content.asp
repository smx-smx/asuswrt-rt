<%
	if request_Form("postflag") = "1" then
		stop_service()
		tcWebApi_set("System_Entry","upgrade_fw_status","value_NONE")
		tcWebApi_set("System_Entry","upgrade_fw","HTML_HEADER_TYPE")
		tcWebApi_CommitWithoutSave("System_Entry")
	end if	
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<!--Advanced_FirmwareUpgrade_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_6_3","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<style>
	.Bar_container{
	width:85%;
	height:21px;
	border:2px inset #999;
	margin:0 auto;
	background-color:#FFFFFF;
	z-index:100;
	}
	#proceeding_img_text{
	position:absolute; z-index:101; font-size:11px; color:#000000; margin-left:110px; line-height:21px;
	}
	#proceeding_img{
	height:21px;
	background:#C0D1D3 url(/images/proceeding_img.gif);
	}
</style>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/ajax.js"></script>
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var varload = 0;
var count=0;

var winWidth = document.documentElement.scrollWidth;

var webs_state_update = '<% tcWebApi_get("WebCustom_Entry", "webs_state_update", "s" ) %>';
var webs_state_upgrade = '<% tcWebApi_get("WebCustom_Entry", "webs_state_upgrade", "s" ) %>';
var webs_state_error = '<% tcWebApi_get("WebCustom_Entry", "webs_state_error", "s" ) %>';
var webs_state_info = '<% tcWebApi_get("WebCustom_Entry", "webs_state_info", "s" ) %>';
var webs_state_reboot = '<% tcWebApi_get("WebCustom_Entry", "webs_state_reboot", "s" ) %>';

var exist_firmver="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>";

function detect_firmware(){

	$j.ajax({
    		url: '/cgi-bin/detect_firmware.asp',
    		dataType: 'script',

    		error: function(xhr){
    				setTimeout("detect_firmware();", 1000);
    		},

    		success: function(){

      			if(webs_state_update==0){
      					setTimeout("detect_firmware();", 1000);
      			}else{
      					if(webs_state_error==1 && webs_state_info==""){
      						document.getElementById('update_scan').style.display="none";
									document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","connect_failed","s")%>";
      						return;
      					}
      					else if(webs_state_error==1 && webs_state_info != ""){
      						document.getElementById('update_scan').style.display="none";
      						document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","FIRM_fail_desc","s")%>";
      						return;
      					}
      					else{
      						var Latest_firmver = webs_state_info;
	      					if(Latest_firmver.length > 0){	//match model FW
	      						Latest_firm = parseInt(Latest_firmver);
      							current_firm = parseInt(exist_firmver.replace(/[.]/gi,""));
      							if(current_firm < Latest_firm){
								document.getElementById('update_scan').style.display="none";												
								if(webs_state_reboot > 0)
									var confirm_content = "<%tcWebApi_get("String_Entry","exist_new","s")%>\n\nAfter firmware updated, please press the reset button more than five seconds to reset the (modem) router in order to avoid some compatibility issues.\n\nDo not power off <%tcWebApi_get("String_Entry","Web_Title2","s")%> while upgrade in progress.";
								else
									var confirm_content = "<%tcWebApi_get("String_Entry","exist_new","s")%>\n\nDo not power off <%tcWebApi_get("String_Entry","Web_Title2","s")%> while upgrade in progress.";
													
								if(confirm(confirm_content)){
	
									document.start_update.action_mode.value="apply";
									document.start_update.action_script.value="start_webs_upgrade";
									document.start_update.live_upgrade_flag.value="1";
									document.start_update.DOWNLOAD_HEADER_TYPE.value="1";
									startDownloading();
									document.start_update.submit();
									return;
								}
								else{
									document.getElementById('update_scan').style.display="none";
									document.getElementById('update_states').innerHTML="";
								} 									
							}
							else{												
								document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","is_latest","s")%>";
								document.getElementById('update_scan').style.display="none";
    							}
      						}
	      					else{		//miss-match model FW
      							
							document.getElementById('update_scan').style.display="none";
							document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","unavailable_update","s")%>.";
							return;
						}
					}
			}
  		}
  	});
}

function detect_update(){	
  document.start_update.action_mode.value="apply";
  document.start_update.action_script.value="start_webs_update";
  document.start_update.action_wait.value="60";

  document.start_update.live_update_flag.value="1";

	if((link_status == "2" && (link_auxstatus == "0" || link_auxstatus == "2")) || (secondary_link_status == "2" && (secondary_link_auxstatus == "0" || secondary_link_auxstatus == "2"))){
	  document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","check_proceeding","s")%>";
	  document.getElementById('update_scan').style.display="";
	  setTimeout("redirect_current();", 10000);
		document.start_update.submit();
	}
	else{
		document.getElementById('update_scan').style.display="none";
		document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","USB_App_No_Internet","s")%>";
	}
}

function initial(){
	show_menu();
	if(live_update_support == -1)
		document.getElementById("update").style.display = "none";
	else if('<% tcWebApi_get("WebCustom_Entry", "webs_state_update", "s" ) %>' != '')
		detect_firmware();
}

function redirect(){
	document.location.href = "/cgi-bin/index2.asp";
}

function redirect_current(){
	document.location.href = "/cgi-bin/Advanced_FirmwareUpgrade_Content.asp";
}

function uiDoUpdate()
{
	var form=document.uiPostUpdateForm;
	var string4 = form.tools_FW_UploadFile.value.search(/<%tcWebApi_Get("SysInfo_Entry", "ProductName", "s")%>/);

	if (form.tools_FW_UploadFile.value=="") {
		alert("You must select a firmware file to upload.");
	}
	else {
		if (string4 >= 0) {
			form.postflag.value = "1";
			if(model_name == "DSL-N66U")
			{
				showLoading(220);
				setTimeout("redirect();", 220000);
			}
			else if(model_name == "DSL-N55U-C1" || model_name == "DSL-N16U")
			{
				showLoading(142);
				setTimeout("redirect();", 142000);
			}
			else //DSL-N14U ...
			{
				showLoading(172);
				setTimeout("redirect();", 172000);
			}
			chk_upgrade();
			form.submit();
		}
		else
			alert("<%tcWebApi_get("String_Entry","FW_item5","s")%> <%tcWebApi_get("String_Entry","Manual_Setting_JS_invalid","s")%>");
	}
}

var dead = 0;
function detect_httpd(){
	$j.ajax({
    		url: '/cgi-bin/httpd_check.asp',
    		dataType: 'script',
				timeout: 1500,
    		error: function(xhr){
    				dead++;
    				if(dead < 6)
    						setTimeout("detect_httpd();", 1000);
    				else{
    						$('loading_block1').style.display = "none";
    						$('loading_block2').style.display = "none";
    						$('loading_block3').style.display = "";
    						$('loading_block3').innerHTML = "<div><%tcWebApi_get("String_Entry","Firm_reboot_manually","s")%></div>";
    				}
    		},

    		success: function(){
    				setTimeout("hideLoadingBar();",1000);
      			location.href = "/cgi-bin/index2.asp";
  			}
  		});
}

var rebooting = 0;
function isDownloading(){
	$j.ajax({
    		url: '/cgi-bin/detect_firmware.asp',
    		dataType: 'script',
				timeout: 1500,
    		error: function(xhr){
					// start reboot.
					if(rebooting > 20){
						$("hiddenMask").style.visibility = "hidden";
						showLoadingBar(180);
					}
					else
						rebooting++
    		},
    		success: function(){
					rebooting = 0;
					if(webs_state_error == 1){
						$("drword").innerHTML = "<%tcWebApi_get("String_Entry","FIRM_fail_desc","s")%><br><%tcWebApi_get("String_Entry","FW_desc1","s")%>";
						return false;
					}
					else if(webs_state_error == 2){
						$("drword").innerHTML = "Memory space is NOT enough to upgrade on internet. Please wait for rebooting.<br><%tcWebApi_get("String_Entry","FW_desc1","s")%>";
						return false;
					}
					else if(webs_state_upgrade != 0 && webs_state_error == 0){
						$("hiddenMask").style.visibility = "hidden";
						if(model_name == "DSL-N66U"){
								showLoadingBar(220);
								setTimeout("detect_httpd();", 220000);
						}
						else if(model_name == "DSL-N55U-C1" || model_name == "DSL-N16U"){
								showLoadingBar(142);
								setTimeout("detect_httpd();", 142000);
						}
						else{	 //DSL-N14U ...else
								showLoadingBar(172);
								setTimeout("detect_httpd();", 172000);
						}
												
					}
					else{
    				setTimeout("isDownloading();", 2000);
					}
  			}
  		});
}

function DownloadDone(){
			if('<% tcWebApi_get("WebCustom_Entry", "webs_state_error", "s" ) %>' == '1'){
						$("drword").innerHTML = "<%tcWebApi_get("String_Entry","FIRM_fail_desc","s")%><br><%tcWebApi_get("String_Entry","FW_desc1","s")%>";
						return false;
			}
			else{
				
						$("hiddenMask").style.visibility = "hidden";
						if(model_name == "DSL-N66U"){
								showLoadingBar(220);
								setTimeout("detect_httpd();", 220000);
						}
						else if(model_name == "DSL-N55U-C1" || model_name == "DSL-N16U"){
								showLoadingBar(142);
								setTimeout("detect_httpd();", 142000);
						}
						else{	 //DSL-N14U ...else
								showLoadingBar(172);
								setTimeout("detect_httpd();", 172000);
						
						}
						document.redirectForm.live_do_upgrade_flag.value = "1";
						setTimeout("document.redirectForm.submit();", 15000);
			}			
}

function startDownloading(){
	disableCheckChangedStatus();
	dr_advise();
	$("drword").innerHTML = "&nbsp;&nbsp;&nbsp;<%tcWebApi_get("String_Entry","fw_downloading","s")%>...";
}

var dead_sts =0;
function chk_upgrade(){
	$j.ajax({
    		url: '/cgi-bin/detect_firmware_sts.asp',
    		dataType: 'script',
				timeout: 1500,
    		error: function(xhr){
    			dead_sts++;
    			if(dead_sts < 10)    				
							chk_upgrade();
					//else
							//do nothing
    		},
    		success: function(){
					if(upgrade_sts == "FAIL")
					{
						hideLoading();
						showLoading(120, "upgrade_FAIL");
						setTimeout("redirect();", 120000);
					}
					else if(upgrade_sts == "SAME")
					{
						hideLoading();
						showLoading(120, "upgrade_SAME");
						setTimeout("redirect();", 120000);
					}
					else
						setTimeout("chk_upgrade();", 1000);
  			}
  		});
}
function LoadingProgress(seconds){
	if(webs_state_reboot > 0)
		document.getElementById("loading_block2").innerHTML = "<%tcWebApi_get("String_Entry","FIRM_ok_desc","s")%><br>After firmware updated, please press the reset button more than five seconds to reset the (modem) router in order to avoid some compatibility issues.<br>Do not power off <%tcWebApi_get("String_Entry","Web_Title2","s")%> while upgrade in progress.";
	$("LoadingBar").style.visibility = "visible";
	y = y + progress;
	if(typeof(seconds) == "number" && seconds >= 0){
		if(seconds != 0){
			$("proceeding_img").style.width = Math.round(y) + "%";
			$("proceeding_img_text").innerHTML = Math.round(y) + "%";
			if($("loading_block1")){
				$("proceeding_img_text").style.width = document.getElementById("loading_block1").clientWidth;
				$("proceeding_img_text").style.marginLeft = "175px";
			}	
			--seconds;
			setTimeout("LoadingProgress("+seconds+");", 1000);
		}
		else{
			$("proceeding_img_text").innerHTML = "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>";
			y = 0;
			if(location.pathname.indexOf("QIS_wizard.asp") < 0){
				setTimeout("hideLoadingBar();",1000);
				location.href = "index.asp";
			}
		}
	}
}
</script>
</head>
<body onload="initial();">
<div id="TopBanner"></div>
<div id="LoadingBar" class="popup_bar_bg">
<table cellpadding="5" cellspacing="0" id="loadingBarBlock" class="loadingBarBlock" align="center">
<tr>
<td height="80">
<div id="loading_block1" class="Bar_container">
<span id="proceeding_img_text" ></span>
<div id="proceeding_img"></div>
</div>
		<div id="loading_block2" style="margin:5px auto; width:85%;"><%tcWebApi_get("String_Entry","FIRM_ok_desc","s")%><br>Do not power off <%tcWebApi_get("String_Entry","Web_Title2","s")%> while upgrade in progress.</div>
		<div id="loading_block3" style="margin:5px auto;width:85%; font-size:12pt;"></div>
</td>
</tr>
</table>
</div>
<div id="Loading" class="popup_bg"></div><div id="hiddenMask" class="popup_bg">
<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
<tr>
<td>
<br/>
<div class="drword" id="drword" style="height:50px;">&nbsp;&nbsp;&nbsp;Proceeding...
<br/>
<br/>
</div>
</td>
</tr>
</table>
</div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
</form>
<form method="post" name="redirectForm" action="/cgi-bin/live_update.asp" target="hidden_frame">
<input type="hidden" name="flag" value="">
<input type="hidden" name="prev_page" value="">
<input type="hidden" name="live_do_upgrade_flag" value="0">
<INPUT TYPE="HIDDEN" NAME="value_NONE" VALUE="NONE">
<input type="hidden" name="DOWNLOAD_HEADER_TYPE" value="1">
</form>
<FORM ENCTYPE="multipart/form-data" METHOD="POST" name="uiPostUpdateForm" action="/cgi-bin/Advanced_FirmwareUpgrade_Content.asp" target="hidden_frame">
<INPUT TYPE="HIDDEN" NAME="postflag" VALUE="1">
<INPUT TYPE="HIDDEN" NAME="HTML_HEADER_TYPE" VALUE="2">
<INPUT TYPE="HIDDEN" NAME="value_NONE" VALUE="NONE">
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<!--===================================Beginning of Main Content===========================================-->
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
	<td align="left" valign="top" >
	<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
	<tbody>
	<tr>
		<td bgcolor="#4D595D" valign="top" >
			<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6_adv","s")%> - <%tcWebApi_get("String_Entry","menu5_6_3","s")%></div>
			<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
			<div class="formfontdesc"><strong>
		  	<%tcWebApi_get("String_Entry","FW_note","s")%></strong>
				<ol>
					<li><%tcWebApi_get("String_Entry","FW_n0","s")%></li>
					<li><%tcWebApi_get("String_Entry","FW_n1","s")%></li>
					<li><%tcWebApi_get("String_Entry","FW_n2","s")%></li>
					<li><%tcWebApi_get("String_Entry","FW_desc0","s")%></li>
				</ol>
			</div>
			<br>

			<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
			<tr>
				<th><%tcWebApi_get("String_Entry","FW_item1","s")%></th>
				<td><%tcWebApi_get("String_Entry","Web_Title2","s")%></td>
			</tr>
			<tr>
				<th><% tcWebApi_Get("String_Entry", "adsl_fw_ver_in", "s") %></th>
				<td><%If tcWebApi_get("Info_Adsl","fwVer","h") <> "" then tcWebApi_staticGet("Info_Adsl","fwVer","s") end if%></td>
			</tr>
			<tr>
				<th><%tcWebApi_get("String_Entry","FW_item2","s")%></th>
				<td><%If tcWebApi_get("DeviceInfo","FwVer","h") <> "" Then tcWebApi_staticGet("DeviceInfo","FwVer","s") end if%>
						<input type="button" id="update" name="update" class="button_gen" onclick="detect_update();" value="<%tcWebApi_get("String_Entry","liveupdate","s")%>" />
						<div id="check_states">
							<span id="update_states"></span>
							<img id="update_scan" style="display:none;" src="/images/InternetScan.gif" />
						</div>
				</td>
			</tr>
			<tr>
				<th><%tcWebApi_get("String_Entry","FW_item5","s")%></th>
				<td><input type="file" name="tools_FW_UploadFile" class="input" style="color:#FFCC00;">
						<br>
				</td>
			</tr>
			<tr align="center">
				<td colspan="2"><input type="button" name="FW_apply" class="button_gen" onclick="uiDoUpdate();" value="<%tcWebApi_get("String_Entry","CTL_upload","s")%>" /></td>
			</tr>
			</table>
		</td>
	</tr>
	</tbody>
	</table>
</td>
</tr>
</table>
<!--===================================Ending of Main Content===========================================-->
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</form>

<form method="post" name="start_update" action="/cgi-bin/live_update.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_FirmwareUpgrade_Content.asp">
<input type="hidden" name="next_page" value="Advanced_FirmwareUpgrade_Content.asp">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="live_update_flag" value="0">
<input type="hidden" name="live_upgrade_flag" value="0">
<input type="hidden" name="value_NONE" value="NONE">
<input type="hidden" name="DOWNLOAD_HEADER_TYPE" value="1">
</form>

</body>
<!--Advanced_FirmwareUpgrade_Content.asp-->
</html>

