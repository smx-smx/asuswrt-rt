/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,97 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996 Charles F. Randall <crandall@goldsys.com>
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

/* $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/boa-asp/src/util.c#1 $ */

#include "boa.h"
#include <stdarg.h>
#include <shared.h>
#include <tcapi.h>

extern char lan_ip[20], lan_netmask[20];


#define HEX_TO_DECIMAL(char1, char2)	\
    (((char1 >= 'A') ? (((char1 & 0xdf) - 'A') + 10) : (char1 - '0')) * 16) + \
    (((char2 >= 'A') ? (((char2 & 0xdf) - 'A') + 10) : (char2 - '0')))

const char month_tab[48] =
    "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";
const char day_tab[] = "Sun,Mon,Tue,Wed,Thu,Fri,Sat,";

/*
 * Name: clean_pathname
 *
 * Description: Replaces unsafe/incorrect instances of:
 *  //[...] with /
 *  /./ with /
 *  /../ with / (technically not what we want, but browsers should deal
 *   with this, not servers)
 */

void clean_pathname(char *pathname)
{
    char *cleanpath, c;

    cleanpath = pathname;
    while ((c = *pathname++)) {
        if (c == '/') {
            while (1) {
                if (*pathname == '/')
                    pathname++;
                else if (*pathname == '.' && *(pathname + 1) == '/')
                    pathname += 2;
                else if (*pathname == '.' && *(pathname + 1) == '.' &&
                         *(pathname + 2) == '/') {
                    pathname += 3;
                } else
                    break;
            }
            c = '/';
        }
        *cleanpath++ = c;
    }

    *cleanpath = '\0';
}

/*
 * Name: get_commonlog_time
 *
 * Description: Returns the current time in common log format in a static
 * char buffer.
 *
 * commonlog time is exactly 25 characters long
 * because this is only used in logging, we add " [" before and "] " after
 * making 29 characters
 * "[27/Feb/1998:20:20:04 +0000] "
 *
 * Constrast with rfc822 time:
 * "Sun, 06 Nov 1994 08:49:37 GMT"
 *
 * Altered 10 Jan 2000 by Jon Nelson ala Drew Streib for non UTC logging
 *
 */

char *get_commonlog_time(void)
{
    struct tm *t;
    char *p;
    unsigned int a;
    static char buf[30];
    int time_offset;

    if (use_localtime) {
        t = localtime(&current_time);
        time_offset = TIMEZONE_OFFSET(t);
    } else {
        t = gmtime(&current_time);
        time_offset = 0;
    }

    p = buf + 29;
    *p-- = '\0';
    *p-- = ' ';
    *p-- = ']';
    a = abs(time_offset / 60);
    *p-- = '0' + a % 10;
    a /= 10;
    *p-- = '0' + a % 6;
    a /= 6;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = (time_offset >= 0) ? '+' : '-';
    *p-- = ' ';

    a = t->tm_sec;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_min;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_hour;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = 1900 + t->tm_year;
    while (a) {
        *p-- = '0' + a % 10;
        a /= 10;
    }
    /* p points to an unused spot */
    *p-- = '/';
    p -= 2;
    memcpy(p--, month_tab + 4 * (t->tm_mon), 3);
    *p-- = '/';
    a = t->tm_mday;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p = '[';
    return p;                   /* should be same as returning buf */
}

/*
 * Name: month2int
 *
 * Description: Turns a three letter month into a 0-11 int
 *
 * Note: This function is from wn-v1.07 -- it's clever and fast
 */

int month2int(char *monthname)
{
    switch (*monthname) {
    case 'A':
        return (*++monthname == 'p' ? 3 : 7);
    case 'D':
        return (11);
    case 'F':
        return (1);
    case 'J':
        if (*++monthname == 'a')
            return (0);
        return (*++monthname == 'n' ? 5 : 6);
    case 'M':
        return (*(monthname + 2) == 'r' ? 2 : 4);
    case 'N':
        return (10);
    case 'O':
        return (9);
    case 'S':
        return (8);
    default:
        return (-1);
    }
}

/*
 * Name: modified_since
 * Description: Decides whether a file's mtime is newer than the
 * If-Modified-Since header of a request.
 *

 Sun, 06 Nov 1994 08:49:37 GMT    ; RFC 822, updated by RFC 1123
 Sunday, 06-Nov-94 08:49:37 GMT   ; RFC 850, obsoleted by RFC 1036
 Sun Nov  6 08:49:37 1994         ; ANSI C's asctime() format
 31 September 2000 23:59:59 GMT   ; non-standard

 * RETURN VALUES:
 *  0: File has not been modified since specified time.
 *  1: File has been.
 * -1: Error!
 */

