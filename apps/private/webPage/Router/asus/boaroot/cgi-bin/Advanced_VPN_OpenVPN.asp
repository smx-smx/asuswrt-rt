<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="images/favicon.png">
<link rel="icon" href="images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> - <%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/menu_style.css">
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" language="JavaScript" src="/merlin.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<style type="text/css">
.contentM_qis{
	width:740px;
	margin-top:280px;
	margin-left:380px;
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:200;
	background-color:#2B373B;
	display:none;
	/*behavior: url(/PIE.htc);*/
}

.QISform_wireless{
	width:600px;
	font-size:12px;
	color:#FFFFFF;
	margin-top:10px;
	*margin-left:10px;
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
window.onresize = cal_panel_block;

<% wanlink() %>
<% vpn_server_get_parameter() %>

var $j = jQuery.noConflict();

var vpn_server_clientlist_array_ori = '<%tcWebApi_char_to_ascii("OpenVPN_Common","vpn_server_clientlist", "s")%>';
var vpn_server_clientlist_array = decodeURIComponent(vpn_server_clientlist_array_ori);
var openvpn_unit = '1';
var vpn_server_enable = '<%tcWebApi_Get("VPNControl_Entry", "VPNServer_enable", "s")%>';
var open_vpn_enable = vpn_server_enable;
var service_state = '<%tcWebApi_Get("OpenVPN_Entry", "state", "s")%>';

var openvpnd_connected_clients = [];
var openvpn_clientlist_array = decodeURIComponent('<%tcWebApi_char_to_ascii("OpenVPN_Entry","ccd_val", "s")%>');
var ciphersarray = [
		["AES-128-CBC"],
		["AES-192-CBC"],
		["AES-256-CBC"],
		["BF-CBC"],
		["CAST5-CBC"],
		["CAMELLIA-128-CBC"],
		["CAMELLIA-192-CBC"],
		["CAMELLIA-256-CBC"],
		["DES-CBC"],
		["DES-EDE-CBC"],
		["DES-EDE3-CBC"],
		["DESX-CBC"],
		["IDEA-CBC"],
		["RC2-40-CBC"],
		["RC2-64-CBC"],
		["RC2-CBC"],
		["RC5-CBC"],
		["SEED-CBC"]
];
var hmacarray = [
	["MD 5", "MD5"],
	["SHA 1", "SHA1"],
	["SHA 224", "SHA224"],
	["SHA 256", "SHA256"],
	["SHA 384", "SHA384"],
	["SHA 512", "SHA512"],
	["RIPEMD 160", "RIPEMD160"],
	["RSA MD4", "RSA-MD4"]
];

function initial(){
	var current_server_igncrt = "<%tcWebApi_Get("OpenVPN_Entry","igncrt", "s")%>";
	var currentcipher = "<%tcWebApi_Get("OpenVPN_Entry","cipher", "s")%>";

	show_menu();
	
	//if support pptpd and openvpnd then show switch button
	if(pptpd_support && openvpnd_support) {
		document.getElementById("divSwitchMenu").style.display = "";
	}

	formShowAndHide(vpn_server_enable, "openvpn");

	//check DUT is belong to private IP.
	if(realip_support){
		if(!external_ip){
			document.getElementById("privateIP_notes").innerHTML = "<%tcWebApi_Get("String_Entry","vpn_privateIP_hint","s")%>"
			document.getElementById("privateIP_notes").style.display = "";
		}
	}
	else if(validator.isPrivateIP(wanlink_ipaddr())){
		document.getElementById("privateIP_notes").innerHTML = "<%tcWebApi_Get("String_Entry","vpn_privateIP_hint","s")%>"
		document.getElementById("privateIP_notes").style.display = "";
	}

	/*Advanced Setting start */
	allowed_openvpn_clientlist();

	//generate select option of cipher list
	add_option(document.form.vpn_server_cipher, "Default","default",(currentcipher == "default"));
	add_option(document.form.vpn_server_cipher, "None","none",(currentcipher == "none"));
	for(var i = 0; i < ciphersarray.length; i += 1){
		add_option(document.form.vpn_server_cipher, ciphersarray[i][0], ciphersarray[i][0], (currentcipher == ciphersarray[i][0]));
	}

	//generate select option of HMAC list
	var currentHMAC = '<%tcWebApi_Get("OpenVPN_Entry","digest", "s")%>';
	for(var i = 0; i < hmacarray.length; i += 1) {
		add_option(document.form.vpn_server_digest, hmacarray[i][0], hmacarray[i][1], (currentHMAC == hmacarray[i][1]));
	}

	// Set these based on a compound field
	setRadioValue(document.form.vpn_server_x_eas, ((document.form.vpn_serverx_eas.value.indexOf(''+(openvpn_unit)) >= 0) ? "1" : "0"));
	setRadioValue(document.form.vpn_server_x_dns, ((document.form.vpn_serverx_dns.value.indexOf(''+(openvpn_unit)) >= 0) ? "1" : "0"));

	enable_server_igncrt(current_server_igncrt);
	//update_visibility();
	update_cipher();
	update_digest();

	/*Advanced Setting end */

	updateVpnServerClientAccess();
	setTimeout("update_FAQ();", 300);
}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq_windows", "https://www.asus.com", "/support/FAQ/", "1004469");
		faqURL("faq_macOS", "https://www.asus.com", "/support/FAQ/", "1004472");
		faqURL("faq_iPhone", "https://www.asus.com", "/support/FAQ/", "1004471");
		faqURL("faq_android", "https://www.asus.com", "/support/FAQ/", "1004466");
	}
}

function show_selected_language_path(){
var lang_flag = get_preferred_lang_type();
	switch(lang_flag){
		case '1':{	//EN
			return "/";
			break;
		}
		case '3':{
			return "/CN/";
			break;
		}
		case '18':{
			return "/TW/";
			break;
		}
		case '2':{	//BR
			return "/";
			break;
		}
		case '4':{
			return "/CZ/";
			break;
		}
		case '5':{	//DA
			return "/";
			break;
		}
		case '6':{
			return "/DE/";
			break;
		}
		case '7':{
			return "/ES/";
			break;
		}
		case '8':{	//FI
			return "/";
			break;
		}
		case '9':{
			return "/FR/";
			break;
		}
		case '10':{
			return "/IT/";
			break;
		}
		case '11':{	//MS
			return "/";
			break;
		}
		case '12':{ //NO
			return "/";
			break;
		}
		case '13':{
			return "/PL/";
			break;
		}
		case '14':{
			return "/RU/";
			break;
		}
		case '15':{	//SV
			return "/";
			break;
		}
		case '16':{
			return "/TH/";
			break;
		}
		case '17':{
			return "/TR/";
			break;
		}
		case '19':{
			return "/UK/";
			break;
		}
	}
}

function formShowAndHide(server_enable, server_type) {

	if(server_enable == 1 && server_type == "openvpn"){     //General View
		document.getElementById("trVPNServerMode").style.display = "";		
		document.getElementById("selSwitchMode").value = "1";
		document.getElementById("trRSAEncryptionBasic").style.display = ("<%tcWebApi_Get("OpenVPN_Entry","crypt","s")%>" == "secret")?"none":"";
		document.getElementById("trClientWillUseVPNToAccess").style.display = "";
		document.getElementById('OpenVPN_setting').style.display = ("<%tcWebApi_Get("OpenVPN_Entry","crypt","s")%>" == "secret")?"none":"";

		if(vpn_server_enable == '0' )
			document.getElementById('openvpn_export').style.display = "none";
		else
			document.getElementById('openvpn_export').style.display = "";

		document.getElementById("divAdvanced").style.display = "none";

		if(service_state == false || service_state != '2')
			document.getElementById('export_div').style.display = "none";

		if(!email_support)
			document.getElementById('exportViaEmail').style.display = "none";

		showopenvpnd_clientlist();
		update_vpn_client_state();
		openvpnd_connected_status();
		check_vpn_server_state();
		document.getElementById("divApply").style.display = "";
		updateVpnServerClientAccess();
	}
	else{
		document.getElementById("trVPNServerMode").style.display = "none";
		document.getElementById("trRSAEncryptionBasic").style.display = "none";
		document.getElementById("trClientWillUseVPNToAccess").style.display = "none";
		document.getElementById("openvpn_export").style.display = "none";
		document.getElementById("OpenVPN_setting").style.display = "none";
		document.getElementById("divAdvanced").style.display = "none";
	}
}

function openvpnd_connected_status(){
	var rule_num = document.getElementById("openvpnd_clientlist_table").rows.length;
	var username_status = "";
	for(var x=0; x < rule_num; x++){
		var ind = x;
		username_status = "conn"+ind;
		if(openvpnd_connected_clients.length >0){
			if(document.getElementById(username_status)) {
				document.getElementById(username_status).innerHTML = '<%tcWebApi_Get("String_Entry","Disconnected","s")%>';
			}
			for(var y=0; y<openvpnd_connected_clients.length; y++){
				if(document.getElementById("openvpnd_clientlist_table").rows[x].cells[1].title == openvpnd_connected_clients[y].username){
					document.getElementById(username_status).innerHTML = '<a class="hintstyle2" href="javascript:void(0);" onClick="showOpenVPNClients(\''+openvpnd_connected_clients[y].username+'\');"><%tcWebApi_Get("String_Entry","Connected","s")%></a>';
					break;
				}
			}
		}else if(document.getElementById(username_status)){
			document.getElementById(username_status).innerHTML = '<%tcWebApi_Get("String_Entry","Disconnected","s")%>';
		}
	}
}

