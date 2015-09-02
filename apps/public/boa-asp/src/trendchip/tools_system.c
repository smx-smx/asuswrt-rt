#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#if 0 // syscall reboot 
#include <sys/syscall.h>
#include <linux/reboot.h>
#include <signal.h>
#include <sys/syslog.h>
#endif

#include "trendchip-def.h"
#include "gb-constants.h"
#include "trendchip_load.h"

void SysRestart_WithSetting();
void SysRestart_WithDefault();

int TcWebApiSysRestart_set(int id,char *value)
{
	char *ret = NULL;
	ret = ASP_FORM(value);
	if(ret == NULL)
		return -1;
	if(id == GB_C_SYSTEM_RESTART)
	{
		if(!strcmp(ret,val_def->sys_curr_set))
		{
			SysRestart_WithSetting();
		}		
		else
			SysRestart_WithDefault();
	}	 		
	return 1;	
	
}

void SysRestart_WithSetting()
{
#if 0			
		asp_execute("/bin/rm -rf /etc/services /etc/group /etc/hosts /etc/resolv.conf /etc/inittab /etc/init.d /etc/igd /etc/snmp/snmpd.conf.tmp /etc/gated.conf.tmp /etc/fstab /etc/config/ssh");
		asp_execute("cd /");
	  	asp_execute("/bin/tar cvf /tmp/x.tar etc/*");
		asp_execute("/bin/dd if=/tmp/x.tar of=/dev/mtdb1");
	  	asp_execute("/bin/sleep 3");	
	  	asp_execute("/sbin/reboot > /var/log/reboot.log");					
#else
		asp_execute("/Save_Restart.sh");
#endif
	
}

void SysRestart_WithDefault()
{
#if 0	
	  	asp_execute("/bin/rm -rf /etc/*");			
	  	asp_shell("echo 0 > /dev/mtdb1");
	  	sleep(3);
		sync();
		syscall(SYS_reboot,LINUX_REBOOT_MAGIC1,LINUX_REBOOT_MAGIC2,LINUX_REBOOT_CMD_RESTART,NULL);		  	
#else
	system("/userfs/bin/mtd -r write /userfs/romfile.cfg romfile");
#endif
}
