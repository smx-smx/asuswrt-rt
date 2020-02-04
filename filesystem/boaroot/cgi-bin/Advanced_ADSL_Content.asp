<%
If Request_Form("saveFlag") = "1" Then
	if tcWebApi_get("WebCustom_Entry", "isDSLPowerSavingSupport", "h") = "Yes" then
		tcWebApi_set("Adsl_Entry","dslx_power_saving","dslx_power_saving")
	end if
	if tcWebApi_get("WebCustom_Entry","ADSLChip","h") = "Yes" then
		tcWebApi_set("Adsl_Entry","dslx_testlab","dslx_testlab")
	elseif tcWebApi_get("WebCustom_Entry", "isMT7510", "h") = "Yes" then
		tcWebApi_set("Adsl_Entry","dslx_testlab","dslx_testlab")
	end if
tcWebApi_set("Adsl_Entry","dslx_snrm_offset","dslx_snrm_offset")
tcWebApi_set("Adsl_Entry","adsl_rx_agc","adsl_rx_agc")
tcWebApi_set("Adsl_Entry","vdsl_snrm_offset","vdsl_snrm_offset")
If Request_Form("vdtxpwrtestmodeFlag") = "1" Then
	tcWebApi_set("Adsl_Entry","vdsl_tx_gain_off","vdsl_tx_gain_off")
end if
tcWebApi_set("Adsl_Entry","vdsl_rx_agc","vdsl_rx_agc")
tcWebApi_set("Adsl_Entry","vdsl_upbo","vdsl_upbo")
if tcWebApi_get("WebCustom_Entry", "isMT7510", "h") = "Yes" then
	tcWebApi_set("Adsl_Entry","dslx_adsl_esnp","dslx_adsl_esnp")
elseif tcWebApi_get("WebCustom_Entry", "ADSLChip", "h") = "Yes" then
	tcWebApi_set("Adsl_Entry","dslx_adsl_esnp","dslx_adsl_esnp")
end if
tcWebApi_set("Adsl_Entry","dslx_vdsl_esnp","dslx_vdsl_esnp")
tcWebApi_set("Adsl_Entry","dslx_sra","dslx_sra")
tcWebApi_set("Adsl_Entry","dslx_bitswap","dslx_bitswap")
tcWebApi_set("Adsl_Entry","dslx_vdsl_bitswap","dslx_vdsl_bitswap")
tcWebApi_set("Adsl_Entry","MODULATIONTYPE","ModulationType")
tcWebApi_set("Adsl_Entry","ANNEXTYPEA","AnnexTypeA")
	if tcWebApi_get("WebCustom_Entry", "isSharePVCSupport", "h") = "Yes" then
		tcWebApi_set("Wan_Common","UniqueMac","UniqueMac")
		tcWebApi_set("Wan_Common","sharepvc","sharepvc")
	end if
	if tcWebApi_get("WebCustom_Entry", "havePtm", "h") = "Yes" then
		tcWebApi_set("Adsl_Entry","vdsl_profile","vdsl_profile")
	end if
	if tcWebApi_get("WebCustom_Entry", "isGINPSupport", "h") = "Yes" then
		tcWebApi_set("Adsl_Entry","dslx_ginp","dslx_ginp")
		tcWebApi_set("Adsl_Entry","dslx_ginp_try_enable","dslx_ginp_try_enable")
	end if
tcWebApi_set("Adsl_Entry","dslx_vdsl_vectoring","dslx_vdsl_vectoring")
tcWebApi_set("Adsl_Entry","dslx_vdsl_nonstd_vectoring","dslx_vdsl_nonstd_vectoring")
tcWebApi_set("Adsl_Entry","dslx_dla_enable","dslx_dla_enable")
tcWebApi_commit("Adsl_Entry")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_ADSL_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu_dsl_setting","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script>

var $j = jQuery.noConflict();
var log_xDSLmode;
var log_lineState;
var log_SNRMarginDown;
var init_dla_enable = "<%tcWebApi_get("Adsl_Entry", "dslx_dla_enable", "s")%>";
init_dla_enable = (init_dla_enable.length > 0)?init_dla_enable:1;
var dslx_ginp_try_enable = "<%tcWebApi_get("Adsl_Entry", "dslx_ginp_try_enable", "s")%>";
var dslx_ginp_try_enable_disp = "<%tcWebApi_get("Adsl_Entry", "dslx_ginp_try_enable_disp", "s")%>";

var display_dslx_adsl_esnp = 0;
<%if tcWebApi_get("WebCustom_Entry","isMT7510","h") = "Yes" then%>
display_dslx_adsl_esnp = 1;
<%elseif tcWebApi_get("WebCustom_Entry","ADSLChip","h") = "Yes" then%>
display_dslx_adsl_esnp = 1;
<%end if%>


