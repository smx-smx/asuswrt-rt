
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TMP_FILE_DIR "swap_file"
#define MAX_MACRO_NUM   8
#define REMOVE_ALL       0
#define REMOVE_DEF       1
#define REMOVE_TOT       1

#define ERROR_STRING	-2
#define MATCH_PLAIN_CODE    -1
#define MATCH_ENDIF	0
#define MATCH_IF_FOUND      1
#define MATCH_IF_NOTFOUND   2
#define MATCH_ELSE	    3


#define IFDEF	1
#define ELSE	0

//#define DEBUG
/*CPL_OPT !DDD OVER*/
/*CPL_OPT CMD_POO||CCDD OVER*/

/*END CPL_OPT*/
/*END CPL_OPT*/

int removeDef(char *string, int len)
{
	char tmp[512] = {0};
	char *ptr;
	char *ptrTmp = tmp;

	if(string[0] == '!')
		*ptrTmp++ = '!';

	ptr = strstr(string,"(");
	ptr++;
	while(*ptr == ' ')
	{
		ptr++;
	}

	do
	{
		*ptrTmp = *ptr;
		ptr++; ptrTmp++;
	}while(  (*ptr != ')') && (*ptr != ' ') );
	
	memset(string, 0, len);
	strcpy(string, tmp);
#ifdef DEBUG
	
#endif
	return 0;
}


int getOptionFromConfigFile(char* opt,char* configFile)
{
	FILE *fp =NULL;
	char line[512] = {0};
	int rev = 0;

	fp = fopen(configFile,"r");
	 if(opt[0] == '!')
        {
                rev = 1;
                opt++;
        }
	//printf("configFile:%s\n",configFile);
#ifdef DEBUG
	printf("rev:%d, macro:%s\n",rev,opt);
#endif
	if(fp == NULL)
	{
		printf("error opening configure file!\n");
		return -1;
	}

	while(fgets (line, sizeof(line), fp) )
	{
		  if(strstr(line,opt) && strstr(line,"y"))
		  {
		  	fclose(fp);
//			printf("SUCESS\n");
			if(rev == 0)
				return 1;
			else
				return 0;
		  }
		memset(line, 0, sizeof(line));
	}

	fclose(fp);
//	printf("not found\n");
	if(rev == 0)
		return 0;
	else
		return 1;
}


int parseMacros(char *tmpBuf, char* configFile)
{
      
	char symb[512] = {0};
	char tmp[5];

	char macro[4][512] = {0};;
	int count = 0, ret;
	char cmd[512] = {0};
	int i;
	int ret_status;
	int andFlag = 0;
	int nfFlag = 0;

//modify macros to 8, tell it in SOP	
	ret  = sscanf(tmpBuf, "%s %s %s %s %s %s %s %s",  \
		symb,macro[0],tmp,macro[1],tmp,macro[2],tmp,macro[3]);
	//printf("parseMacros: enter parseMacro:%s,%s\n",symb,macro);

	if(strcmp(tmp,"&&") == 0)
	{
		andFlag = 1;
	}

	/* case match #endif */
	if( 0 == strcmp(symb, "#endif/*TCSUPPORT_COMPILE*/") )
		return MATCH_ENDIF;

	if( 0 == strcmp(symb, "#else/*TCSUPPORT_COMPILE*/") )
                return MATCH_ELSE;

	if( 0 != strcmp(symb, "#if/*TCSUPPORT_COMPILE*/") )
	{
		/* not match any case, plain letter */
		return MATCH_PLAIN_CODE;
	}
#if 0
	for(i = 0; i < ret/2; i++)
	{
		if( NULL == strstr(macro[i],"TCSUPPORT_COMPILE")  ||
			NULL == strstr(macro[i],"defined")
		  )
		{
			/* not match any case, plain code */
			return MATCH_PLAIN_CODE;
		}
		
	}
#endif

#ifdef DEBUG
	printf("andFlag: %d\n", andFlag);
#endif
	for(i = 0; i < ret/2; i++)
	{
		removeDef(macro[i],sizeof(macro[i]));


		ret_status = getOptionFromConfigFile(macro[i],configFile);
		if(ret_status == -1)
		{
			printf("error in Config file\n");
			return ERROR_STRING;
		}
		else if(ret_status)
		{
			/* match one or more macro*/
#ifdef DEBUG
			printf("match if found!\n");
#endif
			if(andFlag == 0)
			{
#ifdef DEBUG
 	                       printf("In or flag: returned if found\n");
#endif

				return MATCH_IF_FOUND;
			}
		}
		else
		{
			if(andFlag == 1)
			{
#ifdef DEBUG
				printf("In and flag: if not found\n");
#endif
				nfFlag = 1;
			}
		}
	}

#ifdef DEBUG
	printf("andFlag:%d\n", andFlag);
#endif
	/* case of #if, not match any case */
	if(andFlag == 1)
	{
		if(nfFlag == 0)
		{
#ifdef DEBUG
			printf("In and flag: returned if found\n");
#endif
			return MATCH_IF_FOUND;
		}
		else
		{
#ifdef DEBUG
                        printf("In and flag: returned if not found\n");
#endif
			return MATCH_IF_NOTFOUND;
		}
	}
#ifdef DEBUG
                        printf("In or flag: returned if not found\n");
#endif
	return MATCH_IF_NOTFOUND;
}


