<%
If Request_Form("Trflag")="1" then 
	TCWebApi_set("TR069_Entry", "tr_enable", "TR_Enable")
	TCWebApi_set("TR069_Entry", "tr_acs_url", "TR_ACSURL")
	TCWebApi_set("TR069_Entry", "tr_username", "TR_ACSUserName")
	TCWebApi_set("TR069_Entry", "tr_passwd", "TR_ACSPassword")
	TCWebApi_set("TR069_Entry", "tr_conn_username", "TR_ConnectionRequestUserName")
	TCWebApi_set("TR069_Entry", "tr_conn_passwd", "TR_ConnectionRequestPassword")
	TCWebApi_set("TR069_Entry", "tr_inform_enable", "TR_PeriodActive")
	TCWebApi_set("TR069_Entry", "tr_inform_interval", "TR_PeriodInterval")
	TCWebApi_set("TR069_Entry", "tr_ca0", "tr_ca0")
	TCWebApi_set("TR069_Entry", "tr_ca1", "tr_ca1")
	TCWebApi_set("TR069_Entry", "tr_ca2", "tr_ca2")
	TCWebApi_set("TR069_Entry", "tr_ca3", "tr_ca3")
	TCWebApi_set("TR069_Entry", "tr_ca4", "tr_ca4")
	TCWebApi_set("TR069_Entry", "tr_ca5", "tr_ca5")
	TCWebApi_set("TR069_Entry", "tr_ccert0", "tr_ccert0")
	TCWebApi_set("TR069_Entry", "tr_ccert1", "tr_ccert1")
	TCWebApi_set("TR069_Entry", "tr_ccert2", "tr_ccert2")
	TCWebApi_set("TR069_Entry", "tr_ccert3", "tr_ccert3")
	TCWebApi_set("TR069_Entry", "tr_ccert4", "tr_ccert4")
	TCWebApi_set("TR069_Entry", "tr_ccert5", "tr_ccert5")
	TCWebApi_set("TR069_Entry", "tr_ckey0", "tr_ckey0")
	TCWebApi_set("TR069_Entry", "tr_ckey1", "tr_ckey1")
	TCWebApi_set("TR069_Entry", "tr_ckey2", "tr_ckey2")
	TCWebApi_set("TR069_Entry", "tr_ckey3", "tr_ckey3")
	TCWebApi_set("TR069_Entry", "tr_ckey4", "tr_ckey4")
	TCWebApi_set("TR069_Entry", "tr_ckey5", "tr_ckey5")

	tcWebApi_commit("TR069_Entry")
End if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_TR069_Content.asp-->
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
<script type="text/javascript" language="JavaScript" src="/help.js"></script>
<script type="text/javascript" language="JavaScript" src="/detect.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
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

.contentM_qis{
	width:740px;	
	margin-top:110px;
	margin-left:380px;
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:200;
	background-color:#2B373B;
	display:none;
	/*behavior: url(/PIE.htc);*/
}
</style>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var $j = jQuery.noConflict();
function trinit(){
	show_menu();

	//load_body();
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_TR069_Content.asp";
}


function trSave()
{
	document.TR_form.Trflag.value=1;

	split_cert_key();

	showLoading(2);
	setTimeout("redirect();", 2000);

	document.TR_form.submit();
}

function cwmpSave()
{
	document.CWMP_form.Cwmpflag.value=1;

	showLoading(2);
	setTimeout("redirect();", 2000);

	document.CWMP_form.submit();
}

function set_cert(){
	document.getElementById('edit_tr_ca_cert').value = document.TR_form.tr_ca_cert.value;
	document.getElementById('edit_tr_client_cert').value = document.TR_form.tr_client_cert.value;
	document.getElementById('edit_tr_client_key').value = document.TR_form.tr_client_key.value;
	cal_panel_block();
	$j("#cert_panel").fadeIn(300);	
}

