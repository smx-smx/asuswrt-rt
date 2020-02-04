document.write('<script type="text/javascript" src="/require/require.min.js"></script>');
document.write('<script type="text/javascript" src="/js/support_site.js"></script>');
/* String splice function */
String.prototype.splice = function( idx, rem, s ) {
    return (this.slice(0,idx) + s + this.slice(idx + Math.abs(rem)));
};

/* String repeat function */
String.prototype.repeat = function(times) {
   return (new Array(times + 1)).join(this);
};

/* Internet Explorer lacks this array method */
if (!('indexOf' in Array.prototype)) {
        Array.prototype.indexOf = function(find, i) {
                if(i===undefined) i=0;
                if(i<0) i+= this.length;
                if(i<0) i=0;
                for(var n=this.length; i<n; i++){
                        if (i in this && this[i]===find)
                                return i;
                }
                return -1;
        };
}

/* add Array.prototype.forEach() in IE8 */
if(typeof Array.prototype.forEach != 'function'){
        Array.prototype.forEach = function(callback){
                for(var i = 0; i < this.length; i++){
                        callback.apply(this, [this[i], i, this]);
                }
        };
}

String.prototype.toArray = function(){
        var ret = eval(this.toString());
        if(Object.prototype.toString.apply(ret) === '[object Array]')
                return ret;
        return [];
}

Array.prototype.getIndexByValue = function(value){
        var index = -1;
        for(var i=0; i<this.length; i++){
                if (this[i] == value){
                        index = i;
                        break;
                }
        }
        return index;
}

Array.prototype.getIndexByValue2D = function(value){
        for(var i=0; i<this.length; i++){
                if(this[i].getIndexByValue(value) != -1){
                        return [i, this[i].getIndexByValue(value)]; // return [1-D_index, 2-D_index];
                }
        }
        return -1;
}

Array.prototype.del = function(n){
　if(n < 0)
　　return this;
　else
　　return this.slice(0,n).concat(this.slice(n+1,this.length));
}

// for compatibility jQuery trim on IE
String.prototype.trim = function() {
    return this.replace(/^\s+|\s+$/g, '');
}

var htmlEnDeCode = (function() {
	var charToEntityRegex, entityToCharRegex, charToEntity, entityToChar;

	function resetCharacterEntities() {
		charToEntity = {};
		entityToChar = {};
		// add the default set
		addCharacterEntities({
			'&amp;'     :   '&',
			'&gt;'      :   '>',
			'&lt;'      :   '<',
			'&quot;'    :   '"',
			'&#39;'     :   "'"
		});
	}

	function addCharacterEntities(newEntities) {
		var charKeys = [], entityKeys = [], key, echar;
		for (key in newEntities) {
			echar = newEntities[key];
			entityToChar[key] = echar;
			charToEntity[echar] = key;
			charKeys.push(echar);
			entityKeys.push(key);
		}
		charToEntityRegex = new RegExp('(' + charKeys.join('|') + ')', 'g');
		entityToCharRegex = new RegExp('(' + entityKeys.join('|') + '|&#[0-9]{1,5};' + ')', 'g');
	}

	function htmlEncode(value){
		var htmlEncodeReplaceFn = function(match, capture) {
		return charToEntity[capture];
		};

		return (!value) ? value : String(value).replace(charToEntityRegex, htmlEncodeReplaceFn);
	}

	function htmlDecode(value) {
		var htmlDecodeReplaceFn = function(match, capture) {
		return (capture in entityToChar) ? entityToChar[capture] : String.fromCharCode(parseInt(capture.substr(2), 10));
		};

		return (!value) ? value : String(value).replace(entityToCharRegex, htmlDecodeReplaceFn);
	}

	resetCharacterEntities();

	return {
		htmlEncode: htmlEncode,
		htmlDecode: htmlDecode
	};
})();

var sw_mode = '1';
var productid = '<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>';
var odm_productid = '<% ej_get_productid() %>';
var uptimeStr = "<% uptime(); %>";
var timezone = uptimeStr.substring(26,31);
var boottime = parseInt(uptimeStr.substring(32,42));
var uptimeStr_update = "<% uptime(); %>";
var boottime_update = parseInt(uptimeStr_update.substring(32,42));
var newformat_systime = uptimeStr.substring(8,11) + " " + uptimeStr.substring(5,7) + " " + uptimeStr.substring(17,25) + " " + uptimeStr.substring(12,16); //Ex format: Jun 23 10:33:31 2008
//Mon Aug 27 21:02:17 2012

//var newformat_systime = uptimeStr_new.substring(4,24); //Ex format: Jun 23 10:33:31 2008
var systime_millsec = Date.parse(newformat_systime); // millsec from system
var JS_timeObj = new Date(); // 1970.1.1
var wan_route_x = "";
var wan_nat_x = "";
var wan_proto = "";
var test_page = 0;
var testEventID = "";
var httpd_dir = "/cifs1"
var isFromWAN = false;
if((location.hostname.search('<% tcWebApi_staticGet("Lan_Entry", "IP","s") %>') == -1) && (location.hostname.search('router.asus') == -1)){
        isFromWAN = true;
}


var wanstate = -1;
var wansbstate = -1;
var wanauxstate = -1;
var stopFlag = 0;
/* 

var gn_array_2g = [
		   ["1", "%41%53%55%53%5F%47%75%65%73%74%31%35%36%37", "WPA2PSK", "NONE", "%31%32%33%34%35%36%37%38%31%32%33", \
		    "0", "1", "", "", "", \
		    "", "0", "off", "0", \
		    "deny", "1", "00:AA:34:45:56:77<00:AA:23:23:34:56", "", \	//Mac Filter
		    "", "", ""], \	//Bandwith Limiter
		   ["0", "%41%53%55%53%5F%47%75%65%73%74%32", "WPA2PSK", "AES", "%35%35%34%34%33%33%32%32%31%31", \
		    "0", "1", "", "", "", \
		    "", "0", "off", "0", \
		    "disabled", "0", "", "", \	//Mac Filter
		    "", "", ""], \	//Bandwith Limiter
		   ["0", "%35%35%67%74", "WPAPSKWPA2PSK", "AES", "%33%32%34%32%35%32%36%32%36%61", \
		    "0", "1", "", "", "", \
		    "", "0", "off", "0", \
		    "disabled", "0", "", "", \	//Mac Filter
		    "", "", "",	\	//Bandwith Limiter
		    "0", "0", ""] \	//URL Filter
		  ];
var gn_array_5g = [
		   ["1", "%41%53%55%53%5F%35%47%5F%47%75%65%73%74%31", "WPAPSKWPA2PSK", "AES", "%35%34%33%32%31%32%33%34%35", \
		    "0", "1", "", "", "", \
		    "", "0", "off", "0", \
		    "disabled", "1", "", "", \	//Mac Filter
		    "", "", ""], \	//Bandwith Limiter
		   ["1", "%35%35%35%47%5F%47%75%65%73%74%32", "WPA2PSK", "AES", "%67%74%72%66%76%62%67%64%64", \
		    "0", "1", "", "", "", \
		    "", "0", "off", "0", \
		    "deny", "1", "00:AA:34:45:56:77<00:AA:23:23:34:56", "", \	//Mac Filter
		    "", "", ""], \	//Bandwith Limiter
		   ["0", "%41%53%55%53%5F%35%47%5F%47%75%65%73%74%33", "OPEN", "NONE", "", \
		    "0", "1", "", "", "", \
		    "", "0", "off", "0", \
		    "disabled", "0", "", "", \	//Mac Filter
		    "", "", "",	\	//Bandwith Limiter
		    "0", "0", ""] \	//URL Filter
		  ];
*/

var gn_array_2g = <% wl_get_2G_guestnetwork(); %>;
var gn_array_5g = <% wl_get_5G_guestnetwork(); %>;

<% available_disk_names_and_sizes() %>
<% disk_pool_mapping_info() %>

//SysInfo_Entry TerritoryCode
var ttc = '<%tcWebApi_staticGet("SysInfo_Entry", "TerritoryCode", "s") %>';
//TerritoryCode sku
function in_territory_code(_ptn){
        return (ttc.search(_ptn) == -1) ? false : true;
}
var is_UK_sku = in_territory_code("UK"); 
var is_EU_sku = in_territory_code("EU");
var is_AU_sku = in_territory_code("AU");

//notification value
var country_code = "<% tcWebApi_get("WLan_Common","wl0_CountryCode","s") %>";
var notice_acpw_is_default = '<% check_acpw(); %>';
var noti_auth_mode_2g = '<%tcWebApi_staticGet("WLan_Entry0", "wl0_auth_mode_x", "s")%>';
var noti_auth_mode_5g = '<%tcWebApi_staticGet("WLan_Entry0", "wl1_auth_mode_x", "s")%>';
var webs_state_flag = '<% tcWebApi_get("WebCustom_Entry", "webs_state_flag", "s" ) %>';
var st_ftp_mode = '<%tcWebApi_staticGet("Samba_Entry", "st_ftp_mode", "s") %>';
var st_ftp_force_mode = '<%tcWebApi_staticGet("Samba_Entry", "st_ftp_force_mode", "s") %>';
var st_samba_mode = '<%tcWebApi_staticGet("Samba_Entry", "st_samba_mode", "s") %>';
var st_samba_force_mode = '<%tcWebApi_staticGet("Samba_Entry", "st_samba_force_mode", "s") %>';
var enable_samba = '<%tcWebApi_staticGet("Samba_Entry", "Active", "s") %>';
var enable_ftp = '<%tcWebApi_staticGet("Samba_Entry", "enable_ftp", "s") %>';
var dla_modified = '<%tcWebApi_staticGet("WebCurSet_Entry", "dsltmp_dla_modified", "s") %>';
var dsl_loss_sync = "";
if(dla_modified == "1")
	dsl_loss_sync = "1";
else
	dsl_loss_sync = '<%tcWebApi_staticGet("WebCurSet_Entry", "loss_sync", "s") %>';
var experience_DSL_fb = '<%tcWebApi_staticGet("Misc_Entry","experience_DSL_fb", "s") %>';
var exist_firmver="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>";
var qis_notification_iptv = '<%tcWebApi_staticGet("Misc_Entry","qis_notification_iptv", "s") %>';

/* Hide this function. Unsafe.
var noti_notif_Flag = '<%tcWebApi_staticGet("WebCustom_Entry","webs_notif_flag", "s") %>';

var notif_hint_index = '<%tcWebApi_staticGet("WebCustom_Entry","webs_notif_index", "s") %>';
var notif_hint_info1 = '<%tcWebApi_staticGet("WebCustom_Entry","webs_notif_info1", "s") %>';
var notif_hint_info2 = '<%tcWebApi_staticGet("WebCustom_Entry","webs_notif_info2", "s") %>';
var notif_hint_infomation = notif_hint_info1+notif_hint_info2;
if(notif_hint_infomation.charAt(0) == "+")
		notif_hint_infomation = notif_hint_infomation.substring(2, notif_hint_infomation.length);
var notif_msg = "";		
var notif_hint_array = notif_hint_infomation.split("++");
if(notif_hint_array[0] != ""){
	notif_msg = "<ol style=\"margin-left:-20px;*margin-left:20px;\">";
	for(var i=0; i<notif_hint_array.length; i++){
		if(i==0)
			notif_msg += "<li>"+notif_hint_array[i];
		else
			notif_msg += "<div><img src=\"/images/New_ui/export/line_export_notif.png\" style=\"margin-top:2px;margin-bottom:2px;margin-left:-20px;*margin-left:-20px;\"></div><li>"+notif_hint_array[i];
	}
	notif_msg += "</ol>";
}
else{
	notif_msg = "";
}
*/

var wan_line_state = "<% tcWebApi_staticGet("Info_Adsl", "lineState", "s") %>";
var wan_diag_state = "<% tcWebApi_staticGet("DslDiag_Entry", "dslx_diag_state", "s") %>";
var wlan0_radio_flag = "<% tcWebApi_staticGet("WLan_Entry", "wl0_radio_flag", "s") %>";
var wlan1_radio_flag = "<% tcWebApi_staticGet("WLan_Entry", "wl1_radio_flag", "s") %>";

var support_dfs = "<% support_dfs(); %>";

function change_wl_unit_status(_unit){
	
	document.change_wunit.wl_unit.value = _unit;	
	showLoading(2);
	setTimeout("document.location.href='/Advanced_Wireless_Content.asp';", 2000);
	document.change_wunit.submit();
}

var wans_dualwan_orig = '<%tcWebApi_Get("Dualwan_Entry", "wans_dualwan", "s")%>';
var active_wan_unit = '<% get_wan_unit() %>';
var wans_dualwan_array = new Array();
wans_dualwan_array = wans_dualwan_orig.split(" ");
var usb_index = wans_dualwan_array.getIndexByValue("usb");
var dualwan_enabled = (wans_dualwan_orig.search("none") == -1) ? 1:0;

var realip_support = 0;
var realip_state = "";
var realip_ip = "";
var external_ip = 0;

