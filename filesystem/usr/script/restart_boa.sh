#!/bin/sh

sleep $1
killall -9 boa
/userfs/bin/boa -c /boaroot/ -d &
