<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
</head>
<script language="JavaScript" type="text/javascript">
var	ACL_SERVICE = 7;
var	ACL_NEED_ACCOUNT = 4;
var	MAX_ACL_ACCOUNT = 3;
var ATTRIBUTE_NULL = "N/A";
var LAN_DIRECTION = 0;
var WAN_DIRECTION = 1;
var	cur_account = "<%tcWebAPI_get("WebCurSet_Entry", "CurrentAccess", "s")%>";
var ary_account = [
	"<% TCWebAPI_get("Account_Entry0", "username", "s") %>",
	"<% TCWebAPI_get("Account_Entry1", "username", "s") %>",
	"<% TCWebAPI_get("Account_Entry2", "username", "s") %>"
];
var	ary_servie = [
	"web",
	"ftp",
	"telnet",
	"ssh",
	"tftp",
	"snmp",
	"ping"
];
var aclLang_obj = {};
var str_aclLang = [
	[ary_servie[0], "<%TCWebAPI_get("String_Entry", "ACLApplicationSelectText0", "s")%>"],
	[ary_servie[1], "<%TCWebAPI_get("String_Entry", "ACLApplicationSelectText1", "s")%>"],
	[ary_servie[2], "<%TCWebAPI_get("String_Entry", "ACLApplicationSelectText2", "s")%>"],
	[ary_servie[3], "<%TCWebAPI_get("String_Entry", "ACLApplicationSelectText3", "s")%>"],
	[ary_servie[4], "<%TCWebAPI_get("String_Entry", "ACLApplicationSelectText4", "s")%>"],
	[ary_servie[5], "<%TCWebAPI_get("String_Entry", "ACLApplicationSelectText5", "s")%>"],
	[ary_servie[6], "<%TCWebAPI_get("String_Entry", "ACLApplicationSelectText6", "s")%>"],
	["LAN", "<%TCWebAPI_get("String_Entry", "ACLInterfaceSelectText1", "s")%>"],
	["WAN", "<%TCWebAPI_get("String_Entry", "ACLInterfaceSelectText0", "s")%>"],
	["Yes", "<%tcWebApi_get("String_Entry", "ACLYesText", "s")%>"],
	["No", "<%tcWebApi_get("String_Entry", "ACLNoText", "s")%>"]
];
for (var i=0; i<str_aclLang.length; i++){
	aclLang_obj[str_aclLang[i][0]] = str_aclLang[i][1];
}
/*******
str_aclmainInfo[][0] => lan_port;
str_aclmainInfo[][1] => wan_port;
str_aclmainInfo[][2] => lan_active;
str_aclmainInfo[][3] => wan_active;
str_aclmainInfo[][4] => lan_start_ip;
str_aclmainInfo[][5] => wan_start_ip;
str_aclmainInfo[][6] => lan_end_ip;
str_aclmainInfo[][7] => wan_end_ip;
*******/
var	str_aclmainInfo = [
		["<% TCWebAPI_get("ACL_Entry0", "lan_port", "s") %>","<% TCWebAPI_get("ACL_Entry0", "wan_port", "s") %>","<% TCWebAPI_get("ACL_Entry0", "lan_active", "s") %>","<% TCWebAPI_get("ACL_Entry0", "wan_active", "s") %>","<% TCWebAPI_get("ACL_Entry0", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry1", "lan_port", "s") %>","<% TCWebAPI_get("ACL_Entry1", "wan_port", "s") %>","<% TCWebAPI_get("ACL_Entry1", "lan_active", "s") %>","<% TCWebAPI_get("ACL_Entry1", "wan_active", "s") %>","<% TCWebAPI_get("ACL_Entry1", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry2", "lan_port", "s") %>","<% TCWebAPI_get("ACL_Entry2", "wan_port", "s") %>","<% TCWebAPI_get("ACL_Entry2", "lan_active", "s") %>","<% TCWebAPI_get("ACL_Entry2", "wan_active", "s") %>","<% TCWebAPI_get("ACL_Entry2", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry3", "lan_port", "s") %>","<% TCWebAPI_get("ACL_Entry3", "wan_port", "s") %>","<% TCWebAPI_get("ACL_Entry3", "lan_active", "s") %>","<% TCWebAPI_get("ACL_Entry3", "wan_active", "s") %>","<% TCWebAPI_get("ACL_Entry3", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry4", "lan_port", "s") %>","<% TCWebAPI_get("ACL_Entry4", "wan_port", "s") %>","<% TCWebAPI_get("ACL_Entry4", "lan_active", "s") %>","<% TCWebAPI_get("ACL_Entry4", "wan_active", "s") %>","<% TCWebAPI_get("ACL_Entry4", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry4", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry4", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry4", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry5", "lan_port", "s") %>","<% TCWebAPI_get("ACL_Entry5", "wan_port", "s") %>","<% TCWebAPI_get("ACL_Entry5", "lan_active", "s") %>","<% TCWebAPI_get("ACL_Entry5", "wan_active", "s") %>","<% TCWebAPI_get("ACL_Entry5", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry5", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry5", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry5", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry6", "lan_port", "s") %>","<% TCWebAPI_get("ACL_Entry6", "wan_port", "s") %>","<% TCWebAPI_get("ACL_Entry6", "lan_active", "s") %>","<% TCWebAPI_get("ACL_Entry6", "wan_active", "s") %>","<% TCWebAPI_get("ACL_Entry6", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry6", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry6", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry6", "wan_end_ip", "s") %>"]
];