var banner_code, menu_code="", menu1_code="", menu2_code="", tab_code="", footer_code;
function show_banner(L3){// L3 = The third Level of Menu
var banner_code = "";
banner_code +='<form method="post" name="titleForm" id="titleForm" action="start_apply.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="next_page" value="">\n';
banner_code +='<input type="hidden" name="current_page" value="">\n';
banner_code +='<input type="hidden" name="action_mode" value="apply">\n';
banner_code +='<input type="hidden" name="action_script" value="">\n';
banner_code +='<input type="hidden" name="action_wait" value="5">\n';
banner_code +='<input type="hidden" name="wl_unit" value="" disabled>\n';
banner_code +='<input type="hidden" name="wl_subunit" value="-1" disabled>\n';
banner_code +='<input type="hidden" name="preferred_lang" value="">\n';
banner_code +='<input type="hidden" name="flag" value="">\n';
banner_code +='<input type="hidden" name="rebootFlag" value="1" disabled>\n';
banner_code +='<input type="hidden" name="restoreFlag" value="1" disabled>\n';
banner_code +='<input type="hidden" name="wan_unit" value="">\n';
banner_code +='</form>\n';
banner_code +='<form method="post" name="diskForm_title" action="device-map/safely_remove_disk.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="disk" value="">\n';
banner_code +='</form>\n';

banner_code +='<form method="post" name="change_wunit" action="/Advanced_WAdvanced_Content.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="wl_unit" value="">\n';
banner_code +='<input type="hidden" name="editFlag" value="0">\n';
banner_code +='</form>\n';

banner_code +='<form method="post" name="noti_ftp" action="aidisk/switch_share_mode.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="protocol" value="ftp">\n';
banner_code +='<input type="hidden" name="mode" value="account">\n';
banner_code +='</form>\n';

banner_code +='<form method="post" name="noti_samba" action="aidisk/switch_share_mode.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="protocol" value="cifs">\n';
banner_code +='<input type="hidden" name="mode" value="account">\n';
banner_code +='</form>\n';

banner_code +='<form method="post" name="noti_experience_DSL" action="/start_apply.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="experience_DSL_Flag" value="1">\n';
banner_code +='<input type="hidden" name="experience_DSL_Feedback" value="1">\n';
banner_code +='</form>\n';

banner_code +='<form method="post" name="noti_notif_hint" action="/start_apply.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="action_script" value="notif_hint">\n';
banner_code +='<input type="hidden" name="noti_notif_Flag" value="0">\n';
banner_code +='</form>\n';

if(iptv_support != -1){
	banner_code +='<form method="post" name="qis_notification_iptv_form" action="/start_apply.asp" target="hidden_frame">\n';
	banner_code +='<input type="hidden" name="qis_notification_iptv_Flag" value="1">\n';
	banner_code +='<input type="hidden" name="qis_notification_iptv" value="0">\n';
	banner_code +='</form>\n';
}

banner_code +='<form method="post" name="internetForm_title" action="disconnect_wan.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="DvInfo_PVC" value="<% tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","s") %>">\n';
banner_code +='<input type="hidden" name="Saveflag" value="1">\n';
banner_code +='<input type="hidden" name="Dipflag" value="0">\n';
banner_code +='<input type="hidden" name="DipConnFlag" value="0">\n';
banner_code +='<input type="hidden" name="ModemConnFlag" value="0">\n';
banner_code +='</form>\n';
banner_code +='<div class="banner1" align="center"><img src="/images/New_ui/asustitle.png" width="218" height="54" align="left">\n';
banner_code +='<div style="margin-top:13px;margin-left:-90px;*margin-top:0px;*margin-left:0px;" align="center"><span class="modelName_top"><%tcWebApi_get("String_Entry","Web_Title2","s")%></span></div>';

banner_code +='<form method="post" name="canceldiagForm" action="/stop_dsl_diag.asp" target="hidden_frame">\n';
banner_code +='<input type="hidden" name="action_script" value="stop_dsl_diag">\n';
banner_code +='</form>\n';

// logout, reboot
banner_code +='<a href="javascript:logout();"><div style="margin-top:13px;margin-left:25px; *width:136px;" class="titlebtn" align="center"><span><%tcWebApi_get("String_Entry","t1Logout","s")%></span></div></a>\n';
banner_code +='<a href="javascript:reboot();"><div style="margin-top:13px;margin-left:0px;*width:136px;" class="titlebtn" align="center"><span><%tcWebApi_get("String_Entry","BTN_REBOOT","s")%></span></div></a>\n';

banner_code +='<ul class="navigation"><a href="#">';
banner_code +='<li><dl><dt id="selected_lang"></dt>\n';

if(is_UK_sku){
	banner_code +='<dd><a onclick="submit_language(1)" id="EN">English</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(12)" id="NO">Norsk</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(8)" id="FI">Suomi</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(5)" id="DA">Dansk</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(15)" id="SV">Svensk</a></dd>\n'
}
else{ //EU, WE support all langs (19)
	banner_code +='<dd><a onclick="submit_language(1)" id="EN">English</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(18)" id="TW">繁體中文</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(3)" id="CN">简体中文</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(4)" id="CZ">Česky</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(13)" id="PL">Polski</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(14)" id="RU">Pусский</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(6)" id="DE">Deutsch</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(9)" id="FR">Français</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(17)" id="TR">Türkçe</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(16)" id="TH">ไทย</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(11)" id="MS">Malay</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(12)" id="NO">Norsk</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(8)" id="FI">Suomi</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(5)" id="DA">Dansk</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(15)" id="SV">Svensk</a></dd>\n'
	banner_code +='<dd><a onclick="submit_language(2)" id="BR">Português(Brazil)</a></dd>\n';
	//banner_code +='<dd><a onclick="submit_language(this)" id="JP">日本語</a></dd>\n'
	banner_code +='<dd><a onclick="submit_language(7)" id="ES">Español</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(10)" id="IT">Italiano</a></dd>\n';
	banner_code +='<dd><a onclick="submit_language(19)" id="UK">Український</a></dd>\n';
}	
	
banner_code +='</dl></li>\n';
banner_code +='</a></ul>';
banner_code +='</div>\n';
banner_code +='<table width="998" border="0" align="center" cellpadding="0" cellspacing="0" class="statusBar">\n';
banner_code +='<tr>\n';
banner_code +='<td background="/images/New_ui/midup_bg.png" height="179" valign="top"><table width="764" border="0" cellpadding="0" cellspacing="0" height="12px" style="margin-left:230px;">\n';
banner_code +='<tbody><tr>\n';
banner_code +='<td valign="center" class="titledown" width="auto">';
banner_code +='<span style="font-family:Verdana, Arial, Helvetica, sans-serif;"><%tcWebApi_get("String_Entry","General_x_FirmwareVersion_in","s")%>:</span><a href="Advanced_FirmwareUpgrade_Content.asp" style="color:white;"><span id="firmver" class="title_link"></span></a>\n';
banner_code +='<span style="font-family:Verdana, Arial, Helvetica, sans-serif;">SSID:</span>';
banner_code +='<span onclick=window.location="Wireless_Content_redirect.asp?refresh=2" id="elliptic_ssid_2g" class="title_link"></span>';
banner_code +='<span onclick=window.location="Wireless_Content_redirect.asp?refresh=5" id="elliptic_ssid_5g" style="display:none;" class="title_link"></span>\n';
banner_code +='</td>\n';

banner_code +='<td width="30" id="notification_status1" class="notificationOn"><div id="notification_status" class="notificationOn"></div><div id="notification_desc" class=""></div></td>\n';

if(wifi_hw_sw_support != -1)
	banner_code +='<td width="30"><div id="wifi_hw_sw_status"></div></td>\n';
	
if(cooler_support != -1)
	banner_code +='<td width="30"><div id="cooler_status" style="display:none;"></div></td>\n';
	
//APP Link
if(app_support){
	banner_code +='<td width="30"><div id="app_icon" style="cursor:pointer;padding-right:10px;font-size:12px;font-weight:bold;color:#07C503">App</div>';
	banner_code +='<div id="app_link_table" style="display:none;width:325px;height:360px;position:absolute;background-color:rgb(35, 38, 41);z-index:10;margin-top:13px;margin-left:-170px;;border-radius:5px;box-shadow:3px 3px 4px #000;opacity:.95">';
	banner_code +='<div style="padding:10px;">';
	banner_code +='<div id="cancel_app" style="width:20px;height:20px;background:url(\'/images/button-close.png\') no-repeat;position:absolute;right:10px;"></div>';
	banner_code +='</div>';
	//ASUS Router icon
	banner_code +='<div style="padding:10px;border-bottom:1px solid #666;">';
	banner_code +='<div style="display:table-cell;vertical-align:middle;padding-left:10px;">';
	banner_code +='<div style="width:75px;height:75px;background:url(\'/images/New_ui/asus_router.png\') no-repeat;"></div>';
	banner_code +='</div>';
	banner_code +='<div style="display:table-cell;">';
	banner_code +='<div style="padding:5px 0 5px 15px;font-size:22px;">ASUS Router</div>';
	banner_code +='<div style="padding:5px 0 5px 15px;font-size:14px;color:#BDBDBD">ASUS Router is a revolution app to manage your home network.</div>';
	banner_code +='</div>';	
	banner_code +='</div>';
	//Play Store
	banner_code +='<div style="padding:20px 10px;">';
	banner_code +='<div style="display:table-cell;vertical-align:middle;padding-left:10px;">';
	banner_code +='<div><img src="/images/New_ui/asus_router_android_qr.png" style="width:75px;height:75px;"></div>';
	banner_code +='</div>';
	banner_code +='<div style="display:table-cell;vertical-align:middle;width:100%;text-align:center">';
	banner_code +='<div style="padding-left: 30px;"><a href="https://play.google.com/store/apps/details?id=com.asus.aihome" target="_blank"><div style="width:172px;height:60px;background:url(\'/images/googleplay.png\') no-repeat;"></div></a></div>';
	banner_code +='</div>';	
	banner_code +='</div>';
	//AppStore
	banner_code +='<div style="padding:20px 10px;">';
	banner_code +='<div style="display:table-cell;vertical-align:middle;padding-left:10px;">';
	banner_code +='<div><img src="/images/New_ui/asus_router_ios_qr.png" style="width:75px;height:75px;"></div>';
	banner_code +='</div>';
	banner_code +='<div style="display:table-cell;vertical-align:middle;width:100%;text-align:center">';
	banner_code +='<div style="padding-left: 30px;"><a href="https://itunes.apple.com/tw/app/asus-router/id1033794044" target="_blank"><div style="width:172px;height:51px;background:url(\'/images/AppStore.png\') no-repeat;"></div></a></div>';
	banner_code +='</div>';	
	banner_code +='</div>';
	
	banner_code +='</div>';
	banner_code +='</td>\n';
}
// APP Link End	
	
if(multissid_support != -1)
	banner_code +='<td width="30"><div id="guestnetwork_status"></div></td>\n';

//Viz add 2013.03 for adsl syncup status
banner_code +='<td width="30"><div id="adsl_line_status"></div></td>\n';

if(sw_mode != 3)
	banner_code +='<td width="30"><div id="connect_status"></div></td>\n';
	
if(usb_support != -1)
	banner_code +='<td width="30"><div id="usb_status"></div></td>\n';
	
if(printer_support != -1)
	banner_code +='<td width="30"><div id="printer_status"></div></td>\n';
	
banner_code +='<td width="17"></td>\n';
banner_code +='</tr></tbody></table></td></tr></table>\n';
$("TopBanner").innerHTML = banner_code;
show_loading_obj();
show_top_status();
set_Dr_work();
updateStatus();

	if(app_support && !isIE8){
			document.body.addEventListener('click', show_app_table, false);
	}
}

function show_app_table(evt){

	var target = document.getElementById("app_link_table");
	var evt_target = evt.target || evt.srcElement;  //evt.target for Firefox patched

	if(evt_target.id == "app_icon" || evt_target.id == "cancel_app"){
		if(target.style.display == "none"){
			target.style.display = "";		
		}
		else{
			target.style.display = "none";
		}

	}
	else if(evt_target.offsetParent == null){
		if(target.style.display == ""){
			target.style.display = "none";
		}
		
	}
	else if((evt_target.id != "null" && evt_target.id == "app_link_table") || (evt_target.offsetParent.id != "null" && evt_target.offsetParent.id) == "app_link_table"){	
		return true;

	}
	else{
		if(target.style.display == ""){
			target.style.display = "none";
		}

	}
}

// parsing rc_support
var rc_support = "<% tcWebApi_get("SysInfo_Entry", "rc_support", "s") %>";
var wl_vifnames = "wl0.1 wl0.2 wl0.3";
var traffic_L1_dx = 3;
var traffic_L2_dx = 11;
var band2g_support = rc_support.search("2.4G");
var band5g_support = rc_support.search("5G");
var band5g_11ac_support = rc_support.search("11AC");
var hideQIS = "<% tcWebApi_get("SysInfo_Entry", "hideQIS", "s") %>";
var hideQIS_support = (hideQIS.toUpperCase() == "YES") ? true : false;
var smart_connect_support = rc_support.search("SMART");
var rrsut_support = rc_support.search("rrsut");
var live_update_support = rc_support.search("update");
var cooler_support = rc_support.search("fanctrl");
var power_support = rc_support.search("pwrctrl");
var dbwww_support = rc_support.search("dbwww");
var repeater_support = rc_support.search("repeater");
var Rawifi_support = rc_support.search("rawifi");
var SwitchCtrl_support = rc_support.search("switchctrl");
var Jumbo_frame_support = rc_support.search("jumbo_frame");
var dsl_support = rc_support.search("dsl");
var dsl_diag_support = rc_support.search("dsl_diag");
var dualWAN_support = rc_support.search("dualwan");
var multissid_support = rc_support.search("mssid");
if(sw_mode == 2 || sw_mode == 4)
	multissid_support = -1;
if(multissid_support != -1)
	multissid_support = wl_vifnames.split(" ").length;
var no5gmssid_support = rc_support.search("no5gmssid");
var nat_support = 1;
var wifi_hw_sw_support = rc_support.search("wifi_hw_sw");
var usb_support = rc_support.search("usb");
var printer_support = rc_support.search("printer");
var appbase_support = rc_support.search("appbase");
var appnet_support = rc_support.search("appnet");
var media_support = (rc_support.search("media") == -1) ? false : true;
var nomedia_support = (rc_support.search("nomedia") == -1) ? false : true;
var cloudsync_support = (rc_support.search("cloudsync") == -1) ? false : true;
var aicloudipk_support = false;
var modem_support = rc_support.search("modem");
var IPv6_support = rc_support.search("ipv6");
var ParentalCtrl_support = rc_support.search("PARENTAL");
var pptpd_support = rc_support.search("pptpd");
var openvpnd_support = rc_support.search("openvpnd");
var vpnc_support = rc_support.search("vpnc"); 
var WebDav_support = rc_support.search("webdav");
var HTTPS_support = rc_support.search("HTTPS");
var nodm_support = rc_support.search("nodm");
var wimax_support = rc_support.search("wimax");
var tcipsec_support = rc_support.search("tcipsec");
var wds_support = rc_support.search("wds");
var spectrum_support = rc_support.search("spectrum");
var userRSSI_support = rc_support.search("user_low_rssi");
var iptv_support = rc_support.search("iptv");
var timemachine_support = rc_support.search("timemachine");
var email_support = rc_support.search("email");
var feedback_support = rc_support.search("feedback");
var ssh_support = rc_support.search("ssh");
var location_list_support = rc_support.search("loclist");
var telnet_support = rc_support.search("telnet");
var wps_reset_support = rc_support.search("WPS_reset");
var cwmp_support = rc_support.search("cwmp");
var networkTool_support = true;
var tr069_support = rc_support.search("tr069");
var gobi_support = rc_support.search("gobi");
var webmon_support = rc_support.search("ipt_webmon");
var access_log_support = rc_support.search("access_log");
var bwdpi_support = rc_support.search("bwdpi");
var app_support = true;	//all model incleded for now
var ifttt_support = rc_support.search("ifttt");
var alexa_support = rc_support.search("alexa");
// rc_support end

var Diag2jffs_support = (productid=="DSL-N16" || productid=="DSL-AC51" || productid=="DSL-AC750")?true:false;

// parsing ss_support (Smart Sync)
var ss_support = '<% tcWebApi_Get("AiCloud_Entry","ss_support","s") %>';
var smart_sync_support = false;
function isSupport(_nvramvalue, _ptn){
	return (_nvramvalue.search(_ptn) == -1) ? false : true;
}
if( isSupport(ss_support, "asuswebstorage") || isSupport(ss_support, "dropbox") ||
	isSupport(ss_support, "ftp") || isSupport(ss_support, "samba") || isSupport(ss_support, "usb")){ 
	smart_sync_support = true;
}

var tabtitle = new Array();
tabtitle[0] = new Array("", "<%tcWebApi_get("String_Entry","menu5_1_1","s")%>", "<%tcWebApi_get("String_Entry","menu5_1_2","s")%>", "WDS" ,"<%tcWebApi_get("String_Entry","menu5_1_4","s")%>", "<%tcWebApi_get("String_Entry","menu5_1_5","s")%>", "<%tcWebApi_get("String_Entry","menu5_1_6","s")%>");
tabtitle[1] = new Array("", "<%tcWebApi_get("String_Entry","menu5_2_1","s")%>", "<%tcWebApi_get("String_Entry","menu5_2_2","s")%>", "<%tcWebApi_get("String_Entry","menu5_2_3","s")%>", "IPTV", "<%tcWebApi_get("String_Entry","Switch_itemname","s")%>");
tabtitle[2] = new Array("", "<%tcWebApi_get("String_Entry","menu5_3_1","s")%>", "Dual WAN", "<%tcWebApi_get("String_Entry","menu5_3_3","s")%>", "<%tcWebApi_get("String_Entry","menu5_3_4","s")%>", "<%tcWebApi_get("String_Entry","menu5_3_5","s")%>", "<%tcWebApi_get("String_Entry","menu5_3_6","s")%>", "<%tcWebApi_get("String_Entry","NAT_passthrough_in","s")%>", "<%tcWebApi_get("String_Entry","menu5_4_4","s")%>");
tabtitle[3] = new Array("", "<%tcWebApi_get("String_Entry","UPnPMediaServer","s")%>", "<%tcWebApi_get("String_Entry","menu5_4_1","s")%>", "<%tcWebApi_get("String_Entry","menu5_4_2","s")%>");
tabtitle[4] = new Array("", "IPv6");
tabtitle[5] = new Array("", "<%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%>", "VPN Client", "IPSec VPN");
tabtitle[6] = new Array("", "<%tcWebApi_get("String_Entry","menu5_1_1","s")%>", "<%tcWebApi_get("String_Entry","menu5_5_2","s")%>", "<%tcWebApi_get("String_Entry","menu5_5_3","s")%>", "<%tcWebApi_get("String_Entry","menu5_5_5","s")%>", "<%tcWebApi_get("String_Entry","menu5_5_4","s")%>");
tabtitle[7] = new Array("", "<%tcWebApi_get("String_Entry","menu5_6_2","s")%>", "<%tcWebApi_get("String_Entry","menu5_6_3","s")%>", "<%tcWebApi_get("String_Entry","menu5_6_4","s")%>", "Performance tuning", "<%tcWebApi_get("String_Entry","menu_dsl_setting","s")%>", "<%tcWebApi_get("String_Entry","menu_dsl_feedback","s")%>", "CWMP", "TR069", "<%tcWebApi_get("String_Entry","menu_privacy","s")%>");
tabtitle[8] = new Array("", "<%tcWebApi_get("String_Entry","menu5_7_2","s")%>", "<%tcWebApi_get("String_Entry","menu5_7_4","s")%>", "<%tcWebApi_get("String_Entry","menu5_7_3","s")%>", "<%tcWebApi_get("String_Entry","menu5_7_6","s")%>", "<%tcWebApi_get("String_Entry","menu5_7_5","s")%>", "<%tcWebApi_get("String_Entry","menu_dsl_log","s")%>", "<%tcWebApi_get("String_Entry","Connections","s")%>", "Access Log");
tabtitle[9] = new Array("", "<%tcWebApi_get("String_Entry","Network_Analysis","s")%>", "Netstat", "<%tcWebApi_get("String_Entry","NetworkTools_WOL","s")%>");
tabtitle[10] = new Array("", "QoS", "<%tcWebApi_get("String_Entry","traffic_monitor","s")%>", "<% tcWebApi_get("String_Entry","Adaptive_History","s") %>", "<% tcWebApi_get("String_Entry","Spectrum_title","s") %>");
tabtitle[11] = new Array("");

if(bwdpi_support != -1){
	tabtitle[12] = new Array("", "<%tcWebApi_get("String_Entry","AiProtection_Home","s")%>", "<%tcWebApi_get("String_Entry","Parental_Control","s")%>");
}

