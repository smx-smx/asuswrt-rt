
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_PTM_manual_setting.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript">
var country_code = "";
var selected_country = "";
var ISP_List = [<% nvram_dump("ISP_PTM_List") %>];
var ISP_List_IPTV = [<% nvram_dump("ISP_PTM_List_IPTV") %>];
var ru_idx_start = 0;
var x_Setting = "<%tcWebApi_get("SysInfo_Entry","x_Setting","s")%>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";

function $(){
	var elements = new Array();
	for(var i = 0; i < arguments.length; ++i){
		var element = arguments[i];
		if(typeof element == 'string')
			element = document.getElementById(element);
		if(arguments.length == 1)
			return element;
		elements.push(element);
	}
	return elements;
}

function setIptvNumPvc() {
	var pvc_cnt = 0;
	var tmp = document.form.dsltmp_cfg_iptv_idx.value;
	var dsltmp_cfg_iptv_pvclist_value = "";

	if (tmp != "") {
		for(var i = 0; i < ISP_List_IPTV.length; i++){
		if (ISP_List_IPTV[i][0] == tmp) {
				pvc_cnt++;
				dsltmp_cfg_iptv_pvclist_value +=
					"<" + ISP_List_IPTV[i][1]
					+ ">" + ISP_List_IPTV[i][2]
					+ ">" + ISP_List_IPTV[i][3]
					+ ">" + ISP_List_IPTV[i][4]
					+ ">" + ISP_List_IPTV[i][5]
			}
		}
		
		document.form.dsltmp_cfg_iptv_enable.value = "1";
		document.form.dsltmp_cfg_iptv_num_pvc.value = pvc_cnt.toString();
		document.form.dsltmp_cfg_iptv_pvclist.value = dsltmp_cfg_iptv_pvclist_value;
	}	
}

function showCountryList(o){
	var code = "";
	var showed_country = "";
	code +="<select name='country' onchange='showAllList(this.value);' tabindex='1' class='input_option'>";
	code +="<option value='default'><%tcWebApi_get("String_Entry","Select_menu_default","s")%></option>";
	for(var i = 0; i < ISP_List.length; i++){
		if(showed_country != ISP_List[i][1]){
			code +="<option value='"+ISP_List[i][1];
			if(o == ISP_List[i][1]){
				code +="' selected='selected'";
			}
			code +="'>"+ISP_List[i][2]+"</option>";
			showed_country = ISP_List[i][1];
		}
	}
	code +="<option value='NO'><%tcWebApi_get("String_Entry","Not_Listed","s")%></option>";
	code +="</select>";
	$("CountryList").innerHTML = code;
}
function showCityList(o){
	var code = "";
	var showedCity = "";
	code +="<select name='city' onchange='showRussiaISPList(this.value);' tabindex='2' class='input_option'>";
	for(var i = 0; i < ISP_List.length; i++){
		if((showedCity != ISP_List[i][3]) && (o == ISP_List[i][1])){
			code +="<option value='"+ISP_List[i][3]+"'>"+ISP_List[i][3]+"</option>";
			showedCity = ISP_List[i][3];
		}
	}
	code +="</select>";
	$("CityList").innerHTML = code;
}
function showNomoISPList(country){
	var code = "";
	var showed_isp = "";
	var first_element = 0;

	code +="<select id='ISP' name='ISP' onChange='ShowPVC(this.value); showNomoISPServiceByIdx(this.value); showSpecificSetting(this.value);' tabindex='3' class='input_option'>";
	for(var i = 0; i < ISP_List.length; i++){
		if(country == ISP_List[i][1]){
			if(showed_isp == ISP_List[i][4]){	//same isp internet setting with additional service
				continue;
			}
			else {
				showed_isp = ISP_List[i][4];
			}
			if (first_element==0) {
				first_element=1;
				ShowPVC(i);
				code +="<option value='"+ISP_List[i][0]+"' selected='selected'>"+ISP_List[i][4]+"</option>";
			}
			else {
				code +="<option value='"+ISP_List[i][0]+"'>"+ISP_List[i][4]+"</option>";
			}
		}
	}

	code +="<option value='NO'><%tcWebApi_get("String_Entry","Not_Listed","s")%></option>";
	code +="</select>";
	$("ISPList").innerHTML = code;
}

