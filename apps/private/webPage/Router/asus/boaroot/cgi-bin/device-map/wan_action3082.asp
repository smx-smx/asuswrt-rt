<html>

<!--device-map/wan_action.asp?wanaction=Disconnect-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<script>
var action = 'Disconnect';
var restart_time;
var wan_proto = 'pppoe';
function restart_needed_time(second){
if('0' == "1")
restart_time = 60;
else
restart_time = second;
}
function Callback(){
parent.showLoading(restart_time);
setTimeout("document.redirectForm.submit();", restart_time*1000);
return;
}
</script>
</head>
<body onLoad="Callback();">
<script>restart_needed_time(0);</script>

<form method="post" name="redirectForm" action="index.asp" target="_parent">
<input type="hidden" name="flag" value="">
</form>
</body>

<!--device-map/wan_action.asp?wanaction=Disconnect-->
</html>

