/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996 Charles F. Randall <crandall@goldsys.com>
 *  Some changes Copyright (C) 1996 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-2002 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/boa-asp/src/boa.c#2 $*/

#include "boa.h"
#include <sys/resource.h>

/* globals */
int backlog = SO_MAXCONN;
time_t start_time;

#ifdef TRENDCHIP
//krammer add
int dontDoAsp=0;
#endif

int sighup_flag = 0;            /* 1 => signal has happened, needs attention */
int sigchld_flag = 0;           /* 1 => signal has happened, needs attention */
int sigalrm_flag = 0;           /* 1 => signal has happened, needs attention */
int sigterm_flag = 0;           /* lame duck mode */
time_t current_time;
int max_fd = 0;
int pending_requests = 0;
char lan_ip[20], lan_netmask[20];
char https_lanport[8] = {0};
char def_passwd[128];
int compare_passwd = 0;

struct pageset* pageMap[PAGEMAP_I_NUM][PAGEMAP_J_NUM];

/* static to boa.c */
static void fixup_server_root(void);
static int create_server_socket(void);
static void drop_privs(void);

extern void init_asp_funcs(void);
#ifndef TRENDCHIP
extern void init_asp_constants(void);
#endif
static int sock_opt = 1;
static int do_fork = 1;
int devnullfd = -1;
#if defined(TCSUPPORT_WEBSERVER_SSL)
int http_mode = -1;
#endif
#ifndef TRENDCHIP
int load_config_file();
#endif

#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
extern	void	initParseStruct(void);
extern	int	initandparserfile(void);
#endif

extern void http_update_allowed_client(void);


