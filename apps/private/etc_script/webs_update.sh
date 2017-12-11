#!/bin/sh

forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
echo "forsq = ${forsq}" >> /tmp/webs_upgrade.log #1 for sq
model=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
model="$model#"
echo "model = ${model}" >> /tmp/webs_upgrade.log #DSL-N66U#

touch /tmp/update_url
update_url=`cat /tmp/update_url`

#current firmware information
current_firm=`/userfs/bin/tcapi get DeviceInfo FwVer`	#1.1.2.2_7-gabcde12
current_firm=`echo $current_firm | sed s/'\.'//g;`	#1122_7-gabcde12
current_firm_1st_bit=`echo $current_firm | cut -c1-1`		#current_firm_1st_bit=${current_firm:0:1}
echo "current_firm_1st_bit = ${current_firm_1st_bit}" >> /tmp/webs_upgrade.log
current_buildno=`echo $current_firm | sed s/_.*//;`
current_extendno=`echo $current_firm | sed s/.....//;`
current_extendno=`echo $current_extendno | sed s/-g.*//;`

#must be marked before checkin.
#update_url="https://192.168.123.198"

/userfs/bin/tcapi set WebCustom_Entry webs_state_update 0 &		# INITIALIZING
/userfs/bin/tcapi set WebCustom_Entry webs_state_error 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_flag 0 &			# 0: Don't do upgrade  1: Do upgrade	
/userfs/bin/tcapi set WebCustom_Entry webs_state_url "" &
/userfs/bin/tcapi set WebCustom_Entry webs_state_reboot 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_info "" &
/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "" &

if [ "$update_url" != "" ]; then
	echo "---- update_url exist HTTPS ----" >> /tmp/webs_upgrade.log
	wget -q --no-check-certificate ${update_url}/wlan_update_dsl.zip -O /tmp/wlan_update.txt
elif [ "$forsq" = "1" ]; then
	echo "---- sq pqth HTTPS----" >> /tmp/webs_upgrade.log
	wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/wlan_update_dsl.zip -O /tmp/wlan_update.txt
else
	echo "---- Official path HTTPS----" >> /tmp/webs_upgrade.log
	wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless/wlan_update_dsl.zip -O /tmp/wlan_update.txt
fi

