<%
IF Request_Form("LineFlag")="1" Then
	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
End if



If request_Form("VoIP_media_flag")="1" then


	tcWebApi_set("WebCurSet_Entry","LineNum","Line")
	tcWebApi_set("VoIPMedia_Entry","VAD","VoIPVAD")
	tcWebApi_set("VoIPMedia_Entry","EchoCancellationEnable","VoIPEchoCancellation")
	tcWebApi_set("VoIPMedia_Entry","SilenceCompressionEnable","VoIPSilenceCompressionEnable")
	tcWebApi_set("VoIPMedia_Entry","CodecIndex","editnum")

	
	If request_Form("editnum")="0" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs0","VoIPSupportedCodecs")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime0","VoIPPacketizationTime")
	elseif request_Form("editnum")="1" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs1","VoIPSupportedCodecs")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime1","VoIPPacketizationTime")
	elseif request_Form("editnum")="2" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs2","VoIPSupportedCodecs")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime2","VoIPPacketizationTime")
	elseif request_Form("editnum")="3" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs3","VoIPSupportedCodecs")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime3","VoIPPacketizationTime")
	elseif request_Form("editnum")="4" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs4","VoIPSupportedCodecs")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime4","VoIPPacketizationTime")	
	end if

	tcWebApi_commit("VoIPMedia_Entry")

	
