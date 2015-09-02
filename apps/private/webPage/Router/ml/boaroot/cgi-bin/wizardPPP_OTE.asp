<%
 If Request_Form("saveFlag") = "1" Then	
 	tcWebApi_set("Wan_PVC0","USERNAME","IntePPPNameSetupOte")
	tcWebApi_set("Wan_PVC0","PASSWORD","IntePPPPswSetupOte")
	tcWebApi_set("Wan_PVC0","ISP","wanType")
	tcWebApi_set("Wan_PVC0","ENCAP","IntePPPEncapOte")
	tcWebApi_commit("Wan_PVC0")
End If
%>
<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="javascript" src="/general.js"></script>
<style type="text/css">
</style>
<script>
function StringCheck(val)
{
    re = /^[^\s]+$/;
    if( re.test(val) )
        return true;
    else
        return false;
}
function IsStringEmpty(val)
{
	var sw;
    sw = /^\s*$/;
    if( val == "" || sw.test(val) )
        return true;
    else
        return false;
}

function quotationCheck(object, limit_len) {
	var len = object.value.length;
	var c;
	var i, j = 0;
    for (i = 0; i < len; i++)
    {
	 	var c = object.value.charAt(i);
      
	  	if (c == '"')
		{
			j += 6;
		}
		else
			j++;
    }
   	if (j > limit_len)
	{
   		alert('<% tcWebApi_get("String_Entry","WirelessJS17Text","s") %>');
		return true;
	}	
	return false;
}
function uiSave() {
	if(chineseCheck(document.WizardPPPOteform.IntePPPNameSetupOte, "WizPPPUsernameId"))
	{
		return false;
	}
	if(chineseCheck(document.WizardPPPOteform.IntePPPPswSetupOte, "WizPPPPasswordId"))
	{
		return false;
	}
	if (quotationCheck(document.WizardPPPOteform.IntePPPNameSetupOte, 64) )
	{
		return false;
	}
	if ((!IsStringEmpty(document.WizardPPPOteform.IntePPPNameSetupOte.value))&& 
		(!StringCheck(document.WizardPPPOteform.IntePPPNameSetupOte.value)))
	{                                
		 alert("<% tcWebApi_get("String_Entry","WizardISPuspwJS1Text","s") %>");
		return false;                
	}
	if (quotationCheck(document.WizardPPPOteform.IntePPPPswSetupOte, 64) )
	{
		return false;
	}
	if ((!IsStringEmpty(document.WizardPPPOteform.IntePPPPswSetupOte.value))&& 
		(!StringCheck(document.WizardPPPOteform.IntePPPPswSetupOte.value)))
	{
		alert("<% tcWebApi_get("String_Entry","WizardISPuspwJS3Text","s") %>");
		return false;                
	}
	document.WizardPPPOteform.saveFlag.value="1";
	document.WizardPPPOteform.submit();
}

</script>
</head>
<body>
<form name="WizardPPPOteform" method="post">
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF" class="tabdata">
  <tr>
 	<td height="5" class="light-orange" colspan="5">&nbsp;
 	<INPUT TYPE="HIDDEN" NAME="wanType" VALUE="2">
 	<INPUT TYPE="HIDDEN" NAME="saveFlag" VALUE="0">
 	</td>
  </tr>
  <tr>
    <td width="150" height="30" class="title-main">
    <% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoText","s") %>
    </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="170"><hr noshade></td>
    <td width="10" ><hr noshade></td>
    <td width="420"><hr noshade></td>
  </tr>
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc0Text","s") %> 
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" colspan="3">
		<%if tcWebApi_Get("Info_Adsl","lineState","h")="up" then%>
		<% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc6Text","s") %>
		<%else%>
		<% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc7Text","s") %>
		<%end if%>
	  </td>
  </tr>
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc1Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
     <td class="tabdata" colspan="3">
	<%tcWebApi_staticGet("Info_Adsl","DataRateDown","s")%>
    	/
	<%tcWebApi_staticGet("Info_Adsl","DataRateUp","s")%>
    </td>  
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc2Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" colspan="3">
	<%tcWebApi_staticGet("Info_Adsl","SNRMarginDown","s")%>
    	/
	<%tcWebApi_staticGet("Info_Adsl","SNRMarginUp","s")%>
    </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc3Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
     <td class="tabdata" colspan="3">
	<%tcWebApi_staticGet("Info_Adsl","FECDown","s")%>
    	/
	<%tcWebApi_staticGet("Info_Adsl","FECUp","s")%>
    </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc4Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
     <td class="tabdata" colspan="3">
    	<%tcWebApi_staticGet("Info_Adsl","CRCDown","s")%>
    	/
	<%tcWebApi_staticGet("Info_Adsl","CRCUp","s")%>
    </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEAdslInfoc5Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" colspan="3">
    	<%tcWebApi_staticGet("Info_Adsl","HECDown","s")%>
    	/
	<%tcWebApi_staticGet("Info_Adsl","HECUp","s")%>
    </td>
  </tr>
