
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

	ptr = strstr(string,"defined(");
	if(ptr == NULL)
		return -1;
	ptr+=sizeof("defined(") - 1;
	//printf("%s\n", ptr);

	if( (*ptr == 0) || (*ptr == ' ') || (*ptr == ')'))
	{
		return -1;
	}
#if 0
	while(*ptr == ' ')
	{
		ptr++;
	}
#endif
	do
	{
		*ptrTmp = *ptr;
		ptr++; ptrTmp++;
	}while(  (*ptr != ')') && (*ptr != ' ') && (*ptr != 0));
#if 0
		
	if(*(ptr-1) != ")")
	{
		printf("%s\n",tmp);
	//	printf("N");
	}
	else
		printf("Y");
#endif
	if( strstr(string,")") != ptr )
		return -1;
//	printf("\n%d\n",(int)( strstr(string,")") - ptr) );		
//	memset(string, 0, len);
//	strcpy(string, tmp);

	return 0;
}

#if 0
int getOptionFromConfigFile(char* opt,char* configFile)
{
	FILE *fp =NULL;
	char line[512] = {0};

	fp = fopen(configFile,"r");
	//printf("configFile:%s\n",configFile);
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
//			printf("SUCESS");
			return 1;
		  }
		memset(line, 0, sizeof(line));
	}

	fclose(fp);
//	printf("not found\n");
	return 0;
}
#endif

