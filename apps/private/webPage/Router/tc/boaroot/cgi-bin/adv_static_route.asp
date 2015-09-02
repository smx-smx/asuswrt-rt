<%
	if Request_form("add_num") <> "" then
		tcWebApi_set("WebCurSet_Entry","route_id","add_num")
	end if
	if Request_form("editnum") <> "" then
		tcWebApi_set("WebCurSet_Entry","route_id","editnum")
	end if

	if Request_form("EditFlag") = "1" then
		tcWebApi_unset("Route_Entry")
		tcWebApi_commit("Route_Entry")
		tcWebApi_set("Route_Entry","Active","RouteActive")
		tcWebApi_set("Route_Entry","DST_IP","staticDestIP")
		tcWebApi_set("Route_Entry","Sub_mask","staticSubnetMask")
		tcWebApi_set("Route_Entry","User_def","EditFlag")
		if Request_form("Route_PVCGateway") = "Yes" then 
			tcWebApi_set("Route_Entry","Gateway","staticGatewayIP")
		else
			tcWebApi_set("Route_Entry","Device","Route_PVC_Index")
		end if
		tcWebApi_set("Route_Entry","metric","staticMetric")
		tcWebApi_commit("WebCurSet_Entry")
		tcWebApi_commit("Route_Entry")
	end if
