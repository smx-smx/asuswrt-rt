<%
request_Form("QosRuleIndex")

/*add a rule*/
if request_Form("QOS_Flag") = "0" then
	TCWebAPI_set("WebCurSet_Entry","qos_id","QosRuleIndex")
if request_Form("QosRuleActive") = "Yes" then	
	/*remove original data*/
	TCWebAPI_unset("QoS_Entry")
end if
	
	/*add new data*/
	TCWebAPI_set("QoS_Common","Active","Qos_active")
	
	TCWebAPI_set("QoS_Entry","Active","QosRuleActive")
	TCWebAPI_set("QoS_Entry","Application","QosApp")
	TCWebAPI_set("QoS_Entry","eth0.1","QosPhyPortEth0")
	TCWebAPI_set("QoS_Entry","eth0.2","QosPhyPortEth1")
	if request_Form("QoS2PortsFlag") <> "Yes" then
		TCWebAPI_set("QoS_Entry","eth0.3","QosPhyPortEth2")	
		TCWebAPI_set("QoS_Entry","eth0.4","QosPhyPortEth3")
	end if
/*
	TCWebAPI_set("QoS_Entry","usb0","QosPhyPortUsb0")
*/
	if request_Form("wlanISExist") = "On" then
		if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then
			TCWebAPI_set("QoS_Entry","ra0","QosPhyPortRa0")
		elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "2" then
			TCWebAPI_set("QoS_Entry","ra0","QosPhyPortWLANMssid0")
			TCWebAPI_set("QoS_Entry","ra1","QosPhyPortWLANMssid1")
		elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "3" then
			TCWebAPI_set("QoS_Entry","ra0","QosPhyPortWLANMssid0")
			TCWebAPI_set("QoS_Entry","ra1","QosPhyPortWLANMssid1")
			TCWebAPI_set("QoS_Entry","ra2","QosPhyPortWLANMssid2")
		elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "4" then
			TCWebAPI_set("QoS_Entry","ra0","QosPhyPortWLANMssid0")
			TCWebAPI_set("QoS_Entry","ra1","QosPhyPortWLANMssid1")
			TCWebAPI_set("QoS_Entry","ra2","QosPhyPortWLANMssid2")
			TCWebAPI_set("QoS_Entry","ra3","QosPhyPortWLANMssid3")	
		end if
	end if	
	TCWebAPI_set("QoS_Entry","DesMac","QosDestMacValue")
	TCWebAPI_set("QoS_Entry","DesIP","QosDestIpValue")
	TCWebAPI_set("QoS_Entry","DesMask","QosDestMaskValue")
	TCWebAPI_set("QoS_Entry","DesPortRangeBegin","QosDestPortValue1")
	TCWebAPI_set("QoS_Entry","DesPortRangeEnd","QosDestPortValue2")
	TCWebAPI_set("QoS_Entry","SrcMac","QosSrcMacValue")
	TCWebAPI_set("QoS_Entry","SrcIP","QosSrcIpValue")
	TCWebAPI_set("QoS_Entry","SrcMask","QosSrcMaskValue")
	TCWebAPI_set("QoS_Entry","SrcPortRangeBegin","QosSrcPortValue1")
	TCWebAPI_set("QoS_Entry","SrcPortRangeEnd","QosSrcPortValue2")
	TCWebAPI_set("QoS_Entry","ProtocolID","QosProtocol")
	TCWebAPI_set("QoS_Entry","VlanIDRangeBegin","QosVIDValue1")
	TCWebAPI_set("QoS_Entry","VlanIDRangeEnd","QosVIDValue2")
	TCWebAPI_set("QoS_Entry","IPPDSField","Qos_IPP_DSCP1")
	TCWebAPI_set("QoS_Entry","IPPRangeBegin","QosIPPValue1")
	TCWebAPI_set("QoS_Entry","IPPRangeEnd","QosIPPValue2")
	TCWebAPI_set("QoS_Entry","TypeOfService","QosTOS")
	TCWebAPI_set("QoS_Entry","DSCPRangeBegin","QosDSCPValue1")
	TCWebAPI_set("QoS_Entry","DSCPRangeEnd","QosDSCPValue2")
	TCWebAPI_set("QoS_Entry","dotpBegin","Qos8021pValue1")
	TCWebAPI_set("QoS_Entry","dotpEnd","Qos8021pValue2")
	TCWebAPI_set("QoS_Entry","ActIPPDSField","Qos_IPP_DSCP2")
	TCWebAPI_set("QoS_Entry","ActIPPRemarking","QosReIPPValue")
	TCWebAPI_set("QoS_Entry","ActToSRemarking","QosReTOS")
	TCWebAPI_set("QoS_Entry","ActDSCPRemarking","QosReDSCPValue")
	'TCWebAPI_set("QoS_Entry","Act8021pRemarkingNum","Qos8021pReValue")
	'TCWebAPI_set("QoS_Entry","Act8021pRemarking","Qos8021pReApp")
	TCWebAPI_set("QoS_Entry","ActQueue","QosConfigPriority")
	
	/*commit changes*/
	TCWebAPI_commit("QoS_Entry")

/*select a rule*/	
elseif Request_Form("QOS_Flag")="1" then
	TCWebAPI_set("WebCurSet_Entry","qos_id","QosRuleIndex")

/*remove a rule*/
elseif Request_Form("QOS_Flag")="2" then
	TCWebAPI_unset("QoS_Entry")
	TCWebAPI_commit("QoS_Entry")
	
elseif Request_Form("QOS_Flag")="4" then
	TCWebAPI_set("QoS_Common","Active","Qos_active")
	TCWebAPI_set("QoS_Common","Drop","Qosdrop")
	TCWebAPI_set("QoS_Common","Discipline","Qosdiscipline")
	TCWebAPI_set("QoS_Common","DisciplineFlag","Qosdisciplineflag")
	TCWebAPI_set("QoS_Common","WeightHighest","QosWRRweight1")
	TCWebAPI_set("QoS_Common","WeightHigh","QosWRRweight2")
	TCWebAPI_set("QoS_Common","WeightMedium","QosWRRweight3")
	TCWebAPI_set("QoS_Common","WeightLow","QosWRRweight4")			

	TCWebAPI_commit("QoS_Entry")
end if
%>

<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" type="text/javascript" src="/jsl.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript">
	