function applyRule(){
	var validForm = function() {
		if(!validator.numberRange(document.form.vpn_server_port, 1, 65535)) {
			return false;
		}
		/*!-- rm 2017/06/28 
		if(!validator.numberRange(document.form.vpn_server_poll, 0, 1440)) {
			return false;
		}
		*/
		if(!validator.numberRange(document.form.vpn_server_reneg, -1, 2147483647)) {
			return false;
		}
		return true;
	};
	if(!validForm())
		return false;

	var confirmFlag = true;

	/* Advanced setting start */
	var check_openvpn_conflict = function () {		//if conflict with LAN ip & DHCP ip pool & static
		var origin_lan_ip = '<%tcWebApi_Get("Lan_Entry0", "IP", "s")%>';
		var lan_ip_subnet = origin_lan_ip.split(".")[0]+"."+origin_lan_ip.split(".")[1]+"."+origin_lan_ip.split(".")[2]+".";
		var lan_ip_end = parseInt(origin_lan_ip.split(".")[3]);
		var pool_start = '<%tcWebApi_Get("Dhcpd_Common", "start", "s")%>';
		var pool_end = '<%tcWebApi_Get("Dhcpd_Common", "end", "s")%>';
		var dhcp_staticlists = '<% nvram_get("dhcp_staticlist"); %>';
		var netmask_obj = document.form.vpn_server_nm;
		var vpnSubnet = document.form.vpn_server_sn;
		var pool_subnet = pool_start.split(".")[0]+"."+pool_start.split(".")[1]+"."+pool_start.split(".")[2]+".";

		if(document.form.vpn_server_if.value == 'tun'){
			if(vpnSubnet.value == ""){
				alert("<%tcWebApi_Get("String_Entry","JS_fieldblank","s")%>");
				vpnSubnet.focus();
				vpnSubnet.select();
				return false;
			}

			if(!validator.ipRange(vpnSubnet, "")){
				vpnSubnet.focus();
				vpnSubnet.select();
				return false;
			}

			var openvpn_server_subnet = vpnSubnet.value.split(".")[0]
								+ "." + vpnSubnet.value.split(".")[1]
								+ "." + vpnSubnet.value.split(".")[2]
								+ ".";

			//LAN ip
			if(origin_lan_ip == vpnSubnet.value) {
				alert("<%tcWebApi_Get("String_Entry","vpn_conflict_LANIP","s")%> " + origin_lan_ip);
				vpnSubnet.focus();
				vpnSubnet.select();
				return false;
			}

			//LAN ip pool
			if(lan_ip_subnet == openvpn_server_subnet) {
				alert("<%tcWebApi_Get("String_Entry","vpn_conflict_DHCPpool","s")%>"+pool_start+" ~ "+pool_end);
				vpnSubnet.focus();
				vpnSubnet.select();
				return false;
			}

			//mask must be 255.255.0.0 ~ 255.255.255.248 openvpn limiting
			if(!validator.maskRange("255.255.0.0", "255.255.255.248", netmask_obj.value)) {
				alert("Netmask range must be 255.255.0.0 (/16) ~ 255.255.255.248 (/29)");
				netmask_obj.focus();
				netmask_obj.select();
				return false;
			}

			//check subnet/netmask combination whether it invalid or not

			if(!validator.subnetAndMaskCombination(vpnSubnet.value, netmask_obj.value)) {
				alert(vpnSubnet.value + " / " + netmask_obj.value + " combination is invalid");
				vpnSubnet.focus();
				vpnSubnet.select();
				return false;
			}
		}
		else if(document.form.vpn_server_if.value == 'tap' && document.form.vpn_server_dhcp.value == '0'){
			if(!validator.isLegalIP(document.form.vpn_server_r1, "")){
				document.form.vpn_server_r1.focus();
				document.form.vpn_server_r1.select();
				return false;
			}

			if(document.form.vpn_server_r1.value.split(".")[3] == 255){		//*.*.*.255 can't be IP in the IP pool
				alert(document.form.vpn_server_r1.value + " <%tcWebApi_Get("String_Entry","JS_validip","s")%>");
				document.form.vpn_server_r1.focus();
				document.form.vpn_server_r1.select();
				return false;
			}

			if(!validator.isLegalIP(document.form.vpn_server_r2, "")){
				document.form.vpn_server_r2.focus();
				document.form.vpn_server_r2.select();
				return false;
			}

			if(document.form.vpn_server_r2.value.split(".")[3] == 255){		//*.*.*.255 can't be IP in the IP pool
				alert(document.form.vpn_server_r2.value + " <%tcWebApi_Get("String_Entry","JS_validip","s")%>");
				document.form.vpn_server_r2.focus();
				document.form.vpn_server_r2.select();
				return false;
			}

			var openvpn_clients_start_subnet = document.form.vpn_server_r1.value.split(".")[0] + "."
											 + document.form.vpn_server_r1.value.split(".")[1] + "."
											 + document.form.vpn_server_r1.value.split(".")[2] + ".";
			var openvpn_clients_end_subnet = document.form.vpn_server_r2.value.split(".")[0] + "."
										   + document.form.vpn_server_r2.value.split(".")[1] + "."
										   + document.form.vpn_server_r2.value.split(".")[2] + ".";
			var openvpn_clients_start_ip = parseInt(document.form.vpn_server_r1.value.split(".")[3]);
			var openvpn_clients_end_ip = parseInt(document.form.vpn_server_r2.value.split(".")[3]);

			//LAN ip
			if( (lan_ip_subnet == openvpn_clients_start_subnet || lan_ip_subnet == openvpn_clients_end_subnet)
			 && (lan_ip_end >= openvpn_clients_start_ip && lan_ip_end <= openvpn_clients_end_ip)) {
				alert("<%tcWebApi_Get("String_Entry","vpn_conflict_LANIP","s")%> "+origin_lan_ip);
				document.form.vpn_server_r1.focus();
				document.form.vpn_server_r1.select();
				return false;
			}

			if(openvpn_clients_end_ip < openvpn_clients_start_ip){
				alert(document.form.vpn_server_r2.value + " <%tcWebApi_Get("String_Entry","JS_validip","s")%>");
				document.form.vpn_server_r2.focus();
				document.form.vpn_server_r2.select();
				return false;
			}

			//DHCP pool
			if(pool_subnet != openvpn_clients_start_subnet) {
				alert(document.form.vpn_server_r1.value + " <%tcWebApi_Get("String_Entry","JS_validip","s")%>");
				document.form.vpn_server_r1.focus();
				document.form.vpn_server_r1.select();
				return false;
			}

			if(pool_subnet != openvpn_clients_end_subnet) {
				alert(document.form.vpn_server_r2.value + " <%tcWebApi_Get("String_Entry","JS_validip","s")%>");
				document.form.vpn_server_r2.focus();
				document.form.vpn_server_r2.select();
				return false;
			}

			//DHCP static IP
			if(dhcp_staticlists != "") {
				for(var i = 1; i < staticclist_row.length; i +=1 ) {
					var static_subnet = static_ip.split(".")[0]+"."+static_ip.split(".")[1]+"."+static_ip.split(".")[2]+".";
					var static_end = parseInt(static_ip.split(".")[3]);
					var static_ip = staticclist_row[i].split('>')[1];
					if(static_subnet != openvpn_clients_start_subnet) {
						alert(document.form.vpn_server_r1.value + " <%tcWebApi_Get("String_Entry","JS_validip","s")%>");
						document.form.vpn_server_r1.focus();
						document.form.vpn_server_r1.select();
						return false;
					}
					if(static_subnet != openvpn_clients_end_subnet) {
						alert(document.form.vpn_server_r2.value + " <%tcWebApi_Get("String_Entry","JS_validip","s")%>");
						document.form.vpn_server_r2.focus();
						document.form.vpn_server_r2.select();
						return false;
					}
				}
			}
		}

		return true;
	};

	/* Advanced setting end */
	if(confirmFlag && check_openvpn_conflict() ) {
		document.openvpnTLSKeyForm.vpn_crt_server1_ca.disabled = true;
		document.openvpnTLSKeyForm.vpn_crt_server1_crt.disabled = true;
		document.openvpnTLSKeyForm.vpn_crt_server1_key.disabled = true;
		document.openvpnTLSKeyForm.vpn_crt_server1_dh.disabled = true;
		document.openvpnTLSKeyForm.vpn_crt_server1_static.disabled = true;
		document.openvpnTLSKeyForm.vpn_crt_server1_crl.disabled = true;
		document.openvpnStaticKeyForm.vpn_crt_server1_static.disabled = true;

		var get_group_value = function () {
			var rule_num = document.getElementById("openvpnd_clientlist_table").rows.length;
			var item_num = document.getElementById("openvpnd_clientlist_table").rows[0].cells.length;
			var tmp_value = "";

			for(var i = 1; i < rule_num; i += 1) {
				tmp_value += "<"
				for(var j = 1; j < item_num - 1; j += 1) {
					if(document.getElementById("openvpnd_clientlist_table").rows[i].cells[j].innerHTML.lastIndexOf("...") < 0) {
						tmp_value += document.getElementById("openvpnd_clientlist_table").rows[i].cells[j].innerHTML;
					}
					else {
						tmp_value += document.getElementById("openvpnd_clientlist_table").rows[i].cells[j].title;
					}
					if(j != item_num - 2)
						tmp_value += ">";
				}
			}
			if(tmp_value == "<"+"<%tcWebApi_Get("String_Entry","IPC_VSList_Norule","s")%>" || tmp_value == "<")
				tmp_value = "";

			return tmp_value;
		};

		if(document.form.VPNServer_enable.value == "1") {
			document.form.action_script.value = "restart_openvpnd;restart_chpass";
			document.form.vpn_server_clientlist.value = get_group_value();
			/* Advanced setting start */
			//Viz add 2014.06
			if(document.getElementById("server_reneg").style.display == "none")
				document.form.vpn_server_reneg.disabled = true;

			var getAdvancedValue = function () {
				var client_num = document.getElementById("openvpn_clientlist_table").rows.length;
				var item_num = document.getElementById("openvpn_clientlist_table").rows[0].cells.length;
				var tmp_value = "";
				for(var i = 0; i < client_num; i += 1) {
					// Insert first field (1), which enables the client.
					tmp_value += "<1>";
					for(var j = 0; j < item_num - 1; j += 1) {
						if (j == 3)
							tmp_value += (document.getElementById("openvpn_clientlist_table").rows[i].cells[j].innerHTML == "Yes" ? 1 : 0);
						else
							tmp_value += document.getElementById("openvpn_clientlist_table").rows[i].cells[j].innerHTML;

						if(j != item_num - 2)
							tmp_value += ">";
					}
				}

				if(tmp_value == "<"+"<%tcWebApi_Get("String_Entry","IPC_VSList_Norule","s")%>" || tmp_value == "<1>")
					tmp_value = "";

				document.form.vpn_server_ccd_val.value = tmp_value;
				tmp_value = "";
				for (var i = 1; i < 3; i += 1) {
					if (i == openvpn_unit) {
						if(getRadioValue(document.form.vpn_server_x_eas) == 1)
							tmp_value += ""+i+",";
					}
					else{
						if(document.form.vpn_serverx_eas.value.indexOf(''+(i)) >= 0)
							tmp_value += ""+i+","
					}
				}

				document.form.vpn_serverx_eas.value = tmp_value;
				tmp_value = "";
				for (var i = 1; i < 3; i += 1) {
					if (i == openvpn_unit) {
						if (getRadioValue(document.form.vpn_server_x_dns) == 1)
							tmp_value += ""+i+",";
					} else {
						if (document.form.vpn_serverx_dns.value.indexOf(''+(i)) >= 0)
							tmp_value += ""+i+","
					}
				}

				if (tmp_value != document.form.vpn_serverx_dns.value) {
					document.form.action_script.value += ";restart_dnsmasq";
					document.form.vpn_serverx_dns.value = tmp_value;
				}
			}();
			/* Advanced setting end */
		}
		else {		//disable server
			document.form.action_script.value = "stop_openvpnd";
			document.form.vpn_server_clientlist.value = get_group_value();
		}

		showLoading();
		document.form.submit();
	}
}

function addRow(obj, head){
	if(head == 1)
		vpn_server_clientlist_array += "<" /*&#60*/
	else
		vpn_server_clientlist_array += ">" /*&#62*/

	vpn_server_clientlist_array += obj.value;
	obj.value = "";
}

function validForm(){
	var valid_username = document.form.vpn_server_clientlist_username;
	var valid_password = document.form.vpn_server_clientlist_password;

	if(valid_username.value == "") {
		alert("<%tcWebApi_Get("String_Entry","JS_fieldblank","s")%>");
		valid_username.focus();
		return false;
	}
	else if(!Block_chars(valid_username, [" ", "@", "*", "+", "|", ":", "?", "<", ">", ",", ".", "/", ";", "[", "]", "\\", "=", "\"", "&" ])) {
		return false;
	}

	if(valid_password.value == "") {
		alert("<%tcWebApi_Get("String_Entry","JS_fieldblank","s")%>");
		valid_password.focus();
		return false;
	}
	else if(!Block_chars(valid_password, ["<", ">", "&"])) {
		return false;
	}

	return true;
}

