<%
If Request_Form("SaveTime") = "1" Then	
	TCWebApi_set("Timezone_Entry","TZ","uiViewdateToolsTZ") 
	TCWebApi_set("Timezone_Entry","DAYLIGHT","uiViewdateDS") 
	TCWebApi_set("Timezone_Entry","SERVER","uiViewSNTPServer")
	If Request_Form("ntp2ServerFlag") = "Yes" Then
		TCWebApi_set("Timezone_Entry","SERVER2","uiViewSNTPServer2")
	elseif Request_Form("ntp3ServerFlag") = "Yes" Then
		TCWebApi_set("Timezone_Entry","SERVER2","uiViewSNTPServer2")
		TCWebApi_set("Timezone_Entry","SERVER3","uiViewSNTPServer3")
	end if	
	TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
	If Request_Form("uiViewSyncWith") = "1" Then
		TCWebApi_set("Timezone_Entry","PC_CLOCK","uiCurrentTime2")
		TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
	End if	

	If Request_Form("uiViewSyncWith") = "2" Then
		TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
		TcWebApi_set("Timezone_Entry","Year","uiPCdateYear")
		TcWebApi_set("Timezone_Entry","Month","uiPCdateMonth")
		TcWebApi_set("Timezone_Entry","Date","uiPCdateDay")
		TcWebApi_set("Timezone_Entry","Hour","uiPCdateHour")
		TcWebApi_set("Timezone_Entry","Min","uiPCdateMinute")
		TcWebApi_set("Timezone_Entry","Sec","uiPCdateSec")
	End if		
	
	tcWebApi_commit("Timezone_Entry")

End if

If Request_Form("uiViewSyncWith") <> "" Then
	If Request_Form("uiViewSyncWith") = "0" Then
		TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
		TCWebApi_set("Timezone_Entry","PCSyncFlag","uiClearPCSyncFlag")
	End if
	If Request_Form("uiViewSyncWith") = "1" Then
		TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
		If Request_Form("uiRadioValue") = "1" Then
			TCWebApi_set("Timezone_Entry","PC_CLOCK","uiCurrentTime2")
			TCWebApi_set("Timezone_Entry","PCSyncFlag","uiClearPCSyncFlag")
			tcWebApi_commit("Timezone_Entry")
		End if
	End if	

	If Request_Form("uiViewSyncWith") = "2" Then
		TCWebApi_set("Timezone_Entry","TYPE","uiViewSyncWith")
		TCWebApi_set("Timezone_Entry","PCSyncFlag","uiClearPCSyncFlag")
	End if		
End if
%>


<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css" >
<script language="JavaScript" src="/val.js"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
</head><SCRIPT language="javascript">

function doTimeChange() {
    document.Alpha_Time.ToolsTimeSetFlag.value = 1;
  
    if(document.Alpha_Time.uiViewSyncWith[0].checked)
        document.Alpha_Time.uiCurrentTime1.value =  document.Alpha_Time.uiCurTime.value;

    return;
}

function isAsciiString(s) {
    var len= s.length;
    var ch;

    if(len == 0)
        return false;
    for( i=0; i< len; i++) {
       ch= s.charCodeAt(i);
       if(ch > 0x39 || ch < 0x30)
          return false;
    }
    return true;
}

function uiComputerTimeOnLoad() {
    var smtmp;
    if(document.Alpha_Time.uiRadioValue.value != null ){
    	smtmp = document.Alpha_Time.uiRadioValue.value;
    }
    else{
    	return;
    }
    if(smtmp=="1"){
        syncwithpc();
      }
    else
        return;
}

function syncwithpc() {
    var form=document.Alpha_Time;
    theDate=(new Date()).getDate();
    form.uiwPCdateDay.value = theDate;

    theMonth=(new Date()).getMonth();
    form.uiwPCdateMonth.value = theMonth;

    theYear=(new Date()).getFullYear();
    form.uiwPCdateYear.value = theYear;

    theHour=(new Date()).getHours();
    form.uiwPCdateHour.value = theHour;

    theMinute=(new Date()).getMinutes();
    form.uiwPCdateMinute.value = theMinute;

    theSec=(new Date()).getSeconds();
    form.uiwPCdateSec.value = theSec;

    form.ToolsTimeSetFlag.value = 1;
    uiShowNow();

}

