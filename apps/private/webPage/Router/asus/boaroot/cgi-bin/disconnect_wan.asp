<%
	tcWebApi_set("WebCurSet_Entry","dev_pvc","DvInfo_PVC")
	If Request_Form("Saveflag")="1" then
		tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
		If Request_Form("Dipflag")="1" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="2" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="3" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_set("DeviceInfo_PVC","IP6DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		elseif Request_Form("Dipflag")="4" then
			tcWebApi_set("DeviceInfo_PVC","DispBtnType","Dipflag")
			tcWebApi_set("DeviceInfo_PVC","IP6DispBtnType","Dipflag")
			tcWebApi_commit("DeviceInfo_PVC")
		END If
		If Request_Form("DipConnFlag")="1" then
			tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
			tcWebApi_commit("DeviceInfo_PVC");
		elseif Request_Form("DipConnFlag")="2" then
			tcWebApi_set("DeviceInfo_PVC","DispConnBtnType","DipConnFlag")
			tcWebApi_commit("DeviceInfo_PVC");
		END If
	END If
%>
