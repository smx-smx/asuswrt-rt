#include "base.h"
#include "log.h"
#include "buffer.h"

#include "plugin.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <dlinklist.h>

#if EMBEDDED_EANBLE
#ifndef APP_IPKG
#include "disk_share.h"
#endif
#endif

#define DBE 1

typedef struct {
	array *access_deny;	
	array *auth_deny;
} plugin_config;

typedef struct {
	PLUGIN_DATA;

	plugin_config **config_storage;

	plugin_config conf;
	
	buffer *tmp_buf;
	
	smb_info_t *smb_info_list;
} plugin_data;

INIT_FUNC(mod_aicloud_auth_init) {
	plugin_data *p;

	p = calloc(1, sizeof(*p));

	return p;
}

FREE_FUNC(mod_aicloud_auth_free) {
	plugin_data *p = p_d;

	UNUSED(srv);

	if (!p) return HANDLER_GO_ON;

	if (p->config_storage) {
		size_t i;
		for (i = 0; i < srv->config_context->used; i++) {
			plugin_config *s = p->config_storage[i];

			array_free(s->access_deny);
			array_free(s->auth_deny);
			
			free(s);
		}
		free(p->config_storage);
	}

	free(p);

	return HANDLER_GO_ON;
}

SETDEFAULTS_FUNC(mod_aicloud_auth_set_defaults) {
	plugin_data *p = p_d;
	size_t i = 0;

	config_values_t cv[] = {
		{ "url.access-deny",             NULL, T_CONFIG_ARRAY, T_CONFIG_SCOPE_CONNECTION },
		{ "url.aicloud-auth-deny",       NULL, T_CONFIG_ARRAY, T_CONFIG_SCOPE_CONNECTION },
		{ NULL,                          NULL, T_CONFIG_UNSET, T_CONFIG_SCOPE_UNSET }
	};

	p->config_storage = calloc(1, srv->config_context->used * sizeof(specific_config *));
	
	for (i = 0; i < srv->config_context->used; i++) {
		plugin_config *s;

		s = calloc(1, sizeof(plugin_config));
		s->access_deny    = array_init();
		s->auth_deny      = array_init();
		
		cv[0].destination = s->access_deny;
		cv[1].destination = s->auth_deny;

		p->config_storage[i] = s;

		if (0 != config_insert_values_global(srv, ((data_config *)srv->config_context->data[i])->value, cv)) {
			return HANDLER_ERROR;
		}
	}

	return HANDLER_GO_ON;
}

static void aicloud_check_direct_file(server *srv, connection *con)
{
	config_values_t cv[] = {
		{ "alias.url",		NULL, T_CONFIG_ARRAY, T_CONFIG_SCOPE_CONNECTION },       /* 0 */
		{ NULL,             NULL, T_CONFIG_UNSET,  T_CONFIG_SCOPE_UNSET }
	};
	
	size_t i, j;
	for (i = 1; i < srv->config_context->used; i++) {
		int found = 0;
		array* alias = array_init();
		cv[0].destination = alias;
	
		if (0 != config_insert_values_global(srv, ((data_config *)srv->config_context->data[i])->value, cv)) {
			continue;
		}
		
		for (j = 0; j < alias->used; j++) {
			data_string *ds = (data_string *)alias->data[j];			
						
			if( strncmp(con->request.uri->ptr, ds->key->ptr, ds->key->used-1) == 0 ){
				con->mode = DIRECT;
				found = 1;				
				break;
			}
		}

		array_free(alias);

		if(found==1)
			break;
	}
}

#define PATCH(x) \
	p->conf.x = s->x;

