<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--device-map/disk.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title></title>
<link href="/form_style.css" rel="stylesheet" type="text/css" />
<link href="/NM_style.css" rel="stylesheet" type="text/css" />
<style>
a:link {
	text-decoration: underline;
	color: #FFFFFF;
}
a:visited {
	text-decoration: underline;
	color: #FFFFFF;
}
a:hover {
	text-decoration: underline;
	color: #FFFFFF;
}
a:active {
	text-decoration: none;
	color: #FFFFFF;
}
</style>
<script type="text/javascript" src="/state.js"></script>
<script>
var diskOrder = parent.getSelectedDiskOrder();
var _DMDiskNum = (pool_devices().getIndexByValue('<% tcWebApi_get("Apps_Entry", "apps_dev", "s") %>') < foreign_disk_total_mounted_number()[0])? 0 : 1;
var all_accessable_size = parent.simpleNum2(parent.computeallpools(diskOrder, "size")-parent.computeallpools(diskOrder, "size_in_use"));
var all_total_size = parent.simpleNum2(parent.computeallpools(diskOrder, "size"));
var mountedNum = parent.getDiskMountedNum(diskOrder);

<% get_AiDisk_status(); %>

var apps_array = <% apps_info("asus") %>;
var apps_dev = "<% tcWebApi_get("Apps_Entry", "apps_dev", "s") %>";

function initial(){	
	showtext($("disk_model_name"), parent.getDiskModelName(diskOrder));
	showtext($("disk_total_size"), parent.getDiskTotalSize(diskOrder));

	if(!parent.media_support)
		$("mediaserver_hyperlink").style.display = "none";
	if(mountedNum > 0){
		showtext($("disk_total_size"), all_total_size+" GB");
		showtext($("disk_avail_size"), all_accessable_size+" GB");		
	}
	else{
		$("mounted_item1").style.display = "none";
		$("unmounted_refresh").style.display = "";
	}
	if(sw_mode == "2" || sw_mode == "3")
		$("aidisk_hyperlink").style.display = "none";

	for(var i = 0; i < apps_array.length; i++){
		if(apps_array[i][0] == "downloadmaster" && apps_array[i][4] == "yes" && apps_array[i][3] == "yes"){
			if(_DMDiskNum == diskOrder || foreign_disks().length == 1) {
				$("dmLink").style.display = "";
				break;
			}
		}
	}
}

function goUPnP(){
	parent.location.href = "/cgi-bin/mediaserver.asp";
}
function gotoAidisk(){
	parent.location.href = "/cgi-bin/aidisk.asp";
}

function gotoDM(){
	var dm_http_port = '<%TcWebApi_get("Apps_Entry","dm_http_port","s")%>';
	if(dm_http_port == "")
		dm_http_port = "8081";

	var dm_url = "";
	if(parent.location.host.split(":").length > 1)
		dm_url = "http://" + parent.location.host.split(":")[0] + ":" + dm_http_port;
	else
		dm_url = "http://" + parent.location.host + ":" + dm_http_port;

	window.open(dm_url);
}

function remove_disk(){
	var str = "<%tcWebApi_get("String_Entry","Safelyremovedisk_confirm","s")%>";
	if(confirm(str)){
		parent.showLoading();
		document.diskForm.action = "safely_remove_disk.asp";
		document.diskForm.disk.value = parent.getDiskPort(this.diskOrder);
		setTimeout("document.diskForm.submit();", 1);
	}
}
</script>
</head>
<body class="statusbody" onload="initial();">
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
	<tr>
	<td style="padding:5px 10px 0px 15px;">
    	<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","Modelname","s")%>:</p>
		<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px; color:#FFFFFF;" id="disk_model_name"></p>
		<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
	</td>
	</tr>
</table>
<table id="mounted_item1" width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
	<tr>
		<td style="padding:5px 10px 0px 15px;">
    	<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","Totalspace","s")%>:</p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px; color:#FFFFFF;" id="disk_total_size"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr>
		<td style="padding:5px 10px 0px 15px;">
    	<p class="formfonttitle_nwm"><%tcWebApi_get("String_Entry","Availablespace","s")%>:</p>
			<p style="padding-left:10px; margin-top:3px; background-color:#444f53; line-height:20px; color:#FFFFFF;" id="disk_avail_size"></p>
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr id="mediaserver_hyperlink">
		<td style="padding:10px 15px 0px 15px;;">
    	<p class="formfonttitle_nwm" style="float:left;width:138px;"><%tcWebApi_get("String_Entry","UPnPMediaServer","s")%>:</p>
      <input type="button" class="button_gen" onclick="goUPnP();" value="<%tcWebApi_get("String_Entry","btn_go","s")%>" >
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr id="aidisk_hyperlink">
		<td height="50" style="padding:10px 15px 0px 15px;">
    	<p class="formfonttitle_nwm" style="float:left;width:138px;"><%tcWebApi_get("String_Entry","AiDiskWizard","s")%>:</p>
    	<input type="button" class="button_gen" onclick="gotoAidisk();" value="<%tcWebApi_get("String_Entry","btn_go","s")%>" >
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
	<tr id="dmLink" style="display:none;">
		<td height="50" style="padding:10px 15px 0px 15px;">
			<p class="formfonttitle_nwm" style="float:left;width:138px;">Download Master</p>
    	<input type="button" class="button_gen" onclick="gotoDM();" value="<%tcWebApi_get("String_Entry","btn_go","s")%>" >
			<img style="margin-top:5px;" src="/images/New_ui/networkmap/linetwo2.png">
		</td>
	</tr>
</table>
<table width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="table1px">
	<tr>
	<td height="50" style="padding:10px 15px 0px 15px;">
    	<p class="formfonttitle_nwm" style="float:left;width:138px; "><%tcWebApi_get("String_Entry","Safelyremovedisk_title","s")%>:</p>
    	<input id="show_remove_button" class="button_gen" type="button" class="button" onclick="remove_disk();" value="<%tcWebApi_get("String_Entry","btn_remove","s")%>">
    	<div id="show_removed_string" style="display:none;"><%tcWebApi_get("String_Entry","Safelyremovedisk","s")%></div>
	</td>
	</tr>
</table>
<div id="unmounted_refresh" style="padding:5px 0px 5px 25px; display:none">
	<ul style="font-size:11px; font-family:Arial; padding:0px; margin:0px; list-style:outside; line-height:150%;">
		<li><%tcWebApi_get("String_Entry","DiskStatus_refresh1","s")%><a href="/cgi-bin/index2.asp" target="_parent"><%tcWebApi_get("String_Entry","DiskStatus_refresh2","s")%></a><%tcWebApi_get("String_Entry","DiskStatus_refresh3","s")%></li>
	</ul>
</div>

<form method="post" name="diskForm" action="#">
<input type="hidden" name="disk" value="">
</form>
</body>

<!--device-map/disk.asp-->
</html>

