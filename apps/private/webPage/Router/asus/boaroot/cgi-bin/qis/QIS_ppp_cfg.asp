<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_ppp_cfg.asp-->
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
<script type="text/Javascript" src="/jstz.min.js"></script>
<script type="text/javascript">
var detect_status = "<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>";
var prctl_str = "";
var gvt_note = "";
var vpi_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VPI","s") %>";
var vci_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VCI","s") %>";
var encap_val = "<% tcWebApi_get("AutoPVC_Common","Detect_Encap","s") %>";
var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";
if(detect_status == "pppoe")
	prctl_str = "PPPoE";
else
	prctl_str = "PPPoA";
var encap_str = "LLC";
if (encap_val == "vc") encap_str = "VC-Mux";

var is_Custom = "<% tcWebApi_get("SysInfo_Entry", "Customer", "s") %>";

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";

	if(is_Custom.toUpperCase() == "POL"){
				document.form.dsltmp_cfg_pppoe_username.value = "<% tcWebApi_get("Wan_PVC0","USERNAME", "s") %>";
				document.form.dsltmp_cfg_pppoe_passwd.value = "<% tcWebApi_get("Wan_PVC0","PASSWORD", "s") %>";
	}	

	//parent.set_step("t3");
	if(detect_status == "pppoe")
		document.form.dsltmp_cfg_prctl.value = "0";
	else
		document.form.dsltmp_cfg_prctl.value = "1";

	if(encap_val == "vc")
		document.form.dsltmp_cfg_encap.value = "1";
	else
		document.form.dsltmp_cfg_encap.value = "0";

	var timezone = jstz.determine_timezone();
	var timez = timezone.offset();
	if (vpi_val == "0" && vci_val == "35" && encap_val == "llc" && detect_status == "pppoe" && timez == "-03:00")
	{ //Brazil - GVT
		gvt_note = "<br><br>" + "(<%tcWebApi_get("String_Entry","isp_br_gvt_note","s")%>)";
		document.getElementById('hint_msg').innerHTML = gvt_note;
	}
	/*else if (vpi_val == "0" && vci_val == "35" && encap_val == "llc" && detect_status == "pppoe" && timez == "+05:30")
	{ //India - BSNL (PPPOE). This will also change Dada Infosys and HCL Infinet Ltd's MTU to 1442, but it's ok.
		document.form.dsltmp_qis_pppoe_mtu.value = "1442";
	}
	else if (vpi_val == "0" && vci_val == "35" && encap_val == "llc" && detect_status == "pppoa" && timez == "+05:30")
	{ //India - BSNL (PPPOA)
		document.form.dsltmp_qis_pppoe_mtu.value = "1442";
	}
	else if (vpi_val == "0" && vci_val == "32" && encap_val == "llc" && detect_status == "pppoe" && timez == "+05:30")
	{ //India - MTNL (PPPOE)
		document.form.dsltmp_qis_pppoe_mtu.value = "1442";
	}
	else if (vpi_val == "0" && vci_val == "32" && encap_val == "llc" && detect_status == "pppoa" && timez == "+05:30")
	{ //India - MTNL (PPPOA)
		document.form.dsltmp_qis_pppoe_mtu.value = "1442";
	}*/

	if(wan_type == "ATM")
		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp";
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
		<%if tcWebApi_get("AutoPVC_Common","Detect_XDSL","h") = "PTM" then %>
		var tmp_vlanid = document.form.dsltmp_cfg_vlanid.value;
		if ( isNaN(tmp_vlanid) == true ) {
			alert('<%tcWebApi_get("String_Entry","WANVLANIDText","s")%> "' + tmp_vlanid + '" <%tcWebApi_get("String_Entry","Manual_Setting_JS_invalid","s")%>');
			document.form.dsltmp_cfg_vlanid.focus();
			return false;
		}
		<%end if%>
		document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
		document.form.submit();
	}
}

</script>
</head>
<body onLoad="QKfinish_load_body();" >
<form method="post" name="form" action="QIS_process.asp" target="">
<input type="hidden" name="prev_page" value="/cgi-bin/qis/QIS_manual_setting.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_ppp_cfg.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_wireless.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="dsltmp_cfg_vpi" value="<% tcWebApi_get("AutoPVC_Common","Detect_VPI","s") %>">
<input type="hidden" name="dsltmp_cfg_vci" value="<% tcWebApi_get("AutoPVC_Common","Detect_VCI","s") %>">
<input type="hidden" name="dsltmp_cfg_prctl" value="">
<input type="hidden" name="dsltmp_cfg_encap" value="">
<input type="hidden" name="dsltmp_transfer_mode" value="<% tcWebApi_get("AutoPVC_Common","Detect_XDSL","s") %>">
<input type="hidden" name="dsltmp_wanTypeOption" value="2">
<input type="hidden" name="with_wan_setting" value="1">
<div class="QISmain">
<div>
<table width="730px">
	<tr>
		<td align="left">
			<span class="description_down"><script>document.write(prctl_str);</script></span>
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
			if(wan_type == "ATM")
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: ADSL WAN (ATM), <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: " + prctl_str + ", VPI/VCI: " + vpi_val + "/" + vci_val + ", " + encap_str + "<br><br>");
			else //PTM
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: VDSL WAN (PTM), <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: PPPoE<br><br>");
		</script>
		<% tcWebApi_Get("String_Entry", "PPP_cfg_usersel_desc", "s") %>
		<div id=hint_msg></div>
		</td>
	</tr>
</table>
<br><br>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<th width="120"><% tcWebApi_Get("String_Entry", "PPPC_UserName_in", "s") %></th>
<td>
<input type="text" name="dsltmp_cfg_pppoe_username" value="" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
</td>
</tr>
<tr>
<th width="120"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></th>
<td>
<input type="password" name="dsltmp_cfg_pppoe_passwd" value="" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
</td>
</tr>
<%if tcWebApi_get("AutoPVC_Common","Detect_XDSL","h") = "PTM" then %>
<tr>
<th width="120"><%tcWebApi_get("String_Entry","WANVLANIDText","s")%></th>
<td>
<input type="text" name="dsltmp_cfg_vlanid" value="" maxlength="4" class="input_6_table">
</td>
</tr>
<%end if%>
</table>
<div class="apply_gen" style="margin-top:30px">
<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" onclick="gotoprev(document.form);" class="button_gen_long" >
<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" onclick="submitForm();" class="button_gen">
</div>
</div>
</form>
</body>

<!--qis/QIS_ppp_cfg.asp-->
</html>
