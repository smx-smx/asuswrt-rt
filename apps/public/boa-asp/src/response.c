/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-99 Jon Nelson <jnelson@boa.org>
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

/* $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/boa-asp/src/response.c#2 $*/

#include "boa.h"
#include "shared.h"
#include "tcapi.h"

#define HTML "text/html; charset=ISO-8859-1"
#define CRLF "\r\n"

extern int req_write_escape_http_new(request * req, char *msg);
extern asus_token_t* add_asus_token(const CLIENT_TYPE cli_type, const char *token, const char *ipaddr);
extern int decode_uri(char *uri);

void print_content_type(request * req)
{
    req_write(req, "Content-Type: ");
    req_write(req, get_mime_type(req->request_uri));
    req_write(req, "\r\n");
}

void print_content_length(request * req)
{
    req_write(req, "Content-Length: ");
    req_write(req, simple_itoa(req->filesize));
    req_write(req, "\r\n");
}

void print_last_modified(request * req)
{
    static char lm[] = "Last-Modified: "
        "                             " "\r\n";
    rfc822_time_buf(lm + 15, req->last_modified);
    req_write(req, lm);
}

void print_ka_phrase(request * req)
{
    if (req->kacount > 0 &&
        req->keepalive == KA_ACTIVE && req->response_status < 500) {
        req_write(req, "Connection: Keep-Alive\r\nKeep-Alive: timeout=");
        req_write(req, simple_itoa(ka_timeout));
        req_write(req, ", max=");
        req_write(req, simple_itoa(req->kacount));
        req_write(req, "\r\n");
    } else
    {
        req_write(req, "Connection: close\r\n");
}
}

void print_fromapp_header(request *req)
{
	char buf[1024], tmp[64];

	if(!req)
	{
		_dprintf("[%s, %d] no req!\n", __FUNCTION__, __LINE__);
		return;
	}

	int cli_type = check_client_type(req->header_user_agent);

	if(cli_type != CLI_TYPE_BROWSER)
	{
		//req_write(req, "Cache-Control: no-store\r\n");
		//req_write(req, "Pragma: no-cache\r\n");
		if(CLI_TYPE_APP == cli_type)
		{
			snprintf(buf, sizeof(buf), "AiHOMEAPILevel: %d\r\n", EXTEND_AIHOME_API_LEVEL);
			req_write(req, buf);
			snprintf(buf, sizeof(buf), "Httpd_AiHome_Ver: %d\r\n", EXTEND_HTTPD_AIHOME_VER);
			req_write(req, buf);
			snprintf(buf, sizeof(buf), "Model_Name: %s\r\n", get_productid());
			req_write(req, buf);
		}
		else if(CLI_TYPE_ASSIA == cli_type)
		{
			snprintf(buf, sizeof(buf), "ASSIA_API_Level: %d\r\n", EXTEND_ASSIA_API_LEVEL);
			req_write(req, buf);
		}
	}	
}

void print_http_headers(request * req)
{
    static char stuff[] = "Date: "
        "                             "
        "\r\nServer: " SERVER_VERSION "\r\n";

    rfc822_time_buf(stuff + 6, 0);
    req_write(req, stuff);
#if defined(TCSUPPORT_WEBSERVER_SSL)
	if(req->ssl==NULL)
#endif 
    print_ka_phrase(req);
    print_fromapp_header(req);
}

void print_no_cache(request * req)
{
#if defined(TCSUPPORT_WEBSERVER_SSL)
	if(strstr(req->request_uri,"romfile.cfg") == NULL) 
#endif 
	{
    req_write(req, "Cache-Control: no-cache\r\n");
    req_write(req, "Pragma: no-cache\r\n");
    req_write(req, "Expires: 0\r\n");
	}
}

/* The routines above are only called by the routines below.
 * The rest of Boa only enters through the routines below.
 */

/* R_REQUEST_OK: 200 */
void send_r_request_ok(request * req)
{
#ifdef BOA_DEBUG
	struct timeval tv;
	gettimeofday(&tv,NULL);
	fprintf(stderr,"send_r_request_ok:%d,%d\n",tv.tv_usec,tv.tv_sec);
#endif
    req->response_status = R_REQUEST_OK;
    if (req->simple)
        return;

    req_write(req, "HTTP/1.0 200 OK\r\n");
    print_http_headers(req);
    print_no_cache(req);    

    if (!req->is_cgi) {
        print_content_length(req);
        print_last_modified(req);
        print_content_type(req);
        req_write(req, "\r\n");
    }
}

