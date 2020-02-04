#!/bin/sh

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi

if [ $1 -lt 8 -o $1 -gt 10 ]; then
	if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
		if [ $# -gt 3 ] ; then
			echo "usage: $0 [PVCn] [UniqueMac Flag] [Unique Mac]"
			exit 0
		fi
	else
		if [ $# != 1 ] ; then
			echo "usage: $0 [PVCn]"
			exit 0
		fi
	fi
else
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
		if [ $# != 2 ]; then
			echo "usage: $0 [PVCn] [Service_Num]"
			exit 0
		fi
	else	
		if [ $# != 1 ] ; then
			echo "usage: $0 [PVCn]"
			exit 0
		fi
	fi
fi

i=$1
k=$1
if [ "$i" = "8" ] || [ "$i" = "9" ] || [ "$i" = "10" ]; then
	isPTMETHER=1
else
	isPTMETHER=0
fi
if [ "$i" = "12" ]; then
	isLAN_ETHERWAN=1
else
	isLAN_ETHERWAN=0
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
	if [ "$isPTMETHER" = "1" ] ; then
		org_i=$i
		serv_num=$2
		i="$i"_"$serv_num"
		k="$org_i""$serv_num"
		ifconfig nas$org_i up
	fi
fi

if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
	UNIQUEMAC_FLAG=$2
	UNIQUE_MAC=$3
fi

CONFFILE=/etc/isp$i.conf
echo $CONFFILE

if [ -f $CONFFILE ] ; then
	chmod 777 $CONFFILE
	. $CONFFILE
else
	exit 0
fi

if [ "$WAN_MAC" = "" ] ; then
	MAC_CONFFILE=/etc/mac.conf
	if [ -f $MAC_CONFFILE ]; then
		chmod 777 $MAC_CONFFILE
		. $MAC_CONFFILE
	fi
	WAN_MAC=$LAN_MAC
fi

if [ "$Active" != "Yes" ] ; then
	exit 0
fi

# power up Ethernet Wan
#this case means the model including Ethernet WAN.
#if [ "$TCSUPPORT_MTK_INTERNAL_ETHER_SWITCH" = "" -a "$i" = "10" ]; then
	#rtkethcmd up wan
	#sleep 2
#if [ "$TCSUPPORT_MT7530_EXTERNAL" != "" ] ; then
#	isEtherUp=`cat /proc/tc3162/eth1_link_st`
#else
	#this case means the model using RTK giga switch.
#	isEtherUp=`cat /proc/tc3162/eth_wan_link_st`
#fi
#	if [ "$isEtherUp" = "0" ]; then
#		exit 0
#	fi
#fi

# WAN_STATE_CONNECTING
if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
	if [ "$isPTMETHER" = "1" ] ; then
		if [ "$serv_num" = "0" ]; then
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_state_t" "1" &
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_sbstate_t" "0" &
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_auxstate_t" "0" &
		fi
	else
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_state_t" "1" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_sbstate_t" "0" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_auxstate_t" "0" &
	fi
else
	/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_state_t" "1" &
	/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_sbstate_t" "0" &
	/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_auxstate_t" "0" &
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

## function to set clone MAC address
setCloneMAC()
{
	if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
		if [ "$UNIQUEMAC_FLAG" = "1" ]; then
			/sbin/ifconfig nas$i hw ether $UNIQUE_MAC
		else
			if [ "$WAN_MAC" != "" ]; then
				/sbin/ifconfig nas$i hw ether $WAN_MAC
			fi
		fi
	else
		if [ "$WAN_MAC" != "" ]; then
			/sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi
}

if [ $ISP = "0" ] ; then

	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add ipoe nas$org_i nas$i
	fi	

	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
		echo 1 > /proc/sys/net/ipv6/conf/nas$i/accept_ra 
	elif [ "$IPVERSION" = "IPv6" ] ; then
		echo 1 > /proc/sys/net/ipv6/conf/nas$i/accept_ra 		
	fi
	
	if [ "$QOS" = "ubr" ] ; then
		PCR_V="-p $PCR"
	elif [ "$QOS" = "cbr" ] || [ "$QOS" = "rt-vbr" ] || [ "$QOS" = "nrt-vbr" ] ; then
		PCR_V="-p $PCR -q $SCR -m $MBS"
	fi
	
	if [ "$ENCAP" = "1483 Bridged IP LLC" ] ; then
		ENCAP_T="-e 0"
	elif [ "$ENCAP" = "1483 Bridged IP VC-Mux" ] ; then
		ENCAP_T="-e 1"
	elif [ "$ENCAP" = "1483 Routed IP LLC(IPoA)" ] ; then
		ENCAP_T="-e 2"
	elif [ "$ENCAP" = "1483 Routed IP VC-Mux" ] ; then
		ENCAP_T="-e 3"
	fi
	
	if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		#ATM only
		if [ "$isPTMETHER" = "0" ] ; then
			br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
			echo $! > /var/run/nas$i.pid
			sleep 1
		fi
	else
		br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
		echo $! > /var/run/nas$i.pid
		sleep 1
	fi

	/sbin/ifconfig nas$i down

	setCloneMAC

	if [ "$IPVERSION" != "IPv4" ] && [ "$EnableDynIPv6" = "0" ]; then
		echo 1 > /proc/sys/net/ipv6/conf/nas$i/autoconf
		if [ "$TCSUPPORT_IPV6_PRIVACYADDRS" != "" ] ;then
			echo $IPv6Extension > /proc/sys/net/ipv6/conf/nas$i/use_tempaddr
		fi
	fi
	/sbin/ifconfig nas$i 0.0.0.0
	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig nas$i mtu 1500
		# iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig nas$i mtu $MTU
		# iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	if [ "$IPVERSION" != "IPv6" ] ; then
		if [ -f /var/run/udhcpc-nas$i.pid ] ; then
			kill -9 `cat /var/run/udhcpc-nas$i.pid`
		fi
		if [ "$wan_hostname" != "" ] ; then
			UDHCPC_PARAM="-H $wan_hostname"
		fi		
		if [ "$DEFAULTROUTE" != "Yes" ] ; then
			UDHCPC_PARAM="$UDHCPC_PARAM -o -O subnet -O dns -O hostname -O router -O domain -O broadcast -O staticroutes"
		fi		
               if [ "$ASUS_BUSYBOX_NEW" != "" ]  ; then
			if [ "$dhcp_vendorid" != "" ] ; then
				UDHCPC_PARAM="$UDHCPC_PARAM -V $dhcp_vendorid"
			fi
			if [ "$dhcp_clientid_val" != "" ] ; then
				UDHCPC_PARAM="$UDHCPC_PARAM -x $dhcp_clientid_val"
			fi
			if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
				if [ "$UNIQUEMAC_FLAG" = "1" ]; then
					if [ "$DEFAULTROUTE" = "Yes" ] ; then
						/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -a $UNIQUE_MAC $UDHCPC_PARAM &
					else
						/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -a $UNIQUE_MAC $UDHCPC_PARAM &
					fi
				else	
					if [ "$WAN_MAC" != "" ]; then
						if [ "$DEFAULTROUTE" = "Yes" ] ; then
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -a $WAN_MAC $UDHCPC_PARAM &
						else
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -a $WAN_MAC $UDHCPC_PARAM &
						fi
					fi
				fi
			else
				if [ "$TCSUPPORT_WPA_SUPPLICANT" = "" ] || [ "$IPOE_DOT1X_STATUS" != "Enable" ];then
					if [ "$WAN_MAC" != "" ]; then
						if [ "$DEFAULTROUTE" = "Yes" ] ; then
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -a $WAN_MAC $UDHCPC_PARAM &
						else
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -a $WAN_MAC $UDHCPC_PARAM &
						fi
					fi
				fi
			fi
               else
			if [ "$dhcp_clientid" != "" ] ; then
				UDHCPC_PARAM="$UDHCPC_PARAM -c $DHCP_clientid"
			fi
			if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
				if [ "$UNIQUEMAC_FLAG" = "1" ]; then
					if [ "$DEFAULTROUTE" = "Yes" ] ; then
						/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -m $UNIQUE_MAC $UDHCPC_PARAM &
					else
						/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -m $UNIQUE_MAC $UDHCPC_PARAM &
					fi
				else	
					if [ "$WAN_MAC" != "" ]; then
						if [ "$DEFAULTROUTE" = "Yes" ] ; then
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC $UDHCPC_PARAM &
						else
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC $UDHCPC_PARAM &
						fi
					fi
				fi
			else
				if [ "$TCSUPPORT_WPA_SUPPLICANT" = "" ] || [ "$IPOE_DOT1X_STATUS" != "Enable" ];then
					if [ "$WAN_MAC" != "" ]; then
						if [ "$DEFAULTROUTE" = "Yes" ] ; then
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC $UDHCPC_PARAM &
						else
							/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC $UDHCPC_PARAM &
						fi
					fi
				fi
			fi
               fi

		# Set dns info
		if [ "$DNS_type" = "1" ] ; then
			if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
				if [ "$isPTMETHER" = "1" ] ; then
					if [ "$serv_num" = "0" ]; then
						/userfs/bin/tcapi set "WanExt_PVC"$org_i"e"$serv_num "dns_x" "$Primary_DNS $Secondary_DNS" &
						/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_dns" "$Primary_DNS $Secondary_DNS" &
					fi
				else
					/userfs/bin/tcapi set "Wan_PVC"$i "dns_x" "$Primary_DNS $Secondary_DNS" &
					/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_dns" "$Primary_DNS $Secondary_DNS" &
				fi
			else
				/userfs/bin/tcapi set "Wan_PVC"$i "dns_x" "$Primary_DNS $Secondary_DNS" &
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_dns" "$Primary_DNS $Secondary_DNS" &
			fi
		fi

		# if [ "$NATENABLE" = "Enable" ] ; then
			# iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
			# if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
				# iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
			# fi	
			# iptables -t nat -A POSTROUTING -o nas$i -j MASQUERADE
			# iptables -t nat -A PREROUTING -j VS_PRE$i
			# iptables -t nat -A PREROUTING -j DMZ_PRE$i
			# iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
			# if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
				# iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
			# fi
			# if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
				# iptables -t nat -A PREROUTING -i nas$i -j PREROUTING_WAN
				# iptables -t filter -A FORWARD -i nas$i -j FORWARD_WAN
			# fi
		# fi
	fi
	WAN_IF=nas$i


elif [ $ISP = "1" ] ; then

	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add ipoe nas$org_i nas$i
	fi	
	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
		echo 1 > /proc/sys/net/ipv6/conf/nas$i/accept_ra 
	elif [ "$IPVERSION" = "IPv6" ] ; then
		echo 1 > /proc/sys/net/ipv6/conf/nas$i/accept_ra 		
	fi
	
	if [ "$QOS" = "ubr" ] ; then
		PCR_V="-p $PCR"
	elif [ "$QOS" = "cbr" ] || [ "$QOS" = "rt-vbr" ] || [ "$QOS" = "nrt-vbr" ] ; then
		PCR_V="-p $PCR -q $SCR -m $MBS"
	fi
	
	if [ "$ENCAP" = "1483 Bridged IP LLC" ] ; then
		ENCAP_T="-e 0"
	elif [ "$ENCAP" = "1483 Bridged IP VC-Mux" ] ; then
		ENCAP_T="-e 1"
	elif [ "$ENCAP" = "1483 Routed IP LLC(IPoA)" ] ; then
		ENCAP_T="-e 2"
	elif [ "$ENCAP" = "1483 Routed IP VC-Mux" ] ; then
		ENCAP_T="-e 3"
	fi
	
	if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		#ATM only
		if [ "$isPTMETHER" = "0" ] ; then
			br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
			echo $! > /var/run/nas$i.pid
			sleep 1
		fi
	else
		br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
		echo $! > /var/run/nas$i.pid
		sleep 1
	fi

	/sbin/ifconfig nas$i down

	setCloneMAC

	#	/sbin/ifconfig nas$i $IPADDR netmask $NETMASK up
	#	/sbin/ifconfig nas$i $IPADDR6/$PREFIX6
	#	echo -e "server=$DNSIPv61st@nas$i\\nserver=$DNSIPv62nd@nas$i">>/etc/dnsmasq.conf
	#	echo -e "nameserver $DNSIPv61st\\nnameserver $DNSIPv62nd">/var/run/dhcp6c-nas$i.info

	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig nas$i mtu 1500
		# iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig nas$i mtu $MTU
		# iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	/sbin/ifconfig nas$i up
	if [ "$IPVERSION" != "IPv6" ] ; then	
		/sbin/ifconfig nas$i $IPADDR netmask $NETMASK
	fi
	if [ "$IPVERSION" != "IPv4" ] ; then
		/sbin/ifconfig nas$i $IPADDR6/$PREFIX6
		echo -e "nameserver $DNSIPv61st\\nnameserver $DNSIPv62nd">/var/run/dhcp6c-nas$i.info
	fi

	if [ "$DEFAULTROUTE" = "Yes"  -a -z "$DUALWAN_ENABLED" ] ; then
		if [ "$IPVERSION" != "IPv6" ] ; then
			route add default gw $GATEWAY dev nas$i
		fi
		if [ "$IPVERSION" != "IPv4" ] ; then
			route -A inet6 add default gw $DEFGATEWAY6 dev nas$i 
			echo -e "$IPADDR6/$PREFIX6">/var/tmp/ipaddr6_nas$i.msg
			echo -e "$DEFGATEWAY">/var/tmp/gw6_nas$i.msg
		fi
		#/sbin/route -A inet6 add $IPADDR6/$PREFIX6 gw $DEFGATEWAY6 dev $WAN_IF
		#echo -e "server=$DNSIPv61st@nas$i\\nserver=$DNSIPv62nd@nas$i">>/etc/dnsmasq.conf
		# route add default gw 
	fi

	# if [ "$IPVERSION" != "IPv6" ] ; then
		# if [ "$NATENABLE" = "Enable" ] ; then
			# iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
			# if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
				# iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
			# fi
			# iptables -t nat -A POSTROUTING -o nas$i -j MASQUERADE
			# iptables -t nat -A PREROUTING -j VS_PRE$i
			# iptables -t nat -A PREROUTING -j DMZ_PRE$i
			# iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
			# if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
				# iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
			# fi
			# if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
				# iptables -t nat -A PREROUTING -i nas$i -j PREROUTING_WAN
				# iptables -t filter -A FORWARD -i nas$i -j FORWARD_WAN
			# fi
		# fi
	# fi
	WAN_IF=nas$i
	
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
		if [ "$isPTMETHER" = "1" ] ; then
			if [ "$serv_num" = "0" ]; then
				# WAN_STATE_CONNECTED
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_state_t" "2" &
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_sbstate_t" "0" &
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_auxstate_t" "0" &
				# Set IP info
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_ipaddr" "$IPADDR" &
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_netmask" "$NETMASK" &
				# Set gateway info
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_gateway" "$GATEWAY" &
				# Set dns info
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_dns" "$Primary_DNS $Secondary_DNS" &
			fi
		else
			# WAN_STATE_CONNECTED
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_state_t" "2" &
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_sbstate_t" "0" &
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_auxstate_t" "0" &
			# Set IP info
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_ipaddr" "$IPADDR" &
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_netmask" "$NETMASK" &
			# Set gateway info
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_gateway" "$GATEWAY" &
			# Set dns info
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_dns" "$Primary_DNS $Secondary_DNS" &
		fi
	else
		# WAN_STATE_CONNECTED
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_state_t" "2" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_sbstate_t" "0" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_auxstate_t" "0" &
		# Set IP info
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_ipaddr" "$IPADDR" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_netmask" "$NETMASK" &
		# Set gateway info
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_gateway" "$GATEWAY" &
		# Set dns info
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_dns" "$Primary_DNS $Secondary_DNS" &
	fi

	/sbin/wan_up $WAN_IF &

	# restart firewall
	#/userfs/bin/tcapi commit "Firewall" &

	# restart QoS
	#/userfs/bin/tcapi commit "QoS" &

	# restart ddns
	#/userfs/bin/tcapi commit "Ddns" &

	# check and start vpnc
	#if [ "$auto_conn" = "1" ] ; then
		#/userfs/bin/tcapi commit "VPNC" &
		#killall run_vpnc
		#if [ -f /var/run/l2tpd-vpnc.pid ] ; then
			#kill -15 `cat /var/run/l2tpd-vpnc.pid`
			#sleep 1
		#fi
		#if [ -f /var/run/ppp20.pid ] ; then
			#kill -15 `cat /var/run/ppp20.pid`
			#sleep 2 
			#if [ -f /var/run/ppp20.pid ] ; then
				#kill -9 `cat /var/run/ppp20.pid`
				#rm -f /var/run/ppp20.pid
			#fi
		#fi
		#/sbin/run_vpnc &
	#fi

elif [ $ISP = "2" ] ; then
	# for pppd success or fail
	/bin/ln -sf /sbin/rc /etc/ppp/ip-up
	/bin/ln -sf /sbin/rc /etc/ppp/ip-down
	/bin/ln -sf /sbin/rc /etc/ppp/auth-fail

	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add pppoe nas$org_i nas$i
	fi	
	if [ "$CONNECTION" != "Connect_Manually" ] ; then
  		#make sure the LCP echo will take no effect about ppp
		if [ "$TCSUPPORT_CZ_GENERAL" != "" ] ;then
			PPP_PARAM="unit $i user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns"
		else
			if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
				PPP_PARAM="unit "$org_i""$serv_num" user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns"
			else
				PPP_PARAM="unit $i user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns"
			fi
		fi

		# For Internet Detection
		if [ -z $InetDetect ] || [ "$InetDetect"x == "1"x ]; then
			if [ ! -z $lcpEchoInterval ]; then
				LCP_ECHO_INTERVAL="lcp-echo-interval $lcpEchoInterval"
			else # No $lcpEchoInterval, also set it as default
				LCP_ECHO_INTERVAL="lcp-echo-interval 6"
			fi
			PPP_PARAM="$PPP_PARAM $LCP_ECHO_INTERVAL"
			if [ ! -z $lcpEchoFailure ]; then
				LCP_ECHO_FAILURE="lcp-echo-failure $lcpEchoFailure"
			else # No $lcpEchoFailure, also set it as default
				LCP_ECHO_FAILURE="lcp-echo-failure 10"
			fi
			PPP_PARAM="$PPP_PARAM $LCP_ECHO_FAILURE"
		elif [ "$InetDetect"x == "0"x ]; then
			LCP_ECHO_INTERVAL="lcp-echo-interval 0"
			PPP_PARAM="$PPP_PARAM $LCP_ECHO_INTERVAL"
		fi
		
		PPP_PARAM="$PPP_PARAM novj"

		if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
			if [ "$isPTMETHER" = "1" -o "$isLAN_ETHERWAN" = "1" ] ; then
				PPP_PARAM="$PPP_PARAM plugin libpppoe.so nas$i"
				if [ "$SRVNAME" != "" ] ; then
					PPP_PARAM="$PPP_PARAM rp_pppoe_service $SRVNAME" 
				fi 
				ENCAP=""
			fi
		fi

		if [ "$ENCAP" = "PPPoE LLC" ] || [ "$ENCAP" = "PPPoE VC-Mux" ] ; then
			PPP_PARAM="$PPP_PARAM plugin libpppoe.so" 
			if [ "$SRVNAME" != "" ] ; then
				PPP_PARAM="$PPP_PARAM rp_pppoe_service $SRVNAME" 
			fi
			PPP_PARAM="$PPP_PARAM nas$i" 
		elif [ "$ENCAP" = "PPPoA LLC" ] ; then
			PPP_PARAM="$PPP_PARAM plugin libpppoatm.so llc-encaps $VPI.$VCI"
		elif [ "$ENCAP" = "PPPoA VC-Mux" ] ; then
			PPP_PARAM="$PPP_PARAM plugin libpppoatm.so vc-encaps $VPI.$VCI"
		fi

		/sbin/ifconfig nas$i down

		if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
			if [ "$isPTMETHER" = "1" -o "$isLAN_ETHERWAN" = "1" ] ; then
				if [ "$AUTHEN" = "CHAP" ] ; then
					PPP_PARAM="$PPP_PARAM -pap"
				elif [ "$AUTHEN" = "PAP" ] ; then
					PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
				fi

				setCloneMAC

				/sbin/ifconfig nas$i 0.0.0.0
				ENCAP=""
				AUTHEN=""	
			fi
		fi

		# PPPoE
		if [ "$ENCAP" = "PPPoE LLC" ] || [ "$ENCAP" = "PPPoE VC-Mux" ] ; then
			if [ "$AUTHEN" = "CHAP" ] ; then
				PPP_PARAM="$PPP_PARAM -pap"
			elif [ "$AUTHEN" = "PAP" ] ; then
				PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
			fi

			if [ "$QOS" = "ubr" ] ; then
				PCR_V="-p $PCR"
			elif [ "$QOS" = "cbr" ] || [ "$QOS" = "rt-vbr" ] || [ "$QOS" = "nrt-vbr" ] ; then
				PCR_V="-p $PCR -q $SCR -m $MBS"
			fi

			if [ "$ENCAP" = "PPPoE LLC" ] ; then
				ENCAP_T="-e 0"
    			elif [ "$ENCAP" = "PPPoE VC-Mux" ] ; then
				ENCAP_T="-e 1"
			fi

			br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
			echo $! > /var/run/nas$i.pid
			sleep 1

			setCloneMAC

			/sbin/ifconfig nas$i 0.0.0.0

			# PPPoA
		else
			if [ "$AUTHEN" = "CHAP" ] ; then
				PPP_PARAM="$PPP_PARAM -pap"
			elif [ "$AUTHEN" = "PAP" ] ; then
				PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
			fi

			PCR_V=""
			if [ "$QOS" = "ubr" ] ; then
				PCR_V="qos UBR qos_pcr $PCR"
			elif [ "$QOS" = "cbr" ] || [ "$QOS" = "rt-vbr" ] || [ "$QOS" = "nrt-vbr" ] ; then
				if [ "$QOS" = "cbr" ] ; then 
					PCR_V="qos CBR"
				elif [ "$QOS" = "rt-vbr" ] ; then
					PCR_V="qos VBR"
			elif [ "$QOS" = "nrt-vbr" ] ; then
				PCR_V="qos NRTVBR"
			fi
			PCR_V="$PCR_V qos_pcr $PCR qos_scr $SCR qos_mbs $MBS"
		fi

		if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
			if [ "$isPTMETHER" = "1" ] ; then
				PCR_V=""
			fi
		fi
		PPP_PARAM="$PPP_PARAM $PCR_V"
    	fi

	if [ "$DEFAULTROUTE" = "Yes" ] ; then
		PPP_PARAM="$PPP_PARAM defaultroute"
	fi
	if [ "$PPPGETIP" = "Dynamic" ] ; then
		PPP_PARAM="$PPP_PARAM noipdefault"
	else
		PPP_PARAM="$PPP_PARAM $IPADDR:$GATEWAY"
	fi
	if [ "$NETMASK" != "" ] ; then
		PPP_PARAM="$PPP_PARAM netmask $NETMASK"
	fi
	if [ "$CONNECTION" = "Connect_on_Demand" ] ; then
		# transform minute to second. shnwind 2008.4.17
		IDLETIME=$(expr $CLOSEIFIDLE \* 60)
		PPP_PARAM="$PPP_PARAM demand idle $IDLETIME"
	else
		PPP_PARAM="$PPP_PARAM persist"
	fi	
	if [ "$MTU" != "0" ] && [ "$MTU" != "" ] ; then
		PPP_PARAM="$PPP_PARAM mtu $MTU mru $MTU"
	else
		PPP_PARAM="$PPP_PARAM mtu 1492 mru 1492"
	fi

	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
		echo 0 > /proc/sys/net/ipv6/conf/nas$i/accept_ra 
		if [ "$CONNECTION" = "Connect_on_Demand" ] ; then
			if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
				localIP="::11"$org_i""$serv_num""
				remoteIP="::22"$org_i""$serv_num""
			else	
				localIP="::11$i"
				remoteIP="::22$i"
			fi			
			PPP_PARAM="$PPP_PARAM ipv6 $localIP,$remoteIP "	
		else
			PPP_PARAM="$PPP_PARAM ipv6 , "
		fi
	elif [ "$IPVERSION" = "IPv6" ] ; then
		echo 0 > /proc/sys/net/ipv6/conf/nas$i/accept_ra 
		if [ "$CONNECTION" = "Connect_on_Demand" ] ; then
			if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
				localIP="::11"$org_i""$serv_num""
				remoteIP="::22"$org_i""$serv_num""
			else
				localIP="::11$i"
				remoteIP="::22$i"
			fi	
			PPP_PARAM="$PPP_PARAM ipv6 $localIP,$remoteIP noip"	
		else
			PPP_PARAM="$PPP_PARAM ipv6 , noip"
		fi		
	fi
	if [ -f /var/run/ppp$k.pid ] ; then
		kill -9 `cat /var/run/ppp$k.pid`
	fi

	if [ -n "$wan_pppoe_hostuniq" ] ; then
		PPP_PARAM="$PPP_PARAM host-uniq $wan_pppoe_hostuniq"
	fi
	PPP_PARAM="$PPP_PARAM $wan_pppoe_options"
	pppd $PPP_PARAM &
	echo $! > /var/run/ppp$k.pid
	echo "pppd $PPP_PARAM &" > /var/tmp/ppp$k.conf
	# if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		# if [ "$LASTmss" != "0" ] && [ "$LASTmss" != "" ] ; then
			# iptables -D FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
		# fi
	
		# if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
			# iptables -A FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
		# else
			# iptables -A FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
		# fi
	# else
		# if [ "$LASTmss" != "0" ] && [ "$LASTmss" != "" ] ; then
			# iptables -D FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
		# fi
	
		# if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
			# iptables -A FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
		# else
			# iptables -A FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
		# fi
	# fi	
	# if [ "$IPVERSION" != "IPv6" ] ; then
	# if [ "$NATENABLE" = "Enable" ] ; then
		# if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
			# iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
		# fi
		# if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
			# iptables -t nat -A POSTROUTING -o ppp"$org_i""$serv_num" -j MASQUERADE
		# else	
			# iptables -t nat -A POSTROUTING -o ppp$i -j MASQUERADE
		# fi	
		# iptables -t nat -A PREROUTING -j VS_PRE$i
		# iptables -t nat -A PREROUTING -j DMZ_PRE$i
		# if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
			# iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
		# fi
		# if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
			# if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
				# iptables -t nat -A PREROUTING -i ppp"$org_i""$serv_num" -j PREROUTING_WAN
				# iptables -t filter -A FORWARD -i ppp"$org_i""$serv_num" -j FORWARD_WAN
			# else	
				# iptables -t nat -A PREROUTING -i ppp$i -j PREROUTING_WAN
				# iptables -t filter -A FORWARD -i ppp$i -j FORWARD_WAN
			# fi	
		# fi
	# fi
	# fi
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		WAN_IF=ppp"$org_i""$serv_num"
	else
		WAN_IF=ppp$i
	fi

	#this case means the model including Ethernet WAN.
	if [ "$TCSUPPORT_MTK_INTERNAL_ETHER_SWITCH" = "" ]; then
		if [ "$i" = "10" ]; then
			rand1=`expr $RANDOM % 255`
			RANDOM=$$
			rand2=`expr $RANDOM % 255`
			randip="169.254.$rand1.$rand2"
			ifconfig nas$i $randip
			/userfs/bin/tcapi commit "IPTV" &
		fi
	fi

# feature added by rclv 20100536 for ipv6 options 

		
fi
#end of code added by rclv

elif [ $ISP = "3" ] ; then
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add bridge nas$org_i nas$i
	fi	
	if [ "$TCSUPPORT_CZ_GENERAL" != "" ] ;then
	if [ "$IPVERSION" != "IPv4" ] ; then
		ebtables -t filter -A OUTPUT -o nas$i -p IPv6 --ip6-proto 58 --ip6-icmpv6type 134 -j DROP
	fi
	fi

	if [ "$QOS" = "ubr" ] ; then
		PCR_V="-p $PCR"
	elif [ "$QOS" = "cbr" ] || [ "$QOS" = "rt-vbr" ] || [ "$QOS" = "nrt-vbr" ] ; then
		PCR_V="-p $PCR -q $SCR -m $MBS"
	fi
	
	if [ "$ENCAP" = "1483 Bridged Only LLC" ] ; then
		ENCAP_T="-e 0"
	elif [ "$ENCAP" = "1483 Bridged Only VC-Mux" ] ; then
		ENCAP_T="-e 1"
	fi
	
	if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		#ATM only
		if [ "$isPTMETHER" = "0" ] ; then
			br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
			echo $! > /var/run/nas$i.pid
			sleep 1
		fi
	else
		br2684ctl -c $i $ENCAP_T -t $QOS $PCR_V -a 0.$VPI.$VCI &
		echo $! > /var/run/nas$i.pid
		sleep 1
	fi

setCloneMAC

	# WAN_STATE_STOPPED.
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
		if [ "$isPTMETHER" = "1" ] ; then
			if [ "$serv_num" = "0" ]; then
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_state_t" "4" &
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_sbstate_t" "3" &
				/userfs/bin/tcapi set "Wanduck_Common" "wan"$org_i"_auxstate_t" "1" &
			fi
		else
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_state_t" "4" &
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_sbstate_t" "3" &
			/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_auxstate_t" "1" &
		fi
	else
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_state_t" "4" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_sbstate_t" "3" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$i"_auxstate_t" "1" &
	fi

	/sbin/ifconfig nas$i 0.0.0.0
	WAN_IF=nas$i
	LAN_PORT_ITF_PREFIX="elan."

	brctl addif $br_ifname $WAN_IF
	brctl delif br0 $LAN_PORT_ITF_PREFIX$bridge_lan_port
	brctl addif $br_ifname $LAN_PORT_ITF_PREFIX$bridge_lan_port

	if [ "$isIPTV" = "Yes" ]; then
		# restart IPTV
		/userfs/bin/tcapi commit "IPTV" &
	fi

	#These setting should not be used anymore.
	#if [ "$isIPTV" = "No" ]; then		
		#brctl addif br1 $WAN_IF
		#Arcadyan require bridge WAN port to all LAN port in the xDSL and Ethernet WAN bridge mode for their testing.
		#brctl addif br0 $WAN_IF
	#else
		#brctl addif br0 $WAN_IF
	#fi

	#ebtables -t filter -A INPUT -i nas$i -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP
	#ebtables -t filter -A INPUT -i nas$i -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP
fi
#jrchen
#	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
#		sleep 1
#		/sbin/ifconfig $WAN_IF inet6 $IPADDR6/$PREFIX6
#		sleep 1
#		route -A inet6 add default gw $DEFGATEWAY6 dev $WAN_IF &
#	fi

/usr/script/ether_mac.sh &

#if [ $ISP != "3" ] ; then
	#if ! [ "$ISP" = "2" -a "$CONNECTION" = "Connect_Manually" ] ; then
		#if [ "$BridgeInterface" = "Yes" ] ; then
			#/userfs/bin/pppoe-relay -C br0 -S nas$i &
		#fi
	#fi
#fi

#echo "======pppoe-relay start======="
if [ "$isIPTV" = "No" ]; then
/usr/script/start_pppoe_relay.sh $i &
fi
#echo "======pppoe-relay end======="

#/usr/bin/qoscmd dev add nas$i &
#FW_CONF=/etc/firewall.conf
#if [ -f $FW_CONF ]; then
#	chmod +x $FW_CONF
#	. $FW_CONF
#	if [ "$spi_status" = "1" ]; then
#        echo spi status
#		/usr/bin/iptables -A SPI_FW -i $WAN_IF -m state --state NEW,INVALID -j DROP
#        echo spi status2
#		/usr/bin/iptables -A SPI_FW -i $WAN_IF -p icmp --icmp-type echo-request -j DROP			
#	fi
#fi

#/usr/script/ipfilter_start.sh #wtw add
