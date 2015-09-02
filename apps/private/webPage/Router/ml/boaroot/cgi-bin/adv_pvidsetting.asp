<% 
if request_form("VlanTag_flag") = "1" then
	TCWebAPI_unset("VlanTag_Entry")
	TCWebAPI_set("VlanTag_Entry", "portsNum", "VlanTag_portsNum")
	
	TCWebAPI_set("VlanTag_Entry", "e0pvid", "VlanTag_e0pvid")
	TCWebAPI_set("VlanTag_Entry", "e1pvid", "VlanTag_e1pvid")
	TCWebAPI_set("VlanTag_Entry", "e2pvid", "VlanTag_e2pvid")
	TCWebAPI_set("VlanTag_Entry", "e3pvid", "VlanTag_e3pvid")
	TCWebAPI_set("VlanTag_Entry", "e0tag", "VlanTag_e0tag")
	TCWebAPI_set("VlanTag_Entry", "e1tag", "VlanTag_e1tag")
	TCWebAPI_set("VlanTag_Entry", "e2tag", "VlanTag_e2tag")
	TCWebAPI_set("VlanTag_Entry", "e3tag", "VlanTag_e3tag")
	
	TCWebAPI_set("VlanTag_Entry", "u0pvid", "VlanTag_u0pvid")
	TCWebAPI_set("VlanTag_Entry", "u0tag", "VlanTag_u0tag")

	TCWebAPI_set("VlanTag_Entry", "r0pvid", "VlanTag_r0pvid")
	TCWebAPI_set("VlanTag_Entry", "r0tag", "VlanTag_r0tag")
if request_form("UserMode") = "1" then
	if request_form("wlanPvid_1") <> "N/A" then
		TCWebAPI_set("VlanTag_Entry", "r1pvid", "wlanPvid_1")	
	end if
	if request_form("wlanPvid_2") <> "N/A" then
		TCWebAPI_set("VlanTag_Entry", "r2pvid", "wlanPvid_2")
	end if
	if request_form("wlanPvid_3") <> "N/A" then
		TCWebAPI_set("VlanTag_Entry", "r3pvid", "wlanPvid_3")
	end if
else	
	TCWebAPI_set("VlanTag_Entry", "r1pvid", "VlanTag_r1pvid")
	TCWebAPI_set("VlanTag_Entry", "r1tag", "VlanTag_r1tag")
	TCWebAPI_set("VlanTag_Entry", "r2pvid", "VlanTag_r2pvid")
	TCWebAPI_set("VlanTag_Entry", "r2tag", "VlanTag_r2tag")
	TCWebAPI_set("VlanTag_Entry", "r3pvid", "VlanTag_r3pvid")
	TCWebAPI_set("VlanTag_Entry", "r3tag", "VlanTag_r3tag")
end if	
	TCWebAPI_set("VlanTag_Entry", "w0pvid", "VlanTag_w0pvid")
	TCWebAPI_set("VlanTag_Entry", "w1pvid", "VlanTag_w1pvid")
	TCWebAPI_set("VlanTag_Entry", "w2pvid", "VlanTag_w2pvid")
	TCWebAPI_set("VlanTag_Entry", "w3pvid", "VlanTag_w3pvid")
	TCWebAPI_set("VlanTag_Entry", "w4pvid", "VlanTag_w4pvid")
	TCWebAPI_set("VlanTag_Entry", "w5pvid", "VlanTag_w5pvid")
	TCWebAPI_set("VlanTag_Entry", "w6pvid", "VlanTag_w6pvid")
	TCWebAPI_set("VlanTag_Entry", "w7pvid", "VlanTag_w7pvid")	
	TCWebAPI_set("VlanTag_Entry", "w0tag", "VlanTag_w0tag")
	TCWebAPI_set("VlanTag_Entry", "w1tag", "VlanTag_w1tag")
	TCWebAPI_set("VlanTag_Entry", "w2tag", "VlanTag_w2tag")
	TCWebAPI_set("VlanTag_Entry", "w3tag", "VlanTag_w3tag")
	TCWebAPI_set("VlanTag_Entry", "w4tag", "VlanTag_w4tag")
	TCWebAPI_set("VlanTag_Entry", "w5tag", "VlanTag_w5tag")
	TCWebAPI_set("VlanTag_Entry", "w6tag", "VlanTag_w6tag")	
	TCWebAPI_set("VlanTag_Entry", "w7tag", "VlanTag_w7tag")
if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then
	TCWebAPI_set("VlanTag_Entry", "w8pvid", "VlanTag_w8pvid")
	TCWebAPI_set("VlanTag_Entry", "w9pvid", "VlanTag_w9pvid")	
	TCWebAPI_set("VlanTag_Entry", "w8tag", "VlanTag_w8tag")
	TCWebAPI_set("VlanTag_Entry", "w9tag", "VlanTag_w9tag")
