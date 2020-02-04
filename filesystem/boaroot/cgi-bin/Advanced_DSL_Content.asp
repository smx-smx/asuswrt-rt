<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%
	if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then
		set_dsl_restart_flag()
	end if
%>
<% disable_other_wan()%>
<%
If Request_Form("DvInfo_PVC") <> "" Then
	tcWebApi_set("WebCurSet_Entry","dev_pvc","DvInfo_PVC")
End If

If Request_Form("isIPv6Supported") = "0" Then
	tcWebApi_set("Wan_PVC","IPVERSION","IPVERSION_IPv4")
End If

If request_Form("wanVCFlag") = "3" Then
	if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
		tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_VC")
		tcWebApi_set("Wan_PVC","Active","wan_VCStatus")
		tcWebApi_set("Wan_PVC","VPI","Alwan_VPI")
		tcWebApi_set("Wan_PVC","VCI","Alwan_VCI")
		tcWebApi_set("Wan_PVC","QOS","Alwan_QoS")
		tcWebApi_set("Wan_PVC","PCR","wan_PCR")
		tcWebApi_set("Wan_PVC","SCR","wan_SCR")
		tcWebApi_set("Wan_PVC","MBS","wan_MBS")
	else
		tcWebApi_set("WebCurSet_Entry","wan_pvc","DvInfo_PVC")
		tcWebApi_set("Wan_PVC","Active","wan_TransStatus")
	end if

	tcWebApi_set("Wan_PVC","bridge_lan_port","IPTV_LANPort_Option")
	tcWebApi_set("Wan_PVC","ISP","wanTypeOption")
	tcWebApi_set("Wan_PVC","IPVERSION","ipVerRadio")
	tcWebApi_set("Wan_PVC","IPADDR6","wan_IPv6Addr")
	tcWebApi_set("Wan_PVC","PREFIX6","wan_IPv6Prefix")
	tcWebApi_set("Wan_PVC","DEFGATEWAY6","wan_IPv6DefGw")

	if Request_Form("wan_8021q") = "1" then
		if Request_Form("disp_wan_8021q") = "1" then
			tcWebApi_set("Wan_PVC","dot1q","wan_dot1q")
			if Request_Form("wan_dot1q") = "Yes" then
				tcWebApi_set("Wan_PVC","VLANID","wan_vid")
				if tcWebApi_get("WebCustom_Entry","isdot1pSupport","h") = "Yes" then
					tcWebApi_set("Wan_PVC","DOT1P","wan_dot1p")
				end if
				if Request_Form("isWanTagChk") = "Yes" then
					tcWebApi_set("Wan_PVC", "TAG", "TAGSEL")
				end if
				tcWebApi_set("IPTV_Entry","rmvlan","rmvlan")
				tcWebApi_CommitWithoutSave("IPTV_Entry")
			end if
		end if
	end if

	if request_Form("wanTypeOption") = "0" then	/*Automatic IP*/

		if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
			tcWebApi_set("Wan_PVC","ENCAP","wan_Encap0")
		end if

		if request_Form("wan_Encap0") = "1483 Bridged IP LLC" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
	  	elseif request_Form("wan_Encap0") = "1483 Bridged IP VC-Mux" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		end if

		if request_Form("IPv4_setting_flag") = "1" then
			tcWebApi_set("Wan_PVC","DNS_type","dnsTypeRadio")
			tcWebApi_set("Wan_PVC","Primary_DNS","PrimaryDns")
			tcWebApi_set("Wan_PVC","Secondary_DNS","SecondDns")
			tcWebApi_set("Wan_Common","NATENABLE","wan_NAT")
		end if	
		
		tcWebApi_set("Wan_PVC","MTU","wan_TCPMTU")
		tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP")
		tcWebApi_set("Wan_PVC","DIRECTION","wan_RIP_Dir")
		tcWebApi_set("Wan_PVC","IGMPproxy","wan_IGMP")

		If Request_Form("isIPv6Supported") = "1" Then
			tcWebApi_set("Wan_PVC","IPVERSION","ipVerRadio")
			tcWebApi_set("Wan_PVC","EnableDynIPv6","DynIPv6EnableRadio")
			if request_Form("DynIPv6EnableRadio") = "1" then
				tcWebApi_set("Wan_PVC","MLDproxy","wan_MLD0")
			else
				tcWebApi_set("Wan_PVC","MLDproxy","String_Value_No")
			end if
			if request_Form("IPv6PrivacyAddrsSupportedFlag") = "Yes" then
				if request_Form("DynIPv6EnableRadio") = "0" then
					tcWebApi_set("Wan_PVC","IPv6Extension","privacyaddrsradio")
				end if
			end if

			if tcwebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" then
				TCWebApi_set("Wan_PVC","RIPNGENABLE","ripngEnableRadio")
				If Request_Form("ripngEnableRadio") = "1" Then
					TCWebApi_set("Wan_PVC","RIPNGDIRECTION","wan_Ripng_Dir")
				End if
			end if

			tcWebApi_set("Wan_PVC","PPPv6PD","PPPIPv6PDRadio")

			If Request_Form("isDSLITESupported") = "1" Then
				If Request_Form("ipVerRadio") = "IPv6" Then
					tcWebApi_set("Wan_PVC","DSLITEEnable","DSLITEEnableRadio")
					tcWebApi_set("Wan_PVC","DSLITEMode","DSLITEModeRadio")
					If Request_Form("DSLITEModeRadio") = "1" Then
						tcWebApi_set("Wan_PVC","DSLITEAddr","DSLITEAddr")
					End if
				End if
			End if
	  	End if
	   	If Request_Form("is8021xsupport") = "1" Then
			tcWebApi_set("Wan_PVC","IPOE_DOT1X_STATUS","wan_status")
			tcWebApi_set("Wan_PVC","EAPIDENTITY","wan_eapIdentity")
			tcWebApi_set("Wan_PVC","BIDIRECTIONALAUTHENTICATION","wan_HiddenBiDirectionalAuth")
			tcWebApi_set("Wan_PVC","CERTIFICATE","wan_certificate")
			tcWebApi_set("Wan_PVC","TRUSTEDCA","wan_CA")
		End if

		tcWebApi_set("Wan_PVC","wan_hostname", "wan_hostname")
		tcWebApi_set("Wan_PVC","WAN_MAC", "wan_hwaddr_x")

		tcWebApi_set("Wan_PVC","dhcp_vendorid", "dhcp_vendorid")
		tcWebApi_set("Wan_PVC","dhcp_clientid_type", "dhcp_clientid_type")
		tcWebApi_set("Wan_PVC","dhcp_clientid", "dhcp_clientid")		

	elseif request_Form("wanTypeOption") = "1" Then	/*Static IP*/
		if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
			tcWebApi_set("Wan_PVC","ENCAP","wan_Encap1")
		end if

		if request_Form("wan_Encap1") = "1483 Bridged IP LLC" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
	  	elseif request_Form("wan_Encap1") = "1483 Bridged IP VC-Mux" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		end if

		if request_Form("IPv4_setting_flag") = "1" then
			tcWebApi_set("Wan_PVC","IPADDR","wan_StaticIPaddr")
			tcWebApi_set("Wan_PVC","NETMASK","wan_StaticIPSubMask")
			tcWebApi_set("Wan_PVC","GATEWAY","wan_StaticIpGateway")
			TCWebApi_set("Wan_PVC","DNS_type","dnsTypeRadio")
			TCWebApi_set("Wan_PVC","Primary_DNS","PrimaryDns")
			TCWebApi_set("Wan_PVC","Secondary_DNS","SecondDns")
			tcWebApi_set("Wan_Common","NATENABLE","wan_NAT")
		end if
		
		tcWebApi_set("Wan_PVC","MTU","wan_TCPMTU")
		tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP")
		tcWebApi_set("Wan_PVC","DIRECTION","wan_RIP_Dir")
		tcWebApi_set("Wan_PVC","IGMPproxy","wan_IGMP")

		if Request_Form("isIPv6Supported") = "1" Then

			tcWebApi_set("Wan_PVC","IPADDR6","wan_IPv6Addr")
			tcWebApi_set("Wan_PVC","PREFIX6","wan_IPv6Prefix")
			tcWebApi_set("Wan_PVC","DEFGATEWAY6","wan_IPv6DefGw")
			tcWebApi_set("Wan_PVC","DNSIPv61st","wan_IPv6DNS1")
			tcWebApi_set("Wan_PVC","DNSIPv62nd","wan_IPv6DNS2")
			tcWebApi_set("Wan_PVC","MLDproxy","wan_MLD0")

			if tcwebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" then
				TCWebApi_set("Wan_PVC","RIPNGENABLE","ripngEnableRadio")
				If Request_Form("ripngEnableRadio") = "1" Then
					TCWebApi_set("Wan_PVC","RIPNGDIRECTION","wan_Ripng_Dir")
				End if
			end if

			If Request_Form("DHCP6SMode_Flag") = "0" Then
				tcWebApi_set("Dhcp6s_Entry","DNSserver","wan_IPv6DNS1")
				tcWebApi_set("Dhcp6s_Entry","SecDNSserver","wan_IPv6DNS2")
				If request_Form("wanSaveFlag") = "1" Then
					tcWebApi_CommitWithoutSave("Dhcp6s_Entry")
				End If
			End If
			If Request_Form("isDSLITESupported") = "1" Then
				If Request_Form("ipVerRadio") = "IPv6" Then
					tcWebApi_set("Wan_PVC","DSLITEEnable","DSLITEEnableRadio")
					tcWebApi_set("Wan_PVC","DSLITEMode","DSLITE_Mnl_MODE")
					tcWebApi_set("Wan_PVC","DSLITEAddr","DSLITEAddr1")
				End if
			End if
		end If
		If Request_Form("is8021xsupport") = "1" Then
			tcWebApi_set("Wan_PVC","IPOE_DOT1X_STATUS","wan_status")
			tcWebApi_set("Wan_PVC","EAPIDENTITY","wan_eapIdentity")
			tcWebApi_set("Wan_PVC","BIDIRECTIONALAUTHENTICATION","wan_HiddenBiDirectionalAuth")
			tcWebApi_set("Wan_PVC","CERTIFICATE","wan_certificate")
			tcWebApi_set("Wan_PVC","TRUSTEDCA","wan_CA")
		End if

		tcWebApi_set("Wan_PVC","WAN_MAC", "wan_hwaddr_x")

	elseif request_Form("wanTypeOption") = "2" Then	/*PPPoE/PPPoA*/

		if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
			tcWebApi_set("Wan_PVC","ENCAP","wan_Encap2")
		end if

		if request_Form("wan_Encap2") = "PPPoE LLC" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		elseif request_Form("wan_Encap2") = "PPPoE VC-Mux" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface")
		end if

		tcWebApi_set("Wan_PVC","USERNAME","wan_PPPUsername")
			if request_Form("TTNETGuiSupport") = "1" then
				tcWebApi_set("Wan_PVC", "USERNAME", "username1")
			end if
		tcWebApi_set("Wan_PVC","PASSWORD","wan_PPPPassword")
			if request_Form("isPPPAuthen") = "Yes" then
				tcWebApi_set("Wan_PVC", "AUTHEN", "PPPAuthen")
			end if
		tcWebApi_set("Wan_PVC","CONNECTION","wan_ConnectSelect")
		tcWebApi_set("Wan_PVC","CLOSEIFIDLE","wan_IdleTimeT")
		tcWebApi_set("Wan_PVC","MSS","wan_TCPMSS")
		tcWebApi_set("Wan_PVC","wan_pppoe_hostuniq","wan_pppoe_hostuniq")
		tcWebApi_set("Wan_PVC","InetDetect","InetDetect")
		tcWebApi_set("Wan_PVC","lcpEchoInterval","lcpEchoInterval")
		tcWebApi_set("Wan_PVC","lcpEchoFailure","lcpEchoFailure")
		tcWebApi_set("Wan_PVC","wan_pppoe_options","wan_pppoe_options")
		if request_Form("ipv6SupportValue")="0" then
			 tcWebApi_set("Wan_PVC","MTU","wan_TCPMTU")
		end if

		If Request_Form("ipVerRadio") <> "IPv6" Then
			if request_Form("IPv4_setting_flag") = "1" then
				tcWebApi_set("Wan_PVC","PPPGETIP","wan_PPPGetIP")
				tcWebApi_set("Wan_PVC","IPADDR","wan_StaticIPaddr")
				tcWebApi_set("Wan_PVC","NETMASK","wan_StaticIPSubMask")
				tcWebApi_set("Wan_PVC","GATEWAY","wan_StaticIpGateway")
				TCWebApi_set("Wan_PVC","DNS_type","dnsTypeRadio")
				TCWebApi_set("Wan_PVC","Primary_DNS","PrimaryDns")
				TCWebApi_set("Wan_PVC","Secondary_DNS","SecondDns")
				tcWebApi_set("Wan_Common","NATENABLE","wan_NAT")
			end if			
			tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP")
			tcWebApi_set("Wan_PVC","DIRECTION","wan_RIP_Dir")
			tcWebApi_set("Wan_PVC","IGMPproxy","wan_IGMP")
		End if		

		If Request_Form("isIPv6Supported") = "1" Then
			tcWebApi_set("Wan_PVC","PPPv6Mode","PPPDHCPv6Mode_Flag")
			tcWebApi_set("Wan_PVC","PPPv6PD","PPPIPv6PDRadio")
			if request_Form("IPv6PrivacyAddrsSupportedFlag") = "Yes" then
				if request_Form("PPPDHCPv6Mode_Flag") = "0" then
					 tcWebApi_set("Wan_PVC","IPv6Extension","privacyaddrsradio")
				end if
			end if
			tcWebApi_set("Wan_PVC","MLDproxy","wan_MLD0")

			if tcwebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" then
				TCWebApi_set("Wan_PVC","RIPNGENABLE","ripngEnableRadio")
				If Request_Form("ripngEnableRadio") = "1" Then
					TCWebApi_set("Wan_PVC","RIPNGDIRECTION","wan_Ripng_Dir")
				End if
			end if

			If Request_Form("isDSLITESupported") = "1" Then
				If Request_Form("ipVerRadio") = "IPv6" Then
					tcWebApi_set("Wan_PVC","DSLITEEnable","DSLITEEnableRadio")
					tcWebApi_set("Wan_PVC","DSLITEMode","DSLITEModeRadio")
					If Request_Form("DSLITEModeRadio2") = "1" Then
						tcWebApi_set("Wan_PVC","DSLITEAddr","DSLITEAddr")
					End if
				End if
			End if
		End if

		tcWebApi_set("Wan_PVC","WAN_MAC", "wan_hwaddr_x")

	elseif request_Form("wanTypeOption") = "3" Then	/*Bridge*/
		if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
			tcWebApi_set("Wan_PVC","ENCAP","wan_Encap3")
		end if
	end if

	if request_Form("UPnP_active_flag") = "1" then
		TCWebApi_set("Upnpd_Entry","Active","UPnP_active")
	end if

	if tcWebApi_get("Wan_Common","TransMode","h") = "LAN" then
		tcWebApi_set("Dualwan_Entry", "wans_lanport", "wans_lanport")
	end if


	If request_Form("wanSaveFlag") = "1" Then
		tcWebApi_CommitWithoutSave("Wan_PVC")
		tcWebApi_CommitWithoutSave("Upnpd_Entry")
	End If

	tcWebApi_commit("Route")

