<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<!--aidisk/popCreateAccount.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>Add New Account</title>
<link rel="stylesheet" href="/form_style.css" type="text/css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript">
function clickevent(){
	$("Submit").onclick = function(){
		if(validForm()){
			parent.showLoading();
			document.createAccountForm.submit();
			parent.hidePop("apply");
		}
	};
}
function validForm(){
	showtext($("alert_msg2"), "");

	//account
        if($("account").value.length == 0){
                alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc1","s")%>");
                $("account").focus();
                return false;
        }
        else{
                var alert_str = validate_hostname($("account"));

                if(alert_str != ""){
                        alert(alert_str);
                        $("account").focus();
                        return false;
                }
                        

                $("account").value = trim($("account").value);

                if($("account").value == "root"
                                || $("account").value == "guest"
                                || $("account").value == "anonymous"
                ){
                                alert("<%tcWebApi_get("String_Entry","USB_App_account_alert","s")%>");
                                $("account").focus();
                                return false;
                }
                else if(checkDuplicateName($("account").value, parent.get_accounts())){
                                alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc5","s")%>");
                                $("account").focus();
                                return false;
                }
	}

        // password
        if($("password").value.length <= 0 || $("confirm_password").value.length <= 0){
                showtext($("alert_msg2"),"*<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc6","s")%>");
                if($("password").value.length <= 0){
                                $("password").focus();
                                $("password").select();
                }else{
                                $("confirm_password").focus();
                                $("confirm_password").select();
                }
                return false;
        }

        if($("password").value != $("confirm_password").value){
                showtext($("alert_msg2"),"*<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s")%>");
                $("confirm_password").focus();
                return false;
        }

        if(!validate_string(document.createAccountForm.password)){
                $("password").focus();
                $("password").select();
                return false;
        }

        if($("password").value.length > 16){
                showtext($("alert_msg2"),"*<%tcWebApi_get("String_Entry","LHC_x_Password_id","s")%>");
                $("password").focus();
                $("password").select();
                return false;
        }

	var re = new RegExp("[^a-zA-Z0-9]+","gi");
	if(re.test($("password").value)){
		showtext($("alert_msg2"),"<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc9","s")%>");
		$("password").focus();
		return false;
	}

	return true;
}
</script>
</head>
<body onLoad="clickevent();">
<form method="post" name="createAccountForm" action="create_account.asp" target="hidden_frame">
<table width="90%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<thead>
		<tr>
			<td colspan="2"><span style="color:#FFF"><%tcWebApi_get("String_Entry","AddAccountTitle","s")%></span><img src="/images/button-close.gif" onClick="parent.hidePop('OverlayMask');"></td>
		</tr>
	</thead>
	<tbody>
		<tr align="center">
			<td height="25" colspan="2"><%tcWebApi_get("String_Entry","AddAccountAlert","s")%></td>
		</tr>
		<tr>
			<th><%tcWebApi_get("String_Entry","AiDisk_Account","s")%>: </th>
			<td>
				<input class="input_15_table" name="account" id="account" type="text" maxlength="20">
			</td>
		</tr>
		<tr>
			<th><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%>: </th>
			<td><input class="input_15_table" name="password" id="password" type="password" maxlength="17"></td>
		</tr>
		<tr>
			<th><%tcWebApi_get("String_Entry","Confirmpassword","s")%>: </th>
			<td><input class="input_15_table" name="confirm_password" id="confirm_password" type="password" maxlength="17">
				<br/><span id="alert_msg2" style="color:#FC0;margin-left:8px;"></span>
			</td>
		</tr>
		<tr bgcolor="#E6E6E6">
			<td colspan="2" align="right">
				<input id="Submit" type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_add","s")%>">
			</td>
		</tr>
	</tbody>
</table>
</form>
</body>

<!--aidisk/popCreateAccount.asp-->
</html>

