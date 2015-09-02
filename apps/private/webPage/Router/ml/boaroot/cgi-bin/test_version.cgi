#!/bin/sh
echo -e "Content-Type: text/html\n\n<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\">\n</head><script language=\"JavaScript\"></script>"

echo -e "<body>\n"
echo -e "<table width=\"45%\" border=\"1\" align=center>\n"
echo -e  " <tr> \n"
echo -e   "  <td width=\"22%\">version:</td>\n"
echo -e    " <td colspan=\"3\"> \n"


	FwVer=`/userfs/bin/tcapi get DeviceInfo FwVer`
	case $FwVer in
		"no node information" | "no attribute information" | "connection error") FwVer=" ";;
	esac		

	echo -e "$FwVer"

	
echo -e "	</td>\n"
echo -e "  </tr>\n"
echo -e "  <tr> \n"
echo -e "   <td>build date:</td>\n"
echo -e "    <td colspan=\"3\">\n"

	FwMakeTime=`/userfs/bin/tcapi get DeviceInfo FwMakeTime`
	case $FwMakeTime in
		"no node information" | "no attribute information" | "connection error") FwMakeTime=" ";;
	esac				

	echo -e "$FwMakeTime"


echo -e "</td>\n"
echo -e   "</tr>\n"
echo -e   "<tr> \n"
echo -e   "  <td rowspan=\"300\">flash:</td>\n"
echo -e   "  <td width=\"40%\">Name</td>\n"
echo -e   "  <td width=\"18%\">ID</td>\n"
echo -e   " <td width=\"20%\">Size</td>\n"
echo -e   "</tr>\n"

MaxNum=`/userfs/bin/tcapi get DeviceInfo FlashCount`
i=0
support_flash=""
while [ "$i" -lt "$MaxNum" ]
do
	attr_name="SupportFlash$i"
	support_flash=`/userfs/bin/tcapi get DeviceInfo $attr_name`
	case $support_flash in
		"no node information" | "no attribute information" | "connection error") support_flash=" ";;
	esac	

	echo -e "$support_flash"

	i=`expr $i + 1` 
done


echo -e "</table>\n"
echo -e "</body>\n"
echo -e "</html>\n"
