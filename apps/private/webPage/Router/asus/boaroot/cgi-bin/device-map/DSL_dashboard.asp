<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--device-map/DSL_dashboard.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>DSL dashboard</title>
<link rel="stylesheet" type="text/css" href="/NM_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<style type="text/css">
.title{
	font-size:16px;
	text-align:center;
	font-weight:bolder;
	margin-bottom:5px;
}

.line_image{
	*margin-top:-10px;
}

.earth_logo{
	width: 70px;	/*94*/
  height: 70px;	/*94*/
  margin-left:85px;
	background-image: url('/images/New_ui/networkmap/DSL_icon.png');
	background-size: 70px;
	background-repeat: no-repeat;	
	z-index:2;
}
.connected_logo{
	width: 10px;	/*10*/
  height: 135px;	/*172*/
  margin-top:-48px;
  margin-left:116px;
	background-image: url('/images/New_ui/networkmap/vertical_line.png');
	/*background-size: 70px;*/
	background-repeat: no-repeat;
	z-index:1;
}
.connecting_logo{
	width: 10px;	/*10*/
  height: 135px;	/*172*/
  margin-top:-9px;
  margin-left:116px;
	background-image: url('/images/New_ui/networkmap/vertical_connecting.gif');
	background-position: 0px -19px;	
	/*background-size: 70px;*/
	background-repeat: no-repeat;
	z-index:1;
}
.disconnect_logo{
	width: 10px;	/*10*/
  height: 135px;	/*172*/
  margin-top:-9px;
  margin-left:116px;
	background-image: url('/images/New_ui/networkmap/vertical_disconnect.png');
	background-position: 0px -19px;	
	/*background-size: 70px;*/
	background-repeat: no-repeat;
	z-index:1;
}
.router_logo{
	width: 70px;	/*94*/
  height: 70px;	/*94*/
  margin-top:-37px;
  margin-left:92px;
	background-image: url('/images/New_ui/networkmap/router_icon.png');
	background-size: 65px;
	background-repeat: no-repeat;
	z-index:2;
}

.router_logo_disconnect{
	width: 70px;	/*94*/
  height: 70px;	/*94*/
  margin-top:-53px;
  margin-left:92px;
	background-image: url('/images/New_ui/networkmap/router_icon.png');
	background-size: 65px;
	background-repeat: no-repeat;
	z-index:2;
}
.tx_logo{
	width: 22px; 
	height: 32px;	
	background-image:url('/images/New_ui/networkmap/direction_bw.gif');	
	background-size: 18px;
	background-repeat: no-repeat;
}
.rx_logo{
  width: 22px;
  height: 32px;
	background-image:url('/images/New_ui/networkmap/direction_bw.gif');
	-webkit-transform: rotate(180deg);
	-moz-transform: rotate(180deg);
	-ms-transform: rotate(180deg);
	-o-transform: rotate(180deg);
	transform: rotate(180deg);
	background-size: 18px;
	background-repeat: no-repeat;
}

</style>
<script>
<% wanlink(); %>
var $j = jQuery.noConflict();
var wan_type = wanlink_type();
var wan_IP = wanlink_ipaddr();
var wandns = wanlink_dns();
var wan_Gateway = wanlink_gateway();
	
var vendor_id = "<% tcWebApi_get("Info_Adsl","FarEndVendorID","s"); %>";	
var sync_status = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";	
var sync_status_update = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
var adsl_timestamp = parseInt("<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>");
var adsl_timestamp_update = parseInt("<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>");
var adsl_boottime = boottime - adsl_timestamp;

var fwVer = "<% tcWebApi_get("Info_Adsl","fwVer","s") %>";
var filter_HwVer = fwVer.indexOf("HwVer") >= 0 ? true:false; 
if(filter_HwVer)
	fwVer = fwVer.slice(0, fwVer.indexOf("HwVer")-1);

