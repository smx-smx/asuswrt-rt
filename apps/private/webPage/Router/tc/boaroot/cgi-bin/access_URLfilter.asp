<%
if request_Form("Duplicate")="1" then
	TCWebAPI_unset("UrlFilter_Entry")
	TCWebAPI_commit("UrlFilter_Entry")
	tcWebApi_set("UrlFilter_Common","Duplicate","NoDup")
end if
if request_form("UrlFilter_index") <> "" Then
	tcWebApi_set("WebCurSet_Entry","url_filter_id","UrlFilter_index")
	tcWebApi_set("GUITemp_Entry0","IndexTmp","UrlFilter_index")
end if

if request_form("Save_or_Delete") = "1" Then 
	tcWebApi_set("UrlFilter_Common","Activate","RuleIndex_active")
	tcWebApi_set("UrlFilter_Entry","URL","UrlFilter_URL")
	tcWebApi_set("UrlFilter_Entry","Activate","SingleRule_active")
	tcWebApi_commit("UrlFilter")
elseif request_form("Save_or_Delete") = "2" Then 
	tcWebApi_unset("UrlFilter_Entry")
	tcWebApi_commit("UrlFilter")
End If

%>

<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<SCRIPT language=javascript>

function switchFilterType(object)
{
  var index = object.selectedIndex;
	switch(index)
	{
		case 0:
			window.location='access_ipfilter.asp';
			break;
		case 1:
			window.location='access_appfilter.asp';
			break;
	}
}
function doSubmit(){
	var str=UrlFilterform.UrlFilter_URL.value;
	var lower_str;

	lower_str = str.toLowerCase();
	str = lower_str;
	UrlFilterform.UrlFilter_URL.value = lower_str;

	if ((UrlFilterform.SingleRule_active[1].checked) && (str.length == 0)) {
		//alert("set switch for each");
	}
	else {
		if (isValidUrlName(str) == false){
			alert("invalid url, please check!");
			return false;
		}	
	}
	document.UrlFilterform.Save_or_Delete.value = 1;
	document.UrlFilterform.submit();
}

function init()
{
	if(document.forms[0].Duplicate.value == 1)
	{
		alert("The rule has already exist!");
		document.forms[0].submit();
	}
}
</SCRIPT>

<META content="MSHTML 6.00.2900.3059" name=GENERATOR></HEAD>
<BODY onLoad="init();">
<FORM name=UrlFilterform action="/cgi-bin/access_URLfilter.asp" method=post>
<DIV align=center>
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff  border=0>
<TBODY>
	<TR>
    		<TD class=light-orange colSpan=5 height=5>&nbsp;</TD>
    	</TR>
  	<TR>
    		<TD class=title-main width=150 height=30><FONT color=#ffffff>Filter</FONT> </TD>
    		<TD class=black width=10>&nbsp;</TD>
    		<TD width=150></TD>
    		<TD width=10></TD>
    		<TD width=440></TD>
    	</TR>
  	<TR>
    		<TD class=title-sub height=25><FONT color=#000000>Filter Type</FONT> </TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    	</TR>
  	<TR>
    		<TD class=light-orange>&nbsp;</TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata><DIV align=right><FONT color=#000000>Filter Type Selection</FONT> </DIV></TD>
    		<TD class=tabdata><DIV align=center>:</DIV></TD>
    		<TD class=tabdata>    
			<SELECT onchange=switchFilterType(this) size=1 name=FILTERTYPE_index>
				<OPTION >IP / MAC Filter
				<OPTION>Application Filter
				<OPTION SELECTED>URL Filter
    			</SELECT>
		</TD>
	</TR>
  	<TR>
    		<TD class=title-sub height=25><FONT color=#000000>URL Filter Editing</FONT> </TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    	</TR>
  	<TR>
    		<TD class=light-orange>&nbsp;</TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata><DIV align=right><FONT color=#000000>Active</FONT> </DIV></TD>
    		<TD class=tabdata><DIV align=center>:</DIV></TD>
    		<TD class=tabdata>
    			<INPUT type=radio <% if tcWebApi_get("UrlFilter_Common","Activate","h") = "1" then asp_Write("checked") end if %> value=1 name=RuleIndex_active> <FONT color=#000000>Yes</FONT> 
    			<INPUT type=radio <% if tcWebApi_get("UrlFilter_Common","Activate","h") = "0" then asp_Write("checked") end if 
    							 	 if tcWebApi_get("UrlFilter_Common","Activate","h") = "N/A" then asp_Write("checked") end if %> value=0 name=RuleIndex_active> <FONT color=#000000>No</FONT>
     		</TD>
     	</TR>
  	<TR>
    		<TD class=light-orange>&nbsp;</TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD class=tabdata>
      			<HR class=light-orange-line noShade>
    		</TD>
    	</TR>
  	<TR>
    		<TD class=light-orange>&nbsp;</TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata><DIV align=right><FONT color=#000000>URL Index</FONT> </DIV></TD>
    		<TD class=tabdata><DIV align=center>:</DIV></TD>
		<TD class=tabdata>
    			 <SELECT NAME="UrlFilter_index" SIZE="1" onchange="document.UrlFilterform.submit();">
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "0" then asp_Write("selected") end if %> value="0">1
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "1" then asp_Write("selected") end if %> value="1">2
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "2" then asp_Write("selected") end if %> value="2">3
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "3" then asp_Write("selected") end if %> value="3">4
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "4" then asp_Write("selected") end if %> value="4">5
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "5" then asp_Write("selected") end if %> value="5">6
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "6" then asp_Write("selected") end if %> value="6">7
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "7" then asp_Write("selected") end if %> value="7">8
					<!-- Only Support 8 rules
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "8" then asp_Write("selected") end if %> value="8">9
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "9" then asp_Write("selected") end if %> value="9">10
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "10" then asp_Write("selected") end if %> value="10">11
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "11" then asp_Write("selected") end if %> value="11">12
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "12" then asp_Write("selected") end if %> value="12">13
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "13" then asp_Write("selected") end if %> value="13">14
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "14" then asp_Write("selected") end if %> value="14">15
      				<OPTION <% if tcWebApi_get("WebCurSet_Entry","url_filter_id","h") = "15" then asp_Write("selected") end if %> value="15">16
					-->
    			</SELECT>
    		</TD>
    	</TR>
    	<TR>
    		<TD class=light-orange>&nbsp;</TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata><DIV align=right><FONT color=#000000>Individual active</FONT> </DIV></TD>
    		<TD class=tabdata><DIV align=center>:</DIV></TD>
    		<TD class=tabdata>
    			<INPUT type=radio <% if tcWebApi_get("UrlFilter_Entry","Activate","h") = "1" then asp_Write("checked") end if %> value=1 name=SingleRule_active> <FONT color=#000000>Yes</FONT> 
    			<INPUT type=radio <% if tcWebApi_get("UrlFilter_Entry","Activate","h") = "0" then asp_Write("checked") end if 
    							 	 if tcWebApi_get("UrlFilter_Entry","Activate","h") = "N/A" then asp_Write("checked") end if %> value=0 name=SingleRule_active> <FONT color=#000000>No</FONT>
     		</TD>
     	</TR>
  	<TR>
    		<TD class=light-orange>&nbsp;</TD>
    		<TD class=light-orange></TD>
    		<TD class=tabdata><DIV align=right><FONT color=#000000>URL(host)</FONT> </DIV></TD>
    		<TD class=tabdata><DIV align=center>:</DIV></TD>
    		<TD class=tabdata>
    			<INPUT TYPE="TEXT" maxLength=48 size=48 name="UrlFilter_URL" VALUE=<% if 
    			tcWebApi_get("UrlFilter_Entry","URL","h") <> "N/A" then  tcWebApi_get("UrlFilter_Entry","URL","s") end if %> > 
  		</TD>
  	</TR>
  </TBODY>
