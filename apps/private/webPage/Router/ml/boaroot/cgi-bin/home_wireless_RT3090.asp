<%
If Request_Form("wlanWEPFlag")="1" Then
	'tcWebApi_set("WebCurSet_Entry","wlan_id","wlan_VC")
	tcWebApi_set("WebCurSet_Entry","wlan_id","SSID_INDEX")
	tcWebApi_set("WLan_Common","BssidNum","BssidNum")
	TCWebApi_set("WLan_Common","Country","Countries_Channels")
	TCWebApi_set("WLan_Common","APOn","wlan_APenable")
	TCWebApi_set("WLan_Common","Channel","Channel_ID")
	TCWebApi_set("WLan_Common","BeaconPeriod","BeaconInterval")
  TCWebApi_set("WLan_Common","RTSThreshold","RTSThreshold")
	TCWebApi_set("WLan_Common","FragThreshold","FragmentThreshold")
	TCWebApi_set("WLan_Common","DtimPeriod","DTIM")
	TCWebApi_set("WLan_Common","WirelessMode","WirelessMode")
	If Request_Form("Is11nMode") = "1" then
		TCWebApi_set("WLan_Common","HT_BW","WLANChannelBandwidth")
		'TCWebApi_set("WLan_Common","HT_EXTCHA","WLANExtensionChannel")
		TCWebApi_set("WLan_Common","HT_EXTCHA","ExtChannFlag")
		TCWebApi_set("WLan_Common","HT_GI","WLANGuardInterval")
		TCWebApi_set("WLan_Entry","HT_MCS","WLANMCS")
	else
		TCWebApi_set("WLan_Common","HT_BW","HTBW")
		TCWebApi_set("WLan_Entry","HT_MCS","HTMCSAUTO")
		TCWebApi_set("WLan_Entry","WMM","WMM_Selection")
	end IF
	TCWebApi_set("WLan_Common","11nMode","Is11nMode")
	If Request_Form("WirelessMode") = "0" then
		TCWebApi_set("WLan_Common","BasicRate","BasicRate_Value1")
	elseif Request_Form("WirelessMode") = "1" then
		TCWebApi_set("WLan_Common","BasicRate","BasicRate_Value2")
	else
		TCWebApi_set("WLan_Common","BasicRate","BasicRate_Value3")
	end if
	TCWebApi_set("WLan_Entry","SSID","ESSID")
	TCWebApi_set("WLan_Common","BeaconPeriod","BeaconInterval")
	TCWebApi_set("WLan_Entry","HideSSID","ESSID_HIDE_Selection")
	'TCWebApi_set("WLan_Entry","WMM","WMM_Selection")
	if Request_form("WLAN_FltActive") = "0" then
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltActive")
	else
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltAction")
	end if
	TCWebApi_set("WLan_Entry","WLan_MAC0","WLANFLT_MAC1")	
	TCWebApi_set("WLan_Entry","WLan_MAC1","WLANFLT_MAC2")	
	TCWebApi_set("WLan_Entry","WLan_MAC2","WLANFLT_MAC3")		
	TCWebApi_set("WLan_Entry","WLan_MAC3","WLANFLT_MAC4")		
	TCWebApi_set("WLan_Entry","WLan_MAC4","WLANFLT_MAC5")	
	TCWebApi_set("WLan_Entry","WLan_MAC5","WLANFLT_MAC6")	
	TCWebApi_set("WLan_Entry","WLan_MAC6","WLANFLT_MAC7")		
	TCWebApi_set("WLan_Entry","WLan_MAC7","WLANFLT_MAC8")

	TCWebApi_set("WLan_Entry","AuthMode","WEP_Selection")
end if

If Request_Form("CountryChange")="1" Then
	'tcWebApi_set("WebCurSet_Entry","wlan_id","wlan_VC")
	tcWebApi_set("WebCurSet_Entry","wlan_id","SSID_INDEX")
	TCWebApi_set("WLan_Common","Country","Countries_Channels")
	if Request_Form("Countries_Channels") = "CANADA" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "COLOMBIA" then
   		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")	
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "DOMINICAN REPUBLIC" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "GUATEMALA" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "MEXICO" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "NORWAY" then 
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "PANAMA" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "PUERTO RICO" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "UNITED STATES" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "TAIWAN" then
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	elseif Request_Form("Countries_Channels") = "UZBEKISTAN" then			             
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion0")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion0")
	else
		'TCWebApi_set("WLan_Entry","CountryRegion","CountryRegion1")
		TCWebApi_set("WLan_Common","CountryRegion","CountryRegion1")
	end if
End If
IF Request_Form("wlanWEPFlag")="2" Then
	tcWebApi_set("WebCurSet_Entry","wlan_id","SSID_INDEX")	
End if
If Request_Form("wlanWEPFlag")="3" Then
	'tcWebApi_set("WebCurSet_Entry","wlan_id","wlan_VC")
	tcWebApi_set("WebCurSet_Entry","wlan_id","SSID_INDEX")
	tcWebApi_set("WLan_Common","BssidNum","BssidNum")
	TCWebApi_set("WLan_Common","Country","Countries_Channels")
	TCWebApi_set("WLan_Common","Channel","Channel_ID")
	TCWebApi_set("WLan_Common","APOn","wlan_APenable")
	TCWebApi_set("WLan_Common","BeaconPeriod","BeaconInterval")
  TCWebApi_set("WLan_Common","RTSThreshold","RTSThreshold")
	'TCWebApi_set("WLan_Entry","FragThreshold","FragmentThreshold")
	TCWebApi_set("WLan_Common","FragThreshold","FragmentThreshold")
	TCWebApi_set("WLan_Common","DtimPeriod","DTIM")
	TCWebApi_set("WLan_Common","WirelessMode","WirelessMode")
	If Request_Form("Is11nMode") = "1" then
		TCWebApi_set("WLan_Common","HT_BW","WLANChannelBandwidth")
		'TCWebApi_set("WLan_Common","HT_EXTCHA","WLANExtensionChannel")
		TCWebApi_set("WLan_Common","HT_EXTCHA","ExtChannFlag")
		TCWebApi_set("WLan_Common","HT_GI","WLANGuardInterval")
		TCWebApi_set("WLan_Entry","HT_MCS","WLANMCS")
	else
		TCWebApi_set("WLan_Common","HT_BW","HTBW")
		TCWebApi_set("WLan_Entry","HT_MCS","HTMCSAUTO")
		TCWebApi_set("WLan_Entry","WMM","WMM_Selection")
	end IF
	TCWebApi_set("WLan_Common","11nMode","Is11nMode")	
	TCWebApi_set("WLan_Entry","SSID","ESSID")
	TCWebApi_set("WLan_Common","BeaconPeriod","BeaconInterval")	
	TCWebApi_set("WLan_Entry","HideSSID","ESSID_HIDE_Selection")
	'TCWebApi_set("WLan_Entry","WMM","WMM_Selection")
	if Request_form("WLAN_FltActive") = "0" then
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltActive")
	else
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltAction")
	end if	
	TCWebApi_set("WLan_Entry","WLan_MAC0","WLANFLT_MAC1")	
	TCWebApi_set("WLan_Entry","WLan_MAC1","WLANFLT_MAC2")	
	TCWebApi_set("WLan_Entry","WLan_MAC2","WLANFLT_MAC3")		
	TCWebApi_set("WLan_Entry","WLan_MAC3","WLANFLT_MAC4")		
	TCWebApi_set("WLan_Entry","WLan_MAC4","WLANFLT_MAC5")	
	TCWebApi_set("WLan_Entry","WLan_MAC5","WLANFLT_MAC6")	
	TCWebApi_set("WLan_Entry","WLan_MAC6","WLANFLT_MAC7")		
	TCWebApi_set("WLan_Entry","WLan_MAC7","WLANFLT_MAC8")	
	TCWebApi_set("WLan_Entry","AuthMode","WEP_Selection")
	if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-64Bits"  then
		TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey3")
		TCWebApi_set("WLan_Entry","Key1Str","WEP_Key13")	
		TCWebApi_set("WLan_Entry","Key2Str","WEP_Key23")		
		TCWebApi_set("WLan_Entry","Key3Str","WEP_Key33")		
		TCWebApi_set("WLan_Entry","Key4Str","WEP_Key43")
	elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-128Bits" then
		TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey4")
		TCWebApi_set("WLan_Entry","Key1Str","WEP_Key14")	
		TCWebApi_set("WLan_Entry","Key2Str","WEP_Key24")		
		TCWebApi_set("WLan_Entry","Key3Str","WEP_Key34")		
		TCWebApi_set("WLan_Entry","Key4Str","WEP_Key44")
	elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSK" then
		TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey2")
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection5")
	elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2PSK" then
		TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey1")
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection4")
	elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSKWPA2PSK"  then
		TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey3")
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection6")
	end if
	'TCWebApi_set("WLan_Entry","WPSConfStatus","WPSConfigured")			
	tcWebApi_commit("WLan_Entry")	
