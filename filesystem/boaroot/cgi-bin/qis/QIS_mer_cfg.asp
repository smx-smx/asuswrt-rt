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
<script type="text/Javascript" src="/general.js"></script>
<script type="text/Javascript" src="/jquery.js"></script>
<script type="text/javascript">

var $j = jQuery.noConflict();

var ISP_List;
var ISP_List_IPTV;
var RAW_ISP_List = [<% nvram_dump("ISP_List") %>];
var RAW_ISP_List_IPTV = [<% nvram_dump("ISP_List_IPTV") %>];
var RAW_ISP_PTM_List = [<% nvram_dump("ISP_PTM_List") %>];
var RAW_ISP_PTM_List_IPTV = [<% nvram_dump("ISP_PTM_List_IPTV") %>];
	
var detect_status = "<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>";
var vpi_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VPI","s") %>";
var vci_val = "<% tcWebApi_get("AutoPVC_Common","Detect_VCI","s") %>";
var encap_val = "<% tcWebApi_get("AutoPVC_Common","Detect_Encap","s") %>";
var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";
var encap_str = "LLC";
if (encap_val == "vc") encap_str = "VC-Mux";
var mac_addr_2g = "<%tcWebApi_get("WLan_Common","wl0_MacAddress","s")%>";
var mac_addr_last_3bytes = "\"" + mac_addr_2g.substring(9, 11) + mac_addr_2g.substring(12, 14) + mac_addr_2g.substring(15, 17) + "\"";
var model_name = "<%tcWebApi_get("SysInfo_Entry","ProductName","s")%>";


function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	parent.set_step("t2");

	if(encap_val == "vc")
		document.form.dsltmp_cfg_encap.value = "1";
	else
		document.form.dsltmp_cfg_encap.value = "0";

	/* Renjie: Remove this field, we will use Option 61 and 12 for UK Sky.
	if (vpi_val == "0" && vci_val == "40" && encap_val == "vc")
	{
		//UK ISP SKY Broadband, MER requires some tweak.
		//PPP Username and Password needs to be added into DHCP option 61.		
		document.getElementById("special_ISP_ppp").style.display = "";
	}
	*/


	if(wan_type == "PTM"){
		document.form.prev_page.value = "/cgi-bin/qis/QIS_PTM_manual_setting.asp";
		ISP_List = RAW_ISP_PTM_List;
		ISP_List_IPTV = RAW_ISP_PTM_List_IPTV;
	}
	else{ //ATM
		document.form.prev_page.value = "/cgi-bin/qis/QIS_manual_setting.asp";
		ISP_List = RAW_ISP_List;
		ISP_List_IPTV = RAW_ISP_List_IPTV;
	}	
		
	showHideDHCPoptList(false);	
	showHideIPTVList(false);
	if(haveIPTVService()) {
		showhide("iptv_manual_setting", 1);		
	}
	else {
		showhide("iptv_manual_setting", 0);
	}
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

	if (vpi_val == "0" && vci_val == "40" && encap_val == "vc")
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

	document.form.next_page.value = "/cgi-bin/qis/QIS_wireless.asp";
	if(document.form.dsltmp_cfg_iptv_enable.value == "1"){
		if(valid_ISP())
			setIptvNumPvc();
		else{
			document.form.ISP.focus();
			return false;	
		}		
	}	
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

	code +="<select id='ISP' name='ISP' class='input_option'>";
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
	}
	document.form.dsltmp_cfg_iptv_num_pvc.value = pvc_cnt.toString();
	document.form.dsltmp_cfg_iptv_rmvlan.value = ISP_List[isp_idx][11];
	document.form.dsltmp_cfg_iptv_mr.value = ISP_List[isp_idx][12];
	document.form.dsltmp_cfg_iptv_pvclist.value = dsltmp_cfg_iptv_pvclist_value;
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
<input type="hidden" name="dsltmp_cfg_iptv_rmvlan" value="">
<input type="hidden" name="dsltmp_cfg_iptv_mr" value="">
<input type="hidden" name="dsltmp_cfg_iptv_num_pvc" value="">
<input type="hidden" name="dsltmp_cfg_iptv_pvclist" value="">
<input type="hidden" name="dsltmp_cfg_iptv_enable" value="0">
<input type="hidden" name="dsltmp_transfer_mode" value="<% tcWebApi_get("AutoPVC_Common","Detect_XDSL","s") %>">
<input type="hidden" name="dsltmp_cfg_DHCPClient" id="dsltmp_cfg_DHCPClient" value="1">
<input type="hidden" name="dsltmp_cfg_dnsenable" id="dsltmp_cfg_dnsenable" value="1">
<input type="hidden" name="dsltmp_wanTypeOption" value="0">
<input type="hidden" name="with_wan_setting" value="1">
<input type="hidden" name="dsltmp_dhcp_hostname" value="">

