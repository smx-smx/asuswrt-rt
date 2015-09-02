#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "mini-asp.h"
#include "grammar.h"


void* asp_alloc (asp_reent* reent, size_t size)
{
    unsigned char* new_pos;
    void* ret = reent->mem_pos;

    size += (8 - (size % 8));
    new_pos  = reent->mem_pos + size;
    if (new_pos > reent->mem + sizeof(reent->mem))
	return NULL;
    reent->mem_pos = new_pos;
    return ret;
}

asp_stmt* new_stmt (asp_reent* reent, asp_stmt_type type)
{
    asp_stmt* ret = asp_alloc(reent, sizeof(*ret));
    
    memset (ret, 0, sizeof(*ret));    
    ret->type = type;
    ret->next = 0;
    ret->last = ret;
    return ret;
}

int asp_name_cmp(const char* s1, const char* s2, size_t len)
{
    int nr = 0;
    while (len --)
    {
	if (0 != (nr = toupper(*s1++) - toupper(*s2++)))
	    break;
    }
    return nr;
}

struct __asp_funcs {
    struct __asp_funcs* next;
    const char* name;
    size_t len;
    asp_func func;
};

static struct __asp_funcs* asp_funcs = NULL;
asp_func lookup_asp_func (const char* name, size_t len)
{
    struct __asp_funcs* item = asp_funcs;
    
    while (item)
    {
	if (len == item->len)
	    if (0 == asp_name_cmp(name, item->name, len))
		return item->func;
	item = item->next;
    }
    return NULL;
}

void append_asp_func (const char* name, asp_func func)
{
    struct __asp_funcs* item;
    size_t len = strlen(name);
#ifdef ASP_RT_CHECK
    if (lookup_asp_func(name, len))
	/* What to do? */ ;
#endif    
    item = sys_alloc (sizeof(*item));
    item->name = name;
    item->len = len;
    item->func = func;
    item->next = asp_funcs;
    asp_funcs = item;
}

/* constant */
struct __constant_item 
{
    struct __constant_item* next;
    int len;
    const char* name;
    int value;    
};
typedef struct __constant_item constant_item;
static constant_item* constants;
int lookup_asp_constant (const char* name, size_t len, int* ret)
{
    struct __constant_item* item = constants;
    
    while (item)
    {
	if (len == item->len)
	    if (0 == asp_name_cmp(name, item->name, len))
	    {
		*ret = item->value;
		return 0;
	    }
	
	item = item->next;
    }
    return 1;
}

void append_asp_constant (const char* name, int val)
{
    struct __constant_item* item;
    size_t len = strlen(name);
#ifdef ASP_RT_CHECK
    if (lookup_asp_constant(name, len))
	/* What to do? */ ;
#endif    
    item = sys_alloc (sizeof(*item));
    item->name = name;
    item->len = len;
    item->value = val;
    item->next = constants;
    constants = item;
}
    
static int safe_read (int fd, void* buffer, size_t size)
{
    int nr;
    int ret = 0;
    
    while (size)
    {
	nr = read (fd, buffer, size);
	if (nr <= 0)
	    return nr;
	ret += nr;
	size -= nr;

	buffer = (void*) (((char*)buffer) + nr);
    }
    return ret;
}

static void read_asp (asp_reent* reent, const char* filename)
{
    int fd = open (filename, O_RDONLY);

    if (fd >= 0)
    {
	struct stat st_buf;
	
	if ((0 != fstat (fd, &st_buf)) || !(reent->asp_start = sys_alloc(st_buf.st_size)))
	    goto exit;

	if (st_buf.st_size != safe_read (fd, reent->asp_start, st_buf.st_size))
	{
	    sys_free (reent->asp_start);
	    reent->asp_start = 0;
	    goto exit;
	}

	reent->asp_size = st_buf.st_size;
exit:
	close(fd);
    }
}

static void run_stmt (asp_reent* reent, asp_stmt* stmt, asp_text* ret);
static void run_stmts (asp_reent* reent, asp_stmt* stmt);

