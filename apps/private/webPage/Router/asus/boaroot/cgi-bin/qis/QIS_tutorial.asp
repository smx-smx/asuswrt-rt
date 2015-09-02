<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_tutorial.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<style type="text/css">
span{
border:0px solid #FFFFFF;
color:#FFFFFF;
font-size:14px;
font-family:Arial, Helvetica, sans-serif;
/*width:27px;*/
text-align:right;
margin:0px auto;
ime-mode:disabled;
}
.QISTutorFont{
width:250px;
line-height: 20px;
}
</style>
<script type="text/Javascript" src="/state.js"></script>
<script type="text/javascript">
function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	//parent.set_step("t3");
	var OSName="Unknown OS";
	var OS = navigator.appVersion;
	var ie = false;
	if (/MSIE (\d+\.\d+);/.test(navigator.userAgent))
		ie = true;
	if(!ie){
		OS = navigator.userAgent;
	}
	if(OS.indexOf("Win")!=-1){
		if((OS.indexOf("Windows NT 5.1")!=-1) || (OS.indexOf("Windows XP")!=-1)){
			document.getElementById("winxp").style.display = "";
		}
		else if((OS.indexOf("Windows NT 7.0")!=-1) || (OS.indexOf("Windows NT 6.1")!=-1) || (OS.indexOf("Windows NT 6.0")!=-1)){
			document.getElementById("win7").style.display = "";
		}
	}
	else if(navigator.userAgent.match(/iP/i)){
		document.getElementById("ios").style.display = "";
	}else if(navigator.userAgent.match(/Android/i)){
		document.getElementById("android").style.display = "";
	}else if(OS.indexOf("Mac")!=-1){
		document.getElementById("mac").style.display = "";
	}else{
		document.getElementById("win7").style.display = "";
	}
}

function gotoIndex(){
	parent.location.href = '../index2.asp';
}
</script>
</head>
<body onLoad="QKfinish_load_body();" >
<div class="QISmain">
<div class="formfonttitle" style="padding:6 0 0 10;">
<div>
<table width="730px">
<tr>
<td align="left">
<span class="description_down"><% tcWebApi_Get("String_Entry", "qis_tutor_title1", "s") %></span>
</td>
</tr>
</table>
</div>
<div style="margin:5px;"><img style="width: 720px; *width: 710px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
</div>
<br/>
<div style="margin-left:30px;display:none;" id="win7">
<span><% tcWebApi_Get("String_Entry", "qis_tutor_desc1", "s") %></span><br><br><br><br>
<div class="QISTutorFont"><% tcWebApi_Get("String_Entry", "qis_tutor_desc2", "s") %></div><br><br>
<div class="QISTutorFont"><% tcWebApi_Get("String_Entry", "qis_tutor_desc3", "s") %></div>
<div style="margin-left:300px;margin-top:-150px;"><img src="/images/qis/win7.jpg"></span></div>
</div>
<div style="margin-left:30px;display:none;" id="winxp">
<span><% tcWebApi_Get("String_Entry", "qis_tutor_desc1", "s") %></span><br><br><br><br>
<div class="QISTutorFont"><% tcWebApi_Get("String_Entry", "qis_tutor_desc4", "s") %></div><br><br>
<div class="QISTutorFont"><% tcWebApi_Get("String_Entry", "qis_tutor_desc3", "s") %></div>
<div style="margin-left:280px;margin-top:-160px;"><img height="300" width="400" src="/images/qis/winxp.jpg"></span></div>
</div>
<div style="margin-left:30px;display:none;" id="mac">
<span><% tcWebApi_Get("String_Entry", "qis_tutor_desc1", "s") %></span><br><br>
<span><% tcWebApi_Get("String_Entry", "qis_tutor_desc5", "s") %></span><br><br>
<div style="margin-left:30px;margin-top:10px;"><img height="280" src="/images/qis/mac.jpg"></span></div>
</div>
<div style="margin-left:30px;display:none;" id="ios">
<span>Your wireless network name and security key had changed. To connect your iOS device to wireless network, follow these steps.</span><br><br>
<br><br>
<span>1. Tap the setting icon.</span>
<div style="margin-left:30px;margin-top:30px;"><img src="/images/qis/iOS1.jpg"></div>
<br><br>
<div style="margin-left:200px;margin-top:-177px;">2. Tap the Wi-Fi tab and choose the network you want to connect to.</div>
<div style="margin-left:220px;margin-top:30px;"><img src="/images/qis/iOS2.jpg"></div>
<br><br>
</div>
<div style="margin-left:30px;display:none;" id="android">
<span>Your wireless network name and security key had changed. To connect your android device to wireless network, follow these steps.</span>
<br><br>
<span>1. Tap the setting icon to enter the setting center.</span>
<br><br>
<span>2. Tap the Wireless Manager</span>
<div style="margin-left:30px;margin-top:30px;"><img src="/images/qis/android2.jpg"></div>
<br><br>
<span>3. Tap the Wi-Fi </span>
<div style="margin-left:30px;margin-top:30px;"><img src="/images/qis/android3.jpg"></div>
<br><br>
<span>4. In the Wi-Fi network area, tab the network you want to connect to, and then click connect.</span>
<div style="margin-left:30px;margin-top:30px;"><img src="/images/qis/android4.jpg"></div>
<br><br>
</div>
<div class="apply_gen" style="margin-top:15px">
<input type="button" id="nextButton" value="Finish" onclick="gotoIndex();" class="button_gen">
</div>
</body>

<!--qis/QIS_tutorial.asp-->
</html>