static int check_aicloud_auth_url(server *srv, connection *con, plugin_data *p){
	smb_info_t *c;
	int i, j, k;
	plugin_config *s;
	
	/* skip the first, the global context */
	for (i = 1; i < srv->config_context->used; i++) {
		data_config *dc = (data_config *)srv->config_context->data[i];
		s = p->config_storage[i];

		/* condition didn't match */
		if (!config_check_cond(srv, con, dc)) continue;

		/* merge config */
		for (j = 0; j < dc->value->used; j++) {
			data_unset *du = dc->value->data[j];

			if (buffer_is_equal_string(du->key, CONST_STR_LEN("url.aicloud-auth-deny"))) {
				PATCH(auth_deny);
			}
		}
	}

	if(p->conf.auth_deny){
	
		for (k = 0; k < p->conf.auth_deny->used; k++) {
			data_string *ds = (data_string *)p->conf.auth_deny->data[k];
			
			if (ds->value->used == 0) continue;
			
			if (strstr(con->uri.path->ptr, ds->value->ptr)) {
				
				if(p->smb_info_list==NULL)
					return 0;
				
				for (c = p->smb_info_list; c; c = c->next) {
					if( buffer_is_empty(c->server) &&
						buffer_is_empty(c->share) &&
						buffer_is_equal(c->src_ip, con->dst_addr_buf) ){
						if(buffer_is_empty(c->username))
							return 0;
						else
							return 1;			
					}				
				}
				return 0;
			}
		}

	}
	
	return 1;
}

static void get_aicloud_connection_auth_type(server *srv, connection *con)
{
	data_string *ds;
	int found = 0;
		
	aicloud_check_direct_file(srv, con);

	if(con->mode==DIRECT)
		return;

	if (NULL == (ds = (data_string *)array_get_element(con->request.headers, "user-Agent")))
		return;
		
	config_values_t cv[] = {
		{ "smbdav.auth_ntlm",    NULL, T_CONFIG_ARRAY, T_CONFIG_SCOPE_CONNECTION },       /* 0 */
		{ NULL,                         NULL, T_CONFIG_UNSET, T_CONFIG_SCOPE_UNSET }
	};
	
	size_t i, j;
	for (i = 1; i < srv->config_context->used; i++) {
		int found = 0;
		array* auth_ntlm_array = array_init();
		cv[0].destination = auth_ntlm_array;
	
		if (0 != config_insert_values_global(srv, ((data_config *)srv->config_context->data[i])->value, cv)) {
			continue;
		}
		
		for (j = 0; j < auth_ntlm_array->used; j++) {
			data_string *ds2 = (data_string *)auth_ntlm_array->data[j];

			if(ds2->key->used){
				/*
				Cdbg(DBE, "ds2->key=%s, ds2->value=%s", 
					ds2->key->ptr, 
					ds2->value->ptr );
				*/
				if( strncmp(ds->value->ptr, ds2->value->ptr, ds2->value->used-1) == 0 ){
					con->mode = SMB_NTLM;
					found = 1;
					break;
				}
			}
		}

		array_free(auth_ntlm_array);

		if(found==1)
			break;
	}

}

static void aicloud_connection_smb_info_url_patch(server *srv, connection *con)
{
	char strr[2048]="\0";
	char uri[2048]="\0";
	
	UNUSED(srv);
	
	char* pch = strchr(con->request.uri->ptr,'?');
	if(pch){	
		buffer_copy_string_len(con->url_options, pch+1, strlen(pch)-1);
		int len = pch-con->request.uri->ptr;
		strncpy(uri,con->request.uri->ptr, len);
	}
	else{
		strcpy(uri,con->request.uri->ptr);
	}
	
	if(con->mode == DIRECT){
		sprintf(strr, "%s", uri);
	}
	else {
		if(con->smb_info&&con->smb_info->server->used) {
			if(con->mode == SMB_BASIC){
				if(con->smb_info->username->used&&con->smb_info->password->used){
					sprintf(strr, "smb://%s:%s@%s", con->smb_info->username->ptr, con->smb_info->password->ptr, uri+1);
				}
				else
					sprintf(strr, "smb://%s", uri+1);
			}
			else if(con->mode == SMB_NTLM){
				sprintf(strr, "smb://%s", uri+1);		
			}
		} else {
			sprintf(strr, "smb://");
		}
	}

	buffer_copy_string(con->url.path, strr);
	buffer_copy_string(con->url.rel_path, uri);	

	buffer_urldecode_path(con->url.path);
	buffer_urldecode_path(con->url.rel_path);
	
}

