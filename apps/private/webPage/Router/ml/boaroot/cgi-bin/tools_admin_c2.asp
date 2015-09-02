<%
' save
If Request_Form("adminFlag") = "1" Then
	TCWebApi_set("GUITemp_Entry0","toolsUserMode","user_Mode")
	If Request_Form("user_Mode") = "0" Then
		TCWebApi_set("Account_Entry0","username","admin_name")
		TCWebApi_set("Account_Entry0","web_passwd","admin_Password")
		TCWebApi_set("Account_Entry0","console_passwd","admin_Password")
		tcWebApi_commit("Account_Entry0")
	elseif Request_Form("user_Mode") = "1" Then
		TCWebApi_set("Account_Entry1","username","user2_username")
		TCWebApi_set("Account_Entry1","web_passwd","user2_Password")
		tcWebApi_commit("Account_Entry1")
	elseif Request_Form("user_Mode") = "2" Then
		TCWebApi_set("Account_Entry2","username","user3_username")
		TCWebApi_set("Account_Entry2","web_passwd","user3_Password")
		tcWebApi_commit("Account_Entry2")
	End If
	TCWebApi_set("Account_Entry0","web_passwd","uiViewTools_Password")
	tcWebApi_commit("Account_Entry0")
End If
' delete
If Request_Form("adminFlag") = "2" Then
	TCWebApi_set("GUITemp_Entry0","toolsUserMode","user_Mode")
	If Request_Form("user_Mode") = "1" Then
		tcWebApi_get("GUI_Entry0","toolsAdminAccount1","h")
		tcWebApi_commit("Account_Entry1")
	elseif Request_Form("user_Mode") = "2" Then
		tcWebApi_get("GUI_Entry0","toolsAdminAccount2","h")
		tcWebApi_commit("Account_Entry2")
	End If
End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">

<script language="JavaScript">
function uiSave()
{	
	if( (document.tool_admin.admin_name.value == document.tool_admin.user2_username.value) ||
	 (document.tool_admin.admin_name.value == document.tool_admin.user3_username.value))
	{
		alert("<%tcWebApi_get("String_Entry","AdminJS3Text","s")%>");
		return;
	}
	
	if(document.tool_admin.user2_username.value == document.tool_admin.user3_username.value)
	{
		alert("<%tcWebApi_get("String_Entry","AdminJS4Text","s")%>");
		return;
	}
	
	switch(document.tool_admin.user_Mode.selectedIndex)
	{
		case 0:
			if(1 == userNamePasswordCheck(document.tool_admin.admin_name , document.tool_admin.admin_Password ,document.tool_admin.admin_PasswordConfirm))
				return;
			break;
		case 1:
			if(1 == userNamePasswordCheck(document.tool_admin.user2_username ,document.tool_admin.user2_Password , document.tool_admin.user2_PasswordConfirm))
				return;
			break;
		case 2:
			if(1 == userNamePasswordCheck(document.tool_admin.user3_username ,document.tool_admin.user3_Password , document.tool_admin.user3_PasswordConfirm))
				return;
			break;
		default:
			break;
	}
	
	document.tool_admin.adminFlag.value = 1;
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
   		alert('<%tcWebApi_get("String_Entry","AdminJS2Text","s")%>');      	  								    	    	   		
		return true;
	}	
	return false;
}

function doUserModeChange()
{
	switch(document.tool_admin.user_Mode.selectedIndex)
	{
		case 0:
			document.tool_admin.deleteBtn.disabled=true;
			document.getElementById("adminPart").style.display="";
			document.getElementById("userPart1").style.display="none";
			document.getElementById("userPart2").style.display="none";
			break;
		case 1:
			document.tool_admin.deleteBtn.disabled=false;
			document.getElementById("adminPart").style.display="none";
			document.getElementById("userPart1").style.display="";
			document.getElementById("userPart2").style.display="none";
			break;
		case 2:
			document.tool_admin.deleteBtn.disabled=false;
			document.getElementById("adminPart").style.display="none";
			document.getElementById("userPart1").style.display="none";
			document.getElementById("userPart2").style.display="";
			break;
		default:
			document.tool_admin.deleteBtn.disabled=true;
			document.getElementById("adminPart").style.display="";
			document.getElementById("userPart1").style.display="none";
			document.getElementById("userPart2").style.display="none";
			break;
	}
}

function uiDelete() 
{
	document.tool_admin.adminFlag.value = 2;
    document.tool_admin.submit();
    return;
}

function doload() 
{
	doUserModeChange();
    return;
}

