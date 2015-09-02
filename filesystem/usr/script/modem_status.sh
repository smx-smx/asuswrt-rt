#!/bin/sh
# $1: type.
# echo "This is a script to get the modem status."


modem_enable=`/userfs/bin/tcapi get USBModem_Entry modem_enable`
modem_type=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_type`
act_node1="usb_modem_act_int"
act_node2="usb_modem_act_bulk"
modem_vid=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_vid`
modem_pid=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_pid`
modem_dev=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_dev`
modem_roaming_scantime=`/userfs/bin/tcapi get USBModem_Entry modem_roaming_scantime`
modem_roaming_scanlist=`/userfs/bin/tcapi get USBModem_Entry modem_roaming_scanlist`

at_lock="flock -x /tmp/at_cmd_lock"

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
			i=$((i+1))
			continue
		fi

		if [ "$rp1" == "$rp2" ]; then
			echo "qcqmi$i"
			return
		fi

		i=$((i+1))
	done

	echo ""
}


act_node=
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

if [ "$1" == "sim" ]; then
	# check the SIM status.
	at_ret=`$at_lock modem_at.sh '+CPIN?' 2>/dev/null`
	sim_inserted1=`echo "$at_ret" |grep "READY" 2>/dev/null`
	sim_inserted2=`echo "$at_ret" |grep "SIM" |awk '{FS=": "; print $2}' 2>/dev/null`
	sim_inserted3=`echo "$at_ret" |grep "+CME ERROR: " |awk '{FS=": "; print $2}' 2>/dev/null`
	sim_inserted4=`echo "$sim_inserted2" |cut -c 1-3`
	if [ -n "$sim_inserted1" ]; then
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
			if [ "$modem_enable" == "2" -a "$sim_inserted3" == "SIM busy" ]; then
				echo "Detected CDMA2000's SIM"
				act_sim=1
			else
				echo "CME ERROR: $sim_inserted3"
				act_sim=-2
			fi
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
fi

