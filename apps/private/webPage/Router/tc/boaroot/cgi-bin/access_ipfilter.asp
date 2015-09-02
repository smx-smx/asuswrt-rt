<%
if request_Form("Duplicate")="1" then
	TCWebAPI_unset("IpMacFilter_Entry")
	TCWebAPI_commit("IpMacFilter_Entry")
	tcWebApi_set("IpMacFilter_Common","Duplicate","NoDup")
end if
/*select a rule*/	
if request_Form("RuleTypeChange") = "0" then
	TCWebAPI_set("WebCurSet_Entry","ipfilter_id","RuleIndexSEL")
	tcwebApi_set("GUITemp_Entry0","IndexTmp","RuleIndexSEL")
/*add a rule*/
elseif request_Form("RuleTypeChange") = "1" then
	if request_Form("c2Support") <> "1" then
		TCWebAPI_set("IpMacFilter_Common","ListType","RuleTypeSEL")
	end if
	tcwebApi_set("GUITemp_Entry0","IndexTmp","RuleIndexSEL")
	TCWebAPI_set("WebCurSet_Entry","ipfilter_id","RuleIndexSEL")
	/*remove original data*/
	TCWebAPI_unset("IpMacFilter_Entry")
	
  TCWebAPI_set("IpMacFilter_Entry","Active","RuleActiveRDO")

if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
	if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then
		if tcWebApi_get("WebCustom_Entry", "filter_interface", "h") = "PVC8" then
			TCWebAPI_set("IpMacFilter_Entry","Interface","ServiceID")
		elseif tcWebApi_get("WebCustom_Entry", "filter_interface", "h") = "PVC9" then	
			TCWebAPI_set("IpMacFilter_Entry","Interface","ServiceID")
		elseif tcWebApi_get("WebCustom_Entry", "filter_interface", "h") = "PVC10" then	
			TCWebAPI_set("IpMacFilter_Entry","Interface","ServiceID")
		else	
			TCWebAPI_set("IpMacFilter_Entry","Interface","InterfaceSEL")
		end if	
	else
		TCWebAPI_set("IpMacFilter_Entry","Interface","InterfaceSEL")
	end if
else
	TCWebAPI_set("IpMacFilter_Entry","Interface","InterfaceSEL")
end if		

  TCWebAPI_set("IpMacFilter_Entry","Direction","DirectionSEL")
 	TCWebAPI_set("IpMacFilter_Entry","RuleType","FILTERRuleTypeSEL")
 	
  TCWebAPI_set("IpMacFilter_Entry","SrcIPAddr","SrcIPTXT")
  TCWebAPI_set("IpMacFilter_Entry","SrcIPMask","SrcMaskTXT")
  TCWebAPI_set("IpMacFilter_Entry","SrcPort","SrcPortTXT")
  TCWebAPI_set("IpMacFilter_Entry","DesIPAddr","DestIPTXT")
  TCWebAPI_set("IpMacFilter_Entry","DesIPMask","DestMaskTXT")
  TCWebAPI_set("IpMacFilter_Entry","DesPort","DestPortTXT")
  if request_Form("DSCPFLT") = "Yes" then
  TCWebAPI_set("IpMacFilter_Entry","DSCP","DSCPTXT")
  end if
  TCWebAPI_set("IpMacFilter_Entry","Protocol","ProtocolSEL")

  TCWebAPI_set("IpMacFilter_Entry","MacAddr","MacAddrTXT")  

  /*commit changes*/
  TCWebAPI_commit("IpMacFilter_Entry")
/*remove a rule*/
elseif request_Form("RuleTypeChange") = "2" then
	TCWebAPI_unset("IpMacFilter_Entry")
	TCWebAPI_commit("IpMacFilter_Entry")
/*change rule type*/
elseif request_Form("RuleTypeChange") = "3" then
	TCWebAPI_set("WebCurSet_Entry","ipfilter_id","RuleIndexSEL")
	tcwebApi_set("GUITemp_Entry0","IndexTmp","RuleIndexSEL")
	TCWebAPI_set("IpMacFilter_Entry","RuleType","FILTERRuleTypeSEL")
	
  /*commit changes*/
end if  

if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then
if request_Form("RuleTypeChange") = "5" then
  	TCWebAPI_set("WebCustom_Entry","filter_interface","InterfaceSEL")
end if
end if
end if

%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">

<SCRIPT language=javascript>

function tableShow(id,header,data,keyIndex){
			var html = ["<table width=560 border=1 align=center cellpadding=1 cellspacing=0  bordercolor=#CCCCCC bgcolor=#FFFFFF>"];
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
	document.getElementById(id).innerHTML = html.join('');
}

<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
function doVCChange(){
	document.forms[0].RuleTypeChange.value = 5;
	document.forms[0].submit();
	return;
}
<%End If%>
<%End If%>

function doIndexChange()
{
	document.forms[0].RuleTypeChange.value = 0;
	document.forms[0].submit();
	return;
}

