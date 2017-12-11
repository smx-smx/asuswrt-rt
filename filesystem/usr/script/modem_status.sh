#!/bin/sh
# $1: type.
# echo "This is a script to get the modem status."


act_node1="usb_modem_act_int"
act_node2="usb_modem_act_bulk"
modem_vid=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_vid`
modem_pid=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_pid`
modem_dev=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_dev`
sim_order=`/userfs/bin/tcapi get USBModem_Entry modem_sim_order`

usb_gobi2=`/userfs/bin/tcapi get USBModem_Entry usb_gobi2`

stop_lock=`/userfs/bin/tcapi get USBModem_Entry stop_lock`
if [ -n "$stop_lock" ] && [ "$stop_lock" -eq "1" ]; then
	at_lock=""
else
	at_lock="flock -x /tmp/at_cmd_lock"
fi

jffs_dir="/jffs"


# $1: ifname.
_get_qcqmi_by_usbnet(){
	rp1=`readlink -f /sys/class/net/$1/device 2>/dev/null`
	if [ "$rp1" == "" ]; then
		echo ""
		return
	fi

	rp2=
	i=0
	while [ $i -lt 10 ]; do
		rp2=`readlink -f /sys/class/GobiQMI/qcqmi$i/device 2>/dev/null`
		if [ "$rp2" == "" ]; then
			i=$(($i+1))
			continue
		fi

		if [ "$rp1" == "$rp2" ]; then
			echo "qcqmi$i"
			return
		fi

		i=$(($i+1))
	done

	echo ""
}

# $1: VID, $2: PID.
_get_gobi_device(){
	if [ -z "$1" ] || [ -z "$2" ]; then
		echo "0"
		return
	fi

	if [ "$1" == "1478" ] && [ "$2" == "36902" -o "$2" == "36903" ]; then
		echo "1"
		return
	fi

	echo "0"
}


act_node=
#modem_type=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_type`
#if [ "$modem_type" == "tty" -o "$modem_type" == "mbim" ]; then
#	if [ "$modem_type" == "tty" -a "$modem_vid" == "6610" ]; then # e.q. ZTE MF637U
#		act_node=$act_node1
#	else
#		act_node=$act_node2
#	fi
#else
	act_node=$act_node1
#fi

modem_act_node=`/userfs/bin/tcapi get USBModem_Entry $act_node`
if [ "$modem_act_node" == "" ]; then
	find_modem_node.sh

	modem_act_node=`/userfs/bin/tcapi get USBModem_Entry $act_node`
	if [ "$modem_act_node" == "" ]; then
		echo "Can't get $act_node!"
		exit 1
	fi
fi

is_gobi=`_get_gobi_device $modem_vid $modem_pid`

if [ "$1" == "sim" ]; then
	stop_sim=`/userfs/bin/tcapi get USBModem_Entry stop_sim`
	if [ -n "$stop_sim" ] && [ "$stop_sim" -eq "1" ]; then
		echo "Skip to detect SIM..."
		exit 0
	fi

	modem_enable=`/userfs/bin/tcapi get USBModem_Entry modem_enable`
	simdetect=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_simdetect`
	if [ -z "$simdetect" ]; then
		modem_status.sh simdetect
	fi

	# check the SIM status.
	at_ret=`$at_lock modem_at.sh '+CPIN?' 2>/dev/null`
	sim_inserted1=`echo "$at_ret" |grep "READY" 2>/dev/null`
	sim_inserted2=`echo "$at_ret" |grep "SIM" |awk '{FS=": "; print $2}' 2>/dev/null`
	sim_inserted3=`echo "$at_ret" |grep "+CME ERROR: " |awk '{FS=": "; print $2}' 2>/dev/null`
	sim_inserted4=`echo "$sim_inserted2" |cut -c 1-3`
	if [ "$modem_enable" == "2" ]; then
		echo "Detected CDMA2000's SIM"
		act_sim=1
	elif [ -n "$sim_inserted1" ]; then
		echo "Got SIM."
		act_sim=1
	elif [ "$sim_inserted2" == "SIM PIN" ]; then
		echo "Need PIN."
		act_sim=2
	elif [ "$sim_inserted2" == "SIM PUK" ]; then
		echo "Need PUK."
		act_sim=3
	elif [ "$sim_inserted2" == "SIM PIN2" ]; then
		echo "Need PIN2."
		act_sim=4
	elif [ "$sim_inserted2" == "SIM PUK2" ]; then
		echo "Need PUK2."
		act_sim=5
	elif [ "$sim_inserted4" == "PH-" ]; then
		echo "Waiting..."
		act_sim=6
	elif [ "$sim_inserted3" != "" ]; then
		if [ "$sim_inserted3" == "SIM not inserted" ]; then
			echo "SIM not inserted."
			act_sim=-1
		else
			echo "CME ERROR: $sim_inserted3"
			act_sim=-2
		fi
	else
		echo "No or unknown response."
		act_sim=-10
	fi

	act_sim_orig=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_sim`
	if [ "$act_sim_orig" != "$act_sim" ]; then
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_sim $act_sim &
	fi

	echo "done."
