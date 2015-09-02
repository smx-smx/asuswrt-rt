#!/bin/sh
#To build dslite tunnel
#Usage:./dslite_start.sh local_wan_ipv6_address tunnel_ipv6_address

if [ $# != 3 ] ; then
	echo "usage: $0 local_wan_ipv6_address remote_tunnel_ipv6_address wan_if_name"
	exit 0
fi
LOCAL_ADDR=$1
REMOTE_ADDR=$2
WAN_IF=$3
#Warning:remeber to modify the name of dslite in wan_stop.sh synchronousl
DSLITE_NAME="dslite"

ip -6 tunnel del $DSLITE_NAME

ip -6 tunnel add $DSLITE_NAME mode ipip6 local $LOCAL_ADDR remote $REMOTE_ADDR
ip link set up $DSLITE_NAME

ip route del default
ip route add default dev $DSLITE_NAME

echo 1 > /var/run/dslite_on_$WAN_IF

exit 0
