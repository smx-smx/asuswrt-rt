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
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/Javascript" src="/jstz.min.js"></script>
<style>
#DE_ISP_note{
	font-size:12px;
	font-family:Arial, Helvetica, sans-serif;
	text-align:left;
	margin-left:75px;	
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

var $j = jQuery.noConflict();

var ISP_List;
var ISP_List_IPTV;
var RAW_ISP_List = [<% nvram_dump("ISP_List") %>];
var RAW_ISP_List_IPTV = [<% nvram_dump("ISP_List_IPTV") %>];
var RAW_ISP_PTM_List = [<% nvram_dump("ISP_PTM_List") %>];
var RAW_ISP_PTM_List_IPTV = [<% nvram_dump("ISP_PTM_List_IPTV") %>];
var detect_status = "<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>";
var prctl_str = "";
var gvt_note = "";
var vpi_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VPI","s") %>";
var vci_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VCI","s") %>";
var encap_val = "<% tcWebApi_get("AutoPVC_Common","Detect_Encap","s") %>";
var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";
if(detect_status == "pppoa")
	prctl_str = "PPPoA";
else
	prctl_str = "PPPoE";
var encap_str = "LLC";
if (encap_val == "vc") encap_str = "VC-Mux";

var is_Custom = "<% tcWebApi_get("SysInfo_Entry", "Customer", "s") %>";

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	parent.set_step("t2");

	if(is_Custom.toUpperCase() == "POL"){
				document.form.dsltmp_cfg_pppoe_username.value = "<% tcWebApi_get("Wan_PVC0","USERNAME", "s") %>";
				document.form.dsltmp_cfg_pppoe_passwd.value = "<% tcWebApi_get("Wan_PVC0","PASSWORD", "s") %>";
	}	
	
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

	if(wan_type == "PTM")
	{		
		document.form.prev_page.value = "/cgi-bin/qis/QIS_PTM_manual_setting.asp";
		ISP_List = RAW_ISP_PTM_List;
		ISP_List_IPTV = RAW_ISP_PTM_List_IPTV;
	}
	else //ATM
	{
		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp";
		ISP_List = RAW_ISP_List;
		ISP_List_IPTV = RAW_ISP_List_IPTV;
	}

	showHideIPTVList(false);
	if(haveIPTVService()) {
		showhide("iptv_manual_setting", 1);
	}
	else {
		showhide("iptv_manual_setting", 0);
	}
	
	DE_ISP_note_detect();
}

function DE_ISP_note_detect(){
	if(wan_type == "ATM" && vpi_val == "1" && vci_val == "32" && prctl_str == "PPPoE" && encap_str == "LLC"){
		//ATM, VPI: 1/ VCI: 32/ Protocol: PPPoE/ Encap: LLC 
		document.getElementById("DE_ISP_note").style.display="";
		document.getElementById("DE_ISP_title1").style.display="";
		document.getElementById("Deutsche_Telekom").style.display="";
		document.getElementById("Deutsche_1n1_ATM").style.display="";
		document.getElementById("DE_ISP_title2").style.display="none";
		document.getElementById("Deutsche_NetCologne").style.display="none";
			
	}
	else if(wan_type == "ATM" && vpi_val == "8" && vci_val == "35" && prctl_str == "PPPoE" && encap_str == "LLC"){
		//ATM, VPI: 8/ VCI: 35/ Protocol: PPPoE/ Encap: LLC	
		document.getElementById("DE_ISP_note").style.display="";
		document.getElementById("DE_ISP_title1").style.display="none";
		document.getElementById("Deutsche_Telekom").style.display="none";
		document.getElementById("Deutsche_1n1_ATM").style.display="none";
		document.getElementById("DE_ISP_title2").style.display="";
		document.getElementById("Deutsche_NetCologne").style.display="";
	}
	else
		document.getElementById("DE_ISP_note").style.display="none";
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
	if(!validate_string(document.form.dsltmp_cfg_pppoe_passwd)){
		alert("<%tcWebApi_get("String_Entry","WANJS20Text","s")%>");
		document.form.dsltmp_cfg_pppoe_passwd.focus();
		return false;
	}
	
		<%if tcWebApi_get("AutoPVC_Common","Detect_XDSL","h") = "PTM" then %>
		var tmp_vlanid = document.form.dsltmp_cfg_vlanid.value;
		if ( isNaN(tmp_vlanid) == true ) {
			alert('<%tcWebApi_get("String_Entry","WANVLANIDText","s")%> "' + tmp_vlanid + '" <%tcWebApi_get("String_Entry","Manual_Setting_JS_invalid","s")%>');
			document.form.dsltmp_cfg_vlanid.focus();
			return false;
		}
		<%end if%>
				
		if(document.form.dsltmp_cfg_iptv_enable.value == "1"){
			if(valid_ISP())
				setIptvNumPvc();
			else{
				document.form.ISP.focus();
				return false;	
			}	
		}	
		document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
		document.form.submit();
}