int modified_since(time_t * mtime, char *if_modified_since)
{
    struct tm *file_gmt;
    char *ims_info;
    char monthname[10 + 1];
    int day, month, year, hour, minute, second;
    int comp;

    ims_info = if_modified_since;
    while (*ims_info != ' ' && *ims_info != '\0')
        ++ims_info;
    if (*ims_info != ' ')
        return -1;

    /* the pre-space in the third scanf skips whitespace for the string */
    if (sscanf(ims_info, "%d %3s %d %d:%d:%d GMT", /* RFC 1123 */
               &day, monthname, &year, &hour, &minute, &second) == 6);
    else if (sscanf(ims_info, "%d-%3s-%d %d:%d:%d GMT", /* RFC 1036 */
                    &day, monthname, &year, &hour, &minute, &second) == 6)
        year += 1900;
    else if (sscanf(ims_info, " %3s %d %d:%d:%d %d", /* asctime() format */
                    monthname, &day, &hour, &minute, &second, &year) == 6);
    /*  allow this non-standard date format: 31 September 2000 23:59:59 GMT */
    /* NOTE: Use if_modified_since here, because the date *starts*
     *       with the day, versus a throwaway item
     */
    else if (sscanf(if_modified_since, "%d %10s %d %d:%d:%d GMT",
                    &day, monthname, &year, &hour, &minute, &second) == 6);
    else {
        log_error_time();
        fprintf(stderr, "Error in %s, line %d: Unable to sscanf \"%s\"\n",
                __FILE__, __LINE__, ims_info);
        return -1;              /* error */
    }

    file_gmt = gmtime(mtime);
    month = month2int(monthname);

    /* Go through from years to seconds -- if they are ever unequal,
     we know which one is newer and can return */

    if ((comp = 1900 + file_gmt->tm_year - year))
        return (comp > 0);
    if ((comp = file_gmt->tm_mon - month))
        return (comp > 0);
    if ((comp = file_gmt->tm_mday - day))
        return (comp > 0);
    if ((comp = file_gmt->tm_hour - hour))
        return (comp > 0);
    if ((comp = file_gmt->tm_min - minute))
        return (comp > 0);
    if ((comp = file_gmt->tm_sec - second))
        return (comp > 0);

    return 0;                   /* this person must really be into the latest/greatest */
}


/*
 * Name: to_upper
 *
 * Description: Turns a string into all upper case (for HTTP_ header forming)
 * AND changes - into _
 */

char *to_upper(char *str)
{
    char *start = str;

    while (*str) {
        if (*str == '-')
            *str = '_';
        else
            *str = toupper(*str);

        str++;
    }

    return start;
}

/*
 * Name: unescape_uri
 *
 * Description: Decodes a uri, changing %xx encodings with the actual
 * character.  The query_string should already be gone.
 *
 * Return values:
 *  1: success
 *  0: illegal string
 */

int unescape_uri(char *uri, char ** query_string)
{
    char c, d;
    char *uri_old;

    uri_old = uri;

    while ((c = *uri_old)) {
        if (c == '%') {
            uri_old++;
            if ((c = *uri_old++) && (d = *uri_old++))
                *uri++ = HEX_TO_DECIMAL(c, d);
            else
                return 0;       /* NULL in chars to be decoded */
        } else if (c == '?') { /* query string */
            if (query_string)
                *query_string = ++uri_old;
            /* stop here */
            *uri++ = '\0';
            //return(1);
            //break;		//continue to process uri decode. Sam, 2013/7/4
        } else if (c == '#') { /* fragment */
            /* legal part of URL, but we do *not* care.
             * However, we still have to look for the query string */
            if (query_string) {
                ++uri_old;
                while((c = *uri_old)) {
                    if (c == '?') {
                        *query_string = ++uri_old;
                        break;
                    }
                    ++uri_old;
                }
            }
            break;
        } else {
            *uri++ = c;
            uri_old++;
        }
    }

    *uri = '\0';
    return 1;
}

/* rfc822 (1123) time is exactly 29 characters long
 * "Sun, 06 Nov 1994 08:49:37 GMT"
 */

