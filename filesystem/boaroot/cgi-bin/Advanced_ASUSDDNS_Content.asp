<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - DDNS</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javaScript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/asus_eula.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
<% wanlink(); %>
var ddns_hostname_x_t = '<% tcWebApi_get("Ddns_Entry","MYHOST","s"); %>';
var ddns_server_x = '<% tcWebApi_get("Ddns_Entry","SERVERNAME","s"); %>';

var ddns_return_code = '<% nvram_get_ddns("Vram_Entry","ddns_return_code"); %>';
var ddns_old_name = '<% tcWebApi_get("Vram_Entry","ddns_old_name","s"); %>';
var ddns_enable_x = '<% tcWebApi_get("Ddns_Entry","Active","s"); %>';

<% login_state_hook(); %>
var wireless = []; // [[MAC, associated, authorized], ...]

var $j = jQuery.noConflict();
function init(){
	show_banner(1);
	show_footer();
	ddns_load_body();
	update_ddns_wan_unit_option();
 	autoFocus('<% get_parameter("af"); %>');

    ASUS_EULA.config(applyRule, refreshpage);
    if(ddns_enable_x == "1" && ddns_server_x == "WWW.ASUS.COM"){
        ASUS_EULA.check('asus');
    }
}

function update_ddns_wan_unit_option(){
	var wans_dualwan_array = '<% tcWebApi_get("Dualwan_Entry","wans_dualwan","s"); %>'.split(" ");
	var wans_mode = '<% tcWebApi_get("Dualwan_Entry","wans_mode","s"); %>';
	if(document.form.ddns_enable_x.value == "0" || !dualWAN_support || wans_mode != "lb" || wans_dualwan_array.indexOf("none") != -1){
		document.getElementById("ddns_wan_unit_th").style.display = "none";
		document.getElementById("ddns_wan_unit_td").style.display = "none";
	}else{
		document.getElementById("ddns_wan_unit_th").style.display = "";
		document.getElementById("ddns_wan_unit_td").style.display = "";
	}
}

function valid_wan_ip() {
	var A_class_start = inet_network("10.0.0.0");
	var A_class_end = inet_network("10.255.255.255");
	var B_class_start = inet_network("172.16.0.0");
	var B_class_end = inet_network("172.31.255.255");
	var C_class_start = inet_network("192.168.0.0");
	var C_class_end = inet_network("192.168.255.255");
	var ip_obj = wanlink_ipaddr();
	var ip_num = inet_network(ip_obj);
	var ip_class = "";
	if(ip_num > A_class_start && ip_num < A_class_end)
		ip_class = 'A';
	else if(ip_num > B_class_start && ip_num < B_class_end)
		ip_class = 'B';
	else if(ip_num > C_class_start && ip_num < C_class_end)
		ip_class = 'C';
	else if(ip_num != 0){
		showhide("wan_ip_hide2", 0);
		showhide("wan_ip_hide3", 0);
		return;
	}
	showhide("wan_ip_hide2", 1);
	showhide("wan_ip_hide3", 0);
	return;
}
function ddns_load_body(){
	show_menu();
	valid_wan_ip();

	if(ddns_enable_x == '1'){
		inputCtrl(document.form.ddns_server_x, 1);
		document.getElementById('ddns_hostname_tr').style.display = "";
		if(ddns_server_x == "WWW.ASUS.COM" || ddns_server_x == ""){
			document.form.ddns_hostname_x.parentNode.style.display = "none";
			document.form.DDNSName.parentNode.style.display = "";
			var ddns_hostname_title = ddns_hostname_x_t.substring(0, ddns_hostname_x_t.indexOf('.asuscomm.com'));
			if(ddns_hostname_x_t != '' && ddns_hostname_title)
				document.getElementById("DDNSName").value = ddns_hostname_title;
			else
				document.getElementById("DDNSName").value = "<% tcWebApi_Get("String_Entry", "asusddns_inputhint", "s") %>";
		}else{
			document.form.ddns_hostname_x.parentNode.style.display = "";
			document.form.DDNSName.parentNode.style.display = "none";
			inputCtrl(document.form.ddns_username_x, 1);
			inputCtrl(document.form.ddns_passwd_x, 1);
			if(ddns_hostname_x_t != '')
				document.getElementById("ddns_hostname_x").value = ddns_hostname_x_t;
			else
				document.getElementById("ddns_hostname_x").value = "<% tcWebApi_Get("String_Entry", "asusddns_inputhint", "s") %>";
		}
		change_ddns_setting(document.form.ddns_server_x.value);
	}
	else{
		inputCtrl(document.form.ddns_server_x, 0);
		document.getElementById('ddns_hostname_tr').style.display = "none";
		inputCtrl(document.form.ddns_username_x, 0);
		inputCtrl(document.form.ddns_passwd_x, 0);
		document.form.ddns_wildcard_x[0].disabled= 1;
		document.form.ddns_wildcard_x[1].disabled= 1;
		showhide("wildcard_field",0);
	}

	hideLoading();

	if(ddns_enable_x == "1")
    {
        var ddnsHint = getDDNSState(ddns_return_code, ddns_hostname_x_t, ddns_old_name);

        if(ddnsHint != "")
            alert(ddnsHint);
        if(ddns_return_code.indexOf('200')!=-1 || ddns_return_code.indexOf('220')!=-1 || ddns_return_code == 'register,230'){
            showhide("wan_ip_hide2", 0);
            if(ddns_server_x == "WWW.ASUS.COM"){
                showhide("wan_ip_hide3", 1);
            }
        }
    }
}

