// JavaScript Document
function isValidIpAddr(ip1,ip2,ip3,ip4)
{
if(ip1==0 || ip4==0 || ip4==255 || ip1==127 )	
	return false;			

return true;	
}

function inValidIPAddr(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { 
  	alert(_("InvalidIpAddr") + " " + Address);
  	return true;
  }
  
  digits = address[0].split(".");
  for(i=0; i < 4; i++)
  {
  	if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223) || (digits[i] == null))
  	{ 
  		alert(_("InvalidIpAddr") + " " + Address);
  		return true;
  	}
  }
  if(!isValidIpAddr(digits[0],digits[1],digits[2],digits[3],false))
  {
    	alert(_("InvalidIpAddr") + " " + Address);
  	return true;
  }
  return false;
}

function inValidSubnetMask(Mask)
{
  var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var bMask = 0;
  var watch = false;
  var i;

  if(mask == null)
  { 
	  alert(_("InvalidNetMask") + " " + Mask);
	  return true;
  }
  digits = mask[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || digits[i] == null)
     {
	     alert(_("SubMaskInvalid") + " " + Mask);
	     return true;	
     }
	bMask = (bMask << 8) | Number(digits[i]);
  }
  //edited by Janey
  if((Number(digits[0]) == 0))
	{
	     alert(_("InvalidNetMask") + " " + Mask);
	     return true;	
	}

  bMask = bMask & 0x0FFFFFFFF;
  for(i=0; i<32; i++)
  {
    if((watch==true) && ((bMask & 0x1)==0)) { 
    alert(_("InvalidNetMask") + " " + Mask);
    return true;
  }
	bMask = bMask >> 1;
	if((bMask & 0x01) == 1) watch=true;
  }
  return false;
}
//add by Janey
//check the IP & NetMask is unavailable
function inValidNetAddr(Address,Mask)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var mask = Mask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var bMask = 0;
  var bIp = 0;
  if(inValidIPAddr(Address)){
  	return true;}
  if(inValidSubnetMask(Mask)){
  	return true;}
  	
  Ipdigits = address[0].split(".");
  Maskdigits = mask[0].split(".");
  if(Number(Maskdigits[3]) == 255)
  {
       alert(_("InvalidNetMask") + " " + Mask);
       return true;	
  }
  for(i=0; i < 4; i++)
  {
  	bIp = (bIp << 8) | Number(Ipdigits[i]);
  	bMask = (bMask << 8) | Number(Maskdigits[i]);
  }

  bIp = bIp & 0x0FFFFFFFF;
  bMask = bMask & 0x0FFFFFFFF;
  bMask = ~bMask & 0x0FFFFFFFF;
  if(((bIp & bMask) == bMask) || ((bIp & bMask) == 0))
  {
  	alert(_("InvalidIpAndNetMask") + " " + Address + ' & ' + Mask);
  	return true;
  }
  
  return false;
}

function inValidateGateway(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { 	alert(_("InvalidGWIP") + " " + Address); return true; }
  digits = address[0].split(".");
  for(i=0; i < 4; i++)
  {
    if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223))
    { 	alert(_("InvalidIpAddr") + " " + address[0]); return true; }
  }
  if ( (Number(digits[0]) == 0) && (Number(digits[1]) == 0)
  	&& (Number(digits[2]) == 0) && (Number(digits[3]) == 0))
	{
	     	alert(_("InvalidGWIP") + " " + Address);
	     return true;	
	}
 
   return false;
}

//Basic check that it's a IP address and not letters or anything
function basicIPAddrCheck(Address)
{
  var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
  var digits;
  var i;

  if(address == null) { 
  	alert(_("InvalidIpAddr") + " " + Address);
  	return true;
  }
  else
  {
    digits = address[0].split(".");
    for(i=0; i < 4; i++)
    {
      if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ))
      { 
  		alert(_("InvalidIpAddr") + " " + Address);
      	return true;
      }
    }
  }
  return false;
}
function trim(s)
{
    var l=0; var r=s.length -1;
    while(l < s.length && s[l] == ' ')
    {     l++; }
    while(r > l && s[r] == ' ')
    {     r-=1;     }
    return s.substring(l, r+1);
} 