function showNomoISPServiceByIdx(idx) {
	$("Service_tr").style.display="none";

	var code = "";
	var first_element = 0;
	var sel_idx = 0;

	code +="<select id='ISPSVC' name='ISPSVC' onChange='' class='input_option'>";
	for(var i = idx; i < ISP_List.length; i++){
		if(ISP_List[idx][4] != ISP_List[i][4])	//only show this isp
			break;
		if(ISP_List[i][5] == "")
			continue;

		if (first_element == 0) {
			first_element = 1;
			sel_idx = i;
			code +="<option value='"+ISP_List[i][0]+"' selected='selected'>"+ISP_List[i][5]+"</option>";
		}
		else {
			code +="<option value='"+ISP_List[i][0]+"'>"+ISP_List[i][5]+"</option>";
		}
	}
	code +="</select>";	

	if(first_element != 0) {
		$("Service_tr").style.display="";
		$("Service").innerHTML = code;		
	}
}

function showRussiaISPList(o){
	ISPlocatedCity = o;
	hidePVCInfo(1);
	var code = "";
	var First_ISP= "";
	var showed_ISP = "";
	code +="<select id='ISP' name='ISP' onChange='ShowPVC(this.value); showRussiaISPServiceByIdx(ISPlocatedCity, this.value)' class='input_option'>";
	var first_element = 0;
	for(var i = ru_idx_start; i < ISP_List.length; i++){
		if((showed_ISP != ISP_List[i][4]) && (o == ISP_List[i][3])){
			if (first_element==0) {
				first_element=1;
				ShowPVC(i);
				code +="<option value='"+ISP_List[i][0]+"' selected='selected'>"+ISP_List[i][4]+"</option>";
			}
			else {
				code +="<option value='"+ISP_List[i][0]+"'>"+ISP_List[i][4]+"</option>";
			}
			if(First_ISP == "")
			First_ISP = ISP_List[i][4];
			showed_ISP = ISP_List[i][4];
		}
	}
	code +="<option value='NO'><%tcWebApi_get("String_Entry","Not_Listed","s")%></option>";
	code +="</select>";
	$("ISPList").innerHTML = code;
	showRussiaISPService(ISPlocatedCity, First_ISP);
}
function showRussiaISPServiceByIdx(c, idx){
	if (idx=="NO"){
		$("Service_tr").style.display="none";
	}
	else{
		var isp_str = "";
		for(var i = ru_idx_start; i < ISP_List.length; i++){
			if((idx == ISP_List[i][0])){
				isp_str = ISP_List[i][4];
			}
		}
		$("Service_tr").style.display="";
		var code = "";
		code +="<select id='ISPSVC' name='ISPSVC' onChange='' tabindex='4' class='input_option'>";
		var first_element = 0;
		var sel_idx = 0;
		for(var i = ru_idx_start; i < ISP_List.length; i++){
			if((c == ISP_List[i][3]) && (isp_str == ISP_List[i][4])){
				if (first_element == 0)
				{
					first_element = 1;
					sel_idx = i;
				}
				code +="<option value='"+ISP_List[i][0]+"'>"+ISP_List[i][5]+"</option>";
			}
		}
		code +="</select>";

		$("Service").innerHTML = code;
	}
}
function showRussiaISPService(c, o){
	if (o=="NO")
		$("Service_tr").style.display="none";
	else {
		$("Service_tr").style.display="";
		var code = "";
		code +="<select id='ISPSVC' name='ISPSVC' onChange='' tabindex='4' class='input_option'>";
		var first_element = 0;
		var sel_idx = 0;
		for(var i = ru_idx_start; i < ISP_List.length; i++){
			if((c == ISP_List[i][3]) && (o == ISP_List[i][4])){
				if (first_element == 0)
				{
					first_element = 1;
					sel_idx = i;
				}
				code +="<option value='"+ISP_List[i][0]+"'>"+ISP_List[i][5]+"</option>";
			}
		}
		code +="</select>";

		$("Service").innerHTML = code;		
	}
}
function showAllList(o){
	var i;
	selected_country = o;
	if(0){//if(o == "Russia"){	//Do not use since no country information in PTM list currently
		$("City_tr").style.display="";
		$("Service_tr").style.display="";
		hideCityList(0);
		showCityList(o);
		if (ru_idx_start == 0) {
			for(i=0; i< ISP_List.length; i++) {
				if(ISP_List[i][1]=="Russia")
					break;
			}
			ru_idx_start = i;
		}
		showRussiaISPList(ISP_List[ru_idx_start][3]);
		showRussiaISPService(ISP_List[ru_idx_start][3], ISP_List[ru_idx_start][4]);
		hidePVCInfo(1);
	}
	else{
		$("City_tr").style.display="none";
		$("Service_tr").style.display="none";
		hideCityList(1);
		showNomoISPList(o);
		for(i=0; i< ISP_List.length; i++) {
			if(ISP_List[i][1] == o) {
				showNomoISPServiceByIdx(i);
				showSpecificSetting(i);
				break;
			}
		}
	}
	if (o=="NO")
		hidePVCInfo(0);
	else
		hidePVCInfo(1);
}