function apply_eula_check(){
    if(document.form.ddns_enable_x[0].checked == true && document.form.ddns_server_x.value == "WWW.ASUS.COM"){
        if(!ASUS_EULA.check("asus")) return false;
    }
    applyRule();
}

function applyRule(){
	if(validForm()){
		if(document.form.ddns_enable_x[0].checked == true && document.form.ddns_server_x.selectedIndex == 0){
			document.form.ddns_hostname_x.value = document.form.DDNSName.value+".asuscomm.com";
		}

		showLoading();
		document.form.submit();
	}
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_ASUSDDNS_Content.asp";
}

function validForm(){
	if(document.form.ddns_enable_x[0].checked){	//ddns enable

		if(document.form.ddns_server_x.selectedIndex == 0){ //WWW.ASUS.COM
			if(document.form.DDNSName.value == ""){
				alert("<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_14","s")%>");
				document.form.DDNSName.focus();
				document.form.DDNSName.select();
				return false;
			}
			else if(!validate_ddns_hostname(document.form.DDNSName)){
					document.form.DDNSName.focus();
					document.form.DDNSName.select();
					return false;
			}
			else
				return true;
		}
		else{

			if(document.form.ddns_hostname_x.value == ""){
				alert("<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_14","s")%>");
				document.form.ddns_hostname_x.focus();
				document.form.ddns_hostname_x.select();
				return false;
			}
			else if(!validator.string(document.form.ddns_hostname_x)){
				return false;
			}
			
			if(document.form.ddns_username_x.value == ""){
				alert("<%tcWebApi_get("String_Entry","QKS_account_nameblank","s")%>");
				document.form.ddns_username_x.focus();
				document.form.ddns_username_x.select();
				return false;
			}
			else if(!validator.string(document.form.ddns_username_x)){
				return false;
			}
			
			if(document.form.ddns_passwd_x.value == ""){
				alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc6","s")%>");
				document.form.ddns_passwd_x.focus();
				document.form.ddns_passwd_x.select();
				return false;
			}
			else if(!validator.string(document.form.ddns_passwd_x)){
				return false;
			}
			
			return true;
		}
	}
	else
		return true;
}