function addRow_Group(upper){
	var username_obj = document.form.vpn_server_clientlist_username;
	var password_obj = document.form.vpn_server_clientlist_password;
	var rule_num = document.getElementById("openvpnd_clientlist_table").rows.length;
	var item_num = document.getElementById("openvpnd_clientlist_table").rows[0].cells.length;
	if(rule_num >= upper) {
		alert("<%tcWebApi_Get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_Get("String_Entry","JS_itemlimit2","s")%>");
		return false;
	}

	if(validForm()){
		//Viz check same rule  //match(username) is not accepted
		if(item_num >= 2) {
			for(var i = 0; i < rule_num; i += 1) {
				if(username_obj.value == document.getElementById("openvpnd_clientlist_table").rows[i].cells[1].title) {
					alert("<%tcWebApi_Get("String_Entry","JS_duplicate","s")%>");
					username_obj.focus();
					username_obj.select();
					return false;
				}
			}
		}

		addRow(username_obj ,1);
		addRow(password_obj, 0);
		showopenvpnd_clientlist();
		openvpnd_connected_status();
	}
}

function del_Row(rowdata){
	var i = rowdata.parentNode.parentNode.rowIndex;
	document.getElementById("openvpnd_clientlist_table").deleteRow(i);
  	var vpn_server_clientlist_value = "";
  	var rowLength = document.getElementById("openvpnd_clientlist_table").rows.length;
	for(var k = 1; k < rowLength; k += 1){
		for(var j=1; j < document.getElementById("openvpnd_clientlist_table").rows[k].cells.length - 1; j += 1) {
			if(j == 1)
				vpn_server_clientlist_value += "<";
			else {
				vpn_server_clientlist_value += document.getElementById("openvpnd_clientlist_table").rows[k].cells[1].innerHTML;
				vpn_server_clientlist_value += ">";
				vpn_server_clientlist_value += document.getElementById("openvpnd_clientlist_table").rows[k].cells[2].innerHTML;
			}
		}
	}

	vpn_server_clientlist_array = vpn_server_clientlist_value;
	if(vpn_server_clientlist_array == "") {
		showopenvpnd_clientlist();
		openvpnd_connected_status();
	}
}

var overlib_str2 = new Array();	//Viz add 2013.10 for record longer VPN client username/pwd for OpenVPN
var overlib_str3 = new Array();	//Viz add 2013.10 for record longer VPN client username/pwd for OpenVPN
function showopenvpnd_clientlist(){
	var vpn_server_clientlist_row = vpn_server_clientlist_array.split('<');
	var code = "";
	code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="openvpnd_clientlist_table">';
	code +='<tr id="row0"><td width="15%" id="conn0"></td><td width="35%" title="<%tcWebApi_Get("Account_Entry0", "username", "s")%>"><%tcWebApi_Get("Account_Entry0", "username", "s")%></td><td width="35%" style="text-align:center;">-</td><td width="15%" style="text-align:center;">-</td></tr>';
	if(vpn_server_clientlist_row.length > 1){
		for(var i = 1; i < vpn_server_clientlist_row.length; i++){
			overlib_str2[i] = "";
			overlib_str3[i] = "";
			code +='<tr id="row'+i+'">';
			var vpn_server_clientlist_col = vpn_server_clientlist_row[i].split('>');
			code +='<td width="15%" id="conn'+i+'"></td>';
			for(var j = 0; j < vpn_server_clientlist_col.length; j++){
				if(j == 0){
					if(vpn_server_clientlist_col[0].length >32){
						overlib_str2[i] += vpn_server_clientlist_col[0];
						vpn_server_clientlist_col[0] = vpn_server_clientlist_col[0].substring(0, 30)+"...";
						code +='<td width="35%" title="'+overlib_str2[i]+'">'+ vpn_server_clientlist_col[0] +'</td>';
					}else{
						code +='<td width="35%" title="'+vpn_server_clientlist_col[0]+'">'+ vpn_server_clientlist_col[0] +'</td>';
					}
				}
				else if(j ==1){
					if(vpn_server_clientlist_col[1].length >32){
						overlib_str3[i] += vpn_server_clientlist_col[1];
						vpn_server_clientlist_col[1] = vpn_server_clientlist_col[1].substring(0, 30)+"...";
						code +='<td width="35%" title="'+overlib_str3[i]+'">'+ vpn_server_clientlist_col[1] +'</td>';
					}else{
						code +='<td width="35%">'+ vpn_server_clientlist_col[1] +'</td>';
					}
				}
			}

			code +='<td width="15%">';
			code +='<input class="remove_btn" onclick="del_Row(this, \'openvpnd\');" value=""/></td></tr>';
		}
	}

	code +='</table>';
	document.getElementById("openvpnd_clientlist_Block").innerHTML = code;
}

function parseOpenVPNClients(client_status){		//192.168.123.82:46954 10.8.0.6 pine\n
	openvpnd_connected_clients = [];
	var Loginfo = client_status;
	if (Loginfo == "") {return;}

	Loginfo = Loginfo.replace('\r\n', '\n');
	Loginfo = Loginfo.replace('\n\r', '\n');
	Loginfo = Loginfo.replace('\r', '\n');

	var lines = Loginfo.split('\n');
	for (i = 0; i < lines.length; i++){
		var fields = lines[i].split(' ');
		if ( fields.length != 3 ) continue;
		openvpnd_connected_clients.push({"username":fields[2],"remoteIP":fields[0],"VPNIP":fields[1]});
	}
}

function showOpenVPNClients(uname){
	var statusmenu = "";
	var statustitle = "";
	statustitle += "<div style=\"text-decoration:underline;\">VPN IP ( Remote IP )</div>";
	_caption = statustitle;

	for (i = 0; i < openvpnd_connected_clients.length; i++){
		if(uname == openvpnd_connected_clients[i].username){
				statusmenu += "<div>"+openvpnd_connected_clients[i].VPNIP+" \t( "+openvpnd_connected_clients[i].remoteIP+" )</div>";
		}
	}

	return overlib(statusmenu, WIDTH, 260, OFFSETX, -360, LEFT, STICKY, CAPTION, _caption, CLOSETITLE, '');
}

function check_vpn_server_state(){
	if(vpn_server_enable == '1' && service_state != '2'){
		document.getElementById('export_div').style.display = "none";
		document.getElementById('openvpn_initial').style.display = "";
		update_vpn_server_state();
	}
}

function update_vpn_server_state() {
	$j.ajax({
		url: '/cgi-bin/ajax_openvpn_server.asp',
		dataType: 'script',

		error: function(xhr) {
			setTimeout("update_vpn_server_state();", 1000);
		},

		success: function() {
			if(vpnd_state != '2' && (vpn_server1_errno == '1' || vpn_server1_errno == '2')){
				document.getElementById('openvpn_initial').style.display = "none";
				document.getElementById('openvpn_error_message').innerHTML = "<span><%tcWebApi_Get("String_Entry","vpn_openvpn_fail1","s")%></span>";
				document.getElementById('openvpn_error_message').style.display = "";
			}
			else if(vpnd_state != '2' && vpn_server1_errno == '4'){
				document.getElementById('openvpn_initial').style.display = "none";
				document.getElementById('openvpn_error_message').innerHTML = "<span><%tcWebApi_Get("String_Entry","vpn_openvpn_fail2","s")%></span>";
				document.getElementById('openvpn_error_message').style.display = "";
			}
			else if(vpnd_state != '2' && vpn_server1_errno == '5'){
				document.getElementById('openvpn_initial').style.display = "none";
				document.getElementById('openvpn_error_message').innerHTML = "<span><%tcWebApi_Get("String_Entry","vpn_openvpn_fail3","s")%></span>";
				document.getElementById('openvpn_error_message').style.display = "";
			}
			else if(vpnd_state == '-1' && vpn_server1_errno == '0'){
				document.getElementById('openvpn_initial').style.display = "none";
				document.getElementById('openvpn_error_message').innerHTML = "<span><%tcWebApi_Get("String_Entry","vpn_openvpn_fail4","s")%></span>";
				document.getElementById('openvpn_error_message').style.display = "";
			}
			else if(vpnd_state != '2'){
				setTimeout("update_vpn_server_state();", 1000);
			}
			else{	// OpenVPN server ready , vpn_server1_state==2
				setTimeout("location.href='Advanced_VPN_OpenVPN.asp';", 1000);
				return;
			}
		}
	});
}

function showMailPanel(){
	var checker = {
		server: document.mailConfigForm.PM_SMTP_SERVER.value,
		mailPort: document.mailConfigForm.PM_SMTP_PORT.value,
		user: document.mailConfigForm.PM_SMTP_AUTH_USER.value,
		pass: document.mailConfigForm.PM_SMTP_AUTH_PASS.value,
		end: 0
	}

	if(checker.server == "" || checker.mailPort == "" || checker.user == "" || checker.pass == ""){
		$j("#mailConfigPanelContainer").fadeIn(300);
		$j("#mailSendPanelContainer").fadeOut(300);
	}
	else{
		$j("#mailConfigPanelContainer").fadeOut(300);
		$j("#mailSendPanelContainer").fadeIn(300);
	}
}

function switchMode(mode){
	if(mode == "1"){		//general setting
		
		document.getElementById("trRSAEncryptionBasic").style.display = ("<%tcWebApi_Get("OpenVPN_Entry","crypt","s")%>" == "secret")?"none":"";		
		document.getElementById("trClientWillUseVPNToAccess").style.display = "";
		document.getElementById('OpenVPN_setting').style.display = ("<%tcWebApi_Get("OpenVPN_Entry","crypt","s")%>" == "secret")?"none":"";		
		if(vpn_server_enable == '0')
			document.getElementById('openvpn_export').style.display = "none";
		else
			document.getElementById('openvpn_export').style.display = "";
		document.getElementById("divAdvanced").style.display = "none";
		updateVpnServerClientAccess();

	}
	else{

		document.getElementById("trRSAEncryptionBasic").style.display = "none";
		document.getElementById("trClientWillUseVPNToAccess").style.display = "none";
		document.getElementById("OpenVPN_setting").style.display = "none";
		document.getElementById("openvpn_export").style.display = "none";
		document.getElementById("divAdvanced").style.display = "";	
	}
}

/* Advanced Setting start */
function change_vpn_unit(val){
	FormActions("apply.cgi", "change_vpn_server_unit", "", "");
	document.form.target = "";
	document.form.submit();
}

function update_visibility(){
	var auth = document.form.vpn_server_crypt.value;
	var iface = document.form.vpn_server_if.value;
	var hmac = document.form.vpn_server_hmac.value;
	var dhcp = getRadioValue(document.form.vpn_server_dhcp);
	var dns = getRadioValue(document.form.vpn_server_x_dns);
	if(auth != "tls")
		ccd = 0;
	else
		ccd = getRadioValue(document.form.vpn_server_ccd);

	showhide("server_authhmac", (auth != "secret"));
	showhide("server_snnm", ((auth == "tls") && (iface == "tun")));
	showhide("server_plan", ((auth == "tls") && (iface == "tun")));
	showhide("server_rgw", (auth == "tls"));
	showhide("server_local", ((auth == "secret") && (iface == "tun")));
	showhide("server_reneg", (auth != "secret"));		//add by Viz 2014.06
	showhide("server_ccd", (auth == "tls"));
	showhide("server_c2c", ccd);
	showhide("server_ccd_excl", ccd);
	showhide("openvpn_client_table", ccd);
	showhide("openvpn_clientlist_Block", ccd);
	showhide("server_pdns", ((auth == "tls") && (dns == 1)));
	showhide("server_dhcp",((auth == "tls") && (iface == "tap")));
	showhide("server_range", ((dhcp == 0) && (auth == "tls") && (iface == "tap")));
	showhide("server_tls_crypto_text", (auth == "tls"));		//add by Viz
	showhide("server_static_crypto_text", (auth == "secret"));		//add by Viz
	// rm 2017/07/21 showhide("server_custom_crypto_text", (auth == "custom"));
}

