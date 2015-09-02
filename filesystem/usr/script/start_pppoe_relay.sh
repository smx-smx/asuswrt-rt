#!/bin/sh

#echo "======get pppoe-relay start======="
getpppoe_relay=`/userfs/bin/tcapi get Firewall_Entry fw_pt_pppoerelay`
#echo "getpppoe_relay=${getpppoe_relay}"
if [ "$getpppoe_relay" = "1" ] ; then
	killall pppoe-relay
	/userfs/bin/pppoe-relay -C br0 -S nas$1 &
fi
#echo "======get pppoe-relay end======="
