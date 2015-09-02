<%
	request_Form("PortBind_Index")
	TCWebAPI_set("WebCurSet_Entry","portbind_id","PortBind_Index")
	TCWebAPI_set("PortBind_Common", "Active", "PortBind_active")
	if request_Form("Add_or_Delete") = "1" then
		TCWebAPI_unset("PortBind_Entry")
		
		TCWebAPI_set("PortBind_Entry", "p0", "PortBind_ATM_Port_0")
		TCWebAPI_set("PortBind_Entry", "p1", "PortBind_ATM_Port_1")
		TCWebAPI_set("PortBind_Entry", "p2", "PortBind_ATM_Port_2")
		TCWebAPI_set("PortBind_Entry", "p3", "PortBind_ATM_Port_3")
		TCWebAPI_set("PortBind_Entry", "p4", "PortBind_ATM_Port_4")
		TCWebAPI_set("PortBind_Entry", "p5", "PortBind_ATM_Port_5")
		TCWebAPI_set("PortBind_Entry", "p6", "PortBind_ATM_Port_6")
		TCWebAPI_set("PortBind_Entry", "p7", "PortBind_ATM_Port_7")
		
		TCWebAPI_set("PortBind_Entry", "e0", "PortBind_Ethernet_Port_1")
		
		if request_Form("wlanISExist") = "On" then
		if request_Form("MBSSIDNumber") = "1" then
			TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
		elseif request_Form("MBSSIDNumber") = "2" then
			TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
			TCWebAPI_set("PortBind_Entry", "w1", "PortBind_Wireless_Port_1")
		elseif request_Form("MBSSIDNumber") = "3" then
			TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
			TCWebAPI_set("PortBind_Entry", "w1", "PortBind_Wireless_Port_1")
			TCWebAPI_set("PortBind_Entry", "w2", "PortBind_Wireless_Port_2")
		elseif request_Form("MBSSIDNumber") = "4" then
			TCWebAPI_set("PortBind_Entry", "w0", "PortBind_Wireless_Port_0")
			TCWebAPI_set("PortBind_Entry", "w1", "PortBind_Wireless_Port_1")
			TCWebAPI_set("PortBind_Entry", "w2", "PortBind_Wireless_Port_2")
			TCWebAPI_set("PortBind_Entry", "w3", "PortBind_Wireless_Port_3")
		end if
		end if
		
		TCWebAPI_set("PortBind_Entry", "u0", "PortBind_USB_Port_0")
	elseif request_Form("Add_or_Delete") = "2" then
		TCWebAPI_unset("PortBind_Entry")
	end if
	TCWebAPI_commit("PortBind_Entry")
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

function etherOnOffSwitch(on_off)
{
	var port;
	for (port = 1; port < 2; port++) {
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
	var bssidNum = document.getElementById("MBSSIDNumber").value;
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
		atmOnOffSwitch(1);
		etherOnOffSwitch(1);
		wlanOnOffSwitch(1);
		usbSwitch(1);
	}
	else if (document.PortBind_form.elements[1].checked) {
		document.getElementById("PortBind_Index").disabled = true;
		atmOnOffSwitch(0);
		etherOnOffSwitch(0);
		wlanOnOffSwitch(0);
		usbSwitch(0);
	}
}