var tablink = new Array();
tablink[0] = new Array("", "Advanced_Wireless_Content.asp", "Advanced_WWPS_Content.asp", "Advanced_WMode_Content.asp", "Advanced_ACL_Content.asp", "Advanced_WSecurity_Content.asp", "Advanced_WAdvanced_Content.asp");
tablink[1] = new Array("", "Advanced_LAN_Content.asp", "Advanced_DHCP_Content.asp", "Advanced_GWStaticRoute_Content.asp", "Advanced_IPTV_Content.asp", "Advanced_SwitchCtrl_Content.asp");
tablink[2] = new Array("", "Advanced_DSL_Content.asp", "Advanced_WANPort_Content.asp", "Advanced_PortTrigger_Content.asp", "Advanced_VirtualServer_Content.asp", "Advanced_Exposed_Content.asp", "Advanced_ASUSDDNS_Content.asp", "Advanced_NATPassThrough_Content.asp", "Advanced_Modem_Content.asp");
tablink[3] = new Array("", "mediaserver.asp", "Advanced_AiDisk_samba.asp", "Advanced_AiDisk_ftp.asp");
tablink[4] = new Array("", "Advanced_IPv6_Content.asp");
tablink[5] = new Array("", "Advanced_VPN_PPTP.asp", "Advanced_VPNClient_Content.asp", "adv_vpn_setting.asp");
tablink[6] = new Array("", "Advanced_BasicFirewall_Content.asp", "Advanced_URLFilter_Content.asp", "Advanced_MACFilter_Content.asp", "Advanced_KeywordFilter_Content.asp", "Advanced_Firewall_Content.asp");
tablink[7] = new Array("", "Advanced_System_Content.asp", "Advanced_FirmwareUpgrade_Content.asp", "Advanced_SettingBackup_Content.asp", "Advanced_PerformanceTuning_Content.asp", "Advanced_ADSL_Content.asp", "Advanced_Feedback.asp", "Advanced_CWMP_Content.asp", "Advanced_TR069_Content.asp", "Advanced_Privacy.asp");
tablink[8] = new Array("", "Main_LogStatus_Content.asp", "Main_WStatus_Content.asp", "Main_DHCPStatus_Content.asp", "Main_RouteStatus_Content.asp", "Main_IPTStatus_Content.asp", "Main_AdslStatus_Content.asp", "Main_ConnStatus_Content.asp", "Main_AccessLog_Content.asp");
tablink[9] = new Array("", "Main_Analysis_Content.asp", "Main_Netstat_Content.asp", "Main_WOL_Content.asp");
tablink[10] = new Array("", "QoS_EZQoS.asp", "Main_TrafficMonitor_realtime.asp", "Main_WebHistory_Content.asp", "Main_Spectrum_Content.asp", "Main_TrafficMonitor_last24.asp", "Main_TrafficMonitor_daily.asp", "Advanced_QOSUserPrio_Content.asp", "Advanced_QOSUserRules_Content.asp");
tablink[11] = new Array("");

if(bwdpi_support != -1){
	tablink[12] = new Array("", "AiProtection_HomeProtection.asp", "AiProtection_WebProtector.asp");
}

//Level 2 Menu
menuL2_title = new Array("", "<%tcWebApi_get("String_Entry","menu5_1","s")%>", "<%tcWebApi_get("String_Entry","menu5_2","s")%>", "<%tcWebApi_get("String_Entry","menu5_3","s")%>", "<%tcWebApi_get("String_Entry","menu5_4","s")%>", "IPv6", "VPN", "<%tcWebApi_get("String_Entry","menu5_5","s")%>", "<%tcWebApi_get("String_Entry","menu5_6","s")%>", "<%tcWebApi_get("String_Entry","System_Log","s")%>", "<%tcWebApi_get("String_Entry","Network_Tools","s")%>");
menuL2_link = new Array("", tablink[0][1], tablink[1][1], tablink[2][1], tablink[3][1], tablink[4][1], tablink[5][1], tablink[6][1], tablink[7][1], tablink[8][1], tablink[9][1]);

if(bwdpi_support != -1){
	menuL1_title = new Array("", "<%tcWebApi_get("String_Entry","menu1","s")%>", "<%tcWebApi_get("String_Entry","Guest_Network","s")%>", "<%tcWebApi_get("String_Entry","Menu_TrafficManager","s")%>", "<%tcWebApi_get("String_Entry","AiProtection_title","s")%>", "<%tcWebApi_get("String_Entry","Menu_usb_application","s")%>", "<%tcWebApi_Get("String_Entry", "AiCloud_Title","s")%>", "<%tcWebApi_get("String_Entry","menu5","s")%>");
	menuL1_link = new Array("", "index2.asp", "Guest_network.asp", "QoS_EZQoS.asp", "AiProtection_HomeProtection.asp", "APP_Installation.asp", "cloud_main.asp", "");
}
else {
	menuL1_title = new Array("", "<%tcWebApi_get("String_Entry","menu1","s")%>", "<%tcWebApi_get("String_Entry","Guest_Network","s")%>", "<%tcWebApi_get("String_Entry","Menu_TrafficManager","s")%>", "<%tcWebApi_get("String_Entry","Parental_Control","s")%>", "<%tcWebApi_get("String_Entry","Menu_usb_application","s")%>", "<%tcWebApi_Get("String_Entry", "AiCloud_Title","s")%>", "<%tcWebApi_get("String_Entry","menu5","s")%>");
	menuL1_link = new Array("", "index2.asp", "Guest_network.asp", "QoS_EZQoS.asp", "ParentalControl.asp", "APP_Installation.asp", "cloud_main.asp", "");
}

var calculate_height = menuL1_link.length+tablink.length-3;
var model_name = "<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>";
var downsize_support = -1;
var wl6_support = -1;

//wireless
var wl_nband_title = [];
var wl_nband_array = '<% wl_nband_info(); %>'.toArray();
var band2g_count = 0;
var band5g_count = 0;
for (var j=0; j<wl_nband_array.length; j++) {
	if(wl_nband_array[j] == '2'){
		band2g_count++;
		wl_nband_title.push("2.4 GHz" + ((band2g_count > 1) ? ("-" + band2g_count) : ""));
	}
	else if(wl_nband_array[j] == '1'){
		band5g_count++;
		wl_nband_title.push("5 GHz" + ((band5g_count > 1) ? ("-" + band5g_count) : ""));
	}
}
if(wl_nband_title.indexOf("2.4 GHz-2") > 0) wl_nband_title[wl_nband_title.indexOf("2.4 GHz")] = "2.4 GHz-1";
if(wl_nband_title.indexOf("5 GHz-2") > 0) wl_nband_title[wl_nband_title.indexOf("5 GHz")] = "5 GHz-1";

var wl_info = {
	band2g_support:(function(){
				if(band2g_count > 0)
					return true;
				else
					return false;
			})(),
	band5g_support:(function(){
				if(band5g_count > 0)
					return true;
				else
					return false;
			})(),
	band5g_2_support:(function(){
				if(band5g_count == 2)
					return true;
				else
					return false;
			})(),
	band2g_total:band2g_count,
	band5g_total:band5g_count,
	wl_if_total:wl_nband_array.length
};
//wireless end

var QISWIZARD = "QIS_wizard.asp";

function remove_url(){
	remove_menu_item(2, "Advanced_Modem_Content.asp");
	if(wds_support == -1) {
		remove_menu_item(0, "Advanced_WMode_Content.asp");
	}
	
	if(dsl_support == -1) {
		remove_menu_item(7, "Advanced_ADSL_Content.asp");
		remove_menu_item(8, "Main_AdslStatus_Content.asp");
		remove_menu_item(10, "Main_Spectrum_Content.asp");
	}else{
		if(spectrum_support == -1)	// not to support Spectrum page.
			remove_menu_item(10, "Main_Spectrum_Content.asp");
	}
	
	if(!networkTool_support){
		menuL2_title[10] = "";
		menuL2_link[10] = "";
	}	
	
	if(WebDav_support != -1) {
		tabtitle[3][2] = "<%tcWebApi_get("String_Entry","menu5_4_1","s")%> / Cloud Disk";
	}
	if(sw_mode == 2){
		// Guest Network
		menuL1_title[2] ="";
		menuL1_link[2] ="";
		// Traffic Manager
		menuL1_title[3] ="";
		menuL1_link[3] ="";
		// Parental Ctrl
		menuL1_title[4] ="";
		menuL1_link[4] ="";
		// AiCloud
		menuL1_title[6] ="";
		menuL1_link[6] ="";
		// Wireless
		menuL2_title[1]="";
		menuL2_link[1]="";
		// WAN
		menuL2_title[3]="";
		menuL2_link[3]="";
		// LAN
		remove_menu_item(1, "Advanced_DHCP_Content.asp");
		remove_menu_item(1, "Advanced_GWStaticRoute_Content.asp");
		remove_menu_item(1, "Advanced_IPTV_Content.asp");
		remove_menu_item(1, "Advanced_SwitchCtrl_Content.asp");
		// VPN
		menuL2_title[5]="";
		menuL2_link[5]="";
		//IPv6
		menuL2_title[6]="";
		menuL2_link[6]="";
		// Firewall
		menuL2_title[7]="";
		menuL2_link[7]="";
		// Log
		remove_menu_item(8, "Main_DHCPStatus_Content.asp");
		remove_menu_item(8, "Main_IPTStatus_Content.asp");
		remove_menu_item(8, "Main_RouteStatus_Content.asp");
	}
	else if(sw_mode == 3){
		// Traffic Manager
		menuL1_title[3] ="";
		menuL1_link[3] ="";
		// Parental Ctrl
		menuL1_title[4] ="";
		menuL1_link[4] ="";
		// AiCloud
		menuL1_title[6] ="";
		menuL1_link[6] ="";

		// Wireless
		remove_menu_item(0, "Advanced_WMode_Content.asp");
		remove_menu_item(0, "Advanced_WWPS_Content.asp");
		// WAN
		menuL2_title[3]="";
		menuL2_link[3]="";
		// LAN
		remove_menu_item(1, "Advanced_DHCP_Content.asp");
		remove_menu_item(1, "Advanced_GWStaticRoute_Content.asp");
		remove_menu_item(1, "Advanced_IPTV_Content.asp");
		remove_menu_item(1, "Advanced_SwitchCtrl_Content.asp");
		// VPN
		menuL2_title[5]="";
		menuL2_link[5]="";
		// IPv6
		menuL2_title[6]="";
		menuL2_link[6]="";
		// Firewall
		menuL2_title[7]="";
		menuL2_link[7]="";
		// Log
		remove_menu_item(8, "Main_DHCPStatus_Content.asp");
		remove_menu_item(8, "Main_IPTStatus_Content.asp");
		remove_menu_item(8, "Main_RouteStatus_Content.asp");
	}

	if (dualWAN_support == -1) {
		remove_menu_item(2, "Advanced_WANPort_Content.asp");
	}
	else {
		var dualwan_pri_if = '<%tcWebApi_get("Dualwan_Entry","wans_dualwan","s")%>'.split(" ")[0];		
		
		if(dualwan_pri_if == 'dsl' || dualwan_pri_if == 'wan'){
				menuL2_link[3] = "Advanced_DSL_Content.asp";
				tablink[2][1] = "Advanced_DSL_Content.asp";
		}		
		else if(dualwan_pri_if == 'usb'){
				menuL2_link[3] = "Advanced_Modem_Content.asp";
				tablink[2][1] = "Advanced_Modem_Content.asp";
		}		
		else{
				menuL2_link[3] = "Advanced_DSL_Content.asp";
				tablink[2][1] = "Advanced_DSL_Content.asp";
		}		
	}

	if(!media_support){
		tabtitle[3].splice(1, 1);
		tablink[3].splice(1, 1);
	}
	if(cooler_support == -1){
		remove_menu_item(7, "Advanced_PerformanceTuning_Content.asp");
	}
	if(ParentalCtrl_support == -1){
		if(sw_mode == 3){
			menuL1_title[4]="";
			menuL1_link[4]="";
		}
	}
	else
		remove_menu_item(6, "Advanced_MACFilter_Content.asp");
	if(nat_support == -1){ // ifdef nat_support
		remove_menu_item(2, "Advanced_PortTrigger_Content.asp");
		remove_menu_item(2, "Advanced_VirtualServer_Content.asp");
		remove_menu_item(2, "Advanced_Exposed_Content.asp");
		remove_menu_item(8, "Main_IPTStatus_Content.asp");
	}
	if(IPv6_support == -1){
		menuL2_title[5] = "";
		menuL2_link[5] = "";
	}
	if(multissid_support == -1){
		menuL1_title[2]="";
		menuL1_link[2]="";
	}
	if(usb_support == -1){
		menuL1_title[5]="";
		menuL1_link[5]="";
		//if(!cloudsync_support && WebDav_support == -1){
			menuL1_title[6] = "";
			menuL1_link[6] = "";
		//}
	}
	
	if(pptpd_support == -1 && openvpnd_support == -1 && vpnc_support == -1){
			menuL2_title[6] = "";
			menuL2_link[6] = "";
	}else{
		if(vpnc_support == -1 && openvpnd_support == -1){
			tabtitle[5].splice(2, 1);
			tablink[5].splice(2, 1);
		}
		if(pptpd_support == -1 && openvpnd_support == -1){
			tabtitle[5].splice(1, 1);
			tablink[5].splice(1, 1);
			menuL2_link[6] = "Advanced_VPNClient_Content.asp";
		}
		var VPNServer_mode = '<%tcWebApi_Get("VPNControl_Entry", "VPNServer_mode", "s")%>';
		var VPNServer_enable = '<%tcWebApi_Get("VPNControl_Entry", "VPNServer_enable", "s")%>';
		if((pptpd_support == -1 && openvpnd_support) || (VPNServer_mode == "openvpn" && VPNServer_enable == "1")){
			tablink[5][1] = "Advanced_VPN_OpenVPN.asp";
			menuL2_link[6] = "Advanced_VPN_OpenVPN.asp";
		}
	}
	
	if(SwitchCtrl_support == -1){
		remove_menu_item(1, "Advanced_SwitchCtrl_Content.asp");
	}
	if(tcipsec_support == -1){
		remove_menu_item(5, "adv_vpn_setting.asp");
	}
	if(iptv_support == -1){
		remove_menu_item(1, "Advanced_IPTV_Content.asp");
	}
	if(feedback_support == -1){
		remove_menu_item(7, "Advanced_Feedback.asp");
	}
	if(cwmp_support == -1){
		remove_menu_item(7, "Advanced_CWMP_Content.asp");
	}
	if(tr069_support == -1){
		remove_menu_item(7, "Advanced_TR069_Content.asp");
	}	
	if(webmon_support == -1){
		remove_menu_item(10, "Main_WebHistory_Content.asp");
	}
	if(access_log_support == -1){
		remove_menu_item(8, "Main_AccessLog_Content.asp");
	}
}
function remove_menu_item(L2, remove_url){
var dx;
for(var i = 0; i < tablink[L2].length; i++){
dx = tablink[L2].getIndexByValue(remove_url);
if(dx == -1) //If not match, pass it
return false;
else if(dx == 1) //If removed url is the 1st tablink then replace by next tablink
menuL2_link.splice(L2+1, 1, tablink[L2][2]);
else{
tabtitle[L2].splice(dx, 1);
tablink[L2].splice(dx, 1);
break;
}
}
}

String.prototype.shorter = function(len){
	var replaceWith = "...";

	if(this.length > len)
		return this.substring(0, len-replaceWith.length)+replaceWith;
	else
		return this.toString();
}

Array.prototype.getIndexByValue= function(value){
var index = -1;
for (var i = 0; i < this.length; i++){
if (this[i] == value){
index = i;
break;
}
}
return index;
}

Array.prototype.getIndexByValue2D = function(value){
	for(var i=0; i<this.length; i++){
		if(this[i].getIndexByValue(value) != -1){
			return [i, this[i].getIndexByValue(value)]; // return [1-D_index, 2-D_index];
		}
	}
	return -1;
}

Array.prototype.del = function(n){
if(n < 0)
	return this;
else
	return this.slice(0,n).concat(this.slice(n+1,this.length));
}