elseif request_Form("delFlag")="1" then
	tcWebApi_set("VoIPMedia_Entry","CodecIndex","editnum")

	If request_Form("editnum")="0" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs0","VoIPSupportedCodecsTmp")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime0","VoIPPacketizationTimeTmp")
	elseif request_Form("editnum")="1" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs1","VoIPSupportedCodecsTmp")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime1","VoIPPacketizationTimeTmp")
	elseif request_Form("editnum")="2" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs2","VoIPSupportedCodecsTmp")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime2","VoIPPacketizationTimeTmp")
	elseif request_Form("editnum")="3" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs3","VoIPSupportedCodecsTmp")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime3","VoIPPacketizationTimeTmp")
	elseif request_Form("editnum")="4" then
		tcWebApi_set("VoIPMedia_Entry","SIPSupportedCodecs4","VoIPSupportedCodecsTmp")
		tcWebApi_set("VoIPMedia_Entry","SIPPacketizationTime4","VoIPPacketizationTimeTmp")	
	end if


	tcWebApi_commit("VoIPMedia_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" type='text/javascript' src="/chksel.js"></script>
<script language="JavaScript">


function chkRepeatCodec(){
	var HaveSettingCodecs = new Array(4);

	<%If request_Form("Line")="1" then %>
		HaveSettingCodecs[0] = "<% TCWebAPI_get("VoIPMedia_Entry1", "SIPSupportedCodecs0", "s") %>";
		HaveSettingCodecs[1] = "<% TCWebAPI_get("VoIPMedia_Entry1", "SIPSupportedCodecs1", "s") %>";
		HaveSettingCodecs[2] = "<% TCWebAPI_get("VoIPMedia_Entry1", "SIPSupportedCodecs2", "s") %>";
		HaveSettingCodecs[3] = "<% TCWebAPI_get("VoIPMedia_Entry1", "SIPSupportedCodecs3", "s") %>";
		HaveSettingCodecs[4] = "<% TCWebAPI_get("VoIPMedia_Entry1", "SIPSupportedCodecs4", "s") %>";
	<%else %>

		HaveSettingCodecs[0] = "<% TCWebAPI_get("VoIPMedia_Entry0", "SIPSupportedCodecs0", "s") %>";
		HaveSettingCodecs[1] = "<% TCWebAPI_get("VoIPMedia_Entry0", "SIPSupportedCodecs1", "s") %>";
		HaveSettingCodecs[2] = "<% TCWebAPI_get("VoIPMedia_Entry0", "SIPSupportedCodecs2", "s") %>";
		HaveSettingCodecs[3] = "<% TCWebAPI_get("VoIPMedia_Entry0", "SIPSupportedCodecs3", "s") %>";
		HaveSettingCodecs[4] = "<% TCWebAPI_get("VoIPMedia_Entry0", "SIPSupportedCodecs4", "s") %>";

	<% end if %>
		
	var f=top.main.document.VOIP_MEDIA_form;
	var now=f.editnum.value;
	var codec=f.VoIPSupportedCodecs.options[f.VoIPSupportedCodecs.selectedIndex].value;
	for(i=0; i<5; i++){
		if(i == now){
			continue;
		}
		else{
			if(codec == HaveSettingCodecs[i]){
				alert("<%tcWebApi_get("String_Entry","VoIPMediaJS0Text","s")%>");
				return 1;
			}
		}
	}
	return 0;
}
function dosave(){
	var form=document.VOIP_MEDIA_form
	if(!chkRepeatCodec()){
		form.VoIP_media_flag.value=1;
		form.submit();
	}
}

function doLineChange(){
	document.VOIP_MEDIA_form.LineFlag.value = 1;
	document.VOIP_MEDIA_form.submit();
}
function add_packtime_option(i){
	var f=top.main.document.VOIP_MEDIA_form.VoIPPacketizationTime.options
	var additem;
	if(i=="G.723"){
		additem=new Option("30","30");
		f.add(additem); 
		additem=new Option("60","60");
		f.add(additem);
		additem=new Option("90","90");
		f.add(additem);
	}else{
		additem=new Option("10","10");
		f.add(additem); 
		additem=new Option("20","20");
		f.add(additem);
		additem=new Option("30","30");
		f.add(additem); 
		additem=new Option("40","40");
		f.add(additem);	
	}
}
function checksel(){
	var f=top.main.document.VOIP_MEDIA_form;
	var form=document.VOIP_MEDIA_form;

	switch(top.main.document.VOIP_MEDIA_form.editnum.value){
		case "0":
			TcCheckSel(f.VoIPSupportedCodecs, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "s")%>");
			add_packtime_option("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "s")%>");

			if( ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "s")%>" == "G.723") || ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime0", "s")%>" != "N/A") ){
				TcCheckSel(f.VoIPPacketizationTime, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime0", "s")%>");
			}
			else{
				f.VoIPPacketizationTime.selectedIndex=1;
			}

			top.main.document.getElementById('PriorityIndex').innerHTML = 1;
			break;
		case "1":
			TcCheckSel(f.VoIPSupportedCodecs, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "s")%>");
			add_packtime_option("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "s")%>");

			if( ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs1", "s")%>" == "G.723") || ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime1", "s")%>" != "N/A") ){
				TcCheckSel(f.VoIPPacketizationTime, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime1", "s")%>");
			}
			else{
				f.VoIPPacketizationTime.selectedIndex=1;
			}
	
			top.main.document.getElementById('PriorityIndex').innerHTML = 2;
			break;
		case "2":
			TcCheckSel(f.VoIPSupportedCodecs, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "s")%>");
			add_packtime_option("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "s")%>");

			if( ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs2", "s")%>" == "G.723") || ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime2", "s")%>" != "N/A") ){
				TcCheckSel(f.VoIPPacketizationTime, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime2", "s")%>");
			}
			else{
				f.VoIPPacketizationTime.selectedIndex=1;
			}
	
			top.main.document.getElementById('PriorityIndex').innerHTML = 3;
			break;
		case "3":
			TcCheckSel(f.VoIPSupportedCodecs, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "s")%>");
			add_packtime_option("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "s")%>");

			if( ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs3", "s")%>" == "G.723") || ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime3", "s")%>" != "N/A") ){
				TcCheckSel(f.VoIPPacketizationTime, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime3", "s")%>");
			}
			else{
				f.VoIPPacketizationTime.selectedIndex=1;
			}
	
			top.main.document.getElementById('PriorityIndex').innerHTML = 4;
			break;
		case "4":
			TcCheckSel(f.VoIPSupportedCodecs, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "s")%>");
			add_packtime_option("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "s")%>");

			if( ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs4", "s")%>" == "G.723") || ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime4", "s")%>" != "N/A") ){
				TcCheckSel(f.VoIPPacketizationTime, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime4", "s")%>");
			}
			else{
				f.VoIPPacketizationTime.selectedIndex=1;
			}
	
			top.main.document.getElementById('PriorityIndex').innerHTML = 5;
			break;
		default:
			TcCheckSel(f.VoIPSupportedCodecs, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "s")%>");
			add_packtime_option("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "s")%>");

			if( ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPSupportedCodecs0", "s")%>" == "G.723") || ("<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime0", "s")%>" != "N/A") ){
				TcCheckSel(f.VoIPPacketizationTime, "<% TCWebAPI_get("VoIPMedia_Entry", "SIPPacketizationTime0", "s")%>");
			}
			else{
				f.VoIPPacketizationTime.selectedIndex=1;
			}

			top.main.document.getElementById('PriorityIndex').innerHTML = 1;
			break;
	}


<% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") = "No" then%>
		
<% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") = "No" then%>			
		
	
	form.VoIPVAD[0].disabled=true;
	form.VoIPSilenceCompressionEnable[0].disabled=true;

	form.VoIPSupportedCodecs.disabled=true;
	form.VoIPPacketizationTime.disabled=true;
	


	form.VoIPVAD[1].disabled=true;
	form.VoIPSilenceCompressionEnable[1].disabled=true;

	form.Line.disabled=true;

	


	
<%end if%>
<%end if%>

}
function check_codec(){
	var f=top.main.document.VOIP_MEDIA_form;
	index = f.VoIPSupportedCodecs.selectedIndex;

	for(i=f.VoIPPacketizationTime.length;i>=0;i--){
		f.VoIPPacketizationTime.options.remove(i);
	}
	add_packtime_option(f.VoIPSupportedCodecs.options[index].value);	
	if( f.VoIPSupportedCodecs.options[index].value != "G.723" ){
		f.VoIPPacketizationTime.selectedIndex=1;
	}
}
</script>
</head><body onload="checksel()">
<FORM METHOD="POST" ACTION="/cgi-bin/voip_media.asp" name="VOIP_MEDIA_form"><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td></tr>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","VoIPMediaText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"></td><td width="440"></td></tr>	
		<INPUT TYPE="HIDDEN" NAME="LineFlag" VALUE="0">


	

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaLineText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<select NAME="Line" SIZE="1" onChange="doLineChange()" >

		<OPTION value="0" <%if TCWebApi_get("WebCurSet_Entry","LineNum","h") = "0" then asp_write("selected") end if%> <% if TCWebAPI_get("VoIPBasic_Entry0", "SIPEnable", "h") <> "Yes" then%> disabled=true <%end if%>>1
<% if TCWebAPI_get("VoIPBasic_Common", "VoIPLine2Enable", "h") = "Yes" then%>
		<OPTION value="1" <%if TCWebApi_get("WebCurSet_Entry","LineNum","h") = "1" then asp_write("selected") end if%> <% if TCWebAPI_get("VoIPBasic_Entry1", "SIPEnable", "h") <> "Yes" then%> disabled=true <%end if%>>2
<%end if%>