/* R_MOVED_PERM: 301 */
void send_r_moved_perm(request * req, char *url)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_PERM;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 301 Moved Permanently\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");
        req_write_escape_http(req, url);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>301 Moved Permanently</TITLE></HEAD>\n"
                  "<BODY>\n<H1>301 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_MOVED_TEMP: 302 */
void send_r_moved_temp(request * req, char *url, char *more_hdr)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_TEMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 302 Moved Temporarily\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");
        req_write_escape_http(req, url);
        req_write(req, "\r\n");
        req_write(req, more_hdr);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>302 Moved Temporarily</TITLE></HEAD>\n"
                  "<BODY>\n<H1>302 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

#if !defined(TCSUPPORT_CT)
/* R_MOVED_TEMP: 302 */
void send_r_moved_temp_new(request * req, char *url, char *more_hdr)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_TEMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 302 Moved Temporarily\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");

	/* In request url, the ipv6 ip address will be enclosed in the square brackets '[' and ']'. 
	And this square brackets should not be transferred.*/
        //req_write_escape_http(req, url);
	req_write_escape_http_new(req, url);
	 
        req_write(req, "\r\n");
        req_write(req, more_hdr);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>302 Moved Temporarily</TITLE></HEAD>\n"
                  "<BODY>\n<H1>302 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}
#endif


/* R_NOT_MODIFIED: 304 */
void send_r_not_modified(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_MODIFIED;
    req_write(req, "HTTP/1.0 304 Not Modified\r\n");
    print_http_headers(req);
    print_content_type(req);
    req_write(req, "\r\n");
    req_flush(req);
}

/* R_BAD_REQUEST: 400 */
void send_r_bad_request(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_REQUEST;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 400 Bad Request\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD)
        req_write(req,
                  "<HTML><HEAD><TITLE>400 Bad Request</TITLE></HEAD>\n"
                  "<BODY><H1>400 Bad Request</H1>\nYour client has issued "
                  "a malformed or illegal request.\n</BODY></HTML>\n");
    req_flush(req);
}

/* Paul add start 2013/1/3 */
/* R_BAD_REQUEST: 400 */
void send_r_cannot_login(request * req)
{
		SQUASH_KA(req);
		char cur_temp_ip[32];
		memset(cur_temp_ip, 0, 32);
#ifdef ASUS_LOGIN_SESSION
		asus_token_t *token = SEARCH_TOKEN_LST_BY_CLITYPE(CLI_TYPE_BROWSER);
		if(token)
			snprintf(cur_temp_ip, sizeof(cur_temp_ip), token->ipaddr);
#else
		tcapi_get("WebCurSet_Entry", "login_ip_tmp", cur_temp_ip);
#endif
		req->response_status = R_BAD_REQUEST;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 400 Bad Request\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD)
        req_write(req,
									"<html><head>\n"
									"<title>ASUS Wireless Modem Router Web Manager</title>\n"
									"<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>\n"
									"<meta HTTP-EQUIV='Pragma' CONTENT='no-cache'>\n"
									"<meta HTTP-EQUIV='Expires' CONTENT='-1'>\n"
									"<style type='text/css'>\n"
									".Desc{\n"
									"text-shadow: 0px 1px 0px white;\n"
									"font-weight: bolder;\n"
									"font-size: 22px;\n"
									"font-family:Segoe UI, Arial, sans-serif;\n"
									"color: #000;\n"
									"line-height: 40px;\n"
									"text-align: left;\n"
									"}\n"
									"</style>\n"
									"</head>\n"
									"<body style='text-align:center;background: #DDD'>\n"
									"<div style='margin-top:100px;width:50%;margin-left:25%'>\n"
									"<div class='Desc'>Login user IP: ");
        req_write(req, cur_temp_ip);
        req_write(req,
									"</div>"
									"<div class='Desc'>You cannot Login unless logout another user first.</div>\n"
									"</div>\n"
									"</body></html>\n");

    req_flush(req);
}
/* Paul add end */