End If
%>
 
<html>     
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language="JavaScript">
/*
var wpsenable;
var wepidx;
function doStartWPS(){
	if(document.WLAN.isInWPSing.value==0){//xyyou???
		alert("Please Start WPS peer within 2 minutes.");
	}
	document.WLAN.WpsStart.value = 1;
	document.WLAN.submit();
}

function doResetOOB(){
	document.WLAN.WpsOOB.value = 1;
	document.WLAN.submit();
}
*/

function strESSIDCheck(str) {
if(str.value.match(/[^\x00-\xff]/g)){
	alert("<%tcWebApi_get("String_Entry","WirelessJS1Text","s")%>");
	return true;
}
if(document.WLAN.ESSID.value.length <= 0){
	alert("<%tcWebApi_get("String_Entry","WirelessJS2Text","s")%>");
	return true;
}
return false;
}
function doRegionCheck(){
	var vCountryName = document.WLAN.Countries_Channels.value;
	var ctlCountryRegion = document.WLAN.hCountryRegion;
	var ctlCountryRegion0 = document.WLAN.CountryRegion0;
	var ctlCountryRegion1 = document.WLAN.CountryRegion1;
	if(vCountryName == "CANADA")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "COLOMBIA")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "DOMINICAN REPUBLIC")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "GUATEMALA")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "MEXICO")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "NORWAY")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "PANAMA")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "PUERTO RICO")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "UNITED STATES")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "TAIWAN")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "UZBEKISTAN")			             
		ctlCountryRegion.value = ctlCountryRegion0.value;		
	else
		ctlCountryRegion.value = ctlCountryRegion1.value;

	RefreshPage();
	
	document.WLAN.CountryChange.value = 1; 
	//document.WLAN.submit();
}

/*
function dowpscheck(){
	var wlan=document.WLAN;
if(wlan.SSID_INDEX.value==0){	
	if((wlan.WEP_Selection.selectedIndex == 1 || wlan.WEP_Selection.selectedIndex == 2)&&(wlan.UseWPS_Selection[0].checked == true)){
		alert("We should not use WEP when WPS function turned on!");
		if(wpsenable){
			wlan.UseWPS_Selection[0].checked = true;
		}else{
			wlan.UseWPS_Selection[1].checked = true;
		}
		wlan.WEP_Selection.selectedIndex = wepidx;
		return 0;
	}else{
		return 1;
	}
}else{
	return 1;
}
}
*/
function doWEPChange(){
//	if(dowpscheck()){
		document.WLAN.wlanWEPFlag.value = 1;
		//document.WLAN.submit();
		doLoad();
//	}
}
function doSSIDChange(){
	//alert(document.WLAN.SSID_INDEX.value);
	document.WLAN.wlanWEPFlag.value = 2;
	document.WLAN.submit();
}

function doWirelessModeChange(){
	document.WLAN.wlanWEPFlag.value = 1;
	if(document.WLAN.WirelessMode.selectedIndex>=3){
		document.WLAN.Is11nMode.value=1;
	}else{
		document.WLAN.Is11nMode.value=0;
	}
	//document.WLAN.submit();
	doLoad();
}

function doChannelBandwidthChange(){
	document.WLAN.wlanWEPFlag.value = 1;
	//if(document.WLAN.WLANExtensionChannel.selectedIndex==0){
	//	document.WLAN.ExtChannFlag.value = 0;
	//}else{
	//	document.WLAN.ExtChannFlag.value = 1;
	//}
	//document.WLAN.submit();
	doLoad();
}

function doExtChannChange(){
	if(document.WLAN.WLANExtensionChannel.selectedIndex==0){
		document.WLAN.ExtChannFlag.value = 0;
	}else{
		document.WLAN.ExtChannFlag.value = 1;
	}
}

function doExtChaLockChange() {
	if(document.WLAN.WirelessMode.selectedIndex >= 3){
		if(document.WLAN.WLANChannelBandwidth.selectedIndex == 1){
			document.WLAN.wlanWEPFlag.value = 1;
			//document.WLAN.submit();
			doLoad();
		}
	}
}
/*
function doWPSUseChange(){
	if(dowpscheck()){
		document.WLAN.wlanWEPFlag.value = 1;
		document.WLAN.submit();
	}
}

function doWPSModeChange(){
	document.WLAN.wlanWEPFlag.value = 1;
	document.WLAN.submit();
}
*/
function wpapskCheck(object) {
	var keyvalue=object.value;
	var wpapsklen=object.value.length;
	
   	if(wpapsklen >= 8 && wpapsklen < 64) {
    	if(keyvalue.match(/[^\x00-\xff]/g))
   	    {
			alert("<%tcWebApi_get("String_Entry","WirelessJS25Text","s")%>");
			return true;
   	    } else {
   	        for ( i = 0; i < wpapsklen; i++)
   	        if (keyvalue.charAt(i) == ','){
			   alert("<%tcWebApi_get("String_Entry","WirelessJS25Text","s")%>");
			   return true;
   	        }
   	    }
	}else if(wpapsklen==64){
		for(i=0;i<64;i++){
			var c=keyvalue.charAt(i);
			if(doHexCheck(c)<0){
				alert('<%tcWebApi_get("String_Entry","WirelessJS26Text","s")%>');
				return true;
			}
		}
	}else {
    	alert('<%tcWebApi_get("String_Entry","WirelessJS27Text","s")%>');
		return true;
	}			
	return false;
}

function doHexCheck(c)
{
  if ((c >= "0")&&(c <= "9"))
  {
    return 1;
  }
  else if ((c >= "A")&&(c <= "F"))
  {
    return 1;
  }
  else if ((c >= "a")&&(c <= "f"))
  {
    return 1;
  }

  return -1;
}

function doNonSympolCheck(c)
{
	if ((c >= "0")&&(c <= "9"))
	{
		return 1;
	}
	else if ((c >= "A")&&(c <= "Z"))
	{
		return 1;
	}
	else if ((c >= "a")&&(c <= "z"))
	{
		return 1;
	}

  return -1;
}

