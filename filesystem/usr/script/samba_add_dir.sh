#!/bin/sh

SAMBA_FILE=/etc/samba/smb.conf

if [ ! -n "$4" ]; then
	echo "insufficient arguments!"
	echo "Usage: $0 <dir name> <access path> <browseable> <writable>"
	echo "Example: $0 temp /temp admin"
	exit 0
fi


echo "
["$1"]
path = "$2"
browseable = "$3"
writable = "$4"
create mask = 0664
directory mask = 0775
" >> $SAMBA_FILE

