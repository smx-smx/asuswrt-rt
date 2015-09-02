#!/bin/sh
echo -e "Content-Type: text/html\n"
echo "<html><head><meta http-equiv=Content-Script-Type content=text/javascript><meta http-equiv=Content-Style-Type content=text/css><meta http-equiv=Content-Type content=\"text/html hhh; charset=iso-8859-1\"><link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\"></head><script language=\"JavaScript\">function CtrClickSetting(index,checked){ var f = parent.document.PORTTRIGGERINGList_form; switch(index){ case 0:if(checked) f.PortTriggeringList_Cntr0.value = 1;else f.PortTriggeringList_Cntr0.value = 0; break; case 1: if(checked) f.PortTriggeringList_Cntr1.value = 1; else f.PortTriggeringList_Cntr1.value = 0; break; case 2: if(checked) f.PortTriggeringList_Cntr2.value = 1; else f.PortTriggeringList_Cntr2.value = 0; break; case 3: if(checked) f.PortTriggeringList_Cntr3.value = 1; else f.PortTriggeringList_Cntr3.value = 0; break; case 4: if(checked) f.PortTriggeringList_Cntr4.value = 1; else f.PortTriggeringList_Cntr4.value = 0; break; case 5: if(checked) f.PortTriggeringList_Cntr5.value = 1; else f.PortTriggeringList_Cntr5.value = 0; break; case 6: if(checked) f.PortTriggeringList_Cntr6.value = 1; else f.PortTriggeringList_Cntr6.value = 0; break; case 7: if(checked) f.PortTriggeringList_Cntr7.value = 1; else f.PortTriggeringList_Cntr7.value = 0; break; }}"
echo -e "\tvar virstrtmp = parent.ary_strings;\n\tvar vir_obj = {};\n\tfor(var i=0; virstrtmp[i][0] != \"\"; i++) vir_obj[virstrtmp[i][0]]=virstrtmp[i][1];\n"
echo "</script><body><table width=\"100%\" height=\"100%\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><tr height=\"10%\"><td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerClear\"]);</script></font></strong></td>"
echo "<td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerApp\"]);</script></font></strong></td><td colspan=\"3\" align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerRag\"]);</script></font></strong></td><td colspan=\"3\" align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerOpen\"]);</script></font></strong></td></tr>"
echo "<tr><td align=center class=\"tabdata\">&nbsp;</td><td align=center class=\"tabdata\">&nbsp;</td><td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerSPort\"]);</script></font></strong></td><td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerEPort\"]);</script></font></strong></td><td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerProtocol\"]);</script></font></strong></td>"
echo "<td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerSPort\"]);</script></font></strong></td><td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerEPort\"]);</script></font></strong></td><td align=center class=\"tabdata\"><strong><font color=\"#000000\"><script>document.writeln(vir_obj[\"porttriggerProtocol\"]);</script></font></strong></td></tr>"
i=0
tmpstr=""
for i in 0 1 2 3 4 5 6 7
do
	tmpstr=""
	node_name="portTriggering_Entry$i"
	Name=`/userfs/bin/tcapi staticGet $node_name Name`
	TSPort=`/userfs/bin/tcapi staticGet $node_name TSPort`
	TEPort=`/userfs/bin/tcapi staticGet $node_name TEPort`
	TProtocol=`/userfs/bin/tcapi staticGet $node_name TProtocol`
	OSPort=`/userfs/bin/tcapi staticGet $node_name OSPort`
	OEPort=`/userfs/bin/tcapi staticGet $node_name OEPort`
	OProtocol=`/userfs/bin/tcapi staticGet $node_name OProtocol`
	case $Name in
		"no node information" | "no attribute information" | "connection error") Name="-";;
	esac
	case $TSPort in
		"no node information" | "no attribute information" | "connection error") TSPort=0;;
	esac
	case $TEPort in
		"no node information" | "no attribute information" | "connection error") TEPort=0;;
	esac
	case $TProtocol in
		"no node information" | "no attribute information" | "connection error") TProtocol="-";;
	esac
	case $OSPort in
		"no node information" | "no attribute information" | "connection error") OSPort=0;;
	esac
	case $OEPort in
		"no node information" | "no attribute information" | "connection error") OEPort=0;;
	esac
	case $OProtocol in
		"no node information" | "no attribute information" | "connection error") OProtocol="-";;
	esac
	if [ $Name = "Aim Talk" ]
	then
		Name="<script>document.writeln(vir_obj[\"porttriggerAim\"]);</script>"
	elif [ "$Name" = "Asheron's Call" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerAsh\"]);</script>"
	elif [ "$Name" = "Calista IPPhone" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerCal\"]);</script>"
	elif [ "$Name" = "Delta Force" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerDel\"]);</script>"
	elif [ "$Name" = "ICQ" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerICQ\"]);</script>"
	elif [ "$Name" = "Napster" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerNap\"]);</script>"	
	elif [ "$Name" = "Net2Phone" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerNet\"]);</script>"
	elif [ "$Name" = "QuickTimeClient" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerQui\"]);</script>"
	elif [ "$Name" = "Rainbow 6 Game" ]
	then	
		Name="<script>document.writeln(vir_obj[\"porttriggerRai\"]);</script>"						
	fi
	tmpstr="<tr><td align=\"center\" class=\"tabdata\"><INPUT TYPE=\"CHECKBOX\" NAME=\"PortTriggeringListClear$i\" onClick=CtrClickSetting($i,this.checked);></td><td align=\"center\" class=\"tabdata\">$Name</td><td align=\"center\" class=\"tabdata\">$TSPort</td><td align=\"center\" class=\"tabdata\">$TEPort</td><td align=\"center\" class=\"tabdata\">$TProtocol</td><td align=\"center\" class=\"tabdata\">$OSPort</td><td align=\"center\" class=\"tabdata\">$OEPort</td><td align=\"center\" class=\"tabdata\">$OProtocol</td></tr>"
    	echo "$tmpstr"
done
echo "</table></body></html>"