elif [ "$1" == "setmode" ]; then
	if [ "$is_gobi" -eq "1" ]; then
		mode=
		if [ "$2" == "0" ]; then	# Auto
			mode=10
		elif [ "$2" == "43" ]; then	# 4G/3G
			mode=17
		elif [ "$2" == "4" ]; then	# 4G only
			mode=11
		elif [ "$2" == "3" ]; then	# 3G only
			mode=2
		elif [ "$2" == "2" ]; then	# 2G only
			mode=1
		else
			echo "Can't identify the mode type: $2."
			exit 7
		fi

		at_ret=`$at_lock modem_at.sh '+CSETPREFNET='$mode 2>/dev/null`
		ret=`echo "$at_ret" |grep '+CSETPREFNET=' |awk '{FS="="; print $2}' 2>/dev/null`
		if [ "$ret" == "" ]; then
			echo "Fail to set the modem mode from $modem_act_node."
			exit 8
		fi

		echo "Set the mode be $2($ret)."
		echo "done."
	fi
elif [ "$1" == "imsi" ]; then
	echo "Getting IMSI..."
	at_ret=`$at_lock modem_at.sh '+CIMI' 2>/dev/null`
	ret=`echo "$at_ret" |grep "^[0-9].*$" 2>/dev/null`
	if [ "$ret" == "" ]; then
		echo "Fail to get the IMSI from $modem_act_node."
		exit 11
	fi

	/userfs/bin/tcapi set USBModem_Entry usb_modem_act_imsi $ret &

	sim_num=`/userfs/bin/tcapi get USBModem_Entry modem_sim_num`
	if [ -z "$sim_num" ]; then
		sim_num=10
	fi

	/userfs/bin/tcapi set USBModem_Entry modem_sim_order -1 &
	i=1
	while [ $i -le $sim_num ]; do
		echo -n "check SIM($i)..."
		got_imsi=`/userfs/bin/tcapi get USBModem_Entry modem_sim_imsi$i`

		if [ "$got_imsi" == "" ]; then
			echo "Set SIM($i)."
			/userfs/bin/tcapi set USBModem_Entry modem_sim_order $i &
			/userfs/bin/tcapi set USBModem_Entry modem_sim_imsi${i} $ret &
			break
		elif [ "$got_imsi" == "$ret" ]; then
			echo "Get SIM($i)."
			/userfs/bin/tcapi set USBModem_Entry modem_sim_order $i &
			break
		fi

		i=$((i+1))
	done

	echo "done."
elif [ "$1" == "iccid" ]; then
	if [ "$is_gobi" -eq "1" ]; then
		echo -n "Getting ICCID..."
		at_ret=`$at_lock modem_at.sh '+ICCID' 2>/dev/null`
		ret=`echo "$at_ret" |grep "ICCID: " |awk '{FS="ICCID: "; print $2}' 2>/dev/null`
		if [ "$ret" == "" ]; then
			echo "Fail to get the ICCID from $modem_act_node."
			exit 13
		fi

		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_iccid $ret &

		echo "done."
	fi
elif [ "$1" == "rate" ]; then
	if [ "$is_gobi" -eq "1" ]; then
		echo -n "Getting Rate..."
		qcqmi=`_get_qcqmi_by_usbnet $modem_dev 2>/dev/null`
		at_ret=`gobi_api $qcqmi rate |grep "Max Tx" 2>/dev/null`
		max_tx=`echo "$at_ret" |awk '{FS=","; print $1}' |awk '{FS=" "; print $3}' 2>/dev/null`
		max_rx=`echo "$at_ret" |awk '{FS=","; print $2}' |awk '{FS=" "; print $2}' |awk '{FS="."; print $1}' 2>/dev/null`
		if [ "$max_tx" == "" -o "$max_rx" == "" ]; then
			echo "Fail to get the rate from $modem_act_node."
			exit 14
		fi

		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_tx $max_tx &
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_rx $max_rx &

		echo "done."
	fi
elif [ "$1" == "band" ]; then
	if [ "$is_gobi" -eq "1" ]; then
		echo -n "Getting Band..."
		at_ret=`$at_lock modem_at.sh '$CRFI' 2>/dev/null`
		ret=`echo "$at_ret" |grep '$CRFI:' |awk '{FS=":"; print $2}' 2>/dev/null`
		if [ "$ret" == "" ]; then
			echo "Fail to get the current band from $modem_act_node."
			exit 16
		fi

		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_band "$ret" &

		echo "done."
	fi
elif [ "$1" == "station" ]; then
	modem_reg_time=`/userfs/bin/tcapi get USBModem_Entry modem_reg_time`
	$at_lock modem_at.sh "+COPS=1,0,\"$2\"" "$modem_reg_time" 1,2>/dev/null
	if [ $? -ne 0 ]; then
		echo "19:Fail to set the station: $2."
		exit 19
	fi

	echo "done."
