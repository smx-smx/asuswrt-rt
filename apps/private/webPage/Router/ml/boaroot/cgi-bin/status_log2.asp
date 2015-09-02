
<%
if request_Form("logFlag") = "1" then
	TCWebApi_set("Syslog_Entry","LogEnable","syslogEnable")
	TCWebApi_set("Syslog_Entry","WriteLevel","logLevelSelect")
	TCWebApi_set("Syslog_Entry","DisplayLevel","DisplayLevelSelect")
	if request_Form("remoteSyslog") = "Yes" then
		TCWebApi_set("Syslog_Entry","remoteSyslogEnable","RemotelogEnable")
		TCWebApi_set("Syslog_Entry","remoteHost","syslogServerAddr")
		TCWebApi_set("Syslog_Entry","remotePort","serverPort")
	end if
	
	TCWebApi_commit("Syslog_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script>
function init()
{
	if (document.StatusLogEnhaceForm.syslogEnable[1].checked) {
		/* disable all */
		document.StatusLogEnhaceForm.logLevelSelect.disabled = true;
		document.StatusLogEnhaceForm.DisplayLevelSelect.disabled = true;
		if (document.StatusLogEnhaceForm.remoteSyslog.value == "Yes") {
			document.StatusLogEnhaceForm.RemotelogEnable[0].disabled = true;
			document.StatusLogEnhaceForm.RemotelogEnable[1].disabled = true;
			document.StatusLogEnhaceForm.syslogServerAddr.disabled = true;
			document.StatusLogEnhaceForm.serverPort.disabled = true;
		}
	}
	else {
		/* check if disable remote server */
		document.StatusLogEnhaceForm.logLevelSelect.disabled = false;
		document.StatusLogEnhaceForm.DisplayLevelSelect.disabled = false;
		if (document.StatusLogEnhaceForm.remoteSyslog.value == "Yes") {
			document.StatusLogEnhaceForm.RemotelogEnable[0].disabled = false;
			document.StatusLogEnhaceForm.RemotelogEnable[1].disabled = false;
			init2();
		}
	}
}
function init2()
{
	if (document.StatusLogEnhaceForm.RemotelogEnable[1].checked) {
		document.StatusLogEnhaceForm.syslogServerAddr.disabled = true;
		document.StatusLogEnhaceForm.serverPort.disabled = true;
	}
	else {
		document.StatusLogEnhaceForm.syslogServerAddr.disabled = false;
		document.StatusLogEnhaceForm.serverPort.disabled = false;
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

var intervalid;
function SysLogDoLoad()
{
	intervalid = setInterval( "GetHomePage()",60*1000 );
}
function GetHomePage()
{
	window.location='status_SysLogEnhance.htm';
	clearInterval(intervalid);
}
function isValidIpAddr(ip1,ip2,ip3,ip4)
{
	if(ip1==0 || ip4==0 || ip4==255 || ip1==127 )
		return false;
	return true;
}
function valSvValidateIP(Address)
{
	var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var i;
	var error=null;
	if(address == null) {
		alert("<%tcWebApi_get("String_Entry","SystemLog2Alert0Text","s")%>");
		error="Invalid IP address";
		return false;
	}
	else {
		digits = address[0].split(".");
		for(i=0; i < 4; i++) {
			if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223)) {
				alert("<%tcWebApi_get("String_Entry","SystemLog2Alert0Text","s")%>");
				error="Invalid IP address";
				return false;
				break;
			}
		}
		if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3],false)) {
			alert("<%tcWebApi_get("String_Entry","SystemLog2Alert0Text","s")%>");
			return false;
		}
	}
	return error;
}
function svIpAddressCheck(address)
{
	var message;
	if (address.value != "N/A") {
		message = valSvValidateIP(address.value);
		if(message!=null) {
			address.value="0.0.0.0";
			if(address.disabled == false) {
				address.focus();
			}
			return false;
		}
	}
	return true;
}
function PortCheck(port)
{
	if (!isNumeric(port)) {
		//alert("UDP Server Port must be 514!");
		alert("<%tcWebApi_get("String_Entry","SystemLog2Alert1Text","s")%>");
		document.StatusLogEnhaceForm.serverPort.value = 514;
		document.StatusLogEnhaceForm.serverPort.focus();
		return false;
	}
	return true;
}
function doSubmit(index)
{
	if(index == 1){
		window.location.href="/cgi-bin/status_log2.cgi";
	}
	else {
		document.StatusLogEnhaceForm.logFlag.value = 1;
		if (document.StatusLogEnhaceForm.remoteSyslog.value == "Yes") {
			if (document.StatusLogEnhaceForm.syslogEnable[0].checked && document.StatusLogEnhaceForm.RemotelogEnable[0].checked) {
				/* remote syslog server is enable */
				if(svIpAddressCheck(document.StatusLogEnhaceForm.syslogServerAddr) == true){
					if(PortCheck(document.StatusLogEnhaceForm.serverPort.value) == true){
						document.StatusLogEnhaceForm.submit();
					}
					else{
						document.StatusLogEnhaceForm.serverPort.focus();
					}
				}
				else{
					document.StatusLogEnhaceForm.syslogServerAddr.focus();
				}
			}
			else {
				/* remote syslog server is disable */
				document.StatusLogEnhaceForm.submit();
			}
		}
		else {
			document.StatusLogEnhaceForm.submit();
		}
	}
}
</script>
</head><body onLoad="init();">
<FORM METHOD="POST" ACTION="/cgi-bin/status_log2.asp" name="StatusLogEnhaceForm">
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
    <tr> 
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
    </tr>
    <tr> 
      <td width="150" height="30" class="title-main"> <%tcWebApi_get("String_Entry","SystemLog2Text","s")%></td>
      <td width="10" class="black">&nbsp;</td>
      <td width="150"></td>
      <td width="10" ></td>
      <td width="440"></td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td colspan="3" class="tabdata"><div align=left> <%tcWebApi_get("String_Entry","SystemLog2InfoText","s")%></div></td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td colspan="3" class="tabdata">&nbsp;</td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata" colspan="3"><div align=left><%tcWebApi_get("String_Entry","SystemLog2Info2Text","s")%></div></tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td colspan="3" class="tabdata">&nbsp;</td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <%tcWebApi_get("String_Entry","SystemLog2LogText","s")%></div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> 
		  <INPUT TYPE="RADIO" NAME="syslogEnable" VALUE="Yes" <%if tcWebApi_get("Syslog_Entry","LogEnable","h") = "Yes" then asp_Write("checked") end if %> onclick="init()">
		   <%tcWebApi_get("String_Entry","SystemLog2EnText","s")%>
		  <INPUT TYPE="RADIO" NAME="syslogEnable" VALUE="No" <%if tcWebApi_get("Syslog_Entry","LogEnable","h") = "No" then asp_Write("checked") end if %> onclick="init()">
		   <%tcWebApi_get("String_Entry","SystemLog2DisaText","s")%>
	  </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata" colspan="3">&nbsp;</td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <%tcWebApi_get("String_Entry","SystemLog2Level0Text","s")%></div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <SELECT NAME="logLevelSelect" SIZE="1">
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "0" then asp_Write("selected") end if %> value="0"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt0Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "1" then asp_Write("selected") end if %> value="1"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt1Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "2" then asp_Write("selected") end if %> value="2"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt2Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "3" then asp_Write("selected") end if %> value="3"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt3Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "4" then asp_Write("selected") end if %> value="4"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt4Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "5" then asp_Write("selected") end if %> value="5"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt5Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "6" then asp_Write("selected") end if %> value="6"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt6Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","WriteLevel","h") = "7" then asp_Write("selected") end if %> value="7"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt7Text","s")%> 
        </SELECT></td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <%tcWebApi_get("String_Entry","SystemLog2Level1Text","s")%></div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <SELECT NAME="DisplayLevelSelect" SIZE="1">
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "0" then asp_Write("selected") end if %> value="0"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt0Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "1" then asp_Write("selected") end if %> value="1"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt1Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "2" then asp_Write("selected") end if %> value="2"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt2Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "3" then asp_Write("selected") end if %> value="3"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt3Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "4" then asp_Write("selected") end if %> value="4"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt4Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "5" then asp_Write("selected") end if %> value="5"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt5Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "6" then asp_Write("selected") end if %> value="6"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt6Text","s")%> 
          <OPTION <%if tcWebApi_get("Syslog_Entry","DisplayLevel","h") = "7" then asp_Write("selected") end if %> value="7"><%tcWebApi_get("String_Entry","SystemLog2LevelOpt7Text","s")%> 
        </SELECT></td>
    </tr>
	<%if tcWebApi_get("WebCustom_Entry","isRemoteSyslog","h") = "Yes" then%>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <%tcWebApi_get("String_Entry","SystemLog2RemoteText","s")%></div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata">
	    <INPUT TYPE="RADIO" NAME="RemotelogEnable" VALUE="1" <%if tcWebApi_get("Syslog_Entry","remoteSyslogEnable","h") = "1" then asp_Write("checked") end if %> onclick="init2()">
        <%tcWebApi_get("String_Entry","SystemLog2EnText","s")%>
        <INPUT TYPE="RADIO" NAME="RemotelogEnable" VALUE="0" <%if tcWebApi_get("Syslog_Entry","remoteSyslogEnable","h") = "0" then asp_Write("checked") end if %> onclick="init2()">
        <%tcWebApi_get("String_Entry","SystemLog2DisaText","s")%>
		</td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <%tcWebApi_get("String_Entry","SystemLog2IPText","s")%></div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> 
	  <INPUT TYPE="TEXT" NAME="syslogServerAddr" SIZE="25" MAXLENGTH="25" VALUE="<%if tcWebApi_get("Syslog_Entry","remoteHost","h") <> "N/A" then tcWebApi_get("Syslog_Entry","remoteHost","s") else asp_Write("0.0.0.0") end if%>" class="tableinputvalue">
	  </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <%tcWebApi_get("String_Entry","SystemLog2PortText","s")%></div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> 
	  <INPUT TYPE="TEXT" NAME="serverPort" SIZE="5" MAXLENGTH="5" VALUE="<%if tcWebApi_get("Syslog_Entry","remotePort","h") <> "N/A" then tcWebApi_get("Syslog_Entry","remotePort","s") else asp_Write("514") end if%>" class="tableinputvalue">
	  </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td colspan="3" class="tabdata">&nbsp;</td>
    </tr>
	<%end if%>
  </table>
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
	<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td>
      <td width="440" class="orange">&nbsp; 
	  <input type="BUTTON" name="Syslog_Save" value="<%tcWebApi_get("String_Entry","ButtonApplyText","s")%>" onClick="doSubmit(0);">
      &nbsp; 
      <INPUT TYPE="BUTTON" NAME="Viewlog" VALUE="<%tcWebApi_get("String_Entry","SystemLog2BtnViewText","s")%>" onClick="doSubmit(1);">
	  <INPUT TYPE="HIDDEN" NAME="logFlag" VALUE="0">
	  <INPUT TYPE="HIDDEN" NAME="remoteSyslog" VALUE="<%tcWebApi_get("WebCustom_Entry","isRemoteSyslog","s")%>">
	  </td>
	 </tr>
  </table></form>
</body></html>