function ornIppTosDscp(type){
var form = document.QoS_Form;

switch(type){
	case 0://disable all
		form.Qos_IPP_DSCP1[0].disabled = true;
		form.Qos_IPP_DSCP1[1].disabled = true;
		form.QosIPPValue1.disabled = true;
		form.QosIPPValue2.disabled = true;
		form.QosTOS.disabled = true;
		form.QosDSCPValue1.disabled = true;
		form.QosDSCPValue2.disabled = true;
		break;
	case 1://enable all
		form.Qos_IPP_DSCP1[0].disabled = false;
		form.Qos_IPP_DSCP1[1].disabled = false;
		form.QosIPPValue1.disabled = false;
		form.QosIPPValue2.disabled = false;
		form.QosTOS.disabled = false;
		form.QosDSCPValue1.disabled = false;
		form.QosDSCPValue2.disabled = false;
		break;
	case 2://IPP
		form.Qos_IPP_DSCP1[0].disabled = false;
		form.Qos_IPP_DSCP1[1].disabled = false;
		form.QosIPPValue1.disabled = false;
		form.QosIPPValue2.disabled = false;
		form.QosTOS.disabled = false;
		form.QosDSCPValue1.disabled = true;
		form.QosDSCPValue2.disabled = true;
		break;
	case 3://DSCP
		form.Qos_IPP_DSCP1[0].disabled = false;
		form.Qos_IPP_DSCP1[1].disabled = false;
		form.QosIPPValue1.disabled = true;
		form.QosIPPValue2.disabled = true;
		form.QosTOS.disabled = true;
		form.QosDSCPValue1.disabled = false;
		form.QosDSCPValue2.disabled = false;
		break;
	default:
		break;
	}
}

function reIppTosDscp(type){
	var form = document.QoS_Form;
	
	switch(type){
		case 0://disable all
		form.Qos_IPP_DSCP2[0].disabled = true;
		form.Qos_IPP_DSCP2[1].disabled = true;
		form.QosReIPPValue.disabled = true;
		form.QosReTOS.disabled = true;
		form.QosReDSCPValue.disabled = true;
		break;
	case 1://enable all
		form.Qos_IPP_DSCP2[0].disabled = false;
		form.Qos_IPP_DSCP2[1].disabled = false;
		form.QosReIPPValue.disabled = false;
		form.QosReTOS.disabled = false;
		form.QosReDSCPValue.disabled = false;
		break;
	case 2://IPP
		form.Qos_IPP_DSCP2[0].disabled = false;
		form.Qos_IPP_DSCP2[1].disabled = false;
		form.QosReIPPValue.disabled = false;
		form.QosReTOS.disabled = false;
		form.QosReDSCPValue.disabled = true;
		break;
	case 3://DSCP
		form.Qos_IPP_DSCP2[0].disabled = false;
		form.Qos_IPP_DSCP2[1].disabled = false;
		form.QosReIPPValue.disabled = true;
		form.QosReTOS.disabled = true;
		form.QosReDSCPValue.disabled = false;
		break;
	default:
		break;
	}
}

function fromDMacToPriority(TorF){
	var value;
	var form = document.QoS_Form;
	
	if(TorF == 0){
		value = true;
		ornIppTosDscp(0);
		reIppTosDscp(0);
	}else{
		value = false;
		ornIppTosDscp(1);
		reIppTosDscp(1);
	}
	
	form.QosDestMacValue.disabled = value;
	form.QosDestIpValue.disabled = value;
	form.QosDestMaskValue.disabled = value;
	form.QosDestPortValue1.disabled = value;
	form.QosDestPortValue2.disabled = value;
	form.QosSrcMacValue.disabled = value;
	form.QosSrcIpValue.disabled = value;
	form.QosSrcMaskValue.disabled = value;
	form.QosSrcPortValue1.disabled = value;
	form.QosSrcPortValue2.disabled = value;
	form.QosProtocol.disabled = value;
	form.QosVIDValue1.disabled = value;
	form.QosVIDValue2.disabled = value;
	form.Qos8021pValue1.disabled = value;
	form.Qos8021pValue2.disabled = value;
	//form.Qos8021pReValue.disabled = value;
	//form.Qos8021pReApp.disabled = value;
	form.QosConfigPriority.disabled = value;
}

function fromSumToDeact(TorF){
	var form = document.QoS_Form;
	var value;
	
	if(TorF == 0)
		value = true;
	else
		value = false;

	/*
	form.Qosdrop[0].disabled = value;
	form.Qosdrop[1].disabled = value;
	form.Qosdrop[2].disabled = value;
	*/
	form.Qosdiscipline[0].disabled = value;
	form.Qosdiscipline[1].disabled = value;
	form.Qosdisciplinesave.disabled = value;
	form.QosWRRweight1.disabled = value;
	form.QosWRRweight2.disabled = value;
	form.QosWRRweight3.disabled = value;
	form.QosWRRweight4.disabled = value;
	form.Qos_Summary.disabled = value;
	form.QosRuleIndex.disabled = value;
	form.QosRuleActive[0].disabled = value;
	form.QosRuleActive[1].disabled = value;
}

function formAppToPhyport(TorF){
	var form = document.QoS_Form;
	var value;
	
	if(TorF == 0)
		value = true;
	else
		value = false;
		
	form.QosApp.disabled = value;
	
	if (form.wlanISExist.value == "On") {
		if(form.QosMBSSIDNumberFlag.value == 1){
			//form.QosPhyPortWLANMssid0.disabled=value;
			form.QosPhyPortRa0.disabled = value;
		}
		if(form.QosMBSSIDNumberFlag.value == 2){
			form.QosPhyPortWLANMssid0.disabled = value;
			form.QosPhyPortWLANMssid1.disabled = value;
		}
		if(form.QosMBSSIDNumberFlag.value == 3){
			form.QosPhyPortWLANMssid0.disabled = value;
			form.QosPhyPortWLANMssid1.disabled = value;
			form.QosPhyPortWLANMssid2.disabled = value;
		}
		if(form.QosMBSSIDNumberFlag.value == 4){
			form.QosPhyPortWLANMssid0.disabled = value;
			form.QosPhyPortWLANMssid1.disabled = value;
			form.QosPhyPortWLANMssid2.disabled = value;
			form.QosPhyPortWLANMssid3.disabled = value;
		}
	}
	
	form.QosPhyPortEth0.disabled = value;


	form.QosPhyPortEth1.disabled = value;
	
	if (form.QoS2PortsFlag.value != "Yes") {
		form.QosPhyPortEth2.disabled = value;
		form.QosPhyPortEth3.disabled = value;
	}

/*
	form.QosPhyPortUsb0.disabled = value;
*/
	//form.QosPhyPortRa0.disabled = value;

}

