<%
If Request_Form("addNumFlag") <> "FULL" Then
	tcWebApi_set("WebCurSet_Entry","natSession_entry","addNumFlag")
	If Request_Form("submitFlag") = "1" Then
		tcWebApi_set("natSessions_Entry","IPAddress","AddSessionIPAddr")
		tcWebApi_set("natSessions_Entry","sessionNum","AddSessionNum")
		tcWebApi_constSet("natSessions_setting","SessionMode","1")
		tcWebApi_commit("natSessions_Entry")
	End If
End If
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<SCRIPT language=javascript>
function doSave() {
	var ipAddr;
	var sessNum;
	
	ipAddr = document.forms[0].AddSessionIPAddr.value;
	sessNum = document.forms[0].AddSessionNum.value;
	document.forms[0].submitFlag.value = 1;
	if(!valDoValidateIP(ipAddr)) 
	    return false; 
		
	if((!isNumeric(sessNum)) || (Number(sessNum) > 1024))
	{
		alert("<%tcWebApi_get("String_Entry","NatAddSessionJS0Text","s")%>");		return false;
	}
	document.forms[0].submit();
}

function isNumeric(value)
{
    var len= value.length;
    if(len==0)
     	return false;
	if(value.match("[^0-9]") != null)	                             
		return false;
	else
		return true;
}

function valDoValidateIP(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { 
    alert("<%tcWebApi_get("String_Entry","NatAddSessionJS1Text","s")%>");    return false;
  }
  else
  {
    digits = address[0].split(".");
    for(i=0; i < 4; i++)
    {
      if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
      { 
    alert("<%tcWebApi_get("String_Entry","NatAddSessionJS2Text","s")%>" + Address);       	return false;
      }
    }
    if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3],false))
    {
    alert("<%tcWebApi_get("String_Entry","NatAddSessionJS2Text","s")%>" + Address);       	return false;
    }
  }
  return true;
}

function isValidIpAddr(ip1,ip2,ip3,ip4)
{
	if(ip1==0 || ip4==0 || ip4==255 || ip1==127 )	
		return false;			

	return true;	
}
</SCRIPT>

<META content="MSHTML 6.00.2900.5726" name=GENERATOR></HEAD>
<BODY>
<FORM name=NAT_AddSession_form method=post>
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
		<TBODY>
			<TR>
				<TD class=light-orange colSpan=6 height=5>&nbsp;</TD>
			</TR>
			<TR>
				<TD class=title-main width=150 height=30><%tcWebApi_get("String_Entry","NatAddSessionText","s")%></TD>
				<TD class=black width=10>&nbsp;</TD>
				<TD width=50></TD>
				<TD width=160></TD>
				<TD width=10></TD>
				<TD width=380></TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata colSpan=3>
					<DIV align=left><B><%tcWebApi_get("String_Entry","NatAddSessionMsgText","s")%></B></DIV>
				</TD>
			</TR>
			<TR>
				
        <TD height="40" class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=right><%tcWebApi_get("String_Entry","NatAddSessionIPAddrText","s")%><BR><%tcWebApi_get("String_Entry","NatAddSessionIPAddrCommText","s")%> </DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center>:</DIV>
				</TD>
				<TD class=tabdata>
					<INPUT maxLength=15 size=15 value="<%if TCWebAPI_get("natSessions_Entry", "IPAddress", "h") <> "N/A" then TCWebAPI_get("natSessions_Entry", "IPAddress", "s") else asp_Write("0.0.0.0") end if %>" name=AddSessionIPAddr>
				</TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=right><%tcWebApi_get("String_Entry","NatAddSessionNumText","s")%></DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center>:</DIV>
				</TD>
				<TD class=tabdata>
					<INPUT maxLength=4 size=15 value="<%if TCWebAPI_get("natSessions_Entry", "sessionNum", "h") <> "N/A" then TCWebAPI_get("natSessions_Entry", "sessionNum", "s") else asp_Write("0") end if %>" name=AddSessionNum>
					<INPUT type=hidden value=0 name=submitFlag>
					<INPUT type=hidden value="<%if TCWebAPI_get("natSessions_setting", "add_num", "h") <> "N/A" then TCWebAPI_get("natSessions_setting", "add_num", "s") else asp_Write("0") end if %>" name=addNumFlag>
				</TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD>&nbsp;</TD>
				<TD>&nbsp;</TD>
				<TD>&nbsp;</TD>
			</TR>
		</TBODY>
	</TABLE>
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
		<TBODY>
			<TR>
				<TD class=black width=160 height=42>&nbsp;</TD>
				<TD class=orange width=160>&nbsp;</TD>
				<TD class=orange width=440>
					<INPUT onclick="javascript:window.location='/cgi-bin/adv_nat_session.asp'" type=button value=<%tcWebApi_get("String_Entry","ButtonReturnText","s")%> name=AddSessionBackBtn> 
					<INPUT onclick=doSave(); type=button value=<%tcWebApi_get("String_Entry","ButtonNatAdvText","s")%> name=AddSessionSaveBtn> 
				</TD>
			</TR>
		</TBODY>
	</TABLE>
</FORM>
</BODY>
</HTML>
