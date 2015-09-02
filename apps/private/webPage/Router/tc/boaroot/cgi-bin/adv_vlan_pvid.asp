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
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
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
				alert("The range of pvid value is 1 ~ 8");
				form.elements[i].focus();
				return false;
			}
		}
		pvid = parseInt(form.Ether0_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("The range of pvid value is 1 ~ 8");
			form.Ether0_pvid.focus();
			return false;
		}
				pvid = parseInt(form.Ether1_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("The range of pvid value is 1 ~ 8");
			form.Ether1_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("The range of pvid value is 1 ~ 8");
			form.Ether2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("The range of pvid value is 1 ~ 8");
			form.Ether3_pvid.focus();
			return false;
		}
						pvid = parseInt(form.USB_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 8)
		{
			alert("The range of pvid value is 1 ~ 8");
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
				alert("The range of pvid value is 1 ~ 4094");
				form.elements[i].focus();
				return false;
			}
		}
		pvid = parseInt(form.Ether0_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");
			form.Ether0_pvid.focus();
			return false;
		}
				pvid = parseInt(form.Ether1_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");
			form.Ether1_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether2_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");
			form.Ether2_pvid.focus();
			return false;
		}
		pvid = parseInt(form.Ether3_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");
			form.Ether3_pvid.focus();
			return false;
		}
						pvid = parseInt(form.USB_pvid.value);
		if(isNaN(pvid) || pvid < 1 || pvid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");
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
    <font color="#FFFFFF">PVID Assign</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">ATM VC #0</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC0_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID0) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">VC #1</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC1_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID1) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">VC #2</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC2_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID2) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">VC #3</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC3_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID3) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">VC #4</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC4_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID4) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">VC #5</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC5_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID5) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">VC #6</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC6_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID6) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">VC #7</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="VC7_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ATMPVID7) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Ethernet Port #1</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="Ether0_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID1) %> >    </td></tr>  <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Port #2</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="Ether1_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID2) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Port #3</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="Ether2_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID3) %> >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Port #4</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>	<INPUT TYPE="TEXT" NAME="Ether3_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_ETHERNETPVID4) %> >    </td></tr>    <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td><td class="tabdata"><hr noshade class="light-orange-line"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">USB</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <font color="#000000">PVID</font>    <INPUT TYPE="TEXT" NAME="USB_pvid" SIZE="6" MAXLENGTH="4" VALUE=<% TCWebApi_get(VLAN_USBPVID4) %> >	</td></tr>    <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
    	<INPUT TYPE="SUBMIT" NAME="vlan_pvid_apply" VALUE="SAVE" onclick="document.VLANPVID_form.Apply_or_Next.value = 0;">    	<input type="reset"  name="vlan_pvid_cancel" value="CANCEL">        <INPUT TYPE="SUBMIT" NAME="vlan_pvid_next" VALUE="NEXT" onclick="document.VLANPVID_form.Apply_or_Next.value = 1;">    </td></tr></table></div></form>
</body></html>