void rfc822_time_buf(char *buf, time_t s)
{
    struct tm *t;
    char *p;
    unsigned int a;

    if (!s) {
        t = gmtime(&current_time);
    } else
        t = gmtime(&s);

    p = buf + 28;
    /* p points to the last char in the buf */

    p -= 3;
    /* p points to where the ' ' will go */
    memcpy(p--, " GMT", 4);

    a = t->tm_sec;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_min;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_hour;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ' ';
    a = 1900 + t->tm_year;
    while (a) {
        *p-- = '0' + a % 10;
        a /= 10;
    }
    /* p points to an unused spot to where the space will go */
    p -= 3;
    /* p points to where the first char of the month will go */
    memcpy(p--, month_tab + 4 * (t->tm_mon), 4);
    *p-- = ' ';
    a = t->tm_mday;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ' ';
    p -= 3;
    memcpy(p, day_tab + t->tm_wday * 4, 4);
}

char *simple_itoa(unsigned int i)
{
    /* 21 digits plus null terminator, good for 64-bit or smaller ints
     * for bigger ints, use a bigger buffer!
     *
     * 4294967295 is, incidentally, MAX_UINT (on 32bit systems at this time)
     * and is 10 bytes long
     */
    static char local[22];
    char *p = &local[21];
    *p-- = '\0';
    do {
        *p-- = '0' + i % 10;
        i /= 10;
    } while (i > 0);
    return p + 1;
}

/* I don't "do" negative conversions
 * Therefore, -1 indicates error
 */

int boa_atoi(char *s)
{
    int retval;
    char *reconv;

    if (!isdigit(*s))
        return -1;

    retval = atoi(s);
    if (retval < 0)
        return -1;

    reconv = simple_itoa(retval);
    if (memcmp(s,reconv,strlen(s)) != 0) {
        return -1;
    }
    return retval;
}

int create_temporary_file(short want_unlink, char *storage, int size)
{
    static char boa_tempfile[MAX_PATH_LENGTH + 1];
    int fd;

#ifndef TRENDCHIP
    snprintf(boa_tempfile, MAX_PATH_LENGTH,
             "%s/boa-temp.XXXXXX", tempdir);

#else
    snprintf(boa_tempfile, MAX_PATH_LENGTH,
                 "%s/boa-temp", tempdir);
#endif
    /* open temp file */
#ifndef TRENDCHIP
    fd = mkstemp(boa_tempfile);
#else
    unlink(boa_tempfile);
    fd = open(boa_tempfile,O_RDWR | O_CREAT);
#endif
    if (fd == -1) {
        log_error_time();
        perror("mkstemp");
        return 0;
    }

    if (storage != NULL) {
        int len = strlen(boa_tempfile);

        if (len < size) {
            memcpy(storage, boa_tempfile, len + 1);
        } else {
            close(fd);
            fd = 0;
            log_error_time();
            fprintf(stderr, "not enough memory for memcpy in storage\n");
            want_unlink = 1;
        }
    }

#ifndef TRENDCHIP
    if (want_unlink) {
        if (unlink(boa_tempfile) == -1) {
            close(fd);
            fd = 0;
            log_error_time();
            fprintf(stderr, "unlink temp file\n");
        }
    }
#endif

    return (fd);
}

/*
 * Name: normalize_path
 *
 * Description: Makes sure relative paths are made absolute
 *
 */

#define DIRBUF_SIZE MAX_PATH_LENGTH * 2 + 1
char * normalize_path(char *path)
{
    char dirbuf[DIRBUF_SIZE];
    int len1, len2;
    char *endpath;

    if (path[0] == '/') {
        endpath = strdup(path);
    } else {

#ifndef HAVE_GETCWD
        perror("boa: getcwd() not defined. Aborting.");
        exit(1);
#endif
        if (getcwd(dirbuf, DIRBUF_SIZE) == NULL) {
            if (errno == ERANGE)
                perror
                    ("boa: getcwd() failed - unable to get working directory. "
                     "Aborting.");
            else if (errno == EACCES)
                perror("boa: getcwd() failed - No read access in current "
                       "directory. Aborting.");
            else
                perror("boa: getcwd() failed - unknown error. Aborting.");
            exit(1);
        }

        /* OK, now the hard part. */
        len1 = strlen(dirbuf);
        len2 = strlen(path);
        if (len1 + len2 > MAX_PATH_LENGTH * 2) {
            perror("boa: eek. unable to normalize pathname");
            exit(1);
        }
        if (strcmp(path,".") != 0) {
            memcpy(dirbuf + len1, "/", 1);
            memcpy(dirbuf + len1 + 1, path, len2 + 1);
        }
        /* fprintf(stderr, "boa: normalize gets \"%s\"\n", dirbuf); */

        endpath = strdup(dirbuf);
    }

    if (endpath == NULL) {
        fprintf(stderr,
                "boa: Cannot strdup path. Aborting.\n");
        exit(1);
    }
    return endpath;
}

