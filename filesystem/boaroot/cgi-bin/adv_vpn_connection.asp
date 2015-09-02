<%
	if Request_form("add_num") <> "" then
		tcWebApi_set("WebCurSet_Entry","vpn_id","add_num")
	end if
	if Request_form("editnum") <> "" then
		tcWebApi_set("WebCurSet_Entry","vpn_id","editnum")
	end if

	if Request_form("EditFlag") = "1" then
		tcWebApi_unset("VPN_Entry")
		tcWebApi_set("VPN_Entry","Connection_Name","ConnectionName")
		tcWebApi_set("VPN_Entry","Gateway_IP","GatewayIP")
		tcWebApi_set("VPN_Entry","Local_Gateway_IP","LocalGatewayIP")
		tcWebApi_set("VPN_Entry","Local_SubnetOrIP","LocalSubnetOrIP")
		tcWebApi_set("VPN_Entry","Local_IP","LocalIP")
		tcWebApi_set("VPN_Entry","Local_SubnetMask","LocalSbunetMask")
		tcWebApi_set("VPN_Entry","Remote_SubnetOrIP","RemoteSubnetOrIP")
		tcWebApi_set("VPN_Entry","Remote_IP","RemoteIP")
		tcWebApi_set("VPN_Entry","Remote_SubnetMask","RemoteSbunetMask")
		tcWebApi_set("VPN_Entry","PreShared_Key","PreSharedKey")
		tcWebApi_set("VPN_Entry","Pfs_Enable","PfsEnable")
		tcWebApi_set("VPN_Entry","Phase1_Mode","Phase1Mode")
		tcWebApi_set("VPN_Entry","Phase1_EncryptAlgo","Phase1EncryptAlgo")
		tcWebApi_set("VPN_Entry","Phase1_AuthAlgo","Phase1AuthAlgo")
		tcWebApi_set("VPN_Entry","Phase1_DhGroup","Phase1DhGroup")
		tcWebApi_set("VPN_Entry","Phase1_KeyLifeTime","Phase1KeyLifeTime")
		tcWebApi_set("VPN_Entry","Phase1_KeyLifeUnit","Phase1KeyLifeUnit")
		tcWebApi_set("VPN_Entry","Phase2_EncryptAlgo","Phase2EncryptAlgo")
		tcWebApi_set("VPN_Entry","Phase2_AuthAlgo","Phase2AuthAlgo")
		tcWebApi_set("VPN_Entry","Phase2_PfsGroup","Phase2PfsGroup")
		tcWebApi_set("VPN_Entry","Phase2_KeyLifeTime","Phase2KeyLifeTime")
		tcWebApi_set("VPN_Entry","Phase2_KeyLifeUnit","Phase2KeyLifeUnit")
		tcWebApi_commit("VPN_Entry")
	end if
%>
<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<LINK href="/style.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript>

<%if Request_form("EditFlag") = "1" then%>
window.location.href = "/cgi-bin/adv_vpn_setting.asp";
<%end if%>

function doOnloadCheck()
{
}

function localSubnetCheck()
{
	if (document.forms[0].LocalSubnetOrIP.selectedIndex == 1)
		document.forms[0].LocalSbunetMask.value = "255.255.255.255";
}

function remoteSubnetCheck()
{
	if (document.forms[0].RemoteSubnetOrIP.selectedIndex == 1)
		document.forms[0].RemoteSbunetMask.value = "255.255.255.255";
}

function pfsEnableCheck()
{
	if (document.forms[0].PfsEnable.selectedIndex == 0)
		document.forms[0].Phase2PfsGroup.disabled = false;
	else{
		document.forms[0].Phase2PfsGroup.value = 0;
		document.forms[0].Phase2PfsGroup.disabled = true;
	}
}

function isValidIpAddrRoute(ip1,ip2,ip3,ip4) {
    if(ip1==0 || ip4==255 || ip1==127 )
        return false;

    return true;
}

