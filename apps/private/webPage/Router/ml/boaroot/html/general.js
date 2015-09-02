/******************For Multi-Language Strings Start*************************/
var	vstrings = parent.ary_strings;
function DictionaryStrings()
{
	this.dicObj = {};
	
	if(typeof(vstrings) == 'undefined')
	{
		alert("Load strings failure for js!!");
		return;
	}
	for (var i=0; vstrings[i][0] != ''; i++)
	{
		this.dicObj[vstrings[i][0]] = vstrings[i][1];
	}
	
	/***********external method************/
	this.getstring = function(key){
		if (typeof(this.dicObj[key]) != 'undefined')
			return this.dicObj[key];
		else{
			alert(key + ":has not be defined in language string array!");
			return "";
		}
	}
}
var	langtxt = new DictionaryStrings();
window._ = function(key) {return langtxt.getstring(key);}
/******************End*************************/
function doValidIPAndMask(Address, Mask, Where)
{
	if((Address == "") && (Mask != ""))
	{
		if(Where == 1){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
           		alert(_("DesIPInvalid")+Address);
    	<!-- RpEnd -->   
		}else if(Where == 2){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
				alert(_("SorIPInvalid"));
		<!-- RpEnd -->
		}else {
   		<!-- RpNamedDisplayText Name="JavaScriptErrMsg0" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg0" RpTextType="ASCII" -->	  	  	         		       
        		alert(_("IPIsEmpty"));
    	<!-- RpEnd -->
		}	    	        				         
        return false; 
	}
	else if((Address != "") && (Mask == ""))
	{
		if(Where == 1){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg77" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg77" RpTextType="ASCII" -->	  	  	         		       
				 alert(_("DesNetInvalid"));
		 <!-- RpEnd -->
		}else if(Where == 2){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg78" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg78" RpTextType="ASCII" -->	  	  	         		       
				 alert(_("SorNetInvalid"));
		<!-- RpEnd -->
		}else{
   		<!-- RpNamedDisplayText Name="JavaScriptErrMsg44" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg44" RpTextType="ASCII" -->  	                 
	  			alert(_("SubMaskInvalid") + Mask);
    	<!-- RpEnd --> 
		}     	  
	  	return false;
	}
	else
		return true;
} 

function chineseCheck(object, objectId)
{
	var obj = document.getElementById(objectId);
	var objName = obj.innerText;
	var inputStr = object.value;
	var i;
	
	if(objName == undefined)
		objName = "Warning : input";
	for(i = 0; i < inputStr.length; i++)
	{
		if(inputStr.charCodeAt(i) < 0 || inputStr.charCodeAt(i) > 255)
		{
		<!-- RpNamedDisplayText Name="chineseCheckText" RpGetType=Function RpGetPtr="RpGet_chineseCheckText" RpTextType="ASCII" -->
			alert(objName + _("ContentCHN"));
		<!-- RpEnd -->
			return true;
		}
	} 
	return false;
}
function isValidAscii(val)
{
    for ( var i = 0 ; i < val.length ; i++ )
    {
        var ch = val.charAt(i);
        if ( ch < ' ' || ch > '~' )
        {
            return ch;
        }
    }
    return '';
}
function isNumeric(s)
{
  var len= s.length;
  var ch;
  if(len==0)
    return false;
  for( i=0; i< len; i++)
  {
    ch= s.charAt(i);
    if( ch > '9' || ch < '0')
    {
      return false;
    }
  }
  return true;
}
function isValidUrlName(url)
{
	var i=0;
	var invalidArray = new Array();
	invalidArray[i++] = "www";
	invalidArray[i++] = "com";
	invalidArray[i++] = "org";
	invalidArray[i++] = "net";
	invalidArray[i++] = "edu";
	invalidArray[i++] = "www.";
	invalidArray[i++] = ".com";
	invalidArray[i++] = ".org";
	invalidArray[i++] = ".net";
	invalidArray[i++] = ".edu";
	if (isValidAscii(url) != '')
	{
		return false;
	}
	for (i = 0; i < url.length; i++)
	{
		if (url.charAt(i) == '\\' || url.charAt(i) ==' ' || url.charAt(i)=='#' 
		|| url.charAt(i)=='@' || url.charAt(i)==',' || url.charAt(i)=='%' )
		{
			return false;
		} 
	}
	if (url == "")
	{
		return false;
	}
	if (url.length < 3)
	{
		return false;
	}
	for(j=0; j< invalidArray.length; j++)
	{
		if (url == invalidArray[j])
		{
			return false;
		}
	}
	return true;
}
