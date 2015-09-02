<%
	request_Form("PortBind_Index")
	TCWebAPI_set("WebCurSet_Entry","portbind_id","PortBind_Index")
	TCWebAPI_set("PortBind_Common", "Active", "PortBind_active")
	if request_Form("Add_or_Delete") = "1" then
		TCWebAPI_set("PortBind_Common", "Active", "PortBind_active")
		if request_Form("PortBind_active") = "Yes" then
			TCWebAPI_unset("PortBind_Entry")
		end if
		if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then
			TCWebAPI_set("PortBind_Entry", "p0", "PortBind_ATM_Port_0")
			TCWebAPI_set("PortBind_Entry", "p1", "PortBind_ATM_Port_1")
			TCWebAPI_set("PortBind_Entry", "p2", "PortBind_ATM_Port_2")
			TCWebAPI_set("PortBind_Entry", "p3", "PortBind_ATM_Port_3")
			TCWebAPI_set("PortBind_Entry", "p4", "PortBind_ATM_Port_4")
			TCWebAPI_set("PortBind_Entry", "p5", "PortBind_ATM_Port_5")
			TCWebAPI_set("PortBind_Entry", "p6", "PortBind_ATM_Port_6")
			TCWebAPI_set("PortBind_Entry", "p7", "PortBind_ATM_Port_7")
		end if	

		if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then
			if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
				TCWebAPI_set("PortBind_Entry", "p8_0", "PortBind_PTM_Port_0_0")
				TCWebAPI_set("PortBind_Entry", "p8_1", "PortBind_PTM_Port_0_1")
				TCWebAPI_set("PortBind_Entry", "p8_2", "PortBind_PTM_Port_0_2")
				TCWebAPI_set("PortBind_Entry", "p8_3", "PortBind_PTM_Port_0_3")
				TCWebAPI_set("PortBind_Entry", "p8_4", "PortBind_PTM_Port_0_4")
				TCWebAPI_set("PortBind_Entry", "p8_5", "PortBind_PTM_Port_0_5")
				TCWebAPI_set("PortBind_Entry", "p8_6", "PortBind_PTM_Port_0_6")
				TCWebAPI_set("PortBind_Entry", "p8_7", "PortBind_PTM_Port_0_7")
				TCWebAPI_set("PortBind_Entry", "p9_0", "PortBind_PTM_Port_1_0")
				TCWebAPI_set("PortBind_Entry", "p9_1", "PortBind_PTM_Port_1_1")
				TCWebAPI_set("PortBind_Entry", "p9_2", "PortBind_PTM_Port_1_2")
				TCWebAPI_set("PortBind_Entry", "p9_3", "PortBind_PTM_Port_1_3")
				TCWebAPI_set("PortBind_Entry", "p9_4", "PortBind_PTM_Port_1_4")
				TCWebAPI_set("PortBind_Entry", "p9_5", "PortBind_PTM_Port_1_5")
				TCWebAPI_set("PortBind_Entry", "p9_6", "PortBind_PTM_Port_1_6")
				TCWebAPI_set("PortBind_Entry", "p9_7", "PortBind_PTM_Port_1_7")
			else	
				TCWebAPI_set("PortBind_Entry", "p8", "PortBind_PTM_Port_0")
				TCWebAPI_set("PortBind_Entry", "p9", "PortBind_PTM_Port_1")
			end if
		end if	

		if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then
			if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
				TCWebAPI_set("PortBind_Entry", "p10_0", "PortBind_WAN0_Port_0_0")
				TCWebAPI_set("PortBind_Entry", "p10_1", "PortBind_WAN0_Port_0_1")
				TCWebAPI_set("PortBind_Entry", "p10_2", "PortBind_WAN0_Port_0_2")
				TCWebAPI_set("PortBind_Entry", "p10_3", "PortBind_WAN0_Port_0_3")
				TCWebAPI_set("PortBind_Entry", "p10_4", "PortBind_WAN0_Port_0_4")
				TCWebAPI_set("PortBind_Entry", "p10_5", "PortBind_WAN0_Port_0_5")
				TCWebAPI_set("PortBind_Entry", "p10_6", "PortBind_WAN0_Port_0_6")
				TCWebAPI_set("PortBind_Entry", "p10_7", "PortBind_WAN0_Port_0_7")
			else	
				TCWebAPI_set("PortBind_Entry", "p10", "PortBind_WAN0_Port_0")
			end if	
		end if	

		TCWebAPI_set("PortBind_Entry", "e1", "PortBind_Ethernet_Port_1")
		TCWebAPI_set("PortBind_Entry", "e2", "PortBind_Ethernet_Port_2")
		if request_Form("PB2PortsFlag") <> "Yes" then
			TCWebAPI_set("PortBind_Entry", "e3", "PortBind_Ethernet_Port_3")
			TCWebAPI_set("PortBind_Entry", "e4", "PortBind_Ethernet_Port_4")
		end if
		
		if request_Form("wlanISExist") = "On" then
			if request_Form("MBSSIDNumber") = "1" then
				TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
			elseif request_Form("MBSSIDNumber") = "2" then
				TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
				if request_Form("UserMode")="1" then
					if request_Form("wlanPort_1")="Yes" then
						TCWebAPI_set("PortBind_Entry", "w1", "wlanPort_1")
					end if
				else
					TCWebAPI_set("PortBind_Entry", "w1", "PortBind_Wireless_Port_1")
				end if	
			elseif request_Form("MBSSIDNumber") = "3" then
				TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
				if request_Form("UserMode")="1" then
					if request_Form("wlanPort_1")="Yes" then
						TCWebAPI_set("PortBind_Entry", "w1", "wlanPort_1")
					end if
					if request_Form("wlanPort_2")="Yes" then
						TCWebAPI_set("PortBind_Entry", "w2", "wlanPort_2")
					end if
				else
					TCWebAPI_set("PortBind_Entry", "w1", "PortBind_Wireless_Port_1")
					TCWebAPI_set("PortBind_Entry", "w2", "PortBind_Wireless_Port_2")
				end if
			elseif request_Form("MBSSIDNumber") = "4" then
				TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
				if request_Form("UserMode")="1" then
					if request_Form("wlanPort_1")="Yes" then
						TCWebAPI_set("PortBind_Entry", "w1", "wlanPort_1")
					end if
					if request_Form("wlanPort_2")="Yes" then
						TCWebAPI_set("PortBind_Entry", "w2", "wlanPort_2")
					end if
					if request_Form("wlanPort_3")="Yes" then
						TCWebAPI_set("PortBind_Entry", "w3", "wlanPort_3")
					end if
				else
					TCWebAPI_set("PortBind_Entry", "w1", "PortBind_Wireless_Port_1")
					TCWebAPI_set("PortBind_Entry", "w2", "PortBind_Wireless_Port_2")
					TCWebAPI_set("PortBind_Entry", "w3", "PortBind_Wireless_Port_3")
				end if
			end if
		end if
		
		TCWebAPI_set("PortBind_Entry", "u0", "PortBind_USB_Port_0")
		TCWebAPI_commit("PortBind_Entry")
		
	elseif request_Form("Add_or_Delete") = "2" then
		TCWebAPI_unset("PortBind_Entry")
		TCWebAPI_commit("PortBind_Entry")
	end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">