function doKEYcheck(object)
{
	var index = object.value.indexOf("0x");
	len=object.value.length;
	
	if(len == 0){
		return true;
	}

	if(document.WLAN.WEP_Selection.selectedIndex==1)
	{
	        if(len==5)/*wep 64*/
	        {
		        for(i=0;i<len;i++)
		        {
			var c = object.value.charAt(i);	
			if(doNonSympolCheck(c)==-1)
			{
				alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					return false;
			}
		}
	}
	else if(len==10)/*wep 64*/
	{
		for(i=0;i<len;i++)
		{
			var c = object.value.charAt(i);	
			if(doHexCheck(c)==-1)
			{
				alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					return false;
			}
		}
	}
	//	else if(len==12)/*wep 64*/
	//	{
	//		if(index==0)
	//	{
	//		    for ( i = 2; i < len; i++ )
	//		{
	//			var c = object.value.charAt(i);	
	//			if(doHexCheck(c)==-1)
	//			{
	//				alert("Invalid Key Value");
	//				return;
	//			}
	//		}
	//	}
	//		else
	//		{
	//			alert("Invalid Key Value");
	//			return;
	//		}
	//	}
		else
		{
			alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
			return false;
		}
	}
	else if(document.WLAN.WEP_Selection.selectedIndex==2)
	{
		if(len==13)/*wep 128*/
	{
		for(i=0;i<len;i++)
		{
			var c = object.value.charAt(i);	
			if(doNonSympolCheck(c)==-1)
			{
				alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					return false;
			}
		}
	}
	else if(len==26)/*wep 128*/
	{
		for(i=0;i<len;i++)
		{
			var c = object.value.charAt(i);	
			if(doHexCheck(c)==-1)
			{
				alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					return false;
			}
		}
	}
		//else if(len==28)/*wep 128*/
		//{
		//	if(index==0)
		//	{
		//	    for ( i = 2; i < len; i++ )
		//		{
		//			var c = object.value.charAt(i);	
		//			if(doHexCheck(c)==-1)
		//			{
		//				alert("Invalid Key Value");
		//				return;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		alert("Invalid Key Value");
		//		return;
		//	}
		//}
		else
		{
			alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
			return false;
		}
	}
	return true;
}

function doMACcheck(object)
{
  var szAddr = object.value;
  var len = szAddr.length;
  var errMsg = "Invalid MAC Address";

  if ( len == 0 )
  {
    object.value ="00:00:00:00:00:00";
    return;
  }

  if ( len == 12 )
  {
    var newAddr = "";
    var i = 0;

    for ( i = 0; i < len; i++ )
    {
      var c = szAddr.charAt(i);
      
      if ( doHexCheck(c) < 0 )
      {
      alert("<%tcWebApi_get("String_Entry","WirelessJS10Text","s")%>");        object.focus();
        return;
      }
      if ( (i == 2) || (i == 4) || (i == 6) || (i == 8) || (i == 10) )
        newAddr = newAddr + ":";
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

    if ( (doHexCheck(c0) < 0) || (doHexCheck(c1) < 0) )
    {
       alert("<%tcWebApi_get("String_Entry","WirelessJS10Text","s")%>");       	object.focus();
      return;
    }
    
    i = 2;
    while ( i < len )
    {
      var c0 = szAddr.charAt(i);
      var c1 = szAddr.charAt(i+1);
      var c2 = szAddr.charAt(i+2);  
      if ( (c0 != ":") || (doHexCheck(c1) < 0) || (doHexCheck(c2) < 0) )
      {
         alert("<%tcWebApi_get("String_Entry","WirelessJS10Text","s")%>");         	object.focus();
        return;
      }
      i = i + 3;
    }
    return; 
  }
  else
  {
  alert("<%tcWebApi_get("String_Entry","WirelessJS10Text","s")%>");     	object.focus();
    return;
  }
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

function checkBeacon(value) 
{
  if (!isNumeric(value)) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS11Text","s")%>" + value);    return true;
  }
  if (value < 20 || value > 1000) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS12Text","s")%>");    return true;  	
  }
  return false;
}

function checkRTS(value) 
{
  if (!isNumeric(value)) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS11Text","s")%>" + value);    return true;
  }
  if (value < 1500 || value > 2347) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS13Text","s")%>");    return true;  	
  }
  return false;
}

function checkFrag(value) 
{
  if (!isNumeric(value)) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS11Text","s")%>" + value);    return true;
  }
  if (value < 256 || value > 2346) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS14Text","s")%>");    return true;  	
  }
  if (value % 2) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS15Text","s")%>");    return true;  	
  }
  return false;
}

function checkDTIM(value) 
{
  if (!isNumeric(value)) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS11Text","s")%>" + value);    return true;
  }
  if (value < 1 || value > 255) {
    alert("<%tcWebApi_get("String_Entry","WirelessJS16Text","s")%>");		return true;
	}	
	return false;
}

function quotationCheck(object, limit_len) {
	var len = object.value.length;
	var c;
	var i, j = 0;
    for (i = 0; i < len; i++)
    {
	 	var c = object.value.charAt(i);
      
	  	if (c == '"')
		{
			j += 6;
		}
		else
			j++;
    }
   	if (j > limit_len-1)
	{
    alert('<%tcWebApi_get("String_Entry","WirelessJS17Text","s")%>');		return true;
	}	
	return false;
}

function ValidateChecksum(PIN)
{
	var accum = 0;
	accum += 3 * (((PIN - PIN % 10000000) / 10000000) % 10);
	accum += 1 * (((PIN - PIN % 1000000) / 1000000) % 10);
	accum += 3 * (((PIN - PIN % 100000) / 100000) % 10);
	accum += 1 * (((PIN - PIN % 10000) / 10000) % 10);
	accum += 3 * (((PIN - PIN % 1000) / 1000) % 10);
	accum += 1 * (((PIN - PIN % 100) / 100) % 10);
	accum += 3 * (((PIN - PIN % 10) / 10) % 10);
	accum += 1 * (((PIN - PIN % 1) / 1) % 10);
	if ((accum % 10) == 0)
		return true;
	else
		return false;
}

/*
function doPINCodeCheck(object)
{
	var len= object.value.length;
	var ch;
	if (len > 0)
	{
		if(len < 8)
		{
			alert("WPS PIN code must be 8 digits!");
			return;
		}
		for( i=0; i < len; i++)
		{
			ch= object.value.charAt(i);
			if( ch > '9' || ch < '0')
			{
				alert("WPS PIN code must be 8 digits!");
				return;
			}
		}
		if (ValidateChecksum(Number(object.value)) == false){
			alert("WPS PIN code checksum error!");
		}
		return;
	}
}
*/

function doSave(){
	if(checkBeacon(document.WLAN.BeaconInterval.value) ||
		checkRTS(document.WLAN.RTSThreshold.value) ||
		checkFrag(document.WLAN.FragmentThreshold.value) ||
		checkDTIM(document.WLAN.DTIM.value)
	){
		return false;
	}
		
	if(document.WLAN.WEP_Selection.selectedIndex == 3){
	/*
		if (wpapskCheck()){
			return false;
		}
	*/	
		if(quotationCheck(document.WLAN.PreSharedKey2, 65) ){
			return false;	 
		}  			
	}
	if(document.WLAN.WEP_Selection.selectedIndex == 4){
	/*
		if (wpapskCheck()){
			return false;
		}
	*/	
		if(quotationCheck(document.WLAN.PreSharedKey1, 65) ){
			return false;	 
		}  			
	}
	if(document.WLAN.WEP_Selection.selectedIndex == 1){
		if((!doKEYcheck(document.WLAN.WEP_Key13))||
		(!doKEYcheck(document.WLAN.WEP_Key23))||
		(!doKEYcheck(document.WLAN.WEP_Key33))||
		(!doKEYcheck(document.WLAN.WEP_Key43))){
			//alert("key check fail");
			return false;
		}
	}
	if(document.WLAN.WEP_Selection.selectedIndex == 2){
		if((!doKEYcheck(document.WLAN.WEP_Key14))||
		(!doKEYcheck(document.WLAN.WEP_Key24))||
		(!doKEYcheck(document.WLAN.WEP_Key34))||
		(!doKEYcheck(document.WLAN.WEP_Key44))){
			//alert("key check fail");
			return false;
		}
	}
	if(quotationCheck(document.WLAN.ESSID, 33)||strESSIDCheck(document.WLAN.ESSID)){
		return;	   
	}

/*
	if(!checkSelectedKEY()){
		return false;
	}
*/

	document.WLAN.wlanWEPFlag.value = 3;
	if(document.WLAN.WirelessMode.selectedIndex>=3){
		document.WLAN.Is11nMode.value=1;
	}else{
		document.WLAN.Is11nMode.value=0;
	}
	document.WLAN.submit();
}

