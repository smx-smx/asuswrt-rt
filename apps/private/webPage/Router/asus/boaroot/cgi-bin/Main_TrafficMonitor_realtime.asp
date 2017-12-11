<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<meta http-equiv="X-UA-Compatible" content="IE=Edge" />
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","traffic_monitor","s")%> : <%tcWebApi_get("String_Entry","menu4_2_1","s")%></title>	
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/tmmenu.css">
<link rel="stylesheet" type="text/css" href="/menu_style.css">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/tmmenu.js"></script>
<script language="JavaScript" type="text/javascript" src="/tmcal.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script type='text/javascript'>
/*
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
*/

nvram = {
	wan_ifname: '',
	lan_ifname: 'br0',
	wl_ifname: 'rai0',
	wan_proto: 'pppoe',
	web_svg: '',
	rstats_colors: '',
	http_id: 'TIDe855a6487043d70a'};

var cprefix = 'bw_r';
var updateInt = 2;
var updateDiv = updateInt;
var updateMaxL = 300;
var updateReTotal = 1;
var prev = [];
var speed_history = [];
var debugTime = 0;
var avgMode = 0;
var wdog = null;
var wdogWarn = null;

//disable auto log out
AUTOLOGOUT_MAX_MINUTE = 0;

var ref = new TomatoRefresh('update.asp', 'output=netdev', 2);

ref.stop = function() {
	this.timer.start(1000);
}

ref.refresh = function(text) {
	var c, i, h, n, j, k;

	watchdogReset();

	++updating;
	try {
		netdev = null;
		eval(text);

		n = (new Date()).getTime();
		if (this.timeExpect) {
			if (debugTime) E('dtime').innerHTML = (this.timeExpect - n) + ' ' + ((this.timeExpect + 2000) - n);
			this.timeExpect += 2000;
			this.refreshTime = MAX(this.timeExpect - n, 500);
		}
		else {
			this.timeExpect = n + 2000;
		}

		for (i in netdev) {
			c = netdev[i];
			if ((p = prev[i]) != null) {
				h = speed_history[i];

				h.rx.splice(0, 1);
				//h.rx.push((c.rx < p.rx) ? (c.rx + (0xFFFFFFFF - p.rx)) : (c.rx - p.rx));
				h.rx.push((c.rx < p.rx) ? 100 : (c.rx - p.rx));

				h.tx.splice(0, 1);
				//h.tx.push((c.tx < p.tx) ? (c.tx + (0xFFFFFFFF - p.tx)) : (c.tx - p.tx));
				h.tx.push((c.tx < p.tx) ? 100 : (c.tx - p.tx));
			}
			else if (!speed_history[i]) {
				speed_history[i] = {};
				h = speed_history[i];
				h.rx = [];
				h.tx = [];
				for (j = 300; j > 0; --j) {
					h.rx.push(0);
					h.tx.push(0);
				}
				h.count = 0;
			}
			prev[i] = c;
		}
		loadData();
	}
	catch (ex) {
	}
	--updating;
}

function watchdog()
{
	watchdogReset();
	ref.stop();
	wdogWarn.style.display = '';
}

function watchdogReset()
{
	if (wdog) clearTimeout(wdog)
	wdog = setTimeout(watchdog, 10000);
}

function init()
{
	speed_history = [];
	initCommon(2, 0, 0, 1);
	wdogWarn = E('warnwd');
	watchdogReset();
	ref.start();
	document.getElementById("faq0").href = "https://www.asus.com/support/FAQ/114483/" ;
}

function switchPage(page){
	if(page == "1")
		return false;
	else if(page == "2")
		location.href = "Main_TrafficMonitor_last24.asp";
	else
		location.href = "Main_TrafficMonitor_daily.asp";
}
</script>
</head>

<body onload="show_menu();init();" >
<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="apply.cgi" target="hidden_frame">
<input type="hidden" name="current_page" value="Main_TrafficMonitor_realtime.asp">
<input type="hidden" name="next_page" value="Main_TrafficMonitor_realtime.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
</form>
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
	<td width="23">&nbsp;</td>