function uiDoCancel() {
    document.location.href="/cgi-bin/tools_time.asp";
}

function isValidIpAddr(ip1,ip2,ip3,ip4) {
alert("krammer is here!!");
    if(ip1==0 || ip4==0 || ip4==255 || ip1==127)
        return false;
        return true;
}

function uiTimeSave() 
{
	var value;
	var timestring;
	var form=document.Alpha_Time;

	if (form.uiViewSyncWith[2].checked)
	{
		day = form.uiPCdateDay.value;
		value = parseInt(day);
		if(isNaN(value))
		{
			alert ("Invalid day!");
			return false;
		}
		month = form.uiPCdateMonth.value;
		value = parseInt(month);
		if(isNaN(value))
		{
			alert ("Invalid month!");
			return false;
		}
		year = form.uiPCdateYear.value;
		value = parseInt(year);
		if(isNaN(value) || value > 2036 || value < 1970)
		{
			alert ("Invalid year!");
			return false;
		}
		
		if (!((1<=month) && (12>=month) && (31>=day) && (1<=day)) )
		{
			alert ("Invalid day!");
			return false;
		}
		if (!((year % 4)==0) && (month==2) && (day==29))
		{
			alert ("This is not a leap year!");
			return false;
		}
		if ((month<=7) && ((month % 2)==0) && (day>=31))
		{
			alert ("This month is a small month!");
			return false;
		
		}
		if ((month>=8) && ((month % 2)==1) && (day>=31))
		{
			alert ("This month is a small month!");
			return false;
		}
		if ((month==2) && (day==30))
		{
			alert("The February never has this day!");
			return false;
		}
		
		hour = form.uiPCdateHour.value;
		value = parseInt(hour);
		if(isNaN(value) || value >23 || value <0)
		{
			alert ("Invalid hour!");
			return false;
		}
		min = form.uiPCdateMinute.value;
		value = parseInt(min);
		if(isNaN(value) || value >59 || value <0)
		{
			alert ("Invalid minute!");
			return false;
		}
		sec = form.uiPCdateSec.value;
		value = parseInt(sec);
		if(isNaN(value) || value >59 || value <0)
		{
			alert ("Invalid second!");
			return false;
		}
		timestring=year+"/"+month+"/"+day+" "+hour+":"+min+":"+sec
		form.uiCurrentTime.value=timestring
	}
	
	else if (form.uiViewSyncWith[0].checked)
	{
		value = form.uiViewSNTPServer.value;
		
		//if(value !="0.0.0.0")
		//{
		//	if (inValidIPAddr(value)){
		//	    alert("krammer is here!!");
		//		return false;
		//	}
		//}
		
	}
	else if (form.uiViewSyncWith[1].checked)
	{
		syncwithpc();
	}
	form.SaveTime.value = 1;
    form.submit();
}

/*
function uiShowNow() {
	var  now  =  new  Date();  
        var  hh  =  now.getHours();  
        var  mm  =  now.getMinutes();  
        var  ss  =  now.getTime()  %  60000;  
        ss  =  (ss  -  (ss  %  1000))  /  1000;  
        var  clock  =  hh+':';  
        if  (mm  <  10)  clock  +=  '0';  
        clock  +=  mm+':';  
        if  (ss  <  10)  clock  +=  '0';  
        clock  +=  ss;  
	document.Alpha_Time.uiCurrentTime.value = now.getYear() + "/" + (now.getMonth()+1) + "/" + now.getDate() + " " + clock;
	setTimeout("uiShowNow()",1000);
}*/
function uiShowNow() {
	if(document.Alpha_Time.uiViewSyncWith[1].checked) 
	{ 
		var  now  =  new  Date();
		var  hh  =  now.getHours();  
		var  mm  =  now.getMinutes();  
		var  ss  =  now.getTime()  %  60000;  
		ss  =  (ss  -  (ss  %  1000))  /  1000;  
		var  clock  =  hh+':';  
		if  (mm  <  10)  clock  +=  '0';  
		clock  +=  mm+':';  
		if  (ss  <  10)  clock  +=  '0';  
		clock  +=  ss;  
		document.Alpha_Time.uiCurrentTime.value = now.getYear() + "/" + (now.getMonth()+1) + "/" + now.getDate() + " " + clock;
		document.Alpha_Time.uiCurrentTime1.value = now.toString();
	}
	else
	{
		document.Alpha_Time.uiCurrentTime1.value =  document.Alpha_Time.uiCurTime.value;
	}
}

