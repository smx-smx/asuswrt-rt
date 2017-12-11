<%
If Request_Form("IPTV_flag") = "1" Then
	TCWebApi_set("IPTV_Entry","ad_mr_enable_x","mr_enable_x")
	TCWebApi_set("IPTV_Entry","ad_wan_port","pvc_service_num")
	TCWebApi_set("IPTV_Entry","ad_udpxy_enable_x","udpxy_enable_x")
	TCWebApi_set("IPTV_Entry","ad_rmvlan","rmvlan_x")
Elseif Request_Form("IPTV_flag") = "2" Then
	TCWebApi_set("IPTV_Entry","vd_mr_enable_x","mr_enable_x")
	TCWebApi_set("IPTV_Entry","vd_wan_port","pvc_service_num")
	TCWebApi_set("IPTV_Entry","vd_udpxy_enable_x","udpxy_enable_x")
	TCWebApi_set("IPTV_Entry","vd_rmvlan","rmvlan_x")
Elseif Request_Form("IPTV_flag") = "3" Then
	TCWebApi_set("IPTV_Entry","eth_mr_enable_x","mr_enable_x")
	TCWebApi_set("IPTV_Entry","eth_wan_port","pvc_service_num")
	TCWebApi_set("IPTV_Entry","eth_udpxy_enable_x","udpxy_enable_x")
	TCWebApi_set("IPTV_Entry","eth_rmvlan","rmvlan_x")
End If

If Request_Form("IPTV_flag") <> "0" Then
	TCWebApi_commit("IPTV_Entry")
End If	
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_IPTV_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - IPTV</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script>
	
var iptv_list_ad = [<%get_iptv_wan_list_ad()%>];
var iptv_list_vd = [<%get_iptv_wan_list_vd()%>];
var iptv_list_eth = [<%get_iptv_wan_list_eth()%>];

var wans_dualwan_orig = '<%tcWebApi_Get("Dualwan_Entry","wans_dualwan","s")%>';
var isMultiService_supported = '<%tcWebApi_Get("WebCustom_Entry","isMultiSerSupported","s")%>';
	
function initial(){
	show_menu();	
	load_TransMode();
	if(wans_dualwan_orig == "lan none" || wans_dualwan_orig == "usb none" ||
		wans_dualwan_orig == "lan usb" || wans_dualwan_orig == "usb lan"){
		doTransChange("disabled");
	}	
}

function load_TransMode(){
	if(iptv_list_ad.length > 0){
		add_option(document.form.wan_TransMode, "ADSL WAN (ATM)", "ATM", 1);
	}	
	if(isMultiService_supported == "Yes" && iptv_list_vd.length > 0){
		add_option(document.form.wan_TransMode, "VDSL WAN (PTM)", "PTM", 0);
	}	
	if(isMultiService_supported == "Yes" && iptv_list_eth.length > 0){
		if(model_name != "DSL-N66U"){	//modeldep: DSL-N66U not support IPTV through Ethernet WAN
			add_option(document.form.wan_TransMode, "Ethernet WAN", "Ethernet", 0);
		}
	}
		
	if(document.form.wan_TransMode.length == 0){
		if("<% tcWebApi_get("Wan_Common","TransMode","s") %>" == "ATM"){
			add_option(document.form.wan_TransMode, "ADSL WAN (ATM)", "ATM", 1);
			doTransChange("ATM");
			if(iptv_list_ad.length == 0){	//Default setting
				document.form.mr_enable_x.value = "0";
				document.form.mr_enable_x[1].disabled = true;
				enable_wan_port("0");
			}	
	
		}
		else if("<% tcWebApi_get("Wan_Common","TransMode","s") %>" == "PTM"){
			add_option(document.form.wan_TransMode, "VDSL WAN (ATM)", "PTM", 1);
			doTransChange("PTM");
			if(iptv_list_vd.length == 0){	//Default setting
				document.form.mr_enable_x.value = "0";
				document.form.mr_enable_x[1].disabled = true;
				enable_wan_port("0");
			}
			
			if(isMultiService_supported != "Yes"){
				doTransChange("disabled");
			}	
		}
		else if("<% tcWebApi_get("Wan_Common","TransMode","s") %>" == "Ethernet"){
			add_option(document.form.wan_TransMode, "Ethernet WAN", "Ethernet", 1);
			doTransChange("Ethernet");
			if(iptv_list_eth.length == 0){	//Default setting
				document.form.mr_enable_x.value = "0";
				document.form.mr_enable_x[1].disabled = true;
				enable_wan_port("0");
			}
			
			/*if(isMultiService_supported != "Yes" || model_name == "DSL-N66U"){		//modeldep: DSL-N66U not support IPTV through Ethernet WAN
				doTransChange("disabled");
			}*/
		}			
	}
	else{
		doTransChange(document.form.wan_TransMode.value);
	}	
}

