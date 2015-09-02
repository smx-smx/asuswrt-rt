#!/bin/bash

if [ $# != 2 ] ; then
	echo "usage: $0 [kernel_config_path] [temp_config_path]"
	exit 0
fi

KERNEL_CONFIG=$1
NEW_KERNEL_CONFIG=$2
echo "Add kernel config for Samba"
	SEDCMD="$SEDCMD -e 's/CONFIG_FAT_DEFAULT_CODEPAGE=437/CONFIG_FAT_DEFAULT_CODEPAGE=936/'"
	SEDCMD="$SEDCMD -e 's/CONFIG_FAT_DEFAULT_IOCHARSET=\"ascii\"/CONFIG_FAT_DEFAULT_IOCHARSET=\"utf8\"/'"
	SEDCMD="$SEDCMD -e 's/CONFIG_NLS_DEFAULT=\"iso8859-1\"/CONFIG_NLS_DEFAULT=\"utf8\"/'"
	SEDCMD="$SEDCMD -e 's/CONFIG_NLS_CODEPAGE_437=m/# CONFIG_NLS_CODEPAGE_437 is not set/'"
	SEDCMD="$SEDCMD -e 's/# CONFIG_NLS_CODEPAGE_936 is not set/CONFIG_NLS_CODEPAGE_936=m/'"
	SEDCMD="$SEDCMD -e 's/CONFIG_NLS_ASCII=m/# CONFIG_NLS_ASCII is not set/'"
	SEDCMD="$SEDCMD -e 's/# CONFIG_NLS_UTF8 is not set/CONFIG_NLS_UTF8=m/'"

	gen="sed $SEDCMD $KERNEL_CONFIG"
	eval $gen > $NEW_KERNEL_CONFIG
echo "kernel config for Samba finish"

