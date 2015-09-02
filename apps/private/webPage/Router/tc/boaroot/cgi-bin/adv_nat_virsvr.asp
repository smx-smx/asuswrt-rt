<%
If Request_Form("editFlag") = "1" then
	tcWebApi_set("WebCurSet_Entry","virServ_id","editnum")
	tcWebApi_set("VirServer_PVC_Entry","STARTPORT","start_port")
	tcWebApi_set("VirServer_PVC_Entry","ENDPORT","end_port")
	tcWebApi_set("VirServer_PVC_Entry","LOCALIP","Addr")
If Request_Form("isLocalPortSupport") = "Yes" then
	tcWebApi_set("VirServer_PVC_Entry","LOCAL_SPORT","local_sport")
	tcWebApi_set("VirServer_PVC_Entry","LOCAL_EPORT","local_eport")
end if
	tcWebApi_commit("WebCurSet_Entry")
	tcWebApi_commit("VirServer_PVC_Entry")
elseif Request_Form("delFlag") = "1" then
  tcWebApi_set("WebCurSet_Entry","virServ_id","editnum")
  tcWebApi_unset("VirServer_PVC_Entry")
  tcWebApi_commit("VirServer_PVC_Entry")
end if  
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language='javascript'>

function chkPortRange()
{	
	var port = parseInt(document.VIRTUALSVR_form.start_port.value);
	var port2 = parseInt(document.VIRTUALSVR_form.end_port.value);
	var port3;
	var port4;
	
	if (port > 65535 || port < 1 || port2 > 65535 || port2 < 1 || isNaN(port) || isNaN(port2))
	{	
		alert("The Port value must position between 1 to 65535.");
		return true;	
	}
	if (port2 < port)
	{	
		alert("End port must greater than Start port.");
		return true;
	}
	
	if(document.VIRTUALSVR_form.isLocalPortSupport.value == "Yes"){
		port3 = parseInt(document.VIRTUALSVR_form.local_sport.value);
		port4 = parseInt(document.VIRTUALSVR_form.local_eport.value);
		
		if (port3 > 65535 || port3 < 1 || port4 > 65535 || port4 < 1 || isNaN(port3) || isNaN(port4))
		{	
			alert("The Port value must position between 1 to 65535.");
			return true;	
		}
		if (port4 < port3)
		{	
			alert("End port must greater than Start port.");
			return true;
		}
	}
	if (document.VIRTUALSVR_form.AccoutAclSupported.value == "Yes") {
		var acl_ports = document.VIRTUALSVR_form.ACLPort.value;
		var ports_array = acl_ports.split(",");

		var i;
		for (i = 0; i < ports_array.length; i++) {
			if (port <= parseInt(ports_array[i]) && port2 >= parseInt(ports_array[i])) {
				alert("This port has been used by ACL.");
				return true;
			}
		}
	}
	
	Sports = new Array(10);
	Eports = new Array(10);
	return false;
}

function chkRule(){
	var start_port = new Array(10);
	var end_port = new Array(10);
	var ipaddr = new Array(10);
	var edit_id = document.VIRTUALSVR_form.editnum.value;
	var edit_start_port = parseInt(document.VIRTUALSVR_form.start_port.value);
	var edit_end_port = parseInt(document.VIRTUALSVR_form.end_port.value);
	var edit_ipaddr = document.VIRTUALSVR_form.Addr.value.split(".");	
					
	for(i = 0 ; i < 10 ; i++){
		ipaddr[i] = new Array(4);
		
		start_port[i] = parseInt(document.getElementById("cgi_frame").contentWindow.document.getElementById("CgiInnerTable").rows[i+1].cells[1].innerHTML);
		end_port[i] = parseInt(document.getElementById("cgi_frame").contentWindow.document.getElementById("CgiInnerTable").rows[i+1].cells[2].innerHTML);
		ipaddr[i] = document.getElementById("cgi_frame").contentWindow.document.getElementById("CgiInnerTable").rows[i+1].cells[3].innerHTML.split(".");
	}

	for(i = 0; i < 10; i++){
		if((edit_id != i) && (isNaN(start_port[i]) == 0)){
			if((Number(ipaddr[i][0]) == Number(edit_ipaddr[0])) && (Number(ipaddr[i][1]) == Number(edit_ipaddr[1])) &&
			   (Number(ipaddr[i][2]) == Number(edit_ipaddr[2])) && (Number(ipaddr[i][3]) == Number(edit_ipaddr[3]))){
				if((edit_start_port == start_port[i]) && (edit_end_port == end_port[i])){ 
			   		alert("Invalid !! The same NAT rules");
					return true;
				}			
			}
			else{
				if(!((edit_start_port > end_port[i]) || (edit_end_port < start_port[i]))){
					alert("Invalid !! One port should not be assigned to different IP address");
					return true;
				}				
			}
		}
	}

	return false;
}

