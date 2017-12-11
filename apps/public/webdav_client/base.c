#include "base.h"

char *my_str_malloc(size_t len){

    char *s;
    s = (char *)malloc(sizeof(char)*len);
    if(s == NULL)
    {
        printf("Out of memory.\n");
        exit(1);
    }

    memset(s,'\0',sizeof(char)*len);
    return s;
}

static void *xmalloc_fatal(size_t size) {
    if (size==0) return NULL;
    fprintf(stderr, "Out of memory.");
    exit(1);
}

void *xmalloc (size_t size) {
    void *ptr = malloc (size);
    if (ptr == NULL) return xmalloc_fatal(size);
    return ptr;
}

void *xrealloc (void *ptr, size_t size) {
    void *p = realloc (ptr, size);
    if (p == NULL) return xmalloc_fatal(size);
    return p;
}

char *xstrdup (const char *s) {
    void *ptr = xmalloc(strlen(s)+1);
    strcpy (ptr, s);
    return (char*) ptr;
}

/**
 * Escape 'string' according to RFC3986 and
 * http://oauth.net/core/1.0/#encoding_parameters.
 *
 * @param string The data to be encoded
 * @return encoded string otherwise NULL
 * The caller must free the returned string.
 */
char *oauth_url_escape(const char *string) {
    size_t alloc, newlen;
    char *ns = NULL, *testing_ptr = NULL;
    unsigned char in;
    size_t strindex=0;
    size_t length;

    if (!string) return xstrdup("");

    alloc = strlen(string)+1;
    newlen = alloc;

    ns = (char*) xmalloc(alloc);

    length = alloc-1;
    while(length--) {
        in = *string;

        switch(in){
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_': case '~': case '.': case '-':
            ns[strindex++]=in;
            break;
        default:
            newlen += 2; /* this'll become a %XX */
            if(newlen > alloc) {
                alloc *= 2;
                testing_ptr = (char*) xrealloc(ns, alloc);
                ns = testing_ptr;
            }
            snprintf(&ns[strindex], 4, "%%%02x", in);
            strindex+=3;
            break;
        }
        string++;
    }
    ns[strindex]=0;
    return ns;
}

#ifndef ISXDIGIT
# define ISXDIGIT(x) (isxdigit((int) ((unsigned char)x)))
#endif

/**
 * Parse RFC3986 encoded 'string' back to  unescaped version.
 *
 * @param string The data to be unescaped
 * @param olen unless NULL the length of the returned string is stored there.
 * @return decoded string or NULL
 * The caller must free the returned string.
 */
char *oauth_url_unescape(const char *string, size_t *olen) {
    size_t alloc, strindex=0;
    char *ns = NULL;
    unsigned char in;
    long hex;

    if (!string) return NULL;
    alloc = strlen(string)+1;
    ns = (char*) xmalloc(alloc);

    while(--alloc > 0) {
        in = *string;
        if(('%' == in) && ISXDIGIT(string[1]) && ISXDIGIT(string[2])) {
            char hexstr[3]; // '%XX'
            hexstr[0] = string[1];
            hexstr[1] = string[2];
            hexstr[2] = 0;
            hex = strtol(hexstr, NULL, 16);
            in = (unsigned char)hex; /* hex is always < 256 */
            string+=2;
            alloc-=2;
        }
        ns[strindex++] = in;
        string++;
    }
    ns[strindex]=0;
    if(olen) *olen = strindex;
    return ns;
}

static void s_progress(void *userdata, ne_off_t prog, ne_off_t total){
    if(prog_total == 0)
    {
        time_t t;
        t = time(NULL);
        DEBUG("progress1 = %.2f\n",(float)(1));
        prog_time = t;
    }

    if(prog_total != -1 && prog_total != 0)
    {
        time_t t;
        t = time(NULL);
        if((t-prog_time >= 5) || prog == prog_total)
        {
            DEBUG("progress2 = %.2f\n",(float)prog/prog_total);
            prog_time = t;
        }
    }
}

static int my_verify(void *userdata,int failures,const ne_ssl_certificate *cert)
{
    return 0;
}


/*大小写转换
 *type == 0,小写转成大写
 *type ==1,大写转成小写*/
int Change(char *s, int type)
{
    char *str = s;
    char *res = s;
    if (type == 0)
    {
        while(*res) *str++ = toupper(*res++);
    }
    else
    {
        while(*res) *str++ = tolower(*res++);
    }

    return 0;
}

/*Used for create Session*/
int wd_create_session(int j){
    if (ne_sock_init())
    {
        printf(" Socket library initalization failed.\n");
        return -1;
    }
    ne_uri uri = {0};

    Change(asus_cfg.prule[j]->host,1);

    DEBUG("asus_cfg.prule[j]->host= %s\n",asus_cfg.prule[j]->host);

    if (ne_uri_parse(asus_cfg.prule[j]->host, &uri) || uri.host==NULL || uri.path==NULL)
    {
        printf("Could not parse url %s/n", asus_cfg.prule[j]->host);
        return -1;
    }
    asus_cfg.prule[j]->sess = ne_session_create(uri.scheme, uri.host, uri.port);

    if(!strcmp(uri.scheme,"https"))
    {
        DEBUG("https session\n");
        ne_ssl_set_verify(asus_cfg.prule[j]->sess,my_verify,uri.host);
    }

    ne_set_useragent(asus_cfg.prule[j]->sess,"");
    ne_set_connect_timeout(asus_cfg.prule[j]->sess,80);
    ne_set_read_timeout(asus_cfg.prule[j]->sess,80);
    ne_set_progress(asus_cfg.prule[j]->sess, s_progress, NULL);

    ne_uri_free(&uri);

    return 0;
}

int is_local_space_enough(CloudFile *do_file,int index){
    long long int freespace;
    freespace = get_local_freespace(index);

    DEBUG("do_file->getcontentlength = %s\n",do_file->getcontentlength);
    DEBUG("freespace = %lld,do_file->getcontentlength = %lld\n",freespace,atoll(do_file->getcontentlength));

    if(freespace <= atoll(do_file->getcontentlength)){
        DEBUG("local freespace is not enough!\n");
        return 0;
    }
    else
    {
        DEBUG("local freespace is enough!\n");
        return 1;
    }
}

int is_server_space_enough(const char *localfilepath,int index){
    int status;
    long long int filesize;
    status = ne_getrouterinfo(wd_parsexml_RouterInfo,index);

    filesize = stat_file(localfilepath);
    DEBUG("DiskAvailable = %lld,filesize = %lld\n",DiskAvailable,filesize);
    if(status == 0)
    {
        if(DiskAvailable > filesize)
        {
            DEBUG("server freespace is enough!\n");
            return 1;
        }
        else
        {
            DEBUG("server freespace is not enough!\n");
            return 0;
        }
    }
    return status;

}

int get_filename_length(char *filepath){

    const char split = '/';
    char *p;
    int len;

    p = strrchr(filepath,split);

    if(p)
    {
        p++;
        len = strlen(p);
        return len;
    }

    return 0;
}

char *parse_name_from_path(const char *path)
{
    char *name;
    char *p;

    name = (char *)malloc(sizeof(char)*512);
    memset(name,0,sizeof(char)*512);

    p = strrchr(path,'/');

    if( p == NULL)
    {
        free(name);
        return NULL;
    }

    p++;

    snprintf(name, sizeof(char)*512, "%s", p);

    return name;
}

char *parse_parentfolder_from_path(const char *path)
{
    char *filename = parse_name_from_path(path);

    int len = 0;
    len = strlen(filename);

    char *parentfolder = my_str_malloc((size_t)(strlen(path)-len+1));

    snprintf(parentfolder,strlen(path)-len+1,"%s",path);

    free(filename);
    return parentfolder;
}

char *change_local_same_name(char *fullname)
{
    int i = 1;
    char *temp_name = NULL;
    int len = 0;
    char *path;
    char newfilename[256];

    char *fullname_tmp = NULL;
    fullname_tmp = my_str_malloc(strlen(fullname)+1);
    snprintf(fullname_tmp, sizeof(char)*(strlen(fullname)+1), "%s",fullname);

    char *filename = parse_name_from_path(fullname_tmp);
    len = strlen(filename);
    DEBUG("filename len is %d\n",len);
    path = my_str_malloc((size_t)(strlen(fullname)-len+1));
    DEBUG("fullname = %s\n",fullname);
    snprintf(path,strlen(fullname)-len+1,"%s",fullname);
    DEBUG("path = %s\n",path);
    free(fullname_tmp);

    while(1)
    {
        int n = i;
        int j = 0;
        while((n=(n/10)))
        {
            j++;
        }
        memset(newfilename,'\0',sizeof(newfilename));
        snprintf(newfilename,252-j,"%s",filename);
        snprintf(newfilename, 256, "%s(%d)",newfilename,i);
        DEBUG("newfilename = %s\n",newfilename);

        i++;

        temp_name = my_str_malloc((size_t)(strlen(path)+strlen(newfilename)+1));
        snprintf(temp_name, sizeof(char)*(strlen(path)+strlen(newfilename)+1), "%s%s",path,newfilename);

        if(access(temp_name,F_OK) != 0)
            break;
        else
            free(temp_name);
    }

    free(path);
    free(filename);
    return temp_name;
}

char *change_server_same_name(char *fullname,int index){

    int i = 1;
    int exist;
    char *filename = NULL;
    char *temp_name = NULL;
    int len = 0;
    char *path;
    char newfilename[512];
    int exit = 1;

    char *fullname_tmp = NULL;
    fullname_tmp = my_str_malloc(strlen(fullname)+1);
    snprintf(fullname_tmp, sizeof(char)*(strlen(fullname)+1), "%s",fullname);
    filename = parse_name_from_path(fullname_tmp);
    len = strlen(filename);
    DEBUG("filename len is %d\n",len);
    path = my_str_malloc((size_t)(strlen(fullname)-len+1));
    DEBUG("fullname = %s\n",fullname);
    snprintf(path,strlen(fullname)-len+1,"%s",fullname);
    DEBUG("path = %s\n",path);

    free(fullname_tmp);

    while(exit)
    {
        int n = i;
        int j = 0;
        while((n=(n/10)))
        {
            j++;
        }
        memset(newfilename,'\0',sizeof(newfilename));
        snprintf(newfilename,252-j,"%s",filename);
        snprintf(newfilename, 512, "%s(%d)",newfilename,i);

        DEBUG("newfilename = %s\n",newfilename);

        i++;

        temp_name = my_str_malloc((size_t)(strlen(path)+strlen(newfilename)+1));
        snprintf(temp_name, sizeof(char)*(strlen(path)+strlen(newfilename)+1), "%s%s",path,newfilename);
        DEBUG("temp_name = %s\n",temp_name);

        exist = is_server_exist(path,temp_name,index);

        if(exist)
        {
            free(temp_name);
        }
        else
        {
            exit = 0;
        }

    }

    free(path);
    free(filename);
    return temp_name;

}

char *get_temp_name(char *fullname)
{
    char *temp_name = NULL;
    char *temp_suffix = ".asus.td";
    int len = 0;
    char *path;
    char newfilename[256];

    memset(newfilename,0,sizeof(newfilename));
    char *filename = parse_name_from_path(fullname);
    len = strlen(filename);

    DEBUG("filename len is %d\n",len);
    path = my_str_malloc((size_t)(strlen(fullname)-len));

    if(len > 247)
    {
        strncpy(path,fullname,strlen(fullname)-len-1);
        strncpy(newfilename,filename,247);
        temp_name = my_str_malloc((strlen(path)+strlen("/")+strlen(newfilename)+strlen(temp_suffix)+1));
        snprintf(temp_name, sizeof(char)*(strlen(path)+strlen("/")+strlen(newfilename)+strlen(temp_suffix)+1), "%s/%s%s",path,newfilename,temp_suffix);
    }
    else
    {
        temp_name = my_str_malloc(strlen(fullname)+strlen(temp_suffix)+1);
        snprintf(temp_name, sizeof(char)*(strlen(fullname)+strlen(temp_suffix)+1), "%s%s",fullname,temp_suffix);
    }

    free(path);
    free(filename);
    return temp_name;
}

