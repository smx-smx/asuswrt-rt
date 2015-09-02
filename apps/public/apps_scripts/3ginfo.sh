#!/bin/sh
# ASUS 3ginfo script to get dongle information

echo ">"
cat /proc/bus/usb/devices
echo ">"
cat /etc/g3.conf.1
echo ">"
cat /etc/g3.conf.2
echo ">"
cat /tmp/ppp/peers/3g
echo ">"
ls /dev/tty*
echo ">"
ifconfig
echo ">"
lsmod
echo ">"
tcapi get SysInfo_Entry ProductName
echo ">"
tcapi get DeviceInfo FwVer
echo ">"
echo "dualwan nvram:>"
tcapi show Dualwan
echo ">"
tcapi get USBModem_Entry Dev3G
echo ">"
tcapi show USBModem_Entry |grep g3state
echo ">"
tcapi show USBModem_Entry |grep g3err
echo ">"
echo "modem nvram:>"
nvram show|grep ^modem_ |grep -v "modem_pincode="
echo ">"
echo "modem state:>"
str=`tcapi get USBModem_Entry usb_modem_act_path`
echo "usb_modem_act_path=$str"
str=`tcapi get USBModem_Entry usb_modem_act_type`
echo "usb_modem_act_type=$str"
str=`tcapi get USBModem_Entry usb_modem_act_dev`
echo "usb_modem_act_dev=$str"
str=`tcapi get USBModem_Entry usb_modem_act_int`
echo "usb_modem_act_int=$str"
str=`tcapi get USBModem_Entry usb_modem_act_bulk`
echo "usb_modem_act_bulk=$str"
str=`tcapi get USBModem_Entry usb_modem_act_vid`
echo "usb_modem_act_vid=$str"
str=`tcapi get USBModem_Entry usb_modem_act_pid`
echo "usb_modem_act_pid=$str"
str=`tcapi get USBModem_Entry usb_modem_act_sim`
echo "usb_modem_act_sim=$str"
str=`tcapi get USBModem_Entry usb_modem_act_signal`
echo "usb_modem_act_signal=$str"
str=`tcapi get USBModem_Entry usb_modem_act_operation`
echo "usb_modem_act_operation=$str"
str=`tcapi get USBModem_Entry usb_modem_act_imsi |cut -c '1-6'`
echo "usb_modem_act_imsi=$str"
str=`tcapi get USBModem_Entry usb_modem_act_tx`
echo "usb_modem_act_tx=$str"
str=`tcapi get USBModem_Entry usb_modem_act_rx`
echo "usb_modem_act_rx=$str"
str=`tcapi get USBModem_Entry usb_modem_act_hwver`
echo "usb_modem_act_hwver=$str"
str=`tcapi get USBModem_Entry usb_modem_act_band`
echo "usb_modem_act_band=$str"
str=`tcapi get USBModem_Entry usb_modem_act_scanning`
echo "usb_modem_act_scanning=$str"
str=`tcapi get USBModem_Entry usb_modem_act_auth`
echo "usb_modem_act_auth=$str"
str=`tcapi get USBModem_Entry usb_modem_act_auth_pin`
echo "usb_modem_act_auth_pin=$str"
str=`tcapi get USBModem_Entry usb_modem_act_auth_puk`
echo "usb_modem_act_auth_puk=$str"
str=`tcapi get USBModem_Entry usb_modem_act_startsec`
echo "usb_modem_act_startsec=$str"
echo ">"
echo "modem autoapn:>"
tcapi show USBModem_Entry |grep ^usb_modem_auto
echo ">"
echo "resolv.conf >"
cat /etc/resolv.conf
echo "/etc/ppp/resolv_*.conf >"
ls -al /etc/ppp/
cat /etc/ppp/resolv_*
echo ">"
echo "udhcpd.conf >"
cat /etc/udhcpd.conf
echo ">"
echo "show dns nvram >"
echo ">>PVC0"
tcapi get DeviceInfo_PVC0 DNSIP
echo ">>PVC8"
tcapi get DeviceInfo_PVC8 DNSIP
echo ">>PVC10"
tcapi get DeviceInfo_PVC10 DNSIP
echo ">>PVC11"
tcapi get DeviceInfo_PVC11 DNSIP
echo ">"
echo "syslog>"
cat /var/log/currLogFile
echo ">"
echo "usblog>"
cat /tmp/usb.log