function doAdd()
{
	document.forms[0].RuleTypeChange.value = 1;
	<%if tcwebApi_get("WebCustom_Entry","isC2TrueSupported","h") = "Yes" then %>
	document.forms[0].c2Support.value = 1;
	<%end if%>
	
}

function doDel()
{
	document.forms[0].RuleTypeChange.value = 2;
}

function doChangeRuleType()
{
	if(document.IPFILTERform.FILTERRuleTypeSEL.selectedIndex == 0)
	{
		document.getElementById("divMac").style.display="none";
		document.getElementById("divIP").style.display="block";		
	}
	else
	{
		document.getElementById("divIP").style.display="none";
		document.getElementById("divMac").style.display="block";
	}
	return;
}

function doCancel()
{
	document.forms[0].RuleTypeChange.value = 4;
	javascript:window.location='access_ipfilter.asp'
}
	
function switchFilterType(object)
{
	var index = object.selectedIndex;
	switch(index)
	{
		case 1:
			window.location='access_appfilter.asp';
			break;
		case 2:
			window.location='access_URLfilter.asp';
			break;
	}
}

function isValidIpAddr(ip1,ip2,ip3,ip4)
{
	if(ip1==0 || ip4==255 || ip1==127 || ip4==0)
	{
		if(ip1==0 && ip2==0 && ip3==0 && ip4==0)
			return true;
		else
			return false;
	}
	return true;	
}

function unValidIP(Address)
{
	var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var i;

	var test=0;
	var point=-1;
	while(test!=-1)
	{
		point++;
		test=Address.indexOf(".",test+1);	
	}

	if(point<3)
	{
		alert("IP address is empty or wrong format!");
		return true;
	}

	if(address == null) 
	{ 
		alert("IP address is empty or wrong format!");
		return true;
	}
	else
	{
		digits = address[0].split(".");
		for(i=0; i < 4; i++)
		{
			if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
			{ 
				alert("Invalid IP address: " + Address);
				return true;
			}
		}
		if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3]))	
		{
			alert("Invalid IP address: " + Address);
			return true;
		}
		return false;
	}
}

function unValidMask(Mask)
{
	var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var bMask = 0;
	var watch = false;
	var i;
  
	var test=0;
	var point=-1;
	while(test!=-1)
	{
		point++;
		test=Mask.indexOf(".",test+1);	
	}
	
	if(point<3)
	{
		alert("Invalid subnet mask!");
		return true;
	}

	if(mask == null)
	{ 
		alert("Invalid subnet mask!");
		return true;
	}

	digits = mask[0].split(".");
	for(i=0; i < 4; i++)
	{
		if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
		{
			alert("Invalid subnet mask!");
			return true;
		}
		bMask = (bMask << 8) | Number(digits[i]);
	}
	if ((Number(digits[0]) == 0) && (Number(digits[1]) == 0)
		&& (Number(digits[2]) == 0) && (Number(digits[3]) == 0))
	{
		alert("Invalid subnet mask!");
		return true;	
	}
	bMask = bMask & 0x0FFFFFFFF;
	
	for(i=0; i<32; i++)
	{
		if((watch==true) && ((bMask & 0x1)==0))
		{
			alert("Invalid subnet mask!");
			return true;
		}
		bMask = bMask >> 1;
		if((bMask & 0x01) == 1)
		{
			watch=true;
		}
	}
	return false;
}

function IPFilterinit()
{
	if(document.IPFILTERform.FULL.value.length != 0){
		alert(document.IPFILTERform.FULL.value);
	}
	if(document.IPFILTERform.Duplicate.value==1){
		alert("the rule has been set !");
		document.IPFILTERform.submit();
	}
	if(document.IPFILTERform.FILTERRuleTypeSEL.selectedIndex == 0)
		document.getElementById("divMac").style.display="none";
	else
		document.getElementById("divIP").style.display="none";		
}

function validateInput()
{
	RuleType = document.IPFILTERform.FILTERRuleTypeSEL.selectedIndex;
	switch(RuleType)
	{
		case 0:
			IP = document.IPFILTERform.SrcIPTXT.value;
			if(unValidIP(IP)){
				return false;
	    }

			if(IP != "0.0.0.0" && unValidMask(document.IPFILTERform.SrcMaskTXT.value))
			{
				return false;
			}

			SrcPort = parseInt(document.IPFILTERform.SrcPortTXT.value);
			if(isNaN(SrcPort) || SrcPort < 0 || SrcPort > 65535){
				alert("Port number's range: 0 ~ 65535");
				return false;
			}

			IP = document.IPFILTERform.DestIPTXT.value;
			if(unValidIP(IP)){
				return false;
			}
			
			if(IP != "0.0.0.0" && unValidMask(document.IPFILTERform.DestMaskTXT.value))
			{
				return false;
			}
			
			DestPort = parseInt(document.IPFILTERform.DestPortTXT.value);
			if(isNaN(DestPort) || DestPort < 0 || DestPort > 65535)
			{	
				alert("Port number's range: 0 ~ 65535");
				return false;
			}
			if (document.IPFILTERform.DSCPFLT.value == "Yes") {
				dscp = parseInt(document.IPFILTERform.DSCPTXT.value);
				if(isNaN(dscp) || dscp < 0 || dscp > 64)
				{	
					alert("DSCP's range: 0 ~ 64");
					return false;
				}
			}
			
			break;
		case 1:
			if(document.IPFILTERform.MacAddrTXT.value.length==0)
			{
	    		return false;
	    }
			break;
	}
	return true;
}

