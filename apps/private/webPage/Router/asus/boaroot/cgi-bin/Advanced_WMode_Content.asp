<%
tcWebApi_set("WLan_Common","editFlag","editFlag")
tcWebApi_set("WLan_Common","MBSSID_changeFlag","MBSSID_changeFlag")
tcWebApi_set("WLan_Common","MBSSID_able_Flag","MBSSID_able_Flag")

If Request_Form("editFlag") = "1" then
	tcWebApi_Set("SysInfo_Entry","w_Setting","w_Setting")
	tcWebApi_commit("SysInfo_Entry")

	tcWebApi_Set("WLan_Common","wl_unit","wl_unit")
	tcWebApi_Set("WLan_Common","Channel","wl_channel")
	tcWebApi_Set("WLan_WDS","wl_ap_mode","wl_mode_x")
	tcWebApi_Set("WLan_WDS","wl_wdsapply","wl_wdsapply_x")
	tcWebApi_Set("WLan_WDS","wl_wdslist","wl_wdslist")
	tcWebApi_commit("WLan_Entry")
end if

If Request_Form("editFlag") = "1" then
load_parameters_from_generic()
end if
load_parameters_to_generic()

If Request_Form("rebootFlag") = "1" then
	tcWebApi_Set("System_Entry","reboot_type","restoreFlag")
	tcWebApi_delay_CommitWithoutSave("System_Entry", "30")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_WMode_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - WDS</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<style>
#pull_arrow{
float:center;
cursor:pointer;
border:2px outset #EFEFEF;
background-color:#CCC;
padding:3px 2px 4px 0px;
}
#WDSAPList{
border:1px outset #999;
background-color:#576D73;
position:absolute;
margin-top:25px;
margin-left:180px;
*margin-left:-150px;
width:255px;
*width:259px;
text-align:left;
height:auto;
overflow-y:auto;
z-index:200;
padding: 1px;
display:none;
}
#WDSAPList div{
background-color:#576D73;
height:20px;
line-height:20px;
text-decoration:none;
padding-left:2px;
}
#WDSAPList a{
background-color:#EFEFEF;
color:#FFF;
font-size:12px;
font-family:Arial, Helvetica, sans-serif;
text-decoration:none;
}
#WDSAPList div:hover, #ClientList_Block a:hover{
background-color:#3366FF;
color:#FFFFFF;
cursor:default;
}
</style>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/JavaScript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }


var wireless = []; // [[MAC, associated, authorized], ...]
var wl_wdslist_array = '<% tcWebApi_get("WLan_WDS","wl_wdslist","s"); %>';
var wds_aplist = "";
var $j = jQuery.noConflict();
function initial(){
show_menu();
load_body();
//insertExtChannelOption();
//document.form.wl_channel.value = document.form.wl_channel_orig.value;
if(sw_mode == 2 && '0' == ''){
for(var i=5; i>=3; i--)
document.getElementById("MainTable1").deleteRow(i);
for(var i=2; i>=0; i--)
document.getElementById("MainTable2").deleteRow(i);
document.getElementById("repeaterModeHint").style.display = "";
document.getElementById("wl_wdslist_Block").style.display = "none";
document.getElementById("submitBtn").style.display = "none";
}
else
show_wl_wdslist();

document.getElementById("wl0_hwaddr").value =	document.getElementById("wl0_hwaddr").value.toString().toUpperCase();
document.getElementById("wl1_hwaddr").value =	document.getElementById("wl1_hwaddr").value.toString().toUpperCase();

if(!wl_info.band5g_support){
	document.getElementById("wl_5g_mac").style.display = "none";
	document.getElementById("wl_unit_field").style.display = "none";
}

wl_bwch_hint();
}
function show_wl_wdslist(){
var wl_wdslist_row = wl_wdslist_array.split('<');
var code = "";
code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="list_table" id="wl_wdslist_table">';
if(wl_wdslist_row.length == 1){
		code +='<tr><td style="color:#FFCC00;"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td>';
}
else{
for(var i = 1; i < wl_wdslist_row.length; i++){
code +='<tr id="row'+i+'">';
code +='<td width="70%">'+ wl_wdslist_row[i] +'</td>';
code +='<td width="30%"><input type="button" class=\"remove_btn\" onclick=\"deleteRow(this);\" value=\"\"/></td></tr>';
}
}
code +='</tr></table>';
document.getElementById("wl_wdslist_Block").innerHTML = code;
}
function deleteRow(r){
var i=r.parentNode.parentNode.rowIndex;
document.getElementById('wl_wdslist_table').deleteRow(i);
var wl_wdslist_value = "";
for(i=0; i<document.getElementById('wl_wdslist_table').rows.length; i++){
wl_wdslist_value += "<";
wl_wdslist_value += document.getElementById('wl_wdslist_table').rows[i].cells[0].innerHTML;
}
wl_wdslist_array = wl_wdslist_value;
if(wl_wdslist_array == "")
show_wl_wdslist();
}
function addRow(obj, upper) {
	var rule_num = document.getElementById('wl_wdslist_table').rows.length;
	var item_num = document.getElementById('wl_wdslist_table').rows[0].cells.length;

	if(rule_num >= upper) {
		alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
		return false;
	}

	if(obj.value == "") {
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
		obj.focus();
		obj.select();
		return false;
	}
	else if (!check_macaddr(obj, check_hwaddr_flag(obj))) {
		obj.focus();
		obj.select();		
		return false;
	}
	
	for(i = 0; i < rule_num; i++) {
		for(j = 0; j < item_num - 1; j++) {
			if(obj.value.toLowerCase() == document.getElementById('wl_wdslist_table').rows[i].cells[j].innerHTML.toLowerCase()) {
				alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");
				return false;
			}
		}
	}
	wl_wdslist_array += "<";
	wl_wdslist_array += obj.value;
	obj.value = "";
	show_wl_wdslist();
}
function redirect(){
	document.location.href = "/cgi-bin/Advanced_WMode_Content.asp";
}
function applyRule(){
	var rule_num = document.getElementById('wl_wdslist_table').rows.length;
	var item_num = document.getElementById('wl_wdslist_table').rows[0].cells.length;
	var tmp_value = "";
	for(i=0; i<rule_num; i++){
		tmp_value += "<"
		for(j=0; j<item_num-1; j++){
			tmp_value += document.getElementById('wl_wdslist_table').rows[i].cells[j].innerHTML;
			if(j != item_num-2)
				tmp_value += ">";
		}
	}
	if(tmp_value == "<"+"<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>" || tmp_value == "<")
		tmp_value = "";
	document.form.wl_wdslist.value = tmp_value;

	if(document.form.wl_mode_x.value == "0"){
		inputRCtrl1(document.form.wl_wdsapply_x, 1);
		inputRCtrl2(document.form.wl_wdsapply_x, 1);
	}
	else if(document.form.wl_mode_x.value == "1"){
		document.form.wl_wdsapply_x.value = "1";
	}	
	
	if(document.form.wl_mode_x.value != document.form.wl_mode_x_orig.value){		
		document.form.rebootFlag.disabled = false;
		document.form.restoreFlag.disabled = false;
		showLoading(150);
		setTimeout("redirect();", 150000);

	}
	else{
		if(model_name == "DSL-N66U"){
			showLoading(28);
			setTimeout("redirect();", 28000);
		}
		else{
			showLoading(23);
			setTimeout("redirect();", 23000);
		}				
	}

	if(navigator.appName.indexOf("Microsoft") >= 0){ 		// Jieming added at 2013/05/21, to avoid browser freeze when submitting form on IE
			stopFlag = 1;
	}
	document.form.editFlag.value = "1";	
	document.form.submit();	
}
function done_validating(action){
refreshpage();
}
/*------------ Site Survey Start -----------------*/
function wds_scan(){
	var ajaxURL = 'wds_aplist_2g.asp';
	if('<% tcWebApi_get("WLan_Common","wl_unit","s"); %>' == '1')
		var ajaxURL = 'wds_aplist_5g.asp';
	$j.ajax({
		url: ajaxURL,
		dataType: 'script',

		error: function(xhr){
			setTimeout("wds_scan();", 1000);
		},
		success: function(response){
			showLANIPList();
		}
	});
}
function setClientIP(num){
document.form.wl_wdslist_0.value = wds_aplist[num][1];
hideClients_Block();
over_var = 0;
}
function rescan(){
	wds_aplist = "";
	showLANIPList()
	wds_scan();
}
function showLANIPList(){
	var code = "";
	var show_name = "";
	var show_title = "";
	if(wds_aplist != ""){
		for(var i = 0; i < wds_aplist.length ; i++){
			wds_aplist[i][0] = htmlEnDeCode.htmlEncode(decodeURIComponent(wds_aplist[i][0]));
			if(wds_aplist[i][0] && wds_aplist[i][0].length > 12)
				show_name = wds_aplist[i][0].substring(0, 10) + "...";
			else
				show_name = wds_aplist[i][0];
			show_title = wds_aplist[i][0];
			if(wds_aplist[i][1]){
				code += '<a href="#"><div onmouseover="over_var=1;" onmouseout="over_var=0;" onclick="setClientIP('+i+');" title="'+show_title+'"><strong>'+show_name+'</strong>';
			if(show_name && show_name.length > 0)
				code += '( '+wds_aplist[i][1]+')';
			else
				code += wds_aplist[i][1];
			code += ' </div></a>';
			}
		}
		code += '<div style="text-decoration:underline;font-weight:bolder;" onclick="rescan();"><%tcWebApi_get("String_Entry","AP_survey","s")%></div>';
	}
	else{
		code += '<div style="width:98px"><img height="15px" style="margin-left:5px;margin-top:2px;" src="/images/InternetScan.gif"></div>';
	}
	code +='<!--[if lte IE 6.5]><iframe class="hackiframe2"></iframe><![endif]-->';
	document.getElementById("WDSAPList").innerHTML = code;
}
function pullLANIPList(obj){
	if(isMenuopen == 0){
		obj.src = "/images/arrow-top.gif"
		setTimeout("wds_scan();", 500);
		document.getElementById("WDSAPList").style.display = 'block';
		document.form.wl_wdslist_0.focus();
		isMenuopen = 1;
	}
	else{
		hideClients_Block();
	}
}
var over_var = 0;
var isMenuopen = 0;
function hideClients_Block(){
document.getElementById("pull_arrow").src = "/images/arrow-down.gif";
document.getElementById('WDSAPList').style.display='none';
isMenuopen = 0;
}

