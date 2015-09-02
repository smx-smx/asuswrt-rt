<%
If Request_Form("SaveFlag") = "1" Then 
	TCWebApi_set("Ddns_Entry","Active","Enable_DyDNS")
	TCWebApi_set("Ddns_Entry","SERVERNAME","ddns_ServerName")
	TCWebApi_set("Ddns_Entry","MYHOST","sysDNSHost")
	TCWebApi_set("Ddns_Entry","USERNAME","sysDNSUser")
	TCWebApi_set("Ddns_Entry","PASSWORD","sysDNSPassword")
	TCWebApi_set("Ddns_Entry","WILDCARD","Enable_Wildcard")
  	tcWebApi_commit("Ddns_Entry")

End If

%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">

</head><script language="JavaScript">
function ddnsSwitch(on_off)
{
	if(on_off == 0)
		for(i = 2; i < 8; i++)
			document.DDNS_form.elements[i].disabled = true;
	else
		for(i = 2; i < 9; i++)
			document.DDNS_form.elements[i].disabled = false;
}

function checkSubmit()
{
	if(document.DDNS_form.elements[0].checked)
	{
		for(i = 3; i < 6; i++)
		{
			if(document.DDNS_form.elements[i].value.length == 0)
			{
				alert('<%tcWebApi_get("String_Entry","dyDNSJS0Text","s")%>');
				document.DDNS_form.elements[i].focus();
				return false;
			}
		}
		if(invalidCharCheck(document.DDNS_form.sysDNSHost) ) 
			return false;
		if(invalidCharCheck(document.DDNS_form.sysDNSUser) ) 
			return false;
		if(invalidCharCheck(document.DDNS_form.sysDNSPassword) ) 
			return false;
	}
	document.DDNS_form.SaveFlag.value = 1;
	document.DDNS_form.submit();
}
function invalidCharCheck(object)
{
	var len = object.value.length;
	var c;
	var i;
    for (i = 0; i < len; i++)
    {
	 	 var c = object.value.charAt(i);
      
	 	 if (c == '"' || c == ':' || c == '&' || c == '\'' || c == '(' || c== ')' || c==';' || c=='`' || c =='|' || c=='\\')
		 {
				alert('<%tcWebApi_get("String_Entry","dyDNSJS1Text","s")%>');      	  								    	    	   		
		 		return true;
		 }
    }
    
	return false;
}

</script>

<body onLoad="if(!document.DDNS_form.elements[0].checked) ddnsSwitch(0);">

<FORM METHOD="POST" ACTION="/cgi-bin/access_ddns.asp" name="DDNS_form"><div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF"><%tcWebApi_get("String_Entry","dyDNS1Text","s")%></font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","dyDNS1Text","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT NAME="Enable_DyDNS" TYPE="RADIO" onClick="ddnsSwitch(1)" VALUE="Yes" <% If TCWebApi_get("Ddns_Entry","Active","h") = "Yes" then asp_Write("checked") end if%> >		<font color="#000000"><%tcWebApi_get("String_Entry","dyDNSActivatedText","s")%></font>		
		<INPUT TYPE="RADIO" NAME="Enable_DyDNS" VALUE="No" <% If TCWebApi_get("Ddns_Entry","Active","h") = "No" then asp_Write("checked") end if%> onClick="ddnsSwitch(0)">		<font color="#000000"><%tcWebApi_get("String_Entry","dyDNSDeactivatedText","s")%></font>	</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","dyDNSServiceProviderText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<SELECT NAME="ddns_ServerName" SIZE="1">
 <option value="www.dyndns.org" <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.dyndns.org" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","dyDNSWebSiteText","s")%>
</SELECT>
</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","dyDNSHostNameText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
       <INPUT TYPE="TEXT" NAME="sysDNSHost" SIZE="32" MAXLENGTH="63" VALUE="<% If TCWebApi_get("Ddns_Entry","MYHOST","h") <> "N/A" then TCWebApi_get("Ddns_Entry","MYHOST","s") end if%>" >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","dyDNSUsernameText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT TYPE="TEXT" NAME="sysDNSUser" SIZE="32" MAXLENGTH="31" VALUE="<% If TCWebApi_get("Ddns_Entry","USERNAME","h") <> "N/A" then TCWebApi_get("Ddns_Entry","USERNAME","s") end if%>" >	</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","dyDNSPasswordText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
      <INPUT TYPE="PASSWORD" NAME="sysDNSPassword" SIZE="32" MAXLENGTH="31" VALUE="<% If TCWebApi_get("Ddns_Entry","PASSWORD","h") <> "N/A" then TCWebApi_get("Ddns_Entry","PASSWORD","s") end if%>" >	</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","dyDNSWildcardText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata"><input type="RADIO" name="Enable_Wildcard" value="Yes" <% If TCWebApi_get("Ddns_Entry","WILDCARD","h") = "Yes" then asp_Write("checked") end if%> >
      <font color="#000000"><%tcWebApi_get("String_Entry","dyDNSYesText","s")%></font>
        <INPUT TYPE="RADIO" NAME="Enable_Wildcard" VALUE="No" <% If TCWebApi_get("Ddns_Entry","WILDCARD","h") = "No" then asp_Write("checked") end if%> > <font color="#000000"><%tcWebApi_get("String_Entry","dyDNSNoText","s")%></font>	</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="SUBMIT" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick=" return checkSubmit()">
		<INPUT TYPE="HIDDEN" NAME="SaveFlag" VALUE="0">		</td></tr></table></div></form>
</body></html>