ElseIf Request_Form("wanVCFlag")="1" Then
	if request_Form("wanTransFlag") = "1" Then
		tcWebApi_set("Wan_Common","TransMode","wan_TransMode")
		tcWebApi_set("WebCurSet_Entry","wan_pvc","DvInfo_PVC")

	elseif request_Form("wanBarrierFlag") = "1" Then
		tcWebApi_set("WebCurSet_Entry","wan_pvc","ptm_VC")

	elseif request_Form("service_num_flag") = "1" Then
 		tcWebApi_set("WebCurSet_Entry","wan_pvc_ext","service_num")
 		tcWebApi_set("WebCurSet_Entry","nat_pvc_ext","service_num")

	else
		tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_VC")

	end if

ElseIf Request_Form("wanVCFlag")="2" Then
	tcWebApi_unset("Wan_PVC")
	If Request_Form("wan_TransMode")="ATM" Then
		tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_VC")
	Else
		tcWebApi_set("WebCurSet_Entry","wan_pvc_ext","service_num")
	End If
	
	if  tcwebApi_get("WebCustom_Entry","isCZOTESupported","h")<>"Yes" then
		tcWebApi_set("Wan_PVC","ISP","DefaultWan_ISP")
		tcWebApi_set("Wan_PVC","Active","DefaultWan_Active")
		tcWebApi_set("Wan_PVC","VPI","DefaultWan_VPI")
		tcWebApi_set("Wan_PVC","VCI","DefaultWan_VCI")
		tcWebApi_set("Wan_PVC","QOS","DefaultWan_QOS")
		tcWebApi_set("Wan_PVC","PCR","DefaultWan_PCR")
		tcWebApi_set("Wan_PVC","SCR","DefaultWan_SCR")
		tcWebApi_set("Wan_PVC","MBS","DefaultWan_MBS")
		tcWebApi_set("Wan_PVC","IPVERSION","DefaultWan_IPVERSION")
		tcWebApi_set("Wan_PVC","ENCAP","DefaultWan_ENCAP")
		tcWebApi_set("Wan_PVC","MLDproxy","DefaultWan_MLDproxy")
	end if
	tcWebApi_commit("Wan_PVC")
End If

If Request_Form("wanSaveFlag")="1" Then
	If should_dsl_do_commit()="1" Then
		tcWebApi_commit("Adsl_Entry")
	End If
End If

%>
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_DSL_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_3_1","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script>

<% login_state_hook(); %>

var wireless = []; // [[MAC, associated, authorized], ...]
var dsl_pvc_selected = '0';
var dsl_pvc_enabled = ["0", "0", "0", "0", "0", "0", "0", "0"];
var wans_dualwan = '<%tcWebApi_Get("Dualwan_Entry", "wans_dualwan", "s")%>';
var wans_TransMode = '<%tcWebApi_get("Wan_Common","TransMode","s")%>';
var wan_pvc_orig = '<%tcWebApi_get("WebCurSet_Entry","wan_pvc","s")%>';
var wan_pvc_ext_orig = '<%tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","s")%>';
var wans_dualwan_orig = '<%tcWebApi_Get("Dualwan_Entry", "wans_dualwan", "s")%>';
var wans_flag = (wans_dualwan_orig.search("none") == -1) ? 1:0;
var switch_stb_x = "<%tcWebApi_get("IPTV_Entry","switch_stb_x","s")%>";

var pppoe_username = "<% tcWebApi_get("Wan_PVC","USERNAME","s")%>";
var pppoe_password = "<% tcWebApi_get("Wan_PVC","PASSWORD","s")%>";

var ipv6_active_orig = "<%tcWebApi_get("ipv6rd_Entry","Active","s")%>";	//Yes/No
var ipv6_TransMode_orig = "<%tcWebApi_get("ipv6rd_Entry","tunMode","s")%>";		//NATIVE/6RD/6TO4/6IN4
var IPVersion_orig = "<%tcWebApi_get("Wan_PVC","IPVERSION","s")%>";	//IPv4/ IPv4/IPv6 / IPv6

var original_ppp_echo = parseInt("<%tcWebApi_get("Wan_PVC","InetDetect","s")%>");		//Internet Detection, PPP Echo
var original_lcpEchoInterval = parseInt("<%tcWebApi_get("Wan_PVC","lcpEchoInterval","s")%>");	//lcpEchoInterval
var original_lcpEchoFailure = parseInt("<%tcWebApi_get("Wan_PVC","lcpEchoFailure","s")%>");	//lcpEchoFailure

var DSLWANList = [<% get_DSL_WAN_list() %>];

var wans_lanport_orig = parseInt("<% tcWebApi_Get("Dualwan_Entry", "wans_lanport", "s") %>");
var wan_TransMode_orig = "<% tcWebApi_get("Wan_Common","TransMode","s") %>";
var ad_mr_enable = "<% TCWebApi_get("IPTV_Entry","ad_mr_enable_x","s") %>";
var vd_mr_enable = "<% TCWebApi_get("IPTV_Entry","vd_mr_enable_x","s") %>";
var eth_mr_enable = "<% TCWebApi_get("IPTV_Entry","eth_mr_enable_x","s") %>";
var ad_wan_port = "<% TCWebApi_get("IPTV_Entry","ad_wan_port","s") %>";
var vd_wan_port = "<% TCWebApi_get("IPTV_Entry","vd_wan_port","s") %>";
var eth_wan_port = "<% TCWebApi_get("IPTV_Entry","eth_wan_port","s") %>";
function showDSLWANList(){
	var addRow;
	var cell = new Array(10);
	var config_num = 0;
	for(var i = 0; i < DSLWANList.length; i++){
		if (DSLWANList[i][0] == "Yes") {
			config_num++;
		}
	}
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if(config_num == 0){
		addRow = document.getElementById('ATMPVCTable').insertRow(2);
		cell[0] = addRow.insertCell(0);
		cell[0].colSpan = "8";
		cell[0].innerHTML = "<center><%tcWebApi_get("String_Entry","no_data","s")%></center>";
		cell[0].style.color = "white";
	}
	else{
		var row_count=0;
		for(var i = 0; i < DSLWANList.length; i++){
			if (DSLWANList[i][0] == "Yes") {
				addRow = document.getElementById('ATMPVCTable').insertRow(row_count+2);
				row_count++;

				for (var j = 0; j < 3; j++) {
					cell[j] = addRow.insertCell(j);
					cell[j].innerHTML = "<center>"+DSLWANList[i][j+1]+"</center>";
					cell[j].style.color = "white";
				}
				cell[3] = addRow.insertCell(3);
				if (DSLWANList[i][4]=="0") {
					cell[3].innerHTML = "<center>Automatic IP</center>";
				}
				else if (DSLWANList[i][4]=="1") {
					cell[3].innerHTML = "<center>Static IP</center>";
				}
				else if (DSLWANList[i][4]=="2") {
					cell[3].innerHTML = "<center><%if tcWebApi_get("Wan_Common","sharepvc","h") <> "1" then%>PPPoA / <%end if%>PPPoE</center>";
				}
				else {
					cell[3].innerHTML = "<center>BRIDGE</center>";
				}
				for (var k = 4; k <= 5 ; k++) {
					cell[k] = addRow.insertCell(k);
					cell[k].innerHTML = "<center>"+DSLWANList[i][k+1]+"</center>";
					cell[k].style.color = "white";
				}

				cell[6] = addRow.insertCell(6);
				if (i==0) {
					cell[6].innerHTML = "<center><img src=\"/images/checked.gif\" border=\"0\"></center>";
				}
				else {
					cell[6].innerHTML = "";
				}
				cell[6].style.color = "white";
				
				cell[7] = addRow.insertCell(7);
				if ((ad_mr_enable == "1" && i == ad_wan_port) || DSLWANList[i][4]=="3") {	//UGMP Proxy port matched or BEIDGE
					cell[7].innerHTML = "<center><img src=\"/images/checked.gif\" border=\"0\"></center>";
				}
				else {
					cell[7].innerHTML = "";
				}
				cell[7].style.color = "white";
			}
		}
	}
<%else%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<%if tcWebApi_get("Wan_Common","TransMode","h") <> "LAN" then%>
	if(config_num == 0){
		addRow = document.getElementById('MultiServiceTable').insertRow(2);
		cell[0] = addRow.insertCell(0);
		cell[0].colSpan = "8";
		cell[0].innerHTML = "<center><%tcWebApi_get("String_Entry","no_data","s")%></center>";
		cell[0].style.color = "white";
	}
	else{
		var row_count=0;
		for(var i = 0; i < DSLWANList.length; i++){
			if (DSLWANList[i][0] == "Yes") {
				addRow = document.getElementById('MultiServiceTable').insertRow(row_count+2);
				row_count++;

				cell[0] = addRow.insertCell(0);
				var service_num_temp = parseInt(DSLWANList[i][1])+1;
				cell[0].innerHTML = "<center>Service "+service_num_temp+"</center>";				

				for (var j = 1; j < 3; j++) {
					cell[j] = addRow.insertCell(j);
					cell[j].innerHTML = "<center>"+DSLWANList[i][j+1]+"</center>";
					cell[j].style.color = "white";
				}

				cell[3] = addRow.insertCell(3);
				if (DSLWANList[i][4]=="0") {
					cell[3].innerHTML = "<center>Automatic IP</center>";
				}
				else if (DSLWANList[i][4]=="1") {
					cell[3].innerHTML = "<center>Static IP</center>";
				}
				else if (DSLWANList[i][4]=="2") {
					cell[3].innerHTML = "<center>PPPoE</center>";
				}
				else {
					cell[3].innerHTML = "<center>BRIDGE</center>";
				}

				cell[4] = addRow.insertCell(4);
				if (i==0) {
					cell[4].innerHTML = "<center><img src=\"/images/checked.gif\" border=\"0\"></center>";
				}
				else {
					cell[4].innerHTML = "";
				}
				cell[4].style.color = "white";

				cell[5] = addRow.insertCell(5);
				if(wan_TransMode_orig == "PTM" && ((vd_mr_enable == "1" && i == vd_wan_port) || DSLWANList[i][4]=="3")) {	//UGMP Proxy port matched or BEIDGE
					cell[5].innerHTML = "<center><img src=\"/images/checked.gif\" border=\"0\"></center>";
				}
				else if(wan_TransMode_orig == "Ethernet" && ((eth_mr_enable == "1" && i == eth_wan_port) || DSLWANList[i][4]=="3")) {	//UGMP Proxy port matched or BEIDGE
					cell[5].innerHTML = "<center><img src=\"/images/checked.gif\" border=\"0\"></center>";
				}
				else {
					cell[5].innerHTML = "";
				}
				cell[5].style.color = "white";
			}
		}
	}
<%end if%>
<%end if%>
<%end if%>
}

function isForInternet() {
	if( (wans_TransMode == "ATM" && wan_pvc_orig == "0")
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	|| ((wans_TransMode == "PTM" || wans_TransMode == "Ethernet") && wan_pvc_ext_orig == "0")
<%else%>
	|| (wans_TransMode == "PTM" || wans_TransMode == "Ethernet")
<%end if%>
	|| (wans_TransMode == "LAN")
	)
		return 1;
	else
		return 0;
}

// "ATM"	ADSL WAN (ATM)
// "PTM"	VDSL WAN (PTM)
// "Ethernet"		Ethernet WAN
// "USB"	USB Modem
// "LAN"	Ethernet LAN
function genWANSoption(){
	free_options(document.form.wan_TransMode);
	for(i=0; i<wans_dualwan.split(" ").length; i++){
		if(wans_dualwan.split(" ")[i].toUpperCase() == "DSL"){
		<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
			var opt = document.createElement('option');
			opt.value = "ATM";
			opt.innerHTML = "ADSL WAN (ATM)";
			if(wans_TransMode == "ATM")
					opt.selected = true;
			document.form.wan_TransMode.appendChild(opt);
		<%end if%>
		<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			var opt1 = document.createElement('option');
			opt1.value = "PTM";
			opt1.innerHTML = "VDSL WAN (PTM)";
			if(wans_TransMode == "PTM")
					opt1.selected = true;
			document.form.wan_TransMode.appendChild(opt1);
		<%end if%>
		}
		if(wans_dualwan.split(" ")[i].toUpperCase() == "WAN"){
			var opt2 = document.createElement('option');
			opt2.value = "Ethernet";
			opt2.innerHTML = "Ethernet WAN";
			if(wans_TransMode == "Ethernet")
				opt2.selected = true;
			document.form.wan_TransMode.appendChild(opt2);
		}
		if(wans_dualwan.split(" ")[i].toUpperCase() == "USB"){
			var opt3 = document.createElement('option');
			opt3.value = "USB";
			opt3.innerHTML = "USB Modem";
			document.form.wan_TransMode.appendChild(opt3);
		}
		if(wans_dualwan.split(" ")[i].toUpperCase() == "LAN"){
			var opt4 = document.createElement('option');
			opt4.value = "LAN";
			opt4.innerHTML = "Ethernet WAN";
			if(wans_TransMode == "LAN")
				opt4.selected = true;
			document.form.wan_TransMode.appendChild(opt4);
		}
	}
}

function initial(){
	show_menu();

	if(dualWAN_support != "-1" && wans_flag)
		genWANSoption();

<%if tcWebApi_get("Wan_Common","sharepvc","h") = "1" then%>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	//if single band, show only 4 pvc unit.
	if(!wl_info.band5g_support) {
		DSLWANList.splice(4, 4);
		document.getElementById("wan_VC").options.remove(7);
		document.getElementById("wan_VC").options.remove(6);
		document.getElementById("wan_VC").options.remove(5);
		document.getElementById("wan_VC").options.remove(4);
	}
<%end if%>
<%end if%>

	showDSLWANList();

	//hidden some settings
	showhide("wan_DynamicRoute", 0);
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then%>
	showhide("ptm_barrier", 0);
<%end if%>
<%end if%>

	if(document.form.wan_8021q.value != 1 || document.form.disp_wan_8021q.value != 1){
		showhide("div_8021q", 0);
	}

	<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
		doQos();
		showhide("SummaryTable", 1);
		showhide("ATMPVCTable", 1);
		<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
			showhide("MultiServiceTable", 0);
		<%end if%>		
	<%else%>
		showhide("ATMPVCTable", 0);
		<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
			<%if tcWebApi_get("Wan_Common","TransMode","h") = "LAN" then%>
				showhide("MultiServiceTable", 0);
				showhide("SummaryTable", 0);
				if(productid == "DSL-AC51" || productid == "DSL-N16P"){	//-- MODELDEP:DSL-AC51 (odmpid:DSL-AC750)/DSL-N16P LAN port 1,2 only
					document.form.wans_lanport.remove(3);
					document.form.wans_lanport.remove(2);
				}
				document.form.wanTypeOption.remove(3);	//remove BRIDGE				
			<%elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then%>

				<%if tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-N66U" then%>
				//modeldep: DSL-N66U not support IPTV through Ethernet WAN
				showhide("MultiServiceTable", 0);
				showhide("SummaryTable", 0);
				inputCtrl(document.form.service_num, 0);
				document.form.wanTypeOption.remove(3);  //remove BRIDGE
				<%else%>
				showhide("MultiServiceTable", 1);
				showhide("SummaryTable", 1);
				//remove bridge if service number is 0, lock to bridge if 1~7
				if(document.form.service_num.selectedIndex == 0) {
					document.form.wanTypeOption.remove(3);  //remove BRIDGE
				}
				<%end if%>
			<%else%>	
				//PTM
				showhide("MultiServiceTable", 1);
				showhide("SummaryTable", 1);
				/*keep bridge if service number is 0, lock to bridge if 1~7
				if(document.form.service_num.selectedIndex == 0) {
					document.form.wanTypeOption.remove(3);	//remove BRIDGE
				}*/
			<%end if%>
		<%else%>
			showhide("SummaryTable", 0);
			if(document.form.wan_TransMode.value == "Ethernet" || document.form.wan_TransMode.value == "LAN")
				document.form.wanTypeOption.remove(3);	//remove BRIDGE
			if(document.form.wan_TransMode.value == "LAN"){
				if(productid == "DSL-AC51" || productid == "DSL-N16P"){	//-- MODELDEP:DSL-AC51 (odmpid:DSL-AC750)/DSL-N16P LAN port 1,2 only
					document.form.wans_lanport.remove(3);
					document.form.wans_lanport.remove(2);
				}	
			}
		<%end if%>

	<%end if%>

	if(document.form.wanTypeOption.selectedIndex == 2)	//pppoa, pppoe
		pppStaticCheck();

	//PVC1 / Service1 : IPversion setting by IPv6
	if(isForInternet()){
		Limited_IP_version();
	}

	doConTypeChange();	

	if(pppoe_username != "")
		document.form.wan_PPPUsername.value = pppoe_username;

	if(pppoe_password != "")
		document.form.wan_PPPPassword.value = pppoe_password;
	
}

