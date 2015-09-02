<%
	tcWebApi_set("WebCurSet_Entry","wan_pvc","pvc")


If Request_Form("flag") = "1" Then
	tcWebApi_set("Wan_PVC","USERNAME","wan_PPPUsername")
	tcWebApi_set("Wan_PVC","PASSWORD","wan_PPPPassword")
	tcWebApi_set("Wan_PVC","ENCAP","wan_PPPEncap")
	tcWebApi_set("Wan_PVC","Active","active") 
	tcWebApi_set("Wan_PVC","ISP","isp") 
	tcWebApi_set("Wan_PVC","NATENABLE","nat")
	tcWebApi_set("Wan_PVC","DEFAULTROUTE","defrouter")
	tcWebApi_commit("Wan_PVC")
End If
%>
<html><head>
</head>
<script language="JavaScript">
function doLoad()
{
	document.wan.flag.value = 0;
}
</script>
<body onLoad="doLoad()">
<form name="wan" METHOD="POST">
<INPUT TYPE="TEXT" NAME="wan_PPPServicename" SIZE="32" MAXLENGTH="31" VALUE="OTE">
<INPUT TYPE="TEXT" NAME="wan_PPPUsername" SIZE="32" MAXLENGTH="70" VALUE="<%if tcWebApi_get("Wan_PVC","USERNAME","h") <> "N/A" then tcWebApi_get("Wan_PVC","USERNAME","s") end if%>">
<INPUT TYPE="PASSWORD" NAME="wan_PPPPassword" SIZE="32" MAXLENGTH="70" VALUE="<%if Request_Form("flag") = "1" then if tcWebApi_get("Wan_PVC","PASSWORD","h") <> "N/A" then tcWebApi_get("Wan_PVC","PASSWORD","s") end if end if%>">
<INPUT TYPE="TEXT" NAME="Alwan_VPI" SIZE="5" MAXLENGTH="3" VALUE="<%if tcWebApi_get("Wan_PVC","VPI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VPI","s") else asp_Write("0") end if%>">
<INPUT TYPE="TEXT" NAME="Alwan_VCI" SIZE="5" MAXLENGTH="5" VALUE="<%if tcWebApi_get("Wan_PVC","VCI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VCI","s") else asp_Write("0") end if%>">

<SELECT NAME="wan_PPPEncap" SIZE="1" onchange="doBridgeSwitch()"><OPTION <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE LLC" then asp_Write("SELECTED") end if %>>PPPoE LLC
<OPTION <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE VC-Mux" then asp_Write("SELECTED") end if %>>PPPoE VC-Mux
<OPTION <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA LLC" then asp_Write("SELECTED") end if %>>PPPoA LLC
<OPTION <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA VC-Mux" then asp_Write("SELECTED") end if %>>PPPoA VC-Mux</SELECT>

</form>
</body>
</html>