if [ "$?" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download wlan_update.zip fail" &
else
	##FW1106_86-g09787df
	buildno=`grep $model /tmp/wlan_update.txt | sed 's/.*#FW//;' | sed 's/#.*//;'`
	extendno=`grep $model /tmp/wlan_update.txt | sed 's/.*#EXT//;' | sed 's/#.*//;'`
	lextendno=`echo $extendno | sed s/-g.*//;`
	buildno_beta=`grep $model /tmp/wlan_update.txt | sed 's/.*#BETAFW//;' | sed 's/#.*//;'`
	extendno_beta=`grep $model /tmp/wlan_update.txt | sed 's/.*#BETAEXT//;' | sed 's/#.*//;'`
	lextendno_beta=`echo $extendno_beta | sed s/-g.*//;`
	url_path=`grep $model /tmp/wlan_update.txt | sed 's/.*#URL//;' | sed 's/#.*//;'`
	need_reset=`grep $model /tmp/wlan_update.txt | grep '#RBT' /tmp/wlan_update.txt`
	if [ "$buildno" = "" ]; then
		/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
		/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "parse wlan_update.txt fail" &
	else
		/userfs/bin/tcapi set WebCustom_Entry webs_state_info ${buildno}_${extendno} &
		/userfs/bin/tcapi set WebCustom_Entry webs_state_info_beta ${buildno_beta}_${extendno_beta} &
		if [ "$url_path" != "" ]; then
			/userfs/bin/tcapi set WebCustom_Entry webs_state_url $url_path &
		else
			/userfs/bin/tcapi set WebCustom_Entry webs_state_url "" &
		fi
		if [ ${#need_reset} -gt 0 ]; then
			/userfs/bin/tcapi set WebCustom_Entry webs_state_reboot ${#need_reset} &
		fi
	fi
fi
rm /tmp/wlan_update.txt


#set webs_state_flag=1
echo "---- current fw info : $current_buildno $current_extendno ----" >> /tmp/webs_upgrade.log
echo "---- formal fw info : $buildno $lextendno ----" >> /tmp/webs_upgrade.log
update_webs_state_info=`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`
last_webs_state_info=`/userfs/bin/tcapi get WebCustom_Entry webs_last_info`
if [ "$buildno" == "" ] || [ "$lextendno" == "" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &	# exist no Info
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "parsing info fail" &
else
	if [ "$current_buildno" -lt "$buildno" ]; then
		echo "---- buildno: $buildno ----" >> /tmp/webs_upgrade.log
		/userfs/bin/tcapi set WebCustom_Entry webs_state_flag 1 &	# Do upgrade
		if [ "$last_webs_state_info" != "$update_webs_state_info" ]; then
			if [ "$current_firm_1st_bit" != 9 ]; then
				#Notify_Event2NC "$SYS_FW_NWE_VERSION_AVAILABLE_EVENT" "New FW Available now."	#Send Event to Notification Center
				/userfs/bin/tcapi set WebCustom_Entry webs_last_info ${update_webs_state_info} &
			fi	
		fi
	elif [ "$current_buildno" -eq "$buildno" ]; then
		if [ "$current_extendno" -lt "$lextendno" ]; then
			echo "---- lextendno: $lextendno ----" >> /tmp/webs_upgrade.log
			/userfs/bin/tcapi set WebCustom_Entry webs_state_flag 1	&	# Do upgrade
			if [ "$last_webs_state_info" != "$update_webs_state_info" ]; then
				if [ "$current_firm_1st_bit" != 9 ]; then
					#Notify_Event2NC "$SYS_FW_NWE_VERSION_AVAILABLE_EVENT" "New FW Available now."	#Send Event to Notification Center
					/userfs/bin/tcapi set WebCustom_Entry webs_last_info ${update_webs_state_info} &
				fi	
			fi
		fi
	fi	
fi


# download releasee note
get_productid=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
get_productid=`echo $get_productid | sed s/+/plus/;`	#replace 'plus' to '+' for one time

#releasenote_file0=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`_"$LANG"_note.zip
releasenote_file0_US=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`_US_note.zip
releasenote_path0="/tmp/release_note0.txt"
#releasenote_file1=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info_beta`_"$LANG"_note.zip
releasenote_file1_US=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info_beta`_US_note.zip
releasenote_path1="/tmp/release_note1.txt"

if [ "$forsq" == "1" ]; then
	echo "---- download SQ release note ----" >> /tmp/webs_upgrade.log
	#wget $wget_options https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$releasenote_file0 -O $releasenote_path0
		#if [ "$?" != "0" ]; then
			wget $wget_options https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$releasenote_file0_US -O $releasenote_path0
			echo "https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$releasenote_file0_US" >> /tmp/webs_upgrade.log
		#fi	
	#wget $wget_options https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$releasenote_file1 -O $releasenote_path1
	#if [ "$?" != "0" ]; then
		wget $wget_options https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$releasenote_file1_US -O $releasenote_path1
		echo "https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$releasenote_file1_US" >> /tmp/webs_upgrade.log
	#fi	
else
	echo "---- download real release note ----" >> /tmp/webs_upgrade.log
	#wget $wget_options https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$releasenote_file0 -O $releasenote_path0
		#if [ "$?" != "0" ]; then
			wget $wget_options https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$releasenote_file0_US -O $releasenote_path0
			echo "---- https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$releasenote_file0_US ----" >> /tmp/webs_upgrade.log
		#fi		
	#wget $wget_options https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$releasenote_file1 -O $releasenote_path1
		#if [ "$?" != "0" ]; then
			wget $wget_options https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$releasenote_file1_US -O $releasenote_path1
			echo "---- https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$releasenote_file1_US ----" >> /tmp/webs_upgrade.log
		#fi		
fi
	
if [ "$?" != "0" ]; then
	echo "---- download SQ release note failed ----" >> /tmp/webs_upgrade.log
	/userfs/bin/tcapi set webs_state_error 1 &
fi


/userfs/bin/tcapi set WebCustom_Entry webs_state_update 1 &
