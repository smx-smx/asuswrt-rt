#ifndef __MINI_ASP_H__
#define __MINI_ASP_H__

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef RA_PARENTALCONTROL
#include "../boa.h"
#endif/*RA_PARENTALCONTROL*/
#ifdef TRENDCHIP
#define ASP_MAX_PARAMS 4
#endif

struct __asp_text{
   const char* str;   
   size_t len;
};
typedef struct __asp_text asp_text;

#if 0
struct __asp_params{
    asp_text params[ASP_MAX_PARAMS];
    int count;
};
typedef struct __asp_params asp_params;
#endif
struct __asp_stmt;

typedef struct __asp_call_stmt {
    asp_text func;
    int id;
#ifndef TRENDCHIP
    asp_text param;
#else
    /*shnwind add 2008.5.22*/
    asp_text params[ASP_MAX_PARAMS]; 
#endif  
//    int count;
}asp_call_stmt;

typedef enum {
    ACO_EQU, ACO_NEQ, ACO_TRUE,
}asp_cond_op;

typedef struct __asp_cond_stmt {
    struct __asp_stmt* next,*last;
    asp_cond_op op;
    struct __asp_stmt* stmt1, *stmt2;
    struct __asp_stmt* stmt_act;
}asp_cond_stmt;

typedef enum {
    AST_HTML, AST_ID, AST_CALL, AST_CALL_ID, AST_CALL_ID_1, AST_STRING,AST_NULL,AST_COND,
}asp_stmt_type;
typedef struct __asp_stmt{
    struct __asp_stmt* next, *last;
    int type;
    union {
	asp_call_stmt v_call;
	asp_text      v_text;
	asp_cond_stmt v_cond;
    };
}asp_stmt;

struct __asp_reent {
#ifdef RA_PARENTALCONTROL
    /* Modify for taking CGI environment to ASP handler by richard.lai */
    request* server_env; 
#else
    void* server_env;
#endif/*RA_PARENTALCONTROL*/
    char* asp_start;
    size_t asp_size;
    size_t asp_lex_pos;
    size_t asp_lex_cur;
    size_t asp_token_len;
    int lex_line;

    int in_asp;
    int error;

    asp_stmt* root;
    asp_stmt* last;
    #ifdef TRENDCHIP
    /*change from 1024*32 to 1024*40 because the buffer size is not enough.
      shnwind 2008.5.19*/
    //unsigned char mem[1024*64];
	/*change from 1024*64 to 1024*128 xyzhu_20100319*/
#if !defined(TCSUPPORT_CD_NEW_GUI) 
#if defined(TCSUPPORT_TTNET) || defined(TCSUPPORT_TURKISH) || defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
				unsigned char mem[1024*200];
#endif
#endif
    #else
    unsigned char mem[1024*32];
    #endif 
    unsigned char* mem_pos;
};
typedef struct __asp_reent asp_reent;

//typedef asp_stmt * asp_yystype;
//#define YYSTYPE asp_yystype

#include "grammar.h"

int gb_parse (void*);

int gb_lex (YYSTYPE* yylval, asp_reent* reent);
void gb_yyerror(const char* msg);

#ifdef RA_PARENTALCONTROL
/* Modify for bring CGI environment to ASP handler by richard.lai */
void do_asp (request* env,const char* filename);
#else
void do_asp (void* env,const char* filename);
#endif/*RA_PARENTALCONTROL*/

void* asp_alloc (asp_reent*, size_t); 
asp_stmt* new_stmt(asp_reent*,asp_stmt_type);

#define sys_alloc(size) malloc(size)
#define sys_free(ptr) free(ptr)

void on_call(asp_reent* reent,const asp_text* func, const asp_text* params,asp_text* ret, int id);
#ifdef TRENDCHIP
typedef void (*asp_func) (asp_reent* reent, const asp_text* param, asp_text* ret);
#else
typedef void (*asp_func) (asp_reent* reent, int id, const asp_text* param, asp_text* ret);
#endif
void append_asp_func (const char* name, asp_func func);
asp_func lookup_asp_func(const char* name, size_t len);

void append_asp_constant (const char* name, int val);
int lookup_asp_constant (const char*name, size_t len, int* ret);

int asp_name_cmp(const char* s1, const char* s2, size_t len);

int asp_send_response (asp_reent* reent, const void* buffer, size_t len);

void gb_error (const char* msg);

#endif /* __MINI_ASP_H__ */

