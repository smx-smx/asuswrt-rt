<%
	If Request_Form("pvcFlag")="1" then
	tcWebApi_set("WebCurSet_Entry","dev_pvc","DvInfo_PVC")
	else
	set_primary_pvc();
	END If

	If Request_Form("Saveflag")="1" then
		tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
		If Request_Form("Dipflag")="1" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="2" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="3" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_set("DeviceInfo_PVC","IP6DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="4" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_set("DeviceInfo_PVC","IP6DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		END If
		If Request_Form("DipConnFlag")="1" then
			tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
			tcWebApi_commit("DeviceInfo_PVC");
		elseif Request_Form("DipConnFlag")="2" then
			tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
			tcWebApi_commit("DeviceInfo_PVC");
		END If
	END If
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--device-map/internet.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title><% tcWebApi_Get("String_Entry", "Internet", "s") %></title>
<link rel="stylesheet" type="text/css" href="/NM_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>

<script>
<% wanlink(); %>

var wan_type = wanlink_type();
var wan_IP = wanlink_ipaddr();
var wandns = wanlink_dns();
var wan_Gateway = wanlink_gateway();
	
function tabclickhandler(unit){
	if(unit == 1){
		location.href = "DSL_dashboard.asp";
	}
	else{
		return;
	}	
}	
	
function initial()
{
	update_all_info();	
	show_DSL_tab();
	setTimeout('update_wan_status();', 1000);
}

function show_DSL_tab(){
	//0:ATM, 8:PTM
	if("<% tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","s") %>" == "0" || "<% tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","s") %>" == "8")	
		document.getElementById("DSL_tab").style.display = "";
	else
		document.getElementById("DSL_tab").style.display = "none";
}

function doSave() {
	document.DvInfo_Form.pvcFlag.value = 1;
	document.DvInfo_Form.submit();
}
function renewrelease(ip){
  document.DvInfo_Form.Dipflag.value = ip;
  document.DvInfo_Form.Saveflag.value = 1;
  document.DvInfo_Form.DipConnFlag.value = 0;
	parent.showLoading(6);
	parent.setTimeout('window.location.reload();', 6000);
  document.DvInfo_Form.submit();
}
function reconnect(flag){
  document.DvInfo_Form.DipConnFlag.value = flag;
  document.DvInfo_Form.Saveflag.value = 1;
	parent.showLoading(6);
	parent.setTimeout('window.location.reload();', 6000);
  document.DvInfo_Form.submit();
}

function update_all_info()
{
	wan_type = wanlink_type();
	wan_IP = wanlink_ipaddr();
	wandns = wanlink_dns();
	wan_Gateway = wanlink_gateway();
	var dnsArray = wandns.split(" ");

	var dns_ip = "";
	var dns_ip_2 = "";
	if (typeof(dnsArray[0]) !== "undefined")
		dns_ip = dnsArray[0];
	if (typeof(dnsArray[1]) !== "undefined")
		dns_ip_2 = dnsArray[1];

	if(wan_type == "dhcp")
		wan_type = "<%tcWebApi_Get("String_Entry", "BOP_ctype_title1", "s")%>";
	else if(wan_type == "static")
		wan_type = "<%tcWebApi_Get("String_Entry", "BOP_ctype_title5", "s")%>"+"IPoA";
	else if(wan_type == "pppoe" || wan_type == "PPPOE")
		wan_type = "PPPoE/PPPoA";
	else if(wan_type == "pptp")
		wan_type = "PPTP";
	else if(wan_type == "l2tp")
		wan_type = "L2TP";
	else if(wan_type == "pppoa")
		wan_type = "PPPoA";
	else if(wan_type == "ipoa")
		wan_type = "IPoA";

	document.getElementById("div_wanType").innerHTML = '<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">' + wan_type + '</p>';
	document.getElementById("div_wanIP").innerHTML = '<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">' + wan_IP + '</p>';

	if("<%tcWebApi_get("Wan_PVC","DNS_type","s")%>" == "1")
	{
		var pri_dns = "<%tcWebApi_get("Wan_PVC","Primary_DNS","s")%>";
		var scnd_dns = "<%tcWebApi_get("Wan_PVC","Secondary_DNS","s")%>";
		if(pri_dns != "")
			dns_ip = pri_dns;
		else if(scnd_dns != "")
			dns_ip = scnd_dns;
		else
			dns_ip = "";

		if((dns_ip == pri_dns) && scnd_dns != "")
			document.getElementById("div_wanDNS").innerHTML = '<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">' + dns_ip + '<br>' + scnd_dns + '</p>';
		else
			document.getElementById("div_wanDNS").innerHTML = '<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">' + dns_ip + '</p>';
	}
	else{
		if(dns_ip_2 == "")
			document.getElementById("div_wanDNS").innerHTML = '<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">' + dns_ip + '</p>';
		else
			document.getElementById("div_wanDNS").innerHTML = '<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">' + dns_ip + '<br>' + dns_ip_2 + '</p>';
	}

	document.getElementById("div_wanGateway").innerHTML = '<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">' + wan_Gateway + '</p>';
}