function blockMask(index)
{
	switch(index)
	{
		case 0:
			IP = document.IPFILTERform.SrcIPTXT;
			mask = document.IPFILTERform.SrcMaskTXT;
			break;
		case 1:
			IP = document.IPFILTERform.DestIPTXT;
			mask = document.IPFILTERform.DestMaskTXT;
			break;
	}
	if(IP.value == "0.0.0.0")
	{
		mask.value = "0.0.0.0";
	}else{
		mask.disabled = false;
	}
}

function doHexCheck(c)
{
  if ( (c >= "0") && (c <= "9") )
    return 1;
  else if ( (c >= "A") && (c <= "F") )
    return 1;
  else if ( (c >= "a") && (c <= "f") )
    return 1;

  return -1;
}

function doMACcheck(object)
{
	var szAddr = object.value;
	var len = szAddr.length;

	if(len==0)
	{
		alert("Empty MAC Address!");
		return;
	}

	if(len==12)
	{
		var newAddr = "";
		var i = 0;

		for(i=0; i<len; i++)
		{
			var c = szAddr.charAt(i);

			if(doHexCheck(c) < 0)
			{
				alert("Invalid MAC Address");
				object.focus();
				return;
			}
			if((i == 2)||(i == 4)||(i == 6)||(i == 8)||(i == 10))
			{
				newAddr = newAddr + ":";
			}
			newAddr = newAddr + c;
		}
		object.value = newAddr;
		return;
	}
	else if ( len == 17 )
	{
		var i = 2;
		var c0 = szAddr.charAt(0);
		var c1 = szAddr.charAt(1);

		if ((doHexCheck(c0) < 0)||(doHexCheck(c1) < 0))
		{
			alert("Invalid MAC Address");
			object.focus();
			return;
		}

		i = 2;
		while (i<len)
		{
			var c0 = szAddr.charAt(i);
			var c1 = szAddr.charAt(i+1);
			var c2 = szAddr.charAt(i+2);

			if ((c0 != ":")||(doHexCheck(c1)<0)||(doHexCheck(c2)<0))
			{
				alert("Invalid MAC Address");
				object.focus();
				return;
			}
			i = i + 3;
		}
		return; 
	}
	else
	{
		alert("Invalid MAC Address");
		object.focus();
		return;
	}
}

</SCRIPT>

