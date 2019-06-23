/* Plugin */
var isJsonChanged = function(objNew, objOld){
	for(var i in objOld){	
		if(typeof objOld[i] == "object"){
			if(objNew[i].join() != objOld[i].join()){
				return true;
			}
		}
		else{
			if(typeof objNew[i] == "undefined" || objOld[i] != objNew[i]){
				return true;				
			}
		}
	}

    return false;
};

var ipState = new Array();
ipState["Static"] =  "<%tcWebApi_get("String_Entry","BOP_ctype_title5","s")%>";
ipState["DHCP"] =  "<%tcWebApi_get("String_Entry","BOP_ctype_title1","s")%>";
ipState["Manual"] =  "MAC-IP Binding";
ipState["OffLine"] =  "Client is disconnected";


/* get client info form dhcp lease log */
var leaseArray = {
	hostname: [],
	mac: []
};

var retHostName = function(_mac){
	return leaseArray.hostname[leaseArray.mac.indexOf(_mac.toUpperCase())] || _mac;
}
/* end */

var networkmap_fullscan = '<%tcWebApi_get("ClientList_Common","scan","s")%>';	/*1: scan, 0: done*/

var originDataTmp;
var originData = {
	onlinelist: '<% get_client_list() %>',
	onlinelist_cache: '<% get_client_list_cache() %>',
	size: decodeURIComponent('<% get_client_list_cache() %>').split("<").length-1,
	customList: '<% get_cl_userdef_list() %>',
	asusDevice: '<% get_asus_dev_list() %>',
	fromDHCPLease: <% dhcpLeaseMacList(); %>,	//Viz add but not exist yet
	staticList: decodeURIComponent('<% get_static_dhcp_list() %>').replace(/&#62/g, ">").replace(/&#60/g, "<").split('<'),
	fromNetworkmapd: '',
	//fromBWDPI: '',
	wlList_2g: [<% wl_sta_list_2g() %>],
	wlList_5g: [<% wl_sta_list_5g() %>],
	wlList_5g_2: [],
	qosRuleList: '',
	init: true
}


var totalClientNum = {
	online: 0,
	wireless: 0,
	wired: 0,
	wireless_0: 0,
	wireless_1: 0,
	wireless_2: 0
}

var setClientAttr = function(){
	this.type = "";
	this.Name = "";
	this.IP = "offline";
	this.MacAddr = "";
	this.from = "";
	this.macRepeat = 1;
	this.group = "";
	this.dpiType = "";
	this.rssi = "";
	this.ssid = "";
	this.isWL = 0; // 0: wired, 1: 2.4GHz, 2: 5GHz/5GHz-1 3:5GHz-2.
	this.qosLevel = "";
	this.curTx = "";
	this.curRx = "";
	this.totalTx = "";
	this.totalRx = "";
	this.callback = "";
	this.keeparp = "";
	this.isGateway = false;
	this.isWebServer = false;
	this.isPrinter = false;
	this.isITunes = false;
	this.isASUS = false;
	this.isLogin = false;
	this.isOnline = false;
	this.ipMethod = "Static";
}

var Login_ip_str = '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>';
var clientList = new Array(0);
var ClientRow = new Array(0);
function genClientList(){
	
	leaseArray = {hostname: [], mac: []};	
	for(var i = 0; i < originData.fromDHCPLease.length; i += 1) {		
		var dhcpMac = originData.fromDHCPLease[i][0].toUpperCase();
		var dhcpName = decodeURIComponent(originData.fromDHCPLease[i][1]);
		if(dhcpMac != "") {
			leaseArray.mac.push(dhcpMac);
			leaseArray.hostname.push(dhcpName);			
		}		
	}
	
	clientList = [];
	totalClientNum.wireless = 0;
	totalClientNum.wireless_0 = 0;
	totalClientNum.wireless_1 = 0;
	totalClientNum.wireless_2 = 0;

	if(networkmap_fullscan == 0){
		
		totalClientNum.online = originData.size;
		ClientRow = originData.onlinelist.split("<");
	}
	else{
		
		totalClientNum.online = originData.size;
		ClientRow = originData.onlinelist_cache.split("<");
	}
	
	var ClientField;
	if(typeof ClientRow != "undefined"){
			for(var i=1; i<=totalClientNum.online; i++){
				//[dev]:   IP     >  mac   >  name > type  > httpd  > isprinter  > itunes
				//dev0=192.168.1.2>e8:9a:8f:04:af:31>KyCH-NB>6>0>0>0
				ClientField = ClientRow[i].split(">");
				thisClientMacAddr = ClientField[1].toUpperCase();
				if(!thisClientMacAddr){
					continue;
				}
				
				if(typeof clientList[thisClientMacAddr] == "undefined"){
					clientList.push(thisClientMacAddr);
					clientList[thisClientMacAddr] = new setClientAttr();
					clientList[thisClientMacAddr].from = "networkmapd";
				}
				else{
					if(clientList[thisClientMacAddr].from == "networkmapd")
						clientList[thisClientMacAddr].macRepeat++;
					else
						clientList[thisClientMacAddr].from = "networkmapd";
				}
				
				/*if(typeof clientList[thisClientMacAddr] == "undefined"){
					clientList.push(thisClientMacAddr);
					clientList[thisClientMacAddr] = new setClientAttr();
					clientList[thisClientMacAddr].from = "asusDevice";
				}
				else{
					if(clientList[thisClientMacAddr].from == "asusDevice")
						clientList[thisClientMacAddr].macRepeat++;
					else
						clientList[thisClientMacAddr].from = "asusDevice";
				}*/
				
				clientList[thisClientMacAddr].IP = ClientField[0];
				clientList[thisClientMacAddr].isLogin = (ClientField[0] == Login_ip_str) ? true : false;		
				clientList[thisClientMacAddr].MacAddr = ClientField[1];
				clientList[thisClientMacAddr].Name = ClientField[2].trim();
				if(clientList[thisClientMacAddr].Name == ""){
					clientList[thisClientMacAddr].Name = clientList[thisClientMacAddr].MacAddr;
				}
				clientList[thisClientMacAddr].type = ClientField[3];
				clientList[thisClientMacAddr].dpiType = '';//for image_Type title
				clientList[thisClientMacAddr].isGateway = (ClientField[0] == '<% tcWebApi_get("Lan_Dhcp","IP","s"); %>') ? true : false;
				clientList[thisClientMacAddr].isWebServer = false;	//Viz: need to confirm if exists webserver for this IP
				clientList[thisClientMacAddr].isHttp = ClientField[4];		
				clientList[thisClientMacAddr].isPrinter = (ClientField[5] == 0) ? false : true; // Viz for temp : ClientField[5];
				clientList[thisClientMacAddr].isITunes = (ClientField[6] == 0) ? false : true;  // Viz for temp : ClientField[6];
				clientList[thisClientMacAddr].isOnline = true;		
				//var thisClientMacisOnline = true;
				
			}
	}
	
	for(var i=0; i<originData.wlList_2g.length; i++){
		var wlClientMacAddr = (typeof originData.wlList_2g[i][0] == "undefined") ? false : originData.wlList_2g[i][0].toUpperCase();

		if(!wlClientMacAddr || typeof clientList[thisClientMacAddr] == "undefined"){
			continue;
		}		

		if(typeof clientList[wlClientMacAddr] != "undefined"){
			clientList[wlClientMacAddr].rssi = originData.wlList_2g[i][1];
			clientList[wlClientMacAddr].isWL = 1;
			totalClientNum.wireless++;
			totalClientNum.wireless_0++;		
		}		
	}	
	
	for(var i=0; i<originData.wlList_5g.length; i++){		
		var wlClientMacAddr = (typeof originData.wlList_5g[i][0] == "undefined") ? false : originData.wlList_5g[i][0].toUpperCase();

		if(!wlClientMacAddr || typeof clientList[thisClientMacAddr] == "undefined"){
			continue;
		}

		if(typeof clientList[wlClientMacAddr] != "undefined"){
			clientList[wlClientMacAddr].rssi = originData.wlList_5g[i][1];
			clientList[wlClientMacAddr].isWL = 2;
			totalClientNum.wireless++;
			totalClientNum.wireless_1++;
		}
	}
	
	for(var i = 0; i < leaseArray.mac.length; i += 1) {
		if(typeof clientList[leaseArray.mac[i]] != "undefined"){
			clientList[leaseArray.mac[i]].ipMethod = "DHCP";
		}
	}

	for(var i=0; i<originData.staticList.length; i++){
		if(originData.staticList.length == "0") break;	//nvram : "dhcp_static_x"	//'192.168.100.40>F4:6D:04:5D:9C:68<192.168.100.41>F4:6D:04:5D:9C:55'

		var thisClient = originData.staticList[i].split(">");
		var thisClientMacAddr = (typeof thisClient[1] == "undefined") ? false : thisClient[1].toUpperCase();

		if(!thisClientMacAddr || typeof clientList[thisClientMacAddr] == "undefined"){
			continue;
		}

		if(typeof clientList[thisClientMacAddr] != "undefined"){
			if(clientList[thisClientMacAddr].ipMethod == "DHCP") {				
				if(clientList[thisClientMacAddr].IP == thisClient[0] || clientList[thisClientMacAddr].IP == "offline")
					clientList[thisClientMacAddr].ipMethod = "Manual";
			}			
		}
	}
	
	totalClientNum.wired = parseInt(totalClientNum.online - totalClientNum.wireless);
}

function getUserIcon(clientMac, custom_usericon) {	
	var userIcon = "";
	var custom_usericon_array = custom_usericon.split("<");
	for(var i = 0; i < custom_usericon_array.length; i += 1) {
		var custom_usericon_array_col = custom_usericon_array[i].split(">");
		if(custom_usericon_array_col[0] == clientMac) {
			userIcon = custom_usericon_array_col[1];
			break;
		}
	}	
	return userIcon;
}