function applyRule(){
	if(validForm()){
						
		showLoading(5);
		setTimeout("redirect();", 5000);
		document.form.submit();
	}
}
function redirect(){
	document.location.href = "/Advanced_IPTV_Content.asp";
}
function validForm(){
	if(!validator.rangeAllowZero(document.form.udpxy_enable_x, 1024, 65535, 0)){
		return false;	
	}
		
	return true;
}
function doTransChange(mode){
	if(mode == "disabled"){
		Setting_load_disabled();
	}
	else if(mode == "ATM"){
		Setting_load_ATM();
	}
	else if(mode == "PTM"){
		Setting_load_PTM();
	}	
	else{	//Ethernet
		Setting_load_Ethernet();
	}
}

function Setting_load_disabled(){
	
	document.getElementById("lan_none_desc").style.display = "";
	document.form.wan_TransMode.disabled = true;
	document.form.mr_enable_x.disabled = true;
	document.form.udpxy_enable_x.disabled = true;
	document.form.rmvlan_x.disabled = true;
	document.form.wan_TransMode.style.backgroundColor = "#CCCCCC";
	document.form.mr_enable_x.style.backgroundColor = "#CCCCCC";
	document.form.udpxy_enable_x.style.backgroundImage = "url(/images/New_ui/inputbg_disable.png)";
	document.form.rmvlan_x.style.backgroundColor = "#CCCCCC";
	document.getElementById("button_gen_div").style.display = "none";
}

/*
[mr_enable_x]  0 (disable) / 1 (enable) :: ad_mr_enable_x, vd_mr_enable_x, eth_mr_enable_x
[pvc_service_num]  0~7 :: ad_wan_port, vd_wan_port, eth_wan_port
[udpxy_enable_x]  1024 ~ 65535 :: ad_udpxy_enable_x, vd_udpxy_enable_x, eth_udpxy_enable_x
[rmvlan_x]  0 (disable) / 1 (enable) :: ad_rmvlan, vd_rmvlan, eth_rmvlan
*/

function Setting_load_ATM(){
	document.form.mr_enable_x.value = "<% tcWebApi_get("IPTV_Entry","ad_mr_enable_x","s") %>";
	enable_wan_port("<% tcWebApi_get("IPTV_Entry","ad_mr_enable_x","s") %>");
	free_options(document.form.pvc_service_num);
	for(var i=0; i<iptv_list_ad.length; i++){
		var pvc_service_num_temp = eval(iptv_list_ad[i]+1);
		if(iptv_list_ad[i] == "<% tcWebApi_get("IPTV_Entry","ad_wan_port","s") %>"){
			add_option(document.form.pvc_service_num, "PVC "+pvc_service_num_temp, iptv_list_ad[i], 1);
		}	
		else{
			add_option(document.form.pvc_service_num, "PVC "+pvc_service_num_temp, iptv_list_ad[i], 0);
		}	
	}	
	document.form.udpxy_enable_x.value = "<% tcWebApi_get("IPTV_Entry","ad_udpxy_enable_x","s") %>";
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		inputCtrl(document.form.rmvlan_x, 1);
		document.form.rmvlan_x.value = "<% tcWebApi_get("IPTV_Entry","ad_rmvlan","s") %>";
	<% end if %>
	document.form.IPTV_flag.value = 1;
}
function Setting_load_PTM(){
	document.form.mr_enable_x.value = "<% tcWebApi_get("IPTV_Entry","vd_mr_enable_x","s") %>";
	enable_wan_port("<% tcWebApi_get("IPTV_Entry","vd_mr_enable_x","s") %>");
	free_options(document.form.pvc_service_num);
	for(var i=0; i<iptv_list_vd.length; i++){
		var pvc_service_num_temp = eval(iptv_list_vd[i]+1);		
		if(iptv_list_vd[i] == "<% tcWebApi_get("IPTV_Entry","vd_wan_port","s") %>"){
			add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_vd[i], 1);
		}	
		else{
			add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_vd[i], 0);
		}	
	}
	document.form.udpxy_enable_x.value = "<% tcWebApi_get("IPTV_Entry","vd_udpxy_enable_x","s") %>";
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		inputCtrl(document.form.rmvlan_x, 1);
		document.form.rmvlan_x.value = "<% tcWebApi_get("IPTV_Entry","vd_rmvlan","s") %>";
	<% end if %>	
	document.form.IPTV_flag.value = 2;
}
function Setting_load_Ethernet(){
	document.form.mr_enable_x.value = "<% tcWebApi_get("IPTV_Entry","eth_mr_enable_x","s") %>";
	enable_wan_port("<% tcWebApi_get("IPTV_Entry","eth_mr_enable_x","s") %>");
	free_options(document.form.pvc_service_num);
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>		
		for(var i=0; i<iptv_list_eth.length; i++){
			var pvc_service_num_temp = eval(iptv_list_eth[i]+1);		
			if(iptv_list_eth[i] == "<% tcWebApi_get("IPTV_Entry","eth_wan_port","s") %>"){
				add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_eth[i], 1);
			}	
			else{
				add_option(document.form.pvc_service_num, "Service "+pvc_service_num_temp, iptv_list_eth[i], 0);
			}	
		}
	<%else%>
		document.form.pvc_service_num.parentNode.parentNode.style.display = "none";
		add_option(document.form.pvc_service_num, "Service 1", 0, 1);
	<% end if %>	
	document.form.udpxy_enable_x.value = "<% tcWebApi_get("IPTV_Entry","eth_udpxy_enable_x","s") %>";
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		inputCtrl(document.form.rmvlan_x, 1);
		document.form.rmvlan_x.value = "<% tcWebApi_get("IPTV_Entry","eth_rmvlan","s") %>";
	<%else%>
		inputCtrl(document.form.rmvlan_x, 0);
	<% end if %>	
	document.form.IPTV_flag.value = 3;
}

