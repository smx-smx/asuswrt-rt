<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_detect.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link type="text/css" rel="stylesheet" href="/qis/qis_style.css">
<link type="text/css" rel="stylesheet" href="/form_style.css">
<style>
	.test_css{
	padding-left:10px;
	font-size:13px;
	font-weight:bolder;
	color:#FFFFFF;
	/*background-color:#C0DAE4;*/
}
</style>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/Javascript" src="/jquery.js"></script>
<script type="text/javascript">
var Redirect_count = 0;
var extend_autodet = 1;
var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";
var dsl_autodet_state = "<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>";
var w_Setting = "<%tcWebApi_get("SysInfo_Entry","w_Setting","s")%>";

var makeRequest = {
        _notSuccessCount: 0,
        _notSupportXML: false,

        start: function(url, callBackSuccess, callBackError){
                var xmlHttp;
                if(window.XMLHttpRequest)
                        xmlHttp = new XMLHttpRequest();
                else if(window.ActiveXObject)
                        xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
                else{
                        makeRequest._notSupportXML = true;
                        alert("Your browser does not support XMLHTTP.");
                        return false;
                }

                xmlHttp.onreadystatechange = function(){
                        if(xmlHttp.readyState == 4){
                                if(xmlHttp.status == 200){
                                        callBackSuccess(xmlHttp.responseXML);
                                }
                                else{
                                        makeRequest._notSuccessCount++;
                                        callBackError();
                                }       
                        }
                }

                xmlHttp.open('GET', url, true);
                xmlHttp.send(null);
        }
};

function QKDetect_load_body(){
	//parent.set_step("t1");
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_detect_sanglass","s")%>";
	if(parent.autodet_annex_counter == 1){	//trigger dsl autodetect again after switch Annex mode
		var update_dsl_info = function(){
			makeRequest.start('/cgi-bin/start_dsl_autodet.asp', function(){}, update_dsl_info);
		};
		setTimeout(update_dsl_info, 700);
	}		
	getWANStatus();
}
function getWANStatus(){
	$.ajax({
		url: '/cgi-bin/result_of_get_changed_status_QIS.asp',
		dataType: 'script',

		error: function(xhr){
			setTimeout("getWANStatus();", 1000);
		},
		success: function(response){
			if(dsl_autodet_state == "pppoe" || dsl_autodet_state == "pppoa")
				redirect_page("ppp_cfg");
			else if(dsl_autodet_state == "dhcp")
				redirect_page("mer_cfg");
			else if(dsl_autodet_state == "Fail")
			{
				if(wan_type == "ATM"){
					document.form.action = "QIS_manual_setting.asp";
				}
				else{
					document.form.action = "QIS_PTM_manual_setting.asp";
				}
				document.form.submit();
				return;
			}
			else if((dsl_autodet_state == "initializing") || (dsl_autodet_state == "wait_for_init") ||
					(dsl_autodet_state == "up") || (dsl_autodet_state == "down") || (dsl_autodet_state == "")) {
					if(dsl_autodet_state == "up" && wan_type == "PTM"){
						document.form.action = "QIS_PTM_manual_setting.asp";
						document.form.submit();
						return;
					}
					++Redirect_count;
					if(Redirect_count >= 24){
					Redirect_count = 0;
					++parent.autodet_annex_counter;
					if(dsl_autodet_state == "up"){
						if(extend_autodet == 1){ //(ATM)extend for another (24-9)*5=75 seconds for auto detection to finish, so max 195 seconds
							extend_autodet = 0;
							Redirect_count = 9;
							getWANStatus();
						}
						else{
							if(wan_type == "ATM"){
								document.form.action = "QIS_manual_setting.asp";
							}
							else{
								document.form.action = "QIS_PTM_manual_setting.asp";
							}
							document.form.submit();
							return;
						}
					}
					else if(parent.autodet_annex_counter == 1 && parent.model_name != "DSL-N66U" && parent.model_name != "DSL-N12U-C1"){	//MODELDEP : Skip DSL-N66U, DSL-N12U-C1
						if(document.redirectForm.AnnexTypeA.value == "ANNEX A/I/J/L/M")
							document.redirectForm.AnnexTypeA.value = "ANNEX B/J/M";
						else if(document.redirectForm.AnnexTypeA.value == "ANNEX B/J/M")
							document.redirectForm.AnnexTypeA.value = "ANNEX A/I/J/L/M";
						document.redirectForm.submit();
					}
					else{
						parent.autodet_annex_counter == 0;
						redirect_page("annex_setting");
					}
				}
				else{
					if(dsl_autodet_state == "down" || dsl_autodet_state == ""){
						set_no_cable_info(true);
					}
					else{
						set_no_cable_info(false);
					}
					setTimeout('getWANStatus();', 5000);
				}
			}
			else{
				set_no_cable_info(true);
				setTimeout('getWANStatus();', 5000);
			}
		}
	});
}

function set_no_cable_info(no_cable){
	if(no_cable)
		document.getElementById("no_cable").style.display = "";
	else
		document.getElementById("no_cable").style.display = "none";
}

function redirect_page(redirect_flag){
	document.form.next_page.value = "/cgi-bin/qis/QIS_"+redirect_flag+".asp";
	document.form.submit();
}

</script>
</head>
<body onLoad="QKDetect_load_body();">
<form method="post" name="form" action="QIS_process.asp" target="">
<input type="hidden" name="prev_page" value="">
<input type="hidden" name="current_page" value="QIS_detect.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="flag" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="preferred_lang" value="EN">
<div class="QISmain">
<div>
<table width="730px">
	<tr>
		<td align="left">
			<span class="description_down"><%tcWebApi_get("String_Entry","QKS_detect_desc1","s")%></span>
		</td>
		<!--td align="right">
			<img onclick="gotoIndex();" style="cursor:pointer;" align="right" title="Go to Home" src="/images/backtohome.png" onMouseOver="this.src='/images/backtohomeclick.png'" onMouseOut="this.src='/images/backtohome.png'">
		</td-->
	</tr>
</table>
</div>
<div style="margin:5px;"><img style="width: 720px; *width: 710px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
<br/><br/>
<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
	<tr>
		<td align="right">
			<img src="/images/InternetScan.gif"></span>
		</td>
		<td class="test_css">
			<strong><span><%tcWebApi_get("String_Entry","QKS_detect_waitdesc","s")%></span></strong>
		</td>
	</tr>
	  <tr id="no_cable" style="display:none;">
		<td>
		</td>
		<td class="test_css">
			<strong><span><%tcWebApi_get("String_Entry","QIS_NoCable_desc","s")%></span></strong>
		</td>
	  </tr>
</table>
</div>
</form>
<form method="post" name="redirectForm" action="QIS_process.asp" target="">
<input type="hidden" name="flag" value="detect">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_detect.asp">
<input type="hidden" name="next_page" value="/cgi-bin/qis/QIS_detect.asp">
<input type="hidden" name="prev_page" value="">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_dsl_setting">
<input type="hidden" name="action_wait" value="1">
<input type="hidden" name="AnnexTypeA" value="<%tcWebApi_get("Adsl_Entry","ANNEXTYPEA","s")%>">
</form>
</body>
