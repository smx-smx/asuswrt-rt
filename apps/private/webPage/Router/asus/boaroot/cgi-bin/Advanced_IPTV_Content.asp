<%
If Request_Form("IPTV_flag") = "1" Then
	TCWebApi_set("IPTV_Entry","ad_mr_enable_x","igmp_enable_x")
	TCWebApi_set("IPTV_Entry","ad_pim_enable_x","pim_enable_x")
	TCWebApi_set("IPTV_Entry","ad_wan_port","pvc_service_num")
	TCWebApi_set("IPTV_Entry","ad_udpxy_enable_x","udpxy_enable_x")
	TCWebApi_set("IPTV_Entry","ad_rmvlan","rmvlan_x")
	TCWebApi_set("IPTV_Entry","TransMode","wan_TransMode")
	TCWebApi_commit("IPTV_Entry")
Elseif Request_Form("IPTV_flag") = "2" Then
	TCWebApi_set("IPTV_Entry","vd_mr_enable_x","igmp_enable_x")
	TCWebApi_set("IPTV_Entry","vd_pim_enable_x","pim_enable_x")
	TCWebApi_set("IPTV_Entry","vd_wan_port","pvc_service_num")
	TCWebApi_set("IPTV_Entry","vd_udpxy_enable_x","udpxy_enable_x")
	TCWebApi_set("IPTV_Entry","vd_rmvlan","rmvlan_x")
	TCWebApi_set("IPTV_Entry","TransMode","wan_TransMode")
	TCWebApi_commit("IPTV_Entry")
Elseif Request_Form("IPTV_flag") = "3" Then
	TCWebApi_set("IPTV_Entry","eth_mr_enable_x","igmp_enable_x")
	TCWebApi_set("IPTV_Entry","eth_pim_enable_x","pim_enable_x")
	TCWebApi_set("IPTV_Entry","eth_wan_port","pvc_service_num")
	TCWebApi_set("IPTV_Entry","eth_udpxy_enable_x","udpxy_enable_x")
	TCWebApi_set("IPTV_Entry","eth_rmvlan","rmvlan_x")
	TCWebApi_set("IPTV_Entry","TransMode","wan_TransMode")
	TCWebApi_commit("IPTV_Entry")
End If
If Request_Form("snoop_flag") = "1" Then
	TCWebApi_set("Lan_IgmpSnoop","igmpsnoopEnable","igmpsnoopEnable")
	TCWebApi_commit("Lan_IgmpSnoop")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_IPTV_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - IPTV</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/form.js"></script>
<style type="text/css">
.contentM_qis{
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:200;
	background-color:#2B373B;
	display:block;
	margin-left: 300px;
	margin-top: 280px;
	width:540px;
	box-shadow: 3px 3px 10px #000;
	display:none;
}
</style>
<script>

var $j = jQuery.noConflict();
window.onresize = function() {
	if(document.getElementById("igmp_panel").style.display == "block") {
		cal_panel_block("igmp_panel", 0.35);
	}
}

var iptv_list_ad = [<%get_iptv_wan_list_ad()%>];
var iptv_list_vd = [<%get_iptv_wan_list_vd()%>];
var iptv_list_eth = [<%get_iptv_wan_list_eth()%>];

var wans_dualwan_orig = '<%tcWebApi_Get("Dualwan_Entry","wans_dualwan","s")%>';
var isMultiService_supported = '<%tcWebApi_Get("WebCustom_Entry","isMultiSerSupported","s")%>';
var isPIMDSupport = '<%tcWebApi_Get("WebCustom_Entry","isPIMDSupport","s")%>';
var ad_mr_enable_x =  '<% tcWebApi_get("IPTV_Entry","ad_mr_enable_x","s") %>';
var vd_mr_enable_x =  '<% tcWebApi_get("IPTV_Entry","vd_mr_enable_x","s") %>';
var eth_mr_enable_x =  '<% tcWebApi_get("IPTV_Entry","eth_mr_enable_x","s") %>';
var ad_pim_enable_x = '<% tcWebApi_get("IPTV_Entry","ad_pim_enable_x","s") %>';
var vd_pim_enable_x = '<% tcWebApi_get("IPTV_Entry","vd_pim_enable_x","s") %>';
var eth_pim_enable_x = '<% tcWebApi_get("IPTV_Entry","eth_pim_enable_x","s") %>';
var iptv_wan_TransMode = '<% tcWebApi_get("IPTV_Entry","TransMode","s") %>';

