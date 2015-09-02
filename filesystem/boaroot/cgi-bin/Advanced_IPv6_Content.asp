<%
if Request_Form("apply_flag") <> "" Then
	TCWebApi_set("ipv6rd_Entry","Active","tunnelEnable")
	if TCWebAPI_get("ipv6rd_Entry", "Active", "h") = "Yes" then
		TCWebApi_set("ipv6rd_Entry","Prefix","ipv6rd_prefix")
		TCWebApi_set("ipv6rd_Entry","PrefixLen","ipv6rd_prefixlen")
		TCWebApi_set("ipv6rd_Entry","CEIPv4Addr","CEipv4Addr")
		TCWebApi_set("ipv6rd_Entry","IPv4MaskLen","ipv4masklen")
		TCWebApi_set("ipv6rd_Entry","BRIPv4Addr","BRipv4Addr")
	end if
	tcWebApi_commit("ipv6rd_Entry")
end if
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_IPv6_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - IPv6</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>

function initial(){
	show_menu();
	ipv6rdenable();
}

function ipv6rdenable(){
	with(document.form) {
		if(tunnelEnable[1].checked){
			inputCtrl(ipv6rd_prefix, 0);
			inputCtrl(ipv6rd_prefixlen, 0);
			inputCtrl(CEipv4Addr, 0);
			inputCtrl(ipv4masklen, 0);
			inputCtrl(BRipv4Addr, 0);
			showhide("PrefixDelegation", 0);
		}
		else{
			inputCtrl(ipv6rd_prefix, 1);
			inputCtrl(ipv6rd_prefixlen, 1);
			inputCtrl(CEipv4Addr, 1);
			inputCtrl(ipv4masklen, 1);
			inputCtrl(BRipv4Addr, 1);
			showhide("PrefixDelegation", 1);
		}
	}
}

