<%

IF Request_Form("LineFlag")="1" Then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
End if


If request_Form("VoIP_advanced_flag")="1" then
	if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
	if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then
		if tcWebApi_get("WebCustom_Entry", "voip_interface", "h") = "8" then
			tcWebAPI_set("VoIPAdvanced_Common", "pvcId_serv", "voipPVC_serv")
		elseif tcWebApi_get("WebCustom_Entry", "voip_interface", "h") = "9" then
			tcWebAPI_set("VoIPAdvanced_Common", "pvcId_serv", "voipPVC_serv")
		elseif tcWebApi_get("WebCustom_Entry", "voip_interface", "h") = "10" then
			tcWebAPI_set("VoIPAdvanced_Common", "pvcId_serv", "voipPVC_serv")
		end if
	end if
	end if

	tcWebApi_set("VoIPAdvanced_Common","pvcId","voipPVC")
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
	tcWebApi_set("VoIPAdvanced_Entry","VoiceVolumeListen","VoIPVoiceVolumeListen")
	tcWebApi_set("VoIPAdvanced_Entry","VoiceVolumeSpeak","VoIPVoiceVolumeSpeak")
	tcWebApi_set("VoIPAdvanced_Common","T38Enable","T38Flag")	
	tcWebApi_set("VoIPAdvanced_Common","FaxCodec","VoIPFaxCodec")
	tcWebApi_set("VoIPAdvanced_Common","DTMFTransportMode","VoIPDTMFTransportMode")
	tcWebApi_set("VoIPAdvanced_Common","VoIPRegion","Region")
	tcWebApi_set("VoIPAdvanced_Common","RegistrationExpire","VoIPRegistrationExpire")
	tcWebApi_commit("VoIPAdvanced_Entry")
	tcWebApi_commit("VoIPAdvanced_Common")
end if

if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then
if request_Form("VoIP_advanced_flag")="2" then
	TCWebAPI_set("WebCustom_Entry","voip_interface","voipPVC")
end if
end if
end if

%>


<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" src="/wanfunc.js"></script>
<script language="JavaScript" src="/chksel.js"></script>
<script language="JavaScript">
function check_volume(vol){
	if(valDoValidateIntegerGain(vol) == false){
		return false;	
	}
	if((Number(vol) < -6 ) || (Number(vol) > 6 )){
		alert('<%tcWebApi_get("String_Entry","VoIPJS0Text","s")%>' + vol);
		return false;
	}
	return true;
}


function doLineChange(){
	document.VOIP_ADVANCED_form.LineFlag.value = 1;
	document.VOIP_ADVANCED_form.submit();
}


function doT38Change(){
var form=document.VOIP_ADVANCED_form;
	switch(form.VoIPFaxCodec.selectedIndex) {
		case 0:
			form.T38Flag.value = 1;
			break;
		case 1:
			form.T38Flag.value = 0;
			break;
		case 2:
		form.T38Flag.value = 0;
			break;
		}
 return;
}

function onLoadFun(){
	var form=document.VOIP_ADVANCED_form;

<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") = "No" then%>
		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") = "No" then%>
		
	
	form.VoIPVoiceVolumeListen.disabled=true;
	form.VoIPVoiceVolumeSpeak.disabled=true;
	form.Line.disabled=true;

	
<%end if%>
<%end if%>
}

function do_check(){
	var form=document.VOIP_ADVANCED_form;
	var volumeTx = form.VoIPVoiceVolumeListen.value;
	var volumeRx = form.VoIPVoiceVolumeSpeak.value;	
	var registraExpire = form.VoIPRegistrationExpire.value;
	
	if(volumeTx.length <= 0){
		alert("<%tcWebApi_get("String_Entry","VoIPJS1Text","s")%>");
		return false;
	}		
	if(check_volume(volumeTx) == false){
		return false;
	}
	
	if(volumeRx.length <= 0){
		alert("<%tcWebApi_get("String_Entry","VoIPJS2Text","s")%>");
		return false;
	}		
	if(check_volume(volumeRx) == false){
		return false;
	}
	
	if(registraExpire.length <= 0){
		alert("<%tcWebApi_get("String_Entry","VoIPJS3Text","s")%>");
		return false;
	}
	
	if(valDoValidateIntegerGain(registraExpire) == false){
		return false;	
	}
	if((Number(registraExpire) < 0 ) || (Number(registraExpire) > 65535 )){
		alert('<%tcWebApi_get("String_Entry","VoIPJS4Text","s")%>' + registraExpire + '<%tcWebApi_get("String_Entry","VoIPJS5Text","s")%>');
		return false;
	}
	
	return true;
}