</select>


		</td>
	</tr>



	<tr>
		<td class="title-sub"><font color="#000000">&nbsp;</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td></tr>






	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaVADText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPVAD" VALUE="Yes" <%if TCWebAPI_get("VoIPMedia_Entry", "VAD", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaVAD0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPVAD" VALUE="No" <%if TCWebAPI_get("VoIPMedia_Entry", "VAD", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaVAD1Text","s")%></font>
		</td></tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaSilenceCompText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="VoIPSilenceCompressionEnable" VALUE="Yes" <%if TCWebAPI_get("VoIPMedia_Entry", "SilenceCompressionEnable", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaVAD0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPSilenceCompressionEnable" VALUE="No" <%if TCWebAPI_get("VoIPMedia_Entry", "SilenceCompressionEnable", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaVAD1Text","s")%></font>
		</td></tr>		
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaCancellationText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata">	
			<INPUT TYPE="RADIO" NAME="VoIPEchoCancellation" VALUE="Yes" <%if TCWebAPI_get("VoIPMedia_Entry", "EchoCancellationEnable", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaVAD0Text","s")%></font>
			<INPUT TYPE="RADIO" NAME="VoIPEchoCancellation" VALUE="No" <%if TCWebAPI_get("VoIPMedia_Entry", "EchoCancellationEnable", "h") <> "Yes" then asp_write("checked") end if%>><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaVAD1Text","s")%></font>
		</td></tr>	
<% end if %>
				
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","VoIPMediaCODECSetText","s")%></font></td>
		<td width="10" class="black">&nbsp;</td><td width="150"><hr noshade class="light-orange-line"></td><td width="10"><hr noshade class="light-orange-line"></td><td width="440"><hr noshade class="light-orange-line"></td></tr>
	<tr>	


	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<INPUT TYPE="HIDDEN" NAME="VoIPSupportedCodecsTmp" VALUE="N/A">
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaPriorityIndexText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<td class="tabdata" id="PriorityIndex"><%tcWebApi_get("VoIPMedia_Entry","CodecIndex", "s")%>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodecText","s")%></font></td>

		<td class="tabdata" align=center>:</td>
		<td class="tabdata">
			<SELECT NAME="VoIPSupportedCodecs" SIZE="1" onchange="check_codec()">
				<OPTION value="G.711 A-law" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec0Text","s")%>
				<OPTION value="G.711 U-law" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec1Text","s")%>
				<OPTION value="G.722" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec3Text","s")%>
				<OPTION value="G.726 - 32" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec6Text","s")%>
				<OPTION value="G.729" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec2Text","s")%>
<% if TCWebAPI_get("VoIPMedia_Common", "VOIP_switch", "h") = "Yes" then%>
				<OPTION value="G.726 - 16" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec4Text","s")%>		
				<OPTION value="G.726 - 24" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec5Text","s")%>			
				<OPTION value="G.726 - 40" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec7Text","s")%>
				<OPTION value="G.723" ><%tcWebApi_get("String_Entry","VoIPMediaSupportedCodec8Text","s")%>

				
<% end if %>
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata" align=right><font color="#000000"><%tcWebApi_get("String_Entry","VoIPMediaPacketizationText","s")%></font></td>
		<td class="tabdata" align=center>:</td>
		<INPUT TYPE="HIDDEN" NAME="VoIPPacketizationTimeTmp" VALUE="N/A">
		<td class="tabdata">
			<SELECT NAME="VoIPPacketizationTime" SIZE="1">
			</SELECT>
		</td></tr>
	<tr>
		<td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr></table>
	<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
	<TBODY>
	<TR>
		<TD class=light-orange width=150>&nbsp;</TD>
		<TD class=light-orange width=10></TD>

		<%If request_Form("Line")="1" then %>
			<TD class=tabdata><iframe src="/cgi-bin/voip_media2.cgi" id="cgi_frame" frameborder="0" width="550" height="250"></iframe></TD>	
		
		<%else %>
			<TD class=tabdata><iframe src="/cgi-bin/voip_media.cgi" id="cgi_frame" frameborder="0" width="550" height="250"></iframe></TD>		
		<% end if %>

		</TR></TBODY></TABLE>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  	<tr>
		<td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="VoIP_media_flag" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="editnum" VALUE="<%tcWebApi_get("WebCurSet_Entry","SIPIndexMedia","s")%>">
		<INPUT TYPE="HIDDEN" NAME="delFlag" VALUE="0">
		<INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="dosave();"></td></tr></table>
</form></body></html>