function doSave() {
	if (chkPortRange())
		return;
	if (inValidIPAddr(document.VIRTUALSVR_form.Addr.value))
		return;
	if (chkRule())
		return;
	document.VIRTUALSVR_form.editFlag.value ="1"	
	document.VIRTUALSVR_form.submit();
}
</script>
</head>
<body>
<form name="VIRTUALSVR_form" method="post" ACTION="/cgi-bin/adv_nat_virsvr.asp">
<INPUT TYPE="HIDDEN" NAME="isLocalPortSupport" value="<%if tcWebApi_get("WebCustom_Entry","isVirSvrLocalPortSupport","h") <> "N/A" then tcWebApi_get("WebCustom_Entry","isVirSvrLocalPortSupport","s") end if%>">
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&#12288;</td>
  </tr>
  <tr>
    <td width="150" height="30" class="title-main">
    
    Virtual Server 
    
    </td>
    <td width="10" class="black">&#12288;</td>
    <td width="150"></td>
    <td width="10" ></td>
    <td width="440"></td>
  </tr>
  <tr>
    <td class="light-orange">&#12288;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    
    Virtual Server for 
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
	<%If tcWebApi_get("Nat_PVC","IPMODE","h") = "Single" Then%>
    Single IPs Account/	PVC<%tcWebApi_get("WebCurSet_Entry","nat_pvc","s")%>
	<%Else%>
    Multiple IPs Account/ PVC<%tcWebApi_get("WebCurSet_Entry","nat_pvc","s")%>
	<%End If%>

<%else%>
	<%If tcWebApi_get("Nat_PVC","IPMODE","h") = "Single" Then%>
		<%if tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "0" then%>
			 Single IPs Account/ PVC0
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "1" then%>
			 Single IPs Account/ PVC1
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "2" then%>
			 Single IPs Account/ PVC2
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "3" then%>
			 Single IPs Account/ PVC3
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "4" then%>
			 Single IPs Account/ PVC4
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "5" then%>
			 Single IPs Account/ PVC5
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "6" then%>
			 Single IPs Account/ PVC6
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "7" then%>
			 Single IPs Account/ PVC7
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "8" then%>
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then%>
		    		Single IPs Account/ PTM0 Service ID 0
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then%>
		    		Single IPs Account/ PTM0 Service ID 1
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then%>
		    		Single IPs Account/ PTM0 Service ID 2
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then%>
		    		Single IPs Account/ PTM0 Service ID 3
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then%>
		    		Single IPs Account/ PTM0 Service ID 4
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then%>
		    		Single IPs Account/ PTM0 Service ID 5
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then%>
		    		Single IPs Account/ PTM0 Service ID 6
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then%>
		    		Single IPs Account/ PTM0 Service ID 7
				<%End if%>	
			<%else%>
		    	Single IPs Account/ PTM0
			<%End if%>	
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "9" then%>
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then%>
		    		Single IPs Account/ PTM1 Service ID 0
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then%>
		    		Single IPs Account/ PTM1 Service ID 1
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then%>
		    		Single IPs Account/ PTM1 Service ID 2
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then%>
		    		Single IPs Account/ PTM1 Service ID 3
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then%>
		    		Single IPs Account/ PTM1 Service ID 4
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then%>
		    		Single IPs Account/ PTM1 Service ID 5
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then%>
		    		Single IPs Account/ PTM1 Service ID 6
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then%>
		    		Single IPs Account/ PTM1 Service ID 7
				<%End if%>
			<%else%>
		    	Single IPs Account/ PTM1
			<%End if%>	
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "10" then%>
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then%>
		    		Single IPs Account/ WAN0 Service ID 0
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then%>
		    		Single IPs Account/ WAN0 Service ID 1
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then%>
		    		Single IPs Account/ WAN0 Service ID 2
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then%>
		    		Single IPs Account/ WAN0 Service ID 3
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then%>
		    		Single IPs Account/ WAN0 Service ID 4
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then%>
		    		Single IPs Account/ WAN0 Service ID 5
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then%>
		    		Single IPs Account/ WAN0 Service ID 6
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then%>
		    		Single IPs Account/ WAN0 Service ID 7
				<%End if%>	
			<%else%>
		    	Single IPs Account/ WAN0
			<%End if%>	
		<%end if%>

	<%else%>
		<%if tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "0" then%>
			 Multiple IPs Account/ PVC0
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "1" then%>
			 Multiple IPs Account/ PVC1
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "2" then%>
			 Multiple IPs Account/ PVC2
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "3" then%>
			 Multiple IPs Account/ PVC3
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "4" then%>
			 Multiple IPs Account/ PVC4
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "5" then%>
			 Multiple IPs Account/ PVC5
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "6" then%>
			 Multiple IPs Account/ PVC6
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "7" then%>
			 Multiple IPs Account/ PVC7
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "8" then%>
			 Multiple IPs Account/ PTM0
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "8" then%>
			 Multiple IPs Account/ PTM1
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "10" then%>
			 Multiple IPs Account/ WAN0
		<%end if%>
	<%end if%>

