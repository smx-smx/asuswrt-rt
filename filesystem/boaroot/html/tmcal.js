/*
Tomato GUI
Copyright (C) 2006-2009 Jonathan Zarate
http://www.polarcloud.com/tomato/
For use with Tomato Firmware only.
No part of this file may be used without permission.
*/
var tabs = [];
var rx_max, rx_avg;
var tx_max, tx_avg;
var xx_max = 0;
var ifname;
var htmReady = 0;
var svgReady = 0;
var updating = 0;
var scaleMode = 0;
var scaleLast = -1;
var drawMode = 0;
var drawLast = -1;
var drawColor = 0;
var drawColorRX = 0; //Viz add 2010.09
var drawColorTX = 0; //Viz add 2010.09
var avgMode = 0;
var avgLast = -1;
var colorX = 0;
var colors = [
['Green &amp; Blue', '#118811', '#6495ed'], ['Blue &amp; Orange', '#003EBA', '#FF9000'],
['Blue &amp; Red', '#003EDD', '#CC4040'], ['Blue', '#22f', '#225'], ['Gray', '#000', '#999'],
['Red &amp; Black', '#d00', '#000']];
var colorRX = [ '#FF9000', '#3CF', '#000000', '#dd0000', '#999999', '#118811'];
var colorTX = ['#FF9000', '#3CF', '#000000', '#dd0000', '#999999', '#118811'];
var modelname = "<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>";

function xpsb(byt)
{
/* REMOVE-BEGIN
kbit/s = 1000 bits/s
125 = 1000 / 8
((B * 8) / 1000)
REMOVE-END */
return (byt / 1024).toFixed(2) + ' <small>KB/s</small>';
}
function showCTab()
{
showTab('speed-tab-' + ifname);
}
function showSelectedOption(prefix, prev, now)
{
var e;
elem.removeClass(prefix + prev, 'selected'); // safe if prev doesn't exist
if ((e = E(prefix + now)) != null) {
elem.addClass(e, 'selected');
e.blur();
}
}
function showDraw()
{
if (drawLast == drawMode) return;
showSelectedOption('draw', drawLast, drawMode);
drawLast = drawMode;
}
function switchDraw(n)
{
if ((!svgReady) || (updating)) return;
drawMode = n;
showDraw();
showCTab();
cookie.set(cprefix + 'draw', drawMode);
}

