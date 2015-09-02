#!/bin/sh
PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    chmod 777 $PROFILE_CFG
    . $PROFILE_CFG
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
	if [ $# != 4 ] ; then
		echo "usage: $0 [add/del] [PVC#] [Service ID#] [Rule#]"
		exit 0
	fi
else
	if [ $# != 3 ] ; then
		echo "usage: $0 [add/del] [PVC#] [Rule#]"
		exit 0
	fi
fi

if [ $1 = "add" ] ; then
        OPT=-I
elif [ $1 = "del" ] ; then
        OPT=-D
else
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		echo "usage: $0 [add/del] [PVC#] [Service ID#] [Rule#]"
	else	
		echo "usage: $0 [add/del] [PVC#] [Rule#]"
	fi	

    exit 0
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
    if [ "$2" = "8" ] || [ "$2" = "9" ] || [ "$2" = "10" ]; then
		PVC="$2"_"$3"
		RULE=$4
	else
		PVC=$2
		RULE=$4
	fi	
else
	PVC=$2
	RULE=$3
fi

VS_CONF=/etc/nat_pvc$PVC/vserver$RULE
if [ -f $VS_CONF ] ; then
        chmod +x $VS_CONF
        . $VS_CONF
else
	exit 0
fi

EXT_SPORT=$STARTPORT
EXT_EPORT=$ENDPORT
INT_IP=$LOCALIP
if [ "$TCSUPPORT_VIRSVR_LOCAL_PORT" != "" ] ;then
INT_SPORT=$LOCAL_SPORT
INT_EPORT=$LOCAL_EPORT
fi

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
	    if [ "$2" = "8" ] || [ "$2" = "9" ] || [ "$2" = "10" ]; then
			WAN_IF=ppp"$2""$3"
		else
			WAN_IF=ppp$2
		fi	
	else
        WAN_IF=ppp$PVC
	fi		
fi

if [ "$SERVER_IP" = "0.0.0.0" ] ; then
	if [ "$TCSUPPORT_VIRSVR_LOCAL_PORT" != "" ] ;then
	iptables -t nat $OPT VS_PRE$PVC -p tcp --dport $EXT_SPORT:$EXT_EPORT -i $WAN_IF -j DNAT --to $INT_IP:$INT_SPORT-$INT_EPORT

	iptables -t nat $OPT VS_PRE$PVC -p udp --dport $EXT_SPORT:$EXT_EPORT -i $WAN_IF -j DNAT --to $INT_IP:$INT_SPORT-$INT_EPORT
	else
	iptables -t nat $OPT VS_PRE$PVC -p tcp --dport $EXT_SPORT:$EXT_EPORT -i $WAN_IF -j DNAT --to $INT_IP

	iptables -t nat $OPT VS_PRE$PVC -p udp --dport $EXT_SPORT:$EXT_EPORT -i $WAN_IF -j DNAT --to $INT_IP
	fi
else

iptables -t nat $OPT VS_PRE$PVC -p tcp --dport $EXT_SPORT:$EXT_EPORT -d $SERVER_IP -s 0/0  -j DNAT --to $INT_IP

iptables -t nat $OPT VS_PRE$PVC -p udp --dport $EXT_SPORT:$EXT_EPORT -d $SERVER_IP -s 0/0  -j DNAT --to $INT_IP

fi