function dosave(){
	if(do_check()== false){
		return;	
	}
	document.VOIP_ADVANCED_form.VoIP_advanced_flag.value=1;
	document.VOIP_ADVANCED_form.submit();
}

function doVCChange(){
	document.VOIP_ADVANCED_form.VoIP_advanced_flag.value=2;
	document.VOIP_ADVANCED_form.submit();
}

</script>

</head><body onLoad="onLoadFun()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_advanced.asp" name="VOIP_ADVANCED_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","VoIPAdvancedText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>
	<INPUT TYPE="HIDDEN" NAME="VoIP_advanced_flag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="LineFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="T38Flag" VALUE="1">


<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<%if TCWebAPI_get("VoIPAdvanced_Common", "SIPBinding", "h") = "Wan" then%>
			<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPPVCBindingText","s")%></font></td>
			<td class="tabdata" align=center>:</td>
			<td  class="tabdata">
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>			
			<SELECT NAME="voipPVC" SIZE="1" onchange="doVCChange();">
<%else%>			
			<SELECT NAME="voipPVC" SIZE="1">
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
					<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "0" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC0Text","s")%>
					<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "1" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC1Text","s")%>
					<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "2" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC2Text","s")%>
					<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "3" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC3Text","s")%>
					<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "4" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC4Text","s")%>
					<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "5" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC5Text","s")%>
					<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "6" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC6Text","s")%>
					<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "7" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC7Text","s")%>
<%else%>

<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>

	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
					<OPTION VALUE="0" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "0" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC0Text","s")%>
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "1" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC1Text","s")%>
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "2" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC2Text","s")%>
					<OPTION VALUE="3" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "3" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC3Text","s")%>
					<OPTION VALUE="4" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "4" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC4Text","s")%>
					<OPTION VALUE="5" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "5" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC5Text","s")%>
					<OPTION VALUE="6" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "6" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC6Text","s")%>
					<OPTION VALUE="7" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "7" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC7Text","s")%>
	<%end if%>
	
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
					<OPTION VALUE="8" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "8" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC8Text","s")%>
					<OPTION VALUE="9" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "9" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC9Text","s")%>
	<%end if%>

	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
					<OPTION VALUE="10" <%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "10" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC10Text","s")%>
	<%end if%>

<%else%>

	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
					<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "0" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC0Text","s")%>
					<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "1" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC1Text","s")%>
					<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "2" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC2Text","s")%>
					<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "3" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC3Text","s")%>
					<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "4" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC4Text","s")%>
					<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "5" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC5Text","s")%>
					<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "6" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC6Text","s")%>
					<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "7" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC7Text","s")%>
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
					<OPTION VALUE="8" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "8" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC8Text","s")%>
					<OPTION VALUE="9" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "9" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC9Text","s")%>
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
					<OPTION VALUE="10" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId", "h") = "10" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC10Text","s")%>
	<%end if%>

