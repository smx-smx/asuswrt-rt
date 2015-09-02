#!/bin/sh
echo -e "Content-Type: text/html\n\n<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\">\n</head>\n<script language=\"JavaScript\">"
echo -e "
function doDelete(i){
	var f=top.main.document.VOIP_MEDIA_form
	f.editnum.value=i;
	f.delFlag.value=1;
	f.submit();
}
function TcCheckSel(selitem, value){
	if(value==\"N/A\"){
		selitem.selectedIndex=0;
	}
	for(k=0;k<selitem.length;k++){
		if(selitem.options[k].value==value){
			selitem.selectedIndex=k;
		break;
		}
	}
}
function add_packtime_option(i){
	var f=top.main.document.VOIP_MEDIA_form.VoIPPacketizationTime.options
	var additem;

	if(i==\"G.723\"){
		additem=new Option(\"30\",\"30\");
		f.add(additem); 
		additem=new Option(\"60\",\"60\");
		f.add(additem);
		additem=new Option(\"90\",\"90\");
		f.add(additem);
	}else{
		additem=new Option(\"10\",\"10\");
		f.add(additem); 
		additem=new Option(\"20\",\"20\");
		f.add(additem);
		additem=new Option(\"30\",\"30\");
		f.add(additem); 
		additem=new Option(\"40\",\"40\");
		f.add(additem);	
	}
}
function changeswitch(i,j){
	var f=top.main.document.VOIP_MEDIA_form

	for(t=f.VoIPPacketizationTime.length;t>=0;t--){
		f.VoIPPacketizationTime.options.remove(t);
	}
	add_packtime_option(i);

	if(i==\"N/A\" || j==\"N/A\"){
		f.VoIPSupportedCodecs.selectedIndex=0;
		if( i == \"G.723\"){
			f.VoIPPacketizationTime.selectedIndex=0;
		}
		else{
			f.VoIPPacketizationTime.selectedIndex=1;
		}
	}else{
		TcCheckSel(f.VoIPSupportedCodecs, i);
		TcCheckSel(f.VoIPPacketizationTime, j);
	}

	switch(top.main.document.VOIP_MEDIA_form.editnum.value){
		case \"0\":
			top.main.document.getElementById('PriorityIndex').innerHTML = 1;
			break;
		case \"1\":
			top.main.document.getElementById('PriorityIndex').innerHTML = 2;
			break;
		case \"2\":
			top.main.document.getElementById('PriorityIndex').innerHTML = 3;
			break;
		case \"3\":
			top.main.document.getElementById('PriorityIndex').innerHTML = 4;
			break;
		case \"4\":
			top.main.document.getElementById('PriorityIndex').innerHTML = 5;
			break;	
		default:
			top.main.document.getElementById('PriorityIndex').innerHTML = 1;
			break;
	}
}
"
echo -e "</script><body><FORM METHOD=\"POST\" name=\"VoIPMediaCgiForm\">\n<table id=\"CgiInnerTable\" align=center width=\"440\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">\n<tr height=\"30\"><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Codecs Priority</strong></td><td width=\"120\" align=\"center\" class=\"tabdata\"><strong>Supported Codecs</strong></td>\n<td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Packetization Time</strong></td>\n<td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Edit</strong></td><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Drop</strong></td></tr>"
tcapi_path=/userfs/bin/
java_str=
for j in 0 1 2 3 4
do 
	node_name="VoIPMedia_Entry1"
	codec_index="SIPSupportedCodecs"$j""
	ptime_index="SIPPacketizationTime"$j""
	Supported_Codecs=`"$tcapi_path"tcapi get $node_name $codec_index`
	case $Supported_Codecs in
		"no node information" | "no attribute information" | "connection error") 
			case $j in
				"0" )	Supported_Codecs="G.711 a-law"
		  			Packetization_Time="20" ;;
				* )	Supported_Codecs="N/A"
		  			Packetization_Time="N/A" ;;
			esac;;
		* )	Packetization_Time=`"$tcapi_path"tcapi get $node_name $ptime_index`	
			case $Packetization_Time in
				"no node information" | "no attribute information" | "connection error" )	Packetization_Time="N/A";;
		  	esac;;
	esac
	javastr="<tr><td align=\"center\" class=\"tabdata\">"
	case $j in 
		"0")	javastr=""$javastr"1st";;
		"1")	javastr=""$javastr"2nd";;
		"2")	javastr=""$javastr"3rd";;
		"3")	javastr=""$javastr"4th";;
		"4")	javastr=""$javastr"5th";;
      	esac	       
	javastr=""$javastr"</td><td align=\"center\" class=\"tabdata\">$Supported_Codecs</td><td align=\"center\" class=\"tabdata\">$Packetization_Time</td><td align=\"center\" class=\"tabdata\"><IMG src=\"/pen.gif\" onmouseover=\"this.style.cursor='hand'\" onClick='javascript:"
	case $Supported_Codecs in
		"N/A")	javastr=""$javastr"top.main.document.VOIP_MEDIA_form.VoIPSupportedCodecs.text=\"G.711 a-law\";" ;;
		*)	javastr=""$javastr"top.main.document.VOIP_MEDIA_form.VoIPSupportedCodecs.text=\"$Supported_Codecs\";" ;;

	esac
	case $Packetization_Time in
		"N/A")	javastr=""$javastr"top.main.document.VOIP_MEDIA_form.VoIPPacketizationTime.text=\"20\";" ;;
		*)	javastr=""$javastr"top.main.document.VOIP_MEDIA_form.VoIPPacketizationTime.text=\"$Packetization_Time\";" ;;
	esac
	if [ $Supported_Codecs = "N/A" ] || [ "$j" = "0" ]
	then
		javastr=""$javastr"top.main.document.VOIP_MEDIA_form.editnum.value=\"$j\"; changeswitch(\""$Supported_Codecs"\",\""$Packetization_Time"\");'></td></tr>"
	else
		javastr=""$javastr"top.main.document.VOIP_MEDIA_form.editnum.value=\"$j\"; changeswitch(\""$Supported_Codecs"\",\""$Packetization_Time"\");'></td><td align=\"center\" class=\"tabdata\"><IMG onClick=\"doDelete($j);\"; src=\"/cross.gif\"></td></tr>"
	fi
	echo $javastr
done
echo "</table></form></body></html>"
