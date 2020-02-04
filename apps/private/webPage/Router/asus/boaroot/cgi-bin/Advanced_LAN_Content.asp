<%
If Request_Form("lanFlag") <> "" Then
  TCWebApi_set("Lan_Entry","IP","uiViewIPAddr")
  TCWebApi_set("Lan_Entry","netmask","uiViewNetMask")

  If Request_Form("dhcpFlag") ="0" Then
  	If Request_Form("dhcppoolFlag") ="1" Then
	    	TCWebApi_set("Dhcpd_Common","start","dhcp_start")
	    	TCWebApi_set("Dhcpd_Common","end","dhcp_end")
  	End If		
		tcWebApi_Save()
		tcWebApi_CommitWithoutSave("Lan")
  	If Request_Form("dhcppoolFlag") ="1" Then
		tcWebApi_CommitWithoutSave("Dhcpd")
  		update_variables()
  	End If		
  	tcWebApi_CommitWithoutSave("Wan")
  End If
  
	tcWebApi_commit("Route")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_LAN_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - LAN IP</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
wanip_obj = '<%tcWebApi_staticGet("DeviceInfo_PVC","WanIP","s")%>';
wannm_obj= '<%tcWebApi_staticGet("DeviceInfo_PVC","WanSubMask","s")%>';
var lan_proto = '<%tcWebApi_staticGet("Lan_Dhcp","type","s")%>';        //1: dhcp enable 0: dhcp disabled
var extend_sec = (productid == "DSL-AC52U" || productid == "DSL-AC55U")?5:0;

var origin_lan_ip = '<%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%>';

function initial(){	
	final_flag = 1; // for the function in general.js
	show_menu();
	showLANIPList();
}

function showLANIPList(){	
	if(clientList.length == 0){
		setTimeout(function() {
			genClientList();			
			showLANIPList();
		}, 500);
		return false;
	}
}	

function applyRule(){
	if(validForm()){
		showLoading(28+extend_sec);
		if(lan_proto == '1'){   //1: "value", dhcp enable
			document.getElementById('drword').innerHTML = "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>.<%tcWebApi_get("String_Entry","LANConfig_ChangedLANIP","s")%>";
		}
		else{
			document.getElementById('drword').innerHTML = "<%tcWebApi_get("String_Entry","LAN_IP_changed_suggedtion1","s")%>"+document.uiViewLanForm.lan_ipaddr.value+"<%tcWebApi_get("String_Entry","LAN_IP_changed_suggedtion2","s")%><br/>";
		}
		setTimeout("hideLoading();", 28100+extend_sec*1000);
		setTimeout("dr_advise();", 28100+extend_sec*1000);
		setTimeout("redirect();", 31000+extend_sec*1000);
		originData.onlinelist_cache = "";
		document.uiViewLanForm.action_script.value = "restart_boa";
		document.uiViewLanForm.action_mode.value = "apply";
		document.uiViewLanForm.submit();
	}
}

function redirect(){
	document.location.href = "http://"+document.uiViewLanForm.lan_ipaddr.value+"/cgi-bin/Advanced_LAN_Content.asp";
}

function valid_IP(obj_name, obj_flag){
var A_class_start = inet_network("1.0.0.0");
var A_class_end = inet_network("126.255.255.255");
var B_class_start = inet_network("127.0.0.0");
var B_class_end = inet_network("127.255.255.255");
var C_class_start = inet_network("128.0.0.0");
var C_class_end = inet_network("255.255.255.255");
var ip_obj = obj_name;
var ip_num = inet_network(ip_obj.value);
if(obj_flag == "DNS" && ip_num == -1){ //DNS allows to input nothing
return true;
}
if(obj_flag == "GW" && ip_num == -1){ //GW allows to input nothing
return true;
}
if(ip_num > A_class_start && ip_num < A_class_end)
return true;
else if(ip_num > B_class_start && ip_num < B_class_end){
			alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
ip_obj.focus();
ip_obj.select();
return false;
}
else if(ip_num > C_class_start && ip_num < C_class_end)
return true;
else{
			alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
ip_obj.focus();
ip_obj.select();
return false;
}
}

