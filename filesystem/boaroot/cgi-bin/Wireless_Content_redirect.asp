<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_Wireless_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - General</title>
<script type="text/javascript" src="/general.js"></script>
<script>
var refresh = getURLParameter("refresh");

function initial(){
	if(refresh){
		if(refresh == "2")//direct to 2.4G
		{
			document.form.wl_unit.value = 0;
		}
		else if(refresh == "5")//direct to 5G
		{
			document.form.wl_unit.value = 1;
		}
		document.form.submit();
	}
}
</script>
</head>
<body onload="initial();">
<form method="post" name="form" action="Advanced_Wireless_Content.asp">
<input type="hidden" name="wl_unit">
<input type="hidden" name="editFlag" value="0">
</form>
</body>
</html>
