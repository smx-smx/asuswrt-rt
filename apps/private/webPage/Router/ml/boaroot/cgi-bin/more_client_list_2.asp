
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
	["111", "<%tcWebApi_get("DhcpLease_Entry110", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry110", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry110", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry110", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry110", "ExpireTime","s")%>"],
	["112", "<%tcWebApi_staticGet("DhcpLease_Entry111", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry111", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry111", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry111", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry111", "ExpireTime","s")%>"],
	["113", "<%tcWebApi_staticGet("DhcpLease_Entry112", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry112", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry112", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry112", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry112", "ExpireTime","s")%>"],
	["114", "<%tcWebApi_staticGet("DhcpLease_Entry113", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry113", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry113", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry113", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry113", "ExpireTime","s")%>"],
	["115", "<%tcWebApi_staticGet("DhcpLease_Entry114", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry114", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry114", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry114", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry114", "ExpireTime","s")%>"],
	["116", "<%tcWebApi_staticGet("DhcpLease_Entry115", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry115", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry115", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry115", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry115", "ExpireTime","s")%>"],
	["117", "<%tcWebApi_staticGet("DhcpLease_Entry116", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry116", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry116", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry116", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry116", "ExpireTime","s")%>"],
	["118", "<%tcWebApi_staticGet("DhcpLease_Entry117", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry117", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry117", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry117", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry117", "ExpireTime","s")%>"],
	["119", "<%tcWebApi_staticGet("DhcpLease_Entry118", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry118", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry118", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry118", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry118", "ExpireTime","s")%>"],
	["120", "<%tcWebApi_staticGet("DhcpLease_Entry119", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry119", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry119", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry119", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry119", "ExpireTime","s")%>"],
	["121", "<%tcWebApi_staticGet("DhcpLease_Entry120", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry120", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry120", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry120", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry120", "ExpireTime","s")%>"],
	["122", "<%tcWebApi_staticGet("DhcpLease_Entry121", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry121", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry121", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry121", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry121", "ExpireTime","s")%>"],
	["123", "<%tcWebApi_staticGet("DhcpLease_Entry122", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry122", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry122", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry122", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry122", "ExpireTime","s")%>"],
	["124", "<%tcWebApi_staticGet("DhcpLease_Entry123", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry123", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry123", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry123", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry123", "ExpireTime","s")%>"],
	["125", "<%tcWebApi_staticGet("DhcpLease_Entry124", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry124", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry124", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry124", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry124", "ExpireTime","s")%>"],
	["126", "<%tcWebApi_staticGet("DhcpLease_Entry125", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry125", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry125", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry125", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry125", "ExpireTime","s")%>"],
	["127", "<%tcWebApi_staticGet("DhcpLease_Entry126", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry126", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry126", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry126", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry126", "ExpireTime","s")%>"],
	["128", "<%tcWebApi_staticGet("DhcpLease_Entry127", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry127", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry127", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry127", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry127", "ExpireTime","s")%>"],
	["129", "<%tcWebApi_staticGet("DhcpLease_Entry128", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry128", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry128", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry128", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry128", "ExpireTime","s")%>"],
	["130", "<%tcWebApi_staticGet("DhcpLease_Entry129", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry129", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry129", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry129", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry129", "ExpireTime","s")%>"],
	["131", "<%tcWebApi_staticGet("DhcpLease_Entry130", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry130", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry130", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry130", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry130", "ExpireTime","s")%>"],
	["132", "<%tcWebApi_staticGet("DhcpLease_Entry131", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry131", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry131", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry131", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry131", "ExpireTime","s")%>"],
	["133", "<%tcWebApi_staticGet("DhcpLease_Entry132", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry132", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry132", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry132", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry132", "ExpireTime","s")%>"],
	["134", "<%tcWebApi_staticGet("DhcpLease_Entry133", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry133", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry133", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry133", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry133", "ExpireTime","s")%>"],
	["135", "<%tcWebApi_staticGet("DhcpLease_Entry134", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry134", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry134", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry134", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry134", "ExpireTime","s")%>"],
	["136", "<%tcWebApi_staticGet("DhcpLease_Entry135", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry135", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry135", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry135", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry135", "ExpireTime","s")%>"],
	["137", "<%tcWebApi_staticGet("DhcpLease_Entry136", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry136", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry136", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry136", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry136", "ExpireTime","s")%>"],
	["138", "<%tcWebApi_staticGet("DhcpLease_Entry137", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry137", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry137", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry137", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry137", "ExpireTime","s")%>"],
	["139", "<%tcWebApi_staticGet("DhcpLease_Entry138", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry138", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry138", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry138", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry138", "ExpireTime","s")%>"],
	["140", "<%tcWebApi_staticGet("DhcpLease_Entry139", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry139", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry139", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry139", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry139", "ExpireTime","s")%>"],
	["141", "<%tcWebApi_staticGet("DhcpLease_Entry140", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry140", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry140", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry140", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry140", "ExpireTime","s")%>"],
	["142", "<%tcWebApi_staticGet("DhcpLease_Entry141", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry141", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry141", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry141", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry141", "ExpireTime","s")%>"],
	["143", "<%tcWebApi_staticGet("DhcpLease_Entry142", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry142", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry142", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry142", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry142", "ExpireTime","s")%>"],
	["144", "<%tcWebApi_staticGet("DhcpLease_Entry143", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry143", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry143", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry143", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry143", "ExpireTime","s")%>"],
	["145", "<%tcWebApi_staticGet("DhcpLease_Entry144", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry144", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry144", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry144", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry144", "ExpireTime","s")%>"],
	["146", "<%tcWebApi_staticGet("DhcpLease_Entry145", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry145", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry145", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry145", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry145", "ExpireTime","s")%>"],
	["147", "<%tcWebApi_staticGet("DhcpLease_Entry146", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry146", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry146", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry146", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry146", "ExpireTime","s")%>"],
	["148", "<%tcWebApi_staticGet("DhcpLease_Entry147", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry147", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry147", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry147", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry147", "ExpireTime","s")%>"],
	["149", "<%tcWebApi_staticGet("DhcpLease_Entry148", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry148", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry148", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry148", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry148", "ExpireTime","s")%>"],
	["150", "<%tcWebApi_staticGet("DhcpLease_Entry149", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry149", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry149", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry149", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry149", "ExpireTime","s")%>"],
	["151", "<%tcWebApi_staticGet("DhcpLease_Entry150", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry150", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry150", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry150", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry150", "ExpireTime","s")%>"],
	["152", "<%tcWebApi_staticGet("DhcpLease_Entry151", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry151", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry151", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry151", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry151", "ExpireTime","s")%>"],
	["153", "<%tcWebApi_staticGet("DhcpLease_Entry152", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry152", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry152", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry152", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry152", "ExpireTime","s")%>"],
	["154", "<%tcWebApi_staticGet("DhcpLease_Entry153", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry153", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry153", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry153", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry153", "ExpireTime","s")%>"],
	["155", "<%tcWebApi_staticGet("DhcpLease_Entry154", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry154", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry154", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry154", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry154", "ExpireTime","s")%>"],
	["156", "<%tcWebApi_staticGet("DhcpLease_Entry155", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry155", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry155", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry155", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry155", "ExpireTime","s")%>"],
	["157", "<%tcWebApi_staticGet("DhcpLease_Entry156", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry156", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry156", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry156", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry156", "ExpireTime","s")%>"],
	["158", "<%tcWebApi_staticGet("DhcpLease_Entry157", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry157", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry157", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry157", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry157", "ExpireTime","s")%>"],
	["159", "<%tcWebApi_staticGet("DhcpLease_Entry158", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry158", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry158", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry158", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry158", "ExpireTime","s")%>"],
	["160", "<%tcWebApi_staticGet("DhcpLease_Entry159", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry159", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry159", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry159", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry159", "ExpireTime","s")%>"],
	["161", "<%tcWebApi_staticGet("DhcpLease_Entry160", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry160", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry160", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry160", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry160", "ExpireTime","s")%>"],
	["162", "<%tcWebApi_staticGet("DhcpLease_Entry161", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry161", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry161", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry161", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry161", "ExpireTime","s")%>"],
	["163", "<%tcWebApi_staticGet("DhcpLease_Entry162", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry162", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry162", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry162", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry162", "ExpireTime","s")%>"],
	["164", "<%tcWebApi_staticGet("DhcpLease_Entry163", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry163", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry163", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry163", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry163", "ExpireTime","s")%>"],
	["165", "<%tcWebApi_staticGet("DhcpLease_Entry164", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry164", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry164", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry164", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry164", "ExpireTime","s")%>"],
	["166", "<%tcWebApi_staticGet("DhcpLease_Entry165", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry165", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry165", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry165", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry165", "ExpireTime","s")%>"],
	["167", "<%tcWebApi_staticGet("DhcpLease_Entry166", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry166", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry166", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry166", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry166", "ExpireTime","s")%>"],
	["168", "<%tcWebApi_staticGet("DhcpLease_Entry167", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry167", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry167", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry167", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry167", "ExpireTime","s")%>"],
	["169", "<%tcWebApi_staticGet("DhcpLease_Entry168", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry168", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry168", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry168", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry168", "ExpireTime","s")%>"],
	["170", "<%tcWebApi_staticGet("DhcpLease_Entry169", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry169", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry169", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry169", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry169", "ExpireTime","s")%>"],
	["171", "<%tcWebApi_staticGet("DhcpLease_Entry170", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry170", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry170", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry170", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry170", "ExpireTime","s")%>"],
	["172", "<%tcWebApi_staticGet("DhcpLease_Entry171", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry171", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry171", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry171", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry171", "ExpireTime","s")%>"],
	["173", "<%tcWebApi_staticGet("DhcpLease_Entry172", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry172", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry172", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry172", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry172", "ExpireTime","s")%>"],
	["174", "<%tcWebApi_staticGet("DhcpLease_Entry173", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry173", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry173", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry173", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry173", "ExpireTime","s")%>"],
	["175", "<%tcWebApi_staticGet("DhcpLease_Entry174", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry174", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry174", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry174", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry174", "ExpireTime","s")%>"],
	["176", "<%tcWebApi_staticGet("DhcpLease_Entry175", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry175", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry175", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry175", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry175", "ExpireTime","s")%>"],
	["177", "<%tcWebApi_staticGet("DhcpLease_Entry176", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry176", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry176", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry176", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry176", "ExpireTime","s")%>"],
	["178", "<%tcWebApi_staticGet("DhcpLease_Entry177", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry177", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry177", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry177", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry177", "ExpireTime","s")%>"],
	["179", "<%tcWebApi_staticGet("DhcpLease_Entry178", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry178", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry178", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry178", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry178", "ExpireTime","s")%>"],
	["180", "<%tcWebApi_staticGet("DhcpLease_Entry179", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry179", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry179", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry179", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry179", "ExpireTime","s")%>"],
	["181", "<%tcWebApi_staticGet("DhcpLease_Entry180", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry180", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry180", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry180", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry180", "ExpireTime","s")%>"],
	["182", "<%tcWebApi_staticGet("DhcpLease_Entry181", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry181", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry181", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry181", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry181", "ExpireTime","s")%>"],
	["183", "<%tcWebApi_staticGet("DhcpLease_Entry182", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry182", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry182", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry182", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry182", "ExpireTime","s")%>"],
	["184", "<%tcWebApi_staticGet("DhcpLease_Entry183", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry183", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry183", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry183", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry183", "ExpireTime","s")%>"],
	["185", "<%tcWebApi_staticGet("DhcpLease_Entry184", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry184", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry184", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry184", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry184", "ExpireTime","s")%>"],
	["186", "<%tcWebApi_staticGet("DhcpLease_Entry185", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry185", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry185", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry185", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry185", "ExpireTime","s")%>"],
	["187", "<%tcWebApi_staticGet("DhcpLease_Entry186", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry186", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry186", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry186", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry186", "ExpireTime","s")%>"],
	["188", "<%tcWebApi_staticGet("DhcpLease_Entry187", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry187", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry187", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry187", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry187", "ExpireTime","s")%>"],
	["189", "<%tcWebApi_staticGet("DhcpLease_Entry188", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry188", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry188", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry188", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry188", "ExpireTime","s")%>"],
	["190", "<%tcWebApi_staticGet("DhcpLease_Entry189", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry189", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry189", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry189", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry189", "ExpireTime","s")%>"],
	["191", "<%tcWebApi_staticGet("DhcpLease_Entry190", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry190", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry190", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry190", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry190", "ExpireTime","s")%>"],
	["192", "<%tcWebApi_staticGet("DhcpLease_Entry191", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry191", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry191", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry191", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry191", "ExpireTime","s")%>"],
	["193", "<%tcWebApi_staticGet("DhcpLease_Entry192", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry192", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry192", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry192", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry192", "ExpireTime","s")%>"],
	["194", "<%tcWebApi_staticGet("DhcpLease_Entry193", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry193", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry193", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry193", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry193", "ExpireTime","s")%>"],
	["195", "<%tcWebApi_staticGet("DhcpLease_Entry194", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry194", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry194", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry194", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry194", "ExpireTime","s")%>"],
	["196", "<%tcWebApi_staticGet("DhcpLease_Entry195", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry195", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry195", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry195", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry195", "ExpireTime","s")%>"],
	["197", "<%tcWebApi_staticGet("DhcpLease_Entry196", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry196", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry196", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry196", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry196", "ExpireTime","s")%>"],
	["198", "<%tcWebApi_staticGet("DhcpLease_Entry197", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry197", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry197", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry197", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry197", "ExpireTime","s")%>"],
	["199", "<%tcWebApi_staticGet("DhcpLease_Entry198", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry198", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry198", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry198", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry198", "ExpireTime","s")%>"],
	["200", "<%tcWebApi_staticGet("DhcpLease_Entry199", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry199", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry199", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry199", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry199", "ExpireTime","s")%>"],
	["201", "<%tcWebApi_staticGet("DhcpLease_Entry200", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry200", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry200", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry200", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry200", "ExpireTime","s")%>"],
	["202", "<%tcWebApi_staticGet("DhcpLease_Entry201", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry201", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry201", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry201", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry201", "ExpireTime","s")%>"],
	["203", "<%tcWebApi_staticGet("DhcpLease_Entry202", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry202", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry202", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry202", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry202", "ExpireTime","s")%>"],
	["204", "<%tcWebApi_staticGet("DhcpLease_Entry203", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry203", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry203", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry203", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry203", "ExpireTime","s")%>"],
	["205", "<%tcWebApi_staticGet("DhcpLease_Entry204", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry204", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry204", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry204", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry204", "ExpireTime","s")%>"],
	["206", "<%tcWebApi_staticGet("DhcpLease_Entry205", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry205", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry205", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry205", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry205", "ExpireTime","s")%>"],
	["207", "<%tcWebApi_staticGet("DhcpLease_Entry206", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry206", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry206", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry206", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry206", "ExpireTime","s")%>"],
	["208", "<%tcWebApi_staticGet("DhcpLease_Entry207", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry207", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry207", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry207", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry207", "ExpireTime","s")%>"],
	["209", "<%tcWebApi_staticGet("DhcpLease_Entry208", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry208", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry208", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry208", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry208", "ExpireTime","s")%>"],
	["210", "<%tcWebApi_staticGet("DhcpLease_Entry209", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry209", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry209", "MAC","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry209", "ExpireDay","s")%>" + "days " + "<%tcWebApi_staticGet("DhcpLease_Entry209", "ExpireTime","s")%>"]

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
    <td width="440">&nbsp;</td>
  </tr>
</table>
    <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">

  <tr height="42" >

    <td width="160" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
    </td>
   </tr>
  </table>
</form>
</body>
</html>
