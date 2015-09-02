
<%
If Request_Form("Cwmpflag")="1" then 
	TCWebApi_set("Cwmp_Entry", "Active", "CWMP_Active")
	TCWebApi_set("Cwmp_Entry", "acsUrl", "CWMP_ACSURL")
	TCWebApi_set("Cwmp_Entry", "acsUserName", "CWMP_ACSUserName")
	TCWebApi_set("Cwmp_Entry", "acsPassword", "CWMP_ACSPassword")
	TCWebApi_set("Cwmp_Entry", "conReqPath", "CWMP_ConnectionRequestPath")
	TCWebApi_set("Cwmp_Entry", "conReqUserName", "CWMP_ConnectionRequestUserName")
	TCWebApi_set("Cwmp_Entry", "conReqPassword", "CWMP_ConnectionRequestPassword")
	
	TCWebApi_set("Cwmp_Entry", "periodActive", "CWMP_PeriodActive")
	TCWebApi_set("Cwmp_Entry", "periodInterval", "CWMP_PeriodInterval")
	
	tcWebApi_commit("Cwmp_Entry")
End if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">

<script language="javascript" src="/general.js"></script>
<script language="JavaScript">
function cwmpinit(){
	if(document.CWMP_form.elements[0].checked) 
		cwmpSwitch(1);
	else
		cwmpSwitch(0);	
		
	if(document.CWMP_form.CWMPLockFlag.value == 1)
		doCWMPLock();
}