function initial(){
	show_menu();
	remove_rx_agc_option();
	change_help_desc();
	autoFocus("<% get_parameter("af"); %>");

	<%if tcWebApi_get("WebCustom_Entry","ADSLChip","h") = "Yes" then%>
		hideXDSLSetting(document.form.dslx_testlab.value);
	<%elseif tcWebApi_get("WebCustom_Entry","isMT7510","h") = "Yes" then%>
		hideXDSLSetting(document.form.dslx_testlab.value);
	<%end if%>

	change_dla(init_dla_enable);
	<%if tcWebApi_get("WebCustom_Entry", "isGVectorSupport", "h") = "Yes" then%>
	hide_nonstd_vectoring(<% tcWebApi_get("Adsl_Entry","dslx_vdsl_vectoring","s") %>);
	<%end if%>
	modify_AnnexType_option();
	if(display_dslx_adsl_esnp == 1)
	{
		document.getElementById("show_dslx_adsl_esnp").style.display = "";
	}
	else
	{
		document.getElementById("show_dslx_adsl_esnp").style.display = "none";
	}
	setTimeout("update_current_SNR_margin();", 3000);

<%if tcWebApi_get("WebCustom_Entry", "isGINPSupport", "h") = "Yes" then%>
	if(dslx_ginp_try_enable_disp == 1)
	{
		document.getElementById('dslx_ginp_try_checkbox').style.display = "";
		if(dslx_ginp_try_enable == 1)
		{
			document.getElementById('dslx_ginp_try_checkbox').checked = true;
		}
		else
		{
			document.getElementById('dslx_ginp_try_checkbox').checked = false;
		}
		check_ginp_try(document.getElementById('dslx_ginp_try_checkbox'));
	}
	else
	{
		document.getElementById('dslx_ginp_try_checkbox').style.display = "none";
	}
<%end if%>

}

function update_current_SNR_margin(){
	$j.ajax({
		url: '/cgi-bin/ajax_AdslSetting.asp',
		dataType: 'script',
		error: function(xhr){
				setTimeout("update_current_SNR_margin();", 1000);
			},

		success: function(){
				if(log_lineState == "up")
				{
					if(log_xDSLmode == "ADSL"){
						document.getElementById("id_sta_adj_adsl").style.display = "";
						document.getElementById("id_sta_adj_adsl").innerHTML = "<%tcWebApi_get("String_Entry","dslsetting_SNR_Margin","s")%>: "+log_SNRMarginDown;
					}
				<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
					else if(log_xDSLmode == "VDSL"){
						document.getElementById("id_sta_adj_vdsl").style.display = "";
						document.getElementById("id_sta_adj_vdsl").innerHTML = "<%tcWebApi_get("String_Entry","dslsetting_SNR_Margin","s")%>: "+log_SNRMarginDown;
					}
				<%end if%>
				}
				else{
					document.getElementById("id_sta_adj_adsl").style.display = "none";
				<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
					document.getElementById("id_sta_adj_vdsl").style.display = "none";
				<%end if%>
				}

				setTimeout("update_current_SNR_margin();", 5000);
			}
	});
}

function modify_AnnexType_option(){
	if(is_AU_sku){//	remove Annex L
		for(var x=0; x < document.form.AnnexTypeA.length; x++){
			if(document.form.AnnexTypeA[x].value == "ANNEX A/L")
				document.form.AnnexTypeA.remove(x);
		}				
		
		document.getElementById("AnnexTypeAIJLM").innerHTML = "ANNEX A/I/J/M";
	}
	else
		return;
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_ADSL_Content.asp";
}

function applyRule(){
	document.form.saveFlag.value = 1;
<%if tcWebApi_get("Adsl_Entry","vdtxpwrtestmode","h") = "1" then%>
	document.form.vdtxpwrtestmodeFlag.value = 1;
<%end if%>
	showLoading(8);
	setTimeout("redirect();", 8000);
	document.form.submit();
}
function doSharePVCChange(){
	with (document.form){
		if(sharepvc.value == 0)
			UniqueMac.value = "0";
		else
			UniqueMac.value = "1";
	}
	return;
}

<%if tcWebApi_get("WebCustom_Entry","ADSLChip","h") = "Yes" then%>
function hideXDSLSetting(_value){
	if( _value == "AU" || _value == "BR" || _value == "GB" ){
		document.form.dslx_snrm_offset.style.display = "none";
		document.form.dslx_snrm_offset.disabled = true;         
		document.getElementById("dslx_snrm_offset_read").style.display = "";
		document.getElementById("dslx_snrm_offset_read").innerHTML = '<%tcWebApi_get("String_Entry","btn_Disabled","s")%>';

		//disable DLA, then hidden
		document.form.dslx_dla_enable.options[1].selected = 1;
		inputHideCtrl(document.form.dslx_dla_enable, 0);
	}
	else
	{
		document.form.dslx_snrm_offset.style.display = "";
		document.form.dslx_snrm_offset.disabled = false;
		document.getElementById("dslx_snrm_offset_read").style.display = "none";

		//enable DLA		
		inputHideCtrl(document.form.dslx_dla_enable, 1);
	}
}
<%elseif tcWebApi_get("WebCustom_Entry","isMT7510","h") = "Yes" then%>
function hideXDSLSetting(_value)
{
	if( _value == "disable" )
	{
		//display DLA
		inputHideCtrl(document.form.dslx_dla_enable, 1);
	}
	else
	{
		//disable DLA, then hidden
		document.form.dslx_dla_enable.options[1].selected = 1;
		change_dla('0')
		inputHideCtrl(document.form.dslx_dla_enable, 0);
	}
}
<%end if%>

