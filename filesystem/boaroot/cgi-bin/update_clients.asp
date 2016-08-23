originDataTmp = originData;
originData = {
	onlinelist: '<% get_client_list() %>',
	onlinelist_cache: '<% get_client_list_cache() %>',
	size: decodeURIComponent('<% get_client_list_cache() %>').split("<").length-1,
	customList: '<% get_cl_userdef_list() %>',
	asusDevice: '<% get_asus_dev_list() %>',
	fromDHCPLease: <% dhcpLeaseMacList(); %>,
	staticList: decodeURIComponent('<% get_static_dhcp_list() %>').split("<"),
	fromNetworkmapd: 'ajax',
	//fromBWDPI: '',
	wlList_2g: [<% wl_sta_list_2g() %>],
	wlList_5g: [<% wl_sta_list_5g() %>],
	wlList_5g_2: [],
	qosRuleList: '',
	init: true
}

networkmap_fullscan = '<%tcWebApi_get("ClientList_Common","scan","s")%>';	/*1: scan, 0: done*/
genClientList();

