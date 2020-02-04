<%
	if request_Form("live_update_flag") = "1" then
		update_variables()
	elseif request_Form("live_upgrade_flag") = "1" then
		update_variables()
	elseif request_Form("sig_update_flag") = "1" then
		update_variables()
	end if	
	
	if request_Form("live_do_upgrade_flag") = "1" then
		stop_service()
		tcWebApi_set("System_Entry","upgrade_fw_status","value_NONE")
		tcWebApi_set("System_Entry","upgrade_fw","DOWNLOAD_HEADER_TYPE")
		tcWebApi_CommitWithoutSave("System_Entry")
	end if		
%>

<html>
<!--live_update.asp-->
<head>
<title></title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<script type="text/javascript">	
	var action_script = "<% get_parameter("action_script"); %>";
	
	function initial(){		
		if(action_script.indexOf("frs_update") >= 0) {				
				setTimeout("parent.detect_firmware();", 5000);
		}
		if(action_script.indexOf("start_webs_upgrade") >= 0) {
  				setTimeout("parent.DownloadDone();", 3000);
  				setTimeout("parent.chk_upgrade();", 5000);
		}
	}	
</script>
</head>
<body onload="initial();">
<form method="post" name="redirectForm" action="/cgi-bin/live_update.asp" target="hidden_frame">
<input type="hidden" name="flag" value="">
<input type="hidden" name="prev_page" value="">
</form>




</body>
<!--live_update.asp-->
</html>


