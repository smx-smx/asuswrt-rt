#!/bin/sh

tcapicom="/userfs/bin/tcapi get"
TRANSMODE=`$tcapicom Wan_Common TransMode`
j=`$tcapicom WebCurSet_Entry wan_pvc`
case "$TRANSMODE" in
	"Ethernet")
		STRING="WAN0";;
	"PTM")
		case "$j" in
			"8")
				STRING="PTM Barrier 0";;
			"9")
				STRING="PTM Barrier 1";;
		esac;;			
esac		
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\" tppabs=\"http://192.168.1.1/css/style.css\"><style type=\"text/css\"></style></head><body topmargin=\"10\" leftmargin=\"0\"><form id=\"pvclist_form\" method=\"post\"><table width=\"590\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\"><tr><td width=\"590\" height=\"5\" valign=\"baseline\" class=\"orange\"></td></tr></table><table border=\"0\" width=\"590\" align=\"center\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"200\" height=\"50\" align=\"center\" valign=\"middle\" bgcolor=\"#FFFFFF\"><div align=\"left\"><img src=\"/logo.gif\" width=\"200\" height=\"50\"></div></td><td width=\"500\" align=\"right\" valign=\"bottom\" bgcolor=\"#FFFFFF\" class=\"model\">ADSL Router</td></tr></table><table width=\"590\" height=\"2\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" class=\"orange\"><tr><td width=\"590\" valign=\"baseline\" class=\"orange\"></td></tr></table><table width=\"590\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" color=\"#FF9933\"><tr><td width=\"590\" height=\"35\" bgcolor=\"#FFFFFF\" class=\"headline\">Service Information Summary</td></tr><tr><td></table><table width=\"590\" border=\"1\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr height=\"30\"><td width=\"90\" align=\"center\" class=\"tabdata\"><strong>"$STRING"</strong></div></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Active</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>ISP</strong></td><td width=\"120\" align=\"center\" class=\"tabdata\"><strong>IP Address</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Status</strong></td></tr>"
tmpstr=""
for i in 0 1 2 3 4 5 6 7
do	
	node_name="WanExt_PVC"$j"e"$i""
	ACTIVE=`$tcapicom $node_name Active`

	if [ "$ACTIVE" != "Yes" ];then
		ACTIVE="No"
	fi	

	if [ "$ACTIVE" = "No" ];then
		lineState="N/A"
		IPADDR="N/A"
		IPADDR6="N/A"
		ISP="N/A"
	else
		PPPGETIP=`$tcapicom $node_name PPPGETIP`
		ISP=`$tcapicom $node_name ISP`
		IPVERSION=`$tcapicom $node_name IPVERSION`
		lineState=`$tcapicom Info_Adsl lineState`

		if [ "$lineState" = "up" -a "$ACTIVE" = "Yes" ];then
			lineState="up"
		else
			lineState="N/A"
		fi
		case "$ISP" in
			"0")
				ISP="Dynamic"
				IPADDR="Dynamic";;
			"1")
				ISP="Static"
				if [ "$IPVERSION" = "IPv6" ];then
					IPADDR=`$tcapicom $node_name IPADDR6`
				elif [ "$IPVERSION" = "IPv4/IPv6" ];then
					IPADDR=`$tcapicom $node_name IPADDR`
					IPADDR6=`$tcapicom $node_name IPADDR6`
					IPADDR="$IPADDR / $IPADDR6"
				else
					IPADDR=`$tcapicom $node_name IPADDR`
				fi;;
			"2")
				ISP="PPPoE"
				if [ "$PPPGETIP" = "Dynamic" ];then
					IPADDR="Dynamic"
				elif [ "$PPPGETIP" = "Static" ];then
					if [ "$IPVERSION" = "IPv6" ];then
						IPADDR="Dynamic"
					elif [ "$IPVERSION" = "IPv4/IPv6" ];then
						IPADDR=`$tcapicom $node_name IPADDR`
						IPADDR="$IPADDR / Dynamic"
					else
						IPADDR=`$tcapicom $node_name IPADDR`
					fi
				else
					IPADDR="N/A"
				fi;;
			*)
				ISP="Bridge"
				IPADDR="N/A";;
		esac
	fi

	tmpstr="$tmpstr<tr><td align=\"center\" class=\"tabdata\">$i</td>"
	tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$ACTIVE</td>"
	tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$ISP</td>"
	tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$IPADDR</td>"
	tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">$lineState</td></tr>"
done
echo "$tmpstr</table><table width=\"590\" height=\"15\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" class=\"orange\"><tr><td width=\"590\" height=\"10\" valign=\"baseline\" bgcolor=\"#FFFFFF\"></td></tr><tr><td width=\"590\" valign=\"baseline\" class=\"orange\"></td></tr></table></form></body></html>"
