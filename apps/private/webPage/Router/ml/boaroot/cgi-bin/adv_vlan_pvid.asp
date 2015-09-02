<%@SCRIPT LANGUAGE="VBSCRIPT"%>

<%
If Request.Form("vlan_pvid_apply") = "SAVE"  Then
   TCWebApi_set(VLAN_ATMPVID0,"VC0_pvid")
   TCWebApi_set(VLAN_ATMPVID1,"VC1_pvid")
   TCWebApi_set(VLAN_ATMPVID2,"VC2_pvid")
   TCWebApi_set(VLAN_ATMPVID3,"VC3_pvid")
   TCWebApi_set(VLAN_ATMPVID4,"VC4_pvid")
   TCWebApi_set(VLAN_ATMPVID5,"VC5_pvid")
   TCWebApi_set(VLAN_ATMPVID6,"VC6_pvid")
   TCWebApi_set(VLAN_ATMPVID7,"VC7_pvid")
   
   TCWebApi_set(VLAN_ETHERNETPVID1,"Ether0_pvid")
   TCWebApi_set(VLAN_ETHERNETPVID2,"Ether1_pvid")
   TCWebApi_set(VLAN_ETHERNETPVID3,"Ether2_pvid")
   TCWebApi_set(VLAN_ETHERNETPVID4,"Ether3_pvid")
   
   TCWebApi_set(VLAN_USBPVID4,"USB_pvid")
   
   TCWebApi_execute(VLAN_PVIDSETTING)
  
End If

If Request.Form("vlan_pvid_next") = "NEXT" Then

   TCWebApi_set(VLAN_ATMPVID0,"VC0_pvid")
   TCWebApi_set(VLAN_ATMPVID1,"VC1_pvid")
   TCWebApi_set(VLAN_ATMPVID2,"VC2_pvid")
   TCWebApi_set(VLAN_ATMPVID3,"VC3_pvid")
   TCWebApi_set(VLAN_ATMPVID4,"VC4_pvid")
   TCWebApi_set(VLAN_ATMPVID5,"VC5_pvid")
   TCWebApi_set(VLAN_ATMPVID6,"VC6_pvid")
   TCWebApi_set(VLAN_ATMPVID7,"VC7_pvid")
   
   TCWebApi_set(VLAN_ETHERNETPVID1,"Ether0_pvid")
   TCWebApi_set(VLAN_ETHERNETPVID2,"Ether1_pvid")
   TCWebApi_set(VLAN_ETHERNETPVID3,"Ether2_pvid")
   TCWebApi_set(VLAN_ETHERNETPVID4,"Ether3_pvid")
   
   TCWebApi_set(VLAN_USBPVID4,"USB_pvid")
   
   TCWebApi_execute(VLAN_PVIDNEXT)
End If
%>

<html><head>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="javascript" type="text/javascript">
//<![CDATA[
function verifyForm()
{
	var form = document.VLANPVID_form;
	if(form.ICIndex.value == 0)
	{
		for(i = 2; i < 11; i++)
		{
			var pvid = parseInt(form.elements[i].value);
			if(isNaN(pvid) || pvid < 1 || pvid > 8)
			{
				alert("<%tcWebApi_get("String_Entry","VLANPvidJS0Text","s")%>");
				form.elements[i].focus();
				return false;
			}
		}
		pvid = parseInt(form.Ether0_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS0Text","s")%>");
			form.Ether0_pvid.focus();
			return false;
		}
				pvid = parseInt(form.Ether1_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS0Text","s")%>");
			form.Ether1_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS0Text","s")%>");
			form.Ether2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS0Text","s")%>");
			form.Ether3_pvid.focus();
			return false;
		}
						pvid = parseInt(form.USB_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS0Text","s")%>");
			form.USB_pvid.focus();
			return false;
		}
					}
	else
	{
		for(i = 2; i < 11; i++)
		{
			var pvid = parseInt(form.elements[i].value);
			if(isNaN(pvid) || pvid < 1 || pvid > 4094)
			{
				alert("<%tcWebApi_get("String_Entry","VLANPvidJS1Text","s")%>");
				form.elements[i].focus();
				return false;
			}
		}
		pvid = parseInt(form.Ether0_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS1Text","s")%>");
			form.Ether0_pvid.focus();
			return false;
		}
				pvid = parseInt(form.Ether1_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS1Text","s")%>");
			form.Ether1_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS1Text","s")%>");
			form.Ether2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS1Text","s")%>");
			form.Ether3_pvid.focus();
			return false;
		}
						pvid = parseInt(form.USB_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("<%tcWebApi_get("String_Entry","VLANPvidJS1Text","s")%>");
			form.USB_pvid.focus();
			return false;
		}
					}
	return true;		
}
//]]>
</script>
</head><body>

<FORM METHOD="POST" ACTION="/cgi-bin/adv_vlan_pvid.asp" name="VLANPVID_form" onsubmit="return verifyForm()"><INPUT TYPE="HIDDEN" NAME="ICIndex" VALUE="0"><INPUT TYPE="HIDDEN" NAME="Apply_or_Next" VALUE="0"><div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF"><%tcWebApi_get("String_Entry","VLANPvidAssignText","s")%></font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC0Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC0_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID0) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC1Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC1_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID1) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC2Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC2_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID2) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC3Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC3_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID3) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC4Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC4_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID4) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC5Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC5_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID5) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC6Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC6_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID6) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidATMVC7Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="VC7_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID7) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidEthPort1Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="Ether0_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID1) %> >    </td></tr>  <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidEthPort2Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="Ether1_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID2) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidEthPort3Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="Ether2_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID3) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidEthPort4Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>	<INPUT TYPE="TEXT" NAME="Ether3_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID4) %> >    </td></tr>    <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidUSBText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANPvidText","s")%></font>    <INPUT TYPE="TEXT" NAME="USB_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_USBPVID4) %> >	</td></tr>    <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
    	<INPUT TYPE="SUBMIT" NAME="vlan_pvid_apply" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onclick="document.VLANPVID_form.Apply_or_Next.value = 0;">    	<input type="reset"  name="vlan_pvid_cancel" value="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>">        <INPUT TYPE="SUBMIT" NAME="vlan_pvid_next" VALUE="<%tcWebApi_get("String_Entry","ButtonNextText","s")%>" onclick="document.VLANPVID_form.Apply_or_Next.value = 1;">    </td></tr></table></div></form>
</body></html>

