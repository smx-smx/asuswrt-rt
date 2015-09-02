<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_VPN_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<style>
.vpndetails{
	display: none;
}
</style>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';

<% login_state_hook(); %>
var wireless = []; // [[MAC, associated, authorized], ...]
var pptpd_clientlist_array_ori = '<% tcWebApi_char_to_ascii("PPTP_Entry", "pptpd_clientlist", "s") %>';
var pptpd_clientlist_array = decodeURIComponent(pptpd_clientlist_array_ori);
var pptpd_connected_clients = [];
var pptpd_clients = '<% tcWebApi_get("PPTP_Entry", "pptpd_clients", "s") %>';

var origin_lan_ip = '<%If tcWebApi_get("Lan_Entry0","IP","h") <> "" then tcWebApi_get("Lan_Entry0","IP","s") end if%>';
var lan_ip_subnet = origin_lan_ip.split(".")[0]+"."+origin_lan_ip.split(".")[1]+"."+origin_lan_ip.split(".")[2]+".";
var lan_ip_end = parseInt(origin_lan_ip.split(".")[3]);

var dhcp_enable = '<% tcWebApi_get("Dhcpd_Common", "Active", "s") %>';
var pool_start = '<% tcWebApi_get("Dhcpd_Common", "start", "s") %>';
var pool_end = '<% tcWebApi_get("Dhcpd_Common", "end", "s") %>';
var pool_subnet = pool_start.split(".")[0]+"."+pool_start.split(".")[1]+"."+pool_start.split(".")[2]+".";
var pool_start_end = parseInt(pool_start.split(".")[3]);
var pool_end_end = parseInt(pool_end.split(".")[3]);
var static_enable = '0';
var dhcp_staticlists = '';
var staticclist_row = dhcp_staticlists.split('&#60');
var dualwan_mode = '';

function initial(){
	show_menu();
	loadNewpptpd_clientlist();
	showpptpd_clientlist();
	parsePPTPClients();
	pptpd_connected_status();

	//*/ Viz marked 2012.04 cuz these codes move to PPTPAdvanced page
	if (pptpd_clients != "") {
		document.form._pptpd_clients_start.value = pptpd_clients.split("-")[0];
		document.form._pptpd_clients_end.value = pptpd_clients.split("-")[1];
		$('pptpd_subnet').innerHTML = pptpd_clients.split(".")[0] + "." +
				pptpd_clients.split(".")[1] + "." +
				pptpd_clients.split(".")[2] + ".";
	}

	if (document.form.pptpd_mppe.value == 0)
		document.form.pptpd_mppe.value = (1 | 4 | 8);
	document.form.pptpd_mppe_128.checked = (document.form.pptpd_mppe.value & 1);
	//document.form.pptpd_mppe_56.checked = (document.form.pptpd_mppe.value & 4);
	document.form.pptpd_mppe_40.checked = (document.form.pptpd_mppe.value & 4);
	document.form.pptpd_mppe_no.checked = (document.form.pptpd_mppe.value & 8);
	//*/	//Viz marked 2012.04

	addOnlineHelp($("faq"), ["ASUSWRT", "VPN"]);
	check_pptpd_broadcast();

	if(dualwan_mode == "lb"){
		$('wan_ctrl').style.display = "none";
		$('dualwan_ctrl').style.display = "";
	}

	show_ddns_status();

}

function show_ddns_status(){
	var ddns_enable = '<% tcWebApi_get("Ddns_Entry","Active","s") %>';
	var ddns_server_x = '<% tcWebApi_get("Ddns_Entry","SERVERNAME","s") %>';
	var ddnsName = '<% tcWebApi_get("Ddns_Entry","MYHOST","s") %>';

        if( ddns_enable == '0')
		return;
        else if(ddnsName == '')
		return;
        else
		$("set_ddns").innerHTML = '(6) If your WAN IP address is dynamic, the DDNS host name is <a href="../Advanced_ASUSDDNS_Content.asp" target="_blank" style="text-decoration: underline; font-family:Lucida Console;">' + ddnsName+ '</a>';

}

