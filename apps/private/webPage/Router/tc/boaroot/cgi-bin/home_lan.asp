<%
If Request_Form("isIPv6Supported") <> "1" Then
	tcWebApi_set("Wan_PVC","IPVERSION","IPv4/IPv6")
End If

If Request_Form("lanFlag") <> "" Then

	tcWebApi_set("WebCurSet_Entry","lan_id","lan_VC")
	
		TCWebApi_set("Lan_Dhcp","type","dhcpTypeRadio")
	
	If Request_Form("aliasFlag")="Yes" Then
		TCWebApi_set("WebCurSet_Entry","lanAlias_id","lan_Alias_VC")
		TCWebApi_set("LanAlias_Entry","IP","uiViewAliasIPAddr")
		TCWebApi_set("LanAlias_Entry","netmask","uiViewAliasNetMask")	
		If Request_Form("dhcpFlag") ="0" Then		
			tcWebApi_commit("LanAlias")
		End If
	End If
	
        TCWebApi_set("Lan_Entry","IP","uiViewIPAddr")
        TCWebApi_set("Lan_Entry","netmask","uiViewNetMask")
        TCWebApi_set("Lan_IgmpSnoop","igmpsnoopEnable","lan_Igmpsnoop")		
        TCWebApi_set("Lan_Entry","IP6","uiViewIPv6Addr")
        TCWebApi_set("Lan_Entry","PREFIX6","uiViewIPv6Prefix")
        
        TCWebApi_set("LanRipd_Entry","RIPVERSION","lan_RIP")
        TCWebApi_set("LanRipd_Entry","DIRECTION","lan_RIP_Dir")

		If Request_Form("isIPv6Supported") = "1" Then 
			if tcwebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" then
				TCWebApi_set("LanRipd_Entry","RIPNGENABLE","ripngEnableRadio")
				If Request_Form("ripngEnableRadio") = "1" Then 
					TCWebApi_set("LanRipd_Entry","RIPNGDIRECTION","lan_Ripng_Dir")
				End if
			end if
		End if
	
	if tcwebApi_get("WebCustom_Entry","isMLDSupported","h") = "Yes" then
		TCWebApi_set("Lan_MldSnoop","mldsnoopEnable","lan_Mldsnoop")
	end if

	If tcWebApi_get("Lan_EtherMedia","enable","h") = "support" Then 
        	TCWebApi_set("Lan_EtherMedia","port0","EtherMedia0")
	        if tcWebApi_get("Info_Ether","is4PortsEther","h") = "Yes" Then
        	        TCWebApi_set("Lan_EtherMedia","port1","EtherMedia1")
                	TCWebApi_set("Lan_EtherMedia","port2","EtherMedia2")
	                TCWebApi_set("Lan_EtherMedia","port3","EtherMedia3")
        	End If
        End If 

        If Request_Form("dhcpFlag") ="0" Then
                tcWebApi_commit("LanRipd_Entry")
				tcWebApi_save()
                tcWebApi_commit("Lan")
        End If

        If Request_Form("dhcpTypeRadio")="0" Then
                TCWebApi_unset("DhcpRelay_Entry")
                TCWebApi_unset("Dproxy_Entry")
                If Request_Form("dhcpFlag") ="0" Then
                        tcWebApi_commit("DhcpRelay_Entry")
                        tcWebApi_commit("Dhcpd_Common")
                        tcWebApi_commit("Dproxy_Entry")
                End If
        End If
        
        If Request_Form("dhcpTypeRadio")="1" Then
				TCWebApi_unset("Dhcpd_Common")
                TCWebApi_set("Dhcpd_Common","start","StartIp")
                TCWebApi_set("Dhcpd_Common","pool_count","PoolSize")
                TCWebApi_set("Dhcpd_Common","lease","dhcp_LeaseTime")
				
				' set dhcp port filter value to cfg node 
				if request_Form("DHCPFilterFlag") = "Yes" then
					if request_Form("QoS1PortsFlag") = "Yes" then
						TCWebApi_set("Dhcpd_Common","eth0","DHCPPhyPortEth0")
					else
						TCWebApi_set("Dhcpd_Common","eth0.1","DHCPPhyPortEth0")
						TCWebApi_set("Dhcpd_Common","eth0.2","DHCPPhyPortEth1")
					end if
					if request_Form("QoS2PortsFlag") <> "Yes" then
						if request_Form("QoS1PortsFlag") <> "Yes" then
							TCWebApi_set("Dhcpd_Common","eth0.3","DHCPPhyPortEth2")
							TCWebApi_set("Dhcpd_Common","eth0.4","DHCPPhyPortEth3")
						end if
					end if
					'TCWebAPI_set("Dhcpd_Common","usb0","DHCPPhyPortUsb0")
					if request_Form("wlanISExist") = "On" then
						if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then
							TCWebAPI_set("Dhcpd_Common","ra0","DHCPPhyPortRa0")
						elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "2" then
							TCWebAPI_set("Dhcpd_Common","ra0","DHCPPhyPortWLANMssid0")
							TCWebAPI_set("Dhcpd_Common","ra1","DHCPPhyPortWLANMssid1")
						elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "3" then
							TCWebAPI_set("Dhcpd_Common","ra0","DHCPPhyPortWLANMssid0")
							TCWebAPI_set("Dhcpd_Common","ra1","DHCPPhyPortWLANMssid1")
							TCWebAPI_set("Dhcpd_Common","ra2","DHCPPhyPortWLANMssid2")
						elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "4" then
							TCWebAPI_set("Dhcpd_Common","ra0","DHCPPhyPortWLANMssid0")
							TCWebAPI_set("Dhcpd_Common","ra1","DHCPPhyPortWLANMssid1")
							TCWebAPI_set("Dhcpd_Common","ra2","DHCPPhyPortWLANMssid2")
							TCWebAPI_set("Dhcpd_Common","ra3","DHCPPhyPortWLANMssid3")	
						end if
					end if
				end if
				
                If Request_Form("option60Flag")="Yes" Then
                        TCWebApi_set("Dhcpd_Option60","start","ConPoolStartIp")
                        TCWebApi_set("Dhcpd_Option60","pool_count","ConPoolPoolSize")
                        TCWebApi_set("Dhcpd_Option60","VendorID","VendorClassID")
                        TCWebApi_set("Dhcpd_Option60","lease","dhcp_LeaseTime")
                        If Request_Form("dhcpFlag") ="0" Then				
                                tcWebApi_commit("Dhcpd_Option60")
                        End If
                End If

                tcWebApi_set("Dproxy_Entry","Active","DNSproxy")
                TCWebApi_set("Dproxy_Entry","type","dnsTypeRadio")
                TCWebApi_set("Dproxy_Entry","Primary_DNS","PrimaryDns")
                TCWebApi_set("Dproxy_Entry","Secondary_DNS","SecondDns")
                                        
                If Request_form("delnum") <> "" Then
                        tcWebApi_set("WebCurSet_Entry","dhcpd_id","delnum")
                        tcWebApi_unset("Dhcpd_Entry")	
                elseif Request_Form("addFlag")="1" Then
                        if Request_form("emptyEntry") <> "" then
                        tcWebApi_set("WebCurSet_Entry","dhcpd_id","emptyEntry")				
                        tcWebApi_set("Dhcpd_Entry","IP","IpAddr")
                        tcWebApi_set("Dhcpd_Entry","MAC","MACAddr")
                        end if
                end if	

                TCWebApi_unset("DhcpRelay_Entry")
                If Request_Form("dhcpFlag") ="0" Then
                        tcWebApi_commit("DhcpRelay_Entry")
                        tcWebApi_commit("Dhcpd_Common")
                        tcWebApi_commit("Dproxy_Entry")
                End If
        End If
        
        If Request_Form("dhcpTypeRadio")="2" Then
                TCWebApi_set("DhcpRelay_Entry","Server","ServerIp")
                TCWebApi_unset("Dhcpd_Common")
                TCWebApi_unset("Dproxy_Entry")
                If Request_Form("dhcpFlag") ="0" Then
                        tcWebApi_commit("DhcpRelay_Entry")
                        tcWebApi_commit("Dhcpd_Common")
                        tcWebApi_commit("Dproxy_Entry")
                End If
        End If
		

		tcWebApi_set("DyVLAN_Common","Type","OUIDeviceSeleradio")	

	if Request_Form("ouiFlag")="3" Then
		tcWebApi_set("WebCurSet_Entry","DyVlan_Index","IndexOfOUI")
		If Request_Form("OUIDeviceSeleradio")="STB" Then
			tcWebApi_set("DyVLAN_Entry","STB_OUI","uiOUI")
                elseif Request_Form("OUIDeviceSeleradio")="Voip" Then
                        tcWebApi_set("DyVLAN_Entry","Voip_OUI","uiOUI")
                End If
                If Request_Form("dhcpFlag") ="0" Then
                	tcWebApi_commit("DyVLAN_Entry")	
                End If
        End If
End If
	
	
	If Request_Form("isIPv6Supported") = "1" Then 
		

	
			If Request_Form("radvdFlag")="1" Then
				TCWebApi_set("Radvd_Entry","Enable","radvdEnableFlag")
				If Request_Form("radvdEnableFlag")="1" Then
					TCWebApi_set("Radvd_Entry","Mode","radvdModeFlag")
				if tcwebApi_get("WebCustom_Entry","isRaFlagSetSupported","h") = "Yes" then
					TCWebApi_set("Radvd_Entry","M_Flag","radvdManagedAddrflag")
					TCWebApi_set("Radvd_Entry","O_Flag","radvdOtherConfigflag")
				end if
				End If
			End If
			
			If Request_Form("radvdFlag")="0" Then
				TCWebApi_set("Radvd_Entry","Enable","radvdEnableFlag")
				If Request_Form("radvdEnableFlag")="1" Then
					TCWebApi_set("Radvd_Entry","Mode","radvdModeFlag")
					if tcwebApi_get("WebCustom_Entry","isRaFlagSetSupported","h") = "Yes" then
					TCWebApi_set("Radvd_Entry","M_Flag","radvdManagedAddrflag")
					TCWebApi_set("Radvd_Entry","O_Flag","radvdOtherConfigflag")
					end if
					If Request_Form("radvdModeFlag")="1" Then
						TCWebApi_set("Radvd_Entry","PrefixIPv6","uiViewIPv6PrefixRadvd")
						TCWebApi_set("Radvd_Entry","Prefixv6Len","uiViewIPv6PrefixLenRadvd")
						TCWebApi_set("Radvd_Entry","PreferredLifetime", "uiPreferredLifetimeRadvd")
						TCWebApi_set("Radvd_Entry","ValidLifetime","uiValidLifetimeRadvd")
						If Request_Form("dhcpFlag") ="0" Then					
							tcWebApi_commit("Radvd_Entry")
						End If
					End If
				End If		
			End If
			
			If Request_Form("dhcp6sFlag")="1" Then
				TCWebApi_set("Dhcp6s_Entry","Enable","dhcp6sEnableFlag")
				TCWebApi_set("Dhcp6s_Entry","Mode","dhcp6sModeFlag")						
			End If
			
			If Request_Form("dhcp6sFlag")="0" Then
				
					TCWebApi_set("Dhcp6s_Entry","Enable","dhcp6sEnableFlag")
					
				If Request_Form("dhcp6sEnableFlag") = "1" Then
					TCWebApi_set("Dhcp6s_Entry","Mode","dhcp6sModeFlag")
					If Request_Form("dhcp6sModeFlag") = "1" Then
							TCWebApi_set("Dhcp6s_Entry","PrefixIPv6","uiViewIPv6DHCPPrefix")
							TCWebApi_set("Dhcp6s_Entry","Prefixv6Len","uiViewIPv6DHCPPrefixLen")
							TCWebApi_set("Dhcp6s_Entry","PreferredLifetime", "uiPreferredLifetimeDHCP6")
							TCWebApi_set("Dhcp6s_Entry","ValidLifetime","uiValidLifetimeDHCP6")
							TCWebApi_set("Dhcp6s_Entry","DNSserver","uiPrimaryDNSDHCP6")
							TCWebApi_set("Dhcp6s_Entry","SecDNSserver","uiSecondaryDNSDHCP6")						
					End if
				End if
				If Request_Form("dhcpFlag") ="0" Then
					tcWebApi_commit("Radvd_Entry")
					tcWebApi_commit("Dhcp6s_Entry")	
				End if	
			End If

End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" tppabs="/style.css" type="text/css">
<script language="JavaScript" type="text/javascript" src="/jsl.js"></script>
<script language="JavaScript" type="text/javascript" src="/val.js"></script>
<script language="JavaScript" type="text/javascript" src="/pvc.js"></script>
<script language="JavaScript" type="text/javascript" src="/ip.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript">

