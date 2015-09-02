iptables -F acl_chain
#iptables -t filter -D INPUT -p TCP -m multiport --dport ftp,telnet,http,snmp -j acl_chain
#iptables -t filter -D INPUT -p UDP -m multiport --dport ftp,telnet,http,snmp -j acl_chain
#iptables -t filter -D INPUT -p ICMP --icmp-type 8 -j acl_chain
iptables -t filter -D ACL -p TCP -m multiport --dport ftp,telnet,http,snmp -j acl_chain
iptables -t filter -D ACL -p UDP -m multiport --dport ftp,telnet,http,snmp -j acl_chain
iptables -t filter -D ACL -p ICMP --icmp-type 8 -j acl_chain
iptables -X acl_chain
