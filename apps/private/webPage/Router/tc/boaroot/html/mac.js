
function isValidHex( ucHex)
{
	return (ucHex>='0' && ucHex<='9') || (ucHex>='a' && ucHex<='f') || (ucHex>='A' && ucHex<='F') ? true : false;
}

////////////////////////////////////////////////////////////////////
// function formatMacAddr( szMac)
// input:  szMac: the user input mac address, as the following format:
//         xxxxxxxxxxxx, xx:xx:xx:xx:xx:xx, xx xx xx xx xx xx, xx-xx-xx-xx-xx-xx
// return: fromatted mac address, as this format: xx:xx:xx:xx:xx:xx
//         return null string if the input mac address is invalid

function formatMacAddr( szMac)
{
	var i=0;
	var j=0;
	var bErrorFlag=false;
	var bOctet= false;
	var iMacIdx=0;
	var szTemp="";
	var szFormattedMac= "";
	var nLength= szMac.length;
	for( i=0; i < nLength; i++)
	{
		ch= szMac.charAt(i);
		if( ch==' ' || ch==':' || ch=='-')
		{
			if( szTemp.length > 0)
				bOctet= true;
		}
		else if( isValidHex(ch))
		{
			szTemp+= ch;
			if( szTemp.length==2 || i==nLength-1)	// szTemp.length == 2 or reached the end of szMac
			{
				bOctet= true;
			}
		}
		else
		{
			bErrorFlag= true;
			break;
		}
		if( bOctet)
		{
			iMacIdx++;
			if( iMacIdx>6)
			{
				bErrorFlag= true;
				break;
			}
			
			if( szTemp.length < 2)
				szTemp= "0" + szTemp;
			
			for( j=0; j < szTemp.length; j++)
			{
				if( !isValidHex(szTemp.charAt(j)))
				{
				bErrorFlag= true;
				break;
				}
			}
			szFormattedMac+= szTemp;
			if( iMacIdx<6)
			szFormattedMac+= ":";
			
			szTemp="";
			bOctet= false;
		}
	}
	
	if(!bErrorFlag)
	{
		if(iMacIdx==6)
			return szFormattedMac;
		else
			return "";
	}
	return "";
}