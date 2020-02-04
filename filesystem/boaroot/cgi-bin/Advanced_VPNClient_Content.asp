<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> - VPN Client Settings</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<style type="text/css">
.contentM_qis{
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:200;
	background-color:#2B373B;
	display:none;
	margin-left: 30%;
	top: 290px;
	width:650px;
}
.contentM_qis_manual{
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:200;
	background-color:#2B373B;
	margin-left: -30px;
	margin-left: -100px \9;
	margin-top:-400px;
	width:740px;
	box-shadow: 3px 3px 10px #000;
}

.QISform_wireless{
	width:600px;
	font-size:12px;
	color:#FFFFFF;
	margin-top:10px;
	*margin-left:10px;
}

.QISform_wireless thead{
	font-size:15px;
	line-height:20px;
	color:#FFFFFF;
}

.QISform_wireless th{
	padding-left:10px;
	*padding-left:30px;
	font-size:12px;
	font-weight:bolder;
	color: #FFFFFF;
	text-align:left;
}

.description_down{
	margin-top:10px;
	margin-left:10px;
	padding-left:5px;
	font-weight:bold;
	line-height:140%;
	color:#ffffff;
}

</style>
<script>
var $j = jQuery.noConflict();


var vpnc_clientlist_array = decodeURIComponent('<% tcWebApi_char_to_ascii("VPNC_Entry","clientlist","s") %>');
var vpnc_pptp_options_x_list_array = decodeURIComponent('<% tcWebApi_char_to_ascii("VPNC_Entry", "pptp_options_list","s") %>');
var overlib_str0 = new Array();	//Viz add 2013.04 for record longer VPN client username/pwd
var overlib_str1 = new Array();	//Viz add 2013.04 for record longer VPN client username/pwd

var restart_vpncall_flag = 0; //Viz add 2014.04 for Edit Connecting rule then restart_vpncall

function initial(){
	show_menu();
	show_vpnc_rulelist();

	if(vpnc_support == -1 && openvpnd_support != -1)
		document.getElementById("rules_limit").innerHTML = "(<%tcWebApi_get("String_Entry","List_limit","s")%>&nbsp;5)";
	else
		document.getElementById("rules_limit").innerHTML = "(<%tcWebApi_get("String_Entry","List_limit","s")%>&nbsp;10)";
}

function Add_profile(){
	document.getElementById('importOvpnFile').style.display = "none";
	document.form.vpnc_des_edit.value = "";
	document.form.vpnc_svr_edit.value = "";
	document.form.vpnc_account_edit.value = "";
	document.form.vpnc_pwd_edit.value = "";
	document.form.selPPTPOption.value = "auto";
	document.getElementById("pptpOptionHint").style.display = "none";
	document.vpnclientForm.vpnc_openvpn_des.value = "";
	document.vpnclientForm.vpnc_openvpn_username.value = "";
	document.vpnclientForm.vpnc_openvpn_pwd.value = "";
	document.vpnclientForm.file.value = "";
	document.openvpnCAForm.file.value = "";
	document.getElementById("cbManualImport").checked = false;
	document.getElementById('edit_vpn_crt_client_ca').value = "";
	document.getElementById('edit_vpn_crt_client_crt').value = "";
	document.getElementById('edit_vpn_crt_client_key').value = "";
	document.getElementById('edit_vpn_crt_client_static').value = "";
	document.getElementById('edit_vpn_crt_client_crl').value = "";
	manualImport(false);
	tabclickhandler(0);

	document.getElementById("cancelBtn").style.display = "";
	document.getElementById("cancelBtn_openvpn").style.display = "";
	document.getElementById("pptpcTitle").style.display = "";
	document.getElementById("l2tpcTitle").style.display = "";
	document.getElementById("opencTitle").style.display = "";
	document.getElementById("pptpcTitle_openvpn").style.display = "";
	document.getElementById("l2tpcTitle_openvpn").style.display = "";
	document.getElementById("opencTitle_openvpn").style.display = "";
	$j("#vpnc_setting").fadeIn(300);

	if (openvpnd_support == -1){
		document.getElementById('opencTitle').style.display = "none";
	}
	else if ((vpnc_clientlist_array.split("OpenVPN>1").length == 2 && vpnc_clientlist_array.split("OpenVPN>2").length == 2 &&
			vpnc_clientlist_array.split("OpenVPN>3").length == 2 && vpnc_clientlist_array.split("OpenVPN>4").length == 2 &&
			vpnc_clientlist_array.split("OpenVPN>5").length == 2)){
		document.getElementById('opencTitle').style.display = "none";
	}
	else{
		document.getElementById('opencTitle').style.display = "";
	}
}

function cancel_add_rule(){
	restart_vpncall_flag = 0;
	idx_tmp = "";
	$j("#vpnc_setting_openvpn").fadeOut(300);
	$j("#vpnc_setting").fadeOut(300);
}

