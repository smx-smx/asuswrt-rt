<HTML><HEAD><TITLE></TITLE>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT>
function DoYes()
{
	document.forms[0].WZTrue_EA_Flag.value = 1;
	document.forms[0].submit();	
}

function DoNo()
{	
	jumpUrl = "/cgi-bin/wzIspUserPwd_true.asp"
	top.location.href = jumpUrl;
}
</SCRIPT>
</HEAD>
<BODY leftMargin=0 topMargin=0>
<form name="WZTrue_EA_Form" method="post" action="/cgi-bin/wzWait3_true.asp">
<TABLE cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange vAlign=baseline width=500 height=5>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD vAlign=center align=middle width=200 bgColor=#ffffff height=50>
				<DIV align=left>
				<img src="/logo.gif" tppabs="logo.gif" width="200" height="50">
				</DIV>
			</TD>
			<TD class=model vAlign=bottom align=right width=300 bgColor=#ffffff height=50>
			</TD>
		</TR>
		<TR>
			<TD class=orange width=500 colSpan=2></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE class=orange height=2 cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange></TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=2 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD width=500>&nbsp;</TD>
		</TR>
		<TR>
			<TD width=500 align="center"><%tcWebApi_get("String_Entry","WizardExistAccInfoText","s")%></TD>
		</TR>
		<TR>
			<TD width=500>&nbsp;</TD>
		</TR>
		<TR>
			<TD width=500 align="center">
				<INPUT onclick=DoYes() type=button value="   <%tcWebApi_get("String_Entry","WANDefaultRoute0Text","s")%>   " name=WZTrue_accout_Yes align="middle">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<INPUT onclick=DoNo() type=button value="   <%tcWebApi_get("String_Entry","WANDefaultRoute0Text","s")%>   " name=WZTrue_accout_No align="middle">
				<input name="WZTrue_EA_Flag" type="hidden" value="0">
			</TD>
		</TR>
		<TR>
			<TD align=middle colSpan=3 height=25>
				<TABLE>
					<TBODY>
						<TR>
							<TD align=middle></TD>
						</TR>
					</TBODY>
				</TABLE>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE class=orange height=30 cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange>&nbsp;</TD>
		</TR>
	</TBODY>
</TABLE>
</FORM>
</BODY>
</HTML>