function set_Keys(auth) {
	cal_panel_block();
	updateCRTValue(auth);
	if(auth == 'tls') {
		$j("#tlsKey_panel").fadeIn(300);
	}
	else if(auth == 'secret') {
		$j("#staticKey_panel").fadeIn(300);
	}
}

function enable_server_igncrt(flag){
	//alert((flag==1)?"Hide vpn_server_crypt":"Display vpn_server_crypt");
	document.form.vpn_server_crypt.style.display = (flag==1)?"none":"";
	document.form.vpn_server_crypt.value = (flag==1)?"tls":"<%tcWebApi_Get("OpenVPN_Entry","crypt","s")%>";
	update_visibility();
	document.getElementById("Hint_fixed_tls_crypto").style.display = (flag==1)?"":"none";
	document.getElementById("Fixed_tls_crypto").style.display = (flag==1)?"":"none";
	document.getElementById("allowed_client_name").innerHTML = (flag==1)?"<#HSDPAConfig_Username_itemname#>":"Common Name(CN)";

}

function updateCRTValue(auth){
	$j.ajax({
		url: '/cgi-bin/ajax_openvpn_server.asp',
		dataType: 'script',
		timeout: 1500,
		error: function(xhr){
			setTimeout("updateCRTValue('"+auth+"');",1000);
		},
		success: function(){
			if(auth == "tls") {
				document.openvpnTLSKeyForm.edit_vpn_crt_server1_ca.value = vpn_crt_server1_ca[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
				document.openvpnTLSKeyForm.edit_vpn_crt_server1_crt.value = vpn_crt_server1_crt[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
				document.openvpnTLSKeyForm.edit_vpn_crt_server1_key.value = vpn_crt_server1_key[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
				document.openvpnTLSKeyForm.edit_vpn_crt_server1_dh.value = vpn_crt_server1_dh[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
				document.openvpnTLSKeyForm.edit_vpn_crt_server1_static.value = vpn_crt_server1_static[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
				document.openvpnTLSKeyForm.edit_vpn_crt_server1_crl.value = vpn_crt_server1_crl[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			}
			else if(auth == "secret") {
				document.openvpnStaticKeyForm.edit_vpn_crt_server1_static.value = vpn_crt_server1_static[0].replace(/&#10/g, "\n").replace(/&#13/g, "\r");
			}
		}
	})
}

function addRow_Group_Advanced(upper){
	var client_num = document.getElementById("openvpn_clientlist_table").rows.length;
	var item_num = document.getElementById("openvpn_clientlist_table").rows[0].cells.length;
	if(client_num >= upper){
		alert("<%tcWebApi_Get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_Get("String_Entry","JS_itemlimit2","s")%>");
		return false;
	}

	if(document.form.vpn_clientlist_commonname_0.value==""){
		alert("<%tcWebApi_Get("String_Entry","JS_fieldblank","s")%>");
		document.form.vpn_clientlist_commonname_0.focus();
		document.form.vpn_clientlist_commonname_0.select();
		return false;
	}

	if(document.form.vpn_clientlist_subnet_0.value==""){
		alert("<%tcWebApi_Get("String_Entry","JS_fieldblank","s")%>");
		document.form.vpn_clientlist_subnet_0.focus();
		document.form.vpn_clientlist_subnet_0.select();
		return false;
	}

	if(document.form.vpn_clientlist_netmask_0.value==""){
		alert("<%tcWebApi_Get("String_Entry","JS_fieldblank","s")%>");
		document.form.vpn_clientlist_netmask_0.focus();
		document.form.vpn_clientlist_netmask_0.select();
		return false;
	}

	if(!validator.isLegalIP(document.form.vpn_clientlist_subnet_0)) {
		return false;
	}

	if(!validator.isLegalMask(document.form.vpn_clientlist_netmask_0)){
		return false;
	}

	if(!validator.subnetAndMaskCombination(document.form.vpn_clientlist_subnet_0.value, document.form.vpn_clientlist_netmask_0.value)) {
		alert(document.form.vpn_clientlist_subnet_0.value + " / " + document.form.vpn_clientlist_netmask_0.value + " combination is invalid");
		document.form.vpn_clientlist_subnet_0.focus();
		document.form.vpn_clientlist_subnet_0.select();
		return false;
	}

// Check for duplicate
	if(item_num >=2){
		for(i=0; i<client_num; i++){
			if(document.form.vpn_clientlist_commonname_0.value.toLowerCase() == document.getElementById("openvpn_clientlist_table").rows[i].cells[0].innerHTML.toLowerCase()
			&& document.form.vpn_clientlist_subnet_0.value == document.getElementById("openvpn_clientlist_table").rows[i].cells[1].innerHTML
			&& document.form.vpn_clientlist_netmask_0.value == document.getElementById("openvpn_clientlist_table").rows[i].cells[2].innerHTML){
				alert('<%tcWebApi_Get("String_Entry","JS_duplicate","s")%>');
				document.form.vpn_clientlist_commonname_0.focus();
				document.form.vpn_clientlist_commonname_0.select();
				return false;
			}
		}
	}

	do_addRow_Group();
}

function do_addRow_Group(){
	addRowAdvanced(document.form.vpn_clientlist_commonname_0 ,1);
	addRowAdvanced(document.form.vpn_clientlist_subnet_0, 0);
	addRowAdvanced(document.form.vpn_clientlist_netmask_0, 0);
	addRowAdvanced(document.form.vpn_clientlist_push_0, 0);
	document.form.vpn_clientlist_push_0.value="0"; //reset selection
	allowed_openvpn_clientlist();
}

function addRowAdvanced(obj, head){
	if(head == 1)
		openvpn_clientlist_array += "<1>";
	else
		openvpn_clientlist_array += ">";

	openvpn_clientlist_array += obj.value;
	obj.value = "";
}

function allowed_openvpn_clientlist(){
	var openvpn_clientlist_row = openvpn_clientlist_array.split('<');
	var code = "";

	code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="openvpn_clientlist_table">';
	if(openvpn_clientlist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;" colspan="6"><%tcWebApi_Get("String_Entry","IPC_VSList_Norule","s")%></td>';
	else{
		for(var i = 1; i < openvpn_clientlist_row.length; i++){
			code +='<tr id="row'+i+'">';
			var openvpn_clientlist_col = openvpn_clientlist_row[i].split('>');
			var wid=[0, 36, 20, 20, 12];
				//skip field[0] as it contains "1" for "Enabled".
			for (var j = 1; j < openvpn_clientlist_col.length; j++){
				if (j == 4)
					code +='<td width="'+wid[j]+'%">'+ ((openvpn_clientlist_col[j] == 1 || openvpn_clientlist_col[j] == 'Yes') ? 'Yes' : 'No') +'</td>';
				else
					code +='<td width="'+wid[j]+'%">'+ openvpn_clientlist_col[j] +'</td>';

			}

			code +='<td width="12%">';
			code +='<input class="remove_btn" onclick="del_openvpnRow(this);" value=""/></td>';
		}
	}

	code +='</table>';
	document.getElementById("openvpn_clientlist_Block").innerHTML = code;
}

function del_openvpnRow(r) {
	var i = r.parentNode.parentNode.rowIndex;
	document.getElementById("openvpn_clientlist_table").deleteRow(i);

	var openvpn_clientlist_value = "";
	var rowLength = document.getElementById("openvpn_clientlist_table").rows.length;
	for(var k = 0; k < rowLength; k += 1) {
		for(var j = 0; j < document.getElementById("openvpn_clientlist_table").rows[k].cells.length - 1; j += 1){
			if(j == 0)
				openvpn_clientlist_value += "<1>";
			else
				openvpn_clientlist_value += ">";

			openvpn_clientlist_value += document.getElementById("openvpn_clientlist_table").rows[k].cells[j].innerHTML;
		}
	}

	openvpn_clientlist_array = openvpn_clientlist_value;
	if(openvpn_clientlist_array == "")
		allowed_openvpn_clientlist();
}

function cancel_Key_panel(auth) {
	if(auth == 'tls') {
		this.FromObject ="0";
		$j("#tlsKey_panel").fadeOut(300);
	}
	else if(auth == 'secret') {
		this.FromObject ="0";
		$j("#staticKey_panel").fadeOut(300);
	}
}

function save_keys(auth) {
	if(auth == 'tls') {
		document.openvpnTLSKeyForm.vpn_crt_server1_ca.value = document.openvpnTLSKeyForm.edit_vpn_crt_server1_ca.value;
		document.openvpnTLSKeyForm.vpn_crt_server1_crt.value = document.openvpnTLSKeyForm.edit_vpn_crt_server1_crt.value;
		document.openvpnTLSKeyForm.vpn_crt_server1_key.value = document.openvpnTLSKeyForm.edit_vpn_crt_server1_key.value;
		document.openvpnTLSKeyForm.vpn_crt_server1_dh.value = document.openvpnTLSKeyForm.edit_vpn_crt_server1_dh.value;
		document.openvpnTLSKeyForm.vpn_crt_server1_static.value = document.openvpnTLSKeyForm.edit_vpn_crt_server1_static.value;
		document.openvpnTLSKeyForm.vpn_crt_server1_crl.value = document.openvpnTLSKeyForm.edit_vpn_crt_server1_crl.value;
		document.openvpnTLSKeyForm.vpn_crt_server1_ca.disabled = false;
		document.openvpnTLSKeyForm.vpn_crt_server1_crt.disabled = false;
		document.openvpnTLSKeyForm.vpn_crt_server1_key.disabled = false;
		document.openvpnTLSKeyForm.vpn_crt_server1_dh.disabled = false;
		document.openvpnTLSKeyForm.vpn_crt_server1_static.disabled = false;
		document.openvpnTLSKeyForm.vpn_crt_server1_crl.disabled = false;
		document.openvpnTLSKeyForm.submit();
		cancel_Key_panel('tls');
	}
	else if(auth == 'secret') {
		document.openvpnStaticKeyForm.vpn_crt_server1_static.value = document.openvpnStaticKeyForm.edit_vpn_crt_server1_static.value;
		document.openvpnStaticKeyForm.vpn_crt_server1_static.disabled = false;
		document.openvpnStaticKeyForm.submit();
		cancel_Key_panel('secret');
	}
}

function cal_panel_block(){
	var blockmarginLeft;
	if(window.innerWidth)
		winWidth = window.innerWidth;
	else if((document.body) && (document.body.clientWidth))
		winWidth = document.body.clientWidth;

	if(document.documentElement  && document.documentElement.clientHeight && document.documentElement.clientWidth){
		winWidth = document.documentElement.clientWidth;
	}

	if(winWidth >1050){
		winPadding = (winWidth - 1050) / 2;
		winWidth = 1105;
		blockmarginLeft= (winWidth * 0.15) + winPadding;
	}
	else if(winWidth <= 1050){
		blockmarginLeft = (winWidth) * 0.15 + document.body.scrollLeft;
	}

	document.getElementById("tlsKey_panel").style.marginLeft = blockmarginLeft + "px";
}
/* Advanced Setting end */

function update_vpn_client_state() {
	$j.ajax({
		url: '/cgi-bin/ajax_openvpn_client_status.xml?hash=' + Math.random().toString(),
		dataType: 'xml',

		error: function(xml) {
			setTimeout("update_vpn_client_state();", 1000);
		},

		success: function(xml) {
			var vpnserverXML = xml.getElementsByTagName("vpnserver");
			var client_status = vpnserverXML[0].firstChild.nodeValue;
			parseOpenVPNClients(client_status);
			openvpnd_connected_status();
			setTimeout("update_vpn_client_state();", 3000);
		}
	});
}

function vpnServerTlsKeysize(_obj) {
	document.form.vpn_server_tls_keysize.value = _obj.value;
	setRadioValue(document.form.vpn_server_tls_keysize_basic, _obj.value);
	setRadioValue(document.form.vpn_server_tls_keysize_adv, _obj.value);
}

function update_cipher() {
	$j("#cipher_hint").css("display", "none");
	var cipher = document.form.vpn_server_cipher.value;
	if(cipher == "default")
		$j("#cipher_hint").css("display", "");
}
function update_digest() {
	$j("#digest_hint").css("display", "none");
	var digest = document.form.vpn_server_digest.value;
	if(digest == "MD5" || digest == "RSA-MD4")
		$j("#digest_hint").css("display", "");
}
function vpnServerClientAccess() {
	var vpn_server_client_access = getRadioValue(document.form.vpn_server_client_access);
	switch(parseInt(vpn_server_client_access)) {
		case 0 :
			setRadioValue(document.form.vpn_server_plan, 1);
			setRadioValue(document.form.vpn_server_rgw, 0);
			setRadioValue(document.form.vpn_server_x_dns, 0);
			setRadioValue(document.form.vpn_server_pdns, 0);
			$j(".client_access_custom").css("display", "none");
			break;
		case 1 :
			setRadioValue(document.form.vpn_server_plan, 1);
			setRadioValue(document.form.vpn_server_rgw, 1);
			setRadioValue(document.form.vpn_server_x_dns, 1);
			setRadioValue(document.form.vpn_server_pdns, 1);
			$j(".client_access_custom").css("display", "none");
			break;
	}
	update_visibility();
}
function updateVpnServerClientAccess() {
	var vpn_server_plan = getRadioValue(document.form.vpn_server_plan);
	var vpn_server_rgw = getRadioValue(document.form.vpn_server_rgw);
	var vpn_server_x_dns = getRadioValue(document.form.vpn_server_x_dns);
	var vpn_server_pdns = getRadioValue(document.form.vpn_server_pdns);
	if(vpn_server_plan == "1" && vpn_server_rgw == "0" && vpn_server_x_dns == "0" && vpn_server_pdns == "0") {
		setRadioValue(document.form.vpn_server_client_access, 0);
		$j(".client_access_custom").css("display", "none");
	}
	else if(vpn_server_plan == "1" && vpn_server_rgw == "1" && vpn_server_x_dns == "1" && vpn_server_pdns == "1") {
		setRadioValue(document.form.vpn_server_client_access, 1);
		$j(".client_access_custom").css("display", "none");
	}
	else {
		setRadioValue(document.form.vpn_server_client_access, 2);
		$j(".client_access_custom").css("display", "");
	}
}
</script>
</head>
<body onload="initial();">
<div id="tlsKey_panel"  class="contentM_qis" style="box-shadow: 3px 3px 10px #000;">
	<!--===================================Beginning of tls Content===========================================-->
	<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
	<form method="post" name="openvpnTLSKeyForm" action="apply.asp" target="hidden_frame">
		<input type="hidden" name="current_page" value="Advanced_VPN_OpenVPN.asp">
		<input type="hidden" name="next_page" value="Advanced_VPN_OpenVPN.asp">
		<input type="hidden" name="modified" value="0">
		<input type="hidden" name="flag" value="background">
		<input type="hidden" name="action_mode" value="apply">
		<input type="hidden" name="action_script" value="saveNvram">
		<input type="hidden" name="action_wait" value="1">
		<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
		<input type="hidden" name="firmver" value="">
		<input type="hidden" name="vpn_crt_server1_ca" value="" disabled>
		<input type="hidden" name="vpn_crt_server1_crt" value="" disabled>
		<input type="hidden" name="vpn_crt_server1_key" value="" disabled>
		<input type="hidden" name="vpn_crt_server1_dh" value="" disabled>
		<input type="hidden" name="vpn_crt_server1_static" value="" disabled>
		<input type="hidden" name="vpn_crt_server1_crl" value="" disabled>
		<input type="hidden" name="formname" value="openvpnTLSKeyForm">
		<tr>
			<div class="description_down"><%tcWebApi_Get("String_Entry","vpn_openvpn_Keys_Cert","s")%></div>
		</tr>
		<tr>
			<div style="margin-left:30px; margin-top:10px;">
				<p><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_Edit1","s")%> <span style="color:#FFCC00;">----- BEGIN xxx ----- </span>/<span style="color:#FFCC00;"> ----- END xxx -----</span> <%tcWebApi_Get("String_Entry","vpn_openvpn_KC_Edit2","s")%></p>
				<p><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_Limit","s")%></p>
			</div>
			<div style="margin:5px;*margin-left:-5px;"><img style="width: 730px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
		</tr>
		<tr>
			<td valign="top">
				<table width="700px" border="0" cellpadding="4" cellspacing="0">
					<tbody>
					<tr>
						<td valign="top">
							<table width="100%" id="page1_tls" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
								<tr>
									<th><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_CA","s")%></th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" name="edit_vpn_crt_server1_ca" cols="65" maxlength="3999"></textarea>
									</td>
								</tr>
								<tr>
									<th><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_SA","s")%></th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" name="edit_vpn_crt_server1_crt" cols="65" maxlength="3999"></textarea>
									</td>
								</tr>
								<tr>
									<th><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_SK","s")%></th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" name="edit_vpn_crt_server1_key" cols="65" maxlength="3999"></textarea>
									</td>
								</tr>
								<tr>
									<th><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_DH","s")%></th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" name="edit_vpn_crt_server1_dh" cols="65" maxlength="3999"></textarea>
									</td>
								</tr>
								<tr>
									<th><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_StaticK","s")%></th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" name="edit_vpn_crt_server1_static" cols="65" maxlength="3999"></textarea>
									</td>
								</tr>
								<tr>
									<th>Certificate Revocation List (Optional)</th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" name="edit_vpn_crt_server1_crl" cols="65" maxlength="3999"></textarea>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					</tbody>
				</table>
				<div style="margin-top:5px;width:100%;text-align:center;">
					<input class="button_gen" type="button" onclick="cancel_Key_panel('tls');" value="<%tcWebApi_Get("String_Entry","CTL_Cancel","s")%>">
					<input class="button_gen" type="button" onclick="save_keys('tls');" value="<%tcWebApi_Get("String_Entry","CTL_onlysave","s")%>">
				</div>
			</td>
		</tr>
	</form>
	</table>
	<!--===================================Ending of tls Content===========================================-->
</div>
<div id="staticKey_panel"   class="contentM_qis" style="box-shadow: 3px 3px 10px #000;">
	<!--===================================Beginning of tls Content===========================================-->
	<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
		<form method="post" name="openvpnStaticKeyForm" action="apply.asp" target="hidden_frame">
		<input type="hidden" name="current_page" value="Advanced_VPN_OpenVPN.asp">
		<input type="hidden" name="next_page" value="Advanced_VPN_OpenVPN.asp">
		<input type="hidden" name="modified" value="0">
		<input type="hidden" name="flag" value="background">
		<input type="hidden" name="action_mode" value="apply">
		<input type="hidden" name="action_script" value="saveNvram">
		<input type="hidden" name="action_wait" value="1">
		<input type="hidden" name="preferred_lang" value="EN">
		<input type="hidden" name="firmver" value="">
		<input type="hidden" name="vpn_crt_server1_static" value="" disabled>
		<input type="hidden" name="formname" value="openvpnStaticKeyForm">
		<tr>
			<div class="description_down"><%tcWebApi_Get("String_Entry","vpn_openvpn_Keys_Cert","s")%></div>
		</tr>
		<tr>
			<div style="margin-left:30px; margin-top:10px;">
				<p><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_Edit1","s")%> <span style="color:#FFCC00;">----- BEGIN xxx ----- </span>/<span style="color:#FFCC00;"> ----- END xxx -----</span> <%tcWebApi_Get("String_Entry","vpn_openvpn_KC_Edit2","s")%></p>
				<p><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_Limit","s")%></p>
			</div>
			<div style="margin:5px;*margin-left:-5px;"><img style="width: 730px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
		</tr>
		<tr>
			<td valign="top">
				<table width="700px" border="0" cellpadding="4" cellspacing="0">
					<tbody>
					<tr>
						<td valign="top">
							<table width="100%" id="page1_static" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
								<tr>
									<th><%tcWebApi_Get("String_Entry","vpn_openvpn_KC_StaticK","s")%></th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" name="edit_vpn_crt_server1_static" cols="65" maxlength="3999"></textarea>
									</td>
								</tr>
							</table>
							<div style="margin-top:5px;width:100%;text-align:center;">
								<input class="button_gen" type="button" onclick="cancel_Key_panel('secret');" value="<%tcWebApi_Get("String_Entry","CTL_Cancel","s")%>">
								<input class="button_gen" type="button" onclick="save_keys('secret');" value="<%tcWebApi_Get("String_Entry","CTL_onlysave","s")%>">
							</div>
						</td>
					</tr>
				</tbody>
				</table>
			</td>
		</tr>
		</form>
	</table>
	<!--===================================Ending of tls Content===========================================-->
</div>
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<div id="openvpn_connected_info" style="display:none;"><pre><% nvram_dump("openvpn_connected",""); %></pre></div>
<form method="post" name="form" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_VPN_OpenVPN.asp">
<input type="hidden" name="next_page" value="Advanced_VPN_OpenVPN.asp">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="10">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="">
<input type="hidden" name="VPNServer_enable" value="<%tcWebApi_Get("VPNControl_Entry", "VPNServer_enable", "s")%>">
<input type="hidden" name="VPNServer_mode" value="<%tcWebApi_Get("VPNControl_Entry", "VPNServer_mode", "s")%>">
<input type="hidden" name="vpn_server_clientlist" value="">
<input type="hidden" name="vpn_serverx_eas" value="<%tcWebApi_Get("OpenVPN_Common", "vpn_serverx_eas", "s")%>">
<input type="hidden" name="vpn_serverx_dns" value="<%tcWebApi_Get("OpenVPN_Common", "vpn_serverx_dns", "s")%>">
<input type="hidden" name="vpn_server_ccd_val" value="">
<input type="hidden" name="vpn_server_tls_keysize" value="<% tcWebApi_Get("OpenVPN_Entry", "tls_keysize", "s") %>">
<input type="hidden" name="formname" value="form">
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
					<td valign="top" >
						<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle" >
							<tbody>
							<tr>
								<td bgcolor="#4D595D" valign="top">
									<div>&nbsp;</div>
									<div class="formfonttitle"><%tcWebApi_Get("String_Entry","BOP_isp_heart_item","s")%> - OpenVPN</div>
									<div id="divSwitchMenu" style="margin-top:-40px;float:right;display:none;">
										<div style="width:173px;height:30px;border-top-left-radius:8px;border-bottom-left-radius:8px;" class="block_filter">
											<a href="Advanced_VPN_PPTP.asp">
												<div class="block_filter_name">PPTP</div>
											</a>
										</div>
										<div style="width:172px;height:30px;margin:-32px 0px 0px 173px;border-top-right-radius:8px;border-bottom-right-radius:8px;" class="block_filter_pressed">
											<div style="text-align:center;padding-top:5px;color:#93A9B1;font-size:14px">OpenVPN</div>
										</div>
									</div>
									<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
									<div id="privateIP_notes" class="formfontdesc" style="display:none;color:#FFCC00;"></div>
									<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
										<thead>
										<tr>
											<td colspan="2"><%tcWebApi_Get("String_Entry","t2BC","s")%></td>
										</tr>
										</thead>
										<tr>
											<th><%tcWebApi_Get("String_Entry","vpn_openvpn_enable","s")%></th>
											<td>
												<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_VPNServer_enable"></div>
												<script type="text/javascript">
													$j('#radio_VPNServer_enable').iphoneSwitch(open_vpn_enable,
													function(){
														document.form.VPNServer_enable.value = "1";
														formShowAndHide(1, "openvpn");
													},
													function(){
														document.form.VPNServer_enable.value = "0";
														formShowAndHide(0, "openvpn");
													}
													);
												</script>
											</td>
										</tr>
										<tr id="trVPNServerMode">
											<th><%tcWebApi_Get("String_Entry","vpn_Adv","s")%></th>
											<td>
												<select id="selSwitchMode" onchange="switchMode(this.options[this.selectedIndex].value)" class="input_option">
													<option value="1" selected><%tcWebApi_Get("String_Entry","menu5_1_1","s")%></option>
													<option value="2"><%tcWebApi_Get("String_Entry","menu5","s")%></option>
												</select>
											</td>
										</tr>
										<tr id="trRSAEncryptionBasic">
											<th>RSA Encryption<!--untranslated--></th>
											<td>
												<input type="radio" name="vpn_server_tls_keysize_basic" id="vpn_server_tls_keysize_basic_0" class="input" value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "tls_keysize", "0", "checked"); %> onchange="vpnServerTlsKeysize(this);">
												<label for='vpn_server_tls_keysize_basic_0'>1024 bit<!--untranslated--></label>
												<input type="radio" name="vpn_server_tls_keysize_basic" id="vpn_server_tls_keysize_basic_1" class="input" value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "tls_keysize", "1", "checked"); %> onchange="vpnServerTlsKeysize(this);">
												<label for='vpn_server_tls_keysize_basic_1'>2048 bit<!--untranslated--></label>
											</td>
										</tr>
										<tr id="trClientWillUseVPNToAccess">
											<th>Client will use VPN to access<!--untranslated--></th>
											<td>
												<input type="radio" name="vpn_server_client_access" id="vpn_server_client_access_local" class="input" value="0" onchange="vpnServerClientAccess();">
												<label for="vpn_server_client_access_local">Local network only<!--untranslated--></label>
												<input type="radio" name="vpn_server_client_access" id="vpn_server_client_access_both" class="input" value="1" onchange="vpnServerClientAccess();">
												<label for="vpn_server_client_access_both">Internet and local network<!--untranslated--></label>
												<input type="radio" name="vpn_server_client_access" id="vpn_server_client_access_custom" class="input client_access_custom" value="2" onchange="vpnServerClientAccess();">
												<label for="vpn_server_client_access_custom" class="client_access_custom"><%tcWebApi_Get("String_Entry","Custom","s")%></label>
											</td>
										</tr>
										<tr id="openvpn_export" style="display:none;">
											<th><%tcWebApi_Get("String_Entry","vpn_export_ovpnfile","s")%></th>
											<td>
												<div id="export_div">
													<input id="exportToLocal" class="button_gen" type="button" value="<%tcWebApi_Get("String_Entry","btn_Export","s")%>" />
													<input id="exportViaEmail" class="button_gen" type="button" value="via Email" style="display:none;"/><!-- Viz hide it temp 2014.06 -->
												</div>
												<script type="text/javascript">
													document.getElementById("exportToLocal").onclick = function(){
														location.href = 'client.ovpn';
													}
													document.getElementById("exportViaEmail").onclick = function(){
														showMailPanel();
													}
												</script>
              									<div id="openvpn_initial" style="display:none;margin-left:5px;">
              										<span>
              											 <%tcWebApi_Get("String_Entry","vpn_openvpn_init","s")%>
              											 <img src="/images/InternetScan.gif" />
              										</span>
              									</div>
              									<div id="openvpn_error_message" style="display:none;margin-left:5px;"></div>
            								</td>
          								</tr>
									</table>
									<div id="OpenVPN_setting" style="display:none;margin-top:8px;">
										<div class="formfontdesc">
											<%tcWebApi_Get("String_Entry","vpn_openvpn_desc1","s")%>&nbsp;<%tcWebApi_Get("String_Entry","vpn_openvpn_desc3","s")%>&nbsp;<%tcWebApi_Get("String_Entry","vpn_openvpn_desc2","s")%>
											<br />
											<ol>
												<li><a id="faq_windows" href="https://www.asus.com/support/FAQ/1004469/" target="_blank" style="text-decoration:underline;">Windows</a></li>
												<li><a id="faq_macOS" href="https://www.asus.com/support/FAQ/1033575/" target="_blank" style="text-decoration:underline;">Mac OS</a></li>
												<li><a id="faq_iPhone" href="https://www.asus.com/support/FAQ/1033574/" target="_blank" style="text-decoration:underline;">iPhone/iPad</a></li>
												<li><a id="faq_android" href="https://www.asus.com/support/FAQ/1033572/" target="_blank" style="text-decoration:underline;">Android</a></li>
											</ol>
										</div>
										<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" style="margin-top:8px;">
											<thead>
											<tr>
												<td colspan="4"><%tcWebApi_Get("String_Entry","Username_Pwd","s")%>&nbsp;(<%tcWebApi_Get("String_Entry","List_limit","s")%>&nbsp;16)</td>
											</tr>
											</thead>
											<tr>
												<th><%tcWebApi_Get("String_Entry","PPPC_x_WANLink_in","s")%></th>
												<th><%tcWebApi_Get("String_Entry","HSDPAC_Username_in","s")%></th>
												<th><%tcWebApi_Get("String_Entry","HSDPAC_Password_in","s")%></th>
												<th><%tcWebApi_Get("String_Entry","list_add_delete","s")%></th>
											</tr>
											<tr>
												<td width="15%" style="text-align:center;">-
												</td>
												<td width="35%">
													<input type="text" class="input_25_table" maxlength="64" name="vpn_server_clientlist_username" onKeyPress="return validator.isString(this, event)" autocorrect="off" autocapitalize="off">
												</td>
												<td width="35%">
													<input type="text" class="input_25_table" maxlength="64" name="vpn_server_clientlist_password" onKeyPress="return validator.isString(this, event)" autocorrect="off" autocapitalize="off">
												</td>
												<td width="15%">
													<div><input type="button" class="add_btn" onClick="addRow_Group(16);" value=""></div>
												</td>
											</tr>
										</table>

										<div id="openvpnd_clientlist_Block"></div>
									</div>

									<div id="divAdvanced" style="display:none;margin-top:8px;">
										<div class="formfontdesc">
											<p><%tcWebApi_Get("String_Entry","vpn_openvpn_desc3","s")%></p>
											<p><%tcWebApi_Get("String_Entry","vpn_openvpn_hint1","s")%></p>
											<p><%tcWebApi_Get("String_Entry","vpn_openvpn_hint2","s")%></p>
											<p>Before changing any value in advanced settings, please check the openVPN client software ability.</p>
										</div>
										<!-- Advanced setting table start-->
										<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable" style="margin-top:8px;">
											<thead>
											<tr>
												<td colspan="2"><%tcWebApi_Get("String_Entry","menu5","s")%></td>
											</tr>
											</thead>
											<tr style="display:none;">
												<th>Select server instance</th>
												<td>
													<select name="vpn_server_unit" class="input_option" onChange="change_vpn_unit(this.value);">
														<option value="1" <% tcWebApi_MatchThenWrite("WebCurSet_Entry","openvpn_id","21","selected") %> >Server 1</option>
														<option value="2" <% tcWebApi_MatchThenWrite("WebCurSet_Entry","openvpn_id","22","selected") %> >Server 2</option>
													</select>
												</td>
											</tr>
											<tr style="display:none;">
												<th>Service state</th>
												<td>
													<div class="left" style="width:94px; float:left; cursor:pointer;" id="radio_service_enable"></div>
													<script type="text/javascript">
														if (openvpn_unit == '1')
															var service_state_advanced = (<% sysinfo("pid.vpnserver1") %> > 0);
														else if (openvpn_unit == '2')
															var service_state_advanced = (<% sysinfo("pid.vpnserver2") %> > 0);
														else
															var service_state_advanced = false;

														$j('#radio_service_enable').iphoneSwitch(service_state_advanced,
															function() {
																document.form.action_script.value = "start_vpnserver"+openvpn_unit;
																parent.showLoading();
																document.form.submit();
															},
															function() {
																document.form.action_script.value = "stop_vpnserver"+openvpn_unit;
																parent.showLoading();
																document.form.submit();
															}
														);
													</script>
													<span>Warning: any unsaved change will be lost.</span>
												</td>
											</tr>
											<tr style="display:none;">
												<th>Start with WAN</th>
												<td>
													<input type="radio" name="vpn_server_x_eas" class="input" value="1"><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_x_eas" class="input" value="0"><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,4);"><%tcWebApi_Get("String_Entry","vpn_openvpn_interface","s")%></a></th>
												<td>
													<select name="vpn_server_if" class="input_option" onChange="update_visibility();">
														<option value="tap" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "iface","tap","selected") %> >TAP</option>
														<option value="tun" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "iface","tun","selected") %> >TUN</option>
													</select>
												</td>
											</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,5);"><%tcWebApi_Get("String_Entry","IPC_VServerProto_in","s")%></a></th>
												<td>
													<select name="vpn_server_proto" class="input_option">
														<option value="tcp-server" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "proto","tcp-server","selected") %> >TCP</option>
														<option value="udp" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "proto","udp","selected") %> >UDP</option>
													</select>
												</td>
											</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,6);"><%tcWebApi_Get("String_Entry","WA11a_ExAuthDBPortNumber_in","s")%></a></th>
												<td>
													<input type="text" maxlength="5" class="input_6_table" name="vpn_server_port" onKeyPress="return validator.isNumber(this,event);" value="<% tcWebApi_Get("OpenVPN_Entry", "port", "s") %>" autocorrect="off" autocapitalize="off">
													<span style="color:#FC0">(<%tcWebApi_Get("String_Entry","Setting_factorydefault_value","s")%> : 1194)</span>
												</td>
											</tr>

											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,15);"><%tcWebApi_Get("String_Entry","vpn_openvpn_ResponseDNS","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_x_dns" class="input" value="1" onclick="update_visibility();"><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_x_dns" class="input" value="0" onclick="update_visibility();"><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
											<tr id="server_pdns">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,16);"><%tcWebApi_Get("String_Entry","vpn_openvpn_AdvDNS","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_pdns" class="input" value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "push_dns", "1", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_pdns" class="input" value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "push_dns", "0", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,17);"><%tcWebApi_Get("String_Entry","vpn_openvpn_Encrypt","s")%></a></th>
												<td>
													<select name="vpn_server_cipher" class="input_option"></select>
													<span id="cipher_hint" style="color:#FC0">(Default : BF-CBC)</span>
												</td>
											</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,26);">HMAC Authentication<!--untranslated--></a></th>
												<td>
													<select name="vpn_server_digest" class="input_option" onChange="update_digest();"></select>
													<span id="digest_hint" style="color:#FC0">(Not recommended)<!--untranslated--></span>
												</td>
											</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,18);"><%tcWebApi_Get("String_Entry","vpn_openvpn_Compression","s")%></a></th>
												<td>
													<select name="vpn_server_comp" class="input_option">
														<option value="-1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "comp","-1","selected"); %> ><%tcWebApi_Get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
														<option value="no" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "comp","no","selected"); %> ><%tcWebApi_Get("String_Entry","wl_securitylevel_0","s")%></option>
														<option value="yes" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "comp","yes","selected"); %> ><%tcWebApi_Get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
														<option value="adaptive" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "comp","adaptive","selected"); %> ><%tcWebApi_Get("String_Entry","Adaptive","s")%></option>
													</select>
												</td>
											</tr>

											<!--rm 2017/07/21 tr style="display:none">
												<th><%tcWebApi_Get("String_Entry","menu5_5","s")%></th>
												<td>
													<select name="vpn_server_firewall" class="input_option">
														<option value="auto" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "firewall","auto","selected"); %> ><%tcWebApi_Get("String_Entry","Auto","s")%></option>
														<option value="external" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "firewall","external","selected"); %> ><%tcWebApi_Get("String_Entry","External","s")%></option>
														<option value="custom" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "firewall","custom","selected"); %> ><%tcWebApi_Get("String_Entry","Custom","s")%></option>
													</select>
												</td>
											</tr-->

											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,9);"><%tcWebApi_Get("String_Entry","vpn_openvpn_AuthOnly","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_igncrt" class="input" value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "igncrt", "1", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_igncrt" class="input" value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "igncrt", "0", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
													<span id="Hint_fixed_tls_crypto" style="display:none;">Authorization Mode fixes on TLS</span><!--untranslated-->
												</td>
											</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,7);"><%tcWebApi_Get("String_Entry","vpn_openvpn_Auth","s")%></a></th>
												<td>
													<select name="vpn_server_crypt" class="input_option" onChange="update_visibility();">
														<option value="tls" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "crypt","tls","selected"); %> >TLS</option>
														<option value="secret" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "crypt","secret","selected"); %> >Static Key</option>
														<!--option value="custom" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "crypt","custom","selected"); %> >Custom</option-->
													</select>
													<span id="Fixed_tls_crypto" style="color:#FFFFFF;display:none;">TLS</span>
													<span id="server_tls_crypto_text" onclick="set_Keys('tls');" style="text-decoration:underline;cursor:pointer;"><%tcWebApi_Get("String_Entry","vpn_openvpn_ModifyKeys","s")%></span>
													<span id="server_static_crypto_text" onclick="set_Keys('secret');" style="text-decoration:underline;cursor:pointer;"><%tcWebApi_Get("String_Entry","vpn_openvpn_ModifyKeys","s")%></span>
													<!-- rm 2017/07/21 span id="server_custom_crypto_text"><%tcWebApi_Get("String_Entry","vpn_openvpn_MustManual","s")%></span-->
												</td>
											</tr>
											<tr>
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,8);">RSA Encryption</a><!--untranslated--></th>
												<td>
													<input type="radio" name="vpn_server_tls_keysize_adv" id="vpn_server_tls_keysize_adv_0" class="input" value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "tls_keysize", "0", "checked"); %> onchange="vpnServerTlsKeysize(this);">
													<label for='vpn_server_tls_keysize_adv_0'>1024 bit<!--untranslated--></label>
													<input type="radio" name="vpn_server_tls_keysize_adv" id="vpn_server_tls_keysize_adv_1" class="input" value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "tls_keysize", "1", "checked"); %> onchange="vpnServerTlsKeysize(this);">
													<label for='vpn_server_tls_keysize_adv_1'>2048 bit<!--untranslated--></label>
												</td>
											</tr>
											
											<tr id="server_authhmac">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,10);"><%tcWebApi_Get("String_Entry","vpn_openvpn_AuthHMAC","s")%></a></th>
												<td>
													<select name="vpn_server_hmac" class="input_option">
														<option value="-1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "hmac","-1","selected"); %> ><%tcWebApi_Get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
														<option value="2" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "hmac","2","selected"); %> >Bi-directional</option>
														<option value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "hmac","0","selected"); %> >Incoming (0)</option>
														<option value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "hmac","1","selected"); %> >Incoming (1)</option>
													</select>
													<span style="color:#FC0">(TLS-Auth)</span>
												</td>
											</tr>
											<tr id="server_snnm">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,11);"><%tcWebApi_Get("String_Entry","vpn_openvpn_SubnetMsak","s")%></a></th>
												<td>
													<input type="text" maxlength="15" class="input_15_table" name="vpn_server_sn" onkeypress="return validator.isIPAddr(this, event);" value="<% tcWebApi_Get("OpenVPN_Entry", "subnet", "s") %>" autocorrect="off" autocapitalize="off">
													<input type="text" maxlength="15" class="input_15_table" name="vpn_server_nm" onkeypress="return validator.isIPAddr(this, event);" value="<% tcWebApi_Get("OpenVPN_Entry", "netmask", "s") %>" autocorrect="off" autocapitalize="off">
												</td>
											</tr>
											<tr id="server_dhcp">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,13);"><%tcWebApi_Get("String_Entry","vpn_openvpn_dhcp","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_dhcp" class="input" value="1" onclick="update_visibility();" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "dhcp", "1", "checked") %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_dhcp" class="input" value="0" onclick="update_visibility();" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "dhcp", "0", "checked") %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
											<tr id="server_range">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,14);"><%tcWebApi_Get("String_Entry","vpn_openvpn_ClientPool","s")%></a></th>
												<td>
													<input type="text" maxlength="15" class="input_15_table" name="vpn_server_r1" onkeypress="return validator.isIPAddr(this, event);" value="<% tcWebApi_Get("OpenVPN_Entry", "pool_start", "s") %>" autocorrect="off" autocapitalize="off">
													<input type="text" maxlength="15" class="input_15_table" name="vpn_server_r2" onkeypress="return validator.isIPAddr(this, event);" value="<% tcWebApi_Get("OpenVPN_Entry", "pool_end", "s") %>" autocorrect="off" autocapitalize="off">
												</td>
											</tr>
											<tr id="server_local">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,12);"><%tcWebApi_Get("String_Entry","vpn_openvpn_LocalRemote_IP","s")%></a></th>
												<td>
													<input type="text" maxlength="15" class="input_15_table" name="vpn_server_local" onkeypress="return validator.isIPAddr(this, event);" value="<% tcWebApi_Get("OpenVPN_Entry", "local", "s") %>" autocorrect="off" autocapitalize="off">
													<input type="text" maxlength="15" class="input_15_table" name="vpn_server_remote" onkeypress="return validator.isIPAddr(this, event);" value="<% tcWebApi_Get("OpenVPN_Entry", "remote", "s") %>" autocorrect="off" autocapitalize="off">
												</td>
											</tr>
											<!-- tr> rm 2017/07/21
												<th><%tcWebApi_Get("String_Entry","vpn_openvpn_PollInterval","s")%></th>
												<td>
													<input type="text" maxlength="4" class="input_6_table" name="vpn_server_poll" onKeyPress="return validator.isNumber(this,event);" value="<% tcWebApi_Get("OpenVPN_Entry", "poll", "s") %>" autocorrect="off" autocapitalize="off"> <%tcWebApi_Get("String_Entry","Minute","s")%>
													<span style="color:#FC0">(<%tcWebApi_Get("String_Entry","zero_disable","s")%>)</span>
												</td>
											</tr-->
											<tr id="server_plan">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,2);"><%tcWebApi_Get("String_Entry","vpn_openvpn_PushLAN","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_plan" class="input" value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "push_lan", "1", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_plan" class="input" value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "push_lan", "0", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
											<tr id="server_rgw">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,3);"><%tcWebApi_Get("String_Entry","vpn_openvpn_RedirectInternet","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_rgw" class="input" value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "redirect_gateway", "1", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_rgw" class="input" value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "redirect_gateway", "0", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
											<tr id="server_reneg">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,19);"><%tcWebApi_Get("String_Entry","vpn_openvpn_TLSTime","s")%></a></th>
												<td>
													<input type="text" maxlength="5" class="input_6_table" name="vpn_server_reneg" value="<% tcWebApi_Get("OpenVPN_Entry","reneg","s") %>" autocorrect="off" autocapitalize="off"> <%tcWebApi_Get("String_Entry","Second","s")%>
													<span style="color:#FC0">(<%tcWebApi_Get("String_Entry","Setting_factorydefault_value","s")%> : -1)</span>
												</td>
											</tr>
											<tr id="server_ccd">
												<th><%tcWebApi_Get("String_Entry","vpn_openvpn_SpecificOpt","s")%></th>
												<td>
													<input type="radio" name="vpn_server_ccd" class="input" value="1" onclick="update_visibility();" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "ccd", "1", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_ccd" class="input" value="0" onclick="update_visibility();" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "ccd", "0", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
											<tr id="server_c2c">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,20);"><%tcWebApi_Get("String_Entry","vpn_openvpn_ClientMutual","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_c2c" class="input" value="1" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "c2c", "1", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_c2c" class="input" value="0" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "c2c", "0", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
											</tr>
											<tr id="server_ccd_excl">
												<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(32,21);"><%tcWebApi_Get("String_Entry","vpn_openvpn_ClientSpecified","s")%></a></th>
												<td>
													<input type="radio" name="vpn_server_ccd_excl" class="input" value="1" onclick="update_visibility();" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "ccd_excl", "1", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_Yes","s")%>
													<input type="radio" name="vpn_server_ccd_excl" class="input" value="0" onclick="update_visibility();" <% tcWebApi_MatchThenWrite("OpenVPN_Entry", "ccd_excl", "0", "checked"); %>><%tcWebApi_Get("String_Entry","checkbox_No","s")%>
												</td>
		 									</tr>
										</table>
										<!-- Advanced setting table end-->
										<!-- Allowed Clients setting table start-->
										<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" id="openvpn_client_table">
											<thead>
											<tr>
												<td colspan="5">Allowed Clients&nbsp;(<%tcWebApi_Get("String_Entry","List_limit","s")%>&nbsp;16)</td>
											</tr>
											</thead>
											<tr>
												<th width="36%"><a id="allowed_client_name" class="hintstyle" href="javascript:void(0);" onClick="openHint(32,22);">Common Name(CN)</a></th>	<!--<%tcWebApi_Get("String_Entry","HSDPAC_Username_in","s")%>-->
												<th width="20%"><a id="allowed_client_name" class="hintstyle" href="javascript:void(0);" onClick="openHint(32,23);"><%tcWebApi_Get("String_Entry","Subnet","s")%></a></th>
												<th width="20%"><a id="allowed_client_name" class="hintstyle" href="javascript:void(0);" onClick="openHint(32,24);">Mask</a></th>	<!-- Untranslated -->
												<th width="12%"><a id="allowed_client_name" class="hintstyle" href="javascript:void(0);" onClick="openHint(32,25);"><%tcWebApi_Get("String_Entry","Push","s")%></a></th>
												<th width="12%"><%tcWebApi_Get("String_Entry","list_add_delete","s")%></th>
											</tr>
											<tr>
												<div id="VPNClientList_Block_PC" class="VPNClientList_Block_PC"></div>
												<td width="36%">
								 					<input type="text" class="input_25_table" maxlength="25" name="vpn_clientlist_commonname_0" autocorrect="off" autocapitalize="off">
								 				</td>
												<td width="20%">
								 					<input type="text" class="input_15_table" maxlength="15" name="vpn_clientlist_subnet_0"  onkeypress="return validator.isIPAddr(this, event);" autocorrect="off" autocapitalize="off">
								 				</td>
												<td width="20%">
								 					<input type="text" class="input_15_table" maxlength="15" name="vpn_clientlist_netmask_0"  onkeypress="return validator.isIPAddr(this, event);" autocorrect="off" autocapitalize="off">
								 				</td>
								 				<td width="12%">
													<select name="vpn_clientlist_push_0" class="input_option">
														<option value="0" selected>No</option>
														<option value="1">Yes</option>
													</select>
												</td>
								 				<td width="12%">
													<input class="add_btn" type="button" onClick="addRow_Group_Advanced(16);" name="vpn_clientlist2" value="">
								 				</td>
											</tr>
										</table>
										<div id="openvpn_clientlist_Block"></div>
										<!-- Allowed Clients setting table end-->

										<!-- Custom setting table start-->
										<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
											<thead>
											<tr>
												<td><%tcWebApi_Get("String_Entry","vpn_openvpn_CustomConf","s")%></td>
											</tr>
											</thead>

											<tr>
												<td>
													<textarea rows="8" class="textarea_ssh_table" name="vpn_server_custom" cols="55" maxlength="4095"><% tcWebApi_clean_get("OpenVPN_Entry", "custom"); %></textarea>
												</td>
											</tr>
										</table>
										<!-- Custom setting table end-->
									</div>

									<div id="divApply" class="apply_gen" style="display:none;">
										<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_Get("String_Entry","CTL_apply","s")%>"/>
									</div>
								</td>
							</tr>
							</tbody>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
