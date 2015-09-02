<%@SCRIPT LANGUAGE="VBSCRIPT"%>

<%
	TcWebApi_set(VLAN_STATUS,"Vlan_active")	
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">

<style type="text/css">
</style>
<script language='javascript'>
var linkable = true;
function VlanPvidLink()
{
	if(linkable)
		window.location="/cgi-bin/adv_vlan_pvid.asp";
}
function VlanGroupLink()
{
	if(linkable)
		window.location="/cgi-bin/adv_vlan_group.asp";
}
function vlanSwitch(on_off)
{
	if(on_off == 0)
		linkable = false;
	else
		linkable = true;
}
</script>
</head><body onclick="if(!document.Vlan_form.elements[0].checked) vlanSwitch(0);">

<FORM METHOD="POST" ACTION="/cgi-bin/adv_vlan_top.asp" name="Vlan_form">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF"><%tcWebApi_get("String_Entry","VLANTopText","s")%></font>    </td><td width="10" class="black">&nbsp;</td><td width="150"><hr noshade></td><td width="10" ><hr noshade></td><td width="440"><hr noshade></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td height="30" class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","VLANTopFunctionText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="RADIO" NAME="Vlan_active" VALUE="1" <%TcWebApi_get(VLAN_ACTIVATED)%> onclick="submit()">	   <font color="#000000"><%tcWebApi_get("String_Entry","VLANTopActivatedText","s")%></font>       
        <INPUT TYPE="RADIO" NAME="Vlan_active" VALUE="0" <%TcWebApi_get(VLAN_DEACTIVATED)%> onclick="submit()">   <font color="#000000"><%tcWebApi_get("String_Entry","VLANTopDeactivatedText","s")%></font>        </td></tr><tr>  
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td height="30"><div align=right><img src="/next.gif" width="16" height="16"></div></td><td>&nbsp;</td><td class="content"> <a href="javascript:VlanPvidLink();" target="main">
    <font color="#666666"><%tcWebApi_get("String_Entry","VLANTopCommentText","s")%></font>    </a></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td height="30"><div align=right><img src="/next.gif" width="16" height="16"></div></td><td>&nbsp;</td><td class="content" ><a href="javascript:VlanGroupLink();" target="main">
    <font color="#666666"><%tcWebApi_get("String_Entry","VLANTopDefineText","s")%></font>    </a></td></tr><tr>
    <td height="20" class="light-orange">&nbsp;</td><td class="light-orange"></td><td height="30">&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange"></td></tr></table><p>&nbsp;</p>
</div></form>
</body></html>