int convertInitialString(char *tmpLine,char *tmpBuf, int flag /*IFDEF*/)
{	
	char *ptr; // strstr(tmpBuf,"/*TCSUPPORT_COMPILE*/");
	char macro[4][512] ={ {0},{0},{0},{0} };
	char symb[128] = {0};
	char tmp[128] = {0};
	int i, ret, len;
	int flagOr = 1;


//	if(IFDEF == flag)
	{
		ptr = strstr(tmpBuf,"#if/*TCSUPPORT_COMPILE*/");
		ptr += sizeof("#if/*TCSUPPORT_COMPILE*/");
		
	}
//	else
//	{
//	 	ptr = strstr(tmpBuf,"#if/*TCSUPPORT_COMPILE*/");
  //              ptr += sizeof("#/*TCSUPPORT_COMPILE*/");
//	}
//	printf("before ptr is:%s\n", ptr);
///	printf("000\n");;
//	ptr += sizeof("#/*TCSUPPORT_COMPILE*/");
//	printf("after ptr is:%s\n", ptr);
	ret  = sscanf(tmpBuf, "%s %s %s %s %s %s %s %s",  \
                symb,macro[0],tmp,macro[1],tmp,macro[2],tmp,macro[3]);

	if(strcmp(tmp,"&&") == 0 )
		flagOr = 0;
//	printf("111");
	for(i = 0; i < ret/2; i++)
	{
		removeDef(macro[i],sizeof(macro[i]));
#ifdef DEBUG
		printf("macro[%d]:%s\n", i, macro[i]);
#endif
	}
//	printf("222");
	while(*ptr == ' ')
		ptr++;

//	printf("333");

	if(flag == IFDEF)
	{
		sprintf(tmpLine, "#if %s", ptr);
#ifdef DEBUG
		printf("tmpline:%s\n",tmpLine);		
#endif
	}

	else if(flag == ELSE)
	{
		sprintf(tmpLine,"#if ");
		for(i = 0; i < ret/2; i++)
		{
			len = strlen(tmpLine);

			if(macro[i][0] == '!')
				sprintf(tmpLine + len, "defined(%s) ", macro[i]+1);
			else
				sprintf(tmpLine + len, "!defined(%s) ", macro[i]);

			if( i != ret/2 - 1 )
			{
				len = strlen(tmpLine);
				if(flagOr == 1)
					sprintf(tmpLine + len, "&& ");
				else
					sprintf(tmpLine + len, "|| ");
			}
		}
			
		len = strlen(tmpLine);

		//sprintf(tmpLine + len, "\n");
#ifdef DEBUG
		printf("tmpline:%s\n", tmpLine);
#endif
	}

	return 0;
}



