<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_mer_cfg.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<script type="text/Javascript" src="/state.js"></script>
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
<script type="text/Javascript" src="/jquery.js"></script>
<script type="text/javascript">
var detect_status = "<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>";
var vpi_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VPI","s") %>";
var vci_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VCI","s") %>";
var encap_val = "<% tcWebApi_get("AutoPVC_Common","Detect_Encap","s") %>";
var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";
var encap_str = "LLC";
if (encap_val == "vc") encap_str = "VC-Mux";

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	//parent.set_step("t3");

	if(encap_val == "vc")
		document.form.dsltmp_cfg_encap.value = "1";
	else
		document.form.dsltmp_cfg_encap.value = "0";

	if (vpi_val == "0" && vci_val == "40" && encap_val == "1")
	{
		//UK ISP SKY Broadband, MER requires some tweak.
		//PPP Username and Password needs to be added into DHCP option 61.
		document.getElementById("ppp_username1").style.display = "";
		document.getElementById("ppp_username2").style.display = "";
		document.getElementById("ppp_password1").style.display = "";
		document.getElementById("ppp_password2").style.display = "";
	}


	if(wan_type == "ATM")
		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp";
	else //PTM
		document.form.prev_page.value = "/cgi-bin/qis/QIS_PTM_manual_setting.asp";
}

function submitForm(){
	<%if tcWebApi_get("AutoPVC_Common","Detect_XDSL","h") = "PTM" then %>
	var tmp_vlanid = document.form.dsltmp_cfg_vlanid.value;
	if ( isNaN(tmp_vlanid) == true ) {
		alert('<%tcWebApi_get("String_Entry","WANVLANIDText","s")%> "' + tmp_vlanid + '" <%tcWebApi_get("String_Entry","Manual_Setting_JS_invalid","s")%>');
		document.form.dsltmp_cfg_vlanid.focus();
		return false;
	}
	<%end if%>

	if (vpi_val == "0" && vci_val == "40" && encap_val == "1")
	{
		//Only for UK ISP SKY Broadband
		if(document.form.ppp_username.value != "")
			document.form.dsltmp_dhcp_clientid.value = document.form.ppp_username.value + "|" + document.form.ppp_password.value;

		document.form.ppp_username.disabled = true;
		document.form.ppp_password.disabled = true;
	}

	document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
	document.form.submit();
}

</script>
</head>
<body onLoad="QKfinish_load_body();" >
<form method="post" name="form" action="QIS_process.asp" target="">
<input type="hidden" name="prev_page" value="/cgi-bin/qis/QIS_manual_setting.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_mer_cfg.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_wireless.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="dsltmp_cfg_vpi" value="<% tcWebApi_get("AutoPVC_Common","Detect_VPI","s") %>">
<input type="hidden" name="dsltmp_cfg_vci" value="<% tcWebApi_get("AutoPVC_Common","Detect_VCI","s") %>">
<input type="hidden" name="dsltmp_cfg_prctl" value="2">
<input type="hidden" name="dsltmp_cfg_encap" value="">
<input type="hidden" name="dsltmp_transfer_mode" value="<% tcWebApi_get("AutoPVC_Common","Detect_XDSL","s") %>">
<input type="hidden" name="dsltmp_cfg_DHCPClient" id="dsltmp_cfg_DHCPClient" value="1">
<input type="hidden" name="dsltmp_cfg_dnsenable" id="dsltmp_cfg_dnsenable" value="1">
<input type="hidden" name="dsltmp_wanTypeOption" value="0">
<input type="hidden" name="with_wan_setting" value="1">
<input type="hidden" name="dsltmp_dhcp_clientid" value="">
<div class="QISmain">
<div>
<table width="730px">
	<tr>
		<td align="left">
			<span class="description_down">
			<script>
				if(wan_type == "ATM")
					document.write("MER");
				else //PTM
					document.write("Automatic IP");
			</script>
			</span>
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
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: ADSL WAN (ATM), <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: MER, VPI/VCI: " + vpi_val + "/" + vci_val + ", " + encap_str + "<br><br>");
			else //PTM
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: VDSL WAN (PTM), <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: Automatic IP<br><br>");
		</script>
		</td>
	</tr>
</table>

<table style="margin-left:50px;">
  <tr>
	<th width="120" class="test_css"><span id="ppp_username1" style="display:none;">SKY <% tcWebApi_Get("String_Entry", "HSDPAC_Username_in", "s") %>:</span></th>
	<td>
	  <span id="ppp_username2" style="display:none;">
	  <input type="text" name="ppp_username" class="input_25_table" value="" maxlength="32" autocapitalization="off" autocomplete="off">
	  </span>
	</td>
  </tr>
  <tr>
	<th width="120" class="test_css"><span id="ppp_password1" style="display:none;">SKY <% tcWebApi_Get("String_Entry", "HSDPAC_Password_in", "s") %>:</span></th>
	<td>
	  <span id="ppp_password2" style="display:none;">
	  <input type="text" name="ppp_password" class="input_25_table" value="" maxlength="32" autocapitalization="off" autocomplete="off">
	  </span>
	</td>
  </tr>
</table>

<br><br>
<%if tcWebApi_get("AutoPVC_Common","Detect_XDSL","h") = "PTM" then %>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<th width="120"><%tcWebApi_get("String_Entry","WANVLANIDText","s")%></th>
<td>
<input type="text" name="dsltmp_cfg_vlanid" value="" maxlength="4" class="input_6_table">
</td>
</tr>
</table>
<%end if%>
<div class="apply_gen" style="margin-top:30px">
<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" onclick="gotoprev(document.form);" class="button_gen_long">
<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" onclick="submitForm();" class="button_gen">
</div>
</div>
</form>
</body>

<!--qis/QIS_mer_cfg.asp-->
</html>
