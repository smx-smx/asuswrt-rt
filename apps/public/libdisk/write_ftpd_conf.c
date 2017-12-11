/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <libtcapi.h>
#include <shutils.h>

#include "usb_info.h"
#include "disk_initial.h"
#include "disk_share.h"

#define VSFTPD_CONF "/etc/vsftpd.conf"


int main(int argc, char *argv[]) {
	FILE *fp;
	char tmp[128];
	int st_ftp_mode, st_max_user;
	
	unlink("/var/log.vsftpd");
	
	if ((fp=fopen(VSFTPD_CONF, "r"))) {
		fclose(fp);
		unlink(VSFTPD_CONF);
	}
	
	if((fp = fopen(VSFTPD_CONF, "w")) == NULL)
		return -1;
	
	tcapi_get("Samba_Entry", "st_ftp_mode", tmp);
	st_ftp_mode = atoi(tmp);
	//if (nvram_match("st_ftp_mode", "2"))
	if (st_ftp_mode == 2)
		fprintf(fp, "anonymous_enable=NO\n");
	else{
		fprintf(fp, "anonymous_enable=YES\n");
		fprintf(fp, "anon_upload_enable=YES\n");
		fprintf(fp, "anon_mkdir_write_enable=YES\n");
		fprintf(fp, "anon_other_write_enable=YES\n");
	}

	fprintf(fp, "nopriv_user=root\n");
	fprintf(fp, "write_enable=YES\n");
	fprintf(fp, "local_enable=YES\n");
	fprintf(fp, "chroot_local_user=YES\n");
	fprintf(fp, "local_umask=000\n");
	fprintf(fp, "dirmessage_enable=NO\n");
	fprintf(fp, "xferlog_enable=NO\n");
	fprintf(fp, "syslog_enable=NO\n");
	fprintf(fp, "connect_from_port_20=YES\n");
//	fprintf(fp, "listen=YES\n");
	fprintf(fp, "listen%s=YES\n",
//#ifdef RTCONFIG_IPV6
#ifdef TCSUPPORT_IPV6
	//ipv6_enabled() ? "_ipv6" : "");
	"_ipv6");
#else
	"");
#endif
	fprintf(fp, "pasv_enable=YES\n");
	fprintf(fp, "ssl_enable=NO\n");
	fprintf(fp, "tcp_wrappers=NO\n");
	
	//strcpy(maxuser, nvram_safe_get("st_max_user"));
	//if ((atoi(maxuser)) > 0)
	tcapi_get("Samba_Entry", "st_max_user", tmp);
	st_max_user = atoi(tmp);
	if (st_max_user > 0)
		fprintf(fp, "max_clients=%d\n", st_max_user);
	else
		fprintf(fp, "max_clients=%s\n", "10");
	fprintf(fp, "ftp_username=anonymous\n");
	//fprintf(fp, "ftpd_banner=Welcome to ASUS %s FTP service.\n", nvram_safe_get("productid"));
	fprintf(fp, "ftpd_banner=Welcome to ASUS %s FTP service.\n", "DSL-N66U");	//tmp
	fprintf(fp, "hide_file={.router_temp*}\n");

	// update codepage
	eval("modprobe", "-r", "nls_cp936");
	eval("modprobe", "-r", "nls_cp950");

	tcapi_get("Samba_Entry", "ftp_lang", tmp);
	//if (strcmp(nvram_safe_get("ftp_lang"), "EN") != 0)
	if (strcmp(tmp, "EN"))
	{
		fprintf(fp, "enable_iconv=YES\n");
		//if (nvram_match("ftp_lang", "TW")) {
		if (!strcmp(tmp, "TW")) {
			fprintf(fp, "remote_charset=cp950\n");		
			eval("modprobe", "nls_cp950");
		}
		//else if (nvram_match("ftp_lang", "CN")) {
		else if (!strcmp(tmp, "CN")) {
			fprintf(fp, "remote_charset=cp936\n");	
			eval("modprobe", "nls_cp936");
		}
	}
	fclose(fp);
	return 0;
}
