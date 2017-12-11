<%
If Request_Form("usernamepasswdFlag") = "1" Then
	TCWebApi_set("Account_Entry0","username","http_username")
	TCWebApi_set("Account_Entry0","web_passwd","http_passwd")
	TCWebApi_set("Account_Entry0","console_passwd","http_passwd")	
	tcWebApi_CommitWithoutSave("Account_Entry0")
	modify_aidisk_account()
End If
%>
<html>


<head>
<title></title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<script type="text/javascript">

var next_page = ('<% get_parameter("next_page") %>' == '')? '/index.asp' : '<% get_parameter("next_page") %>';
var getusernamepasswdFlag = '<% get_parameter("usernamepasswdFlag") %>';

function initial(){
	if(getusernamepasswdFlag == "1"){
		setTimeout("redirect_page(next_page)", 200);
	}
	else
		setTimeout("redirect_page(next_page)", 200);
}

function redirect_page(next_url){
	parent.location.href = next_url;
}

</script>
</head>
<body onload="initial();">
<form method="post" name="redirectForm" action="#" target="">
<input type="hidden" name="flag" value="">
<input type="hidden" name="prev_page" value="">
</form>

</body>

</html>

