<% 

If Request_Form("natFlag") = "1" Then
	If Request_Form("NAT_VCFlag") = "0" Then
		tcWebApi_set("WebCurSet_Entry","nat_pvc","NAT_VCIndex")
		tcWebApi_set("WebCurSet_Entry","wan_pvc","NAT_VCIndex")
		tcWebApi_set("Nat_PVC","IPMODE","radiobutton")
		tcWebApi_commit("Nat_PVC")
	elseif request_Form("NAT_VCFlag") = "1" Then
		tcWebApi_set("WebCurSet_Entry","nat_pvc","NAT_VCIndex")
		tcWebApi_set("WebCurSet_Entry","wan_pvc","NAT_VCIndex")

		If request_Form("service_num_flag") = "1" Then
			if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then
			if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then
				tcWebApi_set("WebCurSet_Entry","nat_pvc_ext","service_num")
				tcWebApi_set("WebCurSet_Entry","wan_pvc_ext", "service_num")
			end if
			end if
		End if	

	End If
End If	
if tcWebApi_get("WebCustom_Entry","isVPNSwitchGUI","h") = "Yes" then
 	If Request_Form("natFlag") = "2" Then
		tcWebApi_set("ALGSwitch_Entry","IPSECSW","VpnEnable")
		tcWebApi_set("ALGSwitch_Entry","L2TPSW","VpnEnable")
		tcWebApi_commit("ALGSwitch_Entry")
	End If
End If
%>		


<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script>
<% if tcWebApi_get("WebCustom_Entry","isVPNSwitchGUI","h") = "Yes" then %>
function doVPNChange() {
	document.NAT_form.natFlag.value = 2;
	document.NAT_form.submit();
	return;
}
<%End If%>

<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
function doServiceChange(){
	document.NAT_form.NAT_VCFlag.value = 1;
	document.NAT_form.service_num_flag.value = 1;
	document.NAT_form.submit();
	return;
}
<%End If%>

function doVCChange() {
	document.NAT_form.NAT_VCFlag.value = 1;
	document.NAT_form.submit();
	return;
}

function doNATLoad(value) {
    document.NAT_form.single_multi_flag.value = value;
    document.NAT_form.submit();
    return;
}
<% if tcWebApi_get("GUI_Entry0","portTriggering","h") = "support" Then %>  
function onClickPortTriggeringSummary() {
	window.open("/cgi-bin/adv_nat_porttriggering_list.asp","Port_Triggering_List","toolbar=no,menubar=no,scrollbars=no,height=400, width=650,location=0,left=150,top=150");
	return false;
}
<%End If%>
</script>
</head>
<body>

<form name="NAT_form" method="post">
<INPUT TYPE="HIDDEN" NAME="natFlag" VALUE="1">
<INPUT TYPE="HIDDEN" NAME="service_num_flag" VALUE="0">
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
    <td width="150" height="30" class="title-main">   
    NAT
    </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="150"></td>
    <td width="10" ></td>
    <td width="440"></td>
  </tr>
  <% if tcWebApi_get("WebCustom_Entry","isVPNSwitchGUI","h") = "Yes" then %>
  <tr>
    <TD class=light-orange>&nbsp;</TD>
    <TD class=light-orange></TD>
    <TD class=tabdata><DIV align=right>VPN Switch</DIV></TD>
    <TD class=tabdata>
      <DIV align=center>:</DIV></TD>
    <TD class=tabdata>
    <INPUT type="radio" value="on" name="VpnEnable" onClick="doVPNChange()" 
	<% If tcWebApi_get("ALGSwitch_Entry","IPSECSW","h") = "on" then%>		
	<% If tcWebApi_get("ALGSwitch_Entry","L2TPSW","h") = "on" then %>
	<% asp_Write("checked") end if%>
	<%end if%>> 
    	Enabled
    <INPUT type="radio" value="off" name="VpnEnable" onClick="doVPNChange()" 
	<% If tcWebApi_get("ALGSwitch_Entry","IPSECSW","h") = "off" then%>		
	<% asp_Write("checked") %>
	<% elseif tcWebApi_get("ALGSwitch_Entry","L2TPSW","h") = "off" then%> 
	<% asp_Write("checked") end if%>> 
	Disabled
     </TD></tr>
  <%End If%>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>Interface<%else%>Virtual Circuit<%end if%>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        
    <select NAME="NAT_VCIndex" onChange="doVCChange()">

<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
		<OPTION value="0" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "0" then asp_Write("selected") end if%>>PVC0
		<OPTION value="1" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "1" then asp_Write("selected") end if%>>PVC1
		<OPTION value="2" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "2" then asp_Write("selected") end if%>>PVC2
		<OPTION value="3" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "3" then asp_Write("selected") end if%>>PVC3
		<OPTION value="4" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "4" then asp_Write("selected") end if%>>PVC4
		<OPTION value="5" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "5" then asp_Write("selected") end if%>>PVC5
		<OPTION value="6" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "6" then asp_Write("selected") end if%>>PVC6
		<OPTION value="7" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "7" then asp_Write("selected") end if%>>PVC7

