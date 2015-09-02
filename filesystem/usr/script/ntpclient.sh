#!/bin/sh

/bin/kill -9 `/bin/pidof ntpclient`

if [ -f /etc/ntp.sh ]; then
	chmod 777 /etc/ntp.sh
	. /etc/ntp.sh
fi