int Download(char *dofile_href,int index){

    DEBUG("**********Download****************\n");

    char *LocalFilePath;
    char *LocalFilePath_temp;
    char *path;

    path = oauth_url_unescape(dofile_href,NULL);
    LocalFilePath = my_str_malloc((size_t)(strlen(path)+asus_cfg.prule[index]->base_path_len+1));
    snprintf(LocalFilePath, sizeof(char)*(strlen(path)+asus_cfg.prule[index]->base_path_len+1), "%s%s",asus_cfg.prule[index]->base_path,path);
    LocalFilePath_temp = get_temp_name(LocalFilePath);
    DEBUG("LocalFilePath_temp = %s\n",LocalFilePath_temp);

    free(path);

    if(access(LocalFilePath,F_OK) == 0)
    {

        DEBUG("Local has %s\n",LocalFilePath);

        unlink(LocalFilePath);
        add_action_item("remove",LocalFilePath,g_pSyncList[index]->server_action_list);
    }

    char *serverPath;
    serverPath = my_str_malloc(strlen(dofile_href)+strlen(asus_cfg.prule[index]->rootfolder)+1);
    snprintf(serverPath, sizeof(char)*(strlen(dofile_href)+strlen(asus_cfg.prule[index]->rootfolder)+1), "%s%s",asus_cfg.prule[index]->rootfolder,dofile_href);
    int cp = 0;
    DEBUG("serverPath = %s\n",serverPath);

    do{
        cp = is_Server_Copying(serverPath,index);
    }while(cp == 1 && !exit_loop);

    if(cp != 0)
    {
        free(serverPath);
        free(LocalFilePath);
        free(LocalFilePath_temp);
        if(cp == -1)
            return SERVER_FILE_DELETED;
        return cp;
    }

    int ret,fd;
    fd = open(LocalFilePath_temp,O_WRONLY | O_CREAT,S_IRWXU | S_IRWXG | S_IRWXO);
    write_log(S_DOWNLOAD,"",LocalFilePath,index);
    ret = ne_get(asus_cfg.prule[index]->sess,serverPath,fd);

    free(serverPath);
    prog_total = -1;

    if(ret != 0){

        DEBUG("get file error!\n");

        char derror_info[512];
        wd_set_error(derror_info,512,"download %s fail !",LocalFilePath);
        write_log(S_ERROR,derror_info,"",index);

        write_error_log(derror_info);

        char error_info[100];
        memset(error_info,0,sizeof(error_info));
        snprintf(error_info, 100, "%s",ne_get_error(asus_cfg.prule[index]->sess));

        DEBUG("%s\n",error_info);
        close(fd);
        if(ret == NE_WEBDAV_QUIT && del_rule_num == index)
        {
            unlink(LocalFilePath_temp);
        }
        free(LocalFilePath);
        free(LocalFilePath_temp);

        if(strstr(error_info,"Could not connect to server") != NULL)
        {
            return COULD_NOT_CONNECNT_TO_SERVER;
        }
        if(strstr(error_info,"Connection timed out") != NULL || strstr(error_info,"connection timed out") != NULL)
        {
            return CONNECNTION_TIMED_OUT;
        }
        if(strstr(error_info,"Could not read response body") != NULL)
        {
            return COULD_NOT_READ_RESPONSE_BODY;
        }


        return ret;
    }
    close(fd);

    char *newLocalFilePath,*newLocalFilePath_temp;
    int path_changed = 0;
    if(access(LocalFilePath_temp,F_OK) != 0)
    {
        newLocalFilePath = search_newpath(LocalFilePath,index);
        newLocalFilePath_temp = search_newpath(LocalFilePath_temp,index);
        DEBUG("newLocalFilePath = %s\n",newLocalFilePath);
        DEBUG("newLocalFilePath_temp = %s\n",newLocalFilePath_temp);
        path_changed = 1;
    }

    if(path_changed)
    {
        rename(newLocalFilePath_temp,newLocalFilePath);
        free(newLocalFilePath_temp);
        free(newLocalFilePath);
    }
    else
    {
        rename(LocalFilePath_temp,LocalFilePath);
    }

    free(LocalFilePath);
    free(LocalFilePath_temp);

    if(finished_initial)
        write_log(S_SYNC,"","",index);
    else
        write_log(S_INITIAL,"","",index);
    return ret;
}

int strnicmp(const char *s1, const char *s2, size_t len){

    /* Yes, Virginia, it had better be unsigned */

    unsigned char c1, c2;



    c1 = c2 = 0;

    if (len) {

        do {

            c1 = *s1;

            c2 = *s2;

            s1++;

            s2++;

            if (!c1)

                break;

            if (!c2)

                break;

            if (c1 == c2)

                continue;

            c1 = tolower(c1);

            c2 = tolower(c2);

            if (c1 != c2)

                break;

        } while (--len);

    }

    return (int)c1 - (int)c2;

}


/*upload the file*/
int auth_put(const char *uri, const char *buf,long int bufsize,long long int start,long long int stop,long long int filesize,int index)
{
    ne_request *req;
    int ret;
    char range[128];
    snprintf(range, 128, "bytes %lld-%lld/%lld",start,stop,filesize);

    req = ne_request_create(asus_cfg.prule[index]->sess,"PUT",uri);
    ne_add_request_header(req,"Content-Range",range);
    ne_set_request_body_buffer(req, buf,bufsize);

    ret = ne_request_dispatch(req);

    if (ret == NE_OK && ne_get_status(req)->klass != 2)
        ret = NE_ERROR;

    ne_request_destroy(req);

    if(ret == NE_OK)
    {
        time_t t;
        t = time(NULL);
        if((t-prog_time >= 5) || ((stop+1) == filesize))
        {
            if(filesize == 0)
            {
                DEBUG("progress = %.2f\n",(float)(1));
                prog_time = t;
            }
            else
            {
                DEBUG("progress = %.2f\n",(float)(stop+1)/filesize);
                prog_time = t;
            }
        }
    }

    return ret;
}

/*cut the upload file*/
int my_put(char *filepath,char *serverpath,int index){
    int fd;
    FILE *fp;
    int ret;
    long int max_buf_size = 51200;
    long long int file_size;
    long long int left_file_size;
    long long int range_start = 0;
    long long int range_stop = 0;
    int i=0;
    char buffer[51200];

    fd = open(filepath,O_RDONLY);

    struct stat filestat;

    if(fstat(fd,&filestat)==-1)
    {
        return NE_ERROR;
    }

    file_size = filestat.st_size;
    left_file_size = filestat.st_size;

    close(fd);

    if(access(filepath,F_OK) != 0)
    {
        DEBUG("Local has no %s\n",filepath);
        return LOCAL_FILE_LOST;
    }

    if((fp = fopen(filepath,"r")) == NULL)
    {
        printf("open file fail\n");
        return -1;
    }

    if(file_size <= max_buf_size)
    {
        range_stop = file_size -1;
        fread(buffer,5120,10,fp);
        ret = auth_put(serverpath,buffer,file_size,range_start,range_stop,file_size,index);
    }

    else
    {
        while(left_file_size > 0)
        {
            if(left_file_size <= max_buf_size)
            {
                range_start = max_buf_size*i;
                range_stop = file_size -1;
                fread(buffer,5120,10,fp);
                ret = auth_put(serverpath,buffer,left_file_size,range_start,range_stop,file_size,index);
                if(ret != 0)
                {
                    DEBUG("Put file error!\n");
                    fclose(fp);
                    return ret;
                }
                left_file_size = 0;
            }
            else
            {
                range_start = max_buf_size*i;
                range_stop = range_start + max_buf_size -1;
                left_file_size = file_size - range_stop - 1;
                fread(buffer,5120,10,fp);
                ret = auth_put(serverpath,buffer,max_buf_size,range_start,range_stop,file_size,index);
                ++i;
                if(ret != 0)
                {
                    DEBUG("Put file error!\n");
                    fclose(fp);
                    return ret;
                }
            }

        }
    }

    DEBUG("ret = %d\n",ret);
    fclose(fp);
    if(ret != 0)
    {
        DEBUG("Put file error!\n");
        return ret;
    }

    return ret;
}

