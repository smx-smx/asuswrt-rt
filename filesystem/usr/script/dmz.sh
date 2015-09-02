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
        echo "usage: $0 [PVC#]"
        exit 0
	fi
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
	if [ "$1" = "8" ] || [ "$1" = "9" ] || [ "$1" = "10" ]; then
		PVC="$1"_"$2"
	else
		PVC=$1
	fi	
else
	PVC=$1
fi

iptables -t nat -F DMZ_PRE$PVC

# get ipmode (Single or Multiple)
IPMODE_CONF=/etc/nat_pvc$PVC/ipmode
if [ -f $IPMODE_CONF ] ; then
        chmod +x $IPMODE_CONF
        . $IPMODE_CONF
fi
SERVER_IP="0.0.0.0"
if [ "$IPMODE" = "Multiple" ] ; then
	SERVERIP_CONF=/etc/nat_pvc$PVC/server_ip
	if [ -f $SERVERIP_CONF ] ; then
        	chmod +x $SERVERIP_CONF
        	. $SERVERIP_CONF
	fi
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
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		if [ "$1" = "8" ] || [ "$1" = "9" ] || [ "$1" = "10" ]; then
			WAN_IF=ppp"$1""$2"
		else
			WAN_IF=ppp$1
		fi	
	else
        WAN_IF=ppp$PVC
	fi	
fi

DMZ_CONF=/etc/nat_pvc$PVC/dmz
if [ -f $DMZ_CONF ] ; then
        chmod +x $DMZ_CONF
        . $DMZ_CONF
else
	exit 0
fi

if [ "$Active" = "Yes" ] ; then
    if [ "$SERVER_IP" = "0.0.0.0" ] ; then
	iptables -t nat -I DMZ_PRE$PVC -p all -i $WAN_IF -j DNAT --to $DMZ_IP
    else
	iptables -t nat -I DMZ_PRE$PVC -p all -d $SERVER_IP -j DNAT --to $DMZ_IP
    fi
fi
