<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--device-map/remote.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>Untitled Document</title>
<link rel="stylesheet" type="text/css" href="../NM_style.css">
<link rel="stylesheet" type="text/css" href="../form_style.css">
<script type="text/javascript" src="/formcontrol.js"></script>
<script type="text/javasscript" src="../state.js"></script>
<script>
var remoteIP = '';
remoteIP = (remoteIP == '')?'';
var re_status = parent.getConnectingStatus();
function initial(){
if(re_status == -1){
		showtext($("Connstatus"), "<%tcWebApi_get("String_Entry","OP_RE_item","s")%>");
$("remoteIP_tr").style.display = "none";
setTimeout("set_re_status();",6000);
}
else
set_re_status();
		
	$("remoteIP_span").innerHTML = (remoteIP == "")?"<%tcWebApi_get("String_Entry","AP_fail_get_IPaddr","s")%>":remoteIP;
}
function set_re_status(){
re_status = parent.getConnectingStatus();
if(re_status == 2){
		showtext($("Connstatus"), "<%tcWebApi_get("String_Entry","Connected","s")%>");
$("remoteIP_tr").style.display = "";
}
else{
		showtext($("Connstatus"), "<%tcWebApi_get("String_Entry","CTL_Disconnect","s")%>");
$("remoteIP_tr").style.display = "none";
}
}
function sbtnOver(o){
o.style.color = "#FFFFFF";
o.style.background = "url(/images/sbtn.gif) #FFCC66";
o.style.cursor = "pointer";
}
function sbtnOut(o){
o.style.color = "#000000";
o.style.background = "url(/images/sbtn0.gif) #FFCC66";
}
</script>
</head>
<body class="statusbody" onload="initial();">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
<tr>
    <th width="120"><%tcWebApi_get("String_Entry","PPPC_x_WANLink_in","s")%></th>
<td width="150"><span id="Connstatus"></span></td>
</tr>
<tr id="remoteIP_tr">
    <th><%tcWebApi_get("String_Entry","AP_Remote_IP","s")%></th>
<td><span id="remoteIP_span"></span></td>
</tr>
<tr>
    <th><%tcWebApi_get("String_Entry","AP_survey","s")%></th>
<td><input type="button" class="sbtn" value="<%tcWebApi_get("String_Entry","btn_go","s")%>" onclick="javascript:parent.location.href='../survey.asp';" onmouseover="sbtnOver(this);" onmouseout="sbtnOut(this);"></td>
</tr>
</table>
</body>

<!--device-map/remote.asp-->
</html>

