/* files.h */
#ifndef _FILES_H
#define _FILES_H
#define LEASE_PATH "/etc/udhcp_lease"

struct config_keyword {
	const char *keyword;
	int (* const handler)(const char *line, void *var);
	void *var;
	const char *def;
};

int read_config(const char *file);
void write_leases(void);
void read_leases(const char *file);
void write_leases_enhance(struct dhcpOfferedAddr *lease);
void sendCwmpMsg(int msgType);
#endif
