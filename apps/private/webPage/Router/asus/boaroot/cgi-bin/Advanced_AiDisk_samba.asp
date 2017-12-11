<%
If Request_Form("Sambaflag")="1" then 	
	tcWebApi_Set("Samba_Entry","st_samba_workgroup","st_samba_workgroup")
	tcWebApi_Set("Samba_Entry","NetBiosName","computer_name")
	tcWebApi_Set("GUITemp_Entry0","action_script", "action_script")
	tcWebApi_Commit("Samba_Entry")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_AiDisk_samba.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_4_2","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/aidisk/AiDisk_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/disk_functions.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script type="text/javascript">
var $j = jQuery.noConflict();

<% disk_pool_mapping_info(); %>
<% available_disk_names_and_sizes(); %>
<% get_AiDisk_status(); %>
<% initial_folder_var_file(); %>
<% get_permissions_of_account(); %>

var PROTOCOL = "cifs";
var NN_status = get_cifs_status(); // Network-Neighborhood
var FTP_status = get_ftp_status(); // FTP
var AM_to_cifs = get_share_management_status("cifs"); // Account Management for Network-Neighborhood
var AM_to_ftp = get_share_management_status("ftp"); // Account Management for FTP
var accounts = [<% get_usb_accounts(); %>];
var lastClickedAccount = 0;
var selectedAccount = null;
var changedPermissions = new Array();
var folderlist = new Array();
function initial(){
	show_menu();
	$("option5").innerHTML = '<table><tbody><tr><td><div id="index_img5"></div></td><td><div style="width:120px;"><%tcWebApi_get("String_Entry","Menu_usb_application","s")%></div></td></tr></tbody></table>';
	$("option5").className = "m5_r";
	document.aidiskForm.protocol.value = PROTOCOL;
	showShareStatusControl(PROTOCOL);
	showAccountControl(PROTOCOL);
	
	// show accounts
	showAccountMenu();
	
	// show the kinds of permission
	showPermissionTitle();

	// show mask
	if(get_manage_type(PROTOCOL)){
		$("loginMethod").innerHTML = "<%tcWebApi_get("String_Entry","AiDisk_SAMBA_hint_2","s")%>";
		$("accountMask").style.display = "none";
	}
	else{
		$("loginMethod").innerHTML = "<%tcWebApi_get("String_Entry","AiDisk_SAMBA_hint_1","s")%>";
		$("accountMask").style.display = "block";
	}
	
	if("<% tcWebApi_get("Ddns_Entry","Active","s"); %>" == 1)
		document.getElementById("machine_name").innerHTML = "<% tcWebApi_get("Ddns_Entry","MYHOST","s"); %>";
	else
		document.getElementById("machine_name").innerHTML = "<%tcWebApi_get("String_Entry","Web_Title2","s")%>";	
		
	setTimeout('get_disk_tree();', 1000);
	onEvent();
	chech_usb();	
}

function chech_usb()
{
	var usb_path1 = '<% tcWebApi_get("USB_Entry", "usb_path1", "s") %>';
	var usb_path2 = '<% tcWebApi_get("USB_Entry", "usb_path2", "s") %>';
	if (usb_path1 != "storage" && usb_path2 != "storage"){
		//$("accountbtn").disabled = true;
		//$("sharebtn").disabled = true;
	}
}

