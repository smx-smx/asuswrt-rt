#!/bin/bash

if [ $# != 2 ] ; then
	echo "usage: $0 [kernel_config_path] [temp_config_path]"
	exit 0
fi

KERNEL_CONFIG=$1
NEW_KERNEL_CONFIG=$2

if [ "$TCSUPPORT_SAMBA" != "" ] || [ "$TCSUPPORT_DMS" != "" ] || [ "$TCSUPPORT_6RD" != "" ] || [ "$TCSUPPORT_IPV6_PRIVACYADDRS" != "" ]; then

	if [ "$TCSUPPORT_SAMBA" != "" ];then
		echo "Add kernel config for Samba"
		SEDCMD="$SEDCMD -e 's/CONFIG_FAT_DEFAULT_CODEPAGE=437/CONFIG_FAT_DEFAULT_CODEPAGE=936/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_FAT_DEFAULT_IOCHARSET=\"ascii\"/CONFIG_FAT_DEFAULT_IOCHARSET=\"utf8\"/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_NLS_DEFAULT=\"iso8859-1\"/CONFIG_NLS_DEFAULT=\"utf8\"/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_NLS_CODEPAGE_437=m/# CONFIG_NLS_CODEPAGE_437 is not set/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_NLS_CODEPAGE_936 is not set/CONFIG_NLS_CODEPAGE_936=m/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_NLS_ASCII=m/# CONFIG_NLS_ASCII is not set/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_NLS_UTF8 is not set/CONFIG_NLS_UTF8=m/'"
	fi

	if [ "$TCSUPPORT_DMS" != "" ];then
		echo "Add kernel config for DMS"
		SEDCMD="$SEDCMD -e 's/# CONFIG_EXT2_FS is not set/CONFIG_EXT2_FS=y\nCONFIG_EXT2_FS_XATTR=y\nCONFIG_EXT2_FS_POSIX_ACL=y\nCONFIG_EXT2_FS_SECURITY=y\nCONFIG_EXT2_FS_XIP=y\nCONFIG_FS_XIP=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_EXT3_FS is not set/CONFIG_EXT3_FS=y\nCONFIG_EXT3_FS_XATTR=y\nCONFIG_EXT3_FS_POSIX_ACL=y\nCONFIG_EXT3_FS_SECURITY=y/'"
		SEDCMD="$SEDCMD -e '/# CONFIG_EXT4DEV_FS is not set/a\\CONFIG_JBD=y\n# CONFIG_JBD_DEBUG is not set\nCONFIG_FS_MBCACHE=y'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_FS_POSIX_ACL is not set/CONFIG_FS_POSIX_ACL=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_INOTIFY is not set/CONFIG_INOTIFY=y\nCONFIG_INOTIFY_USER=y/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_FAT_FS=m/CONFIG_FAT_FS=y/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_VFAT_FS=m/CONFIG_VFAT_FS=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_NFS_FS is not set/CONFIG_NFS_FS=m\nCONFIG_NFS_V3=y\n# CONFIG_NFS_V3_ACL is not set\n# CONFIG_NFS_V4 is not set\nCONFIG_NFS_DIRECTIO=y/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_VFAT_FS=m/CONFIG_VFAT_FS=y/'"
		SEDCMD="$SEDCMD -e '/# CONFIG_NFSD is not set/a\\CONFIG_LOCKD=m\nCONFIG_LOCKD_V4=y\nCONFIG_NFS_COMMON=y\nCONFIG_SUNRPC=m\n# CONFIG_SUNRPC_BIND34 is not set\n# CONFIG_RPCSEC_GSS_KRB5 is not set\n# CONFIG_RPCSEC_GSS_SPKM3 is not set'"
	fi

	if [ "$TCSUPPORT_6RD" != "" ];then
		echo "Add kernel config for 6RD"
		SEDCMD="$SEDCMD -e 's/# CONFIG_IPV6_SIT_6RD is not set/CONFIG_IPV6_SIT_6RD=y/'"
	fi

	if [ "$TCSUPPORT_IPV6_PRIVACYADDRS" != "" ];then
		echo "Add kernel config for privacy addr"
		SEDCMD="$SEDCMD -e 's/# CONFIG_IPV6_PRIVACY is not set/CONFIG_IPV6_PRIVACY=y/'"
	fi

	if [ "$TCSUPPORT_ADD_JFFS" != "" ];then
		echo "Add kernel config for JFFS"
		SEDCMD="$SEDCMD -e 's/# CONFIG_JFFS2_FS is not set/CONFIG_JFFS2_FS=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_JFFS2_FS_DEBUG is not set/CONFIG_JFFS2_FS_DEBUG=0/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_JFFS2_FS_WRITEBUFFER is not set/CONFIG_JFFS2_FS_WRITEBUFFER=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_JFFS2_COMPRESSION_OPTIONS is not set/CONFIG_JFFS2_COMPRESSION_OPTIONS=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_JFFS2_ZLIB is not set/CONFIG_JFFS2_ZLIB=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_JFFS2_CMODE_PRIORITY is not set/CONFIG_JFFS2_CMODE_PRIORITY=y/'"
	else
		echo "Remove kernel config of JFFS"
		SEDCMD="$SEDCMD -e 's/CONFIG_JFFS2_FS=y/# CONFIG_JFFS2_FS is not set/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_JFFS2_FS_DEBUG=0/# CONFIG_JFFS2_FS_DEBUG is not set/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_JFFS2_FS_WRITEBUFFER=y/# CONFIG_JFFS2_FS_WRITEBUFFER is not set/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_JFFS2_COMPRESSION_OPTIONS=y/# CONFIG_JFFS2_COMPRESSION_OPTIONS is not set/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_JFFS2_ZLIB=y/# CONFIG_JFFS2_ZLIB is not set/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_JFFS2_CMODE_PRIORITY=y/# CONFIG_JFFS2_CMODE_PRIORITY is not set/'"
	fi

	if [ "$TCSUPPORT_PIMD" != "" ];then
		echo "Add PIM config for PIMD"
		SEDCMD="$SEDCMD -e 's/# CONFIG_IP_PIMSM_V1 is not set/CONFIG_IP_PIMSM_V1=y/'"
		SEDCMD="$SEDCMD -e 's/# CONFIG_IP_PIMSM_V2 is not set/CONFIG_IP_PIMSM_V2=y/'"
	else
		echo "Remove kernel config of PIMD"
		SEDCMD="$SEDCMD -e 's/CONFIG_IP_PIMSM_V1=y/# CONFIG_IP_PIMSM_V1 is not set/'"
		SEDCMD="$SEDCMD -e 's/CONFIG_IP_PIMSM_V2=y/# CONFIG_IP_PIMSM_V2 is not set/'"
	fi

	echo "sed $SEDCMD"
	gen="sed $SEDCMD $KERNEL_CONFIG"
	eval $gen > $NEW_KERNEL_CONFIG
else
	echo "no need to add kernel option"
fi
	
echo "modify kernel config finish"

