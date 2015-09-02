<%
If Request_Form("adminFlag") = "1" Then
	TCWebApi_set("Account_Entry0","web_passwd","uiViewTools_Password")
	TCWebApi_set("Account_Entry0","console_passwd","uiViewTools_Password")
	tcWebApi_commit("Account_Entry0")
End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script language="JavaScript" src="/jsl.js"></script>
<script language="JavaScript" src="/ip.js"></script>

<script language="JavaScript">
function uiSave() {
    if (document.tool_admin.uiViewTools_Password.value.length == 0) {
    	alert("Empty Password Invaild. ");
    	return;
    }
	<%if tcwebApi_get("WebCustom_Entry", "isCZTMSupported", "h")="Yes" Then%>
	var password = document.tool_admin.uiViewTools_Password.value;
	if (password.length < 8 || password.length > 30) {
    	alert("Password Invalid.The length of password should be 8~30 character(s)!");
		return -1;
	}
	
	for(var i = 0; i < password.length; i ++ )
	{
	   var ascNum = password.charCodeAt(i);
	   if (ascNum < 33 || ascNum > 126 || ascNum == 59){
   			alert("Password Invalid.");
			return -1;
		}
	}
	<%end if%>
    if (document.tool_admin.uiViewTools_Password.value != document.tool_admin.uiViewTools_PasswordConfirm.value) {
        alert("Your Password and Confirm Password must match before you can apply. ");
        return;
    }
    if(quotationCheck(document.tool_admin.uiViewTools_Password, 30) ) 
			return;
    document.tool_admin.adminFlag.value=1;
    document.tool_admin.submit();
    return;
}

function quotationCheck(object, limit_len) {
	var len = object.value.length;
	var c;
	var i, j = 0;
    for (i = 0; i < len; i++)
    {
	 	var c = object.value.charAt(i);
      
	  	if (c == '"')
		{
			j += 6;
		}
		else
			j++;
    }
   	if (j > limit_len)
	{
   	alert('Input too many character!!');      	  								    	    	   		
		return true;
	}	
	return false;
}
</script>
</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/tools_admin.asp" name="tool_admin">
<INPUT TYPE="HIDDEN" NAME="adminFlag" VALUE="0">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">Administrator</font></td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"> </td><td width="440"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Username</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" style="word-wrap:break-word;word-break:break-all;"><strong>
    <font color="#000000">
	<% tcWebApi_get("Account_Entry0","username","s") %>
	</font>    </strong></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">New Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="uiViewTools_Password" SIZE="32" MAXLENGTH="30" VALUE="">    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Confirm Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <INPUT TYPE="PASSWORD" NAME="uiViewTools_PasswordConfirm" SIZE="32" MAXLENGTH="30" VALUE="">    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
	<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="uiSave()">        <INPUT TYPE="BUTTON" NAME="CancelBtn" VALUE="CANCEL" onClick="javascript:window.location='/cgi-bin/tools_admin.asp'">        </td></tr></table></div></form>
</body></html>