%>
<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<LINK href="/style.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript>
<%if Request_form("EditFlag") = "1" then%>
window.location.href = "/cgi-bin/adv_routing_table.asp";
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
var aryISP = new Array("<% TcWebApi_get("Wan_PVC0","ISP","s")%>", "<% TcWebApi_get("Wan_PVC1","ISP","s")%>", "<% TcWebApi_get("Wan_PVC2","ISP","s")%>", "<% TcWebApi_get("Wan_PVC3","ISP","s")%>", "<% TcWebApi_get("Wan_PVC4","ISP","s")%>", "<% TcWebApi_get("Wan_PVC5","ISP","s")%>", "<% TcWebApi_get("Wan_PVC6","ISP","s")%>", "<% TcWebApi_get("Wan_PVC7","ISP","s")%>");
<%else%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
var aryISP = new Array("<% TcWebApi_get("Wan_PVC0","ISP","s")%>","<% TcWebApi_get("Wan_PVC1","ISP","s")%>", "<% TcWebApi_get("Wan_PVC2","ISP","s")%>", "<% TcWebApi_get("Wan_PVC3","ISP","s")%>", "<% TcWebApi_get("Wan_PVC4","ISP","s")%>", "<% TcWebApi_get("Wan_PVC5","ISP","s")%>", "<% TcWebApi_get("Wan_PVC6","ISP","s")%>", "<% TcWebApi_get("Wan_PVC7","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e0","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e1","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e2","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e3","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e4","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e5","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e6","ISP","s")%>","<% TcWebApi_get("WanExt_PVC8e7","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e0","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e1","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e2","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e3","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e4","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e5","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e6","ISP","s")%>","<% TcWebApi_get("WanExt_PVC9e7","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e0","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e1","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e2","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e3","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e4","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e5","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e6","ISP","s")%>","<% TcWebApi_get("WanExt_PVC10e7","ISP","s")%>");
<%else%>
var aryISP = new Array("<% TcWebApi_get("Wan_PVC0","ISP","s")%>","<% TcWebApi_get("Wan_PVC1","ISP","s")%>", "<% TcWebApi_get("Wan_PVC2","ISP","s")%>", "<% TcWebApi_get("Wan_PVC3","ISP","s")%>", "<% TcWebApi_get("Wan_PVC4","ISP","s")%>", "<% TcWebApi_get("Wan_PVC5","ISP","s")%>", "<% TcWebApi_get("Wan_PVC6","ISP","s")%>", "<% TcWebApi_get("Wan_PVC7","ISP","s")%>","<% TcWebApi_get("Wan_PVC8","ISP","s")%>","<% TcWebApi_get("Wan_PVC9","ISP","s")%>","<% TcWebApi_get("Wan_PVC10","ISP","s")%>");
<%end if%>
<%end if%>
function doGatewayCheck()
{
   	if (document.forms[0].Route_PVCGateway[0].checked == true){
        document.forms[0].staticGatewayIP.disabled = false;
		document.forms[0].Route_PVC_Index.disabled = true;
	}
	else{
	    document.forms[0].staticGatewayIP.disabled = true;
		document.forms[0].Route_PVC_Index.disabled = false;
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
        alert('Invalid destination IP address!');
        error="Invalid destination IP address";
        return false;
    }
    else {
        digits = address[0].split(".");
        for(i=0; i < 4; i++) {
            if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223) || (digits[i] == null)) {
                alert('Invalid destination IP address!');
              	error="Invalid IP address";
              	return false;
              	break;
            }
        }

        if((Number(digits[0])==0) && (Number(digits[1])==0) && (Number(digits[2])==0) && (Number(digits[3])==0))
        {
              	alert("Invalid destination IP address: "+Address);
              	return false;
        }

        if(!isValidIpAddrRoute(digits[0],digits[1],digits[2],digits[3],false)) {
         	alert('Invalid destination IP address!');
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
        alert('Invalid IP address!');
        error="Invalid IP address";
        return false;
    }
    else {
        digits = address[0].split(".");
        for(i=0; i < 4; i++) {
            if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223)) {
                alert('Invalid IP address!');
              	error="Invalid IP address";
              	return false;
              	break;
            }
        }

        if((Number(digits[0])==1) && (Number(digits[1])==0) && (Number(digits[2])==0) && (Number(digits[3])==0))
            return error;

        if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3],false)) {
            alert('Invalid IP address!');
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


function metricCheck() {

	var value = document.forms[0].staticMetric.value;

	if (!isNumeric(value)) {
		alert("Value for Metric must be in decimal and between 0 and 15");
		return true;
	}

	if (value < 1 || value > 15) {
		alert("Value for Metric must be between 1 and 15");
		return true;
	}

	return false;
}

function trans_to_realindex(now_index) {
	var pvc = now_index;
	var hasAtm=0, hasPtm=0, hasWan0=0;

<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	hasAtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	hasPtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	hasWan0 = 1;
<%end if%>

	if(hasAtm==1 && hasPtm==0 && hasWan0==1){
		//AtmEther, no Ptm
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		if(pvc>=8)
			pvc+=16;
<%else%>
		if(pvc==8) //ether
			pvc = 10;
<%end if%>			
	}

	if(hasAtm==0 && hasPtm==1 && hasWan0==1){
		//PtmEther, no Atm
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		pvc += 8;
<%else%>
		if(pvc==0) //ptm0
			pvc = 8;
		else if(pvc==1) //ptm1
			pvc = 9;
		else if(pvc==2) //ether
			pvc = 10;
<%end if%>		
		}

	if(hasAtm==0 && hasPtm==1 && hasWan0==0){
		//Ptm, no AtmEther
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		pvc += 8;
<%else%>
		if(pvc==0) //ptm0
			pvc = 8;
		else if(pvc==1) //ptm1
			pvc = 9;
<%end if%>			
	}

	if(hasAtm==0 && hasPtm==0 && hasWan0==1){
		//Ether, no AtmPtm
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		pvc += 24;
<%else%>
		if(pvc==0) //ether
			pvc = 10;
<%end if%>
		}
	return pvc;
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
		alert("Invalid subnet mask!");mask.focus();
		v = "0.0.0.0";
		return false;
	}
	if((Number(addr[3])==0)&&(Number(digits[3])==255)){
		alert("Invalid subnet mask!");return false;
	}
	if(!((Number(addr[0]&digits[0])==Number(addr[0]))
		&&(Number(addr[1]&digits[1])==Number(addr[1]))
		&&(Number(addr[2]&digits[2])==Number(addr[2]))
		&&(Number(addr[3]&digits[3])==Number(addr[3])))){
		alert("Invalid subnet mask!");return false;
	}
	return true;
}