<META content="MSHTML 6.00.2900.3059" name=GENERATOR>
</HEAD>
<BODY onload=IPFilterinit()>
<FORM name=IPFILTERform action=/cgi-bin/access_ipfilter.asp method=post>
<DIV align=center>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
	<TR>
		<TD class=light-orange colSpan=5 height=5>&nbsp;</TD></TR>
	<TR>
		<TD class=title-main width=150 height=30><FONT color=#ffffff>Filter</FONT></TD>
		<TD class=black width=10>&nbsp;</TD>
		<TD width=150></TD>
		<TD width=10></TD>
		<TD width=440></TD>
	</TR>
	<TR>
		<TD class=title-sub height=25><FONT color=#000000>Filter Type</FONT> </TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Filter Type Selection</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=FILTERTYPE_index onchange=switchFilterType(this) size=1>
				<OPTION SELECTED>IP / MAC Filter
				<OPTION>Application Filter
				<OPTION>URL Filter
			</SELECT> 
		</TD>
	</TR>
	<%if tcwebApi_get("WebCustom_Entry","isC2TrueSupported","h")<>"Yes" then %>
  <TR>
		<TD class=title-sub height=25><FONT color=#000000>Rule Type</FONT></TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Rule Type Selection</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=RuleTypeSEL size=1>
				<OPTION VALUE="Black" <%if TCWebAPI_get("IpMacFilter_Common", "ListType", "h") = "Black" then asp_write("selected") end if %>>Black List
				<OPTION VALUE="White" <%if TCWebAPI_get("IpMacFilter_Common", "ListType", "h") = "White" then asp_write("selected") end if %>>White List
			</SELECT> 
		</TD>
	</TR>
	<% end if%>
  <TR>
		<TD class=title-sub height=25><FONT color=#000000>IP / MAC Filter Rule Editing</FONT></TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
	</TR>
  <TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>IP / MAC Filter Rule Index</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=RuleIndexSEL onchange="doIndexChange();" size=1>
				<OPTION VALUE="0" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "0" then asp_Write("selected") end if %>>1
				<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "1" then asp_Write("selected") end if %>>2
				<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "2" then asp_Write("selected") end if %>>3
				<OPTION VALUE="3" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "3" then asp_Write("selected") end if %>>4
				<OPTION VALUE="4" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "4" then asp_Write("selected") end if %>>5
				<OPTION VALUE="5" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "5" then asp_Write("selected") end if %>>6
				<OPTION VALUE="6" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "6" then asp_Write("selected") end if %>>7
				<OPTION VALUE="7" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "7" then asp_Write("selected") end if %>>8
				<OPTION VALUE="8" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "8" then asp_Write("selected") end if %>>9
				<OPTION VALUE="9" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "9" then asp_Write("selected") end if %>>10
				<OPTION VALUE="10" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "10" then asp_Write("selected") end if %>>11
				<OPTION VALUE="11" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "11" then asp_Write("selected") end if %>>12
				<OPTION VALUE="12" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "12" then asp_Write("selected") end if %>>13
				<OPTION VALUE="13" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "13" then asp_Write("selected") end if %>>14
				<OPTION VALUE="14" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "14" then asp_Write("selected") end if %>>15
				<OPTION VALUE="15" <%if TCWebAPI_get("WebCurSet_Entry", "ipfilter_id", "h") = "15" then asp_Write("selected") end if %>>16
			</SELECT> 
		</TD>
	</TR>
  <TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Active</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<INPUT TYPE="RADIO" name="RuleActiveRDO" VALUE="Yes" 
			<%if TCWebAPI_get("IpMacFilter_Entry", "Active", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000">Yes</font>        
			<INPUT TYPE="RADIO" name="RuleActiveRDO" VALUE="No" 
			<%if TCWebAPI_get("IpMacFilter_Entry", "Active", "h") = "No" then asp_write("checked")%>
			<%elseif TCWebAPI_get("IpMacFilter_Entry", "Active", "h") = "N/A" then asp_write("checked") end if %>><font color="#000000">No</font>
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Interface</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>

	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
			<SELECT name=InterfaceSEL size=1 onchange="doVCChange();">
					<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC0" then asp_write("selected") end if %>>PVC0
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC1" then asp_write("selected") end if %>>PVC1
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC2" then asp_write("selected") end if %>>PVC2
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC3" then asp_write("selected") end if %>>PVC3
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC4" then asp_write("selected") end if %>>PVC4
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC5" then asp_write("selected") end if %>>PVC5
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC6" then asp_write("selected") end if %>>PVC6
				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC7" then asp_write("selected") end if %>>PVC7
				<%end if%>

				<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
				<OPTION VALUE="PVC8" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8" then asp_write("selected") end if %>>PTM0
				<OPTION VALUE="PVC9" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9" then asp_write("selected") end if %>>PTM1
				<%end if%>

				<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
				<OPTION VALUE="PVC10" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10" then asp_write("selected") end if %>>WAN0
				<%end if%>

				<OPTION <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "LAN" then asp_write("selected") end if %>>LAN
			</SELECT> 
		</TD>
	</TR>

		
	<%else%>		
			<SELECT name=InterfaceSEL size=1>
				<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC0" then asp_write("selected") end if %>>PVC0
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC1" then asp_write("selected") end if %>>PVC1
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC2" then asp_write("selected") end if %>>PVC2
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC3" then asp_write("selected") end if %>>PVC3
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC4" then asp_write("selected") end if %>>PVC4
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC5" then asp_write("selected") end if %>>PVC5
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC6" then asp_write("selected") end if %>>PVC6
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC7" then asp_write("selected") end if %>>PVC7
				<%end if%>

				<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
				<OPTION VALUE="PVC8" <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC8" then asp_write("selected") end if %>>PTM0
				<OPTION VALUE="PVC9" <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC9" then asp_write("selected") end if %>>PTM1
				<%end if%>

				<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
				<OPTION VALUE="PVC10" <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "PVC10" then asp_write("selected") end if %>>WAN0
				<%end if%>

				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Interface", "h") = "LAN" then asp_write("selected") end if %>>LAN
			</SELECT> 
		</TD>
	</TR>

	<%end if%>	

	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
	<%if tcWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8" then%>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>MULTI SERVICE</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=ServiceID size=1>
				<OPTION VALUE="PVC8_0" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_0" then asp_write("selected") end if %>>0
				<OPTION VALUE="PVC8_1" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_1" then asp_write("selected") end if %>>1
				<OPTION VALUE="PVC8_2" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_2" then asp_write("selected") end if %>>2
				<OPTION VALUE="PVC8_3" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_3" then asp_write("selected") end if %>>3
				<OPTION VALUE="PVC8_4" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_4" then asp_write("selected") end if %>>4
				<OPTION VALUE="PVC8_5" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_5" then asp_write("selected") end if %>>5
				<OPTION VALUE="PVC8_6" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_6" then asp_write("selected") end if %>>6
				<OPTION VALUE="PVC8_7" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC8_7" then asp_write("selected") end if %>>7
			</SELECT>
		</TD>
	</TR>
	<%end if%>

	<%if tcWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9" then%>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>MULTI SERVICE</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=ServiceID size=1>
				<OPTION VALUE="PVC9_0" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_0" then asp_write("selected") end if %>>0
				<OPTION VALUE="PVC9_1" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_1" then asp_write("selected") end if %>>1
				<OPTION VALUE="PVC9_2" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_2" then asp_write("selected") end if %>>2
				<OPTION VALUE="PVC9_3" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_3" then asp_write("selected") end if %>>3
				<OPTION VALUE="PVC9_4" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_4" then asp_write("selected") end if %>>4
				<OPTION VALUE="PVC9_5" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_5" then asp_write("selected") end if %>>5
				<OPTION VALUE="PVC9_6" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_6" then asp_write("selected") end if %>>6
				<OPTION VALUE="PVC9_7" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC9_7" then asp_write("selected") end if %>>7
			</SELECT>
		</TD>
	</TR>
	<%end if%>

	<%if tcWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10" then%>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>MULTI SERVICE</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=ServiceID size=1>
				<OPTION VALUE="PVC10_0" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_0" then asp_write("selected") end if %>>0
				<OPTION VALUE="PVC10_1" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_1" then asp_write("selected") end if %>>1
				<OPTION VALUE="PVC10_2" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_2" then asp_write("selected") end if %>>2
				<OPTION VALUE="PVC10_3" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_3" then asp_write("selected") end if %>>3
				<OPTION VALUE="PVC10_4" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_4" then asp_write("selected") end if %>>4
				<OPTION VALUE="PVC10_5" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_5" then asp_write("selected") end if %>>5
				<OPTION VALUE="PVC10_6" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_6" then asp_write("selected") end if %>>6
				<OPTION VALUE="PVC10_7" <%if TCWebAPI_get("WebCustom_Entry", "filter_interface", "h") = "PVC10_7" then asp_write("selected") end if %>>7
			</SELECT>
		</TD>
	</TR>
	<%end if%>
	<%end if%>
	<%end if%>

	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Direction</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=DirectionSEL size=1>
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Direction", "h") = "Both" then asp_write("selected") end if %>>Both
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Direction", "h") = "Incoming" then asp_write("selected") end if %>>Incoming
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Direction", "h") = "Outgoing" then asp_write("selected") end if %>>Outgoing
			</SELECT>
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Rule Type</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT name=FILTERRuleTypeSEL onchange="doChangeRuleType()" size=1>
				<OPTION VALUE="IP" <%if TCWebAPI_get("IpMacFilter_Entry", "RuleType", "h") = "IP" then asp_write("selected") end if %>>IP
				<OPTION VALUE="MAC" <%if TCWebAPI_get("IpMacFilter_Entry", "RuleType", "h") = "MAC" then asp_write("selected") end if %>>MAC
			</SELECT>
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
	</TR>
	</TBODY>
	</TABLE>
<DIV id="divIP">
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TR>
		<TD class=light-orange width=150 height=30>&nbsp;</TD>
		<TD class=light-orange width=10></TD>
		<TD class=tabdata width=150><DIV align=right><FONT color=#000000>Source IP Address</FONT></DIV></TD>
		<TD class=tabdata width=10><DIV align=center>:</DIV></TD>
		<TD class=tabdata width=440>
			<INPUT name=SrcIPTXT onblur=blockMask(0); maxLength=15 size=15 
				VALUE="<%if TCWebAPI_get("IpMacFilter_Entry", "SrcIPAddr", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "SrcIPAddr", "s") else asp_Write("0.0.0.0") end if %>">
			<FONT color=#000000>(0.0.0.0 means Don't care)</FONT>
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Subnet Mask</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<INPUT maxLength=15 size=15 name=SrcMaskTXT 
				VALUE="<%if TCWebAPI_get("IpMacFilter_Entry", "SrcIPMask", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "SrcIPMask", "s") else asp_Write("0.0.0.0") end if %>" > 
		</TD>
	</TR>
  <TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Port Number</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<INPUT maxLength=7 size=8 name=SrcPortTXT 
				VALUE="<%if TCWebAPI_get("IpMacFilter_Entry", "SrcPort", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "SrcPort", "s") else asp_Write("0") end if %>" >
			<FONT color=#000000>(0 means Don't care)</FONT>
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
	</TR>
	<TR>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata><DIV align=right><FONT color=#000000>Destination IP Address</FONT></DIV></TD>
    <TD class=tabdata><DIV align=center>:</DIV></TD>
    <TD class=tabdata>
			<INPUT onblur=blockMask(1); maxLength=15 size=15 name=DestIPTXT 
				VALUE="<%if TCWebAPI_get("IpMacFilter_Entry", "DesIPAddr", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "DesIPAddr", "s") else asp_Write("0.0.0.0") end if %>" >
			<FONT color=#000000>(0.0.0.0 means Don't care)</FONT>
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Subnet Mask</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<INPUT maxLength=15 size=15 name=DestMaskTXT 
				VALUE="<%if TCWebAPI_get("IpMacFilter_Entry", "DesIPMask", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "DesIPMask", "s") else asp_Write("0.0.0.0") end if %>" > 
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Port Number</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<INPUT maxLength=7 size=8 name=DestPortTXT 
				VALUE="<%if TCWebAPI_get("IpMacFilter_Entry", "DesPort", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "DesPort", "s") else asp_Write("0") end if %>" >
			<FONT color=#000000>(0 means Don't care)</FONT> 
		</TD>
	</TR>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
		<TD class=tabdata><HR class=light-orange-line noShade></TD>
	</TR>
	<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then%>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>DSCP</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<INPUT maxLength=7 size=8 name=DSCPTXT 
				VALUE="<%if TCWebAPI_get("IpMacFilter_Entry", "DSCP", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "DSCP", "s") else asp_Write("0") end if %>" >
			<FONT color=#000000>(Value Range:0~64, 64 means Don't care)</FONT> 
		</TD>
	</TR>
	<%end if%>
	<TR>
		<TD class=light-orange>&nbsp;</TD>
		<TD class=light-orange></TD>
		<TD class=tabdata><DIV align=right><FONT color=#000000>Protocol</FONT></DIV></TD>
		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
			<SELECT size=1 name=ProtocolSEL>
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Protocol", "h") = "TCP" then asp_write("selected") end if %>>TCP
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Protocol", "h") = "UDP" then asp_write("selected") end if %>>UDP
				<OPTION <%if TCWebAPI_get("IpMacFilter_Entry", "Protocol", "h") = "ICMP" then asp_write("selected") end if %>>ICMP
			</SELECT>
		</TD>
	</TR>
</TABLE>
</DIV>
<DIV id="divMac">
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>	
	<TR>
		<TD class=light-orange width=150 height=30>&nbsp;</TD>
		<TD class=light-orange width=10></TD>
		<TD class=tabdata width=150><DIV align=right><FONT color=#000000>MAC Address</FONT></DIV></TD>
		<TD class=tabdata width=10><DIV align=center>:</DIV></TD>
		<TD class=tabdata width=440>
			<INPUT name=MacAddrTXT onblur=doMACcheck(this) maxLength=17 size=17 
				value="<%if TCWebAPI_get("IpMacFilter_Entry", "MacAddr", "h") <> "N/A" then TCWebAPI_get("IpMacFilter_Entry", "MacAddr", "s") else asp_Write("") end if %>">
			<FONT color=#000000></FONT>
		</TD>
	</TR>
</TABLE>
</DIV>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
  <TBODY>
  <TR>
		<TD class=title-sub width=150><FONT color=#000000>IP / MAC Filter Listing</FONT></TD>
		<TD class=light-orange width=10></TD>
		<TD width=10><HR class=light-orange-line noShade></TD>
		<TD width=580><HR class=light-orange-line noShade></TD>
		<TD width=10><HR class=light-orange-line noShade></TD>
	</TR>
	<TR>
		<TD class=light-orange width=150></TD>
		<TD class=light-orange width=10></TD>
		<TD width=10></TD>
		<TD width=580></TD>
		<TD width=10></TD>
	</TR>
	<TR>
		<TD class="light-orange"></TD>
		<TD class="light-orange"></TD>
		<TD>&nbsp;</TD>
		<TD>
		<!--
			<iframe src="/cgi-bin/access_ipfilterlist.cgi" frameborder="0" width="560" height="200"></iframe>
			-->
<%if tcwebApi_get("IpMacFilter_Common","Duplicate","h")<>"Yes" then %>
			<div id=ipmacList></div>
			<script language=JavaScript>
			var tableHeader = [
				["10","#"],
				["30","Active"],
				["40","Interface"],
				["40","Direction"],
				["120","Src IP Address" + "/" + "Mask"],
				["120","Dest IP Address" + "/"+ "Mask"],
				["60","Mac Address"],
				["40","Src Port"],
				["40","Dest Port"],
			<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>
				["40", "DSCP"],
			<%end if%>
				["40","Protocol"]
			];
			var tableData = [
<%if TCWebAPI_get("IpMacFilter_Entry0", "RuleType", "h") = "MAC" then %>
				["1", "<%tcWebApi_get("IpMacFilter_Entry0", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry0", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry0", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry0", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>
"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry0", "RuleType", "h") = "IP" then %>
				["1", "<%tcWebApi_get("IpMacFilter_Entry0", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry0", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry0", "Interface","h")="PVC9" then %>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry0", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry0","Interface","s")%>",<%end if%> "<%tcWebApi_staticGet("IpMacFilter_Entry0", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry0", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry0", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry0", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry0", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry0", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry0", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry0", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry0", "Protocol","s")%>"],
<%else%>
				["1", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry1", "RuleType", "h") = "MAC" then %>
				["2","<%tcWebApi_get("IpMacFilter_Entry1", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry1", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry1", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry1", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry1", "RuleType", "h") = "IP" then %>
				["2","<%tcWebApi_get("IpMacFilter_Entry1", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry1", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry1", "Interface","h")="PVC9" then %>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry1", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry1","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry1", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry1", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry1", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry1", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry1", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry1", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry1", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry1", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry1", "Protocol","s")%>"],
<%else%>
				["2", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry2", "RuleType", "h") = "MAC" then %>
				["3", "<%tcWebApi_get("IpMacFilter_Entry2", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry2", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry2", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry2", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry2", "RuleType", "h") = "IP" then %>
				["3","<%tcWebApi_get("IpMacFilter_Entry2", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry2", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry2", "Interface","h")="PVC9" then %>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry2", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry2","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry2", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry2", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry2", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry2", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry2", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry2", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry2", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry2", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry2", "Protocol","s")%>"],
<%else%>
				["3", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry3", "RuleType", "h") = "MAC" then %>
				["4", "<%tcWebApi_get("IpMacFilter_Entry3", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry3", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry3", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry3", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry3", "RuleType", "h") = "IP" then %>
				["4", "<%tcWebApi_get("IpMacFilter_Entry3", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry3", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry3", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry3", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry3","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry3", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry3", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry3", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry3", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry3", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry3", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry3", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry3", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry3", "Protocol","s")%>"],
<%else%>
				["4", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry4", "RuleType", "h") = "MAC" then %>
				["5", "<%tcWebApi_get("IpMacFilter_Entry4", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry4", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry4", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry4", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry4", "RuleType", "h") = "IP" then %>
				["5", "<%tcWebApi_get("IpMacFilter_Entry4", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry4", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry4", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry4", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry4","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry4", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry4", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry4", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry4", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry4", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry4", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry4", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry4", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry4", "Protocol","s")%>"],
<%else%>
				["5", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry5", "RuleType", "h") = "MAC" then %>
				["6", "<%tcWebApi_get("IpMacFilter_Entry5", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry5", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry5", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry5", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry5", "RuleType", "h") = "IP" then %>
				["6", "<%tcWebApi_get("IpMacFilter_Entry5", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry5", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry5", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry5", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry5","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry5", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry5", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry5", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry5", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry5", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry5", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry5", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry5", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry5", "Protocol","s")%>"],
<%else%>
				["6", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry6", "RuleType", "h") = "MAC" then %>
				["7", "<%tcWebApi_get("IpMacFilter_Entry6", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry6", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry6", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry6", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry6", "RuleType", "h") = "IP" then %>
				["7", "<%tcWebApi_get("IpMacFilter_Entry6", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry6", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry6", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry6", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry6","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry6", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry6", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry6", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry6", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry6", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry6", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry6", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry6", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry6", "Protocol","s")%>"],
<%else%>
				["7", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry7", "RuleType", "h") = "MAC" then %>
				["8", "<%tcWebApi_get("IpMacFilter_Entry7", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry7", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry7", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry7", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry7", "RuleType", "h") = "IP" then %>
				["8", "<%tcWebApi_get("IpMacFilter_Entry7", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry7", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry7", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry7", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry7","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry7", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry7", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry7", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry7", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry7", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry7", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry7", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry7", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry7", "Protocol","s")%>"],
<%else%>
				["8", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry8", "RuleType", "h") = "MAC" then %>
				["9", "<%tcWebApi_get("IpMacFilter_Entry8", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry8", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry8", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry8", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry8", "RuleType", "h") = "IP" then %>
				["9", "<%tcWebApi_get("IpMacFilter_Entry8", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry8", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry8", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry8", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry8","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry8", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry8", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry8", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry8", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry8", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry8", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry8", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry8", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry8", "Protocol","s")%>"],
<%else%>
				["9", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry9", "RuleType", "h") = "MAC" then %>
				["10", "<%tcWebApi_get("IpMacFilter_Entry9", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry9", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry9", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry9", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry9", "RuleType", "h") = "IP" then %>
				["10", "<%tcWebApi_get("IpMacFilter_Entry9", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry9", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry9", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry9", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry9","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry9", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry9", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry9", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry9", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry9", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry9", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry9", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry9", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry9", "Protocol","s")%>"],
<%else%>
				["10", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry10", "RuleType", "h") = "MAC" then %>
				["11", "<%tcWebApi_get("IpMacFilter_Entry10", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry10", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry10", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry10", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry10", "RuleType", "h") = "IP" then %>
				["11", "<%tcWebApi_get("IpMacFilter_Entry10", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry10", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry10", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry10", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry10","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry10", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry10", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry10", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry10", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry10", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry10", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry10", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry10", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry10", "Protocol","s")%>"],
<%else%>
				["11", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry11", "RuleType", "h") = "MAC" then %>
				["12", "<%tcWebApi_get("IpMacFilter_Entry11", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry11", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry11", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry11", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry11", "RuleType", "h") = "IP" then %>
				["12", "<%tcWebApi_get("IpMacFilter_Entry11", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry11", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry11", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry11", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry11","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry11", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry11", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry11", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry11", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry11", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry11", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry11", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry11", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry11", "Protocol","s")%>"],
<%else%>
				["12", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry12", "RuleType", "h") = "MAC" then %>
				["13", "<%tcWebApi_get("IpMacFilter_Entry12", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry12", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry12", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry12", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry12", "RuleType", "h") = "IP" then %>
				["13", "<%tcWebApi_get("IpMacFilter_Entry12", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry12", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry12", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry12", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry12","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry12", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry12", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry12", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry12", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry12", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry12", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry12", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry12", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry12", "Protocol","s")%>"],
<%else%>
				["13", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry13", "RuleType", "h") = "MAC" then %>
				["14", "<%tcWebApi_get("IpMacFilter_Entry13", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry13", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry13", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry13", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry13", "RuleType", "h") = "IP" then %>
				["14", "<%tcWebApi_get("IpMacFilter_Entry13", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry13", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry13", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry13", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry13","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry13", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry13", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry13", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry13", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry13", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry13", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry13", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry13", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry13", "Protocol","s")%>"],
<%else%>
				["14", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry14", "RuleType", "h") = "MAC" then %>
				["15", "<%tcWebApi_get("IpMacFilter_Entry14", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry14", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry14", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry14", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%elseif TCWebAPI_get("IpMacFilter_Entry14", "RuleType", "h") = "IP" then %>
				["15", "<%tcWebApi_get("IpMacFilter_Entry14", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry14", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry14", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry14", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry14","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry14", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry14", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry14", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry14", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry14", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry14", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry14", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry14", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry14", "Protocol","s")%>"],
<%else%>
				["15", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"],
<%end if%>
<%if TCWebAPI_get("IpMacFilter_Entry15", "RuleType", "h") = "MAC" then %>
				["16", "<%tcWebApi_get("IpMacFilter_Entry15", "Active","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry15", "Interface","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry15", "Direction","s")%>","N/A","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry15", "MacAddr","s")%>","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"]
<%elseif TCWebAPI_get("IpMacFilter_Entry15", "RuleType", "h") = "IP" then %>
				["16", "<%tcWebApi_get("IpMacFilter_Entry15", "Active","s")%>",<%if tcWebApi_staticGet("IpMacFilter_Entry15", "Interface","h")="PVC8" then %>"PTM0" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry15", "Interface","h")="PVC9" then%>"PTM1" ,<%elseif tcWebApi_staticGet("IpMacFilter_Entry15", "Interface","h")="PVC10" then %>"WAN0" ,<%else%> "<%tcWebApi_staticGet("IpMacFilter_Entry15","Interface","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry15", "Direction","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry15", "SrcIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry15", "SrcIPMask","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry15", "DesIPAddr","s")%>" + "/<br>" + "<%tcWebApi_staticGet("IpMacFilter_Entry15", "DesIPMask","s")%>","N/A","<%tcWebApi_staticGet("IpMacFilter_Entry15", "SrcPort","s")%>","<%tcWebApi_staticGet("IpMacFilter_Entry15", "DesPort","s")%>",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"<%tcWebApi_staticGet("IpMacFilter_Entry15", "DSCP","s")%>",<%end if%>"<%tcWebApi_staticGet("IpMacFilter_Entry15", "Protocol","s")%>"]
<%else%>
				["16", "N/A","N/A","N/A","N/A","N/A","N/A","N/A","N/A",<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") = "Yes" then %>"N/A",<%end if%>"N/A"]
<%end if%>	
			];
			tableShow('ipmacList',tableHeader,tableData,1);
			</script>
<%end if%>
 		</TD>
		<TD>&nbsp;</TD>
	</TR>
	<TR>
		<TD class=light-orange></TD>
		<TD class=light-orange></TD>
		<TD>&nbsp;</TD>
		<TD>&nbsp;</TD>
		<TD>&nbsp;</TD>
	</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
	<TR>
		<TD class=black width=160 height=42>&nbsp;</TD>
		<TD class=orange width=160>&nbsp;</TD>
		<TD class=orange width=440>
			
			<INPUT type=hidden name=c2Support VALUE="0">
			<INPUT type="submit" value=SET name=IpFilterApply onclick="doAdd();return validateInput();">
			<INPUT type="submit" value=DELETE name=IpFilterDelete onclick="doDel();"> 
			<INPUT type="reset" value=CANCEL name=IpFilterCancel onclick="doCancel();"> 
			<INPUT type=hidden name=FULL>
			<INPUT type=hidden name=RuleTypeChange VALUE="0">
			<INPUT type="hidden" name="Duplicate" VALUE="<%if tcwebApi_get("IpMacFilter_Common","Duplicate","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
			<Input type="hidden" name="NoDup" value="No">
			<INPUT type=hidden name=DSCPFLT VALUE="<%if TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "h") <> "N/A" then TCWebAPI_get("WebCustom_Entry", "isDSCPSupported", "s") else asp_Write("No") end if %>">
		</TD>
	</TR>
	</TBODY>
</TABLE>
</DIV>
</FORM>
</BODY>
</HTML>