function cancel_cert_panel(){
	this.FromObject ="0";
	$j("#cert_panel").fadeOut(300);	
	//setTimeout("document.getElementById('edit_tr_ca_cert').value = '<% nvram_clean_get("tr_ca_cert"); %>';", 300);
	//setTimeout("document.getElementById('edit_tr_client_cert').value = '<% nvram_clean_get("tr_client_cert"); %>';", 300);
	//setTimeout("document.getElementById('edit_tr_client_key').value = '<% nvram_clean_get("tr_client_key"); %>';", 300);
	/*setTimeout("document.getElementById('edit_tr_ca_cert').value = decodeURIComponent('<% nvram_char_to_ascii("", "tr_ca_cert"); %>');", 300);
	setTimeout("document.getElementById('edit_tr_client_cert').value = decodeURIComponent('<% nvram_char_to_ascii("", "tr_client_cert"); %>');", 300);
	setTimeout("document.getElementById('edit_tr_client_key').value = decodeURIComponent('<% nvram_char_to_ascii("", "tr_client_key"); %>');", 300);*/
}

function save_cert(){
	document.TR_form.tr_ca_cert.value = document.getElementById('edit_tr_ca_cert').value;
	document.TR_form.tr_client_cert.value = document.getElementById('edit_tr_client_cert').value;
	document.TR_form.tr_client_key.value = document.getElementById('edit_tr_client_key').value;
	cancel_cert_panel();
}

function cal_panel_block(){
	var blockmarginLeft;
	if (window.innerWidth)
		winWidth = window.innerWidth;
	else if ((document.body) && (document.body.clientWidth))
		winWidth = document.body.clientWidth;
		
	if (document.documentElement  && document.documentElement.clientHeight && document.documentElement.clientWidth){
		winWidth = document.documentElement.clientWidth;
	}

	if(winWidth >1050){	
		winPadding = (winWidth-1050)/2;	
		winWidth = 1105;
		blockmarginLeft= (winWidth*0.15)+winPadding;
	}
	else if(winWidth <=1050){
		blockmarginLeft= (winWidth)*0.15+document.body.scrollLeft;	

	}

	$("cert_panel").style.marginLeft = blockmarginLeft+"px";
}

function split_cert_key(){
	/* split ca */
	document.TR_form.tr_ca0.value = document.TR_form.tr_ca_cert.value.substring(0,500);
	document.TR_form.tr_ca1.value = document.TR_form.tr_ca_cert.value.substring(500,1000);
	document.TR_form.tr_ca2.value = document.TR_form.tr_ca_cert.value.substring(1000,1500);
	document.TR_form.tr_ca3.value = document.TR_form.tr_ca_cert.value.substring(1500,2000);
	document.TR_form.tr_ca4.value = document.TR_form.tr_ca_cert.value.substring(2000,2500);
	document.TR_form.tr_ca5.value = document.TR_form.tr_ca_cert.value.substring(2500,3000);

	/* split client cert */
	document.TR_form.tr_ccert0.value = document.TR_form.tr_client_cert.value.substring(0,500);
	document.TR_form.tr_ccert1.value = document.TR_form.tr_client_cert.value.substring(500,1000);
	document.TR_form.tr_ccert2.value = document.TR_form.tr_client_cert.value.substring(1000,1500);
	document.TR_form.tr_ccert3.value = document.TR_form.tr_client_cert.value.substring(1500,2000);
	document.TR_form.tr_ccert4.value = document.TR_form.tr_client_cert.value.substring(2000,2500);
	document.TR_form.tr_ccert5.value = document.TR_form.tr_client_cert.value.substring(2500,3000);

	/* split client key */
	document.TR_form.tr_ckey0.value = document.TR_form.tr_client_key.value.substring(0,500);
	document.TR_form.tr_ckey1.value = document.TR_form.tr_client_key.value.substring(500,1000);
	document.TR_form.tr_ckey2.value = document.TR_form.tr_client_key.value.substring(1000,1500);
	document.TR_form.tr_ckey3.value = document.TR_form.tr_client_key.value.substring(1500,2000);
	document.TR_form.tr_ckey4.value = document.TR_form.tr_client_key.value.substring(2000,2500);
	document.TR_form.tr_ckey5.value = document.TR_form.tr_client_key.value.substring(2500,3000);
}

/* password item show or not */
function TR_pass_checked(obj){
	if(obj.name == "TR_ACSPassword")
		switchType(obj, document.TR_form.show_TR_ACSPassword.checked, true);
	else	//TR_ACSPassword
		switchType(obj, document.TR_form.show_TR_ConnectionRequestPassword.checked, true);	
}

</script>
</head>
<body onload="trinit();" onunLoad="return unload_body();">

