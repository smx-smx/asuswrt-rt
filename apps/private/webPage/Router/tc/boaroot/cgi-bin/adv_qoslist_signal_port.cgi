#!/bin/sh



echo "Content-Type: text/html"
echo ""

echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><body topmargin=\"10\" leftmargin=\"0\"><table width=\"820\" align=center cellpadding=\"0\" cellspacing=\"0\"><tr><td width=\"820\" height=\"5\" valign=\"baseline\" class=\"orange\"></td></tr></table><table width=\"820\" align=center cellpadding=\"0\" cellspacing=\"0\"><tr><td width=\"820\" height=\"35\" class=\"headline\"><font color=\"#666666\">QoS Settings Summary</font></td></tr><tr><td><table border=\"1\" align=center ><tr height=\"30\"><td height=\"35\" bgcolor=\"ffff66\" class=\"tabdata\" align=center><strong><font color=\"#000000\">Rules</font></strong></td><td height=\"35\" bgcolor=\"ffff99\" class=\"tabdata\" align=center><strong><font color=\"#000000\">Actions</font></strong></td></tr><tr><td width=\"630\" height=\"115\" bgcolor=\"ffff66\" class=\"tabdata\" align=center><table cellspacing=1 cellpadding=1 border=1 width=\"620\" align=center><tr><td class=\"tabdata\" rowspan=2 align=center>#</td><td class=\"tabdata\" rowspan=2 align=center><font color=\"#000000\">Active</font></td><td class=\"tabdata\" rowspan=2 align=center><font color=\"#000000\">Physical Ports</font></td><td class=\"tabdata\" align=center><font color=\"#000000\">Destination</font></td><td class=\"tabdata\" align=center><font color=\"#000000\">Source</font></td><td class=\"tabdata\" rowspan=2 align=center><font color=\"#000000\">Protocol ID</font></td><td class=\"tabdata\" rowspan=2 align=center><font color=\"#000000\">VLAN ID</font></td><td class=\"tabdata\" rowspan=2 align=center><div><font color=\"#000000\">IPP/TOS</font></div><div><font color=\"#000000\">(DSCP)</font></div></td><td class=\"tabdata\" rowspan=2 align=center><font color=\"#000000\">802.1p</font></td></tr><tr><td class=\"tabdata\" align=center><div><font color=\"#000000\">MAC</font></div><div><font color=\"#000000\">IP/Mask</font></div><div><font color=\"#000000\">Port Range</font></div></td><td class=\"tabdata\" align=center><div><font color=\"#000000\">MAC</font></div><div><font color=\"#000000\">IP/Mask</font></div><div><font color=\"#000000\">Port Range</font></div></td></tr>"

