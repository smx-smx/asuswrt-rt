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
<link rel="stylesheet" type="text/css" href="/css/confirm_block.css"></script>
<style>
.FormTable{
	margin-top:10px;	
}	
.Bar_container{
	width:85%;
	height:21px;
	border:2px inset #999;
	margin:0 auto;
	margin-top:20px \9;
	background-color:#FFFFFF;
	z-index:100;
}
#proceeding_img_text{
	position:absolute; 
	z-index:101; 
	font-size:11px; 
	color:#000000; 
	margin-left:110px; 
	line-height:21px;
}
#proceeding_img{
	height:21px;
	background:#C0D1D3 url(/images/proceeding_img.gif);
}
.button_helplink{
	font-weight: bolder;
	text-shadow: 1px 1px 0px black;
	text-align: center;
	vertical-align: middle;
  background: transparent url(/images/New_ui/contentbt_normal.png) no-repeat scroll center top;
  _background: transparent url(/images/New_ui/contentbt_normal_ie6.png) no-repeat scroll center top;
  border:0;
  color: #FFFFFF;
	height:33px;
	width:122px;
	font-family:Verdana;
	font-size:12px;
  overflow:visible;
	cursor:pointer;
	outline: none; /* for Firefox */
 	hlbr:expression(this.onFocus=this.blur()); /* for IE */
 	white-space:normal;
}
.button_helplink:hover{
	font-weight: bolder;
	background:url(/images/New_ui/contentbt_over.png) no-repeat scroll center top;
	height:33px;
 	width:122px;
	cursor:pointer;
	outline: none; /* for Firefox */
 	hlbr:expression(this.onFocus=this.blur()); /* for IE */
}
</style>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/confirm_block.js"></script>
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
var webs_state_flag = '<% tcWebApi_get("WebCustom_Entry", "webs_state_flag", "s" ) %>';

var confirm_show = '<% get_parameter("confirm_show") %>';
var webs_release_note= "";

var helplink = "";
var dpi_engine_status = <%bwdpi_engine_status();%>;	// {DpiEngine:"1", SigVersion:"1.008"}
var sig_update_t = '<% tcWebApi_get("Vram_Entry", "sig_update_t", "s" ) %>';

function initial(){
	show_menu();
	if(bwdpi_support){
		if(dpi_engine_status.DpiEngine == 1)
			document.getElementById("sig_ver_field").style.display="";
		else
			document.getElementById("sig_ver_field").style.display="none";
			
		var sig_ver = dpi_engine_status.SigVersion;
		if(sig_ver == "")
			document.getElementById("sig_ver_word").innerHTML = "1.008";
		else
			document.getElementById("sig_ver_word").innerHTML = sig_ver;

		
		if(sig_update_t == "" || sig_update_t == "0")
			document.getElementById("sig_update_date").innerHTML = "";
		else
			document.getElementById("sig_update_date").innerHTML = "&nbsp;&nbsp;"+transferTimeFormat(sig_update_t*1000);
	}
		
	if(webs_state_upgrade != "" && webs_state_upgrade != "1"){   //Show firmware is downloading or fw upgrade loading bar if doing webs_upgrade.sh 
		startDownloading();
	}
		
	if(live_update_support == -1 || HTTPS_support == -1){
		document.getElementById("update_div").style.display = "none";
		document.getElementById("linkpage_div").style.display = "";
		document.getElementById("linkpage").style.display = "";
		helplink = get_helplink();
		document.getElementById("linkpage").href = helplink;
	} 
	else{
		document.getElementById("update_div").style.display = "";
		document.getElementById("linkpage_div").style.display = "none";
		if(confirm_show.length > 0 && confirm_show == 0){
			do_show_confirm(webs_state_flag);
		}
	}	
}


