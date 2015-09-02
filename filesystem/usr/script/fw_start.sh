#!/bin/sh

/bin/echo 1 > /proc/sys/net/ipv4/tcp_syncookies
/bin/echo 16 > /proc/sys/net/ipv4/tcp_max_syn_backlog
/bin/echo 3 > /proc/sys/net/netfilter/nf_conntrack_tcp_timeout_syn_recv