function change_dla(enable){
	if(enable == "1" 
<%if tcWebApi_get("WebCustom_Entry","ADSLChip","h") = "Yes" then%>
		|| document.form.dslx_testlab.value != "disable"	//"BR" "AU"
<%end if%>
	) {		
		document.form.dslx_snrm_offset.style.display = "none";
		document.form.dslx_snrm_offset.disabled = true;         
		document.getElementById("dslx_snrm_offset_read").style.display = "";
		document.getElementById("dslx_snrm_offset_read").innerHTML = get_snrm_offset();
		document.getElementById("dslx_snrm_offset_read").title = "Auto configured by Dynamic Line Adjustment (DLA)";

	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>

		document.form.dslx_vdsl_esnp.style.display = "none";
		document.form.dslx_vdsl_esnp.disabled = true;
		document.getElementById("dslx_vdsl_esnp_read").style.display = "";
		document.getElementById("dslx_vdsl_esnp_read").innerHTML = get_vdsl_esnp();
		document.getElementById("dslx_vdsl_esnp_read").title = "Auto configured by Dynamic Line Adjustment (DLA)";
	<%end if%>
	}
	else {
		document.form.dslx_snrm_offset.style.display = "";
		document.form.dslx_snrm_offset.disabled = false;
		document.getElementById("dslx_snrm_offset_read").style.display = "none";

	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		document.form.dslx_vdsl_esnp.style.display = "";
		document.form.dslx_vdsl_esnp.disabled = false;
		document.getElementById("dslx_vdsl_esnp_read").style.display = "none";
	<%end if%>
	}
	
	<%if tcWebApi_get("WebCustom_Entry","ADSLChip","h") = "Yes" then%>
	if(document.form.dslx_testlab.value != "disable"){	//"BR" "AU"
		document.getElementById("dslx_snrm_offset_read").innerHTML = '<%tcWebApi_get("String_Entry","btn_Disabled","s")%>';
	}	
	<%end if%>
}

function get_snrm_offset(){
        var offset_ori = "<%tcWebApi_get("Adsl_Entry","dslx_snrm_offset","s")%>";
        switch(offset_ori){
                case "0":
                                        return "<%tcWebApi_get("String_Entry","btn_Disabled","s")%>";
                                        break;
                case "5120":
                                        return "10 dB";
                                        break;
                case "4608":
                                        return "9 dB";
                                        break;
                case "4096":
                                        return "8 dB";
                                        break;
                case "3584":
                                        return "7 dB";
                                        break;
                case "3072":
                                        return "6 dB";
                                        break;
                case "2560":
                                        return "5 dB";
                                        break;
                case "2048":
                                        return "4 dB";
                                        break;
                case "1536":
                                        return "3 dB";
                                        break;
                case "1024":
                                        return "2 dB";
                                        break;
                case "512":
                                        return "1 dB";
                                        break;
                case "-512":
                                        return "-1 dB";
                                        break;
                case "-1024":
                                        return "-2 dB";
                                        break;
                case "-1536":
                                        return "-3 dB";
                                        break;
                case "-2048":
                                        return "-4 dB";
                                        break;
                case "-2560":
                                        return "-5 dB";
                                        break;
                case "-3072":
                                        return "-6 dB";
                                        break;
                case "-3584":
                                        return "-7 dB";
                                        break;
                case "-4096":
                                        return "-8 dB";
					break;
                case "-4608":
                                        return "-9 dB";
                                        break;
                case "-5120":
                                        return "-10 dB";
                                        break;
                default:
                                        return "<%tcWebApi_get("String_Entry","btn_Disabled","s")%>";
                                        break;
        }
}

function get_vdsl_rx_agc(){
	var vdsl_rx_agc="<%tcWebApi_get("Adsl_Entry","vdsl_rx_agc","s")%>";
	switch(vdsl_rx_agc){
		case "394":
			return "Stable";
		case "550":
			return "High Performance";
		case "65535":
		default:
			return "Default";
	}
}

function get_vdsl_esnp(){
	var vdsl_esnp="<%tcWebApi_get("Adsl_Entry","dslx_vdsl_esnp","s")%>";
	switch(vdsl_esnp){
		case "1":
			return "Stable";
		case "0":
		default:
			return "Default";
	}
}