</table>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF" class="tabdata">
  <tr>
    <td width="150" height="30" class="title-main">
    <% tcWebApi_get("String_Entry","WizardPPPOTEIPTVText","s") %>
    </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="170"><hr noshade></td>
    <td width="10" ><hr noshade></td>
    <td width="420"><hr noshade></td>
  </tr>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEIPTVc0Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" colspan="3">
    <%tcWebApi_Get("Wan_PVC1","VPI","s")%>
    /   
    <%tcWebApi_Get("Wan_PVC1","VCI","s")%>
    </td>
  </tr>
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEIPTVc1Text","s") %>
    </div></td>
    <td class="tabdata" ><div align="center">:</div></td>
    <td class="tabdata">
	<%If tcWebApi_Get("Info_Iptv","linkStatus","h") = "1" Then asp_Write("Connected") else asp_Write("Not Connected") end if%>
    </td>
  </tr>
    <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>	
    <td class="tabdata"><div align="right"> 
          <% tcWebApi_get("String_Entry","WizardPPPOTEIPTVc2Text","s") %> 
        </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" width="140">
	<%tcWebApi_staticGet("DeviceInfo_PVC1","connType","s")%>
    </td>
 </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEIPTVc3Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
	<%If tcWebApi_staticGet("Info_Iptv","linkStatus","h") = "1" Then tcWebApi_staticGet("Info_Iptv","linkUpTime","s") else asp_Write("N/A") end if%>
    </td>
  </tr>	                       		                     
</table>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" height="30" class="title-main">
    <% tcWebApi_get("String_Entry","WizardPPPOTENetConnText","s") %>
    </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="170"><hr noshade></td>
    <td width="10" ><hr noshade></td>
    <td width="420"><hr noshade></td>
  </tr>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEIPTVc0Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" colspan="3">
    <%tcWebApi_Get("Wan_PVC0","VPI","s")%>
    /
    <%tcWebApi_Get("Wan_PVC0","VCI","s")%>
    </td>
  </tr>
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTEIPTVc1Text","s") %>
    </div></td>
    <td class="tabdata" ><div align="center">:</div></td>
    <td class="tabdata" colspan="2">
	<%if tcWebApi_Get("DeviceInfo_PVC0","connType","h")="PPPoE" then
	      if tcWebApi_staticGet("DeviceInfo_PVC0","Status","h")="1" then
	  	  asp_Write("Connected")
	      else
		  If tcWebApi_staticGet("DeviceInfo_PVC0","IP6Status","h") = "1" Then asp_Write("Connected") else 
		  asp_Write("Not Connected")			
	          end if
	      end if
	  elseif tcWebApi_Get("DeviceInfo_PVC0","connType","h")="PPPoA" then
	      if tcWebApi_staticGet("DeviceInfo_PVC0","Status","h")="1" then
	  	  asp_Write("Connected")
	      else
		  If tcWebApi_staticGet("DeviceInfo_PVC0","IP6Status","h") = "1" Then asp_Write("Connected") else 
		  asp_Write("Not Connected")			
		  end if	
	      end if
	  else
		asp_Write("Not Connected")
	  end if
	%>
    </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTENetConnc0Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
	<%if tcWebApi_staticGet("DeviceInfo_PVC0","connType","h")="PPPoE" then
	      if tcWebApi_staticGet("DeviceInfo_PVC0","Status","h")="1" then
		tcWebApi_staticGet("DeviceInfo_PVC0","pppConnTime","s")
	      else
		  If tcWebApi_staticGet("DeviceInfo_PVC0","IP6Status","h") = "1" Then tcWebApi_staticGet("DeviceInfo_PVC0","IP6pppConnTime","s") else
		  asp_Write("N/A")
		  end if
	      end if		
	elseif tcWebApi_Get("DeviceInfo_PVC0","connType","h")="PPPoA" then
	      if tcWebApi_staticGet("DeviceInfo_PVC0","Status","h")="1" then
		tcWebApi_staticGet("DeviceInfo_PVC0","pppConnTime","s")
	      else
		  If tcWebApi_staticGet("DeviceInfo_PVC0","IP6Status","h") = "1" Then tcWebApi_staticGet("DeviceInfo_PVC0","IP6pppConnTime","s") else
		  asp_Write("N/A")
		  end if
	      end if		
	else
		asp_Write("N/A")
	end if
	%>
	  </td>
  </tr>	 
    <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WizardPPPOTENetConnc1Text","s") %>
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" width="140">
     	<%if tcWebApi_staticGet("DeviceInfo_PVC0","connType","h")="PPPoE" then
		if tcWebApi_staticGet("DeviceInfo_PVC0","WanIP","h") <> "N/A" then	
			tcWebApi_staticGet("DeviceInfo_PVC0","WanIP","s")
		else%>
			<script language="JavaScript" type="text/JavaScript">
			var str_IP6 = "<%tcWebApi_get("DeviceInfo_PVC0","IP6WanIP","s")%>";
			if("N/A" != str_IP6){
				var str_ip6value = str_IP6;
				var vlen = str_IP6.indexOf('/');
				if(vlen != -1){
					str_ip6value = str_IP6.substring(0, vlen);
				}
				document.writeln(str_ip6value);
			}
			else{
				document.writeln(str_IP6);
			}
			</script>
		<%end if
	elseif tcWebApi_Get("DeviceInfo_PVC0","connType","h")="PPPoA" then
		if tcWebApi_staticGet("DeviceInfo_PVC0","WanIP","h") <> "N/A" then	
			tcWebApi_staticGet("DeviceInfo_PVC0","WanIP","s")
		else%>
			<script language="JavaScript" type="text/JavaScript">
			var str_IP6 = "<%tcWebApi_get("DeviceInfo_PVC0","IP6WanIP","s")%>";
			if("N/A" != str_IP6){
				var str_ip6value = str_IP6;
				var vlen = str_IP6.indexOf('/');
				if(vlen != -1){
					str_ip6value = str_IP6.substring(0, vlen);
				}
				document.writeln(str_ip6value);
			}
			else{
				document.writeln(str_IP6);
			}
			</script>
		<%end if
	else
		asp_Write("N/A")
	end if
	%>
	</td>
 </tr>                      		                     