<%end if%>

    </td>
  </tr>
  <tr>
    <td class="light-orange">&#12288;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">    
    Start Port Number   
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">    	
        <input type="text" size="5" maxlength="5" name="start_port" value="<%if tcWebApi_get("VirServer_PVC_Entry","STARTPORT","h") <> "N/A" then tcWebApi_get("VirServer_PVC_Entry","STARTPORT","s") end if%>"  id="uiViewPvcVpi1">       
    </td>
  </tr>
  <tr>
    <td class="light-orange">&#12288;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">   
    End Port Number    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">  	
        <input type="text" size="5" maxlength="5" value="<%if tcWebApi_get("VirServer_PVC_Entry","ENDPORT","h") <> "N/A" then tcWebApi_get("VirServer_PVC_Entry","ENDPORT","s") end if%>" name="end_port" id="uiViewPvcVpi2">       
    </td>
  </tr>
  <tr>
    <td class="light-orange">&#12288;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    Local IP Address     
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">        
        <input type="text" class="uiTextInput" size="16" maxlength="15" name="Addr" id="uiViewIpAddressMark" value="<%if tcWebApi_get("VirServer_PVC_Entry","LOCALIP","h") <> "N/A" then tcWebApi_get("VirServer_PVC_Entry","LOCALIP","s") end if%>">        
  </tr>
<%If tcWebApi_get("WebCustom_Entry","isVirSvrLocalPortSupport","h") = "Yes" Then%>   
  <tr>
    <td class="light-orange">&#12288;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">    
    Start Port Number(Local)   
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">    	
        <input type="text" size="5" maxlength="5" name="local_sport" value="<%if tcWebApi_get("VirServer_PVC_Entry","LOCAL_SPORT","h") <> "N/A" then tcWebApi_get("VirServer_PVC_Entry","LOCAL_SPORT","s") end if%>" >       
    </td>
  </tr>
  <tr>
    <td class="light-orange">&#12288;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">   
    End Port Number(Local)
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">  	
        <input type="text" size="5" maxlength="5"  name="local_eport" value="<%if tcWebApi_get("VirServer_PVC_Entry","LOCAL_EPORT","h") <> "N/A" then tcWebApi_get("VirServer_PVC_Entry","LOCAL_EPORT","s") end if%>">       
    </td>
  </tr>
<%End If%>
</table>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td height="30" class="title-sub">    
    Virtual Server Listing     
    </td>
    <td class="light-orange">&#12288;</td>
    <td><hr noshade class="light-orange-line"></td>
    <td><hr noshade class="light-orange-line"></td>
    <td><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td width="150" class="light-orange">&#12288;</td>
    <td width="10" class="light-orange"></td>
    <td width="10">&#12288;</td>
    <td width="580" align="center">	
    </td>
    <INPUT TYPE="HIDDEN" NAME="editFlag" VALUE="0">
    <INPUT TYPE="HIDDEN" NAME="delFlag" VALUE="0">
    <INPUT TYPE="HIDDEN" NAME="editnum" VALUE="0">
    <td width="10">&#12288;</td>
  </tr>
  <tr>
    <td class="light-orange">&#12288;</td>
    <td class="light-orange"></td>
    <td>&#12288;</td>
    <td>&#12288;</td>
    <td width="10">&#12288;</td>
  </tr>
</table>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TR>
    <TD class=light-orange width=150>&nbsp;</TD>
    <TD class=light-orange width=10></TD>
    <TD class=tabdata>
		  <iframe src="/cgi-bin/virsvr_table.cgi" id="cgi_frame" frameborder="0" width="550" height="250"></iframe>
    </TD></TR></TBODY></TABLE>


<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&#12288;</td>
    <td width="160" class="orange">&#12288;</td>
    <td width="440" class="orange">
		<input type="button" name="SaveBtn"  value="APPLY"  onClick="doSave();">
		<input type="button" name="BackBtn"   value="BACK"   onClick="javascript:window.location='/cgi-bin/adv_nat_top.asp'">
		<input type="button" name="CancelBtn" value="CANCEL" onClick="javascript:window.location='/cgi-bin/adv_nat_virsvr.asp'">
		<INPUT TYPE="HIDDEN" NAME="ACLPort" VALUE="<%tcWebApi_get("Info_Service","acl_ports","s")%>">
		<INPUT TYPE="HIDDEN" NAME="AccoutAclSupported" VALUE="<%tcWebApi_get("WebCustom_Entry","isAccoutAclSupported","s")%>">
	</td>
  </tr>
</table>
<p>&#12288;</p>
</div>
</form>
</body>
</html>        
