<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--device-map/clients.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>device-map/clients.asp</title>
<style>
p{
	font-weight: bolder;
}
.type0:hover{
	background-image:url('/images/New_ui/networkmap/client.png') !important;
	background-position:52% 70% !important;
}
.type6:hover{
	background-image:url('/images/New_ui/networkmap/client.png') !important;
	background-position:52% 70% !important;
}
.circle {
	position: absolute;
	width: 23px;
	height: 23px;
	border-radius: 50%;
	background: #333;
	margin-top: -77px;
	margin-left: 57px;
}
.circle div{
	height: 23px;
	text-align: center;
	margin-top: 4px;
}
.nav {
	display:none;
    float: left;
    width: 107%;
    margin-bottom: 30px;
    margin-top: -7px;
}
.nav ul{
    margin: 0;
    padding: 0;
    border-top:solid 2px #666;
}
.nav li{
	font-family:Arial;
    position: relative;
    float: left;
    color: #FFF;
    list-style: none;
    background:#4d595d;
    cursor:pointer;
    width: 100%;
}
.nav li:hover{
	background-color:#77A5C6;
}
.nav li a {
    display: block; 
    padding: 6px;      
    color: #FFF;
    border-bottom:solid 1px #666;
    text-decoration: none;
    cursor:pointer;
} 
.ipMethod{
	background-color: #222;
	font-size: 10px;
	font-family: monospace;
	padding: 1px 2px;
	border-radius: 3px;	
}
.imgUserIcon{
	cursor: pointer;
	position: relative; 
	left: 15px; 
	width: 52px;
	height: 52px;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
}
</style>
<link href="/form_style.css" rel="stylesheet" type="text/css" />
<link href="/NM_style.css" rel="stylesheet" type="text/css" />
<link href="/device-map/device-map.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/jquery.xdomainajax.js"></script>
<script type="text/javascript" src="/help.js"></script>

<script>
var wirelessOverFlag = false;
overlib.isOut = true;
var $j = jQuery.noConflict();
var pagesVar = {
	curTab: "online",
	CLIENTSPERPAGE: 7,
	startIndex: 0,
	endIndex: 7, /* refer to startIndex + CLIENTSPERPAGE */
	startArray: [0],

	resetVar: function(){
		pagesVar.CLIENTSPERPAGE = 7;
		pagesVar.startIndex = 0;
		pagesVar.endIndex = pagesVar.startIndex + pagesVar.CLIENTSPERPAGE;
		pagesVar.startArray = [0];

		document.getElementById("select_wlclient_band").style.display = "none";
	}
}

var clientMacUploadIcon = new Array();

var ipState = new Array();
ipState["Static"] =  "<%tcWebApi_get("String_Entry","BOP_ctype_title5","s")%>";
ipState["DHCP"] =  "<%tcWebApi_get("String_Entry","BOP_ctype_title1","s")%>";
ipState["Manual"] =  "Manually Assigned IP";

function generate_wireless_band_list(){	
	if(wl_nband_title.length == 1) return false;

	var code = '<ul>';
	for(var i=0; i<wl_nband_title.length; i++){
		code += '<li><a onclick="switchTab_drawClientList(\'';
		code += i;
		code += '\')">&nbsp;&nbsp;';
		code += wl_nband_title[i];
		code += '&nbsp;&nbsp;(<b style="font-size:11px;" id="liWirelessNum';
		code += i;
		code += '">0</b>)</a></li>';
	}
	code += '</ul>';

	document.getElementById('select_wlclient_band').innerHTML = code;
}

function initial(){	
	//parent.hideEditBlock(); Viz hide temp	
	generate_wireless_band_list();
	updateClientList();	
}

function convRSSI(val){
	if(val == "") return "wired";

	val = parseInt(val);
	if(val >= -50) return 4;
	else if(val >= -80)	return Math.ceil((24 + ((val + 80) * 26)/10)/25);
	else if(val >= -90)	return Math.ceil((((val + 90) * 26)/10)/25);
	else return 1;
}