</TABLE>

<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff border=0>
<TBODY>
  	<TR>
    		<TD class=title-sub width=150><FONT color=#000000>URL Filter Listing</FONT> </TD>
    		<TD class=light-orange width=10></TD>
    		<TD width=10>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD width=580>
      			<HR class=light-orange-line noShade>
    		</TD>
    		<TD width=10>
      			<HR class=light-orange-line noShade>
    		</TD>
    	</TR>
  	<TR>
    		<TD class=light-orange></TD>
    		<TD class=light-orange></TD>
    		<TD>&nbsp;</TD>
    		<TD>
			<!--
    			<iframe src="/cgi-bin/access_urlfilterlist.cgi" frameborder="0" width="580" height="200"></iframe>
			-->
<%if tcwebApi_get("UrlFilter_Common","Duplicate","h")<>"Yes" then %>

			<TABLE width="580" border="1" align=center cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
				<tr>
					
                <TD class=tabdata align=center width=40><STRONG>Index</STRONG></TD>
					<td width=80 align=center class="tabdata"><strong><FONT color=\"#000000\">Active</FONT></strong></td>
					
					 
                <TD class=tabdata align=left width=460><STRONG><font color=#000000> 
                  URL </font></STRONG></TD>
				</tr>
				<% tcWebApi_get("GUITemp_Entry0","urlfilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry1","urlfilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry2","urlfilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry3","urlfilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry4","urlfilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry5","urlfilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry6","urlfilterTRLine","s") %>
				<% tcWebApi_get("GUITemp_Entry7","urlfilterTRLine","s") %>
				
				</TABLE>
<%end if%>
		</TD>
    		<TD>&nbsp;</TD>
    	</TR>
  <TR>
    <TD class=light-orange></TD>
    <TD class=light-orange></TD>
    <TD>&nbsp;</TD>
    <TD>&nbsp;</TD>
    <TD>&nbsp;</TD></TR></TBODY></TABLE>
   
<TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
  <TBODY>
  <TR>
    <TD class=black width=160 height=42>&nbsp;</TD>
    <TD class=orange width=160>&nbsp;</TD>
    <TD class=orange width=440>
    <INPUT TYPE="HIDDEN" NAME="Save_or_Delete" VALUE="0">
    <INPUT type=button value=SAVE name=UrlFilterApply onclick="doSubmit()">
    <INPUT type=submit value=DELETE name=UrlFilterDelete onclick="document.UrlFilterform.Save_or_Delete.value = 2">
    <INPUT type=reset value=CANCEL name=UrlFilterCancel>
	<INPUT type="hidden" name="Duplicate" VALUE="<%if tcwebApi_get("UrlFilter_Common","Duplicate","h")="Yes" then asp_write("1") else asp_write("0") end if%>">
	<Input type="hidden" name="NoDup" value="No">
</TD></TR></TBODY></TABLE></DIV></FORM></BODY></HTML>