function haveIPTVService() {
	//compare detection result with isp list
	for(var i = 0; i < ISP_List.length; i++){
		if(wan_type == "ATM") {
			if(document.form.dsltmp_cfg_vpi.value == ISP_List[i][6]	//vpi
				&& document.form.dsltmp_cfg_vci.value == ISP_List[i][7]	//vci
				&& document.form.dsltmp_cfg_prctl.value == ISP_List[i][8]	//proto
				&& document.form.dsltmp_cfg_encap.value == ISP_List[i][9]	//encap
				&& (ISP_List[i][12] != "" || ISP_List[i][13] != "")	//mr, iptv idx
			) {
				return true;
			}
		}
		else {	//PTM
			if(document.form.dsltmp_cfg_prctl.value == ISP_List[i][8]	//proto
				&& document.form.dsltmp_cfg_vpi.value == ISP_List[i][10]	//vlan id
				&& (ISP_List[i][12] != "" || ISP_List[i][13] != "")	//mr, iptv idx
			) {
				return true;
			}
		}
	}

	return false;
}

function showHideIPTVList(iptv_enable) {
	if(iptv_enable.checked) {
		document.form.dsltmp_cfg_iptv_enable.value = "1";		
		document.getElementById("ISP_table").style.visibility = "visible";
		showCountryList();
		showISPList("");
	}
	else {
		document.form.dsltmp_cfg_iptv_enable.value = "0";
		document.getElementById("ISP_table").style.visibility = "hidden";
	}
}

function showCountryList(){
	var code = "";
	var showed_country = "";

	code +="<select name='country' onchange='showISPList(this.value);' class='input_option'>";
	code +="<option value='default'><%tcWebApi_get("String_Entry","Select_menu_default","s")%></option>";
	for(var i = 0; i < ISP_List.length; i++){
		if(ISP_List[i][5] == "")
			continue;

		if(document.form.dsltmp_cfg_prctl.value == ISP_List[i][8])  {	//proto
			if(wan_type == "ATM") {
				if(document.form.dsltmp_cfg_vpi.value == ISP_List[i][6]	//vpi
					&& document.form.dsltmp_cfg_vci.value == ISP_List[i][7]	//vci
					&& document.form.dsltmp_cfg_encap.value == ISP_List[i][9]	//encap
				) {
					if(showed_country != ISP_List[i][1]){
						code +="<option value='"+ISP_List[i][1];
						code +="'>"+ISP_List[i][2]+"</option>";
						showed_country = ISP_List[i][1];
					}
				}
			}
			else {	//PTM
				if(document.form.dsltmp_cfg_vlanid.value == ISP_List[i][10]) {	//vlan id
					if(showed_country != ISP_List[i][1]){
						code +="<option value='"+ISP_List[i][1];
						code +="'>"+ISP_List[i][2]+"</option>";
						showed_country = ISP_List[i][1];
					}
				}
			}
		}
	}
	//code +="<option value='NO'><%tcWebApi_get("String_Entry","Not_Listed","s")%></option>";
	code +="</select>";
	$("CountryList").innerHTML = code;
}

function showISPList(country){
	var code = "";
	var showed_isp = "";
	var first_element = 0;
	var sel_idx = 0;

	code +="<select id='ISP' name='ISP' onChange='' class='input_option'>";
	code +="<option value='default'><%tcWebApi_get("String_Entry","Select_menu_default","s")%></option>";
	for(var i = 0; i < ISP_List.length; i++){
		if(country == ISP_List[i][1]){
			if(ISP_List[i][5] == "")
				continue;
			if(ISP_List[i][12] == "" && ISP_List[i][13] == "")	//no iptv service
				continue;			

			if(document.form.dsltmp_cfg_prctl.value == ISP_List[i][8])  {	//proto
				if(wan_type == "ATM") {
					if(document.form.dsltmp_cfg_vpi.value == ISP_List[i][6]	//vpi
						&& document.form.dsltmp_cfg_vci.value == ISP_List[i][7]	//vci
						&& document.form.dsltmp_cfg_encap.value == ISP_List[i][9]	//encap
					) {
						if (first_element==0) {
							first_element=1;
							sel_idx = i;
							code +="<option value='"+ISP_List[i][0]+"' selected='selected'>"+ISP_List[i][4]+", "+ISP_List[i][5]+"</option>";
						}
						else {
							code +="<option value='"+ISP_List[i][0]+"'>"+ISP_List[i][4]+", "+ISP_List[i][5]+"</option>";
						}
					}
				}
				else {	//PTM
					if(document.form.dsltmp_cfg_vlanid.value == ISP_List[i][10]) {	//vlan id
						if (first_element==0) {
							first_element=1;
							code +="<option value='"+ISP_List[i][0]+"' selected='selected'>"+ISP_List[i][4]+", "+ISP_List[i][5]+"</option>";
						}
						else {
							code +="<option value='"+ISP_List[i][0]+"'>"+ISP_List[i][4]+", "+ISP_List[i][5]+"</option>";
						}
					}
				}
			}
			
		}
	}

	//code +="<option value='NO'><%tcWebApi_get("String_Entry","Not_Listed","s")%></option>";
	code +="</select>";
	$("ISPnServiceList").innerHTML = code;	
}