function showTable(id,header,data,keyIndex){
	var html = ["<table id=client_list width=580 border=1 align=center cellpadding=1 cellspacing=0  bordercolor=#CCCCCC bgcolor=#FFFFFF>"];
	// 1.generate table header
	html.push("<tr>");
	for(var i =0; i<header.length; i++){
		html.push("<td width=" + header[i][0] + " align=center class=tabdata>" +"<STRONG><FONT color=#000000>"+ header[i][1] +"</font></strong>"+ "</td>");
	}
	html.push("</tr>");
	// 2.generate table data
	for(var i =0; i<data.length; i++){
		if(data[i][keyIndex] != "N/A"){
			html.push("<tr>");
			for(var j=0; j<data[i].length; j++){
				html.push("<td align=center class=tabdata>" + data[i][j] + "</td>");
			}
			html.push("</tr>");
		}
	}
	html.push("</table>");
	if(parseInt(document.uiViewLanForm.LeaseNum.value)>10)
	{
		html.push("<input type=button name=MORE  value=More... onClick=javascript:window.open(\"/cgi-bin/more_client_list.asp\")>")
	}
	document.getElementById(id).innerHTML = html.join('');
}

function doDelete(i)
{
	document.uiViewLanForm.delnum.value=i;
	document.uiViewLanForm.submit();
}


function checkPhysicalPort(){

	if(document.forms[0].DHCPPhyPortEth0.checked)
		document.forms[0].DHCPPhyPortEth0.value = "Yes";
	else
		document.forms[0].DHCPPhyPortEth0.value = "No";
		
	if (document.forms[0].DHCP1PortsFlag.value != "Yes")
		if(document.forms[0].DHCPPhyPortEth1.checked)
			document.forms[0].DHCPPhyPortEth1.value = "Yes";
		else
			document.forms[0].DHCPPhyPortEth1.value = "No";
		
	if (document.forms[0].DHCP2PortsFlag.value != "Yes" && document.forms[0].DHCP1PortsFlag.value != "Yes") {
		if(document.forms[0].DHCPPhyPortEth2.checked)
			document.forms[0].DHCPPhyPortEth2.value = "Yes";
		else
			document.forms[0].DHCPPhyPortEth2.value = "No";		
			
		if(document.forms[0].DHCPPhyPortEth3.checked)
			document.forms[0].DHCPPhyPortEth3.value = "Yes";
		else
			document.forms[0].DHCPPhyPortEth3.value = "No";
	}
/*		
	if(document.forms[0].DHCPPhyPortUsb0.checked)
		document.forms[0].DHCPPhyPortUsb0.value = "Yes";
	else
		document.forms[0].DHCPPhyPortUsb0.value = "No";

*/
	if (document.forms[0].wlanISExist.value == "On") {
		if (document.forms[0].DHCPMBSSIDNumberFlag.value == "1") {	
			if(document.forms[0].DHCPPhyPortRa0.checked)
				document.forms[0].DHCPPhyPortRa0.value = "Yes";
			else
				document.forms[0].DHCPPhyPortRa0.value = "No";
		}
		if(document.forms[0].DHCPMBSSIDNumberFlag.value == "2"){
			if(document.forms[0].DHCPPhyPortWLANMssid0.checked)
				document.forms[0].DHCPPhyPortWLANMssid0.value = "Yes"
			else
				document.forms[0].DHCPPhyPortWLANMssid1.value = "No"
			if(document.forms[0].DHCPPhyPortWLANMssid1.checked)
				document.forms[0].DHCPPhyPortWLANMssid1.value = "Yes"
			else
				document.forms[0].DHCPPhyPortWLANMssid1.value = "No"
		}
		if(document.forms[0].DHCPMBSSIDNumberFlag.value == "3"){
			if(document.forms[0].DHCPPhyPortWLANMssid0.checked){
				document.forms[0].DHCPPhyPortWLANMssid0.value = "Yes"
			}
			else
				document.forms[0].DHCPPhyPortWLANMssid0.value = "No"
			if(document.forms[0].DHCPPhyPortWLANMssid1.checked)
				document.forms[0].DHCPPhyPortWLANMssid1.value = "Yes"
			else
				document.forms[0].DHCPPhyPortWLANMssid1.value = "No"
			if(document.forms[0].DHCPPhyPortWLANMssid2.checked)
				document.forms[0].DHCPPhyPortWLANMssid2.value = "Yes"
			else
				document.forms[0].DHCPPhyPortWLANMssid2.value = "No"
		}
		if(document.forms[0].DHCPMBSSIDNumberFlag.value == "4"){	
			if(document.forms[0].DHCPPhyPortWLANMssid0.checked){	
				document.forms[0].DHCPPhyPortWLANMssid0.value = "Yes"
			}
			else
				document.forms[0].DHCPPhyPortWLANMssid0.value = "No"
			if(document.forms[0].DHCPPhyPortWLANMssid1.checked)
				document.forms[0].DHCPPhyPortWLANMssid1.value = "Yes"
			else
				document.forms[0].DHCPPhyPortWLANMssid1.value = "No"
			if(document.forms[0].DHCPPhyPortWLANMssid2.checked)
				document.forms[0].DHCPPhyPortWLANMssid2.value = "Yes"
			else
				document.forms[0].DHCPPhyPortWLANMssid2.value = "No"
			if(document.forms[0].DHCPPhyPortWLANMssid3.checked)
				document.forms[0].DHCPPhyPortWLANMssid3.value = "Yes"
			else
				document.forms[0].DHCPPhyPortWLANMssid3.value = "No"
		}
	}
}

function StringCheck(val)
{
    re = /^[^\s]+$/;
    if( re.test(val) )
        return true;
    else
        return false;
}

function doCheckmacAddr(){
	var macstr = document.uiViewLanForm.MACAddr.value;
var maclen = macstr.length;
	var tmp = macstr.toUpperCase();
	document.uiViewLanForm.MACAddr.value = tmp;
if(maclen != 0){
		var findpos = macstr.search("^([0-9A-Fa-f]{2})(:[0-9A-Fa-f]{2}){5}$");
		if( findpos != 0 )
		{
			alert("Invalid MAC address:" + macstr);
		}	
		return findpos;
	}
	return 0;
}
function doUserModeUiMgmtIpValidate()
{
    var value;
    var value_temp;
    
    value = document.uiViewLanForm.uiViewIPAddr.value;
    value_temp = document.uiViewLanForm.uiViewNetMask.value;
    if(inValidNetAddr(value,value_temp))
        return false;
	//jrchen add		
	value = document.uiViewLanForm.isIPv6Supported.value;
	if(value == 1){
		//check if IPv6 address/prefix field is not inputed
		if(document.uiViewLanForm.uiViewIPv6Addr.value != "" || document.uiViewLanForm.uiViewIPv6Prefix.value != ""){
			//check IPv6 Address format	
			value = document.uiViewLanForm.uiViewIPv6Addr.value;	
			if(inValidIPv6Addr(value))
				return false;	
			if(false == isGlobalIpv6Address(value)){
				alert('Invalid IPv6 GlobalAddress: ' + value);
				return false;
			}
	
			//jrchen add
			//check IPv6 Prefix format
			value = document.uiViewLanForm.uiViewIPv6Prefix.value;
			if(inValidIPv6Prefix(value))
				return false;
		}
	}
				        
	if("Yes" == document.uiViewLanForm.aliasFlag.value)
	{	
		value = document.uiViewLanForm.uiViewAliasIPAddr.value;
        value_temp = document.uiViewLanForm.uiViewAliasNetMask.value;
        if(inValidNetAddr(value,value_temp))
             return false;  
			
		if(document.uiViewLanForm.uiViewIPAddr.value == document.uiViewLanForm.uiViewAliasIPAddr.value)
		{
			alert("Alias ip duplicate to Main ip!" );
			return false;
		}
	}
}
function doAdminUiMgmtIpValidate()
{
	var value;

	if(document.uiViewLanForm.dhcpTypeRadio[1].checked)    
	 {
    	value = document.uiViewLanForm.StartIp.value;
    	if(inValidIPAddr(value))
        return false;
		if("Yes" == document.uiViewLanForm.aliasFlag.value)
		{
			if(!doPoolRangeAlias())
				return false;
		}
		else
		{
      if(!doPoolRange())
        return false;
		}
		
		/* set physical port value */
		if ("Yes" == document.uiViewLanForm.DHCPFilterFlag.value)
			checkPhysicalPort();
		
      	if("Yes" == document.uiViewLanForm.option60Flag.value)
	{	
		value = document.uiViewLanForm.ConPoolStartIp.value;
		if(inValidIPAddr(value))
			return false;

		value = document.uiViewLanForm.ConPoolPoolSize.value;
    		if(value.match("[^0-9]") != null) {
        		alert("IP Conditional Pool Count needs to be an positive integer");
       	 		return false;
    		}
    		
         	if(!doConditionalPoolRange())
        		return false;
			
	}
	
      if(document.uiViewLanForm.dnsTypeRadio[1].checked)
      {  
      	value = document.uiViewLanForm.PrimaryDns.value;
    	if(inValidIPAddr(value))
        	return false;  
          
      	value = document.uiViewLanForm.SecondDns.value;
    	if(inValidIPAddr(value))
        	return false;  
      } 
      if(doValidateServer())
      	return false;  

<%if tcwebApi_get("WebCustom_Entry","isDhcpStaticSupported","h")="Yes" then%>	
	if(StringCheck(document.uiViewLanForm.IpAddr.value) || StringCheck(document.uiViewLanForm.MACAddr.value))
		{
			if(document.uiViewLanForm.emptyEntry.value == "N/A")
			{
				alert("DHCP reservation table is full!" );
				return false;
			} 
			value = document.uiViewLanForm.IpAddr.value;
			if(inValidIPAddr(value))
			return false; 
				
			if(doCheckmacAddr())
			return false;	
			
			if(!doStaticTableRange())
			return false;			
				
			document.uiViewLanForm.addFlag.value = "1";
		}
		if(document.uiViewLanForm.staticNum.value != "0"){
		if(document.uiViewLanForm.tmpStartIp.value != document.uiViewLanForm.StartIp.value)
		{
			if(confirm("Change Start Ip may lead to reservation item be deleted!\nContinue?") == false)
				return false;
		}
		if(document.uiViewLanForm.tmpPoolCount.value != document.uiViewLanForm.PoolSize.value)
		{
			if(confirm("Change Ip pool count may lead to reservation item be deleted!\nContinue?") == false)
				return false;
		}
		}
    <% end if %>            
    }
    if(document.uiViewLanForm.dhcpTypeRadio[2].checked)
    {
    	value = document.uiViewLanForm.ServerIp.value;
    	 if(inValidIPAddr(value))
    	 	return false;
    }
}
function uiMgmtIpDoValidate() {
	if(doUserModeUiMgmtIpValidate()==false)
		return false;
	if(document.uiViewLanForm.userMode.value != 1)
	{
		if(doAdminUiMgmtIpValidate()==false)
			return false;
	}
    return true;
}
function reloadAdminAction()
{
	if(!dhcpRelayCheck()){
		return false;
	}
	
	onloadCheck();
	
	if(document.uiViewLanForm.dhcpTypeRadio[0].checked)
	{
		document.getElementById("dhcp_enabled_div0").style.display="none";
		document.getElementById("dhcp_relay_div").style.display="none";
		<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
		document.getElementById("dhcp_client_div").style.display="none";
		<% end if %>
	}
	else if(document.uiViewLanForm.dhcpTypeRadio[1].checked)
	{
		document.getElementById("dhcp_enabled_div0").style.display="";
		document.getElementById("dhcp_relay_div").style.display="none";
		<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
		document.getElementById("dhcp_client_div").style.display="none";
		<% end if %>
	}
	else if(document.uiViewLanForm.dhcpTypeRadio[2].checked)
	{
		document.getElementById("dhcp_enabled_div0").style.display="none";
		document.getElementById("dhcp_relay_div").style.display="";
		<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
		document.getElementById("dhcp_client_div").style.display="none";
		<% end if %>
	}
	<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
	else if(document.uiViewLanForm.dhcpTypeRadio[3].checked)
	{
		document.getElementById("dhcp_enabled_div0").style.display="none";
		document.getElementById("dhcp_relay_div").style.display="none";
		document.getElementById("dhcp_client_div").style.display="";
	}
	<% else %>
	else
	;
	<% end if %>
	return;
}
function reloadUsermodeAction()
{
	return;
}
function doReload() {
	reloadUsermodeAction();
	if(document.uiViewLanForm.userMode.value !=1){
		reloadAdminAction();
	}
	return;
}

