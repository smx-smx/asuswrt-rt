var winH,winW;
var hideInitialLoading = "";
function winW_H(){
if(parseInt(navigator.appVersion) > 3){
winW = document.documentElement.scrollWidth;
if(document.documentElement.clientHeight > document.documentElement.scrollHeight)
winH = document.documentElement.clientHeight;
else
winH = document.documentElement.scrollHeight;
}
}
function LoadingTime(seconds, flag){
	showtext($("proceeding_main_txt"), "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...");
	$("Loading").style.visibility = "visible";
	y = y+progress;
	if(typeof(seconds) == "number" && seconds >= 0){
		if(seconds != 0){
			if(flag == "upgrade_FAIL")
			{
				showtext($("proceeding_main_txt"), "Invalid firmware, now rebooting.<br><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> ");
				hideInitialLoading = "1";
			}
			else if(flag == "upgrade_SAME")
			{
				showtext($("proceeding_main_txt"), "Identical firmware, now rebooting.<br><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> ");
				hideInitialLoading = "1";
			}
			else if(flag == "upgrade_FLASH_NOT_SUPPORT")
			{
				showtext($("proceeding_main_txt"), "Invalid firmware, this firmware does not support current hardware(flash).<br><br>Please upgrade with latest firmware, now rebooting.<br><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> ");
				hideInitialLoading = "1";
			}
			else if(flag == "upgrade_FAIL_ROMFILE")
			{
				showtext($("proceeding_main_txt"), "Upload setting failed, now rebooting.<br><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> ");
				hideInitialLoading = "1";
			}
			else
				showtext($("proceeding_main_txt"), "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> ");

			if(Math.round(y) > 100)
				showtext($("proceeding_txt"), "100% <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>");
			else
				showtext($("proceeding_txt"), Math.round(y)+"% <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>");
			--seconds;
			
			if(flag != "upgrade_FAIL" && flag != "upgrade_SAME" && flag != "upgrade_FAIL_ROMFILE" && flag != "upgrade_FLASH_NOT_SUPPORT" && hideInitialLoading == "1"){
				//skip setTimeout();
			}
			else
				setTimeout("LoadingTime("+seconds+", '"+flag+"');", 1000);
		}
		else{
			showtext($("proceeding_main_txt"), translate("<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>"));
			showtext($("proceeding_txt"), "");
			y = 0;
			if(flag != "waiting")
				setTimeout("hideLoading();",1000);
		}
	}
}
function LoadingProgress(seconds){
$("LoadingBar").style.visibility = "visible";
y = y + progress;
if(typeof(seconds) == "number" && seconds >= 0){
if(seconds != 0){
$("proceeding_img").style.width = Math.round(y) + "%";
$("proceeding_img_text").innerHTML = Math.round(y) + "%";
if($("loading_block1")){
	$("proceeding_img_text").style.width = document.getElementById("loading_block1").clientWidth;
	$("proceeding_img_text").style.marginLeft = "175px";
}	
--seconds;
setTimeout("LoadingProgress("+seconds+");", 1000);
}
else{
			$("proceeding_img_text").innerHTML = "<%tcWebApi_get("String_Entry","Main_alert_proceeding_desc3","s")%>";
y = 0;
if(location.pathname.indexOf("QIS_wizard.asp") < 0){
setTimeout("hideLoadingBar();",1000);
location.href = "index.asp";
}
}
}
}
function showLoading(seconds, flag){
	if(window.scrollTo)
		window.scrollTo(0,0);

	disableCheckChangedStatus();
	
	htmlbodyforIE = document.getElementsByTagName("html");  //this both for IE&FF, use "html" but not "body" because <!DOCTYPE html PUBLIC.......>
	htmlbodyforIE[0].style.overflow = "hidden";	  //hidden the Y-scrollbar for preventing from user scroll it.
	
	winW_H();
	
	var blockmarginTop;
	var blockmarginLeft;
	if (window.innerWidth)
		winWidth = window.innerWidth;
	else if ((document.body) && (document.body.clientWidth))
		winWidth = document.body.clientWidth;
	
	if (window.innerHeight)
		winHeight = window.innerHeight;
	else if ((document.body) && (document.body.clientHeight))
		winHeight = document.body.clientHeight;
	
	if (document.documentElement  && document.documentElement.clientHeight && document.documentElement.clientWidth){
		winHeight = document.documentElement.clientHeight;
		winWidth = document.documentElement.clientWidth;
	}

	if(winWidth >1050){
	
		winPadding = (winWidth-1050)/2;	
		winWidth = 1105;
		blockmarginLeft= (winWidth*0.3)+winPadding;
	}
	else if(winWidth <=1050){
		blockmarginLeft= (winWidth)*0.3+document.body.scrollLeft;	

	}
	
	if(winHeight >660)
		winHeight = 660;
	
	blockmarginTop= winHeight*0.3	
	
	$("loadingBlock").style.marginTop = blockmarginTop+"px";
	$("loadingBlock").style.marginLeft = blockmarginLeft+"px";

	$("Loading").style.width = winW+"px";
	$("Loading").style.height = winH+"px";
	
	loadingSeconds = seconds;
	progress = 100/loadingSeconds;
	y = 0;
	
	LoadingTime(seconds, flag);
}

