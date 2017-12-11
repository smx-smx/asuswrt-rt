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
var linkup_autodet = 0;
var AnnexSwitch_count = 0;
var AnnexSwitch_enable = 1;
var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";	// ATM | PTM
var wan_type_info = "<%tcWebApi_get("Info_Adsl","xDSLmode","s")%>";	// ADSL | VDSL
var wan_type_compare = (wan_type_info=="VDSL")? "PTM":"ATM"; 
var dsl_autodet_state = "<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>";
var dsl_line_state = "<% tcWebApi_get("Info_Adsl","lineState","s") %>";
var AnnexTypeA_orig = "<%tcWebApi_get("Adsl_Entry","ANNEXTYPEA","s")%>";
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
	parent.set_step("t1");
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","QKS_detect_sanglass","s")%>";
	if(parent.autodet_annex_counter > 0 || wan_type != wan_type_compare){	//trigger dsl autodetect again after switch Annex mode || Detect_XDSL not matched to xDSLmode
		var update_dsl_info = function(){
			makeRequest.start('/cgi-bin/start_dsl_autodet.asp', function(){}, update_dsl_info);
		};
		setTimeout(update_dsl_info, 100);
	}		
	setTimeout("getWANStatus()", 700);
}
function getWANStatus(){
	$.ajax({
		url: '/cgi-bin/result_of_get_changed_status_QIS.asp',
		dataType: 'script',

		error: function(xhr){
			setTimeout("getWANStatus();", 1000);
		},
		success: function(response){
			
			if(dsl_autodet_state == "Fail"){
				if(wan_type_info == "VDSL"){	//wan_type == "PTM"
					redirect_page("PTM_manual_setting");
				}	
				else{
					redirect_page("manual_setting");
				}						
			}
			
			
			if( dsl_line_state == "up"){

				if(wan_type_info == "VDSL"){	//wan_type == "PTM"
					redirect_page("PTM_manual_setting");
				}	
				else if(wan_type_info == "ADSL" && (AnnexTypeA_orig == "ANNEX B" || AnnexTypeA_orig == "ANNEX B/J" || AnnexTypeA_orig == "ANNEX B/J/M")){	//wan_type == "ATM"	
					redirect_page("manual_setting");
				}	
				else if(dsl_autodet_state == "pppoe" || dsl_autodet_state == "pppoa"){
					redirect_page("ppp_cfg");
				}	
				else if(dsl_autodet_state == "dhcp"){
					redirect_page("mer_cfg");
				}	
				else{
					AnnexSwitch_enable = 0;
					linkup_autodet = 1;

					++Redirect_count;
					if(Redirect_count >= 36){	//36*5 = 180 sec = 3 min
						if(extend_autodet == 1){ //(ATM)extend for another 3*5=15 seconds for auto detection to finish, so max 195 seconds
							extend_autodet = 0;
							Redirect_count -= 3;
						}
						else{
							if(wan_type_info == "VDSL"){	//wan_type == "PTM"
								document.form.action = "QIS_PTM_manual_setting.asp";
							}
							else{
								document.form.action = "QIS_manual_setting.asp";
							}
							document.form.submit();
							return;
						}
					}
					set_state_info(dsl_line_state);
					setTimeout('getWANStatus();', 5000);
				}
			}
			else if ((dsl_line_state == "initializing")){
				if(linkup_autodet == 1) {	//up -> down, restart auto det
					linkup_autodet = 0;
					Redirect_count = 0;
					document.redirectForm.action_script.value = "restart_autodet";
					document.redirectForm.submit();
				}
				AnnexSwitch_enable = 0;
				set_state_info(dsl_line_state);
				setTimeout('getWANStatus();', 5000);
			}
			else if ( (AnnexSwitch_enable == 1) &&
				((dsl_line_state == "down") || (dsl_line_state == "") || (dsl_line_state == "wait for init"))
			){
				if(linkup_autodet == 1) {	//up -> down, restart auto det
					linkup_autodet = 0;
					Redirect_count = 0;
					document.redirectForm.action_script.value = "restart_autodet";
					document.redirectForm.submit();
				}

				++AnnexSwitch_count;
				if(AnnexSwitch_count >= 18){	//18*5 = 90 sec = 1.5 min
					AnnexSwitch_count = 0;
					++parent.autodet_annex_counter;
					if(parent.autodet_annex_counter <= 3 && parent.model_name != "DSL-N66U" && parent.model_name != "DSL-N12U-C1" && parent.model_name != "DSL-N12U-D1"){	//MODELDEP : Skip DSL-N66U, DSL-N12U-C1, DSL-N12U-D1
						if(document.redirectForm.AnnexTypeA.value == "ANNEX A/I/J/L/M")
							document.redirectForm.AnnexTypeA.value = "ANNEX B/J/M";
						else if(document.redirectForm.AnnexTypeA.value == "ANNEX B/J/M"
								|| document.redirectForm.AnnexTypeA.value == "ANNEX B/J")
							document.redirectForm.AnnexTypeA.value = "ANNEX A/I/J/L/M";
						document.redirectForm.action_script.value = "restart_dsl_setting";
						document.redirectForm.submit();

						set_state_info(dsl_line_state);
						setTimeout('getWANStatus();', 5000);
					}
					else{
						parent.autodet_annex_counter = 0;
						redirect_page("annex_setting");
					}
				}
				else {
					set_state_info(dsl_line_state);
					setTimeout('getWANStatus();', 5000);
				}
			}
			else{
				set_state_info(dsl_line_state);
				setTimeout('getWANStatus();', 5000);
			}
		}
	});
}

