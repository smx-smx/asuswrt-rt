#!/bin/sh
while [ 1 ]
do
	echo 1 > /proc/sys/vm/drop_caches
	sleep 60 
done
