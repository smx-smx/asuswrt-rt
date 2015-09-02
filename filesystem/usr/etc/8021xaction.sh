#!/bin/sh
########################################################
##					Auther : Brian					  ##														
########################################################
IFNAME=$1		#interface name(nas0,nas1,...)
CMD=$2			#CONNECTED or DISCONNECTED
i=$3			#interface index(0,1,2...)
RENEWVERSION=$4	#1:v4 renew    3:v6 renew

CONFFILE=/etc/isp$i.conf
echo $CONFFILE

if [ -f $CONFFILE ] ; then
	chmod 777 $CONFFILE
	. $CONFFILE
else
	exit 0
fi

MAC_CONFFILE=/etc/mac.conf
if [ -f $MAC_CONFFILE ]; then
	chmod 777 $MAC_CONFFILE
	. $MAC_CONFFILE
fi

if [ "$Active" != "Yes" ] ; then
	exit 0
fi

if [ $ISP = "0" ] ; then
	if [ "$CMD" = "CONNECTED" ]; then
		#authentication successes
		if [ "$IPVERSION" != "IPv6" ] ; then
			#if udhcpc-nasx.pid exists,it means dhcp renew trigger 802.1x authentication,so if success,just send singal to udhcpc
			if [ -f /var/run/udhcpc-nas$i.pid ] ; then
				if [ "$RENEWVERSION" = "1" ] || [ "$RENEWVERSION" = "0" ]; then
					kill -SIGUSR1 `cat /var/run/udhcpc-nas$i.pid`
					/userfs/bin/tcapi set IPOEDot1xInfo_Entry renew_${IFNAME}_v4 0
				fi
			else
			#else,it means first 802.1x authentication,need to start udhcpc
				if [ "$WAN_MAC" != "" ]; then
					if [ "$DEFAULTROUTE" = "Yes" ] ; then
						/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC &
					else
						/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC &
					fi
				fi
			fi
		fi
	
		if [ "$IPVERSION" != "IPv4" ] ; then
			#handle dhcp6c
			if [ -f /var/run/dhcp6c_nas$i.pid ] ; then
				if [ "$RENEWVERSION" = "3" ] || [ "$RENEWVERSION" = "0" ]; then
					ip6_wanip=`/userfs/bin/tcapi get DeviceInfo_PVC$i IP6WanIP`
					if [ $ip6_wanip != "" ] ; then
						kill -SIGHUP `cat /var/run/dhcp6c_nas$i.pid`
					else
						kill -SIGHUP `cat /var/run/dhcp6c_nas$i.pid`
					fi
					/userfs/bin/tcapi set IPOEDot1xInfo_Entry renew_${IFNAME}_v6 0
				fi
			else
				/userfs/bin/dhcp6c -c /etc/dhcp6c_nas$i.conf -p /var/run/dhcp6c_nas$i.pid $IFNAME &
			fi
		fi	
	fi

#	if [ "$CMD" = "DISCONNECTED" ]; then
		#disconnection operation
#	fi

#If use retry mechanism when authentication fail,follows are not necessarily,and if not use,need to add following operations
	if [ "$CMD" = "AUTHFAILURE" ]; then
		if [ "$IPVERSION" != "IPv6" ] ; then
			if [ -f /var/run/udhcpc-nas$i.pid ] ; then
				if [ "$RENEWVERSION" = "1" ] || [ "$RENEWVERSION" = "0" ]; then
					#use SIGTERM to send release
					kill -SIGTERM `cat /var/run/udhcpc-nas$i.pid`
				fi
			fi
		fi
		
		if [ "$IPVERSION" != "IPv4" ] ; then
			if [ -f /var/run/dhcp6c_nas$i.pid ] ; then
				if [ "$RENEWVERSION" = "3" ] || [ "$RENEWVERSION" = "0" ]; then
					#use SIGTERM to send release
					kill -SIGTERM `cat /var/run/dhcp6c_nas$i.pid`
				fi
			fi
		fi
	fi
fi

