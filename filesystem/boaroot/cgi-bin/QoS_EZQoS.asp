<% If Request_Form("apply_flag") = "1" then
	tcWebApi_set("QoS_Entry0","qos_enable","qos_enable")
	tcWebApi_set("QoS_Entry0","qos_obw", "qos_obw")
	tcWebApi_set("QoS_Entry0","qos_ibw", "qos_ibw")
	tcWebApi_CommitWithoutSave("Firewall")
	tcWebApi_commit("QoS")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--QoS_EZQoS.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <% tcWebApi_get("SysInfo_Entry","ProductName","s") %> <% tcWebApi_get("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","EZQoS","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script>
var $j = jQuery.noConflict();
</script>
<style type="text/css">
.qos_table{
	width:740px;
	padding:10px; 
	margin-top:-17px;
	position:relative;
	background-color:#4d595d;
	height: 650px;
	-webkit-border-bottom-right-radius: 3px;
	-webkit-border-bottom-left-radius: 3px;
	-moz-border-radius-bottomright: 3px;
	-moz-border-radius-bottomleft: 3px;
	border-bottom-right-radius: 3px;
	border-bottom-left-radius: 3px;
}
</style>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var qos_rulelist_array = "<% tcWebApi_get("QoS_Entry0","qos_rulelist","s") %>";

var overlib_str0 = new Array();	//Viz add 2011.06 for record longer qos rule desc
var overlib_str = new Array();	//Viz add 2011.06 for record longer portrange value

function initial(){
	show_menu();

	if(downsize_support != -1)
		$("guest_image").parentNode.style.display = "none";

	if(document.form.qos_enable.value==1){
		document.form.obw.parentNode.parentNode.style.display = "";
		document.form.ibw.parentNode.parentNode.style.display = "";		
	}else{
		document.form.obw.parentNode.parentNode.style.display = "none";
		document.form.ibw.parentNode.parentNode.style.display = "none";		
	}
	
	init_changeScale();	
	showqos_rulelist();
	addOnlineHelp($("faq"), ["ASUSWRT", "QoS"]);
}

function init_changeScale(){
	var upload = document.form.qos_obw.value;
	var download = document.form.qos_ibw.value;
	
	document.form.obw.value = upload/1024;
	document.form.ibw.value = download/1024;
}

function changeScale(_obj_String){
	if($(_obj_String+"_scale").value == "Mb/s")
		$(_obj_String).value = Math.round(($(_obj_String).value/1024)*100)/100;
	else
		$(_obj_String).value = Math.round($(_obj_String).value*1024);
}

function switchPage(page){
	if(page == "2")
		location.href = "Advanced_QOSUserPrio_Content.asp";
	else if(page == "3")
		location.href = "Advanced_QOSUserRules_Content.asp";
	else
		return false;
}

function submitQoS(){
	if(document.form.qos_enable.value == 0 && document.form.qos_enable_orig.value == 0){
		return false;
	}

	if(document.form.qos_enable.value == 1){
		// Jieming To Do: please add a hint here when error occurred, and qos_ibw & qos_obw should allow number only.
		if(document.form.obw.value.length == 0){
				alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
				document.form.obw.focus();
				document.form.obw.select();
				return;
		}
		else if(document.form.obw.value == 0){
				alert("Upload Bandwidth can not be 0");
				document.form.obw.focus();
				document.form.obw.select();
				return;	
			
		}
		else if(document.form.obw.value.split(".").length > 2){		//To check more than two point symbol
			alert("The format of field of upload bandwidth is invalid");
			document.form.obw.focus();
			document.form.obw.select();
			return;	
		}
		
		if(document.form.ibw.value.length == 0){
				alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
				document.form.ibw.focus();
				document.form.ibw.select();
				return;
		}
		else if(document.form.ibw.value < 1){
			alert("Download Bandwidth can not be less than 1");
			document.form.ibw.focus();
			document.form.ibw.select();
			return;	
		}
		else if(document.form.ibw.value.split(".").length > 2){
			alert("The format of field of download bandwidth is invalid");
			document.form.ibw.focus();
			document.form.ibw.select();
			return;	
		}
		// end
  }	

	document.form.qos_obw.disabled = false;
	document.form.qos_ibw.disabled = false;
	document.form.qos_obw.value = document.form.obw.value*1024;
	document.form.qos_ibw.value = document.form.ibw.value*1024;
	
	document.form.apply_flag.value = "1";

	document.form.action = "/cgi-bin/QoS_EZQoS.asp";
	showLoading(5);
	setTimeout('location = "'+ location.pathname +'";', 5000);
	document.form.submit();	
	
}

function showqos_rulelist(){
	var qos_rulelist_row = "";
	qos_rulelist_row = decodeURIComponent(qos_rulelist_array).split('<');	

	var code = "";
	code +='<table style="margin-left:3px;" width="95%" border="1" align="center" cellpadding="4" cellspacing="0" class="list_table" id="qos_rulelist_table">';
	if(qos_rulelist_row.length == 1)	// no exist "<"
		code +='<tr><td style="color:#FFCC00;height:30px;" colspan="6"><%tcWebApi_get("String_Entry","checkbox_No","s")%> data in table.</td></tr>';
	else{
		for(var i = 1; i < qos_rulelist_row.length; i++){
			overlib_str0[i] ="";
			overlib_str[i] ="";			
			code +='<tr id="row'+i+'">';
			var qos_rulelist_col = qos_rulelist_row[i].split('>');
			var wid=[22, 21, 17, 14, 16, 12];						
				for(var j = 0; j < qos_rulelist_col.length; j++){
						if(j != 0 && j !=2 && j!=5){
							code +='<td width="'+wid[j]+'%" style="height:30px;">'+ qos_rulelist_col[j] +'</td>';
						}else if(j==0){
							if(qos_rulelist_col[0].length >15){
								overlib_str0[i] += qos_rulelist_col[0];
								qos_rulelist_col[0]=qos_rulelist_col[0].substring(0, 13)+"...";
								code +='<td width="'+wid[j]+'%"  title="'+overlib_str0[i]+'" style="height:30px;">'+ qos_rulelist_col[0] +'</td>';
							}else
								code +='<td width="'+wid[j]+'%" style="height:30px;">'+ qos_rulelist_col[j] +'</td>';
						}else if(j==2){
							if(qos_rulelist_col[2].length >13){
								overlib_str[i] += qos_rulelist_col[2];
								qos_rulelist_col[2]=qos_rulelist_col[2].substring(0, 11)+"...";
								code +='<td width="'+wid[j]+'%"  title="'+overlib_str[i]+'" style="height:30px;">'+ qos_rulelist_col[2] +'</td>';
							}else
								code +='<td width="'+wid[j]+'%" style="height:30px;">'+ qos_rulelist_col[j] +'</td>';
						}else if(j==5){
								code += '<td width="'+wid[j]+'%" style="height:30px;">';

								if(qos_rulelist_col[5] =="0")
									code += 'Highest';
								if(qos_rulelist_col[5] =="1")
									code += 'High';
								if(qos_rulelist_col[5] =="2")
									code += 'Medium';
								if(qos_rulelist_col[5] =="3")
									code += 'Low';
								if(qos_rulelist_col[5] =="4")
									code += 'Lowest';
						}
						code +='</td>';
				}
				code +='</tr>';
		}
	}
	code +='</table>';
	$("qos_rulelist_Block").innerHTML = code;
	
	
	parse_port="";
}
</script>
</head>

<body onload="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_get("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="current_page" value="QoS_EZQoS.asp">
<input type="hidden" name="next_page" value="QoS_EZQoS.asp">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_qos">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="flag" value="">
<input type="hidden" name="qos_enable" value="<%if tcWebApi_get("QoS_Entry0","qos_enable","h") = "1" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="qos_enable_orig" value="<%if tcWebApi_get("QoS_Entry0","qos_enable","h") = "1" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="qos_obw" value="<% tcWebApi_get("QoS_Entry0","qos_obw","s") %>" disabled>
<input type="hidden" name="qos_ibw" value="<% tcWebApi_get("QoS_Entry0","qos_ibw","s") %>" disabled>
<table class="content" align="center" cellpadding="0" cellspacing="0">
  <tr>
	<td width="17">&nbsp;</td>
	
	<!--=====Beginning of Main Menu=====-->
	<td valign="top" width="202">
	  <div id="mainMenu"></div>
	  <div id="subMenu"></div>
	</td>
	
    <td valign="top">
			<div id="tabMenu" class="submenuBlock"></div>
			<br>
		<!--===================================Beginning of Main Content===========================================-->
		<div class="qos_table" id="qos_table">
		<table >
  		<tr>
    			<td bgcolor="#4D595D" valign="top">
    				<table>
        			<tr>
						<td>
							<table width="100%">
								<tr>
									<td  class="formfonttitle" align="left">								
										<div ><% tcWebApi_Get("String_Entry", "Menu_TrafficManager", "s") %> - QoS</div>
									</td>
									<td align="right" >
										<div>
											<select onchange="switchPage(this.options[this.selectedIndex].value)" class="input_option">
												<!--option><%tcWebApi_get("String_Entry","switchpage","s")%></option-->
												<option value="1" selected><%tcWebApi_get("String_Entry","qos_automatic_mode","s")%></option>
												<option value="2"><% tcWebApi_Get("String_Entry", "qos_user_prio", "s") %></option>
												<option value="3"><% tcWebApi_Get("String_Entry", "qos_user_rules", "s") %></option>
											</select>	    
										</div>
									</td>	
								</tr>
							</table>	
						</td>
        			</tr>
        			<tr>
          				<td height="5"><img src="/images/New_ui/export/line_export.png" /></td>
        			</tr>
        			<tr>
          				<td height="30" align="left" valign="middle" >
										<div>
											<table width="650px">
												<tr>
													<td width="130px">
														<img id="guest_image" src="/images/New_ui/QoS.png">
													</td>
													<td style="font-style: italic;font-size: 14px;">
														<div class="formfontdesc" style="line-height:20px;"><%tcWebApi_get("String_Entry","ezqosDesw","s")%></div>
														<div class="formfontdesc">
															<a id="faq" href="" target="_blank" style="text-decoration:underline;">QoS FAQ</a>
														</div>
													</td>
												</tr>
											</table>
										</div>
          				</td>
        			</tr>
							
							<tr>
								<td>
									<table style="margin-left:3px; margin-top:15px;" width="95%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
										<tr>
										<th><%tcWebApi_get("String_Entry","EzQoS_smart_enable","s")%></th>
											<td>
												<div class="left" style="width:94px; float:left; cursor:pointer;" id="radio_qos_enable"></div>
												<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
												<script type="text/javascript">
													$j('#radio_qos_enable').iphoneSwitch('<%tcWebApi_get("QoS_Entry0","qos_enable","s")%>', 
														 function() {
															document.form.qos_enable.value = "1";
															document.form.obw.parentNode.parentNode.style.display = "";
															document.form.ibw.parentNode.parentNode.style.display = "";
														 },
														 function() {
															document.form.qos_enable.value = "0";
															document.form.obw.parentNode.parentNode.style.display = "none";
															document.form.ibw.parentNode.parentNode.style.display = "none";
														 },
														 {
															switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
														 }
													);
												</script>			
												</div>	
											</td>
										</tr>										
										
										<tr id="upload_tr">
											<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 2);"><%tcWebApi_get("String_Entry","upload_bandwidth","s")%></a></th>
											<td>
												<input type="text" maxlength="10" id="obw" name="obw" onKeyPress="return isNumberFloat(this,event);" class="input_15_table" value="">
												<label style="margin-left:5px;">Mb/s</label>
											</td>
										</tr>									
										<tr id="download_tr">
											<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(20, 2);"><%tcWebApi_get("String_Entry","download_bandwidth","s")%></a></th>
											<td>
												<input type="text" maxlength="10" id="ibw" name="ibw" onKeyPress="return isNumberFloat(this,event);" class="input_15_table" value="">
												<label style="margin-left:5px;">Mb/s</label>
											</td>
										</tr>

									</table>
								</td>
				</tr>	

        			<tr>
          				<td height="50" >
          					<div style=" *width:136px;margin-left:300px;" class="titlebtn" align="center" onClick="submitQoS();">
								<input type="hidden" name="apply_flag" value="0">
								<span><%tcWebApi_get("String_Entry","CTL_onlysave","s")%></span>
							</div>
          				</td>
        			</tr>
        			<tr>
          				<td>
											<table style="margin-left:3px; margin-top:15px;" width="95%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
											<thead>
											<tr>
													<td colspan="6" id="TriggerList" style="border-right:none;height:22px;"><% tcWebApi_Get("String_Entry", "BM_UserList_title", "s") %> (<% tcWebApi_Get("String_Entry", "List_limit", "s") %> 32)</td>
											</tr>
											</thead>			
											<tr>
													<th width="22%" style="height:30px;"><% tcWebApi_Get("String_Entry", "BM_UserList1", "s") %></th>
													<th width="21%"><a href="javascript:void(0);" onClick="openHint(18,6);"><div class="table_text"><% tcWebApi_Get("String_Entry", "BM_UserList2", "s") %></div></a></th>
													<th width="17%"><a href="javascript:void(0);" onClick="openHint(18,4);"><div class="table_text"><% tcWebApi_Get("String_Entry", "BM_UserList3", "s") %></div></a></th>
													<th width="14%"><div class="table_text"><% tcWebApi_Get("String_Entry", "IPC_VServerProto_in", "s") %></div></th>
													<th width="16%"><a href="javascript:void(0);" onClick="openHint(18,5);"><div class="table_text"><div class="table_text"><% tcWebApi_Get("String_Entry", "UserQoS_transferred", "s") %></div></a></th>
													<th width="12%"><% tcWebApi_Get("String_Entry", "BM_UserList4", "s") %></th>
											</tr>											
										</table>          					
          					
          					<div id="qos_rulelist_Block"></div>
          				</td>
        			</tr>        			
      			</table>
      		</td>  
      	</tr>
		</table>
	</div>
		<!--===================================End of Main Content===========================================-->
		</td>
		
	</tr>
</table>
</form>
<div id="footer"></div>
</body>
</html>
<!--QoS_EZQoS.asp-->
