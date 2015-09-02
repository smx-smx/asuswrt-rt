#!/bin/sh

Interface="eth0.4"
iptables -D FORWARD -o $Interface -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
iptables -D FORWARD -o ppp100 -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu

iptables -t nat -D POSTROUTING -o $Interface -j MASQUERADE
iptables -t nat -D POSTROUTING -o ppp100 -j MASQUERADE
iptables -F PKT_FLT

if [ -f /var/run/udhcpc-$Interface.pid ] ; then
	#shnwind modify ->use SIGTERM to send release
	kill -SIGTERM `cat /var/run/udhcpc-$Interface.pid`
fi

if [ -f /var/run/ppp100.pid ] ; then
        kill -15 `cat /var/run/ppp100.pid`
        sleep 1
	if [ -f /var/run/ppp100.pid ] ; then
        	kill -9 `cat /var/run/ppp100.pid`
	fi
fi
/sbin/ifconfig $Interface down
#brctl addif br0 $Interface

