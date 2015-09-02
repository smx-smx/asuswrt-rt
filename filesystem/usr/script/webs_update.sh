#!/bin/sh

forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
echo "forsq = ${forsq}" > /tmp/webs_upgrade.log #1 for sq
model=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
echo "model = ${model}" >> /tmp/webs_upgrade.log #DSL-N66U
if [ "$model" == "DSL-N12E-C1" ] || [ "$model" == "DSL-N10-C1" ]; then
        model_http="1"
else
        model_http="0"
fi

touch /tmp/update_url
update_url=`cat /tmp/update_url`

#must be marked before checkin.
#update_url="http://192.168.123.198"

/userfs/bin/tcapi set WebCustom_Entry webs_state_update 0 &# INITIALIZING
/userfs/bin/tcapi set WebCustom_Entry webs_state_error 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_url "" &
/userfs/bin/tcapi set WebCustom_Entry webs_state_reboot 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_info "" &
/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "" &

if [ "$update_url" != "" ]; then
	if [ "$model_http" = "1" ]; then
		echo "---- update_url exist HTTP ----" >> /tmp/webs_upgrade.log
		wget -q ${update_url}/wlan_update_v2.zip -O /tmp/wlan_update.txt
	else
		echo "---- update_url exist HTTPS ----" >> /tmp/webs_upgrade.log
	        wget -q --no-check-certificate ${update_url}/wlan_update_v2.zip -O /tmp/wlan_update.txt
	fi
elif [ "$forsq" = "1" ]; then
	if [ "$model_http" = "1" ]; then
		echo "---- sq pqth HTTP----" >> /tmp/webs_upgrade.log
                wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/wlan_update_v2.zip -O /tmp/wlan_update.txt
	else
		echo "---- sq pqth HTTPS----" >> /tmp/webs_upgrade.log
        	wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/wlan_update_v2.zip -O /tmp/wlan_update.txt
	fi
else
	if [ "$model_http" = "1" ]; then
		echo "---- Official path HTTP----" >> /tmp/webs_upgrade.log
		wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless/wlan_update_v2.zip -O /tmp/wlan_update.txt
	else
		echo "---- Official path HTTPS----" >> /tmp/webs_upgrade.log
		wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless/wlan_update_v2.zip -O /tmp/wlan_update.txt
	fi
fi

if [ "$?" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download wlan_update.zip fail" &
else
	buildno=`grep $model /tmp/wlan_update.txt | sed 's/.*#FW//;' | sed 's/#.*//;'`
	url_path=`grep $model /tmp/wlan_update.txt | sed 's/.*#URL//;' | sed 's/#.*//;'`
	need_reboot=`grep $model /tmp/wlan_update.txt | grep '#RBT' /tmp/wlan_update.txt`
	if [ "$buildno" = "" ]; then
		/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
		/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "parse wlan_update.txt fail" &
	else
		/userfs/bin/tcapi set WebCustom_Entry webs_state_info $buildno &
		if [ "$url_path" != "" ]; then
			/userfs/bin/tcapi set WebCustom_Entry webs_state_url $url_path &
		else
			/userfs/bin/tcapi set WebCustom_Entry webs_state_url "" &
		fi
		if [ ${#need_reboot} > 0 ]; then
			/userfs/bin/tcapi set WebCustom_Entry webs_state_reboot ${#need_reboot} &
		fi
	fi
fi
rm /tmp/wlan_update.txt
/userfs/bin/tcapi set WebCustom_Entry webs_state_update 1 &