function doDisplay()
{
	if(document.Alpha_Time.uiViewSyncWith[0].checked)
	{
		setDisplay('timezone_ntp_div0',1);
		setDisplay('timezone_manual_div2',0);
	}
	else if(document.Alpha_Time.uiViewSyncWith[1].checked)
	{
		setDisplay('timezone_ntp_div0',0);
		setDisplay('timezone_manual_div2',0);
	}
	else if(document.Alpha_Time.uiViewSyncWith[2].checked)
	{
		setDisplay('timezone_ntp_div0',0);
		setDisplay('timezone_manual_div2',1);	
	}
	
}
function startRefeshForNtp()
{
	var str1="N/A (NTP server is connecting)";
	var str2="N/A (Can't find NTP server)";
	var str3=document.Alpha_Time.uiCurrentTime1.value;
	
	setTimeout("startRefeshForNtp()",10000);
	
	if( (!document.Alpha_Time.uiViewSyncWith[0].checked) || (document.Alpha_Time.uiTimezoneType.value != "0") )
		return ;

	if(str1 == str3 || str2 == str3)
		document.location.href="/cgi-bin/tools_time.asp";
}
</SCRIPT>

<body onLoad="uiComputerTimeOnLoad();doDisplay();uiShowNow();setTimeout('startRefeshForNtp()',10000);">
<FORM METHOD="POST" ACTION="/cgi-bin/tools_time.asp" name="Alpha_Time">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
<INPUT TYPE="HIDDEN" name="SaveTime" VALUE="0">
<INPUT TYPE="HIDDEN" id="uiCurrentTime" name="uiCurrentTime2">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">Time Zone</font></td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td></tr><tr>
    <td class="light-orange"></td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Current Date/Time</font></div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
	<input type="text" id="uiCurrentTime1" name="uiCurrentTime1" size="40" maxlength="40" onFocus="document.Alpha_Time.SaveBtn.focus()" style="border:0">
	</td></tr>	
<tr>
    <td height="30" class="title-sub"><div align="right">
    <font color="#000000">Time Synchronization</font>    </div></td>
    <INPUT TYPE="HIDDEN" NAME="ToolsTimeSetFlag" VALUE="0">
    <% If tcWebApi_get("Timezone_Entry","TYPE","h") = "1" Then %>
    <INPUT TYPE="HIDDEN" NAME="uiRadioValue" VALUE="<%TCWebApi_get("Timezone_Entry","PCSyncFlag","s")%>">
    <% Else %>
    <INPUT TYPE="HIDDEN" NAME="uiRadioValue" VALUE="0">
    <%End If %>
    <INPUT TYPE="HIDDEN" NAME="uiClearPCSyncFlag" VALUE="0">
        <INPUT TYPE="HIDDEN" NAME="uiwPCdateMonth" VALUE="0">
        <INPUT TYPE="HIDDEN" NAME="uiwPCdateDay" VALUE="">
        <INPUT TYPE="HIDDEN" NAME="uiwPCdateYear" VALUE="">
        <INPUT TYPE="HIDDEN" NAME="uiwPCdateHour" VALUE="">
        <INPUT TYPE="HIDDEN" NAME="uiwPCdateMinute" VALUE="">
        <INPUT TYPE="HIDDEN" NAME="uiwPCdateSec" VALUE="">
        <INPUT TYPE="HIDDEN" NAME="uiCurTime" VALUE="<%TCWebApi_get("DeviceInfo","cur_time","s")%>">
        <INPUT TYPE="HIDDEN" NAME="uiTimezoneType" VALUE="<%TCWebApi_get("Timezone_Entry","TYPE","s")%>">
    <td class="light-orange">&nbsp;</td><td><hr noshade class="light-orange-line"></td><td><hr noshade class="light-orange-line"></td><td><hr noshade class="light-orange-line">
    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Synchronize time with</font>    </div></td><td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata"><INPUT NAME="uiViewSyncWith" TYPE="RADIO"  onclick="doTimeChange();doDisplay();" VALUE="0"  <% If TCWebApi_get("Timezone_Entry","TYPE","h") = "0" then asp_Write("checked") end if%>>
    <font color="#000000"  >NTP Server automatically</font></td></tr>

    <tr><td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata">&nbsp;</td><td class="tabdata">&nbsp;</td>
 
    <td class="tabdata"><INPUT TYPE="RADIO" NAME="uiViewSyncWith" VALUE="1" onClick="syncwithpc();doDisplay();"  <% If TCWebApi_get("Timezone_Entry","TYPE","h") = "1" then asp_Write("checked") end if%>>
        <font color="#000000">PC's Clock</font> 
 </td></tr>			


    <tr><td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata">&nbsp;</td><td class="tabdata">&nbsp;</td>
    
   <td class="tabdata"><INPUT TYPE="RADIO" NAME="uiViewSyncWith" VALUE="2"  onClick="doTimeChange();doDisplay();" <% If TCWebApi_get("Timezone_Entry","TYPE","h") = "2" then asp_Write("checked") end if%>><font color="#000000">Manually</font></td></tr>  
 </table>