int parseOptionsFromConfigFile(char *fileName, char *configFile)
{
	
	
	char tmpBuf[512] = {0};
	FILE *fp= NULL;
	FILE *fpTmp = NULL;

	char opts[4][512] = { {0},{0},{0},{0} } ;
	int operand[128];


	int layerStack[8] ;
	int layer = 0;
	int matchFlag[8] = {0};
	int endifStack[8] = {0};

	char cmd[512] = {0};
		
	char tmpLine[512] = {0};
	char ifStack[8][512];

	memset(opts, 0, sizeof(opts));
	//memset(&operand, 0, sizeof(operand));
	//strcpy(tmpFileName, options)
	
	//printf("1\n");
	fp = fopen(fileName,"r");
	if( fp  == NULL)
	{
		printf("error in %s\n", fileName);
		return -1;
	}

//	printf("111\n");
	fpTmp = fopen(TMP_FILE_DIR, "w");
	if(fpTmp == NULL)
	{
		fclose(fp);
		printf("error in opening TMP_FILE_DIR for writing !\n");
		return -1;
	}

//	printf("2\n");
	while(fgets (tmpBuf, sizeof(tmpBuf), fp)  )
	{

	//	printf("3\n");
		int ret;

		
		
//		if(layer !=0 )
//		{
//			if(layerStack[layer-1] == REMOVE_ALL)
//				layerStack[layer++] = REMOVE_ALL;
//		}

		ret = parseMacros(tmpBuf, configFile);
		if(ret == ERROR_STRING)
		{
			printf("error occur in parse Macros\n");
			return -1;
		}
			

	    //case when #ifdef is found, and match
		if ( MATCH_IF_FOUND == ret )
		{
#ifdef DEBUG
			printf("#if found\n");
#endif
			if(  (layer != 0) && (layerStack[layer-1] == REMOVE_ALL) )
			{
					layerStack[layer++] = REMOVE_ALL;
					continue;

			}
			else
			{
				layerStack[layer++] = REMOVE_DEF;
				memset(tmpLine, 0, sizeof(tmpLine));
                                convertInitialString(tmpLine, tmpBuf, IFDEF);

                                memset(ifStack[layer-1], 0, sizeof(ifStack[layer-1]) );
                                strcpy(ifStack[layer-1], tmpBuf);

                                fputs(tmpLine, fpTmp);
                                endifStack[layer-1] = 1;
			}
//			printf("#if\n");
			continue;						
		}

		//case when #if is found, NOT match
		if(MATCH_IF_NOTFOUND == ret)
		{
#ifdef DEBUG
			printf("#if not found\n");
#endif
			layerStack[layer++] = REMOVE_ALL;
			memset(tmpLine, 0, sizeof(tmpLine));
			//printf("#if not found\n");
			
			memset(ifStack[layer-1], 0, sizeof(ifStack[layer-1]) );
                        strcpy(ifStack[layer-1], tmpBuf);
			//convertInitialString(tmpLine, tmpBuf, IFDEF);

			//fputs(tmpLine, fpTmp);
                        endifStack[layer-1] = 0;
			continue;
		}

		//case when #endif is found
		else if( MATCH_ENDIF == ret)
		{
#ifdef DEBUG
			printf("#endif\n");
#endif
			if(endifStack[layer-1])
			{
				endifStack[layer-1] = 0;
			//	layer--;
				memset(tmpLine, 0, sizeof(tmpLine));
				sprintf(tmpLine,"#endif\n");
				fputs(tmpLine, fpTmp);
				
		//		printf("enter parseOptionsFromConfigFile #endif \n");
			}
			layer--;
			
		}
		else if( MATCH_ELSE == ret)
		{
#ifdef DEBUG
			printf("else\n");
#endif
#if 1
			if( (layerStack[layer-2] == REMOVE_ALL) && (layer > 1) )
			{
				layerStack[layer-1] = REMOVE_ALL;
				endifStack[layer-1] = 0;
//				printf("111\n");
				continue;
			}
#endif
		//	printf("222111\n");
			if(layerStack[layer-1] == REMOVE_ALL) 
			{
		//		printf("333\n");
				memset(tmpLine, 0, sizeof(tmpLine));
		//		printf("333_000\n");
                                convertInitialString(tmpLine, ifStack[layer-1], ELSE);
		//		printf("333_111\n");
                                //memset(ifStack[layer-1], 0, sizeof(ifStack) );
                                //strcpy(ifStack[layer-1], tmpBuf);
				//fputs("\n", fpTmp);
                                fputs(tmpLine, fpTmp);
				fputs("\n", fpTmp);
                                //layerStack[layer++] = REMOVE_DEF;

				layerStack[layer-1] = REMOVE_DEF;
				endifStack[layer-1] = 1;
				
			}
			else if(layerStack[layer-1] == REMOVE_DEF)
			{
//				printf("444\n");
				layerStack[layer-1] = REMOVE_ALL;
				endifStack[layer-1] = 1;
			}
		//	endifStack[layer-1] = 1;

		//	printf("enter parseOptionsFromConfigFile #else \n");
                        continue;
		}
		
		else if( MATCH_PLAIN_CODE == ret )
		{
			//printf("code\n");
			if( (layer == 0) ||  (layerStack[layer-1] == REMOVE_DEF) )
			{
				fputs(tmpBuf, fpTmp);
				//printf("enter parseOptionsFromConfigFile oringinal line \n");
			}
			else
			{
		//		printf("remove all:%s\n", tmpBuf);
			}
		}
	//	printf("4\n");
		memset(tmpBuf, 0, sizeof(tmpBuf) );
		
	}

	
	fclose(fpTmp);
	fclose(fp);
	memset(cmd, 0, sizeof(cmd) );
	sprintf(cmd, "cp -f %s %s",TMP_FILE_DIR,  fileName);
	system(cmd);
	
	memset(cmd, 0, sizeof(cmd) );
	sprintf(cmd, "rm -f %s", TMP_FILE_DIR);
	system(cmd);

	return 0;
		
}


