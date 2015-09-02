<%
If request_form("NoChange_or_Save") = "1" Then 
	tcWebApi_set("AppFilter_Entry","Activate","APPFilter_status")
	tcWebApi_set("AppFilter_Entry","Block_ICQ","ICQ_allowance")
	tcWebApi_set("AppFilter_Entry","Block_MSN","MSN_allowance")	
	tcWebApi_set("AppFilter_Entry","Block_YMSG","YMSG_allowance")	
	tcWebApi_set("AppFilter_Entry","Block_RTSP","RA_allowance")
	tcWebApi_commit("AppFilter_Entry")
End If
%>

<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">

<SCRIPT language=javascript>
function switchFilterType(object)
{
  var index = object.selectedIndex;
	switch(index)
	{
		case 0:
			window.location='access_ipfilter.asp';
			break;
		case 2:
			window.location='access_URLfilter.asp';
			break;
	}
}

</SCRIPT>

<META content="MSHTML 6.00.2900.3059" name=GENERATOR></HEAD>
<BODY>
<FORM name=APPFILTERform action="/cgi-bin/access_appfilter.asp" method=post>
<DIV align=center>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
  	<TD class=light-orange colSpan=5 height=5>&nbsp;</TD>
  </TR>
  <TR>
    	<TD class=title-main width=150 height=30><FONT color=#ffffff>Filter</FONT></TD>
    	<TD class=black width=10>&nbsp;</TD>
    	<TD width=150></TD>
    	<TD width=10></TD>
    	<TD width=440></TD>
  </TR>
  <TR>
    	<TD class=title-sub height=25><FONT color=#000000>Filter Type</FONT> </TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><HR class=light-orange-line noShade></TD>
    	<TD class=tabdata><HR class=light-orange-line noShade></TD>
    	<TD class=tabdata><HR class=light-orange-line noShade></TD>
  </TR>
  <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>Filter Type Selection</FONT></DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
		<SELECT onchange=switchFilterType(this) size=1 name=FILTERTYPE_index>
				<OPTION >IP / MAC Filter
				<OPTION SELECTED>Application Filter
				<OPTION>URL Filter
		</SELECT>
	</TD>
  </TR>
  <TR>
    	<TD class=title-sub height=25><FONT color=#000000>Application Filter  Editing</FONT> </TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><HR class=light-orange-line noShade></TD>
    	<TD class=tabdata><HR class=light-orange-line noShade></TD>
    	<TD class=tabdata><HR class=light-orange-line noShade></TD>
  </TR>
  <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>Application Filter</FONT> </DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Activate","h") = "1" then asp_Write("checked") end if %>   value=1 name=APPFilter_status> <FONT  color=#000000>Activated</FONT>
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Activate","h") = "0" then asp_Write("checked") end if 
    							 if tcWebApi_get("AppFilter_Entry","Activate","h") = "N/A" then asp_Write("checked") end if %> value=0 name=APPFilter_status> <FONT color=#000000>Deactivated</FONT>
    	</TD>
  </TR>
  <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>ICQ</FONT> </DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_ICQ","h") = "0" then asp_Write("checked") end if %> value=0 name=ICQ_allowance> <FONT color=#000000>Allow</FONT>
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_ICQ","h") = "1" then asp_Write("checked") end if 
    							 if tcWebApi_get("AppFilter_Entry","Block_ICQ","h") = "N/A" then asp_Write("checked") end if %> value=1 name=ICQ_allowance> <FONT color=#000000>Deny</FONT> 
    	</TD>
  </TR>
  <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>MSN</FONT> </DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_MSN","h") = "0" then asp_Write("checked") end if %> value=0 name=MSN_allowance> <FONT color=#000000>Allow</FONT> 
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_MSN","h") = "1" then asp_Write("checked") end if 
    							 if tcWebApi_get("AppFilter_Entry","Block_MSN","h") = "N/A" then asp_Write("checked") end if %> value=1 name=MSN_allowance> <FONT color=#000000>Deny</FONT>
    	</TD>
  </TR>
  <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>YMSG</FONT> </DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_YMSG","h") = "0" then asp_Write("checked") end if %> value=0 name=YMSG_allowance> <FONT color=#000000>Allow</FONT> 
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_YMSG","h") = "1" then asp_Write("checked") end if 
    							 if tcWebApi_get("AppFilter_Entry","Block_YMSG","h") = "N/A" then asp_Write("checked") end if %> value=1 name=YMSG_allowance> <FONT color=#000000>Deny</FONT> 
    	</TD>
  </TR>
  <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>Real Audio/Video(RTSP)</FONT> </DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_RTSP","h") = "0" then asp_Write("checked") end if %> value=0 name=RA_allowance> <FONT color=#000000>Allow</FONT> 
    		<INPUT type=radio <% if tcWebApi_get("AppFilter_Entry","Block_RTSP","h") = "1" then asp_Write("checked") end if 
    							 if tcWebApi_get("AppFilter_Entry","Block_RTSP","h") = "N/A" then asp_Write("checked") end if %> value=1 name=RA_allowance> <FONT color=#000000>Deny</FONT> 
    	</TD>
  </TR>
  <TR>
    	<TD class=light-orange></TD>
    	<TD class=light-orange></TD>
    	<TD>&nbsp;</TD>
    	<TD>&nbsp;</TD>
    	<TD>&nbsp;</TD>
  </TR>
  </TBODY>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
<TBODY>
  <TR>
    	<TD class=black width=160 height=42>¡@</TD>
    	<TD class=orange width=160>&nbsp;</TD>
    	<INPUT TYPE="HIDDEN" NAME="NoChange_or_Save" VALUE="0">
    	<TD class=orange width=440>
    		<INPUT type=submit value=SAVE name=APPFilterApply onclick="document.APPFILTERform.NoChange_or_Save.value = 1">
     		<INPUT type=reset value=CANCEL name=APPFilterCancel> 
	</TD>
  </TR>
</TBODY>
</TABLE></DIV>
</FORM>
</BODY>
</HTML>