function doUserModeDispaly()
{
	return;
}
function doAdminDispaly()
{
<% If tcWebApi_get("Lan_Dhcp","type","h") = "0" Then%>
	{
		document.getElementById("dhcp_enabled_div0").style.display="none";
		document.getElementById("dhcp_relay_div").style.display="none";
		<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
		document.getElementById("dhcp_client_div").style.display="none";
		<% end if %>
	}
	<% end if %>

	<% If tcWebApi_get("Lan_Dhcp","type","h") = "1" Then%>
	{
		document.getElementById("dhcp_enabled_div0").style.display="";
		document.getElementById("dhcp_relay_div").style.display="none";
		<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
		document.getElementById("dhcp_client_div").style.display="none";
		<% end if %>
	}
	<% end if %>

	<% If tcWebApi_get("Lan_Dhcp","type","h") = "2" Then%>
	{
		document.getElementById("dhcp_enabled_div0").style.display="none";
		document.getElementById("dhcp_relay_div").style.display="";
		<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
		document.getElementById("dhcp_client_div").style.display="none";
		<% end if %>
	}
	<% end if %>
	
	<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
	<% If tcWebApi_get("Lan_Dhcp","type","h") = "3" Then%>
	{
		document.getElementById("dhcp_enabled_div0").style.display="none";
		document.getElementById("dhcp_relay_div").style.display="none";
		document.getElementById("dhcp_client_div").style.display="";
	}
	<% end if %>
	<% end if %>
	return;
}
function doDisplay() {
	doUserModeDispaly();
	if(document.forms[0].userMode.value != 1)
	{
		doAdminDispaly();
	}
	return;
}
function doValidateRange(startIP,endIP) {
    var staddress;
    var edaddress;
    var cnt;

    staddress=startIP.split(".");
    edaddress=endIP.split(".");
    for(cnt=0; cnt < 4; cnt++) {
        if(Number(edaddress[cnt])<Number(staddress[cnt])) {
            alert("End IP address is less than Start IP address");
	return false;
}
    }
    return true;
}

function doValidateServer() {
    var Element;
    var ElementValue;

    Element = document.uiViewLanForm.PoolSize;
    ElementValue = Element.value;
    if(ElementValue.match("[^0-9]") != null) {
        alert("IP Pool Count needs to be an positive integer");
        return true;
    }
    Element = document.uiViewLanForm.dhcp_LeaseTime;
    ElementValue = Element.value;
    if(ElementValue.match("[^0-9]") != null) {
        alert("Lease Time needs to be an positive integer");
        return true;
    }
    if(!StringCheck(ElementValue))
	{
		alert("Empty Lease Time!!");
		return true;
	}
	if(!parseInt(ElementValue))
		document.uiViewLanForm.dhcp_LeaseTime.value = 259200;
    Element = document.uiViewLanForm.StartIp;
    ElementValue = Element.value;
    if(inValidIPAddr(Element.value)) return true;
    if(doValidateRange(ElementValue,Element.value)!=true) return true;
	return false;
}

function poolcheck(st,pool,value,Mvalue){
	if( (pool > 253) || (st+pool) > ((Mvalue & st) + value - 1) )
	{
		//mtk04880: for resolving bug 12324 : showing alert twice
	  	//alert("DHCP IP Pool Range exceed limit!!");
		return false;
	}else
	{
		return true;
	}
}
function checkPoolOverlap(startIP1, num1, startIP2, num2)
{
	var digits1 = startIP1[0].split(".");
	var digits2 = startIP2[0].split(".");
	for(i=0;i<4;i++)
	{
		if(parseInt(digits1[i]) < parseInt(digits2[i]))
		{
			if(parseInt(digits1[3])+num1-1 >= parseInt(digits2[3]))
				return true;
			else
				return false;
		}
		if(parseInt(digits1[i]) > parseInt(digits2[i]))
		{
			if(parseInt(digits2[3])+num2-1 >= parseInt(digits1[3]))
				return true;
			else
				return false;
		}
	}
	return true;
}

<%if tcwebApi_get("WebCustom_Entry","isDhcpStaticSupported","h")="Yes" then%>
function doStaticTableRange()
{
	var sIP = document.uiViewLanForm.StartIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits = sIP[0].split(".");
	var num = document.uiViewLanForm.PoolSize.value.match("^[0-9]{1,3}$");
	var mask = document.uiViewLanForm.uiViewNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");	
	var staticIP = document.uiViewLanForm.IpAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var staticdigits = staticIP[0].split(".");
	
	var stIP = parseInt(digits[3]);
	var Pool_num = parseInt(num);
	var staticIP = parseInt(staticdigits[3]);
	var isIpValid = 1;
	var total = parseInt(document.uiViewLanForm.staticNum.value);
	var leaseTotal = parseInt(document.uiViewLanForm.LeaseNum.value);
	
	for(i=0;i<4;i++)
	{
	  if((digits2[i] & digits[i]) != (digits2[i] & staticdigits[i])){	
			isIpValid = 0;
			break;
		}
	}
	if("Yes" == document.uiViewLanForm.aliasFlag.value){
		var mask = document.uiViewLanForm.uiViewAliasNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
		var digits3= mask[0].split(".");
		if(isIpValid == 0){
		for(i=0;i<4;i++)
		{
		  if((digits3[i] & digits[i]) != (digits3[i] & staticdigits[i])){	
				isIpValid = 0;
				break;
			}
		}
		}
	}
	if(isIpValid == 0){
		alert("DHCP Start IP and DHCP Static IP are not in the same subnet");                                   						
		return false;
	}
	
    if( staticIP <  stIP || staticIP >= stIP+Pool_num)
	{
	   alert("DHCP Static IP is out of DHCP pool range!");
	return false;
	}


	var table=document.getElementById("static_list");
	for(j=1; j<=total; j++)
	{	
		var str1 = table.rows[j].cells[1].innerHTML.replace(" ","");
		var str2 = table.rows[j].cells[2].innerHTML.replace(" ","");
		if( str1 == document.uiViewLanForm.IpAddr.value)
		{
			   alert("DHCP Static IP has existed in the list!");
			 return false;
		 }
		 if(str2 == document.uiViewLanForm.MACAddr.value)
		{
			   alert("DHCP Static MAC address has existed in the list!");
			 return false;
		 }
	}
/*	
	var table=document.getElementById("client_list").contentWindow.document.getElementById("dhcplist");
	
	for(j=1; j<=leaseTotal; j++)
	{	
		if(table.rows[j].cells[2].innerText == document.uiViewLanForm.IpAddr.value)
		{
			   alert("DHCP Client IP has existed in the list!");
			 return false;
		 }
		 if(table.rows[j].cells[3].innerText == document.uiViewLanForm.MACAddr.value)
		{
			   alert("DHCP Client MAC address has existed in the list!");
			 return false;
		 }
	}
*/
	return true;
}
<% End If %>
function doPoolRange()
{
	var sIP = document.uiViewLanForm.StartIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits = sIP[0].split(".");
 	var num = document.uiViewLanForm.PoolSize.value.match("^[0-9]{1,3}$");
	var mask = document.uiViewLanForm.uiViewNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");
	var lanip = document.uiViewLanForm.uiViewIPAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits3 = lanip[0].split(".");
	
  var stIP = parseInt(digits[3]);
  var Pool_num = parseInt(num);
	var Mask = parseInt(digits2[3]);
	
	if(!StringCheck(document.uiViewLanForm.PoolSize.value))
	{
		alert("Empty IP Pool Count!!");
		return false;
	}
  if(document.uiViewLanForm.uiViewIPAddr.value == document.uiViewLanForm.StartIp.value)
	{
		alert("DHCP Start IP and Router Local IP are not allowed to be the same");   	                                   								
		return false;
	}
	
	for(i=0;i<4;i++)
	{
	  if((digits2[i] & digits3[i]) != (digits2[i] & digits[i]))
		{
			alert("DHCP Start IP and Router Local IP are not in the same subnet");                                   						
			return false;
		}
	}
		
  if( (digits2[0]== 255) && (digits2[1] == 255) && (digits2[2] == 255) )
	{
		for( n=0; n<7; n++ )
     {
       k = (256 >> n) ;
       if((256 - k) == digits2[3])
       {
    	   if( !poolcheck(stIP,Pool_num,k,Mask) ){
    	   		//mtk04880: for resolving bug 12324 : showing alert twice
    	   		alert("DHCP IP Pool Range exceed limit!!");
    	   		return false;
    	   }
         
       }
     }
	}else
	{
		if(Pool_num > 254)
		{
	  	alert("DHCP IP Pool Range exceed limit!!"); 	                                   								
			return false;
		}
	}
	return true;
}

function doPoolRangeAlias()
{
	var sIP = document.uiViewLanForm.StartIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits = sIP[0].split(".");
 	var num = document.uiViewLanForm.PoolSize.value.match("^[0-9]{1,3}$");
	var mask = document.uiViewLanForm.uiViewNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");
	var lanip = document.uiViewLanForm.uiViewIPAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits3 = lanip[0].split(".");

	var mask = document.uiViewLanForm.uiViewAliasNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits4 = mask[0].split(".");
	var lanip = document.uiViewLanForm.uiViewAliasIPAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits5 = lanip[0].split(".");
	var MaskAlias = parseInt(digits4[3]);
	var validMainFlag = 1;
	var validAliasFlag = 1;
	var validDhcpMainFlag = 1;
	var validDhcpAliasFlag = 1;
	
    var stIP = parseInt(digits[3]);
  	var Pool_num = parseInt(num);
	var Mask = parseInt(digits2[3]);
	
	if(!StringCheck(document.uiViewLanForm.PoolSize.value))
	{
		alert("Empty IP Pool Count!!");
		return false;
	}
 	if(document.uiViewLanForm.uiViewIPAddr.value == document.uiViewLanForm.StartIp.value)
	{
		alert("DHCP Start IP and Router Local Main IP are not allowed to be the same");   	                                   								
		return false;
	}
 	if(document.uiViewLanForm.uiViewAliasIPAddr.value == document.uiViewLanForm.StartIp.value)
	{
		alert("DHCP Start IP and Router Local Alias IP are not allowed to be the same");   	                                   								
		return false;
	}
	
	for(i=0;i<4;i++)
	{
	  if((digits2[i] & digits3[i]) != (digits2[i] & digits[i]))
		{
			validMainFlag = 0;
			break;
			//alert("DHCP Start IP and Router Local IP are not in the same subnet");                                   						
			//return false;
		}
	}
	for(i=0;i<4;i++)
	{
		 if((digits4[i] & digits5[i]) != (digits4[i] & digits[i]))
		{
			validAliasFlag = 0;
			break;
			//alert("DHCP Start IP and Router Local Alias IP are not in the same subnet");                                   						
			//return false;
		}
	}
	
	if((validMainFlag == 0) && (validAliasFlag == 0))
	{
		alert("DHCP Start IP should be in the same subnet with main ip or alias ip!");
		return false;
	}
	if(Pool_num > 254)
	{
	 	alert("DHCP IP Pool Range exceed limit!!"); 	                                   								
		return false;
	}
	if(validMainFlag == 1)
	{
		if( (digits2[0]== 255) && (digits2[1] == 255) && (digits2[2] == 255) )
		{
			for( n=0; n<7; n++ )
     			{
       				k = (256 >> n) ;
       				if((256 - k) == digits2[3])
       				{
    	   				if( !poolcheck(stIP,Pool_num,k,Mask) )
    	   				{   
						validDhcpMainFlag = 0;
        					break;
        				}
       				}
     			}
		}
	}

	if(validAliasFlag == 1)
	{
		if( (digits4[0]== 255) && (digits4[1] == 255) && (digits4[2] == 255) )
		{
			for( n=0; n<7; n++ )
     			{
       			k = (256 >> n) ;
       			if((256 - k) == digits4[3])
       			{
    	   				if( !poolcheck(stIP,Pool_num,k,MaskAlias) )
    	   				{
						validDhcpAliasFlag = 0;
        					break;		
					}
       			}
     			}
		}
	}
	
	if( ((validMainFlag == 1 && validAliasFlag == 1) && ((validDhcpMainFlag | validDhcpAliasFlag) == 0)) 
	|| (validMainFlag == 1 && validDhcpMainFlag == 0)
	|| (validAliasFlag == 1 && validDhcpAliasFlag == 0) )
	{
		alert("DHCP IP Pool Range exceed limit!!"); 	            
		return false;
	}
	
	return true;
}