function remove_rx_agc_option(){
	if(productid == "DSL-N66U" || productid == "DSL-N55U-C1" || productid == "DSL-N16U" || productid == "DSL-N14U" || productid == "DSL-N14U-B1" || productid == "DSL-N12U-C1" || productid == "DSL-N12U-D1" || productid == "DSL-N12E-C1" || productid == "DSL-N10-D1" || productid == "DSL-N10-C1" || productid == "DSL-N10P-C1"){	//MODELDEP
		for(var i=0; i<document.form.adsl_rx_agc.length; i++){
			if(document.form.adsl_rx_agc.options[i].value == 'High Performance')
				document.form.adsl_rx_agc.remove(i);
		}
	}
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	if(productid == "DSL-N66U"){
		for(var i=0; i<document.form.vdsl_rx_agc.length; i++){
			if(document.form.vdsl_rx_agc.options[i].value == '550')
				document.form.vdsl_rx_agc.remove(i);
		}
	}
<%end if%>
}

function change_help_desc(){
	if(productid == "DSL-N66U"){
		document.getElementById('vdsl_rx_agc_desc').onclick=function(){return openHint(25,13)};
	}
	if(productid == "DSL-N17U" || productid == "DSL-AC56U" || productid == "DSL-AC52U" || productid == "DSL-AC55U"){
		document.getElementById('adsl_rx_agc_desc').onclick=function(){return openHint(25,14)};
	}
}

<%if tcWebApi_get("WebCustom_Entry", "isGVectorSupport", "h") = "Yes" then%>
function hide_nonstd_vectoring(_value){
$("nonstd_vectoring").style.display = (_value == "0") ? "none" : "";
}
<%end if%>

<%if tcWebApi_get("WebCustom_Entry", "isGINPSupport", "h") = "Yes" then%>
function get_dslx_ginp(){
	var dslx_ginp="<%tcWebApi_get("Adsl_Entry","dslx_ginp","s")%>";
	switch(dslx_ginp){
		case "on":
			return "<%tcWebApi_get("String_Entry","btn_Enabled","s")%>";
		case "off":
		default:
			return "<%tcWebApi_get("String_Entry","btn_Disabled","s")%>";
	}
}

function check_ginp_try(obj){
	if(obj.checked)
	{
		document.form.dslx_ginp.style.display = "none";
		document.form.dslx_ginp.disabled = true;
		document.getElementById("dslx_ginp_read").style.display = "";
		document.getElementById("dslx_ginp_read").innerHTML = get_dslx_ginp();
		document.form.dslx_ginp_try_enable.value = 1;
	}
	else
	{
		document.form.dslx_ginp.style.display = "";
		document.form.dslx_ginp.disabled = false;
		document.getElementById("dslx_ginp_read").style.display = "none";
		document.form.dslx_ginp_try_enable.value = 0;
	}
}

<%end if%>

</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="hiddenMask" class="popup_bg">
<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
<tr>
<td>
<div class="drword" id="drword" style="height:110px;"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
<br/>
<br/>
</div>
<div class="drImg"><img src="/images/alertImg.png"></div>
<div style="height:70px;"></div>
</td>
</tr>
</table>
</div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="POST" action="/cgi-bin/Advanced_ADSL_Content.asp" name="form" target="hidden_frame">
<input type="hidden" name="UniqueMac" value="<%tcWebApi_get("Wan_Common", "UniqueMac", "s")%>"/>
<input type="hidden" name="dslx_ginp_try_enable" value="<%tcWebApi_get("Adsl_Entry", "dslx_ginp_try_enable", "s")%>"/>
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
<td align="left" valign="top">

<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","menu_dsl_setting","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","dslsetting_disc0","s")%></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
	<tr>
		<td colspan="2">
			Common Settings
		</td>	
	</tr>		
