#!/bin/sh
echo -e "Content-Type: text/html\n\n<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\" tppabs=\"http://192.168.1.1/css/style.css\">\n</head><script language=\"JavaScript\">"
echo -e "function doDelete(i){\n\tvar f=top.main.document.IPADDRMAP_form\n\n\tf.editnum.value=i;\n\tf.delFlag.value=1;\n\tf.submit();\n}"
echo -e "function doChange(str){\n\tvar f=top.main.document.IPADDRMAP_form;\n\n\tfor(i=0; i<f.MapEditType.options.length; i++){\n\t\tif(str==f.MapEditType[i].value){\n\t\t\tf.MapEditType[i].selected=true;\n\t\tf.MapEditType.value=str;\n\t\t}\n\t}\n\tchktype(str);\n}"
echo -e "function chktype(str){\n\tvar f=top.main.document.IPADDRMAP_form;\n\tf.IPADDRMAP_localstartIP.disabled=false;\n\tf.IPADDRMAP_localendIP.disabled=false;\n\tf.IPADDRMAP_publicstartIP.disabled=false;\n\tf.IPADDRMAP_publicendIP.disabled=false;\n\ttop.main.document.getElementById(\"localSIP_Div\").style.display=\"\";\n\ttop.main.document.getElementById(\"localEIP_Div\").style.display=\"\";\n\ttop.main.document.getElementById(\"publicSIP_Div\").style.display=\"none\";\n\tif(str==\"One-to-One\"){\n\t\tf.IPADDRMAP_localendIP.disabled=true;\n\t\tf.IPADDRMAP_publicendIP.disabled=true;\n\t\ttop.main.document.getElementById(\"publicSIP_Div\").style.display=\"\";\n\t\ttop.main.document.getElementById(\"localSIP_Div\").style.display=\"none\";\n\t\ttop.main.document.getElementById(\"localEIP_Div\").style.display=\"none\";\n\t}else if(str==\"Many-to-One\"){\n\t\tf.IPADDRMAP_publicendIP.disabled=true;\n\t\ttop.main.document.getElementById(\"publicSIP_Div\").style.display=\"\";\n\t}else if(str==\"Server\"){\n\t\tf.IPADDRMAP_localstartIP.disabled=true;\n\t\tf.IPADDRMAP_localendIP.disabled=true;\n\t\tf.IPADDRMAP_publicstartIP.disabled=false;\n\t\tf.IPADDRMAP_publicendIP.disabled=true;\n\t\ttop.main.document.getElementById(\"publicSIP_Div\").style.display=\"\";\n\t\ttop.main.document.getElementById(\"localSIP_Div\").style.display=\"none\";\n\t\ttop.main.document.getElementById(\"localEIP_Div\").style.display=\"none\";\n\t}\n}"
echo -e "</script><body><FORM METHOD=\"POST\" name=\"StatusLogForm\">\n<table align=center width=\"500\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">\n<tr height=\"30\"><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Rule</strong></td><td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Type</strong></td>\n<td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Local Start IP</strong></td><td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Local End IP</strong></td>\n<td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Public Start IP</strong></td><td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Public End IP</strong></td>\n<td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Edit</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Drop</strong></td></tr>"
tcapi_path=/userfs/bin/
cur_pvc=`"$tcapi_path"tcapi get WebCurSet_Entry nat_pvc`
java_str=
for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do       
       node_name="IpAddrMap_PVC"$cur_pvc"_Entry"$j""
       RULETYPE=`"$tcapi_path"tcapi get $node_name RULETYPE`
	case $RULETYPE in
		"no node information" | "no attribute information" | "connection error") RULETYPE="N/A"
			LOCALSTARTIP="N/A"
         		LOCALENDIP="N/A"
			PUBLICSTARTIP="N/A"
        		PUBLICENDIP="N/A" ;;
       *)
	 LOCALSTARTIP=`"$tcapi_path"tcapi get $node_name LOCALSTARTIP`
         LOCALENDIP=`"$tcapi_path"tcapi get $node_name LOCALENDIP`
         PUBLICSTARTIP=`"$tcapi_path"tcapi get $node_name PUBLICSTARTIP`
         PUBLICENDIP=`"$tcapi_path"tcapi get $node_name PUBLICENDIP`
	 case $LOCALSTARTIP in
		"no node information" | "no attribute information" | "connection error") LOCALSTARTIP="N/A" ;;
	 esac
	 case $LOCALENDIP in
		"no node information" | "no attribute information" | "connection error") LOCALENDIP="N/A" ;;
	 esac
	 case $PUBLICSTARTIP in
		"no node information" | "no attribute information" | "connection error") PUBLICSTARTIP="N/A" ;;
	 esac
	 case $PUBLICENDIP in
		"no node information" | "no attribute information" | "connection error") PUBLICENDIP="N/A" ;;
	 esac;;
 	esac 
       java_str="<tr><td align=\"center\" class=\"tabdata\">$j</td><td align=\"center\" class=\"tabdata\">$RULETYPE</td><td align=\"center\" class=\"tabdata\">$LOCALSTARTIP</td><td align=\"center\" class=\"tabdata\">$LOCALENDIP</td><td align=\"center\" class=\"tabdata\">$PUBLICSTARTIP</td><td align=\"center\" class=\"tabdata\">$PUBLICENDIP</td><td align=\"center\" class=\"tabdata\"><IMG src=\"/pen.gif\" onmouseover=\"this.style.cursor='hand'\" onClick='javascript:"
	case $RULETYPE in
		"N/A") ;;
		*)java_str=""$java_str"doChange(\"$RULETYPE\");";;
	 esac
        case $LOCALSTARTIP in
		"N/A") java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_localstartIP.value=\"0.0.0.0\";" ;;
		*) java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_localstartIP.value=\"$LOCALSTARTIP\";" ;;
	 esac 
	 case $LOCALENDIP in
		"N/A") java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_localendIP.value=\"0.0.0.0\";" ;;
		*) java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_localendIP.value=\"$LOCALENDIP\";" ;;
	 esac
       case $PUBLICSTARTIP in
		"N/A") java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_publicstartIP.value=\"0.0.0.0\";" ;;
		*) java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_publicstartIP.value=\"$PUBLICSTARTIP\";"  ;;
	 esac
	case $PUBLICENDIP in
		"N/A") java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_publicendIP.value=\"0.0.0.0\";" ;;
		*) java_str=""$java_str"top.main.document.IPADDRMAP_form.IPADDRMAP_publicendIP.value=\"$PUBLICENDIP\";"  ;;
	 esac
	case $RULETYPE in
		"N/A")java_str=""$java_str"top.main.document.IPADDRMAP_form.editnum.value=\"$j\";'></tr>"  ;;
		*)java_str=""$java_str"top.main.document.IPADDRMAP_form.editnum.value=\"$j\";'></td><td align=\"center\" class=\"tabdata\"><IMG onClick=\"doDelete($j);\"; src=\"/cross.gif\"></td></tr>" ;;
	 esac
echo $java_str
done
echo "</table></form></body></html>"
