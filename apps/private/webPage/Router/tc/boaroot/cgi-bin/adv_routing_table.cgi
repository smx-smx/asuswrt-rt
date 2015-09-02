#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><script language=\"JavaScript\">function doDelete(i) {top.main.document.RoutingTable_form.delnum.value=i;top.main.document.RoutingTable_form.submit();}function doedit(i) {top.main.document.RoutingTable_form.action=\"/cgi-bin/adv_static_route.asp\";top.main.document.RoutingTable_form.editnum.value=i;top.main.document.RoutingTable_form.submit();}</script><body><table width=\"580\" border=\"1\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr height=\"30\"><td width=\"40\" align=center class=\"tabdata\"><strong>#</strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Dest IP</font></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Mask</font></strong></td><td width=\"110\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Gateway IP</font></strong></td><td width=\"50\"  align=center class=\"tabdata\"><strong><font color=\"#000000\">Metric</font></strong></td><td width=\"70\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Device</font></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Use</font></strong></td><td width=\"45\" align=center class=\"tabdata\"><strong><font color=\"#000000\">Edit</font></strong></td><td align=center class=\"tabdata\"><strong><font color=\"#000000\">Drop</font></strong></td></tr>"
routeindex=`/userfs/bin/tcapi get WebCurSet_Entry route_id`
Route_num=`/userfs/bin/tcapi get Route Route_num`
i=0
j=0
tmpstr=""
while [ "$j" -lt "$Route_num" ]
do
	node_name="Route_Entry$i"
	DST_IP=`/userfs/bin/tcapi get $node_name DST_IP`
	Sub_mask=`/userfs/bin/tcapi get $node_name Sub_mask`
	Device=`/userfs/bin/tcapi get $node_name Device`
	Gateway=`/userfs/bin/tcapi get $node_name Gateway`
	metric=`/userfs/bin/tcapi get $node_name metric`
	User_def=`/userfs/bin/tcapi get $node_name User_def`
	Use=`/userfs/bin/tcapi get $node_name Use`
	case "$DST_IP" in
        "no attribute information" | "no node information" )
		DST_IP="N/A";;
	esac
	if [ "$DST_IP" != "N/A" ]
	then	
	case "$Sub_mask" in
	"no attribute information" | "no node information" )
		Sub_mask="N/A";;
	esac
	case "$Device" in
	"no attribute information" | "no node information" | "")
		Device="N/A";;
	esac
	case "$Gateway" in
	"no attribute information" | "no node information" | "")
		Gateway="N/A";;
	esac
	case "$metric" in
	"no attribute information" | "no node information" )
		metric="N/A";;
	esac
	case "$Use" in
	"no attribute information" | "no node information" )
		Use="0";;
	esac
	tmpstr="$tmpstr<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$DST_IP</td><td align=\"center\" class=\"tabdata\">$Sub_mask</td><td align=\"center\" class=\"tabdata\">$Gateway</td><td align=\"center\" class=\"tabdata\">$metric</td><td align=\"center\" class=\"tabdata\">$Device</td><td align=\"center\" class=\"tabdata\">$Use</td>"
	case "$User_def" in
	"0" ) tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\">&nbsp;</td><td align=\"center\" class=\"tabdata\">&nbsp;</td></tr>" ;;
	"1" ) tmpstr="$tmpstr<td align=\"center\" class=\"tabdata\"><IMG  src=\"/pen.gif\" onmouseover=\"this.style.cursor='hand'\" onClick=\"doedit($i)\"></td><td align=\"center\" class=\"tabdata\"><IMG src=\"/cross.gif\" onmouseover=\"this.style.cursor='hand'\" onClick=\"doDelete($i);\"></td></tr>" ;;
	esac
	j=`expr $j + 1` 
    fi
	i=`expr $i + 1` 
done
echo "$tmpstr</table></body></html>"

