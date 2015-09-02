<%
If Request_Form("SaveBtn") = "SAVE" Then 
	TCWebApi_set("Parental_Entry","Enable","tr_Active")
	if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") <> "Yes" then 
	TCWebApi_set("Parental_Entry","MacAddr","tr_MacAddr")
	End If
	if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") = "Yes" then 
		TCWebApi_set("Parental_Entry","BlockType","tr_BlockType")
		If Request_Form("tr_BlockType") = "MAC" Then 
			TCWebApi_set("Parental_Entry","MacAddr","tr_MacAddr")
		End If
		If Request_Form("tr_BlockType") = "URL" Then 
			TCWebApi_set("Parental_Entry","URL","tr_URL")
		End If
	End If
	TCWebApi_set("Parental_Entry","Mon","tr_Mon_Enable")
	TCWebApi_set("Parental_Entry","Start_Mon","tr_Start_Mon")
	TCWebApi_set("Parental_Entry","Inter_Mon","tr_Inter_Mon")
	
	TCWebApi_set("Parental_Entry","Tue","tr_Tue_Enable")
	TCWebApi_set("Parental_Entry","Start_Tue","tr_Start_Tue")
	TCWebApi_set("Parental_Entry","Inter_Tue","tr_Inter_Tue")
	
	TCWebApi_set("Parental_Entry","Wed","tr_Wed_Enable")
	TCWebApi_set("Parental_Entry","Start_Wed","tr_Start_Wed")
	TCWebApi_set("Parental_Entry","Inter_Wed","tr_Inter_Wed")
	
	TCWebApi_set("Parental_Entry","Thu","tr_Thu_Enable")
	TCWebApi_set("Parental_Entry","Start_Thu","tr_Start_Thu")
	TCWebApi_set("Parental_Entry","Inter_Thu","tr_Inter_Thu")
	
	TCWebApi_set("Parental_Entry","Fri","tr_Fri_Enable")
	TCWebApi_set("Parental_Entry","Start_Fri","tr_Start_Fri")
	TCWebApi_set("Parental_Entry","Inter_Fri","tr_Inter_Fri")
	
	TCWebApi_set("Parental_Entry","Sat","tr_Sat_Enable")
	TCWebApi_set("Parental_Entry","Start_Sat","tr_Start_Sat")
	TCWebApi_set("Parental_Entry","Inter_Sat","tr_Inter_Sat")
	
	TCWebApi_set("Parental_Entry","Sun","tr_Sun_Enable")
	TCWebApi_set("Parental_Entry","Start_Sun","tr_Start_Sun")
	TCWebApi_set("Parental_Entry","Inter_Sun","tr_Inter_Sun")
	
  	tcWebApi_commit("Parental_Entry")
End If

%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" src="/val.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>

</head><script language="JavaScript">
function timeSwitch(on_off) {
        var num=25;
<% if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") = "Yes" then %>
        num=28;
<% End If %> 	  
	for(i = 2; i < num; i++)
		document.TIME_form.elements[i].disabled = (on_off == 0);
        document.TIME_form.tr_Browser.checked = false;

}
function typeSwitch(on_off) {

		if(on_off){
		        setDisplay('div_url', 0);
				setDisplay('div_mac', 1);
				}
		else{
		        setDisplay('div_url', 1);
				setDisplay('div_mac', 0);
		}

}

function doGetBrowserMac() {

        if(document.TIME_form.tr_Browser.checked == true) {
                document.TIME_form.tr_MacAddr.value = "<% tcWebApi_get("Parental", "BrowserMAC", "s") %>";
                document.TIME_form.tr_MacAddr.disabled = true;
        } else {
                document.TIME_form.tr_MacAddr.value = "";
                document.TIME_form.tr_MacAddr.disabled = false;
        }
        return;
}

function checkMacAddr(str) {

        var regex=/^([0-9a-f]{2}(:|$)){6}$/i;

        if (regex.test(str) && str.length==17) {
                return true;
        } else {
                return false;
        }

}

function checkStartTime(time) {

        var regex=/^([0-9]{2}(:|$)){2}$/i;

        if (regex.test(time) && time.length==5) {
                var strArray = time.split(":");
                if(parseInt(strArray[0]) >= 0 && parseInt(strArray[0]) <= 23 &&
                   parseInt(strArray[1]) >= 0 && parseInt(strArray[1]) <= 59) {
                        return true;
                }
        }

        return false;

}

function checkIntervalTime(time) {

        var regex=/^([0-9]{2}(:|$)){2}$/i;

        if (regex.test(time) && time.length==5) {
                var strArray = time.split(":");
                if((parseInt(strArray[0]) >= 0 && parseInt(strArray[0]) <= 23 &&
                    parseInt(strArray[1]) >= 0 && parseInt(strArray[1]) <= 59) || 
                   (parseInt(strArray[0]) == 24 && parseInt(strArray[1]) == 0)) {
                        return true;
                }
        }

        return false;

}

