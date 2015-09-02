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
	If Request_Form("StationNum") <> "" then
		TCWebApi_set("WLan_Entry","MaxStaNum","StationNum")
	End if
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
	If Request_Form("isPerSSIDSupport") = "1" then
	TCWebApi_set("WLan_Entry","EnableSSID","ESSID_Enable_Selection")
	end if
	'TCWebApi_set("WLan_Entry","WMM","WMM_Selection")
	if Request_form("WLAN_FltActive") = "0" then
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltActive")
	else
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltAction")
	end if
	if Request_Form("isWPSSupported") = "1" then
	if Request_Form("SSID_INDEX") = "0" then
		'TCWebApi_set("WLan_Entry","WPS","UseWPS_Selection")
		if Request_Form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan_Entry","WPSConfMode","WpsConfModeAll")
		else
			TCWebApi_set("WLan_Entry","WPSConfMode","WpsConfModeNone")
		end if
		if Request_form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan_Entry","WPSMode","WPSMode_Selection")
			if Request_form("WPSMode_Selection") = "0" then
				TCWebApi_set("WLan_Entry","enrolleePinCode","WPSEnrolleePINCode")
			end if
		end if
	end if
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
	elseif Request_Form("Countries_Channels") = "COLUMBIA" then
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
	TCWebApi_set("WLan_Entry","AuthMode","WEP_Selection")	
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
	If Request_Form("StationNum") <> "" then
		TCWebApi_set("WLan_Entry","MaxStaNum","StationNum")
	End if
	TCWebApi_set("WLan_Common","11nMode","Is11nMode")	
	TCWebApi_set("WLan_Entry","SSID","ESSID")
	TCWebApi_set("WLan_Common","BeaconPeriod","BeaconInterval")	
	TCWebApi_set("WLan_Entry","HideSSID","ESSID_HIDE_Selection")
	If Request_Form("isPerSSIDSupport") = "1" then
	TCWebApi_set("WLan_Entry","EnableSSID","ESSID_Enable_Selection")
	end if
	'TCWebApi_set("WLan_Entry","WMM","WMM_Selection")


	if Request_Form("isDot1XSupported")  = "1" then
		if Request_Form("WEP_Selection")  = "Radius-WEP64" then
			TCWebApi_set("WLan_Common","session_timeout_interval","radiusSVR_ReAuthInter1")
			TCWebApi_set("WLan_Entry","RADIUS_Server","radiusSVR_IP1")
			TCWebApi_set("WLan_Entry","RADIUS_Port","radiusSVR_Port1")
			TCWebApi_set("WLan_Entry","RADIUS_Key","radiusSVR_Key1")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Server","bakradiusSVR_IP1")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Port","bakradiusSVR_Port1")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Key","bakradiusSVR_Key1")
			end if
		elseif Request_Form("WEP_Selection")  = "Radius-WEP128"  then
			TCWebApi_set("WLan_Common","session_timeout_interval","radiusSVR_ReAuthInter2")
			TCWebApi_set("WLan_Entry","RADIUS_Server","radiusSVR_IP2")
			TCWebApi_set("WLan_Entry","RADIUS_Port","radiusSVR_Port2")
			TCWebApi_set("WLan_Entry","RADIUS_Key","radiusSVR_Key2")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Server","bakradiusSVR_IP2")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Port","bakradiusSVR_Port2")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Key","bakradiusSVR_Key2")
			end if
		elseif Request_Form("WEP_Selection")  = "WPA"  then
			TCWebApi_set("WLan_Common","session_timeout_interval","radiusSVR_ReAuthInter3")
			TCWebApi_set("WLan_Entry","RekeyInterval","WPARekeyInter3")			
			TCWebApi_set("WLan_Entry","RADIUS_Server","radiusSVR_IP3")
			TCWebApi_set("WLan_Entry","RADIUS_Port","radiusSVR_Port3")
			TCWebApi_set("WLan_Entry","RADIUS_Key","radiusSVR_Key3")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Server","bakradiusSVR_IP3")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Port","bakradiusSVR_Port3")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Key","bakradiusSVR_Key3")
			end if
		elseif Request_Form("WEP_Selection")  = "WPA2"  then
			TCWebApi_set("WLan_Common","session_timeout_interval","radiusSVR_ReAuthInter4")
			TCWebApi_set("WLan_Entry","RekeyInterval","WPARekeyInter4")
			TCWebApi_set("WLan_Entry","RADIUS_Server","radiusSVR_IP4")
			TCWebApi_set("WLan_Entry","RADIUS_Port","radiusSVR_Port4")
			TCWebApi_set("WLan_Entry","RADIUS_Key","radiusSVR_Key4")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Server","bakradiusSVR_IP4")				
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Port","bakradiusSVR_Port4")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Key","bakradiusSVR_Key4")
			end if
		elseif Request_Form("WEP_Selection")  = "WPA1WPA2"  then
			TCWebApi_set("WLan_Common","session_timeout_interval","radiusSVR_ReAuthInter5")
			TCWebApi_set("WLan_Entry","RekeyInterval","WPARekeyInter5")
			TCWebApi_set("WLan_Entry","RADIUS_Server","radiusSVR_IP5")
			TCWebApi_set("WLan_Entry","RADIUS_Port","radiusSVR_Port5")
			TCWebApi_set("WLan_Entry","RADIUS_Key","radiusSVR_Key5")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Server","bakradiusSVR_IP5")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Port","bakradiusSVR_Port5")
				TCWebApi_set("WLan_Entry","BAK_RADIUS_Key","bakradiusSVR_Key5")
			end if
		end if	
	end if	
	if Request_form("WLAN_FltActive") = "0" then
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltActive")
	else
		TCWebApi_set("WLan_Entry","AccessPolicy","WLAN_FltAction")
	end if
	if Request_Form("isWPSSupported") = "1" then	
	if Request_Form("SSID_INDEX") = "0" then
		'TCWebApi_set("WLan_Entry","WPS","UseWPS_Selection")
		if Request_Form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan_Entry","WPSConfMode","WpsConfModeAll")
		else
			TCWebApi_set("WLan_Entry","WPSConfMode","WpsConfModeNone")
		end if
		if Request_form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan_Entry","WPSMode","WPSMode_Selection")
			if Request_form("WPSMode_Selection") = "0" then
				TCWebApi_set("WLan_Entry","enrolleePinCode","WPSEnrolleePINCode")
			end if
		end if
	end if	
	end if		
	if Request_Form("isWDSSupported") = "1" then
		TCWebApi_set("WLan_WDS","WdsEnable","WLAN_WDS_Active")
		if Request_form("WLAN_WDS_Active") = "1" then
			if Request_Form("WEP_Selection") = "OPEN" then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_NONE")
			elseif Request_Form("WEP_Selection") = "WEP-64Bits"  then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "WEP-128Bits" then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "Radius-WEP64" then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "Radius-WEP128" then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "WPAPSK" then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA2PSK" then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPAPSKWPA2PSK"  then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA"  then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA2"  then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA1WPA2"  then
				TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan_WDS","WdsKey","WDS_Key")
			end if
				
			TCWebApi_set("WLan_WDS","Wds_MAC0","WLANWDS_PEER_MAC1")
			TCWebApi_set("WLan_WDS","Wds_MAC1","WLANWDS_PEER_MAC2")
			TCWebApi_set("WLan_WDS","Wds_MAC2","WLANWDS_PEER_MAC3")
			TCWebApi_set("WLan_WDS","Wds_MAC3","WLANWDS_PEER_MAC4")
		end if
	end if			
	TCWebApi_set("WLan_Entry","WLan_MAC0","WLANFLT_MAC1")	
	TCWebApi_set("WLan_Entry","WLan_MAC1","WLANFLT_MAC2")	
	TCWebApi_set("WLan_Entry","WLan_MAC2","WLANFLT_MAC3")		
	TCWebApi_set("WLan_Entry","WLan_MAC3","WLANFLT_MAC4")		
	TCWebApi_set("WLan_Entry","WLan_MAC4","WLANFLT_MAC5")	
	TCWebApi_set("WLan_Entry","WLan_MAC5","WLANFLT_MAC6")	
	TCWebApi_set("WLan_Entry","WLan_MAC6","WLANFLT_MAC7")		
	TCWebApi_set("WLan_Entry","WLan_MAC7","WLANFLT_MAC8")
	'TCWebApi_set("WLan_Entry","WEPAuthType","WEP_TypeSelection")
	'TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey")	
	'TCWebApi_set("WLan_Entry","Key1Str","WEP_Key1")	
	'TCWebApi_set("WLan_Entry","Key2Str","WEP_Key2")		
	'TCWebApi_set("WLan_Entry","Key3Str","WEP_Key3")		
	'TCWebApi_set("WLan_Entry","Key4Str","WEP_Key4")	
	'TCWebApi_set("WLan_Entry","AuthMode","WEP_Selection")
	'TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection")	
	'TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey")
	'TCWebApi_set("WLan_Entry","RekeyInterval","keyRenewalInterval")	
	if Request_Form("WEP_Selection") = "WEP-64Bits"  then
		TCWebApi_set("WLan_Entry","WEPAuthType","WEP_TypeSelection1")
		TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey3")
		TCWebApi_set("WLan_Entry","Key1Str","WEP_Key13")	
		TCWebApi_set("WLan_Entry","Key2Str","WEP_Key23")		
		TCWebApi_set("WLan_Entry","Key3Str","WEP_Key33")		
		TCWebApi_set("WLan_Entry","Key4Str","WEP_Key43")
	elseif Request_Form("WEP_Selection") = "WEP-128Bits" then
		TCWebApi_set("WLan_Entry","WEPAuthType","WEP_TypeSelection2")
		TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey4")
		TCWebApi_set("WLan_Entry","Key1Str","WEP_Key14")	
		TCWebApi_set("WLan_Entry","Key2Str","WEP_Key24")		
		TCWebApi_set("WLan_Entry","Key3Str","WEP_Key34")		
		TCWebApi_set("WLan_Entry","Key4Str","WEP_Key44")
	elseif Request_Form("WEP_Selection") = "Radius-WEP64" then
		TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey1")
		TCWebApi_set("WLan_Entry","Key1Str","WEP_Key11")	
		TCWebApi_set("WLan_Entry","Key2Str","WEP_Key21")		
		TCWebApi_set("WLan_Entry","Key3Str","WEP_Key31")		
		TCWebApi_set("WLan_Entry","Key4Str","WEP_Key41")				
	elseif Request_Form("WEP_Selection") = "Radius-WEP128" then
		TCWebApi_set("WLan_Entry","DefaultKeyID","DefWEPKey2")
		TCWebApi_set("WLan_Entry","Key1Str","WEP_Key12")	
		TCWebApi_set("WLan_Entry","Key2Str","WEP_Key22")		
		TCWebApi_set("WLan_Entry","Key3Str","WEP_Key32")		
		TCWebApi_set("WLan_Entry","Key4Str","WEP_Key42")
	elseif Request_Form("WEP_Selection") = "WPAPSK" then
		TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey2")
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection5")
		TCWebApi_set("WLan_Entry","RekeyInterval","keyRenewalInterval2")
	elseif Request_Form("WEP_Selection") = "WPA2PSK" then
		TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey1")
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection4")
		TCWebApi_set("WLan_Entry","RekeyInterval","keyRenewalInterval1")
	elseif Request_Form("WEP_Selection") = "WPAPSKWPA2PSK"  then
		TCWebApi_set("WLan_Entry","WPAPSK","PreSharedKey3")
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection6")
		TCWebApi_set("WLan_Entry","RekeyInterval","keyRenewalInterval3")
	elseif Request_Form("WEP_Selection") = "WPA"  then
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection1")
	elseif Request_Form("WEP_Selection") = "WPA2"  then
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection2")
	elseif Request_Form("WEP_Selection") = "WPA1WPA2"  then
		TCWebApi_set("WLan_Entry","EncrypType","TKIP_Selection3")
	end if
	
	if Request_Form("isWPSSupported") = "1" then
	TCWebApi_set("WLan_Entry","WPSConfStatus","WPSConfigured")
	end if			
	tcWebApi_commit("WLan_Entry")	
