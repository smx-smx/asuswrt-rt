function valDoValidateName(Name,Result)
{
   if(Name.match("^[0-9]") != null) { alert("A Username/Password cannot start with a number"); return false; return("A name can not start with a number."); }
   if(Name.match("[^0-9a-zA-Z_-]") != null) { alert("A Username/Password name can only have alpha-numeric characters"); return false; return("A name can only have alpha-numeric characters"); }
   else if (Name.length == 0) { alert("A Username/Password must contain at least one character"); return false; return("A name must contain at least one character"); }
   return null;
}

function valDoValidateUsrPwd(Name,val,Result)
{
	if(Name.match("[^@.*0-9a-zA-Z_-]") != null) 
	{ 
		alert("An Username/Password  can only have alpha-numeric characters"); 
		return false; 
		return("A name can only have alpha-numeric characters"); 
	}
	
	var fstchr=Name.split("");
	if(fstchr[0]=="@"||fstchr[0]=="-"||fstchr[0]==".") 
	{ 
		alert("An Username/Password can start only with  alpha-numeric characters");
		return false; 
		return("A name can only start with alpha-numeric characters"); 
	}
	
	if(val==1)
	{
		for(var i=0;i<(Name.length)-1;i++)
		{
			if((fstchr[i]=="@" && fstchr[i+1]==".")||(fstchr[i]=="." && fstchr[i+1]=="@")) 
			{ 
				alert("Invalid Username");
				return false;
				return("Invalid Username"); 
			}
		}
	}
   return null;
}


function valDoValidateInteger(Integer,Result)
{   
   if(Integer.match("^[0-9]+")) 
   { 
   	return null;
   }
   alert('An integer can only have digits');
   return false;
   return("An integer can only have digits");
}



function isValidIpAddr(ip1,ip2,ip3,ip4)
{
if(ip1==0 || ip4==0 || ip4==255 || ip1==127 )	
	return false;			

return true;	
}

function valDoValidateIP(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;
  var error=null;

  if(address == null) { 
  	alert('Invalid IP address');
  	error="Invalid IP address"; 
  	return false;
  }
  else
  {
    digits = address[0].split(".");
    for(i=0; i < 4; i++)
    {
      if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
      { 
      	alert('Invalid IP address');
      	error="Invalid IP address"; 
      	return false;
      	break; 
      }
    }
    if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3],false))
    {
    	alert("Invalid IP address");
    	return false;
    }
  }
  return error;
}

function valDoValidateIPDNS(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;
  var error=null;

  if(address == null) { 
  	alert('Invalid IP address');
  	error="Invalid IP address"; 
  	return false;
  }
  else
  {
    digits = address[0].split(".");
    for(i=0; i < 4; i++)
    {
      if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
      { 
      	alert('Invalid IP address');
      	error="Invalid IP address"; 
      	return false;
      	break; 
      }
    }
  }
  return error;
}


function valDoValidateNet(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var error=null;

  if(address == null) 
  { 
	alert('Invalid network ID');
	return false;
  	error="Invalid network ID"; 
  	return error; 
  }
  if(address[0] == "255.255.255.255")
  { 
	alert('Invalid network ID');
	return false;
	error="Invalid network ID"; return error; 
  }
  digits = address[0].split(".");
  for(i=1; i <= 4; i++)
   if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
    { 
    	alert('Invalid network ID');
    	error="Invalid network ID"; 
    	return false;
    	break; 
    }
   return error;
}

function valDoValidateMask(Mask)
{
  var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var bMask = 0;
  var watch = false;
  var i;
  var error=null;

  if(mask == null)
  { 
	  alert('Invalid network mask');
	  error="Invalid network mask"; 
	  return false;
	  return error; 
 }
  digits = mask[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
     {
	     alert('Invalid network mask');
	     error="Invalid network mask"; 
	     return false;	
	     return error; 
     }
	bMask = (bMask << 8) | Number(digits[i]);
  }
  bMask = bMask & 0x0FFFFFFFF;
  for(i=0; i<32; i++)
  {
    if((watch==true) && ((bMask & 0x1)==0)) { 
    alert('Invalid network mask');
    error="Invalid network mask"; 
    return false;
    return error; 
  }
	bMask = bMask >> 1;
	if((bMask & 0x01) == 1) watch=true;
  }
  return error;
}

