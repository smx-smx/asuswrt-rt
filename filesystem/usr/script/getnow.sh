#!/bin/sh

CONFFILE=/etc/timezone.conf

if [ -f $CONFFILE ] ; then
. $CONFFILE
export TZ
fi

date