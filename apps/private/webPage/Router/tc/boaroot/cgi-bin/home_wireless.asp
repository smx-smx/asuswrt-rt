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
	TCWebApi_set("WLan_Entry","SSID","ESSID")
	TCWebApi_set("WLan_Common","BeaconPeriod","BeaconInterval")	
	TCWebApi_set("WLan_Entry","HideSSID","ESSID_HIDE_Selection")
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
	TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey")	
	TCWebApi_set("WLan_Entry","Key1Str","WEP_Key1")	
	TCWebApi_set("WLan_Entry","Key2Str","WEP_Key2")		
	TCWebApi_set("WLan_Entry","Key3Str","WEP_Key3")		
	TCWebApi_set("WLan_Entry","Key4Str","WEP_Key4")	
	TCWebApi_set("WLan_Entry","AuthMode","WEP_Selection")
	TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection")	
	TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey")		
	tcWebApi_commit("WLan_Entry")	
End If
%>

<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language="JavaScript">
function strESSIDCheck(str) {
if(str.value.match(/[^\x00-\xff]/g)){
	alert("Invalid SSID Input!");
	return true;
}
if(document.WLAN.ESSID.value.length <= 0){
	alert("SSID is empty");
	return true;
}
return false;
}
function doRegionCheck(){
	document.WLAN.CountryChange.value = 1; 
	document.WLAN.submit();
}
function doWEPChange(){
	document.WLAN.wlanWEPFlag.value = 1;
	document.WLAN.submit();
}
function doSSIDChange(){
	//alert(document.WLAN.SSID_INDEX.value);
	document.WLAN.wlanWEPFlag.value = 2;
	document.WLAN.submit();
}
function wpapskCheck() {
   	if(document.WLAN.PreSharedKey.value.length < 8) {
    alert('Pre-Shared Key length error!');		return true;
	}	
   	if(document.WLAN.PreSharedKey.value.length > 63) {
    alert('Pre-Shared Key length error!');		return true;
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
				alert("Invalid Key Value");
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
				alert("Invalid Key Value");
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
			alert("Invalid Key Value");
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
				alert("Invalid Key Value");
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
				alert("Invalid Key Value");
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
			alert("Invalid Key Value");
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
      alert("Invalid MAC Address");        object.focus();
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
       alert("Invalid MAC Address");       	object.focus();
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
         alert("Invalid MAC Address");         	object.focus();
        return;
      }
      i = i + 3;
    }
    return; 
  }
  else
  {
  alert("Invalid MAC Address");     	object.focus();
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
    alert("Non-integer value given " + value);    return true;
  }
  if (value < 20 || value > 1000) {
    alert("Beacon value must be between 20 and 1000");    return true;  	
  }
  return false;
}

function checkRTS(value) 
{
  if (!isNumeric(value)) {
    alert("Non-integer value given " + value);    return true;
  }
  if (value < 1500 || value > 2347) {
    alert("RTS Threshold value must be between 1500 and 2347");    return true;  	
  }
  return false;
}

function checkFrag(value) 
{
  if (!isNumeric(value)) {
    alert("Non-integer value given " + value);    return true;
  }
  if (value < 256 || value > 2346) {
    alert("Fragmentation Threshold value must be between 256 and 2346");    return true;  	
  }
  if (value % 2) {
    alert("Fragmentation Threshold value must be an even number");    return true;  	
  }
  return false;
}

function checkDTIM(value) 
{
  if (!isNumeric(value)) {
    alert("Non-integer value given " + value);    return true;
  }
  if (value < 1 || value > 255) {
    alert("DTIM value must be between 1 and 255");		return true;
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
    alert('too many quotation marks!');		return true;
	}	
	return false;
}

