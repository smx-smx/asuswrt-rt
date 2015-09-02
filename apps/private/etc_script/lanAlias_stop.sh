#!/bin/sh

if [ $# != 1 ] ; then
	echo "usage: $0 [Entryn]"
	exit 0
fi

i=$1
#/sbin/ifconfig br0:$i down
/sbin/ifconfig br0:$i down
