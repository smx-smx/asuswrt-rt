var wan_type = "<%tcWebApi_staticGet("DeviceInfo_PVC","connType","s")%>";
var wan_IP = "<%tcWebApi_staticGet("DeviceInfo_PVC","WanIP","s")%>";
var str_type = "<% tcWebApi_get("Dproxy_Entry","type","s") %>";
var str_PriDNS = "<% tcWebApi_staticGet("Dproxy_Entry","Primary_DNS","s") %>";
var str_DevDNS = "<% tcWebApi_staticGet("DeviceInfo_PVC","DNSIP","s") %>";
var str_DevDNS2 = "<% tcWebApi_staticGet("DeviceInfo_PVC","SECDNSIP","s") %>";
var wan_Gateway = "<%tcWebApi_staticGet("DeviceInfo_PVC","WanDefGW","s")%>";
