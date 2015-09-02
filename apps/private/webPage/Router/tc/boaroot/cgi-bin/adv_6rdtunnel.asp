<%
	if Request_Form("SaveFlag")="1" then 
	TCWebApi_set("ipv6rd_Entry","Active","tunnelEnable")
	if TCWebAPI_get("ipv6rd_Entry", "Active", "h") = "Yes" then
		TCWebApi_set("ipv6rd_Entry","Prefix","ipv6rd_prefix")		
		TCWebApi_set("ipv6rd_Entry","PrefixLen","ipv6rd_prefixlen")
		TCWebApi_set("ipv6rd_Entry","CEIPv4Addr","CEipv4Addr")
		TCWebApi_set("ipv6rd_Entry","IPv4MaskLen","ipv4masklen")
		TCWebApi_set("ipv6rd_Entry","BRIPv4Addr","BRipv4Addr")
	end if
	tcWebApi_commit("ipv6rd_Entry")

	End If
%>
<HTML><HEAD>
<META http-equiv=Content-Script-Type content=text/javascript>
<META http-equiv=Content-Style-Type content=text/css>
<META http-equiv=Content-Type content="text/html; charset=&#10;iso-8859-1">
<LINK href="/style.css" type=text/css rel=stylesheet>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<SCRIPT language=javascript>

function isNumeric(value)
{
    var len= value.length;
    if(len==0)
     	return false;
	if(value.match("[^0-9]") != null)	                             
		return false;
	else
		return true;
}

function init(){
	var i;
	if(document.ipv6RDtunnelForm.tunnelEnable[1].checked){
	document.ipv6RDtunnelForm.ipv6rd_prefix.disabled = true;
	document.ipv6RDtunnelForm.ipv6rd_prefixlen.disabled = true;
	document.ipv6RDtunnelForm.CEipv4Addr.disabled = true;
	document.ipv6RDtunnelForm.ipv4masklen.disabled = true;
	document.ipv6RDtunnelForm.BRipv4Addr.disabled = true;	
	}
	else{
	document.ipv6RDtunnelForm.ipv6rd_prefix.disabled = false;
	document.ipv6RDtunnelForm.ipv6rd_prefixlen.disabled = false;
	document.ipv6RDtunnelForm.CEipv4Addr.disabled = false;
	document.ipv6RDtunnelForm.ipv4masklen.disabled = false;
	document.ipv6RDtunnelForm.BRipv4Addr.disabled = false;		
	}
}

function uiSave()
{
	var value1;
	var value2;
	var value3;
	
	value1 = document.forms[0].ipv4masklen.value;
	value2 = document.forms[0].ipv6rd_prefixlen.value;
	value3 = parseInt(value2) + 32 - parseInt(value1);
	
	if(document.ipv6RDtunnelForm.tunnelEnable[0].checked){
	if(inValidIPv6Addr(document.forms[0].ipv6rd_prefix.value)) 
	{
		return false;
	}
	if(inValidIPv6Prefix(document.forms[0].ipv6rd_prefixlen.value))
	{
		return false;
	}
	if(inValidIPAddr(document.forms[0].CEipv4Addr.value))
	{
		return false;
	}
	if((!isNumeric(value1)) || (Number(value1) > 32) || (Number(value1) < 0))
	{
		alert("IPv4 mask length must be in the range 0-32");
		return false;
	}
	if(inValidIPAddr(document.forms[0].BRipv4Addr.value))
	{
		return false;
	}
	if( value3 < 0 || value3 > 128 )
	{
		alert('Difference between Prefix Length and Mask Length should not be greater than 96');
		return false;
	}
	}
	document.forms[0].SaveFlag.value = 1;
	document.forms[0].submit();
	
}

</SCRIPT>

<META content="MSHTML 6.00.2800.1400" name=GENERATOR></HEAD>
<BODY onload="init()">
<FORM METHOD="POST" ACTION="/cgi-bin/adv_6rdtunnel.asp" name="ipv6RDtunnelForm">
<DIV align=center>
    <TABLE cellSpacing=0 cellPadding=0 width=760 align=center bgColor=#ffffff 
