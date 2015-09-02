<%
If Request_Form("NATSession_submitFlag") = "1" Then
	tcWebApi_set("WebCurSet_Entry","natSession_entry","NATTAB_Session_index")
	tcWebApi_unset("natSessions_Entry")
	tcWebApi_commit("natSessions_Entry")
End If

If Request_Form("NATSession_submitFlag") = "2" Then
	tcWebApi_set("natSessions_setting","MaxNatSessions","natMaxSession")
	tcWebApi_set("natSessions_setting","SessionMode","NAT_Session_mode")
	If Request_Form("NAT_Session_mode") = "0" Then
		tcWebApi_set("natSessions_setting","SessionPerUser","natSessionPerUser")
	End If
	tcWebApi_commit("natSessions_Entry")
End If
%>
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="/style.css">
<SCRIPT language=javascript>
var peruser = 	0;
function fromload()
{
    with ( document.forms[0] ) 
	{	
 		peruser = natSessionPerUser.value;
		if(true == NAT_Session_mode[0].checked)
		{
			if (document.forms[0].NATSession_AddBtn != null)
					NATSession_AddBtn.disabled = true;
		}
		else
		{
			natSessionPerUser.disabled = true;
			natSessionPerUser.value = "";
			if (document.forms[0].NATSession_AddBtn != null)
				NATSession_AddBtn.disabled = false;		
		}
	}
}

function typeClick()
{
     with ( document.forms[0] ) {
    	if (true == NAT_Session_mode[1].checked)
		{
		   natSessionPerUser.disabled = true; 
		   natSessionPerUser.value = "";   
		   if (document.forms[0].NATSession_AddBtn != null)
		   		NATSession_AddBtn.disabled = false;  		   
		}
		else
		{
		   natSessionPerUser.disabled = false; 
		   natSessionPerUser.value = peruser;  
		   if (document.forms[0].NATSession_AddBtn != null)		     
		   		NATSession_AddBtn.disabled = true; 
		}
  	}
}


function doSave()
{
	var natMaxSess = document.forms[0].natMaxSession.value;
	var natPerUser = document.forms[0].natSessionPerUser.value;
	document.NAT_Session_form.NATSession_submitFlag.value = 2;
	
	if((!isNumeric(natMaxSess)) || (Number(natMaxSess) > 1024) || (Number(natMaxSess) < 1))
	{
			return false;
	}
	 
	with ( document.forms[0] ) 
	{
    	if (true == NAT_Session_mode[0].checked)
		{
			 if((!isNumeric(natPerUser)) || (Number(natPerUser) > 1024))
			 {
				alert("NAT Session Per User must be in the range 0-1024");
				return false;
			 }
		}
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
</SCRIPT>

<META content="MSHTML 6.00.2900.5726" name=GENERATOR></HEAD>
<BODY onload=fromload()>
<FORM name=NAT_Session_form method=post>
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
		<TBODY>
			<TR>
				<TD class=light-orange colSpan=6 height=5>&nbsp;</TD>
			</TR>
			<TR>
				<TD class=title-main width=150 height=30>NAT Sessions </TD>
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
				<TD class=tabdata>
					<DIV align=left><B>NAT Session Limited </B></DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center></DIV>
				</TD>
				<TD class=tabdata></TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=left>&nbsp;&nbsp;&nbsp;&nbsp; Max NAT Sessions[1-1024] </DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center>:</DIV>
				</TD>
				<TD class=tabdata>
					<INPUT maxLength=4 size=15 value="<%if TCWebAPI_get("natSessions_setting", "MaxNatSessions", "h") <> "N/A" then TCWebAPI_get("natSessions_setting", "MaxNatSessions", "s") else asp_Write("") end if %>" name=natMaxSession> 
				</TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=right></DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center></DIV>
				</TD>
				<TD class=tabdata></TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=left>
					<INPUT onclick=typeClick() type=radio <%if TCWebAPI_get("natSessions_setting", "SessionMode", "h") = "0" then asp_Write("CHECKED") elseif TCWebAPI_get("natSessions_setting", "SessionMode", "h") = "N/A" then asp_Write("CHECKED") end if %> value="0" name=NAT_Session_mode> &nbsp; Same Sessions per user 
					</DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center></DIV>
				</TD>
				<TD class=tabdata></TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
				<DIV align=left>&nbsp;&nbsp;&nbsp;&nbsp; NAT Session Per User </DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center>:</DIV>
				</TD>
				<TD class=tabdata>
					<INPUT maxLength=4 size=15 value="<%if TCWebAPI_get("natSessions_setting", "SessionPerUser", "h") <> "N/A" then TCWebAPI_get("natSessions_setting", "SessionPerUser", "s") else asp_Write("") end if %>" name=natSessionPerUser> 
				</TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=right></DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center></DIV>
				</TD>
				<TD class=tabdata></TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata colSpan=3>
					<DIV align=left>
					<INPUT onclick=typeClick() type=radio value="1" <%if TCWebAPI_get("natSessions_setting", "SessionMode", "h") = "1" then asp_Write("CHECKED") end if %> name=NAT_Session_mode> 
					&nbsp; Set Sessions every IP Address. You can see every IP Address's NAT session in this page. 
					</DIV>
				</TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata colSpan=3><BR>
					<iframe src="/cgi-bin/adv_nat_session.cgi" frameborder="0" width="420" height="200" align=left></iframe>
				</TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=right></DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center></DIV>
				</TD>
				<TD class=tabdata></TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD>
					<INPUT type=hidden value=0 name=NATTAB_Session_index>
					<INPUT type=hidden value=0 name=NATSession_submitFlag>
				</TD>
				<TD>&nbsp;</TD>
				<TD>
					<INPUT onclick="window.location='/cgi-bin/adv_nat_addsession.asp'" type=button value=add name=NATSession_AddBtn> 
				</TD>
			</TR>
			<TR>
				<TD class=light-orange>&nbsp;</TD>
				<TD class=light-orange></TD>
				<TD>&nbsp;</TD>
				<TD class=tabdata>
					<DIV align=right></DIV>
				</TD>
				<TD class=tabdata>
					<DIV align=center></DIV>
				</TD>
				<TD class=tabdata></TD>
			</TR>
		</TBODY>
	</TABLE>
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
		<TBODY>
			<TR>
				<TD class=black width=160 height=42>&nbsp;</TD>
				<TD class=orange width=160>&nbsp;</TD>
				<TD class=orange width=440>
					<INPUT onclick=doSave(); type=button value=SAVE name=NATSession_SaveBtn> 
					<INPUT onclick="javascript:window.location='/cgi-bin/adv_nat_top.asp'" type=button value=BACK name=NATSession_BackBtn> 
				</TD>
			</TR>
		</TBODY>
	</TABLE>
</FORM>
</BODY>
</HTML>
