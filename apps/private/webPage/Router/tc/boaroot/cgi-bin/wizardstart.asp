
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
		document.wzStartform.wzExitFlag.value = 1;
		document.wzStartform.submit();
		window.close();
	}
}
</script>
</head>

<body topmargin="10" leftmargin="0">

<form name="wzStartform" method="post" >


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
    <td width="500" colspan="2" class="orange"> </td>
  </tr>
</table>

<table width="500" height="2" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td class="orange"> </td>
  </tr>
</table>

<table width="500" height="35" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
  <tr>
    <td width="500" bgcolor="#FFFFFF" class="headline">&nbsp; Quick Start
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&#12288;</td>
    <td colspan="2" class="tabdata">    				
        The Wizard will guide you through these four quick steps. Begin by 
		clicking on <strong>NEXT.</strong>
        
        </td>
  </tr>

  <tr>
    <td width="40">&#12288;</td>
    <td width="30"></td>
    <td class="tabdata" align="left"> 
    
    Step 1. Set your new password 
    
    </td>
  </tr>
  <tr>
    <td width="40">&#12288;</td>
    <td width="30"></td>
    <td class="tabdata"> 
    
    Step 2. Choose your time zone
    
    </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td width="30"></td>
    <td class="tabdata"> 
    
    Step 3. Set your Internet connection 
    
    </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td></td>
    <td class="tabdata"> 
    
    Step 4. Confirm the Configuration and Save it 
    
    </td>
  </tr>
  <tr>
    <td>&#12288;</td>
    <td width="30"></td>
    <td>
        
        <input type="hidden" name="wzExitFlag">
        
        </td>
  </tr>
  <tr>
    <td></td>
  </tr>
</table>

<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange">
        <div align="right" >
          <input name="NextBtn" type="button" width="50" value="NEXT" class="tabdata" onClick="javascript:window.location='/cgi-bin/wizardpwd.asp'">
          <input name="ExitBtn" type="button" width="50" value="EXIT" class="tabdata" onClick="ExitWizard()">
        </div></td>
    <td width="25" valign="baseline" class="orange">&#12288;</td>
</tr>
</table>

</form>
</body>
</html>        
