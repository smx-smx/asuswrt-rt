PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi


if [ "$TCSUPPORT_WLAN" != "" ] ;then
echo "TCSUPPORT_WLAN: ifconfig"
i=0
CONFFILE=/etc/Wireless/WLAN_APOn

if [ -f $CONFFILE ]; then
	chmod 777 $CONFFILE
	. $CONFFILE
	while [ "$i" -lt "$Bssid_num" ];do
		ifconfig ra$i 0.0.0.0 down
		i=`expr $i + 1`
	done
fi
fi


/usr/bin/killall -9 dnsmasq
/usr/bin/killall -9 bftpd
/usr/bin/killall -9 utelnetd 
/usr/bin/killall -9 udhcpd
/usr/bin/killall -9 radvd
/usr/bin/killall -9 arp
/usr/bin/killall -9 inetd
/usr/bin/killall -9 tftpd
/usr/bin/killall -9 wscd
/usr/bin/killall -9 tr69


echo "3" > /proc/sys/vm/drop_caches
