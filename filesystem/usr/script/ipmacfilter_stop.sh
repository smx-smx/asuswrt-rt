iptables -t filter -F ipfilter_chain
iptables -t filter -Z ipfilter_chain
#iptables -t filter -D FORWARD -j ipfilter_chain
#iptables -t filter -X ipfilter_chain