function hideCityList(hide) {
	var status = 'visible';
	if ( hide == 1 )
		status = 'hidden';
	if (document.getElementById) // DOM3 = IE5, NS6
		document.getElementById("CityList").style.visibility = status;
	else {
		if (document.layers) { // Netscape 4
		} else // IE 4
			document.all.CityList.style.visibility = status;
	}
}

function hidePVCInfo(hide) {
	var status = '';
	if ( hide == 1 )
		status = 'none';
	if (document.getElementById) // DOM3 = IE5, NS6
		document.getElementById('vccInfo').style.display = status;
	else {
		if (document.layers) { // Netscape 4
			if ( hide == 1 ) {
				//document.forms.user_vpi.value = "";
				//document.forms.user_vci.value = "";
				document.forms.user_prctl.value = 0;
				//document.forms.user_encap.value = 0;
				document.forms.user_vlanid.value = "";
			}
		} else // IE 4
			document.all.vccInfo.style.display = status;
	}
}
function hidewarn(hide) {
	var status = 'visible';
	if ( hide == 1 )
		status = 'hidden';
	if (document.getElementById) // DOM3 = IE5, NS6
		document.getElementById('warnning').style.visibility = status;
	else {
		if (document.layers) { // Netscape 4
		} else // IE 4
			document.all.warning.style.visibility = status;
	}
}
function ShowPVC(idx) {
	if ( idx=='NO' )
		hidePVCInfo(0);
	else
		hidePVCInfo(1);
}

function QIS_menual_setting_load_body() {
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","Manual_Setting_btn","s")%>";
	parent.set_step("t2");
	
	hidePVCInfo(1);
	if(country_code=="")
		country_code = "default";
	showCountryList(country_code);
	showAllList(country_code);
}

function submit_page(){
	setIptvNumPvc();
	document.form.submit();
}