function doConditionalPoolRange()
{
	var sIP = document.uiViewLanForm.StartIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits = sIP[0].split(".");
 	var num = document.uiViewLanForm.PoolSize.value.match("^[0-9]{1,3}$");
	var mask = document.uiViewLanForm.uiViewNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits2 = mask[0].split(".");
	var lanip = document.uiViewLanForm.uiViewIPAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits3 = lanip[0].split(".");

    	var stIP = parseInt(digits[3]);
  	var Pool_num = parseInt(num);
	var Mask = parseInt(digits2[3]);

	var validMainFlag = 1;
	var validAliasFlag = 1;
	var validDhcpMainFlag = 1;
	var validDhcpAliasFlag = 1;

	var conPoolStartIP = document.uiViewLanForm.ConPoolStartIp.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  	var digits6 = conPoolStartIP[0].split(".");
 	var conPoolNum = document.uiViewLanForm.ConPoolPoolSize.value.match("^[0-9]{1,3}$");

  	var conPoolStartIPValue = parseInt(digits6[3]);
  	var conPoolNumValue = parseInt(conPoolNum);
  	
	if(document.uiViewLanForm.uiViewIPAddr.value == document.uiViewLanForm.ConPoolStartIp.value)
	{
		alert("DHCP Conditional Pool Start IP and Router Local Main IP are not allowed to be the same");   	                                   								
		return false;
	}
	
 	for(i=0;i<4;i++)
	{
		if((digits2[i] & digits3[i]) != (digits2[i] & digits6[i]))
	  	{
			validMainFlag = 0;
			break;
			//alert("DHCP Start IP and Router Local IP are not in the same subnet");                                   						
			//return false;
	  	}
	}

	if("Yes" == document.uiViewLanForm.aliasFlag.value)
	{
		var mask = document.uiViewLanForm.uiViewAliasNetMask.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
		var digits4 = mask[0].split(".");
		var lanip = document.uiViewLanForm.uiViewAliasIPAddr.value.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
		var digits5 = lanip[0].split(".");
		var MaskAlias = parseInt(digits4[3]);

		if(document.uiViewLanForm.uiViewAliasIPAddr.value == document.uiViewLanForm.ConPoolStartIp.value)
		{
			alert("DHCP Conditional Pool Start IP and Router Local Alias IP are not allowed to be the same");   	                                   								
			return false;
		}
		for(i=0;i<4;i++)
		{
			 if((digits4[i] & digits5[i]) != (digits4[i] & digits6[i]))
			{
				validAliasFlag = 0;
				break;
				//alert("DHCP Start IP and Router Local Alias IP are not in the same subnet");                                   						
				//return false;
			}
		}
		
	}


 	if("Yes" == document.uiViewLanForm.aliasFlag.value)
 	{
 		if((validMainFlag == 0) && (validAliasFlag == 0))
		{
			alert("DHCP Conditional Pool Start IP should be in the same subnet with Router Local IP or Alias ip!");
			return false;
		}

 	}
       else
 	{
 		if(validMainFlag == 0)
		{
			alert("DHCP Conditional Pool Start IP should be in the same subnet with Router Local IP!");
			return false;
		}

 	}
 
	if(document.uiViewLanForm.StartIp.value == document.uiViewLanForm.ConPoolStartIp.value)
	{
		alert("DHCP Conditional Pool Start IP and DHCP Main Pool Start IP are not allowed to be the same"); 
		return false;
	}
	if(checkPoolOverlap(sIP, Pool_num, conPoolStartIP, conPoolNumValue) == true)
	{
		alert("DHCP Conditional Pool is Overlap with DHCP Main Pool!");
		return false;
	} 
	if(conPoolNumValue > 254)
	{
	 	alert("DHCP Conditional Pool IP Range exceed limit!!"); 	                                   								
		return false;
	}
	
	if(validMainFlag == 1)
	{
		if( (digits2[0]== 255) && (digits2[1] == 255) && (digits2[2] == 255) )
		{
			for( n=0; n<7; n++ )
     			{
       			k = (256 >> n) ;
       			if((256 - k) == digits2[3])
       			{
    	  				if( !poolcheck(conPoolStartIPValue,conPoolNumValue,k,Mask) )
    	  				{
						validDhcpMainFlag = 0;
        					break;
					}
       			}
     			}
		}
	}
	if(validAliasFlag == 1)
	{
		if("Yes" == document.uiViewLanForm.aliasFlag.value)
		{
			if( (digits4[0]== 255) && (digits4[1] == 255) && (digits4[2] == 255) )
			{
				for( n=0; n<7; n++ )
     				{
       				k = (256 >> n) ;
       				if((256 - k) == digits4[3])
       				{
    						if( !poolcheck(conPoolStartIPValue,conPoolNumValue,k,MaskAlias) )
    						{
							validDhcpAliasFlag = 0;
       						break;
       					}
     					}
				}
			}
		}
	}

	if( ((validMainFlag == 1 && validAliasFlag == 1) && ((validDhcpMainFlag | validDhcpAliasFlag) == 0)) 
	|| (validMainFlag == 1 && validDhcpMainFlag == 0)
	|| (validAliasFlag == 1 && validDhcpAliasFlag == 0) )
	{
		alert("DHCP Conditional Pool IP Range exceed limit!!"); 
		return false;
	}
	return true;
}

function inValidIPv6PrefixDHCP6S(Address) {
	var address1 = Address.match("^[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}::$"); 
	var address2 = Address.match("^[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}::$");
	var address3 = Address.match("^[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}::$"); 
	var address4 = Address.match("^[0-9A-Fa-f]{1,4}::$");
	
	  if( (address1 != null) || (address2 != null) || (address3 != null) || (address4 != null) ) { 
  	
  	return true;
  }
  alert('Invalid IPv6 Prefix');
  return false;
}

function doUserModeSave()
{
	var form = document.uiViewLanForm;
	if (!uiMgmtIpDoValidate())
		return false;
	if (form.isIPv6Supported.value == 1 )
	{
		if(!checkAllIPv6Options())
			return false;
	}
	return true;
}

function doAdminSave()
{
	var form = document.uiViewLanForm;
	if(!dhcpRelayCheck()){
		return false;
	}
	<% if tcWebApi_get("GUI_Entry0","DyVlan","h") = "support" then %>
	if(true == uiOUICheck(document.uiViewLanForm.uiOUI))
		return false;
	document.uiViewLanForm.ouiFlag.value = 3;
<% End If %>
	return true;
}

function uiSave() {

	var form = document.uiViewLanForm;
	var straliasori = "<% tcWebApi_get("LanAlias_Entry","Active","s") %>";
	if (straliasori == "Yes") {
		if (form.uiViewAliasIPAddr.value == "0.0.0.0") {
			form.uiViewAliasNetMask.value = "0.0.0.0";
			form.aliasFlag.value = "No";
		}
	}
	
	if(false == doUserModeSave())
		return;
	if(document.uiViewLanForm.userMode.value != 1)
		if(false == doAdminSave())
			return;
	if (straliasori == "Yes") {
		form.aliasFlag.value = "Yes";
	}

	document.uiViewLanForm.dhcpFlag.value = 0;
	document.uiViewLanForm.submit();
}

function checkAllIPv6Options( )
{
//jrchen add			
	var value;
	// = document.uiViewLanForm.ipv6Flag.value;
	var form = document.uiViewLanForm;
	//if(value == 1)
	if(document.uiViewLanForm.uiViewIPv6Addr.value != "" || document.uiViewLanForm.uiViewIPv6Prefix.value != "")
	{
	//check IPv6 Address format	
	value = document.uiViewLanForm.uiViewIPv6Addr.value;	
	if(inValidIPv6Addr(value))
		return false;	
	
	if(false == isGlobalIpv6Address(value)){
		alert('Invalid IPv6 GlobalAddress: ' + value);
		return false;
	}
		//jrchen add
		//check IPv6 Prefix format
	value = document.uiViewLanForm.uiViewIPv6Prefix.value;
	if(!inValidPrefixLen(value))
		return false;
	}
	
 	if(form.radvdRadio[1].checked)
	{
		if( form.radvdModeFlag.value == 1)
		{
			if(! inValidIPv6PrefixDHCP6S(form.uiViewIPv6PrefixRadvd.value) )
				return false;
			if(!checkRadvdEnable( ) )
				return false;
		}		
	}
		if(form.dhcp6sEnableRadio[1].checked)
	{
		//if(! inValidIPv6Prefix(form.uiViewIPv6PrefixRadvd.value) )
		//	return;
		//if(!inValidIPv6PrefixDHCP6S(form.uiViewIPv6DHCPPrefix.value) )
			//return;
		if(!checkDHCP6SMode())
			return false;

	}    	
	document.uiViewLanForm.radvdFlag.value = 0;
	document.uiViewLanForm.dhcp6sFlag.value = 0;
	return true;
}
function checkRadvdEnable( )
{
	form = document.uiViewLanForm;
	var ret;
	if(form.radvdRadio[1].checked)
	{
		ret = checkRadvdInput( );
	}
	
	return ret;
}

	
	
function checkRadvdInput( )
{
	form = document.uiViewLanForm;
	
	if(!inValidIPv6PrefixDHCP6S(form.uiViewIPv6PrefixRadvd.value) )
		return false;
	if(!inValidPrefixLen(form.uiViewIPv6PrefixLenRadvd.value) )
		return false;
	if(!invalidLifetimeValue(form.uiPreferredLifetimeRadvd.value) )
		return false;
	if(!invalidLifetimeValue(form.uiValidLifetimeRadvd.value) )
		return false;
        var preferredlifetime = parseInt(form.uiPreferredLifetimeRadvd.value);
        var validlifetime = parseInt(form.uiValidLifetimeRadvd.value);
        if((validlifetime) <= (preferredlifetime) )
       {
                    alert("Validlifetime should be larger than Preferredlifetime!!");

                    return false;
    
       }
	return true
}

function checkDHCP6SMode( )
{
		form = document.uiViewLanForm;
		
		if(form.dhcp6sModeFlag.value == 1)
	{
		 if(!checkDHCP6SParam() )
		 	return false;
	}
	return true;
}


function checkDHCP6SParam()
{
	form = document.uiViewLanForm;
	if(!inValidIPv6PrefixDHCP6S(form.uiViewIPv6DHCPPrefix.value) )
		return false;
	if(!inValidPrefixLen(form.uiViewIPv6DHCPPrefixLen.value) )
		return false;		
	if(!invalidLifetimeValue(form.uiPreferredLifetimeDHCP6.value) )
		return false;
	if(!invalidLifetimeValue(form.uiValidLifetimeDHCP6.value) )
		return false;
        var preferredlifetime = parseInt(form.uiPreferredLifetimeDHCP6.value);
        var validlifetime = parseInt(form.uiValidLifetimeDHCP6.value);
        if((validlifetime) <= (preferredlifetime) )
       {
                    alert("Validlifetime should be larger than preferredlifetime!!");

                    return false;
       }
	if(inValidIPv6Addr(form.uiPrimaryDNSDHCP6.value) )
		return false;
	if(inValidIPv6Addr(form.uiSecondaryDNSDHCP6.value) )
		return false;
		
	return true;
}

function inValidPrefixLen(value1) {

	
	if(value1.match("[^0-9]") != null)  {

        alert("Radvd Prefix Length should not a number!!");
    	                    
        return false;
    }
	var PrefixLen = parseInt(value1);
	if (value1=="") {

        alert("Radvd Prefix Length should not be empty!!");

		return false;
	}
	if ( (PrefixLen > 64) || (PrefixLen < 16) ) {

        alert("Radvd Prefix Length should be between 16 and 64!!");

		return false;
	}
	return true;
}

function invalidLifetimeValue(value1) {
	var form = document.uiViewLanForm;
	
	if(value1.match("[^0-9]") != null)  {

        alert("Life Time should not a number!!");
    	                    
        return false;
    }
	var lifetime = parseInt(value1);
	if (value1 == "") {

        alert("Life Time value should not be empty!!");

		return false;
	}
	if ( (lifetime > 4294967295) || (lifetime < 300) ) {

        alert("Lifetime should be between 300 and 4294967295!!");

		return false;
	}
	return true;
}

function autoDNSRelay()
{
	document.uiViewLanForm.PrimaryDns.disabled = true;
	document.uiViewLanForm.SecondDns.disabled = true;
}	

function manualDNSRelay()
{
	document.uiViewLanForm.PrimaryDns.disabled = false;
	document.uiViewLanForm.SecondDns.disabled = false;
}

function dhcpRelayCheckFail()
{
	document.uiViewLanForm.dhcpTypeRadio[2].disabled = true;
	document.uiViewLanForm.ServerIp.disabled = true;
}

function disableTheAliasIp()
{
	if(document.uiViewLanForm.dhcpTypeRadio[3].checked)
	{
		document.uiViewLanForm.uiViewAliasIPAddr.disabled = true;
		document.uiViewLanForm.uiViewAliasNetMask.disabled = true;
	}
	else
	{
		document.uiViewLanForm.uiViewAliasIPAddr.disabled = false;
		document.uiViewLanForm.uiViewAliasNetMask.disabled = false;
	}
}
function userModeOnloadCheck()
{
	return;
}

function adminOnloadCheck()
{
<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
	disableTheAliasIp();
<% End If %>
	if(document.uiViewLanForm.dhcpTypeRadio[1].checked)
	{
		if(document.uiViewLanForm.dnsTypeRadio[0].checked)	
			autoDNSRelay();
		else if(document.uiViewLanForm.dnsTypeRadio[1].checked)
			manualDNSRelay();
	}
	if(document.uiViewLanForm.dhcpTypeRadio[2].checked)
	{
		if(!dhcpRelayCheck())
		{
			dhcpRelayCheckFail();
		}
	}
}
function onloadCheck()
{
	userModeOnloadCheck();
	if(document.uiViewLanForm.userMode.value !=1)
	{
		adminOnloadCheck();
	}
	return;

}

