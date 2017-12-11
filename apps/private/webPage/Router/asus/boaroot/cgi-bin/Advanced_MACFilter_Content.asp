<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_MACFilter_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_5_3","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
var client_mac = login_mac_str();
var macfilter_num_x = '0';
var smac = client_mac.split(":");
var simply_client_mac = smac[0] + smac[1] + smac[2] + smac[3] + smac[4] + smac[5];
var macfilter_rulelist_array = '';
function initial(){
show_menu();
load_body();
showmacfilter_rulelist();
}
function showmacfilter_rulelist(){
var macfilter_rulelist_row = macfilter_rulelist_array.split('&#60');
var code = "";
code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="list_table" id="macfilter_rulelist_table">';
if(macfilter_rulelist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td>';
else{
for(var i = 1; i < macfilter_rulelist_row.length; i++){
code +='<tr id="row'+i+'">';
code +='<td width="40%">'+ macfilter_rulelist_row[i] +'</td>'; //MAC
code +='<td width="20%">';
code +="<input class=\"remove_btn\" type=\"button\" onclick=\"deleteRow(this);\" value=\"\"/></td>";
}
}
code +='</tr></table>';
$("macfilter_rulelist_Block").innerHTML = code;
}
function deleteRow(r){
var i=r.parentNode.parentNode.rowIndex;
$('macfilter_rulelist_table').deleteRow(i);
var macfilter_rulelist_value = "";
for(i=0; i<$('macfilter_rulelist_table').rows.length; i++){
macfilter_rulelist_value += "&#60";
macfilter_rulelist_value += $('macfilter_rulelist_table').rows[i].cells[0].innerHTML;
}
macfilter_rulelist_array = macfilter_rulelist_value;
if(macfilter_rulelist_array == "")
showmacfilter_rulelist();
}
function addRow(obj, upper){
var rule_num = $('macfilter_rulelist_table').rows.length;//rule list
var item_num = $('macfilter_rulelist_table').rows[0].cells.length; //
if(rule_num >= upper){
		alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
return false;
}
if(!check_hwaddr_temp(obj))
return false;
if(obj.value==""){
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
obj.focus();
obj.select();
}else if(check_hwaddr_temp(obj)){
for(i=0; i<rule_num; i++){
for(j=0; j<item_num-1; j++){ //only 1 value column
if(obj.value.toLowerCase() == $('macfilter_rulelist_table').rows[i].cells[j].innerHTML.toLowerCase()){
					alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");
return false;
}
}
}
macfilter_rulelist_array += "&#60";
macfilter_rulelist_array += obj.value;
obj.value = "";
showmacfilter_rulelist();
}else
return false;
}
function applyRule(){
if(prevent_lock()){
var rule_num = $('macfilter_rulelist_table').rows.length;//rule list
var item_num = $('macfilter_rulelist_table').rows[0].cells.length; //
var tmp_value = "";
for(i=0; i<rule_num; i++){
tmp_value += "<"
for(j=0; j<item_num-1; j++){
tmp_value += $('macfilter_rulelist_table').rows[i].cells[j].innerHTML;
if(j != item_num-2)
tmp_value += ">";
}
}
		if(tmp_value == "<"+"<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>" || tmp_value == "<")
tmp_value = "";
document.form.macfilter_rulelist.value = tmp_value;
showLoading();
document.form.submit();
}
else
return false;
}
function prevent_lock(){
if(document.form.macfilter_enable_x.value == "1"){
if(macfilter_num_x == 0){
			if(confirm("<%tcWebApi_get("String_Entry","FC_MFList_accept_hint1","s")%>")){
return true;
}
else
return false;
}
else
return true;
}
else
return true;
}
function done_validating(action){
refreshpage();
}
function check_macaddr(obj,flag){ //control hint of input mac address
if (flag){
$("check_mac") ? $("check_mac").style.display="none" : true;
}
else{
var childsel=document.createElement("div");
childsel.setAttribute("id","check_mac");
childsel.style.color="#FFCC00";
obj.parentNode.appendChild(childsel);
$("check_mac").innerHTML="The format for the MAC address is six groups of two hexadecimal digits, separated by colons (:), in transmission order (e.g. 12:34:56:aa:bc:ef)";
}
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<input type="hidden" name="current_page" value="Advanced_MACFilter_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_script" value="restart_firewall">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="macfilter_rulelist" value="">
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
<td valign="top" >
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_5","s")%> - <%tcWebApi_get("String_Entry","menu5_5_3","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc"><%tcWebApi_get("String_Entry","FC_display5_sd","s")%></div>
	  
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
						<td colspan="4"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
        		<th width="30%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(18,1);"><%tcWebApi_get("String_Entry","FC_MFMethod_in","s")%></a></th>
<td>
<select name="macfilter_enable_x" class="input_option" onchange="return change_common(this, 'FirewallConfig', 'macfilter_enable_x')">
<option value="0" selected><%tcWebApi_get("String_Entry","CTL_Disabled","s")%></option>
<option value="1" ><%tcWebApi_get("String_Entry","FC_MFMethod_item1","s")%></option>
<option value="2" ><%tcWebApi_get("String_Entry","FC_MFMethod_item2","s")%></option>
</select>
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
<thead>
<tr>
						<td colspan="4"><%tcWebApi_get("String_Entry","FC_MFList_groupitemname","s")%></td>
</tr>
</thead>
<tr>
<th width="40%"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,10);">
<%tcWebApi_get("String_Entry","FC_MFhwaddr_in","s")%></a>
<input type="hidden" name="macfilter_num_x_0" value="0" readonly="1"/>
</th>
<th width="20%"><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
</tr>
<tr>
<td width="40%">
<input type="text" maxlength="17" class="input_macaddr_table" name="macfilter_list_x_0" onKeyPress="return validator.isHWAddr(this,event)" onblur="check_macaddr(this,check_hwaddr_temp(this))">
</td>
<td width="20%">
<input class="add_btn" type="button" onclick="addRow(document.form.macfilter_list_x_0, 32);" value="">
</td>
</tr>
</table>
<div id="macfilter_rulelist_Block"></div>
<div class="apply_gen">
<input name="button" type="button" class="button_gen" onclick="applyRule()" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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

<!--Advanced_MACFilter_Content.asp-->
</html>

