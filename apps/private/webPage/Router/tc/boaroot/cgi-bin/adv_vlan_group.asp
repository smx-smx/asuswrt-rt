<%@SCRIPT LANGUAGE="VBSCRIPT"%>


<%  
	TCWebApi_set(VLAN_INDEX,"VlanGroup_index")
If Request.Form("SaveBtn_vlan_group") = "SAVE" Then
   TCWebApi_set(VLAN_INDEX,"VlanGroup_index")
   TCWebApi_set(VLAN_ACTIVE,"VlanGroup_active")
   TCWebApi_set(VLAN_ID,"VlanGroup_id")
   
   TCWebApi_set(VLAN_ATMTAGGED0,"VlanGroup_ATM_Tagged_0")
   TCWebApi_set(VLAN_ATMTAGGED1,"VlanGroup_ATM_Tagged_1")
   TCWebApi_set(VLAN_ATMTAGGED2,"VlanGroup_ATM_Tagged_2")
   TCWebApi_set(VLAN_ATMTAGGED3,"VlanGroup_ATM_Tagged_3")
   TCWebApi_set(VLAN_ATMTAGGED4,"VlanGroup_ATM_Tagged_4")
   TCWebApi_set(VLAN_ATMTAGGED5,"VlanGroup_ATM_Tagged_5")
   TCWebApi_set(VLAN_ATMTAGGED6,"VlanGroup_ATM_Tagged_6")
   TCWebApi_set(VLAN_ATMTAGGED7,"VlanGroup_ATM_Tagged_7")
   
   TCWebApi_set(VLAN_ATMPORT0,"VlanGroup_ATM_Port_0")
   TCWebApi_set(VLAN_ATMPORT1,"VlanGroup_ATM_Port_1")
   TCWebApi_set(VLAN_ATMPORT2,"VlanGroup_ATM_Port_2")
   TCWebApi_set(VLAN_ATMPORT3,"VlanGroup_ATM_Port_3")
   TCWebApi_set(VLAN_ATMPORT4,"VlanGroup_ATM_Port_4")
   TCWebApi_set(VLAN_ATMPORT5,"VlanGroup_ATM_Port_5")
   TCWebApi_set(VLAN_ATMPORT6,"VlanGroup_ATM_Port_6")
   TCWebApi_set(VLAN_ATMPORT7,"VlanGroup_ATM_Port_7")
   
   TCWebApi_set(VLAN_ETHERNETTAGGED1,"VlanGroup_Ethernet_Tagged_1")
   TCWebApi_set(VLAN_ETHERNETTAGGED2,"VlanGroup_Ethernet_Tagged_2")
   TCWebApi_set(VLAN_ETHERNETTAGGED3,"VlanGroup_Ethernet_Tagged_3")
   TCWebApi_set(VLAN_ETHERNETTAGGED4,"VlanGroup_Ethernet_Tagged_4")
   
   TCWebApi_set(VLAN_ETHERNETPORT1,"VlanGroup_Ethernet_Port_1")
   TCWebApi_set(VLAN_ETHERNETPORT2,"VlanGroup_Ethernet_Port_2")
   TCWebApi_set(VLAN_ETHERNETPORT3,"VlanGroup_Ethernet_Port_3")
   TCWebApi_set(VLAN_ETHERNETPORT4,"VlanGroup_Ethernet_Port_4")
   
   TCWebApi_set(VLAN_USBTAGGED1,"VlanGroup_USB_Tagged_1")
   TCWebApi_set(VLAN_USBPORT1,"VlanGroup_USB_Port_1")

   TCWebApi_execute(VLAN_GROUPSETTING)
   
End If

If Request.Form("DeleteBtn_vlan_group") = "DELETE" Then
   TCWebApi_execute(VLAN_GROUPDELETE)
End If



%>


<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">

<script language="JavaScript">

