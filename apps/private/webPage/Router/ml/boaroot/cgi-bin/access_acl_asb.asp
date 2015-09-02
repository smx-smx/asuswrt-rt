<%
/*add a rule*/
if request_Form("ACLActionFlag") = "0" then
	TCWebAPI_set("WebCurSet_Entry", "acl_id", "SRVIndexSEL")
	TCWebAPI_set("ACL_Entry", "Direction", "Direction")
elseif request_Form("ACLActionFlag") = "1" then
	TCWebAPI_set("WebCurSet_Entry", "acl_id", "SRVIndexSEL")
elseif request_Form("ACLActionFlag") = "2" then
	TCWebAPI_set("WebCurSet_Entry", "acl_id", "SRVIndexSEL")
	TCWebAPI_set("WebCurSet_Entry", "account_id", "ACCIndexSEL")
elseif request_Form("ACLActionFlag") = "3" then
	TCWebAPI_set("ACL_Common", "Active", "Activate")
	TCWebAPI_set("WebCurSet_Entry", "acl_id", "SRVIndexSEL")
	TCWebAPI_set("ACL_Entry", "Direction", "Direction")
	TCWebAPI_set("WebCurSet_Entry", "account_id", "ACCIndexSEL")
	
	if TCWebAPI_get("ACL_Entry", "Direction", "h") = "LAN" then
		TCWebAPI_set("ACL_Entry", "lan_port", "SRVPORT")
		TCWebAPI_set("ACL_Entry", "lan_active", "SRVACTIVE")
		TCWebAPI_set("ACL_Entry", "lan_start_ip", "SRVALLOWEDSTARTIP")
		TCWebAPI_set("ACL_Entry", "lan_end_ip", "SRVALLOWEDENDIP")
		TCWebAPI_set("ACL_Entry", "lan_submask", "SRVSUBMASK")
		
		TCWebAPI_set("ACL_Entry", "lan_port_auth_user2", "allowed_flag0")
		TCWebAPI_set("ACL_Entry", "lan_port_auth_user3", "allowed_flag1")

		TCWebAPI_set("ACL_Entry_Entry", "lan_allowed", "ACCALLOWED")
		TCWebAPI_set("ACL_Entry_Entry", "lan_start_ip", "ACCALLOWEDSTARTIP")
		TCWebAPI_set("ACL_Entry_Entry", "lan_end_ip", "ACCALLOWEDENDIP")
		'TCWebAPI_set("ACL_Entry_Entry", "lan_submask", "ACCSUBMASK")
		
		TCWebAPI_set("ACL_Entry_Entry", "lan_allowed_auth_user2", "allowed_flag2")
		TCWebAPI_set("ACL_Entry_Entry", "lan_ip_auth_user2", "allowed_flag4")
		TCWebAPI_set("ACL_Entry_Entry", "lan_allowed_auth_user3", "allowed_flag3")
		TCWebAPI_set("ACL_Entry_Entry", "lan_ip_auth_user3", "allowed_flag5")
	else
		TCWebAPI_set("ACL_Entry", "wan_port", "SRVPORT")
		TCWebAPI_set("ACL_Entry", "wan_active", "SRVACTIVE")
		TCWebAPI_set("ACL_Entry", "wan_start_ip", "SRVALLOWEDSTARTIP")
		TCWebAPI_set("ACL_Entry", "wan_end_ip", "SRVALLOWEDENDIP")
		TCWebAPI_set("ACL_Entry", "wan_submask", "SRVSUBMASK")
		
		TCWebAPI_set("ACL_Entry", "wan_port_auth_user2", "allowed_flag0")
		TCWebAPI_set("ACL_Entry", "wan_port_auth_user3", "allowed_flag1")

		TCWebAPI_set("ACL_Entry_Entry", "wan_allowed", "ACCALLOWED")
		TCWebAPI_set("ACL_Entry_Entry", "wan_start_ip", "ACCALLOWEDSTARTIP")
		TCWebAPI_set("ACL_Entry_Entry", "wan_end_ip", "ACCALLOWEDENDIP")
		'TCWebAPI_set("ACL_Entry_Entry", "wan_submask", "ACCSUBMASK")
		
		TCWebAPI_set("ACL_Entry_Entry", "wan_allowed_auth_user2", "allowed_flag2")
		TCWebAPI_set("ACL_Entry_Entry", "wan_ip_auth_user2", "allowed_flag4")
		TCWebAPI_set("ACL_Entry_Entry", "wan_allowed_auth_user3", "allowed_flag3")
		TCWebAPI_set("ACL_Entry_Entry", "wan_ip_auth_user3", "allowed_flag5")
	end if

		TCWebAPI_Commit("ACL_Entry")
end if
%>

<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" src="/val.js"></script>
</head>
<!--<script language="javascript" src="/tc2func.js"></script>-->
<script language="JavaScript">
var	service_flag = 0;
<% if request_Form("ACLActionFlag") = "1" then %>
service_flag = 1;
<% end if %>

