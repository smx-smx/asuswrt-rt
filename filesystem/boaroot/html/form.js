function checkIP(o,e){
var nextInputBlock = o.nextSibling.nextSibling; //find the next input (sibling include ".")
var nc = window.event ? e.keyCode : e.which;
var s = o.value;
if((nc>=48 && nc<=57) || (nc>=97 && nc<=105) || nc==8 || nc==0)
{
return true;
}
else if(nc==190 || nc==32 || nc==110 || nc==46){
;
}else if((nc==37 || nc==8) && s==""){
;
}
else{
nc=0;
return false;
}
}
var moveLeft_key = 0;
var moveRight_key = 0;
function checkWord(o,e){
var nextInputBlock = o.nextSibling.nextSibling; //find the next input (sibling include ".")
var prevInputBlock = o.previousSibling;
prevInputBlock = (prevInputBlock != null)?prevInputBlock.previousSibling:o;
var sk = window.event ? e.keyCode : e.which;
var s = o.value;
if((sk>=48 && sk<=57) || (sk>=97 && sk<=105) || sk==0) // 0->other key
{
if(s.length == 3){
if(getCaretPos(o) == 3 && nextInputBlock){
if(validateRange(o)){
nextInputBlock.focus();
nextInputBlock.select();
}
}
else{
validateRange(o);
}
}
else if(s.length == getCaretPos(o)){
moveRight_key = 1;
}
moveLeft_key = 0;
}
else if(sk==190 || sk==110 || sk==32){ //190 & 110-> "dot", 32->"space"
if(o.value == '.' || o.value == '..' || o.value == '...'
|| o.value == ' ' || o.value == ' ' || o.value == ' '){
o.value = "";
o.focus();
}
else{
validateIP(o);
if(nextInputBlock){
nextInputBlock.focus();
nextInputBlock.select();
}
}
}
else if(sk==8){ //8->backspace
if(getCaretPos(o) == 0 && moveLeft_key == 0){
moveLeft_key = 1;
}
else if(getCaretPos(o) == 0){
if(prevInputBlock){
prevInputBlock.focus();
prevInputBlock.select();
moveLeft_key = 0;
moveRight_key = 1;
}
}
}
else if(sk==37){ // 37-> 鍵盤向左鍵
if(getCaretPos(o) == 0 && moveLeft_key == 0){
moveLeft_key = 1;
}
else if(prevInputBlock && getCaretPos(o) == 0 && moveLeft_key == 1){
prevInputBlock.focus();
prevInputBlock.select();
moveLeft_key = 0;
}
else{
moveLeft_key = 0;
}
moveRight_key = 0;
}
else if(sk==39){ //39 ->鍵盤向右鍵
if(getCaretPos(o) == 0 && s.length == 0 && nextInputBlock && validateRange(o)){
nextInputBlock.focus();
moveRight_key = 0;
moveLeft_key = 1;
}
else if(getCaretPos(o) == s.length && moveRight_key == 0){
moveRight_key = 1;
moveLeft_key = 0;
}
else if(moveRight_key == 1 && nextInputBlock && validateRange(o)){
nextInputBlock.focus();
moveRight_key = 0;
moveLeft_key = 1;
}
else{
moveLeft_key = 0;
}
}
else{
if(isNaN(s) && s.length >= 1 && sk != 13){
				alert("<%tcWebApi_get("String_Entry","LHC_x_DDNS_alarm_9","s")%>");
o.focus();
o.select();
return false;
}
else{
nc=0;
return false;
}
}
}
function validateIP(o){
var s = o.value;
if(s.indexOf(".") == 0 || s.indexOf(" ") == 0){
o.value = s.substring(1,2);
}
else if(s.indexOf(".") == 1 || s.indexOf(" ") == 1){
o.value = s.substring(0,1);
}
else if(s.indexOf(".") == 2 || s.indexOf(" ") == 2){
o.value = s.substring(0,2);
}
}
function validateRange(v){
if(v.value < 0 || v.value >= 256){
		alert(v.value+" <%tcWebApi_get("String_Entry","BM_alert_IP2","s")%>. \n<%tcWebApi_get("String_Entry","BM_alert_port1","s")%> 0<%tcWebApi_get("String_Entry","BM_alert_to","s")%>255.");
v.focus();
v.select();
return false;
}else{
return true;
}
}
function combineIP(obj){ //combine all IP info before validation and submit
var IP_List = document.getElementById(obj+"_div").childNodes;
var IP_str = "";
for(var i=0; i < IP_List.length; i++){
if(IP_List[i].type == "text"){
if(IP_List[i].value != ""){
IP_List[i].value = parseInt(IP_List[i].value,10);
}
IP_str += IP_List[i].value;
}
else if(IP_List[i].nodeValue.indexOf(".") != -1){
IP_str += ".";
}
}
if(IP_str != "..."){
document.getElementById(obj).value = IP_str;
}else{
document.getElementById(obj).value = "";
}
}
function fulfillIP(obj){
var src_ip = obj.value.split(".");
var IP_List = document.getElementById(obj.name+"_div").childNodes;
for(var i=0,j=0; i < IP_List.length, j<src_ip.length; i++){
if(IP_List[i].type == "text"){
IP_List[i].value = src_ip[j];
j++;
}
}
}
function IPinputCtrl(obj, t){
var IP_List = document.getElementById(obj.name+"_div").childNodes;
document.getElementById(obj.name+"_div").style.background = (t==0)?"#999999":"#FFFFFF";
for(var i=0; i < IP_List.length; i++){
if(IP_List[i].type == "text"){
IP_List[i].disabled = (t==0)?true:false;
IP_List[i].style.color = (t==0)?"#FFFFFF":"#000000";
IP_List[i].style.background = (t==0)?"#999999":"#FFFFFF";
}
}
}
function getCaretPos(obj){
if(obj.selectionStart >= 0){
return obj.selectionStart; //Gecko
}
else{ //For IE
var currentRange=document.selection.createRange();
var workRange=currentRange.duplicate();
obj.select();
var allRange=document.selection.createRange();
var len=0;
while(workRange.compareEndPoints("StartToStart",allRange)>0){
workRange.moveStart("character",-1);
len++;
}
currentRange.select();
return len;
}
}

