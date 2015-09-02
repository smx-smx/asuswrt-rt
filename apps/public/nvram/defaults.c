/* nvram table for asuswrt, version 0.1 */

#include <typedefs.h>
#include <string.h>
#include <ctype.h>
#include <bcmnvram.h>
#include <stdio.h>
#include <shared.h>
#include <version.h>
#include <shutils.h>

#define XSTR(s) STR(s)
#define STR(s) #s

// stub for wlconf, etc.
struct nvram_tuple router_defaults[] = {
	// NVRAM for init_syspara: system paraamters getting from MFG/NVRAM Area
	//{ "boardflags", "" },
	//{ "productid", "" },
	//{ "firmver", "" },
	//{ "hardware_version", "" }, // bootloader and hardware version
	//{ "et0macaddr", "" },
	//{ "wl0macaddr", "" },
	//{ "wl1macaddr", "" },

	// NVRAM for restore_defaults: system wide parameters
	{ "nvramver", ""},
	{ "restore_defaults",	"0"	},	// Set to 0 to not restore defaults on boot
	{ "sw_mode", "1" 		}, 	// big switch for different mode 
	{ "preferred_lang", "EN"	},

	{ NULL, NULL }
}; // router_defaults

// nvram for system control state
struct nvram_tuple router_state_defaults[] = {
	{ "rc_service", ""},

	{ NULL, NULL }
};

/* nvram override default setting for Media Router */
struct nvram_tuple router_defaults_override_type1[] = {
	{ "router_disable", "1", 0 },		/* lan_proto=static lan_stp=0 wan_proto=disabled */

	{ 0, 0, 0 }
};

/* Translates from, for example, wl0_ (or wl0.1_) to wl_. */
/* Only single digits are currently supported */

static void
fix_name(const char *name, char *fixed_name)
{
	char *pSuffix = NULL;

	/* Translate prefix wlx_ and wlx.y_ to wl_ */
	/* Expected inputs are: wld_root, wld.d_root, wld.dd_root
	 * We accept: wld + '_' anywhere
	 */
	pSuffix = strchr(name, '_');

	if ((strncmp(name, "wl", 2) == 0) && isdigit(name[2]) && (pSuffix != NULL)) {
		strcpy(fixed_name, "wl");
		strcpy(&fixed_name[2], pSuffix);
		return;
	}

	/* No match with above rules: default to input name */
	strcpy(fixed_name, name);
}


/*
 * Find nvram param name; return pointer which should be treated as const
 * return NULL if not found.
 *
 * NOTE: This routine special-cases the variable wl_bss_enabled. It will
 * return the normal default value if asked for wl_ or wl0_. But it will
 * return 0 if asked for a virtual BSS reference like wl0.1_.
 */
char *
nvram_default_get(const char *name)
{
	int idx;
	char fixed_name[NVRAM_MAX_VALUE_LEN];

	fix_name(name, fixed_name);
	if (strcmp(fixed_name, "wl_bss_enabled") == 0) {
		if (name[3] == '.' || name[4] == '.') { /* Virtual interface */
			return "0";
		}
	}

	/* check name wlx_xxx first */
	if ((strncmp(name, "wl", 2) == 0) && isdigit(name[2]) && (name[3] == '_'))
	for (idx = 0; router_defaults[idx].name != NULL; idx++) {
		if (strcmp(router_defaults[idx].name, name) == 0) {
			return router_defaults[idx].value;
		}
	}

	for (idx = 0; router_defaults[idx].name != NULL; idx++) {
		if (strcmp(router_defaults[idx].name, fixed_name) == 0) {
			return router_defaults[idx].value;
		}
	}

	return NULL;
}
/* validate/restore all per-interface related variables */
void
nvram_validate_all(char *prefix, bool restore)
{
	struct nvram_tuple *t;
	char tmp[100];
	char *v;

	return;

	for (t = router_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3)) {
			strcat_r(prefix, &t->name[3], tmp);
			if (!restore && nvram_get(tmp))
				continue;
			v = nvram_get(t->name);
			nvram_set(tmp, v ? v : t->value);
		}
	}

	/* override router type1 nvram setting */
	if (!strcmp(nvram_safe_get("devicemode"), "1")) {
		for (t = router_defaults_override_type1; t->name; t++) {
			if (!strncmp(t->name, "wl_", 3)) {
				strcat_r(prefix, &t->name[3], tmp);
				if (!restore && nvram_get(tmp))
					continue;
				v = nvram_get(t->name);
				nvram_set(tmp, v ? v : t->value);
			}
		}
	}
}

/* restore specific per-interface variable */
void
nvram_restore_var(char *prefix, char *name)
{
	struct nvram_tuple *t;
	char tmp[100];

	for (t = router_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3) && !strcmp(&t->name[3], name)) {
			nvram_set(strcat_r(prefix, name, tmp), t->value);
			break;
		}
	}

	/* override router type1 setting */
	if (!strcmp(nvram_safe_get("devicemode"), "1")) {
		for (t = router_defaults_override_type1; t->name; t++) {
			if (!strncmp(t->name, "wl_", 3) && !strcmp(&t->name[3], name)) {
				nvram_set(strcat_r(prefix, name, tmp), t->value);
				break;
			}
		}
	}
}