var cur_server = "<%TCWebAPI_get("Info_Service", "cur_serv", "s")%>";
//common info
function stACLCommonInfo(lanport, wanport, lanactive, wanactive, lansip, wansip, laneip, waneip, lansub, wansub, lanportuser2, wanportuser2, lanportuser3, wanportuser3){
	if ("N/A" != lanport)
		this.lanport = lanport;
	else
		this.lanport = "";
	if ("N/A" != wanport)
		this.wanport = wanport;
	else
		this.wanport = "";
	this.lanactive = lanactive;
	this.wanactive = wanactive;
	if ("N/A" != lansip)
		this.lansip = lansip;
	else
		this.lansip = "0.0.0.0";
	if ("N/A" != wansip)
		this.wansip = wansip;
	else
		this.wansip = "0.0.0.0";
	if ("N/A" != laneip)
		this.laneip = laneip;
	else
		this.laneip = "0.0.0.0";
	if ("N/A" != waneip)
		this.waneip = waneip;
	else
		this.waneip = "0.0.0.0";
	if ("N/A" != lansub)
		this.lansub = lansub;
	else
		this.lansub = "0.0.0.0";
	if ("N/A" != wansub)
		this.wansub = wansub;
	else
		this.wansub = "0.0.0.0";
		
	this.lanportuser2 = lanportuser2;
	this.wanportuser2 = wanportuser2;
	this.lanportuser3 = lanportuser3;
	this.wanportuser3 = wanportuser3;
}
//each account
function stforEachAccount(lan_allowed, wan_allowed, allow_lan_start_ip, allow_wan_start_ip, allow_lan_end_ip, allow_wan_end_ip, lan_allowed_auth_user2, wan_allowed_auth_user2, lan_allowed_auth_user3, wan_allowed_auth_user3, lan_ip_auth_user2, wan_ip_auth_user2, lan_ip_auth_user3, wan_ip_auth_user3)
{
	this.lan_allowed = lan_allowed;
	this.wan_allowed = wan_allowed;
	if ("N/A" != allow_lan_start_ip)
		this.allow_lan_start_ip = allow_lan_start_ip;
	else
		this.allow_lan_start_ip = "0.0.0.0";
	if ("N/A" != allow_wan_start_ip)
		this.allow_wan_start_ip = allow_wan_start_ip;
	else
		this.allow_wan_start_ip = "0.0.0.0";
	if ("N/A" != allow_lan_end_ip)
		this.allow_lan_end_ip = allow_lan_end_ip;
	else
		this.allow_lan_end_ip = "0.0.0.0";
	if ("N/A" != allow_wan_end_ip)
		this.allow_wan_end_ip = allow_wan_end_ip;
	else
		this.allow_wan_end_ip = "0.0.0.0";
		
	this.lan_allowed_auth_user2 = lan_allowed_auth_user2;
	this.wan_allowed_auth_user2 = wan_allowed_auth_user2;
	this.lan_allowed_auth_user3 = lan_allowed_auth_user3;
	this.wan_allowed_auth_user3 = wan_allowed_auth_user3;
	this.lan_ip_auth_user2 = lan_ip_auth_user2;
	this.wan_ip_auth_user2 = wan_ip_auth_user2;
	this.lan_ip_auth_user3 = lan_ip_auth_user3;
	this.wan_ip_auth_user3 = wan_ip_auth_user3;
}
var ACLCommInfo = new stACLCommonInfo("<% TCWebAPI_get("ACL_Entry", "lan_port", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "wan_port", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "lan_active", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "wan_active", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "lan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "wan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "lan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "wan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "lan_submask", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "wan_submask", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "lan_port_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "wan_port_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "lan_port_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry", "wan_port_auth_user3", "s") %>");
							
var ACLEachAccount = new Array(new stforEachAccount("<% TCWebAPI_get("ACL_Entry_Entry0", "lan_allowed", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "wan_allowed", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "lan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "wan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "lan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "wan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "lan_allowed_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "wan_allowed_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "lan_allowed_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "wan_allowed_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "lan_ip_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "wan_ip_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "lan_ip_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry0", "wan_ip_auth_user3", "s") %>"),
							new stforEachAccount("<% TCWebAPI_get("ACL_Entry_Entry1", "lan_allowed", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "wan_allowed", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "lan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "wan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "lan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "wan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "lan_allowed_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "wan_allowed_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "lan_allowed_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "wan_allowed_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "lan_ip_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "wan_ip_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "lan_ip_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry1", "wan_ip_auth_user3", "s") %>"),
							new stforEachAccount("<% TCWebAPI_get("ACL_Entry_Entry2", "lan_allowed", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "wan_allowed", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "lan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "wan_start_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "lan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "wan_end_ip", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "lan_allowed_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "wan_allowed_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "lan_allowed_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "wan_allowed_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "lan_ip_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "wan_ip_auth_user2", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "lan_ip_auth_user3", "s") %>",
							"<% TCWebAPI_get("ACL_Entry_Entry2", "wan_ip_auth_user3", "s") %>"), null);

