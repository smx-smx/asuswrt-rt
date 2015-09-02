<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_verify_detect.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link type="text/css" rel="stylesheet" href="qis_style.css">
<link type="text/css" rel="stylesheet" href="../form_style.css">
<style>
.test_css{
padding-left:10px;
font-size:13px;
font-weight:bolder;
color:#FFFFFF;
/*background-color:#C0DAE4;*/
}
</style>
<script type="text/JavaSscript" src="../jquery.js"></script>
<script type="text/javascript">
var now_flag = '';
var wantype = '';
var wanstate = -1;
var wansbstate = -1;
var wanauxstate = -1;
var RetryCountDown = 5;
var successcase = 0;
var RetryCountDown_fail = 5;
function QKDetect_load_body(){
parent.set_step("t1");
parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_detect_sanglass","s")%>";
getWANStatus();
}
function getWANStatus(){
$.ajax({
url: '/result_of_get_changed_status_QIS.asp',
dataType: 'script',
error: function(xhr){
--RetryCountDown_fail;
if(RetryCountDown_fail < 1){
alert("<% tcWebApi_Get("String_Entry", "Disconnected", "s") %>!");
refreshpage();
}
else
getWANStatus();
},
success: function(response){
RetryCountDown_fail = 5;
if(wanstate == -1 || wanstate == 0 || wanstate == 1){
if(wanauxstate == 3 && wanstate == 1){ // PPP_AUTH_FAIL
alert("<% tcWebApi_Get("String_Entry", "QKS_Internet_Setup_fail_r2", "s") %>");
redirect_page("internet_account");
return true;
}
else
--RetryCountDown;
if(RetryCountDown < 1){
if(confirm("<% tcWebApi_Get("String_Entry", "QIS_detect_timeout", "s") %>")){
RetryCountDown = 5;
getWANStatus();
}
else
redirect_page("fail");
}
else
setTimeout("getWANStatus();", 2000);
}
else if(wanstate == 4 && wansbstate == 4){
parent.location.href = "../error_pagee152.asp?flag=6";
}
else{
if(wanauxstate != 1 && wanstate == 2){ //success case
successcase++;
if(successcase > 1)
redirect_page("wireless");
else
setTimeout("getWANStatus();", 2000);
}
else{
if(wanlink_type() == "dhcp" && now_flag != "resetModem")
redirect_page("resetModem");
else if(RetryCountDown > 0){
--RetryCountDown;
setTimeout("getWANStatus();", 2000);
}
else
redirect_page("fail");
}
}
}
});
}
function redirect_page(redirect_flag){
document.redirectForm.action = "/qis/QIS_"+redirect_flag+".asp";
document.redirectForm.submit();
}
</script>
</head>
<body onLoad="QKDetect_load_body();">
<form method="post" name="form" action="#" target="">
<input type="hidden" name="prev_page" value="">
<input type="hidden" name="current_page" value="QIS_detect.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="flag" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="preferred_lang" value="EN">
<div class="QISmain">
<div class="formfonttitle" style="padding:6 0 0 10;">
<div>
<table width="730px">
<tr>
<td align="left">
<span class="description_down"><% tcWebApi_Get("String_Entry", "QKS_detect_desc2", "s") %></span>
</td>
<!--td align="right">
<img onclick="parent.gotoIndex();" style="cursor:pointer;" align="right" title="Go to Home" src="../images/backtohome.png" onMouseOver="this.src='/images/backtohomeclick.png'" onMouseOut="this.src='/images/backtohome.png'">
</td-->
</tr>
</table>
</div>
<div style="margin:5px;"><img style="width: 720px; *width: 710px; height: 2px;" src="../images/New_ui/export/line_export.png"></div>
</div>
<br/><br/>
<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
<tr>
<td align="right">
<img src="../images/InternetScan.gif"></span>
</td>
<td class="test_css">
<strong><span><%tcWebApi_get("String_Entry","QKS_detect_waitdesc","s")%></span></strong>
</td>
</tr>
</table>
</div>
</form>
<form method="post" name="redirectForm" action="#">
<input type="hidden" name="flag" value="">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="prev_page" value="">
</form>
</body>