function addRow_Group(upper, flag, idx){
	document.openvpnManualForm.vpn_crt_client_ca.disabled = true;
	document.openvpnManualForm.vpn_crt_client_crt.disabled = true;
	document.openvpnManualForm.vpn_crt_client_key.disabled = true;
	document.openvpnManualForm.vpn_crt_client_static.disabled = true;
	document.openvpnManualForm.vpn_crt_client_crl.disabled = true;
	idx = parseInt(idx);

	if(idx >= 0){		//idx: edit row
		var table_id = "vpnc_clientlist_table";
		var rule_num = document.getElementById(table_id).rows.length;
		var item_num = document.getElementById(table_id).rows[0].cells.length;
		if(flag == 'PPTP' || flag == 'L2TP'){
			description_obj = document.form.vpnc_des_edit;
			type_obj = document.form.vpnc_type;
			server_obj = document.form.vpnc_svr_edit;
			username_obj = document.form.vpnc_account_edit;
			password_obj = document.form.vpnc_pwd_edit;

		}else{	//OpenVPN: openvpn
			description_obj = document.vpnclientForm.vpnc_openvpn_des;
			type_obj = document.vpnclientForm.vpnc_type;
			server_obj = document.vpnclientForm.vpnc_openvpn_unit_edit;
			username_obj = document.vpnclientForm.vpnc_openvpn_username;
			password_obj = document.vpnclientForm.vpnc_openvpn_pwd;
		}

		if(validForm(flag)){
			var vpnc_clientlist_row = vpnc_clientlist_array.split('<');
			duplicateCheck.tmpIdx = "";
			duplicateCheck.saveTotmpIdx(idx);
			duplicateCheck.tmpStr = "";
			duplicateCheck.saveToTmpStr(type_obj, 0);
			duplicateCheck.saveToTmpStr(server_obj, 0);
			duplicateCheck.saveToTmpStr(username_obj, 0);
			duplicateCheck.saveToTmpStr(password_obj, 0);
			if(duplicateCheck.isDuplicate()){
				username_obj.focus();
				username_obj.select();
				alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>")
				return false;
			}

			vpnc_clientlist_row[idx+1] = description_obj.value+">"+type_obj.value+">"+server_obj.value+">"+username_obj.value+">"+password_obj.value;
			vpnc_clientlist_array = vpnc_clientlist_row.join("<");

			//edit vpnc_pptp_options_x_list
			if(vpnc_pptp_options_x_list_array == "") {
				//idx: NaN=Add i=edit row
				vpnc_pptp_options_x_list_array = reFillEmptyPPTPOPtion(idx, document.form.selPPTPOption.value, vpnc_clientlist_array);
			}
			else {
				vpnc_pptp_options_x_list_array = handlePPTPOPtion(idx, document.form.selPPTPOption.value, vpnc_clientlist_array);
			}

			document.vpnclientForm.vpnc_pptp_options_x_list.value = vpnc_pptp_options_x_list_array;
			document.form.vpnc_pptp_options_x_list.value = vpnc_pptp_options_x_list_array;
			show_vpnc_rulelist();

			if(restart_vpncall_flag == 1){	//restart_vpncall
				var vpnc_clientlist_row = vpnc_clientlist_array.split('<');
				var vpnc_clientlist_col = vpnc_clientlist_row[idx+1].split('>');
				for(var j=0; j<vpnc_clientlist_col.length; j++){
					if(j == 0){
						document.form.vpnc_des_edit.value = vpnc_clientlist_col[0];
					}
					else if(j ==1){
						if(vpnc_clientlist_col[1] == "PPTP")
							document.form.vpnc_proto.value = "pptp";
						else if(vpnc_clientlist_col[1] == "L2TP")
							document.form.vpnc_proto.value = "l2tp";
						else	//OpenVPN
							document.form.vpnc_proto.value = "openvpn";
					}
					else if(j ==2){
						if(vpnc_clientlist_col[1] == "OpenVPN") {
							document.form.vpn_client_unit.value = vpnc_clientlist_col[2];
							document.form.openvpn_id.value = parseInt(vpnc_clientlist_col[2]) + 10;
						}
						else
							document.form.vpnc_heartbeat_x.value = vpnc_clientlist_col[2];
					}
					else if(j ==3){
						if(vpnc_clientlist_col[1] == "OpenVPN")
							document.form.vpn_client_username.value = vpnc_clientlist_col[3];
						else
							document.form.vpnc_pppoe_username.value = vpnc_clientlist_col[3];
					}
					else if(j ==4){
						if(vpnc_clientlist_col[1] == "OpenVPN")
							document.form.vpn_client_password.value = vpnc_clientlist_col[4];
						else
							document.form.vpnc_pppoe_passwd.value = vpnc_clientlist_col[4];
					}
				}

				// update vpnc_pptp_options_x
				document.form.vpnc_pptp_options_x.value = "";
				if(vpnc_clientlist_col[1] == "PPTP" && document.form.selPPTPOption.value != "auto") {
					document.form.vpnc_pptp_options_x.value = document.form.selPPTPOption.value;
				}
				document.form.listFlag.value = "1";
				if(vpnc_clientlist_col[1] == "OpenVPN")
					document.form.connFlag.value = "2";
				else
					document.form.connFlag.value = "1";
				document.form.vpn_clientx_eas.disabled = true;
				document.form.vpnc_clientlist.value = vpnc_clientlist_array;
				document.getElementById("vpnc_clientlist_table").rows[idx].cells[0].innerHTML = "-";
				document.getElementById("vpnc_clientlist_table").rows[idx].cells[5].innerHTML = "<img src='/images/InternetScan.gif'>";
				document.form.submit();
			}
			else{
				document.vpnclientForm.listFlag.value = "1";
				document.vpnclientForm.vpnc_clientlist.value = vpnc_clientlist_row.join("<");
				document.vpnclientForm.submit();
			}
			cancel_add_rule();
		}
	}
	else{	//Add Rule

		var rule_num = document.getElementById("vpnc_clientlist_table").rows.length;
		var item_num = document.getElementById("vpnc_clientlist_table").rows[0].cells.length;
		if(rule_num >= upper){
				alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
				return false;
		}

		if(flag == 'PPTP' || flag == 'L2TP'){
			type_obj = document.form.vpnc_type;
			description_obj = document.form.vpnc_des_edit;
			server_obj = document.form.vpnc_svr_edit;
			username_obj = document.form.vpnc_account_edit;
			password_obj = document.form.vpnc_pwd_edit;

		}else{	//OpenVPN: openvpn
			description_obj = document.vpnclientForm.vpnc_openvpn_des;
			type_obj = document.vpnclientForm.vpnc_type;
			server_obj = document.vpnclientForm.vpnc_openvpn_unit_edit;	// vpn_client_unit: 1 or 2
			username_obj = document.vpnclientForm.vpnc_openvpn_username;
			password_obj = document.vpnclientForm.vpnc_openvpn_pwd;
		}

		if(validForm(flag)){

			duplicateCheck.tmpStr = "";
			duplicateCheck.saveToTmpStr(type_obj, 0);
			duplicateCheck.saveToTmpStr(server_obj, 0);
			duplicateCheck.saveToTmpStr(username_obj, 0);
			duplicateCheck.saveToTmpStr(password_obj, 0);
			if(duplicateCheck.isDuplicate()){
				alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>")
				return false;
			}

			addRow(description_obj ,1);
			addRow(type_obj, 0);
			addRow(server_obj, 0);
			addRow(username_obj, 0);
			addRow(password_obj, 0);

			//add vpnc_pptp_options_x_list
			if(vpnc_pptp_options_x_list_array == "") {		//idx: NaN=Add i=edit row
				vpnc_pptp_options_x_list_array = reFillEmptyPPTPOPtion(idx, document.form.selPPTPOption.value, vpnc_clientlist_array);
			}
			else{
				vpnc_pptp_options_x_list_array = handlePPTPOPtion(idx, document.form.selPPTPOption.value, vpnc_clientlist_array);
			}

			show_vpnc_rulelist();
			cancel_add_rule();
			document.vpnclientForm.listFlag.value = "1";
			document.vpnclientForm.connFlag.value = "0";
			document.vpnclientForm.vpn_clientx_eas.disabled = true;
			document.vpnclientForm.vpnc_pptp_options_x_list.value = vpnc_pptp_options_x_list_array;
			document.vpnclientForm.vpnc_clientlist.value = vpnc_clientlist_array;
			document.vpnclientForm.submit();
		}
	}
}

function reFillEmptyPPTPOPtion(idx, objValue, vpncClientList) {
	var finalPPTPOptionsList = "";
	var currentVPNCList = vpncClientList;
	var currentVPNCListNum = currentVPNCList.split("<").length;
	if(idx >= 0){
		for(var i = 1; i < currentVPNCListNum; i += 1){
			if(i == idx + 1){
				finalPPTPOptionsList += "<" + objValue;
			}
			else{
				finalPPTPOptionsList += "<auto";
			}
		}
	}
	else{ //add
		for(var i = 1; i < currentVPNCListNum - 1; i += 1) {
			finalPPTPOptionsList += "<auto";
		}

		finalPPTPOptionsList += "<" + objValue;
	}

	return finalPPTPOptionsList;
}

function handlePPTPOPtion(idx, objValue, vpncClientList) {
	var finalPPTPOptionsList = "";
	var currentVPNCList = vpncClientList;
	var currentVPNCListNum = currentVPNCList.split("<").length;
	var origPPTPOptionsList = vpnc_pptp_options_x_list_array;
	var origPPTPOptionsListArray = origPPTPOptionsList.split("<");
	var origPPTPOptionsListArrayNum = origPPTPOptionsListArray.length;
	if(idx >= 0){ //edit
		for(var i = 1; i < currentVPNCListNum; i += 1) {
			if(i == idx + 1) {
				if(objValue == "") {
					finalPPTPOptionsList += "<auto";
				}
				else {
					finalPPTPOptionsList += "<" + objValue;
				}
			}
			else {
				if(origPPTPOptionsListArray[i] == "undefined") {
					finalPPTPOptionsList += "<auto";
				}
				else {
					finalPPTPOptionsList += "<" +  origPPTPOptionsListArray[i];
				}
			}
		}
	}
	else{ //add
		for(var i = 1; i < currentVPNCListNum - 1; i += 1){
			if(origPPTPOptionsListArray[i] == "undefined"){
				finalPPTPOptionsList += "<auto";
			}
			else{
				finalPPTPOptionsList += "<" + origPPTPOptionsListArray[i];
			}
		}

		finalPPTPOptionsList += "<" + objValue;
	}

	return finalPPTPOptionsList;
}
var duplicateCheck = {
	tmpIdx: "",

	saveTotmpIdx: function(obj){
		this.tmpIdx = obj;
	},

	tmpStr: "",

	saveToTmpStr: function(obj, head){
		if(head != 1)
			this.tmpStr += ">" /*&#62*/

		this.tmpStr += obj.value;
	},

	isDuplicate: function(){
		var vpnc_clientlist_row = vpnc_clientlist_array.split('<');
		var index_del = parseInt(this.tmpIdx);
		for(var i=1; i<vpnc_clientlist_row.length; i++){
			var compareData = vpnc_clientlist_row[i];
			var start_idx = compareData.indexOf(">");
			if(start_idx != -1)
				compareData = compareData.substring(start_idx);
			
			if(index_del == "" && index_del != 0){
				if(compareData == this.tmpStr){
					return true;
				}
			}
			else if(i != index_del + 1){
				if(compareData == this.tmpStr){
					return true;
				}
			}
		}
		return false;
	}
}

function addRow(obj, head){
	if(head == 1)
		vpnc_clientlist_array += "<" /*&#60*/
	else
		vpnc_clientlist_array += ">" /*&#62*/

	vpnc_clientlist_array += obj.value;
}

function validForm(mode){
	if(mode == "PPTP" || mode == "L2TP"){
		valid_des = document.form.vpnc_des_edit;
		valid_server = document.form.vpnc_svr_edit;
		valid_username = document.form.vpnc_account_edit;
		valid_password = document.form.vpnc_pwd_edit;

		if(valid_des.value==""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			valid_des.focus();
			return false;
		}else if(!Block_chars(valid_des, ["*", "+", "|", ":", "?", "<", ">", ",", ".", "/", ";", "[", "]", "\\", "=", "\"" ])){
			return false;
		}

		if(valid_server.value==""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			valid_server.focus();
			return false;
		}
		else{
			var isIPAddr = valid_server.value.replace(/\./g,"");
			var re = /^[0-9]+$/;
			if(!re.test(isIPAddr)) { //DDNS
				if(!Block_chars(valid_server, ["<", ">"])) {
					return false;
				}
			}
			else { // IP
				if(!validator.isLegalIP(valid_server,"")) {
					return false;
				}
			}
		}

		if(valid_username.value==""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			valid_username.focus();
			return false;
		}else if(!Block_chars(valid_username, ["<", ">"])){
			return false;
		}

		if(valid_password.value==""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			valid_password.focus();
			return false;
		}else if(!Block_chars(valid_password, ["<", ">"])){
			return false;
		}
	}
	else{		//OpenVPN
		valid_des = document.vpnclientForm.vpnc_openvpn_des;
		valid_username = document.vpnclientForm.vpnc_openvpn_username;
		valid_password = document.vpnclientForm.vpnc_openvpn_pwd;

		if(valid_des.value == ""){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			valid_des.focus();
			return false;
		}
		else if(!Block_chars(valid_des, ["*", "+", "|", ":", "?", "<", ">", ",", ".", "/", ";", "[", "]", "\\", "=", "\"" ])){
			return false;
		}

		if(valid_username.value != "" && !Block_chars(valid_username, ["<", ">"])){
			return false;
		}

		if(valid_password.value != "" && !Block_chars(valid_password, ["<", ">"])){
			return false;
		}
	}

	return true;
}


