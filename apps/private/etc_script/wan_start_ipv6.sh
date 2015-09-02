#!/bin/sh

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi

if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
	if [ $# -gt 3 ] ; then
		echo "usage: $0 [PVCn] [UniqueMac Flag] [Unique Mac]"
		exit 0
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
if [ "$i" = "8" ] || [ "$i" = "9" ] || [ "$i" = "10" ] ; then
	isPTMETHER=1
else
	isPTMETHER=0
fi	
if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
	if [ "$isPTMETHER" = "1" ] ; then
		org_i=$i
		serv_num=$2
		i="$i"_"$serv_num"
		k="$org_i""$serv_num"
	fi	
	ifconfig nas$org_i up
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

MAC_CONFFILE=/etc/mac.conf
if [ -f $MAC_CONFFILE ]; then
	chmod 777 $MAC_CONFFILE
	. $MAC_CONFFILE
fi
		
if [ "$Active" != "Yes" ] ; then
	exit 0
fi

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

if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
	if [ "$UNIQUEMAC_FLAG" = "1" ]; then
		/sbin/ifconfig nas$i hw ether $UNIQUE_MAC
	else	
		if [ "$WAN_MAC" != "" ]; then
		/sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi
else
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" = "" ] || [ "$TCSUPPORT_WAN_PTM" = "" -a "$TCSUPPORT_WAN_ETHER" = "" ] || [ "$isPTMETHER" != "1" ] ;then
		if [ "$WAN_MAC" != "" ]; then
			 /sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi	
fi
	if [ "$IPVERSION" != "IPv4" ] && [ "$EnableDynIPv6" = "0" ]; then
		echo 1 > /proc/sys/net/ipv6/conf/nas$i/autoconf
		if [ "$TCSUPPORT_IPV6_PRIVACYADDRS" != "" ] ;then
			echo $IPv6Extension > /proc/sys/net/ipv6/conf/nas$i/use_tempaddr
		fi
	fi
	/sbin/ifconfig nas$i 0.0.0.0
	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig nas$i mtu 1500
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig nas$i mtu $MTU
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	if [ "$IPVERSION" != "IPv6" ] ; then
	if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
		if [ "$UNIQUEMAC_FLAG" = "1" ]; then
			if [ "$DEFAULTROUTE" = "Yes" ] ; then
				/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -m $UNIQUE_MAC &
			else
				/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -m $UNIQUE_MAC &
			fi
		else	
			if [ "$WAN_MAC" != "" ]; then
				if [ "$DEFAULTROUTE" = "Yes" ] ; then
					/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC &
				else	
					/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC &
				fi
			fi
		fi
	else
		if [ "$TCSUPPORT_WPA_SUPPLICANT" = "" ] || [ "$IPOE_DOT1X_STATUS" != "Enable" ];then
			if [ "$WAN_MAC" != "" ]; then
				if [ "$DEFAULTROUTE" = "Yes" ] ; then
					/sbin/udhcpc -i nas$i -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC &
				else	
					/sbin/udhcpc -i nas$i -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas$i.pid -m $WAN_MAC &
				fi
			fi
		fi	
	fi

	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
	if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
		iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
	fi	
		iptables -t nat -A POSTROUTING -o nas$i -j MASQUERADE
		iptables -t nat -A PREROUTING -j VS_PRE$i
		iptables -t nat -A PREROUTING -j DMZ_PRE$i
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
		iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
	fi
if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
		iptables -t nat -A PREROUTING -i nas$i -j PREROUTING_WAN
		iptables -t filter -A FORWARD -i nas$i -j FORWARD_WAN
fi
	fi
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

if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
	if [ "$UNIQUEMAC_FLAG" = "1" ]; then
		/sbin/ifconfig nas$i hw ether $UNIQUE_MAC
	else	
		if [ "$WAN_MAC" != "" ]; then
		/sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi
else
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" = "" ] || [ "$TCSUPPORT_WAN_PTM" = "" -a "$TCSUPPORT_WAN_ETHER" = "" ] || [ "$isPTMETHER" != "1" ] ;then
		if [ "$WAN_MAC" != "" ]; then
			 /sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi	
fi
#	/sbin/ifconfig nas$i $IPADDR netmask $NETMASK up
#	/sbin/ifconfig nas$i $IPADDR6/$PREFIX6
#	echo -e "server=$DNSIPv61st@nas$i\\nserver=$DNSIPv62nd@nas$i">>/etc/dnsmasq.conf

	
#	echo -e "nameserver $DNSIPv61st\\nnameserver $DNSIPv62nd">/var/run/dhcp6c-nas$i.info

	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig nas$i mtu 1500
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig nas$i mtu $MTU
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	/sbin/ifconfig nas$i up
	if [ "$IPVERSION" != "IPv6" ] ; then	
		/sbin/ifconfig nas$i $IPADDR netmask $NETMASK
	fi
	if [ "$IPVERSION" != "IPv4" ] ; then
		/sbin/ifconfig nas$i $IPADDR6/$PREFIX6
		echo -e "nameserver $DNSIPv61st\\nnameserver $DNSIPv62nd">/var/run/dhcp6c-nas$i.info
	fi

	if [ "$DEFAULTROUTE" = "Yes" ] ; then
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
	if [ "$IPVERSION" != "IPv6" ] ; then
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
	if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
		iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
	fi
		iptables -t nat -A POSTROUTING -o nas$i -j MASQUERADE
		iptables -t nat -A PREROUTING -j VS_PRE$i
		iptables -t nat -A PREROUTING -j DMZ_PRE$i
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
		iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
	fi
if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
		iptables -t nat -A PREROUTING -i nas$i -j PREROUTING_WAN
		iptables -t filter -A FORWARD -i nas$i -j FORWARD_WAN
fi
	fi
	fi
	WAN_IF=nas$i
elif [ $ISP = "2" ] ; then
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add pppoe nas$org_i nas$i
	fi	
	if [ "$CONNECTION" != "Connect_Manually" ] ; then
  		#make sure the LCP echo will take no effect about ppp
		if [ "$TCSUPPORT_CZ_GENERAL" != "" ] ;then
			PPP_PARAM="unit $i user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 10 lcp-echo-failure 60"
		else
			if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
        		PPP_PARAM="unit "$org_i""$serv_num" user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 1 lcp-echo-failure 60"
			else
        		PPP_PARAM="unit $i user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 1 lcp-echo-failure 60"
			fi
	fi 
	if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		if [ "$isPTMETHER" = "1" ] ; then
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

	if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		if [ "$isPTMETHER" = "1" ] ; then
			if [ "$AUTHEN" = "CHAP" ] ; then
				PPP_PARAM="$PPP_PARAM -pap"
			elif [ "$AUTHEN" = "PAP" ] ; then
				PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
			fi
if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
	if [ "$UNIQUEMAC_FLAG" = "1" ]; then
		/sbin/ifconfig nas$i hw ether $UNIQUE_MAC
	else	
		if [ "$WAN_MAC" != "" ]; then
		/sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi
else
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" = "" ] || [ "$TCSUPPORT_WAN_PTM" = "" -a "$TCSUPPORT_WAN_ETHER" = "" ] || [ "$isPTMETHER" != "1" ] ;then
		if [ "$WAN_MAC" != "" ]; then
			 /sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi	
fi
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
if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
	if [ "$UNIQUEMAC_FLAG" = "1" ]; then
		/sbin/ifconfig nas$i hw ether $UNIQUE_MAC
	else	
		if [ "$WAN_MAC" != "" ]; then
		/sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi
else
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" = "" ] || [ "$TCSUPPORT_WAN_PTM" = "" -a "$TCSUPPORT_WAN_ETHER" = "" ] || [ "$isPTMETHER" != "1" ] ;then
		if [ "$WAN_MAC" != "" ]; then
			 /sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi	
fi
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

	pppd $PPP_PARAM &
	echo $! > /var/run/ppp$k.pid
	echo "pppd $PPP_PARAM &" > /var/tmp/ppp$k.conf
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		if [ "$LASTmss" != "0" ] && [ "$LASTmss" != "" ] ; then
			iptables -D FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
		fi
	
		if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
			iptables -A FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
		else
			iptables -A FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
		fi
	else
		if [ "$LASTmss" != "0" ] && [ "$LASTmss" != "" ] ; then
			iptables -D FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
		fi
	
		if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
			iptables -A FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
		else
			iptables -A FORWARD -o ppp$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
		fi
	fi	
	if [ "$IPVERSION" != "IPv6" ] ; then
	if [ "$NATENABLE" = "Enable" ] ; then
		if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
			iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
		fi
		if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
			iptables -t nat -A POSTROUTING -o ppp"$org_i""$serv_num" -j MASQUERADE
		else	
			iptables -t nat -A POSTROUTING -o ppp$i -j MASQUERADE
		fi	
		iptables -t nat -A PREROUTING -j VS_PRE$i
		iptables -t nat -A PREROUTING -j DMZ_PRE$i
		if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
			iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
		fi
		if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
			if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
				iptables -t nat -A PREROUTING -i ppp"$org_i""$serv_num" -j PREROUTING_WAN
				iptables -t filter -A FORWARD -i ppp"$org_i""$serv_num" -j FORWARD_WAN
			else	
				iptables -t nat -A PREROUTING -i ppp$i -j PREROUTING_WAN
				iptables -t filter -A FORWARD -i ppp$i -j FORWARD_WAN
			fi	
		fi
	fi
	fi
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		WAN_IF=ppp"$org_i""$serv_num"
	else
		WAN_IF=ppp$i
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

	
if [ "$TCSUPPORT_UNIQUEMAC" != "" ] ;then
	if [ "$UNIQUEMAC_FLAG" = "1" ]; then
		if [ "$LAN_MAC" != "" ]; then
		/sbin/ifconfig nas$i hw ether $LAN_MAC
		fi
	else	
		if [ "$WAN_MAC" != "" ]; then
		/sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi
else
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" = "" ] || [ "$TCSUPPORT_WAN_PTM" = "" -a "$TCSUPPORT_WAN_ETHER" = "" ] || [ "$isPTMETHER" != "1" ] ;then
		if [ "$WAN_MAC" != "" ]; then
			 /sbin/ifconfig nas$i hw ether $WAN_MAC
		fi	
	fi	
fi

	/sbin/ifconfig nas$i 0.0.0.0
	brctl addif br1 nas$i
	WAN_IF=nas$i
#	ebtables -t filter -A INPUT -i nas$i -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP
#	ebtables -t filter -A INPUT -i nas$i -p IPv6 --ip6-proto 17 --ip6-dport 547 -j DROP
fi
#jrchen
#	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
#		sleep 1
#		/sbin/ifconfig $WAN_IF inet6 $IPADDR6/$PREFIX6
#		sleep 1
#		route -A inet6 add default gw $DEFGATEWAY6 dev $WAN_IF &
#	fi

/usr/script/ether_mac.sh &

if [ $ISP != "3" ] ; then
	if ! [ "$ISP" = "2" -a "$CONNECTION" = "Connect_Manually" ] ; then
		if [ "$BridgeInterface" = "Yes" ] ; then
			/userfs/bin/pppoe-relay -C br0 -S nas$i &
		fi
	fi
fi
/usr/bin/qoscmd dev add nas$i &
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