function valid_ISP(){
	if(document.getElementById("ISP").value == "default")
		return false;
	else
		return true;
}

function setIptvNumPvc() {
	var pvc_cnt = 0;
	var isp_idx = document.getElementById("ISP").value
	var iptv_idx = ISP_List[isp_idx][13];
	var dsltmp_cfg_iptv_pvclist_value = "";

	if (iptv_idx != "") {
		for(var i = 0; i < ISP_List_IPTV.length; i++){
			if (ISP_List_IPTV[i][0] == iptv_idx) {
				pvc_cnt++;
				dsltmp_cfg_iptv_pvclist_value +=
					"<" + ISP_List_IPTV[i][1]
					+ ">" + ISP_List_IPTV[i][2]
					+ ">" + ISP_List_IPTV[i][3]
					+ ">" + ISP_List_IPTV[i][4]
					+ ">" + ISP_List_IPTV[i][5]
			}
		}

		if(document.form.ISP.value.search("Deutsche Telekom") >= 0){
			document.form.dsltmp_auto_detect_bng_flag.value = 1;
		}
	}
	document.form.dsltmp_cfg_iptv_num_pvc.value = pvc_cnt.toString();
	document.form.dsltmp_cfg_iptv_rmvlan.value = ISP_List[isp_idx][11];
	document.form.dsltmp_cfg_iptv_mr.value = ISP_List[isp_idx][12];
	document.form.dsltmp_cfg_iptv_pvclist.value = dsltmp_cfg_iptv_pvclist_value;

}