var save_flag;	//type of Saving profile
function tabclickhandler(_type){
	document.getElementById('pptpcTitle').className = "vpnClientTitle_td_unclick";
	document.getElementById('l2tpcTitle').className = "vpnClientTitle_td_unclick";
	document.getElementById('opencTitle').className = "vpnClientTitle_td_unclick";
	document.getElementById("pptpcTitle_openvpn").className = "vpnClientTitle_td_unclick";
	document.getElementById("l2tpcTitle_openvpn").className = "vpnClientTitle_td_unclick";
	document.getElementById("opencTitle_openvpn").className = "vpnClientTitle_td_unclick";
	if(_type == 0){
		save_flag = "PPTP";
		document.form.vpnc_type.value = "PPTP";
		document.vpnclientForm.vpnc_type.value = "PPTP";
		document.getElementById('pptpcTitle').className = "vpnClientTitle_td_click";
		document.getElementById('vpnc_setting').style.display = "block";
		document.getElementById('vpnc_setting_openvpn').style.display = "none";
		document.getElementById('trPPTPOptions').style.display = "";
	}
	else if(_type == 1){
		save_flag = "L2TP";
		document.form.vpnc_type.value = "L2TP";
		document.vpnclientForm.vpnc_type.value = "L2TP";
		document.getElementById('l2tpcTitle').className = "vpnClientTitle_td_click";
		document.getElementById('vpnc_setting').style.display = "block";
		document.getElementById('vpnc_setting_openvpn').style.display = "none";
		document.getElementById('trPPTPOptions').style.display = "none";
	}
	else if(_type == 2){
		save_flag = "OpenVPN";
		update_unit_option();
		document.form.vpnc_type.value = "OpenVPN";
		document.vpnclientForm.vpnc_type.value = "OpenVPN";
		document.getElementById("opencTitle_openvpn").className = "vpnClientTitle_td_click";
		document.getElementById('vpnc_setting').style.display = "none";
		document.getElementById('vpnc_setting_openvpn').style.display = "block";
	}
}

function update_unit_option(){
	if(vpnc_clientlist_array.split("OpenVPN>1").length!=2)
		document.vpnclientForm.vpnc_openvpn_unit_edit.value = "1";
	if(vpnc_clientlist_array.split("OpenVPN>2").length!=2)
		document.vpnclientForm.vpnc_openvpn_unit_edit.value = "2";
	if(vpnc_clientlist_array.split("OpenVPN>3").length!=2)
		document.vpnclientForm.vpnc_openvpn_unit_edit.value = "3";
	if(vpnc_clientlist_array.split("OpenVPN>4").length!=2)
		document.vpnclientForm.vpnc_openvpn_unit_edit.value = "4";
	if(vpnc_clientlist_array.split("OpenVPN>5").length!=2)
		document.vpnclientForm.vpnc_openvpn_unit_edit.value = "5";
}

function show_vpnc_rulelist(){

	var vpnc_clientlist_row = vpnc_clientlist_array.split('<');
	var code = "";
	code +='<table style="margin-bottom:30px;" width="98%" border="1" align="center" cellpadding="4" cellspacing="0" class="list_table" id="vpnc_clientlist_table">';
	if(vpnc_clientlist_array == "")
		code +='<tr><td style="color:#FFCC00;" colspan="6"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></td></tr>';
	else{

		for(var i=1; i<vpnc_clientlist_row.length; i++){
			overlib_str0[i] = "";
			overlib_str1[i] = "";
			code +='<tr id="row'+i+'">';

			var vpnc_clientlist_col = vpnc_clientlist_row[i].split('>');
			var client_state = "0";

			if(vpnc_clientlist_col[1] == "OpenVPN"){
				var connect_idx = document.form.vpn_clientx_eas.value.replace(",", "");
				var client_errno = "";
				if(connect_idx == vpnc_clientlist_col[2]) {
					switch(vpnc_clientlist_col[2]) {
						case "1" :
							client_state = vpnc_state_t1;
							client_errno = vpnc_errno_t1;
							break;
						case "2" :
							client_state = vpnc_state_t2;
							client_errno = vpnc_errno_t2;
							break;
						case "3" :
							client_state = vpnc_state_t3;
							client_errno = vpnc_errno_t3;
							break;
						case "4" :
							client_state = vpnc_state_t4;
							client_errno = vpnc_errno_t4;
							break;
						case "5" :
							client_state = vpnc_state_t5;
							client_errno = vpnc_errno_t5;
							break;
					}
				}
				// state: default 0, connecting 1, connected 2, error -1
				// when state is -1, check errno, default 0, conflict 1 2 3, auth fail 4 5 6
				if(client_state == 0)
					code +='<td width="10%">-</td>';
				else if(client_state == 1)
					code +="<td width='10%'><img title='<%tcWebApi_Get("String_Entry", "CTL_Add_enrollee", "s")%>' src='/images/InternetScan.gif'></td>";
				else if(client_state == 2)
					code +="<td width='10%'><img title='<%tcWebApi_Get("String_Entry", "Connected", "s")%>' src='/images/checked_parentctrl.png' style='width:25px;'></td>";
				else if(client_errno == 1 || client_errno == 2 || client_errno == 3)
					code +="<td width='10%'><img title='<%tcWebApi_Get("String_Entry", "vpn_openvpn_conflict", "s")%>' src='/images/New_ui/notification.png' style='background-image:url(/images/New_ui/notification.png);background-repeat:no-repeat;height:25px;width:25px;'></td>";
				else if(client_errno == 4 || client_errno == 5 || client_errno == 6)
					code +="<td width='10%'><img title='<%tcWebApi_Get("String_Entry", "qis_fail_desc1", "s")%>' src='/images/button-close2.png' style='width:25px;'></td>";
				else		//Stop connection
					code +="<td width='10%'><img title='<%tcWebApi_Get("String_Entry", "ConnectionFailed", "s")%>' src='/images/button-close2.png' style='width:25px;'></td>";
			}
			else{		//PPTP or L2TP
				if( vpnc_clientlist_col[1] == document.form.vpnc_proto.value.toUpperCase()
				 && vpnc_clientlist_col[2] == document.form.vpnc_heartbeat_x.value
				 && vpnc_clientlist_col[3] == document.form.vpnc_pppoe_username.value)		//matched connecting rule
				{
					if(vpnc_state_t == 0 || vpnc_state_t ==1) // Initial or Connecting
						code +='<td width="10%"><img title="<%tcWebApi_Get("String_Entry", "CTL_Add_enrollee", "s")%>" src="/images/InternetScan.gif"></td>';
					else if(vpnc_state_t == 2) // Connected
						code +='<td width="10%"><img title="<%tcWebApi_Get("String_Entry", "Connected", "s")%>" src="/images/checked_parentctrl.png" style="width:25px;"></td>';
					else if(vpnc_state_t == 4 && vpnc_sbstate_t == 2)
						code +="<td width=\"10%\"><img title=\"<%tcWebApi_Get("String_Entry","qis_fail_desc1","s")%>\" src=\"/images/button-close2.png\" style=\"width:25px;\"></td>";
					else // Stop connection
						code +='<td width="10%"><img title="<%tcWebApi_Get("String_Entry", "ConnectionFailed", "s")%>" src="/images/button-close2.png" style="width:25px;"></td>';
				}
				else{
					code +='<td width="10%">-</td>';
				}
			}

			for(var j=0; j<vpnc_clientlist_col.length; j++){
				if(j == 0){
					if(vpnc_clientlist_col[0].length >28){
						overlib_str0[i] += vpnc_clientlist_col[0];
						vpnc_clientlist_col[0]=vpnc_clientlist_col[0].substring(0, 25)+"...";
						code +='<td title="'+overlib_str0[i]+'">'+ vpnc_clientlist_col[0] +'</td>';
					}else{
						code +='<td>'+ vpnc_clientlist_col[0] +'</td>';
					}
				}
				else if(j == 1){
					code += '<td width="15%">'+ vpnc_clientlist_col[1] +'</td>';
				}
			}

			// EDIT
		 	code += '<td width="10%"><input class="edit_btn" type="button" onclick="Edit_Row(this, \'vpnc\');" value=""/></td>';

			if(vpnc_clientlist_col[1] == "OpenVPN"){
				if(client_state != 0) {	//connecting
					code += '<td width="10%"><input class="remove_btn" type="button" onclick="del_Row(this, \'vpnc_enable\');" value=""/></td>';
					code += '<td width="25%"><input class="button_gen" type="button" onClick="connect_Row(this, \'disconnect\');" id="disonnect_btn" value="Deactivate" style="padding:0 0.3em 0 0.3em;" >';
				}
				else{			//OpenVPN is not connecting
					code += '<td width="10%"><input class="remove_btn" type="button" onclick="del_Row(this, \'vpnc\');" value=""/></td>';
					code += '<td width="25%"><input class="button_gen" type="button" onClick="connect_Row(this, \'vpnc\');" id="Connect_btn" name="Connect_btn" value="Activate" style="padding:0 0.3em 0 0.3em;" >';
				}
			}
			else{	//PPTP or L2TP

				if( vpnc_clientlist_col[1] == document.form.vpnc_proto.value.toUpperCase()
				 && vpnc_clientlist_col[2] == document.form.vpnc_heartbeat_x.value
				 && vpnc_clientlist_col[3] == document.form.vpnc_pppoe_username.value)
				{		// This rule is connecting
					code += '<td width="10%"><input class="remove_btn" type="button" onclick="del_Row(this, \'vpnc_enable\');" value=""/></td>';
					code += '<td width="25%"><input class="button_gen" type="button" onClick="connect_Row(this, \'disconnect\');" id="disonnect_btn" value="Deactivate" style="padding:0 0.3em 0 0.3em;" >';
				}
				else{		// This rule is not connecting
					code += '<td width="10%"><input class="remove_btn" type="button" onclick="del_Row(this, \'vpnc\');" value=""/></td>';
					code += '<td width="25%"><input class="button_gen" type="button" onClick="connect_Row(this, \'vpnc\');" id="Connect_btn" name="Connect_btn" value="Activate" style="padding:0 0.3em 0 0.3em;" >';
				}
			}
		}
	}
	code +='</table>';
	document.getElementById("vpnc_clientlist_Block").innerHTML = code;
}

