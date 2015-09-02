<%
If Request_Form("SaveFlag")="1" Then
	TCWebApi_set("Upnpd_Entry","Active","UPnP_active")
TCWebApi_set("Upnpd_Entry","autoconf","UPnP_auto")
tcWebApi_commit("Upnpd_Entry")
End If	
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
</head>
<script language="JavaScript">
function upnpOff(off)
{
	if(off)
	{
		document.UPnP_form.elements[2].checked = false;
		document.UPnP_form.elements[2].disabled = true;
		document.UPnP_form.elements[3].checked = true;
	}
	else
		document.UPnP_form.elements[2].disabled = false;
}
function doSubmit()
{
document.UPnP_form.SaveFlag.value = 1;
document.UPnP_form.submit();
}
</script>

<body onLoad="if(document.UPnP_form.elements[1].checked) upnpOff(1); else upnpOff(0)">
<FORM METHOD="POST" ACTION="/cgi-bin/access_upnp.asp" name="UPnP_form"><div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF"><%tcWebApi_get("String_Entry","UPNPPlugText","s")%></font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"> </td><td width="440"></td>
      </tr><tr>
    <td width="150" height="25" class="light-orange">&nbsp;</td><td width="10" class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","UPNPText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="RADIO" NAME="UPnP_active" VALUE="Yes" onClick="upnpOff(0)" <% If TCWebApi_get("Upnpd_Entry","Active","h") = "Yes" then asp_Write("checked") end if%> >        <font color="#000000"><%tcWebApi_get("String_Entry","UPNPActivatedText","s")%></font>
        <INPUT TYPE="RADIO" NAME="UPnP_active" VALUE="No" onClick="upnpOff(1)" <% If TCWebApi_get("Upnpd_Entry","Active","h") = "No" then asp_Write("checked") end if%> >        <font color="#000000"><%tcWebApi_get("String_Entry","UPNPDeactivatedText","s")%></font>        </td></tr><tr>
    <td height="25" class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","UPNPconfiguredText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT NAME="UPnP_auto" TYPE="RADIO" VALUE="1" <% If TCWebApi_get("Upnpd_Entry","autoconf","h") = "1" then asp_Write("checked") end if%> >        
        <font color="#000000"><%tcWebApi_get("String_Entry","UPNPActivatedText","s")%></font>        
        <INPUT TYPE="RADIO" NAME="UPnP_auto" VALUE="0" <% If TCWebApi_get("Upnpd_Entry","autoconf","h") = "0" then asp_Write("checked") end if%> >        <font color="#000000"><%tcWebApi_get("String_Entry","UPNPDeactivatedText","s")%></font>&nbsp; 
        <font color="#000000"><%tcWebApi_get("String_Entry","UPNPCommentText","s")%></font>        </td></tr><tr>
    <td height="25"  class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<INPUT TYPE="button" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="doSubmit();">
	<INPUT TYPE="HIDDEN" NAME="SaveFlag" VALUE="0"></td></tr></table></div></form>
</body></html>