function inValidIPv6Addr(Address1)
{
	var regExp = /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/;
	
	var Address = trim(Address1);
	var address = Address.match(regExp);
	if(address == null){		
		alert(_("InvalidIPv6Addr") + " " + Address);
 		return true;
	}	
	return false;
}

function isIPv6GlobalAddr(Address1)
{
	var str1=Address1.indexOf("fe80:");
	var str2=Address1.indexOf("fec0:");
	var str3=Address1.indexOf("ff");
	if((str1!=0)&&(str2!=0)&&(str3!=0)){
		return true;	
	}else
		return false;

}	

function inValidIPv6Prefix(Prefix1)
{
	var Prefix = trim(Prefix1);
	var IPv6Prefix = Prefix.match("^[0-9]{1,3}$");
	
	if(IPv6Prefix == null) { 
 		alert(_("InvalidIPv6Prefix") + " " + Prefix);
 		return true;
	}
	
	if((Number(Prefix) > 128 ) || (Number(Prefix) < 1 )){
		alert(_("InvalidIPv6Prefix") + " " + Prefix);
		return true;
	}
	
	return false;
}
function convertToHex(num)
{
	var num1;
	var ch;
	var i;
	
	num1 = 0;
	for(i = 0; i < 4; i++){
		ch = num.charAt(i);

		if(ch == 'a' || ch == 'A'){
			num1 = num1 + 10 * Math.pow(16, 4 - i - 1);
		}
		else if(ch == 'b' || ch == 'B'){
			num1 = num1 + 11 * Math.pow(16, 4 - i - 1);
		}
		else if(ch == 'c' || ch == 'C'){
			num1 = num1 + 12 * Math.pow(16, 4 - i - 1);
		}
		else if(ch == 'd' || ch == 'D'){
			num1 = num1 + 13 * Math.pow(16, 4 - i - 1);
		}
		else if(ch == 'e' || ch == 'E'){
			num1 = num1 + 14 * Math.pow(16, 4 - i - 1);
		}
		else if(ch == 'f' || ch == 'F'){
			num1 = num1 + 15 * Math.pow(16, 4 - i - 1);
		}
		else{
			num1 = num1 + ch * Math.pow(16, 4 - i - 1);
		}
	}
	return num1;
}

function checkColon(str)
{
	var num = 0;
	
	while(1){
		if(str.indexOf(":") < 0){
			break;
		}
		else{
			num = num + 1;
			str = str.substring(str.indexOf(":") + 1, str.length);
		}
	}
	num = num + 1;
	
	return num;
}

function formalIPv6Addr(ip)
{
	var before;
	var after;
	var i;
	var ret;
		
	if(ip.indexOf("::") < 0){
		return ip;
	}
	else if(ip.indexOf("::") == 0){
		after = ip.substring(ip.indexOf("::") + 2, ip.length);
		
		var num = checkColon(after);
		ret = "";
		for(i = 0; i < 8 - num; i++){
			ret = ret + "0:"; 
		}		
		ret = ret + after;		
	}
	else if(ip.indexOf("::") == (ip.length - 2)){		
		before = ip.substring(0, ip.indexOf("::"));
	
		var num = checkColon(before);
		
		ret = "";
	
		for(i = 0; i < 8 - num; i++){
			ret = ret + ":0"; 
		}		
		ret = before + ret;	
	}
	else{
		before = ip.substring(0, ip.indexOf("::"));
		after = ip.substring(ip.indexOf("::") + 2, ip.length);
		
		var num = checkColon(before);		
		var num1 = checkColon(after);
		ret = "";
	
		for(i = 0; i < 8 - num - num1; i++){
			ret = ret + ":0"; 
		}		
		ret = ret + ":";
		ret = before + ret + after;
	}
	return ret;
}


