<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta charset="utf-8"/>
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">

<script type="text/JavaScript" src="/jquery.js"></script>
<script type="text/javascript">
var detect_dhcp_pppoe = "";
var current_lanip = '<% tcWebApi_get("Lan_Entry","IP","s") %>';
var current_mask = '<% tcWebApi_get("Lan_Entry","netmask","s") %>';
var new_lan_ip = '';
var setting_LAN_time = 23;
var wanstate = -1;
var wansbstate = -1;
var wanauxstate = -1;
//var _wlc_ssid = decodeURIComponent('<% nvram_char_to_ascii("WLANConfig11b", "wlc_ssid"); %>');

// get the wan state at the first time.
<% wanstate(); %>

var done_setting_lan = 0;

var winH, winW;
var progress, y;

var casenum = '<% get_parameter("flag"); %>';

function initial(){
	var html_code = '';

	hideLinkTag();

	if(casenum == 1){
		showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","dsl_w_r_r1","s")%>");

		$("#case1").show();
	}
	else if(casenum == 2){
		if(wansbstate == 2 || wanauxstate == 3){
			showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","dsl_w_r_r2_1","s")%>");

			$("#ppp_input_error").show();
		}
		else{
			showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","w_r_r2_2","s")%>");

			$("#ppp_no_response").show();
		}

		$("#case2").show();
	}
	else if(casenum == 3){
		/*if(detect_dhcp_pppoe=="dhcp"){
			showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","w_r_r3_1","s")%>");

			$("#case3").show();
		}
		else{
			showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","w_r_r3_2","s")%>");
		}*/
		showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","qis_fail_desc5","s")%>");
	}
	else if(casenum == 4){
		showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","w_r_r4","s")%>");

		$("#case4").show();
	}
	else if(casenum == 5){
		if(wanstate == 5)
			showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","w_r_r5_1","s")%>");
		else
			showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","w_r_r5_2","s")%>");

		html_code += '<ul>\n';
		if(wanstate == 5)
			html_code += '<li><%tcWebApi_get("String_Entry","w_r_suggestion5_1","s")%></li>\n';
		else
			html_code += '<li><%tcWebApi_get("String_Entry","w_r_suggestion5_2","s")%></li>\n';
		html_code += '</ul>\n';

		if(wanstate == 5){
			html_code += '<ul>\n';
			html_code += '<li><a href="javascript:manually_start_wan_Link();" style="color:#569AC7; text-decoration:underline;"><%tcWebApi_get("String_Entry","w_r_suggestion_mnl_start_wan","s")%></a></li>\n';
			html_code += '</ul>\n';
		}

		$("#case5")[0].innerHTML = html_code;
		$("#case5").show();
	}
	else if(casenum.search("6") !== -1){
		new_lan_ip = get_new_lanip();

		showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","w_r_r6","s")%>");

		showtext($("#suggestion_title")[0], "<%tcWebApi_get("String_Entry","w_r_suggestion0","s")%>:");

		html_code += "<ul>\n";
		html_code += "<li><%tcWebApi_get("String_Entry","web_redirect_case6_desc1","s")%><br>";
		html_code += "<B>"+new_lan_ip+"</B></li>\n";
		html_code += "<li><%tcWebApi_get("String_Entry","web_redirect_case6_desc2","s")%></li>\n";
		html_code += "</ul>\n";
		html_code += "<br>\n";
		html_code += "<div style=\"text-align: right;\">\n";
		html_code += "<input type=\"button\" class=\"button_gen\" width=\"72\" onclick=\"change_lan_subnet();\" value=\"<%tcWebApi_get("String_Entry","btn_next","s")%>\">\n";
		html_code += "</div>\n";

		$("#case6")[0].innerHTML = html_code;
		$("#case6").show();
		$("#suggestion").hide();
	}
	else if(casenum == 8){
		new_lan_ip = get_new_lanip();
		showtext($("#failReason1")[0], "<%tcWebApi_get("String_Entry","APS_action_ConnectingStatus0","s")%>");
		showtext($("#suggestion_title")[0], "<%tcWebApi_get("String_Entry","w_r_suggestion0","s")%>:");
		html_code += "<ul>\n";
		html_code += "<li><%tcWebApi_get("String_Entry","APS_action_search_again_hint2","s")%>";
		html_code += "<span style=\"text-align: right;\">\n";
		html_code += "<input type=\"button\" class=\"button_gen\" width=\"72\" onclick=\"javascript:location.href=\'http://router.asus.com/cgi-bin/QIS_wizard.asp?flag=sitesurvey\';\" value=\"<%tcWebApi_get("String_Entry","QIS_rescan","s")%>\">";
		html_code += "</span>\n";
		html_code += "</li>\n";

		html_code += "<li><%tcWebApi_get("String_Entry","QKS_finish_moveto8","s")%>";
		html_code += "<span style=\"text-align: right;\">\n";
		html_code += "<input type=\"button\" class=\"button_gen\" width=\"72\" onclick=\"javascript:location.href=\'/cgi-bin/QIS_wizard.asp?flag=manual\';\" value=\"<%tcWebApi_get("String_Entry","Manual_Setting_btn","s")%>\">";
		html_code += "</span>\n";
		html_code += "</li>\n";
		html_code += "</ul>\n";
		$("#case8")[0].innerHTML = html_code;
		$("#case8").show();
		$("#suggestion").hide();
	}
	else{
		parent.location = "/cgi-bin/index2.asp";
	}
}

