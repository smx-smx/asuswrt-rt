<%
If Request_Form("submitFlag") = "1" Then
	tcWebApi_set("signature_Entry","Status","SignatureStauts")
	tcWebApi_commit("signature_Entry")
End If
%>
<HTML><HEAD>
<META HTTP-EQUIV="pragma" CONTENT="no-cache"> 
<META HTTP-EQUIV="Cache-Control" CONTENT="no-store, must-revalidate"> 
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT language=JavaScript>
function page_submit()
{
	document.Sys_signature.submitFlag.value = 1;
	document.Sys_signature.submit();
}
</SCRIPT>
</HEAD>
<BODY>
<FORM name=Sys_signature method=post>
<H2 align=center>Digital Signature</H2>
<TABLE cellSpacing=0 cellPadding=0 align=center bgColor=#ffffff border=0>
	<TBODY>
		<TR>
			<TD width=120>Signature Status:</TD>
			<TD><%If tcWebApi_get("signature_Entry","Status","h") <> "N/A" Then tcWebApi_staticGet("signature_Entry","Status","s") end if%></TD>
		</TR>
		<TR>
			<TD width=160 colSpan=2>
			<INPUT type=radio value="Enable" name=SignatureStauts <% if tcWebApi_get("signature_Entry","Status","h") = "Enable" then asp_Write("checked") end if %>>Enable<BR>
			<INPUT type=radio value="Disable" name=SignatureStauts <% if tcWebApi_get("signature_Entry","Status","h") = "Disable" then asp_Write("checked") end if %>>Disable
			</TD>
		</TR>
		<TR>
			<TD colSpan=2>&nbsp;</TD></TR>
		<TR>
			<TD align="center" colSpan=2>
			<INPUT onclick=page_submit() type=button value=Apply name=SignaApply>
			<INPUT TYPE="HIDDEN" NAME="submitFlag" VALUE="0">
			</TD>
		</TR>
	</TBODY>
</TABLE>
</FORM>
</BODY>
</HTML>