function check_macaddr(obj,flag){ //control hint of input mac address
	if(flag == 1) {
		var childsel = document.createElement("div");
		childsel.setAttribute("id","check_mac");
		childsel.style.color = "#FFCC00";
		obj.parentNode.appendChild(childsel);
		document.getElementById("check_mac").innerHTML = '<br><br><%tcWebApi_get("String_Entry","LHC_MnlDHCPMacaddr_id","s")%>';
		document.getElementById("check_mac").style.display = "";

		return false;	
	}
	else if(flag == 2) {
		var childsel = document.createElement("div");
		childsel.setAttribute("id","check_mac");
		childsel.style.color = "#FFCC00";
		obj.parentNode.appendChild(childsel);
		document.getElementById("check_mac").innerHTML = "<br><br><%tcWebApi_get("String_Entry","IPC_x_illegal_mac","s")%>";
		document.getElementById("check_mac").style.display = "";
		return false;			
	}
	else {
		document.getElementById("check_mac") ? document.getElementById("check_mac").style.display="none" : true;
		return true;
	} 	
}

function _change_wl_unit(wl_unit){
	document.form_band.wl_unit.value = wl_unit;	
	document.form_band.action = "/cgi-bin/Advanced_WMode_Content.asp";
	showLoading(2);
	setTimeout("redirect(1);", 2000);
	document.form_band.submit();
}
/*---------- Site Survey End -----------------*/