var current_url = location.pathname.substring(location.pathname.lastIndexOf('/') + 1);
function show_menu(){
var L1 = 0, L2 = 0, L3 = 0;
if(current_url == "") current_url = "index2.asp";
remove_url();
for(var i = 1; i < menuL1_link.length; i++){
if(current_url == menuL1_link[i]){
L1 = i;
break;
}
else
L1 = menuL1_link.length;
}

// cloud
if(current_url.indexOf("cloud") == 0)
	L1 = 6;

if(L1 == menuL1_link.length){
for(var j = 0; j < tablink.length; j++){
for(var k = 1; k < tablink[j].length; k++){
if(current_url == tablink[j][k]){
L2 = j+1;
L3 = k;
break;
}
}
}
}
if(L1 == traffic_L1_dx || L2 == traffic_L2_dx){
if(current_url.indexOf("Main_TrafficMonitor_") == 0){
L1 = traffic_L1_dx;
L2 = traffic_L2_dx;
L3 = 2;
}
else if(current_url.indexOf("Main_WebHistory_") == 0){
L1 = traffic_L1_dx;
L2 = traffic_L2_dx;
L3 = 3;
}
else if(current_url.indexOf("Main_Spectrum_") == 0){
L1 = traffic_L1_dx;
L2 = traffic_L2_dx;
L3 = 4;
}
else{
L1 = traffic_L1_dx;
L2 = traffic_L2_dx;
L3 = 1;
}
}

	if(bwdpi_support != -1){
	
		if(current_url.indexOf("AiProtection_HomeProtection.asp") == 0){
			L1 = 4;
			traffic_L2_dx = 13;
			traffic_L3_dx = 1;
			L2 = traffic_L2_dx;
			L3 = traffic_L3_dx;
		}
		if(current_url.indexOf("AiProtection_WebProtector.asp") == 0 ||
			current_url.indexOf("ParentalControl") == 0){
			traffic_L1_dx = 4;
			traffic_L2_dx = 13;
			traffic_L3_dx = 2;
			
			L1 = traffic_L1_dx;	
			L2 = traffic_L2_dx;
			L3 = traffic_L3_dx;
		}	
	}
	else{
		if(current_url.indexOf("ParentalControl") == 0){
			traffic_L2_dx = 12;
			traffic_L3_dx = 5;
			
			L2 = traffic_L2_dx;
			L3 = traffic_L3_dx;		
		}
	}

	if(current_url.indexOf("Advanced_VPNAdvanced") == 0){
			traffic_L1_dx = 0;
			traffic_L2_dx = 6;
			L1 = traffic_L1_dx;	
			L2 = traffic_L2_dx;
			L3 = 1;
	}	
	if(current_url.indexOf("Advanced_VPN") == 0){
		traffic_L1_dx = 0;
		traffic_L2_dx = 6;
		L1 = traffic_L1_dx;
		L2 = traffic_L2_dx;
		L3 = 1;
		if(current_url.indexOf("Advanced_VPNClient_Content.asp") == 0 && (pptpd_support != -1 || openvpnd_support != -1)) {
			L3 = 2;
		}
	}

	if('<%tcwebApi_get("Wan_Common","TransMode","s")%>' == 'USB'){
			menuL2_link[3] = "Advanced_Modem_Content.asp";
			tablink[2][1] = "Advanced_Modem_Content.asp";
	}
	
	if(	current_url.indexOf("Advanced_DSL_Content") == 0 || 
			current_url.indexOf("Advanced_VDSL_Content") == 0 ||
			current_url.indexOf("Advanced_Modem_Content") == 0){
			traffic_L1_dx = 0;
			traffic_L2_dx = 3;
			L1 = traffic_L1_dx;	
			L2 = traffic_L2_dx;
			L3 = 1;
	}		

	//Feedback Info
	if(current_url.indexOf("Feedback_Info") == 0){
			traffic_L1_dx = 0;
			traffic_L2_dx = 8;
			L1 = traffic_L1_dx;	
			L2 = traffic_L2_dx;
			L3 = 5;
	}	

show_banner(L3);
show_footer();
browser_compatibility();
show_selected_language();

	// QIS wizard
	if(hideQIS_support){
		menu1_code += '<div style="margin-top:-150px;"></div>\n';
	}else if(sw_mode == 2){
		menu1_code += '<div class="m_qis_r" style="margin-top:-170px;cursor:pointer;" onclick="go_setting(\'/'+ QISWIZARD +'?flag=sitesurvey\');"><table><tr><td><div id="index_img0"></div></td><td><div><%tcWebApi_get("String_Entry","QIS","s")%></div></td></tr></table></div>\n';
	}else if(sw_mode == 3){
		menu1_code += '<div class="m_qis_r" style="margin-top:-170px;cursor:pointer;" onclick="go_setting(\''+ QISWIZARD +'?flag=lanip\');"><table><tr><td><div id="index_img0"></div></td><td><div><%tcWebApi_get("String_Entry","QIS","s")%></div></td></tr></table></div>\n'; 	
	}else if(sw_mode == 4){
		menu1_code += '<div class="m_qis_r" style="margin-top:-170px;cursor:pointer;" onclick="go_setting(\'/'+ QISWIZARD +'?flag=sitesurvey_mb\');"><table><tr><td><div id="index_img0"></div></td><td><div><%tcWebApi_get("String_Entry","QIS","s")%></div></td></tr></table></div>\n';
	}else{
		menu1_code += '<div class="m_qis_r" style="margin-top:-170px;cursor:pointer;" onclick="go_setting(\''+ QISWIZARD +'?flag=detect\');"><table><tr><td><div id="index_img0"></div></td><td><div><%tcWebApi_get("String_Entry","QIS","s")%></div></td></tr></table></div>\n';
	}
	
	// Feature
	menu1_code += '<div class="m0_r" style="margin-top:10px;" id="option0"><table width="192px" height="37px"><tr><td><%tcWebApi_get("String_Entry","menu5_1_1","s")%></td></tr></table></div>\n';
	for(i = 1; i <= menuL1_title.length-2; i++){
		if(menuL1_title[i] == ""){
			calculate_height--;
			continue;
		}
		else if(L1 == i && (L2 <= 0 || L2 == traffic_L2_dx)){
		  menu1_code += '<div class="m'+i+'_r" id="option'+i+'">'+'<table><tr><td><div id="index_img'+i+'"></div></td><td><div id="menu_string'+i+'" style="width:120px;">'+menuL1_title[i]+'</div></td></tr></table></div>\n';
		}
		else{
		  menu1_code += '<div class="menu" id="option'+i+'" onclick="location.href=\''+menuL1_link[i]+'\'" style="cursor:pointer;"><table><tr><td><div id="index_img'+i+'"></div></td><td><div id="menu_string" style="width:120px;">'+menuL1_title[i]+'</div></td></tr></table></div>\n';
		}
	}
	menu1_code += '<div class="m0_r" style="margin-top:10px;" id="option0"><table width="192px" height="37px"><tr><td><%tcWebApi_get("String_Entry","menu5","s")%></td></tr></table></div>\n'; 	
	$("mainMenu").innerHTML = menu1_code;


	// Advanced
	if(L2 != -1){ 
		for(var i = 1; i < menuL2_title.length; ++i){
			if(menuL2_link[i] == "Advanced_Wireless_Content.asp" && "-1" != "0" && "-1" != "-1")
				menuL2_link[i] = "javascript:change_wl_unit_status(" + 0 + ");";
			if(menuL2_title[i] == "" || i == 4){
				calculate_height--;
				continue;
			}
			else if(L2 == i){
				menu2_code += '<div class="m'+i+'_r" id="option'+i+'">'+'<table><tr><td><div id="menu_img'+i+'"></div></td><td><div id="option_str1" style="width:120px;">'+menuL2_title[i]+'</div></td></tr></table></div>\n';
			}else{				
				menu2_code += '<div class="menu" id="option'+i+'" onclick="location.href=\''+menuL2_link[i]+'\'" style="cursor:pointer;"><table><tr><td><div id="menu_img'+i+'"></div></td><td><div id="option_str1" style="width:120px;">'+menuL2_title[i]+'</div></td></tr></table></div>\n';
			}	
		}
	}
	$("subMenu").innerHTML = menu2_code;



if(L3){
if(L2 == traffic_L2_dx){ // if tm
tab_code = '<table border="0" cellspacing="0" cellpadding="0"><tr>\n';
for(var i=1; i < tabtitle[traffic_L2_dx-1].length; ++i){
if(tabtitle[traffic_L2_dx-1][i] == "")
continue;
else if(L3 == i)
tab_code += '<td><div class="tabclick"><span>'+ tabtitle[traffic_L2_dx-1][i] +'</span></div></td>';
else
tab_code += '<td><a href="' + tablink[traffic_L2_dx-1][i] + '"><div class="tab"><span>'+ tabtitle[traffic_L2_dx-1][i] +'</span></div></a></td>';
}
tab_code += '</tr></table>\n';
}
else{
tab_code = '<table border="0" cellspacing="0" cellpadding="0"><tr>\n';
for(var i=1; i < tabtitle[L2-1].length; ++i){
if(tabtitle[L2-1][i] == "")
continue;
else if(L3 == i)
tab_code += '<td><div class="tabclick"><span>'+tabtitle[L2-1][i]+'</span></div></td>';
else
tab_code += '<td><a href="'+tablink[L2-1][i]+'"><div class="tab"><span>'+tabtitle[L2-1][i]+'</span></div></a></td>';
}
tab_code += '</tr></table>\n';
}
$("tabMenu").innerHTML = tab_code;
}
cal_height();

        if(notice_acpw_is_default == 1){        //case1
                notification.array[0] = 'noti_acpw';
                notification.acpw = 1;
                notification.desc[0] = '<%tcWebApi_get("String_Entry","ASUSGATE_note1","s")%>';
                notification.action_desc[0] = '<%tcWebApi_get("String_Entry","ASUSGATE_act_change","s")%>';
                notification.clickCallBack[0] = "location.href = '/Advanced_System_Content.asp';"
        }else
                notification.acpw = 0;

        if(webs_state_flag==1 || webs_state_flag==2){     //case2
                notification.array[1] = 'noti_upgrade';
                notification.upgrade = 1;
                notification.desc[1] = '<%tcWebApi_get("String_Entry","ASUSGATE_note2","s")%>';
                if(live_update_support == -1 || HTTPS_support == -1){
                	notification.action_desc[1] = '<a id="link_to_downlodpage" target="_blank" href="'+get_helplink()+'" style="color:#FFCC00;"><%tcWebApi_get("String_Entry","ASUSGATE_act_update","s")%></a>';
					notification.clickCallBack[1] = "";
                }
                else{
                	notification.action_desc[1] = '<%tcWebApi_get("String_Entry","ASUSGATE_act_update","s")%>';
                	notification.clickCallBack[1] = "location.href = '/Advanced_FirmwareUpgrade_Content.asp?confirm_show=0'";
                }	
        }else
                notification.upgrade = 0;

        if(wl_info.band2g_support && noti_auth_mode_2g == 'OPEN'){ //case3-1
                        notification.array[2] = 'noti_wifi_2g';
                        notification.wifi_2g = 1;
                        notification.desc[2] = '<%tcWebApi_get("String_Entry","ASUSGATE_note3","s")%>';
                        notification.action_desc[2] = '<%tcWebApi_get("String_Entry","ASUSGATE_act_change","s")%> (2.4GHz)';
                        notification.clickCallBack[2] = "change_wl_unit_status(0);";
        }else
                notification.wifi_2g = 0;

        if(wl_info.band5g_support && noti_auth_mode_5g == 'OPEN'){      //case3-2
                        notification.array[3] = 'noti_wifi_5g';
                        notification.wifi_5g = 1;
                        notification.desc[3] = '<%tcWebApi_get("String_Entry","ASUSGATE_note3","s")%>';
                        notification.action_desc[3] = '<%tcWebApi_get("String_Entry","ASUSGATE_act_change","s")%> (5GHz)';
                        notification.clickCallBack[3] = "change_wl_unit_status(1);";
        }else
                notification.wifi_5g = 0;
                         
	//st_ftp_force_mode  0: reset default 1:force changed 2:User changed
        if(usb_support != '-1' && enable_ftp == 'Yes' && st_ftp_mode == 1  && st_ftp_force_mode == ''){ //case4_1
                        notification.array[4] = 'noti_ftp';
                        notification.ftp = 1;
                        notification.desc[4] = '<%tcWebApi_get("String_Entry","ASUSGATE_note4_1","s")%>';
                        notification.action_desc[4] = '<%tcWebApi_get("String_Entry","w_r_suggestion_etc","s")%>';
                        notification.clickCallBack[4] = "showLoading();setTimeout('document.noti_ftp.submit();', 1);setTimeout('notification.redirectftp()', 2000);";
        }else if(usb_support != '-1' && enable_ftp == 'Yes' && st_ftp_mode != 2){ //case4
                        notification.array[4] = 'noti_ftp';
                        notification.ftp = 1;
                        notification.desc[4] = '<%tcWebApi_get("String_Entry","ASUSGATE_note4","s")%>';
                        notification.action_desc[4] = '<%tcWebApi_get("String_Entry","ASUSGATE_act_change","s")%>';
                        notification.clickCallBack[4] = "showLoading();setTimeout('document.noti_ftp.submit();', 1);setTimeout('notification.redirectftp()', 2000);";
        }else
                notification.ftp = 0;
			
	if(usb_support != '-1' && enable_samba == "Yes" && st_samba_mode == 2 && st_samba_force_mode == 1 ){ //case5_1
			notification.array[5] = 'noti_samba';
			notification.samba = 1;
			notification.desc[5] = '<%tcWebApi_get("String_Entry","ASUSGATE_note5_1","s")%>';
			notification.action_desc[5] = '<%tcWebApi_get("String_Entry","w_r_suggestion_etc","s")%>';	
			notification.clickCallBack[5] = "showLoading();setTimeout('document.noti_samba.submit();', 1);setTimeout('notification.redirectsamba()', 2000);";
	}else if(usb_support != '-1' && enable_samba == "Yes" && st_samba_mode != 4){     //case5
                        notification.array[5] = 'noti_samba';
                        notification.samba = 1;
                        notification.desc[5] = '<%tcWebApi_get("String_Entry","ASUSGATE_note5","s")%>';
                        notification.action_desc[5] = '<%tcWebApi_get("String_Entry","ASUSGATE_act_change","s")%>';
                        notification.clickCallBack[5] = "showLoading();setTimeout('document.noti_samba.submit();', 1);setTimeout('notification.redirectsamba()', 2000);";
        }else
                notification.samba = 0;
                
		//Higher priority: DLA intervened case dsltmp_dla_modified 0: default / 1:need to feedback / 2:Feedback submitted 
		//Lower priority:  dsl_loss_sync  0: default 1:need to feedback 2:Feedback submitted
		if(dsl_loss_sync == 1 && feedback_support != -1){				//case9(case10 act) + case6

			notification.loss_sync = 1;
			if(dla_modified == 1){
				notification.array[9] = 'noti_dla_modified';
				notification.desc[9] = Untranslated.ASUSGATE_note9;
				notification.action_desc[9] = Untranslated.ASUSGATE_DSL_setting;
				notification.clickCallBack[9] = "location.href = '/Advanced_ADSL_Content.asp?af=dslx_dla_enable';";
				notification.array[10] = 'noti_dla_modified_fb';
				notification.action_desc[10] = Untranslated.ASUSGATE_act_feedback;
				notification.clickCallBack[10] = "location.href = '/Advanced_Feedback.asp';";
			}
			else{
				notification.array[6] = 'noti_loss_sync';
				notification.desc[6] = Untranslated.ASUSGATE_note6;
				notification.action_desc[6] = Untranslated.ASUSGATE_act_feedback;
				notification.clickCallBack[6] = "location.href = '/Advanced_Feedback.asp';"
			}
		}
		else
			notification.loss_sync = 0;
								
		//experiencing DSL issue experience_DSL=null&0: default, 1:no display again.
		if((experience_DSL_fb == 0 || experience_DSL_fb == "") && feedback_support != -1){		//case7
			notification.array[7] = 'noti_experience_DSL';
			notification.array[12] = 'noti_experience_DSL_cancel';
			notification.experience_DSL = 1;
			notification.desc[7] = Untranslated.ASUSGATE_note7;						
			notification.action_desc[7] = Untranslated.ASUSGATE_act_feedback;
			notification.clickCallBack[7] = "setTimeout('document.noti_experience_DSL.submit();', 1);setTimeout('notification.redirectFeedback()', 1000);";
			notification.action_desc[12] = '<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>';
			notification.clickCallBack[12] = "setTimeout('document.noti_experience_DSL.submit();', 1);setTimeout('notification.redirectRefresh()', 1000);";
			
		}
		else
			notification.experience_DSL = 0;					
								
		//Notification hint-- null&0: default, 1:display info
		/* Hide this function. Unsafe.
		if(noti_notif_Flag == 1 && notif_msg != ""){		//case8
			notification.array[8] = 'noti_notif_hint';
			notification.notif_hint = 1;
			notification.desc[8] = notif_msg;
			notification.action_desc[8] = "OK";
			notification.clickCallBack[8] = "setTimeout('document.noti_notif_hint.submit();', 1);setTimeout('notification.notiClick()', 100);"
		}else
			notification.notif_hint = 0;
		*/

	//DLA send debug log  -- 4: send by manual, else: nothing to show
	if(dsl_diag_support >= 0 && wan_diag_state == "4"){               //case11
		notification.array[11] = 'noti_send_debug_log';
		notification.send_debug_log = 1;
		notification.desc[11] = "-	The debug log of diagnostic DSL / Wi-Fi captured.";
		notification.action_desc[11] = "Send debug log now";
		notification.clickCallBack[11] = "setTimeout('notification.redirectFeedbackInfo()', 1000);";
	
	}else
		notification.send_debug_log = 0;

	//qis_notification_iptv  1: to show re-setup QIS for IPTV setting 0: no show
	if(iptv_support != -1 && qis_notification_iptv == "1"){              //case13 & case14
		notification.array[13] = 'noti_iptv';
		notification.array[14] = 'noti_iptv_cancel';
		notification.notif_iptv = 1;
		notification.desc[13] = 'Currently configured IPTV setting might not work, please go through Quick Internet Setup(QIS) again.';	/* untranslated */
		notification.action_desc[13] = '<%tcWebApi_Get("String_Entry", "btn_go", "s")%>';
		notification.clickCallBack[13] = "setTimeout('notification.redirectQIS()', 1000);";
		notification.action_desc[14] = '<%tcWebApi_get("String_Entry","CTL_Cancel","s")%>';
		notification.clickCallBack[14] = "setTimeout('document.qis_notification_iptv_form.submit();', 1);setTimeout('notification.redirectRefresh()', 1000);";
	}
	else
		notification.notif_iptv = 0;
                        
        if( notification.acpw || notification.upgrade || notification.wifi_2g || notification.wifi_5g || notification.ftp || notification.samba || notification.loss_sync || notification.experience_DSL || notification.notif_hint || notification.send_debug_log || notification.notif_iptv){
                notification.stat = "on";
                notification.flash = "on";
                notification.run();
        }


}

function get_helplink(){
	var model_name_supportsite = supportsite_model(country_code, odm_productid);	// @ /js/support_site.js

	var getlink="https://www.asus.com/Networking/" +model_name_supportsite+ "/HelpDesk_Download/";	
	return getlink;
}

