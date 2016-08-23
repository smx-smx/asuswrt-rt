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
#DE_ISP_note{
	font-size:14px;
	font-family:Arial, Helvetica, sans-serif;
	text-align:left;
	margin-left:70px;	
	margin-top:-10px;
}
.isp_note_title{
	color:#FFCC00;
}
.isp_note{	
	margin-top:5px;
	line-height:100%;
}
.account_format{
	margin-top:5px;
	margin-left:30px;
}
.num2{
	color:#569AC7;
	font-weight: bold;
}
.num1{
	color:#CCFF00;
	font-weight: bold;
}
</style>
<script type="text/javascript">
var prctl_str = "";
var prctl_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_prctl","s") %>";
var vpi_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vpi","s") %>";
var vci_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vci","s") %>";
var encap_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_encap","s") %>";
var vlanid_val = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vlanid","s") %>";
var ispname_str = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_ispname","s") %>";
var country_str = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_country","s") %>";
var transfer_mode = "<% tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","s") %>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";

if (prctl_val == "0")
	prctl_str = "PPPoE";
else
	prctl_str = "PPPoA";

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
	parent.set_step("t2");
	
	DE_ISP_note_detect();
	
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

function DE_ISP_note_detect(){
	//Germany ISP setting note start-------------------
	//console.log( country_str+" > "+prctl_val+" > "+vpi_val+" > "+vci_val+" > "+encap_val+" > "+ispname_str);
	//ATM
	//Germany > 0 > 1 > 32 > 0 > Deutsche Telekom
	//Germany > 0 > 1 > 32 > 0 > 1&1
	//Germany > 0 > 8 > 35 > 0 > NetCologne
	//Germany > 0 > 8 > 35 > 0 > NetCologne (VLAN ID 10)
	//PTM
	//Germany > 0 > 8 > 35 > 0 > Deutsche Telekom (ohne Entertain)
	//Germany > 0 > 8 > 35 > 0 > Deutsche Telekom (mit Entertain)
	//Germany > 0 > 8 > 35 > 0 > 1&1 (Telekom VDSL Resale Anschluss)
	if(country_str == "Germany" 
		&& (ispname_str == "Deutsche Telekom" || ispname_str == "1&1" || ispname_str == "NetCologne" || ispname_str == "NetCologne (VLAN ID 10)" || 
				ispname_str == "Deutsche Telekom (ohne Entertain)" || ispname_str == "Deutsche Telekom (mit Entertain)" || ispname_str == "1&1 (Telekom VDSL Resale Anschluss)"))
	{
		document.getElementById("DE_ISP_note").style.display="";
		document.getElementById("Deutsche_Telekom").style.display="none";
		document.getElementById("Deutsche_1n1_ATM").style.display="none";
		document.getElementById("Deutsche_1n1_PTM").style.display="none";
		document.getElementById("Deutsche_NetCologne").style.display="none";
		if(ispname_str == "Deutsche Telekom" || ispname_str == "Deutsche Telekom (ohne Entertain)" || ispname_str == "Deutsche Telekom (mit Entertain)")
			document.getElementById("Deutsche_Telekom").style.display="";
		if(ispname_str == "1&1")	
			document.getElementById("Deutsche_1n1_ATM").style.display="";
		if(ispname_str == "1&1 (Telekom VDSL Resale Anschluss)")	
			document.getElementById("Deutsche_1n1_PTM").style.display="";
		if(ispname_str == "NetCologne" || ispname_str == "NetCologne (VLAN ID 10)")	
			document.getElementById("Deutsche_NetCologne").style.display="";				
	}
	else
		document.getElementById("DE_ISP_note").style.display="none";
	
	//Germany ISP setting note end-------------------	
}

