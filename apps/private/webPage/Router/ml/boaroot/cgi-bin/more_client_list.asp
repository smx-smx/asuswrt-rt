
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">

<script language="JavaScript">

function showTable(id,header,data,keyIndex){
	var html = [""];
	html.push("<table id=client_list width=580 border=1 align=center cellpadding=1 cellspacing=0  bordercolor=#CCCCCC bgcolor=#FFFFFF>");
	
	// 1.generate table header
	html.push("<tr>");
	for(var i =0; i<header.length; i++){
		html.push("<td width=" + header[i][0] + " align=center class=tabdata>" +"<STRONG><FONT color=#000000>"+ header[i][1] +"</font></strong>"+ "</td>");
	}
	html.push("</tr>");
	// 2.generate table data
	for(var i =0; i<data.length; i++){
		if(data[i][keyIndex] != "N/A"){
			html.push("<tr>");
			for(var j=0; j<data[i].length; j++){
				html.push("<td align=center class=tabdata>" + data[i][j] + "</td>");
			}
			html.push("</tr>");
		}
	}
	html.push("</table>");
	if(parseInt(document.dchp_client_list.LeaseNum.value)>210)
	{
		html.push("<input type=button name=MORE  value=\" 1 \" onClick=javascript:window.location=\'/cgi-bin/more_client_list.asp\'>");
    		html.push("<input type=button name=MORE  value=\" 2 \" onClick=javascript:window.location=\'/cgi-bin/more_client_list_2.asp\'>");
  		html.push("<input type=button name=MORE  value=\" 3 \" onClick=javascript:window.location=\'/cgi-bin/more_client_list_3.asp\'>");
	}
	else if(parseInt(document.dchp_client_list.LeaseNum.value)>110)
	{
		html.push("<input type=button name=MORE  value=\" 1 \" onClick=javascript:window.location=\'/cgi-bin/more_client_list.asp\'>");
    		html.push("<input type=button name=MORE  value=\" 2 \" onClick=javascript:window.location=\'/cgi-bin/more_client_list_2.asp\'>");
  	//	html.push("<input type=button name=MORE  value=\" 3 \" onClick=javascript:window.location=\'/cgi-bin/more_client_list_3.asp\'>");
	}
	else
	{
		html.push("<input type=button name=MORE  value=\" 1 \" onClick=javascript:window.location=\'/cgi-bin/more_client_list.asp\'>");

	}
	document.getElementById(id).innerHTML = html.join('');
}
</script>

<style type="text/css">

</style>
</head>
<body>
<FORM METHOD="POST" ACTION="/cgi-bin/more_client_list.asp" name="dchp_client_list">
<INPUT type="HIDDEN" name="LeaseNum" value="<%TcWebApi_get("DhcpLease","LeaseNum","s")%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF" id="uiViewBodyTable">
  <tr>
      <td height="4" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
  	<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","LANDHCPListText","s")%></font></td>
 	 <td width="10" class="black">&nbsp;</td>
  	<td width="150"><hr noshade></td>
  	<td width= "10"><hr noshade></td>
 	 <td width="440"><hr noshade> </td>
  </tr>	 
  <tr>
  	<td width="150" class="light-orange">&nbsp;</td>
  	<td width="10"  class="light-orange"></td>
  	<td colspan="3" align=center>
	<div id=dhcpclientList></div>