End If
if Request_Form("isWDSSupported") = "1" then
	If Request_Form("wlanWEPFlag")="4" Then
		TCWebApi_set("WLan_WDS","WdsEncrypType","WDS_EncrypType_Selection")
	End If
End If
if Request_Form("isWPSSupported") = "1" then
If Request_Form("WpsStart")="1" Then
	TCWebApi_set("Info_WLan","WPSActiveStatus","WpsStart")
	TCWebApi_set("WLan_Entry","WPSConfMode","WpsConfModeAll")
	if Request_form("WPSMode_Selection") = "0" then
		TCWebApi_set("WLan_Entry","enrolleePinCode","WPSEnrolleePINCode")
	end if
	TCWebApi_set("WLan_Entry","WPSMode","WPSMode_Selection")
	'TCWebApi_set("WLan_Entry","WPSConfStatus","WPSConfigured")
	tcWebApi_commit("WLan_Entry")
End If
If Request_Form("WpsOOB")="1" Then
	TCWebApi_set("Info_WLan","WPSOOBActive","WpsOOB")
	tcWebApi_commit("WLan_Entry")
end if

If Request_Form("WpsGenerate") = "1" then
	TCWebApi_set("Info_WLan","WPSGenPinCode","WpsGenerate")
	TCWebApi_commit("WLan_Entry")
end if
end if
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
<script language="JavaScript" type="text/javascript" src="/ip_new.js"></script>
<script language="JavaScript">
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
var wpsenable;
<%end if%>
var wepidx;
var WEPSelectIndex;
function doCheckWepSelectIndex()
{
	var wlan=document.WLAN;
	<%if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
	WEPSelectIndex = wlan.WEP_Selection.selectedIndex + 1;
	//alert("WEPSelectIndex = "+WEPSelectIndex);
	<%else%>
	WEPSelectIndex = wlan.WEP_Selection.selectedIndex;
	<%end if%>
	return true;	
}