elif [ "$1" == "simauth" ]; then
	if [ "$is_gobi" -eq "1" ]; then
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_auth "" &
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_auth_pin "" &
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_auth_puk "" &
		at_ret=`$at_lock modem_at.sh '+CPINR' |grep "+CPINR:" |awk '{FS=":"; print $2}' 2>/dev/null`
		if [ "$at_ret" == "" ]; then
			echo "Fail to get the SIM status."
			exit 20
		fi

		ret=`echo "$at_ret" |awk '{FS=","; print $3}' 2>/dev/null`
		if [ "$ret" == "" ]; then
			echo "Fail to get the SIM auth state."
			exit 21
		fi
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_auth $ret &
		if [ "$ret" == "1" ]; then
			echo "SIM auth state is ENABLED_NOT_VERIFIED."
		elif [ "$ret" == "2" ]; then
			echo "SIM auth state is ENABLED_VERIFIED."
		elif [ "$ret" == "3" ]; then
			echo "SIM auth state is DISABLED."
		elif [ "$ret" == "4" ]; then
			echo "SIM auth state is BLOCKED."
		elif [ "$ret" == "5" ]; then
			echo "SIM auth state is PERMANENTLY_BLOCKED."
		else
			echo "SIM auth state is UNKNOWN."
		fi

		ret=`echo "$at_ret" |awk '{FS=","; print $4}' 2>/dev/null`
		if [ "$ret" == "" ]; then
			echo "Fail to get the PIN retry."
			exit 22
		fi
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_auth_pin $ret &
		echo "SIM PIN retry is $ret."

		ret=`echo "$at_ret" |awk '{FS=","; print $5}' 2>/dev/null`
		if [ "$ret" == "" ]; then
			echo "Fail to get the PUK retry."
			exit 23
		fi
		/userfs/bin/tcapi set USBModem_Entry usb_modem_act_auth_puk $ret &

		echo "SIM PUK retry is $ret."
		echo "done."
	fi
elif [ "$1" == "simpin" ]; then
	if [ "$2" == "" ]; then
		/userfs/bin/tcapi set USBModem_Entry g3state_pin 2 &

		echo "24:Need to input the PIN code."
		exit 24
	fi

	/userfs/bin/tcapi set USBModem_Entry g3state_pin 1 &
	at_ret=`$at_lock modem_at.sh '+CPIN='\"$2\" |grep "OK" 2>/dev/null`
	if [ "$at_ret" == "" ]; then
		/userfs/bin/tcapi set USBModem_Entry g3err_pin 1 &

		echo "25:Fail to unlock the SIM: $2."
		exit 25
	fi

	/userfs/bin/tcapi set USBModem_Entry g3err_pin 0 &
	echo "done."
elif [ "$1" == "simpuk" ]; then
	# $2: the original PUK. $3: the new PIN.
	if [ "$2" == "" ]; then
		echo "26:Need to input the PUK code."
		exit 26
	elif [ "$3" == "" ]; then
		echo "27:Need to input the new PIN code."
		exit 27
	fi

	at_ret=`$at_lock modem_at.sh '+CPIN='\"$2\"','\"$3\" |grep "OK" 2>/dev/null`
	if [ "$at_ret" == "" ]; then
		echo "28:Fail to unlock the SIM PIN: $2."
		exit 28
	fi

	echo "done."
elif [ "$1" == "simdetect" ]; then
	if [ "$is_gobi" -eq "1" ] && [ "$usb_gobi2" != "1" ]; then
		# $2: 0: disable, 1: enable.
		at_ret=`$at_lock modem_at.sh '$NV70210' 2>/dev/null`
		ret=`echo -n $at_ret |grep "OK" 2>/dev/null`
		if [ -z "$ret" ]; then
			echo "44:Fail to get the value of SIM detect."
			exit 44
		fi

		current=`echo -n $at_ret |awk '{print $2}'`

		if [ -z "$2" ]; then
			echo "$current"
			/userfs/bin/tcapi set USBModem_Entry usb_modem_act_simdetect $current &
		elif [ "$2" == "1" -a "$current" == "0" ] || [ "$2" == "0" -a "$current" == "1" ]; then
			at_ret=`$at_lock modem_at.sh '$NV70210='$2 |grep "OK" 2>/dev/null`
			if [ -z "$at_ret" ]; then
				echo "45:Fail to set the SIM detect to be $2."
				exit 45
			fi
			/userfs/bin/tcapi set USBModem_Entry usb_modem_act_simdetect $2 &

			# Use reboot to replace this.
			#at_ret=`$at_lock modem_at.sh '+CFUN=1,1' |grep "OK" 2>/dev/null`
			#if [ -z "$at_ret" ]; then
			#	echo "45:Fail to reset the Gobi."
			#	exit 46
			#fi
		fi

		echo "done."
	fi
fi

