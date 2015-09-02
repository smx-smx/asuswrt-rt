<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--mediaserver.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png"><title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu2","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/usp_style.css">
<link rel="stylesheet" type="text/css" href="/ext/css/ext-all.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script type="text/javascript" src="/ext/ext-base.js"></script>
<script type="text/javascript" src="/ext/ext-all.js"></script>
<script>
var $j = jQuery.noConflict();
</script>
<style type="text/css">
.upnp_table{
width:750px;
padding:5px;
padding-top:20px;
margin-top:-17px;
position:relative;
background-color:#4d595d;
align:left;
-webkit-border-top-right-radius: 05px;
-webkit-border-bottom-right-radius: 5px;
-webkit-border-bottom-left-radius: 5px;
-moz-border-radius-topright: 05px;
-moz-border-radius-bottomright: 5px;
-moz-border-radius-bottomleft: 5px;
border-top-right-radius: 05px;
border-bottom-right-radius: 5px;
border-bottom-left-radius: 5px;
}
.line_export{
height:20px;
width:736px;
}
.upnp_button_table{
width:730px;
background-color:#15191b;
margin-top:15px;
margin-right:5px;
}
.upnp_button_table th{
width:300px;
height:40px;
text-align:left;
background-color:#1f2d35;
font:Arial, Helvetica, sans-serif;
font-size:12px;
padding-left:10px;
color:#FFFFFF;
background:	#2F3A3E;
}
.upnp_button_table td{
width:436px;
height:40px;
background-color:#475a5f;
font:Arial, Helvetica, sans-serif;
font-size:12px;
padding-left:5px;
color:#FFFFFF;
}
.upnp_icon{
width:736px;
height:500px;
margin-top:15px;
margin-right:5px;
}
/* folder tree */
.mask_bg{
position:absolute;
margin:auto;
top:0;
left:0;
width:100%;
height:100%;
z-index:100;
/*background-color: #FFF;*/
background:url(/images/popup_bg2.gif);
background-repeat: repeat;
filter:progid:DXImageTransform.Microsoft.Alpha(opacity=60);
-moz-opacity: 0.6;
display:none;
/*visibility:hidden;*/
overflow:hidden;
}
.mask_floder_bg{
position:absolute;
margin:auto;
top:0;
left:0;
width:100%;
height:100%;
z-index:300;
/*background-color: #FFF;*/
background:url(/images/popup_bg2.gif);
background-repeat: repeat;
filter:progid:DXImageTransform.Microsoft.Alpha(opacity=60);
-moz-opacity: 0.6;
display:none;
/*visibility:hidden;*/
overflow:hidden;
}
.panel{
width:450px;
position:absolute;
margin-top:-8%;
margin-left:35%;
z-index:200;
display:none;
}
.floder_panel{
background-color:#999;
border:2px outset #CCC;
font-size:15px;
font-family:Verdana, Geneva, sans-serif;
color:#333333;
width:450px;
position:absolute;
margin-top:-8%;
margin-left:35%;
z-index:400;
display:none;
}
</style>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var dms_status = <% dms_info() %>;
var _dms_dir = '<%nvram_get("dms_dir")%>';
var _layer_order = "";
function initial(){
show_menu();
	$("option5").innerHTML = '<table><tbody><tr><td><div id="index_img5"></div></td><td><div style="width:120px;"><%tcWebApi_get("String_Entry","Menu_usb_application","s")%></div></td></tr></tbody></table>';
$("option5").className = "m5_r";
if(dms_status[0] != "")
$("dmsStatus").innerHTML = "Scanning.."
if((calculate_height-3)*52 + 20 > 535)
$("upnp_icon").style.height = (calculate_height-3)*52 + 20 + "px";
else
$("upnp_icon").style.height = "500px";
}
function submit_mediaserver(server, x){
var server_type = eval('document.mediaserverForm.'+server);
showLoading();
if(x == 1)
server_type.value = 0;
else
server_type.value = 1;
document.mediaserverForm.flag.value = "nodetect";
document.mediaserverForm.submit();
}
var dm_dir = new Array();
var Download_path = 'Download2';
var WH_INT=0,Floder_WH_INT=0,General_WH_INT=0;
var BASE_PATH;
var folderlist = new Array();
function showPanel(){
WH_INT = setInterval("getWH();",1000);
$j("#DM_mask").fadeIn(1000);
$j("#panel_add").show(1000);
create_tree();
}
function getWH(){
var winWidth;
var winHeight;
winWidth = document.documentElement.scrollWidth;
if(document.documentElement.clientHeight > document.documentElement.scrollHeight)
winHeight = document.documentElement.clientHeight;
else
winHeight = document.documentElement.scrollHeight;
$("DM_mask").style.width = winWidth+"px";
$("DM_mask").style.height = winHeight+"px";
}
function getFloderWH(){
var winWidth;
var winHeight;
winWidth = document.documentElement.scrollWidth;
if(document.documentElement.clientHeight > document.documentElement.scrollHeight)
winHeight = document.documentElement.clientHeight;
else
winHeight = document.documentElement.scrollHeight;
$("DM_mask_floder").style.width = winWidth+"px";
$("DM_mask_floder").style.height = winHeight+"px";
}
function show_AddFloder(){
Floder_WH_INT = setInterval("getFloderWH();",1000);
$j("#DM_mask_floder").fadeIn(1000);
$j("#panel_addFloder").show(1000);
}
function hidePanel(){
($j("#tree").children()).remove();
clearInterval(WH_INT);
$j("#DM_mask").fadeOut('fast');
$j("#panel_add").hide('fast');
}
function create_tree(){
var rootNode = new Ext.tree.TreeNode({ text:'/mnt', id:'0'});
var rootNodechild = new Ext.tree.TreeNode({ text:'', id:'0t'});
rootNode.appendChild(rootNodechild);
var tree = new Ext.tree.TreePanel({
tbar:[{text:"Ok",handler:function(){$j("#PATH").attr("value",Download_path);hidePanel();}},
'->',{text:'X',handler:function(){hidePanel();}}
],
title:"Please select the desire folder",
applyTo:'tree',
root:rootNode,
height:400,
autoScroll:true
});
tree.on('expandnode',function(node){
var allParentNodes = getAllParentNodes(node);
var path='';
for(var j=0; j<allParentNodes.length; j++){
path = allParentNodes[j].text + '/' +path;
}
initial_dir(path,node);
});
tree.on('collapsenode',function(node){
while(node.firstChild){
node.removeChild(node.firstChild);
}
var childNode = new Ext.tree.TreeNode({ text:'', id:'0t'});
node.appendChild(childNode);
});
tree.on('click',function(node){
var allParentNodes = getAllParentNodes(node);
var path='';
for(var j=0; j<allParentNodes.length; j++){
if(j == allParentNodes.length-2)
continue;
else
path = allParentNodes[j].text + '/' +path;
}
Download_path = path;
path = BASE_PATH + '/' + path;
var url = "getfoldertree.asp";
var type = "General";
url += "?motion=gettree&layer_order="+ _layer_order +"&t=" +Math.random();
$j.get(url,function(data){initial_folderlist(data);});
});
}
function initial_folderlist(data){
eval("folderlist=["+data+"]");
}
function getAllParentNodes(node) {
var parentNodes = [];
var _nodeID = node.id;
_layer_order = "0"
for(i=1; i<_nodeID.length; i++)
_layer_order += "_" + node.id[i]; // generating _layer_order for initial_dir()
parentNodes.push(node);
while (node.parentNode) {
parentNodes = parentNodes.concat(node.parentNode);
node = node.parentNode;
}
return parentNodes;
};
function initial_dir(path,node){
var url = "getfoldertree.asp";
var type = "General";
url += "?motion=gettree&layer_order="+ _layer_order +"&t=" +Math.random();
$j.get(url,function(data){initial_dir_status(data,node);});
}
function initial_dir_status(data,node){
dm_dir.length = 0;
if(data == "/" || (data != null && data != "")){
eval("dm_dir=[" + data +"]");
while(node.lastChild &&(node.lastChild !=node.firstChild)) {
node.removeChild(node.lastChild);
}
for(var i=0; i<dm_dir.length; i++){
var childNodeId = node.id +i;
var childnode = new Ext.tree.TreeNode({id:childNodeId,text:dm_dir[i].split("#")[0]});
node.appendChild(childnode);
var childnodeT = new Ext.tree.TreeNode({id:childNodeId+'t',text:''});
childnode.appendChild(childnodeT);
}
node.removeChild(node.firstChild);
}
else{
while(node.firstChild){
node.removeChild(node.firstChild);
}
}
}
function applyRule(){
document.form.dms_dir.value = $("PATH").value;
showLoading();
FormActions("start_apply.asp", "apply", "restart_media", "3");
document.form.submit();
}
</script>
</head>
<body onload="initial();" onunload="unload_body();">
<div id="TopBanner"></div>
<div id="DM_mask" class="mask_bg"></div>
<div id="panel_add" class="panel">
<div id="tree"></div>
</div>
<div id="DM_mask_floder" class="mask_floder_bg"></div>
<div id="panel_addFloder" class="floder_panel">
<span style="margin-left:95px;"><b id="multiSetting_0"></b></span><br /><br />
<span style="margin-left:8px;margin-right:8px;"><b id="multiSetting_1"></b></span>
<input type="text" id="newFloder" class="input_15_table" value="" /><br /><br />
<input type="button" name="AddFloder" id="multiSetting_2" value="" style="margin-left:100px;" onclick="AddFloderName();">
&nbsp;&nbsp;
<input type="button" name="Cancel_Floder_add" id="multiSetting_3" value="" onClick="hide_AddFloder();">
</div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0" scrolling="no"></iframe>
<form method="post" name="mediaserverForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_media">
<input type="hidden" name="action_wait" value="2">
<input type="hidden" name="current_page" value="mediaserver.asp">
<input type="hidden" name="flag" value="">
<input type="hidden" name="dms_enable" value="">
<input type="hidden" name="daapd_enable" value="">
</form>
<form method="post" name="form" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="current_page" value="mediaserver.asp">
<input type="hidden" name="next_page" value="mediaserver.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="dms_dir" value="">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="">
</form>
<form method="post" name="aidiskForm" action="#" target="hidden_frame">
<input type="hidden" name="motion" id="motion" value="">
<input type="hidden" name="layer_order" id="layer_order" value="">
</form>
<table class="content" align="center" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<br>
<div class="upnp_table">
<table>
<tr>
<td>
<div style="width:730px">
<table width="730px">
<tr>
<td align="left">
								<span class="formfonttitle"><%tcWebApi_get("String_Entry","UPnPMediaServer","s")%></span>
