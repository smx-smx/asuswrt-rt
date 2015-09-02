
%{
    #include <assert.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>

    #define YYLEX_PARAM yy_reent
    
    #define YYERROR_VERBOSE 1
    #include "mini-asp.h"
    #include "grammar.h"

    #define YYPARSE_PARAM __yy_reent
    #define yy_reent ((asp_reent*)__yy_reent)
    /*unuse because 
    static void constant_error (asp_text* text)
    {
	    static char buf[64];
	    buf[0] = ':';
	    buf[1] = '\'';
	
	    memcpy (buf+2, text->str, text->len);
	    sprintf (buf+text->len+2, "\' Unknown constant,");
	
	    yyerror(buf);
    }*/
  
%}

%pure_parser

%union {
    asp_text v_text;
    asp_stmt* v_stmt;
}

%token GB_IF GB_ELSE GB_ELSEIF GB_END GB_THEN GB_SCRIPT 

%token <v_text> GB_HTML_BLOCK GB_ID GB_STRING

%type <v_stmt> statement statements aspfile stmt stmt_call  stmt_cond val cond cond_list cond_else

%start aspfile
%%

aspfile:statements {$$=$1;yy_reent->root = $$;};


statements: statement {$$=$1;$$->last = $1;$$->next = NULL;}
| statements statement {$$ = $1; $$->last->next = $2; $$->last=$2; $2->next = NULL;}
;

statement:  GB_SCRIPT GB_ID '=' GB_STRING {$$ = new_stmt(yy_reent, AST_NULL);}
| GB_HTML_BLOCK {$$ = new_stmt(yy_reent,AST_HTML); $$->v_text.str = yy_reent->asp_start + yy_reent->asp_lex_cur; $$->v_text.len= yy_reent->asp_token_len;}
| stmt {$$=$1;}
| cond {$$=$1;}
;

stmt: stmt_call {$$ = $1;}
| val {$$=$1;}
;

//modify to support new tcWebApi
stmt_call: GB_ID '('')' { $$=new_stmt(yy_reent,AST_CALL);  $$->v_call.func = $1; }
| GB_ID '(' GB_STRING ')' { $$=new_stmt(yy_reent,AST_CALL);  $$->v_call.func = $1;$$->v_call.params[0] = $3; }
| GB_ID '(' GB_STRING ',' GB_STRING ')' { $$=new_stmt(yy_reent,AST_CALL);  $$->v_call.func = $1;$$->v_call.params[0] = $3;$$->v_call.params[1] = $5; }
| GB_ID '(' GB_STRING ',' GB_STRING ',' GB_STRING ')' { $$=new_stmt(yy_reent,AST_CALL);  $$->v_call.func = $1;$$->v_call.params[0] = $3;$$->v_call.params[1] = $5;$$->v_call.params[2] = $7; }
| GB_ID '(' GB_STRING ',' GB_STRING ',' GB_STRING ',' GB_STRING ')' { $$=new_stmt(yy_reent,AST_CALL);  $$->v_call.func = $1;$$->v_call.params[0] = $3;$$->v_call.params[1] = $5;$$->v_call.params[2] = $7;$$->v_call.params[3] = $9; }
/*old asp grammer
| GB_ID '(' GB_ID ')' { $$=new_stmt(yy_reent,AST_CALL_ID); $$->v_call.func = $1;if(lookup_asp_constant($3.str, $3.len, &($$->v_call.id))){constant_error(&$3); YYABORT;}}
| GB_ID '(' GB_ID ',' GB_STRING ')' { $$=new_stmt(yy_reent,AST_CALL_ID_1); $$->v_call.func = $1;$$->v_call.param = $5;if (lookup_asp_constant($3.str,$3.len, &($$->v_call.id))){constant_error(&$3);  YYABORT;}}*/
;

/*
stmt_call: GB_ID '(' params ')' {$$=$3;$$->v_call.func=$1;};

params: val {$$=new_stmt(yy_reent, AST_CALL); $$->v_call.count=1;$$->v_call.params[0] = $1->v_text;}
| params ',' val {$$=$1; $$->v_call.params[$$->v_call.count++] = $3->v_text; }
| {$$=new_stmt(yy_reent,AST_CALL); $$->v_call.count=0;}
;
*/

val: GB_STRING {$$=new_stmt(yy_reent, AST_STRING);$$->v_text = $1;}
| GB_ID {$$=new_stmt(yy_reent, AST_ID);$$->v_text = $1;}
;

cond: cond_list  GB_END GB_IF {$$=$1;}
| cond_list cond_else GB_END GB_IF {$$=$1; $$->v_cond.last->v_cond.next = $2; $$->v_cond.last = $2;}
;

cond_list: GB_IF stmt_cond GB_THEN statements {$$=$2;
     $$->v_cond.stmt_act = $4;}
| cond_list GB_ELSEIF stmt_cond GB_THEN statements {$$=$1; $3->v_cond.stmt_act = $5; 
     $$->v_cond.last->v_cond.next = $3; 
     $$->v_cond.last = $3;}
;

cond_else: GB_ELSE statements { $$=new_stmt(yy_reent,AST_COND); $$->v_cond.op = ACO_TRUE;$$->v_cond.stmt_act = $2;
     $$->v_cond.next = NULL; $$->v_cond.last = $$;}

stmt_cond: stmt '=' stmt {$$=new_stmt(yy_reent, AST_COND); $$->v_cond.op = ACO_EQU;
    $$->v_cond.stmt1 = $1; $$->v_cond.stmt2 = $3; $$->v_cond.next= NULL;$$->v_cond.last = $$;}
| stmt '<' '>' stmt { $$=new_stmt(yy_reent,AST_COND);$$->v_cond.op = ACO_NEQ;
    $$->v_cond.stmt1 = $1; $$->v_cond.stmt2 = $4;$$->v_cond.next= NULL;$$->v_cond.last = $$;}
;








