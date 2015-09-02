#MAC address Settings
MACCONFFILE=/etc/mac.conf
if [ -f $MACCONFFILE ]; then
	chmod 777 $MACCONFFILE
	. $MACCONFFILE
fi
if [ "$LAN_MAC" != "" ]; then
	ifconfig br0 hw ether $LAN_MAC
fi