function cwmpSwitch(on_off)
{
	if(on_off == 0){
		for(i = 2; i < 11; i++)
			document.CWMP_form.elements[i].disabled = true;
	}		
	else{
		for(i = 2; i < 11; i++){
			document.CWMP_form.elements[i].disabled = false;
}
		if(document.CWMP_form.CWMP_PeriodActive[1].checked)
			document.CWMP_form.elements[10].disabled = true;
	}
}
function cwmpPeriodSwitch(on_off)
{
	if(on_off == 0)
			document.CWMP_form.elements[10].disabled = true;
	else
			document.CWMP_form.elements[10].disabled = false;
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

function cwmpNumValidCheck()
{
	var value1;
/*	
	value1 = document.CWMP_form.CWMP_ConnectionRequestPort.value;
	if(!isNumeric(value1)){
		alert("The connection request port should be interger!");
		return false;
	}
	else{
		if(Number(value1) > 65535 || Number(value1) < 1){
			alert("The connection request port should be 1-65535!");
			return false;
		}
	}
*/
	value1 = document.CWMP_form.CWMP_PeriodInterval.value;
	if(!document.CWMP_form.elements[8].checked)
		return true;
	if(!isNumeric(value1)){
		alert("<%tcWebApi_get("String_Entry","CwmpJS0Text","s")%>");
		return false;
	}
	else{
		if(Number(value1) > 999999 || Number(value1) < 1){
			alert("<%tcWebApi_get("String_Entry","CwmpJS1Text","s")%>");
			return false;
		}
	}
	
	return true;
}

function StringCheck(val)
{
    re = /^[^\s]+$/;
    if( re.test(val) )
        return true;
    else
        return false;
}

function cwmpStringValidCheck()
{
	var value1;
	var value2 = null;
	var url = /^https*:\/\/\w+/;
	var path = /^\/\w+/;

	value1 = document.CWMP_form.CWMP_ACSURL.value;
	if(value1 == "") {
		alert("<%tcWebApi_get("String_Entry","CwmpJS2Text","s")%>");
		return false;
	}

	if(!StringCheck(value1)){
		alert("<%tcWebApi_get("String_Entry","CwmpJS5Text","s")%>");
		return false;
	}
	else{	
		if(StringCheck(value1)){
			value2 = value1.match(url);
			if(value2 == null){
				alert("<%tcWebApi_get("String_Entry","CwmpJS3Text","s")%>");
				return false;
			}
			if(chineseCheck(document.CWMP_form.CWMP_ACSURL, "ACSURLId"))
			{
				return false;
			}
		}
	}
	
	if(chineseCheck(document.CWMP_form.CWMP_ACSUserName, "ACSUserNameId"))
	{
		return false;
	}
	if(chineseCheck(document.CWMP_form.CWMP_ACSPassword, "ACSPasswordId"))
	{
		return false;
	}
	if(chineseCheck(document.CWMP_form.CWMP_ConnectionRequestUserName, "CWMPConnectionRequestUserNameId"))
	{
		return false;
	}
	if(chineseCheck(document.CWMP_form.CWMP_ConnectionRequestPassword, "CWMPConnectionRequestPasswordId"))
	{
		return false;
	}
	/* value1 = document.CWMP_form.CWMP_ACSUserName.value;
	if(!StringCheck(value1)){
		alert("Username for logining ACS should not be empty!");
		return false;
	}
	
	value1 = document.CWMP_form.CWMP_ACSPassword.value;
	if(!StringCheck(value1)){
		alert("Password for logining ACS should not be empty!");
		return false;
	}
	*/
	
	value1 = document.CWMP_form.CWMP_ConnectionRequestPath.value;
    /*	
	if(!StringCheck(value1)){
		alert("Connection request path should not be empty!");
		return false;
	}
	else{
	*/
	if(StringCheck(value1)){
		value2 = value1.match(path);
		if(value2 == null){
			alert("<%tcWebApi_get("String_Entry","CwmpJS4Text","s")%>");
			return false;
		}
	}
	/*
	value1 = document.CWMP_form.CWMP_ConnectionRequestUserName.value;
	if(!StringCheck(value1)){
		alert("Username for Connection request should not be empty!");
		return false;
	}
	
	value1 = document.CWMP_form.CWMP_ConnectionRequestPassword.value;
	if(!StringCheck(value1)){
		alert("Password for Connection request should not be empty!");
		return false;
	}
	*/
	return true;
}

function cwmpSave()
{
	if(document.CWMP_form.elements[0].checked){
		if(!cwmpNumValidCheck())
			return false;
		if(!cwmpStringValidCheck())
			return false;
	}
	
	document.CWMP_form.Cwmpflag.value=1;	
	document.CWMP_form.submit();

}
function doCWMPLock()
{
	for(i = 0; i < document.forms[0].elements.length; i++)
	{	
		document.forms[0].elements[i].disabled = true;
	}
}
</script>
</head>
<body onload="cwmpinit()">

<form name="CWMP_form" method="post" ACTION="/cgi-bin/access_cwmp.asp" >
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
      <td width="150" height="30" class="title-main"> 
        <%tcWebApi_get("String_Entry","CwmpSetupText","s")%> 
      </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="150"></td>
    <td width="9" ></td>
    <td width="441"></td>
  </tr>
  <tr>
    <td height="25" class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <%tcWebApi_get("String_Entry","CwmpText","s")%>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
      <input name="CWMP_Active" type="radio" value="Yes" <%if tcWebApi_get("Cwmp_Entry", "Active", "h") = "Yes" then asp_Write("checked") end if%> onclick="cwmpSwitch(1)">
      <font color="#000000"><%tcWebApi_get("String_Entry","CwmpActivatedText","s")%></font>
	  <input name="CWMP_Active" type="radio" value="No" <%if tcWebApi_get("Cwmp_Entry", "Active", "h") = "No" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry", "Active", "h") = "N/A" then asp_Write("checked") end if%> onclick="cwmpSwitch(0)">
   	  <font color="#000000"><%tcWebApi_get("String_Entry","CwmpDeactivatedText","s")%></font>
     </td>
  </tr>
  <tr>
      <td height="25" class="title-sub"> 
        <%tcWebApi_get("String_Entry","CwmpACSLoginInfoText","s")%>
      </td>
    <td class="light-orange"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td class="tabdata"><div align="right" id="ACSURLId"> 
          <%tcWebApi_get("String_Entry","CwmpURLText","s")%>
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ACSURL" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsUrl", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "acsUrl", "s") end if%>" size="64" maxlength="256">
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="ACSUserNameId"> 
          <%tcWebApi_get("String_Entry","CwmpUserNameText","s")%>
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ACSUserName" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsUserName", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry",  "acsUserName", "s") end if%>" size="32" maxlength="256">
       	</td>
  </tr>
  </tr>
  <tr>
    <td class="light-orange"><hr noshade class="light-orange-line"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="ACSPasswordId"> 
          <%tcWebApi_get("String_Entry","CwmpPasswordText","s")%>
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ACSPassword" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "acsPassword", "s") end if%>" size="32" maxlength="256">
	 </td>
  </tr>
   <tr>
      <td height="25" class="title-sub"> 
        <%tcWebApi_get("String_Entry","CwmpConnInfoText","s")%>
      </td>
    <td class="light-orange"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
          <%tcWebApi_get("String_Entry","CwmpPathText","s")%>
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ConnectionRequestPath" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqPath", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqPath", "s") end if%>" size="63" maxlength="256">
	</td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="CWMPConnectionRequestUserNameId"> 
          <%tcWebApi_get("String_Entry","CwmpUserNameText","s")%> 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ConnectionRequestUserName" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqUserName", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqUserName", "s") end if%>" size="32" maxlength="256">
	 </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right" id="CWMPConnectionRequestPasswordId"> 
          <%tcWebApi_get("String_Entry","CwmpPasswordText","s")%> 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="CWMP_ConnectionRequestPassword" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqPassword" ,"s") end if%>" size="32" maxlength="256">
	 </td>
	</td>
  </tr>
  <tr>
      <td height="25" class="title-sub"> 
        <%tcWebApi_get("String_Entry","CwmpPeriodicConfigText","s")%>
      </td>
    <td class="light-orange"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
    <td class="tabdata"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
          <%tcWebApi_get("String_Entry","CwmpPeriodicInfoText","s")%>
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
      <input name="CWMP_PeriodActive" value="Yes" type="radio" <%if tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "Yes" then asp_Write("checked") end if%> onclick="cwmpPeriodSwitch(1)">
  	    <%tcWebApi_get("String_Entry","CwmpActivatedText","s")%>
 	  <input name="CWMP_PeriodActive" value="No" type="radio" <%if tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "No" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "N/A" then asp_Write("checked") end if%> onclick="cwmpPeriodSwitch(0)">
 		<%tcWebApi_get("String_Entry","CwmpDeactivatedText","s")%>
    </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right"> 
          <%tcWebApi_get("String_Entry","CwmpIntervalText","s")%> 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
		<input type="text" name="CWMP_PeriodInterval" value="<%if tcWebApi_get("Cwmp_Entry", "periodInterval", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "periodInterval", "s") end if%>" size="9" maxlength="9" >
     </td>
  </tr>

  </tr>
 </table>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >
	<input name="CWMPLockFlag" type="HIDDEN" value="<%if tcWebApi_get("Cwmp_Entry", "CWMPLockFlag", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "CWMPLockFlag", "s") else asp_write("0") end if%>">
	</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="Cwmpflag" VALUE="0">
		<input name="cwmp_apply" type="button" value="<%tcWebApi_get("String_Entry","ButtonApplyText","s")%>" onClick="cwmpSave()" class="sbutton">
      	<input type="reset"  name="cwmp_cancel" value="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>">
   	</td>
  </tr>
</table>
</div>
</form>
</body>
</html>        
