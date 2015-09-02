#!/bin/bash
rm -rf tr.db

sqlite3 tr.db "CREATE TABLE tr(
    id INTEGER PRIMARY KEY, 
    pid INTEGER DEFAULT -1,
    name TEXT NOT NULL COLLATE NOCASE,
    rw varchar(5) DEFAULT 'true',
    getc varchar(5) DEFAULT 'false',
    noc INTEGER DEFAULT 0,
    nocc charchar(3) DEFAULT '',
    nin INTEGER DEFAULT 1,
    il INTEGER DEFAULT 0,
    acl varchar(255) DEFAULT '',
    type varchar(20) DEFAULT 'node',
    value TEXT DEFAULT NULL,
    UNIQUE(pid, name));"

#pid is -1 means the root node of the tree
#################################################
#Create the Device
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, type) VALUES (-1, 'Device', 'false', 'node');"
device=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=-1;"`
#################################################

#################################################
#Create the ManagementServer
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, type) VALUES ($device, 'ManagementServer', 'false', 'node');"
server=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='ManagementServer';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'URL', 'true', 0, '', '', 'string', 'http://172.31.0.162/comserver/node1/tr069');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'ConnectionRequestURL', 'true', 0, '', '', 'string', 'http://127.0.0.1:7547');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'ParameterKey', 'true', 0, '', '', 'string', 'parameterkey');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'PeriodicInformInterval', 'true', 0, '', '', 'int', '30');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'PeriodicInformTime', 'true', 0, '', '', 'dateTime', '2009-01-01T00:00:00');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'PeriodicInformEnable', 'true', 0, '', '', 'boolean', 'true');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'Username', 'true', 0, '', '', 'string', 'username');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'Password', 'true', 'true', 0, '', '', 'string', 'password');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($server, 'ConnectionRequestUsername', 'true', 0, '', '', 'string', 'username');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'ConnectionRequestPassword', 'true', 'true', 0, '', '', 'string', 'password');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'UDPConnectionRequestAddress', 'false', 'false', 0, '', '', 'string', '127.0.0.1:7547');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'UDPConnectionRequestAddressNotificationLimit', 'true', 'false', 0, '', '', 'unsignedInt', '30');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'STUNEnable', 'true', 'false', 0, '', '', 'boolean', 'true');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'STUNServerAddress', 'true', 'false', 0, '', '', 'string', '116.213.94.142');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'STUNServerPort', 'true', 'false', 0, '', '', 'unsignedInt', '3478');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'STUNUsername', 'true', 'false', 0, '', '', 'string', 'test');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'STUNPassword', 'true', 'true', 0, '', '', 'string', 'f5d74b9277dc7f03587630ee535bf017e0fb75c7');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'STUNMaximumKeepAlivePeriod', 'true', 'false', 0, '', '', 'int', '3600');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'STUNMinimumKeepAlivePeriod', 'true', 'false', 0, '', '', 'unsignedInt', '10');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'NATDetected', 'false', 'false', 2, '2', '', 'boolean', 'false');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'CWMPRetryMinimumWaitInterval', 'true', 'false', 0, '', '', 'unsignedInt', '5');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, getc, noc, nocc, acl, type, value) VALUES ($server, 'CWMPRetryIntervalMultiplier', 'true', 'false', 0, '', '', 'unsignedInt', '2000');"
#################################################



#################################################
#Create the DeviceInfo
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'DeviceInfo', 'false', 1, 'node');"
devinfo=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='DeviceInfo';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'Manufacturer', 'false', 0, '0', '', 'string', 'TRAgent');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'ManufacturerOUI', 'false', 0, '0', '', 'string', 'TRAgent');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'ProductClass', 'false', 0, '0', '', 'string', 'TRAgent');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'SerialNumber', 'false', 0, '0', '', 'string', '123456789ABC');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'SoftwareVersion', 'false', 2, '2', '', 'string', '1.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'HardwareVersion', 'false', 0, '0', '', 'string', '1.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'SpecVersion', 'false', 2, '2', '', 'string', '1.1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'ProvisioningCode', 'true', 2, '2', '', 'string', '123');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($devinfo, 'X_WKS_Key', 'true', 'false', '0', '', 'string', 'wkss');"
#################################################

#################################################
#Create the WANDevice
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'WANDevice', 'false', 1, 'node');"
wandev=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='WANDevice';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($wandev, '1', 'false', 1, 'node');"
wandev1=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$wandev AND name='1';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($wandev1, 'WANConnectionDevice', 'false', 1, 'node');"
wandev2=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$wandev1 AND name='WANConnectionDevice';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($wandev2, '1', 'false', 1, 'node');"
wandev3=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$wandev2 AND name='1';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($wandev3, 'WANPPPConnection', 'false', 1, 'node');"
wandev4=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$wandev3 AND name='WANPPPConnection';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($wandev4, '1', 'false', 1, 'node');"
wandev5=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$wandev4 AND name='1';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($wandev5, 'ExternalIPAddress', 'true', 0, '2', '', 'string', '1.2.3.4');"
#################################################

