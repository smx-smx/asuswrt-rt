WLAN_MODULE_RT3390=/lib/modules/rt3390ap.ko
WLAN_MODULE_RT3090=/lib/modules/rt3090ap.ko
WLAN_NODULE_RT2561=/lib/modules/rt61ap.ko
WLAN_NODULE_RT5392=/lib/modules/rt5390ap.ko

/sbin/ifconfig ra0 down
/sbin/ifconfig ra1 down
/sbin/ifconfig ra2 down
/sbin/ifconfig ra3 down
/sbin/ifconfig eth0.1 down
/sbin/ifconfig eth0.2 down
/sbin/ifconfig eth0.3 down
/sbin/ifconfig eth0.4 down

if [ -f $WLAN_MODULE_RT3390 ] ; then
	/sbin/rmmod rt3390ap
fi

if [ -f $WLAN_MODULE_RT3090 ] ; then
	/sbin/rmmod rt3090ap
fi

if [ -f $WLAN_MODULE_RT2561 ] ; then
	/sbin/rmmod rt61ap
fi

if [ -f $WLAN_MODULE_RT5392 ] ; then
	/sbin/rmmod rt5390ap
fi

/usr/bin/killall -9 syslogd
/usr/bin/killall -9 klogd
/usr/bin/killall -9 dnsmasq
/usr/bin/killall -9 bftpd
/usr/bin/killall -9 utelnetd 
/usr/bin/killall -9 udhcpd
/usr/bin/killall -9 radvd
/usr/bin/killall -9 arp
/usr/bin/killall -9 dropbear
/usr/bin/killall -9 wscd
/usr/bin/killall -9 rt2860apd
/usr/bin/killall -9 tftpd

echo 3 > /proc/sys/vm/drop_caches