function connect_Row(rowdata, flag){
	var idx = rowdata.parentNode.parentNode.rowIndex;
	var vpnc_clientlist_row = vpnc_clientlist_array.split('<');
	var vpnc_clientlist_col = vpnc_clientlist_row[idx+1].split('>');

	if(flag == "disconnect"){
		if(vpnc_clientlist_col[1] == "OpenVPN"){
			document.form.vpnc_proto.value = "disable";
			document.form.vpn_client_unit.value = vpnc_clientlist_col[2];
			document.form.vpn_clientx_eas.disabled = false;
			document.form.vpn_clientx_eas.value = document.form.vpn_clientx_eas.value.replace(vpnc_clientlist_col[2] + ",", "");
			document.vpnclientForm.vpn_clientx_eas.value = document.form.vpn_clientx_eas.value.replace(vpnc_clientlist_col[2] + ",", "");
			document.form.connFlag.value = "2";
		}else{ //pptp/l2tp
			document.form.vpnc_proto.value = "disable";
			document.form.vpnc_heartbeat_x.value = "";
			document.form.vpnc_pppoe_username.value = "";
			document.form.vpnc_pppoe_passwd.value = "";
			document.form.connFlag.value = "1";
			if(vpnc_clientlist_col[1] == "PPTP") {
				document.form.vpnc_pptp_options_x.value = "";
			}
		}
	}
	else{		//"vpnc" making connection

		for(var j=0; j<vpnc_clientlist_col.length; j++){
			if(j == 0){
				document.form.vpnc_des_edit.value = vpnc_clientlist_col[0];
			}
			else if(j ==1){
				if(vpnc_clientlist_col[1] == "PPTP"){
					document.form.vpnc_proto.value = "pptp";
					document.form.connFlag.value = "1";
				}else if(vpnc_clientlist_col[1] == "L2TP"){
					document.form.vpnc_proto.value = "l2tp";
					document.form.connFlag.value = "1";
				}else{	//OpenVPN
					document.form.vpnc_proto.value = "openvpn";
					document.form.connFlag.value = "2";
				}
			}
			else if(j ==2){
				if(vpnc_clientlist_col[1] == "OpenVPN") {
					document.form.vpn_client_unit.value = vpnc_clientlist_col[2];
					document.form.openvpn_id.value = parseInt(vpnc_clientlist_col[2]) + 10;
				}
				else
					document.form.vpnc_heartbeat_x.value = vpnc_clientlist_col[2];
			}
			else if(j ==3){
				if(vpnc_clientlist_col[1] == "OpenVPN")
					document.form.vpn_client_username.value = vpnc_clientlist_col[3];
				else
					document.form.vpnc_pppoe_username.value = vpnc_clientlist_col[3];
			}
			else if(j ==4){
				if(vpnc_clientlist_col[1] == "OpenVPN")
					document.form.vpn_client_password.value = vpnc_clientlist_col[4];
				else
					document.form.vpnc_pppoe_passwd.value = vpnc_clientlist_col[4];
			}

			if(vpnc_clientlist_col[1] == "OpenVPN"){
				document.form.vpn_clientx_eas.disabled = false;
				if(document.form.vpn_clientx_eas.value.search(vpnc_clientlist_col[2]) < 0 )
					document.form.vpn_clientx_eas.value = vpnc_clientlist_col[2] + ",";
				if(document.vpnclientForm.vpn_clientx_eas.value.search(vpnc_clientlist_col[2]) < 0 )
					document.vpnclientForm.vpn_clientx_eas.value = vpnc_clientlist_col[2] + ",";
			}
			else{
				document.form.vpnc_auto_conn.value = 1;
			}
		}

		//handle vpnc_pptp_options_x
		if(vpnc_clientlist_col[1] == "PPTP"){
			var origPPTPOptionsList = vpnc_pptp_options_x_list_array;
			if(origPPTPOptionsList != ""){
				var origPPTPOptionsListArray = origPPTPOptionsList.split("<");
				var setPPTPOption = origPPTPOptionsListArray[idx + 1];
				document.form.vpnc_pptp_options_x.value = "";
				if(setPPTPOption != "auto" && setPPTPOption != "undefined") {
					document.form.vpnc_pptp_options_x.value = setPPTPOption;
				}
			}
			else{
				document.form.vpnc_pptp_options_x.value = "";
			}
		}
	}
	document.form.vpnc_pptp_options_x_list.value = vpnc_pptp_options_x_list_array;
	document.form.vpnc_clientlist.value = vpnc_clientlist_array;
	rowdata.parentNode.innerHTML = "<img src='/images/InternetScan.gif'>";
	document.form.submit();
}