end if
if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then
	TCWebAPI_set("VlanTag_Entry", "w10pvid", "VlanTag_w10pvid")
	TCWebAPI_set("VlanTag_Entry", "w10tag", "VlanTag_w10tag")
end if
	
	TCWebAPI_set("VlanTag_Entry", "e0sw", "LAN1_Active")
	TCWebAPI_set("VlanTag_Entry", "e1sw", "LAN2_Active")
	TCWebAPI_set("VlanTag_Entry", "e2sw", "LAN3_Active")
	TCWebAPI_set("VlanTag_Entry", "e3sw", "LAN4_Active")
	
	TCWebAPI_set("VlanTag_Entry", "u0sw", "USB0_Active")
	
	TCWebAPI_set("VlanTag_Entry", "r0sw", "SSID1_Active")
	if request_Form("UserMode") = "1" then
		if request_Form("wlanSw_1") <> "N/A" then
			TCWebAPI_set("VlanTag_Entry", "r1sw", "wlanSw_1")
		end if
		if request_Form("wlanSw_2") <> "N/A" then
			TCWebAPI_set("VlanTag_Entry", "r2sw", "wlanSw_2")
		end if
		if request_Form("wlanSw_3") <> "N/A" then
			TCWebAPI_set("VlanTag_Entry", "r3sw", "wlanSw_3")
		end if
	else
		TCWebAPI_set("VlanTag_Entry", "r1sw", "SSID2_Active")
		TCWebAPI_set("VlanTag_Entry", "r2sw", "SSID3_Active")
		TCWebAPI_set("VlanTag_Entry", "r3sw", "SSID4_Active")
	end if
	
	TCWebAPI_set("VlanTag_Entry", "w0sw", "PVC1_Active")
	TCWebAPI_set("VlanTag_Entry", "w1sw", "PVC2_Active")
	TCWebAPI_set("VlanTag_Entry", "w2sw", "PVC3_Active")
	TCWebAPI_set("VlanTag_Entry", "w3sw", "PVC4_Active")
	TCWebAPI_set("VlanTag_Entry", "w4sw", "PVC5_Active")
	TCWebAPI_set("VlanTag_Entry", "w5sw", "PVC6_Active")
	TCWebAPI_set("VlanTag_Entry", "w6sw", "PVC7_Active")
	TCWebAPI_set("VlanTag_Entry", "w7sw", "PVC8_Active")
if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then
	TCWebAPI_set("VlanTag_Entry", "w8sw", "PVC9_Active")
	TCWebAPI_set("VlanTag_Entry", "w9sw", "PVC10_Active")
end if
if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then
	TCWebAPI_set("VlanTag_Entry", "w10sw", "PVC11_Active")
end if
	
	TCWebAPI_set("VlanTag_Entry", "Active", "VlanTag_Active")
	TCWebAPI_commit("VlanTag_Entry")
elseif request_form("VlanTag_flag") = "2" then
	TCWebAPI_set("VlanTag_Entry", "Active", "VlanTag_Active")
	TCWebAPI_commit("VlanTag_Entry")
end if
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="javascript" type="text/javascript">
/* init the whole web page */
function isNumeric(s)
{
	var len = s.length;
	var ch;
	
	if (len == 0)
		return false;
		
	for (i = 0; i < len; i++) {
		ch = s.charAt(i);
		if (ch > '9' ||ch < '0') {
			return false;
		}
	}
	
	return true;
}