for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
	node_name="QoS_Entry$j"
	ACTIVATE=`/userfs/bin/tcapi get $node_name Active`
	APPLICATION=`/userfs/bin/tcapi get $node_name Application`
	ENET0=`/userfs/bin/tcapi get $node_name eth0`
	ENET1=`/userfs/bin/tcapi get $node_name eth1`
	ENET2=`/userfs/bin/tcapi get $node_name eth2`
	ENET3=`/userfs/bin/tcapi get $node_name eth3`
	USB0=`/userfs/bin/tcapi get $node_name usb0`
	RA0=`/userfs/bin/tcapi get $node_name ra0`
	RA1=`/userfs/bin/tcapi get $node_name ra1`
	RA2=`/userfs/bin/tcapi get $node_name ra2`
	RA3=`/userfs/bin/tcapi get $node_name ra3`
	DESMAC=`/userfs/bin/tcapi get $node_name DesMac`
	DESIP=`/userfs/bin/tcapi get $node_name DesIP`
	DESMASK=`/userfs/bin/tcapi get $node_name DesMask`
	DESPORTRANGEBEGIN=`/userfs/bin/tcapi get $node_name DesPortRangeBegin`
	DESPORTRANGEEND=`/userfs/bin/tcapi get $node_name DesPortRangeEnd`
	SRCMAC=`/userfs/bin/tcapi get $node_name SrcMac`
	SRCIP=`/userfs/bin/tcapi get $node_name SrcIP`
	SRCMASK=`/userfs/bin/tcapi get $node_name SrcMask`
	SRCPORTRANGEBEGIN=`/userfs/bin/tcapi get $node_name SrcPortRangeBegin`
	SRCPORTRANGEEND=`/userfs/bin/tcapi get $node_name SrcPortRangeEnd`
	PROTOCOLID=`/userfs/bin/tcapi get $node_name ProtocolID`
	VLANIDRANGEBEGIN=`/userfs/bin/tcapi get $node_name VlanIDRangeBegin`
	VLANIDRANGEEND=`/userfs/bin/tcapi get $node_name VlanIDRangeEnd`
	IPPDS=`/userfs/bin/tcapi get $node_name IPPDSField`
	IPPRANGEBEGIN=`/userfs/bin/tcapi get $node_name IPPRangeBegin`
	IPPRANGEEND=`/userfs/bin/tcapi get $node_name IPPRangeEnd`
	TOS=`/userfs/bin/tcapi get $node_name TypeOfService`
	DSCPRANGEBEGIN=`/userfs/bin/tcapi get $node_name DSCPRangeBegin`
	DSCPRANGEEND=`/userfs/bin/tcapi get $node_name DSCPRangeEnd`
	dotpBEGIN=`/userfs/bin/tcapi get $node_name dotpBegin`
	dotpEND=`/userfs/bin/tcapi get $node_name dotpEnd`
	
	if [ "$ACTIVATE" = "no attribute information" -o "$ACTIVATE" = "no node information" -o "$ACTIVATE" = "N/A" ]
	then
		ACTIVATE="N/A"
		continue
	fi
	
	if [ "$APPLICATION" = "no attribute information" -o "$APPLICATION" = "no node information" ]
	then
		APPLICATION="N/A"
	fi
	if [ "$ENET0" = "no attribute information" -o "$ENET0" = "no node information" ]
	then
		ENET0="N/A"
	fi
	if [ "$ENET1" = "no attribute information" -o "$ENET1" = "no node information" ]
	then
		ENET1="N/A"
	fi
	if [ "$ENET2" = "no attribute information" -o "$ENET2" = "no node information" ]
	then
		ENET2="N/A"
	fi
	if [ "$ENET3" = "no attribute information" -o "$ENET3" = "no node information" ]
	then
		ENET3="N/A"
	fi
	if [ "$USB0" = "no attribute information" -o "$USB0" = "no node information" ]
	then
		USB0="N/A"
	fi
	if [ "$RA0" = "no attribute information" -o "$RA0" = "no node information" ]
	then
		RA0="N/A"
	fi
	if [ "$RA1" = "no attribute information" -o "$RA1" = "no node information" ]
	then
		RA1="N/A"
	fi
	if [ "$RA2" = "no attribute information" -o "$RA2" = "no node information" ]
	then
		RA2="N/A"
	fi
	if [ "$RA3" = "no attribute information" -o "$RA3" = "no node information" ]
	then
		RA3="N/A"
	fi
	if [ "$DESMAC" = "no attribute information" -o "$DESMAC" = "no node information" ]
	then
		DESMAC="N/A"
	fi
	if [ "$DESIP" = "no attribute information" -o "$DESIP" = "no node information" ]
	then
		DESIP="N/A"
	fi
	if [ "$DESMASK" = "no attribute information" -o "$DESMASK" = "no node information" ]
	then
		DESMASK="N/A"
	fi
	if [ "$DESPORTRANGEBEGIN" = "no attribute information" -o "$DESPORTRANGEBEGIN" = "no node information" ]
	then
		DESPORTRANGEBEGIN="N/A"
	fi
	if [ "$DESPORTRANGEEND" = "no attribute information" -o "$DESPORTRANGEEND" = "no node information" ]
	then
		DESPORTRANGEEND="N/A"
	fi
	if [ "$SRCMAC" = "no attribute information" -o "$SRCMAC" = "no node information" ]
	then
		SRCMAC="N/A"
	fi
	if [ "$SRCIP" = "no attribute information" -o "$SRCIP" = "no node information" ]
	then
		SRCIP="N/A"
	fi
	if [ "$SRCMASK" = "no attribute information" -o "$SRCMASK" = "no node information" ]
	then
		SRCMASK="N/A"
	fi
	if [ "$SRCPORTRANGEBEGIN" = "no attribute information" -o "$SRCPORTRANGEBEGIN" = "no node information" ]
	then
		SRCPORTRANGEBEGIN="N/A"
	fi
	if [ "$SRCPORTRANGEEND" = "no attribute information" -o "$SRCPORTRANGEEND" = "no node information" ]
	then
		SRCPORTRANGEEND="N/A"
	fi
	if [ "$PROTOCOLID" = "no attribute information" -o "$PROTOCOLID" = "no node information" ]
	then
		PROTOCOLID="N/A"
	fi
	if [ "$VLANIDRANGEBEGIN" = "no attribute information" -o "$VLANIDRANGEBEGIN" = "no node information" ]
	then
		VLANIDRANGEBEGIN="N/A"
	fi
	if [ "$VLANIDRANGEEND" = "no attribute information" -o "$VLANIDRANGEEND" = "no node information" ]
	then
		VLANIDRANGEEND="N/A"
	fi
	if [ "$IPPDS" = "no attribute information" -o "$IPPDS" = "no node information" ]
	then
		IPPDS="N/A"
	fi
	if [ "$IPPRANGEBEGIN" = "no attribute information" -o "$IPPRANGEBEGIN" = "no node information" ]
	then
		IPPRANGEBEGIN="N/A"
	fi
	if [ "$IPPRANGEEND" = "no attribute information" -o "$IPPRANGEEND" = "no node information" ]
	then
		IPPRANGEEND="N/A"
	fi
	if [ "$TOS" = "no attribute information" -o "$TOS" = "no node information" ]
	then
		TOS="N/A"
	fi
	if [ "$DSCPRANGEBEGIN" = "no attribute information" -o "$DSCPRANGEBEGIN" = "no node information" ]
	then
		DSCPRANGEBEGIN="N/A"
	fi
	if [ "$DSCPRANGEEND" = "no attribute information" -o "$DSCPRANGEEND" = "no node information" ]
	then
		DSCPRANGEEND="N/A"
	fi
	if [ "$dotpBEGIN" = "no attribute information" -o "$dotpBEGIN" = "no node information" ]
	then
		dotpBEGIN="N/A"
	fi
	if [ "$dotpEND" = "no attribute information" -o "$dotpEND" = "no node information" ]
	then
		dotpEND="N/A"
	fi

	if [ "$ENET0" = "Yes" ]
	then
		ENET0="e0,"
	elif [ "$ENET0" = "No" -o "$ENET0" = "N/A" ]
	then
		ENET0=""
	fi
	
	if [ "$ENET1" = "Yes" ]
	then
		ENET1="e1,"
	elif [ "$ENET1" = "No" -o "$ENET1" = "N/A" ]
	then
		ENET1=""
	fi

	if [ "$ENET2" = "Yes" ]
	then
		ENET2="e2,"
	elif [ "$ENET2" = "No" -o "$ENET2" = "N/A" ]
	then
		ENET2=""
	fi
	
	if [ "$ENET3" = "Yes" ]
	then
		ENET3="e3,"
	elif [ "$ENET3" = "No" -o "$ENET3" = "N/A" ]
	then
		ENET3=""
	fi

	if [ "$USB0" = "Yes" ]
	then
		USB0="usb0,"
	elif [ "$USB0" = "No" -o "$USB0" = "N/A" ]
	then
		USB0=""
	fi

	if [ "$RA0" = "Yes" ]
	then
		RA0="ra0,"
	elif [ "$RA0" = "No" -o "$RA0" = "N/A" ]
	then
		RA0=""
	fi
	
	if [ "$RA1" = "Yes" ]
	then
		RA1="ra1,"
	elif [ "$RA1" = "No" -o "$RA1" = "N/A" ]
	then
		RA1=""
	fi

	if [ "$RA2" = "Yes" ]
	then
		RA2="ra2,"
	elif [ "$RA2" = "No" -o "$RA2" = "N/A" ]
	then
		RA2=""
	fi

	if [ "$RA3" = "Yes" ]
	then
		RA3="ra3,"
	elif [ "$RA3" = "No" -o "$RA3" = "N/A" ]
	then
		RA3=""
	fi

	if [ "$TOS" = "Normal service" ]
	then
		TOS="NS"
	fi
	if [ "$TOS" = "Minimize delay" ]
	then
		TOS="MD"
	fi
	if [ "$TOS" = "Maximize throughput" ]
	then
		TOS="MT"
	fi
	if [ "$TOS" = "Maximize reliability" ]
	then
		TOS="MR"
	fi
	if [ "$TOS" = "Minimize monetary cost" ]
	then
		TOS="MC"
	fi

	if [ "$PROTOCOLID" = "" ]
	then
		PROTOCOLID="&nbsp;"
	fi

