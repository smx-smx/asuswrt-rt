iptables -t filter -F url_filter_chain
iptables -t filter -Z url_filter_chain
#iptables -t filter -D FORWARD -j ipfilter_chain
#iptables -t filter -X ipfilter_chain
