#!/bin/sh

SERVER_DYNDNS=WWW.DYNDNS.ORG
SERVER_DYNDNS_CUSTOM=WWW.DYNDNS.ORG_CUSTOM
SERVER_DYNDNS_STATIC=WWW.DYNDNS.ORG_STATIC
SERVER_TZO=WWW.TZO.COM
SERVER_ZONE=WWW.ZONEEDIT.COM
SERVER_JUSTLINUX=WWW.JUSTLINUX.COM
SERVER_EASYDNS=WWW.EASYDNS.COM
SERVER_DNSOMATIC=WWW.DNSOMATIC.COM
SERVER_TUNNELBROKER=WWW.TUNNELBROKER.NET
SERVER_NOIP=WWW.NO-IP.COM
SERVER_ASUS=WWW.ASUS.COM
SERVER_SELFHOST=WWW.SELFHOST.DE
SERVER_DOMAINS_GOOGLE=DOMAINS.GOOGLE.COM

IPUPDATE_CONF=/etc/ipupdate.conf
DDNS_CONF=/etc/ddns.conf
IPUPDATE_PID=/var/run/ez-ipupdate.pid

	if [ ! -f $DDNS_CONF ] ; then
		echo "error:$DDNS_CONF"
		exit 1
	fi

	rm -f $IPUPDATE_CONF
	chmod 755 $DDNS_CONF	
	. $DDNS_CONF
	if [ "$Active" != "1" ] ; then
		exit 0
	fi

	echo "host=$MYHOST" >>$IPUPDATE_CONF
#	echo "cache-file=/tmp/ez-ipupdate.cache" >>$IPUPDATE_CONF
	
	if [ "$SERVERNAME" != "$SERVER_ASUS" ]; then
		echo "user=$USERNAME:$PASSWORD" >>$IPUPDATE_CONF
		if [	"$WILDCARD" = "1" ] ; then
			echo "wildcard" >>$IPUPDATE_CONF
		fi
	fi

	if [  "$SERVERNAME" = "$SERVER_DYNDNS" ] ;  then
		echo "service-type=dyndns" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_DYNDNS_CUSTOM" ] ;  then
		echo "service-type=dyndns-custom" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_DYNDNS_STATIC" ] ;  then
		echo "service-type=dyndns-static" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_TZO" ] ; then
		echo "service-type=tzo" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_ZONE" ] ; then
		echo "service-type=zoneedit" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_JUSTLINUX" ] ; then
		echo "service-type=justlinux" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_EASYDNS" ] ; then
		echo "service-type=easydns" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [ "$SERVERNAME" = "$SERVER_DNSOMATIC" ] ; then
	  	echo "service-type=dnsomatic" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_TUNNELBROKER" ] ; then
		echo "service-type=heipv6tb" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_NOIP" ] ; then
		echo "service-type=noip" >>$IPUPDATE_CONF
#		echo "interface=eth0" >>$IPUPDATE_CONF
		echo "max-interval=2073600" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_ASUS" ] ; then
		echo "service-type=dyndns" >>$IPUPDATE_CONF
	elif [  "$SERVERNAME" = "$SERVER_SELFHOST" ] ; then
		echo "service-type=selfhost" >>$IPUPDATE_CONF	
	else
		echo "error:$SERVERNAME"
		exit 1
	fi
	
#	echo kill run script
#	if [ -e $IPUPDATE_PID ]; then
#		kill -9 `cat $IPUPDATE_PID`
#	fi
	
#/etc/ddns_run.sh