function valDoValidateIPRoute(Address) {
    var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
    var digits;
    var i;
    var error=null;

    if(address == null) {
        error="not an IP address";
        return false;
    }
    else {
        digits = address[0].split(".");
        for(i=0; i < 4; i++) {
            if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223) || (digits[i] == null)) {
              	error="Invalid IP address";
              	return false;
              	break;
            }
        }

        if((Number(digits[0])==0) && (Number(digits[1])==0) && (Number(digits[2])==0) && (Number(digits[3])==0))
        {
              	return false;
        }

        if(!isValidIpAddrRoute(digits[0],digits[1],digits[2],digits[3])) {
        	return false;
        }
    }
    return error;
}

function IPCheckRoute(address) {
    var message;
    if (address.value != "N/A") {
        message = valDoValidateIPRoute(address.value);
        if(message!=null) {
            address.focus();
            return false;
        }
   }
   return true;
}

function isValidIpAddr(ip1,ip2,ip3,ip4) {
    if(ip1==0 || ip4==0 || ip4==255 || ip1==127 )
        return false;

    return true;
}

function valDoValidateIP(Address) {
    var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
    var digits;
    var i;
    var error=null;

    if(address == null) {
        error="IP address is null";
        return false;
    }
    else {
        digits = address[0].split(".");
        for(i=0; i < 4; i++) {
            if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223)) {
              	error="Invalid IP address";
              	return false;
              	break;
            }
        }

        if((Number(digits[0])==1) && (Number(digits[1])==0) && (Number(digits[2])==0) && (Number(digits[3])==0))
            return error;

        if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3])) {
        	return false;
        }
    }
    return error;
}

function IPCheck(address) {
    var message;
    if (address.value != "N/A") {
        message = valDoValidateIP(address.value);
        if(message!=null) {
     	    address.focus();
     	    return false;
        }
    }
    return true;
}

function stringExist(s)
{
  var len = s.length;

  if(len == 0)
    return false;

	return true;
}

function isNumeric(s)
{
  var len= s.length;
  var ch;
  if(len==0)
    return false;
  for( i=0; i< len; i++)
  {
    ch= s.charAt(i);
    if( ch > '9' || ch < '0')
    {
      return false;
    }
  }
  return true;
}


function SubnetCheckRouter(mask,IPAddr) {
	var v=mask.value;
	var addr = IPAddr.value.split(".");
	var digits = v.split(".");
	if ( !((v == "0.0.0.0")||
		(v == "128.0.0.0")||
		(v == "192.0.0.0")||
		(v == "224.0.0.0")||
		(v == "240.0.0.0")||
		(v == "248.0.0.0")||
		(v == "252.0.0.0")||
		(v == "254.0.0.0")||
		(v == "255.0.0.0")||
		(v == "255.128.0.0")||
		(v == "255.192.0.0")||
		(v == "255.224.0.0")||
		(v == "255.240.0.0")||
		(v == "255.248.0.0")||
		(v == "255.252.0.0")||
		(v == "255.254.0.0")||
		(v == "255.255.0.0")||
		(v == "255.255.128.0")||
		(v == "255.255.192.0")||
		(v == "255.255.224.0")||
		(v == "255.255.240.0")||
		(v == "255.255.252.0")||
		(v == "255.255.254.0")||
		(v == "255.255.255.0")||
		(v == "255.255.255.128")||
		(v == "255.255.255.192")||
		(v == "255.255.255.224")||
		(v == "255.255.255.240")||
		(v == "255.255.255.248")||
		(v == "255.255.255.252")||
		(v == "255.255.255.254")||
		(v == "255.255.255.255")) )
	{
		alert("Invalid subnet mask value!");mask.focus();
		v = "0.0.0.0";
		return false;
	}
	
	if((Number(addr[3])==0)&&(Number(digits[3])==255))
	{
		alert("Invalid subnet mask!");
		return false;
	}
	
	if(!((Number(addr[0]&digits[0])==Number(addr[0]))
		&&(Number(addr[1]&digits[1])==Number(addr[1]))
		&&(Number(addr[2]&digits[2])==Number(addr[2]))
		&&(Number(addr[3]&digits[3])==Number(addr[3]))))
	{
		alert("Wrong subnet mask!");
		return false;
	}
	
	return true;
}