echo "<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$ACTIVATE</td><td align=\"center\" class=\"tabdata\">&nbsp;$ENET0$ENET1$ENET2$ENET3$USB0$RA0$RA1$RA2$RA3</td><td align=\"center\" class=\"tabdata\"><div>$DESMAC</div><div>$DESIP/$DESMASK</div><div>$DESPORTRANGEBEGIN~$DESPORTRANGEEND</div></td><td align=\"center\" class=\"tabdata\"><div>$SRCMAC</div><div>$SRCIP/$SRCMASK</div><div>$SRCPORTRANGEBEGIN~$SRCPORTRANGEEND</div></td><td align=\"center\" class=\"tabdata\">$PROTOCOLID</td><td align=\"center\" class=\"tabdata\"><div>$VLANIDRANGEBEGIN</div><div>|</div><div>$VLANIDRANGEEND</div></td>"

	if [ "$IPPDS" = "IPPTOS" ]
	then
		echo "								<td align=\"center\" class=\"tabdata\">$IPPRANGEBEGIN~$IPPRANGEEND/$TOS</td>"
	elif [ "$IPPDS" = "DSCP" ]
	then
		echo "								<td align=\"center\" class=\"tabdata\">$DSCPRANGEBEGIN~$DSCPRANGEEND</td>"
	elif [ "$IPPDS" = "N/A" ]
	then
		echo "								<td align=\"center\" class=\"tabdata\">-/-</td>"
	fi

