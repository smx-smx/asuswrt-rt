#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\" tppabs=\"http://192.168.1.1/css/style.css\"><style type=\"text/css\"><!--.style1 {color: #666666}.style3 {	font-size: 12pt;	font-weight: bold;}.style4 {color: #333333}--></style></head><body topmargin=\"10\" leftmargin=\"0\"><form id=\"pvclist_form\" method=\"post\"><table width=\"590\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\"><tr><td width=\"590\" height=\"5\" valign=\"baseline\" class=\"orange\"></td></tr></table><table border=\"0\" width=\"590\" align=\"center\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"200\" height=\"50\" align=\"center\" valign=\"middle\" bgcolor=\"#FFFFFF\"><div align=\"left\"><img src=\"/logo.gif\" width=\"200\" height=\"50\"></div></td><td width=\"500\" align=\"right\" valign=\"bottom\" bgcolor=\"#FFFFFF\" class=\"model\">ADSL Router</td></tr></table><table width=\"590\" height=\"2\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" class=\"orange\"><tr><td width=\"590\" valign=\"baseline\" class=\"orange\"></td></tr></table><table width=\"590\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" color=\"#FF9933\"><tr><td width=\"590\" height=\"35\"  bgcolor=\"#FFFFFF\" class=\"headline\">Service Information Summary</td></tr><tr><td></table><table width=\"590\" border=\"1\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr height=\"30\"><td width=\"55\"  align=\"center\" class=\"tabdata\"><strong>#</strong></div></td><td width=\"70\"  align=\"center\" class=\"tabdata\"><strong>Active</strong></td><td width=\"50\"  align=\"center\" class=\"tabdata\"><strong>VPI</strong></td><td width=\"50\"  align=\"center\" class=\"tabdata\"><strong>VCI</strong></td><td width=\"200\"  align=\"center\" class=\"tabdata\"><strong>ENCAP</strong></td><td width=\"120\"  align=\"center\" class=\"tabdata\"><strong>Mux</strong></td><td width=\"120\"  align=\"center\" class=\"tabdata\"><strong>IP Address</strong></td><td width=\"70\"  align=\"center\" class=\"tabdata\"><strong>Status</strong></td></tr>"
tmpstr=""
tcapicom="/userfs/bin/tcapi get"
for j in 0 1 2 3 4 5 6 7
do
       node_name="Wan_PVC$j"
       ACTIVE=`$tcapicom $node_name Active`
       VPI=`$tcapicom $node_name VPI`
       VCI=`$tcapicom $node_name VCI`
       ENCAP=`$tcapicom $node_name ENCAP`
       ENCAPtmp=""
       PPPGETIP=`$tcapicom $node_name PPPGETIP`
	   ISP=`$tcapicom $node_name ISP`
	   IPVERSION=`$tcapicom $node_name IPVERSION`
       for i in $ENCAP
	     do
	       case $i in
	       "LLC" | "LLC\(IPoA\)" | "LLC(IPoA)" )
       			Mux="LLC";;
       			"VC-Mux" )
       			Mux="VC-Mux";;
       			"information")
       			Mux="N/A";;
       			*) ENCAPtmp="$ENCAPtmp $i";;
          esac
	     done
       lineState=`$tcapicom Info_Adsl lineState`
       if [ "$lineState" = "up" -a "$ACTIVE" = "Yes" ]
       then
	       lineState="up"
       else
	       lineState="N/A"
       fi
       
      case "$ENCAPtmp" in
	       "no attribute information")
       			ENCAPtmp="N/A"
          esac
		  
      case "$ISP" in
	       "0")
       			IPADDR="Dynamic"
			;;
	       "1")
				if [ "$IPVERSION" = "IPv6" ]
				then
       				IPADDR=`$tcapicom $node_name IPADDR6`
				elif [ "$IPVERSION" = "IPv4/IPv6" ]
				then
					IPADDR=`$tcapicom $node_name IPADDR`
					IPADDR6=`$tcapicom $node_name IPADDR6`
					IPADDR="$IPADDR / $IPADDR6"
				else
					IPADDR=`$tcapicom $node_name IPADDR`
				fi
			;;
	       "2")
				if [ "$PPPGETIP" = "Dynamic" ]
				then
					IPADDR="Dynamic"
				elif [ "$PPPGETIP" = "Static" ]
				then
					if [ "$IPVERSION" = "IPv6" ]
					then
						IPADDR="Dynamic"
					elif [ "$IPVERSION" = "IPv4/IPv6" ]
					then
						IPADDR=`$tcapicom $node_name IPADDR`
						IPADDR="$IPADDR / Dynamic"
					else
						IPADDR=`$tcapicom $node_name IPADDR`
					fi
				else
					IPADDR="N/A"
				fi
			;;
	       *)
       			IPADDR="N/A"
			;;
      esac

       tmpstr="$tmpstr<tr><td align=\"center\" class=\"tabdata\">$j</td>"
       tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$ACTIVE</td>"
       tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$VPI</td>"
       tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$VCI</td>"
       tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$ENCAPtmp</td>"
       tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$Mux</td>"
       tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$IPADDR</td>"
       tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$lineState</td></tr>"

done
echo "$tmpstr</table><table width=\"590\" height=\"15\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" class=\"orange\"><tr><td width=\"590\" height=\"10\" valign=\"baseline\" bgcolor=\"#FFFFFF\"></td></tr><tr><td width=\"590\" valign=\"baseline\" class=\"orange\"></td></tr></table></form></body></html>"
