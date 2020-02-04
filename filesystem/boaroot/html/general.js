var nav;
var change;
var keyPressed;
var wItem;
var ip = "";
var ip_addr = '<!--#echo var="REMOTE_ADDR"-->';
var chanList = 0;
var wep1, wep2, wep3, wep4;
var varload=0;
var final_flag = 0; // for validate_ipaddr() always return true.

if(navigator.appName.indexOf("Microsoft") != -1){
nav = false;
document.onkeydown = MicrosoftEventHandler_KeyDown;
}
else{
nav = true;
}
function inet_network(ip_str){
if(!ip_str)
return -1;
var re = /^(\d+)\.(\d+)\.(\d+)\.(\d+)$/;
if(re.test(ip_str)){
var v1 = parseInt(RegExp.$1);
var v2 = parseInt(RegExp.$2);
var v3 = parseInt(RegExp.$3);
var v4 = parseInt(RegExp.$4);
if(v1 < 256 && v2 < 256 && v3 < 256 && v4 < 256)
return v1*256*256*256+v2*256*256+v3*256+v4;
}
return -2;
}

//Filtering ip address with leading zero, if end-user keyin IP 192.168.02.1, system auto filtering IP 192.168.2.1
function ipFilterZero(ip_str){ 
	var re = /^(\d+)\.(\d+)\.(\d+)\.(\d+)$/;
	if(re.test(ip_str)){
		var v1 = parseInt(RegExp.$1);
		var v2 = parseInt(RegExp.$2);
		var v3 = parseInt(RegExp.$3);
		var v4 = parseInt(RegExp.$4);
		return v1+"."+v2+"."+v3+"."+v4;
	}
	return -2;
}

function isMask(ip_str){
if(!ip_str)
return 0;
var re = /^(\d+)\.(\d+)\.(\d+)\.(\d+)$/;
if(re.test(ip_str)){
var v1 = parseInt(RegExp.$1);
var v2 = parseInt(RegExp.$2);
var v3 = parseInt(RegExp.$3);
var v4 = parseInt(RegExp.$4);
if(v4 == 255 || !(v4 == 0 || (is1to0(v4) && v1 == 255 && v2 == 255 && v3 == 255)))
return -4;
if(!(v3 == 0 || (is1to0(v3) && v1 == 255 && v2 == 255)))
return -3;
if(!(v2 == 0 || (is1to0(v2) && v1 == 255)))
return -2;
if(!is1to0(v1))
return -1;
}
return 1;
}
function is1to0(num){
if(typeof(num) != "number")
return 0;
if(num == 255 || num == 254 || num == 252 || num == 248
|| num == 240 || num == 224 || num == 192 || num == 128)
return 1;
return 0;
}
function getSubnet(ip_str, mask_str, flag){
var ip_num, mask_num;
var sub_head, sub_end;
if(!ip_str || !mask_str)
return -1;
if(isMask(mask_str) <= 0)
return -2;
if(!flag || (flag != "head" && flag != "end"))
flag = "head";
ip_num = inet_network(ip_str);
mask_num = inet_network(mask_str);
if(ip_num < 0 || mask_num < 0)
return -3;
sub_head = ip_num-(ip_num&~mask_num);
sub_end = sub_head+~mask_num;
if(flag == "head")
return sub_head;
else
return sub_end;
}
function NetscapeEventHandler_KeyDown(e)
{keyPressed = e.which;
if (keyPressed<48 && keyPressed!=16)
{keyPressed = 0;
}
return true;
}
function changeDate()
{pageChanged = 1;
return true;
}
function MicrosoftEventHandler_KeyDown()
{/* if (keyPressed == 13 && event.srcElement.type != 'button' && event.srcElement.type != 'textarea' && event.srcElement.type != 'submit')
return false;
return true;*/
return true;
}
function str2val(v)
{for(i=0; i<v.length; i++)
{if (v.charAt(i) !='0') break;
}
return v.substring(i);
}
function inputRCtrl1(o, flag)
{if (flag == 0)
{o[0].disabled = 1;
o[1].disabled = 1;
}
else
{o[0].disabled = 0;
o[1].disabled = 0;
}
}
function inputRCtrl2(o, flag)
{if (flag == 0)
{o[0].checked = true;
o[1].checked = false;
}
else
{o[0].checked = false;
o[1].checked = true;
}
}

function markGroup(o, s, c, b) {
return true;
}
function portrange_min(o, v){
var num = 0;
var common_index = o.substring(0, v).indexOf(':');
if(common_index == -1)
num = parseInt(o.substring(0, v));
else
num = parseInt(o.substring(0, common_index));
return num;
}
function portrange_max(o, v){
var num = 0;
var common_index = o.substring(0, v).indexOf(':');
if(common_index == -1)
num = parseInt(o.substring(0, v));
else
num = parseInt(o.substring(common_index+1, v+1));
return num;
}
function isBlank(s) {
for(i=0; i<s.length; i++) {
c=s.charAt(i);
if((c!=' ')&&(c!='\n')&&(c!='\t'))return false;}
return true;
}
function numbersonly()
{if (keyPressed>47 && keyPressed<58)
return true;
else
return false;
}
function check_ptl()
{if(keyPressed==38)
return false;
else
return true;
}
function entry_cmp(entry, match, len){ //compare string length function
var j;
if(entry.length < match.length)
return (1);
for(j=0; j < entry.length && j<len; j++){
c1 = entry.charCodeAt(j);
if (j>=match.length)
c2=160;
else
c2 = match.charCodeAt(j);
if (c1==160)
c1 = 32;
if (c2==160)
c2 = 32;
if (c1>c2)
return (1);
else if (c1<c2)
return(-1);
}
return 0;
}
function validate_duplicate_noalert(o, v, l, off){
for (var i=0; i<o.options.length; i++)
{
if (entry_cmp(o.options[i].text.substring(off).toLowerCase(), v.toLowerCase(), l)==0){
return false;
}
}
return true;
}
function validate_duplicate(o, v, l, off){ // 2008.01 James.
for(var i = 0; i < o.options.length; i++){
if(entry_cmp(o.options[i].text.substring(off).toLowerCase(), v.toLowerCase(), l) == 0){
alert("<%tcWebApi_get("String_Entry","JS_duplicate","s")%>");
return false;
}
}
return true;
}
function validate_duplicate2(o, v, l, off){
var i;
for (i=0; i<o.options.length; i++)
{
if (entry_cmp(o.options[i].text.substring(off).toLowerCase(), v.toLowerCase(), l)==0)
{
return false;
}
}
return true;
}

function validate_ssidchar(ch){
if(ch >= 32 && ch <= 126)
return false;
return true;
}
function validate_string_ssid(o){
var c; // character code
for(var i = 0; i < o.value.length; ++i){
c = o.value.charCodeAt(i);
if(validate_ssidchar(c)){
alert("<%tcWebApi_get("String_Entry","JS_validSSID1","s")%>'"+o.value.charAt(i)+"<%tcWebApi_get("String_Entry","JS_validSSID2","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
}
return true;
}

function validate_string_webpassword(string_obj){
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
				if(string_obj.value.charAt(i) < ' ' || string_obj.value.charAt(i) > '~' || string_obj.value.charAt(i) == '%' || string_obj.value.charAt(i) == '"' || string_obj.value.charAt(i) == ';'){		//ascii: 32~126  except 37 (%) & 34 (") & ; (59)
					invalid_char = invalid_char+string_obj.value.charAt(i);
				}
		}
		
		if(invalid_char != ""){
			alert("<%tcWebApi_get("String_Entry","JS_validstr2","s")%> '"+invalid_char+"' !");
			string_obj.value = "";
			string_obj.focus();
			return false;
		}
		return true;
	}
}

function validate_string_group(o){
var c; // character code
for(var i = 0; i < o.value.length; ++i){
c = o.value.charCodeAt(i);
if(!validate_groupchar(c)){
alert("<%tcWebApi_get("String_Entry","JS_validSSID1","s")%>'"+o.value.charAt(i)+"<%tcWebApi_get("String_Entry","JS_validSSID2","s")%>");
o.focus();
o.select();
return false;
}
}
return true;
}
function validate_groupchar(ch){
if(ch == 60 || ch == 62) /*ch == 39 || */
return false;
return true;
}

function isNumberFloat(o,event){
		var keyPressed = event.keyCode ? event.keyCode : event.which;

		if (isFunctionButton(event)){
			return true;
		}

		if ((keyPressed == 46) || (keyPressed>47 && keyPressed<58))
			return true;
		else
			return false;
}
function validate_range(o, min, max) {
for(i=0; i<o.value.length; i++)
{
if (o.value.charAt(i)<'0' || o.value.charAt(i)>'9')
{
alert("<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> " + min + " <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> " + max);
o.focus();
o.select();
return false;
}
	var reg=/^(0|([1-9]\d*))$/;
	if(!reg.test(o.value) && Untranslated.valid_range_int){
		alert(Untranslated.valid_range_int);
		o.focus();
		o.select();
		return false;
	}
}
if(o.value<min || o.value>max) {
alert("<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> " + min + " <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> " + max);
o.focus();
o.select();
return false;
}
else {
o.value = str2val(o.value);
if(o.value=="")
o.value="0";
return true;
}
}

//javi.B
function validate_even_range(o, min, max) {
	for(i=0; i<o.value.length; i++){
		if (o.value.charAt(i)<'0' || o.value.charAt(i)>'9'){
			alert("<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> " + min + " <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> " + max);
			o.focus();
			o.select();
			return false;
		}
	}
	if(o.value<min || o.value>max){
		alert("<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> " + min + " <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> " + max);
		o.focus();
		o.select();
		return false;
	}
	else {
		o.value = str2val(o.value);
		//alert(o.value + ' numbers'); 
		if(o.value % 2){
			alert(o.value + ' is not an even number, please enter the valid number.');
			return false;
		}
		return true;
	}
}
//javi.E

function validate_range_sp(o, min, max, def) { //allow to set "0"
if (o.value==0) return true;

	var reg=/^(0|([1-9]\d*))$/;
	if(!reg.test(o.value) && Untranslated.valid_range_int){
		alert(Untranslated.valid_range_int);
		o.focus();
		o.select();
		return false;
	}

if(o.value<min || o.value>max) {
alert("<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> " + min + " <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> " + max);
o.value = def;
o.focus();
o.select();
return false;
}
else {
o.value = str2val(o.value);
if(o.value=="") o.value="0";
return true;
}
}

function isFunctionButton(e){
	//function keycode for Firefox/Opera
		var keyCode = e.keyCode;
		if(e.which == 0) {
			if (keyCode == 0
				|| keyCode == 27 //Esc
				|| keyCode == 35 //end
				|| keyCode == 36 //home
				|| keyCode == 37 //<-
				|| keyCode == 39 //->
				|| keyCode == 45 //Insert
				|| keyCode == 46 //Del
				){
				return true;
			}
		}
		if (keyCode == 8 	//backspace
			|| keyCode == 9 	//tab
			){
			return true;
		}
		return false;
}

