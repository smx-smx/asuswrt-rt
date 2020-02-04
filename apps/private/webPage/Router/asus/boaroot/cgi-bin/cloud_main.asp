<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_Get("String_Entry", "AiCloud_Title","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css"> 
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script>

//if('<% tcWebApi_get("AiCloud_Entry", "start_aicloud", "s") %>' == '0')
//	location.href = "cloud__main.asp";

var $j = jQuery.noConflict();

<% login_state_hook() %>
<% wanlink() %>

//var wireless = [<% wl_auth_list(); %>];	// [[MAC, associated, authorized], ...]
//wan_route_x = '<% nvram_get("wan_route_x"); %>';
//wan_nat_x = '<% nvram_get("wan_nat_x"); %>';
//wan_proto = '<% nvram_get("wan_proto"); %>';

var cloud_sync = '<% tcWebApi_get("AiCloud_Entry", "cloud_sync", "s") %>';
var cloud_status;
var curState = '<% tcWebApi_get("AiCloud_Entry", "webdav_aidisk", "s") %>';
var ddns_hostname = '<% tcWebApi_get("Ddns_Entry", "MYHOST", "s") %>';
var https_port = '<% tcWebApi_get("AiCloud_Entry", "webdav_https_port", "s") %>';

function initial(){
	show_menu();

	switch(valid_is_wan_ip(wanlink_ipaddr())){
		/* private */
		case 0:			
			//- router mode
			var aicloud_url = "https://router.asus.com";

			if(https_port != 443){
				aicloud_url += ":" + https_port;
			}

			document.getElementById("accessMethod").innerHTML = "<%tcWebApi_Get("String_Entry", "AiCloud_enter", "s")%> : <a id=\"cloud_url\" style=\"font-weight: bolder;text-decoration: underline;\" href=\"" + aicloud_url + "\" target=\"_blank\">" + aicloud_url + "</a>";
			
			break;
		/* public */
		case 1:
			if('<% tcWebApi_get("Ddns_Entry", "Active", "s") %>' == '1' && ddns_hostname != ''){
				if(https_port == 443) // if the port number of https is 443, hide it
					document.getElementById("accessMethod").innerHTML = '<%tcWebApi_Get("String_Entry", "aicloud_disk_case11", "s")%> <a style="font-weight: bolder;text-decoration: underline;word-break:break-all;" href="https://'+ ddns_hostname + ':'+ https_port +' "target="_blank">https://'+ ddns_hostname +'</a><br />';
				else
					document.getElementById("accessMethod").innerHTML = '<%tcWebApi_Get("String_Entry", "aicloud_disk_case11", "s")%> <a style="font-weight: bolder;text-decoration: underline;word-break:break-all;" href="https://'+ ddns_hostname + ':'+ https_port +' "target="_blank">https://'+ ddns_hostname +':'+ https_port +'</a><br />';

				document.getElementById("accessMethod").innerHTML += '<%tcWebApi_Get("String_Entry", "aicloud_disk_case12", "s")%>';
			}
			else{
				document.getElementById("accessMethod").innerHTML = '<%tcWebApi_Get("String_Entry", "aicloud_disk_case21", "s")%><br />';
				document.getElementById("accessMethod").innerHTML += '<%tcWebApi_Get("String_Entry", "aicloud_disk_case22", "s")%>';
			}
			break;
	}

	if(!smart_sync_support){
		document.getElementById("ssLink").style.display = "none";
		document.getElementById("tr_smart_sync").style.display = "none";
	}

	if(rrsut_support == -1)
		document.getElementById("rrsLink").style.display = "none";
}

function valid_is_wan_ip(ip_obj){
  // test if WAN IP is a private IP.
  var A_class_start = inet_network("10.0.0.0");
  var A_class_end = inet_network("10.255.255.255");
  var B_class_start = inet_network("172.16.0.0");
  var B_class_end = inet_network("172.31.255.255");
  var C_class_start = inet_network("192.168.0.0");
  var C_class_end = inet_network("192.168.255.255");
  
  var ip_num = inet_network(ip_obj);
  if(ip_num > A_class_start && ip_num < A_class_end)
		return 0;
  else if(ip_num > B_class_start && ip_num < B_class_end)
		return 0;
  else if(ip_num > C_class_start && ip_num < C_class_end)
		return 0;
	else if(ip_num == 0)
		return 0;
  else
		return 1;
}

function inet_network(ip_str){
	if(!ip_str)
		return -1;
	
	var re = /^(\d+)\.(\d+)\.(\d+)\.(\d+)$/;
	if(re.test(ip_str)){
		var v1 = parseInt(RegExp.$1);
		var v2 = parseInt(RegExp.$2);
		var v3 = parseInt(RegExp.$3);
		var v4 = parseInt(RegExp.$4);
		
		if(v1 < 256 && v2 < 256 && v3 < 256 && v4 < 256)
			return v1*256*256*256+v2*256*256+v3*256+v4;
	}
	return -2;
}

</script>
</head>
<body onload="initial();" onunload="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>

	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
	</table>
	<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
	</div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>
<form method="post" name="form" action="cloud_apply.asp" target="hidden_frame">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="current_page" value="cloud_main.asp">
<input type="hidden" name="next_page" value="cloud_main.asp">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="enable_cloudsync" value="<% tcWebApi_get("AiCloud_Entry", "enable_cloudsync", "s") %>">
<input type="hidden" name="enable_webdav" value="<% tcWebApi_get("AiCloud_Entry", "enable_webdav", "s") %>">
<input type="hidden" name="webdav_aidisk" value="<% tcWebApi_get("AiCloud_Entry", "webdav_aidisk", "s") %>">
<input type="hidden" name="webdav_proxy" value="<% tcWebApi_get("AiCloud_Entry", "webdav_proxy", "s") %>">
<input type="hidden" name="editFlag" value="0">

<table border="0" align="center" cellpadding="0" cellspacing="0" class="content">
	<tr>
		<td valign="top" width="17">&nbsp;</td>
		<!--=====Beginning of Main Menu=====-->
		<td valign="top" width="202">
			<div id="mainMenu"></div>
			<div id="subMenu"></div>
		</td>
		<td valign="top">
			<div id="tabMenu" class="submenuBlock">
				<table border="0" cellspacing="0" cellpadding="0">
					<tbody>
					<tr>
						<td>
							<div class="tabclick"><span><%tcWebApi_Get("String_Entry", "AiCloud_Title","s")%></span></div>
						</td>
						<td>
							<a id="ssLink" href="cloud_sync.asp"><div class="tab" id="tab_smartsync"><span><% tcWebApi_Get("String_Entry","smart_sync","s") %></span></div></a>
						</td>
						<td>
							<a id="rrsLink" href="cloud_router_sync.asp"><div class="tab" id="tab_routersync"><span><% tcWebApi_Get("String_Entry","Server_Sync","s") %></span></div></a>
						</td>
						<td>
							<a href="cloud_settings.asp"><div class="tab" id="tab_setting"><span><% tcWebApi_Get("String_Entry","Settings","s") %></span></div></a>
						</td>
						<td>
							<a href="cloud_syslog.asp"><div class="tab" id="tab_syslog"><span><% tcWebApi_Get("String_Entry","Log","s") %></span></div></a>
						</td>
					</tr>
					</tbody>
				</table>
			</div>
<!--==============Beginning of hint content=============-->
			<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
			  <tr>
					<td align="left" valign="top">
					  <table width="100%" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
							<tbody>
							<tr>
							  <td bgcolor="#4D595D" valign="top">
									<div>&nbsp;</div>
									<div class="formfonttitle"><%tcWebApi_Get("String_Entry", "AiCloud_Title","s")%></div>
									<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
									<table width="100%" height="550px" style="border-collapse:collapse;">

									  <tr bgcolor="#444f53">
									    <td colspan="5" bgcolor="#444f53" class="cloud_main_radius">
												<div style="padding:10px;width:95%;font-style:italic;font-size:14px;">
                                                <%tcWebApi_Get("String_Entry", "AiCloud_maintext_note","s")%>
                                                    <br/><br/>
                                                    <table width="100%" >
                                                        <tr>
                                                            <td>
                                                                <ul style="margin: 0px;padding-left:15px;" >
                                                                    <li style="margin-top:-5px;">
                                                                    <div id="accessMethod"></div>
                                                                    </li>
                                                                    <li style="margin-top:-5px;">
                                                                     <%tcWebApi_Get("String_Entry", "FAQ_Find","s")%> <a style="font-weight: bolder;text-decoration: underline;" href="http://www.asus.com/search/results.aspx?SearchKey=AiCloud&SearchType=FAQ&IsSupport=True&Page=1" target="_blank">GO</a>
                                                                    </li>
                                                                </ul>
                                                            </td>
                                                            <td>
                                                                <a id="googleplay" href="https://play.google.com/store/apps/details?id=com.asustek.aicloud" target="_blank">
                                                                    <div style="width:172px;height:51px;background:url('images/cloudsync/googleplay.png') no-repeat;background-size:75%;"></div>
                                                                </a>
                                                                <a href="https://itunes.apple.com/us/app/aicloud-lite/id527118674" target="_blank">
                                                                    <div style="width:172px;height:51px;background:url('images/cloudsync/AppStore.png') no-repeat;background-size:75%;"></div>
                                                                </a>
                                                            </td>
                                                        </tr>
                                                    </table>
                                                </div>
                                                <div id="privateIP_notes" class="formfontdesc" style="display:none; color:#FFCC00; padding:10px;"></div>
										</td>
									  </tr>

									  <tr height="10px">
											<td colspan="3">
											</td>
									  </tr>

									  <tr bgcolor="#444f53" width="235px">
									    <td bgcolor="#444f53" class="cloud_main_radius_left" width="20%" height="50px">
												<div style="padding:10px;" align="center"><img src="/images/cloudsync/001.png">
													<div align="center" style="margin-top:10px;font-size: 18px;text-shadow: 1px 1px 0px black;"><%tcWebApi_Get("String_Entry", "Cloud_Disk", "s")%></div>
												</div>
											</td>

									    <td width="6px">
												<div align="center"><img src="/images/cloudsync/line.png"></div>
											</td>

									    <td width="1px"></td>

									    <td>
												<div style="padding:10px;width:95%;font-style:italic;font-size:14px;">
													<%tcWebApi_Get("String_Entry", "aicloud_disk_desc", "s")%><br />
													<div id="accessMethod"></div>
												</div>
											</td>

									    <td bgcolor="#444f53" class="cloud_main_radius_right" width="100px">
												<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_clouddisk_enable"></div>
												<div id="iphone_switch_container" class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
												<script type="text/javascript">
													$j('#radio_clouddisk_enable').iphoneSwitch('<% tcWebApi_get("AiCloud_Entry", "webdav_aidisk", "s") %>', 
														 function(){
															if(window.scrollTo)
																window.scrollTo(0,0);
															curState = 1;
															document.form.editFlag.value = 1;
															document.form.webdav_aidisk.value = 1;
															document.form.enable_webdav.value = 1;
															FormActions("cloud_apply.asp", "apply", "restart_webdav", "3");
															showLoading();
															document.form.submit();
														 },
														 function() {
															document.form.webdav_aidisk.value = 0;
															if(document.form.webdav_proxy.value == 0)
																document.form.enable_webdav.value = 0;
															document.form.editFlag.value = 1;
															FormActions("cloud_apply.asp", "apply", "restart_webdav", "3");
															showLoading();	
															document.form.submit();
														 },
														 {
															switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
														 }
													);
												</script>
												</div>	
											</td>
									  </tr>

									  <tr height="10px">
											<td colspan="3">
											</td>
									  </tr>
										
									  <tr bgcolor="#444f53">
									    <td bgcolor="#444f53" class="cloud_main_radius_left" width="20%" height="50px">
												<div align="center"><img src="/images/cloudsync/002.png">
													<div align="center" style="margin-top:10px;font-size: 18px;text-shadow: 1px 1px 0px black;"><%tcWebApi_Get("String_Entry", "Smart_Access", "s")%></div>
												</div>
											</td>
									    <td>
												<div align="center"><img src="/images/cloudsync/line.png"></div>
											</td>
									    <td>
												&nbsp;
											</td>
									    <td width="">
												<div style="padding:10px;width:95%;font-style:italic;font-size:14px;">
													<%tcWebApi_Get("String_Entry", "smart_access_desc", "s")%><br />
												</div>
											</td>
									    <td bgcolor="#444f53" class="cloud_main_radius_right" width="100">
												<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_smartAccess_enable"></div>
												<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
												<script type="text/javascript">
													$j('#radio_smartAccess_enable').iphoneSwitch('<% tcWebApi_get("AiCloud_Entry", "webdav_proxy", "s") %>', 
														 function() {
															curState = '<% tcWebApi_get("AiCloud_Entry", "webdav_proxy", "s") %>';
															document.form.webdav_proxy.value = 1;
															document.form.enable_webdav.value = 1;
															document.form.editFlag.value = 1;
															FormActions("cloud_apply.asp", "apply", "restart_webdav", "3");
															showLoading();	
															document.form.submit();
														 },
														 function() {
															curState = '<% tcWebApi_get("AiCloud_Entry", "webdav_proxy", "s") %>';
															document.form.webdav_proxy.value = 0;
															if(document.form.webdav_aidisk.value == 0)
																document.form.enable_webdav.value = 0;
															document.form.editFlag.value = 1;
															FormActions("cloud_apply.asp", "apply", "restart_webdav", "3");
															showLoading(5);	
															document.form.submit();
														 },
														 {
															switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
														 }
													);
												</script>			
												</div>	
											</td>
									  </tr>

									  <tr height="10px">
											<td colspan="3">
											</td>
									  </tr>

									  <tr id="tr_smart_sync" bgcolor="#444f53">
									    <td bgcolor="#444f53" class="cloud_main_radius_left" width="20%" height="50px">
												<div align="center">
													<img src="/images/cloudsync/003.png">
													<div align="center" style="margin-top:10px;font-size: 18px;text-shadow: 1px 1px 0px black;"><% tcWebApi_Get("String_Entry","smart_sync","s") %></div>
												</div>
											</td>
									    <td>
												<div align="center"><img src="/images/cloudsync/line.png"></div>
											</td>
									    <td>
												&nbsp;
											</td>
									    <td width="">
											<div style="padding:10px;width:95%;font-style:italic;font-size:14px;">
												<%tcWebApi_Get("String_Entry", "smart_sync_desc", "s")%><br />
											</div>
										</td>
									    <td bgcolor="#444f53" class="cloud_main_radius_right" width="100">

						  					<input name="button" type="button" class="button_gen_short" onclick="location.href='/cgi-bin/cloud_sync.asp'" value="<%tcWebApi_Get("String_Entry", "btn_go", "s")%>"/>
										</td>
									  </tr>

									</table>

							  </td>
							</tr>				
							</tbody>	
				  	</table>			
					</td>
				</tr>
			</table>
		</td>
		<td width="20"></td>
	</tr>
</table>
<div id="footer"></div>
</form>

</body>
</html>