static smb_info_t *smbdav_get_smb_info_from_pool(server *srv, connection *con, plugin_data *p)
{
	smb_info_t *c;

	if(p->smb_info_list==NULL||con->mode==DIRECT)
		return NULL;
	
	//- Get user-Agent
	data_string *ds = (data_string *)array_get_element(con->request.headers, "user-Agent");
	if(ds==NULL){
		return NULL;
	}
	
	char pWorkgroup[30]={0};
	char pServer[64]={0};
	char pShare[1280]={0};
	char pPath[1280]={0};
	
	smbc_wrapper_parse_path2(con, pWorkgroup, pServer, pShare, pPath);

	buffer* buffer_server = buffer_init();
	if(pServer[0] != '\0')
		buffer_append_string(buffer_server,pServer);
	
	buffer* buffer_share = buffer_init();
	if(pShare[0] != '\0')
		buffer_append_string(buffer_share,pShare);
	
	int count = 0;
		
	for (c = p->smb_info_list; c; c = c->next) {
		
		count++;
		
		if(!buffer_is_equal(c->server, buffer_server))
			continue;

		//Cdbg(DBE, "c->share=[%s], buffer_share=[%s]", c->share->ptr, buffer_share->ptr);
		//if(con->mode==SMB_BASIC && !buffer_is_equal(c->share, buffer_share))
		//	continue;

		//Cdbg(DBE, "%d, c->src_ip=[%s], dst_addr_buf=[%s]", count, c->src_ip->ptr, con->dst_addr_buf->ptr);
		if(!buffer_is_equal(c->src_ip, con->dst_addr_buf))
			continue;
		
		Cdbg(DBE, "%d, c->user_agent=[%s], user_agent=[%s]", count, c->user_agent->ptr, ds->value->ptr);
		if(!buffer_is_equal(c->user_agent, ds->value)){
			continue;
		}
		
		//Cdbg(DBE, "return %d, c->server=[%s]", count, c->server->ptr);

		buffer_free(buffer_server);
		buffer_free(buffer_share);
	
		return c;
	}

	buffer_free(buffer_server);
	buffer_free(buffer_share);
	
	return NULL;
}

static int aicloud_connection_smb_info_init(server *srv, connection *con, plugin_data *p) 
{
	UNUSED(srv);

	char pWorkgroup[30]={0};
	char pServer[64]={0};
	char pShare[1280]={0};
	char pPath[1280]={0};
		
	smbc_wrapper_parse_path2(con, pWorkgroup, pServer, pShare, pPath);
	
	buffer* bworkgroup = buffer_init();
	buffer* bserver = buffer_init();
	buffer* bshare = buffer_init();
	buffer* bpath = buffer_init();
	URI_QUERY_TYPE qflag = SMB_FILE_QUERY;
	
	if(pWorkgroup[0] != '\0')
		buffer_copy_string(bworkgroup, pWorkgroup);
	
	if(pServer[0] != '\0') {		
		int isHost = smbc_check_connectivity(con->physical_auth_url->ptr);
		if(isHost) {
			buffer_copy_string(bserver, pServer);
		}
		else{
			buffer_free(bworkgroup);
			buffer_free(bserver);
			buffer_free(bshare);
			buffer_free(bpath);
			return 2;
		}
	} 
	else {
		if(qflag == SMB_FILE_QUERY) {
			qflag = SMB_HOST_QUERY;
		}
	}
	
	if(pServer[0] != '\0' && pShare[0] != '\0') {
		buffer_copy_string(bshare, pShare);
	} 
	else {
		if(qflag == SMB_FILE_QUERY)  {
			qflag = SMB_SHARE_QUERY;
		}
	}
	
	if(pServer[0] != '\0' && pShare[0] != '\0' && pPath[0] != '\0') {
		buffer_copy_string(bpath, pPath);
		qflag = SMB_FILE_QUERY;
	}

	data_string *ds = (data_string *)array_get_element(con->request.headers, "user-Agent");

	smb_info_t *smb_info;

	if( ds && 
	    ( strstr( ds->value->ptr, "Mozilla" ) || 
	      strstr( ds->value->ptr, "Opera" ) || 
	      con->mode == SMB_NTLM ) ){
	    
		//- From browser, like IE, Chrome, Firefox, Safari		
		if(smb_info = smbdav_get_smb_info_from_pool(srv, con, p)){ 
			Cdbg(DBE, "Get smb_info from pool smb_info->qflag=[%d], smb_info->user=[%s], smb_info->pass=[%s]", 
				smb_info->qflag, smb_info->username->ptr, smb_info->password->ptr);
		}
		else{
			smb_info = calloc(1, sizeof(smb_info_t));
			smb_info->username = buffer_init();
			smb_info->password = buffer_init();
			smb_info->workgroup = buffer_init();
			smb_info->server = buffer_init();
			smb_info->share = buffer_init();
			smb_info->path = buffer_init();
			smb_info->user_agent = buffer_init();
			smb_info->src_ip = buffer_init();			
			
			if(con->mode == SMB_NTLM){
				smb_info->cli = smbc_cli_initialize();
				if(!buffer_is_empty(bserver)){
					smbc_cli_connect(smb_info->cli, bserver->ptr, SMB_PORT);
				}
				smb_info->ntlmssp_state = NULL; 
				smb_info->state = NTLMSSP_INITIAL;
			}

			DLIST_ADD(p->smb_info_list, smb_info);			
		}	
		con->smb_info = smb_info;
			
	}
	else{		
		smb_info = calloc(1, sizeof(smb_info_t));
		smb_info->username = buffer_init();
		smb_info->password = buffer_init();
		smb_info->workgroup = buffer_init();
		smb_info->server = buffer_init();
		smb_info->share = buffer_init();
		smb_info->path = buffer_init();
		smb_info->user_agent = buffer_init();
		smb_info->src_ip = buffer_init();			
		
		con->smb_info = smb_info;
	}
	
	con->smb_info->auth_time = time(NULL);	
	con->smb_info->auth_right = 0;

	if(ds)
		buffer_copy_string(con->smb_info->user_agent, ds->value->ptr);
	
	con->smb_info->qflag = qflag;
	buffer_copy_string_buffer(con->smb_info->workgroup, bworkgroup);
	buffer_copy_string_buffer(con->smb_info->server, bserver);
	buffer_copy_string_buffer(con->smb_info->share, bshare);
	buffer_copy_string_buffer(con->smb_info->path, bpath);
	buffer_copy_string_buffer(con->smb_info->src_ip, con->dst_addr_buf);

	Cdbg(DBE, "con->smb_info->workgroup=[%s]", con->smb_info->workgroup->ptr);
	Cdbg(DBE, "con->smb_info->server=[%s]", con->smb_info->server->ptr);
	Cdbg(DBE, "con->smb_info->share=[%s]", con->smb_info->share->ptr);
	Cdbg(DBE, "con->smb_info->path=[%s]", con->smb_info->path->ptr);
	Cdbg(DBE, "con->smb_info->user_agent=[%s]", con->smb_info->user_agent->ptr);
	Cdbg(DBE, "con->smb_info->src_ip=[%s]", con->smb_info->src_ip->ptr);
	Cdbg(DBE, "con->smb_info->qflag=[%d]", con->smb_info->qflag);
		
	buffer_free(bworkgroup);
	buffer_free(bserver);
	buffer_free(bshare);
	buffer_free(bpath);

	return 1;
}

