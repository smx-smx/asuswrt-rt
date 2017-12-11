<%
If Request_Form("flag") = "1" Then
	If Request_Form("usb_support_Flag") = "1" Then
		TCWebApi_set("BackupLog_Entry", "log_bkp_nonhide","log_bkp_nonhide")
	End if	
	TCWebApi_set("WebHistory_Entry", "wh_enable","wh_enable")
	TCWebApi_set("WebHistory_Entry", "wh_bkp_enable","wh_bkp_enable_x")
	TCWebApi_set("WebHistory_Entry", "wh_bkp_path","wh_bkp_path_x")
	TCWebApi_set("WebHistory_Entry", "wh_clear","wh_clear")
	TCWebApi_set("WebHistory_Entry", "wh_bkp_period","wh_bkp_period_x")
	tcWebApi_commit("WebHistory_Entry")
End if
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="images/favicon.png">
<link rel="icon" href="images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <% tcWebApi_staticGet("SysInfo_Entry","Adaptive_History","s") %></title>
<link rel="stylesheet" type="text/css" href="/index_style.css"> 
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<link rel="stylesheet" type="text/css" href="/css/element.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/js/jquery.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<style>
.transition_style{
	-webkit-transition: all 0.2s ease-in-out;
	-moz-transition: all 0.2s ease-in-out;
	-o-transition: all 0.2s ease-in-out;
	transition: all 0.2s ease-in-out;
}
</style>
<script>
var $j = jQuery.noConflict();
<% disk_pool_mapping_info(); %>
<% available_disk_names_and_sizes(); %>
<% check_log_path("WebHistory"); %>

if(usb_support != -1){
	var wh_bkp_path_orig = '<% tcWebApi_get("WebHistory_Entry", "wh_bkp_path","s") %>';
	var wh_bkp_path_fail_flag = '<% tcWebApi_get("Vram_Entry", "wh_bkp_path_fail","s") %>';
	
	var usb_device_index = disk_interface_names();
	var usb_mnt_point = pool_names();
	var usb_device_info = foreign_disk_model_info();	
}

function initial(){
	show_menu();
	if(document.form.wh_enable.value == 1){
		document.getElementById("log_field").style.display = "";
		if(usb_support == -1){
			inputCtrl(document.form.wh_bkp_enable_x[0], 0);
			inputCtrl(document.form.wh_bkp_enable_x[1], 0);
			inputCtrl(document.form.wh_bkp_period_x, 0);
			document.getElementById("submitBtn").style.display = "none";
		}
		else{
			usb_disk_options();
			inputCtrl(document.form.wh_bkp_enable_x[0], 1);
			inputCtrl(document.form.wh_bkp_enable_x[1], 1);
			change_wh_bkp_enable('<%tcWebApi_get("WebHistory_Entry","wh_bkp_enable","s")%>');
			document.getElementById("submitBtn").style.display = "";
		}
		corrected_timezone(DAYLIGHT_orig, TZ_orig);
	}
	else{
		document.getElementById("log_field").style.display = "none";
		inputCtrl(document.form.wh_bkp_enable_x[0], 0);
		inputCtrl(document.form.wh_bkp_enable_x[1], 0);
		change_wh_bkp_enable('<%tcWebApi_get("WebHistory_Entry","wh_bkp_enable","s")%>');
		document.getElementById("submitBtn").style.display = "none";
	}		
}

function usb_disk_options(){
	free_options(document.form.wh_bkp_path_x);
	for(var i=0; i<usb_device_index.length; i++){
		if(usb_device_index[i]=="usb"){
			if(wh_bkp_path_orig == usb_mnt_point[i]){
				add_option(document.form.wh_bkp_path_x, usb_device_info[i], usb_mnt_point[i], 1);
			}	
			else{
				add_option(document.form.wh_bkp_path_x, usb_device_info[i], usb_mnt_point[i], 0);
			}
		}
	}
	
	if(wh_bkp_path_fail_flag == 1){	//show wh_bkp_path not exist hint
		document.getElementById("bkup_path_fail").style.display = "";
	}
	else{
		document.getElementById("bkup_path_fail").style.display = "none";
	}
}
function redirect(){
	document.location.href = "/Main_WebHistory_Content.asp";
}

function back_syslog(){
	var cfg = '/web_history.txt';
	var code = 'location.assign(\"' + cfg + '\")';
	eval(code);
}

