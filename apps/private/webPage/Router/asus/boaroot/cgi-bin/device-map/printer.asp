<% 
if Request_Form("action_mode") <> "" then
	do_apply_cgi()
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>Untitled Document</title>
<link href="/NM_style.css" rel="stylesheet" type="text/css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/state.js"></script>
<script>
var printer_manufacturer_array = parent.printer_manufacturers();
var printer_model_array = parent.printer_models(); 
var printer_pool_array = parent.printer_pool();
var printer_order = parent.get_clicked_device_order();

<% login_state_hook(); %>

function initial(){
	if(printer_model_array.length > 0 ) {
		showtext($("printerModel"), printer_manufacturer_array[printer_order]+" "+printer_model_array[printer_order]);
		
		if(printer_pool_array[printer_order] != ""){
			showtext(document.getElementById("printerStatus"), '<%tcWebApi_get("String_Entry","CTL_Enabled","s")%>');
			document.getElementById("printer_button").style.display = "";
			document.getElementById("button_descrition").style.display = "";
		}
		else{
			showtext(document.getElementById("printerStatus"), '<%tcWebApi_get("String_Entry","CTL_Disabled","s")%>');
			document.getElementById("printer_button").style.display = "none";
			document.getElementById("button_descrition").style.display = "none";
		}
	}
	else
		showtext(document.getElementById("printerStatus"), '<% translate_x("System_Internet_Details_Item5_desc2"); %>');

	if('<% tcWebApi_get("USB_Entry", "mfp_ip_monopoly", "s") %>' != "" &&
		"<% tcWebApi_get("USB_Entry", "mfp_ip_monopoly", "s") %>" != "N/A" &&
		'<% tcWebApi_get("USB_Entry", "mfp_ip_monopoly", "s") %>' != login_ip_str()){
		document.getElementById("monoBtn").style.display = "none";
		document.getElementById("monoDesc").style.display = "";
		document.getElementById("monoP").style.width = "";
		document.getElementById("monoP").style.float = "";
	}
	else{
		document.getElementById("monoBtn").style.display = "";
		document.getElementById("monoDesc").style.display = "none";
	}

}

function cleanTask(){
	parent.showLoading(2);
	FormActions("/cgi-bin/device-map/printer.asp", "mfp_monopolize", "", "");
	document.form.submit();
}
</script>
</head>

<body class="statusbody" onload="initial();">

<form method="post" name="form" action="/cgi-bin/start_apply.asp">
<input type="hidden" name="productid" value="<% tcWebApi_get("SysInfo_Entry", "ProductName", "s") %>">
<input type="hidden" name="current_page" value="/cgi-bin/device-map/printer.asp">
<input type="hidden" name="next_page" value="/cgi-bin/device-map/printer.asp">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="2">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>">

<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","PS_x_PrinterModel_in","s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="printerModel"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>  

	<tr>
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","Printing_status","s")%></p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;" id="printerStatus"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>  

	<tr id="printer_button" style="display:none;">
		<td style="padding:5px 10px 5px 15px;">
			<p class="formfonttitle_nwm" id="monoP" style="width:138px;"><%tcWebApi_get("String_Entry","Printing_button_item","s")%></p>
			<input id="monoBtn" type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%>" onclick="cleanTask();">
			<p id="monoDesc" style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px;"><% tcWebApi_get("USB_Entry", "mfp_ip_monopoly", "s") %></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>  
</table>

<div id="button_descrition" style="display:none;padding:5px 0px 5px 25px;">
<ul style="font-size:11px; font-family:Arial; color:#FFF; padding:0px; margin:0px; list-style:outside; line-height:150%;">
	<li><%tcWebApi_get("String_Entry","PS_x_Monopoly_id","s")%></li>
	<li>
		<a id="faq" href="https://www.asus.com/support/FAQ/113988/" target="_blank" style="text-decoration:underline;"><%tcWebApi_get("String_Entry","Printing_button_item","s")%> FAQ</a>
	</li>
</ul>
</div>
</form>
</body>
</html>