/***********
str_aclentryInfo[][0] => lan_allowed;
				[][1] => wan_allowed;
				[][2] => lan_start_ip;
				[][3] => wan_start_ip;
				[][4] => lan_end_ip;
				[][5] => wan_end_ip;
************/
var str_aclentryInfo = [
//account 0
		["<% TCWebAPI_get("ACL_Entry0_Entry0", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry0", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry0", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry0", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry0", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry0", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry1_Entry0", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry0", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry0", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry0", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry0", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry0", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry2_Entry0", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry0", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry0", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry0", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry0", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry0", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry3_Entry0", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry0", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry0", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry0", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry0", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry0", "wan_end_ip", "s") %>"],
//account 1
		["<% TCWebAPI_get("ACL_Entry0_Entry1", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry1", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry1", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry1", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry1", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry1", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry1_Entry1", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry1", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry1", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry1", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry1", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry1", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry2_Entry1", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry1", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry1", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry1", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry1", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry1", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry3_Entry1", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry1", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry1", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry1", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry1", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry1", "wan_end_ip", "s") %>"],
//account 2
		["<% TCWebAPI_get("ACL_Entry0_Entry2", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry2", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry2", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry2", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry2", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry0_Entry2", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry1_Entry2", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry2", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry2", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry2", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry2", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry1_Entry2", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry2_Entry2", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry2", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry2", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry2", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry2", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry2_Entry2", "wan_end_ip", "s") %>"],
		["<% TCWebAPI_get("ACL_Entry3_Entry2", "lan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry2", "wan_allowed", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry2", "lan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry2", "wan_start_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry2", "lan_end_ip", "s") %>","<% TCWebAPI_get("ACL_Entry3_Entry2", "wan_end_ip", "s") %>"]
];

