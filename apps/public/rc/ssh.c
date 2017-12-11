
#include "rc.h"

inline int check_host_key(const char *ktype, const char *nvname, const char *hkfn)
{
	char cmd[256];
	
	unlink(hkfn);
	
	memset(cmd, 0, sizeof(cmd));
	if (!nvram_get_multiattr_file("SSH_Entry", nvname, hkfn, 2048)) {
		snprintf(cmd, sizeof(cmd), "dropbearkey -t %s -f %s", (char *)ktype, (char *)hkfn);
		system(cmd);
		if (nvram_set_multiattr_file("SSH_Entry", nvname, hkfn, 2048)) {
			return 1;
		}
	}
	return 0;
}

void start_sshd(void)
{
	char keyvalue[2048];
	char enable[4];
	char cmd[128];
	int pass;
	
	memset(enable, 0, sizeof(enable));
	memset(keyvalue, 0, sizeof(keyvalue));
	
	if (!tcapi_match("SSH_Entry", "Enable", "Yes"))
		return;
	
	mkdir("/tmp/home", 0700);
	mkdir("/tmp/home/root", 0700);
	mkdir("/tmp/home/root/.ssh", 0700);
	mkdir("/tmp/etc/dropbear", 0700);
	
	unlink("/tmp/home/root/.ssh/authorized_keys");
	if(!tcapi_get_int("SSH_Entry","Need_Pass")) {
		tcapi_get("SSH_Entry","Authkeys", keyvalue);
		f_write_string("/tmp/home/root/.ssh/authorized_keys", keyvalue, 0, 0700);
	}
	
	if(!f_exists("/etc/dropbear/dropbear_rsa_host_key"))
		check_host_key("rsa", "sshd_hostkey", "/etc/dropbear/dropbear_rsa_host_key");
	if(!f_exists("/etc/dropbear/dropbear_dss_host_key"))
		check_host_key("dss", "sshd_dsskey",  "/etc/dropbear/dropbear_dss_host_key");
	
	snprintf(cmd, sizeof(cmd), "dropbear -p %d %s -a &", 
			tcapi_get_int("SSH_Entry", "sshport"), 
			(tcapi_get_int("SSH_Entry", "Need_Pass")==0)? "-s":"");
	system(cmd);
}

void stop_sshd(void)
{
	killall("dropbear", SIGTERM);
}
