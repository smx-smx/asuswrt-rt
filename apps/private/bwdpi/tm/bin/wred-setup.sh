#!/bin/sh

PID_FILE=wred.pid
MON_INTL=5

run_wre()
{
  cmd="$1"
  [ -z "$cmd" ] && cmd="start"

  case "$cmd" in
    start)
        echo "Start web reputation engine daemon..."
        LD_LIBRARY_PATH=. ./wred &
        ;;
    stop)
        echo "Stop web reputation engine daemon..."
        killall -INT wred
        ;;
  esac
}

# program monitor # 
while [ true ];
do
  if [ ! -e $PID_FILE -o ! -e /proc/`cat $PID_FILE` ]; then 
    run_wre start
  fi

  sleep $MON_INTL;
done