</thead>	
<tr>
<th width="30%"><%tcWebApi_get("String_Entry","dslsetting_disc1","s")%></th>
<td>
	<SELECT NAME="ModulationType" class="input_option">
	<%if tcWebApi_get("Adsl_Entry","ANNEXTYPE","h") = "B" then %>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "Auto Sync-Up" then asp_Write("selected") end if %>>Auto Sync-Up
		<%if tcWebApi_get("WebCustom_Entry","isCwmpVDSLSupport","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL Auto Sync-Up" then asp_Write("selected") end if %>>ADSL Auto Sync-Up
		<%end if%>
		<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "VDSL2" then asp_Write("selected") end if %>>VDSL2
		<%end if%>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2+" then asp_Write("selected") end if %>>ADSL2+
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2" then asp_Write("selected") end if %>>ADSL2
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.DMT" then asp_Write("selected") end if %>>G.DMT
	<%else%>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "Auto Sync-Up" then asp_Write("selected") end if %>>Auto Sync-Up
		<%if tcWebApi_get("WebCustom_Entry","isCwmpVDSLSupport","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL Auto Sync-Up" then asp_Write("selected") end if %>>ADSL Auto Sync-Up
		<%end if%>
		<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "VDSL2" then asp_Write("selected") end if %>>VDSL2
		<%end if%>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2+" then asp_Write("selected") end if %>>ADSL2+
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2" then asp_Write("selected") end if %>>ADSL2
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.DMT" then asp_Write("selected") end if %>>G.DMT
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "T1.413" then asp_Write("selected") end if %>>T1.413
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.lite" then asp_Write("selected") end if %>>G.lite
	<%end if%>
	</SELECT>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","dslsetting_disc2","s")%></th>
<td>
	<SELECT NAME="AnnexTypeA" class="input_option">
		<option value="ANNEX A" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A" then asp_Write("selected") end if %>>ANNEX A</option>
		<option value="ANNEX I" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX I" then asp_Write("selected") end if %>>ANNEX I</option>
		<option value="ANNEX A/L" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A/L" then asp_Write("selected") end if %>>ANNEX A/L</option>
		<option value="ANNEX M" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX M" then asp_Write("selected") end if %>>ANNEX M</option>
		<option id="AnnexTypeAIJLM" value="ANNEX A/I/J/L/M"<% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A/I/J/L/M" then asp_Write("selected") end if %>>ANNEX A/I/J/L/M</option>
		<%if tcWebApi_get("SysInfo_Entry","ProductName","h") <> "DSL-N12U-C1" then%>
			<%if tcWebApi_get("SysInfo_Entry","ProductName","h") <> "DSL-N12U-D1" then%>
				<option value="ANNEX B" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B" then asp_Write("selected") end if %>>ANNEX B</option>
				<%if tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-N66U" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%elseif tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-N17U" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%elseif tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-N16" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%elseif tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-AC51" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%elseif tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-N16P" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%elseif tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-AC56U" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%elseif tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-AC52U" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%elseif tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-AC55U" then%>
					<option value="ANNEX B/J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J" then asp_Write("selected") end if %>>ANNEX B/J</option>
				<%else%>
					<option value="ANNEX B/J/M" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J/M" then asp_Write("selected") end if %>>ANNEX B/J/M</option>
				<%end if%>
			<%end if%>
		<%end if%>
	</SELECT>
</td>
</tr>

<%if tcWebApi_get("WebCustom_Entry", "isMT7510", "h") = "Yes" then%>
<tr>
	<th>Country / ISP - Specific Setting</th>
	<td>
		<select name="dslx_testlab" class="input_option" onchange="hideXDSLSetting(this.value);">
			<option value="disable" <% tcWebApi_MatchThenWrite("Adsl_Entry","dslx_testlab","disable","selected") %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
			<option value="TR_TT" <% tcWebApi_MatchThenWrite("Adsl_Entry","dslx_testlab","TR_TT","selected") %>>Türk Telekom</option>
		</select>
	</td>
</tr>
<%end if%>

<tr>
	<th>
		<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,12);">Dynamic Line Adjustment (DLA)</a>
	</th>
	<td>
		<select class="input_option" name="dslx_dla_enable" onchange='change_dla(this.value);'>
			<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_dla_enable","h") <> "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
			<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_dla_enable","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		</select>
	</td>
</tr>

<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,2);"><%tcWebApi_get("String_Entry","dslsetting_SRA","s")%></a>
</th>
<td>
	<select class="input_option" name="dslx_sra">
		<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_sra","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
		<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_sra","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
	</select>
</td>
</tr>

<%if tcWebApi_get("WebCustom_Entry", "isSharePVCSupport", "h") = "Yes" then%>
<tr>
	<th>
		<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,3);"><%tcWebApi_get("String_Entry","dslsetting_sharePVC","s")%></a> 				
	</th>
	<td>
		<select class="input_option" name="sharepvc" onchange="doSharePVCChange()">
			<option value="1" <% if tcWebApi_get("Wan_Common","sharepvc","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
			<option value="0" <% if tcWebApi_get("Wan_Common","sharepvc","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		</select>
	</td>
</tr>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry", "isDSLPowerSavingSupport", "h") = "Yes" then%>
<tr>
	<th>
		<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,5);"><%tcWebApi_get("String_Entry","DSL_pwr_saving_in","s")%></a>
	</th>
	<td>
		<select class="input_option" name="dslx_power_saving">
			<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_power_saving","h") = "1" then asp_Write("selected") end if %>>Auto Adjust</option>
			<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_power_saving","h") <> "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		</select>
	</td>
</tr>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry", "isGINPSupport", "h") = "Yes" then%>
<tr>
	<th>
		<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,11);">G.INP (G.998.4)</a>
	</th>
	<td>
		<select class="input_option" name="dslx_ginp">
			<option value="off" <% if tcWebApi_get("Adsl_Entry","dslx_ginp","h") = "off" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
			<option value="on" <% if tcWebApi_get("Adsl_Entry","dslx_ginp","h") = "on" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
		</select>
		<span id="dslx_ginp_read" style="display:none;color:#FFFFFF;"></span>
		<span id="dslx_ginp_try_checkbox" style="display:none;">
			<br/>
			<input type="checkbox" onClick="check_ginp_try(this);" <% tcWebApi_MatchThenWrite("Adsl_Entry","dslx_ginp_try_enable","1","checked") %>>Try to connect with G.INP Enabled</input>
		</span>
	</td>