//Control display chanspec hint when wl_bw=1(20/40) or wl_channel=0(Auto)
function wl_bwch_hint(){  
		document.getElementById("wl_bw_hint").style.display=(document.form.wl_bw.value == "1") ? "" : "none";
		document.getElementById("wl_ch_hint").style.display=(document.form.wl_channel_orig.value == "0") ? "" : "none";
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form_band" action="Advanced_WMode_Content.asp" target="hidden_frame">
<input type="hidden" name="wl_unit">
<input type="hidden" name="editFlag" value="0">
</form>
<form method="post" name="form" id="ruleForm" action="Advanced_WMode_Content.asp" target="hidden_frame">
<input type="hidden" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>" name="productid" >
<input type="hidden" value="" name="wan_route_x" >
<input type="hidden" value="1" name="wan_nat_x" >
<input type="hidden" name="current_page" value="Advanced_WMode_Content.asp">
<input type="hidden" name="next_page" value="Advanced_WMode_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="wl_wdslist">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="3">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="restart_wireless">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" maxlength="15" size="15" name="x_RegulatoryDomain" value="" readonly="1">
<input type="hidden" name="wl_wdsnum_x_0" value="" readonly="1">
<input type="hidden" name="wl_nmode_x" value='0'>
<input type="hidden" name="wl_wdslist" value=''>
<input type="hidden" name="wl_subunit" value="-1">
<input type="hidden" name="wl_bw" value="<% tcWebApi_staticGet("WLan_Common","HT_BW","s") %>">
<input type="hidden" name="wl_channel_orig" value="<% tcWebApi_staticGet("WLan_Common","Channel","s") %>">
<input type="hidden" name="MBSSID_changeFlag" value="0">
<input type="hidden" name="MBSSID_able_Flag" value="0">
<input type="hidden" name="editFlag" value="0">
<input type="hidden" name="rebootFlag" value="1" disabled>
<input type="hidden" name="restoreFlag" value="1" disabled>
<input type="hidden" name="wl_mode_x_orig" value="<% tcWebApi_get("WLan_WDS","wl_ap_mode","s") %>">
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
<td align="left" valign="top" >
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
		  	<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_1","s")%> - <%tcWebApi_get("String_Entry","menu5_1_3","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>

<div class="formfontdesc"><%tcWebApi_get("String_Entry","WC11b_display1_sd","s")%></div>
<div class="formfontdesc" style="color:#FFCC00;"><%tcWebApi_get("String_Entry","ADSL_FW_note","s")%><%tcWebApi_get("String_Entry","WC11b_display2_sd","s")%></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","WC11b_display3_sd","s")%>
	<ol>
		<li><%tcWebApi_get("String_Entry","WC11b_display31_sd","s")%></li>
		<li><%tcWebApi_get("String_Entry","WC11b_display32_sd","s")%></li>
		<li><%tcWebApi_get("String_Entry","WC11b_display33_sd","s")%></li>
		<li><%tcWebApi_get("String_Entry","WC11b_display34_sd","s")%></li>					
	</ol>					
</div>
<div id="wl_bw_hint" style="font-size:13px;font-family: Arial, Helvetica, sans-serif;color:#FC0;margin-left:28px;"><%tcWebApi_get("String_Entry","WC11b_display41_sd","s")%></div>
<div id="wl_ch_hint" style="font-size:13px;font-family: Arial, Helvetica, sans-serif;color:#FC0;margin-left:28px;"><%tcWebApi_get("String_Entry","WC11b_display42_sd","s")%></div>

<table id="MainTable1" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
				<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>

				<tr>
					<th>2.4GHz MAC</th>
					<td>
							<input type="text" maxlength="17" class="input_20_table" id="wl0_hwaddr" name="wl0_hwaddr" value="<% tcWebApi_staticGet("WLan_Common","wl0_MacAddress","s") %>" readonly>
					</td>		
			  </tr>

				
				<tr id="wl_5g_mac">
					<th>5GHz MAC</th>
					<td>
							<input type="text" maxlength="17" class="input_20_table" id="wl1_hwaddr" name="wl1_hwaddr" value="<% tcWebApi_staticGet("WLan_Common","wl1_MacAddress","s") %>" readonly>
					</td>		
			  </tr>	

	<tr id="wl_unit_field">
		<th><% tcWebApi_Get("String_Entry", "Interface", "s") %></th>
		<td>
			<select name="wl_unit" class="input_option" onChange="_change_wl_unit(this.value);">
				<option class="content_input_fd" value="0" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "0" then asp_Write("selected") end if %>>2.4GHz</option>
				<option class="content_input_fd" value="1" <% if tcWebApi_get("WLan_Common","wl_unit","h") = "1" then asp_Write("selected") end if %>>5GHz</option>
			</select>
		</td>
	</tr>
<tr id="repeaterModeHint" style="display:none;">
					<td colspan="2" style="color:#FFCC00;height:30px;" align="center"><%tcWebApi_get("String_Entry","page_not_support_mode_hint","s")%></td>
</tr>
	<tr>
		<th align="right" >
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(1,1);">
			<%tcWebApi_get("String_Entry","WC11b_x_APMode_in","s")%></a>
		</th>
		<td>
			<select name="wl_mode_x" class="input_option" onChange="change_wireless_bridge(this.value);">
				<option value="0" <% if tcWebApi_get("WLan_WDS","wl_ap_mode","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_x_APMode_option0","s")%></option>
				<option value="1" <% if tcWebApi_get("WLan_WDS","wl_ap_mode","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_x_APMode_option1","s")%></option>
				<option value="2" <% if tcWebApi_get("WLan_WDS","wl_ap_mode","h") = "2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WC11b_x_APMode_option2","s")%></option>
			</select>
		</td>
	</tr>
	<tr>
		<th align="right">
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(1,3);">
			<%tcWebApi_get("String_Entry","WC11b_x_BRApply_in","s")%>
			</a>
		</th>
		<td>
			<input type="radio" value="1" name="wl_wdsapply_x" class="input" <% if tcWebApi_get("WLan_WDS","wl_wdsapply","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
			<input type="radio" value="0" name="wl_wdsapply_x" class="input" <% if tcWebApi_get("WLan_WDS","wl_wdsapply","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
		</td>
	</tr>

	<!-- tr id="wl_channel_field">
		<th><a id="wl_channel_select" class="hintstyle" href="javascript:void(0);" onClick="openHint(0, 3);"><% tcWebApi_Get("String_Entry", "WC11b_Channel_in", "s") %></a></th>
		<td>
			<select name="wl_channel" class="input_option" onChange="return change_common(this, 'WLANConfig11b', 'wl_channel')">
				<option value="0" <% if tcWebApi_get("WLan_Common","Channel","h") = "0" then asp_Write("selected") end if %>>Auto</option>
				<option value="1" <% if tcWebApi_get("WLan_Common","Channel","h") = "1" then asp_Write("selected") end if %>>1</option>
				<option value="2" <% if tcWebApi_get("WLan_Common","Channel","h") = "2" then asp_Write("selected") end if %>>2</option>
				<option value="3" <% if tcWebApi_get("WLan_Common","Channel","h") = "3" then asp_Write("selected") end if %>>3</option>
				<option value="4" <% if tcWebApi_get("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>4</option>
				<option value="5" <% if tcWebApi_get("WLan_Common","Channel","h") = "5" then asp_Write("selected") end if %>>5</option>
				<option value="6" <% if tcWebApi_get("WLan_Common","Channel","h") = "6" then asp_Write("selected") end if %>>6</option>
				<option value="7" <% if tcWebApi_get("WLan_Common","Channel","h") = "7" then asp_Write("selected") end if %>>7</option>
				<option value="8" <% if tcWebApi_get("WLan_Common","Channel","h") = "8" then asp_Write("selected") end if %>>8</option>
				<option value="9" <% if tcWebApi_get("WLan_Common","Channel","h") = "9" then asp_Write("selected") end if %>>9</option>
				<option value="10" <% if tcWebApi_get("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10</option>
				<option value="11" <% if tcWebApi_get("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11</option>
			</select>
		</td>
	</tr>

<tr style="display:none;">
				 	<th><%tcWebApi_get("String_Entry","WC11b_EChannel_in","s")%></th>
<td>
<select name="wl_nctrlsb">
<option value="lower" selected>lower</option>
<option value="upper">upper</option>
</select>
</td>
</tr -->
</table>
<table id="MainTable2" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable_table">
<thead>
<tr>
				<td colspan="4"><%tcWebApi_get("String_Entry","WC11b_RBRList_groupitemdesc","s")%> (<%tcWebApi_get("String_Entry","List_limit","s")%> 4)</td>
</tr>
</thead>
<tr>
<th width="70%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,10);">
								 <%tcWebApi_get("String_Entry","WC11b_RBRList_groupitemdesc","s")%>
</th>
<th class="edit_table" width="30%"><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
</tr>
<tr>
<td width="70%">
	<input type="text" style="margin-left:180px;float:left;" maxlength="17" class="input_macaddr_table" name="wl_wdslist_0" onKeyPress="return validator.isHWAddr(this,event)">
	<img style="float:left;" id="pull_arrow" height="14px;" src="/images/arrow-down.gif" onclick="pullLANIPList(this);" title="<% tcWebApi_Get("String_Entry", "select_AP", "s") %>" onmouseover="over_var=1;" onmouseout="over_var=0;">
	<div id="WDSAPList" class="WDSAPList">
		<div style="width:98px">
			<img height="15px" style="margin-left:5px;margin-top:2px;" src="/images/InternetScan.gif">
		</div>
	</div>
</td>
<td width="30%">
<input type="button" class="add_btn" onClick="addRow(document.form.wl_wdslist_0, 4);" value="">
</td>
</tr>
</table>
<div id="wl_wdslist_Block"></div>
<div class="apply_gen">
<input class="button_gen" id="submitBtn" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>" />
</div>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_WMode_Content.asp-->
</html>

