<%

IF Request_Form("LineFlag")="1" Then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
End if

if request_Form("VoIP_call_ctrl_flag")="1" then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
	tcWebApi_set("VoIPCallCtrl_Entry","AnonymousCallBlock","Anonymous_call_blocking")
	tcWebApi_set("VoIPCallCtrl_Entry","AnonymousCall","Anonymous_calling")

	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallerIdEnable","VoIPCallerId")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallWaitingReply","VoIPCallWaitingReply")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallForwardEnable","VoIPCallForward")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCFUNumber","VoIPCFUNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCFBNumber","VoIPCFBNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCFNRNumber","VoIPCFNRNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallTransfer","VoIPCallTransfer")
if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then
	tcWebApi_set("VoIPCallCtrl_Entry","SIPBlindTransferNumber","VoIPBlindTransferNumber")
End if

	tcWebApi_set("VoIPCallCtrl_Entry","SIPAttendedTransferNumber","VoIPAttendedTransferNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallHold","VoIPCallHold")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallReturnEnable","VoIPCallReturn")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPCallReturnNumber","VoIPCallReturnNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIP3wayConf","VoIP3wayConf")
	tcWebApi_set("VoIPCallCtrl_Entry","SIP3wayConfNumber","VoIP3wayConfNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPMWIEnable","VoIPMWIEnable")

	tcWebApi_set("VoIPCallCtrl_Entry","SIPDNDEnable","VoIPDNDEnable")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPDNDEnableNumber","VoIPDNDEnableNumber")
	tcWebApi_set("VoIPCallCtrl_Entry","SIPDNDDisableNumber","VoIPDNDDisableNumber")
if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then
	tcWebApi_set("VoIPCallCtrl_Entry","DODEnable","VoIPDODEnable")
	tcWebApi_set("VoIPCallCtrl_Entry","DODNumber","VoIPDODNumber")
