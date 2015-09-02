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
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
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
		if [ "$TCSUPPORT_WPA_SUPPLICANT" = "" ] && [ "$802_1X_STATUS" != "Enable" ];then
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
	WAN_IF=nas$i
elif [ $ISP = "1" ] ; then
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add ipoe nas$org_i nas$i
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
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
	fi
fi
	/sbin/ifconfig nas$i $IPADDR netmask $NETMASK up
	if [ "$MTU" = "0" ] || [ "$MTU" = "" ] ; then
		/sbin/ifconfig nas$i mtu 1500
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		/sbin/ifconfig nas$i mtu $MTU
		iptables -A FORWARD -o nas$i -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	fi
	if [ "$DEFAULTROUTE" = "Yes" ] ; then
		route add default gw $GATEWAY dev nas$i
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
	PPP_PARAM="unit $k user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 1 lcp-echo-failure 60"
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
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
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
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
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

	pppd $PPP_PARAM &
	echo $! > /var/run/ppp$k.pid
	echo "pppd $PPP_PARAM &" > /var/tmp/ppp$k.conf
	#	if [ "$LASTmss" != "0" ] ; then
	if [ "$LASTmss" != "0" ] && [ "$LASTmss" != "" ] ; then
		iptables -D FORWARD -o ppp$k -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
	fi
	
	if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
		iptables -A FORWARD -o ppp$k -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
	else
		iptables -A FORWARD -o ppp$k -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
	fi
	if [ "$NATENABLE" = "Enable" ] ; then
#		iptables -t nat -A POSTROUTING -j ADDRMAP_POS$i
	if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then	
		iptables -t nat -A ADDRMAP_POS -j ADDRMAP_POS$i
	fi	
		iptables -t nat -A POSTROUTING -o ppp$k -j MASQUERADE
		iptables -t nat -A PREROUTING -j VS_PRE$i
		iptables -t nat -A PREROUTING -j DMZ_PRE$i
#		iptables -t nat -A PREROUTING -j ADDRMAP_PRE$i
	if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
		iptables -t filter -A ADDRMAP_FORWARD -j ADDRMAP_FORWARD$i	
	fi
if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
		iptables -t nat -A PREROUTING -i ppp$k -j PREROUTING_WAN
		iptables -t filter -A FORWARD -i ppp$k -j FORWARD_WAN
fi
	fi

		WAN_IF=ppp$k
fi
elif [ $ISP = "3" ] ; then
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add bridge nas$org_i nas$i
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
	if [ "$WAN_MAC" != "" ]; then
		 /sbin/ifconfig nas$i hw ether $WAN_MAC
	fi
fi

	/sbin/ifconfig nas$i 0.0.0.0
	brctl addif br1 nas$i
	WAN_IF=nas$i
#	ebtables -t filter -A INPUT -i nas$i -p IPv4 --ip-proto 17 --ip-dport 67 -j DROP
fi

/usr/script/ether_mac.sh

if [ $ISP != "3" ] ; then
	if ! [ "$ISP" = "2" -a "$CONNECTION" = "Connect_Manually" ] ; then
		if [ "$BridgeInterface" = "Yes" ] ; then
			/userfs/bin/pppoe-relay -C br0 -S nas$i &
		fi
	fi
fi
/usr/bin/qoscmd dev add nas$i
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