</form>

<div id="mailSendPanelContainer" class="hiddenPanelContainer">
	<div class="hiddenPanel">
		<form method="post" name="mailSendForm" action="start_apply.asp" target="hidden_frame">
			<input type="hidden" name="action_mode" value="apply">
			<input type="hidden" name="action_script" value="restart_sendmail">
			<input type="hidden" name="action_wait" value="5">
			<input type="hidden" name="flag" value="background">
			<input type="hidden" name="PM_MAIL_SUBJECT" value="My ovpn file">
			<input type="hidden" name="PM_MAIL_FILE" value="/www/client.ovpn">
			<input type="hidden" name="PM_LETTER_CONTENT" value="Here is the ovpn file.">

			<div class="panelTableTitle">
				<div>Send</div>
				<img style="width: 100%; height: 2px;" src="/images/New_ui/export/line_export.png">
			</div>

			<table border=0 align="center" cellpadding="5" cellspacing="0" class="FormTable panelTable">
				<tr>
					<th>PM_MAIL_TARGET</th>
					<td valign="top">
						<input type="text" class="input_32_table" name="PM_MAIL_TARGET" value="" autocorrect="off" autocapitalize="off">
					</td>
				</tr>
			</table>

			<div class="panelSubmiter">
				<input id="mailSendPannelCancel" class="button_gen" type="button" value="<%tcWebApi_Get("String_Entry","CTL_Cancel","s")%>">
				<input id="mailSendPannelSubmiter" class="button_gen" type="button" value="Send">
				<img id="mailSendLoadingIcon" style="margin-left:5px;display:none;" src="/images/InternetScan.gif">
				<script>
					document.getElementById("mailSendPannelCancel").onclick = function(){
						$j("#mailSendPanelContainer").fadeOut(300);
					}
					document.getElementById("mailSendPannelSubmiter").onclick = function(){
						// ToDo: validator.
						$j("#mailSendLoadingIcon").fadeIn(200);
						document.mailSendForm.submit();
						setTimeout(function(){
							document.mailSendForm.PM_MAIL_TARGET.value = "";
							$j("#mailSendLoadingIcon").fadeOut(200);
							$j("#mailSendPanelContainer").fadeOut(300);
						}, document.mailSendForm.action_wait.value*1000);
					}
				</script>
			</div>
		</form>
	</div>
