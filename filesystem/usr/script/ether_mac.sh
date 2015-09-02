#!/bin/sh
CONFFILE=/etc/mac.conf
if [ -f $CONFFILE ]; then
	chmod 777 $CONFFILE
	. $CONFFILE
fi

if [ "$LAN_MAC" != "" ]; then
	/sbin/ifconfig br0 hw ether $LAN_MAC
fi