int Upload(char *filepath,int index){

    DEBUG("*****************   Upload : %s    ***************\n",filepath);

    int server_enough;
    if(access(filepath,F_OK) != 0)
    {
        DEBUG("Local has no %s\n",filepath);

        char error_info[512];
        wd_set_error(error_info,512,"upload %s fail,file is not exist",filepath);
        write_log(S_ERROR,error_info,"",index);
        write_error_log(error_info);
        return LOCAL_FILE_LOST;
    }

    server_enough = is_server_space_enough(filepath,index);
    if(server_enough != 1)
    {
        if(server_enough == 0)
        {
            char error_info[512];
            wd_set_error(error_info,512,"upload %s fail,server space is not enough!",filepath);
            write_log(S_ERROR,"server space is not enough!","",index);
            write_error_log(error_info);
            action_item *item;
            item = get_action_item("upload",filepath,g_pSyncList[index]->up_space_not_enough_list,index);
            if(item == NULL)
            {
                add_action_item("upload",filepath,g_pSyncList[index]->up_space_not_enough_list);
            }
            return SERVER_SPACE_NOT_ENOUGH;
        }
        else
        {
            return server_enough;
        }
    }

    char *localpath;
    localpath = strstr(filepath,asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
    int ret;

    //如果本地同名文件比Server小，那么就无法覆盖这个Server上的文件，因为上传是一段一段上传的，所以先删除Server上的同名文件
    //Delete(filepath);

    char *serverpath;
    serverpath = localpath_to_serverpath(filepath,index);
    DEBUG("Upload -> serverpath = %s\n",serverpath);
    write_log(S_UPLOAD,"",filepath,index);
    ret = my_put(filepath,serverpath,index);
    free(serverpath);

    if(ret != 0)
    {
        DEBUG("Put file error! ret = %d\n",ret);
        char error_info[200];
        memset(error_info,0,sizeof(error_info));
        snprintf(error_info, 200, "%s",ne_get_error(asus_cfg.prule[index]->sess));

        DEBUG("%s\n",error_info);

        char derror_info[512];
        wd_set_error(derror_info,512,"upload %s fail !",filepath);
        write_error_log(derror_info);

        if(strstr(error_info,"507") != NULL)
        {
            Delete(filepath,index);
            write_log(S_ERROR,"server space is not enough!","",index);
            action_item *item;
            item = get_action_item("upload",filepath,g_pSyncList[index]->up_space_not_enough_list,index);
            if(item == NULL)
            {
                add_action_item("upload",filepath,g_pSyncList[index]->up_space_not_enough_list);
            }
            return SERVER_SPACE_NOT_ENOUGH;
        }
        if(strstr(error_info,"Could not connect to server") != NULL)
        {
            Delete(filepath,index);
            action_item *item;
            item = get_action_item("upload",filepath,g_pSyncList[index]->unfinished_list,index);
            if(item == NULL)
            {
                add_action_item("upload",filepath,g_pSyncList[index]->unfinished_list);
            }

            write_log(S_ERROR,derror_info,"",index);
            return COULD_NOT_CONNECNT_TO_SERVER;
        }
        if(strstr(error_info,"Connection timed out") != NULL || strstr(error_info,"connection timed out") != NULL)
        {
            Delete(filepath,index);
            action_item *item;
            item = get_action_item("upload",filepath,g_pSyncList[index]->unfinished_list,index);
            if(item == NULL)
            {
                add_action_item("upload",filepath,g_pSyncList[index]->unfinished_list);
            }
            write_log(S_ERROR,derror_info,"",index);
            return CONNECNTION_TIMED_OUT;
        }
        if(strstr(error_info,"Invalid argument") != NULL)
        {
            Delete(filepath,index);
            action_item *item;
            item = get_action_item("upload",filepath,g_pSyncList[index]->unfinished_list,index);
            if(item == NULL)
            {
                add_action_item("upload",filepath,g_pSyncList[index]->unfinished_list);
            }
            return INVALID_ARGUMENT;
        }
        if(strstr(error_info,"427") != NULL)
        {
            Delete(filepath,index);
            char *filename = parse_name_from_path(filepath);
            int len = strlen(filename);

            DEBUG("filename = %s\nlen = %d\n",filename,len);

            char *path = my_str_malloc((size_t)(strlen(filepath)-len+1));

            snprintf(path,strlen(filepath)-len,"%s",filepath);
            ret = Mkcol(path,index);

            free(path);
            free(filename);
            return NO_PARENT;

        }
        if(ret == NE_WEBDAV_QUIT)
        {
            FILE *fp;
            fp = fopen(g_pSyncList[index]->up_item_file,"w");
            if(fp == NULL)
            {
                DEBUG("open %s error\n",g_pSyncList[index]->up_item_file);
                return ret;
            }
            fprintf(fp,"%s",localpath);
            fclose(fp);
            return ret;
        }
        return ret;
    }

    del_action_item("upload",filepath,g_pSyncList[index]->unfinished_list);
    del_action_item("upload",filepath,g_pSyncList[index]->up_space_not_enough_list);

    usleep(1000*100);
    return ret;
}

int Delete(char *filepath,int index){

    DEBUG("*************Delete*****************\n");

    int ret;
    char *serverpath;
    serverpath = localpath_to_serverpath(filepath,index);

    DEBUG("serverpath = %s\n",serverpath);

    ret = ne_delete(asus_cfg.prule[index]->sess,serverpath);

    free(serverpath);

    if(ret != 0){
        DEBUG("Delete file error!\n");

        char error_info[100];
        memset(error_info,0,sizeof(error_info));
        snprintf(error_info, 100, "%s",ne_get_error(asus_cfg.prule[index]->sess));

        DEBUG("%s\n",error_info);

        if(strstr(error_info,"404") != NULL)
        {
            return SERVER_FILE_DELETED;
        }

        return ret;
    }

    return 0;
}

int test_if_dir(const char *dir){

    struct stat buf;
    if(stat(dir,&buf) < 0)
    {
        return 0;
    }

    if(S_ISDIR(buf.st_mode))
    {
        return 1;
    }
    return 0;
}

int test_if_dir_empty(char *dir)
{
    struct dirent* ent = NULL;
    DIR *pDir;
    int i = 0;
    pDir=opendir(dir);

    if(pDir != NULL )
    {
        while (NULL != (ent=readdir(pDir)))
        {

            if(ent->d_name[0] == '.')
                continue;
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;
            i++;

        }
        closedir(pDir);
    }

    return  (i == 0) ? 1 : 0;
}

long long stat_file(const char *filename)
{
    struct stat filestat;
    if( stat(filename,&filestat) == -1)
    {
        perror("stat:");
        return -1;
    }
    return  filestat.st_size;
}

int is_file_copying(char *filename)
{
    long long size1,size2;
    size1 = stat_file(filename);
    usleep(1000*100);
    size2 = stat_file(filename);

    return (size2 - size1);
}

int createFolder(char *dir,int index)
{
    DEBUG("dir = %s\n",dir);

    int status;

    struct dirent *ent = NULL;
    DIR *pDir;
    pDir=opendir(dir);


    if(pDir != NULL )
    {
        status = Mkcol(dir,index);
        if(status != 0)
        {
            DEBUG("Create %s failed\n",dir);
            closedir(pDir);
            return status;
        }
        add_action_item("createfolder",dir,g_pSyncList[index]->dragfolder_action_list);
        //如果能返回文件或文件夹已存在信息，这边可以加入对create_folder_cmp是否为0的判断

        while ((ent=readdir(pDir)) != NULL)
        {
#if 1
            if(ent->d_name[0] == '.')
                continue;
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;

            char *fullname;
            fullname = my_str_malloc(strlen(dir)+strlen(ent->d_name)+2);
            snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);
            if(test_if_dir(fullname) == 1)
            {
                status = createFolder(fullname,index);
            	if(status != 0)
                {
                    DEBUG("CreateFolder %s failed\n",fullname);
                    free(fullname);
                    closedir(pDir);
                    return status;
            	}
            }
            else
            {
                status = Upload(fullname,index);
                if(status == 0)
                {
                    char *serverpath;
                    serverpath = localpath_to_serverpath(fullname,index);
                    time_t modtime;
                    modtime = Getmodtime(serverpath,index);
                    if(modtime != -1)
                        ChangeFile_modtime(fullname,modtime,index);
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                    }
                    free(serverpath);
                    add_action_item("createfile",fullname,g_pSyncList[index]->dragfolder_action_list);
                }
                else if(status == SERVER_SPACE_NOT_ENOUGH)
                {
                    DEBUG("upload %s failed,server space is not enough!\n",fullname);
                }
                else
                {
                    DEBUG("upload %s failed\n",fullname);

                    free(fullname);
                    closedir(pDir);
                    return status;
                }         
            }
            free(fullname);
#endif
        }
        closedir(pDir);
        return 0;
    }
    else
    {
        DEBUG("open %s fail \n",dir);
        return LOCAL_FILE_LOST;
    }
}

int deal_dragfolder_to_socketlist(char *dir,int index)
{
    DEBUG("dir = %s\n",dir);

    int status;
    struct dirent *ent = NULL;
    char info[512];
    memset(info,0,sizeof(info));
    DIR *pDir;
    pDir=opendir(dir);
    if(pDir != NULL)
    {
        while((ent=readdir(pDir)) != NULL)
        {
            if(ent->d_name[0] == '.')
                continue;
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;

            char *fullname;

            fullname = my_str_malloc(strlen(dir)+strlen(ent->d_name)+2);
            snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);
            if(test_if_dir(fullname) == 1)
            {
                snprintf(info, 512, "createfolder\n%s\n%s",dir,ent->d_name);
                pthread_mutex_lock(&mutex_socket);
                add_socket_item(info,0,index);
                pthread_mutex_unlock(&mutex_socket);
                status = deal_dragfolder_to_socketlist(fullname,index);
            }
            else
            {
                snprintf(info, 512, "createfile\n%s\n%s",dir,ent->d_name);
                pthread_mutex_lock(&mutex_socket);
                add_socket_item(info,0,index);
                pthread_mutex_unlock(&mutex_socket);
            }
            free(fullname);
        }
        closedir(pDir);
        return 0;
    }
}


int Mkcol(char *folderpath,int index){

    DEBUG("****************Mkcol****************\n");

    int ret;

    if(access(folderpath,0) != 0)
    {
        DEBUG("Local has no %s\n",folderpath);
        return LOCAL_FILE_LOST;
    }

    if(strcmp(folderpath,asus_cfg.prule[index]->base_path) == 0)
    {
        write_log(S_ERROR,"Server Deleted Sync Folder!","",index);
        return SERVER_ROOT_DELETED;
    }

    int exist = 0;
    char *filename = parse_name_from_path(folderpath);
    int len = strlen(filename);

    DEBUG("filename = %s\nlen = %d\n",filename,len);

    char *path = my_str_malloc((size_t)(strlen(folderpath)-len+1));

    snprintf(path,strlen(folderpath)-len,"%s",folderpath);
    DEBUG("path = %s\n",path);
    exist = is_server_exist(path,folderpath,index);
    free(path);
    free(filename);

    if(exist)
    {
        return 0;
    }

    char *serverpath;
    serverpath = localpath_to_serverpath(folderpath,index);

    DEBUG("serverpath = %s\n",serverpath);


    ret = ne_mkcol(asus_cfg.prule[index]->sess,serverpath);

    free(serverpath);

    if(ret != 0){

        DEBUG("Create Folder error!\n");
        DEBUG("ERROR info: %s\n",ne_get_error(asus_cfg.prule[index]->sess));

        return ret;
    }

    return 0;
}


int Move(char *oldpath,char *newpath,int index){

    DEBUG("****************Move****************\n");

    char *server_oldpath;
    char *server_newpath;

    server_oldpath = localpath_to_serverpath(oldpath,index);
    server_newpath = localpath_to_serverpath(newpath,index);

    int ret;

    DEBUG("server_oldpath = %s\nserver_newpath = %s\n",server_oldpath,server_newpath);
    ret = ne_move(asus_cfg.prule[index]->sess,1,server_oldpath,server_newpath);
    DEBUG("move ret = %d\n",ret);

    if(ret != 0){
        DEBUG("move/rename %s to %s ERROR!\n",server_oldpath,server_newpath);
        char error_info[200];
        memset(error_info,0,sizeof(error_info));
        snprintf(error_info, 200, "%s",ne_get_error(asus_cfg.prule[index]->sess));
        DEBUG("ret = %d\nerror_info = %s\n",ret,error_info);

        if(strstr(error_info,"404") != NULL)
        {
            if(test_if_dir(newpath))
            {
                DEBUG("it is folder\n");

                ret = createFolder(newpath,index);
                if(ret != 0)
                {
                    DEBUG("Create %s failed\n",newpath);
                    free(server_oldpath);
                    free(server_newpath);
                    return ret;
                }
            }
            else
            {
                DEBUG("it is file\n");
                ret = Upload(newpath,index);
                if(ret != 0)
                {
                    free(server_oldpath);
                    free(server_newpath);
                    return ret;
                }
                else
                {
                    time_t modtime;
                    modtime = Getmodtime(server_newpath,index);
                    if(modtime != -1)
                        ChangeFile_modtime(newpath,modtime,index);
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                    }
                }
            }
        }
        if(strstr(error_info,"427") != NULL)
        {
            char *filename = parse_name_from_path(newpath);
            int len = strlen(filename);

            DEBUG("filename = %s\nlen = %d\n",filename,len);

            char *path = my_str_malloc((size_t)(strlen(newpath)-len+1));

            snprintf(path,strlen(newpath)-len,"%s",newpath);
            DEBUG("path = %s\n",path);
            Mkcol(path,index);

            free(path);
            free(filename);
            free(server_oldpath);
            free(server_newpath);
            return ret;

        }
    }

    free(server_oldpath);
    free(server_newpath);
    return 0;
}

time_t Getmodtime(char *serverhref,int index){
    time_t modtime;
    int ret;

    if(NULL == strstr(serverhref,asus_cfg.prule[index]->rootfolder))
    {
        char *serverpath;
        serverpath = my_str_malloc(asus_cfg.prule[index]->rootfolder_length+strlen(serverhref)+1);
        snprintf(serverpath, sizeof(char)*(asus_cfg.prule[index]->rootfolder_length+strlen(serverhref)+1), "%s%s",asus_cfg.prule[index]->rootfolder,serverhref);
        ret = ne_getmodtime(asus_cfg.prule[index]->sess,serverpath,&modtime);
        free(serverpath);
    }
    else
    {
        ret = ne_getmodtime(asus_cfg.prule[index]->sess,serverhref,&modtime);
    }

    if(ret != 0){

        DEBUG("getmodtime of %s ERROR!\n",serverhref);
        DEBUG("ERROR info: %s\n",ne_get_error(asus_cfg.prule[index]->sess));

        return (time_t)-1;
    }
    return modtime;
}

int ne_getContentLength(ne_session *ne_sess, const char *uri,long long int *length)
{
    ne_request *req = ne_request_create(ne_sess, "HEAD", uri);
    const char *value;
    int ret;

    ret = ne_request_dispatch(req);

    value = ne_get_response_header(req, "Content-Length");

    if (ret == NE_OK && ne_get_status(req)->klass != 2) {
        *length = -1;
        ret = NE_ERROR;
    }
    else if (value) {

        DEBUG("value = %s\n",value);
        *length = atoll(value);
    }
    else {
        *length = 0;
    }

    ne_request_destroy(req);

    return ret;
}

