<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">

<style>
<!--
 p.MsoNormal
	{mso-style-parent:"";
	margin-bottom:.0001pt;
	font-size:12.0pt;
	font-family:"Times New Roman";
	color:black; margin-left:0cm; margin-right:0cm; margin-top:0cm}
span.help-text1
	{font-family:Arial;
	color:black}
h1
	{margin-bottom:.0001pt;
	page-break-after:avoid;
	font-size:9.0pt;
	font-family:Arial;
	color:blue;
	margin-left:0cm; margin-right:0cm; margin-top:0cm}
-->
</style>


<body text=#000000 bgcolor=#ffffff>
<table cellspacing=0 cellpadding=0 width=750 border=0>
<tbody>
  <tr>
    <td height="50" class="orange"><span class="help-header">&nbsp;
    
    <%tcWebApi_get("String_Entry","HelpIndexAdvSetupText","s")%>
    
    </span></td>
  </tr>
  <tr>
    <td>&nbsp;</td></tr>
                                                                                 
  <tr>
    <td>
      <span class="help-main"><a name="Firewall">
      
      <%tcWebApi_get("String_Entry","HelpIndexFirewallText","s")%>
      
      </a></span><br>
      <span class="help-text">
      
	    <%tcWebApi_get("String_Entry","HelpAdvTip0Text","s")%>
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                        
                                                                             
  <tr>
    <td>
      <span class="help-main"><a name="Routing">
      
      <%tcWebApi_get("String_Entry","HelpIndexRoutingText","s")%>
      
      </a></span><br>
      <span class="help-text">
      
	    <%tcWebApi_get("String_Entry","HelpAdvTip1Text","s")%>
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      <%tcWebApi_get("String_Entry","HelpAdvTip2Text","s")%>
      
      </span><br>      
      <span class="help-text">
      <%tcWebApi_get("String_Entry","HelpAdvTip3Text","s")%>
      <%tcWebApi_get("String_Entry","HelpAdvTip4Text","s")%>  
	  <%tcWebApi_get("String_Entry","HelpAdvTip5Text","s")%>
	  </span>	</td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                      
                                                                                             
  <tr>
    <td>
      <span class="help-main"><a name="NAT">
      
      <%tcWebApi_get("String_Entry","HelpIndexNATText","s")%>
      
      </a></span><br>
      <span class="help-text">
      <%tcWebApi_get("String_Entry","HelpAdvTip6Text","s")%>
         <%tcWebApi_get("String_Entry","HelpAdvTip7Text","s")%>
	  	
      </span> 
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      NAT :: DMZ
      
      </span><br>
      <span class="help-text">
      <%tcWebApi_get("String_Entry","HelpAdvTip8Text","s")%>
	    
	  	
      </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      <%tcWebApi_get("String_Entry","HelpAdvTip9Text","s")%>
      
      </span><br>
	  <span class="help-text">
	  <%tcWebApi_get("String_Entry","HelpAdvTip10Text","s")%>
       <%tcWebApi_get("String_Entry","HelpAdvTip11Text","s")%> 
	  
        </span>
     </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td bgcolor="#FFFFFF">
      <span class="help-sub">
      
      <%tcWebApi_get("String_Entry","HelpAdvTip12Text","s")%>
      
      </span><br>
      <span class="help-text">
        <%tcWebApi_get("String_Entry","HelpAdvTip13Text","s")%>
		<%tcWebApi_get("String_Entry","HelpAdvTip14Text","s")%>
        		
	  	
	  </span>    </td>   
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                      
                        
  <tr>
    <td>
	<span class="help-main"><a name="QoS">
		<%tcWebApi_get("String_Entry","HelpIndexQoSText","s")%>	
	
	</span>
	<br>
	<span class="help-text">
<%tcWebApi_get("String_Entry","HelpAdvTip15Text","s")%>	

	
	</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
    <span class="help-sub">
    
    <%tcWebApi_get("String_Entry","HelpAdvTip16Text","s")%>
    
    </span><br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip17Text","s")%>
	<%tcWebApi_get("String_Entry","HelpAdvTip18Text","s")%>
	
	</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
	<span class="help-sub">
	
	<%tcWebApi_get("String_Entry","HelpAdvTip19Text","s")%>
	
	</span><br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip20Text","s")%>

	</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
	<span class="help-sub">
	
	<%tcWebApi_get("String_Entry","HelpAdvTip21Text","s")%>
	
	</span><br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip22Text","s")%>

	</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
	<span class="help-sub">
	
	<%tcWebApi_get("String_Entry","HelpAdvTip23Text","s")%>
	
	</span>
	<br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip24Text","s")%>

	</span></td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                        

  <tr>
    <td>
	<span class="help-main"><a name="VLAN">
	
	<%tcWebApi_get("String_Entry","HelpIndexVLANText","s")%>
	
	</span><br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip25Text","s")%>
	
	
	</span>
	<p class="MsoNormal" style="margin-left: 18.0pt">
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip26Text","s")%>
	
	
	</span></p>
	<p class="MsoNormal" style="margin-left: 18.0pt">
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip27Text","s")%>
	
	
	</span></p>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip28Text","s")%>
	<%tcWebApi_get("String_Entry","HelpAdvTip29Text","s")%>
	
	</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
    <span class="help-sub">
    <%tcWebApi_get("String_Entry","HelpAdvTip30Text","s")%>
    
    
    </span><br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip31Text","s")%>
	
	</span></td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>  
                          

  <tr>
    <td>
      <span class="help-main"><a name="ADSL">
      <%tcWebApi_get("String_Entry","HelpIndexADSLText","s")%>
      
      
      </a></span><br>
      <span class="help-text">
      <%tcWebApi_get("String_Entry","HelpAdvTip32Text","s")%>
	    
	  
	  </span>
    </td>
  </tr>  
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
    <span class="help-sub">
    <%tcWebApi_get("String_Entry","HelpAdvTip33Text","s")%>
    
    
    </span><br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip34Text","s")%>
	
	</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>   
  <tr>
    <td>
    <span class="help-sub">
    <%tcWebApi_get("String_Entry","HelpAdvTip35Text","s")%>
    
    </span><br>
	<span class="help-text">
	<%tcWebApi_get("String_Entry","HelpAdvTip36Text","s")%>
	 
	</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>       
                             
  </tbody>
</table>
</body></html>
