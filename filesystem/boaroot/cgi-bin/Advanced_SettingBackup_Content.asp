<%
	If Request_Form("rebootFlag") = "1"  Then
		TCWebApi_set("System_Entry","reboot_type","restoreFlag")
		tcWebApi_CommitWithoutSave("System_Entry")
	End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_SettingBackup_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_6_4","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script>
var $j = jQuery.noConflict();
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var varload = 0;
var lan_ipaddr = '<%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%>';

function initial(){
	show_menu();
}

function redirect(){
	document.location.href = "/cgi-bin/index.asp";
}

function redirect_self(){
	document.location.href = "/cgi-bin/Advanced_SettingBackup_Content.asp";
}

function uploadSetting()
{
	var string3 = document.getElementById("tools_FW_UploadFile").value.search(/Settings_<%ej_get_productid()%>/);

	if (document.getElementById("tools_FW_UploadFile").value=="") {
		alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
		document.getElementById("tools_FW_UploadFile").focus();
	}
	else {
		if (string3 >= 0) {
			showtext(document.getElementById("loading_block2"), "<%tcWebApi_get("String_Entry","SET_ok_desc","s")%>");
			document.getElementById('loading_block3').style.display = "none";
			showLoadingBar(120);
			document.uiPostUpdateForm.postflag.value = "1";
			document.uiPostUpdateForm.submit();
		}
		else{
			alert("<%tcWebApi_get("String_Entry","Setting_upload_hint","s")%>");
			document.getElementById("tools_FW_UploadFile").focus();
		}	
	}
}

function backup_settings()
{
	var cfg = '/Settings_' + odm_productid + '.cfg';
	var code = 'location.assign("' + cfg + '")';
	eval(code);
}

function restoreRule()
{
	var alert_string = "<%tcWebApi_get("String_Entry","Setting_factorydefault_hint1","s")%>";
	alert_string += "<%tcWebApi_get("String_Entry","Setting_factorydefault_hint2","s")%>";
	if(confirm(alert_string)){
		document.tools_System_Restore.RestartBtn.blur();
		document.tools_System_Restore.rebootFlag.value=1;
		showtext(document.getElementById("loading_block2"), "<%tcWebApi_get("String_Entry","SAVE_restart_desc","s")%>");
		document.getElementById('loading_block3').style.display = "none";
		if(model_name == "DSL-N66U" || model_name == "DSL-AC52U"){
			showLoading(220);
                        setTimeout("redirect();", 220100);
		}
		else{
			showLoading(150);
			setTimeout("redirect();", 150100);
		}
		document.tools_System_Restore.submit();
	}
	else
		return false;
}

function chk_upgrade(){
	$j.ajax({
    		url: '/cgi-bin/detect_firmware_sts.asp',
    		dataType: 'script',
				timeout: 1500,
    		error: function(xhr){
					setTimeout("chk_upgrade();", 1000);
    		},
    		success: function(){
					if(upgrade_sts == "FAIL")
					{
						hideLoading();
						showLoading(120, "upgrade_FAIL_ROMFILE");
						setTimeout("redirect();", 120000);
					}
					else if(upgrade_sts == "WRONG_ROMFILE")
					{
						var form=document.uiPostUpdateForm;
						form.postflag.value = "5";
						form.submit();
					}
					else
						setTimeout("chk_upgrade();", 1000);
  			}
  		});
}
</script>
</head>
<body onload="initial();">
<div id="TopBanner"></div>

<div id="LoadingBar" class="popup_bar_bg">
<table cellpadding="5" cellspacing="0" id="loadingBarBlock" class="loadingBarBlock" align="center">
	<tr>
		<td height="80">
		<div id="loading_block1" class="Bar_container">
			<div id="proceeding_img_text"></div>
			<div id="proceeding_img"></div>
		</div>
		<div id="loading_block2" style="margin:5px auto; width:85%;"></div>
		<div id="loading_block3" style="margin:5px auto; width:85%; font-size:12pt;"></div>	
		</td>
	</tr>
</table>
<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
</div>
<div id="hiddenMask" class="popup_bg">
	<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
		<tr>
		<td>
			<div class="drword" id="drword"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
				<br/>
				<br/>
		    </div>
		  <div class="drImg"><img src="/images/alertImg.png"></div>
			<div style="height:70px; "></div>
		</td>
		</tr>
	</table>
<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
</div>
<div id="Loading" class="popup_bg"></div>
<!--for uniform show, useless but have exist-->

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value=""></form>
<table class="content" align="center" cellpadding="0" cellspacing="0">
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
<td align="left" valign="top" >
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","menu5_6_4","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc"><%tcWebApi_get("String_Entry","Setting_save_upload_desc","s")%></div>

<table width="100%" border="1" align="center" cellpadding="6" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
            <th width="25%" align="right"><a class="hintstyle"  href="javascript:void(0);" onclick="openHint(19,1)"><%tcWebApi_get("String_Entry","Setting_factorydefault_in","s")%></a></th>
<td>
<form method="POST" action="/Advanced_SettingBackup_Content.asp" name="tools_System_Restore" target="hidden_frame">
<input type="hidden" name="testFlag" value="0">
<input type="hidden" name="restoreFlag" value="2">
<input type="hidden" name="rebootFlag" value="0">
<input class="button_gen" onclick="restoreRule();" type="button" value="<%tcWebApi_get("String_Entry","CTL_restore","s")%>" name="RestartBtn" />
</form>
</td>
</tr>
<form enctype="multipart/form-data" method="POST" name="uiPostUpdateForm" target="hidden_frame" action="/start_apply.asp">
<input type="hidden" name="current_page" value="Advanced_SettingBackup_Content.asp">
<input type="hidden" name="postflag" value="1">
<input type="hidden" name="HTML_HEADER_TYPE" value="2">
<input type="hidden" name="value_NONE" value="NONE">
<tr>
<th align="right"><a class="hintstyle"  href="javascript:void(0);" onclick="openHint(19,2)"><%tcWebApi_get("String_Entry","Setting_save_in","s")%></a></th>
<td>
<input class="button_gen" onclick="backup_settings();" type="button" value="<%tcWebApi_get("String_Entry","CTL_onlysave","s")%>" name="action2" />
</td>
</tr>
<tr>
            <th align="right"><a class="hintstyle" href="javascript:void(0);" onclick="openHint(19,3)"><%tcWebApi_get("String_Entry","Setting_upload_in","s")%></a></th>
<td>
<div style="margin-left:-10px;"><table><tr>
<td style="border:0px"><input type="button" class="button_gen" onclick="uploadSetting();" value="<% tcWebApi_Get("String_Entry", "CTL_upload", "s") %>"/></td>
<td style="border:0px"><input type="file" id="tools_FW_UploadFile" name="tools_FW_UploadFile" class="input" style="color:#FFCC00;"/></td>
</tr></table></div>
</td>
</tr>
</table>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
<form name="hint_form"></form>
</body>
<!--Advanced_SettingBackup_Content.asp-->
</html>

