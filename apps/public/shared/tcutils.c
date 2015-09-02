#include <stdlib.h>
#include <string.h>
#include "libtcapi.h"
#include "tcapi.h"

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