function init()
{
	var form = document.VlanGroup_form;
	form.Apply_or_Delete_or_Update.value = 0;
	if(form.ICIndex_Group.value == 0)
		form.VlanGroup_id.disabled = true;
	for(i = 4; i < 12; i++)
		if(!form.elements[i + 8].checked || form.ICIndex_Group.value == 0)
			form.elements[i].disabled = true;
	if(!form.VlanGroup_Ethernet_Port_1.checked || form.ICIndex_Group.value == 0)
		form.VlanGroup_Ethernet_Tagged_1.disabled = true;
		if(!form.VlanGroup_Ethernet_Port_2.checked || form.ICIndex_Group.value == 0)
		form.VlanGroup_Ethernet_Tagged_2.disabled = true;
	if(!form.VlanGroup_Ethernet_Port_3.checked || form.ICIndex_Group.value == 0)
		form.VlanGroup_Ethernet_Tagged_3.disabled = true;
	if(!form.VlanGroup_Ethernet_Port_4.checked || form.ICIndex_Group.value == 0)
		form.VlanGroup_Ethernet_Tagged_4.disabled = true;
			if(!form.VlanGroup_USB_Port_1.checked || form.ICIndex_Group.value == 0)
		form.VlanGroup_USB_Tagged_1.disabled = true;
		}

function atmOnOffSwitch(port)
{
	if(document.VlanGroup_form.ICIndex_Group.value != 1)
		return false;
	var Tag = document.getElementById("VlanGroup_ATM_Tagged_" + port);
	if(Tag.disabled == true)
		Tag.disabled = false;
	else
	{
		Tag.disabled = true;
		Tag.checked = false;
	}
}

function etherOnOffSwitch(port)
{
	if(document.VlanGroup_form.ICIndex_Group.value != 1)
		return false;
	var Tag = document.getElementById("VlanGroup_Ethernet_Tagged_" + port);
	if(Tag.disabled == true)
		Tag.disabled = false;
	else
	{
		Tag.disabled = true;
		Tag.checked = false;
	}
}

function usbWirelessSwitch(index)
{
	if(document.VlanGroup_form.ICIndex_Group.value != 1)
		return false;
	if(index == 28)
		Tag = document.VlanGroup_form.VlanGroup_USB_Tagged_1;
	else if(index == 30)
		Tag = document.VlanGroup_form.VlanGroup_WLAN_Tagged_1;
	if(Tag.disabled == true)
		Tag.disabled = false;
	else
	{
		Tag.disabled = true;
		Tag.checked = false;
	}

}

function vidCheck() 
{
	var vid = parseInt(document.VlanGroup_form.VlanGroup_id.value);
	if(document.VlanGroup_form.ICIndex_Group.value == 1)
	{
		if(isNaN(vid) || vid < 1 || vid > 4094)
		{
			alert("The range of pvid value is 1 ~ 4094");
			document.VlanGroup_form.VlanGroup_id.focus();
			return false;
		}
	}
	var form = document.VlanGroup_form;
	for(i = 12; i <= 19; i++)
		if(form.elements[i].checked == true)
			return true;
	if(form.VlanGroup_Ethernet_Port_1.checked == true)
		return true;
		if(form.VlanGroup_Ethernet_Port_2.checked == true)
		return true;
	if(form.VlanGroup_Ethernet_Port_3.checked == true)
		return true;
	if(form.VlanGroup_Ethernet_Port_4.checked == true)
		return true;
			if(form.VlanGroup_USB_Port_1.checked == true)
		return true;
			alert("Not a valid action");
	return false;
}
</script>
</head><body leftmargin="20" topmargin="0" rightmargin="15" bottommargin="0" marginwidth="0" marginheight="0" onload="init()">

<FORM METHOD="POST" ACTION="/cgi-bin/adv_vlan_group.asp" name="VlanGroup_form" onsubmit="return vidCheck()">
<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">VLAN Group Setting</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td width="440"><hr noshade></td></tr><tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td width="10" class="light-orange"></td><td height="30" class="tabdata"><div align=right>
    <font color="#000000">VLAN Index</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <SELECT NAME="VlanGroup_index" SIZE="1" onchange="document.VlanGroup_form.Apply_or_Delete_or_Update.value = 2; document.VlanGroup_form.submit();">