</tr>
<%end if%>

</table>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:10px;">
<thead>
	<tr>
		<td colspan="2">
			ADSL Settings
		</td>	
	</tr>		
</thead>

	<%if tcWebApi_get("WebCustom_Entry","ADSLChip","h") = "Yes" then%>
	<tr>
		<th>Country-Specific Setting</th>
		<td>
			<select name="dslx_testlab" class="input_option" onchange="hideXDSLSetting(this.value);">
				<option value="disable" <% if tcWebApi_get("Adsl_Entry","dslx_testlab","h") = "disable" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
				<option value="GB" <% if tcWebApi_get("Adsl_Entry","dslx_testlab","h") = "GB" then asp_Write("selected") end if %>>United Kingdom</option>
				<option value="AU" <% if tcWebApi_get("Adsl_Entry","dslx_testlab","h") = "AU" then asp_Write("selected") end if %>>Australia</option>
				<option value="BR" style="display:none;" <% if tcWebApi_get("Adsl_Entry","dslx_testlab","h") = "BR" then asp_Write("selected") end if %>>Telefonica Brazil</option>
			</select>
		</td>
	</tr>
	<%end if%>

	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,1);"><%tcWebApi_get("String_Entry","dslsetting_Stability_Adj","s")%></a>
		</th>
		<td>
			<span id="dslx_snrm_offset_read" name="dslx_snrm_offset_read" style="display:none;color:#FFFFFF;"></span>
			<select id="dslx_snrm_offset" class="input_option" name="dslx_snrm_offset" style="display:none;">
				<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
				<option value="5120" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "5120" then asp_Write("selected") end if %>>10 dB</option>
				<option value="4608" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "4608" then asp_Write("selected") end if %>>9 dB</option>
				<option value="4096" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "4096" then asp_Write("selected") end if %>>8 dB</option>
				<option value="3584" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "3584" then asp_Write("selected") end if %>>7 dB</option>
				<option value="3072" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "3072" then asp_Write("selected") end if %>>6 dB</option>
				<option value="2560" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "2560" then asp_Write("selected") end if %>>5 dB</option>
				<option value="2048" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "2048" then asp_Write("selected") end if %>>4 dB</option>
				<option value="1536" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "1536" then asp_Write("selected") end if %>>3 dB</option>
				<option value="1024" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "1024" then asp_Write("selected") end if %>>2 dB</option>
				<option value="512" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "512" then asp_Write("selected") end if %>>1 dB</option>
				<option value="-512" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-512" then asp_Write("selected") end if %>>-1 dB</option>
				<option value="-1024" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-1024" then asp_Write("selected") end if %>>-2 dB</option>
				<option value="-1536" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-1536" then asp_Write("selected") end if %>>-3 dB</option>
				<option value="-2048" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-2048" then asp_Write("selected") end if %>>-4 dB</option>
				<option value="-2560" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-2560" then asp_Write("selected") end if %>>-5 dB</option>
				<option value="-3072" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-3072" then asp_Write("selected") end if %>>-6 dB</option> 
				<option value="-3584" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-3584" then asp_Write("selected") end if %>>-7 dB</option> 
				<option value="-4096" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-4096" then asp_Write("selected") end if %>>-8 dB</option> 
				<option value="-4608" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-4608" then asp_Write("selected") end if %>>-9 dB</option> 
				<option value="-5120" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-5120" then asp_Write("selected") end if %>>-10 dB</option>
			</select>
			<span id="id_sta_adj_adsl" style="display:none;margin-left:10px;"></span>
		</td>
	</tr>
	<tr>
		<th>
			<a id="adsl_rx_agc_desc" class="hintstyle" href="javascript:void(0);" onClick="openHint(25,15);">Rx AGC GAIN Adjustment</a>
		</th>
		<td>
			<select id="adsl_rx_agc" class="input_option" name="adsl_rx_agc">
				<option value="Default" <% if tcWebApi_get("Adsl_Entry","adsl_rx_agc","h") = "Default" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Setting_factorydefault_value","s")%></option>
				<option value="Stable" <% if tcWebApi_get("Adsl_Entry","adsl_rx_agc","h") = "Stable" then asp_Write("selected") end if %>>Stable</option>
				<option value="High Performance" <% if tcWebApi_get("Adsl_Entry","adsl_rx_agc","h") = "High Performance" then asp_Write("selected") end if %>>High Performance</option>
			</select>
		</td>
	</tr>

	<!--esnp stands for enhanced sudden noise protection-->
	<tr id="show_dslx_adsl_esnp">
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,18);">ESNP - Enhanced Sudden Noise Protection</a>
		</th>
		<td>
			<select id="dslx_adsl_esnp" class="input_option" name="dslx_adsl_esnp">
				<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_adsl_esnp","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Setting_factorydefault_value","s")%></option>
				<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_adsl_esnp","h") = "1" then asp_Write("selected") end if %>>Stable</option>
			</select>
		</td>
	</tr>

	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,4);">Bitswap</a>
		</th>
		<td>
			<select class="input_option" name="dslx_bitswap">
				<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_bitswap","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
				<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_bitswap","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
			</select>
		</td>
	</tr>