function Block_chars(obj, keywordArray){
	// bolck ascii code 32~126 first
	var invalid_char = "";
	for(var i = 0; i < obj.value.length; ++i){
		if(obj.value.charCodeAt(i) < '32' || obj.value.charCodeAt(i) > '126'){
			invalid_char += obj.value.charAt(i);
		}
	}
	if(invalid_char != ""){
		alert('<%tcWebApi_get("String_Entry","JS_validstr2","s")%>" '+ invalid_char +'" !');
		obj.focus();
		return false;
	}

	// check if char in the specified array
	if(obj.value){
		for(var i=0; i<keywordArray.length; i++){
				if( obj.value.indexOf(keywordArray[i]) >= 0){
					alert(keywordArray+ " are invalid characters.");
					obj.focus();
					return false;
				}
		}
	}

	return true;
}

function cal_height(){
	var tableClientHeight;
	var optionHeight = 52;
	var manualOffSet = 25;
	var table_height = Math.round(optionHeight*calculate_height - manualOffSet*calculate_height/14 - document.getElementById("tabMenu").clientHeight);	
	
	if(navigator.userAgent.search("MSIE 8") > -1 || navigator.userAgent.search("MSIE 9") > -1 || navigator.userAgent.search("MSIE 10") > -1)
		var contentObj = document.querySelectorAll('.content');
	else if(navigator.appName.indexOf("Microsoft") < 0)
		var contentObj = document.getElementsByClassName("content");
	else
		var contentObj = getElementsByClassName_iefix("table", "content");

	if(document.getElementById("FormTitle") && current_url.indexOf("Advanced_AiDisk_ftp") != 0 && current_url.indexOf("Advanced_AiDisk_samba") != 0 && current_url.indexOf("QoS_EZQoS") != 0){
		table_height = table_height + 24;
		document.getElementById("FormTitle").style.height = table_height + "px";
		tableClientHeight = $("FormTitle").clientHeight;
	}
	// index.asp
	else if(document.getElementById("NM_table")){
		var statusPageHeight = 720;
		if(table_height < 800)
			table_height = 800;
		document.getElementById("NM_table").style.height = table_height + "px";

		tableClientHeight = document.getElementById("NM_table").clientHeight;
	}
	// aidisk.asp
	else if(document.getElementById("AiDiskFormTitle")){
		table_height = table_height + 24;
		document.getElementById("AiDiskFormTitle").style.height = table_height + "px";
		tableClientHeight = document.getElementById("AiDiskFormTitle").clientHeight;
	}
	// APP Install
	else if(document.getElementById("applist_table")){
		if(sw_mode == 2 || sw_mode == 3 || sw_mode == 4)
			table_height = table_height + 120;				
		else
			table_height = table_height + 40;	//2
		
		document.getElementById("applist_table").style.height = table_height + "px";		
		tableClientHeight = document.getElementById("applist_table").clientHeight;
		
		if(navigator.appName.indexOf("Microsoft") >= 0)
			contentObj[0].style.height = contentObj[0].clientHeight + 18 + "px";
	}
	// PrinterServ
	else if(document.getElementById("printerServer_table")){
		if(sw_mode == 2 || sw_mode == 4)
			table_height = table_height + 90;
		else
			table_height = table_height + 2;
		
		document.getElementById("printerServer_table").style.height = table_height + "px";		
		tableClientHeight = document.getElementById("printerServer_table").clientHeight;
		
		if(navigator.appName.indexOf("Microsoft") >= 0)
			contentObj[0].style.height = contentObj[0].clientHeight + 18 + "px";
	}
}

function show_footer(){		
	var href_lang = get_supportsite_lang();		//Not ready in support site for now 2014.05
	footer_code = '<div align="center" class="bottom-image"></div>\n';
	footer_code +='<div align="center" class="copyright"><%tcWebApi_get("String_Entry","footer_copyright_desc","s")%></div><br>';

	// FAQ searching bar{
	footer_code += '<div style="margin-top:-75px;margin-left:205px;"><table width="765px" border="0" align="center" cellpadding="0" cellspacing="0"><tr>';
	footer_code += '<td width="20" align="right"><div id="bottom_help_icon" style="margin-right:3px;"></div></td><td width="110" id="bottom_help_title" align="left"><%tcWebApi_get("String_Entry","Help","s")%> & <%tcWebApi_get("String_Entry","Support","s")%></td>';
	
	var model_name_supportsite = supportsite_model(country_code, odm_productid);	//  @/js/support_site.js
	
	var tracing_path_Manual = "/HelpDesk_Manual/?utm_source=asus-product&utm_medium=referral&utm_campaign=router";
	var tracing_path_Utility = "/HelpDesk_Download/?utm_source=asus-product&utm_medium=referral&utm_campaign=router";		
	
	footer_code += "<td width=\"335\" id=\"bottom_help_link\" align=\"left\">&nbsp&nbsp<a style=\"font-weight: bolder;text-decoration:underline;cursor:pointer;\" href=\"https://www.asus.com/Networking/" + model_name_supportsite + tracing_path_Manual + "\" target=\"_blank\"><%tcWebApi_get("String_Entry","Manual","s")%></a>&nbsp";
	footer_code += '|&nbsp<a style="font-weight: bolder;text-decoration:underline;cursor:pointer;" href="https://www.asus.com/Networking/' + model_name_supportsite + tracing_path_Utility + '" target="_blank"><%tcWebApi_get("String_Entry","Utility","s")%></a>';	
	if(feedback_support != -1)
		footer_code += '&nbsp|&nbsp<a href="/Advanced_Feedback.asp" style="font-weight: bolder;text-decoration:underline;cursor:pointer;" target="_self"><%tcWebApi_get("String_Entry","menu_dsl_feedback","s")%></a>';

	footer_code += '&nbsp|&nbsp<a id="registration_link" target="_blank" href="https://account.asus.com/" target="_self" style="font-weight:bolder;text-decoration:underline;cursor:pointer;"><%tcWebApi_get("String_Entry","Product_Registration","s")%></a>';
	footer_code += '</td>';

	footer_code += '<td width="270" id="bottom_help_FAQ" align="right" style="font-family:Arial, Helvetica, sans-serif;">FAQ&nbsp&nbsp<input type="text" id="FAQ_input" name="FAQ_input" class="input_FAQ_table" maxlength="40"></td>';
	footer_code += '<td width="30" align="left"><div id="bottom_help_FAQ_icon" class="bottom_help_FAQ_icon" style="cursor:pointer;margin-left:3px;" target="_blank" onClick="search_supportsite();"></div></td>';
	footer_code += '</tr></table></div>\n';

	$("footer").innerHTML = footer_code;
}

function get_supportsite_lang(obj){
	var faqLang = {
		selectedLang : get_selected_language()
	}	
	
	var href_lang = faqLang.selectedLang;
	return href_lang;
}

function search_supportsite(obj){
	var faqLang = {
		selectedLang : get_selected_language()		
	}
		//      https://www.asus.com/search/results.aspx?SearchKey=%s&SearchType=FAQ&PDLineFilter=1849
		var faq_href = "https://www.asus.com";
		faq_href += faqLang.selectedLang;
		faq_href += "search/results.aspx?SearchKey=";
		faq_href += (document.getElementById("FAQ_input")) ? document.getElementById("FAQ_input").value : '';
		faq_href += "&SearchType=FAQ&PDLineFilter=1849";
		window.open(faq_href);
}

var isFirefox = navigator.userAgent.search("Firefox") > -1;
var isOpera = navigator.userAgent.search("Opera") > -1;
var isIE8 = navigator.userAgent.search("MSIE 8") > -1;
var isiOS = navigator.userAgent.search("iP") > -1;
function browser_compatibility(){
	if(isiOS){
		obj_inputBtn = document.getElementsByClassName("button_gen");
		for(var i=0; i<obj_inputBtn.length; i++){
			obj_inputBtn[i].addEventListener('touchstart', function(){this.className = 'button_gen_touch';}, false);
			obj_inputBtn[i].addEventListener('touchend', function(){this.className = 'button_gen';}, false);
		}
		obj_inputBtn = document.getElementsByClassName("button_gen_long");
		for(var i=0; i<obj_inputBtn.length; i++){
			obj_inputBtn[i].addEventListener('touchstart', function(){this.className = 'button_gen_long_touch';}, false);
			obj_inputBtn[i].addEventListener('touchend', function(){this.className = 'button_gen_long';}, false);
		}
	}

	if(current_url.indexOf("index2") != 0 && current_url.indexOf("APP_Installation") != 0){
		try{
			// if jQuery is available
			var $container = $j("#tabMenu").parent();
			$j('<div>')
				.css({"margin-top":"-140px"})
				.append($container.children())
				.appendTo($container)
		}
		catch(e){
			var container = document.getElementById('tabMenu').parentNode;
			var newDiv = document.createElement('div');	
			newDiv.style.marginTop = "-140px";
			for(var i=0; i<container.children.length; i++){
				newDiv.appendChild(container.children[i].cloneNode(true));
			}
			container.innerHTML = "";
			container.appendChild(newDiv);
		}
	}
}
function show_top_status(){

	var ssid_status_2g = decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","wl0_ssid","s") %>');
	var topbanner_ssid_2g = handle_show_str(ssid_status_2g);	
	if(topbanner_ssid_2g.length >18){
		document.getElementById("elliptic_ssid_2g").innerHTML = extend_display_ssid(topbanner_ssid_2g)+"...";
	}
	else{
		document.getElementById("elliptic_ssid_2g").innerHTML = topbanner_ssid_2g;
	}
	document.getElementById("elliptic_ssid_2g").title = "2.4 GHz: \n"+ssid_status_2g;

	if(wl_info.band5g_support){		
		document.getElementById("elliptic_ssid_5g").style.display = "";
		var ssid_status_5g = decodeURIComponent('<% tcWebApi_char_to_ascii("WLan_Entry","wl1_ssid","s") %>');
		var topbanner_ssid_5g = handle_show_str(ssid_status_5g);
		if(topbanner_ssid_5g.length >18){	
			document.getElementById("elliptic_ssid_5g").innerHTML = extend_display_ssid(topbanner_ssid_5g)+"...";
		}
		else{
			document.getElementById("elliptic_ssid_5g").innerHTML = topbanner_ssid_5g;
		}
		document.getElementById("elliptic_ssid_5g").title = "5 GHz: \n"+ssid_status_5g;	
	}

	var firmver="<% tcWebApi_staticGet("DeviceInfo","firmver","s") %>";
	var extendno="<% tcWebApi_staticGet("DeviceInfo","extendno","s") %>";
	showtext(document.getElementById("firmver"), firmver + '_' + extendno.split("-g")[0]);
}

function extend_display_ssid(ssid){		//"&amp;"5&; "&lt;"4< ; "&gt;"4> ; "&nbsp;"6space
		//alert(ssid.substring(0,20)+" : "+ssid.substring(0,20).lastIndexOf("&nbsp;")+" <&nbsp>");
		if(ssid.substring(0,20).lastIndexOf("&nbsp;") >=15 || ssid.substring(0,20).lastIndexOf("&nbsp;") < 10 ){
				//alert(ssid.substring(0,19)+" : "+ssid.substring(0,19).lastIndexOf("&amp;")+" <&amp>");
				if(ssid.substring(0,19).lastIndexOf("&amp;") >=15 || ssid.substring(0,19).lastIndexOf("&amp;") < 11 ){
						//alert(ssid.substring(0,18)+" : "+ssid.substring(0,18).lastIndexOf("&lt;")+" <&lt>");
						if(ssid.substring(0,18).lastIndexOf("&lt;") >=15 || ssid.substring(0,19).lastIndexOf("&lt;") < 12 ){
								//alert(ssid.substring(0,18)+" : "+ssid.substring(0,18).lastIndexOf("&gt;")+" <&gt>");
								if(ssid.substring(0,18).lastIndexOf("&gt;") >=15 || ssid.substring(0,19).lastIndexOf("&gt;") < 12 ){
										//alert(ssid.substring(0,15));
										return ssid.substring(0,15);
								}
								else{
										switch(ssid.substring(0,18).lastIndexOf("&gt;")){
												case 12:
																	return ssid.substring(0,16);
																	break;
												case 13:
																	return ssid.substring(0,17);	
																	break;
												case 14:
																	return ssid.substring(0,18);		
																	break;	
										}
								}
						}
						else{
									switch(ssid.substring(0,18).lastIndexOf("&lt;")){
											case 12:
																return ssid.substring(0,16);
																break;
											case 13:
																return ssid.substring(0,17);
																break;
											case 14:
																return ssid.substring(0,18);
																break;

									}			
						}
				}
				else{
							switch (ssid.substring(0,19).lastIndexOf("&amp;")){
									case 11:
														return ssid.substring(0,16);
														break;
									case 12:
														return ssid.substring(0,17);
														break;
									case 13:
														return ssid.substring(0,18);
														break;
									case 14:
														return ssid.substring(0,19);
														break;

							}
				}
		}
		else{
					switch (ssid.substring(0,20).lastIndexOf("&nbsp;")){
									case 10:			
														return ssid.substring(0,16);
														break;			
									case 11:
														return ssid.substring(0,17);
														break;
									case 12:
														return ssid.substring(0,18);
														break;
									case 13:
														return ssid.substring(0,19);
														break;
									case 14:
														return ssid.substring(0,20);
														break;					
					}
		}
}

function go_setting(page){
location.href = page;
}
function go_setting_parent(page){
parent.location.href = page;
}
function show_time(){
JS_timeObj.setTime(systime_millsec); // Add millsec to it.
JS_timeObj3 = JS_timeObj.toString();
JS_timeObj3 = checkTime(JS_timeObj.getHours()) + ":" +
checkTime(JS_timeObj.getMinutes()) + ":" +
checkTime(JS_timeObj.getSeconds());
$('systemtime').innerHTML ="<a href='/Advanced_System_Content.asp'>" + JS_timeObj3 + "</a>";
systime_millsec += 1000;
stime_ID = setTimeout("show_time();", 1000);
}
function checkTime(i)
{
if (i<10)
{i="0" + i}
return i
}
function show_loading_obj(){
var obj = $("Loading");
var code = "";
code +='<table cellpadding="5" cellspacing="0" id="loadingBlock" class="loadingBlock" align="center">\n';
code +='<tr>\n';
code +='<td width="20%" height="80" align="center"><img src="/images/loading.gif"></td>\n';
	code +='<td><span id="proceeding_main_txt"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%></span><span id="proceeding_txt" style="color:#FFFFCC;"></span></td>\n';
code +='</tr>\n';
code +='</table>\n';
code +='\n';
obj.innerHTML = code;
}
var nav;
if(navigator.appName == 'Netscape')
nav = true;
else{
nav = false;
document.onkeydown = MicrosoftEventHandler_KeyDown;
}
function MicrosoftEventHandler_KeyDown(){
return true;
}
function submit_language(selected_value){
with(document.titleForm){
	action = "language_change.asp";
	preferred_lang.value =selected_value;
	flag.value = "set_language";
	showLoading(3);
	submit();
}
}
function change_language(){
if($("select_lang").value != $("preferred_lang").value)
$("change_lang_btn").disabled = false;
else
$("change_lang_btn").disabled = true;
}
function logout(){
	if(confirm('<%tcWebApi_get("String_Entry","JS_logout","s")%>')){
	setTimeout('location = "Logout.asp";', 1);
}
}

function redirect(path){
	if(path != "")
		document.location.href = path;
	else
		document.location.href = "/index.asp";
}

function reboot(){
	with(document.titleForm){
		if(confirm("<%tcWebApi_get("String_Entry","Main_content_Login_Item7","s")%>")){
			action = "reboot.asp";
			preferred_lang.disabled = true;
			flag.disabled = true;
			if(model_name == "DSL-N66U" || model_name == "DSL-AC52U"){
				showLoading(220);
				setTimeout("redirect(location.pathname);", 220100);
			}
			else{
				showLoading(150); //90
				setTimeout("redirect(location.pathname);", 150100); //90000
			}
			document.titleForm.rebootFlag.disabled = false;
			document.titleForm.restoreFlag.disabled = false;
			stopFlag = 1;
			submit();
		}
	}
}
function kb_to_gb(kilobytes){
if(typeof(kilobytes) == "string" && kilobytes.length == 0)
return 0;
return (kilobytes*1024)/(1024*1024*1024);
}
function simpleNum(num){
if(typeof(num) == "string" && num.length == 0)
return 0;
return parseInt(kb_to_gb(num)*1000)/1000;
}
function simpleNum2(num){
if(typeof(num) == "string" && num.length == 0)
return 0;
return parseInt(num*1000)/1000;
}
function simpleNum3(num){
if(typeof(num) == "string" && num.length == 0)
return 0;
return parseInt(num)/1024;
}
function $(){
var elements = new Array();
for(var i = 0; i < arguments.length; ++i){
var element = arguments[i];
if(typeof element == 'string')
element = document.getElementById(element);
if(arguments.length == 1)
return element;
elements.push(element);
}
return elements;
}
function getElementsByName_iefix(tag, name){
var tagObjs = document.getElementsByTagName(tag);
var objsName;
var targetObjs = new Array();
var targetObjs_length;
if(!(typeof(name) == "string" && name.length > 0))
return [];
for(var i = 0, targetObjs_length = 0; i < tagObjs.length; ++i){
objsName = tagObjs[i].getAttribute("name");
if(objsName && objsName.indexOf(name) == 0){
targetObjs[targetObjs_length] = tagObjs[i];
++targetObjs_length;
}
}
return targetObjs;
}
function getElementsByClassName_iefix(tag, name){
var tagObjs = document.getElementsByTagName(tag);
var objsName;
var targetObjs = new Array();
var targetObjs_length;
if(!(typeof(name) == "string" && name.length > 0))
return [];
for(var i = 0, targetObjs_length = 0; i < tagObjs.length; ++i){
if(navigator.appName == 'Netscape')
objsName = tagObjs[i].getAttribute("class");
else
objsName = tagObjs[i].getAttribute("className");
if(objsName == name){
targetObjs[targetObjs_length] = tagObjs[i];
++targetObjs_length;
}
}
return targetObjs;
}
function showtext(obj, str){
if(obj)
obj.innerHTML = str;//*/
}
function showhtmlspace(ori_str){
var str = "", head, tail_num;
head = ori_str;
while((tail_num = head.indexOf(" ")) >= 0){
str += head.substring(0, tail_num);
str += "&nbsp;";
head = head.substr(tail_num+1, head.length-(tail_num+1));
}
str += head;
return str;
}
function showhtmland(ori_str){
var str = "", head, tail_num;
head = ori_str;
while((tail_num = head.indexOf("&")) >= 0){
str += head.substring(0, tail_num);
str += "&amp;";
head = head.substr(tail_num+1, head.length-(tail_num+1));
}
str += head;
return str;
}
function translate(str){
return str;
}
function trim(val){
val = val+'';
for (var startIndex=0;startIndex<val.length && val.substring(startIndex,startIndex+1) == ' ';startIndex++);
for (var endIndex=val.length-1; endIndex>startIndex && val.substring(endIndex,endIndex+1) == ' ';endIndex--);
return val.substring(startIndex,endIndex+1);
}
function IEKey(){
return event.keyCode;
}
function NSKey(){
return event.which;
}
function is_string(o, event){
keyPressed = event.keyCode ? event.keyCode : event.which;
if(keyPressed == 0)
return true;
else if(keyPressed >= 0 && keyPressed <= 126)
return true;

	alert('<%tcWebApi_get("String_Entry","JS_validchar","s")%>');
return false;
}

