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

        if [ "$TCSUPPORT_IPSEC" != "" ] ;then
            echo "To add kernel config for IPSEC"
            SEDCMD="$SEDCMD -e 's/# CONFIG_XFRM is not set/CONFIG_XFRM=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_TUNNEL is not set/CONFIG_INET_TUNNEL=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_XFRM_USER is not set/CONFIG_XFRM_USER=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_NETFILTER_XT_MATCH_POLICY is not set/CONFIG_NETFILTER_XT_MATCH_POLICY=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_NET_KEY is not set/CONFIG_NET_KEY=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_IP_ROUTE_VERBOSE is not set/CONFIG_IP_ROUTE_VERBOSE=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_AH is not set/CONFIG_INET_AH=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_ESP is not set/CONFIG_INET_ESP=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_IPCOMP is not set/CONFIG_INET_IPCOMP=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_XFRM_TUNNEL is not set/CONFIG_INET_XFRM_TUNNEL=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_XFRM_MODE_TRANSPORT is not set/CONFIG_INET_XFRM_MODE_TRANSPORT=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_XFRM_MODE_TUNNEL is not set/CONFIG_INET_XFRM_MODE_TUNNEL=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET_XFRM_MODE_BEET is not set/CONFIG_INET_XFRM_MODE_BEET=m/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_CRYPTO_NULL is not set/CONFIG_CRYPTO_NULL=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_CRYPTO_SHA256 is not set/CONFIG_CRYPTO_SHA256=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_CRYPTO_SHA512 is not set/CONFIG_CRYPTO_SHA512=y/'"
        else
            echo "To remove kernel config for IPSEC"
            SEDCMD="$SEDCMD -e 's/CONFIG_XFRM=y/# CONFIG_XFRM is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_TUNNEL=m/# CONFIG_INET_TUNNEL is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_XFRM_USER=m/# CONFIG_XFRM_USER is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_NETFILTER_XT_MATCH_POLICY=m/# CONFIG_NETFILTER_XT_MATCH_POLICY is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_NET_KEY=m/# CONFIG_NET_KEY is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_IP_ROUTE_VERBOSE=m/# CONFIG_IP_ROUTE_VERBOSE is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_AH=m/# CONFIG_INET_AH is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_ESP=m/# CONFIG_INET_ESP is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_IPCOMP=m/# CONFIG_INET_IPCOMP is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_XFRM_TUNNEL=m/# CONFIG_INET_XFRM_TUNNEL is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_XFRM_MODE_TRANSPORT=m/# CONFIG_INET_XFRM_MODE_TRANSPORT is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_XFRM_MODE_TUNNEL=m/# CONFIG_INET_XFRM_MODE_TUNNEL is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET_XFRM_MODE_BEET=m/# CONFIG_INET_XFRM_MODE_BEET is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_CRYPTO_NULL=y/# CONFIG_CRYPTO_NULL is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_CRYPTO_SHA256=y/# CONFIG_CRYPTO_SHA256 is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_CRYPTO_SHA512=y/# CONFIG_CRYPTO_SHA512 is not set/'"
        fi

        if [ "$TCSUPPORT_IPV6" != "" -a "$TCSUPPORT_IPSEC" != "" ] ;then
            echo "To add kernel config for IPSEC with IPV6"
            SEDCMD="$SEDCMD -e 's/# CONFIG_IPV6 is not set/CONFIG_IPV6=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET6_XFRM_MODE_TRANSPORT is not set/CONFIG_INET6_XFRM_MODE_TRANSPORT=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET6_XFRM_MODE_TUNNEL is not set/CONFIG_INET6_XFRM_MODE_TUNNEL=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET6_XFRM_MODE_BEET is not set/CONFIG_INET6_XFRM_MODE_BEET=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_IPV6_SIT is not set/CONFIG_IPV6_SIT=y/'"
            SEDCMD="$SEDCMD -e 's/# CONFIG_INET6_XFRM_MODE_TRANSPORT is not set/CONFIG_INET6_XFRM_MODE_TRANSPORT=y/'"
        else
            echo "To remove kernel config for IPSEC with IPV6"
            SEDCMD="$SEDCMD -e 's/CONFIG_IPV6=y/# CONFIG_IPV6 is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET6_XFRM_MODE_TRANSPORT=y/# CONFIG_INET6_XFRM_MODE_TRANSPORT is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET6_XFRM_MODE_TUNNEL=m/# CONFIG_INET6_XFRM_MODE_TUNNEL is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET6_XFRM_MODE_BEET=m/# CONFIG_INET6_XFRM_MODE_BEET is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_IPV6_SIT=y/# CONFIG_IPV6_SIT is not set/'"
            SEDCMD="$SEDCMD -e 's/CONFIG_INET6_XFRM_MODE_TRANSPORT=y/# CONFIG_INET6_XFRM_MODE_TRANSPORT is not set/'"
        fi

	echo "sed $SEDCMD"
	gen="sed $SEDCMD $KERNEL_CONFIG"
	eval $gen > $NEW_KERNEL_CONFIG
else
	echo "no need to add kernel option"
fi
	
echo "modify kernel config finish"