<%else%>
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
		<OPTION value="0" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "0" then asp_Write("selected") end if%>>PVC0
		<OPTION value="1" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "1" then asp_Write("selected") end if%>>PVC1
		<OPTION value="2" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "2" then asp_Write("selected") end if%>>PVC2
		<OPTION value="3" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "3" then asp_Write("selected") end if%>>PVC3
		<OPTION value="4" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "4" then asp_Write("selected") end if%>>PVC4
		<OPTION value="5" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "5" then asp_Write("selected") end if%>>PVC5
		<OPTION value="6" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "6" then asp_Write("selected") end if%>>PVC6
		<OPTION value="7" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "7" then asp_Write("selected") end if%>>PVC7
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
		<OPTION value="8" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "8" then asp_Write("selected") end if%>>PTM0
		<OPTION value="9" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "9" then asp_Write("selected") end if%>>PTM1
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
		<OPTION value="10" <%if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "10" then asp_Write("selected") end if%>>WAN0
	<%end if%>

<%end if%>
		
		</SELECT>
	
	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
	<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
	<%if tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "8" then%>
		<tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF">MULTI SERVICE</font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"><hr noshade></td>
			<td width="10" ><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Service Num</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">

				<SELECT NAME="service_num" SIZE="1" onChange="doServiceChange()">
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
				</SELECT>&nbsp;
	<%end if%>

	<%if tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "9" then%>
		<tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF">MULTI SERVICE</font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"><hr noshade></td>
			<td width="10" ><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Service Num</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">

				<SELECT NAME="service_num" SIZE="1" onChange="doServiceChange()">
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
				</SELECT>&nbsp;
	<%end if%>

	<%if tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "10" then%>
		<tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF">MULTI SERVICE</font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"><hr noshade></td>
			<td width="10" ><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Service Num</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">

				<SELECT NAME="service_num" SIZE="1" onChange="doServiceChange()">
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
					<option <% if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
				</SELECT>&nbsp;

	<%end if%>
	<%end if%>
	<%end if%>

		<INPUT TYPE="HIDDEN" NAME="NAT_VCFlag" VALUE="0">	
    </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">    
    NAT Status
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">	
		<% If tcWebApi_get("Wan_PVC","NATENABLE","h") <> "N/A" then tcWebApi_get("Wan_PVC","NATENABLE","s") else asp_Write("Deactivated") end if%>    
	</td>
  </tr>

<% If tcWebApi_get("Wan_PVC","NATENABLE","h") = "Enable" then%>
<% if tcWebApi_get("WebCustom_Entry","isMultiNatSupported","h") = "Yes" then %>
<% If tcWebApi_get("Nat_Common","MultiNat","h") = "1" then%>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">     
    Number of IPs   
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <input name="radiobutton" type="radio" value="Single" <%If tcWebApi_get("Nat_PVC","IPMODE","h") = "Single" then asp_Write("checked") end if%> onClick="doNATLoad(1)">Single        
		<input name="radiobutton" type="radio" value="Multiple" <%If tcWebApi_get("Nat_PVC","IPMODE","h") = "Multiple" then asp_Write("checked") end if%> onClick="doNATLoad(2)">Multiple
		<input type="hidden" name="single_multi_flag" value="0">	
    </td>
  </tr>
<%End if%>
<% end if %>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td height="30"><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content"> <a href="/cgi-bin/adv_nat_dmz.asp" target="main">   
    DMZ   
    </a></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td height="30"><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content" ><a href="/cgi-bin/adv_nat_virsvr.asp" target="main">    
    Virtual Server    
    </a></td>
  </tr>
 	<% if tcWebApi_get("GUI_Entry0","portTriggering","h") = "support" Then %>  
	<tr>
		<td class="light-orange">&#12288;</td>
		<td class="light-orange"></td>
		<td height="30"><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
		<td>&#12288;</td>
		<td class="content" >
			<a href="/cgi-bin/adv_nat_porttriggering.asp"  target="main">
			Port Triggering
			&nbsp;
			</a>
			<INPUT onclick=onClickPortTriggeringSummary(); type=button value="Port Triggering Summary" name=PortTriggeringSummary>
		</td>
	</tr>
	<%End If%>

 <% if tcWebApi_get("Nat_PVC","IPMODE","h") = "Multiple" Then %>  

  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td height="30"><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
    <td>&nbsp;</td>
    <td class="content" ><a href="/cgi-bin/adv_nat_ipaddrmap.asp"  target="main">
    
    IP Address Mapping (for Multiple IPs Service)
    
    </a></td>
  </tr>
	<%End If%>  

<%End If%>


	<% if tcWebApi_get("GUI_Entry0","natSessionsPerIP","h") = "support" Then %>  
	<tr>
		<td class="light-orange">&#12288;</td>
		<td class="light-orange"></td>
		<td height="30"><div align="right"><img src="/next.gif" width="16" height="16"></div></td>
		<td>&#12288;</td>
		<td class="content" >
			<a href="/cgi-bin/adv_nat_session.asp"  target="main">
			NAT Sessions
			</a>
		</td>
	</tr>
	<%End If%>
	
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td class="tabdata">&nbsp;</td>
  </tr>
</table>

<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
	</td>
  </tr>
</table>
</div>
</form>
</body>
</html>        
