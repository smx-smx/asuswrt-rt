<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_no_cable.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link type="text/css" rel="stylesheet" href="/qis/qis_style.css">
<link type="text/css" rel="stylesheet" href="/form_style.css">
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
<script type="text/JavaSscript" src="/jquery.js"></script>
<script type="text/javascript">
/*
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

wanstate = 1;
wansbstate = 0;
wanauxstate = 1;

*/
function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	parent.set_step("t3");
	setTimeout('getWANStatus();', 1000);
/*
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
*/
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
				if(dsl_autodet_state != "down")
					redirect_page("detect");
				setTimeout('getWANStatus();', 1000);
			}
	});
}
function redirect_page(redirect_flag){
	//document.redirectForm.action = "QIS_"+redirect_flag+".asp";
	//document.redirectForm.submit();
	location.href = "QIS_"+redirect_flag+".asp";
}
function goWireless(){
	location.href = '/qis/QIS_wireless.asp';
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
	<!--div class="formfonttitle" style="padding:0 0 0 10;" id="FailReason">Unable to connect to the Internet</div-->
	<div>
	<table width="730px">
		<tr>
			<td align="left">
				<span class="description_down">Unable to connect to the Internet</span>
			</td>
			<!--td align="right">
				<img onclick="gotoIndex();" style="cursor:pointer;" align="right" title="Go to Home" src="/images/backtohome.png" onMouseOver="this.src='/images/backtohomeclick.png'" onMouseOut="this.src='/images/backtohome.png'">
			</td-->
		</tr>
	</table>
	</div>
	<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img style="width: 700px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
	<br/>
	<p class="QISGeneralFont"><% tcWebApi_Get("String_Entry", "QIS_NoCable_title", "s") %></p>
	<p class='QISGeneralFont'>Please ensure that the phone cable is connected to the DSL port.</p>
	</p>
	<div class="apply_gen" style="margin-top:30px">
		<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "Manual_Setting_btn", "s") %>" onclick="redirect_page('manual_setting');" class="button_gen_long">
		<input type="button" id="nextButton" value="Wireless Setting" onclick="goWireless();" class="button_gen_long">
	</div>
</div>
</form>
</body>

<!--qis/QIS_no_cable.asp-->
</html>