function accesscheck(nStyle, nIndex){
	var disabled_flag = new Array(false, false);
	with (document.ACL_form){
		if (CurAccount.value == "1") {
			switch (nStyle) {
				case 0://lan
						if ("Yes" != ACLEachAccount[nIndex].lan_allowed_auth_user2){
							disabled_flag[0] = true;
						}
						
						if ("Yes" != ACLEachAccount[nIndex].lan_ip_auth_user2){
							disabled_flag[1] = true;
						}
				break;
				case 1://wan
						if ("Yes" != ACLEachAccount[nIndex].wan_allowed_auth_user2){
							disabled_flag[0] = true;
						}
						
						if ("Yes" != ACLEachAccount[nIndex].wan_ip_auth_user2){
							disabled_flag[1] = true;
						}
				break;
				default:break;
			}
			ACCALLOWED[0].disabled = disabled_flag[0];
			ACCALLOWED[1].disabled = disabled_flag[0];
			ACCALLOWEDSTARTIP.disabled = disabled_flag[1];
			ACCALLOWEDENDIP.disabled = disabled_flag[1];
		}
	}
}
function LoadPageinner(){
	with (document.ACL_form){
		var strtemp = Direction.value;
		var accIndex = parseInt(ACCIndexSEL.value);
		if (strtemp == "LAN"){
				SRVPORT.value = ACLCommInfo.lanport;
				if ("Yes" == ACLCommInfo.lanactive)
					SRVACTIVE[0].checked = true;
				else
					SRVACTIVE[1].checked = true;
				SRVALLOWEDSTARTIP.value = ACLCommInfo.lansip;
				SRVALLOWEDENDIP.value = ACLCommInfo.laneip;
				SRVSUBMASK.value = ACLCommInfo.lansub;
				if ("Yes" == ACLCommInfo.lanportuser2)
					SRVAUTHUSER2.checked = true;
				if ("Yes" == ACLCommInfo.lanportuser3)
					SRVAUTHUSER3.checked = true;
					
				if ("Yes" == ACLEachAccount[accIndex].lan_allowed)
					ACCALLOWED[0].checked = true;
				else
					ACCALLOWED[1].checked = true;
					
				ACCALLOWEDSTARTIP.value = ACLEachAccount[accIndex].allow_lan_start_ip;
				ACCALLOWEDENDIP.value = ACLEachAccount[accIndex].allow_lan_end_ip;
				
				if ("Yes" == ACLEachAccount[accIndex].lan_allowed_auth_user2)
					ACCAUTHALLOWEDUSER2.checked = true;
				else
					ACCAUTHALLOWEDUSER2.checked = false;
				if ("Yes" == ACLEachAccount[accIndex].lan_allowed_auth_user3){
					ACCAUTHALLOWEDUSER3.checked = true;
				}
				else{
					ACCAUTHALLOWEDUSER3.checked = false;
				}
				
				if ("Yes" == ACLEachAccount[accIndex].lan_ip_auth_user2)
					ACCAUTHALLOWEDIPUSER2.checked = true;
				else {
					ACCAUTHALLOWEDIPUSER2.checked = false;
				}
				if ("Yes" == ACLEachAccount[accIndex].lan_ip_auth_user3){
					ACCAUTHALLOWEDIPUSER3.checked = true;
				}
				else {
					ACCAUTHALLOWEDIPUSER3.checked = false;
				}
				accesscheck(0, accIndex);
		}
		else {
			SRVPORT.value = ACLCommInfo.wanport;
				if ("Yes" == ACLCommInfo.wanactive)
					SRVACTIVE[0].checked = true;
				else
					SRVACTIVE[1].checked = true;
				SRVALLOWEDSTARTIP.value = ACLCommInfo.wansip;
				SRVALLOWEDENDIP.value = ACLCommInfo.waneip;
				SRVSUBMASK.value = ACLCommInfo.wansub;
				if ("Yes" == ACLCommInfo.wanportuser2)
					SRVAUTHUSER2.checked = true;
				if ("Yes" == ACLCommInfo.wanportuser3)
					SRVAUTHUSER3.checked = true;
					
				if ("Yes" == ACLEachAccount[accIndex].wan_allowed)
					ACCALLOWED[0].checked = true;
				else
					ACCALLOWED[1].checked = true;
					
				ACCALLOWEDSTARTIP.value = ACLEachAccount[accIndex].allow_wan_start_ip;
				ACCALLOWEDENDIP.value = ACLEachAccount[accIndex].allow_wan_end_ip;
				
				if ("Yes" == ACLEachAccount[accIndex].wan_allowed_auth_user2)
					ACCAUTHALLOWEDUSER2.checked = true;
				else
					ACCAUTHALLOWEDUSER2.checked = false;
					
				if ("Yes" == ACLEachAccount[accIndex].wan_allowed_auth_user3){
					ACCAUTHALLOWEDUSER3.checked = true;
				}
				else{
					ACCAUTHALLOWEDUSER3.checked = false;
				}
				
				if ("Yes" == ACLEachAccount[accIndex].wan_ip_auth_user2)
					ACCAUTHALLOWEDIPUSER2.checked = true;
				else {
					ACCAUTHALLOWEDIPUSER2.checked = false;
				}
				
				if ("Yes" == ACLEachAccount[accIndex].wan_ip_auth_user3){
					ACCAUTHALLOWEDIPUSER3.checked = true;
				}
				else {
					ACCAUTHALLOWEDIPUSER3.checked = false;
				}
				accesscheck(1, accIndex);
		}
	}
}
function pageDisplay()
{
	with (document.ACL_form) {
		setDisplay('div_curserv', 0);
		setDisplay('div_curacc0', 0);
		setDisplay('srvport_info', 0);
		setDisplay('div_authset', 0);
		
		if (SRVIndexSEL.value != "6")
			setDisplay('srvport_info', 1);
		if ((CurAccount.value != "2") && (SRVIndexSEL.value != "6")){
			setDisplay('div_authset', 1);
			if (CurAccount.value == "0")
				setDisplay('div_svrport', 1);
			else
				setDisplay('div_svrport', 0);
		}
		if (cur_server == "hasacc") {
			setDisplay('div_curserv', 1);
			if ((CurAccount.value == "0") && (ACCIndexSEL.value != "0")){
				setDisplay('div_curacc0', 1);
				if (ACCIndexSEL.value == "2"){
					setDisplay('allowed_acc_id2', 1);
					setDisplay('allowedip_acc_id2', 1);
				}
				else {
					setDisplay('allowed_acc_id2', 0);
					setDisplay('allowedip_acc_id2', 0);
				}
			}
			else if((CurAccount.value == "1") && (ACCIndexSEL.value == "2")){
				setDisplay('div_curacc0', 1);
				setDisplay('allowed_acc_id1', 0);
				setDisplay('allowedip_acc_id1', 0);
				
				ACCAUTHALLOWEDUSER3.disabled = true;
				ACCAUTHALLOWEDIPUSER3.disabled = true;
			}
		}
	}
}
function checktheCheckboxs(){
	with (document.ACL_form) {
		if (SRVAUTHUSER2.checked)
			allowed_flag0.value = "Yes";
		if (SRVAUTHUSER3.checked)
			allowed_flag1.value = "Yes";
		if (ACCAUTHALLOWEDUSER2.checked)
			allowed_flag2.value = "Yes";
		if (ACCAUTHALLOWEDUSER3.checked)
			allowed_flag3.value = "Yes";
		if (ACCAUTHALLOWEDIPUSER2.checked)
			allowed_flag4.value = "Yes";
		if (ACCAUTHALLOWEDIPUSER3.checked)
			allowed_flag5.value = "Yes";
	}
}
	
function init()
{
	LoadPageinner();
	if (document.ACL_form.Activate[1].checked) {
		aclSwitch(0);
	}
	else {
		aclSwitch(1);
	}
	pageDisplay();
}

