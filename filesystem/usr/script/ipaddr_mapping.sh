#!/bin/sh

if [ $# != 3 ] ; then
        echo "usage: $0 [add/del] [PVC#] [Rule#]"
        exit 0
fi

if [ $1 = "add" ] ; then
	OPT=-A
elif [ $1 = "del" ] ; then
	OPT=-D
else
        echo "usage: $0 [add/del] [PVC#] [Rule#]"
	exit 0
fi

PVC=$2
RULE=$3

IPADDRMAPPING_CONF=/etc/nat_pvc$PVC/ipaddr_mapping$RULE
if [ -f $IPADDRMAPPING_CONF ] ; then
        chmod +x $IPADDRMAPPING_CONF
        . $IPADDRMAPPING_CONF
else
	exit 0
fi

# get ipmode (Single or Multiple)
IPMODE_CONF=/etc/nat_pvc$PVC/ipmode
if [ -f $IPMODE_CONF ] ; then
        chmod +x $IPMODE_CONF
        . $IPMODE_CONF
fi

# default wan_if
WAN_IF=nas$PVC
CONFFILE=/etc/isp$PVC.conf
if [ -f $CONFFILE ] ; then
        chmod +x $CONFFILE
        . $CONFFILE
fi
# PPPoE/PPPoA
if [ "$ISP" = "2" ] ; then
        WAN_IF=ppp$PVC
fi

if [ "$IPMODE" = "Single" ] ; then
	if [ "$PUBLICIP" != "0.0.0.0" ] ; then
		for i in $PUBLICIP
		do
			/bin/ip addr del $i dev $WAN_IF label $WAN_IF:0
		done
	fi
	exit 0
fi

if [ "$PUBLICIP" != "0.0.0.0" ] ; then
	if [ $1 = "add" ] ; then
		for i in $PUBLICIP
		do
			/bin/ip addr add $i dev $WAN_IF label $WAN_IF:0
		done
	elif [ $1 = "del" ] ; then
		for i in $PUBLICIP
		do
			/bin/ip addr del $i dev $WAN_IF label $WAN_IF:0
		done
	fi
fi

if [ "$RULETYPE" = "One-to-One" ] ; then
	
    if [ "$PUBLICSTARTIP" = "0.0.0.0" ] ; then
	iptables -t nat $OPT ADDRMAP_PRE$PVC -i $WAN_IF -j DNAT --to $LOCALSTARTIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -s $LOCALSTARTIP -o $WAN_IF -j MASQUERADE
    else
	iptables -t nat $OPT ADDRMAP_PRE$PVC -d $PUBLICSTARTIP -j DNAT --to $LOCALSTARTIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -s $LOCALSTARTIP -j SNAT --to $PUBLICSTARTIP
    fi
	
elif [ "$RULETYPE" = "Many-to-One" ] ; then
	
    if [ "$PUBLICSTARTIP"= "0.0.0.0" ] ; then
#	iptables -t nat $OPT ADDRMAP_PRE$PVC -i $WAN_IF -j DNAT --to $LOCALSTARTIP-$LOCALENDIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -m iprange --src-range $LOCALSTARTIP-$LOCALENDIP -o $WAN_IF -j MASQUERADE
    else
#	iptables -t nat $OPT ADDRMAP_PRE$PVC -d $PUBLICSTARTIP -j DNAT --to $LOCALSTARTIP-$LOCALENDIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -m iprange --src-range $LOCALSTARTIP-$LOCALENDIP -j SNAT --to $PUBLICSTARTIP
    fi
	
elif [ "$RULETYPE" = "Many-to-ManyOverload" ] ; then
	
    if [ "$PUBLICSTARTIP" = "0.0.0.0" ] ; then
#	iptables -t nat $OPT ADDRMAP_PRE$PVC -i $WAN_IF -j DNAT --to $LOCALSTARTIP-$LOCALENDIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -m iprange --src-range $LOCALSTARTIP-$LOCALENDIP -o $WAN_IF -j MASQUERADE
    else
#	iptables -t nat $OPT ADDRMAP_PRE$PVC -m iprange --dst-range $PUBLICSTARTIP-$PUBLICENDIP -j DNAT --to $LOCALSTARTIP-$LOCALENDIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -m iprange --src-range $LOCALSTARTIP-$LOCALENDIP -j SNAT --to $PUBLICSTARTIP-$PUBLICENDIP
    fi
	
elif [ "$RULETYPE" = "Many-to-Many-No-Overload" ] ; then
	
    if [ "$PUBLICSTARTIP" = "0.0.0.0" ] ; then
	iptables -t nat $OPT ADDRMAP_PRE$PVC -i $WAN_IF -j DNAT --to $LOCALSTARTIP-$LOCALENDIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -m iprange --src-range $LOCALSTARTIP-$LOCALENDIP -o $WAN_IF -j MASQUERADE
    else
	iptables -t nat $OPT ADDRMAP_PRE$PVC -m iprange --dst-range $PUBLICSTARTIP-$PUBLICENDIP -j DNAT --to $LOCALSTARTIP-$LOCALENDIP
	iptables -t nat $OPT ADDRMAP_POS$PVC -m iprange --src-range $LOCALSTARTIP-$LOCALENDIP -j SNAT --to $PUBLICSTARTIP-$PUBLICENDIP
    fi

fi