</table>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:10px;">
<thead>
	<tr>
		<td colspan="2">
			VDSL Settings
		</td>	
	</tr>		
</thead>

	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,7);">VDSL Profile</a>
		</th>
		<td>
			<select class="input_option" name="vdsl_profile">
				<option value="0" <% if tcWebApi_get("Adsl_Entry","vdsl_profile","h") = "0" then asp_Write("selected") end if %>>30a multi mode</option>
				<option value="1" <% if tcWebApi_get("Adsl_Entry","vdsl_profile","h") = "1" then asp_Write("selected") end if %>>17a multi mode</option>
				<option value="2" <% if tcWebApi_get("Adsl_Entry","vdsl_profile","h") = "2" then asp_Write("selected") end if %>>12a multi mode</option>
				<option value="3" <% if tcWebApi_get("Adsl_Entry","vdsl_profile","h") = "3" then asp_Write("selected") end if %>>8a multi mode</option>
			</select>
		</td>
	</tr>

	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,6);"><%tcWebApi_get("String_Entry","dslsetting_Stability_Adj","s")%></a>
		</th>
		<td>
			<select class="input_option" name="vdsl_snrm_offset">
				<option value="32767" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "32767" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
				<option value="1024" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "1024" then asp_Write("selected") end if %>>2 dB</option>
				<option value="1536" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "1536" then asp_Write("selected") end if %>>3 dB</option>
				<option value="2048" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "2048" then asp_Write("selected") end if %>>4 dB</option>
				<option value="2560" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "2560" then asp_Write("selected") end if %>>5 dB</option>
				<option value="3072" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "3072" then asp_Write("selected") end if %>>6 dB</option>
				<option value="3584" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "3584" then asp_Write("selected") end if %>>7 dB</option>
				<option value="4096" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "4096" then asp_Write("selected") end if %>>8 dB</option>
				<option value="4608" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "4608" then asp_Write("selected") end if %>>9 dB</option>
				<option value="5120" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "5120" then asp_Write("selected") end if %>>10 dB</option>
				<option value="5632" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "5632" then asp_Write("selected") end if %>>11 dB</option>
				<option value="6144" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "6144" then asp_Write("selected") end if %>>12 dB</option>
				<option value="6656" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "6656" then asp_Write("selected") end if %>>13 dB</option>
				<option value="7168" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "7168" then asp_Write("selected") end if %>>14 dB</option>
				<option value="7680" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "7680" then asp_Write("selected") end if %>>15 dB</option>
				<option value="8192" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "8192" then asp_Write("selected") end if %>>16 dB</option>
				<option value="8704" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "8704" then asp_Write("selected") end if %>>17 dB</option>
				<option value="9216" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "9216" then asp_Write("selected") end if %>>18 dB</option>
				<option value="9728" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "9728" then asp_Write("selected") end if %>>19 dB</option>
				<option value="10240" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "10240" then asp_Write("selected") end if %>>20 dB</option>
				<option value="10752" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "10752" then asp_Write("selected") end if %>>21 dB</option>
				<option value="11264" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "11264" then asp_Write("selected") end if %>>22 dB</option>
				<option value="11776" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "11776" then asp_Write("selected") end if %>>23 dB</option>
				<option value="12288" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "12288" then asp_Write("selected") end if %>>24 dB</option>
				<option value="12800" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "12800" then asp_Write("selected") end if %>>25 dB</option>
				<option value="13312" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "13312" then asp_Write("selected") end if %>>26 dB</option>
				<option value="13824" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "13824" then asp_Write("selected") end if %>>27 dB</option>
				<option value="14336" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "14336" then asp_Write("selected") end if %>>28 dB</option>
				<option value="14848" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "14848" then asp_Write("selected") end if %>>29 dB</option>
				<option value="15360" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "15360" then asp_Write("selected") end if %>>30 dB</option>
			</select>
			<span id="id_sta_adj_vdsl" style="display:none;margin-left:10px;"></span>
		</td>
	</tr>
	
	<!--vdsl_rx_agc-->
	<tr>
		<th>
			<a id="vdsl_rx_agc_desc" class="hintstyle" href="javascript:void(0);" onClick="openHint(25,9);">Rx AGC GAIN Adjustment</a>
		</th>
		<td>
			<select id="vdsl_rx_agc" class="input_option" name="vdsl_rx_agc">
				<option value="65535" <% if tcWebApi_get("Adsl_Entry","vdsl_rx_agc","h") = "65535" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Setting_factorydefault_value","s")%></option>
				<option value="394" <% if tcWebApi_get("Adsl_Entry","vdsl_rx_agc","h") = "394" then asp_Write("selected") end if %>>Stable</option>
				<option value="550" <% if tcWebApi_get("Adsl_Entry","vdsl_rx_agc","h") = "550" then asp_Write("selected") end if %>>High Performance</option>
			</select>
		</td>
	</tr>
	
	<!--upbo stands for upstream power back off-->
	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,10);">UPBO - Upstream Power Back Off</a>
		</th>
		<td>
			<select class="input_option" name="vdsl_upbo">
				<option value="auto" <% if tcWebApi_get("Adsl_Entry","vdsl_upbo","h") = "auto" then asp_Write("selected") end if %>><% tcWebApi_Get("String_Entry", "Auto", "s") %></option>
				<option value="on" <% if tcWebApi_get("Adsl_Entry","vdsl_upbo","h") = "on" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
				<option value="off" <% if tcWebApi_get("Adsl_Entry","vdsl_upbo","h") = "off" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
			</select>
		</td>
	</tr>
	
	<!--esnp stands for enhanced sudden noise protection-->
	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,18);">ESNP - Enhanced Sudden Noise Protection</a>
		</th>
		<td>
			<span id="dslx_vdsl_esnp_read" name="dslx_vdsl_esnp_read" style="display:none;color:#FFFFFF;"></span>
			<select id="dslx_vdsl_esnp" class="input_option" name="dslx_vdsl_esnp">
				<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_esnp","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Setting_factorydefault_value","s")%></option>
				<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_esnp","h") = "1" then asp_Write("selected") end if %>>Stable</option>
			</select>
		</td>
	</tr>
	
	<%if tcWebApi_get("WebCustom_Entry","isMT7510","h") = "Yes" then%>
	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,4);">Bitswap</a>
		</th>
		<td>
			<select class="input_option" name="dslx_vdsl_bitswap">
				<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_bitswap","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
				<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_bitswap","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
			</select>
		</td>
	</tr>
