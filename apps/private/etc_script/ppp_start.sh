#!/bin/sh

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi
if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] ;then
	if [ $# != 2 ] ; then
		echo "usage: $0 [PVCn] [Service_Num]"
		exit 0
   fi
else
	if [ $# != 1 ] ; then
		echo "usage: $0 [PVCn]"
		exit 0
	fi
fi

i=$1
k=$1
if [ "$i" = "8" ] || [ "$i" = "9" ] || [ "$i" = "10" ] || [ "$i" = "12" ]; then
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

if [ $ISP != "2" ] ; then
	exit 0
fi
 
if [ "$CONNECTION" != "Connect_Manually" ] ; then
	exit 0
fi

# power up Ethernet Wan
if [ "$TCSUPPORT_MTK_INTERNAL_ETHER_SWITCH" = "" -a "$i" = "10" ]; then
if [ "$TCSUPPORT_MT7530_EXTERNAL" = ""]; then
	rtkethcmd up wan
fi
fi

	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		/usr/bin/smuxctl add pppoe nas$org_i nas$i
	fi

  #make sure the LCP echo will take no effect about ppp
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		PPP_PARAM="unit "$org_i""$serv_num" user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 6 lcp-echo-failure 10"
	else
		PPP_PARAM="unit $i user $USERNAME password $PASSWORD nodetach holdoff 4 maxfail 0 usepeerdns lcp-echo-interval 6 lcp-echo-failure 10"
	fi

	PPP_PARAM="$PPP_PARAM novj"

	if [ "$TCSUPPORT_WAN_ATM" != "" -o "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ]; then
		if [ "$isPTMETHER" = "1" ]; then
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
			if [ "$AUTHEN" = "PAP" ] ; then
				PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
			elif [ "$AUTHEN" = "CHAP" ] ; then
				PPP_PARAM="$PPP_PARAM -pap"
			fi

			if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" = "" ] || [ "$TCSUPPORT_WAN_PTM" = "" -a "$TCSUPPORT_WAN_ETHER" = "" ]|| [ "$isPTMETHER" != "1" ] ; then
				if [ "$WAN_MAC" != "" ]; then
		 			/sbin/ifconfig nas$i hw ether $WAN_MAC
				fi
			fi	
			/sbin/ifconfig nas$i 0.0.0.0
			ENCAP=""
		fi
	fi

    # PPPoE
    if [ "$ENCAP" = "PPPoE LLC" ] || [ "$ENCAP" = "PPPoE VC-Mux" ] ; then
	if [ "$AUTHEN" = "PAP" ] ; then
		PPP_PARAM="$PPP_PARAM -chap -mschap -mschap-v2"
	elif [ "$AUTHEN" = "CHAP" ] ; then
		PPP_PARAM="$PPP_PARAM -pap"
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
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" = "" ] || [ "$TCSUPPORT_WAN_PTM" = "" -a "$TCSUPPORT_WAN_ETHER" = "" ] || [ "$isPTMETHER" != "1" ] ; then
		if [ "$WAN_MAC" != "" ]; then
			 /sbin/ifconfig nas$i hw ether $WAN_MAC
		fi
	fi	
	/sbin/ifconfig nas$i 0.0.0.0
    # PPPoA
    else
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
	if [ "$MTU" != "0" ] && [ "$MTU" != "" ] ; then
		PPP_PARAM="$PPP_PARAM mtu $MTU mru $MTU"
	fi
	if [ "$IPVERSION" = "IPv4/IPv6" ] ; then
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
		if [ "$LASTmss" != "0" ] || [ "$LASTmss" != "" ]; then
			iptables -D FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $LASTmss
		fi
	
		if [ "$MSS" = "0" ] || [ "$MSS" = "" ] ; then
			iptables -A FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu
		else
			iptables -A FORWARD -o ppp"$org_i""$serv_num" -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss $MSS
		fi
	else
		if [ "$LASTmss" != "0" ] || [ "$LASTmss" != "" ]; then
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
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] && [ "$TCSUPPORT_WAN_PTM" != "" -o "$TCSUPPORT_WAN_ETHER" != "" ] && [ "$isPTMETHER" = "1" ]; then
		if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
			iptables -t nat -A PREROUTING -i ppp"$org_i""$serv_num" -j PREROUTING_WAN
			iptables -t filter -A FORWARD -i ppp"$org_i""$serv_num" -j FORWARD_WAN
		fi
	else
		if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
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

/usr/script/ether_mac.sh
if [ $BridgeInterface = "Yes" ] ; then
	/userfs/bin/pppoe-relay -C br0 -S nas$i &
fi
