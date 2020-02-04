<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_mer_cfg_tmp.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<script type="text/Javascript" src="/state.js"></script>
<script type="text/Javascript" src="/general.js"></script>
<script type="text/Javascript" src="/form.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<style>
span{
border:0px solid #FFFFFF;
color:#FFFFFF;
font-size:14px;
font-family:Arial, Helvetica, sans-serif;
/*width:27px;*/
text-align:right;
margin:0px auto;
ime-mode:disabled;
}
</style>
<script type="text/javascript">
var vpi_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vpi","s") %>";
var vci_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vci","s") %>";
var encap_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_encap","s") %>";
var vlanid_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vlanid","s") %>";
var ispname_str = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_ispname","s") %>";
var country_str = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_country","s") %>";
var transfer_mode = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","s") %>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";
var encap_str = "LLC";
if (encap_val == "1") encap_str = "VC-Mux";
var iptv_atm_pvc_str = "";
var iptv_ptm_pvc_str = "";
var dsltmp_cfg_iptv_pvclist = decodeURIComponent('<%tcWebApi_char_to_ascii("GUITemp_Entry0","dsltmp_cfg_iptv_pvclist", "s")%>');															
var iptv_num_pvc_val = "<% tcWebApi_staticGet("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","s") %>";
var mac_addr_2g = "<%tcWebApi_get("WLan_Common","wl0_MacAddress","s")%>";
var mac_addr_last_3bytes = "\"" + mac_addr_2g.substring(9, 11) + mac_addr_2g.substring(12, 14) + mac_addr_2g.substring(15, 17) + "\"";
var model_name = "<%tcWebApi_get("SysInfo_Entry","ProductName","s")%>";
//var ISP_DHCP_OPT_List =[["0", "Switzerland", "Swisscom", "100008","0001","ASUS"], ["1", "Taiwan", "HiNet (0, 33, PPPoE)", "100009","0002","ASUS"], ["2", "USA", "AT&T (8, 35, MER)", "100010","0003","ASUS"]];
var ISP_DHCP_OPT_List =[<% get_isp_dhcp_opt_list() %>];