function init()
{
	var form=document.VlanTag_Form;
	var i;
	if(document.forms[0].UserMode.value == "1"){
		var bssidNum =1;
	}
	else{
		var bssidNum = parseInt(document.getElementById("MBSSIDNumber").value);
	}
	var usbNum = parseInt(document.getElementById("USBNumber").value);
	if (document.VlanTag_Form.elements[1].checked) {
		for (i = 2; i <= 49+usbNum*4 + bssidNum*4; i++)
			document.VlanTag_Form.elements[i].disabled = true;
	}
	else {
		for (i = 2; i <= 49+usbNum*4 + bssidNum*4; i++)
			document.VlanTag_Form.elements[i].disabled = false;
		
		for (i = 2; i <=46 +usbNum*4 + bssidNum*4; i+=4) {
			if (document.VlanTag_Form.elements[i].checked) {
				document.VlanTag_Form.elements[i+2].disabled = false;
				document.VlanTag_Form.elements[i+3].disabled = false;
			}
			else {
				document.VlanTag_Form.elements[i+2].disabled = true;
				document.VlanTag_Form.elements[i+3].disabled = true;
			}
		}
	}
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	if (document.VlanTag_Form.VlanTag_Active[0].checked) {
			form.PVC9_Active[0].disabled=false;
			form.PVC9_Active[1].disabled=false;
			form.PVC10_Active[0].disabled=false;
			form.PVC10_Active[1].disabled=false;
		if(form.PVC9_Active[0].checked){
			form.VlanTag_w8pvid.disabled=false;
			form.VlanTag_w8tag.disabled=false;
		}
		else{
			form.VlanTag_w8pvid.disabled=true;
			form.VlanTag_w8tag.disabled=true;
		}
		if(form.PVC10_Active[0].checked){
			form.VlanTag_w9pvid.disabled=false;
			form.VlanTag_w9tag.disabled=false;
		}
		else{
			form.VlanTag_w9pvid.disabled=true;
			form.VlanTag_w9tag.disabled=true;
		}
	}
	else{
			form.PVC9_Active[0].disabled=true;
			form.PVC9_Active[1].disabled=true;
			form.VlanTag_w8pvid.disabled=true;
			form.VlanTag_w8tag.disabled=true;
			form.PVC10_Active[0].disabled=true;
			form.PVC10_Active[1].disabled=true;
			form.VlanTag_w9pvid.disabled=true;
			form.VlanTag_w9tag.disabled=true;
	}
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	if (document.VlanTag_Form.VlanTag_Active[0].checked) {
			form.PVC11_Active[0].disabled=false;
			form.PVC11_Active[1].disabled=false;
		if(form.PVC11_Active[0].checked){
			form.VlanTag_w10pvid.disabled=false;
			form.VlanTag_w10tag.disabled=false;
		}
		else{
			form.VlanTag_w10pvid.disabled=true;
			form.VlanTag_w10tag.disabled=true;
		}
	}
	else{
			form.PVC11_Active[0].disabled=true;
			form.PVC11_Active[1].disabled=true;
			form.VlanTag_w10pvid.disabled=true;
			form.VlanTag_w10tag.disabled=true;
	}
<%end if%>
	
	switch (bssidNum) {
		case 1:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			break;
		case 2:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			document.VlanTag_Form.VlanTag_r1tag.disabled = true;
			break;
		case 3:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			document.VlanTag_Form.VlanTag_r1tag.disabled = true;
			document.VlanTag_Form.VlanTag_r2tag.disabled = true;
			break;
		case 4:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			document.VlanTag_Form.VlanTag_r1tag.disabled = true;
			document.VlanTag_Form.VlanTag_r2tag.disabled = true;
			document.VlanTag_Form.VlanTag_r3tag.disabled = true;
			break;
		default:
			break;
	}
}
function wlan_init(index)
{
	var num = parseInt(index);
	if(document.VlanTag_Form.usbSupport.value ==1){
		num = num+54;
		init2(num);
	}
	else{
		num = num+50;
		init2(num);
	}
}
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
function ptm_init(barrier)
{
	var form=document.VlanTag_Form;

	with (document.VlanTag_Form){
		if(barrier==0){
			if(PVC9_Active[0].checked == true){
				form.VlanTag_w8pvid.disabled=false;
				form.VlanTag_w8tag.disabled=false;
			}
			else{
				form.VlanTag_w8pvid.disabled=true;
				form.VlanTag_w8tag.disabled=true;
			}
		}
		else if(barrier==1){
			if(PVC10_Active[0].checked == true){
				form.VlanTag_w9pvid.disabled=false;
				form.VlanTag_w9tag.disabled=false;
			}
			else{
				form.VlanTag_w9pvid.disabled=true;
				form.VlanTag_w9tag.disabled=true;
			}
		}
	}
}
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
function wan0_init()
{
	var form=document.VlanTag_Form;

	with (document.VlanTag_Form){
		if(PVC11_Active[0].checked == true){
			form.VlanTag_w10pvid.disabled=false;
			form.VlanTag_w10tag.disabled=false;
		}
		else{
			form.VlanTag_w10pvid.disabled=true;
			form.VlanTag_w10tag.disabled=true;
		}
	}

}
<%end if%>

