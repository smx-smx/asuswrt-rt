/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_INTERFACE = 258,
     T_PREFIX = 259,
     T_ROUTE = 260,
     T_RDNSS = 261,
     T_CLIENTS = 262,
     STRING = 263,
     NUMBER = 264,
     SIGNEDNUMBER = 265,
     DECIMAL = 266,
     SWITCH = 267,
     IPV6ADDR = 268,
     INFINITY = 269,
     T_IgnoreIfMissing = 270,
     T_AdvSendAdvert = 271,
     T_MaxRtrAdvInterval = 272,
     T_MinRtrAdvInterval = 273,
     T_MinDelayBetweenRAs = 274,
     T_AdvManagedFlag = 275,
     T_AdvOtherConfigFlag = 276,
     T_AdvLinkMTU = 277,
     T_AdvReachableTime = 278,
     T_AdvRetransTimer = 279,
     T_AdvCurHopLimit = 280,
     T_AdvDefaultLifetime = 281,
     T_AdvDefaultPreference = 282,
     T_AdvSourceLLAddress = 283,
     T_AdvOnLink = 284,
     T_AdvAutonomous = 285,
     T_AdvValidLifetime = 286,
     T_AdvPreferredLifetime = 287,
     T_AdvRouterAddr = 288,
     T_AdvHomeAgentFlag = 289,
     T_AdvIntervalOpt = 290,
     T_AdvHomeAgentInfo = 291,
     T_Base6to4Interface = 292,
     T_UnicastOnly = 293,
     T_HomeAgentPreference = 294,
     T_HomeAgentLifetime = 295,
     T_AdvRoutePreference = 296,
     T_AdvRouteLifetime = 297,
     T_AdvRDNSSPreference = 298,
     T_AdvRDNSSOpenFlag = 299,
     T_AdvRDNSSLifetime = 300,
     T_AdvMobRtrSupportFlag = 301,
     T_BAD_TOKEN = 302
   };
#endif
#define T_INTERFACE 258
#define T_PREFIX 259
#define T_ROUTE 260
#define T_RDNSS 261
#define T_CLIENTS 262
#define STRING 263
#define NUMBER 264
#define SIGNEDNUMBER 265
#define DECIMAL 266
#define SWITCH 267
#define IPV6ADDR 268
#define INFINITY 269
#define T_IgnoreIfMissing 270
#define T_AdvSendAdvert 271
#define T_MaxRtrAdvInterval 272
#define T_MinRtrAdvInterval 273
#define T_MinDelayBetweenRAs 274
#define T_AdvManagedFlag 275
#define T_AdvOtherConfigFlag 276
#define T_AdvLinkMTU 277
#define T_AdvReachableTime 278
#define T_AdvRetransTimer 279
#define T_AdvCurHopLimit 280
#define T_AdvDefaultLifetime 281
#define T_AdvDefaultPreference 282
#define T_AdvSourceLLAddress 283
#define T_AdvOnLink 284
#define T_AdvAutonomous 285
#define T_AdvValidLifetime 286
#define T_AdvPreferredLifetime 287
#define T_AdvRouterAddr 288
#define T_AdvHomeAgentFlag 289
#define T_AdvIntervalOpt 290
#define T_AdvHomeAgentInfo 291
#define T_Base6to4Interface 292
#define T_UnicastOnly 293
#define T_HomeAgentPreference 294
#define T_HomeAgentLifetime 295
#define T_AdvRoutePreference 296
#define T_AdvRouteLifetime 297
#define T_AdvRDNSSPreference 298
#define T_AdvRDNSSOpenFlag 299
#define T_AdvRDNSSLifetime 300
#define T_AdvMobRtrSupportFlag 301
#define T_BAD_TOKEN 302




/* Copy the first part of user declarations.  */
#line 16 "gram.y"

#include <config.h>
#include <includes.h>
#include <radvd.h>
#include <defaults.h>

extern struct Interface *IfaceList;
struct Interface *iface = NULL;
struct AdvPrefix *prefix = NULL;
struct AdvRoute *route = NULL;
struct AdvRDNSS *rdnss = NULL;

extern char *conf_file;
extern int num_lines;
extern char *yytext;
extern int sock;

static void cleanup(void);
static void yyerror(char *msg);