function checkIPandGateway(ip, pref, gw)
{
	var formalIp = formalIPv6Addr(ip);
	var formalGw = formalIPv6Addr(gw);

	var prefix1 = Math.floor(pref / 16);
	var prefix2 = pref % 16;
	
	var ipArr = formalIp.split(":", prefix1 + 1);
	var gwArr = formalGw.split(":", prefix1 + 1);
	
	var i = 0;
	for(i = 0; i < prefix1; i++){			
		if((ipArr[i] & 0xffff) != (gwArr[i] & 0xffff)){
			alert(_("IPv6WanIPGWDomain"));
			return true;
		}
	}
			
	if(prefix2 != 0){
		var tmp1 = convertToHex(ipArr[i]);
		var tmp2 = convertToHex(gwArr[i]);
		
		var value = 0;
		for(i = 1; i <= prefix2; i++){
			value = value | (1 << (16 - i));
		}
					
		if((tmp1 & value) != (tmp2 & value)){
			alert(_("IPv6WanIPGWDomain"));
			return true;
		}
	}
	return false;
}
//IPv6
function ParseIpv6Array(str)
{
    var Num;
    var i,j;
    var finalAddrArray = new Array();
    var falseAddrArray = new Array();
    
    var addrArray = str.split(':');
    Num = addrArray.length;
    if (Num > 8)
    {
        return falseAddrArray;
    }

    for (i = 0; i < Num; i++)
    {
        if ((addrArray[i].length > 4) 
            || (addrArray[i].length < 1))
        {
            return falseAddrArray;
        }
        for (j = 0; j < addrArray[i].length; j++)
        {
            if ((addrArray[i].charAt(j) < '0')
                || (addrArray[i].charAt(j) > 'f')
                || ((addrArray[i].charAt(j) > '9') && 
                (addrArray[i].charAt(j) < 'a')))
            {
                return falseAddrArray;
            }
        }

        finalAddrArray[i] = '';
        for (j = 0; j < (4 - addrArray[i].length); j++)
        {
            finalAddrArray[i] += '0';
        }
        finalAddrArray[i] += addrArray[i];
    }

    return finalAddrArray;
}

function getFullIpv6Address(address)
{
    var c = '';
    var i = 0, j = 0, k = 0, n = 0;
    var startAddress = new Array();
    var endAddress = new Array();
    var finalAddress = '';
    var startNum = 0;
    var endNum = 0;
    var lowerAddress;
    var totalNum = 0;

    lowerAddress = address.toLowerCase();
 
    var addrParts = lowerAddress.split('::');
    if (addrParts.length == 2)
    {
        if (addrParts[0] != '')
        {
            startAddress = ParseIpv6Array(addrParts[0]);
            if (startAddress.length == 0)
            {
                return '';
            }
        }
        if (addrParts[1] != '')
        {
            endAddress = ParseIpv6Array(addrParts[1]);
            if (endAddress.length == 0)
            {
               return '';
            }
        }
	else
		return '';

        if (startAddress.length +  endAddress.length >= 8)
        {
            return '';
        }
    }
    else if (addrParts.length == 1)
    {
        startAddress = ParseIpv6Array(addrParts[0]);
        if (startAddress.length != 8)
        {
            return '';
        }
    }
    else
    {
        return '';
    }

    for (i = 0; i < startAddress.length; i++)
    {
        finalAddress += startAddress[i];
        if (i != 7)
        {
            finalAddress += ':';
        }
    }
    for (; i < 8 - endAddress.length; i++)
    {
        finalAddress += '0000';
        if (i != 7)
        {
            finalAddress += ':';
        }
    }
    for (; i < 8; i++)
    {
        finalAddress += endAddress[i - (8 - endAddress.length)];
        if (i != 7)
        {
            finalAddress += ':';
        }
    }

    return finalAddress;
}

function isGlobalIpv6Address(address)
{
    var tempAddress = getFullIpv6Address(address);
    if ((tempAddress == '')
        || (tempAddress == '0000:0000:0000:0000:0000:0000:0000:0000')
        || (tempAddress == '0000:0000:0000:0000:0000:0000:0000:0001')
        || (tempAddress.substring(0, 3) == 'fe8')
        || (tempAddress.substring(0, 3) == 'fe9')
        || (tempAddress.substring(0, 3) == 'fea')
        || (tempAddress.substring(0, 3) == 'feb')
	|| (tempAddress.substring(0, 3) == 'fec')
        || (tempAddress.substring(0, 2) == 'ff'))
    {
        return false;
    }
    
    return true;
}