<div id="cert_panel"  class="contentM_qis" style="box-shadow: 3px 3px 10px #000;">
		<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
			<tr>
				<div class="description_down">Keys and Certification</div>
			</tr>
			<tr>
				<div style="margin-left:30px; margin-top:10px;">
					<p>Only paste the content of the <span style="color:#FFCC00;">----- BEGIN xxx ----- </span>/<span style="color:#FFCC00;"> ----- END xxx -----</span> block (including those two lines).
					<p>Limit: 2999 characters per field
				</div>
				<div style="margin:5px;*margin-left:-5px;"><img style="width: 730px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
			</tr>			
			<!--===================================Beginning of tls Content===========================================-->
        	<tr>
          		<td valign="top">
            		<table width="700px" border="0" cellpadding="4" cellspacing="0">
                	<tbody>
                	<tr>
                		<td valign="top">
							<table width="100%" id="page_cert" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
								<tr>
									<th>Certificate Authority</th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" id="edit_tr_ca_cert" name="edit_tr_ca_cert" cols="65" maxlength="2999"></textarea>
									</td>
								</tr>
								<tr>
									<th>Client Certificate</th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" id="edit_tr_client_cert" name="edit_tr_client_cert" cols="65" maxlength="2999"></textarea>
									</td>
								</tr>
								<tr>
									<th>Client Key</th>
									<td>
										<textarea rows="8" class="textarea_ssh_table" id="edit_tr_client_key" name="edit_tr_client_key" cols="65" maxlength="2999"></textarea>
									</td>
								</tr>
							</table>
			  			</td>
			  		</tr>						
	      			</tbody>						
      				</table>
						<div style="margin-top:5px;width:100%;text-align:center;">
							<input class="button_gen" type="button" onclick="cancel_cert_panel();" value="<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>">
							<input class="button_gen" type="button" onclick="save_cert();" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>">	
						</div>					
          		</td>
      		</tr>
      </table>		
      <!--===================================Ending of Certification & Key Content===========================================-->			
</div>

<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_TR069_Content.asp" name="TR_form" target="hidden_frame">
<INPUT TYPE="HIDDEN" NAME="Trflag" VALUE="0">
<input type="hidden" name="tr_ca_cert" value="<% tcWebApi_Get("TR069_Entry", "tr_ca0", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ca1", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ca2", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ca3", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ca4", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ca5", "s") %>">
<input type="hidden" name="tr_client_cert" value="<% tcWebApi_Get("TR069_Entry", "tr_ccert0", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ccert1", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ccert2", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ccert3", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ccert4", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ccert5", "s") %>">
<input type="hidden" name="tr_client_key" value="<% tcWebApi_Get("TR069_Entry", "tr_ckey0", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ckey1", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ckey2", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ckey3", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ckey4", "s") %><% tcWebApi_Get("TR069_Entry", "tr_ckey5", "s") %>">
<input type="hidden" name="tr_ca0" value="">
<input type="hidden" name="tr_ca1" value="">
<input type="hidden" name="tr_ca2" value="">
<input type="hidden" name="tr_ca3" value="">
<input type="hidden" name="tr_ca4" value="">
<input type="hidden" name="tr_ca5" value="">
<input type="hidden" name="tr_ccert0" value="">
<input type="hidden" name="tr_ccert1" value="">
<input type="hidden" name="tr_ccert2" value="">
<input type="hidden" name="tr_ccert3" value="">
<input type="hidden" name="tr_ccert4" value="">
<input type="hidden" name="tr_ccert5" value="">
<input type="hidden" name="tr_ckey0" value="">
<input type="hidden" name="tr_ckey1" value="">
<input type="hidden" name="tr_ckey2" value="">
<input type="hidden" name="tr_ckey3" value="">
<input type="hidden" name="tr_ckey4" value="">
<input type="hidden" name="tr_ckey5" value="">
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
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - TR069</div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc">TR069 Setup</div>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
          <td colspan="2"><%tcWebApi_get("String_Entry","CWMP_ACS","s")%></td>