function validForm(){
	//sw_mode == 1 && 	
	if(wanip_obj != "0.0.0.0" && wanip_obj != "" && wannm_obj != "0.0.0.0" && wannm_obj != ""){
			if(matchSubnet2(wanip_obj, wannm_obj, document.uiViewLanForm.uiViewIPAddr.value, document.uiViewLanForm.uiViewNetMask.value)){
						document.uiViewLanForm.uiViewIPAddr.focus();
						document.uiViewLanForm.uiViewIPAddr.select();
						alert("<%tcWebApi_get("String_Entry","IPC_x_WAN_LAN_conflict","s")%>");
						return false;
			}						
	}	
	var ip_obj = document.getElementById('lan_ipaddr');
	var ip_num = inet_network(ip_obj.value);
	var ip_class = "";
	if(!valid_IP(ip_obj, "")) return false; //LAN IP
	var netmask_obj = document.getElementById('lan_netmask');
	var netmask_num = inet_network(netmask_obj.value);
	var netmask_reverse_num = ~netmask_num;
	var default_netmask = "";
	var wrong_netmask = 0;
	if(netmask_num < 0) wrong_netmask = 1;
	if(ip_class == 'A')
		default_netmask = "255.0.0.0";
	else if(ip_class == 'B')
		default_netmask = "255.255.0.0";
	else
		default_netmask = "255.255.255.0";

	var test_num = netmask_reverse_num;
	while(test_num != 0){
		if((test_num+1)%2 == 0)
			test_num = (test_num+1)/2-1;
		else{
			wrong_netmask = 1;
			break;
		}
	}
	if(wrong_netmask == 1){
		alert(netmask_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
		netmask_obj.value = default_netmask;
		netmask_obj.focus();
		netmask_obj.select();
		return false;
	}
	var subnet_head = getSubnet(ip_obj.value, netmask_obj.value, "head");
	var subnet_end = getSubnet(ip_obj.value, netmask_obj.value, "end");
	if(ip_num == subnet_head || ip_num == subnet_end){
		alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
		ip_obj.focus();
		ip_obj.select();
		return false;
	}
	
	// check IP pool changed or not
  // No matter it changes or not, it will submit the form
  //Viz modify 2011.10 for DHCP pool issue {
	if(sw_mode == "1"){
		var pool_change = changed_DHCP_IP_pool();
		if(!pool_change)
			return false;
		/*else{
			document.form.lan_ipaddr_rt.value = document.getElementById('lan_ipaddr').value;
			document.form.lan_netmask_rt.value = document.getElementById('lan_netmask').value;			
		}*/
	}	
	//}Viz modify 2011.10 for DHCP pool issue	
	
	

	return true;
}

function done_validating(action){
	refreshpage();
}

function changed_DHCP_IP_pool(){	
	if(document.uiViewLanForm.lan_ipaddr.value != origin_lan_ip){ // IP changed

		if(!matchSubnet(document.uiViewLanForm.lan_ipaddr.value, document.uiViewLanForm.dhcp_start.value, 3) ||
		!matchSubnet(document.uiViewLanForm.lan_ipaddr.value, document.uiViewLanForm.dhcp_end.value, 3)){ // Different Subnet
		document.uiViewLanForm.dhcp_start.value = subnetPrefix(document.uiViewLanForm.lan_ipaddr.value, document.uiViewLanForm.dhcp_start.value, 3);
		document.uiViewLanForm.dhcp_end.value = subnetPrefix(document.uiViewLanForm.lan_ipaddr.value, document.uiViewLanForm.dhcp_end.value, 3);
		}
	}
	var post_lan_netmask='';
	var pool_start='';
	var pool_end='';
	var nm = new Array("0", "128", "192", "224", "240", "248", "252");
	z=0;
	tmp_ip=0;
	for(i=0;i<document.uiViewLanForm.lan_ipaddr.value.length;i++){
	if (document.uiViewLanForm.lan_ipaddr.value.charAt(i) == '.') z++;
	if (z==3){ tmp_ip=i+1; break;}
	}
	post_lan_ipaddr = document.uiViewLanForm.lan_ipaddr.value.substr(tmp_ip,3);
	c=0;
	tmp_nm=0;
	for(i=0;i<document.uiViewLanForm.lan_netmask.value.length;i++){
	if (document.uiViewLanForm.lan_netmask.value.charAt(i) == '.') c++;
	if (c==3){ tmp_nm=i+1; break;}
	}
	post_lan_netmask = document.uiViewLanForm.lan_netmask.value.substr(tmp_nm,3);
	for(i=0;i<nm.length;i++){
	if(post_lan_netmask==nm[i]){
	gap=256-Number(nm[i]);
	subnet_set = 256/gap;
	for(j=1;j<=subnet_set;j++){
		if(post_lan_ipaddr < j*gap && post_lan_ipaddr == (j-1)*gap+1){	//Viz add to avoid default (1st) LAN ip in DHCP pool
			pool_start=(j-1)*gap+2;
			pool_end=j*gap-2;
			break;
		}
		else if(post_lan_ipaddr < j*gap && post_lan_ipaddr == j*gap-2){    //Viz add to avoid default (last) LAN ip in DHCP pool
			pool_start=(j-1)*gap+1;
			pool_end=j*gap-3;
			break;
		}
		else if(post_lan_ipaddr < j*gap){
			pool_start=(j-1)*gap+1;
			pool_end=j*gap-2;
			break;						
		}
	}
	break;
	}
	}
	var update_pool_start = subnetPostfix(document.uiViewLanForm.dhcp_start.value, pool_start, 3);
	var update_pool_end = subnetPostfix(document.uiViewLanForm.dhcp_end.value, pool_end, 3);
	/*alert("hook_start: <% tcWebApi_get("Dhcpd_Common","start","s") %>, change_start:"+update_pool_start+" , hook_end: <% tcWebApi_get("Dhcpd_Common","end","s") %> , change_end:"+update_pool_end);	*/
	if(("<% tcWebApi_get("Dhcpd_Common","start","s") %>" != update_pool_start) || ("<% tcWebApi_get("Dhcpd_Common","end","s") %>" != update_pool_end)){	
	if(confirm("<%tcWebApi_get("String_Entry","JS_DHCP1","s")%>")){
	document.uiViewLanForm.dhcp_start.value = update_pool_start;
	document.uiViewLanForm.dhcp_end.value = update_pool_end;
	document.uiViewLanForm.dhcppoolFlag.value = 1;
	}else{
		return false;
	}
	}
	return true;
}

function check_vpn(){ //true: lAN ip & VPN client ip conflict
	var lan_ip_subnet = origin_lan_ip.split(".")[0]+"."+origin_lan_ip.split(".")[1]+"."+origin_lan_ip.split(".")[2]+".";
	var lan_ip_end = parseInt(origin_lan_ip.split(".")[3]);

	if(lan_ip_subnet == pptpd_clients_subnet && lan_ip_end >= pptpd_clients_start_ip && lan_ip_end <= pptpd_clients_end_ip){
		return true;
	}else{
		return false;
	}
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="hiddenMask" class="popup_bg">
<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
<tr>
<td>
			<div class="drword" id="drword" style="height:110px;"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
<br/>
<br/>
</div>
<div class="drImg"><img src="/images/alertImg.png"></div>
<div style="height:70px;"></div>
</td>
</tr>
</table>
</div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value=""></form>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_LAN_Content.asp" name="uiViewLanForm" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<INPUT TYPE="HIDDEN" NAME="dhcppoolFlag" VALUE="0">
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
<td align="left" valign="top">
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_2","s")%> - <%tcWebApi_get("String_Entry","menu5_2_1","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","LHC_display1_sd","s")%></div>
<div id="VPN_conflict" class="formfontdesc" style="color:#FFCC00;display:none;"><%tcWebApi_get("String_Entry","LHC_display1_sd2","s")%></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
<th width="30%">
			  <a class="hintstyle" href="javascript:void(0);" onClick="openHint(4,1);"><%tcWebApi_get("String_Entry","IPC_ExternalIPAddress_in","s")%></a>
</th>
<td>
<input type="text" name="uiViewIPAddr" id="lan_ipaddr" class="input_15_table" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%>" onKeyPress="return validator.isIPAddr(this, event);">
<INPUT TYPE="HIDDEN" NAME="dhcpFlag" VALUE="0"><INPUT TYPE="HIDDEN" NAME="lanFlag" VALUE="0">
</td>
</tr>
<tr>
<th>
			  <a class="hintstyle"  href="javascript:void(0);" onClick="openHint(4,2);"><%tcWebApi_get("String_Entry","IPC_x_ExternalSubnetMask_in","s")%></a>
</th>
<td>
<input type="text" name="uiViewNetMask" id="lan_netmask" class="input_15_table" SIZE="15" MAXLENGTH="15" VALUE="<%If tcWebApi_get("Lan_Entry","netmask","h") <> "" then tcWebApi_get("Lan_Entry","netmask","s") end if%>" onkeypress="return validator.isIPAddr(this, event);">
<input type="hidden" name="dhcp_start" value="<% tcWebApi_get("Dhcpd_Common","start","s") %>">
<input type="hidden" name="dhcp_end" value="<% tcWebApi_get("Dhcpd_Common","end","s") %>">
</td>
</tr>
</table>
<div class="apply_gen">
			<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
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
</body>

<!--Advanced_LAN_Content.asp-->
</html>

