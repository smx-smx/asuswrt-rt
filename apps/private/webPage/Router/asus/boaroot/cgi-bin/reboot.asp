<%
If Request_Form("rebootFlag") = "1"  Then
	TCWebApi_set("System_Entry","reboot_type","restoreFlag")
	tcWebApi_CommitWithoutSave("System_Entry")
End If
%>