var vpnCount = 8;
function getVpnLocalIPs()
{
	var	localIPAddress = new Array(vpnCount);
	
	localIPAddress[0] = "<% tcWebApi_get("VPN_Entry0","Local_IP","s") %>";
	localIPAddress[1] = "<% tcWebApi_get("VPN_Entry1","Local_IP","s") %>";
	localIPAddress[2] = "<% tcWebApi_get("VPN_Entry2","Local_IP","s") %>";
	localIPAddress[3] = "<% tcWebApi_get("VPN_Entry3","Local_IP","s") %>";
	localIPAddress[4] = "<% tcWebApi_get("VPN_Entry4","Local_IP","s") %>";
	localIPAddress[5] = "<% tcWebApi_get("VPN_Entry5","Local_IP","s") %>";
	localIPAddress[6] = "<% tcWebApi_get("VPN_Entry6","Local_IP","s") %>";
	localIPAddress[7] = "<% tcWebApi_get("VPN_Entry7","Local_IP","s") %>";
	
	return localIPAddress;
}

function getVpnRemoteIPs()
{
	var	remoteIPAddress = new Array(vpnCount);

	remoteIPAddress[0] = "<% tcWebApi_get("VPN_Entry0","Remote_IP","s") %>";
	remoteIPAddress[1] = "<% tcWebApi_get("VPN_Entry1","Remote_IP","s") %>";
	remoteIPAddress[2] = "<% tcWebApi_get("VPN_Entry2","Remote_IP","s") %>";
	remoteIPAddress[3] = "<% tcWebApi_get("VPN_Entry3","Remote_IP","s") %>";
	remoteIPAddress[4] = "<% tcWebApi_get("VPN_Entry4","Remote_IP","s") %>";
	remoteIPAddress[5] = "<% tcWebApi_get("VPN_Entry5","Remote_IP","s") %>";
	remoteIPAddress[6] = "<% tcWebApi_get("VPN_Entry6","Remote_IP","s") %>";
	remoteIPAddress[7] = "<% tcWebApi_get("VPN_Entry7","Remote_IP","s") %>";
	
	return remoteIPAddress;
}


var vpnLocalIPs = getVpnLocalIPs();
var vpnRemoteIPs = getVpnRemoteIPs();

