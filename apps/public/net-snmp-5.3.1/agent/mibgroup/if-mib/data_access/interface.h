/*
 * interface data access header
 *
 * $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/if-mib/data_access/interface.h#1 $
 */
#ifndef NETSNMP_ACCESS_INTERFACE_CONFIG_H
#define NETSNMP_ACCESS_INTERFACE_CONFIG_H

/**---------------------------------------------------------------------*/
/*
 * configure required files
 *
 * Notes:
 *
 * 1) prefer functionality over platform, where possible. If a method
 *    is available for multiple platforms, test that first. That way
 *    when a new platform is ported, it won't need a new test here.
 *
 * 2) don't do detail requirements here. If, for example,
 *    HPUX11 had different reuirements than other HPUX, that should
 *    be handled in the *_hpux.h header file.
 */
config_require(if-mib/data_access/interface)
#if defined( linux )
config_require(if-mib/data_access/interface_linux)
config_require(if-mib/data_access/interface_ioctl)
#else
#   define NETSNMP_ACCESS_INTERFACE_NOARCH 1
#endif

#endif /* NETSNMP_ACCESS_INTERFACE_CONFIG_H */