function userNamePasswordCheck(object1 , object2 , object3) 
{
    if (object1.value.length == 0)
	{
    	alert("<%tcWebApi_get("String_Entry","AdminJS5Text","s")%>");
    	return 1;
    }

    if (object2.value.length == 0)
	{
    	alert("<%tcWebApi_get("String_Entry","AdminJS0Text","s")%>");
    	return 1;
    }
	
    if (object2.value != object3.value)
	{
        alert("<%tcWebApi_get("String_Entry","AdminJS1Text","s")%>");
        return 1;
    }
	
    if(quotationCheck(object2, 30) ) 
		return 1;
    return 0;
}
</script>
</head><body onLoad="doload();">
<FORM METHOD="POST" ACTION="/cgi-bin/tools_admin.asp" name="tool_admin">
<INPUT TYPE="HIDDEN" NAME="adminFlag" VALUE="0">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main">
		<font color="#FFFFFF"><%tcWebApi_get("String_Entry","AdminText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"></td>
		<td width="10"> </td>
		<td width="440"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","AdminusertypeText","s")%></font>
			</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<select name="user_Mode" size="1" onchange="doUserModeChange()">
			<option value="0" <% if tcWebApi_get("GUITemp_Entry0","toolsUserMode","h") = "0" then asp_Write("selected") elseif tcWebApi_get("GUITemp_Entry0","toolsUserMode","h") = "N/A" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Adminuser0Text","s")%></option>
			<option value="1" <% if tcWebApi_staticGet("GUITemp_Entry0","toolsUserMode","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Adminuser1Text","s")%></option>
			<option value="2" <% if tcWebApi_staticGet("GUITemp_Entry0","toolsUserMode","h") = "2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","Adminuser2Text","s")%></option>
			</select>
		</td>
	</tr>
</table>

<div id="adminPart">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td class="light-orange"  width="150" height="30" >&nbsp;</td>
		<td class="light-orange"  width="10" ></td>
		<td class="tabdata" width="150">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","Adminusername0Text","s")%></font>    
			</div>
		</td>
		<td class="tabdata" width="10"><div align=center>:</div></td>
		<td class="tabdata" width="440">
			<INPUT TYPE="text" NAME="admin_name" SIZE="32" MAXLENGTH="30" value="<% If tcWebApi_get("Account_Entry0", "username", "h")<>"N/A" then tcWebApi_staticGet("Account_Entry0", "username", "s") end if %>">    
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","AdminNewPasswordText","s")%></font>    
			</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="admin_Password" SIZE="32" MAXLENGTH="30" value="">    
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","AdminConfirmPasswordText","s")%></font>    
			</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="admin_PasswordConfirm" SIZE="32" MAXLENGTH="30" value="">    
		</td>
	</tr>
</table>
</div>
<div id="userPart1">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td class="light-orange"  width="150" height="30" >&nbsp;</td>
		<td class="light-orange"  width="10" ></td>
		<td class="tabdata" width="150">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","Adminusername1Text","s")%></font>    
			</div>
		</td>
		<td class="tabdata" width="10"><div align=center>:</div></td>
		<td class="tabdata" width="440">
			<INPUT TYPE="text" NAME="user2_username" SIZE="32" MAXLENGTH="30" value="<% If tcWebApi_get("Account_Entry1", "username", "h")<>"N/A" then tcWebApi_staticGet("Account_Entry1", "username", "s") end if %>">    
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","AdminNewPasswordText","s")%></font>    
			</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="user2_Password" SIZE="32" MAXLENGTH="30" value="">    
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","AdminConfirmPasswordText","s")%></font>    
			</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="user2_PasswordConfirm" SIZE="32" MAXLENGTH="30" value="">    
		</td>
	</tr>
</table>
</div>
<div id="userPart2">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td class="light-orange"  width="150" height="32" >&nbsp;</td>
		<td class="light-orange"  width="10" ></td>
		<td class="tabdata" width="150">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","Adminusername2Text","s")%></font>    
			</div>
		</td>
		<td class="tabdata" width="10"><div align=center>:</div></td>
		<td class="tabdata" width="440">
			<INPUT TYPE="text" NAME="user3_username" SIZE="32" MAXLENGTH="30" value="<% If tcWebApi_get("Account_Entry2", "username", "h")<>"N/A" then tcWebApi_staticGet("Account_Entry2", "username", "s") end if %>">    
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","AdminNewPasswordText","s")%></font>    
			</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="user3_Password" SIZE="32" MAXLENGTH="30" value="">    
		</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">
			<div align=right>
			<font color="#000000"><%tcWebApi_get("String_Entry","AdminConfirmPasswordText","s")%></font>    
			</div>
		</td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="user3_PasswordConfirm" SIZE="32" MAXLENGTH="30" value="">    
		</td>
	</tr>
</table>
</div>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td class="light-orange"  width="150" height="30" >&nbsp;</td>
		<td class="light-orange"  width="10" ></td>
		<td width="150">&nbsp;</td>
		<td width="10">&nbsp;</td>
		<td width="440">&nbsp;</td>
	</tr>
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td>
		<td width="160" class="orange">&nbsp;</td>
		<td width="440" class="orange">
			<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="uiSave()"> 
			<INPUT TYPE="BUTTON" NAME="deleteBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonDeleteText","s")%>" onClick="uiDelete()">    
		</td>
	</tr>
</table>
</form>
</body>
</html>