function aclSwitch(on_off)
{
	var form;
	form = document.ACL_form;
	var cur_sev_index = form.SRVIndexSEL.value;
	var cur_account = form.ACCIndexSEL.value;
	
	if (on_off == 0) {
		value = true;
	}
	else {
		value = false;
	}

	/* basic setting */
	form.SRVIndexSEL.disabled = value;
	form.Direction.disabled = value;

	/* 6 is ping service */
	if (cur_sev_index != "6") {
		form.SRVPORT.disabled = value;

		/* serve port may not edit by user2/user3 */
		if (form.CurAccount.value == 1) {
			if (form.Direction.value == "LAN") {
				if ("<%tcWebApi_get("ACL_Entry", "lan_port_auth_user2", "s")%>" != "Yes") {
					form.SRVPORT.disabled = true;
				}
			}
			else if (form.Direction.value == "WAN") {
				if ("<%tcWebApi_get("ACL_Entry", "wan_port_auth_user2", "s")%>" != "Yes") {
					form.SRVPORT.disabled = true;
				}
			}
		}
		else if (form.CurAccount.value == 2) {
			if (form.Direction.value == "LAN") {
				if ("<%tcWebApi_get("ACL_Entry", "lan_port_auth_user3", "s")%>" != "Yes") {
					form.SRVPORT.disabled = true;
				}
			}
			else if (form.Direction.value == "WAN") {
				if ("<%tcWebApi_get("ACL_Entry", "wan_port_auth_user3", "s")%>" != "Yes") {
					form.SRVPORT.disabled = true;
				}
			}
		}
	}

	if (form.CurAccount.value == 0) {
		form.SRVACTIVE[0].disabled = value;
		form.SRVACTIVE[1].disabled = value;
		form.SRVALLOWEDSTARTIP.disabled = value;
		form.SRVALLOWEDENDIP.disabled = value;
		form.SRVSUBMASK.disabled = value;
	}
	else {
		form.Activate[0].disabled = true;
		form.Activate[1].disabled = true;
		form.SRVACTIVE[0].disabled = true;
		form.SRVACTIVE[1].disabled = true;
		form.SRVALLOWEDSTARTIP.disabled = true;
		form.SRVALLOWEDENDIP.disabled = true;
		form.SRVSUBMASK.disabled = true;
	}

	if (cur_sev_index != 6) {
		if (form.CurAccount.value == 0) {
			form.SRVAUTHUSER2.disabled = value;
			form.SRVAUTHUSER3.disabled = value;
		}
		else if (form.CurAccount.value == 1) {
			form.SRVAUTHUSER3.disabled = true;
		}
	}

	if (cur_server == "hasacc") {
		/* account setting */
		form.ACCIndexSEL.disabled = value;
		form.ACCALLOWED[0].disabled = value;
		form.ACCALLOWED[1].disabled = value;
		
		form.ACCALLOWEDSTARTIP.disabled = value;
		form.ACCALLOWEDENDIP.disabled = value;
		//form.ACCSUBMASK.disabled = value;

		if (form.CurAccount.value == 1) {
			if (form.Direction.value == "LAN") {
				if ("<%tcWebApi_get("ACL_Entry_Entry", "lan_allowed_auth_user2", "s")%>" != "Yes") {
					form.ACCALLOWED[0].disabled = true;
					form.ACCALLOWED[1].disabled = true;
				}

				if ("<%tcWebApi_get("ACL_Entry_Entry", "lan_ip_auth_user2", "s")%>" != "Yes") {
					form.ACCALLOWEDSTARTIP.disabled = true;
					form.ACCALLOWEDENDIP.disabled = true;
					//form.ACCSUBMASK.disabled = true;
				}
			}
			else {
				if ("<%tcWebApi_get("ACL_Entry_Entry", "wan_allowed_auth_user2", "s")%>" != "Yes") {
					form.ACCALLOWED[0].disabled = true;
					form.ACCALLOWED[1].disabled = true;
				}

				if ("<%tcWebApi_get("ACL_Entry_Entry", "wan_ip_auth_user2", "s")%>" != "Yes") {
					form.ACCALLOWEDSTARTIP.disabled = true;
					form.ACCALLOWEDENDIP.disabled = true;
					//form.ACCSUBMASK.disabled = true;
				}
			}
		}
		else if (form.CurAccount.value == 2) {
			if (form.Direction.value == "LAN") {
				if ("<%tcWebApi_get("ACL_Entry_Entry", "lan_allowed_auth_user3", "s")%>" != "Yes") {
					form.ACCALLOWED[0].disabled = true;
					form.ACCALLOWED[1].disabled = true;
				}

				if ("<%tcWebApi_get("ACL_Entry_Entry", "lan_ip_auth_user3", "s")%>" != "Yes") {
					form.ACCALLOWEDSTARTIP.disabled = true;
					form.ACCALLOWEDENDIP.disabled = true;
					//form.ACCSUBMASK.disabled = true;
				}
			}
			else {
				if ("<%tcWebApi_get("ACL_Entry_Entry", "wan_allowed_auth_user3", "s")%>" != "Yes") {
					form.ACCALLOWED[0].disabled = true;
					form.ACCALLOWED[1].disabled = true;
				}

				if ("<%tcWebApi_get("ACL_Entry_Entry", "wan_ip_auth_user3", "s")%>" != "Yes") {
					form.ACCALLOWEDSTARTIP.disabled = true;
					form.ACCALLOWEDENDIP.disabled = true;
					//form.ACCSUBMASK.disabled = true;
				}
			}
		}
		
		if (form.CurAccount.value == 0) {
			/* current user is admin */
			if (cur_account == "1") {
				/* select user2 */
				form.ACCAUTHALLOWEDUSER2.disabled = value;
				form.ACCAUTHALLOWEDIPUSER2.disabled = value;
			}
			
			if (cur_account == "2") {
				/* select user3 */
				form.ACCAUTHALLOWEDUSER2.disabled = value;
				form.ACCAUTHALLOWEDIPUSER2.disabled = value;
				form.ACCAUTHALLOWEDUSER3.disabled = value;
				form.ACCAUTHALLOWEDIPUSER3.disabled = value;
			}
		}
		else if (form.CurAccount.value == 1) {
			if (cur_account == "2") {
				form.ACCAUTHALLOWEDUSER3.disabled = true;
				form.ACCAUTHALLOWEDIPUSER3.disabled = true;
			}
		}
	}
	
}

function aclRuleSwitch(on_off)
{
	var value;
	form = document.ACL_form;

	if(on_off == 0){
		value = true;
	}else{
		value = false;
	}
	form.ScrIPaddrBeginTXT.disabled = value;
	form.ScrIPaddrEndTXT.disabled = value;
	form.ApplicationSEL.disabled = value;
	form.InterfaceSEL.disabled = value;
}

function directSwitch()
{
	//document.forms[0].ACLActionFlag.value = 0;
	//document.forms[0].submit();
	LoadPageinner();
	return;
}

function doSRVIndexChange() 
{
	document.forms[0].ACLActionFlag.value = 1;
	document.forms[0].submit();
	return;
}

function doAccIndexChange()
{
	//document.forms[0].ACLActionFlag.value = 2;
	//document.forms[0].submit();
	LoadPageinner();
	pageDisplay();
	return;
	
}

function doAdd() 
{
	if (verifyForm()) {
	
		document.forms[0].ACLActionFlag.value = 3;
		document.forms[0].submit();
		return;
	}
}            