function drawClientList(tab){		
	var clientHtml = '<table width="100%" cellspacing="0" align="center"><tbody><tr><td>';
	var clientHtmlTd = '';
	var i = pagesVar.startIndex;
	var userImage = "";
	var custom_usericon = '';
	
	//user icon
	var userIconBase64 = "NoIcon";
	
	if(typeof tab == "undefined"){
		tab = pagesVar.curTab;		
	}
	genClientList();
	pagesVar.endIndex = pagesVar.startIndex + pagesVar.CLIENTSPERPAGE;	
	while(i < pagesVar.endIndex){
		var clientObj = clientList[clientList[i]];
		
		// fileter /*
		if(i > clientList.length-1) break;		
		if(tab == 'online' && !clientObj.isOnline){i++; pagesVar.endIndex++; continue;}		
		if((tab == 'wired' && clientObj.isWL != 0) || !clientObj.isOnline){i++; pagesVar.endIndex++; continue;}		
		if((tab == 'wireless' && clientObj.isWL == 0) || !clientObj.isOnline){i++; pagesVar.endIndex++; continue;}
		if((tab == 'wireless0' && (clientObj.isWL == 0 || clientObj.isWL == 2 || clientObj.isWL == 3)) || !clientObj.isOnline){i++; pagesVar.endIndex++; continue;}
		if((tab == 'wireless1' && (clientObj.isWL == 0 || clientObj.isWL == 1 || clientObj.isWL == 3)) || !clientObj.isOnline){i++; pagesVar.endIndex++; continue;}
		if((tab == 'wireless2' && (clientObj.isWL == 0 || clientObj.isWL == 1 || clientObj.isWL == 2)) || !clientObj.isOnline){i++; pagesVar.endIndex++; continue;}
		if(tab == 'custom' && clientObj.from != "customList"){i++; pagesVar.endIndex++; continue;}
		if(clientObj.Name.toString().toLowerCase().indexOf(document.getElementById("searchingBar").value.toLowerCase()) == -1){i++; pagesVar.endIndex++; continue;}
		// filter */ 

		clientHtmlTd += '<div class="clientBg"><table width="100%" height="85px" border="0">';
		//imgUserIcon
		clientHtmlTd += '<tr><td rowspan="3" width="80px">';
		userImage = getUserIcon(clientObj.MacAddr, custom_usericon);		
		if(userImage != "") {
			clientHtmlTd += '<div title="'+ clientObj.dpiType + '"">';
			clientHtmlTd += '<img id="imgUserIcon_'+ i +'" class="imgUserIcon" src="' + userImage + '"';
			clientHtmlTd += 'onclick="popupCustomTable(\'' + clientObj.MacAddr + '\');"';
			clientHtmlTd += '></div>';
		}
		else{
			clientHtmlTd += '<div class="clientIcon type';
			clientHtmlTd += clientObj.type;
			//clientHtmlTd += '" onclick="popupCustomTable(\'';		Viz hide temporary
			//clientHtmlTd += clientObj.MacAddr;
			//clientHtmlTd += '\')" title="';
			clientHtmlTd += '" title="';
			clientHtmlTd += clientObj.dpiType;
			clientHtmlTd += '"></div>';			
		}
		clientHtmlTd += '</td>';
		
		//Name		
		clientHtmlTd += '<td style="height:20px;">';		
		clientHtmlTd += (clientObj.Name.length > 18) ? (clientObj.Name.substr(0,16) + "...") : clientObj.Name;
		clientHtmlTd += '</td></tr>';
		
		//IP, IP_type
		clientHtmlTd += '<tr>';
		clientHtmlTd += '<td style="height:20px;">';
		clientHtmlTd += (clientObj.isWebServer) ? '<a class="link" href="http://' + clientObj.IP + '" target="_blank">' + clientObj.IP + '</a>' : clientObj.IP;

		if(parent.sw_mode == 1){
			clientHtmlTd += ' <span class="ipMethod" onmouseover="return overlib(\''
			clientHtmlTd += ipState[clientObj.ipMethod];
			clientHtmlTd += '\')" onmouseout="nd();">'
			clientHtmlTd += clientObj.ipMethod + '</span>';
		}
		clientHtmlTd += '</td>';
		
		//wired/wireless
		clientHtmlTd += '<td>';
		clientHtmlTd += '<div style="height:30px;margin-left:0px;" title="'; 
		clientHtmlTd += clientObj.Name;
		clientHtmlTd += '" class="radioIcon radio_';		
		clientHtmlTd += convRSSI(clientObj.rssi);		
		clientHtmlTd += '"></div>';
		
		if(clientObj.isWL != 0) {
				//var bandClass = (navigator.userAgent.toUpperCase().match(/CHROME\/([\d.]+)/)) ? "band_chrome" : "band";
				var bandClass = "band_chrome";
				clientHtmlTd += '<div class="' + bandClass + '">' + wl_nband_title[clientObj.isWL-1].replace("Hz", "") + '</div>';
		}
		
		clientHtmlTd += '</td></tr>';
		
		
		clientHtmlTd += '<tr><td colspan="2" style="height:20px;"><div style="margin-top:-15px;" class="link" onclick="oui_query(\'';
		clientHtmlTd += clientObj.MacAddr;
		clientHtmlTd += '\');return overlib(\'';		//Viz patched 2014.12.22
		clientHtmlTd += retOverLibStr(clientObj);
		clientHtmlTd += '\');" onmouseout="nd();">';
		clientHtmlTd += clientObj.MacAddr;
		clientHtmlTd += '</td></tr>';
		
		clientHtmlTd += '</table></div>';

		// display how many clients that hide behind a repeater.
		if(clientObj.macRepeat > 1){
			clientHtmlTd += '<div class="circle"><div>';
			clientHtmlTd += clientObj.macRepeat;
			clientHtmlTd += '</div></div>';
		}

		i++;
	}
		
	if(clientHtmlTd == ''){
		if(networkmap_fullscan == 1)
			clientHtmlTd = '<div style="color:#FC0;height:30px;text-align:center;margin-top:15px"><%tcWebApi_get("String_Entry","Device_Searching","s")%><img src="/images/InternetScan.gif"></div>';
		else
			clientHtmlTd = '<div style="color:#FC0;height:30px;text-align:center;margin-top:15px"><%tcWebApi_get("String_Entry","IPC_VSList_Norule","s")%></div>';
	}

	clientHtml += clientHtmlTd;
	clientHtml += '</td></tr></tbody></table>';	
	document.getElementById("client_list_Block").innerHTML = clientHtml;

	document.getElementById("leftBtn").style.visibility = (pagesVar.startIndex == 0) ? "hidden" : "visible";
	document.getElementById("rightBtn").style.visibility = (pagesVar.endIndex >= clientList.length) ? "hidden" : "visible";

	document.getElementById("tabWired").style.display = (totalClientNum.wired == 0)? "none" : "";	
	document.getElementById("tabWiredNum").innerHTML = 	totalClientNum.wired;

	document.getElementById("tabWireless").style.display = (totalClientNum.wireless == 0)? "none" : "";

	if(smart_connect_support == -1){
		if(tab.indexOf('wireless') == -1){
			document.getElementById("select_wlclient_band").style.display="none";
			display_wlclient_band = '0';
			document.getElementById("searchingBar").placeholder = 'Search';
		}		
		document.getElementById("tabWirelessNum").innerHTML = totalClientNum.wireless;
		if(document.getElementById("liWirelessNum0"))
			document.getElementById("liWirelessNum0").innerHTML = totalClientNum.wireless_0;
		if(document.getElementById("liWirelessNum1"))	
			document.getElementById("liWirelessNum1").innerHTML = totalClientNum.wireless_1;
		if(document.getElementById("liWirelessNum2"))
			document.getElementById("liWirelessNum2").innerHTML = totalClientNum.wireless_2;

		if(tab == 'wireless0')
			document.getElementById("searchingBar").placeholder = '[2.4GHz]('+totalClientNum.wireless_0+')';
		else if(tab =='wireless1')
			document.getElementById("searchingBar").placeholder = '[5GHz-1]('+totalClientNum.wireless_1+')';
		else if(tab =='wireless2')
			document.getElementById("searchingBar").placeholder = '[5GHz-2]('+totalClientNum.wireless_2+')';
		else if(tab =='wireless')
			document.getElementById("searchingBar").placeholder = '[All]('+totalClientNum.wireless+')';				
	}else{
		document.getElementById("tabWirelessNum").innerHTML = totalClientNum.wireless;
	}


	if(pagesVar.curTab != tab){
		document.getElementById("client_list_Block").style.display = 'none';
		$j("#client_list_Block").fadeIn(300);
		pagesVar.curTab = tab;
	}

	
	$j(".circle").mouseover(function(){
		return overlib(this.firstChild.innerHTML + " clients are connecting to <% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %> through this device.");
	});

	$j(".circle").mouseout(function(){
		nd();
	});
}

