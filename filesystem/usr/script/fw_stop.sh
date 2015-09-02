#!/bin/sh

/bin/echo 128 > /proc/sys/net/ipv4/tcp_max_syn_backlog
/bin/echo 60 > /proc/sys/net/netfilter/nf_conntrack_tcp_timeout_syn_recv
#krammer
iptables -t filter -F FIREWALL
iptables -t filter -Z FIREWALL