<%TCWebApi_get(VLAN_INDEX)%>
</SELECT>        </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">Active</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    	<INPUT TYPE="RADIO" NAME="VlanGroup_active" VALUE="Yes" <% TCWebApi_get(VLAN_ACTIVEYES) %> >        <font color="#000000">Yes</font>
    	<INPUT TYPE="RADIO" NAME="VlanGroup_active" VALUE="No" <% TCWebApi_get(VLAN_ACTIVENO) %> >        <font color="#000000">No</font>        </td></tr><tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td width="10" class="light-orange">&nbsp;</td><td width="150" height="30" class="tabdata"> <div align=right>
    <font color="#000000">VLAN ID</font>    </div></td><td width="10" class="tabdata"><div align=center>:</div></td><td width="440" class="tabdata">
        <INPUT TYPE="TEXT" NAME="VlanGroup_id" SIZE="9" MAXLENGTH="9" VALUE=<% TCWebApi_get(VLAN_ID) %> >        <font color="#000000">(Decimal)</font>        </td></tr><tr>
    <td width="150" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" class="tabdata"> <div align=right>
    <font color="#000000">ATM VCs</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="80" class="tabdata">
      <table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr class="tabdata">
        <td class="tabdata"><div align=right>
        <font color="#000000">Tagged</font>        </div></td><td class="tabdata"><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_0" <% TCWebApi_get(VLAN_ATMTAGGED0) %> id="VlanGroup_ATM_Tagged_0">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_1" <% TCWebApi_get(VLAN_ATMTAGGED1) %> id="VlanGroup_ATM_Tagged_1">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_2" <% TCWebApi_get(VLAN_ATMTAGGED2) %> id="VlanGroup_ATM_Tagged_2">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_3" <% TCWebApi_get(VLAN_ATMTAGGED3) %> id="VlanGroup_ATM_Tagged_3">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_4" <% TCWebApi_get(VLAN_ATMTAGGED4) %> id="VlanGroup_ATM_Tagged_4">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_5" <% TCWebApi_get(VLAN_ATMTAGGED5) %> id="VlanGroup_ATM_Tagged_5">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_6" <% TCWebApi_get(VLAN_ATMTAGGED6) %> id="VlanGroup_ATM_Tagged_6">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Tagged_7" <% TCWebApi_get(VLAN_ATMTAGGED7) %> id="VlanGroup_ATM_Tagged_7">            </div></td></tr><tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_0" <% TCWebApi_get(VLAN_ATMPORT0) %>  onclick="atmOnOffSwitch(0)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_1" <% TCWebApi_get(VLAN_ATMPORT1) %>  onclick="atmOnOffSwitch(1)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_2" <% TCWebApi_get(VLAN_ATMPORT2) %>  onclick="atmOnOffSwitch(2)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_3" <% TCWebApi_get(VLAN_ATMPORT3) %>  onclick="atmOnOffSwitch(3)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_4" <% TCWebApi_get(VLAN_ATMPORT4) %>  onclick="atmOnOffSwitch(4)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_5" <% TCWebApi_get(VLAN_ATMPORT5) %>  onclick="atmOnOffSwitch(5)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_6" <% TCWebApi_get(VLAN_ATMPORT6) %>  onclick="atmOnOffSwitch(6)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_ATM_Port_7" <% TCWebApi_get(VLAN_ATMPORT7) %>  onclick="atmOnOffSwitch(7)">            </div></td></tr><tr class="tabdata">
        <td class="tabdata"><div align=center>0</div></td><td class="tabdata"><div align=center>1</div></td><td class="tabdata"><div align=center>2</div></td><td class="tabdata"><div align=center>3</div></td><td class="tabdata"><div align=center>4</div></td><td class="tabdata"><div align=center>5</div></td><td class="tabdata"><div align=center>6</div></td><td class="tabdata"><div align=center>7</div></td></tr></table></td></tr><tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td width="150" height="45" class="tabdata"> <div align=right>
    <font color="#000000">Ethernet</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td width="440" height="70" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr>
        <td class="tabdata"><div align=right>
		<font color="#000000">Tagged</font>		</div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Tagged_1" <% TCWebApi_get(VLAN_ETHERNETTAGGED1) %> id="VlanGroup_Ethernet_Tagged_1">            </div></td>        <td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Tagged_2" <% TCWebApi_get(VLAN_ETHERNETTAGGED2) %> id="VlanGroup_Ethernet_Tagged_2">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Tagged_3" <% TCWebApi_get(VLAN_ETHERNETTAGGED3) %> id="VlanGroup_Ethernet_Tagged_3">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Tagged_4" <% TCWebApi_get(VLAN_ETHERNETTAGGED4) %> id="VlanGroup_Ethernet_Tagged_4">            </div></td>        </tr><tr>
        <td rowspan="2" class="tabdata"><div align=right>
        <font color="#000000">Port #</font>        </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Port_1" <% TCWebApi_get(VLAN_ETHERNETPORT1) %> onclick="etherOnOffSwitch(1)">            </div></td>        <td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Port_2" <% TCWebApi_get(VLAN_ETHERNETPORT2) %> onclick="etherOnOffSwitch(2)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Port_3" <% TCWebApi_get(VLAN_ETHERNETPORT3) %> onclick="etherOnOffSwitch(3)">            </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_Ethernet_Port_4" <% TCWebApi_get(VLAN_ETHERNETPORT4) %> onclick="etherOnOffSwitch(4)">            </div></td>      </tr><tr>
        <td class="tabdata"><div align=center>1</div></td>        <td class="tabdata"><div align=center>2</div></td><td class="tabdata"><div align=center>3</div></td><td class="tabdata"><div align=center>4</div></td>      </tr></table></td></tr>  <tr>
    <td height="25" class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td height="80" class="tabdata">
      <div align=right>
      <font color="#000000">USB</font>      </div></td><td class="tabdata"><div align=center>:</div></td><td height="25" class="tabdata"><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
      <tr class="tabdata">
        <td class="tabdata"><div align=right>
		<font color="#000000">Tagged</font>		</div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_USB_Tagged_1" <% TCWebApi_get(VLAN_USBTAGGED1) %> >        </div></td></tr><tr class="tabdata">
        <td rowspan="2" class="tabdata">
          <div align=right>
          <font color="#000000">Port #</font>          </div></td><td><div align=center>
            <INPUT TYPE="CHECKBOX" NAME="VlanGroup_USB_Port_1" <% TCWebApi_get(VLAN_USBPORT1) %> onclick="usbWirelessSwitch(28)">        </div></td></tr></table></td></tr>    <tr>
    <td height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td height="30">&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" height="30" class="title-sub">
    <font color="#000000">VLAN Group Summary</font>    </td><td width="10" class="light-orange">&nbsp;</td><td width="10" ><hr noshade class="light-orange-line"></td><td width="580"><hr noshade class="light-orange-line"></td><td width="10" ><hr noshade class="light-orange-line"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td><table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
      <tr height="30">
        <td width="30"  align=center class="tabdata"><strong>
        <font color="#000000">Grp</font>        </strong></td><td width="45"  align=center class="tabdata"><strong>
        <font color="#000000">Active</font>        </strong></td><td width="40"  align=center class="tabdata"><strong>
        <font color="#000000">ID</font>        </strong></td><td width="225" align=center class="tabdata"><strong>
        <font color="#000000">VLAN Group Ports</font>        </strong></td><td width="225" align=center class="tabdata"><strong>
        <font color="#000000">VLAN Tagged Ports</font>        </strong> </td></tr>
        
<%TCWebApi_get(VLAN_SUMMARY)%>
        </table></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td valign="bottom" class="tabdata">p:pvc, e:ethernet, u:usb    </td><td>&nbsp;</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<INPUT TYPE="SUBMIT" NAME="SaveBtn_vlan_group" VALUE="SAVE">
	<INPUT TYPE="SUBMIT" NAME="DeleteBtn_vlan_group" VALUE="DELETE" onclick="document.VlanGroup_form.Apply_or_Delete_or_Update.value = 1">
	<input type="reset"  name="vlan_group_cancel" value="CANCEL" onclick="init()">
	</td></tr></table></div>
	<INPUT TYPE="HIDDEN" NAME="ICIndex_Group" VALUE="1">
	<INPUT TYPE="HIDDEN" NAME="Apply_or_Delete_or_Update" VALUE="0"></form>
</body></html>