</div>

<div id="mailConfigPanelContainer" class="hiddenPanelContainer">
	<div class="hiddenPanel">
		<form method="post" name="mailConfigForm" action="start_apply.asp" target="hidden_frame">
			<input type="hidden" name="action_mode" value="apply">
			<input type="hidden" name="action_script" value="saveNvram">
			<input type="hidden" name="action_wait" value="3">
			<input type="hidden" name="PM_SMTP_SERVER" value="<% tcWebApi_Get("PushMail_Entry", "PM_SMTP_SERVER","s") %>">
			<input type="hidden" name="PM_SMTP_PORT" value="<% tcWebApi_Get("PushMail_Entry", "PM_SMTP_PORT","s") %>">
			<input type="hidden" name="PM_SMTP_AUTH_USER" value="<% tcWebApi_Get("PushMail_Entry", "PM_SMTP_AUTH_USER","s") %>">
			<input type="hidden" name="PM_SMTP_AUTH_PASS" value="<% tcWebApi_Get("PushMail_Entry", "PM_SMTP_AUTH_PASS","s") %>">
			<input type="hidden" name="PM_MY_NAME" value="<% tcWebApi_Get("PushMail_Entry", "PM_MY_NAME","s") %>">
			<input type="hidden" name="PM_MY_EMAIL" value="<% tcWebApi_Get("PushMail_Entry", "PM_MY_EMAIL","s") %>">

			<div class="panelTableTitle">
				<div>Setup mail server</div>
				<img style="width: 100%; height: 2px;" src="/images/New_ui/export/line_export.png">
			</div>

			<table border=0 align="center" cellpadding="5" cellspacing="0" class="FormTable panelTable">
				<tr>
					<th>PM_SMTP_SERVER</th>
					<td valign="top">
				    <select style="width:350px;" name="PM_SMTP_SERVER_TMP" class="input_option">
							<option value="smtp.gmail.com" <% tcWebApi_MatchThenWrite("PushMail_Entry", "PM_SMTP_SERVER", "smtp.gmail.com", "selected") %>>Google Gmail</option>
				    </select>
						<script>
							var smtpList = new Array()
							smtpList = [
								{smtpServer: "smtp.gmail.com", smtpPort: "587", smtpDomain: "gmail.com"},
								{end: 0}
							];

							document.mailConfigForm.PM_SMTP_SERVER_TMP.onchange = function(){
								document.mailConfigForm.PM_SMTP_PORT_TMP.value = smtpList[this.selectedIndex].smtpPort;
								document.mailConfigForm.PM_SMTP_AUTH_USER_TMP.value = "";
								document.mailConfigForm.PM_SMTP_AUTH_PASS_TMP.value = "";
								document.mailConfigForm.PM_MY_NAME_TMP.value = "";
								document.mailConfigForm.PM_MY_EMAIL_TMP.value = "";
							}
						</script>
					</td>
				</tr>
				<input type="hidden" name="PM_SMTP_PORT_TMP" value="<% tcWebApi_Get("PushMail_Entry", "PM_SMTP_PORT","s") %>">
				<tr>
					<th>PM_SMTP_AUTH_USER</th>
					<td valign="top">
						<input type="text" class="input_32_table" name="PM_SMTP_AUTH_USER_TMP" value="<% tcWebApi_Get("PushMail_Entry", "PM_SMTP_AUTH_USER","s") %>" autocorrect="off" autocapitalize="off">
						<script>
							document.mailConfigForm.PM_SMTP_AUTH_USER_TMP.onkeyup = function(){
								document.mailConfigForm.PM_MY_NAME_TMP.value = this.value;
								document.mailConfigForm.PM_MY_EMAIL_TMP.value = this.value + "@" + smtpList[document.mailConfigForm.PM_SMTP_SERVER_TMP.selectedIndex].smtpDomain;
							}
						</script>
					</td>
				</tr>
				<tr>
					<th>PM_SMTP_AUTH_PASS</th>
					<td valign="top">
						<input type="password" class="input_32_table" name="PM_SMTP_AUTH_PASS_TMP" maxlength="100" value="" autocorrect="off" autocapitalize="off">
					</td>
				</tr>
				<tr>
					<th>PM_MY_NAME (Optional)</th>
					<td valign="top">
						<input type="text" class="input_32_table" name="PM_MY_NAME_TMP" value="<% tcWebApi_Get("PushMail_Entry", "PM_MY_NAME","s") %>" autocorrect="off" autocapitalize="off">
					</td>
				</tr>
				<tr>
					<th>PM_MY_EMAIL (Optional)</th>
					<td valign="top">
						<input type="text" class="input_32_table" name="PM_MY_EMAIL_TMP" value="<% tcWebApi_Get("PushMail_Entry", "PM_MY_EMAIL","s") %>" autocorrect="off" autocapitalize="off">
					</td>
				</tr>
			</table>

			<div class="panelSubmiter">
				<input id="mailConfigPannelCancel" class="button_gen" type="button" value="<%tcWebApi_Get("String_Entry","CTL_Cancel","s")%>"/>
				<input id="mailConfigPannelSubmiter" class="button_gen" type="button" value="<%tcWebApi_Get("String_Entry","CTL_onlysave","s")%>"/>
				<img id="mailConfigLoadingIcon" style="margin-left:5px;display:none;" src="/images/InternetScan.gif"/>
				<script>
					document.getElementById("mailConfigPannelCancel").onclick = function(){
						$j("#mailConfigPanelContainer").fadeOut(300);
					}
					document.getElementById("mailConfigPannelSubmiter").onclick = function(){
						// ToDo: validator.

						document.mailConfigForm.PM_SMTP_SERVER.value = document.mailConfigForm.PM_SMTP_SERVER_TMP.value;
						if (document.mailConfigForm.PM_SMTP_PORT_TMP.value == "")
							document.mailConfigForm.PM_SMTP_PORT.value = smtpList[0].smtpPort;
						else
							document.mailConfigForm.PM_SMTP_PORT.value = document.mailConfigForm.PM_SMTP_PORT_TMP.value;
						document.mailConfigForm.PM_SMTP_AUTH_USER.value = document.mailConfigForm.PM_SMTP_AUTH_USER_TMP.value;
						document.mailConfigForm.PM_SMTP_AUTH_PASS.value = document.mailConfigForm.PM_SMTP_AUTH_PASS_TMP.value;
						document.mailConfigForm.PM_MY_NAME.value = document.mailConfigForm.PM_MY_NAME_TMP.value;
						document.mailConfigForm.PM_MY_EMAIL.value = document.mailConfigForm.PM_MY_EMAIL_TMP.value;

						$j("#mailConfigLoadingIcon").fadeIn(200);
						document.mailConfigForm.submit();
						setTimeout(function(){
							$j("#mailConfigLoadingIcon").fadeOut(200);
							showMailPanel();
						}, document.mailConfigForm.action_wait.value*1000);
					}
				</script>
			</div>
		</form>
	</div>
</div>
<div id="footer"></div>
</body>
</html>
