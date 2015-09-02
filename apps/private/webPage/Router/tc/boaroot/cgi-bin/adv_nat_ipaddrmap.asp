<%
If Request_Form("editFlag") = "1" then
	tcWebApi_set("WebCurSet_Entry","ipAddr_id","editnum")
	tcWebApi_unset("IpAddrMap_PVC_Entry")
  	tcWebApi_commit("IpAddrMap_PVC_Entry")
	tcWebApi_set("IpAddrMap_PVC_Entry","RULETYPE","MapEditType")
	tcWebApi_set("IpAddrMap_PVC_Entry","LOCALSTARTIP","IPADDRMAP_localstartIP")
	tcWebApi_set("IpAddrMap_PVC_Entry","LOCALENDIP","IPADDRMAP_localendIP")
	tcWebApi_set("IpAddrMap_PVC_Entry","PUBLICSTARTIP","IPADDRMAP_publicstartIP")
	tcWebApi_set("IpAddrMap_PVC_Entry","PUBLICENDIP","IPADDRMAP_publicendIP")
	tcWebApi_commit("WebCurSet_Entry")
	tcWebApi_commit("IpAddrMap_PVC_Entry")
elseif Request_Form("delFlag") = "1" then
  tcWebApi_set("WebCurSet_Entry","ipAddr_id","editnum")
  tcWebApi_unset("IpAddrMap_PVC_Entry")
  tcWebApi_commit("IpAddrMap_PVC_Entry")
end if  
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language='javascript'>

function chkNATtype(flag)
{
	var form=document.IPADDRMAP_form;
	var index = form.MapEditType.selectedIndex;

	if( flag == 0 )
	{
	    form.IPADDRMAP_Flag.value = 3;
	    form.submit();	
	    return;
	}	
	
	form.IPADDRMAP_localstartIP.disabled = false;			
	form.IPADDRMAP_localendIP.disabled = false;			
	form.IPADDRMAP_publicstartIP.disabled = false;
	form.IPADDRMAP_publicendIP.disabled = false;
	document.getElementById("localSIP_Div").style.display = "none";
	document.getElementById("localEIP_Div").style.display = "none";
	document.getElementById("publicSIP_Div").style.display = "none";				
	switch(index)
	{	
		case 0://One-to-One
			form.IPADDRMAP_localendIP.value = "0.0.0.0";						
			form.IPADDRMAP_publicendIP.value = "0.0.0.0";
			form.IPADDRMAP_localendIP.disabled = true;						
			form.IPADDRMAP_publicendIP.disabled = true;
			document.getElementById("publicSIP_Div").style.display = "";			
			break;
		case 1://Many-to-One
			form.IPADDRMAP_publicendIP.value = "0.0.0.0";
			form.IPADDRMAP_publicendIP.disabled = true;
			document.getElementById("localSIP_Div").style.display = "";
			document.getElementById("localEIP_Div").style.display = "";
			document.getElementById("publicSIP_Div").style.display = "";			
			break;
		case 2://Many-to-One (No) Overload
		case 3:
			document.getElementById("localSIP_Div").style.display = "";
			document.getElementById("localEIP_Div").style.display = "";		
			break;
		case 4://Server
			form.IPADDRMAP_localstartIP.value = "0.0.0.0";
			form.IPADDRMAP_localendIP.value = "0.0.0.0";
			form.IPADDRMAP_publicendIP.value = "0.0.0.0";
			form.IPADDRMAP_localstartIP.disabled = true;			
			form.IPADDRMAP_localendIP.disabled = true;			
			form.IPADDRMAP_publicendIP.disabled = true;
			document.getElementById("publicSIP_Div").style.display = "";	
			break;
	}
}

function isValidIpAddr(ip1,ip2,ip3,ip4) 
{
    if(ip1==0 || ip4==0 || ip4==255 || ip1==127)
        return false;
    return true;
}

function doValidateIP(Address, option) 
{
    var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
    var digits;
    var i;

	if((option == 1 && Address == "0.0.0.0") || (option == 2 && Address == "255.255.255.255"))
		return true;
	
    if(address == null) 
    { 
      	     
  	    alert("IP address is empty!!");
      
    	return false; 
    }
    digits = address[0].split(".");
    for(i=0; i < 4; i++) 
    {
        if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223) || (digits[i] == null))
        { 
      	               
          	alert("Invalid IP address: " + Address);
     
        	return false; 
        }
    }

    if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3],false)) 
    {
      	               
      	alert("Invalid IP address: " + Address);
     
    	return false;
	}

    return true;
}

function doValidateRange(startIP,endIP) 
{
    var staddress;
    var edaddress;
    var cnt;

    staddress=startIP.split(".");
    edaddress=endIP.split(".");
    for(cnt=0; cnt < 4; cnt++) 
    {
        if(Number(edaddress[cnt])<Number(staddress[cnt])) 
        {
      	                       
            alert("End IP address is less than Start IP address");
    
	        return false;
        }
    }
    return true;
}			