//udpate iptv information
if (iptv_num_pvc_val != "0" && iptv_num_pvc_val != "") {
	var dsltmp_cfg_iptv_pvclist_row;
	var dsltmp_cfg_iptv_pvclist_col;
	var i, j;

	iptv_atm_pvc_str = iptv_num_pvc_val + " IPTV PVC(s)";
	iptv_ptm_pvc_str = iptv_num_pvc_val + " IPTV service(s)";

	//<vpi>vci>proto>encap>vlanid
	if(dsltmp_cfg_iptv_pvclist.charAt(0) == "<")    //rempve the 1st "<"
		dsltmp_cfg_iptv_pvclist = dsltmp_cfg_iptv_pvclist.substr(1,dsltmp_cfg_iptv_pvclist.length);
	dsltmp_cfg_iptv_pvclist_row = dsltmp_cfg_iptv_pvclist.split('<');

	iptv_atm_pvc_str += ": ";
	iptv_ptm_pvc_str += ": ";
	for(i=0; i<dsltmp_cfg_iptv_pvclist_row.length; i++) {
		if(i > 0) {
			iptv_atm_pvc_str += " + ";
			iptv_ptm_pvc_str += " + ";
		}
		dsltmp_cfg_iptv_pvclist_col = dsltmp_cfg_iptv_pvclist_row[i].split('>');

		//Protocol
		iptv_atm_pvc_str += "<span class='cfg_val'>";
		iptv_ptm_pvc_str += "<span class='cfg_val'>";
		if(dsltmp_cfg_iptv_pvclist_col[2] == 2) {
			iptv_atm_pvc_str += "MER, ";
			iptv_ptm_pvc_str += "Automatic IP ";
		}
		else if(dsltmp_cfg_iptv_pvclist_col[2] == 1) {
			iptv_atm_pvc_str += "PPPoA, ";
			iptv_ptm_pvc_str += " ";
		}
		else if(dsltmp_cfg_iptv_pvclist_col[2] == 0) {
			iptv_atm_pvc_str += "PPPoE, ";
			iptv_ptm_pvc_str += "PPPoE ";
		}
		else {  //3
			iptv_atm_pvc_str += "Bridge, ";
			iptv_ptm_pvc_str += "Bridge ";
		}
		//vpi
		iptv_atm_pvc_str += dsltmp_cfg_iptv_pvclist_col[0] + "/";
		//vci
		iptv_atm_pvc_str += dsltmp_cfg_iptv_pvclist_col[1] + ", ";
		//encap
		if(dsltmp_cfg_iptv_pvclist_col[3] == 1)
			iptv_atm_pvc_str += "VC-Mux ";
		else
			iptv_atm_pvc_str += "LLC ";
		//VLAN ID
		if(dsltmp_cfg_iptv_pvclist_col[4] != "") {
			iptv_atm_pvc_str += "</span>VLAN ID: <span class='cfg_val'>" + dsltmp_cfg_iptv_pvclist_col[4];
			iptv_ptm_pvc_str += "</span>VLAN ID: <span class='cfg_val'>" + dsltmp_cfg_iptv_pvclist_col[4];
		}       
                
		iptv_atm_pvc_str += "</span>";
		iptv_ptm_pvc_str += "</span>";
	}
}

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	parent.set_step("t2");
	document.form.x_DHCPClient[0].checked = 0;
	document.form.x_DHCPClient[1].checked = 1;
	document.form.wan_dnsenable_x[0].checked = 0;
	document.form.wan_dnsenable_x[1].checked = 1;
	show_dhcpenable(1);
	match_ISP_DHCP_opt();

	/* Renjie: Remove this field, we will use Option 61 and 12 for UK Sky.
	if (country_str == "United Kingdom" && (ispname_str == "SKY (MER)" || ispname_str == "Sky - Fibre Broadband"))
	{
		//UK ISP SKY Broadband, MER requires some tweak.
		//PPP Username and Password needs to be added into DHCP option 61.
		document.getElementById("ppp_username1").style.display = "";
		document.getElementById("ppp_username2").style.display = "";
		document.getElementById("ppp_password1").style.display = "";
		document.getElementById("ppp_password2").style.display = "";
	}
	*/

	if(transfer_mode == "ATM")
		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp";
	else //PTM
	{
		document.form.prev_page.value = "/cgi-bin/qis/QIS_PTM_manual_setting.asp";
		document.form.x_DHCPClient[0].disabled = true;
		document.form.x_DHCPClient[1].disabled = true;
	}
}

function match_ISP_DHCP_opt(){
	for(var a=0;a<ISP_DHCP_OPT_List.length;a++){
		if( country_str == ISP_DHCP_OPT_List[a][1] && ispname_str == ISP_DHCP_OPT_List[a][2]){
			document.form.dhcp_opt_enable.checked = true;
			showHideDHCPoptList(document.form.dhcp_opt_enable);
			//<dhcp opt 60 vendorid>, <dhcp opt 61 clientid>, <dhcp opt 61 clientid type>
			document.form.dsltmp_dhcp_vendorid.value = ISP_DHCP_OPT_List[a][3];
			if(ISP_DHCP_OPT_List[a][5] == 1){
				document.form.dsltmp_dhcp_clientid_type_tmp.checked = true;
				showDisableDHCPclientID(document.form.dsltmp_dhcp_clientid_type_tmp);
			}
			else{
				document.form.dsltmp_dhcp_clientid_type_tmp.checked = false;
				showDisableDHCPclientID(document.form.dsltmp_dhcp_clientid_type_tmp);
				document.form.text_dsltmp_dhcp_clientid.value = ISP_DHCP_OPT_List[a][4];
			}			
		}
		else{
			document.form.dhcp_opt_enable.checked = false;
			showHideDHCPoptList(document.form.dhcp_opt_enable);
		}
	}
}

