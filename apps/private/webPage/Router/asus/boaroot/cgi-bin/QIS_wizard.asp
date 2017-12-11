<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--QIS_wizard.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QIS_title","s")%></title>
<link rel="stylesheet" type="text/css" href="/other.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<style>
.banner1{
	width:998px;
	height:54px;
	background:url(/images/New_ui/title_bg.png) 0 0 no-repeat;
	margin:0px auto;
}
.qismenu{
	width:130px;
	font-size: 12px;
	font-weight: bold;
	text-shadow: 1px 1px 0px black;
	filter: progid:DXImageTransform.Microsoft.DropShadow(OffX=1, OffY=1, Color=#000000);
}
</style>
<script type="text/JavaScript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var targeturl = "index.asp";
var firmver = "<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>";
var detect_status = '<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>';
var AnnexTypeA_orig = "<%tcWebApi_get("Adsl_Entry","ANNEXTYPEA","s")%>";

var autodet_annex_counter = 0;  //for QIS_detect to switch dslx_annex value
var x_Setting = "<%tcWebApi_get("SysInfo_Entry","x_Setting","s")%>";
var w_Setting_tmp = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";
var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";	//ATM | PTM
var wan_type_info = "<%tcWebApi_get("Info_Adsl","xDSLmode","s")%>";	//ADSL | VDSL
var wan_type_compare = (wan_type_info=="VDSL")? "PTM":"ATM";

function set_step(focus_id){
	document.getElementById("t1").className = "lefttabmid";
	document.getElementById("t2").className = "lefttabmid";
	document.getElementById("t3").className = "lefttabmid";

	switch(focus_id){
		case "t2":
			document.getElementById("t2").className = "lefttabmidfocus";
			break;
		case "t3":
			document.getElementById("t3").className = "lefttabmidfocus";
			break;
		default:	
			document.getElementById("t1").className = "lefttabmidfocus";
			break;
	}
} 

function QIS_load_body(){
	winW_H();
	if(navigator.appName.indexOf("Microsoft") >= 0){
		$("contentM").width = "105%";
		$("contentM").style.marginLeft = "-45px";
	}

	with(document.redirectForm){
		if (x_Setting == "0") {
			action = "qis/QIS_admin_pass.asp";
			submit();
			return;
		}
		else { //x_Setting == 1

		   if(wan_type != wan_type_compare){

			action = "qis/QIS_detect.asp";
			submit();
		   }
		   else{

			if (detect_status == "Fail") {
				if(wan_type_info == "VDSL"){	//wan_type == "PTM"
					action = "qis/QIS_PTM_manual_setting.asp";
				}
				else{
					action = "qis/QIS_manual_setting.asp";
				}
				submit();
				return;
			}

			if ((detect_status == "initializing") || (detect_status == "wait_for_init") || (detect_status == "up")) {
				if(detect_status == "up" && wan_type_info == "VDSL"){	//wan_type == "PTM"
					action = "qis/QIS_PTM_manual_setting.asp";
				}
				else if(detect_status == "up" && wan_type_info == "ADSL" && (AnnexTypeA_orig == "ANNEX B" || AnnexTypeA_orig == "ANNEX B/J" || AnnexTypeA_orig == "ANNEX B/J/M")){	// wan_type == "ATM"
					action = "qis/QIS_manual_setting.asp";
				}
				else{
					action = "qis/QIS_detect.asp";
				}
				submit();
			}
			else if (detect_status == "down") {
				if((model_name == "DSL-N12U-C1") || (model_name == "DSL-N12U-D1")){
					action = "qis/QIS_manual_setting.asp";
				}
				else{
					action = "qis/QIS_annex_setting.asp";
				}
				submit();
			}
			else if ((detect_status == "pppoe") || (detect_status == "pppoa")) { //1:PPPoE, 2:PPPoA
				if(AnnexTypeA_orig == "ANNEX B" || AnnexTypeA_orig == "ANNEX B/J" || AnnexTypeA_orig == "ANNEX B/J/M")
					action = "qis/QIS_manual_setting.asp";
				else
					action = "qis/QIS_ppp_cfg.asp";
				submit();
			}
			else if (detect_status == "dhcp") { //2:MER/Automatic IP
				if(AnnexTypeA_orig == "ANNEX B" || AnnexTypeA_orig == "ANNEX B/J" || AnnexTypeA_orig == "ANNEX B/J/M")
					action = "qis/QIS_manual_setting.asp";
				else
					action = "qis/QIS_mer_cfg.asp";
				submit();
			}
			else if (detect_status == "") { //Unknown state
				var link_sts = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
				var link_mode = "<%tcWebApi_get("Info_Adsl","Opmode","s")%>";
				if(link_sts == "up")
				{
					var PTM_mode = link_mode.search(/VDSL/);
					if(PTM_mode >= 0)
						action = "qis/QIS_PTM_manual_setting.asp";
					else
						action = "qis/QIS_manual_setting.asp";
				}
				else{
					if((model_name == "DSL-N12U-C1") || (model_name == "DSL-N12U-D1")){
						action = "qis/QIS_manual_setting.asp";
					}
					else{
						action = "qis/QIS_annex_setting.asp";
					}
				}
				submit();
			}
			else {
				if((model_name == "DSL-N12U-C1") || (model_name == "DSL-N12U-D1")){
					action = "qis/QIS_manual_setting.asp";
				}
				else{
					action = "qis/QIS_annex_setting.asp";
				}
				submit();
			}

		   }
			return;
		}
	}
}

function gotoIndex(){	
	if(w_Setting_tmp == '0'){
		alert("<%tcWebApi_get("String_Entry","QIS_recommand_encryption","s")%>");
		with(document.redirectForm){
			action = "qis/QIS_wireless.asp";
			submit();
			return;			
		}
	}
	else
		location.href = '/index2.asp';
}
</script>
</head>
<body class="LevelQIS" onload="QIS_load_body();">
<noscript>
	<div class="popup_bg" style="visibility:visible; z-index:999;">
	<div style="margin:200px auto; width:300px; background-color:#006699; color:#FFFFFF; line-height:150%; border:3px solid #FFF; padding:5px;"><%tcWebApi_get("String_Entry","not_support_script","s")%></p></div>
	</div>
</noscript>
<div id="LoadingBar" class="popup_bar_bg">
	<table cellpadding="5" cellspacing="0" id="loadingBarBlock" class="loadingBarBlock" align="center">
		<tr>
			<td height="80">
				<div class="Bar_container">
					<span id="proceeding_img_text"></span>
					<div id="proceeding_img"></div>
				</div>
				<div style="margin:5px auto; width:85%;"><%tcWebApi_get("String_Entry","APS_msg_waiting_to_connect","s")%>
					<span style="font-weight:bolder;" id="stassid"></span>
				</div>
			</td>
		</tr>
	</table>
</div>
<div id="Loading" class="popup_bg">
	<table cellpadding="5" cellspacing="0" id="loadingBlock" class="loadingBlock" align="center">
		<tr>
			<td width="20%" height="80" align="center"><img src="/images/loading.gif"></td>
			<td><span id="proceeding_main_txt"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%></span><span id="proceeding_txt" style="color:#FFFFCC;"></span></td>
		</tr>
	</table>
<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->	
</div>
<div id="hiddenMask" class="popup_bg">
	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
		<tr>
			<td>
				<div class="drword" id="drword"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
					<br/>
					<br/>
				</div>
				<div class="drImg"><img src="/images/alertImg.png"></div>
				<div style="height:70px; "></div>
			</td>
		</tr>
	</table>
</div>

<!--For content in ifame-->	
<!--[if !lte IE 6]>-->
<div class="banner1" align="center">
	<img src="/images/New_ui/asustitle.png" width="218" height="54" align="left">
	<span class="modelName_top" style="margin-top:20px;"><%tcWebApi_get("String_Entry","Web_Title2","s")%></span>
</div>
<!--<![endif]-->
<!--[if lte IE 6]>
<div class="banner1" align="center">
	<img src="/images/New_ui/asustitle_ie6.png" width="218" height="54" align="left">
	<span class="modelName_top" style="margin-top:20px;"><%tcWebApi_get("String_Entry","Web_Title2","s")%></span>
</div>
<![endif]-->

<table width="998px" border="0" align="center" cellpadding="0" cellspacing="0">
	<tr>
		<td height="170px" width="998px" valign="top" background="/images/New_ui/midup_bg.png"></td>
	</tr>
</table>
<table width="998px" border="0" align="center" cellpadding="0" cellspacing="0" background="/images/New_ui/middown_bg.png">
	<tr>
		<td style="width:218px;*width:263px;" valign="top">			
			<div class="m_qis_r" id="btn_gotoindex" style="margin-left: 15px;margin-top: -160px;cursor:pointer;" onclick="gotoIndex();"><table><tbody><tr><td><div class="_index"></div></td><td><div><%tcWebApi_get("String_Entry","CTL_Setting","s")%></div></td></tr></tbody></table></div>
			<div class="lefttabhead" style="margin-left:15px;" id="option0"><table width="192px" height="37px"><tbody><tr><td><%tcWebApi_get("String_Entry","QIS_title","s")%></td></tr></tbody></table></div>			
			<div class="lefttabmid" id="t1" style="margin-left:15px;"><table height="52px"><tbody><tr><td><img style="margin-left:3px;" border="0" src="/images/qis/qisStep1.png"></td><td><div class="qismenu"><%tcWebApi_get("String_Entry","QIS_step2","s")%></div></td></tr></tbody></table></div>
			<div class="lefttabmid" id="t2" style="margin-left:15px;"><table height="52px"><tbody><tr><td><img style="margin-left:3px;" border="0" src="/images/qis/qisStep2.png"></td><td><div class="qismenu"><%tcWebApi_get("String_Entry","QIS_step3","s")%></div></td></tr></tbody></table></div>
			<div class="lefttabmid" id="t3" style="margin-left:15px;"><table height="52px"><tbody><tr><td><img style="margin-left:3px;" border="0" src="/images/qis/qisStep3.png"></td><td><div class="qismenu"><%tcWebApi_get("String_Entry","QIS_step4","s")%></div></td></tr></tbody></table></div>
		</td>
		<td>
			<div class="_contentM_qis"><iframe name="contentM" id="contentM" class="contentM_qis" scrolling="auto" width="99%" height="540" frameborder="0"></iframe></div>
		</td>
		<td width="8px" height="365" valign="top"></td>
	</tr>
	<tr>
		<td height="27" colspan="3" style="background: url('/images/New_ui/bottom_bg.png') no-repeat">&nbsp;</td>
	</tr>
</table>
<form method="post" name="QKform">
	<input type="hidden" name="flag" id="flag" value=''>
	<input type="hidden" name="prev_page" value="">
	<input type="hidden" name="current_page" value="QIS_wizard.asp">
	<input type="hidden" name="next_page" value="">
	<input type="hidden" name="action_mode" value="apply">
	<input type="hidden" name="action_script" value="restart_wan_if">
	<input type="hidden" name="wan_unit" value="0">
	<input type="hidden" name="action_wait" value="5">
	<input type="hidden" name="modified" value="">
</form>
<form method="post" name="redirectForm" action="" target="contentM">
	<input type="hidden" name="flag" value="">
	<input type="hidden" name="prev_page" value="">
</form>
</body>

<!--QIS_wizard.asp-->
</html>