</tr>
</thead>
	<tr>
	<th width="40%"><% tcWebApi_Get("String_Entry", "CWMP_enable", "s") %></th>
	<td>
		<input name="TR_Enable" type="radio" value="1" <%if tcWebApi_get("TR069_Entry", "tr_enable", "h") = "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
		<input name="TR_Enable" type="radio" value="0" <%if tcWebApi_get("TR069_Entry", "tr_enable", "h") = "0" then asp_Write("checked") elseif tcWebApi_get("TR069_Entry", "tr_enable", "h") = "N/A" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>

	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","CWMP_URL","s")%></a></th>
	<td>
		<input name="TR_ACSURL" type="text" value="<%if tcWebApi_get("TR069_Entry", "tr_acs_url", "h") <> "N/A" then tcWebApi_get("TR069_Entry", "tr_acs_url", "s") end if%>" maxlength="256" class="input_32_table">
		<span id="cert_text" onclick="set_cert();" style="text-decoration:underline;cursor:pointer;">Import Certificate</span>
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%></a></th>
	<td>
		<input name="TR_ACSUserName" type="text" value="<%if tcWebApi_get("TR069_Entry", "tr_username", "h") <> "N/A" then tcWebApi_get("TR069_Entry",  "tr_username", "s") end if%>" maxlength="256" class="input_32_table" onKeyPress="return is_string(this, event);">
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></a></th>
	<td>
		<input type="password" autocapitalization="off" autocomplete="off" name="TR_ACSPassword" value="<%if tcWebApi_get("TR069_Entry", "tr_passwd", "h") <> "N/A" then tcWebApi_get("TR069_Entry", "tr_passwd", "s") end if%>" maxlength="256" class="input_32_table" onKeyPress="return is_string(this, event);">
		<div style="margin:-25px 0px 5px 355px;"><input type="checkbox" name="show_TR_ACSPassword" onclick="TR_pass_checked(document.TR_form.TR_ACSPassword);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
	</td>
	</tr>
</table>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:10px;">
<thead>
<tr>
          <td colspan="2"><%tcWebApi_get("String_Entry","CWMP_Conn_Info","s")%></td>
</tr>
</thead>
	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Username_in","s")%></a></th>
	<td>
		<input name="TR_ConnectionRequestUserName" type="text" value="<%if tcWebApi_get("TR069_Entry", "tr_conn_username", "h") <> "N/A" then tcWebApi_get("TR069_Entry", "tr_conn_username", "s") end if%>" maxlength="256" class="input_32_table" onKeyPress="return is_string(this, event);">
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","HSDPAC_Password_in","s")%></a></th>
	<td>
		<input type="password" autocapitalization="off" autocomplete="off" name="TR_ConnectionRequestPassword" value="<%if tcWebApi_get("TR069_Entry", "tr_conn_passwd", "h") <> "N/A" then tcWebApi_get("TR069_Entry", "tr_conn_passwd" ,"s") end if%>" maxlength="256" class="input_32_table" onKeyPress="return is_string(this, event);">
		<div style="margin:-25px 0px 5px 355px;"><input type="checkbox" name="show_TR_ConnectionRequestPassword" onclick="TR_pass_checked(document.TR_form.TR_ConnectionRequestPassword);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
	</td>
	</tr>
</table>

<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:10px;">
<thead>
<tr>
          <td colspan="2"><%tcWebApi_get("String_Entry","CWMP_Periodic_Inform","s")%></td>
</tr>
</thead>
	<tr>
	<th width="40%"><% tcWebApi_Get("String_Entry", "CWMP_Periodic_Inform_enable", "s") %></th>
	<td>
		<input name="TR_PeriodActive" value="1" type="radio" <%if tcWebApi_get("TR069_Entry", "tr_inform_enable", "h") = "1" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
		<input name="TR_PeriodActive" value="0" type="radio" <%if tcWebApi_get("TR069_Entry", "tr_inform_enable", "h") = "0" then asp_Write("checked") elseif tcWebApi_get("TR069_Entry", "tr_inform_enable", "h") = "N/A" then asp_Write("checked") end if%> ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
	</td>
	</tr>

	<tr>
	<th><a class="hintstyle" href="javascript:void(0);"><%tcWebApi_get("String_Entry","Interval","s")%></a></th>
	<td>
		<input type="text" name="TR_PeriodInterval" value="<%if tcWebApi_get("TR069_Entry", "tr_inform_interval", "h") <> "N/A" then tcWebApi_get("TR069_Entry", "tr_inform_interval", "s") end if%>" maxlength="9" class="input_12_table">
	</td>
	</tr>
</table>

  
<div class="apply_gen">
<input name="tr_apply" type="button" class="button_gen" onclick="trSave()" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
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

<!--Advanced_TR069_Content.asp-->
</html>

