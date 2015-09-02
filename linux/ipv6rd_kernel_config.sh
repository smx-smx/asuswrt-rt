#!/bin/bash

if [ $# != 2 ] ; then
	echo "usage: $0 [kernel_config_path] [temp_config_path]"
	exit 0
fi

KERNEL_CONFIG=$1
NEW_KERNEL_CONFIG=$2

IS_IPV6RD_EXIST=$(grep "CONFIG_IPV6_SIT_6RD=y" $KERNEL_CONFIG)
if ["$IS_IPV6RD_EXIST" == ""];then
echo "Add kernel config for 6rd"
	SEDCMD="$SEDCMD -e 's/# CONFIG_IPV6_SIT_6RD is not set/CONFIG_IPV6_SIT_6RD=y/'"

	echo "sed $SEDCMD"
	gen="sed $SEDCMD $KERNEL_CONFIG"
	eval $gen > $NEW_KERNEL_CONFIG
	#sed "$SEDCMD" $KERNEL_CONFIG > $NEW_KERNEL_CONFIG

else
	echo "kernel config for 6RD has exist"
fi