function initial(){
	show_menu();	
	load_TransMode();
	load_MulticastRouting();
	if(wans_dualwan_orig == "lan none" || wans_dualwan_orig == "usb none" ||
		wans_dualwan_orig == "lan usb" || wans_dualwan_orig == "usb lan"){
		doTransChange("disabled");
	}	
}

function load_TransMode(){
	if(iptv_list_ad.length > 0){
		if(iptv_wan_TransMode == "ATM") {
			add_option(document.form.wan_TransMode, "ADSL WAN (ATM)", "ATM", 1);
		}
		else {
			add_option(document.form.wan_TransMode, "ADSL WAN (ATM)", "ATM", 0);
		}
	}	
	if(iptv_list_vd.length > 0){
		if(iptv_wan_TransMode == "PTM") {
			add_option(document.form.wan_TransMode, "VDSL WAN (PTM)", "PTM", 1);
		}
		else {
			add_option(document.form.wan_TransMode, "VDSL WAN (PTM)", "PTM", 0);
		}
	}	
	if(iptv_list_eth.length > 0){
		if(iptv_wan_TransMode == "Ethernet") {
			add_option(document.form.wan_TransMode, "Ethernet WAN", "Ethernet", 1);
		}
		else {
			add_option(document.form.wan_TransMode, "Ethernet WAN", "Ethernet", 0);
		}
	}

	if(document.form.wan_TransMode.length == 0){
		if("<% tcWebApi_get("Wan_Common","TransMode","s") %>" == "ATM"){
			add_option(document.form.wan_TransMode, "ADSL WAN (ATM)", "ATM", 1);
			doTransChange("ATM");
			if(iptv_list_ad.length == 0){	//Default setting
				document.form.mr_enable_x.value = "0";
				document.form.mr_enable_x[1].disabled = true;
				if(isPIMDSupport == "Yes")
					document.form.mr_enable_x[2].disabled = true;
				enable_wan_port("0");
			}	
	
		}
		else if("<% tcWebApi_get("Wan_Common","TransMode","s") %>" == "PTM"){
			add_option(document.form.wan_TransMode, "VDSL WAN (ATM)", "PTM", 1);
			doTransChange("PTM");
			if(iptv_list_vd.length == 0){	//Default setting
				document.form.mr_enable_x.value = "0";
				document.form.mr_enable_x[1].disabled = true;
				if(isPIMDSupport == "Yes")
					document.form.mr_enable_x[2].disabled = true;
				enable_wan_port("0");
			}
		}
		else if("<% tcWebApi_get("Wan_Common","TransMode","s") %>" == "Ethernet"){
			add_option(document.form.wan_TransMode, "Ethernet WAN", "Ethernet", 1);
			doTransChange("Ethernet");
			if(iptv_list_eth.length == 0){	//Default setting
				document.form.mr_enable_x.value = "0";
				document.form.mr_enable_x[1].disabled = true;
				if(isPIMDSupport == "Yes")
					document.form.mr_enable_x[2].disabled = true;
				enable_wan_port("0");
			}
		}			
	}
	else{
		doTransChange(document.form.wan_TransMode.value);
	}	
}

