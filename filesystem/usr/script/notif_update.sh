#!/bin/sh

forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
echo "forsq = ${forsq}" > /tmp/notif.log #1 for sq
model=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
echo "model = ${model}" >> /tmp/notif.log #DSL-N66U

#current info
fv=`/userfs/bin/tcapi get SysInfo_Entry InnerVersion`
fv=`echo $fv | sed s/'\.'//g;`
flag_orig=`/userfs/bin/tcapi get WebCustom_Entry webs_notif_flag`
index_orig=`/userfs/bin/tcapi get WebCustom_Entry webs_notif_index`

/userfs/bin/tcapi set WebCustom_Entry webs_notif_update 0 & # INITIALIZING
/userfs/bin/tcapi set WebCustom_Entry webs_notif_error 0 &
if [ "$flag_orig" = "" ];then
	/userfs/bin/tcapi set WebCustom_Entry webs_notif_flag "" &
fi
if [ "$index_orig" = "" ];then
	/userfs/bin/tcapi set WebCustom_Entry webs_notif_index "" &
fi	
/userfs/bin/tcapi set WebCustom_Entry webs_notif_info1 "" &
/userfs/bin/tcapi set WebCustom_Entry webs_notif_info2 "" &
/userfs/bin/tcapi set WebCustom_Entry webs_notif_error_msg "" &

if [ "$forsq" = "1" ]; then
  wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/dslasuswrt_notif.zip -O /tmp/dslasuswrt_notif.txt
else
	wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless/dslasuswrt_notif.zip -O /tmp/dslasuswrt_notif.txt
fi

if [ "$?" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_notif_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_notif_error_msg "download dslasuswrt_notif.zip fail" &
else
	filename='/tmp/dslasuswrt_notif.txt'
	exec < $filename
	while read line
	do		
		if [ "`echo $line | grep INDEX`" != "" ]; then
			index=`echo $line | grep INDEX | sed 's/.*#SN_//;'`
			echo "---- index : $index ----" >> /tmp/notif.log
			if [ "$forsq" = "1" ]; then
					/userfs/bin/tcapi set WebCustom_Entry webs_notif_flag 1 &
			elif [ "`/userfs/bin/tcapi get WebCustom_Entry webs_notif_index`" != "$index" ]; then
					/userfs/bin/tcapi set WebCustom_Entry webs_notif_flag 1 &
			fi
			if [ "$forsq" = "1" ]; then
				/userfs/bin/tcapi set WebCustom_Entry webs_notif_index "0000" &
			else
				/userfs/bin/tcapi set WebCustom_Entry webs_notif_index "$index" &
			fi
		fi			
		if [ "`echo $line | grep $model`" != "" ]; then	#Parsing single model
				EQ_qual=`echo $line | grep $model | sed 's/.*#EQ_//;' | sed 's/#.*//;'`
				firmver=`echo $line | grep $model | sed 's/.*#FW//;' | sed 's/#.*//;'`
				if [ "$EQ_qual" = "E" ]; then
						if [ "$fv" = "$firmver" ]; then
							hint=$hint++`echo $line | grep $model | sed 's/.*#NC_//;' | sed 's/#.*//;'`
						fi
				elif [ "$EQ_qual" = "EL" ]; then
						if [ "$fv" -lt "$firmver" ] || [ "$fv" = "$firmver" ]; then							
							hint=$hint++`echo $line | grep $model | sed 's/.*#NC_//;' | sed 's/#.*//;'`
						fi
				fi				
		fi		
		if [ "`echo $line | grep DSLASUSWRT_ALL`" != "" ]; then	#Parsing DSLASUSWRT ALL model
				EQ_qual=`echo $line | grep DSLASUSWRT_ALL | sed 's/.*#EQ_//;' | sed 's/#.*//;'`				
				firmver=`echo $line | grep DSLASUSWRT_ALL | sed 's/.*#FW//;' | sed 's/#.*//;'`
				if [ "$EQ_qual" = "E" ]; then
						if [ "$fv" = "$firmver" ]; then
							hint=$hint++`echo $line | grep DSLASUSWRT_ALL | sed 's/.*#NC_//;' | sed 's/#.*//;'`
						fi				
				elif [ "$EQ_qual" = "EL" ]; then
						if [ "$fv" -lt "$firmver" ] || [ "$fv" = "$firmver" ]; then
							hint=$hint++`echo $line | grep DSLASUSWRT_ALL | sed 's/.*#NC_//;' | sed 's/#.*//;'`
						fi
				fi
		fi
	done	
	echo "---- ${#hint} : $hint ----" >> /tmp/notif.log
	hint1=`echo $hint | cut -c 1-500`
	hint2=`echo $hint | cut -c 501-1000`
	/userfs/bin/tcapi set WebCustom_Entry webs_notif_info1 "$hint1" &
	/userfs/bin/tcapi set WebCustom_Entry webs_notif_info2 "$hint2" &
fi
rm /tmp/dslasuswrt_notif.txt
/userfs/bin/tcapi set WebCustom_Entry webs_notif_update 1 &
