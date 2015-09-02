iptables -t filter -A parent_control_chain -m layer7 --l7proto msnmessenger -m condition --condition ! sched_msn -j DROP
iptables -t filter -A parent_control_chain -p TCP -m multiport --dport 20,21 -m condition --condition ! sched_ftp -j DROP
iptables -t filter -A parent_control_chain -p TCP -m multiport --dport http -m condition --condition ! sched_http -j DROP
iptables -t filter -A parent_control_chain -m layer7 --l7proto Url_sched -m condition --condition sched_url -j DROP

