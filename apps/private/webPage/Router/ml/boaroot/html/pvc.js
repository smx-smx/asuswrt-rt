
var pvcPageErrors = new Array();
var pvcPageErrorCount = 0;
function pvcDoOnLoad()
{
  jslSetValue("uiViewPvcVpi","uiPostPvcVpi");
  jslSetValue("uiViewPvcVci","uiPostPvcVci");
}
function pvcIndexOnLoad(arg)
{
  document.getElementById("uiViewWanConn").selectedIndex = parseInt(arg.charAt(10));
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

function QoSDoOnLoad()
{

  document.getElementById("uiPostPvcCbrPcr").value = parseInt(document.getElementById("uiPostPvcCbrPcr").value)/1024;
  document.getElementById("uiPostPvcVbrPcr").value = parseInt(document.getElementById("uiPostPvcVbrPcr").value)/1024;
  document.getElementById("uiPostPvcVbrScr").value = parseInt(document.getElementById("uiPostPvcVbrScr").value)/1024;	
 
  switch(document.getElementById("uiPostPvcQoS").value)
  {
    case "CBR":
      jslSetValue("uiViewPvcPcr","uiPostPvcCbrPcr");
	  document.getElementById("uiViewPvcScr").value="";
    break;
    case "VBR":
      jslSetValue("uiViewPvcPcr","uiPostPvcVbrPcr");
      jslSetValue("uiViewPvcScr","uiPostPvcVbrScr");
    break;
    case "UBR":
    default:
	  document.getElementById("uiViewPvcPcr").value="";
	  document.getElementById("uiViewPvcScr").value="";
	break;
  }
  pvcDoSetQoS(document.getElementById("uiPostPvcQoS").value);
  
}
function pvcDoSave()
{
  jslSetValue("uiPostPvcVpi","uiViewPvcVpi");
  jslSetValue("uiPostPvcVci","uiViewPvcVci");
}
function QoSDoSave()
{
  var message;

  jslSetValue("uiPostPvcQoS","uiViewQosS");
  switch(document.getElementById("uiPostPvcQoS").value)
  {
    case "CBR":
      jslSetValue("uiPostPvcCbrPcr","uiViewPvcPcr");
    break;
    case "VBR":
      jslSetValue("uiPostPvcVbrPcr","uiViewPvcPcr");
      jslSetValue("uiPostPvcVbrScr","uiViewPvcScr");
    break;
    case "UBR":
    default:
	break;
  }
  	document.getElementById("uiPostPvcCbrPcr").value = parseInt(document.getElementById("uiPostPvcCbrPcr").value)*1024;
	document.getElementById("uiPostPvcVbrPcr").value = parseInt(document.getElementById("uiPostPvcVbrPcr").value)*1024;		
	document.getElementById("uiPostPvcVbrScr").value = parseInt(document.getElementById("uiPostPvcVbrScr").value)*1024;	

}

function pvcDoSetQoS(arg)
{
  var index = 0;
  switch(arg)
  {
    case "CBR":
      index = 1;
	  jslEnable("uiViewPvcPcr");
	  jslDisable("uiViewPvcScr");
    break;
    case "VBR":
      index = 2;
	  jslEnable("uiViewPvcPcr");
	  jslEnable("uiViewPvcScr");
    break;
    case "UBR":
    default:
      index = 0;
	  jslDisable("uiViewPvcPcr");
	  jslDisable("uiViewPvcScr");
	break;
  }
  document.getElementById("uiViewQosS").selectedIndex=index;
}

/* PVC :VPI:0-255 VCI:0-65535*/
function pvcDoValidatePage()
{
  var message;
  var value;
  var pvcPageErrorCount=0;

  value = document.getElementById("uiViewPvcVpi").value;
  message = valDoValidateInteger(value);
  if(message!=null) 
  { 
  	return false;
  	erlDoAddError("bad","uiViewPvcVpi","",value,message); 
  	pvcPageErrorCount++; 
  }
  else if(Number(value) > 255 || Number(value)<0) 
  { 
	alert(_("vpiRange"));
  	return false; 
  	erlDoAddError("bad","uiViewPvcVpi","",value,"VPI must be in the range 0-255"); 
  	pvcPageErrorCount++ ;
  }


  value = document.getElementById("uiViewPvcVci").value;
  message = valDoValidateInteger(value);
  if(message!=null) 
  { 
  	return false; 
  	erlDoAddError("bad","uiViewPvcVci","",value,message); 
  	pvcPageErrorCount++; 
  }
  else if(Number(value) > 65535 || Number(value)<0) 
  { 
	alert(_("vciRange")); 
  	return false; 
  	erlDoAddError("bad","uiViewPvcVci","",value,"VCI must be in the range 0-65535"); 
  	pvcPageErrorCount++; 
  }  

  if(pvcPageErrorCount!=0) message="PVC";
  else message="";
  return true;
}


/* QoS: UBR CBR VBR*/

function QoSDoValidatePage()
{
  var message;
  var value;
  var pvcPageErrorCount=0;
  value = document.getElementById("uiPostPvcQoS").value;
  switch(value)
  {
    case "UBR":
	 break;
    case "CBR":
                value = document.getElementById("uiViewPvcPcr").value;
                if(!isNumeric(value))
                {
					alert(_("PCRInvalid"));	
                	pvcPageErrorCount++; 
                	return false; 
                }
                else if(Number(value) > 63) { alert(_("pcrRange")); return false; erlDoAddError("bad","uiViewPvcPcr","",value,"PCR must be less than 64"); pvcPageErrorCount++; }
	 break;

    case "VBR":
                value = document.getElementById("uiViewPvcPcr").value;
                if(!isNumeric(value))
                {
					alert(_("PCRInvalid"));
                	pvcPageErrorCount++; 
                	return false; 
                }
                else if(Number(value) > 63) { alert(_("pcrRange")); return false; erlDoAddError("bad","uiViewPvcPcr","",value,"PCR must be less than 64"); pvcPageErrorCount++; }

               value = document.getElementById("uiViewPvcScr").value;
                if(!isNumeric(value))
                {
					alert(_("SCRInvalid"));	
                	pvcPageErrorCount++; 
                	return false; 
                }
               else if(Number(value) > 63) { alert(_("pcrRange")); return false; erlDoAddError("bad","uiViewPvcScr","",value,"SCR must be less than 64"); pvcPageErrorCount++; }
	 break;

     default:
	 break;
  }

  if(pvcPageErrorCount!=0) message="QOS";
  else message="";
  return true;
  
}
	