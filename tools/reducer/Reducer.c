#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char gVersion[]="1.1";

int reduceFile(char *srcp, char *dstp);
int restoreFile();
void printUsage();
/* mode 0: do not replace '\n' to ' ';
   mode 1: replace '\n' to ' ';
*/
//int mode = 0; 
                    

int main(int argc, char * argv[])
{
	FILE * ctrp, *ctrp2; 
	int  result;
	char	str[200];
	char	ObjFileName[50];//this name is for Pagebuilder, this is same as original file name
	char	BakFileName[50]; // this name is for backup file
	char * p, *e;

	if(argc > 1 ){
		if( strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-V") == 0){
			printf("Reducer Version %s \n", gVersion);
			printf("Author: Haijian Zhang 2006-11-21 \n");
			return 0;
		}
		// this mode is used to rename bak file to original name; 
		else if( strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-R") == 0){
			restoreFile();
			return 0;
		}else{
			printUsage();
			return 0;
		}

#if 0
		n=atoi(argv[1]);
		switch(n){
		case 0:
			mode = 0;
			break;
		case 1:
			mode = 1; 
			break;
		default:
			printf("usage: Reducer 0 (Do not replace '\\n' to ' ' ) \n");
			printf("       Reducer 1 (Replace '\\n' to ' ' ) \n");
			break;
		}
#endif
	}
	//printf("Reducer mode = %d\n", mode);
	/* open Reducer control file */ 
	if((ctrp = fopen("ctrl.txt","rt"))==NULL){ 
		printf("\nCannot open Reducer control file: ctrl.txt"); 
		exit(1); 
	} 
	
	/* open PagerBuilder control file */ 
	if(( ctrp2=fopen("basic.pbb","wt"))==NULL){ 
		printf("\nCannot open PagerBuilder control file: basic.pbb"); 
		exit(1); 
	}

	/* get file name from control file */
	while( fgets(str, 1000, ctrp) != NULL){
		p = str;
		while( *p == ' ' || *p == '\t')
			;
		if( *p == '\n')
			continue;
		e = strchr(p, '\n');
		if( e != NULL)
			*e = '\0';
		/* delete */
		strcpy(ObjFileName, p);
		/* bakup origianl file by renaming it */
		strcpy(BakFileName, p);
		strcat(BakFileName, ".bak"); // file.htm.bak
		result= rename( p , BakFileName); 
		if (result != 0 ){
			printf( "Error renaming file \n" );
			goto error;
		}
		/* Generate reduced file */
		printf("Reducing %s ... ... ... ", ObjFileName);
		if( reduceFile(BakFileName,ObjFileName) != 0){
			printf("reduceFile fail!\n");
			goto error;
		}
		printf("OK!\n");
		/*  save name to PageBuilder control file */
		fprintf(ctrp2, "%s\n",ObjFileName);
	} 
	printf("Reducer finished successfully!\n");
error:
	fclose(ctrp); 
	fclose(ctrp2);
	return 0;
}


int reduceFile(char * srcFile, char * dstFile)
{
	FILE *f1 , *f2;
	char str[1000];
	char *h, *e;

	/* open src file (html file) */
	if((f1=fopen(srcFile,"rt"))==NULL){ 
		printf("\nCannot open srcFile: %s\n", srcFile); 
		return -1;
	} 
	
	if((f2=fopen(dstFile,"wt"))==NULL){ 
		printf("\nCannot open dstFile: %s\n", dstFile); 
		return -1; 
	} 

	while( fgets(str, 1000, f1) != NULL){
		h=str;
		
		if(strstr(dstFile, ".asp")!=NULL)
		{
			while( *h == ' ' || *h == '\t')
				h++;
		}
		else
		{
			if((*h == '#')&&(!strstr(str, "#!/bin/sh")))
				continue; //ignore comment
		}
		if((*h =='\r')&&(*(h+1)=='\n')) // empty line, only has a '\n';
			continue;  // ignore this line;
		if (*h == '\n')
			continue;

		/* remove plank space at the end of the line */
		e=strchr(h,'\n');
		if( e != NULL){
			e--;
			while ( *e == ' ' || *e == '\t')
				e--;
			e++;
			*e = '\n';
			e++;
			*e='\0';
		}
#if 0 // this method could not reduce code size, discard it. by zhj 2006-11-24
		if( mode ){
			/*try to replace '\n' to ' ', this MAY reduce code. */
			e = strchr(h, '\n');
			if( e!=NULL)
				*e=' ';
		}
#endif
		fputs(h, f2);
	}

	fclose(f1); 
	fclose(f2);
	return 0;
}

int restoreFile()
{
	FILE * ctrp; 
	int  result;
	char	str[200];
	char	cmd[100];
	char	BakFileName[50]; // this name is for backup file
	char * p, *e;

	if((ctrp = fopen("ctrl.txt","rt"))==NULL){ 
		printf("\nCannot open Reducer control file: ctrl.txt"); 
		exit(1); 
	} 
	
	/* get file name from control file */
	while( fgets(str, 1000, ctrp) != NULL){
		p = str;
		while( *p == ' ' || *p == '\t')
			;
		if( *p == '\n')
			continue;
		e = strchr(p, '\n');
		if( e != NULL)
			*e = '\0';
		
		/* delete tmp file */
		sprintf(cmd,"rm -f  %s",p);
		system(cmd);
		/* get backup file name */
		strcpy(BakFileName, p);
		strcat(BakFileName, ".bak"); // file.htm.bak
		/* restore file */
		result= rename( BakFileName , p); 
		if (result != 0 ){
			printf( "Error restore file \n" );
			goto error;
		}
	} 
	printf("Restore finished successfully!\n");
error:
	fclose(ctrp); 
	return 0;

}
void printUsage()
{
	printf("Reducer		: Backup original file and generate a reduced file;\n");
	printf("Reducer -v	: Display version information;\n");
	printf("Reducer -r	: Restore backup file;\n");
	return;
}