var dead=0;
var note_display=0;	//formal path
var check_webs_state_info="";
function detect_firmware(flag){

	$j.ajax({
    		url: '/detect_firmware.asp',
    		dataType: 'script',

    		error: function(xhr){
    			dead++;
					if(dead < 30){
						setTimeout("detect_firmware();", 1000);
					}	
					else{
						document.getElementById('update_scan').style.display="none";
						document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","info_failed","s")%>";
					}
			
				},

    		success: function(){

      			if(webs_state_update==0){
      				setTimeout("detect_firmware();", 1000);
      			}
      			else{      				
      				if(webs_state_error==1 && webs_state_info==""){
      					document.getElementById('update_scan').style.display="none";
      					document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","info_failed","s")%>";
      					return;
      					
      				}
      				else if(webs_state_error==1 && webs_state_info != ""){
      					document.getElementById('update_scan').style.display="none";
      					document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","FIRM_fail_desc","s")%>";
      					return;
      					
      				}
      				else{
      					check_webs_state_info = webs_state_info;
						note_display=0;
						do_show_confirm(webs_state_flag);
							
					}	
				return;
				}
			}		
	});
}

function do_show_confirm(flag){

		if(flag==1 || flag==2){
				document.getElementById('update_scan').style.display="none";
				document.getElementById('update_states').style.display="none";				
				confirm_asus({
						title: "New Firmware Available",
						contentA: "<%tcWebApi_get("String_Entry","exist_new","s")%><br>",
						contentC: "<br><%tcWebApi_get("String_Entry","ADSL_FW_note","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc5","s")%>",
						left_button: (flag==2)? "<%tcWebApi_get("String_Entry","CTL_UpgradeNight","s")%>":"<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>",
						left_button_callback: function(){confirm_cancel();},
						left_button_args: {},
						right_button: (flag==2)? "<%tcWebApi_get("String_Entry","CTL_UpgradeNow","s")%>":"<%tcWebApi_get("String_Entry","CTL_upgrade","s")%>",
						right_button_callback: function(){
							document.start_update.action_mode.value="apply";
							document.start_update.action_script.value="start_webs_upgrade";
							document.start_update.live_upgrade_flag.value="1";
							document.start_update.DOWNLOAD_HEADER_TYPE.value="1";
							startDownloading();
							document.start_update.submit();
						},
						right_button_args: {},
						iframe: "get_release_note0.asp",
						margin: "100px 0px 0px 25px",
						note_display_flag: note_display
				});  		     				
		}
		else{
				document.getElementById('update_scan').style.display="none";
				document.getElementById('update_states').style.display="";
				document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","is_latest","s")%>";

		}	
}

function detect_update(){	
  document.start_update.action_mode.value="apply";
  document.start_update.action_script.value="frs_update";
  document.start_update.action_wait.value="60";  

	if(document.getElementById("connect_status").className == "connectstatusoff"){	//WAN disconnected
		document.getElementById('update_scan').style.display="none";
		document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","connect_failed","s")%>";
	}	
	else{
		document.start_update.live_update_flag.value="1";
		document.getElementById('update_states').innerHTML="<%tcWebApi_get("String_Entry","check_proceeding","s")%>";
		document.getElementById('update_scan').style.display="";
		document.start_update.submit();
	}
}

function sig_version_check(){
	document.sig_update.action_mode.value="apply";
	document.sig_update.action_script.value="start_sig_check";
	document.sig_update.action_wait.value="60";  

	if(document.getElementById("connect_status").className == "connectstatusoff"){	//WAN disconnected
		$j("#sig_status").html("The router cannot connect to ASUS server to check for the signature update. After reconnecting to the Internet, go back to this page and click Check to check for the latest signature updates");	/* Untranslated */
	}	
	else{
		document.getElementById("sig_check").disabled = true;
		document.sig_update.sig_update_flag.value="1";
		$j("#sig_status").show();
		document.sig_update.submit();
		$j("#sig_status").html("Signature checking ...");	/* Untranslated */
		document.getElementById("sig_update_scan").style.display = "";
		setTimeout("sig_check_status();", 8000);
	}
}

