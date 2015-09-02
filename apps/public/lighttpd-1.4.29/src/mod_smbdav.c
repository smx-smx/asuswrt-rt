#include "base.h"
#include "log.h"
#include "buffer.h"
#include "response.h"

#include "plugin.h"

#include "stream.h"
#include "stat_cache.h"

#include "sys-mmap.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#include <unistd.h>
#include <dirent.h>

#include <sys/shm.h>
#include <signal.h>
#include <sys/vfs.h>

#if EMBEDDED_EANBLE 
#ifndef APP_IPKG
#include <utils.h>
#include <shutils.h>
#include <shared.h>
#endif
#include "nvram_control.h"
#if HAVE_PUSHLOG
#include <push_log.h>
#endif
#endif

#include <curl/curl.h>
#include <openssl/md5.h>

int shmid;
void* shared_memory = (void*)0;

#ifdef SOURCEINSIGH
	server *srv;
	connection *con;
	void *p_d;
#endif

#if defined(HAVE_LIBXML_H) && defined(HAVE_SQLITE3_H)
#define USE_PROPPATCH
#include <libxml/tree.h>
#include <libxml/parser.h>

#include <sqlite3.h>
#endif

#if 0
#define HAVE_ICONV_H
#include <iconv.h>
#define MAX_ICONV_BUF 2048

typedef enum {
	ICONV_OK,
	ICONV_TRYNEXT,
	ICONV_FATAL
} iconv_result;
#endif
/*
#if defined(HAVE_LIBXML_H) && defined(HAVE_SQLITE3_H) && defined(HAVE_UUID_UUID_H)
#define USE_LOCKS
include <uuid/uuid.h>
#endif
*/

#if defined(HAVE_LIBXML_H) && defined(HAVE_SQLITE3_H)
#define USE_LOCKS
#endif

// smb client api header
#include <unistd.h> 

#include <dirent.h>

#include "libsmbclient.h"
#include "mod_smbdav.h"

#define DBG_ENABLE_MOD_SMBDAV 1
#define DBE	DBG_ENABLE_MOD_SMBDAV

//#define SCAN_MASTER_BROWSER
time_t prv_ts = 0;

unsigned long prev_total_0 = 0;
unsigned long prev_user_0 = 0;
unsigned long prev_total_1 = 0;
unsigned long prev_user_1 = 0;

extern handler_t ntlm_authentication_handler(server *srv, connection *con, plugin_data *p);
extern handler_t basic_authentication_handler(server *srv, connection *con, plugin_data *p);

#if 0
static iconv_result
do_iconv(const char* to_ces, const char* from_ces,
	 char *inbuf,  size_t inbytesleft,
	 char *outbuf_orig, size_t outbytesleft_orig)
{
#ifdef HAVE_ICONV_H
	size_t rc;
	iconv_result ret = ICONV_OK;

	size_t outbytesleft = outbytesleft_orig - 1;
	char* outbuf = outbuf_orig;

	iconv_t cd  = iconv_open(to_ces, from_ces);

	if(cd == (iconv_t)-1)
	{
		return ICONV_FATAL;
	}
	rc = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
	//Cdbg(1, "outbuf=%s", outbuf);
	if(rc == (size_t)-1)
	{
		Cdbg(1, "fail %s", strerror(errno));
		if(errno == E2BIG)
		{
			ret = ICONV_FATAL;
		}
		else
		{
			ret = ICONV_TRYNEXT;
			memset(outbuf_orig, '\0', outbytesleft_orig);
		}
	}
	iconv_close(cd);

	return ret;
#else // HAVE_ICONV_H
	return ICONV_FATAL;
#endif // HAVE_ICONV_H
}
#endif

size_t static curl_write_callback_func(void *ptr, size_t size, size_t count, void *stream)
{
   	/* ptr - your string variable.
     	stream - data chuck you received */
	char **response_ptr =  (char**)stream;

    /* assuming the response is a string */
    *response_ptr = strndup(ptr, (size_t)(size *count));
	
  	//printf("%.*s", size, (char*)stream));
   	//Cdbg(1, "callback_func.... %s", (char*)ptr);
}
#if 0
/* md5sum:
 * Concatenates a series of strings together then generates an md5
 * message digest. Writes the result directly into 'output', which
 * MUST be large enough to accept the result. (Size ==
 * 128 + null terminator.)
 */
char secret[100] = "804b51d14d840d6e";
/* sprint_hex:
 * print a long hex value into a string buffer. This function will
 * write 'len' bytes of data from 'char_buf' into 2*len bytes of space
 * in 'output'. (Each hex value is 4 bytes.) Space in output must be
 * pre-allocated. */
void sprint_hex(char* output, unsigned char* char_buf, int len)
{
	int i;
	for (i=0; i < len; i++) {
		sprintf(output + i*2, "%02x", char_buf[i]);
	}
}

void md5sum(char* output, int counter, ...)
{
	va_list argp;
	unsigned char temp[MD5_DIGEST_LENGTH];
	char* md5_string;
	int full_len;
	int i, str_len;
	int buffer_size = 10;

	md5_string = (char*)malloc(buffer_size);
	md5_string[0] = '\0';

	full_len = 0;
	va_start(argp, secret);
	for (i = 0; i < counter; i++) {
		char* string = va_arg(argp, char*);
		int len = strlen(string);

		/* If the buffer is not large enough, expand until it is. */
		while (len + full_len > buffer_size - 1) {
			buffer_size += buffer_size;
			md5_string = realloc(md5_string, buffer_size);
		}

		strncat(md5_string, string, len);

		full_len += len;
		md5_string[full_len] = '\0';
	}
	va_end(argp);

	str_len = strlen(md5_string);
	MD5((const unsigned char*)md5_string, (unsigned int)str_len, temp);

	free(md5_string);

	sprint_hex(output, temp, MD5_DIGEST_LENGTH);	
	output[129] = '\0';
}
#endif
static int is_host2(char *uri)
{
	char pWorkgroup[30]={0};
	char pServer[64]={0};
	char pShare[128]={0};
	char pPath[128]={0};
	smbc_parse_path(uri, pWorkgroup, pServer, pShare, pPath);

	struct cli_state *cli;

	if(pServer[0] == '\0')
		return 0;
	
	cli = smbc_cli_initialize();

	uint32_t res = smbc_cli_connect(cli, pServer, SMB_PORT);
	if(res) {
		res = smbc_cli_connect(cli, pServer, NMB_PORT);
	}
Cdbg(DBE, "%s can %sbe connected..", uri, (res) ? "not " : "");
	smbc_cli_shutdown(cli);

	return !res;
}

static int is_host(char *host)
{
	struct cli_state *cli;
	cli = smbc_cli_initialize();

	uint32_t res = smbc_cli_connect(cli, host, SMB_PORT);

	smbc_cli_shutdown(cli);

	return !res;
}

/* simple combsort algorithm */
void smbc_dirls_sort(smbc_dirls_entry_t **ent, int num) {
	int gap = num;
	int i, j;
	int swapped;
	smbc_dirls_entry_t *tmp;

	do {
		gap = (gap * 10) / 13;
		if (gap == 9 || gap == 10)
			gap = 11;
		if (gap < 1)
			gap = 1;
		swapped = 0;

		for (i = 0; i < num - gap; i++) {
			j = i + gap;
			if (strcmp(DIRLIST_ENT_NAME(ent[i]), DIRLIST_ENT_NAME(ent[j])) > 0) {
				tmp = ent[i];
				ent[i] = ent[j];
				ent[j] = tmp;
				swapped = 1;
			}
		}

	} while (gap > 1 || swapped);
}

/* buffer must be able to hold "999.9K"
 * conversion is simple but not perfect
 */
int smbc_list_directory_sizefmt(char *buf, off_t size) 
{
	const char unit[] = "KMGTPE";	/* Kilo, Mega, Tera, Peta, Exa */
	const char *u = unit - 1;		/* u will always increment at least once */
	int remain;
	char *out = buf;

	if (size < 100)
		size += 99;
	if (size < 100)
		size = 0;

	while (1) {
		remain = (int) size & 1023;
		size >>= 10;
		u++;
		if ((size & (~0 ^ 1023)) == 0)
			break;
	}

	remain /= 100;
	if (remain > 9)
		remain = 9;
	if (size > 999) {
		size   = 0;
		remain = 9;
		u++;
	}

	out   += LI_ltostr(out, size);
	out[0] = '.';
	out[1] = remain + '0';
	out[2] = *u;
	out[3] = '\0';
	
	return (out + 3 - buf);	
}

static void get_auth_data_fn(const char * pServer,
                 const char * pShare,
                 char * pWorkgroup,
                 int maxLenWorkgroup,
                 char * pUsername,
                 int maxLenUsername,
                 char * pPassword,
                 int maxLenPassword)
{
	UNUSED(pServer);
	UNUSED(pShare);
	UNUSED(maxLenWorkgroup);
	UNUSED(maxLenUsername);
	UNUSED(maxLenPassword);
	
	UNUSED(pWorkgroup);
	UNUSED(pUsername);
	UNUSED(pPassword);
}

/* init the plugin data */
INIT_FUNC(mod_smbdav_init) {

	plugin_data *p;
	
	p = calloc(1, sizeof(*p));

	p->tmp_buf = buffer_init();
	
	p->uri.scheme = buffer_init();
	p->uri.path_raw = buffer_init();
	p->uri.path = buffer_init();
	p->uri.authority = buffer_init();

	p->physical.path = buffer_init();
	p->physical.rel_path = buffer_init();
	p->physical.doc_root = buffer_init();
	p->physical.basedir = buffer_init();

	int          dbglv = 0;
	smbc_init(get_auth_data_fn, dbglv);
	Cdbg(DBE,"complete smbc_init...");

	//stop_arpping_process();

	return p;
}

URIHANDLER_FUNC(mod_msbdav_connection_close) {	
	UNUSED(srv);
	
	if(con->smb_info && con->smb_info->cli != NULL) {
		smbc_cli_shutdown(con->smb_info->cli);
	}

	return HANDLER_GO_ON;
}

/* detroy the plugin data */
FREE_FUNC(mod_smbdav_free) {
	plugin_data *p = p_d;

	UNUSED(srv);

	if (!p) return HANDLER_GO_ON;

	if (p->config_storage) {
		size_t i;
		for (i = 0; i < srv->config_context->used; i++) {
			plugin_config *s = p->config_storage[i];

			if (!s) continue;

			buffer_free(s->sqlite_db_name);
#ifdef USE_PROPPATCH
			if (s->sql) {
				sqlite3_finalize(s->stmt_delete_prop);
				sqlite3_finalize(s->stmt_delete_uri);
				sqlite3_finalize(s->stmt_copy_uri);
				sqlite3_finalize(s->stmt_move_uri);
				sqlite3_finalize(s->stmt_update_prop);
				sqlite3_finalize(s->stmt_select_prop);
				sqlite3_finalize(s->stmt_select_propnames);

				sqlite3_finalize(s->stmt_read_lock);
				sqlite3_finalize(s->stmt_read_lock_by_uri);
				sqlite3_finalize(s->stmt_create_lock);
				sqlite3_finalize(s->stmt_remove_lock);
				sqlite3_finalize(s->stmt_refresh_lock);
				sqlite3_close(s->sql);
			}
#endif
			
			//- 20130304 JerryLin add
			buffer_free(s->sqlite_minidlna_db_name);
			if (s->sql_minidlna) {
				sqlite3_close(s->sql_minidlna);
			}
			
			free(s);
		}
		free(p->config_storage);
	}

	buffer_free(p->uri.scheme);
	buffer_free(p->uri.path_raw);
	buffer_free(p->uri.path);
	buffer_free(p->uri.authority);

	buffer_free(p->physical.path);
	buffer_free(p->physical.rel_path);
	buffer_free(p->physical.doc_root);
	buffer_free(p->physical.basedir);

	buffer_free(p->tmp_buf);

	free(p);

	stop_arpping_process();
	
	return HANDLER_GO_ON;
}

/* handle plugin config and check values */

SETDEFAULTS_FUNC(mod_smbdav_set_defaults) {
	plugin_data *p = p_d;
	size_t i = 0;

	config_values_t cv[] = {
		{ "smbdav.activate",            NULL, T_CONFIG_BOOLEAN, T_CONFIG_SCOPE_CONNECTION },       /* 0 */
		{ "smbdav.is-readonly",         NULL, T_CONFIG_BOOLEAN, T_CONFIG_SCOPE_CONNECTION },       /* 1 */
		{ "smbdav.sqlite-db-name",      NULL, T_CONFIG_STRING,  T_CONFIG_SCOPE_CONNECTION },       /* 2 */
		{ "smbdav.log-xml",             NULL, T_CONFIG_BOOLEAN, T_CONFIG_SCOPE_CONNECTION },       /* 3 */
		{ "smbdav.sqlite-minidlna-db-name", NULL, T_CONFIG_STRING,  T_CONFIG_SCOPE_CONNECTION },   /* 4 */
		{ NULL,                         NULL, T_CONFIG_UNSET, T_CONFIG_SCOPE_UNSET }
	};

	if (!p) return HANDLER_ERROR;

	p->config_storage = calloc(1, srv->config_context->used * sizeof(specific_config *));
	
	for (i = 0; i < srv->config_context->used; i++) {
		plugin_config *s;

		s = calloc(1, sizeof(plugin_config));
		s->sqlite_db_name = buffer_init();

		//- 20130304 JerryLin add
		s->sqlite_minidlna_db_name = buffer_init();
			
		cv[0].destination = &(s->enabled);
		cv[1].destination = &(s->is_readonly);
		cv[2].destination = s->sqlite_db_name;
		cv[3].destination = &(s->log_xml);
		cv[4].destination = s->sqlite_minidlna_db_name;
		
		p->config_storage[i] = s;
				
		if (0 != config_insert_values_global(srv, ((data_config *)srv->config_context->data[i])->value, cv)) {
			return HANDLER_ERROR;
		}

#ifdef USE_MINIDLNA_DB
		//- 20130304 JerryLin add
		if (!buffer_is_empty(s->sqlite_minidlna_db_name)) {
			if (SQLITE_OK != sqlite3_open(s->sqlite_minidlna_db_name->ptr, &(s->sql_minidlna))) {				
				log_error_write(srv, __FILE__, __LINE__, "sbs", "sqlite3_open failed for",
						s->sqlite_minidlna_db_name,
						sqlite3_errmsg(s->sql_minidlna));
				Cdbg(DBE, "Fail to open minidlna db %s", s->sqlite_minidlna_db_name->ptr);
			}
		}
#endif

		if (!buffer_is_empty(s->sqlite_db_name)) {
#ifdef USE_PROPPATCH
			const char *next_stmt;
			char *err;

			if (SQLITE_OK != sqlite3_open(s->sqlite_db_name->ptr, &(s->sql))) {
				log_error_write(srv, __FILE__, __LINE__, "sbs", "sqlite3_open failed for",
						s->sqlite_db_name,
						sqlite3_errmsg(s->sql));
				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_exec(s->sql,
					"CREATE TABLE properties ("
					"  resource TEXT NOT NULL,"
					"  prop TEXT NOT NULL,"
					"  ns TEXT NOT NULL,"
					"  value TEXT NOT NULL,"
					"  PRIMARY KEY(resource, prop, ns))",
					NULL, NULL, &err)) {

				if (0 != strcmp(err, "table properties already exists")) {
					log_error_write(srv, __FILE__, __LINE__, "ss", "can't open transaction:", err);
					sqlite3_free(err);

					return HANDLER_ERROR;
				}
				sqlite3_free(err);
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("SELECT value FROM properties WHERE resource = ? AND prop = ? AND ns = ?"),
				&(s->stmt_select_prop), &next_stmt)) {
				/* prepare failed */

				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed:", sqlite3_errmsg(s->sql));
				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("SELECT ns, prop FROM properties WHERE resource = ?"),
				&(s->stmt_select_propnames), &next_stmt)) {
				/* prepare failed */

				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed:", sqlite3_errmsg(s->sql));
				return HANDLER_ERROR;
			}


			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("REPLACE INTO properties (resource, prop, ns, value) VALUES (?, ?, ?, ?)"),
				&(s->stmt_update_prop), &next_stmt)) {
				/* prepare failed */

				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed:", sqlite3_errmsg(s->sql));
				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("DELETE FROM properties WHERE resource = ? AND prop = ? AND ns = ?"),
				&(s->stmt_delete_prop), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("DELETE FROM properties WHERE resource = ?"),
				&(s->stmt_delete_uri), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("INSERT INTO properties SELECT ?, prop, ns, value FROM properties WHERE resource = ?"),
				&(s->stmt_copy_uri), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("UPDATE properties SET resource = ? WHERE resource = ?"),
				&(s->stmt_move_uri), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			/* LOCKS */

			if (SQLITE_OK != sqlite3_exec(s->sql,
					"CREATE TABLE locks ("
					"  locktoken TEXT NOT NULL,"
					"  resource TEXT NOT NULL,"
					"  lockscope TEXT NOT NULL,"
					"  locktype TEXT NOT NULL,"
					"  owner TEXT NOT NULL,"
					"  depth INT NOT NULL,"
					"  timeout TIMESTAMP NOT NULL,"
					"  PRIMARY KEY(locktoken))",
					NULL, NULL, &err)) {

				if (0 != strcmp(err, "table locks already exists")) {
					log_error_write(srv, __FILE__, __LINE__, "ss", "can't open transaction:", err);
					sqlite3_free(err);

					return HANDLER_ERROR;
				}
				sqlite3_free(err);
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("INSERT INTO locks (locktoken, resource, lockscope, locktype, owner, depth, timeout) VALUES (?,?,?,?,?,?, CURRENT_TIME + 600)"),
				&(s->stmt_create_lock), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("DELETE FROM locks WHERE locktoken = ?"),
				&(s->stmt_remove_lock), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("SELECT locktoken, resource, lockscope, locktype, owner, depth, timeout FROM locks WHERE locktoken = ?"),
				&(s->stmt_read_lock), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("SELECT locktoken, resource, lockscope, locktype, owner, depth, timeout FROM locks WHERE resource = ?"),
				&(s->stmt_read_lock_by_uri), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}

			if (SQLITE_OK != sqlite3_prepare(s->sql,
				CONST_STR_LEN("UPDATE locks SET timeout = CURRENT_TIME + 600 WHERE locktoken = ?"),
				&(s->stmt_refresh_lock), &next_stmt)) {
				/* prepare failed */
				log_error_write(srv, __FILE__, __LINE__, "ss", "sqlite3_prepare failed", sqlite3_errmsg(s->sql));

				return HANDLER_ERROR;
			}


#else
			log_error_write(srv, __FILE__, __LINE__, "s", "Sorry, no sqlite3 and libxml2 support include, compile with --with-smbdav-props");
			return HANDLER_ERROR;
#endif
		}
	}

	return HANDLER_GO_ON;
}

#define PATCH_OPTION(x) \
	p->conf.x = s->x;
static int mod_smbdav_patch_connection(server *srv, connection *con, plugin_data *p) {
	size_t i, j;
	plugin_config *s = p->config_storage[0];

	PATCH_OPTION(enabled);
	PATCH_OPTION(is_readonly);
	PATCH_OPTION(log_xml);
	//PATCH_OPTION(always_auth);
	
#ifdef USE_PROPPATCH
	PATCH_OPTION(sql);
	PATCH_OPTION(stmt_update_prop);
	PATCH_OPTION(stmt_delete_prop);
	PATCH_OPTION(stmt_select_prop);
	PATCH_OPTION(stmt_select_propnames);

	PATCH_OPTION(stmt_delete_uri);
	PATCH_OPTION(stmt_move_uri);
	PATCH_OPTION(stmt_copy_uri);

	PATCH_OPTION(stmt_remove_lock);
	PATCH_OPTION(stmt_refresh_lock);
	PATCH_OPTION(stmt_create_lock);
	PATCH_OPTION(stmt_read_lock);
	PATCH_OPTION(stmt_read_lock_by_uri);
#endif

#ifdef USE_MINIDLNA_DB
	PATCH_OPTION(sqlite_minidlna_db_name);
	PATCH_OPTION(sql_minidlna);
#endif

	/* skip the first, the global context */
	for (i = 1; i < srv->config_context->used; i++) {
		data_config *dc = (data_config *)srv->config_context->data[i];
		s = p->config_storage[i];

		/* condition didn't match */
		if (!config_check_cond(srv, con, dc)) continue;

		/* merge config */
		for (j = 0; j < dc->value->used; j++) {
			data_unset *du = dc->value->data[j];

			if (buffer_is_equal_string(du->key, CONST_STR_LEN("smbdav.activate"))) {
				PATCH_OPTION(enabled);
			} else if (buffer_is_equal_string(du->key, CONST_STR_LEN("smbdav.is-readonly"))) {
				PATCH_OPTION(is_readonly);
			} else if (buffer_is_equal_string(du->key, CONST_STR_LEN("smbdav.log-xml"))) {
				PATCH_OPTION(log_xml);
			//} else if (buffer_is_equal_string(du->key, CONST_STR_LEN("smbdav.always-auth"))) {
			//	PATCH_OPTION(always_auth);			
			} else if (buffer_is_equal_string(du->key, CONST_STR_LEN("smbdav.sqlite-db-name"))) {				
#ifdef USE_PROPPATCH
				PATCH_OPTION(sql);
				PATCH_OPTION(stmt_update_prop);
				PATCH_OPTION(stmt_delete_prop);
				PATCH_OPTION(stmt_select_prop);
				PATCH_OPTION(stmt_select_propnames);

				PATCH_OPTION(stmt_delete_uri);
				PATCH_OPTION(stmt_move_uri);
				PATCH_OPTION(stmt_copy_uri);

				PATCH_OPTION(stmt_remove_lock);
				PATCH_OPTION(stmt_refresh_lock);
				PATCH_OPTION(stmt_create_lock);
				PATCH_OPTION(stmt_read_lock);
				PATCH_OPTION(stmt_read_lock_by_uri);
#endif
			}
			else if (buffer_is_equal_string(du->key, CONST_STR_LEN("smbdav.sqlite-minidlna-db-name"))) {				
#ifdef USE_MINIDLNA_DB
				PATCH_OPTION(sqlite_minidlna_db_name);
				PATCH_OPTION(sql_minidlna);
#endif
			}
		}
	}

	return 0;
}

URIHANDLER_FUNC(mod_smbdav_uri_handler) {
	plugin_data *p = p_d;

	if (con->uri.path->used == 0) 
		return HANDLER_GO_ON;

	mod_smbdav_patch_connection(srv, con, p);

	if (!p->conf.enabled) 
		return HANDLER_GO_ON;
	
	char pWorkgroup[30]={0};
	char pServer[64]={0};
	char pShare[128]={0};
	char pPath[128]={0};
	smbc_parse_path(con->physical.path->ptr, pWorkgroup, pServer, pShare, pPath);
	
	switch (con->request.http_method) {
	case HTTP_METHOD_OPTIONS:
		/* we fake a little bit but it makes MS W2k happy and it let's us mount the volume */
		response_header_overwrite(srv, con, CONST_STR_LEN("DAV"), CONST_STR_LEN("1,2"));
		response_header_overwrite(srv, con, CONST_STR_LEN("MS-Author-Via"), CONST_STR_LEN("DAV"));

		if (p->conf.is_readonly) {
			response_header_insert(srv, con, CONST_STR_LEN("Allow"), CONST_STR_LEN("PROPFIND"));
		} else {
			response_header_insert(srv, con, CONST_STR_LEN("Allow"), CONST_STR_LEN("PROPFIND, DELETE, MKCOL, PUT, MOVE, COPY, PROPPATCH, LOCK, UNLOCK"));
		}
		break;
	default:
		break;
	}

	/* not found */
	

	return HANDLER_GO_ON;
}

static int is_usbdisk_exist(server *srv, connection *con, const char* usbdisk_name){

	#if EMBEDDED_EANBLE
	char *a = nvram_get_webdavaidisk();
	if(strcmp( a, "0" ) == 0)
		return 0;
	#ifdef APP_IPKG
	free(a);
	#endif
	#endif
	
	config_values_t cv[] = {
		{ "alias.url",					NULL, T_CONFIG_ARRAY, T_CONFIG_SCOPE_CONNECTION },		 /* 0 */
		{ NULL, 						NULL, T_CONFIG_UNSET,  T_CONFIG_SCOPE_UNSET }
	};
	
	int found_file = 0;
	size_t i, k;
	for (i = 1; i < srv->config_context->used; i++) {
			
		array* alias = array_init();
		cv[0].destination = alias;
		
		if (0 != config_insert_values_global(srv, ((data_config *)srv->config_context->data[i])->value, cv)) {
			continue;
		}
				
		for (k = 0; k < alias->used; k++) {
			data_string *ds = (data_string *)alias->data[k];
				
			if( strstr(ds->key->ptr, usbdisk_name) ){
					
				DIR *dir;
				if (NULL != (dir = opendir(ds->value->ptr))) {
					struct dirent *de;
					while(NULL != (de = readdir(dir))) {
	
						if(strcmp(de->d_name, ".")==0||strcmp(de->d_name, "..")==0)
							continue;
							
						found_file = 1;
						//Cdbg(DBE, "alias->data[k]->key=%s, value=%s, de->d_name=%s", ds->key->ptr, ds->value->ptr, de->d_name );
						break;
					}
					closedir(dir);
				}
					
				break;
			}
				
		}
	
		array_free(alias);
	
		if(found_file==1)
			break;
	
	}

	return found_file;
}

