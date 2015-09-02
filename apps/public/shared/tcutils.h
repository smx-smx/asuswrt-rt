#ifndef __TCUTILS_H__
#define __TCUTILS_H__

int tcapi_get_int(char* node, char* attr);
int tcapi_match(char* node, char* attr, char* value);
char *tcapi_get_string(char* node, char* attr, char* buffer);

#endif