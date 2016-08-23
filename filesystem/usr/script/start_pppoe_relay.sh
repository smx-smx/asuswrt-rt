#!/bin/sh

#echo "======get pppoe-relay start======="
wan_primary=`/userfs/bin/tcapi get Wanduck_Common wan_primary`
wan_unit=`expr substr $1 1 2`
if [ "`expr index $wan_unit _`" != "0" ]; then
wan_unit=`expr substr $wan_unit 1 1`
fi

#echo "getpppoe_relay=${getpppoe_relay}"
if [ "$wan_unit" = "$wan_primary" ]; then
	service "restart_pppoe_relay $wan_unit"
fi
#echo "======get pppoe-relay end======="