function checkIPAddress() 
{
	var form = document.IPADDRMAP_form;
	
	switch(form.MapEditType.selectedIndex)
	{
		case 0:
			if(!doValidateIP(form.IPADDRMAP_localstartIP.value, 0))
			{
				form.IPADDRMAP_localstartIP.focus();
				return true;
			}
			else if(!doValidateIP(form.IPADDRMAP_publicstartIP.value, 1))
			{
				form.IPADDRMAP_publicstartIP.focus();
				return true;
			}
			break;
		case 1:
			if(!doValidateIP(form.IPADDRMAP_localstartIP.value, 1))
			{
				form.IPADDRMAP_localstartIP.focus();
				return true;
			}
			else if(!doValidateIP(form.IPADDRMAP_localendIP.value, 2))
			{
				form.IPADDRMAP_localendIP.focus();
				return true;
			}
			else if(!doValidateIP(form.IPADDRMAP_publicstartIP.value, 1))
			{
				form.IPADDRMAP_publicstartIP.focus();
				return true;
			}
			if( !doValidateRange(form.IPADDRMAP_localstartIP.value,form.IPADDRMAP_localendIP.value) )
			{
				form.IPADDRMAP_localstartIP.focus();
				return true;
			}			

			break;
		case 2:
		case 3:
			if(!doValidateIP(form.IPADDRMAP_localstartIP.value, 1))
			{
				form.IPADDRMAP_localstartIP.focus();
				return true;
			}
			else if(!doValidateIP(form.IPADDRMAP_localendIP.value, 2))
			{
				form.IPADDRMAP_localendIP.focus();
				return true;
			}
			else if(!doValidateIP(form.IPADDRMAP_publicstartIP.value, 1))
			{
				form.IPADDRMAP_publicstartIP.focus();
				return true;
			}
			else if(!doValidateIP(form.IPADDRMAP_publicendIP.value, 0))
			{
				form.IPADDRMAP_publicendIP.focus();
				return true;
			}
			if( !doValidateRange(form.IPADDRMAP_localstartIP.value,form.IPADDRMAP_localendIP.value) )
			{
				form.IPADDRMAP_localstartIP.focus();
				return true;
			}			
			if( !doValidateRange(form.IPADDRMAP_publicstartIP.value,form.IPADDRMAP_publicendIP.value) )
			{
				form.IPADDRMAP_publicstartIP.focus();
				return true;
			}			
			if(form.MapEditType.selectedIndex == 3)
			{
				var Index = new Array(5);
				Index[0] = -1;
				var LocalStartIP = new Array(4);
				var LocalEndIP = new Array(4);
				var GlobalStartIP = new Array(4);
				var GlobalEndIP = new Array(4);
				var Constants = new Array(Math.pow(2, 24), Math.pow(2, 16), Math.pow(2, 8), Math.pow(2, 0));
				var IPs = new Array(LocalStartIP, LocalEndIP, GlobalStartIP, GlobalEndIP);
				var IPValues = new Array(0, 0, 0, 0);
				for(i = 1; i <= 4; i++)
				{
					var IP = form.elements[i].value;
					Index[1] = IP.indexOf('.', Index[0]);
					Index[2] = IP.indexOf('.', Index[1] + 1);
					Index[3] = IP.indexOf('.', Index[2] + 1);
					Index[4] = form.elements[i].value.length;
					for(j = 0; j < 4; j++)
					{
						var value;
						IPs[i-1][j] = IP.substring(Index[j] + 1, Index[j + 1]);
						value = parseInt(IPs[i-1][j]);
						IPValues[i-1] = IPValues[i-1] + value * Constants[j];
					}
				}
				if(IPValues[1] - IPValues[0] <= 0)
				{
      	                       				
					alert("Wrong local IP pool");
     					
					form.IPADDRMAP_localstartIP.focus();
					return true;
				}
				else if(IPValues[3] - IPValues[2] <= 0)
				{
      	                       								
					alert("Wrong global IP pool");
     										
					form.IPADDRMAP_publicstartIP.focus();
					return true;
				}
				else if(IPValues[1] - IPValues[0] != IPValues[3] - IPValues[2])
				{
      	            alert("The numbers of local IPs and global IPs are not the same");
     															
					form.IPADDRMAP_localstartIP.focus();
					return true;
				}
			}
			break;
		case 4:
			if(!doValidateIP(form.IPADDRMAP_publicstartIP.value, 1))
			{
				form.IPADDRMAP_publicstartIP.focus();
				return true;
			}
			break;
	}
	return false;
}

function doSave() {
	if (checkIPAddress())
		return;
	document.forms[0].IPADDRMAP_Flag.value = 1;	
	document.forms[0].editFlag.value = 1;
	document.forms[0].submit();
}