int real_set_block_fd(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        return -1;

    flags &= ~O_NONBLOCK;
    flags = fcntl(fd, F_SETFL, flags);
    return flags;
}

int real_set_nonblock_fd(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        return -1;

    flags |= O_NONBLOCK;
    flags = fcntl(fd, F_SETFL, flags);
    return flags;
}

unsigned char get_rand()
{
	unsigned char buf[1];
	FILE *fp;

	fp = fopen("/dev/urandom", "r");
	if (fp == NULL) {
		return 0;
	}
	fread(buf, 1, 1, fp);
	fclose(fp);

	return buf[0];
}

unsigned long readFileSize( char *filepath )
{
	struct stat sb;

	if( stat(filepath, &sb) == 0 )
	{
		//on success, return file size
		return sb.st_size;
	}
	return 0;

}

//Ren.B
#define ALIGN_SIZE 1024
#define DEFAULT_HEADER "DSL-N66U"

//must be the same structure in /apps/private/cfg_manager/utility.c
typedef struct romfile_header_s
{
	char productName[16];
	char keyWord[16];  //encryptRomfile
	unsigned long fileLength;
	unsigned int rand;
}romfile_header_t;

//must be the same keyword in /apps/private/cfg_manager/utility.c
#define KEYWORD "EnCrYpTRomFIle"

/**
** src : source file path, ex: /tmp/var/romfile.cfg
** dst : destination file path, ex: /tmp/var/romfile_encrypt.cfg
** productName : the encrypted romfile header, string type.
** Encrypt the src file to the dst file, the src file will not be changed.
**/
int encryptRomfile(char *src, char *dst, char *productName)
{
	FILE *fp = NULL;
	unsigned long count, filelen, i;
	unsigned int rand = 0;
	char *buffer = NULL;
	int srcFD = -1;
	int ret = 0;
	romfile_header_t rhdr;

	unlink(dst);
	if ((fp = fopen(dst, "w")) == NULL) return -1;

	if( ( srcFD = open(src, O_RDONLY) ) < 0 )
	{
		return -2;
	}
	count = readFileSize(src);
	buffer = (char *) calloc( count, sizeof(char));
	
	ret = read(srcFD, buffer, count);
	close(srcFD);
	if( ret < 0 )
	{
		free(buffer);
		fclose(fp);
		return -3;
	}

	memset(&rhdr, 0, sizeof(rhdr));
	strcpy(rhdr.keyWord, KEYWORD);
	rand = (get_rand() % 15) + 15; //rand will be 15~29
	rhdr.rand = rand;

	//currently ROMFILE_PADDING is not needed. We keep it just in case in the future.
	#if defined(ROMFILE_PADDING)
	filelen = count + (ALIGN_SIZE - (count%ALIGN_SIZE));
	#else
	filelen = count;
	#endif
	rhdr.fileLength = filelen;
	
	if((!productName)||( strlen(productName) <= 0 ))
	{
		strncpy(rhdr.productName, DEFAULT_HEADER, sizeof(rhdr.productName) );
	}
	else
	{
		strncpy(rhdr.productName, productName, sizeof(rhdr.productName) );
		rhdr.productName[sizeof(rhdr.productName)-1] = '\0';
	}

	//write header
	fwrite(&rhdr, 1, sizeof(rhdr), fp);

	//encrypt data
	for (i = 0; i < count; i++)
	{
		if (buffer[i] == 0x0)
			buffer[i] = 0xfd + get_rand() % 3;
		else
			buffer[i] = (0xff - buffer[i] + rand) & 0xff;
	}

	//write data
	fwrite(buffer, 1, count, fp);

	#if defined(ROMFILE_PADDING)
	//paddings
	for (i = count; i < filelen; i++)
	{
		temp = 0xfd + get_rand() % 3;
		fwrite(&temp, 1, 1, fp);
	}
	#endif
	
	fclose(fp);
	free(buffer);
	return 0;
}
//Ren.E