</table>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" height="30" class="title-main">
    <% tcWebApi_get("String_Entry","WizardPPPOTENetConnPPPText","s") %>
    </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="170"><hr noshade></td>
    <td width="10" ><hr noshade></td>
    <td width="420"><hr noshade></td>
  </tr>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
	<td class="tabdata" align=right id="WizPPPUsernameId">
		<font color="#000000"><% tcWebApi_get("String_Entry","WANUsernameText","s") %>:</font>
	</td>
    <td class="tabdata"><div align="center">:</div></td>
	<td class="tabdata" align=left>
		<INPUT TYPE="TEXT" NAME="IntePPPNameSetupOte" SIZE="32" MAXLENGTH="64" VALUE="<%if tcWebApi_get("Wan_PVC0","USERNAME","h") <> "N/A" then tcWebApi_get("Wan_PVC0","USERNAME","s") end if%>">
	</td>
  </tr>
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
	<td class="tabdata" align=right id="WizPPPPasswordId">
	<font color="#000000"><% tcWebApi_get("String_Entry","WANPasswardText","s") %>:</font>
	</td>
    <td class="tabdata" ><div align="center">:</div></td>
	<td class="tabdata" align=left>
		<INPUT TYPE="PASSWORD" NAME="IntePPPPswSetupOte" SIZE="32" MAXLENGTH="64" VALUE="<%if tcWebApi_get("Wan_PVC0","PASSWORD","h" ) <> "N/A" then tcWebApi_get("Wan_PVC0","PASSWORD","s") end if%>">
	</td>
  </tr>
    <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    <% tcWebApi_get("String_Entry","WANEncapsulationText","s") %>:
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <select name="IntePPPEncapOte" size="1">
		<option value="PPPoE LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE LLC" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation4Text","s")%>
		<option value="PPPoE VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE VC-Mux" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation5Text","s")%>
		<option value="PPPoA LLC" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA LLC" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation6Text","s")%>
		<option value="PPPoA VC-Mux" <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA VC-Mux" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","WANEncapsulation7Text","s")%>
	</select>
	</td>
 </tr>
 <tr>
	<td height="20"></td><td></td><td></td><td></td><td></td>
</tr>	                       		                     
</table>
<table width="760" height="40" border="0" align=center cellpadding="0" class="orange">
<tr>
	<td class="orange">
		<div align=center>
		<INPUT TYPE="BUTTON" NAME="WizSetupPPPOteSaveBtn" VALUE="<% tcWebApi_get("String_Entry","ButtonApplyText","s") %>" class="tabdata" onClick="uiSave()">
		</div>
	</td>	
</tr>
</table>
</form>
</body>
</html>
