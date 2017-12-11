<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<!--aidisk/popDeleteFolder.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>Del Folder</title>
<link rel="stylesheet" href="/form_style.css" type="text/css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript">
var PoolDevice = parent.pool_devices()[parent.getSelectedPoolOrder()];
var selectedFolder = parent.get_sharedfolder_in_pool(PoolDevice)[parent.getSelectedFolderOrder()];
function initial(){
	showFolder = check_folder_length(selectedFolder);
	showtext(document.getElementById("selected_Folder"), showhtmlspace(showhtmland(showFolder)));
	document.deleteFolderForm.Cancel.focus();
	clickevent();
}

function clickevent(){
	if(navigator.userAgent.search("MSIE") == -1){		
		window.addEventListener('keydown',keyDownHandler,false);
	}

	document.getElementById("Submit").onclick = function(){				
		document.getElementById("pool").value = PoolDevice;
		document.getElementById("folder").value = selectedFolder;
		parent.showLoading();
		document.deleteFolderForm.submit();
		parent.hidePop("apply");
	};	
}


function check_folder_length(folder_name){   //Jieming added, to divide folder name when length of folder name exceed 44
	var n;
	var temp_name = "";
	var start = 0;
	var end = 0;

	if(folder_name.length > 44){
		n = parseInt(folder_name.length/44);
		for(i=0;i<n;i++){
			if(i !=0)
				start = 44*i-1;
				
			end = 44*(i+1)-1;
			temp_name += folder_name.substring(start, end);
			temp_name += "<br>";
		}
		temp_name += folder_name.substring(end, folder_name.length);
		folder_name = temp_name;
	}

	return folder_name;
}

function keyDownHandler(event){
	var keyPressed = event.keyCode ? event.keyCode : event.which;

	if(keyPressed == 13){   // Enter key
		submit();
	}	
	else if(keyPressed == 27){  // Escape key
		parent.hidePop("apply");
	}	
}
</script>
</head>
<body onLoad="initial();" onKeyPress="keyDownHandler(event);">
<form method="post" name="deleteFolderForm" action="delete_sharedfolder.asp" target="hidden_frame">
<input type="hidden" name="pool" id="pool" value="">
<input type="hidden" name="folder" id="folder" value="">
<table width="100%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<thead>
		<tr>
			<td><span style="color:#FFF"><%tcWebApi_get("String_Entry","DelFolderTitle","s")%></span><img src="/images/button-close.gif" onClick="parent.hidePop('OverlayMask');"></td>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td height="70" valign="middle"><%tcWebApi_get("String_Entry","DelFolderAlert","s")%> <span id="selected_Folder" style="color:#333333; "></span> ?</td>
		</tr>
		<tr>
			<td height="30" align="right" bgcolor="#E6E6E6">
	  <input name="Submit" id="Submit" type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_del","s")%>">
	  <input name="Cancel" id="Cancel" type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>" onClick="parent.hidePop('OverlayMask');"></td>
		</tr>
	</tbody>
</table>
</form>
</body>

<!--aidisk/popDeleteFolder.asp-->
</html>