function valDoValidateHostName(HostName)
{
   if (HostName.match("^[a-zA-Z]+") == null) 
   {
   	alert('Host Name must start with a character');
   	return false;
   	return ("Host Name must start with a character");
   }	   	
   else if (HostName.match("^([a-zA-Z]+[a-zA-Z0-9_-]*)(\.*[a-zA-Z0-9])+$") == null) 
   {
   	alert('Host Name does not have correct format')
   	return false;
   	return ("Host Name does not have correct format");
   } 	   	
   else if ((HostName.length == 0) || (HostName.length > 63))
   { 
   	alert('A host name must contain 1..63 characters');
   	return false;
   	return("A host name must contain 1..63 characters"); 
   }
   return null;
}

function pppDoValidatePage()
{
	var message;
	var value;
	var count=0;
	
	value = document.getElementById("Al_PPPUsername").value;
	message = valDoValidateUsrPwd(value,'1');
	if(message!=null) { return false; erlDoAddError("bad","Al_PPPUsername","",value,message); count++; }
	
	value = document.getElementById("Al_PPPPassword").value;
	message = valDoValidateUsrPwd(value,'0');
	if(message!=null) { return false; erlDoAddError("bad","Al_PPPPassword","",value,message);  count++; }
	if(count!=0) message="PPP";
	else message="";
	return true;
}

function unValidIP(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { 
  	alert('Invalid IP address');
  	return true;
  }
  else
  {
    digits = address[0].split(".");
    for(i=0; i < 4; i++)
    {
      if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
      { 
      	alert('Invalid IP address');
      	error="Invalid IP address"; 
      	return true;
      	break; 
      }
    }
    if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3],false))	{
    	alert("Invalid IP address");
    	return true;
    }
	return false;
  }
}

function unValidMask(Mask)
{
  var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var bMask = 0;
  var watch = false;
  var i;

  if(mask == null)
  { 
	  alert('Invalid network mask');
	  return true;
  }
  digits = mask[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
     {
	     alert('Invalid network mask');
	     return true;	
     }
	bMask = (bMask << 8) | Number(digits[i]);
  }
  bMask = bMask & 0x0FFFFFFFF;
  for(i=0; i<32; i++)
  {
    if((watch==true) && ((bMask & 0x1)==0)) { 
    alert('Invalid network mask');
    return true;
  }
	bMask = bMask >> 1;
	if((bMask & 0x01) == 1) watch=true;
  }
  return false;
}

function staticDoValidatePage()
{
	var message;
	var value;
	var count=0;
	
	value = document.getElementById("Al_IPaddr").value;
	message = valDoValidateIP(value);
	if(message!=null) {return false; erlDoAddError("bad","Al_IPaddr","",value,message); count++; }
	return true;
}		
function getElementById(sId)
{
	if (document.getElementById)
	{
		return document.getElementById(sId);	
	}
	else if (document.all)
	{
		// old IE
		return document.all(sId);
	}
	else if (document.layers)
	{
		// Netscape 4
		return document.layers[sId];
	}
	else
	{
		return null;
	}
}

/*getElByName*/
function getElementByName(sId)
{    // standard
	if (document.getElementsByName)
	{
		var element = document.getElementsByName(sId);
		
		if (element.length == 0)
		{
			return null;
		}
		else if (element.length == 1)
		{
			return 	element[0];
		}
		
		return element;		
	}
}
function getElement(sId)
{
	var ele = getElementByName(sId);
	if (ele == null)
	{
		return getElementById(sId);
	}
	return ele;
}
function setDisplay (sId, sh)
{
    var status;
    if (sh > 0) 
    {
        status = "";
    }
    else 
    {
        status = "none";
    }
    getElement(sId).style.display = status;
}
