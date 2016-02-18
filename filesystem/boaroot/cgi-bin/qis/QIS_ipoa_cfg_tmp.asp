<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_ipoa_cfg_tmp.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<script type="text/Javascript" src="/state.js"></script>
<script type="text/Javascript" src="/general.js"></script>
<script type="text/Javascript" src="/form.js"></script>
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
		for(j=0; j<dsltmp_cfg_iptv_pvclist_col.length; j++){
		switch(j) {
			case 0: //vpi
				iptv_atm_pvc_str += "<span class='cfg_val'>" + dsltmp_cfg_iptv_pvclist_col[j] + "/";
				break;
			case 1: //vci
				iptv_atm_pvc_str += dsltmp_cfg_iptv_pvclist_col[j] + ",";
				break;
			case 2: //proto
				iptv_ptm_pvc_str += "<span class='cfg_val'>";
				if(dsltmp_cfg_iptv_pvclist_col[j] == 3) {
					iptv_atm_pvc_str += "Bridged, ";
					iptv_ptm_pvc_str += "Bridged, ";
				}
				else {
					iptv_atm_pvc_str += ", ";
					iptv_ptm_pvc_str += "Bridged, ";
				}
				break;
			case 3: //encap
				if(dsltmp_cfg_iptv_pvclist_col[j] == 1)
					iptv_atm_pvc_str += "VC-Mux";
				else
					iptv_atm_pvc_str += "LLC";
				break;
			case 4: //vlan id
				if(dsltmp_cfg_iptv_pvclist_col[j] != "") {
					iptv_atm_pvc_str += "VLAN ID " + dsltmp_cfg_iptv_pvclist_col[j];
					iptv_ptm_pvc_str += "VLAN ID " + dsltmp_cfg_iptv_pvclist_col[j];
				}
				iptv_atm_pvc_str += "</span>";
				iptv_ptm_pvc_str += "</span>";
				break;
		}
		}
	}
}

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	//parent.set_step("t3");
	IPinputCtrl(document.form.dsl_ipaddr, 1);
	IPinputCtrl(document.form.dsl_netmask, 1);
	IPinputCtrl(document.form.dsl_gateway, 1);
	IPinputCtrl(document.form.dsl_dns1_x, 1);
	IPinputCtrl(document.form.dsl_dns2_x, 1);

	if(transfer_mode == "ATM")
		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp";
	else //PTM
		document.form.prev_page.value = "/cgi-bin/qis/QIS_PTM_manual_setting.asp";

	if (country_str == "France" && ispname_str == "Free Degroupe"){
				document.form.dsl_dns1_x1.value = "212";
				document.form.dsl_dns1_x2.value = "27";
				document.form.dsl_dns1_x3.value = "40";
				document.form.dsl_dns1_x4.value = "240";
				document.form.dsl_dns2_x1.value = "212";
				document.form.dsl_dns2_x2.value = "27";
				document.form.dsl_dns2_x3.value = "40";
				document.form.dsl_dns2_x4.value = "241";
	}
}

function validate_ip(){
	if(document.form.dsl_gateway.value == document.form.dsl_ipaddr.value){
		alert("<%tcWebApi_get("String_Entry","IPC_warning_WANIPEQUALGatewayIP","s")%>");
		return false;
	}
	return true;
}

