#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  RUNNING_ROMFILE_FLAG "romfile.cfg"

int main(int argc, char **argv)
{

	 FILE * fpread = NULL;
	 FILE * fpwrite = NULL;
	 char * buf = NULL;
	 char * p = NULL;
  	 fpread = fopen(RUNNING_ROMFILE_FLAG, "r");
 	 if(NULL == fpread)
	 {
  		printf("Open romfile failured.\n");
		return 0;
	 }
    	fseek(fpread,0L,SEEK_END);
    	int LENGTH = ftell(fpread);
	buf = (char *)malloc(LENGTH);
	if(NULL == buf)
	{
           	printf("Allocate memory failed.\n ");
		fclose(fpread);		
		return 0;
	}
	
	fseek(fpread,0L,SEEK_SET);
	fread(buf,LENGTH,1,fpread);
	p = strstr(buf, "RestoreLinos");
	 
	 if(NULL == p)
	{
		printf("Not found Rom-tToRomfile Flag.\n");
		free(buf);
		fclose(fpread);
		return 0;
	}
	 
	else
	{
		p += 14;		
		*p = '0';
		printf("p is %c \n", *p);
		fclose(fpread);
		fpwrite = fopen(RUNNING_ROMFILE_FLAG, "w+");
		if(NULL==fpwrite)
		{
			printf("Open romfile failured.\n");
			return 0;
		}
		fwrite(buf, LENGTH, 1, fpwrite);
		free(buf);
		fclose(fpwrite);
		return 1;
			
	}		 
	 	
	 
}

