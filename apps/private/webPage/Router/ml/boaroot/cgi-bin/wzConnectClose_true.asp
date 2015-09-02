<HTML><HEAD><TITLE></TITLE>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT language=JavaScript>
function onClickCloseBtn()
{	
		self.opener=null; 
		self.open('','_self');  
		self.close();
		setTimeout("alert('Your current browser can not close the window, please close it by yourself')", 3);
}
</SCRIPT>
</HEAD>
<BODY leftMargin=0 topMargin=0>
<FORM name=WZTrue_Close_Form method=post>
<TABLE cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange vAlign=baseline width=500 height=5></TD>
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
			<TD class=orange width=500 colSpan=2>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE height=35 cellSpacing=0 cellPadding=0 width=500 align=center border=0 color="#FF9933">
	<TBODY>
		<TR>
			<TD class=headline width=500 bgColor=#ffffff>
				&nbsp; <%tcWebApi_get("String_Entry","WizardCompleteText","s")%>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=2 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD width=40>&nbsp;</TD>
			<TD class=tabdata colSpan=2>
			<SPAN class=tabdata><%tcWebApi_get("String_Entry","WizardSaveChangeText","s")%></SPAN>
			</TD>
		</TR>
		<TR>
			<TD width=40>&nbsp;
			
			</TD>
			<TD class=tabdata colSpan=2>
			<SPAN class=tabdata>
			<%tcWebApi_get("String_Entry","WizardConnCloseInfoText","s")%>
			</SPAN>
			</TD>
		</TR>
		<TR>
			<TD width=40>&nbsp;
			
			</TD>
			<TD class=tabdata align=right width=150>&nbsp;
			
			</TD>
			<TD class=tabdata align=left width=310>&nbsp;
			
			</TD>
		</TR>
		<TR>
			<TD width=40>&nbsp;
			
			</TD>
			<TD class=tabdata align=right width=150>&nbsp;
			
			</TD>
			<TD class=tabdata align=left>&nbsp;
			
			</TD>
		</TR>
		<TR>
			<TD height=25>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE class=orange height=40 cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange>
				<DIV align=right>
				<INPUT class=tabdata onclick=onClickCloseBtn() type=button value="<% tcWebApi_get("String_Entry","ButtonCloseText","s") %>" name=WZTrue_Close_CloseBtn>
				<INPUT type=hidden name=WZTrue_URL>
				</DIV>
			</TD>
			<TD class=orange vAlign=baseline width=25>&nbsp;
			
			</TD>
		</TR>
	</TBODY>
</TABLE>
</FORM>
</BODY>
</HTML>
</HTML>