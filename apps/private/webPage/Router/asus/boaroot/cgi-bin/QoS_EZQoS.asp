<% If Request_Form("apply_flag") = "1" then
	tcWebApi_set("QoS_Entry0","qos_enable","qos_enable")
	tcWebApi_set("QoS_Entry0","qos_type", "qos_type")
	tcWebApi_set("QoS_Entry0","qos_obw", "qos_obw")
	tcWebApi_set("QoS_Entry0","qos_ibw", "qos_ibw")
	tcWebApi_CommitWithoutSave("Firewall")
	tcWebApi_commit("QoS")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--QoS_EZQoS.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <% tcWebApi_get("String_Entry","Web_Title2","s") %> <% tcWebApi_get("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","EZQoS","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script type="text/javascript" src="/js/httpApi.js"></script>
<script type="text/javascript" src="/js/asus_eula.js"></script>
<script>
var $j = jQuery.noConflict();
</script>
<style type="text/css">
.qos_table{
	width:740px;
	padding:10px; 
	margin-top:-17px;
	position:relative;
	background-color:#4d595d;
	height: 820px;
	-webkit-border-bottom-right-radius: 3px;
	-webkit-border-bottom-left-radius: 3px;
	-moz-border-radius-bottomright: 3px;
	-moz-border-radius-bottomleft: 3px;
	border-bottom-right-radius: 3px;
	border-bottom-left-radius: 3px;
}
</style>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';

var $j = jQuery.noConflict();

<% first_wanlink(); %>
<% secondary_wanlink(); %>
var first_wanlink_status = first_wanlink_status();
var secondary_wanlink_status = secondary_wanlink_status();

var wans_dualwan_orig = '<%tcWebApi_Get("Dualwan_Entry", "wans_dualwan", "s")%>';
var wans_primary_orig = wans_dualwan_orig.split(" ")[0];
var wans_second_orig = wans_dualwan_orig.split(" ")[1];

var qos_rulelist_array = "<% tcWebApi_get("QoS_Entry0","qos_rulelist","s") %>";
//WebCurSet_dev_pvc : 0:ATM / 8:PTM / 10:Ethernet / 11:USB / 12:LAN
var WebCurSet_dev_pvc = "<% tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","s") %>";

var overlib_str0 = new Array();	//Viz add 2011.06 for record longer qos rule desc
var overlib_str = new Array();	//Viz add 2011.06 for record longer portrange value

var dsl_DataRateDown = parseInt("<% tcWebApi_get("Info_Adsl","DataRateDown","s") %>".replace("kbps",""))/1024;
var dsl_DataRateUp = parseInt("<% tcWebApi_get("Info_Adsl","DataRateUp","s") %>".replace("kbps",""))/1024;
var upload = "<% tcWebApi_get("QoS_Entry0","qos_obw","s") %>";
var download = "<% tcWebApi_get("QoS_Entry0","qos_ibw","s") %>";	

var GN_with_BandwidthLimeter = false;
var gn_array_2g_length = gn_array_2g.length;
for(var i=0;i<gn_array_2g_length;i++){
	if(gn_array_2g[i][18] == "1"){	//GN with Bandwidth Limiter 
		GN_with_BandwidthLimeter = true;
	}	
}
if(wl_info.band5g_support){
	var gn_array_5g_length = gn_array_5g.length;	
	for(var j=0;j<gn_array_5g_length;j++){
		if(gn_array_5g[j][18] == "1"){	//GN with Bandwidth Limiter 
			GN_with_BandwidthLimeter = true;
		}	
	}
}