function submitForm(){
	if(document.form.dsltmp_cfg_pppoe_username.value.length <= 0){
		alert("<%tcWebApi_get("String_Entry","WANJS9Text","s")%>.");
		document.form.dsltmp_cfg_pppoe_username.focus();
		return false;
	}
	if(!validate_string(document.form.dsltmp_cfg_pppoe_username)){
		alert("<%tcWebApi_get("String_Entry","WANJS19Text","s")%>");
		document.form.dsltmp_cfg_pppoe_username.focus();
		return false;
	}
	if(document.form.dsltmp_cfg_pppoe_passwd.value.length <= 0){
		alert("<%tcWebApi_get("String_Entry","WANJS9Text","s")%>.");
		document.form.dsltmp_cfg_pppoe_passwd.focus();
		return false;
	}
	if(document.form.dsltmp_cfg_pppoe_passwd.value != document.form.confirm_cfg_pppoe_passwd.value){
		alert("<% tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s") %>");
		document.form.dsltmp_cfg_pppoe_passwd.focus();
		return false;
	}
	if(!validate_string(document.form.dsltmp_cfg_pppoe_passwd)){
		alert("<%tcWebApi_get("String_Entry","WANJS20Text","s")%>");
		document.form.dsltmp_cfg_pppoe_passwd.focus();
		return false;
	}			
	
	document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
	document.form.submit();
	
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
					document.write("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>ADSL WAN (ATM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>" + prctl_str + "</span>, VPI/VCI: <span class='cfg_val'>" + vpi_val + "/" + vci_val + ", " + encap_str + "</span>");
					if(vlanid_val.length > 0)
						document.write(", <%tcWebApi_get("String_Entry","WANVLANIDText","s")%>: <span class='cfg_val'>" + vlanid_val + "</span>");
					if(iptv_atm_pvc_str.length > 0)
						document.writeln("<br>"+iptv_atm_pvc_str);
				}
				else //PTM
				{					
					document.write("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>VDSL WAN (PTM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>PPPoE</span>");
					if(vlanid_val.length > 0)
						document.write(", <%tcWebApi_get("String_Entry","WANVLANIDText","s")%>: <span class='cfg_val'>" + vlanid_val + "</span>");
					if(iptv_ptm_pvc_str.length > 0)
						document.writeln("<br>"+iptv_ptm_pvc_str);	
				}
			</script>
			<br>
			<% tcWebApi_Get("String_Entry", "PPP_cfg_usersel_desc", "s") %>
			</td>
		</tr>
	</table>

	<br>
	
	<div id="DE_ISP_note" style="display:none;">
		<span class="isp_note_title">Notice:</span> Bitte geben Sie diese Zugangsdaten in folgendem Format ein.</span>
		<div class="isp_note" id="Deutsche_Telekom" style="display:none;">	<!--ATM: 231 & PTM: 66, 67 -->
			Für Telekom ADSL und VDSL Anschlüsse<br>
			Bei einer 12-stelligen Zugangsnummer (Neue Zugangsdaten):<br>
			<div class="account_format">Anschlusskennung<span class="num1">Zugangsnummer</span><span class="num2">Mitbenutzernummer</span>@t-online.de</div><br>
			Bei einer Zugangsnummer mit weniger als 12 Stellen (Alte Zugangsdaten):<br>
			<div class="account_format">Anschlusskennung<span class="num1">Zugangsnummer</span>#<span class="num2">Mitbenutzernummer</span>@t-online.de</div>
		</div>	
		<div class="isp_note" id="Deutsche_1n1_ATM" style="display:none;">	<!-- ATM: 219 -->
			Bei einem 1&1 ADSL Anschluss
			<div class="account_format">1und1/<span class="num1">benutzername</span>@online.de (1&1 Internetzugangs-Kennung)</div>
		</div>
		<div class="isp_note" id="Deutsche_1n1_PTM" style="display:none;">	<!-- PTM: 62 -->
			Bei einem 1&1 VDSL Anschluss (basierend auf Telekom Technik) - Bitte ein „H“ voranstellen:
			<div class="account_format">H1und1/<span class="num1">benutzername</span>@online.de (1&1 Internetzugangs-Kennung)</div>
		</div>
		<div class="isp_note" id="Deutsche_NetCologne" style="display:none;">	<!-- ATM: 246, 247 -->
			<div class="account_format">nc-<span class="num1">username</span>@netcologne.de</div>
		</div>
	</div>
	
	<br>

	<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
		<tr>
			<th width="180"><% tcWebApi_Get("String_Entry", "PPPC_UserName_in", "s") %></th>
			<td>
				<input type="text" name="dsltmp_cfg_pppoe_username" value="" tabindex="1" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
			</td>
		</tr>
		<tr>
			<th width="180"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></th>
			<td>
				<input type="password" name="dsltmp_cfg_pppoe_passwd" value="" tabindex="2" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
			</td>
		</tr>
		<tr>
			<th width="180"><%tcWebApi_get("String_Entry","PASS_retype","s")%></th>
			<td>
				<input type="password" name="confirm_cfg_pppoe_passwd" value="" tabindex="3" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
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
		<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" tabindex="5" onclick="gotoprev(document.form);" class="button_gen_long">
		<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" tabindex="4" onclick="submitForm();" class="button_gen">
	</div>
</div>
</form>
</body>

<!--qis/QIS_ppp_cfg_tmp.asp-->
</html>
