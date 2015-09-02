#!/bin/sh
/sbin/iptables -t nat -I PREROUTING -p $1 -d $2 --dport $3 -j DNAT --to $4:$5