//it shows debug messages on console.
void dbgprintf (const char * format, ...)
{
	FILE *f;
	int nfd;
	va_list args;
	/* Don't let /dev/console block */
	if((nfd = open("/dev/console", O_WRONLY | O_NONBLOCK)) > 0){
		if((f = fdopen(nfd, "w")) != NULL){
			va_start(args, format);
			vfprintf(f, format, args);
			va_end(args);
			fclose(f);
		}
		close(nfd);
	}
}

#ifdef ASUS_LOGIN_SESSION

void dump_token(asus_token_t *token)
{
	if(token)
	{
		dbgprintf("type=%d\n", token->client_type);
		dbgprintf("ipaddr=%s\n", token->ipaddr);
		dbgprintf("token=%s\n", token->token);
		dbgprintf("login_timestamp=%ld\n", token->login_timestamp);
	}
}

static void _dump_all_token_list()
{
	asus_token_t *token = token_head;
	int i;

	for(i = 0; token; ++i)
	{
		dbgprintf("\nindex=%d\n", i);
		dump_token(token);
		token = token->next;
	}
}

/*******************************************************************
* NAME: _generate_token
* AUTHOR: Andy Chiu
* CREATE DATE: 2015/12/01
* DESCRIPTION: generate token
* INPUT: token: the buffer to store token. 
*            len: the size of token buffer
* OUTPUT: the pointer of token
* RETURN:
* NOTE: Andy Chiu, 2016/02/25. modify for asus_token_t
*******************************************************************/
char *generate_token(char *token, int len)
{
	int i, num;
	const char token_contain[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
		, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
		, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

	if(!token || len < ASUS_TOKEN_LEN)
	{
		return NULL;
	}

	memset(token,0, len);

	for(i = 0; i < ASUS_TOKEN_LEN - 1; ++i)
	{
		num = get_rand() % 62;
		token[i] = token_contain[num];
	}
	//dbgprintf("[%s, %d]generate token=%s\n", __FUNCTION__, __LINE__, token);
	return token;
}


/*******************************************************************
* NAME: check_client_type
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/02/25
* DESCRIPTION: check the client type by user_agent in http header
* INPUT: user_agent: the user agent in http header.
* OUTPUT:
* RETURN: CLIENT_TYPE
* NOTE:	2017/2/23. Andy Chiu. Add IFTTT type.
*******************************************************************/
CLIENT_TYPE check_client_type(const char *user_agent)
{
	if(user_agent && strstr(user_agent, ASUSROUTER_TAG))
	{
		if(strstr(user_agent, ASUSROUTER_APP))
			return CLI_TYPE_APP;
		else if(strstr(user_agent, ASUSROUTER_ATE))
			return CLI_TYPE_ATE;		
		else if(strstr(user_agent, ASUSROUTER_ASSIA))
			return CLI_TYPE_ASSIA;	
		else if(strstr(user_agent, ASUSROUTER_IFTTT))
			return CLI_TYPE_IFTTT;	
		
	}
	return CLI_TYPE_BROWSER;
}

/*******************************************************************
* NAME: rm_token_in_list
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/02/25
* DESCRIPTION: remove the items in the list which match the conditions
* INPUT: ipaddr: a string of ip address. 
*            token: a string of token. 
*            timeout: 0, won't do anything. otherwise, check timeout in the list.
* OUTPUT:
* RETURN: 
* NOTE:	
*******************************************************************/
void rm_token_in_list(const char *ipaddr, const char *token, const int timeout)
{
	asus_token_t *ptr = token_head, *prev = NULL, *temp = NULL;
	char tmp[256] = {0};
	int logout_time = 0, find = 0;
	long now;

	//get timeout time
	tcapi_get("Misc_Entry", "http_autologout", tmp);
	logout_time = atoi(tmp) * 60;

	while(ptr)
	{
		//dump_token(ptr);
		find = 0;
		
		//check ip
		if(ipaddr && !strcmp(ptr->ipaddr, ipaddr))
		{
			find = 1;
		}

		//check token
		if(token && !strcmp(ptr->token, token))
		{
			find = 1;
		}

		//check timeout
		if(timeout)
		{
			now = uptime();
			if(ptr->client_type == CLI_TYPE_BROWSER)
			{
				if(logout_time > 0 && (now - ptr->login_timestamp > logout_time))
				{
					find = 1;
				}
			}
			else	//not from browser, the logout time is defined in boa.h
			{
				if(now - ptr->login_timestamp > APP_LOGOUT_TIME)
				{
					find = 1;
				}				
			}
		}

		if(find)
		{
			if(prev)
				prev->next = ptr->next;
			temp = ptr;
			if(ptr == token_head)
				token_head = ptr->next;
			ptr = ptr->next;
			free(temp);			
		}
		else
		{
			prev = ptr;
			ptr = ptr->next;
		}
	}
}

/*******************************************************************
* NAME: search_token_list
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/02/25
* DESCRIPTION: find the asus_token_t item in the list
* INPUT:
* OUTPUT:
* RETURN: the pointer of matched item.
* NOTE:	
*******************************************************************/
asus_token_t* search_token_list(const char *token, const char *ipaddr, CLIENT_TYPE cli_type)
{
	asus_token_t *ptr = token_head;

	while(ptr)
	{
		//dump_token(ptr);
		
		if(token && !strcmp(ptr->token, token))
			return ptr;

		if(ipaddr && !strcmp(ptr->ipaddr, ipaddr))
			return ptr;

		if(ptr->client_type == cli_type)
			return ptr;
		ptr = ptr->next;
	}
	return NULL;
}

/*******************************************************************
* NAME: add_asus_token
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/02/25
* DESCRIPTION: add asus_token in the list
* INPUT:
* OUTPUT:
* RETURN: pointer of asus_token_t
* NOTE:	
*******************************************************************/
asus_token_t* add_asus_token(const CLIENT_TYPE cli_type, const char *token, const char *ipaddr)
{
	asus_token_t *new_token;
	
	if(!token || !ipaddr)
		return NULL;

	//check timeout items in the list.
	RM_TOKEN_ITEM_BY_TIMEOUT();
	
	//create new item.
	new_token = (asus_token_t*)malloc(sizeof(asus_token_t));
	if(!new_token)
		return NULL;

	//init new item
	memset(new_token, 0, sizeof(asus_token_t));
	new_token->client_type = cli_type;
	snprintf(new_token->ipaddr, sizeof(new_token->ipaddr), ipaddr);
	snprintf(new_token->token, sizeof(new_token->token), token);
	new_token->login_timestamp = uptime();

	//add in the list
	new_token->next = token_head;
	token_head = new_token;

	//_dump_all_token_list();
	
	return new_token;
}

#ifdef USE_RETRY_LIST
void dump_retry(login_retry_t *retry)
{
	if(retry)
	{
		dbgprintf("[%s, %d]\n", __FUNCTION__, __LINE__);
		dbgprintf("ipaddr=%s\n", retry->ipaddr);
		dbgprintf("try_num=%d\n", retry->try_num);
		dbgprintf("last_login_timestamp=%ld\n", retry->last_login_timestamp);
	}
}

/*******************************************************************
* NAME: rm_token_in_list
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/02/25
* DESCRIPTION: remove the items in the list which match the conditions
* INPUT: ipaddr: a string of ip address. 
*            token: a string of token. 
*            timeout: 0, won't do anything. otherwise, check timeout in the list.
* OUTPUT:
* RETURN: 
* NOTE:	
*******************************************************************/
void rm_retry_in_list(const char *ipaddr)
{
	login_retry_t *ptr = retry_head, *prev = NULL, *tmp = NULL;

  	if(!ipaddr)
		return;
	
	while(ptr)
	{
		//check ip
		if(ipaddr && !strcmp(ptr->ipaddr, ipaddr))
		{
			if(prev)
				prev->next = ptr->next;
			tmp = ptr;
			if(ptr == retry_head)
				retry_head = ptr->next;
			ptr = ptr->next;
			free(tmp);			
 		}
		else
		{
			prev = ptr;
			ptr = ptr->next;
		}
	}
}

/*******************************************************************
* NAME: search_token_list
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/02/25
* DESCRIPTION: find the asus_token_t item in the list
* INPUT:
* OUTPUT:
* RETURN: the pointer of matched item.
* NOTE:	
*******************************************************************/
login_retry_t* search_retry_list(const char *ipaddr)
{
	login_retry_t *ptr = retry_head;
	if(!ipaddr)
		return NULL;

	while(ptr)
	{
		if(!strcmp(ptr->ipaddr, ipaddr))
		{
			//dump_retry(ptr);
			return ptr;
		}
		ptr = ptr->next;
	}
	return NULL;
}

/*******************************************************************
* NAME: add_asus_token
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/02/25
* DESCRIPTION: add asus_token in the list
* INPUT:
* OUTPUT:
* RETURN: pointer of asus_token_t
* NOTE:	
*******************************************************************/
login_retry_t* add_login_retry(const char *ipaddr)
{
	login_retry_t *new_retry;
	
	if(!ipaddr)
		return NULL;

	//create new item.
	new_retry = (login_retry_t*)malloc(sizeof(login_retry_t));
	if(!new_retry)
		return NULL;

	//init new item
	memset(new_retry, 0, sizeof(login_retry_t));
	snprintf(new_retry->ipaddr, sizeof(new_retry->ipaddr), ipaddr);
	new_retry->last_login_timestamp = uptime();

	//add in the list
	new_retry->next = retry_head;
	retry_head = new_retry;

	return new_retry;
}
#endif
#endif

//return value: 0: LAN,  1: WAN,  -1: ERROR
int _check_ip_is_lan_or_wan(const char *target_ip)
{
	char tmp1[20], tmp2[20];

	if(!target_ip)
		return -1;

	//Convert target ip and lan ip.
	//ex. target ip is 168.95.10.10, lan ip is 192.168.1.1, subnet mask is 255.255.255.0. 
	//convert them as 168.95.10.0 and 192.168.1.0. Them compare these 2 values.
	//If they are different, the target ip would be WAN.
	if(get_network_addr_by_ip_prefix(target_ip, lan_netmask, tmp1, sizeof(tmp1)) == -1)
		return -1;

	if(get_network_addr_by_ip_prefix(lan_ip, lan_netmask, tmp2, sizeof(tmp2)) == -1)
		return -1;

	return !strcmp(tmp1, tmp2)? 0: 1;
}

//return value: 0: LAN,  1: WAN,  -1: ERROR
int check_current_ip_is_lan_or_wan(const char *target_ip)
{
	if(!target_ip)
		return -1;

	return _check_ip_is_lan_or_wan(target_ip);
}

int check_xss_blacklist(char* para, int check_www)
{
	int i = 0;
	int file_len;
	char *query, *para_t;
	char para_str[256];
	char filename[128];
	char url_str[128];
	memset(filename, 0, sizeof(filename));
	memset(para_str, 0, sizeof(para_str));

	if(para == NULL || !strcmp(para, "")){
		//_dprintf("check_xss_blacklist: para is NULL\n");
		return 1;
	}

	para_t = strdup(para);
	while(*para) {
		//if(*para=='<' || *para=='>' || *para=='%' || *para=='/' || *para=='(' || *para==')' || *para=='&') {
		if(*para=='<' || *para=='>' || *para=='%' || *para=='(' || *para==')' || *para=='&') {
			//_dprintf("check_xss_blacklist: para is Invalid\n");
			free(para_t);
			return 1;
		}
		else {
			para_str[i] = tolower(*para);
			i++;
			para++;
		}
	}

	if(strstr(para_str, "script") || strstr(para_str, "//") ){
		//_dprintf("check_xss_blacklist: para include script\n");
		free(para_t);
		return 1;
	}

	if(check_www == 1){
		memset(url_str, 0, sizeof(url_str));
		if ((query = index(para_t, '?')) != NULL) {
			file_len = strlen(para_t)-strlen(query);

			if(file_len > sizeof(url_str))
				file_len = sizeof(url_str);

			strncpy(url_str, para_t, file_len);
		}
		else
		{
			strncpy(url_str, para_t, sizeof(url_str)-1);
		}

		snprintf(filename, sizeof(filename), "/www/%s", url_str);
		if(!check_if_file_exist(filename)){
			//dbgprintf("check_xss_blacklist:%s is not in www\n", url_str);
			free(para_t);
			return 1;
		}
	}

	free(para_t);
	return 0;
}

login_retry_t* get_login_retry_by_url(const char* remote_ip)
{
	int ip_type;
	
	if(!remote_ip)
		return NULL;

	ip_type = check_current_ip_is_lan_or_wan(remote_ip);
	if(ip_type == 0)
	{
		return  (&retry_flag);
	}
	else if(ip_type == 1)
	{
		return (&retry_flag_wan);
	}
	else
	{
		return NULL;
	}
}