function initial(){
	show_menu();

	if(downsize_support != -1)
		$("guest_image").parentNode.style.display = "none";

	if(document.form.qos_enable.value==1){
		if(document.form.qos_type.value != 2){
			document.form.obw.parentNode.parentNode.style.display = "";
			document.form.ibw.parentNode.parentNode.style.display = "";
		}
		else{
			document.form.obw.parentNode.parentNode.style.display = "none";
			document.form.ibw.parentNode.parentNode.style.display = "none";
		}	
		
		document.getElementById('qos_type_tr').style.display = "";
		/*
		if(bwdpi_support != -1){
			document.getElementById('int_type').style.display = "";
			document.getElementById('int_type_link').style.display = "";
			change_qos_type(document.form.qos_type_orig.value);
		}
		else
			show_settings("NonAdaptive");*/
			
	}else{
		document.form.obw.parentNode.parentNode.style.display = "none";
		document.form.ibw.parentNode.parentNode.style.display = "none";
		document.getElementById('qos_type_tr').style.display = "none";
		/*
		if(bwdpi_support){			
			show_settings("NonAdaptive");
		}*/
	}
	
	/*
	if(bwdpi_support){
		document.getElementById('content_title').innerHTML = "<% tcWebApi_Get("String_Entry", "menu5_3_2","s")%> - <% tcWebApi_Get("String_Entry", "Adaptive_QoS_Conf","s")%>";
		if(document.form.qos_enable.value == 1){
			if(document.form.qos_type.value == 0){		//Traditional Type
				document.getElementById("settingSelection").length = 1;
				add_option(document.getElementById("settingSelection"), '<%tcWebApi_Get("String_Entry", "qos_user_rules","s")%>', 3, 0);
				add_option(document.getElementById("settingSelection"), '<%tcWebApi_Get("String_Entry", "qos_user_prio","s")%>', 4, 0);
			}
			else if(document.form.qos_type.value == 2){		//Bandwidth Limiter
				add_option(document.getElementById("settingSelection"), "Bandwidth Limiter", 5, 0);
			}
			else{		//Adaptive Type or else
				document.getElementById('settingSelection').style.display = "none";	
			}
		}
		else{		// hide select option if qos disable
			document.getElementById('settingSelection').style.display = "none";	
		}
	}
	else{	*/
		document.getElementById('content_title').innerHTML = "<% tcWebApi_Get("String_Entry", "Menu_TrafficManager", "s") %> - QoS";		
		if(document.form.qos_type.value == 2){		//Bandwidth Limiter
			add_option(document.getElementById("settingSelection"), "<%tcWebApi_get("String_Entry","Bandwidth_Limiter","s")%>", 5, 0);
		}
		else{		//Traditional Type						
			add_option(document.getElementById("settingSelection"), '<% tcWebApi_Get("String_Entry", "qos_user_prio", "s") %>', 4, 0);
			add_option(document.getElementById("settingSelection"), '<% tcWebApi_Get("String_Entry", "qos_user_rules", "s") %>', 3, 0);
		}		
	//}
	
	init_changeScale();
	setTimeout("update_FAQ();", 300);
}

function update_FAQ(){
    if(document.getElementById("connect_status").className == "connectstatuson"){
        faqURL("faq", "https://www.asus.com", "/support/FAQ/", "1008718");
    }
}

function init_changeScale(){
	
	//if((WebCurSet_dev_pvc == "0" || WebCurSet_dev_pvc == "8") && ((!upload || upload == "0") && (!download || download == "0"))){
	if(((wans_primary_orig == "dsl" && first_wanlink_status == 1) || (wans_second_orig == "dsl" && secondary_wanlink_status == 1)) && ((!upload || upload == "0") && (!download || download == "0"))){
		document.form.obw.value = isNaN(dsl_DataRateUp)? 0:dsl_DataRateUp.toFixed(1);
		document.form.ibw.value = isNaN(dsl_DataRateDown)? 0:dsl_DataRateDown.toFixed(1);
	}	
	else{
		if(!upload)	upload = 0;
		
		document.form.obw.value = upload/1024;
		
		if(!download) download = 0;
		
		document.form.ibw.value = download/1024;
	}
}

function changeScale(_obj_String){
	if($(_obj_String+"_scale").value == "Mb/s")
		$(_obj_String).value = Math.round(($(_obj_String).value/1024)*100)/100;
	else
		$(_obj_String).value = Math.round($(_obj_String).value*1024);
}

function switchPage(page){
	if(page == "1")	
		location.href = "/cgi-bin/QoS_EZQoS.asp";
	else if(page == "2")	
		location.href = "/cgi-bin/AdaptiveQoS_Adaptive.asp";	//remove 2015.07
	else if(page == "3")	
		location.href = "/cgi-bin/Advanced_QOSUserRules_Content.asp";
	else if(page == "4")	
		location.href = "/cgi-bin/Advanced_QOSUserPrio_Content.asp";
	else if(page == "5")	
		location.href = "/cgi-bin/Bandwidth_Limiter.asp";
	else
		return false;
}