function doSubmit(index) {
	var currVpnConnStr = "<% tcWebApi_get("WebCurSet_Entry","vpn_id","s") %>";
	var cruuVpnConnNum = parseInt(currVpnConnStr);

	document.forms[0].EditFlag.value = index;

	if(index)
	{
		var localIP = document.forms[0].LocalIP;
		var remoteIP = document.forms[0].RemoteIP;
		var localMask = document.forms[0].LocalSbunetMask;
		var remoteMask = document.forms[0].RemoteSbunetMask;

		
		for (i = 0; i < vpnCount; i++)
		{
			if (i == cruuVpnConnNum)
				continue;
			
			if (vpnLocalIPs[i] == localIP.value)
			{
				for (j = 0; j < vpnCount; j++)
				{
					if (j == cruuVpnConnNum)
						continue;

					if (vpnRemoteIPs[j] == remoteIP.value)
					{
						alert('The same Local & Remote IP Address have been added!');
						return false;
					}
				}
			}
		}


		if (!stringExist(document.forms[0].ConnectionName.value))
		{
			alert("Please insert the Connection Name");
			document.forms[0].ConnectionName.focus();
			return false;
		}
		
		if(!IPCheckRoute(document.forms[0].LocalIP))
		{
			alert('The Local IP is invalid!');
			return false;
		}
		if(!IPCheckRoute(document.forms[0].RemoteIP))
		{
			alert('The Remote IP is invalid!');
			return false;
		}
		if(!IPCheck(document.forms[0].GatewayIP))
		{
			alert('The Remote Gateway IP is invalid!');
			return false;
		}
		if(!IPCheck(document.forms[0].LocalGatewayIP))
		{
			alert('The Local Gateway IP is invalid!');
			return false;
		}
	
		if(!SubnetCheckRouter(localMask,localIP))
		{ 
			alert('The Local Subnet Mask is invalid!');
			return false;
		}
		if(!SubnetCheckRouter(remoteMask,remoteIP))
		{ 
			alert('The Remote Subnet Mask is invalid!');
			return false;
		}
		if((localIP.value != "0.0.0.0") && (localMask.value == "0.0.0.0"))
		{
			alert("Invalid local subnet mask!");
			document.forms[0].LocalSbunetMask.focus();
			return false;
		}
		if((remoteIP.value != "0.0.0.0") && (remoteMask.value == "0.0.0.0"))
		{
			alert("Invalid remote subnet mask!");
			document.forms[0].RemoteSbunetMask.focus();
			return false;
		}

		if (
			(document.forms[0].LocalSubnetOrIP.selectedIndex == 1) && 
			(localMask.value != "255.255.255.255")
		)
		{
			alert("local subnet mask should be 255.255.255.255!");
			document.forms[0].LocalSbunetMask.focus();
			return false;			
		}

		if (
			(document.forms[0].RemoteSubnetOrIP.selectedIndex == 1) && 
			(remoteMask.value != "255.255.255.255")
		)
		{
			alert("remote subnet mask should be 255.255.255.255!");
			document.forms[0].RemoteSbunetMask.focus();
			return false;			
		}

		if (!isNumeric(document.forms[0].Phase1KeyLifeTime.value))
		{
			alert("Key Life Time for phase 1 is not a number");
			document.forms[0].Phase1KeyLifeTime.focus();
			return false;
		}
		if (!isNumeric(document.forms[0].Phase2KeyLifeTime.value))
		{
			alert("Key Life Time for phase 2 is not a number");
			document.forms[0].Phase2KeyLifeTime.focus();
			return false;
		}
	}
	
	document.forms[0].submit();	
}
</SCRIPT>

