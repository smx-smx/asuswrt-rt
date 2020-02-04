<%
If Request_Form("Cwmpflag")="1" then 
	TCWebApi_set("Cwmp_Entry", "Active", "CWMP_Active")
	TCWebApi_set("Cwmp_Entry", "acsUrl", "CWMP_ACSURL")
	TCWebApi_set("Cwmp_Entry", "acsUserName", "CWMP_ACSUserName")
	TCWebApi_set("Cwmp_Entry", "acsPassword", "CWMP_ACSPassword")
	TCWebApi_set("Cwmp_Entry", "conReqPath", "CWMP_ConnectionRequestPath")
	TCWebApi_set("Cwmp_Entry", "conReqUserName", "CWMP_ConnectionRequestUserName")
	TCWebApi_set("Cwmp_Entry", "conReqPassword", "CWMP_ConnectionRequestPassword")
	
	TCWebApi_set("Cwmp_Entry", "periodActive", "CWMP_PeriodActive")
	TCWebApi_set("Cwmp_Entry", "periodInterval", "CWMP_PeriodInterval")
	
	tcWebApi_commit("Cwmp_Entry")
End if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_CWMP_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_6_2","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/pwdmeter.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/jquery.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<style>
#ClientList_Block_PC{
border:1px outset #999;
background-color:#576D73;
position:absolute;
margin-top:106px;
*margin-top:96px;
margin-left:127px;
width:345px;
text-align:left;
height:auto;
overflow-y:auto;
z-index:200;
padding: 1px;
display:none;
}
#ClientList_Block_PC div{
background-color:#576D73;
height:auto;
*height:20px;
line-height:20px;
text-decoration:none;
font-family: Lucida Console;
padding-left:2px;
}
#ClientList_Block_PC a{
background-color:#EFEFEF;
color:#FFF;
font-size:12px;
font-family:Arial, Helvetica, sans-serif;
text-decoration:none;
}
#ClientList_Block_PC div:hover, #ClientList_Block a:hover{
background-color:#3366FF;
color:#FFFFFF;
cursor:default;
}
</style>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var $j = jQuery.noConflict();
function cwmpinit(){
	show_menu();

	//load_body();
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_CWMP_Content.asp";
}


function cwmpSave()
{
	document.CWMP_form.Cwmpflag.value=1;

	showLoading(2);
	setTimeout("redirect();", 2000);

	document.CWMP_form.submit();
}
</script>
</head>
<body onload="cwmpinit();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_CWMP_Content.asp" name="CWMP_form" target="hidden_frame">
<INPUT TYPE="HIDDEN" NAME="Cwmpflag" VALUE="0">
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
<td valign="top">
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","CWMP","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc"><%tcWebApi_get("String_Entry","CWMP_setup","s")%></div>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
          <td colspan="2"><%tcWebApi_get("String_Entry","CWMP_ACS","s")%></td>
</tr>
</thead>
	<tr>
	<th width="40%"><% tcWebApi_Get("String_Entry", "CWMP_enable", "s") %></th>
	<td>
		<input name="CWMP_Active" type="radio" value="Yes" <%if tcWebApi_get("Cwmp_Entry", "Active", "h") = "Yes" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
		<input name="CWMP_Active" type="radio" value="No" <%if tcWebApi_get("Cwmp_Entry", "Active", "h") = "No" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry", "Active", "h") = "N/A" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>

	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","CWMP_URL","s")%></a></th>
	<td>
		<input name="CWMP_ACSURL" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsUrl", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "acsUrl", "s") end if%>" size="64" maxlength="256" class="input_32_table">
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%></a></th>
	<td>
		<input name="CWMP_ACSUserName" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsUserName", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry",  "acsUserName", "s") end if%>" size="32" maxlength="256" class="input_32_table" onKeyPress="return validator.isString(this, event);">
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></a></th>
	<td>
		<input name="CWMP_ACSPassword" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "acsPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "acsPassword", "s") end if%>" size="32" maxlength="256" class="input_32_table" onKeyPress="return validator.isString(this, event);">
	</td>
	</tr>
</table>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
          <td colspan="2"><%tcWebApi_get("String_Entry","CWMP_Conn_Info","s")%></td>
</tr>
</thead>
	<tr>
	<th width="40%"><% tcWebApi_Get("String_Entry", "CWMP_path", "s") %></th>
	<td>
		<input name="CWMP_ConnectionRequestPath" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqPath", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqPath", "s") end if%>" size="63" maxlength="256" class="input_32_table" onKeyPress="return validator.isString(this, event);">
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%></a></th>
	<td>
		<input name="CWMP_ConnectionRequestUserName" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqUserName", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqUserName", "s") end if%>" size="32" maxlength="256" class="input_32_table" onKeyPress="return validator.isString(this, event);">
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></a></th>
	<td>
		<input name="CWMP_ConnectionRequestPassword" type="text" value="<%if tcWebApi_get("Cwmp_Entry", "conReqPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqPassword" ,"s") end if%>" size="32" maxlength="256" class="input_32_table" onKeyPress="return validator.isString(this, event);">
	</td>
	</tr>
</table>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
          <td colspan="2"><%tcWebApi_get("String_Entry","CWMP_Periodic_Inform","s")%></td>
</tr>
</thead>
	<tr>
	<th width="40%"><% tcWebApi_Get("String_Entry", "CWMP_Periodic_Inform_enable", "s") %></th>
	<td>
		<input name="CWMP_PeriodActive" value="Yes" type="radio" <%if tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "Yes" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
		<input name="CWMP_PeriodActive" value="No" type="radio" <%if tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "No" then asp_Write("checked") elseif tcWebApi_get("Cwmp_Entry", "periodActive", "h") = "N/A" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","Interval","s")%></a></th>
	<td>
		<input type="text" name="CWMP_PeriodInterval" value="<%if tcWebApi_get("Cwmp_Entry", "periodInterval", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "periodInterval", "s") end if%>" size="9" maxlength="9" class="input_12_table">
	</td>
	</tr>
</table>

  
<div class="apply_gen">
<input name="cwmp_apply" type="button" class="button_gen" onclick="cwmpSave()" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>


</div>
</td></tr>
</tbody>
</table></td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_CWMP_Content.asp-->
</html>