function submitQoS(){	
	if(document.form.qos_enable.value == 0 && document.form.qos_enable_orig.value == 0){		
		return false;
	}

	if(document.form.qos_enable.value == 1){
		if(document.form.qos_type.value != 2){
			if(document.form.obw.value.length == 0){	//To check field is empty
				alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
				document.form.obw.focus();
				document.form.obw.select();
				return;
			}
			else if(document.form.obw.value == 0){		// To check field is 0
				alert("Upload Bandwidth can not be 0");
				document.form.obw.focus();
				document.form.obw.select();
				return;			
			}
			else if(document.form.obw.value.split(".").length > 2){		//To check more than two point symbol
				alert("The format of field of upload bandwidth is invalid");
				document.form.obw.focus();
				document.form.obw.select();
				return;	
			}
			else if(!validator.rangeFloat(document.form.obw, 0, 9999999999, "")){
				return;
			}
		
			if(document.form.ibw.value.length == 0){
				alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
				document.form.ibw.focus();
				document.form.ibw.select();
				return;
			}
			else if(document.form.ibw.value == 0){
				alert("Download Bandwidth can not be 0");
				document.form.ibw.focus();
				document.form.ibw.select();
				return;	
			}
			else if(document.form.ibw.value.split(".").length > 2){
				alert("The format of field of download bandwidth is invalid");
				document.form.ibw.focus();
				document.form.ibw.select();
				return;	
			}
			else if(!validator.rangeFloat(document.form.obw, 0, 9999999999, "")){
				return;
			}	
  		
			document.form.qos_obw.disabled = false;
			document.form.qos_ibw.disabled = false;
			document.form.qos_obw.value = document.form.obw.value*1024;
			document.form.qos_ibw.value = document.form.ibw.value*1024;
			document.form.apply_flag.value = "1";

			document.form.action = "/cgi-bin/QoS_EZQoS.asp";			
			showLoading(5);
			setTimeout('location = "'+ location.pathname +'";', 5000);
			document.form.submit();	
		}
		else{
			location.href = "/cgi-bin/Bandwidth_Limiter.asp";	
		}
	}
	else{
		document.form.apply_flag.value = "1";
		document.form.action = "/cgi-bin/QoS_EZQoS.asp";			
		showLoading(5);
		setTimeout('location = "'+ location.pathname +'";', 5000);
		document.form.submit();	
	}
}

function change_qos_type(value){
	if(value == 0){		//Traditional QoS
		//document.getElementById('int_type').checked = false;
		document.getElementById('trad_type').checked = true;
		document.getElementById('bw_limit_type').checked = false;
		document.form.obw.parentNode.parentNode.style.display = "";
		document.form.ibw.parentNode.parentNode.style.display = "";
		if(document.form.qos_type_orig.value == 0 && document.form.qos_enable_orig.value != 0){
			document.form.action_script.value = "restart_qos;restart_firewall";
		}	
		else{
			document.form.action_script.value = "reboot";
			document.form.next_page.value = "Advanced_QOSUserRules_Content.asp";
		}
		//show_settings("NonAdaptive");
	}	
	else{		//2: Bandwidth Limiter
		//document.getElementById('int_type').checked = false;
		document.getElementById('trad_type').checked = false;
		document.getElementById('bw_limit_type').checked = true;
		document.form.obw.parentNode.parentNode.style.display = "none";
		document.form.ibw.parentNode.parentNode.style.display = "none";
		if(document.form.qos_type_orig.value == 2 && document.form.qos_enable_orig.value != 0)
			document.form.action_script.value = "restart_qos;restart_firewall";
		else{
			document.form.action_script.value = "reboot";
			document.form.next_page.value = "Bandwidth_Limiter.asp";
		}
		//show_settings("NonAdaptive");
	}
	document.form.qos_type.value = value;
	
	if(value != 2 && GN_with_BandwidthLimeter){
		alert("Guest Network > Bandwidth Limiter will be Disabled.");		/* Untranslated */
	}
}
</script>
</head>

