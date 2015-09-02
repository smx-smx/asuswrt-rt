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
    if (confirm("Quit setup wizard and discard settings ?")) {
		window.close();
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
	  
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%>ADSL Router
	  
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
    <td width="500" bgcolor="#FFFFFF" class="headline">&nbsp; Quick Start - Time 
	Zone
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">
    
    Select the appropriate time zone for your location and click <B>NEXT</B> to 
	continue.
    
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
	  <option value="GMT-12:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-12:00" then asp_Write("selected") end if %>>(GMT-12:00) Enewetak, Kwajalein
	  <option value="GMT-11:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-11:00" then asp_Write("selected") end if %>>(GMT-11:00) Midway Island, Samoa
	  <option value="GMT-10:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-10:00" then asp_Write("selected") end if %>>(GMT-10:00) Hawaii
	  <option value="GMT-09:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-09:00" then asp_Write("selected") end if %>>(GMT-09:00) Alaska
	  <option value="GMT-08:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-08:00" then asp_Write("selected") end if %>>(GMT-08:00) Pacific Time (US,Canada)
	  <option value="GMT-07:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-07:00" then asp_Write("selected") end if %>>(GMT-07:00) Mountain Time (US & Canada)
	  <option value="GMT-06:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-06:00" then asp_Write("selected") end if %>>(GMT-06:00) Central Time (US & Canada), Maxico City, Saskatchewan
	  <option value="GMT-05:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-05:00" then asp_Write("selected") end if %>>(GMT-05:00) Eastern Time (US & Canada), Indiana(East)
	  <option value="GMT-04:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-04:00" then asp_Write("selected") end if %>>(GMT-04:00) Altlantic Time (Canada), Caracas, La Poz
	  <option value="GMT-03:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-03:30" then asp_Write("selected") end if %>>(GMT-03:30) Newfoundland
	  <option value="GMT-03:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-03:00" then asp_Write("selected") end if %>>(GMT-03:00) Brasilia, Buenos Aires, Georgetown
	  <option value="GMT-02:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-02:00" then asp_Write("selected") end if %>>(GMT-02:00) Mid-Atlantic
	  <option value="GMT-01:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-01:00" then asp_Write("selected") end if %>>(GMT-01:00) Azores, Cape Verde Is
	  <option value="GMT" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT" then asp_Write("selected") end if %>>(GMT) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London
	  <option value="GMT+01:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+01:00" then asp_Write("selected") end if %>>(GMT+01:00) Berlin, Stockholm, Rome, Bern, Brussels, Vienna
	 <option value="GMT+02:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+02:00" then asp_Write("selected") end if %>>(GMT+02:00) Athens, Helsinki, Istanbul, Cairo, Eastern Europe, Israel
	 <option value="GMT+03:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+03:00" then asp_Write("selected") end if %>>(GMT+03:00) Baghdad, Kuwait, Nairobi, Riyadh, Moscow
	 <option value="GMT+04:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+04:00" then asp_Write("selected") end if %>>(GMT+04:00) Abu Dhabi, Muscat, Tbilisi, Kazan, Volgograd
	 <option value="GMT+04:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+04:30" then asp_Write("selected") end if %>>(GMT+04:30) Kabul
	 <option value="GMT+05:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+05:00" then asp_Write("selected") end if %>>(GMT+05:00) Islamabad, Karachi, Ekaterinburg, Tashkent
	 <option value="GMT+06:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+06:00" then asp_Write("selected") end if %>>(GMT+06:00) Almaty, Dhaka
	 <option value="GMT+07:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+07:00" then asp_Write("selected") end if %>>(GMT+07:00) Bangkok, Jakarta, Hanoi
	 <option value="GMT+08:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+08:00" then asp_Write("selected") end if %>>(GMT+08:00) Beijing, Hong Kong, Perth, Singapore, Taipei
	 <option value="GMT+09:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+09:00" then asp_Write("selected") end if %>>(GMT+09:00) Tokyo, Osaka, Sapporo, Seoul, Yakutsk
	 <option value="GMT+09:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+09:30" then asp_Write("selected") end if %>>(GMT+09:30) Adelaide, Darwin
	 <option value="GMT+10:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+10:00" then asp_Write("selected") end if %>>(GMT+10:00) Brisbane, Canberra, Melbourne, Sydney, Hobart
	 <option value="GMT+11:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+11:00" then asp_Write("selected") end if %>>(GMT+11:00) Magadan, Solomon Is., New Caledonia
	 <option value="GMT+12:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+12:00" then asp_Write("selected") end if %>>(GMT+12:00) Fiji, Kamchatka, Marshall Is., Wellington, Auckland
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
          <input name="BackBtn" type="button" width="50" value="BACK" class="tabdata" onClick="javascript:window.location='/cgi-bin/wizardpwd.asp'">
      	  <input name="NextBtn" type="button" width="50" value="NEXT" class="tabdata" onClick="uiSave()">
	    <input name="ExitBtn" type="button" width="50" value="EXIT" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>

</form>
</body>
</html>        