long long int GetContentLength(char *serverhref,int index){

    long long int length;

    DEBUG("GetContentLength serverhref = %s\n",serverhref);

    int ret;

    ret = ne_getContentLength(asus_cfg.prule[index]->sess,serverhref,&length);

    DEBUG("ret = %d,length = %lld\n",ret,length);

    if(ret != 0){
        char error_info[100];
        memset(error_info,0,sizeof(error_info));
        snprintf(error_info, 100, "%s",ne_get_error(asus_cfg.prule[index]->sess));

        DEBUG("%s\n",error_info);
        DEBUG("GetContentLength     of %s ERROR!\n",serverhref);

        if(strstr(error_info,"Could not connect to server") != NULL)
        {
            return (long long int)-2;
        }
        if(strstr(error_info,"Connection timed out") != NULL || strstr(error_info,"connection timed out") != NULL)
        {
            return (long long int)-2;
        }
        if(strstr(error_info,"404") != NULL)
        {
            return (long long int)-3;
        }

        return (long long int)-1;
    }
    return length;

}

int is_Server_Copying(char *serverhref,int index){

    long long int old_length;
    long long int new_length;
    long long int d_value;

    old_length = GetContentLength(serverhref,index);
    if(old_length == -1)
    {
        prog_total = -1;
        return -1;
    }
    if(old_length == -2)
    {
        prog_total = -1;
        return COULD_NOT_CONNECNT_TO_SERVER;
    }
    if(old_length == -3)
    {
        prog_total = -1;
        return SERVER_FILE_DELETED;
    }

    usleep(1000*1000);

    new_length = GetContentLength(serverhref,index);
    if(new_length == -1)
    {
        prog_total = -1;
        return -1;
    }
    if(new_length == -2)
    {
        prog_total = -1;
        return COULD_NOT_CONNECNT_TO_SERVER;
    }
    if(new_length == -3)
    {
        prog_total = -1;
        return SERVER_FILE_DELETED;
    }

    d_value = new_length - old_length;

    if(d_value == 0)
    {
        prog_total = new_length;
        return 0;
    }
    else
        return 1;

}

char *search_newpath(char *href,int i)
{
    char *ret = my_str_malloc(strlen(href) + 1);
    snprintf(ret, sizeof(char)*(strlen(href) + 1), "%s",href);
    queue_entry_t socket_execute;
    socket_execute = g_pSyncList[i]->SocketActionList_Priority->head;

    while(socket_execute != NULL)
    {
        char *p,*q;
        char split = '\n';
        char *change_start;
        char new_cmd_name[1024];
        char path0[512];
        char path1[512];
        char path2[256];
        char *newpath_part,*oldpath_part;

        bzero(path0,512);
        bzero(path1,512);
        bzero(path2,256);

        p = strchr(socket_execute->cmd_name,split);
        p++;

        q = strchr(p,split);

        if(p!=NULL)
        {
            strncpy(path0,p,strlen(p)-strlen(q));
        }
        else
        {
            return ret;
        }

        q++;
        p = strchr(q,split);

        if(q != NULL)
        {
            strncpy(path1,q,strlen(q)-strlen(p));
        }
        else
        {
            return ret;
        }

        p++;
        if(p != NULL)
        {
            strncpy(path2,p,strlen(p));
        }
        else
        {
            return ret;
        }

        if(!strncmp(socket_execute->cmd_name,"move0",5))    //move0
        {
            newpath_part = my_str_malloc(strlen(path0)+strlen(path2)+3);
            oldpath_part = my_str_malloc(strlen(path1)+strlen(path2)+3);
            snprintf(newpath_part, sizeof(char)*(strlen(path0)+strlen(path2)+3), "%s/%s/",path0,path2);
            snprintf(oldpath_part, sizeof(char)*(strlen(path1)+strlen(path2)+3), "%s/%s/",path1,path2);
        }
        else       //rename0
        {
            newpath_part = my_str_malloc(strlen(path0)+strlen(path2)+3);
            oldpath_part = my_str_malloc(strlen(path0)+strlen(path1)+3);
            snprintf(newpath_part, sizeof(char)*(strlen(path0)+strlen(path2)+3), "%s/%s/",path0,path2);
            snprintf(oldpath_part, sizeof(char)*(strlen(path0)+strlen(path1)+3), "%s/%s/",path0,path1);
        }

        if(strncmp(ret,oldpath_part,strlen(oldpath_part)) == 0)
        {
            memset(new_cmd_name,0,1024);
            change_start = ret + strlen(oldpath_part);
            snprintf(new_cmd_name, 1024, "%s%s",newpath_part,change_start);

            free(ret);
            ret = my_str_malloc(strlen(new_cmd_name)+1);
            snprintf(ret, sizeof(char)*(strlen(new_cmd_name)+1), "%s",new_cmd_name);
        }

        socket_execute = socket_execute->next_ptr;
    }

    return ret;
}

int ChangeFile_modtime(char *filepath,time_t servermodtime,int index){

    DEBUG("**************ChangeFile_modtime**********\n");
    int path_changed = 0;
    char *newpath;

    if(access(filepath,F_OK) != 0)
    {
        newpath = search_newpath(filepath,index);
        path_changed = 1;
    }

    struct utimbuf *ub;
    ub = (struct utimbuf *)malloc(sizeof(struct utimbuf));

    DEBUG("servermodtime = %lu\n",servermodtime);

    if(servermodtime == -1){

        DEBUG("ChangeFile_modtime ERROR!\n");
        return -1;
    }

    ub->actime = servermodtime;
    ub->modtime = servermodtime;

    if(path_changed)
    {
        utime(newpath,ub);
        free(newpath);
    }
    else
    {
        utime(filepath,ub);
    }

    free(ub);
    return 0;
}

int compareTime(time_t servermodtime ,char *localpath){

    DEBUG("************compareTime*************\n");

    struct stat buf;

    if(servermodtime == -1){

        DEBUG("compareTime ERROR!\n");
        return -1;
    }

    DEBUG("localpath = %s\n",localpath);

    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
    }
    time_t msec = buf.st_mtime;

    DEBUG("msec = %lu,servermodtime = %lu\n",msec,servermodtime);

    if(servermodtime > msec)
        return 0;
    else if(servermodtime < msec)
        return 1;
    else
        return 2;
}

void wd_escape(char *unescapepath,char *escapepath){
    char *p;
    const char *split = "/";
    unescapepath = unescapepath+1;

    p=strtok(unescapepath,split);
    while(p!=NULL)
    {
        p = oauth_url_escape(p);
        sprintf(escapepath,"%s/%s",escapepath,p);

        free(p);
        p=strtok(NULL,split);
    }

}

void del_all_items(char *dir,int index)
{
    struct dirent* ent = NULL;
    DIR *pDir;
    pDir=opendir(dir);

    if(pDir != NULL )
    {
        while (NULL != (ent=readdir(pDir)))
        {
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;

            char *fullname;
            size_t len;
            len = strlen(dir)+strlen(ent->d_name)+2;
            fullname = my_str_malloc(len);
            snprintf(fullname, sizeof(char)*len, "%s/%s",dir,ent->d_name);

            if(test_if_dir(fullname) == 1)
            {
                wait_handle_socket(index);
                del_all_items(fullname,index);
            }
            else
            {
                wait_handle_socket(index);
                add_action_item("remove",fullname,g_pSyncList[index]->server_action_list);
                remove(fullname);
            }

            free(fullname);

        }
        closedir(pDir);

        add_action_item("remove",dir,g_pSyncList[index]->server_action_list);
        remove(dir);
    }
    else
        printf("open %s fail \n",dir);
}

time_t GetFile_modtime(char *localpath){
    struct stat buf;
    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
        return 0;
    }
    time_t msec = buf.st_mtime;
    return msec;
}

long long int get_local_freespace(int index){
    /*************unit is B************/
    DEBUG("***********get %s freespace!***********\n",asus_cfg.prule[index]->base_path);

    long long int freespace = 0;
    struct statvfs diskdata;
    if(!statvfs(asus_cfg.prule[index]->base_path,&diskdata))
    {
        freespace = (long long)diskdata.f_bsize * (long long)diskdata.f_bavail;
        return freespace;
    }
    else
    {
        return 0;
    }
}

void free_CloudFile_item(CloudFile *head)
{
    CloudFile *p = head;
    while(p != NULL)
    {
        head = head->next;
        if(p->href != NULL)
        {
            free(p->href);
            free(p->getcontentlength);
        }
        free(p);
        p = head;
    }
}

void free_LocalFolder_item(LocalFolder *head)
{
    LocalFolder *p = head;
    while(p != NULL)
    {
        head = head->next;
        if(p->path != NULL)
        {
            free(p->path);
            free(p->name);
        }
        free(p);
        p = head;
    }
}

void free_LocalFile_item(LocalFile *head)
{
    LocalFile *p = head;
    while(p != NULL)
    {
        head = head->next;
        if(p->path != NULL)
        {
            free(p->path);
            free(p->name);
        }
        free(p);
        p = head;
    }
}