static int smbdav_gen_prop_tag(server *srv, connection *con,
		char *prop_name,
		char *prop_ns,
		char *value,
		buffer *b) {

	UNUSED(srv);
	UNUSED(con);

	if (value) {
		buffer_append_string_len(b,CONST_STR_LEN("<"));
		buffer_append_string(b, prop_name);
		buffer_append_string_len(b, CONST_STR_LEN(" xmlns=\""));
		buffer_append_string(b, prop_ns);
		buffer_append_string_len(b, CONST_STR_LEN("\">"));

		buffer_append_string(b, value);

		buffer_append_string_len(b,CONST_STR_LEN("</"));
		buffer_append_string(b, prop_name);
		buffer_append_string_len(b, CONST_STR_LEN(">"));
	} else {
		buffer_append_string_len(b,CONST_STR_LEN("<"));
		buffer_append_string(b, prop_name);
		buffer_append_string_len(b, CONST_STR_LEN(" xmlns=\""));
		buffer_append_string(b, prop_ns);
		buffer_append_string_len(b, CONST_STR_LEN("\"/>"));
	}

	return 0;
}


static int smbdav_gen_response_status_tag(server *srv, connection *con, physical *dst, int status, buffer *b) {
	UNUSED(srv);

	buffer_append_string_len(b,CONST_STR_LEN("<D:response xmlns:ns0=\"urn:uuid:c2f41010-65b3-11d1-a29f-00aa00c14882/\">\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:href>\n"));
	buffer_append_string_buffer(b, dst->rel_path);
	buffer_append_string_len(b,CONST_STR_LEN("</D:href>\n"));
	buffer_append_string_len(b,CONST_STR_LEN("<D:status>\n"));

	if (con->request.http_version == HTTP_VERSION_1_1) {
		buffer_copy_string_len(b, CONST_STR_LEN("HTTP/1.1 "));
	} else {
		buffer_copy_string_len(b, CONST_STR_LEN("HTTP/1.0 "));
	}
	buffer_append_long(b, status);
	buffer_append_string_len(b, CONST_STR_LEN(" "));
	buffer_append_string(b, get_http_status_name(status));

	buffer_append_string_len(b,CONST_STR_LEN("</D:status>\n"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:response>\n"));

	return 0;
}

static int smbdav_delete_file(server *srv, connection *con, plugin_data *p, physical *dst, buffer *b) 
{
	int status = 0;

	/* try to unlink it */
	if (-1 == smbc_wrapper_unlink(con, dst->path->ptr)) {
		switch(errno) {
		case EACCES:
		case EPERM:
			/* 403 */
			status = 403;
			break;
		default:
			status = 501;
			break;
		}
		smbdav_gen_response_status_tag(srv, con, dst, status, b);
	} else {
#ifdef USE_PROPPATCH
		sqlite3_stmt *stmt = p->conf.stmt_delete_uri;

		if (!stmt) {
			status = 403;
			smbdav_gen_response_status_tag(srv, con, dst, status, b);
		} else {
			sqlite3_reset(stmt);

			/* bind the values to the insert */

			sqlite3_bind_text(stmt, 1,
					  dst->rel_path->ptr,
					  dst->rel_path->used - 1,
					  SQLITE_TRANSIENT);

			if (SQLITE_DONE != sqlite3_step(stmt)) {
				/* */
			}
		}
#else
		UNUSED(p);
#endif
	}

	return (status != 0);
}

static int smbdav_delete_dir(server *srv, connection *con, plugin_data *p, physical *dst, buffer *b) 
{
	DIR *dir;
	int have_multi_status = 0;
	physical d;

	d.path = buffer_init();
	d.rel_path = buffer_init();
	
	if (NULL != (dir = smbc_wrapper_opendir(con, dst->path->ptr))) {
		struct smbc_dirent *de;

		while(NULL != (de = smbc_wrapper_readdir(con, dir))) {
			struct stat st;
			int status = 0;

			if ((de->name[0] == '.' && de->name[1] == '\0')  ||
			    (de->name[0] == '.' && de->name[1] == '.' && de->name[2] == '\0')) {
				continue;
				/* ignore the parent dir */
			}
			
			buffer_copy_string_buffer(d.path, dst->path);
			BUFFER_APPEND_SLASH(d.path);
			buffer_append_string(d.path, de->name);

			buffer_copy_string_buffer(d.rel_path, dst->rel_path);
			BUFFER_APPEND_SLASH(d.rel_path);
			buffer_append_string(d.rel_path, de->name);
			
			/* stat and unlink afterwards */
			if (-1 == smbc_wrapper_stat(con, d.path->ptr, &st)) {
				/* don't about it yet, rmdir will fail too */
			} else if (S_ISDIR(st.st_mode)) {				
				have_multi_status = smbdav_delete_dir(srv, con, p, &d, b);

				/* try to unlink it */
				if (-1 == smbc_wrapper_rmdir(con, d.path->ptr)) {
					switch(errno) {
					case EACCES:
					case EPERM:
						/* 403 */
						status = 403;
						break;
					default:
						status = 501;
						break;
					}
					have_multi_status = 1;

					smbdav_gen_response_status_tag(srv, con, &d, status, b);
				} else {
#ifdef USE_PROPPATCH
					sqlite3_stmt *stmt = p->conf.stmt_delete_uri;

					status = 0;

					if (stmt) {
						sqlite3_reset(stmt);

						/* bind the values to the insert */

						sqlite3_bind_text(stmt, 1,
								  d.rel_path->ptr,
								  d.rel_path->used - 1,
								  SQLITE_TRANSIENT);

						if (SQLITE_DONE != sqlite3_step(stmt)) {
							/* */
						}
					}
#endif
				}
			} else {
				have_multi_status = smbdav_delete_file(srv, con, p, &d, b);
			}
		}
		smbc_wrapper_closedir(con, dir);

		buffer_free(d.path);
		buffer_free(d.rel_path);
	}

	return have_multi_status;
}

static int smbdav_copy_file(server *srv, connection *con, plugin_data *p, physical *src, physical *dst, int overwrite) 
{		
	int status = 0, ret, src_fd = -1, dst_fd = -1;
	char buff[2048]; 

	UNUSED(srv);
	UNUSED(con);
	
	if (-1 == (dst_fd = smbc_open(dst->path->ptr, O_WRONLY|O_TRUNC|O_CREAT|(overwrite ? 0 : O_EXCL), WEBDAV_FILE_MODE))) {
		/* opening the destination failed for some reason */
		switch(errno) {
		case EEXIST:
			status = 412;
			break;
		case EISDIR:
			status = 409;
			break;
		case ENOENT:
			/* at least one part in the middle wasn't existing */
			status = 409;
			break;
		default:
			status = 403;
			break;
		}
		smbc_close(dst_fd);
		return status;
	}
		
	if (src_fd = smbc_open(src->path->ptr, O_RDONLY, WEBDAV_FILE_MODE)) {
		do {
			ret = smbc_read(src_fd, buff, sizeof(buff));
			if (ret > 0) {
				if (-1 == smbc_write(dst_fd, buff, ret)) {
					switch(errno) {
					case ENOSPC:
						status = 507;
						break;
					default:
						status = 403;
						break;
					}

					smbc_close(src_fd);
					smbc_close(dst_fd);
					return status;
				}
			}
		} while (ret > 0);
	}
	
	smbc_close(src_fd);
	smbc_close(dst_fd);

#ifdef USE_PROPPATCH
	if (0 == status) {
		/* copy worked fine, copy connected properties */
		sqlite3_stmt *stmt = p->conf.stmt_copy_uri;

		if (stmt) {
			sqlite3_reset(stmt);

			/* bind the values to the insert */
			sqlite3_bind_text(stmt, 1,
					  dst->rel_path->ptr,
					  dst->rel_path->used - 1,
					  SQLITE_TRANSIENT);

			sqlite3_bind_text(stmt, 2,
					  src->rel_path->ptr,
					  src->rel_path->used - 1,
					  SQLITE_TRANSIENT);

			if (SQLITE_DONE != sqlite3_step(stmt)) {
				/* */
			}
		}
	}
#else
	UNUSED(p);
#endif
	
	return status;
}

static int smbdav_copy_dir(server *srv, connection *con, plugin_data *p, physical *src, physical *dst, int overwrite) 
{
	DIR *srcdir;
	int status = 0;

	if (NULL != (srcdir = smbc_wrapper_opendir(con, src->path->ptr))) {
		struct smbc_dirent *de;
		physical s, d;

		s.path = buffer_init();
		s.rel_path = buffer_init();

		d.path = buffer_init();
		d.rel_path = buffer_init();

		while (NULL != (de = smbc_wrapper_readdir(con, srcdir))) {
			struct stat st;

			if( (de->name[0] == '.' && de->name[1] == '\0') ||
		        (de->name[0] == '.' && de->name[1] == '.' && de->name[2] == '\0')) {
				continue;
			}
			
			buffer_copy_string_buffer(s.path, src->path);
			BUFFER_APPEND_SLASH(s.path);
			buffer_append_string(s.path, de->name);

			buffer_copy_string_buffer(d.path, dst->path);
			BUFFER_APPEND_SLASH(d.path);
			buffer_append_string(d.path, de->name);
			
			buffer_copy_string_buffer(s.rel_path, src->rel_path);
			BUFFER_APPEND_SLASH(s.rel_path);
			buffer_append_string(s.rel_path, de->name);

			buffer_copy_string_buffer(d.rel_path, dst->rel_path);
			BUFFER_APPEND_SLASH(d.rel_path);
			buffer_append_string(d.rel_path, de->name);
			
			if (-1 == smbc_wrapper_stat(con, s.path->ptr, &st)) {
				/* why ? */
			} else if (S_ISDIR(st.st_mode)) {
				/* a directory */
				if (-1 == smbc_wrapper_mkdir(con, dst->path->ptr, WEBDAV_DIR_MODE) &&
				    errno != EEXIST) {
					/* WTH ? */
				} else {
#ifdef USE_PROPPATCH
					sqlite3_stmt *stmt = p->conf.stmt_copy_uri;

					if (0 != (status = smbdav_copy_dir(srv, con, p, &s, &d, overwrite))) {
						break;
					}
					
					/* directory is copied, copy the properties too */
					if (stmt) {
						sqlite3_reset(stmt);

						/* bind the values to the insert */
						sqlite3_bind_text(stmt, 1,
							  dst->rel_path->ptr,
							  dst->rel_path->used - 1,
							  SQLITE_TRANSIENT);

						sqlite3_bind_text(stmt, 2,
							  src->rel_path->ptr,
							  src->rel_path->used - 1,
							  SQLITE_TRANSIENT);

						if (SQLITE_DONE != sqlite3_step(stmt)) {
							/* */
						}
					}
#endif
				}
			} 
			else if (S_ISREG(st.st_mode)) {				
				/* a plain file */
				if (0 != (status = smbdav_copy_file(srv, con, p, &s, &d, overwrite))) {
					break;
				}
			}
		}

		buffer_free(s.path);
		buffer_free(s.rel_path);
		buffer_free(d.path);
		buffer_free(d.rel_path);

		smbc_wrapper_closedir(con, srcdir);
	}

	return status;
}

static int smbdav_get_live_property(server *srv, connection *con, plugin_data *p, physical *dst, char *prop_name, buffer *b) {
	stat_cache_entry *sce = NULL;
	int found = 0;

	UNUSED(p);
	
	if (HANDLER_ERROR != (stat_cache_get_entry(srv, con, dst->path, &sce))) {
		char ctime_buf[] = "2005-08-18T07:27:16Z";
		char mtime_buf[] = "Thu, 18 Aug 2005 07:27:16 GMT";
		size_t k;
		
		if (0 == strcmp(prop_name, "resourcetype")) {
			if (S_ISDIR(sce->st.st_mode)) {
				buffer_append_string_len(b, CONST_STR_LEN("<D:resourcetype><D:collection/></D:resourcetype>"));
				found = 1;
			}
		} else if (0 == strcmp(prop_name, "getcontenttype")) {			
			if (S_ISDIR(sce->st.st_mode)) {
				buffer_append_string_len(b, CONST_STR_LEN("<D:getcontenttype>httpd/unix-directory</D:getcontenttype>"));
				found = 1;
			} else if(S_ISREG(sce->st.st_mode)) {
				for (k = 0; k < con->conf.mimetypes->used; k++) {
					data_string *ds = (data_string *)con->conf.mimetypes->data[k];

					if (ds->key->used == 0) continue;

					if (buffer_is_equal_right_len(dst->path, ds->key, ds->key->used - 1)) {
						buffer_append_string_len(b,CONST_STR_LEN("<D:getcontenttype>"));
						buffer_append_string_buffer(b, ds->value);
						buffer_append_string_len(b, CONST_STR_LEN("</D:getcontenttype>"));
						found = 1;

						break;
					}
				}
			}
		} else if (0 == strcmp(prop_name, "creationdate")) {
			//buffer_append_string_len(b, CONST_STR_LEN("<D:creationdate ns0:dt=\"dateTime.tz\">"));
			buffer_append_string_len(b, CONST_STR_LEN("<D:creationdate>"));
			strftime(ctime_buf, sizeof(ctime_buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&(sce->st.st_ctime)));
			buffer_append_string(b, ctime_buf);
			buffer_append_string_len(b, CONST_STR_LEN("</D:creationdate>"));
			found = 1;
		} else if (0 == strcmp(prop_name, "getlastmodified")) {
			//buffer_append_string_len(b,CONST_STR_LEN("<D:getlastmodified ns0:dt=\"dateTime.rfc1123\">"));
			buffer_append_string_len(b,CONST_STR_LEN("<D:getlastmodified>"));
			strftime(mtime_buf, sizeof(mtime_buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&(sce->st.st_mtime)));
			//strftime(mtime_buf, sizeof(mtime_buf), "%Y/%m/%d %H:%M:%S", localtime(&(sce->st.st_mtime)));
			
			buffer_append_string(b, mtime_buf);
			buffer_append_string_len(b, CONST_STR_LEN("</D:getlastmodified>"));
			found = 1;
		} else if (0 == strcmp(prop_name, "getcontentlength")) {
			buffer_append_string_len(b,CONST_STR_LEN("<D:getcontentlength>"));
			buffer_append_off_t(b, sce->st.st_size);
			buffer_append_string_len(b, CONST_STR_LEN("</D:getcontentlength>"));
			found = 1;
		} else if (0 == strcmp(prop_name, "getcontentlanguage")) {
			buffer_append_string_len(b,CONST_STR_LEN("<D:getcontentlanguage>"));
			buffer_append_string_len(b, CONST_STR_LEN("en"));
			buffer_append_string_len(b, CONST_STR_LEN("</D:getcontentlanguage>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getuniqueid")) {
			//- 20120104 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:getuniqueid>"));
			//Cdbg( DBE, "dst->rel_path->ptr=%s, sce->st.st_ino=%d", dst->rel_path->ptr, sce->st.st_ino );

			char* result;
			md5String(dst->rel_path->ptr, NULL, &result);				
			buffer_append_string(b, result);
			free(result);
			
			buffer_append_string_len(b, CONST_STR_LEN("</D:getuniqueid>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getattr")) {
			//- 20120215 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:getattr>"));

			buffer_append_string_len(b, CONST_STR_LEN("<D:readonly>"));
			if( (sce->st.st_mode & 0x01) != 0 &&S_ISREG(sce->st.st_mode)){
				//Cdbg(DBE, "2222222222222222222222 %s is readonly %X", dst->path->ptr, (sce->st.st_mode & 0x01));
				buffer_append_string_len(b, CONST_STR_LEN("true"));
			}
			else
				buffer_append_string_len(b, CONST_STR_LEN("false"));
			buffer_append_string_len(b, CONST_STR_LEN("</D:readonly>"));


			buffer_append_string_len(b, CONST_STR_LEN("<D:hidden>"));
			if( (sce->st.st_mode & 0x02) != 0 &&S_ISREG(sce->st.st_mode)){
				//Cdbg(DBE, "1111111111111111111111 %s is hidden %X", dst->path->ptr, (sce->st.st_mode & 0x02));
				buffer_append_string_len(b, CONST_STR_LEN("true"));
			}
			else
				buffer_append_string_len(b, CONST_STR_LEN("false"));
			buffer_append_string_len(b, CONST_STR_LEN("</D:hidden>"));
			
			/*
			if( (sce->st.st_mode & 0x02) != 0 )
				Cdbg(DBE, "1111111111111111111111 %s is hidden %X", dst->path->ptr, (sce->st.st_mode & 0x02));

			//if( (sce->st.st_mode & 0x04) != 0 )
			//	Cdbg(DBE, "1111111111111111111111 %s is system %X", dst->path->ptr, (sce->st.st_mode & 0x04));

			if( (sce->st.st_mode & 0x01) != 0 )
				Cdbg(DBE, "2222222222222222222222 %s is readonly %X", dst->path->ptr, (sce->st.st_mode & 0x01));
			*/
			
			buffer_append_string_len(b, CONST_STR_LEN("</D:getattr>"));
			found = 1;
		}
	}

	return found ? 0 : -1;
}

static int smbdav_get_property(server *srv, connection *con, plugin_data *p, physical *dst, char *prop_name, char *prop_ns, buffer *b) {	
	
	if (0 == strcmp(prop_ns, "DAV:")) {
		/* a local 'live' property */
		
		return smbdav_get_live_property(srv, con, p, dst, prop_name, b);
	} else {
		int found = 0;
#ifdef USE_PROPPATCH
		sqlite3_stmt *stmt = p->conf.stmt_select_prop;

		if (stmt) {
			/* perhaps it is in sqlite3 */
			sqlite3_reset(stmt);

			/* bind the values to the insert */

			sqlite3_bind_text(stmt, 1,
					  dst->rel_path->ptr,
					  dst->rel_path->used - 1,
					  SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2,
					  prop_name,
					  strlen(prop_name),
					  SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3,
					  prop_ns,
					  strlen(prop_ns),
					  SQLITE_TRANSIENT);

			/* it is the PK */
			while (SQLITE_ROW == sqlite3_step(stmt)) {
				/* there is a row for us, we only expect a single col 'value' */
				smbdav_gen_prop_tag(srv, con, prop_name, prop_ns, (char *)sqlite3_column_text(stmt, 0), b);
				found = 1;
			}
		}
#endif
		return found ? 0 : -1;
	}

	/* not found */
	return -1;
}

typedef struct {
	char *ns;
	char *prop;
} smbdav_property;

smbdav_property smbdav_live_properties[] = {
	{ "DAV:", "creationdate" },
	{ "DAV:", "displayname" },
	{ "DAV:", "getcontentlanguage" },
	{ "DAV:", "getcontentlength" },
	{ "DAV:", "getcontenttype" },
	{ "DAV:", "getetag" },
	{ "DAV:", "getlastmodified" },
	{ "DAV:", "resourcetype" },
	{ "DAV:", "lockdiscovery" },
	{ "DAV:", "source" },
	{ "DAV:", "supportedlock" },
	{ "DAV:", "getmac" },
	{ "DAV:", "getip" },
	{ "DAV:", "getonline" },
	{ "DAV:", "getuniqueid" },
	{ "DAV:", "gettype" },
	{ "DAV:", "getattr" },
	{ "DAV:", "getname" },
	{ "DAV:", "getpath" },
	{ "DAV:", "getuseragent" },
	
	{ NULL, NULL }
};

typedef struct {
	smbdav_property **ptr;

	size_t used;
	size_t size;
} smbdav_properties;

static int smbdav_get_props(server *srv, connection *con, plugin_data *p, physical *dst, smbdav_properties *props, buffer *b_200, buffer *b_404) {
	size_t i;

	if (props) {
		for (i = 0; i < props->used; i++) {
			smbdav_property *prop;			
			prop = props->ptr[i];			
			if (0 != smbdav_get_property(srv, con, p, dst, prop->prop, prop->ns, b_200)) {				
				smbdav_gen_prop_tag(srv, con, prop->prop, prop->ns, NULL, b_404);
			}
		}
	} else {		
		for (i = 0; smbdav_live_properties[i].prop; i++) {
			/* a local 'live' property */
			smbdav_get_live_property(srv, con, p, dst, smbdav_live_properties[i].prop, b_200);
		}
	}

	return 0;
}

#ifdef USE_PROPPATCH
static int smbdav_parse_chunkqueue(server *srv, connection *con, plugin_data *p, chunkqueue *cq, xmlDoc **ret_xml) {
	xmlParserCtxtPtr ctxt;
	xmlDoc *xml;
	int res;
	int err;

	chunk *c;

	UNUSED(con);

	/* read the chunks in to the XML document */
	ctxt = xmlCreatePushParserCtxt(NULL, NULL, NULL, 0, NULL);

	for (c = cq->first; cq->bytes_out != cq->bytes_in; c = cq->first) {
		size_t weWant = cq->bytes_out - cq->bytes_in;
		size_t weHave;

		switch(c->type) {
		case SMB_CHUNK:
			break;
		case FILE_CHUNK:
			weHave = c->file.length - c->offset;

			if (weHave > weWant) weHave = weWant;

			/* xml chunks are always memory, mmap() is our friend */
			if (c->file.mmap.start == MAP_FAILED) {
				if (-1 == c->file.fd &&  /* open the file if not already open */
				    -1 == (c->file.fd = open(c->file.name->ptr, O_RDONLY))) {
					log_error_write(srv, __FILE__, __LINE__, "ss", "open failed: ", strerror(errno));

					return -1;
				}

				if (MAP_FAILED == (c->file.mmap.start = mmap(0, c->file.length, PROT_READ, MAP_SHARED, c->file.fd, 0))) {
					log_error_write(srv, __FILE__, __LINE__, "ssbd", "mmap failed: ",
							strerror(errno), c->file.name,  c->file.fd);
					close(c->file.fd);
					c->file.fd = -1;

					return -1;
				}

				close(c->file.fd);
				c->file.fd = -1;

				c->file.mmap.length = c->file.length;

				/* chunk_reset() or chunk_free() will cleanup for us */
			}

			if (XML_ERR_OK != (err = xmlParseChunk(ctxt, c->file.mmap.start + c->offset, weHave, 0))) {
				log_error_write(srv, __FILE__, __LINE__, "sodd", "xmlParseChunk failed at:", cq->bytes_out, weHave, err);
			}

			c->offset += weHave;
			cq->bytes_out += weHave;

			break;
		case MEM_CHUNK:
			/* append to the buffer */
			weHave = c->mem->used - 1 - c->offset;

			if (weHave > weWant) weHave = weWant;

			if (p->conf.log_xml) {
				log_error_write(srv, __FILE__, __LINE__, "ss", "XML-request-body:", c->mem->ptr + c->offset);
			}

			if (XML_ERR_OK != (err = xmlParseChunk(ctxt, c->mem->ptr + c->offset, weHave, 0))) {
				log_error_write(srv, __FILE__, __LINE__, "sodd", "xmlParseChunk failed at:", cq->bytes_out, weHave, err);
			}

			c->offset += weHave;
			cq->bytes_out += weHave;

			break;
		case UNUSED_CHUNK:
			break;
		}
		chunkqueue_remove_finished_chunks(cq);
	}

	switch ((err = xmlParseChunk(ctxt, 0, 0, 1))) {
	case XML_ERR_DOCUMENT_END:
	case XML_ERR_OK:
		break;
	default:
		log_error_write(srv, __FILE__, __LINE__, "sd", "xmlParseChunk failed at final packet:", err);
		break;
	}

	xml = ctxt->myDoc;
	res = ctxt->wellFormed;
	xmlFreeParserCtxt(ctxt);

	if (res == 0) {
		xmlFreeDoc(xml);
	} else {
		*ret_xml = xml;
	}

	return res;
}
#endif

#ifdef USE_LOCKS
static int smbdav_lockdiscovery(server *srv, connection *con,
		buffer *locktoken, const char *lockscope, const char *locktype, int depth) {

	buffer *b;

	response_header_overwrite(srv, con, CONST_STR_LEN("Lock-Token"), CONST_BUF_LEN(locktoken));

	response_header_overwrite(srv, con,
		CONST_STR_LEN("Content-Type"),
		CONST_STR_LEN("text/xml; charset=\"utf-8\""));

	b = chunkqueue_get_append_buffer(con->write_queue);

	buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:prop xmlns:D=\"DAV:\" xmlns:ns0=\"urn:uuid:c2f41010-65b3-11d1-a29f-00aa00c14882/\">\n"));
	buffer_append_string_len(b,CONST_STR_LEN("<D:lockdiscovery>\n"));
	buffer_append_string_len(b,CONST_STR_LEN("<D:activelock>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:lockscope>"));
	buffer_append_string_len(b,CONST_STR_LEN("<D:"));
	buffer_append_string(b, lockscope);
	buffer_append_string_len(b, CONST_STR_LEN("/>"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:lockscope>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:locktype>"));
	buffer_append_string_len(b,CONST_STR_LEN("<D:"));
	buffer_append_string(b, locktype);
	buffer_append_string_len(b, CONST_STR_LEN("/>"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:locktype>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:depth>"));
	buffer_append_string(b, depth == 0 ? "0" : "infinity");
	buffer_append_string_len(b,CONST_STR_LEN("</D:depth>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:timeout>"));
	buffer_append_string_len(b, CONST_STR_LEN("Second-600"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:timeout>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:owner>"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:owner>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("<D:locktoken>"));
	buffer_append_string_len(b, CONST_STR_LEN("<D:href>"));
	buffer_append_string_buffer(b, locktoken);
	buffer_append_string_len(b, CONST_STR_LEN("</D:href>"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:locktoken>\n"));

	buffer_append_string_len(b,CONST_STR_LEN("</D:activelock>\n"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:lockdiscovery>\n"));
	buffer_append_string_len(b,CONST_STR_LEN("</D:prop>\n"));

	return 0;
}
#endif

/**
 * check if resource is having the right locks to access to resource
 *
 *
 *
 */
static int smbdav_has_lock(server *srv, connection *con, plugin_data *p, buffer *uri) {
	int has_lock = 1;
	
#ifdef USE_LOCKS
	data_string *ds;
	UNUSED(srv);

	/**
	 * This implementation is more fake than real
	 * we need a parser for the If: header to really handle the full scope
	 *
	 * X-Litmus: locks: 11 (owner_modify)
	 * If: <http://127.0.0.1:1025/dav/litmus/lockme> (<opaquelocktoken:2165478d-0611-49c4-be92-e790d68a38f1>)
	 * - a tagged check:
	 *   if http://127.0.0.1:1025/dav/litmus/lockme is locked with
	 *   opaquelocktoken:2165478d-0611-49c4-be92-e790d68a38f1, go on
	 *
	 * X-Litmus: locks: 16 (fail_cond_put)
	 * If: (<DAV:no-lock> ["-1622396671"])
	 * - untagged:
	 *   go on if the resource has the etag [...] and the lock
	 */
	if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "If"))) {
		/* Ooh, ooh. A if tag, now the fun begins.
		 *
		 * this can only work with a real parser
		 **/
	} else {
		/* we didn't provided a lock-token -> */
		/* if the resource is locked -> 423 */

		sqlite3_stmt *stmt = p->conf.stmt_read_lock_by_uri;

		sqlite3_reset(stmt);

		sqlite3_bind_text(stmt, 1,
			  CONST_BUF_LEN(uri),
			  SQLITE_TRANSIENT);
		
		while (SQLITE_ROW == sqlite3_step(stmt)) {
			has_lock = 0;			
		}
	}
#else
	UNUSED(srv);
	UNUSED(con);
	UNUSED(p);
	UNUSED(uri);
#endif

	return has_lock;
}

static int smbdav_get_share_property(server *srv, connection *con, 
	plugin_data *p, 
	physical *dst, 
	buffer *ip,
	buffer *mac,
	int online,
	int type,
	char *prop_name, 
	buffer *b) 
{
	int found = 0;

	UNUSED(p);

	//if (HANDLER_ERROR != (smb_cache_get_entry(srv, con, p, dst->path, &sce))) 
	{
		char ctime_buf[] = "2005-08-18T07:27:16Z";
		char mtime_buf[] = "Thu, 18 Aug 2005 07:27:16 GMT";
		size_t k;

		if (0 == strcmp(prop_name, "resourcetype")) {
			buffer_append_string_len(b, CONST_STR_LEN("<D:resourcetype><D:collection/></D:resourcetype>"));
			found = 1;
		} else if (0 == strcmp(prop_name, "getcontenttype")) {
			buffer_append_string_len(b, CONST_STR_LEN("<D:getcontenttype>httpd/unix-directory</D:getcontenttype>"));
			found = 1;
#if 0			
		} else if (0 == strcmp(prop_name, "creationdate")) {
			buffer_append_string_len(b, CONST_STR_LEN("<D:creationdate ns0:dt=\"dateTime.tz\">"));
			strftime(ctime_buf, sizeof(ctime_buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&(sce->st.st_ctime)));
			buffer_append_string(b, ctime_buf);
			buffer_append_string_len(b, CONST_STR_LEN("</D:creationdate>"));
			found = 1;
		} else if (0 == strcmp(prop_name, "getlastmodified")) {
			buffer_append_string_len(b,CONST_STR_LEN("<D:getlastmodified ns0:dt=\"dateTime.rfc1123\">"));
			strftime(mtime_buf, sizeof(mtime_buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&(sce->st.st_mtime)));
			buffer_append_string(b, mtime_buf);
			buffer_append_string_len(b, CONST_STR_LEN("</D:getlastmodified>"));
			found = 1;
		} else if (0 == strcmp(prop_name, "getcontentlength")) {
			buffer_append_string_len(b,CONST_STR_LEN("<D:getcontentlength>"));
			buffer_append_off_t(b, sce->st.st_size);
			buffer_append_string_len(b, CONST_STR_LEN("</D:getcontentlength>"));
			found = 1;
#endif			
		} else if (0 == strcmp(prop_name, "getcontentlanguage")) {
			buffer_append_string_len(b,CONST_STR_LEN("<D:getcontentlanguage>"));
			buffer_append_string_len(b, CONST_STR_LEN("en"));
			buffer_append_string_len(b, CONST_STR_LEN("</D:getcontentlanguage>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getmac")) {
			//- 20111124 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:getmac>"));
			if(mac && mac->used)
				buffer_append_string_buffer(b, mac);			
			buffer_append_string_len(b, CONST_STR_LEN("</D:getmac>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getip")) {
			//- 20120417 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:getip>"));
			if(ip && ip->used)
				buffer_append_string_buffer(b, ip);			
			buffer_append_string_len(b, CONST_STR_LEN("</D:getip>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getonline")) {
			//- 20111219 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:getonline>"));
			char char_status[5]="\0";
			sprintf(char_status, "%d", online);
			buffer_append_string(b, char_status);
			buffer_append_string_len(b, CONST_STR_LEN("</D:getonline>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getuniqueid")) {
			//- 20120104 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:getuniqueid>"));
			char* result;
			md5String(dst->rel_path->ptr, ( (mac && mac->used) ? mac->ptr : NULL ), &result);				
			buffer_append_string(b, result);
			free(result);
			buffer_append_string_len(b, CONST_STR_LEN("</D:getuniqueid>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "gettype")) {
			//- 20120208 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:gettype>"));
			if(type==1)
				buffer_append_string(b, "sambapc");
			else if( type==2)
				buffer_append_string(b, "usbdisk");
			else
				buffer_append_string(b, "unknown");
			buffer_append_string_len(b, CONST_STR_LEN("</D:gettype>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getuseragent")) {			
			//- 20121205 Jerry add
			buffer_append_string_len(b,CONST_STR_LEN("<D:getuseragent>"));
			if(con->smb_info){
				buffer_append_string_buffer(b, con->smb_info->user_agent);
			}
			buffer_append_string_len(b, CONST_STR_LEN("</D:getuseragent>"));
			found = 1;
		}
		/*else if (0 == strcmp(prop_name, "getname")) {
			//- 20120920 Jerry add			
			buffer_append_string_len(b,CONST_STR_LEN("<D:getname>"));
			buffer_append_string_len(b,CONST_STR_LEN("sda"));
			buffer_append_string_len(b, CONST_STR_LEN("</D:getname>"));
			found = 1;
		}else if (0 == strcmp(prop_name, "getpath")) {
			//- 20120920 Jerry add
			Cdbg(1, "qqqqqqqqqqqqqq %s", dst->rel_path->ptr);
			buffer_append_string_len(b,CONST_STR_LEN("<D:getpath>"));
			buffer_append_string_buffer(b,dst->rel_path);
			buffer_append_string_len(b, CONST_STR_LEN("</D:getpath>"));
			found = 1;
		}*/
	}

	return found ? 0 : -1;
}

static int smbdav_get_share_props(server *srv, connection *con, 
	plugin_data *p, 
	physical *dst, 
	buffer *ip,
	buffer *mac,
	int online,
	int type,
	smbdav_properties *props, 
	buffer *b_200, 
	buffer *b_404) 
{
	size_t i;

	if (props) {
		for (i = 0; i < props->used; i++) {
			smbdav_property *prop;
			prop = props->ptr[i];			
			if (0 != smbdav_get_share_property(srv, con, p, dst, ip, mac, online, type, prop->prop, b_200)) {
				smbdav_gen_prop_tag(srv, con, prop->prop, prop->ns, NULL, b_200);
			}
		}

		if(props->used==0){
			for (i = 0; smbdav_live_properties[i].prop; i++) {
				if (0 != smbdav_get_share_property(srv, con, p, dst, ip, mac, online, type, smbdav_live_properties[i].prop, b_200)) {
					smbdav_gen_prop_tag(srv, con, smbdav_live_properties[i].prop, smbdav_live_properties[i].ns, NULL, b_200);
				}
			}
		}

	} else {
		for (i = 0; smbdav_live_properties[i].prop; i++) {
			/* a local 'live' property */
			smbdav_get_share_property(srv, con, p, dst, ip, mac, online, type, smbdav_live_properties[i].prop, b_200);
		}
	}	return 0;
}

 void smbdav_propfind_prepare_response_bulk(server *srv, connection *con, plugin_data *p,
	buffer *b, physical *d, buffer *prop_200, buffer *prop_404)
{
	buffer_append_string_len(b,CONST_STR_LEN("<D:response>\n"));
	buffer_append_string_len(b,CONST_STR_LEN("<D:href>"));
	buffer_append_string_buffer(b, con->uri.scheme);
	buffer_append_string_len(b,CONST_STR_LEN("://"));
	buffer_append_string_buffer(b, con->uri.authority);
	
	if( con->share_link_shortpath->used ){
		char buff[4096];
		char* tmp = replace_str(&d->rel_path->ptr[0], 
			                    (char*)con->share_link_realpath->ptr, 
			                    (char*)con->share_link_shortpath->ptr, 
				                (char *)&buff[0]);
						
		buffer_append_string(b, "/");
		buffer_append_string_encoded(b, tmp, strlen(tmp), ENCODING_REL_URI);		
	}
	else{
		//- 20111227
		buffer_append_string_encoded(b, CONST_BUF_LEN(d->rel_path), ENCODING_REL_URI);		
	}
	
	buffer_append_string_len(b,CONST_STR_LEN("</D:href>\n"));

	if (!buffer_is_empty(prop_200)) {
		buffer_append_string_len(b,CONST_STR_LEN("<D:propstat>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<D:prop>\n"));

		buffer_append_string_buffer(b, prop_200);

		buffer_append_string_len(b,CONST_STR_LEN("</D:prop>\n"));

		buffer_append_string_len(b,CONST_STR_LEN("<D:status>HTTP/1.1 200 OK</D:status>\n"));

		buffer_append_string_len(b,CONST_STR_LEN("</D:propstat>\n"));
	}
	if (!buffer_is_empty(prop_404)) {
		buffer_append_string_len(b,CONST_STR_LEN("<D:propstat>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<D:prop>\n"));

		buffer_append_string_buffer(b, prop_404);

		buffer_append_string_len(b,CONST_STR_LEN("</D:prop>\n"));

		buffer_append_string_len(b,CONST_STR_LEN("<D:status>HTTP/1.1 404 Not Found</D:status>\n"));

		buffer_append_string_len(b,CONST_STR_LEN("</D:propstat>\n"));
	}

	buffer_append_string_len(b,CONST_STR_LEN("</D:response>\n"));

	return;						

}

int smbc_list_directory(server *srv, connection *con, plugin_data *p, buffer *dir) 
{
	int dh;
	buffer *out;	
	data_string *ds_userAgent = (data_string *)array_get_element(con->request.headers, "user-Agent");
	
	out = chunkqueue_get_append_buffer(con->write_queue);
	buffer_copy_string_len(out, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\""));
	buffer_append_string_len(out, CONST_STR_LEN("utf-8"));
	buffer_append_string_len(out, CONST_STR_LEN("\"?>\n"));
#if 1
	/*
	if( ds_userAgent && 		
		( //strstr( ds_userAgent->value->ptr, "Chrome" ) ||
		  strstr( ds_userAgent->value->ptr, "iPhone" ) || 
		  strstr( ds_userAgent->value->ptr, "iPad"   ) || 
		  strstr( ds_userAgent->value->ptr, "iPod"   ) ||
		  strstr( ds_userAgent->value->ptr, "Android") ) ){
	*/
	if(buffer_is_equal_string(con->uri.query, CONST_STR_LEN("mobile=1"))){
		buffer_append_string_len(out, CONST_STR_LEN(
				"<!DOCTYPE html>\n"
				"<html>\n"
				"<head>\n"
				"<meta charset=\"utf-8\"/>\n"
				"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"
				"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\" />"
				"<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n"
				"<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"black\">\n"
				"<link rel=\"apple-touch-icon\" href=\"/smb/css/appicon.png\">\n"
				"<link rel=\"apple-touch-startup-image\" href=\"/smb/css/startup.png\">\n"
				"<title>AiCloud</title>\n"
			));

	
		//- Mobile UI
		buffer_append_string_len(out, CONST_STR_LEN(
				"<link rel='stylesheet' id='mainCss' href='/smb/css/style-theme1.app.css' type='text/css'/>\n"
				"<script type='text/javascript' src='/smb/js/tools.app.js'></script>\n"
				"<script type='text/javascript' src='/smb/js/davclient_tools.js'></script>\n"			
				"<script type='text/javascript' src='/smb/js/smbdav-main.app.min.js'></script>\n"
			));

		buffer_append_string_len(out, CONST_STR_LEN("</head>"));
		buffer_append_string_len(out, CONST_STR_LEN("<body class='ui-mobile-viewport ui-overlay-c'>"));

		buffer_append_string_len(out, CONST_STR_LEN("<div id='mainpage' data-role='page' data-theme='a' data-fullscreen='false'>")); 

		//- header
		buffer_append_string_len(out, CONST_STR_LEN("<div id='header' data-role='header' data-position='fixed'>"));
		buffer_append_string_len(out, CONST_STR_LEN("<div id='light'></div>"));	
		buffer_append_string_len(out, CONST_STR_LEN("<span id='title-logo'>ASUS AiCloud</span>"));	
		buffer_append_string_len(out, CONST_STR_LEN("<span id='title-welcome'>Welcome home, <span id='username'>admin</span></span>"));	
		buffer_append_string_len(out, CONST_STR_LEN("<div id='options'>"));
		buffer_append_string_len(out, CONST_STR_LEN("<div id='btn-changeuser' class='bicon'></div>"));
		buffer_append_string_len(out, CONST_STR_LEN("<div id='btn-select' class='bicon'></div>"));	
		buffer_append_string_len(out, CONST_STR_LEN("</div>"));
		buffer_append_string_len(out, CONST_STR_LEN("</div>"));

		//- content
		buffer_append_string_len(out, CONST_STR_LEN("<div id='content' data-role='content'>"));
		buffer_append_string_len(out, CONST_STR_LEN("<ul data-role='listview' id='list' data-inset='false'>"));
		buffer_append_string_len(out, CONST_STR_LEN("</ul>"));		
		buffer_append_string_len(out, CONST_STR_LEN("</div>"));

		//- footer
		buffer_append_string_len(out, CONST_STR_LEN("<div id='footer' data-role='footer' data-position='fixed'>"));
		buffer_append_string_len(out, CONST_STR_LEN("<div id='navbar' data-role='navbar' style='display:block'>"));
		buffer_append_string_len(out, CONST_STR_LEN("<ul>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-home' href='#' data-role='button' data-icon='cloud-home' data-iconpos='top' data-corners='false'>Home</a>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-refresh' href='#' data-role='button' data-icon='cloud-refresh' data-iconpos='top' data-corners='false'>Refresh</a>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-logout' href='#' data-role='button' data-icon='cloud-logout' data-iconpos='top' data-corners='false'>Logout</a>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-setting' href='#' data-role='button' data-icon='cloud-setting' data-iconpos='top' data-corners='false'>Setting</a>"));		
		buffer_append_string_len(out, CONST_STR_LEN("</ul>"));
		buffer_append_string_len(out, CONST_STR_LEN("</div>"));		
		buffer_append_string_len(out, CONST_STR_LEN("<div id='navbar2' data-role='navbar' style='display:none'>"));
		buffer_append_string_len(out, CONST_STR_LEN("<ul>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-delete' href='#' data-role='button' data-icon='cloud-delete' data-iconpos='top' data-corners='false'>Delete</a>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-rename' href='#' data-role='button' data-icon='cloud-rename' data-iconpos='top' data-corners='false'>Rename</a>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-share' href='#' data-role='button' data-icon='cloud-share' data-iconpos='top' data-corners='false'>Share</a>"));
		buffer_append_string_len(out, CONST_STR_LEN("<li><a id='btn-createfolder' href='#' data-role='button' data-icon='cloud-createfolder' data-iconpos='top' data-corners='false'>Add Folder</a>"));
		buffer_append_string_len(out, CONST_STR_LEN("</ul>"));		
		buffer_append_string_len(out, CONST_STR_LEN("</div>"));
		buffer_append_string_len(out, CONST_STR_LEN("</div>"));
		
		buffer_append_string_len(out, CONST_STR_LEN("</div>"));
		
		buffer_append_string_len(out, CONST_STR_LEN("</body>"));
	}
	else
#endif
	{
		buffer_append_string_len(out, CONST_STR_LEN(
			"<!DOCTYPE html>\n"
			"<html>\n"
			"<head>\n"
			"<meta charset=\"utf-8\"/>\n"
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"
                        "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
			//"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
			//"<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n"
			//"<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"black\">\n"
			//"<link rel=\"apple-touch-icon\" href=\"/smb/css/appicon.png\">\n"
			//"<link rel=\"apple-touch-startup-image\" href=\"/smb/css/startup.png\">\n"
			"<title>AiCloud</title>\n"
		));
		
		//- General UI
		buffer_append_string_len(out, CONST_STR_LEN(
				"<link rel='stylesheet' id='mainCss' href='/smb/css/style-theme.css' type='text/css'/>\n"
				"<link rel='stylesheet' id='mainCss' href='/smb/css/jplayer.blue.monday.css' type='text/css'/>\n"
				"<script type='text/javascript' src='/smb/js/tools.js'></script>\n"
				"<script type='text/javascript' src='/smb/js/davclient_tools.js'></script>\n"			
				"<script type='text/javascript' src='/smb/js/smbdav-main.min.js'></script>\n"
			));

		if( buffer_is_equal_string(con->url.rel_path, CONST_STR_LEN("/"))){
			buffer_append_string_len(out, CONST_STR_LEN("</head>\n<body></body>\n"));
		}
		else{
			buffer_append_string_len(out, CONST_STR_LEN("</head>\n<body openurl='"));
			buffer_append_string( out, con->url.rel_path->ptr );
			buffer_append_string_len(out, CONST_STR_LEN("' fileview_only='1'></body>\n"));
		}
	}

	/* Insert possible charset to Content-Type */
	response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/html; charset=UTF-8"));

	con->file_finished = 1;

	return 0;
}

int smbc_list_host(server *srv, connection *con, 
	smbdav_properties *req_props,
	plugin_data *p, buffer *b, 
	buffer *prop_200,
	buffer *prop_404) 
{
	int dir;	
	int count = 0;
	smb_srv_info_t *c;
	physical d;	
	physical *dst = &con->url;
	
#ifdef SCAN_MASTER_BROWSER
	//- Count samba server
	for (c = smb_srv_info_list; c; c = c->next) {
		count++;
	}

	if(count<=0){
		if (NULL != (dir = smbc_wrapper_opendir(con, con->url.path->ptr))) {
			struct smbc_dirent *de;
			physical d;
			
			d.path = buffer_init();
			d.rel_path = buffer_init();

			while(NULL != (de = smbc_wrapper_readdir(con, dir))) {
				char workgroup[100];
				DIR *subdir;
				struct smbc_dirent *subde;
				sprintf(workgroup, "smb://%s/", de->name);

				if (NULL != (subdir = smbc_wrapper_opendir(con, workgroup))) {
					while(NULL != (subde = smbc_wrapper_readdir(con, subdir))) {

						buffer_copy_string_buffer(d.path, dst->path);
						BUFFER_APPEND_SLASH(d.path);

						buffer_copy_string_buffer(d.rel_path, dst->rel_path);
						BUFFER_APPEND_SLASH(d.rel_path);

						if (subde->name[0] == '.' && subde->name[1] == '\0') {
								/* don't append the . */
						} else {
							buffer_append_string(d.path, subde->name);
							buffer_append_string(d.rel_path, subde->name);
						}

						buffer_reset(prop_200);
						buffer_reset(prop_404);

						smbdav_get_share_props(srv, con, p, &d, NULL, NULL, 0, 1, req_props, prop_200, prop_404);
						smbdav_propfind_prepare_response_bulk(srv, con, p, b, &d, prop_200, prop_404);
					}
					smbc_wrapper_closedir(con, subdir);
				}
			}
			smbc_wrapper_closedir(con, dir);
			buffer_free(d.path);
			buffer_free(d.rel_path);
		}
		
		return 0;
	}
#endif

	d.path = buffer_init();
	d.rel_path = buffer_init();
		
	//- 20111209 Jerry add
	//- Add usbdisk	
	char *usbdisk_name;
#if EMBEDDED_EANBLE
	char *a = nvram_get_productid();
	int l = strlen(a)+1;
	usbdisk_name = (char*)malloc(l);
	memset(usbdisk_name,'\0', l);
	strcpy(usbdisk_name, a);
	#ifdef APP_IPKG
	free(a);
	#endif
#else
	usbdisk_name = (char*)malloc(8);
	memset(usbdisk_name,'\0', 8);
	strcpy(usbdisk_name, "usbdisk");
#endif
		
	if( is_usbdisk_exist( srv, con, usbdisk_name ) == 1 ){

		if (NULL != (dir = opendir("/mnt"))) {
			struct dirent *de;
			physical d;
			physical *dst = &(con->physical);

			d.path = buffer_init();
			d.rel_path = buffer_init();

			while(NULL != (de = readdir(dir))) {
				if ( de->d_name[0] == '.' && de->d_name[1] == '.' && de->d_name[2] == '\0' ) {
					continue;
					/* ignore the parent dir */
				}

				if ( de->d_name[0] == '.' ) {
					continue;
					/* ignore the hidden file */
				}
				
				if ( strcmp(de->d_name, "minidlna")==0 ||
					 strcmp(de->d_name, "asusware")==0) {
					continue;
				}
				
				buffer_copy_string(d.path, usbdisk_name);
				BUFFER_APPEND_SLASH(d.path);
				
				buffer_copy_string(d.rel_path, "/");
				buffer_append_string(d.rel_path, usbdisk_name);
				BUFFER_APPEND_SLASH(d.rel_path);
				
				if (de->d_name[0] == '.' && de->d_name[1] == '\0') {
					/* don't append the . */
				} else {
					buffer_append_string(d.path, de->d_name);
					buffer_append_string(d.rel_path, de->d_name);
				}

				Cdbg(1,"path=%s, rel_path=%s", d.path->ptr, d.rel_path->ptr);
				buffer* buffer_ip = buffer_init();
				char ip[16]="\0";
				
				buffer* buffer_mac = buffer_init();
#if EMBEDDED_EANBLE
				char* mac = nvram_get_router_mac();
				buffer_copy_string(buffer_mac, mac);

				#ifdef APP_IPKG
				if(mac) free(mac);
				#endif
#else
				char mac[20]="\0";
				get_mac_address("eth0", &mac);
				buffer_copy_string(buffer_mac, mac);
#endif

				buffer_reset(prop_200);
				buffer_reset(prop_404);					
				smbdav_get_share_props(srv, con, p, &d, buffer_ip, buffer_mac, 1, 2, req_props, prop_200, prop_404);
				smbdav_propfind_prepare_response_bulk(srv, con, p, b, &d, prop_200, prop_404);

				buffer_free(buffer_ip);
				buffer_free(buffer_mac);
			}
		}		
	}

	free(usbdisk_name);
	
	//- Only show usbdisk if account right is not equal 0.
	//if( con->smb_info->auth_right != 1 )
	//	return 0;

#if EMBEDDED_EANBLE
	#ifndef APP_IPKG
	if(strcmp( nvram_get_webdavproxy(), "0" ) == 0)
		return 0;
	#else
	char *wwebdavproxy = nvram_get_webdavproxy();
    if(strcmp( wwebdavproxy, "0" ) == 0)
		return 0;
    free(wwebdavproxy);
	#endif
	char* aa = nvram_get_smbdav_str();
	
	if(aa==NULL){
		buffer_free(d.path);
		buffer_free(d.rel_path);
		return;
	}
	
	char* str_smbdav_list = (char*)malloc(strlen(aa)+1);
	strcpy(str_smbdav_list, aa);
	#ifdef APP_IPKG
	free(aa);
	#endif
	if(str_smbdav_list!=NULL){
		char * pch1;
		char * pch;
		pch = strtok(str_smbdav_list, "<>");	
		
		while(pch!=NULL){
			
			char name[20], ip[20], mac[50];
			int name_len, ip_len, mac_len, online;
			
			//- PC Name
			name_len = strlen(pch);
			strncpy(name, pch, name_len);
			name[name_len] = '\0';
			//fprintf(stderr,"name=%s\n", pch);
			
			//- IP Address
			pch = strtok(NULL,"<>");
			ip_len = strlen(pch);
			strncpy(ip, pch, ip_len);
			ip[ip_len] = '\0';
			buffer* buffer_ip = buffer_init();
			buffer_copy_string(buffer_ip, ip);
			//fprintf(stderr,"ip=%s\n", pch);
			
			//- MAC Address
			pch = strtok(NULL,"<>");
			mac_len = strlen(pch);
			strncpy(mac, pch, mac_len);
			mac[mac_len] = '\0';
			buffer* buffer_mac = buffer_init();
			buffer_copy_string(buffer_mac, mac);
			//fprintf(stderr,"mac=%s\n", pch);
			
			//- PC Online?
			pch = strtok(NULL,"<>");
			online = atoi(pch);
			buffer_copy_string_buffer(d.path, dst->path);
			BUFFER_APPEND_SLASH(d.path);
																		
			buffer_copy_string_buffer(d.rel_path, dst->rel_path);
			BUFFER_APPEND_SLASH(d.rel_path);
				
			buffer_append_string(d.path, name);
			buffer_append_string(d.rel_path, name);

			//Cdbg(DBE, "dst->path=[%s], online=[%d]", dst->path->ptr, online);
			//Cdbg(DBE, "d.path=[%s], d.rel_path=[%s]", d.path->ptr, d.rel_path->ptr);
			
			buffer_reset(prop_200);
			buffer_reset(prop_404);
			
			smbdav_get_share_props(srv, con, p, &d, buffer_ip, buffer_mac, online, 1, req_props, prop_200, prop_404);
			smbdav_propfind_prepare_response_bulk(srv, con, p, b, &d, prop_200, prop_404);

			buffer_free(buffer_ip);
			buffer_free(buffer_mac);
						
			pch = strtok(NULL,"<>");
			//fprintf(stderr,"next pch=%s\n\n", pch);
		}

		free(str_smbdav_list);
	}
#else
	size_t j;
	int length, filesize;
	char* g_temp_file = "/tmp/arpping_list";
	FILE* fp = fopen(g_temp_file, "r");
	if(fp!=NULL){		
		
		char str[1024];

		while(fgets(str,sizeof(str),fp) != NULL)
		{
      		// strip trailing '\n' if it exists
      		int len = strlen(str)-1;
      		if(str[len] == '\n') 
         		str[len] = 0;

			char name[20], ip[20], mac[50];
			int name_len, ip_len, mac_len, online;
			char * pch;

			//- PC Name
			pch = strtok(str,"<");
			name_len = strlen(pch);
			strncpy(name, pch, name_len);
			name[name_len] = '\0';
			
			//- IP Address
			pch = strtok(NULL,"<");
			ip_len = strlen(pch);
			strncpy(ip, pch, ip_len);
			ip[ip_len] = '\0';
			buffer* buffer_ip = buffer_init();
			buffer_copy_string(buffer_ip, ip);

			//- MAC Address
			pch = strtok(NULL,"<");
			mac_len = strlen(pch);
			strncpy(mac, pch, mac_len);
			mac[mac_len] = '\0';
			buffer* buffer_mac = buffer_init();
			buffer_copy_string(buffer_mac, mac);

			//- PC Online?
			pch = strtok(NULL,"<");
			online = atoi(pch);
			buffer_copy_string_buffer(d.path, dst->path);
			BUFFER_APPEND_SLASH(d.path);
																		
			buffer_copy_string_buffer(d.rel_path, dst->rel_path);
			BUFFER_APPEND_SLASH(d.rel_path);

			buffer_append_string(d.path, name);
			buffer_append_string(d.rel_path, name);

			//Cdbg(DBE, "dst->path=[%s], online=[%d]", dst->path->ptr, online);
			Cdbg(DBE, "d.path=[%s], d.rel_path=[%s]", d.path->ptr, d.rel_path->ptr);
					
			buffer_reset(prop_200);
			buffer_reset(prop_404);
			
			smbdav_get_share_props(srv, con, p, &d, buffer_ip, buffer_mac, online, 1, req_props, prop_200, prop_404);
			smbdav_propfind_prepare_response_bulk(srv, con, p, b, &d, prop_200, prop_404);

			Cdbg(DBE, "b=%s", b->ptr);
			
			buffer_free(buffer_ip);
			buffer_free(buffer_mac);
		}
			
		fclose(fp);
	}
#endif

	buffer_free(d.path);
	buffer_free(d.rel_path);

}


URIHANDLER_FUNC(mod_smbdav_subrequest_handler) {
	plugin_data *p = p_d;
	buffer *b;
	DIR *dir;
	data_string *ds;
	int depth = -1;
	struct stat st;
	buffer *prop_200;
	buffer *prop_404;
	smbdav_properties *req_props;
	stat_cache_entry *sce = NULL;
	handler_t res = HANDLER_GO_ON;
	
#if 0
	//UNUSED(srv);
	Cdbg(DBE,"enter mod_smbdav_subrequest_handler..");
	Cdbg(DBE, "con->physical.path = %s", con->physical.path->ptr);
	Cdbg(DBE, "con->uri->path = %s", con->uri.path->ptr);
#endif

	if (!p->conf.enabled) {
		return HANDLER_GO_ON;
	}

	/* physical path is setup */
	if (con->physical.path->used == 0) {
		return HANDLER_GO_ON;
	}

	if (con->url.path->used == 0) {
		return HANDLER_GO_ON;
	}

	/* PROPFIND need them */
	if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "Depth"))) {
		depth = strtol(ds->value->ptr, NULL, 10);
	}

	if(!con->smb_info){
		return HANDLER_GO_ON;
	}
	
	Cdbg(DBE, "http_method=[%d][%s], depth=[%d], con->smb_info->qflag=[%d]", 
		con->request.http_method, get_http_method_name(con->request.http_method), 
		depth,
		con->smb_info->qflag );	
	Cdbg(DBE, "con->url.path = %s", con->url.path->ptr);
	
	switch (con->request.http_method) {
	case HTTP_METHOD_OPTIONS:
		/* we fake a little bit but it makes MS W2k happy and it let's us mount the volume */
		response_header_overwrite(srv, con, CONST_STR_LEN("DAV"), CONST_STR_LEN("1,2"));
		response_header_overwrite(srv, con, CONST_STR_LEN("MS-Author-Via"), CONST_STR_LEN("DAV"));
		return HANDLER_FINISHED;
	case HTTP_METHOD_HEAD:
	      /**
	        * a HEAD request has the same as a GET
	        * without the content
	        */
		con->http_status = 200;
		con->file_finished = 1;
		chunkqueue_reset(con->write_queue);
		con->response.transfer_encoding &= ~HTTP_TRANSFER_ENCODING_CHUNKED;
		return HANDLER_FINISHED;
	case HTTP_METHOD_GET:
		switch(con->smb_info->qflag) {
		case SMB_SHARE_QUERY:
		case SMB_HOST_QUERY:{
			smbc_list_directory(srv, con, p, con->url.path);
			con->http_status = 200;
			con->file_finished = 1;
			joblist_append(srv, con);
			return HANDLER_FINISHED;
		}	

		case SMB_FILE_QUERY:{
			if (-1 == smbc_wrapper_stat(con, con->url.path->ptr, &st)) {
				/* don't about it yet, rmdir will fail too */
				return HANDLER_FINISHED;
			} 
			else if (S_ISDIR(st.st_mode)) {
				smbc_list_directory(srv, con, p, con->url.path);
				con->http_status = 200;
				con->file_finished = 1;
				joblist_append(srv, con);
				return HANDLER_FINISHED;
			}
			else{
				log_sys_write(srv, "sbss", "Download samba file", con->url.rel_path, "from ip", con->dst_addr_buf->ptr);
				Cdbg(DBE, "METHOD_GET->SMB_FILE_QUERY: HANDLER_GO_ON");
				return HANDLER_GO_ON;
			}
		}
		
		default: //leave the FILE_QUERY to system default behavior
			return HANDLER_GO_ON;
		}
	case HTTP_METHOD_PROPFIND:
		/* they want to know the properties of the directory */
		req_props = NULL;
		if(con->smb_info->qflag == SMB_FILE_QUERY) {
			/* is there a content-body ? */			
			
			if( smbc_wrapper_stat(con, con->url.path->ptr, &st) < 0 ) {
				
				if(errno == EACCES) {
					smbc_wrapper_response_401(srv, con);
				} else {				
					//if (errno == ENOENT) 
					con->http_status = 404;
					Cdbg(DBE, "con->http_status = 404");
				}
				return HANDLER_FINISHED;
			}
		}

#ifdef USE_PROPPATCH		
		/* any special requests or just allprop ? */
		if (con->request.content_length) {
			xmlDocPtr xml;
			if (1 == smbdav_parse_chunkqueue(srv, con, p, con->request_content_queue, &xml)) {

				xmlNode *rootnode = xmlDocGetRootElement(xml);

				assert(rootnode);

				if (0 == xmlStrcmp(rootnode->name, BAD_CAST "propfind")) {
					xmlNode *cmd;					

					req_props = calloc(1, sizeof(*req_props));

					for (cmd = rootnode->children; cmd; cmd = cmd->next) {
						
						if (0 == xmlStrcmp(cmd->name, BAD_CAST "prop")) {
							/* get prop by name */
							xmlNode *prop;
							
							for (prop = cmd->children; prop; prop = prop->next) {
								if (prop->type == XML_TEXT_NODE) continue; /* ignore WS */

								if (prop->ns &&
								    (0 == xmlStrcmp(prop->ns->href, BAD_CAST "")) &&
								    (0 != xmlStrcmp(prop->ns->prefix, BAD_CAST ""))) {
									size_t i;
									log_error_write(srv, __FILE__, __LINE__, "ss",
											"no name space for:",
											prop->name);

									xmlFreeDoc(xml);

									for (i = 0; i < req_props->used; i++) {
										free(req_props->ptr[i]->ns);
										free(req_props->ptr[i]->prop);
										free(req_props->ptr[i]);
									}
									free(req_props->ptr);
									free(req_props);

									con->http_status = 400;
									return HANDLER_FINISHED;
								}

								/* add property to requested list */
								if (req_props->size == 0) {
									req_props->size = 16;
									req_props->ptr = malloc(sizeof(*(req_props->ptr)) * req_props->size);
								} else if (req_props->used == req_props->size) {
									req_props->size += 16;
									req_props->ptr = realloc(req_props->ptr, sizeof(*(req_props->ptr)) * req_props->size);
								}

								req_props->ptr[req_props->used] = malloc(sizeof(smbdav_property));
								req_props->ptr[req_props->used]->ns = (char *)xmlStrdup(prop->ns ? prop->ns->href : (xmlChar *)"");
								req_props->ptr[req_props->used]->prop = (char *)xmlStrdup(prop->name);
								
								req_props->used++;
							}
						} else if (0 == xmlStrcmp(cmd->name, BAD_CAST "propname")) {
							sqlite3_stmt *stmt = p->conf.stmt_select_propnames;

							if (stmt) {
								/* get all property names (EMPTY) */
								sqlite3_reset(stmt);
								/* bind the values to the insert */

								sqlite3_bind_text(stmt, 1,
										  con->uri.path->ptr,
										  con->uri.path->used - 1,
										  SQLITE_TRANSIENT);

								if (SQLITE_DONE != sqlite3_step(stmt)) {
								}
							}
						} else if (0 == xmlStrcmp(cmd->name, BAD_CAST "allprop")) {
							/* get all properties (EMPTY) */
							req_props = NULL;
						}
					}
				}

				xmlFreeDoc(xml);
			} else {
				con->http_status = 400;
				return HANDLER_FINISHED;
			}
		}
#endif

		con->http_status = 207;

		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));

		b = chunkqueue_get_append_buffer(con->write_queue);

		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<D:multistatus xmlns:D=\"DAV:\" xmlns:ns0=\"urn:uuid:c2f41010-65b3-11d1-a29f-00aa00c14882/\" "));
		
	#if 0
		//- Key
		buffer_append_string_len(b, CONST_STR_LEN(" key=\""));
		char mac[20]="\0";
		get_mac_address("eth0", &mac);
		char* base64_key = ldb_base64_encode(mac, strlen(mac));
		buffer_append_string(b, base64_key);
		free(base64_key);
		
		//- Auth		
		buffer_append_string_len(b, CONST_STR_LEN("\" auth=\""));
		char auth_buf[100]="\0";
		if(con->smb_info)
			sprintf(auth_buf, "%s:%s", con->smb_info->username->ptr, con->smb_info->password->ptr);
		char* base64_auth = ldb_base64_encode(auth_buf, strlen(auth_buf));
		buffer_append_string(b, base64_auth);
		free(base64_auth);

		buffer_append_string_len(b, CONST_STR_LEN("\""));
	#endif
	
		//- Read Only
		buffer_append_string_len(b, CONST_STR_LEN(" readonly=\"0"));
	
		//- Router user name
		buffer_append_string_len(b, CONST_STR_LEN("\" ruser=\""));
		data_string *ds = (data_string *)array_get_element(con->request.headers, "user-Agent");
		if(ds!=NULL){
			smb_info_t *c;
			for (c = srv->smb_srv_info_list; c; c = c->next) {

				if( buffer_is_equal(c->user_agent, ds->value) && 
				    buffer_is_equal(c->src_ip, con->dst_addr_buf) &&
				    buffer_is_empty(c->server) ){
					buffer_append_string(b, c->username->ptr);
					break;
				}
			
			}
		}
		
		//- Query Type
		buffer_append_string_len(b, CONST_STR_LEN("\" qtype=\""));
		char qtype[2]="\0";
		sprintf(qtype, "%d", con->smb_info->qflag);
		buffer_append_string(b, qtype);

	#if EMBEDDED_EANBLE
		//- Computer Name
		#ifndef APP_IPKG
		buffer_append_string_len(b, CONST_STR_LEN("\" computername=\""));
		buffer_append_string(b, nvram_get_computer_name());
		#else
		char *computer_name = nvram_get_computer_name();
		buffer_append_string_len(b, CONST_STR_LEN("\" computername=\""));
        buffer_append_string(b, computer_name);
        //buffer_append_string(b, nvram_get_computer_name());
        free(computer_name);
		#endif
	#else
		//- Computer Name
		buffer_append_string_len(b, CONST_STR_LEN("\" computername=\"WebDAV"));
	#endif

		buffer_append_string_len(b, CONST_STR_LEN("\" isusb=\"0"));
	
		buffer_append_string_len(b,CONST_STR_LEN("\">\n"));
		
		/* allprop */
		prop_200 = buffer_init();
		prop_404 = buffer_init();
		
		switch(depth) {
		case 0:			
			/* Depth: 0 */
			switch(con->smb_info->qflag) {			
			case SMB_FILE_QUERY:
				smbdav_get_props(srv, con, p, &con->url, req_props, prop_200, prop_404);
				break;
			case SMB_SHARE_QUERY:
			case SMB_HOST_QUERY:
				smbdav_get_share_props(srv, con, p, &con->url, NULL, NULL, 0, 1, req_props, prop_200, prop_404);
				break;
			}
			smbdav_propfind_prepare_response_bulk(srv, con, p, b, &(con->physical), prop_200, prop_404);
			
			break;
		case 1:
			switch(con->smb_info->qflag) {
			case SMB_FILE_QUERY: 
			{
				if (-1 != (dir = smbc_wrapper_opendir(con, con->url.path->ptr))) {
					struct smbc_dirent *de;
					physical d;
					
					physical *dst = &(con->url);
					
					if(!buffer_is_empty(con->match_smb_ip)){
						int index = strstr(dst->rel_path->ptr, con->match_smb_ip->ptr) - dst->rel_path->ptr;			
						if( index==1 ){
							char buff[4096];
							char* tmp = replace_str(dst->rel_path->ptr, 
												    con->match_smb_ip->ptr, 
												    con->replace_smb_name->ptr, 
												    (char *)&buff[0]);
											
							buffer_copy_string(dst->rel_path, tmp);
						}
					}
		
					d.path = buffer_init();
					d.rel_path = buffer_init();

					while(NULL != (de = smbc_wrapper_readdir(con, dir))) {
						if( (de->name[0] == '.' && de->name[1] == '.' && de->name[2] == '\0') ){
							continue;
							//- ignore the parent dir
						}

						if ( de->name[0] == '.' ) {
							continue;
							//- ignore the hidden file
						}
						
						buffer* de_name = buffer_init();
						buffer_copy_string(de_name, de->name);

						buffer_copy_string_buffer(d.path, dst->path);
						BUFFER_APPEND_SLASH(d.path);

						buffer_copy_string_buffer(d.rel_path, dst->rel_path);
						BUFFER_APPEND_SLASH(d.rel_path);
						
						if (de->name[0] == '.' && de->name[1] == '\0') {
							/* don't append the . */
						} else {
							buffer_append_string_buffer(d.path, de_name);
							buffer_append_string_buffer(d.rel_path, de_name);
						}
			
						buffer_reset(prop_200);
						buffer_reset(prop_404);
	
						//Cdbg(DBE,"SMB_FILE_QUERY: de->name %s, d.path %s", de_name->ptr, d.path->ptr);

						buffer_free(de_name);
						
						smbdav_get_props(srv, con, p, &d, req_props, prop_200, prop_404);
						smbdav_propfind_prepare_response_bulk(srv, con, p, b, &d, prop_200, prop_404);						
						
					}
					smbc_wrapper_closedir(con,dir);
					buffer_free(d.path);
					buffer_free(d.rel_path);
				}
				else{
					smbc_wrapper_response_401(srv, con);
				}
				
				break;
			}
			
			case SMB_SHARE_QUERY: 
			{
				if (-1 != (dir = smbc_wrapper_opensharedir(con, con->url.path->ptr))) {
					struct smbc_dirent *de;
					physical d;
					physical *dst = &(con->physical);

					d.path = buffer_init();
					d.rel_path = buffer_init();
					
					while(NULL != (de = smbc_wrapper_readdir(con, dir))) {
						if( (de->name[0] == '.' && de->name[1] == '.' && de->name[2] == '\0')/* ||
							( strstr(de->name, "$") ) */ ){
							continue;
							/* ignore the parent dir */
						}
						
						buffer* de_name = buffer_init();
						//buffer_copy_string(de_name, "");						
						//buffer_append_string_encoded(de_name, de->name, strlen(de->name), ENCODING_REL_URI);
						buffer_copy_string(de_name, de->name);

						
						buffer_copy_string_buffer(d.path, dst->path);
						BUFFER_APPEND_SLASH(d.path);

						buffer_copy_string_buffer(d.rel_path, dst->rel_path);
						BUFFER_APPEND_SLASH(d.rel_path);

						if( (de->name[0] == '.' && de->name[1] == '\0') ||
 						    strstr(de->name, "$") ){
							//- don't append the .
							//continue;
						} else {
							//buffer_append_string(d.path, de->name);
							//buffer_append_string(d.rel_path, de->name);

							buffer_append_string_buffer(d.path, de_name);
							buffer_append_string_buffer(d.rel_path, de_name);			
						}

						//Cdbg(DBE,"SMB_SHARE_QUERY: de->name %s, d.path %s", de_name->ptr, d.path->ptr);

						buffer_free(de_name);
						
						buffer_reset(prop_200);
						buffer_reset(prop_404);

						smbdav_get_share_props(srv, con, p, &d, NULL, NULL, 0, 1, req_props, prop_200, prop_404);
						smbdav_propfind_prepare_response_bulk(srv, con, p, b, &d, prop_200, prop_404);
					}
					
					smbc_wrapper_closedir(con, dir);
					buffer_free(d.path);
					buffer_free(d.rel_path);
				}
				else{
					smbc_wrapper_response_401(srv, con);
				}
				
				break;
			}
			
			default:
			case SMB_HOST_QUERY:	
			{
				smbc_list_host(srv, con, req_props, p, b, prop_200, prop_404);				
				break;
			}
			
			}
			break;
		default:
			break;
		}
		
		if (req_props) {
			size_t i;
			for (i = 0; i < req_props->used; i++) {
				free(req_props->ptr[i]->ns);
				free(req_props->ptr[i]->prop);
				free(req_props->ptr[i]);
			}
			free(req_props->ptr);
			free(req_props);
		}

		buffer_free(prop_200);
		buffer_free(prop_404);

		buffer_append_string_len(b,CONST_STR_LEN("</D:multistatus>\n"));

		if (p->conf.log_xml) {
			log_error_write(srv, __FILE__, __LINE__, "sb", "XML-response-body:", b);
		}
		con->file_finished = 1;
		return HANDLER_FINISHED;
		
	case HTTP_METHOD_MKCOL:
		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		if (con->request.content_length != 0) {
			/* we don't support MKCOL with a body */
			con->http_status = 415;

			return HANDLER_FINISHED;
		}
		
		/* let's create the directory */
		if (-1 == smbc_wrapper_mkdir(con, con->url.path->ptr, WEBDAV_DIR_MODE)) {
			switch(errno) {
			case EPERM:
				con->http_status = 403;
				break;
			case ENOENT:
			case ENOTDIR:
				con->http_status = 409;
				break;
			case EEXIST:
			default:
				con->http_status = 405; /* not allowed */
				break;
			}
		} else {
			log_sys_write(srv, "sbss", "Create folder", con->url.rel_path, "from ip", con->dst_addr_buf->ptr);
			con->http_status = 201;
			con->file_finished = 1;
		}
		
		return HANDLER_FINISHED;

		
	case HTTP_METHOD_DELETE:
		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		/* does the client have a lock for this connection ? */
		if (!smbdav_has_lock(srv, con, p, con->uri.path)) {
			con->http_status = 423;
			return HANDLER_FINISHED;
		}
					
		/* stat and unlink afterwards */		
		if (-1 == smbc_wrapper_stat(con, con->url.path->ptr, &st)) {			
			/* don't about it yet, unlink will fail too */
			switch(errno) {
			case ENOENT:
				 con->http_status = 404;
				 break;
			default:
				 con->http_status = 403;
				 break;
			}
		}
		else if (S_ISDIR(st.st_mode)) {				
			buffer *multi_status_resp = buffer_init();			
			if (smbdav_delete_dir(srv, con, p, &(con->url), multi_status_resp)) {
				#if 0
				// we got an error somewhere in between, build a 207 
				response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));

				b = chunkqueue_get_append_buffer(con->write_queue);

				buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));

				buffer_append_string_len(b,CONST_STR_LEN("<D:multistatus xmlns:D=\"DAV:\">\n"));

				buffer_append_string_buffer(b, multi_status_resp);
				Cdbg(DBE, "multi_status_resp = %s", multi_status_resp->ptr);
				buffer_append_string_len(b,CONST_STR_LEN("</D:multistatus>\n"));

				if (p->conf.log_xml) {
					log_error_write(srv, __FILE__, __LINE__, "sb", "XML-response-body:", b);
				}

				con->http_status = 207;
				con->file_finished = 1;
				#else
				con->http_status = 403;
				con->file_finished = 1;
				#endif
			} 
			else {				
				// everything went fine, remove the directory
				if (-1 == smbc_wrapper_rmdir(con, con->url.path->ptr)) {
					switch(errno) {
					case EPERM:
					case 13:
						con->http_status = 403;
						break;
					case ENOENT:
						con->http_status = 404;
						break;
					default:
						con->http_status = 501;
						break;
					}
				} else {
					log_sys_write(srv, "sbss", "Delete folder", con->url.rel_path, "from ip", con->dst_addr_buf->ptr);
					con->http_status = 204;
				}
			}

			buffer_free(multi_status_resp);
		}
		else if (-1 == smbc_wrapper_unlink(con, con->url.path->ptr)) {			
			switch(errno) {
			case EPERM:
			case 13:
				con->http_status = 403;
				break;
			case ENOENT:
				con->http_status = 404;
				break;
			default:
				con->http_status = 501;
				break;
			}
		} 
		else {
			log_sys_write(srv, "sbss", "Delete file", con->url.rel_path, "from ip", con->dst_addr_buf->ptr);
			con->http_status = 204;
		}
		return HANDLER_FINISHED;
		
	case HTTP_METHOD_PUT: {
		Cdbg(DBE,"========================================================> mode = %d",con->mode );
		int fd;
		chunkqueue *cq = con->request_content_queue;
		chunk *c;
		data_string *ds_range;

		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		/* is a exclusive lock set on the source */
		if (!smbdav_has_lock(srv, con, p, con->uri.path)) {
			con->http_status = 423;
			return HANDLER_FINISHED;
		}

		//- JerryLin add
		int autoCreateFolder = 0;
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "Auto-CreateFolder"))) {			
			if (ds->value->used != 2 ||
			    (ds->value->ptr[0] != 'F' &&
			     ds->value->ptr[0] != 'T') )  {
				con->http_status = 400;
				return HANDLER_FINISHED;
			}
			autoCreateFolder = (ds->value->ptr[0] == 'F' ? 0 : 1);
		}
		
		if(autoCreateFolder&&con->smb_info){
			/*
			char* file_path = NULL;
			extract_filepath(con->physical.path->ptr, &file_path);
			Cdbg(DBE, "aaaaaa file_path=%s", file_path);
			if (-1 == smbc_wrapper_mkdir_recursive(con, file_path, WEBDAV_DIR_MODE) ){
				con->http_status = 403;
			}
			
			free(file_path);
			*/

			char* at = strstr(con->url.path->ptr, "@");
			int prefix_len = at - con->url.path->ptr;
			buffer* buffer_prefix_url = buffer_init();
			buffer_copy_string_len(buffer_prefix_url,con->url.path->ptr,prefix_len+1);
			buffer_append_string_buffer(buffer_prefix_url, con->smb_info->server);
			buffer_append_string(buffer_prefix_url, "/");
			buffer_append_string_buffer(buffer_prefix_url, con->smb_info->share);
			buffer_append_string(buffer_prefix_url, "/");
			
			char* path;
			if( extract_filepath(con->smb_info->path->ptr, &path) ){			
				char* pch;
				pch = strtok(path, "/");
				while(pch!=NULL){
					
					buffer_append_string(buffer_prefix_url, pch);
					buffer_append_string(buffer_prefix_url, "/");
					Cdbg(DBE, "buffer_prefix_url=%s", buffer_prefix_url->ptr);
					smbc_mkdir(buffer_prefix_url->ptr, WEBDAV_DIR_MODE);
					
					pch = strtok( NULL, "/" );
				}
				free(path);
			}
			
			buffer_free(buffer_prefix_url);
		}
		////////////////////////////////
		
		assert(chunkqueue_length(cq) == (off_t)con->request.content_length);

		/* RFC2616 Section 9.6 PUT requires us to send 501 on all Content-* we don't support
		 * - most important Content-Range
		 *
		 *
		 * Example: Content-Range: bytes 100-1037/1038 */

		if (NULL != (ds_range = (data_string *)array_get_element(con->request.headers, "Content-Range"))) {
			const char *num = ds_range->value->ptr;
			off_t offset;
			char *err = NULL;
			Cdbg(DBE,"Content-Range =%s",num);

			if (0 != strncmp(num, "bytes ", 6)) {
				con->http_status = 501; /* not implemented */

				return HANDLER_FINISHED;
			}
			
			/* we only support <num>- ... */			
			num += 6;

			/* skip WS */
			while (*num == ' ' || *num == '\t') num++;
			
			if (*num == '\0') {
				con->http_status = 501; /* not implemented */
				return HANDLER_FINISHED;
			}
			
			offset = strtoll(num, &err, 10);

			if (*err != '-' || offset < 0) {
				con->http_status = 501; /* not implemented */
				return HANDLER_FINISHED;
			}
			
			//- JerryLin modify if offset is zero, we create a new file.
			if(offset==0){
				/* if the file doesn't exist, create it */
				fd = smbc_wrapper_open(con, con->url.path->ptr, O_WRONLY|O_TRUNC, 0);
				if (fd<0 || (void*)fd==NULL ) {					
					if (errno == ENOENT){						
						fd = smbc_wrapper_create(con, con->url.path->ptr,  FILE_READ_DATA | FILE_WRITE_DATA,0);
						if(fd <0 || (void*)fd==NULL ){														
							/* we can't open the file */
							con->http_status = 403;
							return HANDLER_FINISHED;
					   }
					}
					else if (errno == EPERM||errno == 13){
						con->http_status = 403;
						return HANDLER_FINISHED;
					}
					else {
						con->http_status = 201; /* created */
					}
				} 
				else {
					con->http_status = 201;
				}
			}
			else{				
				if (0> (fd = smbc_wrapper_open(con,con->url.path->ptr, O_WRONLY| O_APPEND, 0) )) {
					
					switch (errno) {
					case ENOENT:
						con->http_status = 404; /* not found */
						break;
					default:
						con->http_status = 403; /* not found */
						break;
					}
					
					if (errno == ENOENT){
						fd = smbc_wrapper_create(con, con->url.path->ptr,  FILE_READ_DATA | FILE_WRITE_DATA,0);
						if(fd <0 || (void*)fd==NULL ){
															
							/* we can't open the file */
							con->http_status = 403;
							Cdbg(DBE,"we can't open the file 403");
							return HANDLER_FINISHED;
					   }
					}
					else if (errno == EPERM||errno == 13){
						con->http_status = 403;
						return HANDLER_FINISHED;
					}
					else {
						con->http_status = 201; /* created */
					}
				}
			}
			
			if (-1 == smbc_wrapper_lseek(con, fd, offset, SEEK_SET)) {
				con->http_status = 501; /* not implemented */
				smbc_wrapper_close(con,fd);
				return HANDLER_FINISHED;
			}
			
			con->http_status = 200; /* modified */
		} 
		else {		
		
		   	Cdbg(DBE,"ds range= NUL, url =%s",con->url.path->ptr);
			/* take what we have in the request-body and write it to a file */

			/* if the file doesn't exist, create it */
		   	Cdbg(DBE,"ready to smbc open, %s", con->url.path->ptr);
			fd = smbc_wrapper_open(con, con->url.path->ptr, O_WRONLY|O_TRUNC, /*WEBDAV_FILE_MODE*/0);
			if (fd<0 || (void*)fd==NULL ) {
				Cdbg(DBE,"open failed errno=%d",errno);
				if (errno == ENOENT){
					fd = smbc_wrapper_create(con, con->url.path->ptr,  FILE_READ_DATA | FILE_WRITE_DATA,0);
					if(fd <0 || (void*)fd==NULL ){
														
						/* we can't open the file */
						con->http_status = 403;
						Cdbg(DBE,"we can't open the file 403");
						return HANDLER_FINISHED;
				   }
				}
				else if (errno == EPERM||errno == 13){
					con->http_status = 403;
					return HANDLER_FINISHED;
				}
				else {
					con->http_status = 201; /* created */
				}
			} 
			else {
				con->http_status = 201;
			}
		}

		con->file_finished = 1;
		Cdbg(DBE,"file finished=%d cq=%p",con->file_finished,cq);
		
		for (c = cq->first; c; c = cq->first) {
			int r = 0;
			
			/* copy all chunks */
			switch(c->type) {
			case SMB_CHUNK:
				Cdbg(DBE,"SMB_CHUNK: ============================================>");
				r=0;
				c->offset = con->request.content_length;
				cq->bytes_out = con->request.content_length;
				Cdbg(1,"offset = %lli, bytes_out=%lli", c->offset, cq->bytes_out);
				break;

			case FILE_CHUNK:
				Cdbg(DBE,"FILE_CHUNK %s", c->file.name->ptr);

				if (c->file.mmap.start == MAP_FAILED) {
					Cdbg(DBE, "MAP_FAILED");
					if (-1 == c->file.fd &&  /* open the file if not already open */
					    -1 == (c->file.fd = open(c->file.name->ptr, O_RDONLY))) {
						log_error_write(srv, __FILE__, __LINE__, "ss", "open failed: ", strerror(errno));

						return HANDLER_ERROR;
					}

					if (MAP_FAILED == (c->file.mmap.start = mmap(0, c->file.length, PROT_READ, MAP_SHARED, c->file.fd, 0))) {
						log_error_write(srv, __FILE__, __LINE__, "ssbd", "mmap failed: ",
								strerror(errno), c->file.name,  c->file.fd);
						close(c->file.fd);
						c->file.fd = -1;

						return HANDLER_ERROR;
					}

					c->file.mmap.length = c->file.length;

					close(c->file.fd);
					c->file.fd = -1;

					/* chunk_reset() or chunk_free() will cleanup for us */
				}

				Cdbg(DBE, "abcde");
				Cdbg(DBE, "c->file.mmap.start = %d, c->offset = %d, c->file.length = %d", c->file.mmap.start, c->offset, c->file.length );
				Cdbg(DBE, "smbc_wrapper_write %d, %d", c->file.mmap.start + c->offset, c->file.length - c->offset - 1 );
				size_t wrlen2 = c->file.length - (size_t)c->offset;
//				if((r=  smbc_wrapper_write(con, fd, c->mem->ptr + c->offset,c->mem->used - c->offset - 1, 0))<0 ){
				if((r=  smbc_wrapper_write(con, fd, c->file.mmap.start + c->offset, wrlen2, 0))<0 ){
				//if((r= smbc_wrapper_write(con, fd, c->file.mmap.start + c->offset, c->file.length - c->offset, 0))<0 ){
				//if ((r = write(fd, c->file.mmap.start + c->offset, c->file.length - c->offset)) < 0) {
					switch(errno) {
					case ENOSPC:
						con->http_status = 507;
						break;
					default:
						
						con->http_status = 403;
						break;
					}
				}
				
				break;
			case MEM_CHUNK:
				Cdbg(DBE,"MEM_CHUNK:");
				//Cdbg(DBE,"fd =%p, mem=%p , content=%s",fd, c->mem->ptr + c->offset, c->mem->ptr + c->offset);
				
				size_t wrlen = c->mem->used - (size_t)c->offset - 1;
//				if((r=  smbc_wrapper_write(con, fd, c->mem->ptr + c->offset,c->mem->used - c->offset - 1, 0))<0 ){
				if((r=  smbc_wrapper_write(con, fd, c->mem->ptr + c->offset, wrlen, 0))<0 ){
			//	if ((r = write(fd, c->mem->ptr + c->offset, c->mem->used - c->offset - 1)) < 0) {
					switch(errno) {
					case ENOSPC:
						con->http_status = 507;

						break;
					default:
						con->http_status = 403;
						break;
					}
				}
				break;
			case UNUSED_CHUNK:
				Cdbg(DBE,"UNUSED CHUNK");
				break;
			}

			Cdbg(DBE,"r =%d", r);
			Cdbg(DBE," c->offset= %lli", c->offset);
			Cdbg(DBE," c->bytes_out = %lli", cq->bytes_out);
			if (r > 0) {
				c->offset += r;
				cq->bytes_out += r;
			} else {
			   Cdbg(DBE,"r <0");
				break;
			}
			   Cdbg(DBE," c->offset= %lli", c->offset);
			   Cdbg(DBE," c->bytes_out = %lli", cq->bytes_out);
			chunkqueue_remove_finished_chunks(cq);
		}
//		close(fd);
		Cdbg(DBE,"close smb file https =%d",con->http_status);
		if(fd>0) smbc_wrapper_close(con, fd);
		Cdbg(DBE,"METHOD_PUT ends");

		//log_sys_write(srv, "sbss", "Upload", con->url.rel_path, "from ip", con->dst_addr_buf->ptr);
		
		return HANDLER_FINISHED;
	}
	case HTTP_METHOD_MOVE:
	case HTTP_METHOD_COPY: {
		buffer *destination = NULL;
		char *sep, *sep2, *start;
		int overwrite = 1;
		
		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		/* is a exclusive lock set on the source */
		if (con->request.http_method == HTTP_METHOD_MOVE) {
			if (!smbdav_has_lock(srv, con, p, con->uri.path)) {
				con->http_status = 423;
				return HANDLER_FINISHED;
			}
		}
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "Destination"))) {
			destination = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "Overwrite"))) {
			if (ds->value->used != 2 ||
			    (ds->value->ptr[0] != 'F' &&
			     ds->value->ptr[0] != 'T') )  {
				con->http_status = 400;
				return HANDLER_FINISHED;
			}
			
			overwrite = (ds->value->ptr[0] == 'F' ? 0 : 1);
		}
		/* let's parse the Destination
		 *
		 * http://127.0.0.1:1025/dav/litmus/copydest
		 *
		 * - host has to be the same as the Host: header we got
		 * - we have to stay inside the document root
		 * - the query string is thrown away
		 *  */

		buffer_reset(p->uri.scheme);
		buffer_reset(p->uri.path_raw);
		buffer_reset(p->uri.authority);

		start = destination->ptr;
		
		if (NULL == (sep = strstr(start, "://"))) {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		buffer_copy_string_len(p->uri.scheme, start, sep - start);

		start = sep + 3;

		if (NULL == (sep = strchr(start, '/'))) {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		if (NULL != (sep2 = memchr(start, '@', sep - start))) {
			/* skip login information */
			start = sep2 + 1;
		}
		buffer_copy_string_len(p->uri.authority, start, sep - start);

		start = sep + 1;

		if (NULL == (sep = strchr(start, '?'))) {
			/* no query string, good */
			buffer_copy_string(p->uri.path_raw, start);
		} else {
			buffer_copy_string_len(p->uri.path_raw, start, sep - start);
		}

		if (!buffer_is_equal(p->uri.authority, con->uri.authority)) {
			/* not the same host */
			con->http_status = 502;
			return HANDLER_FINISHED;
		}
		
		buffer_copy_string_buffer(p->tmp_buf, p->uri.path_raw);
		buffer_urldecode_path(p->tmp_buf);
		buffer_path_simplify(p->uri.path, p->tmp_buf);

		//- 20121115 JerryLin add for router sync.
		if(con->share_link_shortpath->used){	
			char* a;
			if (NULL != ( a = strstr(p->uri.path->ptr, con->share_link_shortpath->ptr))){
				char buff[4096];
				replace_str( a,
					         con->share_link_shortpath->ptr, 
					         con->share_link_realpath->ptr, 
					         buff );
				buffer_copy_string( p->uri.path, buff );
			}
			else{
				con->http_status = 502;
				return HANDLER_FINISHED;
			}
		}
		
		/* we now have a URI which is clean. transform it into a physical path */
		buffer_copy_string_buffer(p->physical.doc_root, con->physical.doc_root);
		buffer_copy_string_buffer(p->physical.rel_path, p->uri.path);

		if (con->conf.force_lowercase_filenames) {
			buffer_to_lower(p->physical.rel_path);
		}
		
		buffer_copy_string_buffer(p->physical.path, p->physical.doc_root);
		BUFFER_APPEND_SLASH(p->physical.path);
		buffer_copy_string_buffer(p->physical.basedir, p->physical.path);

		/* don't add a second / */
		if (p->physical.rel_path->ptr[0] == '/') {
			//- Jerry add
			buffer_append_string(p->physical.path, con->smb_info->username->ptr);
			buffer_append_string(p->physical.path,":");
			buffer_append_string(p->physical.path, con->smb_info->password->ptr);
			buffer_append_string(p->physical.path,"@");

			char* tmp = p->physical.rel_path->ptr + 1;
			int len = p->physical.rel_path->used - 2;
			//buffer_append_string_encoded(p->physical.path, tmp, len, ENCODING_REL_URI);
			buffer_append_string(p->physical.path, tmp);
		} else {
			buffer_append_string_buffer(p->physical.path, p->physical.rel_path);			
		}

		//- con->url.path is url-decode string.
		//- p->uri.path is url-decode string.
		//- p->physical.rel_path is url-decode string.
		//- p->physical.path is url-decode string.
		
		/* let's see if the source is a directory
		 * if yes, we fail with 501 */		
		if (-1 == smbc_wrapper_stat(con, con->url.path->ptr, &st)) {
			/* don't about it yet, unlink will fail too */
			switch(errno) {
			case ENOENT:
				 con->http_status = 404;
				 break;
			default:{
				 con->http_status = 403;
				 break;
				}
			}
		}
		else if (S_ISDIR(st.st_mode)) {			
			int r;			
			/* src is a directory */
			r = smbc_wrapper_stat(con, p->physical.path->ptr, &st);
			if (r == -1) {
				if(in_the_same_folder(con->url.path, p->physical.path)) {					
					if( smbc_rename(con->url.path->ptr, p->physical.path->ptr) ) {
						con->http_status = 403;
					} else {
						con->http_status = 201; //Created
						log_sys_write(srv, "sbsbss", "Move", con->url.rel_path, "to", p->physical.rel_path ,"from ip", con->dst_addr_buf->ptr);						
					}
					con->file_finished = 1;
					return HANDLER_FINISHED;
				} else if (!smbc_wrapper_mkdir(con, p->physical.path->ptr, WEBDAV_DIR_MODE)) {				
					con->http_status = 403;
					return HANDLER_FINISHED;
				}
			}
			else if (!S_ISDIR(st.st_mode)) {				
				if (overwrite == 0) {
					/* copying into a non-dir ? */
					con->http_status = 409;
					return HANDLER_FINISHED;
				} else {					
					smbc_wrapper_unlink(con, con->url.path->ptr);
					if (-1 == smbc_wrapper_mkdir(con, con->url.path->ptr, WEBDAV_DIR_MODE)) {
						con->http_status = 403;
						return HANDLER_FINISHED;
					}
				}
			}
			else if (S_ISDIR(st.st_mode)) {
				//- dst folder is exist.
				if (overwrite == 0) {
					/* copying into a non-dir ? */
					con->http_status = 412;
					return HANDLER_FINISHED;
				}
			}
			
			/* copy the content of src to dest */
			if (0 != (r = smbdav_copy_dir(srv, con, p, &(con->url), &(p->physical), overwrite))) {
				con->http_status = r;
				return HANDLER_FINISHED;
			}
			
			if (con->request.http_method == HTTP_METHOD_MOVE) {
				b = buffer_init();
				smbdav_delete_dir(srv, con, p, &(con->url), b); /* content */
				buffer_free(b);

				smbc_wrapper_rmdir(con, con->url.path->ptr);

				log_sys_write(srv, "sbsbss", "Move", con->url.rel_path, "to", p->physical.rel_path ,"from ip", con->dst_addr_buf->ptr);
			}
			con->http_status = 201;
			con->file_finished = 1;
		} 
		else {
			/* it is just a file, good */
			int r;
			
			/* does the client have a lock for this connection ? */
			if (!smbdav_has_lock(srv, con, p, p->uri.path)) {
				con->http_status = 423;
				return HANDLER_FINISHED;
			}
			
			/* destination exists */
			if (0 == (r = smbc_wrapper_stat(con, con->url.path->ptr, &st))) {
				if (S_ISDIR(st.st_mode)) {
					if (overwrite == 0) {
						/* copying into a non-dir ? */
						con->http_status = 409;
						return HANDLER_FINISHED;
					}
					
					/* file to dir/
					 * append basename to physical path */
					if (NULL != (sep = strrchr(con->physical.path->ptr, '/'))) {
						buffer_append_string(p->physical.path, sep);
						r = smbc_wrapper_stat(con, con->url.path->ptr, &st);
					}
				}
			}
			
			if (-1 == r) {
				con->http_status = 201; /* we will create a new one */
				con->file_finished = 1;

				switch(errno) {
				case ENOTDIR:
					con->http_status = 409;
					return HANDLER_FINISHED;
				}
			}
			else {
				con->http_status = 204; /* resource already existed */
			}
			
			if (con->request.http_method == HTTP_METHOD_MOVE) {				
				/* try a rename */		
				Cdbg(DBE,"smbc_rename it is just a file, good %s, %s", 
					con->url.path->ptr, p->physical.path->ptr);
				if (0 == smbc_rename(con->url.path->ptr, p->physical.path->ptr)) {
#ifdef USE_PROPPATCH
					sqlite3_stmt *stmt;

					stmt = p->conf.stmt_delete_uri;
					if (stmt) {

						sqlite3_reset(stmt);

						/* bind the values to the insert */
						sqlite3_bind_text(stmt, 1,
								  con->uri.path->ptr,
								  con->uri.path->used - 1,
								  SQLITE_TRANSIENT);

						if (SQLITE_DONE != sqlite3_step(stmt)) {
							log_error_write(srv, __FILE__, __LINE__, "ss", "sql-move(delete old) failed:", sqlite3_errmsg(p->conf.sql));
						}
					}
					
					stmt = p->conf.stmt_move_uri;
					if (stmt) {

						sqlite3_reset(stmt);

						/* bind the values to the insert */
						sqlite3_bind_text(stmt, 1,
								  p->uri.path->ptr,
								  p->uri.path->used - 1,
								  SQLITE_TRANSIENT);

						sqlite3_bind_text(stmt, 2,
								  con->uri.path->ptr,
								  con->uri.path->used - 1,
								  SQLITE_TRANSIENT);

						if (SQLITE_DONE != sqlite3_step(stmt)) {
							log_error_write(srv, __FILE__, __LINE__, "ss", "sql-move failed:", sqlite3_errmsg(p->conf.sql));
						}
					}
#endif	
					log_sys_write(srv, "sbsbss", "Move", con->url.rel_path, "to", p->physical.rel_path ,"from ip", con->dst_addr_buf->ptr);
					
					return HANDLER_FINISHED;
				}

				/* rename failed, fall back to COPY + DELETE */
			}
			
			if (0 != (r = smbdav_copy_file(srv, con, p, &(con->url), &(p->physical), overwrite))) {
				con->http_status = r;
				return HANDLER_FINISHED;
			}
			
			if (con->request.http_method == HTTP_METHOD_MOVE) {
				b = buffer_init();
				smbdav_delete_file(srv, con, p, &(con->physical), b);
				buffer_free(b);
				log_sys_write(srv, "sbsbss", "Move", con->url.rel_path, "to", p->physical.rel_path ,"from ip", con->dst_addr_buf->ptr);
			}
		}

		return HANDLER_FINISHED;
	}
	case HTTP_METHOD_PROPPATCH:
		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		if (!smbdav_has_lock(srv, con, p, con->uri.path)) {
			con->http_status = 423;
			return HANDLER_FINISHED;
		}

		/* check if destination exists */
		if (-1 == smbc_wrapper_stat(con, con->physical.path->ptr, &st)) {
			switch(errno) {
			case ENOENT:
				con->http_status = 404;
				break;
			}
		}

#ifdef USE_PROPPATCH
		if (con->request.content_length) {
			xmlDocPtr xml;

			if (1 == smbdav_parse_chunkqueue(srv, con, p, con->request_content_queue, &xml)) {
				xmlNode *rootnode = xmlDocGetRootElement(xml);

				if (0 == xmlStrcmp(rootnode->name, BAD_CAST "propertyupdate")) {
					xmlNode *cmd;
					char *err = NULL;
					int empty_ns = 0; /* send 400 on a empty namespace attribute */

					/* start response */

					if (SQLITE_OK != sqlite3_exec(p->conf.sql, "BEGIN TRANSACTION", NULL, NULL, &err)) {
						log_error_write(srv, __FILE__, __LINE__, "ss", "can't open transaction:", err);
						sqlite3_free(err);

						goto propmatch_cleanup;
					}

					/* a UPDATE request, we know 'set' and 'remove' */
					for (cmd = rootnode->children; cmd; cmd = cmd->next) {
						xmlNode *props;
						/* either set or remove */

						if ((0 == xmlStrcmp(cmd->name, BAD_CAST "set")) ||
						    (0 == xmlStrcmp(cmd->name, BAD_CAST "remove"))) {

							sqlite3_stmt *stmt;

							stmt = (0 == xmlStrcmp(cmd->name, BAD_CAST "remove")) ?
								p->conf.stmt_delete_prop : p->conf.stmt_update_prop;

							for (props = cmd->children; props; props = props->next) {
								if (0 == xmlStrcmp(props->name, BAD_CAST "prop")) {
									xmlNode *prop;
									int r;

									prop = props->children;

									if (prop->ns &&
									    (0 == xmlStrcmp(prop->ns->href, BAD_CAST "")) &&
									    (0 != xmlStrcmp(prop->ns->prefix, BAD_CAST ""))) {
										log_error_write(srv, __FILE__, __LINE__, "ss",
												"no name space for:",
												prop->name);

										empty_ns = 1;
										break;
									}

									sqlite3_reset(stmt);

									/* bind the values to the insert */

									sqlite3_bind_text(stmt, 1,
											  con->uri.path->ptr,
											  con->uri.path->used - 1,
											  SQLITE_TRANSIENT);
									sqlite3_bind_text(stmt, 2,
											  (char *)prop->name,
											  strlen((char *)prop->name),
											  SQLITE_TRANSIENT);
									if (prop->ns) {
										sqlite3_bind_text(stmt, 3,
												  (char *)prop->ns->href,
												  strlen((char *)prop->ns->href),
												  SQLITE_TRANSIENT);
									} else {
										sqlite3_bind_text(stmt, 3,
												  "",
												  0,
												  SQLITE_TRANSIENT);
									}
									if (stmt == p->conf.stmt_update_prop) {
										sqlite3_bind_text(stmt, 4,
											  (char *)xmlNodeGetContent(prop),
											  strlen((char *)xmlNodeGetContent(prop)),
											  SQLITE_TRANSIENT);
									}

									if (SQLITE_DONE != (r = sqlite3_step(stmt))) {
										log_error_write(srv, __FILE__, __LINE__, "ss",
												"sql-set failed:", sqlite3_errmsg(p->conf.sql));
									}
								}
							}
							if (empty_ns) break;
						}
					}

					if (empty_ns) {
						if (SQLITE_OK != sqlite3_exec(p->conf.sql, "ROLLBACK", NULL, NULL, &err)) {
							log_error_write(srv, __FILE__, __LINE__, "ss", "can't rollback transaction:", err);
							sqlite3_free(err);

							goto propmatch_cleanup;
						}

						con->http_status = 400;
					} else {
						if (SQLITE_OK != sqlite3_exec(p->conf.sql, "COMMIT", NULL, NULL, &err)) {
							log_error_write(srv, __FILE__, __LINE__, "ss", "can't commit transaction:", err);
							sqlite3_free(err);

							goto propmatch_cleanup;
						}
						con->http_status = 200;
					}
					con->file_finished = 1;

					return HANDLER_FINISHED;
				}

propmatch_cleanup:

				xmlFreeDoc(xml);
			} else {
				con->http_status = 400;
				return HANDLER_FINISHED;
			}
		}
#endif
		con->http_status = 501;
		return HANDLER_FINISHED;
	case HTTP_METHOD_LOCK:		
		/**
		 * a mac wants to write
		 *
		 * LOCK /dav/expire.txt HTTP/1.1\r\n
		 * User-Agent: WebDAVFS/1.3 (01308000) Darwin/8.1.0 (Power Macintosh)\r\n
		 * Accept: * / *\r\n
		 * Depth: 0\r\n
		 * Timeout: Second-600\r\n
		 * Content-Type: text/xml; charset=\"utf-8\"\r\n
		 * Content-Length: 229\r\n
		 * Connection: keep-alive\r\n
		 * Host: 192.168.178.23:1025\r\n
		 * \r\n
		 * <?xml version=\"1.0\" encoding=\"utf-8\"?>\n
		 * <D:lockinfo xmlns:D=\"DAV:\">\n
		 *  <D:lockscope><D:exclusive/></D:lockscope>\n
		 *  <D:locktype><D:write/></D:locktype>\n
		 *  <D:owner>\n
		 *   <D:href>http://www.apple.com/smbdav_fs/</D:href>\n
		 *  </D:owner>\n
		 * </D:lockinfo>\n
		 */
		depth = 0;
		if (depth != 0 && depth != -1) {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

#ifdef USE_LOCKS
		if (con->request.content_length) {
			xmlDocPtr xml;
			buffer *hdr_if = NULL;

			if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "If"))) {
				hdr_if = ds->value;
			}

			/* we don't support Depth: Infinity on locks */
			if (hdr_if == NULL && depth == -1) {
				con->http_status = 409; /* Conflict */
				return HANDLER_FINISHED;
			}
			
			if (1 == smbdav_parse_chunkqueue(srv, con, p, con->request_content_queue, &xml)) {
				xmlNode *rootnode = xmlDocGetRootElement(xml);

				assert(rootnode);
				
				if (0 == xmlStrcmp(rootnode->name, BAD_CAST "lockinfo")) {
					xmlNode *lockinfo;
					const xmlChar *lockscope = NULL, *locktype = NULL; /* TODO: compiler says unused: *owner = NULL; */

					for (lockinfo = rootnode->children; lockinfo; lockinfo = lockinfo->next) {
						if (0 == xmlStrcmp(lockinfo->name, BAD_CAST "lockscope")) {
							xmlNode *value;
							for (value = lockinfo->children; value; value = value->next) {
								if ((0 == xmlStrcmp(value->name, BAD_CAST "exclusive")) ||
								    (0 == xmlStrcmp(value->name, BAD_CAST "shared"))) {
									lockscope = value->name;
								} else {
									con->http_status = 400;

									xmlFreeDoc(xml);
									return HANDLER_FINISHED;
								}
							}
						} else if (0 == xmlStrcmp(lockinfo->name, BAD_CAST "locktype")) {
							xmlNode *value;
							for (value = lockinfo->children; value; value = value->next) {
								if ((0 == xmlStrcmp(value->name, BAD_CAST "write"))) {
									locktype = value->name;
								} else {
									con->http_status = 400;

									xmlFreeDoc(xml);
									return HANDLER_FINISHED;
								}
							}

						} else if (0 == xmlStrcmp(lockinfo->name, BAD_CAST "owner")) {
						}
					}

					if (lockscope && locktype) {
						sqlite3_stmt *stmt = p->conf.stmt_read_lock_by_uri;

						/* is this resourse already locked ? */

						/* SELECT locktoken, resource, lockscope, locktype, owner, depth, timeout
						 *   FROM locks
						 *  WHERE resource = ? */

						if (stmt) {

							sqlite3_reset(stmt);
							
							sqlite3_bind_text(stmt, 1,
									  con->uri.path->ptr,
									  con->uri.path->used - 1,
									  SQLITE_TRANSIENT);
							/*
							sqlite3_bind_text(stmt, 1,
									  p->uri.path->ptr,
									  p->uri.path->used - 1,
									  SQLITE_TRANSIENT);
							*/
							/* it is the PK */
							while (SQLITE_ROW == sqlite3_step(stmt)) {
								/* we found a lock
								 * 1. is it compatible ?
								 * 2. is it ours */
								char *sql_lockscope = (char *)sqlite3_column_text(stmt, 2);
								
								//exclusive
								if (strcmp(sql_lockscope, "exclusive")) {
									con->http_status = 423;
								} else if (0 == xmlStrcmp(lockscope, BAD_CAST "exclusive")) {
									/* resourse is locked with a shared lock
									 * client wants exclusive */
									con->http_status = 423;
								}
							}
							if (con->http_status == 423) {
								xmlFreeDoc(xml);
								return HANDLER_FINISHED;
							}
						}

						stmt = p->conf.stmt_create_lock;
						if (stmt) {
							/* create a lock-token */							
							char uuid[37] /* 36 + \0 */;

							sprintf( uuid, "%d", rand() );
							/*
							#if EMBEDDED_EANBLE
							sprintf( uuid, "%d", rand() );
							#else
							uuid_t id;
							uuid_generate(id);
							uuid_unparse(id, uuid);
							#endif
							*/
							
							buffer_copy_string_len(p->tmp_buf, CONST_STR_LEN("opaquelocktoken:"));
							buffer_append_string(p->tmp_buf, uuid);

							/* "CREATE TABLE locks ("
							 * "  locktoken TEXT NOT NULL,"
							 * "  resource TEXT NOT NULL,"
							 * "  lockscope TEXT NOT NULL,"
							 * "  locktype TEXT NOT NULL,"
							 * "  owner TEXT NOT NULL,"
							 * "  depth INT NOT NULL,"
							 */

							sqlite3_reset(stmt);

							sqlite3_bind_text(stmt, 1,
									  CONST_BUF_LEN(p->tmp_buf),
									  SQLITE_TRANSIENT);
							Cdbg(DBE, "5 p->tmp_buf=%s", p->tmp_buf->ptr);
							sqlite3_bind_text(stmt, 2,
									  CONST_BUF_LEN(con->uri.path),
									  SQLITE_TRANSIENT);
							Cdbg(DBE, "6 con->uri.path=%s", con->uri.path->ptr);
							sqlite3_bind_text(stmt, 3,
									  (const char *)lockscope,
									  xmlStrlen(lockscope),
									  SQLITE_TRANSIENT);

							sqlite3_bind_text(stmt, 4,
									  (const char *)locktype,
									  xmlStrlen(locktype),
									  SQLITE_TRANSIENT);
					
							/* owner */
							sqlite3_bind_text(stmt, 5,
									  "",
									  0,
									  SQLITE_TRANSIENT);

							/* depth */
							sqlite3_bind_int(stmt, 6,
									 depth);


							if (SQLITE_DONE != sqlite3_step(stmt)) {
								log_error_write(srv, __FILE__, __LINE__, "ss",
										"create lock:", sqlite3_errmsg(p->conf.sql));
							}

							/* looks like we survived */
							smbdav_lockdiscovery(srv, con, p->tmp_buf, (const char *)lockscope, (const char *)locktype, depth);

							con->http_status = 201;
							con->file_finished = 1;
						}
					}
				}

				xmlFreeDoc(xml);
				return HANDLER_FINISHED;
			} else {
				con->http_status = 400;
				return HANDLER_FINISHED;
			}
		} else {

			if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "If"))) {
				buffer *locktoken = ds->value;
				sqlite3_stmt *stmt = p->conf.stmt_refresh_lock;

				/* remove the < > around the token */
				if (locktoken->used < 6) {
					con->http_status = 400;

					return HANDLER_FINISHED;
				}

				buffer_copy_string_len(p->tmp_buf, locktoken->ptr + 2, locktoken->used - 5);

				sqlite3_reset(stmt);

				sqlite3_bind_text(stmt, 1,
					  CONST_BUF_LEN(p->tmp_buf),
					  SQLITE_TRANSIENT);

				if (SQLITE_DONE != sqlite3_step(stmt)) {
					log_error_write(srv, __FILE__, __LINE__, "ss",
						"refresh lock:", sqlite3_errmsg(p->conf.sql));
				}

				smbdav_lockdiscovery(srv, con, p->tmp_buf, "exclusive", "write", 0);

				con->http_status = 200;
				con->file_finished = 1;
				return HANDLER_FINISHED;
			} else {
				/* we need a lock-token to refresh */
				con->http_status = 400;

				return HANDLER_FINISHED;
			}
		}
		break;