function load_MulticastRouting(){
	if(isPIMDSupport == "Yes"){
		var selected;
		if(document.form.wan_TransMode.value == "ATM" && ad_pim_enable_x == "1"
			|| document.form.wan_TransMode.value == "PTM" && vd_pim_enable_x == "1"
			|| document.form.wan_TransMode.value == "Ethernet" && eth_pim_enable_x == "1"
		) {
			selected = 1;
		}
		else
			selected = 0;
		add_option(document.form.mr_enable_x, "PIM", "2", selected);
	}
}

function compile_multicast_routing(){
	if(document.form.mr_enable_x){
		if(document.form.mr_enable_x.value == "1"){
			document.form.igmp_enable_x.value = "1";
			document.form.pim_enable_x.value = "0";
		}
		else if(document.form.mr_enable_x.value == "2"){
			document.form.igmp_enable_x.value = "0";
			document.form.pim_enable_x.value = "1";
		}
	}
}

function applyRule(){
	if(validForm()){
		loadingtime = 5;
		compile_multicast_routing();
		if(document.form.igmpsnoopEnable.value != '<%tcWebApi_Get("Lan_IgmpSnoop","igmpsnoopEnable","s")%>') {
			document.form.snoop_flag.value = "1";
			loadingtime += 20;
		}
		showLoading(loadingtime);
		setTimeout("redirect();", loadingtime*1000);
		document.form.submit();
	}
}
function redirect(){
	document.location.href = "/Advanced_IPTV_Content.asp";
}
function validForm(){
	if(!validator.rangeAllowZero(document.form.udpxy_enable_x, 1024, 65535, 0)){
		return false;	
	}
		
	return true;
}
function doTransChange(mode){
	if(mode == "disabled"){
		Setting_load_disabled();
	}
	else if(mode == "ATM"){
		Setting_load_ATM();
	}
	else if(mode == "PTM"){
		Setting_load_PTM();
	}	
	else{	//Ethernet
		Setting_load_Ethernet();
	}
}

function Setting_load_disabled(){
	
	document.getElementById("lan_none_desc").style.display = "";
	document.form.wan_TransMode.disabled = true;
	document.form.mr_enable_x.disabled = true;
	document.form.pvc_service_num.disabled = true;
	document.form.udpxy_enable_x.disabled = true;
	document.form.rmvlan_x.disabled = true;
	document.form.wan_TransMode.style.backgroundColor = "#CCCCCC";
	document.form.mr_enable_x.style.backgroundColor = "#CCCCCC";
	document.form.pvc_service_num.style.backgroundColor = "#CCCCCC";
	document.form.udpxy_enable_x.style.backgroundImage = "url(/images/New_ui/inputbg_disable.png)";
	document.form.rmvlan_x.style.backgroundColor = "#CCCCCC";
	document.getElementById("button_gen_div").style.display = "none";
}

/*
[mr_enable_x]  0 (disable) / 1 (IGMP Proxy) / 2 (PIM) :: ad_mr_enable_x, vd_mr_enable_x, eth_mr_enable_x / ad_pim_enable_x, vd_pim_enable_x, eth_pim_enable_x
[pvc_service_num]  0~7 :: ad_wan_port, vd_wan_port, eth_wan_port
[udpxy_enable_x]  1024 ~ 65535 :: ad_udpxy_enable_x, vd_udpxy_enable_x, eth_udpxy_enable_x
[rmvlan_x]  0 (disable) / 1 (enable) :: ad_rmvlan, vd_rmvlan, eth_rmvlan
*/

