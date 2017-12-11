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

<% login_state_hook(); %>

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
		else {	//3
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

function IPTV_pppoe_detect(){
	if(dsltmp_cfg_iptv_pvclist != "" && dsltmp_cfg_iptv_pvclist_col[2] == "0"){
		document.getElementById("tblsetting_IPTV_PPPoE").style.display="";
	}
	else
		document.getElementById("tblsetting_IPTV_PPPoE").style.display="none";
}
function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	parent.set_step("t2");
	
	DE_ISP_detect();
	IPTV_pppoe_detect();
	
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

function DE_ISP_detect(){
	//Germany ISP setting note start-------------------
	if(country_str == "Germany" 
		&& (ispname_str == "1&1" || 
				ispname_str == "Congstar" || /*Both in ATM/PTM*/
				ispname_str == "Deutsche Telekom" || ispname_str == "Deutsche Telekom (ohne Entertain)" || ispname_str == "Deutsche Telekom (mit Entertain)" || /*1st is ATM, else PTM*/
				ispname_str.search("GMX") >= 0 || /*Both in ATM/PTM.*/
				ispname_str == "KielNET" || ispname_str == "KielNET DSL" || /*1st is ATM, 2nd is PTM*/
				ispname_str == "NetAachen" ||	ispname_str == "NetAachen (VLAN ID 10)" || 	/*1st is ATM, 2nd is PTM*/
				ispname_str == "NetCologne" || ispname_str == "NetCologne (VLAN ID 10)" || 	/*1st is ATM, 2nd is PTM*/
				ispname_str == "1&1 (Telekom VDSL Resale Anschluss)"	/*PTM*/
				)
	){		
		document.getElementById("tblsetting_2").style.display="none";
		document.getElementById("tblsetting_DE_ATM").style.display="";
		document.getElementById("tr_DE_password").style.display="";
		document.getElementById("tr_DE_hwaddr").style.display="";
		//document.getElementById("DE_ISP_note").style.display="";		
		//document.getElementById("Deutsche_Telekom").style.display="none";		
		//document.getElementById("Deutsche_1n1_PTM").style.display="none";
		//document.getElementById("Deutsche_NetCologne").style.display="none";
		if(ispname_str == "1&1"){
			document.getElementById("tr_DE_username").style.display="";
			document.getElementById("ATM_1n1").style.display="";
		}
		if(ispname_str == "Congstar"){
			document.getElementById("tr_DE_username").style.display="";
			document.getElementById("ATM_PTM_Congstar").style.display="";
		}
		if(ispname_str == "Deutsche Telekom" || ispname_str == "Deutsche Telekom (ohne Entertain)" || ispname_str == "Deutsche Telekom (mit Entertain)"){
			document.getElementById("tr_DE_Deutsche_id").style.display="";
			document.getElementById("tr_DE_Deutsche_AN").style.display="";
			document.getElementById("tr_DE_Deutsche_Co").style.display="";
		}		
		if(ispname_str.search("GMX") >= 0){
			document.getElementById("tr_DE_username").style.display="";
			document.getElementById("ATM_PTM_GMX").style.display="";
		}
		if(ispname_str == "KielNET" || ispname_str == "KielNET DSL"){
			document.getElementById("tr_DE_username").style.display="";
			document.getElementById("default_username").style.display="";
			document.form.DE_pppoe_username.value = "acs";
			document.form.pppoe_password_DE.value = "acs";
		}
		
		if(ispname_str == "NetAachen" || ispname_str == "NetAachen (VLAN ID 10)" ||
			ispname_str == "NetCologne" || ispname_str == "NetCologne (VLAN ID 10)"
		){
			document.getElementById("tr_DE_username").style.display="";
			document.getElementById("default_username").style.display="";
			document.form.DE_pppoe_username.value = "nc@nc";
			document.form.pppoe_password_DE.value = "nc";			
		}	
			
		if(ispname_str == "1&1 (Telekom VDSL Resale Anschluss)"){	//PTM
			document.getElementById("tr_DE_username").style.display="";
			document.getElementById("PTM_1n1").style.display="";
		}		
	}
	
	//Germany ISP setting note end-------------------	
}

function validForm(){
	if(document.getElementById("tblsetting_2").style.display != "none"){

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
		
		if(!validate_string(document.form.dsltmp_cfg_pppoe_passwd)){
			alert("<%tcWebApi_get("String_Entry","WANJS20Text","s")%>");
			document.form.dsltmp_cfg_pppoe_passwd.focus();
			return false;
		}
	}

	return true;
}

function submitForm(){
	if(validForm()){
		
		if(country_str == "Germany"){ 			
		
			if(ispname_str == "1&1"){//ATM
				document.form.dsltmp_cfg_pppoe_username.value = "1und1/"+ document.form.pppoe_username_1n1_ATM.value +"@online.de";
				document.form.dsltmp_cfg_pppoe_passwd.value = document.form.pppoe_password_DE.value;
			}
					
			if(ispname_str == "Congstar"){	//Both in ATM/PTM
				document.form.dsltmp_cfg_pppoe_username.value = document.form.pppoe_username_Congstar.value +"@congstar.de";
				document.form.dsltmp_cfg_pppoe_passwd.value = document.form.pppoe_password_DE.value;
			}	
					
			if(ispname_str == "Deutsche Telekom" || ispname_str == "Deutsche Telekom (ohne Entertain)" || ispname_str == "Deutsche Telekom (mit Entertain)"){	/*1st is ATM, else PTM*/
				if(document.form.pppoe_AN_Deutsche.value.length < 12)	//length < 12, Add '#' symbol
					document.form.dsltmp_cfg_pppoe_username.value = document.form.pppoe_id_Deutsche.value + document.form.pppoe_AN_Deutsche.value +"#"+ document.form.pppoe_Co_Deutsche.value +"@t-online.de";
				else
					document.form.dsltmp_cfg_pppoe_username.value = document.form.pppoe_id_Deutsche.value + document.form.pppoe_AN_Deutsche.value + document.form.pppoe_Co_Deutsche.value +"@t-online.de";
				document.form.dsltmp_cfg_pppoe_passwd.value = document.form.pppoe_password_DE.value;
			}
					
			if(ispname_str.search("GMX") >= 0){	//Both in ATM/PTM
				document.form.dsltmp_cfg_pppoe_username.value = "GMX/"+ document.form.pppoe_username_GMX.value +"@online.de";
				document.form.dsltmp_cfg_pppoe_passwd.value = document.form.pppoe_password_DE.value;
			}
					
			if(ispname_str == "KielNET" || ispname_str == "KielNET DSL" ||		/*1st is ATM, 2nd is PTM*/
				ispname_str == "NetAachen" || ispname_str == "NetAachen (VLAN ID 10)" ||	/*1st is ATM, 2nd is PTM*/
				ispname_str == "NetCologne" || ispname_str == "NetCologne (VLAN ID 10)"	/*1st is ATM, 2nd is PTM*/
			){
				document.form.dsltmp_cfg_pppoe_username.value = document.form.DE_pppoe_username.value;
				document.form.dsltmp_cfg_pppoe_passwd.value = document.form.pppoe_password_DE.value;
			}
					
			if(ispname_str == "1&1 (Telekom VDSL Resale Anschluss)"){	//PTM
				document.form.dsltmp_cfg_pppoe_username.value = "H1und1/"+ document.form.pppoe_username_1n1_PTM.value +"@online.de";
				document.form.dsltmp_cfg_pppoe_passwd.value = document.form.pppoe_password_DE.value;
			}
										
		}
	
		document.form.dsltmp_cfg_hwaddr.value = document.form.wan_hwaddr_DE.value;
		document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
		//alert(document.form.dsltmp_cfg_pppoe_username.value +" ; "+ document.form.dsltmp_cfg_pppoe_passwd.value +" ; "+ document.form.dsltmp_cfg_hwaddr.value);
		document.form.submit();
	}
}

function showMAC(){
	var tempMAC = "";	
	document.form.wan_hwaddr_DE.value = login_mac_str();
	document.form.wan_hwaddr_DE.focus();
}

/* password item show or not */
function pass_checked(obj){
	if(obj.name == "dsltmp_cfg_pppoe_passwd")
		switchType(obj, document.form.show_pass_1.checked, true);	
	if(obj.name == "dsltmp_cfg_iptv_passwd")
		switchType(obj, document.form.show_pass_2.checked, true);		
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
	
	<!--div id="DE_ISP_note" style="display:none;">
		<span class="isp_note_title">Notice:</span> Bitte geben Sie diese Zugangsdaten in folgendem Format ein.</span>
		<div class="isp_note" id="Deutsche_Telekom" style="display:none;">
			Für Telekom ADSL und VDSL Anschlüsse<br>
			Bei einer 12-stelligen Zugangsnummer (Neue Zugangsdaten):<br>
			<div class="account_format">Anschlusskennung<span class="num1">Zugangsnummer</span><span class="num2">Mitbenutzernummer</span>@t-online.de</div><br>
			Bei einer Zugangsnummer mit weniger als 12 Stellen (Alte Zugangsdaten):<br>
			<div class="account_format">Anschlusskennung<span class="num1">Zugangsnummer</span>#<span class="num2">Mitbenutzernummer</span>@t-online.de</div>
		</div>	
		<div class="isp_note" id="Deutsche_1n1_ATM" style="display:none;">
			Bei einem 1&1 ADSL Anschluss
			<div class="account_format">1und1/<span class="num1">benutzername</span>@online.de (1&1 Internetzugangs-Kennung)</div>
		</div>
		<div class="isp_note" id="Deutsche_1n1_PTM" style="display:none;">
			Bei einem 1&1 VDSL Anschluss (basierend auf Telekom Technik) - Bitte ein „H“ voranstellen:
			<div class="account_format">H1und1/<span class="num1">benutzername</span>@online.de (1&1 Internetzugangs-Kennung)</div>
		</div>
		<div class="isp_note" id="Deutsche_NetCologne" style="display:none;">
			<div class="account_format">nc-<span class="num1">username</span>@netcologne.de</div>
		</div>
	</div-->
	
	<br>

	<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
		<tr>
			<th width="180"><% tcWebApi_Get("String_Entry", "HSDPAC_Username_in", "s") %></th>
			<td>
				<input type="text" name="dsltmp_cfg_pppoe_username" value="" tabindex="1" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
			</td>
		</tr>
		<tr>
			<th width="180"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></th>
			<td>
				<input type="password" name="dsltmp_cfg_pppoe_passwd" value="" tabindex="2" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
				<div><input type="checkbox" name="show_pass_1" onclick="pass_checked(document.form.dsltmp_cfg_pppoe_passwd);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
			</td>
		</tr>		
		<tr style="display:none;">
			<th width="120"><span id="mac_addr1" style="display:none"><% tcWebApi_Get("String_Entry", "MAC_Address", "s") %></span></th>
			<td>
				<span id="mac_addr2" style="display:none">
					<input type='text' name='dsltmp_cfg_hwaddr' maxlength="12" size='12'>&nbsp;<% tcWebApi_Get("String_Entry", "PPPC_x_MacAddressForISP_id", "s") %>
				</span>
			</td>
		</tr>
	</table>

	<!-- Germany ISP setting start-->
	<table id="tblsetting_DE_ATM" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0" style="margin-top:10px;">
  		<tr id="tr_DE_username" style="display:none;">
			<th width="120"><% tcWebApi_Get("String_Entry", "HSDPAC_Username_in", "s") %></th>
			<td>
				<span id="ATM_1n1" style="display:none;">
	  				1und1/<input type="text" name="pppoe_username_1n1_ATM" class="input_22_table" value="" maxlength="64" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">@online.de
	  			</span>
	  			<span id="ATM_PTM_Congstar" style="display:none;">
	  				<input type="text" name="pppoe_username_Congstar" class="input_22_table" value="" maxlength="64" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">@congstar.de
	  			</span>
	  			<span id="ATM_PTM_GMX" style="display:none;">
	  				GMX/<input type="text" name="pppoe_username_GMX" class="input_22_table" value="" maxlength="64" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">@online.de
	  			</span>
	  			<span id="PTM_1n1" style="display:none;">
	  				H1und1/<input type="text" name="pppoe_username_1n1_PTM" class="input_22_table" value="" maxlength="64" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">@online.de
	  			</span>
	  			<span id="default_username" style="display:none;">
	  				<input type="text" name="DE_pppoe_username" class="input_32_table" value="" maxlength="64" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">
	  			</span>
			</td>		
  		</tr>
  
  		<tr id="tr_DE_Deutsche_id" style="display:none;">
  			<th><% tcWebApi_Get("String_Entry", "ppp_ID_Connection", "s") %></th>
  			<td>
	  			<input type="text" name="pppoe_id_Deutsche" class="input_18_table" value="" maxlength="12" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">
  			</td>
  		</tr>	
  		<tr id="tr_DE_Deutsche_AN" style="display:none;">
  			<th><% tcWebApi_Get("String_Entry", "ppp_Access_Number", "s") %></th>
  			<td>
	  			<input type="text" name="pppoe_AN_Deutsche" class="input_18_table" value="" maxlength="12" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">
  			</td>
  		</tr>
  		<tr id="tr_DE_Deutsche_Co" style="display:none;">
  			<th><% tcWebApi_Get("String_Entry", "ppp_CoUser_Number", "s") %></th>
  			<td>
	  			<input type="text" name="pppoe_Co_Deutsche" class="input_6_table" value="" maxlength="4" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">
  			</td>
  		</tr>
  
  		<tr id="tr_DE_password" style="display:none;">
			<th width="120"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></th>
			<td>
	  			<input type="text" name="pppoe_password_DE" class="input_32_table" value="" maxlength="32" autocomplete="off" autocorrect="off" autocapitalize="off" autocapitalize="off">
			</td>
  		</tr>
  		<tr id="tr_DE_hwaddr" style="display:none;">
			<th width="120"><% tcWebApi_Get("String_Entry", "BOP_isp_mac_item", "s") %></th>
			<td class="QISformtd">
				<input type="text" name="wan_hwaddr_DE" onkeypress="return validator.isHWAddr(this,event);" style="height:25px;" class="input_20_table" size="36" maxlength="17" value="" autocorrect="off" autocapitalize="off">
				<input type="button" class="button_gen_long" onclick="showMAC();" value="<% tcWebApi_Get("String_Entry", "BOP_isp_MACclone", "s") %>">
			</td>
		</tr>
	</table>	
	<!-- Germany ISP setting end-->	
	
	<!-- IPTV pppoe username/passwd start-->	
	<table id="tblsetting_IPTV_PPPoE" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0" style="margin-top:10px;">
		<thead>
		<tr>
			<th colspan="2">IPTV PVC Account Setting</th>				
		</tr>
		</thead>
		<tr>
			<th width="180"><% tcWebApi_Get("String_Entry", "HSDPAC_Username_in", "s") %></th>
			<td>
				<input type="text" name="dsltmp_cfg_iptv_username" value="" tabindex="4" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">	
			</td>	
		</tr>
		<tr>
			<th width="180"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></th>
			<td>
				<input type="password" name="dsltmp_cfg_iptv_passwd" value="" tabindex="5" maxlength="64" class="input_32_table" autocapitalization="off" autocomplete="off">
				<div><input type="checkbox" name="show_pass_2" onclick="pass_checked(document.form.dsltmp_cfg_iptv_passwd);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
			</td>	
		</tr>
	</table>	
	<!-- IPTV pppoe username/passwd end-->	

	<div class="apply_gen" style="margin-top:30px">
		<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" tabindex="7" onclick="gotoprev(document.form);" class="button_gen_long">
		<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" tabindex="6" onclick="submitForm();" class="button_gen_long">
	</div>
</div>
</form>
</body>

<!--qis/QIS_ppp_cfg_tmp.asp-->
</html>