function doSave(){
	if(checkBeacon(document.WLAN.BeaconInterval.value) ||
		checkRTS(document.WLAN.RTSThreshold.value) ||
		checkFrag(document.WLAN.FragmentThreshold.value) ||
		checkDTIM(document.WLAN.DTIM.value)
	){
		return false;
	}
		
	if((document.WLAN.WEP_Selection.selectedIndex == 3) ||
	(document.WLAN.WEP_Selection.selectedIndex == 4)){
		if (wpapskCheck()){
			return false;
		}
		
		if(quotationCheck(document.WLAN.PreSharedKey, 64) ){
			return false;	 
		}  			
	}
      	
	if((document.WLAN.WEP_Selection.selectedIndex == 1) ||
	(document.WLAN.WEP_Selection.selectedIndex == 2)){
		if((!doKEYcheck(document.WLAN.WEP_Key1))||
		(!doKEYcheck(document.WLAN.WEP_Key2))||
		(!doKEYcheck(document.WLAN.WEP_Key3))||
		(!doKEYcheck(document.WLAN.WEP_Key4))){
			alert("key check fail");
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
	document.WLAN.submit();
}

function checkSelectedKEY(){
	var group = document.WLAN.DefWEPKey;
	var curCBX;

	for (var i=0; i<group.length; i++){
		if (group[i].checked)
		break;
	}
	
	switch (i)
	{
	case 0:
		curCBX = document.WLAN.WEP_Key1;
		break;
	case 1:
		curCBX = document.WLAN.WEP_Key2;
		break;
	case 2:
		curCBX = document.WLAN.WEP_Key3;
		break;
	case 3:
		curCBX = document.WLAN.WEP_Key4;
		break;
	default:
	  ;
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
					alert("Invalid Key Value");
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
					alert("Invalid Key Value");
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
						alert("Invalid Key Value");
						curCBX.focus();
						return false;
					}
				}
			}
			else
			{
				alert("Invalid Key Value");
				curCBX.focus();
				return false;
			}
		}
		else
		{
			alert("Invalid Key Value");
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
					alert("Invalid Key Value");
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
					alert("Invalid Key Value");
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
						alert("Invalid Key Value");
						curCBX.focus();
						return false;
					}
				}
			}
			else
			{
				alert("Invalid Key Value");
				curCBX.focus();
				return false;
			}
		}
		else
		{
			alert("Invalid Key Value");
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

</script>
</head>

<body>
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
<!--
<INPUT TYPE="HIDDEN" NAME="wlan_VC" value="<%tcWebApi_get("WebCurSet_Entry","wlan_id","s")%>">
-->
<INPUT TYPE="HIDDEN" NAME="BssidNum" value="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">
<INPUT TYPE="HIDDEN" NAME="CountryName" value="<%tcWebApi_get("WLan_Common","Country","s")%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<% If tcWebApi_get("Info_WLan","isExist","h") = "Error" Then%>
	<td class="tabdata"> <div align=center><font color="#FF0000" size=+1>There are some errors when cpe getting data from EEPROM!!</font></div></td>
	<% Else %>  
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Access Point Settings</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"></td>
		<td width="10" ></td>
		<td width="440"></td>
	</tr>
	<tr>
		<td class="light-orange"></td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Access Point</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wlan_APenable" VALUE="1" <% if tcWebApi_get("WLan_Common","APOn","h") = "1" then asp_Write("checked") end if %> ><font color="#000000">Activated</font>        
			<INPUT TYPE="RADIO" NAME="wlan_APenable" VALUE="0" <% if tcWebApi_get("WLan_Common","APOn","h") = "0" then asp_Write("checked") end if %> <% if tcWebApi_get("WLan_Common","APOn","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font>
		</td>
	</tr>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">Channel</font></div></td>
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
	countryarr[24]="DOMINICANRE PUBLIC";
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
			<SELECT NAME="Channel_ID" SIZE="1">
				<%if tcWebApi_get("WLan_Common","CountryRegion","h") = "0" then%>
				<OPTION value="1" <% if tcWebApi_get("WLan_Common","Channel","h") = "1" then asp_Write("selected") end if %>>01
				<OPTION value="2" <% if tcWebApi_get("WLan_Common","Channel","h") = "2" then asp_Write("selected") end if %>>02
				<OPTION value="3" <% if tcWebApi_get("WLan_Common","Channel","h") = "3" then asp_Write("selected") end if %>>03
				<OPTION value="4" <% if tcWebApi_get("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>04
				<OPTION value="5" <% if tcWebApi_get("WLan_Common","Channel","h") = "5" then asp_Write("selected") end if %>>05
				<OPTION value="6" <% if tcWebApi_get("WLan_Common","Channel","h") = "6" then asp_Write("selected") end if %>>06
				<OPTION value="7" <% if tcWebApi_get("WLan_Common","Channel","h") = "7" then asp_Write("selected") end if %>>07
				<OPTION value="8" <% if tcWebApi_get("WLan_Common","Channel","h") = "8" then asp_Write("selected") end if %>>08
				<OPTION value="9" <% if tcWebApi_get("WLan_Common","Channel","h") = "9" then asp_Write("selected") end if %>>09
				<OPTION value="10" <% if tcWebApi_get("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="11" <% if tcWebApi_get("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11
				<%elseif tcWebApi_get("WLan_Common","CountryRegion","h") = "2" then%>
				<OPTION value="10" <% if tcWebApi_get("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="11" <% if tcWebApi_get("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11
				<%elseif tcWebApi_get("WLan_Common","CountryRegion","h") = "3" then%>
				<OPTION value="10" <% if tcWebApi_get("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="11" <% if tcWebApi_get("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11
				<OPTION value="12" <% if tcWebApi_get("WLan_Common","Channel","h") = "12" then asp_Write("selected") end if %>>12
				<OPTION value="13" <% if tcWebApi_get("WLan_Common","Channel","h") = "13" then asp_Write("selected") end if %>>13
				<%elseif tcWebApi_get("WLan_Common","CountryRegion","h") = "4" then%>
				<OPTION value="4" <% if tcWebApi_get("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>04
				<%elseif tcWebApi_get("WLan_Common","CountryRegion","h") = "5" then%>
				<OPTION value="1" <% if tcWebApi_get("WLan_Common","Channel","h") = "1" then asp_Write("selected") end if %>>01
				<OPTION value="2" <% if tcWebApi_get("WLan_Common","Channel","h") = "2" then asp_Write("selected") end if %>>02
				<OPTION value="3" <% if tcWebApi_get("WLan_Common","Channel","h") = "3" then asp_Write("selected") end if %>>03
				<OPTION value="4" <% if tcWebApi_get("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>04
				<OPTION value="5" <% if tcWebApi_get("WLan_Common","Channel","h") = "5" then asp_Write("selected") end if %>>05
				<OPTION value="6" <% if tcWebApi_get("WLan_Common","Channel","h") = "6" then asp_Write("selected") end if %>>06
				<OPTION value="7" <% if tcWebApi_get("WLan_Common","Channel","h") = "7" then asp_Write("selected") end if %>>07
				<OPTION value="8" <% if tcWebApi_get("WLan_Common","Channel","h") = "8" then asp_Write("selected") end if %>>08
				<OPTION value="9" <% if tcWebApi_get("WLan_Common","Channel","h") = "9" then asp_Write("selected") end if %>>09
				<OPTION value="10" <% if tcWebApi_get("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="11" <% if tcWebApi_get("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11
				<OPTION value="12" <% if tcWebApi_get("WLan_Common","Channel","h") = "12" then asp_Write("selected") end if %>>12
				<OPTION value="13" <% if tcWebApi_get("WLan_Common","Channel","h") = "13" then asp_Write("selected") end if %>>13
				<OPTION value="14" <% if tcWebApi_get("WLan_Common","Channel","h") = "14" then asp_Write("selected") end if %>>14
				<%elseif tcWebApi_get("WLan_Common","CountryRegion","h") = "6" then%>
				<OPTION value="3" <% if tcWebApi_get("WLan_Common","Channel","h") = "3" then asp_Write("selected") end if %>>03
				<OPTION value="4" <% if tcWebApi_get("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>04
				<OPTION value="5" <% if tcWebApi_get("WLan_Common","Channel","h") = "5" then asp_Write("selected") end if %>>05
				<OPTION value="6" <% if tcWebApi_get("WLan_Common","Channel","h") = "6" then asp_Write("selected") end if %>>06
				<OPTION value="7" <% if tcWebApi_get("WLan_Common","Channel","h") = "7" then asp_Write("selected") end if %>>07
				<OPTION value="8" <% if tcWebApi_get("WLan_Common","Channel","h") = "8" then asp_Write("selected") end if %>>08
				<OPTION value="9" <% if tcWebApi_get("WLan_Common","Channel","h") = "9" then asp_Write("selected") end if %>>09
				<%else%>
				<OPTION value="1" <% if tcWebApi_get("WLan_Common","Channel","h") = "1" then asp_Write("selected") end if %>>01
				<OPTION value="2" <% if tcWebApi_get("WLan_Common","Channel","h") = "2" then asp_Write("selected") end if %>>02
				<OPTION value="3" <% if tcWebApi_get("WLan_Common","Channel","h") = "3" then asp_Write("selected") end if %>>03
				<OPTION value="4" <% if tcWebApi_get("WLan_Common","Channel","h") = "4" then asp_Write("selected") end if %>>04
				<OPTION value="5" <% if tcWebApi_get("WLan_Common","Channel","h") = "5" then asp_Write("selected") end if %>>05
				<OPTION value="6" <% if tcWebApi_get("WLan_Common","Channel","h") = "6" then asp_Write("selected") end if %>>06
				<OPTION value="7" <% if tcWebApi_get("WLan_Common","Channel","h") = "7" then asp_Write("selected") end if %>>07
				<OPTION value="8" <% if tcWebApi_get("WLan_Common","Channel","h") = "8" then asp_Write("selected") end if %>>08
				<OPTION value="9" <% if tcWebApi_get("WLan_Common","Channel","h") = "9" then asp_Write("selected") end if %>>09
				<OPTION value="10" <% if tcWebApi_get("WLan_Common","Channel","h") = "10" then asp_Write("selected") end if %>>10
				<OPTION value="11" <% if tcWebApi_get("WLan_Common","Channel","h") = "11" then asp_Write("selected") end if %>>11
				<OPTION value="12" <% if tcWebApi_get("WLan_Common","Channel","h") = "12" then asp_Write("selected") end if %>>12
				<OPTION value="13" <% if tcWebApi_get("WLan_Common","Channel","h") = "13" then asp_Write("selected") end if %>>13
				<%end if%>
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Beacon Interval</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="BeaconInterval" SIZE="7" MAXLENGTH="4" VALUE="<%If tcWebApi_get("WLan_Common","BeaconPeriod","h") <> "N/A" then tcWebApi_get("WLan_Common","BeaconPeriod","s") else asp_Write("100") end if%>"><font color="#000000">(range: 20~1000)</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">RTS/CTS Threshold</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="RTSThreshold" SIZE="7" MAXLENGTH="5" VALUE="<%If tcWebApi_get("WLan_Common","RTSThreshold","h") <> "N/A" then tcWebApi_get("WLan_Common","RTSThreshold","s") else asp_Write("2347") end if%>"><font color="#000000">(range: 1500~2347)</font>
		</td>
	</tr>
	<tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">Fragmentation Threshold</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
			<INPUT TYPE="TEXT" NAME="FragmentThreshold" SIZE="7" MAXLENGTH="5" VALUE="<%If tcWebApi_get("WLan_Common","FragThreshold","h") <> "N/A" then tcWebApi_get("WLan_Common","FragThreshold","s") else asp_Write("2346") end if%>"><font color="#000000">(range: 256~2346, even numbers only)</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">DTIM</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="DTIM" SIZE="7" MAXLENGTH="5" VALUE="<%If tcWebApi_get("WLan_Common","DtimPeriod","h") <> "N/A" then tcWebApi_get("WLan_Common","DtimPeriod","s") else asp_Write("1") end if%>" ><font color="#000000">(range: 1~255)</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">802.11 b/g</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="WirelessMode" SIZE="1">
				<OPTION value="0" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "0" then asp_Write("selected") end if %>>802.11b+g
				<OPTION value="1" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "1" then asp_Write("selected") end if %>>802.11b
				<OPTION value="2" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "2" then asp_Write("selected") end if %> <%If tcWebApi_get("WLan_Common","WirelessMode","h") = "N/A" then asp_Write("checked") end if%>>802.11g
			</SELECT>
		</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">SSID Settings</font></td>
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
		<td class="tabdata"><div align=right><font color="#000000">SSID index</font></div></td>
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
		<td class="tabdata"><div align=right><font color="#000000">SSID index</font></div></td>
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
		<td class="tabdata"><div align=right><font color="#000000">SSID index</font></div></td>
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
		<td class="tabdata"><div align=right><font color="#000000">SSID index</font></div></td>
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
		<td class="tabdata"><div align=right><font color="#000000">SSID</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="ESSID" SIZE="35" MAXLENGTH="32" VALUE="<%If tcWebApi_get("WLan_Entry","SSID","h") <> "N/A" then tcWebApi_get("WLan_Entry","SSID","s") end if%>">
		</td>
	</tr>
	<tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">Broadcast SSID</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
			<INPUT TYPE="RADIO" NAME="ESSID_HIDE_Selection" VALUE="0" <%If tcWebApi_get("WLan_Entry","HideSSID","h") = "0" then asp_Write("checked") end if%> >		<font color="#000000">Yes</font>		
			<INPUT TYPE="RADIO" NAME="ESSID_HIDE_Selection" VALUE="1" <%If tcWebApi_get("WLan_Entry","HideSSID","h") = "1" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","HideSSID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">No</font>
		</td>
	</tr>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Authentication Type</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="WEP_Selection" SIZE="1" onChange="doWEPChange()">
				<OPTION <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %>>OPEN
				<OPTION <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-64Bits" then asp_Write("selected") end if %>>WEP-64Bits
				<OPTION <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-128Bits" then asp_Write("selected") end if %>>WEP-128Bits
				<OPTION <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSK" then asp_Write("selected") end if %>>WPAPSK
				<OPTION <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2PSK" then asp_Write("selected") end if %>>WPA2PSK 
			</SELECT>
			<INPUT TYPE="HIDDEN" NAME="wlanWEPFlag" VALUE="0">
		</td>
	</tr>
</table>

<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
	<% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-64Bits"  then %>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">WEP</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 64-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000">For each key, please enter either (1) 5 characters excluding symbols, or (2) 10 characters ranging from 0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 128-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000">For each key, please enter either (1) 13 characters excluding symbols, or (2) 26 characters ranging from 0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") elseif tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#1</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">   
			<INPUT TYPE="TEXT" NAME="WEP_Key1" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this)" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#2</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key2" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this)" >
		</td>
	</tr>
	<tr>                                                                                      
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#3</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key3" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key3Str","s") end if%>"  onBlur="doKEYcheck(this)" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#4</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key4" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key4Str","s") end if%>"  onBlur="doKEYcheck(this)" >
		</td>
	</tr>
	<tr>
		
	<%elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-128Bits"  then %>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">WEP</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td><td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 64-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000">For each key, please enter either (1) 5 characters excluding symbols, or (2) 10 characters ranging from 0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 128-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000">For each key, please enter either (1) 13 characters excluding symbols, or (2) 26 characters ranging from 0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#1</div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">   
			<INPUT TYPE="TEXT" NAME="WEP_Key1" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#2</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WEP_Key2" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td><td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#3</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">           
			<INPUT TYPE="TEXT" NAME="WEP_Key3" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key3Str","s") end if%>"  onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#4</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WEP_Key4" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key4Str","s") end if%>"  onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>

	<% elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2PSK"  then %>
		<td height="30" class="title-main"><font color="#FFFFFF">WPA-PSK</font></td>
		<td class="black">&nbsp;</td><td><hr noshade></td><td><hr noshade></td>
		<td><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encryption</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection" SIZE="1">
				<OPTION <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP
				<OPTION <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES 
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Pre-Shared Key</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey" SIZE="48" MAXLENGTH="63" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck()"><font color="#000000">(8~63 characters)</font>
		</td>
	</tr>
	<tr>
		
	<% elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSK"  then %>
		<td height="30" class="title-main"><font color="#FFFFFF">WPA-PSK</font></td>
		<td class="black">&nbsp;</td>
		<td><hr noshade></td>
		<td><hr noshade></td>
		<td><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encryption</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection" SIZE="1">
				<OPTION <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%>>TKIP
				<OPTION <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if%>>AES 
		</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Pre-Shared Key</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey" SIZE="48" MAXLENGTH="63" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck()"><font color="#000000">(8~63 characters)</font>
		</td>
	</tr>
	<tr>
	<%End If%>

		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Wireless MAC Address Filter</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Active</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="WLAN_FltActive" VALUE="1" <%If tcWebApi_get("WLan_Entry","AccessPolicy","h") <> "0" then asp_Write("checked") end if%> ><font color="#000000">Activated</font>
			<INPUT TYPE="RADIO" NAME="WLAN_FltActive" VALUE="0" <%If tcWebApi_get("WLan_Entry","AccessPolicy","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","AccessPolicy","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">Deactivated</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Action</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="WLAN_FltAction" SIZE="1">
				<OPTION value="1" <% if tcWebApi_get("WLan_Entry","AccessPolicy","h") = "1" then asp_Write("selected") end if %>>Allow
				<OPTION value="2" <% if tcWebApi_get("WLan_Entry","AccessPolicy","h") = "2" then asp_Write("selected") end if %>>Deny
			</SELECT>
			<font color="#000000">the follow Wireless LAN station(s) association.</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #1	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
 		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC1" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC0","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC0","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #2	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
 		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC2" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC1","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC1","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #3	</div></td>
		<td class="tabdata"><div align="center">:</div></td>
 		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC3" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC2","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC2","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #4	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC4" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC3","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC3","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #5	</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC5" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC4","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC4","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #6	</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC6" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC5","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC5","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #7	</div></td>
    <td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANFLT_MAC7" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_Entry","WLan_MAC6","h") <> "N/A" then tcWebApi_get("WLan_Entry","WLan_MAC6","s") end if%>" onBlur="doMACcheck(this)">
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align="right">Mac Address #8	</div></td>
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
			<INPUT TYPE="SUBMIT" NAME="BUTTON" VALUE="SAVE" onClick="return doSave();">
			<INPUT TYPE="BUTTON" NAME="CancelBtn" VALUE="CANCEL" onClick="javascript:window.location='home_wireless.asp'">
			<INPUT TYPE="HIDDEN" NAME="CountryChange" VALUE="0">
		</td>
	</tr>
</table>
</div>
</form>
<%End If%>
</body>
</html>
