<%
tcWebApi_set("WebCurSet_Entry","diag_pvc","Test_PVC")
tcWebApi_set("WebCurSet_Entry","diag_pvc_ext","Test_PVC_Ext")
TCWebApi_set("Diagnostic_PVC","PingOtherIPaddr","IP")
TCWebApi_set("Diagnostic_PVC","PingOtherType","pingtest_type")
If Request_Form("testFlag") = "1" then
tcWebApi_commit("Diagnostic_PVC")
elseif Request_Form("testFlag") = "2" then
tcWebApi_unset("Diagnostic_PVC")
tcWebApi_set("WebCurSet_Entry","dev_pvc","Test_PVC")
tcWebApi_set("WebCurSet_Entry","dev_pvc_ext","Test_PVC_Ext")
end if
%>


<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" href="/style.css" tppabs="http://192.168.1.1/css/style.css" type="text/css">
<script language="JavaScript" src="/general.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript">
	function DoRefresh() {
		document.Test_Form.testFlag.value = 2;
		document.Test_Form.submit();
	}
	
	function doTest(){
		if(document.Test_Form.pingtest_type[0].checked){
		    value = document.Test_Form.IP.value;
    		if(inValidIPAddr(value)){
        		return false;
        	}
		}    
		document.Test_Form.testFlag.value = 1;
		document.Test_Form.submit();
	}
	function doReload() {
	    if(document.Test_Form.pingtest_type[0].checked){
			document.Test_Form.pingtest_type.value = "Yes";
		}
		else{
			document.Test_Form.pingtest_type.value = "No";
		}
		document.Test_Form.submit();
		return;
    }
</script>
</head><body><FORM METHOD="POST" ACTION="/cgi-bin/tools_test.asp" name="Test_Form">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF"><INPUT TYPE="HIDDEN" NAME="testFlag" VALUE="0">
	<tr>
		<td height="5" class="light-orange" colspan="6">&nbsp;</td>
	</tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","TestDiagnosticText","s")%></font>
		<td width="10" class="black">&nbsp;</td>
		<td colspan="2"></td>
		<td width="60"> </td>
		<td width="215"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td width="50" >&nbsp;</td>
		<td width="275"  class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","TestCircuitText","s")%></font>
			<SELECT NAME="Test_PVC" SIZE="1" onChange="DoRefresh()">
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
				<OPTION value="0" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC0Text","s")%>
				<OPTION value="1" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC1Text","s")%>
				<OPTION value="2" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC2Text","s")%>
				<OPTION value="3" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "3" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC3Text","s")%>
				<OPTION value="4" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "4" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC4Text","s")%>
				<OPTION value="5" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "5" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC5Text","s")%>
				<OPTION value="6" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "6" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC6Text","s")%>
				<OPTION value="7" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "7" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC7Text","s")%>
<%else%>
	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
				<OPTION value="0" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC0Text","s")%>
				<OPTION value="1" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC1Text","s")%>
				<OPTION value="2" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC2Text","s")%>
				<OPTION value="3" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "3" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC3Text","s")%>
				<OPTION value="4" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "4" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC4Text","s")%>
				<OPTION value="5" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "5" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC5Text","s")%>
				<OPTION value="6" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "6" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC6Text","s")%>
				<OPTION value="7" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "7" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","TestPVC7Text","s")%>
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
				<OPTION value="8" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "8" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC8Text","s")%>
				<OPTION value="9" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "9" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC9Text","s")%>
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
				<OPTION value="10" <% if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") = "10" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","StaticRoutePVC10Text","s")%>
	<%end if%>

<%end if%>
			</SELECT></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<%if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "8" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td width="50" >&nbsp;</td>
	<td width="275"  class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","TestServiceText","s")%></font>
		<SELECT NAME="Test_PVC_Ext" SIZE="1" onChange="DoRefresh()">
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
	</SELECT></td></tr></tr>
<%end if%>
<%if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "9" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td width="50" >&nbsp;</td>
	<td width="275"  class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","TestServiceText","s")%></font>
		<SELECT NAME="Test_PVC_Ext" SIZE="1" onChange="DoRefresh()">
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
	</SELECT></td></tr></tr>
