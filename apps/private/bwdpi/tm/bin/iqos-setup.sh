#!/bin/sh

cmd=$1
iqos_conf=qos.conf

cd $(dirname $0)
shn_ctrl=$(pwd)/shn_ctrl
cd -
iqos_setup=$(basename $0)

if [ -z "$shn_ctrl" -o  -z "$shn_ctrl" ]; then
	echo "wrong cmd path"
	exit 1
fi

if [ -z "$cmd" ]; then
	echo "$0 start|stop|restart"
	exit 1
fi

case "$cmd" in 
start)
	echo "Start iQoS..."
	if [ "$(ps | grep tcd | grep -v grep)" == "" ]; then
		./tcd &
	fi
	LD_LIBRARY_PATH=$(pwd) $shn_ctrl -a set_qos_on
	;;
stop)
	echo "Stop iQoS..."
	LD_LIBRARY_PATH=$(pwd) $shn_ctrl -a set_qos_off
	;;
restart)
	$0 stop
	LD_LIBRARY_PATH=$(pwd) $shn_ctrl -a set_qos_conf -R $iqos_conf
	$0 start
	;;
*)
	echo "$0 start|stop|restart"
	;;
esac
