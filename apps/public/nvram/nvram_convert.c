#include <nvram_convert.h>

#define WL(a)	"wl_"a	
#define WL0(a)	"wl0_"a	

#define PPP(a)		"ppp_"a
#define PPPOE(a)	"pppoe_"a

struct nvram_convert nvram_converts[] = {
	{ WL0("ifname"), 		WL("ifname")},	//	wlconf sets wl0_ifname, but not wl_ifname -- zzz
	// for PPPoE
	{ PPP("username"), 		PPPOE("username")},
	{ PPP("passwd"), 		PPPOE("passwd")},
	{ PPP("idletime"), 		PPPOE("idletime")},
	{ PPP("keepalive"), 	PPPOE("keepalive")},
	{ PPP("demand"), 		PPPOE("demand")},
	{ PPP("service"), 		PPPOE("service")},
	{ PPP("ac"), 			PPPOE("ac")},
	{ PPP("static"),		PPPOE("static")},
	{ PPP("static_ip"), 	PPPOE("static_ip")},
	{ PPP("username_1"), 	PPPOE("username_1")},
	{ PPP("passwd_1"), 		PPPOE("passwd_1")},
	{ PPP("idletime_1"), 	PPPOE("idletime_1")},
	{ PPP("keepalive_1"), 	PPPOE("keepalive_1")},
	{ PPP("demand_1"), 		PPPOE("demand_1")},
	{ PPP("service_1"), 	PPPOE("service_1")},
	{ PPP("ac_1"), 			PPPOE("ac_1")},
	
	{ 0, 0 },
};
