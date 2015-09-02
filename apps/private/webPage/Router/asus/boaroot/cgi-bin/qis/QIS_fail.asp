<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_fail.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="../form_style.css">
<link rel="stylesheet" type="text/css" href="qis_style.css">
<style>
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
</style>
<script type="text/javascript">
var wantype = '';
var wanstate = -1;
var wansbstate = -1;
var wanauxstate = -1;
function wanlink_status() { return 0;}
function wanlink_statusstr() { return '<% tcWebApi_Get("String_Entry", "Disconnected", "s") %>';}
function wanlink_type() { return 'pppoe';}
function wanlink_ipaddr() { return '0.0.0.0';}
function wanlink_netmask() { return '0.0.0.0';}
function wanlink_gateway() { return '0.0.0.0';}
function wanlink_dns() { return '';}
function wanlink_lease() { return ;}
function is_private_subnet() { return '0';}

wanstate = 4;
wansbstate = 0;
wanauxstate = 1;

function QKfinish_load_body(){
parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
parent.set_step("t3");
wantype = wanlink_type();
if( wansbstate == 2 && wantype != "dhcp" && wantype != "static"){
document.getElementById("FailReason").innerHTML = "Authentication failed.";
document.getElementById("AuthFail").style.display = "";
}
else if(wansbstate == 1 && wantype != "dhcp" && wantype != "static"){
document.getElementById("FailReason").innerHTML = "PPPoE/DSL Connection failed";
document.getElementById("PPPConnFail").style.display = "";
if(wantype == "pppoe")
document.getElementById("wanproto").innerHTML = "PPPoE";
else if(wantype == "pptp")
document.getElementById("wanproto").innerHTML = "PPTP";
else if(wantype == "l2tp")
document.getElementById("wanproto").innerHTML = "L2TP";
else
document.getElementById("wanproto").innerHTML = "Internet setting is error!";
}
else{
document.getElementById("FailReason").innerHTML = "Unable to connect to the Internet";
document.getElementById("ConnFail").style.display = "";
}
}
function redirect_page(redirect_flag){
document.redirectForm.action = "/qis/QIS_"+redirect_flag+".asp";
document.redirectForm.submit();
}
function goWireless(){
location.href = 'QIS_wireless.asp';
}
</script>
</head>
<body onLoad="QKfinish_load_body();" >
<form method="post" name="redirectForm" action="#" target="">
<input type="hidden" name="prev_page" value="">
<input type="hidden" name="current_page" value="QIS_fail.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<div class="QISmain">
<div class="formfonttitle" style="padding:0 0 0 10;" id="FailReason">Unable to connect to the Internet</div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img style="width: 700px; height: 2px;" src="../images/New_ui/export/line_export.png"></div>
<br/>
<div id="ConnFail" style="display:none;">
<p class="QISGeneralFont">Unable to connect to the Internet. Please double-check your account data with your ISP or contact ASUS Support Hotline for technical inquiries and support.</p>
<div>
<p class='QISGeneralFont'>
Click <b>Wireless Setting</b> to enter the login information again.
Click <b>Select Internet Type</b> to select another Internet connection type.
</p>
</div>
<div class="apply_gen" style="margin-top:30px">
<input type="button" id="nextButton" value="Select Internet Type" onclick="redirect_page('internet_type');" class="button_gen_long" >
<input type="button" id="nextButton" value="Wireless Setting" onclick="goWireless();" class="button_gen_long">
</div>
</div>
<div id="AuthFail" style="display:none;">
<p class="QISGeneralFont">The account name and password are not correct.</p>
<div>
<p class='QISGeneralFont'>
Click <b>Wireless Setting</b> to enter the login information again.
Click <b>Select Internet Type</b> to select another Internet connection type.
</p>
</div>
<div class="apply_gen" style="margin-top:30px">
<input type="button" id="nextButton" value="Select Internet Type" onclick="redirect_page('internet_type');" class="button_gen_long" >
<input type="button" id="nextButton" value="Wireless Setting" onclick="goWireless();" class="button_gen_long">
</div>
<div>
<div id="PPPConnFail" style="display:none;">
<p class="QISGeneralFont">The account name and password has passed authentication but failed to get an IP address from your ISP. Please contact your ISP for assistance.</p>
<table class="QISform_finish" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
<tr>
<th width="180"><span>Internet Type</span></th>
<td class="QISformtd"><span id="wanproto"></span></td>
</tr>
<tr>
<th width="180"><span>Account Name</span></th>
<td class="QISformtd"><span>aaaaaaaa</span></td>
</tr>
<tr>
<th width="180"><span>Password</span></th>
<td class="QISformtd"><span>bbbbbbbb</span></td>
</tr>
<tr>
<th width="180"><span>WAN IP</span></th>
<td class="QISformtd"><span></span></td>
</tr>
<tr>
<th width="180"><span>LAN IP</span></th>
<td class="QISformtd"><span><%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%></span></td>
</tr>
<tr>
<th width="180"><span>MAC</span></th>
<td class="QISformtd"><span>00:26:B6:DA:59:C2</span></td>
</tr>
</table>
<div>
<p class='QISGeneralFont'>
Click <b>Wireless Setting</b> to enter the login information again.
Click <b>Select Internet Type</b> to select another Internet connection type.
</p>
</div>
<div class="apply_gen" style="margin-top:30px">
<input type="button" id="nextButton" value="Select Internet Type" onclick="redirect_page('internet_type');" class="button_gen_long" >
<input type="button" id="nextButton" value="Wireless Setting" onclick="goWireless();" class="button_gen_long">
</div>
<div>
</div>
</form>
</body>

<!--qis/QIS_fail.asp-->
</html>

