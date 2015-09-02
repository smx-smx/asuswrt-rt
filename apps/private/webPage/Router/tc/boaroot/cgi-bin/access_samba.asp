<%
If Request_Form("Sambaflag")="1" then 
	TCWebApi_set("Samba_Entry","Active","Samba_active")
	if TCWebAPI_get("Samba_Entry", "Active", "h") = "Yes" then
		TCWebAPI_set("Samba_Entry","Passwd","newSambaPassword")
		TCWebAPI_set("Samba_Entry","WorkGroup","WorkGroup")
		TCWebAPI_set("Samba_Entry","NetBiosName","NetBiosName")
	end if
	/*commit changes*/
	TCWebAPI_commit("Samba_Entry")	
End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">

<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript">
function isValidNameEx(name) {
   var i = 0;	
   
   for ( i = 0; i < name.length; i++ ) {
      if ( isNameUnsafeEx(name.charAt(i)) == true ){
		return false;
	}
   }

   return true;
}

function isNameUnsafeEx(compareChar)
{	
   if ( compareChar.charCodeAt(0) > 32
        && compareChar.charCodeAt(0) < 127)
      return false; // found no unsafe chars, return false
   else
      return true;
}	

function Sambasave() 
{
	if(document.Samba_form.Samba_active[0].checked)
	{
		if(isValidNameEx(document.Samba_form.newSambaPassword.value) == false ){
			alert("new password invalid!");
			document.Samba_form.newSambaPassword.focus();
			return false;
		}
		if(isValidNameEx(document.Samba_form.confirmSambaPassword.value) == false ){
			document.Samba_form.confirmSambaPassword.focus();
			alert("confirm password invalid!");
			return false;
		}
		if(isValidNameEx(document.Samba_form.WorkGroup.value) == false ){
			document.Samba_form.WorkGroup.focus();
			alert("Work Group invalid!");
			return false;
		}
		if(isValidNameEx(document.Samba_form.NetBiosName.value) == false ){
			document.Samba_form.NetBiosName.focus();
			alert("Net NIOS Name invalid!");
			return false;
		}
		if((document.Samba_form.newSambaPassword.value.length == 0)||(document.Samba_form.newSambaPassword.value.length >15))
		{
			alert('The length of password should between 1 and 15');
			document.Samba_form.newSambaPassword.focus();
			return false;
		}
		if((document.Samba_form.confirmSambaPassword.value.length == 0)||(document.Samba_form.confirmSambaPassword.value.length > 15))
		{
			alert('The length of password should between 1 and 15');
			document.Samba_form.confirmSambaPassword.focus();
			return false;
		}
		if((document.Samba_form.WorkGroup.value.length == 0)||(document.Samba_form.WorkGroup.value.length > 15))
		{
			alert('The length of WorkGroup should between 1 and 15');
			document.Samba_form.WorkGroup.focus();
			return false;
		}
		if((document.Samba_form.NetBiosName.value.length == 0)||(document.Samba_form.NetBiosName.value.length > 15))
		{
			alert('The length of NetBiosName should between 1 and 15');
			document.Samba_form.NetBiosName.focus();
			return false;
		}

		if (document.Samba_form.newSambaPassword.value != document.Samba_form.confirmSambaPassword.value) 
		{
        	alert("Your Password and Confirm Password must match before you can apply. ");
        	return false;
    	}
	}
	   
  document.Samba_form.Sambaflag.value=1;
  document.Samba_form.submit();
}

function sambaOff(off){
	if(off){
  	document.Samba_form.newSambaPassword.disabled = true;
	document.Samba_form.confirmSambaPassword.disabled = true;
	document.Samba_form.WorkGroup.disabled = true;
	document.Samba_form.NetBiosName.disabled = true;	
	}
	else{
	document.Samba_form.newSambaPassword.disabled = false;
	document.Samba_form.confirmSambaPassword.disabled = false;
	document.Samba_form.WorkGroup.disabled = false;
	document.Samba_form.NetBiosName.disabled = false;		
	}
}
</script>
</head><body onLoad="if(document.Samba_form.elements[1].checked) sambaOff(1); else sambaOff(0)">
<FORM METHOD="POST" ACTION="/cgi-bin/access_samba.asp" name="Samba_form">
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
    <tr> 
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
    </tr>
    <tr> 
      <td width="150" height="30" class="title-main"> <font color="#FFFFFF">Samba 
        Server</font> </td>
      <td width="10" class="black">&nbsp;</td>
      <td width="150"></td>
      <td width="10" ></td>
      <td width="440"></td>
    </tr>
    <tr> 
      <td width="150" height="25" class="light-orange">&nbsp;</td>
      <td width="10" class="light-orange"></td>
      <td class="tabdata"><div align=right><font color="#000000">Samba</font></div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <INPUT TYPE="RADIO" NAME="Samba_active"  VALUE="Yes"  onClick="sambaOff(0);" <% If TCWebApi_get("Samba_Entry","Active","h") = "Yes" then asp_Write("checked") end if%> >
        <font color="#000000">Activated</font> <INPUT TYPE="RADIO" NAME="Samba_active"  VALUE="No"  onClick="sambaOff(1);" <% If TCWebApi_get("Samba_Entry","Active","h") = "No" then asp_Write("checked") end if%> >
        <font color="#000000">Deactivated</font> </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <font color="#000000">Username</font> 
        </div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <strong><font color="#000000"> 
        <% tcWebApi_get("Account_Entry0","username","s") %>
        </font></strong> </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <font color="#000000">New Password</font> 
        </div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <INPUT TYPE="PASSWORD" NAME="newSambaPassword" SIZE="32" MAXLENGTH="31" VALUE="<% If TCWebApi_get("Samba_Entry","Passwd","h") <> "N/A" then TCWebApi_get("Samba_Entry","Passwd","s") end if%>" > 
      </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <font color="#000000">Confirm Password</font> 
        </div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <INPUT TYPE="PASSWORD" NAME="confirmSambaPassword" SIZE="32" MAXLENGTH="31" VALUE="<% If TCWebApi_get("Samba_Entry","Passwd","h") <> "N/A" then TCWebApi_get("Samba_Entry","Passwd","s") end if%>" > 
      </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <font color="#000000">Work Group</font> 
        </div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <INPUT TYPE="TEXT" NAME="WorkGroup" SIZE="32" MAXLENGTH="31" VALUE="<% If TCWebApi_get("Samba_Entry","WorkGroup","h") <> "N/A" then TCWebApi_get("Samba_Entry","WorkGroup","s") end if%>" > 
      </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata"><div align=right> <font color="#000000">Net BIOS Name</font> 
        </div></td>
      <td class="tabdata"><div align=center>:</div></td>
      <td class="tabdata"> <INPUT TYPE="TEXT" NAME="NetBiosName" SIZE="32" MAXLENGTH="31" VALUE="<% If TCWebApi_get("Samba_Entry","NetBiosName","h") <> "N/A" then TCWebApi_get("Samba_Entry","NetBiosName","s") end if%>" > 
      </td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td class="tabdata">&nbsp;</td>
    </tr>
  </table>
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="Sambaflag" VALUE="0">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="Sambasave();">	</td></tr></table></form>
</body></html>