function get_disk_tree(){
	if(this.isLoading == 0){
		get_layer_items("0", "gettree");
		setTimeout('get_disk_tree();', 1000);
	}
	else
		;
}
function get_accounts(){
	return this.accounts;
}
function switchAppStatus(protocol){ // turn on/off the share
	var status;
	var confirm_str_on, confirm_str_off;
	if(protocol == "cifs"){
		status = this.NN_status;
		
		confirm_str_off= "<%tcWebApi_get("String_Entry","confirm_disablecifs","s")%>";
		confirm_str_on = "<%tcWebApi_get("String_Entry","confirm_enablecifs","s")%>";
	}
	else if(protocol == "ftp"){
		status = this.FTP_status;
		
		confirm_str_off = "<%tcWebApi_get("String_Entry","confirm_disableftp","s")%>";
		confirm_str_on = "<%tcWebApi_get("String_Entry","confirm_enableftp","s")%>";
	}
	switch(status){
		case 1:
			if(confirm(confirm_str_off)){
				document.aidiskForm.action = "aidisk/switch_AiDisk_app.asp";
				document.aidiskForm.protocol.value = protocol;
				document.aidiskForm.flag.value = "off";
				showLoading(6);
				setTimeout('location = "'+ location.pathname +'";', 6000);
				document.aidiskForm.submit();
			}
			else{
				refreshpage();
			}
			break;
		case 0:
			if(confirm(confirm_str_on)){
				document.aidiskForm.action = "aidisk/switch_AiDisk_app.asp";
				document.aidiskForm.protocol.value = protocol;
				document.aidiskForm.flag.value = "on";
				showLoading(6);
				setTimeout('location = "'+ location.pathname +'";', 6000);
				document.aidiskForm.submit();
			}
			else{				
				refreshpage();
			}	
			break;
	}
}
function resultOfSwitchAppStatus(){
	refreshpage(1);
}
function showShareStatusControl(protocol){
	var status;
	var str_on, str_off;
	if(protocol == "cifs"){
		status = this.NN_status;
		
		str_on = "<%tcWebApi_get("String_Entry","enableCIFS","s")%>";
		str_off = "<%tcWebApi_get("String_Entry","disableCIFS","s")%>";
	}
	else if(protocol == "ftp"){
		status = this.FTP_status;
		
		str_on = "<%tcWebApi_get("String_Entry","enableFTP","s")%>";
		str_off = "<%tcWebApi_get("String_Entry","disableFTP","s")%>";
	}
	else
		return;
	switch(status){
		case 1:			
			$("tableMask").style.width = "0px";			
			showSamba();
			break;
		case 0:			
			$("tableMask").style.width = "600px";			
			showSamba();
			break;
	}
}
function showSamba(){
/* $("ie_link").href = '\\\\'+decodeURIComponent(document.form.computer_name.value);
$("computer_show1").value = '\\\\'+decodeURIComponent(document.form.computer_name.value);
$("computer_show2").value = '\\\\'+decodeURIComponent(document.form.computer_name.value);
if(NN_status == 1){
$("ShareClose").style.display = "none";
$("Sambainfo").style.display = "block";
if(navigator.appName.indexOf("Microsoft") >= 0)
$("ie_hint").style.display = "block";
else
$("notie_hint").style.display = "block";
}
else{
$("ShareClose").style.display = "block";
$("Sambainfo").style.display = "none";
}*/
}
function switchAccount(protocol){
	var status;
	if(protocol != "cifs" && protocol != "ftp")
		return;
	status = get_manage_type(protocol);
	
	switch(status){
		case 1:
			if(confirm("<%tcWebApi_get("String_Entry","AiDisk_SAMBA_hint_3","s")%>")){
				document.aidiskForm.action = "aidisk/switch_share_mode.asp";
				document.aidiskForm.protocol.value = protocol;
				document.aidiskForm.mode.value = "share";
				showLoading(6);
				setTimeout('location = "'+ location.pathname +'";', 6000);
				document.aidiskForm.submit();
			}
			else{
				refreshpage();
			}
			break;
		case 0:
				document.aidiskForm.action = "aidisk/switch_share_mode.asp";
				document.aidiskForm.protocol.value = protocol;
				document.aidiskForm.mode.value = "account";
				showLoading(6);
				setTimeout('location = "'+ location.pathname +'";', 6000);
				document.aidiskForm.submit();
			break;
	}
}
function resultOfSwitchShareMode(){
	refreshpage();
}
function showAccountMenu(){
	var account_menu_code = "";
	if(this.accounts.length <= 0)
		account_menu_code += '<div class="noAccount" id="noAccount"><%tcWebApi_get("String_Entry","Noaccount","s")%></div>\n'
	else
		for(var i = 0; i < this.accounts.length; ++i){
			account_menu_code += '<div class="accountName" id="';
			account_menu_code += "account"+i;
			account_menu_code += '" onClick="setSelectAccount(this);">'
			account_menu_code += this.accounts[i];
			account_menu_code += '</div>\n';
		}
	$("account_menu").innerHTML = account_menu_code;
	if(this.accounts.length > 0){
		if(get_manage_type(PROTOCOL) == 1)
			setSelectAccount($("account0"));
	}
}
function showAccountControl(protocol){
	var status;
	var str_on, str_off;
	if(protocol != "cifs" && protocol != "ftp")
		return;
	status = get_manage_type(protocol);
	
	str_on = "<%tcWebApi_get("String_Entry","enableAccountManage","s")%>";
	str_off = "<%tcWebApi_get("String_Entry","disableAccountManage","s")%>";
	
	switch(status){
		case 1:
			$("accountMask").style.display = "none";			
			break;
		case 0:
			$("accountMask").style.display = "block";			
			break;
	}
}
function showPermissionTitle(){
	var code = "";
	code += '<table width="190"><tr>';
	if(PROTOCOL == "cifs"){
		code += '<td width="34%" align="center">R/W</td>';
		code += '<td width="28%" align="center">R</td>';
		code += '<td width="38%" align="center"><%tcWebApi_get("String_Entry","checkbox_No","s")%></td>';
	}else if(PROTOCOL == "ftp"){
		code += '<td width="28%" align="center">R/W</td>';
		code += '<td width="22%" align="center">W</td>';
		code += '<td width="22%" align="center">R</td>';
		code += '<td width="28%" align="center"><%tcWebApi_get("String_Entry","checkbox_No","s")%></td>';
	}
	code += '</tr></table>';
	$("permissionTitle").innerHTML = code;
}
var controlApplyBtn = 0;
function showApplyBtn(){
	if(this.controlApplyBtn == 1){
		$("changePermissionBtn").className = "button_gen_long";
		$("changePermissionBtn").disabled = false;
	}else{
		$("changePermissionBtn").className = "button_gen_long_dis";
		$("changePermissionBtn").disabled = true;
	}
}
function setSelectAccount(selectedObj){
	this.selectedAccount = selectedObj.firstChild.nodeValue;
	onEvent();
	show_permissions_of_account(selectedObj, PROTOCOL);
	contrastSelectAccount(selectedObj);
}
function getSelectedAccount(){
	return this.selectedAccount;
}
function show_permissions_of_account(selectedObj, protocol){
	var accountName = selectedObj.firstChild.nodeValue;
	var poolName;
	var permissions;
	for(var i = 0; i < pool_devices().length; ++i){
		poolName = pool_devices()[i];
		if(!this.clickedFolderBarCode[poolName])
			continue;
		permissions = get_account_permissions_in_pool(accountName, poolName);
		for(var j = 1; j < permissions.length; ++j){
			var folderBarCode = get_folderBarCode_in_pool(poolName, permissions[j][0]);
			if(protocol == "cifs")
				showPermissionRadio(folderBarCode, permissions[j][1]);
			else if(protocol == "ftp")
				showPermissionRadio(folderBarCode, permissions[j][2]);
			else{
				alert("Wrong protocol when get permission!"); // system error msg. must not be translate
				return;
			}
		}
	}
}
function get_permission_of_folder(accountName, poolName, folderName, protocol){
	var permissions = get_account_permissions_in_pool(accountName, poolName);
	for(var i = 1; i < permissions.length; ++i)
		if(permissions[i][0] == folderName){
			if(protocol == "cifs")
				return permissions[i][1];
			else if(protocol == "ftp")
				return permissions[i][2];
			else{
				alert("Wrong protocol when get permission!"); // system error msg. must not be translate
				return;
			}
		}
	alert("Wrong folderName when get permission!"); // system error msg. must not be translate
}
function contrastSelectAccount(selectedObj){
	if(this.lastClickedAccount != 0){
		this.lastClickedAccount.style.marginRight = "0px";
		this.lastClickedAccount.style.background = "url(/images/New_ui/advancesetting/user_icon0.png)left no-repeat";
		this.lastClickedAccount.style.cursor = "pointer";
		this.lastClickedAccount.style.fontWeight ="normal";
	}
	selectedObj.style.marginRight = "-1px";
	selectedObj.style.background = "url(/images/New_ui/advancesetting/user_icon.png) left no-repeat";
	selectedObj.style.cursor = "default";
	selectedObj.style.fontWeight ="bolder";
	this.lastClickedAccount = selectedObj;
}
function submitChangePermission(protocol){
	var orig_permission;
	var target_account = null;
	var target_folder = null;
	for(var i = -1; i < accounts.length; ++i){
		if(i == -1)
			target_account = "guest";
		else
			target_account = accounts[i];
		if(!this.changedPermissions[target_account])
			continue;
		for(var j = 0; j < pool_devices().length; ++j){
			if(!this.changedPermissions[target_account][pool_devices()[j]])
				continue;
			folderlist = get_sharedfolder_in_pool(pool_devices()[j]);
			for(var k = 0; k < folderlist.length; ++k){
				target_folder = folderlist[k];
				if(!this.changedPermissions[target_account][pool_devices()[j]][target_folder])
					continue;
				if(target_account == "guest")
					orig_permission = get_permission_of_folder(null, pool_devices()[j], target_folder, PROTOCOL);
				else
					orig_permission = get_permission_of_folder(target_account, pool_devices()[j], target_folder, PROTOCOL);
				if(this.changedPermissions[target_account][pool_devices()[j]][target_folder] == orig_permission)
					continue;
				if(this.changedPermissions[target_account][pool_devices()[j]][target_folder] == -1)
					continue;
				document.aidiskForm.action = "aidisk/set_account_permission.asp";
				if(target_account == "guest")
					$("account").disabled = 1;
				else{
					$("account").disabled = 0;
					$("account").value = target_account;
				}
				document.aidiskForm.pool.value = pool_devices()[j];
				if(target_folder == "")
					$("folder").disabled = 1;
				else{
					$("folder").disabled = 0;
					$("folder").value = target_folder;
				}
				document.aidiskForm.protocol.value = protocol;
				$("permission").value = this.changedPermissions[target_account][pool_devices()[j]][target_folder];
				this.changedPermissions[target_account][pool_devices()[j]][target_folder] = -1;
				/*alert("account = "+$("account").value+"\n"+
				"pool = "+$("pool").value+"\n"+
				"folder = "+$("folder").value+"\n"+
				"protocol = "+$("protocol").value+"\n"+
				"permission = "+$("permission").value);//*/
				// showLoading(2);
				// setTimeout('location = "'+ location.pathname +'";', 2000);
				showLoading();
				document.aidiskForm.submit();
				return;
			}
		}
	}
	refreshpage();
}
function changeActionButton(selectedObj, type, action, flag){
	if(type == "User")
		if(this.accounts.length <= 0)
			if(action == "Del" || action == "Mod")
				return;
	if(typeof(flag) == "number")
		selectedObj.src = '/images/New_ui/advancesetting/'+type+action+'_'+flag+'.png';
	else
		selectedObj.src = '/images/New_ui/advancesetting/'+type+action+'.png';
}
function resultOfCreateAccount(){
	refreshpage();
}
function onEvent(){
	var adminAccount = "<%tcWebApi_get("Account_Entry0","username","s")%>";
	if(get_manage_type(PROTOCOL) == 1 && accounts.length < 6){
		changeActionButton($("createAccountBtn"), 'User', 'Add', 0);
		$("createAccountBtn").onclick = function(){
			popupWindow('OverlayMask','aidisk/popCreateAccount.asp');
		};
		$("createAccountBtn").onmouseover = function(){
			changeActionButton(this, 'User', 'Add', 1);
		};
		$("createAccountBtn").onmouseout = function(){
			changeActionButton(this, 'User', 'Add', 0);
		};
	}
	else{
		changeActionButton($("createAccountBtn"), 'User', 'Add');
		$("createAccountBtn").onclick = function(){};
		$("createAccountBtn").onmouseover = function(){};
		$("createAccountBtn").onmouseout = function(){};
		$("createAccountBtn").title = (accounts.length < 6)?"<%tcWebApi_get("String_Entry","AddAccountTitle","s")%>":"<%tcWebApi_get("String_Entry","account_overflow","s")%>";
	}
	if(this.accounts.length > 0 && this.selectedAccount != null && this.selectedAccount.length > 0 && this.selectedAccount != adminAccount){
		changeActionButton($("modifyAccountBtn"), 'User', 'Mod', 0);
		$("modifyAccountBtn").onclick = function(){
			if(!selectedAccount){
				alert("No chosen account!");
				return;
			}
			popupWindow('OverlayMask','aidisk/popModifyAccount.asp');
		};
		$("modifyAccountBtn").onmouseover = function(){
			changeActionButton(this, 'User', 'Mod', 1);
		};
		$("modifyAccountBtn").onmouseout = function(){
			changeActionButton(this, 'User', 'Mod', 0);
		};
	}
	else{
		changeActionButton($("modifyAccountBtn"), 'User', 'Mod');
		$("modifyAccountBtn").onclick = function(){};
		$("modifyAccountBtn").onmouseover = function(){};
		$("modifyAccountBtn").onmouseout = function(){};
	}
	if(this.accounts.length > 1 && this.selectedAccount != null && this.selectedAccount.length > 0 && this.selectedAccount != adminAccount){
		changeActionButton($("deleteAccountBtn"), 'User', 'Del', 0);
		$("deleteAccountBtn").onclick = function(){
			if(!selectedAccount){
			alert("No chosen account!");
			return;
			}
			popupWindow('OverlayMask','aidisk/popDeleteAccount.asp');
		};
		$("deleteAccountBtn").onmouseover = function(){
			changeActionButton(this, 'User', 'Del', 1);
		};
		$("deleteAccountBtn").onmouseout = function(){
			changeActionButton(this, 'User', 'Del', 0);
		};
	}
	else{
		changeActionButton($("deleteAccountBtn"), 'User', 'Del');
		$("deleteAccountBtn").onclick = function(){};
		$("deleteAccountBtn").onmouseover = function(){};
		$("deleteAccountBtn").onmouseout = function(){};
	}
	if(this.selectedPoolOrder >= 0 && this.selectedFolderOrder < 0){
		changeActionButton($("createFolderBtn"), 'Folder', 'Add', 0);
		$("createFolderBtn").onclick = function(){
			if(selectedDiskOrder < 0){
				alert("No chosen Disk for creating the shared-folder!");
				return;
			}
			if(selectedPoolOrder < 0){
				alert("No chosen Partition for creating the shared-folder!");
				return;
			}
			popupWindow('OverlayMask','aidisk/popCreateFolder.asp');
		};
		$("createFolderBtn").onmouseover = function(){
			changeActionButton(this, 'Folder', 'Add', 1);
		};
		$("createFolderBtn").onmouseout = function(){
			changeActionButton(this, 'Folder', 'Add', 0);
		};
	}
	else{
		changeActionButton($("createFolderBtn"), 'Folder', 'Add');
		$("createFolderBtn").onclick = function(){};
		$("createFolderBtn").onmouseover = function(){};
		$("createFolderBtn").onmouseout = function(){};
	}
	if(this.selectedFolderOrder >= 0){
		changeActionButton($("deleteFolderBtn"), 'Folder', 'Del', 0);
		changeActionButton($("modifyFolderBtn"), 'Folder', 'Mod', 0);
		$("deleteFolderBtn").onclick = function(){
			if(selectedFolderOrder < 0){
				alert("No chosen folder!");
				return;
			}
			popupWindow('OverlayMask','aidisk/popDeleteFolder.asp');
		};
		$("deleteFolderBtn").onmouseover = function(){
			changeActionButton(this, 'Folder', 'Del', 1);
		};
		$("deleteFolderBtn").onmouseout = function(){
			changeActionButton(this, 'Folder', 'Del', 0);
		};
		$("modifyFolderBtn").onclick = function(){
			if(selectedFolderOrder < 0){
				alert("No chosen folder!");
				return;
			}
			popupWindow('OverlayMask','aidisk/popModifyFolder.asp');
		};
		$("modifyFolderBtn").onmouseover = function(){
			changeActionButton(this, 'Folder', 'Mod', 1);
		};
		$("modifyFolderBtn").onmouseout = function(){
			changeActionButton(this, 'Folder', 'Mod', 0);
		};
	}
	else{
		changeActionButton($("deleteFolderBtn"), 'Folder', 'Del');
		changeActionButton($("modifyFolderBtn"), 'Folder', 'Mod');
		$("deleteFolderBtn").onclick = function(){};
		$("deleteFolderBtn").onmouseover = function(){};
		$("deleteFolderBtn").onmouseout = function(){};
		$("modifyFolderBtn").onclick = function(){};
		$("modifyFolderBtn").onmouseover = function(){};
		$("modifyFolderBtn").onmouseout = function(){};
	}
	$("changePermissionBtn").onclick = function(){
		submitChangePermission(PROTOCOL);
	};
}
function unload_body(){
	$("createAccountBtn").onclick = function(){};
	$("createAccountBtn").onmouseover = function(){};
	$("createAccountBtn").onmouseout = function(){};
	$("deleteAccountBtn").onclick = function(){};
	$("deleteAccountBtn").onmouseover = function(){};
	$("deleteAccountBtn").onmouseout = function(){};
	$("modifyAccountBtn").onclick = function(){};
	$("modifyAccountBtn").onmouseover = function(){};
	$("modifyAccountBtn").onmouseout = function(){};
	$("createFolderBtn").onclick = function(){};
	$("createFolderBtn").onmouseover = function(){};
	$("createFolderBtn").onmouseout = function(){};
	$("deleteFolderBtn").onclick = function(){};
	$("deleteFolderBtn").onmouseover = function(){};
	$("deleteFolderBtn").onmouseout = function(){};
	$("modifyFolderBtn").onclick = function(){};
	$("modifyFolderBtn").onmouseover = function(){};
	$("modifyFolderBtn").onmouseout = function(){};
}