function showHideDHCPoptList(dhcp_opt_enable) {
	if(dhcp_opt_enable.checked) {
		document.getElementById("dhcp_opt_table").style.visibility = "visible";		
	}
	else {
		document.getElementById("dhcp_opt_table").style.visibility = "hidden";
	}
}

function showDisableDHCPclientID(clientid_enable){
	if(clientid_enable.checked) {
		document.form.dsltmp_dhcp_clientid_type.value = "1";
		document.form.text_dsltmp_dhcp_clientid.value = "";
		document.form.text_dsltmp_dhcp_clientid.disabled = true;
	}
	else {
		document.form.dsltmp_dhcp_clientid_type.value = "0";
		document.form.text_dsltmp_dhcp_clientid.disabled = false;
	}
}

function validate_ip(){
	if(document.form.x_DHCPClient[0].checked){
		if(document.form.dsl_gateway.value == document.form.dsl_ipaddr.value){
			alert("<%tcWebApi_get("String_Entry","IPC_warning_WANIPEQUALGatewayIP","s")%>");
			return false;
		}
	}
	return true;
}

function validate_dns(){
	return true;
}

function show_dhcpenable(flag){
	if(flag == 1 || flag == true){
		show_dnsenable(0);
		IPinputCtrl(document.form.dsl_ipaddr, 0);
		IPinputCtrl(document.form.dsl_netmask, 0);
		IPinputCtrl(document.form.dsl_gateway, 0);
		document.form.wan_dnsenable_x[0].checked = 0;
		document.form.wan_dnsenable_x[1].checked = 1;
		inputCtrl(document.form.wan_dnsenable_x[0], 1);
		inputCtrl(document.form.wan_dnsenable_x[1], 1);
	}
	else{
		show_dnsenable(1);
		IPinputCtrl(document.form.dsl_ipaddr, 1);
		IPinputCtrl(document.form.dsl_netmask, 1);
		IPinputCtrl(document.form.dsl_gateway, 1);
		document.form.wan_dnsenable_x[0].checked = 1;
		document.form.wan_dnsenable_x[1].checked = 0;
		inputCtrl(document.form.wan_dnsenable_x[0], 1);
		inputCtrl(document.form.wan_dnsenable_x[1], 0);
	}
}

function show_dnsenable(flag){
	if(flag == 1 || flag == true){
		IPinputCtrl(document.form.dsl_dns1_x, 1);
		IPinputCtrl(document.form.dsl_dns2_x, 1);
	}
	else{
		IPinputCtrl(document.form.dsl_dns1_x, 0);
		IPinputCtrl(document.form.dsl_dns2_x, 0);
	}
}

function get_result_of_all_IP(){
	combineIP("dsl_ipaddr");
	combineIP("dsl_netmask");
	combineIP("dsl_gateway");
	combineIP("dsl_dns1_x");
	combineIP("dsl_dns2_x");
}

function remove_red_field(){
	$("dsl_ipaddr_div").style.border = "2px inset #F4F4F4";
	$("dsl_netmask_div").style.border = "2px inset #F4F4F4";
	$("dsl_gateway_div").style.border = "2px inset #F4F4F4";
	$("dsl_dns1_x_div").style.border = "2px inset #F4F4F4";
	$("dsl_dns2_x_div").style.border = "2px inset #F4F4F4";
}