function showLoadingBar(seconds){
	if(window.scrollTo)
		window.scrollTo(0,0);

	disableCheckChangedStatus();
	
	htmlbodyforIE = document.getElementsByTagName("html");  //this both for IE&FF, use "html" but not "body" because <!DOCTYPE html PUBLIC.......>
	htmlbodyforIE[0].style.overflow = "hidden";	  //hidden the Y-scrollbar for preventing from user scroll it.
	
	winW_H();

	var blockmarginTop;
	var blockmarginLeft;
	if (window.innerWidth)
		winWidth = window.innerWidth;
	else if ((document.body) && (document.body.clientWidth))
		winWidth = document.body.clientWidth;
	
	if (window.innerHeight)
		winHeight = window.innerHeight;
	else if ((document.body) && (document.body.clientHeight))
		winHeight = document.body.clientHeight;

	if (document.documentElement  && document.documentElement.clientHeight && document.documentElement.clientWidth){
		winHeight = document.documentElement.clientHeight;
		winWidth = document.documentElement.clientWidth;
	}

	if(winWidth >1050){
	
		winPadding = (winWidth-1050)/2;	
		winWidth = 1105;
		blockmarginLeft= (winWidth*0.35)+winPadding;
	}
	else if(winWidth <=1050){
		blockmarginLeft= (winWidth)*0.35+document.body.scrollLeft;	

	}
	
	if(winHeight >660)
		winHeight = 660;
	
	blockmarginTop= winHeight*0.3			
	
	$("loadingBarBlock").style.marginTop = blockmarginTop+"px";
	// marked by Jerry 2012.11.14 using CSS to decide the margin
	$("loadingBarBlock").style.marginLeft = blockmarginLeft+"px";

	
	/*blockmarginTop = document.documentElement.scrollTop + 200;
	$("loadingBarBlock").style.marginTop = blockmarginTop+"px";*/

	$("LoadingBar").style.width = winW+"px";
	$("LoadingBar").style.height = winH+"px";
	
	loadingSeconds = seconds;
	progress = 100/loadingSeconds;
	y = 0;
	LoadingProgress(seconds);
}
function hideLoadingBar(){
$("LoadingBar").style.visibility = "hidden";
}
function hideLoading(flag){
$("Loading").style.visibility = "hidden";
htmlbodyforIE = document.getElementsByTagName("html"); //this both for IE&FF, use "html" but not "body" because <!DOCTYPE html PUBLIC.......>
htmlbodyforIE[0].style.overflow = ""; //hidden the Y-scrollbar for preventing from user scroll it.
}
function simpleSSID(obj){
var SSID = document.loginform.wl_ssid.value;
if(SSID.length < 16)
showtext(obj, SSID);
else{
obj.title = SSID;
showtext(obj, SSID.substring(0, 16)+"...");
}
}
function dr_advise(){
disableCheckChangedStatus();
htmlbodyforIE = document.getElementsByTagName("html"); //this both for IE&FF, use "html" but not "body" because <!DOCTYPE html PUBLIC.......>
htmlbodyforIE[0].style.overflow = "hidden"; //hidden the Y-scrollbar for preventing from user scroll it.
winW_H();
var blockmarginTop;
blockmarginTop = document.documentElement.scrollTop + 200;
$("dr_sweet_advise").style.marginTop = blockmarginTop+"px"
$("hiddenMask").style.width = winW+"px";
$("hiddenMask").style.height = winH+"px";
$("hiddenMask").style.visibility = "visible";
}

