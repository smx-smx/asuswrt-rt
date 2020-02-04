<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--PrinterServer.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png"><title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","Network_Printer_Server","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<style type="text/css">
	div.wrapper { margin: 0 auto; width: 730px;}
	td.sidenav { width:200px;}
	body {font-family: Verdana, Tohoma, Arial, Helvetica, sans-serif;padding:0;margin:0;}
	.wrapperDesc { margin: 0 auto; width: 570px;}
</style>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<style type="text/css">
.printerServer_table{
	width:740px;
	padding:5px;
	padding-top:20px;
	margin-top:-16px;
	position:relative;
	background-color:#4d595d;
	align:left;
	-webkit-border-radius: 3px;
	-moz-border-radius: 3px;
	border-radius: 3px;
	height: 760px;
}
.line_export{
	height:20px;
	width:736px;
}
.desctitle{
	font-size: 14px;
	font-weight: bolder;
	margin-left: 20px;
	margin-top: 15px;
}
.desc{
	margin-left: 20px;
	margin-top: 10px;
}
.descimage{
	margin-left: 20px;
	margin-top: 5px;
}
.statusBar{
	margin:auto;
}
.MethodDesc{
	font-style: italic;
	color: #999;
}
.imdShade{
	-moz-box-shadow: 15px 15px 10px rgba(0, 0, 0, 0.6);
	-webkit-box-shadow: 15px 15px 10px rgba(0, 0, 0, 0.6);
	box-shadow: 15px 15px 10px #333;
}
</style>
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
function initial(){
	show_menu();
	setTimeout("update_FAQ();", 300);
}

function update_FAQ(){
	if(document.getElementById("connect_status").className == "connectstatuson"){		
		faqURL("faq1", "https://www.asus.com", "/support/FAQ/", "114046");
		faqURL("faq2", "https://www.asus.com", "/support/FAQ/", "114024");
		faqURL("faq3", "https://www.asus.com", "/support/FAQ/", "113661");
	}
}
function showMethod(flag1, flag2){
	document.getElementById("method1").style.display = flag1;
	document.getElementById("method1Title").style.display = flag1;
	document.getElementById("method2").style.display = flag2;
	document.getElementById("method2Title").style.display = flag2;
	if(flag1 == ""){
		$("help1").style.color = "#FFF";
		$("help2").style.color = "gray";
	}
	else{
		$("help1").style.color = "gray";
		$("help2").style.color = "#FFF";
	}
}
</script>
</head>
<body onload="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>
<form method="post" name="form" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="current_page" value="PrinterServer.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
</form>
<table class="content" align="center" cellspacing="0" style="margin:auto;">
<tr>
	<td width="17">&nbsp;</td>
	<td valign="top" width="202">
		<div id="mainMenu"></div>
		<div id="subMenu"></div>
	</td>
	<td valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
		<br>
		<div class="printerServer_table" id="printerServer_table">
		<table>
			<tr>
				<td class="formfonttitle"><%tcWebApi_get("String_Entry","Network_Printer_Server","s")%>
				<img onclick="go_setting('APP_Installation.asp')" align="right" style="cursor:pointer;margin-right:10px;margin-top:-10px" title="Back to USB Extension" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'">
				</td>
			</tr>
			<tr>
				<td class="line_export"><img src="/images/New_ui/export/line_export.png" /></td>
			</tr>
			<tr>
   	<td><div class="formfontdesc"><%tcWebApi_get("String_Entry","Network_Printer_desc","s")%></div></td> 
			</tr>
			<tr>
				<td>
				<div id="mainbody">
				<div class="wrapper">
				<div class="shadow-l">
				<div class="shadow-r">
				<table class="" cellspacing="0" cellpadding="0">
				<tbody><tr valign="top">
				<td class="">
				<div class="padding">
				<div class="">
				<ul class="">
					<li>
						<a id="faq1" href="https://www.asus.com/support/FAQ/114046/" target="_blank" style="text-decoration:underline;font-size:14px;font-weight:bolder;color:#FFF"><%tcWebApi_get("String_Entry","asus_ez_print_share","s")%> FAQ</a>&nbsp;&nbsp;
						<a href="http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless/Printer.zip" style="text-decoration:underline;font-size:14px;font-weight:bolder;color:#FC0"><%tcWebApi_get("String_Entry","Download_now","s")%></a>
					</li>
					<li style="margin-top:10px;">
						<a id="faq2" href="https://www.asus.com/support/FAQ/114024/" target="_blank" style="text-decoration:underline;font-size:14px;font-weight:bolder;color:#FFF"><%tcWebApi_get("String_Entry","LPR_print_share","s")%> FAQ (Windows)</a>&nbsp;&nbsp;
					</li>
					<li style="margin-top:10px;">
						<a id="faq3" href="https://www.asus.com/support/FAQ/113661/" target="_blank" style="text-decoration:underline;font-size:14px;font-weight:bolder;color:#FFF"><%tcWebApi_get("String_Entry","LPR_print_share","s")%> FAQ (MAC)</a>&nbsp;&nbsp;
					</li>
				</ul>
				</div>
				<span class="article_seperator">&nbsp;</span>
				</div>
				</td>
				</tr>
				</tbody></table>
				</div>
				</div>
				</div>
				</div>
				</td>
			</tr>
		</table>
	</td>
	<td width="20" align="center" valign="top"></td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--PrinterServer.asp-->
</html>