function checkSubmit() {

	if(document.getElementById('tr_Active_Yes').checked) {
	
      <% if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") = "Yes" then %>
	       if(document.getElementById('tr_BlockType_MAC').checked) {
	  <% End If %>    
                /* Check MacAddr */
                if(!checkMacAddr(document.TIME_form.tr_MacAddr.value)) {
                        alert("Invalid MAC Address");
                        return false;
                }
	<% if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") = "Yes" then %>
	   }
              /* Check URL */
		if(document.getElementById('tr_BlockType_URL').checked) {
                if(!isValidUrlName(document.TIME_form.tr_URL.value)) {
                        alert("Invalid URL");
                        return false;
                }
			}
	 <% End If %> 
                /* Check Start Time */
                if(!checkStartTime(document.TIME_form.tr_Start_Mon.value)) {
                        alert("Invalid Start Time");
                        document.TIME_form.tr_Start_Mon.select();
                        return false;
                }
                if(!checkStartTime(document.TIME_form.tr_Start_Tue.value)) {
                        alert("Invalid Start Time");
                        document.TIME_form.tr_Start_Tue.select();
                        return false;
                }
                if(!checkStartTime(document.TIME_form.tr_Start_Wed.value)) {
                        alert("Invalid Start Time");
                        document.TIME_form.tr_Start_Wed.select();
                        return false;
                }
                if(!checkStartTime(document.TIME_form.tr_Start_Thu.value)) {
                        alert("Invalid Start Time");
                        document.TIME_form.tr_Start_Thu.select();
                        return false;
                }
                if(!checkStartTime(document.TIME_form.tr_Start_Fri.value)) {
                        alert("Invalid Start Time");
                        document.TIME_form.tr_Start_Fri.select();
                        return false;
                }
                if(!checkStartTime(document.TIME_form.tr_Start_Sat.value)) {
                        alert("Invalid Start Time");
                        document.TIME_form.tr_Start_Sat.select();
                        return false;
                }
                if(!checkStartTime(document.TIME_form.tr_Start_Sun.value)) {
                        alert("Invalid Start Time");
                        document.TIME_form.tr_Start_Sun.select();
                        return false;
                }

                /* Check Interval Time */
                if(!checkIntervalTime(document.TIME_form.tr_Inter_Mon.value)) {
                        alert("Invalid Interval Time");
                        document.TIME_form.tr_Inter_Mon.select();
                        return false;
                }
                if(!checkIntervalTime(document.TIME_form.tr_Inter_Tue.value)) {
                        alert("Invalid Interval Time");
                        document.TIME_form.tr_Inter_Tue.select();
                        return false;
                }
                if(!checkIntervalTime(document.TIME_form.tr_Inter_Wed.value)) {
                        alert("Invalid Interval Time");
                        document.TIME_form.tr_Inter_Wed.select();
                        return false;
                }
                if(!checkIntervalTime(document.TIME_form.tr_Inter_Thu.value)) {
                        alert("Invalid Interval Time");
                        document.TIME_form.tr_Inter_Thu.select();
                        return false;
                }
                if(!checkIntervalTime(document.TIME_form.tr_Inter_Fri.value)) {
                        alert("Invalid Interval Time");
                        document.TIME_form.tr_Inter_Fri.select();
                        return false;
                }
                if(!checkIntervalTime(document.TIME_form.tr_Inter_Sat.value)) {
                        alert("Invalid Interval Time");
                        document.TIME_form.tr_Inter_Sat.select();
                        return false;
                }
                if(!checkIntervalTime(document.TIME_form.tr_Inter_Sun.value)) {
                        alert("Invalid Interval Time");
                        document.TIME_form.tr_Inter_Sun.select();
                        return false;
                }
	}

        document.TIME_form.tr_MacAddr.disabled = false;
        document.TIME_form.tr_Mon_Enable.value = document.TIME_form.tr_Mon.checked?"Yes":"No";
        document.TIME_form.tr_Tue_Enable.value = document.TIME_form.tr_Tue.checked?"Yes":"No";
        document.TIME_form.tr_Wed_Enable.value = document.TIME_form.tr_Wed.checked?"Yes":"No";
        document.TIME_form.tr_Thu_Enable.value = document.TIME_form.tr_Thu.checked?"Yes":"No";
        document.TIME_form.tr_Fri_Enable.value = document.TIME_form.tr_Fri.checked?"Yes":"No";
        document.TIME_form.tr_Sat_Enable.value = document.TIME_form.tr_Sat.checked?"Yes":"No";
        document.TIME_form.tr_Sun_Enable.value = document.TIME_form.tr_Sun.checked?"Yes":"No";

        return true;

}