function valid_IP(obj_name, obj_flag){
	var A_class_start = inet_network("1.0.0.0");
	var A_class_end = inet_network("126.255.255.255");
	var B_class_start = inet_network("127.0.0.0");
	var B_class_end = inet_network("127.255.255.255");
	var C_class_start = inet_network("128.0.0.0");
	var C_class_end = inet_network("255.255.255.255");
	var ip_obj = obj_name;
	var ip_num = inet_network(ip_obj.value);
	if(obj_flag == "DNS" && ip_num == -1){ //DNS allows to input nothing
		return true;
	}
	if(obj_flag == "GW" && ip_num == -1){ //GW allows to input nothing
		return true;
	}
	if(ip_num > A_class_start && ip_num < A_class_end)
		return true;
	else if(ip_num > B_class_start && ip_num < B_class_end){
		alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
	else if(ip_num > C_class_start && ip_num < C_class_end)
		return true;
	else{
		alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
}

function ipv6_valid(obj){
	var rangere=new RegExp("^((([0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}:[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){5}:([0-9A-Fa-f]{1,4}:)?[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){4}:([0-9A-Fa-f]{1,4}:){0,2}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){3}:([0-9A-Fa-f]{1,4}:){0,3}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){2}:([0-9A-Fa-f]{1,4}:){0,4}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}((\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b)\\.){3}(\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b))|(([0-9A-Fa-f]{1,4}:){0,5}:((\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b)\\.){3}(\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b))|(::([0-9A-Fa-f]{1,4}:){0,5}((\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b)\\.){3}(\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b))|([0-9A-Fa-f]{1,4}::([0-9A-Fa-f]{1,4}:){0,5}[0-9A-Fa-f]{1,4})|(::([0-9A-Fa-f]{1,4}:){0,6}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){1,7}:))$", "gi");
	if(rangere.test(obj.value)){
		return true;
	}else{
			alert(obj.value+"<%tcWebApi_get("String_Entry","JS_validip","s")%>");
		obj.focus();
		obj.select();
	return false;
	}
}

function inValidIPv6Prefix(Prefix1)
{
	var Prefix = trim(Prefix1);
	var IPv6Prefix = Prefix.match("^[0-9]{1,3}$");
	
	if(IPv6Prefix == null) { 
 		alert(Untranslated.IPv6_prefix_validation+ ": " + Prefix);
 		return true;
	}
	
	if((Number(Prefix) > 128 ) || (Number(Prefix) < 1 )){
		alert(Untranslated.IPv6_prefix_validation+ ": " + Prefix);
		return true;
	}
	
	return false;
}

function validInteger(Integer)
{
   if(Integer.match("^[0-9]+"))
   	return true;
   else
	return false;
}
/*
function calcIP6(ip) {
	var octet = ip.split(".");
	base = 16;
	var octet1 = parseInt(octet[0]);
	var octet2 = parseInt(octet[1]);
	var octet3 = parseInt(octet[2]);
	var octet4 = parseInt(octet[3]);
	hextet21 = octet1.toString(base);
	hextet22 = octet2.toString(base);
	hextet31 = octet3.toString(base);
	hextet32 = octet4.toString(base);
	var hextetval21 = (octet1 < 16) ? "0" + hextet21 : hextet21;
	var hextetval22 = (octet2 < 16) ? "0" + hextet22 : hextet22;
	var hextetval31 = (octet3 < 16) ? "0" + hextet31 : hextet31;
	var hextetval32 = (octet4 < 16) ? "0" + hextet32 : hextet32;
	var Calc_hex = hextetval21+hextetval22+":"+hextetval31+hextetval32;
	return Calc_hex;
}
*/
function validForm(){
	with (document.form) {
		if(tunnelEnable[0].checked){
			if(!ipv6_valid(ipv6rd_prefix)) {
				return false;
			}
			
			if(!valid_IP(CEipv4Addr, "")) {
				return false;
			}
			
			if(!valid_IP(BRipv4Addr, "")) {
				return false;
			}
			
			value1 = ipv4masklen.value;
			if((!validInteger(value1)) ) {
				alert("<%tcWebApi_get("String_Entry","6rdJS0Text","s")%>");
				return false;
			}
			if((Number(value1) > 32) || (Number(value1) < 0)) {
				alert("<%tcWebApi_get("String_Entry","6rdJS0Text","s")%>");
				return false;
			}
			
			value2 = ipv6rd_prefixlen.value;
			if(inValidIPv6Prefix(value2)) {
				return false;
			}
			
			value3 = parseInt(value2) + 32 - parseInt(value1);
			if( value3 < 0 || value3 > 128 ) {
				alert("<%tcWebApi_get("String_Entry","6rdJS1Text","s")%>");
				return false;
			}
		}
	}
	return true;
}

function applyRule(){
	if(validForm()){
		showLoading(5);
		setTimeout('location = "'+ location.pathname +'";', 5000);
		document.form.submit();
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
<form method="post" name="form" id="ruleForm" action="Advanced_IPv6_Content.asp" target="hidden_frame">
<!--input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_IPv6_Content.asp">
<input type="hidden" name="next_page" value="Advanced_IPv6_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_allnet">
<input type="hidden" name="action_wait" value="30">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wan_unit" value="0">
<input type="hidden" name="ipv6_dhcppd" value="">
<input type="hidden" name="ipv6_accept_ra" value="1"-->
<input type="hidden" name="apply_flag" value="1">
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
							<td bgcolor="#4D595D" valign="top">
								<div>&nbsp;</div>
								<div class="formfonttitle">IPv6</div>
								<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
								<div class="formfontdesc"><%tcWebApi_get("String_Entry","LHC_display6_sd","s")%></div>
								<!--div class="formfontdesc" style="margin-top:-10px;">
								<a id="faq" href="" target="_blank" style="font-family:Lucida Console;text-decoration:underline;">IPv6 FAQ</a>
								</div-->
								<table id="basic_config" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
									<thead>
										<tr>
						<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
										</tr>
									</thead>
									<tr>
						<th><%tcWebApi_get("String_Entry","Connectiontype","s")%></th>
										<td>
											<INPUT type="radio" value="Yes" name="tunnelEnable" class="input" <% if tcWebApi_get("ipv6rd_Entry","Active","h") = "Yes" then asp_Write("checked") elseif tcWebApi_get("ipv6rd_Entry","Active","h") = "" then asp_Write("checked") end if %> onclick="ipv6rdenable()"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>
											<INPUT type="radio" value="No" name="tunnelEnable" class="input" <% if tcWebApi_get("ipv6rd_Entry","Active","h") = "No" then asp_Write("checked") end if %> onclick="ipv6rdenable()"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%>

										</td>
									</tr>
									<tr>
										<th><%tcWebApi_get("String_Entry","6rdIPv6PrefixText","s")%></th>
										<td>
											<INPUT TYPE="TEXT" name="ipv6rd_prefix" maxLength="39" class="input_32_table" value="<%If TcWebApi_get("ipv6rd_Entry","Prefix","h") <> "" then TcWebApi_get("ipv6rd_Entry","Prefix","s") else asp_Write("2001:55c::") end if %>" >
											<INPUT TYPE="TEXT" NAME="ipv6rd_prefixlen" MAXLENGTH="3" class="input_3_table" VALUE="<%if tcwebapi_get("ipv6rd_Entry","PrefixLen","h")<> "" then tcwebApi_get("ipv6rd_Entry","PrefixLen","s") else asp_write("32") end if%>">
										</td>
									</tr>
									<tr>
										<th><%tcWebApi_get("String_Entry","6rdIPv4AddrText","s")%></th>
										<td>
											<INPUT TYPE="TEXT" name="CEipv4Addr" maxLength=20 class="input_20_table" value="<%If TcWebApi_get("ipv6rd_Entry","CEIPv4Addr","h") <> "" then TcWebApi_get("ipv6rd_Entry","CEIPv4Addr","s") else asp_Write("58.211.230.102") end if %>" >
										</td>
									</tr>
									<tr>
										<th><%tcWebApi_get("String_Entry","6rdIPv4MaskLengthText","s")%></th>
										<td>
											<INPUT TYPE="TEXT" name="ipv4masklen" maxLength=3 class="input_3_table" value="<%If TcWebApi_get("ipv6rd_Entry","IPv4MaskLen","h") <> "" then TcWebApi_get("ipv6rd_Entry","IPv4MaskLen","s") else asp_Write("0") end if %>" >
										</td>
									</tr>
									<tr>
										<th><%tcWebApi_get("String_Entry","6rdBorderRelayIPv4AddrText","s")%></th>
										<td>
											<INPUT TYPE="TEXT" name="BRipv4Addr" maxLength=39 class="input_32_table" value="<%If TcWebApi_get("ipv6rd_Entry","BRIPv4Addr","h") <> "" then TcWebApi_get("ipv6rd_Entry","BRIPv4Addr","s") else asp_Write("69.252.80.66") end if %>" >
										</td>
									</tr>
									<tr id="PrefixDelegation">
										<th><%tcWebApi_get("String_Entry","6rdPrefixDelegationText","s")%></th>
										<td>
											<%If TcWebApi_get("ipv6rd_Entry","6rdPD","h") <> "" Then TcWebApi_get("ipv6rd_Entry","6rdPD","s") else asp_Write("") end if%>
										</td>
									</tr>
								</table>

								<div class="apply_gen">
									<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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

<!--Advanced_IPv6_Content.asp-->
</html>

