#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include <json.h>

#include "http-get-utils.h"
//#include "../../../lib/libtcapi.h"
char *get_param (s_var **vars, const char *name)
{
    int i;
	if(vars == NULL)
		return NULL;
	
    for (i=0;vars[i]; i++)
	if (!strcmp(name,vars[i]->name)) {	
	    return vars[i]->value;
	}
	
    return NULL;
}

/* for ASUS Router (AiHome) APP */
char *get_cgi_json(s_var **vars, char *name, json_object *root)
{
    int i;
	if(vars == NULL)
		return NULL;

    for (i=0;vars[i]; i++)
	if (!strcmp(name,vars[i]->name)) {
	    return vars[i]->value;
	}

	struct json_object *json_value;
	json_value = json_object_object_get(root, name);
	return (char *)json_object_get_string(json_value);
}

void free_param_line(s_var **vars)
{
	int i;
	if(vars == 	NULL)
		return;
	
	for (i = 0;vars[i];i++)
	{
		if(vars[i]->name)
			free(vars[i]->name);
		if(vars[i]->value)
			free(vars[i]->value);
		free(vars[i]);
	}
	free(vars);
}

s_var **parse_param_line (char *line)
{
    int numargs;
    char *cp, *ip, *esp, *sptr;
    s_var **result = NULL;
    int i, k, len;
    
    if (line) {
	    for (cp=line; *cp; cp++)
		if (*cp == '+')
		    *cp = ' ';

	    if (strlen(line)) {
		for (numargs=1,cp=line; *cp; cp++)
		    if (*cp == '&') numargs++;
	    } else
		numargs = 0;


	    len = (numargs+1) * sizeof(s_var *);
	    if ((result = (s_var **)malloc (len)) == NULL)
	    {
			return NULL;
			}
	    memset (result, 0, len);

	    cp = line;
	    i=0;
	    while (*cp) {
		if ((ip = (char *)strchr(cp, '&')) != NULL) {
		    *ip = '\0';
		}else
		    ip = cp + strlen(cp);

		if ((esp=(char *)strchr(cp, '=')) == NULL) {
		    cp = ++ip;
		    continue;
		}

		if (!strlen(esp)) {
		    cp = ++ip;
		    continue;
		}

		if (i<numargs) {

		    /* try to find out if there's already such a variable */
		    for (k=0; k<i && (strncmp (result[k]->name,cp, esp-cp) || !(strlen (result[k]->name) == esp-cp)); k++);

		    if (k == i) {	/* No such variable yet */
			if ((result[i] = (s_var *)malloc(sizeof(s_var))) == NULL)
			{
				    return NULL;
			}
			if ((result[i]->name = (char *)malloc((esp-cp+1) * sizeof(char))) == NULL)
			 {

		    return NULL;
		  }
		  
			memset (result[i]->name, 0, esp-cp+1);
			strncpy(result[i]->name, cp, esp-cp);
			cp = ++esp;
			if ((result[i]->value = (char *)malloc((ip-esp+1) * sizeof(char))) == NULL)
			{

			    return NULL;
			 }
			  
			memset (result[i]->value, 0, ip-esp+1);
			strncpy(result[i]->value, cp, ip-esp);
			//FIXME result[i]->value = cgiDecodeString(result[i]->value);
			i++;
		    } else {	/* There is already such a name, suppose a mutiple field */
			cp = ++esp;
			len = (strlen(result[k]->value)+(ip-esp)+2) * sizeof (char);
			if ((sptr = (char *)malloc(len)) == NULL)
			{
			    return NULL;
			  }
			  
			memset (sptr, 0, len);
			sprintf (sptr, "%s\n", result[k]->value);
			strncat(sptr, cp, ip-esp);
			free(result[k]->value);
		//FIXMEresult[k]->value = cgiDecodeString (sptr);
		result[k]->value = sptr;
	    }
	}
	cp = ++ip;
    }
  }

  return result;
}
/*______________________________________________________________________________
**function name:multipart_parser
**
**description:
*   In order to get item name and value for multi part
**parameters:
*	req:gloable value to save some information for web page
*	boundary:like ------xxxxxx 
**global:
*    none
**return:
*    	0:success
*	-1:fail
**call:
*    none
**revision:
*     1.shnwind 20090410
**____________________________________________________________________________*/
int multipart_parser(request *req, char *boundary){

	int fd, len, bound_len, read_size;
	int pos = 0;
	int pos_offset = 0;
	char status = GET_BOUNDARY; 
	char get_binary_flag = 0;//if set ,it means need to save data start and end
	char *temp, *name, *value;

	/*This parser is used to get all form information.
		detail state describution is written in design spec - Linux Firmware Upgrade*/
	bound_len=strlen(boundary);
	len = atoi(req->content_length);

	if((bound_len <= 0) || len <= bound_len){
		return FAIL;
	}

	unlink(MULTI_TO_NORMAL_TEMP);
	fd = open(MULTI_TO_NORMAL_TEMP,O_WRONLY | O_CREAT);
	if(fd < 0){
		return FAIL;
	}
	temp = (char *)malloc(MULTI_PASER_TEMP_SIZE);
	if(temp == NULL){
		close(fd);
		return FAIL;
	}
	name = (char*)malloc(STRING_BUF_SIZE);
	if(name == NULL){
		close(fd);
		free(temp);
		return FAIL;
	}
	value = (char*)malloc(STRING_BUF_SIZE);
	if(value == NULL){
		close(fd);
		free(temp);
		free(name);
		return FAIL;
	}
	
	while(pos < len){
		/*move to position which has pasered.*/
		if(lseek(req->post_data_fd, pos, SEEK_SET) != pos){
			free(temp);
			free(name);
			free(value);
			close(fd);
			return FAIL;
		}
		/*decide read len*/
		if(len - pos <= MULTI_PASER_TEMP_SIZE){
			read_size = len - pos;
		}else{
			read_size = MULTI_PASER_TEMP_SIZE;
		}
		
		read(req->post_data_fd,temp,read_size);
			
		//tcdbg_printf("len %d status %d pos %d\n",len,status,pos);
		switch(status){
			case GET_BOUNDARY:
				/*search boundary like ---------xxxxxxxx*/
				pos_offset = paser_get_boundary(temp, read_size, boundary);
				switch(pos_offset){
					case KEEP_SEARCH:
						pos_offset = read_size - bound_len;
						
						break;
					case FAIL:
						status = ERROR;
						break;	
					default:
						status = GET_CONTENT;
						if(get_binary_flag == 1){
							if(find_data_end((pos+pos_offset),req->post_data_fd, fd) == FAIL){
								fprintf(stderr,"fail to get data end\n");
								status = ERROR;
							}
							get_binary_flag = 0;	
						}
						break;
				}
				break;
			case GET_CONTENT:
				/*search content*/
				pos_offset = paser_get_content(temp, read_size, name, pos, fd);	
				//tcdbg_printf("offset %d\n",pos_offset);
				switch(pos_offset){
					case FIND_BINARY_DATA:
						//find binary data
						get_binary_flag = 1;
						pos_offset = 0;
						status = GET_BOUNDARY;
						break;
					/*unuse condition. shnwind 2009.9.15*/
					//case KEEP_SEARCH:
						//has content but not binary data
					//	pos_offset = 0;
					//	status = GET_BOUNDARY;
					//	break;
					case FAIL:
						status = ERROR;
						break;
					default:
						status = GET_VALUE;
						break;
				}
				break;
			case GET_VALUE:
				/*get item value*/
				pos_offset = paser_get_value(temp, read_size, value, boundary);
				//tcdbg_printf("offset %d\n",pos_offset);
				if(pos_offset == FAIL){
						pos_offset = 0;
				}else{
					write_name_value(fd,name,value);
				}
				status = GET_BOUNDARY;
				break;
			default:
				break;
		}
		pos += 	pos_offset;				
		if(status == ERROR){
			//fprintf(stderr,"error happen\n");
			break;
		}
	}
	close(fd);
	free(temp);
	free(name);
	free(value);
	return SUCCESS;
}
void 
write_name_value(int fd, char *name, char *value){
	
	//tcdbg_printf("name %s value %s\n",name,value);
	write(fd,name,strlen(name));
	write(fd,"=",1);
	write(fd,value,strlen(value));
	write(fd,"&",1);

}
/*______________________________________________________________________________
**function name:paser_get_boundary
**
**description:
*   find the boundary to the form item
**parameters:
*	buf:the string need search
*    	buf_size:buffer length
*	boundary:like ------xxxxxx 
**global:
*    none
**return:
*    any:content start offset
*	-1:fail
*	-2:need keep search
**call:
*    none
**revision:
*     1.shnwind 20090410
**____________________________________________________________________________*/
int 
paser_get_boundary(char *buf, int buf_size, char *boundary)
{
	int i;
	int len = strlen(boundary);
	
	/*only check (buf_size - len) to prevent processing over the buf_size*/
	for(i=0;i<(buf_size - len); i++){
		if(memcmp((buf+i),boundary,len) == 0){
			break;
		}
	}
	
	if(i == (buf_size - len)){
		return KEEP_SEARCH;
	}else{
		/*return offset need add boundary length*/
		return (i+len);
	}

}
/*______________________________________________________________________________
**function name:paser_get_content
**
**description:
*   find the content to the form item
**parameters:
*	buf:the string need search
*    	buf_size:buffer length
*	name:reture name
*	pos : The position of file before processing
**global:
*    none
**return:
*    any:value start offset
*	-1:fail
*	-2:find binary data
**call:
*    none
**revision:
*     1.shnwind 20090410
**____________________________________________________________________________*/
int 
paser_get_content(char *buf, int buf_size, char *name, int pos, int write_fd)
{
	int i, j;
	char value_start[4]={'\r','\n','\r','\n'};
	int value_start_len = sizeof(value_start);
	//int contentType_strlen = strlen("Content-Type");
	//int octetstream_strlen = strlen("octet-stream");
	int filename_strlen = strlen("filename");	
	//int content_start_offset = 0;
	int crlf_len = 2;// "/r/n" length

	/*should be start with \r\n*/
	if((buf[0] != '\r') || (buf[1] != '\n')){
		//exit(1);
		return FAIL;
	}
	
	/*only check (buf_size - value_start_len) to prevent processing the buf_size*/
	for(i = crlf_len ; i < (buf_size - value_start_len);i++){
		/*try to find value start*/
		if(memcmp(buf+i,value_start,sizeof(value_start)) == 0){
			break;
		}
	}
	if(i == (buf_size - value_start_len)){
		//not find value start
		return FAIL;
	}else{
		/*try to find content_type*/
		//for(j = crlf_len ; j < (i - contentType_strlen + 1);j++){
		//	/*try to find Content-Type*/
		//	if(memcmp(buf+j,"Content-Type",contentType_strlen) == 0){
		//		break;
		//	}
		//}
		/*Only try to find "filename" because the google chrome only include this information.
			shnwind modify. 20090915*/
		for(j = crlf_len ; j < (i - filename_strlen + 1);j++){
			if(memcmp(buf+j,"filename",filename_strlen) == 0){
				break;
			}
		}
		
		//if(j == (i - contentType_strlen+ 1)){
		if(j == (i - filename_strlen + 1)){	
			/*has no content data. It means this data is normal form data.*/
			for(j = crlf_len ; j < (i - value_start_len + 1);j++){
			/*try to find item name --> binary data*/
				if(memcmp(buf+j,"name",4) == 0){
					break;
				}
			}

			if(j == (i - value_start_len + 1)){
				/*not find name*/
				return FAIL;
			}else{
				if(get_parameter_value(buf+j, "name", name) != FAIL){/*find item name*/
					return i;
				}else{
					return FAIL;
				}
			}
			return i;
		}else{
			/*Only try to find "filename" because the google chrome only include this information.
			shnwind modify. 20090915*/
			//content_start_offset = j;
			/*has content data*/
			//for(j = content_start_offset ; j < (i - octetstream_strlen + 1);j++){
			/*try to find octet-stream --> binary data*/
			//	if(memcmp(buf+j,"octet-stream",octetstream_strlen) == 0){
			//		break;
			//	}
			//}
			//if(j == (i - octetstream_strlen + 1)){
			//	/*other type. no support*/
			//	return KEEP_SEARCH;
			//}else{
				/*save file_name*/
				if(get_parameter_value(buf+crlf_len, "filename", name) != FAIL){/*find item name*/
					write_name_value(write_fd,"bfilename",name);
					sprintf(name,"%d",(pos + i + value_start_len));
					write_name_value(write_fd,"bdata_start",name);
					return FIND_BINARY_DATA;
				}else{
					return FAIL;
				}
			//}
			
		}
	
	}

}
/*______________________________________________________________________________
**function name:paser_get_value
**
**description:
*   find the value to the form item
**parameters:
*	buf:the string need search
*    	buf_size:buffer length
*	val:reture value
*	boundary:like ------xxxxxx 
**global:
*    none
**return:
*    any:boundary start offset
*	-1:fail
**call:
*    none
**revision:
*     1.shnwind 20090410
**____________________________________________________________________________*/
int 
paser_get_value(char *buf, int buf_size, char *value, char *boundary)
{
	char value_start[4]={'\r','\n','\r','\n'};
	int i = 0, j = 0;
	int check_bound_len = strlen(boundary);
	int value_start_len = sizeof(value_start);
	int check_pattern_len = 4; //"\r\n--" length
	
	if(memcmp(buf,value_start,sizeof(value_start)) != 0){/*should find next value start*/
		return FAIL;
	}
	if(buf_size < check_bound_len){
		return FAIL;
	}
	/*only check (buf_size - check_bound_len) to prevent processing over the buf_size*/
	/*try to find value end*/
	for(i=value_start_len;i<(buf_size - check_bound_len);i++){
		if(memcmp((buf+i),boundary,check_bound_len) == 0){
			//know real string end
			for(j = i;j > value_start_len;j--){
				if(memcmp((buf+j),"\r\n--",check_pattern_len) == 0){
					break;
				}
			}
			/*check string length*/
			if((j-value_start_len) > STRING_BUF_SIZE){
				return FAIL;
			}

			/*check the string lenth is zero or not*/
			memset(value,0,STRING_BUF_SIZE);
			if(j != value_start_len){
				/*do not copy first /r/n/r/n*/
				memcpy(value,buf+value_start_len,(j-value_start_len));
			}
				
			break;
		}
	}	

	return i;
}
/*______________________________________________________________________________
**function name:get_parameter_value
**
**description:
*   find the value to the specific parameter
**parameters:
*     buf:the string need search
*    	paremeter:specific parameter
*	val:reture value
**global:
*    none
**return:
*    0:success
*	-1:fail
**call:
*    none
**revision:
*     1.shnwind 20080716
**____________________________________________________________________________*/
int
get_parameter_value(char *buf, char *paremeter, char *ret_val){
	char *ret, *ret2;
#if !defined(TCSUPPORT_CT) 
	char tmp[256];
#endif
	
	sprintf(tmp, "%s=\"",paremeter);
   	ret = strstr(buf,tmp);
	if(ret != NULL){
		ret = ret +strlen(tmp);
		ret2 = strstr(ret,"\"");
		if((ret2 ==NULL) || (ret2==ret) || (ret2-ret > STRING_BUF_SIZE)){
			return FAIL;
		}
		memset(ret_val,0,STRING_BUF_SIZE);
		memcpy(ret_val,ret,ret2-ret);
		ret_val[ret2-ret] = 0;
	}else{
		return FAIL;
	}
	return SUCCESS;
}
int
find_data_end(int bound_pos ,int fd, int write_fd){
	int i, check_start;
	char temp[32];
	char *buf;
	int check_pattern_len = 4; //"\r\n--" length
	
	/*read MULTI_PASER_TEMP_SIZE before boundary start because we 
		need to prevent found the boundary start but the \r\n-- 
		at the out of buffer like below.
		----\r\n-----------xxxxxxxxxxxxx
	                     ^     ^
		           temp  pos_offset
	*/
	
	buf = (char *)malloc(MULTI_PASER_TEMP_SIZE);
	if(buf == NULL){
		return FAIL;
	}
	if(bound_pos >= MULTI_PASER_TEMP_SIZE){
		lseek(fd, (bound_pos-MULTI_PASER_TEMP_SIZE), SEEK_SET);
		read(fd,buf,MULTI_PASER_TEMP_SIZE);
		check_start = MULTI_PASER_TEMP_SIZE;
		
	}else{
		lseek(fd, 0, SEEK_SET);
		read(fd,buf,bound_pos);
		check_start = bound_pos;
	}
	
	for(i=(check_start-1) ; i > 0 ; i--){
		if(memcmp((buf+i),"\r\n--",check_pattern_len) == 0){
			//data end is before \r\n-
			i--;
			break;
		}
	}
	free(buf);
	if(i == 0){
		/*not found data end*/
		return FAIL;
	}else{
		/*found data end*/
		//tcdbg_printf("ddd data_end = %d %d %d\n",bound_pos,i,bound_pos-check_start+i);
		/* found data end and save dataend to temp*/
		sprintf(temp,"%d",(bound_pos-check_start+i));
		write_name_value(write_fd,"bdata_end",temp);
		return SUCCESS;
	}
}