function Limited_IP_version(){
	if(ipv6_active_orig == "Yes" && ipv6_TransMode_orig == "NATIVE"){
		document.form.ipVerRadio[0].disabled = true;
		if(IPVersion_orig == "IPv4"){
			document.form.ipVerRadio[1].checked = true;
		}
	}
	else if(ipv6_active_orig == "Yes" && (ipv6_TransMode_orig == "6RD" || ipv6_TransMode_orig == "6TO4" || ipv6_TransMode_orig == "6IN4")){
		document.form.ipVerRadio[0].checked = true;
		document.form.ipVerRadio[1].disabled = true;
		document.form.ipVerRadio[2].disabled = true;
	}
}

function addWANOption(obj, wanscapItem){
	free_options(obj);

	for(i=0; i<wanscapItem.length; i++){
		if(wanscapItem[i].length > 0){
			var wanscapName = wanscapItem[i].toUpperCase();
			if(wanscapName == "LAN")
				wanscapName = "Ethernet WAN";
			else if(wanscapName == "WAN")
				wanscapName = "Ethernet WAN";
			obj.options[i] = new Option(wanscapName, wanscapItem[i]);
		}
	}
}

function applyRule(){
	var form = document.form;
	var transModeSupport = 0;

<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	transModeSupport += 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	transModeSupport += 2;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	transModeSupport += 4;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveUSBModem","h") = "Yes" then%>
	transModeSupport += 8;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
	transModeSupport += 16;
<%end if%>

	if(dualWAN_support == "-1" || !wans_flag){		//no support Dualwan or disabled
		pvc=form.wan_TransMode.selectedIndex;

		switch(transModeSupport) {
			case 5:	//ATM + ETHER, ex. DSL-N17VPN
				if(pvc==1) //ether
					pvc = 10;
				break;
			case 7:	//ATM + PTM + ETHER, ex. DSL-N66U
				if(pvc==1) {//ptm
					if(form.ptm_Barrier.selectedIndex == 0)
						pvc = 8;
					else
						pvc = 9;
				}
				else if(pvc==2) //ether
					pvc = 10;
				break;
			case 9: //ATM + USB, ex. DSL-N12U-C1
				if(pvc==1) //USB
					pvc = 11;
				break;
			case 13:	//ATM + ETHER + USB, ex.
				if(pvc==1) //ether
					pvc = 10;
				else if(pvc==2) //USB
					pvc = 11;
				break;
			case 15:	//ATM + PTM + ETHER + USB, ex.
				if(pvc==1) //ptm
					pvc = 8;
				else if(pvc==2) //ether
					pvc = 10;
				else if(pvc==3) //USB
					pvc = 11;
				break;
			case 17:	//ATM + ETHER_LAN
				if(pvc==1) //ETHER_LAN
					pvc = 12;
				break;
			case 19:	//ATM + PTM + ETHER_LAN
				if(pvc==1) //PTM
					pvc = 8;
				else if(pvc==2) //ETHER_LAN
					pvc = 12;
				break;
			case 25:	//ATM + USB + ETHER_LAN
				if(pvc==1) //USB
					pvc = 11;
				else if(pvc==2) //ETHER_LAN
					pvc = 12;
				break;
			case 27:	//ATM + PTM + USB + ETHER_LAN
				if(pvc==1) //PTM
					pvc = 8;
				else if(pvc==2) //USB
					pvc = 11;
				else if(pvc==3) //ETHER_LAN
					pvc = 12;
				break;
			//add other case for other product if necessary
		}
		form.DvInfo_PVC.value = pvc;


	}
	else{		//DualWAN enabled
		if(form.wan_TransMode.value == "ATM")
			form.DvInfo_PVC.value = "0";
		else if(form.wan_TransMode.value == "PTM")
			form.DvInfo_PVC.value = "8";
		else if(form.wan_TransMode.value == "LAN")
			form.DvInfo_PVC.value = "12";
		else
			form.DvInfo_PVC.value = "10";
	}

<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if(!validPVC())
		return;
	if(!validQoS())
		return;
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
	if(form.wan_TransMode.value == "LAN") {
		var port_conflict = false;
		var lan_port_num = form.wans_lanport.value;

		if(switch_stb_x == lan_port_num)
			port_conflict = true;
		else if (switch_stb_x == '5' && (lan_port_num == '1' || lan_port_num == '2'))
			port_conflict = true;
		else if (switch_stb_x == '6' && (lan_port_num == '3' || lan_port_num == '4'))
			port_conflict = true;

		if (port_conflict) {
			alert("<%tcWebApi_get("String_Entry","RC_IPTV_conflict","s")%>");
			return;
		}
	}
<%end if%>

	if(validForm()){
		form.wanSaveFlag.value = 1;
		form.wanVCFlag.value = 3;

<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		if(form.wan_dot1q.value == "Yes")
			form.rmvlan.value = "1";
<%end if%>

		showLoading(13);
		setTimeout("redirect();", 13000);
		form.target = "hidden_frame";
		form.submit();
	}
}

function validPVC() {
	var vpi = document.form.Alwan_VPI.value;
	var vci = document.form.Alwan_VCI.value;
	var pvc = document.form.wan_VC.value;
<%if tcWebApi_get("Wan_Common","sharepvc","h") <> "1" then%>
	for(var i = 0; i < DSLWANList.length; i++){
		if(i == pvc)
			continue;
		if (vpi == DSLWANList[i][2] && vci == DSLWANList[i][3] && DSLWANList[i][0] == "Yes") {
			alert("VPI VCI already used by another PVC");
			return false;
		}
	}
<%end if%>
	if(!validInteger(vpi)) {
		return false;
	}

	vpi_num = Number(vpi);
	if(vpi_num > 255 || vpi_num < 0) {
		alert("<%tcWebApi_get("String_Entry","JSVPIInvalid","s")%>");
		return false;
	}

	if(!validInteger(vci)) {
		return false;
	}

	vci_num = Number(vci);
	if(vci_num > 65535 || vci_num < 32) {
		alert("<%tcWebApi_get("String_Entry","JSVCIInvalid","s")%>");
		return false;
	}

	return true;
}

function validInteger(Integer)
{
   if(Integer.match("^[0-9]+"))
   	return true;
   else
	return false;
}