function changeMppe(){
	if (!document.form.pptpd_mppe_128.checked &&
	    //!document.form.pptpd_mppe_56.checked &&
		!document.form.pptpd_mppe_40.checked)
		document.form.pptpd_mppe_no.checked = true;
}

function pptpd_connected_status(){
	var rule_num = document.getElementById('pptpd_clientlist_table').rows.length;
	var username_status = "";	
	for(var x=0; x < rule_num; x++){
		var ind = x+1;
		username_status = "status"+ind;
		if(pptpd_connected_clients.length >0){
			for(var y=0; y<pptpd_connected_clients.length; y++) {								
				if(document.getElementById('pptpd_clientlist_table').rows[x].cells[1].title == pptpd_connected_clients[y].username){
					document.getElementById(username_status).innerHTML = '<a class="hintstyle2" href="javascript:void(0);" onClick="showPPTPClients(\''+pptpd_connected_clients[y].username+'\');"><%tcWebApi_get("String_Entry","Connected","s")%></a>';
					break;
				}		
			}
			
			if(document.getElementById(username_status).innerHTML == "") {
				document.getElementById(username_status).innerHTML = '<%tcWebApi_get("String_Entry","Disconnected","s")%>';
			}
		}
		else if(document.getElementById(username_status)) {
			document.getElementById(username_status).innerHTML = '<%tcWebApi_get("String_Entry","Disconnected","s")%>';
		}	
	}
}

function applyRule(){
	var rule_num = $('pptpd_clientlist_table').rows.length;
	var item_num = $('pptpd_clientlist_table').rows[0].cells.length;
	var tmp_value = "";

	for(i=0; i<rule_num; i++){
		tmp_value += "<"
		for(j=1; j<item_num-1; j++){
			if($('pptpd_clientlist_table').rows[i].cells[j].innerHTML.lastIndexOf("...")<0){
				tmp_value += $('pptpd_clientlist_table').rows[i].cells[j].innerHTML;
			}else{
				tmp_value += $('pptpd_clientlist_table').rows[i].cells[j].title;
			}

			if(j != item_num-2)
				tmp_value += ">";
		}
	
		if(tmp_value == "<"+"<%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%>" || tmp_value == "<")
			tmp_value = "";

		switch(i){
			case 0:
				document.form.pptpd_clientlist.value = tmp_value;
				break;
			case 1:
				document.form.pptpd_clientlist1.value = tmp_value;
				break;
			case 2:
				document.form.pptpd_clientlist2.value = tmp_value;
				break;
			case 3:
				document.form.pptpd_clientlist3.value = tmp_value;
				break;
			case 4:
				document.form.pptpd_clientlist4.value = tmp_value;
				break;
			case 5:
				document.form.pptpd_clientlist5.value = tmp_value;
				break;
			case 6:
				document.form.pptpd_clientlist6.value = tmp_value;
				break;
			case 7:
				document.form.pptpd_clientlist7.value = tmp_value;
				break;
			case 8:
				document.form.pptpd_clientlist8.value = tmp_value;
				break;
			case 9:
				document.form.pptpd_clientlist9.value = tmp_value;
				break;
			case 10:
				document.form.pptpd_clientlist10.value = tmp_value;
				break;
			case 11:
				document.form.pptpd_clientlist11.value = tmp_value;
				break;
			case 12:
				document.form.pptpd_clientlist12.value = tmp_value;
				break;
			case 13:
				document.form.pptpd_clientlist13.value = tmp_value;
				break;
			case 14:
				document.form.pptpd_clientlist14.value = tmp_value;
				break;
			case 15:
				document.form.pptpd_clientlist15.value = tmp_value;
				break;
			default:
					document.form.pptpd_clientlist1.value = "Out of Limitation";
					break;
		}
		tmp_value = "";
	}

	/* Viz marked 2012.04
	document.form.pptpd_clients.value = document.form._pptpd_clients_start.value + "-" + document.form._pptpd_clients_end.value;

	document.form.pptpd_mppe.value = 0;
	if (document.form.pptpd_mppe_128.checked)
		document.form.pptpd_mppe.value |= 1;
	//if (document.form.pptpd_mppe_56.checked)
	//	document.form.pptpd_mppe.value |= 2;
	if (document.form.pptpd_mppe_40.checked)
		document.form.pptpd_mppe.value |= 4;
	if (document.form.pptpd_mppe_no.checked)
		document.form.pptpd_mppe.value |= 8;

	*/ //Viz marked 2012.04

	showLoading();
	document.form.submit();
}