#else
		con->http_status = 501;
		return HANDLER_FINISHED;
#endif
	case HTTP_METHOD_UNLOCK:		
#ifdef USE_LOCKS
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "Lock-Token"))) {
			buffer *locktoken = ds->value;
			sqlite3_stmt *stmt = p->conf.stmt_remove_lock;

			/* remove the < > around the token */
			if (locktoken->used < 4) {
				con->http_status = 400;

				return HANDLER_FINISHED;
			}

			/**
			 * FIXME:
			 *
			 * if the resourse is locked:
			 * - by us: unlock
			 * - by someone else: 401
			 * if the resource is not locked:
			 * - 412
			 *  */

			buffer_copy_string_len(p->tmp_buf, locktoken->ptr + 1, locktoken->used - 3);

			sqlite3_reset(stmt);

			sqlite3_bind_text(stmt, 1,
				  CONST_BUF_LEN(p->tmp_buf),
				  SQLITE_TRANSIENT);

			sqlite3_bind_text(stmt, 2,
				  CONST_BUF_LEN(con->uri.path),
				  SQLITE_TRANSIENT);

			if (SQLITE_DONE != sqlite3_step(stmt)) {
				log_error_write(srv, __FILE__, __LINE__, "ss",
					"remove lock:", sqlite3_errmsg(p->conf.sql));
			}

			if (0 == sqlite3_changes(p->conf.sql)) {
				con->http_status = 401;
			} else {
				con->http_status = 204;
			}
			return HANDLER_FINISHED;
		} else {
			/* we need a lock-token to unlock */
			con->http_status = 400;

			return HANDLER_FINISHED;
		}
		break;