<%end if%>
<%end if%>	
				</SELECT>
			</td>
		<%end if%>
	</tr>
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>	
	<%if TCWebAPI_get("VoIPAdvanced_Common", "SIPBinding", "h") = "Wan" then%>
	<%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "8" then %>
	<tr>
        <td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPServiceText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
		<SELECT NAME="voipPVC_serv" SIZE="1">
		<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "0" then  asp_Write("selected") end if %>>0
		<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "1" then  asp_Write("selected") end if %>>1
		<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "2" then  asp_Write("selected") end if %>>2
		<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "3" then  asp_Write("selected") end if %>>3
		<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "4" then  asp_Write("selected") end if %>>4
		<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "5" then  asp_Write("selected") end if %>>5
		<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "6" then  asp_Write("selected") end if %>>6
		<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "7" then  asp_Write("selected") end if %>>7
		</SELECT>
		</td>
	<%end if%>
	<%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "9" then %>
	<tr>
        <td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPServiceText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
		<SELECT NAME="voipPVC_serv" SIZE="1">
		<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "0" then  asp_Write("selected") end if %>>0
		<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "1" then  asp_Write("selected") end if %>>1
		<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "2" then  asp_Write("selected") end if %>>2
		<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "3" then  asp_Write("selected") end if %>>3
		<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "4" then  asp_Write("selected") end if %>>4
		<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "5" then  asp_Write("selected") end if %>>5
		<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "6" then  asp_Write("selected") end if %>>6
		<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "7" then  asp_Write("selected") end if %>>7
		</SELECT>
		</td>
	<%end if%>
	<%if TCWebAPI_get("WebCustom_Entry", "voip_interface", "h") = "10" then %>
	<tr>
        <td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPServiceText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
		<SELECT NAME="voipPVC_serv" SIZE="1">
		<OPTION VALUE="0" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "0" then  asp_Write("selected") end if %>>0
		<OPTION VALUE="1" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "1" then  asp_Write("selected") end if %>>1
		<OPTION VALUE="2" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "2" then  asp_Write("selected") end if %>>2
		<OPTION VALUE="3" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "3" then  asp_Write("selected") end if %>>3
		<OPTION VALUE="4" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "4" then  asp_Write("selected") end if %>>4
		<OPTION VALUE="5" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "5" then  asp_Write("selected") end if %>>5
		<OPTION VALUE="6" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "6" then  asp_Write("selected") end if %>>6
		<OPTION VALUE="7" <%if TCWebAPI_get("VoIPAdvanced_Common", "pvcId_serv", "h") = "7" then  asp_Write("selected") end if %>>7
		</SELECT>
		</td>
	<%end if%>
	<%end if%>
		</tr>
<%end if%>						
	<tr> 
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPRegionText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="Region" SIZE="1">
				<OPTION value="BEL-Belgium" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "BEL-Belgium" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion0Text","s")%>
				<OPTION value="BRA-Brazil" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "BRA-Brazil" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion1Text","s")%>
				<OPTION value="CHL-Chile" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "CHL-Chile" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion2Text","s")%>
				<OPTION value="CHN-China" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "CHN-China" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion3Text","s")%>
				<OPTION value="DNK-Denmark" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "DNK-Denmark" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion4Text","s")%>
				<OPTION value="ESP-Spain" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "ESP-Spain" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion5Text","s")%>
				<OPTION value="ETS-ETSI" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "ETS-ETSI" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion6Text","s")%>
				<OPTION value="FIN-Finland" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "FIN-Finland" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion7Text","s")%>
				<OPTION value="FRA-France" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "FRA-France" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion8Text","s")%>
				<OPTION value="DEU-Germany" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "DEU-Germany" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion9Text","s")%>
				<OPTION value="HUN-Hungary" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "HUN-Hungary" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion10Text","s")%>
				<OPTION value="ITA-Italy" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "ITA-Italy" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion11Text","s")%>
				<OPTION value="JPN-Japan" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "JPN-Japan" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion12Text","s")%>
				<OPTION value="NLD-Netherlands" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "NLD-Netherlands" then  asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion13Text","s")%>
				<OPTION value="USA-United States" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "USA-United States" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion14Text","s")%>
				<OPTION value="SWE-Sweden" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "SWE-Sweden" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion15Text","s")%>
				<OPTION value="CHE-Switzerland" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "CHE-Switzerland" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion16Text","s")%>
				<OPTION value="GBR-Great Britain" <%if TCWebAPI_get("VoIPAdvanced_Common", "VoIPRegion", "h") = "GBR-Great Britain" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPRegion17Text","s")%>
			</SELECT>
		</td></tr>
<td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right><img src="/exclamation.gif"></div></td><td>&nbsp;</td><td class="tabdata">
<font color="#000000"> <%tcWebApi_get("String_Entry","VoIPCommentText","s")%></font></td></tr><tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPDTMFTransportText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="VoIPDTMFTransportMode" SIZE="1">
				<OPTION value="Inband" <%if TCWebAPI_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "Inband" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPDTMFTransport0Text","s")%>
				<OPTION value="RFC2833" <%if TCWebAPI_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "RFC2833" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPDTMFTransport1Text","s")%>
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>			
				<OPTION value="Via SIP" <%if TCWebAPI_get("VoIPAdvanced_Common", "DTMFTransportMode", "h") = "Via SIP" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPDTMFTransport2Text","s")%>