#if 0 /* no longer necessary? */
#ifndef HAVE_IN6_ADDR_S6_ADDR
# ifdef __FreeBSD__
#  define s6_addr32 __u6_addr.__u6_addr32
#  define s6_addr16 __u6_addr.__u6_addr16
# endif
#endif
#endif

#define ABORT	do { cleanup(); YYABORT; } while (0);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 112 "gram.y"
typedef union YYSTYPE {
	unsigned int		num;
	int			snum;
	double			dec;
	struct in6_addr		*addr;
	char			*str;
	struct AdvPrefix	*pinfo;
	struct AdvRoute		*rinfo;
	struct AdvRDNSS		*rdnssinfo;
	struct Clients		*ainfo;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 215 "gram.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 227 "gram.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   172

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  36
/* YYNRULES -- Number of rules. */
#define YYNRULES  83
/* YYNRULES -- Number of states. */
#define YYNSTATES  181

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   302

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    51,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    50,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,    49,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     6,     8,    14,    17,    19,    25,    26,
      28,    29,    31,    32,    34,    35,    37,    38,    40,    43,
      45,    49,    53,    57,    61,    65,    69,    73,    77,    81,
      85,    89,    93,    97,   101,   105,   109,   113,   117,   121,
     125,   129,   133,   137,   141,   147,   150,   154,   157,   163,
     168,   169,   171,   174,   176,   180,   184,   188,   192,   196,
     200,   203,   209,   214,   215,   217,   220,   222,   226,   230,
     233,   239,   242,   244,   246,   249,   250,   252,   255,   257,
     261,   265,   269,   271
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      53,     0,    -1,    53,    54,    -1,    54,    -1,    55,    48,
      57,    49,    50,    -1,     3,    56,    -1,     8,    -1,    58,
      59,    60,    61,    62,    -1,    -1,    63,    -1,    -1,    67,
      -1,    -1,    65,    -1,    -1,    73,    -1,    -1,    79,    -1,
      63,    64,    -1,    64,    -1,    18,     9,    50,    -1,    17,
       9,    50,    -1,    19,     9,    50,    -1,    18,    11,    50,
      -1,    17,    11,    50,    -1,    19,    11,    50,    -1,    15,
      12,    50,    -1,    16,    12,    50,    -1,    20,    12,    50,
      -1,    21,    12,    50,    -1,    22,     9,    50,    -1,    23,
       9,    50,    -1,    24,     9,    50,    -1,    26,     9,    50,
      -1,    27,    10,    50,    -1,    25,     9,    50,    -1,    28,
      12,    50,    -1,    35,    12,    50,    -1,    36,    12,    50,
      -1,    34,    12,    50,    -1,    39,     9,    50,    -1,    40,
       9,    50,    -1,    38,    12,    50,    -1,    46,    12,    50,
      -1,     7,    48,    66,    49,    50,    -1,    13,    50,    -1,
      66,    13,    50,    -1,    68,    59,    -1,    69,    48,    70,
      49,    50,    -1,     4,    13,    51,     9,    -1,    -1,    71,
      -1,    71,    72,    -1,    72,    -1,    29,    12,    50,    -1,
      30,    12,    50,    -1,    33,    12,    50,    -1,    31,    87,
      50,    -1,    32,    87,    50,    -1,    37,    56,    50,    -1,
      74,    61,    -1,    75,    48,    76,    49,    50,    -1,     5,
      13,    51,     9,    -1,    -1,    77,    -1,    77,    78,    -1,
      78,    -1,    41,    10,    50,    -1,    42,    87,    50,    -1,
      80,    62,    -1,    83,    48,    84,    49,    50,    -1,    81,
      82,    -1,    82,    -1,    13,    -1,     6,    81,    -1,    -1,
      85,    -1,    85,    86,    -1,    86,    -1,    43,     9,    50,
      -1,    44,    12,    50,    -1,    45,    87,    50,    -1,     9,
      -1,    14,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   126,   126,   127,   130,   176,   191,   198,   207,   208,
     212,   215,   219,   222,   226,   229,   233,   236,   239,   240,
     243,   247,   251,   255,   259,   263,   267,   271,   275,   279,
     283,   287,   291,   295,   299,   303,   307,   311,   315,   319,
     323,   327,   331,   335,   341,   347,   358,   373,   380,   411,
     474,   475,   478,   479,   482,   486,   490,   497,   501,   505,
     517,   524,   532,   556,   557,   560,   561,   565,   569,   575,
     582,   589,   590,   593,   628,   637,   638,   641,   642,   646,
     650,   654,   667,   671
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_INTERFACE", "T_PREFIX", "T_ROUTE",
  "T_RDNSS", "T_CLIENTS", "STRING", "NUMBER", "SIGNEDNUMBER", "DECIMAL",
  "SWITCH", "IPV6ADDR", "INFINITY", "T_IgnoreIfMissing", "T_AdvSendAdvert",
  "T_MaxRtrAdvInterval", "T_MinRtrAdvInterval", "T_MinDelayBetweenRAs",
  "T_AdvManagedFlag", "T_AdvOtherConfigFlag", "T_AdvLinkMTU",
  "T_AdvReachableTime", "T_AdvRetransTimer", "T_AdvCurHopLimit",
  "T_AdvDefaultLifetime", "T_AdvDefaultPreference", "T_AdvSourceLLAddress",
  "T_AdvOnLink", "T_AdvAutonomous", "T_AdvValidLifetime",
  "T_AdvPreferredLifetime", "T_AdvRouterAddr", "T_AdvHomeAgentFlag",
  "T_AdvIntervalOpt", "T_AdvHomeAgentInfo", "T_Base6to4Interface",
  "T_UnicastOnly", "T_HomeAgentPreference", "T_HomeAgentLifetime",
  "T_AdvRoutePreference", "T_AdvRouteLifetime", "T_AdvRDNSSPreference",
  "T_AdvRDNSSOpenFlag", "T_AdvRDNSSLifetime", "T_AdvMobRtrSupportFlag",
  "T_BAD_TOKEN", "'{'", "'}'", "';'", "'/'", "$accept", "grammar",
  "ifacedef", "ifacehead", "name", "ifaceparams", "optional_ifacevlist",
  "optional_prefixlist", "optional_clientslist", "optional_routelist",
  "optional_rdnsslist", "ifacevlist", "ifaceval", "clientslist",
  "v6addrlist", "prefixlist", "prefixdef", "prefixhead",
  "optional_prefixplist", "prefixplist", "prefixparms", "routelist",
  "routedef", "routehead", "optional_routeplist", "routeplist",
  "routeparms", "rdnsslist", "rdnssdef", "rdnssaddrs", "rdnssaddr",
  "rdnsshead", "optional_rdnssplist", "rdnssplist", "rdnssparms",
  "number_or_infinity", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   123,   125,
      59,    47
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    52,    53,    53,    54,    55,    56,    57,    58,    58,
      59,    59,    60,    60,    61,    61,    62,    62,    63,    63,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    65,    66,    66,    67,    68,    69,
      70,    70,    71,    71,    72,    72,    72,    72,    72,    72,
      73,    74,    75,    76,    76,    77,    77,    78,    78,    79,
      80,    81,    81,    82,    83,    84,    84,    85,    85,    86,
      86,    86,    87,    87
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     1,     5,     2,     1,     5,     0,     1,
       0,     1,     0,     1,     0,     1,     0,     1,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     5,     2,     3,     2,     5,     4,
       0,     1,     2,     1,     3,     3,     3,     3,     3,     3,
       2,     5,     4,     0,     1,     2,     1,     3,     3,     2,
       5,     2,     1,     1,     2,     0,     1,     2,     1,     3,
       3,     3,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     3,     0,     6,     5,     1,     2,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    10,     9,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    12,    11,    10,     0,    18,    26,    27,    21,    24,
      20,    23,    22,    25,    28,    29,    30,    31,    32,    35,
      33,    34,    36,    39,    37,    38,    42,    40,    41,    43,
       4,     0,     0,    14,    13,    47,    50,     0,     0,     0,
      16,    15,    14,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    53,    49,     0,     0,     0,     0,     7,    17,
      16,     0,    60,    63,     0,     0,    82,    83,     0,     0,
       0,     0,     0,    52,    45,     0,     0,     0,    73,    74,
      72,    69,    75,     0,     0,     0,    64,    66,    54,    55,
      57,    58,    56,    59,    48,    46,    44,    62,    71,     0,
       0,     0,     0,    76,    78,     0,     0,     0,    65,     0,
       0,     0,     0,    77,    67,    68,    61,    79,    80,    81,
      70
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     2,     3,     4,     6,    31,    32,    61,    93,   100,
     118,    33,    34,    94,   115,    62,    63,    64,   110,   111,
     112,   101,   102,   103,   145,   146,   147,   119,   120,   139,
     140,   121,   162,   163,   164,   128
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -108
static const yysigned_char yypact[] =
{
      -2,    28,    18,  -108,   -16,  -108,  -108,  -108,  -108,   -12,
      33,    38,     8,    24,    35,    39,    40,    44,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      58,    12,    67,   -12,  -108,     9,    22,    23,    25,    26,
      27,    29,    30,    31,    32,    34,    36,    37,    41,    42,
      43,    45,    56,    57,    61,    62,    63,    64,    65,    66,
      70,    71,  -108,    67,    14,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,    68,    69,    80,  -108,  -108,    10,    79,    76,    77,
      88,  -108,    80,    72,    84,    85,    11,    11,    86,    28,
      73,    10,  -108,  -108,    74,   -11,    75,    87,  -108,  -108,
      88,    81,  -108,     7,    78,    82,  -108,  -108,    83,    89,
      90,    91,    92,  -108,  -108,    93,    94,    95,  -108,    87,
    -108,  -108,   -14,    98,    11,    96,     7,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,   100,
     106,    11,    97,   -14,  -108,    99,   101,   102,  -108,   103,
     104,   105,   107,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
    -108,  -108,   108,  -108,   -35,  -108,  -108,   109,  -108,    -3,
     -19,  -108,   114,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
      -9,  -108,  -108,  -108,  -108,  -108,   -43,  -108,  -108,  -108,
     -34,  -108,  -108,  -108,   -42,  -107
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
     129,     1,   135,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    37,     7,    38,
     126,     1,    24,    25,    26,   127,    27,    28,    29,   159,
     160,   161,     9,    39,    30,    40,     5,   166,   136,   104,
     105,   106,   107,   108,    41,    35,    42,   109,   143,   144,
      36,    43,    44,    45,   171,    46,    47,    48,    49,    66,
      50,    59,    96,    51,    52,    53,    54,    55,    56,    57,
      58,    60,    67,    68,   131,    69,    70,    71,    92,    72,
      73,    74,    75,    91,    76,    99,    77,    78,   113,   114,
     116,    79,    80,    81,   117,    82,   124,   125,   130,   122,
     138,   141,   133,   168,   157,   158,    83,    84,   165,   169,
       8,    85,    86,    87,    88,    89,    90,    98,   170,    97,
     123,   173,   132,     0,   134,     0,   137,     0,   148,   142,
       0,     0,   149,   150,     0,     0,     0,     0,     0,   151,
     152,   153,   154,   155,   156,   167,   172,    65,     0,   174,
       0,   175,   176,   177,   178,   179,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95
};

static const short yycheck[] =
{
     107,     3,    13,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     9,     0,    11,
       9,     3,    34,    35,    36,    14,    38,    39,    40,    43,
      44,    45,    48,     9,    46,    11,     8,   144,    49,    29,
      30,    31,    32,    33,     9,    12,    11,    37,    41,    42,
      12,    12,    12,     9,   161,     9,     9,     9,     9,    50,
      10,    49,    48,    12,    12,    12,    12,    12,     9,     9,
      12,     4,    50,    50,   109,    50,    50,    50,     7,    50,
      50,    50,    50,    13,    50,     5,    50,    50,     9,    13,
      13,    50,    50,    50,     6,    50,    12,    12,    12,   102,
      13,   120,   111,   146,     9,   139,    50,    50,    10,     9,
       2,    50,    50,    50,    50,    50,    50,    48,    12,    51,
      48,   163,    49,    -1,    50,    -1,    51,    -1,    50,    48,
      -1,    -1,    50,    50,    -1,    -1,    -1,    -1,    -1,    50,
      50,    50,    50,    50,    50,    49,    49,    33,    -1,    50,
      -1,    50,    50,    50,    50,    50,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,    53,    54,    55,     8,    56,     0,    54,    48,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    34,    35,    36,    38,    39,    40,
      46,    57,    58,    63,    64,    12,    12,     9,    11,     9,
      11,     9,    11,    12,    12,     9,     9,     9,     9,     9,
      10,    12,    12,    12,    12,    12,     9,     9,    12,    49,
       4,    59,    67,    68,    69,    64,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    13,     7,    60,    65,    59,    48,    51,    48,     5,
      61,    73,    74,    75,    29,    30,    31,    32,    33,    37,
      70,    71,    72,     9,    13,    66,    13,     6,    62,    79,
      80,    83,    61,    48,    12,    12,     9,    14,    87,    87,
      12,    56,    49,    72,    50,    13,    49,    51,    13,    81,
      82,    62,    48,    41,    42,    76,    77,    78,    50,    50,
      50,    50,    50,    50,    50,    50,    50,     9,    82,    43,
      44,    45,    84,    85,    86,    10,    87,    49,    78,     9,
      12,    87,    49,    86,    50,    50,    50,    50,    50,    50,
      50
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 131 "gram.y"
    {
			struct Interface *iface2;

			iface2 = IfaceList;
			while (iface2)
			{
				if (!strcmp(iface2->Name, iface->Name))
				{
					flog(LOG_ERR, "duplicate interface "
						"definition for %s", iface->Name);
					ABORT;
				}
				iface2 = iface2->next;
			}			

			if (check_device(sock, iface) < 0) {
				if (iface->IgnoreIfMissing) {
					dlog(LOG_DEBUG, 4, "interface %s did not exist, ignoring the interface", iface->Name);
				}
				else {
					flog(LOG_ERR, "interface %s does not exist", iface->Name);
					ABORT;
				}
			}
			if (setup_deviceinfo(sock, iface) < 0)
				if (!iface->IgnoreIfMissing)
				ABORT;
			if (check_iface(iface) < 0)
				if (!iface->IgnoreIfMissing)
				ABORT;
			if (setup_linklocal_addr(sock, iface) < 0)
				if (!iface->IgnoreIfMissing)
				ABORT;
			if (setup_allrouters_membership(sock, iface) < 0)
				if (!iface->IgnoreIfMissing)
				ABORT;

			dlog(LOG_DEBUG, 4, "interface definition for %s is ok", iface->Name);

			iface->next = IfaceList;
			IfaceList = iface;

			iface = NULL;
		}
    break;

  case 5:
#line 177 "gram.y"
    {
			iface = malloc(sizeof(struct Interface));

			if (iface == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			iface_init_defaults(iface);
			strncpy(iface->Name, yyvsp[0].str, IFNAMSIZ-1);
			iface->Name[IFNAMSIZ-1] = '\0';
		}
    break;

  case 6:
#line 192 "gram.y"
    {
			/* check vality */
			yyval.str = yyvsp[0].str;
		}
    break;

  case 7:
#line 199 "gram.y"
    {
			iface->AdvPrefixList = yyvsp[-3].pinfo;
			iface->ClientList = yyvsp[-2].ainfo;
			iface->AdvRouteList = yyvsp[-1].rinfo;
			iface->AdvRDNSSList = yyvsp[0].rdnssinfo;
		}
    break;

  case 10:
#line 212 "gram.y"
    {
			yyval.pinfo = NULL;
		}
    break;

  case 12:
#line 219 "gram.y"
    {
			yyval.ainfo = NULL;
		}
    break;

  case 14:
#line 226 "gram.y"
    {
			yyval.rinfo = NULL;
		}
    break;

  case 16:
#line 233 "gram.y"
    {
			yyval.rdnssinfo = NULL;
		}
    break;

  case 20:
#line 244 "gram.y"
    {
			iface->MinRtrAdvInterval = yyvsp[-1].num;
		}
    break;

  case 21:
#line 248 "gram.y"
    {
			iface->MaxRtrAdvInterval = yyvsp[-1].num;
		}
    break;

  case 22:
#line 252 "gram.y"
    {
			iface->MinDelayBetweenRAs = yyvsp[-1].num;
		}
    break;

  case 23:
#line 256 "gram.y"
    {
			iface->MinRtrAdvInterval = yyvsp[-1].dec;
		}
    break;

  case 24:
#line 260 "gram.y"
    {
			iface->MaxRtrAdvInterval = yyvsp[-1].dec;
		}
    break;

  case 25:
#line 264 "gram.y"
    {
			iface->MinDelayBetweenRAs = yyvsp[-1].dec;
		}
    break;

  case 26:
#line 268 "gram.y"
    {
			iface->IgnoreIfMissing = yyvsp[-1].num;
		}
    break;

  case 27:
#line 272 "gram.y"
    {
			iface->AdvSendAdvert = yyvsp[-1].num;
		}
    break;

  case 28:
#line 276 "gram.y"
    {
			iface->AdvManagedFlag = yyvsp[-1].num;
		}
    break;

  case 29:
#line 280 "gram.y"
    {
			iface->AdvOtherConfigFlag = yyvsp[-1].num;
		}
    break;

  case 30:
#line 284 "gram.y"
    {
			iface->AdvLinkMTU = yyvsp[-1].num;
		}
    break;

  case 31:
#line 288 "gram.y"
    {
			iface->AdvReachableTime = yyvsp[-1].num;
		}
    break;

  case 32:
#line 292 "gram.y"
    {
			iface->AdvRetransTimer = yyvsp[-1].num;
		}
    break;

  case 33:
#line 296 "gram.y"
    {
			iface->AdvDefaultLifetime = yyvsp[-1].num;
		}
    break;

  case 34:
#line 300 "gram.y"
    {
			iface->AdvDefaultPreference = yyvsp[-1].snum;
		}
    break;

  case 35:
#line 304 "gram.y"
    {
			iface->AdvCurHopLimit = yyvsp[-1].num;
		}
    break;

  case 36:
#line 308 "gram.y"
    {
			iface->AdvSourceLLAddress = yyvsp[-1].num;
		}
    break;

  case 37:
#line 312 "gram.y"
    {
			iface->AdvIntervalOpt = yyvsp[-1].num;
		}
    break;

  case 38:
#line 316 "gram.y"
    {
			iface->AdvHomeAgentInfo = yyvsp[-1].num;
		}
    break;

  case 39:
#line 320 "gram.y"
    {
			iface->AdvHomeAgentFlag = yyvsp[-1].num;
		}
    break;

  case 40:
#line 324 "gram.y"
    {
			iface->HomeAgentPreference = yyvsp[-1].num;
		}
    break;

  case 41:
#line 328 "gram.y"
    {
			iface->HomeAgentLifetime = yyvsp[-1].num;
		}
    break;

  case 42:
#line 332 "gram.y"
    {
			iface->UnicastOnly = yyvsp[-1].num;
		}
    break;

  case 43:
#line 336 "gram.y"
    {
			iface->AdvMobRtrSupportFlag = yyvsp[-1].num;
		}
    break;

  case 44:
#line 342 "gram.y"
    {
			yyval.ainfo = yyvsp[-2].ainfo;
		}
    break;

  case 45:
#line 348 "gram.y"
    {
			struct Clients *new = calloc(1, sizeof(struct Clients));
			if (new == NULL) {
				flog(LOG_CRIT, "calloc failed: %s", strerror(errno));
				ABORT;
			}

			memcpy(&(new->Address), yyvsp[-1].addr, sizeof(struct in6_addr));
			yyval.ainfo = new;
		}
    break;

  case 46:
#line 359 "gram.y"
    {
			struct Clients *new = calloc(1, sizeof(struct Clients));
			if (new == NULL) {
				flog(LOG_CRIT, "calloc failed: %s", strerror(errno));
				ABORT;
			}

			memcpy(&(new->Address), yyvsp[-1].addr, sizeof(struct in6_addr));
			new->next = yyvsp[-2].ainfo;
			yyval.ainfo = new;
		}
    break;

  case 47:
#line 374 "gram.y"
    {
			yyvsp[-1].pinfo->next = yyvsp[0].pinfo;
			yyval.pinfo = yyvsp[-1].pinfo;
		}
    break;

  case 48:
#line 381 "gram.y"
    {
			unsigned int dst;

			if (prefix->AdvPreferredLifetime >
			    prefix->AdvValidLifetime)
			{
				flog(LOG_ERR, "AdvValidLifeTime must be "
					"greater than AdvPreferredLifetime in %s, line %d", 
					conf_file, num_lines);
				ABORT;
			}

			if( prefix->if6to4[0] )
			{
				if (get_v4addr(prefix->if6to4, &dst) < 0)
				{
					flog(LOG_ERR, "interface %s has no IPv4 addresses, disabling 6to4 prefix", prefix->if6to4 );
					prefix->enabled = 0;
				} else
				{
					*((uint16_t *)(prefix->Prefix.s6_addr)) = htons(0x2002);
					memcpy( prefix->Prefix.s6_addr + 2, &dst, sizeof( dst ) );
				}
			}

			yyval.pinfo = prefix;
			prefix = NULL;
		}
    break;

  case 49:
#line 412 "gram.y"
    {
			struct in6_addr zeroaddr;
			prefix = malloc(sizeof(struct AdvPrefix));
			
			if (prefix == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			prefix_init_defaults(prefix);

			if (yyvsp[0].num > MAX_PrefixLen)
			{
				flog(LOG_ERR, "invalid prefix length in %s, line %d", conf_file, num_lines);
				ABORT;
			}

			prefix->PrefixLen = yyvsp[0].num;

			memcpy(&prefix->Prefix, yyvsp[-2].addr, sizeof(struct in6_addr));

			memset(&zeroaddr, 0, sizeof(zeroaddr));
			if (!memcmp(yyvsp[-2].addr, &zeroaddr, sizeof(struct in6_addr))) {
#ifndef HAVE_IFADDRS_H
				flog(LOG_ERR, "invalid all-zeros prefix in %s, line %d", conf_file, num_lines);
				ABORT;
#else
				dlog(LOG_DEBUG, 5, "all-zeros prefix in %s, line %d, parsing..", conf_file, num_lines);
				struct ifaddrs *ifap, *ifa;
				if (getifaddrs(&ifap) != 0)
					flog(LOG_ERR, "getifaddrs failed: %s", strerror(errno));
			        for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
				        struct sockaddr_in6 *s6;
					char buf[INET6_ADDRSTRLEN];
					if (strncmp(ifa->ifa_name, iface->Name, IFNAMSIZ))
						continue;
                			if (ifa->ifa_addr->sa_family != AF_INET6)
			                        continue;
					s6 = (struct sockaddr_in6 *)(ifa->ifa_addr);
	                		if (IN6_IS_ADDR_LINKLOCAL(&s6->sin6_addr))
						continue;
					if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s6->sin6_addr), buf, sizeof(buf)) == NULL) {
						flog(LOG_ERR, "%s: inet_ntop failed in %s, line %d!", ifa->ifa_name, conf_file, num_lines);
					}
					else {
						dlog(LOG_DEBUG, 5, "auto-selected prefix %s on interface %s", buf, ifa->ifa_name);
						memcpy(&prefix->Prefix, &s6->sin6_addr, sizeof(struct in6_addr));
						prefix->AdvRouterAddr=1;
						prefix->AutoSelected=1;
					}
				}
				if (!memcmp(&prefix->Prefix, &zeroaddr, sizeof(struct in6_addr))) {
					prefix->enabled = 0;
					flog(LOG_WARNING, "no auto-selected prefix on interface %s, disabling advertisements",  iface->Name);
				}
				freeifaddrs(ifap);
				freeifaddrs(ifa);
#endif /* ifndef HAVE_IFADDRS_H */
			}
		}
    break;

  case 54:
#line 483 "gram.y"
    {
			prefix->AdvOnLinkFlag = yyvsp[-1].num;
		}
    break;

  case 55:
#line 487 "gram.y"
    {
			prefix->AdvAutonomousFlag = yyvsp[-1].num;
		}
    break;

  case 56:
#line 491 "gram.y"
    {
			if (prefix->AutoSelected && yyvsp[-1].num == 0)
				flog(LOG_WARNING, "prefix automatically selected, AdvRouterAddr always enabled, ignoring config line %d", num_lines);
			else  
				prefix->AdvRouterAddr = yyvsp[-1].num;
		}
    break;

  case 57:
#line 498 "gram.y"
    {
			prefix->AdvValidLifetime = yyvsp[-1].num;
		}
    break;

  case 58:
#line 502 "gram.y"
    {
			prefix->AdvPreferredLifetime = yyvsp[-1].num;
		}
    break;

  case 59:
#line 506 "gram.y"
    {
			if (prefix->AutoSelected) {
				flog(LOG_ERR, "automatically selecting the prefix and Base6to4Interface are mutually exclusive");
				ABORT;
			} /* fallthrough */
			dlog(LOG_DEBUG, 4, "using interface %s for 6to4", yyvsp[-1].str);
			strncpy(prefix->if6to4, yyvsp[-1].str, IFNAMSIZ-1);
			prefix->if6to4[IFNAMSIZ-1] = '\0';
		}
    break;

  case 60:
#line 518 "gram.y"
    {
			yyvsp[-1].rinfo->next = yyvsp[0].rinfo;
			yyval.rinfo = yyvsp[-1].rinfo;
		}
    break;

  case 61:
#line 525 "gram.y"
    {
			yyval.rinfo = route;
			route = NULL;
		}
    break;

  case 62:
#line 533 "gram.y"
    {
			route = malloc(sizeof(struct AdvRoute));
			
			if (route == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			route_init_defaults(route, iface);

			if (yyvsp[0].num > MAX_PrefixLen)
			{
				flog(LOG_ERR, "invalid route prefix length in %s, line %d", conf_file, num_lines);
				ABORT;
			}

			route->PrefixLen = yyvsp[0].num;

			memcpy(&route->Prefix, yyvsp[-2].addr, sizeof(struct in6_addr));
		}
    break;

  case 67:
#line 566 "gram.y"
    {
			route->AdvRoutePreference = yyvsp[-1].snum;
		}
    break;

  case 68:
#line 570 "gram.y"
    {
			route->AdvRouteLifetime = yyvsp[-1].num;
		}
    break;

  case 69:
#line 576 "gram.y"
    {
			yyvsp[-1].rdnssinfo->next = yyvsp[0].rdnssinfo;
			yyval.rdnssinfo = yyvsp[-1].rdnssinfo;
		}
    break;

  case 70:
#line 583 "gram.y"
    {
			yyval.rdnssinfo = rdnss;
			rdnss = NULL;
		}
    break;

  case 73:
#line 594 "gram.y"
    {
			if (!rdnss) {
				/* first IP found */
				rdnss = malloc(sizeof(struct AdvRDNSS));
				
				if (rdnss == NULL) {
					flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
					ABORT;
				}

				rdnss_init_defaults(rdnss, iface);
			}
			
			switch (rdnss->AdvRDNSSNumber) {
				case 0:
					memcpy(&rdnss->AdvRDNSSAddr1, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				case 1:
					memcpy(&rdnss->AdvRDNSSAddr2, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				case 2:
					memcpy(&rdnss->AdvRDNSSAddr3, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				default:
					flog(LOG_CRIT, "Too many addresses in RDNSS section");
					ABORT;
			}
			
		}
    break;

  case 74:
#line 629 "gram.y"
    {
			if (!rdnss) {
				flog(LOG_CRIT, "No address specified in RDNSS section");
				ABORT;
			}
		}
    break;

  case 79:
#line 647 "gram.y"
    {
			rdnss->AdvRDNSSPreference = yyvsp[-1].num;
		}
    break;

  case 80:
#line 651 "gram.y"
    {
			rdnss->AdvRDNSSOpenFlag = yyvsp[-1].num;
		}
    break;

  case 81:
#line 655 "gram.y"
    {
			if (yyvsp[-1].num < iface->MaxRtrAdvInterval && yyvsp[-1].num != 0) {
				flog(LOG_ERR, "AdvRDNSSLifetime must be at least MaxRtrAdvInterval");
				ABORT;
			}
			if (yyvsp[-1].num > 2*(iface->MaxRtrAdvInterval))
				flog(LOG_WARNING, "Warning: AdvRDNSSLifetime <= 2*MaxRtrAdvInterval would allow stale DNS servers to be deleted faster");

			rdnss->AdvRDNSSLifetime = yyvsp[-1].num;
		}
    break;

  case 82:
#line 668 "gram.y"
    {
                                yyval.num = yyvsp[0].num; 
                        }
    break;

  case 83:
#line 672 "gram.y"
    {
                                yyval.num = (uint32_t)~0;
                        }
    break;


    }

/* Line 1000 of yacc.c.  */
#line 1906 "gram.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 677 "gram.y"


static
void cleanup(void)
{
	if (iface)
		free(iface);
	
	if (prefix)
		free(prefix);

	if (route)
		free(route);

	if (rdnss)
		free(rdnss);
}

static void
yyerror(char *msg)
{
	cleanup();
	flog(LOG_ERR, "%s in %s, line %d: %s", msg, conf_file, num_lines, yytext);
}