var routecount = 16;
function stStaticRoute(domain,DestIPAddress)
{
	this.domain = domain;
	this.DestIPAddress = DestIPAddress;
}
function getStaticRouteInfo()
{
	var	nCurTemp = 0;
	var	vDestIPAddress = new Array(routecount);
	var	vcurLinks = new Array(routecount);
	
	vDestIPAddress[0] = "<% tcWebApi_get("Route_Entry0","DST_IP","s") %>";
	vDestIPAddress[1] = "<% tcWebApi_get("Route_Entry1","DST_IP","s") %>";
	vDestIPAddress[2] = "<% tcWebApi_get("Route_Entry2","DST_IP","s") %>";
	vDestIPAddress[3] = "<% tcWebApi_get("Route_Entry3","DST_IP","s") %>";
	vDestIPAddress[4] = "<% tcWebApi_get("Route_Entry4","DST_IP","s") %>";
	vDestIPAddress[5] = "<% tcWebApi_get("Route_Entry5","DST_IP","s") %>";
	vDestIPAddress[6] = "<% tcWebApi_get("Route_Entry6","DST_IP","s") %>";
	vDestIPAddress[7] = "<% tcWebApi_get("Route_Entry7","DST_IP","s") %>";
	vDestIPAddress[8] = "<% tcWebApi_get("Route_Entry8","DST_IP","s") %>";
	vDestIPAddress[9] = "<% tcWebApi_get("Route_Entry9","DST_IP","s") %>";
	vDestIPAddress[10] = "<% tcWebApi_get("Route_Entry10","DST_IP","s") %>";
	vDestIPAddress[11] = "<% tcWebApi_get("Route_Entry11","DST_IP","s") %>";
	vDestIPAddress[12] = "<% tcWebApi_get("Route_Entry12","DST_IP","s") %>";
	vDestIPAddress[13] = "<% tcWebApi_get("Route_Entry13","DST_IP","s") %>";
	vDestIPAddress[14] = "<% tcWebApi_get("Route_Entry14","DST_IP","s") %>";
	vDestIPAddress[15] = "<% tcWebApi_get("Route_Entry15","DST_IP","s") %>";
	
	for(var i=0; i<routecount; i++)
	{
		if(vDestIPAddress[i] != "N/A")
			vcurLinks[nCurTemp++] = new stStaticRoute(i, vDestIPAddress[i]);
	}
	var	vObjRet = new Array(nCurTemp+1);
	for(var m=0; m<nCurTemp; m++)
	{
		vObjRet[m] = vcurLinks[m];
	}
	vObjRet[nCurTemp] = null;
	return vObjRet;
}
var StaticRouteInfo = getStaticRouteInfo();

function doSubmit(index) {
	var pvc= document.forms[0].Route_PVC_Index.options.selectedIndex;
	var mask = document.forms[0].staticSubnetMask;
	var destIP = document.forms[0].staticDestIP;

	document.forms[0].EditFlag.value=index;
	document.forms[0].RouteActive.value="Yes";

	if(index)
	{
	
		var strdstIPtmp = "<% tcWebApi_get("Route_Entry","DST_IP","s") %>";
		var stripvalue = document.forms[0].staticDestIP.value;
		if ((strdstIPtmp == "N/A") || ((strdstIPtmp != "N/A") && (stripvalue != strdstIPtmp))) {
			for (i = 0; i < StaticRouteInfo.length - 1; i++)
			{
				if (StaticRouteInfo[i].DestIPAddress == stripvalue)
				{
					alert(stripvalue + ' has added!');
					return false;
				}
			}
		}
		
		if (metricCheck()) return false;
		if(!IPCheckRoute(document.forms[0].staticDestIP)) return false;
		if (document.forms[0].Route_PVCGateway[0].checked)
			if(!IPCheck(document.forms[0].staticGatewayIP)) return false;
		if (document.forms[0].Route_PVCGateway[1].checked) {	
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>			
			var strpvc = document.forms[0].Route_PVC_Index.value;
			var strisptmp = aryISP[parseInt(strpvc.charAt(3))];
<%else%>
			var strpvc = trans_to_realindex(document.forms[0].Route_PVC_Index.options.selectedIndex);
			var strisptmp = aryISP[strpvc];
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
			if (strpvc >= 8){
				if ((strisptmp == "0") || (strisptmp == "1")){
					alert("Can not choose Service on MER mode!");
					return false;
				}
				else if((strisptmp == "3") || (strisptmp == "N/A")){
					alert("Can not choose Service on Bridge mode!");
					return false;
				}
			} else {
<%end if%>
			if ((strisptmp == "0") || (strisptmp == "1")){
				alert("Can not choose pvc on MER mode!");
				return false;
			}
			else if((strisptmp == "3") || (strisptmp == "N/A")){
				alert("Can not choose pvc on Bridge mode!");
				return false;
			}
			
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
			}	
<%end if%>			

		}
		if(!SubnetCheckRouter(mask,destIP))
		{ 
			return false;
		}
		if((destIP.value != "0.0.0.0") && (mask.value == "0.0.0.0") ){
			alert("Invalid subnet mask!");
			document.forms[0].staticSubnetMask.focus();
			return false;
		}
	}
	document.forms[0].submit();	
}
function changePVC(theselect)
{
	var pvc = trans_to_realindex(document.forms[0].Route_PVC_Index.options.selectedIndex);

	document.forms[0].pvc_index_num.value = pvc;
}
</SCRIPT>