function submitForm(){
	remove_red_field();
	get_result_of_all_IP();
	if(!validate_ip())
		return;
	if(!validate_dns())
		return;
	if(document.form.x_DHCPClient[0].checked) //Use static WAN and DNS IP; 1 Dynamic, 0 Static
	{
		document.getElementById("dsltmp_cfg_DHCPClient").value = "0";
		document.form.dsltmp_wanTypeOption.value = "1";
		document.getElementById("dsltmp_cfg_ipaddr").value = document.getElementById("dsl_ipaddr").value;
		document.getElementById("dsltmp_cfg_netmask").value = document.getElementById("dsl_netmask").value;
		document.getElementById("dsltmp_cfg_gateway").value = document.getElementById("dsl_gateway").value;

		document.getElementById("dsltmp_cfg_dnsenable").value = "0";
		document.getElementById("dsltmp_cfg_dns1").value = document.getElementById("dsl_dns1_x").value;
		document.getElementById("dsltmp_cfg_dns2").value = document.getElementById("dsl_dns2_x").value;
	}
	else //Use dynamic WAN and DNS IP; 1 Dynamic, 0 Static
	{
		document.getElementById("dsltmp_cfg_DHCPClient").value = "1";
		document.form.dsltmp_wanTypeOption.value = "0";

		if(document.form.wan_dnsenable_x[0].checked) //Use static DNS
		{
			document.getElementById("dsltmp_cfg_dnsenable").value = "0";
			document.getElementById("dsltmp_cfg_dns1").value = document.getElementById("dsl_dns1_x").value;
			document.getElementById("dsltmp_cfg_dns2").value = document.getElementById("dsl_dns2_x").value;
		}
		else //Use dynamic DNS
			document.getElementById("dsltmp_cfg_dnsenable").value = "1";
	}

	if (country_str == "United Kingdom" && (ispname_str == "SKY (MER)" || ispname_str == "Sky - Fibre Broadband"))
	{
		//Only for UK ISP SKY Broadband
		/* Renjie: Remove this field, we will use Option 61 and 12 for UK Sky.
		if(document.form.ppp_username.value != "")
			document.form.dsltmp_dhcp_clientid.value = document.form.ppp_username.value + "|" + document.form.ppp_password.value;

		document.form.ppp_username.disabled = true;
		document.form.ppp_password.disabled = true;
		*/

		document.form.dsltmp_dhcp_clientid.value = mac_addr_last_3bytes;
		document.form.dsltmp_dhcp_hostname.value = model_name;
	}
	else
	{
		if(document.form.text_dsltmp_dhcp_clientid.value != ""){
            document.form.dsltmp_dhcp_clientid.value = document.form.text_dsltmp_dhcp_clientid.value;
        }
        else{
			document.form.dsltmp_dhcp_clientid.value = "";
			document.form.dsltmp_dhcp_hostname.value = "";
		}
	}

	document.form.dsl_ipaddr.disabled = true;
	document.form.dsl_netmask.disabled = true;
	document.form.dsl_gateway.disabled = true;
	document.form.dsl_dns1_x.disabled = true;
	document.form.dsl_dns2_x.disabled = true;

	document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
	document.form.submit();
}

</script>
</head>
<body onLoad="QKfinish_load_body();" >
<form method="post" name="form" action="QIS_process.asp" target="">
<input type="hidden" name="prev_page" value="/cgi-bin/qis/QIS_manual_setting.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_mer_cfg_tmp.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_wireless.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="dsltmp_cfg_DHCPClient" id="dsltmp_cfg_DHCPClient" value="1">
<input type="hidden" name="dsltmp_cfg_ipaddr" id="dsltmp_cfg_ipaddr" value="">
<input type="hidden" name="dsltmp_cfg_netmask" id="dsltmp_cfg_netmask" value="">
<input type="hidden" name="dsltmp_cfg_gateway" id="dsltmp_cfg_gateway" value="">
<input type="hidden" name="dsltmp_cfg_dnsenable" id="dsltmp_cfg_dnsenable" value="1">
<input type="hidden" name="dsltmp_cfg_dns1" id="dsltmp_cfg_dns1" value="">
<input type="hidden" name="dsltmp_cfg_dns2" id="dsltmp_cfg_dns2" value="">
<input type="hidden" name="dsltmp_dhcp_hostname" value="">
<input type="hidden" name="dsltmp_wanTypeOption" value="">
<input type="hidden" name="with_wan_setting" value="1">