</script>
<% if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") <> "Yes" then %>
<body onLoad="if(document.getElementById('tr_Active_No').checked) timeSwitch(0);">
<% End If %> 
<% if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") = "Yes" then %>
<body onLoad="if(document.getElementById('tr_Active_No').checked) timeSwitch(0); if(document.getElementById('tr_BlockType_MAC').checked) typeSwitch(1); if(document.getElementById('tr_BlockType_URL').checked) typeSwitch(0);">
<% End If %> 
<FORM METHOD="POST" ACTION="/cgi-bin/access_parental.asp" name="TIME_form"><div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
    <td width="150" height="30" class="title-main"><font color="#FFFFFF">Parental control</font>    </td>
    <td width="10" class="black">&nbsp;</td><td width="150"></td>
    <td width="10" ></td>
    <td width="440"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">Enable</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
      <INPUT NAME="tr_Active" TYPE="RADIO" onClick="timeSwitch(1)" id="tr_Active_Yes" VALUE="Yes" <% If TCWebApi_get("Parental_Entry","Enable","h") = "Yes" then asp_Write("checked") end if%> ><font color="#000000">Activated</font>
      <INPUT NAME="tr_Active" TYPE="RADIO" onClick="timeSwitch(0)" id="tr_Active_No" VALUE="No" <% If TCWebApi_get("Parental_Entry","Enable","h") <> "Yes" then asp_Write("checked") end if%> ><font color="#000000">Deactivated</font>
    </td>
  </tr>
  <% if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") = "Yes" then %>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata" colspan="3"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">Blocking Type</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata"> 
	<input name="tr_BlockType" type="RADIO" onClick="typeSwitch(1)" id="tr_BlockType_MAC" value="MAC" <% If TCWebApi_get("Parental_Entry","BlockType","h") = "N/A" then asp_Write("checked") elseif TCWebApi_get("Parental_Entry","BlockType","h") = "MAC" then asp_Write("checked") end if%> ><font color="#000000">MAC</font> 
    <INPUT NAME="tr_BlockType" TYPE="RADIO" onClick="typeSwitch(0)" id="tr_BlockType_URL" VALUE="URL" <% If TCWebApi_get("Parental_Entry","BlockType","h") <> "N/A" then If TCWebApi_get("Parental_Entry","BlockType","h") <> "MAC" then asp_Write("checked") end if end if%> ><font color="#000000">URL</font> 
	</td>
  </tr>
  <% End If %> 
  </table>
  
  <div id="div_mac">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">	
	<tr>
    	<td class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width= "10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
    <td class="tabdata"><div align=right><font color="#000000">MAC Address</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
      <INPUT NAME="tr_MacAddr" TYPE="TEXT" SIZE="32" MAXLENGTH="17" VALUE="<% If TCWebApi_get("Parental_Entry","MacAddr","h") <> "N/A" then TCWebApi_get("Parental_Entry","MacAddr","s") end if%>" >
      <INPUT NAME="tr_Browser" TYPE="CHECKBOX" ID="tr_Browser" onClick="doGetBrowserMac()">Browser's MAC Address
	</tr>
</table></div>
<% if tcWebApi_get("WebCustom_Entry","isParental_Urlblock","h") = "Yes" then %>
  <div id="div_url">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">	
	<tr>
    	<td class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width= "10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">URL </font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
      <INPUT NAME="tr_URL" TYPE="TEXT" SIZE="48" MAXLENGTH="48" VALUE="<% If TCWebApi_get("Parental_Entry","URL","h") <> "N/A" then TCWebApi_get("Parental_Entry","URL","s") end if%>" >
	</tr>
