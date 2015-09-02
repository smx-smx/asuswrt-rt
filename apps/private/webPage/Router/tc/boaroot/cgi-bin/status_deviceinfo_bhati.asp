<%
	tcWebApi_set("WebCurSet_Entry","dev_pvc","DvInfo_PVC")
	If Request_Form("Saveflag")="1" then
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
<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<TITLE>ASP Example</TITLE>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language="JavaScript">
function doSave() {
	document.DvInfo_Form.submit();
}
function renewrelease(ip){
  document.DvInfo_Form.Dipflag.value = ip;
  document.DvInfo_Form.Saveflag.value = 1;
  document.DvInfo_Form.submit();  
}
function reconnect(flag){
  document.DvInfo_Form.DipConnFlag.value = flag;
  document.DvInfo_Form.Saveflag.value = 1;
  document.DvInfo_Form.submit();  
}
self.setTimeout("self.location.reload(true);",20000);
</script>
</HEAD>
<BODY>
<FORM METHOD="POST" ACTION="/cgi-bin/status_deviceinfo.asp" name="DvInfo_Form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<INPUT TYPE="HIDDEN" NAME="Saveflag" VALUE="0">
<tr>
<td height="5" class="light-orange" colspan="5">
<INPUT TYPE="HIDDEN" NAME="Dipflag" VALUE="0">
</td></tr><tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF">Device Information</font></td><td width="10" class="black">&nbsp;</td><td width="150">&nbsp;</td><td width="10" >&nbsp;</td><td width="440">&nbsp;</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"> <div align=right>
<font color="#000000">Firmware Version</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_get("DeviceInfo","FwVer","h") <> "N/A" Then tcWebApi_staticGet("DeviceInfo","FwVer","s") end if%>     
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">MAC Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_get("Info_Ether","mac","h") <> "N/A" Then tcWebApi_staticGet("Info_Ether","mac","s") end if%>   
</td></tr><tr>
<td class="title-main">
<font color="#FFFFFF">LAN</font></td><td class="black">&nbsp;</td><td><hr noshade></td><td><hr noshade></td><td><hr noshade></td></tr>
<tr>
		<td class="title-sub"><font color="#000000">IPv4</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("Info_Ether","ip","h") <> "N/A" Then tcWebApi_staticGet("Info_Ether","ip","s") end if%>       
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Subnet Mask</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_get("Lan_Entry","netmask","h") <> "N/A" Then tcWebApi_staticGet("Lan_Entry","netmask","s") end if%>  	  
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">DHCP Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("Lan_Dhcp","type","h") = "1" Then asp_Write("Enable") elseif tcWebApi_staticGet("Lan_Dhcp","type","h") = "2" Then asp_Write("Relay") else asp_Write("Disabled") end if%>
</td></tr>
<% if tcWebApi_staticGet("Info_Ether","isIPv6Supported","h") = "Yes" then %>
<tr>
		<td class="title-sub"><font color="#000000">IPv6</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("Lan_Entry0","IP6","h") <> "N/A" Then tcWebApi_staticGet("Lan_Entry0","IP6","s") end if%>       
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Prefix Length</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_staticGet("Lan_Entry0","PREFIX6","h") <> "N/A" Then tcWebApi_staticGet("Lan_Entry0","PREFIX6","s") end if%>  	  
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">DHCP Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If tcWebApi_get("Dhcp6s_Entry","Enable","h") = "1" Then asp_Write("Enable") else asp_Write("Disabled") end if%>
</td></tr>
<% end if %>
</table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF">WAN</font></td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td width="440"><hr noshade>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
<font color="#000000">Virtual Circuit</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata"><font color="#000000">PVC</font>
<SELECT NAME="DvInfo_PVC" SIZE="1" onChange="doSave()">
<OPTION <% if tcWebApi_get("WebCurSet_Entry","dev_pvc","h") = "0" then asp_Write("selected") end if %>>0
<OPTION <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "1" then asp_Write("selected") end if %>>1
<OPTION <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "2" then asp_Write("selected") end if %>>2
<OPTION <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "3" then asp_Write("selected") end if %>>3
<OPTION <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "4" then asp_Write("selected") end if %>>4
<OPTION <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "5" then asp_Write("selected") end if %>>5
<OPTION <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "6" then asp_Write("selected") end if %>>6
<OPTION <% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "7" then asp_Write("selected") end if %>>7
</SELECT></td></tr><tr><td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Connection Type</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<INPUT TYPE="HIDDEN" NAME="DipConnFlag" VALUE="0">
        <% if tcWebApi_get("DeviceInfo_PVC","connType","h") <> "" then tcWebApi_staticGet("DeviceInfo_PVC","connType","s")  end if %>
        &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp 
		<%If tcWebApi_staticGet("DeviceInfo_PVC","DispConnBtnType","h") <> "0" then%>
        	<%If tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "1" Then%>
        	<input type="button" name="Disconnect" value="Disconnect" onClick="reconnect(2)" > 
        	<%else %>
        	<INPUT TYPE="button" NAME="Connect" VALUE="Connect" onClick="reconnect(1)" >  		
			<%End if%>
		<%End if%>