function unValidMask(Mask)
{
	var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var bMask = 0;
	var watch = false;
	var i;
  
	var test=0;
	var point=-1;
	while(test!=-1)
	{
		point++;
		test=Mask.indexOf(".",test+1);	
	}
	
	if(point<3)
	{
		alert("<%tcWebApi_get("String_Entry","ACLASBJS0Text","s")%>");
		return true;
	}

	if(mask == null)
	{ 
		alert("<%tcWebApi_get("String_Entry","ACLASBJS0Text","s")%>");
		return true;
	}

	digits = mask[0].split(".");
	for(i=0; i < 4; i++)
	{
		if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
		{
			alert("<%tcWebApi_get("String_Entry","ACLASBJS0Text","s")%>");
			return true;
		}
		bMask = (bMask << 8) | Number(digits[i]);
	}
	if ((Number(digits[0]) == 0) && (Number(digits[1]) == 0)
		&& (Number(digits[2]) == 0) && (Number(digits[3]) == 0))
	{
		alert("<%tcWebApi_get("String_Entry","ACLASBJS0Text","s")%>");
		return true;	
	}
	bMask = bMask & 0x0FFFFFFFF;
	
	for(i=0; i<32; i++)
	{
		if((watch==true) && ((bMask & 0x1)==0))
		{
			alert("<%tcWebApi_get("String_Entry","ACLASBJS0Text","s")%>");
			return true;
		}
		bMask = bMask >> 1;
		if((bMask & 0x01) == 1)
		{
			watch=true;
		}
	}
	return false;
}

function port_check()
{
	var cur_sev_index = document.ACL_form.SRVIndexSEL.value;

	
	var vs_port = document.ACL_form.VSPort.value;
	var lan_ports = document.ACL_form.LanPorts.value;
	var wan_ports = document.ACL_form.WanPorts.value;
	
	var port_array, i;

	/* it is ping service, just return */
	if (cur_sev_index == 6)
		return true;
		
	var serve_port = parseInt(document.forms[0].SRVPORT.value);
	
	if (document.ACL_form.Activate[0].checked) {
		if (isNaN(serve_port) || serve_port < 0 || serve_port > 65535) {
			alert("<%tcWebApi_get("String_Entry","ACLASBJS2Text","s")%>");
			return false;
		}
	}

	/* acheck virtual server port */
	port_array = vs_port.split(",");

	
	if (document.ACL_form.Activate[0].checked) {
		if (document.ACL_form.SRVACTIVE[0].checked) {
			if (document.ACL_form.Direction.value == "WAN") {
				for (i = 0; i < port_array.length; i++) {
					var port = port_array[i].split("-");

					if (serve_port >= parseInt(port[0]) && serve_port <= parseInt(port[1])) {
						alert("<%tcWebApi_get("String_Entry","ACLASBJS3Text","s")%>");
						return false;
					}
				}
			}
		}
	}
	
	/* check acl port has been used */
	if (document.ACL_form.Direction.value == "WAN") 
		port_array = wan_ports.split(",");
	else
		port_array = lan_ports.split(",");
	
	if (document.ACL_form.Activate[0].checked) {
		if (document.ACL_form.SRVACTIVE[0].checked) {
			for (i = 0; i < port_array.length; i++) {
				
				if (serve_port == parseInt(port_array[i])) {
					alert("<%tcWebApi_get("String_Entry","ACLASBJS4Text","s")%>");
					return false;
				}
			}
		}
	}
	
	return true;
}

function ip_check()
{
	var form = document.ACL_form;

	/* check service allowed ip */
	var addrBegin = form.SRVALLOWEDSTARTIP.value;
	var addrEnd = form.SRVALLOWEDENDIP.value;
	var submask = form.SRVSUBMASK.value;
	var cur_sev_index = form.SRVIndexSEL.value;
	var check_success = 0;
	
	if((addrBegin == "0.0.0.0")&&(addrEnd=="0.0.0.0")){
		check_success = 1;
	}
	else {
		if ((!doValidateIP(addrBegin,0,1,0))||(!doValidateIP(addrEnd,0,1,0))) {
			return false;
		}
		else if (unValidMask(submask)) {
			return false;
		}
		else {
			if (!doValidateRange(addrBegin, addrEnd, submask, 0))
				return false;
		}
	}

	/* tftp / snmp / ping should not check the account ip */
	if (parseInt(cur_sev_index) > 3)
		return true;
		
	/* check account allowed ip */
	var addrBegin = form.ACCALLOWEDSTARTIP.value;
	var addrEnd = form.ACCALLOWEDENDIP.value;
	/* check account allowed ip and service allowed ip in the same subnet */
	if (check_success == 0)
		if (!doValidateRange(form.SRVALLOWEDSTARTIP.value, form.ACCALLOWEDSTARTIP.value, submask, 1)) {
			return false;
		}
	
	if ((addrBegin == "0.0.0.0")&&(addrEnd=="0.0.0.0")) {
		return true;
	}
	else {
		if((!doValidateIP(addrBegin,0,1,0))||(!doValidateIP(addrEnd,0,1,0))){
			return false;
		}
		else if (unValidMask(submask)){
			return false;
		}else{
			return doValidateRange(addrBegin, addrEnd, submask, 0);
		}
	}

	return true;
}

function verifyForm()
{
	/* port check */
	if (!port_check())
		return false;
	
	/* ip check */
	if (!ip_check())
		return false;
	checktheCheckboxs();
	return true;	
}

/*function onClickACLSummary() 
{
	window.open("/cgi-bin/access_acllist.cgi","ACL_List","toolbar=no,menubar=no,scrollbars=yes,height=300, width=520,location=0,left=100,top=100");
	return false;
}*/

