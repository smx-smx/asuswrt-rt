#ifndef _HTTP_GET_UTILS_H_
#define _HTTP_GET_UTILS_H_
#include "../boa.h"

typedef struct var_s {
	char	*name;
	char	*value;
} s_var;

/*2006-7-30 lee:global value for trendchip asp function,so i can get posted vars*/
s_var **g_var_post;

#define SUCCESS		0
#define FAIL		-1
#define KEEP_SEARCH	-2
#define FIND_BINARY_DATA -3

#define GET_BOUNDARY 	1
#define GET_CONTENT 	2
#define GET_VALUE   	3
#define ERROR		4
#define MULTI_PASER_TEMP_SIZE 1024
#if !defined(TCSUPPORT_CT) 
#define STRING_BUF_SIZE	2048
#endif
#define MULTI_TO_NORMAL_TEMP "/tmp/parse_temp"


char   *get_param        (s_var **, const char *);
s_var **parse_param_line (char *);
void free_param_line(s_var **vars);

//shnwind add for multipart
int multipart_parser(request *req, char *boundary);
int paser_get_boundary(char *buf, int buf_size, char *boundary);
int find_data_end(int bound_pos ,int fd, int write_fd);
int paser_get_content(char *buf, int buf_size, char *name, int pos, int write_fd);
int paser_get_value(char *buf, int buf_size, char *value, char *boundary);
void write_name_value(int fd, char *name, char *value);
int get_parameter_value(char *buf, char *paremeter, char *ret_val);

#endif
