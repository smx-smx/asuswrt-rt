<% set_current_active_wan() %>
<%
if Request_Form("apply_flag") = "1" Then
		TCWebApi_set("Dhcp6s_Entry","Enable","dhcp6sEnable")
		TCWebApi_set("Dhcp6s_Entry","Mode","dhcp6sMode")
		TCWebApi_set("Dhcp6s_Entry","DNSserver","PrimaryDns")
		TCWebApi_set("Dhcp6s_Entry","SecDNSserver","SecondDns")
		if Request_Form("ipv6rd_clean_flag") = "1" Then
			TCWebApi_set("Dhcp6s_Entry","PrefixIPv6","CleanValue")
			TCWebApi_set("Dhcp6s_Entry","Prefixv6Len","CleanValue")
		end if
		TCWebApi_set("Radvd_Entry","Enable","radvdEnable")
		TCWebApi_set("Radvd_Entry","M_Flag","M_Flag")
		TCWebApi_set("Radvd_Entry","O_Flag","O_Flag")
		TCWebApi_set("Radvd_Entry","Mode","radvdMode")
		TCWebApi_set("Radvd_Entry","PrefixIPv6","ipv6rd_prefix")
			
		TCWebApi_set("ipv6rd_Entry","Active","IPv6_Active")
		TCWebApi_set("ipv6rd_Entry","tunMode","ipv6_TransMode")
		if Request_Form("wan_pvc_flag") = "1" Then
			tcWebApi_set("Wan_PVC","IPVERSION","ipVerValue")
		end if
		
		TCWebApi_set("ipv6rd_Entry","Prefix","ipv6rd_prefix")
		TCWebApi_set("ipv6rd_Entry","PrefixLen","ipv6rd_prefixlen")
		TCWebApi_set("ipv6rd_Entry","6rd_dhcp","ipv6rd_dhcpRadio")		
		TCWebApi_set("ipv6rd_Entry","BRIPv4Addr","BRipv4Addr")
		TCWebApi_set("ipv6rd_Entry","IPv4MaskLen","ipv4masklen")		
		
		TCWebApi_set("ipv6rd_Entry","ipv6_tun_v4end","ipv6_tun_v4end")
		TCWebApi_set("ipv6rd_Entry","ipv6_relay","ipv6_relay")
		TCWebApi_set("ipv6rd_Entry","ipv6_tun_addr","ipv6_tun_addr")
		TCWebApi_set("ipv6rd_Entry","ipv6_tun_addrlen","ipv6_tun_addrlen")
		TCWebApi_set("ipv6rd_Entry","ipv6_tun_peer","ipv6_tun_peer")
		TCWebApi_set("ipv6rd_Entry","ipv6_tun_mtu","ipv6_tun_mtu")
		TCWebApi_set("ipv6rd_Entry","ipv6_tun_ttl","ipv6_tun_ttl")
		
		if Request_Form("apply_6IN4_flag") = "1" Then
			TCWebApi_set("Dhcp6s_Entry","Prefixv6Len","ipv6_prefix_length")
			TCWebApi_set("ipv6rd_Entry","ipv6_tun_prefix","ipv6_prefix")
			TCWebApi_set("Dhcp6s_Entry","PrefixIPv6","ipv6_prefix")
			TCWebApi_set("Radvd_Entry","PrefixIPv6","ipv6_prefix")
			TCWebApi_set("ipv6rd_Entry","ipv6_tun_DNS1","PrimaryDns")
			TCWebApi_set("ipv6rd_Entry","ipv6_tun_DNS2","SecondDns")
		end if
		
		if Request_Form("ipv6_disable_flag") = "1" Then
			tcWebApi_commit("Radvd_Entry")
			tcWebApi_commit("Dhcp6s_Entry")			
		end if
		
		tcWebApi_commit("ipv6rd_Entry")
		tcWebApi_commit("Wan_PVC")				