function doValidateIP(Address, option1, option2, Where) 
{

	var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var i;

	if(((option1 == 1 || option1 == 4) && Address == "0.0.0.0") || (option1 == 2 && Address == "255.255.255.255"))
		return true;
		
	if(address == null)
	{
		if(option1 == 4 && Address == "")
			return true;
		if(Where == 1){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
			alert("<%tcWebApi_get("String_Entry","ACLJS0Text","s")%>"+Address);
			<!-- RpEnd -->   
		}else if(Where == 2){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
			alert("<%tcWebApi_get("String_Entry","ACLJS1Text","s")%>");
			<!-- RpEnd -->
		}else {
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg0" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg0" RpTextType="ASCII" -->	  	  	         		       
			alert("<%tcWebApi_get("String_Entry","ACLJS2Text","s")%>");
		<!-- RpEnd -->
		}	    	        				         
		return false; 
	}
	
	digits = address[0].split(".");
	
	for(i=0; i < 4; i++)
	{
		if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (option1 != 4 && Number(digits[0]) > 223))
		{ 
			if(Where == 1){
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
				alert("<%tcWebApi_get("String_Entry","ACLJS0Text","s")%>"+Address);
				<!-- RpEnd --> 
			}else if(Where == 2){
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
				alert("<%tcWebApi_get("String_Entry","ACLJS1Text","s")%>");
				<!-- RpEnd -->
			}else{	  
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg1" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg1" RpTextType="ASCII" -->  	               
				alert("<%tcWebApi_get("String_Entry","ACLJS3Text","s")%>" + Address);
				<!-- RpEnd --> 
			}     	
			return false; 
		}
	}

	if((!isValidIpAddr(digits[0],digits[1],digits[2],digits[3])) || (option1 == 3 && Address == "1.0.0.0") || (option2 == 1 && digits[3] == 0))
	{
		if(Where == 1)
		{
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
			alert("<%tcWebApi_get("String_Entry","ACLJS0Text","s")%>"+Address);
			<!-- RpEnd --> 
		}else if(Where == 2){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
			alert("<%tcWebApi_get("String_Entry","ACLJS1Text","s")%>");
			<!-- RpEnd -->
		}else{	    
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg1" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg1" RpTextType="ASCII" -->  	               
			alert("<%tcWebApi_get("String_Entry","ACLJS3Text","s")%>" + Address);
			<!-- RpEnd --> 
		}     	
		return false;
	}

	return true;
}

function doValidateRange(startIP, endIP, subMK, flag) 
{
	var staddress;
	var edaddress;
	var submask;

	var stIPVal, endIPVal, sbMKVal;

	staddress = startIP.split(".");
	edaddress = endIP.split(".");
	submask = subMK.split(".");
	
	stIPVal = (parseInt(staddress[0]) << 24) + (parseInt(staddress[1]) << 16) + (parseInt(staddress[2]) << 8) + parseInt(staddress[3]);
	endIPVal = (parseInt(edaddress[0]) << 24) + (parseInt(edaddress[1]) << 16) + (parseInt(edaddress[2]) << 8) + parseInt(edaddress[3]);
	sbMKVal = (parseInt(submask[0]) << 24) + (parseInt(submask[1]) << 16) + (parseInt(submask[2]) << 8) + parseInt(submask[3]);

	var i;
	for (i = 0; i < 4; i++) {
		stIPVal = parseInt(staddress[i]);
		endIPVal = parseInt(edaddress[i]);
		sbMKVal = parseInt(submask[i]);
		
		if ((stIPVal & sbMKVal) != (endIPVal & sbMKVal)) {
			if (flag == 1) {
				alert("<%tcWebApi_get("String_Entry","ACLASBJS1Text","s")%>");
			}
			else {
				alert("<%tcWebApi_get("String_Entry","ACLASBJS5Text","s")%>");
			}
			return false;
		}

		if ((flag == 0) && (endIPVal < stIPVal)) {
			alert("<%tcWebApi_get("String_Entry","ACLJS5Text","s")%>");
			return false;
		}
	}
	
	

	return true;
}

function isValidIpAddr(ip1,ip2,ip3,ip4) 
{

	if(ip1==0 || ip4==255 || ip1==127 || ip4==0)
		return false;

	return true;
}

function onClickACLSummary()
{
	window.open("/cgi-bin/acl_table.asp","ACL_List","toolbar=no,menubar=no,scrollbars=yes,height=600, width=800,location=0,left=100,top=100");
	return true;
}

</script>

