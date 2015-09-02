#!/bin/sh

#/usr/bin/iptables -N SPI_FW
#/usr/bin/iptables -I INPUT 1 -j SPI_FW
/usr/bin/iptables -I FORWARD 1 -j SPI_FW

PROFILE_CFG=/userfs/profile.cfg
if [ -f $PROFILE_CFG ] ; then
    . $PROFILE_CFG
fi

if [ "$TCSUPPORT_WAN_ETHER" != "" ] ;then
	pvc_num="0 1 2 3 4 5 6 7 8 9 10"
elif [ "$TCSUPPORT_WAN_PTM" != "" ] ;then
	pvc_num="0 1 2 3 4 5 6 7 8 9"
else
	pvc_num="0 1 2 3 4 5 6 7"
fi

if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
	EXTEND="0"
fi

for i in $pvc_num
do
	if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
		if [ "$i" = "8" ] || [ "$i" = "9" ] || [ "$i" = "10" ] ; then
			service_num="0 1 2 3 4 5 6 7"
		else
			service_num="0"
		fi
	else
		service_num="0"
	fi

	for j in $service_num
	do
		if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
			if [ "$i" = "8" ] || [ "$i" = "9" ] || [ "$i" = "10" ] ; then
				pppNum="$i""$j"
				nasNum="$i"_"$j"
				ispNum="$i"_"$j"
				EXTEND="1"
			else
				EXTEND="0"
			fi
		fi
		if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
			if [ "$EXTEND" = "1" ] ; then
				CONFFILE=/etc/isp$ispNum.conf
			else
				CONFFILE=/etc/isp$i.conf
			fi
		else
			CONFFILE=/etc/isp$i.conf
		fi

		if [ -f $CONFFILE ]; then
			chmod +x $CONFFILE
			. $CONFFILE
			if [ "$Active" = "Yes" ]; then 
				if [ "$ISP" = "2" ]; then
					if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
						if [ "$EXTEND" = "1" ] ; then
							WAN_IF=ppp$pppNum
						else
							WAN_IF=ppp$i
						fi
					else
						WAN_IF=ppp$i
					fi
				else
					if [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" ] ; then
						if [ "$EXTEND" = "1" ] ; then
							WAN_IF=nas$nasNum
						else
							WAN_IF=nas$i
						fi
					else
						WAN_IF=nas$i
					fi
				fi

	            		/usr/bin/iptables -A SPI_FW -i $WAN_IF -m state --state NEW,INVALID -j DROP
				/usr/bin/iptables -A SPI_FW -i $WAN_IF -p icmp --icmp-type echo-request -j DROP
			fi
		fi
	done	
done
	
