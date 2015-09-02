function doValidIPAndMask(Address, Mask, Where)
{
	if((Address == "") && (Mask != ""))
	{
		if(Where == 1){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
           		alert("Invalid destination IP address: "+Address);
    	<!-- RpEnd -->   
		}else if(Where == 2){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
				alert("Invalid Source IP Address!");
		<!-- RpEnd -->
		}else {
   		<!-- RpNamedDisplayText Name="JavaScriptErrMsg0" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg0" RpTextType="ASCII" -->	  	  	         		       
        		alert("IP address is empty!");
    	<!-- RpEnd -->
		}	    	        				         
        return false; 
	}
	else if((Address != "") && (Mask == ""))
	{
		if(Where == 1){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg77" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg77" RpTextType="ASCII" -->	  	  	         		       
				 alert("Invalid Destination network mask!");
		 <!-- RpEnd -->
		}else if(Where == 2){
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg78" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg78" RpTextType="ASCII" -->	  	  	         		       
				 alert("Invalid Source network mask!");
		<!-- RpEnd -->
		}else{
   		<!-- RpNamedDisplayText Name="JavaScriptErrMsg44" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg44" RpTextType="ASCII" -->  	                 
	  			alert("Invalid subnet mask: " + Mask);
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
			alert(objName + "can not contain chinese!!");
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