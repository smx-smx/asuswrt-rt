#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include "libtcapi.h"
#include "tcapi.h"

#define MAX_MULTI_BUF    500
#define MAX_MULTI_COUNT  32

int tcapi_get_int(char* node, char* attr)
{
	char buf[16] = {0};

	if(!node || !attr)
		return 0;

	if(tcapi_get(node, attr, buf))
		return 0;
	else
		return atoi(buf);
}

int tcapi_set_int(char* node, char* attr, int value)
{
	char szValue[16];
	snprintf(szValue, sizeof(szValue), "%d", value);
	return tcapi_set(node, attr, szValue);
}

int tcapi_match(char* node, char* attr, char* value)
{
	char buf[MAXLEN_TCAPI_MSG] = {0};

	if(!node || !attr || !value)
		return 0;

	if(tcapi_get(node, attr, buf))
		return 0;
	else {
		if(strcmp(buf, value))
			return 0;
		else
			return 1;
	}
}
//recommended buf size is 128 or 256 bytes. If your attribute is a rule list, 2048 is recommended.
char *tcapi_get_string(char* node, char* attr, char* buf)
{
	if(!node || !attr || !buf)
		return NULL;
	
	if(tcapi_get(node, attr, buf))
		return NULL;

	return buf;
}

enum TCAPI_LIST_DATA_TYPE
{
	TCAPI_LIST_TYPE_NODE = 0,
	TCAPI_LIST_TYPE_ATTR
};

/*
 * Get and merge to list for UI. e.g. <1>aa>bb>cc>...<2>dd>ee>ff>...<3
 * @param	base_node	base name of tcapi node
 * @param	base_attr	base name of tcapi attribute
 * @param	list_type	0:	merge number to base_node and then get base_attr
 * 							e.g. get base_attr in XXX_Entry0 node, base_attr in XXX_Entry1 ...
 * 							and then merge to the list_buf
 * 						1:	merge number to base_attr and get data from base_node
 * 							e.g. get base_attr, base_attr1, base_attr2, ... from base_node
 * 							and then merge to the list_buf
 * 			list_num	total number of list to get
 * 			list_value	result
 * 			len			size of list_value
 * @return	0:	succsess
 * 			-1:	fail
 */
int _tcapi_get_list(char* base_node, char* base_attr
					, int list_type, int list_num
					, char* list_value, size_t len)
{
	char node[MAXLEN_NODE_NAME];
	char attr[MAXLEN_ATTR_NAME];
	char value[MAXLEN_TCAPI_MSG];
	int i;
	int ret = -1;

	memset(list_value, 0, len);

	if(list_type == TCAPI_LIST_TYPE_ATTR)	//fixed node, varied attr
		strncpy(node, base_node, MAXLEN_NODE_NAME-1);
	else	//varied node, fixed attr
		strncpy(attr, base_attr, MAXLEN_ATTR_NAME-1);

	for(i = 0; i < list_num; i++)	{
		//update node/attr
		if(list_type == TCAPI_LIST_TYPE_ATTR) {
			if(i)
				snprintf(attr, sizeof(attr), "%s%d", base_attr, i);
			else
				snprintf(attr, sizeof(attr), "%s", base_attr);
		}
		else {
			snprintf(node, sizeof(node), "%s%d", base_node, i);
		}

		memset(value, 0, sizeof(value));
		ret = tcapi_get(node, attr, value);
		switch(ret) {
			case TCAPI_PROCESS_OK:
				//_dprintf("get %s %s=%s\n", node, attr, value);
				if( (len - strlen(list_value) -1) > strlen(value) )
					strcat(list_value, value);
				else
					return -1;
				break;
			case TCAPI_NONE_NODE:
			case TCAPI_NONE_ATTR:
			default:
				if(i)
					return 0;
				else
					return -1;
		}
	}
	//_dprintf("finally get %s\n", list_value);
	return 0;

}

/*
 * Parse and set the UI type list. e.g. <1>aa>bb>cc>...<2>dd>ee>ff>...
 * @param	base_node	base name of tcapi node
 * @param	base_attr	base name of tcapi attribute
 * @param	list_type	0:	merge number to base_node and then set base_attr
 * 							i.e. parse list_value and then set base_attr in XXX_Entry0 node, base_attr in XXX_Entry1 ...
 * 						1:	merge number to base_attr and set data to base_node
 * 							i.e. parse list_value and then set base_attr, base_attr1, base_attr2, ... to base_node
 * 			list_num	total number of data to set
 * 			list_value	data to be set
 * @return	0:	succsess
 * 			-1:	fail
 */