function validate_hostname(obj){
        var re = new RegExp("^[a-zA-Z0-9][a-zA-Z0-9\-\_]+$","gi");
        if(re.test(obj.value)){
                return "";
        }else{
                return "<%tcWebApi_get("String_Entry","JS_validhostname","s")%>";
        }
}

function validate_account(string_obj, flag){
	var invalid_char = "";

	if(string_obj.value.charAt(0) == ' '){
		if(flag != "noalert")
			alert('<%tcWebApi_get("String_Entry","JS_validstr1","s")%> [  ]');

		string_obj.value = "";
		string_obj.focus();

		if(flag != "noalert")
			return false;
		else
			return '<%tcWebApi_get("String_Entry","JS_validstr1","s")%> [&nbsp;&nbsp;&nbsp;]';
	}
	else if(string_obj.value.charAt(0) == '-'){
		if(flag != "noalert")
			alert('<%tcWebApi_get("String_Entry","JS_validstr1","s")%> [-]');

		string_obj.value = "";
		string_obj.focus();

		if(flag != "noalert")
			return false;
		else
			return '<%tcWebApi_get("String_Entry","JS_validstr1","s")%> [-]';
	}

	for(var i = 0; i < string_obj.value.length; ++i){
		if(validate_ssidchar(string_obj.value.charCodeAt(i))){
			invalid_char = invalid_char+string_obj.value.charAt(i);
		}	
		
		if(string_obj.value.charAt(i) == '"'
				||  string_obj.value.charAt(i) == '/'
				||  string_obj.value.charAt(i) == '\\'
				||  string_obj.value.charAt(i) == '['
				||  string_obj.value.charAt(i) == ']'
				||  string_obj.value.charAt(i) == ':'
				||  string_obj.value.charAt(i) == ';'
				||  string_obj.value.charAt(i) == '|'
				||  string_obj.value.charAt(i) == '='
				||  string_obj.value.charAt(i) == ','
				||  string_obj.value.charAt(i) == '+'
				||  string_obj.value.charAt(i) == '*'
				||  string_obj.value.charAt(i) == '?'
				||  string_obj.value.charAt(i) == '<'
				||  string_obj.value.charAt(i) == '>'
				||  string_obj.value.charAt(i) == '@'
				||  string_obj.value.charAt(i) == ' '
				){
			invalid_char = invalid_char+string_obj.value.charAt(i);
		}
	}

	if(invalid_char != ""){
		if(flag != "noalert")
			alert('<%tcWebApi_get("String_Entry","JS_validstr2","s")%> '+invalid_char+' !');

		string_obj.value = "";
		string_obj.focus();

		if(flag != "noalert")
			return false;
		else
			return "<%tcWebApi_get("String_Entry","JS_validstr2","s")%> ' "+invalid_char+" ' !";
	}

	if(flag != "noalert")
		return true;
	else
		return "";
}

function validate_string(string_obj, flag){
if(string_obj.value.charAt(0) == '"'){
if(flag != "noalert")
			alert('<%tcWebApi_get("String_Entry","JS_validstr1","s")%> ["]');

string_obj.value = "";
string_obj.focus();
return false;
}
else{
invalid_char = "";
for(var i = 0; i < string_obj.value.length; ++i){
if(string_obj.value.charAt(i) < ' ' || string_obj.value.charAt(i) > '~'){
invalid_char = invalid_char+string_obj.value.charAt(i);
}
}
if(invalid_char != ""){
if(flag != "noalert")
				alert("<%tcWebApi_get("String_Entry","JS_validstr2","s")%> '"+invalid_char+"' !");
string_obj.value = "";
string_obj.focus();
return false;
}
}
return true;
}
function validate_hex(obj){
var obj_value = obj.value
var re = new RegExp("[^a-fA-F0-9]+","gi");
if(re.test(obj_value))
return false;
else
return true;
}
function validate_psk(psk_obj){
var psk_length = psk_obj.value.length;
if(psk_length < 8){
		alert("<%tcWebApi_get("String_Entry","JS_passzero","s")%>");
psk_obj.value = "00000000";
psk_obj.focus();
psk_obj.select();
return false;
}
if(psk_length > 64){
		alert("<%tcWebApi_get("String_Entry","JS_passzero","s")%>");
psk_obj.value = "00000000";
psk_obj.focus();
psk_obj.select();
return false;
}
if(psk_length >= 8 && psk_length <= 63 && !validate_string(psk_obj)){
		alert("<%tcWebApi_get("String_Entry","JS_PSK64Hex","s")%>");
psk_obj.focus();
psk_obj.select();
return false;
}
if(psk_length == 64 && !validate_hex(psk_obj)){
		alert("<%tcWebApi_get("String_Entry","JS_PSK64Hex","s")%>");
psk_obj.focus();
psk_obj.select();
return false;
}
return true;
}
function validate_wlkey(key_obj){
var wep_type = document.form.wl_wep_x.value;
var iscurrect = true;
	var str = "<%tcWebApi_get("String_Entry","JS_wepkey","s")%>";

if(wep_type == "0")
iscurrect = true; // do nothing
else if(wep_type == "1"){
if(key_obj.value.length == 5 && validate_string(key_obj)){
document.form.wl_key_type.value = 1; /*Lock Add 11.25 for ralink platform*/
iscurrect = true;
}
else if(key_obj.value.length == 10 && validate_hex(key_obj)){
document.form.wl_key_type.value = 0; /*Lock Add 11.25 for ralink platform*/
iscurrect = true;
}
else{
			str += "(<%tcWebApi_get("String_Entry","WC11b_WEPKey_itemtype1","s")%>)";

iscurrect = false;
}
}
else if(wep_type == "2"){
if(key_obj.value.length == 13 && validate_string(key_obj)){
document.form.wl_key_type.value = 1; /*Lock Add 11.25 for ralink platform*/
iscurrect = true;
}
else if(key_obj.value.length == 26 && validate_hex(key_obj)){
document.form.wl_key_type.value = 0; /*Lock Add 11.25 for ralink platform*/
iscurrect = true;
}
else{
			str += "(<%tcWebApi_get("String_Entry","WC11b_WEPKey_itemtype2","s")%>)";

iscurrect = false;
}
}
else{
alert("System error!");
iscurrect = false;
}
if(iscurrect == false){
alert(str);
key_obj.focus();
key_obj.select();
}
return iscurrect;
}
function checkDuplicateName(newname, targetArray){
var existing_string = targetArray.join(',');
existing_string = ","+existing_string+",";
var newstr = ","+trim(newname)+",";
var re = new RegExp(newstr, "gi");
var matchArray = existing_string.match(re);
if(matchArray != null)
return true;
else
return false;
}
function alert_error_msg(error_msg){
alert(error_msg);
refreshpage();
}
function refreshpage(seconds){
if(typeof(seconds) == "number")
setTimeout("refreshpage()", seconds*1000);
else
location.href = location.href;
}
function hideLinkTag(){
if(document.all){
var tagObjs = document.all.tags("a");
for(var i = 0; i < tagObjs.length; ++i)
tagObjs(i).outerHTML = tagObjs(i).outerHTML.replace(">"," hidefocus=true>");
}
}

function gotoprev(formObj){
var prev_page = formObj.prev_page.value;
if(prev_page == "/")
prev_page = "/index.asp";
if(prev_page.indexOf('QIS') < 0){
formObj.action = prev_page;
formObj.target = "_parent";
formObj.submit();
}
else{
formObj.action = prev_page;
formObj.target = "";
formObj.submit();
}
}
function add_option(selectObj, str, value, selected){
var tail = selectObj.options.length;
if(typeof(str) != "undefined")
selectObj.options[tail] = new Option(str);
else
selectObj.options[tail] = new Option();
if(typeof(value) != "undefined")
selectObj.options[tail].value = value;
else
selectObj.options[tail].value = "";
if(selected == 1)
selectObj.options[tail].selected = selected;
}
function free_options(selectObj){
if(selectObj == null)
return;
for(var i = selectObj.options.length-1; i >= 0; --i){
selectObj.options[i].value = null;
selectObj.options[i] = null;
}
}
function blocking(obj_id, show){
var state = show?'block':'none';
if(document.getElementById)
$(obj_id).style.display = state;
else if(document.layers)
document.layers[obj_id].display = state;
else if(document.all)
document.all[obj_id].style.display = state;
}
function inputCtrl(obj, flag){
	if(flag == 0){
		obj.disabled = true;
		if(obj.type != "select-one")
			obj.style.backgroundColor = "#CCCCCC";
		if(obj.type == "radio" || obj.type == "checkbox")
			obj.style.backgroundColor = "#475A5F";
		if(obj.type == "text" || obj.type == "password")
			obj.style.backgroundImage = "url(/images/New_ui/inputbg_disable.png)";
	}
	else{
		obj.disabled = false;
		if(obj.type != "select-one")
			obj.style.backgroundColor = "#FFF";
		if(obj.type == "radio" || obj.type == "checkbox")
			obj.style.backgroundColor = "#475A5F";
		if(obj.type == "text" || obj.type == "password")
			obj.style.backgroundImage = "url(/images/New_ui/inputbg.png)";
	}
	if(current_url.indexOf("Advanced_Wireless_Content") == 0
	|| current_url.indexOf("Guest_network") == 0
	|| current_url.indexOf("Advanced_PerformanceTuning_Content") == 0
	|| current_url.indexOf("Advanced_Modem_Content") == 0
	|| current_url.indexOf("Advanced_IPv6_Content") == 0
	|| current_url.indexOf("Advanced_WAdvanced_Content") == 0
	|| current_url.indexOf("Advanced_IPTV_Content") == 0
	|| current_url.indexOf("Advanced_WANPort_Content.asp") == 0
	|| current_url.indexOf("Advanced_ASUSDDNS_Content.asp") == 0
	|| current_url.indexOf("Advanced_DSL_Content.asp") == 0
	|| current_url.indexOf("Advanced_SwitchCtrl_Content.asp") == 0
	|| current_url.indexOf("Main_WebHistory_Content.asp") == 0
	|| current_url.indexOf("Main_AccessLog_Content.asp") == 0
	|| current_url.indexOf("router.asp") == 0
	){
		if(obj.type == "checkbox")
			return true;
		if(flag == 0)
			obj.parentNode.parentNode.style.display = "none";
		else
			obj.parentNode.parentNode.style.display = "";
		return true;
	}
}
function inputHideCtrl(obj, flag){
if(obj.type == "checkbox")
return true;
if(flag == 0)
obj.parentNode.parentNode.style.display = "none";
else
obj.parentNode.parentNode.style.display = "";
return true;
}

//Update current system status
var AUTOLOGOUT_MAX_MINUTE = "";
if("<% tcWebApi_Get("Misc_Entry", "http_autologout", "s") %>" != "")
	AUTOLOGOUT_MAX_MINUTE = parseInt("<% tcWebApi_Get("Misc_Entry", "http_autologout", "s") %>") * 20;
else
	AUTOLOGOUT_MAX_MINUTE = 30 * 20;
function updateStatus(){
	if(stopFlag == 1) return false;
	if(AUTOLOGOUT_MAX_MINUTE == 1) location = "Main_Login.asp"; // 0:disable auto logout, 1:trigger auto logout. 

	require(['/require/modules/makeRequest.js'], function(makeRequest){
		if(AUTOLOGOUT_MAX_MINUTE != 0) AUTOLOGOUT_MAX_MINUTE--;
		makeRequest.start('/cgi-bin/ajax_status.xml', refreshStatus, function(){stopFlag = 1;});
	});
}

var link_status;
var link_auxstatus;
var link_sbstatus;
var usb_path1;
var usb_path2;
var usb_path1_tmp = "init";
var usb_path2_tmp = "init";
var usb_path1_removed;
var usb_path2_removed;
var wifi_hw_switch;
var usb_path1_removed_tmp = "init";
var usb_path2_removed_tmp = "init";
var ddns_return_code = '<% tcWebApi_Get("Vram_Entry", "ddns_return_code_chk", "s") %>';
var ddns_updated = '<% tcWebApi_Get("Vram_Entry", "ddns_updated", "s") %>';
var first_link_status = '';
var first_link_sbstatus = '';
var first_link_auxstatus = '';
var secondary_link_status = '';
var secondary_link_sbstatus = '';
var secondary_link_auxstatus = '';

var vpnc_state_t = '';
var vpnc_sbstate_t = '';
var vpnc_proto = '<% tcWebApi_Get("VPNC_Entry", "vpnc_proto", "s") %>';
var vpnd_state;
var vpnc_state_t1 = '';
var vpnc_state_t2 = '';
var vpnc_state_t3 = '';
var vpnc_state_t4 = '';
var vpnc_state_t5 = '';
var vpnc_errno_t1 = '';
var vpnc_errno_t2 = '';
var vpnc_errno_t3 = '';
var vpnc_errno_t4 = '';
var vpnc_errno_t5 = '';

var modem_enable = '';
var wanConnectStatus = true;

if(usb_support != -1){
	var tmp_mount_0 = foreign_disk_total_mounted_number()[0];		//Viz 2013.07
	var tmp_mount_1 = foreign_disk_total_mounted_number()[1];		//Viz 2013.07
}