static void run_cond (asp_reent* reent, asp_stmt* stmt)
{
    asp_cond_stmt* cond_stmt = &stmt->v_cond;
    
    while (cond_stmt)
    {
	asp_text r1, r2;
	int eq = 0;

	if (ACO_TRUE == cond_stmt->op)
	{
	    run_stmts (reent, cond_stmt->stmt_act);
	    break;
	}
	run_stmt (reent, cond_stmt->stmt1, &r1);
	run_stmt (reent, cond_stmt->stmt2, &r2);
	if (r1.len == r2.len)
	    if (!memcmp (r1.str, r2.str, r1.len))
		eq = 1;
	if (1 == eq && ACO_EQU == cond_stmt->op)
	{
	    run_stmts (reent, cond_stmt->stmt_act);
	    break;
	}
	if (0 == eq && ACO_NEQ == cond_stmt->op)
	{
	    run_stmts (reent, cond_stmt->stmt_act);
	    break;
	}
  
	stmt = cond_stmt->next;
	if (stmt)
	    cond_stmt = &stmt->v_cond;
	else
	    cond_stmt = NULL;
    }
}

static void run_stmt (asp_reent* reent, asp_stmt* stmt, asp_text* ret)
{
    ret->len = 0;
    ret->str = 0;

    switch (stmt->type)
    {
    case AST_HTML:
	asp_send_response (reent, stmt->v_text.str, stmt->v_text.len);
	break;
    case AST_CALL:
    case AST_CALL_ID:
    case AST_CALL_ID_1:
    #ifdef TRENDCHIP
	on_call (reent, &stmt->v_call.func, stmt->v_call.params, ret, stmt->v_call.id);
    #else
        on_call (reent, &stmt->v_call.func, &stmt->v_call.param, ret, stmt->v_call.id);
    #endif
	break;
    case AST_ID:
	break;
    case AST_STRING:
	*ret = stmt->v_text;
	break;
    case AST_NULL:
	break;
    case AST_COND:
	run_cond (reent, stmt);
	break;
    default:
	/* What to do? */;
    }    
}

static void run_stmts (asp_reent* reent, asp_stmt* stmt)
{
    asp_text ret;
    while (stmt)
    {
	run_stmt (reent, stmt, &ret);
	stmt = stmt->next;
    }    
}

static asp_reent* dbg_reent;
#ifdef RA_PARENTALCONTROL
/* Modify for taking CGI environment to ASP handler by richard.lai */
void do_asp (request* env, const char* filename) 
#else
void do_asp (void* env, const char* filename)
#endif/*RA_PARENTALCONTROL*/
{
    asp_reent* reent = sys_alloc (sizeof(*reent));
    
    if (!reent)
	return;
    memset (reent, 0, sizeof(*reent));

    reent->server_env = env;
    reent->mem_pos = reent->mem;
    dbg_reent = reent;

    reent->lex_line = 1;
    read_asp (reent, filename);

    if (!reent->asp_start) {
	sys_free(reent);
	return;
    }

    /* Do */
    gb_parse (reent);

    if (!reent->error)
	run_stmts (reent, reent->root);


    if (reent->asp_start)
	sys_free(reent->asp_start);
    sys_free(reent);
}


void on_html_block(asp_reent* reent, const asp_text* html)
{
    fprintf(stderr, "%s %d\n", __FUNCTION__, html->len);
    fwrite (html->str, html->len, 1, stderr);
}


void on_call (asp_reent* reent, const asp_text* func, const asp_text* param, asp_text* ret, int id)
{
    asp_func _func;
    
    ret->str = "";
    ret->len = 0;
    
    _func = lookup_asp_func (func->str, func->len);
    if (_func)
    {
    #ifdef TRENDCHIP
      _func (reent, param, ret);
    #else
      _func (reent, id, param, ret);
    #endif
    }
}

void gb_error (const char* msg)
{
    char buff[128];
    int len;

    if (dbg_reent)
    {
	len = sprintf (buff, "ASP %s at line: %d\n", msg, dbg_reent->lex_line);
	asp_send_response( dbg_reent, buff, len);
	dbg_reent->error = 1;
    }
}