function doAdd()
{
	if (document.PortBind_form.elements[1].checked) {
		return;
	}
	
	document.PortBind_form.Add_or_Delete.value = 1;
	
	/* check box value setting */
	var port;
	
	/* atm port */
	for (port = 0; port < 8; port++) {
		var Tag = document.getElementById("PortBind_ATM_Port_" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.vaue = "No";
		}
	}
	
	/* ethernet port */
	for (port = 1; port < 2; port++) {
		var Tag = document.getElementById("PortBind_Ethernet_Port_" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.vaue = "No";
		}
	}
	
	var bssidNum = document.getElementById("MBSSIDNumber").value;
	/* wireless port */
	if (document.getElementById("IDwlanISExist").value == "On") {
	for (port = 0; port < bssidNum; port++) {
		var Tag = document.getElementById("PortBind_Wireless_Port_" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.vaue = "No";
			}
		}
	}
	
	/* usb port */
	for (port = 0; port < 1; port++) {
		var Tag = document.getElementById("PortBind_USB_Port_" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.vaue = "No";
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
    <td width="150" height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" height="45" class="tabdata"> <div align=right>
    <font color="#000000">Ethernet</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="70" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Ethernet_Port_1" ID="PortBind_Ethernet_Port_1" <%if tcWebApi_get("PortBind_Entry", "e0", "h") = "Yes" then asp_write("checked") end if%> >            </div></td> </tr><tr>
        <td class="tabdata"><div align=center>1</div></td>         
		</tr>
		<INPUT TYPE="HIDDEN" NAME="PortBind_Ethernet_Tag_1" ID="PortBind_Ethernet_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "e0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "e0", "s") end if%>">
		</table></td></tr>
<% if tcWebApi_get("Info_WLan","isExist","h") = "On" then %>		
<% if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then %>
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
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_0" ID="PortBind_Wireless_Port_0" <%if tcWebApi_get("PortBind_Entry", "w0", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>        <td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_1" ID="PortBind_Wireless_Port_1" <%if tcWebApi_get("PortBind_Entry", "w1", "h") = "Yes" then asp_write("checked") end if%> >            </div></td> </tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        
		<td class="tabdata"><div align=center>2</div></td>
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
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_0" ID="PortBind_Wireless_Port_0" <%if tcWebApi_get("PortBind_Entry", "w0", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>        <td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_1" ID="PortBind_Wireless_Port_1" <%if tcWebApi_get("PortBind_Entry", "w1", "h") = "Yes" then asp_write("checked") end if%> >            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_2" ID="PortBind_Wireless_Port_2" <%if tcWebApi_get("PortBind_Entry", "w2", "h") = "Yes" then asp_write("checked") end if%> >            </div></td> </tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        
		<td class="tabdata"><div align=center>2</div></td>
		<td class="tabdata"><div align=center>3</div></td>
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
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_0" ID="PortBind_Wireless_Port_0" <%if tcWebApi_get("PortBind_Entry", "w0", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>        <td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_1" ID="PortBind_Wireless_Port_1" <%if tcWebApi_get("PortBind_Entry", "w1", "h") = "Yes" then asp_write("checked") end if%> >            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_2" ID="PortBind_Wireless_Port_2" <%if tcWebApi_get("PortBind_Entry", "w2", "h") = "Yes" then asp_write("checked") end if%> >            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="PortBind_Wireless_Port_3" ID="PortBind_Wireless_Port_3" <%if tcWebApi_get("PortBind_Entry", "w3", "h") = "Yes" then asp_write("checked") end if%> >            </div></td>      </tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        
		<td class="tabdata"><div align=center>2</div></td>
		<td class="tabdata"><div align=center>3</div></td>
		<td class="tabdata"><div align=center>4</div></td>     
		 </tr>
		 <INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_0" ID="PortBind_Wireless_Tag_0" VALUE="<%if tcWebApi_get("PortBind_Common", "w0", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w0", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_1" ID="PortBind_Wireless_Tag_1" VALUE="<%if tcWebApi_get("PortBind_Common", "w1", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w1", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_2" ID="PortBind_Wireless_Tag_2" VALUE="<%if tcWebApi_get("PortBind_Common", "w2", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w2", "s") end if%>">
		<INPUT TYPE="HIDDEN" NAME="PortBind_Wireless_Tag_3" ID="PortBind_Wireless_Tag_3" VALUE="<%if tcWebApi_get("PortBind_Common", "w3", "h") <> "N/A" then tcWebApi_get("PortBind_Common", "w3", "s") end if%>">
		 </table></td></tr>  
<%end if%>
<%end if%>
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
	<INPUT TYPE="SUBMIT" NAME="SaveBtn_PortBind_group" VALUE="ADD" onClick="doAdd();">
	<INPUT TYPE="SUBMIT" NAME="DeleteBtn_PortBind_group" VALUE="DELETE" onclick="document.PortBind_form.Add_or_Delete.value = 2">
	<input type="reset"  name="PortBind_group_cancel" value="CANCEL">
	</td></tr></table></div>
	<INPUT TYPE="HIDDEN" NAME="ICIndex_Group" VALUE="1">
	<INPUT TYPE="HIDDEN" NAME="Add_or_Delete" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="MBSSIDNumber" ID="MBSSIDNumber" VALUE="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">
	<INPUT TYPE="HIDDEN" NAME="wlanISExist" ID="IDwlanISExist" VALUE="<%tcWebApi_get("Info_WLan","isExist","s")%>">
	</form>
</body></html>