<META content="MSHTML 6.00.2800.1400" name=GENERATOR></HEAD>
<BODY onLoad="doOnloadCheck();">
<FORM name="vpnConnection_form" action="/cgi-bin/adv_vpn_connection.asp" method="post">
<DIV align=center>
<INPUT TYPE="HIDDEN" NAME="editnum">
<INPUT TYPE="HIDDEN" NAME="add_num">
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TD class=light-orange colSpan=5 height=5>&nbsp;</TD></TR>
  <TR>
    <TD class=title-main width=150 height=30><FONT color=#ffffff>VPN Connection Setting</FONT> </TD>
    <TD class=black width=10>&nbsp;</TD>
    <TD width=150>
      <HR noShade>
    </TD>
    <TD width=10>
      <HR noShade>
    </TD>
    <TD width=440>
      <HR noShade>
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Connection Name</FONT> 
    </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Connection_Name","h") <> "N/A" then TcWebApi_get("VPN_Entry","Connection_Name","s") else asp_Write("") end if %>" name="ConnectionName"> </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Local Gateway IP</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Local_Gateway_IP","h") <> "N/A" then TcWebApi_get("VPN_Entry","Local_Gateway_IP","s")  else asp_Write("0.0.0.0")  end if%>" name="LocalGatewayIP">
	</TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Remote Gateway IP</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Gateway_IP","h") <> "N/A" then TcWebApi_get("VPN_Entry","Gateway_IP","s")  else asp_Write("0.0.0.0")  end if%>" name="GatewayIP">
	</TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
	</TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Local Access Range</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="LocalSubnetOrIP" SIZE="1" onChange="localSubnetCheck()">
    		<option value="Subnet" <% if tcWebApi_staticGet("VPN_Entry","Local_SubnetOrIP","h") = "Subnet" then asp_Write("selected") end if %>>Subnet
			<option value="SingleIP" <% if tcWebApi_staticGet("VPN_Entry","Local_SubnetOrIP","h") = "SingleIP" then asp_Write("selected") end if %>>Single IP
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Local IP Address</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Local_IP","h") <> "N/A" then TcWebApi_get("VPN_Entry","Local_IP","s")  else asp_Write("0.0.0.0")  end if%>" name="LocalIP">
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>IP Subnetmask</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Local_SubnetMask","h") <> "N/A" then TcWebApi_get("VPN_Entry","Local_SubnetMask","s")  else asp_Write("0.0.0.0")  end if%>" name="LocalSbunetMask">
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
	</TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Remote Access Range</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="RemoteSubnetOrIP" SIZE="1" onChange="remoteSubnetCheck()">
    		<option value="Subnet" <% if tcWebApi_staticGet("VPN_Entry","Remote_SubnetOrIP","h") = "Subnet" then asp_Write("selected") end if %>>Subnet
			<option value="SingleIP" <% if tcWebApi_staticGet("VPN_Entry","Remote_SubnetOrIP","h") = "SingleIP" then asp_Write("selected") end if %>>Single IP
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Remote IP Address</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Remote_IP","h") <> "N/A" then TcWebApi_get("VPN_Entry","Remote_IP","s")  else asp_Write("0.0.0.0")  end if%>" name="RemoteIP">
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>IP Subnetmask</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Remote_SubnetMask","h") <> "N/A" then TcWebApi_get("VPN_Entry","Remote_SubnetMask","s")  else asp_Write("0.0.0.0")  end if%>" name="RemoteSbunetMask">
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
	</TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Pre-Shared Key</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","PreShared_Key","h") <> "N/A" then TcWebApi_get("VPN_Entry","PreShared_Key","s")  else asp_Write("")  end if%>" name="PreSharedKey">
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Perfect Forward Secrecy</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="PfsEnable" SIZE="1" onChange="pfsEnableCheck()">
    		<option value="Enable" <% if tcWebApi_staticGet("VPN_Entry","Pfs_Enable","h") = "Enable" then asp_Write("selected") end if %>>Enable
			<option value="Disable" <% if tcWebApi_staticGet("VPN_Entry","Pfs_Enable","h") = "Disable" then asp_Write("selected") end if %>>Disable
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
	</TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=center><strong><FONT color=#000000>Key Exchange Phase 1</FONT></strong></DIV>
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Exchange Mode</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Phase1Mode" SIZE="1">
    		<option value="0" <% if tcWebApi_staticGet("VPN_Entry","Phase1_Mode","h") = "0" then asp_Write("selected") end if %>>Main
			<option value="1" <% if tcWebApi_staticGet("VPN_Entry","Phase1_Mode","h") = "1" then asp_Write("selected") end if %>>Aggressive
			<option value="2" <% if tcWebApi_staticGet("VPN_Entry","Phase1_Mode","h") = "2" then asp_Write("selected") end if %>>Base
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Encryption Algorithm</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Phase1EncryptAlgo" SIZE="1">
    		<option value="des" <% if tcWebApi_staticGet("VPN_Entry","Phase1_EncryptAlgo","h") = "des" then asp_Write("selected") end if %>>DES
			<option value="3des" <% if tcWebApi_staticGet("VPN_Entry","Phase1_EncryptAlgo","h") = "3des" then asp_Write("selected") end if %>>3DES
			<option value="aes 128" <% if tcWebApi_staticGet("VPN_Entry","Phase1_EncryptAlgo","h") = "aes 128" then asp_Write("selected") end if %>>AES-128
			<option value="aes 192" <% if tcWebApi_staticGet("VPN_Entry","Phase1_EncryptAlgo","h") = "aes 192" then asp_Write("selected") end if %>>AES-192
			<option value="aes 256" <% if tcWebApi_staticGet("VPN_Entry","Phase1_EncryptAlgo","h") = "aes 256" then asp_Write("selected") end if %>>AES-256
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Authentication Algorithm</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Phase1AuthAlgo" SIZE="1">
    		<option value="md5" <% if tcWebApi_staticGet("VPN_Entry","Phase1_AuthAlgo","h") = "md5" then asp_Write("selected") end if %>>MD5
			<option value="sha1" <% if tcWebApi_staticGet("VPN_Entry","Phase1_AuthAlgo","h") = "sha1" then asp_Write("selected") end if %>>SHA1
			<option value="sha256" <% if tcWebApi_staticGet("VPN_Entry","Phase1_AuthAlgo","h") = "sha256" then asp_Write("selected") end if %>>SHA256
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Diffie-Hellman Group</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Phase1DhGroup" SIZE="1">
    		<option value="1" <% if tcWebApi_staticGet("VPN_Entry","Phase1_DhGroup","h") = "1" then asp_Write("selected") end if %>>768bit
			<option value="2" <% if tcWebApi_staticGet("VPN_Entry","Phase1_DhGroup","h") = "2" then asp_Write("selected") end if %>>1024bit
			<option value="5" <% if tcWebApi_staticGet("VPN_Entry","Phase1_DhGroup","h") = "5" then asp_Write("selected") end if %>>1536bit
    		<option value="14" <% if tcWebApi_staticGet("VPN_Entry","Phase1_DhGroup","h") = "14" then asp_Write("selected") end if %>>2048bit
			<option value="15" <% if tcWebApi_staticGet("VPN_Entry","Phase1_DhGroup","h") = "15" then asp_Write("selected") end if %>>3072bit
			<option value="16" <% if tcWebApi_staticGet("VPN_Entry","Phase1_DhGroup","h") = "16" then asp_Write("selected") end if %>>4096bit
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Key Life Time</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
		<INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Phase1_KeyLifeTime","h") <> "N/A" then TcWebApi_get("VPN_Entry","Phase1_KeyLifeTime","s")  else asp_Write("")  end if%>" name="Phase1KeyLifeTime">
    	<SELECT NAME="Phase1KeyLifeUnit" SIZE="1">
    		<option value="secs" <% if tcWebApi_staticGet("VPN_Entry","Phase1_KeyLifeUnit","h") = "secs" then asp_Write("selected") end if %>>Seconds
			<option value="mins" <% if tcWebApi_staticGet("VPN_Entry","Phase1_KeyLifeUnit","h") = "mins" then asp_Write("selected") end if %>>Minutes
			<option value="hours" <% if tcWebApi_staticGet("VPN_Entry","Phase1_KeyLifeUnit","h") = "hours" then asp_Write("selected") end if %>>Hours
    	</SELECT>
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
	</TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=center><strong><FONT color=#000000>Key Exchange Phase 2</FONT></strong></DIV>
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Encryption Algorithm</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Phase2EncryptAlgo" SIZE="1">
    		<option value="0" <% if tcWebApi_staticGet("VPN_Entry","Phase2_EncryptAlgo","h") = "0" then asp_Write("selected") end if %>>DES
			<option value="1" <% if tcWebApi_staticGet("VPN_Entry","Phase2_EncryptAlgo","h") = "1" then asp_Write("selected") end if %>>3DES
			<option value="2" <% if tcWebApi_staticGet("VPN_Entry","Phase2_EncryptAlgo","h") = "2" then asp_Write("selected") end if %>>AES-128
			<option value="3" <% if tcWebApi_staticGet("VPN_Entry","Phase2_EncryptAlgo","h") = "3" then asp_Write("selected") end if %>>AES-192
			<option value="4" <% if tcWebApi_staticGet("VPN_Entry","Phase2_EncryptAlgo","h") = "4" then asp_Write("selected") end if %>>AES-256
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Authentication Algorithm</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Phase2AuthAlgo" SIZE="1">
    		<option value="0" <% if tcWebApi_staticGet("VPN_Entry","Phase2_AuthAlgo","h") = "0" then asp_Write("selected") end if %>>MD5
			<option value="1" <% if tcWebApi_staticGet("VPN_Entry","Phase2_AuthAlgo","h") = "1" then asp_Write("selected") end if %>>SHA1
			<option value="2" <% if tcWebApi_staticGet("VPN_Entry","Phase2_AuthAlgo","h") = "2" then asp_Write("selected") end if %>>SHA256
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Diffie-Hellman Group</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<SELECT NAME="Phase2PfsGroup" SIZE="1">
    		<option value="1" <% if tcWebApi_staticGet("VPN_Entry","Phase2_PfsGroup","h") = "1" then asp_Write("selected") end if %>>768bit
			<option value="2" <% if tcWebApi_staticGet("VPN_Entry","Phase2_PfsGroup","h") = "2" then asp_Write("selected") end if %>>1024bit
			<option value="5" <% if tcWebApi_staticGet("VPN_Entry","Phase2_PfsGroup","h") = "5" then asp_Write("selected") end if %>>1536bit
    		<option value="14" <% if tcWebApi_staticGet("VPN_Entry","Phase2_PfsGroup","h") = "14" then asp_Write("selected") end if %>>2048bit
			<option value="15" <% if tcWebApi_staticGet("VPN_Entry","Phase2_PfsGroup","h") = "15" then asp_Write("selected") end if %>>3072bit
			<option value="16" <% if tcWebApi_staticGet("VPN_Entry","Phase2_PfsGroup","h") = "16" then asp_Write("selected") end if %>>4096bit
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Key Life Time</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
		<INPUT maxLength=16 size=16 value="<%If TcWebApi_get("VPN_Entry","Phase2_KeyLifeTime","h") <> "N/A" then TcWebApi_get("VPN_Entry","Phase2_KeyLifeTime","s")  else asp_Write("")  end if%>" name="Phase2KeyLifeTime">
    	<SELECT NAME="Phase2KeyLifeUnit" SIZE="1">
    		<option value="secs" <% if tcWebApi_staticGet("VPN_Entry","Phase2_KeyLifeUnit","h") = "secs" then asp_Write("selected") end if %>>Seconds
			<option value="mins" <% if tcWebApi_staticGet("VPN_Entry","Phase2_KeyLifeUnit","h") = "mins" then asp_Write("selected") end if %>>Minutes
			<option value="hours" <% if tcWebApi_staticGet("VPN_Entry","Phase2_KeyLifeUnit","h") = "hours" then asp_Write("selected") end if %>>Hours
    	</SELECT>
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD>&nbsp;</TD>
    <TD>&nbsp;</TD>
    <TD>&nbsp;</TD></TR></TBODY></TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR height=42>
    <TD class=black width=160>&nbsp;</TD>
    <TD class=orange width=50>&nbsp;</TD>
    <TD class=orange width=550>
	<INPUT onClick="doSubmit(1);" type=button value="SAVE" name="VpnSubmit">&nbsp;&nbsp;  
	<INPUT type="button" value="BACK" name="VpnBack" onClick="javascript:window.location='/cgi-bin/adv_vpn_setting.asp'" >&nbsp;&nbsp; 
	<INPUT type="hidden" value="0" name="EditFlag">    </TD></TR></TBODY>
  </TABLE>
</FORM></DIV></BODY></HTML>
