/*
 * route data access header
 *
 * $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/ip-forward-mib/data_access/route.h#1 $
 */
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
config_require(ip-forward-mib/data_access/route_common)

#if defined( linux )
config_require(ip-forward-mib/data_access/route_linux)
config_require(ip-forward-mib/data_access/route_ioctl)
#else
#error "the route data access library is not available for this platform."
#endif

/** need interface for ifIndex */
config_require(if-mib/data_access/interface)

