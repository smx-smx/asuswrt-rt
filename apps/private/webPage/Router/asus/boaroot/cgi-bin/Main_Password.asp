<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS Login</title>
<style>
.content{
	width:580px;
	height:526px;
	background:rgba(40,52,55,0.1);
}
.wrapper{
	background:url(/images/New_ui/login_bg.png) #283437 no-repeat;
	background-size: 1280px 1076px;
	background-position: center 0%;
	margin: 0px; 
}
.title_name {
	font-family:Arial;
	font-size: 40pt;
	color:#93d2d9;
}
.prod_madelName{
	font-family: Arial;
	font-size: 26pt;
	color:#fff;
}
.p1{
	font-family: Arial;
	font-size: 16pt;
	color:#fff;
}
.button{
	background-color: #279FD9;
	border: 0px;
	border-radius: 4px ;
	transition: visibility 0s linear 0.218s,opacity 0.218s,background-color 0.218s;
	height: 68px;
	width: 300px;
	font-family: Arial;
	font-size: 28pt;
	color:#fff;
	color:#000\9; /* IE6 IE7 IE8 */
	text-align:center;
	vertical-align:center
}
.button_text{
	font-family: Arial;
	font-size: 28pt;
	color:#fff;
	text-align:center;
	vertical-align:center
}
.form_input{
	background-color:rgba(255,255,255,0.2);
	border-radius: 4px;
	padding:26px 22px;
	width: 480px;
	border: 0;
	height:25px;
	color:#fff;
	color:#000\9; /* IE6 IE7 IE8 */
	font-size:28px
}
.form_input_text{
	font-family: Arial;
	font-size: 28pt;
	color:#a9a9a9;
}
.p2{
	font-family: Arial;
	font-size: 18pt;
	color:#28fff7;
}
</style>
<script>
var isIE8 = navigator.userAgent.search("MSIE 8") > -1; 
var isIE9 = navigator.userAgent.search("MSIE 9") > -1;
var redirect_page = '<% get_parameter("page"); %>';
var is_KR_sku = (function(){
	var ttc = '<%tcWebApi_staticGet("SysInfo_Entry", "TerritoryCode", "s") %>';
	return (ttc.search("KR") == -1) ? false : true;
})();

function initial(){
	if(is_KR_sku)
		document.getElementById("KRHint").style.display = "";
	if(isIE8 || isIE9){
		document.getElementById("router_name_tr").style.display = "";
		document.getElementById("router_password_tr").style.display = "";
		document.getElementById("router_password_confirm_tr").style.display = "";
	}

	var windowHeight = (function(){
		if(window.innerHeight)
			return window.innerHeight;
		else if(document.body && document.body.clientHeight)
			return document.body.clientHeight;
		else if(document.documentElement && document.documentElement.clientHeight)
			return document.documentElement.clientHeight;
		else
			return 800;
	})();

	document.getElementById("loginTable").style.height = windowHeight + "px";
	document.getElementById("loginTable").style.display = "";
	document.form.http_username_x.focus();

	document.form.http_username_x.onkeyup = function(e){
		if(e.keyCode == 13){
			document.form.http_passwd_x.focus();
		}
	};

	document.form.http_passwd_x.onkeyup = function(e){
		if(e.keyCode == 13){
			document.form.http_passwd_2_x.focus();
		}
	};

	document.form.http_passwd_2_x.onkeyup = function(e){
		if(e.keyCode == 13){
			submitForm();
		}
	};
}

function submitForm(){
	if(!validator.chkLoginId(document.form.http_username_x)){
		return false;
	}

	if(is_KR_sku){		/* MODELDEP by Territory Code */
		if(!validator.chkLoginPw_KR(document.form.http_passwd_x)){
			return false;
		}
	}
	else{
		if(!validator.chkLoginPw(document.form.http_passwd_x)){
			return false;
		}
	}

	document.getElementById("error_status_field").style.display = "none";
	document.form.http_username.value = document.form.http_username_x.value;
	document.form.http_passwd.value = document.form.http_passwd_x.value;
	document.form.http_username_x.disabled = true;
	document.form.http_passwd_x.disabled = true;
	document.form.http_passwd_2_x.disabled = true;
	document.form.btn_modify.style.display = "none";
	document.getElementById('loadingIcon').style.display = '';
	document.form.usernamepasswdFlag.value = 1;
	if(redirect_page == "" || redirect_page == "Logout.asp" || redirect_page == "Main_Login.asp" || redirect_page == "Main_Password.asp" || redirect_page.indexOf(" ") != -1 || (redirect_page.indexOf(".asp") == -1 && redirect_page.indexOf(".htm") == -1))
                document.form.next_page.value = "/index.asp";
        else
                document.form.next_page.value = redirect_page;
	document.form.submit();
}


