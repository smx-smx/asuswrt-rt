<%
	TCWebApi_set("RoutePolicy_Entry","startIP","StartSourceIPAddr")
	TCWebApi_set("RoutePolicy_Entry","endIP","EndSourceIPAddr")		
	TCWebApi_set("RoutePolicy_Entry","TOS","TOS")
	TCWebApi_set("RoutePolicy_Entry","Gateway","GateWay")

If request_Form("RPSaveFlag") = "1" Then
	tcWebApi_commit("RoutePolicy_Entry")

End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" href="/style.css" tppabs="/style.css" type="text/css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language="JavaScript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/jsl.js"></script>
<script language="JavaScript" type="text/javascript" src="/val.js"></script>
<script language="JavaScript" type="text/javascript" src="/pvc.js"></script>
<script language="JavaScript" type="text/javascript" src="/ip.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script>

function uiTOSCheck(){
	var value;
	
	value = document.uiRoutePolicyForm.TOS.value;
		if(parseInt(value) > 5  || parseInt(value) < 0)
		{
			alert('<%tcWebApi_get("String_Entry","RtPolicyJS0Text","s")%>');
			return false;
		}
		
	return true;
	
}


function uiSave() {
	var form = document.uiRoutePolicyForm;
	var value1;
	var value2;
	var value3;
	
	value1=form.StartSourceIPAddr.value;
	value2=form.EndSourceIPAddr.value;
	value3=form.GateWay.value;
	
	if(inValidIPAddr(value1))
	{
        return false;
	}
	
	if(inValidIPAddr(value2))
	{
        return false;
	}
	
	if(inValidIPAddr(value3))
	{
        return false;
	}
	
		
	if(uiTOSCheck())
	{
		form.RPSaveFlag.value = 1;
		form.submit();
	}
}

</script>
</head><body>



<FORM METHOD="POST" ACTION="/cgi-bin/adv_routepolicy.asp" name="uiRoutePolicyForm">
  <div align=center>
    <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
        <td height="5" class="light-orange" colspan="5">&nbsp;</td>
      </tr>
      <tr> 
        <td width="150" height="30" class="title-main"> <font color="#FFFFFF"><%tcWebApi_get("String_Entry","RtPolicyText","s")%></font> </td>
        <td width="10" class="black"></td>
        <td width="150"></td>
        <td width="10" ></td>
        <td width="440"></td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000"><%tcWebApi_get("String_Entry","RtPolicyStaSrcIPText","s")%></font> </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="StartSourceIPAddr" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("RoutePolicy_Entry","startIP","h") <> "N/A" then tcWebApi_get("RoutePolicy_Entry","startIP","s") end if%>" >
        </td>
      </tr>
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"> <div align=right> <font color="#000000"><%tcWebApi_get("String_Entry","RtPolicyEndSrcIPText","s")%></font> </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="EndSourceIPAddr" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("RoutePolicy_Entry","endIP","h") <> "N/A" then tcWebApi_get("RoutePolicy_Entry","endIP","s") end if%>" >	
        </td>
      </tr>
	  <!--
      <tr> 
        <td height="24" class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000"> IP Precedence</font> 
          </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="IP_Precedence" SIZE="15" MAXLENGTH="15" VALUE="" >	
      </tr>
	  -->
      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"> <div align=right> <font color="#000000"><%tcWebApi_get("String_Entry","RtPolicyTOSText","s")%></font> 
          </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="TOS" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("RoutePolicy_Entry","TOS","h") <> "N/A" then tcWebApi_get("RoutePolicy_Entry","TOS","s") end if%>" >
          <%tcWebApi_get("String_Entry","RtPolicyTipText","s")%>
		</td>
      </tr>
    </table>
    <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr> 
        <td width="150" height="30" class="title-main"> <font color="#FFFFFF"><%tcWebApi_get("String_Entry","RtPolicyPacFwdText","s")%></font> </td>
        <td width="10" class="black">&nbsp;</td>
        <td width="150"><hr noshade></td>
        <td width= "12"><hr noshade></td>
        <td width="440"><hr noshade> </td>
      </tr>

      <tr> 
        <td class="light-orange">&nbsp;</td>
        <td class="light-orange">&nbsp;</td>
        <td class="tabdata"><div align=right> <font color="#000000"><%tcWebApi_get("String_Entry","RtPolicyGatewayText","s")%></font> 
          </div></td>
        <td class="tabdata"><div align=center>:</div></td>
        <td class="tabdata"> <INPUT TYPE="TEXT" NAME="GateWay" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("RoutePolicy_Entry","Gateway","h") <>"N/A" then tcWebApi_get("RoutePolicy_Entry","Gateway","s") end if%>" >	
        </td>
      </tr>
    </table>
  

 	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" class="light-orange">&nbsp;</td><td width="10"  class="light-orange"></td><td width="150">&nbsp;</td><td width="10" >&nbsp;</td><td width="440">&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr height="42" >
    <td width="160" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
        <INPUT TYPE="button" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="uiSave()">
          <input type="HIDDEN" name="RPSaveFlag" value="0"> 
          <INPUT TYPE="reset" NAME="lan_cancel" VALUE="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>" ></td></tr></table></div></form></body>
</html>