int main(int argc, char *argv[])
{
	char *ptr = NULL;
	FILE *fp;
	char cmd[128] = {0};
	char fileName[256] = {0};
	int errFlag = 0;
	int ret;
#if 0 
	if(strcmp(argv[1], "kernel") != 0 )
	{
		printf("finding .c files\n");
		memset(cmd, 0, sizeof(cmd) );
		sprintf(cmd, "find %s/ -name \"*.c\"  > file_list.tmp", argv[1] );
		system(cmd);

		printf(" adding .h files\n");
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "find %s/ -name \"*.h\"  >> file_list.tmp",argv[1] );
        	system(cmd);
	}
#endif
	printf("parsing the list\n");
//	system("rm -f tools/cplopts_checktool/tmp");
//	system("mkdir tools/cplopts_checktool/tmp");
	
	
	fp = fopen("tools/cplopts_checktool/tmp/operate_filelist.tmp", "r");
	if( fp == NULL)
	{
		printf("open file error!\n");
		return -1;
	}

//	ptr = (char *)malloc(0);
//	printf("ptr is : %p \n", ptr);
//	free(ptr);
//	return 0;
	
	while ( fscanf(fp, "%s", fileName) != EOF )
	{
		printf("processing file:%s\n",fileName);
		ret = parseOptionsFromConfigFile(fileName, argv[2]);
		if(ret < 0)
		{
//			printf("error occur  parseOptionsFromConfigFile in file %s\n", fileName);
			if(errFlag == 0 )
				printf("\n");
			printf("ERROR:%s\n", fileName);
	//		exit(1);
			
			errFlag = 1;
		}
		//printf("%s\n", argv[1]);
		memset(fileName, 0, sizeof(fileName));
//		if(errFlag)
//			printf(".");	
	}
	printf("\ndone!\n");
	fclose(fp);

	//printf("test2-----------------------------\n");
	//parseOptionsFromConfigFile("file", "Config");
	return 0;
}
