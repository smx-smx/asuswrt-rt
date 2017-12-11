<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_ipoa_cfg_tmp.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<script language="JavaScript" type="text/Javascript" src="/state.js"></script>
<script language="JavaScript" type="text/Javascript" src="/general.js"></script>
<script language="JavaScript" type="text/JavaScript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
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

	if(transfer_mode == "ATM")
		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp";
	else //PTM
		document.form.prev_page.value = "/cgi-bin/qis/QIS_PTM_manual_setting.asp";

	if (country_str == "France" && ispname_str == "Free Degroupe"){
				document.form.dsl_dns1_x.value = "212.27.40.240";
				document.form.dsl_dns2_x.value = "212.27.40.241";
	}
}

function validate_ip(){
	if(document.form.dsl_gateway.value == document.form.dsl_ipaddr.value){
		alert("<%tcWebApi_get("String_Entry","IPC_warning_WANIPEQUALGatewayIP","s")%>");
		return false;
	}
	return true;
}

function submitForm(){
	if(validForm()){		

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
	
}

function validForm(){
	// test if LAN IP is a private IP.
	var A_class_start = inet_network("10.0.0.0");
	var A_class_end = inet_network("10.255.255.255");
	var B_class_start = inet_network("172.16.0.0");
	var B_class_end = inet_network("172.31.255.255");
	var C_class_start = inet_network("192.168.0.0");
	var C_class_end = inet_network("192.168.255.255");
	
	var ip_obj = document.form.dsl_ipaddr;
	var ip_num = inet_network(ip_obj.value);
	var ip_class = "";	
	
	if(ip_num > A_class_start && ip_num < A_class_end)
		ip_class = 'A';
	else if(ip_num > B_class_start && ip_num < B_class_end)
		ip_class = 'B';
	else if(ip_num > C_class_start && ip_num < C_class_end)
		ip_class = 'C';
	else{
		alert(ip_obj.value+" <#JS_validip#>");
		ip_obj.value = "";
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
	
	// test if netmask is valid.
	var netmask_obj = document.form.dsl_netmask;
	var netmask_num = inet_network(netmask_obj.value);
	var netmask_reverse_num = ~netmask_num;
	var default_netmask = "";
	var wrong_netmask = 0;

	if(netmask_num < 0) wrong_netmask = 1;	

	if(ip_class == 'A')
		default_netmask = "255.0.0.0";
	else if(ip_class == 'B')
		default_netmask = "255.255.0.0";
	else
		default_netmask = "255.255.255.0";
	
	var test_num = netmask_reverse_num;
	while(test_num != 0){
		if((test_num+1)%2 == 0)
			test_num = (test_num+1)/2-1;
		else{
			wrong_netmask = 1;
			break;
		}
	}
	if(wrong_netmask == 1){
		alert(netmask_obj.value+" <#JS_validip#>");
		netmask_obj.value = default_netmask;
		netmask_obj.focus();
		netmask_obj.select();
		return false;
	}
	
	if(!validate_ip())
			return false;
	
	var subnet_head = getSubnet(ip_obj.value, netmask_obj.value, "head");
	var subnet_end = getSubnet(ip_obj.value, netmask_obj.value, "end");
	
	if(ip_num == subnet_head || ip_num == subnet_end){
		alert(ip_obj.value+" <#JS_validip#>");
		ip_obj.value = "";
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
	return true;
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
<input type="text" id="dsl_ipaddr" name="dsl_ipaddr" tabindex="1" class="input_15_table" value="" maxlength="15" onkeypress="return validator.isIPAddr(this,event);" title="WAN IP" autocorrect="off" autocapitalize="off">
</td>
</tr>
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_x_ExternalSubnetMask_in","s")%>
</th>
<td class="QISformtd">
<input type="text" id="dsl_netmask" name="dsl_netmask" tabindex="2" class="input_15_table" value="" onkeypress="return validator.isIPAddr(this,event);" maxlength="15" title="WAN Subnet Mask" autocorrect="off" autocapitalize="off">
</td>
</tr>
<tr>
<th>
<%tcWebApi_get("String_Entry","IPC_x_ExternalGateway_in","s")%>
</th>
<td class="QISformtd">
<input type="text" id="dsl_gateway" name="dsl_gateway" tabindex="3" class="input_15_table" value="" maxlength="15" onkeypress="return validator.isIPAddr(this,event);" class="input" autocorrect="off" autocapitalize="off">
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
<input type="text" id="dsl_dns1_x" name="dsl_dns1_x" tabindex="4" class="input_15_table" value="" onkeypress="return validator.isIPAddr(this,event);" maxlength="15" autocorrect="off" autocapitalize="off">
</td>
</tr>
<tr>
<th width="180">
<%tcWebApi_get("String_Entry","IPC_x_DNSServer2_in","s")%>:
</th>
<td class="QISformtd">
<input type="text" id="dsl_dns2_x" name="dsl_dns2_x" tabindex="5" class="input_15_table" value="" maxlength="15" onkeypress="return validator.isIPAddr(this,event);" autocorrect="off" autocapitalize="off">
</td>
</tr>
</table>
</fieldset>
</td></tr></table>

<div class="test_css" style="margin-top:20px;margin-left:55px;">Note:  If you are not sure about the IP/DNS address setting, then please contact with your ISP.</div>

<div class="apply_gen" style="margin-top:30px">
<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" tabindex="7" onclick="gotoprev(document.form);" class="button_gen_long">
<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" tabindex="6" onclick="submitForm();" class="button_gen_long">
</div>
</div>
</form>
</body>

<!--qis/QIS_ipoa_cfg_tmp.asp-->
</html>