<%end if%>

	<%if tcWebApi_get("Adsl_Entry","vdtxpwrtestmode","h") = "1" then%>
	<!--vdsl_tx_gain_off-->
	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,8);">Tx Power Control</a>
		</th>
		<td>
			<select class="input_option" name="vdsl_tx_gain_off">
				<option value="32767" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "32767" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
				<option value="30" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "30" then asp_Write("selected") end if %>>3 dB</option>
				<option value="20" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "20" then asp_Write("selected") end if %>>2 dB</option>
				<option value="10" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "10" then asp_Write("selected") end if %>>1 dB</option>
				<option value="0" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "0" then asp_Write("selected") end if %>>0 dB</option>
				<option value="-10" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-10" then asp_Write("selected") end if %>>-1 dB</option>
				<option value="-20" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-20" then asp_Write("selected") end if %>>-2 dB</option>
				<option value="-30" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-30" then asp_Write("selected") end if %>>-3 dB</option>
				<option value="-40" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-40" then asp_Write("selected") end if %>>-4 dB</option>
				<option value="-50" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-50" then asp_Write("selected") end if %>>-5 dB</option>
				<option value="-60" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-60" then asp_Write("selected") end if %>>-6 dB</option>
				<option value="-70" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-70" then asp_Write("selected") end if %>>-7 dB</option>
			</select>
		</td>
	</tr>
	<%end if%>
	
	<%if tcWebApi_get("WebCustom_Entry", "isGVectorSupport", "h") = "Yes" then%>
	<tr>
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,16);">G.vector (G.993.5)</a>
		</th>
		<td>
			<select class="input_option" name="dslx_vdsl_vectoring" onchange="hide_nonstd_vectoring(this.value);">
				<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_vectoring","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
				<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_vectoring","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
			</select>
		</td>
	</tr>
	<tr id="nonstd_vectoring">
		<th>
			<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,17);">Non-standard G.vector (G.993.5)</a>
		</th>
		<td>
			<select class="input_option" name="dslx_vdsl_nonstd_vectoring">
				<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_nonstd_vectoring","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
				<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_vdsl_nonstd_vectoring","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
			</select>
		</td>
	</tr>
	<%end if%>

</table>
<%end if%>	<!-- VDSL havePTM-->

<INPUT TYPE="HIDDEN" NAME="saveFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="vdtxpwrtestmodeFlag" VALUE="0">
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_ADSL_Content.asp-->
</html>
