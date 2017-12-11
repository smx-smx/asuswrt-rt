#!/bin/sh
#To build 6rd tunnel
#Usage:./ipv6rd_start.sh 

if [ $# != 7 ] ; then
	echo "usage: $0 CE_address 6rd_prefix prefix_length 6rd_PD BR_address MTU TTL"
	exit 0
fi
CE_ADDR=$1
PREFIX=$2
PREFIX_LEN=$3
PD=$4
BR_ADDR=$5
MTU=$6
TTL=$7

TUNNEL_NAME="6rd"

ip -6 route flush dev $TUNNEL_NAME

ip link set dev $TUNNEL_NAME down

ip -6 tunnel del $TUNNEL_NAME

ip tunnel add $TUNNEL_NAME mode sit local $CE_ADDR ttl $TTL

ip tunnel 6rd dev $TUNNEL_NAME 6rd_prefix $PREFIX/$PREFIX_LEN

ip link set $TUNNEL_NAME mtu $MTU up

ip -6 addr add $PD/$PREFIX_LEN dev $TUNNEL_NAME

ip -6 route add ::/0 via ::$BR_ADDR dev $TUNNEL_NAME


exit 0