<div id="timezone_manual_div2">
 <table width="760" border="0" align=center cellpadding="0" cellspacing="0">
     <tr>
    	<td width="150" height="30"  class="light-orange">&nbsp;</td><td width="10"  class="light-orange">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td>
     </tr>
    <tr>
	    <td class="light-orange">&nbsp;</td>
	    <td class="light-orange"></td>
	    <td class="tabdata"><div align="right">Date</div></td>
	    <td class="tabdata"><div align="center">:</div></td>
	    <td class="tabdata">
	        <select id="uiPCdateMonth"  NAME="uiPCdateMonth" SIZE="1">
	        <option VALUE=1>1
	        <option VALUE=2>2
	        <option VALUE=3>3
	        <option VALUE=4>4
	        <option VALUE=5>5
	        <option VALUE=6>6
	        <option VALUE=7>7
	        <option VALUE=8>8
	        <option VALUE=9>9
	        <option VALUE=10>10
	        <option VALUE=11>11
	        <option VALUE=12>12
	      </SELECT>
	      /
	      <input type="text" id="uiPCdateDay" name="uiPCdateDay" size="2" maxlength="2" value="">
	      /
	      <input type="text" id="uiPCdateYear" name="uiPCdateYear" size="4" maxlength="4" value="">
	        (Month/Date/Year)	    </td>
    </tr>
    <tr>
	    <td class="light-orange">&nbsp;</td>
	    <td class="light-orange"></td>
	    <td class="tabdata"><div align="right">
	    Time
	    </div></td>
	    <td class="tabdata"><div align="center">:</div></td>
	    <td class="tabdata">
	        <input type="text" id="uiPCdateHour" name="uiPCdateHour" size="2" maxlength="2" value="">
	        <input type="text" id="uiPCdateMinute" name="uiPCdateMinute" size="2" maxlength="2" value="">      
	        <input type="text" id="uiPCdateSec" name="uiPCdateSec" size="2" maxlength="2" value="">      
	        (hour:min:sec)	    </td>
    </tr>
	<script language="JavaScript" type="text/JavaScript">
	var strmonth = "<% TCWebApi_get("Timezone_Entry","Month","s") %>";
	var strdate = "<% TCWebApi_get("Timezone_Entry","Date","s") %>";
	var stryear = "<% TCWebApi_get("Timezone_Entry","Year","s") %>";
	var strhour = "<% TCWebApi_get("Timezone_Entry","Hour","s") %>";
	var strmin = "<% TCWebApi_get("Timezone_Entry","Min","s") %>";
	var strsec = "<% TCWebApi_get("Timezone_Entry","Sec","s") %>";
	
	if(strmonth != "N/A")
		document.Alpha_Time.uiPCdateMonth.selectedIndex = parseInt(strmonth) - 1;
	if(strdate != "N/A")
		document.Alpha_Time.uiPCdateDay.value = strdate;
	if(stryear != "N/A")
		document.Alpha_Time.uiPCdateYear.value = stryear;
	if(strhour != "N/A")
		document.Alpha_Time.uiPCdateHour.value = strhour;
	if(strmin != "N/A")
		document.Alpha_Time.uiPCdateMinute.value = strmin;
	if(strsec != "N/A")
		document.Alpha_Time.uiPCdateSec.value = strsec;
	</script>