function showtext(obj, str){
	if(obj)
		obj.innerHTML = str;
}

function hideLinkTag(){
	if(document.all){
		var tagObjs = document.all.tags("a");

		for(var i = 0; i < tagObjs.length; ++i)
			tagObjs(i).outerHTML = tagObjs(i).outerHTML.replace(">"," hidefocus=true>");
	}
}

function wanLink(){
	location.href = "/cgi-bin/Advanced_DSL_Content.asp";
}

function detectLink(){
	location.href = "/cgi-bin/QIS_wizard.asp?flag=detect";
}

function manually_start_wan_Link(){
	location.href = "/cgi-bin/index2.asp?flag=Internet";
}

function change_lan_subnet(){
	showLoadingBar(setting_LAN_time);
	send_setting();
	setTimeout("check_system_ready();", setting_LAN_time*1000);
}

function check_system_ready(){
	if(wanstate != 2){
		$("#proceeding_action")[0].innerHTML = "<%tcWebApi_get("String_Entry","error_page_lanip_changed_desc1","s")%>";
		detect_router();
		setTimeout("check_system_ready();", 3000);
	}
	else{
		$("#proceeding_img")[0].style.width = "100%";
		$("#proceeding_img_text")[0].innerHTML = "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>";
		$("#proceeding_action")[0].innerHTML = "<%tcWebApi_get("String_Entry","APS_action_renew_success","s")%>";

		setTimeout("parent.location = \"http://"+new_lan_ip+"/cgi-bin/QIS_wizard.asp?flag=detect\"", 2*1000);
	}
}

function send_setting(){
	$.ajax({
		url: '/cgi-bin/setting_lan.asp',
		dataType: 'script',

		error: function(xhr){
			setTimeout("send_setting();", 1000);
		},
		success: function(response){
			;
		}
	});
}

function detect_router(){
	$.ajax({
		url: 'http://'+new_lan_ip+'/cgi-bin/WAN_info.asp',
		dataType: 'script',

		error: function(xhr){
			setTimeout("detect_router();", 1000);
		},
		success: function(response){
			setTimeout("detect_router();", 1000);
		}
	});
}

function get_new_lanip(){
	var MAX_SUBNET = 3232300800;
	var MIN_LAN_IP = 3232235521;
	var reverse_mask_num = ~(inet_aton(current_mask));
	var current_lanip_num = inet_aton(current_lanip);	
	var lan_subnet = current_lanip_num+reverse_mask_num;
	
	if(lan_subnet >= MAX_SUBNET)
		var new_lanip_num = MIN_LAN_IP;
	else
		var new_lanip_num = lan_subnet+1;	

	return inet_ntoa(new_lanip_num);
}