/* init the specified web element */
function init2(i)
{
	if(document.forms[0].UserMode.value == "1"){
		var bssidNum =1;
	}
	else{
		var bssidNum = parseInt(document.getElementById("MBSSIDNumber").value);
	}
	
	if (document.VlanTag_Form.elements[0].checked) {
		if (parseInt(i)%2 == 0) {
			document.VlanTag_Form.elements[parseInt(i)+2].disabled = false;
			document.VlanTag_Form.elements[parseInt(i)+3].disabled = false;
		}
		else {
			document.VlanTag_Form.elements[parseInt(i)+1].disabled = true;
			document.VlanTag_Form.elements[parseInt(i)+2].disabled = true;
		}
	}
	
	/* disable tag function of ssid */
	switch (bssidNum) {
		case 1:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			break;
		case 2:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			document.VlanTag_Form.VlanTag_r1tag.disabled = true;
			break;
		case 3:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			document.VlanTag_Form.VlanTag_r1tag.disabled = true;
			document.VlanTag_Form.VlanTag_r2tag.disabled = true;
			break;
		case 4:
			document.VlanTag_Form.VlanTag_r0tag.disabled = true;
			document.VlanTag_Form.VlanTag_r1tag.disabled = true;
			document.VlanTag_Form.VlanTag_r2tag.disabled = true;
			document.VlanTag_Form.VlanTag_r3tag.disabled = true;
			break;
		default:
			break;
	}
}

function vlantag_save()
{
	var form=document.VlanTag_Form;
	var tmpTag, tmpPort;
	var i, pvid;
	if(document.forms[0].UserMode.value==1){
		var bssidNum = 1;
	}else{
		var bssidNum = parseInt(document.getElementById("MBSSIDNumber").value);
	}
	var usbNum = parseInt(document.getElementById("USBNumber").value);
	
	for (i = 4; i <=50+usbNum*4 + bssidNum*4; i+=4) {
		
		if (!isNumeric(document.VlanTag_Form.elements[i].value)) {
			alert("<%tcWebApi_get("String_Entry","PvidsettingJS0Text","s")%>");
			return false;	
		}
		
		pvid = parseInt(document.VlanTag_Form.elements[i].value);
		if (pvid < 0 || pvid > 4095) {
			alert("<%tcWebApi_get("String_Entry","PvidsettingJS1Text","s")%>");
			return false;
		}
	}

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	if (!isNumeric(form.VlanTag_w8pvid.value) || !isNumeric(form.VlanTag_w9pvid.value)){
			alert("<%tcWebApi_get("String_Entry","PvidsettingJS0Text","s")%>");
			return false;
	}
	pvid = parseInt(form.VlanTag_w8pvid.value);
	if (pvid < 0 || pvid > 4095) {
		alert("<%tcWebApi_get("String_Entry","PvidsettingJS1Text","s")%>");
		return false;
	}
	pvid = parseInt(form.VlanTag_w9pvid.value);
	if (pvid < 0 || pvid > 4095) {
		alert("<%tcWebApi_get("String_Entry","PvidsettingJS1Text","s")%>");
		return false;
	}
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	if (!isNumeric(form.VlanTag_w10pvid.value)){
			alert("<%tcWebApi_get("String_Entry","PvidsettingJS0Text","s")%>");
			return false;
	}
	pvid = parseInt(form.VlanTag_w10pvid.value);
	if (pvid < 0 || pvid > 4095) {
		alert("<%tcWebApi_get("String_Entry","PvidsettingJS1Text","s")%>");
		return false;
	}
<%end if%>
	
	/* ethernet ports */
	for (port = 0; port < 4; port++) {
		var Tag = document.getElementById("IDetag" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.value = "No";
		}
	}

	/* wireless ports */
	for (port = 0; port < bssidNum; port++) {
		var Tag = document.getElementById("IDrtag" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.value = "No";
		}
	}
	
	/* wan ports */
	for (port = 0; port < 8; port++) {
		var Tag = document.getElementById("IDwtag" + port);
		if (Tag.checked) {
			Tag.value = "Yes";
		}
		else {
			Tag.value = "No";
		}
	}
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	for (tmpPort=8; tmpPort<10; tmpPort++) {
		tmpTag = document.getElementById("IDwtag" + tmpPort);
		if (tmpTag.checked) {
			tmpTag.value = "Yes";
		}
		else {
			tmpTag.value = "No";
		}
	}
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	for (tmpPort=10; tmpPort<11; tmpPort++) {
		tmpTag = document.getElementById("IDwtag" + tmpPort);
		if (tmpTag.checked) {
			tmpTag.value = "Yes";
		}
		else {
			tmpTag.value = "No";
		}
	}
<%end if%>

	/* vlan tag function is enable, VlanTag_flag = 1, or VlanTag_flag = 2 */
	if (document.VlanTag_Form.elements[0].checked) {
		document.VlanTag_Form.VlanTag_flag.value = "1";
	}
	else if (document.VlanTag_Form.elements[1].checked) {
		document.VlanTag_Form.VlanTag_flag.value = "2";
	}

	document.VlanTag_Form.submit();
}