<script language=JavaScript>
var tableHeader = [
	["50","#"],
	["140","<%tcWebApi_get("String_Entry","LANDTABLEHOSTNAMEText","s")%>"],
	["150","<%tcWebApi_get("String_Entry","LANDTABLEIPText","s")%>"],
	["140","<%tcWebApi_get("String_Entry","LANDTABLEMACText","s")%>"],
	["100","<%tcWebApi_get("String_Entry","LANDTABLEEXPIREText","s")%>"]
];
var tableData = [
	["11", "<%tcWebApi_get("DhcpLease_Entry10", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry10", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry10", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry10", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry10", "ExpireTime","s")%>"],
	["12", "<%tcWebApi_staticGet("DhcpLease_Entry11", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry11", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry11", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry11", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry11", "ExpireTime","s")%>"],
	["13", "<%tcWebApi_staticGet("DhcpLease_Entry12", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry12", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry12", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry12", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry12", "ExpireTime","s")%>"],
	["14", "<%tcWebApi_staticGet("DhcpLease_Entry13", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry13", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry13", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry13", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry13", "ExpireTime","s")%>"],
	["15", "<%tcWebApi_staticGet("DhcpLease_Entry14", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry14", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry14", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry14", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry14", "ExpireTime","s")%>"],
	["16", "<%tcWebApi_staticGet("DhcpLease_Entry15", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry15", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry15", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry15", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry15", "ExpireTime","s")%>"],
	["17", "<%tcWebApi_staticGet("DhcpLease_Entry16", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry16", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry16", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry16", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry16", "ExpireTime","s")%>"],
	["18", "<%tcWebApi_staticGet("DhcpLease_Entry17", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry17", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry17", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry17", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry17", "ExpireTime","s")%>"],
	["19", "<%tcWebApi_staticGet("DhcpLease_Entry18", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry18", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry18", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry18", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry18", "ExpireTime","s")%>"],
	["20", "<%tcWebApi_staticGet("DhcpLease_Entry19", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry19", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry19", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry19", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry19", "ExpireTime","s")%>"],
	["21", "<%tcWebApi_staticGet("DhcpLease_Entry20", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry20", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry20", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry20", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry20", "ExpireTime","s")%>"],
	["22", "<%tcWebApi_staticGet("DhcpLease_Entry21", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry21", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry21", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry21", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry21", "ExpireTime","s")%>"],
	["23", "<%tcWebApi_staticGet("DhcpLease_Entry22", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry22", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry22", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry22", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry22", "ExpireTime","s")%>"],
	["24", "<%tcWebApi_staticGet("DhcpLease_Entry23", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry23", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry23", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry23", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry23", "ExpireTime","s")%>"],
	["25", "<%tcWebApi_staticGet("DhcpLease_Entry24", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry24", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry24", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry24", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry24", "ExpireTime","s")%>"],
	["26", "<%tcWebApi_staticGet("DhcpLease_Entry25", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry25", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry25", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry25", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry25", "ExpireTime","s")%>"],
	["27", "<%tcWebApi_staticGet("DhcpLease_Entry26", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry26", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry26", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry26", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry26", "ExpireTime","s")%>"],
	["28", "<%tcWebApi_staticGet("DhcpLease_Entry27", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry27", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry27", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry27", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry27", "ExpireTime","s")%>"],
	["29", "<%tcWebApi_staticGet("DhcpLease_Entry28", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry28", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry28", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry28", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry28", "ExpireTime","s")%>"],
	["30", "<%tcWebApi_staticGet("DhcpLease_Entry29", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry29", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry29", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry29", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry29", "ExpireTime","s")%>"],
	["31", "<%tcWebApi_staticGet("DhcpLease_Entry30", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry30", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry30", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry30", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry30", "ExpireTime","s")%>"],
	["32", "<%tcWebApi_staticGet("DhcpLease_Entry31", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry31", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry31", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry31", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry31", "ExpireTime","s")%>"],
	["33", "<%tcWebApi_staticGet("DhcpLease_Entry32", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry32", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry32", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry32", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry32", "ExpireTime","s")%>"],
	["34", "<%tcWebApi_staticGet("DhcpLease_Entry33", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry33", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry33", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry33", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry33", "ExpireTime","s")%>"],
	["35", "<%tcWebApi_staticGet("DhcpLease_Entry34", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry34", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry34", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry34", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry34", "ExpireTime","s")%>"],
	["36", "<%tcWebApi_staticGet("DhcpLease_Entry35", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry35", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry35", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry35", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry35", "ExpireTime","s")%>"],
	["37", "<%tcWebApi_staticGet("DhcpLease_Entry36", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry36", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry36", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry36", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry36", "ExpireTime","s")%>"],
	["38", "<%tcWebApi_staticGet("DhcpLease_Entry37", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry37", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry37", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry37", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry37", "ExpireTime","s")%>"],
	["39", "<%tcWebApi_staticGet("DhcpLease_Entry38", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry38", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry38", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry38", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry38", "ExpireTime","s")%>"],
	["40", "<%tcWebApi_staticGet("DhcpLease_Entry39", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry39", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry39", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry39", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry39", "ExpireTime","s")%>"],
	["41", "<%tcWebApi_staticGet("DhcpLease_Entry40", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry40", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry40", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry40", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry40", "ExpireTime","s")%>"],
	["42", "<%tcWebApi_staticGet("DhcpLease_Entry41", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry41", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry41", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry41", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry41", "ExpireTime","s")%>"],
	["43", "<%tcWebApi_staticGet("DhcpLease_Entry42", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry42", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry42", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry42", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry42", "ExpireTime","s")%>"],
	["44", "<%tcWebApi_staticGet("DhcpLease_Entry43", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry43", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry43", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry43", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry43", "ExpireTime","s")%>"],
	["45", "<%tcWebApi_staticGet("DhcpLease_Entry44", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry44", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry44", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry44", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry44", "ExpireTime","s")%>"],
	["46", "<%tcWebApi_staticGet("DhcpLease_Entry45", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry45", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry45", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry45", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry45", "ExpireTime","s")%>"],
	["47", "<%tcWebApi_staticGet("DhcpLease_Entry46", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry46", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry46", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry46", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry46", "ExpireTime","s")%>"],
	["48", "<%tcWebApi_staticGet("DhcpLease_Entry47", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry47", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry47", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry47", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry47", "ExpireTime","s")%>"],
	["49", "<%tcWebApi_staticGet("DhcpLease_Entry48", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry48", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry48", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry48", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry48", "ExpireTime","s")%>"],
	["50", "<%tcWebApi_staticGet("DhcpLease_Entry49", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry49", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry49", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry49", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry49", "ExpireTime","s")%>"],
	["51", "<%tcWebApi_staticGet("DhcpLease_Entry50", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry50", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry50", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry50", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry50", "ExpireTime","s")%>"],
	["52", "<%tcWebApi_staticGet("DhcpLease_Entry51", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry51", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry51", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry51", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry51", "ExpireTime","s")%>"],
	["53", "<%tcWebApi_staticGet("DhcpLease_Entry52", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry52", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry52", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry52", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry52", "ExpireTime","s")%>"],
	["54", "<%tcWebApi_staticGet("DhcpLease_Entry53", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry53", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry53", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry53", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry53", "ExpireTime","s")%>"],
	["55", "<%tcWebApi_staticGet("DhcpLease_Entry54", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry54", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry54", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry54", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry54", "ExpireTime","s")%>"],
	["56", "<%tcWebApi_staticGet("DhcpLease_Entry55", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry55", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry55", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry55", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry55", "ExpireTime","s")%>"],
	["57", "<%tcWebApi_staticGet("DhcpLease_Entry56", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry56", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry56", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry56", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry56", "ExpireTime","s")%>"],
	["58", "<%tcWebApi_staticGet("DhcpLease_Entry57", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry57", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry57", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry57", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry57", "ExpireTime","s")%>"],
	["59", "<%tcWebApi_staticGet("DhcpLease_Entry58", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry58", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry58", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry58", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry58", "ExpireTime","s")%>"],
	["60", "<%tcWebApi_staticGet("DhcpLease_Entry59", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry59", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry59", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry59", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry59", "ExpireTime","s")%>"],
	["61", "<%tcWebApi_staticGet("DhcpLease_Entry60", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry60", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry60", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry60", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry60", "ExpireTime","s")%>"],
	["62", "<%tcWebApi_staticGet("DhcpLease_Entry61", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry61", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry61", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry61", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry61", "ExpireTime","s")%>"],
	["63", "<%tcWebApi_staticGet("DhcpLease_Entry62", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry62", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry62", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry62", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry62", "ExpireTime","s")%>"],
	["64", "<%tcWebApi_staticGet("DhcpLease_Entry63", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry63", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry63", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry63", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry63", "ExpireTime","s")%>"],
	["65", "<%tcWebApi_staticGet("DhcpLease_Entry64", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry64", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry64", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry64", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry64", "ExpireTime","s")%>"],
	["66", "<%tcWebApi_staticGet("DhcpLease_Entry65", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry65", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry65", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry65", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry65", "ExpireTime","s")%>"],
	["67", "<%tcWebApi_staticGet("DhcpLease_Entry66", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry66", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry66", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry66", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry66", "ExpireTime","s")%>"],
	["68", "<%tcWebApi_staticGet("DhcpLease_Entry67", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry67", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry67", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry67", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry67", "ExpireTime","s")%>"],
	["69", "<%tcWebApi_staticGet("DhcpLease_Entry68", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry68", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry68", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry68", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry68", "ExpireTime","s")%>"],
	["70", "<%tcWebApi_staticGet("DhcpLease_Entry69", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry69", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry69", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry69", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry69", "ExpireTime","s")%>"],
	["71", "<%tcWebApi_staticGet("DhcpLease_Entry70", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry70", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry70", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry70", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry70", "ExpireTime","s")%>"],
	["72", "<%tcWebApi_staticGet("DhcpLease_Entry71", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry71", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry71", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry71", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry71", "ExpireTime","s")%>"],
	["73", "<%tcWebApi_staticGet("DhcpLease_Entry72", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry72", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry72", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry72", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry72", "ExpireTime","s")%>"],
	["74", "<%tcWebApi_staticGet("DhcpLease_Entry73", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry73", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry73", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry73", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry73", "ExpireTime","s")%>"],
	["75", "<%tcWebApi_staticGet("DhcpLease_Entry74", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry74", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry74", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry74", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry74", "ExpireTime","s")%>"],
	["76", "<%tcWebApi_staticGet("DhcpLease_Entry75", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry75", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry75", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry75", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry75", "ExpireTime","s")%>"],
	["77", "<%tcWebApi_staticGet("DhcpLease_Entry76", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry76", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry76", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry76", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry76", "ExpireTime","s")%>"],
	["78", "<%tcWebApi_staticGet("DhcpLease_Entry77", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry77", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry77", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry77", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry77", "ExpireTime","s")%>"],
	["79", "<%tcWebApi_staticGet("DhcpLease_Entry78", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry78", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry78", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry78", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry78", "ExpireTime","s")%>"],
	["80", "<%tcWebApi_staticGet("DhcpLease_Entry79", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry79", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry79", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry79", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry79", "ExpireTime","s")%>"],
	["81", "<%tcWebApi_staticGet("DhcpLease_Entry80", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry80", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry80", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry80", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry80", "ExpireTime","s")%>"],
	["82", "<%tcWebApi_staticGet("DhcpLease_Entry81", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry81", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry81", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry81", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry81", "ExpireTime","s")%>"],
	["83", "<%tcWebApi_staticGet("DhcpLease_Entry82", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry82", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry82", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry82", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry82", "ExpireTime","s")%>"],
	["84", "<%tcWebApi_staticGet("DhcpLease_Entry83", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry83", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry83", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry83", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry83", "ExpireTime","s")%>"],
	["85", "<%tcWebApi_staticGet("DhcpLease_Entry84", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry84", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry84", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry84", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry84", "ExpireTime","s")%>"],
	["86", "<%tcWebApi_staticGet("DhcpLease_Entry85", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry85", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry85", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry85", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry85", "ExpireTime","s")%>"],
	["87", "<%tcWebApi_staticGet("DhcpLease_Entry86", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry86", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry86", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry86", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry86", "ExpireTime","s")%>"],
	["88", "<%tcWebApi_staticGet("DhcpLease_Entry87", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry87", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry87", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry87", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry87", "ExpireTime","s")%>"],
	["89", "<%tcWebApi_staticGet("DhcpLease_Entry88", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry88", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry88", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry88", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry88", "ExpireTime","s")%>"],
	["90", "<%tcWebApi_staticGet("DhcpLease_Entry89", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry89", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry89", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry89", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry89", "ExpireTime","s")%>"],
	["91", "<%tcWebApi_staticGet("DhcpLease_Entry90", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry90", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry90", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry90", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry90", "ExpireTime","s")%>"],
	["92", "<%tcWebApi_staticGet("DhcpLease_Entry91", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry91", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry91", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry91", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry91", "ExpireTime","s")%>"],
	["93", "<%tcWebApi_staticGet("DhcpLease_Entry92", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry92", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry92", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry92", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry92", "ExpireTime","s")%>"],
	["94", "<%tcWebApi_staticGet("DhcpLease_Entry93", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry93", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry93", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry93", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry93", "ExpireTime","s")%>"],
	["95", "<%tcWebApi_staticGet("DhcpLease_Entry94", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry94", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry94", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry94", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry94", "ExpireTime","s")%>"],
	["96", "<%tcWebApi_staticGet("DhcpLease_Entry95", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry95", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry95", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry95", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry95", "ExpireTime","s")%>"],
	["97", "<%tcWebApi_staticGet("DhcpLease_Entry96", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry96", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry96", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry96", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry96", "ExpireTime","s")%>"],
	["98", "<%tcWebApi_staticGet("DhcpLease_Entry97", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry97", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry97", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry97", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry97", "ExpireTime","s")%>"],
	["99", "<%tcWebApi_staticGet("DhcpLease_Entry98", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry98", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry98", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry98", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry98", "ExpireTime","s")%>"],
	["100", "<%tcWebApi_staticGet("DhcpLease_Entry99", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry99", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry99", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry99", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry99", "ExpireTime","s")%>"],	
	["101", "<%tcWebApi_staticGet("DhcpLease_Entry100", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry100", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry100", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry100", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry100", "ExpireTime","s")%>"],
	["102", "<%tcWebApi_staticGet("DhcpLease_Entry101", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry101", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry101", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry101", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry101", "ExpireTime","s")%>"],
	["103", "<%tcWebApi_staticGet("DhcpLease_Entry102", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry102", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry102", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry102", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry102", "ExpireTime","s")%>"],
	["104", "<%tcWebApi_staticGet("DhcpLease_Entry103", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry103", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry103", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry103", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry103", "ExpireTime","s")%>"],
	["105", "<%tcWebApi_staticGet("DhcpLease_Entry104", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry104", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry104", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry104", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry104", "ExpireTime","s")%>"],
	["106", "<%tcWebApi_staticGet("DhcpLease_Entry105", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry105", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry105", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry105", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry105", "ExpireTime","s")%>"],
	["107", "<%tcWebApi_staticGet("DhcpLease_Entry106", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry106", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry106", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry106", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry106", "ExpireTime","s")%>"],
	["108", "<%tcWebApi_staticGet("DhcpLease_Entry107", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry107", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry107", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry107", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry107", "ExpireTime","s")%>"],
	["109", "<%tcWebApi_staticGet("DhcpLease_Entry108", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry108", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry108", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry108", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry108", "ExpireTime","s")%>"],
	["110", "<%tcWebApi_staticGet("DhcpLease_Entry109", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry109", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry109", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry109", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry109", "ExpireTime","s")%>"]

];

		showTable('dhcpclientList',tableHeader,tableData,2);
		</script>
</td>
</tr>
</table>
 <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

  <tr>
    <td width="150" class="light-orange">&nbsp;</td>
    <td width="10"  class="light-orange"></td>
    <td width="150">&nbsp;</td>
    <td width="10" >&nbsp;</td>
    <td width="440">

    </td>
  </tr>
</table>
    <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

  <tr height="42" >

    <td width="160" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange"></td>
   </tr>
  </table>
</form>
</body>
</html>