function btnNext() {
	var connection_type = 0;
	if(document.form.country.value=='default'){
                        alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
                        document.form.country.focus();
                        return false;           
        }
        else if ( document.form.country.value=='NO'|| document.form.ISP.value=='NO' ){
		var tmp_vlanid = document.form.user_vlanid.value;
		if ( isNaN(tmp_vlanid) == true ) {
			alert('<%tcWebApi_get("String_Entry","WANVLANIDText","s")%> "' + tmp_vlanid + '" <%tcWebApi_get("String_Entry","Manual_Setting_JS_invalid","s")%>');
			document.form.user_vlanid.focus();
			return false;
		}
		var vlanid = parseInt(tmp_vlanid);
		if ( vlanid < 0 || vlanid > 4095 ) {
			alert('<%tcWebApi_get("String_Entry","WANVLANIDText","s")%> "' + tmp_vlanid + '" is out of range [0-4095].');
			document.form.user_vlanid.focus();
			document.form.user_vlanid.select();
			return false;
		}
		connection_type = document.form.user_prctl.value;
		document.form.dsltmp_cfg_prctl.value = document.form.user_prctl.value;
		document.form.dsltmp_cfg_vlanid.value = document.form.user_vlanid.value;
		document.form.dsltmp_cfg_iptv_idx.value = "";
		document.form.dsltmp_cfg_ispname.value = "";
		document.form.dsltmp_cfg_ispservice.value = "";
		document.form.dsltmp_cfg_country.value = "";
	}
	else {
		var isp_idx = 0
		if(0) {	//if (selected_country == "Russia") {	//Do not use since no country information in PTM list currently
			isp_idx = document.getElementById("ISPSVC").value;
		}
		else {
			isp_idx = document.getElementById("ISP").value;
			if(ISP_List[isp_idx][5] != "")
				isp_idx = document.getElementById("ISPSVC").value;
		}
		connection_type = ISP_List[isp_idx][8];
		document.form.dsltmp_cfg_country.value = ISP_List[isp_idx][1];
		document.form.dsltmp_cfg_ispname.value = ISP_List[isp_idx][4];
		document.form.dsltmp_cfg_ispservice.value = ISP_List[isp_idx][5];
		document.form.dsltmp_cfg_prctl.value = ISP_List[isp_idx][8];
		document.form.dsltmp_cfg_vlanid.value = ISP_List[isp_idx][10];
		document.form.dsltmp_cfg_iptv_rmvlan.value = ISP_List[isp_idx][11];
		document.form.dsltmp_cfg_iptv_mr.value = ISP_List[isp_idx][12];
		document.form.dsltmp_cfg_iptv_idx.value = ISP_List[isp_idx][13];
	}

	//Specific setting
	if( document.form.dsltmp_cfg_country.value=='Thailand'
		&& document.form.dsltmp_cfg_ispname.value=='3BB 15M/1.5M'
	){
		document.form.dsltmp_cfg_th3bb.value = "1";	//flag
		document.form.dsltmp_transfer_mode.value = "ATM";
		document.form.dsltmp_cfg_modulation.value = "ADSL2+";
		document.form.dsltmp_cfg_annex.value = "ANNEX M";
		document.form.dsltmp_cfg_vpi.value = ISP_List[isp_idx][6];
		document.form.dsltmp_cfg_vci.value = ISP_List[isp_idx][7];
		document.form.dsltmp_cfg_encap.value = ISP_List[isp_idx][9];
	}
	if( document.form.country.value=='Germany'
		&& document.form.dsltmp_cfg_vdslprofile.value == "0"
	) {
		document.form.dsltmp_cfg_de17a.value = "1";	//flag
		document.form.dsltmp_cfg_vdslprofile.value = "1";
	}
	if(document.form.dsltmp_cfg_ispname.value.search("Deutsche Telekom") >= 0 && document.form.dsltmp_cfg_iptv_idx.value > 0){
		document.form.dsltmp_auto_detect_bng_flag.value = 1;
	}
	if( (document.form.dsltmp_cfg_country.value=='United Kingdom'
			&& (document.form.dsltmp_cfg_ispname.value=='Sky - Fibre Broadband'
			|| document.form.dsltmp_cfg_ispname.value=='TalkTalk (Automatic IP)')
			)
		|| (document.form.dsltmp_cfg_country.value=='India'
			&& document.form.dsltmp_cfg_ispname.value=='Airtel (Automatic IP)'
			)
	){
		document.form.dsltmp_cfg_uksky.value = "1";	//flag
	}
	if( document.form.dsltmp_cfg_country.value=='Turkey'
		&& document.form.dsltmp_cfg_ispname.value.search("Turk Telekom") >= 0
	){
		document.form.dsltmp_cfg_trtt.value = "1";
	}

	if (connection_type==0) //PPPoE
	{
		document.form.next_page.value = "QIS_ppp_cfg_tmp.asp";
		document.form.dsltmp_wanTypeOption.value = "2";
		submit_page();
	}
	else if (connection_type==2) //Automatic IP
	{
		document.form.next_page.value = "QIS_mer_cfg_tmp.asp";
		document.form.dsltmp_wanTypeOption.value = "0";
		submit_page();
	}
	else if (connection_type==3) //BRIDGE
	{
		document.form.next_page.value = "QIS_bridge_cfg_tmp.asp";
		document.form.dsltmp_wanTypeOption.value = "3";
		submit_page();
	}
	else if (connection_type==4) //Static IP
	{
		document.form.next_page.value = "QIS_ipoa_cfg_tmp.asp";
		document.form.dsltmp_wanTypeOption.value = "1";
		submit_page();
	}
	else
		return;
}

