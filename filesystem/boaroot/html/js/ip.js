
function Isempty(Arg)
{
	var len=Arg.length;
	if(Arg=="" || len==0)
	{
		alert("Should be filled");
		return false;
	}
	else
		return true;
}


function isValidIpAddr(ip1,ip2,ip3,ip4)
{
if(ip1==0 || ip4==0 || ip4==255 || ip1==127)	
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
  return true;
}

function valDoValidateIP0000(addr, hidden, ACL_active)
{
	if(hidden == 0 && ACL_active.checked == false)
	{
		if(addr == "0.0.0.0")
			return true;
		else
			return valDoValidateIP(addr);
	}
}

function doValidateIpBetween(Address, ipstart, ipend)
{
  	
  	var digits=ipstart.split(".");
  	var digite=ipend.split(".");
	
	var digit=Address.split(".");
	
	var inrange=1;
	for ( var i=0;i<4;i++)
	{
		if( ( parseInt(digit[i]) < parseInt(digits[i]) ) || ( parseInt(digit[i]) > parseInt(digite[i]) ) )
		{
			inrange=0;
			break;
		}
		
	}
	if( inrange==0 )
	{ 
		alert("Invalid IP, range:"+ipstart+"-"+ipend);
		return false;
	}
	else
		return true;
}


function Ischar(Name)
{

	var name=Name.split("");
	
	if( name[0]!='_'  && (name[0] >='A' && name[0] <= 'z') )
	{
		alert(name[0]);
		return true;
	}
 	else
	{
		alert('Should start with Character');
		return false;
	}

}



function Isstartchar(Name)
{

	var name=Name.match("^[0-9 a-z A-Z]$");

	if(name==null)
	{
		alert('Start with character');
		return false;
	}
	if(name[0] >='0' && name[0]<='9')
	{


		alert('Start with character');
		return false;
	}
	else
	{

		return true;
	}


}




function Isnumber(Arg, item)
{
	if (Arg=="")
	{
  		alert(item+" should be a number");
		return false;
	}
	
	var len=Arg.length;
	

	var re = new RegExp("^[0-9]{1,"+Arg.length+"}$", "g");
	if (!re.test(Arg))
	{
  		alert(item+" should be a number");
		return false;
	}
	else
	{

		return true;
	}
}


function doValidateName(Name)
{

	var name= Name.match("^[0-9 a-z A-Z]{1,32}$");
	if(Name.length==0)
	{
		alert('Invalid Name');
		return false;
	}
	if(name==null)
	{
		alert('Invalid Name');
		return false;
	}
	if(name[0] >= '0' && name [0] <= '9')
	{
		alert('invalid Name');
		return false;
	}

	return true;

}


function doValidatePasswd(Password1)
{
	if(Password1.length==0)
	{
		alert('Enter a password before apply');
		return false;
	}
	return true;
}


function doValidatePassword(Password1,Password2)
{

	if(Password1 != Password2)
	{
		alert('Password and Confirmed Password must match');
		return false;
	}
	return true;
}


function valDoValidateHostName(HostName)
{
   if (HostName.match("^[a-zA-Z]+") == null) 
   {
   alert ("Host Name must start with a character");
   return false;
   }
   else if (HostName.match("^([a-zA-Z]+[a-zA-Z0-9_-]*)(\.*[a-zA-Z0-9])+$") == null) 
   {
   alert ("Host Name does not have correct format");
   return false;
   }
   
   else if ((HostName.length == 0) || (HostName.length > 63)) 
   { 
   alert("A host name must contain 1..63 characters");
   return false;
   }
   else
  	 return true;
}

function doValidateGateway(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { alert('Invalid Gateway IP address ' + Address); return false; }
  digits = address[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
    { alert('Invalid IP address '  + address[0]); return false; }
  }
   return true;
}




function doValidateMac(Address)
{
  var address=Address.match("^[0-9 a-f A-F]{2}\-[0-9 a-f A-F]{2}\-[0-9 a-f A-F]{2}\-[0-9 a-f A-F]{2}\-[0-9 a-f A-F]{2}\-[0-9 a-f A-F]{2}$");

  var digits;
  var i;

  if(address == null)
  {
     alert("Invalid Mac address");
     return false;
  }
  digits=address[0].split("-");
  if(digits.length != 6)
  {
     alert("Invalid Mac address");
     return false;
  }

  return true;
}


function doValidateHostName(Name)
{
   if(Name.match("[^0-9a-zA-Z_-]") != null) { alert("A name can only have\nalpha-numeric characters"); return false; }
   return true;
}