</script>
</head>
<body >
<form name="IPADDRMAP_form" method="post" ACTION="/cgi-bin/adv_nat_ipaddrmap.asp">
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="6">&nbsp;</td>
  </tr>
  <tr>
    <td width="150" height="30" class="title-main">
    
    IP Address Mapping    </td>
    <td class="black" width="10">&nbsp;</td>
    <td class="tabdata" width="150">&nbsp;</td>
    <td class="tabdata" width="10" >&nbsp;</td>
    <td class="tabdata" width="110">&nbsp;</td>
	<td class="tabdata" width="330">&nbsp;</td>
  </tr>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    
    Address Mapping Rule
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata" colspan="2">
		<%If tcWebApi_get("Nat_PVC","IPMODE","h") = "Single" Then%>
    	Single IPs Account/	PVC<%tcWebApi_get("WebCurSet_Entry","nat_pvc","s")%>
    <%Else%>
    	Multiple IPs Account/ PVC<%tcWebApi_get("WebCurSet_Entry","nat_pvc","s")%>
    <%End If%>	</td>
  </tr>
  <tr>
    <td class="light-orange"></td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    
    Rule Type
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
       
      <td class="tabdata" colspan="2"> <select onChange="chkNATtype(1)" id="uiViewFlanRIPState" name="MapEditType">
          <option value="One-to-One">One-to-One</option>
          <option value="Many-to-One">Many-to-One</option>
          <option value="Many-to-Many-Overload" >Many-to-Many-Overload</option>
          <option value="Many-to-Many-No-Overload">Many-to-Many-No-Overload</option>
          <option value="Server" >Server</option>
        </select></td>
    </tr>
  <tr>
    <td class="light-orange" width="150" >&nbsp;</td>
    <td class="light-orange" width="10" >&nbsp;</td>
    <td class="tabdata" width="150"><div align="right">
    
    Local Start IP 
    
    </div></td>
    <td class="tabdata" width="10"><div align="center">:</div></td>
      <td class="tabdata" width="110"><input type="text" class="uiTextInput" size="15" maxlength="15" name="IPADDRMAP_localstartIP" id="IPADDRMAP_localstartIP" value="0.0.0.0" ></td>
	<td class="tabdata" id="localSIP_Div"><div>(for all local IPs, enter 0.0.0.0 for Start IP)</div></td> 
  </tr>
  <tr>
    <td class="light-orange" width="150">&nbsp;</td>
    <td class="light-orange" width="10">&nbsp;</td>
    <td class="tabdata" width="150"><div align="right">
    
    Local End IP
    
    </div></td>
    <td class="tabdata" width="10"><div align="center">:</div></td>
      <td class="tabdata" width="110"><input type="text" class="uiTextInput" size="15" maxlength="15" name="IPADDRMAP_localendIP" id="IPADDRMAP_localendIP" value="0.0.0.0"></td>
	  <td class="tabdata" id="localEIP_Div"><div>(for all local IPs, enter 255.255.255.255 for End IP)</div></td> 
  </tr>
  <tr>
    <td class="light-orange" width="150">&nbsp;</td>
    <td class="light-orange" width="10">&nbsp;</td>
    <td class="tabdata" width="150"><div align="right">
    
    Public Start IP
    
    </div></td>
    <td class="tabdata" width="10"><div align="center">:</div></td>
    <td class="tabdata" width="110">
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="IPADDRMAP_publicstartIP" id="uiViewIpAddressMark" value="0.0.0.0">
	   </td> 
	   <td class="tabdata" id="publicSIP_Div"><div>(0.0.0.0 for Dynamic IP)</div></td>   
  </tr>
  <tr>
    <td class="light-orange" width="150">&nbsp;</td>
    <td class="light-orange" width="10">&nbsp;</td>
    <td class="tabdata" width="150"><div align="right">
    
    Public End IP 
    
    </div></td>
    <td class="tabdata" width="10"><div align="center">:</div></td>
    <td class="tabdata" width="110"> 
        <input type="text" class="uiTextInput" size="15" maxlength="15" name="IPADDRMAP_publicendIP" id="uiViewIpAddressMark" value="0.0.0.0">
      </td>
	  <td class="tabdata">&nbsp;</td>
  </tr>
</table>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="150" height="30" class="title-sub">
    Address Mapping List 
    </td>
    <td width="10" class="light-orange">&nbsp;</td>
    <td width="200"><hr noshade class="light-orange-line"></td>
    <td width="200"><hr noshade class="light-orange-line"></td>
	<INPUT TYPE="HIDDEN" NAME="editFlag" VALUE="0">
    <INPUT TYPE="HIDDEN" NAME="delFlag" VALUE="0">
    <INPUT TYPE="HIDDEN" NAME="editnum" VALUE="0">
    <td width="200"><hr noshade class="light-orange-line"></td>
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
		  <iframe src="/cgi-bin/ipaddr_table.cgi" frameborder="0" width="550" height="360" align=left></iframe>
    </TD></TR></TBODY></TABLE>
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
    <input type="hidden" name="IPADDRMAP_Flag">
		<input type="button" name="SaveBtn" value="APPLY" onClick="doSave();">
		<input type="button" name="BackBtn"   value="BACK"   onClick="javascript:window.location='/cgi-bin/adv_nat_top.asp'">
		<input type="button" name="CancelBtn" value="CANCEL" onClick="javascript:window.location='/cgi-bin/adv_nat_ipaddrmap.asp'">
	</td>
  </tr>
</table>
</form>
</body>
<script>
chkNATtype(1);
</script>
</html>        
