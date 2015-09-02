#!/bin/sh

CONFFILE0="/etc/ipfilter/ipfilter_url.conf"

if [ -f $CONFFILE0 ] ; then
	. $CONFFILE0
else
	exit 0
fi

if [  "$urlfilter_active" = "1" ] ; then
	if [  "$urlfilter_index" = "1" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url0? -j DROP
	elif [  "$urlfilter_index" = "2" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url1? -j DROP
	elif [  "$urlfilter_index" = "3" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url2? -j DROP
	elif [  "$urlfilter_index" = "4" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url3? -j DROP
	elif [  "$urlfilter_index" = "5" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url4? -j DROP
	elif [  "$urlfilter_index" = "6" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url5? -j DROP
	elif [  "$urlfilter_index" = "7" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url6? -j DROP
	elif [  "$urlfilter_index" = "8" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url7? -j DROP
	elif [  "$urlfilter_index" = "9" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url8? -j DROP
	elif [  "$urlfilter_index" = "10" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url9? -j DROP
	elif [  "$urlfilter_index" = "11" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url10? -j DROP
	elif [  "$urlfilter_index" = "12" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url11? -j DROP
	elif [  "$urlfilter_index" = "13" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url12? -j DROP
	elif [  "$urlfilter_index" = "14" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url13? -j DROP
	elif [  "$urlfilter_index" = "15" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url14? -j DROP
	elif [  "$urlfilter_index" = "16" ] ; then
		echo "Current version does not support L7 filter" #wtw add
		# wtw add ifl7 iptables -t mangle -A URL_FLT -m layer7
?$urlfilter_url15? -j DROP
	fi
elif [  "$urlfilter_active" = "0" ] ; then
	exit 0
fi