#if !defined(TCSUPPORT_C1_NEW_GUI) 
#if !defined(TCSUPPORT_CD_NEW_GUI) 
void init_pageMap()
{
	int i, j;
	struct pageset* pagesetNow;
        for(i = 0; i < PAGEMAP_I_NUM; i++)
                for(j = 0; j < PAGEMAP_J_NUM; j++)
			pageMap[i][j] = malloc(sizeof(struct pageset));
#if !defined(TCSUPPORT_CT_E8GUI) 
	pageMap[0][0]->page_name = "navigation.asp";
	pageMap[0][0]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[0][0]->next;
	pagesetNow->page_name = "wizardBridge.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardclose.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardcomp.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardConType.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardDHCP.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardPPP.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardpwd.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardStatic.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardTZ.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "wizardstart.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "home_wizard.asp";
	pagesetNow->next = NULL;

	pageMap[1][0]->page_name = "navigation-basic.asp";
	pageMap[1][0]->next = NULL;
	pageMap[1][1]->page_name = "home_wan.asp";
	pageMap[1][1]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[1][1]->next;
	pagesetNow->page_name = "home_pvclist.cgi";
	pagesetNow->next = NULL;
	pageMap[1][2]->page_name = "home_lan.asp";
	pageMap[1][2]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[1][2]->next;
	pagesetNow->page_name = "dhcp_static_list.cgi";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "dhcp_client_list.cgi";
	pagesetNow->next = NULL;
	pageMap[1][3]->page_name = "home_wireless.asp";
	pagesetNow->next = NULL;

	pageMap[2][0]->page_name = "navigation-advanced.asp";
	pageMap[2][0]->next = NULL;
	pageMap[2][1]->page_name = "adv_firewall.asp";
	pageMap[2][1]->next = NULL;
	pageMap[2][2]->page_name = "adv_routing_table.asp";
	pageMap[2][2]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[2][2]->next;
	pagesetNow->page_name = "adv_routing_table.cgi";
	pagesetNow->next = NULL;
	pageMap[2][3]->page_name = "adv_nat_dmz.asp";
	pageMap[2][3]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[2][3]->next;
	pagesetNow->page_name = "adv_nat_ipaddrmap.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "adv_nat_top.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "adv_nat_virsvr.asp";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "virsvr_table.cgi";
	pagesetNow->next = NULL;
	pageMap[2][4]->page_name = "adv_adsl.asp";
	pagesetNow->next = NULL;
	pageMap[2][5]->page_name = "adv_qos.asp";
	pagesetNow->next = NULL;

	pageMap[2][6]->page_name = "adv_portbinding.asp";
	pagesetNow->next = NULL;
	pageMap[2][7]->page_name = "adv_pvidsetting.asp";
	pagesetNow->next = NULL;
	pageMap[3][0]->page_name = "navigation-access.asp";
	pageMap[3][0]->next = NULL;
	pageMap[3][1]->page_name = "access_snmp.asp";
	pageMap[3][1]->next = NULL;
	pageMap[3][2]->page_name = "access_upnp.asp";
	pageMap[3][2]->next = NULL;
	pageMap[3][3]->page_name = "access_ddns.asp";
	pageMap[3][3]->next = NULL;
	pageMap[3][4]->page_name = "access_acl.asp";
	pageMap[3][4]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[3][4]->next;
	pagesetNow->page_name = "access_acllist.cgi";
	pagesetNow->next = NULL;
	pageMap[3][5]->page_name = "access_ipfilter.asp";
	pageMap[3][5]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[3][5]->next;
	pagesetNow->page_name = "access_ipfilterlist.cgi";
	pagesetNow->next = NULL;
	pageMap[3][6]->page_name = "access_cwmp.asp";
	pageMap[3][6]->next = NULL;

	pageMap[4][0]->page_name = "navigation-maintenance.asp";
	pageMap[4][0]->next = NULL;
	pageMap[4][1]->page_name = "tools_admin.asp";
	pageMap[4][1]->next = NULL;
	pageMap[4][2]->page_name = "tools_time.asp";
	pageMap[4][2]->next = NULL;
	pageMap[4][3]->page_name = "tools_update.asp";
	pageMap[4][3]->next = NULL;
	pageMap[4][4]->page_name = "tools_system.asp";
	pageMap[4][4]->next = NULL;
	pageMap[4][5]->page_name = "tools_test.asp";
	pageMap[4][6]->next = NULL;
	pageMap[4][6]->page_name = "tools_save.asp";
	pageMap[4][6]->next = NULL;

	pageMap[5][0]->page_name = "navigation-voip.asp";
	pageMap[5][0]->next = NULL;
	pageMap[5][1]->page_name = "voip_basic.asp";
	pageMap[5][1]->next = NULL;
	pageMap[5][2]->page_name = "voip_media.asp";
	pageMap[5][2]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[5][2]->next;
	pagesetNow->page_name = "voip_media.cgi";
	pagesetNow->next = NULL;
	pageMap[5][3]->page_name = "voip_speed_dial.asp";
	pageMap[5][3]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[5][3]->next;
	pagesetNow->page_name = "voip_speed_dial.cgi";
	pagesetNow->next = NULL;

	pageMap[6][0]->page_name = "navigation-status.asp";
	pageMap[6][0]->next = NULL;
	pageMap[6][1]->page_name = "status_deviceinfo.asp";
	pageMap[6][1]->next = NULL;
	pageMap[6][2]->page_name = "status_log.cgi";
	pageMap[6][2]->next = NULL;
	pageMap[6][3]->page_name = "status_statistics.asp";
	pageMap[6][3]->next = NULL;

	pageMap[7][0]->page_name = "navigation-help.asp";
	pageMap[7][0]->next = malloc(sizeof(struct pageset));
	pagesetNow = pageMap[7][0]->next;
	pagesetNow->page_name = "navigation-help.cgi";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "help_access.htm";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "help_advanced.htm";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "help_index.htm";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "help_interface.htm";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "help_maintenance.htm";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "help_quickstart.htm";
	pagesetNow->next = malloc(sizeof(struct pageset));
	pagesetNow = pagesetNow->next;
	pagesetNow->page_name = "help_status.htm";
	pagesetNow->next = NULL;
#endif
}
#endif
#endif

void free_pageMap()
{
	int i, j;
	struct pageset *pagesetNow, *pagesetNext;

	for(i = 0; i < PAGEMAP_I_NUM; i++){
		for(j = 0; j < PAGEMAP_J_NUM; j++){
			pagesetNow = pageMap[i][j];
			pagesetNext = pageMap[i][j]->next;
			free(pagesetNow);
			while(pagesetNext){
				pagesetNow = pagesetNext;
				pagesetNext = pagesetNow->next;
				free(pagesetNow);
			}
			free(pageMap[i][j]);
		}
	}
}