function inet_aton(ip_str){
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

function inet_ntoa(ip_num){
	var v1, v2, v3, v4;
	var ip_str = "";

	if(!ip_num || typeof(ip_num) != "number")
		return null;

	v1 = (ip_num&0xff000000)>>>24;
	v2 = (ip_num&0x00ff0000)>>>16;
	v3 = (ip_num&0x0000ff00)>>>8;
	v4 = (ip_num&0x000000ff);

	ip_str = v1+"."+v2+"."+v3+"."+v4;

	return ip_str;
}

function winW_H(){
	if(parseInt(navigator.appVersion) > 3){
		winW = document.documentElement.scrollWidth;
		if(document.documentElement.clientHeight > document.documentElement.scrollHeight)
			winH = document.documentElement.clientHeight;
		else
			winH = document.documentElement.scrollHeight;
	}
}

function showLoadingBar(seconds){
	htmlbodyforIE = document.getElementsByTagName("html");  //this both for IE&FF, use "html" but not "body" because <!DOCTYPE html PUBLIC.......>
	htmlbodyforIE[0].style.overflow = "hidden";	  //hidden the Y-scrollbar for preventing from user scroll it.

	winW_H();
	var blockmarginTop;
	blockmarginTop = document.documentElement.scrollTop + 200;
	$("#loadingBarBlock")[0].style.marginTop = blockmarginTop+"px";
	$("#LoadingBar")[0].style.width = winW+"px";
	$("#LoadingBar")[0].style.height = winH+"px";

	if(!seconds)
		seconds = 0;
	else
		progress = 100/seconds;

	y = 0;
	LoadingProgress(seconds);
}

function LoadingProgress(seconds){
	$("#LoadingBar")[0].style.visibility = "visible";

	y = y+progress;
	if(typeof(seconds) == "number" && seconds >= 0){
		if(seconds != 0){
			if(Math.round(y) == 100){
				$("#proceeding_img")[0].style.width = "99%";
				$("#proceeding_img_text")[0].innerHTML = "99%";
			}
			else{
				$("#proceeding_img")[0].style.width = Math.round(y) + "%";
				$("#proceeding_img_text")[0].innerHTML = Math.round(y) + "%";
			}
			--seconds;
			setTimeout("LoadingProgress("+seconds+");", 1000);
		}
		else{
			$("#proceeding_img")[0].style.width = "99%";
			$("#proceeding_img_text")[0].innerHTML = "99%";
			y = 0;
		}
	}
}

function hideLoadingBar(){
	$("#LoadingBar")[0].style.visibility = "hidden";
}
</script>
</head>

<body onload="initial();">
<div id="LoadingBar" class="popup_bar_bg">
<table cellpadding="5" cellspacing="0" id="loadingBarBlock" class="loadingBarBlock" align="center">
	<tr>
		<td height="80">
		<div class="Bar_container">
			<span id="proceeding_img_text" ></span>
			<div id="proceeding_img"></div>
		</div>
		<div id="proceeding_action" style="margin:5px auto; width:85%;">Changing the local subnet...</div>
		</td>
	</tr>
</table>
<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
</div>

<br><br>
<table width="500" border="0" align="center" cellspacing="0" class="erTable">
<thead>
  <tr>
    <td height="52" align="left" valign="top"><span class="modelName"><%tcWebApi_get("String_Entry","Web_Title2","s")%></span></td>
  </tr>
</thead>
  <tr>
    <th valign="top" height="60px" background="images/er_bg.gif"><div class="er_title"><%tcWebApi_get("String_Entry","w_r_message","s")%></div></th>
  </tr>


  <tr>
    <td  valign="top" class="erpage_td">
  	  <div class="alertImg"><img src="/images/alertImg.png"></div>
  	  <span class="Epagetitle"><%tcWebApi_get("String_Entry","w_r_fail_r0","s")%> :</span>

	  <div id="failReason" class="Epagecontent" style="color:red">
	    <ul>
		  <li>
		    <span id="failReason1"></span>
		  </li>
	    </ul>
	  </div>

		<div class="Epagetitle">
			<span><%tcWebApi_get("String_Entry","w_r_suggestion0","s")%>: (<%tcWebApi_get("String_Entry","w_r_suggestion0_1","s")%>)</span>
		</div>
	<!--span id="suggestion_title" class="Epagetitle"><%tcWebApi_get("String_Entry","w_r_suggestion0","s")%>: (<%tcWebApi_get("String_Entry","w_r_suggestion0_1","s")%>)</span-->
	<div class="Epagecontent">
		<div id="case1" style="display:none;">
		  <ul>
			 <li><%tcWebApi_get("String_Entry","w_r_suggestion1","s")%></li>
		  </ul>
		</div>

		<div id="case2" style="display:none;">
		  <ul>
			 <li>
			 	<div id="ppp_input_error" style="display:none;">
			 	  <a href="javascript:wanLink();" style="color:#569AC7; text-decoration:underline;"><%tcWebApi_get("String_Entry","w_r_suggestion2_1","s")%></a>
			 	  <span><%tcWebApi_get("String_Entry","w_r_suggestion2_1_desc","s")%></span>
			 	</div>

			 	<div id="ppp_no_response" style="display:none;">
			 	  <a href="javascript:detectLink();" style="color:#569AC7; text-decoration:underline;"><%tcWebApi_get("String_Entry","w_r_suggestion2_2","s")%></a>
			 	  <span><%tcWebApi_get("String_Entry","w_r_suggestion2_2_desc","s")%></span>
			 	</div>
			 </li>
		  </ul>
		</div>

		<div id="case3" style="display:none;">
		  <ul>
			 <li><%tcWebApi_get("String_Entry","w_r_suggestion3","s")%></li>
		  </ul>
		</div>

		<div id="case4" style="display:none;">
		  <ul>
			 <li><%tcWebApi_get("String_Entry","w_r_suggestion4","s")%></li>
		  </ul>
		</div>

		<div id="case5" style="display:none;"></div>

		<div id="case6" style="display:none;">
			<ul>
				<li><%tcWebApi_get("String_Entry","w_r_suggestion6","s")%></li>
			</ul>
		</div>

		<div id="case8" style="display:none;">
			<ul>
				<li><%tcWebApi_get("String_Entry","APS_action_ConnectingStatus0","s")%></li>
			</ul>
		</div>

		<div id="suggestion" style="display:;">
		<ul>
		  <li>
		  	<span id="suggestion_final"><%tcWebApi_get("String_Entry","w_r_suggestion_final","s")%></span>
		  	<a href="javascript:wanLink();" style="color:#569AC7; text-decoration:underline;"><%tcWebApi_get("String_Entry","w_r_suggestion_etc","s")%></a>
		   	<span><%tcWebApi_get("String_Entry","w_r_suggestion_etc_desc","s")%></span>
		  </li>
		</ul>
		</div>
	  </div>
	</td>
  </tr>

  <tr>
    <td height="22" class="erpage_footer"><span></span></td>
  </tr>
</table>
</body>
</html>