<input type="hidden" name="dsltmp_dhcp_clientid_type" value="">
<input type="hidden" name="dsltmp_dhcp_clientid" value="">
<div class="QISmain">
<div>
<table width="730px">
	<tr>
		<td align="left">
			<span class="description_down"><% tcWebApi_Get("String_Entry", "Manual_Setting_Title", "s") %></span>
		</td>
		<!--td align="right">
			<img onclick="gotoIndex();" style="cursor:pointer;" align="right" title="Go to Home" src="/images/backtohome.png" onMouseOver="this.src='/images/backtohomeclick.png'" onMouseOut="this.src='/images/backtohome.png'">
		</td-->
	</tr>
</table>
</div>
<div style="margin:5px;"><img style="width: 720px; *width: 710px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
<br>
<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
	<tr>
		<td align="left">
		<script>
			if(transfer_mode == "ATM")
			{				
					document.write("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>ADSL WAN (ATM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>MER</span>, VPI/VCI: <span class='cfg_val'>" + vpi_val + "/" + vci_val + ", " + encap_str + "</span>");
					if(vlanid_val.length > 0)
						document.write(", <%tcWebApi_get("String_Entry","WANVLANIDText","s")%>: <span class='cfg_val'>" + vlanid_val + "</span>");
					if(iptv_atm_pvc_str.length > 0)
						document.writeln("<br>"+iptv_atm_pvc_str);
			}
			else //PTM
			{				
					document.write("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>VDSL WAN (PTM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>Automatic IP</span>");
					if(vlanid_val.length > 0)
						document.write(", <%tcWebApi_get("String_Entry","WANVLANIDText","s")%>: <span class='cfg_val'>" + vlanid_val + "</span>");
					if(iptv_ptm_pvc_str.length > 0)
						document.writeln("<br>"+iptv_ptm_pvc_str);	
			}
		</script>
		</td>
	</tr>
</table>
<br>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr><td>
<div>
<fieldset>
<legend>
<%tcWebApi_get("String_Entry","L3F_x_DHCPClient_in","s")%>
<span id="dhcp_info_radio">
<input type="radio" name="x_DHCPClient" tabindex="1" class="input" value="0" onclick="show_dhcpenable(this.value);"><%tcWebApi_get("String_Entry","checkbox_No","s")%>
<input type="radio" name="x_DHCPClient" tabindex="2" class="input" value="1" onclick="show_dhcpenable(this.value);"><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
</span>
</legend>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_ExternalIPAddress_in","s")%>
</th>
<td class="QISformtd">
<input type="hidden" id="dsl_ipaddr" name="dsl_ipaddr" value="" maxlength="15" onkeypress="return validator.isIPAddr(this,event);" title="WAN IP">
<div class="IPaddr" id="dsl_ipaddr_div">
<input maxlength="3" tabindex="3"
name="dsl_ipaddr1";
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
id="dsl_ipaddr2"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
name="dsl_ipaddr4";
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />
</div>
</td>
</tr>
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_x_ExternalSubnetMask_in","s")%>
</th>
<td class="QISformtd">
<input type="hidden" id="dsl_netmask" name="dsl_netmask" value="" maxlength="15" title="WAN Subnet Mask">
<div class="IPaddr" id="dsl_netmask_div">
<input maxlength="3" tabindex="4"
name="dsl_netmask1"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_netmask4"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />
</div>
</td>
</tr>
<tr>
<th>
<%tcWebApi_get("String_Entry","IPC_x_ExternalGateway_in","s")%>
</th>
<td class="QISformtd">
<input type="hidden" id="dsl_gateway" name="dsl_gateway" value="" maxlength="15" onkeypress="return validator.isIPAddr(this,event);" class="input">
<div class="IPaddr" id="dsl_gateway_div">
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" tabindex="5"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_gateway4"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />
</div>
</td>
</tr>
</table>
</fieldset>
</div>