function checkDDNSReturnCode(){
	$j.ajax({
	url: '/cgi-bin/ajax_ddnscode.asp',
	dataType: 'script',
	error: function(xhr){
		checkDDNSReturnCode();
	},
	success: function(response){
		if(ddns_return_code == 'ddns_query')
			setTimeout("checkDDNSReturnCode();", 500);
		else
			refreshpage();
	}
	});
}
function cleandef(){
	if(document.form.DDNSName.value == "<% tcWebApi_Get("String_Entry", "asusddns_inputhint", "s") %>")
		document.form.DDNSName.value = "";
}
</script>
</head>
<body onload="init();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="wan_route_x" value="">
<input type="hidden" name="wan_nat_x" value="1">
<input type="hidden" name="current_page" value="Advanced_ASUSDDNS_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_ddns">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
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
		<td align="left" valign="top" >
			<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
			<tbody>
			<tr>
			<td bgcolor="#4D595D" valign="top" >
				<div>&nbsp;</div>
				<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_3","s")%> - DDNS</div>
				<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
				<div class="formfontdesc"><% tcWebApi_Get("String_Entry", "LHC_x_DDNSEnable_sd", "s") %></div>
				<div class="formfontdesc" id="wan_ip_hide2" style="color:#FFCC00;"><% tcWebApi_Get("String_Entry", "LHC_x_DDNSEnable_sd2", "s") %></div>
				<div class="formfontdesc" id="wan_ip_hide3" style="color:#FFCC00;"><% tcWebApi_Get("String_Entry", "LHC_x_DDNSEnable_sd3", "s") %></div>
				<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
					<input type="hidden" name="wl_gmode_protection_x" value="">
					<tr>
						<th><% tcWebApi_Get("String_Entry", "LHC_x_DDNSEnable_in", "s") %>?</th>
						<td>
							<input type="radio" value="1" name="ddns_enable_x"onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '1')" <% if tcWebApi_get("Ddns_Entry","Active","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
							<input type="radio" value="0" name="ddns_enable_x"onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_enable_x', '0')" <% if tcWebApi_get("Ddns_Entry","Active","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
						</td>
					</tr>
					<tr>
						<th id="ddns_wan_unit_th"><%tcWebApi_get("String_Entry","wan_interface","s")%></th>
						<td id="ddns_wan_unit_td">
							<select name="ddns_wan_unit" class="input_option">
								<option class="content_input_fd" value="-1" <% if tcWebApi_get("Ddns_Entry","ddns_wan_unit","h") = "-1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Auto","s")%></option>
								<option class="content_input_fd" value="0" <% if tcWebApi_get("Ddns_Entry","ddns_wan_unit","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","dualwan_primary","s")%></option>
								<option class="content_input_fd" value="1" <% if tcWebApi_get("Ddns_Entry","ddns_wan_unit","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","dualwan_secondary","s")%></option>
							</select>
						</td>
					</tr>
					<tr>
						<th><%tcWebApi_get("String_Entry","LHC_x_DDNSServer_in","s")%></th>
						<td>
							<select name="ddns_server_x"class="input_option"onchange="return change_common(this, 'LANHostConfig', 'ddns_server_x')">
								<option value="WWW.ASUS.COM" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.ASUS.COM" then asp_Write("selected") end if %>>WWW.ASUS.COM</option>
								<option value="DOMAINS.GOOGLE.COM"  <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "DOMAINS.GOOGLE.COM" then asp_Write("selected") end if %>>DOMAINS.GOOGLE.COM</option>
								<option value="WWW.DYNDNS.ORG" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.DYNDNS.ORG" then asp_Write("selected") end if %>>WWW.DYNDNS.ORG</option>
								<option value="WWW.DYNDNS.ORG_CUSTOM" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.DYNDNS.ORG_CUSTOM" then asp_Write("selected") end if %>>WWW.DYNDNS.ORG(CUSTOM)</option>
								<option value="WWW.DYNDNS.ORG_STATIC" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.DYNDNS.ORG_STATIC" then asp_Write("selected") end if %>>WWW.DYNDNS.ORG(STATIC)</option>								
								<option value="WWW.SELFHOST.DE" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.SELFHOST.DE" then asp_Write("selected") end if %>>WWW.SELFHOST.DE</option>
								<option value="WWW.ZONEEDIT.COM" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.ZONEEDIT.COM" then asp_Write("selected") end if %>>WWW.ZONEEDIT.COM</option>
								<option value="WWW.DNSOMATIC.COM" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.DNSOMATIC.COM" then asp_Write("selected") end if %>>WWW.DNSOMATIC.COM</option>
								<option value="WWW.TUNNELBROKER.NET" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.TUNNELBROKER.NET" then asp_Write("selected") end if %>>WWW.TUNNELBROKER.NET</option>
								<option value="WWW.NO-IP.COM" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "WWW.NO-IP.COM" then asp_Write("selected") end if %>>WWW.NO-IP.COM</option>								
							</select>
							<a id="link" href="javascript:openLink('x_DDNSServer')" style=" margin-left:5px; text-decoration: underline;"><%tcWebApi_get("String_Entry","LHC_x_DDNSServer_linkname","s")%></a>
							<a id="linkToHome" href="javascript:openLink('x_DDNSServer')" style=" margin-left:5px; text-decoration: underline;display:none;"><%tcWebApi_get("String_Entry","ddns_home_link","s")%></a>
						</td>
					</tr>
					<tr id="ddns_hostname_tr">
						<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(5,13);"><%tcWebApi_get("String_Entry","LHC_x_DDNSHostNames_in","s")%></a></th>
						<td>
							<div id="ddnsname_input" style="display:none;">
								<input type="text" maxlength="64" class="input_25_table" name="ddns_hostname_x" id="ddns_hostname_x" value="<% If tcWebApi_get("Ddns_Entry","MYHOST","h") <> "" then  tcWebApi_get("Ddns_Entry","MYHOST","s") end if %>" onKeyPress="return validator.isString(this, event)">
							</div>
							<div id="asusddnsname_input" style="display:none;">
								<input type="text" maxlength="32" class="input_32_table" name="DDNSName" id="DDNSName" class="inputtext" onKeyPress="return validator.isString(this, event)" OnClick="cleandef();">.asuscomm.com
								<div id="alert_block" style="color:#FFCC00; margin-left:5px; font-size:11px;display:none;">
									<span id="alert_str"></span>
								</div>
							</div>
						</td>
					</tr>
					<tr>
						<th><%tcWebApi_get("String_Entry","LHC_x_DDNSUserName_in","s")%></th>
						<td><input type="text" maxlength="64" class="input_25_table" name="ddns_username_x" value="<% If tcWebApi_get("Ddns_Entry","USERNAME","h") <> "" then  tcWebApi_get("Ddns_Entry","USERNAME","s") end if %>" onKeyPress="return validator.isString(this, event)" autocapitalization="off" autocomplete="off"></td>
					</tr>
					<tr>
						<th><%tcWebApi_get("String_Entry","LHC_x_DDNSPassword_in","s")%></th>
						<td><input type="password" maxlength="64" class="input_25_table" name="ddns_passwd_x" value="<% If tcWebApi_get("Ddns_Entry","PASSWORD","h") <> "" then  tcWebApi_get("Ddns_Entry","PASSWORD","s") end if %>" autocapitalization="off" autocomplete="off"></td>
					</tr>
					<tr id="wildcard_field">
						<th><%tcWebApi_get("String_Entry","LHC_x_DDNSWildcard_in","s")%>?</th>
						<td>
							<input type="radio" value="1" name="ddns_wildcard_x" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '1')" <% if tcWebApi_get("Ddns_Entry","WILDCARD","h") = "1" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
							<input type="radio" value="0" name="ddns_wildcard_x" onClick="return change_common_radio(this, 'LANHostConfig', 'ddns_wildcard_x', '0')" <% if tcWebApi_get("Ddns_Entry","WILDCARD","h") = "0" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
						</td>
					</tr>
					<tr style="display:none;">
						<th>Update Manually:</th>
						<td>
							<input type="hidden" maxlength="15" class="button_gen" size="12" name="" value="">
							<input type="submit" maxlength="15" class="button_gen" onclick="showLoading();return onSubmitApply('ddnsclient');" size="12" name="LANHostConfig_x_DDNSStatus_button" value="Update" />
						</td>
					</tr>
				</table>
				<div class="apply_gen">
					<input class="button_gen" onclick="apply_eula_check();" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>" />
				</div>
			</td>
			</tr>
			</tbody>
			</table>
		</td>
		</tr>
		</table>
	</td>
	<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
</form>
<div id="footer"></div>
</body>
</html>
