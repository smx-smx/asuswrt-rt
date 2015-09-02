iptables -t filter -F app_filter_chain
iptables -t filter -Z app_filter_chain
#iptables -t filter -D FORWARD -j ipfilter_chain
#iptables -t filter -X ipfilter_chain
