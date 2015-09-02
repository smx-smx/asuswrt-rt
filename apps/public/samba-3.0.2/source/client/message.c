/*
   Note: I created this file by copying client.c and then cutting out
   everything not needed for messaging.  -- Chris Wilson, 2/21/2004
*/
/* 
   Unix SMB/CIFS implementation.
   SMB client
   Copyright (C) Andrew Tridgell 1994-1998
   Copyright (C) Simo Sorce 2001-2002
   Copyright (C) Jelmer Vernooij 2003
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#define NO_SYSLOG

#include "includes.h"
#include "../client/client_proto.h"
#ifndef REGISTER
#define REGISTER 0
#endif

struct cli_state *cli;
extern BOOL in_client;
static int port = 0;
static pstring desthost;
static pstring username;
static pstring password;

static int name_type = 0x20;

/* 30 second timeout on most commands */
#define CLIENT_TIMEOUT (30*1000)
#define SHORT_TIMEOUT (5*1000)

/* value for unused fid field in trans2 secondary request */
#define FID_UNUSED (0xFFFF)

/* clitar bits insert */
extern int blocksize;
extern BOOL tar_inc;
extern BOOL tar_reset;
/* clitar bits end */
 

#define SEPARATORS " \t\n\r"

extern file_info def_finfo;

#define USENMB

/****************************************************************************
 Send a message.
****************************************************************************/

static void send_message(void)
{
	int total_len = 0;
	int grp_id;

	if (!cli_message_start(cli, desthost, username, &grp_id)) {
		d_printf("message start: %s\n", cli_errstr(cli));
		return;
	}


	d_printf("Connected. Type your message, ending it with a Control-D\n");

	while (!feof(stdin) && total_len < 1600) {
		int maxlen = MIN(1600 - total_len,127);
		pstring msg;
		int l=0;
		int c;

		ZERO_ARRAY(msg);

		for (l=0;l<maxlen && (c=fgetc(stdin))!=EOF;l++) {
			if (c == '\n')
				msg[l++] = '\r';
			msg[l] = c;   
		}

		if (!cli_message_text(cli, msg, l, grp_id)) {
			d_printf("SMBsendtxt failed (%s)\n",cli_errstr(cli));
			return;
		}      
		
		total_len += l;
	}

	if (total_len >= 1600)
		d_printf("the message was truncated to 1600 bytes\n");
	else
		d_printf("sent %d bytes\n",total_len);

	if (!cli_message_end(cli, grp_id)) {
		d_printf("SMBsendend failed (%s)\n",cli_errstr(cli));
		return;
	}      
}

/****************************************************************************
 Handle a message operation.
****************************************************************************/

static int do_message_op(void)
{
	struct in_addr ip;
	struct nmb_name called, calling;
	fstring server_name;
	char name_type_hex[10];

	make_nmb_name(&calling, global_myname(), 0x0);
	make_nmb_name(&called , desthost, name_type);

	fstrcpy(server_name, desthost);
	snprintf(name_type_hex, sizeof(name_type_hex), "#%X", name_type);
	fstrcat(server_name, name_type_hex);

        zero_ip(&ip);

	if (!(cli=cli_initialise(NULL)) || (cli_set_port(cli, port) != port) ||
	    !cli_connect(cli, server_name, &ip)) {
		d_printf("Connection to %s failed\n", desthost);
		return 1;
	}

	if (!cli_session_request(cli, &calling, &called)) {
		d_printf("session request failed\n");
		cli_shutdown(cli);
		return 1;
	}

	send_message();
	cli_shutdown(cli);

	return 0;
}

/****************************************************************************
  main program
****************************************************************************/

 int main(int argc,char *argv[])
{
	extern BOOL AllowDebugChange;
	int opt;
	BOOL message = False;
	poptContext pc;
	char *p;
	fstring new_workgroup;
	struct poptOption long_options[] = {
		POPT_AUTOHELP

		{ "message", 'M', POPT_ARG_STRING, NULL, 'M', "Send message", "HOST" },
		POPT_COMMON_SAMBA
		POPT_COMMON_CONNECTION
		POPT_COMMON_CREDENTIALS
		POPT_TABLEEND
	};
	

	/* initialize the workgroup name so we can determine whether or 
	   not it was set by a command line option */
	   
	set_global_myworkgroup( "" );

        /* set default debug level to 0 regardless of what smb.conf sets */
	setup_logging( "smbmessage", True );
	DEBUGLEVEL_CLASS[DBGC_ALL] = 1;
	dbf = x_stderr;
	x_setbuf( x_stderr, NULL );

	pc = poptGetContext("smbmessage", argc, (const char **) argv, long_options, 
				POPT_CONTEXT_KEEP_FIRST);

	in_client = True;   /* Make sure that we tell lp_load we are */

	while ((opt = poptGetNextOpt(pc)) != -1) {
		switch (opt) {
		case 'M':
			/* Messages are sent to NetBIOS name type 0x3
			 * (Messenger Service).  Make sure we default
			 * to port 139 instead of port 445. srl,crh
			 */
			name_type = 0x03; 
			pstrcpy(desthost,poptGetOptArg(pc));
			if( 0 == port )
				port = 139;
 			message = True;
 			break;
		}
	}

	poptGetArg(pc);
	
	/*
	 * Don't load debug level from smb.conf. It should be
	 * set by cmdline arg or remain default (0)
	 */
	AllowDebugChange = False;
	
	/* save the workgroup...
	
	   FIXME!! do we need to do tyhis for other options as well 
	   (or maybe a generic way to keep lp_load() from overwriting 
	   everything)?  */
	
	fstrcpy( new_workgroup, lp_workgroup() );		
	
	if (!lp_load(dyn_CONFIGFILE,True,False,False)) {
		fprintf(stderr, "%s: Can't load %s - run testparm to debug it\n",
			argv[0], dyn_CONFIGFILE);
	}
	
	load_interfaces();
	
	if ( strlen(new_workgroup) != 0 )
		set_global_myworkgroup( new_workgroup );

	if (poptPeekArg(pc)) {
		poptPrintUsage(pc, stderr, 0);
		exit(1);
	}

	init_names();

	if (!message) {
		poptPrintUsage(pc, stderr, 0);
		exit(1);
	}

	poptFreeContext(pc);

	pstrcpy(username, cmdline_auth_info.username);
	pstrcpy(password, cmdline_auth_info.password);

	DEBUG(3,("Client started (version %s).\n", SAMBA_VERSION_STRING));

	return do_message_op();
}