<META content="MSHTML 6.00.2800.1400" name=GENERATOR></HEAD>
<BODY onLoad="doGatewayCheck();">
<FORM name="Static_Eng" action="/cgi-bin/adv_static_route.asp" method="post">
<DIV align=center>
<INPUT TYPE="HIDDEN" NAME="User_def" value="1">
<INPUT TYPE="HIDDEN" NAME="editnum">
<INPUT TYPE="HIDDEN" NAME="Route_num">
<INPUT TYPE="HIDDEN" NAME="add_num">
<INPUT TYPE="HIDDEN" NAME="RouteActive" value="Yes">
<INPUT TYPE="HIDDEN" NAME="pvc_index_num" value="0">
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TD class=light-orange colSpan=5 height=5>&nbsp;</TD></TR>
  <TR>
    <TD class=title-main width=150 height=30><FONT color=#ffffff>Static 
      Route</FONT> </TD>
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
      <DIV align=right><FONT color=#000000>Destination IP Address</FONT> 
    </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("Route_Entry","DST_IP","h") <> "N/A" then TcWebApi_get("Route_Entry","DST_IP","s") else asp_Write("0.0.0.0") end if %>" name="staticDestIP"> </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>IP Subnet Mask</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=16 size=16 value="<%If TcWebApi_get("Route_Entry","Sub_mask","h") <> "N/A" then TcWebApi_get("Route_Entry","Sub_mask","s")  else asp_Write("0.0.0.0")  end if%>" name="staticSubnetMask"></TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Gateway IP Address</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    	<INPUT onclick=doGatewayCheck() type=radio value="Yes" <%If TcWebApi_get("Route_Entry","Gateway","h") <> "N/A" then asp_Write("checked")  end if%> name="Route_PVCGateway">
    	<INPUT maxLength=16 size=16 value="<%If TcWebApi_get("Route_Entry","Gateway","h") <> "N/A" then TcWebApi_get("Route_Entry","Gateway","s")  else asp_Write("0.0.0.0")  end if%>" name="staticGatewayIP">
    	<INPUT onclick=doGatewayCheck() type=radio value="No" <%If TcWebApi_get("Route_Entry","Gateway","h") = "N/A" then asp_Write("checked")  end if%> name="Route_PVCGateway">
    		<SELECT NAME="Route_PVC_Index" SIZE="1" onChange="changePVC()">

<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
    	<option value="PVC0">PVC0
		<option value="PVC1">PVC1
		<option value="PVC2">PVC2
		<option value="PVC3">PVC3
		<option value="PVC4">PVC4
		<option value="PVC5">PVC5
		<option value="PVC6">PVC6
		<option value="PVC7">PVC7

