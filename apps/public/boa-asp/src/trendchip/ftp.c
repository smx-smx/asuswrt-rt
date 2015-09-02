#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

static int ftp_type = -1;//rootfd:2;kernal:1;

char *TcWebApiFtp_get(int id)
{
	if(!ASP_ISPOST)
	{
		ASP_OUTPUT("\"\"");
		return ASP_OK;
	}
	
	if(id == GB_C_UPDATE_STATUS)
	{
		if(ftp_type ==-1)
			ASP_OUTPUT("\"FTP Firmware Upgrade Failure.\"");
	
	}
	return ASP_OK;
		
}

int TcWebApiFtp_set(int id,char *value)
{
	char *ret;
	ftp_type = -1;
	ret = ASP_FORM(value);
	if(ret == NULL)
		return 0;
	if(id == GB_C_UPDATE_TYPE)
	{
		if(!strcmp(ret,val_def->update_rootfs))
			ftp_type = 2;
		else if(!strcmp(ret,val_def->update_kernal))
			ftp_type = 1;
	}
	return 0;
}

int TcWebApiFtp_execute()
{
	int child_pid;
	char *type = sprintf_buf,*path=sprintf_buf+128;
	struct stat buf;
	
	if(ftp_type == -1)
		return 0;
	
	sprintf(type,"%d",ftp_type);
	if(ftp_type ==1)
		sprintf(path,"/var/ftp/rootfs");	
	else if(ftp_type ==2)
		sprintf(path,"/var/ftp/linux.7z");		

	if(stat(path,&buf)== -1)
	{
		ftp_type =-1;
		return 0;
	}	
		//dd		
		child_pid = vfork();
		switch(child_pid)
		{
			case -1:
				break;
			case 0:
				execl("/var/boaroot/cgi-bin/tools_dd", "/var/boaroot/cgi-bin/tools_dd", type, path,
                      NULL);
				_exit(1);
				break;
			default:	
				break;
		}

	return 0;	
}

