#!/bin/sh

wget_timeout=`/userfs/bin/tcapi get Apps_Entry apps_wget_timeout`
wget_options="-q -t 2 -T $wget_timeout --no-check-certificate"
fs_dir="/yaffs"
signature_dir="$fs_dir/signature"
echo "signature_dir = $signature_dir"

/userfs/bin/tcapi set Vram_Entry sig_state_upgrade 0 # INITIALIZING
/userfs/bin/tcapi set Vram_Entry sig_state_error 0

#openssl support rsa check
rsa_enabled=`/userfs/bin/tcapi get SysInfo_Entry rc_support| grep HTTPS`

touch /tmp/update_url
update_url=`cat /tmp/update_url`
#update_url="http://192.168.123.198"

sig_file=`/userfs/bin/tcapi get Vram_Entry SKU`_`/userfs/bin/tcapi get Vram_Entry sig_state_info`_un.zip
if [ "$rsa_enabled" != "" ]; then
sig_rsasign=`/userfs/bin/tcapi get Vram_Entry SKU`_`/userfs/bin/tcapi get Vram_Entry sig_state_info`_rsa.zip
fi
echo $sig_file >> /tmp/sig_upgrade.log
echo $sig_rsasign >> /tmp/sig_upgrade.log

# get signature zip file
forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
#urlpath=`/userfs/bin/tcapi get Apps_Entry sig_state_url`
if [ "$forsq" == "1" ]; then
	echo "---- wget trf sq ----"
	echo "---- wget trf sq ----" >> /tmp/sig_upgrade.log
	wget $wget_options https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$sig_file -O /var/tmp/rule.trf
	if [ "$rsa_enabled" != "" ]; then
		wget $wget_options https://dlcdnets.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$sig_rsasign -O /var/tmp/rsasign.bin
	fi
else
	echo "---- wget trf Real ----"
	echo "---- wget trf Real ----" >> /tmp/sig_upgrade.log
	wget $wget_options https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$sig_file -O /var/tmp/rule.trf
	if [ "$rsa_enabled" != "" ]; then
		wget $wget_options https://dlcdnets.asus.com/pub/ASUS/wireless/ASUSWRT/$sig_rsasign -O /var/tmp/rsasign.bin
	fi
fi

if [ "$?" != "0" ]; then	#download failure
	echo "---- Download and mv trf Failure ----"
	/userfs/bin/tcapi set Vram_Entry sig_state_error 1
else
	/userfs/bin/tcapi set Vram_Entry sig_state_upgrade 2	
	echo "---- Download and mv trf OK ----"
	echo "---- Download and mv trf OK ----" >> /tmp/sig_upgrade.log
	if [ "$rsa_enabled" != "" ]; then
		/userfs/bin/tcapi set Vram_Entry bwdpi_rsa_check 0
		rsasign_sig_check /var/tmp/rule.trf
		sleep 1
	fi

	if [ "$rsa_enabled" != "" ]; then
		rsasign_check_ret=`/userfs/bin/tcapi get Vram_Entry bwdpi_rsa_check`
	fi

	if [ "$rsasign_check_ret" == "1" ]; then
		echo "---- sig check OK ----" >> /var/tmp/sig_upgrade.log
		if [ -f $signature_dir/rule.trf ];then
			echo "---- sig rule mv /var/tmp to /jffs/signature ----" >> /var/tmp/sig_upgrade.log
			rm $signature_dir/rule.trf
			mv /var/tmp/rule.trf $signature_dir/rule.trf
		else
			echo "---- sig rule mv jffs ----" >> /var/tmp/sig_upgrade.log
			mkdir $signature_dir
			mv /var/tmp/rule.trf $signature_dir/rule.trf
		fi
		if [ "$1" == "" ];then
			#force stop & start
			echo "Do restart_wrs"
			echo "Do restart_wrs" >> /tmp/sig_upgrade.log
			bwdpi service stop	
			bwdpi service start
			/userfs/bin/tcapi set Vram_Entry sig_update_t `date +%s`	#set timestamp for download signature and restart_wrs
		else
			echo "do nothing..."
			echo "do nothing..." >> /tmp/sig_upgrade.log	
		fi
	else
		echo "---- sig check error ----" >> /var/tmp/sig_upgrade.log
		/userfs/bin/tcapi set Vram_Entry sig_state_error 3	# wrong sig trf
	fi
fi

rm -f /var/tmp/rsasign.bin

/userfs/bin/tcapi set Vram_Entry sig_state_upgrade 1