</td>
<td align="right">
<img onclick="go_setting('APP_Installation.asp')" align="right" style="cursor:pointer;position:absolute;margin-left:-20px;margin-top:-30px;" title="Back to USB Extension" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'">
</td>
</tr>
</table>
</div>
<div style="margin:5px;"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","upnp_Desc","s")%></div> </td> </tr>
<tr>
<td>
<div class="upnp_button_table">
<table cellspacing="1">
<tr>
<th><%tcWebApi_get("String_Entry","DLNA_enable","s")%></th>
<td>
<div class="left" style="width:94px; position:relative; left:3%;" id="radio_dms_enable"></div>
<div class="clear"></div>
<script type="text/javascript">
$j('#radio_dms_enable').iphoneSwitch('',
function() {
submit_mediaserver("dms_enable", 0);
},
function() {
submit_mediaserver("dms_enable", 1);
},
{
switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
}
);
</script>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","BC_EnableiTunesServer_in","s")%></th>
<td>
<div class="left" style="width:94px; position:relative; left:3%;" id="radio_daapd_enable"></div>
<div class="clear"></div>
<script type="text/javascript">
$j('#radio_daapd_enable').iphoneSwitch('',
function() {
submit_mediaserver("daapd_enable", 0);
},
function() {
submit_mediaserver("daapd_enable", 1);
},
{
switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
}
);
</script>
</td>
</tr>
<tr>
<th class="hintstyle_download" id="multiSetting_5">Media server directory</th>
<td>
<input type="text" id="PATH" class="input_25_table" style="margin-left:15px;height:25px;" value="" onclick="showPanel();" readonly="readonly"/">
<input type="button" class="button_gen" onclick="applyRule()" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</td>
</tr>
<tr>
<th>Media Server Status</th>
<td><span id="dmsStatus" style="margin-left:15px"><%tcWebApi_get("String_Entry","WlanWPSStatus0Text","s")%></span>
</td>
</tr>
</table>
</div>
</td>
</tr>
<tr>
<td>
<div id="upnp_icon" class="upnp_icon"></div>
</td>
</tr>
</table>
</td>
<td width="20" align="center" valign="top"></td>
</tr>
</table>
</div>
<div id="footer"></div>
<br>
</body>

<!--mediaserver.asp-->
</html>