int write_log(int status, char *message, char *filename,int index)
{
    if(exit_loop)
    {
        return 0;
    }
    pthread_mutex_lock(&mutex_log);
    Log_struc log_s;
    FILE *fp;
    int mount_path_length;
    int ret;
    struct timeval now;
    struct timespec outtime;

    if(status == S_SYNC && exit_loop ==0)
    {
        ret = ne_getrouterinfo(wd_parsexml_RouterInfo,index);
    }

    long long int totalspace = PreDiskAvailableShow+PreDiskUsedShow;

    mount_path_length = strlen(asus_cfg.prule[index]->mount_path);

    memset(&log_s,0,LOG_SIZE);

    log_s.status = status;

    fp = fopen(general_log,"w");

    if(fp == NULL)
    {
        DEBUG("open %s error\n",general_log);

        pthread_mutex_unlock(&mutex_log);
        return -1;
    }

    if(log_s.status == S_ERROR)
    {
        DEBUG("******** status is ERROR *******\n");

        snprintf(log_s.error, 512, "%s", message);
        fprintf(fp,"STATUS:%d\nERR_MSG:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,log_s.error,totalspace,PreDiskUsedShow,index);
    }
    else if(log_s.status == S_DOWNLOAD)
    {
        DEBUG("******** status is DOWNLOAD *******\n");

        snprintf(log_s.path, 512, "%s", filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,asus_cfg.prule[index]->mount_path,log_s.path+mount_path_length,totalspace,PreDiskUsedShow,index);
    }
    else if(log_s.status == S_UPLOAD)
    {
        DEBUG("******** status is UPLOAD *******\n");

        snprintf(log_s.path, 512, "%s", filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,asus_cfg.prule[index]->mount_path,log_s.path+mount_path_length,totalspace,PreDiskUsedShow,index);
    }
    else
    {
        if (log_s.status == S_INITIAL)
            DEBUG("******** other status is INIT *******\n");
        else
            DEBUG("******** other status is SYNC *******\n");
        fprintf(fp,"STATUS:%d\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,totalspace,PreDiskUsedShow,index);
    }

    fclose(fp);

    if(!exit_loop)
    {
        gettimeofday(&now, NULL);
        outtime.tv_sec = now.tv_sec + 3;
        outtime.tv_nsec = now.tv_usec * 1000;
        pthread_cond_timedwait(&cond_socket, &mutex_log, &outtime);
    }

    pthread_mutex_unlock(&mutex_log);
    return 0;
}

int write_error_log(char *message)
{
    FILE *fp;

    fp = fopen(ERRORLOG,"a");

    if(NULL == fp)
    {
        printf("open %s failed\n",ERRORLOG);
        return -1;
    }

    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    char *ctime = asctime(timeinfo);

    if( ctime[ strlen(ctime)-1 ] == '\n')
        ctime[ strlen(ctime)-1 ] = '\0';

    fprintf(fp,"%s webdav_client : %s\n",ctime,message);
    fclose(fp);

    //free(ctime);
    return 0;
}

int add_FolderList_item(CloudFile *head,char *item_href){

    DEBUG("***************add_FolderList_item*****************\n");

    CloudFile *p;
    p = head->next;
    CloudFile *q = (CloudFile *)malloc(sizeof(CloudFile));
    memset(q, 0, sizeof(CloudFile));

    q->href = my_str_malloc(strlen(item_href)+1);

    if(q == NULL)
        exit(-1);

    q->isFolder = 1;
    snprintf(q->href, sizeof(char)*(strlen(item_href)+1), "%s",item_href);

    if(p == NULL)
    {
        head->next = q;
        q->next = NULL;
    }
    else
    {
        q->next = p;
        head->next = q;
    }
    return 0;

}

int do_unfinished(int index){
    if(exit_loop)
    {
        return 0;
    }
    DEBUG("*************do_unfinished*****************\n");

    action_item *p,*p1;
    p = g_pSyncList[index]->unfinished_list->next;
    int ret;

    while(p != NULL)
    {
        if(!strcmp(p->action,"download"))
        {
            CloudFile *filetmp = NULL;
            filetmp = get_CloudFile_node(g_pSyncList[index]->ServerRootNode,p->path,0x2);
            if(filetmp == NULL)   //if filetmp == NULL,it means server has deleted filetmp
            {
                DEBUG("filetmp is NULL\n");

                p1 = p->next;
                del_action_item("download",p->path,g_pSyncList[index]->unfinished_list);
                p = p1;
                continue;
            }            
            char *localpath;
            localpath = serverpath_to_localpath(filetmp->href,index);

            DEBUG("localpath = %s\n",localpath);

            if(is_local_space_enough(filetmp,index))
            {
                add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                ret = Download(filetmp->href,index);
                if (ret == NE_OK)
                {
                    ChangeFile_modtime(localpath,filetmp->modtime,index);
                    p1 = p->next;
                    del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    p = p1;
                }
                else
                {
                    DEBUG("download %s failed",filetmp->href);

                    p = p->next;
                }
            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                p = p->next;
            }
            free(localpath);
        }
        else if(!strcmp(p->action,"upload"))
        {
            p1 = p->next;
            char *serverpath;
            serverpath = localpath_to_serverpath(p->path,index);
            char *path_temp;
            path_temp = my_str_malloc(strlen(p->path)+1);
            snprintf(path_temp, sizeof(char)*(strlen(p->path)+1), "%s",p->path);
            ret = Upload(p->path,index);

            DEBUG("********* uploadret = %d\n",ret);

            if(ret == NE_OK)
            {
                time_t modtime;
                modtime = Getmodtime(serverpath,index);
                if(modtime != -1)
                    ChangeFile_modtime(path_temp,modtime,index);
                else
                {
                    DEBUG("ChangeFile_modtime failed!\n");
                }
                p = p1;
            }
            else if(ret == LOCAL_FILE_LOST)
            {
                del_action_item("upload",p->path,g_pSyncList[index]->unfinished_list);
                p = p1;
            }
            else
            {
                DEBUG("upload %s failed",p->path);
                p = p->next;
            }
            free(serverpath);
            free(path_temp);
        }
        else
        {
            p = p->next;
        }
    }

    p = g_pSyncList[index]->up_space_not_enough_list->next;
    while(p != NULL)
    {
        p1 = p->next;
        DEBUG("up_space_not_enough_list\n");

        char *serverpath;
        serverpath = localpath_to_serverpath(p->path,index);
        char *path_temp;
        path_temp = my_str_malloc(strlen(p->path)+1);
        snprintf(path_temp, sizeof(char)*(strlen(p->path)+1), "%s",p->path);
        ret = Upload(p->path,index);

        DEBUG("########### uploadret = %d\n",ret);

        if(ret == NE_OK)
        {
            DEBUG("serverpath = %s\n",serverpath);

            time_t modtime;
            modtime = Getmodtime(serverpath,index);
            if(modtime != -1)
                ChangeFile_modtime(path_temp,modtime,index);
            else
            {
                DEBUG("ChangeFile_modtime failed!\n");
            }
            p = p1;
        }
        else if(ret == LOCAL_FILE_LOST)
        {
            del_action_item("upload",p->path,g_pSyncList[index]->up_space_not_enough_list);
            p = p1;
        }
        else
        {
            DEBUG("upload %s failed",p->path);
            p = p->next;
        }
        free(serverpath);
        free(path_temp);
    }
    return 0;
}

/*queue function*/
int queue_empty (queue_t q)
{
    return q->head == NULL;
}

queue_t queue_create ()
{
    queue_t q;
    q = malloc (sizeof (struct queue_struct));
    if (q == NULL)
        exit (-1);

    q->head = q->tail = NULL;
    return q;
}

void queue_enqueue (queue_entry_t d, queue_t q)
{
    d->next_ptr = NULL;
    if (q->tail)
    {
        q->tail->next_ptr = d;
        q->tail = d;
    }
    else
    {
        q->head = q->tail = d;
    }
}

queue_entry_t  queue_dequeue (queue_t q)
{
    queue_entry_t first = q->head;

    if (first)
    {
        q->head = first->next_ptr;
        if (q->head == NULL)
        {
            q->tail = NULL;
        }
        first->next_ptr = NULL;
    }
    return first;
}

void queue_destroy (queue_t q)
{
    if (q != NULL)
    {
        while (q->head != NULL)
        {
            queue_entry_t next = q->head;
            q->head = next->next_ptr;
            next->next_ptr = NULL;
            free(next->cmd_name);
            free(next);
        }
        q->head = q->tail = NULL;
        free (q);
    }
}

int is_copying_finished(char *filename)
{
    long long size1,size2;
    size1 = stat_file(filename);
    sleep(1);
    size2 = stat_file(filename);

    return (size2 - size1);
}

/*changed from serverpath to localpath*/
char *serverpath_to_localpath(char *from_serverpath,int index){
    char *hreftmp;
    char *to_localpath;

    hreftmp = oauth_url_unescape(from_serverpath,NULL);
    to_localpath = my_str_malloc(strlen(hreftmp)+asus_cfg.prule[index]->base_path_len+1);
    snprintf(to_localpath, sizeof(char)*(strlen(hreftmp)+asus_cfg.prule[index]->base_path_len+1), "%s%s",asus_cfg.prule[index]->base_path,hreftmp);
    free(hreftmp);
    DEBUG("serverpath_to_localpath to_localpath = %s\n",to_localpath);

    return to_localpath;
}

/*changed from localpath to serverpath
 *to_serverpath without http://xxx.xx.xx.xx:xx
 */
char *localpath_to_serverpath(char *from_localpath,int index){
    char *to_serverpath = NULL;
    char *pathtmp;
    char *tmp;
    char *p;
    const char *split = "/";
    char *strtok_tmp;
    int n;
    int len;

    len = strlen(from_localpath);

    if(len == asus_cfg.prule[index]->base_path_len)
    {
        tmp = my_str_malloc((size_t)asus_cfg.prule[index]->rootfolder_length+2);
        snprintf(tmp, sizeof(char)*(asus_cfg.prule[index]->rootfolder_length+2), "%s",asus_cfg.prule[index]->rootfolder);
        return tmp;
    }

    to_serverpath = (char *)malloc(sizeof(char));
    memset(to_serverpath,'\0',sizeof(to_serverpath));
    pathtmp = strstr(from_localpath,asus_cfg.prule[index]->base_path)+asus_cfg.prule[index]->base_path_len;
    pathtmp = pathtmp+1;
    strtok_tmp = my_str_malloc(strlen(pathtmp)+1);
    snprintf(strtok_tmp, sizeof(char)*(strlen(pathtmp)+1), "%s",pathtmp);
    p=strtok(strtok_tmp,split);
    while(p!=NULL)
    {
        p = oauth_url_escape(p);
        n = strlen(p);
        to_serverpath = realloc(to_serverpath, strlen(to_serverpath)+n+2);
        sprintf(to_serverpath,"%s/%s",to_serverpath,p);
        free(p);
        p=strtok(NULL,split);
    }
    tmp = my_str_malloc((size_t)strlen(to_serverpath)+asus_cfg.prule[index]->rootfolder_length+2);
    snprintf(tmp, sizeof(char)*(strlen(to_serverpath)+asus_cfg.prule[index]->rootfolder_length+2), "%s%s",asus_cfg.prule[index]->rootfolder,to_serverpath);
    free(to_serverpath);
    free(strtok_tmp);
    return tmp;
}

/*used for initial,local syncfolder is NULL*/
int initMyLocalFolder(Server_TreeNode *servertreenode,int index)
{
    int res = 0;

    if(servertreenode->browse != NULL)
    {
        CloudFile *init_foldercurrent = NULL;
        CloudFile *init_filecurrent = NULL;
        if(servertreenode->browse->foldernumber > 0)
            init_foldercurrent = servertreenode->browse->folderlist->next;
        if(servertreenode->browse->filenumber > 0)
            init_filecurrent = servertreenode->browse->filelist->next;
        int ret;

        while(init_foldercurrent != NULL && !exit_loop)
        {
            char *createpath;
            createpath = serverpath_to_localpath(init_foldercurrent->href,index);
            if(NULL == opendir(createpath))
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                if(-1 == mkdir(createpath,0777))
                {
                    printf("mkdir %s fail",createpath);
                    exit(-1);
                }
                else
                {
                    add_action_item("createfolder",createpath,g_pSyncList[index]->server_action_list);
                }
            }
            free(createpath);
            init_foldercurrent = init_foldercurrent->next;
        }

        while(init_filecurrent != NULL && !exit_loop)
        {
            if(is_local_space_enough(init_filecurrent,index))
            {
                char *createpath;
                createpath = serverpath_to_localpath(init_filecurrent->href,index);
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",createpath,g_pSyncList[index]->server_action_list);
                ret = Download(init_filecurrent->href,index);
                if (ret == NE_OK)
                {
                    ChangeFile_modtime(createpath,init_filecurrent->modtime,index);
                    free(createpath);
                }
                else
                {
                    free(createpath);
                    return ret;
                }
            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                add_action_item("download",init_filecurrent->href,g_pSyncList[index]->unfinished_list);
            }
            init_filecurrent = init_filecurrent->next;
        }
    }

    if(servertreenode->Child != NULL && !exit_loop)
    {
        res = initMyLocalFolder(servertreenode->Child,index);
        if(res != 0)
        {
            return res;
        }
    }

    if(servertreenode->NextBrother != NULL && !exit_loop)
    {
        res = initMyLocalFolder(servertreenode->NextBrother,index);
        if(res != 0)
        {
            return res;
        }
    }

    if(exit_loop)
        res = NE_WEBDAV_QUIT;

    return res;
}

int add_action_item(const char *action,const char *path,action_item *head){

    DEBUG("add_action_item,action = %s,path = %s\n",action,path);

    action_item *p1,*p2;

    p1 = head;

    p2 = (action_item *)malloc(sizeof(action_item));
    memset(p2,'\0',sizeof(action_item));
    p2->action = my_str_malloc(strlen(action)+1);
    p2->path = my_str_malloc(strlen(path)+1);
    snprintf(p2->action, sizeof(char)*(strlen(action)+1), "%s",action);
    snprintf(p2->path, sizeof(char)*(strlen(path)+1), "%s",path);

    while(p1->next != NULL)
        p1 = p1->next;

    p1->next = p2;
    p2->next = NULL;

    DEBUG("add action item OK!\n");

    return 0;
}

int del_action_item(const char *action,const char *path,action_item *head){

    action_item *p1,*p2;
    p1 = head->next;
    p2 = head;

    while(p1 != NULL)
    {
        if( !strcmp(p1->action,action) && !strcmp(p1->path,path))
        {
            p2->next = p1->next;
            free(p1->action);
            free(p1->path);
            free(p1);
            return 0;
        }
        p2 = p1;
        p1 = p1->next;
    }

    return 1;
}

void del_download_only_action_item(const char *action,const char *path,action_item *head)
{
    if(head == NULL)
    {
        return;
    }
    action_item *p1, *p2;
    char *cmp_name;
    char *p1_cmp_name;
    p1 = head->next;
    p2 = head;

    cmp_name = my_str_malloc((size_t)(strlen(path)+2));
    snprintf(cmp_name, sizeof(char)*(strlen(path)+2), "%s/",path);    //add for delete folder and subfolder in download only socket list

    while(p1 != NULL)
    {
        p1_cmp_name = my_str_malloc((size_t)(strlen(p1->path)+2));
        snprintf(p1_cmp_name, sizeof(char)*(strlen(p1->path)+2), "%s/",p1->path);      //add for delete folder and subfolder in download only socket list
        if(strstr(p1_cmp_name,cmp_name) != NULL)
        {
            p2->next = p1->next;
            free(p1->action);
            free(p1->path);
            free(p1);
            p1 = p2->next;
        }
        else
        {
            p2 = p1;
            p1 = p1->next;
        }
        free(p1_cmp_name);
    }
    free(cmp_name);
}

int add_all_download_only_socket_list(char *cmd,const char *dir,int index)
{
    struct dirent* ent = NULL;
    char *fullname;
    int fail_flag = 0;

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
        DEBUG("opendir %s fail",dir);
        fail_flag = 1;
        return -1;
    }

    add_action_item(cmd,dir,g_pSyncList[index]->download_only_socket_head);

    while (NULL != (ent=readdir(dp)))
    {

        if(ent->d_name[0] == '.')
            continue;
        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        fullname = my_str_malloc((size_t)(strlen(dir)+strlen(ent->d_name)+2));
        snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);

        if( test_if_dir(fullname) == 1)
        {
            add_all_download_only_socket_list("createfolder",fullname,index);
        }
        else
        {
            add_action_item("createfile",fullname,g_pSyncList[index]->download_only_socket_head);
        }
        free(fullname);
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}

