#!/bin/sh
echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><body topmargin=\"10\" leftmargin=\"0\"><table width=\"560\" align=center cellpadding=\"0\" cellspacing=\"0\">"

node_name="WebCustom_Entry"
isDscpSupported=`/userfs/bin/tcapi get $node_name isDSCPSupported`

if [ "$isDscpSupported" = "Yes" ] 
then
echo "<tr><td><table width=\"560\" border=\"1\" align=center cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr><td width=\"15\" align=center class=\"tabdata\"><strong>#</strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Active</FONT></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Interface</FONT></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Direction</FONT></strong></td><td width=\"120\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Src Address/<br>Mask</FONT></strong></td><td width=\"120\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Dest Address/<br>Mask</FONT></strong></td><td width=\"120\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Mac Address</FONT></strong></td><td width=\"60\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Src Port</FONT></strong></td><td width=\"60\"  align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Dest Port</FONT></strong></td><td width=\"60\"  align=center class=\"tabdata\"><strong><FONT color=\"#000000\">DSCP</FONT></strong></td><td width=\"60\"  align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Protocol</FONT></strong></td></tr>"
else
echo "<tr><td><table width=\"560\" border=\"1\" align=center cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr><td width=\"15\" align=center class=\"tabdata\"><strong>#</strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Active</FONT></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Interface</FONT></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Direction</FONT></strong></td><td width=\"120\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Src Address/<br>Mask</FONT></strong></td><td width=\"120\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Dest Address/<br>Mask</FONT></strong></td><td width=\"120\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Mac Address</FONT></strong></td><td width=\"60\" align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Src Port</FONT></strong></td><td width=\"60\"  align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Dest Port</FONT></strong></td><td width=\"60\"  align=center class=\"tabdata\"><strong><FONT color=\"#000000\">Protocol</FONT></strong></td></tr>"
fi

for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
	node_name="IpMacFilter_Entry$j"
  Interface=`/userfs/bin/tcapi get $node_name Interface`
  Direction=`/userfs/bin/tcapi get $node_name Direction`
  RuleType=`/userfs/bin/tcapi get $node_name RuleType`
  Active=`/userfs/bin/tcapi get $node_name Active`
  SrcIPAddr=`/userfs/bin/tcapi get $node_name SrcIPAddr`
  SrcIPMask=`/userfs/bin/tcapi get $node_name SrcIPMask`
  SrcPort=`/userfs/bin/tcapi get $node_name SrcPort`
  DesIPAddr=`/userfs/bin/tcapi get $node_name DesIPAddr`
  DesIPMask=`/userfs/bin/tcapi get $node_name DesIPMask`
  DesPort=`/userfs/bin/tcapi get $node_name DesPort`
  Protocol=`/userfs/bin/tcapi get $node_name Protocol`
  MacAddr=`/userfs/bin/tcapi get $node_name MacAddr`
  if [ "$isDscpSupported" = "Yes" ] 
  then
  	DSCP=`/userfs/bin/tcapi get $node_name DSCP`
  fi
  
	if [ "$Active" = "no attribute information" -o "$Active" = "no node information" -o "$Active" = "N/A" ]
	then
		Active="N/A"
		continue
	fi
	
	if [ "$Interface" = "no attribute information" -o "$Interface" = "no node information" ]
	then
		Interface="N/A"
	fi
	if [ "$Direction" = "no attribute information" -o "$Direction" = "no node information" ]
	then
		Direction="N/A"
	fi
	if [ "$RuleType" = "no attribute information" -o "$RuleType" = "no node information" ]
	then
		RuleType="N/A"
	fi
	if [ "$SrcIPAddr" = "no attribute information" -o "$SrcIPAddr" = "no node information" ]
	then
		SrcIPAddr="N/A"
	fi
	if [ "$SrcIPMask" = "no attribute information" -o "$SrcIPMask" = "no node information" ]
	then
		SrcIPMask="N/A"
	fi
	if [ "$SrcPort" = "no attribute information" -o "$SrcPort" = "no node information" ]
	then
		SrcPort="N/A"
	fi
	if [ "$DesIPAddr" = "no attribute information" -o "$DesIPAddr" = "no node information" ]
	then
		DesIPAddr="N/A"
	fi
	if [ "$DesIPMask" = "no attribute information" -o "$DesIPMask" = "no node information" ]
	then
		DesIPMask="N/A"
	fi
	if [ "$DesPort" = "no attribute information" -o "$DesPort" = "no node information" ]
	then
		DesPort="N/A"
	fi
	if [ "$Protocol" = "no attribute information" -o "$Protocol" = "no node information" ]
	then
		Protocol="N/A"
	fi
	if [ "$MacAddr" = "no attribute information" -o "$MacAddr" = "no node information" ]
	then
		MacAddr="N/A"
	fi
	if [ "$isDscpSupported" = "Yes" ] 
	then
		if [ "$DSCP" = "no attribute information" -o "$DSCP" = "no node information" ]
		then
			DSCP="0"
		fi
	fi

	j=`expr $j + 1`

	if [ "$isDscpSupported" = "Yes" ] 
	then
		echo "<tr><td align=center class=\"tabdata\">$j</td><td align=center class=\"tabdata\">$Active</td><td align=center class=\"tabdata\">$Interface</td><td align=center class=\"tabdata\">$Direction</td><td align=center class=\"tabdata\">$SrcIPAddr/<br>$SrcIPMask</td><td align=center class=\"tabdata\">$DesIPAddr/<br>$DesIPMask</td><td align=center class=\"tabdata\">$MacAddr</td><td align=center class=\"tabdata\">$SrcPort</td><td align=center class=\"tabdata\">$DesPort</td><td align=center class=\"tabdata\">$DSCP</td><td align=center class=\"tabdata\">$Protocol</td></tr>"
	else
		echo "<tr><td align=center class=\"tabdata\">$j</td><td align=center class=\"tabdata\">$Active</td><td align=center class=\"tabdata\">$Interface</td><td align=center class=\"tabdata\">$Direction</td><td align=center class=\"tabdata\">$SrcIPAddr/<br>$SrcIPMask</td><td align=center class=\"tabdata\">$DesIPAddr/<br>$DesIPMask</td><td align=center class=\"tabdata\">$MacAddr</td><td align=center class=\"tabdata\">$SrcPort</td><td align=center class=\"tabdata\">$DesPort</td><td align=center class=\"tabdata\">$Protocol</td></tr>"
	fi
	j=`expr $j - 1`

done

echo "</table></td></tr></table></body></html>"