echo "<td align=\"center\" class=\"tabdata\"><div>$dotpBEGIN</div><div>|</div><div>$dotpEND</div></td></tr>"

done

echo "</table></td><td height=\"115\" bgcolor=\"ffff99\" class=\"tabdata\"><table width=\"190\" height=\"115\" border=1 align=center cellpadding=1 cellspacing=1><tr height=\"65\"><td class=\"tabdata\" align=center><div><font color=\"#000000\">IPP/TOS</font></div><div><font color=\"#000000\">(DSCP)</font></div><div><font color=\"#000000\">Remarking</font></div></td><td class=\"tabdata\" align=center><div><font color=\"#000000\">802.1p</font></div><div><font color=\"#000000\">Remarking</font></div></td><td class=\"tabdata\" align=center><font color=\"#000000\">Queue #</font></td></tr>"

for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
	node_name="QoS_Entry$j"
	ACTIVATE=`/userfs/bin/tcapi get $node_name Active`
	ACTIPPDS=`/userfs/bin/tcapi get $node_name ActIPPDSField`
	ACTIPP=`/userfs/bin/tcapi get $node_name ActIPPRemarking`
	ACTTOS=`/userfs/bin/tcapi get $node_name ActToSRemarking`
	ACTDSCP=`/userfs/bin/tcapi get $node_name ActDSCPRemarking`
	ACT8021PNUM=`/userfs/bin/tcapi get $node_name Act8021pRemarkingNum`
	ACT8021P=`/userfs/bin/tcapi get $node_name Act8021pRemarking`
	ACTQUEUE=`/userfs/bin/tcapi get $node_name ActQueue`

	if [ "$ACTIVATE" = "no attribute information" -o "$ACTIVATE" = "no node information" -o "$ACTIVATE" = "N/A" ]
	then
		ACTIVATE="N/A"
		continue
	fi

	if [ "$ACTIPPDS" = "no attribute information" -o "$ACTIPPDS" = "no node information" ]
	then
		ACTIPPDS="N/A"
	fi
	if [ "$ACTIPP" = "no attribute information" -o "$ACTIPP" = "no node information" ]
	then
		ACTIPP="N/A"
	fi
	if [ "$ACTTOS" = "no attribute information" -o "$ACTTOS" = "no node information" ]
	then
		ACTTOS="N/A"
	fi
	if [ "$ACTDSCP" = "no attribute information" -o "$ACTDSCP" = "no node information" ]
	then
		ACTDSCP="N/A"
	fi
	if [ "$ACT8021PNUM" = "no attribute information" -o "$ACT8021PNUM" = "no node information" ]
	then
		ACT8021PNUM="N/A"
	fi
	if [ "$ACT8021P" = "no attribute information" -o "$ACT8021P" = "no node information" ]
	then
		ACT8021P="N/A"
	fi
	if [ "$ACTQUEUE" = "no attribute information" -o "$ACTQUEUE" = "no node information" ]
	then
		ACTQUEUE="N/A"
	fi
	
	if [ "$ACTQUEUE" = "Low" ]
	then
		ACTQUEUE="L"
	fi
	if [ "$ACTQUEUE" = "Medium" ]
	then
		ACTQUEUE="M"
	fi
	if [ "$ACTQUEUE" = "High" ]
	then
		ACTQUEUE="H"
	fi
	if [ "$ACTQUEUE" = "Highest" ]
	then
		ACTQUEUE="HH"
	fi
	if [ "$ACTQUEUE" = ""]
	then
		ACTQUEUE="&nbsp;"
	fi
	
	if [ "$ACTTOS" = "Normal service" ]
	then
		ACTTOS="NS"
	elif [ "$ACTTOS" = "Minimize delay" ]
	then
		ACTTOS="MD"
	elif [ "$ACTTOS" = "Maximize throughput" ]
	then
		ACTTOS="MT"
	elif [ "$ACTTOS" = "Maximize reliability" ]
	then
		ACTTOS="MR"
	elif [ "$ACTTOS" = "Minimize monetary cost" ]
	then
		ACTTOS="MC"
	elif [ "$ACTTOS" = "" ]
	then
		ACTTOS="&nbsp;"
	fi

	if [ "$ACTDSCP" = "" ]
	then
		ACTDSCP="&nbsp;"
	fi
	if [ "$ACTIPP" = "" ]
	then
		ACTIPP="&nbsp;"
	fi
	if [ "$ACT8021P" = "" ]
	then
		ACT8021P="&nbsp;"
	fi
	