</td></tr>
<%If tcWebApi_staticGet("DeviceInfo_PVC","DispPPPconn","h") <> "0" then%>
<tr><td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">PPP connection time</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","pppConnTime","s")%>	
</td></tr>
<%end if%>
<tr>
		<td class="title-sub"><font color="#000000">IPv4</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Status</font>    </div></td><td class="tabdata" ><div align=center>:</div></td><td class="tabdata">
<%
if tcWebApi_get("GUI_Entry0","Custom","h") = "C9" then
    If tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "3" Then
	asp_Write("ADSL CONNECTION FAILURE")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "5" Then 
	asp_Write("PPP up") 
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "4" Then  
	asp_Write("PPP down")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "2" Then
	asp_Write("PPP USERNAME/PASSWORD INVALID")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "1" Then
	asp_Write("Connected")
    else asp_Write("Not Connected") 
    end if
else
    If tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "1" Then asp_Write("Connected") else asp_Write("Not Connected") end if
end if
%>
</td></tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","WanIP","s")%>  &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp		
<%If tcWebApi_staticGet("DeviceInfo_PVC","DispBtnType","h") <> "0" then%>
<INPUT TYPE="button" NAME="renewIP" VALUE="renewIP" onClick="renewrelease(1)" >
<INPUT TYPE="button" NAME="releaseIP" VALUE="releaseIP" onClick="renewrelease(2)" >  		
<%End if%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Subnet Mask</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","WanSubMask","s")%>	    
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Default Gateway</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","WanDefGW","s")%>	
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">DNS Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<script language="JavaScript" type="text/JavaScript">
var artIpver = ["<% tcWebApi_staticGet("Wan_PVC0","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC1","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC2","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC3","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC4","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC5","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC6","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC7","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC8","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC9","IPVERSION","s") %>",
					"<% tcWebApi_staticGet("Wan_PVC10","IPVERSION","s") %>"];
	var pvc_index = parseInt(document.DvInfo_Form.DvInfo_PVC.value);
	var strtype = "<% tcWebApi_get("Dproxy_Entry","type","s") %>";
	var strPriDNS = "<% tcWebApi_staticGet("Dproxy_Entry","Primary_DNS","s") %>";
	var strDevDNS = "<% tcWebApi_staticGet("DeviceInfo_PVC","DNSIP","s") %>";
	
	if (("IPv6" == artIpver[pvc_index]) || ("N/A" == artIpver[pvc_index]))
		document.writeln("N/A");
	else{
		if (strtype == "1") {
			document.writeln(strPriDNS);
		}
		else{
			document.writeln(strDevDNS);
		}	
	}	
</script>
</td></tr>
<% if tcWebApi_staticGet("Info_Ether","isIPv6Supported","h") = "Yes" then %>
<tr>
		<td class="title-sub"><font color="#000000">IPv6</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Status</font>    </div></td><td class="tabdata" ><div align=center>:</div></td><td class="tabdata">
