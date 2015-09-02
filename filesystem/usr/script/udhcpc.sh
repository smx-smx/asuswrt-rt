#!/bin/sh

# udhcpc script edited by Tim Riker <Tim@Rikers.org>

[ -z "$1" ] && echo "Error: should be called from udhcpc" && exit 1
PROFILE_CFG=/userfs/profile.cfg
#TYPE=`tcapi get Dproxy_Entry type`
#echo $TYPE
if [ -f $PROFILE_CFG ] ; then
	. $PROFILE_CFG
fi
#if [ "$TCSUPPORT_DNSEACHPVC" != "" ] ;then
	RESOLV_CONF="/etc/resolv_"$interface".conf"
#else
#	RESOLV_CONF="/etc/resolv.conf"
#fi
GATEWAY_FILE="/etc/"$interface"_gateway.conf"
IFNAME=`expr substr $interface 1 3`
if [ "$IFNAME" = "eth" -o "$IFNAME" = "usb" ]; then
WAN_NUM=11
else
WAN_NUM=`expr substr $interface 4 2`
fi

if [ "`expr index $WAN_NUM _`" != "0" ]; then
WAN_NUM=`expr substr $WAN_NUM 1 1`
fi
LEASE_FILE="/tmp/udhcpc"$WAN_NUM".lease"

[ -n "$broadcast" ] && BROADCAST="broadcast $broadcast"
[ -n "$subnet" ] && NETMASK="netmask $subnet"

if [ "$WAN_NUM" = "11" ]; then
ISPCONF="/etc/isp""$WAN_NUM"".conf"
elif [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" -a $WAN_NUM -gt 7 -a $WAN_NUM -lt 11 ]; then
ISPCONF="/etc/isp""$WAN_NUM""_0.conf"
else
ISPCONF=/etc/isp$WAN_NUM.conf
fi
if [ -f $ISPCONF ] ; then
	chmod 777 $ISPCONF
	. $ISPCONF
else
	exit 0
fi

VPNC_CFG=/etc/vpnc.conf
if [ -f $VPNC_CFG ] ; then
    . $VPNC_CFG
fi

DUALWAN_CFG=/etc/dualwan.conf
if [ -f $DUALWAN_CFG ] ; then
	. $DUALWAN_CFG
else
	DUALWAN_ENABLED=0
fi
if $( echo "$wans_dualwan" | grep -q 'none' )
then
	DUALWAN_ENABLED=0
else
	DUALWAN_ENABLED=1
fi
case "$1" in
	deconfig)
		#/sbin/ifconfig $interface down
		/sbin/ifconfig $interface 0.0.0.0
		rm -f $GATEWAY_FILE
		rm -f /tmp/udhcpc*
		;;

	renew|bound)
		/sbin/ifconfig $interface $ip $BROADCAST $NETMASK
		/sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_ipaddr" "$ip"
		/sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_netmask" "$subnet"

		if [ -n "$router" -a -z "$DUALWAN_ENABLED" ] ; then
			echo "deleting routers"
			while /sbin/route del default gw 0.0.0.0 dev $interface ; do
				:
			done

			echo "adding default routers"
			for i in $router ; do
				echo "route add default gw $i dev $interface"
				/sbin/route add default gw $i dev $interface
				echo "gateway=$i" > $GATEWAY_FILE
			done
		fi

		#if [ $TYPE != "1" ] ; then
			echo -n > $RESOLV_CONF
			for i in $dns ; do
				echo adding dns $i
				echo nameserver $i >> $RESOLV_CONF
			done
			[ -n "$domain" ] && echo search $domain >> $RESOLV_CONF
		#fi

		# Save the lease time
		echo -n $lease > $LEASE_FILE
		/sbin/udhcpc_expires $interface $lease

		# bound, check network whether the same with lan
		/sbin/udhcpc_bound $ip $subnet $WAN_NUM
		if [ "$?" -ne "0" ] ; then
			exit 1
		fi

		# Set gateway info
		[ -n "$router" ] && /sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_gateway" "$router"

		# Set dns info
		[ -n "$dns" ] && /sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_dns_rx" "$dns"

		echo "[udhcpc.sh]DNS=$dns" > /dev/console
		/sbin/wan_up $interface

		;;
esac

exit 0
