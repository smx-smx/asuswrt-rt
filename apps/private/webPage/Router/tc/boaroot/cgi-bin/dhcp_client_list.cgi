#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><script language=\"JavaScript\"></script><body><table id=dhcplist width=\"580\" border=\"1\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr height=\"30\"><td width=\"40\" align=center class=\"tabdata\"><strong>#</strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Hostname</font></strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">IP Address</font></strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">MAC Address</font></strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Expire Time</font></strong></td></tr>"
Lease_num=`/userfs/bin/tcapi get DhcpLease LeaseNum`
j=0
tmpstr=""
while [ "$j" -lt "$Lease_num" ]
do
	node_name="DhcpLease_Entry$j"
	IP=`/userfs/bin/tcapi get $node_name IP`
	MAC=`/userfs/bin/tcapi get $node_name MAC`
	ExpireDays=`/userfs/bin/tcapi get $node_name ExpireDay`
	ExpireTime=`/userfs/bin/tcapi get $node_name ExpireTime`
	Hostname=`/userfs/bin/tcapi get $node_name HostName`
	case "$IP" in
        "no attribute information" | "no node information" )
		IP="N/A";;
	esac
	case "$MAC" in
	"no attribute information" | "no node information" )
		MAC="N/A";;
	esac
	case "$ExpireDay" in
	"no attribute information" | "no node information" )
		Expire="N/A";;
	esac
	case "$ExpireTime" in
	"no attribute information" | "no node information" )
		Expire="N/A";;
	esac
	Expire="$ExpireDays Days $ExpireTime"
	tmpstr="$tmpstr<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$Hostname</td><td align=\"center\" class=\"tabdata\">$IP</td><td align=\"center\" class=\"tabdata\">$MAC</td><td align=\"center\" class=\"tabdata\">$Expire</td></tr>"
	j=`expr $j + 1` 
done

echo "$tmpstr</table></body></html>"