end if
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_IPv6_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - IPv6</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
var Wan_Ipversion_orig = "<%tcWebApi_get("Wan_PVC","IPVERSION","s")%>";		//IPv4, IPv4/IPv6, IPv6
var ipv6_active_orig = "<%tcWebApi_get("ipv6rd_Entry","Active","s")%>";	//Yes/No
var	ipv6_TransMode_orig = "<%tcWebApi_get("ipv6rd_Entry","tunMode","s")%>";		//NATIVE/6TO4/6IN4/6RD

var Radvd_MFlag_orig = "<%tcWebApi_get("Radvd_Entry","M_Flag","s")%>";
var Radvd_OFlag_orig = "<%tcWebApi_get("Radvd_Entry","O_Flag","s")%>";

function initial(){
	show_menu();
	load_ipv6_TransMode();	
}

function load_ipv6_TransMode(){	//ipv6_active_orig & ipv6_TransMode_orig
	
	if(ipv6_active_orig == "Yes"){
		if(ipv6_TransMode_orig == "NATIVE"){
			document.form.ipv6_TransMode.value = "NATIVE";
		}
		else if(ipv6_TransMode_orig == "6TO4"){
			document.form.ipv6_TransMode.value = "6TO4";
		}
		else if(ipv6_TransMode_orig == "6IN4"){
			document.form.ipv6_TransMode.value = "6IN4";
		}		
		else if(ipv6_TransMode_orig == "6RD"){
			document.form.ipv6_TransMode.value = "6RD";
		}
	}
	else{	//Active: No
		document.form.ipv6_TransMode.value = "DISABLE";
	}
	
	doTransChange(document.form.ipv6_TransMode);
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

function ipv6_valid(obj){
	var rangere=new RegExp("^((([0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}:[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){5}:([0-9A-Fa-f]{1,4}:)?[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){4}:([0-9A-Fa-f]{1,4}:){0,2}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){3}:([0-9A-Fa-f]{1,4}:){0,3}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){2}:([0-9A-Fa-f]{1,4}:){0,4}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}((\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b)\\.){3}(\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b))|(([0-9A-Fa-f]{1,4}:){0,5}:((\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b)\\.){3}(\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b))|(::([0-9A-Fa-f]{1,4}:){0,5}((\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b)\\.){3}(\\b((25[0-5])|(1\\d{2})|(2[0-4]\\d)|(\\d{1,2}))\\b))|([0-9A-Fa-f]{1,4}::([0-9A-Fa-f]{1,4}:){0,5}[0-9A-Fa-f]{1,4})|(::([0-9A-Fa-f]{1,4}:){0,6}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){1,7}:))$", "gi");	
	if(rangere.test(obj.value)){
		return true;
	}
	else{
		alert(obj.value+"<%tcWebApi_get("String_Entry","JS_validip","s")%>");
		obj.focus();
		obj.select();
		return false;
	}
}

function validInteger(Integer)
{
   if(Integer.match("^[0-9]+"))
		return true;
   else
		return false;
}

function validForm(){
		
	if(document.form.ipv6_TransMode.value == "NATIVE"){
		
		if(document.form.PrimaryDns.value.length == 0 && document.form.SecondDns.value.length == 0){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			document.form.PrimaryDns.focus();
			return false;
		}
		if(document.form.PrimaryDns.value != ""){
			if(!ipv6_valid(document.form.PrimaryDns)) return false;
		}
		if(document.form.SecondDns.value != ""){
			if(!ipv6_valid(document.form.SecondDns)) return false;
		}		
	}
	else if(document.form.ipv6_TransMode.value == "6TO4"){
			if(!valid_IP(document.form.ipv6_relay, "")) return false;  //6to4 tun relay
			
			if(!validator.rangeAllowZero(document.form.ipv6_tun_mtu,1280,1480,0))  return false;  //MTU
			if(!validator.rangeAllowZero(document.form.ipv6_tun_ttl,0,255,255))  return false;  //TTL
		
	}
	else if(document.form.ipv6_TransMode.value == "6IN4"){
			if(!validator.ipRange(document.form.ipv6_tun_v4end, "")) return false;  //6in4 tun endpoint
			if(!ipv6_valid(document.form.ipv6_tun_addr)) return false;  //6in4 Client IPv6 Address
			if(!validator.range(document.form.ipv6_tun_addrlen, 3, 64))  return false;
			if(document.form.ipv6_tun_peer.value != "" && !ipv6_valid(document.form.ipv6_tun_peer)) return false;
			if(	!validator.range(document.form.ipv6_prefix_length, 3, 64))	return false;
			if(!ipv6_valid(document.form.ipv6_prefix)) return false;
			
	}
	else if(document.form.ipv6_TransMode.value == "6RD"){
		
		if(!ipv6_valid(document.form.ipv6rd_prefix) ||
			!validator.range(document.form.ipv6rd_prefixlen, 3, 64))
		{
			return false;
		}		
		
		if(!validator.ipRange(document.form.BRipv4Addr, "")) return false;  //6rd Border Relay IPv4 Addr
		if(!validator.range(document.form.ipv4masklen, 0, 32)) return false;  //6rd IPv4 Mask Length
		
		if(!validator.rangeAllowZero(document.form.ipv6_tun_mtu,1280,1480,0))  return false;  //MTU
		if(!validator.rangeAllowZero(document.form.ipv6_tun_ttl,0,255,255))  return false;  //TTL
		
		if(document.form.PrimaryDns.value.length == 0 && document.form.SecondDns.value.length == 0){
			alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
			document.form.PrimaryDns.focus();
			return false;
		}
		if(document.form.PrimaryDns.value != ""){
			if(!ipv6_valid(document.form.PrimaryDns)) return false;
		}
		if(document.form.SecondDns.value != ""){
			if(!ipv6_valid(document.form.SecondDns)) return false;
		}	
	}
	
	return true;
}

function applyRule(){
	if(validForm()){
				
		if(document.form.ipv6_TransMode.value == "6TO4" || document.form.ipv6_TransMode.value == "6IN4" || document.form.ipv6_TransMode.value == "6RD"){
			if(Wan_Ipversion_orig != "IPv4"){
				document.form.ipVerValue.value = "IPv4";
				document.form.wan_pvc_flag.value = "1";
			}
			
			document.form.IPv6_Active.value = "Yes";
			
			document.form.dhcp6sEnable.value = 1;
			document.form.dhcp6sMode.value = 1;
				
			document.form.radvdEnable.value = 1;
			document.form.M_Flag.value = Radvd_MFlag_orig;
			document.form.O_Flag.value = Radvd_OFlag_orig;								
			document.form.radvdMode.value = 1;			
			
			document.form.ipv6rd_clean_flag.value = 1;
			document.form.apply_flag.value = 1;
			
			if(document.form.ipv6_TransMode.value == "6IN4"){
				document.form.apply_6IN4_flag.value = 1;	
			}
			
			showLoading(13);
			setTimeout('location = "'+ location.pathname +'";', 13000);
			document.form.submit();				
				
		}
		else if(document.form.ipv6_TransMode.value == "NATIVE"){
			if(Wan_Ipversion_orig == "IPv4"){
				document.form.ipVerValue.value = "IPv4/IPv6";
				document.form.wan_pvc_flag.value = "1";
			}	
			
			document.form.IPv6_Active.value = "Yes";
			
			document.form.dhcp6sEnable.value = 1;			
			document.form.dhcp6sMode.value = 1;
			document.form.radvdEnable.value = 1;
			if(document.form.ipv6_autoconf_type[0].checked == true){	//Steteless				
				document.form.M_Flag.value = "off";
				document.form.O_Flag.value = "on";
			}	
			else{
				document.form.M_Flag.value = "on";
				document.form.O_Flag.value = "on";
			}	
				
			document.form.radvdMode.value = 0;
			
			document.form.apply_flag.value = 1;
			showLoading(13);
			setTimeout('location = "'+ location.pathname +'";', 13000);
			document.form.submit();
		}
		else{	//Disable
			
			document.form.ipVerValue.value = "IPv4";
			document.form.wan_pvc_flag.value = "1";
			
			document.form.IPv6_Active.value = "No";
			if(ipv6_TransMode_orig != "DISABLE"){
				document.form.apply_flag.value = 1;
				document.form.ipv6_disable_flag.value = 1;
				showLoading(13);
				setTimeout('location = "'+ location.pathname +'";', 13000);
				document.form.submit();
			}
			else{	//do nothing
				return;
			}
		}
	}
}

function doTransChange(obj){	
		switch(obj.value) {			
			case "NATIVE":
						document.form.transmode.value = "NATIVE";
						//document.getElementById("ipv6_TransMode_hint").innerHTML = "( Only support in WAN IP version <a style=\"font-weight:bolder;text-decoration:underline;color:#FFCC00;\" href=\"/Advanced_DSL_Content.asp\" target=\"_blank\">IPv4 / IPv6</a> or <a style=\"font-weight:bolder;text-decoration:underline;color:#FFCC00;\" href=\"/Advanced_DSL_Content.asp\" target=\"_blank\">IPv6</a>)";
						//document.getElementById("ipv6_TransMode_hint").style.display = "";
						inputCtrl(document.form.ipv6_tun_v4end, 0);
						inputCtrl(document.form.ipv6_relay, 0);
						inputCtrl(document.form.ipv6_tun_addr, 0);
						inputCtrl(document.form.ipv6_tun_addrlen, 0);
						inputCtrl(document.form.ipv6_tun_peer, 0);
						inputCtrl(document.form.ipv6_tun_mtu, 0);
						inputCtrl(document.form.ipv6_tun_ttl, 0);
						
						document.getElementById("6rd_config").style.display = "none";
						document.getElementById("ipv6_lan_setting").style.display = "";
						inputCtrl(document.form.ipv6_prefix_length, 0);
						inputCtrl(document.form.ipv6_prefix, 0);
						document.getElementById("ipv6_prefix_r").style.display = "";
						document.getElementById("ipv6_prefix_length_r").style.display = "";
						
						if(ipv6_TransMode_orig != obj.value){
							document.getElementById("ipv6_ipaddr_span").innerHTML = "";
							document.getElementById("ipv6_prefix_length_span").innerHTML = "";
							document.getElementById("ipv6_prefix_span").innerHTML = "";
						}
						else{
							document.getElementById("ipv6_ipaddr_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_rtr_addr", "s")%>";
							document.getElementById("ipv6_prefix_length_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_prefix_length", "s")%>";
							document.getElementById("ipv6_prefix_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_prefix", "s")%>";		
						}
						document.getElementById("ipv6_dns_config").style.display = "";
						inputCtrl(document.form.ipv6_autoconf_type[0], 1);
						load_autoconf_type();
				break;
			case "6TO4":
						document.form.transmode.value = "6TO4";
						inputCtrl(document.form.ipv6_tun_v4end, 0);
						inputCtrl(document.form.ipv6_relay, 1);
						inputCtrl(document.form.ipv6_tun_addr, 0);
						inputCtrl(document.form.ipv6_tun_addrlen, 0);
						inputCtrl(document.form.ipv6_tun_peer, 0);
						inputCtrl(document.form.ipv6_tun_mtu, 1);
						inputCtrl(document.form.ipv6_tun_ttl, 1);
						
						document.getElementById("6rd_config").style.display = "none";
						document.getElementById("ipv6_lan_setting").style.display = "";
						inputCtrl(document.form.ipv6_prefix_length, 0);
						inputCtrl(document.form.ipv6_prefix, 0);
						document.getElementById("ipv6_prefix_r").style.display = "";
						document.getElementById("ipv6_prefix_length_r").style.display = "";
						inputCtrl(document.form.ipv6_autoconf_type[0], 0);
						
						if(ipv6_TransMode_orig != obj.value){
							document.getElementById("ipv6_ipaddr_span").innerHTML = "";
							document.getElementById("ipv6_prefix_length_span").innerHTML = "";
							document.getElementById("ipv6_prefix_span").innerHTML = "";
						}
						else{
							document.getElementById("ipv6_ipaddr_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_rtr_addr", "s")%>";
							document.getElementById("ipv6_prefix_length_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_prefix_length", "s")%>";
							document.getElementById("ipv6_prefix_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_prefix", "s")%>";
						}
						document.getElementById("ipv6_dns_config").style.display = "";
				break;	
			case "6IN4":
						document.form.transmode.value = "6IN4";
						inputCtrl(document.form.ipv6_tun_v4end, 1);
						inputCtrl(document.form.ipv6_relay, 0);
						inputCtrl(document.form.ipv6_tun_addr, 1);
						inputCtrl(document.form.ipv6_tun_addrlen, 1);
						inputCtrl(document.form.ipv6_tun_peer, 1);
						inputCtrl(document.form.ipv6_tun_mtu, 1);
						inputCtrl(document.form.ipv6_tun_ttl, 1);
						
						document.getElementById("6rd_config").style.display = "none";
						document.getElementById("ipv6_lan_setting").style.display = "";
						inputCtrl(document.form.ipv6_prefix_length, 1);
						inputCtrl(document.form.ipv6_prefix, 1);
						document.getElementById("ipv6_prefix_r").style.display = "none";
						document.getElementById("ipv6_prefix_length_r").style.display = "none";
						inputCtrl(document.form.ipv6_autoconf_type[0], 0);
						
						if(ipv6_TransMode_orig != obj.value){
							document.getElementById("ipv6_ipaddr_span").innerHTML = "";							
						}
						else{
							document.getElementById("ipv6_ipaddr_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_rtr_addr", "s")%>";							
						}
						
						document.getElementById("ipv6_dns_config").style.display = "";
						
				break;			
			case "6RD":
						document.form.transmode.value = "6RD";
						//document.getElementById("ipv6_TransMode_hint").innerHTML = "( Only support in WAN IP version <a style=\"font-weight:bolder;text-decoration:underline;color:#FFCC00;\" href=\"/Advanced_DSL_Content.asp?af=ipVerRadio\" target=\"_blank\">IPv4 / IPv6</a> )";
						//document.getElementById("ipv6_TransMode_hint").style.display = "";
						inputCtrl(document.form.ipv6_tun_v4end, 0);
						inputCtrl(document.form.ipv6_relay, 0);
						inputCtrl(document.form.ipv6_tun_addr, 0);
						inputCtrl(document.form.ipv6_tun_addrlen, 0);
						inputCtrl(document.form.ipv6_tun_peer, 0);
						inputCtrl(document.form.ipv6_tun_mtu, 1);
						inputCtrl(document.form.ipv6_tun_ttl, 1);
						document.getElementById("6rd_config").style.display = "";
						document.getElementById("ipv6_lan_setting").style.display = "";
						inputCtrl(document.form.ipv6_prefix_length, 0);
						inputCtrl(document.form.ipv6_prefix, 0);
						document.getElementById("ipv6_prefix_r").style.display = "";
						document.getElementById("ipv6_prefix_length_r").style.display = "";
						
						if(ipv6_TransMode_orig != obj.value){
							document.getElementById("ipv6_ipaddr_span").innerHTML = "";
							document.getElementById("ipv6_prefix_length_span").innerHTML = "";
							document.getElementById("ipv6_prefix_span").innerHTML = "";
						}
						else{
							document.getElementById("ipv6_ipaddr_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_rtr_addr", "s")%>";
							document.getElementById("ipv6_prefix_length_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_prefix_length", "s")%>";
							document.getElementById("ipv6_prefix_span").innerHTML = "<%tcWebApi_get("Vram_Entry", "ipv6_prefix", "s")%>";		
						}	
						document.getElementById("ipv6_dns_config").style.display = "";
						inputCtrl(document.form.ipv6_autoconf_type[0], 0);
						load_autoconf_type();
				break;
			default: //Disable
						document.form.transmode.value = "Disable";
						//document.getElementById("ipv6_TransMode_hint").style.display = "none";
						inputCtrl(document.form.ipv6_tun_v4end, 0);
						inputCtrl(document.form.ipv6_relay, 0);
						inputCtrl(document.form.ipv6_tun_addr, 0);
						inputCtrl(document.form.ipv6_tun_addrlen, 0);
						inputCtrl(document.form.ipv6_tun_peer, 0);
						inputCtrl(document.form.ipv6_tun_mtu, 0);
						inputCtrl(document.form.ipv6_tun_ttl, 0);
						document.getElementById("6rd_config").style.display = "none";
						document.getElementById("ipv6_lan_setting").style.display = "none";
						document.getElementById("ipv6_dns_config").style.display = "none";
				break;				
		}
}

function load_autoconf_type(){
	if(Radvd_MFlag_orig == "off" && Radvd_OFlag_orig == "on")	//Stateless
		document.form.ipv6_autoconf_type[0].checked = true;
	else	//Stateful
		document.form.ipv6_autoconf_type[1].checked = true;	
}

</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="Advanced_IPv6_Content.asp" target="hidden_frame">
<input type="hidden" name="apply_flag" value="0">
<input type="hidden" name="transmode" value="">
<input type="hidden" name="wan_pvc_flag" value="0">
<input type="hidden" name="ipv6_disable_flag" value="0">
<input type="hidden" name="ipv6rd_clean_flag" value="0">
<input type="hidden" name="apply_6IN4_flag" value="0">
<input type="hidden" name="ipVerValue" value="">
<input type="hidden" name="IPv6_Active" value="No">
<input type="hidden" name="dhcp6sEnable" value="0">
<input type="hidden" name="dhcp6sMode" value="0">
<input type="hidden" name="ipv6rd_dhcpRadio" value="0">
<input type="hidden" name="radvdEnable" value="0">
<input type="hidden" name="M_Flag" value="off">
<input type="hidden" name="O_Flag" value="on">
<input type="hidden" name="radvdMode" value="0">
<input type="hidden" name="CleanValue" value="">
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
								<div class="formfonttitle">IPv6</div>
								<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
								<div class="formfontdesc"><%tcWebApi_get("String_Entry","IPv6_LAN_Setting","s")%></div>
								<table id="basic_config" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
									<thead>
									<tr>
										<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
									</tr>
									</thead>
									<tr>
										<th><%tcWebApi_get("String_Entry","Connectiontype","s")%></th>
										<td>											
											<select class="input_option" name="ipv6_TransMode" onchange="doTransChange(this);">
												<option value="DISABLE"><%tcWebApi_get("String_Entry","btn_disable","s")%>
												<option value="NATIVE">Native
												<option value="6TO4">Tunnel 6to4
												<option value="6IN4">Tunnel 6in4
												<option value="6RD">Tunnel 6rd
											</select>
											<span id="ipv6_TransMode_hint" style="display:none;"></span>
										</td>
									</tr>
									<tr style="display:none;">
										<th><%tcWebApi_get("String_Entry","IPv6_tun_v4end","s")%></th>
										<td>
											<input type="text" maxlength="15" class="input_15_table" name="ipv6_tun_v4end" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_tun_v4end","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_tun_v4end","s") end if %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
									<tr style="display:none;">
										<th><%tcWebApi_get("String_Entry","IPv6_relay","s")%></th>
										<td>
											<input type="text" maxlength="15" class="input_15_table" name="ipv6_relay" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_relay","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_relay","s") end if %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
									<tr style="display:none;">
										<th><%tcWebApi_get("String_Entry","ipv6_client_ip","s")%></th>
										<td>
											<input type="text" maxlength="39" class="input_32_table" name="ipv6_tun_addr" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_tun_addr","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_tun_addr","s") end if %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
									<tr style="display:none;">
										<th><%tcWebApi_get("String_Entry","IPv6_Prefix_Length","s")%></th>
										<td>
											<input type="text" maxlength="2" class="input_3_table" name="ipv6_tun_addrlen" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_tun_addrlen","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_tun_addrlen","s") end if %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
									<tr style="display:none;">
										<th><%tcWebApi_get("String_Entry","ipv6_peer_addr","s")%></th>
										<td>
											<input type="text" maxlength="39" class="input_32_table" name="ipv6_tun_peer" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_tun_peer","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_tun_peer","s") end if %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
									<tr style="display:none;">
										<th><%tcWebApi_get("String_Entry","tunnel_MTU","s")%></th>
										<td>
											<input type="text" maxlength="4" class="input_6_table" name="ipv6_tun_mtu" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_tun_mtu","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_tun_mtu","s") end if %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
									<tr style="display:none;">
										<th><%tcWebApi_get("String_Entry","tunnel_TTL","s")%></th>
										<td>
											<input type="text" maxlength="3" class="input_6_table" name="ipv6_tun_ttl" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_tun_ttl","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_tun_ttl","s") end if %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
								</table>	

								<table id="6rd_config" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:10px;display:none;">
									<thead>
									<tr>
										<td colspan="2">6rd Config</td>
									</tr>
									</thead>									
									<tr>
										<th><%tcWebApi_get("String_Entry","6rdIPv6PrefixText","s")%></th>
										<td>
											<input type="text" name="ipv6rd_prefix" maxLength="39" class="input_32_table" value="<%If TcWebApi_get("ipv6rd_Entry","Prefix","h") <> "" then TcWebApi_get("ipv6rd_Entry","Prefix","s") end if %>">
										</td>
									</tr>
									<tr>
										<th><%tcWebApi_get("String_Entry","IPv6_Prefix_Length","s")%></th>
										<td>
											<input type="text" name="ipv6rd_prefixlen" maxLength="3" class="input_3_table" value="<%if tcwebapi_get("ipv6rd_Entry","PrefixLen","h")<> "" then tcwebApi_get("ipv6rd_Entry","PrefixLen","s") else asp_write("32") end if%>">
										</td>
									</tr>									
									<tr>
										<th><%tcWebApi_get("String_Entry","6rdBorderRelayIPv4AddrText","s")%></th>
										<td>
											<INPUT TYPE="TEXT" name="BRipv4Addr" maxLength=39 class="input_32_table" value="<%If TcWebApi_get("ipv6rd_Entry","BRIPv4Addr","h") <> "" then TcWebApi_get("ipv6rd_Entry","BRIPv4Addr","s") end if %>" >
										</td>
									</tr>
									<tr>
										<th><%tcWebApi_get("String_Entry","6rdIPv4MaskLengthText","s")%></th>
										<td>
											<INPUT TYPE="TEXT" name="ipv4masklen" maxLength=3 class="input_3_table" value="<%If TcWebApi_get("ipv6rd_Entry","IPv4MaskLen","h") <> "" then TcWebApi_get("ipv6rd_Entry","IPv4MaskLen","s") else asp_Write("0") end if %>" >
										</td>
									</tr>
									<tr id="PrefixDelegation">
										<th><%tcWebApi_get("String_Entry","6rdPrefixDelegationText","s")%></th>
										<td>
											<%If TcWebApi_get("ipv6rd_Entry","6rdPD","h") <> "" Then TcWebApi_get("ipv6rd_Entry","6rdPD","s") else asp_Write("") end if%>
										</td>
									</tr>									
								</table>
								
								<!---------------------------------------IPv6 LAN setting start------------------------------->  
								<table id="ipv6_lan_setting" style="margin-top:8px;display:none;" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
								<thead>
								<tr>
									<td colspan="2"><%tcWebApi_get("String_Entry","IPv6_LAN_Setting","s")%></td>
								</tr>
								</thead>
				  								
								<tr id="ipv6_ipaddr_r">
									<th><%tcWebApi_get("String_Entry","ipv6_lan_addr","s")%></th>	<!-- for other ipv6 proto -->
									<td>
										<div id="ipv6_ipaddr_span" name="ipv6_ipaddr_span" style="color:#FFFFFF;margin-left:8px;"></div>										
									</td>
								</tr>				  
				  								
				  			<tr id="ipv6_prefix_length">
				  				<th><%tcWebApi_get("String_Entry","Prefix_lan_Length","s")%></th>
				  				<td>
				  					<input type="text" maxlength="2" class="input_3_table" name="ipv6_prefix_length" value="<%If TcWebApi_get("Dhcp6s_Entry","Prefixv6Len","h") <> "" then TcWebApi_get("Dhcp6s_Entry","Prefixv6Len","s") end if %>" autocorrect="off" autocapitalize="off">
				  				</td>
				  			</tr>					
								<tr id="ipv6_prefix_length_r">
									<th><%tcWebApi_get("String_Entry","Prefix_lan_Length","s")%></th>
									<td>
										<div id="ipv6_prefix_length_span" name="ipv6_prefix_length_span" style="color:#FFFFFF;margin-left:8px;"></div>
									</td>
								</tr>
								
		     										
		     				<tr id="ipv6_prefix">
									<th><%tcWebApi_get("String_Entry","IPv6_lan_Prefix","s")%></th>
									<td>
										<input type="text" maxlength="39" class="input_32_table" name="ipv6_prefix" value="<%If TcWebApi_get("ipv6rd_Entry","ipv6_tun_prefix","h") <> "" then TcWebApi_get("ipv6rd_Entry","ipv6_tun_prefix","s") end if %>" autocorrect="off" autocapitalize="off">
									</td>
								</tr>							
								<tr id="ipv6_prefix_r">
									<th><%tcWebApi_get("String_Entry","IPv6_lan_Prefix","s")%></th>
									<td>
										<div id="ipv6_prefix_span" name="ipv6_prefix_span" style="color:#FFFFFF;margin-left:8px;"></div>
									</td>
								</tr>		     	     	
		     	
								<tr>
									<th><%tcWebApi_get("String_Entry","ipv6_auto_config","s")%></th>	
									<td>
										<input type="radio" name="ipv6_autoconf_type" class="input" value="0">Stateless
										<input type="radio" name="ipv6_autoconf_type" class="input" value="1">Stateful
									</td>	
								</tr>									
							</table>
							<!---------------------------------------IPv6 LAN setting end------------------------------->
								
								<!---------------------------------------IPv6 DNS setting start------------------------------->
								<table id="ipv6_dns_config" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" style="margin-top:10px;display:none;">
									<thead>
									<tr>
										<td colspan="2">IPv6 DNS Setting</td>
									</tr>
									</thead>	
									<!--tr>
										<th><% tcWebApi_Get("String_Entry", "IPC_x_DNSServerEnable_in", "s") %>?</th>
										<td>
											<INPUT TYPE="RADIO" NAME="dnsTypeRadio" class="input" VALUE="0" onClick="" disabled><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
											<INPUT TYPE="RADIO" NAME="dnsTypeRadio" class="input" VALUE="1" onClick="" checked><%tcWebApi_get("String_Entry","checkbox_No","s")%>
										</td>
									</tr-->
									<tr>
										<th><% tcWebApi_Get("String_Entry", "IPC_x_DNSServer1_in", "s") %></th>
										<td>
											<input type="TEXT" name="PrimaryDns" class="input_32_table" maxlength="39" value="<% tcWebApi_get("Dhcp6s_Entry","DNSserver","s")  %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
									<tr>
										<th><% tcWebApi_Get("String_Entry", "IPC_x_DNSServer2_in", "s") %></th>
										<td>
											<input type="TEXT" name="SecondDns" class="input_32_table" maxlength="39" value="<% tcWebApi_get("Dhcp6s_Entry","SecDNSserver","s") %>" autocorrect="off" autocapitalize="off">
										</td>
									</tr>
								</table>
								<!---------------------------------------IPv6 DNS setting end------------------------------->

								<div class="apply_gen">
									<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
								</div>
							</td>
						</tr>
					</tbody>
					</table>
				</td>
			</tr>
		</table>
	</td>
	<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
</form>
<div id="footer"></div>
</body>

<!--Advanced_IPv6_Content.asp-->
</html>

