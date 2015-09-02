#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><script language=\"JavaScript\">function doDelete(i) {top.main.document.uiViewLanForm.delnum.value=i;top.main.document.uiViewLanForm.submit();}</script><body><table id=dhcplist width=\"380\" border=\"1\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr height=\"30\"><td width=\"40\" align=center class=\"tabdata\"><strong>#</strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">IP Address</font></strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">MAC Address</font></strong></td><td align=center class=\"tabdata\"><strong><font color=\"#000000\">Drop</font></strong></td></tr>"
MaxNum=`/userfs/bin/tcapi get Dhcpd MaxStaticNum`
i=0
j=0
tmpstr=""
while [ "$i" -lt "$MaxNum" ]
do
	node_name="Dhcpd_Entry$i"
	IP=`/userfs/bin/tcapi get $node_name IP`
	MAC=`/userfs/bin/tcapi get $node_name MAC`
	case "$IP" in
        "no attribute information" | "no node information" )
		IP="N/A";;
	esac
	case "$MAC" in
	"no attribute information" | "no node information" )
		MAC="N/A";;
	esac
	if [ "$IP" != "N/A" ]; then
	tmpstr="$tmpstr<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$IP</td><td align=\"center\" class=\"tabdata\">$MAC</td><td align=\"center\" class=\"tabdata\"><IMG src=\"/cross.gif\" onmouseover=\"this.style.cursor='hand'\" onClick=\"doDelete($i);\"></td></tr>"
	j=`expr $j + 1`
	fi
	i=`expr $i + 1` 
done

echo "$tmpstr</table></body></html>"