#################################################
#Create the Layer3Forwarding
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'Layer3Forwarding', 'false', 1, 'node');"
l3f=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='Layer3Forwarding';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($l3f, 'DefaultConnectionService', 'true', 2, '2', '', 'string', 'value');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($l3f, 'ForwardNumberOfEntries', 'true', 0, '2', '', 'unsignedInt', '1');"
#################################################

#################################################
#Create the IPPingDiagnostics
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'IPPingDiagnostics', 'false', 1, 'node');"
ipping=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='IPPingDiagnostics';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'DiagnosticsState', 'true', 2, '2', '', 'string', 'Complete');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'Interface', 'true', 2, '2', '', 'string', 'eth0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'Host', 'true', 2, '2', '', 'string', 'www.sina.com');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'NumberOfRepetitions', 'true', 0, '2', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'Timeout', 'true', 0, '2', '', 'unsignedInt', '30');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'DataBlockSize', 'true', 0, '2', '', 'unsignedInt', '32');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'DSCP', 'true', 0, '2', '', 'unsignedInt', '32');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'SuccessCount', 'false', 0, '2', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'FailureCount', 'false', 0, '2', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'AverageResponseTime', 'false', 0, '2', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'MinimumResponseTime', 'false', 0, '2', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ipping, 'MaximumResponseTime', 'false', 0, '2', '', 'unsignedInt', '1');"
#################################################

#################################################
#Create the TraceRouteDiagnostics
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'TraceRouteDiagnostics', 'false', 1, 'node');"
traceroute=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='TraceRouteDiagnostics';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'DiagnosticsState', 'true', 2, '2', '', 'string', 'Complete');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'Interface', 'true', 2, '2', '', 'string', 'eth0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'Host', 'true', 2, '2', '', 'string', 'www.sina.com');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'NumberOfTries', 'true', 2, '2', '', 'string', 'www.sina.com');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'Timeout', 'true', 0, '2', '', 'unsignedInt', '30');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'DataBlockSize', 'true', 0, '2', '', 'unsignedInt', '32');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'DSCP', 'true', 0, '2', '', 'unsignedInt', '32');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'MaxHopCount', 'true', 0, '2', '', 'unsignedInt', '32');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'ResponseTime', 'true', 2, '2', '', 'UnsignedInt', '12');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($traceroute, 'RouteHopsNumberOfEntries', 'true', 2, '2', '', 'unsignedInt', '5');"
#################################################


#################################################
#Create the Capabilities
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'Capabilities', 'false', 1, 'node');"
cap=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='Capabilities';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($cap, 'PerformanceDiagnostic', 'false', 1, 'node');"
pd=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$cap AND name='PerformanceDiagnostic';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($pd, 'DownloadTransports', 'false', 0, '0', '', 'string', 'HTTP');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($pd, 'UploadTransports', 'false', 0, '0', '', 'string', 'HTTP');"
#################################################

#################################################
#Create the DownloadDiagnostics
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'DownloadDiagnostics', 'false', 1, 'node');"
dd=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='DownloadDiagnostics';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'DiagnosticsState', 'true', 2, '2', '', 'string', 'Completed');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'Interface', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'DownloadURL', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'DSCP', 'true', 0, '0', '', 'unsignedInt[0:63]', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'EthernetPriority', 'true', 0, '0', '', 'unsignedInt[0:7]', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'ROMTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'BOMTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'EOMTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'TestBytesReceived', 'false', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'TotalBytesReceived', 'false', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'TCPOpenRequestTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($dd, 'TCPOpenResponseTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
#################################################


#################################################
#Create the UploadDiagnostics
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'UploadDiagnostics', 'false', 1, 'node');"
ud=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='UploadDiagnostics';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'DiagnosticsState', 'true', 2, '2', '', 'string', 'Completed');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'Interface', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'UploadURL', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'DSCP', 'true', 0, '0', '', 'unsignedInt[0:63]', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'EthernetPriority', 'true', 0, '0', '', 'unsignedInt[0:7]', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'TestFileLength', 'true', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'ROMTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'BOMTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'EOMTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'TotalBytesSent', 'false', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'TCPOpenRequestTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($ud, 'TCPOpenResponseTime', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
#################################################


#################################################
#Create the UDPEcho
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'UDPEchoConfig', 'false', 1, 'node');"
echo=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='UDPEchoConfig';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'Enable', 'true', 0, '0', '', 'boolean', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'Interface', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'SourceIPAddress', 'true', 0, '0', '', 'string', '0.0.0.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'UDPPort', 'true', 0, '0', '', 'unsignedInt', '1235');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'EchoPlusEnabled', 'true', 0, '0', '', 'boolean', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'EchoPlusSupported', 'false', 0, '0', '', 'boolean', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'PacketsReceived', 'false', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'PacketsResponded', 'false', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'BytesReceived', 'false', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'BytesResponded', 'false', 0, '0', '', 'unsignedInt', '0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'TimeFirstPacketReceived', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($echo, 'TimeLastPacketReceived', 'false', 0, '0', '', 'dateTime', '0001-01-01T00:00:00.0');"
#################################################