End if

	tcWebApi_commit("VoIPCallCtrl_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript">
function checkDigit(digit) {
	if(((digit >= 48) && (digit <= 57)) || (digit == 42)) {
		return 0;
	} else {
		return -1;
	}
}

function dosave(){
	var form=document.VOIP_CALL_CTRL_form;
	var i;
	var j;
	var tmp;
	var count;
	var numbers = new Array(20);

	count=0;
	if(form.VoIPCallForward[0].checked) {
		if(form.VoIPCFUNumber.value.length <= 0){
			alert("Please insert Unconditional Forward Number");
			return;
		} else {
			for(i = 0; i < form.VoIPCFUNumber.value.length; i++) {
				tmp = form.VoIPCFUNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Unconditional Forward Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPCFUNumber.value;
			count++;
			
		}
		if(form.VoIPCFBNumber.value.length <= 0){
			alert("Please insert Busy Forward Number");
			return;
		} else {
			for(i = 0; i < form.VoIPCFBNumber.value.length; i++) {
				tmp = form.VoIPCFBNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Busy Forward Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPCFBNumber.value;
			count++;
		}
		if(form.VoIPCFNRNumber.value.length <= 0){
			alert("Please insert No Reply Forward Number");
			return;
		} else {
			for(i = 0; i < form.VoIPCFNRNumber.value.length; i++) {
				tmp = form.VoIPCFNRNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("No Reply Forward Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPCFNRNumber.value;
			count++;
		}
	}
	if(form.VoIPCallTransfer[0].checked) {
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
		if(form.VoIPBlindTransferNumber.value.length <= 0){
			alert("Please insert Blind Transfer Number");
			return;
		} else {
			for(i = 0; i < form.VoIPBlindTransferNumber.value.length; i++) {
				tmp = form.VoIPBlindTransferNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Blind Transfer Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPBlindTransferNumber.value;
			count++;
		}
<%end if%>
		if(form.VoIPAttendedTransferNumber.value.length <= 0){
			alert("Please insert Attended Transfer Number");
			return;
		} else {
			for(i = 0; i < form.VoIPAttendedTransferNumber.value.length; i++) {
				tmp = form.VoIPAttendedTransferNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Attended Transfer Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPAttendedTransferNumber.value;
			count++;
		}
	}
	if(form.VoIP3wayConf[0].checked) {
		if(form.VoIP3wayConfNumber.value.length <= 0){
			alert("Please insert Conference Number");
			return;
		} else {
			for(i = 0; i < form.VoIP3wayConfNumber.value.length; i++) {
				tmp = form.VoIP3wayConfNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Conference Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIP3wayConfNumber.value;
			count++;
		}
	}

	if(form.VoIPCallReturn[0].checked) {
		if(form.VoIPCallReturnNumber.value.length <= 0){
			alert("Please insert Call Return Number");
			return;
		} else {
			for(i = 0; i < form.VoIPCallReturnNumber.value.length; i++) {
				tmp = form.VoIPCallReturnNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Call Return Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPCallReturnNumber.value;
			count++;
		}
	}
	if(form.VoIPDNDEnable[0].checked) {
		if(form.VoIPDNDEnableNumber.value.length <= 0){
			alert("Please insert DND Enable Number");
			return;
		} else {
			for(i = 0; i < form.VoIPDNDEnableNumber.value.length; i++) {
				tmp = form.VoIPDNDEnableNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("DND Enable Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPDNDEnableNumber.value;
			count++;
		}
		if(form.VoIPDNDDisableNumber.value.length <= 0){
			alert("Please insert DND Disable Number Number");
			return;
		} else {
			for(i = 0; i < form.VoIPDNDDisableNumber.value.length; i++) {
				tmp = form.VoIPDNDDisableNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("Call DND Disable Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPDNDDisableNumber.value;
			count++;
		}
	}
<%if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then%>	
	if(form.VoIPDODEnable[0].checked) {
		if(form.VoIPDODNumber.value.length <= 0){
			alert("Please insert DOD Number");
			return;
		} else {
			for(i = 0; i < form.VoIPDODNumber.value.length; i++) {
				tmp = form.VoIPDODNumber.value.charCodeAt(i);
				if(checkDigit(tmp) != 0) {
					alert("DOD Number is invalid");
					return;
				}
			}
			numbers[count]=form.VoIPDODNumber.value;
			count++;
		}
	}
<%end if%>
	for(i = 0; i < count; i++){
		for(j = 0; j < i; j++){
			if(numbers[i]==numbers[j]){
				alert("Duplicate Number is invalid");
				return;
			}

		}

	}


 	form.VoIP_call_ctrl_flag.value=1;
	form.submit();
}

function doLineChange(){
	document.VOIP_CALL_CTRL_form.LineFlag.value = 1;
	document.VOIP_CALL_CTRL_form.submit();
}


function forwardOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIPCFUNumber.disabled=on;
	form.VoIPCFBNumber.disabled=on;
	form.VoIPCFNRNumber.disabled=on;

<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIPCFUNumber.disabled=true;
	form.VoIPCFBNumber.disabled=true;
	form.VoIPCFNRNumber.disabled=true;
<%end if%>	
<%end if%>
}

<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
function waitingOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIPCallWaitingReply[0].disabled=on;
	form.VoIPCallWaitingReply[1].disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIPCallWaitingReply[0].disabled=true;
	form.VoIPCallWaitingReply[1].disabled=true;
<%end if%>	
<%end if%>
}
<%end if%>

function transferOn(on){
	var form=document.VOIP_CALL_CTRL_form;

<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	form.VoIPBlindTransferNumber.disabled=on;
<%end if%>
	
	form.VoIPAttendedTransferNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	form.VoIPBlindTransferNumber.disabled=true;
<%end if%>
	form.VoIPAttendedTransferNumber.disabled=true;
<%end if%>	
<%end if%>
}

function conferenceOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIP3wayConfNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIP3wayConfNumber.disabled=true;
<%end if%>	
<%end if%>
}

function callreturnOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIPCallReturnNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIPCallReturnNumber.disabled=true;
<%end if%>	
<%end if%>
}
function DNDOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIPDNDEnableNumber.disabled=on;
	form.VoIPDNDDisableNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIPDNDEnableNumber.disabled=true;
	form.VoIPDNDDisableNumber.disabled=true;
<%end if%>	
<%end if%>
}
<%if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then%>
function DODOn(on){
	var form=document.VOIP_CALL_CTRL_form;

	form.VoIPDODNumber.disabled=on;
<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>	
	form.VoIPDODNumber.disabled=true;
<%end if%>	
<%end if%>
}
<%end if%>
function onLoadFun(){
	var form=document.VOIP_CALL_CTRL_form;

<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%>
		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%>		
	form.Anonymous_call_blocking[0].disabled=true;
	form.Anonymous_call_blocking[1].disabled=true;
	form.Anonymous_calling[0].disabled=true;
	form.Anonymous_calling[1].disabled=true;
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>	
	form.D_N_D[0].disabled=true;
	form.D_N_D[1].disabled=true;
<%end if%>

	form.VoIPCallerId[0].disabled=true;
	<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	form.VoIPCallWaiting[0].disabled=true;
	<%end if%>
	form.VoIPCallWaitingReply[0].disabled=true;
	form.VoIPCallForward[0].disabled=true;
	form.VoIPCallTransfer[0].disabled=true;
	form.VoIP3wayConf[0].disabled=true;
	form.VoIPCallReturn[0].disabled=true;
	form.VoIPDNDEnable[0].disabled=true;
<%if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then%>
	form.VoIPDODEnable[0].disabled=true;
<%end if%>
	form.VoIPCallerId[1].disabled=true;
	<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	form.VoIPCallWaiting[1].disabled=true;
	<%end if%>
	form.VoIPCallWaitingReply[1].disabled=true;
	form.VoIPCallForward[1].disabled=true;
	form.VoIPCallTransfer[1].disabled=true;
	form.VoIP3wayConf[1].disabled=true;
	form.VoIPCallReturn[1].disabled=true;
	form.VoIPDNDEnable[1].disabled=true;
<%if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then%>
	form.VoIPDODEnable[1].disabled=true;
<%end if%>
	form.Line.disabled=true;
	form.VoIPCFUNumber.disabled=true;
	form.VoIPCFBNumber.disabled=true;
	form.VoIPCFNRNumber.disabled=true;
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>	
	form.VoIPBlindTransferNumber.disabled=true;
<%end if%>
	form.VoIPAttendedTransferNumber.disabled=true;
	form.VoIP3wayConfNumber.disabled=true;
	form.VoIPCallReturnNumber.disabled=true;
	form.VoIPDNDEnableNumber.disabled=true;
	form.VoIPDNDDisableNumber.disabled=true;
<%if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then%>
	form.VoIPDODNumber.disabled=true;
<%end if%>	
<%end if%>
<%end if%>

	if(form.VoIPCallForward[0].checked)
		forwardOn(false);
	else
		forwardOn(true);
	
	<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	if(form.VoIPCallWaiting[0].checked)
		waitingOn(false);
	else
		waitingOn(true);
	<%end if%>
	
	if(form.VoIPCallTransfer[0].checked) {
		transferOn(false);
	} else {
		transferOn(true);
	}

	if(form.VoIP3wayConf[0].checked) {
		conferenceOn(false);
	} else {
		conferenceOn(true);
	}
	
	if(form.VoIPCallReturn[0].checked) {
		callreturnOn(false);
	} else {
		callreturnOn(true);
	}
	if(form.VoIPDNDEnable[0].checked) {
		DNDOn(false);
	} else {
		DNDOn(true);
	}
<%if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then%>
	if(form.VoIPDODEnable[0].checked) {
		DODOn(false);
	} else {
		DODOn(true);
	}
<%end if%>	
}
</script>
</head><body onLoad="onLoadFun()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_call_ctrl.asp" name="VOIP_CALL_CTRL_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
	<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
	<td width="150" height="30" class="title-main">
	<font color="#FFFFFF">VoIP Call Control</font></td>
	<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>
	<INPUT TYPE="HIDDEN" NAME="VoIP_call_ctrl_flag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="LineFlag" VALUE="0">
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">Line</font></div></td>
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
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Anonymous call blocking</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="Anonymous_call_blocking" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCallBlock", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="Anonymous_call_blocking" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCallBlock", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Anonymous calling</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="Anonymous_calling" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCall", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="Anonymous_calling" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "AnonymousCall", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>



	<tr>
		<td class="title-sub"><font color="#000000"></font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>







	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Caller Id Display</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
		<INPUT TYPE="RADIO" NAME="VoIPCallerId" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallerIdEnable", "h") = "Yes" then asp_write("checked") end if %> "><font color="#000000">Enable</fon    t>
		<INPUT TYPE="RADIO" NAME="VoIPCallerId" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallerIdEnable", "h") <> "Yes" then asp_write("checked") end if%> "><font color="#000000">Disable</font    >
		<font color="#000000">(On The Local Phone)</font>
		</td></tr>
	<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Waiting</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPCallWaiting" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingEnable", "h") = "Yes" then asp_write("checked") end if %> onclick="waitingOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallWaiting" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="waitingOn(true)"><font color="#000000">Disable</font>
		</td></tr>
	<%end if%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Waiting SIP Reply</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
		<INPUT TYPE="RADIO" NAME="VoIPCallWaitingReply" VALUE="Ringing" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingReply", "h") = "Ringing" then asp_write("checked") end if %>><font color="#000000">Ringing</font>
		<INPUT TYPE="RADIO" NAME="VoIPCallWaitingReply" VALUE="Queued" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingReply", "h") = "Queued" then asp_write("checked") elseif TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallWaitingReply", "h") = "N/A" then asp_write("checked") end if%>><font color="#000000">Queued</font>
		</td></tr>
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Hold</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPCallHold" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallHold", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallHold" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallHold", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">MWI(message waiting indication)</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPMWIEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPMWIEnable", "h") = "Yes" then asp_write("checked") end if%>><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPMWIEnable" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPMWIEnable", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000">Disable</font>
		</td></tr>
	<tr>
<%end if%>

	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Call Forward</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange" style="height: 22px"></td>
		<td class="light-orange" style="height: 22px"></td>
		<td class="tabdata" align=right style="height: 22px"><font color="#000000">Call Forward</font></td>
		<td class="tabdata" align=center style="height: 22px">:</td>
		<td class="tabdata" style="height: 22px">
			<INPUT TYPE="RADIO" NAME="VoIPCallForward" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallForwardEnable", "h") = "Yes" then asp_write("checked") end if %> onclick="forwardOn(false)"><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallForward" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallForwardEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="forwardOn(true)"><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Unconditional Forward Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPCFUNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCFUNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCFUNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Busy Forward Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPCFBNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCFBNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCFBNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">No Reply Forward Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPCFNRNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCFNRNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCFNRNumber", "s") else asp_Write("") end if %>">
		</td></tr>

	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Call Transfer</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Transfer</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPCallTransfer" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallTransfer", "h") = "Yes" then asp_write("checked") end if%> onclick="transferOn(false)" ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallTransfer" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallTransfer", "h") <> "Yes" then asp_write("checked") end if%> onclick="transferOn(true)" ><font color="#000000">Disable</font>
		</td></tr>
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Blind Transfer Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPBlindTransferNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPBlindTransferNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPBlindTransferNumber", "s") else asp_Write("") end if %>">
		</td></tr>
<%end if%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Attended Transfer Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPAttendedTransferNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPAttendedTransferNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPAttendedTransferNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Three-Way Conference</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Three-Way Conference</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIP3wayConf" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConf", "h") = "Yes" then asp_write("checked") end if%> onclick="conferenceOn(false)" ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIP3wayConf" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConf", "h") <> "Yes" then asp_write("checked") end if%> onclick="conferenceOn(true)" ><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Conference Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIP3wayConfNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConfNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIP3wayConfNumber", "s") else asp_Write("") end if %>">
		</td></tr>

		<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Call Return</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Return</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPCallReturn" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallReturnEnable", "h") = "Yes" then asp_write("checked") end if%> onclick="callreturnOn(false)" ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPCallReturn" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallReturnEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="callreturnOn(true)" ><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">Call Return Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPCallReturnNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallReturnNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPCallReturnNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Do Not Disturb</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">DND</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPDNDEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDEnable", "h") = "Yes" then asp_write("checked") end if%> onclick="DNDOn(false)" ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPDNDEnable" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="DNDOn(true)" ><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">DND enable Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPDNDEnableNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDEnableNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDEnableNumber", "s") else asp_Write("") end if %>">
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">DND disable Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPDNDDisableNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDDisableNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "SIPDNDDisableNumber", "s") else asp_Write("") end if %>">
		</td></tr>
<%if TCWebAPI_get("VoIPBasic_Common", "SlicFXONum", "h") <> "0" then%>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Direct Outward Dialing</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">DOD</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPDODEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "DODEnable", "h") = "Yes" then asp_write("checked") end if%> onclick="DODOn(false)" ><font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="VoIPDODEnable" VALUE="No" <%if TCWebAPI_get("VoIPCallCtrl_Entry", "DODEnable", "h") <> "Yes" then asp_write("checked") end if%> onclick="DODOn(true)" ><font color="#000000">Disable</font>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000">DOD Number</font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="VoIPDODNumber" SIZE="19" MAXLENGTH="19" VALUE="<%if TCWebAPI_get("VoIPCallCtrl_Entry", "DODNumber", "h") <> "N/A" then TCWebAPI_get("VoIPCallCtrl_Entry", "DODNumber", "s") else asp_Write("") end if %>">
		</td></tr>
<%end if%>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="dosave();"></td></tr></table>
</form></body></html>
