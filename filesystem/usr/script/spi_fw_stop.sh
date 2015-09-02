#!/bin/sh

#/usr/bin/iptables -D INPUT -j SPI_FW
/usr/bin/iptables -D FORWARD -j SPI_FW
/usr/bin/iptables -F SPI_FW
#/usr/bin/iptables -X SPI_FW
