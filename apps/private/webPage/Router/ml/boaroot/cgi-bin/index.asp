<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<script language="JavaScript" type="text/JavaScript">
/******************For Multi-Language Strings Start*************************/
var	ary_strings = [
				['DesIPInvalid', '<% tcWebApi_get("String_Entry", "JSDesIPInvalid", "s")%>'],
				['SorIPInvalid', '<% tcWebApi_get("String_Entry", "JSSorIPInvalid", "s")%>'],
				['IPIsEmpty', '<% tcWebApi_get("String_Entry", "JSIPIsEmpty", "s")%>'],
				['DesNetInvalid', '<% tcWebApi_get("String_Entry", "JSDesNetInvalid", "s")%>'],
				['SorNetInvalid', '<% tcWebApi_get("String_Entry", "JSSorNetInvalid", "s")%>'],
				['SubMaskInvalid', '<% tcWebApi_get("String_Entry", "JSSubMaskInvalid", "s")%>'],
				['ContentCHN', '<% tcWebApi_get("String_Entry", "JSContentCHN", "s")%>'],
				['IntegerInValid','<% tcWebApi_get("String_Entry", "JSIntegerInValid", "s")%>'],
				['VPIInValid','<% tcWebApi_get("String_Entry", "JSVPIInValid", "s")%>'],
				['VCIInValid','<% tcWebApi_get("String_Entry", "JSVCIInValid", "s")%>'],
				['PCRInvalid','<% tcWebApi_get("String_Entry", "JSPCRInValid", "s")%>'],
				['PCRSizeInvalid','<% tcWebApi_get("String_Entry", "JSPCRSizeInValid", "s")%>'],
				['SCRInvalid','<% tcWebApi_get("String_Entry", "JSSCRInValid", "s")%>'],
				['SCRSizeInvalid','<% tcWebApi_get("String_Entry", "JSPSRSizeInValid", "s")%>'],
				['MBSInvalid','<% tcWebApi_get("String_Entry", "JSMBSInValid", "s")%>'],
				['MBRSizeInvalid','<% tcWebApi_get("String_Entry", "JSMBRSizeInValid", "s")%>'],
				['UpdateSucc','<% tcWebApi_get("String_Entry", "UpdateSuccessfulMsgText", "s")%>'],
				['VirsvrCgiRule','<% tcWebApi_get("String_Entry", "VirsvrCgiRule", "s")%>'],
				['VirsvrCgiStartPort','<% tcWebApi_get("String_Entry", "PortTriggerStartPort0Text", "s")%>'],
				['VirsvrCgiEndPort','<% tcWebApi_get("String_Entry", "PortTriggerStartPort1Text", "s")%>'],
				['VirsvrCgiLocalIP','<% tcWebApi_get("String_Entry", "VirsvrCgiLocalIP", "s")%>'],
				['VirsvrCgiLocalStartPort','<% tcWebApi_get("String_Entry", "VirsvrCgiLocalStartPort", "s")%>'],
				['VirsvrCgiLocalEndPort','<% tcWebApi_get("String_Entry", "VirsvrCgiLocalEndPort", "s")%>'],
				['VirsvrCgiEdit','<% tcWebApi_get("String_Entry", "VirsvrCgiEdit", "s")%>'],
				['VirsvrCgiDrop','<% tcWebApi_get("String_Entry", "VirsvrCgiDrop", "s")%>'],
				['ACLCgiIndexText','<% tcWebApi_get("String_Entry", "ACLCgiIndexText", "s")%>'],
				['ACLCgiActiveText','<% tcWebApi_get("String_Entry", "ACLCgiActiveText", "s")%>'],
				['ACLCgiSeIPAddrText','<% tcWebApi_get("String_Entry", "ACLCgiSeIPAddrText", "s")%>'],
				['ACLCgiApplicationText','<% tcWebApi_get("String_Entry", "ACLCgiApplicationText", "s")%>'],
				['ACLCgiInterfaceText','<% tcWebApi_get("String_Entry", "ACLCgiInterfaceText", "s")%>'],
				['natcgiip','<% tcWebApi_get("String_Entry", "NatSessionCgiText0", "s")%>'],
				['natcginumber','<% tcWebApi_get("String_Entry", "NatSessionCgiText1", "s")%>'],
				['natcgiremove','<% tcWebApi_get("String_Entry", "NatSessionCgiText2", "s")%>'],
				['ShouldFailed','<% tcWebApi_get("String_Entry", "JSShouldFailedText", "s")%>'],
				['InvalidIP','<% tcWebApi_get("String_Entry", "JSInvalidIPText", "s")%>'],
				['InvalidIPRange','<% tcWebApi_get("String_Entry", "JSInvalidIPRangeText", "s")%>'],
				['ShouldCharacter','<% tcWebApi_get("String_Entry", "JSShouldCharacText", "s")%>'],
				['StartCharacter','<% tcWebApi_get("String_Entry", "JSStartCharacText", "s")%>'],
				['InvalidName','<% tcWebApi_get("String_Entry", "JSInvalidNameText", "s")%>'],
				['InvalidNum','<% tcWebApi_get("String_Entry", "JSInvalidNumText", "s")%>'],
				['EnterPassword','<% tcWebApi_get("String_Entry", "JSEnterPasswordText", "s")%>'],
				['PasswordConfirm','<% tcWebApi_get("String_Entry", "JSPasswordConfirmText", "s")%>'],
				['NameCharacter','<% tcWebApi_get("String_Entry", "JSNameCharacText", "s")%>'],
				['NameFormat','<% tcWebApi_get("String_Entry", "JSNameFormatText", "s")%>'],
				['NameContain','<% tcWebApi_get("String_Entry", "JSNameContainText", "s")%>'],
				['InvalidGWIP','<% tcWebApi_get("String_Entry", "JSInvalidGWIPText", "s")%>'],
				['InvalidMac','<% tcWebApi_get("String_Entry", "JSInvalidMacText", "s")%>'],
				['InvalidNetwork','<% tcWebApi_get("String_Entry", "JSInvalidNewworkText", "s")%>'],
				['InvalidNetSrc','<% tcWebApi_get("String_Entry", "JSInvalidNetSrcText", "s")%>'],
				['InvalidNetMask','<% tcWebApi_get("String_Entry", "JSInvalidNetMaskText", "s")%>'],
				['SelectSize','<% tcWebApi_get("String_Entry", "JSSelectSizeText", "s")%>'],
				['SelectCount','<% tcWebApi_get("String_Entry", "JSSelectCountText", "s")%>'],
				['InvalidPort','<% tcWebApi_get("String_Entry", "JSInvalidPortNumText", "s")%>'],
				['InvalidIpAddr','<% tcWebApi_get("String_Entry", "JSInvalidIPAddrText", "s")%>'],
				['UserPassInvalid','<% tcWebApi_get("String_Entry", "JSInvalidUserPassText", "s")%>'],
				['UsernameNum','<% tcWebApi_get("String_Entry", "JSUsernameNumText", "s")%>'],
				['UserPassalpha','<% tcWebApi_get("String_Entry", "JSUserPassalphaText", "s")%>'],
				['Usernamealpha','<% tcWebApi_get("String_Entry", "JSUsernamealphaText", "s")%>'],
				['UserPassone','<% tcWebApi_get("String_Entry", "JSUserPassoneText", "s")%>'],
				['Usernameone','<% tcWebApi_get("String_Entry", "JSUsernameoneText", "s")%>'],
				['UserPassStart','<% tcWebApi_get("String_Entry", "JSUsernamestartText", "s")%>'],
				['integerDigits','<% tcWebApi_get("String_Entry", "JSintegerDigitsText", "s")%>'],
				['NatIPAddrRuleType','<% tcWebApi_get("String_Entry", "NatIPAddrRuleTypeText", "s")%>'],
				['NatIPAddrStartIP','<% tcWebApi_get("String_Entry", "NatIPAddrStartIPText", "s")%>'],
				['NatIPAddrEndIP','<% tcWebApi_get("String_Entry", "NatIPAddrEndIPText", "s")%>'],
				['NatIPAddrPublicStartIP','<% tcWebApi_get("String_Entry", "NatIPAddrPublicStartIPText", "s")%>'],
				['NatIPAddrPublicEndIP','<% tcWebApi_get("String_Entry", "NatIPAddrPublicEndIPText", "s")%>'],
				['KeywordText','<% tcWebApi_get("String_Entry", "KeywordText", "s")%>'],
				['SystemLogText','<% tcWebApi_get("String_Entry", "SystemLogText", "s")%>'],
				['VoIPMediaCGI0','<% tcWebApi_get("String_Entry", "VoIPMediaCGI0Text", "s")%>'],
				['VoIPMediaCGI1','<% tcWebApi_get("String_Entry", "VoIPMediaCGI1Text", "s")%>'],
				['VoIPMediaCGI2','<% tcWebApi_get("String_Entry", "VoIPMediaCGI2Text", "s")%>'],
				['VoIPSpeedDialNumber','<% tcWebApi_get("String_Entry", "VoIPSpeedDialNumberText", "s")%>'],
				['VoIPSpeedUserId','<% tcWebApi_get("String_Entry", "VoIPSpeedUserIdText", "s")%>'],
				['VoIPSpeedIPName','<% tcWebApi_get("String_Entry", "VoIPSpeedIPNameText", "s")%>'],
				['VoIPSpeedPort','<% tcWebApi_get("String_Entry", "VoIPSpeedPortText", "s")%>'],
				['vpiRange','<% tcWebApi_get("String_Entry", "WizardWanDHCPJS1Text", "s")%>'],
				['vciRange','<% tcWebApi_get("String_Entry", "JSVCIRangeText", "s")%>'],
				['pcrRange','<% tcWebApi_get("String_Entry", "JSPCRRangeText", "s")%>'],
				['InvalidIpAndNetMask','<% tcWebApi_get("String_Entry", "JSInvalidIpAndNetMaskText", "s")%>'],
				['InvalidIPv6Addr','<% tcWebApi_get("String_Entry", "JSInvalidIPv6AddrText", "s")%>'],
				['InvalidIPv6Prefix','<% tcWebApi_get("String_Entry", "JSInvalidIPv6PrefixText", "s")%>'],
				['IPv6WanIPGWDomain','<% tcWebApi_get("String_Entry", "JSIPv6WanIPGWDomainText", "s")%>'],
				['','']
			];
/******************End*************************/
</script>
</head>
<frameset rows="61,67,*" framespacing="0" border="0"frameborder="0">
<frame name="header" src="/cgi-bin/status.asp" marginwidth="0" marginheight="0">
    <frame name="navigation" src="/cgi-bin/navigation-status.asp" marginwidth="0" marginheight="0">
<frame name="main" 
	<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
	<%if tcwebApi_get("WebCustom_Entry","isCZOTESupported","h")="Yes" then%>
	src="/cgi-bin/wizardPPP_OTE.asp"
	<%else%>
	<% IF tcWebApi_get("dynDisp_Entry","CurMaskBit0","h") = "1" THEN%>
	src="/cgi-bin/status_deviceinfo.asp"
	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit1","h") = "1" THEN%>
	src="/cgi-bin/status_log.cgi"
	<% elseif tcWebApi_get("dynDisp_Entry","CurMaskBit2","h") = "1" THEN%>
	src="/cgi-bin/status_statistics.asp"	
	<% End if%>
	<%end if%>
marginwidth="0" marginheight="0">

</frameset><noframes>
</noframes>
</html>