<%else%>
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
    	<option value="PVC0" <%if Request_form("pvc_index_num") = "0" then asp_Write("selected") end if%>>PVC0
		<option value="PVC1" <%if Request_form("pvc_index_num") = "1" then asp_Write("selected") end if%>>PVC1
		<option value="PVC2" <%if Request_form("pvc_index_num") = "2" then asp_Write("selected") end if%>>PVC2
		<option value="PVC3" <%if Request_form("pvc_index_num") = "3" then asp_Write("selected") end if%>>PVC3
		<option value="PVC4" <%if Request_form("pvc_index_num") = "4" then asp_Write("selected") end if%>>PVC4
		<option value="PVC5" <%if Request_form("pvc_index_num") = "5" then asp_Write("selected") end if%>>PVC5
		<option value="PVC6" <%if Request_form("pvc_index_num") = "6" then asp_Write("selected") end if%>>PVC6
		<option value="PVC7" <%if Request_form("pvc_index_num") = "7" then asp_Write("selected") end if%>>PVC7
	<%end if%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		<option value="PVC8e0" <%if Request_form("pvc_index_num") = "8" then asp_Write("selected") end if%>>PTM0_0
		<option value="PVC8e1" <%if Request_form("pvc_index_num") = "9" then asp_Write("selected") end if%>>PTM0_1
		<option value="PVC8e2" <%if Request_form("pvc_index_num") = "10" then asp_Write("selected") end if%>>PTM0_2
		<option value="PVC8e3" <%if Request_form("pvc_index_num") = "11" then asp_Write("selected") end if%>>PTM0_3
		<option value="PVC8e4" <%if Request_form("pvc_index_num") = "12" then asp_Write("selected") end if%>>PTM0_4
		<option value="PVC8e5" <%if Request_form("pvc_index_num") = "13" then asp_Write("selected") end if%>>PTM0_5
		<option value="PVC8e6" <%if Request_form("pvc_index_num") = "14" then asp_Write("selected") end if%>>PTM0_6
		<option value="PVC8e7" <%if Request_form("pvc_index_num") = "15" then asp_Write("selected") end if%>>PTM0_7
		<option value="PVC9e0" <%if Request_form("pvc_index_num") = "16" then asp_Write("selected") end if%>>PTM1_0
		<option value="PVC9e1" <%if Request_form("pvc_index_num") = "17" then asp_Write("selected") end if%>>PTM1_1
		<option value="PVC9e2" <%if Request_form("pvc_index_num") = "18" then asp_Write("selected") end if%>>PTM1_2
		<option value="PVC9e3" <%if Request_form("pvc_index_num") = "19" then asp_Write("selected") end if%>>PTM1_3
		<option value="PVC9e4" <%if Request_form("pvc_index_num") = "20" then asp_Write("selected") end if%>>PTM1_4
		<option value="PVC9e5" <%if Request_form("pvc_index_num") = "21" then asp_Write("selected") end if%>>PTM1_5
		<option value="PVC9e6" <%if Request_form("pvc_index_num") = "22" then asp_Write("selected") end if%>>PTM1_6
		<option value="PVC9e7" <%if Request_form("pvc_index_num") = "23" then asp_Write("selected") end if%>>PTM1_7
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		<option value="PVC10e0" <%if Request_form("pvc_index_num") = "24" then asp_Write("selected") end if%>>WAN0_0
		<option value="PVC10e1" <%if Request_form("pvc_index_num") = "25" then asp_Write("selected") end if%>>WAN0_1
		<option value="PVC10e2" <%if Request_form("pvc_index_num") = "26" then asp_Write("selected") end if%>>WAN0_2
		<option value="PVC10e3" <%if Request_form("pvc_index_num") = "27" then asp_Write("selected") end if%>>WAN0_3
		<option value="PVC10e4" <%if Request_form("pvc_index_num") = "28" then asp_Write("selected") end if%>>WAN0_4
		<option value="PVC10e5" <%if Request_form("pvc_index_num") = "29" then asp_Write("selected") end if%>>WAN0_5
		<option value="PVC10e6" <%if Request_form("pvc_index_num") = "30" then asp_Write("selected") end if%>>WAN0_6
		<option value="PVC10e7" <%if Request_form("pvc_index_num") = "31" then asp_Write("selected") end if%>>WAN0_7
	<%end if%>
<%else%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		<option value="PVC8" <%if Request_form("pvc_index_num") = "8" then asp_Write("selected") end if%>>PTM0
		<option value="PVC9" <%if Request_form("pvc_index_num") = "9" then asp_Write("selected") end if%>>PTM1
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		<option value="PVC10" <%if Request_form("pvc_index_num") = "10" then asp_Write("selected") end if%>>WAN0
	<%end if%>
<%end if%>
<%end if%>
    	</SELECT> 
    </TD></TR>
  <TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=tabdata>
      <DIV align=right><FONT color=#000000>Metric</FONT> </DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata><INPUT maxLength=5 size=3 value="<%If TcWebApi_get("Route_Entry","metric","h") <> "N/A" then TcWebApi_get("Route_Entry","metric","s") else asp_Write("0")  end if%>" name="staticMetric"> 
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
	<INPUT onClick="doSubmit(1);" type=button value="SAVE" name="StaticSubmit">&nbsp;&nbsp;  
	<INPUT type="button" value="BACK" name="StaticBack" onClick="javascript:window.location='/cgi-bin/adv_routing_table.asp'" >&nbsp;&nbsp; 
	<INPUT type="reset" value="CANCEL" name="DTStaticReset">
	<INPUT type="hidden" value="0" name="EditFlag">    </TD></TR></TBODY>
  </TABLE>
</FORM></DIV></BODY></HTML>
