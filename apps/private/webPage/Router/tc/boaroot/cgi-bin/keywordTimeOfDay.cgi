#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><body><table width=\"100%\" border=\"1\" align=center cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr><td width=\"15%\" align=center class=\"tabdata\"><strong>Index</strong></td><td align=center class=\"tabdata\"><strong>Keyword</strong></td></tr>"
maxNatSession_num=`10`
i=0
tmpstr=""
for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
	tmpstr=""
	node_name="keywordTime_Entry$i"
	keyword=`/userfs/bin/tcapi get $node_name keyword`
	case "$keyword" in
        "no attribute information" | "no node information" )
		keyword="";;
	esac
	tmpstr="<tr><td align=center class=\"tabdata\">$i</td><td class=\"tabdata\">$keyword</td></tr>"
    	echo "$tmpstr"
done
echo "</table></body></html>"