function dhcpRelayCheck()
{
	//alert("run here 1");
	if(document.uiViewLanForm.dhcpTypeRadio[2].checked){
		//alert("run here 2");
		if(document.uiViewLanForm.defaultRoute_isp.value == 3){
			alert("DHCP Relay may be no use if default route is not dynamic or static routing mode!!");
			return false;
		}
	}
	return true;
}
function radvdChanged()
{	
		//var form = document.uiViewLanForm;
		//form.uiViewIPv6PrefixRadvd.disabled = true;
		//form.uiViewIPv6PrefixLenRadvd.disabled = true;
		//form.uiPreferredLifetimeRadvd.disabled = true;
		//form.uiValidLifetimeRadvd.disabled = true;
		//form.radvdEnableFlag.value = 0;
		//form.radvdFlag.value = 1;
		//document.uiViewLanForm.submit();


	with (document.uiViewLanForm){
		radvdFlag.value = 1;
		if (radvdRadio[0].checked == true){
			setDisplay('div_radvden', 0);
			radvdEnableFlag.value = 0;
		}
		else {
			setDisplay('div_radvden', 1);
			radvdEnableFlag.value = 1;
			radvdModeChanged();
		}
	}
}

function radvdModeChanged( )
{
		//var  form = document.uiViewLanForm;
		//form.uiViewIPv6PrefixRadvd.disabled = false;
		//form.uiViewIPv6PrefixLenRadvd.disabled = false;
		//form.uiPreferredLifetimeRadvd.disabled = false;
		//form.uiValidLifetimeRadvd.disabled = false;
		//form.radvdEnableFlag.value = 1;		
		//form.radvdFlag.value = 1;
		//document.uiViewLanForm.submit();

		with (document.uiViewLanForm){
		radvdFlag.value = 1;
		if (radvdModeRadio[0].checked){
			radvdModeFlag.value = 0;
			setDisplay('div_radvdprelen', 0);
			setDisplay('div_radvdprelite', 0);
			setDisplay('div_radvdvate', 0);
		}
		else {
			radvdModeFlag.value = 1;
			setDisplay('div_radvdprelen', 1);
			setDisplay('div_radvdprelite', 1);
			setDisplay('div_radvdvate', 1);
		}
	}
}

function dhcp6sChanged()
{
		//var form = document.uiViewLanForm;
		//form.uiViewIPv6PrefixRadvd.disabled = true;
		//form.uiViewIPv6PrefixLenRadvd.disabled = true;
		//form.uiPreferredLifetimeRadvd.disabled = true;
		//form.uiValidLifetimeRadvd.disabled = true;		
		//form.dhcp6sEnableFlag.value = 0;
		//form.dhcp6sFlag.value = 1;
		//document.uiViewLanForm.submit();
		
	with (document.uiViewLanForm){
		dhcp6sFlag.value = 1;
		if (dhcp6sEnableRadio[0].checked == true){
			setDisplay('div_dhcp6sen', 0);
			dhcp6sEnableFlag.value = 0;
		}
		else {
			setDisplay('div_dhcp6sen', 1);
			dhcp6sEnableFlag.value = 1;
			dhcp6sModeChanged();
		}
	}
}

function ripngEnableChanged() 
{
	if(document.uiViewLanForm.ripngEnableRadio[0].checked)
		setDisplay('div_ripng_direction', 0);
	else
		setDisplay('div_ripng_direction', 1);
}

function dhcp6sModeChanged()
{
	//var form = document.uiViewLanForm;	
	//form.dhcp6sModeFlag.value = 0;		
	//form.dhcp6sFlag.value = 1;
	//document.uiViewLanForm.submit();
	
	with (document.uiViewLanForm){
		dhcp6sFlag.value = 1;
		if (dhcp6sModeRadio[0].checked){
			dhcp6sModeFlag.value = 0;
			setDisplay('div_dhcp6sprelen', 0);
			setDisplay('div_dhcp6splite', 0);
			setDisplay('div_dhcp6svate', 0);
			setDisplay('div_dhcp6sdns1', 0);
			setDisplay('div_dhcp6sdns2', 0);
		}
		else {
			dhcp6sModeFlag.value = 1;
			setDisplay('div_dhcp6sprelen', 1);
			setDisplay('div_dhcp6splite', 1);
			setDisplay('div_dhcp6svate', 1);
			setDisplay('div_dhcp6sdns1', 1);
			setDisplay('div_dhcp6sdns2', 1);
		}
	}
}

function dhcp6sModeAuto()
{
	var form = document.uiViewLanForm;
	
	form.dhcp6sModeFlag.value = 2;
		
	form.dhcp6sFlag.value = 1;
	document.uiViewLanForm.submit();
}

function dhcp6sPDEnable()
{
	var form = document.uiViewLanForm;
	form.dhcp6sPDFlag.value = 1;
	form.dhcp6sFlag.value = 1;
	document.uiViewLanForm.submit();
}

function dhcp6sPDDisable()
{
	var form = document.uiViewLanForm;
	form.dhcp6sPDFlag.value = 0;
	form.dhcp6sFlag.value = 1;
	document.uiViewLanForm.submit();
}

<% if tcWebApi_get("GUI_Entry0","DyVlan","h") = "support" then %>

function doIndexOfOUIChange(index)
{
	if(document.uiViewLanForm.OUIDeviceSeleradio[0].checked)
	{
		if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 0)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.stb_index_0.value;
		}
		else if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 1)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.stb_index_1.value;
		}
		else if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 2)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.stb_index_2.value;
		}
		else if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 3)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.stb_index_3.value;
		}
		else
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.stb_index_4.value;
		}
	}
	else
	{
		if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 0)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.voip_index_0.value;
		}
		else if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 1)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.voip_index_1.value;
		}
		else if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 2)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.voip_index_2.value;
		}
		else if(document.uiViewLanForm.IndexOfOUI.selectedIndex == 3)
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.voip_index_3.value;
		}
		else
		{
			document.uiViewLanForm.uiOUI.value = document.uiViewLanForm.voip_index_4.value;
		}
	}
	
}

function uiOUICheck(OUI)
{
	var OUIname = OUI.value;
	var i;
	if( (OUIname.length != 6) && (OUIname.length != 0))
	{
		alert("the length must be 6!!!");
		document.uiViewLanForm.uiOUI.focus();
		return true;
	}
	
	for ( var i = 0; i < OUIname.length; i++ )
	{
		var c = OUIname.charCodeAt(i);
		if(!(
		((c >= 48) && (c <= 57))
		||((c >= 65) && (c <= 70))
		||((c >= 97) && (c <= 102))
		))
		{
			alert("Format error, please enter 0-9,a-f,A-F!!");
			document.uiViewLanForm.uiOUI.focus();
			return true;
		}
	}
	return false;
}
<% End If %>

</script>
</head><body onLoad="onloadCheck()">



<FORM METHOD="POST" ACTION="/cgi-bin/home_lan.asp" name="uiViewLanForm">
<INPUT TYPE="HIDDEN" NAME="lan_VC" value="0">
<INPUT TYPE="HIDDEN" NAME="lan_Alias_VC" value="0">
<INPUT TYPE="HIDDEN" NAME="aliasFlag" value="<%tcWebApi_get("LanAlias_Entry","Active","s")%>">
<INPUT TYPE="HIDDEN" NAME="defaultRoute" value="<%tcWebApi_CurrentDefaultRoute()%>">
<%tcWebApi_get("WebCurSet_Entry","wan_pvc","defaultRoute")%>
<INPUT TYPE="HIDDEN" NAME="defaultRoute_isp" value="<%tcWebApi_get("Wan_PVC","ISP","s")%>">
<INPUT type="HIDDEN" name="staticNum" value="<%TcWebApi_get("Dhcpd","Static_Num","s")%>">
<INPUT type="HIDDEN" name="LeaseNum" value="<%TcWebApi_get("DhcpLease","LeaseNum","s")%>">
<INPUT type="HIDDEN" name="emptyEntry" value="<%TcWebApi_get("Dhcpd","Empty_Entry","s")%>">
<INPUT TYPE="HIDDEN" NAME="addFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="delnum">
<INPUT TYPE="HIDDEN" NAME="tmpStartIp" value="<%tcWebApi_get("Dhcpd_Common","start","s")%>">
<INPUT type="HIDDEN" NAME="tmpPoolCount" value="<%tcWebApi_get("Dhcpd_Common","pool_count","s")%>">
<INPUT TYPE="HIDDEN" NAME="option60Flag" value="<%tcWebApi_get("Dhcpd_Option60","Active","s")%>">
<INPUT type="HIDDEN" NAME="isIPv6Supported" value="<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT type="HIDDEN" NAME="ipv6Flag" value="<% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv4/IPv6" then asp_write("1") else asp_write("0") end if %>">
<INPUT type="hidden" name="userMode" value="<%if tcwebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then asp_write("0") else asp_write("1") end if%>">
<% if tcWebApi_get("GUI_Entry0","DyVlan","h") = "support" then %>
<INPUT type="HIDDEN" NAME="stb_index_0" value="<%tcWebApi_staticGet("DyVLAN_Entry0","STB_OUI","s") %>">
<INPUT type="HIDDEN" NAME="stb_index_1" value="<%tcWebApi_staticGet("DyVLAN_Entry1","STB_OUI","s") %>">
<INPUT type="HIDDEN" NAME="stb_index_2" value="<%tcWebApi_staticGet("DyVLAN_Entry2","STB_OUI","s") %>">
<INPUT type="HIDDEN" NAME="stb_index_3" value="<%tcWebApi_staticGet("DyVLAN_Entry3","STB_OUI","s") %>">
<INPUT type="HIDDEN" NAME="stb_index_4" value="<%tcWebApi_staticGet("DyVLAN_Entry4","STB_OUI","s") %>">
<INPUT type="HIDDEN" NAME="voip_index_0" value="<%tcWebApi_staticGet("DyVLAN_Entry0","Voip_OUI","s") %>">
<INPUT type="HIDDEN" NAME="voip_index_1" value="<%tcWebApi_staticGet("DyVLAN_Entry1","Voip_OUI","s") %>">
<INPUT type="HIDDEN" NAME="voip_index_2" value="<%tcWebApi_staticGet("DyVLAN_Entry2","Voip_OUI","s") %>">
<INPUT type="HIDDEN" NAME="voip_index_3" value="<%tcWebApi_staticGet("DyVLAN_Entry3","Voip_OUI","s") %>">
<INPUT type="HIDDEN" NAME="voip_index_4" value="<%tcWebApi_staticGet("DyVLAN_Entry4","Voip_OUI","s") %>">
<% end if %>

<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
      <td width="150" height="30" class="title-main">
      <font color="#FFFFFF">Router Local IP</font>      </td><td width="10" class="black"></td><td width="150"></td><td width="10" ></td><td width="440"></td></tr>
     <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">IP Address</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="uiViewIPAddr" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Lan_Entry","IP","h") <> "N/A" then tcWebApi_get("Lan_Entry","IP","s") end if%>" >	
        <INPUT TYPE="HIDDEN" NAME="dhcpFlag" VALUE="1"><INPUT TYPE="HIDDEN" NAME="lanFlag" VALUE="0"><INPUT TYPE="HIDDEN" NAME="DNSproxy" VALUE='Yes'>       </td>
     </tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"> <div align=right>
    <font color="#000000">IP Subnet Mask</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
	<INPUT TYPE="TEXT" NAME="uiViewNetMask" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Lan_Entry","netmask","h") <> "N/A" then tcWebApi_get("Lan_Entry","netmask","s") end if%>" >	
	</td></tr>
<% If tcWebApi_get("LanAlias_Entry","Active","h") = "Yes" Then%>
    <tr>
    <td height="24" class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Alias IP Address</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="uiViewAliasIPAddr" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("LanAlias_Entry","IP","h") <> "N/A" then tcWebApi_get("LanAlias_Entry","IP","s") end if%>" >
		&nbsp;(0.0.0.0 means to close the alias ip)	
     </tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"> <div align=right>
    <font color="#000000">Alias IP Subnet Mask</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
	<INPUT TYPE="TEXT" NAME="uiViewAliasNetMask" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("LanAlias_Entry","netmask","h") <> "N/A" then tcWebApi_get("LanAlias_Entry","netmask","s") end if%>" >
	</td></tr>
<% End If %>	
<%if tcwebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">IgmpSnoop</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT NAME="lan_Igmpsnoop" TYPE="RADIO" VALUE="Yes" <% If TCWebApi_get("Lan_IgmpSnoop","igmpsnoopEnable","h") = "Yes" then asp_Write("checked") end if%> >		<font color="#000000">Activated</font>		
		<INPUT TYPE="RADIO" NAME="lan_Igmpsnoop" VALUE="No" <% If TCWebApi_get("Lan_IgmpSnoop","igmpsnoopEnable","h") = "No" then asp_Write("checked") end if%> >		<font color="#000000">Deactivated</font>	</td></tr>
<%end if%>
<%if tcwebApi_get("WebCustom_Entry","isMLDSupported","h") = "Yes" then%>
	<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">MldSnoop</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT TYPE="RADIO" NAME="lan_Mldsnoop" VALUE="Yes" <% If TCWebApi_get("Lan_MldSnoop","mldsnoopEnable","h") = "Yes" then asp_Write("checked") end if%> >		<font color="#000000">Activated</font>		
		<INPUT TYPE="RADIO" NAME="lan_Mldsnoop" VALUE="No" <% If TCWebApi_get("Lan_MldSnoop","mldsnoopEnable","h") = "No" then asp_Write("checked") end if%> >		<font color="#000000">Deactivated</font>	</td></tr>