#else
		con->http_status = 501;
		return HANDLER_FINISHED;
#endif
	case HTTP_METHOD_WOL:{	

		buffer *wol_mac = NULL;

		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "WOLMAC"))) {
			wol_mac = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		char cmd[BUFSIZ]="\0";
		sprintf(cmd, "ether-wake -i %s %s", srv->srvconf.arpping_interface->ptr, wol_mac->ptr);		
		int rc = system(cmd);

		if(rc!=0){
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		Cdbg(DBE, "do HTTP_METHOD_WOL! %s rc=%d", cmd, rc);

		con->http_status = 200;
		con->file_finished = 1;
					
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GSL:{
		buffer *buffer_url = NULL;
		buffer *buffer_filename = NULL;
		buffer *buffer_result_share_link = NULL;
		int expire = 1;
		int toShare = 1;
		Cdbg(DBE, "do HTTP_METHOD_GSL....................");
		
		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "URL"))) {
			buffer_url = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "FILENAME"))) {
			buffer_filename = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "EXPIRE"))) {
			expire = atoi(ds->value->ptr);
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "TOSHARE"))) {
			toShare = atoi(ds->value->ptr);
		}

		//- Check if exceed max sharelink count.
		int sharelink_save_count = get_sharelink_save_count();
		int file_count = 0;
		char* tmp_filename = strdup(buffer_filename->ptr);
		char *pch = strtok(tmp_filename, ";");				
		while(pch!=NULL){
			file_count++;
			pch = strtok(NULL,";");
		}
		free(tmp_filename);
		
		if(toShare==1&&sharelink_save_count+file_count>srv->srvconf.max_sharelink){
			con->http_status = 405;
			return HANDLER_FINISHED;
		}
		
		char auth[100]="\0";
		if(con->smb_info)
			sprintf(auth, "%s:%s", con->smb_info->username->ptr, con->smb_info->password->ptr);
		else{
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		char* base64_auth = ldb_base64_encode(auth, strlen(auth));
		
		if( generate_sharelink(srv, 
							   con, 
							   buffer_filename->ptr, 
							   buffer_url->ptr, 
							   base64_auth, 
							   expire, 
							   toShare, 
							   &buffer_result_share_link) == 0){
			free(base64_auth);
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		con->http_status = 200;

		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));

		b = chunkqueue_get_append_buffer(con->write_queue);

		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));
		buffer_append_string_len(b,CONST_STR_LEN("<sharelink>"));
		buffer_append_string_buffer(b,buffer_result_share_link);
		buffer_append_string_len(b,CONST_STR_LEN("</sharelink>"));
		buffer_append_string_len(b,CONST_STR_LEN("</result>"));

		con->file_finished = 1;

		buffer_free(buffer_result_share_link);
		free(base64_auth);

		if(toShare==1)
			save_sharelink_list();

		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GSLL:{
		Cdbg(DBE, "do HTTP_METHOD_GSLL....................");
		
		buffer* encode_filename = NULL;
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "ENCODE_FILENAME"))) {
			encode_filename = ds->value;			
		}
		
		share_link_info_t* c;
		
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));

		//- encode file name flag: 0: no encode, 1: encode
		buffer_append_string_len(b,CONST_STR_LEN("<encode_filename>"));		
		if(!buffer_is_empty(encode_filename))
			buffer_append_string(b, encode_filename->ptr);
		else
			buffer_append_string(b, "0");		
		buffer_append_string_len(b,CONST_STR_LEN("</encode_filename>"));
			
		time_t cur_time = time(NULL);
		
		for (c = share_link_info_list; c; c = c->next) {
			
			if(c->toshare != 1)
				continue;

			if( cur_time < c->createtime )
				continue;
			
			double offset = difftime(c->expiretime, cur_time);					
			if( c->expiretime!=0 && offset < 0.0 )
				continue;
			
			buffer_append_string_len(b,CONST_STR_LEN("<sharelink"));
			
			//- file name	
			buffer_append_string_len(b,CONST_STR_LEN(" filename=\""));

			if(encode_filename && buffer_is_equal_string(encode_filename, "1", 1))
			buffer_append_string_encoded(b, CONST_BUF_LEN(c->filename), ENCODING_REL_URI);
			else
				buffer_append_string_len(b, CONST_BUF_LEN(c->filename));
			
			buffer_append_string_len(b,CONST_STR_LEN("\""));

			//- share link url
			buffer_append_string_len(b,CONST_STR_LEN(" url=\""));
			buffer_append_string_buffer(b,c->shortpath);
			buffer_append_string_len(b,CONST_STR_LEN("\""));

			//- expire time
			char strTime[25] = {0};
			if(c->expiretime!=0)
				strftime(strTime, sizeof(strTime), "%Y/%m/%d %H:%M:%S", localtime(&(c->expiretime)));
			else
				strcpy(strTime,"0");
			
			buffer_append_string_len(b,CONST_STR_LEN(" expiretime=\""));
			buffer_append_string(b,strTime);
			buffer_append_string_len(b,CONST_STR_LEN("\""));

			//- create time
			char strTime2[25] = {0};
			strftime(strTime2, sizeof(strTime2), "%Y/%m/%d %H:%M:%S", localtime(&(c->createtime)));
			buffer_append_string_len(b,CONST_STR_LEN(" createtime=\""));
			buffer_append_string(b,strTime2);
			buffer_append_string_len(b,CONST_STR_LEN("\""));
				
			//- left time
			char strLeftTime[25] = {0};
			sprintf(strLeftTime,"%f",offset);
			buffer_append_string_len(b,CONST_STR_LEN(" lefttime=\""));
			if(c->expiretime!=0)
				buffer_append_string(b,strLeftTime);
			else
				buffer_append_string(b,"0");
			buffer_append_string_len(b,CONST_STR_LEN("\""));
				
			buffer_append_string_len(b,CONST_STR_LEN("></sharelink>"));
		}
		
		buffer_append_string_len(b,CONST_STR_LEN("</result>\n"));
		
		con->http_status = 200;
		con->file_finished = 1;
		
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_REMOVESL:{
		Cdbg(DBE, "do HTTP_METHOD_REMOVESL....................");

		buffer *buffer_share_link = NULL;
		
		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "SHARELINK"))) {
			buffer_share_link = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		int b_save_arpping_list = 0;
		char * pch;
		pch = strtok(buffer_share_link->ptr, ";");
		
		while(pch!=NULL){
			
			share_link_info_t* c;
			for (c = share_link_info_list; c; c = c->next) {

				if(c->toshare == 0)
					continue;
				
				if(buffer_is_equal_string(c->shortpath, pch, strlen(pch))){					
					free_share_link_info(c);
					DLIST_REMOVE(share_link_info_list, c);
					free(c);
					b_save_arpping_list = 1;
					break;
				}
			}
						
			//- Next
			pch = strtok(NULL,";");
		}

		if(b_save_arpping_list)
			save_sharelink_list();
		
		con->http_status = 200;
		con->file_finished = 1;
		
		return HANDLER_FINISHED;
	}
	
	case HTTP_METHOD_GETSRVTIME:{

		char stime[1024]="\0";
		time_t server_time = time(NULL);
		sprintf(stime, "%ld", server_time);
		Cdbg(DBE, "do HTTP_METHOD_GETSRVTIME....................%s", stime);
		
		con->http_status = 200;
		
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<servertime>\n"));
		buffer_append_string(b,stime);
		buffer_append_string_len(b,CONST_STR_LEN("</servertime>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("</result>\n"));
		
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETROUTERMAC:{

#if EMBEDDED_EANBLE
		char* router_mac = nvram_get_router_mac();
#else
		char router_mac[20]="\0";
		get_mac_address("eth0", &router_mac); 
#endif

		con->http_status = 200;
		
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<mac>\n"));
		buffer_append_string(b,router_mac);
		buffer_append_string_len(b,CONST_STR_LEN("</mac>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("</result>\n"));
		
		con->file_finished = 1;
		#if EMBEDDED_EANBLE
		#ifdef APP_IPKG
        free(router_mac);
		#endif
        #endif
		return HANDLER_FINISHED;
	}
	
	case HTTP_METHOD_GETFIRMVER:{

#if EMBEDDED_EANBLE
		char* firmware_version = nvram_get_firmware_version();
		char* build_no = nvram_get_build_no();
#else
		char* firmware_version = "1.0.0";
		char* build_no = "0";
#endif

		con->http_status = 200;
		
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<version>\n"));
		buffer_append_string(b,firmware_version);
		buffer_append_string(b,".");
		buffer_append_string(b,build_no);
		buffer_append_string_len(b,CONST_STR_LEN("</version>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("</result>\n"));
		
		con->file_finished = 1;
#if EMBEDDED_EANBLE
#ifdef APP_IPKG
        free(firmware_version);
        free(build_no);
#endif
#endif
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETROUTERINFO:{
		Cdbg(DBE, "do HTTP_METHOD_GETROUTERINFO....................");
		
		char stime[1024]="\0";
		time_t server_time = time(NULL);
		sprintf(stime, "%ld", server_time);
		
#if EMBEDDED_EANBLE
		char* router_mac = nvram_get_router_mac();
		
		char* firmware_version = nvram_get_firmware_version();
		char* build_no = nvram_get_build_no();

		char* modal_name = nvram_get_productid();
		
		//- Computer Name		
		char* computer_name = nvram_get_computer_name();		
		char* st_webdav_mode = nvram_get_st_webdav_mode();	
		char* webdav_http_port = nvram_get_webdav_http_port();
		char* webdav_https_port = nvram_get_webdav_https_port();
		char* http_enable = nvram_get_http_enable();
		char* misc_http_x = nvram_get_misc_http_x();
		char* misc_http_port = nvram_get_misc_http_port();
		char* misc_https_port = nvram_get_misc_https_port();
		char* ddns_host_name = nvram_get_ddns_host_name();
		char* disk_path = "/mnt/";
		char* wan_ip = nvram_get_wan_ip();
		char *usbdiskname = nvram_get_productid();
			
		//- Get aicloud version
		#ifdef APP_IPKG
		char* aicloud_version_file = "/opt/lib/ipkg/info/aicloud.control";
		char* aicloud_app_type = "install";
		char* smartsync_version_file = "/opt/lib/ipkg/info/smartsync.control";
		#else
		char* aicloud_version_file = "/usr/lighttpd/control";		
		char* aicloud_app_type = "embed";
		char* smartsync_version_file = "/usr/lighttpd/smartsync_control";
		#endif
		char aicloud_version[20]="\0";
		char smartsync_version[20]="\0";
		char *swpjverno = nvram_get_swpjverno();
		char *extendno = nvram_get_extendno();

		char *https_crt_cn = nvram_get_https_crt_cn();
#else
		char router_mac[20]="\0";
		get_mac_address("eth0", &router_mac); 

		char* firmware_version = "1.0.0";
		char* build_no = "0";

		char* modal_name = "usbdisk";
		
		//- Computer Name		
		char* computer_name = "WebDAV";
		char* st_webdav_mode = "0";
		char* webdav_http_port = "8082";		
		char* webdav_https_port = "443";
		char* http_enable = "2";
		char* misc_http_x = "0";
		char* misc_http_port = "8080";
		char* misc_https_port = "8443";
		char* ddns_host_name = "";
		char* disk_path = "/mnt/";
		char* wan_ip = "192.168.1.10";
		char *usbdiskname = "usbdisk";
		
		//- Get aicloud version
		char* aicloud_version_file = "/usr/css/control";
		char* smartsync_version_file = "/usr/css/smartsync_control";
		char aicloud_version[20]="\0";
		char smartsync_version[20]="\0";
		char *swpjverno = "";
		char *extendno = "";
		char* aicloud_app_type = "embed";

		char *https_crt_cn = "192.168.1.1";
#endif
		int sharelink_save_count = get_sharelink_save_count();
		int dms_enable = is_dms_enabled();

		if(buffer_is_empty(srv->srvconf.aicloud_version)){
			//- Parser version file
			FILE* fp2;
			char line[128];
			if((fp2 = fopen(aicloud_version_file, "r")) != NULL){
				memset(line, 0, sizeof(line));
				while(fgets(line, 128, fp2) != NULL){
					if(strncmp(line, "Version:", 8)==0){
						strncpy(aicloud_version, line + 9, strlen(line)-8);
					}
				}
				fclose(fp2);
			}
		}
		else{
			strcpy(aicloud_version, srv->srvconf.aicloud_version->ptr);
		}

		if(buffer_is_empty(srv->srvconf.smartsync_version)){
			//- Parser version file
			FILE* fp2;
			char line[128];
			if((fp2 = fopen(smartsync_version_file, "r")) != NULL){
				memset(line, 0, sizeof(line));
				while(fgets(line, 128, fp2) != NULL){
					if(strncmp(line, "Version:", 8)==0){
						strncpy(smartsync_version, line + 9, strlen(line)-8);
					}
				}
				fclose(fp2);
			}
		}
		else{
			strcpy(smartsync_version, srv->srvconf.smartsync_version->ptr);
		}
		
#ifndef APP_IPKG
		if( swpjverno!=NULL && strncmp(swpjverno,"", 1)!=0 ){
			strcpy(aicloud_version, swpjverno);
			if(extendno!=NULL && strncmp(extendno,"", 1)!=0)
			{
			    strcat(aicloud_version, "_");
			    strcat(aicloud_version, extendno);
		    }
		}
#endif
		con->http_status = 200;
		
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));
		buffer_append_string_len(b,CONST_STR_LEN("<servertime>"));
		buffer_append_string(b,stime);
		buffer_append_string_len(b,CONST_STR_LEN("</servertime>"));
		buffer_append_string_len(b,CONST_STR_LEN("<mac>"));
		buffer_append_string(b,router_mac);
		buffer_append_string_len(b,CONST_STR_LEN("</mac>"));
		buffer_append_string_len(b,CONST_STR_LEN("<version>"));
		buffer_append_string(b,firmware_version);
		buffer_append_string(b,".");
		buffer_append_string(b,build_no);
		buffer_append_string_len(b,CONST_STR_LEN("</version>"));
		buffer_append_string_len(b,CONST_STR_LEN("<aicloud_version>"));
		buffer_append_string(b,aicloud_version);
		buffer_append_string_len(b,CONST_STR_LEN("</aicloud_version>"));
		buffer_append_string_len(b,CONST_STR_LEN("<extendno>"));
		if(extendno!=NULL && strncmp(extendno,"", 1)!=0) buffer_append_string(b,extendno);
		buffer_append_string_len(b,CONST_STR_LEN("</extendno>"));
		buffer_append_string_len(b,CONST_STR_LEN("<aicloud_app_type>"));
		buffer_append_string(b,aicloud_app_type);		
		buffer_append_string_len(b,CONST_STR_LEN("</aicloud_app_type>"));
		buffer_append_string_len(b,CONST_STR_LEN("<smartsync_version>"));
		buffer_append_string(b,smartsync_version);
		buffer_append_string_len(b,CONST_STR_LEN("</smartsync_version>"));
		buffer_append_string_len(b,CONST_STR_LEN("<modalname>"));
		buffer_append_string(b,modal_name);
		buffer_append_string_len(b,CONST_STR_LEN("</modalname>"));
		buffer_append_string_len(b,CONST_STR_LEN("<computername>"));
		buffer_append_string(b,computer_name);
		buffer_append_string_len(b,CONST_STR_LEN("</computername>"));
		buffer_append_string_len(b,CONST_STR_LEN("<usbdiskname>"));
		buffer_append_string(b,usbdiskname);
		buffer_append_string_len(b,CONST_STR_LEN("</usbdiskname>"));
		buffer_append_string_len(b,CONST_STR_LEN("<webdav_mode>"));
		buffer_append_string(b,st_webdav_mode);
		buffer_append_string_len(b,CONST_STR_LEN("</webdav_mode>"));
		buffer_append_string_len(b,CONST_STR_LEN("<http_port>"));
		buffer_append_string(b,webdav_http_port);
		buffer_append_string_len(b,CONST_STR_LEN("</http_port>"));
		buffer_append_string_len(b,CONST_STR_LEN("<https_port>"));
		buffer_append_string(b,webdav_https_port);
		buffer_append_string_len(b,CONST_STR_LEN("</https_port>"));
		buffer_append_string_len(b,CONST_STR_LEN("<http_enable>"));
		buffer_append_string(b,http_enable);
		buffer_append_string_len(b,CONST_STR_LEN("</http_enable>"));
		buffer_append_string_len(b,CONST_STR_LEN("<misc_http_enable>"));
		buffer_append_string(b,misc_http_x);
		buffer_append_string_len(b,CONST_STR_LEN("</misc_http_enable>"));
		buffer_append_string_len(b,CONST_STR_LEN("<misc_http_port>"));
		buffer_append_string(b,misc_http_port);
		buffer_append_string_len(b,CONST_STR_LEN("</misc_http_port>"));
		buffer_append_string_len(b,CONST_STR_LEN("<misc_https_port>"));
		buffer_append_string(b,misc_https_port);
		buffer_append_string_len(b,CONST_STR_LEN("</misc_https_port>"));
		buffer_append_string_len(b,CONST_STR_LEN("<last_login_info>"));
		if(buffer_is_empty(srv->last_login_info))
			buffer_append_string(b,"");
		else
			buffer_append_string(b,srv->last_login_info->ptr);
		buffer_append_string_len(b,CONST_STR_LEN("</last_login_info>"));
		buffer_append_string_len(b,CONST_STR_LEN("<ddns_host_name>"));
		buffer_append_string(b,ddns_host_name);
		buffer_append_string_len(b,CONST_STR_LEN("</ddns_host_name>"));
		buffer_append_string_len(b,CONST_STR_LEN("<wan_ip>"));
		buffer_append_string(b,wan_ip);
		buffer_append_string_len(b,CONST_STR_LEN("</wan_ip>"));
		buffer_append_string_len(b,CONST_STR_LEN("<dms_enable>"));
		buffer_append_string(b, ((dms_enable==1) ? "1" : "0"));
		buffer_append_string_len(b,CONST_STR_LEN("</dms_enable>"));
		buffer_append_string_len(b,CONST_STR_LEN("<https_crt_cn>"));
		buffer_append_string(b,https_crt_cn);
		buffer_append_string_len(b,CONST_STR_LEN("</https_crt_cn>"));
		buffer_append_string_len(b,CONST_STR_LEN("<app_installation_url>"));
		buffer_append_string(b, buffer_is_empty(srv->srvconf.app_installation_url) ? "" : srv->srvconf.app_installation_url->ptr);
		buffer_append_string_len(b,CONST_STR_LEN("</app_installation_url>"));
		buffer_append_string_len(b,CONST_STR_LEN("<used_sharelink>"));
		char used_sharelink[5] = "\0";
		sprintf(used_sharelink, "%d", sharelink_save_count);
		buffer_append_string(b, used_sharelink);
		buffer_append_string_len(b,CONST_STR_LEN("</used_sharelink>"));
		buffer_append_string_len(b,CONST_STR_LEN("<max_sharelink>"));
		char max_sharelink[5] = "\0";
		sprintf(max_sharelink, "%d", srv->srvconf.max_sharelink);
		buffer_append_string(b, max_sharelink);
		buffer_append_string_len(b,CONST_STR_LEN("</max_sharelink>"));
		
		DIR *dir;
		if (NULL != (dir = opendir(disk_path))) {

			buffer_append_string_len(b,CONST_STR_LEN("<disk_space>"));
			
			struct dirent *de;

			while(NULL != (de = readdir(dir))) {

				if ( de->d_name[0] == '.' && de->d_name[1] == '.' && de->d_name[2] == '\0' ) {
					continue;
					//- ignore the parent dir
				}

				if ( de->d_name[0] == '.' ) {
					continue;
					//- ignore the hidden file
				}

				buffer_append_string_len(b,CONST_STR_LEN("<item>"));
				
				char querycmd[100] = "\0";		

				sprintf(querycmd, "df|grep -i '%s%s'", disk_path, de->d_name);
								
				buffer_append_string_len(b,CONST_STR_LEN("<DiskName>"));
				buffer_append_string(b,de->d_name);
				buffer_append_string_len(b,CONST_STR_LEN("</DiskName>"));
						
				char mybuffer[BUFSIZ]="\0";
				FILE* fp = popen( querycmd, "r");
				if(fp){
					int len = fread(mybuffer, sizeof(char), BUFSIZ, fp);
					mybuffer[len-1]="\0";
					pclose(fp);
					
					char * pch;
					pch = strtok(mybuffer, " ");
					int count=1;
					while(pch!=NULL){				
						if(count==3){
							buffer_append_string_len(b,CONST_STR_LEN("<DiskUsed>"));
							buffer_append_string(b,pch);
							buffer_append_string_len(b,CONST_STR_LEN("</DiskUsed>"));
						}
						else if(count==4){
							buffer_append_string_len(b,CONST_STR_LEN("<DiskAvailable>"));
							buffer_append_string(b,pch);
							buffer_append_string_len(b,CONST_STR_LEN("</DiskAvailable>"));
						}
						else if(count==5){
							buffer_append_string_len(b,CONST_STR_LEN("<DiskUsedPercent>"));
							buffer_append_string(b,pch);
							buffer_append_string_len(b,CONST_STR_LEN("</DiskUsedPercent>"));
						}
						
						//- Next
						pch = strtok(NULL," ");
						count++;
					}
					
				}

				buffer_append_string_len(b,CONST_STR_LEN("</item>"));
			}

			buffer_append_string_len(b,CONST_STR_LEN("</disk_space>"));

			closedir(dir);
		}

		buffer_append_string_len(b,CONST_STR_LEN("</result>"));
		
		con->file_finished = 1;
#if EMBEDDED_EANBLE
#ifdef APP_IPKG
        free(router_mac);

        free(firmware_version);
        free(build_no);

        //- Computer Name
        free(modal_name);
        free(computer_name);
        free(st_webdav_mode);
        free(webdav_http_port);
        free(webdav_https_port);
        free(misc_http_x);
        free(misc_http_port);
        free(ddns_host_name);
		free(wan_ip);
		free(http_enable);
		free(misc_https_port);
		free(usbdiskname);                
		if( swpjverno!=NULL && strncmp(swpjverno,"", 1)!=0 )
		{
			free(swpjverno);
		}
		if( extendno != NULL)
		{
			free(extendno);
		}
		free(https_crt_cn);
#endif
#endif
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETNOTICE:{
		if(!con->conf.is_ssl){
			con->http_status = 403;
			return HANDLER_FINISHED;
		}
		
		buffer* timestamp;
		char log_file[200] = "/tmp/aicloud_notice.log";

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "TIMESTAMP"))) {
			timestamp = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		Cdbg(1, "do HTTP_METHOD_GETNOTICE....................log_file=%s", log_file);
		unlink(log_file);
		
		#if EMBEDDED_EANBLE
		#if HAVE_PUSHLOG
		getlogbytimestr(log_file, timestamp->ptr, 1);
		#endif
		#endif
		
		if( !file_exist(log_file) ||
			file_size(log_file) == 0 ){			
			con->http_status = 404;
			con->file_finished = 1;
			return HANDLER_FINISHED;
		}

		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
			
		b = chunkqueue_get_append_buffer(con->write_queue);
			
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));
		buffer_append_string_len(b,CONST_STR_LEN("<log>"));
		
		FILE* fp = fopen(log_file, "rb");
		
		if(fp!=NULL){
			 //Get file length
        	fseek(fp, 0, SEEK_END);
        	int fileLen = ftell(fp);
        	fseek(fp, 0, SEEK_SET);
			
			char* log_content = (char *)malloc(fileLen+1);
			if(!log_content){
				fclose(fp);
				con->http_status = 404;
				con->file_finished = 1;
				return HANDLER_FINISHED;
			}
			
			fread( log_content, fileLen, sizeof(unsigned char), fp );
			
			buffer_append_string_len(b, log_content, fileLen);

			Cdbg(1, "log_content=%s", log_content);
			
			free(log_content);

			fclose(fp);
		}
		
		buffer_append_string_len(b,CONST_STR_LEN("</log>"));
		buffer_append_string_len(b,CONST_STR_LEN("</result>"));
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}
				
	case HTTP_METHOD_RESCANSMBPC:{
		Cdbg(DBE, "do HTTP_METHOD_RESCANSMBPC");
		
		stop_arpping_process();

		#if EMBEDDED_EANBLE
		nvram_set_smbdav_str("");			
		#else
		unlink("/tmp/arpping_list");
		#endif
			
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_LOGOUT:{
		Cdbg(DBE, "do HTTP_METHOD_LOGOUT %s", con->smb_info->server);
		
		//- Find router user name
		//- Get user-Agent
		data_string *ds = (data_string *)array_get_element(con->request.headers, "user-Agent");
		if(ds==NULL){
			con->http_status = 400;
			con->file_finished = 1;
			return HANDLER_FINISHED;
		}
		
		smb_info_t *c;
		for (c = srv->smb_srv_info_list; c; c = c->next) {

			if( buffer_is_equal(c->user_agent, ds->value) && 
			    buffer_is_equal(c->src_ip, con->dst_addr_buf) &&
			    buffer_is_equal(c->server, con->smb_info->server) ){

				Cdbg(DBE, "RELOGIN c->server=[%s], c->user=[%s] c->pass=[%s]", c->server->ptr, c->username->ptr, c->password->ptr);

				buffer_copy_string(c->username, "RELOGIN");
				buffer_copy_string(c->password, "");
				
				break;
			}
		
		}
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETLATESTVER:{		
		
		#if EMBEDDED_EANBLE
			system("/usr/sbin/webs_update.sh");
			char* newest_version = nvram_get_latest_version();
			int webs_state_error = nvram_get_webs_state_error();
		#else
			char* newest_version = "3004_110";
			int webs_state_error = 0;
		#endif

		if(webs_state_error==1){
			con->http_status = 400;
		}
		else{
			con->http_status = 200;
			
			response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
			
			b = chunkqueue_get_append_buffer(con->write_queue);
			
			buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
			buffer_append_string_len(b,CONST_STR_LEN("<result>"));
			buffer_append_string_len(b,CONST_STR_LEN("<version>"));
			buffer_append_string(b,newest_version);
			buffer_append_string_len(b,CONST_STR_LEN("</version>"));
			buffer_append_string_len(b,CONST_STR_LEN("</result>"));
		}
		
		con->file_finished = 1;
#if EMBEDDED_EANBLE
#ifdef APP_IPKG
		free(newest_version);
#endif
#endif
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETDISKSPACE:{
		Cdbg(DBE, "do HTTP_METHOD_GETDISKSPACE");

		buffer *buffer_disk = NULL;
		
		if (p->conf.is_readonly) {
			con->http_status = 403;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "DISKNAME"))) {
			buffer_disk = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		int len = buffer_disk->size+10;
		char* disk_path = (char*)malloc(len);
		memset(disk_path, '\0', len);
#if EMBEDDED_EANBLE
		strcpy(disk_path, "/mnt/");
#else
		strcpy(disk_path, "/dev/");
#endif
		strcat(disk_path, buffer_disk->ptr);

		if (-1 == stat(disk_path, &st)) {
			free(disk_path);			
			con->http_status = 400;			
			return HANDLER_FINISHED;
		}
		else if (!S_ISDIR(st.st_mode)) {			
		   free(disk_path);			
		   con->http_status = 400;			
		   return HANDLER_FINISHED;		
		}
		
		char querycmd[100] = "\0";		
		sprintf(querycmd, "df |grep -i %s", disk_path);		
		free(disk_path);

		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));
		
		buffer_append_string_len(b,CONST_STR_LEN("<DiskName>"));
		buffer_append_string_buffer(b,buffer_disk);
		buffer_append_string_len(b,CONST_STR_LEN("</DiskName>"));
					
		char mybuffer[BUFSIZ]="\0";
		FILE* fp = popen( querycmd, "r");
		if(fp){
			int len = fread(mybuffer, sizeof(char), BUFSIZ, fp);
			mybuffer[len-1]="\0";
			pclose(fp);
			
			char * pch;
			pch = strtok(mybuffer, " ");
			int count=1;
			while(pch!=NULL){				
				if(count==3){
					buffer_append_string_len(b,CONST_STR_LEN("<DiskUsed>"));
					buffer_append_string(b,pch);
					buffer_append_string_len(b,CONST_STR_LEN("</DiskUsed>"));					
				}
				else if(count==4){
					buffer_append_string_len(b,CONST_STR_LEN("<DiskAvailable>"));
					buffer_append_string(b,pch);
					buffer_append_string_len(b,CONST_STR_LEN("</DiskAvailable>"));
				}
				else if(count==5){
					buffer_append_string_len(b,CONST_STR_LEN("<DiskUsedPercent>"));
					buffer_append_string(b,pch);
					buffer_append_string_len(b,CONST_STR_LEN("</DiskUsedPercent>"));
				}
				
				//- Next
				pch = strtok(NULL," ");
				count++;
			}
			
		}

		buffer_append_string_len(b,CONST_STR_LEN("</result>"));

		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

#if 0
	case HTTP_METHOD_PROPFINDMEDIALIST:{
	
#ifdef USE_MINIDLNA_DB
		//- 20130304 JerryLin add
		Cdbg(DBE, "do HTTP_METHOD_PROPFINDMEDIALIST");
		if (!p->conf.sql_minidlna) {

			if (!buffer_is_empty(p->conf.sqlite_minidlna_db_name)) {
				Cdbg(DBE, "p->conf.sqlite_minidlna_db_name=%s", p->conf.sqlite_minidlna_db_name->ptr);
				if (SQLITE_OK != sqlite3_open(p->conf.sqlite_minidlna_db_name->ptr, &(p->conf.sql_minidlna))) {				
					log_error_write(srv, __FILE__, __LINE__, "sbs", "sqlite3_open failed for",
							p->conf.sqlite_minidlna_db_name,
							sqlite3_errmsg(p->conf.sql_minidlna));
					Cdbg(DBE, "Fail to open minidlna db %s", p->conf.sqlite_minidlna_db_name->ptr);
				}
			}

			if (!p->conf.sql_minidlna) {
				con->http_status = 403;
				con->file_finished = 1;
				return HANDLER_FINISHED;	
			}
		}
		
		sqlite3 *sql_minidlna = p->conf.sql_minidlna;
		/*
		int a, v, p;
		a = sql_get_int_field(sql_minidlna, "SELECT count(*) from DETAILS where MIME glob 'a*'");
		v = sql_get_int_field(sql_minidlna, "SELECT count(*) from DETAILS where MIME glob 'v*'");
		p = sql_get_int_field(sql_minidlna, "SELECT count(*) from DETAILS where MIME glob 'i*'");

		Cdbg(DBE, "aaaaaaaaaaaaaaaaaaaaaaaaaaa audio=%d, video=%d, image=%d", a, v, p);
		*/
		int rows, i;
		char **result;
		sqlite_int64 plID, detailID;
		char *plpath, *plname, *fname, *last_dir;
		char sql_buf[] = "SELECT ID, PATH, TITLE from DETAILS where MIME glob 'a*'";
		if( sql_get_table(sql_minidlna, sql_buf, &result, &rows, NULL) != SQLITE_OK ) 
			return -1;
		if( !rows )
		{
			sqlite3_free_table(result);
			return 0;
		}
		rows++;
		
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));

		b = chunkqueue_get_append_buffer(con->write_queue);

		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
		buffer_append_string_len(b,CONST_STR_LEN("<D:multistatus xmlns:D=\"DAV:\" xmlns:ns0=\"urn:uuid:c2f41010-65b3-11d1-a29f-00aa00c14882/\" "));		
		buffer_append_string_len(b,CONST_STR_LEN(">\n"));

		char* usbdisk_name;
		#if EMBEDDED_EANBLE
		char *a = nvram_get_productid();
		int l = strlen(a)+1;
		usbdisk_name = (char*)malloc(l);
		memset(usbdisk_name,'\0', l);
		strcpy(usbdisk_name, a);
		#ifdef APP_IPKG
		free(a);
		#endif
		#else
		usbdisk_name = (char*)malloc(8);
		memset(usbdisk_name,'\0', 8);
		strcpy(usbdisk_name, "usbdisk");
		#endif

		for( i=3; i<rows*3; i++ )
		{
			plID = strtoll(result[i], NULL, 10);
			plpath = result[++i];
			plname = result[++i];
			
			char buff[4096];
			char* tmp = replace_str(&plpath[0], 
			                    "tmp/mnt", 
			                    usbdisk_name, 
				                (char *)&buff[0]);
			
			buffer_append_string_len(b,CONST_STR_LEN("<D:response>\n"));
			buffer_append_string_len(b,CONST_STR_LEN("<D:href>"));
			buffer_append_string_buffer(b, con->uri.scheme);
			buffer_append_string_len(b,CONST_STR_LEN("://"));
			buffer_append_string_buffer(b, con->uri.authority);			
			buffer_append_string_encoded(b, tmp, strlen(tmp), ENCODING_REL_URI);
			buffer_append_string_len(b,CONST_STR_LEN("</D:href>\n"));

			buffer_append_string_len(b,CONST_STR_LEN("<D:propstat>\n"));
			buffer_append_string_len(b,CONST_STR_LEN("<D:prop>\n"));

			//buffer_append_string_buffer(b, prop_200);

			buffer_append_string_len(b,CONST_STR_LEN("</D:prop>\n"));
			buffer_append_string_len(b,CONST_STR_LEN("<D:status>HTTP/1.1 200 OK</D:status>\n"));
			buffer_append_string_len(b,CONST_STR_LEN("</D:propstat>\n"));

			buffer_append_string_len(b,CONST_STR_LEN("</D:response>\n"));
		}
		
		buffer_append_string_len(b,CONST_STR_LEN("</D:multistatus>\n"));

		Cdbg(DBE, "rows=%d", rows);
		
		sqlite3_free_table(result);
#endif

		con->http_status = 207;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}
#endif

	case HTTP_METHOD_GETPRODUCTICON:{
		Cdbg(DBE, "do HTTP_METHOD_GETPRODUCTICON %s", srv->srvconf.product_image->ptr);
		
		char product_icon_file[1024] = "\0";
		
		char* odmpid = NULL;
		char *productid = NULL;

		#if EMBEDDED_EANBLE
		odmpid = nvram_get_odmpid();
		productid = nvram_get_value("productid");
		#endif
		
		if(!buffer_is_empty(srv->srvconf.product_image))
			strcpy(product_icon_file, srv->srvconf.product_image->ptr);
		else{
			if(odmpid!=NULL&&strcmp(odmpid, "")!=0){
				
				if(productid!=NULL&&strcmp(productid, odmpid)==0){
					strcpy(product_icon_file, "/www/images/Model_producticon.png");
				}
				else{
					strcpy(product_icon_file, "/www/images/");
					strcat(product_icon_file, odmpid);
					strcat(product_icon_file, "Model_producticon.png");
				}
				
		#if EMBEDDED_EANBLE
		#ifdef APP_IPKG
				free(odmpid);
				free(productid);
		#endif
		#endif
		
			}
			else
				strcpy(product_icon_file, "/www/images/Model_producticon.png");
		}
		
		FILE* fp = fopen(product_icon_file, "rb");
		
		if(fp!=NULL){
			 //Get file length
        	fseek(fp, 0, SEEK_END);
        	int fileLen = ftell(fp);
        	fseek(fp, 0, SEEK_SET);

			char* buffer = (char *)malloc(fileLen+1);
			if(!buffer){
				con->http_status = 404;
				con->file_finished = 1;
				return HANDLER_FINISHED;
			}

			char* aa = get_filename_ext(product_icon_file);
			int len = strlen(aa)+1; 		
			char* file_ext = (char*)malloc(len);
			memset(file_ext,'\0', len);
			strcpy(file_ext, aa);
			for (int i = 0; file_ext[i]; i++)
				file_ext[i] = tolower(file_ext[i]);
			
			fread( buffer, fileLen, sizeof(unsigned char), fp );
			
			char* base64_image = ldb_base64_encode(buffer, fileLen);
			
			response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));

			b = chunkqueue_get_append_buffer(con->write_queue);
					
			buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
			buffer_append_string_len(b,CONST_STR_LEN("<result>"));

			if( strcmp(file_ext,"jpg")==0 || strcmp(file_ext,"jpeg")==0 )
				buffer_append_string_len(b,CONST_STR_LEN("<mimetype>image/jpeg</mimetype>"));
			else if( strcmp(file_ext,"png")==0 )
				buffer_append_string_len(b,CONST_STR_LEN("<mimetype>image/png</mimetype>"));
			else if( strcmp(file_ext,"gif")==0 )
				buffer_append_string_len(b,CONST_STR_LEN("<mimetype>image/gif</mimetype>"));
			else
				buffer_append_string_len(b,CONST_STR_LEN("<mimetype>unknown</mimetype>"));
				
			buffer_append_string_len(b,CONST_STR_LEN("<product_icon>"));
			buffer_append_string(b,base64_image);
			buffer_append_string_len(b,CONST_STR_LEN("</product_icon>"));
			buffer_append_string_len(b,CONST_STR_LEN("</result>"));

			free(file_ext);
			free(buffer);
			fclose(fp);
		}
		else{
			con->http_status = 404;
			con->file_finished = 1;
			return HANDLER_FINISHED;
		}
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETVIDEOSUBTITLE:{
		buffer* filename;
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "FILENAME"))) {
			filename = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		Cdbg(DBE, "do HTTP_METHOD_GETVIDEOSUBTITLE %s, filename=%s, con->physical.path=%s", 
				con->url.path->ptr, filename->ptr, con->physical.path->ptr);

		char auth[100]="\0";
		if(con->smb_info)
			sprintf(auth, "%s:%s", con->smb_info->username->ptr, con->smb_info->password->ptr);
		else{
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
				
		char* base64_auth = ldb_base64_encode(auth, strlen(auth));
		
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));
		
		if (NULL != (dir = smbc_wrapper_opendir(con, con->url.path->ptr))) {
			struct smbc_dirent *de;

			while(NULL != (de = smbc_wrapper_readdir(con, dir))) {

				if ((de->name[0] == '.' && de->name[1] == '\0')  ||
				    (de->name[0] == '.' && de->name[1] == '.' && de->name[2] == '\0')) {
					continue;
					/* ignore the parent dir */
				}
				
				char* aa = get_filename_ext(de->name);
				int len = strlen(aa)+1; 		
				char* file_ext = (char*)malloc(len);
				memset(file_ext,'\0', len);
				strcpy(file_ext, aa);
				for (int i = 0; file_ext[i]; i++)
					file_ext[i] = tolower(file_ext[i]);

				if( strcmp(file_ext, "srt")==0 &&
					strstr(de->name, filename->ptr) ){

					buffer* buffer_filepath = buffer_init();
					buffer_copy_string(buffer_filepath, "");
					buffer_append_string_encoded(buffer_filepath, con->physical.rel_path->ptr, strlen(con->physical.rel_path->ptr), ENCODING_REL_URI);
					
					buffer* buffer_result_share_link;
					if( generate_sharelink(srv, 
						                   con, 
						                   de->name, 
						                   buffer_filepath->ptr, 
						                   base64_auth, 0, 0, 
						                   &buffer_result_share_link) == 1){
						buffer_append_string_len(b,CONST_STR_LEN("<file>"));
						buffer_append_string_len(b,CONST_STR_LEN("<name>"));
						buffer_append_string(b,de->name);
						buffer_append_string_len(b,CONST_STR_LEN("</name>"));
						buffer_append_string_len(b,CONST_STR_LEN("<sharelink>"));
						buffer_append_string_buffer(b,buffer_result_share_link);
						buffer_append_string_len(b,CONST_STR_LEN("</sharelink>"));
						buffer_append_string_len(b,CONST_STR_LEN("</file>"));
					}

					buffer_free(buffer_filepath);
					buffer_free(buffer_result_share_link);
				}

				free(file_ext);										
				
			}

			smbc_wrapper_closedir(con, dir);
		}

		buffer_append_string_len(b,CONST_STR_LEN("</result>"));
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GENROOTCERTIFICATE:{
		
		buffer* keylength; //- Private key length
		buffer* caname; //- CA name
		buffer* email; //- email address
		buffer* country; //- Country Name(2 letter code)
		buffer* state; //- State or Province Name(full name)
		buffer* ln; //- Locality Name(eg, city)
		buffer* orag; //- Organization Name(eg, company)
		buffer* ounit; //- Organizational Unit Name(eg, section)
		buffer* cn; //- Common Name(eg. your name or your server's hostname)
#if 0		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "KEYLEN"))) {
			keylength = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "CANAME"))) {
			caname = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "EMAIL"))) {
			email = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "COUNTRY"))) {
			country = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "STATE"))) {
			state = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "LN"))) {
			ln = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "ORAG"))) {
			orag = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "OUNIT"))) {
			ounit = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
#endif
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "CN"))) {
			cn = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		Cdbg(DBE, "nvram_set_https_crt_cn %s", cn->ptr);

		#if EMBEDDED_EANBLE	
		
		nvram_set_https_crt_cn(cn->ptr);
		
		unlink("/etc/cert.pem");
        unlink("/etc/key.pem");

		Cdbg(DBE, "run gencert.sh");
		unsigned long long sn;
		char t[32];
		f_read("/dev/urandom", &sn, sizeof(sn));
		sprintf(t, "%llu", sn & 0x7FFFFFFFFFFFFFFFULL);
                int flag_cert=0;
#ifdef APP_IPKG
                char *cmd_app=NULL;
                cmd_app=(char *)malloc(sizeof(char)*(strlen(t)+64));
                memset(cmd_app,'\0',sizeof(cmd_app));
                sprintf(cmd_app,"%s %s","/opt/etc/apps_asus_script/gencert.sh",t);
                system(cmd_app);
                while(-1==access("/etc/cert.pem",F_OK)||-1==access("/etc/key.pem",F_OK))
                {
                    usleep(1000*100);
                }
                free(cmd_app);

                Cdbg(DBE, "tar -zcvf certification");
                system("tar -C / -czf /tmp/cert.tgz etc/cert.pem etc/key.pem");
                while(-1==access("/tmp/cert.tgz",F_OK))
                {
                    usleep(1000*100);
                }
                flag_cert=1;
#else
        eval("gencert.sh", t);

		Cdbg(DBE, "tar -zcvf certification");
                if (eval("tar", "-C", "/", "-czf", "/tmp/cert.tgz", "etc/cert.pem", "etc/key.pem") == 0)
                {
                        flag_cert=1;
                }
#endif
                if(flag_cert==1)
                {
                    Cdbg(DBE, "nvram_setfile_https_crt_file");

                    if (nvram_setfile_https_crt_file("/tmp/cert.tgz", 8192)) {
                        Cdbg(DBE, "complete nvram_setfile_https_crt_file");
                        nvram_set_https_crt_save("1");
                        nvram_do_commit();
                        Cdbg(DBE, "end nvram_setfile_https_crt_file");
                    }

                    unlink("/tmp/cert.tgz");
                }
				
		//Cdbg(DBE, "restart webdav");
		//system("rc rc_service restart_webdav");
		Cdbg(DBE, "complete");
		
		#endif
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_APPLYAPP:{

		if(!con->conf.is_ssl){
			con->http_status = 403;
			return HANDLER_FINISHED;
		}
		
		buffer* action_mode;
		buffer* rc_service;
		buffer* set_nvram;

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "ACTION_MODE"))) {
			action_mode = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "SET_NVRAM"))) {
			set_nvram = ds->value;
		}
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "RC_SERVICE"))) {
			rc_service = ds->value;
		}
		
		if(buffer_is_equal_string(action_mode, CONST_STR_LEN("apply"))){

			//- set nvram(ex. wl_bss_enabled=1;wl_unit=0;wl_subunit=1)
			if(!buffer_is_empty(set_nvram)){
				char *pch, *pch2, *saveptr=NULL, *saveptr2=NULL;
				pch = strtok_r(set_nvram->ptr, ";", &saveptr);
				int b_nvram_is_set = 0;
				
				while(pch!=NULL){
					char* key_value = strdup(pch);
					pch2 = strtok_r(key_value, "=", &saveptr2);

					while(pch2!=NULL){
						buffer* buffer_key = buffer_init();
						buffer* buffer_value = buffer_init();
						
						//- key
						if(pch2)
							buffer_copy_string(buffer_key, pch2);
						
						//- value
						pch2 = strtok_r(NULL, "=", &saveptr2);
						if(pch2)
							buffer_copy_string(buffer_value, pch2);

						if(!buffer_is_empty(buffer_key)&&!buffer_is_empty(buffer_value)){

						#if EMBEDDED_EANBLE
							nvram_set_value(buffer_key->ptr, buffer_value->ptr);
						#endif
						
							Cdbg(DBE, "set nvram: key=%s, value=%s", buffer_key->ptr, buffer_value->ptr);
							
							b_nvram_is_set = 1;
						}

						buffer_free(buffer_key);
						buffer_free(buffer_value);
						
						//- Next
						pch2 = strtok_r(NULL, "=", &saveptr2);
					}
					
					if(key_value)
						free(key_value);
					
					//- Next
					pch = strtok_r(NULL, ";", &saveptr);
				}

				if(b_nvram_is_set==1){
				#if EMBEDDED_EANBLE
					nvram_do_commit();
				#endif
				}
			}

			//- run rc_service
			if(!buffer_is_empty(rc_service)){
				char *pch;
				pch = strtok(rc_service->ptr, ";");
				
				while(pch!=NULL){
					
					buffer* rc_command = buffer_init();
					buffer_copy_string(rc_command, "rc rc_service ");
					buffer_append_string(rc_command, pch);

					Cdbg(DBE, "rc_command: %s", rc_command->ptr);
					int ret = system(rc_command->ptr);
					
					if(ret < 0){
						buffer_free(rc_command);
						Cdbg(DBE, "cmd: %s\t error: %s", rc_command->ptr, strerror(errno));
						con->http_status = 400;
						con->file_finished = 1;
						return HANDLER_FINISHED;
		
						break;
					}

					buffer_free(rc_command);
					
					//- Next
					pch = strtok(NULL, ";");
				}
			}
		}
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_NVRAMGET:{
		if(!con->conf.is_ssl){
			con->http_status = 403;
			return HANDLER_FINISHED;
		}
		
		buffer* key;
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "KEY"))) {
			key = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
		
		b = chunkqueue_get_append_buffer(con->write_queue);
		
		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));
		
		if(!buffer_is_empty(key)){
			char *pch;
			pch = strtok(key->ptr, ";");
				
			while(pch!=NULL){
				char* value = NULL;
				#if EMBEDDED_EANBLE
				value = nvram_get_value(pch);
				#endif

				if(value){
					buffer_append_string_len(b,CONST_STR_LEN("<nvram key=\""));
					buffer_append_string(b,pch);
					buffer_append_string_len(b,CONST_STR_LEN("\" value=\""));
					buffer_append_string(b,value);
					buffer_append_string_len(b,CONST_STR_LEN("\"/>"));
				}
				
				//- Next
				pch = strtok(NULL, ";");
			}
		}		

		buffer_append_string_len(b,CONST_STR_LEN("</result>"));
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_UPLOADTOFACEBOOK:{
		Cdbg(1, "do HTTP_METHOD_UPLOADTOFACEBOOK");
		
		buffer* filename;
		buffer* title;
		buffer* album;
		buffer* auth_token;
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "FILENAME"))) {
			filename = ds->value;
			buffer_urldecode_path(filename);
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "TITLE"))) {
			title = ds->value;
			buffer_urldecode_path(title);
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "TOKEN"))) {
			auth_token = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "ALBUM"))) {
			album = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		CURL *curl;
		CURLcode rt;
		struct curl_httppost *formpost = NULL;
		struct curl_httppost *lastptr = NULL;
		char md5[129];
		char* response_str;
		char url_upload_facebook[1024] = "\0";
		curl = curl_easy_init();

		strcpy(url_upload_facebook, "https://graph.facebook.com/");
		strcat(url_upload_facebook, album->ptr);
		strcat(url_upload_facebook, "/photos");

		Cdbg(1, "url_upload_facebook=%s", url_upload_facebook);
		
		if(curl) {
			Cdbg(1, "curl_easy_init OK");

			/* Set Host to target in HTTP header, and set response handler
		 	* function */
			curl_easy_setopt(curl, CURLOPT_URL, url_upload_facebook);


			/* Build the form post */			
			curl_formadd(&formpost, &lastptr,
			             CURLFORM_COPYNAME, "access_token",
			             CURLFORM_COPYCONTENTS, auth_token->ptr, CURLFORM_END);

			curl_formadd(&formpost, &lastptr,
			             CURLFORM_COPYNAME, "message",
			             CURLFORM_COPYCONTENTS, title->ptr, CURLFORM_END);

			curl_formadd(&formpost, &lastptr,
			             CURLFORM_COPYNAME, "status",
			             CURLFORM_COPYCONTENTS, "success", CURLFORM_END);
			
			char photo_path[1024] = "\0";
			sprintf(photo_path, "%s/%s", "/tmp", filename->ptr);

			char photo_src_path[1024] = "\0";
			sprintf(photo_src_path, "%s/%s", con->url.path->ptr, filename->ptr);
	
			int status = 0, ret, src_fd = -1;
			FILE* dst_fd;
			
			if (-1 == (dst_fd = fopen(photo_path, "wb"))) {
				/* opening the destination failed for some reason */
				switch(errno) {
				case EEXIST:
					status = 412;
					break;
				case EISDIR:
					status = 409;
					break;
				case ENOENT:
					/* at least one part in the middle wasn't existing */
					status = 409;
					break;
				default:
					status = 403;
					break;
				}
				fclose(dst_fd);
				
				con->http_status = status;
				return HANDLER_FINISHED;
			}

			char buff[2048];
			if (src_fd = smbc_open(photo_src_path, O_RDONLY, WEBDAV_FILE_MODE)) {
				do {
					ret = smbc_read(src_fd, buff, sizeof(buff));
					if (ret > 0) {
						if (-1 == fwrite(buff, sizeof(char), sizeof(buff), dst_fd)) {
							switch(errno) {
							case ENOSPC:
								status = 507;
								break;
							default:
								status = 403;
								break;
							}
							
							smbc_close(src_fd);
							fclose(dst_fd);
							unlink(photo_path);
							
							con->http_status = status;
							return HANDLER_FINISHED;
						}
					}
				} while (ret > 0);
			}

			smbc_close(src_fd);
			fclose(dst_fd);
			
			Cdbg(DBE, "complete copy file %s -> %s", photo_src_path, photo_path);
						
			curl_formadd(&formpost, &lastptr,
			             CURLFORM_COPYNAME, "source",
			             CURLFORM_FILE, photo_path, CURLFORM_END);

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_callback_func);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_str);
			
			Cdbg(1, "Uploading...");
			rt = curl_easy_perform(curl);

			#if 1
			/* now extract transfer info */ 
			double speed_upload, total_time;
      		curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
      		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
 
      		Cdbg(1, "Speed: %.3f bytes/sec during %.3f seconds, response_str=%s",
              		speed_upload, total_time, response_str);
			
			free(response_str);			
			#else
			if (rt) {
				Cdbg(1, "An error occurred during upload! %s", curl_easy_strerror(rt));
			}
			else{
				/* now extract transfer info */ 
				double speed_upload, total_time;
      			curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
      			curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
 
      			Cdbg(1, "Speed: %.3f bytes/sec during %.3f seconds",
              		speed_upload, total_time);
			}
			#endif
			
			/* Done. Cleanup. */ 
			curl_easy_cleanup(curl);
			curl_formfree(formpost);

			unlink(photo_path);
		}
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}
	
	case HTTP_METHOD_UPLOADTOFLICKR:{
		Cdbg(1, "do HTTP_METHOD_UPLOADTOFLICKR");
	
		buffer* filename;
		buffer* title;
		buffer* auth_token;
			
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "FILENAME"))) {
			filename = ds->value;
			buffer_urldecode_path(filename);
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
	
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "TITLE"))) {
			title = ds->value;
			buffer_urldecode_path(title);
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
			
		if (NULL != (ds = (data_string *)array_get_element(con->request.headers, "TOKEN"))) {
			auth_token = ds->value;
		} else {
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		//char api_key[100] = "37140360286c5cd9952023fa8b662a64";
		//char secret[100] = "804b51d14d840d6e";
		char api_key[100] = "c0466d7736e0275d062ce64aefaacfe0";
		char secret[100] = "228e160cf8805246";
		CURL *curl;
		CURLcode rt;
		struct curl_httppost *formpost = NULL;
		struct curl_httppost *lastptr = NULL;
		char md5[129];
		char* response_str;
		buffer* buffer_photoid = buffer_init();
			
		curl = curl_easy_init();
		if(curl) {
			Cdbg(DBE, "curl_easy_init OK");
	
			/* Set Host to target in HTTP header, and set response handler
			* function */
			curl_easy_setopt(curl, CURLOPT_URL, "https://api.flickr.com/services/upload/");
			/* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */
			
			md5sum(md5, 7, secret, "api_key", api_key, "auth_token", auth_token->ptr, "title", title->ptr); 		
			Cdbg(DBE, "md5=%s", md5);
				
			/* Build the form post */
			curl_formadd(&formpost, &lastptr,
						 CURLFORM_COPYNAME, "api_key",
						 CURLFORM_COPYCONTENTS, api_key, CURLFORM_END);
				
			curl_formadd(&formpost, &lastptr,
						 CURLFORM_COPYNAME, "auth_token",
						 CURLFORM_COPYCONTENTS, auth_token->ptr, CURLFORM_END);
	
			curl_formadd(&formpost, &lastptr,
						 CURLFORM_COPYNAME, "api_sig",
						 CURLFORM_COPYCONTENTS, md5, CURLFORM_END);
				
			curl_formadd(&formpost, &lastptr,
						 CURLFORM_COPYNAME, "title",
						 CURLFORM_COPYCONTENTS, title->ptr, CURLFORM_END);

			char photo_path[1024] = "\0";
			sprintf(photo_path, "/tmp/%s", filename->ptr);
			Cdbg(1, "photo_path=%s", photo_path);

			char photo_src_path[1024] = "\0";
			sprintf(photo_src_path, "%s/%s", con->url.path->ptr, filename->ptr);
	
			int status = 0, ret, src_fd = -1;
			FILE* dst_fd;
			
			if (-1 == (dst_fd = fopen(photo_path, "wb"))) {
				/* opening the destination failed for some reason */
				switch(errno) {
				case EEXIST:
					status = 412;
					break;
				case EISDIR:
					status = 409;
					break;
				case ENOENT:
					/* at least one part in the middle wasn't existing */
					status = 409;
					break;
				default:
					status = 403;
					break;
				}
				fclose(dst_fd);
				
				con->http_status = status;
				return HANDLER_FINISHED;
			}

			char buff[2048];
			if (src_fd = smbc_open(photo_src_path, O_RDONLY, WEBDAV_FILE_MODE)) {
				do {
					ret = smbc_read(src_fd, buff, sizeof(buff));
					if (ret > 0) {
						if (-1 == fwrite(buff, sizeof(char), sizeof(buff), dst_fd)) {
							switch(errno) {
							case ENOSPC:
								status = 507;
								break;
							default:
								status = 403;
								break;
							}
							
							smbc_close(src_fd);
							fclose(dst_fd);
							unlink(photo_path);
							
							con->http_status = status;
							return HANDLER_FINISHED;
						}
					}
				} while (ret > 0);
			}

			smbc_close(src_fd);
			fclose(dst_fd);
			
			Cdbg(DBE, "complete copy file %s -> %s", photo_src_path, photo_path);
			
			curl_formadd(&formpost, &lastptr,
						 CURLFORM_COPYNAME, "photo",
						 CURLFORM_FILE, photo_path, CURLFORM_END);

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
			
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_callback_func);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_str);
			
			Cdbg(1, "Uploading...");
			rt = curl_easy_perform(curl);

			xmlDocPtr xml = xmlParseMemory(response_str, strlen(response_str));
			if(xml){
				xmlNode *rootnode = xmlDocGetRootElement(xml);
				
				if (0 == xmlStrcmp(rootnode->name, BAD_CAST "rsp")) {

					xmlChar *stat = xmlGetProp(rootnode,(const xmlChar*) "stat"); 

					if (0 == xmlStrcmp(stat, BAD_CAST "ok")) {
							
						xmlNode *childnode;
						for (childnode = rootnode->children; childnode; childnode = childnode->next) {
							if (0 == xmlStrcmp(childnode->name, BAD_CAST "photoid")) {
								xmlChar *photoid = xmlNodeGetContent(childnode);
								buffer_copy_string( buffer_photoid, photoid );								
								break;
							}
						}
					}
				}
	
				xmlFreeDoc(xml);
			}
	
			free(response_str);
				
			/* Done. Cleanup. */ 
			curl_easy_cleanup(curl);
			curl_formfree(formpost);

			unlink(photo_path);
		}
	
		if(!buffer_is_empty(buffer_photoid)){
					
			Cdbg(1, "buffer_photoid=%s", buffer_photoid->ptr);
			response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
				
			b = chunkqueue_get_append_buffer(con->write_queue);

			buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
			buffer_append_string_len(b,CONST_STR_LEN("<result>"));
			buffer_append_string_len(b,CONST_STR_LEN("<photoid>"));
			buffer_append_string_buffer(b,buffer_photoid);
			buffer_append_string_len(b,CONST_STR_LEN("</photoid>"));
			buffer_append_string_len(b,CONST_STR_LEN("</result>"));
				
			con->http_status = 200;
		}
		else
			con->http_status = 501;
	
		buffer_free(buffer_photoid);
		
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETCPUUSAGE:{
		Cdbg(DBE, "do HTTP_METHOD_GETCPUUSAGE");

		int line_count = 0;
		int cpu_count = 0;
		unsigned long total, user, nice, system, idle, io, irq, softirq, i, j, k;
		char name[10], tmp[10];
		FILE *fp; 
		fp = fopen("/proc/stat", "r");

		if(fp == NULL){
			con->http_status = 400;
			return HANDLER_FINISHED;
		}

		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
				
		b = chunkqueue_get_append_buffer(con->write_queue);

		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b,CONST_STR_LEN("<result>"));

		while(fscanf(fp, "%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu \n", name, &user, &nice, &system, &idle, &io, &irq, &softirq, &i, &j, &k) != EOF){
			//Cdbg(DBE, "name=%s", name);
			if(strcmp(name, "cpu0") == 0){				
				unsigned long total_0_diff, user_0_diff, cpu0_percentage;
				total = user + nice + system + idle + io + irq + softirq;
				total_0_diff = total - prev_total_0;
				user_0_diff = (system + user + nice + io + irq + softirq) - prev_user_0;
				prev_total_0 = total;
				prev_user_0 = system + user + nice + io + irq + softirq;
				cpu0_percentage = (100*user_0_diff/total_0_diff);
				
				buffer_append_string_len(b,CONST_STR_LEN("<cpu0>"));
				memset(tmp, 0, 10);
				sprintf(tmp, "%lu", cpu0_percentage);
				buffer_append_string(b,tmp);
				buffer_append_string_len(b,CONST_STR_LEN("</cpu0>"));
				
				cpu_count++;
				
			}
			else if(strcmp(name, "cpu1") == 0){
				unsigned long total_1_diff, user_1_diff, cpu1_percentage;
				total = user + nice + system + idle + io + irq + softirq;
				total_1_diff = total - prev_total_1;
				user_1_diff = (system + user + nice + io + irq + softirq) - prev_user_1;
				prev_total_1 = total;
				prev_user_1 = system + user + nice + io + irq + softirq;
				cpu1_percentage = (100*user_1_diff/total_1_diff);
				
				buffer_append_string_len(b,CONST_STR_LEN("<cpu1>"));
				memset(tmp, 0, 10);
				sprintf(tmp, "%lu", cpu1_percentage);
				buffer_append_string(b,tmp);
				buffer_append_string_len(b,CONST_STR_LEN("</cpu1>"));
				
				cpu_count++;
				
			}	
			
			line_count++;

			if(line_count>=3)
				break;
		}

		fclose(fp);
		
		buffer_append_string_len(b,CONST_STR_LEN("<cpucount>"));
		memset(tmp, 0, 10);
		sprintf(tmp, "%d", cpu_count);
		buffer_append_string(b,tmp);
		buffer_append_string_len(b,CONST_STR_LEN("</cpucount>"));		
		buffer_append_string_len(b,CONST_STR_LEN("</result>"));
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}

	case HTTP_METHOD_GETMEMORYUSAGE:{
		Cdbg(DBE, "do HTTP_METHOD_GETMEMORYUSAGE");
		
		unsigned long total, used, mfree;
		char buf[80], tmp[80];
		FILE *fp; 
	
		fp = fopen("/proc/meminfo", "r");
	
		if(fp == NULL){
			con->http_status = 400;
			return HANDLER_FINISHED;
		}
		
		fscanf(fp, "MemTotal: %lu %s\n", &total, buf);	
		fscanf(fp, "MemFree: %lu %s\n", &mfree, buf);	
		used = total - mfree;
		fclose(fp);

		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Type"), CONST_STR_LEN("text/xml; charset=\"utf-8\""));
				
		b = chunkqueue_get_append_buffer(con->write_queue);

		buffer_copy_string_len(b, CONST_STR_LEN("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
		buffer_append_string_len(b, CONST_STR_LEN("<result>"));

		buffer_append_string_len(b, CONST_STR_LEN("<Total>"));
		memset(tmp, 0, 80);
		sprintf(tmp, "%lu", total);
		buffer_append_string(b, tmp);
		buffer_append_string_len(b, CONST_STR_LEN("</Total>"));

		buffer_append_string_len(b, CONST_STR_LEN("<Free>"));
		memset(tmp, 0, 80);
		sprintf(tmp, "%lu", mfree);
		buffer_append_string(b, tmp);
		buffer_append_string_len(b, CONST_STR_LEN("</Free>"));

		buffer_append_string_len(b, CONST_STR_LEN("<Used>"));
		memset(tmp, 0, 80);
		sprintf(tmp, "%lu", used);
		buffer_append_string(b, tmp);
		buffer_append_string_len(b, CONST_STR_LEN("</Used>"));

		buffer_append_string_len(b, CONST_STR_LEN("</result>"));
		
		con->http_status = 200;
		con->file_finished = 1;
		return HANDLER_FINISHED;
	}
	
	default:
		break;
	}

	/* not found */
	return HANDLER_GO_ON;
}