function setManualTable (unit) {
	switch (unit) {
		case "1" :
			document.getElementById('edit_vpn_crt_client_ca').value = vpn_crt_client1_ca[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crt').value = vpn_crt_client1_crt[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_key').value = vpn_crt_client1_key[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_static').value = vpn_crt_client1_static[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crl').value = vpn_crt_client1_crl[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			break;
		case "2" :
			document.getElementById('edit_vpn_crt_client_ca').value = vpn_crt_client2_ca[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crt').value = vpn_crt_client2_crt[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_key').value = vpn_crt_client2_key[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_static').value = vpn_crt_client2_static[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crl').value = vpn_crt_client2_crl[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			break;
		case "3" :
			document.getElementById('edit_vpn_crt_client_ca').value = vpn_crt_client3_ca[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crt').value = vpn_crt_client3_crt[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_key').value = vpn_crt_client3_key[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_static').value = vpn_crt_client3_static[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crl').value = vpn_crt_client3_crl[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			break;
		case "4" :
			document.getElementById('edit_vpn_crt_client_ca').value = vpn_crt_client4_ca[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crt').value = vpn_crt_client4_crt[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_key').value = vpn_crt_client4_key[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_static').value = vpn_crt_client4_static[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crl').value = vpn_crt_client4_crl[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			break;
		case "5" :
			document.getElementById('edit_vpn_crt_client_ca').value = vpn_crt_client5_ca[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crt').value = vpn_crt_client5_crt[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_key').value = vpn_crt_client5_key[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_static').value = vpn_crt_client5_static[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			document.getElementById('edit_vpn_crt_client_crl').value = vpn_crt_client5_crl[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			break;
	}
}
function ovpnFileUpdate(unit){
	$j.ajax({
		url: '/ajax_openvpn_client.asp',
		dataType: 'script',
		timeout: 1500,
		error: function(xhr){
			setTimeout("ovpnFileUpdate(" + unit + ");",1000);
		},
		success: function(){
			setManualTable(unit);
		}
	});
}
var idx_tmp = "";
function Edit_Row(rowdata, flag){
	document.getElementById("cancelBtn").style.display = "";
	document.getElementById("cancelBtn_openvpn").style.display = "";
	idx_tmp = rowdata.parentNode.parentNode.rowIndex; //update idx
	var idx = rowdata.parentNode.parentNode.rowIndex;
	if(document.getElementById("vpnc_clientlist_table").rows[idx].cells[0].innerHTML != "-")
		restart_vpncall_flag = 1;
	var vpnc_clientlist_row = vpnc_clientlist_array.split('<');
	var vpnc_clientlist_col = vpnc_clientlist_row[idx+1].split('>');
	//get idx of PPTP option value
	var pptpOptionValue = "";
	var origPPTPOptionsList = vpnc_pptp_options_x_list_array;
	if(idx >= 0){
		var origPPTPOptionsListArray = origPPTPOptionsList.split("<");
		if(origPPTPOptionsListArray.length == 1) { //avoid vpnc_pptp_options_x_list is null
			pptpOptionValue = "auto";
		}
		else{
			if(origPPTPOptionsListArray[idx + 1] == "undefined"){
				pptpOptionValue = "auto";
			}
			else{
				pptpOptionValue = origPPTPOptionsListArray[idx + 1];
			}

		}
	}
	else{	//default is auto
		pptpOptionValue = "auto";
	}

	document.form.selPPTPOption.value = pptpOptionValue;
	pptpOptionChange();
	if(vpnc_clientlist_col[1] == "OpenVPN"){
		document.vpnclientForm.file.value = "";
		document.openvpnCAForm.file.value = "";
		document.vpnclientForm.vpnc_openvpn_des.value = vpnc_clientlist_col[0];
		document.vpnclientForm.vpnc_openvpn_username.value = vpnc_clientlist_col[3];
		document.vpnclientForm.vpnc_openvpn_pwd.value = vpnc_clientlist_col[4];

		ovpnFileUpdate(vpnc_clientlist_col[2]);

		tabclickhandler(2);
		//fixed unit
		document.vpnclientForm.vpnc_openvpn_unit_edit.value = vpnc_clientlist_col[2];

		document.getElementById("caFiled").style.display = "";
		document.getElementById("manualFiled").style.display = "";
		document.getElementById('importOvpnFile').style.display = "none";
		document.getElementById('loadingicon').style.display = "none";
		document.getElementById("cbManualImport").checked = false;
		manualImport(false);
	}
	else{		//PPTP or L2TP
		for(var j=0; j<vpnc_clientlist_col.length; j++){
			if(j == 0){
				document.form.vpnc_des_edit.value = vpnc_clientlist_col[0];
			}
			else if(j == 1){
				if(vpnc_clientlist_col[1] == "PPTP")
					tabclickhandler(0);
				else if(vpnc_clientlist_col[1] == "L2TP")
					tabclickhandler(1);
				else
					tabclickhandler(2);
			}
			else if(j == 2){
				document.form.vpnc_svr_edit.value = vpnc_clientlist_col[2];
			}
			else if(j == 3){
				document.form.vpnc_account_edit.value = vpnc_clientlist_col[3];
			}
			else if(j == 4){
				document.form.vpnc_pwd_edit.value = vpnc_clientlist_col[4];
			}
		}
	}

	if(vpnc_clientlist_col[1] == "PPTP"){
		$j("#vpnc_setting").fadeIn(300);
		document.getElementById("pptpcTitle").style.display = "";
		document.getElementById("l2tpcTitle").style.display = "none";
		document.getElementById("opencTitle").style.display = "none";
		document.getElementById("trPPTPOptions").style.display = "";
	}else if(vpnc_clientlist_col[1] == "L2TP"){
		$j("#vpnc_setting").fadeIn(300);
		document.getElementById("pptpcTitle").style.display = "none";
		document.getElementById("l2tpcTitle").style.display = "";
		document.getElementById("opencTitle").style.display = "none";
		document.getElementById("trPPTPOptions").style.display = "none";
	}else if(vpnc_clientlist_col[1] == "OpenVPN"){
		$j("#vpnc_setting_openvpn").fadeIn(300);
		document.getElementById("pptpcTitle_openvpn").style.display = "none";
		document.getElementById("l2tpcTitle_openvpn").style.display = "none";
		document.getElementById("opencTitle_openvpn").style.display = "";
	}
}

function del_Row(rowdata, flag){
	var idx = rowdata.parentNode.parentNode.rowIndex;
	document.getElementById("vpnc_clientlist_table").deleteRow(idx);

	var vpnc_clientlist_value = "";
	var vpnc_clientlist_row = vpnc_clientlist_array.split('<');
	var vpnc_clientlist_col_delete = vpnc_clientlist_row[idx+1].split('>');
	for(k=1; k<vpnc_clientlist_row.length; k++){
		if(k != idx+1){
			vpnc_clientlist_value += "<"+vpnc_clientlist_row[k];
		}
	}
	vpnc_clientlist_array = vpnc_clientlist_value;
	if(vpnc_clientlist_array == "")
		show_vpnc_rulelist();

	//del vpnc_pptp_options_x_list
	var tempPPTPOptionsValue = "";
	var origPPTPOptionsList = vpnc_pptp_options_x_list_array;
	var origPPTPOptionsListArray = origPPTPOptionsList.split("<");
	var origPPTPOptionsListArrayNum = origPPTPOptionsListArray.length;
	for(var index = 1; index < origPPTPOptionsListArrayNum; index += 1) {
		if(index != (idx + 1)) {	//save exist items
			tempPPTPOptionsValue += "<"+ origPPTPOptionsListArray[index];
		}
	}

	vpnc_pptp_options_x_list_array = tempPPTPOptionsValue;
	document.vpnclientForm.vpnc_pptp_options_x_list.value = vpnc_pptp_options_x_list_array;

	if(flag == "vpnc_enable"){	//remove connected rule.
		document.vpnclientForm.vpnc_proto.value = "disable";
		document.vpnclientForm.vpnc_proto.disabled = false;
		document.vpnclientForm.action = "start_apply.asp";
		document.vpnclientForm.enctype = "application/x-www-form-urlencoded";
		document.vpnclientForm.encoding = "application/x-www-form-urlencoded";
		document.vpnclientForm.action_script.value = "restart_vpncall";
		if(vpnc_clientlist_col_delete[1] == "OpenVPN"){
			document.vpnclientForm.vpn_clientx_eas.disabled = false;
			if(vpnc_clientlist_row[idx+1].split("OpenVPN>1").length == 2) {
					document.vpnclientForm.vpn_client_unit.value = 1;
					document.vpnclientForm.vpn_clientx_eas.value = document.vpnclientForm.vpn_clientx_eas.value.replace("1,", "");
					document.form.vpn_clientx_eas.value = document.form.vpn_clientx_eas.value.replace("1,", "");
			}
			else if(vpnc_clientlist_row[idx+1].split("OpenVPN>2").length == 2) {
					document.vpnclientForm.vpn_client_unit.value = 2;
					document.vpnclientForm.vpn_clientx_eas.value = document.vpnclientForm.vpn_clientx_eas.value.replace("2,", "");
					document.form.vpn_clientx_eas.value = document.form.vpn_clientx_eas.value.replace("2,", "");
			}
			else if(vpnc_clientlist_row[idx+1].split("OpenVPN>3").length == 2) {
					document.vpnclientForm.vpn_client_unit.value = 3;
					document.vpnclientForm.vpn_clientx_eas.value = document.vpnclientForm.vpn_clientx_eas.value.replace("3,", "");
					document.form.vpn_clientx_eas.value = document.form.vpn_clientx_eas.value.replace("3,", "");
			}
			else if(vpnc_clientlist_row[idx+1].split("OpenVPN>4").length == 2) {
					document.vpnclientForm.vpn_client_unit.value = 4;
					document.vpnclientForm.vpn_clientx_eas.value = document.vpnclientForm.vpn_clientx_eas.value.replace("4,", "");
					document.form.vpn_clientx_eas.value = document.form.vpn_clientx_eas.value.replace("4,", "");
			}
			else if(vpnc_clientlist_row[idx+1].split("OpenVPN>5").length == 2) {
					document.vpnclientForm.vpn_client_unit.value = 5;
					document.vpnclientForm.vpn_clientx_eas.value = document.vpnclientForm.vpn_clientx_eas.value.replace("5,", "");
					document.form.vpn_clientx_eas.value = document.form.vpn_clientx_eas.value.replace("5,", "");
			}

			document.vpnclientForm.connFlag.value = "2";
		}
		else {
			document.vpnclientForm.connFlag.value = "1";
		}
	}

	document.vpnclientForm.listFlag.value = "1";
	document.vpnclientForm.vpnc_clientlist.value = vpnc_clientlist_array;
	document.vpnclientForm.submit();
}

function ImportOvpn(unit){
	document.getElementById('importOvpnFile').style.display = "none";
	document.getElementById('loadingicon').style.display = "";

	document.vpnclientForm.action = "vpnupload.asp";
	document.vpnclientForm.enctype = "multipart/form-data";
	document.vpnclientForm.encoding = "multipart/form-data";
	document.vpnclientForm.vpn_upload_unit.value = unit;
	document.vpnclientForm.submit();
	setTimeout("ovpnFileChecker();",2000);
}

function manualImport(_flag){
	if(_flag){
		document.getElementById("caFiled").style.display = "";
		document.getElementById("manualFiled").style.display = "";
	}
	else{
		document.getElementById("caFiled").style.display = "none";
		document.getElementById("manualFiled").style.display = "none";
	}
}

var vpn_upload_state = "init";
function ovpnFileChecker(){
	document.getElementById("importOvpnFile").innerHTML = "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>";
	document.getElementById("manualCRList").style.color = "#FFF";
	document.getElementById("manualStatic").style.color = "#FFF";
	document.getElementById("manualKey").style.color = "#FFF";
	document.getElementById("manualCert").style.color = "#FFF";
	document.getElementById("manualCa").style.color = "#FFF";

	$j.ajax({
			url: '/ajax_openvpn_client.asp',
			dataType: 'script',
			timeout: 1500,
			error: function(xhr){
				setTimeout("ovpnFileChecker();",1000);
			},

			success: function(){
				document.getElementById('importOvpnFile').style.display = "";
				document.getElementById('loadingicon').style.display = "none";
				document.getElementById('importCA').style.display = "";
				document.getElementById('loadingiconCA').style.display = "none";

				if(vpn_upload_state == "init"){
					setTimeout("ovpnFileChecker();",1000);
				}
				else{
					setManualTable(document.vpnclientForm.vpnc_openvpn_unit_edit.value);

					var vpn_upload_state_tmp = "";
					vpn_upload_state_tmp = parseInt(vpn_upload_state) - 16;
					if(vpn_upload_state_tmp > -1){
						document.getElementById("importOvpnFile").innerHTML += ", Lack of Certificate Revocation List(Optional)";
						document.getElementById("manualCRList").style.color = "#FC0";
						vpn_upload_state = vpn_upload_state_tmp;
					}
					vpn_upload_state_tmp = parseInt(vpn_upload_state) - 8;
					if(vpn_upload_state_tmp > -1){
						document.getElementById("importOvpnFile").innerHTML += ", Lack of Static Key(Optional)";
						document.getElementById("manualStatic").style.color = "#FC0";
						vpn_upload_state = vpn_upload_state_tmp;
					}

					vpn_upload_state_tmp = parseInt(vpn_upload_state) - 4;
					if(vpn_upload_state_tmp > -1){
						document.getElementById("importOvpnFile").innerHTML += ", Lack of Client Key!";
						document.getElementById("manualKey").style.color = "#FC0";
						vpn_upload_state = vpn_upload_state_tmp;
					}

					vpn_upload_state_tmp = parseInt(vpn_upload_state) - 2;
					if(vpn_upload_state_tmp > -1){
						document.getElementById("importOvpnFile").innerHTML += ", Lack of Client Certificate";
						document.getElementById("manualCert").style.color = "#FC0";
						vpn_upload_state = vpn_upload_state_tmp;
					}

					vpn_upload_state_tmp = parseInt(vpn_upload_state) - 1;
					if(vpn_upload_state_tmp > -1){
						document.getElementById("importOvpnFile").innerHTML += ", Lack of Certificate Authority";
						document.getElementById("manualCa").style.color = "#FC0";
					}
				}
			}
	});
}

var vpn_upload_state = "init";

function cancel_Key_panel(){
	document.getElementById("manualFiled_panel").style.display = "none";
}

function saveManual(unit){
	document.openvpnManualForm.vpn_client_unit.value = unit;
	document.openvpnManualForm.vpn_crt_client_ca.value = document.getElementById('edit_vpn_crt_client_ca').value;
	document.openvpnManualForm.vpn_crt_client_crt.value = document.getElementById('edit_vpn_crt_client_crt').value;
	document.openvpnManualForm.vpn_crt_client_key.value = document.getElementById('edit_vpn_crt_client_key').value;
	document.openvpnManualForm.vpn_crt_client_static.value = document.getElementById('edit_vpn_crt_client_static').value;
	document.openvpnManualForm.vpn_crt_client_crl.value = document.getElementById('edit_vpn_crt_client_crl').value;
	document.openvpnManualForm.vpn_crt_client_ca.disabled = false;
	document.openvpnManualForm.vpn_crt_client_crt.disabled = false;
	document.openvpnManualForm.vpn_crt_client_key.disabled = false;
	document.openvpnManualForm.vpn_crt_client_static.disabled = false;
	document.openvpnManualForm.vpn_crt_client_crl.disabled = false;
	document.openvpnManualForm.submit();
	cancel_Key_panel();
}

function startImportCA(unit){
	document.getElementById('importCA').style.display = "";
	document.getElementById('loadingiconCA').style.display = "";
	document.openvpnCAForm.vpn_upload_unit.value = unit;
	setTimeout('ovpnFileChecker();',2000);
	document.openvpnCAForm.submit();
}

function addOpenvpnProfile(){
	document.vpnclientForm.action = "start_apply.asp";
	document.vpnclientForm.enctype = "application/x-www-form-urlencoded";
	document.vpnclientForm.encoding = "application/x-www-form-urlencoded";
	addRow_Group(10,save_flag,idx_tmp);
}

function pptpOptionChange() {
	document.getElementById("pptpOptionHint").style.display = "none";
	if(document.form.selPPTPOption.value == "+mppe-40") {
		document.getElementById("pptpOptionHint").style.display = "";
	}
}
</script>
</head>

<body onload="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_VPNClient_Content.asp">
<input type="hidden" name="next_page" value="Advanced_VPNClient_Content.asp">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="flag" value="background">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_vpncall">
<input type="hidden" name="action_wait" value="3">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="">
<input type="hidden" name="listFlag" value="0">
<input type="hidden" name="connFlag" value="0">
<input type="hidden" name="vpnc_pppoe_username" value="<% tcWebApi_get("VPNC_Entry","username","s") %>">
<input type="hidden" name="vpnc_pppoe_passwd" value="<% tcWebApi_get("VPNC_Entry","passwd","s") %>">
<input type="hidden" name="vpnc_heartbeat_x" value="<% tcWebApi_get("VPNC_Entry","heartbeat","s") %>">
<input type="hidden" name="vpnc_dnsenable_x" value="1">
<input type="hidden" name="vpnc_proto" value="<% tcWebApi_get("VPNC_Entry","proto","s") %>">
<input type="hidden" name="vpnc_clientlist" value=''>
<input type="hidden" name="vpnc_type" value="PPTP">
<input type="hidden" name="vpnc_auto_conn" value="">
<input type="hidden" name="vpn_client_unit" value="1">
<input type="hidden" name="openvpn_id" value="0">
<input type="hidden" name="vpn_client_username" value="">
<input type="hidden" name="vpn_client_password" value="">
<input type="hidden" name="vpn_clientx_eas" value="<% tcWebApi_Get("OpenVPN_Common", "vpn_clientx_eas", "s"); %>">
<input type="hidden" name="vpnc_pptp_options_x" value="<% tcWebApi_Get("VPNC_Entry", "pptp_options", "s") %>">
<input type="hidden" name="vpnc_pptp_options_x_list" value="<% tcWebApi_Get("VPNC_Entry", "pptp_options_list", "s") %>">
<div id="vpnc_setting"  class="contentM_qis" style="box-shadow: 1px 5px 10px #000;">
	<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
		<tr style="height:32px;">
			<td>
				<table width="100%" border="0" align="left" cellpadding="0" cellspacing="0" class="vpnClientTitle">
					<tr>
			  		<td width="33%" align="center" id="pptpcTitle" onclick="tabclickhandler(0);">PPTP</td>
			  		<td width="33%" align="center" id="l2tpcTitle" onclick="tabclickhandler(1);">L2TP</td>
			  		<td width="33%" align="center" id="opencTitle" onclick="tabclickhandler(2);">OpenVPN</td>
					</tr>
				</table>
			</td>
		</tr>

		<tr>
			<td>
				<!---- vpnc_pptp/l2tp start  ---->
				<div>
				<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
			 		<tr>
						<th><%tcWebApi_get("String_Entry","IPC_autofwDesc_in","s")%></th>
						<td>
						  	<input type="text" maxlength="64" name="vpnc_des_edit" value="" class="input_32_table" style="float:left;" autocorrect="off" autocapitalize="off"></input>
						</td>
					</tr>
					<tr>
						<th><%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%></th>
						<td>
							<input type="text" maxlength="64" name="vpnc_svr_edit" value="" class="input_32_table" style="float:left;" autocorrect="off" autocapitalize="off"></input>
						</td>
					</tr>
					<tr>
						<th><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%></th>
						<td>
							<input type="text" maxlength="64" name="vpnc_account_edit" value="" class="input_32_table" style="float:left;" autocomplete="off" autocorrect="off" autocapitalize="off"></input>
						</td>
					</tr>
					<tr>
						<th><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></th>
						<td>
							<input type="text" maxlength="64" name="vpnc_pwd_edit" value="" class="input_32_table" style="float:left;" autocomplete="off" autocorrect="off" autocapitalize="off"></input>
						</td>
					</tr>
					<tr id="trPPTPOptions">
						<th><%tcWebApi_Get("String_Entry","PPPC_x_PPTPOptions_in","s")%></th>
						<td>
							<select name="selPPTPOption" class="input_option" onchange="pptpOptionChange();">
								<option value="auto"><%tcWebApi_Get("String_Entry","Auto","s")%></option>
								<option value="-mppc"><%tcWebApi_Get("String_Entry","No_Encryp","s")%></option>
								<option value="+mppe-40">MPPE 40</option>
								<option value="+mppe-128">MPPE 128</option>
							</select>
							<div id="pptpOptionHint" style="display:none;">
								<span>If the VPN Server supports the higher MPPE encryption level but not selected, the higher encryption level will be used for better security.
								<!--untranslated, PPTPOptions_OpenVPN_hint ?? not OpenVPN--></span>
							</div>
						</td>
					</tr>
				</table>
				</div>
				<!---- vpnc_pptp/l2tp end  ---->
			</td>
		</tr>
	</table>

	<div style="margin-top:5px;padding-bottom:10px;width:100%;text-align:center;">
		<input class="button_gen" type="button" onclick="cancel_add_rule();" id="cancelBtn" value="<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>">
		<input class="button_gen" type="button" onclick="addRow_Group(10,save_flag, idx_tmp);" value="<%tcWebApi_get("String_Entry","CTL_ok","s")%>">
	</div>
      <!--===================================Ending of openvpnc setting Content===========================================-->
</div>

<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
	<td width="17">&nbsp;</td>

	<!--=====Beginning of Main Menu=====-->
	<td valign="top" width="202">
		<div id="mainMenu"></div>
		<div id="subMenu"></div>
	</td>

	<td valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
		<!--===================================Beginning of Main Content===========================================-->
		<table width="95%" border="0" align="left" cellpadding="0" cellspacing="0" class="FormTitle" id="FormTitle">
		<tr>
			<td bgcolor="#4D595D" valign="top">
				<table width="760px" border="0" cellpadding="4" cellspacing="0">
				<tr>
					<td bgcolor="#4D595D" valign="top"  >
						<div>&nbsp;</div>
						<div class="formfonttitle">VPN Client</div>
						<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
						<div class="formfontdesc">
							<%tcWebApi_get("String_Entry","vpnc_desc1","s")%><br/>
							<%tcWebApi_get("String_Entry","vpnc_desc2","s")%><br/>
							<%tcWebApi_get("String_Entry","vpnc_desc3","s")%><br/><br/>
							<%tcWebApi_get("String_Entry","vpnc_step","s")%>
								<ol>
									<li><%tcWebApi_get("String_Entry","vpnc_step1","s")%></li>
									<li><%tcWebApi_get("String_Entry","vpnc_step2","s")%></li>
									<li><%tcWebApi_get("String_Entry","vpnc_step3","s")%></li>
								</ol>
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<table width="98%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
							<thead>
								<tr>
									<td colspan="6" id="VPNServerList" style="border-right:none;height:22px;"><%tcWebApi_get("String_Entry","BOP_isp_VPN_list","s")%> <span id="rules_limit" style="color:#FFFFFF"></span></td>
								</tr>
							</thead>
							<tr>
								<th width="10%" style="height:30px;"><%tcWebApi_get("String_Entry","PPPC_x_WANLink_in","s")%></th>
								<th><div><%tcWebApi_get("String_Entry","IPC_autofwDesc_in","s")%></div></th>
								<th width="15%"><div><%tcWebApi_get("String_Entry","QIS_internet_vpn_type","s")%></div></th>
								<th width="10%"><div><%tcWebApi_get("String_Entry","pvccfg_edit","s")%></div></th>
								<th width="10%"><div><%tcWebApi_get("String_Entry","CTL_del","s")%></div></th>
								<th width="25%"><div><%tcWebApi_get("String_Entry","Connecting","s")%></div></th>
							</tr>
						</table>

						<div id="vpnc_clientlist_Block"></div>
						<div class="apply_gen">
							<input class="button_gen_long" onclick="Add_profile()" type="button" value="<%tcWebApi_get("String_Entry","vpnc_step1","s")%>">
						</div>
					</td>
				</tr>
				</table>
			</td>
		</tr>
		</table>
		<!--===================================End of Main Content===========================================-->
	</td>

</tr>
</table>
</form>

<!---- vpnc_OpenVPN start  ---->
<div id="vpnc_setting_openvpn" class="contentM_qis" style="box-shadow: 1px 5px 10px #000; ">
	<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
		<tr style="height:32px;">
			<td>
				<table width="100%" border="0" align="left" cellpadding="0" cellspacing="0" class="vpnClientTitle">
					<tr>
						<td width="33%" align="center" id="pptpcTitle_openvpn" onclick="tabclickhandler(0);">PPTP</td>
						<td width="33%" align="center" id="l2tpcTitle_openvpn" onclick="tabclickhandler(1);">L2TP</td>
						<td width="33%" align="center" id="opencTitle_openvpn" onclick="tabclickhandler(2);">OpenVPN</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<div>
					<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
						<form method="post" name="vpnclientForm" action="start_apply.asp" target="hidden_frame">
						<input type="hidden" name="current_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="next_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="modified" value="0">
						<input type="hidden" name="flag" value="background">
						<input type="hidden" name="action_mode" value="apply">
						<input type="hidden" name="action_script" value="saveNvram">
						<input type="hidden" name="action_wait" value="1">
						<input type="hidden" name="preferred_lang" value="EN">
						<input type="hidden" name="firmver" value="">
						<input type="hidden" name="connFlag" value="0">
						<input type="hidden" name="listFlag" value="0">
						<input type="hidden" name="vpnc_clientlist" value=''>
						<input type="hidden" name="vpn_upload_type" value="ovpn">
						<input type="hidden" name="vpn_upload_unit" value="1">
						<input type="hidden" name="vpn_client_unit" value="1">
						<input type="hidden" name="vpnc_openvpn_unit_edit" value="1">
						<input type="hidden" name="vpnc_type" value="PPTP">
						<input type="hidden" name="vpnc_proto" value="<% tcWebApi_get("VPNC_Entry","proto","s") %>" disabled>
						<input type="hidden" name="vpn_clientx_eas" value="">
						<input type="hidden" name="vpnc_auto_conn" value="">
						<input type="hidden" name="vpnc_pptp_options_x_list" value="<% tcWebApi_Get("VPNC_Entry", "pptp_options_list", "s") %>">

		 				<tr>
							<th><%tcWebApi_get("String_Entry","IPC_autofwDesc_in","s")%></th>
							<td>
								<input type="text" maxlength="64" name="vpnc_openvpn_des" value="" class="input_32_table" style="float:left;" autocorrect="off" autocapitalize="off"></input>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%> (option)</th>
							<td>
								<input type="text" maxlength="64" name="vpnc_openvpn_username" value="" class="input_32_table" style="float:left;" autocomplete="off" autocorrect="off" autocapitalize="off"></input>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%> (option)</th>
							<td>
								<input type="text" maxlength="64" name="vpnc_openvpn_pwd" value="" class="input_32_table" style="float:left;" autocomplete="off" autocorrect="off" autocapitalize="off"></input>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","vpn_openvpnc_importovpn","s")%></th>
							<td>
								<input type="file" name="file" class="input" style="color:#FFCC00;*color:#000;"><br>
								<input class="button_gen" onclick="ImportOvpn(document.vpnclientForm.vpnc_openvpn_unit_edit.value);" type="button" value="<%tcWebApi_get("String_Entry","CTL_upload","s")%>" />
								<img id="loadingicon" style="margin-left:5px;display:none;" src="/images/InternetScan.gif">
								<span id="importOvpnFile" style="display:none;"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%></span>
							</td>
						</tr>
						</form>
					</table>

					<br/>
					<div style="color:#FC0;margin-bottom: 10px;">
						<input type="checkbox" id="cbManualImport" class="input" onclick="manualImport(this.checked)"><%tcWebApi_get("String_Entry","vpn_openvpnc_manual","s")%>
					</div>

					<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable" id="caFiled" style="display:none">
						<form method="post" name="openvpnCAForm" action="vpnupload.asp" target="hidden_frame" enctype="multipart/form-data">
						<input type="hidden" name="current_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="next_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="modified" value="0">
						<input type="hidden" name="flag" value="background">
						<input type="hidden" name="action_mode" value="apply">
						<input type="hidden" name="action_script" value="saveNvram">
						<input type="hidden" name="action_wait" value="1">
						<input type="hidden" name="preferred_lang" value="EN">
						<input type="hidden" name="firmver" value="">
						<input type="hidden" name="vpn_upload_type" value="ca">
						<input type="hidden" name="vpn_upload_unit" value="1">
						<tr>
							<th><%tcWebApi_get("String_Entry","vpn_openvpnc_importCA","s")%></th>
							<td>
								<input type="file" name="file" class="input" style="color:#FFCC00;*color:#000;"><br>
								<input id="importCA" class="button_gen" onclick="startImportCA(document.vpnclientForm.vpnc_openvpn_unit_edit.value);" type="button" value="<%tcWebApi_get("String_Entry","CTL_upload","s")%>" />
								<img id="loadingiconCA" style="margin-left:5px;display:none;" src="/images/InternetScan.gif">
							</td>
						</tr>
						</form>
					</table>

					<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable" id="certFiled" style="display:none">
						<form method="post" name="openvpnCertForm" action="vpnupload.asp" target="hidden_frame" enctype="multipart/form-data">
						<input type="hidden" name="current_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="next_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="modified" value="0">
						<input type="hidden" name="flag" value="background">
						<input type="hidden" name="action_mode" value="apply">
						<input type="hidden" name="action_script" value="saveNvram">
						<input type="hidden" name="action_wait" value="1">
						<input type="hidden" name="preferred_lang" value="EN">
						<input type="hidden" name="firmver" value="">
						<input type="hidden" name="vpn_upload_type" value="cert">
						<input type="hidden" name="vpn_upload_unit" value="1">
						<tr>
							<th>Import Cert file</th>
							<td>
								<input type="file" name="file" class="input" style="color:#FFCC00;*color:#000;"><br>
								<input class="button_gen" onclick="setTimeout('ovpnFileChecker();',2000);document.openvpnCertForm.submit();" type="button" value="<%tcWebApi_get("String_Entry","CTL_upload","s")%>" />
							</td>
						</tr>
						</form>
					</table>

					<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable" id="keyFiled" style="display:none">
						<form method="post" name="openvpnKeyForm" action="vpnupload.asp" target="hidden_frame" enctype="multipart/form-data">
						<input type="hidden" name="current_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="next_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="modified" value="0">
						<input type="hidden" name="flag" value="background">
						<input type="hidden" name="action_mode" value="apply">
						<input type="hidden" name="action_script" value="saveNvram">
						<input type="hidden" name="action_wait" value="1">
						<input type="hidden" name="preferred_lang" value="EN">
						<input type="hidden" name="firmver" value="">
						<input type="hidden" name="vpn_upload_type" value="key">
						<input type="hidden" name="vpn_upload_unit" value="1">
						<tr>
							<th>Import Key file</th>
							<td>
								<input type="file" name="file" class="input" style="color:#FFCC00;*color:#000;"><br>
								<input class="button_gen" onclick="setTimeout('ovpnFileChecker();',2000);document.openvpnKeyForm.submit();" type="button" value="<%tcWebApi_get("String_Entry","CTL_upload","s")%>" />
							</td>
						</tr>
						</form>
					</table>

					<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable" id="staticFiled" style="display:none">
						<form method="post" name="openvpnStaticForm" action="vpnupload.asp" target="hidden_frame" enctype="multipart/form-data">
						<input type="hidden" name="current_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="next_page" value="Advanced_VPNClient_Content.asp">
						<input type="hidden" name="modified" value="0">
						<input type="hidden" name="flag" value="background">
						<input type="hidden" name="action_mode" value="apply">
						<input type="hidden" name="action_script" value="saveNvram">
						<input type="hidden" name="action_wait" value="1">
						<input type="hidden" name="preferred_lang" value="EN">
						<input type="hidden" name="firmver" value="">
						<input type="hidden" name="vpn_upload_type" value="static">
						<input type="hidden" name="vpn_upload_unit" value="1">
						<tr>
							<th>Import Static file</th>
							<td>
								<input type="file" name="file" class="input" style="color:#FFCC00;*color:#000;"><br>
								<input class="button_gen" onclick="setTimeout('ovpnFileChecker();',2000);document.openvpnStaticForm.submit();" type="button" value="<%tcWebApi_get("String_Entry","CTL_upload","s")%>" />
							</td>
						</tr>
						</form>
					</table>

					<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable" id="manualFiled" style="display:none">
						<tr>
							<th><%tcWebApi_get("String_Entry","Manual_Setting_btn","s")%></th>
							<td>
		  					<input class="button_gen" onclick="document.getElementById('manualFiled_panel').style.display='';" type="button" value="<%tcWebApi_get("String_Entry","pvccfg_edit","s")%>">
					  	</td>
						</tr>
					</table>

					<div id="manualFiled_panel" class="contentM_qis_manual" style="display:none">
						<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
							<form method="post" name="openvpnManualForm" action="apply.asp" target="hidden_frame">
							<input type="hidden" name="current_page" value="Advanced_VPNClient_Content.asp">
							<input type="hidden" name="next_page" value="Advanced_VPNClient_Content.asp">
							<input type="hidden" name="modified" value="0">
							<input type="hidden" name="flag" value="background">
							<input type="hidden" name="action_mode" value="apply">
							<input type="hidden" name="action_script" value="saveNvram">
							<input type="hidden" name="action_wait" value="1">
							<input type="hidden" name="preferred_lang" value="EN">
							<input type="hidden" name="firmver" value="">
							<input type="hidden" name="vpn_client_unit" value="1">
							<input type="hidden" name="vpn_crt_client_ca" value="" disabled>
							<input type="hidden" name="vpn_crt_client_crt" value="" disabled>
							<input type="hidden" name="vpn_crt_client_key" value="" disabled>
							<input type="hidden" name="vpn_crt_client_static" value="" disabled>
							<input type="hidden" name="vpn_crt_client_crl" value="" disabled>
							<input type="hidden" name="formname" value="openvpnManualForm">
							<tr>
								<td valign="top">
									<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
										<tr>
											<td>
												<div class="description_down"><%tcWebApi_Get("String_Entry", "vpn_openvpn_Keys_Cert", "s")%></div>
											</td>
										</tr>
										<tr>
											<td>
												<div style="margin-left:30px; margin-top:10px;">
													<p/><%tcWebApi_Get("String_Entry", "vpn_openvpn_KC_Edit1", "s")%> <span style="color:#FC0;">----- BEGIN xxx ----- </span>/<span style="color:#FC0;"> ----- END xxx -----</span> <%tcWebApi_Get("String_Entry", "vpn_openvpn_KC_Edit2", "s")%>
													<p/><%tcWebApi_Get("String_Entry", "vpn_openvpn_KC_Limit", "s")%>
												</div>
												<div style="margin:5px;*margin-left:-5px;"><img style="width: 700px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
											</td>
										</tr>
										<tr>
											<td valign="top">
												<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
													<tr>
														<th id="manualCa">Certificate Authority</th>
														<td>
														<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client_ca" name="edit_vpn_crt_client_ca" cols="65" maxlength="3999"></textarea>
														</td>
													</tr>
													<tr>
														<th id="manualCert">Client Certificate</th>
														<td>
														<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client_crt" name="edit_vpn_crt_client_crt" cols="65" maxlength="3999"></textarea>
														</td>
													</tr>
													<tr>
														<th id="manualKey">Client Key</th>
														<td>
														<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client_key" name="edit_vpn_crt_client_key" cols="65" maxlength="3999"></textarea>
														</td>
													</tr>
													<tr>
														<th id="manualStatic">Static Key (Optional)</th>
														<td>
														<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client_static" name="edit_vpn_crt_client_static" cols="65" maxlength="3999"></textarea>
														</td>
													</tr>
													<tr>
														<th id="manualCRList">Certificate Revocation List (Optional)</th>
														<td>
														<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client_crl" name="edit_vpn_crt_client_crl" cols="65" maxlength="3999"></textarea>
														</td>
													</tr>
												</table>
											</td>
										</tr>
									</table>
									<div style="margin-top:5px;width:100%;text-align:center;">
										<input class="button_gen" type="button" onclick="cancel_Key_panel();" value="<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>">
										<input class="button_gen" type="button" onclick="saveManual(document.vpnclientForm.vpnc_openvpn_unit_edit.value);" value="<%tcWebApi_get("String_Entry","CTL_onlysave","s")%>">
									</div>
								</td>
					  		</tr>
							</form>
						</table>
					</div>
				</div>
			</td>
		</tr>
	</table>

	<div style="margin-top:5px;padding-bottom:10px;width:100%;text-align:center;">
		<input class="button_gen" type="button" onclick="cancel_add_rule();" id="cancelBtn_openvpn" value="<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>">
		<input class="button_gen" onclick='addOpenvpnProfile();' type="button" value="<%tcWebApi_get("String_Entry","CTL_ok","s")%>">
	</div>
<!--===================================Ending of openvpn setting Content===========================================-->
</div>

<div id="footer"></div>
</body>
</html>


