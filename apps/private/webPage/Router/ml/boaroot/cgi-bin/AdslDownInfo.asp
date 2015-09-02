<html><head>
<title></title>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=
<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>
">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript">
</script>
</head>

<body topmargin="0" leftmargin="0">
	<table width="700" border="0" align="center" cellpadding="0" cellspacing="0">
		<tr>
			<td width="700" height="5" class="orange"></td>
		</tr>
	</table>

	<table border="0" width="700" align="center" cellspacing="0" cellpadding="0">
		<tr>
			<td width="200" height="50" align="center" valign="middle" bgcolor="#FFFFFF"><div align="left"><img src="/logo.gif" width="200" height="50"></div></td>
			<td width="500" height="50" align="right" valign="bottom" bgcolor="#FFFFFF" class="model">
			<font size="+1">
			<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			<%tcWebApi_get("String_Entry","AdslVdslInfoText","s")%>/<%end if%>
			<%tcWebApi_get("String_Entry","AdslInfoText","s")%>
			</font>			
			</td>
		</tr>
	</table>
		
  	<table width="700" border="0" align="center" cellpadding="2" cellspacing="0">
		<tr> 		  		
			<td width="700" align="center" class="orange" height="60" > 
			<font size="+3">			
			<%tcWebApi_get("String_Entry","AdslInfoDeviceText","s")%>
			</font>
			</td>
		</tr>
  	</table>
	
	<table width="700" height="300" border="0" align="center" cellpadding="0" cellspacing="0" color="#FF9933">
	  <tr>				
		<td width="700"   align="center" class="headline">&nbsp; 
		<%tcWebApi_get("String_Entry","AdslInfoCommentText","s")%>
		</td>
	  </tr>
	</table>

  	<table width="700" height="40" border="0" align="center" cellpadding="0" cellspacing="0" class="orange">
		<tr>
			<td class="orange">
				<div align="right">
				</div>
			</td>			
		</tr>
	</table>
</body>
</html>        
