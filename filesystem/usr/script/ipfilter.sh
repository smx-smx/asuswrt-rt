#!/bin/sh

if [ $# != 3 ] ; then
	echo "usage: ipfilter [add/del] [set index(1~12)] [rule index(1~6)]"
	exit 0
fi

CONFFILE0="/etc/ipfilter/ipfilter_$2""_0.conf"
CONFFILE1="/etc/ipfilter/ipfilter_$2""_$3.conf"

if [ -f $CONFFILE0 ] ; then
	. $CONFFILE0
else
	exit 0
fi

if [ -f $CONFFILE1 ] ; then
	. $CONFFILE1
else
	exit 0
fi

if [ $1 = "add" ] ; then
	ACTION=-A
else
	exit 0
fi

if [  "$ACTIVE" = "Yes" ] ; then
	PERMIT=DROP
elif [  "$ACTIVE" = "No" ] ; then
	exit 0
fi
	DIR_CONFFILE0=$DIRECTION 

if [ "$INTERFACE" = "LAN" ] ; then
	IF="br0"
else 
	if [ "$INTERFACE" = "PVC0" ] ; then
		PVC=0
	elif [ "$INTERFACE" = "PVC1" ] ; then
		PVC=1
	elif [ "$INTERFACE" = "PVC2" ] ; then
		PVC=2
	elif [ "$INTERFACE" = "PVC3" ] ; then
		PVC=3
	elif [ "$INTERFACE" = "PVC4" ] ; then
		PVC=4
	elif [ "$INTERFACE" = "PVC5" ] ; then
		PVC=5
	elif [ "$INTERFACE" = "PVC6" ] ; then
		PVC=6
	elif [ "$INTERFACE" = "PVC7" ] ; then
		PVC=7
	else
		echo "error: INTERFACE name $INTERFACE wrong."
        	exit 0
	fi

 	IF=nas$PVC
	CONFFILE3=/etc/isp$PVC.conf
	if [ -f $CONFFILE3 ] ; then
	        chmod +x $CONFFILE3
        	. $CONFFILE3
	else
		exit 0
	fi
	# PPPoE/PPPoA
	if [ "$ISP" = "2" ] ; then
        	IF=ppp$PVC
	fi
fi

if [ "$RULE_TYPE" = "IP" ] ; then

	if [ "$SRCPORT" = "0" ] || [ "$SRCPORT" = "" ] ; then
		SPORT=
	else
		SPORT="--source-port $SRCPORT"
	fi

	if [ "$DESPORT" = "0" ] || [ "$DESPORT" = "" ] ; then
		DPORT=
	else
		DPORT="--destination-port $DESPORT"
	fi

	if [ "$PROTOCOL" = "ICMP" ] ; then
		SPORT=
		DPORT=
	fi

	if [ "$SRCIP" = "0.0.0.0" ] || [ "$SRCIP" = "" ] ; then
		FLT_SIP=
	else
		if [ "$SRCMASK" = "" ] ; then
			FLT_SIP="-s $SRCIP"
		else	
			FLT_SIP="-s $SRCIP""/$SRCMASK"
		fi
	fi

	if [ "$DESIP" = "0.0.0.0" ] || [ "$DESIP" = "" ] ; then
		FLT_DIP=
	else
		if [ "$DESMASK" = "" ] ; then
			FLT_DIP="-d $DESIP"
		else	
			FLT_DIP="-d $DESIP""/$DESMASK"
		fi
	fi
	if [ "$ISP" = "3" ] ; then
		if [ "$DIR_CONFFILE0" = "Incoming" ] ; then
		# wtw add ifeb ebtables $ACTION IP_FLT -i $IF -p IPv4 -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
		echo "Current version does not support ebtables" #wtw add
		elif [ "$DIR_CONFFILE0" = "Outgoing" ] ; then
		# wtw add ifeb 	ebtables $ACTION IP_FLT -o $IF -p IPv4 -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
		echo "Current version does not support ebtables" #wtw add
		elif [ "$DIR_CONFFILE0" = "Both" ] ; then
		# wtw add ifeb 	ebtables $ACTION IP_FLT -i $IF -p IPv4 -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
		# wtw add ifeb 	ebtables $ACTION IP_FLT -o $IF -p IPv4 -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
		echo "Current version does not support ebtables" #wtw add
		fi
	else
		if [ "$DIR_CONFFILE0" = "Incoming" ] ; then
			iptables $ACTION IP_FLT -i $IF -p $PROTOCOL -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
		elif [ "$DIR_CONFFILE0" = "Outgoing" ] ; then
			iptables $ACTION IP_FLT -o $IF -p $PROTOCOL -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
		elif [ "$DIR_CONFFILE0" = "Both" ] ; then
			iptables $ACTION IP_FLT -i $IF -p $PROTOCOL -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
			iptables $ACTION IP_FLT -o $IF -p $PROTOCOL -j $PERMIT $FLT_SIP $SPORT $FLT_DIP $DPORT
		fi
	fi
else
	if [ "$ISP" = "3" ] ; then
		if [ "$DIR_CONFFILE0" = "Incoming" ] ; then
		# wtw add ifeb ebtables $ACTION MAC_FLT -i $IF -j $PERMIT -s $MAC
		echo "Current version does not support ebtables" #wtw add
		elif [ "$DIR_CONFFILE0" = "Outgoing" ] ; then
		# wtw add ifeb 	ebtables $ACTION MAC_FLT -o $IF -j $PERMIT -s $MAC
		echo "Current version does not support ebtables" #wtw add
		elif [ "$DIR_CONFFILE0" = "Both" ] ; then
		# wtw add ifeb 	ebtables $ACTION MAC_FLT -i $IF -j $PERMIT -s $MAC
		# wtw add ifeb 	ebtables $ACTION MAC_FLT -o $IF -j $PERMIT -s $MAC
		echo "Current version does not support ebtables" #wtw add
		fi
	else
		if [ "$DIR_CONFFILE0" = "Incoming" ] ; then
			iptables $ACTION MAC_FLT -i $IF -m mac --mac-source $MAC -j $PERMIT
		elif [ "$DIR_CONFFILE0" = "Outgoing" ] ; then
			iptables $ACTION MAC_FLT -o $IF -m mac --mac-source $MAC -j $PERMIT
		elif [ "$DIR_CONFFILE0" = "Both" ] ; then
			iptables $ACTION MAC_FLT -i $IF -m mac --mac-source $MAC -j $PERMIT
			iptables $ACTION MAC_FLT -o $IF -m mac --mac-source $MAC -j $PERMIT
		fi
	fi
fi