function disableAll()
{
	fromSumToDeact(0);
	formAppToPhyport(0);
	fromDMacToPriority(0);
}

function enableAll()
{
	fromSumToDeact(1);
	formAppToPhyport(1);
	fromDMacToPriority(1);
}

function init()
{
	if(document.QoS_Form.Qos_active[1].checked){
		disableAll();
		return;
	}
	if(document.QoS_Form.QosRuleActive[1].checked){
		formAppToPhyport(0);
		fromDMacToPriority(0);
		return;
	}
	if(document.QoS_Form.Qos_IPP_DSCP1[0].checked){
		ornIppTosDscp(2);
	}else{
		ornIppTosDscp(3);
	}
	if(document.QoS_Form.Qos_IPP_DSCP2[0].checked){
		reIppTosDscp(2);
	}else{
		reIppTosDscp(3);
	}
}           

function isValidHex(ucHex)
{
	return (ucHex>='0' && ucHex<='9') || (ucHex>='a' && ucHex<='f') || (ucHex>='A' && ucHex<='F') ? true : false;
}

function doMACcheck(object)
{
	var szAddr = object.value;
	var len = szAddr.length;
	
	if ( len == 0 )
	{
		return;
	}
	
	if ( len == 12 )
	{
		var newAddr = "";
		var i = 0;
		for ( i = 0; i < len; i++ )
		{
			var c = szAddr.charAt(i);
			if ( !isValidHex(c) )
			{
				if( object.name == "QosDestMacValue"){
					alert("Invalid Destination MAC address!");}else{
					alert("Invalid Source MAC address!");}
					object.focus();
					return;
			}
			
			if ( (i == 2) || (i == 4) || (i == 6) || (i == 8) || (i == 10) ){
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
		
		if ( (!isValidHex(c0)) || (!isValidHex(c1)) )
		{
			if( object.name == "QosDestMacValue")
			{
				alert("Invalid Destination MAC address!");
			}else{
				alert("Invalid Source MAC address!");
			}
			
			object.focus();
			return;
		}         
		
		i = 2;
		while ( i < len )
		{
			var c0 = szAddr.charAt(i);
			var c1 = szAddr.charAt(i+1);
			var c2 = szAddr.charAt(i+2);
			
			if ( (c0 != ":") || (!isValidHex(c1)) || (!isValidHex(c2)) )
			{
				if( object.name == "QosDestMacValue")
				{
					alert("Invalid Destination MAC address!");
				}else{
					alert("Invalid Source MAC address!");
				}
				
				object.focus();
				return;
			}
		i = i + 3;
		}
		return;
	}
	else
	{
		if( object.name == "QosDestMacValue")
		{
			alert("Invalid Destination MAC address!");
			}else{
			alert("Invalid Source MAC address!");
		}
		
	object.focus();
	return;
	}
}                                   

function errorValueAlert(option){
	switch(option){
		case 0:
			alert("Destination Port Error!");break;
		case 1:
			alert("Source Port Error!");break;
		case 2:
			alert("Vlan ID Error!");break;
		case 3:
			alert("Wrong DSCP Priority!");break;
		case 4:
			alert("Wrong Remarked DSCP value!");break;
		case 6:
			alert("Wrong New 802.1p Priority!");break;
		case 7:
			alert("WRR weight is out of range!");break;
		default:
			break;
	}
}

function checkRangeValue(sElem, eElem, min, max, option)
{
	start = parseInt(sElem.value);
	end = parseInt(eElem.value);
	
	if((sElem.value == "")&&(eElem.value == ""))
		return true;
		
	if(isNaN(start) || start > max || start < min)
	{
		errorValueAlert(option);
		return false;
	}
	
	if(isNaN(end) || end > max || end < min)
	{
		errorValueAlert(option);
		return false;
	}
	
	if(start > end)
	{
		errorValueAlert(option);
		return false;
	}
	
	return true;
}

function inValidQOSIP(Address){
	var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var i;
  
	digits = address[0].split(".");
	for(i=0; i < 4; i++){
 		if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (digits[i] == null)){ 
			alert('Invalid IP address ' + Address);
			return true;
		}
	}
  return false;
}

function verifyForm(){
	var form = document.QoS_Form;
	if(form.QOS_Flag.value != 0)
		return;
		
	if(document.QoS_Form.Qos_active[1].checked)
		return;
		
	if(document.QoS_Form.QosRuleActive[1].checked)
		return;	
		
	if(form.QosDestIpValue.value != "")
	{
		if(inValidQOSIP(form.QosDestIpValue.value))
			return false;
	}
	
	if(form.QosDestMaskValue.value != "")
	{
		if(inValidSubnetMask(form.QosDestMaskValue.value))
			return false;	
	}
	
	if(!checkRangeValue(form.QosDestPortValue1, form.QosDestPortValue2, 0, 65535, 0))
		return false;
		
	if(form.QosSrcIpValue.value != "")
	{
		if(inValidQOSIP(form.QosSrcIpValue.value))
			return false;
	}   
	
	if(form.QosSrcMaskValue.value != "")
	{
		if(inValidSubnetMask(form.QosSrcMaskValue.value))
			return false;
	}
	
	if(!checkRangeValue(form.QosSrcPortValue1, form.QosSrcPortValue2, 0, 65535, 1))
		return false;
		
	if((form.QosVIDValue1.value !=0 ) || (form.QosVIDValue2.value != 0))
	{
		if(!checkRangeValue(form.QosVIDValue1, form.QosVIDValue2, 1, 4094, 2))
			return false;
	}
	
	if(document.QoS_Form.Qos_IPP_DSCP1[0].checked)
	{
		if(form.QosIPPValue1.selectedIndex > form.QosIPPValue2.selectedIndex)
		{
			alert("Wrong IP Precedence value!");
			return false;
		}
	}else{
		if(!checkRangeValue(form.QosDSCPValue1, form.QosDSCPValue2, 0, 63, 3))
			return false;
	}
	
	if(form.Qos8021pValue1.selectedIndex > form.Qos8021pValue2.selectedIndex)
	{
		alert("Wrong Original 802.1p Priority!");return false;
	}
	
	if(form.Qos_IPP_DSCP2[1].checked)
	{
		if(!checkRangeValue(form.QosReDSCPValue, form.QosReDSCPValue, 0, 63, 4))
		return false;
	}
	
	/* check protocol is set for port setting */
	if (form.QosDestPortValue1.value != "" || form.QosSrcPortValue1.value != "") {
		if (form.QosProtocol.options[0].selected == true) {
			//alert("We should set protocol if src or dst port is setted!");
			//return false;
			form.QosProtocol.options[1].selected = true;
		}
	}
	
	if(form.QosConfigPriority.selectedIndex == 0)
	{
		alert("We should assign priority queue for this rule!");return false;
	}
	return;
}

function checkPhysicalPort(){

	if(document.forms[0].QosPhyPortEth0.checked)
		document.forms[0].QosPhyPortEth0.value = "Yes";
	else
		document.forms[0].QosPhyPortEth0.value = "No";
	
	if(document.forms[0].QosPhyPortEth1.checked)
		document.forms[0].QosPhyPortEth1.value = "Yes";
	else
		document.forms[0].QosPhyPortEth1.value = "No";
	
	if (document.forms[0].QoS2PortsFlag.value != "Yes") {	
		if(document.forms[0].QosPhyPortEth2.checked)
			document.forms[0].QosPhyPortEth2.value = "Yes";
		else
			document.forms[0].QosPhyPortEth2.value = "No";		
			
		if(document.forms[0].QosPhyPortEth3.checked)
			document.forms[0].QosPhyPortEth3.value = "Yes";
		else
			document.forms[0].QosPhyPortEth3.value = "No";
	}
/*		
	if(document.forms[0].QosPhyPortUsb0.checked)
		document.forms[0].QosPhyPortUsb0.value = "Yes";
	else
		document.forms[0].QosPhyPortUsb0.value = "No";

*/
	if (document.forms[0].wlanISExist.value == "On") {
		if(document.forms[0].QosMBSSIDNumberFlag.value == "1"){	
			if(document.forms[0].QosPhyPortRa0.checked)
				document.forms[0].QosPhyPortRa0.value = "Yes";
			else
				document.forms[0].QosPhyPortRa0.value = "No";
		}
		if(document.forms[0].QosMBSSIDNumberFlag.value == "2"){
			if(document.forms[0].QosPhyPortWLANMssid0.checked)
				document.forms[0].QosPhyPortWLANMssid0.value = "Yes"
			else
				document.forms[0].QosPhyPortWLANMssid1.value = "No"
			if(document.forms[0].QosPhyPortWLANMssid1.checked)
				document.forms[0].QosPhyPortWLANMssid1.value = "Yes"
			else
				document.forms[0].QosPhyPortWLANMssid1.value = "No"
		}
		if(document.forms[0].QosMBSSIDNumberFlag.value == "3"){
			if(document.forms[0].QosPhyPortWLANMssid0.checked){
				document.forms[0].QosPhyPortWLANMssid0.value = "Yes"
			}
			else
				document.forms[0].QosPhyPortWLANMssid0.value = "No"
			if(document.forms[0].QosPhyPortWLANMssid1.checked)
				document.forms[0].QosPhyPortWLANMssid1.value = "Yes"
			else
				document.forms[0].QosPhyPortWLANMssid1.value = "No"
			if(document.forms[0].QosPhyPortWLANMssid2.checked)
				document.forms[0].QosPhyPortWLANMssid2.value = "Yes"
			else
				document.forms[0].QosPhyPortWLANMssid2.value = "No"
		}
		if(document.forms[0].QosMBSSIDNumberFlag.value == "4"){	
			if(document.forms[0].QosPhyPortWLANMssid0.checked){	
				document.forms[0].QosPhyPortWLANMssid0.value = "Yes"
			}
			else
				document.forms[0].QosPhyPortWLANMssid0.value = "No"
			if(document.forms[0].QosPhyPortWLANMssid1.checked)
				document.forms[0].QosPhyPortWLANMssid1.value = "Yes"
			else
				document.forms[0].QosPhyPortWLANMssid1.value = "No"
			if(document.forms[0].QosPhyPortWLANMssid2.checked)
				document.forms[0].QosPhyPortWLANMssid2.value = "Yes"
			else
				document.forms[0].QosPhyPortWLANMssid2.value = "No"
			if(document.forms[0].QosPhyPortWLANMssid3.checked)
				document.forms[0].QosPhyPortWLANMssid3.value = "Yes"
			else
				document.forms[0].QosPhyPortWLANMssid3.value = "No"
		}
	}
}

function doAdd() {
	document.forms[0].QOS_Flag.value = 0;

	checkPhysicalPort();
}

function doIndexChange() {
	document.forms[0].QOS_Flag.value = 1;
	document.forms[0].submit();
	return;
}

function doDel(){
	document.forms[0].QOS_Flag.value = 2;
}              

function setProtoPort(protoIdx, dPort1, dPort2){
	var form = document.QoS_Form;
	
	form.QosProtocol.selectedIndex = protoIdx;
	form.QosDestPortValue1.value = dPort1;
	form.QosDestPortValue2.value = dPort2;
}         

function doQosAppIdxChange(){
	var form = document.QoS_Form;
	
	switch(form.QosApp.selectedIndex){
		case 0:
		default:
			break;
		case 1://IGMP
			form.QosDestIpValue.value = "224.0.0.0";
			form.QosDestMaskValue.value = "240.0.0.0";
			break;
		case 2://SIP
			setProtoPort(3, 5060, 5060);
			break;
		case 3://H.323
			setProtoPort(3, 1719, 1720);
			break;
		case 4://MGCP
			setProtoPort(3, 2427, 2427);
			break;
		case 5://SNMP
			setProtoPort(1, 161, 161);
			break;
		case 6://DNS
			setProtoPort(3, 53, 53);
			break;
		case 7://DHCP
			setProtoPort(3, 67, 67);
			form.QosSrcPortValue1.value = 68;
			form.QosSrcPortValue2.value = 68;
		break;
			case 8://RIP
			setProtoPort(3, 520, 520);
		break;
			case 9://RSTP
			setProtoPort(3, 554, 554);
			break;
		case 10://RTCP
			setProtoPort(3, 5005, 5005);
			break;
		case 11://RTP
			setProtoPort(3, 5004, 5004);
			break;
	}
}

function doQos1pAppIdxChange(){
	switch(document.QoS_Form.Qos8021pReApp.selectedIndex){
		case 0:
		default:
			break;
		case 1://ketNET
			document.QoS_Form.Qos8021pReValue.selectedIndex = 8;
			break;
		case 2://voice
			document.QoS_Form.Qos8021pReValue.selectedIndex = 7;
			break;
		case 3://vide0
			document.QoS_Form.Qos8021pReValue.selectedIndex = 6;
			break;
		case 4://IGMP
			document.QoS_Form.Qos8021pReValue.selectedIndex = 5;
			break;
		case 5://keyData
			document.QoS_Form.Qos8021pReValue.selectedIndex = 4;
			break;
	}
}
function onClickQosDiscipline(){
	var form = document.QoS_Form;

	
	if((!checkRangeValue(form.QosWRRweight1, form.QosWRRweight1, 1, 15, 7)) ||
       (!checkRangeValue(form.QosWRRweight2, form.QosWRRweight2, 1, 15, 7)) ||
       (!checkRangeValue(form.QosWRRweight3, form.QosWRRweight3, 1, 15, 7)) ||
       (!checkRangeValue(form.QosWRRweight4, form.QosWRRweight4, 1, 15, 7)))
		return false;
	else{		
		document.forms[0].QOS_Flag.value = 4;
		document.QoS_Form.Qosdisciplineflag.value = "Yes";	
		document.forms[0].submit();
	}
}
function onClickQosSummary() {
	window.open("/cgi-bin/adv_qoslist.cgi","QoS_List","toolbar=no,menubar=no,scrollbars=yes,height=600, width=850,location=0,left=100,top=100");
	return false;
}
</script>

</head>
<body onload="init();">
<form METHOD="POST" ACTION="/cgi-bin/adv_qos.asp" name="QoS_Form" onsubmit="return verifyForm()"><div align=center>
	<table width="760" align=center cellpadding="0" cellspacing="0">
		<tr>
			<td height="5" class="light-orange" colspan="5">&nbsp;</td>
		</tr>
		<tr>
			<td width="150" height="30" class="title-main"><font color="#ffffff">Quality of Service</font></td>
			<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td>
			<td width="10" ><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">QoS</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="Qos_active" VALUE="Yes"
				<%if TCWebAPI_get("QoS_Common", "Active", "h") = "Yes" then asp_write("checked") end if %> onclick="enableAll();init();"><font color="#000000">Activated</font>
				<INPUT TYPE="RADIO" NAME="Qos_active" VALUE="No"
				<%if TCWebAPI_get("QoS_Common", "Active", "h") = "No" then asp_write("checked") end if
				  if TCWebAPI_get("QoS_Common", "Active", "h") = "N/A" then asp_write("checked") end if%> onclick="disableAll();"><font color="#000000">Deactivated</font>
			</td>
		</tr>			
<!--
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Drop</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="Qosdrop" VALUE="WRED"
				<%if TCWebAPI_get("QoS_Common", "Drop", "h") = "WRED" then asp_write("checked") end if %>><font color="#000000">WRED</font>			
				<INPUT TYPE="RADIO" NAME="Qosdrop" VALUE="RED"
				<%if TCWebAPI_get("QoS_Common", "Drop", "h") = "RED" then asp_write("checked") end if %>><font color="#000000">RED</font>
				<INPUT TYPE="RADIO" NAME="Qosdrop" VALUE="TAIL"
				<%if TCWebAPI_get("QoS_Common", "Drop", "h") = "TAIL" then asp_write("checked") end if
				  if TCWebAPI_get("QoS_Common", "Drop", "h") = "N/A" then asp_write("checked") end if%>><font color="#000000">Tail drop</font>

			</td>
		</tr>	
-->
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Discipline</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="Qosdiscipline" VALUE="WRR"
				<%if TCWebAPI_get("QoS_Common", "Discipline", "h") = "WRR" then asp_write("checked") end if %>><font color="#000000">WRR</font>
				<INPUT TYPE="RADIO" NAME="Qosdiscipline" VALUE="SP"
				<%if TCWebAPI_get("QoS_Common", "Discipline", "h") = "SP" then asp_write("checked") end if
				  if TCWebAPI_get("QoS_Common", "Discipline", "h") = "N/A" then asp_write("checked") end if%>><font color="#000000">Strict Priority  </font>

				<INPUT TYPE="HIDDEN" NAME="Qosdisciplineflag" VALUE="No">

			</td>
		</tr>				
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">WRR weight</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<font color="#000000">Highest:</font><INPUT TYPE="TEXT" NAME="QosWRRweight1" SIZE="1" MAXLENGTH="2" VALUE="<%if TCWebAPI_get("QoS_Common", "WeightHighest", "h") <> "N/A" then TCWebAPI_get("QoS_Common", "WeightHighest", "s") else asp_Write("") end if %>">
				<font color="#000000">High:</font><INPUT TYPE="TEXT" NAME="QosWRRweight2" SIZE="1" MAXLENGTH="2" VALUE="<%if TCWebAPI_get("QoS_Common", "WeightHigh", "h") <> "N/A" then TCWebAPI_get("QoS_Common", "WeightHigh", "s") else asp_Write("") end if %>">
				<font color="#000000">Medium:</font><INPUT TYPE="TEXT" NAME="QosWRRweight3" SIZE="1" MAXLENGTH="2" VALUE="<%if TCWebAPI_get("QoS_Common", "WeightMedium", "h") <> "N/A" then TCWebAPI_get("QoS_Common", "WeightMedium", "s") else asp_Write("") end if %>">
				<font color="#000000">Low:</font><INPUT TYPE="TEXT" NAME="QosWRRweight4" SIZE="1" MAXLENGTH="2" VALUE="<%if TCWebAPI_get("QoS_Common", "WeightLow", "h") <> "N/A" then TCWebAPI_get("QoS_Common", "WeightLow", "s") else asp_Write("") end if %>"><font color="#000000">(valid:1~15)</font>
			</td>
		</tr>		
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right></td>
			<td class="tabdata" align=center></td>
			<td class="tabdata">				
				<INPUT TYPE="BUTTON" NAME="Qosdisciplinesave" VALUE="Discipline Save" onClick="onClickQosDiscipline();">
				<INPUT TYPE="BUTTON" NAME="Qos_Summary" VALUE="Rule&Action Summary" onClick="onClickQosSummary();">				  
			</td>
		</tr>
		<tr>
			<td class="title-sub"><font color="#000000">Rule</font></td>
			<td class="light-orange">&nbsp;</td><td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Rule Index</font></td>
			<td class="tabdata" align=center>:</td>
			<td  class="tabdata">
				<SELECT NAME="QosRuleIndex" SIZE="1" onChange="doIndexChange()">
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "0" then asp_Write("selected") end if %>>0
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "1" then asp_Write("selected") end if %>>1
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "2" then asp_Write("selected") end if %>>2
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "3" then asp_Write("selected") end if %>>3
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "4" then asp_Write("selected") end if %>>4
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "5" then asp_Write("selected") end if %>>5
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "6" then asp_Write("selected") end if %>>6
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "7" then asp_Write("selected") end if %>>7
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "8" then asp_Write("selected") end if %>>8
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "9" then asp_Write("selected") end if %>>9
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "10" then asp_Write("selected") end if %>>10
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "11" then asp_Write("selected") end if %>>11
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "12" then asp_Write("selected") end if %>>12
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "13" then asp_Write("selected") end if %>>13
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "14" then asp_Write("selected") end if %>>14
					<OPTION <%if TCWebAPI_get("WebCurSet_Entry", "qos_id", "h") = "15" then asp_Write("selected") end if %>>15
				</SELECT>
				<INPUT TYPE="HIDDEN" NAME="QOS_Flag" VALUE="0">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Active</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="QosRuleActive" VALUE="Yes" 
				<%if TCWebAPI_get("QoS_Entry", "Active", "h") = "Yes" then asp_write("checked") end if %> onclick="formAppToPhyport(1);fromDMacToPriority(1);init();"><font color="#000000">Activated</font>
				<INPUT TYPE="RADIO" NAME="QosRuleActive" VALUE="No" 
				<%if TCWebAPI_get("QoS_Entry", "Active", "h") = "No" then asp_write("checked") end if
				  if TCWebAPI_get("QoS_Entry", "Active", "h") = "N/A" then asp_write("checked") end if  %> onclick="formAppToPhyport(0);fromDMacToPriority(0);"><font color="#000000">Deactivated</font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Application</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="QosApp" SIZE="1" onchange="doQosAppIdxChange();">
					<OPTION>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "IGMP" then asp_write("selected") end if %>>IGMP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "SIP" then asp_write("selected") end if %>>SIP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "H.323" then asp_write("selected") end if %>>H.323
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "MGCP" then asp_write("selected") end if %>>MGCP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "SNMP" then asp_write("selected") end if %>>SNMP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "DNS" then asp_write("selected") end if %>>DNS
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "DHCP" then asp_write("selected") end if %>>DHCP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "RIP" then asp_write("selected") end if %>>RIP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "RSTP" then asp_write("selected") end if %>>RSTP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "RTCP" then asp_write("selected") end if %>>RTCP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Application", "h") = "RTP" then asp_write("selected") end if %>>RTP
				</SELECT>
			</td>
		</tr>
		<tr>
			<td width="150" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td width="150" class="tabdata" align=right><font color="#000000">Physical Ports</font></td>
			<td class="tabdata" align=center>:</td>
			<td width="440" height="39" class="tabdata">
				<table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
					<tr>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortEth0" <%if TCWebAPI_get("QoS_Entry", "eth0.1", "h") = "Yes" then asp_write("checked") end if %>></td>

						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortEth1" <%if TCWebAPI_get("QoS_Entry", "eth0.2", "h") = "Yes" then asp_write("checked") end if %>></td>
						<% if tcWebApi_get("WebCustom_Entry","is2PSupported","h") <> "Yes" then %>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortEth2" <%if TCWebAPI_get("QoS_Entry", "eth0.3", "h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortEth3" <%if TCWebAPI_get("QoS_Entry", "eth0.4", "h") = "Yes" then asp_write("checked") end if %>></td>
						<% end if %>
<!--
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortUsb0" <%if TCWebAPI_get("QoS_Entry", "usb0", "h") = "Yes" then asp_write("checked") end if %>></td>
-->
<% if tcWebApi_get("Info_WLan","isExist","h") = "On" then %>
<%
	if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then
%>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortRa0" <%if TCWebAPI_get("QoS_Entry", "ra0", "h") = "Yes" then asp_write("checked") end if %>></td>			
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
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid0" <%if TCWebAPI_get("QoS_Entry","ra0","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid1" <%if TCWebAPI_get("QoS_Entry","ra1","h") = "Yes" then asp_write("checked") end if %>></td>
					</tr>
					<tr class="tabdata">
					<td class="tabdata" align=center><font color="#000000">ra0</font></td>
					<td class="tabdata" align=center><font color="#000000">ra1</font></td>
					</tr>
<%
	elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "3" then
%>
					<tr>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid0" <%if TCWebAPI_get("QoS_Entry","ra0","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid1" <%if TCWebAPI_get("QoS_Entry","ra1","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid2" <%if TCWebAPI_get("QoS_Entry","ra2","h") = "Yes" then asp_write("checked") end if %>></td>	
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
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid0" <%if TCWebAPI_get("QoS_Entry","ra0","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid1" <%if TCWebAPI_get("QoS_Entry","ra1","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid2" <%if TCWebAPI_get("QoS_Entry","ra2","h") = "Yes" then asp_write("checked") end if %>></td>
						<td width="40" align=center><INPUT TYPE="CHECKBOX" NAME="QosPhyPortWLANMssid3" <%if TCWebAPI_get("QoS_Entry","ra3","h") = "Yes" then asp_write("checked") end if %>></td>
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
<% end if %>

				</table>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Destination MAC</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosDestMacValue" SIZE="17" MAXLENGTH="17" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "DesMac", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "DesMac", "s") else asp_Write("") end if %>" onBlur="doMACcheck(this)">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">IP</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosDestIpValue" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "DesIP", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "DesIP", "s") else asp_Write("") end if %>">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Mask</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosDestMaskValue" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "DesMask", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "DesMask", "s") else asp_Write("") end if %>">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Port Range</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosDestPortValue1" SIZE="3" MAXLENGTH="5" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "DesPortRangeBegin", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "DesPortRangeBegin", "s") else asp_Write("") end if %>">~
				<INPUT TYPE="TEXT" NAME="QosDestPortValue2" SIZE="3" MAXLENGTH="5" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "DesPortRangeEnd", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "DesPortRangeEnd", "s") else asp_Write("") end if %>">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Source MAC</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosSrcMacValue" SIZE="17" MAXLENGTH="17" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "SrcMac", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "SrcMac", "s") else asp_Write("") end if %>" onBlur="doMACcheck(this)">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">IP</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosSrcIpValue" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "SrcIP", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "SrcIP", "s") else asp_Write("") end if %>">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td><td class="tabdata" align=right><font color="#000000">Mask</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosSrcMaskValue" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "SrcMask", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "SrcMask", "s") else asp_Write("") end if %>">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Port Range</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosSrcPortValue1" SIZE="3" MAXLENGTH="5" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "SrcPortRangeBegin", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "SrcPortRangeBegin", "s") else asp_Write("") end if %>">~
				<INPUT TYPE="TEXT" NAME="QosSrcPortValue2" SIZE="3" MAXLENGTH="5" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "SrcPortRangeEnd", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "SrcPortRangeEnd", "s") else asp_Write("") end if %>">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Protocol ID</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="QosProtocol" SIZE="1">
					<OPTION SELECTED>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ProtocolID", "h") = "TCP/UDP" then asp_write("selected") end if %>>TCP/UDP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ProtocolID", "h") = "TCP" then asp_write("selected") end if %>>TCP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ProtocolID", "h") = "UDP" then asp_write("selected") end if %>>UDP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ProtocolID", "h") = "ICMP" then asp_write("selected") end if %>>ICMP
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ProtocolID", "h") = "IGMP" then asp_write("selected") end if %>>IGMP
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Vlan ID Range</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosVIDValue1" SIZE="3" MAXLENGTH="5" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "VlanIDRangeBegin", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "VlanIDRangeBegin", "s") else asp_Write("") end if %>">~
				<INPUT TYPE="TEXT" NAME="QosVIDValue2" SIZE="3" MAXLENGTH="5" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "VlanIDRangeEnd", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "VlanIDRangeEnd", "s") else asp_Write("") end if %>">
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">IPP/DS Field</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="Qos_IPP_DSCP1" VALUE="IPPTOS"
				<%if TCWebAPI_get("QoS_Entry", "IPPDSField", "h") = "IPPTOS" then asp_write("checked") end if %> onclick="ornIppTosDscp(2);"><font color="#000000">IPP/TOS</font>&nbsp;&nbsp;
				<INPUT TYPE="RADIO" NAME="Qos_IPP_DSCP1" VALUE="DSCP"
				<%if TCWebAPI_get("QoS_Entry", "IPPDSField", "h") = "DSCP" then asp_write("checked") end if
				  if TCWebAPI_get("QoS_Entry", "IPPDSField", "h") = "N/A" then asp_write("checked") end if %> onclick="ornIppTosDscp(3)"><font color="#000000">DSCP</font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">IP Precedence Range</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="QosIPPValue1" SIZE="1">
					<OPTION>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "0" then asp_write("selected") end if %>>0
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "1" then asp_write("selected") end if %>>1
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "2" then asp_write("selected") end if %>>2
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "3" then asp_write("selected") end if %>>3
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "4" then asp_write("selected") end if %>>4
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "5" then asp_write("selected") end if %>>5
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "6" then asp_write("selected") end if %>>6
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeBegin", "h") = "7" then asp_write("selected") end if %>>7
				</SELECT>~
				<SELECT NAME="QosIPPValue2" SIZE="1">
					<OPTION>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "0" then asp_write("selected") end if %>>0
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "1" then asp_write("selected") end if %>>1
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "2" then asp_write("selected") end if %>>2
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "3" then asp_write("selected") end if %>>3
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "4" then asp_write("selected") end if %>>4
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "5" then asp_write("selected") end if %>>5
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "6" then asp_write("selected") end if %>>6
					<OPTION <%if TCWebAPI_get("QoS_Entry", "IPPRangeEnd", "h") = "7" then asp_write("selected") end if %>>7
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Type of Service</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="QosTOS" SIZE="1">
					<OPTION>
					<OPTION value="Normal service" <%if TCWebAPI_get("QoS_Entry", "TypeOfService", "h") = "Normal service" then asp_write("selected") end if %>>Normal service
					<OPTION value="Minimize delay" <%if TCWebAPI_get("QoS_Entry", "TypeOfService", "h") = "Minimize delay" then asp_write("selected") end if %>>Minimize delay
					<OPTION value="Maximize throughput" <%if TCWebAPI_get("QoS_Entry", "TypeOfService", "h") = "Maximize throughput" then asp_write("selected") end if %>>Maximize throughput
					<OPTION value="Maximize reliability" <%if TCWebAPI_get("QoS_Entry", "TypeOfService", "h") = "Maximize reliability" then asp_write("selected") end if %>>Maximize reliability
					<OPTION value="Minimize monetary cost" <%if TCWebAPI_get("QoS_Entry", "TypeOfService", "h") = "Minimize monetary cost" then asp_write("selected") end if %>>Minimize monetary cost
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">DSCP Range</font></td>
			<td class="tabdata" align=center>:</td><td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosDSCPValue1" SIZE="3" MAXLENGTH="2" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "DSCPRangeBegin", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "DSCPRangeBegin", "s") else asp_Write("") end if %>">~
				<INPUT TYPE="TEXT" NAME="QosDSCPValue2" SIZE="3" MAXLENGTH="2" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "DSCPRangeEnd", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "DSCPRangeEnd", "s") else asp_Write("") end if %>"><font color="#000000">(Value Range: 0 ~ 63)</font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">802.1p</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="Qos8021pValue1" SIZE="1">
					<OPTION>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "0" then asp_write("selected") end if %>>0
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "1" then asp_write("selected") end if %>>1
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "2" then asp_write("selected") end if %>>2
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "3" then asp_write("selected") end if %>>3
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "4" then asp_write("selected") end if %>>4
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "5" then asp_write("selected") end if %>>5
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "6" then asp_write("selected") end if %>>6
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpBegin", "h") = "7" then asp_write("selected") end if %>>7
				</SELECT>~
					<SELECT NAME="Qos8021pValue2" SIZE="1">
					<OPTION>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "0" then asp_write("selected") end if %>>0
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "1" then asp_write("selected") end if %>>1
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "2" then asp_write("selected") end if %>>2
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "3" then asp_write("selected") end if %>>3
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "4" then asp_write("selected") end if %>>4
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "5" then asp_write("selected") end if %>>5
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "6" then asp_write("selected") end if %>>6
					<OPTION <%if TCWebAPI_get("QoS_Entry", "dotpEnd", "h") = "7" then asp_write("selected") end if %>>7
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="title-sub"><font color="#000000">Action</font></td>
			<td class="light-orange"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">IPP/DS Field</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="Qos_IPP_DSCP2" VALUE="IPPTOS"
				<%if TCWebAPI_get("QoS_Entry", "ActIPPDSField", "h") = "IPPTOS" then asp_write("checked") end if %> onclick="reIppTosDscp(2);"><font color="#000000">IPP/TOS</font>&nbsp;&nbsp;
				<INPUT TYPE="RADIO" NAME="Qos_IPP_DSCP2" VALUE="DSCP"
				<%if TCWebAPI_get("QoS_Entry", "ActIPPDSField", "h") = "DSCP" then asp_write("checked") end if
				  if TCWebAPI_get("QoS_Entry", "ActIPPDSField", "h") = "N/A" then asp_write("checked") end if %> onclick="reIppTosDscp(3);"><font color="#000000">DSCP</font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">IP Precedence Remarking</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="QosReIPPValue" SIZE="1">
					<OPTION>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "0" then asp_write("selected") end if %>>0
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "1" then asp_write("selected") end if %>>1
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "2" then asp_write("selected") end if %>>2
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "3" then asp_write("selected") end if %>>3
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "4" then asp_write("selected") end if %>>4
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "5" then asp_write("selected") end if %>>5
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "6" then asp_write("selected") end if %>>6
					<OPTION <%if TCWebAPI_get("QoS_Entry", "ActIPPRemarking", "h") = "7" then asp_write("selected") end if %>>7
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Type of Service Remarking</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="QosReTOS" SIZE="1">
					<OPTION SELECTED>
					<OPTION value="Normal service" <%if TCWebAPI_get("QoS_Entry", "ActToSRemarking", "h") = "Normal service" then asp_write("selected") end if %>>Normal service
					<OPTION value="Minimize delay" <%if TCWebAPI_get("QoS_Entry", "ActToSRemarking", "h") = "Minimize delay" then asp_write("selected") end if %>>Minimize delay
					<OPTION value="Maximize throughput" <%if TCWebAPI_get("QoS_Entry", "ActToSRemarking", "h") = "Maximize throughput" then asp_write("selected") end if %>>Maximize throughput
					<OPTION value="Maximize reliability" <%if TCWebAPI_get("QoS_Entry", "ActToSRemarking", "h") = "Maximize reliability" then asp_write("selected") end if %>>Maximize reliability
					<OPTION value="Minimize monetary cost" <%if TCWebAPI_get("QoS_Entry", "ActToSRemarking", "h") = "Minimize monetary cost" then asp_write("selected") end if %>>Minimize monetary cost
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">DSCP Remarking</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="QosReDSCPValue" SIZE="3" MAXLENGTH="2" VALUE=
				"<%if TCWebAPI_get("QoS_Entry", "ActDSCPRemarking", "h") <> "N/A" then TCWebAPI_get("QoS_Entry", "ActDSCPRemarking", "s") else asp_Write("") end if %>"><font color="#000000">(Value Range: 0 ~ 63)</font>
			</td>
		</tr>
	<!--
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">802.1p Remarking</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="Qos8021pReValue" SIZE="1">
					<OPTION>
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "0" then asp_write("selected") end if %>>0
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "1" then asp_write("selected") end if %>>1
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "2" then asp_write("selected") end if %>>2
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "3" then asp_write("selected") end if %>>3
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "4" then asp_write("selected") end if %>>4
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "5" then asp_write("selected") end if %>>5
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "6" then asp_write("selected") end if %>>6
					<OPTION <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarkingNum", "h") = "7" then asp_write("selected") end if %>>7
				</SELECT>
				<SELECT NAME="Qos8021pReApp" SIZE="1" onchange="doQos1pAppIdxChange();">
					<OPTION>
					<OPTION value="Key Net Traffic(RIP, OSPF)" <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarking", "h") = "Key Net Traffic(RIP, OSPF)" then asp_write("selected") end if %>>Key Net Traffic(RIP, OSPF)
					<OPTION value="Voice" <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarking", "h") = "Voice" then asp_write("selected") end if %>>Voice
					<OPTION value="Video" <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarking", "h") = "Video" then asp_write("selected") end if %>>Video
					<OPTION value="IGMP" <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarking", "h") = "IGMP" then asp_write("selected") end if %>>IGMP
					<OPTION value="Key Data" <%if TCWebAPI_get("QoS_Entry", "Act8021pRemarking", "h") = "Key Data" then asp_write("selected") end if %>>Key Data
				</SELECT>
			</td>
		</tr>
	-->
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata" align=right><font color="#000000">Queue #</font></td>
			<td class="tabdata" align=center>:</td>
			<td class="tabdata">
				<SELECT NAME="QosConfigPriority" SIZE="1">
					<OPTION>
					<OPTION value="Low" <%if TCWebAPI_get("QoS_Entry", "ActQueue", "h") = "Low" then asp_write("selected") end if %>>Low
					<OPTION value="Medium" <%if TCWebAPI_get("QoS_Entry", "ActQueue", "h") = "Medium" then asp_write("selected") end if %>>Medium
					<OPTION value="High" <%if TCWebAPI_get("QoS_Entry", "ActQueue", "h") = "High" then asp_write("selected") end if %>>High
					<OPTION value="Highest" <%if TCWebAPI_get("QoS_Entry", "ActQueue", "h") = "Highest" then asp_write("selected") end if %>>Highest
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange"></td>
			<td class="light-orange"></td>
			<td>&nbsp;</td><td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0">
		<tr>
			<td width="160" height="42" class="black">&nbsp;</td>
			<td width="160" class="orange">&nbsp;</td>
			<td width="440" class="orange">
				<INPUT TYPE="SUBMIT" NAME="QoS_Add" VALUE="ADD" onClick="doAdd();">
				<INPUT TYPE="SUBMIT" NAME="QoS_Del" VALUE="DELETE" onClick="doDel();">
				<INPUT TYPE="SUBMIT" NAME="QoS_Cancel" VALUE="CANCEL" onClick="document.forms[0].QOS_Flag.value = 3; javascript:window.location='adv_qos.htm'">
					<INPUT TYPE="HIDDEN" NAME="QosMBSSIDNumberFlag" VALUE="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">
					<INPUT TYPE="HIDDEN" NAME="QoS2PortsFlag" VALUE="<%tcWebApi_get("WebCustom_Entry","is2PSupported","s")%>">
					<INPUT TYPE="HIDDEN" NAME="wlanISExist" ID="IDwlanISExist" VALUE="<%tcWebApi_get("Info_WLan","isExist","s")%>">
			</td>
		</tr>
	</table>
</div>
</form>
</body>
</html>