/* 
function verifyForm()
{
	var form = document.VLANPVID_form;
	if(form.ICIndex.value == 0)
	{
		for(i = 2; i < 11; i++)
		{
			var pvid = parseInt(form.elements[i].value);
			if(isNaN(pvid) || pvid < 1 || pvid > 8)
			{
				alert("The range of pvid value is 1 ~ 8");
				form.elements[i].focus();
				return false;
			}
		}
		pvid = parseInt(form.Ether0_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Ether0_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether1_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Ether1_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Ether2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Ether3_pvid.focus();
			return false;
		}
		pvid = parseInt(form.USB_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.USB_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");	
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless3_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless3_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless4_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{		
			alert("The range of pvid value is 1 ~ 8");			
			form.Wireless4_pvid.focus();
			return false;
		}
	}
	else
	{
		for(i = 2; i < 11; i++)
		{
			var pvid = parseInt(form.elements[i].value);
			if(isNaN(pvid) || pvid < 1 || pvid > 4094)
			{		
				alert("The range of pvid value is 1 ~ 4094");						
				form.elements[i].focus();
				return false;
			}
		}
		pvid = parseInt(form.Ether0_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Ether0_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether1_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Ether1_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Ether2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Ether3_pvid.focus();
			return false;
		}

		pvid = parseInt(form.USB_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");						
			form.USB_pvid.focus();
			return false;
		}

		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");			
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless3_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless3_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Wireless4_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{		
			alert("The range of pvid value is 1 ~ 4094");						
			form.Wireless4_pvid.focus();
			return false;
		}
	}
	return true;		
}
*/
</script>
</head>

<body onLoad="init();">