function Setting_load_ATM(){
	if(ad_mr_enable_x == "1" && ad_pim_enable_x == "0")
		document.form.mr_enable_x.value = "1";
	else if(ad_mr_enable_x == "0" && ad_pim_enable_x == "1")
		document.form.mr_enable_x.value = "2";
	else
		document.form.mr_enable_x.value = "0";	
		
	enable_wan_port(document.form.mr_enable_x.value);
	free_options(document.form.pvc_service_num);
	for(var i=0; i<iptv_list_ad.length; i++){
		var pvc_service_num_temp = eval(iptv_list_ad[i]+1);
		if(iptv_list_ad[i] == "<% tcWebApi_get("IPTV_Entry","ad_wan_port","s") %>"){
			add_option(document.form.pvc_service_num, "PVC "+pvc_service_num_temp, iptv_list_ad[i], 1);
		}	
		else{
			add_option(document.form.pvc_service_num, "PVC "+pvc_service_num_temp, iptv_list_ad[i], 0);
		}	
	}	
	document.form.udpxy_enable_x.value = "<% tcWebApi_get("IPTV_Entry","ad_udpxy_enable_x","s") %>";
	inputCtrl(document.form.rmvlan_x, 1);
	document.form.rmvlan_x.value = "<% tcWebApi_get("IPTV_Entry","ad_rmvlan","s") %>";
	document.form.IPTV_flag.value = 1;
}
function Setting_load_PTM(){
	if(vd_mr_enable_x == "1" && vd_pim_enable_x == "0")
		document.form.mr_enable_x.value = "1";
	else if(vd_mr_enable_x == "0" && vd_pim_enable_x == "1")
		document.form.mr_enable_x.value = "2";
	else
		document.form.mr_enable_x.value = "0";
		
	enable_wan_port(document.form.mr_enable_x.value);
	free_options(document.form.pvc_service_num);
	for(var i=0; i<iptv_list_vd.length; i++){
		var pvc_service_num_temp = eval(iptv_list_vd[i]+1);		
		if(iptv_list_vd[i] == "<% tcWebApi_get("IPTV_Entry","vd_wan_port","s") %>"){
			add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_vd[i], 1);
		}	
		else{
			add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_vd[i], 0);
		}	
	}
	document.form.udpxy_enable_x.value = "<% tcWebApi_get("IPTV_Entry","vd_udpxy_enable_x","s") %>";
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		inputCtrl(document.form.rmvlan_x, 1);
		document.form.rmvlan_x.value = "<% tcWebApi_get("IPTV_Entry","vd_rmvlan","s") %>";
	<%else%>
		inputCtrl(document.form.rmvlan_x, 0);
	<% end if %>
	document.form.IPTV_flag.value = 2;
}
function Setting_load_Ethernet(){
	if(eth_mr_enable_x == "1" && eth_pim_enable_x == "0")
		document.form.mr_enable_x.value = "1";
	else if(eth_mr_enable_x == "0" && eth_pim_enable_x == "1")
		document.form.mr_enable_x.value = "2";
	else
		document.form.mr_enable_x.value = "0";
		
	enable_wan_port(document.form.mr_enable_x.value);
	free_options(document.form.pvc_service_num);
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>		
		for(var i=0; i<iptv_list_eth.length; i++){
			var pvc_service_num_temp = eval(iptv_list_eth[i]+1);		
			if(iptv_list_eth[i] == "<% tcWebApi_get("IPTV_Entry","eth_wan_port","s") %>"){
				add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_eth[i], 1);
			}	
			else{
				add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_eth[i], 0);
			}	
		}
	<%else%>
		document.form.pvc_service_num.parentNode.parentNode.style.display = "none";
		add_option(document.form.pvc_service_num, "Service 1", 0, 1);
	<% end if %>	
	document.form.udpxy_enable_x.value = "<% tcWebApi_get("IPTV_Entry","eth_udpxy_enable_x","s") %>";
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		inputCtrl(document.form.rmvlan_x, 1);
		document.form.rmvlan_x.value = "<% tcWebApi_get("IPTV_Entry","eth_rmvlan","s") %>";
	<%else%>
		inputCtrl(document.form.rmvlan_x, 0);
	<% end if %>	
	document.form.IPTV_flag.value = 3;
}

