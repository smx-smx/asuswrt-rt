#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

int mode_flag=0;
int type_flag=0;

char *TcWebApiAdv_adsl_get(int id)
{
	char buf[128];
	if(id==GB_C_ADSL_MODE)
	{
		
	  	if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_auto))
    		sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_mode_auto);
		else
    		sprintf(buf,"<OPTION >%s",val_def->adsl_mode_auto);
  		ASP_OUTPUT(buf); 
 
		if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_adsl2_add))
    		sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_mode_adsl2_add);
  		else
    		sprintf(buf,"<OPTION >%s",val_def->adsl_mode_adsl2_add);    
  		ASP_OUTPUT(buf);  

		fprintf(stderr,"----mode:%s\n",web_api->adv_adsl->mode);
		fprintf(stderr,"----adsl_mode_adsl2_add:%s\n",val_def->adsl_mode_adsl2_add);		
		if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_adsl2))
 		   	sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_mode_adsl2);
	  	else
 		   	sprintf(buf,"<OPTION >%s",val_def->adsl_mode_adsl2);  
 	 	ASP_OUTPUT(buf);

		if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_gdmt))
   		 	sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_mode_gdmt);
  		else
    	sprintf(buf,"<OPTION >%s",val_def->adsl_mode_gdmt);
 		ASP_OUTPUT(buf);  

		if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_t1413))
 	 	  sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_mode_t1413);
 		 else
 		   sprintf(buf,"<OPTION >%s",val_def->adsl_mode_t1413);    
 		 ASP_OUTPUT(buf); 

		if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_glite))
 	  		 sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_mode_glite);
	    else
			sprintf(buf,"<OPTION >%s",val_def->adsl_mode_glite);  
 		ASP_OUTPUT(buf); 
 	
    }	
	else if(id==GB_C_ADSL_TYPE)
	{
		if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_a))
		    sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_type_annex_a);
		else
		    sprintf(buf,"<OPTION >%s",val_def->adsl_type_annex_a);  
 		 ASP_OUTPUT(buf);  
  
		if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_i))
 		   sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_type_annex_i);
		else
		    sprintf(buf,"<OPTION >%s",val_def->adsl_type_annex_i);  
		ASP_OUTPUT(buf);  
		if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_a_l))
   			 sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_type_annex_a_l);
 		else
 			 sprintf(buf,"<OPTION >%s",val_def->adsl_type_annex_a_l);  
 		 ASP_OUTPUT(buf); 
		if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_m))
             sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_type_annex_m);
		  else
		    sprintf(buf,"<OPTION >%s",val_def->adsl_type_annex_m);  
 		 ASP_OUTPUT(buf); 
		if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_all))
   			 sprintf(buf,"<OPTION SELECTED>%s",val_def->adsl_type_annex_all);
  		else
  			  sprintf(buf,"<OPTION >%s",val_def->adsl_type_annex_all);  
 		 ASP_OUTPUT(buf); 
   
	}
	return ASP_OK;
}

int TcWebApiAdv_adsl_set(int id,char *value)
{

    	char *ret,str[16],*p,tmp[2],*buf, cmdbuf[128];
    	int i,num,j,fd;
    	char opencmd[] = "wan adsl opencmd ";
    	char setannex[] = "wan ghs set annex ";
	
	
	ret =ASP_FORM(value);
	if(ret ==NULL)
		return -1;

   	i=0;
   	p=ret;
   	for(j=0;j<16;j++)
   	{   	
   		if(*p=='%')
   		{
   			strncpy(tmp,p+1,2);
   			num=strtol(tmp,NULL,16);
   			str[i++]=num;
   			p=p+3;
   		}
   		else
   			str[i++]=*p++;
   		if(p==NULL)
   			break;		
   	
   	}
   	str[i]='\0';
  	fprintf(stderr,"--strlen-%d\n",strlen(str));
  	if(id == GB_C_ADSL_MODE)
  	{
  		strncpy(web_api->adv_adsl->mode,str,16);
  		mode_flag=1;
  	}
  	else if(id == GB_C_ADSL_TYPE)
  	{
		strncpy(web_api->adv_adsl->type,str,16);
		type_flag=1;
  	}		

  	if(mode_flag==1&&type_flag==1)
  	{
		buf=(char *)ASP_MALLOC(128);
		remove("/etc/adsl.conf");
		fd=open("/etc/adsl.conf",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
		sprintf(buf,"MODULATIONTYPE=\"%s\"\nANNEXTYPEA=\"%s\"\n",web_api->adv_adsl->mode,web_api->adv_adsl->type);
		write(fd,buf,strlen(buf));
		close(fd);
		ASP_FREE(buf);  	
		type_flag=0;
		mode_flag=0;
  	}	

  	fd = open("/etc/adsl.sh",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
  	if(fd !=-1)
  	{
		if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_auto))
			sprintf(cmdbuf, "%smultimode", opencmd);
		else if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_adsl2_add))
		    	sprintf(cmdbuf, "%sadsl2plus", opencmd);
		else if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_adsl2))
		 	sprintf(cmdbuf, "%sadsl2", opencmd);
		else if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_gdmt))
		   	sprintf(cmdbuf, "%sgdmt", opencmd);
		else if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_t1413))
		 	sprintf(cmdbuf, "%st1.413", opencmd);
		else if(!strcmp(web_api->adv_adsl->mode,val_def->adsl_mode_glite))
		 	sprintf(cmdbuf, "%sglite", opencmd);

		asp_execute(cmdbuf);
		write(fd, cmdbuf, strlen(cmdbuf));
		write(fd, "\n", strlen("\n"));
		  
		if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_a))
			sprintf(cmdbuf, "%sa", setannex);
		else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_i))
		 	sprintf(cmdbuf, "%si", setannex);
		else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_a_l))
		   	sprintf(cmdbuf, "%sal", setannex);
		else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_m))
		      sprintf(cmdbuf, "%sm", setannex);
		else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_all))
		   	sprintf(cmdbuf, "%saijlm", setannex);

		asp_execute(cmdbuf);
		write(fd, cmdbuf, strlen(cmdbuf));
		write(fd, "\n", strlen("\n"));
		close(fd);
		asp_execute("wan adsl reset");
  	}
	return 0;
}