<body onload="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_get("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="current_page" value="QoS_EZQoS.asp">
<input type="hidden" name="next_page" value="QoS_EZQoS.asp">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_qos">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="flag" value="">
<input type="hidden" name="qos_enable" value="<%if tcWebApi_get("QoS_Entry0","qos_enable","h") = "1" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="qos_enable_orig" value="<%if tcWebApi_get("QoS_Entry0","qos_enable","h") = "1" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="qos_type" value="<% tcWebApi_get("QoS_Entry0","qos_type","s") %>">
<input type="hidden" name="qos_type_orig" value="<% tcWebApi_get("QoS_Entry0","qos_type","s") %>">
<input type="hidden" name="qos_obw" value="<% tcWebApi_get("QoS_Entry0","qos_obw","s") %>" disabled>
<input type="hidden" name="qos_ibw" value="<% tcWebApi_get("QoS_Entry0","qos_ibw","s") %>" disabled>
<table class="content" align="center" cellpadding="0" cellspacing="0">
  <tr>
	<td width="17">&nbsp;</td>
	
	<!--=====Beginning of Main Menu=====-->
	<td valign="top" width="202">
	  <div id="mainMenu"></div>
	  <div id="subMenu"></div>
	</td>
	
    <td valign="top">
			<div id="tabMenu" class="submenuBlock"></div>
			<br>
		<!--===================================Beginning of Main Content===========================================-->
		<div class="qos_table" id="qos_table">
		<table>
  		<tr>
    			<td bgcolor="#4D595D" valign="top">
    				<table>
        			<tr>
						<td>
							<table width="100%">
								<tr>
									<td class="formfonttitle" align="left">								
										<div id="content_title"></div>
									</td>
									<td align="right" >
										<div>
											<select id="settingSelection" onchange="switchPage(this.options[this.selectedIndex].value)" class="input_option">
												<option value="1" selected><%tcWebApi_get("String_Entry","Adaptive_QoS_Conf","s")%></option>
											</select>
										</div>
									</td>												
								</tr>
							</table>	
						</td>
        			</tr>
        			<tr>
          				<td height="5"><img src="/images/New_ui/export/line_export.png" /></td>
        			</tr>
        			<tr>
          				<td height="30" align="left" valign="middle" >
										<div>
											<table width="650px">
												<tr>
													<td width="130px">
														<img id="guest_image" src="/images/New_ui/QoS.png">
													</td>
													<td style="font-style: italic;font-size: 14px;">
														<div id="function_desc" class="formfontdesc" style="line-height:20px;">
															<%tcWebApi_get("String_Entry","EzQoS_desc","s")%>
														<ul>
															<!--li><%tcWebApi_get("String_Entry","EzQoS_desc_Adaptive","s")%></li-->
															<li><%tcWebApi_get("String_Entry","EzQoS_desc_Traditional","s")%></li>
															<li><span style="font-size:14px;font-weight:bolder"><%tcWebApi_get("String_Entry","Bandwidth_Limiter_hint","s")%></li>
														</ul>
														<%tcWebApi_get("String_Entry","EzQoS_desc_note","s")%>
														</div>
														<div class="formfontdesc">
															<a id="faq" href="https://www.asus.com/support/FAQ/1008718" target="_blank" style="text-decoration:underline;">QoS FAQ</a>
														</div>
													</td>
												</tr>
											</table>
										</div>
          				</td>
        			</tr>
							
							<tr>
								<td>
									<table style="margin-left:3px; margin-top:15px;" width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
										<tr>
										<th><%tcWebApi_get("String_Entry","EzQoS_smart_enable","s")%></th>
											<td colspan="2">
												<div class="left" style="width:94px; float:left; cursor:pointer;" id="radio_qos_enable"></div>
												<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
												<script type="text/javascript">
													$j('#radio_qos_enable').iphoneSwitch('<%tcWebApi_get("QoS_Entry0","qos_enable","s")%>', 
														 function() {
															document.form.qos_enable.value = "1";
															if(document.form.qos_enable_orig.value != 1){
																if(document.getElementById('bw_limit_type').checked)		//Bandwidth Limiter
																	document.form.next_page.value = "Bandwidth_Limiter.asp";
																else
																	document.form.next_page.value = "Advanced_QOSUserRules_Content.asp";
															}
															if(document.form.qos_type.value != 2){
																document.form.obw.parentNode.parentNode.style.display = "";
																document.form.ibw.parentNode.parentNode.style.display = "";
															}
															else{
																document.form.obw.parentNode.parentNode.style.display = "none";
																document.form.ibw.parentNode.parentNode.style.display = "none";
															}
															
															document.getElementById('qos_type_tr').style.display = "";
															
														 },
														 function() {
															document.form.qos_enable.value = "0";
															document.form.obw.parentNode.parentNode.style.display = "none";
															document.form.ibw.parentNode.parentNode.style.display = "none";
															document.getElementById('qos_type_tr').style.display = "none";
															
															if(GN_with_BandwidthLimeter){
																alert("Guest Network > Bandwidth Limiter will be Disabled.");		/* Untranslated */
															}
														 },
														 {
															switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
														 }
													);
												</script>			
												</div>	
											</td>
										</tr>										
										<tr id="qos_type_tr" style="display:none">
											<th><% tcWebApi_Get("String_Entry", "QoS_Type","s")%></th>
											<td colspan="2">
												<!--input id="int_type" value="1" onClick="change_qos_type(this.value);" style="display:none;" type="radio" ><a id="int_type_link" class="hintstyle" style="display:none;" href="javascript:void(0);" onClick="openHint(20, 6);"><label for="int_type"><%tcWebApi_Get("String_Entry", "Adaptive_QoS","s")%></label></a-->
												<input id="trad_type" value="0" onClick="change_qos_type(this.value);" type="radio" <%If tcWebApi_get("QoS_Entry0","qos_type","h") = "0" then asp_Write("checked") end if%>><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 7);"><label for="trad_type"><% tcWebApi_Get("String_Entry", "EzQoS_type_traditional","s")%></label></a>
												<input id="bw_limit_type" value="2"  onClick="change_qos_type(this.value);" type="radio" <%If tcWebApi_get("QoS_Entry0","qos_type","h") = "2" then asp_Write("checked") end if%>><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 8)"><label for="bw_limit_type"><% tcWebApi_Get("String_Entry", "Bandwidth_Limiter","s")%></label></a>
											</td>
										</tr>
										<tr id="upload_tr">
											<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 2);"><%tcWebApi_get("String_Entry","upload_bandwidth","s")%></a></th>
											<td>
												<input type="text" maxlength="10" id="obw" name="obw" onKeyPress="return isNumberFloat(this,event);" class="input_15_table" value="">
												<label style="margin-left:5px;">Mb/s</label>
											</td>
											<td rowspan="2" style="width:250px;">
												<div>
                                                                                                        <ul style="padding:0 10px;margin:5px 0;">
														<li><%tcWebApi_get("String_Entry","EzQoS_bandwidth_note1","s")%></li>
													</ul>
												</div>
											</td>
										</tr>									
										<tr id="download_tr">
											<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 2);"><%tcWebApi_get("String_Entry","download_bandwidth","s")%></a></th>
											<td>
												<input type="text" maxlength="10" id="ibw" name="ibw" onKeyPress="return isNumberFloat(this,event);" class="input_15_table" value="">
												<label style="margin-left:5px;">Mb/s</label>
											</td>
										</tr>										
									</table>
								</td>
				</tr>	

        			<tr>
          				<td height="50" >
          					<div style=" *width:136px;margin-left:300px;" class="titlebtn" align="center" onClick="submitQoS();">
								<input type="hidden" name="apply_flag" value="0">
								<span><%tcWebApi_get("String_Entry","CTL_onlysave","s")%></span>
							</div>
          				</td>
        			</tr>        			
      			</table>
      		</td>  
      	</tr>
		</table>
		</div>
		<!--===================================End of Main Content===========================================-->
		</td>
		
	</tr>
</table>
</form>
<div id="footer"></div>
</body>
</html>
<!--QoS_EZQoS.asp-->
