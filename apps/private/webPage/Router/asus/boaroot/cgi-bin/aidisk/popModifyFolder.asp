<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<!--aidisk/popModifyFolder.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>Rename Folder</title>
<link rel="stylesheet" href="/form_style.css" type="text/css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript">
var PoolDevice = parent.pool_devices()[parent.getSelectedPoolOrder()];
var PoolName = parent.pool_names()[parent.getSelectedPoolOrder()];
var folderlist = parent.get_sharedfolder_in_pool(PoolDevice);
var selectedFolder = folderlist[parent.getSelectedFolderOrder()];
function initial(){
	showFolder = check_folder_length(selectedFolder);
	showtext($("selected_Pool"), PoolName);
	showtext($("selected_Folder"), showhtmlspace(showhtmland(showFolder)));
	document.modifyFolderForm.new_folder.focus();
	clickevent();
}

function clickevent(){
	if(navigator.userAgent.search("MSIE") == -1)
		document.getElementById('new_folder').addEventListener('keydown',keyDownHandler,false);		
	else
		document.getElementById('new_folder').attachEvent('onkeydown',keyDownHandler);
		
		
	document.getElementById("Submit").onclick = function(){
		if(validForm()){
			document.getElementById("pool").value = PoolDevice;
			document.getElementById("folder").value = selectedFolder;
			parent.showLoading();
			document.modifyFolderForm.submit();
			parent.hidePop("apply");
		}
	};		
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

function validForm(){
	$("new_folder").value = trim($("new_folder").value);
	if($("new_folder").value.length == 0){
		alert("<%tcWebApi_get("String_Entry","File_content_alert_desc6","s")%>");
		$("new_folder").focus();
		return false;
	}
	var re = new RegExp("[^a-zA-Z0-9 _-]+","gi");
	if(re.test($("new_folder").value)){
		alert("<%tcWebApi_get("String_Entry","File_content_alert_desc7","s")%>");
		$("new_folder").focus();
		return false;
	}
	if(parent.checkDuplicateName($("new_folder").value, folderlist)){
		alert("<%tcWebApi_get("String_Entry","File_content_alert_desc8","s")%>");
		$("new_folder").focus();
		return false;
	}
	if(trim($("new_folder").value).length > 12)
		if (!(confirm("<%tcWebApi_get("String_Entry","File_content_alert_desc10","s")%>")))
			return false;
	return true;
}

function NoSubmit(e){
    e = e || window.event;  
    var keynum = e.keyCode || e.which;
    if(keynum === 13){        
        return false;
    }
}

function check_folder_length(folder_name){   //Jieming added, to divide folder name when length of folder name exceed 30
	var n;
	var temp_name = "";
	var start = 0;
	var end = 0;

	if(folder_name.length > 30){
		n = parseInt(folder_name.length/30);
		for(i=0;i<n;i++){
			if(i !=0)
				start = 30*i-1;
				
			end = 30*(i+1)-1;
			temp_name += folder_name.substring(start, end);
			temp_name += "<br>";
		}
		temp_name += folder_name.substring(end, folder_name.length);
		folder_name = temp_name;
	}

	return folder_name;
}

</script>
</head>
<body onLoad="initial();">
<form method="post" name="modifyFolderForm" action="modify_sharedfolder.asp" target="hidden_frame">
<input type="hidden" name="pool" id="pool" value="">
<input type="hidden" name="folder" id="folder" value="">
<table width="100%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<thead>
		<tr>
			<td colspan="2"><span style="color:#FFF"><%tcWebApi_get("String_Entry","ModFolderTitle","s")%></span><img src="/images/button-close.gif" onClick="parent.hidePop('OverlayMask');"></td>
		</tr>
	</thead>
	<tbody>
		<tr>
        <td  colspan="2" height="30"><%tcWebApi_get("String_Entry","ModFolderAlert","s")%></td>
		</tr>
		<tr>
        <th><%tcWebApi_get("String_Entry","PoolName","s")%>: </th>
			<td colspan="3"><span id="selected_Pool"></span></td>
		</tr>
		<tr height="80px">
        <th><%tcWebApi_get("String_Entry","FolderName","s")%>: </th>
			<td colspan="3"><span id="selected_Folder"></span></td>
		</tr>
		<tr>
        <th><%tcWebApi_get("String_Entry","NewFolderName","s")%>: </th>
			<td><input class="input_15_table" type="text" name="new_folder" id="new_folder" onkeypress="return NoSubmit(event)"></td>
		</tr>
		<tr bgcolor="#E6E6E6">
			<th colspan="2" align="right">
			<input id="Submit" type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_modify","s")%>"></th>
		</tr>
	</tbody>
</table>
</form>
</body>

<!--aidisk/popModifyFolder.asp-->
</html>

