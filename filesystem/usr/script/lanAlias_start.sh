#!/bin/sh

if [ $# != 1 ] ; then
	echo "usage: $0 [PVCn]"
	exit 0
fi

i=$1
CONFFILE=/etc/lanAlias$i.conf
echo $CONFFILE

if [ -f $CONFFILE ] ; then
	chmod 777 $CONFFILE
	. $CONFFILE
else
	exit 0
fi

if [ "$Active" = "Yes" ] ; then
	#/sbin/ifconfig br0:$i $IP netmask $netmask up
	/sbin/ifconfig br0:$i $IP netmask $netmask up
fi