function update_wan_status(){
	$.ajax({
		url: '/status.asp',
		dataType: 'script',

		error: function(xhr){
			setTimeout("update_wan_status();", 3000);
		},
		success: function(response){
			update_all_info();
			setTimeout("update_wan_status();", 3000);
		}
	});
}
</script>
</head>
<body class="statusbody" onload="initial();">
<FORM METHOD="POST" ACTION="/cgi-bin/device-map/internet.asp" name="DvInfo_Form">
	<INPUT TYPE="HIDDEN" NAME="Saveflag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="Dipflag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="IPv6PrivacyAddrsSupportedFlag" value="<%tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","s")%>" >
	<INPUT TYPE="HIDDEN" NAME="DipConnFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="pvcFlag" VALUE="0">
<table id="DSL_tab" border="0" cellpadding="0" cellspacing="0" style="display:none;">
<tr>
	<td>
	<table width="100px" border="0" align="left" style="margin-left:8px;" cellpadding="0" cellspacing="0">
	<td>
		<div id="t0" class="tabclick_NW" align="center" style="margin-right:2px; width:90px;" onclick="tabclickhandler(0)">
			<span id="span0" style="cursor:pointer;">WAN</span>
		</div>
	</td>
	<td>
		<div id="t1" class="tab_NW" align="center" style="font-weight: bolder; margin-right:2px; width:90px;" onclick="tabclickhandler(1)">
			<span id="span1" style="cursor:pointer;font-weight: bolder;">DSL</span>
		</div>
	</td>
	</table>
	</td>
</tr>
</table>
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px" id="rt_table">
<tr>
<td height="50" style="padding:10px 15px 0px 15px;">
<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","Transfer_Mode","s")%></p>

<SELECT NAME="DvInfo_PVC" class="input_option" style="margin-bottom:5px;" onChange="doSave()">
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "0" then asp_Write("selected") end if %>>ADSL WAN (ATM)&nbsp;&nbsp;</OPTION>
<%else%>
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "0" then asp_Write("selected") end if %>>ADSL WAN (ATM)&nbsp;&nbsp;</OPTION>
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		<OPTION value="8" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then asp_Write("selected") end if %>>VDSL WAN (PTM)&nbsp;&nbsp;</OPTION>
		<!--<OPTION value="9" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "9" then asp_Write("selected") end if %>>PTM1&nbsp;&nbsp;</OPTION>-->
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		<OPTION value="10" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then asp_Write("selected") end if %>>Ethernet WAN&nbsp;&nbsp;</OPTION>
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveUSBModem","h") = "Yes" then%>
		<OPTION value="11" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "11" then asp_Write("selected") end if %>>USB Modem&nbsp;&nbsp;</OPTION>
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
		<OPTION value="12" <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "12" then asp_Write("selected") end if %>>Ethernet WAN&nbsp;&nbsp;</OPTION>
	<%end if%>
<%end if%>
</SELECT>

<br>
<!--<%
	If tcWebApi_get("Wan_Common","TransMode","h") = "ATM" Then
		asp_Write("ATM")
	elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" Then
		asp_Write("PTM")
	elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" Then
		asp_Write("Ethernet")
	else asp_Write("ATM")
	end if
%>-->

		<%If tcWebApi_staticGet("DeviceInfo_PVC","DispBtnType","h") <> "0" then%>
			<INPUT TYPE="button" NAME="renewIP" class="button_gen" VALUE="Connect" onClick="renewrelease(1)" >
			<INPUT TYPE="button" NAME="releaseIP" class="button_gen" VALUE="Disconnect" onClick="renewrelease(2)" >
		<%elseif tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_Manually" then%>
			<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "0" then%>
				<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "ATM" then%>
					<INPUT TYPE="button" NAME="renewIP" class="button_gen" VALUE="Connect" onClick="reconnect(1)" >
					<INPUT TYPE="button" NAME="releaseIP" class="button_gen" VALUE="Disconnect" onClick="reconnect(2)" >
				<%End if%>
			<%End if%>
			<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
				<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then%>
					<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "PTM" then%>
						<INPUT TYPE="button" NAME="renewIP" class="button_gen" VALUE="Connect" onClick="reconnect(1)" >
						<INPUT TYPE="button" NAME="releaseIP" class="button_gen" VALUE="Disconnect" onClick="reconnect(2)" >
					<%End if%>
				<%End if%>
				<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "9" then%>
					<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "PTM" then%>
						<INPUT TYPE="button" NAME="renewIP" class="button_gen" VALUE="Connect" onClick="reconnect(1)" >
						<INPUT TYPE="button" NAME="releaseIP" class="button_gen" VALUE="Disconnect" onClick="reconnect(2)" >
					<%End if%>
				<%End if%>
			<%end if%>
			<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then%>
				<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "Ethernet" then%>
					<INPUT TYPE="button" NAME="renewIP" class="button_gen" VALUE="Connect" onClick="reconnect(1)" >
					<INPUT TYPE="button" NAME="releaseIP" class="button_gen" VALUE="Disconnect" onClick="reconnect(2)" >
				<%End if%>
			<%End if%>
			<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
				<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "12" then%>
					<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "LAN" then%>
						<INPUT TYPE="button" NAME="renewIP" class="button_gen" VALUE="Connect" onClick="reconnect(1)" >
						<INPUT TYPE="button" NAME="releaseIP" class="button_gen" VALUE="Disconnect" onClick="reconnect(2)" >
					<%End if%>
				<%End if%>
			<%End if%>
		<%End if%>