function refreshStatus(xhr)
{
	setTimeout(function(){updateStatus();}, 3000);	/* restart ajax */
	
	var devicemapXML = xhr.responseXML.getElementsByTagName("devicemap");
	var SysStatus = devicemapXML[0].getElementsByTagName("sys");
	uptimeStr_update = SysStatus[0].firstChild.nodeValue.replace("uptimeStr=", "");
	boottime_update = parseInt(uptimeStr_update.substring(32,42)); 
	var wanStatus = devicemapXML[0].getElementsByTagName("wan");
	link_status = wanStatus[0].firstChild.nodeValue;
	link_sbstatus = wanStatus[1].firstChild.nodeValue;
	link_auxstatus = wanStatus[2].firstChild.nodeValue;
	first_link_status = wanStatus[0].firstChild.nodeValue;
	first_link_sbstatus = wanStatus[1].firstChild.nodeValue;
	first_link_auxstatus = wanStatus[2].firstChild.nodeValue;		
	var secondary_wanStatus = devicemapXML[0].getElementsByTagName("secondary_wan");
	secondary_link_status = secondary_wanStatus[0].firstChild.nodeValue;
	secondary_link_sbstatus = secondary_wanStatus[1].firstChild.nodeValue;
	secondary_link_auxstatus = secondary_wanStatus[2].firstChild.nodeValue;

	usb_path1 = wanStatus[3].firstChild.nodeValue;
	usb_path2 = wanStatus[4].firstChild.nodeValue;
	monoClient = wanStatus[5].firstChild.nodeValue;
	cooler = wanStatus[6].firstChild.nodeValue;
	_wlc_state = wanStatus[7].firstChild.nodeValue;
	_wlc_sbstate = wanStatus[8].firstChild.nodeValue;
	wifi_hw_switch = wanStatus[9].firstChild.nodeValue;
	usb_path1_removed = wanStatus[11].firstChild.nodeValue;
	usb_path2_removed = wanStatus[12].firstChild.nodeValue;
	ddns_return_code = wanStatus[13].firstChild.nodeValue.replace("ddnsRet=", "");
	ddns_updated = wanStatus[14].firstChild.nodeValue.replace("ddnsUpdate=", "");
	wan_line_state = wanStatus[15].firstChild.nodeValue.replace("wan_line_state=", "");
	wlan0_radio_flag = wanStatus[16].firstChild.nodeValue.replace("wlan0_radio_flag=", "");
	wlan1_radio_flag = wanStatus[17].firstChild.nodeValue.replace("wlan1_radio_flag=", "");
	wan_diag_state = wanStatus[18].firstChild.nodeValue.replace("wan_diag_state=", "");
	
	var vpnStatus = devicemapXML[0].getElementsByTagName("vpn");									
	vpnc_proto = vpnStatus[0].firstChild.nodeValue.replace("vpnc_proto=", "");		
	if(vpnc_support != -1){
		vpnc_state_t1 = vpnStatus[3].firstChild.nodeValue.replace("vpn_client1_state=", "");
		vpnc_errno_t1 = vpnStatus[9].firstChild.nodeValue.replace("vpn_client1_errno=", "");
		vpnc_state_t2 = vpnStatus[4].firstChild.nodeValue.replace("vpn_client2_state=", "");
		vpnc_errno_t2 = vpnStatus[10].firstChild.nodeValue.replace("vpn_client2_errno=", "");
		vpnc_state_t3 = vpnStatus[5].firstChild.nodeValue.replace("vpn_client3_state=", "");
		vpnc_errno_t3 = vpnStatus[11].firstChild.nodeValue.replace("vpn_client3_errno=", "");
		vpnc_state_t4 = vpnStatus[6].firstChild.nodeValue.replace("vpn_client4_state=", "");
		vpnc_errno_t4 = vpnStatus[12].firstChild.nodeValue.replace("vpn_client4_errno=", "");
		vpnc_state_t5 = vpnStatus[7].firstChild.nodeValue.replace("vpn_client5_state=", "");
		vpnc_errno_t5 = vpnStatus[13].firstChild.nodeValue.replace("vpn_client5_errno=", "");
		vpnc_state_t = vpnStatus[1].firstChild.nodeValue.replace("vpnc_state_t=", "");
	}

	vpnc_sbstate_t = vpnStatus[2].firstChild.nodeValue.replace("vpnc_sbstate_t=", "");	
	vpnd_state = vpnStatus[8].firstChild.nodeValue;

	var usbStatus = devicemapXML[0].getElementsByTagName("usb");
	modem_enable = usbStatus[0].firstChild.nodeValue.replace("modem_enable=", "");

	if(current_url.indexOf("QIS_wizard.asp") == 0)
		return false;
	else if(current_url.indexOf("Advanced_VPNClient_Content.asp") == 0)
		show_vpnc_rulelist();
	else if(current_url.indexOf("Advanced_Feedback.asp") == 0 || 
			current_url.indexOf("Main_WebHistory_Content.asp") == 0 ||
			current_url.indexOf("Main_AccessLog_Content.asp") == 0){
		updateUSBStatus();
	}
		
	//internet	
	if(sw_mode == 1){
		//Viz add 2013.03 for adsl sync status
		if(wan_diag_state == "1" && 
				(Diag2jffs_support ||
				 ((rc_support.search("usbX1") >= 0 && usb_path1.search("storage") >= 0) || 
				 (rc_support.search("usbX2") >= 0 && (usb_path1.search("storage") >= 0 || usb_path2.search("storage") >= 0)))
				)
		){
			document.getElementById("adsl_line_status").className = "linestatusdiag";
			document.getElementById("adsl_line_status").onclick = function(){openHint(24,8);}
		}
		else if(wan_line_state == "up"){
			document.getElementById("adsl_line_status").className = "linestatusup";
			document.getElementById("adsl_line_status").onclick = function(){openHint(24,6);}
		}else if(wan_line_state == "wait for init"){
			document.getElementById("adsl_line_status").className = "linestatuselse";
		}else if(wan_line_state == "initializing"){
			document.getElementById("adsl_line_status").className = "linestatuselse";
		}else{
			document.getElementById("adsl_line_status").className = "linestatusdown";
		}
		document.getElementById("adsl_line_status").onmouseover = function(){overHint(9);}
		document.getElementById("adsl_line_status").onmouseout = function(){nd();}

		if((link_status == "2" && link_auxstatus == "0") || (link_status == "2" && link_auxstatus == "2")){
			document.getElementById("connect_status").className = "connectstatuson";
			if(current_url == "index2.asp"){
				document.getElementById("NM_connect_status").innerHTML = "<%tcWebApi_get("String_Entry","Connected","s")%>";
				document.getElementById('single_wan').className = "single_wan_connected";	
			}
			wanConnectStatus = true;
		}
		else if(dualwan_enabled &&
				((first_link_status == "2" && first_link_auxstatus == "0") || (first_link_status == "2" && first_link_auxstatus == "2")) ||
				((secondary_link_status == "2" && secondary_link_auxstatus == "0") || (secondary_link_status == "2" && secondary_link_auxstatus == "2"))){
			document.getElementById("connect_status").className = "connectstatuson";
			if(current_url == "index2.asp"){
				document.getElementById("NM_connect_status").innerHTML = "<%tcWebApi_get("String_Entry","Connected","s")%>";
				document.getElementById('single_wan').className = "single_wan_connected";
			}
			wanConnectStatus = true;
		}
		else{
			document.getElementById("connect_status").className = "connectstatusoff";
			if(current_url == "index2.asp"){
			  document.getElementById("NM_connect_status").innerHTML = "<%tcWebApi_get("String_Entry","Disconnected","s")%>";
			  document.getElementById('single_wan').className = "single_wan_disconnected";
			  document.getElementById("wanIP_div").style.display = "none";
			} 
			wanConnectStatus = false;
		}

		<%If tcWebApi_staticGet("DeviceInfo_PVC","DispBtnType","h") <> "0" then%>
			document.getElementById("connect_status").onclick = function(){openHint(24,3);}
		<%elseif tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_Manually" then%>
			<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "0" then%>
				<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "ATM" then%>
					document.getElementById("connect_status").onclick = function(){openHint(24,3);}
				<%End if%>
			<%End if%>
			<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
				<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then%>
					<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "PTM" then%>
						document.getElementById("connect_status").onclick = function(){openHint(24,3);}
					<%End if%>
				<%End if%>
				<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "9" then%>
					<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "PTM" then%>
						document.getElementById("connect_status").onclick = function(){openHint(24,3);}
					<%End if%>
				<%End if%>
			<%end if%>
			<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then%>
				<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "Ethernet" then%>
					document.getElementById("connect_status").onclick = function(){openHint(24,3);}
				<%End if%>
			<%End if%>
			<% if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "11" then%>
				<% if tcWebApi_staticGet("Wan_Common","TransMode","h") = "USB" then%>
					document.getElementById("connect_status").onclick = function(){openHint(24,3);}
				<%End if%>
			<%End if%>
		<%End if%>

		document.getElementById("connect_status").onmouseover = function(){overHint(3);}
		document.getElementById("connect_status").onmouseout = function(){nd();}
	}
	else if(sw_mode == 2){
		if(_wlc_state == "wlc_state=2"){
			document.getElementById("connect_status").className = "connectstatuson";
			document.getElementById("connect_status").onclick = function(){openHint(24,3);}
			if(location.pathname == "/" || current_url.indexOf("index.asp") == 0){
				document.getElementById("NM_connect_status").innerHTML = "<%tcWebApi_get("String_Entry","APS_msg_connected","s")%>";
				document.getElementById('single_wan').className = "single_wan_connected";
			}	
			wanConnectStatus = true;
		}
		else{
			document.getElementById("connect_status").className = "connectstatusoff";
			if(location.pathname == "/" || current_url.indexOf("index.asp") == 0){
			  document.getElementById("NM_connect_status").innerHTML = "<%tcWebApi_get("String_Entry","Disconnected","s")%>";
			  document.getElementById('single_wan').className = "single_wan_disconnected";
			}  
			wanConnectStatus = false;
		}
		document.getElementById("connect_status").onmouseover = function(){overHint(3);}
		document.getElementById("connect_status").onmouseout = function(){nd();}
	}
	
	//wifi
	if(wifi_hw_sw_support != -1){
		if(wl_info.band5g_support){
			if(wlan0_radio_flag == "0" && wlan1_radio_flag == "0"){
				document.getElementById("wifi_hw_sw_status").className = "wifihwswstatusoff";
				document.getElementById("wifi_hw_sw_status").onclick = function(){}
			}
			else{
				document.getElementById("wifi_hw_sw_status").className = "wifihwswstatuson";
				document.getElementById("wifi_hw_sw_status").onclick = function(){}
			}
		}
		else{
			if(wlan0_radio_flag == "0"){
				document.getElementById("wifi_hw_sw_status").className = "wifihwswstatusoff";
				document.getElementById("wifi_hw_sw_status").onclick = function(){}
			}
			else{
				document.getElementById("wifi_hw_sw_status").className = "wifihwswstatuson";
				document.getElementById("wifi_hw_sw_status").onclick = function(){}
			}			
		}
		
		document.getElementById("wifi_hw_sw_status").onmouseover = function(){overHint(8);}
		document.getElementById("wifi_hw_sw_status").onmouseout = function(){nd();}
	}
		
	if(usb_support != -1){		
		if(current_url=="index2.asp"||current_url==""){
			//umount= : usbmount  ;  umount=1 : no usb mount
			if((usb_path1_removed != usb_path1_removed_tmp && usb_path1_removed_tmp != "init")){
				location.href = "index2.asp";
			}
			else if(usb_path1_removed == "umount="){
				if((usb_path1 != usb_path1_tmp && usb_path1_tmp != "init"))
					location.href = "index2.asp";
			}

			if((usb_path2_removed != usb_path2_removed_tmp && usb_path2_removed_tmp != "init")){
				location.href = "index2.asp";
			}
			else if(usb_path2_removed == "umount="){
				if((usb_path2 != usb_path2_tmp && usb_path2_tmp != "init"))
					location.href = "index2.asp";
			}
		}
		
		if(usb_path1_removed == "umount=1")
			usb_path1 = "usb=";

		if(usb_path2_removed == "umount=1")
			usb_path2 = "usb=";

		// usb.storage&modem
		document.getElementById("usb_status").onmouseover = function(){overHint(2);}
		document.getElementById("usb_status").onmouseout = function(){nd();}
		
		if((usb_path1 == "usb=" || usb_path1 == "usb=printer") && (usb_path2 == "usb=" || usb_path2 == "usb=printer")){	//without usb.storage&modem plug-in
			document.getElementById("usb_status").className = "usbstatusoff";
		}
		else{	//with usb.storage&modem plug-in			
			document.getElementById("usb_status").className = "usbstatuson";
			
			if(usb_path1 == "usb=storage" || usb_path2 == "usb=storage"){		//find_storage
				document.getElementById("usb_status").onclick = function(){openHint(24,2);}				
			}
			else if(modem_support && (usb_path1 == "usb=modem" || usb_path2 == "usb=modem")){		//find_modem
				document.getElementById("usb_status").onclick = function(){openHint(24,7);}	
			}			
			else{
				document.getElementById("usb_status").onclick = function(){overHint(2);}			
			}
		}		
		
		if(printer_support != -1){					
			if(usb_path1 != "usb=printer" && usb_path2 != "usb=printer"){	//without printer plug-in
				document.getElementById("printer_status").className = "printstatusoff";
				document.getElementById("printer_status").parentNode.style.display = "none";
				document.getElementById("printer_status").onmouseover = function(){overHint(5);}
				document.getElementById("printer_status").onmouseout = function(){nd();}
			}
			else{	//at least one printer plug-in				
				document.getElementById("printer_status").className = "printstatuson";
				document.getElementById("printer_status").onmouseover = function(){overHint(6);}
				document.getElementById("printer_status").onmouseout = function(){nd();}
				document.getElementById("printer_status").onclick = function(){openHint(24,1);}				
			}
		}
		
		usb_path1_tmp = usb_path1;
		usb_path2_tmp = usb_path2;
		usb_path1_removed_tmp = usb_path1_removed;
		usb_path2_removed_tmp = usb_path2_removed;
	}
	
	//Guest Network
	if(multissid_support != -1 && wl_info.band5g_support){
		for(var i=0; i<gn_array_2g.length; i++){
			if(gn_array_2g[i][0] == 1 || gn_array_5g[i][0] == 1){
				document.getElementById("guestnetwork_status").className = "guestnetworkstatuson";
				document.getElementById("guestnetwork_status").onclick = function(){openHint(24,4);}
				break;
			}
			else{
				document.getElementById("guestnetwork_status").className = "guestnetworkstatusoff";
				document.getElementById("guestnetwork_status").onclick = function(){overHint(4);}
			}
		}
		document.getElementById("guestnetwork_status").onmouseover = function(){overHint(4);}
		document.getElementById("guestnetwork_status").onmouseout = function(){nd();}
	}
	else if(multissid_support != -1 && !wl_info.band5g_support){
		for(var i=0; i<gn_array_2g.length; i++){
			if(gn_array_2g[i][0] == 1){
				document.getElementById("guestnetwork_status").className = "guestnetworkstatuson";
				document.getElementById("guestnetwork_status").onclick = function(){openHint(24,4);}
				break;
			}
			else{
				document.getElementById("guestnetwork_status").className = "guestnetworkstatusoff";
			}
		}
		document.getElementById("guestnetwork_status").onmouseover = function(){overHint(4);}
		document.getElementById("guestnetwork_status").onmouseout = function(){nd();}
	}
	if(cooler_support != -1){
		if(cooler == "cooler=2"){
			document.getElementById("cooler_status").className = "coolerstatusoff";
			document.getElementById("cooler_status").onclick = function(){}
		}
		else{
			document.getElementById("cooler_status").className = "coolerstatuson";
			document.getElementById("cooler_status").onclick = function(){openHint(24,5);}
		}
		document.getElementById("cooler_status").onmouseover = function(){overHint(7);}
		document.getElementById("cooler_status").onmouseout = function(){nd();}
	}
	if(window.frames["statusframe"] && window.frames["statusframe"].stopFlag == 1 || stopFlag == 1) return;	
}

var notification = {
	stat: "off",
	flash: "off",
	flashTimer: 0,
	hoverText: "",
	clickText: "",
	array: [],
	desc: [],
	action_desc: [],
	upgrade: 0,
	wifi_2g: 0,
	wifi_5g: 0,
	ftp: 0,
	samba: 0,
	loss_sync: 0,
	experience_DSL: 0,
	notif_hint: 0,
	send_debug_log: 0,
	notif_iptv: 0,
	clicking: 0,
	redirectftp:function(){location.href = '/Advanced_AiDisk_ftp.asp';},
	redirectsamba:function(){location.href = '/Advanced_AiDisk_samba.asp';},
	redirectFeedback:function(){location.href = '/Advanced_Feedback.asp';},
	redirectFeedbackInfo:function(){location.href = '/Feedback_Info.asp';},
	redirectRefresh:function(){location.href = location.pathname;},
	redirectQIS:function(){location.href = '/QIS_wizard.asp';},
	clickCallBack: [],
	notiClick: function(){
		// stop flashing after the event is checked.
		cookie_help.set("notification_history", [notification.upgrade, notification.wifi_2g, notification.wifi_5g, notification.ftp, notification.samba, notification.loss_sync, notification.experience_DSL, notification.notif_hint, notification.send_debug_log, notification.notif_iptv].join(), 1000);
		clearInterval(notification.flashTimer);
		document.getElementById("notification_status").className = "notification_on";

		if(notification.clicking == 0){
			var txt = '<div id="notiDiv"><table width="100%">'

			for(i=0; i<notification.array.length; i++){
				if(notification.array[i] != null && notification.array[i] != "off"){
					txt += '<tr><td><table id="notiDiv_table3" width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#232629">';
		  			txt += '<tr><td><table id="notiDiv_table5" border="0" cellpadding="5" cellspacing="0" bgcolor="#232629" width="100%">';
		  			txt += '<tr><td valign="TOP" width="100%"><div style="white-space:pre-wrap;font-size:13px;color:white;cursor:text">' + notification.desc[i] + '</div>';
		  			txt += '</td></tr>';

		  			if( i == 2 ){			  				
		  				txt += '<tr><td width="100%"><div style="text-decoration:underline;text-align:right;color:#FFCC00;font-size:14px;cursor: pointer" onclick="' + notification.clickCallBack[i] + '">' + notification.action_desc[i] + '</div></td></tr>';
		  				if(wl_info.band5g_support && notification.array[3] != null && notification.array[i] != "off")
		  				txt += '<tr><td width="100%"><div style="text-decoration:underline;text-align:right;color:#FFCC00;font-size:14px;cursor: pointer" onclick="' + notification.clickCallBack[i+1] + '">' + notification.action_desc[i+1] + '</div></td></tr>';
		  				notification.array[3] = "off";
		  			}
		  			else if( i == 7){
		  				if(notification.array[12] != null)
							txt += '<tr><td width="100%"><div style="text-align:right;text-decoration:underline;color:#FFCC00;font-size:14px;"><span style="cursor: pointer" onclick="' + notification.clickCallBack[12] + '">' + notification.action_desc[12] + '</span>';
						notification.array[12] = "off";
		  				txt += '<span style="margin-left:10px;cursor: pointer" onclick="' + notification.clickCallBack[i] + '">' + notification.action_desc[i] + '</span></div></td></tr>';
					}
		  			else if( i == 9){
		  				txt += '<tr><td width="100%"><div style="text-decoration:underline;text-align:right;color:#FFCC00;font-size:14px;cursor: pointer" onclick="' + notification.clickCallBack[i] + '">' + notification.action_desc[i] + '</div></td></tr>';
						if(notification.array[10] != null && notification.array[10] != "off")
							txt += '<tr><td width="100%"><div style="text-decoration:underline;text-align:right;color:#FFCC00;font-size:14px;cursor: pointer" onclick="' + notification.clickCallBack[i+1] + '">' + notification.action_desc[i+1] + '</div></td></tr>';
						notification.array[10] = "off";
					}
					else if( i == 13){
						if(notification.array[14] != null)
							txt += '<tr><td width="100%"><div style="text-align:right;text-decoration:underline;color:#FFCC00;font-size:14px;"><span style="cursor: pointer" onclick="' + notification.clickCallBack[14] + '">' + notification.action_desc[14] + '</span>';
						notification.array[14] = "off";
						txt += '<span style="margin-left:10px;cursor: pointer" onclick="' + notification.clickCallBack[i] + '">' + notification.action_desc[i] + '</span></div></td></tr>';
					}
		  			else{
	  					txt += '<tr><td><table width="100%"><div style="text-decoration:underline;text-align:right;color:#FFCC00;font-size:14px;cursor: pointer" onclick="' + notification.clickCallBack[i] + '">' + notification.action_desc[i] + '</div></table></td></tr>';
		  			}

		  			txt += '</table></td></tr></table></td></tr>'
	  			}
			}
			txt += '</table></div>';

			$("notification_desc").innerHTML = txt;
			notification.clicking = 1;
		}else{
			$("notification_desc").innerHTML = "";
			notification.clicking = 0;
		}
	},
	
	run: function(){
		var tarObj = document.getElementById("notification_status");
		var tarObj1 = document.getElementById("notification_status1");

		if(tarObj === null)	
			return false;		

		if(this.stat == "on"){
			tarObj1.onclick = this.notiClick;
			tarObj.className = "notification_on";
			tarObj1.className = "notification_on1";
		}

		if(this.flash == "on" && getCookie_help("notification_history") != [notification.upgrade, notification.wifi_2g, notification.wifi_5g, notification.ftp, notification.samba, notification.loss_sync, notification.experience_DSL, notification.notif_hint, notification.send_debug_log, notification.notif_iptv].join()){
			notification.flashTimer = setInterval(function(){
				tarObj.className = (tarObj.className == "notification_on") ? "notification_off" : "notification_on";
			}, 1000);
		}
	},

	reset: function(){
		this.stat = "off";
		this.flash = "off";
		this.flashTimer = 100;
		this.hoverText = "";
		this.clickText = "";
		this.upgrade = 0;
		this.wifi_2g = 0;
		this.wifi_5g = 0;
		this.ftp = 0;
		this.samba = 0;
		this.loss_sync = 0;
		this.experience_DSL = 0;
		this.notif_hint = 0;
		this.send_debug_log = 0;
		this.notif_iptv = 0;
		this.action_desc = [];
		this.desc = [];
		this.array = [];
		this.clickCallBack = [];
		this.run();
	}
}