function showColor()
{
}
function switchColorRX(rev)
{
if ((!svgReady) || (updating)) return;
drawColorRX = rev;
showColor();
showCTab();
}
function switchColorTX(rev)
{
if ((!svgReady) || (updating)) return;
drawColorTX = rev;
showColor();
showCTab();
}
function showScale()
{
if (scaleMode == scaleLast) return;
showSelectedOption('scale', scaleLast, scaleMode);
scaleLast = scaleMode;
}
function switchScale(n)
{
scaleMode = n;
showScale();
showTab('speed-tab-' + ifname);
cookie.set(cprefix + 'scale', scaleMode);
}
function showAvg()
{
if (avgMode == avgLast) return;
showSelectedOption('avg', avgLast, avgMode);
avgLast = avgMode;
}
function switchAvg(n)
{
if ((!svgReady) || (updating)) return;
avgMode = n;
showAvg();
showCTab();
cookie.set(cprefix + 'avg', avgMode);
}
function tabSelect(name)
{
if (!updating) showTab(name);
}
function showTab(name)
{
var h;
var max;
var i;
var rx, tx;
var e;
ifname = name.replace('speed-tab-', '');
cookie.set(cprefix + 'tab', ifname, 14);
tabHigh(name);
h = speed_history[ifname];
if (!h) return;
E('rx-current').innerHTML = xpsb(h.rx[h.rx.length - 1] / updateDiv);
E('rx-avg').innerHTML = xpsb(h.rx_avg);
E('rx-max').innerHTML = xpsb(h.rx_max);
E('tx-current').innerHTML = xpsb(h.tx[h.tx.length - 1] / updateDiv);
E('tx-avg').innerHTML = xpsb(h.tx_avg);
E('tx-max').innerHTML = xpsb(h.tx_max);
E('rx-total').innerHTML = scaleSize(h.rx_total);
E('tx-total').innerHTML = scaleSize(h.tx_total);
if (svgReady) {
max = scaleMode ? MAX(h.rx_max, h.tx_max) : xx_max
if (max > 12500) max = Math.round((max + 12499) / 12500) * 12500;
else max += 100;


if(ifname == "WIRED" || ifname == "WIRELESS0" || ifname == "WIRELESS1"){
	updateSVG(h.rx, h.tx, max, drawMode, colorTX[drawColorTX], colorRX[drawColorRX],updateInt, updateMaxL, updateDiv, avgMode, clock);
	document.getElementById("rx-current").className = "blue_line";
	document.getElementById("tx-current").className = "orange_line";
}else{
	updateSVG(h.tx, h.rx, max, drawMode, colorTX[drawColorTX], colorRX[drawColorRX],updateInt, updateMaxL, updateDiv, avgMode, clock);
	document.getElementById("rx-current").className = "orange_line";
	document.getElementById("tx-current").className = "blue_line";
}	
}
}
function loadData()
{
var old;
var t, e;
var name;
var i;
var changed;
xx_max = 0;
old = tabs;
tabs = [];
clock = new Date();
if (!speed_history) {
speed_history = [];
}
else {
for (var i in speed_history) {
var h = speed_history[i];
if ((typeof(h.rx) == 'undefined') || (typeof(h.tx) == 'undefined')) {
delete speed_history[i];
continue;
}
if (updateReTotal) {
h.rx_total = h.rx_max = 0;
h.tx_total = h.tx_max = 0;
for (j = (h.rx.length - updateMaxL); j < h.rx.length; ++j) {
t = h.rx[j];
if (t > h.rx_max) h.rx_max = t;
h.rx_total += t;
t = h.tx[j];
if (t > h.tx_max) h.tx_max = t;
h.tx_total += t;
}
h.rx_avg = h.rx_total / updateMaxL;
h.tx_avg = h.tx_total / updateMaxL;
}
if (updateDiv > 1) {
h.rx_max /= updateDiv;
h.tx_max /= updateDiv;
h.rx_avg /= updateDiv;
h.tx_avg /= updateDiv;
}
if (h.rx_max > xx_max) xx_max = h.rx_max;
if (h.tx_max > xx_max) xx_max = h.tx_max;

if(modelname == "DSL-N66U" || modelname == "DSL-AC56U" || modelname == "DSL-AC52U" || modelname == "DSL-AC51" || modelname == "DSL-AC750" || modelname == "DSL-AC55U"){ //dual band + dual wan + PTM
	if (i == "WIRELESS1")
	t = 'Wireless(5GHz)';
	else if (i == "WIRELESS0")
	t = 'Wireless(2.4GHz)';
	else if (i == "WIRED")
	t = 'Wired';
	else if (i == "BRIDGE")
	t = 'LAN';
	else if (i == "ATM")
	t = 'ADSL WAN(ATM)';
	else if (i == "PTM0")
	t = 'VDSL WAN(PTM)';
	else if (i == "PTM1")
	t = 'NotUsed';		
	else if (i == "ETHERNETWAN")
	t = 'Ethernet WAN';
	else if (i.search("WIRELESS") > -1 && i.search(".") > -1)
	t = 'NotUsed';
	else
	t = i;
}
else if(modelname == "DSL-N17U" || modelname == "DSL-N16" || modelname == "DSL-N16P"){ //single band + dual wan + PTM
        if (i == "WIRELESS0")
        t = 'Wireless';
        else if (i == "WIRED")
        t = 'Wired';
        else if (i == "BRIDGE")
        t = 'LAN';
        else if (i == "ATM")
        t = 'ADSL WAN(ATM)';
        else if (i == "PTM0")
        t = 'VDSL WAN(PTM)';
        else if (i == "PTM1")
        t = 'NotUsed';
        else if (i == "ETHERNETWAN")
        t = 'Ethernet WAN';
        else if (i.search("WIRELESS") > -1 && i.search(".") > -1)
        t = 'NotUsed';
        else
        t = i;
}
else if(modelname == "DSL-N55U-C1" || modelname == "DSL-N55U-D1"){ //dual band + dual wan
	if (i == "WIRELESS1")
	t = 'Wireless(5GHz)';
	else if (i == "WIRELESS0")
	t = 'Wireless(2.4GHz)';
	else if (i == "WIRED")
	t = 'Wired';
	else if (i == "BRIDGE")
	t = 'LAN';
	else if (i == "ATM")
	t = 'ADSL WAN(ATM)';
	else if (i == "PTM1")
	t = 'NotUsed';		
	else if (i == "ETHERNETWAN")
	t = 'Ethernet WAN';
	else if (i.search("WIRELESS") > -1 && i.search(".") > -1)
	t = 'NotUsed';
	else
	t = i;
}
else if(modelname == "DSL-N16U" || modelname == "DSL-N14U" || modelname == "DSL-N14U-B1" || modelname == "DSL-N12U-C1" || modelname == "DSL-N12U-D1" || modelname == "DSL-N12E-C1" || modelname == "DSL-N10-D1" || modelname == "DSL-N10-C1" || modelname == "DSL-N10P-C1") //single band + dual wan
{
	if (i == "WIRELESS0")
	t = 'Wireless';
	else if (i == "WIRED")
	t = 'Wired';
	else if (i == "BRIDGE")
	t = 'LAN';
	else if (i == "ATM")
	t = 'ADSL WAN(ATM)';
	else if (i == "ETHERNETWAN")
	t = 'Ethernet WAN';
	else if (i.search("WIRELESS") > -1 && i.search(".") > -1)
	t = 'NotUsed';
	else
	t = i;
}
else //single band + single wan
{
	if (i == "WIRELESS0")
	t = 'Wireless';
	else if (i == "WIRED")
	t = 'Wired';
	else if (i == "BRIDGE")
	t = 'LAN';
	else if (i == "ATM")
	t = 'ADSL WAN(ATM)';
	else if (i.search("WIRELESS") > -1 && i.search(".") > -1)
	t = 'NotUsed';
	else
	t = i;
}

if(modelname == "DSL-N66U" || modelname == "DSL-AC56U" || modelname == "DSL-AC52U" || modelname == "DSL-AC51" || modelname == "DSL-AC750" || modelname == "DSL-AC55U"){ //dual band + dual wan + PTM
	if(t != "LAN" && t != "NotUsed"){ // <-- hide these 2 Tabs
					if(i == "PTM0")
						tabs[0] = ['speed-tab-' + i, t];
					else if(i == "ATM")
						tabs[1] = ['speed-tab-' + i, t];
					else if(i == "ETHERNETWAN")
						tabs[2] = ['speed-tab-' + i, t];
					else if	(i == "WIRED")
						tabs[3] = ['speed-tab-' + i, t];
					else if	(i == "WIRELESS0")
						tabs[4] = ['speed-tab-' + i, t];
					else if	(i == "WIRELESS1")
						tabs[5] = ['speed-tab-' + i, t];
					else if(i == "PTM1")
						tabs[6] = ['speed-tab-' + i, t];
					else if	(i == "BRIDGE")
						tabs[7] = ['speed-tab-' + i, t];
	//tabs.push(['speed-tab-' + i, t]);
	}
}
else if(modelname == "DSL-N17U" || modelname == "DSL-N16" || modelname == "DSL-N16P"){ //single band + dual wan + PTM
        if(t != "LAN" && t != "NotUsed"){ // <-- hide these 2 Tabs
                                        if(i == "PTM0")
                                                tabs[0] = ['speed-tab-' + i, t];
					else if(i == "ATM")
                                                tabs[1] = ['speed-tab-' + i, t];
                                        else if(i == "ETHERNETWAN")
                                                tabs[2] = ['speed-tab-' + i, t];
                                        else if (i == "WIRED")
                                                tabs[3] = ['speed-tab-' + i, t];
                                        else if (i == "WIRELESS0")
                                                tabs[4] = ['speed-tab-' + i, t];
                                        else if(i == "PTM1")
                                                tabs[5] = ['speed-tab-' + i, t];
                                        else if (i == "BRIDGE")
                                                tabs[6] = ['speed-tab-' + i, t];
        //tabs.push(['speed-tab-' + i, t]);
        }
}
else if(modelname == "DSL-N55U-C1" || modelname == "DSL-N55U-D1"){ //dual band + dual wan
	if(t != "LAN" && t != "NotUsed"){ // <-- hide these 2 Tabs					
					if(i == "ATM")
						tabs[0] = ['speed-tab-' + i, t];
					else if(i == "ETHERNETWAN")
						tabs[1] = ['speed-tab-' + i, t];
					else if	(i == "WIRED")
						tabs[2] = ['speed-tab-' + i, t];
					else if	(i == "WIRELESS0")
						tabs[3] = ['speed-tab-' + i, t];
					else if	(i == "WIRELESS1")
						tabs[4] = ['speed-tab-' + i, t];
					else if(i == "PTM1")
						tabs[5] = ['speed-tab-' + i, t];
					else if	(i == "BRIDGE")
						tabs[6] = ['speed-tab-' + i, t];
	//tabs.push(['speed-tab-' + i, t]);
	}
}
else if(modelname == "DSL-N16U" || modelname == "DSL-N14U" || modelname == "DSL-N14U-B1" || modelname == "DSL-N12U-C1" || modelname == "DSL-N12U-D1" || modelname == "DSL-N12E-C1" || modelname == "DSL-N10-D1" || modelname == "DSL-N10-C1" || modelname == "DSL-N10P-C1") //single band + dual wan
{
	if(t != "LAN" && t != "NotUsed"){ // hide Tabs
					if(i == "ATM")
						tabs[0] = ['speed-tab-' + i, t];
					else if(i == "ETHERNETWAN")
						tabs[1] = ['speed-tab-' + i, t];
					else if	(i == "WIRED")
						tabs[2] = ['speed-tab-' + i, t];
					else if	(i == "WIRELESS0")
						tabs[3] = ['speed-tab-' + i, t];
					else if	(i == "BRIDGE")
						tabs[4] = ['speed-tab-' + i, t];
	//tabs.push(['speed-tab-' + i, t]);
	}
}
else //single band + single wan
{
	if(t != "LAN" && t != "NotUsed"){ // hide Tabs
					if(i == "ATM")
						tabs[0] = ['speed-tab-' + i, t];
					else if	(i == "WIRED")
						tabs[1] = ['speed-tab-' + i, t];
					else if	(i == "WIRELESS0")
						tabs[2] = ['speed-tab-' + i, t];
					else if	(i == "BRIDGE")
						tabs[3] = ['speed-tab-' + i, t];
	//tabs.push(['speed-tab-' + i, t]);
	}
}
}

/*tabs = tabs.sort(
function(a, b) {
if (a[1] < b[1]) return -1;
if (a[1] > b[1]) return 1;
return 0;
});*/
}
if (tabs.length == old.length) {
for (i = tabs.length - 1; i >= 0; --i)
if (tabs[i][0] != old[i][0]) break;
changed = i > 0;
}
else changed = 1;
if (changed) {
E('tab-area').innerHTML = _tabCreate.apply(this, tabs);
}
if (((name = cookie.get(cprefix + 'tab')) != null) && ((speed_history[name] != undefined))) {
showTab('speed-tab-' + name);
return;
}
if (tabs.length) showTab(tabs[0][0]);
}
function initData()
{
if (htmReady) {
loadData();
if (svgReady) {
E('graph').style.visibility = 'visible';
E('bwm-controls').style.visibility = 'visible';
}
}
}
function initCommon(defAvg, defDrawMode, defDrawColorRX, defDrawColorTX) //Viz modify defDrawColor 2010.09
{
drawMode = fixInt(cookie.get(cprefix + 'draw'), 0, 1, defDrawMode);
showDraw();
//var c = nvram.rstats_colors.split(',');
/*
while (c.length >= 3) {
c[0] = escapeHTML(c[0]);
colors.push(c.splice(0, 3));
} */
//c = (cookie.get(cprefix + 'color') || '').split(',');
/* alert(c); //3,0 */
/* //Banned by Viz 2010.09
if (c.length == 2) {
drawColorRX = fixInt(c[0], 0, colorRX.length - 1, defDrawColorRX);
drawColorTX = fixInt(c[0], 0, colorTX.length - 1, defDrawColorTX);
}
else {
drawColorRX = defDrawColorRX;
drawColorTX = defDrawColorTX;
}
*/ // Banned by Viz 2010.09 ^^^^^^^^^^^
drawColorRX = defDrawColorRX;
drawColorTX = defDrawColorTX;
showColor();
scaleMode = fixInt(cookie.get(cprefix + 'scale'), 0, 1, 0); //cprefix = 'bw_r';
showScale();
avgMode = fixInt(cookie.get(cprefix + 'avg'), 1, 10, defAvg);
showAvg();
if ((nvram.wan_proto == 'disabled') || (nvram.wan_proto == 'wet')) {
nvram.wan_ifname = '';
}
htmReady = 1;
initData();
}