function submit_detect(){
	document.form.current_page.value = "init_detection";
	document.form.next_page.value = "QIS_detect.asp";
	document.form.submit();
}

function check_ginp_try(obj){
	if(obj.checked)
	{
		document.form.dsltmp_cfg_ginp_try_enable.value = 1;
	}
	else
	{
		document.form.dsltmp_cfg_ginp_try_enable.value = 0;
	}
}

function showSpecificSetting(idx) {
	if ( idx=='NO' )
	{
		document.getElementById('dslx_ginp_try_checkbox').style.display = "none";
		document.form.dsltmp_cfg_ginp_try_enable.value = 0;
		return;
	}

	if( (ISP_List[idx][1] == 'United Kingdom'
			&& (ISP_List[idx][4] == 'Sky - Fibre Broadband'
			|| ISP_List[idx][4] == 'TalkTalk (Automatic IP)')
			)
		|| (ISP_List[idx][1] == 'India'
			&& ISP_List[idx][4] == 'Airtel (Automatic IP)'
			)
	) {
		document.getElementById('dslx_ginp_try_checkbox').style.display = "";
		document.form.dsltmp_cfg_ginp_try_enable.value = 1;
	}
	else {
		document.getElementById('dslx_ginp_try_checkbox').style.display = "none";
		document.form.dsltmp_cfg_ginp_try_enable.value = 0;
	}
}

</script>
</head>
<body onLoad="QIS_menual_setting_load_body();" >
<form method="post" name="form" action="QIS_process.asp" target="">
<input type="hidden" name="prev_page" value="">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_PTM_manual_setting.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_detect.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="dsltmp_cfg_country" value="">
<input type="hidden" name="dsltmp_cfg_ispname" value="">
<input type="hidden" name="dsltmp_cfg_ispservice" value="">
<input type="hidden" name="dsltmp_auto_detect_bng_flag" value="0">
<input type="hidden" name="dsltmp_cfg_prctl" value="">
<input type="hidden" name="dsltmp_cfg_vlanid" value="">
<input type="hidden" name="dsltmp_cfg_iptv_rmvlan" value="">
<input type="hidden" name="dsltmp_cfg_iptv_mr" value="">
<input type="hidden" name="dsltmp_cfg_iptv_idx" value="">
<input type="hidden" name="dsltmp_cfg_iptv_num_pvc" value="">
<input type="hidden" name="dsltmp_cfg_iptv_pvclist" value="">
<input type="hidden" name="dsltmp_cfg_iptv_enable" value="0">
<input type="hidden" name="dsltmp_transfer_mode" value="PTM">
<input type="hidden" name="dsltmp_wanTypeOption" value="">
<input type="hidden" name="dsltmp_dsl_restart" value="<%tcWebApi_Get("GUITemp_Entry0","dsltmp_dsl_restart","s")%>">
<input type="hidden" name="dsltmp_restart_waittime" value=0>
<input type="hidden" name="dsltmp_cfg_th3bb" value="">
<input type="hidden" name="dsltmp_cfg_vpi" value="">
<input type="hidden" name="dsltmp_cfg_vci" value="">
<input type="hidden" name="dsltmp_cfg_encap" value="">
<input type="hidden" name="dsltmp_cfg_modulation" value="">
<input type="hidden" name="dsltmp_cfg_annex" value="">
<input type="hidden" name="dsltmp_cfg_de17a" value="">
<input type="hidden" name="dsltmp_cfg_vdslprofile" value="<%tcWebApi_Get("Adsl_Entry","vdsl_profile","s")%>">
<input type="hidden" name="dsltmp_cfg_uksky" value="">
<input type="hidden" name="dsltmp_cfg_ginp_try_enable" value="0">
<input type="hidden" name="dsltmp_cfg_trtt" value="0">

