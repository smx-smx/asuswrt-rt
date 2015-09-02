#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>  
#include <sys/wait.h>

extern char *post;

static int update_type=0;/*0:kernal;1:rootfs*/

int TcWebApiUpdate_execute(int id)
{
	
	update_type = 0;
	if(post == NULL)
		return 0;
		
}	