function checkSelectedKEY(){
	var group;
	var curCBX;
	var vAuthMode = document.WLAN.WEP_Selection.value;
	if(vAuthMode == "WEP-64Bits"){
		group = document.WLAN.DefWEPKey3;
	}
	else if(vAuthMode == "WEP-128Bits"){
		group = document.WLAN.DefWEPKey4;
	}

	for (var i=0; i<group.length; i++){
		if (group[i].checked)
		break;
	}
	
	if(vAuthMode == "WEP-64Bits"){
		switch (i)
		{
		case 0:
			curCBX = document.WLAN.WEP_Key13;
			break;
		case 1:
			curCBX = document.WLAN.WEP_Key23;
			break;
		case 2:
			curCBX = document.WLAN.WEP_Key33;
			break;
		case 3:
			curCBX = document.WLAN.WEP_Key43;
			break;
		default:
		  ;
		}
	}
	else if(vAuthMode == "WEP-128Bits"){
		switch (i)
		{
		case 0:
			curCBX = document.WLAN.WEP_Key14;
			break;
		case 1:
			curCBX = document.WLAN.WEP_Key24;
			break;
		case 2:
			curCBX = document.WLAN.WEP_Key34;
			break;
		case 3:
			curCBX = document.WLAN.WEP_Key44;
			break;
		default:
		  ;
		}
	}
	
	len=curCBX.value.length;
	index = curCBX.value.indexOf("0x");
	
	if(document.WLAN.WEP_Selection.selectedIndex==1)/*wep 64*/
	{
		if(len==5)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doNonSympolCheck(c)==-1)
				{
					alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==10)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doHexCheck(c)==-1)
				{
					alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==12)
		{
			if(index==0)
			{
				for(i=2;i<len;i++)
				{
					var c = curCBX.value.charAt(i);	
					if(doHexCheck(c)==-1)
					{
						alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
						curCBX.focus();
						return false;
					}
				}
			}
			else
			{
				alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
				curCBX.focus();
				return false;
			}
		}
		else
		{
			alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
			curCBX.focus();
			return false;
		}
	}
	else if(document.WLAN.WEP_Selection.selectedIndex==2)/*wep 128*/
	{
		if(len==13)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doNonSympolCheck(c)==-1)
				{
					alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==26)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doHexCheck(c)==-1)
				{
					alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==28)
		{
			if(index==0)
			{
				for(i=2;i<len;i++)
				{
					var c = curCBX.value.charAt(i);	
					if(doHexCheck(c)==-1)
					{
						alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
						curCBX.focus();
						return false;
					}
				}
			}
			else
			{
				alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
				curCBX.focus();
				return false;
			}
		}
		else
		{
			alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
			curCBX.focus();
			return false;
		}
	}
}

function checkFocus(value){
	if(document.form.WEP_Selection.selectedIndex == 0){
		document.form.WEP_Selection.focus();
	}
}

function doLoad(){
	RefreshPage();	

	if(document.WLAN.WirelessMode.selectedIndex>=3){
		document.WLAN.Is11nMode.value=1;
		if(document.WLAN.WLANChannelBandwidth.selectedIndex == 1){
			if(document.WLAN.Channel_ID.selectedIndex <= 4){
				document.WLAN.WLANExtensionChannel.selectedIndex = 1;
				document.WLAN.WLANExtensionChannel.disabled = true;
				document.WLAN.ExtChannFlag.value = 1;
			}
			else if(document.WLAN.Channel_ID.selectedIndex >= 8){
				document.WLAN.WLANExtensionChannel.selectedIndex = 0;
				document.WLAN.WLANExtensionChannel.disabled = true;
				document.WLAN.ExtChannFlag.value = 0;
			}
			else{
				document.WLAN.WLANExtensionChannel.disabled = false;
				if(document.WLAN.WLANExtensionChannel.selectedIndex==0){
					document.WLAN.ExtChannFlag.value = 0;
				}else{
					document.WLAN.ExtChannFlag.value = 1;
				}
			}
				
		}
	}else{
		document.WLAN.Is11nMode.value=0;
	}
/*
	if(document.WLAN.isInWPSing.value==1){
		document.WLAN.ResetOOB.disabled = true;
		document.WLAN.BUTTON.disabled = true;
		document.WLAN.CancelBtn.disabled = true;
	}
	if(document.WLAN.SSID_INDEX.value==0){
		wpsenable = document.WLAN.UseWPS_Selection[0].checked;
	}
*/
	wepidx = document.WLAN.WEP_Selection.selectedIndex;	
}
var bInit = 1;