function change_wh_bkp_enable(flag){
	if(flag == 1){
		document.getElementById("wh_bkp_path_div").style.display = "";
		document.getElementById("wh_clear_div").style.display = "";
		document.getElementById("log_bkp_nonhide_div").style.display = "";
		inputCtrl(document.form.wh_bkp_period_x, 1);
	}
	else{
		document.getElementById("wh_bkp_path_div").style.display = "none";
		document.getElementById("wh_clear_div").style.display = "none";
		document.getElementById("log_bkp_nonhide_div").style.display = "none";
		inputCtrl(document.form.wh_bkp_period_x, 0);
	}	
}

function updateUSBStatus(){	
	if((rc_support.search("usbX") == -1 || rc_support.search("usbX1") >= 0) && usb_path1.search("storage") == -1){	//single USB port		
		document.getElementById("be_lack_storage").style.display = "";
		document.form.wh_bkp_enable_x[0].disabled = true;
		document.form.wh_bkp_enable_x[1].checked = true;
	}
	else if(rc_support.search("usbX2") >= 0 && (usb_path1.search("storage") == -1 && usb_path2.search("storage") == -1)){	//two USBport
		document.getElementById("be_lack_storage").style.display = "";
		document.form.wh_bkp_enable_x[0].disabled = true;
		document.form.wh_bkp_enable_x[1].checked = true;
	} 
	else{		
		document.getElementById("be_lack_storage").style.display = "none";
		document.form.wh_bkp_enable_x[0].disabled = false;
	}					
}

function applyRule(){
	if(document.form.wh_clear_x.checked == true)
		document.form.wh_clear.value = 1;
	else
		document.form.wh_clear.value = 0;

	if(usb_support != -1){
		if(document.form.log_bkp_nonhide_x.checked == true)
			document.form.log_bkp_nonhide.value = 1;
		else
			document.form.log_bkp_nonhide.value = 0;
		document.form.usb_support_Flag.value = 1;
	}

	document.form.flag.value = 1;
	showLoading(3);
	setTimeout("redirect();", 3000);
	document.form.submit();
}

function clear_bkup_path_fail(){
	if(document.getElementById("bkup_path_fail").style.display == ""){
		document.getElementById("bkup_path_fail").style.display = "none";
	}
}
</script>
</head>
<body onload="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<div id="agreement_panel" class="panel_folder" style="margin-top: -100px;"></div>
<div id="hiddenMask" class="popup_bg" style="z-index:999;">
	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center"></table>
	<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