function updatePagesVar(direction){
	if(typeof direction != "undefined"){
		if(direction == '-'){
			pagesVar.startArray.length--;
			pagesVar.startIndex = pagesVar.startArray.slice(-1)[0];
			pagesVar.endIndex = pagesVar.startIndex + pagesVar.CLIENTSPERPAGE;
		}
		else if(direction == '+'){
			pagesVar.startIndex = pagesVar.endIndex;
			pagesVar.endIndex = pagesVar.startIndex + pagesVar.CLIENTSPERPAGE;
			pagesVar.startArray.push(pagesVar.startIndex);
		}

		pagesVar.startIndex = (pagesVar.startIndex < 0) ? 0 : pagesVar.startIndex;
		pagesVar.endIndex = (pagesVar.endIndex > clientList.length) ? clientList.length : pagesVar.endIndex;
		pagesVar.endIndex = (pagesVar.endIndex < pagesVar.CLIENTSPERPAGE) ? pagesVar.CLIENTSPERPAGE : pagesVar.endIndex;
	}
		
	drawClientList(pagesVar.curTab);
}

function doRefresh()
{
		parent.manualUpdate = true;
		document.form.refresh_networkmap_Flag.value = 1;
		document.form.submit();
		setTimeout("window.location.reload()", 1000);
}