<%end if%>
<%end if%>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
<%if tcWebApi_staticGet("WebCurSet_Entry","dev_pvc","h") = "10" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td width="50" >&nbsp;</td>
	<td width="275"  class="tabdata"><font color="#000000"><%tcWebApi_get("String_Entry","TestServiceText","s")%></font>
		<SELECT NAME="Test_PVC_Ext" SIZE="1" onChange="DoRefresh()">
		<OPTION value="0" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
		<OPTION value="1" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
		<OPTION value="2" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
		<OPTION value="3" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
		<OPTION value="4" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
		<OPTION value="5" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
		<OPTION value="6" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
		<OPTION value="7" <% if tcWebApi_staticGet("WebCurSet_Entry","diag_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
	</SELECT></td></tr></tr>
<%end if%>
<%end if%>
<%end if%>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td ><hr noshade class="light-orange-line"></td>
		<td ><hr noshade class="light-orange-line"></td>
		<td ><hr noshade class="light-orange-line"></td>
		<td ><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt; <font color="#000000"><%tcWebApi_get("String_Entry","TestconnectionText","s")%></font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strlanconn = "<%TCWebApi_get("Diagnostic_PVC","EtherLanConn","s")%>";
		if(strlanconn == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong><%tcWebApi_get("String_Entry","EtherLanConn1Text","s")%></strong></div>');
		else if(strlanconn == "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn0Text","s")%></strong></div>');
		else if(strlanconn == "Skipped")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn2Text","s")%></strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strlanconn+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
<%if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") <> "10" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt; <font color="#000000">Testing ADSL Synchronization</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var stradslsyn = "<%TCWebApi_get("Diagnostic_PVC","ADSLSyn","s")%>";
		if(stradslsyn == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong><%tcWebApi_get("String_Entry","EtherLanConn1Text","s")%></strong></div>');
		else if(stradslsyn == "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn0Text","s")%></strong></div>');
		else if(stradslsyn == "Skipped")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn2Text","s")%></strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +stradslsyn+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
<%if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") <> "8" then%>
<%if tcWebApi_get("WebCurSet_Entry","diag_pvc","h") <> "9" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Testing ATM OAM segment ping</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var stratmoa = "<%TCWebApi_get("Diagnostic_PVC","ATMOAMSeg","s")%>";
		if(stratmoa == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong><%tcWebApi_get("String_Entry","EtherLanConn1Text","s")%></strong></div>');
		else if(stratmoa == "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn0Text","s")%></strong></div>');
		else if(stratmoa == "Skipped")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn2Text","s")%></strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +stratmoa+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt; <font color="#000000">Testing ATM OAM end to end ping</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var stratmoaend = "<%TCWebApi_get("Diagnostic_PVC","ATMOAMEnd2End","s")%>";
		if(stratmoaend == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong><%tcWebApi_get("String_Entry","EtherLanConn1Text","s")%></strong></div>');
		else if(stratmoaend == "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn0Text","s")%></strong></div>');
		else if(stratmoaend == "Skipped")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn2Text","s")%></strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +stratmoaend+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
<%end if%>
<%end if%>
<%end if%>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Ping DHCP Primary DNS ( </font>
			<%If tcWebApi_get("Dproxy_Entry","type","h") = "1" Then tcWebApi_get("Dproxy_Entry","Primary_DNS","s") else tcWebApi_get("DeviceInfo_PVC","DNSIP","s") end if%><font color="#000000"> )</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strpingdns = "<%TCWebApi_get("Diagnostic_PVC","PingPriDNS","s")%>";
		if(strpingdns == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong><%tcWebApi_get("String_Entry","EtherLanConn1Text","s")%></strong></div>');
		else if(strpingdns == "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn0Text","s")%></strong></div>');
		else if(strpingdns == "Skipped")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn2Text","s")%></strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strpingdns+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Ping www.yahoo.com ( use DHCP Pri. or Sec. DNS )</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strpingyahoo = "<%TCWebApi_get("Diagnostic_PVC","PingYahoo","s")%>";
		if(strpingyahoo == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong><%tcWebApi_get("String_Entry","EtherLanConn1Text","s")%></strong></div>');
		else if(strpingyahoo == "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn0Text","s")%></strong></div>');
		else if(strpingyahoo == "Skipped")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn2Text","s")%></strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strpingyahoo+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;&gt;&gt;<font color="#000000">Ping other IP address :  </font>			
			<INPUT TYPE="RADIO" NAME="pingtest_type" VALUE="Yes" <% if tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "Yes" then asp_Write("checked")end if %> onClick="doReload()"> <font color="#000000">Yes</font>
			<INPUT TYPE="RADIO" NAME="pingtest_type" VALUE="No"  <% if tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "N/A" then asp_Write("checked") end if %> onClick="doReload()"> <font color="#000000">No</font></td>
		<td class="tabdata">
		<script language="JavaScript" type="text/JavaScript">
		var strpingother = "<%TCWebApi_get("Diagnostic_PVC","PingOther","s")%>";
		if(strpingother == "Fail")
			document.writeln('<div align=center><font face="Arial" color="red"><strong><%tcWebApi_get("String_Entry","EtherLanConn1Text","s")%></strong></div>');
		else if(strpingother == "PASS")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn0Text","s")%></strong></div>');
		else if(strpingother == "Skipped")
			document.writeln('<div align=center><font face="Arial" color="green"><strong><%tcWebApi_get("String_Entry","EtherLanConn2Text","s")%></strong></div>');
		else
			document.writeln('<div align=center><font face="Arial" color="green"><strong>' +strpingother+ '</strong></div>');
		</script>
		</td>
		<td>&nbsp;</td>
	</tr>	
	<% If tcWebApi_get("Diagnostic_PVC","PingOtherType","h") = "Yes" Then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td>&nbsp;</td>
		<td class="tabdata">&nbsp;&nbsp;<div align=center><font color="#000000"><%tcWebApi_get("String_Entry","TestIPText","s")%>  </font><INPUT TYPE="TEXT" NAME="IP" SIZE="15" MAXLENGTH="15" VALUE="<%tcWebApi_get("Diagnostic_PVC","PingOtherIPaddr","s")%>" ></td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>		
	<% End If %>
	<tr>
		<td colspan="2" class="light-orange" >&nbsp;</td>
		<!--<td class="light-orange"></td>-->
		<td colspan="4">&nbsp;</td>
		<!--<td width="60">&nbsp;</td><td width="215">&nbsp;</td>-->
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td>
		<td width="160" class="orange">&nbsp;</td>
		<td width="440" class="orange"><INPUT TYPE="SUBMIT" NAME="TestBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonStartText","s")%>" onClick="doTest();"></td>
	</tr>
</table></div></form>
</body></html>