/* password item show or not */
function pass_checked(obj){
	if(obj.name == "dsltmp_cfg_pppoe_passwd")
		switchType(obj, document.form.show_pass_1.checked, true);	
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
<input type="hidden" name="dsltmp_cfg_vlanid" value="<% tcWebApi_get("AutoPVC_Common","Detect_VLANID","s") %>">
<input type="hidden" name="dsltmp_cfg_iptv_rmvlan" value="">
<input type="hidden" name="dsltmp_cfg_iptv_mr" value="">
<input type="hidden" name="dsltmp_cfg_iptv_num_pvc" value="">
<input type="hidden" name="dsltmp_cfg_iptv_pvclist" value="">
<input type="hidden" name="dsltmp_cfg_iptv_enable" value="0">
<input type="hidden" name="dsltmp_transfer_mode" value="<% tcWebApi_get("AutoPVC_Common","Detect_XDSL","s") %>">
<input type="hidden" name="dsltmp_wanTypeOption" value="2">
<input type="hidden" name="with_wan_setting" value="1">
<input type="hidden" name="dsltmp_auto_detect_bng_flag" value="0">
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
<div>
<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
	<tr>
		<td align="left">
		<script>
			if(wan_type == "PTM")
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>VDSL WAN (PTM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>PPPoE</span><br><br>");
			else //ATM
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>ADSL WAN (ATM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>" + prctl_str + "</span>, VPI/VCI: <span class='cfg_val'>" + vpi_val + "/" + vci_val + ", " + encap_str + "</span><br><br>");
		</script>
		<% tcWebApi_Get("String_Entry", "PPP_cfg_usersel_desc", "s") %>
		<div id=hint_msg></div>
		</td>
	</tr>
</table>
</div>
<br>
<div>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
	<tr>
		<th width="180"><% tcWebApi_Get("String_Entry", "HSDPAC_Username_in", "s") %></th>
		<td>
			<input type="text" name="dsltmp_cfg_pppoe_username" value="" maxlength="64" tabindex="1" class="input_32_table" autocapitalization="off" autocomplete="off">
		</td>
	</tr>
	<tr>
		<th width="180"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></th>
		<td>
			<input type="password" name="dsltmp_cfg_pppoe_passwd" value="" maxlength="64" tabindex="2" class="input_32_table" autocapitalization="off" autocomplete="off">
			<div><input type="checkbox" name="show_pass_1" onclick="pass_checked(document.form.dsltmp_cfg_pppoe_passwd);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
		</td>
	</tr>	
<%if tcWebApi_get("AutoPVC_Common","Detect_XDSL","h") = "PTM" then %>
	<tr>
		<th width="180"><%tcWebApi_get("String_Entry","WANVLANIDText","s")%></th>
		<td>
			<input type="text" name="dsltmp_cfg_vlanid" value="" maxlength="4" class="input_6_table">
		</td>
	</tr>
<%end if%>
</table>
</div>
<br>
	<div id="DE_ISP_note" style="display:none;">
	<div id="DE_ISP_title1" style="display:none;"><span class="isp_note_title">Notice (specifically for Germany ISP Telekom / 1&1):</span><br> Bitte geben Sie diese Zugangsdaten in folgendem Format ein.</div>
		<div class="isp_note" id="Deutsche_Telekom" style="display:none;">
			Für Telekom ADSL und VDSL Anschlüsse<br>
			Bei einer 12-stelligen Zugangsnummer (Neue Zugangsdaten):<br>
			<div class="account_format">Anschlusskennung<span class="num1">Zugangsnummer</span><span class="num2">Mitbenutzernummer</span>@t-online.de</div><br>
			Bei einer Zugangsnummer mit weniger als 12 Stellen (Alte Zugangsdaten):<br>
			<div class="account_format">Anschlusskennung<span class="num1">Zugangsnummer</span>#<span class="num2">Mitbenutzernummer</span>@t-online.de</div><br>
		</div>			
		<div class="isp_note" id="Deutsche_1n1_ATM" style="display:none;">
			Bei einem 1&1 ADSL Anschluss:<br>
			<div class="account_format">1und1/<span class="num1">benutzername</span>@online.de (1&1 Internetzugangs-Kennung)</div>
		</div>
		<div id="DE_ISP_title2" style="display:none;"><span class="isp_note_title">Notice (specifically for Germany ISP NetCologne):</span><br> Bitte geben Sie diese Zugangsdaten in folgendem Format ein.</div>
		<div class="isp_note" id="Deutsche_NetCologne" style="display:none;">
			<div class="account_format">nc-<span class="num1">username</span>@netcologne.de</div>
		</div>
	</div>
	<br>
<div>
<table id="iptv_manual_setting" width="92%" border="0" align="left" cellpadding="3" cellspacing="0" style="margin-left:8%;">
		<tr>
			<td>
				<input type="checkbox" id="specialisp" name="specialisp" onclick="showHideIPTVList(this);">
				<label for="specialisp">
					<span class="QISGeneralFont" style="margin-left:0px;font-style:normal;color:#66CCFF;font-size:12px;font-weight:bolder;"><%tcWebApi_get("String_Entry","PPPC_x_HostNameForISP_sn","s")%> ( IPTV Service )</span>
				</label>
			</td>
		</tr>	
</table>	
</div>

<div style="margin-left:-90px;">
<table id="ISP_table" class="FormTable" width="475px" border="0" align="center" cellpadding="3" cellspacing="0">
	<tbody>		
		<tr id="Country_tr">
			<th width="40%"><%tcWebApi_get("String_Entry","Manual_Setting_contry","s")%></th>
			<td>
				<div id="CountryList"></div>
			</td>
		</tr>
		<!--tr id="City_tr">
			<th width="40%"><%tcWebApi_get("String_Entry","Manual_Setting_city","s")%></th>
			<td>
				<div id="CityList"></div>
			</td>
		</tr-->
		<tr id="ISP_tr">
			<th width="40%"><%tcWebApi_get("String_Entry","HSDPAC_ISP_in","s")%> & <%tcWebApi_get("String_Entry","qis_service","s")%></th>
			<td>
				<div id="ISPnServiceList"></div>
			</td>
		</tr>
		<!--tr id="Service_tr">
			<th width="40%"><%tcWebApi_get("String_Entry","qis_service","s")%></th>
			<td>
				<div id="Service"></div>
			</td>
		</tr-->
	</tbody>
</table>
</div>
</div>
<div class="apply_gen" style="margin-top:20px">
	<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" onclick="gotoprev(document.form);" class="button_gen_long" >
	<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" onclick="submitForm();" class="button_gen_long">
</div>
</form>
</body>

<!--qis/QIS_ppp_cfg.asp-->
</html>