function enable_wan_port(flag){
//	if(flag==1 || flag==2){	//enable
	if(flag==1){	//enable
		inputCtrl(document.form.pvc_service_num, 1);
		if(document.form.wan_TransMode.value == "Ethernet" && isMultiService_supported != "Yes"){
			document.form.pvc_service_num.parentNode.parentNode.style.display = "none";
			document.form.pvc_service_num.value = 0;
		}
		showhide("igmpproxy_param_text",1);
	}
	else{
		inputCtrl(document.form.pvc_service_num, 0);
		showhide("igmpproxy_param_text",0);
	}	
}

function set_igmpproxy_param() {
	$j("#igmp_panel").fadeIn(300);
	cal_panel_block("igmp_panel", 0.35);
}

function cancel_igmpproxy_panel() {
	$j("#igmp_panel").fadeOut(300);
}

function save_igmpproxy_param() {
	if(!validator.range(document.igmpproxy_form.Robustness, 2, 10)) {
		document.igmpproxy_form.Robustness.focus();
		return false;
	}
	if(!validator.range(document.igmpproxy_form.QueryInterval, 2, 3600)) {
		document.igmpproxy_form.QueryInterval.focus();
		return false;
	}
	if(!validator.range(document.igmpproxy_form.QueryResInterval, 1, 3600)) {
		document.igmpproxy_form.QueryResInterval.focus();
		return false;
	}
	if(parseInt(document.igmpproxy_form.QueryResInterval.value) >= parseInt(document.igmpproxy_form.QueryInterval.value)) {
		alert("Query Response Interval must be less than Query Interval");
		document.igmpproxy_form.QueryResInterval.focus();
		return false;
	}
	document.igmpproxy_form.submit();
	cancel_igmpproxy_panel();
}

</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">

<div id="igmp_panel"  class="contentM_qis" style="box-shadow: 3px 3px 10px #000;display:none;">
	<!--===================================Beginning of IGMPproxy Parameter Content===========================================-->
	<form method="post" name="igmpproxy_form" action="apply.asp" target="hidden_frame">
		<input type="hidden" name="current_page" value="Advanced_IPTV_Content.asp">
		<input type="hidden" name="next_page" value="Advanced_IPTV_Content.asp">
		<input type="hidden" name="modified" value="0">
		<input type="hidden" name="flag" value="background">
		<input type="hidden" name="action_mode" value="apply">
		<input type="hidden" name="action_script" value="saveNvram">
		<input type="hidden" name="action_wait" value="1">
		<input type="hidden" name="formname" value="igmpproxy_form">
		<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable" style="margin-top:8px;">
			<thead>
				<tr>
					<td colspan="2">IGMP Parameter</td>
				</tr>
			</thead>
			<tr>
				<th width="30%">Robustness Variable</th>
				<td>
					<input type="text" maxlength="4" class="input_6_table" name="Robustness" value="<% tcWebApi_Get("IGMPproxy_Entry","Robustness","s") %>" autocorrect="off" autocapitalize="off">
				</td>
			</tr>
			<tr>
				<th width="30%">Query Interval</th>
				<td>
					<input type="text" maxlength="4" class="input_6_table" name="QueryInterval" value="<% tcWebApi_Get("IGMPproxy_Entry","QueryInterval","s") %>" autocorrect="off" autocapitalize="off"> <%tcWebApi_Get("String_Entry","Second","s")%>
				</td>
			</tr>
			<tr>
				<th width="30%">Query Response Interval</th>
				<td>
					<input type="text" maxlength="4" class="input_6_table" name="QueryResInterval" value="<% tcWebApi_Get("IGMPproxy_Entry","QueryResInterval","s") %>" autocorrect="off" autocapitalize="off">
				</td>
			</tr>
		</table>
		<div style="margin-top:5px;width:100%;text-align:center;">
			<input class="button_gen" type="button" onclick="cancel_igmpproxy_panel();" value="<%tcWebApi_Get("String_Entry","CTL_Cancel","s")%>">
			<input class="button_gen" type="button" onclick="save_igmpproxy_param();" value="<%tcWebApi_Get("String_Entry","CTL_onlysave","s")%>">
		</div>
	</form>
	<!--===================================Ending of IGMPproxy Parameter Content===========================================-->
