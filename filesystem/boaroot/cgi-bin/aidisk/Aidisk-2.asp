<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--aidisk/Aidisk-2.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>AiDisk Wizard</title>
<link rel="stylesheet" type="text/css" href="/aidisk/aidisk.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script>
var next_page = "";
var dummyShareway = 0;
var acc0_name = "<% tcWebApi_get("Account_Entry0","username", "s") %>";

function initial(){
	parent.hideLoading();
	parent.restore_help_td();
	parent.$("dummyShareway").value = 2;
	showTextinWizard(parent.$("dummyShareway").value);
}

var et0macaddr_array = '<% tcWebApi_get("Info_Ether","mac","s") %>'.split(':');
function showTextinWizard(flag){
	dummyShareway = flag;
	if(dummyShareway == 0){
		parent.$("dummyShareway").value = dummyShareway;
		document.getElementsByName('dummyoption')[2].focus();
		document.getElementsByName('dummyoption')[2].checked = true;
		document.getElementById("share0_Hint").style.display = "";
		document.getElementById("share1").style.display = "none";
		document.getElementById("target1").style.display = "none";
		document.getElementById("target2").style.display = "none";
	}
	else if(dummyShareway == 1){
		parent.$("dummyShareway").value = dummyShareway;
		document.getElementsByName('dummyoption')[1].focus();
		document.getElementsByName('dummyoption')[1].checked = true;
		document.getElementById("share0_Hint").style.display = "none";
		showtext(document.getElementById("user1"), acc0_name);
		showtext(document.getElementById("user2"), "Family");
		document.getElementById("userpasswd2").value = "family" + et0macaddr_array[et0macaddr_array.length-2].toLowerCase() + et0macaddr_array[et0macaddr_array.length-1].toLowerCase();
		document.getElementById("share1").style.display = "block";
		document.getElementById("target1").style.display = "";
		document.getElementById("target2").style.display = "";
	}
	else if(dummyShareway == 2){
		parent.$("dummyShareway").value = dummyShareway;
		document.getElementsByName('dummyoption')[0].focus();
		document.getElementsByName('dummyoption')[0].checked = true;		
		document.getElementById("share0_Hint").style.display = "none";
		showtext(document.getElementById("user1"), acc0_name);
		document.getElementById("share1").style.display = "";
		document.getElementById("target1").style.display = "";
		document.getElementById("target2").style.display = "none";
	}
	else
		alert("System error: No this choice"); // no translate*/
}
function passTheResult(){
	if(dummyShareway == 0){
		parent.$("accountNum").value = 0;
		/*parent.$("account0").value = "";
		parent.$("passwd0").value = "";
		parent.$("permission0").value = "";//*/
		parent.$("account1").value = "";
		parent.$("passwd1").value = "";
		parent.$("permission1").value = "";
	}
	else if(dummyShareway == 1){
	parent.$("accountNum").value = 2;
		/*if(checkPasswdValid($("userpasswd1").value)){
		parent.$("account0").value = $("user1").firstChild.nodeValue;
		parent.$("passwd0").value = $("userpasswd1").value;
		parent.$("permission0").value = "3";
		}
		else{
		$("userpasswd1").focus();
		return;
		}//*/
		if(checkPasswdValid($("userpasswd2").value)){
			parent.$("account1").value = $("user2").firstChild.nodeValue;
			parent.$("passwd1").value = $("userpasswd2").value;
			parent.$("permission1").value = "1";
		}
		else{
			$("userpasswd2").focus();
			return;
		}
	}
	else if(dummyShareway == 2){
		parent.$("accountNum").value = 1;
		/*if(checkPasswdValid($("userpasswd1").value)){
		parent.$("account0").value = $("user1").firstChild.nodeValue;
		parent.$("passwd0").value = $("userpasswd1").value;
		parent.$("permission0").value = "3";
		}
		else{
		$("userpasswd1").focus();
		return;
		}//*/
		parent.$("account1").value = "";
		parent.$("passwd1").value = "";
		parent.$("permission1").value = "";
	}
	document.smartForm.action = "Aidisk-3.asp";
	document.smartForm.submit();
}
function go_pre_page(){
	document.smartForm.action = "Aidisk-1.asp";
	document.smartForm.submit();
}
function checkPasswdValid(passwd){
	var tempPasswd = trim(passwd);
	if(tempPasswd.length != passwd.length){
		alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc8","s")%>");
		
		return false;
	}
	if(trim(tempPasswd).length <= 0){
		alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc6","s")%>");
		
		return false;
	}
	var re = new RegExp("[^a-zA-Z0-9]+","gi");
	if(re.test(tempPasswd)){
		alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc9","s")%>");
		
		return false;
	}
	return true;
}
</script>
</head>
<body onload="initial();">
<form method="GET" name="smartForm" id="smartForm" action="Aidisk-3.asp">
<input type="hidden" name="accountNum" id="accountNum" value="">
<input type="hidden" name="account0" id="account0" value="">
<input type="hidden" name="passwd0" id="passwd0" value="">
<input type="hidden" name="permission0" id="permission0" value="">
<input type="hidden" name="account1" id="account1" value="">
<input type="hidden" name="passwd1" id="passwd1" value="">
<input type="hidden" name="permission1" id="permission1" value="">
</form>
<div class="aidisk1_table">
<table> <!-- width="765" height="760" border="0" bgcolor="#4d595d" cellpadding="0" cellspacing="0" style="padding:10px; padding-top:20px;" //Viz-->
<tr>
<td valign="top" bgcolor="#4d595d" style="padding-top:25px;">
<table width="740" height="225" border="0">
	<tr>
		<td>
		<table width="740" border="0">
		<tr>
			<td width="15%" height="90px" style="background:url(/images/New_ui/aidisk/step1.png) 0% 95% no-repeat;"></td>
			<td width="15%"><img src="/images/New_ui/aidisk/steparrow.png" /></td>
			<td width="15%" height="90px" style="background:url(/images/New_ui/aidisk/step2.png) 0% 0% no-repeat;"></td>
			<td width="15%"><img src="/images/New_ui/aidisk/steparrow.png" /></td>
			<td width="15%" height="90px" style="background:url(/images/New_ui/aidisk/step3.png) 0% 0% no-repeat;"></td>
			<td width="25%">&nbsp;</td>
		</tr>
		</table>
		</td>
	</tr>
	<tr>
    	<td align="left" class="formfonttitle" style="padding-left:20px;" height="72"><%tcWebApi_get("String_Entry","Step2_method","s")%>: <%tcWebApi_get("String_Entry","Step1_desp","s")%></td>
	</tr>
	<tr>
		<td valign="top">
			<div style="margin-left:20px;">
				<br/>
					<p><input type="radio" id="d3" name="dummyoption" value="2" width="10" onclick="showTextinWizard(this.value);"/>
            	<label for="d3"><%tcWebApi_get("String_Entry","Step2_method3","s")%></label>
					</p>				
				<br/>
					<p><input type="radio" id="d2" name="dummyoption" value="1" width="10" onclick="showTextinWizard(this.value);"/>
            	<label for="d2"><%tcWebApi_get("String_Entry","Step2_method2","s")%></label>
					</p>
				<br/>	
					<p><input type="radio" id="d1" name="dummyoption" value="0" width="10" onclick="showTextinWizard(this.value);"/>
            	<label for="d1"><%tcWebApi_get("String_Entry","Step2_method1","s")%></label>
					</p>				
			</div>
					<div id="share0_Hint" style="margin-top:10px; margin-left:45px; color:#FC0;"><span><%tcWebApi_get("String_Entry","AiDisk_shareHint","s")%></span></div>
			<div id="share1" style="margin-top:30px;">
			<table width="80%" border="1" align="center" cellpadding="2" cellspacing="0" bordercolor="#7ea7bd" class="FormTable_table">
				<tr>
                  	<th width="100"><%tcWebApi_get("String_Entry","AiDisk_Account","s")%></th>
                  	<th><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></th>
                  	<th width="50" ><%tcWebApi_get("String_Entry","AiDisk_Read","s")%></th>
                  	<th width="50" ><%tcWebApi_get("String_Entry","AiDisk_Write","s")%></th>
				</tr>
				<tr id="target1">
					<td height="35"><span id="user1" style="color:#FFFFFF;"></span></td>
					<td>*</td>
					<td align="center"><img src="/images/New_ui/checkbox.png"></td>
					<td align="center"><img src="/images/New_ui/checkbox.png"></td>
				</tr>
				<tr id="target2">
					<td height="35"><span id="user2" style="color:#FFFFFF;"></span></td>
					<td><input type="text" name="userpasswd2" id="userpasswd2" value="" maxlength="16" class="input_25_table"></td>
					<td align="center"><img src="/images/New_ui/checkbox.png"></td>
					<td align="center">&nbsp;</td>
				</tr>
			</table>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-top:30px;"><img src="/images/New_ui/export/line_export.png" /></div>
		</td>
	</tr>
	<tr valign="bottom" align="center">
		<td width="20%">
			<div class="apply_gen" style="margin-top:30px">
				<input type="button" id="prevButton" value="<%tcWebApi_get("String_Entry","btn_pre","s")%>" onclick="go_pre_page();" class="button_gen">
				<input type="submit" id="nextButton" value="<%tcWebApi_get("String_Entry","btn_next","s")%>" onclick="passTheResult();" class="button_gen">
			</div>
		</td>
	</tr>
</table>
</td>
</tr>
</table>
</div> </body>

<!--aidisk/Aidisk-2.asp-->
</html>