<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 5px 15px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","Connectiontype","s")%></p>
	<div id="div_wanType">
		<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
		<% if tcWebApi_get("DeviceInfo_PVC","connType","h") <> "" then tcWebApi_staticGet("DeviceInfo_PVC","connType","s")  end if %>
		</p>
	</div>
<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<!-- IPv4 -->
<tr>
<td style="padding:5px 10px 5px 15px;">
<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","WAN_IP","s")%></p>
	<div id="div_wanIP">
		<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
		<%tcWebApi_staticGet("DeviceInfo_PVC","WanIP","s")%>
		</p>
	</div>
<span id="wan_status" style="display:none"></span>
<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 5px 15px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","HSDPAC_DNSServers_in","s")%></p>
	<div id="div_wanDNS">
		<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
<script>
	var dns_ip2 = "<% tcWebApi_staticGet("DeviceInfo_PVC","DNSIP","s") %>";
	var dns_ip3 = "<% tcWebApi_staticGet("DeviceInfo_PVC","SECDNSIP","s") %>";

	if("<%tcWebApi_get("Wan_PVC","DNS_type","s")%>" == "1")
	{
		var pri_dns = "<%tcWebApi_get("Wan_PVC","Primary_DNS","s")%>";
		var scnd_dns = "<%tcWebApi_get("Wan_PVC","Secondary_DNS","s")%>";
		if(pri_dns != "")
		{
			document.write(pri_dns);
			if(scnd_dns != "")
				document.write("<br>" + scnd_dns);
		}
		else if(scnd_dns != "")
			document.write(scnd_dns);
	}
	else{
		document.write(dns_ip2);
		if(dns_ip3 != "")
			document.write("<br>" + dns_ip3);
	}
</script>
		</p>
	</div>
<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 5px 15px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","RC_GWStaticGW_in","s")%></p>
	<div id="div_wanGateway">
		<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
		<%tcWebApi_staticGet("DeviceInfo_PVC","WanDefGW","s")%>
		</p>
	</div>
<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<!-- IPv6 -->
<!-- <tr>
<td style="padding:5px 10px 5px 15px;">
<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","WAN_IP","s")%>(IPv6)</p>
<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
<script language="JavaScript" type="text/JavaScript">
	var str_IP6 = "<%tcWebApi_get("DeviceInfo_PVC","IP6WanIP","s")%>";
	if("N/A" != str_IP6){
		var str_ip6value = str_IP6;
		var vlen = str_IP6.indexOf('/');
		if(vlen != -1){
			str_ip6value = str_IP6.substring(0, vlen);
		}
		document.writeln(str_ip6value);
	}
	else{
		document.writeln(str_IP6);
	}
</script>&nbsp;&nbsp;
<%If tcWebApi_staticGet("DeviceInfo_PVC","IP6DispBtnType","h") <> "0" then%>
<INPUT TYPE="button" NAME="renewIP6" VALUE="renewIP6" onClick="renewrelease(3)" >
<INPUT TYPE="button" NAME="releaseIP6" VALUE="releaseIP6" onClick="renewrelease(4)">
<%End if%>
</p>
<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 5px 15px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","HSDPAC_DNSServers_in","s")%>(IPv6)</p>
<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6DNSIP","s")%>
</p>
<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>
<tr>
<td style="padding:5px 10px 5px 15px;">
<p class="formfonttitle_nwm" ><%tcWebApi_get("String_Entry","RC_GWStaticGW_in","s")%>(IPv6)</p>
<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6WanDefGW","s")%>
</p>
<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
</td>
</tr>-->
</table>
</form>
</body>

<!--device-map/internet.asp-->
</html>