function intoa(ip)
{n=0;
vip=0;
for(i=0;i<ip.length;i++)
{c = ip.charAt(i);
if (c == '.')
{vip = vip * 256 + n;
n = 0;
}
else if(c>='0' && c<='9')
{n = n*10 + (c-'0');
}
}
vip = vip*256 + n;
return(vip);
}
function requireWANIP(v){
if(v == 'wan_ipaddr_x' || v == 'wan_netmask_x' ||
v == 'lan_ipaddr' || v == 'lan_netmask' ||
v == 'lan1_ipaddr' || v == 'lan1_netmask'){
/*if(document.form.wan_proto.value == "static" || document.form.wan_proto.value == "pptp")
return 1;
else
return 0;*/
if(document.form.wan_proto.value == "static")
return 1;
else if(document.form.wan_proto.value == "pppoe" && intoa(document.form.wan_ipaddr_x.value))
return 1;
else if((document.form.wan_proto.value=="pptp" || document.form.wan_proto.value == "l2tp")
&& document.form.wan_ipaddr_x.value != '0.0.0.0')
return 1;
else
return 0;
}
else return 0;
}
function matchSubnet2(ip1, sb1, ip2, sb2){
var nsb;
var nsb1 = intoa(sb1);
var nsb2 = intoa(sb2);
if(nsb1 < nsb2)
nsb = nsb1;
else
nsb = nsb2;
if((intoa(ip1)&nsb) == (intoa(ip2)&nsb))
return 1;
else
return 0;
}
function validate_ipaddr(o, v){
if(final_flag)
return true;
num = -1;
pos = 0;
if (o.value.length==0)
{if (v=='dhcp_start' || v=='dhcp_end' || v=='wan_ipaddr_x' || v=='dhcp1_start' || v=='dhcp1_end' ||
v=='lan_ipaddr' || v=='lan_netmask' || v=='lan1_ipaddr' || v=='lan1_netmask')
{alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
if (v=='wan_ipaddr_x')
{document.form.wan_ipaddr_x.value = "10.1.1.1";
document.form.wan_netmask_x.value = "255.0.0.0";
}
else if (v=='lan_ipaddr')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if (v=='lan1_ipaddr')
{document.form.lan1_ipaddr.value = "192.168.2.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
else if (v=='lan_netmask') document.form.lan_netmask.value = "255.255.255.0";
else if (v=='lan1_netmask') document.form.lan1_netmask.value = "255.255.255.0";
else if (v=='dhcp_start') document.form.dhcp_start.value = document.form.dhcp_end.value;
else if (v=='dhcp_end') document.form.dhcp_end.value = document.form.dhcp_start.value;
else if (v=='dhcp1_start') document.form.dhcp1_start.value = document.form.dhcp1_end.value;
else if (v=='dhcp1_end') document.form.dhcp1_end.value = document.form.dhcp1_start.value;
o.focus();
o.select();
return false;
}
else return true;
}
if(v=='wan_ipaddr_x' && document.form.wan_netmask_x.value=="")
document.form.wan_netmask_x.value="255.255.255.0";
for(i=0; i<o.value.length; i++) {
c=o.value.charAt(i);
if(c>='0' && c<='9')
{if ( num==-1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else
{if ( num<0 || num>255 || c!='.')
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
if (pos==0) v1=num;
else if (pos==1) v2=num;
else if (pos==2) v3=num;
num = -1;
pos++;
}
}
if (pos!=3 || num<0 || num>255)
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
else v4=num;
if (v=='dhcp_start' || v=='dhcp_end' || v=='wan_ipaddr_x' || v=='dhcp1_start' || v=='dhcp1_end' || v=='lan_ipaddr' || v=='lan1_ipaddr' || v=='staticip')
{
if(v != 'wan_ipaddr_x' && (v1 == 255 || v4 == 255 || v1 == 0 || v4 == 0 || v1 == 127 || v1 == 224))
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
if ((v=='wan_ipaddr_x' && matchSubnet2(o.value, document.form.wan_netmask_x, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_ipaddr' && matchSubnet2(o.value, document.form.lan_netmask, document.form.wan_ipaddr_x.value, document.form.wan_netmask_x)))
{
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
if (v=='wan_ipaddr_x')
{document.form.wan_ipaddr_x.value = "10.1.1.1";
document.form.wan_netmask_x.value = "255.0.0.0";
}
else if(v=='lan_ipaddr')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v=='lan1_ipaddr')
{document.form.lan1_ipaddr.value = "192.168.2.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
o.select();
return false;
}
}
else if(v=='lan_netmask' || v=='lan1_netmask')
{if(v1==255&&v2==255&&v3==255&&v4==255)
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
}
if (requireWANIP(v) && (
(v=='wan_netmask_x' && matchSubnet2(document.form.wan_ipaddr_x.value, o, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_netmask' && matchSubnet2(document.form.lan_ipaddr.value, o, document.form.wan_ipaddr_x.value, document.form.wan_netmask_x))))
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
if (v=='wan_netmask_x')
{document.form.wan_ipaddr_x.value = "10.1.1.1";
document.form.wan_netmask_x.value = "255.0.0.0";
}
else if(v=='lan_netmask')
{document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v=='lan1_netmask')
{document.form.lan1_ipaddr.value = "192.168.2.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
o.select();
return false;
}

o.value = v1 + "." + v2 + "." + v3 + "." + v4;
if ((v1 > 0) && (v1 < 127)) mask = "255.0.0.0";
else if ((v1 > 127) && (v1 < 192)) mask = "255.255.0.0";
else if ((v1 > 191) && (v1 < 224)) mask = "255.255.255.0";
else mask = "0.0.0.0";
if (v=='wan_ipaddr_x' && document.form.wan_netmask_x.value=="")
{document.form.wan_netmask_x.value = mask;
}
else if (v=='lan_ipaddr' && document.form.lan_netmask.value=="" )
{document.form.lan_netmask.value = mask;
}
else if (v=='dhcp_start')
{if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3))
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
if (intoa(o.value)>intoa(document.form.dhcp_end.value))
{tmp = document.form.dhcp_start.value;
document.form.dhcp_start.value = document.form.dhcp_end.value;
document.form.dhcp_end.value = tmp;
}
}
else if (v=='dhcp_end')
{if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3))
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
if (intoa(document.form.dhcp_start.value)>intoa(o.value))
{tmp = document.form.dhcp_start.value;
document.form.dhcp_start.value = document.form.dhcp_end.value;
document.form.dhcp_end.value = tmp;
}
}
else if (v=='lan1_ipaddr')
{if(document.form.lan1_netmask.value=="" )
document.form.lan1_netmask.value = mask;
}
else if (v=='dhcp1_start')
{if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3))
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
if (intoa(o.value)>intoa(document.form.dhcp1_end.value))
{tmp = document.form.dhcp1_start.value;
document.form.dhcp1_start.value = document.form.dhcp1_end.value;
document.form.dhcp1_end.value = tmp;
}
}
else if (v=='dhcp1_end')
{if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3))
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
if (intoa(document.form.dhcp1_start.value)>intoa(o.value))
{tmp = document.form.dhcp1_start.value;
document.form.dhcp1_start.value = document.form.dhcp1_end.value;
document.form.dhcp1_end.value = tmp;
}
}
return true;
}
function validate_ipaddr_final(o, v){
var num = -1;
var pos = 0;
if(o.value.length == 0){
if(v == 'dhcp_start' || v == 'dhcp_end' ||
v == 'wan_ipaddr_x' ||
v == 'dhcp1_start' || v=='dhcp1_end' ||
v == 'lan_ipaddr' || v=='lan_netmask' ||
v=='lan1_ipaddr' || v=='lan1_netmask' ||
v == 'wl_radius_ipaddr') {
alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
if(v == 'wan_ipaddr_x'){
document.form.wan_ipaddr_x.value = "10.1.1.1";
document.form.wan_netmask_x.value = "255.0.0.0";
}
else if(v == 'lan_ipaddr'){
document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v == 'lan1_ipaddr'){
document.form.lan1_ipaddr.value = "192.168.2.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
else if(v == 'lan_netmask')
document.form.lan_netmask.value = "255.255.255.0";
else if(v == 'lan1_netmask')
document.form.lan1_netmask.value = "255.255.255.0";
o.focus();
o.select();
return false;
}
else
return true;
}
if(v == 'wan_ipaddr_x' && document.form.wan_netmask_x.value == "")
document.form.wan_netmask_x.value="255.255.255.0";
for(var i = 0; i < o.value.length; ++i){
var c = o.value.charAt(i);
if(c >= '0' && c <= '9'){
if(num == -1 ){
num = (c-'0');
}
else{
num = num*10+(c-'0');
}
}
else{
if(num < 0 || num > 255 || c != '.'){
alert(o.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
if(pos == 0)
v1 = num;
else if(pos == 1)
v2 = num;
else if(pos == 2)
v3 = num;
num = -1;
++pos;
}
}
if(pos!=3 || num<0 || num>255){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
else
v4 = num;
if(v == 'dhcp_start' || v == 'dhcp_end' ||
v == 'wan_ipaddr_x' ||
v == 'dhcp1_start' || v == 'dhcp1_end' ||
v == 'lan_ipaddr' || v == 'lan1_ipaddr' ||
v == 'staticip' || v == 'wl_radius_ipaddr' ||
v == 'dhcp_dns1_x' || v == 'dhcp_gateway_x' || v == 'dhcp_wins_x'){
if((v!='wan_ipaddr_x')&& (v1==255||v4==255||v1==0||v4==0||v1==127||v1==224)){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
if((wan_route_x == "IP_Bridged" && wan_nat_x == "0") || sw_mode=="2" || sw_mode=="3") // variables are defined in state.js
; // there is no WAN in AP mode, so it wouldn't be compared with the wan ip..., etc.
else if(requireWANIP(v) && (
(v=='wan_ipaddr_x' && matchSubnet2(o.value, document.form.wan_netmask_x, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_ipaddr' && matchSubnet2(o.value, document.form.lan_netmask, document.form.wan_ipaddr_x.value, document.form.wan_netmask_x)))){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
if(v == 'wan_ipaddr_x'){
document.form.wan_ipaddr_x.value = "10.1.1.1";
document.form.wan_netmask_x.value = "255.0.0.0";
}
else if(v == 'lan_ipaddr'){
document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v == 'lan1_ipaddr'){
document.form.lan1_ipaddr.value = "192.168.2.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
o.select();
return false;
}
}
else if(v=='lan_netmask' || v=='lan1_netmask'){
if(v1==255&&v2==255&&v3==255&&v4==255){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
}
if((wan_route_x == "IP_Bridged" && wan_nat_x == "0") || sw_mode=="2" || sw_mode=="3") // variables are defined in state.js
; // there is no WAN in AP mode, so it wouldn't be compared with the wan ip..., etc.
else if(requireWANIP(v) && (
(v=='wan_netmask_x' && matchSubnet2(document.form.wan_ipaddr_x.value, o, document.form.lan_ipaddr.value, document.form.lan_netmask)) ||
(v=='lan_netmask' && matchSubnet2(document.form.lan_ipaddr.value, o, document.form.wan_ipaddr_x.value, document.form.wan_netmask_x)))){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
if (v=='wan_netmask_x'){
document.form.wan_ipaddr_x.value = "10.1.1.1";
document.form.wan_netmask_x.value = "255.0.0.0";
}
else if(v=='lan_netmask'){
document.form.lan_ipaddr.value = "192.168.1.1";
document.form.lan_netmask.value = "255.255.255.0";
}
else if(v=='lan1_netmask'){
document.form.lan1_ipaddr.value = "192.168.2.1";
document.form.lan1_netmask.value = "255.255.255.0";
}
o.focus();
o.select();
return false;
}

o.value = v1 + "." + v2 + "." + v3 + "." + v4;
if((v1 > 0) && (v1 < 127)) mask = "255.0.0.0";
else if ((v1 > 127) && (v1 < 192)) mask = "255.255.0.0";
else if ((v1 > 191) && (v1 < 224)) mask = "255.255.255.0";
else mask = "0.0.0.0";
if(v=='wan_ipaddr_x' && document.form.wan_netmask_x.value==""){
document.form.wan_netmask_x.value = mask;
}
else if (v=='lan_ipaddr' && document.form.lan_netmask.value=="" ){
document.form.lan_netmask.value = mask;
}else if (v=='dhcp_start'){
if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3)){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
}
else if (v=='dhcp_end'){
if (!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3)){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
}
else if (v=='lan1_ipaddr'){
if(document.form.lan1_netmask.value=="") document.form.lan1_netmask.value = mask;
}
else if (v=='dhcp1_start'){
if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3)){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
}
else if (v=='dhcp1_end'){
if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3)){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
}
return true;
}

function validate_ipaddrport(o, v)
{num = -1;
pos = 0;
if (o.value.length==0)
return true;
str = o.value;
portidx = str.indexOf(":");
if (portidx!=-1)
{port = str.substring(portidx+1);
len = portidx;
if (port>65535)
{alert(port + " <%tcWebApi_get("String_Entry","JS_validport","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
}
else
{len = o.value.length;
}
for(i=0; i<len; i++) {
c=o.value.charAt(i);
if(c>='0' && c<='9')
{if ( num==-1 )
{num = (c-'0');
}
else
{num = num*10 + (c-'0');
}
}
else
{if ( num<0 || num>255 || c!='.')
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
num = -1;
pos++;
}
}
if (pos!=3 || num<0 || num>255)
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
if (v=='ExternalIPAddress' && document.form.wan_netmask_x.value == '')
{document.form.wan_netmask_x.value = "255.255.255.0";
}
else if (v=='IPRouters' && document.form.lan_netmask.value == '')
{document.form.lan_netmask.value = "255.255.255.0";
}
return true;
}
function validate_portrange(o, v){
if(o.value.length == 0)
return true;
prev = -1;
num = -1;
num_front = 0;
for(var i = 0; i < o.value.length; i++){
c=o.value.charAt(i);
if (c>='0'&&c<='9'){
if(num==-1) num=0;
num = num*10 + (c-'0');
}
else{
if (num>65535 || num==0 || (c!=':' && c!='>' && c!='<' && c!=',')){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validport","s")%>");
o.focus();
o.select();
return false;
}
if (c=='>') prev = -2;
else if (c=='<') prev = -3;
else if (c==','){
prev = -4;
num = 0;
}
else{ //when c=":"
if (prev==-4)
prev == -4;
else{
prev = num;
num = 0;
}
}
}
}
if ((num>65535 && prev!=-3) || (num<1&&prev!=-2) || (prev>num) || (num>=65535&&prev==-2) || (num<=1&&prev==-3)){
if (num>65535){
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validport","s")%>");
o.focus();
o.select();
return false;
}
else{
alert(o.value + " is not a valid port range!");
o.focus();
o.select();
return false;
}
} // wrong port
else{
if (prev==-2){
if (num==65535) o.value = num;
else o.value = (num+1) + ":65535"; //ex. o.value=">2000", it will change to 2001:65535
}
else if (prev==-3){
if (num==1) o.value = num;
else o.value = "1:" + (num-1); //ex. o.value="<2000", it will change to 1:1999
}
else if (prev==-4){
if(document.form.current_page.value == "Advanced_VirtualServer_Content.asp"){ //2008.10.09 Lock add Allow input "," in Virtual Server page
multi_vts_port = o.value.split(",");
split_vts_rule(multi_vts_port);
return false;
}
else{
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validport","s")%>");
o.focus();
o.select();
return false;
}
}
else if (prev!=-1)
o.value = prev + ":" + num;
else
o.value = num; //single port number case;
}// correct port
return true;
}

function validate_portlist(o, v)
{if (o.value.length==0)
return true;
num = 0;
for(i=0; i<o.value.length; i++)
{c=o.value.charAt(i);
if (c>='0'&&c<='9')
{num = num*10 + (c-'0');
}
else
{if (num>255)
{alert(num + " <%tcWebApi_get("String_Entry","JS_validport","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
num = 0;
}
}
if (num>255)
{alert(num + " <%tcWebApi_get("String_Entry","JS_validport","s")%>");
o.value = "";
o.focus();
o.select();
return false;
}
return true;
}
function add_option_match(o, s, f)
{tail = o.options.length;
o.options[tail] = new Option(s);
o.options[tail].value = s;
if (f==s)
{o.options[tail].selected = 1;
return(1);
}
else return(0);
}
function add_option_match_x(o, s, f)
{tail = o.options.length;
o.options[tail] = new Option(s);
o.options[tail].value = tail;
if (tail==f)
{o.options[tail].selected = 1;
return(1);
}
else return(0);
}
function find_option(o)
{count = o.options.length;
for (i=0; i<count; i++)
{if (o.options[i].selected)
return(o.options[i].value);
}
return(null);
}
function add_options(o, arr, orig){
for(var i = 0; i < arr.length; i++){
if(orig == arr[i])
add_option(o, arr[i], arr[i], 1); // obj, str, val, sel
else
add_option(o, arr[i], arr[i], 0);
}
}
function add_options_x(o, arr, orig){
for(var i = 0; i < arr.length; i++){
if(orig == i)
add_option(o, i, arr[i], arr[i], 1);
else
add_option(o, i, arr[i], arr[i], 0);
}
}
function add_options_x2(o, arr, varr, orig){
free_options(o);
for(var i = 0; i < arr.length; ++i){
if(orig == varr[i])
add_option(o, arr[i], varr[i], 1);
else
add_option(o, arr[i], varr[i], 0);
}
}
function add_a_option(o, i, s)
{tail = o.options.length;
o.options[tail] = new Option(s);
o.options[tail].value = i;
}
function rcheck(o){
if(o.value == "1")
return("1");
else
return("0");
}
function SaveChannelList(o)
{count = o.options.length;
chanList = new Array;
for (i=0; i<count; i++)
{chanList[chanList.length] = o.options[i].value;
}
}
function RestoreChannelList(d, isOut, isTurbo)
{for (i=0; i< chanList.length; i++)
{if (isOut=="0"&&chanList[i]>=100) return;
if (chanList[i] == 999) return;
d[d.length] = chanList[i];
}
}
function RefreshChannelList(isOut, isTurbo)
{orig = document.form.WLANConfig11a_Channel.value;
free_options(document.form.WLANConfig11a_Channel);
if (isTurbo=="1")
{if (isOut == "1")
{items = new Array("42","50","58","152","160");
}
else
{items = new Array("42","50","58");
}
}
else
{items = new Array;
RestoreChannelList(items, isOut, isTurbo);
}
add_options(document.form.WLANConfig11a_Channel, items, orig);
}
function RefreshRateList(isTurbo)
{orig = document.form.WLANConfig11a_DataRate.value;
free_options(document.form.WLANConfig11a_DataRate);
if (isTurbo=="1")
{items = new Array("Best","12","18","24","36","48","72","96","108");
}
else
{items = new Array("Best","6","9","12","18","24","36","48","54");
}
add_options_x(document.form.WLANConfig11a_DataRate, items, orig);
}
function RefreshRateSetList(gmode, chg)
{orig = document.form.WLANConfig11b_DataRate.value;
free_options(document.form.WLANConfig11b_DataRate);
if (gmode != "3")
{if (gmode == "1")
{items = new Array("1 & 2 Mbps", "Default", "All");
}
else
{items = new Array("1 & 2 Mbps", "Default", "All");
}
if (chg) orig = 1;
}
else
{items = new Array("1 & 2 Mbps", "Default");
if (chg) orig = 1;
}
add_options_x(document.form.WLANConfig11b_DataRate, items, orig);
}
function getDateCheck(str, pos)
{if (str.charAt(pos) == '1')
return true;
else
return false;
}
function getTimeRange(str, pos)
{if (pos == 0)
return str.substring(0,2);
else if (pos == 1)
return str.substring(2,4);
else if (pos == 2)
return str.substring(4,6);
else if (pos == 3)
return str.substring(6,8);
}
function setDateCheck(d1, d2, d3, d4, d5, d6, d7)
{str = "";
if (d7.checked == true ) str = "1" + str;
else str = "0" + str;
if (d6.checked == true ) str = "1" + str;
else str = "0" + str;
if (d5.checked == true ) str = "1" + str;
else str = "0" + str;
if (d4.checked == true ) str = "1" + str;
else str = "0" + str;
if (d3.checked == true ) str = "1" + str;
else str = "0" + str;
if (d2.checked == true ) str = "1" + str;
else str = "0" + str;
if (d1.checked == true ) str = "1" + str;
else str = "0" + str;
return str;
}
function setTimeRange(sh, sm, eh, em)
{return(sh.value+sm.value+eh.value+em.value);
}
function load_body(){
document.form.next_host.value = location.host;
if(document.form.current_page.value == "Advanced_WMode_Content.asp"){
change_wireless_bridge(document.form.wl_mode_x.value);
}
else if(document.form.current_page.value == "Advanced_PortTrigger_Content.asp"){
wItem = new Array(new Array("Quicktime 4 Client", "554", "TCP", "6970:32000", "UDP"),new Array("Real Audio", "7070", "TCP", "6970:7170", "UDP"));
free_options(document.form.TriggerKnownApps);
add_option(document.form.TriggerKnownApps, "<% tcWebApi_Get("String_Entry", "Select_menu_default", "s") %>", "User Defined", 1);
for (i = 0; i < wItem.length; i++){
add_option(document.form.TriggerKnownApps, wItem[i][0], wItem[i][0], 0);
}
}
else if(document.form.current_page.value == "Advanced_BasicFirewall_Content.asp"){
change_firewall(rcheck(document.form.fw_enable_x));
}
else if(document.form.current_page.value == "Advanced_URLFilter_Content.asp"){
document.form.url_date_x_Sun.checked = getDateCheck(document.form.url_date_x.value, 0);
document.form.url_date_x_Mon.checked = getDateCheck(document.form.url_date_x.value, 1);
document.form.url_date_x_Tue.checked = getDateCheck(document.form.url_date_x.value, 2);
document.form.url_date_x_Wed.checked = getDateCheck(document.form.url_date_x.value, 3);
document.form.url_date_x_Thu.checked = getDateCheck(document.form.url_date_x.value, 4);
document.form.url_date_x_Fri.checked = getDateCheck(document.form.url_date_x.value, 5);
document.form.url_date_x_Sat.checked = getDateCheck(document.form.url_date_x.value, 6);
document.form.url_time_x_starthour.value = getTimeRange(document.form.url_time_x.value, 0);
document.form.url_time_x_startmin.value = getTimeRange(document.form.url_time_x.value, 1);
document.form.url_time_x_endhour.value = getTimeRange(document.form.url_time_x.value, 2);
document.form.url_time_x_endmin.value = getTimeRange(document.form.url_time_x.value, 3);
document.form.url_time_x_starthour_1.value = getTimeRange(document.form.url_time_x_1.value, 0);
document.form.url_time_x_startmin_1.value = getTimeRange(document.form.url_time_x_1.value, 1);
document.form.url_time_x_endhour_1.value = getTimeRange(document.form.url_time_x_1.value, 2);
document.form.url_time_x_endmin_1.value = getTimeRange(document.form.url_time_x_1.value, 3);
}
else if(document.form.current_page.value == "Advanced_DHCP_Content.asp"){
final_flag = 1;
}
change = 0;
}

function change_firewall(r)
{if (r == "0")
{
//inputRCtrl1(document.form.misc_http_x, 0); //Ren: move to Administration page
//inputCtrl(document.form.misc_httpport_x, 0); //Ren: move to Administration page
inputCtrl(document.form.fw_log_x, 0);
inputRCtrl1(document.form.misc_ping_x, 0);
}
else //r=="1"
{
//inputRCtrl1(document.form.misc_http_x, 1); //Ren: move to Administration page
//inputCtrl(document.form.misc_httpport_x, 1); //Ren: move to Administration page
inputCtrl(document.form.fw_log_x, 1);
inputRCtrl1(document.form.misc_ping_x, 1);
}
}
function change_wireless_bridge(m){
	if (m == "0"){
		inputRCtrl2(document.form.wl_wdsapply_x, 1);
		inputRCtrl1(document.form.wl_wdsapply_x, 0);
	}else if (m == "1"){ // N66U-spec
		inputRCtrl2(document.form.wl_wdsapply_x, 0);
		inputRCtrl1(document.form.wl_wdsapply_x, 0);
		/*if (document.form.wl_channel_orig.value == "0" && document.form.wl_channel.options[0].selected == 1){
			alert("<%tcWebApi_get("String_Entry","JS_fixchannel","s")%>");
			document.form.wl_channel.options[0].selected = 0;
			document.form.wl_channel.options[1].selected = 1;
		}*/
	}else{ //m=="2"
		inputRCtrl1(document.form.wl_wdsapply_x, 1);
		/*if (m != 0) {
			if (document.form.wl_channel_orig.value == "0" && document.form.wl_channel.options[0].selected == 1){
				alert("<%tcWebApi_get("String_Entry","JS_fixchannel","s")%>");
				document.form.wl_channel.options[0].selected = 0;
				document.form.wl_channel.options[1].selected = 1;
			}
		}*/
	}
}
function onSubmit(){
change = 0;
pageChanged = 0;
pageChangedCount = 0;
if (document.form.current_page.value == "Advanced_RLANWAN_Content.asp")
{checkSubnet();
}
else if (document.form.current_page.value == "Advanced_DMZIP_Content.asp")
{checkDmzSubnet();
}
else if (document.form.current_page.value == "Advanced_LFirewall_Content.asp")
{updateDateTime(document.form.current_page.value);
}
else if (document.form.current_page.value == "Advanced_WebCam_Content.asp")
{updateDateTime(document.form.current_page.value);
}

return true;
}
function onSubmitCtrl(o, s) {
document.form.action_mode.value = s;
return (onSubmit());
}
function onSubmitCtrlOnly(o, s){
if(s != 'Upload' && s != 'Upload1')
document.form.action_mode.value = s;
if(s == 'Upload1'){
if(document.form.file.value){
setTimeout("detect_firmware();", 200000); //Viz add 2011.11
setTimeout("updateStatus_AJAX();", 200000); //Viz add 2011.11
dr_advise();
disableCheckChangedStatus();
document.form.submit();
}
else{
alert("<%tcWebApi_get("String_Entry","JS_Shareblanktest","s")%>");
document.form.file.focus();
}
}
stopFlag = 1;
disableCheckChangedStatus();
return true;
}
function validate_ddns_hostname(o)
{dot=0;
s=o.value;
var unvalid_start=new RegExp("^[0-9].*", "gi");
if(unvalid_start.test(s) ) // || valid_keyarr.length >2 || valid_keyarr[0]=="" ||valid_keyarr[1] !=""
{alert("<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_7","s")%>");
return false;
}
if (!validate_string(o))
{return false;
}
for(i=0;i<s.length;i++)
{c = s.charCodeAt(i);
if (c==46)
{dot++;
if(dot>0)
{alert("<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_7","s")%>");
return false;
}
}
if (!validate_hostnamechar(c))
{alert("<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_13","s")%> " + s.charAt(i) +"' !");
return(false);
}
}
return(true);
}
function validate_hostnamechar(ch)
{if (ch>=48&&ch<=57) return true;
if (ch>=97&&ch<=122) return true;
if (ch>=65&&ch<=90) return true;
if (ch==45) return true;
if (ch==46) return true;
return false;
}
function onSubmitApply(s){
pageChanged = 0;
pageChangedCount = 0;
if(document.form.current_page.value == "Advanced_ASUSDDNS_Content.asp"){
if(s == "hostname_check"){
showLoading();
if(!validate_ddns_hostname(document.form.ddns_hostname_x)){
hideLoading();
return false;
}
}
document.form.action_mode.value = "Update";
document.form.action_script.value = s;
}
else{
document.form.action_mode.value = "Update";
document.form.action_script.value = s;
}
return true;
}
function setup_script(s)
{if (document.form.current_page.value == "Advanced_ACL_Content.asp")
{document.form.action_script.value = s;
}
}

function automode_hint(){ //Lock add 2009.11.05 for 54Mbps limitation in auto mode + WEP/TKIP.
	if((document.form.wl_nmode_x.value == "9" || document.form.wl_nmode_x.value == "8" || document.form.wl_nmode_x.value == "14") &&	//9:2.4GHz auto, 8:5GHz auto, 14:5GHz AC auto
		(document.form.wl_wep_x.value == 1 || document.form.wl_wep_x.value == 2 || document.form.wl_auth_mode_x.value == "radius" || (document.form.wl_crypto.value.indexOf("TKIP") == 0 && !document.form.wl_crypto.disabled))){
		if(document.form.current_page.value == "Advanced_Wireless_Content.asp"
				|| document.form.current_page.value == "device-map/router.asp"
				|| document.form.current_page.value.indexOf("Guest_network.asp"))
						$("wl_nmode_x_hint").style.display = "";
	}
	else{
		if(document.form.current_page.value == "Advanced_Wireless_Content.asp"
				|| document.form.current_page.value == "device-map/router.asp"
				|| document.form.current_page.value.indexOf("Guest_network.asp"))
						$("wl_nmode_x_hint").style.display = "none";
	}
}
function nmode_limitation(){ //Lock add 2009.11.05 for TKIP limitation in n mode.
if(document.form.wl_nmode_x.value == "6" || document.form.wl_nmode_x.value == "11"){	//6:2.4GHz NOnly 11:5GHz NOnly
if(document.form.wl_auth_mode_x.selectedIndex == 0 && (document.form.wl_wep_x.selectedIndex == "1" || document.form.wl_wep_x.selectedIndex == "2")){
alert("<%tcWebApi_get("String_Entry","WC11n_nmode_limition_hint","s")%>");
document.form.wl_auth_mode_x.selectedIndex = 1;
document.form.wl_crypto.selectIndex = 0;
}
else if(document.form.wl_auth_mode_x.selectedIndex == 2){
alert("<%tcWebApi_get("String_Entry","WC11n_nmode_limition_hint","s")%>");
document.form.wl_auth_mode_x.selectedIndex = 1;
document.form.wl_crypto.selectIndex = 0;
}
else if(document.form.wl_auth_mode_x.selectedIndex == 4){
alert("<%tcWebApi_get("String_Entry","WC11n_nmode_limition_hint","s")%>");
document.form.wl_auth_mode_x.selectedIndex = 3;
document.form.wl_crypto.selectIndex = 0;
}

wl_auth_mode_change(0);
}
}
function change_common(o, s, v){
	change = 1;
	pageChanged = 1;
	if(v == "wl_auth_mode_x"){ /* Handle AuthenticationMethod Change */
		wl_auth_mode_change(0);
		if(o.value == "psk" || o.value == "psk2" || o.value == "pskpsk2" || o.value == "wpa" || o.value == "wpawpa2"){
			opts = document.form.wl_auth_mode_x.options;
			if(opts[opts.selectedIndex].text == "WPA-Personal"){
				document.form.wl_wpa_mode.value="1";
				automode_hint();
			}
			else if(opts[opts.selectedIndex].text == "WPA2-Personal")
				document.form.wl_wpa_mode.value="2";
			else if(opts[opts.selectedIndex].text == "WPA-Auto-Personal")
				document.form.wl_wpa_mode.value="0";
			else if(opts[opts.selectedIndex].text == "WPA-Enterprise")
				document.form.wl_wpa_mode.value="3";
			else if(opts[opts.selectedIndex].text == "WPA-Auto-Enterprise")
				document.form.wl_wpa_mode.value="4";
			if(o.value == "psk" || o.value == "psk2" || o.value == "pskpsk2"){
				document.form.wl_wpa_psk.focus();
				document.form.wl_wpa_psk.select();
			}
		}
		else if(o.value == "shared"){ //2009.03.10 Lock
			document.form.wl_key1.focus();
			document.form.wl_key1.select();
		}
		nmode_limitation();
		automode_hint();
	}
	else if (v == "wl_crypto"){
		wl_auth_mode_change(0);
		nmode_limitation();
		automode_hint();
	}
	else if(v == "wl_wep_x"){ /* Handle AuthenticationMethod Change */
		change_wlweptype(o, "WLANConfig11b");
		nmode_limitation();
		automode_hint();
	}
	else if(v == "wl_key"){ /* Handle AuthenticationMethod Change */
		var selected_key = eval("document.form.wl_key"+o.value);
		selected_key.focus();
		selected_key.select();
	}
	else if(s=="WLANConfig11b" && v == "wl_channel"){
		insertExtChannelOption();
	}
	else if(s=="WLANConfig11b" && v == "wl_bw"){
		insertExtChannelOption();
	}
	else if (s=="WLANConfig11b" && v=="wl_gmode_check"){
		if (document.form.wl_gmode_check.checked == true)
			document.form.wl_gmode_protection.value = "0";
		else
			document.form.wl_gmode_protection.value = "2";
	}
	else if(s=="WLANConfig11b" && v == "wl_nmode_x"){
		if(o.value=='9' || o.value=='0' || o.value=='8' || o.value=='2')
			inputCtrl(document.form.wl_gmode_check, 1);
		else
			inputCtrl(document.form.wl_gmode_check, 0);
				
		if(o.value == "0" || o.value == "2")
			inputCtrl(document.form.wl_bw, 0);
		else
			inputCtrl(document.form.wl_bw, 1);

		insertExtChannelOption();
		if(o.value == "3"){
			document.form.wl_wme.value = "on";
		}
		nmode_limitation();
		automode_hint();
		check_NOnly_to_GN();
	}
	else if (s=="WLANConfig11b" && v=="x_Mode11g"){
		RefreshRateSetList(document.form.WLANConfig11b_x_Mode11g.value, true);
	}
	else if (s=="WLANConfig11b" && v=="Channel" && document.form.current_page.value=="Advanced_WMode_Content.asp"){
		if (document.form.WLANConfig11b_x_APMode.value != "0" && document.form.WLANConfig11b_Channel.value == "0")
		{
			alert("<%tcWebApi_get("String_Entry","JS_fixchannel","s")%>");
			document.form.WLANConfig11b_Channel.options[0].selected = 0;
			document.form.WLANConfig11b_Channel.options[1].selected = 1;
		}
	}
	else if (v=="ddns_server_x"){
		change_ddns_setting(o.value);
	}
	else if (v == "wl_wme"){
		if(o.value == "off"){
			inputCtrl(document.form.wl_wme_no_ack, 0);
			if($("enable_wl_multicast_forward").style.display != "none")
				inputCtrl(document.form.wl_wmf_bss_enable, 0);
			inputCtrl(document.form.wl_wme_apsd, 0);
		}
		else{
			if(document.form.wl_nmode_x.value == "9" || document.form.wl_nmode_x.value == "8" || document.form.wl_nmode_x.value == "6" || document.form.wl_nmode_x.value == "11" || document.form.wl_nmode_x.value == "14"){ //2.4GHz 9:auto & 6:n only; 5GHz 8:auto & 11:n only; 5GHz AC 14: auto
				document.form.wl_wme_no_ack.value = "off";
				inputCtrl(document.form.wl_wme_no_ack, 0);
			}else
				inputCtrl(document.form.wl_wme_no_ack, 1);
			if(Rawifi_support == -1)
				inputCtrl(document.form.wl_wmf_bss_enable, 1);
			inputCtrl(document.form.wl_wme_apsd, 1);
		}
	}else if (v == "time_zone_select"){
		var tzdst = new RegExp("^[a-z]+[0-9\-\.:]+[a-z]+", "i");
		if(o.value.match(tzdst)){
			document.getElementById("chkbox_time_zone_dst").style.display="";
			if(!document.getElementById("time_zone_dst_chk").checked){
				document.form.time_zone_dst.value=0;
				document.getElementById("dst_start").style.display="none";
				document.getElementById("dst_end").style.display="none";
			}else{
				document.form.time_zone_dst.value=1;
				document.getElementById("dst_start").style.display="";
				document.getElementById("dst_end").style.display="";
			}
		}else{
			document.getElementById("chkbox_time_zone_dst").style.display="none";
			document.getElementById("time_zone_dst_chk").checked = false;
			document.form.time_zone_dst.value=0;
			document.getElementById("dst_start").style.display="none";
			document.getElementById("dst_end").style.display="none";
		}
	}else if (v == "time_zone_dst_chk"){
		if (document.form.time_zone_dst_chk.checked){
			document.form.time_zone_dst.value = "1";
			document.getElementById("dst_start").style.display="";
			document.getElementById("dst_end").style.display="";
		}else{
			document.form.time_zone_dst.value = "0";
			document.getElementById("dst_start").style.display="none";
			document.getElementById("dst_end").style.display="none";
		}
	}
	return true;
}
function change_ddns_setting(v){
	var hostname_x = '<% tcWebApi_get("Ddns_Entry","MYHOST","s"); %>';
	if (v == "WWW.ASUS.COM"){
		document.form.ddns_hostname_x.parentNode.style.display = "none";
		document.form.DDNSName.parentNode.style.display = "";
		var ddns_hostname_title = hostname_x.substring(0, hostname_x.indexOf('.asuscomm.com'));
		if(hostname_x != '' && ddns_hostname_title)
			document.getElementById("DDNSName").value = ddns_hostname_title;						
		else
			document.getElementById("DDNSName").value = "<% tcWebApi_Get("String_Entry", "asusddns_inputhint", "s") %>";

		inputCtrl(document.form.ddns_username_x, 0);
		inputCtrl(document.form.ddns_passwd_x, 0);
		document.form.ddns_wildcard_x[0].disabled= 1;
		document.form.ddns_wildcard_x[1].disabled= 1;
		showhide("link", 0);
		showhide("linkToHome", 0);	
		showhide("wildcard_field",0);
	}
	else{
		document.form.ddns_hostname_x.parentNode.style.display = "";
		document.form.DDNSName.parentNode.style.display = "none";
		inputCtrl(document.form.ddns_username_x, 1);
		inputCtrl(document.form.ddns_passwd_x, 1);
		if(v == "WWW.TUNNELBROKER.NET" || v == "WWW.SELFHOST.DE" || v == "DOMAINS.GOOGLE.COM")
			var disable_wild = 1;
		else
			var disable_wild = 0;	
		document.form.ddns_wildcard_x[0].disabled= disable_wild;
		document.form.ddns_wildcard_x[1].disabled= disable_wild;
		
		if(v == "WWW.ZONEEDIT.COM" || v == "DOMAINS.GOOGLE.COM"){
			showhide("link", 0);
			showhide("linkToHome", 1);
		}
		else{
			showhide("link", 1);
			showhide("linkToHome", 0);				
		} 
		
		showhide("wildcard_field",!disable_wild);
	}
}
function change_common_radio(o, s, v, r){
	change = 1;
	pageChanged = 1;
	if (v=='wl_closed'){
			if(r==1)
					showhide("WPS_hideSSID_hint",1);
			else
					showhide("WPS_hideSSID_hint",0);			
	}
	else if (v=='wps_mode')
	{
		if (r == '1')
			inputCtrl(document.form.wsc_sta_pin, 1);
		else
			inputCtrl(document.form.wsc_sta_pin, 0);
	}
	else if(v == "ddns_enable_x"){
		var hostname_x = '<% tcWebApi_get("Ddns_Entry","MYHOST","s") %>';
		if(r == 1){
			inputCtrl(document.form.ddns_server_x, 1);
			if('<% tcWebApi_get("Ddns_Entry","SERVERNAME","s") %>' == 'WWW.ASUS.COM'){
				document.form.DDNSName.disabled = false;
				document.form.DDNSName.parentNode.parentNode.parentNode.style.display = "";
				if(hostname_x != ''){
					var ddns_hostname_title = hostname_x.substring(0, hostname_x.indexOf('.asuscomm.com'));
					if(!ddns_hostname_title)
						document.getElementById("DDNSName").value = '<% tcWebApi_get("String_Entry", "asusddns_inputhint", "s") %>';
					else
						document.getElementById("DDNSName").value = ddns_hostname_title;						
				}else{
					document.getElementById("DDNSName").value = '<% tcWebApi_get("String_Entry", "asusddns_inputhint", "s") %>';
				}
				showhide("wildcard_field",0);
			}else{
				document.form.ddns_hostname_x.parentNode.parentNode.parentNode.style.display = "";
				inputCtrl(document.form.ddns_username_x, 1);
				inputCtrl(document.form.ddns_passwd_x, 1);
				showhide("wildcard_field",1);				
			}
			change_ddns_setting(document.form.ddns_server_x.value);			
		}else{
			if(document.form.ddns_server_x.value == "WWW.ASUS.COM"){
				document.form.DDNSName.parentNode.parentNode.parentNode.style.display = "none";
			}else{
				document.form.ddns_hostname_x.parentNode.parentNode.parentNode.style.display = "none";
				inputCtrl(document.form.ddns_username_x, 0);
				inputCtrl(document.form.ddns_passwd_x, 0);			
			}
			inputCtrl(document.form.ddns_server_x, 0);
			document.form.ddns_wildcard_x[0].disabled= 1;
			document.form.ddns_wildcard_x[1].disabled= 1;
			showhide("wildcard_field",0);
		}
		update_ddns_wan_unit_option();
	}
	else if(v == "wan_dnsenable_x"){
	if(r == 1){
	inputCtrl(document.form.wan_dns1_x, 0);
	inputCtrl(document.form.wan_dns2_x, 0);
	}
	else{
	inputCtrl(document.form.wan_dns1_x, 1);
	inputCtrl(document.form.wan_dns2_x, 1);
	}
	}
	else if (v=="fw_enable_x")
	{change_firewall(r);
	}
	else if (v=="lan_proto_x")
	{if (r == '1')
	{inputCtrl(document.form.lan_ipaddr, 0);
	inputCtrl(document.form.lan_netmask, 0);
	inputCtrl(document.form.lan_gateway, 0);
	}
	else
	{inputCtrl(document.form.lan_ipaddr, 1);
	inputCtrl(document.form.lan_netmask, 1);
	inputCtrl(document.form.lan_gateway, 1);
	}
	}
	else if (s=="PPPConnection" && v=="wan_pppoe_idletime")
	{if (document.form.wan_pppoe_idletime_check.checked)
	{document.form.wan_pppoe_txonly_x.value = "1";
	}
	else
	{document.form.wan_pppoe_txonly_x.value = "0";
	}
	}
	else if (s=="PPPConnection" && v=="x_IdleTime1")
	{if (document.form.PPPConnection_x_IdleTime1_check.checked)
	{document.form.PPPConnection_x_IdleTxOnly1.value = "1";
	}
	else
	{document.form.PPPConnection_x_IdleTxOnly1.value = "0";
	}
	}
	else if (s=="PPPConnection" && v=="x_MultiPPPoEEnable1")
	{if (document.form.PPPConnection_x_MultiPPPoEEnable1[0].checked == true)
	{flag=1;
	}
	else
	{flag=0;
	}
	inputCtrl(document.form.PPPConnection_x_UserName1, flag);
	inputCtrl(document.form.PPPConnection_x_Password1, flag);
	inputCtrl(document.form.PPPConnection_x_IdleTime1, flag);
	inputCtrl(document.form.PPPConnection_x_IdleTime1_check, flag);
	inputCtrl(document.form.PPPConnection_x_PPPoEMTU1, flag);
	inputCtrl(document.form.PPPConnection_x_PPPoEMRU1, flag);
	inputCtrl(document.form.PPPConnection_x_ServiceName1, flag);
	inputCtrl(document.form.PPPConnection_x_AccessConcentrator1, flag);
	}
	else if (s=="PPPConnection" && v=="x_IdleTime2")
	{if (document.form.PPPConnection_x_IdleTime2_check.checked)
	{document.form.PPPConnection_x_IdleTxOnly2.value = "1";
	}
	else
	{document.form.PPPConnection_x_IdleTxOnly2.value = "0";
	}
	}
	else if (s=="PPPConnection" && v=="x_MultiPPPoEEnable2")
	{if (document.form.PPPConnection_x_MultiPPPoEEnable2[0].checked == true)
	{flag=1;
	}
	else
	{flag=0;
	}
	inputCtrl(document.form.PPPConnection_x_UserName2, flag);
	inputCtrl(document.form.PPPConnection_x_Password2, flag);
	inputCtrl(document.form.PPPConnection_x_IdleTime2, flag);
	inputCtrl(document.form.PPPConnection_x_IdleTime2_check, flag);
	inputCtrl(document.form.PPPConnection_x_PPPoEMTU2, flag);
	inputCtrl(document.form.PPPConnection_x_PPPoEMRU2, flag);
	inputCtrl(document.form.PPPConnection_x_ServiceName2, flag);
	inputCtrl(document.form.PPPConnection_x_AccessConcentrator2, flag);
	}
	return true;
}
function valid_WPAPSK(o){
if(o.value.length >= 64){
o.value = o.value.substring(0, 63);
alert("<%tcWebApi_get("String_Entry","JS_wpapass","s")%>");
return false;
}
return true;
}
function encryptionType(authType, wepType)
{pflag = "1";
if (authType.value == "1")
{if (wepType.value == "0") wepLen = "64";
else wepLen = "128";
}
else if (authType.value == "2")
{wepLen = "0";
}
else if (authType.value == "3")
{wepLen = "0";
pflag = "0";
}
else if (authType.value == "4")
{if (wepType.value == "0") wepLen = "64";
else wepLen = "128";
}
else
{if (wepType.value == "0")
{wepLen = "0";
pflag = "0";
}
else if (wepType.value == "1") wepLen = "64";
else wepLen = "128";
}
return(pflag + wepLen);
}
function change_wlweptype(o, s, isload, edit_unit){
	var wflag = 0;
	
	if(o.value != "0"){
		wflag = 1;
		if(document.form.wl_phrase_x.value.length > 0 && isload == 0)
			is_wlphrase("WLANConfig11b", "wl_phrase_x", document.form.wl_phrase_x);
	}
	
	wl_wep_change(edit_unit);
}

function change_wlkey(o, s){
wep = document.form.wl_wep_x.value;
if(wep == "1"){
if(o.value.length > 10)
o.value = o.value.substring(0, 10);
}
else if(wep == "2"){
if(o.value.length > 26)
o.value = o.value.substring(0, 26);
}
return true;
}
function validate_timerange(o, p)
{
if (o.value.length==0)
o.value = "00";
else if (o.value.length==1)
o.value = "0" + o.value;
if (o.value.charAt(0)<'0' || o.value.charAt(0)>'9')
o.value = "00";
else if (o.value.charAt(1)<'0' || o.value.charAt(1)>'9')
o.value = "00";
else if (p==0 || p==2)
{
if(o.value>23){
alert("<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> 00 <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> 23");
o.value = "00";
o.focus();
o.select();
return false;
}
return true;
}
else
{
if(o.value>59){
alert("<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> 00 <%tcWebApi_get("String_Entry","JS_validrange_to","s")%> 59");
o.value = "00";
o.focus();
o.select();
return false;
}
return true;
}
return true;
}
function matchSubnet(ip1, ip2, count)
{var c = 0;
var v1 = 0;
var v2 = 0;
for(i=0;i<ip1.length;i++)
{if (ip1.charAt(i) == '.')
{if (ip2.charAt(i) != '.')
return false;
c++;
if (v1!=v2) return false;
v1 = 0;
v2 = 0;
}
else
{if (ip2.charAt(i)=='.') return false;
v1 = v1*10 + (ip1.charAt(i) - '0');
v2 = v2*10 + (ip2.charAt(i) - '0');
}
if (c==count) return true;
}
return false;
}
function subnetPrefix(ip, orig, count)
{r='';
c=0;
for(i=0;i<ip.length;i++)
{if (ip.charAt(i) == '.')
c++;
if (c==count) break;
r = r + ip.charAt(i);
}
c=0;
for(i=0;i<orig.length;i++)
{if (orig.charAt(i) == '.')
{c++;
}
if (c>=count)
r = r + orig.charAt(i);
}
return (r);
}
function subnetPostfix(ip, num, count){ //Change subnet postfix .xxx
r='';
orig="";
c=0;
for(i=0;i<ip.length;i++){
if (ip.charAt(i) == '.') c++;
r = r + ip.charAt(i);
if (c==count) break;
}
c=0;
orig = String(num);
for(i=0;i<orig.length;i++){
r = r + orig.charAt(i);
}
return (r);
}
function checkSubnet(){
/* Rule : If addresses in pool are match to subnet, don't change */
/* Rule : If addresses in pool are not match to subnet, change to subnet.2~subnet.254 */
if(!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3) ||
!matchSubnet(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3)){
if(confirm("<%tcWebApi_get("String_Entry","JS_DHCP1","s")%>")){
document.form.dhcp_start.value = subnetPrefix(document.form.lan_ipaddr.value, document.form.dhcp_start.value, 3);
document.form.dhcp_end.value = subnetPrefix(document.form.lan_ipaddr.value, document.form.dhcp_end.value, 3);
return true;
}else{
return false;
}
}
}
function checkDmzSubnet()
{/* Rule : If addresses in pool are match to subnet, don't change */
/* Rule : If addresses in pool are not match to subnet, change to subnet.2~subnet.254 */
if (!matchSubnet(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMinAddress.value, 3) ||
!matchSubnet(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMaxAddress.value, 3))
{if (confirm("The setting of DHCP server does not match current IP address of Wireless Firewall. Would you like to change it automatically?"))
{document.form.LANHostConfig_DmzMinAddress.value = subnetPrefix(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMinAddress.value, 3);
document.form.LANHostConfig_DmzMaxAddress.value = subnetPrefix(document.form.FirewallConfig_DmzIP.value, document.form.LANHostConfig_DmzMaxAddress.value, 3);
}
}
}
function checkSubnetGuest()
{
/* Rule : If addresses in pool are match to subnet, don't change */
/* Rule : If addresses in pool are not match to subnet, change to subnet.2~subnet.254 */
if (!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3) ||
!matchSubnet(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3))
{
if (confirm("<%tcWebApi_get("String_Entry","JS_DHCP1","s")%>"))
{
document.form.dhcp1_start.value = subnetPrefix(document.form.lan1_ipaddr.value, document.form.dhcp1_start.value, 3);
document.form.dhcp1_end.value = subnetPrefix(document.form.lan1_ipaddr.value, document.form.dhcp1_end.value, 3);
}
}
}
function wan_netmask_x_check(o) {
ip =intoa(document.form.wan_ipaddr_x.value);
gw =intoa(document.form.wan_gateway_x.value);
nm =intoa(document.form.wan_netmask_x.value);
lip=intoa(document.form.lan_ipaddr.value);
lnm=intoa(document.form.lan_netmask.value);
rem=1;
if (document.form.wan_ipaddr_x.value != '0.0.0.0' && (ip&lnm)==(lip&lnm))
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
document.form.wan_ipaddr_x.value="10.1.1.1";
document.form.wan_netmask_x.value="255.0.0.0";
o.focus();
o.select();
return false;
}
if (gw==0 || gw==0xffffffff || (ip&nm)==(gw&nm))
{return true;
}
else
{alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
o.focus();
o.select();
return false;
}
}
function updateDateTime(s)
{
if (s == "Advanced_Firewall_Content.asp")
{
document.form.filter_lw_date_x.value = setDateCheck(
document.form.filter_lw_date_x_Sun,
document.form.filter_lw_date_x_Mon,
document.form.filter_lw_date_x_Tue,
document.form.filter_lw_date_x_Wed,
document.form.filter_lw_date_x_Thu,
document.form.filter_lw_date_x_Fri,
document.form.filter_lw_date_x_Sat);
document.form.filter_lw_time_x.value = setTimeRange(
document.form.filter_lw_time_x_starthour,
document.form.filter_lw_time_x_startmin,
document.form.filter_lw_time_x_endhour,
document.form.filter_lw_time_x_endmin);
}
else if (s == "Advanced_URLFilter_Content.asp")
{
document.form.url_date_x.value = setDateCheck(
document.form.url_date_x_Sun,
document.form.url_date_x_Mon,
document.form.url_date_x_Tue,
document.form.url_date_x_Wed,
document.form.url_date_x_Thu,
document.form.url_date_x_Fri,
document.form.url_date_x_Sat);
document.form.url_time_x.value = setTimeRange(
document.form.url_time_x_starthour,
document.form.url_time_x_startmin,
document.form.url_time_x_endhour,
document.form.url_time_x_endmin);
document.form.url_time_x_1.value = setTimeRange(
document.form.url_time_x_starthour_1,
document.form.url_time_x_startmin_1,
document.form.url_time_x_endhour_1,
document.form.url_time_x_endmin_1);
}
else if (s == "Advanced_LFirewall_Content.asp")
{
document.form.FirewallConfig_WanLocalActiveDate.value = setDateCheck(
document.form.FirewallConfig_WanLocalActiveDate_Sun,
document.form.FirewallConfig_WanLocalActiveDate_Mon,
document.form.FirewallConfig_WanLocalActiveDate_Tue,
document.form.FirewallConfig_WanLocalActiveDate_Wed,
document.form.FirewallConfig_WanLocalActiveDate_Thu,
document.form.FirewallConfig_WanLocalActiveDate_Fri,
document.form.FirewallConfig_WanLocalActiveDate_Sat);
document.form.FirewallConfig_WanLocalActiveTime.value = setTimeRange(
document.form.FirewallConfig_WanLocalActiveTime_starthour,
document.form.FirewallConfig_WanLocalActiveTime_startmin,
document.form.FirewallConfig_WanLocalActiveTime_endhour,
document.form.FirewallConfig_WanLocalActiveTime_endmin);
}
}
function openLink(s){
if (s == 'x_DDNSServer'){
	if (document.form.ddns_server_x.value.indexOf("WWW.DYNDNS.ORG")!=-1)
		tourl = "https://account.dyn.com/services/zones/svc/add.html?_add_dns=c&trial=standarddns";
	else if (document.form.ddns_server_x.value == 'WWW.SELFHOST.DE')
		tourl = "http://WWW.SELFHOST.DE";
	else if (document.form.ddns_server_x.value == 'WWW.ZONEEDIT.COM')
		tourl = "http://www.zoneedit.com/";
	else if (document.form.ddns_server_x.value == 'WWW.DNSOMATIC.COM')
		tourl = "http://dnsomatic.com/create/";
	else if (document.form.ddns_server_x.value == 'WWW.TUNNELBROKER.NET')
		tourl = "http://www.tunnelbroker.net/register.php";
	else if (document.form.ddns_server_x.value == 'WWW.ASUS.COM')
		tourl = "";
	else if (document.form.ddns_server_x.value == 'WWW.NO-IP.COM')
		tourl = "http://www.no-ip.com/newUser.php";
	else if (document.form.ddns_server_x.value == 'DOMAINS.GOOGLE.COM')
		tourl = "https://domains.google/";
	else tourl = "";
	link = window.open(tourl, "DDNSLink","toolbar=yes,location=yes,directories=no,status=yes,menubar=yes,scrollbars=yes,resizable=yes,copyhistory=no,width=640,height=480");
}
else if (s=='x_NTPServer1'){
	tourl = "http://support.ntp.org/bin/view/Main/WebHome";
	link = window.open(tourl, "NTPLink","toolbar=yes,location=yes,directories=no,status=yes,menubar=yes,scrollbars=yes,resizable=yes,copyhistory=no,width=640,height=480");
}
}

/* input : s: service id, v: value name, o: current value */
/* output: wep key1~4 */
function is_wlphrase(s, v, o){
var pseed = new Array;
var wep_key = new Array(5);
if(v=='wl_wpa_psk')
return(valid_WPAPSK(o));
// note: current_page == "Advanced_Wireless_Content.asp"
wepType = document.form.wl_wep_x.value;
wepKey1 = document.form.wl_key1;
wepKey2 = document.form.wl_key2;
wepKey3 = document.form.wl_key3;
wepKey4 = document.form.wl_key4;

phrase = o.value;
if(wepType == "1"){
for(var i = 0; i < phrase.length; i++){
pseed[i%4] ^= phrase.charCodeAt(i);
}
randNumber = pseed[0] | (pseed[1]<<8) | (pseed[2]<<16) | (pseed[3]<<24);
for(var j = 0; j < 5; j++){
randNumber = ((randNumber*0x343fd)%0x1000000);
randNumber = ((randNumber+0x269ec3)%0x1000000);
wep_key[j] = ((randNumber>>16)&0xff);
}
wepKey1.value = binl2hex_c(wep_key);
for(var j = 0; j < 5; j++){
randNumber = ((randNumber * 0x343fd) % 0x1000000);
randNumber = ((randNumber + 0x269ec3) % 0x1000000);
wep_key[j] = ((randNumber>>16) & 0xff);
}
wepKey2.value = binl2hex_c(wep_key);
for(var j = 0; j < 5; j++){
randNumber = ((randNumber * 0x343fd) % 0x1000000);
randNumber = ((randNumber + 0x269ec3) % 0x1000000);
wep_key[j] = ((randNumber>>16) & 0xff);
}
wepKey3.value = binl2hex_c(wep_key);
for(var j = 0; j < 5; j++){
randNumber = ((randNumber * 0x343fd) % 0x1000000);
randNumber = ((randNumber + 0x269ec3) % 0x1000000);
wep_key[j] = ((randNumber>>16) & 0xff);
}
wepKey4.value = binl2hex_c(wep_key);
}
else if(wepType == "2"){
password = "";
if(phrase.length > 0){
for(var i = 0; i < 64; i++){
ch = phrase.charAt(i%phrase.length);
password = password+ch;
}
}	
	for(i=0;i<4;i++){
			password = calcMD5(password);
			if(i == 0)
				wepKey1.value = password.substr(0, 26);
			else if(i == 1)
				wepKey2.value = password.substr(0, 26);
			else if(i == 2)
				wepKey3.value = password.substr(0, 26);
			else
				wepKey4.value = password.substr(0, 26);
	}
}
return true;
}

var wl0_nmode_x = '<% tcWebApi_get("WLan_Common","wl0_WirelessMode","s") %>';
var wl1_nmode_x = '<% tcWebApi_get("WLan_Common","wl1_WirelessMode","s") %>';
function wl_wep_change(edit_unit){	
var mode = document.form.wl_auth_mode_x.value;
var wep = document.form.wl_wep_x.value;

	if(mode == "psk" || mode == "psk2" || mode == "pskpsk2" || mode == "wpa" || mode == "wpa2" || mode == "wpawpa2"){
		if(mode != "wpa" && mode != "wpa2" && mode != "wpawpa2"){
			inputCtrl(document.form.wl_crypto, 1);
			inputCtrl(document.form.wl_wpa_psk, 1);
		}
		inputCtrl(document.form.wl_wpa_gtk_rekey, 1);
		inputCtrl(document.form.wl_wep_x, 0);
		inputCtrl(document.form.wl_phrase_x, 0);
		inputCtrl(document.form.wl_key1, 0);
		inputCtrl(document.form.wl_key2, 0);
		inputCtrl(document.form.wl_key3, 0);
		inputCtrl(document.form.wl_key4, 0);
		inputCtrl(document.form.wl_key, 0);
	}	
	else if(mode == "radius"){ //2009.01 magic
		inputCtrl(document.form.wl_crypto, 0);
		inputCtrl(document.form.wl_wpa_psk, 0);
		inputCtrl(document.form.wl_wpa_gtk_rekey, 0);
		inputCtrl(document.form.wl_wep_x, 0); //2009.0310 Lock
		inputCtrl(document.form.wl_phrase_x, 0);
		inputCtrl(document.form.wl_key1, 0);
		inputCtrl(document.form.wl_key2, 0);
		inputCtrl(document.form.wl_key3, 0);
		inputCtrl(document.form.wl_key4, 0);
		inputCtrl(document.form.wl_key, 0);
	}
	else{
		inputCtrl(document.form.wl_crypto, 0);
		inputCtrl(document.form.wl_wpa_psk, 0);
		inputCtrl(document.form.wl_wpa_gtk_rekey, 0);
				
		if(edit_unit != "undefine" || edit_unit != ""){	//for Guest network			
			if(mode == "open" && (edit_unit == '0' && wl0_nmode_x != 0)){	//0: 2.4GHz legacy
				document.form.wl_wep_x.parentNode.parentNode.style.display = "none";
				document.form.wl_wep_x.value = "0";
			}
			else if(mode == "open" && (edit_unit ==  '1' && wl1_nmode_x != 2)){ //2: 5GHz legacy
				document.form.wl_wep_x.parentNode.parentNode.style.display = "none";
				document.form.wl_wep_x.value = "0";			
			}	
			else{
				inputCtrl(document.form.wl_wep_x, 1);
				if(wep != "0"){
					inputCtrl(document.form.wl_phrase_x, 1);
					inputCtrl(document.form.wl_key1, 1);
					inputCtrl(document.form.wl_key2, 1);
					inputCtrl(document.form.wl_key3, 1);
					inputCtrl(document.form.wl_key4, 1);
					inputCtrl(document.form.wl_key, 1);
				}
				else{
					inputCtrl(document.form.wl_phrase_x, 0);
					inputCtrl(document.form.wl_key1, 0);
					inputCtrl(document.form.wl_key2, 0);
					inputCtrl(document.form.wl_key3, 0);
					inputCtrl(document.form.wl_key4, 0);
					inputCtrl(document.form.wl_key, 0);
				}
			}	
		}		
		else{	//for else wireless setting pages
			if(mode == "open" && (document.form.wl_nmode_x.value != 0 && document.form.wl_nmode_x.value != 2)){
				document.form.wl_wep_x.parentNode.parentNode.style.display = "none";
				document.form.wl_wep_x.value = "0";
			}	
			else{
				inputCtrl(document.form.wl_wep_x, 1);	
				if(wep != "0"){
					inputCtrl(document.form.wl_phrase_x, 1);
					inputCtrl(document.form.wl_key1, 1);
					inputCtrl(document.form.wl_key2, 1);
					inputCtrl(document.form.wl_key3, 1);
					inputCtrl(document.form.wl_key4, 1);
					inputCtrl(document.form.wl_key, 1);
				}
				else{
					inputCtrl(document.form.wl_phrase_x, 0);
					inputCtrl(document.form.wl_key1, 0);
					inputCtrl(document.form.wl_key2, 0);
					inputCtrl(document.form.wl_key3, 0);
					inputCtrl(document.form.wl_key4, 0);
					inputCtrl(document.form.wl_key, 0);
				}
			}	
		}			
	}
	change_key_des(); // 2008.01 James.
}

function change_wep_type(mode, isload, edit_unit){
	var cur_wep = "<% tcWebApi_get("WLan_Entry","wep_x", "s") %>";
	var wep_type_array;
	var value_array;

	free_options(document.form.wl_wep_x);
	if(mode == "shared"){ //2009.0310 Lock
		wep_type_array = new Array("WEP-64bits", "WEP-128bits");
		value_array = new Array("1", "2");
	}
	else{
		if((document.form.wl_nmode_x.value != 0 && '<% tcWebApi_get("WLan_Common","wl_unit","s"); %>' == '0')
				|| (document.form.wl_nmode_x.value != 2 && '<% tcWebApi_get("WLan_Common","wl_unit","s"); %>' == '1')){		//2.4G Legacy -> 0, 5G Legacy -> 2			
			wep_type_array = new Array("None");
			value_array = new Array("0");	
		}
		else{
			wep_type_array = new Array("None", "WEP-64bits", "WEP-128bits");
			value_array = new Array("0", "1", "2");
		}
	}
	add_options_x2(document.form.wl_wep_x, wep_type_array, value_array, cur_wep);
	if(mode == "psk" || mode == "psk2" || mode == "pskpsk2" || mode == "wpa" || mode == "wpa2" || mode == "wpawpa2" || mode == "radius") //2009.03 magic
			document.form.wl_wep_x.value = "0";
			
	change_wlweptype(document.form.wl_wep_x, "WLANConfig11b", isload, edit_unit);
}

function wl_auth_mode_reconf(){
if(document.form.wl_auth_mode_x.value == "radius" || document.form.wl_auth_mode_x.value == "wpa" || document.form.wl_auth_mode_x.value == "wpa2" || document.form.wl_auth_mode_x.value == "wpawpa2")
document.form.wl_auth_mode_x.value = "open";
for(var i = 5; i <= 8; ++i){
document.form.wl_auth_mode_x.options[i].value = null;
document.form.wl_auth_mode_x.options[i] = null;
}
}

function insertExtChannelOption(){
	if('<% tcWebApi_get("WLan_Common","wl_unit","s"); %>' == '1')
	{
		insertExtChannelOption_5g();
	}
	else
	{
		insertExtChannelOption_2g();
	}

}

function insertExtChannelOption_5g(){
    var country = "<% tcWebApi_get("WLan_Common","wl1_CountryCode","s"); %>";
    var orig = document.form.wl_channel.value;
    var wl_channel_list_5g = new Array(<% get_wl_channel_list_5g(); %>);
    var wl_channel_skiplist_5g = { <% get_wl_channel_skiplist_5g(); %> };    

    free_options(document.form.wl_channel);
    
    //With wireless channel 5g hook
		if(wl_channel_list_5g != ""){
				var wl_channel_list_5g_length = wl_channel_list_5g.length;
				var wl_channel_skiplist_5g_length = 0;				
				if(document.form.wl_bw.value == "2"){
						// bw== 40MHz
						wl_channel_skiplist_5g_length = wl_channel_skiplist_5g[40].length;
						for(var x=wl_channel_skiplist_5g_length;x>0;x--){
								for(var y=0;y<wl_channel_list_5g_length;y++){
									if(wl_channel_skiplist_5g[40][x-1] == wl_channel_list_5g[y])
										wl_channel_list_5g.splice(y,1)
								}	
						}
				} 
				else if(document.form.wl_bw.value == "3"){	
						// bw== 80MHz
						wl_channel_skiplist_5g_length = wl_channel_skiplist_5g[80].length;
						for(var x=wl_channel_skiplist_5g_length;x>0;x--){
								for(var y=0;y<wl_channel_list_5g_length;y++){
									if(wl_channel_skiplist_5g[80][x-1] == wl_channel_list_5g[y])
										wl_channel_list_5g.splice(y,1)
								}	
						}
				}
				else{	
						// bw== 20MHz
						wl_channel_skiplist_5g_length = wl_channel_skiplist_5g[20].length;
						for(var x=wl_channel_skiplist_5g_length;x>0;x--){
								for(var y=0;y<wl_channel_list_5g_length;y++){
									if(wl_channel_skiplist_5g[20][x-1] == wl_channel_list_5g[y])
										wl_channel_list_5g.splice(y,1)
								}	
						}						
				}
				 				
				if(wl_channel_list_5g[0] != 0)
					wl_channel_list_5g.splice(0,0,"0");
												
				channels = wl_channel_list_5g;
		}
		else{   	//start Without wireless channel 5g hook
        if (document.form.wl_bw.value == "0"){ // 20 MHz
						inputCtrl(document.form.wl_nctrlsb, 0);
                	if (country == "AL" || 
                	 country == "DZ" || 
			 country == "AU" || 
			 country == "BH" || 
              	         country == "BY" ||
              	         country == "CA" || 
              	         country == "CL" || 
              	         country == "CO" || 
                	 country == "CR" ||
                	 country == "DO" || 
                	 country == "SV" || 
                	 country == "GT" || 
			 country == "HN" || 
			 country == "HK" || 
              	         country == "IN" ||
              	         country == "IL" || 
              	         country == "JO" || 
              	         country == "KZ" || 
                	 country == "LB" ||
                	 country == "MO" || 
                	 country == "MK" ||                	 
                	 country == "MY" ||
                	 country == "MX" || 
			 country == "NZ" || 
			 country == "NO" || 
              	         country == "OM" ||
              	         country == "PK" || 
              	         country == "PA" || 
              	         country == "PR" || 
                	 country == "QA" ||
                	 country == "RO" || 
                	 country == "RU" || 
                	 country == "SA" || 
			 country == "SG" || 
			 country == "SY" || 
              	         country == "TH" ||
              	         country == "UA" || 
              	         country == "AE" || 
              	         country == "US" || 
              	         country == "Q2" || 
                	 country == "VN" ||
                	 country == "YE" || 
                	 country == "ZW")
                		channels = new Array(0, 36, 40, 44, 48, 149, 153, 157, 161, 165); // Region 0
                		
                	else if(country == "AT" ||
                		country == "BE" ||
            		    	country == "BR" ||
            		    	country == "BG" ||
            		    	country == "CY" || 
            		    	country == "DK" || 
            		    	country == "EE" ||
            		    	country == "FI" || 
            	  	        country == "DE" || 
            	  	        country == "GR" || 
                		country == "HU" ||
             		   	country == "IS" ||
             		   	country == "IE" || 
            		    	country == "IT" || 
            		    	country == "LV" ||
            		    	country == "LI" || 
            		    	country == "LT" || 
            		    	country == "LU" || 
            		    	country == "NL" ||
            		    	country == "PL" || 
            		    	country == "PT" || 
            		    	country == "SK" || 
            		    	country == "SI" ||
            		    	country == "ZA" || 
            		    	country == "ES" || 
            		    	country == "SE" || 
            		    	country == "CH" ||
            		    	country == "GB" || 
            		    	country == "EU" || 
            		    	country == "UZ")
                		channels = new Array(0, 36, 40, 44, 48);  // Region 1
                	
                	else if(country == "AM" ||
            		    	country == "AZ" || 
            		    	country == "HR" ||
            		    	country == "CZ" || 
            		    	country == "EG" || 
            		    	country == "FR" || 
            		    	country == "GE" ||
            		    	country == "MC" ||
            		    	country == "TT" || 
            		    	country == "TN" ||
            		    	country == "TR")
                		channels = new Array(0, 36, 40, 44, 48);  // Region 2
                	
                	else if(country == "AR" || country == "TW")
				channels = new Array(0, 52, 56, 60, 64, 149, 153, 157, 161, 165);  // Region 3
                	
                	else if(country == "BZ" ||
                		country == "BO" || 
            		    	country == "BN" ||
            		    	country == "CN" || 
            		    	country == "ID" || 
            		    	country == "IR" || 
            		    	country == "PE" ||
            		    	country == "PH")
                		channels = new Array(0, 149, 153, 157, 161, 165);  // Region 4
                	
                	else if(country == "KP" ||
                		country == "KR" || 
            		    	country == "UY" ||
            		    	country == "VE")
                		channels = new Array(0, 149, 153, 157, 161, 165);  // Region 5
                   	
									else if(country == "JP")
                		channels = new Array(0, 36, 40, 44, 48); // Region 9
									
									else
                		channels = new Array(0, 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165); // Region 7
                }
								else if(document.form.wl_bw.value == "1"){  // 20/40 MHz
									inputCtrl(document.form.wl_nctrlsb, 1);
                	if (country == "AL" || 
                	 country == "DZ" || 
			 country == "AU" || 
			 country == "BH" || 
              	         country == "BY" ||
              	         country == "CA" || 
              	         country == "CL" || 
              	         country == "CO" || 
                	 country == "CR" ||
                	 country == "DO" || 
                	 country == "SV" || 
                	 country == "GT" || 
			 country == "HN" || 
			 country == "HK" || 
              	         country == "IN" ||
              	         country == "IL" || 
              	         country == "JO" || 
              	         country == "KZ" || 
                	 country == "LB" ||
                	 country == "MO" || 
                	 country == "MK" ||                	 
                	 country == "MY" ||
                	 country == "MX" || 
			 country == "NZ" || 
			 country == "NO" || 
              	         country == "OM" ||
              	         country == "PK" || 
              	         country == "PA" || 
              	         country == "PR" || 
                	 country == "QA" ||
                	 country == "RO" || 
                	 country == "RU" || 
                	 country == "SA" || 
			 country == "SG" || 
			 country == "SY" || 
              	         country == "TH" ||
              	         country == "UA" || 
              	         country == "AE" || 
              	         country == "US" || 
              	         country == "Q2" || 
                	 country == "VN" ||
                	 country == "YE" || 
                	 country == "ZW")
                		channels = new Array(0, 36, 40, 44, 48, 149, 153, 157, 161); // Region 0
                		
                	else if(country == "AT" ||
                		country == "BE" ||
            		    	country == "BR" ||
            		    	country == "BG" ||
            		    	country == "CY" || 
            		    	country == "DK" || 
            		    	country == "EE" ||
            		    	country == "FI" || 
            	  	        country == "DE" || 
            	  	        country == "GR" || 
                		country == "HU" ||
             		   	country == "IS" ||
             		   	country == "IE" || 
            		    	country == "IT" || 
            		    	country == "LV" ||
            		    	country == "LI" || 
            		    	country == "LT" || 
            		    	country == "LU" || 
            		    	country == "NL" ||
            		    	country == "PL" || 
            		    	country == "PT" || 
            		    	country == "SK" || 
            		    	country == "SI" ||
            		    	country == "ZA" || 
            		    	country == "ES" || 
            		    	country == "SE" || 
            		    	country == "CH" ||
            		    	country == "GB" || 
            		    	country == "EU" || 
            		    	country == "UZ")
                		channels = new Array(0, 36, 40, 44, 48); // Region 1
                	
                	else if(country == "AM" ||
            		    	country == "AZ" || 
            		    	country == "HR" ||
            		    	country == "CZ" || 
            		    	country == "EG" || 
            		    	country == "FR" || 
            		    	country == "GE" ||
            		    	country == "MC" ||
            		    	country == "TT" || 
            		    	country == "TN" ||
            		    	country == "TR")
                		channels = new Array(0, 36, 40, 44, 48); // Region 2
                	
                	else if(country == "AR" || country == "TW")
				channels = new Array(0, 52, 56, 60, 64, 149, 153, 157, 161);  // Region 3
                	
                	else if(country == "BZ" ||
                		country == "BO" || 
            		    	country == "BN" ||
            		    	country == "CN" || 
            		    	country == "ID" || 
            		    	country == "IR" || 
            		    	country == "PE" ||
            		    	country == "PH")
                		channels = new Array(0, 149, 153, 157, 161); // Region 4
                	
                	else if(country == "KP" ||
                		country == "KR" || 
            		    	country == "UY" ||
            		    	country == "VE")
                		channels = new Array(0, 149, 153, 157, 161); // Region 5
                	
									else if(country == "JP")
                		channels = new Array(0, 36, 40, 44, 48); // Region 9

									else
                		channels = new Array(0, 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161); // Region 7
                }		
                else{  // 40 MHz
                	inputCtrl(document.form.wl_nctrlsb, 1);
                	if (country == "AL" || 
                	 country == "DZ" || 
			 country == "AU" || 
			 country == "BH" || 
              	         country == "BY" ||
              	         country == "CA" || 
              	         country == "CL" || 
              	         country == "CO" || 
                	 country == "CR" ||
                	 country == "DO" || 
                	 country == "SV" || 
                	 country == "GT" || 
			 country == "HN" || 
			 country == "HK" || 
              	         country == "IN" ||
              	         country == "IL" || 
              	         country == "JO" || 
              	         country == "KZ" || 
                	 country == "LB" ||
                	 country == "MO" || 
                	 country == "MK" ||                	 
                	 country == "MY" ||
                	 country == "MX" || 
			 country == "NZ" || 
			 country == "NO" || 
              	         country == "OM" ||
              	         country == "PK" || 
              	         country == "PA" || 
              	         country == "PR" || 
                	 country == "QA" ||
                	 country == "RO" || 
                	 country == "RU" || 
                	 country == "SA" || 
			 country == "SG" || 
			 country == "SY" || 
              	         country == "TH" ||
              	         country == "UA" || 
              	         country == "AE" || 
              	         country == "US" || 
              	         country == "Q2" || 
                	 country == "VN" ||
                	 country == "YE" || 
                	 country == "ZW")
                		channels = new Array(0, 36, 40, 44, 48, 149, 153, 157, 161);
                		
                	else if(country == "AT" ||
                		country == "BE" ||
            		    	country == "BR" ||
            		    	country == "BG" ||
            		    	country == "CY" || 
            		    	country == "DK" || 
            		    	country == "EE" ||
            		    	country == "FI" || 
            	  	        country == "DE" || 
            	  	        country == "GR" || 
                		country == "HU" ||
             		   	country == "IS" ||
             		   	country == "IE" || 
            		    	country == "IT" || 
            		    	country == "LV" ||
            		    	country == "LI" || 
            		    	country == "LT" || 
            		    	country == "LU" || 
            		    	country == "NL" ||
            		    	country == "PL" || 
            		    	country == "PT" || 
            		    	country == "SK" || 
            		    	country == "SI" ||
            		    	country == "ZA" || 
            		    	country == "ES" || 
            		    	country == "SE" || 
            		    	country == "CH" ||
            		    	country == "GB" || 
            		    	country == "EU" || 
            		    	country == "UZ")
                		channels = new Array(0, 36, 40, 44, 48);
                	
                	else if(country == "AM" ||
            		    	country == "AZ" || 
            		    	country == "HR" ||
            		    	country == "CZ" || 
            		    	country == "EG" || 
            		    	country == "FR" || 
            		    	country == "GE" ||
            		    	country == "MC" ||
            		    	country == "TT" || 
            		    	country == "TN" ||
            		    	country == "TR")
                		channels = new Array(0, 36, 40, 44, 48);
                	
                	else if(country == "AR" || country == "TW")
				channels = new Array(0, 52, 56, 60, 64, 149, 153, 157, 161);  // Region 3
                	
                	else if(country == "BZ" ||
                		country == "BO" || 
            		    	country == "BN" ||
            		    	country == "CN" || 
            		    	country == "ID" || 
            		    	country == "IR" || 
            		    	country == "PE" ||
            		    	country == "PH")
                		channels = new Array(0, 149, 153, 157, 161);
                	
                	else if(country == "KP" ||
                		country == "KR" || 
            		    	country == "UY" ||
            		    	country == "VE")
                		channels = new Array(0, 149, 153, 157, 161);
                	
									else if(country == "JP")
                		channels = new Array(0, 36, 40, 44, 48);

									else
                		channels = new Array(0, 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161);
                }                
                
    }	//end Without wireless channel 5g hook
    
        var ch_v = new Array();
        for(var i=0; i<channels.length; i++){
        	ch_v[i] = channels[i];
        }
        if(ch_v[0] == "0")
        	channels[0] = "<% tcWebApi_Get("String_Entry", "Auto", "s") %>";
        add_options_x2(document.form.wl_channel, channels, ch_v, orig);
				var x = document.form.wl_nctrlsb;
				//x.remove(x.selectedIndex);
				free_options(x);
				add_option(x, "<% tcWebApi_Get("String_Entry", "Auto", "s") %>", "lower");
				x.selectedIndex = 0;
}

function insertExtChannelOption_2g(){
	var wmode = document.form.wl_nmode_x.value;
	var CurrentCh = document.form.wl_channel.value;
	if(CurrentCh == "") CurrentCh = '<% tcWebApi_get("WLan_Common","Channel","s"); %>';	
	
	//setting wl channel
	free_options(document.form.wl_channel);
	var country = '<% tcWebApi_get("WLan_Common","wl0_CountryCode","s"); %>';
	if(country == "DB") {
		channels = new Array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14); // Region 5
	}
	else if (country == "CA" ||
		country == "CR" ||
		country == "DO" ||
		country == "GT" ||
		country == "MX" ||
		country == "NO" ||
		country == "PA" ||
		country == "PR" ||
		country == "TW" ||
		country == "US" ||
		country == "UZ") {
		channels = new Array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11); // Region 0
	}
	else {
		channels = new Array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13); // Region 1
	}
	
	var ch_v = new Array();
	for(var i = 0; i < channels.length; i++) {
		ch_v[i] = channels[i];
	}
	if(ch_v[0] == "0") {
		channels[0] = "<% tcWebApi_Get("String_Entry", "Auto", "s") %>";
	}
	add_options_x2(document.form.wl_channel, channels, ch_v, CurrentCh);
			
	//setting wl_nctrlsb
	var option_length = document.form.wl_channel.options.length;
	if ((wmode == "9"||wmode == "8"||wmode == "6"||wmode == "11") && document.form.wl_bw.value != "0"){
		inputCtrl(document.form.wl_nctrlsb, 1);
		var x = document.form.wl_nctrlsb;
		var length = document.form.wl_nctrlsb.options.length;
		if (length > 1){
			x.selectedIndex = 1;
			x.remove(x.selectedIndex);
		}
		if ((CurrentCh >=1) && (CurrentCh <= 4)){
			x.options[0].text = "Upper";
			x.options[0].value = "1";
		}
		else if ((CurrentCh >= 5) && (CurrentCh <= 7)){
			x.options[0].text = "Lower";
			x.options[0].value = "0";
			add_a_option(document.form.wl_nctrlsb, "1", "Upper");
			if (document.form.wl_nctrlsb_old.value == "1"){
				document.form.wl_nctrlsb.options.selectedIndex=1;
			}
		}
		else if ((CurrentCh >= 8) && (CurrentCh <= 9)){
			x.options[0].text = "Upper";
			x.options[0].value = "1";
			if (option_length >=14){
				add_a_option(document.form.wl_nctrlsb, "0", "Lower");
				if (document.form.wl_nctrlsb_old.value == "0"){
					document.form.wl_nctrlsb.options.selectedIndex=1;
				}
			}
		}
		else if (CurrentCh == 10){
			x.options[0].text = "Lower";
			x.options[0].value = "0";
			if (option_length > 14){
				add_a_option(document.form.wl_nctrlsb, "1", "Upper");
				if (document.form.wl_nctrlsb_old.value == "1"){
					document.form.wl_nctrlsb.options.selectedIndex=1;
				}
			}
		}
		else if (CurrentCh >= 11){
			x.options[0].text = "Lower";
			x.options[0].value = "0";
		}
		else{
			x.options[0].text = "<% tcWebApi_Get("String_Entry", "Auto", "s") %>";
			x.options[0].value = "1";
		}
	}
	else
		inputCtrl(document.form.wl_nctrlsb, 0);
}

function change_channel() {
	var band = document.form.wl_unit.value;

	if(band == 0){
		if(document.form.wl_channel.value  == '0' && document.getElementById("wl_channel").length > 12) {	//with channel 12, 13
			document.getElementById('acs_ch13_checkbox').style.display = "";
			document.form.acs_ch13.disabled = false;					
		}
		else{
			document.getElementById('acs_ch13_checkbox').style.display = "none";
			document.form.acs_ch13.disabled = true;							
		}
	}
	
	insertExtChannelOption();
}

function wl_auth_mode_change(isload, edit_unit){
	var mode = document.form.wl_auth_mode_x.value;
	var i, cur, algos;
	inputCtrl(document.form.wl_wep_x, 1);
	/* enable/disable crypto algorithm */
	if(mode == "wpa" || mode == "wpa2" || mode == "wpawpa2" || mode == "psk" || mode == "psk2" || mode == "pskpsk2")
	inputCtrl(document.form.wl_crypto, 1);
	else
	inputCtrl(document.form.wl_crypto, 0);
	/* enable/disable psk passphrase */
	if(mode == "psk" || mode == "psk2" || mode == "pskpsk2")
	inputCtrl(document.form.wl_wpa_psk, 1);
	else
	inputCtrl(document.form.wl_wpa_psk, 0);
	/* update wl_crypto */
	if(mode == "psk" || mode == "psk2" || mode == "pskpsk2"){
		/* Save current crypto algorithm */
		for(var i = 0; i < document.form.wl_crypto.length; i++){
		if(document.form.wl_crypto[i].selected){
		cur = document.form.wl_crypto[i].value;
		break;
		}
		}
		
		opts = document.form.wl_auth_mode_x.options;
		if(opts[opts.selectedIndex].text == "WPA-Personal")
		algos = new Array("TKIP");
		else if(opts[opts.selectedIndex].text == "WPA2-Personal")
		algos = new Array("AES");
		else
		algos = new Array("AES", "TKIP+AES");
		
		/* Reconstruct algorithm array from new crypto algorithms */
		document.form.wl_crypto.length = algos.length;
		for(i=0; i<algos.length; i++){
			document.form.wl_crypto[i] = new Option(algos[i], algos[i]);
			document.form.wl_crypto[i].value = algos[i];
			if(algos[i] == cur)
				document.form.wl_crypto[i].selected = true;
		}
	}
	else if(mode == "wpa" || mode == "wpawpa2"){
		for(var i = 0; i < document.form.wl_crypto.length; i++){
			if(document.form.wl_crypto[i].selected){
				cur = document.form.wl_crypto[i].value;
				break;
			}
		}
		opts = document.form.wl_auth_mode_x.options;
		if(opts[opts.selectedIndex].text == "WPA-Enterprise")
			algos = new Array("TKIP");
		else
			algos = new Array("AES", "TKIP+AES");
		document.form.wl_crypto.length = algos.length;
		for(i=0; i<algos.length; i++){
			document.form.wl_crypto[i] = new Option(algos[i], algos[i]);
			document.form.wl_crypto[i].value = algos[i];
			if(algos[i] == cur)
				document.form.wl_crypto[i].selected = true;
		}
	}
	else if(mode == "wpa2"){
		for(var i = 0; i < document.form.wl_crypto.length; i++){
			if(document.form.wl_crypto[i].selected){
				cur = document.form.wl_crypto[i].value;
				break;
			}
		}
		algos = new Array("AES");
		document.form.wl_crypto.length = algos.length;
		for(i=0; i<algos.length; i++){
			document.form.wl_crypto[i] = new Option(algos[i], algos[i].toLowerCase());
			//document.form.wl_crypto[i].value = algos[i].toLowerCase();
			document.form.wl_crypto[i].value = algos[i];
			if(algos[i].toLowerCase() == cur)
				document.form.wl_crypto[i].selected = true;
		}
	}
	change_wep_type(mode, isload, edit_unit);
	/* Save current network key index */
	cur = "1";
	for(var i = 0; i < document.form.wl_key.length; i++){
		if(document.form.wl_key[i].selected){
			cur = document.form.wl_key[i].value;
			break;
		}
	}
	/* Define new network key indices */
	if(mode == "wpa" || mode == "wpa2" || mode == "wpawpa2" || mode == "psk" || mode == "psk2" || mode == "pskpsk2" || mode == "radius")
		algos = new Array("1", "2", "3", "4");
	else{
		algos = new Array("1", "2", "3", "4");
	}
	/* Reconstruct network key indices array from new network key indices */
	free_options(document.form.wl_key);
	document.form.wl_key.length = algos.length;
	for(i=0; i<algos.length; i++){
		document.form.wl_key[i] = new Option(algos[i], algos[i]);
		document.form.wl_key[i].value = algos[i];
		if(algos[i] == cur)
			document.form.wl_key[i].selected = true;
	}
	
	wl_wep_change(edit_unit);
}

function showhide(element, sh)
{
var status;
if (sh == 1){
status = "";
}
else{
status = "none"
}
if(document.getElementById){
document.getElementById(element).style.display = status;
}
else if (document.all){
document.all[element].style.display = status;
}
else if (document.layers){
document.layers[element].display = status;
}
}

var pageChanged = 0;
var pageChangedCount = 0;

/*Viz 2011.03 for Input value check, start*/
function valid_IP_form(obj, flag){
	if(obj.value == ""){
		return true;
	}
	else if(flag==0){	//without netMask
		if(!validate_ipaddr_final(obj, obj.name)){
			obj.focus();
			obj.select();		
			return false;	
		}else
				return true;
	}else if(flag==1){	//with netMask and generate netmask
		var strIP = obj.value;
		var re = new RegExp("^([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})$", "gi");			
			
		if(!validate_ipaddr_final(obj, obj.name)){
			obj.focus();
			obj.select();		
			return false;	
		}			
			
		if(obj.name=="sr_ipaddr_x_0" && re.test(strIP)){
				if((RegExp.$1 > 0) && (RegExp.$1 < 127)) document.form.sr_netmask_x_0.value = "255.0.0.0";
				else if ((RegExp.$1 > 127) && (RegExp.$1 < 192)) document.form.sr_netmask_x_0.value = "255.255.0.0";
				else if ((RegExp.$1 > 191) && (RegExp.$1 < 224)) document.form.sr_netmask_x_0.value = "255.255.255.0";
				else document.form.sr_netmask_x_0.value = "0.0.0.0";												
		}
		else if(obj.name=="wan_ipaddr_x" && re.test(strIP)){
				if((RegExp.$1 > 0) && (RegExp.$1 < 127)) document.form.wan_netmask_x.value = "255.0.0.0";
				else if ((RegExp.$1 > 127) && (RegExp.$1 < 192)) document.form.wan_netmask_x.value = "255.255.0.0";
				else if ((RegExp.$1 > 191) && (RegExp.$1 < 224)) document.form.wan_netmask_x.value = "255.255.255.0";
				else document.form.wan_netmask_x.value = "0.0.0.0";												
		}
		else if(obj.name=="lan_ipaddr" && re.test(strIP)){
				if((RegExp.$1 > 0) && (RegExp.$1 < 127)) document.form.lan_netmask.value = "255.0.0.0";
				else if ((RegExp.$1 > 127) && (RegExp.$1 < 192)) document.form.lan_netmask.value = "255.255.0.0";
				else if ((RegExp.$1 > 191) && (RegExp.$1 < 224)) document.form.lan_netmask.value = "255.255.255.0";
				else document.form.lan_netmask.value = "0.0.0.0";				
		}
			
		return true;
	}else if(flag==2){ 	//flag =2, ip plus netmask
				
		if(obj.value.search("/") == -1){		// only IP
			if(!validate_ipaddr_final(obj, obj.name)){
				obj.focus();
				obj.select();		
				return false;	
			}
			else
				return true;
		}
		else{															// IP plus netmask
			if(obj.value.split("/").length > 2){
				alert(obj.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
				obj.value = "";
				obj.focus();
				obj.select();
				return false;
			}
			else{
				if(obj.value.split("/")[1] == "" || obj.value.split("/")[1] == 0 || obj.value.split("/")[1] > 30){
					alert(obj.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
					obj.value = "";
					obj.focus();
					obj.select();
					return false;								
				}
				else{
					var IP_tmp = obj.value;
					obj.value = obj.value.split("/")[0];
					if(!validate_ipaddr_final(obj, obj.name)){
						obj.focus();
						obj.select();		
						return false;	
					}
					else{
						obj.value = IP_tmp;
						return true;
					}		
				}
			}
		}		// IP plus netmask
	}	//flag =2, ip plus netmask
	else
		return false;
}


function valid_IP_subnet(obj){
	var ipPattern1 = new RegExp("(^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.(\\*)$)", "gi");
	var ipPattern2 = new RegExp("(^([0-9]{1,3})\\.([0-9]{1,3})\\.(\\*)\\.(\\*)$)", "gi");
	var ipPattern3 = new RegExp("(^([0-9]{1,3})\\.(\\*)\\.(\\*)\\.(\\*)$)", "gi");
	var ipPattern4 = new RegExp("(^(\\*)\\.(\\*)\\.(\\*)\\.(\\*)$)", "gi");
	var parts = obj.value.split(".");
	if(!ipPattern1.test(obj.value) && !ipPattern2.test(obj.value) && !ipPattern3.test(obj.value) && !ipPattern4.test(obj.value)){
			alert(obj.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
    	obj.focus();
    	obj.select();
    	return false;
	
	}else if(parts[0] == 0 || parts[0] > 255 || parts[1] > 255 || parts[2] > 255){			
			alert(obj.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
			obj.focus();
    	obj.select();
    	return false;
    	
	}else
			return true;
}

function check_hwaddr_temp(obj){ //remove alert()
if(obj.value == ""){
return true;
}else{
var hwaddr = new RegExp("(([a-fA-F0-9]{2}(\:|$)){6})", "gi");
if(!hwaddr.test(obj.value)){
obj.focus();
obj.select();
return false;
}
else
return true;
}
}
function check_hwaddr(obj){
if(obj.value == ""){
return true;
}else{
var hwaddr = new RegExp("(([a-fA-F0-9]{2}(\:|$)){6})", "gi"); // ,"g" whole erea match & "i" Ignore Letter
if(!hwaddr.test(obj.value)){
alert("<%tcWebApi_get("String_Entry","LHC_MnlDHCPMacaddr_id","s")%>");
obj.focus();
obj.select();
return false;
}
else
return true;
}
}
function valid_IPorMAC(obj){
var hwaddr = new RegExp("(([a-fA-F0-9]{2}(\:|$)){6})", "gi"); // ,"g" whole erea match & "i" Ignore Letter
if(obj.value == ""){
return true;
}else if(hwaddr.test(obj.value)){
return true;
}else if(validate_ipaddr_final(obj, obj.name)){
return true;
}else{
obj.focus();
obj.select();
return false;
}
}

function check_hwaddr_flag(obj){  //check_hwaddr() remove alert()
	if(obj.value == ""){
		return 0;
	}else{
		var hwaddr = new RegExp("(([a-fA-F0-9]{2}(\:|$)){6})", "gi");
		var legal_hwaddr = new RegExp("(^([a-fA-F0-9][aAcCeE02468])(\:))", "gi"); // for legal MAC, unicast & globally unique (OUI enforced)

		if(!hwaddr.test(obj.value))
			return 1;
		else if(!legal_hwaddr.test(obj.value))
			return 2;
		else
			return 0;
	}
}

function validate_number_range(obj, mini, maxi){
var PortRange = obj.value;
var rangere=new RegExp("^([0-9]{1,5})\:([0-9]{1,5})$", "gi");
if(rangere.test(PortRange)){
if(parseInt(RegExp.$1) >= parseInt(RegExp.$2)){
alert(obj.value + " <%tcWebApi_get("String_Entry","JS_validport","s")%>");
obj.focus();
obj.select();
return false;
}
else{
if(!validate_each_port(obj, RegExp.$1, mini, maxi) || !validate_each_port(obj, RegExp.$2, mini, maxi)){
obj.focus();
obj.select();
return false;
}
return true;
}
}
else{
if(!validate_range(obj, mini, maxi)){
obj.focus();
obj.select();
return false;
}
return true;
}
}
function validate_each_port(o, num, min, max) {
if(num<min || num>max) {
alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
return false;
}else {
if(o.value=="")
o.value="0";
return true;
}
}


function getURLParameter(param)
{
           param = param.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
           var regexS = "[\\?&]"+param+"=([^&#]*)";
           var regex = new RegExp(regexS);
           var results = regex.exec( window.location.href );
           if(results == null)
                     return "";
           else
                     return results[1];
}
/*Viz 2011.03 for Input value check, end */

/* Handle WEP encryption changed */
function wep_encryption_change(obj){
	change_wlweptype(obj, 0);
	//nmode_limitation();
	automode_hint();
}

/* Handle Authentication Method changed */
function authentication_method_change(obj, edit_unit){	//edit_unit for Guestnetwork	
	wl_auth_mode_change(0, edit_unit);	
		
	//nmode_limitation();
	automode_hint();
}

/* Handle wireless mode changed */
function wireless_mode_change(obj){
	if(obj.value=='9' || obj.value=='0')	//2.4GHZ: auto, legacy
		inputCtrl(document.form.wl_gmode_check, 1);
	else
		inputCtrl(document.form.wl_gmode_check, 0);
		
	/*	
	if(obj.value == "0")	//2.4GHz: legacy
		inputCtrl(document.form.wl_bw, 0);
	else
		inputCtrl(document.form.wl_bw, 1);
	*/
	free_options(document.form.wl_bw);
	var bws = new Array();
	var bwsDesc = new Array();
	var cur = "<%tcWebApi_get("WLan_Common","HT_BW","s")%>";
	if(obj.value == '0' || obj.value == '2') { //legacy: 0 for 2.4GHz, 2 for 5GHz
		bws = [0];
		bwsDesc = ["20 MHz"];
		add_options_x2(document.form.wl_bw, bwsDesc, bws, cur);
	}
	else {
		bws = [1, 0, 2];
		bwsDesc = ["20/40 MHz", "20 MHz", "40 MHz"];
		add_options_x2(document.form.wl_bw, bwsDesc, bws, cur);
		handle_11ac_80MHz();
	}

	insertExtChannelOption();
	//if(obj.value == "3"){document.form.wl_wme.value = "on";}	//without this option

	limit_auth_method();
	//nmode_limitation();
	automode_hint();
	check_NOnly_to_GN();
}

function handle_11ac_80MHz(){
	if(document.form.wl_nmode_x.value == '0' || document.form.wl_nmode_x.value == '2') //legacy: 0 for 2.4GHz, 2 for 5GHz
		document.form.wl_bw[0].text = "20 MHz";
	else if(!wl_info.band5g_support || band5g_11ac_support == -1 || document.form.wl_unit[0].selected == true || document.form.wl_nmode_x.value== '6' || document.form.wl_nmode_x.value== '11') {			
		document.form.wl_bw[0].text = "20/40 MHz";
		document.form.wl_bw.remove(3); //remove 80 Mhz when not when not required required
	} 
	else {
		document.form.wl_bw[0].text = "20/40/80 MHz";
		if(document.form.wl_bw.length == 3)
			document.form.wl_bw[3] = new Option("80 MHz", "3");
	}
}

/* To hide Shared key, WPA-Personal/Enterprise and RADIUS with 802.1X*/
function limit_auth_method(){
	var auth_method_array = document.form.wl_auth_mode_x.value;
	if(document.form.wl_nmode_x.value != "0" && document.form.wl_nmode_x.value != "2" && sw_mode !="2"){		//not legacy
		var auth_array = [["Open System", "open"], ["WPA2-Personal", "psk2"], ["WPA-Auto-Personal", "pskpsk2"], ["WPA2-Enterprise", "wpa2"], ["WPA-Auto-Enterprise", "wpawpa2"]];
	}
	else{
		var auth_array = [["Open System", "open"], ["Shared Key", "shared"], ["WPA-Personal", "psk"], ["WPA2-Personal", "psk2"], ["WPA-Auto-Personal", "pskpsk2"], ["WPA-Enterprise", "wpa"], ["WPA2-Enterprise", "wpa2"], ["WPA-Auto-Enterprise", "wpawpa2"], ["Radius with 802.1x", "radius"]];
	}
	
	free_options(document.form.wl_auth_mode_x);
	for(i = 0; i < auth_array.length; i++){		
		if(auth_method_array  == auth_array[i][1]){
			add_option(document.form.wl_auth_mode_x, auth_array[i][0], auth_array[i][1], 1);
		}else{
			add_option(document.form.wl_auth_mode_x, auth_array[i][0], auth_array[i][1], 0);	
		}	
	}
		
	authentication_method_change(document.form.wl_auth_mode_x);
}

/* To hide Shared key, WPA-Personal/Enterprise and RADIUS with 802.1X for Guest_network.asp*/
function GN_limit_auth_method(){
        var auth_method_array = document.form.wl_auth_mode_x.value;
        if(document.form.wl_nmode_x.value != "0" && document.form.wl_nmode_x.value != "2" && sw_mode !="2"){            //not legacy
                var auth_array = [["Open System", "open"], ["WPA2-Personal", "psk2"], ["WPA-Auto-Personal", "pskpsk2"]];
        }
        else{
                var auth_array = [["Open System", "open"], ["Shared Key", "shared"], ["WPA-Personal", "psk"], ["WPA2-Personal", "psk2"], ["WPA-Auto-Personal", "pskpsk2"]];
        }

        free_options(document.form.wl_auth_mode_x);
        for(i = 0; i < auth_array.length; i++){
                if(auth_method_array  == auth_array[i][1]){
                        add_option(document.form.wl_auth_mode_x, auth_array[i][0], auth_array[i][1], 1);
                }else{
                        add_option(document.form.wl_auth_mode_x, auth_array[i][0], auth_array[i][1], 0);
                }
        }

        authentication_method_change(document.form.wl_auth_mode_x);
}

function getDDNSState(ddns_return_code, ddns_hostname, ddns_old_hostname)
{
	var ddnsStateHint = "";
	if(ddns_return_code == 'register,-1')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2","s")%>";
	else if(ddns_return_code.indexOf('200')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_3","s")%>";
	else if(ddns_return_code.indexOf('203')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_hostname","s")%> '"+ddns_hostname+"' <%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_registered","s")%>";
	else if(ddns_return_code.indexOf('220')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_4","s")%>";
	else if(ddns_return_code == 'register,230')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_5","s")%>";
	else if(ddns_return_code.indexOf('233')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_hostname","s")%> '"+ddns_hostname+"' <%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_registered_2","s")%> '"+ddns_old_name+"'";
	else if(ddns_return_code.indexOf('296')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_6","s")%>";
	else if(ddns_return_code.indexOf('297')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_7","s")%>";
	else if(ddns_return_code.indexOf('298')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_8","s")%>";
	else if(ddns_return_code.indexOf('299')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_9","s")%>";
	else if(ddns_return_code.indexOf('401')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_10","s")%>";
	else if(ddns_return_code.indexOf('407')!=-1)
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_11","s")%>";
	else if(ddns_return_code == 'Time-out')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_1","s")%>";
	else if(ddns_return_code =='unknown_error')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2","s")%>";
	else if(ddns_return_code =='connect_fail')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","qis_fail_desc7","s")%>";
	else if(ddns_return_code =='no_change')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_nochange","s")%>";
	else if(ddns_return_code =='auth_fail')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","qis_fail_desc1","s")%>";
	else if(ddns_return_code =='Updating' || ddns_return_code =='ddns_query')
		ddnsStateHint = "Still query ASUS DDNS registration status. Please wait.";
	else if(ddns_return_code != '')
		ddnsStateHint = "<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_2","s")%>";

	return ddnsStateHint;
}