var sdead=0;
function sig_check_status(){
	$j.ajax({
    	url: '/detect_firmware.asp',
    	dataType: 'script',
	timeout: 3000,
    	error:	function(xhr){
			sdead++;
			if(sdead < 20){				
				setTimeout("sig_check_status();", 1000);
			}
			else{
				return;
			}
    		},
    	success: function(){
			$j("#sig_status").show();
			if(sig_state_flag == 0){		// no need upgrade
				$j("#sig_status").html("Signature is up to date");	/* Untranslated */
				document.getElementById("sig_update_scan").style.display = "none";
				document.getElementById("sig_check").disabled = false;
			}
			else if(sig_state_flag == 1){
				if(sig_state_error != 0){		// update error
					$j("#sig_status").html("Signature update failed");	/* Untranslated */
					document.getElementById("sig_update_scan").style.display = "none";
					document.getElementById("sig_check").disabled = false;
				}
				else{
					if(sig_state_upgrade == 1){		//update complete
						$j("#sig_status").html("Signature update completely");	/* Untranslated */
						document.getElementById("sig_update_scan").style.display = "none";
						$j("#sig_ver").html(sig_ver);
						document.getElementById("sig_check").disabled = false;
					}
					else{		//updating
						$j("#sig_status").html("Signature is updating");	/* Untranslated */
						setTimeout("sig_check_status();", 1000);
					}				
				}			
			}
  		}
  	});
}

function redirect(){
	document.location.href = "/index2.asp";
}

function uiDoUpdate()
{
	var form=document.uiPostUpdateForm;
	var string4 = form.tools_FW_UploadFile.value.search(/<%ej_get_productid()%>/);

	if (form.tools_FW_UploadFile.value=="") {
		alert("You must select a firmware file to upload.");
	}
	else {
		if (string4 >= 0) {
			form.postflag.value = "1";
			if(model_name == "DSL-N66U" || model_name == "DSL-AC52U")
			{
				showLoading(220);
				setTimeout("redirect();", 220000);
			}
			else if(model_name == "DSL-N55U-C1" || model_name == "DSL-N16U")
			{
				showLoading(152);
				setTimeout("redirect();", 152000);
			}
			else //DSL-N14U ...
			{
				showLoading(182);
				setTimeout("redirect();", 182000);
			}
			setTimeout("chk_upgrade();", 5000);
			form.submit();
		}
		else
			alert("<%tcWebApi_get("String_Entry","FW_item5","s")%> <%tcWebApi_get("String_Entry","Manual_Setting_JS_invalid","s")%>");
	}
}