int _tcapi_set_list(char* base_node, char* base_attr
					, int list_type, int list_num
					, char* list_value)
{
	char node[MAXLEN_NODE_NAME];
	char attr[MAXLEN_ATTR_NAME];
	char value[MAXLEN_TCAPI_MSG];
	int i;
	char *pch_start, *pch_end;

	if(list_type == TCAPI_LIST_TYPE_ATTR)	//fixed node, varied attr
		strncpy(node, base_node, MAXLEN_NODE_NAME-1);
	else	//varied node, fixed attr
		strncpy(attr, base_attr, MAXLEN_ATTR_NAME-1);

	if(!strcmp(base_attr, "MULTIFILTER_DEVICENAME")
		|| !strcmp(base_attr, "MULTIFILTER_MACFILTER_DAYTIME")
		|| !strcmp(base_attr, "qos_bw_rulelist")
	)
		pch_start = list_value;	//first
	else
		pch_start = strchr(list_value,'<');	//first

	for(i = 0; i < list_num; i++)	{
		//update node/attr
		if(list_type == TCAPI_LIST_TYPE_ATTR) {
			if(i)
				snprintf(attr, sizeof(attr), "%s%d", base_attr, i);
			else
				snprintf(attr, sizeof(attr), "%s", base_attr);
		}
		else {
			snprintf(node, sizeof(node), "%s%d", base_node, i);
		}

		if(!pch_start) {	//wrong format or null string
			_dprintf("set %s=\"\"\n", attr);
			tcapi_set(node, attr, "");
			continue;
		}

		if(*(pch_start+1)) {
			if(!strcmp(base_attr, "MULTIFILTER_DEVICENAME"))
				pch_end = strchr(pch_start+1,'>');
			else if(!strcmp(base_attr, "MULTIFILTER_MACFILTER_DAYTIME"))
				pch_end = strchr(pch_start+1,'#');
			else
				pch_end = strchr(pch_start+1,'<');
			if(pch_end) {
				snprintf(value, pch_end - pch_start + 1, "%s", pch_start);
				if(!strcmp(base_attr, "MULTIFILTER_MACFILTER_DAYTIME"))
					pch_start = pch_end + 1;
				else
					pch_start = pch_end;
			}
			else {	//last
				sprintf(value, "%s", pch_start);
				pch_start = NULL;
			}
			_dprintf("set %s=%s\n", attr, value);
			tcapi_set(node, attr, value);
		}
		else { //when the first attribute is '<', also need to add
			sprintf(value, "%s", pch_start);
			pch_start = NULL;
			tcapi_set(node, attr, value);
		}
	}
	return 0;
}

int _tcapi_x_list(int type, char* target_list, char* list_value, size_t len)
{
	char base_node[MAXLEN_NODE_NAME];
	char base_attr[MAXLEN_ATTR_NAME];
	int list_type = TCAPI_LIST_TYPE_ATTR;
	int list_num = 0;

	if(!strcmp(target_list, "pptpd_clientlist")
		|| !strcmp(target_list, "pptpd_sr_rulelist")
	) {
		snprintf(base_node, sizeof(base_node), "PPTP_Entry");
		snprintf(base_attr, sizeof(base_attr), target_list);
		list_type = TCAPI_LIST_TYPE_ATTR;
		list_num = 16;
	}
	else if(!strcmp(target_list, "vpnc_clientlist")) {
		snprintf(base_node, sizeof(base_node), "VPNC_Entry");
		snprintf(base_attr, sizeof(base_attr), "clientlist");
		list_type = TCAPI_LIST_TYPE_ATTR;
		list_num = 10;
	}
	else if(!strcmp(target_list, "qos_rulelist")) {
		snprintf(base_node, sizeof(base_node), "QoS_Entry0");
		snprintf(base_attr, sizeof(base_attr), target_list);
		list_type = TCAPI_LIST_TYPE_ATTR;
		list_num = 32;
	}
	else if(!strcmp(target_list, "qos_bw_rulelist")) {
		snprintf(base_node, sizeof(base_node), "QoS_Entry0");
		snprintf(base_attr, sizeof(base_attr), target_list);
		list_type = TCAPI_LIST_TYPE_ATTR;
		list_num = 32;
	}
	else if(!strcmp(target_list, "MULTIFILTER_DEVICENAME")) {
		snprintf(base_node, sizeof(base_node), "Parental_Entry");
		snprintf(base_attr, sizeof(base_attr), target_list);
		list_type = TCAPI_LIST_TYPE_ATTR;
		list_num = 32;
	}
	else if(!strcmp(target_list, "MULTIFILTER_MACFILTER_DAYTIME")) {
		snprintf(base_node, sizeof(base_node), "Parental_Entry");
		snprintf(base_attr, sizeof(base_attr), target_list);
		list_type = TCAPI_LIST_TYPE_ATTR;
		list_num = 49; /* (9*12*7*32+31+48)/500=24272/500=48.xxx */
	}
	else if(!strcmp(target_list, "vpn_server_ccd_val")) {
		snprintf(base_node, sizeof(base_node), "OpenVPN_Entry");
		snprintf(base_attr, sizeof(base_attr), "ccd_val");
		list_type = TCAPI_LIST_TYPE_ATTR;
		list_num = 16;
	}
	else {
		_dprintf("\nadd %s handler!!!\n\n", target_list);
		return -1;
	}

	if(type == TCAPI_GET_CMD)
		return _tcapi_get_list(base_node, base_attr, list_type, list_num, list_value, len);
	else
		return _tcapi_set_list(base_node, base_attr, list_type, list_num, list_value);
}

