#!/bin/sh
# ASUS check if spectrum daemon is running

if [ -z "$(pidof spectrum)" ];
then
	#echo "spectrum is not running"
	/userfs/bin/spectrum &
fi