int main(int argc, char **argv)
{
    int c;                      /* command line arg */
    int server_s;                   /* boa socket */
#ifndef TRENDCHIP
	/*2006-8-17 lee:load trendchip configuration file from flash,write configuration to ram structure*/
	if(load_config_file()== -1)
		exit(1);
	init_asp_constants();	
#endif
	init_asp_funcs ();
#if !defined(TCSUPPORT_C1_NEW_GUI) 
#if !defined(TCSUPPORT_CD_NEW_GUI) 
        init_pageMap();
#endif
#endif
      
#ifndef USE_RETRY_LIST
	memset(&retry_flag, 0, sizeof(retry_flag));
	memset(&retry_flag_wan, 0, sizeof(retry_flag_wan));
#endif

    /* set umask to u+rw, u-x, go-rwx */
    c = umask(~0600);
    if (c == -1) {
        perror("umask");
        exit(1);
    }

    devnullfd = open("/dev/null", 0);

    /* make STDIN and STDOUT point to /dev/null */
    if (devnullfd == -1) {
        DIE("can't open /dev/null");
    }

    if (dup2(devnullfd, STDIN_FILENO) == -1) {
        DIE("can't dup2 /dev/null to STDIN_FILENO");
    }

    if (dup2(devnullfd, STDOUT_FILENO) == -1) {
        DIE("can't dup2 /dev/null to STDOUT_FILENO");
    }

    //get lan ip
    memset(lan_ip, 0, sizeof(lan_ip));
    if(tcapi_get("Lan_Entry0", "IP", lan_ip) != TCAPI_PROCESS_OK || !strlen(lan_ip))
    {
        DIE("can't get lan ip");
    }
	
	//get lan netmask
	memset(lan_netmask, 0, sizeof(lan_netmask));
	if(tcapi_get("Lan_Entry0", "netmask", lan_netmask) != TCAPI_PROCESS_OK)
	{
		DIE("can't get lan netmask");
	}

	//get http lan port
	memset(https_lanport, 0, sizeof(https_lanport));
	if(tcapi_get("Https_Entry", "https_lanport", https_lanport) != TCAPI_PROCESS_OK)
	{
		DIE("can't get lan netmask");
	}
	
	//get default_passwd
	memset(def_passwd, 0, sizeof(def_passwd));
	if(tcapi_get("Account_Entry0", "default_passwd", def_passwd) != TCAPI_PROCESS_OK)
	{
		DIE("can't get default password");
	}

	//compare current password and default password
	compare_passwd = tcapi_match("Account_Entry0", "web_passwd", def_passwd)? 0: 1;

    /* but first, update timestamp, because log_error_time uses it */
    (void) time(&current_time);

    while ((c = getopt(argc, argv, "c:r:s:p:d")) != -1) {
        switch (c) {
        case 'c':
            if (server_root)
                free(server_root);
            server_root = strdup(optarg);
            if (!server_root) {
                perror("strdup (for server_root)");
                exit(1);
            }
            break;
#if defined(TCSUPPORT_WEBSERVER_SSL)
        case 's':
            if ((http_mode=atoi(optarg))<=0) {
                perror("atoi (for http_mode)");
                exit(1);
            }
            break;
        case 'p':
            if ((ssl_server_port=atoi(optarg))<=0) {
                perror("atoi (for ssl_server_port)");
                exit(1);
            }
            break;
#endif
        case 'r':
            if (chdir(optarg) == -1) {
                log_error_time();
                perror("chdir (to chroot)");
                exit(1);
            }
            if (chroot(optarg) == -1) {
                log_error_time();
                perror("chroot");
                exit(1);
            }
            if (chdir("/") == -1) {
                log_error_time();
                perror("chdir (after chroot)");
                exit(1);
            }
            break;
        case 'd':
            do_fork = 0;
            break;
        default:
            fprintf(stderr, "Usage: %s [-c serverroot] [-r chroot] [-d]\n", argv[0]);
            exit(1);
        }
    }

    fixup_server_root();
    read_config_files();
    open_logs();
    server_s = create_server_socket();
    init_signals();
    drop_privs();
    create_common_env();
    build_needs_escape();
    http_update_allowed_client(); 

#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
	initParseStruct();
	initandparserfile();
#endif

#if defined(TCSUPPORT_WEBSERVER_SSL)
	if( http_mode != HTTP_ONLY) {
		if (boa_sslInit() != 1) {
			tcdbg_printf("\n init SSL Stuff failed!");
			DIE("init SSL Stuff failed! ");
		}
	}
#endif
    if (max_connections < 1) {
        struct rlimit rl;

        /* has not been set explicitly */
        c = getrlimit(RLIMIT_NOFILE, &rl);
        if (c < 0) {
            perror("getrlimit");
            exit(1);
        }
        max_connections = rl.rlim_cur;
    }

    /* background ourself */
    if (do_fork) {
        switch(fork()) {
        case -1:
            /* error */
            perror("fork");
            exit(1);
            break;
        case 0:
      	     perror("child");
            /* child, success */
            break;
        default:
            perror("parent");
            /* parent, success */
            exit(0);
            break;
        }
    }

    /* main loop */
    timestamp();

    status.requests = 0;
    status.errors = 0;

    start_time = current_time;
    select_loop(server_s);
	free_pageMap();

    return 0;
}

