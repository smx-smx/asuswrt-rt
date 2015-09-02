<%
	tcWebApi_set("WebCurSet_Entry","dev_pvc","dpvc")
%>
<html><head>
</head>
<script language="JavaScript">
</script>
<body>
<form name="dvinfo">
<font color="#000000">Connection Type</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata" width="140"><% if tcWebApi_get("DeviceInfo_PVC","connType","h") <> "" then tcWebApi_staticGet("DeviceInfo_PVC","connType","s")  end if %></td>
<font color="#000000">Status</font></div></td><td class="tabdata" ><div align=center>:</div></td><td class="tabdata" colspan="2"><% If tcWebApi_staticGet("DeviceInfo_PVC","Status","h") = "1" Then asp_Write("Connection") else asp_Write("Not Connected") end if %></td>
<input type="text" name="dpvc" value="0">
</form>
</body>
</html>

