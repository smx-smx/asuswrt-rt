<%
If Request_Form("editFlag") = "1" then
	tcWebApi_Set("KeywordFilter_Common","Active","keyword_enable_x")
	tcWebApi_Set("KeywordFilter_Common","Keyword_Rule_List","keyword_rulelist")
  	tcWebApi_commit("KeywordFilter_Common")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_URLFilter_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_5_2","s")%></title>
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
var keyword_rulelist_array = '<% If tcWebApi_get("KeywordFilter_Common","Keyword_Rule_List","h") <> "" then  tcWebApi_get("KeywordFilter_Common","Keyword_Rule_List","s") end if %>';
function initial(){
	show_menu();
	load_body();
	show_keyword_rulelist();
	enable_keyword();
	enable_keyword_1();
}
function show_keyword_rulelist(){
	var keyword_rulelist_row = keyword_rulelist_array.split('<');
	var code = "";
	code +='<table width="100%" border="1" cellspacing="0" cellpadding="4" align="center" class="list_table" id="keyword_rulelist_table">';
	if(keyword_rulelist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td>';
	else{
		for(var i =1; i < keyword_rulelist_row.length; i++){
			code +='<tr id="row'+i+'">';
			code +='<td width="80%">'+ keyword_rulelist_row[i] +'</td>'; //Url keyword
			code +='<td width="20%">';
			code +="<input class=\"remove_btn\" type=\"button\" onclick=\"deleteRow(this);\" value=\"\"/></td>";
		}
	}
	code +='</tr></table>';
	$("keyword_rulelist_Block").innerHTML = code;
}
function deleteRow(r){
	var i=r.parentNode.parentNode.rowIndex;
	$('keyword_rulelist_table').deleteRow(i);
	var keyword_rulelist_value = "";
	for(i=0; i<$('keyword_rulelist_table').rows.length; i++){
		keyword_rulelist_value += "<";
		keyword_rulelist_value += $('keyword_rulelist_table').rows[i].cells[0].innerHTML;
	}
	keyword_rulelist_array =keyword_rulelist_value;
	if(keyword_rulelist_array == "")
		show_keyword_rulelist();
}
function addRow(obj, upper){
	var rule_num = $('keyword_rulelist_table').rows.length;
	var item_num = $('keyword_rulelist_table').rows[0].cells.length;
	if(rule_num >= upper){
		alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
		return false;
	}
	if(obj.value==""){
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
		obj.focus();
		obj.select();
	}else{
		for(i=0; i<rule_num; i++){
			for(j=0; j<item_num-1; j++){ //only 1 value column
				if(obj.value == $('keyword_rulelist_table').rows[i].cells[j].innerHTML){
					alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");
					return false;
				}
			}
		}
		keyword_rulelist_array += "<";
		keyword_rulelist_array += obj.value;
		obj.value = "";
		show_keyword_rulelist();
	}
}
function applyRule(){
	if(validForm()){
		var rule_num = $('keyword_rulelist_table').rows.length;
		var item_num = $('keyword_rulelist_table').rows[0].cells.length;
		var tmp_value = "";
		for(i=0; i<rule_num; i++){
			tmp_value += "<"
			for(j=0; j<item_num-1; j++){
				tmp_value += $('keyword_rulelist_table').rows[i].cells[j].innerHTML;
				if(j != item_num-2)
					tmp_value += ">";
			}
		}
		if(tmp_value == "<"+"<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>" || tmp_value == "<")
		tmp_value = "";
		document.form.keyword_rulelist.value = tmp_value;
		document.form.action = "/cgi-bin/Advanced_KeywordFilter_Content.asp";
		document.form.editFlag.value = "1" ;
		showLoading(5);
		setTimeout('location = "'+ location.pathname +'";', 5000);
		document.form.submit();
	}
}
function validForm(){
return true;
}
function enable_keyword(){
	return change_common_radio(document.form.keyword_enable_x, 'FirewallConfig', 'keyword_enable_x', '1')
}
function enable_keyword_1(){
	return change_common_radio(document.form.keyword_enable_x_1, 'FirewallConfig', 'keyword_enable_x_1', '1')
}
function done_validating(action){
refreshpage();
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_KeywordFilter_Content.asp">
<input type="hidden" name="next_page" value="Advanced_KeywordFilter_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="action_script" value="restart_firewall">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="keyword_num_x_0" value="" readonly="1">
<input type="hidden" name="keyword_rulelist" value="">
<input type="hidden" name="keyword_enable_x_orig" value="0">
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
		<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_5","s")%> - <%tcWebApi_get("String_Entry","menu5_5_5","s")%></div>
		<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		<div class="formfontdesc"><%tcWebApi_get("String_Entry","FC_KeywordFilterEnable_sd","s")%></div>
		<div class="formfontdesc"><%tcWebApi_get("String_Entry","FC_KeywordFilterEnable_sd2","s")%></div>	

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
						<td colspan="4"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
          <th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(9,1);"><%tcWebApi_get("String_Entry","FC_KeywordFilterEnable_in","s")%></th>
<td>
<input type="radio" value="1" name="keyword_enable_x" onClick="enable_keyword();" <% if tcWebApi_get("KeywordFilter_Common","Active","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","CTL_Enabled","s")%>
<input type="radio" value="0" name="keyword_enable_x" onClick="enable_keyword();" <% if tcWebApi_get("KeywordFilter_Common","Active","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","CTL_Disabled","s")%>
</td>
</tr>
</table>
<table width="100%" style="display:none;" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
          <th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(9,1);"><%tcWebApi_get("String_Entry","FC_KeywordFilterEnable_in","s")%> 1:</th>
<td>
<input type="radio" value="1" name="keyword_enable_x_0" onClick="enable_keyword();" ><%tcWebApi_get("String_Entry","CTL_Enabled","s")%>
<input type="radio" value="0" name="keyword_enable_x_0" onClick="enable_keyword();" checked><%tcWebApi_get("String_Entry","CTL_Disabled","s")%>
</td>
</tr>
<tr>
          <th><a class="hintstyle"  href="javascript:void(0);" onClick="openHint(9,1);"><%tcWebApi_get("String_Entry","FC_KeywordFilterEnable_in","s")%> 2:</th>
<td>
<input type="radio" value="1" name="keyword_enable_x_1" onClick="enable_keyword_1();" ><%tcWebApi_get("String_Entry","CTL_Enabled","s")%>
<input type="radio" value="0" name="keyword_enable_x_1" onClick="enable_keyword_1();" checked><%tcWebApi_get("String_Entry","CTL_Disabled","s")%>
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
<thead>
<tr>
						<td colspan="4"><%tcWebApi_get("String_Entry","FC_KeywordList_groupitemname","s")%> (<%tcWebApi_get("String_Entry","List_limit","s")%> 27)</td>
</tr>
</thead>
<tr>
		<th width="80%"><%tcWebApi_get("String_Entry","FC_KeywordList_groupitemname","s")%></th>
		<th width="20%"><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
</tr>
<tr>
<td width="80%">
<input type="text" maxlength="20" class="input_32_table" name="keyword_keyword_x_0" onKeyPress="return validator.isString(this, event)">
</td>
<td width="20%">
<input class="add_btn" type="button" onClick="if(validForm()){addRow(document.form.keyword_keyword_x_0, 27);}" value="">
</td>
</tr>
</table>
<div id="keyword_rulelist_Block"></div>
<div class="apply_gen">
<input type="button" class="button_gen" onclick="applyRule()" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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

<!--Advanced_URLFilter_Content.asp-->
</html>