function stAclInfo(service, direction, port, accindex, active, allowed, allowedips, allowedipe)
{
	//var str_temp = ary_servie[service];
	this.service = aclLang_obj[ary_servie[service]];
	
	if (LAN_DIRECTION == direction)
		this.direction = aclLang_obj["LAN"];
	else
		this.direction = aclLang_obj["WAN"];

	this.port = port;
	
	if ("N/A" != accindex)
		this.account = ary_account[accindex];
	else
		this.account = accindex;
		
	if ("N/A" != active)
		this.active = aclLang_obj[active];
	else
		this.active = active;
	
	if ("N/A" != allowed)
		this.allowed = aclLang_obj[allowed];
	else
		this.allowed = allowed;
	
	if (allowedips != "0.0.0.0")
		this.allowedips = allowedips;
	else
	{
		if (LAN_DIRECTION == direction)
			this.allowedips = str_aclmainInfo[service][4];
		else
			this.allowedips = str_aclmainInfo[service][5];
	}

	if (allowedipe != "0.0.0.0")
		this.allowedipe = allowedipe;
	else
	{
		if (LAN_DIRECTION == direction)
			this.allowedipe = str_aclmainInfo[service][6];
		else
			this.allowedipe = str_aclmainInfo[service][7];
	}
}
var acl_obj = new Array();
var	disp_count = 0;
for (var i=0; i<ACL_SERVICE; i++)
{
	if (i<ACL_NEED_ACCOUNT){
		for (var j=0; j<MAX_ACL_ACCOUNT; j++){
			//lan direction
			if (str_aclentryInfo[j*4 + i][0] != ATTRIBUTE_NULL){
				acl_obj[disp_count++] = new  stAclInfo(i, LAN_DIRECTION, str_aclmainInfo[i][0], j, str_aclmainInfo[i][2], str_aclentryInfo[j*4 + i][0], str_aclentryInfo[j*4 + i][2], str_aclentryInfo[j*4 + i][4]);
			}
			//wan direction
			if (str_aclentryInfo[j*4 + i][1] != ATTRIBUTE_NULL){
				acl_obj[disp_count++] = new  stAclInfo(i, WAN_DIRECTION, str_aclmainInfo[i][1], j, str_aclmainInfo[i][3], str_aclentryInfo[j*4 + i][1], str_aclentryInfo[j*4 + i][3], str_aclentryInfo[j*4 + i][5]);
			}
		}
	}
	else{
		//lan direction
		if (str_aclmainInfo[i][2] != ATTRIBUTE_NULL){
			acl_obj[disp_count++] = new  stAclInfo(i, LAN_DIRECTION, str_aclmainInfo[i][0], ATTRIBUTE_NULL, str_aclmainInfo[i][2], ATTRIBUTE_NULL, str_aclmainInfo[i][4], str_aclmainInfo[i][6]);
		}
		//wan direction
		if (str_aclmainInfo[i][3] != ATTRIBUTE_NULL){
			acl_obj[disp_count++] = new  stAclInfo(i, WAN_DIRECTION, str_aclmainInfo[i][1], ATTRIBUTE_NULL, str_aclmainInfo[i][3], ATTRIBUTE_NULL, str_aclmainInfo[i][5], str_aclmainInfo[i][7]);
		}
	}
}

function writeAclTable(accindex)
{
	var strtemp = "";
	for (var n=0; n<disp_count; n++)
	{
		if("0" != accindex){
			if ("1" == accindex){
				if (ary_account[0] == acl_obj[n].account)
					continue;
			}
			else{
				if ((ary_account[0] == acl_obj[n].account) || (ary_account[1] == acl_obj[n].account))
					continue;
			}
		}
		
		strtemp += '<tr><td align="center" class="tabdata">' + acl_obj[n].service + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + acl_obj[n].direction + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + acl_obj[n].port + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + acl_obj[n].account + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + acl_obj[n].active + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + acl_obj[n].allowed + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + acl_obj[n].allowedips + '~' + acl_obj[n].allowedipe + '</td></tr>\n';
	}
	document.write(strtemp);
}
</script>
<body>
<FORM METHOD="POST" name="ACLForm">
<table border="1" align=center cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
	<tr height="20">
				<td width="60" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBServiceText","s")%></font></strong></td>
				<td width="80" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBDirectionText","s")%></font></strong></td>
				<td width="50" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBPortText","s")%></font></strong></td>
				<td width="100" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAcountText","s")%></font></strong></td>
				<td width="60" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","ACLCgiActiveText","s")%></font></strong></td>
				<td width="60" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAllowedText","s")%></font></strong></td>
				<td width="180" align=center class="tabdata"><strong><font color="#000000"><%tcWebApi_get("String_Entry","ACLASBAllowedIPText","s")%></font></strong></td>
	</tr>
	<script language="JavaScript" type="text/JavaScript">
	writeAclTable(cur_account);
	</script>
</table>
</form>
</body>
</html>