<script language="JavaScript">

function doPBChange()
{
	document.PortBind_form.submit();
}

function atmOnOffSwitch(on_off)
{
	var port;
	for (port = 0; port < 8; port++) {
		var pt = document.getElementById("PortBind_ATM_Port_" + port);
		var Tag = document.getElementById("PortBind_ATM_Tag_" + port);
		if (on_off == 0) {
			pt.disabled = true;
			continue;
		}
		if ((Tag.value == "Yes" && pt.checked) || Tag.value == "No") {
			pt.disabled = false;
		}
		else {
			pt.disabled = true;
		}
	}
}

function ptmOnOffSwitch(on_off)
{
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	var serv;
<%end if%>
	var port;	
	for (port = 0; port < 2; port++) {
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		for (serv = 0; serv < 8; serv++) {
			var pt = document.getElementById("PortBind_PTM_Port_" + port + "_" + serv);
			var Tag = document.getElementById("PortBind_PTM_Tag_" + port + "_" + serv);
<%else%>
		var pt = document.getElementById("PortBind_PTM_Port_" + port);
		var Tag = document.getElementById("PortBind_PTM_Tag_" + port);
<%end if%>		
		if((pt == null) || (Tag == null)){
			continue;	
		}
		if (on_off == 0) {
			pt.disabled = true;
			continue;
		}
		if ((Tag.value == "Yes" && pt.checked) || Tag.value == "No") {
			pt.disabled = false;
		}
		else {
			pt.disabled = true;
		}
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		}
<%end if%>		
	}
}

function wan0OnOffSwitch(on_off)
{
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
    var serv;
<%end if%>
	var port;
	for (port = 0; port < 1; port++) {
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
        for (serv = 0; serv < 8; serv++) {
			var pt = document.getElementById("PortBind_WAN0_Port_" + port + "_" + serv);
			var Tag = document.getElementById("PortBind_WAN0_Tag_" + port + "_" + serv);
<%else%>
		var pt = document.getElementById("PortBind_WAN0_Port_" + port);
		var Tag = document.getElementById("PortBind_WAN0_Tag_" + port);
<%end if%>	
		if((pt == null) || (Tag == null)){
			continue;	
		}
		if (on_off == 0) {
			pt.disabled = true;
			continue;
		}
		if ((Tag.value == "Yes" && pt.checked) || Tag.value == "No") {
			pt.disabled = false;
		}
		else {
			pt.disabled = true;
		}
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		}
<%end if%>		
	}
}

function etherOnOffSwitch(on_off)
{
	var port, portNum;
	if (document.PortBind_form.PB2PortsFlag.value == "Yes")
		portNum = 3;
	else
		portNum = 5;
		
	for (port = 1; port < portNum; port++) {
		var pt = document.getElementById("PortBind_Ethernet_Port_" + port);
		var Tag = document.getElementById("PortBind_Ethernet_Tag_" + port);
		if (on_off == 0) {
			pt.disabled = true;
			continue;
		}
		if ((Tag.value == "Yes" && pt.checked) || Tag.value == "No") {
			pt.disabled = false;
		}
		else {
			pt.disabled = true;
		}
	}
}