</div>

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
<form method="post" name="form" id="ruleForm" action="Advanced_IPTV_Content.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_IPTV_Content.asp">
<input type="hidden" name="next_page" value="Advanced_IPTV_Content.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="20">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="IPTV_flag" value="0">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="rmvlan" value='<% tcWebApi_get("IPTV_Entry","rmvlan","s"); %>'>
<input type="hidden" name="igmp_enable_x" value="0">
<input type="hidden" name="pim_enable_x" value="0">
<input type="hidden" name="snoop_flag" value="0">
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
						<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_2","s")%> - IPTV</div>
						<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
						<div class="formfontdesc">The WAN port must connect to the internet when watch IPTV.</div>
						<div id="lan_none_desc" class="formfontdesc" style="display:none;color:#FFCC00;"><span><%tcWebApi_get("String_Entry","FW_note","s")%> IPTV is not supported for the current WAN transfer mode.</span></div>
						<div>
							<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
								<thead>
								<tr>
									<td colspan="2"><%tcWebApi_get("String_Entry","wan_type","s")%></td>
								</tr>
								</thead>
								<tr>
									<th><%tcWebApi_get("String_Entry","wan_type","s")%></th>
									<td>
										<select class="input_option" name="wan_TransMode" onchange="doTransChange(this.value);"></td>
								</tr>
							</table>
						</div>

						<div style="margin-top:10px;">
							<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
								<thead>
								<tr>
									<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
								</tr>
								</thead>
								<tr>
									<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,11);"><%tcWebApi_get("String_Entry","RC_GWMulticastEnable_in","s")%></a></th>
									<td>
										<select name="mr_enable_x" class="input_option" onChange="enable_wan_port(this.value);">
											<option value="0"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
											<option value="1">IGMP Proxy</option>
										</select>
										<span id="igmpproxy_param_text" onclick="set_igmpproxy_param();" style="text-decoration:underline;cursor:pointer;">Configuration</span>
									</td>
								</tr>
								<tr>
									<th>WAN Port</th>
									<td>
										<select name="pvc_service_num" class="input_option">
										</select>
									</td>
								</tr>
								<tr>
									<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(6, 6);"><%tcWebApi_get("String_Entry","RC_IPTV_in","s")%></a></th>
									<td>
										<input id="udpxy_enable_x" type="text" maxlength="5" class="input_6_table" name="udpxy_enable_x" value="<%tcWebApi_get("IPTV_Entry", "udpxy_enable_x", "s")%>" onkeypress="return validator.isNumber(this,event);">
									</td>
								</tr>
								<tr>
									<th>Remove VLAN TAG from WAN</th>
									<td>
										<select name="rmvlan_x" class="input_option">
											<option value="0" <%if tcWebApi_get("IPTV_Entry","rmvlan","h") = "0" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
											<option value="1" <%if tcWebApi_get("IPTV_Entry","rmvlan","h") = "1" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
										</select>
									</td>
								</tr>
								<tr>
									<th>IGMP Snooping</th>
									<td>
										<select name="igmpsnoopEnable" class="input_option">
											<option value="No" <%tcWebApi_MatchThenWrite("Lan_IgmpSnoop","igmpsnoopEnable","No","selected")%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
											<option value="Yes" <%tcWebApi_MatchThenWrite("Lan_IgmpSnoop","igmpsnoopEnable","Yes","selected")%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
										</select>
									</td>
								</tr>
							</table>
						</div>

						<div id="button_gen_div" class="apply_gen">
							<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
						</div>
					</td>
					</tr>
					</tbody>
				</table>
				</td>
			</tr>
			</table>
		</td>
		<td width="10" align="center" valign="top">&nbsp;</td>
	</tr>
</table>
</form>

<div id="footer"></div>
</body>

<!--Advanced_IPTV_Content.asp-->
</html>