function addRow(obj, head){
	if(head == 1)
		pptpd_clientlist_array += "<" /*&#60*/
	else
		pptpd_clientlist_array += ">" /*&#62*/

	pptpd_clientlist_array += obj.value;

	obj.value = "";
}

function validForm(){

		if(document.form.pptpd_clientlist_username.value==""){
				alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
				document.form.pptpd_clientlist_username.focus();
				return false;

		}else if(!Block_chars(document.form.pptpd_clientlist_username, [" ", "@", "*", "+", "|", ":", "?", "<", ">", ",", ".", "/", ";", "[", "]", "\\", "=", "\"", "&" ])){
				return false;
		}

		if(document.form.pptpd_clientlist_password.value==""){
				alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
				document.form.pptpd_clientlist_password.focus();
				return false;
		}else if(!Block_chars(document.form.pptpd_clientlist_password, ["<", ">", "&"])){
				return false;
		}

		return true;
}

function addRow_Group(upper){
	var rule_num = $('pptpd_clientlist_table').rows.length;
	var item_num = $('pptpd_clientlist_table').rows[0].cells.length;
	if(rule_num >= upper){
		alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
		return false;
	}

	if(validForm()){
		//Viz check same rule  //match(username) is not accepted
		if(item_num >=2){
			for(i=0; i<rule_num; i++){
				if(document.form.pptpd_clientlist_username.value.toLowerCase() == $('pptpd_clientlist_table').rows[i].cells[0].innerHTML.toLowerCase()){
					alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");
					document.form.pptpd_clientlist_username.focus();
					document.form.pptpd_clientlist_username.select();
					return false;
				}
			}
		}

		addRow(document.form.pptpd_clientlist_username ,1);
		addRow(document.form.pptpd_clientlist_password, 0);
		showpptpd_clientlist();
		pptpd_connected_status();
	}
}

function del_Row(r){
	var i=r.parentNode.parentNode.rowIndex;
	$('pptpd_clientlist_table').deleteRow(i);
	var pptpd_clientlist_value = "";
	for(k=0; k<$('pptpd_clientlist_table').rows.length; k++){
		for(j=0; j<$('pptpd_clientlist_table').rows[k].cells.length-1; j++){
			if(j == 0)
				pptpd_clientlist_value += "<";
			else {
			pptpd_clientlist_value += $('pptpd_clientlist_table').rows[k].cells[0].firstChild.innerHTML;
			pptpd_clientlist_value += ">";
			pptpd_clientlist_value += $('pptpd_clientlist_table').rows[k].cells[1].firstChild.innerHTML;
			}
		}
	}

	pptpd_clientlist_array = pptpd_clientlist_value;
	if(pptpd_clientlist_array == "")
		showpptpd_clientlist();
}

function edit_Row(r){
	var i=r.parentNode.parentNode.rowIndex;
	document.form.pptpd_clientlist_username.value = $('pptpd_clientlist_table').rows[i].cells[0].innerHTML;
	document.form.pptpd_clientlist_password.value = $('pptpd_clientlist_table').rows[i].cells[1].innerHTML;
	del_Row(r);
}

function loadNewpptpd_clientlist(){
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist1","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist2","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist3","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist4","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist5","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist6","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist7","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist8","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist9","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist10","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist11","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist12","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist13","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist14","s") %>";
pptpd_clientlist_array_ori += "<% TCWebApi_get("PPTP_Entry","pptpd_clientlist15","s") %>";

pptpd_clientlist_array = decodeURIComponent(pptpd_clientlist_array_ori);
}