function wlanOnOffSwitch(on_off)
{
	var port;
	if(document.forms[0].UserMode.value == 1){
		var bssidNum = 1;
	}
	else{
		var bssidNum = document.getElementById("MBSSIDNumber").value;
	}
	if (document.getElementById("IDwlanISExist").value == "On") {
		for (port = 0; port < bssidNum; port++) {
			var pt = document.getElementById("PortBind_Wireless_Port_" + port);
			var Tag = document.getElementById("PortBind_Wireless_Tag_" + port);
			if (on_off == 0) {
				pt.disabled = true;
				continue;
			}
			if ((Tag.value == "Yes" && pt.checked) || Tag.value == "No") {
				pt.disabled = false;
			}
			else {
				pt.disabled = true;
			}
		}
	}
}

function usbSwitch(on_off)
{
	var port;
	for (port = 0; port < 1; port++) {
		var pt = document.getElementById("PortBind_USB_Port_" + port);
		var Tag = document.getElementById("PortBind_USB_Tag_" + port);
		if (on_off == 0) {
			pt.disabled = true;
			continue;
		}
		if ((Tag.value == "Yes" && pt.checked) || Tag.value == "No") {
			pt.disabled = false;
		}
		else {
			pt.disabled = true;
		}
	}
}

function init()
{
	if (document.PortBind_form.elements[0].checked) {
		document.getElementById("PortBind_Index").disabled = false;
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
		atmOnOffSwitch(1);
	<%end if%>

	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		ptmOnOffSwitch(1);
	<%end if%>

	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		wan0OnOffSwitch(1);
	<%end if%>

		etherOnOffSwitch(1);
		wlanOnOffSwitch(1);
<%if tcwebApi_get("Info_Ether","isUSBSupported","h")="Yes" then%>
		usbSwitch(1);
<%end if%>
	}
	else if (document.PortBind_form.elements[1].checked) {
		document.getElementById("PortBind_Index").disabled = true;
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
		atmOnOffSwitch(0);
	<%end if%>

	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		ptmOnOffSwitch(0);
	<%end if%>

	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		wan0OnOffSwitch(0);
	<%end if%>

		etherOnOffSwitch(0);
		wlanOnOffSwitch(0);
<%if tcwebApi_get("Info_Ether","isUSBSupported","h")="Yes" then%>
		usbSwitch(0);
<%end if%>
	}
}

function doAdd()
{
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	var serv;
<%end if%>
	/*

	if (document.PortBind_form.elements[1].checked) {
		return;
	}
	*/
	
	document.PortBind_form.Add_or_Delete.value = 1;
	
	/* check box value setting */
	var port, portNum;
	
	if (document.PortBind_form.PB2PortsFlag.value == "Yes")
		portNum = 3;
	else
		portNum = 5;
	
<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	/* atm port */
	for (port = 0; port < 8; port++) {
		var Tag = document.getElementById("PortBind_ATM_Port_" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.value = "No";
		}	
	}

<%end if%>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>

	/* ptm port */
	for (port = 0; port < 2; port++) {
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
        for (serv = 0; serv < 8; serv++) {
			var Tag = document.getElementById("PortBind_PTM_Port_" + port + "_" + serv);
<%else%>
		var Tag = document.getElementById("PortBind_PTM_Port_" + port);
<%end if%>
		if(Tag == null){
			continue;
		}	
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.vaue = "No";
		}
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	}
<%end if%>
	}
<%end if%>
	
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>

	/* wan0 port */
	for (port = 0; port < 1; port++) {
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
        for (serv = 0; serv < 8; serv++) {
			var Tag = document.getElementById("PortBind_WAN0_Port_" + port + "_" + serv);
<%else%>
		var Tag = document.getElementById("PortBind_WAN0_Port_" + port);
<%end if%>
		if(Tag == null){
			continue;
		}	
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.vaue = "No";
		}
		
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	}
<%end if%>
	}

<%end if%>
	
	/* ethernet port */
	for (port = 1; port < portNum; port++) {
		var Tag = document.getElementById("PortBind_Ethernet_Port_" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.value = "No";
		}
	}
	
	if(document.forms[0].UserMode.value == 1){
		var bssidNum = 1;
	}
	else{
		var bssidNum = document.getElementById("MBSSIDNumber").value;
	}
	/* wireless port */
	if (document.getElementById("IDwlanISExist").value == "On") {
		for (port = 0; port < bssidNum; port++) {
			var Tag = document.getElementById("PortBind_Wireless_Port_" + port);
			if (Tag.checked) {
				Tag.value = "Yes";
			}
			else {
				Tag.value = "No";
			}
		}
	}
	
	if(document.PortBind_form.usbSupport.value ==1){
	/* usb port */
		for (port = 0; port < 1; port++) {
			var Tag = document.getElementById("PortBind_USB_Port_" + port);
			if (Tag.checked) {
				Tag.value = "Yes";
			}
			else {
				Tag.value = "No";
			}
		}
	}
}

function onClickPortBindSummary()
{
	window.open("/cgi-bin/portbinding_table.asp","PortBind_List","toolbar=no,menubar=no,scrollbars=yes,height=400, width=600,location=0,left=100,top=100");
	return false;
}
</script>

</head><body leftmargin="20" topmargin="0" rightmargin="15" bottommargin="0" marginwidth="0" marginheight="0" onload="init()">

