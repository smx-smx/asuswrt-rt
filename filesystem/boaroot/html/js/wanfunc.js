// Error Check for WAN Page
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


function valDoValidateIntegerWAN(Integer)
{   
   if(Integer.match("^[0-9]+")) 
   { 
   	return true;
   }
   alert("An integer can only have digits");
   return false;
}

/* PVC :VPI:0-255 VCI:1-65535*/
function pvcDoValidatePageWAN()
{
  var value;

  value = document.Alpha_WAN.Alwan_VPI.value;
  if(!valDoValidateIntegerWAN(value)) 
  { 
  	return false;
  }
  else if(Number(value) > 255 || Number(value)<0) 
  { 
  	alert("VPI must be in the range 0-255"); 
  	return false; 
  }
  value = document.Alpha_WAN.Alwan_VCI.value;
  if(!valDoValidateIntegerWAN(value)) 
  { 
  	return false; 
  }
  else if(Number(value) > 65535 || Number(value)<1) 
  { 
  	alert("VCI must be in the range 1-65535"); 
  	return false; 
  }  
  return true;
}

function staticDoValidatePageWAN()
{
	var message;
	var value;
	var count=0;
	
	value = document.Alpha_WAN.wan_StaticIPaddr.value;
	message = valDoValidateIP(value);
	if(message!=null) {return false; erlDoAddError("bad","wan_StaticIPaddr","",value,message); count++; }
	return true;
}		

function pppDoValidatePageWAN()
{
	var message;
	var value;
	var count=0;
	
	value = document.Alpha_WAN.wan_PPPUsername.value;
	message = valDoValidateUsrPwd(value,'1');
	if(message!=null) { return false; erlDoAddError("bad","wan_PPPUsername","",value,message); count++; }
	
	value = document.Alpha_WAN.wan_PPPPassword.value;
	message = valDoValidateUsrPwd(value,'0');
	if(message!=null) { return false; erlDoAddError("bad","wan_PPPPassword","",value,message);  count++; }
	if(count!=0) message="PPP";
	else message="";
	return true;
}
function QoSDoValidatePageWAN()
{
  var value;
  var value1;
  
  switch(document.Alpha_WAN.Alwan_QoS.selectedIndex)
  {
    case 0:
    case 1:
                value = document.Alpha_WAN.wan_PCR.value;
                if(!isNumeric(value))
                {
                	alert("PCR must be Interger");
                	return false; 
                }
                else
                {
                	pcr = parseInt(value);
                	if(pcr > 5500 || pcr < 0)
                	{
                		alert("PCR Range: 0 <= PCR <= 5500");
                		return false;
                	}
                }
	 break;

    case 2:
    case 3:
                value = document.Alpha_WAN.wan_PCR.value;
                if(!isNumeric(value))
                {
                	alert("PCR must be Interger");
                	return false; 
                }
                else
                {
                	pcr = parseInt(value);
                	if(pcr > 5500 || pcr < 0)
                	{
                		alert("PCR Range: 0 <= PCR <= 5500");
                		return false;
                	}
                }
                value1 = document.Alpha_WAN.wan_SCR.value;            
                if(!isNumeric(value1))
                {
                	alert("SCR must be Interger");
                	return false; 
                }
                else if(Number(value1) > Number(value)) 
                { 
                	alert("SCR must be less than or equal to PCR"); 
                	return false; 
                }
                value2 = document.Alpha_WAN.wan_MBS.value
                if(!isNumeric(value2))
                {
                	alert("MBS must be Interger");
                	return false;
                }
                else
                {
                	mbs = parseInt(value2);
					if(mbs > 65535 || mbs < 0)
					{
						alert("MBS Range: 0 <= MBS <= 65535");
						return false;
					}
                }
	 break;

     default:
	 break;
  }
  return true;
 }