var dsl_type = "<% tcWebApi_get("Info_Adsl","AdslType","s") %>".replace("_", " ");
var status_isVDSLmode = "<%tcWebApi_staticGet("Info_Adsl","xDSLmode","s")%>";
if(status_isVDSLmode == "VDSL"){
	dsl_type = dsl_type.replace("ANNEX ", "");
}

function display_vdsl_band_status(){
	if(status_isVDSLmode == "VDSL") {
		document.getElementById("th_AdslType").innerHTML = "<%tcWebApi_get("String_Entry","dslsetting_disc2_vdsl","s")%>";	/*untranslated*/
	}
	else {
		document.getElementById("th_AdslType").innerHTML = "<%tcWebApi_get("String_Entry","dslsetting_disc2","s")%>";
	}
}
	
function tabclickhandler(unit){	
	location.href = "internet.asp";	
}

function showadslbootTime(){
	if(adsl_timestamp_update != "" && sync_status_update == "up")
	{
		if(adsl_boottime < 0)
			adsl_boottime = boottime - adsl_timestamp_update;
		Days = Math.floor(adsl_boottime / (60*60*24));
		Hours = Math.floor((adsl_boottime / 3600) % 24);
		Minutes = Math.floor(adsl_boottime % 3600 / 60);
		Seconds = Math.floor(adsl_boottime % 60);

		document.getElementById("div_Uptime").style.display = "";
		$("boot_days").innerHTML = Days;
		$("boot_hours").innerHTML = Hours;
		$("boot_minutes").innerHTML = Minutes;
		$("boot_seconds").innerHTML = Seconds;
		adsl_boottime += 1;
		setTimeout("showadslbootTime()", 1000);
	}
	else
	{
		document.getElementById("div_Uptime").style.display = "none";
	}
}
	
function initial()
{
	document.getElementById("dsl_ver_info").innerHTML = fwVer;
	if(sync_status_update.toUpperCase() == "UP"){
		if(vendor_id == "Unknown")
			document.getElementById("vendor_id").innerHTML = "";
		else
			document.getElementById("vendor_id").innerHTML = "<b>"+vendor_id+"</b>";
		document.getElementById("connection_icon").className = "connected_logo";
		document.getElementById("DataRateU_div").style.display = "";
		document.getElementById("DataRateD_div").style.display = "";
		document.getElementById("div_AdslState").innerHTML = "";
		document.getElementById("router_icon").className = "router_logo";
		document.getElementById("area_Opmode").style.display = "";
		display_vdsl_band_status();
		document.getElementById("area_AdslType").style.display = "";
		document.getElementById("div_AdslType").innerHTML = "<p style=\"padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;\">"+dsl_type+"</p>";
		document.getElementById("area_Uptime").style.display = "";
		
	}
	else{
		document.getElementById("vendor_id").innerHTML = "";
		if(sync_status_update.toUpperCase() == "DOWN"){
				document.getElementById("connection_icon").className = "disconnect_logo";	
				document.getElementById("div_AdslState").innerHTML = "<b>Link Down</b>";
		}else{
				if(sync_status_update.toUpperCase() == "INITIALIZING")
					document.getElementById("div_AdslState").innerHTML = "<b>Initializing</b>";
				else
					document.getElementById("div_AdslState").innerHTML = "<b>Wait for init</b>";	
					
				document.getElementById("connection_icon").className = "connecting_logo";
		}		
		document.getElementById("DataRateU_div").style.display = "none";
		document.getElementById("DataRateD_div").style.display = "none";
		document.getElementById("router_icon").className = "router_logo_disconnect";
		document.getElementById("area_Opmode").style.display = "none";
		document.getElementById("area_AdslType").style.display = "none";
		document.getElementById("area_Uptime").style.display = "none";
	}	
	
	showadslbootTime();
	setTimeout("update_log();", 3000);
	
}