int add_all_download_only_dragfolder_socket_list(const char *dir,int index)
{
    struct dirent* ent = NULL;
    char *fullname;
    int fail_flag = 0;

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
        DEBUG("opendir %s fail",dir);
        fail_flag = 1;
        return -1;
    }

    while (NULL != (ent=readdir(dp)))
    {

        if(ent->d_name[0] == '.')
            continue;
        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        fullname = my_str_malloc((size_t)(strlen(dir)+strlen(ent->d_name)+2));
        snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);

        if( test_if_dir(fullname) == 1)
        {
            add_action_item("createfolder",fullname,g_pSyncList[index]->dragfolder_action_list);
            add_action_item("createfolder",fullname,g_pSyncList[index]->download_only_socket_head);
            add_all_download_only_dragfolder_socket_list(fullname,index);
        }
        else
        {
            add_action_item("createfile",fullname,g_pSyncList[index]->dragfolder_action_list);
            add_action_item("createfile",fullname,g_pSyncList[index]->download_only_socket_head);
        }
        free(fullname);
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}

action_item *get_action_item(const char *action,const char *path,action_item *head,int index){

    action_item *p;
    p = head->next;

    while(p != NULL)
    {
        if(asus_cfg.prule[index]->rule == 1)
        {
            if(!strcmp(p->path,path))
            {
                return p;
            }
        }
        else
        {
            if(!strcmp(p->action,action) && !strcmp(p->path,path))
            {
                return p;
            }
        }
        p = p->next;
    }
    return NULL;
}

void free_action_item(action_item *head){

    action_item *point;
    point = head->next;

    while(point != NULL)
    {
        head->next = point->next;
        free(point->action);
        free(point->path);
        free(point);
        point = head->next;
    }
    free(head);
}

action_item *create_action_item_head(){

    action_item *head;

    head = (action_item *)malloc(sizeof(action_item));
    if(head == NULL)
    {
        printf("create memory error!\n");
        exit(-1);
    }
    memset(head,'\0',sizeof(action_item));
    head->next = NULL;

    return head;
}

int test_if_download_temp_file(char *filename)
{
    char file_suffix[9];
    char *temp_suffix = ".asus.td";
    memset(file_suffix,0,sizeof(file_suffix));
    char *p = filename;

    if(strstr(filename,temp_suffix))
    {
        snprintf(file_suffix, 9, "%s", p+(strlen(filename)-strlen(temp_suffix)));

        if(!strcmp(file_suffix,temp_suffix))
            return 1;
    }

    return 0;

}

int test_if_aicloud_temp_file(char *filename)
{
    char file_suffix[9];
    char *temp_suffix = ".aicloud.td";
    memset(file_suffix,0,sizeof(file_suffix));
    char *p = filename;

    if(strstr(filename,temp_suffix))
    {
        snprintf(file_suffix, 9, "%s", p+(strlen(filename)-strlen(temp_suffix)));
        if(!strcmp(file_suffix,temp_suffix))
            return 1;
    }

    return 0;
}

void handle_quit_upload()
{
    DEBUG("###handle_quit_upload###\n");

    int i;
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(g_pSyncList[i]->sync_disk_exist == 0)
            continue;
        if(asus_cfg.prule[i]->rule != 1)
        {
            FILE *fp;
            long long filesize;
            char *buf;
            char *filepath;
            int status;
            if(access(g_pSyncList[i]->up_item_file,0) == 0)
            {
                filesize = stat_file(g_pSyncList[i]->up_item_file);
                fp = fopen(g_pSyncList[i]->up_item_file,"r");
                if(fp == NULL)
                {
                    DEBUG("open %s error\n",g_pSyncList[i]->up_item_file);
                    return;
                }
                buf = my_str_malloc((size_t)(filesize+1));
                fscanf(fp,"%s",buf);
                fclose(fp);

                if((strlen(buf)) <=1 )
                {
                    return ;
                }

                unlink(g_pSyncList[i]->up_item_file);

                filepath = my_str_malloc((size_t)(filesize+asus_cfg.prule[i]->base_path_len+strlen(buf)+1));
                snprintf(filepath, sizeof(char)*(filesize+asus_cfg.prule[i]->base_path_len+strlen(buf)+1), "%s%s",asus_cfg.prule[i]->base_path,buf);
                free(buf);
                Delete(filepath,i);
                status = Upload(filepath,i);

                if(status != 0)
                {
                    DEBUG("upload %s failed\n",filepath);
                }
                else
                {
                    char *serverpath;
                    serverpath = localpath_to_serverpath(filepath,i);
                    time_t modtime;
                    modtime = Getmodtime(serverpath,i);
                    if(modtime != -1)
                        ChangeFile_modtime(filepath,modtime,i);
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                    }
                    free(serverpath);
                }

                free(filepath);
            }
        }
    }
}

void my_mkdir(char *path)
{
    DIR *dir;
    if(NULL == (dir = opendir(path)))
    {
        if(-1 == mkdir(path,0777))
        {
            printf("please check disk can write or dir has exist???");
            printf("mkdir %s fail\n",path);
            return;
        }
    }
    else
        closedir(dir);
}

void my_mkdir_r(char *path,int index)
{
    int i,len;
    char str[512];
    char fullname[512];
    char *temp;

    memset(str,0,sizeof(str));

    temp = strstr(path,asus_cfg.prule[index]->mount_path);

    len = strlen(asus_cfg.prule[index]->mount_path);
    snprintf(str, 512, "%s", temp+len);

    len = strlen(str);
    for(i=0; i < len ; i++)
    {
        if(str[i] == '/' && i != 0)
        {
            str[i] = '\0';
            memset(fullname,0,sizeof(fullname));
            snprintf(fullname, 512, "%s%s",asus_cfg.prule[index]->mount_path,str);
            if(access(fullname,F_OK) != 0)
            {
                my_mkdir(fullname);
            }
            str[i] = '/';
        }
    }


    memset(fullname,0,sizeof(fullname));
    snprintf(fullname, 512, "%s%s",asus_cfg.prule[index]->mount_path,str);

    if(len > 0 && access(fullname,F_OK) != 0)
    {
        my_mkdir(fullname);
    }
}

/*0,local file newer
 *1,server file newer
 *2,local time == server time
 *-1,get server modtime or local modtime failed
**/
int newer_file(char *localpath,int index){

    char *serverpath;
    serverpath = localpath_to_serverpath(localpath,index);
    time_t modtime1,modtime2;
    modtime1 = Getmodtime(serverpath,index);
    free(serverpath);
    if(modtime1 == -1)
    {
        DEBUG("newer_file Getmodtime failed!\n");
        return -1;
    }

    struct stat buf;

    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
        return -1;
    }
    modtime2 = buf.st_mtime;

    if(modtime2 > modtime1)
        return 0;
    else if(modtime2 == modtime1)
        return 2;
    else
        return 1;

}

/*0,local time != server time
 *1,local time - server time = (1 || -1)
 *2,local time == server time
 *-1,get server modtime or local modtime failed
**/
int init_newer_file(char *localpath,int index){

    char *serverpath;
    serverpath = localpath_to_serverpath(localpath,index);
    time_t modtime1,modtime2,modtime3;
    modtime1 = Getmodtime(serverpath,index);
    free(serverpath);
    if(modtime1 == -1)
    {
        DEBUG("newer_file Getmodtime failed!\n");
        return -1;
    }

    struct stat buf;

    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
        return -1;
    }
    modtime2 = buf.st_mtime;
    modtime3 = modtime1 - modtime2;

    if(modtime3 == 0)
    {
        return 2;
    }
    else if(modtime3 == 1 || modtime3 == -1)
    {
        DEBUG("%s init change file time 111111111111111111111111111111111\n",localpath);
        ChangeFile_modtime(localpath,modtime1,index);
        return 1;
    }
    else
    {
        return 0;
    }
}

/*0,local time == server time
 *1,local modify
 *2,server modify
 *3,local and server modify
 *-1,get server modtime or local modtime failed
**/
int sync_newer_file(char *localpath,int index)
{
    //DEBUG("sync_newer_file start!\n");
    char *serverpath;
    serverpath = localpath_to_serverpath(localpath,index);
    time_t modtime1,modtime2;

    modtime1 = Getmodtime(serverpath,index);
    free(serverpath);
    if(modtime1 == -1)
    {
        DEBUG("newer_file Getmodtime failed!\n");
        return -1;
    }

    struct stat buf;
    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
        return -1;
    }
    modtime2 = buf.st_mtime;

    DEBUG("localtime = %lu,servertime = %lu\n",modtime2,modtime1);

    if(modtime1 == modtime2)     //no modify
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

long long get_file_size(const char *path)
{
    long long filesize = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0){
        return filesize;
    }else{
        filesize = statbuff.st_size;
    }
    return filesize;
}

/*
 *find 'chr' in 'str' at the 'n' times,'n' need to >=1;
 *返回'chr'在'str'中第'n'次出现的位置
*/
char *my_nstrchr(const char chr,char *str,int n){

    if(n<1)
    {
        DEBUG("my_nstrchr need n>=1\n");
        return NULL;
    }

    char *p1,*p2;
    int i = 1;
    p1 = str;

    do{
        p2 = strchr(p1,chr);
        p1 = p2;
        p1++;
        i++;
    }while(p2!=NULL && i<=n);

    if(i<n)
    {
        return NULL;
    }

    return p2;
}