URIHANDLER_FUNC(mod_smbdav_trigger_handler){
#ifndef EMBEDDED_EANBLE

	time_t cur_ts = time(NULL);

#if 0
	//- stop arp every 2 hour
	if(prv_ts != 0 && cur_ts - prv_ts >= 7200){
		stop_arpping_process();
		prv_ts = cur_ts;
	}
#endif

	start_arpping_process(srv->srvconf.arpping_interface->ptr);
#endif

	return HANDLER_GO_ON;
}

/* this function is called at dlopen() time and inits the callbacks */
#ifndef APP_IPKG
int mod_smbdav_plugin_init(plugin *p) {
	
	p->version     = LIGHTTPD_VERSION_ID;
	p->name        = buffer_init_string("smbdav");

	p->init        = mod_smbdav_init;

	p->handle_uri_clean  = mod_smbdav_uri_handler;
	p->handle_physical   = mod_smbdav_subrequest_handler;
	p->set_defaults  = mod_smbdav_set_defaults;
	p->cleanup     = mod_smbdav_free;
	p->data        = NULL;
	p->handle_connection_close = mod_msbdav_connection_close;

#ifndef EMBEDDED_EANBLE
	//- Jerry add 20110930
	p->handle_trigger = mod_smbdav_trigger_handler;
#endif

	read_sharelink_list();
	
	return 0;
}
#else
int aicloud_mod_smbdav_plugin_init(plugin *p) {
	
	p->version     = LIGHTTPD_VERSION_ID;
	p->name        = buffer_init_string("smbdav");

	p->init        = mod_smbdav_init;

	p->handle_uri_clean  = mod_smbdav_uri_handler;
	p->handle_physical   = mod_smbdav_subrequest_handler;
	p->set_defaults  = mod_smbdav_set_defaults;
	p->cleanup     = mod_smbdav_free;
	p->data        = NULL;
	p->handle_connection_close = mod_msbdav_connection_close;

#ifndef EMBEDDED_EANBLE
	//- Jerry add 20110930
	p->handle_trigger = mod_smbdav_trigger_handler;
#endif

	read_sharelink_list();
	
	return 0;
}
#endif
