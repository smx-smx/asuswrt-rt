#!/bin/sh

DEVICECFG_PATH=$1
DMSCFG_PATH=$2

mount -t vfat /dev/sda1 /userfs/dlna/media/

export LD_LIBRARY_PATH=/userfs/dlna/Thirdp/lib:/userfs/dlna/lib:$LD_LIBRARY_PATH
cd /userfs/dlna/bin/
./dlna_dmsTcApp $DEVICECFG_PATH $DMSCFG_PATH &
echo $! > /var/run/dms.pid

exit 0
