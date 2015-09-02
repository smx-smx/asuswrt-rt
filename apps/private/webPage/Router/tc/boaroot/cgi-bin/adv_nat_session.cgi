#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><script language=\"JavaScript\">function removeClick(idx){top.main.document.NAT_Session_form.NATTAB_Session_index.value = idx;top.main.document.NAT_Session_form.NATSession_submitFlag.value = 1;	top.main.document.NAT_Session_form.submit();}</script><body><TABLE borderColor=#cccccc cellSpacing=0 cellPadding=1 bgColor=#ffffff border=1><TR height=30><TD class=tabdata align=middle width=150><STRONG>IP Address </STRONG></TD><TD class=tabdata align=middle width=150><STRONG>Session number </STRONG></TD><TD class=tabdata align=middle width=100><STRONG>Remove </STRONG></TD></TR>"
maxNatSession_num=`10`
i=0
tmpstr=""
for i in 0 1 2 3 4 5 6 7 8 9
do
	tmpstr=""
	node_name="natSessions_Entry$i"
	IPAddress=`/userfs/bin/tcapi get $node_name IPAddress`
	sessionNum=`/userfs/bin/tcapi get $node_name sessionNum`
	case "$IPAddress" in
        "no attribute information" | "no node information" )
		IPAddress="N/A";;
	esac
	if [ "$IPAddress" != "N/A" ]
	then
	tmpstr="<TR height=\"20\"><TD class=tabdata align=middle width=150>$IPAddress</TD><TD class=tabdata align=middle width=150>$sessionNum</TD><TD class=tabdata align=middle><INPUT onclick=removeClick($i) type=button value=remove></TD></TR>"
    	fi
    	echo "$tmpstr"
done
echo "</table></body></html>"
