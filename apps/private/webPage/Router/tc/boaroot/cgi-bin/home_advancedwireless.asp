<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">

<script language="JavaScript">
function onClickBasic() {
	location.replace("/cgi-bin/wifiqa_basic.asp");
	//window.open("/cgi-bin/wifiqa_basic.asp","Basic","toolbar=no,menubar=no,scrollbars=yes,height=600, width=850,location=0,left=50,top=50");
	return false;
}
function onClickAdvanced() {
	location.replace("/cgi-bin/wifiqa_advanced.asp");
	//window.open("/cgi-bin/wifiqa_advanced.asp","Advanced","toolbar=no,menubar=no,scrollbars=yes,height=600, width=850,location=0,left=50,top=50");
	return false;
}
function onClickSecurity() {
	window.open("/cgi-bin/security.asp","Security","toolbar=no,menubar=no,scrollbars=yes,height=600, width=850,location=0,left=50,top=50");
	return false;
}
function onClickWds() {
	//window.open("/cgi-bin/wifiqa_wds.asp","WDS","toolbar=no,menubar=no,scrollbars=yes,height=600, width=850,location=0,left=50,top=50");
	location.replace("/cgi-bin/wifiqa_wds.asp");
	return false;
}
function onClickWps() {
	window.open("/cgi-bin/wps.asp","WPS","toolbar=no,menubar=no,scrollbars=yes,height=600, width=850,location=0,left=50,top=50");
	return false;
}
function onClickStatistic() {
	//window.open("/cgi-bin/wifiqa_apstatistics.asp","ap_statistics","toolbar=no,menubar=no,scrollbars=yes,height=600, width=850,location=0,left=50,top=50");
	location.replace("/cgi-bin/wifiqa_apstatistics.asp");
	return false;
}
</script>
</head><body onLoad="">
<FORM METHOD="POST" ACTION="/cgi-bin/home_advancedwireless.asp" name="advancedwireless_form">
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
    <tr> 
      <td height="5" class="light-orange" colspan="6">&nbsp;</td>
    </tr>
    <tr> 
      <td width="150" height="30" class="title-main"> <font color="#FFFFFF">Advanced 
        Wireless</font> </td>
      <td width="10" class="black">&nbsp;</td>
      <td width="50">&nbsp;</td>
      <td width="10" ></td>
      <td width="20">&nbsp;</td>
      <td width="520">&nbsp;</td>
    </tr>
    <tr> 
      <td height="25" class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata">&nbsp;</td>
      <td class="tabdata"><INPUT type=button style="width: 80px;" value="Basic" name=btn_Basic onclick="onClickBasic()"></td>
      <td class="tabdata">&nbsp;</td>
      <td class="tabdata">You could configure the minimum number of Wireless settings 
        for communication. </td>
    </tr>
    <tr> 
      <td width="150" height="25" class="light-orange"><font color="#FFFFFF">&nbsp;</font></td>
      <td width="10" class="light-orange">&nbsp;</td>
      <td class="tabdata"><hr noshade></td>
      <td class="tabdata"><hr noshade></td>
      <td class="tabdata"><hr noshade></td>
      <td class="tabdata"><hr noshade></td>
    </tr>
    <tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata">&nbsp;</td>
      <td class="tabdata"><INPUT type=button style="width: 80px;" value="Advanced" name=btn_Advanced onclick="onClickAdvanced()"></td>
      <td class="tabdata">&nbsp;</td>
      <td class="tabdata">Use the Advanced Setup page to make detailed settings 
        for the Wireless. Advanced Setup includes items that are not available 
        from the Basic Setup page.</td>
    </tr>
    <tr> 
      <td width="150" height="25" class="light-orange"><font color="#FFFFFF">&nbsp;</font></td>
      <td width="10" class="light-orange">&nbsp;</td>
      <td class="tabdata"><hr noshade></td>
      <td class="tabdata"><hr noshade></td>
      <td class="tabdata"><hr noshade></td>
      <td class="tabdata"><hr noshade></td>
    </tr>
	<tr> 
      <td class="light-orange">&nbsp;</td>
      <td class="light-orange"></td>
      <td class="tabdata">&nbsp;</td>
      <td class="tabdata"><input type="button" style="width: 80px;" name="btn_Statistic" value="Statistics" onclick="onClickStatistic()"></td>
      <td class="tabdata">&nbsp;</td>
      <td class="tabdata">Wireless TX and RX Statistics</td>
    </tr>
	<tr>
    	<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;</td>
	</tr>
  </table>

  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

  <tr>

    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">

		<INPUT TYPE="HIDDEN" NAME="SaveBtn" VALUE="SAVE" onClick="">	</td></tr></table></form>

</body></html>