function IP_Validate(o){
var ip_reg=/^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/;
if(ip_reg.test(o.value)){ //區分是否為IP
return true;
}
else{
return false;
}
}
function wan_netmask_check_NUI(o) {
var wip =intoa(document.form.wan_ipaddr_x.value);
var gw =intoa(document.form.wan_gateway_x.value);
var nm =intoa(document.form.wan_netmask_x.value);
var lip=intoa(document.form.lan_ipaddr.value);
var lnm=intoa(document.form.lan_netmask.value);
var rem=1;
var nm_valid;
var nm_valid_flag=false;
for(nm_valid=0xffffffff; nm_valid!=0; nm_valid-=rem,rem*=2)
{
if (nm==nm_valid)
{
nm_valid_flag=true;
break;
}
}
if (!nm_valid_flag)
{
		alert(o.value + "<%tcWebApi_get("String_Entry","JS_validip","s")%>");
return false;
}
if (document.form.wan_ipaddr_x.value != '0.0.0.0' && (wip&lnm)==(lip&lnm))
{
		alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
return false;
}
if(gw==0 || gw==0xffffffff || (wip&nm)==(gw&nm))
{
return true;
}
else
{
		alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
return false;
}
}
function validate_ipaddr_final(o,v){
if(v == 'wan_ipaddr_x'){
if(o.value.length == 0){ /*Blank.*/
			alert(o.title+"<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
document.form.wan_ipaddr_x1.focus();
return false;
}
else if(o.value.indexOf("0") == 0){ /*首字不能為0*/
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
			alert(document.form.wan_ipaddr_x.value + "<%tcWebApi_get("String_Entry","JS_validip","s")%>");
document.form.wan_ipaddr_x1.focus();
return false;
}
else if(!(IP_Validate(o))){ /*IP格式錯誤*/
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
			alert(document.form.wan_ipaddr_x.value + "<%tcWebApi_get("String_Entry","JS_validip","s")%>");
document.form.wan_ipaddr_x4.focus();
return false;
}
else if(IP_Validate(o)){
if(document.form.wan_ipaddr_x1.value >= 224){
				alert("<%tcWebApi_get("String_Entry","JS_field_wanip_rule2","s")%>");
document.form.wan_ipaddr_x1.focus();
document.form.wan_ipaddr_x1.select();
return false;
}
else if(document.form.wan_ipaddr_x1.value == 127){
				alert(document.form.wan_ipaddr_x1.value + "<%tcWebApi_get("String_Entry","JS_field_wanip_rule1","s")%>");
document.form.wan_ipaddr_x1.focus();
document.form.wan_ipaddr_x1.select();
return false;
}
else{
return true;
}
}
}
else if(v == 'wan_netmask_x'){
var wan_ipaddr_x1 = document.form.wan_ipaddr_x1.value;
if(o.value.length == 0){ /*Blank.*/
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
			
			if(confirm(o.title+"<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>\n<%tcWebApi_get("String_Entry","JS_field_fulfillSubmask","s")%>")){
if((wan_ipaddr_x1 > 0) && (wan_ipaddr_x1 < 127)) o.value = "255.0.0.0";
else if ((wan_ipaddr_x1 > 127) && (wan_ipaddr_x1 < 192)) o.value = "255.255.0.0";
else if ((wan_ipaddr_x1 > 191) && (wan_ipaddr_x1 < 224)) o.value = "255.255.255.0";
else o.value = "0.0.0.0";
fulfillIP(o);
}
document.form.wan_netmask_x1.focus();
return false;
}
else if(!(IP_Validate(o))){ /*IP格式錯誤*/
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
			alert(o.value + "<%tcWebApi_get("String_Entry","JS_validip","s")%>");
return false;
}
else if(IP_Validate(o)){
if(requireWANIP(v) && (
(matchSubnet2(document.form.wan_ipaddr_x.value, o, document.form.lan_ipaddr.value, document.form.lan_netmask))
)){
				alert(o.value + " <%tcWebApi_get("String_Entry","JS_validip","s")%>");
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
return false;
}
else{
return true;
}
}
}
else if(v == 'wan_gateway_x'){
if(o.value.length > 0){
if(!(IP_Validate(o))){ /* IP格式錯誤*/
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
				alert(o.value + "<%tcWebApi_get("String_Entry","JS_validip","s")%>");
return false;
}
else if(o.value == document.form.wan_ipaddr_x.value){
				alert("<%tcWebApi_get("String_Entry","IPC_warning_WANIPEQUALGatewayIP","s")%>");
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
return false;
}
}
return true;
}
else if(v == 'wan_dns1_x' || v == 'wan_dns2_x'){
var split_IP = o.value.split(".");
if(!(IP_Validate(o))){
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
			alert(o.value + "<%tcWebApi_get("String_Entry","JS_validip","s")%>");
return false;
}
return true;
/*
else if(IP_Validate(o)){
if(split_IP[0]==255||split_IP[1]==255||split_IP[2]==255||split_IP[3]==255||split_IP[0]==0||split_IP[3]==0||split_IP[0]==127||split_IP[0]==224){
				alert(o.value +" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
document.getElementById(o.name+"_div").style.border = "2px solid #CE1E1E";
return false;
}
else{
return true;
}
}
*/
}
}

function cal_panel_block(obj, multiple) {
	var isMobile = function() {
		var tmo_support = ('<% nvram_get("rc_support"); %>'.search("tmo") == -1) ? false : true;
		if(!tmo_support)
			return false;

		if(	navigator.userAgent.match(/iPhone/i)	||
			navigator.userAgent.match(/iPod/i)		||
			navigator.userAgent.match(/iPad/i)		||
			(navigator.userAgent.match(/Android/i) && (navigator.userAgent.match(/Mobile/i) || navigator.userAgent.match(/Tablet/i))) ||
			(navigator.userAgent.match(/Opera/i) && (navigator.userAgent.match(/Mobi/i) || navigator.userAgent.match(/Mini/i))) ||	// Opera mobile or Opera Mini
			navigator.userAgent.match(/IEMobile/i)	||	// IE Mobile
			navigator.userAgent.match(/BlackBerry/i)	//BlackBerry
		 ) {
			return true;
		}
		else {
			return false;
		}
	};
	var blockmarginLeft;
	if (window.innerWidth) {
		winWidth = window.innerWidth;
	}
	else if ((document.body) && (document.body.clientWidth)) {
		winWidth = document.body.clientWidth;
	}

	if (document.documentElement  && document.documentElement.clientHeight && document.documentElement.clientWidth) {
		winWidth = document.documentElement.clientWidth;
	}

	if(winWidth > 1050) {
		winPadding = (winWidth - 1050) / 2;
		winWidth = 1105;
		blockmarginLeft = (winWidth * multiple) + winPadding;
	}
	else if(winWidth <= 1050) {
		if(isMobile()) {
			if(document.body.scrollLeft < 50) {
				blockmarginLeft= (winWidth) * multiple + document.body.scrollLeft;
			}
			else if(document.body.scrollLeft >320) {
				blockmarginLeft = 320;
			}
			else {
				blockmarginLeft = document.body.scrollLeft;
			}
		}
		else {
			blockmarginLeft = (winWidth) * multiple + document.body.scrollLeft;
		}
	}

	document.getElementById(obj).style.marginLeft = blockmarginLeft + "px";
}

function adjust_TM_eula_height(_objID) {
    var scrollTop = window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop || 0;
    document.getElementById(_objID).style.top = (scrollTop + 10) + "px";
    var visiable_height = document.documentElement.clientHeight;
    var tm_eula_container_height = parseInt(document.getElementById(_objID).offsetHeight);
    console.log(visiable_height+" , "+tm_eula_container_height);
    var tm_eula_visiable_height = visiable_height - tm_eula_container_height;
    if(tm_eula_visiable_height < 0) {
        var tm_eula_content_height = parseInt(document.getElementById("tm_eula_content").style.height);
        document.getElementById("tm_eula_content").style.height = (tm_eula_content_height - Math.abs(tm_eula_visiable_height) - 20) + "px"; //content height - overflow height - margin top and margin bottom
    }
}
function adjust_panel_block_top(_objID, _offsetHeight) {
    var scrollTop = window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop || 0;
    document.getElementById(_objID).style.top = (scrollTop + _offsetHeight) + "px";
}
