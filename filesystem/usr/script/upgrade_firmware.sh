#!/bin/sh

/usr/bin/killall -9 snmpd

for i in 0 1 2 3 4 5 6 7
do
        /usr/script/wan_stop.sh $i
done

wan adsl close
/usr/bin/killall -SIGUSR1 cc