var overlib_str0 = new Array();	//Viz add 2013.04 for record longer VPN client username/pwd
var overlib_str1 = new Array();	//Viz add 2013.04 for record longer VPN client username/pwd
function showpptpd_clientlist(){
	var pptpd_clientlist_row = pptpd_clientlist_array.split('<');
	var code = "";

	code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="pptpd_clientlist_table">';
	if(pptpd_clientlist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;" colspan="4"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td></tr>';
	else{
		for(var i = 1; i < pptpd_clientlist_row.length; i++){
			overlib_str0[i] = "";
			overlib_str1[i] = "";
			code +='<tr id="row'+i+'">';
			var pptpd_clientlist_col = pptpd_clientlist_row[i].split('>');
			code +='<td width="15%" id="status'+i+'"></td>';
			for(var j = 0; j < pptpd_clientlist_col.length; j++){
				if(j == 0){
						if(pptpd_clientlist_col[0].length >32){
								overlib_str0[i] += pptpd_clientlist_col[0];
								pptpd_clientlist_col[0]=pptpd_clientlist_col[0].substring(0, 30)+"...";
								code +='<td width="35%" title="'+overlib_str0[i]+'">'+ pptpd_clientlist_col[0] +'</td>';
						}else
								code +='<td width="35%" title="'+pptpd_clientlist_col[0]+'">'+ pptpd_clientlist_col[0] +'</td>';
				}
				else if(j ==1){
						if(pptpd_clientlist_col[1].length >32){
								overlib_str1[i] += pptpd_clientlist_col[1];
								pptpd_clientlist_col[1]=pptpd_clientlist_col[1].substring(0, 30)+"...";
								code +='<td width="35%" title="'+overlib_str1[i]+'">'+ pptpd_clientlist_col[1] +'</td>';
						}else
								code +='<td width="35%">'+ pptpd_clientlist_col[1] +'</td>';
				}
			}
			code +='<td width="15%"><!--input class="edit_btn" onclick="edit_Row(this);" value=""/-->';
			code +='<input class="remove_btn" onclick="del_Row(this);" value=""/></td></tr>';
		}
	}
	code +='</table>';
	$("pptpd_clientlist_Block").innerHTML = code;
}

function parsePPTPClients(){	
	var Loginfo = document.getElementById("pptp_connected_info").firstChild.innerHTML;
	var lines = Loginfo.split('\n');
	if(Loginfo == "")
		return;
	
	Loginfo = Loginfo.replace('\r\n', '\n');
	Loginfo = Loginfo.replace('\n\r', '\n');
	Loginfo = Loginfo.replace('\r', '\n');			
	for (i = 0; i < lines.length; i++){
		var fields = lines[i].split(' ');
		if(fields.length != 5) 
			continue;		
   		
		pptpd_connected_clients.push({"username":fields[4],"remoteIP":fields[3],"VPNIP":fields[2]});
	}
}

var _caption = "";
function showPPTPClients(uname) {
	var statusmenu = "";
	var statustitle = "";
	statustitle += "<div style=\"text-decoration:underline;\">VPN IP ( Remote IP )</div>";
	_caption = statustitle;
	
	for (i = 0; i < pptpd_connected_clients.length; i++){
		if(uname == pptpd_connected_clients[i].username){		
			statusmenu += "<div>"+pptpd_connected_clients[i].VPNIP+" \t( "+pptpd_connected_clients[i].remoteIP+" )</div>";
		}				
	}
		
	return overlib(statusmenu, WIDTH, 260, OFFSETX, -360, LEFT, STICKY, CAPTION, _caption, CLOSETITLE, '');	
}

// test if Private ip
function valid_IP(obj_name, obj_flag){
	// A : 1.0.0.0~126.255.255.255
	// B : 127.0.0.0~127.255.255.255 (forbidden)
	// C : 128.0.0.0~255.255.255.254
	var A_class_start = inet_network("1.0.0.0");
	var A_class_end = inet_network("126.255.255.255");
	var B_class_start = inet_network("127.0.0.0");
	var B_class_end = inet_network("127.255.255.255");
	var C_class_start = inet_network("128.0.0.0");
	var C_class_end = inet_network("255.255.255.255");

	var ip_obj = obj_name;
	var ip_num = inet_network(ip_obj.value); //-1 means nothing

	//1~254
	if(obj_name.value.split(".")[3] < 1 || obj_name.value.split(".")[3] > 254){
		alert('<%tcWebApi_get("String_Entry","JS_validrange","s")%> ' + 1 + ' <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> ' + 254);
		obj_name.focus();
		return false;
	}

	if(ip_num > A_class_start && ip_num < A_class_end)
		return true;
	else if(ip_num > B_class_start && ip_num < B_class_end){
		alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
	else if(ip_num > C_class_start && ip_num < C_class_end)
		return true;
	else{
		alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
}

function setEnd(){
	var end="";
	var pptpd_clients_subnet = document.form._pptpd_clients_start.value.split(".")[0]
					+"."+document.form._pptpd_clients_start.value.split(".")[1]
					+"."+document.form._pptpd_clients_start.value.split(".")[2]
					+".";
	var pptpd_clients_start_ip = parseInt(document.form._pptpd_clients_start.value.split(".")[3]);

	$('pptpd_subnet').innerHTML = pptpd_clients_subnet;

	end = pptpd_clients_start_ip+9;
	if(end >254) end = 254;

	document.form._pptpd_clients_end.value = end;
}
function check_pptpd_broadcast(){
	if(document.form.pptpd_broadcast.value =="ppp" || document.form.pptpd_broadcast.value =="br0ppp")
		$('pptpd_broadcast_ppp_yes').checked="true";
	else
		$('pptpd_broadcast_ppp_no').checked="true";
}

function set_pptpd_broadcast(obj){
	var pptpd_temp;
	pptpd_temp = document.form.pptpd_broadcast.value;

	if(obj.value ==1){
		if(pptpd_temp == "br0")
			document.form.pptpd_broadcast.value="br0ppp";
		else
			document.form.pptpd_broadcast.value="ppp";
	}
	else{
		if(pptpd_temp == "br0ppp")
			document.form.pptpd_broadcast.value="br0";
		else
			document.form.pptpd_broadcast.value="disable";
	}
}

function switchPage(page){
	if(page == "1")	
		return false;
	else
		location.href = "/Advanced_VPNAdvanced_Content.asp";
}
</script>
</head>

<body onload="initial();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<div id="pptp_connected_info" style="display:none;"><pre><% nvram_dump("pptp_connected",""); %></pre></div>
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
			<input type="hidden" name="current_page" value="Advanced_VPN_Content.asp">
			<input type="hidden" name="next_page" value="Advanced_VPN_Content.asp">
			<input type="hidden" name="next_host" value="">
			<input type="hidden" name="modified" value="0">
			<input type="hidden" name="action_mode" value="apply">
			<input type="hidden" name="action_wait" value="5">
			<input type="hidden" name="action_script" value="restart_pptpd">
			<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
			<input type="hidden" name="firmver" value="">
			<input type="hidden" name="wl_ssid" value="">
			<input type="hidden" name="pptpd_clientlist" value="">
			<input type="hidden" name="pptpd_clientlist1" value="">
			<input type="hidden" name="pptpd_clientlist2" value="">
			<input type="hidden" name="pptpd_clientlist3" value="">
			<input type="hidden" name="pptpd_clientlist4" value="">
			<input type="hidden" name="pptpd_clientlist5" value="">
			<input type="hidden" name="pptpd_clientlist6" value="">
			<input type="hidden" name="pptpd_clientlist7" value="">
			<input type="hidden" name="pptpd_clientlist8" value="">
			<input type="hidden" name="pptpd_clientlist9" value="">
			<input type="hidden" name="pptpd_clientlist10" value="">
			<input type="hidden" name="pptpd_clientlist11" value="">
			<input type="hidden" name="pptpd_clientlist12" value="">
			<input type="hidden" name="pptpd_clientlist13" value="">
			<input type="hidden" name="pptpd_clientlist14" value="">
			<input type="hidden" name="pptpd_clientlist15" value="">
			<input type="hidden" name="pptpd_clients" value="<% tcWebApi_get("PPTP_Entry","pptpd_clients", "s") %>">
			<input type="hidden" name="pptpd_mppe" value="<% tcWebApi_get("PPTP_Entry", "pptpd_mppe", "s") %>">
			<input type="hidden" name="pptpd_broadcast" value="<% tcWebApi_get("PPTP_Entry", "pptpd_broadcast", "s") %>">
			<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
				<tr>
					<td valign="top" >
						<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
							<tbody>
								<tr>
									<td bgcolor="#4D595D" valign="top">
									<div>&nbsp;</div>
									<div class="formfonttitle"><%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%> - <%tcWebApi_get("String_Entry","menu5_1_1","s")%></div>
									<div align="right" style="margin-top:-35px;">
     		   					<select onchange="switchPage(this.options[this.selectedIndex].value)" class="input_option">
											<option value="1" selected><%tcWebApi_get("String_Entry","menu5_1_1","s")%></option>
											<option value="2"><%tcWebApi_get("String_Entry","menu5","s")%></option>
										</select>
									</div>
									<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
									<div class="formfontdesc"><%tcWebApi_get("String_Entry","PPTP_desc","s")%></div>
									<div id="wan_ctrl" class="formfontdesc"><%tcWebApi_get("String_Entry","PPTP_desc2","s")%> <% tcWebApi_get("DeviceInfo_PVC", "WanIP", "s") %></div>
									<div id="dualwan_ctrl" style="display:none;" class="formfontdesc"><%tcWebApi_get("String_Entry","PPTP_desc2","s")%> <span class="formfontdesc">Primary WAN IP : <% nvram_get("wan0_ipaddr"); %> </span><span class="formfontdesc">Secondary WAN IP : <% nvram_get("wan1_ipaddr"); %> </span></div>
									<div id ="set_ddns" class="formfontdesc" style="margin-top:-10px;font-weight: bolder;"><%tcWebApi_get("String_Entry","PPTP_desc3","s")%></div>
									<div class="formfontdesc" style="margin-top:-10px;">
										(7) <a id="faq" href="" target="_blank" style="font-family:Lucida Console;text-decoration:underline;"><%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%> FAQ</a>
									</div>
									<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
										<thead>
											<tr>
												<td colspan="3" id="GWStatic"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
											</tr>
										</thead>
										<tr>
											<th><%tcWebApi_get("String_Entry","vpn_enable","s")%></th>
											<td>
												<input type="radio" value="1" name="pptpd_enable" <%if tcWebApi_get("PPTP_Entry", "pptpd_enable", "h") = "1" then asp_write("checked") end if%> /><% tcWebApi_Get("String_Entry", "checkbox_Yes", "s") %>
												<input type="radio" value="0" name="pptpd_enable" <%if tcWebApi_get("PPTP_Entry", "pptpd_enable", "h") = "0" then asp_write("checked") end if%> /><% tcWebApi_Get("String_Entry", "checkbox_No", "s") %>
												<!-- need to unify -->
												<!--select name="pptpd_enable" class="input_option">
													<option class="content_input_fd" value="0" <% nvram_match("pptpd_enable", "0","selected"); %>><% tcWebApi_Get("String_Entry", "btn_disable", "s") %></option>
													<option class="content_input_fd" value="1"<% nvram_match("pptpd_enable", "1","selected"); %>><% tcWebApi_Get("String_Entry", "WC11b_WirelessCtrl_button1name", "s") %></option>
												</select-->
											</td>
										</tr>
										<tr>
											<th><%tcWebApi_get("String_Entry","vpn_network_place","s")%></th>
											<td>
												<input type="radio" value="1" id="pptpd_broadcast_ppp_yes" name="pptpd_broadcast_ppp" onchange="set_pptpd_broadcast(this);"/><% tcWebApi_Get("String_Entry", "checkbox_Yes", "s") %>
												<input type="radio" value="0" id="pptpd_broadcast_ppp_no" name="pptpd_broadcast_ppp" onchange="set_pptpd_broadcast(this);"/><% tcWebApi_Get("String_Entry", "checkbox_No", "s") %>
											</td>
										</tr>
										<tr class="vpndetails">
											<th><% tcWebApi_Get("String_Entry", "PPPC_Authentication_in", "s") %></th>
											<td>
												<select name="pptpd_chap" class="input_option">
													<option value="0" <% nvram_match("pptpd_chap", "0","selected"); %>><% tcWebApi_Get("String_Entry", "Auto", "s") %></option>
													<option value="1" <% nvram_match("pptpd_chap", "1","selected"); %>>MS-CHAPv1</option>
													<option value="2" <% nvram_match("pptpd_chap", "2","selected"); %>>MS-CHAPv2</option>
												</select>
											</td>
										</tr>
										<tr class="vpndetails">
											<th><% tcWebApi_Get("String_Entry", "MPPE_Encryp", "s") %></th>
											<td>
												<input type="checkbox" class="input" name="pptpd_mppe_128" onClick="return changeMppe();">MPPE-128<br>
												<input type="checkbox" class="input" name="pptpd_mppe_40" onClick="return changeMppe();">MPPE-40<br>
												<input type="checkbox" class="input" name="pptpd_mppe_no" onClick="return changeMppe();"><%tcWebApi_get("String_Entry","checkbox_No","s")%> Encryption
											</td>
										</tr>
										<tr class="vpndetails">
											<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","IPC_x_DNSServer1_in","s")%></a></th>
											<td><input type="text" maxlength="15" class="input_15_table" name="pptpd_dns1" value="" onkeypress="return is_ipaddr(this, event)" ></td>
										</tr>
										<tr class="vpndetails">
											<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","IPC_x_DNSServer2_in","s")%></a></th>
											<td><input type="text" maxlength="15" class="input_15_table" name="pptpd_dns2" value="" onkeypress="return is_ipaddr(this, event)" ></td>
										</tr>

										<tr class="vpndetails">
											<th>WINS 1</th>
											<td><input type="text" maxlength="15" class="input_15_table" name="pptpd_wins1" value="" onkeypress="return is_ipaddr(this, event)" ></td>
										</tr>

										<tr class="vpndetails">
											<th>WINS 2</th>
											<td><input type="text" maxlength="15" class="input_15_table" name="pptpd_wins2" value="" onkeypress="return is_ipaddr(this, event)" ></td>
										</tr>
										<tr class="vpndetails">
											<th><%tcWebApi_get("String_Entry","vpn_client_ip","s")%></th>
											<td>
												<input type="text" maxlength="15" class="input_15_table" name="_pptpd_clients_start" onBlur="setEnd();" onKeyPress="return is_ipaddr(this, event);" value=""/> ~
												<span id="pptpd_subnet" style="font-family: Lucida Console;color: #FFF;"></span><input type="text" maxlength="3" class="input_3_table" name="_pptpd_clients_end" value=""/><span style="color:#FFCC00;"> <%tcWebApi_get("String_Entry","vpn_maximum_clients","s")%></span>
												<br><span id="pptpd_conflict"></span>
											</td>
										</tr>
									</table>

									<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" style="margin-top:8px;">
										<thead>
											<tr>
												<td colspan="4" id="GWStatic"><% tcWebApi_Get("String_Entry", "Username_Pwd", "s") %> (<%tcWebApi_get("String_Entry","List_limit","s")%> 16)</td>
											</tr>
										</thead>
										<tr>
											<th><%tcWebApi_get("String_Entry","PPPC_x_WANLink_in","s")%></th>
											<th><%tcWebApi_get("String_Entry","PPPC_UserName_in","s")%></th>
											<th><%tcWebApi_get("String_Entry","PPPC_Password_in","s")%></th>
											<th><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
										</tr>
										<tr>
											<td width="15%" style="text-align:center;">-</td>
											<td width="35%">
												<input type="text" class="input_25_table" maxlength="64" name="pptpd_clientlist_username" onKeyPress="return is_string(this, event)">
											</td>
											<td width="35%">
												<input type="text" class="input_25_table" maxlength="64" name="pptpd_clientlist_password" onKeyPress="return is_string(this, event)">
											</td>
											<td width="15%">
												<div><input type="button" class="add_btn" onClick="addRow_Group(16);" value=""></div>
											</td>
										</tr>
									</table>

									<div id="pptpd_clientlist_Block"></div>
									<div class="apply_gen">
										<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
									</div>
									</td>
								</tr>
							</tbody>
						</table>
					</td>
				</tr>
			</table>
		<td width="10" align="center" valign="top">&nbsp;</td>
	</tr>
</table>
</form>
<div id="footer"></div>
</body>

<!--Advanced_VPN_Content.asp-->
</html>

