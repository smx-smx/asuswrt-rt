<%
	if request_form("WZTrue_ExitFlag") <> "" then
		if request_form("WZTrue_ExitFlag") = "1" then
			tcWebApi_constSet("Wan_PVC0","CONNECTION","Connect_Manually")
			tcWebApi_Commit("Wan_PVC0")
		else
			tcWebApi_constSet("Wan_PVC0","CONNECTION","Connect_Keep_Alive")
			tcWebApi_Commit("Wan_PVC0")
			tcWebApi_constSet("Wan_PVC0","CONNECTION","Connect_Manually")
		end if
	end if
%>
<HTML><HEAD><TITLE></TITLE>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT language=JavaScript>
var intervalid;
function onClickConBtn()
{
	var thisForm = document.forms[0];
	thisForm.WZTrue_ExitFlag.value = 0;
	thisForm.submit();
}
function exec(command)
{
	window.oldOnError = window.onerror;
	window._command = command;
	var wsh = new ActiveXObject('WScript.Shell');
	if(wsh)
	{
		wsh.run(command);
		window.onerror = window.oldOnError;
	}
}
function onClickDisconBtn()
{
	var thisForm = document.forms[0];
	if (confirm("Quit setup wizard and discard settings ?")) 
	{
		thisForm.WZTrue_ExitFlag.value = 1;
		thisForm.submit();
		exec('ipconfig /flushdns');
		window.opener=null;
		window.close();
	}
}

function doLoad()
{
	var thisForm = document.forms[0];
	<% if tcWebApi_get("DeviceInfo_PVC0","Status","h") <> "1" then %>
		thisForm.WZTrue_MC_ConBtn.disabled = false;
		thisForm.WZTrue_MC_DisConBtn.disabled =true;
	<% else %>
		thisForm.WZTrue_MC_ConBtn.disabled =true;
		thisForm.WZTrue_MC_DisConBtn.disabled =false;
	<% end if %>
}
</SCRIPT>
</HEAD>
<body topmargin="0" leftmargin="0" onLoad="doLoad()">
<form name="WZTrue_MC_Form" method="post" action="http://true.true/cgi-bin/wzManualConnect_true.asp">
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
			<TD class=model vAlign=bottom align=right width=300 bgColor=#ffffff></TD></TR>
			<TR>
			<TD class=orange width=500 colSpan=2>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE class=orange height=2 cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange>
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE height=35 cellSpacing=0 cellPadding=0 width=500 align=center border=0 color="#FF9933">
	<TBODY>
		<TR>
			<TD class=headline width=500 bgColor=#ffffff>
			&nbsp; Manaul Connection
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=2 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD width=40>&nbsp;
			
			</TD>
			<TD class=tabdata colSpan=2>
				Please don't surf internet by this page and 
				close current browser also.<BR>Otherwise,you needs to open connection page 
				by URL:http://true/connection in browser.
				<INPUT type=hidden value=1 name=WZTrue_ConnectFlag>
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
			<TD>&nbsp;
			
			</TD>
			<TD class=tabdata align=right>&nbsp;
			
			</TD>
			<TD class=tabdata align=left>&nbsp;
			
			</TD>
		</TR>
		<TR>
			<TD class=tabdata align=middle colSpan=3>
				<TABLE>
					<TBODY>
						<TR>
							<TD>
								<INPUT class=tabdata onclick=onClickConBtn() type=button value=Connect name=WZTrue_MC_ConBtn>
							</TD>
							<TD width=80>
								<INPUT type=hidden name=WZTrue_ExitFlag>
							</TD>
							<TD>
								<INPUT class=tabdata onclick=onClickDisconBtn() type=button value=Disconnect name=WZTrue_MC_DisConBtn>
							</TD>
						</TR>
					</TBODY>
				</TABLE>
			</TD>
		</TR>
		<TR>
			<TD>&nbsp;
			
			</TD>
			<TD class=tabdata align=right>&nbsp;
			
			</TD>
			<TD class=tabdata align=left>&nbsp;
			
			</TD>
		</TR>
	</TBODY>
</TABLE>
<TABLE class=orange height=30 cellSpacing=0 cellPadding=0 width=500 align=center border=0>
	<TBODY>
		<TR>
			<TD class=orange>&nbsp;
			
			</TD>
		</TR>
	</TBODY>
</TABLE>
<SCRIPT language=JavaScript>
     self.setTimeout("self.location.href='http://true.true/cgi-bin/wzManualConnect_true.asp';", 10000);
</SCRIPT>
</FORM>
</BODY>
</HTML>
