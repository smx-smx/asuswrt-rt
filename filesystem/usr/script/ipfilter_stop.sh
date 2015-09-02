#!/bin/sh

iptables -F IP_FLT
iptables -F MAC_FLT
# wtw add ifeb ebtables -F IP_FLT
# wtw add ifeb ebtables -F MAC_FLT