function retOverLibStr(client){
	var overlibStr = "<p><%tcWebApi_get("String_Entry","MAC_Address","s")%>:</p>" + client.MacAddr.toUpperCase();

	if(client.ssid)
		overlibStr += "<p>SSID:</p>" + client.ssid;
	if(client.isLogin)
		overlibStr += "<p><%tcWebApi_get("String_Entry","CTL_localdevice","s")%>:</p>YES";
	if(client.isPrinter)
		overlibStr += "<p><%tcWebApi_get("String_Entry","Device_service_Printer","s")%></p>YES";
	if(client.isITunes)
		overlibStr += "<p><%tcWebApi_get("String_Entry","Device_service_iTune","s")%></p>YES";
	if(client.isWL > 0)
		overlibStr += "<p><%tcWebApi_get("String_Entry","Wireless_Radio","s")%>:</p>" + ((client.isWL == 2) ? "5GHz (" : "2.4GHz (") + client.rssi + "db)";
	
	return overlibStr;
}

var overlib_str_tmp = "";
function oui_query(mac) {
	var tab = new Array();	
	tab = mac.split(mac.substr(2,1));

  $j.ajax({
    url: 'https://services11.ieee.org/RST/standards-ra-web/rest/assignments/download/?registry=MA-L&format=html&text=' + tab[0] + tab[1] + tab[2],
		type: 'GET',
    error: function(xhr) {
			if(overlib.isOut)
				return true;
			else
				oui_query(mac);
    },
    success: function(response) {    	
			//if(overlib.isOut)
				//return nd();							
			var retData = response.responseText.split("pre")[1].split("(base 16)")[1].replace("PROVINCE OF CHINA", "R.O.C").split("&lt;/");						
			overlib_str_tmp = retOverLibStr(clientList[mac]);
			overlib_str_tmp +="<p><span>.....................................</span></p><p style='margin-top:5px'><%tcWebApi_get("String_Entry","Manufacturer","s")%> :</p>";
			overlib_str_tmp +=retData[0];
			
			return overlib(overlib_str_tmp);
		}    
  });
}

function popupCustomTable(mac){
	parent.popupEditBlock(clientList[mac]);
}

function updateClientList(e){
	$j.ajax({
		url: '/update_clients.asp',
		dataType: 'script', 
		error: function(xhr) {
			setTimeout("updateClientList();", 1000);
		},
		success: function(response){			
			document.getElementById("loadingIcon").style.visibility = (networkmap_fullscan == 1 && parent.manualUpdate) ? "visible" : "hidden";			
			if(isJsonChanged(originData, originDataTmp) || originData.fromNetworkmapd == ""){
				drawClientList();				
			}
			
			parent.show_client_status(originData.size);
			if(networkmap_fullscan == 0){
				parent.manualUpdate = false;
			}		
			
			setTimeout("updateClientList();", 3000);				
		}    
	});
}
</script>
</head>
<body class="statusbody" onload="initial();">
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>	
<form method="POST" name="form" action="/start_apply.asp" target="hidden_frame">
<input type="hidden" name="refresh_networkmap_Flag" value="0">	
</form>

