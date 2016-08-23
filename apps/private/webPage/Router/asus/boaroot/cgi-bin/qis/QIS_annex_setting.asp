<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<!--qis/QIS_annex_setting.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/qis/qis_style.css">
<script type="text/Javascript" src="/state.js"></script>
<style>
span{
border:0px solid #FFFFFF;
color:#FFFFFF;
font-size:14px;
font-family:Arial, Helvetica, sans-serif;
/*width:27px;*/
text-align:right;
margin:0px auto;
ime-mode:disabled;
}
</style>
<script type="text/Javascript" src="/jquery.js"></script>
<script type="text/javascript">
<% auto_detection(); %>

var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";

function QKfinish_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_all_title","s")%>";
	parent.set_step("t1");
}
function submitForm(){
	if(validForm()){
			document.form.next_page.value = "/cgi-bin/qis/QIS_detect.asp";
			document.form.submit();
	}		
}
function validForm(){
		if(document.form.AnnexTypeA[0].checked == false && document.form.AnnexTypeA[1].checked == false){
				alert("<%tcWebApi_get("String_Entry","QIS_chooseAnnex_yet","s")%>");
				document.form.AnnexTypeA[0].focus();
				return false;
		}
		return true;
}
function gotomanual() {
	document.form.action = "/cgi-bin/qis/QIS_manual_setting.asp";
	document.form.submit();
}

</script>
</head>
<body onLoad="QKfinish_load_body();" >
<form method="post" name="form" action="QIS_process.asp" target="">
<input type="hidden" name="prev_page" value="/cgi-bin/qis/QIS_detect.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_annex_setting.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_manual_setting.asp">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
<div class="QISmain">
<div>
<table width="730px">
	<tr>
		<td align="left">
			<span class="description_down"><%tcWebApi_get("String_Entry","dslsetting_disc2_setting","s")%></span>
		</td>		
	</tr>
</table>
</div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img style="width: 700px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
<br/><br/>
<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
	<tr>
		<td>
			<input type="radio" value="ANNEX A/I/J/L/M" name="AnnexTypeA" id="id_AnnexTypeA" tabindex="1" class="input"><label for="id_AnnexTypeA">Annex A</label>
		<br><br>
			<input type="radio" value="ANNEX B/J/M" name="AnnexTypeA" id="id_AnnexTypeB" tabindex="2" class="input"><label for="id_AnnexTypeB">Annex B</label>
		<br><br><br>
		<%tcWebApi_get("String_Entry","QIS_chooseAnnex_desc1","s")%><br><br>
		<%tcWebApi_get("String_Entry","ADSL_FW_note","s")%> <%tcWebApi_get("String_Entry","QIS_NoCable_desc","s")%> <%tcWebApi_get("String_Entry","QIS_chooseAnnex_desc2","s")%>
		</td>
	</tr>
</table>
<div class="apply_gen" style="margin-top:30px">
	<!--<input type="button" id="prevButton" value="Manual Setting" onclick="gotomanual(document.form);" class="button_gen" >-->
	<input type="button" id="nextButton" value="<% tcWebApi_Get("String_Entry", "btn_next", "s") %>"  tabindex="3" onclick="submitForm();" class="button_gen">
</div>
</div>
</form>
</body>

<!--qis/QIS_annex_setting.asp-->
</html>