#################################################
#Create the SelfTest Diagnostics
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'SelfTestDiagnostics', 'false', 1, 'node');"
self=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='SelfTestDiagnostics';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($self, 'DiagnosticsState', 'true', 0, '0', '', 'string', 'Complete');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($self, 'Results', 'false', 0, '0', '', 'string', '');"
#################################################


#################################################
#Create the NSLookup Diagnostics
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($device, 'NSLookupDiagnostics', 'false', 1, 'node');"
nslookup=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='NSLookupDiagnostics';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'DiagnosticsState', 'true', 0, '0', '', 'string', 'Complete');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'Interface', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'HostName', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'DNSServer', 'true', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'Timeout', 'true', 0, '0', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'NumberOfRepetitions', 'true', 0, '0', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'SuccessCount', 'true', 0, '0', '', 'unsignedInt', '1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($nslookup, 'ResultNumberOfEntries', 'true', 0, '0', '', 'unsignedInt', '1');"

sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($nslookup, 'Result', 'false', 1, 'node');"
result=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$nslookup AND name='Result';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, type) VALUES ($result, '1', 'false', 1, 'node');"
inst=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$result AND name='1';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($inst, 'Status', 'false', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($inst, 'AnswerType', 'false', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($inst, 'HostNameReturned', 'false', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($inst, 'IPAddress', 'false', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($inst, 'DNSServerIP', 'false', 0, '0', '', 'string', '');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($inst, 'ResponseTime', 'false', 0, '0', '', 'string', '');"
#################################################


#################################################
#Create Multi-Instance Sample
#
# InternetGatewayDevice +----- Others
#                       |
#                       |
#                       +----- MultiInstanceSample +----- 1 +----- SrcDest
#                                                  |        |
#                                                  |        |
#                                                  |        +----- MappedDest
#                                                  |
#                                                  |
#                                                  +----- 2 +----- SrcDest
#                                                           |
#                                                           |
#                                                           +----- MappedDest
#

sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, il, type) VALUES ($device, 'MultiInstanceSample', 'true', 1, 3, 'node');"
multi=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$device AND name='MultiInstanceSample';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, nin, il, type) VALUES ($multi, 'template', 'true', 0, 0, 'node');"
temp=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$multi AND name='template';"`

sqlite3 tr.db "INSERT INTO tr(pid, name, rw, type) VALUES ($temp, 'user1', 'false', 'node');"
user1=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$temp AND name='user1';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user1, 'username1', 'true', 0, '0', '', 'string', 'username1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user1, 'username2', 'true', 0, '0', '', 'string', 'username2');"

sqlite3 tr.db "INSERT INTO tr(pid, name, rw, type) VALUES ($temp, 'user2', 'false', 'node');"
user2=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$temp AND name='user2';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user2, 'username1', 'true', 0, '0', '', 'string', 'username1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user2, 'username2', 'true', 0, '0', '', 'string', 'username2');"

sqlite3 tr.db "INSERT INTO tr(pid, name, rw, type) VALUES ($user2, 'user2_1', 'false', 'node');"
user2_1=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$user2 AND name='user2_1';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user2_1, 'username1', 'true', 0, '0', '', 'string', 'username1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user2_1, 'username2', 'true', 0, '0', '', 'string', 'username2');"

sqlite3 tr.db "INSERT INTO tr(pid, name, rw, type) VALUES ($user2_1, 'user2_1_1', 'false', 'node');"
user2_1_1=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$user2_1 AND name='user2_1_1';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user2_1_1, 'username1', 'true', 0, '0', '', 'string', 'username1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user2_1_1, 'username2', 'true', 0, '0', '', 'string', 'username2');"

sqlite3 tr.db "INSERT INTO tr(pid, name, rw, type) VALUES ($temp, 'user3', 'false', 'node');"
user3=`sqlite3 tr.db "SELECT id FROM tr WHERE pid=$temp AND name='user3';"`
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user3, 'username1', 'true', 0, '0', '', 'string', 'username1');"
sqlite3 tr.db "INSERT INTO tr(pid, name, rw, noc, nocc, acl, type, value) VALUES ($user3, 'username2', 'true', 0, '0', '', 'string', 'username2');"

