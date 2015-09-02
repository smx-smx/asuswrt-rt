<%
	tcWebApi_constSet("WebCurSet_Entry","wlan_id","0")
%>
<HTML><HEAD>
<META HTTP-EQUIV="pragma" CONTENT="no-cache"> 
<META HTTP-EQUIV="Cache-Control" CONTENT="no-store, must-revalidate"> 
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<STYLE type=text/css></STYLE>
</HEAD>
<BODY>
<FORM name=Sys_info method=post>
<H2 align=center><FONT color=#000000>System Info</FONT></H2>
<TABLE cellSpacing=0 cellPadding=3 align=center bgColor=#ffffff border=1>
	<TBODY>
		<TR>
			<TD width=120>
			<DIV align=left><FONT color=#000000>Software Version</FONT></DIV></TD>
			<TD><%If tcWebApi_get("DeviceInfo","Customer_FwVer","h") <> "N/A" Then tcWebApi_staticGet("DeviceInfo","Customer_FwVer","s") end if%></TD>
		</TR>
		<TR>
			<TD width=160>
			<DIV align=left>MAC Address</DIV></TD>
			<TD><%If tcWebApi_get("Info_Ether","mac","h") <> "N/A" Then tcWebApi_staticGet("Info_Ether","mac","s") end if%></TD>
		</TR>
		<TR>
		<TR>
			<TD width=160>
			<DIV align=left>PVC</DIV></TD>
			<TD><%If tcWebApi_get("DeviceInfo","NewFwVerPvc","h") <> "N/A" Then tcWebApi_staticGet("DeviceInfo","NewFwVerPvc","s") end if%></TD>
		</TR>
		<TR>
			<TD width=160>
			<DIV align=left>Device ID</DIV></TD>
			<TD><FONT color=#000000><%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%><%if tcWebApi_get("SysInfo_Entry","ProductName","h") <> "N/A" then tcWebApi_get("SysInfo_Entry","ProductName","s") end if%></FONT></TD>
		</TR>
		<% If tcWebApi_get("Info_WLan","isExist","h") <> "Error" Then%>
		<TR>
			<TD width=160>
			<DIV align=left>SSID</DIV></TD>
			<TD><%If tcWebApi_get("WLan_Entry","SSID","h") <> "N/A" then tcWebApi_staticGet("WLan_Entry","SSID","s") end if%></TD>
		</TR>
		<%End If%>
		<TR>
			<TD width=160>
			<DIV align=left><FONT color=#000000>Upstream Rate (Kbps)</FONT></DIV></TD>
			<TD><%tcWebApi_get("Info_Adsl","DataRateUp","s")%></TD>
		</TR>
		<TR>
			<TD width=160>
			<DIV align=left><FONT color=#000000>Downstream Rate (Kbps)</FONT></DIV></TD>
			<TD><%tcWebApi_staticGet("Info_Adsl","DataRateDown","s")%></TD>
		</TR>
	</TBODY>
</TABLE>
</FORM>
</BODY>
</HTML>