</div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="/Main_WebHistory_Content.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="/Main_WebHistory_Content.asp">
<input type="hidden" name="next_page" value="/Main_WebHistory_Content.asp">
<input type="hidden" name="flag" value="0">
<input type="hidden" name="usb_support_Flag" value="0">
<input type="hidden" name="wh_enable" value="<%tcWebApi_get("WebHistory_Entry","wh_enable","s")%>">
<input type="hidden" name="wh_clear" value="<%tcWebApi_get("WebHistory_Entry","wh_clear","s")%>">
<input type="hidden" name="log_bkp_nonhide" value="<%tcWebApi_get("BackupLog_Entry","log_bkp_nonhide","s")%>">
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
						<table width="760px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3"  class="FormTitle" id="FormTitle">		
							<tr>
								<td bgcolor="#4D595D" colspan="3" valign="top">
									<div>&nbsp;</div>
									<div id="content_title" class="formfonttitle"><% tcWebApi_Get("String_Entry", "Menu_TrafficManager", "s") %> - <%tcWebApi_get("String_Entry","Adaptive_History","s")%></div>
									<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
									<div class="formfontdesc">
										<%tcWebApi_get("String_Entry","Adaptive_History_desc","s")%>
									</div>
									<div style="margin:5px">
										<table style="margin-left:0px;" width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20,6);">Enable Web History</a></th>
												<td>
												<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="wh_enable"></div>
															<script type="text/javascript">
																$j('#wh_enable').iphoneSwitch('<%tcWebApi_get("WebHistory_Entry","wh_enable","s")%>',
																	function(){
																		document.form.flag.value = 1;
																		document.form.wh_enable.value = 1;
																		showLoading(3);
																		setTimeout("redirect();", 3000);
																		document.form.submit();
																	},
																	function(){
																		document.form.flag.value = 1;
																		document.form.wh_enable.value = 0;
																		showLoading(3);
																		setTimeout("redirect();", 3000);
																		document.form.submit();
																	}
																);
															</script>
												<div style="margin-left:15px;"><br><br><span id="timezone_hint" onclick="location.href='Advanced_System_Content.asp?af=time_zone_select'" style="color:#FFCC00;text-decoration:underline;cursor:pointer;display:none;"></span></div>	
												</td>
											</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20,7);">Auto Backup in USB Disk</a></th>	<!-- untranslated -->
												<td>
													<input type="radio" name="wh_bkp_enable_x" class="input" value="1" onclick="change_wh_bkp_enable(1);" <% if tcWebApi_get("WebHistory_Entry","wh_bkp_enable","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="wh_bkp_enable_x" class="input" value="0" onclick="change_wh_bkp_enable(0);" <% if tcWebApi_get("WebHistory_Entry","wh_bkp_enable","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
													<span id="be_lack_storage" style="display:none;color:#FC0">* No USB disk plug-in.</span>													
													<br>
													<div id="wh_bkp_path_div">
														USB Disk :&nbsp;<select name="wh_bkp_path_x" class="input_option" onchange="clear_bkup_path_fail();"></select>														
														<span id="bkup_path_fail" style="display:none;color:#FC0"><br>* Current auto backup path is not exist. Make sure the USB disk is plugged in and setup again.</span>	<!-- untranslated -->
													</div>													
													<div id="wh_clear_div" style="display:none;"><input type="checkbox" name="wh_clear_x" id="wh_clear_x" value="1" <% if tcWebApi_get("WebHistory_Entry","wh_clear","h") = "1" then asp_Write("checked") end if %>> <label for="wh_clear_x">Clear Web History After Backup<label></div> <!-- untranslated -->
													<div id="log_bkp_nonhide_div" style="display:none;"><input type="checkbox" name="log_bkp_nonhide_x" id="log_bkp_nonhide_x" value="1" <% if tcWebApi_get("BackupLog_Entry","log_bkp_nonhide","h") = "1" then asp_Write("checked") end if %>> <label for="log_bkp_nonhide_x">Non-hidden backup files<label></div> <!-- untranslated -->
												</td>	
											</tr>
											<tr>
												<th>Backup Period</th>	<!-- untranslated -->
												<td>
													<select name="wh_bkp_period_x" class="input_option">
														<option value="3600" <%if tcWebApi_get("WebHistory_Entry","wh_bkp_period","h") = "3600" then asp_Write("selected") end if%> >1</option>
														<option value="21600" <%if tcWebApi_get("WebHistory_Entry","wh_bkp_period","h") = "21600" then asp_Write("selected") end if%> >6</option>
														<option value="43200" <%if tcWebApi_get("WebHistory_Entry","wh_bkp_period","h") = "43200" then asp_Write("selected") end if%> >12</option>
														<option value="86400" <%if tcWebApi_get("WebHistory_Entry","wh_bkp_period","h") = "86400" then asp_Write("selected") end if%> >24</option>
													</select>&nbsp;<%tcWebApi_get("String_Entry","Hour","s")%>
												</td>	
											</tr>
										</table>
									</div>
									<div id="submitBtn" class="apply_gen" style="display:none;">
										<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
									</div>
									<div id="log_field" style="margin:10px 5px">
										<div>
											<table width="740px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3" class="FormTitle" id="FormTitle">
											<tr bgcolor="#4D595D">
												<td valign="top">
													<div>&nbsp;</div>
													<div style="margin-top:8px">
														<textarea style="width:99%; font-family:'Courier New', Courier, mono; font-size:12px;background:#475A5F;color:#FFFFFF;" cols="63" rows="25" wrap="off" readonly><% nvram_dump("web_history.log","") %></textarea>
													</div>													
												</td>
											</tr>
											</table>
										</div>										
										<div style="margin-top:-10px;">
										<table class="apply_gen" >
											<tr class="apply_gen" valign="top">
												<td width="50%" align="right" >
													<input type="button" onClick="location.href=location.href" value="<%tcWebApi_get("String_Entry","CTL_refresh","s")%>" class="button_gen">
												</td>
												<td width="50%" align="left">
													<input type="button" onClick="back_syslog();" value="<%tcWebApi_get("String_Entry","CTL_onlysave","s")%>" class="button_gen">
												</td>												
											</tr>
										</table>
										</div>
									</div>
								</td>
							</tr>
						</table>
					</td>  
				</tr>
			</table>
			<!--===================================End of Main Content===========================================-->
		</td>		
	</tr>
</table>
<div id="footer"></div>
</body>
</html>
