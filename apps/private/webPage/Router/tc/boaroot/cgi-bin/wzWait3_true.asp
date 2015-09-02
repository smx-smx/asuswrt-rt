<%
	if request_form("WZTrue_Set_Flag") = "1" then
		if request_form("WZTrue_Reboot") = "1" then
			tcWebApi_constSet("System_Entry","alter_reboot_type","2")
			tcWebApi_constSet("System_Entry","reboot_type","2")
			tcWebApi_commit("System_Entry")
		end if
	end if
%>
<html>
<head>
<title></title><meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/style.css">
<SCRIPT language=JavaScript>
var id;
var seed=120;
function doRefresh()
{
	window.location.href = "http://services.trueinternet.co.th/truehisp/activate.php";
}
function tip()
{
	seed--;
	document.wzWait3Form.wait3time.value=seed;
	if(seed < 1)
	{
		window.clearInterval(id);
		doRefresh();
	}
}
function doLoad()
{
	id=window.setInterval(tip,1000);
<%if request_form("WZTrue_EA_Flag") = "1" then%>
	document.wzWait3Form.WZTrue_Reboot.value=1;
	document.wzWait3Form.submit();
<%end if%>
}
</script>
<body topmargin="0" leftmargin="0" onLoad="doLoad()">
<form name="wzWait3Form" method="post" action="/cgi-bin/wzWait3_true.asp">
<table width="500" border="0" align=center cellpadding="0" cellspacing="0">
	<tr>
		<td width="500" height="5" valign="baseline" class="orange">
		</td>
	</tr>
</table>
<table border="0" width="500" align=center cellspacing="0" cellpadding="0">
	<tr>
		<td width="200" height="50" align=center valign=middle bgcolor="#FFFFFF">
			<div align=left>
			<img src="/logo.gif" tppabs="logo.gif" width="200" height="50">
			</div>
		</td>
		<td width="300" height="50" align=right valign="bottom" bgcolor="#FFFFFF" class="model">
		</td>
		</tr>
		<tr>
		<td width="500" colspan="2" class="orange">
		</td>
	</tr>
</table>
<table width="500" height="2" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
	<tr>
		<td class="orange"> 
		</td>
	</tr>
</table>
<table width="500" height="35" border="0" align=center cellpadding="0" cellspacing="0" color="#FF9933">
	<tr>
		<td width="500" bgcolor="#FFFFFF" class="headline">&nbsp;
		</td>
	</tr>
</table>
<table width="500" border="0" align=center cellpadding="2" cellspacing="0">
	<tr>
		<td width="40">&nbsp;
			
		</td>
		<td colspan="2" class="wzwait" align=left>
		&nbsp; The modem will resart.<br>&nbsp; The left second:&nbsp;
		<INPUT TYPE="TEXT" NAME="wait3time" SIZE="1" MAXLENGTH="1" VALUE="" class="InputTextTime">
		<input type="hidden" name="WZTrue_Set_Flag" value="<%if request_form("WZTrue_EA_Flag")="1" then asp_write("1") else asp_write("0") end if%>">
		<input type="hidden" name="WZTrue_Reboot" value="0">
		</td>
	</tr>
	<tr>
		<td width="40">&nbsp;
		
		</td>
		<td class="tabdata">&nbsp;
		
		</td>
		<td class="tabdata">&nbsp;
		
		</td>
	</tr>
	<tr>
		<td width="40">&nbsp;
		
		</td>
		<td width="150" class="tabdata" align=right>&nbsp;
		
		</td>
		<td class="tabdata" align=left>&nbsp;
		
		</td>
	</tr>
	<tr>
		<td height="25">
		</td>
	</tr>
</table>
<table width="500" height="40" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
	<tr>
		<td class="orange">
		<div align=right>
		&nbsp;
		</div>
		</td>
		<td width="25" valign="baseline" class="orange">&nbsp;
		
		</td>
	</tr>
</table>
</form>
</body>
</html>