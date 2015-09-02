#!/bin/sh

echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><body topmargin=\"10\" leftmargin=\"0\"><table width=\"490\" align=center cellpadding=\"0\" cellspacing=\"0\">"

echo "<tr><td><table width=\"482\" border=\"1\" align=center cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr height=\"30\"><td width=\"60\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Index</font></strong></td><td width=\"60\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Active</font></strong></td><td width=\"180\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Secure IP Address</font></strong></td><td width=\"90\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Application</font></strong></td><td width=\"80\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Interface</font></strong></td></tr>"

for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
	node_name="ACL_Entry$j"
  RuleActive=`/userfs/bin/tcapi get $node_name Activate`
  ScrIPaddrBegin=`/userfs/bin/tcapi get $node_name ScrIPAddrBegin`
  ScrIPaddrEnd=`/userfs/bin/tcapi get $node_name ScrIPAddrEnd`
  Application=`/userfs/bin/tcapi get $node_name Application`
  Interface=`/userfs/bin/tcapi get $node_name Interface`
  
	if [ "$RuleActive" = "no attribute information" -o "$RuleActive" = "no node information" -o "$RuleActive" = "N/A" ]
	then
		RuleActive="N/A"
		continue
	fi
	
	if [ "$ScrIPaddrBegin" = "no attribute information" -o "$APPLICATION" = "no node information" ]
	then
		ScrIPaddrBegin="N/A"
	fi
	if [ "$ScrIPaddrEnd" = "no attribute information" -o "$APPLICATION" = "no node information" ]
	then
		ScrIPaddrEnd="N/A"
	fi
	if [ "$Application" = "no attribute information" -o "$APPLICATION" = "no node information" ]
	then
		Application="N/A"
	fi
	if [ "$Interface" = "no attribute information" -o "$APPLICATION" = "no node information" ]
	then
		Interface="N/A"
	fi

	j=`expr $j + 1`

	echo "<tr height=\"30\"><td width=\"60\" align=center class=\"tabdata\"><strong><font color=\"#000000\">$j</font></strong></td><td width=\"60\" align=center class=\"tabdata\"><strong><font color=\"#000000\">$RuleActive</font></strong></td><td width=\"180\" align=center class=\"tabdata\"><strong><font color=\"#000000\">$ScrIPaddrBegin~$ScrIPaddrEnd</font></strong></td><td width=\"90\" align=center class=\"tabdata\"><strong><font color=\"#000000\">$Application</font></strong></td><td width=\"80\" align=center class=\"tabdata\"><strong><font color=\"#000000\">$Interface</font></strong></td></tr>"

	j=`expr $j - 1`

done                                         
                                             
echo "</td></tr></table></body></html>"                             

