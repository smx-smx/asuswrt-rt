#!/bin/sh
echo -e "Content-Type: text/html\n\n<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\">\n</head><script language=\"JavaScript\">"
echo -e "</script><body><FORM METHOD=\"POST\" name=\"VirtualCgiForm\">\n<table id=\"CgiInnerTable\" align=center width=\"440\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">\n<tr height=\"30\"><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Group ID</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Group port</strong></td></tr>"
tcapi_path=/userfs/bin/
node_name="WebCustom_Entry"
is2PSupported=`/userfs/bin/tcapi get $node_name is2PSupported`
for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do 
	node_name="PortBind_Entry$j"
	P0=`/userfs/bin/tcapi get $node_name p0`
	
	if [ "$P0" = "no node information" ]
	then
		continue
	fi
	
	P1=`/userfs/bin/tcapi get $node_name p1`
	P2=`/userfs/bin/tcapi get $node_name p2`
	P3=`/userfs/bin/tcapi get $node_name p3`
	P4=`/userfs/bin/tcapi get $node_name p4`
	P5=`/userfs/bin/tcapi get $node_name p5`
	P6=`/userfs/bin/tcapi get $node_name p6`
	P7=`/userfs/bin/tcapi get $node_name p7`
	E1=`/userfs/bin/tcapi get $node_name e1`
	E2=`/userfs/bin/tcapi get $node_name e2`
	if [ "$is2PSupported" != "Yes" ];then
		E3=`/userfs/bin/tcapi get $node_name e3`
		E4=`/userfs/bin/tcapi get $node_name e4`
	fi
	P8=`/userfs/bin/tcapi get $node_name p8`
	P9=`/userfs/bin/tcapi get $node_name p9`

	P10=`/userfs/bin/tcapi get $node_name p10`

	W0=`/userfs/bin/tcapi get $node_name w0`
	W1=`/userfs/bin/tcapi get $node_name w1`
	W2=`/userfs/bin/tcapi get $node_name w2`
	W3=`/userfs/bin/tcapi get $node_name w3`
	U0=`/userfs/bin/tcapi get $node_name u0`
	
	if [ "$P0" = "Yes" ];then
		P0="p0,"
	else [ "$P0" = "No" -o "$P0" = "N/A" ]
		P0=""
	fi
	
	if [ "$P1" = "Yes" ];then
		P1="p1,"
	else 
		P1=""
	fi
	
	if [ "$P2" = "Yes" ];then
		P2="p2,"
	else 
		P2=""
	fi
	
	if [ "$P3" = "Yes" ];then
		P3="p3,"
	else
		P3=""
	fi
	
	if [ "$P4" = "Yes" ];then
		P4="p4,"
	else
		P4=""
	fi
	
	if [ "$P5" = "Yes" ];then
		P5="p5,"
	else
		P5=""
	fi
	
	if [ "$P6" = "Yes" ];then
		P6="p6,"
	else
		P6=""
	fi
	
	if [ "$P7" = "Yes" ];then
		P7="p7,"
	else
		P7=""
	fi
	
	if [ "$P8" = "Yes" ];then
		P8="p8,"
	else
		P8=""
	fi
	
	if [ "$P9" = "Yes" ];then
		P9="p9,"
	else
		P9=""
	fi
	
	if [ "$P10" = "Yes" ];then
		P10="p10,"
	else
		P10=""
	fi

	if [ "$E1" = "Yes" ];then
		E1="e1,"
	else
		E1=""
	fi
	
	if [ "$E2" = "Yes" ];then
		E2="e2,"
	else
		E2=""
	fi
	
	if [ "$is2PSupported" != "Yes" ];then
		if [ "$E3" = "Yes" ];then
			E3="e3,"
		else
			E3=""
		fi
		
		if [ "$E4" = "Yes" ];then
			E4="e4,"
		else
			E4=""
		fi
	fi
	
	if [ "$W0" = "Yes" ];then
		W0="w0,"
	else
		W0=""
	fi
	
	if [ "$W1" = "Yes" ];then
		W1="w1,"
	else
		W1=""
	fi
	
	if [ "$W2" = "Yes" ];then
		W2="w1,"
	else
		W2=""
	fi
	
	if [ "$W3" = "Yes" ];then
		W3="w1,"
	else
		W3=""
	fi
	
	if [ "$U0" = "Yes" ];then
		U0="u0,"
	else
		U0=""
	fi
	if [ "$is2PSupported" != "Yes" ];then
		javastr="<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$P0$P1$P2$P3$P4$P5$P6$P7$P8$P9$P10$E1$E2$E3$E4$W0$W1$W2$W3$U0</td></tr>"
	else
		javastr="<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$P0$P1$P2$P3$P4$P5$P6$P7$P8$P9$P10$E1$E2$W0$W1$W2$W3$U0</td></tr>"
	fi
	echo $javastr
done
echo "</table></form></body></html>"
