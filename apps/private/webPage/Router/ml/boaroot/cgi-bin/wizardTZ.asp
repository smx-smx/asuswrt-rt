<%
tcWebApi_set("Account_Entry0","web_passwd","uiViewPassword")
TCWebApi_set("Account_Entry0","console_passwd","uiViewPassword")
%>

<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<script type='text/javascript' src="/jsl.js"></script>
<style type="text/css">

</style>
<script>
function ExitWizard()
{
    if (confirm("<% tcWebApi_get("String_Entry","LoginFailJS0Text","s") %>")) {
		window.parent.close();
	}
}

function uiSave() {
    document.wzTimeZoneform.submit();
}
</script>
</head>

<body topmargin="10" leftmargin="0">

<form name="wzTimeZoneform" method="post" ACTION="/cgi-bin/wizardConType.asp">

<INPUT TYPE="HIDDEN" NAME="wan_VC" VALUE="<%tcWebApi_CurrentDefaultRoute()%>">
<table width="500" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="500" height="5" valign="baseline" class="orange"></td>
  </tr>
</table>

<table border="0" width="500" align="center" cellspacing="0" cellpadding="0">
  <tr>
    <td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
    <td width="300" height="50" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
	  
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	<%tcWebApi_get("String_Entry","AdslVdslInfoText","s")%>/
	<%end if%>
	<%tcWebApi_get("String_Entry","AdslInfoText","s")%>
	</td>
  </tr>
  <tr>
    <td width="500" class="orange"> </td>
  </tr>
</table>

<table width="500" height="3" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td class="orange"> </td>
  </tr>
</table>

<table width="500" height="35" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
  <tr>
    <td width="500" bgcolor="#FFFFFF" class="headline">&nbsp; 
	<% tcWebApi_get("String_Entry","HelpIndexQuickText","s") %> - <% tcWebApi_get("String_Entry","ToolsTimeZoneText","s") %>
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
    <%tcWebApi_get("String_Entry","WizardTZInfoText","s")%>
    
    </td>
  </tr>
  <tr>
    <td width="40" height="10"></td>
    <td width="150" class="tabdata" align="right"></td>
    <td width="310" class="tabdata" align="left"></td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" align="left" class="tabdata">
   	       
        <select id="uiViewdateTZ" NAME="uiViewdateTZ" SIZE="1">
	  <option value="GMT-12:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-12:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry0Text","s")%>
	<option value="GMT-11:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-11:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry1Text","s")%>
	<option value="GMT-10:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-10:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry2Text","s")%>
	<option value="GMT-09:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-09:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry3Text","s")%>
	<option value="GMT-08:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-08:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry4Text","s")%>
	<option value="GMT-07:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-07:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry5Text","s")%>
	<option value="GMT-06:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-06:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry6Text","s")%>
	<option value="GMT-05:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-05:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry7Text","s")%>
	<option value="GMT-04:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-04:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry8Text","s")%>
	<option value="GMT-03:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-03:30" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry9Text","s")%>
	<option value="GMT-03:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-03:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry10Text","s")%>
	<option value="GMT-02:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-02:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry11Text","s")%>
	<option value="GMT-01:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-01:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry12Text","s")%>
	<option value="GMT" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry13Text","s")%>
	<option value="GMT+01:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+01:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry14Text","s")%>
	<option value="GMT+02:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+02:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry15Text","s")%>
	<option value="GMT+03:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+03:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry16Text","s")%>
	<option value="GMT+03:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+03:30" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry17Text","s")%>
	<option value="GMT+04:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+04:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry18Text","s")%>
	<option value="GMT+04:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+04:30" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry19Text","s")%>
	<option value="GMT+05:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+05:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry20Text","s")%>
	<option value="GMT+05:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+05:30" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry21Text","s")%>
	<option value="GMT+06:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+06:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry22Text","s")%>
	<option value="GMT+06:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+06:30" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry23Text","s")%>
	<option value="GMT+07:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+07:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry24Text","s")%>
	<option value="GMT+08:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+08:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry25Text","s")%>
	<option value="GMT+09:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+09:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry26Text","s")%>
	<option value="GMT+09:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+09:30" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry27Text","s")%>
	<option value="GMT+10:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+10:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry28Text","s")%>
	<option value="GMT+11:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+11:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry29Text","s")%>
	<option value="GMT+12:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+12:00" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ToolsTimeCountry30Text","s")%>
        </select>
        
        </td>
    </tr>
  <tr>
    <td height="25">
        
        <input type="hidden" name="wzExitFlag">
        
        </td>
  </tr>
</table>

<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="right">
          <input name="BackBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonBackText","s") %>" class="tabdata" onClick="javascript:window.location='/cgi-bin/wizardpwd.asp'">
      	  <input name="NextBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonNextText","s") %>" class="tabdata" onClick="uiSave()">
	    <input name="ExitBtn" type="button" width="50" value="<% tcWebApi_get("String_Entry","ButtonExitText","s") %>" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>

</form>
</body>
</html>        
