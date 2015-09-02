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

#
# Rebuild NAT rule
#
#NAT Virtual Server
for j in 0 1 2 3 4 5 6 7 8 9
do
	VS_CONF=/etc/nat_pvc$i/vserver$j
	if [ -f $VS_CONF ] ; then
		if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
			/usr/script/vserver.sh add $1 $2 $j
		else
			/usr/script/vserver.sh add $i $j
		fi
	fi
done

#NAT DMZ
DMZ_CONF=/etc/nat_pvc$i/dmz
if [ -f $DMZ_CONF ] ; then
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		/usr/script/dmz.sh $1 $2
	else
		/usr/script/dmz.sh $i
	fi
fi

#NAT IP address mapping
#for j in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
#do
#	IPADDRMAPPING_CONF=/etc/nat_pvc$i/ipaddr_mapping$j
#	if [ -f $IPADDRMAPPING_CONF ] ; then
#		/usr/script/ipaddr_mapping.sh add $i $j
#	fi
#done