int _tcapi_set_xattr(const char* target_node, char* attr, char* value, size_t len)
{
	int i = 0;
	int cnt = 0;
	char xnode[MAXLEN_NODE_NAME];
	char xattr[MAXLEN_ATTR_NAME];
	char xValue[2048];
	char multiattr[MAXLEN_ATTR_NAME];
	char buf[MAXLEN_TCAPI_MSG];
	char *p;
	
	strncpy(xnode, target_node, MAXLEN_NODE_NAME-1);
	strncpy(xattr, attr, MAXLEN_ATTR_NAME-2);
	p = value;

	if (len > (MAX_MULTI_COUNT * MAX_MULTI_BUF))
		return -1;

	//clean all data
	for(i = 0; i < MAX_MULTI_COUNT; i++) {
		if (i == 0) {
			snprintf(multiattr, sizeof(multiattr), "%s", xattr);
		}
		else {
			snprintf(multiattr, sizeof(multiattr), "%s%d", xattr, i);
		}
		if (tcapi_get(xnode, multiattr, xValue) == TCAPI_PROCESS_OK) {
			tcapi_set(xnode, multiattr, "");
		}
	}

	while( len > MAX_MULTI_BUF ) {
		memset(buf, 0, sizeof(buf)); 
		memcpy(buf, p, MAX_MULTI_BUF);
		if (cnt == 0) {
			snprintf(multiattr, sizeof(multiattr), "%s", xattr);
		}
		else {
			snprintf(multiattr, sizeof(multiattr), "%s%d", xattr, cnt);
		}
		//_dprintf("###[%s(%d)]: buf=%s\n", __FUNCTION__, __LINE__, buf);
		tcapi_set(xnode, multiattr, buf);
		len -= MAX_MULTI_BUF;
		p += MAX_MULTI_BUF;
		cnt++;
	} 

	if (len > 0) {
		memset(buf, 0, sizeof(buf)); 
		memcpy(buf, p, len);
		//_dprintf("###[%s(%d)]: buf=%s\n", __FUNCTION__, __LINE__, buf);
		if (cnt == 0) {
			snprintf(multiattr, sizeof(multiattr), "%s", xattr);
		}
		else {
			snprintf(multiattr, sizeof(multiattr), "%s%d", xattr, cnt);
		}
		tcapi_set(xnode, multiattr, buf);
	}
	
	return 0;
}

int _tcapi_get_xattr(const char* target_node, char* attr, char* value, size_t len)
{
	char xnode[MAXLEN_NODE_NAME];
	char xattr[MAXLEN_ATTR_NAME];
	char xValue[MAXLEN_TCAPI_MSG * MAX_MULTI_COUNT];
	char multiattr[MAXLEN_ATTR_NAME];
	int i;
	int ret = -1;

	strncpy(xnode, target_node, MAXLEN_NODE_NAME-1);
	strncpy(xattr, attr, MAXLEN_ATTR_NAME-2);

	memset(value, 0, len);
	for(i = 0; i < MAX_MULTI_COUNT; i++)	{

		if (i == 0) {
			snprintf(multiattr, sizeof(multiattr), "%s", xattr);
		}
		else {
			snprintf(multiattr, sizeof(multiattr), "%s%d", xattr, i);
		}

		memset(xValue, 0, sizeof(xValue));
		ret = tcapi_get(xnode, multiattr, xValue);
		switch(ret) {
			case TCAPI_PROCESS_OK:
				//_dprintf("[%s(%d)][Get] %s %s=%s\n", __FUNCTION__, __LINE__, xnode, multiattr, xValue);
				if( (len - strlen(value) -1) > strlen(xValue) )
					strcat(value, xValue);
				else
					return -1;
				break;
			case TCAPI_NONE_NODE:
			case TCAPI_NONE_ATTR:
			default:
				if(i) {
					//_dprintf("[%s(%d)][FinallyGet] %s\n", __FUNCTION__, __LINE__, value);
					return 0;
				} else
					return -1;
		}
	}
	return 0;
	
}

int tcapi_get_list(char* target_list, char* list_value, size_t len)
{
	return _tcapi_x_list(TCAPI_GET_CMD, target_list, list_value, len);
}

int tcapi_set_list(char* target_list, char* list_value)
{
	return _tcapi_x_list(TCAPI_SET_CMD, target_list, list_value, 0);
}

int tcapi_get_multiattr(char* target_node, char* attr, char* value, size_t len)
{
	return _tcapi_get_xattr(target_node, attr, value, len);
}

int tcapi_set_multiattr(char* target_node, char* attr, char* value, size_t len)
{
	return _tcapi_set_xattr(target_node, attr, value, len);
}
