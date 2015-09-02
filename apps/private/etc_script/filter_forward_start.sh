PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi
iptables -t filter -N ipfilter_chain
iptables -t filter -N app_filter_chain
iptables -t filter -N url_filter_chain
if [ "$TCSUPPORT_TIME_OF_DAY" != "" ] ;then
iptables -t filter -N parent_control_chain
fi
iptables -t filter -A FORWARD -j ipfilter_chain
iptables -t filter -A FORWARD -p TCP -m multiport --dport http -j url_filter_chain
iptables -t filter -A FORWARD -p TCP -j app_filter_chain
iptables -t filter -A FORWARD -p UDP -j app_filter_chain
if [ "$TCSUPPORT_TIME_OF_DAY" != "" ] ;then
iptables -t filter -A FORWARD -p TCP -j parent_control_chain
iptables -t filter -A FORWARD -p UDP -j parent_control_chain
fi
#krammer
iptables -t filter -N SPI_FW
iptables -t filter -N ACL
iptables -t filter -N FIREWALL
if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
iptables -t filter -N ADDRMAP_FORWARD
fi
if [ "$TCSUPPORT_DHCP_PORT_FLT" != "" ] ;then
iptables -t filter -N DHCP_PORT_FLT
fi
iptables -t filter -A INPUT -j SPI_FW
iptables -t filter -A INPUT -j ACL
if [ "$TCSUPPORT_DHCP_PORT_FLT" != "" ] ;then
iptables -t filter -A INPUT -j DHCP_PORT_FLT
fi
iptables -t filter -A INPUT -j FIREWALL
#create a new chain
if [ "$TCSUPPORT_PORT_TRIGGER" != "" ] ;then
iptables -t filter -N FORWARD_WAN
fi

if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
iptables -t filter -I FORWARD 1 -j ADDRMAP_FORWARD
if [ "$TCSUPPORT_WAN_ETHER" != "" ] ;then
	pvc_num="0 1 2 3 4 5 6 7 8 9 10"
elif [ "$TCSUPPORT_WAN_PTM" != "" ] ;then
	pvc_num="0 1 2 3 4 5 6 7 8 9"
else
	pvc_num="0 1 2 3 4 5 6 7"
fi

for j in $pvc_num
do
	iptables -t filter -N ADDRMAP_FORWARD$j
done					
fi

iptables -t nat -N PRE_SERVICE
iptables -t nat -A PREROUTING -j PRE_SERVICE
