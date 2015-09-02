<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_ppp_cfg_tmp.asp-->
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
var prctl_str = "";
var prctl_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_prctl","s") %>";
var vpi_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vpi","s") %>";
var vci_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vci","s") %>";
var encap_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_encap","s") %>";
var ispname_str = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_ispname","s") %>";
var country_str = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_country","s") %>";
var transfer_mode = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","s") %>";
var vlanid = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vlanid","s") %>";
var iptv_num_pvc_str = "";
var status_message1 = "";
var status_message2 = "";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";
if (prctl_val == "0")
	prctl_str = "PPPoE";
else
	prctl_str = "PPPoA";

var encap_str = "LLC";
if (encap_val == "1") encap_str = "VC-Mux";
var iptv_num_pvc_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","s") %>";
if (iptv_num_pvc_val != "0" && iptv_num_pvc_val != "") iptv_num_pvc_str = iptv_num_pvc_val + " IPTV PVC(s).";

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	//parent.set_step("t3");
	if(transfer_mode == "ATM")
	{
		//Brazil - GVT
		if (country_str == "Brazil")
		{
			if (prctl_val == "0" && vpi_val == "0" && vci_val == "35" && encap_val == "0" && ispname_str == "GVT")
			{
				document.form.prevButton.readonly = true;
				document.form.nextButton.readonly = true;
				document.form.dsltmp_cfg_pppoe_username.value = "turbonet@turbonet";
				document.form.dsltmp_cfg_pppoe_passwd.value = "gvt25";
				setTimeout("submitForm()",2000);
			}
			/*if (ispname_str == "Telefonica Speedy")
			{
				document.form.dsltmp_qis_pppoe_mtu.value = "1480";
				//document.form.dsltmp_qis_pppoe_mru.value = "1480";
			}*/
		}
		/*if (country_str == "India")
		{
			if(ispname_str == "BSNL (PPPOE)" || ispname_str == "BSNL (PPPOA)" || ispname_str == "MTNL (PPPOE)" || ispname_str == "MTNL (PPPOA)")
			{
				document.form.dsltmp_qis_pppoe_mtu.value = "1442";
				//document.form.dsltmp_qis_pppoe_mru.value = "1442";
			}
		}
		if (country_str == "Denmark")
		{
			document.getElementById("mac_addr1").style.display = "";
			document.getElementById("mac_addr2").style.display = "";
		}*/

		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp"
	}
	else //PTM
		document.form.prev_page.value = "/cgi-bin/qis/QIS_PTM_manual_setting.asp";
}

function submitForm(){
	if(document.form.dsltmp_cfg_pppoe_username.value == "" || document.form.dsltmp_cfg_pppoe_passwd.value == "")
	{
		alert("<%tcWebApi_get("String_Entry","WANJS9Text","s")%>.");
		return false;
	}
	else
	{
		document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
		document.form.submit();
	}
}

</script>
</head>
<body onLoad="QKfinish_load_body();">
<form method="post" name="form" action="QIS_process.asp" target="">
<input type="hidden" name="prev_page" value="/cgi-bin/qis/QIS_manual_setting.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_ppp_cfg_tmp.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_wireless.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
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
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: ADSL WAN (ATM), <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: " + prctl_str + ", VPI/VCI: " + vpi_val + "/" + vci_val + ", " + encap_str + "<br><br>");
				if(iptv_num_pvc_str != "")
					document.writeln(iptv_num_pvc_str + "<br><br>");
			}
			else //PTM
			{
				if(vlanid == "")
					document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: VDSL WAN (PTM), <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: PPPoE<br><br>");
				else
					document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: VDSL WAN (PTM), <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: PPPoE, <%tcWebApi_get("String_Entry","WANVLANIDText","s")%>: " + vlanid + "<br><br>");
			}
		</script>
		<% tcWebApi_Get("String_Entry", "PPP_cfg_usersel_desc", "s") %>
		</td>
	</tr>
</table>
<br><br>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<th width="120"><% tcWebApi_Get("String_Entry", "PPPC_UserName_in", "s") %></th>
<td>
<input type="text" name="dsltmp_cfg_pppoe_username" value="" tabindex="1" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
</td>
</tr>
<tr>
<th width="120"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></th>
<td>
<input type="password" name="dsltmp_cfg_pppoe_passwd" value="" tabindex="2" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
</td>
</tr>
<!--<tr>
<th width="120"><span id="mac_addr1" style="display:none"><% tcWebApi_Get("String_Entry", "MAC_Address", "s") %></span></th>
<td>
<span id="mac_addr2" style="display:none">
<input type='text' name='dsltmp_qis_hwaddr' maxlength="12" size='12'>&nbsp;<% tcWebApi_Get("String_Entry", "PPPC_x_MacAddressForISP_id", "s") %>
</span>
</td>
</tr>-->
</table>
<div class="apply_gen" style="margin-top:30px">
<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" tabindex="4" onclick="gotoprev(document.form);" class="button_gen_long">
<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" tabindex="3" onclick="submitForm();" class="button_gen">
</div>
</div>
</form>
</body>

<!--qis/QIS_ppp_cfg_tmp.asp-->
</html>