int parseMacros(char *tmpBuf, char* configFile)
{
      
	char symb[512] = {0};
	char tmp[7][32] = {{0},{0}};

	char macro[8][512] = {{0},{0}};
	int count = 0, ret;
	char cmd[512] = {0};
	int i;
	int ret_status, result;
	
	int prevFlag = 0,symFlag = 0;
	
	ret  = sscanf(tmpBuf, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",  \
		symb,macro[0],tmp[0],macro[1],tmp[1],macro[2],tmp[2],macro[3],tmp[3], \
		macro[4],tmp[4],macro[5],tmp[5],macro[6],tmp[6],macro[7]);
	//printf("parseMacros: enter parseMacro:%s,%s\n",symb,macro);

	/* case match #endif */
	if( 0 == strcmp(symb, "#endif/*TCSUPPORT_COMPILE*/") )
		return MATCH_ENDIF;
	else if(
                        ( NULL != strstr(tmpBuf, "#endif") )  && \
                        ( NULL != strstr(tmpBuf, "/*TCSUPPORT_COMPILE*/") )
                )
                return ERROR_STRING;


	if( 0 == strcmp(symb, "#else/*TCSUPPORT_COMPILE*/") )
                return MATCH_ELSE;
	else if( 
			( NULL != strstr(tmpBuf, "#else") )  && \
			( NULL != strstr(tmpBuf, "/*TCSUPPORT_COMPILE*/") )
		)
		return ERROR_STRING;
		
		

	if( 0 != strcmp(symb, "#if/*TCSUPPORT_COMPILE*/") )
	{
		if(
                        ( NULL != strstr(tmpBuf, "#if") )  && \
                        ( NULL != strstr(tmpBuf, "/*TCSUPPORT_COMPILE*/") )
                )
		return ERROR_STRING;

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

	for(i = 0; i < ret/2; i++)
	{
		result = removeDef(macro[i],sizeof(macro[i]));
		if(result < 0)
		{
			return ERROR_STRING;
		}
		if( i != ret/2 -1)
		{
			if(strstr(tmp[i],"&&") == NULL) 
			{
				if(i == 0)
				{
					symFlag = prevFlag = 0;
				}
				else 
				{
					prevFlag = symFlag;
					symFlag = 0;
					if(prevFlag != symFlag)
					{
						return ERROR_STRING;
					}
				}
			}
			
			else if (strstr(tmp[i], "||") == NULL)			
			{
				if(i == 0)
                                {
                                        symFlag = prevFlag = 1;
                                }
                                else
                                {
                                        prevFlag = symFlag;
                                        symFlag = 1;
                                        if(prevFlag != symFlag)
                                        {
                                                return ERROR_STRING;
                                        }
                                }

				
			}
			else
			{
				return ERROR_STRING;
			}
		}
/* no need to parse macro from config file*/
#if 0
		ret_status = getOptionFromConfigFile(macro[i],configFile);
		if(ret_status == -1)
		{
#ifdef DEBUG
			printf("error in Config file\n");
#endif
			return ERROR_STRING;
		}
		else if(ret_status)
		{

#ifdef DEBUG
			printf("match one or more macro\n");
#endif
			/* match one or more macro*/
			return MATCH_IF_FOUND;
		}
#endif
	}
#ifdef DEBUG
	printf("if not match any case\n");
#endif
	/* force to return if found here */
	return MATCH_IF_FOUND;
}
#if 0

int convertInitialString(char *tmpLine,char *tmpBuf, int flag /*IFDEF*/)
{	
	char *ptr; // strstr(tmpBuf,"/*TCSUPPORT_COMPILE*/");
	char macro[4][512] ={ {0},{0},{0},{0} };
	char symb[128] = {0};
	char tmp[128] = {0};
	int i, ret, len;


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

	printf("111");
	for(i = 0; i < ret/2; i++)
	{
		removeDef(macro[i],sizeof(macro[i]));
		printf("macro[%d]:%s\n", i, macro[i]);
	}
	printf("222");
	while(*ptr == ' ')
		ptr++;

	printf("333");

	if(flag == IFDEF)
	{
		sprintf(tmpLine, "#if %s", ptr);
		printf("tmpline:%s\n",tmpLine);		
	}

	else if(flag == ELSE)
	{
		sprintf(tmpLine,"#if ");
		for(i = 0; i < ret/2; i++)
		{
			len = strlen(tmpLine);
			sprintf(tmpLine + len, "!defined(%s) ", macro[i]);
			if( i != ret/2 - 1 )
			{
				len = strlen(tmpLine);
				sprintf(tmpLine + len, "&& ");
			}
		}
			
		len = strlen(tmpLine);
		printf(tmpLine + len, "\n");

		printf("tmpline:%s\n", tmpLine);
	}

	return 0;
}

#endif

int parseOptionsFromConfigFile(char *fileName, char *configFile)
{
	
	
	char tmpBuf[512] = {0};
//	char buf[512] = {0};
	FILE *fp= NULL;



	int layer = 0;
	char cmd[512] = {0};
		

	int operation_flag = 0;
	int ret;

	int line = 0;
	int markedLine = 0;

	
	fp = fopen(fileName,"r");
	if( fp  == NULL)
	{
		printf("\nerror in %s", fileName);
		return -1;
	}


	while(fgets (tmpBuf, sizeof(tmpBuf), fp)  )
	{
		line++;

		if(layer < 0)
		{
			printf("\nerror occurs around line: %d ", markedLine);
			return -1;
		}

		ret = parseMacros(tmpBuf, configFile);
		if(ret == ERROR_STRING)
		{
			printf("\nerror occurs around line: %d ", line);
			return -1;
		}
			

	    //case when #ifdef is found, and match
		if ( MATCH_IF_FOUND == ret )
		{

#ifdef DEBUG
			printf("#if found\n");
#endif
			layer++;
			operation_flag = 1;

			markedLine = line;
			continue;						
		}

		//case when #if is found, NOT match
		if(MATCH_IF_NOTFOUND == ret)
		{
#ifdef DEBUG
			printf("if not found\n");
#endif
			layer++;
			operation_flag = 1;
			
			markedLine = line;
			continue;
		}

		//case when #endif is found
		else if( MATCH_ENDIF == ret)
		{
#ifdef DEBUG
			printf("endif\n");
#endif
			layer--;
			operation_flag = 1;
			
			markedLine = line;
		}
//		memset(buf, 0, sizeof(buf));
//		strcpy(buf, tmpBuf);
		
		//case when none #if in previous but #else founded, return error
		else if( (MATCH_ELSE == ret) && !layer)
                {
#ifdef DEBUG
                        printf("else founded without if\n");
#endif

                        markedLine = line;
			printf("\nerror occurs around line: %d ", markedLine);
			return -1;
                }

		
		
		memset(tmpBuf, 0, sizeof(tmpBuf) );
		
	}

	
	fclose(fp);
	


	if(layer == 0)
	{
		if(operation_flag == 1)
		{
//			printf("kkk\n");
			return 1;
		}
		else
			return 0;
	}
	else
	{
		printf("\nerror occurs around line: %d ", markedLine);
		return -1;
	}
		
}


int backupFiles(char *buf)
{
#define FILENAME "tools/cplopts_checktool/tmp/restore.sh"
        char tmpBuf[512] = {0};
        char cmd[512] = {0};
        char fileName[256] = {0};
        char dirPath[512] = {0};
        int len = strlen(buf);

//	printf("\nSUCCESS:%s\n",buf);
        strcpy(tmpBuf, buf);
        //printf("%s\n",tmpBuf);
        //tmpBuf[len] = 0;
        tmpBuf[len-1] = 0;
        while(tmpBuf[len-1] != '/' && len >= 0)
        {
                //printf("len=%d,%c\n",len,tmpBuf[len-1]);

                len--;
        //      if( (len > 0) && (tmpBuf[len-1] == '\n') )
        //      {
        //              tmpBuf[len-1] = '\n';
        //              printf("error\n");
        //      }

        }
        if(len < 0)
        {
                printf("path error! \n");
                return -1;
        }

        if(len > 0)
                tmpBuf[len-1] = 0;
        //printf("dirPath:%s\n",dirPath);
        strcpy(fileName,(tmpBuf+len));
        tmpBuf[len-1] = 0;
        strcpy(dirPath, tmpBuf);

        //printf("filename:%s,dir:%s\n",fileName,dirPath);

	memset(cmd, 0, sizeof(cmd) );
        sprintf(cmd,"mkdir -p backup/%s", dirPath);
        system(cmd);
#ifdef DEBUG
        printf("%s\n",cmd);
#endif
	memset(cmd, 0, sizeof(cmd) );
        sprintf(cmd, "cp -f %s/%s backup/%s", dirPath,fileName, dirPath);
        system(cmd);
#ifdef DEBUG
        printf("%s\n",cmd);
#endif
      
	memset(cmd, 0, sizeof(cmd) );
        sprintf(cmd,"echo \"cp -f backup/%s/%s %s/%s\" >> %s", dirPath,fileName,dirPath, fileName, FILENAME);
        system(cmd);
#ifdef DEBUG
        printf("%s\n",cmd);
#endif
        return 0;

}

int main(int argc, char *argv[])
{
	char *ptr = NULL;
	FILE *fp;

	FILE *fpWrite = NULL;

	char cmd[512] = {0};
	char fileName[256] = {0};
	int ret;
	int errFlag = 0;
	int num;
 	
	system("rm -rf backup");
	system("rm -f tools/cplopts_checktool/tmp/restore.sh");
	system("rm -f tools/cplopts_checktool/tmp/error");
	system("cat tools/cplopts_checktool/scripts_list.tmp > tools/cplopts_checktool/tmp/file_list.tmp");
//	if(strcmp(argv[1], "kernel") != 0 )
	{
		printf("finding .c files\n");
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "find %s/ -name \"*.c\"  >> tools/cplopts_checktool/tmp/file_list.tmp", argv[1] );
	// sprintf(cmd, "find %s -name \"*.c\" >> file_list.tmp", line );
		system(cmd);

		printf(" adding .h files\n");
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "find %s/ -name \"*.h\"  >> tools/cplopts_checktool/tmp/file_list.tmp",argv[1] );
        	system(cmd);
	}

	printf("parsing the list\n");	
	fp = fopen("tools/cplopts_checktool/tmp/file_list.tmp", "r");
	if( fp == NULL)
	{
		printf("\nopen read file error!\n");
		return -1;
	}

	fpWrite = fopen("tools/cplopts_checktool/tmp/operate_filelist.tmp","w");
	if( fpWrite == NULL)
	{
		printf("\nopen write file error!\n");
		fclose(fp);
		return -1;
	}

//	ptr = (char *)malloc(0);
//	printf("ptr is : %p \n", ptr);
//	free(ptr);
//	return 0;
	
	while ( fscanf(fp, "%s", fileName) != EOF )
	{
		//printf("processing file:%s\n",fileName);
		if(strstr(fileName,"tools/cplopts_checktool/cplopts_") != NULL)
		{
			printf("\nchecking file: %s ommitted!\n", fileName);
			continue;
		}

		ret = parseOptionsFromConfigFile(fileName, argv[2]);
		if(ret < 0)
		{
//			printf("error occur  parseOptionsFromConfigFile in file %s\n", fileName);
			printf("\nERROR:%s\n", fileName);
		//	exit(1);
			errFlag = 1;
		}
		//printf("%s\n", argv[1]);
		num =  strlen(fileName);
		fileName[num] = '\n';
		if(ret == 1)
		{	fwrite(fileName, strlen(fileName) , 1, fpWrite);
			backupFiles(fileName);
			//printf("%s\n", fileName);
		}
		memset(fileName, 0, sizeof(fileName));
		if(errFlag == 0)
			printf(".");	
	}
	printf("\ndone!\n");
	fclose(fpWrite);
	fclose(fp);

	//printf("test2-----------------------------\n");
	//parseOptionsFromConfigFile("file", "Config");
	if(!errFlag)
	{
		memset(cmd, 0, sizeof(cmd) );
		sprintf(cmd, "tools/cplopts_remove %s %s", argv[1], argv[2]);
		printf("%s\n",cmd);		
		system(cmd);
	}
	else
	{
		system("echo 1 > tools/cplopts_checktool/tmp/error");
	}
	return 0;
}
