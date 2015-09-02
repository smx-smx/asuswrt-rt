<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<style type="text/css">
</style>
<script language="JavaScript">
function doLoadHomePage()
{
	window.location.href = "/cgi-bin/index.asp";
}
</script>
</head>

<body topmargin="5">
<form name="StatusInfoOteForm" method="post">
<table width="550" height="20" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
  <tr>
    <td width="550" bgcolor="#FFFFFF" class="headline" height="20">&nbsp;
		<%tcWebApi_get("String_Entry","WizardStatusInfoText","s")%>
    </td>
  </tr>
</table>

<table width="550" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="550" height="2" valign="baseline" class="orange"></td>
  </tr>
    <tr>
    <td width="550" bgcolor="#FFFFFF" class="headline" height="5">&nbsp;</td>
  </tr>
</table>

<table cellpadding="5" cellspacing="0" border="1" width="550" align="center" bordercolor="#CCCCCC">
  <tr>
    <td width="550" height="22" valign="baseline" class="orange" colspan="2"></td>
  </tr>
	<tr> 
		<td class="tabdata" width="40%" align="left">
		<%tcWebApi_get("String_Entry","WizardStatusInfoAdslText","s")%>
		</td>
		<td class="tabdata" width="60%" valign="left" align="left">
		<%if tcWebApi_Get("Info_Adsl","lineState","h")<>"up" then%>
		<IMG src="/red.jpg" width="76" height="76">
		<%else%>
		<IMG src="/green.jpg" width="76" height="76">
		<%end if%>
		</td>
	</tr>
	<tr> 
		<td class="tabdata" width="40%" align="left">
		<%tcWebApi_get("String_Entry","WizardStatusInfoATMText","s")%>
		</td>
		<td class="tabdata" width="60%" valign="middle" align="left">
		<%if tcWebApi_staticGet("Info_Adsl","lineState","h")<>"up" then%>
		<IMG src="/red.jpg" width="76" height="76">
		<%else%>
		<IMG src="/green.jpg" width="76" height="76">
		<%end if%>
		</td>
	</tr>
	<tr> 
		<td class="tabdata" width="40%" align="left">
		<%tcWebApi_get("String_Entry","WizardStatusInfoNetText","s")%>
		</td>
		<td class="tabdata" width="60%" valign="middle" align="left">
		<%if tcWebApi_Get("Wan_PVC0","ISP","h")<>"3" then if tcWebApi_Get("DeviceInfo_PVC0","Status","h") = "1" Then%>
		<IMG src="/green.jpg" width="76" height="76">
		<%elseif tcWebApi_Get("DeviceInfo_PVC0","IP6Status","h") = "1" Then%>
		<IMG src="/green.jpg" width="76" height="76">
		<%else%>
		<IMG src="/red.jpg" width="76" height="76">
		<%end if else%>
		<IMG src="/red.jpg" width="76" height="76">
		<%end if%>
		</td>
	</tr>
</table>  
<table width="550" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="left" >
		<input type="button" name="wizAdminApply" value="<%tcWebApi_get("String_Entry","WizardStatusInfoBtnText","s")%>" onClick="doLoadHomePage();">
        </div></td>
    <td width="25" valign="baseline" class="orange">&nbsp;</td>
</tr>
</table>
  
</form>
</body>
</html>