<input type="hidden" name="dsltmp_dhcp_clientid_type" value="">
<input type="hidden" name="dsltmp_dhcp_clientid" value="">
<div class="QISmain">
<div>
<table width="730px">
	<tr>
		<td align="left">
			<span class="description_down">
			<script>
				if(wan_type == "PTM")
					document.write("Automatic IP");
				else //ATM
					document.write("MER");
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
<div>
<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
	<tr>
		<td align="left">
		<script>
			if(wan_type == "PTM")
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>VDSL WAN (PTM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>Automatic IP</span><br><br>");
			else //ATM
				document.writeln("<% tcWebApi_Get("String_Entry", "Transfer_Mode", "s") %>: <span class='cfg_val'>ADSL WAN (ATM)</span>, <% tcWebApi_Get("String_Entry", "L3F_x_ConnectionType_in", "s") %>: <span class='cfg_val'>MER</span>, VPI/VCI: <span class='cfg_val'>" + vpi_val + "/" + vci_val + ", " + encap_str + "</span><br><br>");
		</script>
		</td>
	</tr>
</table>
</div>
<!-- Renjie: Remove this field, we will use Option 61 and 12 for UK Sky.
<div>
<table id="special_ISP_ppp" style="display:none;margin-left:50px;">
  <tr>
	<th width="120" class="test_css"><span id="ppp_username1">SKY <% tcWebApi_Get("String_Entry", "HSDPAC_Username_in", "s") %>:</span></th>
	<td>
	  <span id="ppp_username2">
	  <input type="text" name="ppp_username" class="input_25_table" value="" maxlength="32" autocapitalization="off" autocomplete="off">
	  </span>
	</td>
  </tr>
  <tr>
	<th width="120" class="test_css"><span id="ppp_password1">SKY <% tcWebApi_Get("String_Entry", "HSDPAC_Password_in", "s") %>:</span></th>
	<td>
	  <span id="ppp_password2">
	  <input type="text" name="ppp_password" class="input_25_table" value="" maxlength="32" autocapitalization="off" autocomplete="off">
	  </span>
	</td>
  </tr>
</table>
</div>
-->
<%if tcWebApi_get("AutoPVC_Common","Detect_XDSL","h") = "PTM" then %>
<br>
<div>
<table id="tblsetting_2" class="QISform" width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<th width="120"><%tcWebApi_get("String_Entry","WANVLANIDText","s")%></th>
<td>
<input type="text" name="dsltmp_cfg_vlanid" value="" maxlength="4" class="input_6_table">
</td>
</tr>
</table>
</div>
<%end if%>

<br/>
<div>
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

<br/>
<div>
<table id="iptv_manual_setting" width="80%" border="0" align="left" cellpadding="3" cellspacing="0" style="margin-left:8%;">
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
	<input type="button" id="prevButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" onclick="gotoprev(document.form);" class="button_gen_long">
	<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" onclick="submitForm();" class="button_gen_long">
</div>
</form>
</body>

<!--qis/QIS_mer_cfg.asp-->
</html>