function doValidateNet(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;

  if(address == null) { alert('Invalid network ' + Address); return false; }
  if(address[0] == "255.255.255.255") { alert('Invalid network ' + address[0]); return false; }
  digits = address[0].split(".");
  if(digits.length<4)
  {
  alert("Invalid net in source");
  return false;
  }
  for(i=1; i <= 4; i++)
   if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
    { alert('Invalid network address ' + address[0]); return false; }
   return true;
}





function doValidateMask(Mask)
{
  var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var bMask = 0;
  var watch = false;
  var i;

  if(mask == null) { alert('Invalid network mask ' + Mask); return false; }
  digits = mask[0].split(".");
  if(digits.length<4)
  {
   alert("invalid");
   return false;
   }
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
     { alert('Invalid network mask ' + mask[0]); return false; }
	bMask = (bMask << 8) | Number(digits[i]);
  }
  bMask = bMask & 0x0FFFFFFFF;
  for(i=0; i<32; i++)
  {
    if((watch==true) && ((bMask & 0x1)==0)) { alert("Invalid Mask " + Mask + " (not contiguous)"); return false; }
	bMask = bMask >> 1;
	if((bMask & 0x01) == 1) watch=true;
  }

   return true;
}






function doValidateMetric(metric)
{
	if(metric=="")
	{
		alert(" metric should be filled");
		return false;
	}
	else
	    if(metric.match("[^0-9]") != null)
		{ alert("Metric has to be an integer");
		return false;
		}
	    else
	       if(Number(metric) >= 16 || Number(metric)<= 0)
	       {
	       	alert("range [1-15]");
		return false;
		}
		else
		 return true;
}




function doValidateM(Mask,ipaddress) {


	var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var i;
	var address;
	if(mask == null) { alert("Invalid Network mask" + Mask); return false; }

	digits = mask[0].split(".");

	address = ipaddress.split(".");

	if(address[0] > 191) {
		for(i=0;i < 3;i++) {
		if(digits[i] != 255) { alert("Invalid Mask"); return false; }
		}
		if((digits[3] > 254) || (digits[3] < 0)) { alert("Invalid Network Mask"); return false; }
	}
	else {
		if((address[0] >127) && (address[0] < 192)) {
		for(i=0;i< 2;i++) {
		if(digits[i] != 255) { alert("Invalid Mask"); return false; }
		}
		if((digits[2] > 255) || (digits[2]< 0)) { alert("Invalid Network Mask"); return false; }
		if((digits[3] > 254) || (digits[3] < 0)) { alert("Invalid NetWork mask"); return false; }
		if((digits[3] >0) && (digits[2] != 255)) { alert("Invalid NetWork mask"); return false; }
		}
		else {
			if((address[0] > 0) && (address[0] < 127)) {

			if(digits[0] != 255) { alert("Invalid Mask"); return false; }

			for(i=1;i< 3;i++){
			if((digits[i] > 255) || !(digits[i] >= 0)) { alert("Invalid Network Mask"); return false; }
			}
			if((digits[3] > 254) || !(digits[3] >= 0)) { alert("Invalid Network Mask"); return false; }
			if(digits[3] > 0) {
				if((digits[2] != 255) || (digits[1] != 255)) { alert("Invalid Network Mask"); return false; }
			}
			if(digits[2] > 0) {
				if(digits[1] != 255) { alert("Invalid Network Mask"); return false; }
			}
			}
		}

	}
	return true;
}




function doValidatePingSize(Pingsize)	{

	var pingsize;
	if(Pingsize=="")
	{
		alert('invalid');
		return false;
	}
	pingsize = parseInt(Pingsize);
	if(pingsize > 64000)
	{
		alert("Please select a ping size less than 64000");
		return false;
	}
	else
	{
		return true;
	}
}



function doValidatePingNo (Pingno)	{

	var pingno;
	if(Pingno=="")
	{
		alert('invlaid');
		return false;
	}
	pingno=parseInt(Pingno);
	if(pingno >= 10)
	{
		alert("please select ping count less than 10");
		return false;
	}
	else
	{
		return true;
	}
}

function doValidatePortNo (Portno)
{

	var pingno;
	if(Portno=="")
	{
		alert('invlaid port no');
		return false;
	}
	pingno=parseInt(Portno);
	if(pingno > 65535)
	{
		alert("Invalid port number:"+Portno);
		return false;
	}
	else if(pingno < 0)
	{
		alert("Invalid port number:"+Portno);
		return false;
	}
	else
	{
		return true;
	}
}