static int create_server_socket(void)
{
    int server_s;

    server_s = socket(SERVER_AF, SOCK_STREAM, IPPROTO_TCP);
    if (server_s == -1) {
        DIE("unable to create socket");
    }

    /* server socket is nonblocking */
    if (set_nonblock_fd(server_s) == -1) {
        DIE("fcntl: unable to set server socket to nonblocking");
    }

    /* close server socket on exec so cgi's can't write to it */
    if (fcntl(server_s, F_SETFD, 1) == -1) {
        DIE("can't set close-on-exec on server socket!");
    }

    /* reuse socket addr */
    if ((setsockopt(server_s, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,
                    sizeof (sock_opt))) == -1) {
        DIE("setsockopt");
    }

    /* internet family-specific code encapsulated in bind_server()  */
    if (bind_server(server_s, server_ip) == -1) {
        DIE("unable to bind");
    }

    /* listen: large number just in case your kernel is nicely tweaked */
    if (listen(server_s, backlog) == -1) {
        DIE("unable to listen");
    }
    return server_s;
}

static void drop_privs(void)
{
    /* give away our privs if we can */
    if (getuid() == 0) {
        struct passwd *passwdbuf;
        passwdbuf = getpwuid(server_uid);
        if (passwdbuf == NULL) {
            DIE("getpwuid");
        }
        if (initgroups(passwdbuf->pw_name, passwdbuf->pw_gid) == -1) {
            DIE("initgroups");
        }
        if (setgid(server_gid) == -1) {
            DIE("setgid");
        }
        if (setuid(server_uid) == -1) {
            DIE("setuid");
        }
        /* test for failed-but-return-was-successful setuid
         * http://www.securityportal.com/list-archive/bugtraq/2000/Jun/0101.html
         */
         //lee 2006-2-14
        //if (setuid(0) != -1) {
        if (server_uid && server_gid && setuid(0) != -1) {
            DIE("icky Linux kernel bug!");
        }
    } else {
        if (server_gid || server_uid) {
            log_error_time();
            fprintf(stderr, "Warning: "
                    "Not running as root: no attempt to change"
                    " to uid %d gid %d\n", server_uid, server_gid);
        }
        server_gid = getgid();
        server_uid = getuid();
    }
}

/*
 * Name: fixup_server_root
 *
 * Description: Makes sure the server root is valid.
 *
 */

static void fixup_server_root()
{
    char *dirbuf;

    if (!server_root) {
#ifdef SERVER_ROOT
        server_root = strdup(SERVER_ROOT);
        if (!server_root) {
            perror("strdup (SERVER_ROOT)");
            exit(1);
        }
#else
        fputs("boa: don't know where server root is.  Please #define "
              "SERVER_ROOT in boa.h\n"
              "and recompile, or use the -c command line option to "
              "specify it.\n", stderr);
        exit(1);
#endif
    }

    if (chdir(server_root) == -1) {
        fprintf(stderr, "Could not chdir to \"%s\": aborting\n",
                server_root);
        exit(1);
    }

    dirbuf = normalize_path(server_root);
    free(server_root);
    server_root = dirbuf;
}