function RefreshPage(){
	var autoText = "AUTO";
	var index = 0;
	var ctlChannel_ID = document.WLAN.Channel_ID;
	var vChannel = ctlChannel_ID.value;
	var vCountryRegion = document.WLAN.hCountryRegion.value;
	if(bInit == 1){
		vChannel = "<%tcWebApi_get("WLan_Common","Channel","s")%>";
		bInit = 0;
	}
	ctlChannel_ID.length = 0;
	if(vCountryRegion == 0){
		ctlChannel_ID.options[index++] = new Option(autoText, "0");
		ctlChannel_ID.options[index++] = new Option("01", "1");
		ctlChannel_ID.options[index++] = new Option("02", "2");
		ctlChannel_ID.options[index++] = new Option("03", "3");
		ctlChannel_ID.options[index++] = new Option("04", "4");
		ctlChannel_ID.options[index++] = new Option("05", "5");
		ctlChannel_ID.options[index++] = new Option("06", "6");
		ctlChannel_ID.options[index++] = new Option("07", "7");
		ctlChannel_ID.options[index++] = new Option("08", "8");
		ctlChannel_ID.options[index++] = new Option("09", "9");
		ctlChannel_ID.options[index++] = new Option("10", "10");
		ctlChannel_ID.options[index++] = new Option("11", "11");
	}else if(vCountryRegion == 2){
		ctlChannel_ID.options[index++] = new Option(autoText, "0");
		ctlChannel_ID.options[index++] = new Option("10", "10");
		ctlChannel_ID.options[index++] = new Option("11", "11");
	}else if(vCountryRegion == 3){
		ctlChannel_ID.options[index++] = new Option(autoText, "0");
		ctlChannel_ID.options[index++] = new Option("10", "10");
		ctlChannel_ID.options[index++] = new Option("11", "11");
		ctlChannel_ID.options[index++] = new Option("12", "12");
		ctlChannel_ID.options[index++] = new Option("13", "13");
	}else if(vCountryRegion == 4){
		ctlChannel_ID.options[index++] = new Option(autoText, "0");
		ctlChannel_ID.options[index++] = new Option("04", "4");
	}else if(vCountryRegion == 5){
		ctlChannel_ID.options[index++] = new Option(autoText, "0");
		ctlChannel_ID.options[index++] = new Option("01", "1");
		ctlChannel_ID.options[index++] = new Option("02", "2");
		ctlChannel_ID.options[index++] = new Option("03", "3");
		ctlChannel_ID.options[index++] = new Option("04", "4");
		ctlChannel_ID.options[index++] = new Option("05", "5");
		ctlChannel_ID.options[index++] = new Option("06", "6");
		ctlChannel_ID.options[index++] = new Option("07", "7");
		ctlChannel_ID.options[index++] = new Option("08", "8");
		ctlChannel_ID.options[index++] = new Option("09", "9");
		ctlChannel_ID.options[index++] = new Option("10", "10");
		ctlChannel_ID.options[index++] = new Option("11", "11");
		ctlChannel_ID.options[index++] = new Option("12", "12");
		ctlChannel_ID.options[index++] = new Option("13", "13");
		ctlChannel_ID.options[index++] = new Option("14", "14");
	}else if(vCountryRegion == 6){
		ctlChannel_ID.options[index++] = new Option(autoText, "0");
		ctlChannel_ID.options[index++] = new Option("03", "3");
		ctlChannel_ID.options[index++] = new Option("04", "4");
		ctlChannel_ID.options[index++] = new Option("05", "5");
		ctlChannel_ID.options[index++] = new Option("06", "6");
		ctlChannel_ID.options[index++] = new Option("07", "7");
		ctlChannel_ID.options[index++] = new Option("08", "8");
		ctlChannel_ID.options[index++] = new Option("09", "9");
	}else{
		ctlChannel_ID.options[index++] = new Option(autoText, "0");
		ctlChannel_ID.options[index++] = new Option("01", "1");
		ctlChannel_ID.options[index++] = new Option("02", "2");
		ctlChannel_ID.options[index++] = new Option("03", "3");
		ctlChannel_ID.options[index++] = new Option("04", "4");
		ctlChannel_ID.options[index++] = new Option("05", "5");
		ctlChannel_ID.options[index++] = new Option("06", "6");
		ctlChannel_ID.options[index++] = new Option("07", "7");
		ctlChannel_ID.options[index++] = new Option("08", "8");
		ctlChannel_ID.options[index++] = new Option("09", "9");
		ctlChannel_ID.options[index++] = new Option("10", "10");
		ctlChannel_ID.options[index++] = new Option("11", "11");
		ctlChannel_ID.options[index++] = new Option("12", "12");
		ctlChannel_ID.options[index++] = new Option("13", "13");
	}
	ctlChannel_ID.options[0].selected=true;
	SelectValue(ctlChannel_ID, vChannel);
	
	if(document.WLAN.Is11nMode.value == "1"){
		document.getElementById("11nMode_0_div").style.display="none";
		document.getElementById("11nMode_1_div").style.display="";
	}else{
		document.getElementById("11nMode_0_div").style.display="";
		document.getElementById("11nMode_1_div").style.display="none";
	}
	
	if(document.WLAN.WLANChannelBandwidth.value == "1")
		document.getElementById("HT_BW_1_div").style.display="";
	else
		document.getElementById("HT_BW_1_div").style.display="none";		

	var vAuthMode = document.WLAN.WEP_Selection.value;

	if(document.getElementById("WEP-64Bits_div") != null)
		document.getElementById("WEP-64Bits_div").style.display="none";
	if(document.getElementById("WEP-128Bits_div") != null)
		document.getElementById("WEP-128Bits_div").style.display="none";
	if(document.getElementById("WPA2PSK_div") != null)
		document.getElementById("WPA2PSK_div").style.display="none";
	if(document.getElementById("WPAPSK_div") != null)
		document.getElementById("WPAPSK_div").style.display="none";
	if(document.getElementById("WPAPSKWPA2PSK_div") != null)
		document.getElementById("WPAPSKWPA2PSK_div").style.display="none";	
	if(vAuthMode == "WEP-64Bits"){
		if(document.getElementById("WEP-64Bits_div") != null)
			document.getElementById("WEP-64Bits_div").style.display="";
	}else if(vAuthMode == "WEP-128Bits"){
		if(document.getElementById("WEP-128Bits_div") != null)
			document.getElementById("WEP-128Bits_div").style.display="";
	}else if(vAuthMode == "WPA2PSK"){
		if(document.getElementById("WPA2PSK_div") != null)
			document.getElementById("WPA2PSK_div").style.display="";
	}else if(vAuthMode == "WPAPSK"){
		if(document.getElementById("WPAPSK_div") != null)
			document.getElementById("WPAPSK_div").style.display="";
	}else if(vAuthMode == "WPAPSKWPA2PSK"){
		if(document.getElementById("WPAPSKWPA2PSK_div") != null)
			document.getElementById("WPAPSKWPA2PSK_div").style.display="";	
	}
}

function SelectValue(o,v){
	for(var i=0; i<o.options.length; i++)
		if(o.options[i].value == v){
		o.options[i].selected=true;
		break;
	}
}
</script>
</head>
<body onLoad="doLoad()">
<FORM METHOD="POST" ACTION="/cgi-bin/home_wireless.asp" name="WLAN">
<div align=center>
<INPUT TYPE="HIDDEN" NAME="BasicRate_Value1" VALUE="15">
<INPUT TYPE="HIDDEN" NAME="BasicRate_Value2" VALUE="3">
<INPUT TYPE="HIDDEN" NAME="BasicRate_Value3" VALUE="351">
<INPUT TYPE="HIDDEN" NAME="CountryRegion0" value="0">
<INPUT TYPE="HIDDEN" NAME="CountryRegion1" value="1">
<INPUT TYPE="HIDDEN" NAME="CountryRegion2" value="2">
<INPUT TYPE="HIDDEN" NAME="CountryRegion3" value="3">
<INPUT TYPE="HIDDEN" NAME="CountryRegion5" value="5">
<INPUT TYPE="HIDDEN" NAME="CountryRegion6" value="6">
<INPUT TYPE="HIDDEN" NAME="HTMCSAUTO" value="33">
<INPUT TYPE="HIDDEN" NAME="HTBW" value="0">
<!--
<INPUT TYPE="HIDDEN" NAME="WPSConfigured" value="2">
<INPUT TYPE="HIDDEN" NAME="WpsConfModeAll" value="7">
<INPUT TYPE="HIDDEN" NAME="WpsConfModeNone" value="0">
-->

<INPUT TYPE="HIDDEN" NAME="Is11nMode"  value="<%tcWebApi_get("WLan_Common","11nMode","s")%>">
<INPUT TYPE="HIDDEN" NAME="ExtChannFlag"  value="<%tcWebApi_get("WLan_Common","HT_EXTCHA","s")%>">
<!--
<INPUT TYPE="HIDDEN" NAME="wlan_VC" value="<%tcWebApi_get("WebCurSet_Entry","wlan_id","s")%>">
-->
<!--<INPUT TYPE="HIDDEN" NAME="WpsStart" value="<%tcWebApi_get("Info_WLan","WPSActiveStatus","s")%>">
<INPUT TYPE="HIDDEN" NAME="WpsStart" value="0">
<INPUT TYPE="HIDDEN" NAME="WpsOOB" value="0">
<INPUT TYPE="HIDDEN" NAME="isInWPSing" value="<%tcWebApi_get("Info_WLan","wlanWPStimerRunning","s")%>">
-->
<INPUT TYPE="HIDDEN" NAME="BssidNum" value="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">
<INPUT TYPE="HIDDEN" NAME="CountryName" value="<%tcWebApi_get("WLan_Common","Country","s")%>">
<INPUT TYPE="HIDDEN" NAME="hCountryRegion" value="<%tcWebApi_get("WLan_Common","CountryRegion","s")%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<% If tcWebApi_get("Info_WLan","isExist","h") = "Error" Then%>
	<td class="tabdata"> <div align=center><font color="#FF0000" size=+1><%tcWebApi_get("String_Entry","WirelessErrText","s")%></font></div></td>
	<% Else %>  
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessAccessPointSetText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"></td>
		<td width="10" ></td>
		<td width="440"></td>
	</tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessAccessPointText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wlan_APenable" VALUE="1" <% if tcWebApi_get("WLan_Common","APOn","h") = "1" then asp_Write("checked") end if %> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessAccessPoint0Text","s")%></font>        
			<INPUT TYPE="RADIO" NAME="wlan_APenable" VALUE="0" <% if tcWebApi_get("WLan_Common","APOn","h") = "0" then asp_Write("checked") end if %> <% if tcWebApi_get("WLan_Common","APOn","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessAccessPoint1Text","s")%></font>
		</td>
	</tr>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessChannelText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="Countries_Channels" SIZE="1" onChange="doRegionCheck()" >
				