/* R_UNAUTHORIZED: 401 */
void send_r_unauthorized(request * req, char *realm_name)
{	
    SQUASH_KA(req);
    
    req->response_status = R_UNAUTHORIZED;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 401 Unauthorized\r\n");
        print_http_headers(req);
        req_write(req, "WWW-Authenticate: Basic realm=\"");
        req_write(req, realm_name);
        req_write(req, "\"\r\n");
        // if(req->cookie == NULL)//lee 2006-4-12:set cookie
	if(req->cookie == NULL || req->session == 0)
        {
        	char id[10];
					char head[256],path[256];
					int r;
					FILE *fd;
					
					srand (time(NULL));
					r = rand();
					sprintf (id, "%.8x",r);
					
					sprintf(path,"/var/tmp/%s",id);
					fd = fopen(path,"w");
					if(fd)
						fclose(fd);
					
        	//sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s;expires=Wednesday,1-Jan-2020 00:00:00 GMT\r\n",id);
        	// sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s;\r\n\r\n",id);
		sprintf(head,"Content-Type: " HTML "\r\nSet-Cookie: SESSIONID=%s; path=/\r\n\r\n",id);
		  #ifdef BOA_DEBUG	
          fprintf(stderr,"authorize set cookie:%s\n",head);
		  #endif
          req_write(req,head); /* terminate header */
        }
        else
        	req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>401 Unauthorized</TITLE></HEAD>\n"
                  "<BODY><H1>401 Unauthorized</H1>\nYour client does not "
                  "have permission to get URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " from this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_FORBIDDEN: 403 */
void send_r_forbidden(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_FORBIDDEN;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 403 Forbidden\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req, "<HTML><HEAD><TITLE>403 Forbidden</TITLE></HEAD>\n"
                  "<BODY><H1>403 Forbidden</H1>\nYour client does not "
                  "have permission to get URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " from this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_NOT_FOUND: 404 */
void send_r_not_found(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_FOUND;
#ifdef RTCONFIG_CLOUDSYNC
	// verify invite code
	if(strlen(req->request_uri) > 50){
		char inviteCode[MAX_HEADER_LENGTH + 128];
		sprintf(inviteCode, "<script>location.href='/cgi-bin/cloud_sync.asp?flag=%s';</script>", req->request_uri+1);
		req_write(req, inviteCode);
		req_flush(req);
		return;
	}
#endif
    if (!req->simple) {
        req_write(req, "HTTP/1.0 404 Not Found\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req, "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>\n"
                  "<BODY><H1>404 Not Found</H1>\nThe requested URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " was not found on this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_ERROR: 500 */
void send_r_error(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_ERROR;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 500 Server Error\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>500 Server Error</TITLE></HEAD>\n"
                  "<BODY><H1>500 Server Error</H1>\nThe server encountered "
                  "an internal error and could not complete your request.\n"
                  "</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_NOT_IMP: 501 */
void send_r_not_implemented(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_IMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 501 Not Implemented\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>501 Not Implemented</TITLE></HEAD>\n"
                  "<BODY><H1>501 Not Implemented</H1>\nPOST to non-script "
                  "is not supported in Boa.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_BAD_GATEWAY: 502 */
void send_r_bad_gateway(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_GATEWAY;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 502 Bad Gateway" CRLF);
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML CRLF CRLF); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>502 Bad Gateway</TITLE></HEAD>\n"
                  "<BODY><H1>502 Bad Gateway</H1>\nThe CGI was "
                  "not CGI/1.1 compliant.\n" "</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_SERVICE_UNAVAILABLE: 503 */
void send_r_service_unavailable(request * req) /* 503 */
{
    static char body[] =
        "<HTML><HEAD><TITLE>503 Service Unavailable</TITLE></HEAD>\n"
        "<BODY><H1>503 Service Unavailable</H1>\n"
        "There are too many connections in use right now.\r\n"
        "Please try again later.\r\n</BODY></HTML>\n";
    static int _body_len;
    static char *body_len;

    if (!_body_len)
        _body_len = strlen(body);
    if (!body_len)
        body_len = strdup(simple_itoa(_body_len));
    if (!body_len) {
        log_error_time();
        perror("strdup of _body_len from simple_itoa");
    }


    SQUASH_KA(req);
    req->response_status = R_SERVICE_UNAV;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 503 Service Unavailable\r\n");
        print_http_headers(req);
        if (body_len) {
            req_write(req, "Content-Length: ");
            req_write(req, body_len);
            req_write(req, "\r\n");
        }
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header
                                                               */
    }
    if (req->method != M_HEAD) {
        req_write(req, body);
    }
    req_flush(req);
}


/* R_NOT_IMP: 505 */
void send_r_bad_version(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_VERSION;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 505 HTTP Version Not Supported\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>505 HTTP Version Not Supported</TITLE></HEAD>\n"
                  "<BODY><H1>505 HTTP Version Not Supported</H1>\nHTTP versions "
                  "other than 0.9 and 1.0 "
                  "are not supported in Boa.\n<p><p>Version encountered: ");
        req_write(req, req->http_version);
        req_write(req, "<p><p></BODY></HTML>\n");
    }
    req_flush(req);
}


#ifdef ASUS_LOGIN_SESSION	//Andy Chiu, 2015/11/27.
/*******************************************************************
* NAME: send_r_login_page
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/11/27
* DESCRIPTION: send login page
* INPUT:
* OUTPUT:
* RETURN:
* NOTE: Andy Chiu, 2016/01/04. Modify the redirect web page function.
*	      Andy Chiu, 2016/01/05. Add APP reply
*******************************************************************/
void send_r_login_page(request *req, int error_status, char* url, int lock_time)
{
	if(!req)
	{
		_dprintf("[%s, %d]no req!\n", __FUNCTION__, __LINE__);
		return;
	}
	
	SQUASH_KA(req);
	req->response_status = R_REQUEST_OK;

	char buffer[256]={0};
	char url_tmp[64]={0};
	int flag;
	int fromapp = 0;
	
	if(url)
	{
		if(strstr(url, "Main_Login.asp"))
		{
			strcpy(url_tmp, "Main_Login.asp");
		}
		else
		{
			if(!strncmp(url, "cgi-bin", 7) || !strncmp(url, "/cgi-bin", 8))
				url_tmp[0] = '\0';
			else
				strcpy(url_tmp, "cgi-bin/");
			
			if(url[0] == '/')
				strncat(url_tmp, url + 1, sizeof(url_tmp) - strlen(url_tmp) - 1);
			else
				strncat(url_tmp, url, sizeof(url_tmp) - strlen(url_tmp) - 1);
		}
	}

	req_write(req, "HTTP/1.0 200 OK\r\n");	
	print_http_headers(req);
       	req_write(req, "Content-Type: " HTML "\r\n\r\n"); // terminate header

	//Andy Chiu, 2016/01/05. Check APP
	if(check_client_type(req->header_user_agent) != CLI_TYPE_BROWSER)
	{
			fromapp = 1;		
	}

	if(!fromapp)
	{
		req_write(req, "<meta http-equiv=\"refresh\" content=\"0; url=/Main_Login.asp");

		if(lock_time > 0 || error_status > 0 || url)
		{
			req_write(req, "?");
			flag = 0;
			if(error_status > 0)
			{
				snprintf(buffer, sizeof(buffer), "error_status=%d", error_status);
				req_write(req, buffer);
				flag = 1;
			}

			if(url)
			{
				snprintf(buffer, sizeof(buffer), flag? "&page=/%s": "page=/%s", url_tmp);
				req_write(req, buffer);
				flag = 1;
			}

			if(lock_time > 0)
			{
				snprintf(buffer, sizeof(buffer), flag? "&lock_time=%d": "lock_time=%d", lock_time);
				req_write(req, buffer);
			}
		}
		req_write(req, "\">\n</HEAD></HTML>\n");
	}
	else
	{
		snprintf(buffer, sizeof(buffer), "{\n\"error_status\":\"%d\"\n}\n", error_status);
		req_write(req, buffer);
		if(error_status == LOGINLOCK) {
			snprintf(buffer, sizeof(buffer), ",\"remaining_lock_time\":\"%d\"", MAX_LOGIN_BLOCK_TIME - login_dt);
			req_write(req, buffer);
		}
	}
}

/*******************************************************************
* NAME: send_r_password_page
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/12/29
* DESCRIPTION: send Main_Password page
* INPUT:
* OUTPUT:
* RETURN:
* NOTE:	Andy Chiu, 2016/01/04. Modify the redirect web page function.
*******************************************************************/
void send_r_password_page(request *req, char* url)
{
	if(!req)
	{
		_dprintf("[%s, %d]no req!\n", __FUNCTION__, __LINE__);
		return;
	}
	
	SQUASH_KA(req);
	req->response_status = R_REQUEST_OK;

	char buffer[256]={0};
	char url_tmp[64]={0};
	char *p;

	if(url)
	{
		//compare cgi-bin
		p = strstr(url, "cgi-bin/");
		if(p)
			snprintf(url_tmp, sizeof(url_tmp), "%s", p + strlen("cgi-bin/"));
		else if(url[0] == '/')
			snprintf(url_tmp, sizeof(url_tmp), "%s", url + 1);
		else
			snprintf(url_tmp, sizeof(url_tmp), "%s", url);
	}

	
	req_write(req, "HTTP/1.0 200 OK\r\n");	
	print_http_headers(req);
       	req_write(req, "Content-Type: " HTML "\r\n\r\n"); // terminate header
	req_write(req, "<meta http-equiv=\"refresh\" content=\"0; url=/Main_Password.asp");
	if(url)
	{
		snprintf(buffer, sizeof(buffer), "?page=/%s", url_tmp);
		req_write(req, buffer);
	}
	req_write(req, "\">\n</HEAD></HTML>\n");
}

/*******************************************************************
* NAME: send_redirect_packet_with_token
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/01/06
* DESCRIPTION: send login packet
* INPUT: req: pointer of request; redirect_path: the redirect target path
* OUTPUT:
* RETURN:
* NOTE: 
*******************************************************************/
void send_redirect_packet_with_token(request *req, char *redirect_path)
{
	int fromapp = 0;
	char wp[2048];
	char buff[256] = {0};
	CLIENT_TYPE cli_type;
	asus_token_t *token;
	login_retry_t *retry_tmp = NULL;

	if(!req)
	{
		_dprintf("[%s, %d]no req!\n", __FUNCTION__, __LINE__);
		return;
	}

	retry_tmp = get_login_retry_by_url(req->remote_ip_addr);

	if(!retry_tmp)
	{
		dbgprintf("[%s, %d]ERROR!! Can not judge the remote ip (%s) is WAN or LAN! Drop packets!\n", __FUNCTION__, __LINE__, req->remote_ip_addr);
		return;
	}

	SQUASH_KA(req);	
	req->response_status = R_REQUEST_OK;
	
	//check client type
	cli_type = check_client_type(req->header_user_agent);
	
	//send login header. If error_status is zero, auth is successful.
	
	token = add_asus_token(cli_type, generate_token(buff, sizeof(buff)), req->remote_ip_addr);
	if(!token)
	{
		retry_tmp->err_status = ACCOUNTFAIL;
		send_r_login_page(req, ACCOUNTFAIL, NULL, 0);
		return;
	}
	
	http_login_header(token->token, req, cli_type);

	//Andy Chiu, 2016/01/05. Check APP
	if(cli_type != CLI_TYPE_BROWSER)
	{
			fromapp = 1;		
	}

	//send content
	if(!fromapp)
	{	
		//send content
		req_write(req, "<HTML><HEAD>\n");

		if(redirect_path)
		{
			decode_uri(redirect_path);

			//Andy Chiu, 2016/01/04. Modify the redirect page function without java script for security issue.
			snprintf(wp, sizeof(wp), "<meta http-equiv=\"refresh\" content=\"0; url=%s\">\n", redirect_path);
			req_write(req, wp);
		}
		req_write(req, "</HEAD></HTML>\n");
	}
	else
	{
		snprintf(wp, sizeof(wp), "{\n\"asus_token\":\"%s\"\n}\n", token->token);
		req_write(req, wp);
	}
}

/*******************************************************************
* NAME: send_r_qis_page
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/12/29
* DESCRIPTION: send QIS page
* INPUT:
* OUTPUT:
* RETURN:
* NOTE:	Andy Chiu, 2016/01/04. Modify the redirect web page function.
*******************************************************************/
void send_r_redirect_page(request *req, const int with_token)
{
	if(!req)
	{
		_dprintf("[%s, %d]no req!\n", __FUNCTION__, __LINE__);
		return;
	}
	
	SQUASH_KA(req);		
	char *p = req->request_uri;
	char tmp[512];

	//remove /cgi-bin in path
	req->response_status = R_REQUEST_OK;
	if(!strncmp(p, "/cgi-bin", strlen("/cgi-bin")))
		p += strlen("/cgi-bin");	

	if(with_token)
	{
		send_redirect_packet_with_token(req, p);
	}
	else
	{
		req_write(req, "HTTP/1.0 200 OK\r\n");	
		print_http_headers(req);
	       	req_write(req, "Content-Type: " HTML "\r\n\r\n"); // terminate header
		snprintf(tmp, sizeof(tmp), "<meta http-equiv=\"refresh\" content=\"0; url=%s", p);	       	
		req_write(req, tmp);
		req_write(req, "\">\n</HEAD></HTML>\n");
	}
}
#endif