function set_state_info(state){
	switch(state) {				
				case "":
								document.getElementById("LED_state").innerHTML = "<%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%> : <span style=\"color:#FFCC00;\">Link down (DSL LED Off)</span><br><br>";
								document.getElementById("LED_state").style.display = "";		
								document.getElementById("no_cable").style.display = "";
								break;
				case "down":
								document.getElementById("LED_state").innerHTML = "<%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%> : <span style=\"color:#FFCC00;\">Link down (DSL LED Off)</span><br><br>";
								document.getElementById("LED_state").style.display = "";		
								document.getElementById("no_cable").style.display = "";
								break;
				case "wait for init":
								document.getElementById("LED_state").innerHTML = "<%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%> : <span style=\"color:#FFCC00;\">Wait for init (DSL LED Flashing)</span><br><br>";
								document.getElementById("LED_state").style.display = "";		
								document.getElementById("no_cable").style.display = "none";
								break;
				case "initializing":
								document.getElementById("LED_state").innerHTML = "<%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%> : <span style=\"color:#FFCC00;\">Initializing (DSL LED Flashing)</span><br><br>";
								document.getElementById("LED_state").style.display = "";		
								document.getElementById("no_cable").style.display = "none";
								break;
				case "up":
								document.getElementById("LED_state").innerHTML = "<%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%> : <span style=\"color:#FFCC00;\">Link up (DSL LED On)</span><br><br>";
								document.getElementById("LED_state").style.display = "";		
								document.getElementById("no_cable").style.display = "none";
								break;
				default:	//include ""
								document.getElementById("LED_state").style.display = "none";		
								document.getElementById("no_cable").style.display = "none";
								break;
								
	}
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
	<tr>
		<td>
		</td>
		<td class="test_css">
			<strong><span><%tcWebApi_get("String_Entry","QKS_detect_waitdesc1","s")%></span></strong>
		</td>
	</tr>
	<tr>
		<td>
		</td>
		<td class="test_css">			
			<strong><span id="LED_state" style="display:none;"></span></strong>
			<strong><span id="no_cable" style="color:#FFCC00;display:none;"><%tcWebApi_get("String_Entry","QIS_NoCable_desc","s")%></span></strong>
		</td>
	</tr>
</table>
</div>
</form>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="redirectForm" action="QIS_process.asp"  target="hidden_frame">
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
