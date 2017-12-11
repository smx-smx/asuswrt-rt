#ifdef TCSUPPORT_GENERAL_MULTILANGUAGE
#ifndef _PARSERS_H_
#define _PARSERS_H_
#define	NULLKEY		0
#define	SUCCESS		1
#define	FAILURE			0
#define	DUPLICATE		-1
#define	MAX_KEY			64
#define	MAX_VALUE		24576 /* Ren: 580 -> 1964, Kengi: 1964 -> 24576 (for Parental Control: MULTIFILTER_MACFILTER_DAYTIME) */
#define	MAX_STREAM	MAX_KEY+MAX_VALUE+4
#define	PATH_LENGTH	32
#define	STRING_PATH	"/userfs/%s.dict" /* Paul modify 2013/2/7 */
#ifdef RTCONFIG_ODMPID
#define	ODMPID_PATH	"/tmp/odm.dict"
#endif

typedef unsigned int	uint;

typedef struct _HASH{
	uint	key;
	uint	ioffset;
}HASH;

typedef struct _SParser{
	HASH	*ele;
	uint	count;
	uint	sizeindex;
}SParser;

#endif
#endif
