<%
If Request_Form("saveFlag") = "1" Then
	if request_form("sraFlag") = "1" then
		tcWebApi_set("Adsl_Entry","SRAActive","ACTIVE")
	end if
tcWebApi_set("Adsl_Entry","MODULATIONTYPE","ModulationType")
tcWebApi_set("Adsl_Entry","ANNEXTYPEA","AnnexTypeA")
tcWebApi_commit("Adsl_Entry")
end if
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">

<script>
function doSave() {
   document.adv_adsl.saveFlag.value = 1;
   document.adv_adsl.submit();
   return;
}
</script>

<style type="text/css">

</style>
</head><body>
<FORM METHOD="POST" ACTION="/cgi-bin/adv_adsl.asp" name="adv_adsl">
<input type="hidden" name="sraFlag" value="<%if TcWebApi_get("WebCurSet_Entry","UserMode","h")="0" then asp_write("1") else asp_write("0") end if%>">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF" id="uiViewBodyTable">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	<%tcWebApi_get("String_Entry","XdslText","s")%>
	<%else%>
	<%tcWebApi_get("String_Entry","AdslText","s")%>
	<%end if%></font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10"> </td><td width="440"></td></tr>
<%if tcWebApi_get("WebCustom_Entry","isC2TrueSupported","h")="Yes" then%>
<%if TcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
	<tr>
   		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","AdslSraText","s")%></font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<input type="RADIO" name="ACTIVE" value="Yes" <% if tcWebApi_get("Adsl_Entry","SRAActive","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000"><%tcWebApi_get("String_Entry","AdslSraEnableText","s")%></font>
			<input type="RADIO" name="ACTIVE" value="No"  <% if tcWebApi_get("Adsl_Entry","SRAActive","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Adsl_Entry","SRAActive","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000"><%tcWebApi_get("String_Entry","AdslSraDisableText","s")%></font>
		</td>
	</tr>
<% end if %>
<% end if %>
	<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	<%tcWebApi_get("String_Entry","XdslModeText","s")%>
	<%else%>
	<%tcWebApi_get("String_Entry","AdslModeText","s")%>
	<%end if%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
        <SELECT NAME="ModulationType" SIZE="1">
<%if tcWebApi_get("Adsl_Entry","ANNEXTYPE","h") = "B" then %>
<option value="Auto Sync-Up" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "Auto Sync-Up" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType0Text","s")%>
<%if tcWebApi_get("WebCustom_Entry","isCwmpVDSLSupport","h") = "Yes" then%>
<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL Auto Sync-Up" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType7Text","s")%>
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<option value="VDSL2" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "VDSL2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType6Text","s")%>
<%end if%>
<option value="ADSL2+" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2+" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType1Text","s")%>
<option value="ADSL2" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType5Text","s")%>
<option value="G.DMT" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.DMT" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType2Text","s")%>
<%else%>
<option value="Auto Sync-Up" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "Auto Sync-Up" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType0Text","s")%>
<%if tcWebApi_get("WebCustom_Entry","isCwmpVDSLSupport","h") = "Yes" then%>
<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL Auto Sync-Up" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType7Text","s")%>
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<option value="VDSL2" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "VDSL2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType6Text","s")%>
<%end if%>
<option value="ADSL2+" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2+" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType1Text","s")%>
<option value="ADSL2" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType5Text","s")%>
<option value="G.DMT" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.DMT" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType2Text","s")%>
<option value="T1.413" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "T1.413" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType3Text","s")%>
<option value="G.lite" <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.lite" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslModulType4Text","s")%>
<%end if%>
</SELECT>    </td></tr>  <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000"><%tcWebApi_get("String_Entry","AdslTypeText","s")%></font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    <SELECT NAME="AnnexTypeA" SIZE="1">
<%if tcWebApi_get("Adsl_Entry","ANNEXTYPE","h") = "B" then %>
<option value="ANNEX B" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType3Text","s")%>
<option value="ANNEX J" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX J" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType5Text","s")%>
<option value="ANNEX M" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX M" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType6Text","s")%>
<option value="ANNEX B/J/M" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J/M" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType7Text","s")%>
<%else%>
<option value="ANNEX A" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType0Text","s")%>
<option value="ANNEX I" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX I" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType1Text","s")%>
<option value="ANNEX A/L" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A/L" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType2Text","s")%>
<option value="ANNEX M" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX M" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType3Text","s")%>
<option value="ANNEX A/I/J/L/M" <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A/I/J/L/M" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","AdslType4Text","s")%>
<%end if%>
</SELECT></td></tr>  <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td class="tabdata">&nbsp;</td></tr>
    </table><table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="HIDDEN" NAME="saveFlag" VALUE="0">
		<INPUT TYPE="SUBMIT" NAME="adsl_opmode_apply" VALUE="<%tcWebApi_get("String_Entry","ButtonSaveText","s")%>" onClick="doSave();">	</td></tr></table><p>&nbsp;</p>
</div></form>
</body></html>