<script language="JavaScript" type='text/javascript'>
	var countryarr = new Array(98);
	countryarr[0]="ALBANIA";
	countryarr[1]="ALGERIA";
	countryarr[2]="ARGENTINA";
	countryarr[3]="ARMENIA";
	countryarr[4]="AUSTRALIA";
	countryarr[5]="AUSTRIA";
	countryarr[6]="AZERBAIJAN";
	countryarr[7]="BAHRAIN";
	countryarr[8]="BELARUS";
	countryarr[9]="BELGIUM";
	countryarr[10]="BELIZE";
	countryarr[11]="BOLVIA";
	countryarr[12]="BRAZIL";
	countryarr[13]="BRUNEI DARUSSALAM";
	countryarr[14]="BULGARIA";
	countryarr[15]="CANADA";
	countryarr[16]="CHILE";
	countryarr[17]="CHINA";
	countryarr[18]="COLOMBIA";
	countryarr[19]="COSTA RICA";
	countryarr[20]="CROATIA";
	countryarr[21]="CYPRUS";
	countryarr[22]="CZCH REPUBLIC";
	countryarr[23]="DENMARK";
	countryarr[24]="DOMINICAN REPUBLIC";
	countryarr[25]="ECUADOR";
	countryarr[26]="EGYPT";
	countryarr[27]="ELSALVADOR";
	countryarr[28]="FINLAND";
	countryarr[29]="FRANCE";
	countryarr[30]="GEORGIA";
	countryarr[31]="GERMANY";
	countryarr[32]="GREECE";
	countryarr[33]="GUATEMALA";
	countryarr[34]="HONDURAS";
	countryarr[35]="HONGKONG";
	countryarr[36]="HUNGARY";
	countryarr[37]="ICELAND";
	countryarr[38]="INDIA";
	countryarr[39]="INDONESIA";
	countryarr[40]="IRAN";
	countryarr[41]="IRELAND";
	countryarr[42]="ISAREL";
	countryarr[43]="ITALY";
	countryarr[44]="JAPAN";
	countryarr[45]="KAZAKHSTAN";
	countryarr[46]="NORTH KOREA";
	countryarr[47]="KORE AREPUBLIC";
	countryarr[48]="LATVIA";
	countryarr[49]="LEBANON";
	countryarr[50]="LIECHTENSTEIN";
	countryarr[51]="LITHUANIA";
	countryarr[52]="LUXEMBOURG";
	countryarr[53]="MACAU";
	countryarr[54]="MACEDONIA";
	countryarr[55]="MALAYSIA";
	countryarr[56]="MEXICO";
	countryarr[57]="MONACO";
	countryarr[58]="MOROCCO";
	countryarr[59]="NETHERLANDS";
	countryarr[60]="NEW ZEALAND";
	countryarr[61]="NORWAY";
	countryarr[62]="OMAN";
	countryarr[63]="PAKISTAN";
	countryarr[64]="PANAMA";
	countryarr[65]="PERU";
	countryarr[66]="PHILIPPINES";
	countryarr[67]="POLAND";
	countryarr[68]="PORTUGAL";
	countryarr[69]="PUERTO RICO";
	countryarr[70]="QATAR";
	countryarr[71]="ROMANIA";
	countryarr[72]="RUSSIA";
	countryarr[73]="SAUDI ARABIA";
	countryarr[74]="SINGAPORE";
	countryarr[75]="SLOVAKIA";
	countryarr[76]="SLOVENIA";
	countryarr[77]="SOUTH AFRICA";
	countryarr[78]="SPAIN";
	countryarr[79]="SWEDAN";
	countryarr[80]="SWITZERLAND";
	countryarr[81]="SYRIAN ARAB REPUBLIC";
	countryarr[82]="TAIWAN";
	countryarr[83]="THAILAND";
	countryarr[84]="TRIANIDAD AND TOBAGO";
	countryarr[85]="TUNISIA";
	countryarr[86]="TURKEY";
	countryarr[87]="UKRAINE";
	countryarr[88]="UNITED ARAB EMIRATES";
	countryarr[89]="UNITED KINGDOM";
	countryarr[90]="UNITED STATES";
	countryarr[91]="URUGUAY";
	countryarr[92]="UZBEKISTAN";
	countryarr[93]="VENEZUELA";
	countryarr[94]="VIETNAM";
	countryarr[95]="YEMEN";
	countryarr[96]="ZIMBABWE";
	countryarr[97]="Undefined";
	
	for(i=0;i<98;i++)
	{
		if(document.WLAN.CountryName.value.match(countryarr[i]) != null)
		{
			document.WLAN.Countries_Channels[i]=new Option(countryarr[i],countryarr[i],false,true);
			document.WLAN.Countries_Channels[i].selected=true;
		}
		else
		{
			if(countryarr[i].match("TAIWAN") !=null){
				document.WLAN.Countries_Channels[i]=new Option(countryarr[i],countryarr[i],true,false);
				//document.WLAN.Countries_Channels[i].selected=true;
			}
		else{
			document.WLAN.Countries_Channels[i]=new Option(countryarr[i],countryarr[i],false,false);
			document.WLAN.Countries_Channels[i].selected=false;
		}
	}
}
</script>

			</SELECT>     
			<SELECT NAME="Channel_ID" SIZE="1" onChange="doExtChaLockChange()">
			</SELECT>
        <font color="#000000"><%tcWebApi_get("String_Entry","WirelessCurrentChannelText","s")%></font> 
        <%if tcWebApi_get("WLan_Common","Channel","h") = "0" then%>
        <INPUT TYPE="TEXT" NAME="CurrentChannel" SIZE="3" MAXLENGTH="2" VALUE="<%If tcWebApi_get("Info_WLan","CurrentChannel","h") <> "N/A" then tcWebApi_get("Info_WLan","CurrentChannel","s") end if%>" disabled>
		<%else%>
		<INPUT TYPE="TEXT" NAME="CurrentChannel" SIZE="3" MAXLENGTH="2" VALUE="<%If tcWebApi_get("WLan_Common","Channel","h") <> "N/A" then tcWebApi_get("WLan_Common","Channel","s") end if%>" disabled>
		<%end if%>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessBeaconIntervalText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="BeaconInterval" SIZE="7" MAXLENGTH="4" VALUE="<%If tcWebApi_get("WLan_Common","BeaconPeriod","h") <> "N/A" then tcWebApi_get("WLan_Common","BeaconPeriod","s") else asp_Write("100") end if%>"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessIntervalrangeText","s")%></font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessThresholdText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="RTSThreshold" SIZE="7" MAXLENGTH="5" VALUE="<%If tcWebApi_get("WLan_Common","RTSThreshold","h") <> "N/A" then tcWebApi_get("WLan_Common","RTSThreshold","s") else asp_Write("2347") end if%>"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessThresholdrangeText","s")%></font>
		</td>
	</tr>
	<tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessFragThresholdText","s")%></font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
			<INPUT TYPE="TEXT" NAME="FragmentThreshold" SIZE="7" MAXLENGTH="5" VALUE="<%If tcWebApi_get("WLan_Common","FragThreshold","h") <> "N/A" then tcWebApi_get("WLan_Common","FragThreshold","s") else asp_Write("2346") end if%>"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessFragThresText","s")%></font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDTIMText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="DTIM" SIZE="7" MAXLENGTH="5" VALUE="<%If tcWebApi_get("WLan_Common","DtimPeriod","h") <> "N/A" then tcWebApi_get("WLan_Common","DtimPeriod","s") else asp_Write("1") end if%>" ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDTIMrangeText","s")%></font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessModeText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="WirelessMode" SIZE="1" onChange="doWirelessModeChange()">
				<OPTION value="1" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessMode0Text","s")%>
				<OPTION value="4" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "4" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessMode1Text","s")%>
				<OPTION value="0" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessMode2Text","s")%>
				<OPTION value="6" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "6" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessMode3Text","s")%>
				<OPTION value="7" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "7" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessMode4Text","s")%>
				<OPTION value="9" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "9" then asp_Write("selected") end if %> <%If tcWebApi_get("WLan_Common","WirelessMode","h") = "N/A" then asp_Write("checked") end if%>><%tcWebApi_get("String_Entry","WirelessMode5Text","s")%>
			</SELECT>
		</td>
	</tr>
	</table>
	<div id="11nMode_1_div">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
      <td width="150" height="20" class="title-main"> <font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessChannelBandwidthText","s")%></font>
      </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="150"><hr noshade></td>
    <td width="10" ><hr noshade></td>
    <td width="440"><hr noshade></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right"><font color="#000000">
    <%tcWebApi_get("String_Entry","WirelessChannelBandwidthText","s")%></font>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
      <td class="tabdata"> <select name="WLANChannelBandwidth" onChange="doChannelBandwidthChange();">
          <option value="0" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessChannelBandwidth0Text","s")%></option>
		  <option value="1" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Common","HT_BW","h") = "N/A" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessChannelBandwidth1Text","s")%></option>
        </select></td>
  </tr>
	</table>
	<div id="HT_BW_1_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td width="150" class="tabdata"><div align="right"><font color="#000000">
    <%tcWebApi_get("String_Entry","WirelessExtensionChannelText","s")%></font>
    </div></td>
    <td width="10" class="tabdata"><div align="center">:</div></td>
    <td width="440" class="tabdata">
        <select name="WLANExtensionChannel" onChange="doExtChannChange();">
        <option value="0" <% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "0" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "N/A" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessExtensionChannel0Text","s")%></option>
		<option value="1" <% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessExtensionChannel1Text","s")%></option>
        </select>
	</td>
  </tr>
	</table>
	</div>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td width="150" class="tabdata"><div align="right"><font color="#000000">
    <%tcWebApi_get("String_Entry","WirelessGuardIntervalText","s")%></font>
    </div></td>
    <td width="10" class="tabdata"><div align="center">:</div></td>
    <td width="440" class="tabdata">
        <select name="WLANGuardInterval">
        <option value="0" <% if tcWebApi_get("WLan_Common","HT_GI","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessGuardInterval0Text","s")%></option>
		<option value="1" <% if tcWebApi_get("WLan_Common","HT_GI","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Common","HT_GI","h") = "N/A" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessGuardInterval1Text","s")%></option>
        </select>
	</td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right"><font color="#000000">
    <%tcWebApi_get("String_Entry","WirelessMCSText","s")%></font>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <select name="WLANMCS">
        <option value="0" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "0" then asp_Write("selected") end if %>>0</option>
        <option value="1" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "1" then asp_Write("selected") end if %>>1</option>
		<option value="2" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "2" then asp_Write("selected") end if %>>2</option>
		<option value="3" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "3" then asp_Write("selected") end if %>>3</option>
		<option value="4" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "4" then asp_Write("selected") end if %>>4</option>
		<option value="5" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "5" then asp_Write("selected") end if %>>5</option>
		<option value="6" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "6" then asp_Write("selected") end if %>>6</option>
        <option value="7" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "7" then asp_Write("selected") end if %>>7</option>
        <option value="8" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "8" then asp_Write("selected") end if %>>8</option>
		<option value="9" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "9" then asp_Write("selected") end if %>>9</option>
		<option value="10" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "10" then asp_Write("selected") end if %>>10</option>
		<option value="11" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "11" then asp_Write("selected") end if %>>11</option>
		<option value="12" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "12" then asp_Write("selected") end if %>>12</option>
		<option value="13" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "13" then asp_Write("selected") end if %>>13</option>
		<option value="14" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "14" then asp_Write("selected") end if %>>14</option>
		<option value="15" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "15" then asp_Write("selected") end if %>>15</option>
		<option value="33" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "33" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "N/A" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessGuardInterval1Text","s")%></option>
       </select>
	</td>
  </tr>
  </table>
  </div>
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessSSIDSettingsText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
<%
if tcWebApi_get("WLan_Common","BssidNum","h") = "1" then
%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSSIDindexText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
		<OPTION value="0" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "0" then asp_write("selected") end if%>>1
		</select>
		</td>
	</tr>