border=0>
      <TBODY>
        <TR> 
          <TD class=light-orange colSpan=5 height=5>&nbsp;</TD>
        </TR>
        <TR> 
          <TD class=light-orange>&nbsp;</TD>
          <TD class=light-orange></TD>
          <TD class=tabdata> <DIV align=right><FONT color=#000000>6RD Tunnel</FONT> 
            </DIV></TD>
          <TD class=tabdata> <DIV align=center>:</DIV></TD>
          <TD class=tabdata> <INPUT type="radio" value="Yes" name="tunnelEnable"  <% if tcWebApi_get("ipv6rd_Entry","Active","h") = "Yes" then asp_Write("checked") elseif tcWebApi_get("ipv6rd_Entry","Active","h") = "N/A" then asp_Write("checked") end if %> onclick="init()"> 
            <FONT color=#000000>Enabled</FONT> <INPUT type="radio" value="No" name="tunnelEnable" <% if tcWebApi_get("ipv6rd_Entry","Active","h") = "No" then asp_Write("checked") end if %> onclick="init()"> 
            <FONT color=#000000>Disabled</FONT> </TD>
        </TR>
        <TR> 
          <TD class=light-orange>&nbsp;</TD>
          <TD class=light-orange></TD>
          <TD class=tabdata> <DIV align=right><font color="#000000">6rd IPv6 Prefix</font> 
            </DIV></TD>
          <TD class=tabdata> <DIV align=center>:</DIV></TD>
          <TD><INPUT name="ipv6rd_prefix" maxLength=39 size=39 value="<%If TcWebApi_get("ipv6rd_Entry","Prefix","h") <> "N/A" then TcWebApi_get("ipv6rd_Entry","Prefix","s") else asp_Write("2001:55c::") end if %>" > 
            <font size=+1>&nbsp;/&nbsp;</font> <INPUT TYPE="TEXT" NAME="ipv6rd_prefixlen" SIZE="3" MAXLENGTH="3" VALUE="<%if tcwebapi_get("ipv6rd_Entry","PrefixLen","h")<> "N/A" then tcwebApi_get("ipv6rd_Entry","PrefixLen","s") else asp_write("32") end if%>"></TD>
        </TR>
        <TR> 
          <TD class=light-orange>&nbsp;</TD>
          <TD class=light-orange></TD>
          <TD class=tabdata> <DIV align=right><font color="#000000">IPv4 Addr</font> 
            </DIV></TD>
          <TD class=tabdata> <DIV align=center>:</DIV></TD>
          <TD><INPUT name="CEipv4Addr" maxLength=20 size=20 value="<%If TcWebApi_get("ipv6rd_Entry","CEIPv4Addr","h") <> "N/A" then TcWebApi_get("ipv6rd_Entry","CEIPv4Addr","s") else asp_Write("58.211.230.102") end if %>" > 
          </TD>
        </TR>
        <TR> 
          <TD class=light-orange>&nbsp;</TD>
          <TD class=light-orange></TD>
          <TD class=tabdata> <DIV align=right><font color="#000000">IPv4 Mask 
              Length</font></DIV></TD>
          <TD class=tabdata> <DIV align=center>:</DIV></TD>
          <TD><INPUT name="ipv4masklen" maxLength=3 size=3 value="<%If TcWebApi_get("ipv6rd_Entry","IPv4MaskLen","h") <> "N/A" then TcWebApi_get("ipv6rd_Entry","IPv4MaskLen","s") else asp_Write("0") end if %>" > 
          </TD>
        </TR>
        <TR> 
          <TD class=light-orange>&nbsp;</TD>
          <TD class=light-orange></TD>
          <TD class=tabdata> <DIV align=right><font color="#000000">6RD Border 
              Relay IPv4Addr</font></DIV></TD>
          <TD class=tabdata> <DIV align=center>:</DIV></TD>
          <TD><INPUT name="BRipv4Addr" maxLength=39 size=39 value="<%If TcWebApi_get("ipv6rd_Entry","BRIPv4Addr","h") <> "N/A" then TcWebApi_get("ipv6rd_Entry","BRIPv4Addr","s") else asp_Write("69.252.80.66") end if %>" > 
          </TD>
        </TR>
		<tr>
<td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right> 
              <font color="#000000">6rd Prefix Delegation</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<%If TcWebApi_get("ipv6rd_Entry","6rdPD","h") <> "N/A" Then TcWebApi_get("ipv6rd_Entry","6rdPD","s") else asp_Write("N/A") end if%>       
</td></tr>
        <TR> 
          <TD class=light-orange width=140>&nbsp;</TD>
          <TD class=light-orange width=11></TD>
          <TD width=179>&nbsp;</TD>
          <TD width=10>&nbsp;</TD>
          <TD width=420>&nbsp;</TD>
        </TR>
      </TBODY>
    </TABLE>
    <table cellspacing=0 cellpadding=0 width=760 align=center bgcolor=#ffffff 

border=0>
  <tbody>
  <tr height="42" >
    <td width="160" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
        <INPUT TYPE="button" NAME="SaveBtn" VALUE="SAVE" onClick="uiSave()">
          <input type="HIDDEN" name="SaveFlag" value="0"> 
          <INPUT TYPE="reset" NAME="lan_cancel" VALUE="CANCEL" ></td></tr>
      </tbody>
    </table>
  </DIV>

</FORM></BODY></HTML>

