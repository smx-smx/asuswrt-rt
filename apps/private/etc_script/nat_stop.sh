#!/bin/sh

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
	if [ $# != 2 ] ; then
		echo "usage: $0 [PVC#] [Service ID#]"
		exit 0
	fi
else
	if [ $# != 1 ] ; then
		echo "usage: $0 [PVCn]"
		exit 0
	fi
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
    if [ "$1" = "8" ] || [ "$1" = "9" ] || [ "$1" = "10" ]; then
		i="$1"_"$2"
	else
		i=$1
	fi	
else
	i=$1
fi

#iptables -t nat -D POSTROUTING -o nas$i -j MASQUERADE
#iptables -t nat -D POSTROUTING -o ppp$i -j MASQUERADE
#iptables -t nat -D POSTROUTING -j ADDRMAP_POS$i
#iptables -t nat -D PREROUTING -j VS_PRE$i
#iptables -t nat -D PREROUTING -j DMZ_PRE$i
#iptables -t nat -D PREROUTING -j ADDRMAP_PRE$i
# flush nat rule
iptables -t nat -F VS_PRE$i
iptables -t nat -F DMZ_PRE$i
#iptables -t nat -F ADDRMAP_PRE$i
#iptables -t nat -F ADDRMAP_POS$i