<body onLoad="init()" >
<form METHOD="POST" action="/cgi-bin/access_acl.asp" name="ACL_form">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td height="5" class="light-orange" colspan="5">
				<input name="allowed_flag0" type="hidden" value="No">
				<input name="allowed_flag1" type="hidden" value="No">
				<input name="allowed_flag2" type="hidden" value="No">
				<input name="allowed_flag3" type="hidden" value="No">
				<input name="allowed_flag4" type="hidden" value="No">
				<input name="allowed_flag5" type="hidden" value="No">
			</td>
		</tr>
		<tr>
			<td width="150" height="25" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","ACLText","s")%></font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"></td>
			<td width="10" ></td>
			<td width="440"></td>
		</tr>
		<tr>
			<td height="25" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLACLText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="Activate" VALUE="Yes" onClick="aclSwitch(1)"><font color="#000000"><%tcWebApi_get("String_Entry","ACLActivatedText","s")%></font>
				<INPUT TYPE="RADIO" NAME="Activate" VALUE="No" onClick="aclSwitch(0)"><font color="#000000"><%tcWebApi_get("String_Entry","ACLDeactivatedText","s")%></font>
			<script language="JavaScript" type="text/JavaScript">
			var	acl_active = "<% tcWebApi_get("ACL_Common", "Active", "s") %>";
			with (document.ACL_form){
				if ("Yes" == acl_active)
					Activate[0].checked = true;
				else {
						if (1 == service_flag)
							Activate[0].checked = true;
						else
							Activate[1].checked = true;
				}
			}
			</script>
			</td>
		</tr>
		<tr>
			<td width="150" height="25" class="title-main"><div align=right><font color="#FFFFFF">Basic setting</font></div></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"><hr noshade></td>
			<td width="10"><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td width="150" height="25" class="light-orange">&nbsp;</td>
			<td width="10" class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBServiceText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="SRVIndexSEL" SIZE="1" onChange="doSRVIndexChange()">
					<OPTION VALUE="0" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText0","s")%>
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText1","s")%>
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText2","s")%>
				<%if TCWebAPI_get("WebCustom_Entry", "isSSHSupported", "h") = "Yes" then %>
					<OPTION VALUE="3" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "3" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText3","s")%>
				<%end if%>
					<OPTION VALUE="4" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "4" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText4","s")%>
					<OPTION VALUE="5" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "5" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText5","s")%>
					<OPTION VALUE="6" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "6" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText6","s")%>
				</SELECT>
			</td>
		</tr>
		<tr>
			<td height="25" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBDirectionText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
			<!--
				<INPUT TYPE="RADIO" NAME="Direction" VALUE="LAN"
				<%if TCWebAPI_get("ACL_Entry", "Direction", "h") = "LAN" then asp_write("checked") end if %> onClick="directSwitch(1)"><font color="#000000">LAN</font>
				<INPUT TYPE="RADIO" NAME="Direction" VALUE="WAN"
				<%if TCWebAPI_get("ACL_Entry", "Direction", "h") = "WAN" then asp_write("checked") elseif TCWebAPI_get("ACL_Entry", "Direction", "h") = "N/A" then asp_write("checked") end if %> onClick="directSwitch(0)"><font color="#000000">WAN</font>
			-->
				<SELECT NAME="Direction" SIZE="1" onChange="directSwitch()">
					<OPTION VALUE="LAN" <%if TCWebAPI_get("ACL_Entry", "Direction", "h") = "LAN" then 
												asp_Write("selected") 
										 elseif TCWebAPI_get("ACL_Entry", "Direction", "h") = "N/A" then 
										 		asp_Write("selected")
										end if %>><%tcWebApi_get("String_Entry","ACLInterfaceSelectText1","s")%>
					<OPTION VALUE="WAN" <%if TCWebAPI_get("ACL_Entry", "Direction", "h") = "WAN" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","ACLInterfaceSelectText0","s")%>
				</SELECT>
			</td>
		</tr>
		<tr>
			<td height="22" class="title-sub"><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBServiceSetText","s")%></font></td>
			<td class="light-orange"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr id="srvport_info">
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBServicePortText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
      		<td class="tabdata"> <input type="TEXT" name="SRVPORT" size="8" maxlength="8" value=""></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLCgiActiveText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="SRVACTIVE" VALUE="Yes" ><font color="#000000"><%tcWebApi_get("String_Entry","ACLYesText","s")%></font>
				<INPUT TYPE="RADIO" NAME="SRVACTIVE" VALUE="No" ><font color="#000000"><%tcWebApi_get("String_Entry","ACLNoText","s")%></font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAllowedIPText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="SRVALLOWEDSTARTIP" SIZE="15" MAXLENGTH="15" VALUE="" >~
				<INPUT TYPE="TEXT" NAME="SRVALLOWEDENDIP" SIZE="15" MAXLENGTH="15" VALUE="" ><font color="#000000"><%tcWebApi_get("String_Entry","ACLEditingSrcIPDescripText","s")%></font>
			</td>
		</tr>
		<tr>
			<td class=light-orange>&nbsp;</td>
			<td class=light-orange></td>
			<td class=tabdata><DIV align=right><FONT color=#000000><%tcWebApi_get("String_Entry","ACLASBSubnetText","s")%></FONT></DIV></td>
			<td class=tabdata><DIV align=center>:</DIV></td>
			<td class=tabdata>
				<INPUT maxLength=15 size=15 name="SRVSUBMASK" VALUE="" > 
			</td>
		</tr></table>
	<div id="div_authset">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="150" height="20" class="title-sub"><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAuthSetText","s")%></font></td>
			<td width="10" class="light-orange"></td>
			<td width="150" class="tabdata"><hr noshade class="light-orange-line"></td>
			<td width="10" class="tabdata"><hr noshade class="light-orange-line"></td>
			<td width="440" class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBServicePortText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
			<TABLE cellSpacing=0 cellPadding=0 align=left border=0>
			<TBODY>
			<tr>
				<div><td class="tabdata" style="BORDER-RIGHT-STYLE:none">
				<div id="div_svrport">
					<INPUT TYPE="checkbox" NAME="SRVAUTHUSER2" VALUE="Yes">
					<INPUT TYPE="TEXT" NAME="text_user2" SIZE="15" VALUE="<%tcWebApi_get("Account_Entry1", "username", "s")%>" title="<%tcWebApi_get("Account_Entry1", "username", "s")%>" style="border:0;" readonly="true">
				</div></td></div>
				<td class="tabdata" style="BORDER-LEFT-STYLE:none">
				&nbsp;<INPUT TYPE="checkbox" NAME="SRVAUTHUSER3" VALUE="Yes">
					<INPUT TYPE="TEXT" NAME="text_user2_1" SIZE="15" VALUE="<%tcWebApi_get("Account_Entry2", "username", "s")%>" title="<%tcWebApi_get("Account_Entry2", "username", "s")%>" style="border:0;" readonly="true">
				</td>
			</tr></TBODY></TABLE></td>
		</tr></table>
	</div>
	<div id="div_curserv">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="150" height="25" class="title-main"><div align=right><font color="#FFFFFF">Account setting</font></div></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"><hr noshade></td>
			<td width="10"><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td width="150" height="25" class="light-orange">&nbsp;</td>
			<td width="10" class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAcountText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="ACCIndexSEL" SIZE="1" style="width:85px;" onChange="doAccIndexChange()">
				<%if TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") = "0" then%>
					<OPTION VALUE="0" <%if TCWebAPI_get("WebCurSet_Entry", "account_id", "h") = "0" then asp_Write("selected") end if %>><%TCWebAPI_get("Account_Entry0", "username", "s")%>
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "account_id", "h") = "1" then asp_Write("selected") end if %>><%TCWebAPI_get("Account_Entry1", "username", "s")%>
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "account_id", "h") = "2" then asp_Write("selected") end if %>><%TCWebAPI_get("Account_Entry2", "username", "s")%>
				<%elseif TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") = "1" then %>
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "account_id", "h") = "1" then asp_Write("selected") end if %>><%TCWebAPI_get("Account_Entry1", "username", "s")%>
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "account_id", "h") = "2" then asp_Write("selected") end if %>><%TCWebAPI_get("Account_Entry2", "username", "s")%>
				<%elseif TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") = "2" then %>
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "account_id", "h") = "2" then asp_Write("selected") end if %>><%TCWebAPI_get("Account_Entry2", "username", "s")%>
				<%end if%>
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAllowedText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="ACCALLOWED" VALUE="Yes" ><font color="#000000"><%tcWebApi_get("String_Entry","ACLYesText","s")%></font>
				<INPUT TYPE="RADIO" NAME="ACCALLOWED" VALUE="No" ><font color="#000000"><%tcWebApi_get("String_Entry","ACLNoText","s")%></font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAllowedIPText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="ACCALLOWEDSTARTIP" SIZE="15" MAXLENGTH="15" VALUE="" >~
				<INPUT TYPE="TEXT" NAME="ACCALLOWEDENDIP" SIZE="15" MAXLENGTH="15" VALUE="" ><font color="#000000"><%tcWebApi_get("String_Entry","ACLEditingSrcIPDescripText","s")%></font>
			</td>
		</tr>
		<!--
		<tr>
			<td class=light-orange>&nbsp;</td>
			<td class=light-orange></td>
			<td class=tabdata><DIV align=right><FONT color=#000000>Subnet Mask</FONT></DIV></td>
			<td class=tabdata><DIV align=center>:</DIV></td>
			<td class=tabdata>
				<INPUT maxLength=15 size=15 name="ACCSUBMASK" VALUE=
					"<%if TCWebAPI_get("ACL_Entry", "Direction", "h") = "LAN" then 
						if TCWebAPI_get("ACL_Entry_Entry", "lan_submask", "h") <> "N/A" then
							TCWebAPI_get("ACL_Entry_Entry", "lan_submask", "s")
						else
							asp_Write("0.0.0.0")
						end if
					 else 
					 	if TCWebAPI_get("ACL_Entry_Entry", "wan_submask", "h") <> "N/A" then
							TCWebAPI_get("ACL_Entry_Entry", "wan_submask", "s")
						else
							asp_Write("0.0.0.0")
						end if	
					end if 
					%>" > 
			</td>
		</tr>
		-->
		</table></div>
		<div id="div_curacc0">
		<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="150" height="20" class="title-sub"><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAuthSetText","s")%></font></td>
			<td width="10" class="light-orange"></td>
			<td width="150" class="tabdata"><hr noshade class="light-orange-line"></td>
			<td width="10" class="tabdata"><hr noshade class="light-orange-line"></td>
			<td width="440" class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAllowedText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
			<TABLE cellSpacing=0 cellPadding=0 align=left border=0>
			<TBODY>
			<tr>
			<td class="tabdata" style="BORDER-RIGHT-STYLE: none">
				<div id="allowed_acc_id1">
				<INPUT TYPE="checkbox" NAME="ACCAUTHALLOWEDUSER2" SIZE="10" MAXLENGTH="10" VALUE="Yes">
				<INPUT TYPE="TEXT" NAME="text_user2_1" SIZE="15" VALUE="<%tcWebApi_get("Account_Entry1", "username", "s")%>" title="<%tcWebApi_get("Account_Entry1", "username", "s")%>" style="border:0;" readonly="true">
				</div>
			</td>
			<td class="tabdata" style="BORDER-LEFT-STYLE: none">
				<div id="allowed_acc_id2">
				&nbsp;<input type="checkbox" name="ACCAUTHALLOWEDUSER3" size="10" maxlength="10" value="Yes">
				<INPUT TYPE="TEXT" NAME="text_user2_11" SIZE="15" VALUE="<%tcWebApi_get("Account_Entry2", "username", "s")%>" title="<%tcWebApi_get("Account_Entry2", "username", "s")%>" style="border:0;" readonly="true">
          		</div>
			</td>
			</tr></TBODY></TABLE></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAllowedIPText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
			<TABLE cellSpacing=0 cellPadding=0 align=left border=0>
			<TBODY>
			<tr>
				<td class="tabdata" style="BORDER-RIGHT-STYLE: none">
					<div id="allowedip_acc_id1">
					<INPUT TYPE="checkbox" NAME="ACCAUTHALLOWEDIPUSER2" SIZE="10" MAXLENGTH="10" VALUE="Yes">
					<INPUT TYPE="TEXT" NAME="text_user2_2" SIZE="15" VALUE="<%tcWebApi_get("Account_Entry1", "username", "s")%>" title="<%tcWebApi_get("Account_Entry1", "username", "s")%>" style="border:0;" readonly="true">
					</div>
				</td>
				<td class="tabdata" style="BORDER-LEFT-STYLE: none">
				<div id="allowedip_acc_id2">
                   &nbsp;<INPUT TYPE="checkbox" NAME="ACCAUTHALLOWEDIPUSER3" SIZE="10" MAXLENGTH="10" VALUE="Yes">
				   <INPUT TYPE="TEXT" NAME="text_user2_22" SIZE="15" VALUE="<%tcWebApi_get("Account_Entry2", "username", "s")%>" title="<%tcWebApi_get("Account_Entry2", "username", "s")%>" style="border:0;" readonly="true">
                  </div>
				</td>
				</tr></TBODY></TABLE>
			</td>
		</tr>
		</table></div>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="150" height="25" class="title-main"><div align=right><font color="#FFFFFF"><%tcWebApi_get("String_Entry","ACLListText","s")%></font></div></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"><hr noshade></td>
			<td width="10"><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"></td>
			<td class="tabdata"></td>
			<td ><INPUT TYPE="BUTTON" NAME="ACL_Summary" VALUE="<%tcWebApi_get("String_Entry","ACLASBACLSummaryText","s")%>" onClick="onClickACLSummary();"></td>
		</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="160" class="black" height="42" >&nbsp;</td>
			<td width="160" class="orange">&nbsp;</td>
			<td width="440" class="orange">
				<INPUT TYPE="HIDDEN" NAME="ACLActionFlag" VALUE="0">
				<INPUT TYPE="HIDDEN" NAME="dfCBValue" VALUE="No">
				<INPUT TYPE="HIDDEN" NAME="CurAccount" VALUE="<%tcWebAPI_get("WebCurSet_Entry", "CurrentAccess", "s")%>">
				<INPUT TYPE="HIDDEN" NAME="VSPort" VALUE="<%tcWebAPI_get("Info_Service", "virser_ports", "s")%>">
				<INPUT TYPE="HIDDEN" NAME="LanPorts" VALUE="<%tcWebAPI_get("Info_Service", "used_lan_pts", "s")%>">
				<INPUT TYPE="HIDDEN" NAME="WanPorts" VALUE="<%tcWebAPI_get("Info_Service", "used_wan_pts", "s")%>">
				<INPUT TYPE="BUTTON" NAME="ACLAddBTN" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="doAdd();">
				<INPUT TYPE="RESET"  NAME="ACLCancelBTN" VALUE="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>"onClick="document.forms[0].ACLActionFlag.value = 99; javascript:window.location='access_acl.asp'">
			</td>
		</tr>
	</table>
</form>
</body>
</html>