function enable_wan_port(flag){
	if(flag==1){	//enable
		inputCtrl(document.form.pvc_service_num, 1);
		if(document.form.wan_TransMode.value == "Ethernet" && isMultiService_supported != "Yes"){
			document.form.pvc_service_num.parentNode.parentNode.style.display = "none";
			document.form.pvc_service_num.value = 0;
		}
	}
	else{
		inputCtrl(document.form.pvc_service_num, 0);
	}	
}
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
<form method="post" name="form" id="ruleForm" action="Advanced_IPTV_Content.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_IPTV_Content.asp">
<input type="hidden" name="next_page" value="Advanced_IPTV_Content.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="20">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="IPTV_flag" value="0">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="rmvlan" value='<% tcWebApi_get("IPTV_Entry","rmvlan","s"); %>'>
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
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_2","s")%> - IPTV</div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc">The WAN port must connect to the internet when watch IPTV.</div>
<div id="lan_none_desc" class="formfontdesc" style="display:none;color:#FFCC00;"><span><%tcWebApi_get("String_Entry","FW_note","s")%> IPTV is not supported for the current WAN transfer mode.</span></div>
<div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
	<thead>
	<tr>
		<td colspan="2"><%tcWebApi_get("String_Entry","wan_type","s")%></td>
	</tr>
	</thead>
	<tr>
		<th><%tcWebApi_get("String_Entry","wan_type","s")%></th>
		<td>
			<select class="input_option" name="wan_TransMode" onchange="doTransChange(this.value);"></td>
	</tr>
</table>
</div>

<div style="margin-top:10px;">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
	<thead>
	<tr>
		<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
	</tr>
	</thead>	
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,11);"><%tcWebApi_get("String_Entry","RC_GWMulticastEnable_in","s")%> (IGMP Proxy)</a></th>
		<td>
			<select name="mr_enable_x" class="input_option" onChange="enable_wan_port(this.value);">
				<option value="0"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
				<option value="1"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
			</select>
		</td>
	</tr>
	<tr>
		<th>WAN Port</th>
		<td>
			<select name="pvc_service_num" class="input_option">
			</select>
		</td>
	</tr>
	<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(6, 6);"><%tcWebApi_get("String_Entry","RC_IPTV_in","s")%></a></th>
		<td>
			<input id="udpxy_enable_x" type="text" maxlength="5" class="input_6_table" name="udpxy_enable_x" value="<%tcWebApi_get("IPTV_Entry", "udpxy_enable_x", "s")%>" onkeypress="return is_number(this,event);">
		</td>
	</tr>
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	<tr>
		<th>Remove VLAN TAG from WAN</th>
		<td>
			<select name="rmvlan_x" class="input_option">
				<option value="0" <%if tcWebApi_get("IPTV_Entry","rmvlan","h") = "0" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
				<option value="1" <%if tcWebApi_get("IPTV_Entry","rmvlan","h") = "1" then asp_Write("selected") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
			</select>
		</td>
	</tr>
	<% end if %>
</table>

</div>

<div id="button_gen_div" class="apply_gen">
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

<!--Advanced_IPTV_Content.asp-->
</html>