<table width="320px" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td>		
			<table width="100px" border="0" align="left" style="margin-left:8px;" cellpadding="0" cellspacing="0">
				<td>
					<div id="tabOnline" class="tabclick_NW" align="center">
						<span>
							Online
						</span>
					</div>
					<script>
						document.getElementById('tabOnline').onclick = function(){
							pagesVar.resetVar();
							drawClientList('online');
							document.getElementById('tabOnline').className = 'tabclick_NW';
							document.getElementById('tabWired').className = 'tab_NW';
							document.getElementById('tabWireless').className = 'tab_NW';
							document.getElementById('tabCustom').className = 'tab_NW';
						}
					</script>
				</td>
				<td>
					<div id="tabWired" class="tab_NW" align="center" style="display:none">
						<span>
							Wired (<b style="font-size:10px;" id="tabWiredNum">0</b>)
						</span>
					</div>
					<script>
						document.getElementById('tabWired').onclick = function(){
							pagesVar.resetVar();
							drawClientList('wired');
							document.getElementById('tabOnline').className = 'tab_NW';
							document.getElementById('tabWired').className = 'tabclick_NW';
							document.getElementById('tabWireless').className = 'tab_NW';
							document.getElementById('tabCustom').className = 'tab_NW';
						}
					</script>
				</td>
				<td>
					<div id="tabWireless" class="tab_NW" align="center" style="display:none">
							<span id="tabWirelessSpan">						
    					Wireless (<b style="font-size:10px;" id="tabWirelessNum">0</b>)
    					</span>
							<nav class="nav" id="select_wlclient_band"></nav>   
					</div>
					<script>						
						function switchTab_drawClientList(wband){
							pagesVar.resetVar();
							drawClientList('wireless'+wband);
							document.getElementById('tabOnline').className = 'tab_NW';
							document.getElementById('tabWired').className = 'tab_NW';
							document.getElementById('tabWireless').className = 'tabclick_NW';
							document.getElementById('tabCustom').className = 'tab_NW';
						}

						$j('#tabWirelessSpan').click(function(){
							switchTab_drawClientList('');
						});
						
						$j('#tabWireless').mouseenter(function(){
							if(wl_nband_title.length > 0){
								$j("#select_wlclient_band").slideDown("fast", function(){});
								wirelessOverFlag = true;
							}
						});

						$j('#tabWireless').mouseleave(function(){
							$j("#select_wlclient_band").css({"display": "none"});
							wirelessOverFlag = false;
						});
					</script>
				</td>
				<td>
					<div id="tabCustom" class="tab_NW" align="center" style="display:none">
						<span>
							History (<b style="font-size:10px;" id="tabCustomNum">0</b>)
						</span>
					</div>
					<script>
						document.getElementById('tabCustom').onclick = function(){
							pagesVar.resetVar();
							drawClientList('custom');
							document.getElementById('tabOnline').className = 'tab_NW';
							document.getElementById('tabWired').className = 'tab_NW';
							document.getElementById('tabWireless').className = 'tab_NW';
							document.getElementById('tabCustom').className = 'tabclick_NW';
						}
					</script>
				</td>

				<td>
				</td>
			</table>
		</td>
	</tr>

	<tr>
		<td>				
			<table width="95%" border="0" align="center" cellpadding="4" cellspacing="0" style="background-color:#4d595d;">
  				<tr>
    				<td style="padding:3px 3px 5px 5px;">
						<input type="text" placeholder="Search" id="searchingBar" class="input_25_table" style="width:96%;margin-top:3px;margin-bottom:3px" maxlength="" value="" autocorrect="off" autocapitalize="off">
						<script>
							document.getElementById('searchingBar').onkeyup = function(){
								pagesVar.resetVar();
								drawClientList();
							}
						</script>

						<div id="client_list_Block"></div>
	    			</td>
  				</tr>
 			</table>
		</td>
	</tr>
</table>
<br/>
<img height="25" id="leftBtn" onclick="updatePagesVar('-');" style="cursor:pointer;margin-left:10px;" src="/images/arrow-left.png">
<input type="button" id="refresh_list" class="button_gen" onClick="return doRefresh();" value="<%tcWebApi_get("String_Entry","CTL_refresh","s")%>" style="margin-left:70px;">
<img src="/images/InternetScan.gif" id="loadingIcon" style="visibility:hidden">
<img height="25" id="rightBtn" onclick="updatePagesVar('+');" style="cursor:pointer;margin-left:25px;" src="/images/arrow-right.png">	

</body>

<!--device-map/clients.asp-->
</html>