function validate_dns(){
	return true;
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
	document.getElementById("dsltmp_cfg_ipaddr").value = document.getElementById("dsl_ipaddr").value;
	document.getElementById("dsltmp_cfg_netmask").value = document.getElementById("dsl_netmask").value;
	document.getElementById("dsltmp_cfg_gateway").value = document.getElementById("dsl_gateway").value;
	document.getElementById("dsltmp_cfg_dns1").value = document.getElementById("dsl_dns1_x").value;
	document.getElementById("dsltmp_cfg_dns2").value = document.getElementById("dsl_dns2_x").value;

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
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_ipoa_cfg_tmp.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_wireless.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="dsltmp_cfg_DHCPClient" id="dsltmp_cfg_DHCPClient" value="0">
<input type="hidden" name="dsltmp_cfg_ipaddr" id="dsltmp_cfg_ipaddr" value="">
<input type="hidden" name="dsltmp_cfg_netmask" id="dsltmp_cfg_netmask" value="">
<input type="hidden" name="dsltmp_cfg_gateway" id="dsltmp_cfg_gateway" value="">
<input type="hidden" name="dsltmp_cfg_dnsenable" id="dsltmp_cfg_dnsenable" value="0">
<input type="hidden" name="dsltmp_cfg_dns1" id="dsltmp_cfg_dns1" value="">
<input type="hidden" name="dsltmp_cfg_dns2" id="dsltmp_cfg_dns2" value="">
<input type="hidden" name="with_wan_setting" value="1">
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
<br/>
<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
	<tr>
		<td align="left">
		<script>
			if(transfer_mode == "ATM")
			{				
					document.write("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>ADSL WAN (ATM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>IPoA</span>, VPI/VCI: <span class='cfg_val'>" + vpi_val + "/" + vci_val + ", " + encap_str +"</span>");
					if(vlanid_val.length > 0)
						document.write(", <%tcWebApi_get("String_Entry","WANVLANIDText","s")%>: <span class='cfg_val'>" + vlanid_val + "</span>");
					if(iptv_atm_pvc_str.length > 0)					
						document.writeln("<br>"+iptv_atm_pvc_str);
			}
			else //PTM
			{				
					document.write("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>VDSL WAN (PTM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>Static IP</span>");
					if(vlanid_val.length > 0)
						document.write(", <%tcWebApi_get("String_Entry","WANVLANIDText","s")%>: <span class='cfg_val'>" + vlanid_val + "</span><br><br>");
					if(iptv_ptm_pvc_str.length > 0)
						document.writeln("<br>"+iptv_ptm_pvc_str);	
			}
		</script>
		</td>
	</tr>
</table>
<br><br>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0"><tr><td>
<fieldset>
<legend>
<%tcWebApi_get("String_Entry","L3F_x_UseStaticIP_in","s")%>
</legend>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_ExternalIPAddress_in","s")%>
</th>
<td class="QISformtd">
<input type="hidden" id="dsl_ipaddr" name="dsl_ipaddr" value="" maxlength="15" onkeypress="return is_ipaddr(this);" onkeyup="change_ipaddr(this);" title="WAN IP">
<div class="IPaddr" id="dsl_ipaddr_div">
<input maxlength="3" tabindex="1"
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
<input maxlength="3" tabindex="2"
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
<input type="hidden" id="dsl_gateway" name="dsl_gateway" value="" maxlength="15" onkeypress="return is_ipaddr(this);" onkeyup="return change_ipaddr(this);" class="input">
<div class="IPaddr" id="dsl_gateway_div">
<input maxlength="3" tabindex="3"
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
<span id="dns_info_title"><%tcWebApi_get("String_Entry","qis_dns","s")%>:</span>
</legend>
<table id="tblsetting_4" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_x_DNSServer1_in","s")%>:
</th>
<td class="QISformtd">
<input type="hidden" id="dsl_dns1_x" name="dsl_dns1_x" value="" onkeypress="return is_ipaddr(this);" onkeyup="return change_ipaddr(this);" maxlength="15">
<div class="IPaddr" id="dsl_dns1_x_div">
<input maxlength="3" tabindex="4" name="dsl_dns1_x1"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_dns1_x2"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_dns1_x3"
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
<input type="hidden" id="dsl_dns2_x" name="dsl_dns2_x" value="" maxlength="15" onkeypress="return is_ipaddr(this);" onkeyup="return change_ipaddr(this);">
<div class="IPaddr" id="dsl_dns2_x_div">
<input maxlength="3" tabindex="5" name="dsl_dns2_x1"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_dns2_x2"
onkeypress="return checkIP(this,event);"
onkeyup="return checkWord(this,event);"
onblur="validateIP(this);"
onpaste="return !clipboardData.getData('text').match(/\D/)"
ondragenter="return false"
autocomplete="off" />.
<input maxlength="3" name="dsl_dns2_x3"
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
</td></tr></table>

<div class="test_css" style="margin-top:20px;margin-left:55px;">Note:  If you are not sure about the IP/DNS address setting, then please contact with your ISP.</div>

<div class="apply_gen" style="margin-top:30px">
<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" tabindex="7" onclick="gotoprev(document.form);" class="button_gen_long">
<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" tabindex="6" onclick="submitForm();" class="button_gen">
</div>
</div>
</form>
</body>

<!--qis/QIS_ipoa_cfg_tmp.asp-->
</html>
