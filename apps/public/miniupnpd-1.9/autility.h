#ifndef __AUTILITY_H__
#define __AUTILITY_H__

//log function
void _write_to_upnp_log(const char *funname, int lineno, const char *fmt, ...);
#define UPNPDBG_LOG(FMT, ARG...) _write_to_upnp_log(__FUNCTION__, __LINE__, FMT, ##ARG)

#endif