function applyRule(){
    if(validForm()){
    		document.form.Sambaflag.value = "1";
        showLoading(3);
				setTimeout('location = "'+ location.pathname +'";', 3000);
				document.form.submit();
     }
}

function validForm(){
	
	if(document.form.computer_name.value.length == 0){
		showtext($("alert_msg1"), "<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
		document.form.computer_name.focus();
		document.form.computer_name.select();
		return false;
	}
	else{
		
		var alert_str = validate_hostname(document.form.computer_name);
		if(alert_str != ""){
			showtext($("alert_msg1"), alert_str);
			$("alert_msg1").style.display = "";
			document.form.computer_name.focus();
			document.form.computer_name.select();
			return false;
		}else{
			$("alert_msg1").style.display = "none";
  	}

		document.form.computer_name.value = trim(document.form.computer_name.value);
	}
			
	if(document.form.st_samba_workgroup.value.length == 0){
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
		document.form.st_samba_workgroup.focus();
		document.form.st_samba_workgroup.select();
		return false;	
	}
  else{
		var workgroup_check = new RegExp('^[a-zA-Z0-9][a-zA-Z0-9\-\_\.]+$','gi');	  	
  	if(!workgroup_check.test(document.form.st_samba_workgroup.value)){
			alert("<%tcWebApi_get("String_Entry","JS_validchar","s")%>");               
			document.form.st_samba_workgroup.focus();
			document.form.st_samba_workgroup.select();
			return false;
		}   
	}

  return true;
}
</script>
</head>
<body onLoad="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>
<form method="post" name="aidiskForm" action="#" target="hidden_frame">
<input type="hidden" name="motion" id="motion" value="">
<input type="hidden" name="layer_order" id="layer_order" value="">
<input type="hidden" name="protocol" id="protocol" value="">
<input type="hidden" name="mode" id="mode" value="">
<input type="hidden" name="flag" id="flag" value="">
<input type="hidden" name="account" id="account" value="">
<input type="hidden" name="pool" id="pool" value="">
<input type="hidden" name="folder" id="folder" value="">
<input type="hidden" name="permission" id="permission" value="">
</form>
<form method="post" name="form" action="Advanced_AiDisk_samba.asp" target="hidden_frame">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="current_page" value="Advanced_AiDisk_samba.asp">
<input type="hidden" name="next_page" value="Advanced_AiDisk_samba.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_ftpsamba">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="Sambaflag" value="0">

<table width="983" border="0" align="center" cellpadding="0" cellspacing="0" class="content">
<tr>
	<td width="17">&nbsp;</td>
	<td valign="top" width="202">
		<div id="mainMenu"></div>
		<div id="subMenu"></div>
	</td>
	<td valign="top">
		<div id="tabMenu" class="submenuBlock"></div>
		<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
		<tr>
			<td valign="top">
				<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
				<tbody>
				<tr>
				<td bgcolor="#4D595D">
					<div>&nbsp;</div>
					<div style="width:730px">
						<table width="730px">
						<tr>
							<td align="left">
							<span class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_4","s")%> - <%tcWebApi_get("String_Entry","menu5_4_1","s")%></span>
							</td>
							<td align="right">
								<img onclick="go_setting('APP_Installation.asp')" align="right" style="cursor:pointer;position:absolute;margin-left:-20px;margin-top:-30px;" title="Back to USB Extension" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'">
							</td>
						</tr>
						</table>
					</div>
					<div style="margin:5px;"><img src="/images/New_ui/export/line_export.png"></div>
					<div class="formfontdesc"><%tcWebApi_get("String_Entry","Samba_desc","s")%></div>
					<!-- a href="javascript:switchAppStatus(PROTOCOL);"><div class="titlebtn" align="center"><span id="sharebtn" style="*width:196px;"></span></div></a>
					<a href="javascript:switchAccount(PROTOCOL);"><div class="titlebtn" align="center"><span id="accountbtn" style="*width:266px;"></span></div></a>
					<a href="javascript:refreshpage();"><div class="titlebtn" align="center"><span id="refreshbtn" style="*width:136px;"><%tcWebApi_get("String_Entry","DrSurf_refresh_page","s")%></span></div></a-->
					
				<table width="740px" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
				<tr>
				<th><%tcWebApi_get("String_Entry","enableCIFS","s")%></th>
					<td>
						<div class="left" style="width:94px; float:left; cursor:pointer;" id="radio_samba_enable"></div>
						<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
							<script type="text/javascript">
								$j('#radio_samba_enable').iphoneSwitch(NN_status, 
									 function() {
									 	switchAppStatus(PROTOCOL);
									 },
									 function() {
									 	switchAppStatus(PROTOCOL);
									 },
									 {
										switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
									 }
								);
							</script>			
						</div>	
					</td>
				</tr>										

				<tr style="height:60px;">
				<th><%tcWebApi_get("String_Entry","AiDisk_Guest_Login","s")%></th>
					<td>
						<div class="left" style="margin-top:5px;width:94px;float:left; cursor:pointer;" id="radio_anonymous_enable"></div>
						<div class="iphone_switch_container" style="display:table-cell;vertical-align:middle;height:45px;position:relative;overflow:hidden">
							<script type="text/javascript">
								$j('#radio_anonymous_enable').iphoneSwitch(!get_manage_type(PROTOCOL), 
									 function() {
									 	switchAccount(PROTOCOL);
									 },
									 function() {
									 	switchAccount(PROTOCOL);
									 },
									 {
										switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
									 }
								);
							</script>			
							<span id="loginMethod" style="color:#FC0"></span>
						</div>
					</td>
				</tr>
				<tr>
					<th>
						<a class="hintstyle" href="javascript:void(0);" onClick="openHint(17,2);"><%tcWebApi_get("String_Entry","ShareNode_DeviceName_in","s")%></a>
					</th>
					<td>
						<input type="text" name="computer_name" id="computer_name" class="input_15_table" maxlength="20" value="<% tcWebApi_Get("Samba_Entry", "NetBiosName", "s") %>">
						<br/><span id="alert_msg1" style="color:#FC0;"></span>
					</td>
				</tr>
				<tr>
					<th>
							<a class="hintstyle" href="javascript:void(0);" onClick="openHint(17,3);"><%tcWebApi_get("String_Entry","ShareNode_WorkGroup_in","s")%></a>
					</th>
					<td>
							<input type="text" name="st_samba_workgroup" class="input_32_table" maxlength="32" value="<% tcWebApi_Get("Samba_Entry", "st_samba_workgroup", "s") %>">
					</td>
				</tr>
			</table>
			<div class="apply_gen">
					<input type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>" onclick="applyRule();">
			</div>	
</form>							
					<br/><br/>
					<div id="shareStatus">
						<div id="tableMask"></div>
						<div id="accountMask"></div>
						<table width="740px" height="35" cellpadding="2" cellspacing="0" class="accountBar">
						<tr>
							<td width="25%" style="border: 1px solid #222;">
								<img id="createAccountBtn" src="/images/New_ui/advancesetting/UserAdd.png" hspace="1" title="<%tcWebApi_get("String_Entry","AddAccountTitle","s")%>">
								<img id="deleteAccountBtn" src="/images/New_ui/advancesetting/UserDel.png" hspace="1" title="<%tcWebApi_get("String_Entry","DelAccountTitle","s")%>">
								<img id="modifyAccountBtn" src="/images/New_ui/advancesetting/UserMod.png" hspace="1" title="<%tcWebApi_get("String_Entry","ModAccountTitle","s")%>">
							</td>
							<td>
					<img id="createFolderBtn" hspace="1" title="<%tcWebApi_get("String_Entry","AddFolderTitle","s")%>">
					<img id="deleteFolderBtn" hspace="1" title="<%tcWebApi_get("String_Entry","DelFolderTitle","s")%>">
					<img id="modifyFolderBtn" hspace="1" title="<%tcWebApi_get("String_Entry","ModFolderTitle","s")%>">						
							</td>
						</tr>
						</table>
					</div>
					<!-- <table width="99%" height="400" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#999999">-->
					<table width="740px" height="200" align="center" border="1" cellpadding="4" cellspacing="0" class="AiDiskTable">
					<tr>
						<!-- <td valign="top" bgcolor="#F8F8F8">-->
						<th align="left" valign="top">
							<div id="account_menu"></div>
						</th>
						<td valign="top">
							<table width="480" border="0" cellspacing="0" cellpadding="0" class="FileStatusTitle">
							<tr>
								<td width="290" height="20" align="left">
									<div id="machine_name" class="machineName"><%tcWebApi_get("String_Entry","Web_Title2","s")%></div>
								</td>
								<td>
									<div id="permissionTitle"></div>
								</td>
							</tr>
							</table>
							<div id="e0" class="FdTemp" style="font-size:10pt; margin-top:2px;"></div>
							<div style="text-align:center; margin:10px auto; border-top:1px dotted #CCC; width:95%; padding:2px;">
			    <input name="changePermissionBtn" id="changePermissionBtn" type="button" value="<%tcWebApi_get("String_Entry","CTL_save_permission","s")%>" class="button_gen_long" disabled="disabled">
							</div>
						</td>
					</tr>
					</table>
				</td>
				</tr>
				</tbody>
				</table>
			</td>
			<td width="10"></td>
		</tr>
		</table>
	</td>
	<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
<div id="OverlayMask" class="popup_bg">
<div align="center">
<iframe src="" frameborder="0" scrolling="no" id="popupframe" width="400" height="400" allowtransparency="true" style="margin-top:150px;"></iframe>
</div>
</div>
</body>

<!--Advanced_AiDisk_samba.asp-->
</html>