<div class="QISmain">
<div class="formfonttitle" style="padding:6 0 0 10;">
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
</div>
<br/>
<table width="80%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
	<thead>
		<tr>
			<td colspan="2"><% tcWebApi_Get("String_Entry", "Manual_Setting_Title", "s") %></td>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td colspan="2">
				<%tcWebApi_get("String_Entry","Manual_Setting_desc1","s")%><br/>
				<span id='warnning'>
				<b></b>
				</span>
			</td>
		</tr>
		<tr>
			<th width="40%"><%tcWebApi_get("String_Entry","Manual_Setting_contry","s")%></th>
			<td>
				<div id="CountryList"></div>
			</td>
		</tr>
		<tr id="City_tr">
			<th width="40%"><%tcWebApi_get("String_Entry","Manual_Setting_city","s")%></th>
			<td>
				<div id="CityList"></div>
			</td>
		</tr>
		<tr id="ISP_tr">
			<th width="40%"><%tcWebApi_get("String_Entry","HSDPAC_ISP_in","s")%></th>
			<td>
				<div id="ISPList"></div>
				<span id="dslx_ginp_try_checkbox" style="display:none;">
					<input type="checkbox" onClick="check_ginp_try(this);" checked>Try to connect with G.INP Enabled</input>
				</span>
			</td>
		</tr>
		<tr id="Service_tr">
			<th width="40%"><%tcWebApi_get("String_Entry","qis_service","s")%></th>
			<td>
				<div id="Service"></div>
			</td>
		</tr>
	</tbody>
</table>
<div id='vccInfo'>
<table width="80%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
	<tbody>
	<tr>
		<td colspan="2"><%tcWebApi_get("String_Entry","Manual_Setting_desc_ptm","s")%></td>
	</tr>
	<tr>
		<th><%tcWebApi_get("String_Entry","L3F_x_ConnectionType_in","s")%></th>
		<td>
			<select name='user_prctl' class="input_option" tabindex="5">
				<option value = 0>PPPoE</option>
				<option value = 2>Automatic IP</option>
				<option value = 4>Static IP</option>
				<!--option value = 3>BRIDGE</option--><!-- remove bridge if service number is 0(internet)  -->
			</select>
		</td>
	</tr>
	<tr>
		<th><%tcWebApi_get("String_Entry","WANVLANIDText","s")%></th>
		<td><input type='text' name='user_vlanid' tabindex="6" maxlength="5" class="input_6_table"><span></span></td>
	</tr>
	</tbody>
</table>
</div>
<div class="apply_gen" style="margin-top:30px">
	<!--input type="button" id="detectButton" value="<% tcWebApi_Get("String_Entry", "CTL_Detect_Again", "s") %>" tabindex="8" onclick="submit_detect();" class="button_gen_long"-->
	<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>" tabindex="7" onclick="btnNext();" class="button_gen">
</div>
</div>
</form>
</body>

<!--qis/QIS_PTM_manual_setting.asp-->
</html>