<form name="VlanTag_Form" method="post">
<div align="center">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr align="center"> 
        <td height="5" class="light-orange" colspan="7" align="center">&nbsp; </td>
      </tr>
      <tr> 
        <td width="155" height="30" class="title-main"> <%tcWebApi_get("String_Entry","PvidsettingAssignText","s")%> </td>
        <td width="1" class="black">&nbsp;</td>
        <td colspan="4" align="center" class="tabdata"> <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingActiveText","s")%></font>
          <INPUT TYPE="RADIO" NAME="VlanTag_Active" ID="VlanTag_ActiveID" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "Active", "h") = "Yes" then asp_Write("checked") end if%> onclick="init();">
          <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingActivatedText","s")%></font> 
          <INPUT TYPE="RADIO" NAME="VlanTag_Active" ID="VlanTag_ActiveID" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "Active", "h") = "No" then asp_write("checked") end if%> onclick="init();">
          <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingDeactivatedText","s")%></font></td>
      </tr>
	  <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
      </tr>
	  <tr> 
       <td class="light-orange">&nbsp;</td>
        <td width="1" class="light-orange">&nbsp;</td>
        <td colspan="2" align="center" class="tabdata"> <font color="#000000"><B><%tcWebApi_get("String_Entry","PvidsettingPORTText","s")%></B></font>
        </td>
		<td align="center" class="tabdata"> <font color="#000000"><B><%tcWebApi_get("String_Entry","PvidsettingPVIDText","s")%></B></font>
        </td>
		<td align="left" class="tabdata"> <font color="#000000"><B><%tcWebApi_get("String_Entry","PvidsettingTAGText","s")%></B></font>
        </td>
      </tr>
	  <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td width="199" class="tabdata"><div align="right"><font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingATMText","s")%></font> 
            <input type="RADIO" name="PVC1_Active" value="Yes" <% if tcWebApi_get("VlanTag_Entry", "w0sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(2);">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingYesText","s")%></font> 
            <INPUT TYPE="RADIO" NAME="PVC1_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w0sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(3);">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingNoText","s")%></font></div></td>
        <td width="14" class="tabdata"><div align="left">:</div></td>
        <td width="63" class="tabdata" align="center">
          <input name="VlanTag_w0pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w0pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w0pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td width="332" align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w0tag" ID="IDwtag0" <%if TCWebAPI_get("VlanTag_Entry", "w0tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr > 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"><font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingVC1Text","s")%></font>
            <INPUT TYPE="RADIO" NAME="PVC2_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w1sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(6)">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingYesText","s")%></font>
            <INPUT TYPE="RADIO" NAME="PVC2_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w1sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(7)">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingNoText","s")%></font></div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w1pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w1pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w1pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w1tag" ID="IDwtag1" <%if TCWebAPI_get("VlanTag_Entry", "w1tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingVC2Text","s")%>
            <INPUT TYPE="RADIO" NAME="PVC3_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w2sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(10)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC3_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w2sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(11)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w2pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w2pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w2pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w2tag" ID="IDwtag2" <%if TCWebAPI_get("VlanTag_Entry", "w2tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingVC3Text","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC4_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w3sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(14)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC4_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w3sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(15)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w3pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w3pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w3pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w3tag" ID="IDwtag3" <%if TCWebAPI_get("VlanTag_Entry", "w3tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingVC4Text","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC5_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w4sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(18)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC5_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w4sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(19)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w4pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w4pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w4pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w4tag" ID="IDwtag4" <%if TCWebAPI_get("VlanTag_Entry", "w4tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td height="25" class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingVC5Text","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC6_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w5sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(22)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC6_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w5sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(23)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w5pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w5pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w5pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w5tag" ID="IDwtag5" <%if TCWebAPI_get("VlanTag_Entry", "w5tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingVC6Text","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC7_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w6sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(26)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC7_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w6sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(27)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w6pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w6pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w6pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w6tag" ID="IDwtag6" <%if TCWebAPI_get("VlanTag_Entry", "w6tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingVC7Text","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC8_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w7sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(30)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC8_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w7sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(31)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w7pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w7pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w7pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w7tag" ID="IDwtag7" <%if TCWebAPI_get("VlanTag_Entry", "w7tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingEthPort1Text","s")%> 
            <INPUT TYPE="RADIO" NAME="LAN1_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "e0sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(34)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="LAN1_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "e0sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(35)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center"> 
          <input name="VlanTag_e0pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "e0pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "e0pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_e0tag" ID="IDetag0" <%if TCWebAPI_get("VlanTag_Entry", "e0tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingEthPort2Text","s")%>
            <INPUT TYPE="RADIO" NAME="LAN2_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "e1sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(38)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="LAN2_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "e1sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(39)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center"> 
          <input name="VlanTag_e1pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "e1pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "e1pvid", "s") else asp_write("0") end if%>"></td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_e1tag" ID="IDetag1" <%if TCWebAPI_get("VlanTag_Entry", "e1tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingEthPort3Text","s")%> 
            <INPUT TYPE="RADIO" NAME="LAN3_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "e2sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(42)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="LAN3_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "e2sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(43)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center"> 
          <input name="VlanTag_e2pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "e2pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "e2pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_e2tag" ID="IDetag2" <%if TCWebAPI_get("VlanTag_Entry", "e2tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingEthPort4Text","s")%> 
            <INPUT TYPE="RADIO" NAME="LAN4_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "e3sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(46)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="LAN4_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "e3sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(47)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_e3pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "e3pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "e3pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_e3tag" ID="IDetag3" <%if TCWebAPI_get("VlanTag_Entry", "e3tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%if tcwebApi_get("Info_Ether", "isUSBSupported","h")="Yes" then%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingUSBText","s")%>
            <INPUT TYPE="RADIO" NAME="USB0_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "u0sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="init2(50)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="USB0_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "u0sw", "h") = "No" then asp_Write("checked") end if%> onClick="init2(51)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata" align="center"> 
          <input name="VlanTag_u0pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "u0pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "u0pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata"> 
          <input type="checkbox" NAME="VlanTag_u0tag" ID="IDutag0" <%if TCWebAPI_get("VlanTag_Entry", "u0tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%end if%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
      </tr>
      <%if TCWebAPI_get("WLan_Common","BssidNum","h") = "1" then%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingWlanText","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(0)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(1)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_r0pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r0pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r0pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata"> 
          <input type="checkbox" NAME="VlanTag_r0tag" ID="IDrtag0" <%if TCWebAPI_get("VlanTag_Entry", "r0tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <%elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "2" then%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID1Text","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(0)">
            <%tcWebApi_get("String_Entry","PvidsettingSSID1Text","s")%>  
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(1)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_r0pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r0pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r0pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_r0tag" ID="IDrtag0" <%if TCWebAPI_get("VlanTag_Entry", "r0tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%if tcWebApi_get("WebCustom_Entry","isC2TrueSupported","h")<>"Yes" then%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID2Text","s")%>
            <input type="RADIO" name="SSID2_Active" value="Yes" <% if tcWebApi_get("VlanTag_Entry", "r1sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(4)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID2_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r1sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(5)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_r1pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r1pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r1pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_r1tag" ID="IDrtag1" <%if TCWebAPI_get("VlanTag_Entry", "r1tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%end if%>
      <%elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "3" then%>
      <tr> 
        <td height="24" class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID1Text","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(0)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%>
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(1)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_r0pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r0pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r0pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_r0tag" ID="IDrtag0" <%if TCWebAPI_get("VlanTag_Entry", "r0tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%if tcWebApi_get("WebCustom_Entry","isC2TrueSupported","h")<>"Yes" then%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID2Text","s")%>
            <INPUT TYPE="RADIO" NAME="SSID2_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r1sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(4)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%>
            <INPUT TYPE="RADIO" NAME="SSID2_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r1sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(5)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center"> <input name="VlanTag_r1pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r1pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r1pvid", "s") else asp_write("0") end if%>"></td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_r1tag" ID="IDrtag1" <%if TCWebAPI_get("VlanTag_Entry", "r1tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID3Text","s")%>
            <INPUT TYPE="RADIO" NAME="SSID3_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r2sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(8)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <input type="RADIO" name="SSID3_Active" value="No" <% if tcWebApi_get("VlanTag_Entry", "r2sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(9)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_r2pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r2pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r2pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata"> 
          <input type="checkbox" NAME="VlanTag_r2tag" ID="IDrtag2" <%if TCWebAPI_get("VlanTag_Entry", "r2tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%end if%>
      <%elseif TCWebAPI_get("WLan_Common","BssidNum","h") = "4" then%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID1Text","s")%>
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(0)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%>
            <INPUT TYPE="RADIO" NAME="SSID1_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r0sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(1)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center"> <input name="VlanTag_r0pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r0pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r0pvid", "s") else asp_write("0") end if%>"></td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_r0tag" ID="IDrtag0" <%if TCWebAPI_get("VlanTag_Entry", "r0tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%if tcWebApi_get("WebCustom_Entry","isC2TrueSupported","h")<>"Yes" then%>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID2Text","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID2_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r1sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(4)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID2_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r1sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(5)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center"> 
          <input name="VlanTag_r1pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r1pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r1pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_r1tag" ID="IDrtag1" <%if TCWebAPI_get("VlanTag_Entry", "r1tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID3Text","s")%>
            <INPUT TYPE="RADIO" NAME="SSID3_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r2sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(8)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID3_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r2sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(9)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center" > <input name="VlanTag_r2pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r2pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r2pvid", "s") else asp_write("0") end if%>"></td>
        <td align="left" class="tabdata"> 
          <input type="checkbox" NAME="VlanTag_r2tag" ID="IDrtag2" <%if TCWebAPI_get("VlanTag_Entry", "r2tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingSSID4Text","s")%>
            <INPUT TYPE="RADIO" NAME="SSID4_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "r3sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wlan_init(12)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="SSID4_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "r3sw", "h") = "No" then asp_Write("checked") end if%> onClick="wlan_init(13)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center"> 
          <input name="VlanTag_r3pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "r3pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "r3pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata"> 
          <input type="checkbox" NAME="VlanTag_r3tag" ID="IDrtag3" <%if TCWebAPI_get("VlanTag_Entry", "r3tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <%end if%>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	  <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td width="199" class="tabdata"><div align="right"><font color="#000000">
		<%tcWebApi_get("String_Entry","PvidsettingPtm0Text","s")%></font> 
            <input type="RADIO" name="PVC9_Active" value="Yes" <% if tcWebApi_get("VlanTag_Entry", "w8sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="ptm_init(0);">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingYesText","s")%></font> 
            <INPUT TYPE="RADIO" NAME="PVC9_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w8sw", "h") = "No" then asp_Write("checked") end if%> onClick="ptm_init(0);">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingNoText","s")%></font></div></td>
        <td width="14" class="tabdata"><div align="left">:</div></td>
        <td width="63" class="tabdata" align="center">
          <input name="VlanTag_w8pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w8pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w8pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td width="332" align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w8tag" ID="IDwtag8" <%if TCWebAPI_get("VlanTag_Entry", "w8tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> <%tcWebApi_get("String_Entry","PvidsettingPtm1Text","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC10_Active" VALUE="Yes" <% if tcWebApi_get("VlanTag_Entry", "w9sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="ptm_init(1)">
            <%tcWebApi_get("String_Entry","PvidsettingYesText","s")%> 
            <INPUT TYPE="RADIO" NAME="PVC10_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w9sw", "h") = "No" then asp_Write("checked") end if%> onClick="ptm_init(1)">
            <%tcWebApi_get("String_Entry","PvidsettingNoText","s")%> </div></td>
        <td class="tabdata"><div align="left">:</div></td>
        <td class="tabdata" align="center">
          <input name="VlanTag_w9pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w9pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w9pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w9tag" ID="IDwtag9" <%if TCWebAPI_get("VlanTag_Entry", "w9tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	  <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
        <td class="tabdata"><hr noshade class="light-orange-line"></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td width="199" class="tabdata"><div align="right"><font color="#000000"> 
            <%tcWebApi_get("String_Entry","PvidsettingWan0Text","s")%></font> 
            <input type="RADIO" name="PVC11_Active" value="Yes" <% if tcWebApi_get("VlanTag_Entry", "w10sw", "h") = "Yes" then asp_Write("checked") end if%> onClick="wan0_init();">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingYesText","s")%></font> 
            <INPUT TYPE="RADIO" NAME="PVC11_Active" VALUE="No" <% if tcWebApi_get("VlanTag_Entry", "w10sw", "h") = "No" then asp_Write("checked") end if%> onClick="wan0_init();">
            <font color="#000000"><%tcWebApi_get("String_Entry","PvidsettingNoText","s")%></font></div></td>
        <td width="14" class="tabdata"><div align="left">:</div></td>
        <td width="63" class="tabdata" align="center">
          <input name="VlanTag_w10pvid" size="5" value="<%if TCWebAPI_get("VlanTag_Entry", "w10pvid", "h") <> "N/A" then TCWebAPI_get("VlanTag_Entry", "w10pvid", "s") else asp_write("0") end if%>"> 
        </td>
        <td width="332" align="left" class="tabdata">
          <input type="checkbox" NAME="VlanTag_w10tag" ID="IDwtag10" <%if TCWebAPI_get("VlanTag_Entry", "w10tag", "h")="Yes" then asp_write("checked") end if%>> 
        </td>
      </tr>
<%end if%>

      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
        <td>&nbsp;</td>
      </tr>
    </table>

<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="165" height="42" class="black">&nbsp;
	<input type="HIDDEN" name="UserMode" value="<% if tcWebApi_get("WebCustom_Entry","isC2TrueSupported","h")<>"Yes" then asp_write("0") else asp_write("1") end if%>"> 
	<INPUT TYPE="HIDDEN" NAME="usbSupport" value="<% if tcwebApi_get("Info_Ether","isUSBSupported","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
	<Input type="hidden" Name="wlanPvid_1" value="<%if tcWebApi_get("VlanTag_Entry", "r1pvid", "h") <> "N/A" then tcWebApi_get("VlanTag_Entry", "r1pvid", "s") else asp_write("N/A") end if %>">
    <input type="hidden" name="wlanPvid_2" value="<%if tcWebApi_get("VlanTag_Entry", "r2pvid", "h") <> "N/A" then tcWebApi_get("VlanTag_Entry", "r2pvid", "s") else asp_write("N/A") end if %>"> 
    <Input type="hidden" Name="wlanPvid_3" value="<%if tcWebApi_get("VlanTag_Entry", "r3pvid", "h") <> "N/A" then tcWebApi_get("VlanTag_Entry", "r3pvid", "s") else asp_write("N/A") end if %>">
	<Input type="hidden" Name="wlanSw_1" value="<%if tcWebApi_get("VlanTag_Entry", "r1sw", "h") <> "N/A" then tcWebApi_get("VlanTag_Entry", "r1sw", "s") else asp_write("N/A") end if %>">
	<Input type="hidden" Name="wlanSw_2" value="<%if tcWebApi_get("VlanTag_Entry", "r2sw", "h") <> "N/A" then tcWebApi_get("VlanTag_Entry", "r2sw", "s") else asp_write("N/A") end if %>">
	<Input type="hidden" Name="wlanSw_3" value="<%if tcWebApi_get("VlanTag_Entry", "r3sw", "h") <> "N/A" then tcWebApi_get("VlanTag_Entry", "r3sw", "s") else asp_write("N/A") end if %>">
	</td>
	<td width="4" class="black">&nbsp;</td>
    <td width="200" class="orange">&nbsp;</td>
	
    <td width="440" class="orange">
    	<input type="button" name="vlan_pvid_apply" value="<%tcWebApi_get("String_Entry","ButtonApplyText","s")%>" onClick="vlantag_save();">
        <input type="reset"  name="vlan_pvid_cancel" value="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>">
		<INPUT type="hidden" NAME="VlanTag_flag" value="0">
		<INPUT TYPE="HIDDEN" NAME="MBSSIDNumber" ID="MBSSIDNumber" VALUE="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">
		<INPUT type="hidden" NAME="VlanTag_portsNum" value="<%TCWebAPI_get("VlanTag_Entry", "portsNum", "s")%>">
		<INPUT TYPE="HIDDEN" NAME="USBNumber" ID="USBNumber" VALUE="<%if tcWebApi_get("Info_Ether","isUSBSupported","h")="Yes" then asp_write("1") else asp_write("0") end if%>">

    </td>
  </tr>
</table>
</div>
</form>
</body>
</html>        