<div id="dns_sec">
<fieldset>
<legend>
<span id="dns_info_title"><%tcWebApi_get("String_Entry","IPC_x_DNSServerEnable_in","s")%>:</span>
<span id="dns_info_radio">
<input type="radio" name="wan_dnsenable_x" value="1" tabindex="6" onclick="show_dnsenable(this.value);" class="input"><%tcWebApi_get("String_Entry","checkbox_No","s")%>
<input type="radio" name="wan_dnsenable_x" value="0" tabindex="7" onclick="show_dnsenable(this.value);" class="input"><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
</span>
</legend>
<table id="tblsetting_4" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_x_DNSServer1_in","s")%>:
</th>
<td class="QISformtd">
<input type="hidden" id="dsl_dns1_x" name="dsl_dns1_x" value="" onkeypress="return validator.isIPAddr(this,event);" maxlength="15">
<div class="IPaddr" id="dsl_dns1_x_div">
<input maxlength="3" tabindex="8"
name="dsl_dns1_x1"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_dns1_x4"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />
</div>
</td>
</tr>
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_x_DNSServer2_in","s")%>:
</th>
<td class="QISformtd">
<input type="hidden" id="dsl_dns2_x" name="dsl_dns2_x" value="" maxlength="15" onkeypress="return validator.isIPAddr(this,event);">
<div class="IPaddr" id="dsl_dns2_x_div">
<input maxlength="3" tabindex="9"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_dns2_x4"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />
</div>
</td>
</tr>
</table>
</fieldset>
</div>
<br>

<div id="DHCP_opt">
<table id="dhcp_opt_setting" width="80%" border="0" align="left" cellpadding="3" cellspacing="0" style="margin-left:8%;">
		<tr>
			<td>
				<input type="checkbox" id="dhcp_opt_enable" name="dhcp_opt_enable" onclick="showHideDHCPoptList(this);">
				<label for="dhcp_opt_enable">
					<span class="QISGeneralFont" style="margin-left:0px;font-style:normal;color:#66CCFF;font-size:12px;font-weight:bolder;">DHCP option</span>
				</label>				
			</td>
		</tr>	
</table>	
</div>

<div style="margin-left:-90px;">
<table id="dhcp_opt_table" class="FormTable" width="475px" border="0" align="center" cellpadding="3" cellspacing="0">
	<tbody>		
		<tr id="Country_tr">
			<th width="40%">Class-identifier (option 60):</th>
			<td>
				<input type="text" name="dsltmp_dhcp_vendorid" class="input_25_table" value="" maxlength="126" autocapitalization="off" autocomplete="off">
			</td>
		</tr>
		<tr id="ISP_tr">
			<th width="40%">Class-identifier (option 61):</th>
			<td>
				<input type="checkbox" name="dsltmp_dhcp_clientid_type_tmp" onclick="showDisableDHCPclientID(this);">IAID/DUID<br>
				<input type="text" name="text_dsltmp_dhcp_clientid" class="input_25_table" value="" maxlength="126" autocapitalization="off" autocomplete="off">
			</td>
		</tr>
	</tbody>
</table>
</div>
<!-- Renjie: Remove this field, we will use Option 61 and 12 for UK Sky.
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
  <tr>
	<th width="120"><span id="ppp_username1" style="display:none"><% tcWebApi_Get("String_Entry", "PPPC_UserName_in", "s") %>:</span></th>
	<td>
	  <span id="ppp_username2" style="display:none">
	  <input type="text" name="ppp_username" value="" tabindex="10" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
	  </span>
	</td>
  </tr>
  <tr>
	<th width="120"><span id="ppp_password1" style="display:none"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %>:</span></th>
	<td>
	  <span id="ppp_password2" style="display:none">
	  <input type="text" name="ppp_password" value="" tabindex="11" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
	  </span>
	</td>
  </tr>
</table>
-->
</td></tr></table>
<div class="apply_gen" style="margin-top:30px">
<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" tabindex="13" onclick="gotoprev(document.form);" class="button_gen_long">
<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" tabindex="12" onclick="submitForm();" class="button_gen_long">
</div>
</div>
</form>
</body>

<!--qis/QIS_mer_cfg_tmp.asp-->
</html>
