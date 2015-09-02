<%
If Request_Form("current_page") = "cloud_main.asp" Then
	TCWebApi_set("AiCloud_Entry", "enable_webdav", "enable_webdav")
	TCWebApi_set("AiCloud_Entry", "enable_cloudsync", "enable_cloudsync")
	TCWebApi_set("AiCloud_Entry", "webdav_aidisk", "webdav_aidisk")
	TCWebApi_set("AiCloud_Entry", "webdav_proxy", "webdav_proxy")
	TCWebApi_set("AiCloud_Entry", "webdav_proxy", "webdav_proxy")
	tcWebApi_set("GUITemp_Entry0", "action_script", "action_script")
  	tcWebApi_commit("AiCloud_Entry")
elseif Request_Form("current_page") = "cloud_settings.asp" then
	TCWebApi_set("AiCloud_Entry", "enable_webdav_lock", "enable_webdav_lock")
	TCWebApi_set("AiCloud_Entry", "webdav_acc_lock", "webdav_acc_lock")
	TCWebApi_set("AiCloud_Entry", "webdav_lock_times", "webdav_lock_times")
	TCWebApi_set("AiCloud_Entry", "webdav_lock_interval", "webdav_lock_interval")
	TCWebApi_set("AiCloud_Entry", "webdav_https_port", "webdav_https_port")
	TCWebApi_set("AiCloud_Entry", "webdav_http_port", "webdav_http_port")
	tcWebApi_set("GUITemp_Entry0", "action_script", "action_script")
	tcWebApi_commit("AiCloud_Entry")
elseif Request_Form("current_page") = "cloud_sync.asp" then
	tcWebApi_set("AiCloud_Entry", "cloud_sync", "cloud_sync")
	tcWebApi_set("AiCloud_Entry", "enable_cloudsync", "enable_cloudsync")
	tcWebApi_set("GUITemp_Entry0", "action_script", "action_script")
	tcWebApi_commit("AiCloud_Entry")
elseif Request_Form("current_page") = "cloud_router_sync.asp" then
	tcWebApi_set("AiCloud_Entry", "share_link_host", "share_link_host")
	tcWebApi_set("GUITemp_Entry0", "action_script", "action_script")
	tcWebApi_commit("AiCloud_Entry")
End If
%>
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
</head>
<script type="text/javascript">
var current_page = "<% get_parameter("current_page") %>";
var next_page = "<% get_parameter("next_page") %>";
var action_wait = parseInt("<% get_parameter("action_wait") %>");
function redirect(){
	if(next_page != "")
		parent.location.href = next_page;
}
function initial() {
	if(action_wait!=NaN) {
		parent.showLoading(action_wait);
		setTimeout("redirect();", action_wait*1000);
	}
}
</script>
<body onload="initial();">
</body>
</html>
