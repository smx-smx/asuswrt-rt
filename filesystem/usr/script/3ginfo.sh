#!/bin/sh
# ASUS 3ginfo script to get dongle information
# Note that if command is not located at /bin, you have to show its full path.
# Ex: /userfs/bin/tcapi or /sbin/ifconfig

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
/sbin/ifconfig
echo ">"
ps
echo ">"
/sbin/lsmod
echo ">"
/userfs/bin/tcapi get SysInfo_Entry ProductName
echo ">"
/userfs/bin/tcapi get DeviceInfo FwVer
echo ">"
echo "dualwan nvram:>"
/userfs/bin/tcapi show Dualwan
echo ">"
/userfs/bin/tcapi get USBModem_Entry Dev3G
echo ">"
/userfs/bin/tcapi show USBModem_Entry |grep g3state
echo ">"
/userfs/bin/tcapi show USBModem_Entry |grep g3err
echo ">"
echo "modem nvram:>"
/userfs/bin/tcapi get USBModem_Entry modem_pincode
echo ">"
echo "modem state:>"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_path`
echo "usb_modem_act_path=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_type`
echo "usb_modem_act_type=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_dev`
echo "usb_modem_act_dev=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_int`
echo "usb_modem_act_int=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_bulk`
echo "usb_modem_act_bulk=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_vid`
echo "usb_modem_act_vid=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_pid`
echo "usb_modem_act_pid=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_sim`
echo "usb_modem_act_sim=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_signal`
echo "usb_modem_act_signal=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_operation`
echo "usb_modem_act_operation=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_imsi |cut -c '1-6'`
echo "usb_modem_act_imsi=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_tx`
echo "usb_modem_act_tx=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_rx`
echo "usb_modem_act_rx=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_hwver`
echo "usb_modem_act_hwver=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_band`
echo "usb_modem_act_band=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_scanning`
echo "usb_modem_act_scanning=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_auth`
echo "usb_modem_act_auth=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_auth_pin`
echo "usb_modem_act_auth_pin=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_auth_puk`
echo "usb_modem_act_auth_puk=$str"
str=`/userfs/bin/tcapi get USBModem_Entry usb_modem_act_startsec`
echo "usb_modem_act_startsec=$str"
echo ">"
echo "modem autoapn:>"
/userfs/bin/tcapi show USBModem_Entry |grep ^usb_modem_auto
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
/userfs/bin/tcapi get DeviceInfo_PVC0 DNSIP
echo ">>PVC8"
/userfs/bin/tcapi get DeviceInfo_PVC8 DNSIP
echo ">>PVC10"
/userfs/bin/tcapi get DeviceInfo_PVC10 DNSIP
echo ">>PVC11"
/userfs/bin/tcapi get DeviceInfo_PVC11 DNSIP
echo ">"
echo "syslog>"
cat /var/log/currLogFile
echo ">"
echo "usblog>"
cat /tmp/usb.log
echo ">"
echo "show USBModem node>"
/userfs/bin/tcapi show USBModem
echo ">"
echo "show USB node>"
/userfs/bin/tcapi show USB

