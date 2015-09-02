PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi
iptables -t filter -X ipfilter_chain
iptables -t filter -X app_filter_chain
iptables -t filter -X url_filter_chain
#krammer
iptables -t filter -X SPI_FW
iptables -t filter -X ACL
iptables -t filter -X FIREWALL
if [ "$TCSUPPORT_MULTI_NAT" != "" ] ;then
iptables -t filter -X ADDRMAP_FORWARD
fi

iptables -t nat -F PRE_SERVICE
iptables -t nat -D PREROUTING -j PRE_SERVICE
iptables -t nat -X PRE_SERVICE
if [ "$TCSUPPORT_DHCP_PORT_FLT" != "" ] ;then
iptables -t filter -F DHCP_PORT_FLT
iptables -t filter -D INPUT -j DHCP_PORT_FLT
iptables -t filter -X DHCP_PORT_FLT
fi