<FORM METHOD="POST" ACTION="/cgi-bin/adv_portbinding.asp" name="PortBind_form">
<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">Portbinding Group Setting</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td width="440"><hr noshade></td></tr>
	<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Active</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    	<INPUT TYPE="RADIO" NAME="PortBind_active" VALUE="Yes" <% if tcWebApi_get("PortBind_Common", "Active", "h") = "Yes" then asp_Write("checked") end if%> onclick="init()"><font color="#000000">Activated</font>
    	<INPUT TYPE="RADIO" NAME="PortBind_active" VALUE="No" <% if tcWebApi_get("PortBind_Common", "Active", "h") = "No" then asp_write("checked") end if%> onclick="init()"> <font color="#000000">Deactivated</font></td></tr><tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td width="10" class="light-orange"></td><td height="30" class="tabdata"><div align=right>
    <font color="#000000">Group Index</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <SELECT NAME="PortBind_Index" ID="PortBind_Index" SIZE="1" onChange="doPBChange()">
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "0" then asp_Write("selected") end if %>>0
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "1" then asp_Write("selected") end if %>>1
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "2" then asp_Write("selected") end if %>>2
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "3" then asp_Write("selected") end if %>>3
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "4" then asp_Write("selected") end if %>>4
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "5" then asp_Write("selected") end if %>>5
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "6" then asp_Write("selected") end if %>>6
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "7" then asp_Write("selected") end if %>>7
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "8" then asp_Write("selected") end if %>>8
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "9" then asp_Write("selected") end if %>>9
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "10" then asp_Write("selected") end if %>>10
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "11" then asp_Write("selected") end if %>>11
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "12" then asp_Write("selected") end if %>>12
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "13" then asp_Write("selected") end if %>>13
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "14" then asp_Write("selected") end if %>>14
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","portbind_id","h") = "15" then asp_Write("selected") end if %>>15
			</SELECT>    </td></tr><tr>
 <%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>

	<td width="150" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" class="tabdata"> <div align=right>
    	<font color="#000000">ATM VCs</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="80" class="tabdata">
      	<table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
            <tr> 
              <td rowspan="2" class="tabdata"><div align=right> <font color="#000000">Port 
                  #</font> </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_0" ID="PortBind_ATM_Port_0" <% if tcWebApi_get("PortBind_Entry", "p0", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_1" ID="PortBind_ATM_Port_1"<% if tcWebApi_get("PortBind_Entry", "p1", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_2" ID="PortBind_ATM_Port_2"<%if tcWebApi_get("PortBind_Entry", "p2", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_3" ID="PortBind_ATM_Port_3"<%if tcWebApi_get("PortBind_Entry", "p3", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_4" ID="PortBind_ATM_Port_4"<% if tcWebApi_get("PortBind_Entry", "p4", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_5" ID="PortBind_ATM_Port_5"<% if tcWebApi_get("PortBind_Entry", "p5", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_6" ID="PortBind_ATM_Port_6"<% if tcWebApi_get("PortBind_Entry", "p6", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
              <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_ATM_Port_7" ID="PortBind_ATM_Port_7"<% if tcWebApi_get("PortBind_Entry", "p7", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
            </tr>
            <tr class="tabdata"> 
              <td class="tabdata"><div align=center>0</div></td>
              <td class="tabdata"><div align=center>1</div></td>
              <td class="tabdata"><div align=center>2</div></td>
              <td class="tabdata"><div align=center>3</div></td>
              <td class="tabdata"><div align=center>4</div></td>
              <td class="tabdata"><div align=center>5</div></td>
              <td class="tabdata"><div align=center>6</div></td>
              <td class="tabdata"><div align=center>7</div></td>
            </tr>
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_0" ID="PortBind_ATM_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "p0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p0", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_1" ID="PortBind_ATM_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "p1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p1", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_2" ID="PortBind_ATM_Tag_2" VALUE="<%if tcWebApi_get("PortBind_Common", "p2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p2", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_3" ID="PortBind_ATM_Tag_3" VALUE="<%if tcWebApi_get("PortBind_Common", "p3", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p3", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_4" ID="PortBind_ATM_Tag_4" VALUE="<%if tcWebApi_get("PortBind_Common", "p4", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p4", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_5" ID="PortBind_ATM_Tag_5" VALUE="<%if tcWebApi_get("PortBind_Common", "p5", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p5", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_6" ID="PortBind_ATM_Tag_6" VALUE="<%if tcWebApi_get("PortBind_Common", "p6", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p6", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_ATM_Tag_7" ID="PortBind_ATM_Tag_7" VALUE="<%if tcWebApi_get("PortBind_Common", "p7", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p7", "s") end if%>">
          </table></td></tr><tr>
<%end if%>

 <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
 	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	    <td width="150" class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">PTM Barrier 0</font></div></td>
		<td class="tabdata"><div align=center>:</div></td><td width="440" height="80" class="tabdata">
      	<table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
        <tr> 
        	<td rowspan="2" class="tabdata"><div align=right> <font color="#000000">Port #</font></div></td>
				<%if tcWebApi_get("WanExt_PVC8e0","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_0" ID="PortBind_PTM_Port_0_0" <% if tcWebApi_get("PortBind_Entry", "p8_0", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC8e1","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_1" ID="PortBind_PTM_Port_0_1" <% if tcWebApi_get("PortBind_Entry", "p8_1", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC8e2","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_2" ID="PortBind_PTM_Port_0_2" <% if tcWebApi_get("PortBind_Entry", "p8_2", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC8e3","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_3" ID="PortBind_PTM_Port_0_3" <% if tcWebApi_get("PortBind_Entry", "p8_3", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC8e4","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_4" ID="PortBind_PTM_Port_0_4" <% if tcWebApi_get("PortBind_Entry", "p8_4", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC8e5","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_5" ID="PortBind_PTM_Port_0_5" <% if tcWebApi_get("PortBind_Entry", "p8_5", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC8e6","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_6" ID="PortBind_PTM_Port_0_6" <% if tcWebApi_get("PortBind_Entry", "p8_6", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC8e7","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0_7" ID="PortBind_PTM_Port_0_7" <% if tcWebApi_get("PortBind_Entry", "p8_7", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
 	    <tr class="tabdata"> 
			<%if tcWebApi_get("WanExt_PVC8e0","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>0</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e1","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>1</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e2","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>2</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e3","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>3</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e4","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>4</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e5","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>5</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e6","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>6</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e7","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>7</div></td>
			<% end if %>
        </tr>
			<%if tcWebApi_get("WanExt_PVC8e0","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_0" ID="PortBind_PTM_Tag_0_0" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_0", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e1","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_1" ID="PortBind_PTM_Tag_0_1" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_1", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e2","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_2" ID="PortBind_PTM_Tag_0_2" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_2", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e3","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_3" ID="PortBind_PTM_Tag_0_3" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_3", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_3", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e4","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_4" ID="PortBind_PTM_Tag_0_4" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_4", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_4", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e5","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_5" ID="PortBind_PTM_Tag_0_5" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_5", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_5", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e6","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_6" ID="PortBind_PTM_Tag_0_6" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_6", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_6", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC8e7","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0_7" ID="PortBind_PTM_Tag_0_7" VALUE="<%if tcWebApi_get("PortBind_Common", "p8_7", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8_7", "s") end if%>">
			<% end if %>
	   </table></td></tr><tr>

	    <td width="150" class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">PTM Barrier 1</font></div></td>
		<td class="tabdata"><div align=center>:</div></td><td width="440" height="80" class="tabdata">
      	<table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
        <tr> 
        	<td rowspan="2" class="tabdata"><div align=right> <font color="#000000">Port #</font></div></td>
				<%if tcWebApi_get("WanExt_PVC9e0","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_0" ID="PortBind_PTM_Port_1_0" <% if tcWebApi_get("PortBind_Entry", "p9_0", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC9e1","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_1" ID="PortBind_PTM_Port_1_1" <% if tcWebApi_get("PortBind_Entry", "p9_1", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC9e2","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_2" ID="PortBind_PTM_Port_1_2" <% if tcWebApi_get("PortBind_Entry", "p9_2", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC9e3","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_3" ID="PortBind_PTM_Port_1_3" <% if tcWebApi_get("PortBind_Entry", "p9_3", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC9e4","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_4" ID="PortBind_PTM_Port_1_4" <% if tcWebApi_get("PortBind_Entry", "p9_4", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC9e5","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_5" ID="PortBind_PTM_Port_1_5" <% if tcWebApi_get("PortBind_Entry", "p9_5", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC9e6","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_6" ID="PortBind_PTM_Port_1_6" <% if tcWebApi_get("PortBind_Entry", "p9_6", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC9e7","Active","h") = "Yes" then%>
            		<td><div align=center> 
                		<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1_7" ID="PortBind_PTM_Port_1_7" <% if tcWebApi_get("PortBind_Entry", "p1_7", "h") = "Yes" then asp_write("checked") end if%> >
                	</div></td>
				<%end if%>
 	    <tr class="tabdata"> 
			<%if tcWebApi_get("WanExt_PVC9e0","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>0</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e1","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>1</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e2","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>2</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e3","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>3</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e4","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>4</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e5","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>5</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e6","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>6</div></td>
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e7","Active","h") = "Yes" then%>
				<td class="tabdata"><div align=center>7</div></td>
			<% end if %>
        </tr>
			<%if tcWebApi_get("WanExt_PVC9e0","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_0" ID="PortBind_PTM_Tag_1_0" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_0", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e1","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_1" ID="PortBind_PTM_Tag_1_1" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_1", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e2","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_2" ID="PortBind_PTM_Tag_1_2" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_2", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e3","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_3" ID="PortBind_PTM_Tag_1_3" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_3", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_3", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e4","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_4" ID="PortBind_PTM_Tag_1_4" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_4", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_4", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e5","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_5" ID="PortBind_PTM_Tag_1_5" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_5", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_5", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e6","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_6" ID="PortBind_PTM_Tag_1_6" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_6", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_6", "s") end if%>">
			<% end if %>
			<%if tcWebApi_get("WanExt_PVC9e7","Active","h") = "Yes" then%>
				<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1_7" ID="PortBind_PTM_Tag_1_7" VALUE="<%if tcWebApi_get("PortBind_Common", "p9_7", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9_7", "s") end if%>">
			<% end if %>
	   </table></td></tr><tr>
	<%else%>
	    <td width="150" class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">PTM </font></div></td>
		<td class="tabdata"><div align=center>:</div></td><td width="440" height="80" class="tabdata">
      	<table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
        <tr> 
        	<td rowspan="2" class="tabdata"><div align=right> <font color="#000000">Port #</font></div></td>
            	<td><div align=center> 
                	<INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_0" ID="PortBind_PTM_Port_0" <% if tcWebApi_get("PortBind_Entry", "p8", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
             <td><div align=center> 
                  <INPUT TYPE="CHECKBOX" NAME="PortBind_PTM_Port_1" ID="PortBind_ATM_Port_1"<% if tcWebApi_get("PortBind_Entry", "p9", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
 	    <tr class="tabdata"> 
              <td class="tabdata"><div align=center>0</div></td>
              <td class="tabdata"><div align=center>1</div></td>
        </tr>
			<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_0" ID="PortBind_PTM_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "p8", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p8", "s") end if%>">
			<INPUT TYPE="HIDDEN" NAME="PortBind_PTM_Tag_1" ID="PortBind_PTM_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "p9", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p9", "s") end if%>">
	   </table></td></tr><tr>
	   <%end if%>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	    <td width="150" class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td width="150" class="tabdata"> <div align=right><font color="#000000">WAN0</font></div></td>
		<td class="tabdata"><div align=center>:</div></td><td width="440" height="80" class="tabdata">
      	<table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
        <tr> 
        	<td rowspan="2" class="tabdata"><div align=right> <font color="#000000">Port #</font> </div></td>
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WanExt_PVC10e0","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_0" ID="PortBind_WAN0_Port_0_0" <% if tcWebApi_get("PortBind_Entry", "p10_0", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e1","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_1" ID="PortBind_WAN0_Port_0_1" <% if tcWebApi_get("PortBind_Entry", "p10_1", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e2","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_2" ID="PortBind_WAN0_Port_0_2" <% if tcWebApi_get("PortBind_Entry", "p10_2", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e3","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_3" ID="PortBind_WAN0_Port_0_3" <% if tcWebApi_get("PortBind_Entry", "p10_3", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e4","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_4" ID="PortBind_WAN0_Port_0_4" <% if tcWebApi_get("PortBind_Entry", "p10_4", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e5","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_5" ID="PortBind_WAN0_Port_0_5" <% if tcWebApi_get("PortBind_Entry", "p10_5", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e6","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_6" ID="PortBind_WAN0_Port_0_6" <% if tcWebApi_get("PortBind_Entry", "p10_6", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e7","Active","h") = "Yes" then%>
					<td><div align=center>
						<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0_7" ID="PortBind_WAN0_Port_0_7" <% if tcWebApi_get("PortBind_Entry", "p10_7", "h") = "Yes" then asp_write("checked") end if%> >
					</div></td>	
				<%end if%>
			<% else %>
            	<td><div align=center> 
            		<INPUT TYPE="CHECKBOX" NAME="PortBind_WAN0_Port_0" ID="PortBind_WAN0_Port_0" <% if tcWebApi_get("PortBind_Entry", "p10", "h") = "Yes" then asp_write("checked") end if%> >
                </div></td>
			<%end if%>	
 	        <tr class="tabdata"> 
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WanExt_PVC10e0","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>0</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e1","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>1</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e2","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>2</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e3","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>3</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e4","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>4</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e5","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>5</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e6","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>6</div></td>
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e7","Active","h") = "Yes" then%>
					<td class="tabdata"><div align=center>7</div></td>
				<%end if%>
			<% else %>
            	<td class="tabdata"><div align=center>0</div></td>
			<%end if%>	
         	</tr>
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WanExt_PVC10e0","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_0" ID="PortBind_WAN0_Tag_0_0" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_0", "s") end if%>">
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e1","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_1" ID="PortBind_WAN0_Tag_0_1" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_1", "s") end if%>">
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e2","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_2" ID="PortBind_WAN0_Tag_0_2" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_2", "s") end if%>">
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e3","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_3" ID="PortBind_WAN0_Tag_0_3" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_3", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_3", "s") end if%>">
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e4","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_4" ID="PortBind_WAN0_Tag_0_4" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_4", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_4", "s") end if%>">
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e5","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_5" ID="PortBind_WAN0_Tag_0_5" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_5", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_5", "s") end if%>">
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e6","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_6" ID="PortBind_WAN0_Tag_0_6" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_6", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_6", "s") end if%>">
				<%end if%>
				<%if tcWebApi_get("WanExt_PVC10e7","Active","h") = "Yes" then%>
					<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0_7" ID="PortBind_WAN0_Tag_0_7" VALUE="<%if tcWebApi_get("PortBind_Common", "p10_7", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10_7", "s") end if%>">
				<%end if%>
			<% else %>
				<INPUT TYPE="HIDDEN" NAME="PortBind_WAN0_Tag_0" ID="PortBind_WAN0_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "p10", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "p10", "s") end if%>">
       		<%end if%>
	   </table></td></tr><tr>

<%end if%>

    <td width="150" height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" height="45" class="tabdata"> <div align=right>
    <font color="#000000">Ethernet</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="70" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Ethernet_Port_1" ID="PortBind_Ethernet_Port_1" <%if tcWebApi_get("PortBind_Entry", "e1", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>        <td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Ethernet_Port_2" ID="PortBind_Ethernet_Port_2" <%if tcWebApi_get("PortBind_Entry", "e2", "h") = "Yes" then asp_write("checked") end if%> >            </div></td><% if tcWebApi_get("WebCustom_Entry","is2PSupported","h") <> "Yes" then %><td><div align=center>
			<INPUT TYPE="CHECKBOX" NAME="PortBind_Ethernet_Port_3" ID="PortBind_Ethernet_Port_3" <%if tcWebApi_get("PortBind_Entry", "e3", "h") = "Yes" then asp_write("checked") end if%> >            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Ethernet_Port_4" ID="PortBind_Ethernet_Port_4" <%if tcWebApi_get("PortBind_Entry", "e4", "h") = "Yes" then asp_write("checked") end if%> >            </div></td><%end if%>      </tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        
		<td class="tabdata"><div align=center>2</div></td>
		<% if tcWebApi_get("WebCustom_Entry","is2PSupported","h") <> "Yes" then %>
		<td class="tabdata"><div align=center>3</div></td>
		<td class="tabdata"><div align=center>4</div></td>   
		<% end if%>   
		</tr>
		<INPUT TYPE="HIDDEN" NAME="PortBind_Ethernet_Tag_1" ID="PortBind_Ethernet_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "e1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "e1", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Ethernet_Tag_2" ID="PortBind_Ethernet_Tag_2" VALUE="<%if tcWebApi_get("PortBind_Common", "e2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "e2", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Ethernet_Tag_3" ID="PortBind_Ethernet_Tag_3" VALUE="<%if tcWebApi_get("PortBind_Common", "e3", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "e3", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Ethernet_Tag_4" ID="PortBind_Ethernet_Tag_4" VALUE="<%if tcWebApi_get("PortBind_Common", "e4", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "e4", "s") end if%>">
		</table></td></tr>
<% if tcWebApi_get("Info_WLan","isExist","h") = "On" then %>
<%
	if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then
%>
		<tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" height="45" class="tabdata"> <div align=right>
    <font color="#000000">WLan</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="70" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_0" ID="PortBind_Wireless_Port_0" <%if tcWebApi_get("PortBind_Entry", "w0", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>  </tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        
		 </tr>
		 <INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_0" ID="PortBind_Wireless_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "w0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w0", "s") end if%>">
		 </table></td></tr> 
<%
	elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "2" then
%>
		<tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" height="45" class="tabdata"> <div align=right>
    <font color="#000000">WLan</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="70" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_0" ID="PortBind_Wireless_Port_0" <%if tcWebApi_get("PortBind_Entry", "w0", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
<%if tcWebApi_get("WebCustom_Entry","isCZTrueSupported","h")<>"Yes" then%>
			<td><div align=center>
		    	<INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_1" ID="PortBind_Wireless_Port_1" <%if tcWebApi_get("PortBind_Entry", "w1", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
<%end if%>
 		</tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        
<%if tcWebApi_get("WebCustom_Entry","isCZTrueSupported","h")<>"Yes" then%>     
		<td class="tabdata"><div align=center>2</div></td>
<% end if%> 
		 </tr>
		 <INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_0" ID="PortBind_Wireless_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "w0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w0", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_1" ID="PortBind_Wireless_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "w1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w1", "s") end if%>">
		 </table></td></tr>  
<%
	elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "3" then
%>
		<tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" height="45" class="tabdata"> <div align=right>
    <font color="#000000">WLan</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="70" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_0" ID="PortBind_Wireless_Port_0" <%if tcWebApi_get("PortBind_Entry", "w0", "h") = "Yes" then asp_write("checked") end if%> value="" >            </div></td>
<%if tcWebApi_get("WebCustom_Entry","isCZTrueSupported","h")<>"Yes" then%>
			<td><div align=center>
		    	<INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_1" ID="PortBind_Wireless_Port_1" <%if tcWebApi_get("PortBind_Entry", "w1", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
			<td><div align=center>
            	<INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_2" ID="PortBind_Wireless_Port_2" <%if tcWebApi_get("PortBind_Entry", "w2", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
<%end if%>
	 	</tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        
<%if tcWebApi_get("WebCustom_Entry","isCZTrueSupported","h")<>"Yes" then%>      
		<td class="tabdata"><div align=center>2</div></td>
		<td class="tabdata"><div align=center>3</div></td>
<% end if%> 
		 </tr>
		 <INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_0" ID="PortBind_Wireless_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "w0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w0", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_1" ID="PortBind_Wireless_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "w1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w1", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_2" ID="PortBind_Wireless_Tag_2" VALUE="<%if tcWebApi_get("PortBind_Common", "w2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w2", "s") end if%>">
		 </table></td></tr>  
<%
	elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "4" then
%>
		<tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" height="45" class="tabdata"> <div align=right>
    <font color="#000000">WLan</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="70" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td>
		<td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_0" ID="PortBind_Wireless_Port_0" <%if tcWebApi_get("PortBind_Entry", "w0", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
<%if tcWebApi_get("WebCustom_Entry","isCZTrueSupported","h")<>"Yes" then%>
			<td><div align=center>
		   		<INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_1" ID="PortBind_Wireless_Port_1" <%if tcWebApi_get("PortBind_Entry", "w1", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
			<td><div align=center>
            	<INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_2" ID="PortBind_Wireless_Port_2" <%if tcWebApi_get("PortBind_Entry", "w2", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
			<td><div align=center>
            	<INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_3" ID="PortBind_Wireless_Port_3" <%if tcWebApi_get("PortBind_Entry", "w3", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>
<%end if%>
		</tr>
		<tr>
        <td class="tabdata"><div align=center>1</div></td>        
<%if tcWebApi_get("WebCustom_Entry","isCZTrueSupported","h")<>"Yes" then%>       
		<td class="tabdata"><div align=center>2</div></td>
		<td class="tabdata"><div align=center>3</div></td>
		<td class="tabdata"><div align=center>4</div></td>     
<%end if%> 
		 </tr>
		 <INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_0" ID="PortBind_Wireless_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "w0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w0", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_1" ID="PortBind_Wireless_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "w1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w1", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_2" ID="PortBind_Wireless_Tag_2" VALUE="<%if tcWebApi_get("PortBind_Common", "w2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w2", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_3" ID="PortBind_Wireless_Tag_3" VALUE="<%if tcWebApi_get("PortBind_Common", "w3", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w3", "s") end if%>">
		 </table></td></tr>  
<%end if%>
<%end if%>
<%if tcwebApi_get("Info_Ether","isUSBSupported","h")="Yes" then%>
		<tr>
    <td height="25" class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td height="80" class="tabdata">
      <div align=right>
      <font color="#000000">USB</font>      </div></td><td class="tabdata"><div align=center>:</div></td><td height="25" class="tabdata">
	  <table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
        <tr class="tabdata">
        <td rowspan="2" class="tabdata">
          <div align=right>
          <font color="#000000">Port #</font>          
		  </div></td>
		  <td>
		  <div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_USB_Port_0" ID="PortBind_USB_Port_0" <%if tcWebApi_get("PortBind_Entry", "u0", "h") = "Yes" then asp_write("checked") end if%> >       
		 </div>
		 </td>
		 </tr>
		<tr>
        <td class="tabdata"><div align=center>1</div></td>
		</tr>
			<INPUT TYPE="HIDDEN" NAME="PortBind_USB_Tag_0" ID="PortBind_USB_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "u0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "u0", "s") end if%>">
		</table>
	  </td>
	  </tr> 
<%end if%>
	  <tr>
    <td height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td height="30">&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  		<tr>
        <td width="150" height="30" class="title-sub"> <font color="#000000"> 
          Group Summary</font> </td>
        <td width="10" class="light-orange">&nbsp;</td>
		<td width="150"> </td>
		<td class="tabdata">				
				<INPUT TYPE="BUTTON" NAME="PortBind_Summary" VALUE="PortBinding Summary" onClick="onClickPortBindSummary();">				  
		</td>
  </table>	
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<INPUT TYPE="SUBMIT" NAME="SaveBtn_PortBind_group" VALUE="SAVE" onClick="doAdd();">
	<INPUT TYPE="SUBMIT" NAME="DeleteBtn_PortBind_group" VALUE="DELETE" onclick="document.PortBind_form.Add_or_Delete.value = 2">
	<input type="SUBMIT"  name="PortBind_group_cancel" value="CANCEL" onClick="document.PortBind_form.Add_or_Delete.value = 3">
	</td></tr></table></div>
	<INPUT TYPE="HIDDEN" NAME="ICIndex_Group" VALUE="1">
	<INPUT TYPE="HIDDEN" NAME="Add_or_Delete" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="MBSSIDNumber" ID="MBSSIDNumber" VALUE="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">
	<INPUT TYPE="HIDDEN" NAME="PB2PortsFlag" VALUE="<%tcWebApi_get("WebCustom_Entry","is2PSupported","s")%>">
	<INPUT TYPE="HIDDEN" NAME="wlanISExist" ID="IDwlanISExist" VALUE="<%tcWebApi_get("Info_WLan","isExist","s")%>">
	<Input type="hidden" name="UserMode" value="<%if tcWebApi_get("WebCustom_Entry","isCZTrueSupported","h")<>"Yes" then asp_write("0") else asp_write("1") end if%>">
	<INPUT TYPE="HIDDEN" NAME="ssid2Value" VALUE="<%tcWebApi_get("PortBind_Entry1","isExist","s")%>">
	<INPUT TYPE="HIDDEN" NAME="ssid3Value"  VALUE="<%tcWebApi_get("PortBind_Entry2","isExist","s")%>">
	<INPUT TYPE="HIDDEN" NAME="ssid4Value"  VALUE="<%tcWebApi_get("PortBind_Entry3","isExist","s")%>">
	<INPUT TYPE="HIDDEN" NAME="usbSupport" value="<% if tcwebApi_get("Info_Ether","isUSBSupported","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
	<Input type="hidden" Name="wlanPort_1" value="<%if tcWebApi_get("PortBind_Entry", "w1", "h") = "Yes" then asp_write("Yes") else asp_write("No") end if %>">
	<Input type="hidden" Name="wlanPort_2" value="<%if tcWebApi_get("PortBind_Entry", "w2", "h") = "Yes" then asp_write("Yes") else asp_write("No") end if %>">
	<Input type="hidden" Name="wlanPort_3" value="<%if tcWebApi_get("PortBind_Entry", "w3", "h") = "Yes" then asp_write("Yes") else asp_write("No") end if %>">
	</form>
</body></html>