echo "							<tr height=\"46\">"

	if [ "$ACTIPPDS" = "IPPTOS" ]
	then
		echo "								<td align=\"center\" class=\"tabdata\">$ACTIPP/$ACTTOS</td>"
	elif [ "$ACTIPPDS" = "DSCP" ]
	then
		echo "								<td align=\"center\" class=\"tabdata\">$ACTDSCP</td>"
	elif [ "$ACTIPPDS" = "N/A" ]
	then
		echo "								<td align=\"center\" class=\"tabdata\">N/A</td>"
	fi
	
echo "<td align=\"center\" class=\"tabdata\">$ACT8021P</td><td align=\"center\" class=\"tabdata\">$ACTQUEUE</td></tr>"

done

echo "</table></td></tr></table><table width=\"820\" height=\"15\" align=center class=\"orange\" cellpadding=\"0\" cellspacing=\"0\"><tr><td height=\"10\" valign=\"baseline\" class=\"tabdata\">e:ethernet,usb:USB,ra:wlan,NS: Normal service, MD: Minimize delay, MT: Maximize throughput, MR: Maximize reliability, MC: Minimize monetary cost, HH: Highest, H: High, M: Medium, L: Low.</td></tr><tr><td valign=\"baseline\" class=\"orange\"> </td></tr></table></td></tr></table></body></html>"