<!--=====Beginning of Main Menu=====-->
	<td valign="top" width="202">
	  <div id="mainMenu"></div>
	  <div id="subMenu"></div>
	</td>

  <td valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
      	<!--===================================Beginning of Main Content===========================================-->
      	<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
  		<tr>
			<td align="left"  valign="top">
			<table width="100%" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
				<tbody>
				<!--===================================Beginning of graph Content===========================================-->
	      		<tr>
					<td bgcolor="#4D595D" valign="top"  >
		  				<table width="740px" border="0" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="TMTable">
        			<tr>
						<td>
							<table width="100%" >
							<tr>
							<td  class="formfonttitle" align="left">
										<div style="margin-top:5px;"><% tcWebApi_Get("String_Entry", "Menu_TrafficManager", "s") %> - <% tcWebApi_Get("String_Entry", "traffic_monitor", "s") %></div>
									</td>
							<td>
     						<div align="right">
     		   					<select onchange="switchPage(this.options[this.selectedIndex].value)" class="input_option">
									<!--option><%tcWebApi_get("String_Entry","switchpage","s")%></option-->
											<option value="1" selected><%tcWebApi_get("String_Entry","menu4_2_1","s")%></option>
											<option value="2"><%tcWebApi_get("String_Entry","menu4_2_2","s")%></option>
											<option value="3"><%tcWebApi_get("String_Entry","menu4_2_3","s")%></option>
								</select>
							</div>
							</td></tr></table>
						</td>
        			</tr>
        			<tr>
          				<td height="5"><img src="/images/New_ui/export/line_export.png" /></td>
        			</tr>
        			<tr>
          				<td height="30" align="left" valign="middle" >
							<div class="formfontcontent">
								<p class="formfontcontent"><%tcWebApi_get("String_Entry","traffic_monitor_desc1","s")%></p>
							</div>
          				</td>
        			</tr>
        			<tr>
          				<td align="left" valign="middle">
						<!-- add some hard code of style attributes to wordkaround for IE 11-->
						<table width="95%" border="1" align="left" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="DescTable" style="font-size:12px; font-family:Arial, Helvetica, sans-serif;border: 1px solid #000000; border-collapse: collapse;">
							<tr><th style=" font-family:Arial, Helvetica, sans-serif; background-color:#1F2D35; color:#FFFFFF; font-weight:normal;line-height:15px; height: 30px; text-align:left; font-size:12px; padding-left: 10px; border: 1px solid #222; border-collapse: collapse; background:#2F3A3E;" width="16%"></th><th style="font-family:Arial, Helvetica, sans-serif; background-color:#1F2D35; color:#FFFFFF; font-weight:normal; line-height:15px; height: 30px; text-align:left; font-size:12px; padding-left: 10px;er: 1px solid #222; border-collapse: collapse; background:#2F3A3E;" width="26%"><% tcWebApi_Get("String_Entry", "Internet", "s") %></th><th style="font-family:Arial, Helvetica, sans-serif; background-color:#1F2D35; color:#FFFFFF; font-weight:normal; line-height:15px;height: 30px; text-align:left; font-size:12px; padding-left: 10px; border: 1px solid #222; border-collapse: collapse; background:#2F3A3E;" width="29%"><% tcWebApi_Get("String_Entry", "tm_wired", "s") %></th><th style="font-family:Arial, Helvetica, sans-serif; background-color:#1F2D35; color:#FFFFFF; font-weight:normal; line-height:15px; height: 30px; text-align:left; font-size:12px;padding-left: 10px; border: 1px solid #222; border-collapse: collapse; background:#2F3A3E;" width="29%"><% tcWebApi_Get("String_Entry", "tm_wireless", "s") %></th></tr>
							<tr><th style=" font-family:Arial, Helvetica, sans-serif; background-color:#1F2D35; color:#FFFFFF; font-weight:normal;line-height:15px; height: 30px; text-align:left; font-size:12px; padding-left: 10px; border: 1px solid #222; border-collapse: collapse; background:#2F3A3E;"><% tcWebApi_Get("String_Entry", "tm_reception", "s") %></th><td style="color:#FF9000;padding-left: 10px; background-color:#475a5f; border: 1px solid #222;border-collapse: collapse;"><% tcWebApi_Get("String_Entry", "tm_recp_int", "s") %></td><td style="color:#3CF;padding-left: 10px; background-color:#475a5f; border: 1px solid #222;border-collapse: collapse;"><% tcWebApi_Get("String_Entry", "tm_recp_wired", "s") %></td><td style="color:#3CF;padding-left: 10px; background-color:#475a5f; border: 1px solid #222;border-collapse: collapse;"><% tcWebApi_Get("String_Entry", "tm_recp_wireless", "s") %></td></tr>
							<tr><th style=" font-family:Arial, Helvetica, sans-serif; background-color:#1F2D35; color:#FFFFFF; font-weight:normal;line-height:15px; height: 30px; text-align:left; font-size:12px; padding-left: 10px; border: 1px solid #222; border-collapse: collapse; background:#2F3A3E;"><% tcWebApi_Get("String_Entry", "tm_transmission", "s") %></th><td style="color:#3CF;padding-left: 10px; background-color:#475a5f; border: 1px solid #222;border-collapse: collapse;"><% tcWebApi_Get("String_Entry", "tm_trans_int", "s") %></td><td style="color:#FF9000;padding-left: 10px; background-color:#475a5f; border: 1px solid #222;border-collapse: collapse;"><% tcWebApi_Get("String_Entry", "tm_trans_wired", "s") %></td><td style="color:#FF9000;padding-left: 10px; background-color:#475a5f; border: 1px solid #222;border-collapse: collapse;"><% tcWebApi_Get("String_Entry", "tm_trans_wireless", "s") %></td></tr>
						</table>
						<!--End-->
          				</td>
        			</tr>
        			<tr>
          				<td height="30" align="left" valign="middle">
						<div class="formfontcontent"><p class="formfontcontent"><%tcWebApi_get("String_Entry","traffic_monitor_desc2","s")%></p></div>
						<div class="formfontcontent"><p class="formfontcontent"><a id="faq0" href="" target="_blank" style="font-weight: bolder;text-decoration:underline;"><% tcWebApi_Get("String_Entry", "traffic_monitor", "s") %> FAQ</a></p></div>
          				</td>
        			</tr>
        			<tr>
        				<td>
							<span id="tab-area"></span>
								<!--========= svg =========-->
								<!--[if IE]>
									<div id="svg-table" align="left">
										<object id="graph" src="/tm.svg" classid="image/svg+xml" width="730" height="350">
									</div>
								<![endif]-->
								<!--[if !IE]>-->
								<object id="graph" data="/tm.svg" type="image/svg+xml" width="730" height="350">
								<!--<![endif]-->
								</object>
      							<!--========= svg =========-->
      					</td>
        			</tr>

  		     		<tr>
						<td >
				    	 	<table width="735px" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable_NWM" style="margin-top:10px">
						  		<tr>
						  			<!--th style="text-align:center; width:160px;height:25px;"><%tcWebApi_get("String_Entry","Network","s")%></th-->
						  			<th style="text-align:center; width:160px;"><%tcWebApi_get("String_Entry","Current","s")%></th>
						  			<th style="text-align:center; width:160px;"><%tcWebApi_get("String_Entry","Average","s")%></th>
						  			<th style="text-align:center; width:160px;"><%tcWebApi_get("String_Entry","Maximum","s")%></th>
						  			<th style="text-align:center; width:160px;"><%tcWebApi_get("String_Entry","Total","s")%></th>
						  		</tr>
						  		<tr>
						  			<!--td style="text-align:center; background-color:#111;">
										<div id='rx-sel'><%tcWebApi_get("String_Entry","tm_reception","s")%>
										</div>
						  			</td-->
						  			<td style="text-align:center;font-weight: bold; background-color:#111;"><div id="rx-current"></div></td>
						  			<td style="text-align:center; background-color:#111;" id='rx-avg'></td>
						  			<td style="text-align:center; background-color:#111;" id='rx-max'></td>
						  			<td style="text-align:center; background-color:#111;" id='rx-total'></td>
						    	</tr>
						    	<tr>
						    		<!--td style="text-align:center; background-color:#111;">
											<div id='tx-sel'><%tcWebApi_get("String_Entry","tm_transmission","s")%>
											</div>
              			</td-->
										<td style="text-align:center;font-weight: bold; background-color:#111;"><div id="tx-current"></div></td>
										<td style="text-align:center; background-color:#111;" id='tx-avg'></td>
										<td style="text-align:center; background-color:#111;" id='tx-max'></td>
										<td style="text-align:center; background-color:#111;" id='tx-total'></td>
									</tr>
							</table>
						</td>
					</tr>
					</table>
					</td>
				</tr>

				<tr style="display:none">
					<td bgcolor="#FFFFFF">
		  				<table width="100%"  border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
							<thead>
								<tr>
									<td colspan="5" id="TriggerList">Display Options</td>
								</tr>
							</thead>

						<div id='bwm-controls'>
							<tr>
								<th width='50%'>Traffic_Avg</th>
								<td>
									<a href='javascript:switchAvg(1)' id='avg1'>Off</a>,
									<a href='javascript:switchAvg(2)' id='avg2'>2x</a>,
									<a href='javascript:switchAvg(4)' id='avg4'>4x</a>,
									<a href='javascript:switchAvg(6)' id='avg6'>6x</a>,
									<a href='javascript:switchAvg(8)' id='avg8'>8x</a>
								</td>
							</tr>
							<tr>
								<th>Traffic_Max</th>
								<td>
									<a href='javascript:switchScale(0)' id='scale0'>Uniform</a>,
									<a href='javascript:switchScale(1)' id='scale1'>Per IF</a>
								</td>
							</tr>
							<tr>
								<th>Traffic_SvgDisp</th>
								<td>
									<a href='javascript:switchDraw(0)' id='draw0'>Solid</a>,
									<a href='javascript:switchDraw(1)' id='draw1'>Line</a>
								</td>
							</tr>
							<tr>
								<th>Traffic_Color</th>
								<td>
									<a href='javascript:switchColor()' id='drawcolor'>-</a><a href='javascript:switchColor(1)' id='drawrev'>Traffic_Reverse</a>
								</td>
							</tr>
						</div>
						</table>
					</td>
				</tr>
			</tbody>
			</table>
		</td>
	</tr>
	</table>
	</td>
	</tr>
</table>

<div id="footer"></div>
</body>
</html>
<!--End of Main_TrafficMonitor_realtime.asp-->


