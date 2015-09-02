<%
tcWebApi_commit("Wan_PVC")
tcWebApi_commit("Account_Entry")
tcWebApi_commit("Timezone_Entry")
tcWebApi_save()
%>

<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=

iso-8859-1

">
<link rel="stylesheet" href="/style.css" type="text/css">
<script type='text/javascript' src="/jsl.js"></script>
<style type="text/css">

</style>
<script>
function onClickclose()
{
    window.close();
    return false;
}
</script>
</head>

<body topmargin="10" leftmargin="0">

<form name="wzCloseform" method="post">


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
    <td width="500" bgcolor="#FFFFFF" class="headline">&nbsp;
    
    Quick Start Completed !! 
    
    </td>
  </tr>
</table>

<table width="500" border="0" align="center" cellpadding="2" cellspacing="0">
  <tr>
    <td width="40">&nbsp;</td>
    <td colspan="2" class="tabdata"><span class="tabdata">
    
    Saved Changes.
    
    </span></td>
  </tr>

  <tr>
    <td></td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td width="40">&nbsp;</td>
    <td width="150" class="tabdata" align="right">&nbsp;</td>
    <td width="310" class="tabdata" align="left">&nbsp;</td>
  </tr>
  <tr>
    <td width="40">&nbsp;</td>
    <td width="150" class="tabdata" align="right">&nbsp;</td>
    <td class="tabdata" align="left">&nbsp;</td>
  </tr>
  <tr>
    <td height="25"></td>
  </tr>
</table>

<table width="500" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
<tr>
    <td class="orange"><div align="right">

<INPUT onClick="onClickclose()" type=button value="Close" name=close>        </div></td>
    <td width="25" valign="baseline" class="orange">&nbsp;</td>
</tr>
</table>


</form>
</body>
</html>        