var validator = {
	chkLoginId: function(obj){
		var re = new RegExp("^[a-zA-Z0-9][a-zA-Z0-9\-\_]+$","gi");

		if(obj.value == ""){
			showError("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc1","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}
		else if(re.test(obj.value)){
			if(obj.value == "root" || obj.value == "guest" || obj.value == "anonymous"){
				showError("<%tcWebApi_get("String_Entry","USB_App_account_alert","s")%>");
				obj.value = "";
				obj.focus();
				obj.select();
				return false;
			}

			return true;
		}
		else{
			showError("<%tcWebApi_get("String_Entry","JS_validhostname","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}
	},

	chkLoginPw: function(obj){
		if(obj.value == ""){
			showError("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc6","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}
		else if(obj.value == '<% tcWebApi_get("Account_Entry0","default_passwd","s") %>'){
			showError("<%tcWebApi_get("String_Entry","QIS_adminpass_confirm0","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}
		else if(obj.value.charAt(0) == '"'){
			showError('<%tcWebApi_get("String_Entry","JS_validstr1","s")%> ["]');
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}
		else{
			var invalid_char = ""; 
			for(var i = 0; i < obj.value.length; ++i){
				if(obj.value.charAt(i) < ' ' || obj.value.charAt(i) > '~'){
					invalid_char = invalid_char+obj.value.charAt(i);
				}
			}

			if(invalid_char != ""){
				showError("<%tcWebApi_get("String_Entry","JS_validstr2","s")%> '"+invalid_char+"' !");
				obj.value = "";
				obj.focus();
				obj.select();
				return false;
			}
		}

		if(obj.value != document.form.http_passwd_2_x.value){
			showError("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;			
		}

		return true;
	},
	
	chkLoginPw_KR: function(obj){		//Alphabets, numbers, specialcharacters mixed
		var string_length = obj.value.length;

		if(obj.value == ""){
			showError("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc6","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}
		else if(obj.value == '<% tcWebApi_get("Account_Entry0","default_passwd","s") %>'){
			showError("<%tcWebApi_get("String_Entry","QIS_adminpass_confirm0","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}
		else if(!/[A-Za-z]/.test(obj.value) || !/[0-9]/.test(obj.value) || string_length < 8
				|| !/[\!\"\#\$\%\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\?\@\[\\\]\^\_\`\{\|\}\~]/.test(obj.value)){
				
				showError("<%tcWebApi_get("String_Entry","JS_validPWD","s")%>");
				obj.value = "";
				obj.focus();
				obj.select();
				return false;	
		}	
		
		var invalid_char = "";
		for(var i = 0; i < obj.value.length; ++i){
			if(obj.value.charAt(i) <= ' ' || obj.value.charAt(i) > '~'){
				invalid_char = invalid_char+obj.value.charAt(i);
			}
		}

		if(invalid_char != ""){
			showError("<%tcWebApi_get("String_Entry","JS_validstr2","s")%> '"+invalid_char+"' !");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;
		}

		if(obj.value != document.form.http_passwd_2_x.value){
			showError("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s")%>");
			obj.value = "";
			obj.focus();
			obj.select();
			return false;			
		}

		return true;
	}
}

function showError(str){
	document.getElementById("error_status_field").style.display = "";
	document.getElementById("error_status_field").innerHTML = str;
}
</script>
</head>
<body class="wrapper" onload="initial();">
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>

<form method="post" name="form" action="/mod_login.asp" target="hidden_frame">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="current_page" value="Main_Password.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="flag" value="">
<input type="hidden" name="usernamepasswdFlag" value="0">
<input type="hidden" name="http_username" value="">
<input type="hidden" name="http_passwd" value="">
<input type="password" name="foilautofill" style="display: none;" >
<table id="loginTable" align="center" cellpadding="0" cellspacing="0" style="display:none">
	<tr>
		<td>
			<div>
				<table class="content">
					<tr style="height:43px;">
						<td style="width:73px" align="left">
							<div><img src="/images/New_ui/icon_titleName.png"></div>
						</td>
						<td align="left">
							<div class="title_name"><%tcWebApi_get("String_Entry","PASS_changepasswd","s")%></div>
						</td>
					</tr>
					<tr>
						<td colspan="2">
							<div class="p1" style="margin:35px 0px 0px 78px;">
								<div style="margin-bottom:10px;">
									<%tcWebApi_get("String_Entry","Web_Title2","s")%> is currently not protected and uses an unsafe default password.	<!-- untranslated  -->
								</div>
								<div style="margin-bottom:10px;">
									<%tcWebApi_get("String_Entry","QIS_pass_desc1","s")%>
								</div>
								<div id="KRHint" style="margin-bottom:10px;display:none">
									<%tcWebApi_get("String_Entry","JS_validPWD","s")%>
								</div>
							</div>
						</td>
					</tr>
					<tr id="router_name_tr" style="display:none">
						<td colspan="2">
							<div style="color:#FFF;margin:20px 0px -10px 78px;"><%tcWebApi_get("String_Entry","Router_Login_Name","s")%></div>
						</td>
					</tr>
					<tr style="height:72px;">
						<td colspan="2">
							<div style="margin:20px 0px 0px 78px;">
								<input type="text" name="http_username_x" tabindex="1" class="form_input" maxlength="20" value="" autocapitalization="off" autocomplete="off" placeholder="<%tcWebApi_get("String_Entry","Router_Login_Name","s")%>">
							</div>
						</td>
					</tr>

					<tr id="router_password_tr" style="display:none">
						<td colspan="2">
							<div style="color:#FFF;margin:20px 0px -20px 78px;"><%tcWebApi_get("String_Entry","PASS_new","s")%></div>
						</td>
					</tr>
					<tr style="height:72px;">
						<td colspan="2">
							<div style="margin:30px 0px 0px 78px;">
								<input type="password" autocapitalization="off" autocomplete="off" value="" name="http_passwd_x" tabindex="2" class="form_input" maxlength="16" onkeyup="" onpaste="return false;"/ onBlur="" placeholder="<%tcWebApi_get("String_Entry","PASS_new","s")%>">
							</div>
						</td>
					</tr>

					<tr id="router_password_confirm_tr" style="display:none">
						<td colspan="2">
							<div style="color:#FFF;margin:20px 0px -20px 78px;"><%tcWebApi_get("String_Entry","Confirmpassword","s")%></div>
						</td>
					</tr>
					<tr style="height:72px;">
						<td colspan="2">
							<div style="margin:30px 0px 0px 78px;">
								<input type="password" autocapitalization="off" autocomplete="off" value="" name="http_passwd_2_x" tabindex="3" class="form_input" maxlength="16" onkeyup="" onpaste="return false;"/ onBlur="" placeholder="<%tcWebApi_get("String_Entry","Confirmpassword","s")%>">
							</div>
						</td>
					</tr>
					<tr style="heigh:72px">
						<td colspan="2">
							<div style="color: rgb(255, 204, 0); margin:10px 0px -10px 78px; display:none;" id="error_status_field"></div>
						</td>
					</tr>
					<tr align="right" style="height:68px;">
						<td colspan="2">
							<div style="text-align: center;float:right; margin:50px 0px 0px 78px;">
								<input name="btn_modify" type="button" class="button" onclick="submitForm();" value="<%tcWebApi_get("String_Entry","CTL_modify","s")%>">
							</div>
							<div id="loadingIcon" style="display:none; margin:50px 0px 0px 0px;">
								<img style="width:35px;height:35px;" src="/images/InternetScan.gif">
							</div>	
						</td>
					</tr>
				</table>
			</div>
		</td>
	</tr>
</table>
</form>
</body>
</html>

