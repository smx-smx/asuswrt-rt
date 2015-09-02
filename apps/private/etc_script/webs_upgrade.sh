#!/bin/sh

/userfs/bin/tcapi set WebCustom_Entry webs_state_upgrade 0 &# INITIALIZING
/userfs/bin/tcapi set WebCustom_Entry webs_state_error 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "" &
/userfs/bin/tcapi set WebCustom_Entry webs_rsa_state_upgrade 0 &

#openssl support rsa check
rsa_enabled=`/userfs/bin/tcapi get SysInfo_Entry rc_support | grep HTTPS`
echo "---- rsa_enabled= $rsa_enabled  ----" > /tmp/webs_upgrade.log

forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
url_path=`/userfs/bin/tcapi get WebCustom_Entry webs_state_url`
model=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
if [ "$model" == "DSL-N12E-C1" ] || [ "$model" == "DSL-N10-C1" ]; then
        model_http="1"
else
        model_http="0"
fi
firmware_file=`/userfs/bin/tcapi get SysInfo_Entry ProductName`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`.zip

if [ "$rsa_enabled" != "" ]; then
firmware_rsasign=`/userfs/bin/tcapi get SysInfo_Entry ProductName`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`_rsa.zip
fi

touch /tmp/update_url
update_url=`cat /tmp/update_url`

#testing purpose, must be marked before checkin.
#update_url="http://192.168.123.198

# get firmware information
echo 3 > /proc/sys/vm/drop_caches
if [ "$update_url" != "" ]; then
	if [ "$model_http" = "1" ]; then
		echo "---- update_url exist upgrade HTTP----" > /tmp/webs_upgrade.log
                wget -q ${update_url}/$firmware_file -O /var/tmp/tclinux.bin
                if [ "$rsa_enabled" != "" ]; then
                        wget -q ${update_url}/$firmware_rsasign -O /var/tmp/rsasign.bin
                fi
	else
		echo "---- update_url exist upgrade HTTPS----" > /tmp/webs_upgrade.log
        	wget -q --no-check-certificate ${update_url}/$firmware_file -O /var/tmp/tclinux.bin
		if [ "$rsa_enabled" != "" ]; then
			wget -q --no-check-certificate ${update_url}/$firmware_rsasign -O /var/tmp/rsasign.bin
		fi
	fi
elif [ "$forsq" = "1" ]; then
	if [ "$model_http" = "1" ]; then
		echo "---- sq path upgrade HTTP----" > /tmp/webs_upgrade.log
		wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$firmware_file -O /var/tmp/tclinux.bin
		if [ "$rsa_enabled" != "" ]; then
			wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$firmware_rsasign -O /var/tmp/rsasign.bin
		fi
	else
		echo "---- sq path upgrade HTTPS----" > /tmp/webs_upgrade.log
        	wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$firmware_file -O /var/tmp/tclinux.bin
		if [ "$rsa_enabled" != "" ]; then
			wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$firmware_rsasign -O /var/tmp/rsasign.bin
		fi
	fi
elif [ "$url_path" = "" ]; then
	if [ "$model_http" = "1" ]; then
		echo "---- Official path upgrade HTTP----" > /tmp/webs_upgrade.log
		wget -q http://dlcdnet.asus.com/pub/ASUS/wireless/ASUSWRT/$firmware_file -O /var/tmp/tclinux.bin
		if [ "$rsa_enabled" != "" ]; then
			wget -q http://dlcdnet.asus.com/pub/ASUS/wireless/ASUSWRT/$firmware_rsasign -O /var/tmp/rsasign.bin
		fi
	else
		echo "---- Official path upgrade HTTPS----" > /tmp/webs_upgrade.log
		wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$firmware_file -O /var/tmp/tclinux.bin
		if [ "$rsa_enabled" != "" ]; then
			wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$firmware_rsasign -O /var/tmp/rsasign.bin
		fi
	fi
else
	if [ "$model_http" = "1" ]; then
		echo "---- External URL path upgrade HTTP----" > /tmp/webs_upgrade.log
		wget -q $url_path/$firmware_file -O /var/tmp/tclinux.bin
		if [ "$rsa_enabled" != "" ]; then
			wget -q $url_path/$firmware_rsasign -O /var/tmp/rsasign.bin
		fi
	else
		echo "---- External URL path upgrade HTTPS----" > /tmp/webs_upgrade.log
		wget -q --no-check-certificate $url_path/$firmware_file -O /var/tmp/tclinux.bin
		if [ "$rsa_enabled" != "" ]; then
			wget -q --no-check-certificate $url_path/$firmware_rsasign -O /var/tmp/rsasign.bin
		fi
	fi
fi

if [ "$?" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download firmware fail" &
else
	echo 3 > /proc/sys/vm/drop_caches
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download firmware successfully" &
fi
/userfs/bin/tcapi set WebCustom_Entry webs_state_upgrade 1 &
/userfs/bin/tcapi set WebCustom_Entry webs_rsa_state_upgrade 1 &