<%end if%>	
  <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">Dynamic Route</font></div></td><td width="10" class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <SELECT NAME="lan_RIP" SIZE="1">
		<OPTION <% if tcWebApi_get("LanRipd_Entry","RIPVERSION","h") = "RIP1" then asp_Write("selected") end if %>>RIP1
		<OPTION <% if tcWebApi_get("LanRipd_Entry","RIPVERSION","h") = "RIP2" then asp_Write("selected") end if %>>RIP2
		</SELECT><font color="#000000">Direction</font>
		<SELECT NAME="lan_RIP_Dir" SIZE="1">
		<OPTION <% if tcWebApi_get("LanRipd_Entry","DIRECTION","h") = "None" then asp_Write("selected") end if %>>None
		<OPTION <% if tcWebApi_get("LanRipd_Entry","DIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
		<OPTION <% if tcWebApi_get("LanRipd_Entry","DIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
		<OPTION <% if tcWebApi_get("LanRipd_Entry","DIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>OUT Only
		</SELECT></td></tr>

 	<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then %>
	<% If tcWebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" Then %>
        <tr>
		<td width="150" height="30" class="light-orange">&nbsp;</td>
                <td width="10" class="light-orange">&nbsp;</td>
                <td width="150" class="tabdata"><div align=right> <font color="#000000">Ripng</font></div></td>
                <td width="10" class="tabdata"><div align=center>:</div></td>
                <td width="440" class="tabdata"> 
				<INPUT TYPE="RADIO" NAME="ripngEnableRadio" VALUE="0"  onClick="ripngEnableChanged()" <%If tcWebApi_get("LanRipd_Entry","RIPNGENABLE","h") <> "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Disable</font> 
				<INPUT TYPE="RADIO" NAME="ripngEnableRadio" VALUE="1"  onClick="ripngEnableChanged()" <%If tcWebApi_get("LanRipd_Entry","RIPNGENABLE","h") = "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Enable</font>  </td>
      </tr>
      <tr id="div_ripng_direction">
      	<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
  		<td class="tabdata"><div align=right><font color="#000000">Ripng Direction</font></div></td><td width="10" class="tabdata"><div align=center>:</div></td><td class="tabdata">
			<SELECT NAME="lan_Ripng_Dir" SIZE="1">
			<OPTION <% if tcWebApi_get("LanRipd_Entry","RIPNGDIRECTION","h") = "None" then asp_Write("selected") end if %>>None
			<OPTION <% if tcWebApi_get("LanRipd_Entry","RIPNGDIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
			<OPTION <% if tcWebApi_get("LanRipd_Entry","RIPNGDIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
			<OPTION <% if tcWebApi_get("LanRipd_Entry","RIPNGDIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>OUT Only
			</SELECT>
		</td>
      </tr>	  
	<script language="JavaScript" type="text/JavaScript">
		ripngEnableChanged();
	</script>
	<% end if %>
	<% end if %>
	
        </table>
		<%if tcwebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
		<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">DHCP Server </font>    </td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width= "10"><hr noshade></td><td width="440"><hr noshade> </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">DHCP</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="RADIO" NAME="dhcpTypeRadio" VALUE="0" <%If tcWebApi_get("Lan_Dhcp","type","h") = "0" then asp_Write("checked") elseIf tcWebApi_get("Lan_Dhcp","type","h") = "N/A" then asp_Write("checked") end if%> onClick="doReload()"><font color="#000000">Disabled</font>
        <INPUT TYPE="RADIO" NAME="dhcpTypeRadio" VALUE="1" <%If tcWebApi_get("Lan_Dhcp","type","h") = "1" then asp_Write("checked") end if%> onClick="doReload()"><font color="#000000">Enabled</font>
        <INPUT TYPE="RADIO" NAME="dhcpTypeRadio" VALUE="2" <%If tcWebApi_get("Lan_Dhcp","type","h") = "2" then asp_Write("checked") end if%> onClick="doReload()"><font color="#000000">Relay</font>
<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>		
		<INPUT TYPE="RADIO" NAME="dhcpTypeRadio" VALUE="3" <%If tcWebApi_get("Lan_Dhcp","type","h") = "3" then asp_Write("checked") end if%> onClick="doReload()"><font color="#000000">Client</font>
<% End If %>
    </td>
  </tr>
</table>
<%end if%>
<%if tcwebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
<% If tcWebApi_get("GUI_Entry0","dhcpClientMode","h") = "support" Then%>
<div id="dhcp_client_div">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td class="light-orange" width="150">&nbsp;</td><td class="light-orange" width="10">&nbsp;</td>
		<td class="tabdata" width="150">
			<div align=right>
			<font color="#000000">IP Address</font>
			</div>
		</td>
		<td class="tabdata" width="10"><div align=center>:</div></td>
		<td class="tabdata">
			<%If tcWebApi_get("LanAlias_Entry","IP","h") <> "N/A" then tcWebApi_get("LanAlias_Entry","IP","s") else asp_Write("0.0.0.0") end if%>
		</td>
	</tr>
	</table>
</div>
<% End If %>

<div id="dhcp_enabled_div0">
		<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr><td class="light-orange" width="150">&nbsp;</td><td class="light-orange" width="10">&nbsp;</td><td class="tabdata" width="150"><div align=right>
     <font color="#000000">Start IP</font>    </div></td><td class="tabdata" width="10"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="StartIp" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Dhcpd_Common","start","h") <>"N/A" then tcWebApi_get("Dhcpd_Common","start","s") end if%>" >	        
     </td></tr>
     
		<tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">IP Pool Count</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="PoolSize" SIZE="15" MAXLENGTH="3" VALUE="<%If tcWebApi_get("Dhcpd_Common","pool_count","h") <>"N/A" then tcWebApi_get("Dhcpd_Common","pool_count","s") end if%>" >	        
     </td></tr>
    <% If tcWebApi_get("Dhcpd_Option60","Active","h") = "Yes" Then%>
    		<tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Conditional Start IP</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="ConPoolStartIp" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Dhcpd_Option60","start","h") <>"N/A" then tcWebApi_get("Dhcpd_Option60","start","s") end if%>" >	        
     </td></tr>
     		<tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Conditional Pool Count</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="ConPoolPoolSize" SIZE="15" MAXLENGTH="3" VALUE="<%If tcWebApi_get("Dhcpd_Option60","pool_count","h") <>"N/A" then tcWebApi_get("Dhcpd_Option60","pool_count","s") end if%>" >	        
     </td></tr>
         	<tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Vendor Class ID</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="VendorClassID" SIZE="15" MAXLENGTH="64" VALUE="<%If tcWebApi_get("Dhcpd_Option60","VendorID","h") <>"N/A" then tcWebApi_get("Dhcpd_Option60","VendorID","s") end if%>" >	        
     </td></tr>
    <% End If %>	    

    <tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Lease Time</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="dhcp_LeaseTime" SIZE="6" MAXLENGTH="6" VALUE="<%If tcWebApi_get("Dhcpd_Common","lease","h") <>"N/A" then tcWebApi_get("Dhcpd_Common","lease","s") else asp_Write("0") end if%>" >
        seconds&nbsp;&nbsp;&nbsp;(0 sets to default value of 259200)	        
     </td></tr>
 <%end if%>
	 <% if tcWebApi_get("WebCustom_Entry","isPortFltSupported","h") = "Yes" then %>
	 <tr>
			<td width="150" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td width="150" class="tabdata" align=right><font color="#000000">Physical Ports</font></td>
			<td class="tabdata" align=center>:</td>
			<td width="440" height="39" class="tabdata">
				<table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
					<tr>
						<% if tcWebApi_get("WebCustom_Entry","is1PSupported","h") = "Yes" then %>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortEth0" <%if TCWebAPI_get("Dhcpd_Common", "eth0", "h") = "Yes" then asp_write("checked") end if %>></td>
						<%else%>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortEth0" <%if TCWebAPI_get("Dhcpd_Common", "eth0.1", "h") = "Yes" then asp_write("checked") end if %>></td>

						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortEth1" <%if TCWebAPI_get("Dhcpd_Common", "eth0.2", "h") = "Yes" then asp_write("checked") end if %>></td>
						<%end if%>
						<% if tcWebApi_get("WebCustom_Entry","is2PSupported","h") <> "Yes" then %>
						<% if tcWebApi_get("WebCustom_Entry","is1PSupported","h") <> "Yes" then %>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortEth2" <%if TCWebAPI_get("Dhcpd_Common", "eth0.3", "h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortEth3" <%if TCWebAPI_get("Dhcpd_Common", "eth0.4", "h") = "Yes" then asp_write("checked") end if %>></td>
						<%end if%>
						<%end if%>
<!--
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortUsb0" <%if TCWebAPI_get("Dhcpd_Common", "usb0", "h") = "Yes" then asp_write("checked") end if %>></td>
-->
<% if tcWebApi_get("Info_WLan","isExist","h") = "On" then %>
<%
	if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then
%>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortRa0" <%if TCWebAPI_get("Dhcpd_Common", "ra0", "h") = "Yes" then asp_write("checked") end if %>></td>			
<%
	end if
%>
<% end if %>
					</tr>
						<tr class="tabdata">
						<td class="tabdata" align=center><font color="#000000">eth0</font></td>
						<td class="tabdata" align=center><font color="#000000">eth1</font></td>
						<% if tcWebApi_get("WebCustom_Entry","is2PSupported","h") <> "Yes" then %>
						<td class="tabdata" align=center><font color="#000000">eth2</font></td>
						<td class="tabdata" align=center><font color="#000000">eth3</font></td>
						<% end if %>
<!--
						<td class="tabdata" align=center><font color="#000000">usb0</font></td>
-->
<% if tcWebApi_get("Info_WLan","isExist","h") = "On" then %>
<%
	if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then
%>
						<td class="tabdata" align=center><font color="#000000">ra0</font></td>
<%
	end if
%>				
					</tr>
<%
	if TCWebAPI_get("WLan_Common","BssidNum","h") = "2" then
%>
					<tr>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid0" <%if TCWebAPI_get("Dhcpd_Common","ra0","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid1" <%if TCWebAPI_get("Dhcpd_Common","ra1","h") = "Yes" then asp_write("checked") end if %>></td>
					</tr>
					<tr class="tabdata">
					<td class="tabdata" align=center><font color="#000000">ra0</font></td>
					<td class="tabdata" align=center><font color="#000000">ra1</font></td>
					</tr>
<%
	elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "3" then
%>
					<tr>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid0" <%if TCWebAPI_get("Dhcpd_Common","ra0","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid1" <%if TCWebAPI_get("Dhcpd_Common","ra1","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid2" <%if TCWebAPI_get("Dhcpd_Common","ra2","h") = "Yes" then asp_write("checked") end if %>></td>	
					</tr>
					<tr class="tabdata">
					<td class="tabdata" align=center><font color="#000000">ra0</font></td>
					<td class="tabdata" align=center><font color="#000000">ra1</font></td>
					<td class="tabdata" align=center><font color="#000000">ra2</font></td>
					</tr>
<%
	elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "4" then
%>
					<tr>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid0" <%if TCWebAPI_get("Dhcpd_Common","ra0","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid1" <%if TCWebAPI_get("Dhcpd_Common","ra1","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid2" <%if TCWebAPI_get("Dhcpd_Common","ra2","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="DHCPPhyPortWLANMssid3" <%if TCWebAPI_get("Dhcpd_Common","ra3","h") = "Yes" then asp_write("checked") end if %>></td>
					</tr>
					<tr class="tabdata">
					<td class="tabdata" align=center><font color="#000000">ra0</font></td>
					<td class="tabdata" align=center><font color="#000000">ra1</font></td>
					<td class="tabdata" align=center><font color="#000000">ra2</font></td>
					<td class="tabdata" align=center><font color="#000000">ra3</font></td>
					</tr>

<%
	end if
%>

<%
	end if
%>
				</table>
			</td>
		</tr>
	<%end if%>
<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
     <tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">DNS</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width= "10"><hr noshade></td><td width="440"><hr noshade> </td></tr>
    <tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">DNS Relay</font>   </div></td><td class="tabdata"><div align=center></div></td><td class="tabdata">
	    <INPUT TYPE="RADIO" NAME="dnsTypeRadio" VALUE="0" onClick="autoDNSRelay()" <%If tcWebApi_get("Dproxy_Entry","type","h") = "0" then asp_Write("checked") elseif  tcWebApi_get("Dproxy_Entry","type","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">Automatically</font>
	    <INPUT TYPE="RADIO" NAME="dnsTypeRadio" VALUE="1" onClick="manualDNSRelay()" <%If tcWebApi_get("Dproxy_Entry","type","h") = "1" then asp_Write("checked") end if%> ><font color="#000000">Manually</font>
     </td></tr>
     
    <tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Primary DNS</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="PrimaryDns" SIZE="15" MAXLENGTH="15" VALUE="<% If tcWebApi_get("Dproxy_Entry","type","h") = "1" then tcWebApi_get("Dproxy_Entry","Primary_DNS","s") else asp_Write("N/A") end if %>" >	        
     </td></tr>

    <tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Secondary DNS</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="SecondDns" SIZE="15" MAXLENGTH="15" VALUE="<% If tcWebApi_get("Dproxy_Entry","type","h") = "1" then tcWebApi_get("Dproxy_Entry","Secondary_DNS","s") else asp_Write("N/A") end if %>" >	        
     </td></tr>
	 

	<%if tcwebApi_get("WebCustom_Entry","isDhcpStaticSupported","h")="Yes" then%>	 	 
	 	<tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">Add DHCP Reservation</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width= "10"><hr noshade></td><td width="440"><hr noshade> </td></tr>
	<tr><td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right> 
            <font color="#000000">IP Address</font> </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="IpAddr" SIZE="15" MAXLENGTH="15" VALUE="" >
          <font color="#000000"> MAC Address</font><font color="#000000"> : </font>
<INPUT TYPE="TEXT" NAME="MACAddr" SIZE="15" MAXLENGTH="17" VALUE="" >	        
     </td></tr>
	  <tr>
    <td width="150" class="light-orange">&nbsp;</td><td width="10"  class="light-orange"></td><td colspan="3" align=center>
        <!--
        <iframe src="/cgi-bin/dhcp_static_list.cgi" id="static_list" frameborder="0" width="600" height="180" align=left></iframe>
       -->
       <table width="400" border="1" align=center cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF" id="static_list">
	<tr height="30">
		      <td width="50" align=center class="tabdata"><strong><font color="#000000">Index</font></strong></td>
		      <td width="150" align=center class="tabdata"><strong><font color="#000000">IP</font></strong></td>
		      <td width="150" align=center class="tabdata"><strong><font color="#000000">MAC</font></strong></td>
		      <td width="50" align=center class="tabdata"><strong><font color="#000000">Drop</font></strong></td>
	</tr>
       <%if tcwebApi_get("Dhcpd_Entry0","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">1</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry0","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry0","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(0);"></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry1","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">2</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry1","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry1","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(1);"></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry2","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">3</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry2","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry2","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(2);"></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry3","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">4</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry3","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry3","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(3);"></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry4","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">5</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry4","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry4","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(4);"></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry5","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">6</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry5","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry5","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(5);"></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry6","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">7</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry6","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry6","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(6);"></td>
	</tr>
	<%end if%>
	<%if tcwebApi_get("Dhcpd_Entry7","IP","h")<>"N/A" then%>
	<tr>
	<td width="50" align=center class="tabdata">8</td>
	 <td width="150" align=center class="tabdata"> <% tcwebApi_staticGet("Dhcpd_Entry7","IP","s") %> </td>
	<td width="150" align=center class="tabdata"><% tcwebApi_staticGet("Dhcpd_Entry7","MAC","s") %></td>
	<td width="50" align=center class="tabdata"><IMG src="/cross.gif" onmouseover="this.style.cursor='hand'" onClick="doDelete(7);"></td>
	</tr>
	<%end if%> 
</table>
        </td></tr>		
<%End if%>		
	 <tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">DHCP Clients List</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width= "10"><hr noshade></td><td width="440"><hr noshade> </td></tr>	 
	 <tr>
    <td width="150" class="light-orange">&nbsp;</td><td width="10"  class="light-orange"></td><td colspan="3" align=center>
 <!--
        <iframe src="/cgi-bin/dhcp_client_list.cgi" id="client_list" frameborder="0" width="600" height="180" align=left></iframe>
        -->
		<div id=dhcpclientList></div>
<script language=JavaScript>
var tableHeader = [
	["50","#"],
	["140","HostName"],
	["150","IP"],
	["140","MAC Address"],
	["100","Expire Time"]
];
var tableData = [
	["1", "<%tcWebApi_Get("DhcpLease_Entry0", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry0", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry0", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry0", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry0", "ExpireTime","s")%>"],
	["2", "<%tcWebApi_staticGet("DhcpLease_Entry1", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry1", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry1", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry1", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry1", "ExpireTime","s")%>"],
	["3", "<%tcWebApi_staticGet("DhcpLease_Entry2", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry2", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry2", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry2", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry2", "ExpireTime","s")%>"],
	["4", "<%tcWebApi_staticGet("DhcpLease_Entry3", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry3", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry3", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry3", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry3", "ExpireTime","s")%>"],
	["5", "<%tcWebApi_staticGet("DhcpLease_Entry4", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry4", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry4", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry4", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry4", "ExpireTime","s")%>"],
	["6", "<%tcWebApi_staticGet("DhcpLease_Entry5", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry5", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry5", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry5", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry5", "ExpireTime","s")%>"],
	["7", "<%tcWebApi_staticGet("DhcpLease_Entry6", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry6", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry6", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry6", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry6", "ExpireTime","s")%>"],
	["8", "<%tcWebApi_staticGet("DhcpLease_Entry7", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry7", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry7", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry7", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry7", "ExpireTime","s")%>"],
	["9", "<%tcWebApi_staticGet("DhcpLease_Entry8", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry8", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry8", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry8", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry8", "ExpireTime","s")%>"],
	["10", "<%tcWebApi_staticGet("DhcpLease_Entry9", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry9", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry9", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry9", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry9", "ExpireTime","s")%>"]
];

		showTable('dhcpclientList',tableHeader,tableData,2);
		</script>
        </td></tr>
		</table>
</div>
	
     
<div id="dhcp_relay_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr><td class="light-orange" width=150>&nbsp;</td><td class="light-orange" width=10>&nbsp;</td><td class="tabdata" width=150><div align=right>
    <font color="#000000">Server IP</font>    </div></td><td class="tabdata" width="10"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="ServerIp" SIZE="15" MAXLENGTH="15" VALUE=<%If tcWebApi_get("DhcpRelay_Entry","Server","h") <>"N/A" then tcWebApi_get("DhcpRelay_Entry","Server","s") end if%> >	        
    </td></tr>

     </table>
</div>
  <%end if%>  
     
<% if tcWebApi_get("GUI_Entry0","DyVlan","h") = "support" then %>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr> 
		<td width="150" height="30" class="title-main"> <font color="#FFFFFF">OUI</font></td>
		<td width="10" class="black"></td>
		<td width="150"><hr noshade></td>
		<td width="12" ><hr noshade></td>
		<td width="438"><hr noshade></td>
	</tr>
	<tr>
		<td class=light-orange width=150></td>
		<td class=light-orange width=10>&nbsp;</td>
		<td class=tabdata width=150><DIV align=right>OUI Device</DIV></td>
		<td class=tabdata><DIV align=center>:</DIV></td>
		<td class=tabdata>
			<INPUT  type=radio <% If tcWebApi_get("DyVLAN_Common" , "Type" , "h") = "STB" then asp_Write("checked") end if %> value=STB name=OUIDeviceSeleradio>STB
			<INPUT  type=radio <% If tcWebApi_staticGet("DyVLAN_Common" , "Type" , "h") = "Voip" then asp_Write("checked") end if %> value=Voip name=OUIDeviceSeleradio>VoIP
			<INPUT type=hidden value=0 name=ouiFlag>
		</td>
	</tr>
	<tr>
		<td class=light-orange width=150>&nbsp;</td>
		<td class=light-orange width=10></td>
		<td class=tabdata width=150><DIV align=right>Index</DIV></td>
		<td class=tabdata><DIV align=center>:</DIV></td>
		<td class=tabdata>
			<SELECT onchange=doIndexOfOUIChange(this.selectedIndex) size=1 name=IndexOfOUI>
				<OPTION value="0" <% if tcWebApi_get("WebCurSet_Entry","DyVlan_Index","h") = "0" then asp_Write("selected") end if %>>1
				<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","DyVlan_Index","h") = "1" then asp_Write("selected") end if %>>2
				<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","DyVlan_Index","h") = "2" then asp_Write("selected") end if %>>3
				<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","DyVlan_Index","h") = "3" then asp_Write("selected") end if %>>4
				<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","DyVlan_Index","h") = "4" then asp_Write("selected") end if %>>5
				</OPTION>
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class=light-orange>&nbsp;</td>
		<td class=light-orange></td>
		<td class=tabdata><DIV id=OUIId align=right>OUI</DIV></td>
		<td class=tabdata><DIV align=center>:</DIV></td>
		<td class=tabdata>
		<% if tcWebApi_get("DyVLAN_Common","Type","h") = "STB" then %>
		<INPUT maxLength=6 size=6 value="<% tcWebApi_staticGet("DyVLAN_Entry","STB_OUI","s") %>" name=uiOUI>
		<% End If %>
		<% if tcWebApi_staticGet("DyVLAN_Common","Type","h") = "Voip" then %>
		<INPUT maxLength=6 size=6 value="<% tcWebApi_staticGet("DyVLAN_Entry","Voip_OUI","s") %>" name=uiOUI>
		<% End If %>
		</td>
	</tr> 
	</table>
<% End If %>  

 	<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then %>
	
	

	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
        <td width="150" height="30" class="title-main"> <font color="#FFFFFF">IPv6 Address</font> 
        </td>
        <td width="10" class="black"></td>
        <td width="150"><hr noshade></td>
        <td width="12" ><hr noshade></td>
        <td width="438"><hr noshade></td>
      </tr>
	  
	  <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">IPv6 Global Address</font></div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="uiViewIPv6Addr" SIZE="39" MAXLENGTH="39" VALUE="<%If tcWebApi_get("Lan_Entry","IP6","h") <> "N/A" then tcWebApi_get("Lan_Entry","IP6","s") end if%>">
          <font size=+1>&nbsp;/&nbsp;</font>
          <INPUT TYPE="TEXT" NAME="uiViewIPv6Prefix" SIZE="3" MAXLENGTH="3" VALUE="<%If tcWebApi_get("Lan_Entry","PREFIX6","h") <> "N/A" then tcWebApi_get("Lan_Entry","PREFIX6","s") end if%>"></td>
      </tr>
	  
	  <tr> 
        <td width="150" height="30" class="title-main"> <font color="#FFFFFF">Radvd</font> 
        </td>
        <td width="10" class="black"></td>
        <td width="150"><hr noshade></td>
        <td width="12" ><hr noshade></td>
        <td width="438"><hr noshade></td>
      </tr>
	     
	  
	  
     <tr>
        <td height="30" class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">Radvd Enable</font> 
          </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="RADIO" NAME="radvdRadio" VALUE="0" onClick="radvdChanged()" <%If tcWebApi_get("Radvd_Entry","Enable","h") = "0" then asp_Write("checked") elseif  tcWebApi_get("Radvd_Entry","Enable","h") = "N/A" then asp_Write("checked") end if%> > 
          <font color="#000000">Disable</font> <INPUT TYPE="RADIO" NAME="radvdRadio" VALUE="1" onClick="radvdChanged()" <%If tcWebApi_get("Radvd_Entry","Enable","h") = "1" then asp_Write("checked") end if%> > 
          <font color="#000000">Enable</font> </td>
		  <INPUT TYPE="HIDDEN" NAME="radvdEnableFlag" VALUE=<%  tcWebApi_get("Radvd_Entry","Enable","s") %> >
		  <INPUT TYPE="HIDDEN" NAME="radvdFlag"  >
     </tr>
	</table>
	<div id="div_radvden">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr id="div_radvdmode">
        <td width="150" height="30" class="light-orange">&nbsp;</td>
        <td width="10" class="light-orange">&nbsp;</td>
        <td width="150" class="tabdata"><div align=right> <font color="#000000">Radvd Mode</font></div></td>
        <td width="10" class="tabdata"><div align=center>:</div></td>
        <td width="440" class="tabdata"> <INPUT TYPE="RADIO" NAME="radvdModeRadio" VALUE="0" onClick="radvdModeChanged()" <%If tcWebApi_get("Radvd_Entry","Mode","h") = "0" then asp_Write("checked") elseif  tcWebApi_get("Radvd_Entry","Mode","h") = "N/A" then asp_Write("checked") end if%> >
          <font color="#000000">Auto</font>
          <INPUT TYPE="RADIO" NAME="radvdModeRadio" VALUE="1" onClick="radvdModeChanged()" <%If tcWebApi_get("Radvd_Entry","Mode","h") = "1" then asp_Write("checked") end if%> > 
          <font color="#000000">Manual</font> </td>
		  <INPUT TYPE="HIDDEN" NAME="radvdModeFlag" VALUE=<%  tcWebApi_get("Radvd_Entry","Mode","s") %> >
     </tr>
      <tr id="div_radvdprelen"> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">Prefix/Length</font></div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="uiViewIPv6PrefixRadvd" SIZE="39" MAXLENGTH="39" VALUE="<%If tcWebApi_get("Radvd_Entry","PrefixIPv6","h") <> "N/A" then tcWebApi_get("Radvd_Entry","PrefixIPv6","s") end if%>"> 
          <font size=+1>&nbsp;/&nbsp;</font> <INPUT TYPE="TEXT" NAME="uiViewIPv6PrefixLenRadvd" SIZE="3" MAXLENGTH="3" VALUE="<%If tcWebApi_get("Radvd_Entry","Prefixv6Len","h") <> "N/A" then tcWebApi_get("Radvd_Entry","Prefixv6Len","s") end if%>"></td>
      </tr>
      <tr id="div_radvdprelite"> 
        <td height="27" class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">Preferred 
            Lifetime</font> </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="uiPreferredLifetimeRadvd" SIZE="30" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Radvd_Entry","PreferredLifetime","h") <> "N/A" then tcWebApi_get("Radvd_Entry","PreferredLifetime","s") end if%>"> 
        </td>
      </tr>
      <tr id="div_radvdvate"> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">ValidLifetime</font> 
          </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <input type="TEXT" name="uiValidLifetimeRadvd" size="30" maxlength="15" value="<%If tcWebApi_get("Radvd_Entry","ValidLifetime","h") <> "N/A" then tcWebApi_get("Radvd_Entry","ValidLifetime","s") end if%>"></td>
      </tr>
<% if tcWebApi_get("WebCustom_Entry","isRaFlagSetSupported","h") = "Yes" then %>
	  	<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">RA Flags Set</font></div></td><td width="10" class="tabdata"><div align=center>:</div></td>
          <td class="tabdata"> ManagedAddr 
            <SELECT NAME="radvdManagedAddrflag" SIZE="1">
		<OPTION <% if tcWebApi_get("Radvd_Entry","M_Flag","h") = "off" then asp_Write("selected") end if %>>off
		<OPTION <% if tcWebApi_get("Radvd_Entry","M_Flag","h") = "on" then asp_Write("selected") end if %>>on
		</SELECT>
            <font color="#000000">OtherConfig</font> 
            <SELECT NAME="radvdOtherConfigflag" SIZE="1">
		<OPTION <% if tcWebApi_get("Radvd_Entry","O_Flag","h") = "off" then asp_Write("selected") end if %>>off
		<OPTION <% if tcWebApi_get("Radvd_Entry","O_Flag","h") = "on" then asp_Write("selected") end if %>>on
		</SELECT></td></tr>
<% End If %> 
    </table></div>

	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
        <td width="150" height="30" class="title-main"> <font color="#FFFFFF">DHCPv6</font> 
        </td>
        <td width="10" class="black"></td>
        <td width="150"><hr noshade></td>
        <td width="10" ><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr>
        <td height="30" class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">DHCP6 Server</font></div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="RADIO" NAME="dhcp6sEnableRadio" VALUE="0" onClick="dhcp6sChanged()" <%If tcWebApi_get("Dhcp6s_Entry","Enable","h") = "0" then asp_Write("checked") elseif  tcWebApi_get("Dhcp6s_Entry","Enable","h") = "N/A" then asp_Write("checked") end if%> > 
          <font color="#000000">Disable</font> <INPUT TYPE="RADIO" NAME="dhcp6sEnableRadio" VALUE="1" onClick="dhcp6sChanged()" <%If tcWebApi_get("Dhcp6s_Entry","Enable","h") = "1" then asp_Write("checked") end if%> > 
          <font color="#000000">Enable</font> </td>
		  <INPUT TYPE="HIDDEN" NAME="dhcp6sEnableFlag" VALUE=<% tcWebApi_get("Dhcp6s_Entry","Enable","s") %> >
		  <INPUT TYPE="HIDDEN" NAME="dhcp6sFlag"  >
		  
		  <INPUT TYPE="HIDDEN" NAME="dhcp6sModeFlag" VALUE=<% tcWebApi_get("Dhcp6s_Entry","Mode","s") %> >
		  <INPUT TYPE="HIDDEN" NAME="dhcp6sPDFlag" VALUE=<% tcWebApi_get("Dhcp6s_Entry","PDvar","s") %> >
		  <INPUT TYPE="HIDDEN" NAME="dhcp6sModeSubmitFlag"  >
      </tr></table>
	<div id="div_dhcp6sen">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
        <td width="150" height="30" class="light-orange">&nbsp;</td>
        <td width="10" class="light-orange">&nbsp;</td>
        <td width="150" class="tabdata"><div align=right> <font color="#000000">DHCP6 Mode</font></div></td>
        <td width="10" class="tabdata"><div align=center>:</div></td>
        <td width="440" class="tabdata"> <INPUT TYPE="RADIO" NAME="dhcp6sModeRadio" VALUE="0"  onClick="dhcp6sModeChanged()" <%If tcWebApi_get("Dhcp6s_Entry","Mode","h") = "0" then asp_Write("checked") elseif  tcWebApi_get("Dhcp6s_Entry","Mode","h") = "N/A" then asp_Write("checked") end if%> > 
          <font color="#000000">Auto</font> <INPUT TYPE="RADIO" NAME="dhcp6sModeRadio" VALUE="1"  onClick="dhcp6sModeChanged()" <%If tcWebApi_get("Dhcp6s_Entry","Mode","h") = "1" then asp_Write("checked") end if%> > 
          <font color="#000000">Manual</font>  </td>
      </tr>
      <tr id="div_dhcp6sprelen"> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">Prefix/Length</font></div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="uiViewIPv6DHCPPrefix" SIZE="39" MAXLENGTH="39" VALUE="<%If tcWebApi_get("Dhcp6s_Entry","PrefixIPv6","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","PrefixIPv6","s") end if%>">
          <font size=+1>&nbsp;/&nbsp;</font>
          <INPUT TYPE="TEXT" NAME="uiViewIPv6DHCPPrefixLen" SIZE="3" MAXLENGTH="3" VALUE="<%If tcWebApi_get("Dhcp6s_Entry","Prefixv6Len","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","Prefixv6Len","s") end if%>"></td>
      </tr>
      <tr id="div_dhcp6splite"> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">Preferred 
            Lifetime</font> </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="uiPreferredLifetimeDHCP6" SIZE="30" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Dhcp6s_Entry","PreferredLifetime","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","PreferredLifetime","s") end if%>"> 
        </td>
      </tr>
      <tr id="div_dhcp6svate"> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000">ValidLifetime</font> 
          </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="uiValidLifetimeDHCP6" SIZE="30" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Dhcp6s_Entry","ValidLifetime","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","ValidLifetime","s") end if%>"> 
        </td>
      </tr>
	  
	  
	  <tr id="div_dhcp6sdns1">
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right> 
            <font color="#000000">Primary DNS</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="uiPrimaryDNSDHCP6" SIZE="39" MAXLENGTH="39" VALUE="<%If tcWebApi_get("Dhcp6s_Entry","DNSserver","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","DNSserver","s") end if%>">
          <font size=+1>&nbsp;</font></td>
     </tr>
	 
	<tr id="div_dhcp6sdns2">
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right> 
            <font color="#000000">Secondary DNS</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="TEXT" NAME="uiSecondaryDNSDHCP6" SIZE="39" MAXLENGTH="39" VALUE="<%If tcWebApi_get("Dhcp6s_Entry","SecDNSserver","h") <> "N/A" then tcWebApi_get("Dhcp6s_Entry","SecDNSserver","s") end if%>">
          <font size=+1>&nbsp;</font></td>
     </tr>	
    </table></div>
	<script language="JavaScript" type="text/JavaScript">
		radvdChanged();
		dhcp6sChanged();
	</script>
	
	<% end if %>
	
	
        <% If tcWebApi_get("Lan_EtherMedia","enable","h") = "support" Then %>
        <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
        <tr>
                <td width="150" height="30" class="title-main"><font color="#FFFFFF">Ethernet Media Type</font></td>
                <td width="10" class="black">&nbsp;</td>
                <td width="150"><hr noshade></td>
                <td width="10" ><hr noshade></td>
                <td width="440"><hr noshade></td>
        </tr>
        <tr>
                <td class="light-orange">&nbsp;</td>
                <td class="light-orange">&nbsp;</td>
                <td class="tabdata"><div align=right><font color="#000000">Port 1</font></div></td>
                <td class="tabdata"><div align=center>:</div></td>
                <td class="tabdata">
                        <SELECT NAME="EtherMedia0" SIZE="1">
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port0","h") = "Auto" then asp_Write("selected") elseif tcWebApi_get("Lan_EtherMedia","port0","h") = "N/A" then asp_Write("selected") end if %>>Auto</OPTION>
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port0","h") = "100_Full" then asp_Write("selected") end if %>>100_Full</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port0","h") = "100_Half" then asp_Write("selected") end if %>>100_Half</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port0","h") = "10_Full" then asp_Write("selected") end if %>>10_Full</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port0","h") = "10_Half" then asp_Write("selected") end if %>>10_Half</OPTION>
		        </SELECT>
                </td>
        </tr>
        <% If tcWebApi_get("Info_Ether","is4PortsEther","h") = "Yes" Then%>
        <tr>
                <td class="light-orange">&nbsp;</td>
                <td class="light-orange">&nbsp;</td>
                <td class="tabdata"><div align=right><font color="#000000">Port 2</font></div></td>
                <td class="tabdata"><div align=center>:</div></td>
                <td class="tabdata">
                        <SELECT NAME="EtherMedia1" SIZE="1">
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port1","h") = "Auto" then asp_Write("selected") elseif tcWebApi_get("Lan_EtherMedia","port1","h") = "N/A" then asp_Write("selected") end if %>>Auto</OPTION>
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port1","h") = "100_Full" then asp_Write("selected") end if %>>100_Full</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port1","h") = "100_Half" then asp_Write("selected") end if %>>100_Half</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port1","h") = "10_Full" then asp_Write("selected") end if %>>10_Full</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port1","h") = "10_Half" then asp_Write("selected") end if %>>10_Half</OPTION>
		        </SELECT>
                </td>
        </tr>
        <tr>
                <td class="light-orange">&nbsp;</td>
                <td class="light-orange">&nbsp;</td>
                <td class="tabdata"><div align=right><font color="#000000">Port 3</font></div></td>
                <td class="tabdata"><div align=center>:</div></td>
                <td class="tabdata">
                        <SELECT NAME="EtherMedia2" SIZE="1">
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port2","h") = "Auto" then asp_Write("selected") elseif tcWebApi_get("Lan_EtherMedia","port2","h") = "N/A" then asp_Write("selected") end if %>>Auto</OPTION>
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port2","h") = "100_Full" then asp_Write("selected") end if %>>100_Full</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port2","h") = "100_Half" then asp_Write("selected") end if %>>100_Half</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port2","h") = "10_Full" then asp_Write("selected") end if %>>10_Full</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port2","h") = "10_Half" then asp_Write("selected") end if %>>10_Half</OPTION>
		        </SELECT>
                </td>
        </tr>
        <tr>
                <td class="light-orange">&nbsp;</td>
                <td class="light-orange">&nbsp;</td>
                <td class="tabdata"><div align=right><font color="#000000">Port 4</font></div></td>
                <td class="tabdata"><div align=center>:</div></td>
                <td class="tabdata">
                        <SELECT NAME="EtherMedia3" SIZE="1">
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port3","h") = "Auto" then asp_Write("selected") elseif tcWebApi_get("Lan_EtherMedia","port3","h") = "N/A" then asp_Write("selected") end if %>>Auto</OPTION>
		                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port3","h") = "100_Full" then asp_Write("selected") end if %>>100_Full</OPTION>
                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port3","h") = "100_Half" then asp_Write("selected") end if %>>100_Half</OPTION>

                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port3","h") = "10_Full" then asp_Write("selected") end if %>>10_Full</OPTION>

                                <OPTION <% if tcWebApi_get("Lan_EtherMedia","port3","h") = "10_Half" then asp_Write("selected") end if %>>10_Half</OPTION>



		        </SELECT>

                </td>

        </tr>

        <% end if %>

        </table>

  	<% end if %>


    <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

  <tr>

    <td width="150" class="light-orange">&nbsp;</td><td width="10"  class="light-orange"></td><td width="150">&nbsp;</td><td width="10" >&nbsp;</td><td width="440">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

  <tr height="42" >

    <td width="160" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">

        <INPUT TYPE="button" NAME="SaveBtn" VALUE="SAVE" onClick="uiSave()">        

        <INPUT TYPE="reset" NAME="lan_cancel" VALUE="CANCEL" >
		<INPUT TYPE="HIDDEN" NAME="DHCPMBSSIDNumberFlag" VALUE="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">

		<INPUT TYPE="HIDDEN" NAME="DHCP2PortsFlag" VALUE="<%tcWebApi_get("WebCustom_Entry","is2PSupported","s")%>">

		<INPUT TYPE="HIDDEN" NAME="DHCP1PortsFlag" VALUE="<%tcWebApi_get("WebCustom_Entry","is1PSupported","s")%>">

		<INPUT TYPE="HIDDEN" NAME="DHCPFilterFlag" VALUE="<%tcWebApi_get("WebCustom_Entry","isPortFltSupported","s")%>">

		<INPUT TYPE="HIDDEN" NAME="wlanISExist" VALUE="<%tcWebApi_get("Info_WLan","isExist","s")%>"></td></tr></table></div></form></body>

<script language="JavaScript">
	doDisplay();
</script>

</html>
