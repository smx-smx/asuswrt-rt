#!/bin/sh
cmd="$1";
[ -z "$cmd" ] && cmd="start"

dev_wan=eth0
dev_lan=br0
qos_wan=eth0
qos_lan=br0
sess_num=30000
user_timeout=`expr 60 \* 60`
app_timeout=`expr 60 \* 60`

udb_param="dev_wan=$dev_wan"
udb_param="$udb_param dev_lan=$dev_lan"
udb_param="$udb_param qos_wan=$qos_wan"
udb_param="$udb_param qos_lan=$qos_lan"
udb_param="$udb_param sess_num=$sess_num"
udb_param="$udb_param user_timeout=$user_timeout"
udb_param="$udb_param app_timeout=$app_timeout"

idp_mod=tdts.ko
udb_mod=tdts_udb.ko
fw_mod=tdts_udbfw.ko
rule=rule.trf
agent=tdts_rule_agent

dev=/dev/detector
dev_maj=190
dev_min=0

fwdev=/dev/idpfw
fwdev_maj=191
fwdev_min=0

wred_setup=wred-setup.sh
iqos_setup=iqos-setup.sh

case "$cmd" in
start)
	echo "in `pwd`"

	if [ ! -f "$rule" ]; then
		echo "Signature file $rule not found"
		exit 1
	fi

	# create dev node
	echo "Creating device nodes..."
	[ ! -c "$dev" ] && mknod $dev c $dev_maj $dev_min
	[ ! -c "$fwdev" ] && mknod $fwdev c $fwdev_maj $fwdev_min
	test -c $dev || echo "...Creat $dev failed"
	test -c $fwdev || echo "...Create $fwdev failed"

	echo "Filter WAN bootp packets..."
	chain=BWDPI_FILTER
	iptables -t mangle -N $chain
	iptables -t mangle -F $chain
	iptables -t mangle -A $chain -i $dev_wan -p udp --sport 68 --dport 67 -j DROP
	iptables -t mangle -A $chain -i $dev_wan -p udp --sport 67 --dport 68 -j DROP
	iptables -t mangle -A PREROUTING -i $dev_wan -p udp -j $chain

	echo "Insert IDP engine..."
	insmod ./$idp_mod || exit -1

	echo "Insert UDB ($udb_param)..."
	insmod ./$udb_mod $udb_param || exit 1

	echo "Insert forward module..."
	insmod ./$fw_mod || exit 1

	# start tcd
	./tcd &

	# start iqos
	if [ -x ./$iqos_setup ]; then
		./$iqos_setup restart
	fi

	# start dcd
	if [ -x ./dc_monitor.sh ]; then
		./dc_monitor.sh &
	fi
		
	# start wred
	if [ -x ./$wred_setup ]; then
		./$wred_setup &
	fi

	# start Web patrol
	# LD_LIBRARY_PATH=. ./wred_set_conf -f wred.conf

	# start App patrol
	#./shn_ctrl -a set_app_patrol -R ./app_patrol.conf

	# start TQ patrol
	#./shn_ctrl -a set_patrol_tq -R ./patrol_tq.conf

	echo "Running rule agent to setup signature file $rule..."
	./$agent -g

	# clean cache
	if [ -x ./clean-cache.sh ]; then
		echo "Running clean-cache.sh..."
		./clean-cache.sh > /dev/null 2>&1 &
	fi

	# agree eula
	./shn_ctrl -a set_eula_agreed
	
	# set function mode
	# APP|DEV|VP|WRS_WEB|WRS_SEC|WRS_CC|ANO|QOS|PATROL|TQ  => all fun bits
	# APP|DEV|VP|   X   |WRS_SEC|WRS_CC|ANO|QOS|   X  | X  => begining test mode
	echo f7 > /proc/bw_dpi_conf

	;;
stop)
	# stop clean cache
	killall -9 clean-cache.sh

	# stop wred
	killall -9 wred-setup.sh
	killall -9 wred

	# stop dcd
	killall -9 dc_monitor.sh
	killall -9 dcd

	# stop iqos
	if [ -x ./$iqos_setup ]; then
		./$iqos_setup stop
		sleep 3
	fi

	# stop tcd
	killall -9 tcd

	echo "Unload engine..."
	rmmod $fw_mod > /dev/null 2>&1
	rmmod $udb_mod > /dev/null 2>&1
	rmmod $idp_mod > /dev/null 2>&1

	echo "Remove device nodes..."
	[ -c "$dev" ] && rm -f $dev 
	[ ! -c "$dev" ] || echo "...Remove $dev failed"
	[ -c "$fwdev" ] && rm -f $fwdev
	[ ! -c "$fwdev" ] || echo "...Remove $fwdev failed"

	;;
restart)
	$0 stop
	sleep 2
	$0 start
	;;
esac;