</table>
</div>

<div id="timezone_ntp_div0">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" >
    <tr>
    	<td width="150" height="30"  class="light-orange">&nbsp;</td><td width="10"  class="light-orange">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td>
    </tr>
    <tr> 
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Time Zone</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
	<SELECT NAME="uiViewdateToolsTZ" SIZE="1">
	<option value="GMT-12:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-12:00" then asp_Write("selected") end if %>>(GMT-12:00) Enewetak, Kwajalein
	<option value="GMT-11:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-11:00" then asp_Write("selected") end if %>>(GMT-11:00) Midway Island, Samoa
	<option value="GMT-10:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-10:00" then asp_Write("selected") end if %>>(GMT-10:00) Hawaii
	<option value="GMT-09:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-09:00" then asp_Write("selected") end if %>>(GMT-09:00) Alaska
	<option value="GMT-08:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-08:00" then asp_Write("selected") end if %>>(GMT-08:00) Pacific Time (US,Canada)
	<option value="GMT-07:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-07:00" then asp_Write("selected") end if %>>(GMT-07:00) Mountain Time (US & Canada)
	<option value="GMT-06:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-06:00" then asp_Write("selected") end if %>>(GMT-06:00) Central Time (US & Canada), Maxico City, Saskatchewan
	<option value="GMT-05:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-05:00" then asp_Write("selected") end if %>>(GMT-05:00) Eastern Time (US & Canada), Indiana(East)
	<option value="GMT-04:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-04:00" then asp_Write("selected") end if %>>(GMT-04:00) Altlantic Time (Canada), Caracas, La Poz
	<option value="GMT-03:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-03:30" then asp_Write("selected") end if %>>(GMT-03:30) Newfoundland
	<option value="GMT-03:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-03:00" then asp_Write("selected") end if %>>(GMT-03:00) Brasilia, Buenos Aires, Georgetown
	<option value="GMT-02:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-02:00" then asp_Write("selected") end if %>>(GMT-02:00) Mid-Atlantic
	<option value="GMT-01:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT-01:00" then asp_Write("selected") end if %>>(GMT-01:00) Azores, Cape Verde Is
	<option value="GMT" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT" then asp_Write("selected") end if %>>(GMT) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London
	<option value="GMT+01:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+01:00" then asp_Write("selected") end if %>>(GMT+01:00) Berlin, Stockholm, Rome, Bern, Brussels, Vienna
	<option value="GMT+02:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+02:00" then asp_Write("selected") end if %>>(GMT+02:00) Athens, Helsinki, Istanbul, Cairo, Eastern Europe, Israel
	<option value="GMT+03:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+03:00" then asp_Write("selected") end if %>>(GMT+03:00) Baghdad, Kuwait, Nairobi, Riyadh, Moscow
	<option value="GMT+03:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+03:30" then asp_Write("selected") end if %>>(GMT+03:30) Tehran
	<option value="GMT+04:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+04:00" then asp_Write("selected") end if %>>(GMT+04:00) Abu Dhabi, Muscat, Tbilisi, Kazan, Volgograd
	<option value="GMT+04:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+04:30" then asp_Write("selected") end if %>>(GMT+04:30) Kabul
	<option value="GMT+05:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+05:00" then asp_Write("selected") end if %>>(GMT+05:00) Islamabad, Karachi, Ekaterinburg, Tashkent
	<option value="GMT+05:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+05:30" then asp_Write("selected") end if %>>(GMT+05:30) New Delhi
	<option value="GMT+06:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+06:00" then asp_Write("selected") end if %>>(GMT+06:00) Almaty, Dhaka
	<option value="GMT+06:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+06:30" then asp_Write("selected") end if %>>(GMT+06:30) Yangon(Rangoon)
	<option value="GMT+07:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+07:00" then asp_Write("selected") end if %>>(GMT+07:00) Bangkok, Jakarta, Hanoi
	<option value="GMT+08:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+08:00" then asp_Write("selected") end if %>>(GMT+08:00) Beijing, Hong Kong, Perth, Singapore, Taipei
	<option value="GMT+09:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+09:00" then asp_Write("selected") end if %>>(GMT+09:00) Tokyo, Osaka, Sapporo, Seoul, Yakutsk
	<option value="GMT+09:30" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+09:30" then asp_Write("selected") end if %>>(GMT+09:30) Adelaide, Darwin
	<option value="GMT+10:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+10:00" then asp_Write("selected") end if %>>(GMT+10:00) Brisbane, Canberra, Melbourne, Sydney, Hobart
	<option value="GMT+11:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+11:00" then asp_Write("selected") end if %>>(GMT+11:00) Magadan, Solomon Is., New Caledonia
	<option value="GMT+12:00" <% if TCWebApi_get("Timezone_Entry","TZ","h") = "GMT+12:00" then asp_Write("selected") end if %>>(GMT+12:00) Fiji, Kamchatka, Marshall Is., Wellington, Auckland
	</SELECT> 
		</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Daylight Saving</font>  </div>  <td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    	<INPUT TYPE="RADIO" NAME="uiViewdateDS" VALUE="Enable"  <% If TCWebApi_get("Timezone_Entry","DAYLIGHT","h") = "Enable" then asp_Write("checked") end if%>   >     <font color="#000000">Enabled</font>        
    	<INPUT TYPE="RADIO" NAME="uiViewdateDS" VALUE="Disable" <% If TCWebApi_get("Timezone_Entry","DAYLIGHT","h") = "Disable" then asp_Write("checked") end if%> >    <font color="#000000">Disabled</font>    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">NTP Server Address</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    	<INPUT TYPE="TEXT" NAME="uiViewSNTPServer" SIZE="24" MAXLENGTH="48" VALUE="<%TCWebApi_get("Timezone_Entry","SERVER","s")%>" >        <font color="#000000">(0.0.0.0: Default Value)</font>    	</td></tr>
	
	<% if TCWebApi_get("WebCustom_Entry","is2NTPServerSupported","h") = "Yes" then%>	
		<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">NTP Server2 Address</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    	<INPUT TYPE="TEXT" NAME="uiViewSNTPServer2" SIZE="24" MAXLENGTH="48" VALUE="<%TCWebApi_get("Timezone_Entry","SERVER2","s")%>" >        <font color="#000000">(0.0.0.0: Default Value)</font>    	</td></tr>
	
	<%elseif TCWebApi_get("WebCustom_Entry","is3NTPServerSupported","h") = "Yes" then%>
		<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">NTP Server2 Address</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    	<INPUT TYPE="TEXT" NAME="uiViewSNTPServer2" SIZE="24" MAXLENGTH="48" VALUE="<%TCWebApi_get("Timezone_Entry","SERVER2","s")%>" >        <font color="#000000">(0.0.0.0: Default Value)</font>    	</td></tr>
		<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">NTP Server3 Address</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
    	<INPUT TYPE="TEXT" NAME="uiViewSNTPServer3" SIZE="24" MAXLENGTH="48" VALUE="<%TCWebApi_get("Timezone_Entry","SERVER3","s")%>" >        <font color="#000000">(0.0.0.0: Default Value)</font>    	</td></tr>
  	<%end if%>
</table>
</div>

<table width="760" border="0" align=center cellpadding="0" cellspacing="0" >
    <tr>
    	<td width="150" height="30"  class="light-orange">&nbsp;</td><td width="10"  class="light-orange">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td>
    </tr>
</table>

<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
        <INPUT TYPE="BUTTON" NAME="SaveBtn" VALUE="SAVE" onClick="uiTimeSave()">
        <INPUT TYPE="BUTTON" NAME="CancelBtn" VALUE="CANCEL" onClick="uiDoCancel()">
		<INPUT TYPE="HIDDEN" NAME="ntp2ServerFlag" VALUE="<%tcWebApi_get("WebCustom_Entry","is2NTPServerSupported","s")%>">
		<INPUT TYPE="HIDDEN" NAME="ntp3ServerFlag" VALUE="<%tcWebApi_get("WebCustom_Entry","is3NTPServerSupported","s")%>"></td></tr></table></form>
</body></html>