function update_log(){
	$j.ajax({
		url: '/cgi-bin/ajax_AdslStatus.asp',
		dataType: 'script',
		error: function(xhr){
				setTimeout("update_log();", 1000);
			},
 	
		success: function(){
				if(adsl_timestamp_update != "" && sync_status != sync_status_update){
					adsl_boottime = boottime - adsl_timestamp_update;
					showadslbootTime();
				}
				sync_status = sync_status_update;				
				if(sync_status_update.toUpperCase() == "UP"){
					document.getElementById("connection_icon").className = "connected_logo";
					if(vendor_id == "Unknown")
						document.getElementById("vendor_id").innerHTML = "";
					else
						document.getElementById("vendor_id").innerHTML = "<b>"+vendor_id+"</b>";
					document.getElementById("DataRateU_div").style.display = "";
					document.getElementById("div_DataRateUp").innerHTML = log_DataRateUp;
					document.getElementById("DataRateD_div").style.display = "";
					document.getElementById("div_DataRateDown").innerHTML = log_DataRateDown;
					document.getElementById("div_AdslState").innerHTML = "";
					document.getElementById("div_Opmode").innerHTML = "<p style=\"padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;\">"+log_Opmode+"</p>";
					if(status_isVDSLmode == "VDSL"){
						log_AdslType = log_AdslType.replace("ANNEX ", "");
					}
					document.getElementById("div_AdslType").innerHTML = "<p style=\"padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;\">"+log_AdslType+"</p>";
					document.getElementById("router_icon").className = "router_logo";
					document.getElementById("area_Opmode").style.display = "";
					document.getElementById("area_AdslType").style.display = "";
					document.getElementById("area_Uptime").style.display = "";
														
				}
				else{
					if(sync_status_update.toUpperCase() == "DOWN"){
							document.getElementById("connection_icon").className = "disconnect_logo";
							document.getElementById("div_AdslType").innerHTML = "";
							document.getElementById("div_AdslState").innerHTML = "<b>Link Down</b>";
					}
					else{
							document.getElementById("connection_icon").className = "connecting_logo";
							document.getElementById("div_AdslType").innerHTML = "";
							if(sync_status_update.toUpperCase() == "INITIALIZING")		
								document.getElementById("div_AdslState").innerHTML = "<b>Initializing</b>";
							else
								document.getElementById("div_AdslState").innerHTML = "<b>Wait for init</b>";	
					}		
					document.getElementById("vendor_id").innerHTML = "";
					document.getElementById("DataRateU_div").style.display = "none";
					document.getElementById("DataRateD_div").style.display = "none";
					document.getElementById("div_Opmode").innerHTML = "<p style=\"padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;\">"+log_Opmode+"</p>";
					document.getElementById("router_icon").className = "router_logo_disconnect";
					document.getElementById("area_Opmode").style.display = "none";
					document.getElementById("area_AdslType").style.display = "none";
					document.getElementById("area_Uptime").style.display = "none";
				}
				display_vdsl_band_status();
				setTimeout("update_log();", 3000);
			}	
	});		
}
</script>
</head>
<body class="statusbody" onload="initial();">
<form method="POST" name="DSLInfo_Form" action="/cgi-bin/device-map/DSL_dashboard.asp">
	<INPUT TYPE="HIDDEN" NAME="Saveflag" VALUE="0">	
<table id="DSL_tab" border="0" cellpadding="0" cellspacing="0">
<tr>
	<td>
	<table width="100px" border="0" align="left" style="margin-left:8px;" cellpadding="0" cellspacing="0">
	<td>
		<div id="t0" class="tab_NW" align="center" style="margin-right:2px; width:90px;" onclick="tabclickhandler(0)">
			<span id="span0" style="cursor:pointer;font-weight: bolder;">WAN</span>
		</div>
	</td>
	<td>
		<div id="t1" class="tabclick_NW" align="center" style="font-weight: bolder; margin-right:2px; width:90px;" onclick="tabclickhandler(1)">
			<span id="span1" style="cursor:pointer;">DSL</span>
		</div>
	</td>
	</table>
	</td>