/*
 *0,no local socket
 *1,local socket
*/
int wait_handle_socket(int index){
    if(g_pSyncList[index]->receve_socket)
    {
        server_sync = 0;
        while(g_pSyncList[index]->receve_socket || local_sync)
        {
            usleep(1000*100);
        }
        server_sync = 1;
        if(g_pSyncList[index]->have_local_socket)
        {
            g_pSyncList[index]->have_local_socket = 0;
            g_pSyncList[index]->first_sync = 1;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

int moveFolder(char *old_dir,char *new_dir,int index)
{
    int status;
    struct dirent *ent = NULL;
    DIR *pDir;

    pDir=opendir(new_dir);
    if(pDir != NULL )
    {
        status = Mkcol(new_dir,index);
        if(status != 0)
        {
            DEBUG("Create %s failed\n",new_dir);
            closedir(pDir);
            return status;
        }
        while ((ent=readdir(pDir)) != NULL)
        {
#if 1
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;

            char *old_fullname;
            char *new_fullname;
            new_fullname = my_str_malloc(strlen(new_dir)+strlen(ent->d_name)+2);
            old_fullname = my_str_malloc(strlen(old_dir)+strlen(ent->d_name)+2);
            snprintf(new_fullname, sizeof(char)*(strlen(new_dir)+strlen(ent->d_name)+2), "%s/%s",new_dir,ent->d_name);
            snprintf(old_fullname, sizeof(char)*(strlen(old_dir)+strlen(ent->d_name)+2), "%s/%s",old_dir,ent->d_name);
            if(test_if_dir(new_fullname) == 1)
            {
                status = moveFolder(old_fullname,new_fullname,index);
                if(status != 0)
                {
                    DEBUG("CreateFolder %s failed\n",new_fullname);
                    free(new_fullname);
                    free(old_fullname);
                    closedir(pDir);
                    return status;
                }
            }
            else
            {
                status = Move(old_fullname,new_fullname,index);
                if(status != 0)
                {
                    DEBUG("move %s failed\n",new_fullname);

                    free(new_fullname);
                    free(old_fullname);
                    closedir(pDir);
                    return status;
                }
            }
            free(new_fullname);
            free(old_fullname);
#endif
        }
        closedir(pDir);
        return 0;
    }
    else{
        DEBUG("open %s fail \n",new_dir);
        return LOCAL_FILE_LOST;
    }
}


/*
 *1,server has the same file
 *0,server has no the same file
*/
int is_server_exist(char *parentpath,char *filepath,int index){

    int status;
    char *server_parenthref;
    char *url;
    char *file_url;
    CloudFile *FileList_one;
    CloudFile *FileTail_one;

    FileList_one = (CloudFile *)malloc(sizeof(CloudFile));
    memset(FileList_one,0,sizeof(CloudFile));

    FileList_one->href = NULL;

    FileTail_one = FileList_one;
    FileTail_one->next = NULL;

    DEBUG("parentpath = %s\n",parentpath);

    server_parenthref = localpath_to_serverpath(parentpath,index);

    DEBUG("server_parenthref = %s\n",server_parenthref);

    url = my_str_malloc(strlen(asus_cfg.prule[index]->host)+strlen(server_parenthref)+1);
    snprintf(url, sizeof(char)*(strlen(asus_cfg.prule[index]->host)+strlen(server_parenthref)+1), "%s%s",asus_cfg.prule[index]->host,server_parenthref);
    free(server_parenthref);

    DEBUG("url = %s\n",url);

    do{
        status = getCloudInfo_one(url,wd_parsexml_one,FileTail_one,index);
        usleep(1000*500);
    }while(status == COULD_NOT_CONNECNT_TO_SERVER && exit_loop == 0);

    free(url);

    if(status != 0)
    {
        free_CloudFile_item(FileList_one);
        FileList_one = NULL;

        DEBUG("get Cloud Info One ERROR! \n");

        return 0;
    }

    char *server_path;

    server_path = localpath_to_serverpath(filepath,index);
    file_url = server_path + strlen(asus_cfg.prule[index]->rootfolder);

    DEBUG("server_path = %s\n",server_path);
    DEBUG("file_url = %s\n",file_url);

    CloudFile *de_filecurrent;
    de_filecurrent = FileList_one->next;
    while(de_filecurrent != NULL)
    {
        if(de_filecurrent->href != NULL)
        {
            if(!(strcmp(de_filecurrent->href,file_url)))
            {

                DEBUG("get it\n");

                free(server_path);
                free_CloudFile_item(FileList_one);
                return 1;
            }
        }
        de_filecurrent = de_filecurrent->next;
    }

    free(server_path);
    free_CloudFile_item(FileList_one);

    return 0;
}

int get_create_threads_state()
{
    int i;
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(asus_cfg.prule[i]->rule != 2)
        {
            return 1;
        }
    }

    return 0;
}

struct tokenfile_info_tag *initial_tokenfile_info_data(struct tokenfile_info_tag **token)
{
    struct tokenfile_info_tag *follow_token;

    if(token == NULL)
        return *token;

    *token = (struct tokenfile_info_tag *)malloc(sizeof(struct tokenfile_info_tag));
    if(*token == NULL)
        return NULL;

    follow_token = *token;

    follow_token->url = NULL;
    follow_token->folder = NULL;
    follow_token->mountpath = NULL;
    follow_token->next = NULL;

    return follow_token;
}

int free_tokenfile_info(struct tokenfile_info_tag *head)
{
    struct tokenfile_info_tag *p = head->next;
    while(p != NULL)
    {
        head->next = p->next;
        if(p->folder != NULL)
        {
            free(p->folder);
        }
        if(p->mountpath != NULL)
        {
            free(p->mountpath);
        }
        if(p->url != NULL)
        {
            free(p->url);
        }
        free(p);
        p = head->next;
    }
    return 0;
}

int delete_tokenfile_info(char *url,char *folder){

    struct tokenfile_info_tag *tmp;

    tmp = tokenfile_info_start;
    tokenfile_info_tmp = tokenfile_info_start->next;

    while(tokenfile_info_tmp != NULL)
    {
        if( !strcmp(tokenfile_info_tmp->url,url) &&
            !strcmp(tokenfile_info_tmp->folder,folder))
        {
            tmp->next = tokenfile_info_tmp->next;
            free(tokenfile_info_tmp->folder);
            free(tokenfile_info_tmp->url);
            free(tokenfile_info_tmp->mountpath);
            free(tokenfile_info_tmp);
            tokenfile_info_tmp = tmp->next;
        }
        else
        {
            tmp = tokenfile_info_tmp;
            tokenfile_info_tmp = tokenfile_info_tmp->next;
        }
    }
    return 0;
}

int add_tokenfile_info(char *url,char *folder,char *mpath){

    if(initial_tokenfile_info_data(&tokenfile_info_tmp) == NULL)
    {
        return -1;
    }

    tokenfile_info_tmp->url = my_str_malloc(strlen(url)+1);
    snprintf(tokenfile_info_tmp->url, sizeof(char)*(strlen(url)+1), "%s",url);

    tokenfile_info_tmp->mountpath = my_str_malloc(strlen(mpath)+1);
    snprintf(tokenfile_info_tmp->mountpath, sizeof(char)*(strlen(mpath)+1), "%s",mpath);

    tokenfile_info_tmp->folder = my_str_malloc(strlen(folder)+1);
    snprintf(tokenfile_info_tmp->folder, sizeof(char)*(strlen(folder)+1), "%s",folder);

    tokenfile_info->next = tokenfile_info_tmp;
    tokenfile_info = tokenfile_info_tmp;

    return 0;
}

char *delete_nvram_contents(char *url,char *folder){
    char *nv;
    char *nvp;
    char *p,*b;
    char *new_nv;
    int n;
    int i=0;
#ifdef NVRAM_
#ifndef USE_TCAPI
    p = nv = strdup(nvram_safe_get("wd_tokenfile"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "wd_tokenfile", tmp);
    p = nv = my_str_malloc(strlen(tmp)+1);
    snprintf(nv, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif
#else
    FILE *fp;
    fp = fopen(TOKENFILE_RECORD,"r");
    if (fp==NULL)
    {
        nv = my_str_malloc(2);
        snprintf(nv, sizeof(char)*2, "%s",  "");
    }
    else
    {
        fseek( fp , 0 , SEEK_END );
        int file_size;
        file_size = ftell( fp );
        fseek(fp , 0 , SEEK_SET);
        nv = my_str_malloc(file_size+2);
        fscanf(fp,"%[^\n]%*c",nv);
        p = nv;
        fclose(fp);
    }

#endif

    nvp = my_str_malloc(strlen(url)+strlen(folder)+2);
    snprintf(nvp, sizeof(char)*(strlen(url)+strlen(folder)+2), "%s>%s",url,folder);

    if(strstr(nv,nvp) == NULL)
    {
        free(nvp);
        return nv;
    }

    if(!strcmp(nv,nvp))
    {
        free(nvp);
        free(nv);
        nv = my_str_malloc(2);
        snprintf(nv, sizeof(char)*2, "%s", "");
        return nv;
    }

    if(nv)
    {
        while((b = strsep(&p, "<")) != NULL)
        {
            if(strcmp(b,nvp))
            {
                n = strlen(b);
                if(i == 0)
                {
                    new_nv = my_str_malloc(n+1);
                    snprintf(new_nv, sizeof(char)*(n+1), "%s",b);
                    ++i;
                }
                else
                {
                    new_nv = realloc(new_nv, strlen(new_nv)+n+2);
                    sprintf(new_nv,"%s<%s",new_nv,b);
                }
            }
        }

        free(nv);
    }
    free(nvp);
    return new_nv;
}

char *add_nvram_contents(char *url,char *folder){

    char *nv;
    int nv_len;
    char *new_nv;
    char *nvp;

    nvp = my_str_malloc(strlen(url)+strlen(folder)+2);
    snprintf(nvp, sizeof(char)*(strlen(url)+strlen(folder)+2), "%s>%s",url,folder);

    DEBUG("add_nvram_contents     nvp = %s\n",nvp);

#ifdef NVRAM_
#ifndef USE_TCAPI
    nv = strdup(nvram_safe_get("wd_tokenfile"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "wd_tokenfile", tmp);
    nv = my_str_malloc(strlen(tmp)+1);
    snprintf(nv, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif
#else
    FILE *fp;
    fp = fopen(TOKENFILE_RECORD,"r");
    if (fp==NULL)
    {
        nv = my_str_malloc(2);
        snprintf(nv, sizeof(char)*2, "%s",  "");
    }
    else
    {
        fseek( fp , 0 , SEEK_END );
        int file_size;
        file_size = ftell( fp );
        fseek(fp , 0 , SEEK_SET);
        nv = my_str_malloc(file_size+2);
        fscanf(fp,"%[^\n]%*c",nv);
        fclose(fp);
    }

#endif
    nv_len = strlen(nv);

    if(nv_len)
    {
        new_nv = my_str_malloc(strlen(nv)+strlen(nvp)+2);
        snprintf(new_nv, sizeof(char)*(strlen(nv)+strlen(nvp)+2), "%s<%s",nv,nvp);

    }
    else
    {
        new_nv = my_str_malloc(strlen(nvp)+1);
        snprintf(new_nv, sizeof(char)*(strlen(nvp)+1), "%s",nvp);
    }
    free(nvp);
    free(nv);
    return new_nv;
}

int write_debug_log(char *str){

    FILE *fp;
    fp = fopen("/tmp/webdav_debug","a");
    if(fp)
    {
        fprintf(fp,"%s\n",str);
        fclose(fp);
    }
    return 0;
}

/*process running,return 1;else return 0*/
#ifdef OLEG_ARM
int detect_process(char * process_name)
{
    FILE *ptr;
    char buff[512];
    char ps[128];
    int n=0;
    snprintf(ps, 128, "ps | awk '{print $3}' |grep  %s",process_name);
    snprintf(buff, 512, "%s", "ABNORMAL");
    if((ptr=popen(ps, "r")) != NULL)
    {
        while (fgets(buff, 512, ptr) != NULL)
        {
            if(strcmp(buff, "nvram") == 0)
            n++;

            if(n>0)
            {
                pclose(ptr);
                return 1;
            }
        }
    }
    if(strcmp(buff,"ABNORMAL")==0)  /*ps command error*/
    {
        return 0;
    }
    pclose(ptr);
    return 0;
}
#else
#ifdef I686
int detect_process(char * process_name)
{
    FILE *ptr;
    char buff[512];
    char ps[128];
    snprintf(ps, 128, "ps | awk '{print $5  $6}' |grep -c %s",process_name);
    snprintf(buff, 512, "%s", "ABNORMAL");
    if((ptr=popen(ps, "r")) != NULL)
    {
        while (fgets(buff, 512, ptr) != NULL)
        {
            if(atoi(buff)>2)
            {
                pclose(ptr);
                return 1;
            }
        }
    }
    if(strcmp(buff,"ABNORMAL")==0)  /*ps command error*/
    {
        return 0;
    }
    pclose(ptr);
    return 0;
}
#else
int detect_process(char * process_name)
{
    FILE *ptr;
    char buff[512];
    char ps[128];
    snprintf(ps, 128, "ps | grep -c %s",process_name);
    snprintf(buff, 512, "%s", "ABNORMAL");
    if((ptr=popen(ps, "r")) != NULL)
    {
        while (fgets(buff, 512, ptr) != NULL)
        {
            if(atoi(buff)>2)
            {
                pclose(ptr);
                return 1;
            }
        }
    }
    if(strcmp(buff,"ABNORMAL")==0)  /*ps command error*/
    {
        return 0;
    }
    pclose(ptr);
    return 0;
}
#endif
#endif
void wd_set_error(char *error,int size,const char *format, ...)
{
    va_list params;

    va_start(params,format);

    vsnprintf(error,size,format,params);

    va_end(params);

    error[size-1]='\0';
}

int write_conflict_log(char *prename, char *conflict_name,int index)
{
    FILE *fp;

    if(access(g_pSyncList[index]->conflict_file,F_OK))
    {
        fp = fopen(g_pSyncList[index]->conflict_file,"w");
    }
    else
    {
        fp = fopen(g_pSyncList[index]->conflict_file,"a");
    }

    if(NULL == fp)
    {
        printf("open %s failed\n",g_pSyncList[index]->conflict_file);
        return -1;
    }

    fprintf(fp,"%s is download from server,%s is local file and rename from %s\n",prename,conflict_name,prename);
    fclose(fp);

    return 0;
}

#ifndef PC
int check_link_internet(int index)
{
    int link_flag = 0;
    int i;

    DEBUG("##########begin check link_internet############\n");

    struct timeval now;
    struct timespec outtime;

    while(!link_flag && !exit_loop)
    {
#if defined NVRAM_
#ifndef USE_TCAPI
        char *link_internet = strdup(nvram_safe_get("link_internet"));
#else
        char tmp[MAXLEN_TCAPI_MSG] = {0};
        tcapi_get(WANDUCK, "link_internet", tmp);
        char *link_internet = my_str_malloc(strlen(tmp)+1);
        snprintf(link_internet, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif
        link_flag = atoi(link_internet);
        free(link_internet);
#else
        system(SH_GET_NVRAM_SCRIPT_2);
        sleep(2);

        char nv[64] = {0};
        FILE *fp;
        fp = fopen(NVRAM_PATH_2,"r");
        fgets(nv,sizeof(nv),fp);
        DEBUG("nv=%s\n",nv);
        fclose(fp);

        link_flag = atoi(nv);
#endif
        if(!link_flag)
        {
            for(i=0;i<asus_cfg.dir_number;i++)
            {
                write_log(S_ERROR,"Network Connection Failed","",i);
            }
            pthread_mutex_lock(&mutex);
            if(!exit_loop)
            {
                gettimeofday(&now, NULL);
                outtime.tv_sec = now.tv_sec + 20;
                outtime.tv_nsec = now.tv_usec * 1000;
                pthread_cond_timedwait(&cond, &mutex, &outtime);
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        write_log(S_SYNC,"","",i);
    }

    return 0;
}
#endif

void create_start_file()
{
    my_mkdir("/tmp/smartsync_app");
    FILE *fp;
    fp = fopen("/tmp/smartsync_app/webdav_client_start","w");
    if(fp)
        fclose(fp);
}

/*
 *0,no file
 *1,have file
*/
int detect_process_file()
{
    struct dirent *ent = NULL;
    DIR *pdir;
    int num = 0;
    pdir = opendir("/tmp/smartsync_app");

    if(pdir != NULL)
    {
        while (NULL != (ent=readdir(pdir)))
        {
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;
            num++;
        }
        closedir(pdir);
    }
    else
        return 0;

    if(num)
        return 1;

    return 0;
}

int wd_parsexml(char *xmlBuf,int xmlBufLength,CloudFile *TreeFolderTail,CloudFile *TreeFileTail,int index)
{
    char *p,*p1;
    char *node;

    char *nodeStart;
    char *nodeEnd;

    char *key = NULL;
    CloudFile *FolderTmp;

    int len = 0;

    if (NULL == strstr(xmlBuf,"<D:multistatus "))
    {
         DEBUG("Have no XML content header!\n");
         return -1;
    }

    if (NULL == strstr(xmlBuf, "</D:multistatus>"))
    {
        DEBUG("Have no XML content tail!\n");
        return -1;
    }

    nodeStart = strstr(xmlBuf,"<D:response>");
    nodeEnd = strstr(xmlBuf,"</D:response>");

    while(nodeStart && nodeEnd)
    {
        node = my_str_malloc(strlen(nodeStart)-strlen(nodeEnd)+1);
        snprintf(node,strlen(nodeStart)-strlen(nodeEnd),"%s",nodeStart);
        /*get href*/
        p = strstr(node,"<D:href>");
        p1 = strstr(node,"</D:href>");
        if(NULL == p)
        {
            free(node);
            node = NULL;
            nodeStart = strstr(nodeEnd,"<D:response>");
            nodeEnd = strstr(nodeStart,"</D:response>");
            continue;
        }
        FolderTmp = (CloudFile *)malloc(sizeof(CloudFile));
        memset(FolderTmp,0,sizeof(CloudFile));
        FolderTmp->next = NULL;
        p = strstr(p,asus_cfg.prule[index]->rootfolder);
        len = strlen(p)-strlen(p1)-strlen(asus_cfg.prule[index]->rootfolder)+1;
        FolderTmp->href = my_str_malloc(len);
        snprintf(FolderTmp->href,len,"%s",p+strlen(asus_cfg.prule[index]->rootfolder));
        FolderTmp->isFolder = 3;
        FolderTmp->ismodify = 0;

        /*get getcontentlength*/
        p = strstr(node,"<D:getcontentlength>");
        p1 = strstr(node,"</D:getcontentlength>");
        len = strlen(p)-strlen(p1)-strlen("<D:getcontentlength>")+1;
        FolderTmp->getcontentlength = my_str_malloc(len);
        snprintf(FolderTmp->getcontentlength,len,"%s",p+strlen("<D:getcontentlength>"));

        /*getcontenttype*/
        p = strstr(node,"<D:getcontenttype>");
        p1 = strstr(p,"httpd/unix-directory");
        if(NULL == p1)
        {
            FolderTmp->isFolder = 0;
        }
        else
        {
            FolderTmp->isFolder = 1;
        }
        /*getlastmodified*/

        p = strstr(node,"<D:getlastmodified>");
        p1 = strstr(node,"</D:getlastmodified>");
        len = strlen(p)-strlen(p1)-strlen("<D:getlastmodified>")+1;
        key = my_str_malloc(len);
        snprintf(key,len,"%s",p+strlen("<D:getlastmodified>"));
        FolderTmp->modtime = ne_httpdate_parse(key);
        free(key);
        key = NULL;

        if(FolderTmp->isFolder == 1)
        {
            TreeFolderTail->next = FolderTmp;
            TreeFolderTail = FolderTmp;
            TreeFolderTail->next = NULL;
        }
        else if(FolderTmp->isFolder == 0)
        {
            TreeFileTail->next = FolderTmp;
            TreeFileTail = FolderTmp;
            TreeFileTail->next = NULL;
        }

        free(node);
        node = NULL;
        nodeStart = strstr(nodeEnd,"<D:response>");
        if(NULL == nodeStart)
            break;
        else
            nodeEnd = strstr(nodeStart,"</D:response>");
    }

    DEBUG("wd_parsexml OVER!!\n");
    return 0;
}

int wd_parsexml_one(char *xmlBuf,int xmlBufLength,CloudFile *FileTail_one,int index)
{
    char *p,*p1;
    char *node;

    char *nodeStart;
    char *nodeEnd;

    char *key = NULL;
    CloudFile *FileTmp_one;

    int len = 0;

    if (NULL == strstr(xmlBuf,"<D:multistatus "))
    {
         DEBUG("Have no XML content header!\n");
         return -1;
    }

    if (NULL == strstr(xmlBuf, "</D:multistatus>"))
    {
        DEBUG("Have no XML content tail!\n");
        return -1;
    }

    nodeStart = strstr(xmlBuf,"<D:response>");
    nodeEnd = strstr(xmlBuf,"</D:response>");

    while(nodeStart && nodeEnd)
    {
        node = my_str_malloc(strlen(nodeStart)-strlen(nodeEnd)+1);
        snprintf(node,strlen(nodeStart)-strlen(nodeEnd),"%s",nodeStart);

        /*get href*/
        p = strstr(node,"<D:href>");
        p1 = strstr(node,"</D:href>");
        if(NULL == p)
        {
            free(node);
            node = NULL;
            nodeStart = strstr(nodeEnd,"<D:response>");
            nodeEnd = strstr(nodeStart,"</D:response>");
            continue;
        }
        FileTmp_one = (CloudFile *)malloc(sizeof(CloudFile));
        memset(FileTmp_one,0,sizeof(CloudFile));
        FileTmp_one->next = NULL;
        p = strstr(p,asus_cfg.prule[index]->rootfolder);
        len = strlen(p)-strlen(p1)-strlen(asus_cfg.prule[index]->rootfolder)+1;
        FileTmp_one->href = my_str_malloc(len);
        snprintf(FileTmp_one->href,len,"%s",p+strlen(asus_cfg.prule[index]->rootfolder));
        FileTmp_one->isFolder = 3;
        FileTmp_one->ismodify = 0;

        /*get getcontentlength*/
        p = strstr(node,"<D:getcontentlength>");
        p1 = strstr(node,"</D:getcontentlength>");
        len = strlen(p)-strlen(p1)-strlen("<D:getcontentlength>")+1;
        FileTmp_one->getcontentlength = my_str_malloc(len);
        snprintf(FileTmp_one->getcontentlength,len,"%s",p+strlen("<D:getcontentlength>"));

        /*getcontenttype*/
        p = strstr(node,"<D:getcontenttype>");
        p1 = strstr(p,"httpd/unix-directory");
        if(NULL == p1)
        {
            FileTmp_one->isFolder = 0;
        }
        else
        {
            FileTmp_one->isFolder = 1;
        }

        /*getlastmodified*/

        p = strstr(node,"<D:getlastmodified>");
        p1 = strstr(node,"</D:getlastmodified>");
        len = strlen(p)-strlen(p1)-strlen("<D:getlastmodified>")+1;
        key = my_str_malloc(len);
        snprintf(key,len,"%s",p+strlen("<D:getlastmodified>"));
        FileTmp_one->modtime = ne_httpdate_parse(key);
        free(key);
        key = NULL;

        FileTail_one->next = FileTmp_one;
        FileTail_one = FileTmp_one;
        FileTail_one->next = NULL;

        free(node);
        node = NULL;
        nodeStart = strstr(nodeEnd,"<D:response>");
        if(NULL == nodeStart)
            break;
        else
            nodeEnd = strstr(nodeStart,"</D:response>");
    }

    DEBUG("wd_parsexml OVER!!\n");
    return 0;
}

int wd_parsexml_RouterInfo(char *xmlBuf,int xmlBufLength,int index)
{
    char *p,*p1;
    char *key = NULL;
    int len = 0;

    p = strstr(xmlBuf,"<DiskAvailable>");
    p1 = strstr(xmlBuf,"</DiskAvailable>");
    if(NULL != p && NULL != p1)
    {
        len = strlen(p)-strlen(p1)-strlen("<DiskAvailable>")+1;
        key = my_str_malloc(len);
        snprintf(key,len,"%s",p+strlen("<DiskAvailable>"));
        DiskAvailable = 1024*(atoll(key));
        DiskAvailableShow = (atoll(key))/1024;

        free(key);
    }
    else
        return -1;

    p = strstr(xmlBuf,"<DiskUsed>");
    p1 = strstr(xmlBuf,"</DiskUsed>");
    if(NULL != p && NULL != p1)
    {
        len = strlen(p)-strlen(p1)-strlen("<DiskUsed>")+1;
        key = my_str_malloc(len);
        snprintf(key,len,"%s",p+strlen("<DiskUsed>"));
        DiskUsedShow = (atoll(key))/1024;

        free(key);
    }
    else
        return -1;

    return 0;
}