<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function doStartWPS(){
	if((document.WLAN.WPSMode_Selection[0].checked)&&(document.WLAN.isInWPSing.value==0))
	{
	 	var pincode = document.WLAN.WPSEnrolleePINCode;
        var len = pincode.value.length;
	if(doPINCodeCheck(pincode) == false)
	{
		return ;
	}
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") <> "1"  then %>
	<%if tcWebApi_get("WLan_Entry", "WPSConfStatus", "h") = "2" then %>
        if(len <= 0)
	{
	    	alert("WPS PIN code couldn't be null!");
			return;
	}
    <%end if%>
	<%end if%>
	
	}

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

function doGenerate(){
	document.WLAN.WpsGenerate.value = "1";
	document.WLAN.submit();
}
<%end if%>

function doWEPTypeChange(){

}
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
	var vCountryName = document.WLAN.Countries_Channels.value;
	var ctlCountryRegion = document.WLAN.hCountryRegion;
	var ctlCountryRegion0 = document.WLAN.CountryRegion0;
	var ctlCountryRegion1 = document.WLAN.CountryRegion1;
	if(vCountryName == "CANADA")
		ctlCountryRegion.value = ctlCountryRegion0.value;
	else if (vCountryName == "COLUMBIA")
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
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function dowpscheck(){
//modified by fred to support WPS2.0

	var wlan=document.WLAN;
if(wlan.SSID_INDEX.value==0){	
	if(wlan.UseWPS_Selection[0].checked == true){

	//check if WscV2Supported
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") = "1"  then %>

		if (wlan.ESSID_HIDE_Selection[1].checked == true){
			alert("SSID broadcast must be enabled if you want to use WPS, in other cases you need to disable WPS firstly!");
		  wlan.UseWPS_Selection[1].checked = true;
		  return 0;
		}
		if (WEPSelectIndex != 0 && WEPSelectIndex != 3 && WEPSelectIndex != 4 && WEPSelectIndex != 5 && WEPSelectIndex != 8 && WEPSelectIndex != 9 && WEPSelectIndex != 10)
		{
			alert("The authentication type must be one of \"OPEN,WPA,WPA2,WPAPSK,WPA2PSK,WPAPSK/WPA2PSK\" if you want to use WPS, in other cases you need to disable WPS firstly!");
			wlan.WEP_Selection.selectedIndex = wepidx;
					
		if(wpsenable)
		{
			wlan.UseWPS_Selection[0].checked = true;
		}else{
			wlan.UseWPS_Selection[1].checked = true;
		}
			return 0;
		}
		
		if(((WEPSelectIndex == 3) && (wlan.TKIP_Selection5.value== "TKIP"))||((WEPSelectIndex==4) && (wlan.TKIP_Selection4.value== "TKIP"))|| ((WEPSelectIndex==5) && (wlan.TKIP_Selection6.value== "TKIP")))
		{//WPAPSK ,WPA2PSK and WPAPSKWPA2PSK with TKIP
			
			
			alert("Encryption type must not be TKIP if you want to use WPS, in other cases you need to disable WPS firstly!");
			wlan.UseWPS_Selection[1].checked = true;
			return 0;
		}
		else if(((WEPSelectIndex == 8) && (wlan.TKIP_Selection1.value== "TKIP"))||((WEPSelectIndex==9) && (wlan.TKIP_Selection2.value== "TKIP"))|| ((WEPSelectIndex==10) && (wlan.TKIP_Selection3.value== "TKIP")))
		{	//WPA,WPA2 and WPAWPA2 with TKIP
			alert("Encryption type must not be TKIP if you want to use WPS, in other cases you need to disable WPS firstly!");
			wlan.UseWPS_Selection[1].checked = true;
			return 0;
		}

		return 1;
	<%end if%>

	//do simple check if only WPS 1.0 supported, use original check code in 1.0
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") <> "1"  then %>
	//WEPSelectIndex 1=WEP64,2=WEP128,3=Radius-WEP64,4=Radius-WEP128
	if(WEPSelectIndex == 1 || WEPSelectIndex == 2 || WEPSelectIndex == 6 || WEPSelectIndex == 7){
		alert("We should not use WEP when WPS function turned on!");
		if(wpsenable){
			wlan.UseWPS_Selection[0].checked = true;
		}else{
			wlan.UseWPS_Selection[1].checked = true;
		}
		//wlan.WEP_Selection.selectedIndex = wepidx;
		return 0;
	}else{
		return 1;
	}
	
	<%end if%>

	}
	else{
		return 1;
	}
}else{
	return 1;
}
}
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function doBroadcastSSIDChange(){

	//check if WscV2Supported
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") = "1"  then %>
	
	var wlan=document.WLAN;
	
	if((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) && (wlan.ESSID_HIDE_Selection[1].checked == true))
	{
			var rv = confirm("WPS will be disabled!!");
			if (rv == false)
			{
			wlan.ESSID_HIDE_Selection[0].checked = true;
				wlan.ESSID_HIDE_Selection[1].checked = false;
			}
			else
			{
				wlan.ESSID_HIDE_Selection[0].checked = false;
				wlan.ESSID_HIDE_Selection[1].checked = true;
				wlan.UseWPS_Selection[0].checked = false;
				wlan.UseWPS_Selection[1].checked = true;
			}
	}
        doLoad();
	<%end if%>
	
	return 1;
	
}

function doEncryptionChange(object){

	//check if WscV2Supported
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") = "1"  then %>
	
	var wlan=document.WLAN;
				
	if((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) && (object.value == "TKIP"))
	{

			var rv = confirm("WPS will be disabled!!");
			if (rv == false)
			{
			object.value="AES";
			}
			else
			{
				object.value = "TKIP";
				wlan.UseWPS_Selection[0].checked = false;
				wlan.UseWPS_Selection[1].checked = true;
			}

	}
	doLoad();
	<%end if%>
	return 1;
}
<%end if%>

function doWEPChange(){
	doCheckWepSelectIndex();
	var wlan=document.WLAN;
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") = "1"  then %>
	if ((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) && (WEPSelectIndex != 0 && WEPSelectIndex != 3 && WEPSelectIndex != 4 && WEPSelectIndex != 5 && WEPSelectIndex != 8 && WEPSelectIndex != 9 && WEPSelectIndex != 10))
	{
		var rv = confirm("WPS will be disabled!!");
		if (rv == false)
		{
			wlan.WEP_Selection.selectedIndex = wepidx;
		}
		else
		{
			wlan.UseWPS_Selection[0].checked = false;
			wlan.UseWPS_Selection[1].checked = true;
		}			
	}
	<%end if%>

	//do simple check if only WPS 1.0 supported, use original check code in 1.0
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") <> "1"  then %>
	if((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) &&(WEPSelectIndex == 1 || WEPSelectIndex == 2 || WEPSelectIndex == 6 || WEPSelectIndex == 7))
	{
		alert("We should not use WEP when WPS function turned on!");
		wlan.WEP_Selection.selectedIndex = wepidx;
	}
	<%end if%>
<%end if%>	
		if(WEPSelectIndex == 0)
		{
			var rv = confirm("Your network will be set to OPEN without security setting, we strongly suggest you choose WPAPSK or WPA2PSK encryption!!");
			if (rv == false)
			{
				wlan.WEP_Selection.selectedIndex = wepidx;
			}
		}

		document.WLAN.wlanWEPFlag.value = 1;
		
		doLoad();
}

function doWEPChange2(){
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	if(dowpscheck()){
<%end if%>
		document.WLAN.wlanWEPFlag.value = 1;
		//if(document.WLAN.WEP_Selection.selectedIndex != 9){
		if(WEPSelectIndex != 9){
		<%if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
			document.WLAN.WEP_Selection.selectedIndex = 8;
		<%else%>
			document.WLAN.WEP_Selection.selectedIndex = 9;
		<%end if%>
			document.WLAN.submit();
		}
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	}
<%end if%>
}
function doWDSEncrypTypeChange(){
		document.WLAN.wlanWEPFlag.value = 4;
		//document.WLAN.submit();
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
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function doWPSUseChange(){
	if(dowpscheck()){
		document.WLAN.wlanWEPFlag.value = 1;
		//document.WLAN.submit();
		RefreshPage();
	        doLoad(); //added by fredli
	}
}

function doWPSModeChange(){
	document.WLAN.wlanWEPFlag.value = 1;
	document.WLAN.submit();
}
<%end if%>
function wpapskCheck(object) {
	var keyvalue=object.value;
	var wpapsklen=object.value.length;
	
   	if(wpapsklen >= 8 && wpapsklen < 64) {
    	if(keyvalue.match(/[^\x00-\xff]/g))
   	    {
//			alert("wpapsk Key should be between 8 and 63 ASCII characters(except \',\') or 64 Hex string.");
			alert("Pre-Shared Key should be between 8 and 63 ASCII characters(except \',\') or 64 Hex string.");
			return true;
   	    } else {
   	        for ( i = 0; i < wpapsklen; i++)
   	        if (keyvalue.charAt(i) == ','){
//			   alert("wpapsk Key should be between 8 and 63 ASCII characters(except \',\') or 64 Hex string.");
			   alert("Pre-Shared Key should be between 8 and 63 ASCII characters(except \',\') or 64 Hex string.");
			   return true;
   	        }
   	    }
	}else if(wpapsklen==64){
		for(i=0;i<64;i++){
			var c=keyvalue.charAt(i);
			if(doHexCheck(c)<0){
//				alert('wpapsk Key Hex value error!');
				alert('Pre-Shared Key Hex value error!');
				return true;
			}
		}
	}else {
//    	alert('wpapsk Key length error!');
    	alert('Pre-Shared Key length error!');
		return true;
	}			
	return false;
}

function WDSKeyCheck(object) {
	var keyvalue=object.value;
	var wdskeylen=object.value.length;
	
   	if(wdskeylen < 8) {
    	alert('WDS Key length error!');
		return true;
	}else if(wdskeylen==64){
		for(i=0;i<64;i++){
			var c=keyvalue.charAt(i);
			if(doHexCheck(c)<0){
				alert('WDS Key Hex value error!');
				return true;
			}
		}
	}else if(wdskeylen > 64) {
    	alert('WDS Key length error!');
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

	//if(document.WLAN.WEP_Selection.selectedIndex==1)
	if(WEPSelectIndex==1)
	{
//	        if(len==5)/*wep 64*/
//	        {
//		        for(i=0;i<len;i++)
//		        {
//			var c = object.value.charAt(i);	
//			if(doNonSympolCheck(c)==-1)
//			{
//				alert("Invalid Key Value");
//					return false;
//			}
//		}
//	}
	if(len==5)/*wep 64*/
	{
		return true;
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
	//else if(document.WLAN.WEP_Selection.selectedIndex==2)
	else if(WEPSelectIndex==2)
	{
//		if(len==13)/*wep 128*/
//	{
//		for(i=0;i<len;i++)
//		{
//			var c = object.value.charAt(i);	
//			if(doNonSympolCheck(c)==-1)
//			{
//				alert("Invalid Key Value");
//					return false;
//			}
//		}
//	}
	if(len==13)/*wep 128*/
	{
		return true;
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
	if(document.WLAN.isDot1XSupported.value==1)
	{
		//if(document.WLAN.WEP_Selection.selectedIndex==6)
		if(WEPSelectIndex==6)
		{
//	        if(len==5)/*wep 64*/
//	        {
//		        for(i=0;i<len;i++)
//		        {
//					var c = object.value.charAt(i);	
//					if(doNonSympolCheck(c)==-1)
//					{
//						alert("Invalid Key Value");
//						return false;
//					}
//				}
//			}
			if(len==5)/*wep 64*/
			{
				return true;
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
			else
			{
				alert("Invalid Key Value");
				return false;
			}
		}
		//else if(document.WLAN.WEP_Selection.selectedIndex==7)
		else if(WEPSelectIndex==7)
		{
//			if(len==13)/*wep 128*/
//			{
//				for(i=0;i<len;i++)
//				{
//					var c = object.value.charAt(i);	
//					if(doNonSympolCheck(c)==-1)
//					{
//						alert("Invalid Key Value");
//						return false;
//					}
//				}
//			}
			if(len==13)/*wep 128*/
			{
				return true;
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
			else
			{
				alert("Invalid Key Value");
				return false;
			}
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
<% If tcWebApi_get("WebCustom_Entry","isMaxStaNumSupported","h") = "Yes" Then %>
function checkStationNum(value, limit) 
{
	if (!isNumeric(value) || parseInt(value,10) < 0 || parseInt(value,10) > parseInt(limit,10)){
    	alert("Station Number value must be between 0 and " + limit);
		return true;
	}	
	return false;
}
<%End If%>
function checkRekeyinteral(value, flag) 
{
	if (!isNumeric(value)) {
		if(flag == 1){
			alert("WPA Group Rekey Interval : Non-integer value given "); 
		}else{
			alert("Key Renewal Interval : Non-integer value given ");
		}
		return true;
	}
	if (value < 10 || value > 4194303) {
		if(flag == 1){
			alert("WPA Group Rekey Interval must be between 10 and 4194303");
		}else{
			alert("Key Renewal Interval must be between 10 and 4194303");
		}	
		return true;
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

<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") = "1"  then %>
//interface for future use
function ValidateChecksum_4(PIN)
{
		return true;
}

function ignoreSpaces(string) {
   var temp = "";
   string = '' + string;
   splitstring = string.split(" ");
   for(i = 0; i < splitstring.length; i++)
       temp += splitstring[i];
   return temp;
}

function ignoreDash(string) {
   var temp = "";
   string = '' + string;
   splitstring = string.split("-");
   for(i = 0; i < splitstring.length; i++)
       temp += splitstring[i];
   return temp;
}
<%end if%>

function doPINCodeCheck(object)
{
	var len= object.value.length;
	var ch;
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") = "1"  then %>
	if(len==9)
	{
		
		acutal_value=0;
		ch_4= object.value.charAt(4);
		if( ch_4 != ' ' & ch_4  != '-')
		{
				alert("WPS PIN code format error!");
				return false;
		}
		object.value = ignoreSpaces(object.value);
		object.value = ignoreDash(object.value);
		
		if (ValidateChecksum(Number(object.value)) == false)
		{
			alert("WPS PIN code checksum error!");
			return false;
		}
                return true;
	}

	if(len==8)
	{
		for( i=0; i < len; i++)
		{
			ch= object.value.charAt(i);
			if( ch > '9' || ch < '0')
			{
				alert("WPS PIN code must be 8 digits!");
				return false;
			}
		}
		if (ValidateChecksum(Number(object.value)) == false){
			alert("WPS PIN code checksum error!");
			return false;
		}
		return true;
	}
	


	if(len==4)
	{
		for( i=0; i < len; i++)
		{
			ch= object.value.charAt(i);
			if( ch > '9' || ch < '0')
			{
				alert("WPS PIN code must be digits!");
				return false;
			}
		}
		if (ValidateChecksum_4(Number(object.value)) == false)
		{
			alert("WPS PIN code checksum error!");
			return false;
		}
		return true;
	}
	
	if(len==0)
	{
		return true;
	}

	alert("WPS PIN code format error!");
	return false;
	<%end if%>

	//simple check for WPS 1.0
	<%if tcWebApi_get("WLan_Entry0","WscV2Support","h") <> "1"  then %>
	if (len > 0)
	{
		if(len < 8)
		{
			alert("WPS PIN code must be 8 digits!");
			return false;
		}
		for( i=0; i < len; i++)
		{
			ch= object.value.charAt(i);
			if( ch > '9' || ch < '0')
			{
				alert("WPS PIN code must be 8 digits!");
				return false;
			}
		}
		if (ValidateChecksum(Number(object.value)) == false){
			alert("WPS PIN code checksum error!");
			return false;
		}
		return true;
	}
        return true;
	<%end if%>
}

<%end if%>
function doSave(){
	<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	if(document.WLAN.SSID_INDEX.value==0){
        if(document.WLAN.WPSMode_Selection[0].checked)
        {
			var pincode = document.WLAN.WPSEnrolleePINCode;
			if((doPINCodeCheck(pincode) == false))
			{
				   return false;
			}
        }
	}
	<%end if%>
	<% if tcWebApi_get("Info_WLan","isbharti","h") = "Yes" then %>
	//if(0 == document.WLAN.WEP_Selection.selectedIndex)
	if(0 == WEPSelectIndex)	
	{
		return false;
	}
	<%end if%>
	if(checkBeacon(document.WLAN.BeaconInterval.value) ||
		checkRTS(document.WLAN.RTSThreshold.value) ||
		checkFrag(document.WLAN.FragmentThreshold.value) ||
		checkDTIM(document.WLAN.DTIM.value)
	<% If tcWebApi_get("WebCustom_Entry","isMaxStaNumSupported","h") = "Yes" Then %>
		|| checkStationNum(document.WLAN.StationNum.value, document.WLAN.maxStaNum.value)
	<%End If%>
	){
		return false;
	}
		
	//if(document.WLAN.WEP_Selection.selectedIndex == 3){
	if(WEPSelectIndex == 3){ 
		document.WLAN.hRekeyMethod.value = "TIME";
		if (wpapskCheck(document.WLAN.PreSharedKey2)){
			return false;
		}
		
		if(quotationCheck(document.WLAN.PreSharedKey2, 65) ){
			return false;	 
		}
		if(checkRekeyinteral(document.WLAN.keyRenewalInterval2.value, 0)){
			return false;
		}  			
	}
	//if(document.WLAN.WEP_Selection.selectedIndex == 4){
	if(WEPSelectIndex == 4){
		document.WLAN.hRekeyMethod.value = "TIME";
		if (wpapskCheck(document.WLAN.PreSharedKey1)){
			return false;
		}
		
		if(quotationCheck(document.WLAN.PreSharedKey1, 65) ){
			return false;	 
		}
		if(checkRekeyinteral(document.WLAN.keyRenewalInterval1.value, 0)){
			return false;
		}  			
	}
	//if(document.WLAN.WEP_Selection.selectedIndex == 5){
	if(WEPSelectIndex == 5){
		document.WLAN.hRekeyMethod.value = "TIME";
		if (wpapskCheck(document.WLAN.PreSharedKey3)){
			return false;
		}
		
		if(quotationCheck(document.WLAN.PreSharedKey3, 65) ){
			return false;	 
		}
		if(checkRekeyinteral(document.WLAN.keyRenewalInterval3.value, 0)){
			return false;
		}  			
	}
      	
	//if(document.WLAN.WEP_Selection.selectedIndex == 5){
	//	document.WLAN.hRekeyMethod.value = "TIME";		
	//}
	//if(document.WLAN.WEP_Selection.selectedIndex == 1){
	if(WEPSelectIndex == 1){ 
		document.WLAN.hRekeyMethod.value = "DISABLE";
		if((!doKEYcheck(document.WLAN.WEP_Key13))||
		(!doKEYcheck(document.WLAN.WEP_Key23))||
		(!doKEYcheck(document.WLAN.WEP_Key33))||
		(!doKEYcheck(document.WLAN.WEP_Key43))){
			//alert("key check fail");
			return false;
		}
	}
	
	//if(document.WLAN.WEP_Selection.selectedIndex == 2){
	if(WEPSelectIndex == 2){ 
		document.WLAN.hRekeyMethod.value = "DISABLE";
		if((!doKEYcheck(document.WLAN.WEP_Key14))||
		(!doKEYcheck(document.WLAN.WEP_Key24))||
		(!doKEYcheck(document.WLAN.WEP_Key34))||
		(!doKEYcheck(document.WLAN.WEP_Key44))){
			//alert("key check fail");
			return false;
		}
	}
	
	if(document.WLAN.isDot1XSupported.value==1)
	{	
		//if(document.WLAN.WEP_Selection.selectedIndex == 6)
		if(WEPSelectIndex == 6) 
		{
			if((!doKEYcheck(document.WLAN.WEP_Key11))||
			(!doKEYcheck(document.WLAN.WEP_Key21))||
			(!doKEYcheck(document.WLAN.WEP_Key31))||
			(!doKEYcheck(document.WLAN.WEP_Key41)))
			{
				//alert("key check fail");
				return false;
			}
		}
		//if(document.WLAN.WEP_Selection.selectedIndex == 7)
		if(WEPSelectIndex == 7) 
		{
			if((!doKEYcheck(document.WLAN.WEP_Key12))||
			(!doKEYcheck(document.WLAN.WEP_Key22))||
			(!doKEYcheck(document.WLAN.WEP_Key32))||
			(!doKEYcheck(document.WLAN.WEP_Key42)))
			{
				//alert("key check fail");
				return false;
			}
		}
		<%if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
		var vAuthMode = document.WLAN.WEP_Selection.selectedIndex + 1;
		<%else%>
		var vAuthMode = document.WLAN.WEP_Selection.selectedIndex;
		<%end if%>
		//var vAuthMode = document.WLAN.WEP_Selection.selectedIndex;
		//if((document.WLAN.WEP_Selection.selectedIndex == 6) ||(document.WLAN.WEP_Selection.selectedIndex == 7) 
		//|| (document.WLAN.WEP_Selection.selectedIndex == 8) || (document.WLAN.WEP_Selection.selectedIndex == 9)
		//|| (document.WLAN.WEP_Selection.selectedIndex == 10))
		if((WEPSelectIndex == 6) || (WEPSelectIndex == 7) || (WEPSelectIndex == 8) || (WEPSelectIndex == 9)
		|| (WEPSelectIndex == 10))
		{
			if(document.WLAN.isDot1XEnhanceSupported.value == 0)
			{
				if(vAuthMode == 6){
					radiusip = document.WLAN.radiusSVR_IP1.value;
				}
				else if(vAuthMode == 7){
					radiusip = document.WLAN.radiusSVR_IP2.value;
				}
				else if(vAuthMode == 8){
					radiusip = document.WLAN.radiusSVR_IP3.value;
				}
				else if(vAuthMode == 9){
					radiusip = document.WLAN.radiusSVR_IP4.value;
				}
				else if(vAuthMode == 10){
					radiusip = document.WLAN.radiusSVR_IP5.value;
				}								
				
				 if(inValidIPAddr(radiusip))
				 {
					return false;
				 }
			}
			 //serverport
			//radiusport = parseInt(document.WLAN.radiusSVR_Port.value);
			if(vAuthMode == 6){
				radiusport = parseInt(document.WLAN.radiusSVR_Port1.value);
				radiuskey = document.WLAN.radiusSVR_Key1.value;
				session_timeout_interval = document.WLAN.radiusSVR_ReAuthInter1.value;
			}
			else if(vAuthMode == 7){
				radiusport = parseInt(document.WLAN.radiusSVR_Port2.value);
				radiuskey = document.WLAN.radiusSVR_Key2.value;
				session_timeout_interval = document.WLAN.radiusSVR_ReAuthInter2.value;
			}
			else if(vAuthMode == 8){
				radiusport = parseInt(document.WLAN.radiusSVR_Port3.value);
				radiuskey = document.WLAN.radiusSVR_Key3.value;
				session_timeout_interval = document.WLAN.radiusSVR_ReAuthInter3.value;
			}
			else if(vAuthMode == 9){
				radiusport = parseInt(document.WLAN.radiusSVR_Port4.value);
				radiuskey = document.WLAN.radiusSVR_Key4.value;
				session_timeout_interval = document.WLAN.radiusSVR_ReAuthInter4.value;
			}
			else if(vAuthMode == 10){
				radiusport = parseInt(document.WLAN.radiusSVR_Port5.value);
				radiuskey = document.WLAN.radiusSVR_Key5.value;
				session_timeout_interval = document.WLAN.radiusSVR_ReAuthInter5.value;
			}
			if(isNaN(radiusport) || radiusport < 0 || radiusport > 65535)
			{	
				alert("Radius Server Port number's range: 0 ~ 65535");
				return false;
			}
			
			// radius share key
			//radiuskey = document.WLAN.radiusSVR_Key.value;
			{
				if (radiuskey.length == 0)
				{
  					alert("Radius Share secret can not be empty"); 
  					return false; 
				}
			}
			//session_timeout_interval = document.WLAN.radiusSVR_ReAuthInter.value;
			{
				if (session_timeout_interval.length == 0)
				{
  					alert("Re-auth Interval can not be empty"); 
  					return false; 
				}
			}
   		 }
		//if((document.WLAN.WEP_Selection.selectedIndex == 8) //WPA
		//	|| (document.WLAN.WEP_Selection.selectedIndex == 9) //WPA2
		//	|| (document.WLAN.WEP_Selection.selectedIndex == 10)) //WPAWPA2
		if((WEPSelectIndex == 8) || (WEPSelectIndex == 9) || (WEPSelectIndex == 10))
		{
			if(vAuthMode == 8){
				WPARekeyInter = document.WLAN.WPARekeyInter3.value;
			}
			else if(vAuthMode == 9){
				WPARekeyInter = document.WLAN.WPARekeyInter4.value;
			}
			else if(vAuthMode == 10){
				WPARekeyInter = document.WLAN.WPARekeyInter5.value;
			}
			
			if(checkRekeyinteral(WPARekeyInter, 1)){
				return false;	
			}
   		 }
	}
	
	//if wds support meantime AuthMode of MAIN SSID is wpa2psk,wpapsk WPAPSKWPA2PSK , then check wds key 
	if(document.WLAN.isWDSSupported.value==1)
	{
		if(document.WLAN.WLAN_WDS_Active[0].checked == true)//if wds enable ,to check wds_key, or not to check
		{
			if(document.WLAN.isDot1XSupported.value==1)
			{
//				if((document.WLAN.WEP_Selection.selectedIndex == 3) ||
//				(document.WLAN.WEP_Selection.selectedIndex == 4) || (document.WLAN.WEP_Selection.selectedIndex == 5)
//				|| (document.WLAN.WEP_Selection.selectedIndex == 8) || (document.WLAN.WEP_Selection.selectedIndex == 9)
//				|| (document.WLAN.WEP_Selection.selectedIndex == 10))
				if((WEPSelectIndex == 3) || (WEPSelectIndex == 4) || (WEPSelectIndex == 5)
				|| (WEPSelectIndex == 8) || (WEPSelectIndex == 9) || (WEPSelectIndex == 10))
				{
					if (WDSKeyCheck(document.WLAN.WDS_Key))
					{
						return false;
					}
			
					if(quotationCheck(document.WLAN.WDS_Key, 65) )
					{
						return false;	 
					}  			
				}	
			}
			else
			{
//				if((document.WLAN.WEP_Selection.selectedIndex == 3) ||
//				(document.WLAN.WEP_Selection.selectedIndex == 4) || (document.WLAN.WEP_Selection.selectedIndex == 5))
				if((WEPSelectIndex == 3) || (WEPSelectIndex == 4) || (WEPSelectIndex == 5))
				{
					if (WDSKeyCheck(document.WLAN.WDS_Key))
					{
						return false;
					}
					if(quotationCheck(document.WLAN.WDS_Key, 65) )
					{
						return false;	 
					}  			
				}	
			}
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
	if(vAuthMode == "Radius-WEP64"){
		group = document.WLAN.DefWEPKey1;
	}
	else if(vAuthMode == "Radius-WEP128"){
		group = document.WLAN.DefWEPKey2;
	}
	else if(vAuthMode == "WEP-64Bits"){
		group = document.WLAN.DefWEPKey3;
	}
	else if(vAuthMode == "WEP-128Bits"){
		group = document.WLAN.DefWEPKey4;
	}

	for (var i=0; i<group.length; i++){
		if (group[i].checked)
		break;
	}
	
	if(vAuthMode == "Radius-WEP64"){
		switch (i)
		{
		case 0:
			curCBX = document.WLAN.WEP_Key11;
			break;
		case 1:
			curCBX = document.WLAN.WEP_Key21;
			break;
		case 2:
			curCBX = document.WLAN.WEP_Key31;
			break;
		case 3:
			curCBX = document.WLAN.WEP_Key41;
			break;
		default:
		  ;
		}
	}
	else if(vAuthMode == "Radius-WEP128"){
		switch (i)
		{
		case 0:
			curCBX = document.WLAN.WEP_Key12;
			break;
		case 1:
			curCBX = document.WLAN.WEP_Key22;
			break;
		case 2:
			curCBX = document.WLAN.WEP_Key32;
			break;
		case 3:
			curCBX = document.WLAN.WEP_Key42;
			break;
		default:
		  ;
		}
	}
	else if(vAuthMode == "WEP-64Bits"){
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
	
//	if(document.WLAN.WEP_Selection.selectedIndex==1)/*wep 64*/
	if(WEPSelectIndex==1)/*wep 64*/
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
//	else if(document.WLAN.WEP_Selection.selectedIndex==2)/*wep 128*/
	else if(WEPSelectIndex==2)/*wep 128*/
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
	//if(document.form.WEP_Selection.selectedIndex == 0){
	if(WEPSelectIndex == 0){
		document.form.WEP_Selection.focus();
	}
}

function autoWLAN_WDS_Active()
{
	if(document.WLAN.WDS_EncrypType_Selection != null){
		document.WLAN.WDS_EncrypType_Selection.disabled = false;
		document.WLAN.WDS_Key.disabled = false;
	}
	document.WLAN.WLANWDS_PEER_MAC1.disabled = false;
	document.WLAN.WLANWDS_PEER_MAC2.disabled = false;
	document.WLAN.WLANWDS_PEER_MAC3.disabled = false;
	document.WLAN.WLANWDS_PEER_MAC4.disabled = false;
}	

function autoWLAN_WDS_Deactive()
{
	if(document.WLAN.WDS_EncrypType_Selection != null){
		document.WLAN.WDS_EncrypType_Selection.disabled = true;
		document.WLAN.WDS_Key.disabled = true;
	}
	document.WLAN.WLANWDS_PEER_MAC1.disabled = true;
	document.WLAN.WLANWDS_PEER_MAC2.disabled = true;
	document.WLAN.WLANWDS_PEER_MAC3.disabled = true;
	document.WLAN.WLANWDS_PEER_MAC4.disabled = true;
}

function doLoad(){
	RefreshPage();
	doCheckWepSelectIndex();	
	if(document.WLAN.isDot1XSupported.value==1)
	{
		if(document.WLAN.isAuthenTypeSupported.value==1)
		{
			document.getElementById("WEP_Selection_div").style.display="none";
			document.getElementById("WEP_Selection_show_div").style.display="";
			doWEPChange2();
		}
		else
		{
			document.getElementById("WEP_Selection_div").style.display="";
			document.getElementById("WEP_Selection_show_div").style.display="none";
		}
	}	
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
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	if(document.WLAN.isInWPSing.value==1){
		document.WLAN.ResetOOB.disabled = true;
		document.WLAN.BUTTON.disabled = true;
		document.WLAN.CancelBtn.disabled = true;
	}

	if(document.WLAN.SSID_INDEX.value==0){
		wpsenable = document.WLAN.UseWPS_Selection[0].checked;
	}
<%end if%>
//	wepidx = document.WLAN.WEP_Selection.selectedIndex;
	wepidx = WEPSelectIndex;

	if(document.WLAN.isWDSSupported.value==1)
	{
		if(document.WLAN.WLAN_WDS_Active[0].checked == true){//if wds enable
			autoWLAN_WDS_Active();
		}else if(document.WLAN.WLAN_WDS_Active[1].checked == true){//if wds disable
			autoWLAN_WDS_Deactive();
		}
	}
	if(document.WLAN.bharti_ssid2.value==1)
		doloadSSID2();		
}

function doloadSSID2()
{
	if(document.WLAN.SSID_INDEX.selectedIndex == 1)
	{
		var j;
		var frm = document.WLAN;
		for(j = 0; j < frm.elements.length; j++)
		{	
			if(frm.elements[j].type != "hidden")		
				frm.elements[j].disabled = true;
		}
	}
	document.WLAN.SSID_INDEX.disabled = false;
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
	<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>	
	if(document.WLAN.SSID_INDEX.value==0){
		if(document.WLAN.UseWPS_Selection[0].checked == true)
			document.getElementById("WPSConfMode_1_div").style.display="";
		else
			document.getElementById("WPSConfMode_1_div").style.display="none";
	}
	<%end if%>
	var vAuthMode = document.WLAN.WEP_Selection.value;
	if(document.getElementById("Radius-WEP64_div") != null) 
		document.getElementById("Radius-WEP64_div").style.display="none";
	if(document.getElementById("Radius-WEP128_div") != null)
		document.getElementById("Radius-WEP128_div").style.display="none";
	if(document.getElementById("WPA_div") != null)
		document.getElementById("WPA_div").style.display="none";
	if(document.getElementById("WPA2_div") != null)
		document.getElementById("WPA2_div").style.display="none";
	if(document.getElementById("WPA1WPA2_div") != null)
		document.getElementById("WPA1WPA2_div").style.display="none";
		
	if(vAuthMode == "Radius-WEP64"){
		if(document.getElementById("Radius-WEP64_div") != null) 
			document.getElementById("Radius-WEP64_div").style.display="";
	}
	if(vAuthMode == "Radius-WEP128"){
		if(document.getElementById("Radius-WEP128_div") != null)
			document.getElementById("Radius-WEP128_div").style.display="";
	}
	if(vAuthMode == "WPA"){
		if(document.getElementById("WPA_div") != null)
			document.getElementById("WPA_div").style.display="";
	}
	if(vAuthMode == "WPA2"){
		if(document.getElementById("WPA2_div") != null)
			document.getElementById("WPA2_div").style.display="";
	}
	if(vAuthMode == "WPA1WPA2"){
		if(document.getElementById("WPA1WPA2_div") != null)
			document.getElementById("WPA1WPA2_div").style.display="";
	}
		
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
	
	if(document.getElementById("else_div") != null)
		document.getElementById("else_div").style.display="none";
	if(vAuthMode == "OPEN")
		;
	else if(vAuthMode == "WEP-64Bits")
		;
	else if(vAuthMode == "WEP-128Bits")
		;
	else if(vAuthMode == "Radius-WEP64")
		;
	else if(vAuthMode == "Radius-WEP128")
		;
	else if(document.getElementById("else_div") != null)
		document.getElementById("else_div").style.display="";
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
<INPUT TYPE="HIDDEN" NAME="isWPSSupported" value="<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT TYPE="HIDDEN" NAME="WscV2Support" value="<% if tcWebApi_get("WLan_Entry0","WscV2Support","h") = "1" then asp_write("1") else asp_write("0") end if %>">
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
<%if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
<INPUT TYPE="HIDDEN" NAME="WPSConfigured" value="2">
<INPUT TYPE="HIDDEN" NAME="WpsConfModeAll" value="7">
<INPUT TYPE="HIDDEN" NAME="WpsConfModeNone" value="0">
<INPUT TYPE="HIDDEN" NAME="WpsStart" value="0">
<INPUT TYPE="HIDDEN" NAME="WpsOOB" value="0">
<INPUT TYPE="HIDDEN" NAME="isInWPSing" value="<%tcWebApi_get("Info_WLan","wlanWPStimerRunning","s")%>">
<INPUT TYPE="HIDDEN" NAME="WpsGenerate" value="0">
<%end if%>
<INPUT TYPE="HIDDEN" NAME="Is11nMode"  value="<%tcWebApi_get("WLan_Common","11nMode","s")%>">
<INPUT TYPE="HIDDEN" NAME="ExtChannFlag"  value="<%tcWebApi_get("WLan_Common","HT_EXTCHA","s")%>">
<INPUT type="HIDDEN" NAME="isAuthenTypeSupported" value="<% if tcWebApi_get("Info_WLan","isAuthenTypeSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT type="HIDDEN" NAME="isDot1XSupported" value="<% if tcWebApi_get("Info_WLan","isDot1XSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT type="HIDDEN" NAME="isDot1XEnhanceSupported" value="<% if tcWebApi_get("Info_WLan","isDot1XEnhanceSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<!--
<INPUT TYPE="HIDDEN" NAME="wlan_VC" value="<%tcWebApi_get("WebCurSet_Entry","wlan_id","s")%>">
-->
<!--<INPUT TYPE="HIDDEN" NAME="WpsStart" value="<%tcWebApi_get("Info_WLan","WPSActiveStatus","s")%>">
-->
<INPUT TYPE="HIDDEN" NAME="BssidNum" value="<%tcWebApi_get("WLan_Common","BssidNum","s")%>">
<INPUT TYPE="HIDDEN" NAME="CountryName" value="<%tcWebApi_get("WLan_Common","Country","s")%>">
<INPUT TYPE="HIDDEN" NAME="hCountryRegion" value="<%tcWebApi_get("WLan_Common","CountryRegion","s")%>">
<INPUT TYPE="HIDDEN" NAME="hRekeyMethod" value="DISABLE">
<INPUT type="HIDDEN" NAME="isWDSSupported" value="<% if tcWebApi_get("Info_WLan","isWDSSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT TYPE="HIDDEN" NAME="WDS_EncrypType_NONE" value="NONE">
<INPUT TYPE="HIDDEN" NAME="WDS_EncrypType_WEP" value="WEP">
<input type="HIDDEN" name="bharti_ssid2" value="<% if tcWebApi_get("Info_WLan","isbharti","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<INPUT TYPE="HIDDEN" NAME="isPerSSIDSupport" value="<% if tcWebApi_get("WebCustom_Entry","isPerSSIDSupport","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
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
	countryarr[11]="BOLIVIA";
	countryarr[12]="BRAZIL";
	countryarr[13]="BRUNEI DARUSSALAM";
	countryarr[14]="BULGARIA";
	countryarr[15]="CANADA";
	countryarr[16]="CHILE";
	countryarr[17]="CHINA";
	countryarr[18]="COLUMBIA";
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
	countryarr[46]="KOREA DEMOCRATIC";
	countryarr[47]="KOREA REPUBLIC";
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
	countryarr[79]="SWEDEN";
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
        <font color="#000000">Current Channel :</font> 
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
		<td class="tabdata"><div align=right><font color="#000000">Wireless Mode</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="WirelessMode" SIZE="1" onChange="doWirelessModeChange()">
				<OPTION value="1" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "1" then asp_Write("selected") end if %>>802.11b
				<OPTION value="4" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "4" then asp_Write("selected") end if %>>802.11g
				<OPTION value="0" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "0" then asp_Write("selected") end if %>>802.11b+g
				<OPTION value="6" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "6" then asp_Write("selected") end if %>>802.11n
				<OPTION value="7" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "7" then asp_Write("selected") end if %>>802.11g+n
				<OPTION value="9" <% if tcWebApi_get("WLan_Common","WirelessMode","h") = "9" then asp_Write("selected") end if %> <%If tcWebApi_get("WLan_Common","WirelessMode","h") = "N/A" then asp_Write("checked") end if%>>802.11b+g+n
			</SELECT>
		</td>
	</tr>
	<% If tcWebApi_get("WebCustom_Entry","isMaxStaNumSupported","h") = "Yes" Then %>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Station Number</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<input type="hidden" name="maxStaNum" value="<%tcWebApi_get("Info_WLan","maxStaNumSupported","s")%>">
			<input name="StationNum" type="text" value="<%if tcWebApi_get("WLan_Entry","MaxStaNum","h") <> "N/A" then tcWebApi_get("WLan_Entry","MaxStaNum","s") else asp_Write("0") end if%>" size="7" maxlength="2" onblur="value=isNumeric(value)?value:'0';">
        (range: 0~<%tcWebApi_get("Info_WLan","maxStaNumSupported","s")%>, 0 means no limit) </td>
	</tr>
	<%End If%>
	</table>
	<div id="11nMode_1_div">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
      <td width="150" height="20" class="title-main"> <font color="#FFFFFF">11n Settings</font>
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
    Channel Bandwidth</font>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
      <td class="tabdata"> <select name="WLANChannelBandwidth" onChange="doChannelBandwidthChange();">
          <option value="0" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "0" then asp_Write("selected") end if %>>20 MHz</option>
		  <option value="1" <% if tcWebApi_get("WLan_Common","HT_BW","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Common","HT_BW","h") = "N/A" then asp_Write("selected") end if %>>20/40 MHz</option>
        </select></td>
  </tr>
	</table>
	<div id="HT_BW_1_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td width="150" class="tabdata"><div align="right"><font color="#000000">    Extension Channel</font>
    </div></td>
    <td width="10" class="tabdata"><div align="center">:</div></td>
    <td width="440" class="tabdata">
        <select name="WLANExtensionChannel" onChange="doExtChannChange();">
        <option value="0" <% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "0" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "N/A" then asp_Write("selected") end if %>>below the control channel</option>
		<option value="1" <% if tcWebApi_get("WLan_Common","HT_EXTCHA","h") = "1" then asp_Write("selected") end if %>>above the control channel</option>
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
    Guard Interval</font>
    </div></td>
    <td width="10" class="tabdata"><div align="center">:</div></td>
    <td width="440" class="tabdata">
        <select name="WLANGuardInterval">
        <option value="0" <% if tcWebApi_get("WLan_Common","HT_GI","h") = "0" then asp_Write("selected") end if %>>800 nsec</option>
		<option value="1" <% if tcWebApi_get("WLan_Common","HT_GI","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Common","HT_GI","h") = "N/A" then asp_Write("selected") end if %>>AUTO</option>
        </select>
	</td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right"><font color="#000000">
    MCS</font>
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
		<% if tcWebApi_get("WLan_Common","rt_device","h") = "3092" then %>
        <option value="8" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "8" then asp_Write("selected") end if %>>8</option>
		<option value="9" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "9" then asp_Write("selected") end if %>>9</option>
		<option value="10" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "10" then asp_Write("selected") end if %>>10</option>
		<option value="11" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "11" then asp_Write("selected") end if %>>11</option>
		<option value="12" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "12" then asp_Write("selected") end if %>>12</option>
		<option value="13" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "13" then asp_Write("selected") end if %>>13</option>
		<option value="14" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "14" then asp_Write("selected") end if %>>14</option>
		<option value="15" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "15" then asp_Write("selected") end if %>>15</option>
		<% end if %>
<% if tcWebApi_get("WLan_Common","rt_device","h") = "5392" then %>
        <option value="8" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "8" then asp_Write("selected") end if %>>8</option>
		<option value="9" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "9" then asp_Write("selected") end if %>>9</option>
		<option value="10" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "10" then asp_Write("selected") end if %>>10</option>
		<option value="11" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "11" then asp_Write("selected") end if %>>11</option>
		<option value="12" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "12" then asp_Write("selected") end if %>>12</option>
		<option value="13" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "13" then asp_Write("selected") end if %>>13</option>
		<option value="14" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "14" then asp_Write("selected") end if %>>14</option>
		<option value="15" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "15" then asp_Write("selected") end if %>>15</option>
		<% end if %>
		<option value="33" <% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "33" then asp_Write("selected") end if %><% if tcWebApi_get("WLan_Entry","HT_MCS","h") = "N/A" then asp_Write("selected") end if %>>AUTO</option>
       </select>
	</td>
  </tr>
  </table>
  </div>
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
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
	<% if tcWebApi_get("GUI_Entry0","ssidOneOnly","h") <> "support" then %>
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "1" then asp_write("selected") end if%>>2
	<%end if%>
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
	<% if tcWebApi_get("GUI_Entry0","ssidOneOnly","h") <> "support" then %>
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "1" then asp_write("selected") end if%>>2
		<OPTION value="2" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "2" then asp_write("selected") end if%>>3
	<%end if%>
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
	<% if tcWebApi_get("GUI_Entry0","ssidOneOnly","h") <> "support" then %>
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "1" then asp_write("selected") end if%>>2
		<OPTION value="2" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "2" then asp_write("selected") end if%>>3
		<OPTION value="3" <%if TCWebApi_get("WebCurSet_Entry","wlan_id","h") = "3" then asp_write("selected") end if%>>4
	<%end if%>
		</select>
		</td>
</tr>
<%end if%>
<% if tcWebApi_get("WebCustom_Entry","isPerSSIDSupport","h") = "Yes"  then %>
	<tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">PerSSID Switch</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
	<INPUT TYPE="RADIO" NAME="ESSID_Enable_Selection" VALUE="1" <%If tcWebApi_get("WLan_Entry","EnableSSID","h") = "1" then asp_Write("checked") end if%> >
        <font color="#000000">Activated</font> 
        <INPUT TYPE="RADIO" NAME="ESSID_Enable_Selection" VALUE="0" <%If tcWebApi_get("WLan_Entry","EnableSSID","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","EnableSSID","h") = "N/A" then asp_Write("checked") end if%> >
        <font color="#000000">Deactivated</font> </td>
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
			<INPUT TYPE="RADIO" NAME="ESSID_HIDE_Selection" VALUE="1" onClick="doBroadcastSSIDChange();"<%If tcWebApi_get("WLan_Entry","HideSSID","h") = "1" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","HideSSID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">No</font>
		</td>
	</tr>
	</table>
	<div id="11nMode_0_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
    <td width="150" class="light-orange"></td>
    <td width="10" class="light-orange"></td>
    <td width="150" class="tabdata"><div align=right><font color="#000000">WMM</font></div></td>
    <td width="10" class="tabdata"><div align=center>:</div></td>
    <td width="440" class="tabdata">
			<INPUT TYPE="RADIO" NAME="WMM_Selection" VALUE="1" <%If tcWebApi_get("WLan_Entry","WMM","h") = "1" then asp_Write("checked") end if%> >		<font color="#000000">Enable</font>		
			<INPUT TYPE="RADIO" NAME="WMM_Selection" VALUE="0" <%If tcWebApi_get("WLan_Entry","WMM","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","WMM","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">Disable</font>
		</td>
	</tr>
	</table>
	</div>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
  <% if tcWebApi_get("WebCurSet_Entry","wlan_id","h") = "0" then %>
  <tr>
    <td width="150" class="light-orange"></td>
    <td width="10" class="light-orange"></td>
    <td width="150" class="tabdata"><div align="right">Use WPS</div></td>
    <td width="10" class="tabdata"><div align="center">:</div></td>
    <td width="440" class="tabdata"><input name="UseWPS_Selection" VALUE="1" <%If tcWebApi_get("WLan_Entry","WPSConfMode","h") <> "0" then asp_Write("checked") end if%> onClick="doWPSUseChange();" type="radio"><font color="#000000">Yes</font>
	<input name="UseWPS_Selection" VALUE="0" <%If tcWebApi_get("WLan_Entry","WPSConfMode","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_Entry","WPSConfMode","h") = "N/A" then asp_Write("checked") end if%> onClick="doWPSUseChange();" type="radio"><font color="#000000">No</font></td>
  </tr>
	</table>
	<div id="WPSConfMode_1_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
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
    <td class="tabdata"><% tcWebApi_get("Info_WLan", "wlanSelfPinCode", "s") %>&nbsp;&nbsp;
	<input type="button" name="pin_generate" value="Generate" onClick="doGenerate()"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"> <div align="right">enrollee PIN code</div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata"><input name="WPSEnrolleePINCode" size="9" maxlength="9" value="<%If tcWebApi_get("WLan_Entry","enrolleePinCode","h") = "N/A" then asp_Write("") else TCWebApi_get("WLan_Entry","enrolleePinCode","s") end if %>" type="text"></td>
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
	</table>
	</div>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <% end if %>
<% end if %> 
  <% if tcWebApi_get("Info_WLan","isDot1XSupported","h") = "Yes" then %>
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">Authentication Type</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<div id="WEP_Selection_div">
			<SELECT NAME="WEP_Selection" SIZE="1" onChange="doWEPChange()">
			<% if tcWebApi_get("Info_WLan","isbharti","h") = "Yes" then %>
				<OPTION value="OPEN" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %>> 
			<% elseif tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
			<%else%>
				<OPTION value="OPEN" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %>>OPEN
			<%end if%>
				<OPTION value="WEP-64Bits" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-64Bits" then asp_Write("selected") end if %>>WEP-64Bits
				<OPTION value="WEP-128Bits" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-128Bits" then asp_Write("selected") end if %>>WEP-128Bits
				<OPTION value="WPAPSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSK" then asp_Write("selected") end if %>>WPAPSK
				<OPTION value="WPA2PSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2PSK" then asp_Write("selected") end if %>>WPA2PSK 
				<OPTION value="WPAPSKWPA2PSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSKWPA2PSK" then asp_Write("selected") end if %>>WPAPSKWPA2PSK 				
				<OPTION value="Radius-WEP64" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "Radius-WEP64" then asp_Write("selected") end if %>>Radius-WEP64
				<OPTION value="Radius-WEP128" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "Radius-WEP128" then asp_Write("selected") end if %>>Radius-WEP128
				<OPTION value="WPA" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA" then asp_Write("selected") end if %>>WPA
				<OPTION value="WPA2" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2" then asp_Write("selected") end if %>>WPA2 
				<OPTION value="WPA1WPA2" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA1WPA2" then asp_Write("selected") end if %>>WPA1WPA2	
			</SELECT>
			</div>
			<INPUT TYPE="HIDDEN" NAME="wlanWEPFlag" VALUE="0">
			
			<div id="WEP_Selection_show_div">
			<SELECT NAME="WEP_Selection_show" SIZE="1">
				<OPTION <% if tcWebApi_get("Info_WLan","isAuthenTypeSupported","h") = "Yes" then asp_Write("selected") elseif tcWebApi_get("Info_WLan","isAuthenTypeSupported","h") = "N/A" then asp_Write("selected") end if %>>WPA2
			</SELECT>
			</div>
			
		</td>
	</tr>
	<%else%>
		<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">Authentication Type</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<SELECT NAME="WEP_Selection" SIZE="1" onChange="doWEPChange()">
			<% if tcWebApi_get("Info_WLan","isbharti","h") <> "Yes" then %>
			<%if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>	
			<%else%>
				<OPTION <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %>>OPEN
			<%end if%>	
			<%end if%>
				<OPTION value="WEP-64Bits" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-64Bits" then asp_Write("selected") end if %>>WEP-64Bits
				<OPTION value="WEP-128Bits" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-128Bits" then asp_Write("selected") end if %>>WEP-128Bits
				<OPTION value="WPAPSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSK" then asp_Write("selected") end if %>><%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h")="Yes" then%>WPA-PSK<%else%>WPAPSK<%end if%>
				<OPTION value="WPA2PSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2PSK" then asp_Write("selected") end if %>><%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h")="Yes" then%>WPA2-PSK<%else%>WPA2PSK<%end if%>
				<OPTION value="WPAPSKWPA2PSK" <% if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSKWPA2PSK" then asp_Write("selected") end if %>><%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h")="Yes" then%>WPA-PSK/WPA2-PSK<%else%>WPAPSKWPA2PSK<%end if%> 	
			</SELECT>
			<INPUT TYPE="HIDDEN" NAME="wlanWEPFlag" VALUE="0">
		</td>
	</tr>
	<%end if%>
</table>

<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<% if tcWebApi_get("Info_WLan","isDot1XSupported","h") = "Yes" then %>
	</table>
	<div id="Radius-WEP64_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">	
	<tr>
        <td height="30" width="150" class="title-main"><font color="#FFFFFF">RADIUS</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width="10"><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr> 
        <td height="24"  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_IP1" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Port1" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Radius Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Key1" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Key","s") end if%>" size="48" maxlength="63"></td>
      </tr> 
	  <% if tcWebApi_get("Info_WLan","isDot1XEnhanceSupported","h") = "Yes" then %>
	<tr> 
        <td height="24"  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_IP1" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Port1" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Backup Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Key1" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","s") end if%>" size="48" maxlength="63"></td>
      </tr>
 	  <%end if%>
      <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
    	Re-auth Interval
		</div></td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata"><input type="text" name="radiusSVR_ReAuthInter1" value="<%if tcWebApi_get("WLan_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan_Common","session_timeout_interval","s") end if%>" size="48" maxlength="63">
        seconds 
		</td>
	  </tr>
  
	  <!--
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Network Re-auth Interval </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input type="text" name="radiusSVR_ReAuthInter" size="48" maxlength="63">
          seconds </td>
      </tr>
	  -->
	  <tr> 
        <td width="150" height="30" class="title-main"><font color="#FFFFFF">Radius-WEP64</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 64-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
        <td class="tabdata"><font color="#000000">For each key, please enter either 
          (1) 5 characters, or (2) 10 characters ranging from 
          0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 128-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
        <td class="tabdata"><font color="#000000">For each key, please enter either 
          (1) 13 characters, or (2) 26 characters ranging from 
          0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>
            <INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#1</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key11" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this);" > 
		</td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#2</div></td>
		<td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key21" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this);" > 
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
            <INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#3</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key31" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key3Str","s") end if%>"  onBlur="doKEYcheck(this);" > 
		</td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#4</div></td>
		<td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key41" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key4Str","s") end if%>"  onBlur="doKEYcheck(this);" > 
        </td>
      </tr>
	</table>
	</div>
	<div id="Radius-WEP128_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
        <td height="30" width="150" class="title-main"><font color="#FFFFFF">RADIUS</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width="10"><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr> 
        <td  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_IP2" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
		</td>
	</tr>
	<tr>                                                                                      
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Port2" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Port","s") else asp_Write("0") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Radius Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Key2" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Key","s") end if%>" size="48" maxlength="63"> 
        </td>
      </tr>
	  <% if tcWebApi_get("Info_WLan","isDot1XEnhanceSupported","h") = "Yes" then %>
	  <tr> 
        <td height="24"  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_IP2" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Port2" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Backup Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Key2" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","s") end if%>" size="48" maxlength="63"></td>
      </tr>
	  <%end if%>
	  <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
    	Re-auth Interval
		</div></td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata"><input type="text" name="radiusSVR_ReAuthInter2" value="<%if tcWebApi_get("WLan_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan_Common","session_timeout_interval","s") end if%>" size="48" maxlength="63">
        seconds 
		</td>
	  </tr>
	  <!--
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Network Re-auth Interval </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input type="text" name="radiusSVR_ReAuthInter" size="48" maxlength="63">
          seconds </td>
      </tr>
	  -->
	  <tr> 
        <td width="150" height="30" class="title-main"><font color="#FFFFFF">Radius-WEP128</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width="10" ><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right><font color="#000000">WEP 64-bits</font></div></td>
        <td class="tabdata">&nbsp;</td>
        <td class="tabdata"><font color="#000000">For each key, please enter either 
          (1) 5 characters, or (2) 10 characters ranging from 
          0~9, a, b, c, d, e, f.</font></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right><font color="#000000">WEP 128-bits</font></div></td>
        <td class="tabdata">&nbsp;</td>
        <td class="tabdata"><font color="#000000">For each key, please enter either 
          (1) 13 characters, or (2) 26 characters ranging from 
          0~9, a, b, c, d, e, f.</font></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
    <td class="tabdata"><div align=right>                                                                                                                                                                                     
            <INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#1</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key12" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this);" > 
		</td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#2</div></td>
		<td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key22" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this);" > 
		</td>
	</tr>
	<tr>                                                                                      
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
            <INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#3</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key32" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key3Str","s") end if%>"  onBlur="doKEYcheck(this);" > 
		</td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#4</div></td>
		<td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key42" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key4Str","s") end if%>"  onBlur="doKEYcheck(this);" > 
		</td>
	</tr>
	</table>
	</div>
	<div id="WPA_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>		
        <td height="30" width="150" class="title-main"><font color="#FFFFFF">RADIUS</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width="10"><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr> 
        <td  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_IP3" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Port3" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Radius Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Key3" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Key","s") end if%>" size="48" maxlength="63"> 
        </td>
      </tr>
	  <% if tcWebApi_get("Info_WLan","isDot1XEnhanceSupported","h") = "Yes" then %>
	  <tr> 
        <td height="24"  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_IP3" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Port3" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Backup Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Key3" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","s") end if%>" size="48" maxlength="63"></td>
      </tr>
	  <%end if%>
	  <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
    	Re-auth Interval
		</div></td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata"><input type="text" name="radiusSVR_ReAuthInter3" value="<%if tcWebApi_get("WLan_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan_Common","session_timeout_interval","s") end if%>" size="48" maxlength="63">
        seconds 
		</td>
	  </tr>
	  <!--
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Network Re-auth Interval </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input type="text" name="radiusSVR_ReAuthInter" size="48" maxlength="63">
          seconds </td>
      </tr>
	  -->
	  <tr> 
        <td height="30" class="title-main"><font color="#FFFFFF">WPA</font></td>
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
        <td class="tabdata"> <SELECT NAME="TKIP_Selection1" onChange="doEncryptionChange(this)" SIZE="1">
			<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES 
            <OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP 
            <OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES 
          </SELECT> </td>
      </tr>
	  <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
			WPA Group Rekey Interval
			</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" id="WPARekeyInter3" NAME="WPARekeyInter3" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 1)">
			<font color="#000000"> seconds &nbsp;&nbsp;(10 ~ 4194303)</font>
			<script language="JavaScript" type="text/JavaScript">
			var rekeystr = "<% tcWebApi_get("WLan_Entry","RekeyInterval","s") %>";
			if("N/A" == rekeystr || "" == rekeystr){
				document.getElementById('WPARekeyInter3').value = "3600";
			}
			else{
				document.getElementById('WPARekeyInter3').value = rekeystr;
			}
			</script>
		</td>
	  </tr>
	</table>
	</div>
	<div id="WPA2_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
        <td height="30" width="150" class="title-main"><font color="#FFFFFF">RADIUS</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width="10"><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr> 
        <td  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_IP4" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Port4" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Radius Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Key4" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Key","s") end if%>" size="48" maxlength="63"> 
        </td>
      </tr>
	  <% if tcWebApi_get("Info_WLan","isDot1XEnhanceSupported","h") = "Yes" then %>
	  <tr> 
        <td height="24"  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_IP4" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Port4" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Backup Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Key4" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","s") end if%>" size="48" maxlength="63"></td>
      </tr>
	  <%end if%>
	  <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
    	Re-auth Interval
		</div></td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata"><input type="text" name="radiusSVR_ReAuthInter4" value="<%if tcWebApi_get("WLan_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan_Common","session_timeout_interval","s") end if%>" size="48" maxlength="63">
        seconds 
		</td>
	  </tr>
	  <!--
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Network Re-auth Interval </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input type="text" name="radiusSVR_ReAuthInter" size="48" maxlength="63">
          seconds </td>
      </tr>
	  -->
	  	  <tr> 
        <td height="30" class="title-main"><font color="#FFFFFF">WPA2</font></td>
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
        <td class="tabdata"> <SELECT NAME="TKIP_Selection2" onChange="doEncryptionChange(this);" SIZE="1">
			<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES
            <OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP 
            <OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES 
          </SELECT> </td>
      </tr>
	  <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
			WPA Group Rekey Interval
			</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" id="WPARekeyInter4" NAME="WPARekeyInter4" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 1)">
			<font color="#000000"> seconds &nbsp;&nbsp;(10 ~ 4194303)</font>
			<script language="JavaScript" type="text/JavaScript">
				var rekeystr = "<% tcWebApi_get("WLan_Entry","RekeyInterval","s") %>";
				if("N/A" == rekeystr || "" == rekeystr){
					document.getElementById('WPARekeyInter4').value = "3600";
				}
				else{
					document.getElementById('WPARekeyInter4').value = rekeystr;
				}
			</script>
		</td>
	  </tr>
	</table>
	</div>
	<div id="WPA1WPA2_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
        <td height="30" width="150" class="title-main"><font color="#FFFFFF">RADIUS</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width="10"><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr> 
        <td  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_IP5" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Radius Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Port5" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Radius Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="radiusSVR_Key5" type="text" value="<%if tcWebApi_get("WLan_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","RADIUS_Key","s") end if%>" size="48" maxlength="63"> 
        </td>
      </tr>
	  <% if tcWebApi_get("Info_WLan","isDot1XEnhanceSupported","h") = "Yes" then %>
	  <tr> 
        <td height="24"  class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server IP </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_IP5" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Server","s") end if%>" size="20" maxlength="15"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Backup Server Port </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Port5" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Port","s") end if%>" size="8" maxlength="5"> 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right">Backup Share secret </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input name="bakradiusSVR_Key5" type="text" value="<%if tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan_Entry","BAK_RADIUS_Key","s") end if%>" size="48" maxlength="63"></td>
      </tr>
	  <%end if%>
	  <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
    	Re-auth Interval
		</div></td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata"><input type="text" name="radiusSVR_ReAuthInter5" value="<%if tcWebApi_get("WLan_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan_Common","session_timeout_interval","s") end if%>" size="48" maxlength="63">
        seconds 
		</td>
	  </tr>
	  <!--
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align="right"> Network Re-auth Interval </div></td>
        <td class="tabdata"><div align="center">:</div></td>
        <td class="tabdata"> <input type="text" name="radiusSVR_ReAuthInter" size="48" maxlength="63">
          seconds </td>
      </tr>
	  -->
	  <tr> 
        <td height="30" class="title-main"><font color="#FFFFFF">WPA1WPA2</font></td>
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
        <td class="tabdata"> <SELECT NAME="TKIP_Selection3" onChange="doEncryptionChange(this);" SIZE="1">
			<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES
            <OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP 
            <OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES 
          </SELECT> </td>
      </tr>
	  <tr>
     	<td class="light-orange">&nbsp;</td>
   		<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
			WPA Group Rekey Interval
			</div></td>
		<td class="tabdata"><div align="center">:</div></td>
		<td class="tabdata">
		<INPUT TYPE="TEXT" id="WPARekeyInter5" NAME="WPARekeyInter5" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 1)">
			<font color="#000000"> seconds &nbsp;&nbsp;(10 ~ 4194303)</font>
			<script language="JavaScript" type="text/JavaScript">
				var rekeystr = "<% tcWebApi_get("WLan_Entry","RekeyInterval","s") %>";
				if("N/A" == rekeystr || "" == rekeystr){
					document.getElementById('WPARekeyInter5').value = "3600";
				}
				else{
					document.getElementById('WPARekeyInter5').value = rekeystr;
				}
			</script>
		</td>
	  </tr>
	</table>
	</div>
	  <%end if%>
	</table>
	<div id="WEP-64Bits_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">       
      <tr> 
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">WEP</font></td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width="10" ><hr noshade></td>
        <td width="440"><hr noshade></td>
      </tr>
      <tr> 
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP AuthType</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata">
		<SELECT NAME="WEP_TypeSelection1" SIZE="1" onChange="doWEPTypeChange()">
				<OPTION value="OpenSystem" <% if tcWebApi_get("WLan_Entry","WEPAuthType","h") = "OpenSystem" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","WEPAuthType","h") = "N/A" then asp_Write("selected") end if %>>OPENWEP
				<OPTION value="SharedKey" <% if tcWebApi_get("WLan_Entry","WEPAuthType","h") = "SharedKey" then asp_Write("selected") end if %>>SHAREDWEP
				<OPTION value="WEPAuto" <% if tcWebApi_get("WLan_Entry","WEPAuthType","h") = "WEPAuto" then asp_Write("selected") end if %>>Both			
		</SELECT>
		</td>
	</tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right><font color="#000000">WEP 64-bits</font></div></td>
        <td class="tabdata">&nbsp;</td>
        <td class="tabdata"><font color="#000000">For each key, please enter either 
          (1) 5 characters, or (2) 10 characters ranging from 
          0~9, a, b, c, d, e, f.</font></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right><font color="#000000">WEP 128-bits</font></div></td>
        <td class="tabdata">&nbsp;</td>
        <td class="tabdata"><font color="#000000">For each key, please enter either 
          (1) 13 characters, or (2) 26 characters ranging from 
          0~9, a, b, c, d, e, f.</font></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") elseif tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#1</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key13" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this)" > 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#2</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key23" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this)" > 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#3</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key33" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key3Str","s") end if%>"  onBlur="doKEYcheck(this)" > 
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange"></td>
        <td class="tabdata"><div align=right> 
            <INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> >
            <font color="#000000">Key</font>#4</div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WEP_Key43" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key4Str","s") end if%>"  onBlur="doKEYcheck(this)" > 
        </td>
      </tr>

	</table>
	</div>
	<div id="WEP-128Bits_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
        <td width="150" height="30" class="title-main"><font color="#FFFFFF">WEP</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td><td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP AuthType</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata">
		<SELECT NAME="WEP_TypeSelection2" SIZE="1" onChange="doWEPTypeChange()">
				<OPTION value="OpenSystem" <% if tcWebApi_get("WLan_Entry","WEPAuthType","h") = "OpenSystem" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","WEPAuthType","h") = "N/A" then asp_Write("selected") end if %>>OPENWEP
				<OPTION value="SharedKey" <% if tcWebApi_get("WLan_Entry","WEPAuthType","h") = "SharedKey" then asp_Write("selected") end if %>>SHAREDWEP
				<OPTION value="WEPAuto" <% if tcWebApi_get("WLan_Entry","WEPAuthType","h") = "WEPAuto" then asp_Write("selected") end if %>>Both			
		</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 64-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000">For each key, please enter either (1) 5 characters, or (2) 10 characters ranging from 0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WEP 128-bits</font></div></td>
		<td class="tabdata">&nbsp;</td>
		<td class="tabdata"><font color="#000000">For each key, please enter either (1) 13 characters, or (2) 26 characters ranging from 0~9, a, b, c, d, e, f.</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="1" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#1</div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">   
			<INPUT TYPE="TEXT" NAME="WEP_Key14" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key1Str","s") end if%>" onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="2" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#2</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WEP_Key24" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan_Entry","Key2Str","s") end if%>"  onBlur="doKEYcheck(this);" >
		</td>
	</tr>
	<tr>                                                                                      
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td><td class="tabdata"><div align=right>                                                                                                                                                                                     
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="3" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#3</div>
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
			<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="4" <%If tcWebApi_get("WLan_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if%> ><font color="#000000">Key</font>#4</div>
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
		<td height="30" width="150" class="title-main"><font color="#FFFFFF">WPA-PSK</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encryption</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection4"  onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES 
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Pre-Shared Key</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey1" SIZE="48" MAXLENGTH="64" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck(this)"><font color="#000000">(8~63 characters or 64 Hex string)</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Key Renewal Interval</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" id="keyRenewalInterval1" NAME="keyRenewalInterval1" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 0)">
          	<font color="#000000"> seconds &nbsp;&nbsp;(10 ~ 4194303)</font>
		  	<script language="JavaScript" type="text/JavaScript">
				var rekeystr = "<% tcWebApi_get("WLan_Entry","RekeyInterval","s") %>";
				if("N/A" == rekeystr || "" == rekeystr){
					document.getElementById('keyRenewalInterval1').value = "3600";
				}
				else{
					document.getElementById('keyRenewalInterval1').value = rekeystr;
				}
			</script>
		  </td>
	</tr>
	</table>
	</div>
	<div id="WPAPSK_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">		
	<tr>		
		<td height="30" width="150" class="title-main"><font color="#FFFFFF">WPA-PSK</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encryption</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection5" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if%>>AES 
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%>>TKIP
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES
		</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Pre-Shared Key</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey2" SIZE="48" MAXLENGTH="64" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck(this)"><font color="#000000">(8~63 characters or 64 Hex string)</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Key Renewal Interval</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" id="keyRenewalInterval2" NAME="keyRenewalInterval2" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 0)">
          	<font color="#000000"> seconds &nbsp;&nbsp;(10 ~ 4194303)</font>
		  	<script language="JavaScript" type="text/JavaScript">
				var rekeystr = "<% tcWebApi_get("WLan_Entry","RekeyInterval","s") %>";
				if("N/A" == rekeystr || "" == rekeystr){
					document.getElementById('keyRenewalInterval2').value = "3600";
				}
				else{
					document.getElementById('keyRenewalInterval2').value = rekeystr;
				}
			</script>
		  </td>
	</tr>
	</table>
	</div>
	<div id="WPAPSKWPA2PSK_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="30" width="150" class="title-main"><font color="#FFFFFF">WPA-PSK</font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encryption</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="TKIP_Selection6" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES 
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES 
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Pre-Shared Key</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PreSharedKey3" SIZE="48" MAXLENGTH="64" VALUE="<%If tcWebApi_get("WLan_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan_Entry","WPAPSK","s") end if%>" onBlur="wpapskCheck(this)"><font color="#000000">(8~63 characters or 64 Hex string)</font>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Key Renewal Interval</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" id="keyRenewalInterval3" NAME="keyRenewalInterval3" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 0)">
          	<font color="#000000"> seconds &nbsp;&nbsp;(10 ~ 4194303)</font>
		  	<script language="JavaScript" type="text/JavaScript">
				var rekeystr = "<% tcWebApi_get("WLan_Entry","RekeyInterval","s") %>";
				if("N/A" == rekeystr || "" == rekeystr){
					document.getElementById('keyRenewalInterval3').value = "3600";
				}
				else{
					document.getElementById('keyRenewalInterval3').value = rekeystr;
				}
			</script>
		  </td>
	</tr>
	</table>
	</div>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
	<% if tcWebApi_get("Info_WLan","isWDSSupported","h") = "Yes" then %>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">WDS Settings</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
    <tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WDS Mode</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		   <input type="RADIO" name="WLAN_WDS_Active" value="1" onClick="autoWLAN_WDS_Active()" <%If tcWebApi_get("WLan_WDS","WdsEnable","h") <> "0" then asp_Write("checked") end if%> >
          <font color="#000000">Activated</font> 
          <input type="RADIO" name="WLAN_WDS_Active" value="0" onClick="autoWLAN_WDS_Deactive()" <%If tcWebApi_get("WLan_WDS","WdsEnable","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan_WDS","WdsEnable","h") = "N/A" then asp_Write("checked") end if%> >
		  <font color="#000000">Deactivated</font>
		</td>
	</tr>
	</table>
	<div id="else_div">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">WDS Encryption Type</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<SELECT NAME="WDS_EncrypType_Selection" SIZE="1" onChange="doWDSEncrypTypeChange()">
				<OPTION value="TKIP" <%If tcWebApi_get("WLan_WDS","WdsEncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP
				<OPTION value="AES" <%If tcWebApi_get("WLan_WDS","WdsEncrypType","h") = "AES" then asp_Write("selected") end if %> >AES 
			</SELECT>
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">WDS Key</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WDS_Key" SIZE="48" MAXLENGTH="64" VALUE="<%If tcWebApi_get("WLan_WDS","WdsKey","h") <> "N/A" then tcWebApi_get("WLan_WDS","WdsKey","s") end if%>" onBlur="WDSKeyCheck(this)"><font color="#000000">(8~63 characters or 64 Hex string)</font>
		</td>
    </tr>
	</table>
	</div>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
    <tr>
   		<td width="150" class="light-orange">&nbsp;</td>
    	<td width="10" class="light-orange"></td>
    	<td width="150" class="tabdata"><div align="right">
        WDS Peer MAC #1</div>
		</td>
		<td width="10" class="tabdata"><div align="center">:</div></td>
    	<td width="440" class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC1" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_WDS","Wds_MAC0","h") <> "N/A" then tcWebApi_get("WLan_WDS","Wds_MAC0","s") end if%>" onBlur="doMACcheck(this)">
		</td>
  	</tr>
    <tr>
    	<td class="light-orange">&nbsp;</td>
    	<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
       	 	WDS Peer MAC #2</div>
		</td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC2" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_WDS","Wds_MAC1","h") <> "N/A" then tcWebApi_get("WLan_WDS","Wds_MAC1","s") end if%>" onBlur="doMACcheck(this)">
		</td>
  	</tr>
    <tr>
    	<td class="light-orange">&nbsp;</td>
    	<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
       		 WDS Peer MAC #3</div>
		</td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC3" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_WDS","Wds_MAC2","h") <> "N/A" then tcWebApi_get("WLan_WDS","Wds_MAC2","s") end if%>" onBlur="doMACcheck(this)">
		</td>
  	</tr>
    <tr>
    	<td class="light-orange">&nbsp;</td>
    	<td class="light-orange"></td>
    	<td class="tabdata"><div align="right">
        	WDS Peer MAC #4</div>
		</td>
		<td class="tabdata"><div align="center">:</div></td>
    	<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC4" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan_WDS","Wds_MAC3","h") <> "N/A" then tcWebApi_get("WLan_WDS","Wds_MAC3","s") end if%>" onBlur="doMACcheck(this)">
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