</table>
</div>  
<% End If %>   
  <table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
    	<td class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width= "10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata" colspan="3">
    <table border="1" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
    <tr class="tabdata">
      <td class="tabdata"></td>
      <td class="tabdata"><div align=center>Mon</div></td>
      <td class="tabdata"><div align=center>Tue</div></td>
      <td class="tabdata"><div align=center>Wed</div></td>
      <td class="tabdata"><div align=center>Thu</div></td>
      <td class="tabdata"><div align=center>Fri</div></td>
      <td class="tabdata"><div align=center>Sat</div></td>
      <td class="tabdata"><div align=center>Sun</div></td>
    </tr>
    <tr> 
      <td class="tabdata"><div align=right>Day of Week</div></td>
      <td><div align=center><INPUT TYPE="CHECKBOX" NAME="tr_Mon" ID="tr_Mon" <% if tcWebApi_get("Parental_Entry", "Mon", "h") = "Yes" then asp_write("checked") end if%> ></div></td>
      <td><div align=center><INPUT TYPE="CHECKBOX" NAME="tr_Tue" ID="tr_Tue" <% if tcWebApi_get("Parental_Entry", "Tue", "h") = "Yes" then asp_write("checked") end if%> ></div></td>
      <td><div align=center><INPUT TYPE="CHECKBOX" NAME="tr_Wed" ID="tr_Wed" <% if tcWebApi_get("Parental_Entry", "Wed", "h") = "Yes" then asp_write("checked") end if%> ></div></td>
      <td><div align=center><INPUT TYPE="CHECKBOX" NAME="tr_Thu" ID="tr_Thu" <% if tcWebApi_get("Parental_Entry", "Thu", "h") = "Yes" then asp_write("checked") end if%> ></div></td>
      <td><div align=center><INPUT TYPE="CHECKBOX" NAME="tr_Fri" ID="tr_Fri" <% if tcWebApi_get("Parental_Entry", "Fri", "h") = "Yes" then asp_write("checked") end if%> ></div></td>
      <td><div align=center><INPUT TYPE="CHECKBOX" NAME="tr_Sat" ID="tr_Sat" <% if tcWebApi_get("Parental_Entry", "Sat", "h") = "Yes" then asp_write("checked") end if%> ></div></td>
      <td><div align=center><INPUT TYPE="CHECKBOX" NAME="tr_Sun" ID="tr_Sun" <% if tcWebApi_get("Parental_Entry", "Sun", "h") = "Yes" then asp_write("checked") end if%> ></div></td>
    </tr>
    <tr class="tabdata"> 
      <td class="tabdata"><div align=right>Start Blocking Time</div></td>
      <td class="tabdata"><INPUT NAME="tr_Start_Mon" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Start_Mon","h") <> "N/A" then TCWebApi_get("Parental_Entry","Start_Mon","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Start_Tue" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Start_Tue","h") <> "N/A" then TCWebApi_get("Parental_Entry","Start_Tue","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Start_Wed" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Start_Wed","h") <> "N/A" then TCWebApi_get("Parental_Entry","Start_Wed","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Start_Thu" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Start_Thu","h") <> "N/A" then TCWebApi_get("Parental_Entry","Start_Thu","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Start_Fri" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Start_Fri","h") <> "N/A" then TCWebApi_get("Parental_Entry","Start_Fri","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Start_Sat" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Start_Sat","h") <> "N/A" then TCWebApi_get("Parental_Entry","Start_Sat","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Start_Sun" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Start_Sun","h") <> "N/A" then TCWebApi_get("Parental_Entry","Start_Sun","s") end if%>" ></td>
    </tr>
    <tr class="tabdata"> 
      <td class="tabdata"><div align=right>Blocking Interval Time</div></td>
      <td class="tabdata"><INPUT NAME="tr_Inter_Mon" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Inter_Mon","h") <> "N/A" then TCWebApi_get("Parental_Entry","Inter_Mon","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Inter_Tue" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Inter_Tue","h") <> "N/A" then TCWebApi_get("Parental_Entry","Inter_Tue","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Inter_Wed" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Inter_Wed","h") <> "N/A" then TCWebApi_get("Parental_Entry","Inter_Wed","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Inter_Thu" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Inter_Thu","h") <> "N/A" then TCWebApi_get("Parental_Entry","Inter_Thu","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Inter_Fri" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Inter_Fri","h") <> "N/A" then TCWebApi_get("Parental_Entry","Inter_Fri","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Inter_Sat" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Inter_Sat","h") <> "N/A" then TCWebApi_get("Parental_Entry","Inter_Sat","s") end if%>" ></td>
      <td class="tabdata"><INPUT NAME="tr_Inter_Sun" TYPE="TEXT" SIZE="4" MAXLENGTH="5" VALUE="<% If TCWebApi_get("Parental_Entry","Inter_Sun","h") <> "N/A" then TCWebApi_get("Parental_Entry","Inter_Sun","s") end if%>" ></td>
    </tr>
    </table>
    </td>
  </tr>
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td>
    <td width="160" class="orange"></td>
    <td width="440" class="orange">
          <INPUT TYPE="HIDDEN" NAME="tr_Mon_Enable" VALUE="">
          <INPUT TYPE="HIDDEN" NAME="tr_Tue_Enable" VALUE="">
          <INPUT TYPE="HIDDEN" NAME="tr_Wed_Enable" VALUE="">
          <INPUT TYPE="HIDDEN" NAME="tr_Thu_Enable" VALUE="">
          <INPUT TYPE="HIDDEN" NAME="tr_Fri_Enable" VALUE="">
          <INPUT TYPE="HIDDEN" NAME="tr_Sat_Enable" VALUE="">
          <INPUT TYPE="HIDDEN" NAME="tr_Sun_Enable" VALUE="">
          <INPUT TYPE="SUBMIT" NAME="SaveBtn" VALUE="SAVE" onClick="return checkSubmit()">
    </td>
  </tr>
</table>
</div></form>
</body></html>