var dead = 0;
function detect_httpd(){
	$j.ajax({
    		url: '/cgi-bin/httpd_check.xml?hash=' + Math.random().toString(),
    		dataType: 'xml',
		timeout: 1500,
    		error: function(xhr){
    				dead++;
    				if(dead < 6)
    					setTimeout("detect_httpd();", 1000);
    				else{
    					document.getElementById('loading_block1').style.display = "none";
    					document.getElementById('loading_block2').style.display = "none";
    					document.getElementById('loading_block3').style.display = "";
    					document.getElementById('loading_block3').innerHTML = "<div><%tcWebApi_get("String_Entry","Firm_reboot_manually","s")%></div>";
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
				document.getElementById("hiddenMask").style.visibility = "hidden";
				showLoadingBar(180);
			}
			else
				rebooting++
    			},
    		success: function(){
					rebooting = 0;
					if(webs_state_error == 1){
						document.getElementById("drword").innerHTML = "<%tcWebApi_get("String_Entry","FIRM_fail_desc","s")%><br><%tcWebApi_get("String_Entry","FW_desc1","s")%>";
						return false;
					}
					else if(webs_state_error == 2){
						document.getElementById("drword").innerHTML = "Memory space is NOT enough to upgrade on internet. Please wait for rebooting.<br><%tcWebApi_get("String_Entry","FW_desc1","s")%>";
						return false;
					}
					else if(webs_state_upgrade != 0 && webs_state_error == 0){
						document.getElementById("hiddenMask").style.visibility = "hidden";
						if(model_name == "DSL-N66U" || model_name == "DSL-AC52U"){
							showLoadingBar(220);
							setTimeout("detect_httpd();", 220000);
						}
						else if(model_name == "DSL-N55U-C1" || model_name == "DSL-N16U"){
							showLoadingBar(152);
							setTimeout("detect_httpd();", 152000);
						}
						else{	 //DSL-N14U ...else
							showLoadingBar(182);
							setTimeout("detect_httpd();", 182000);
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
				document.getElementById("drword").innerHTML = "<%tcWebApi_get("String_Entry","FIRM_fail_desc","s")%><br><%tcWebApi_get("String_Entry","FW_desc1","s")%>";
				return false;
			}
			else{
				
				document.getElementById("hiddenMask").style.visibility = "hidden";
				if(model_name == "DSL-N66U" || model_name == "DSL-AC52U"){
					showLoadingBar(220);
					setTimeout("detect_httpd();", 220000);
				}
				else if(model_name == "DSL-N55U-C1" || model_name == "DSL-N16U"){
					showLoadingBar(152);
					setTimeout("detect_httpd();", 152000);
				}
				else{	 //DSL-N14U ...else
					showLoadingBar(182);
					setTimeout("detect_httpd();", 182000);
				}
				
				document.redirectForm.live_do_upgrade_flag.value = "1";
				setTimeout("document.redirectForm.submit();", 15000);
			}			
}

function startDownloading(){
	disableCheckChangedStatus();
	dr_advise();
	document.getElementById("drword").innerHTML = "&nbsp;&nbsp;&nbsp;<%tcWebApi_get("String_Entry","fw_downloading","s")%>...";
}

var dead_sts =0;
function chk_upgrade(){
	$j.ajax({
    		url: '/cgi-bin/detect_firmware_sts.asp',
    		dataType: 'script',
		timeout: 1500,
    		error: function(xhr){
    			dead_sts++;
    			if(dead_sts < 60)    				
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
				else if(upgrade_sts == "FLASH_NOT_SUPPORT")
				{
					hideLoading();
					showLoading(120, "upgrade_FLASH_NOT_SUPPORT");
					setTimeout("redirect();", 120000);
				}
				else
					setTimeout("chk_upgrade();", 1000);
  			}
  	});
}

function LoadingProgress(seconds){
	
	document.getElementById("LoadingBar").style.visibility = "visible";
	y = y + progress;
	if(typeof(seconds) == "number" && seconds >= 0){
		if(seconds != 0){
			document.getElementById("proceeding_img").style.width = Math.round(y) + "%";
			document.getElementById("proceeding_img_text").innerHTML = Math.round(y) + "%";
			if(document.getElementById("loading_block1")){
				document.getElementById("proceeding_img_text").style.width = document.getElementById("loading_block1").clientWidth;
				document.getElementById("proceeding_img_text").style.marginLeft = "175px";
			}	
			--seconds;
			setTimeout("LoadingProgress("+seconds+");", 1000);
		}
		else{
			document.getElementById("proceeding_img_text").innerHTML = "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>";
			y = 0;
			if(location.pathname.indexOf("QIS_wizard.asp") < 0){
				setTimeout("hideLoadingBar();",1000);
				location.href = "index.asp";
			}
		}
	}
}

function transferTimeFormat(time){
	if(time == 0){
		return "";
	}

	var t = new Date();
	t.setTime(time);
	var year = t.getFullYear();
	var month = t.getMonth() + 1;
	if(month < 10){
		month  = "0" + month;
	}
	
	var date = t.getDate();
	if(date < 10){
		date = "0" + date;
	}
	
	var hour = t.getHours();
	if(hour < 10){
		hour = "0" + hour;
	}
			
	var minute = t.getMinutes();
	if(minute < 10){
		minute = "0" + minute;
	}

	var date_format = "Updated : " + year + "/" + month + "/" + date + " " + hour + ":" + minute;
	return date_format;
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
		<div id="loading_block2" style="margin:5px auto; width:85%;"><%tcWebApi_get("String_Entry","FIRM_ok_desc","s")%><br><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc5","s")%></div>
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
<div class="drword" id="drword" style="height:50px;">&nbsp;&nbsp;&nbsp;<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
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
<FORM ENCTYPE="multipart/form-data" METHOD="POST" name="uiPostUpdateForm" action="/Advanced_FirmwareUpgrade_Content.asp" target="hidden_frame">
<INPUT TYPE="HIDDEN" NAME="postflag" VALUE="0">
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
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","menu5_6_3","s")%></div>
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
			<thead>
			<tr>
				<td colspan="2"><%tcWebApi_get("String_Entry","FW_item2","s")%></td>	
			</tr>	
			</thead>
			<tr>
				<th><%tcWebApi_get("String_Entry","FW_item1","s")%></th>
				<td><%tcWebApi_get("String_Entry","Web_Title2","s")%></td>
			</tr>
			<tr>
				<th><% tcWebApi_Get("String_Entry", "adsl_fw_ver_in", "s") %></th>
				<td><%If tcWebApi_get("Info_Adsl","fwVer","h") <> "" then tcWebApi_staticGet("Info_Adsl","fwVer","s") end if%></td>
			</tr>
			<tr id="sig_ver_field" style="display:none">
				<th>Signature Version</th>
				<td >
					<div style="height:33px;margin-top:5px;"><span id="sig_ver_word" style="color:#FFFFFF;"></span><span id="sig_update_date"></span></div>
					<div style="margin-left:217px;margin-top:-38px;">
						<input type="button" id="sig_check" name="sig_check" class="button_gen" onclick="sig_version_check();" value='<%tcWebApi_get("String_Entry","liveupdate","s")%>'>
					</div>
					<div>
						<span id="sig_status" style="display:none"></span>
						<img id="sig_update_scan" style="display:none;" src="/images/InternetScan.gif">
					</div>
				</td>
			</tr>
			<tr>
				<th><%tcWebApi_get("String_Entry","FW_item2","s")%></th>
				<td><div style="height:33px;margin-top:5px;"><%If tcWebApi_get("DeviceInfo","FwVer","h") <> "" Then tcWebApi_staticGet("DeviceInfo","FwVer","s") end if%></div>
						<div id="update_div" style="margin-left:216px;margin-top:-38px;display:none;">
							<input type="button" id="update" name="update" class="button_gen" onclick="detect_update();" value="<%tcWebApi_get("String_Entry","liveupdate","s")%>" />
						</div>		
						<div id="linkpage_div" class="button_helplink" style="margin-left:216px;margin-top:-38px;display:none;">
							<a id="linkpage" target="_blank"><div style="padding-top:5px;"><%tcWebApi_get("String_Entry","liveupdate","s")%></div></a>
						</div>
						<div id="check_states">
							<span id="update_states"></span>
							<img id="update_scan" style="display:none;" src="/images/InternetScan.gif" />
						</div>
				</td>
			</tr>
			<tr>
				<th><%tcWebApi_get("String_Entry","FW_item5","s")%></th>
				<td><input type="file" name="tools_FW_UploadFile" class="input" style="color:#FFCC00;width:210px;">
						<input type="button" name="FW_apply" class="button_gen" onclick="uiDoUpdate();" value="<%tcWebApi_get("String_Entry","CTL_upload","s")%>" />
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

<form method="post" name="sig_update" action="/cgi-bin/live_update.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_FirmwareUpgrade_Content.asp">
<input type="hidden" name="next_page" value="Advanced_FirmwareUpgrade_Content.asp">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="sig_update_flag" value="0">
</form>

</body>
<!--Advanced_FirmwareUpgrade_Content.asp-->
</html>

