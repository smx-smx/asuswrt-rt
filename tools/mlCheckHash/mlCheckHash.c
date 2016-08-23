#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	NULLKEY		0
#define	SUCCESS		1
#define	FAILURE			0
#define	DUPLICATE		-1
#define	MAX_KEY			64
#define	MAX_VALUE		1964
#define	MAX_STREAM	MAX_KEY+MAX_VALUE+4
#define	PATH_LENGTH	32

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

/*********internal variables************/
static	int	hashsize[] = {2531, 2743, 3137, 3529};//array of prime numbers
static	int	m;
static	SParser	s_parse;
static	char	str_attrname[MAX_KEY] = {0};
static	int	init_flag = 0;
/************internal method************/
static	uint	hash(uint k);
static	void	collision(uint *p, uint d);
static	void	reCreateHashTable(SParser *H);
static	void	ResetHashTable(SParser *H);
static	uint	getkey(const char *ckey);
static	int	Find(SParser *H, uint key, uint *p, uint *c);
static	int	insertToHash(SParser *H, HASH elem);
/************external method as below**********/
int	initandparserfile(char *stringPath);
void		initParseStruct(void);//initializtion Parser struct;

static	uint	hash(uint k){
	return k%m;
}

static	void	collision(uint *p, uint d){
	*p = (*p + d)%m;
}

static	int	initHashTable(SParser *H)
{
	int i=0;
	(*H).count = 0;
	(*H).sizeindex = 0;
	m = hashsize[(*H).sizeindex];
	(*H).ele = (HASH *)malloc(m*sizeof(HASH));
	if(!(*H).ele){
		return FAILURE;
	}
	for (i=0; i<m; i++){
		(*H).ele[i].key = NULLKEY;
		(*H).ele[i].ioffset = NULLKEY;
	}
	return SUCCESS;
}
static	void	reCreateHashTable(SParser *H)
{
	printf("need recreate memory\r\n");
	HASH	*p = NULL;
	HASH	*pold = (HASH *)malloc(m*sizeof(HASH));
	int	i = 0;
	int	count = (*H).count;
	
	if (!pold) 
		return;
	p = pold;
	for (; i<m; i++)
	{
		if((*H).ele[i].key != NULLKEY)
			*p++ = (*H).ele[i];
	}
	(*H).sizeindex++;
	(*H).count = 0;
	m = hashsize[(*H).sizeindex];
	p = (HASH *)realloc((*H).ele, m*sizeof(HASH));
	if (p == NULL)
	{
		printf("realloc memory failure\r\n");
		free(pold);
		return;
	}
	(*H).ele = p;
	for (i=0; i<m; i++)
	{
		(*H).ele[i].key = NULLKEY;
		(*H).ele[i].ioffset = NULLKEY;
	}
	
	for(p=pold; p<(pold+count); p++)
	{
		insertToHash(H, *p);
	}
	free(pold);
}
static	void	ResetHashTable(SParser *H)
{
	if (NULL != (*H).ele)
		free((*H).ele);
	(*H).ele = NULL;
	(*H).count = 0;
	(*H).sizeindex = 0;
}
static	uint	getkey(const char *ckey)
{
	uint	sum = 0;
	uint	seek = 131;
	
	while(*ckey){
		sum = sum*seek + (*ckey++);
	}
	return (sum & 0X7FFFFFFFL);
	
}
static	int	Find(SParser *H, uint key, uint *p, uint *c)
{
	*p = hash(key);
	while(((*H).ele[*p].key != NULLKEY) && ((*H).ele[*p].key != key))
	{
		(*c)++;
		if ((*c)<m)
			collision(p, *c);
		else
			return FAILURE;
	}
	if ((*H).ele[*p].key == key)
	{
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}
}

static	int	insertToHash(SParser *H, HASH elem)
{
	int p,c=0;
	
	if (SUCCESS == Find(H, elem.key, &p, &c))
	{
		printf("!!!Error:%s duplicate, please check!\r\n",str_attrname);
		system("echo 1 > error");
		return DUPLICATE;
	}
	else if(c < (m/2)){
		(*H).ele[p] = elem;
		(*H).count++;
		return SUCCESS;
	}
	else{
		//wait to recreatehash table;
		reCreateHashTable(H);
		return FAILURE;
	}
}

int	initandparserfile(char *stringPath)
{
	FILE	*fp= NULL;
	char *pk = NULL;
	char stream[MAX_STREAM] = {0};
	char str_key[MAX_KEY] = {0};
	char	str_type[4] = {0};
	uint	offset = 0;
	HASH	s_hash;
	SParser	s_parse_tmp;

	printf("STRING_PATH=%s\n",stringPath);
	fp=fopen(stringPath,"r");

	if(fp == NULL)
	{
		printf("open STRING_PATH failed!\n");
		return FAILURE;
	}
	
	if (FAILURE == initHashTable(&s_parse_tmp)) {
		printf("initHashTable failed!\n");
		fclose(fp);
		return FAILURE;
	}
	ResetHashTable(&s_parse);//free memory at first
	s_parse = s_parse_tmp;
	
	fseek(fp, 0, SEEK_SET);
	while(fgets(stream, MAX_STREAM, fp) != NULL)
	{
		int	len = strlen(stream);
		pk = strstr(stream,"=");
		if ((stream[0] == ';') || (NULL == pk)) {
			offset += len;
			continue;
		}
		
		*pk = '\0';
		
		strncpy(str_key, stream, strlen(stream));
		s_hash.key = getkey(str_key);
		s_hash.ioffset = offset;
		offset += len;
		
		strcpy(str_attrname, str_key);
		insertToHash(&s_parse, s_hash);
		
		memset(stream, 0, MAX_STREAM);
		memset(str_key, 0, MAX_KEY);
	}
	fclose(fp);
	return SUCCESS;
}

void	initParseStruct(void)
{
	s_parse.ele = NULL;
	s_parse.count = 0;
	s_parse.sizeindex = 0;
}

int	main(int argc, char *argv[])
{	
	system("rm -f error");
	initParseStruct();
	initandparserfile(argv[1]);
	return 0;
}