function validQoS() {
	var pcr = document.form.wan_PCR.value;
	var scr = document.form.wan_SCR.value;
	var mbs = document.form.wan_MBS.value;

	if(!validInteger(pcr)) {
		alert("<%tcWebApi_get("String_Entry","JSPCRInvalid","s")%>");
		return false;
	}

	pcr_num = Number(pcr);
	if(pcr_num > 5500 || pcr_num < 0) {
		alert("<%tcWebApi_get("String_Entry","JSPCRSizeInvalid","s")%>");
		return false;
	}

	switch(document.form.Alwan_QoS.selectedIndex) {
		case 2:
		case 3:
			if(!validInteger(scr)) {
				alert("<%tcWebApi_get("String_Entry","JSSCRInvalid","s")%>");
				return false;
			}

			scr_num = Number(scr);
			if(scr_num > pcr_num) {
				alert("<%tcWebApi_get("String_Entry","JSSCRSizeInvalid","s")%>");
				return false;
			}

			if(!validInteger(mbs)) {
				alert("<%tcWebApi_get("String_Entry","JSMBSInvalid","s")%>");
				return false;
			}

			mbs_num = Number(mbs);
			if(mbs_num > 65535 || mbs_num < 0) {
				alert("<%tcWebApi_get("String_Entry","JSMBSSizeInvalid","s")%>");
				return false;
			}

			break;
	}
	return true;
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
		alert(ip_obj.value+" is not a valid IP address!");
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
	else if(ip_num > C_class_start && ip_num < C_class_end)
		return true;
	else{
		alert(ip_obj.value+" is not a valid IP address!");
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
}

function inValidSubnetMask(Mask)
{
	var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var bMask = 0;
	var watch = false;
	var i;

	if(mask == null)
	{
	  alert(Mask +" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
	  document.form.wan_StaticIPSubMask.focus();
	  return true;
	}
	digits = mask[0].split(".");
	for(i=0; i < 4; i++)
	{
	if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || digits[i] == null)
	{
	     alert(Mask +" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
	     document.form.wan_StaticIPSubMask.focus();
	     return true;
	}
	bMask = (bMask << 8) | Number(digits[i]);
	}
	//edited by Janey
	if((Number(digits[0]) == 0))
	{
	     alert(Mask +" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
	     document.form.wan_StaticIPSubMask.focus();
	     return true;
	}

	bMask = bMask & 0x0FFFFFFFF;
	for(i=0; i<32; i++)
	{
	if((watch==true) && ((bMask & 0x1)==0)) {
	alert(Mask +" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
	document.form.wan_StaticIPSubMask.focus();
	return true;
	}
	bMask = bMask >> 1;
	if((bMask & 0x01) == 1) watch=true;
	}
	return false;
}

function inValidIPv6Addr(Address1)
{
	var regExp = /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/;

	var Address = trim(Address1);
	var address = Address.match(regExp);
	if(address == null){
		alert(Untranslated.IPv6_addr_validation + ": " + Address);
 		return true;
	}
	return false;
}

function inValidIPv6Prefix(Prefix1)
{
	var Prefix = trim(Prefix1);
	var IPv6Prefix = Prefix.match("^[0-9]{1,3}$");

	if(IPv6Prefix == null) {
 		alert(Untranslated.IPv6_prefix_validation + ": " + Prefix);
 		return true;
	}

	if((Number(Prefix) > 128 ) || (Number(Prefix) < 1 )){
		alert(Untranslated.IPv6_prefix_validation + ": " + Prefix);
		return true;
	}

	return false;
}

function validForm(){
	var form = document.form;
	switch(form.wanTypeOption.selectedIndex) {
		case 0:	//dynamic
			//mtu
			if(!validate_range(form.wan_TCPMTU, 576, 1500)) {
				return false;
			}

			//ipv6, dual stack lite
			//if(form.isIPv6Supported.value){	//ipv6 should be supported
				if(form.isDSLITESupported.value){
					if(form.ipVerRadio[2].checked && form.DSLITEEnableRadio[0].checked){
						if(form.DSLITEModeRadio[1].checked){
							if(inValidIPv6Addr(DSLITEAddr))
								return false;
						}
					}
				}
			//}
			break;
		case 1:	//static
			//mtu
			if(!validate_range(form.wan_TCPMTU, 100, 1500)) {
				return false;
			}

			//ipv4
			if(form.ipVerRadio[0].checked || form.ipVerRadio[1].checked){
				if(!valid_IP(form.wan_StaticIPaddr, ""))
					return false;
				if(!valid_IP(form.wan_StaticIpGateway, "GW"))
					return false;
				if(inValidSubnetMask(form.wan_StaticIPSubMask.value))
					return false;
				if(form.wan_StaticIPaddr.value == form.wan_StaticIpGateway.value) {
					form.wan_StaticIPaddr.focus();
					alert("<%tcWebApi_get("String_Entry","IPC_warning_WANIPEQUALGatewayIP","s")%>");
					return false;
				}
				
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>	
				<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
					if(form.wan_VC.value == 0){
				<%elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then%>
					if(form.service_num.value == 0){
				<%elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then%>
					<%if tcWebApi_get("SysInfo_Entry","ProductName","h") = "DSL-N66U" then%>
						if(1==0){
					<%else%>
						if(form.service_num.value == 0){
					<%end if%>
				<%else%>
					if(1==0){
				<%end if%>
			<%else%>
				if(1==0){
			<%end if%>
					
					if(form.PrimaryDns.value == "" && form.SecondDns.value == "") {
						form.PrimaryDns.focus();					
						alert("<%tcWebApi_get("String_Entry","IPC_x_DNSServer_blank","s")%>");
						return false;
					}
					if(!valid_IP(form.PrimaryDns, "DNS"))
						return false;
					if(!valid_IP(form.SecondDns, "DNS"))
						return false;
					//force enable dns
					form.dnsTypeRadio[1].checked = 1;
				}
			}

			//ipv6
			//if(form.isIPv6Supported.value){
				if(form.ipVerRadio[1].checked || form.ipVerRadio[2].checked) {
					if(inValidIPv6Addr(form.wan_IPv6Addr.value))
						return false;
					if(inValidIPv6Addr(form.wan_IPv6DefGw.value))
						return false;
					if(inValidIPv6Addr(form.wan_IPv6DNS1.value))
						return false;
					if(inValidIPv6Addr(form.wan_IPv6DNS2.value))
						return false;
					if(inValidIPv6Prefix(form.wan_IPv6Prefix.value))
						return false;

					if(form.isDSLITESupported.value){
						if(form.ipVerRadio[2].checked && form.DSLITEEnableRadio[0].checked){
							if(form.DSLITEModeRadio[1].checked){
								if(inValidIPv6Addr(DSLITEAddr))
									return false;
							}
						}
					}
				}
			//}
			break;
		case 2:	//pppoa, pppoe
			if(idleTimeCheck())
				return ;

			//account
			if(form.wan_PPPUsername.value.length <= 0){
				alert("<%tcWebApi_get("String_Entry","WANJS9Text","s")%>");
				form.wan_PPPUsername.focus();
				return;
			}
			if(!validate_string(form.wan_PPPUsername)){
				alert("<%tcWebApi_get("String_Entry","WANJS19Text","s")%>");
				form.wan_PPPUsername.focus();
				return;
			}
			if(form.wan_PPPPassword.value.length <= 0){
				alert("<%tcWebApi_get("String_Entry","WANJS9Text","s")%>");
				form.wan_PPPPassword.focus();
				return;
			}	
			
			if(!validate_string(form.wan_PPPPassword)){
				alert("<%tcWebApi_get("String_Entry","WANJS20Text","s")%>");
				form.wan_PPPPassword.focus();
				return;
			}			

			//MTU
			var mtu_num = Number(form.wan_TCPMTU.value);
			if(!validate_range(form.wan_TCPMTU, 100, 1492)) {
				form.wan_TCPMTU.focus();
				return false;
			}

			//MSS
			mss = form.wan_TCPMSS.value;
			if(!validInteger(mss)) {
				alert('MSS must be digits!');
				form.wan_TCPMSS.focus();
				return false;
			}
			mss_num = Number(mss);
			if((mss_num > 1452 || mss_num < 100) && mss_num != 0) {
				alert("<%tcWebApi_get("String_Entry","WANJS10Text","s")%>");
				form.wan_TCPMSS.focus();
				return false;
			}
			if(( mss_num!=0 && mtu_num!=0 && mss_num > mtu_num-40 ) || (mss_num==0 && mtu_num!=0 &&mtu_num!=1492))
			{
				alert("<%tcWebApi_get("String_Entry","WANJS12Text","s")%>");
				form.wan_TCPMSS.focus();
				return false;
			}

			//wan_pppoe_hostuniq
			if(!validator.hex(form.wan_pppoe_hostuniq)) {
				alert("Host-uniq should be hexadecimal digits.");
				form.wan_pppoe_hostuniq.focus();
				form.wan_pppoe_hostuniq.select();
				return false;
			}

			// InetDetect 


			//IPv4
			if(form.ipVerRadio[0].checked || form.ipVerRadio[1].checked){
				if(form.wan_PPPGetIP[1].checked) {
					if(!valid_IP(form.wan_StaticIPaddr, ""))
						return false;
					if(!valid_IP(form.wan_StaticIpGateway, "GW"))
						return false;
					if(inValidSubnetMask(form.wan_StaticIPSubMask.value))
						return false;
					if(form.wan_StaticIPaddr.value == form.wan_StaticIpGateway.value) {
						form.wan_StaticIPaddr.focus();
						alert("<%tcWebApi_get("String_Entry","IPC_warning_WANIPEQUALGatewayIP","s")%>");
						return false;
					}
				}
				if(form.dnsTypeRadio[1].checked) {
					if(form.PrimaryDns.value == "" && form.SecondDns.value == "") {
						form.PrimaryDns.focus();
						alert("<%tcWebApi_get("String_Entry","IPC_x_DNSServer_blank","s")%>");
						return false;
					}
					if(!valid_IP(form.PrimaryDns, "DNS"))
						return false;
					if(!valid_IP(form.SecondDns, "DNS"))
						return false;
				}
			}

			//IPv6
			if(form.isDSLITESupported.value){
				if(form.ipVerRadio[2].checked && form.DSLITEEnableRadio[0].checked){
					if(form.DSLITEModeRadio[1].checked){
						if(inValidIPv6Addr(DSLITEAddr))
							return false;
					}
				}
			}

			break;
	}

	if(document.form.wan_hostname.value.length > 0){
		var alert_str = validate_hostname(document.form.wan_hostname);

		if(alert_str != ""){
			showtext(document.getElementById("alert_msg1"), alert_str);
			document.getElementById("alert_msg1").style.display = "";
			document.form.wan_hostname.focus();
			document.form.wan_hostname.select();
			return false;
		}else{
			document.getElementById("alert_msg1").style.display = "none";
		}

		document.form.wan_hostname.value = trim(document.form.wan_hostname.value);
	}

	if(document.form.wan_hwaddr_x.value.length > 0){
		if(!check_macaddr(document.form.wan_hwaddr_x,check_hwaddr_flag(document.form.wan_hwaddr_x))){
				document.form.wan_hwaddr_x.select();
				document.form.wan_hwaddr_x.focus();
			return false;
		}
	}

	if(document.form.wan_dot1q.value == "Yes"){
		if(!validate_number_range(document.form.wan_vid, 1, 4094))
			return false;
		<%if tcWebApi_get("WebCustom_Entry","isdot1pSupport","h") = "Yes" then %>
		if(!validate_number_range(document.form.wan_dot1p, 0, 7))
			return false;
		<%end if%>
	}

	return true;
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_DSL_Content.asp";
}

function doDelete() {
	document.form.wanVCFlag.value = 2;
	showLoading(5);
	setTimeout("redirect();", 5000);
	form.target = "hidden_frame";
	document.form.submit();
}

function doTransChange() {
	var pvc, transModeSupport = 0;

<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	transModeSupport += 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	transModeSupport += 2;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	transModeSupport += 4;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveUSBModem","h") = "Yes" then%>
	transModeSupport += 8;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
	transModeSupport += 16;
<%end if%>

	if(dualWAN_support == "-1" || !wans_flag){		//no support Dualwan or disabled
		pvc=document.form.wan_TransMode.selectedIndex;
		switch(transModeSupport) {
			case 5:	//ATM + ETHER
				if(pvc==1) //ether
					pvc = 10;
				break;
			case 7:	//ATM + PTM + ETHER
				if(pvc==1) //ptm
					pvc = 8;
				else if(pvc==2) //ether
					pvc = 10;
				break;
			case 9: //ATM + USB
				if(pvc==1) //USB
					pvc = 11;
				break;
			case 13:	//ATM + ETHER + USB
				if(pvc==1) //ether
					pvc = 10;
				else if(pvc==2) //USB
					pvc = 11;
				break;
			case 15:	//ATM + PTM + ETHER + USB
				if(pvc==1) //ptm
					pvc = 8;
				else if(pvc==2) //ether
					pvc = 10;
				else if(pvc==3) //USB
					pvc = 11;
				break;
			case 17:	//ATM + ETHER_LAN
				if(pvc==1) //ETHER_LAN
					pvc = 12;
				break;
			case 19:	//ATM + PTM + ETHER_LAN
				if(pvc==1) //PTM
					pvc = 8;
				else if(pvc==2) //ETHER_LAN
					pvc = 12;
				break;
			case 25:	//ATM + USB + ETHER_LAN
				if(pvc==1) //USB
					pvc = 11;
				else if(pvc==2) //ETHER_LAN
					pvc = 12;
				break;
			case 27:	//ATM + PTM + USB + ETHER_LAN
				if(pvc==1) //PTM
					pvc = 8;
				else if(pvc==2) //USB
					pvc = 11;
				else if(pvc==3) //ETHER_LAN
					pvc = 12;
				break;
		}
		document.form.DvInfo_PVC.value = pvc;
		if(document.form.wan_TransMode.value == "USB"){
			document.usbform.submit();
			return;
		}

	}
	else{		//DualWAN enabled
		if(document.form.wan_TransMode.value == "ATM")
			document.form.DvInfo_PVC.value = "0";
		else if(document.form.wan_TransMode.value == "PTM")
			document.form.DvInfo_PVC.value = "8";
		else if(document.form.wan_TransMode.value == "USB"){
			document.usbform.submit();
			return;
		}
		else if(document.form.wan_TransMode.value == "LAN")
			document.form.DvInfo_PVC.value = "12";
		else
			document.form.DvInfo_PVC.value = "10";
	}

	document.form.wanVCFlag.value = 1;
	document.form.wanTransFlag.value = 1;
	document.form.submit();
	return;
}

function doBarrierChange(){
	var pvc;

	document.form.DvInfo_PVC.value = "8";
	pvc = document.form.ptm_Barrier.selectedIndex;
	pvc += 8;
	document.form.ptm_VC.value = pvc;
	document.form.wanVCFlag.value = 1;
	document.form.wanBarrierFlag.value = 1;
	document.form.submit();
	return;
}

function doServiceChange(){
	document.form.wanVCFlag.value = 1;
	document.form.service_num_flag.value = 1;
	document.form.submit();
	return;
}

function doVCChange() {
	document.form.DvInfo_PVC.value = "0";
	document.form.wanVCFlag.value = 1;
	document.form.submit();
	return;
}

function doQos() {
	with(document.form) {
		switch(Alwan_QoS.selectedIndex) {
			case 0:
			case 1:
				inputCtrl(wan_PCR, 1);
				inputCtrl(wan_SCR, 0);
				inputCtrl(wan_MBS, 0);
				break;
			case 2:
			case 3:
				inputCtrl(wan_PCR, 1);
				inputCtrl(wan_SCR, 1);
				inputCtrl(wan_MBS, 1);
				break;
		}
	}
	return;
}

function doIPVersionChange() {
	with (document.form){
		if(ipVerRadio[0].checked){//ipv4
			if(wanTypeOption.selectedIndex == 3) {	//bridge mode
				showhide("IPv4Setting", 0);
				showhide("IPv6Setting", 0);
			}
			else {
				if(isForInternet()){
					showhide("IPv4Setting", 1);
				}
				else{	
					if(wanTypeOption.selectedIndex == 0){
						showhide("IPv4Setting", 0);
					}
					else if(wanTypeOption.selectedIndex == 1 || wanTypeOption.selectedIndex == 2){
						showhide("IPv4Setting", 1);
					}
				}
				showhide("IPv6Setting", 0);
				<% If tcWebApi_get("Info_Ether","isDSLITESupported","h") = "Yes" Then %>
				showhide("DSLITEEnable", 0);
				showhide("DSLITEMode", 0);
				showhide("DSLITEAddr", 0);
				<% end if %>
			}
		}
		else if(ipVerRadio[2].checked == true){//ipv6
			if(wanTypeOption.selectedIndex == 3) {	//bridge mode
				showhide("IPv4Setting", 0);
				showhide("IPv6Setting", 0);
			}
			else {
				showhide("IPv4Setting", 0);
				showhide("IPv6Setting", 1);
				<% If tcWebApi_get("Info_Ether","isDSLITESupported","h") = "Yes" Then %>
				showhide("DSLITEEnable", 1);
				<% end if %>
			}
		}
		else{//ipv4/ipv6
			if(wanTypeOption.selectedIndex == 3) {	//bridge mode
				showhide("IPv4Setting", 0);
				showhide("IPv6Setting", 0);
			}
			else {
				
				if(isForInternet()){
					showhide("IPv4Setting", 1);
				}
				else{	
					if(wanTypeOption.selectedIndex == 0){
						showhide("IPv4Setting", 0);
					}
					else if(wanTypeOption.selectedIndex == 1 || wanTypeOption.selectedIndex == 2){
						showhide("IPv4Setting", 1);
					}
				}
				showhide("IPv6Setting", 1);				
				<% If tcWebApi_get("Info_Ether","isDSLITESupported","h") = "Yes" Then %>
				showhide("DSLITEEnable", 0);
				showhide("DSLITEMode", 0);
				showhide("DSLITEAddr", 0);
				<% end if %>
			}
		}
	}
	return;
}

// "WebCurSet_Entry","wan_pvc"		/ ATM
//"WebCurSet_Entry","wan_pvc_ext"	/ PTM | Ethernet
function doConTypeChange() {
	with (document.form){	
		switch(wanTypeOption.selectedIndex) {
			case 0:	//Automatic IP
				if(isForInternet()){
					inputCtrl(document.form.UPnP_active[0], 1);
					inputCtrl(document.form.UPnP_active[1], 1);
					document.form.UPnP_active_flag.value = 1;
				}
				else{
					inputCtrl(document.form.UPnP_active[0], 0);
					inputCtrl(document.form.UPnP_active[1], 0);
				}
				if (is8021xsupport.value == "1") {
					showhide("div_802_1x", 1);
				}
				else {
					showhide("div_802_1x", 0);
				}

				//ip common setting
				showhide("IPCommonSetting", 1);
				<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
				showhide("wan_Encap0", 1);
				showhide("wan_Encap1", 0);
				showhide("wan_Encap2", 0);
				showhide("wan_Encap3", 0);
				<%end if%>
				//showhide("wan_BridgeInterface", 1);				
				showhide("wan_TCPMTU", 1);

				//IPTV
				showhide("IPTV_LANPort_Option", 0);				
				
				showhide("div_isipv6sup", 1);
				
				if(!isForInternet()){	//allow multi-service to change IP version
					document.form.ipVerRadio[0].disabled = false;
					document.form.ipVerRadio[1].disabled = false;
					document.form.ipVerRadio[2].disabled = false;
				}
					
				//ipv4 setting					
				showhide("IPv4Setting", 1);					
				showhide("dnsTypeRadio", 1);
				dnsCheck();
				showhide("wan_NAT", 1);					
				showhide("wan_PPPGetIP", 0);
				showhide("wan_StaticIPaddr", 0);
				showhide("wan_StaticIPSubMask", 0);
				showhide("wan_StaticIpGateway", 0);
				document.form.IPv4_setting_flag.value = 1;
					
				//ipv6 setting
				//showhide("IPv6FetchType", 1);
				showhide("IPv6Setting", 1);		
				showhide("DynIPv6Enable", 1);
				showhide("PPPIPv6Mode", 0);
				showhide("PPPIPv6PD", 1);
				showhide("wan_IPv6Addr", 0);
				showhide("wan_IPv6DefGw", 0);
				showhide("wan_IPv6DNS1", 0);
				showhide("wan_IPv6DNS2", 0);
				showhide("wan_MLD", 1);													

				//ppp setting
				showhide("wan_PPPSetting", 0);

				//DHCP option
				showhide("wan_DHCP_opt", 1);
				showDisableDHCPclientID(document.form.tmp_dhcp_clientid_type);

				// ISP Requirement
				showhide("wan_ISPRequirement", 1);
				showhide("wan_hostname", 1);
				<%if tcWebApi_get("Wan_Common","sharepvc","h") <> "1" then%>
				showhide("wan_hwaddr_x", 1);
				<%else%>
				showhide("wan_hwaddr_x", 0);
				<%end if%>

				break;
			case 1:	//static
				if(isForInternet()){
					inputCtrl(document.form.UPnP_active[0], 1);
					inputCtrl(document.form.UPnP_active[1], 1);
					document.form.UPnP_active_flag.value = 1;
				}
				else{
					inputCtrl(document.form.UPnP_active[0], 0);
					inputCtrl(document.form.UPnP_active[1], 0);
				}
				if (is8021xsupport.value == "1") {
					showhide("div_802_1x", 1);
				}
				else {
					showhide("div_802_1x", 0);
				}

				//ip common setting
				showhide("IPCommonSetting", 1);
				<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
				showhide("wan_Encap0", 0);
				showhide("wan_Encap1", 1);
				showhide("wan_Encap2", 0);
				showhide("wan_Encap3", 0);
				<%end if%>
				//showhide("wan_BridgeInterface", 1);
				showhide("wan_TCPMTU", 1);

				//IPTV
				showhide("IPTV_LANPort_Option", 0);

				showhide("div_isipv6sup", 1);
				
				if(!isForInternet()){	//allow multi-service to change IP version
					document.form.ipVerRadio[0].disabled = false;
					document.form.ipVerRadio[1].disabled = false;
					document.form.ipVerRadio[2].disabled = false;					
				}
				
				//ipv4 setting
				showhide("wan_PPPGetIP", 0);					
				showhide("wan_StaticIPaddr", 1);
				showhide("wan_StaticIPSubMask", 1);
				showhide("wan_StaticIpGateway", 1);
				inputCtrl(wan_StaticIPaddr, 1);
				inputCtrl(wan_StaticIPSubMask, 1);
				inputCtrl(wan_StaticIpGateway, 1);
				
				showhide("dnsTypeRadio", 0);
				if(isForInternet()){
					dnsTypeRadio[1].checked = true;
					showhide("PrimaryDns", 1);
					showhide("SecondDns", 1);
					showhide("wan_NAT", 1);
				}
				else{
					showhide("PrimaryDns", 0);
					showhide("SecondDns", 0);
					showhide("wan_NAT", 0);
				}				
				document.form.IPv4_setting_flag.value = 1;												

				//ipv6 setting
				//showhide("IPv6FetchType", 1);
				showhide("IPv6Setting", 1);
				showhide("DynIPv6Enable", 0);
				showhide("PPPIPv6Mode", 0);
				showhide("PPPIPv6PD", 0);
				showhide("wan_IPv6Addr", 1);
				showhide("wan_IPv6DefGw", 1);
				showhide("wan_IPv6DNS1", 1);
				showhide("wan_IPv6DNS2", 1);
				showhide("wan_MLD", 1);				

				//ppp setting
				showhide("wan_PPPSetting", 0);

				//DHCP option
				showhide("wan_DHCP_opt", 0);

				// ISP Requirement
				<%if tcWebApi_get("Wan_Common","sharepvc","h") <> "1" then%>
				showhide("wan_ISPRequirement", 1);
				showhide("wan_hostname", 0);
				showhide("wan_hwaddr_x", 1);
				<%else%>
				showhide("wan_ISPRequirement", 0);
				<%end if%>

				break;
			case 2:	//pppoa, pppoe
				if(isForInternet()){
					inputCtrl(document.form.UPnP_active[0], 1);
					inputCtrl(document.form.UPnP_active[1], 1);
					document.form.UPnP_active_flag.value = 1;
				}
				else{
					inputCtrl(document.form.UPnP_active[0], 0);
					inputCtrl(document.form.UPnP_active[1], 0);
				}	
				showhide("div_802_1x", 0);

				//ip common setting
				showhide("IPCommonSetting", 1);
				<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
				showhide("wan_Encap0", 0);
				showhide("wan_Encap1", 0);
				showhide("wan_Encap2", 1);
				showhide("wan_Encap3", 0);
				<%end if%>
				//showhide("wan_BridgeInterface", 0);
				showhide("wan_TCPMTU", 1);

				//IPTV
				showhide("IPTV_LANPort_Option", 0);
				
				showhide("div_isipv6sup", 1);
				
				if(!isForInternet()){	//allow multi-service to change IP version
					document.form.ipVerRadio[0].disabled = false;
					document.form.ipVerRadio[1].disabled = false;
					document.form.ipVerRadio[2].disabled = false;
				}
				
				//ipv4 setting		
				showhide("wan_PPPGetIP", 1);
				showhide("wan_StaticIPaddr", 1);
				showhide("wan_StaticIPSubMask", 1);
				showhide("wan_StaticIpGateway", 1);
				
				if(isForInternet()){
					showhide("dnsTypeRadio", 1);
				<%if tcWebApi_get("Wan_PVC","DNS_type","h") = "0" then%>
					dnsTypeRadio[0].checked = true;
				<%end if%>
					dnsCheck();					
					showhide("wan_NAT", 1);	
				}
				else{
					showhide("dnsTypeRadio", 0);
					showhide("PrimaryDns", 0);
					showhide("SecondDns", 0);
					showhide("wan_NAT", 0);
				}				
				document.form.IPv4_setting_flag.value = 1;

				//ipv6 setting
				//showhide("IPv6FetchType", 0);
				showhide("IPv6Setting", 1);
				showhide("DynIPv6Enable", 0);
				showhide("PPPIPv6Mode", 1);
				showhide("PPPIPv6PD", 1);
				showhide("wan_IPv6Addr", 0);
				showhide("wan_IPv6DefGw", 0);
				showhide("wan_IPv6DNS1", 0);
				showhide("wan_IPv6DNS2", 0);
				showhide("wan_MLD", 1);					

				//ppp setting
				showhide("wan_PPPSetting", 1);
				document.form.InetDetect.value = (original_ppp_echo == 0 || original_ppp_echo == 1)?original_ppp_echo:1;
				document.form.lcpEchoInterval.value = (original_lcpEchoInterval >= 0)?original_lcpEchoInterval:6;
				document.form.lcpEchoFailure.value = (original_lcpEchoFailure >= 0)?original_lcpEchoFailure:10;
				ppp_echo_control();

				//DHCP option
				showhide("wan_DHCP_opt", 0);

				// ISP Requirement
				<%if tcWebApi_get("Wan_Common","sharepvc","h") <> "1" then%>
				showhide("wan_ISPRequirement", 1);
				showhide("wan_hostname", 0);
				showhide("wan_hwaddr_x", 1);
				<%else%>
				showhide("wan_ISPRequirement", 0);
				<%end if%>

				pppStaticCheck();

				break;
			case 3:	//bridge
				inputCtrl(document.form.UPnP_active[0], 0);
				inputCtrl(document.form.UPnP_active[1], 0);
				showhide("div_802_1x", 0);

				//ip common setting
				<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
				showhide("wan_Encap0", 0);
				showhide("wan_Encap1", 0);
				showhide("wan_Encap2", 0);
				showhide("wan_Encap3", 1);
				<%end if%>
				//showhide("wan_BridgeInterface", 0);
				showhide("wan_TCPMTU", 0);

				<%if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then%>
				showhide("IPCommonSetting", 0);
				<%elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then%>
				showhide("IPCommonSetting", 0);
				<%end if%>
				
				//IPTV
				showhide("IPTV_LANPort_Option", 1);
				rm_IPTV_LANPort();
				
				showhide("div_isipv6sup", 0);
				showhide("IPv4Setting", 0);
				showhide("IPv6Setting", 0);
				showhide("wan_PPPSetting", 0);

				//DHCP option
				showhide("wan_DHCP_opt", 0);

				// ISP Requirement
				showhide("wan_ISPRequirement", 0);

				break;
			default:
				wanTypeOption.selectedIndex = "<% tcWebApi_get("Wan_PVC","ISP","s") %>";
				doConTypeChange();
				return;
		}
		doIPVersionChange();
		<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
		doEncapChange();
		<%end if%>
	}
	return;
}

function rm_IPTV_LANPort(){	//remove wans_lanport if dual wan with Ethernet LAN
	
	if(productid == "DSL-AC51" || productid == "DSL-N16P"){	//-- MODELDEP:DSL-AC51 (odmpid:DSL-AC750)/DSL-N16P LAN port 1,2 only
		document.getElementById("IPTV_LANPort_Option").options.remove(3);
		document.getElementById("IPTV_LANPort_Option").options.remove(2);
	}

	if(wans_dualwan_orig.search("lan") >= 0 && wans_lanport_orig > 0){
		var wans_lanport_idx = wans_lanport_orig-1;
		document.getElementById("IPTV_LANPort_Option").options.remove(wans_lanport_idx);
	}
}

function doEncapChange() {
	with (document.form){
		if( ( wanTypeOption.value == 0 && (wan_Encap0.value == "1483 Bridged IP LLC" || wan_Encap0.value == "1483 Bridged IP VC-Mux") ) ||
			( wanTypeOption.value == 1 && (wan_Encap1.value == "1483 Bridged IP LLC" || wan_Encap1.value == "1483 Bridged IP VC-Mux") ) ||
			( wanTypeOption.value == 2 && (wan_Encap2.value == "PPPoE LLC" || wan_Encap2.value == "PPPoE VC-Mux") ) ||
			( wanTypeOption.value == 3 && (wan_Encap3.value == "1483 Bridged Only LLC" || wan_Encap3.value == "1483 Bridged Only VC-Mux") ) ){
			//showhide("wan_ISPRequirement", 1);
			showhide("div_8021q", 1);
			document.form.wan_8021q.value = 1;
			document.form.disp_wan_8021q.value = 1;
		}
		else{
			//showhide("wan_ISPRequirement", 0);
			showhide("div_8021q", 0);
		}
	}
}

function doDefaultRouteYes() {
	with(document.form) {
		if(isDSLITESupported.value == 1){
			if(DSLITEEnableRadio[0].checked){
				DSLITEModeRadio[0].disabled = false;
				DSLITEModeRadio[1].disabled = false;
				DSLITEAddr.disabled = false;
			}
			DSLITEEnableRadio[0].disabled = false;
			DSLITEEnableRadio[1].disabled = false;
		}
	}
}

function doDefaultRouteNo() {
	with(document.form) {
		if(isDSLITESupported.value == 1){
			if(DSLITEEnableRadio[0].checked){
				DSLITEModeRadio[0].disabled = true;
				DSLITEModeRadio[1].disabled = true;
				DSLITEAddr.disabled = true;
			}
			DSLITEEnableRadio[0].disabled = true;
			DSLITEEnableRadio[1].disabled = true;
		}
	}
}


function doPPPv6ModeSLAAC()
{
	var form=document.form;
	form.PPPDHCPv6Mode_Flag.value = 0;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function doPPPv6ModeDHCP()
{
	var form=document.form;
	form.PPPDHCPv6Mode_Flag.value = 1;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function doPrivacyaddrsShow()
{
	if(document.form.DynIPv6EnableRadio[0].checked)
		showhide("div_privacyaddrs", 0);
	else
		showhide("div_privacyaddrs", 1);
}

function ripngEnableChanged()
{
	if(document.Alpha_WAN.ripngEnableRadio[0].checked)
		showhide('div_ripng_direction', 1);
	else
		showhide('div_ripng_direction', 0);
}

function doDSLiteEnable(index)
{
	showhide("DSLITEMode", 1);
	showhide("DSLITEAddr", 1);
}

function doDSLiteDisable(index)
{
	showhide("DSLITEMode", 0);
	showhide("DSLITEAddr", 0);
}

function doDSLiteModeAuto()
{
	inputCtrl(document.form.DSLITEAddr, 0);
}


function doDSLiteModeManual()
{
	inputCtrl(document.form.DSLITEAddr, 1);
}

function WANChkIdleTimeT() {
    var form=document.form;
    if (form.wan_ConnectSelect[1].checked)
        form.wan_IdleTimeT.disabled = false;
    else
    	form.wan_IdleTimeT.disabled = true;
}

function idleTimeCheck() {
	if (document.form.wan_ConnectSelect[1].checked) {
		if(!validate_number_range(document.form.wan_IdleTimeT, 0, 999))
			return true;
	}
	return false;
}

function pppStaticCheck() {
	with(document.form) {
		if (wan_PPPGetIP[0].checked) {	//dynamic
			wan_StaticIPaddr.value = "0.0.0.0";
			wan_StaticIPSubMask.value = "0.0.0.0";
			wan_StaticIpGateway.value = "0.0.0.0";
			inputCtrl(wan_StaticIPaddr, 0);
			inputCtrl(wan_StaticIPSubMask, 0);
			inputCtrl(wan_StaticIpGateway, 0);
		} else {	//static
			inputCtrl(wan_StaticIPaddr, 1);
			inputCtrl(wan_StaticIPSubMask, 1);
			inputCtrl(wan_StaticIpGateway, 1);
		}
	}
}

function dnsCheck() {
	with(document.form) {
		if (dnsTypeRadio[0].checked) {	//auto
			inputHideCtrl(PrimaryDns, 0);
			inputHideCtrl(SecondDns, 0);
		}
		else {	//static
			inputHideCtrl(PrimaryDns, 1);
			inputHideCtrl(SecondDns, 1);
		}
	}
}


function showMAC(){
	var tempMAC = "";
	document.form.wan_hwaddr_x.value = login_mac_str();
	document.form.wan_hwaddr_x.focus();
}

function check_macaddr(obj,flag){ //control hint of input mac address
	if(flag == 1){
		var childsel=document.createElement("div");
		childsel.setAttribute("id","check_mac");
		childsel.style.color="#FFCC00";
		obj.parentNode.appendChild(childsel);
		document.getElementById("check_mac").innerHTML="<% tcWebApi_get("String_Entry","LHC_MnlDHCPMacaddr_id","s") %>";
		return false;
	}else if(flag ==2){
		var childsel=document.createElement("div");
		childsel.setAttribute("id","check_mac");
		childsel.style.color="#FFCC00";
		obj.parentNode.appendChild(childsel);
		document.getElementById("check_mac").innerHTML="<% tcWebApi_get("String_Entry","IPC_x_illegal_mac","s") %>";
		return false;
	}else{
		document.getElementById("check_mac") ? document.getElementById("check_mac").style.display="none" : true;
		return true;
	}
}

/* password item show or not */
function pass_checked(obj){
	switchType(obj, document.form.show_pass_1.checked, true);       
}

function ppp_echo_control(flag){
	if (typeof(flag) == 'undefined')
		flag = document.form.InetDetect.value;
	var enable = (flag == 1) ? 1 : 0;
	inputCtrl(document.form.lcpEchoInterval, enable);
	inputCtrl(document.form.lcpEchoFailure, enable);	
}

function showDisableDHCPclientID(clientid_enable){
	if(clientid_enable.checked) {
		document.form.dhcp_clientid_type.value = "1";
		document.form.dhcp_clientid.value = "";
		document.form.dhcp_clientid.disabled = true;
	}
	else {
		document.form.dhcp_clientid_type.value = "0";
		document.form.dhcp_clientid.disabled = false;
	}
}

</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="Advanced_DSL_Content.asp">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_DSL_Content.asp">
<input type="hidden" name="next_page" value="Advanced_DSL_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_dslwan_if 0">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="lan_ipaddr" value="<%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%>" />
<input type="hidden" name="lan_netmask" value="<%If tcWebApi_get("Lan_Entry","netmask","h") <> "" then tcWebApi_get("Lan_Entry","netmask","s") end if%>" />

<INPUT TYPE="HIDDEN" NAME="String_Value_No" VALUE="No">
<INPUT TYPE="HIDDEN" NAME="wanSaveFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wanTransFlag" VALUE="0"/>
<INPUT TYPE="HIDDEN" NAME="UPnP_active_flag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="IPv4_setting_flag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="wanBarrierFlag" VALUE="0"/>
<INPUT TYPE="HIDDEN" NAME="ptm_VC" VALUE="8"/>
<INPUT TYPE="HIDDEN" NAME="wanVCFlag" VALUE="0"/>
<INPUT TYPE="HIDDEN" NAME="service_num_flag" VALUE="0"/>
<INPUT type="HIDDEN" NAME="isDSLITESupported" value="<% if tcWebApi_get("Info_Ether","isDSLITESupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT type="HIDDEN" NAME="is8021xsupport" value="<% if tcWebApi_get("Info_Ether","is8021xsupport","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT type="HIDDEN" NAME="isIPv6Supported" value="<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT TYPE="HIDDEN" NAME="isPPPAuthen" VALUE="<%tcWebApi_get("WebCustom_Entry","isPPPAuthen","s")%>">
<INPUT TYPE="HIDDEN" NAME="PPPDHCPv6Mode_Flag" VALUE="<% tcWebApi_get("Wan_PVC","PPPv6Mode","s") %>" >
<INPUT type="HIDDEN" NAME="wan_8021q" value="<% if tcWebApi_get("Info_dot1q","wan_8021q","h") = "1" then asp_write("1") else asp_write("0") end if %>" >
<INPUT type="HIDDEN" NAME="disp_wan_8021q" value="<% if tcWebApi_get("Info_dot1q","disp_wan_8021q","h") = "1" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_Active" VALUE="<% tcWebApi_get("DefaultWan_Entry","Active","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_VPI" VALUE="<% tcWebApi_get("DefaultWan_Entry","VPI","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_VCI" VALUE="<% tcWebApi_get("DefaultWan_Entry","VCI","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_QOS" VALUE="<% tcWebApi_get("DefaultWan_Entry","QOS","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_PCR" VALUE="<% tcWebApi_get("DefaultWan_Entry","PCR","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_SCR" VALUE="<% tcWebApi_get("DefaultWan_Entry","SCR","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_MBS" VALUE="<% tcWebApi_get("DefaultWan_Entry","MBS","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_ISP" VALUE="<% tcWebApi_get("DefaultWan_Entry","ISP","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_ENCAP" VALUE="<% tcWebApi_get("DefaultWan_Entry","ENCAP","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_IPVERSION" VALUE="<% tcWebApi_get("DefaultWan_Entry","IPVERSION","s") %>" >
<INPUT TYPE="HIDDEN" NAME="DefaultWan_MLDproxy" VALUE="<% tcWebApi_get("DefaultWan_Entry","MLDproxy","s") %>" >
<INPUT TYPE="hidden" NAME="ipv6SupportValue" VALUE="<% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv6" then asp_write("1") else asp_write("0") end if%>">
<INPUT TYPE="HIDDEN" NAME="wan_certificate" value="<% tcWebApi_get("Wan_PVC","CERTIFICATE","s") %>">
<INPUT TYPE="HIDDEN" NAME="wan_CA" value="<% tcWebApi_get("Wan_PVC","TRUSTEDCA","s") %>">
<INPUT TYPE="HIDDEN" NAME="wan_HiddenBiDirectionalAuth" value="<%tcWebApi_get("Wan_PVC","BIDIRECTIONALAUTHENTICATION","s")%>" >
<INPUT TYPE="HIDDEN" NAME="IPv6PrivacyAddrsSupportedFlag" value="<%tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","s")%>" >
<input type="hidden" name="DvInfo_PVC" value="">
<input type="hidden" name="dhcp_clientid_type" value="<% tcWebApi_get("Wan_PVC","dhcp_clientid_type","s") %>">
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<input type="hidden" name="rmvlan" value='<% tcWebApi_get("IPTV_Entry","rmvlan","s"); %>'>
<%end if%>

<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
	<td width="17">&nbsp;</td>
	<td valign="top" width="202">
		<div id="mainMenu"></div>
		<div id="subMenu"></div>
	</td>
	<td height="430" valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
		<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
		<tr>
			<td align="left" valign="top">
				<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
				<tbody>
				<tr height="10px">
					<td bgcolor="#4D595D" valign="top">
					<div>&nbsp;</div>
					<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_3","s")%> - <%tcWebApi_get("String_Entry","menu5_3_1","s")%></div>
					<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
					<div class="formfontdesc"><%tcWebApi_get("String_Entry","L3F_x_ConnectionType_sd","s")%></div>
					</td>
				</tr>
				<tr height="10px" id="SummaryTable">
					<td bgcolor="#4D595D" valign="top">
						<table id="ATMPVCTable" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="8"><% tcWebApi_Get("String_Entry", "PVC_summary", "s") %></td>
						</tr>
						</thead>
						<tr>
							<th style="width: 15%;"><center><% tcWebApi_Get("String_Entry", "PVC_unit", "s") %></center></th>
							<th style="width: 10%;"><center>VPI</center></th>
							<th style="width: 10%;"><center>VCI</center></th>
							<th style="width: 15%;"><center><% tcWebApi_Get("String_Entry", "IPC_VServerProto_in", "s") %></center></th>
							<th style="width: 20%;"><center><%tcWebApi_get("String_Entry","prtcl_JS_encmode","s")%></center></th>
							<th style="width: 10%;"><center>QoS</center></th>
							<th style="width: 10%;"><center><% tcWebApi_Get("String_Entry", "Internet", "s") %></center></th>
							<th style="width: 10%;"><center>IPTV</center></th>
						</tr>
						</table>
					<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
						<table id="MultiServiceTable"width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" id="ATMPVCTable">
						<thead>
						<tr>
							<td colspan="8"><%tcWebApi_get("String_Entry","DSL_multiserv_summary","s")%></td>
						</tr>
						</thead>
						<tr>
							<th style="width: 20%;"><center><%tcWebApi_get("String_Entry","service_unit","s")%></center></th>
							<th style="width: 15%;"><center>802.1Q Enable</center></th>
							<th style="width: 15%;"><center>VLAN ID</center></th>
							<th style="width: 20%;"><center><% tcWebApi_Get("String_Entry", "IPC_VServerProto_in", "s") %></center></th>
							<th style="width: 15%;"><center><% tcWebApi_Get("String_Entry", "Internet", "s") %></center></th>
							<th style="width: 15%;"><center>IPTV</center></th>
						</tr>
						</table>
					<%end if%>
					</td>
				</tr>
				<tr height="10px">
					<td bgcolor="#4D595D" valign="top">
						<table  width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","WAN_Transfer_Mode","s")%></td>
						</tr>
						</thead>
						<tr>
							<th><%tcWebApi_get("String_Entry","Transfer_Mode","s")%></th>
							<td align="left">
								<select class="input_option" name="wan_TransMode" onchange="doTransChange();">
								<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
									<option value="ATM" <%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then asp_write("selected") end if%>>ADSL WAN (ATM)</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
									<option value="PTM" <%if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then asp_write("selected") end if%>>VDSL WAN (PTM)</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
									<option value="Ethernet" <% if tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then asp_Write("selected") end if %>>Ethernet WAN</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","haveUSBModem","h") = "Yes" then%>
									<option value="USB">USB Modem</option>
								<%end if%>
								<%if tcWebApi_get("WebCustom_Entry","haveLanWan","h") = "Yes" then%>
									<option value="LAN" <% if tcWebApi_get("Wan_Common","TransMode","h") = "LAN" then asp_Write("selected") end if %>>Ethernet WAN</option>
								<%end if%>
								</select>
							<%if tcWebApi_get("Wan_Common","TransMode","h") = "LAN" then%>
								<select class="input_option" name="wans_lanport">
									<option value="1" <% if tcWebApi_get("Dualwan_Entry","wans_lanport","h") = "1" then asp_Write("selected") end if %>>LAN Port 1</option>
									<option value="2" <% if tcWebApi_get("Dualwan_Entry","wans_lanport","h") = "2" then asp_Write("selected") end if %>>LAN Port 2</option>
									<option value="3" <% if tcWebApi_get("Dualwan_Entry","wans_lanport","h") = "3" then asp_Write("selected") end if %>>LAN Port 3</option>
									<option value="4" <% if tcWebApi_get("Dualwan_Entry","wans_lanport","h") = "4" then asp_Write("selected") end if %>>LAN Port 4</option>
								</select>
							<%end if%>
							</td>
						</tr>
					<%if tcWebApi_get("Wan_Common","TransMode","h") <> "ATM" then%>
					<%if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then%>
						<tr id="ptm_barrier">
							<th><%tcWebApi_get("String_Entry","WAN_barrier","s")%></th>
							<td align="left">
								<select class="input_option" name="ptm_Barrier" onchange="doBarrierChange();">
									<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "8" then asp_Write("selected") end if %>>0</option>
									<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "9" then asp_Write("selected") end if %>>1</option>
								</select>
							</td>
						</tr>
					<% end if %> <!--PTM-->
					<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
					<%if tcWebApi_get("Wan_Common","TransMode","h") <> "LAN" then%>
						<tr>
							<th><%tcWebApi_get("String_Entry","service_unit","s")%></th>
							<td align="left">
								<select class="input_option" name="service_num" onchange="doServiceChange();">
									<option value= "0" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "0" then asp_Write("selected") end if %>>Service 1(Default Route)</option>
									<option value= "1" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "1" then asp_Write("selected") end if %>>Service 2</option>
									<option value= "2" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "2" then asp_Write("selected") end if %>>Service 3</option>
									<option value= "3" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "3" then asp_Write("selected") end if %>>Service 4</option>
									<option value= "4" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "4" then asp_Write("selected") end if %>>Service 5</option>
									<option value= "5" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "5" then asp_Write("selected") end if %>>Service 6</option>
									<option value= "6" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "6" then asp_Write("selected") end if %>>Service 7</option>
									<option value= "7" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "7" then asp_Write("selected") end if %>>Service 8</option>
								</select>
							</td>
						</tr>
					<% end if %> 
					<% end if %> <!--isMultiSerSupported-->
						<tr>
							<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>?</th>
							<td align="left">
								<input type="radio" name="wan_TransStatus" class="input" value="Yes" <%	if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="radio" name="wan_TransStatus" class="input" value="No" <% if tcWebApi_get("Wan_PVC","Active","h") <> "Yes" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
					<% end if %><!--none atm-->
					<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
						<tr>
							<th>PVC unit</th>
							<td align="left">
								<select id="wan_VC" class="input_option" name="wan_VC" onchange="doVCChange()">
									<OPTION value="0" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "0" then asp_Write("selected")
										elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "8" then asp_Write("selected")
										elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "9" then asp_Write("selected")
										elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "10" then asp_Write("selected")
										end if %>>PVC 1(Default Route)</option>
									<OPTION value="1" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "1" then asp_Write("selected") end if %>>PVC 2</option>
									<OPTION value="2" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "2" then asp_Write("selected") end if %>>PVC 3</option>
									<OPTION value="3" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "3" then asp_Write("selected") end if %>>PVC 4</option>
									<OPTION value="4" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "4" then asp_Write("selected") end if %>>PVC 5</option>
									<OPTION value="5" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "5" then asp_Write("selected") end if %>>PVC 6</option>
									<OPTION value="6" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "6" then asp_Write("selected") end if %>>PVC 7</option>
									<OPTION value="7" <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "7" then asp_Write("selected") end if %>>PVC 8</option>
								</select>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>?</th>
							<td align="left">
								<INPUT TYPE="RADIO" NAME="wan_VCStatus" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<INPUT TYPE="RADIO" NAME="wan_VCStatus" VALUE="No" <% if tcWebApi_get("Wan_PVC","Active","h") <> "Yes" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
					<% end if %><!--atm-->
						</table>
					</td>
				</tr>
			<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then %>

				<tr id="atmSettings" height="10px">
					<td bgcolor="#4D595D" valign="top">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
							<tr>
								<td colspan="2">PVC Settings</td>
							</tr>
						</thead>
						<tr>
							<th>VPI</th>
							<td><INPUT TYPE="TEXT" NAME="Alwan_VPI" MAXLENGTH="3" class="input_12_table" VALUE="<%if tcWebApi_get("Wan_PVC","VPI","h") <> "" then tcWebApi_get("Wan_PVC","VPI","s") else asp_Write("0") end if%>" ></td>
						</tr>
						<tr>
							<th>VCI</th>
							<td><INPUT TYPE="TEXT" NAME="Alwan_VCI" MAXLENGTH="5" class="input_12_table" VALUE="<%if tcWebApi_get("Wan_PVC","VCI","h") <> "" then tcWebApi_get("Wan_PVC","VCI","s") else asp_Write("0") end if%>" ></td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","Wan_Service_Category","s")%></th>
							<td align="left">
								<select class="input_option" name="Alwan_QoS" onchange="doQos(this.selectedIndex)">
									<option value="ubr" <% if tcWebApi_get("Wan_PVC","QOS","h") = "ubr" then asp_Write("selected") end if %>>UBR
									<option value="cbr" <% if tcWebApi_get("Wan_PVC","QOS","h") = "cbr" then asp_Write("selected") end if %>>CBR
									<option value="rt-vbr" <% if tcWebApi_get("Wan_PVC","QOS","h") = "rt-vbr" then asp_Write("selected") end if %>>RT-VBR
									<option value="nrt-vbr" <% if tcWebApi_get("Wan_PVC","QOS","h") = "nrt-vbr" then asp_Write("selected") end if %>>NRT-VBR
								</select>
							</td>
						</tr>
						<tr>
							<th>PCR</th>
							<td><INPUT TYPE="TEXT" NAME="wan_PCR" SIZE="5" MAXLENGTH="7" class="input_12_table" VALUE="<%if tcWebApi_get("Wan_PVC","PCR","h") <> "" then tcWebApi_get("Wan_PVC","PCR","s") else asp_Write("0") end if%>" ></td>
						</tr>
						<tr>
							<th>SCR</th>
							<td><INPUT TYPE="TEXT" NAME="wan_SCR" SIZE="5" MAXLENGTH="7" class="input_12_table" VALUE="<%if tcWebApi_get("Wan_PVC","SCR","h") <> "" then tcWebApi_get("Wan_PVC","SCR","s") else asp_Write("0") end if%>" ></td>
						</tr>
						<tr>
							<th>MBS</th>
							<td><INPUT TYPE="TEXT" NAME="wan_MBS" SIZE="5" MAXLENGTH="7" class="input_12_table" VALUE="<%if tcWebApi_get("Wan_PVC","MBS","h") <> "" then tcWebApi_get("Wan_PVC","MBS","s") else asp_Write("0") end if%>" ></td>
						</tr>
						</table>
					</td>
				</tr>
			<%end if%> <!-- TransMode=="ATM" end -->
				<tr id="t2BC">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
						</tr>
						</thead>
						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,35);"><%tcWebApi_get("String_Entry","L3F_x_ConnectionType_in","s")%></a></th>
							<td align="left">
								<select class="input_option" name="wanTypeOption" onchange="doConTypeChange();">
									<option id="wanTypeOption0" value="0"<% if tcWebApi_get("Wan_PVC","ISP","h") = "0" then asp_Write("selected") end if %>>Automatic IP</option>
										<!--isp0-->
									<option id="wanTypeOption1" value="1"<% if tcWebApi_get("Wan_PVC","ISP","h") = "1" then asp_Write("selected") end if %>>Static IP</option>
										<!--isp1-->
									<option id="wanTypeOption2" value="2"<% if tcWebApi_get("Wan_PVC","ISP","h") = "2" then asp_Write("selected") end if %>><%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%><%if tcWebApi_get("Wan_Common","sharepvc","h") <> "1" then%>PPPoA/<%end if%><%end if%>PPPoE</option>
										<!--isp2-->
									<option id="wanTypeOption3" value="3"<% if tcWebApi_get("Wan_PVC","ISP","h") = "3" then asp_Write("selected") end if %>>BRIDGE</option>
										<!--isp3-->
								</select>
								<select class="input_option" id="IPTV_LANPort_Option" name="IPTV_LANPort_Option" style="display:none;">
									<option value="1" <% if tcWebApi_get("Wan_PVC","bridge_lan_port","h") = "1" then asp_Write("selected") end if %>>LAN Port 1</option>
									<option value="2" <% if tcWebApi_get("Wan_PVC","bridge_lan_port","h") = "2" then asp_Write("selected") end if %>>LAN Port 2</option>
									<option value="3" <% if tcWebApi_get("Wan_PVC","bridge_lan_port","h") = "3" then asp_Write("selected") end if %>>LAN Port 3</option>
									<option value="4" <% if tcWebApi_get("Wan_PVC","bridge_lan_port","h") = "4" then asp_Write("selected") end if %>>LAN Port 4</option>									
								</select>	
							</td>
						</tr>
						
						<tr id="div_isipv6sup">
							<th><%tcWebApi_get("String_Entry","Wan_Ipversion","s")%></th>
							<td>
								<input type="RADIO" name="ipVerRadio" class="input" value="IPv4" onClick="doIPVersionChange()" <% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv4" then asp_Write("checked") end if %>>IPv4
								<input type="RADIO" name="ipVerRadio" class="input" value="IPv4/IPv6" onClick="doIPVersionChange()" <% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv4/IPv6" then asp_Write("checked") end if %>>IPv4 / IPv6
								<input type="RADIO" name="ipVerRadio" class="input" value="IPv6" onClick="doIPVersionChange()" <% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv6" then asp_Write("checked") end if %>>IPv6
							</td>
						</tr>

						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,23);"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> UPnP?</a></th>
							<td>
								<INPUT TYPE="RADIO" NAME="UPnP_active" class="input" VALUE="Yes" onClick="" <% If TCWebApi_get("Upnpd_Entry","Active","h") = "Yes" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<INPUT TYPE="RADIO" NAME="UPnP_active" class="input" VALUE="No" onClick="" <% If TCWebApi_get("Upnpd_Entry","Active","h") = "No" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
						</table>
					</td>
				</tr>
				<tr id="div_8021q">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2">802.1Q</td>
						</tr>
						</thead>
						<tr>
							<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> ?</th>
							<td align="left">
								<input type="RADIO" name="wan_dot1q" class="input" value="Yes" <% if tcWebApi_get("Wan_PVC","dot1q","h") = "Yes" then asp_Write("checked")end if %> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="RADIO" name="wan_dot1q" class="input" value="No"  <% if tcWebApi_get("Wan_PVC","dot1q","h") <> "Yes" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","WANVLANIDText","s")%></th>
							<td align="left">
								<input type="text" name="wan_vid" maxlength="4" class="input_6_table" onkeypress="return validator.isNumber(this,event)" value=<%if tcWebApi_get("Wan_PVC","VLANID","h") <> "" then tcWebApi_get("Wan_PVC","VLANID","s") else asp_Write("0") end if%> > ( 1 ~ 4094 )
							</td>
						</tr>
					<%if tcWebApi_get("WebCustom_Entry","isdot1pSupport","h") = "Yes" then %>
						<tr>
							<th>802.1P</th>
							<td align="left">
								<input type="text" name="wan_dot1p" maxlength="4" class="input_6_table" onkeypress="return validator.isNumber(this,event)" value=<%if tcWebApi_get("Wan_PVC","DOT1P","h") <> "" then tcWebApi_get("Wan_PVC","DOT1P","s") else asp_Write("0") end if%> > ( 0 ~ 7 )
							</td>
						</tr>
					<%end if%>
						</table>
					</td>
				</tr>
				<tr id="div_802_1x">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","WAN8021XInfoText","s")%></td>
						</tr>
						</thead>
						<tr>
							<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> ?</th>
							<td align="left">
								<input type="RADIO" name="wan_status" class="input" value="Enable" <% if tcWebApi_get("Wan_PVC","IPOE_DOT1X_STATUS","h") = "Enable" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="RADIO" name="wan_status" class="input" value="Disabled" <% if tcWebApi_get("Wan_PVC","IPOE_DOT1X_STATUS","h") <> "Enable" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","WAN8021XEAPIdenText","s")%></th>
							<td align="left">
								<input type="text" name="wan_eapIdentity" maxlength="15" class="input_15_table" value=<% if tcWebApi_get("Wan_PVC","EAPIDENTITY","h") <> "" then tcWebApi_get("Wan_PVC","EAPIDENTITY","s")  end if%> >
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","WAN8021XEAPMethText","s")%></th>
							<td align="left">
								<%tcWebApi_get("String_Entry","WAN8021XEAPTLSText","s")%>
							</td>
						</tr>
						</table>
					</td>
				</tr>
				<tr id="IPCommonSetting">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","WAN_IP_universal","s")%></td>
						</tr>
						</thead>
					<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
						<tr id="wan_Encap"><!--atm-->
							<th><%tcWebApi_get("String_Entry","prtcl_JS_encmode","s")%></th>
							<td align="left">
								<select id="wan_Encap0" class="input_option" name="wan_Encap0" onchange="doEncapChange()">
									<option value="1483 Bridged IP LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if %>>1483 Bridged IP LLC</option>
									<option value="1483 Bridged IP VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if %>>1483 Bridged IP VC-Mux</option>
									<option value="1483 Routed IP LLC(IPoA)" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP LLC(IPoA)" then asp_Write("selected") end if %>>1483 Routed IP LLC(IPoA)</option>
									<option value="1483 Routed IP VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP VC-Mux" then asp_Write("selected") end if %>>1483 Routed IP VC-Mux</option>
								</select>
								<select id="wan_Encap1" class="input_option" name="wan_Encap1" onchange="doEncapChange()">
									<option value="1483 Bridged IP LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if %>>1483 Bridged IP LLC</option>
									<option value="1483 Bridged IP VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if %>>1483 Bridged IP VC-Mux</option>
									<option value="1483 Routed IP LLC(IPoA)" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP LLC(IPoA)" then asp_Write("selected") end if %>>1483 Routed IP LLC(IPoA)</option>
									<option value="1483 Routed IP VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP VC-Mux" then asp_Write("selected") end if %>>1483 Routed IP VC-Mux</option>
								</select>
								<select id="wan_Encap2" class="input_option" name="wan_Encap2" onchange="doEncapChange()">
									<option value="PPPoE LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE LLC" then asp_Write("selected") end if %>>PPPoE LLC
									<option value="PPPoE VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE VC-Mux" then asp_Write("selected") end if %>>PPPoE VC-Mux
									<%if tcWebApi_get("Wan_Common","sharepvc","h") <> "1" then%>
									<option value="PPPoA LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA LLC" then asp_Write("selected") end if %>>PPPoA LLC
									<option value="PPPoA VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA VC-Mux" then asp_Write("selected") end if %>>PPPoA VC-Mux
									<%end if%>
								</select>
								<select id="wan_Encap3" class="input_option" name="wan_Encap3" onchange="doEncapChange()">
									<option value="1483 Bridged Only LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged Only LLC" then asp_Write("selected") end if %>>1483 Bridged Only LLC</option>
									<option value="1483 Bridged Only VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged Only VC-Mux" then asp_Write("selected") end if %>>1483 Bridged Only VC-Mux</option>
								</select>
							</td>
						</tr>
					<%end if%> <!-- TransMode=="ATM" end -->
<!--
						<tr id="wan_BridgeInterface">
							<th>Bridge <% tcWebApi_Get("String_Entry", "wan_interface", "s") %></th>
							<td align="left">
								<input type="RADIO" name="wan_BridgeInterface" class="input" value="Yes" <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") = "Yes" then asp_Write("checked")end if %> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="RADIO" name="wan_BridgeInterface" class="input" value="No"  <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") <> "Yes" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
-->						

						<tr id="wan_TCPMTU">
							<th>MTU</th>
							<td align="left">
								<input type="text" name="wan_TCPMTU" maxlength="4" class="input_6_table" value=<% if tcWebApi_get("Wan_PVC","MTU","h") = "" then asp_write("1492") elseif tcWebApi_get("Wan_PVC","MTU","h") = "0" then asp_write("1492") else tcWebApi_get("Wan_PVC","MTU","s")  end if%> onKeyPress="return validator.isNumber(this,event);"> <%tcWebApi_get("String_Entry","WAN_TCPMTU","s")%>
							</td>
						</tr>

						</table>
					</td>
				</tr>
				<tr id="IPv4Setting">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","WAN_IPv4_setting","s")%></td>
						</tr>
						</thead>
						<tr id="wan_PPPGetIP">
							<th><% tcWebApi_Get("String_Entry", "L3F_x_DHCPClient_in", "s") %>?</th>
							<td>
								<input type="RADIO" name="wan_PPPGetIP" class="input" value="Dynamic" onClick="pppStaticCheck();" <%if tcWebApi_get("Wan_PVC","PPPGETIP","h") <> "Static" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="RADIO" name="wan_PPPGetIP" class="input" value="Static" onClick="pppStaticCheck();" <%if tcWebApi_get("Wan_PVC","PPPGETIP","h") = "Static" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>

						<tr id="wan_StaticIPaddr">
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,1);"><% tcWebApi_Get("String_Entry", "IPC_ExternalIPAddress_in", "s") %></a></th>
							<td>
								<input type="text" name="wan_StaticIPaddr" maxlength="15" class="input_15_table" onKeyPress="return validator.isIPAddr(this,event);" value= <% if tcWebApi_get("Wan_PVC","IPADDR","h") <> "" then tcWebApi_get("Wan_PVC","IPADDR","s") end if%> >
							</td>
						</tr>
						<tr id="wan_StaticIPSubMask">
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,2);"><% tcWebApi_Get("String_Entry", "IPC_x_ExternalSubnetMask_in", "s") %></a></th>
							<td>
								<input type="text" name="wan_StaticIPSubMask" maxlength="15" class="input_15_table" onKeyPress="return validator.isIPAddr(this,event);" value=<% if tcWebApi_get("Wan_PVC","NETMASK","h") <> "" then tcWebApi_get("Wan_PVC","NETMASK","s") end if%> >
							</td>
						</tr>
						<tr id="wan_StaticIpGateway">
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,3);"><% tcWebApi_Get("String_Entry", "IPC_x_ExternalGateway_in", "s") %></a></th>
							<td>
								<input type="text" name="wan_StaticIpGateway" maxlength="15" class="input_15_table" onKeyPress="return validator.isIPAddr(this,event);" value= <% if tcWebApi_get("Wan_PVC","GATEWAY","h") <> "" then tcWebApi_get("Wan_PVC","GATEWAY","s") end if%>>
							</td>
						</tr>

						<tr id="dnsTypeRadio">
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,12);"><% tcWebApi_Get("String_Entry", "IPC_x_DNSServerEnable_in", "s") %>?</a></th>
							<td>
								<INPUT TYPE="RADIO" NAME="dnsTypeRadio" class="input" VALUE="0" onClick="dnsCheck()" <%If tcWebApi_get("Wan_PVC","DNS_type","h") <> "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<INPUT TYPE="RADIO" NAME="dnsTypeRadio" class="input" VALUE="1" onClick="dnsCheck()" <%If tcWebApi_get("Wan_PVC","DNS_type","h") = "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>
						<tr id="PrimaryDns">
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,13);"><% tcWebApi_Get("String_Entry", "IPC_x_DNSServer1_in", "s") %></a></th>
							<td>
								<INPUT TYPE="TEXT" NAME="PrimaryDns" class="input_15_table" MAXLENGTH="15" onkeypress="return validator.isIPAddr(this,event)" VALUE="<% If tcWebApi_get("Wan_PVC","DNS_type","h") = "1" then tcWebApi_get("Wan_PVC","Primary_DNS","s") end if %>" >
							</td>
						</tr>
						<tr id="SecondDns">
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,14);"><% tcWebApi_Get("String_Entry", "IPC_x_DNSServer2_in", "s") %></a></th>
							<td>
								<INPUT TYPE="TEXT" NAME="SecondDns" class="input_15_table" MAXLENGTH="15" onkeypress="return validator.isIPAddr(this,event)" VALUE="<% If tcWebApi_get("Wan_PVC","DNS_type","h") = "1" then tcWebApi_get("Wan_PVC","Secondary_DNS","s") end if %>" >
							</td>
						</tr>
						<tr id="wan_NAT">
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,22);"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> NAT</a></th>
							<td>
								<input type="radio" name="wan_NAT" class="input" value="Enable" <% if tcWebApi_get("Wan_Common","NATENABLE","h") <> "Disabled" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
								<input type="radio" name="wan_NAT" class="input" value="Disabled" <% if tcWebApi_get("Wan_Common","NATENABLE","h") = "Disabled" then asp_Write("checked") end if %>><%tcWebApi_get("String_Entry","checkbox_No","s")%>
							</td>
						</tr>

						<tr id="wan_DynamicRoute">
							<th>Dynamic Route</th>
							<td>
								<SELECT NAME="wan_RIP" SIZE="1" class="input_option">
									<option value="RIP1" <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP1" then asp_Write("selected") end if %>>RIP1</option>
									<option value="RIP2" <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP2" then asp_Write("selected") end if %>>RIP2</option>
								</SELECT>
								Direction
								<SELECT NAME="wan_RIP_Dir" SIZE="1" class="input_option">
									<option value="None" <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "None" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","wl_securitylevel_0","s")%></option>
									<option value="Both" <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both</option>
									<option value="IN Only" <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only</option>
									<option value="OUT Only" <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>OUT Only</option>
								</SELECT>
							</td>
						</tr>
					<!--
						<tr id="wan_IGMP">
							<th>IGMP Proxy</th>
							<td>
								<INPUT TYPE="RADIO" NAME="wan_IGMP" class="input" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") = "Yes" then asp_Write("checked")end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>
								<INPUT TYPE="RADIO" NAME="wan_IGMP" class="input" VALUE="No"  <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") <> "Yes" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%>
							</td>
						</tr>
					-->
						</table>
					</td>
				</tr>
				<tr id="IPv6Setting">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2">IPv6 Setting</td>
						</tr>
						</thead>

						<!--tr id="IPv6FetchType">
							<th>IPv6 Message Fetch Type</th>
							<td>Dynamic Mode</td>
							<td>Static Mode</td>
						</tr-->
						<tr id="DynIPv6Enable">
							<th>DHCP</th>
							<td>
							<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
								<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" class="input" VALUE="1" onClick="doPrivacyaddrsShow();" <% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") <> "0" then asp_Write("checked") end if %> >DHCP
								<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" class="input" VALUE="0" onClick="doPrivacyaddrsShow();" <% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "0" then asp_Write("checked") end if %> >SLAAC
							<%else %>
								<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" class="input" VALUE="1" <% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") <> "0" then asp_Write("checked") end if %> >DHCP
								<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" class="input" VALUE="0" <% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "0" then asp_Write("checked") end if %> >SLAAC
							<%end if%>
							</td>
						</tr>
						<tr id="PPPIPv6Mode">
							<th>DHCP</th>
							<td>
							<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
								<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" class="input" VALUE="1" onClick="doPPPv6ModeDHCP();doPrivacyaddrsShow();" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") <> "0" then asp_Write("checked")  end if %> > <%tcWebApi_get("String_Entry","WANIPv6Enable0Text","s")%>
								<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" class="input" VALUE="0" onClick="doPPPv6ModeSLAAC();doPrivacyaddrsShow();" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "0" then asp_Write("checked") end if %> > <%tcWebApi_get("String_Entry","WANIPv6Enable1Text","s")%>
							<%else %>
								<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" class="input" VALUE="1" onClick="doPPPv6ModeDHCP()" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") <> "0" then asp_Write("checked") end if %> > <%tcWebApi_get("String_Entry","WANIPv6Enable0Text","s")%>
								<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" class="input" VALUE="0" onClick="doPPPv6ModeSLAAC()" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "0" then asp_Write("checked") end if %> > <%tcWebApi_get("String_Entry","WANIPv6Enable1Text","s")%>
							<%end if%>
							</td>
						</tr>
						<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
						<tr id="div_privacyaddrs">
							<th>Privacy Address</th>
							<td>
								<INPUT TYPE="RADIO" NAME="privacyaddrsradio" class="input" VALUE="2" <% if tcWebApi_get("Wan_PVC","IPv6Extension","h") = "2" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>
								<INPUT TYPE="RADIO" NAME="privacyaddrsradio" class="input" VALUE="0"  <% if tcWebApi_get("Wan_PVC","IPv6Extension","h") <> "2" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%>
							</td>
							<script language="JavaScript" type="text/JavaScript">
								doPrivacyaddrsShow();
							</script>
						</tr>
						<%end if%>
						<tr id="PPPIPv6PD">
							<th>DHCP PD</th>
							<td>
								<INPUT TYPE="RADIO" NAME="PPPIPv6PDRadio" class="input" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","PPPv6PD","h") <> "No" then asp_Write("checked")  end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>
								<INPUT TYPE="RADIO" NAME="PPPIPv6PDRadio" class="input" VALUE="No" <% if tcWebApi_get("Wan_PVC","PPPv6PD","h") = "No" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%>
							</td>
						</tr>
						<tr id="wan_IPv6Addr">
							<th>IPv6 Address</th>
							<td>
								<input type="text" name="wan_IPv6Addr" MAXLENGTH="39" class="input_32_table" VALUE="<%If tcWebApi_get("Wan_PVC","IPADDR6","h") <> "" then tcWebApi_get("Wan_PVC","IPADDR6","s") end if %>">&nbsp;/&nbsp;
								<input type="text" name="wan_IPv6Prefix" MAXLENGTH="3" class="input_3_table" VALUE="<%If tcWebApi_get("Wan_PVC","PREFIX6","h") <> "" then tcWebApi_get("Wan_PVC","PREFIX6","s") end if %>">
							</td>
						</tr>
						<tr id="wan_IPv6DefGw">
							<th>IPv6 <% tcWebApi_Get("String_Entry", "IPC_x_ExternalGateway_in", "s") %></th>
							<td><input type="text" name="wan_IPv6DefGw" MAXLENGTH="39" class="input_32_table" VALUE="<%If tcWebApi_get("Wan_PVC","DEFGATEWAY6","h") <> "" then tcWebApi_get("Wan_PVC","DEFGATEWAY6","s") end if %>"></td>
						</tr>
						<tr id="wan_IPv6DNS1">
							<th>IPv6 <% tcWebApi_Get("String_Entry", "IPC_x_DNSServer1_in", "s") %></th>
							<td><INPUT TYPE="TEXT" NAME="wan_IPv6DNS1" MAXLENGTH="39" class="input_32_table" VALUE="<%If tcWebApi_get("Wan_PVC","DNSIPv61st","h") <> "" then tcWebApi_get("Wan_PVC","DNSIPv61st","s") end if %>"></td>
						</tr>
						<tr id="wan_IPv6DNS2">
							<th>IPv6 <% tcWebApi_Get("String_Entry", "IPC_x_DNSServer2_in", "s") %></th>
							<td><INPUT TYPE="TEXT" NAME="wan_IPv6DNS2" MAXLENGTH="39" class="input_32_table" VALUE="<%If tcWebApi_get("Wan_PVC","DNSIPv62nd","h") <> "" then tcWebApi_get("Wan_PVC","DNSIPv62nd","s") end if %>"></td>
						</tr>
						<tr id="wan_MLD">
							<th>MLD Proxy</th>
							<td>
								<INPUT TYPE="RADIO" NAME="wan_MLD0" class="input" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","MLDproxy","h") <> "No" then asp_Write("checked")end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>
								<INPUT TYPE="RADIO" NAME="wan_MLD0" class="input" VALUE="No"  <% if tcWebApi_get("Wan_PVC","MLDproxy","h") = "No" then asp_Write("checked") end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%>
							</td>
						</tr>

						<% If tcWebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" Then %>
						<tr id="ripngEnable">
							<th>Ripng</th>
							<td>
								<INPUT TYPE="RADIO" NAME="ripngEnableRadio" class="input" VALUE="1" onClick="ripngEnableChanged()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") = "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>
								<INPUT TYPE="RADIO" NAME="ripngEnableRadio" class="input" VALUE="0" onClick="ripngEnableChanged()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") <> "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%>
							</td>
						</tr>
						 <tr id="div_ripng_direction">
							<th>Ripng Direction</th>
							<td>
								<SELECT NAME="wan_Ripng_Dir" SIZE="1" class="input_option">
									<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "None" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","wl_securitylevel_0","s")%>
									<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
									<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
									<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>OUT Only
								</SELECT>
							</td>
						</tr>
						<script language="JavaScript" type="text/JavaScript">
								ripngEnableChanged();
						</script>
						<% end if %>

						<% If tcWebApi_get("Info_Ether","isDSLITESupported","h") = "Yes" Then %>
						<tr id="DSLITEEnable">
							<th>Dual Stack Lite</th>
							<td>
								<input type="RADIO" name="DSLITEEnableRadio" class="input" value="Yes" onClick="doDSLiteEnable()" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "Yes" then asp_Write("checked")  end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>
								<input type="RADIO" name="DSLITEEnableRadio" class="input" value="No" onClick="doDSLiteDisable()" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") <> "Yes" then asp_Write("checked")  end if %> ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%>
							</td>
						</tr>
						<tr id="DSLITEMode">
							<th>DS Lite Mode</th>
							<td>
								<INPUT TYPE="RADIO" NAME="DSLITEModeRadio" class="input" VALUE="0" onClick="doDSLiteModeAuto()" <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") <> "1" then asp_Write("checked") end if %> >Auto
								<input type="RADIO" name="DSLITEModeRadio" class="input" value="1" onClick="doDSLiteModeManual()" <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") = "1" then asp_Write("checked") end if %> >Manual
							</td>
						</tr>
						<tr id="DSLITEAddr">
							<th>Remote IPv6 Address</th>
							<td>
								<INPUT TYPE="TEXT" NAME="DSLITEAddr" SIZE="39" MAXLENGTH="39" class="input_32_table" VALUE= <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") = "1" then tcWebApi_get("Wan_PVC","DSLITEAddr","s") else asp_Write("N/A") end if%> >
							</td>
						</tr>
						<script language="JavaScript" type="text/JavaScript">
							if(document.form.DSLITEEnableRadio[0].checked)
								doDSLiteEnable();
							else
								doDSLiteDisable();
						</script>
						<% end if %>
						</table>
					</td>
				</tr>

				<tr id="wan_PPPSetting">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","WAN_ppp_setting","s")%></td>
						</tr>
						</thead>
						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,4);"><% tcWebApi_Get("String_Entry", "PPPC_UserName_in", "s") %></a></th>
							<td><input type="text" maxlength="64" class="input_32_table" name="wan_PPPUsername" onkeypress="return validator.isString(this, event)" value="" autocapitalization="off" autocomplete="off"></td>
						</tr>
						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,5);"><% tcWebApi_Get("String_Entry", "PPPC_Password_in", "s") %></a></th>
							<td>
								<input type="password" maxlength="64" class="input_32_table" name="wan_PPPPassword" value="" onBlur="" autocapitalization="off" autocomplete="off">
								<div><input type="checkbox" name="show_pass_1" onclick="pass_checked(document.form.wan_PPPPassword);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
							</td>
						</tr>
						<% if tcWebApi_get("WebCustom_Entry","isPPPAuthen","h") = "Yes" then %>
						<tr>
							<th><%tcWebApi_get("String_Entry","WANPPPAuthText","s")%></th>
							<td align="left">
								<select class="input_option" name="PPPAuthen">
									<option value="AUTO" <% if tcWebApi_get("Wan_PVC","AUTHEN","h") = "AUTO" then asp_Write("selected") elseif tcWebApi_get("Wan_PVC","AUTHEN","h") = "" then asp_Write("selected") end if %>>AUTO
									<option value="PAP" <% if tcWebApi_get("Wan_PVC","AUTHEN","h") = "PAP" then asp_Write("selected") end if %>>PAP
									<option value="CHAP" <% if tcWebApi_get("Wan_PVC","AUTHEN","h") = "CHAP" then asp_Write("selected") end if %>>CHAP
								</select>
							</td>
						</tr>
						<%end if%>

						<tr>
							<th><%tcWebApi_get("String_Entry","Connectiontype","s")%></th>
							<td align="left">
								<INPUT TYPE="RADIO" NAME="wan_ConnectSelect" class="input" VALUE="Connect_Keep_Alive" onClick="WANChkIdleTimeT();" <%if tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_Keep_Alive" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_Manually" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","CONNECTION","h") = "" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","WANConnectioncCommText","s")%>
								<br/>
								<div>
								<INPUT TYPE="RADIO" NAME="wan_ConnectSelect" class="input" VALUE="Connect_on_Demand"   <%if tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_on_Demand" then asp_Write("checked") end if%> onClick="WANChkIdleTimeT();"><%tcWebApi_get("String_Entry","WANConnectioncComm1Text","s")%>
								<INPUT TYPE="TEXT" NAME="wan_IdleTimeT" MAXLENGTH="3" class="input_6_table" onkeypress="return validator.isNumber(this,event)" VALUE=<% if tcWebApi_get("Wan_PVC","CLOSEIFIDLE","h") = "" then asp_Write("0") else tcWebApi_get("Wan_PVC","CLOSEIFIDLE","s") end if%> ><%tcWebApi_get("String_Entry","WANConnectioncComm2Text","s")%>
								<br/>
								</div>
							</td>
						</tr>
						<tr>
							<th><%tcWebApi_get("String_Entry","WANTCPMSSOptionText","s")%></th>
							<td align="left">
								<INPUT TYPE="TEXT" NAME="wan_TCPMSS" MAXLENGTH="4" class="input_6_table" VALUE=<% if tcWebApi_get("Wan_PVC","MSS","h") = "" then asp_Write("0") else tcWebApi_get("Wan_PVC","MSS","s") end if%> ><%tcWebApi_get("String_Entry","WANTCPMSSOptionComm1Text","s")%> (0 means use default)
							</td>
						</tr>
						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,17);">Host-Uniq (Hexadecimal)</a></th>
							<td align="left">
								<INPUT TYPE="TEXT" NAME="wan_pppoe_hostuniq" maxlength="32" class="input_32_table" VALUE="<% tcWebApi_get("Wan_PVC","wan_pppoe_hostuniq","s") %>" onKeyPress="return validator.isString(this,event);"/>
							</td>
						</tr>

						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,32);"><%tcWebApi_get("String_Entry","PPPC_x_InternetDetection_in","s")%></a></th>
							<td>
								<select name="InetDetect" class="input_option" onChange="ppp_echo_control();">
								<option value="0" <% if tcWebApi_get("Wan_PVC","InetDetect","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_disable","s")%></option>
								<option value="1" <% if tcWebApi_get("Wan_PVC","InetDetect","h") = "1" then asp_Write("selected") end if %>>PPP Echo</option>
								</select>
							</td>
						</tr>
						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,33);"><%tcWebApi_get("String_Entry","PPPC_x_PPPEcho_Interval","s")%></a></th>
							<td><input type="text" maxlength="6" class="input_6_table" name="lcpEchoInterval" value="<%tcWebApi_get("Wan_PVC", "lcpEchoInterval", "s")%>" onkeypress="return validator.isNumber(this, event)" autocorrect="off" autocapitalize="off"/></td>
						</tr>
						<tr>
							<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,34);"><%tcWebApi_get("String_Entry","PPPC_x_PPPEcho_Max_Failure","s")%></a></th>
							<td><input type="text" maxlength="6" class="input_6_table" name="lcpEchoFailure" value="<%tcWebApi_get("Wan_PVC", "lcpEchoFailure", "s")%>" onkeypress="return validator.isNumber(this,event);" autocorrect="off" autocapitalize="off"/></td>
						</tr>

						<tr>
							<th><%tcWebApi_get("String_Entry","PPPC_x_AdditionalOptions_in","s")%></th>
							<td align="left">
								<input type="text" name="wan_pppoe_options" value="<%tcWebApi_get("Wan_PVC", "wan_pppoe_options", "s")%>" class="input_32_table" maxlength="255" onKeyPress="return validator.isString(this, event)">
							</td>
						</tr>
						</table>
					</td>
				</tr>

				<tr id="wan_DHCP_opt">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2">DHCP option</td>
						</tr>
						</thead>
						<tr id="Country_tr">
							<th width="40%">Class-identifier (option 60):</th>
							<td>
								<input type="text" name="dhcp_vendorid" class="input_25_table" value="<% tcWebApi_get("Wan_PVC","dhcp_vendorid","s") %>" maxlength="126" autocapitalization="off" autocomplete="off">
							</td>
						</tr>
						<tr id="ISP_tr">
							<th width="40%">Class-identifier (option 61):</th>
							<td>
								<input type="checkbox" id="tmp_dhcp_clientid_type" name="tmp_dhcp_clientid_type" onclick="showDisableDHCPclientID(this);" <%if tcWebApi_get("Wan_PVC","dhcp_clientid_type","h") = "Yes" then asp_Write("checked") end if%> >IAID/DUID<br>
								<input type="text" name="dhcp_clientid" class="input_25_table" value="<% tcWebApi_get("Wan_PVC","dhcp_clientid","s") %>" maxlength="126" autocapitalization="off" autocomplete="off">
							</td>
						</tr>
						</table>
					</td>
				</tr>		

				<tr id="wan_ISPRequirement">
					<td bgcolor="#4D595D">
						<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
						<thead>
						<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","PPPC_x_HostNameForISP_sn","s")%></td>
						</tr>
						</thead>
						<tr id="wan_hostname">
							<th><%tcWebApi_get("String_Entry","PPPC_x_HostNameForISP_in","s")%></th>
							<td align="left">
								<input type="text" name="wan_hostname" class="input_32_table" maxlength="32" value="<% tcWebApi_get("Wan_PVC", "wan_hostname", "s") %>" onkeypress="return validator.isString(this, event)">
								<br/><span id="alert_msg1" style="color:#FC0;"></span>
							</td>
						</tr>
						<tr id="wan_hwaddr_x">
							<th><%tcWebApi_get("String_Entry","PPPC_x_MacAddressForISP_in","s")%></th>
							<td align="left">
								<input type="text" name="wan_hwaddr_x" class="input_20_table" maxlength="17" value="<% tcWebApi_get("Wan_PVC", "WAN_MAC", "s") %>" onKeyPress="return validator.isHWAddr(this,event)">
								<input type="button" class="button_gen_long" onclick="showMAC();" value="<%tcWebApi_get("String_Entry","BOP_isp_MACclone","s")%>">
							</td>
						</tr>
						</table>
					</td>
				</tr>

				<tr>
					<td bgcolor="#4D595D" valign="top">
						<div class="apply_gen">
							<INPUT TYPE="button" NAME="SaveBtn" class="button_gen" VALUE="<%tcWebApi_get("String_Entry","CTL_onlysave","s")%>" onClick="applyRule()">						
							<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then %>
							<%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") <> "0" then%>
							<%if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then%>
							<INPUT TYPE="BUTTON" NAME="DeleteBtn" class="button_gen" VALUE="<%tcWebApi_get("String_Entry","CTL_del","s")%>" onClick="doDelete()">
							<% end if %>
							<% end if %>
							<% end if %>
							<%if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then %>
							<%if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") <> "0" then%>
							<%if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then %>
							<INPUT TYPE="BUTTON" NAME="DeleteBtn" class="button_gen" VALUE="<%tcWebApi_get("String_Entry","CTL_del","s")%>" onClick="doDelete()">
							<% end if %>
							<% end if %>
							<% end if %>
							<%if tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then %>
							<%if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") <> "0" then%>
							<%if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then %>
							<INPUT TYPE="BUTTON" NAME="DeleteBtn" class="button_gen" VALUE="<%tcWebApi_get("String_Entry","CTL_del","s")%>" onClick="doDelete()">
							<% end if %>
							<% end if %>
							<% end if %>
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
<form method="post" name="usbform" action="/cgi-bin/start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_DSL_Content.asp">
<input type="hidden" name="next_page" value="Advanced_Modem_Content.asp">
<input type="hidden" name="USBInfo_PVC" value="11">
<input type="hidden" name="USB_TransMode" value="USB">
</form>
<div id="footer"></div>
</body>

<!--Advanced_DSL_Content.asp-->
</html>


