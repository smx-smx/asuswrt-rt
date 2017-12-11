#!/bin/sh

while [ true ];
do
	echo "###############################"
        tc -s -d class show dev br0 parent 12:
        sleep 3
done

