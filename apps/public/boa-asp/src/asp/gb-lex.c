
#include <ctype.h>
#include <stdio.h>

#include "mini-asp.h"
#include "grammar.h"

#define lexer_stream_eof(reent) (reent->asp_lex_pos>=reent->asp_size)

static void skip_space (asp_reent* reent)
{
    while (!lexer_stream_eof(reent))
    {
	if ('\n' == reent->asp_start[reent->asp_lex_pos])
	    reent->lex_line ++;
	if (!isspace(reent->asp_start[reent->asp_lex_pos]))
	    break;
	reent->asp_lex_pos++;
    }
}

const static struct {
    const char* name;
    int len;
    int id;
} keywords [] = {
    {"If", 2, GB_IF},
    {"Else", 4, GB_ELSE},
    {"ElseIf", 6, GB_ELSEIF},
    {"End", 3, GB_END},
    {"Then", 4, GB_THEN},
    {"@SCRIPT", 7, GB_SCRIPT},
};

#if 0 /* debug */
static const char* name_of_kw (int k)
{
    int i;
    
    for (i = 0; i < sizeof(keywords) /sizeof(*keywords); i++)
	if (k == keywords[i].id)
	    return keywords[i].name;
    return "UN";
}
#endif

#define keyword_cmp asp_name_cmp
static int check_id (asp_reent* reent)
{
    int i;
    int len = reent->asp_token_len;
    char* token = reent->asp_start + reent->asp_lex_cur;

    for (i = 0; i < sizeof(keywords)/sizeof(*keywords); i++)
	if (len == keywords[i].len)
	    if (!keyword_cmp (token, keywords[i].name, len))
		return keywords[i].id;

    return GB_ID;
}

static int do_lex_asp(YYSTYPE* yylval, asp_reent* reent)
{
    int state = 0;
    int in_string = 0;

    skip_space (reent);
    reent->asp_lex_cur = reent->asp_lex_pos;
    reent->asp_token_len = 0;
    while (!lexer_stream_eof(reent))
    {
	char c = reent->asp_start[reent->asp_lex_pos];

	if (in_string)
	{
	    reent->asp_lex_pos ++;
	    if ('\"' == c)
		return GB_STRING;
       	    reent->asp_token_len ++;
	}
	else if (!state && ('%' == c))
	{
	    if (reent->asp_token_len)
		return check_id(reent);
	    state = 1;
	    reent->asp_lex_pos ++;
	}
	else if (state && ('>' == c))
	{
	    assert(!reent->asp_token_len);
	    reent->asp_lex_pos ++;
	    return 0;
	}
	else if (!reent->asp_token_len)
	{
	    switch (c)
	    {
	    case '\"':
		in_string = 1;
		reent->asp_lex_pos ++;
		reent->asp_lex_cur ++;
		break;
	    case '=':
	    case '<':
	    case '>':
	    case ',':
	    case ')':
	    case '(':
		reent->asp_lex_pos ++;
		return c;
	    default:
		reent->asp_lex_pos ++;
		reent->asp_token_len ++;
	    }
	}
	else
	{
	    if (isspace(c))
		return check_id(reent);
	    switch (c)
	    {
	    case '\"':
	    case '=':
	    case '<':
	    case '>':
	    case ')':
	    case '(':
	    case ',':
		return check_id(reent);
	    default:
		reent->asp_lex_pos ++;
		reent->asp_token_len ++;
	    }
	}
    }
    return 0;
}

int gb_lex (YYSTYPE* yyval, asp_reent* reent)
{
    int state = 0;

    if (lexer_stream_eof(reent))
	return 0;
    
    if (reent->in_asp)
    {
	int nr = do_lex_asp(yyval, reent);
	if (nr)
	{
	    //    fprintf (stderr, "[[ASP-LEX: %d, (%s) line: %d]]\n", nr, name_of_kw(nr), reent->lex_line);
	    yyval->v_text.str = reent->asp_start + reent->asp_lex_cur;
	    yyval->v_text.len = reent->asp_token_len;
	    return nr;
	}
    }

    /* HTML_BLOCK */
    reent->asp_lex_cur = reent->asp_lex_pos;
    while (!lexer_stream_eof(reent))
    {
	char c = reent->asp_start[reent->asp_lex_pos++];
	if ('\n' == c)
	    reent->lex_line ++;
	if (state)
	{
	    if ('%' == c)
	    {
		reent->in_asp = 1;
		reent->asp_token_len = reent->asp_lex_pos - reent->asp_lex_cur -2;
		yyval->v_text.str = reent->asp_start + reent->asp_lex_cur;
		yyval->v_text.len = reent->asp_token_len;

		return GB_HTML_BLOCK;
	    }
	    else
		state = 0;
	}
	else
	{
	    if ('<' == c)
		state = 1;
	}
    }

    if (reent->asp_lex_pos > reent->asp_lex_cur )
    {
	reent->asp_token_len = reent->asp_lex_pos - reent->asp_lex_cur ;
	yyval->v_text.str = reent->asp_start + reent->asp_lex_cur;
	yyval->v_text.len = reent->asp_token_len;
	return GB_HTML_BLOCK;
    }
    else
	return 0;
}

