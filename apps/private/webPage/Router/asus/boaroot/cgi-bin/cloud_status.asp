<% UI_cloud_status(); %>
<% UI_cloud_dropbox_status(); %>
<% UI_cloud_ftpclient_status(); %>
<% UI_cloud_sambaclient_status(); %>
<% UI_cloud_usbclient_status(); %>
<% UI_rs_status(); %>
enable_cloudsync = '<% tcWebApi_get("AiCloud_Entry", "enable_cloudsync", "s") %>';
cloud_synclist_array = decodeURIComponent('<% tcWebApi_get("AiCloud_Entry", "cloud_sync", "s") %>').replace(/>/g, "&#62").replace(/</g, "&#60");