<%elseif tcWebApi_get("WLan_Common","BssidNum","h") = "2" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSSIDindexText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
		<OPTION value="0" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "0" then asp_write("selected") end if%>>1
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "1" then asp_write("selected") end if%>>2
		</select>
		</td>
	</tr>
<%
elseif tcWebApi_get("WLan_Common","BssidNum","h") = "3" then
%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSSIDindexText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
		<OPTION value="0" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "0" then asp_write("selected") end if%>>1
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "1" then asp_write("selected") end if%>>2
		<OPTION value="2" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "2" then asp_write("selected") end if%>>3
		</select>
		</td>
	</tr>
<%elseif tcWebApi_get("WLan_Common","BssidNum","h") = "4" then%>
<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSSIDindexText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
		<OPTION value="0" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "0" then asp_write("selected") end if%>>1
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "1" then asp_write("selected") end if%>>2
		<OPTION value="2" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "2" then asp_write("selected") end if%>>3
		<OPTION value="3" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "3" then asp_write("selected") end if%>>4
		</select>
		</td>
</tr>
<%end if%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSSIDText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="ESSID" SIZE="35" MAXLENGTH="32" VALUE="<%If tcWebApi_get("WLan_Entry","SSID","h") <> "N/A" then tcWebApi_get("WLan_Entry","SSID","s") end if%>">
		</td>
	</tr>
	<tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessBroadcastSSIDText","s")%></font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
			<INPUT TYPE="RADIO" NAME="ESSID_HIDE_Selection" VALUE="0" <%If tcWebApi_get("WLan_Entry","HideSSID","h") = "0" then asp_Write("checked") end if%> >		<font color="#000000"><%tcWebApi_get("String_Entry","WirelessBroadcastSSID0Text","s")%></font>		
			<INPUT TYPE="RADIO" NAME="ESSID_HIDE_Selection" VALUE="1" <%If tcWebApi_get("WLan_Entry","HideSSID","h") = "1" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","HideSSID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessBroadcastSSID1Text","s")%></font>
		</td>
	</tr>
	</table>
	<div id="11nMode_0_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
    <td width="150" class="light-orange"></td>
    <td width="10" class="light-orange"></td>
    <td width="150" class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWMMText","s")%></font></div></td>
    <td width="10" class="tabdata"><div align=center>:</div></td>
    <td width="440" class="tabdata">
			<INPUT TYPE="RADIO" NAME="WMM_Selection" VALUE="1" <%If tcWebApi_get("WLan_Entry","WMM","h") = "1" then asp_Write("checked") end if%> >		<font color="#000000"><%tcWebApi_get("String_Entry","WirelessWMM0Text","s")%></font>		
			<INPUT TYPE="RADIO" NAME="WMM_Selection" VALUE="0" <%If tcWebApi_get("WLan_Entry","WMM","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","WMM","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWMM1Text","s")%></font>
		</td>
	</tr>
	</table>
	</div>