void sambaname2ip(server *srv, connection *con){

#if EMBEDDED_EANBLE
	
	char* aa = nvram_get_smbdav_str();

	if(aa==NULL){
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
			
			char name[50]={0}, ip[20]={0};
			int name_len, ip_len;
			
			//- PC Name
			name_len = strlen(pch);			
			strncpy(name, pch, name_len);
			name[name_len] = '\0';
						
			//- IP Address
			pch = strtok(NULL,"<>");
			ip_len = strlen(pch);
			strncpy(ip, pch, ip_len);
			ip[ip_len] = '\0';
						
			//- MAC Address
			pch = strtok(NULL,"<>");
						
			//- PC Online?
			pch = strtok(NULL,"<>");
			
			int index = strstr(con->request.uri->ptr, name) - con->request.uri->ptr;			
			if(index==1 && strcmp(pch, "1")==0){
				char buff[4096];
				char* tmp = replace_str(con->request.uri->ptr, 
									    name, 
									    ip, 
									    (char *)&buff[0]);
								
				buffer_copy_string(con->request.uri, tmp);	
				
				buffer_copy_string(con->match_smb_ip, ip);
				buffer_copy_string(con->replace_smb_name, name);
				
				break;
			}
			
			pch = strtok(NULL,"<>");			
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

			char name[50]={0}, ip[20]={0};
			int name_len, ip_len;
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
			
			//- MAC Address
			pch = strtok(NULL,"<");
			
			//- PC Online?
			pch = strtok(NULL,"<");			

			int index = strstr(con->request.uri->ptr, name) - con->request.uri->ptr;			
			if(index==1 && strcmp(pch, "1")==0){
				char buff[4096];
				char* tmp = replace_str(con->request.uri->ptr, 
									    name, 
									    ip,
									    (char *)&buff[0]);
								
				buffer_copy_string(con->request.uri, tmp);	
				
				buffer_copy_string(con->match_smb_ip, ip);
				buffer_copy_string(con->replace_smb_name, name);				
				
				break;
			}
			
		}
			
		fclose(fp);
	}