function db(obj){
if(typeof console == 'object')
console.log(obj);
}
function FormActions(_Action, _ActionMode, _ActionScript, _ActionWait){
	if(_Action != "")
		document.form.action = _Action;
	if(_ActionMode != "")
		document.form.action_mode.value = _ActionMode;
	if(_ActionScript != "")
		document.form.action_script.value = _ActionScript;
	if(_ActionWait != "")
		document.form.action_wait.value = _ActionWait;
}
function change_wl_unit(){
FormActions("apply.cgi", "change_wl_unit", "", "");
document.form.target = "";
document.form.submit();
}
function compareWirelessClient(target1, target2){
if(target1.length != target2.length)
return (target2.length-target1.length);
for(var i = 0; i < target1.length; ++i)
for(var j = 0; j < 3; ++j)
if(target1[i][j] != target2[i][j])
return 1;
return 0;
}
function addNewScript(scriptName){
var script = document.createElement("script");
script.type = "text/javascript";
script.src = scriptName;
document.getElementsByTagName("head")[0].appendChild(script);
}
var cookie_help = {
set: function(key, value, days) {
document.cookie = key + '=' + value + '; expires=' +
(new Date(new Date().getTime() + ((days ? days : 14) * 86400000))).toUTCString() + '; path=/';
}
};
function getCookie_help(c_name){
if (document.cookie.length > 0){
c_start=document.cookie.indexOf(c_name + "=")
if (c_start!=-1){
c_start=c_start + c_name.length+1
c_end=document.cookie.indexOf(";",c_start)
if (c_end==-1) c_end=document.cookie.length
return unescape(document.cookie.substring(c_start,c_end))
}
}
return null;
}

function unload_body(){
}

function enableCheckChangedStatus(){
}

function disableCheckChangedStatus(){
	stopFlag = 1;
}

function check_if_support_dr_surf(){
}

function check_changed_status(){
}

function get_changed_status(){
}

function set_changed_status(){
}

function set_Dr_work(){
}

function showHelpofDrSurf(){
}

function showDrSurf(){
}

function slowHide(){
}

function hideHint(){
}

function drdiagnose(){
}

function isMobile(){
	var mobiles = new Array(
								"midp", "j2me", "avant", "docomo", "novarra", "palmos", "palmsource",
								"240x320", "opwv", "chtml", "pda", "windows ce", "mmp/",
								"blackberry", "mib/", "symbian", "wireless", "nokia", "hand", "mobi",
								"phone", "cdm", "up.b", "audio", "sie-", "sec-", "samsung", "htc",
								"mot-", "mitsu", "sagem", "sony", "alcatel", "lg", "eric", "vx",
								"NEC", "philips", "mmm", "xx", "panasonic", "sharp", "wap", "sch",
								"rover", "pocket", "benq", "java", "pt", "pg", "vox", "amoi",
								"bird", "compal", "kg", "voda", "sany", "kdd", "dbt", "sendo",
								"sgh", "gradi", "jb", "dddi", "moto", "iphone", "android",
								"iPod", "incognito", "webmate", "dream", "cupcake", "webos",
								"s8000", "bada", "googlebot-mobile");
 	var ua = navigator.userAgent.toLowerCase();
	var _isMobile = false;
	for(var i = 0; i < mobiles.length; i++){
		if(ua.indexOf(mobiles[i]) > 0){
			_isMobile = true;
			break;
		}
	}

	return _isMobile;
}

var stopAutoFocus;
function autoFocus(str){
	if(str == "")
		return false;

	stopAutoFocus = 0;
	if(document.form){
		for(var i = 0; i < document.form.length; i++){
			if(document.form[i].name == str){
				var sec = 600;
				var maxAF = 20;
				if(navigator.userAgent.toLowerCase().search("webkit") < 0){
					window.onclick = function(){stopAutoFocus=1;document.form[i].style.border='';}
					for(var j=0; j<maxAF; j++){
						setTimeout("if(stopAutoFocus==0)document.form["+i+"].style.border='1px solid #56B4EF';", sec*j++);
						setTimeout("if(stopAutoFocus==0)document.form["+i+"].style.border='';", sec*j);
					}
				}
				else{
					window.onclick = function(){stopAutoFocus=1;}
					document.form[i].focus();
					for(var j=1; j<maxAF; j++){
						setTimeout("if(stopAutoFocus==0)document.form["+i+"].blur();", sec*j++);
						setTimeout("if(stopAutoFocus==0)document.form["+i+"].focus();", sec*j);
					}
				}
				break;
			}
		}
	}
}

function charToAscii(str){
	var retAscii = "";
	for(var i = 0; i < str.length; i++){
		retAscii += "%";
		retAscii += str.charCodeAt(i).toString(16).toUpperCase();
	}
	return retAscii;
}

function isPortConflict(_val){
	if(_val == '80')
		return "<%tcWebApi_get("String_Entry","portConflictHint","s")%> HTTP LAN port.";
	else if(_val == '<% TCWebApi_get("Apps_Entry","dm_http_port","s")%>')
		return "<%tcWebApi_get("String_Entry","portConflictHint","s")%> Download Master.";
	else if(_val == '<% TCWebApi_get("AiCloud_Entry","webdav_http_port","s")%>')
		return "<%tcWebApi_get("String_Entry","portConflictHint","s")%> Cloud Disk.";
	else if(_val == '<% TCWebApi_get("AoiCloud_Entry","webdav_https_port","s")%>')
		return "<%tcWebApi_get("String_Entry","portConflictHint","s")%> Cloud Disk.";
	else
		return false;
}

function get_preferred_lang_type(){
var lang_type1 = "<%tcWebApi_get("LanguageSwitch_Entry","Type","s")%>";
var lang_type2 = "<%tcWebApi_get("WebCurSet_Entry","detected_lang_type","s")%>";

	if(lang_type1 == '0')
	{
		return lang_type2;
	}
	else
	{
		return lang_type1;
	}
}

function show_selected_language(){
var lang_flag = get_preferred_lang_type();
	switch(lang_flag){
		case '1':{
			$('selected_lang').innerHTML = "English";
			$('EN').parentNode.style.display = "none";
			break;
		}
		case '3':{
			$('selected_lang').innerHTML = "简体中文";
			$('CN').parentNode.style.display = "none";
			break;
		}
		case '18':{
			$('selected_lang').innerHTML = "繁體中文";
			$('TW').parentNode.style.display = "none";
			break;
		}
		case '2':{
			$('selected_lang').innerHTML = "Português(Brazil)&nbsp&nbsp";
			$('BR').parentNode.style.display = "none";
			break;
		}
		case '4':{
			$('selected_lang').innerHTML = "Česky";
			$('CZ').parentNode.style.display = "none";
			break;
		}
		case '5':{
			$('selected_lang').innerHTML = "Dansk";
			$('DA').parentNode.style.display = "none";
			break;
		}
		case '6':{
			$('selected_lang').innerHTML = "Deutsch";
			$('DE').parentNode.style.display = "none";
			break;
		}
		case '7':{
			$('selected_lang').innerHTML = "Español";
			$('ES').parentNode.style.display = "none";
			break;
		}
		case '8':{
			$('selected_lang').innerHTML = "Suomi";
			$('FI').parentNode.style.display = "none";
			break;
		}
		case '9':{
			$('selected_lang').innerHTML = "Français";
			$('FR').parentNode.style.display = "none";
			break;
		}
		case '10':{
			$('selected_lang').innerHTML = "Italiano";
			$('IT').parentNode.style.display = "none";
			break;
		}
		case '11':{
			$('selected_lang').innerHTML = "Malay";
			$('MS').parentNode.style.display = "none";
			break;
		}
		case '12':{
			$('selected_lang').innerHTML = "Norsk";
			$('NO').parentNode.style.display = "none";
			break;
		}
		case '13':{
			$('selected_lang').innerHTML = "Polski";
			$('PL').parentNode.style.display = "none";
			break;
		}
		case '14':{
			$('selected_lang').innerHTML = "Pусский";
			$('RU').parentNode.style.display = "none";
			break;
		}
		case '15':{
			$('selected_lang').innerHTML = "Svensk";
			$('SV').parentNode.style.display = "none";
			break;
		}
		case '16':{
			$('selected_lang').innerHTML = "ไทย";
			$('TH').parentNode.style.display = "none";
			break;
		}
		case '17':{
			$('selected_lang').innerHTML = "Türkçe";
			$('TR').parentNode.style.display = "none";
			break;
		}
		case '19':{
			$('selected_lang').innerHTML = "Український";
			$('UK').parentNode.style.display = "none";
			break;
		}
	}
}

function get_selected_language(){
var lang_flag = get_preferred_lang_type();
	switch(lang_flag){
		case '1':{	//EN
			return "/";
			break;
		}
		case '3':{	//CN
			return ".cn/";
			break;
		}
		case '18':{	//TW
			return "/tw/";
			break;
		}
		case '2':{	//BR
			return "/br/";
			break;
		}
		case '4':{	//CZ
			return "/cz/";
			break;
		}
		case '5':{	//DA
			return "/dk/";
			break;
		}
		case '6':{	//DE
			return "/de/";
			break;
		}
		case '7':{	//ES
			return "/es/";
			break;
		}
		case '8':{	//FI
			return "/fi/";
			break;
		}
		case '9':{	//FR
			return "/fr/";
			break;
		}
		case '10':{	//IT
			return "/it/";
			break;
		}
		case '11':{	//MS
			return "/my/";
			break;
		}
		case '12':{	//NO
			return "/no/";
			break;
		}
		case '13':{	//PL
			return "/pl/";
			break;
		}
		case '14':{	//RU
			return "/ru/";
			break;
		}
		case '15':{	//SV
			return "/se/";
			break;
		}
		case '16':{	//TH
			return "/th/";
			break;
		}
		case '17':{	//TR
			return "/tr/";
			break;
		}
		case '19':{	//UK
			return "/ua/";
			break;
		}
	}
}

//Jieming added at 2013.05.24, to switch type of password to text or password
//for IE, need to use two input field and ID should be "xxx", "xxx_text" 
var isNotIE = (navigator.userAgent.search("MSIE") == -1); 
function switchType(obj, showText, chkBox){
	if(chkBox == undefined) chkBox = false;
	var newType = showText ? "text" : "password";
	if(isNotIE){	//Not IE
		obj.type = newType;
	}
	else {	//IE
		if(obj.type != newType)
		{
			var input2 = document.createElement('input');
			input2.mergeAttributes(obj);
			input2.type = newType;
			input2.name = obj.name;
			input2.id = obj.id? obj.id : obj.name;
			input2.value = obj.value;

			obj.parentNode.replaceChild(input2,obj);
			if(showText && input2.id && !chkBox)
				setTimeout(function(){document.getElementById(input2.id).focus()},10);
		}
	}
}

/* convert some special character for shown string */
function handle_show_str(show_str)
{
        show_str = show_str.replace(/\&/g, "&amp;");
        show_str = show_str.replace(/\</g, "&lt;");
        show_str = show_str.replace(/\>/g, "&gt;");
        show_str = show_str.replace(/\ /g, "&nbsp;");
        return show_str;
}

function decodeURIComponentSafe(_ascii){
        try{
                return decodeURIComponent(_ascii);
        }
        catch(err){
                return _ascii;
        }
}

var timezone_dst_adjust = {
	"-1200":"-1100",
	"-1100":"-1000",
	"-1000":"-0900",
	"-0900":"-0800",
	"-0800":"-0700",
	"-0700":"-0600",
	"-0600":"-0500",
	"-0500":"-0400",
	"-0400":"-0300",
	"-0330":"-0230",
	"-0300":"-0200",
	"-0200":"-0100",
	"-0100":"+0000",
	"-0000":"+0100",
	"+0000":"+0100",
	"+0100":"+0200",
	"+0200":"+0300",
	"+0300":"+0400",
	"+0330":"+0430",
	"+0400":"+0500",
	"+0430":"+0530",
	"+0500":"+0600",
	"+0530":"+0630",
	"+0545":"+0645",
	"+0600":"+0700",
	"+0630":"+0730",
	"+0700":"+0800",
	"+0800":"+0900",
	"+0900":"+1000",
	"+0930":"+1030",
	"+1000":"+1100",
	"+1100":"+1200",
	"+1200":"+1300"
}

var TZ_orig = "<%tcWebApi_get("Timezone_Entry","TZ","s")%>";
var DAYLIGHT_orig = "<%tcWebApi_get("Timezone_Entry","DAYLIGHT","s")%>";	//Disable:Enable
function corrected_timezone(_flag, _tz){
	_flag = (_flag == "Enable")? true:false;	
	if(_tz == "GMT")
		_tz = "+0000";
	else if(_tz.length >= 0){
		_tz = _tz.replace("GMT","").replace(":","");
		if(_tz.indexOf("+") >= 0)	
			_tz = _tz.replace("+","-");
		else if(_tz.indexOf("-") >= 0)	
			_tz = _tz.replace("-","+");		
	}	
	
	var today = new Date();	//Browser
	var StrIndex;
	if(today.toString().lastIndexOf("-") > 0)
		StrIndex = today.toString().lastIndexOf("-");
	else if(today.toString().lastIndexOf("+") > 0)
		StrIndex = today.toString().lastIndexOf("+");
				
	//alert(_flag+"/"+_tz+"/"+today.toString().substring(StrIndex, StrIndex+5));			
	if(StrIndex > 0){
			
			if(_flag){	//DST			
				if(timezone_dst_adjust[_tz] != today.toString().substring(StrIndex, StrIndex+5)){
					document.getElementById("timezone_hint").style.display = "";
					document.getElementById("timezone_hint").innerHTML = "<%tcWebApi_get("String_Entry","LHC_x_TimeZone_itemhint","s")%>";
				}
				else{
					document.getElementById("timezone_hint").style.display = "none";
					return;
				}	
			}
			else{		
				if(_tz != today.toString().substring(StrIndex, StrIndex+5)){
					document.getElementById("timezone_hint").style.display = "";
					document.getElementById("timezone_hint").innerHTML = "<%tcWebApi_get("String_Entry","LHC_x_TimeZone_itemhint","s")%>";
				}
				else{
					document.getElementById("timezone_hint").style.display = "none";
					return;
				}	
			}						
	}
	else
		return;
}

var cookie = {
	set: function(key, value, days) {
		document.cookie = key + '=' + value + '; expires=' +
		(new Date(new Date().getTime() + ((days ? days : 14) * 86400000))).toUTCString() + '; path=/';
	},
 
	get: function(key) {
		var r = ('; ' + document.cookie + ';').match(key + '=(.*?);');
		return r ? r[1] : null;
	},
 
	unset: function(key) {
		document.cookie = key + '=; expires=' +
		(new Date(1)).toUTCString() + '; path=/';
	}
};