<%
if tcWebApi_staticGet("GUI_Entry0","Custom","h") = "C9" then
    If tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "3" Then
	asp_Write("ADSL CONNECTION FAILURE")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "5" Then 
	asp_Write("PPP up") 
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "4" Then  
	asp_Write("PPP down")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "2" Then
	asp_Write("PPP USERNAME/PASSWORD INVALID")
    elseif tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "1" Then
	asp_Write("Connected")
    else asp_Write("Not Connected") 
    end if
else
    If tcWebApi_staticGet("DeviceInfo_PVC","IP6Status","h") = "1" Then asp_Write("Connected") else asp_Write("Not Connected") end if
end if
%>
</td>
</tr>
<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">IP Address</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
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
</script>  &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp		
<%If tcWebApi_staticGet("DeviceInfo_PVC","IP6DispBtnType","h") <> "0" then%>
<INPUT TYPE="button" NAME="renewIP6" VALUE="renewIP6" onClick="renewrelease(3)" >
<INPUT TYPE="button" NAME="releaseIP6" VALUE="releaseIP6" onClick="renewrelease(4)">  		
<%End if%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Prefix Length</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<script language="JavaScript" type="text/JavaScript">
	if("N/A" != str_IP6){
		var str_prelen = "64";
		var plen = str_IP6.indexOf('/');
		if(plen != -1){
			str_prelen = str_IP6.substring(1+plen, 3+plen);
		}
		document.writeln(str_prelen);
	}
	else{
		document.writeln(str_IP6);
	}
</script>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Default Gateway</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6WanDefGW","s")%>	
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">DNS Server</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6DNSIP","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Prefix Delegation</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%tcWebApi_staticGet("DeviceInfo_PVC","IP6PreDelegation","s")%>	
</td></tr>
<% end if %>
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF" class="tabdata">
<tr>
<td width="150" height="30" class="title-main">
<font color="#FFFFFF">ADSL</font></td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td colspan="3"><hr noshade></td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">ADSL Firmware Ver</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%If tcWebApi_get("Info_Adsl","fwVer","h") <> "N/A" then tcWebApi_staticGet("Info_Adsl","fwVer","s") end if%>     
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Line State</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Modulation</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%tcWebApi_staticGet("Info_Adsl","Opmode","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Annex Mode</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" colspan="3">
<%tcWebApi_get("Adsl_Entry","ANNEXTYPEA","s")%>
</td></tr><tr>
<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata" colspan="3"><hr noshade class="light-orange-line"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right></div></td><td class="tabdata"></td><td width="80" align=center class="tabdata">
<font color="#000000">Downstream</font></td><td width="80" align=center class="tabdata">
<font color="#000000">Upstream</font></td><td width="280">&nbsp;</td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">SNR Margin</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","SNRMarginDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","SNRMarginUp","s")%>
</td><td class="tabdata"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Line Attenuation</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","AttenDown","s")%>
</td><td width="80" align=center class="tabdata">
<%tcWebApi_staticGet("Info_Adsl","AttenUp","s")%>
</td><td width="280" class="tabdata"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
<font color="#000000">Data Rate</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">

<%tcWebApi_staticGet("Info_Adsl","DataRateDown","s")%>

</td><td width="80" align=center class="tabdata">

<%tcWebApi_staticGet("Info_Adsl","DataRateUp","s")%>

</td><td width="280" class="tabdata"></td></tr><tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right> 
<font color="#000000">Out Power</font></div></td><td class="tabdata"><div align=center>:</div></td><td width="80" align=center class="tabdata">

<%tcWebApi_staticGet("Info_Adsl","PowerDown","s")%>

</td><td width="80" align=center class="tabdata">

<%tcWebApi_staticGet("Info_Adsl","PowerUp","s")%>

</td><td width="280" class="tabdata"></td></tr><tr>

<td class="light-orange"></td><td class="light-orange"></td><td width="150">&nbsp;</td><td width="10">&nbsp;</td><td colspan="3" class="tabdata">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

<tr>

<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">&nbsp;</td></tr></table></form>    

</BODY>

</HTML>

