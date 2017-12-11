#!/bin/sh

PID_FILE=tcd.pid
MON_INTL=5

run_tcd()
{
  cmd="$1"
  [ -z "$cmd" ] && cmd="start"

  case "$cmd" in
    start)
        ./tcd # -v 
        ;;
    stop)
        killall -9 tcd
        ;;
  esac
}

# program monitor # 
while [ true ];
do
  if [ ! -e $PID_FILE ]; then
    run_tcd start
  elif [ ! -e /proc/`cat $PID_FILE`/status ]; then 
    run_tcd start
  fi

  sleep $MON_INTL;
done

