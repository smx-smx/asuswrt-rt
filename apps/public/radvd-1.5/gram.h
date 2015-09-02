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
/* Line 1275 of yacc.c.  */
#line 143 "gram.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