<!--
  <% if tcWebApi_get("WebCurSet_Entry","wlan_id","h") = "0" then %>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">Use WPS</div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata"><input name="UseWPS_Selection" VALUE="1" <%If tcWebApi_get("WLan_Entry","WPSConfMode","h") <> "0" then asp_Write("checked") end if%> onClick="doWPSUseChange();" type="radio"><font color="#000000">Yes</font>
	<input name="UseWPS_Selection" VALUE="0" <%If tcWebApi_get("WLan_Entry","WPSConfMode","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","WPSConfMode","h") = "N/A" then asp_Write("checked") end if%> onClick="doWPSUseChange();" type="radio"><font color="#000000">No</font></td>
  </tr>
	<%If tcWebApi_get("WLan_Entry","WPSConfMode","h") <> "0" then %>
  <tr>
    <td class="title-main" width="150" height="30">
    <font color="#ffffff">WPS Settings</font></td>
    <td class="black" width="10">&nbsp;</td><td width="150"><hr noshade="noshade"></td><td width="10"><hr noshade="noshade"></td>
    <td width="440"><hr noshade="noshade"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td>
    <td class="tabdata"> <div align="right">WPS state</div></td>
    <td class="tabdata"><div align="center">:</div></td>
      <td class="tabdata"><%If tcWebApi_get("WLan_Entry", "WPSConfStatus", "h") = "1" then asp_Write("Unconfigured") elseif tcWebApi_get("WLan_Entry", "WPSConfStatus", "h") = "2" then asp_Write("Configured") elseif tcWebApi_get("WLan_Entry", "WPSConfStatus", "h") = "N/A" then asp_Write("Unconfigured") end if%></td>
  </tr>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">WPS mode</div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata"><input name="WPSMode_Selection" value="0" onClick="doWPSModeChange();" <%If tcWebApi_get("WLan_Entry","WPSMode","h") = "0" then asp_Write("checked") end if%> type="radio">PIN code<input name="WPSMode_Selection" value="1" onClick="doWPSModeChange();" <%If tcWebApi_get("WLan_Entry","WPSMode","h") = "1" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","WPSMode","h") = "N/A" then asp_Write("checked") end if%>  type="radio">PBC</td>
  </tr>
  <%If tcWebApi_get("WLan_Entry","WPSMode","h") = "0" then %>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"> <div align="right">AP self PIN code</div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata"><% tcWebApi_get("Info_WLan", "wlanSelfPinCode", "s") %></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"> <div align="right">enrollee PIN code</div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata"><input name="WPSEnrolleePINCode" size="8" maxlength="8" value="<%If tcWebApi_get("WLan_Entry","enrolleePinCode","h") = "N/A" then asp_Write("") else TCWebApi_get("WLan_Entry","enrolleePinCode","s") end if %>" onblur="doPINCodeCheck(this)" type="text"></td>
  </tr>
  <% end if %>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"><div align="right"></div></td>
    <td class="tabdata"><div align="center"></div></td>
    <td class="tabdata"><input name="StartWPS" value="<%IF tcWebApi_get("Info_WLan","wlanWPStimerRunning","h") = "0" then asp_Write("Start WPS") end if%> <%IF tcWebApi_get("Info_WLan","wlanWPStimerRunning","h") = "1" then asp_Write("Stop WPS") end if%><%IF tcWebApi_get("Info_WLan","wlanWPStimerRunning","h") = "N/A" then asp_Write("Start WPS") end if%>" onclick="doStartWPS();" type="button"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td>
    <td class="tabdata"> <div align="right">WPS progress</div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata"><% tcWebApi_get("Info_WLan", "wlanWPSStatus", "s") %></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"><div align="right"></div></td>
    <td class="tabdata"><div align="center"></div></td>
    <td class="tabdata"><input name="ResetOOB" value="Reset to OOB" onclick="doResetOOB();" type="button" ></td>
  </tr>
  <% end if %>
  <% end if %>
  -->
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessAuthTypeText","s")%></font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<SELECT NAME="WEP_Selection" SIZE="1" onChange="doWEPChange()">
				<OPTION value="OPEN" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessWEPStatus0Text","s")%>
				<OPTION value="WEP-64Bits" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-64Bits" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessWEPStatus1Text","s")%>
				<OPTION value="WEP-128Bits" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-128Bits" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessWEPStatus2Text","s")%>
				<OPTION value="WPAPSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSK" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessWEPStatus3Text","s")%>
				<OPTION value="WPA2PSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2PSK" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessWEPStatus4Text","s")%> 
				<OPTION value="WPAPSKWPA2PSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSKWPA2PSK" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessWEPStatus5Text","s")%>				
			</SELECT>
			<INPUT TYPE="HIDDEN" NAME="wlanWEPFlag" VALUE="0">
		</td>
	</tr>
</table>

<div align=center>
<div id="WEP-64Bits_div">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessWEPText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus1Text","s")%></font></div></td>
		<td class="tabdata">&nbsp;</td>
        <td class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus1CommText","s")%></font></td>
	</tr>
	<tr>
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus2Text","s")%></font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus2CommText","s")%></font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
			<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") elseif tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#1</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">   
			<INPUT TYPE="TEXT" NAME="WEP_Key13" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this)" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#2</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key23" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this)" >
		</td>
	</tr>
	<tr>                                                                                      
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#3</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key33" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key3Str","s") end if%>"  onBlur="doKEYcheck(this)" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#4</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key43" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key4Str","s") end if%>"  onBlur="doKEYcheck(this)" >
		</td>
	</tr>	
	</table>
	</div>
	<div id="WEP-128Bits_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessWEPText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td><td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus1Text","s")%></font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus1CommText","s")%></font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus2Text","s")%></font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessWEPStatus2CommText","s")%></font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#1</div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">   
			<INPUT TYPE="TEXT" NAME="WEP_Key14" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#2</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WEP_Key24" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td><td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#3</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key34" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key3Str","s") end if%>"  onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessDefaultKeyText","s")%></font>#4</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WEP_Key44" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key4Str","s") end if%>"  onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	</table>
	</div>

	<div id="WPA2PSK_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="30" width="150" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessWPAPSKText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessEncryptionText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection4" SIZE="1">
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP
				<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES 
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSharedKeyText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey1" SIZE="48" MAXLENGTH="64" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck(this)"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSharedKeyCommText","s")%>
		</td>
	</tr>
	</table>
	</div>
	<div id="WPAPSK_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">		
	<tr>		
		<td height="30" width="150" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessWPAPSKText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessEncryptionText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection5" SIZE="1">
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%>>TKIP
				<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if%>>AES 
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES
		</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSharedKeyText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey2" SIZE="48" MAXLENGTH="64" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck(this)"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSharedKeyCommText","s")%></font>
		</td>
	</tr>
	</table>
	</div>
	<div id="WPAPSKWPA2PSK_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="30" width="150" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","WirelessWPAPSKText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessEncryptionText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection6" SIZE="1">
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP
				<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES 
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES 
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSharedKeyText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey3" SIZE="48" MAXLENGTH="64" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck(this)"><font color="#000000"><%tcWebApi_get("String_Entry","WirelessSharedKeyCommText","s")%></font>
		</td>
	</tr>
	</table>
	</div>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Wireless MAC Address Filter</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessActiveText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="WLAN_FltActive" VALUE="1" <%If tcWebApi_get("WLan_Entry","AccessPolicy","h") <> "0" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessAccessPoint0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="WLAN_FltActive" VALUE="0" <%If tcWebApi_get("WLan_Entry","AccessPolicy","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","AccessPolicy","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000"><%tcWebApi_get("String_Entry","WirelessAccessPoint1Text","s")%></font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","WirelessActionText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="WLAN_FltAction" SIZE="1">
				<OPTION value="1" <% if tcWebApi_get("WLan_Entry","AccessPolicy","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessAction0Text","s")%>
				<OPTION value="2" <% if tcWebApi_get("WLan_Entry","AccessPolicy","h") = "2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WirelessAction1Text","s")%>
			</SELECT>
			<font color="#000000"><%tcWebApi_get("String_Entry","WirelessActionCommText","s")%></font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress0Text","s")%>	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
 		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC1" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC0","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC0","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress1Text","s")%>	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
 		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC2" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC1","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC1","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress2Text","s")%>	</div></td>
		<td class="tabdata"><div align="center">:</div></td>
 		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC3" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC2","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC2","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress3Text","s")%>	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC4" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC3","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC3","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress4Text","s")%>	</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC5" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC4","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC4","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress5Text","s")%>	</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC6" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC5","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC5","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress6Text","s")%>	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC7" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC6","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC6","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right"><%tcWebApi_get("String_Entry","WirelessMacAddress7Text","s")%>	</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC8" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC7","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC7","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr height="42">
		<td width="160" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td>
		<td width="440" class="orange">
			<INPUT TYPE="SUBMIT" NAME="BUTTON" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="return doSave();">
			<INPUT TYPE="BUTTON" NAME="CancelBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>" onClick="javascript:window.location='home_wireless.asp'">
			<INPUT TYPE="HIDDEN" NAME="CountryChange" VALUE="0">
		</td>
	</tr>
</table>
</div>
</form>
<%End If%>
</body>
</html>
