<%
If Request_Form("editFlag") = "1" then
	tcWebApi_set("portTriggering_Entry0","AutoFW_Enable","autofw_enable_x")
	tcWebApi_set("portTriggering_Entry0","AutoFW_RuleList","autofw_rulelist")
	tcWebApi_commit("portTriggering_Entry")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_PortTrigger_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_3_3","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" language="JavaScript" src="/help.js"></script>
<script type="text/javascript" language="JavaScript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
var autofw_rulelist_array = "<%TCWebApi_get("portTriggering_Entry0","AutoFW_RuleList","s")%>";	//javi
function initial(){
show_menu();
setTimeout("update_FAQ();", 300);
load_body();
showautofw_rulelist();
}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq", "https://www.asus.com", "/support/FAQ/", "114110");
	}
}
function applyRule(){
	var rule_num = $('autofw_rulelist_table').rows.length;
	var item_num = $('autofw_rulelist_table').rows[0].cells.length;
	var tmp_value = "";
	for(i=0; i<rule_num; i++){
	tmp_value += "<"
	for(j=0; j<item_num-1; j++){
	tmp_value += $('autofw_rulelist_table').rows[i].cells[j].innerHTML;
	if(j != item_num-2)
	tmp_value += ">";
	}
	}
	if(tmp_value == "<"+"<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>" || tmp_value == "<")
	tmp_value = "";
	document.form.autofw_rulelist.value = tmp_value;
	document.form.action = "/cgi-bin/Advanced_PortTrigger_Content.asp";	//javi
	document.form.editFlag.value = "1" ; 
	showLoading(5);
	setTimeout('location = "'+ location.pathname +'";', 5000);
	document.form.submit();
}
function done_validating(action){
refreshpage();
}
function change_wizard(o, id){
for(var i = 0; i < wItem.length; i++){
if(wItem[i][0] != null){
if(o.value == wItem[i][0]){
document.form.autofw_outport_x_0.value = wItem[i][1];
document.form.autofw_outproto_x_0.value = wItem[i][2];
document.form.autofw_inport_x_0.value = wItem[i][3];
document.form.autofw_inproto_x_0.value = wItem[i][4];
document.form.autofw_desc_x_0.value = wItem[i][0];
break;
}
}
}
}
function addRow(obj, head){
if(head == 1)
autofw_rulelist_array += "<"
else
autofw_rulelist_array += ">"
autofw_rulelist_array += obj.value;
obj.value = "";
}
function addRow_Group(upper){
var rule_num = $('autofw_rulelist_table').rows.length;
var item_num = $('autofw_rulelist_table').rows[0].cells.length;
if(rule_num >= upper){
		alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
return false;
}
if(document.form.autofw_outport_x_0.value==""){
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
document.form.autofw_outport_x_0.focus();
document.form.autofw_outport_x_0.select();
}else if(document.form.autofw_inport_x_0.value==""){
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
document.form.autofw_inport_x_0.focus();
document.form.autofw_inport_x_0.select();
}else if(validate_number_range(document.form.autofw_outport_x_0, 1, 65535)==true && validate_number_range(document.form.autofw_inport_x_0, 1, 65535)==true){
if(item_num >=2){
for(i=0; i<rule_num; i++){
if(document.form.autofw_outport_x_0.value == $('autofw_rulelist_table').rows[i].cells[1].innerHTML
&& document.form.autofw_outproto_x_o.value == $('autofw_rulelist_table').rows[i].cells[2].innerHTML
&& document.form.autofw_inport_x_0.value == $('autofw_rulelist_table').rows[i].cells[3].innerHTML
&& document.form.autofw_inproto_x_o.value == $('autofw_rulelist_table').rows[i].cells[4].innerHTML){
						alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");						
document.form.autofw_outport_x_0.focus();
document.form.autofw_outport_x_0.select();
return false;
}
}
}
addRow(document.form.autofw_desc_x_0 ,1);
addRow(document.form.autofw_outport_x_0, 0);
addRow(document.form.autofw_outproto_x_o, 0);
document.form.autofw_outproto_x_o.value="TCP";
addRow(document.form.autofw_inport_x_0, 0);
addRow(document.form.autofw_inproto_x_o, 0);
document.form.autofw_inproto_x_o.value="TCP";
showautofw_rulelist();
}else{
return false;
}
}
function edit_Row(r){
var i=r.parentNode.parentNode.rowIndex;
document.form.autofw_desc_x_0.value = $('autofw_rulelist_table').rows[i].cells[0].innerHTML;
document.form.autofw_outport_x_0.value = $('autofw_rulelist_table').rows[i].cells[1].innerHTML;
document.form.autofw_outproto_x_o.value = $('autofw_rulelist_table').rows[i].cells[2].innerHTML;
document.form.autofw_inport_x_0.value = $('autofw_rulelist_table').rows[i].cells[3].innerHTML;
document.form.autofw_inproto_x_o.value = $('autofw_rulelist_table').rows[i].cells[4].innerHTML;
del_Row(r);
}
function del_Row(r){
var i=r.parentNode.parentNode.rowIndex;
$('autofw_rulelist_table').deleteRow(i);
var autofw_rulelist_value = "";
for(k=0; k<$('autofw_rulelist_table').rows.length; k++){
for(j=0; j<$('autofw_rulelist_table').rows[k].cells.length-1; j++){
if(j == 0)
autofw_rulelist_value += "<";
else
autofw_rulelist_value += ">";
autofw_rulelist_value += $('autofw_rulelist_table').rows[k].cells[j].innerHTML;
}
}
autofw_rulelist_array = autofw_rulelist_value;
if(autofw_rulelist_array == "")
showautofw_rulelist();
}
function showautofw_rulelist(){
var autofw_rulelist_row = autofw_rulelist_array.split('<');
var code = "";
code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="autofw_rulelist_table">';
if(autofw_rulelist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;" colspan="6"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td></tr>';
else{
for(var i = 1; i < autofw_rulelist_row.length; i++){
code +='<tr id="row'+i+'">';
var autofw_rulelist_col = autofw_rulelist_row[i].split('>');
var wid=[22, 21, 10, 21, 10];
for(var j = 0; j < autofw_rulelist_col.length; j++){
code +='<td width="'+wid[j]+'%">'+ autofw_rulelist_col[j] +'</td>'; //IP width="98"
}
code +='<td width="16%"><!--input class="edit_btn" onclick="edit_Row(this);" value=""/-->';
code +='<input class="remove_btn" onclick="del_Row(this);" value=""/></td></tr>';
}
}
code +='</table>';
$("autofw_rulelist_Block").innerHTML = code;
}
function changeBgColor(obj, num){
if(obj.checked)
$("row" + num).style.background='#FF9';
else
$("row" + num).style.background='#FFF';
}
function trigger_validate_duplicate_noalert(o, v, l, off){
for (var i=0; i<o.length; i++)
{
if (entry_cmp(o[i][0].toLowerCase(), v.toLowerCase(), l)==0){
return false;
}
}
return true;
}
function trigger_validate_duplicate(o, v, l, off){
for(var i = 0; i < o.length; i++)
{
if(entry_cmp(o[i][1].toLowerCase(), v.toLowerCase(), l) == 0){
			alert('<%tcWebApi_get("String_Entry","JS_duplicate","s")%>');	
return false;
}
}
return true;
}
</script>
</head>
<body onload=" initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_PortTrigger_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_script" value="restart_firewall">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="autofw_rulelist" value=''>
<input type="hidden" name="autofw_num_x_0" value="" readonly="1" />
<input type="hidden" name="editFlag" value="0">
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
<td valign="top" >
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
		<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_3","s")%> - <%tcWebApi_get("String_Entry","menu5_3_3","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		<div class="formfontdesc"><%tcWebApi_get("String_Entry","IPC_porttrigger_sd","s")%></div>
		<div class="formfontdesc" style="margin-top:-10px;">
			<a id="faq" href="https://www.asus.com/support/FAQ/114110" target="_blank" style="font-family:Lucida Console;text-decoration:underline;"><%tcWebApi_get("String_Entry","menu5_3_3","s")%>&nbsp FAQ</a>
		</div>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
<thead>
<tr>
						<td colspan="6"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
            	<th colspan="2"><%tcWebApi_get("String_Entry","IPC_autofwEnable_in","s")%></th>
<td colspan="4">
<input type="radio" value="1" name="autofw_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'IPConnection', 'autofw_enable_x', '1')" <% if tcWebApi_get("portTriggering_Entry0","AutoFW_Enable","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" value="0" name="autofw_enable_x" class="content_input_fd" onClick="return change_common_radio(this, 'IPConnection', 'autofw_enable_x', '0')" <% if tcWebApi_get("portTriggering_Entry0","AutoFW_Enable","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
            	<th colspan="2"align="right" id="autofw_rulelist"><%tcWebApi_get("String_Entry","IPC_TriggerList_widzarddesc","s")%></th>
<td colspan="4">
<select name="TriggerKnownApps" class="input_option" onChange="change_wizard(this, 'TriggerKnownApps');">
            			<option value="User Defined"><%tcWebApi_get("String_Entry","Select_menu_default","s")%></option>
</select>
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
<thead>
<tr>
            		<td colspan="6" id="autofw_rulelist"><%tcWebApi_get("String_Entry","IPC_TriggerList_groupitemdesc","s")%> (<%tcWebApi_get("String_Entry","List_limit","s")%> 14)</td>
</tr>
</thead>
<tr>
		<th><%tcWebApi_get("String_Entry","IPC_autofwDesc_in","s")%></th>
            	<th><%tcWebApi_get("String_Entry","IPC_autofwOutPort_in","s")%></th>
            	<th><%tcWebApi_get("String_Entry","IPC_VServerProto_in","s")%></th>
            	<th><%tcWebApi_get("String_Entry","IPC_autofwInPort_in","s")%></th>
            	<th><%tcWebApi_get("String_Entry","IPC_VServerProto_in","s")%></th>            
		<th><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
</tr>
<tr>
<td width="22%">
<input type="text" maxlength="18" class="input_15_table" name="autofw_desc_x_0" onKeyPress="return validator.isString(this, event)">
</td>
<td width="21%">
<input type="text" maxlength="5" class="input_6_table" name="autofw_outport_x_0" onKeyPress="return validator.isPortRange(this,event)">
</td>
<td width="10%">
<select name="autofw_outproto_x_o" class="input_option">
<option value="TCP" >TCP</option>
<option value="UDP" >UDP</option>
<option value="BOTH">BOTH</option>
</select>
</div>
</td>
<td width="21%">
<input type="text" maxlength="5" class="input_6_table" name="autofw_inport_x_0" onKeyPress="return validator.isPortRange(this,event)">
</td>
<td width="10%">
<select name="autofw_inproto_x_o" class="input_option">
<option value="TCP" >TCP</option>
<option value="UDP" >UDP</option>
<option value="BOTH">BOTH</option>
</select>
</td>
<td width="16%">
<input type="button" class="add_btn" onClick="addRow_Group(14);" name="" value="">
</td>
</tr>
</table>
<div id="autofw_rulelist_Block"></div>
<div class="apply_gen">
<input name="button" type="button" class="button_gen" onclick="applyRule();" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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

<!--Advanced_PortTrigger_Content.asp-->
</html>