#endif

}

URIHANDLER_FUNC(mod_aicloud_auth_physical_handler){
	plugin_data *p = p_d;
	int s_len;
	size_t k;
	int res = HANDLER_UNSET;
	
	if (con->uri.path->used == 0) return HANDLER_GO_ON;

#if EMBEDDED_EANBLE
	data_string *ds_useragent = (data_string *)array_get_element(con->request.headers, "user-Agent");
	
	int isBrowser = ( ds_useragent && (strstr( ds_useragent->value->ptr, "Mozilla" ) || strstr( ds_useragent->value->ptr, "Opera" ))) ? 1 : 0;
	if( isBrowser && 
		con->conf.is_ssl==0 && 
		buffer_is_equal_string(con->request.uri, CONST_STR_LEN("/")) ){
		con->http_status = 452;
		return HANDLER_FINISHED;
	}
#endif

	if(strncmp(con->request.uri->ptr, "/GetCaptchaImage", 16)==0)
		return HANDLER_GO_ON;

	sambaname2ip(srv, con);

	Cdbg(DBE, "con->request.uri=%s", con->request.uri->ptr);
		
	con->mode = SMB_BASIC;
	
	get_aicloud_connection_auth_type(srv, con);
	
	if(con->mode == DIRECT){

		if( !check_aicloud_auth_url(srv, con, p)){
			con->http_status = 403;
			return HANDLER_FINISHED;
		}
		
		aicloud_connection_smb_info_url_patch(srv, con);
		return HANDLER_GO_ON;
	}
			
	Cdbg(DBE,"***************************************");
	Cdbg(DBE,"enter do_connection_auth..con->mode = %d, con->request.uri=[%s]", con->mode, con->request.uri->ptr);
	
	config_cond_cache_reset(srv, con);
	config_patch_connection(srv, con, COMP_SERVER_SOCKET);
	config_patch_connection(srv, con, COMP_HTTP_URL);
		
	buffer_copy_string_buffer(con->physical_auth_url, con->conf.document_root);
	buffer_append_string(con->physical_auth_url, con->request.uri->ptr+1);
	
	int result = aicloud_connection_smb_info_init(srv, con, p);	
	if( result == 0 ){
		return HANDLER_FINISHED;
	}
	else if( result == 2 ){
		//- Unable to complete the connection, the device is not turned on, or network problems caused!
		con->http_status = 451;
		return HANDLER_FINISHED;
	}

	/*
	//- Jerry test
	if( strcmp(con->request.uri->ptr, "/")!=0 ){
		srv->smb_srv_info_list = p->smb_info_list;
		aicloud_connection_smb_info_url_patch(srv, con);
		return HANDLER_GO_ON;
	}
	*/
	
	if(con->mode == SMB_NTLM) {
		//try to get NTLM authentication information from HTTP request
		res = ntlm_authentication_handler(srv, con, p);		
	} else if(con->mode == SMB_BASIC){
		//try to get username/password from HTTP request
		res = basic_authentication_handler(srv, con, p);
	}
	
	//- 20120202 Jerry add
	srv->smb_srv_info_list = p->smb_info_list;	
	aicloud_connection_smb_info_url_patch(srv, con);
	
	buffer_reset(con->physical_auth_url);

	if(res != HANDLER_UNSET){
		return HANDLER_FINISHED;
	}

	/* not found */
	return HANDLER_GO_ON;
}

int mod_aicloud_auth_plugin_init(plugin *p);
int mod_aicloud_auth_plugin_init(plugin *p) {
	p->version     = LIGHTTPD_VERSION_ID;
	p->name        = buffer_init_string("aicloud_auth");

	p->init        = mod_aicloud_auth_init;
	p->set_defaults = mod_aicloud_auth_set_defaults;	
	p->handle_physical = mod_aicloud_auth_physical_handler;
	p->cleanup     = mod_aicloud_auth_free;

	p->data        = NULL;

	return 0;
}