<% end if %>
			</SELECT>
		</td></tr>

<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPT38Text","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPT38Enable" VALUE="Yes" <%if TCWebAPI_get("VoIPAdvanced_Common", "T38Enable", "h") = "Yes" then asp_write("checked") end if %> onclick="doT38Change()" ><font color="#000000"><%tcWebApi_get("String_Entry","VoIPT380Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPT38Enable" VALUE="No" <%if TCWebAPI_get("VoIPAdvanced_Common", "T38Enable", "h") <> "Yes" then asp_write("checked") end if%> onclick="doT38Change()" ><font color="#000000"><%tcWebApi_get("String_Entry","VoIPT381Text","s")%></font>
		</td></tr>
<% end if %>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPFAXText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td  class="tabdata">
			<SELECT NAME="VoIPFaxCodec" SIZE="1" onChange="doT38Change()">

				<OPTION value="T38" <%if TCWebAPI_get("VoIPAdvanced_Common", "FaxCodec", "h") = "T38" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPFAX0Text","s")%>
				<OPTION value="G.711 A-law" <%if TCWebAPI_get("VoIPAdvanced_Common", "FaxCodec", "h") = "G.711 A-law" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPFAX1Text","s")%>
				<OPTION value="G.711 U-law" <%if TCWebAPI_get("VoIPAdvanced_Common", "FaxCodec", "h") = "G.711 U-law" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","VoIPFAX2Text","s")%>
				
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange" style="height: 23px"></td>
		<td class="light-orange" style="height: 23px"></td>
		<td class="tabdata" align=right style="height: 23px"><font color="#000000"><%tcWebApi_get("String_Entry","VoIPRegisterExpireText","s")%></font></td>
		<td class="tabdata" align=center style="height: 23px">:</td>
		<td class="tabdata" style="height: 23px">
			<INPUT TYPE="TEXT" NAME="VoIPRegistrationExpire" SIZE="10" MAXLENGTH="10" VALUE="<%if TCWebAPI_get("VoIPAdvanced_Common", "RegistrationExpire", "h") <> "N/A" then TCWebAPI_get("VoIPAdvanced_Common", "RegistrationExpire", "s") else asp_Write("0") end if %>">
		<%tcWebApi_get("String_Entry","VoIPsecText","s")%></td></tr>
	
	<tr>
		<td class="title-sub"><font color="#000000">&nbsp;</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPLineText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<select NAME="Line" SIZE="1" onChange="doLineChange()" >
		<OPTION value="0" <%if TCWebApi_get("WebCurSet_Entry","LineNum","h") = "0" then asp_write("selected") end if%> <% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%> disabled=true <%end if%>>1
<% if TCWebAPI_get("VoIPBasic_Common", "VoIPLine2Enable", "h") = "Yes" then%>
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","LineNum","h") = "1" then asp_write("selected") end if%> <% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%> disabled=true <%end if%>>2
<%end if%>



	</select>
		</td>
	</tr>


		<tr>
		<td class="title-sub"><font color="#000000">&nbsp;</font></td>
		<td class="light-orange">&nbsp;</td></tr>


	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPVolumeText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPVoiceVolumeListen" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeListen", "h") <> "N/A" then TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeListen", "s") else asp_Write("0") end if %>">
		<font color="#000000"><%tcWebApi_get("String_Entry","VoIPVolumedbText","s")%></font>
		</td>
		</tr>			
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPSpeakingVolumeText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPVoiceVolumeSpeak" SIZE="5" MAXLENGTH="5" VALUE="<%if TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeSpeak", "h") <> "N/A" then TCWebAPI_get("VoIPAdvanced_Entry", "VoiceVolumeSpeak", "s") else asp_Write("0") end if %>">
			<font color="#000000"><%tcWebApi_get("String_Entry","VoIPVolumedbText","s")%></font>
			</td>
		</tr>
	

	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="dosave();"></td></tr></table>
</form></body></html>
