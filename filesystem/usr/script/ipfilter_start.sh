#!/bin/sh

for i in 1 2 3 4 5 6 7 8 9 10 11 12
do
	for j in 1 2 3 4 5 6
	do
        if [ -f "/etc/ipfilter/ipfilter_$i""_$j.conf" ] ; then
            /usr/script/ipfilter.sh add $i $j
		fi
	done
done
