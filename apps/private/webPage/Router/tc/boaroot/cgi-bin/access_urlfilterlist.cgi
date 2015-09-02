#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><body topmargin=\"10\" leftmargin=\"0\"><table width=\"580\" align=center cellpadding=\"0\" cellspacing=\"0\">"

echo "<tr><td><table width=\"580\" border=\"1\" align=center cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr><TD class=tabdata align=middle width=15><STRONG>Index</STRONG></TD><td width=15 align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Active</FONT></strong></td><TD class=tabdata align=middle width=45><STRONG><FONT color=#000000>URL</FONT></STRONG></TD></TR></tr>"

#for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
#Only support 8 rules
for j in 0 1 2 3 4 5 6 7
do
	node_name="UrlFilter_Entry$j"
  	Activate=`/userfs/bin/tcapi get $node_name Activate`
  	URL=`/userfs/bin/tcapi get $node_name URL`

  	if [ "$Activate" = "1" ] 
  	then
  		Activate="Yes"
	elif [ "$Activate" = "no attribute information" -o "$Activate" = "no node information" -o "$Activate" = "N/A" -o "$Activate" = "0" ] 
	then
		Activate="No"
	else  Activate="Wrong setting"
	fi


	if [ "$URL" != "no attribute information" -a "$URL" != "no node information" -a "$URL" != "N/A" -a "$URL" != "" ]
	then
		j=`expr $j + 1`
	echo "<tr><td class=tabdata align=middle>$j</td><td class=tabdata align=middle>$Activate</td><td class=tabdata width=500>$URL</td></tr>"

		j=`expr $j - 1`
	fi
done

echo "</table></td></tr></table></body></html>"