</tr>
</table>
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" class="table1px" id="DSL_dashboard">
<tr>
	<td>
		<div style="margin-left:0px;">
		<table>
			<tr>
				<td>
				<div>
					<table>
					<tr>						
						<td class="earth_icon_td">
							<div id="earth_icon" class="earth_logo"></div>														
						</td>
						<td colspan="2">
							<div style="margin-left:0px;"><b>DSL Exchange</b></div>
							<div id="vendor_id" style="margin-top:5px;margin-left:0px;" title="DSL chipset vendor"></div>
						</td>	
					</tr>
					<tr>
						<td colspan="3">
						<div id="DataRateD_div" style="margin-top:45px;margin-left:75px;">	
							<div id="div_DataRateDown" style="margin-top:0px;margin-left:-39px;font-size:12px;" title="Data Rate Down"><% tcWebApi_get("Info_Adsl","DataRateDown","s") %></div>
							<div id="rx_icon" class="rx_logo" style="margin-top:-5px;margin-left:0ppx;"></div>
						</div>
						<div id="DataRateU_div" style="margin-top:-89px;margin-left:170px;">
							<div id="tx_icon" class="tx_logo" style="margin-top:px;margin-left:-28px;"></div>
							<div id="div_DataRateUp" style="margin-top:-6px;margin-left:-35px;font-size:12px;" title="Data Rate Up"><% tcWebApi_get("Info_Adsl","DataRateUp","s") %></div>						
						</div>
						<div id="connection_icon" class=""></div>
						
						</td>						
					</tr>							
					<tr>
						<td class="router_td" colspan="3">
						<div id="div_AdslState" style="margin-top:0px;margin-left:0px;"></div>	
						<div>									
								<div id="router_icon" class=""></div>
						</div>
						</td>
					</tr>
					<tr>						
						<td class="DSL_info" colspan="3">						
						<div id="dsl_ver_info" style="margin-top:-2px;margin-left:50px;" title="<%tcWebApi_get("String_Entry","adsl_fw_ver_in","s")%>"></div>
						</td>
					</tr>
					<tr>
						<td colspan="3">						
						<div style="margin-top:20px;margin-left:0px;">
							<table>
								<tr>
									<td>
										<div id="area_Opmode">
										<img style="margin-bottom:10px;" src="/images/New_ui/networkmap/linetwo2.png">										
										<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","dslsetting_disc1","s")%></p>
										<div id="div_Opmode" style="width:100%;">
												<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
												<% tcWebApi_get("Info_Adsl","Opmode","s") %>
												</p>
										</div>										
										<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
										</div>
									</td>		
								</tr>
								<tr>
									<td>
										<div id="area_AdslType">
										<p class="formfonttitle_nwm" id="th_AdslType"><%tcWebApi_get("String_Entry","dslsetting_disc2","s")%></p>
										<div id="div_AdslType">
												<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
												<% tcWebApi_get("Info_Adsl","AdslType","s") %>
												</p>
										</div>
										<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
										</div>
									</td>		
								</tr>	
								<tr>
									<td>
										<div id="area_Uptime">
										<p class="formfonttitle_nwm" >DSL <%tcWebApi_get("String_Entry","General_x_SystemUpTime_in","s")%></p>
										<div id="div_Uptime">
												<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
												<span id="boot_days"></span> <%tcWebApi_get("String_Entry","Day","s")%> <span id="boot_hours"></span> <%tcWebApi_get("String_Entry","Hour","s")%> <span id="boot_minutes"></span> <%tcWebApi_get("String_Entry","Minute","s")%> <span id="boot_seconds"></span> <%tcWebApi_get("String_Entry","Second","s")%>
												</p>
										</div>
										<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
										</div>
									</td>		
								</tr>
							</table>
						</div>	
						</td>							
					</tr>	
					</table>
				</div>
				</td>
			</tr> 
		</table>
		</div>
	</td>
</tr>
<tr>
</tr>
<tr>
</tr>
<tr>
</tr>
</table>
</form>
</body>

<!--device-map/DSL_dashboard.asp-->
</html>

