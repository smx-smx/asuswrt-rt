#!/bin/sh

# udhcpc script edited by Tim Riker <Tim@Rikers.org>
[ -z "$1" ] && echo "Error: should be called from udhcpc" && exit 1
#PROFILE_CFG=/userfs/profile.cfg
#TYPE=`tcapi get Dproxy_Entry type`
if [ -f $PROFILE_CFG ] ; then
	. $PROFILE_CFG
fi
#if [ "$TCSUPPORT_DNSEACHPVC" != "" ] ;then
	RESOLV_CONF="/etc/resolv_"$interface".conf"
#else
#	RESOLV_CONF="/etc/resolv.conf"
#fi
GATEWAY_FILE="/etc/"$interface"_gateway.conf"
WAN_NUM=`expr substr $interface 4 4`
idx=`expr index $WAN_NUM _`
if [ "$idx" != "0" ]; then
	WAN_NUM=`expr substr $WAN_NUM 1 $(( $idx - 1 ))``expr substr $WAN_NUM $(( $idx + 1 )) 1`
fi
LEASE_FILE="/tmp/udhcpc"$WAN_NUM".lease"

[ -n "$broadcast" ] && BROADCAST="broadcast $broadcast"
[ -n "$subnet" ] && NETMASK="netmask $subnet"

case "$1" in
	deconfig)
		#/sbin/ifconfig $interface down
		/sbin/ifconfig $interface 0.0.0.0
		rm -f $GATEWAY_FILE
		rm -f /tmp/udhcpc*
		# WAN_STATE_DISCONNECTED
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_state_t" "4" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_sbstate_t" "3" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_auxstate_t" "0" &		
		;;

	renew|bound)
		/sbin/ifconfig $interface $ip $BROADCAST $NETMASK
		#Not defult route, no need to setup WAN IP in System node.
		#/userfs/bin/tcapi set System_Entry CurrentWANIP $ip &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_ipaddr" "$ip" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_netmask" "$NETMASK" &

		if [ -n "$router" ] ; then
			echo "writing routers to file"
			for i in $router ; do
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

		# Save the lease time, expires time
		echo -n $lease > $LEASE_FILE
		/sbin/udhcpc_expires $interface $lease

		# bound, check network whether the same with lan
		/sbin/udhcpc_bound $ip $subnet $WAN_NUM
		if [ "$?" -ne "0" ] ; then
			exit 1
		fi

		#check extend WAN
		if [ $WAN_NUM -gt 80 ]; then
			if [ ${#WAN_NUM} -gt 2 ]; then
				PREFIX="WanExt_PVC"`expr substr $WAN_NUM 1 2`"e"`expr substr $WAN_NUM 3 1`
			else
				PREFIX="WanExt_PVC"`expr substr $WAN_NUM 1 1`"e"`expr substr $WAN_NUM 2 1`
			fi			
			# Set gateway info
			[ -n "$router" ] && /userfs/bin/tcapi set $PREFIX "gateway_x" "$router" &		
			# Set dns info
			if [ "$DNS_type" != "1" ] ; then
				[ -n "$dns" ] && /userfs/bin/tcapi set $PREFIX "dns_x" "$dns" &
			fi
		else
			# Set gateway info
			[ -n "$router" ] && /userfs/bin/tcapi set "Wan_PVC"$WAN_NUM "gateway_x" "$router" &		
			# Set dns info
			if [ "$DNS_type" != "1" ] ; then
				[ -n "$dns" ] && /userfs/bin/tcapi set "Wan_PVC"$WAN_NUM "dns_x" "$dns" &
			fi
		fi

		# Set gateway info
		echo $router
		[ -n "$router" ] && /userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_gateway" "$router" &
	
		# Set dns info
		if [ "$DNS_type" != "1" ] ; then
			echo $dns
			[ -n "$dns" ] && /userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_dns" "$dns" &
		fi

		# Set static route info
		echo $staticroutes			
		[ -n "$staticroutes" ] && /userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_staticroutes" "$staticroutes" &
		
		# Set 
		echo $msstaticroutes 
		[ -n "$msstaticroutes" ] && /userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_msstaticroutes" "$msstaticroutes" &
		
		/sbin/wan_up $interface

		# WAN_STATE_CONNECTED
		#/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_state_t" "2" &
		#/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_sbstate_t" "0" &
		#/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_auxstate_t" "0" &

		# restart firewall
		#/userfs/bin/tcapi commit "Firewall" &

		# restart QoS
		#/userfs/bin/tcapi commit "QoS" &
		;;
esac

exit 0
