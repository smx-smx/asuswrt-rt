/*
 * Configuration file loader, lex header
 *
 * $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/linux-atm/lane/load_lex.h#1 $
 *
 */
#ifndef __LOAD_LEX
#define __LOAD_LEX

#include <stdio.h>

#include "lane.h"
#include "load.h"

#define UNIT 20
#define BOOLEAN 21
#define ATMADDRESS 22
#define STRING 23
#define INTEGER 24
#define LANEDEST 25
#define VARNAME 27
#define VCC 28
#define ERROR 26
#define END 0

typedef struct {
  Bool_t bool;
  int intti;
  AtmAddr_t *atmaddress;
  LaneDestination_t *destaddr;
  char *stringgi;
  LaneVcc_t vcc;
} Ret_t;

extern FILE *yyin;
extern int g_buf_index;
extern Ret_t g_return;

int yylex(void);
#endif
