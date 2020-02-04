#!/bin/sh

/userfs/bin/tcapi set WebCustom_Entry webs_state_upgrade 0 &# INITIALIZING
/userfs/bin/tcapi set WebCustom_Entry webs_state_error 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "" &
/userfs/bin/tcapi set WebCustom_Entry webs_rsa_state_upgrade 0 &

#openssl support rsa check
rsa_enabled=`/userfs/bin/tcapi get SysInfo_Entry rc_support | grep HTTPS`
echo "---- rsa_enabled= $rsa_enabled  ----" >> /tmp/webs_upgrade.log

forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
url_path=`/userfs/bin/tcapi get WebCustom_Entry webs_state_url`
firmware_path=`/userfs/bin/tcapi get Misc_Entry firmware_path`
get_productid=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
odmpid_support=`/userfs/bin/tcapi get WebCustom_Entry webs_state_odm`
if [ "$odmpid_support" = "1" ]; then
	get_productid=`/userfs/bin/tcapi get SysInfo_Entry odmpid`
fi

if [ "$firmware_path" = "1" ]; then
	firmware_file=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info_beta`.zip
else
	firmware_file=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`.zip
fi

if [ "$rsa_enabled" != "" ]; then
	if [ "$firmware_path" = "1" ]; then
		firmware_rsasign=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info_beta`_rsa.zip
	else
		firmware_rsasign=`echo $get_productid`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`_rsa.zip
	fi
fi

touch /tmp/update_url
update_url=`cat /tmp/update_url`

#testing purpose, must be marked before checkin.
#update_url="http://192.168.123.198

rm -rf /var/tmp/tclinux.bin &
rm -rf /var/tmp/rsasign.bin &

# get firmware information
dl_fw=""
echo 3 > /proc/sys/vm/drop_caches
if [ "$update_url" != "" ]; then
	echo "---- update_url exist upgrade HTTPS----" >> /tmp/webs_upgrade.log
       	wget -q --no-check-certificate ${update_url}/$firmware_file -O /var/tmp/tclinux.bin
		dl_fw="$?"
	if [ "$rsa_enabled" != "" ]; then
		wget -q --no-check-certificate ${update_url}/$firmware_rsasign -O /var/tmp/rsasign.bin
	fi
elif [ "$forsq" = "1" ]; then
	echo "---- sq path upgrade HTTPS----" >> /tmp/webs_upgrade.log
       	wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$firmware_file -O /var/tmp/tclinux.bin
		dl_fw="$?"
	if [ "$rsa_enabled" != "" ]; then
		wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$firmware_rsasign -O /var/tmp/rsasign.bin
	fi
elif [ "$url_path" = "" ]; then
	echo "---- Official path upgrade HTTPS----" >> /tmp/webs_upgrade.log
	wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$firmware_file -O /var/tmp/tclinux.bin
	dl_fw="$?"
	if [ "$rsa_enabled" != "" ]; then
		wget -q --no-check-certificate https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$firmware_rsasign -O /var/tmp/rsasign.bin
	fi
else
	echo "---- External URL path upgrade HTTPS----" >> /tmp/webs_upgrade.log
	wget -q --no-check-certificate $url_path/$firmware_file -O /var/tmp/tclinux.bin
	dl_fw="$?"
	if [ "$rsa_enabled" != "" ]; then
		wget -q --no-check-certificate $url_path/$firmware_rsasign -O /var/tmp/rsasign.bin
	fi
fi

if [ "$dl_fw" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download firmware fail" &
	echo "---- download firmware fail, exit: $dl_fw ----" >> /tmp/webs_upgrade.log
elif [ "$rsa_enabled" != "" ] && [ "$?" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download rsa fail" &
	echo "---- download rsa fail, exit: $? ----" >> /tmp/webs_upgrade.log
else
	echo 3 > /proc/sys/vm/drop_caches
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download firmware successfully" &
	echo "---- download fw/rsa succesfully, exit: $? ----" >> /tmp/webs_upgrade.log
fi
/userfs/bin/tcapi set WebCustom_Entry webs_state_upgrade 1 &
/userfs/bin/tcapi set WebCustom_Entry webs_rsa_state_upgrade 1 &
