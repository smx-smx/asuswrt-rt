#!/bin/sh
# boa applicaion use this file

WLAN_MODULE_RT5390=/lib/modules/rt5390ap.ko
WLAN_MODULE_RT3390=/lib/modules/rt3390ap.ko
WLAN_MODULE_RT3090=/lib/modules/rt3090ap.ko
WLAN_NODULE_RT2561=/lib/modules/rt61ap.ko

/sbin/ifconfig ra0 down 2>/dev/null
/sbin/ifconfig ra1 down 2>/dev/null
/sbin/ifconfig ra2 down 2>/dev/null
/sbin/ifconfig ra3 down 2>/dev/null

if [ -f $WLAN_MODULE_RT5390 ] ; then

	#echo "rmmod rt5390ap"
	/sbin/rmmod rt5390ap 2>/dev/null
fi

if [ -f $WLAN_MODULE_RT3390 ] ; then
	#echo "rmmod rt3390ap"
	/sbin/rmmod rt3390ap 2>/dev/null
fi

if [ -f $WLAN_MODULE_RT3090 ] ; then
	#echo "rmmod rt3090ap"
	/sbin/rmmod rt3090ap 2>/dev/null
fi

if [ -f $WLAN_MODULE_RT2561 ] ; then
	#echo "rmmod rt2561ap"
	/sbin/rmmod rt61ap 2>/dev/null
fi

/usr/bin/ebtables -F
/sbin/rmmod ebt_ip 2>/dev/null
/sbin/rmmod ebtable_filter 2>/dev/null
/sbin/rmmod ebtables 2>/dev/null
/sbin/rmmod sch_htb 2>/dev/null

/usr/bin/killall -9 dnsmasq 2>/dev/null
/usr/bin/killall -9 udhcpd 2>/dev/null
/usr/bin/killall -9 radvd 2>/dev/null
/usr/bin/killall -9 arp 2>/dev/null
/usr/bin/killall -9 wscd 2>/dev/null
/usr/bin/killall -9 tr69 2>/dev/null
/usr/bin/killall -9 dropbear 2>/dev/null
/usr/bin/killall -9 utelnetd 2>/dev/null
/usr/bin/killall -9 klogd 2>/dev/null
/usr/bin/killall -9 syslogd 2>/dev/null
/usr/bin/killall -9 snmpd 2>/dev/null
/usr/bin/killall -9 upnpd 2>/dev/null
/usr/bin/killall -9 udhcpc 2>/dev/null
/usr/bin/killall -9 igmpproxy 2>/dev/null
/usr/bin/killall -9 rt2860apd 2>/dev/null
/usr/bin/killall -9 skbmgr 2>/dev/null
/usr/bin/killall -9 ra_menu 2>/dev/null
/usr/bin/killall -9 pppoe-relay 2>/dev/null

case $1 in
	"1") 
		/usr/bin/killall -9 tftpd 2>/dev/null
		/usr/bin/killall -9 bftpd 2>/dev/null
		/usr/bin/killall -9 inetd 2>/dev/null ;;
	"2")        
		/usr/bin/killall -9 boa 2>/dev/null
		/usr/bin/killall -9 bftpd 2>/dev/null
		/usr/bin/killall -9 inetd 2>/dev/null ;;
	"3")        
		/usr/bin/killall -9 boa 2>/dev/null
		/usr/bin/killall -9 tftpd 2>/dev/null ;;
esac


echo 0 > /proc/net/skbmgr_hot_list_len
echo 3 > /proc/sys/vm/drop_caches
