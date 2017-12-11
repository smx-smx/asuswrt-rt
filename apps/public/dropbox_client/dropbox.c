#include<stdio.h>
#include<stdlib.h>
#include"data.h"

#define MYPORT 3570
#define INOTIFY_PORT 3678
#define BACKLOG 100 /* max listen num*/
int no_config;
int exit_loop = 0;
int stop_progress;
char username[256];
char password[256];

int is_renamed = 1;
int access_token_expired;

char *Clientfp="*";
double start_time;

#define MAXSIZE 512

int set_iptables(int flag)
{
    int rc = -1;
    char cmd[1024]={0};
#ifdef I686
#else
    snprintf(cmd, 1024, "iptables -D INPUT -p tcp -m tcp --dport %d -j DROP", MYPORT);
    rc = system(cmd);
    snprintf(cmd, 1024, "iptables -D INPUT -p tcp -m tcp -s 127.0.0.1 --dport %d -j ACCEPT", MYPORT);
    rc = system(cmd);
#endif
    if(flag)
    {
#ifdef I686
	rc = 0;
#else
        snprintf(cmd, 1024, "iptables -I INPUT -p tcp -m tcp --dport %d -j DROP", MYPORT);
        rc = system(cmd);
        snprintf(cmd, 1024, "iptables -I INPUT -p tcp -m tcp -s 127.0.0.1 --dport %d -j ACCEPT", MYPORT);
        rc = system(cmd);
#endif
    }

    if(rc == 0)
        return 0;
    else
        return 1;
}

int write_rootca()
{
    FILE *fp;
    fp = fopen(CA_INFO_FILE,"w");
    if(NULL == fp)
    {
        printf("open rootca  file %s fail\n",CA_INFO_FILE);
        return -1;
    }
    fprintf(fp,"%s","-----BEGIN CERTIFICATE-----\n"
            "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
            "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
            "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
            "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
            "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
            "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
            "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
            "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
            "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
            "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
            "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
            "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
            "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
            "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
            "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
            "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
            "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
            "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
            "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
            "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
            "+OkuE6N36B9K\n"
            "-----END CERTIFICATE-----\n");
    fclose(fp);
    return 0;
}

int write_trans_excep_log(char *fullname,int type,char *msg)
{
    FILE *fp = 0;
    char ctype[16] = {0};

    if(type == 1)
        snprintf(ctype,16, "%s", "Error");
    else if(type == 2)
        snprintf(ctype, 16, "%s", "Info");
    else if(type == 3)
        snprintf(ctype, 16, "%s", "Warning");

    if(access(trans_excep_file,0) == 0)
        fp = fopen(trans_excep_file,"a");
    else
        fp = fopen(trans_excep_file,"w");

    if(fp == NULL)
    {
        printf("open %s fail\n",trans_excep_file);
        return -1;
    }

    fprintf(fp,"TYPE:%s\nUSERNAME:%s\nFILENAME:%s\nMESSAGE:%s\n",ctype,username,fullname,msg);
    fclose(fp);
    return 0;
}


char *write_error_message(char *format,...)
{
    int size=256;
    char *p=(char *)malloc(size);
    memset(p,0,size);
    va_list ap;
    va_start(ap,format);
    vsnprintf(p,size,format,ap);
    va_end(ap);
    return p;
}

#define GMTOFF(t) ((t).tm_gmtoff)

static const char short_months[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


/* 2017-05-08T06:25:47Z */
#define RFC1123_FORMAT "%4d-%02d-%02dT%02d:%02d:%02dZ"
time_t ne_rfc1123_parse(const char *date)
{
    struct tm gmt = {0};
    char  mon[4];
    int n;
    time_t result;

    n = sscanf(date, RFC1123_FORMAT,
                  &gmt.tm_year,&gmt.tm_mon, &gmt.tm_mday, &gmt.tm_hour,
               &gmt.tm_min, &gmt.tm_sec);

    gmt.tm_year -= 1900;
    gmt.tm_mon -= 1;
    gmt.tm_isdst = -1;
    result = mktime(&gmt);
    return result + GMTOFF(gmt);
}

void buffer_free(char *b) {
        if (!b) return;

        free(b);
        b = NULL;
}

char *parse_name_from_path(const char *path)
{
    char *name;
    char *p;

    p = strrchr(path,'/');

    if( p == NULL)
    {
        return NULL;
    }

    p++;

    name = (char *)malloc(sizeof(char)*(strlen(p)+2));
    memset(name,0,strlen(p)+2);

    snprintf(name, sizeof(char)*(strlen(p)+2), "%s", p);

    return name;
}
void cjson_change_to_cloudfile(cJSON *q)
{
    if(strcmp(q->string,".tag")==0)
        {
            if(strcmp(q->valuestring, "folder") == 0)
                FolderTmp->isFolder=1;
            else if(strcmp(q->valuestring, "file") == 0)
                FolderTmp->isFolder=0;
        }
    else if(strcmp(q->string,"name")==0)
    {
        FolderTmp->name=(char *)malloc(sizeof(char)*(strlen(q->valuestring)+1));
        snprintf(FolderTmp->name, sizeof(char)*(strlen(q->valuestring)+1), "%s", q->valuestring);
    }
    else if(strcmp(q->string,"path_display")==0)
    {
        FolderTmp->href=(char *)malloc(sizeof(char)*(strlen(q->valuestring)+1));
        snprintf(FolderTmp->href, sizeof(char)*(strlen(q->valuestring)+1), "%s", q->valuestring);
    }
    else if(strcmp(q->string,"server_modified")==0)
    {
        snprintf(FolderTmp->tmptime, MIN_LENGTH, "%s", q->valuestring);
        FolderTmp->mtime=ne_rfc1123_parse(FolderTmp->tmptime);
    }
    else if(strcmp(q->string,"size")==0)
    {
        FolderTmp->size=q->valueint;
    }
}
char *cJSON_parse_name(cJSON *json)
{
    char *name;
    cJSON *q;
    q=json->child;
    int i=0;
    while(q!=NULL)
    {
        if(strcmp(q->string,"path_display")==0)
        {
            wd_DEBUG("path_display : %s\n",q->valuestring);
            return q->valuestring;
        }
        q=q->next;
    }
}

int cJSON_printf_dir(cJSON *json)
{
    int flag = 0;

    cJSON *q;
    q=json->child;
    while(q!=NULL)
    {
        if(strcmp(q->string,".tag") == 0)
        {
            if(0 == strcmp(q->valuestring, "deleted"))
                    flag=1;
            return flag;
        }
        q=q->next;
    }
    return flag;

}

time_t cJSON_printf_mtime(cJSON *json)
{
    if(json)
    {
        time_t mtime=(time_t)-1;
        cJSON *q;
        q=json->child;
        while(q!=NULL)
        {
            if(strcmp(q->string,"server_modified") == 0)
            {
                mtime=ne_rfc1123_parse(q->valuestring);
            }
            else if(strcmp(q->string,"is_deleted") == 0)
            {
                if(q->type)
                    return (time_t)-1;
            }
            q=q->next;
        }
        return mtime;
    }
    else
        return (time_t)-1;
}
/*
 FileTail_one is not only file list ,it is also folder list
*/
int cJSON_printf_one(cJSON *json, int *has_more_one, char **cursor)
{
    if(json)
    {
        cJSON *p,*q,*m;
        q=json->child;
        int i=0;
        while(q!=NULL)
        {
            if(strcmp(q->string,"entries")==0)
            {
                if(q->child!=NULL){
                    p=q->child;m=p->child;
                    while(p!=NULL)
                    {
                        FolderTmp = (CloudFile *)malloc(sizeof(CloudFile));
                        memset(FolderTmp,0,sizeof(CloudFile));
                        FolderTmp->href=NULL;

                        m=p->child;i++;
                        while(m!=NULL)
                        {
                            cjson_change_to_cloudfile(m);
                            m=m->next;
                        }
                        FileTail_one->next = FolderTmp;
                        FileTail_one = FolderTmp;
                        FileTail_one->next = NULL;
                        p=p->next;
                    }
                }
                else
                    wd_DEBUG("this is empty folder\n");
            }
            else if(strcmp(q->string,"has_more") == 0)
            {
                    if(q->valueint == 1)
                            *has_more_one = 1;
                    else if(q->valueint == 0)
                            *has_more_one = 0;
            }
            else if(strcmp(q->string,"cursor") == 0)
            {
                    *cursor=(char *)malloc(sizeof(char)*(strlen(q->valuestring)+1));
                    snprintf(*cursor, sizeof(char)*(strlen(q->valuestring)+1), "%s", q->valuestring);
            }
            q=q->next;
        }
    }

}
time_t cJSON_printf(cJSON *json,char *string, int *has_more, char **cursor)
{
    if(json)
    {
        cJSON *p,*q,*m;
        q=json->child;
        int i=0;
        while(q!=NULL)
        {
            if(strcmp(q->string,"entries")==0)
            {
                if(strcmp(string,"entries")==0)
                {
                    if(q->child!=NULL){
                        p=q->child;m=p->child;
                        while(p!=NULL)
                        {
                            FolderTmp = (CloudFile *)malloc(sizeof(CloudFile));
                            memset(FolderTmp,0,sizeof(CloudFile));
                            FolderTmp->href=NULL;

                            m=p->child;i++;
                            while(m!=NULL)
                            {
                                cjson_change_to_cloudfile(m);
                                m=m->next;
                            }
                            if(FolderTmp->isFolder==0)
                            {
                                TreeFileTail->next = FolderTmp;
                                TreeFileTail = FolderTmp;
                                TreeFileTail->next = NULL;
                            }
                            else if(FolderTmp->isFolder==1)
                            {
                                TreeFolderTail->next = FolderTmp;
                                TreeFolderTail = FolderTmp;
                                TreeFolderTail->next = NULL;
                            }
                            p=p->next;
                        }
                    }
                }
            }
            else if(strcmp(q->string,"server_modified") == 0)
            {
                if(strcmp(string,"server_modified") == 0)
                {
                    time_t mtime=ne_rfc1123_parse(q->valuestring);
                    return mtime;
                }
            }
            else if(strcmp(q->string,"used") == 0)
            {
                if(strcmp(string,"used") == 0)
                {
                    server_used=q->valuelong;
                    if(q->child != NULL)
                    {
                        p=q->child;m=p->child;
                        while(p!=NULL)
                        {
                            cjson_to_space(p);
                            p=p->next;
                        }
                    }
                }
            }
            else if(strcmp(q->string,"allocation") == 0)
            {
                if(strcmp(string,"used") == 0)
                {
                    if(q->child != NULL)
                    {
                        p=q->child;m=p->child;
                        while(p!=NULL)
                        {
                            cjson_to_space(p);
                            p=p->next;
                        }
                    }
                }
            }
            else if(strcmp(q->string,"error_summary") == 0)
            {
                if(strcmp(string,"error_summary")==0)
                {
                    if(strstr(q->valuestring, "from_lookup/not_found/") != NULL)
                        ;
                    if(strstr(q->valuestring, "to/conflict/folder/") != NULL)
                        ;
                }
            }
            else if(strcmp(q->string,"has_more") == 0)
            {
                if(strcmp(string,"entries")==0)
                {
                    if(q->valueint == 1)
                            *has_more = 1;
                    else if(q->valueint == 0)
                            *has_more = 0;
                }
            }
            else if(strcmp(q->string,"cursor") == 0)
            {
                if(strcmp(string,"entries")==0)
                {
                    *cursor=(char *)malloc(sizeof(char)*(strlen(q->valuestring)+1));
                    snprintf(*cursor, sizeof(char)*(strlen(q->valuestring)+1), "%s", q->valuestring);
                }
            }
            q=q->next;
        }
    }
    else
        return (time_t)-1;

}

int cJSON_printf_error(cJSON *json,char *string)
{
    if(json)
    {
        cJSON *q;
        q=json->child;
        int i=0;
        while(q!=NULL)
        {
            if(strcmp(q->string,"error_summary") == 0)
            {
                if(strcmp(string,"error_summary")==0)
                {
                    if(strstr(q->valuestring, "from_lookup/not_found/") != NULL)
                        return 1;
                    if(strstr(q->valuestring, "to/conflict") != NULL)
                        return 2;
                    if(strstr(q->valuestring, "path/conflict") != NULL)
                        return 3;
                }
            }
            q=q->next;
        }
    }
    else
        return -1;
}

void cjson_to_space(cJSON *q)
{
    if(strcmp(q->string,"allocated") == 0)
    {
        server_allocated=q->valuelong;
    }
}
cJSON *doit(char *text)
{
    char *out;cJSON *json;

    json=cJSON_Parse(text);
    if (!json) {wd_DEBUG("Error before: [%s]\n",cJSON_GetErrorPtr());return NULL;}
    else
    {
        return json;
    }
}

/* Read a file, parse, render back, etc. */
cJSON *dofile(char *filename)
{
    cJSON *json;
    FILE *f=fopen(filename,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
    char *data=malloc(len+1);fread(data,1,len,f);fclose(f);
    json=doit(data);
    free(data);
    if(json)
        return json;
    else
        return NULL;
}
#ifdef OAuth1
int open_login_page_first()
{
    char Myurl[MAXSIZE];
    memset(Myurl,0,sizeof(Myurl));
    snprintf(Myurl, MAXSIZE, "%s","https://www.dropbox.com/login");
    wd_DEBUG("Myurl:%s\n",Myurl);
    CURL *curl;
    CURLcode res;
    FILE *fp;

    curl=curl_easy_init();
    if(curl){
        struct curl_slist *headers_l=NULL;
        static const char header1_l[]="Host:www.dropbox.com";
        static const char header2_l[]="Mozilla/5.0 (Windows NT 6.1; rv:17.0) Gecko/20100101 Firefox/17.0";
        static const char header3_l[]="Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        static const char header4_l[]="zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3";
        static const char header6_l[]="Connection:keep-alive";

        headers_l=curl_slist_append(headers_l,header1_l);
        headers_l=curl_slist_append(headers_l,header2_l);
        headers_l=curl_slist_append(headers_l,header3_l);
        headers_l=curl_slist_append(headers_l,header4_l);
        headers_l=curl_slist_append(headers_l,header6_l);

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headers_l);
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR,Con(TMP_R,cookie_open.txt));
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);

#ifdef SKIP_HOSTNAME_VERFICATION
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
#endif

        fp=fopen(Con(TMP_R,xmldate1.xml),"w");
        if(fp==NULL){
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers_l);
            return -1;
        }
        curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,60);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        res=curl_easy_perform(curl);
        fclose(fp);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers_l);
        if(res != 0){
            wd_DEBUG("open_login_page [%d] failed!\n",res);
            return -1;
        }
    }
    else
        wd_DEBUG("url is wrong!!!");
}
int
        login_first(void){

    CURL *curl;
    CURLcode res;
    FILE *fp;
    char Myurl[MAXSIZE]="\0";
    char *data=parse_login_page();
    snprintf(Myurl, MAXSIZE, "%s","https://www.dropbox.com/login");
    struct curl_slist *headerlist=NULL;
    static const char buf[]="Expect:";

    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,buf);

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
        curl_easy_setopt(curl,CURLOPT_COOKIEFILE,Con(TMP_R,cookie_open.txt));//send first saved cookie
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR,Con(TMP_R,cookie_login.txt));
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);

        fp=fopen(Con(TMP_R,xmldate2.xml),"w");

        if(fp==NULL){
            curl_easy_cleanup(curl);
            curl_slist_free_all(headerlist);
            free(data);
            return -1;
        }

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);

        if(res != 0){

            free(data);
            wd_DEBUG("login_first [%d] failed!\n",res);
            return -1;
        }

    }
    free(data);
    return 0;
}

char *parse_login_page()
{
    FILE *fp;
    char *p,*m;
    char *s="name=\"t\" value=";
    char buf[1024],buff[100];
    char cont[256]="\0";
    char *data=NULL;
    data=(char *)malloc(sizeof(char *)*1024);
    memset(data,0,1024);
    char *url="https://www.dropbox.com/1/oauth/authorize";
    snprintf(cont, 256, "%s?oauth_token=%s",url,auth->tmp_oauth_token);
    memset(buf,'\0',sizeof(buf));
    memset(buff,'\0',sizeof(buff));

    fp=fopen(Con(TMP_R,xmldate1.xml),"r");

    while(!feof(fp)){
        fgets(buf,1024,fp);
        p=strstr(buf,s);
        if(p!=NULL){
            m=p;
            p=NULL;
            break;
        }
    }
    m=m+strlen(s)+1;
    p=strchr(m,'"');
    strncpy(buff,m,strlen(m)-strlen(p));
    char *cont_tmp=oauth_url_escape(cont);
    char *usr_tmp=oauth_url_escape(asus_cfg.user);
    char *pwd_tmp=oauth_url_escape(asus_cfg.pwd);
#if 1
    snprintf(data, sizeof(char *)*1024, "t=%s&lhs_type=default&cont=%s&signup_tag=oauth&signup_data=177967&login_email=%s&login_password=%s&%s&%s&%s",
            buff,cont_tmp,usr_tmp,pwd_tmp,"login_submit=1","remember_me=on","login_submit_dummy=Sign+in");
#else
    snprintf(data, sizeof(char *)*1024, "t=%s&cont=%s&signup_tag=oauth&signup_data=177967&display=desktop&login_email=%s&login_password=%s&%s&%s&%s",
            buff,cont_tmp,usr_tmp,pwd_tmp,"login_submit=1","remember_me=on","login_submit_dummy=Sign+in");
#endif
    fclose(fp);
    free(cont_tmp);
    free(usr_tmp);
    free(pwd_tmp);
    return data;

}
char *parse_cookie()
{
    FILE *fp;

    fp=fopen(Con(TMP_R,/cookie_login.txt),"r");

    char buf[1024]="\0";
    const char *m="	";
    char *p;
    char *s="gvc";

    while(!feof(fp)){
        fgets(buf,1024,fp);
        p=strstr(buf,s);
        if(p!=NULL){
            break;
        }
    }
    fgets(buf,1024,fp);
    char *cookie;
    cookie=malloc(128);
    memset(cookie,0,128);
    p=strtok(buf,m);

    int i=0;
    while(p!=NULL)
    {
        switch (i)
        {
        case 6:
            snprintf(cookie, 128, "%s", p);
            break;
        case 1:
            break;
        default:
            break;
        }
        i++;
        p=strtok(NULL,m);
    }
    if(cookie[strlen(buf)+2] == '\n')
    {
        cookie[strlen(buf)+2] = '\0';
    }
    fclose(fp);
    return cookie;

}
int login_second()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char Myurl[MAXSIZE]="\0";
    char *data=malloc(256);
    memset(data,0,256);
    char *cookie=parse_cookie();
#if 1
    snprintf(data, 256, "t=%s&allow_access=Allow&oauth_token=%s&display&osx_protocol",cookie,auth->tmp_oauth_token);
#else
    snprintf(data, 256, "t=%s&allow_access=Allow&saml_assertion&embedded&osx_protocol&oauth_token=%s&display&oauth_callback&user_id=150377145",cookie,auth->tmp_oauth_token);
#endif
#if 1
    snprintf(Myurl, MAXSIZE, "%s","https://www.dropbox.com/1/oauth/authorize");
#else
    snprintf(Myurl, MAXSIZE, "%s","https://www.dropbox.com/1/oauth/authorize_submit");
#endif
    struct curl_slist *headerlist=NULL;
    static const char buf[]="Expect:";

    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,buf);

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);


        curl_easy_setopt(curl,CURLOPT_COOKIEFILE,Con(TMP_R,cookie_login.txt));//send first saved cookie
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR,Con(TMP_R,tmp/cookie_login_2.txt));


        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);


#ifdef SKIP_HOSTNAME_VERFICATION
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
#endif

        fp=fopen(Con(TMP_R,xmldate3.xml),"w");


        if(fp==NULL){
            curl_easy_cleanup(curl);
            free(data);
            free(cookie);
            curl_slist_free_all(headerlist);
            return -1;
        }

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res != 0){
            free(data);
            free(cookie);
            wd_DEBUG("login_second [%d] failed!\n",res);
            return -1;
        }

    }

    free(data);
    free(cookie);
    return 0;
}

char *pare_login_second_response()
{
    FILE *f=fopen(Con(TMP_R,xmldate3.xml),"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
    char *data=malloc(len+1);fread(data,1,len,f);fclose(f);
    char *s="name=\"user_id\" value=\"";
    char *sb="\"";
    char *p = NULL;
    char *pt = NULL;
    char *user_id = NULL;
    int user_id_length;
    p=strstr(data,s);
    if(p!=NULL){
        p+=strlen(s);
        pt = strstr(p,sb);
        user_id_length = strlen(p) - strlen(pt);
        user_id = malloc(user_id_length+1);
        memset(user_id,0,user_id_length+1);
        printf("p=%s\n",p);
        printf("pt=%s\n",pt);
        snprintf(user_id,user_id_length+1,"%s",p);
    }
    my_free(data);

    wd_DEBUG("user_id = %s\n",user_id);
    return user_id;
}

int login_second_submit()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char Myurl[MAXSIZE]="\0";
    char *data=malloc(256);
    memset(data,0,256);
    char *cookie=parse_cookie();
    char *user_id = pare_login_second_response();
#if 0
    snprintf(data, 256, "t=%s&allow_access=Allow&oauth_token=%s&display&osx_protocol",cookie,auth->tmp_oauth_token);
#else
    snprintf(data, 256, "t=%s&allow_access=Allow&saml_assertion&embedded&osx_protocol&oauth_token=%s&display&oauth_callback&user_id=%s",cookie,auth->tmp_oauth_token,user_id);
#endif
#if 0
    snprintf(Myurl, MAXSIZE, "%s","https://www.dropbox.com/1/oauth/authorize");
#else
    snprintf(Myurl, MAXSIZE, "%s","https://www.dropbox.com/1/oauth/authorize_submit");
#endif
    free(user_id);
    struct curl_slist *headerlist=NULL;
    static const char buf[]="Expect:";

    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,buf);

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
        curl_easy_setopt(curl,CURLOPT_COOKIEFILE,Con(TMP_R,cookie_login.txt));//send first saved cookie
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR,Con(TMP_R,tmp/cookie_login_3.txt));
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);

#ifdef SKIP_HOSTNAME_VERFICATION
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
#endif

        fp=fopen(Con(TMP_R,xmldate4.xml),"w");


        if(fp==NULL){
            curl_easy_cleanup(curl);
            free(data);
            free(cookie);
            curl_slist_free_all(headerlist);
            return -1;
        }

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res != 0){
            free(data);
            free(cookie);
            wd_DEBUG("login_second_submit [%d] failed!\n",res);
            return -1;
        }

    }

    free(data);
    free(cookie);
    return 0;
}
int get_access_token()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *header;
    static const char buf[]="Expect:";
    header=makeAuthorize(2);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    if(curl){
        curl_easy_setopt(curl,CURLOPT_URL,"https://api.dropbox.com/1/oauth/access_token");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);

        fp=fopen(Con(TMP_R,data_2.txt),"w");

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            wd_DEBUG("get_access_token [%d] failed!\n",res);
            return res;
        }
    }
    free(header);
    int status;
    status=parse(Con(TMP_R,data_2.txt),2);
    if(status==-1)
        return -1;
    return 0;
}
#endif
int api_get_userspace()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *header;
    static const char buf[]="Expect:";
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,"https://api.dropboxapi.com/2/users/get_space_usage");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,data_3.txt),"w");
        if(fp == NULL)
        {
            free(header);
            return -1;
        }
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS, "null");
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            wd_DEBUG("get server space failed , id [%d] !\n",res);
            return -1;
        }
    }
    free(header);
    return 0;
}
int api_metadata_one(char *phref,cJSON *(*cmd_data)(char *filename), int *has_more_one, char **cursor)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    snprintf(myUrl, 128, "%s","https://api.dropboxapi.com/2/files/list_folder");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(phref) + 128);
        memset(output,0,strlen(phref) + 128);
        if(strcmp(phref, "/") == 0)
            snprintf(output, strlen(phref) + 128, "{\"path\": \"%s\"}", "");
        else
            snprintf(output, strlen(phref) + 128, "{\"path\": \"%s\"}", phref);

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,data_one.txt),"w");
        if(fp == NULL)
        {
            free(output);
            free(header);
            free(myUrl);
            return -1;
        }
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            free(myUrl);
            wd_DEBUG("api_metadata_one [%d] failed!\n",res);
            return -1;
        }
    }
    free(myUrl);
    free(header);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_one.txt));
    if(json){
        cJSON_printf_one(json, has_more_one, cursor);
        cJSON_Delete(json);
        return 0;
    }else
        return -1;
}
int api_metadata_test_dir(char *phref,proc_pt cmd_data)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    snprintf(myUrl,128, "%s","https://api.dropboxapi.com/2/files/get_metadata");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(phref) + 128);
        memset(output,0,strlen(phref) + 128);
        snprintf(output, strlen(phref) + 128, "{\"path\": \"%s\",\"include_deleted\": true}", phref);

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,data_test_dir.txt),"w");
        hd=fopen(Con(TMP_R,data_test_dir_header.txt),"w+");
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);
            free(header);
            free(myUrl);
            wd_DEBUG("api_metadata_test [%d] failed!\n",res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            while(!feof(hd))
            {
                fgets(tmp,sizeof(tmp),hd);
                printf("tmp_ : %s\n",tmp);
                if(strstr(tmp,"409") != NULL)
                {
                    free(myUrl);
                    free(header);
                    fclose(hd);
                    return -1;
                }
                else if(strstr(tmp,"200 OK") != NULL)
                    break;
            }
            fclose(hd);
        }
    }
    free(myUrl);
    free(header);

    int rs = 0;
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_test_dir.txt));
    if(json){
        rs = cJSON_printf_dir(json);
        cJSON_Delete(json);
        return rs;
    }else
        return -1;
}


int api_list_folder(char *phref,proc_pt cmd_data, int *has_more, char **cursor)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    snprintf(myUrl, 128, "%s","https://api.dropboxapi.com/2/files/list_folder");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(phref) + 128);
        memset(output,0,strlen(phref) + 128);
        if(strcmp(phref, "/") == 0)
            snprintf(output, strlen(phref) + 128, "{\"path\": \"%s\"}", "");
        else
            snprintf(output, strlen(phref) + 128, "{\"path\": \"%s\"}", phref);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,data_5.txt),"w");
        hd=fopen(Con(TMP_R,data_check_access_token.txt),"w");
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);
            free(header);
            free(myUrl);
            wd_DEBUG("api_list_folder %s [%d] failed!\n",phref,res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            wd_DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                fclose(hd);
                free(header);
                free(myUrl);
                return -1;
            }
            fclose(hd);
        }
    }
    free(myUrl);
    free(header);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_5.txt));
    if(json)
    {
        cJSON_printf(json,"entries", has_more, cursor);
        cJSON_Delete(json);
        return 0;
    }
    else
    {
        /*the file contents is error*/
        return -1;
    }

}

int api_list_continue(proc_pt cmd_data, int *has_more, char **cursor)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    snprintf(myUrl, 128, "%s","https://api.dropboxapi.com/2/files/list_folder/continue");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(*cursor) + 128);
        memset(output,0,strlen(*cursor) + 128);
        snprintf(output, strlen(*cursor) + 128, "{\"cursor\": \"%s\"}", *cursor);
        if(*cursor)
        {
            free(*cursor);
            *cursor=NULL;
        }
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,data_5.txt),"w");
        hd=fopen(Con(TMP_R,data_check_access_token.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);
            free(header);
            free(myUrl);
            wd_DEBUG("api_list_folder_continue  [%d] failed!\n",res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            wd_DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                fclose(hd);
                free(header);
                free(myUrl);
                return -1;
            }
            fclose(hd);
        }
    }
    free(myUrl);
    free(header);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_5.txt));
    if(json)
    {
        cJSON_printf(json,"entries", has_more, cursor);
        cJSON_Delete(json);
        return 0;
    }
    else
    {
        /*the file contents is error*/
        return -1;
    }

}

int api_list_continue_one(proc_pt cmd_data, int *has_more_one, char **cursor)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    snprintf(myUrl, 128, "%s","https://api.dropboxapi.com/2/files/list_folder/continue");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(*cursor) + 128);
        memset(output,0,strlen(*cursor) + 128);
        snprintf(output, strlen(*cursor) + 128, "{\"cursor\": \"%s\"}", *cursor);
        if(*cursor)
        {
            free(*cursor);
            *cursor=NULL;
        }
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,data_one.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            free(myUrl);
            wd_DEBUG("api_list_folder_continue  [%d] failed!\n",res);
            return -1;
        }
    }
    free(myUrl);
    free(header);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_one.txt));
    if(json)
    {
        cJSON_printf_one(json, has_more_one, cursor);
        cJSON_Delete(json);
        return 0;
    }
    else
    {
        /*the file contents is error*/
        return -1;
    }

}

/*
oldname & newname both server path
*/
int api_move(char *oldname,char *newname,int index,int is_changed_time,char *newname_r)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *header;
    hd=fopen(Con(TMP_R,data_hd.txt),"w+");
    static const char buf[]="Expect:";
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(oldname) + strlen(newname) + 128);
        memset(output,0,strlen(oldname) + strlen(newname) + 128);
        snprintf(output, strlen(oldname) + strlen(newname) + 128, "{\"from_path\": \"%s\",\"to_path\": \"%s\",\"autorename\": true}", oldname, newname);

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,"https://api.dropboxapi.com/2/files/move");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        fp=fopen(Con(TMP_R,data_4.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headerlist);
        fclose(fp);
        free(output);
        if(res!=0){

            fclose(hd);
            free(header);
            wd_DEBUG("rename %s failed,id [%d]!\n",oldname,res);
            return res;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            wd_DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"409")!=NULL)
            {
                cJSON *json = dofile(Con(TMP_R,data_4.txt));
                int move_error_flag=cJSON_printf_error(json,"error_summary");
                cJSON_Delete(json);
                if(1 == move_error_flag)
                {
                    if(newname_r == NULL)  //newname_r is local new name
                    {
                    char *localpath=serverpath_to_localpath(newname,index);
                    if(test_if_dir(localpath))
                    {

                        wd_DEBUG("it is folder\n");

                        res=dragfolder(localpath,index);
                        if(res != 0)
                        {
                            wd_DEBUG("dragfolder %s failed status = %d\n",localpath,res);
                            fclose(hd);
                            free(header);
                            free(localpath);
                            return res;
                            }
                        }
                        else
                        {
                            wd_DEBUG("it is file\n");
                            res=upload_file(localpath,newname,1,index);
                            if(res!=0)
                            {

                                fclose(hd);
                                free(header);
                                free(localpath);
                                return res;
                            }
                            else
                            {
                                cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                                time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                                cJSON_Delete(json);

                                ChangeFile_modtime(localpath,mtime);
                            }
                        }
                        free(localpath);
                    }
                    else
                    {
                        char *localpath = newname_r;
                        if(test_if_dir(localpath))
                        {
                            wd_DEBUG("it is folder\n");

                            res=dragfolder_old_dir(localpath,index,newname);
                            if(res != 0)
                            {
                                wd_DEBUG("dragfolder %s failed status = %d\n",localpath,res);
                                fclose(hd);
                                free(header);
                                return res;
                            }
                        }
                        else
                        {
                            wd_DEBUG("it is file\n");
                            res=upload_file(localpath,newname,1,index);
                            if(res!=0)
                            {
                                fclose(hd);
                                free(header);
                                return res;
                            }
                            else
                            {
                                cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                                time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                                cJSON_Delete(json);

                                ChangeFile_modtime(localpath,mtime);
                            }
                        }
                    }
                }
                else if(2 == move_error_flag)
                {
                    char *server_conflcit_name=get_server_exist(newname,index);
                    printf("server_conflict_name=%s\n",server_conflcit_name);
                    if(server_conflcit_name)
                    {
                        deal_big_low_conflcit(server_conflcit_name,oldname,newname,newname_r,index);
                        free(server_conflcit_name);
                    }
                    else
                    {
                        fclose(hd);
                        free(header);
                        return -1;
                    }
                }
            }
            else if(strstr(tmp,"200")!=NULL)
            {
                if(is_changed_time)
                {
                    char *localpath=serverpath_to_localpath(newname,index);
                    cJSON *json = dofile(Con(TMP_R,data_4.txt));
                    time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                    cJSON_Delete(json);
                    if(test_if_dir(localpath))
                    {
                        dragfolder_rename(localpath,index,mtime);
                    }
                    else
                    {
                        ChangeFile_modtime(localpath,mtime);
                    }
                    free(localpath);
                }
            }
        }
    }

    fclose(hd);
    wd_DEBUG("rename ok\n");
    free(header);
    return 0;
}
/*
fullname=>local
filename=>server
*/
int api_download(char *fullname,char *filename,int index)
{
    if(access(fullname,F_OK) == 0)
    {
        wd_DEBUG("Local has %s\n",fullname);
        unlink(fullname);
        add_action_item("remove",fullname,g_pSyncList[index]->server_action_list);
    }

    char *temp_suffix = ".asus.td";
    char *Localfilename_tmp=(char *)malloc(sizeof(char)*(strlen(fullname)+strlen(temp_suffix)+2));
    memset(Localfilename_tmp,0,strlen(fullname)+strlen(temp_suffix)+2);
    snprintf(Localfilename_tmp, sizeof(char)*(strlen(fullname)+strlen(temp_suffix)+2), "%s%s",fullname,temp_suffix);
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    hd=fopen(Con(TMP_R,api_download_header.txt),"w+");
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);

    snprintf(myUrl, 128, "%s","https://content.dropboxapi.com/2/files/download");
    char *header;
    static const char buf[]="Expect:";
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    char *output = NULL;
    output=(char *)malloc(strlen(filename) + 128);
    memset(output,0,strlen(filename)+128);
    snprintf(output, strlen(filename)+128, "Dropbox-API-Arg: {\"path\": \"%s\"}", filename);

    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, output);
    write_log(S_DOWNLOAD,"",fullname,index);
    free(output);
    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,Clientfp);
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);

        fp=fopen(Localfilename_tmp,"w");
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        res=curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);

            free(header);
            free(myUrl);
            wd_DEBUG("download %s failed,id:[%d]!\n",filename,res);
            unlink(Localfilename_tmp);
            free(Localfilename_tmp);
            char *error_message=write_error_message("download %s failed",filename);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if(res == 7)
            {
                return COULD_NOT_CONNECNT_TO_SERVER;
            }
            else
            {
                return res;
            }
        }
        else
        {
            rewind(hd);
            char tmp_name[256]="\0";
            fgets(tmp_name,sizeof(tmp_name),hd);
            wd_DEBUG("tmp:%s\n",tmp_name);
            fclose(hd);
            if(strstr(tmp_name,"409 path/not_found")!=NULL)
            {
                unlink(Localfilename_tmp);
            }
            else
            {
                if(rename(Localfilename_tmp,fullname)!=0)
                {
                    free(header);
                    free(myUrl);
                    unlink(Localfilename_tmp);
                    free(Localfilename_tmp);
                    return -1;
                }
            }
        }
    }
    free(Localfilename_tmp);
    free(header);
    free(myUrl);
    return 0;
}

long long int
        get_file_size(const char *file)
{
    struct stat file_info;

    if( !(file || *file) )
        return 0;

    if( file[0] == '-' )
        return 0;

    if( stat(file, &file_info) == -1 )
        return 0;
    else
        return(file_info.st_size);
}


int api_upload_post(char *filename,char *serverpath,int flag,int index)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *fp_1;
    FILE *fp_hd;
    struct stat filestat;

    if( stat(filename,&filestat) == -1)
    {
        wd_DEBUG("servr sapce full stat error:%s file not exist\n",filename);
        return -1;
    }

    fp_1=fopen(filename,"rb");
    if(fp_1 == NULL)
    {
        wd_DEBUG("Local has no %s\n",filename);
        return LOCAL_FILE_LOST;
    }

    char *header;
#ifdef OAuth1
    header=makeAuthorize(4);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/octet-stream" );
    curl=curl_easy_init();
    char myUrl[1024]="\0";
    wd_DEBUG("serverpath = %s\n",serverpath);
    snprintf(myUrl, 1024, "%s","https://content.dropboxapi.com/2/files/upload",serverpath);

    if(LOCAL_FILE.path != NULL)
        free(LOCAL_FILE.path);
    LOCAL_FILE.path = (char*)malloc(sizeof(char)*(strlen(filename) + 1));
    snprintf(LOCAL_FILE.path, sizeof(char)*(strlen(filename) + 1), "%s",filename);
    LOCAL_FILE.index = index;

    fp=fopen(Con(TMP_R,upload_chunk_commit.txt),"w");
    fp_hd=fopen(Con(TMP_R,upload_header.txt),"w+");

    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(serverpath) + 128);
        memset(output,0,strlen(serverpath)+128);
        if(flag)
            snprintf(output, strlen(serverpath)+128, "Dropbox-API-Arg: {\"path\": \"%s\",\"mode\": \"overwrite\",\"autorename\": true}", serverpath);
        else
            snprintf(output, strlen(serverpath)+128, "Dropbox-API-Arg: {\"path\": \"%s\",\"mode\": \"add\",\"autorename\": true}", serverpath);

        headerlist=curl_slist_append(headerlist, output);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_PUT,1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)filestat.st_size);
        curl_easy_setopt(curl,CURLOPT_READDATA,fp_1);
        curl_easy_setopt(curl,CURLOPT_READFUNCTION, my_read_func);
        curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10);//upload time_out
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,Clientfp);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);
        res=curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(fp_1);
        free(output);
        if(res!=0){
            fclose(fp_hd);
            free(header);
            wd_DEBUG("upload %s failed,id is [%d]!\n",filename,res);
            char *error_message=write_error_message("upload %s failed",filename);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if( res == 7 )
            {
                return COULD_NOT_CONNECNT_TO_SERVER;
            }
            else if( res == 28)
            {
                return CONNECNTION_TIMED_OUT;
            }
            else if( res == 35 )
            {
                return INVALID_ARGUMENT;
            }
            else
                return res;
        }
        else
        {
            free(header);
            fclose(fp_hd);
        }
    }
    return 0;
}

int my_progress_func(char *clientfp,double t,double d,double ultotal,double ulnow){
#if 1
    int sec;
    double  elapsed = 0;
    elapsed = time(NULL) - start_time;
    sec = (int)elapsed;
    if( sec > 0 )
    {
        if(sec % 5 == 0)
            wd_DEBUG("@%s %g / %g (%g %%)\n", clientfp, ulnow, ultotal, ulnow*100.0/ultotal);
    }
#endif

#if 1
    if(t > 1 && d > 10) // download
        wd_DEBUG("@@%s %10.0f / %10.0f (%g %%)\n", clientfp, d, t, d*100.0/t);
    else//upload
    {
        if(exit_loop==1){
            return -1;
        }
        wd_DEBUG("@@@%s %10.0f / %10.0f (%g %%)\n", clientfp, ulnow, ultotal, ulnow*100.0/ultotal);
    }
#endif
    return 0;
}

size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)

{
    if(exit_loop==1){
        return -1;
    }
    wd_DEBUG("download!\n");
    int len ;
    len = fwrite(ptr, size, nmemb, stream);
    return len;

}

size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if(exit_loop==1){
        return -1;
    }

    struct stat info;
    char *ret = NULL;
    int status = stat(LOCAL_FILE.path,&info);
    if(status == -1)
    {
        usleep(1000*100);
        ret = search_newpath(LOCAL_FILE.path,LOCAL_FILE.index);
        if(ret == NULL)
        {
            return -1;
        }
        else
        {
            if(LOCAL_FILE.path != NULL)
                free(LOCAL_FILE.path);
            LOCAL_FILE.path = (char *)malloc(sizeof(char)*(strlen(ret) + 1));
            snprintf(LOCAL_FILE.path, sizeof(char)*(strlen(ret) + 1), "%s",ret);
            free(ret);
        }
    }

    int len;
    len = fread(ptr, size, nmemb, stream);
    return len;
}

int api_upload_session(char *buffer,char *upload_id,unsigned long offset,unsigned long chunk,int index,char *filepath)
{
    wd_DEBUG("upload_id: %s\n",upload_id);
    FILE *fp;
    FILE *fp_2;
    fp_2=fopen(Con(TMP_R,swap),"w+");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(4);
#else
    header=makeAuthorize(1);
#endif

    fwrite(buffer,4000000/10,10,fp_2);
    rewind(fp_2);
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist=NULL;
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/octet-stream");
    curl=curl_easy_init();
    char *myUrl=(char *)malloc(2046);
    memset(myUrl,0,2046);
    char *output = NULL;
    output=(char *)malloc(256);
    memset(output, 0, 256);
    if( upload_id ==NULL )
    {
        snprintf(output, 256, "%s", "Dropbox-API-Arg: {\"close\": false}");
        snprintf(myUrl, 2046, "%s","https://content.dropboxapi.com/2/files/upload_session/start");
    }
    else
    {
        snprintf(output, 256, "Dropbox-API-Arg: {\"cursor\": {\"session_id\": \"%s\",\"offset\": %lu},\"close\": false}", upload_id, chunk);
        snprintf(myUrl, 2046, "%s", "https://content.dropboxapi.com/2/files/upload_session/append_v2");
    }
    headerlist=curl_slist_append(headerlist,output);
    free(output);
    wd_DEBUG("myUrl: %s\n",myUrl);

    if(LOCAL_FILE.path != NULL)
        free(LOCAL_FILE.path);
    LOCAL_FILE.path = (char*)malloc(sizeof(char)*(strlen(filepath) + 1));
    snprintf(LOCAL_FILE.path, sizeof(char)*(strlen(filepath) + 1), "%s",filepath);
    LOCAL_FILE.index = index;

    if(curl){
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_PUT,1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl,CURLOPT_READDATA,fp_2);
        curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10);//upload time_out
        curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,(curl_off_t)offset);//put Content-Length
        fp=fopen(Con(TMP_R,upload_chunk_1.txt),"w+");
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,Clientfp);
        curl_easy_setopt(curl,CURLOPT_READFUNCTION, my_read_func);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);

        res=curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(fp_2);
        if(res!=CURLE_OK){
            free(header);
            free(myUrl);
            wd_DEBUG("upload fail,id is %d!\n",res);
            char *error_message=write_error_message("upload %s failed",filepath);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if( res == 7 )
            {
                return COULD_NOT_CONNECNT_TO_SERVER;
            }
            else if( res == 28)
            {
                return CONNECNTION_TIMED_OUT;
            }
            else if( res == 35 )
            {
                return INVALID_ARGUMENT;
            }
            else
                return res;

        }
    }
    free(header);
    free(myUrl);
    return 0;
}
char *get_upload_id()
{
    FILE *fp;
    fp=fopen(Con(TMP_R,upload_chunk_1.txt),"r");
    if(fp==NULL)
    {
        wd_DEBUG("open %s fialed\n","upload_chunk_1.txt");
        return NULL;
    }
    char buff[1024]="\0";
    char *p=NULL,*m=NULL;
    char *upload_id;
    upload_id=(char *)malloc(sizeof(char *)*512);
    memset(upload_id,0,512);
    fgets(buff,sizeof(buff),fp);
    p=strstr(buff,"session_id");
    p=p+strlen("session_id")+4;
    m=strchr(p,'"');
    snprintf(upload_id,strlen(p)-strlen(m)+1,"%s",p);
    wd_DEBUG("upload_id : %s,%d\n",upload_id,strlen(upload_id));
    if(upload_id[strlen(upload_id)] == '\0')
    {
        upload_id[strlen(upload_id)] = '\0';
    }
    fclose(fp);
    return upload_id;
}
int api_upload_session_finish(char *buffer,char *upload_id,unsigned long offset,unsigned long chunk,int index,char *filepath, char*serverpath,int flag)
{
    FILE *fp;
    FILE *fp_2;
    FILE *fp_hd;
    fp_2=fopen(Con(TMP_R,swap),"w+");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(4);
#else
    header=makeAuthorize(1);
#endif
    fwrite(buffer,4000000/10,10,fp_2);
    rewind(fp_2);
    CURL *curl;
    CURLcode res;
    curl=curl_easy_init();

    char *myUrl=(char *)malloc(2046);
    memset(myUrl,0,2046);
    snprintf(myUrl, 2046, "%s","https://content.dropboxapi.com/2/files/upload_session/finish");

    char *output = NULL;
    output=(char *)malloc(strlen(upload_id) + strlen(serverpath) + 256);
    memset(output, 0, strlen(upload_id) + strlen(serverpath) + 256);
    if(flag)
        snprintf(output, strlen(upload_id) + strlen(serverpath) + 256, "Dropbox-API-Arg: {\"cursor\": {\"session_id\": \"%s\",\"offset\": %lu},\"commit\": {\"path\": \"%s\",\"mode\": \"overwrite\",\"autorename\": true}}", upload_id, chunk, serverpath);
    else
        snprintf(output, strlen(upload_id) + strlen(serverpath) + 256, "Dropbox-API-Arg: {\"cursor\": {\"session_id\": \"%s\",\"offset\": %lu},\"commit\": {\"path\": \"%s\",\"mode\": \"add\",\"autorename\": true}}", upload_id, chunk, serverpath);

    struct curl_slist *headerlist=NULL;
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/octet-stream");
    headerlist=curl_slist_append(headerlist,output);
     free(output);
    wd_DEBUG("myUrl: %s\n",myUrl);

    if(LOCAL_FILE.path != NULL)
        free(LOCAL_FILE.path);
    LOCAL_FILE.path = (char*)malloc(sizeof(char)*(strlen(filepath) + 1));
    snprintf(LOCAL_FILE.path, sizeof(char)*(strlen(filepath) + 1), "%s",filepath);
    LOCAL_FILE.index = index;

    if(curl){
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_PUT,1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl,CURLOPT_READDATA,fp_2);
        //curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);
        curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10);//upload time_out
        curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,(curl_off_t)offset);//put Content-Length
        fp=fopen(Con(TMP_R,upload_chunk_commit.txt),"w");
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,Clientfp);
        curl_easy_setopt(curl,CURLOPT_READFUNCTION, my_read_func);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        fp_hd=fopen(Con(TMP_R,upload_header.txt),"w+");
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);
        res=curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(fp_2);
        if(res!=CURLE_OK){
            fclose(fp_hd);
            free(header);
            free(myUrl);
            wd_DEBUG("upload fail,id is %d!\n",res);
            char *error_message=write_error_message("upload %s failed",filepath);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if( res == 7 )
            {
                return COULD_NOT_CONNECNT_TO_SERVER;
            }
            else if( res == 28)
            {
                return CONNECNTION_TIMED_OUT;
            }
            else if( res == 35 )
            {
                return INVALID_ARGUMENT;
            }
            else
                return res;
        }
        fclose(fp_hd);
    }

    free(header);
    free(myUrl);
    return 0;
}
/*
 1=>enough
 0=>not enough
 status=>get accout_info fail
*/
int is_server_enough(char *filename)
{
    long long int size_lo=get_file_size(filename);
    if(size_lo == -1)
        return -1;
    int status;
    status=api_get_userspace();
    long long int size_server;
    if(status == 0)
    {
        cJSON *json = dofile(Con(TMP_R,data_3.txt));
        cJSON_printf(json,"used", NULL, NULL);
        cJSON_Delete(json);
        size_server=server_allocated - server_used;
        wd_DEBUG("server free space is %lld\n",size_server);

        if(size_server> size_lo)
        {
            wd_DEBUG("server freespace is enough!\n");

            return 1;
        }
        else
        {
            wd_DEBUG("server freespace is not enough!\n");
            return 0;
        }
    }
    return status;
}

int free_upload_chunk_info()
{
    if(upload_chunk != NULL)
    {
        if(upload_chunk->upload_id != NULL)
            free(upload_chunk->upload_id);
        if(upload_chunk->filename != NULL)
            free(upload_chunk->filename);
        free(upload_chunk);
        upload_chunk = NULL;
    }
    return 1;
}

int add_upload_chunked_info(char *filename,char *upload_id,unsigned long offset,unsigned long chunk)
{
    upload_chunk = (Upload_chunked *)malloc(sizeof(Upload_chunked));
    memset(upload_chunk,0,sizeof(upload_chunk));
    upload_chunk->filename = (char *)malloc(sizeof(char)*(strlen(filename)+1));
    memset(upload_chunk->filename,'\0',strlen(filename)+1);
    snprintf(upload_chunk->filename, sizeof(char)*(strlen(filename)+1), "%s",filename);
    upload_chunk->upload_id = (char *)malloc(sizeof(char)*(strlen(upload_id)+1));
    memset(upload_chunk->upload_id,'\0',strlen(upload_id)+1);
    snprintf(upload_chunk->upload_id, sizeof(char)*(strlen(upload_id)+1), "%s",upload_id);
    upload_chunk->chunk = chunk;
    upload_chunk->offset = offset;
    return 1;
}

int api_upload_chunk_continue(char *filename,int index,int flag,char *serverpath,unsigned long size)
{
    wd_DEBUG("api_upload_chunk_continue\n");
    int res=0;
    unsigned long offset=4*1000*1000;//4M chunk
    unsigned long chunk = 0;
    FILE *fp_1;
    char *upload_id = (char *)malloc(sizeof(char)*(strlen(upload_chunk->upload_id)+1));
    memset(upload_id,'\0',strlen(upload_chunk->upload_id)+1);
    snprintf(upload_id, sizeof(char)*(strlen(upload_chunk->upload_id)+1), "%s",upload_chunk->upload_id);
    char *buffer = malloc(4000000);
    fp_1=fopen(filename,"r");

    if(fp_1 == NULL)
    {
        wd_DEBUG("open %s failed\n",filename);
        return -1;
    }
    fseek(fp_1,upload_chunk->chunk,SEEK_SET);

    chunk = upload_chunk->chunk;
    while(chunk+offset < size)
    {
        memset(buffer,0,4000000);
        fread(buffer,4000000/10,10,fp_1);
        res=api_upload_session(buffer,upload_id,offset,chunk,index,filename);
        if(res != 0)
        {
            free_upload_chunk_info();
            add_upload_chunked_info(filename,upload_id,offset,chunk);

            fclose(fp_1);
            free(upload_id);
            free(buffer);
            return res;
        }
        chunk=chunk+offset;
    }
    offset = size-chunk;
    memset(buffer,0,4000000);
    fread(buffer,4000000/10,10,fp_1);
    res=api_upload_session_finish(buffer,upload_id,offset,chunk,index,filename,serverpath,flag);
    if(res != 0)
    {
        free_upload_chunk_info();
        fclose(fp_1);
        free(buffer);
        free(upload_id);
        return res;
    }

    free_upload_chunk_info();
    free(buffer);
    free(upload_id);
    fclose(fp_1);
    return 0;
}

int api_upload_chunk(char *filename,int index,int flag,char *serverpath,unsigned long size)
{
    int res=0;
    unsigned long offset=4*1000*1000;//4M chunk
    unsigned long chunk = 0;
    FILE *fp_1;
    char *upload_id;
    fp_1=fopen(filename,"r");

    if(fp_1 == NULL)
    {
        wd_DEBUG("open %s failed\n",filename);
        return -1;
    }
    char *buffer = malloc(4000000);
    memset(buffer,0,4000000);
    fread(buffer,4000000/10,10,fp_1);
    start_time = time(NULL);
    res = api_upload_session(buffer,NULL,offset,0,index,filename);//offset :Content-length;chunk :offset
    if(res != 0)
    {
        fclose(fp_1);
        free(buffer);
        return res;
    }

    upload_id = get_upload_id();
    if(upload_id == NULL)
    {
        fclose(fp_1);
        free(buffer);
        return -1;
    }
    chunk = offset;
    while(chunk+offset <= size)
    {
        memset(buffer,0,4000000);
        fread(buffer,4000000/10,10,fp_1);
        res=api_upload_session(buffer,upload_id,offset,chunk,index,filename);
        if(res != 0)
        {
            free_upload_chunk_info();
            add_upload_chunked_info(filename,upload_id,offset,chunk);

            fclose(fp_1);
            free(upload_id);
            free(buffer);
            return res;
        }
        chunk=chunk+offset;
    }
    offset=size-chunk;
    memset(buffer,0,4000000);
    fread(buffer,4000000/10,10,fp_1);
    res=api_upload_session_finish(buffer,upload_id,offset,chunk,index,filename,serverpath,flag);
    if(res!=0)
    {
        free_upload_chunk_info();
        fclose(fp_1);
        free(upload_id);
        free(buffer);
        return res;
    }

    free_upload_chunk_info();
    free(upload_id);
    free(buffer);
    fclose(fp_1);
    return 0;
}
/*
 filename=>local full filename
 serverpath=>server full filename
 third para:ture or false
 ture=>server filename will be overwrite
 false=>server filename will not overwrite,but local filename will be rename ,ex xxx.txt=>xxx(1).txt
 */
int upload_file(char *filename,char *serverpath,int flag,int index)
{


    wd_DEBUG("*****************Upload***************\n");

    if(access(filename,F_OK) != 0)
    {

        wd_DEBUG("Local has no %s\n",filename);
        return LOCAL_FILE_LOST;
    }
#if 1
    int server_enough;
    server_enough=is_server_enough(filename);
    if(server_enough != 1)
    {
        if(server_enough == 0)
        {
            write_log(S_ERROR,"server space is not enough!","",index);
            action_item *item;
            item = get_action_item("upload",filename,g_pSyncList[index]->up_space_not_enough_list,index);
            if(item == NULL)
            {
                add_action_item("upload",filename,g_pSyncList[index]->up_space_not_enough_list);
            }
            return SERVER_SPACE_NOT_ENOUGH;
        }
        else
        {
            return server_enough;
        }
    }
#endif
    unsigned long MAX_FILE_SIZE=140*1000*1000;

    int res=0;
    write_log(S_UPLOAD,"",filename,index);

    long long int size = get_file_size(filename);
    if( size == -1)
    {
        if(access(filename,F_OK) != 0)
        {

            wd_DEBUG("Local has no %s\n",filename);
            return LOCAL_FILE_LOST;
        }
    }
    if( size >= MAX_FILE_SIZE )
    {
        if(upload_chunk != NULL)
        {
            time_t cur_ts = time(NULL);
            if(strcmp(filename,upload_chunk->filename) == 0)
            {
                res = api_upload_chunk_continue(filename,index,flag,serverpath,size);
                if(res != 0)
                {
                    return res;
                }
            }
            else
            {
                free_upload_chunk_info();
                res = api_upload_chunk(filename,index,flag,serverpath,size);
                if(res != 0)
                {
                    return res;
                }
            }
        }
        else
        {
            res = api_upload_chunk(filename,index,flag,serverpath,size);
            if(res != 0)
            {
                return res;
            }
        }
    }
    else
    {
        res=api_upload_post(filename,serverpath,flag,index);
        if(res!=0)
        {
            return res;
        }
    }
    return 0;
}
int api_delete(char *herf,int index)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(herf) + 128);
        memset(output,0,strlen(herf) + 128);
        snprintf(output, strlen(herf) + 128, "{\"path\": \"%s\"}", herf);
        printf("output=%s!!\n", output);

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,"https://api.dropboxapi.com/2/files/delete");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        fp=fopen(Con(TMP_R,api_delete.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            char error_info[100];
            memset(error_info,0,sizeof(error_info));
            snprintf(error_info, 100, "%s%s","delete fail ",herf);
            wd_DEBUG("delete %s failed,id [%d]!\n",herf,res);
            return res;
        }
    }
    free(header);
    return 0;
}
/*
 lcoalpath=>local name
 folderpath=>server path
*/
int api_create_folder(char *localpath,char *foldername)
{

    wd_DEBUG("****************create_folder****************\n");

    if(access(localpath,0) != 0)
    {

        wd_DEBUG("Local has no %s\n",localpath);

        return LOCAL_FILE_LOST;
    }

    CURL *curl;
    CURLcode res;
    FILE *fp;
    fp=fopen(Con(TMP_R,create_folder.txt),"w");

    FILE *fp_hd;
    fp_hd=fopen(Con(TMP_R,create_folder_header.txt),"w");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(foldername) + 128);
        memset(output,0,strlen(foldername)+128);
        snprintf(output, strlen(foldername)+128, "{\"path\": \"%s\",\"autorename\": true}", foldername);

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,"https://api.dropboxapi.com/2/files/create_folder");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(fp_hd);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            wd_DEBUG("create %s failed,is [%d] !\n",foldername,res);
            return res;
        }
        else
        {
            /*deal big or small case sentive problem,will return 409*/
            if(parse_create_folder(Con(TMP_R,create_folder_header.txt)))
            {
                cJSON *json = dofile(Con(TMP_R,create_folder.txt));
                int cf_error_flag=cJSON_printf_error(json,"error_summary");
                cJSON_Delete(json);
                if(3 == cf_error_flag)
                {
#ifndef MULTI_PATH
                    char *server_conflcit_name=get_server_exist(foldername,0);
                    if(server_conflcit_name)
                    {
                        char *local_conflcit_name = serverpath_to_localpath(server_conflcit_name,0);
                        char *g_newname = get_confilicted_name_case(local_conflcit_name,1);
                        my_free(local_conflcit_name);

                        updata_socket_list(localpath,g_newname,0);

                        if(access(localpath,0) == 0)
                        {
                            add_action_item("rename",g_newname,g_pSyncList[0]->server_action_list);
                            rename(localpath,g_newname);
                        }

                        char *s_newname = localpath_to_serverpath(g_newname,0);
                        int status=0;
                        do
                        {
                            status = api_create_folder(g_newname,s_newname);
                        }while(status != 0 && !exit_loop);
                        my_free(g_newname);
                        my_free(s_newname);
                    }
#endif
                }
            }
        }
    }
    free(header);
    return 0;
}
int parse_config_mutidir(char *path,struct asus_config *config)
{
    FILE *fp;
    wd_DEBUG("filename:%s\n",path);
    char buf[512];
    char *buffer = buf;
    char *p;
    int i = 0;
    int k = 0;
    int j = 0;

    memset(buf,0,sizeof(buf));

    if (access(path,0) == 0)
    {
        if(( fp = fopen(path,"r"))==NULL)
        {
            fprintf(stderr,"read Cloud error");
            return -1;
        }
        else
        {
            while(fgets(buffer,512,fp)!=NULL)
            {
                if(buffer[strlen(buffer)-1] == '\n')
                {
                    buffer[strlen(buffer)-1] = '\0';
                }
                p=buffer;
                printf("p is %s,outer is %s\n",p,buffer);
                switch (i)
                {
                case 0 :
                    config->type = atoi(p);
                    break;
                case 1:
                    config->enable = atoi(p);
                    break;
                case 2:
                    strncpy(config->user,p,strlen(p));
                    if(config->user[strlen(config->user)-1]=='\n')
                    {
                        config->user[strlen(config->user)-1]='\0';
                    }
                    break;
                    case 3:

                    strncpy(config->pwd,p,strlen(p));
                    if(config->pwd[strlen(config->pwd)-1]=='\n')
                    {
                        config->pwd[strlen(config->pwd)-1]='\0';
                    }
                    break;
                    case 4:
                    config->dir_number = atoi(p);
                    config->prule = (struct asus_rule **)malloc(sizeof(struct asus_rule*)*config->dir_number);
                    if(NULL == config->prule)
                    {
                        return -1;
                    }
                    break;
                    default:
                    k = j / 2 ;
                    if(i % 2 == 1)
                    {
                        config->prule[k] = (struct asus_rule*)malloc(sizeof(struct asus_rule));
                        config->prule[k]->rule = atoi(p);
                    }
                    else
                    {
                        char *dp;
                        snprintf(config->prule[k]->path, 512, "%s", p);
                        dp=strrchr(config->prule[k]->path,'/');
                        snprintf(config->prule[k]->base_path,strlen(config->prule[k]->path)-strlen(dp)+1,"%s",config->prule[k]->path);
                        config->prule[k]->base_path_len=strlen(config->prule[k]->path)-strlen(dp);
                        snprintf(config->prule[k]->rooturl, 512, "%s", dp);
                        config->prule[k]->rooturl_len=strlen(dp);
                    }
                    j++;
                    break;
                }
                i++;
            }

            fclose(fp);
            return 0;
        }
    }
    else
        return -1;

}
void init_globar_var()
{
#if TOKENFILE
    sync_disk_removed = 0;
    disk_change = 0;
#endif
    upload_chunk = NULL;
    access_token_expired = 0;
    exit_loop = 0;
    stop_progress = 0;
    finished_initial=0;
    snprintf(log_path, MAX_LENGTH, "/tmp/smartsync/.logs");
    my_mkdir("/tmp/smartsync");
    my_mkdir("/tmp/smartsync/dropbox");
    my_mkdir("/tmp/smartsync/dropbox/config");
    my_mkdir("/tmp/smartsync/dropbox/script");
    my_mkdir("/tmp/smartsync/dropbox/temp");
    my_mkdir("/tmp/smartsync/dropbox/cert");
#ifdef NVRAM_
    my_mkdir("/tmp/smartsync/script");
#endif
    my_mkdir(TMP_R);
    my_mkdir("/tmp/smartsync/.logs");
    snprintf(general_log, MAX_CONTENT, "%s/dropbox",log_path);
    snprintf(trans_excep_file, MAX_LENGTH, "%s/dropbox_errlog",log_path);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_socket, NULL);
    pthread_mutex_init(&mutex_receve_socket, NULL);
    pthread_mutex_init(&mutex_log, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond_socket, NULL);
    pthread_cond_init(&cond_log, NULL);
    memset(&asus_cfg,'\0',sizeof(struct asus_config));
#if TOKENFILE
    memset(&asus_cfg_stop,0,sizeof(struct asus_config));
    if(initial_tokenfile_info_data(&tokenfile_info_start) == NULL)
    {
        return;
    }
#endif
}
void read_config()
{
#if TOKENFILE
#ifdef NVRAM_
    if(convert_nvram_to_file_mutidir(CONFIG_PATH,&asus_cfg) == -1)
    {
        wd_DEBUG("convert_nvram_to_file fail\n");
        return;
    }
#else
#ifndef WIN_32
    if(create_webdav_conf_file(&asus_cfg) == -1)
    {
        wd_DEBUG("create_dropbox_conf_file fail\n");
        return;
    }
#endif
#endif
#endif
    int i;
    if(parse_config_mutidir(CONFIG_PATH,&asus_cfg) == -1)
    {
        wd_DEBUG("parse_config_mutidir fail\n");
        no_config = 1;
        return;
    }

    wd_DEBUG("%d,%s,%s,%d,%d\n",asus_cfg.type,asus_cfg.user,asus_cfg.pwd,
             asus_cfg.enable,asus_cfg.dir_number);
#ifdef SMARTSYNCIPK
 if(asus_cfg.type != 3)
    {
        wd_DEBUG("asus_cfg.type=%d\n",asus_cfg.type);
        no_config = 1;
        return;
    }
#endif

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        write_log(S_INITIAL,"","",i);
        wd_DEBUG("rule is %d,path is %s,rooturl is %s\n",asus_cfg.prule[i]->rule,asus_cfg.prule[i]->path,asus_cfg.prule[i]->rooturl);
    }
    snprintf(username, 256, "%s", asus_cfg.user);
    snprintf(password, 256, "%s", asus_cfg.pwd);
    wd_DEBUG("222\n");
    if( strlen(username) == 0 )
    {
        wd_DEBUG("username is blank ,please input your username and passwrod\n");
        no_config = 1;
    }

    no_config = 0 ;
    exit_loop = 0;
}

int sync_initial_again(int index)
{
    int i,status,ret;

    i = index;

    ret = 1;
    if(exit_loop == 0)
    {
#ifdef MULTI_PATH
            if(api_metadata_test_dir(asus_cfg.prule[i]->rooturl,dofile)!=0)
            {
                wd_DEBUG("\nserver sync path is not exist,need create\n");
                api_create_folder(asus_cfg.prule[i]->base_path,asus_cfg.prule[i]->rooturl);
            }
#endif
        free_server_tree(g_pSyncList[i]->ServerRootNode);
        g_pSyncList[i]->ServerRootNode = NULL;

        g_pSyncList[i]->ServerRootNode = create_server_treeroot();
#ifdef MULTI_PATH
        status = browse_to_tree(asus_cfg.prule[i]->rooturl,g_pSyncList[i]->ServerRootNode);
#else
        status = browse_to_tree("/",g_pSyncList[i]->ServerRootNode);
#endif

        usleep(1000*200);
        if(status != 0)
            return status;

        if(exit_loop == 0)
        {

            if(test_if_dir_empty(asus_cfg.prule[i]->path))
            {

                g_pSyncList[i]->init_completed=1;
                g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                ret = 0;
            }
            else
            {

                wd_DEBUG("######## init sync folder,please wait......#######\n");

                if(g_pSyncList[i]->ServerRootNode->Child != NULL)
                {
                    ret=sync_local_with_server(g_pSyncList[i]->ServerRootNode->Child,sync_local_with_server_init,i);
                }

                wd_DEBUG("######## init sync folder end#######\n");

                if(ret != 0 )
                    return ret;
                g_pSyncList[i]->init_completed = 1;
                g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
            }
        }
    }
    if(ret == 0)
        write_log(S_SYNC,"","",i);

    return ret;
}

int sync_initial()
{
    int i,status,ret;
    for(i=0;i<asus_cfg.dir_number;i++)
    {


        if(exit_loop)
            break;
#if TOKENFILE
        if(disk_change)
        {
            check_disk_change();
        }

        if(g_pSyncList[i]->sync_disk_exist == 0)
        {
            write_log(S_ERROR,"Sync disk unplug!","",i);
            continue;
        }
#endif
        if(g_pSyncList[i]->init_completed)
            continue;

        ret = 1;
        if(exit_loop == 0)
        {
#ifdef MULTI_PATH
            if(api_metadata_test_dir(asus_cfg.prule[i]->rooturl,dofile)!=0)
            {
                wd_DEBUG("\nserver sync path is not exist,need create\n");
                api_create_folder(asus_cfg.prule[i]->base_path,asus_cfg.prule[i]->rooturl);
            }
#endif
            free_server_tree(g_pSyncList[i]->ServerRootNode);
            g_pSyncList[i]->ServerRootNode = NULL;

            g_pSyncList[i]->ServerRootNode = create_server_treeroot();
#ifdef MULTI_PATH
            status = browse_to_tree(asus_cfg.prule[i]->rooturl,g_pSyncList[i]->ServerRootNode);
#else
            status = browse_to_tree("/",g_pSyncList[i]->ServerRootNode);
#endif
            usleep(1000*200);
            if(status != 0)
                continue;

            if(exit_loop == 0)
            {

                if(test_if_dir_empty(asus_cfg.prule[i]->path))
                {
                    if(asus_cfg.prule[i]->rule != 2)
                    {

                        wd_DEBUG("######## init sync empty folder,please wait......#######\n");

                        if(g_pSyncList[i]->ServerRootNode->Child != NULL)
                        {
                            ret=initMyLocalFolder(g_pSyncList[i]->ServerRootNode->Child,i);
                            if(ret != 0 )
                                continue;
                            g_pSyncList[i]->init_completed=1;
                            g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                        }

                        wd_DEBUG("######## init sync empty folder end#######\n");

                    }
                    else
                    {
                        g_pSyncList[i]->init_completed=1;
                        g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                        /*
                         fix upload only rule local is empty,UI log is still show "initial"
                        */
                        ret = 0;
                    }
                }
                else
                {

                    wd_DEBUG("######## init sync folder,please wait......#######\n");

                    if(g_pSyncList[i]->ServerRootNode->Child != NULL)
                    {
                        ret=sync_local_with_server(g_pSyncList[i]->ServerRootNode->Child,sync_local_with_server_init,i);
                    }

                    wd_DEBUG("######## init sync folder end#######\n");

                    if(ret != 0 )
                        continue;
                    g_pSyncList[i]->init_completed = 1;
                    g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                }
            }
        }
        if(ret == 0)
            write_log(S_SYNC,"","",i);

    }


    return ret;
}
int sync_local_with_server(Server_TreeNode *treenode,int(* sync_fun)(Server_TreeNode *,Browse*,Local*,int),int index)
{
    if(treenode->parenthref == NULL)
        return 0;
    Local *localnode;
    char *localpath;
    int ret=0;
#ifdef MULTI_PATH
    localpath=serverpath_to_localpath(treenode->parenthref,index);
#else
    if(strcmp(treenode->parenthref,"/") == 0)
    {
        localpath = (char *)malloc(sizeof(char)*(asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2));
        memset(localpath,'\0',asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2);
        snprintf(localpath, sizeof(char)*(asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2), "%s%s",asus_cfg.prule[index]->base_path,asus_cfg.prule[index]->rooturl);
    }
    else
    {
        localpath=serverpath_to_localpath(treenode->parenthref,index);
    }
#endif

    localnode=Find_Floor_Dir(localpath);
    free(localpath);

    if(localnode != NULL)
    {
        ret=sync_fun(treenode,treenode->browse,localnode,index);
        if(ret != 0 && ret != SERVER_SPACE_NOT_ENOUGH
           && ret != LOCAL_FILE_LOST)
        {
            free_localfloor_node(localnode);
            return ret;
        }
        free_localfloor_node(localnode);
    }
    if(treenode->Child != NULL && !exit_loop)
    {
        ret =sync_local_with_server(treenode->Child,sync_fun,index);
        if(ret != 0 && ret != SERVER_SPACE_NOT_ENOUGH
           && ret != LOCAL_FILE_LOST)
        {
            return ret;
        }
    }
    if(treenode->NextBrother != NULL && !exit_loop)
    {
        ret = sync_local_with_server(treenode->NextBrother,sync_fun,index);
        if(ret != 0 && ret != SERVER_SPACE_NOT_ENOUGH
           && ret != LOCAL_FILE_LOST)
        {
            return ret;
        }
    }
    return ret;
}
int sync_local_with_server_init(Server_TreeNode *treenode,Browse *perform_br,Local *perform_lo,int index)
{
    wd_DEBUG("parentf = %s\n",treenode->parenthref);
    wd_DEBUG("perform_br->foldernumber = %d,perform_br->filenumber = %d,\n",perform_br->foldernumber,perform_br->filenumber);
    wd_DEBUG("perform_lo->foldernumber = %d,perform_lo->filenumber = %d,\n",perform_lo->foldernumber,perform_lo->filenumber);
    if(perform_br == NULL || perform_lo == NULL)
    {
        return 0;
    }
    int ret = 0;

    CloudFile *foldertmp=NULL;
    CloudFile *filetmp=NULL;
    LocalFolder *localfoldertmp;
    LocalFile *localfiletmp;
    if(perform_br->foldernumber > 0)
        foldertmp = perform_br->folderlist->next;
    if(perform_br->filenumber > 0)
        filetmp = perform_br->filelist->next;
    localfoldertmp=perform_lo->folderlist->next;
    localfiletmp=perform_lo->filelist->next;

    /****************handle files****************/
    if(perform_br->filenumber == 0 && perform_lo->filenumber !=0)
    {
        while(localfiletmp != NULL && !exit_loop)
        {
            if(asus_cfg.prule[index]->rule != 1)
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }

                char *serverpath=localpath_to_serverpath(localfiletmp->path,index);
                ret=upload_file(localfiletmp->path,serverpath,1,index);
                if(ret == 0 || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
                {
                    if(ret == 0)
                    {
                        cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                        time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                        cJSON_Delete(json);
                        ChangeFile_modtime(localfiletmp->path,mtime);
                        free(serverpath);
                    }
                }
                else
                {
                    free(serverpath);
                    return ret;
                }
            }
            else
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->download_only_socket_head);
            }
            localfiletmp = localfiletmp->next;
        }
    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber ==0)
    {
        while(filetmp != NULL && !exit_loop)
        {
            if(asus_cfg.prule[index]->rule != 2)
            {
                if(is_local_space_enough(filetmp,index))
                {
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    char *localpath= serverpath_to_localpath(filetmp->href,index);
                    add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                    ret=api_download(localpath,filetmp->href,index);
                    if(ret == 0)
                    {
                        ChangeFile_modtime(localpath,filetmp->mtime);
                        free(localpath);
                    }
                    else
                    {
                        free(localpath);
                        return ret;
                    }

                }
                else
                {
                    write_log(S_ERROR,"local space is not enough!","",index);
                    add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                }
            }
            filetmp=filetmp->next;
        }
    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber !=0)
    {

        while(localfiletmp != NULL && !exit_loop) //is based by localfiletmp
        {
            int cmp=1;
            char *serverpath=localpath_to_serverpath(localfiletmp->path,index);
            while(filetmp != NULL)
            {
                if((cmp=strcmp(serverpath,filetmp->href)) == 0)
                {
                    free(serverpath);
                    break;
                }
                else
                {
                    filetmp=filetmp->next;
                }
            }
            if(cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 1)
                {
                    if(wait_handle_socket(index))
                    {
                        buffer_free(serverpath);
                        return HAVE_LOCAL_SOCKET;
                    }

                    ret=upload_file(localfiletmp->path,serverpath,1,index);
                    if(ret == 0 || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
                    {
                        if(ret == 0)
                        {
                            cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                            time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                            cJSON_Delete(json);
                            ChangeFile_modtime(localfiletmp->path,mtime);
                            free(serverpath);
                        }
                    }
                    else
                    {
                        free(serverpath);
                        return ret;
                    }
                }
                else
                {
                    buffer_free(serverpath);
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->download_only_socket_head);
                }
            }
            else
            {
                if((ret = the_same_name_compare(localfiletmp,filetmp,index,1)) != 0)
                {
                    return ret;
                }
            }
            filetmp=perform_br->filelist->next;
            localfiletmp=localfiletmp->next;
        }

        filetmp=perform_br->filelist->next;
        localfiletmp=perform_lo->filelist->next;
        while(filetmp != NULL && !exit_loop) //is based by filetmp
        {
            int cmp=1;
            char *localpath=serverpath_to_localpath(filetmp->href,index);
            while(localfiletmp != NULL)
            {
                if((cmp=strcmp(localpath,localfiletmp->path)) == 0)
                {
                    free(localpath);
                    break;
                }
                else
                {
                    localfiletmp=localfiletmp->next;
                }
            }
            if(cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 2)
                {
                    if(is_local_space_enough(filetmp,index))
                    {
                        if(wait_handle_socket(index))
                        {
                            buffer_free(localpath);
                            return HAVE_LOCAL_SOCKET;
                        }

                        add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                        ret=api_download(localpath,filetmp->href,index);
                        if(ret == 0)
                        {
                            ChangeFile_modtime(localpath,filetmp->mtime);
                            free(localpath);
                        }
                        else
                        {
                            free(localpath);
                            return ret;
                        }

                    }
                    else
                    {
                        buffer_free(localpath);
                        write_log(S_ERROR,"local space is not enough!","",index);
                        add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                }
            }
            localfiletmp=perform_lo->filelist->next;
            filetmp=filetmp->next;
        }
    }
    /*************handle folders**************/
    if(perform_br->foldernumber !=0 && perform_lo->foldernumber ==0)
    {
        if(asus_cfg.prule[index]->rule != 2)
        {
            while(foldertmp != NULL && !exit_loop)
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                char *localpath;
                localpath = serverpath_to_localpath(foldertmp->href,index);
                if(NULL == opendir(localpath))
                {
                    add_action_item("createfolder",localpath,g_pSyncList[index]->server_action_list);
                    int res;
                    res = api_metadata_test_dir(foldertmp->href,dofile);
                    if(res == 0)
                        mkdir(localpath,0777);
                    else if(res == -1)
                    {
                        free(localpath);
                        return res;
                    }
                    else
                    {
                        wd_DEBUG("this %s had been deleted\n",foldertmp->href);
                    }
                }
                free(localpath);
                foldertmp = foldertmp->next;
            }
        }
    }
    else if(perform_br->foldernumber ==0 && perform_lo->foldernumber !=0)
    {
        while(localfoldertmp != NULL && !exit_loop)
        {
            if(asus_cfg.prule[index]->rule != 1)
            {
                char *serverpath;
                serverpath = localpath_to_serverpath(localfoldertmp->path,index);
                ret=api_create_folder(localfoldertmp->path,serverpath);
                if(ret == 0)
                {
                    upload_serverlist(treenode,perform_br,localfoldertmp,index);
                    free(serverpath);
                }
                else
                {
                    free(serverpath);
                    return ret;
                }

            }
            else
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->download_only_socket_head);
            }
            localfoldertmp = localfoldertmp->next;
        }
    }
    else if(perform_br->foldernumber !=0 && perform_lo->foldernumber !=0)
    {
        while(foldertmp != NULL && !exit_loop)
        {
            int cmp=1;
            char *localpath;
            localpath = serverpath_to_localpath(foldertmp->href,index);
            while(localfoldertmp != NULL)
            {
                if((cmp = strcmp(localpath,localfoldertmp->path)) == 0)
                {
                    free(localpath);
                    break;
                }
                localfoldertmp=localfoldertmp->next;
            }
            if(cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 2)
                {
                    if(wait_handle_socket(index))
                    {
                        buffer_free(localpath);
                        return HAVE_LOCAL_SOCKET;
                    }
                    if(NULL == opendir(localpath))
                    {
                        add_action_item("createfolder",localpath,g_pSyncList[index]->server_action_list);
                        int res;
                        res = api_metadata_test_dir(foldertmp->href,dofile);
                        if(res == 0)
                            mkdir(localpath,0777);
                        else if(res == -1)
                        {
                            buffer_free(localpath);
                            return res;
                        }
                        else
                        {
                            wd_DEBUG("this %s had been deleted\n",foldertmp->href);
                        }
                    }
                    free(localpath);
                }
                else
                    buffer_free(localpath);
            }
            foldertmp = foldertmp->next;
            localfoldertmp = perform_lo->folderlist->next;
        }


        foldertmp = perform_br->folderlist->next;
        localfoldertmp = perform_lo->folderlist->next;
        while(localfoldertmp != NULL && !exit_loop)
        {
            int cmp=1;
            char *serverpath;
            serverpath = localpath_to_serverpath(localfoldertmp->path,index);
            while(foldertmp != NULL)
            {
                if((cmp = strcmp(serverpath,foldertmp->href)) == 0)
                {
                    free(serverpath);
                    break;
                }
                foldertmp=foldertmp->next;
            }
            if(cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 1)
                {
                    if(wait_handle_socket(index))
                    {
                        buffer_free(serverpath);
                        return HAVE_LOCAL_SOCKET;
                    }
                    ret=api_create_folder(localfoldertmp->path,serverpath);
                    if(ret == 0)
                    {
                        upload_serverlist(treenode,perform_br,localfoldertmp,index);
                        free(serverpath);
                    }
                    else
                    {
                        free(serverpath);
                        return ret;
                    }
                }
                else
                {
                    buffer_free(serverpath);
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->download_only_socket_head);
                }
            }
            foldertmp = perform_br->folderlist->next;
            localfoldertmp = localfoldertmp->next;
        }
    }
    return ret;
}

int sync_local_with_server_perform(Server_TreeNode *treenode,Browse *perform_br,Local *perform_lo,int index)
{
    wd_DEBUG("parentf = %s\n",treenode->parenthref);
    wd_DEBUG("perform_br->foldernumber = %d,perform_br->filenumber = %d,\n",perform_br->foldernumber,perform_br->filenumber);
    wd_DEBUG("perform_lo->foldernumber = %d,perform_lo->filenumber = %d,\n",perform_lo->foldernumber,perform_lo->filenumber);
    if(perform_br == NULL || perform_lo == NULL)
    {
        return 0;
    }
    int ret = 0;

    CloudFile *foldertmp=NULL;
    CloudFile *filetmp=NULL;
    LocalFolder *localfoldertmp;
    LocalFile *localfiletmp;
    if(perform_br->foldernumber > 0)
        foldertmp = perform_br->folderlist->next;
    if(perform_br->filenumber > 0)
        filetmp = perform_br->filelist->next;
    localfoldertmp=perform_lo->folderlist->next;
    localfiletmp=perform_lo->filelist->next;

    /****************handle files****************/
    if(perform_br->filenumber == 0 && perform_lo->filenumber !=0)
    {
        while(localfiletmp != NULL && exit_loop == 0)
        {
            if(asus_cfg.prule[index]->rule == 1)
            {
                action_item *item;
                item = get_action_item("download_only",localfiletmp->path,
                                       g_pSyncList[index]->download_only_socket_head,index);
                if(item != NULL)
                {
                    localfiletmp = localfiletmp->next;
                    continue;
                }
            }
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            add_action_item("remove",localfiletmp->path,g_pSyncList[index]->server_action_list);

            //[bug][fix]:local file size > cloud space,then the local file will be unlink
            action_item *pp;
            pp = get_action_item("upload",localfiletmp->path,
                                 g_pSyncList[index]->up_space_not_enough_list,index);
            if(pp == NULL)
            {
                unlink(localfiletmp->path);
            }

            localfiletmp = localfiletmp->next;
        }
    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber ==0)
    {
        while(filetmp != NULL && !exit_loop)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            if(is_local_space_enough(filetmp,index))
            {
                action_item *item;
                item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);

                char *localpath= serverpath_to_localpath(filetmp->href,index);

                ret=api_download(localpath,filetmp->href,index);
                if(ret == 0)
                {
                    ChangeFile_modtime(localpath,filetmp->mtime);
                    if(item != NULL)
                    {
                        del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                    free(localpath);
                }
                else
                {
                    free(localpath);
                    return ret;
                }

            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
            }
            filetmp=filetmp->next;
        }
    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber !=0)
    {

        while(localfiletmp != NULL && !exit_loop) //is based by localfiletmp
        {
            int cmp=1;
            char *serverpath=localpath_to_serverpath(localfiletmp->path,index);
            while(filetmp != NULL)
            {
                if((cmp=strcmp(serverpath,filetmp->href)) == 0)
                {
                    free(serverpath);
                    break;
                }
                else
                {
                    filetmp=filetmp->next;
                }
            }
            if(cmp != 0)
            {
                buffer_free(serverpath);
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                if(asus_cfg.prule[index]->rule == 1)
                {
                    action_item *item;
                    item = get_action_item("download_only",localfiletmp->path,
                                           g_pSyncList[index]->download_only_socket_head,index);
                    if(item != NULL)
                    {
                        filetmp = perform_br->filelist->next;
                        localfiletmp = localfiletmp->next;
                        continue;
                    }
                }

                add_action_item("remove",localfiletmp->path,g_pSyncList[index]->server_action_list);

                //[bug][fix]:local file size > cloud space,then the local file will be unlink
                action_item *pp;
                pp = get_action_item("upload",localfiletmp->path,
                                     g_pSyncList[index]->up_space_not_enough_list,index);
                if(pp == NULL)
                {
                    unlink(localfiletmp->path);
                }

            }
            else
            {
                if((ret = the_same_name_compare(localfiletmp,filetmp,index,0)) != 0)
                {
                    return ret;
                }
            }
            filetmp=perform_br->filelist->next;
            localfiletmp=localfiletmp->next;
        }

        filetmp=perform_br->filelist->next;
        localfiletmp=perform_lo->filelist->next;
        while(filetmp != NULL && !exit_loop) //is based by filetmp
        {
            int cmp=1;
            char *localpath=serverpath_to_localpath(filetmp->href,index);
            while(localfiletmp != NULL)
            {
                if((cmp=strcmp(localpath,localfiletmp->path)) == 0)
                {
                    free(localpath);
                    break;
                }
                else
                {
                    localfiletmp=localfiletmp->next;
                }
            }
            if(cmp != 0)
            {
                if(wait_handle_socket(index))
                {
                    buffer_free(localpath);
                    return HAVE_LOCAL_SOCKET;
                }
                if(is_local_space_enough(filetmp,index))
                {
                    action_item *item;
                    item = get_action_item("download",
                                           filetmp->href,g_pSyncList[index]->unfinished_list,index);

                    add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                    ret=api_download(localpath,filetmp->href,index);
                    if(ret == 0)
                    {
                        ChangeFile_modtime(localpath,filetmp->mtime);
                        if(item != NULL)
                        {
                            del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                        }
                        free(localpath);
                    }
                    else
                    {
                        free(localpath);
                        return ret;
                    }

                }
                else
                {
                    buffer_free(localpath);
                    write_log(S_ERROR,"local space is not enough!","",index);
                    add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                }

            }
            localfiletmp=perform_lo->filelist->next;
            filetmp=filetmp->next;
        }
    }
    /*************handle folders**************/
    if(perform_br->foldernumber !=0 && perform_lo->foldernumber ==0)
    {

        while(foldertmp != NULL && !exit_loop)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            char *localpath;
            localpath = serverpath_to_localpath(foldertmp->href,index);
            if(NULL == opendir(localpath))
            {
                int res;
                add_action_item("createfolder",localpath,g_pSyncList[index]->server_action_list);
                res = api_metadata_test_dir(foldertmp->href,dofile);
                if(res == 0)
                    mkdir(localpath,0777);
                else if(res == -1)
                {
                    free(localpath);
                    return res;
                }
                else
                {
                    wd_DEBUG("this %s had been deleted\n",foldertmp->href);
                }
            }
            free(localpath);
            foldertmp = foldertmp->next;
        }

    }
    else if(perform_br->foldernumber ==0 && perform_lo->foldernumber !=0)
    {
        while(localfoldertmp != NULL && !exit_loop)
        {
            if(asus_cfg.prule[index]->rule == 1)
            {
                action_item *item;
                item = get_action_item("download_only",
                                       localfoldertmp->path,g_pSyncList[index]->download_only_socket_head,index);
                if(item != NULL)
                {
                    localfoldertmp = localfoldertmp->next;
                    continue;
                }
            }
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            del_all_items(localfoldertmp->path,index);
            localfoldertmp = localfoldertmp->next;
        }
    }
    else if(perform_br->foldernumber !=0 && perform_lo->foldernumber !=0)
    {
        while(foldertmp != NULL && !exit_loop)
        {
            int cmp=1;
            char *localpath;
            localpath = serverpath_to_localpath(foldertmp->href,index);
            while(localfoldertmp != NULL)
            {
                if((cmp = strcmp(localpath,localfoldertmp->path)) == 0)
                {
                    free(localpath);
                    break;
                }
                localfoldertmp=localfoldertmp->next;
            }
            if(cmp != 0)
            {
                if(wait_handle_socket(index))
                {
                    buffer_free(localpath);
                    return HAVE_LOCAL_SOCKET;
                }
                if(NULL == opendir(localpath))
                {
                    add_action_item("createfolder",localpath,g_pSyncList[index]->server_action_list);
                    int res;
                    res = api_metadata_test_dir(foldertmp->href,dofile);
                    if(res == 0)
                        mkdir(localpath,0777);
                    else if(res == -1)
                    {
                        free(localpath);
                        return res;
                    }
                    else
                    {
                        wd_DEBUG("this %s had been deleted\n",foldertmp->href);
                    }
                }
                free(localpath);
            }
            foldertmp = foldertmp->next;
            localfoldertmp = perform_lo->folderlist->next;
        }


        foldertmp = perform_br->folderlist->next;
        localfoldertmp = perform_lo->folderlist->next;
        while(localfoldertmp != NULL && !exit_loop)
        {
            int cmp=1;
            char *serverpath;
            serverpath = localpath_to_serverpath(localfoldertmp->path,index);
            while(foldertmp != NULL)
            {
                if((cmp = strcmp(serverpath,foldertmp->href)) == 0)
                {
                    free(serverpath);
                    break;
                }
                foldertmp=foldertmp->next;
            }
            if(cmp != 0)
            {
                buffer_free(serverpath);
                if(asus_cfg.prule[index]->rule == 1)
                {
                    action_item *item;
                    item = get_action_item("download_only",
                                           localfoldertmp->path,g_pSyncList[index]->download_only_socket_head,index);
                    if(item != NULL)
                    {
                        foldertmp = perform_br->folderlist->next;
                        localfoldertmp = localfoldertmp->next;
                        continue;
                    }
                }
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                del_all_items(localfoldertmp->path,index);
            }
            foldertmp = perform_br->folderlist->next;
            localfoldertmp = localfoldertmp->next;
        }
    }
    return ret;
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
        wd_DEBUG("open %s fail \n",dir);
}

time_t api_getmtime(char *phref,cJSON *(*cmd_data)(char *filename))
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    snprintf(myUrl, 128, "%s","https://api.dropboxapi.com/2/files/get_metadata");
    char *header;
    FILE *hd;
    hd=fopen(Con(TMP_R,api_getmime.txt),"w+");
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        char *output = NULL;
        output=(char *)malloc(strlen(phref) + 128);
        memset(output,0,strlen(phref) + 128);
        if(strcmp(phref, "/") == 0)
            snprintf(output, strlen(phref) + 128, "{\"path\": \"%s\"}", "");
        else
            snprintf(output, strlen(phref) + 128, "{\"path\": \"%s\"}", phref);

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,mtime.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,output);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        free(output);
        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);
            free(header);
            free(myUrl);
            wd_DEBUG("get %s mtime failed!id: [%d] \n",phref,res);
            return -2;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            fclose(hd);
            if(strstr(tmp,"409")!=NULL)
            {
                return -1;
            }
        }
    }
    free(myUrl);
    free(header);
    cJSON *json;
    time_t mtime;
    json=cmd_data(Con(TMP_R,mtime.txt));
    mtime=cJSON_printf_mtime(json);
    cJSON_Delete(json);
    if(mtime!=-1)
        return mtime;
    else
        return -1;
}
/*0,local file newer
 *1,server file newer
 *2,local time == server time
 *-1,get server modtime failed
**/
int newer_file(char *localpath,int index,int is_init,int rule){

    char *serverpath;
    serverpath = localpath_to_serverpath(localpath,index);
    time_t old_mtime;
    if(is_init == 0 && rule == 1)
    {
        CloudFile *filetmp;
        filetmp=get_CloudFile_node(g_pSyncList[index]->VeryOldServerRootNode,serverpath,0x2);
        if(filetmp == NULL)
        {
            old_mtime=(time_t)-1;
        }
        else
        {
            old_mtime=filetmp->mtime;
        }
    }

    time_t modtime1,modtime2;
    modtime1 = api_getmtime(serverpath,dofile);
    free(serverpath);
    if(modtime1 == -1 || modtime1 == -2)
    {
        wd_DEBUG("newer_file Getmodtime failed!\n");
        return -1;
    }

    struct stat buf;

    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
        return -1;
    }
    modtime2 = buf.st_mtime;
    wd_DEBUG("local>modtime2 = %lu,server>modtime1 = %lu\n",modtime2,modtime1);

    if(is_init)
    {
        if(modtime2 > modtime1)
            return 0;
        else if(modtime2 == modtime1 || modtime2 +1 == modtime1) //FAT32 and umount HD than plugin HD ,it will be local time less then server time 1 sec;
        {
            if(modtime2 +1 == modtime1)
                ChangeFile_modtime(localpath,modtime1);
            return 2;
        }
        else
            return 1;
    }
    else
    {
        if(rule == 1) //download only
        {
            if(modtime2 > modtime1)
                return 0;
            else if(modtime2 == modtime1)
                return 2;
            else
            {
                if(modtime2 == old_mtime)
                    return 1;
                else
                    return 0;
            }
        }
        else
        {
            if(modtime2 > modtime1)
                return 0;
            else if(modtime2 == modtime1)
                return 2;
            else
                return 1;
        }
    }


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
    wd_DEBUG("filename len is %d\n",len);
    path = my_str_malloc((size_t)(strlen(fullname)-len+1));
    wd_DEBUG("fullname = %s\n",fullname);
    snprintf(path,strlen(fullname)-len+1,"%s",fullname);
    wd_DEBUG("path = %s\n",path);
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
        wd_DEBUG("newfilename = %s\n",newfilename);
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
/*
 1=>exit;
 0=>not exit;
 path=>serverpath;
 temp_name=>fullname;
 */
int is_server_exist(char *path,char *temp_name,int index)
{
    int status;
    FileList_one = (CloudFile *)malloc(sizeof(CloudFile));
    memset(FileList_one,0,sizeof(CloudFile));

    FileList_one->href = NULL;
    FileList_one->name = NULL;

    FileTail_one = FileList_one;
    FileTail_one->next = NULL;
    wd_DEBUG("is_server_exist path:%s\n",path);
    int has_more_one=0;
    char *cursor = NULL;
    status=api_metadata_one(path,dofile, &has_more_one, &cursor);
    if(status == 0)
    {
        while(has_more_one && cursor)
        {
            has_more_one=0;
            api_list_continue_one(dofile, &has_more_one, &cursor);
        }
    }
    if(cursor)
    {
        free(cursor);
        cursor=NULL;
    }
    CloudFile *de_filecurrent;
    de_filecurrent=FileList_one->next;
    while(de_filecurrent != NULL)
    {
        if(de_filecurrent->href != NULL)
        {
            wd_DEBUG("de_filecurrent->href:%s\n",de_filecurrent->href);
            wd_DEBUG("temp_name           :%s\n",temp_name);
            if((status=strcmp(de_filecurrent->href,temp_name))==0)
            {
                free_CloudFile_item(FileList_one);
                return 1;
            }
        }
        de_filecurrent = de_filecurrent->next;
    }
    free_CloudFile_item(FileList_one);
    return 0;

}

char *get_server_exist(char *temp_name,int index)
{
    char *path;
    char *p = strrchr(temp_name,'/');
    path = my_str_malloc(strlen(temp_name)-strlen(p)+1);
    snprintf(path,strlen(temp_name)-strlen(p)+1,"%s",temp_name);

    int status;
    FileList_one = (CloudFile *)malloc(sizeof(CloudFile));
    memset(FileList_one,0,sizeof(CloudFile));

    FileList_one->href = NULL;
    FileList_one->name = NULL;

    FileTail_one = FileList_one;
    FileTail_one->next = NULL;
    wd_DEBUG("get_server_exist path:%s\n",path);
    int has_more_one=0;
    char *cursor = NULL;
    status=api_metadata_one(path, dofile, &has_more_one, &cursor);
    if(status == -1)
    {
        free_CloudFile_item(FileList_one);
        free(path);
        if(cursor)
        {
            free(cursor);
            cursor=NULL;
        }
        return NULL;
    }
    else if(status == 0)
    {
        while(has_more_one && cursor)
        {
            has_more_one=0;
            api_list_continue_one(dofile, &has_more_one, &cursor);
        }
    }
    if(cursor)
    {
        free(cursor);
        cursor=NULL;
    }
    char *temp_name_g = my_str_malloc(strlen(temp_name)+1);
    snprintf(temp_name_g, sizeof(char)*(strlen(temp_name)+1), "%s",temp_name);

    CloudFile *de_filecurrent;
    de_filecurrent=FileList_one->next;
    while(de_filecurrent != NULL)
    {
        if(de_filecurrent->href != NULL)
        {
            wd_DEBUG("de_filecurrent->href:%s\n",de_filecurrent->href);
            wd_DEBUG("temp_name           :%s\n",temp_name);
            char *tmp_href = my_str_malloc(strlen(de_filecurrent->href)+1);
            snprintf(tmp_href, sizeof(char)*(strlen(de_filecurrent->href)+1), "%s", de_filecurrent->href);
            if(strcmp(de_filecurrent->href,temp_name) != 0 && (status=strcmp(strlwr(tmp_href),strlwr(temp_name_g)))==0)
            {
                char *conflict_name = my_str_malloc(strlen(de_filecurrent->href)+1);
                snprintf(conflict_name, sizeof(char)*(strlen(de_filecurrent->href)+1), "%s", de_filecurrent->href);
                free_CloudFile_item(FileList_one);
                free(path);
                free(temp_name_g);
                free(tmp_href);
                return conflict_name;
            }
            else
                free(tmp_href);
        }
        de_filecurrent = de_filecurrent->next;
    }
}

#if 0
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
    wd_DEBUG("filename len is %d\n",len);
    path = my_str_malloc((size_t)(strlen(fullname)-len+1));
    wd_DEBUG("fullname = %s\n",fullname);
    snprintf(path,strlen(fullname)-len+1,"%s",fullname);
    wd_DEBUG("path = %s\n",path);

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
        wd_DEBUG("newfilename = %s\n",newfilename);
        i++;
        temp_name = my_str_malloc((size_t)(strlen(path)+strlen(newfilename)+1));
        snprintf(temp_name, strlen(path)+strlen(newfilename)+1, "%s%s",path,newfilename);
        wd_DEBUG("temp_name = %s\n",temp_name);
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
#endif

char *change_server_same_name(char *fullname,int index)
{
    char *newname;
    char *tmp_name = malloc(strlen(fullname)+1);
    memset(tmp_name,0,strlen(fullname)+1);
    snprintf(tmp_name, strlen(fullname)+1, "%s",fullname);
    int is_folder = test_if_dir(fullname);
    int exist;
    int len;
    char *filename;
    char *path;

    filename = parse_name_from_path(fullname);
    len = strlen(filename);

    path = my_str_malloc((size_t)(strlen(fullname)-len+1));

    wd_DEBUG("fullname = %s\n",fullname);

    snprintf(path,strlen(fullname)-len+1,"%s",fullname);

    while(!exit_loop)
    {
        newname = get_confilicted_name(tmp_name,is_folder);
        exist = is_server_exist(path,newname,index);
        if(exist == 1)
        {
            my_free(tmp_name);
            tmp_name = malloc(strlen(newname)+1);
            memset(tmp_name,0,strlen(newname)+1);
            snprintf(tmp_name, strlen(newname)+1, "%s",newname);
            my_free(newname);
        }
        else
            break;
    }
    my_free(path);
    my_free(filename);
    my_free(tmp_name);
    return newname;
}

int the_same_name_compare(LocalFile *localfiletmp,CloudFile *filetmp,int index,int is_init)
{

    wd_DEBUG("###########the same name compare################\n");

    int ret=0;
    int newer_file_ret=0;
    if(asus_cfg.prule[index]->rule == 1)  //download only
    {
        newer_file_ret = newer_file(localfiletmp->path,index,is_init,1);
        if(newer_file_ret !=2 && newer_file_ret !=-1)
        {
            if(newer_file_ret == 0) //local file is change
            {
                char *newname;
                char *tmp_name = malloc(strlen(localfiletmp->path)+1);
                memset(tmp_name,0,strlen(localfiletmp->path)+1);
                snprintf(tmp_name, strlen(localfiletmp->path)+1, "%s",localfiletmp->path);

                while(!exit_loop)
                {
                    newname = get_confilicted_name(tmp_name,0);
                    if(access(newname,F_OK) == 0)
                    {
                        my_free(tmp_name);
                        tmp_name = malloc(strlen(newname)+1);
                        memset(tmp_name,0,strlen(newname)+1);
                        snprintf(tmp_name, strlen(newname)+1, "%s",newname);
                        my_free(newname);
                    }
                    else
                        break;
                }
                my_free(tmp_name);

                rename(localfiletmp->path,newname);
               char *err_msg = write_error_message("%s is download from server,%s is local file and rename from %s",localfiletmp->path,newname,localfiletmp->path);
                write_trans_excep_log(localfiletmp->path,3,err_msg);
                free(err_msg);
                free(newname);
            }

            action_item *item;
            item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);

            if(is_local_space_enough(filetmp,index))
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
                ret=api_download(localfiletmp->path,filetmp->href,index);
                if(ret == 0)
                {
                    time_t mtime=api_getmtime(filetmp->href,dofile);
                    if(mtime != -1 || mtime != -2)
                        ChangeFile_modtime(localfiletmp->path,mtime);
                    else
                    {

                        wd_DEBUG("ChangeFile_modtime failed!\n");

                    }
                    if(item != NULL)
                    {
                        del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                }
                else
                {
                    return ret;
                }
            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                if(item == NULL)
                {
                    add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                }
            }
        }
    }
    else if(asus_cfg.prule[index]->rule == 2)//upload only
    {
        newer_file_ret = newer_file(localfiletmp->path,index,is_init,0);
        if(newer_file_ret !=2 && newer_file_ret !=-1)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            ret=upload_file(localfiletmp->path,filetmp->href,1,index);
            if(ret == 0 || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
            {
                if(ret == 0)
                {
                    cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                    cJSON_Delete(json);
                    ChangeFile_modtime(localfiletmp->path,mtime);
                }
            }
            else
                return ret;
        }
    }
    else //sync
    {
        newer_file_ret = newer_file(localfiletmp->path,index,is_init,0);

        if(newer_file_ret == 1)
        {
            if(is_init)
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                char *serverpath=localpath_to_serverpath(localfiletmp->path,index);
                /*third param flase,false=>if server file has exit ,will not be overwrite*/
                ret=upload_file(localfiletmp->path,serverpath,0,index);
                free(serverpath);
                if(ret == 0 || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
                {
                    if(ret == 0)
                    {
                        char *newlocalname = NULL;
                        cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                        time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                        cJSON_Delete(json);

                        newlocalname=change_local_same_file(localfiletmp->path,index);
                        if(newlocalname)
                            ChangeFile_modtime(newlocalname,mtime);
                        free(newlocalname);
                    }

                }
                else
                {
                    return ret;
                }
            }
            if(is_local_space_enough(filetmp,index))
            {
                action_item *item;
                item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
                ret=api_download(localfiletmp->path,filetmp->href,index);
                if(ret == 0)
                {
                    time_t mtime=api_getmtime(filetmp->href,dofile);
                    if(mtime != -1 || mtime != -2)
                        ChangeFile_modtime(localfiletmp->path,mtime);
                    else
                    {

                        wd_DEBUG("ChangeFile_modtime failed!\n");

                    }
                    if(item != NULL)
                    {
                        del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                }
                else
                {
                    return ret;
                }

            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
            }
        }
        else if(newer_file_ret == 0)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            ret=upload_file(localfiletmp->path,filetmp->href,1,index);
            if(ret == 0 || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
            {
                if(ret == 0)
                {
                    cJSON * json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                    cJSON_Delete(json);

                    ChangeFile_modtime(localfiletmp->path,mtime);
                }
            }
            else
                return ret;
        }
    }
    return 0;
}
int send_action(int type, char *content)
{
    if(exit_loop)
    {
        return 0;
    }
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    char str[1024];
    int port;

    port = INOTIFY_PORT;
    struct sockaddr_in their_addr; /* connector's address information */

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    bzero(&(their_addr), sizeof(their_addr)); /* zero the rest of the struct */
    their_addr.sin_family = AF_INET; /* host byte order */
    their_addr.sin_port = htons(port); /* short, network byte order */
    their_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(their_addr.sin_zero), sizeof(their_addr.sin_zero)); /* zero the rest of the struct */
    if (connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct
                                                              sockaddr)) == -1) {
        perror("connect");
        return -1;
    }

    snprintf(str, 1024, "%d@%s",type,content);
    if (send(sockfd, str, strlen(str), 0) == -1) {
        perror("send");
        return -1;
    }

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv");
        return -1;
    }

    buf[numbytes] = '\0';
    close(sockfd);

    wd_DEBUG("send_action finished!\n");

    return 0;
}
void send_to_inotify(){

    int i;

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        wd_DEBUG("send_action base_path = %s\n",asus_cfg.prule[i]->path);
#if TOKENFILE
        if(g_pSyncList[i]->sync_disk_exist)
        {
            send_action(asus_cfg.type,asus_cfg.prule[i]->path);
            usleep(1000*10);
        }
#else
        send_action(asus_cfg.type,asus_cfg.prule[i]->path);
        usleep(1000*10);
#endif
    }
}
char *get_path_from_socket(char *cmd,int index)
{
    if(!strncmp(cmd,"rmroot",6))
    {
        return NULL;
    }
    char cmd_name[64]="\0";
    char *path;
    char *temp;
    char filename[256]="\0";
    char *fullname;
    char oldname[256]="\0",newname[256]="\0";
    char *oldpath;
    char action[64]="\0";
    char *cmp_name;
    char *mv_newpath;
    char *mv_oldpath;
    char *ch;
    int status;

    ch = cmd;
    int i = 0;
    while(*ch != '\n')
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);
    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));
    snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
    p = strchr(temp,'\n');
    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strcmp(cmd_name, "rename") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');
        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        snprintf(newname, 256, "%s", p1);
        wd_DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);
        if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return NULL;
        }
    }
    else if(strcmp(cmd_name, "move") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');
        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));
        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        snprintf(oldname, 256, "%s", p1);

        wd_DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);
        if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            free(oldpath);
            return NULL;
        }
    }
    else
    {
        snprintf(filename, 256, "%s", p);
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));

        wd_DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);
        if(filename[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return NULL;
        }
    }

    free(temp);

    if( !strcmp(cmd_name,"rename") )
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        snprintf(cmp_name, sizeof(char)*(strlen(path)+strlen(newname)+2), "%s/%s",path,newname);
    }
    else
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
        snprintf(cmp_name, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
    }

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        snprintf(action, 64, "%s", "createfile");
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        snprintf(action, 64, "%s", "remove");
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        snprintf(action, 64, "%s", "createfolder");
    }
    else if( strcmp(cmd_name, "rename") == 0 )
    {
        snprintf(action, 64, "%s", "rename");
    }
#if 1
    if(g_pSyncList[index]->server_action_list->next != NULL)
    {
        action_item *item;
        item = get_action_item(action,cmp_name,g_pSyncList[index]->server_action_list,index);

        if(item != NULL)
        {

            wd_DEBUG("##### %s %s by Dropbox Server self ######\n",action,cmp_name);
            del_action_item(action,cmp_name,g_pSyncList[index]->server_action_list);
            wd_DEBUG("#### del action item success!\n");

            free(path);
            if( strcmp(cmd_name, "rename") != 0 )
                free(fullname);
            free(cmp_name);
            return NULL;
        }
    }
#endif
    free(cmp_name);
    return path;

}
void show(queue_entry_t pTemp)
{
    wd_DEBUG(">>>>>>show socketlist>>>>>>>>>>>>>\n");
    while(pTemp!=NULL)
    {
        wd_DEBUG(">>>>>>>>>>>>>>>>>>>>>>>>>\n");
        wd_DEBUG(">>>>%s\n",pTemp->cmd_name);
        wd_DEBUG(">>>>%s\n",pTemp->re_cmd);
        wd_DEBUG("\n");
        pTemp=pTemp->next_ptr;
    }
}

char *search_newpath(char *href,int index)
{
    wd_DEBUG("################search_newpath###########\n");
    wd_DEBUG("href = %s\n",href);
    int flag_r = 0;
    queue_entry_t pTemp = g_pSyncList[index]->SocketActionList->head;
    while(pTemp != NULL)
    {
        if(strncmp(pTemp->cmd_name,"rename0",strlen("rename0")) == 0 ||
           strncmp(pTemp->cmd_name,"move0",strlen("move0")) == 0)
        {
            char cmd_name[64]="\0";
            char *path;
            char *temp;
            char oldname[256]="\0",newname[256]="\0";
            char *oldpath;
            char *mv_newpath;
            char *mv_oldpath;
            char *ch;

            ch = pTemp->cmd_name;
            int i = 0;
            while(*ch != '\n')
            {
                i++;
                ch++;
            }

            memcpy(cmd_name, pTemp->cmd_name, i);

            char *p = NULL;
            ch++;
            i++;

            temp = my_str_malloc((size_t)(strlen(ch)+1));
            snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
            p = strchr(temp,'\n');
            path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

            if(p!=NULL)
                snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

            p++;
            if(strcmp(cmd_name, "rename0") == 0)
            {
                char *p1 = NULL;

                p1 = strchr(p,'\n');
                if(p1 != NULL)
                    strncpy(oldname,p,strlen(p)- strlen(p1));

                p1++;
                snprintf(newname, 256, "%s", p1);

                if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
                {
                    free(temp);
                    free(path);
                    pTemp=pTemp->next_ptr;
                    continue;
                }
            }
            else if(strcmp(cmd_name, "move0") == 0)
            {
                char *p1 = NULL;

                p1 = strchr(p,'\n');
                oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

                if(p1 != NULL)
                    snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

                p1++;
                snprintf(oldname, 256, "%s", p1);

                if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
                {
                    free(temp);
                    free(path);
                    pTemp=pTemp->next_ptr;
                    continue;
                }
            }
            wd_DEBUG("111\n");
            free(temp);
            wd_DEBUG("222\n");
            if(strcmp(cmd_name, "move0") == 0)
            {
                mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
                mv_oldpath = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
                snprintf(mv_newpath, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
                snprintf(mv_oldpath, sizeof(char)*(strlen(oldpath)+strlen(oldname)+2), "%s/%s",oldpath,oldname);
                free(oldpath);
            }
            else
            {
                mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
                mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
                snprintf(mv_newpath, sizeof(char)*(strlen(path)+strlen(newname)+2), "%s/%s",path,newname);
                snprintf(mv_oldpath, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
            }
            wd_DEBUG("333\n");
            char *pTemp_t=(char *)malloc(sizeof(char)*(strlen(href)+1+1));
            char *oldpath_t=(char *)malloc(sizeof(char)*(strlen(mv_oldpath)+1+1));
            memset(pTemp_t,'\0',strlen(href)+1+1);
            memset(oldpath_t,'\0',strlen(mv_oldpath)+1+1);
            snprintf(pTemp_t, sizeof(char)*(strlen(href)+1+1), "%s/",href);
            snprintf(oldpath_t, sizeof(char)*(strlen(mv_oldpath)+1+1), "%s/",mv_oldpath);

            wd_DEBUG("pTemp_t=%s ,oldpath_t=%s\n",pTemp_t ,oldpath_t);
            char *p_t = NULL;
            char *ret_p = NULL;
            if((p_t=strstr(pTemp_t,oldpath_t)) != NULL)
            {
                wd_DEBUG("666\n");
                p_t=p_t+strlen(oldpath_t);
                wd_DEBUG("p_t=%s\n",p_t);
                ret_p = (char *)malloc(sizeof(char) * (strlen(mv_newpath)+strlen(p_t) + 1 +1));
                memset(ret_p,'\0',strlen(mv_newpath)+strlen(p_t) + 1 +1);
                snprintf(ret_p, sizeof(char) * (strlen(mv_newpath)+strlen(p_t) + 1 +1), "%s/%s",mv_newpath,p_t);
                wd_DEBUG("ret_p = %s\n",ret_p);
                flag_r = 1;
            }
            wd_DEBUG("444\n");
            free(pTemp_t);
            free(oldpath_t);

            free(path);
            free(mv_oldpath);
            free(mv_newpath);
            wd_DEBUG("555\n");
            if(1 == flag_r)
            {
                wd_DEBUG("ret_p = %s\n",ret_p);
                return ret_p;
            }
        }
        pTemp=pTemp->next_ptr;
    }

    if(flag_r == 0)
    {
        return NULL;
    }
}

void set_copyfile_list(char *buf,char *oldpath,char *newpath)
{
    wd_DEBUG("************************set_copyfile_list***********************\n");
    int i;
    char *r_path;
    r_path = get_socket_base_path(buf);
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(!strcmp(r_path,asus_cfg.prule[i]->path))
            break;
    }
    free(r_path);
    check_action_item("copyfile",oldpath,g_pSyncList[i]->copy_file_list,i,newpath);
}

char *get_socket_filename(char *cmd)
{
    char cmd_name[64]="\0";
    char *path;
    char *temp;
    char *ch;
    char *filename;

    ch = cmd;
    int i = 0;
    while(*ch != '\n')
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));
    snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
    p = strchr(temp,'\n');

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    filename = (char *)malloc(sizeof(char)*(strlen(p)+2));
    memset(filename,'\0',strlen(p)+2);
    snprintf(filename, sizeof(char)*(strlen(p)+2), "%s",p);
    return filename;
}

int get_socket_fullname(queue_entry_t pTemp,char *fullname)
{
    char *cmd = pTemp ->cmd_name;
    char cmd_name[64]="\0";
    char *path;
    char *temp;
    char oldname[256]="\0",newname[256]="\0";
    char *oldpath;
    char *mv_newpath;
    char *mv_oldpath;
    char *ch;

    ch = cmd;
    int i = 0;

    while(*ch != '\n')
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));

    snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
    p = strchr(temp,'\n');
    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",6) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;
        snprintf(newname, 256, "%s", p1);

        if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    else if(strncmp(cmd_name, "move",4) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');
        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;
        snprintf(oldname, 256, "%s", p1);

        if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            free(oldpath);
            return 0;
        }
    }

    free(temp);

    if(strncmp(cmd_name, "move",4) == 0)
    {
        mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        mv_oldpath = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
        snprintf(mv_newpath, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
        snprintf(mv_oldpath, sizeof(char)*(strlen(oldpath)+strlen(oldname)+2), "%s/%s",oldpath,oldname);
        free(oldpath);
    }
    else
    {
        mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        snprintf(mv_newpath, sizeof(char)*(strlen(path)+strlen(newname)+2), "%s/%s",path,newname);
        snprintf(mv_oldpath, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
    }

    if(strcmp(fullname,mv_oldpath) == 0 ||strcmp(fullname,mv_newpath) == 0)
    {
        free(path);
        free(mv_oldpath);
        free(mv_newpath);
        return 1;
    }
    if(pTemp->re_cmd != NULL)
    {
        if(strncmp(cmd_name, "move",4) == 0)
        {
            char *p1 = my_str_malloc((size_t)(strlen(pTemp->re_cmd)+strlen(oldname)+2));
            snprintf(p1, sizeof(char)*(strlen(pTemp->re_cmd)+strlen(oldname)+2), "%s%s",pTemp->re_cmd,oldname);
            if(strcmp(fullname,p1) == 0)
            {
                free(path);
                free(mv_oldpath);
                free(mv_newpath);
                free(p1);
                return 1;
            }
            free(p1);
        }
        else
        {
            char *p1 = my_str_malloc((size_t)(strlen(pTemp->re_cmd)+strlen(oldname)+2));
            char *p2 = my_str_malloc((size_t)(strlen(pTemp->re_cmd)+strlen(newname)+2));
            snprintf(p1, sizeof(char)*(strlen(pTemp->re_cmd)+strlen(oldname)+2), "%s%s",pTemp->re_cmd,oldname);
            snprintf(p2, sizeof(char)*(strlen(pTemp->re_cmd)+strlen(newname)+2), "%s%s",pTemp->re_cmd,newname);
            if(strcmp(fullname,p1) == 0 || strcmp(fullname,p2) == 0 )
            {
                free(path);
                free(mv_oldpath);
                free(mv_newpath);
                free(p1);
                free(p2);
                return 1;
            }
            free(p1);
            free(p2);
        }
    }

    free(path);
    free(mv_oldpath);
    free(mv_newpath);
    return 0;
}

int check_localpath_is_socket(int index,char *ParentHerf,char *Fname,char *fullname)
{
    wd_DEBUG("************************check_localpath_is_socket***********************\n");
    int i = index;

    queue_entry_t pTemp = g_pSyncList[i]->SocketActionList->head;
    int reg = 0;// 0==>is not socket
    while(pTemp!=NULL)
    {
        if(strncmp(pTemp->cmd_name,"rename",6) == 0 ||strncmp(pTemp->cmd_name,"move",4) == 0)
        {
            reg=get_socket_fullname(pTemp,fullname);
            if(reg)
                return reg;
        }
        else
        {

            char *local_p1 = (char *)malloc(sizeof(char)*(strlen(ParentHerf)+1+strlen(Fname)+1+1));

            memset(local_p1,'\0',strlen(ParentHerf)+1+strlen(Fname)+1+1);

            snprintf(local_p1, sizeof(char)*(strlen(ParentHerf)+1+strlen(Fname)+1+1), "\n%s\n%s",ParentHerf,Fname);

            if(strstr(pTemp->cmd_name,local_p1) != NULL )
            {
                reg = 1;
                free(local_p1);
                return reg;
            }
            if(pTemp->re_cmd != NULL)
            {
                char *filename = get_socket_filename(pTemp->cmd_name);
                char *local_p2 = (char *)malloc(sizeof(char)*(strlen(pTemp->re_cmd)+strlen(filename)+2));
                memset(local_p2,'\0',strlen(pTemp->re_cmd)+strlen(filename)+2);
                snprintf(local_p2, sizeof(char)*(strlen(pTemp->re_cmd)+strlen(filename)+2), "%s%s",pTemp->re_cmd,filename);
                free(filename);

                if(strcmp(local_p2,fullname) == 0)
                {
                    reg = 1;
                    free(local_p1);
                    free(local_p2);
                    return reg;
                }
                free(local_p2);
            }
            free(local_p1);
        }

        pTemp=pTemp->next_ptr;
    }
    return reg;
}
int check_socket_parser(char *cmd,int index,char *re_cmd,char *str_path,char *so_filename,char *sn_filename)
{
    if( !strncmp(cmd,"exit",4))
    {
        wd_DEBUG("exit socket\n");
        return 0;
    }

    if(!strncmp(cmd,"rmroot",6))
    {
        g_pSyncList[index]->no_local_root = 1;
        return 0;
    }

    char cmd_name[64]="\0";
    char *path;
    char *temp;
    char filename[256]="\0";
    char oldname[256]="\0",newname[256]="\0";
    char *oldpath;
    char *ch;

    ch = cmd;
    int i = 0;
    while(*ch != '\n')
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));

    snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
    p = strchr(temp,'\n');
    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",strlen("rename")) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        snprintf(newname, 256, "%s", p1);

        wd_DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

        if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    else if(strncmp(cmd_name, "move",strlen("move")) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');

        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        snprintf(oldname, 256, "%s", p1);

        wd_DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);


        if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            free(oldpath);
            return 0;
        }
    }
    else
    {
        snprintf(filename, 256, "%s", p);

        wd_DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);

        if(filename[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    free(temp);

    if( strcmp(cmd_name, "modify") == 0 )
    {
        if(re_cmd)
        {
            if(strcmp(re_cmd,str_path) == 0 && strcmp(filename,so_filename)==0)
            {
                pthread_mutex_lock(&mutex_socket);
                memset(cmd,0,1024);
                sprintf(cmd,"%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,sn_filename);
                pthread_mutex_unlock(&mutex_socket);
            }
        }
        else
        {
            if(strcmp(path,str_path) == 0 && strcmp(filename,so_filename)==0)
            {
                pthread_mutex_lock(&mutex_socket);
                memset(cmd,0,1024);
                sprintf(cmd,"%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,sn_filename);
                pthread_mutex_unlock(&mutex_socket);
            }
        }
    }
    else if(strcmp(cmd_name, "delete") == 0  || strcmp(cmd_name, "remove") == 0)
    {
        if(strcmp(path,str_path) == 0 && strcmp(filename,so_filename)==0)
        {
            pthread_mutex_lock(&mutex_socket);
            memset(cmd,0,1024);
            sprintf(cmd,"%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,sn_filename);
            pthread_mutex_unlock(&mutex_socket);
        }
    }
    else if(strncmp(cmd_name, "move",strlen("move")) == 0 || strncmp(cmd_name, "rename",strlen("rename")) == 0)
    {
        if(strncmp(cmd_name, "rename",strlen("rename")) == 0)
        {
            if(re_cmd)
            {
                if(strcmp(re_cmd,str_path) == 0 && strcmp(oldname,so_filename)==0)
                {
                    pthread_mutex_lock(&mutex_socket);
                    memset(cmd,0,1024);
                    sprintf(cmd,"%s%s%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,sn_filename,CMD_SPLIT,newname);
                    pthread_mutex_unlock(&mutex_socket);
                }
            }
            else
            {
                if(strcmp(path,str_path) == 0 && strcmp(oldname,so_filename)==0)
                {
                    pthread_mutex_lock(&mutex_socket);
                    memset(cmd,0,1024);
                    sprintf(cmd,"%s%s%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,sn_filename,CMD_SPLIT,newname);
                    pthread_mutex_unlock(&mutex_socket);
                }
            }
        }
        else
        {
            if(re_cmd)
            {
                if(strcmp(re_cmd,str_path) == 0 && strcmp(oldname,so_filename)==0)
                {
                    pthread_mutex_lock(&mutex_socket);
                    memset(cmd,0,1024);
                    sprintf(cmd,"%s%s%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,oldpath,CMD_SPLIT,sn_filename);
                    pthread_mutex_unlock(&mutex_socket);
                }
            }
            else
            {
                if(strcmp(oldpath,str_path) == 0 && strcmp(oldname,so_filename)==0)
                {
                    pthread_mutex_lock(&mutex_socket);
                    memset(cmd,0,1024);
                    sprintf(cmd,"%s%s%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,oldpath,CMD_SPLIT,sn_filename);
                    pthread_mutex_unlock(&mutex_socket);
                }
            }
            my_free(oldpath);
        }
    }
    else if( strcmp(cmd_name,"dragfolder") == 0)
    {}
    else if(strcmp(cmd_name, "createfolder") == 0)
    {}
    free(path);
    return 0;

}
void updata_socket_list(char *temp_name,char *new_name,int i)
{
    char *path;
    char *old_filename;
    char *p = strrchr(temp_name,'/');
    path = my_str_malloc(strlen(temp_name)-strlen(p)+1);
    snprintf(path,strlen(temp_name)-strlen(p)+1,"%s",temp_name);
    p++;
    old_filename = my_str_malloc(strlen(p)+1);
    snprintf(old_filename, sizeof(char)*(strlen(p)+1), "%s",p);

    p = NULL;
    char *new_filename;
    p = strrchr(new_name,'/');
    p++;
    new_filename = my_str_malloc(strlen(p)+1);
    snprintf(new_filename, sizeof(char)*(strlen(p)+1), "%s",p);

    wd_DEBUG("*****************updata_socket_list***************\n");
    queue_entry_t pTemp = g_pSyncList[i]->SocketActionList->head->next_ptr;//head is current socket,updata from next begin
    while(pTemp!=NULL)
    {
        check_socket_parser(pTemp->cmd_name,i,pTemp->re_cmd,path,old_filename,new_filename);
        pTemp = pTemp->next_ptr;
    }
    my_free(path);
    my_free(old_filename);
    my_free(new_filename);

    show(g_pSyncList[i]->SocketActionList->head);
}

void set_re_cmd(char *buf,char *oldpath,char *newpath)
{
    wd_DEBUG("************************set_re_cmd***********************\n");
    int i;
    char *r_path;
    r_path = get_socket_base_path(buf);
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(!strcmp(r_path,asus_cfg.prule[i]->path))
            break;
    }
    free(r_path);

    queue_entry_t pTemp = g_pSyncList[i]->SocketActionList->head;

    while(pTemp!=NULL)
    {
            char *socket_path=get_path_from_socket(pTemp->cmd_name,i);
            wd_DEBUG("path:%s\n",socket_path);
            wd_DEBUG("path:%s\n",oldpath);
            char *pTemp_t=(char *)malloc(sizeof(char)*(strlen(socket_path)+1+1));
            char *oldpath_t=(char *)malloc(sizeof(char)*(strlen(oldpath)+1+1));
            memset(pTemp_t,'\0',strlen(socket_path)+1+1);
            memset(oldpath_t,'\0',strlen(oldpath)+1+1);
            snprintf(pTemp_t, sizeof(char)*(strlen(socket_path)+1+1), "%s/",socket_path);
            snprintf(oldpath_t, sizeof(char)*(strlen(oldpath)+1+1), "%s/",oldpath);
            char *p_t=NULL;
            if(socket_path != NULL)
            {
                if(pTemp->re_cmd == NULL)
                {
                    if((p_t=strstr(pTemp_t,oldpath_t)) != NULL)
                    {
                        if(strlen(oldpath)<strlen(socket_path))
                        {
                            p_t=p_t+strlen(oldpath);
                            pTemp->re_cmd = (char *)malloc(sizeof(char) * (strlen(newpath)+strlen(p_t) + 1));
                            memset(pTemp->re_cmd,'\0',strlen(newpath)+strlen(p_t) + 1);
                            snprintf(pTemp->re_cmd, sizeof(char) * (strlen(newpath)+strlen(p_t) + 1), "%s%s",newpath,p_t);
                        }
                        else
                        {
                            pTemp->re_cmd = (char *)malloc(sizeof(char) * (strlen(newpath) + 1));
                            snprintf(pTemp->re_cmd, sizeof(char) * (strlen(newpath) + 1), "%s/",newpath);
                        }
                    }
                }
                else
                {
                    if((p_t=strstr(pTemp->re_cmd,oldpath_t)) != NULL)
                    {
                        if(strlen(oldpath_t)<strlen(pTemp->re_cmd))
                        {
                            p_t+=strlen(oldpath);
                            char *p_tt=(char *)malloc(sizeof(char)*(strlen(p_t)+1));
                            memset(p_tt,'\0',strlen(p_t)+1);
                            snprintf(p_tt, sizeof(char)*(strlen(p_t)+1), "%s",p_t);
                            free(pTemp->re_cmd);
                            pTemp->re_cmd = (char *)malloc(sizeof(char) * (strlen(newpath)+strlen(p_tt) + 1));
                            memset(pTemp->re_cmd,'\0',strlen(newpath)+strlen(p_tt) + 1);
                            snprintf(pTemp->re_cmd, sizeof(char) * (strlen(newpath)+strlen(p_tt) + 1), "%s%s",newpath,p_tt);
                            free(p_tt);
                        }
                        else
                        {
                            free(pTemp->re_cmd);
                            pTemp->re_cmd = (char *)malloc(sizeof(char) * (strlen(newpath) + 1));
                            snprintf(pTemp->re_cmd, sizeof(char) * (strlen(newpath) + 1), "%s/",newpath);
                        }
                    }
                }
                free(socket_path);
            }
            free(pTemp_t);
            free(oldpath_t);

        pTemp=pTemp->next_ptr;
    }

    show(g_pSyncList[i]->SocketActionList->head);
}

int change_socklist_re_cmd(char *cmd)
{
    char cmd_name[64]="\0";
    char *path;
    char *temp;
    char oldname[256]="\0",newname[256]="\0";
    char *oldpath;
    char *mv_newpath;
    char *mv_oldpath;
    char *ch;

    ch = cmd;
    int i = 0;

    while(*ch != '\n')
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));
    snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
    p = strchr(temp,'\n');

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strcmp(cmd_name, "rename0") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        snprintf(newname, 256, "%s", p1);

        if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    else if(strcmp(cmd_name, "move0") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');
        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;
        snprintf(oldname, 256, "%s", p1);

        if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            free(oldpath);
            return 0;
        }
    }

    free(temp);

    if(strcmp(cmd_name, "move0") == 0)
    {
        mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        mv_oldpath = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
        snprintf(mv_newpath, strlen(path)+strlen(oldname)+2, "%s/%s",path,oldname);
        snprintf(mv_oldpath, strlen(oldpath)+strlen(oldname)+2, "%s/%s",oldpath,oldname);
        free(oldpath);
    }
    else
    {
        mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        snprintf(mv_newpath, strlen(path)+strlen(newname)+2, "%s/%s",path,newname);
        snprintf(mv_oldpath, strlen(path)+strlen(oldname)+2, "%s/%s",path,oldname);
    }

    pthread_mutex_lock(&mutex_socket);
    set_re_cmd(cmd,mv_oldpath,mv_newpath);
    set_copyfile_list(cmd,mv_oldpath,mv_newpath);
    pthread_mutex_unlock(&mutex_socket);

    free(path);
    free(mv_oldpath);
    free(mv_newpath);
    return 0;

}

void *SyncLocal()
{
    int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd*/
    int numbytes;
    char buf[MAXDATASIZE];
    int yes = 1;
    int ret;

    fd_set read_fds;
    fd_set master;
    int fdmax;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_ZERO(&master);

    struct sockaddr_in my_addr; /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    int sin_size;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("Server-setsockopt() error lol!");
        exit(1);
    }

    my_addr.sin_family = AF_INET; /* host byte order */
    my_addr.sin_port = htons(MYPORT); /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
    bzero(&(my_addr.sin_zero), sizeof(my_addr.sin_zero)); /* zero the rest of the struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct
                                                         sockaddr))== -1) {
        perror("bind");
        exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    sin_size = sizeof(struct sockaddr_in);

    FD_SET(sockfd,&master);
    fdmax = sockfd;

    while(!exit_loop)
    { /* main accept() loop */

        timeout.tv_sec = 0;
        timeout.tv_usec = 100;

        read_fds = master;
        ret = select(fdmax+1,&read_fds,NULL,NULL,&timeout);

        switch (ret)
        {
        case 0:
            continue;
            break;
        case -1:
            perror("select");
            continue;
            break;
        default:
            if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
                                 &sin_size)) == -1) {
                perror("accept");
                continue;
            }
            memset(buf, 0, sizeof(buf));

            if ((numbytes=recv(new_fd, buf, MAXDATASIZE, 0)) == -1) {
                perror("recv");
                exit(1);
            }

            if(buf[strlen(buf)] == '\n')
            {
                buf[strlen(buf)] = '\0';
            }
            wd_DEBUG("socket buf = %s\n",buf);
            close(new_fd);

#ifdef RENAME_F
            if(strncmp(buf,"rename0",strlen("rename0")) == 0 || strncmp(buf,"move0",strlen("move0")) == 0)
            {
                change_socklist_re_cmd(buf);
            }
#endif
#if 1
            pthread_mutex_lock(&mutex_socket);
            add_socket_item(buf);
            pthread_mutex_unlock(&mutex_socket);
#endif
        }

    }
    close(sockfd);

    wd_DEBUG("stop dropbox local sync\n");
}
int add_socket_item(char *buf){

    int i;
    char *r_path;
    r_path = get_socket_base_path(buf);
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(!strcmp(r_path,asus_cfg.prule[i]->path))
            break;
    }

    wd_DEBUG("add_socket_item rule:%d\n",i);
    free(r_path);
    pthread_mutex_lock(&mutex_receve_socket);
    g_pSyncList[i]->receve_socket = 1;
    pthread_mutex_unlock(&mutex_receve_socket);

#if MEM_POOL_ENABLE
    SocketActionTmp = mem_alloc(16);
#else
    SocketActionTmp = malloc (sizeof (struct queue_entry));
#endif

    memset(SocketActionTmp,0,sizeof(struct queue_entry));
    int len = strlen(buf)+1;
#if MEM_POOL_ENABLE
    SocketActionTmp->cmd_name = mem_alloc(len);
#else
    SocketActionTmp->cmd_name = (char *)calloc(len,sizeof(char));
#endif
#if MEM_POOL_ENABLE
    sprintf(SocketActionTmp->cmd_name,"%s",buf);
#else
    snprintf(SocketActionTmp->cmd_name, sizeof(char)*len, "%s",buf);
#endif
    SocketActionTmp->re_cmd = NULL;
    SocketActionTmp->is_first = 0;
    queue_enqueue(SocketActionTmp,g_pSyncList[i]->SocketActionList);

    wd_DEBUG("SocketActionTmp->cmd_name = %s\n",SocketActionTmp->cmd_name);
    return 0;
}
char *get_socket_base_path(char *cmd){
    char *temp = NULL;
    char *temp1 = NULL;
    char path[1024];
    char *root_path = NULL;

    if(!strncmp(cmd,"rmroot",6))
    {
        temp = strchr(cmd,'/');
        root_path = my_str_malloc(512);
        snprintf(root_path, sizeof(char)*512, "%s",temp);
    }
    else
    {
        temp = strchr(cmd,'/');
        temp1 = strchr(temp,'\n');
        memset(path,0,sizeof(path));
        strncpy(path,temp,strlen(temp)-strlen(temp1));

        root_path = my_str_malloc(512);
        if(strncmp(path,"/tmp",4) ==0 )
        {
            temp = my_nstrchr('/',path,5);
        }
        else
        {
            temp = my_nstrchr('/',path,4);
        }
        if(temp == NULL)
        {
            snprintf(root_path, sizeof(char)*512, "%s",path);
        }
        else
        {
            snprintf(root_path,strlen(path)-strlen(temp)+1,"%s",path);
        }
    }
    return root_path;
}
void run()
{
    int create_thid1 = 0;
    int create_thid2 = 0;
    int create_thid3 = 0;
    int need_server_thid = 0;

    create_sync_list();
    send_to_inotify();

    if(set_iptables(1))
        exit(-1);

    if(exit_loop == 0)
    {
        if( pthread_create(&newthid2,NULL,(void *)SyncLocal,NULL) != 0)
        {
            wd_DEBUG("thread creation failder\n");
            exit(1);
        }
        create_thid2 = 1;
    }
#if 1
    if(exit_loop == 0)
    {
        wd_DEBUG("create newthid3\n");
        if( pthread_create(&newthid3,NULL,(void *)Socket_Parser,NULL) != 0)
        {
            wd_DEBUG("thread creation failder\n");
            exit(1);
        }
        create_thid3 = 1;
        usleep(1000*500);
    }

#if 1
        sync_initial();
#endif

    finished_initial=1;

    /*
     fix when socket_parse run sync_initial_again ,local send socket,the process will sleep
    */

    if(exit_loop == 0)
    {
        if( pthread_create(&newthid1,NULL,(void *)SyncServer,NULL) != 0)
        {
            wd_DEBUG("thread creation failder\n");
            exit(1);
        }
        create_thid1 = 1;
        usleep(1000*500);
    }

    if(create_thid1)
        pthread_join(newthid1,NULL);
    if(create_thid3)
        pthread_join(newthid3,NULL);
    if(create_thid2)
        pthread_join(newthid2,NULL);

    usleep(1000);
    clean_up();
#if TOKENFILE
    if(stop_progress != 1)
    {
        wd_DEBUG("run again!\n");

        while(disk_change)
        {
            disk_change = 0;
            sync_disk_removed = check_sync_disk_removed();

            if(sync_disk_removed == 2)
            {
                wd_DEBUG("sync path is change\n");
            }
            else if(sync_disk_removed == 1)
            {
                wd_DEBUG("sync disk is unmount\n");
            }
            else if(sync_disk_removed == 0)
            {
                wd_DEBUG("sync disk exists\n");
            }
        }

        exit_loop = 0;
        run();
    }
#endif
#endif
}

void clean_up()
{
    wd_DEBUG("enter clean up\n");

    int i;

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        queue_destroy(g_pSyncList[i]->SocketActionList);

        if(g_pSyncList[i]->ServerRootNode == g_pSyncList[i]->OldServerRootNode)
        {

            wd_DEBUG("the same Pointer!\n");

            if(g_pSyncList[i]->ServerRootNode != NULL)
                free_server_tree(g_pSyncList[i]->ServerRootNode);
        }
        else
        {
            if(g_pSyncList[i]->ServerRootNode != NULL)
                free_server_tree(g_pSyncList[i]->ServerRootNode);

            wd_DEBUG("clean %d ServerRootNode success!\n",i);

            if(g_pSyncList[i]->OldServerRootNode != NULL)
                free_server_tree(g_pSyncList[i]->OldServerRootNode);

            wd_DEBUG("clean %d OldServerRootNode success!\n",i);

        }

        free_action_item(g_pSyncList[i]->server_action_list);
        free_action_item(g_pSyncList[i]->unfinished_list);
        free_action_item(g_pSyncList[i]->copy_file_list);
        free_action_item(g_pSyncList[i]->up_space_not_enough_list);

        if(asus_cfg.prule[i]->rule == 1)
        {
            free_action_item(g_pSyncList[i]->download_only_socket_head);
        }
        free(g_pSyncList[i]);
    }
    free(g_pSyncList);

#if MEM_POOL_ENABLE
    mem_pool_destroy();
#endif

    wd_DEBUG("clean up end !!!\n");
}

void *SyncServer()
{
    struct timeval now;
    struct timespec outtime;

    int status;
    int i;
    while( !exit_loop )
    {

        wd_DEBUG("***************SyncServer start**************\n");

        for(i=0;i<asus_cfg.dir_number;i++)
        {
            status=0;

            while (local_sync == 1 && exit_loop == 0){
                usleep(1000*10);
            }
            server_sync = 1;

            if(exit_loop)
                break;
#if TOKENFILE
            if(disk_change)
            {
                check_disk_change();
            }
            if(g_pSyncList[i]->sync_disk_exist == 0)
            {
                write_log(S_ERROR,"Sync disk unplug!","",i);
                continue;
            }

#endif
            if(g_pSyncList[i]->no_local_root)
            {
                my_mkdir_r(asus_cfg.prule[i]->path);   //have mountpath
                send_action(asus_cfg.type,asus_cfg.prule[i]->path);
                usleep(1000*10);
                g_pSyncList[i]->no_local_root = 0;
                g_pSyncList[i]->init_completed = 0;
            }
            status = do_unfinished(i);

            if( !g_pSyncList[i]->init_completed )
                status = sync_initial();

            if(status != 0)
                continue;

            if(asus_cfg.prule[i]->rule == 2)
            {
                continue;
            }

            if(exit_loop == 0)
            {
                int get_serlist_fail_time = 0;
                do
                {
                    g_pSyncList[i]->ServerRootNode = create_server_treeroot();
#ifdef MULTI_PATH
                    status = browse_to_tree(asus_cfg.prule[i]->rooturl,g_pSyncList[i]->ServerRootNode);
#else
                    status = browse_to_tree("/",g_pSyncList[i]->ServerRootNode);
#endif
#ifdef __DEBUG__
                    SearchServerTree(g_pSyncList[i]->ServerRootNode);
#endif
                    /*
                for(i=0;i<asus_cfg.dir_number;i++)
                {
                    SearchServerTree(g_pSyncList[i]->ServerRootNode);
                    free_server_tree(g_pSyncList[i]->ServerRootNode);
                }
                */
                    if(status != 0)
                    {
                        wd_DEBUG("get ServerList ERROR! \n");
                        get_serlist_fail_time ++;
                        free_server_tree(g_pSyncList[i]->ServerRootNode);
                        g_pSyncList[i]->ServerRootNode = NULL;
                    }


                }while(status!=0 && get_serlist_fail_time < 5 && exit_loop == 0 && g_pSyncList[i]->receve_socket == 0);
                if (status != 0)
                {
                    /*auth again:
                        for the token not work!
                    */
#ifdef OAuth1
                    if(g_pSyncList[i]->receve_socket == 0)
                    {
                        if(exit_loop == 0)
                            do_auth();
                    }
#endif

                    /*first_sync:
                        after the initial finish,the serverlist is change so force to run server sync;
                        when the server sync failed,next time we must force to run server sync;
                    */
                    g_pSyncList[i]->first_sync = 1;
                    usleep(1000*20);
                    continue;
                }

                if(g_pSyncList[i]->unfinished_list->next != NULL)
                {
                    continue;
                }

                if(g_pSyncList[i]->first_sync)
                {

                    wd_DEBUG("first sync!\n");
                    g_pSyncList[i]->VeryOldServerRootNode=g_pSyncList[i]->OldServerRootNode;
                    g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                    status=Server_sync(i);
                    free_server_tree(g_pSyncList[i]->VeryOldServerRootNode);
                    if(status == 0)
                        g_pSyncList[i]->first_sync = 0;
                    else
                        g_pSyncList[i]->first_sync = 1;
                }
                else
                {
                    if(asus_cfg.prule[i]->rule == 0)
                    {
                        status=compareServerList(i);
                    }
                    if (status == 0 || asus_cfg.prule[i]->rule == 1)
                    {

                        g_pSyncList[i]->VeryOldServerRootNode=g_pSyncList[i]->OldServerRootNode;
                        g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                        status=Server_sync(i);
                        free_server_tree(g_pSyncList[i]->VeryOldServerRootNode);
                        if(status == 0)
                            g_pSyncList[i]->first_sync = 0;
                        else
                            g_pSyncList[i]->first_sync = 1;
                    }
                    else
                    {//serverList same

                        free_server_tree(g_pSyncList[i]->ServerRootNode);
                        g_pSyncList[i]->ServerRootNode = NULL;
                        status = 0;
                    }
                }
            }
                write_log(S_SYNC,"","",i);
        }
        server_sync = 0;      //server sync finished
        pthread_mutex_lock(&mutex);
        if(!exit_loop)
        {
            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec + 10;
            outtime.tv_nsec = now.tv_usec * 1000;
            pthread_cond_timedwait(&cond, &mutex, &outtime);
        }
        pthread_mutex_unlock(&mutex);
    }
    wd_DEBUG("stop dropbox server sync\n");
}

int Server_sync(int index)
{

    wd_DEBUG("compareLocalList start!\n");

    int ret = 0;

    if(g_pSyncList[index]->ServerRootNode->Child != NULL)
    {

        wd_DEBUG("ServerRootNode->Child != NULL\n");

        ret = sync_local_with_server(g_pSyncList[index]->ServerRootNode->Child,sync_local_with_server_perform,index);
    }
    else
    {

        wd_DEBUG("ServerRootNode->Child == NULL\n");

    }

    return ret;
}

/*
 *judge is server changed
 *0:server changed
 *1:server is not changed
*/
int isServerChanged(Server_TreeNode *newNode,Server_TreeNode *oldNode)
{
    int res = 1;
    int serverchanged = 0;
    if(newNode->browse == NULL && oldNode->browse == NULL)
    {
        return 1;
    }
    else if(newNode->browse == NULL && oldNode->browse != NULL)
    {
        return 0;
    }
    else if(newNode->browse != NULL && oldNode->browse == NULL)
    {
        return 0;
    }
    else
    {
        if(newNode->browse->filenumber != oldNode->browse->filenumber || newNode->browse->foldernumber != oldNode->browse->foldernumber)
        {
            return 0;
        }
        else
        {
            int cmp;
            CloudFile *newfoldertmp = NULL;
            CloudFile *oldfoldertmp = NULL;
            CloudFile *newfiletmp = NULL;
            CloudFile *oldfiletmp = NULL;
            if(newNode->browse != NULL)
            {
                if(newNode->browse->foldernumber > 0)
                    newfoldertmp = newNode->browse->folderlist->next;
                if(newNode->browse->filenumber > 0)
                    newfiletmp = newNode->browse->filelist->next;
            }
            if(oldNode->browse != NULL)
            {
                if(oldNode->browse->foldernumber > 0)
                    oldfoldertmp = oldNode->browse->folderlist->next;
                if(oldNode->browse->filenumber > 0)
                    oldfiletmp = oldNode->browse->filelist->next;
            }

            while (newfoldertmp != NULL || oldfoldertmp != NULL)
            {
                if ((cmp = strcmp(newfoldertmp->href,oldfoldertmp->href)) != 0){
                    return 0;
                }
                newfoldertmp = newfoldertmp->next;
                oldfoldertmp = oldfoldertmp->next;
            }
            while (newfiletmp != NULL || oldfiletmp != NULL)
            {
                if ((cmp = strcmp(newfiletmp->href,oldfiletmp->href)) != 0)
                {
                    return 0;
                }
                if (newfiletmp->mtime != oldfiletmp->mtime)
                {
                    return 0;
                }
                newfiletmp = newfiletmp->next;
                oldfiletmp = oldfiletmp->next;
            }
        }

        if((newNode->Child == NULL && oldNode->Child != NULL) || (newNode->Child != NULL && oldNode->Child == NULL))
        {
            return 0;
        }
        if((newNode->NextBrother == NULL && oldNode->NextBrother != NULL) || (newNode->NextBrother!= NULL && oldNode->NextBrother == NULL))
        {
            return 0;
        }

        if(newNode->Child != NULL && oldNode->Child != NULL && !exit_loop)
        {
            res = isServerChanged(newNode->Child,oldNode->Child);
            if(res == 0)
            {
                serverchanged = 1;
            }
        }
        if(newNode->NextBrother != NULL && oldNode->NextBrother != NULL && !exit_loop)
        {
            res = isServerChanged(newNode->NextBrother,oldNode->NextBrother);
            if(res == 0)
            {
                serverchanged = 1;
            }
        }
    }
    if(serverchanged == 1)
    {

        wd_DEBUG("########Server changed9\n");

        return 0;
    }
    else
    {
        return 1;
    }
}

/*ret = 0,server changed
 *ret = 1,server is no changed
*/
int compareServerList(int index)
{
    int ret;

    wd_DEBUG("#########compareServerList\n");

    if(g_pSyncList[index]->ServerRootNode->Child != NULL && g_pSyncList[index]->OldServerRootNode->Child != NULL)
    {
        ret = isServerChanged(g_pSyncList[index]->ServerRootNode->Child,g_pSyncList[index]->OldServerRootNode->Child);
        return ret;
    }
    else if(g_pSyncList[index]->ServerRootNode->Child == NULL && g_pSyncList[index]->OldServerRootNode->Child == NULL)
    {
        ret = 1;
        return ret;
    }
    else
    {
        ret = 0;
        return ret;
    }
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
int download_only_add_socket_item(char *cmd,int index)
{

    wd_DEBUG("download_only_add_socket_item receive socket : %s\n",cmd);

    if( strstr(cmd,"(conflict)") != NULL )
        return 0;

    wd_DEBUG("socket command is %s \n",cmd);


    if( !strncmp(cmd,"exit",4))
    {

        wd_DEBUG("exit socket\n");

        return 0;
    }

    if(!strncmp(cmd,"rmroot",6))
    {
        g_pSyncList[index]->no_local_root = 1;
        return 0;
    }


    char cmd_name[64];
    char *path = NULL;
    char *temp = NULL;
    char filename[256];
    char *fullname = NULL;
    char oldname[256],newname[256];
    char *oldpath = NULL;
    char action[64];
    char *ch = NULL;
    char *old_fullname = NULL;

    memset(cmd_name,'\0',sizeof(cmd_name));
    memset(oldname,'\0',sizeof(oldname));
    memset(newname,'\0',sizeof(newname));
    memset(action,'\0',sizeof(action));

    ch = cmd;
    int i = 0;

    while(*ch != '\n')
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));

    snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
    p = strchr(temp,'\n');

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",6) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;
        snprintf(newname, 256, "%s", p1);

        wd_DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

    }
    else if(strncmp(cmd_name, "move",4) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');
        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        snprintf(oldname, 256, "%s", p1);

        wd_DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);

    }
    else
    {
        snprintf(filename, 256, "%s", p);
        wd_DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);
    }

    free(temp);

    if( !strncmp(cmd_name,"rename",strlen("rename")) )
    {
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        old_fullname = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        snprintf(fullname, sizeof(char)*(strlen(path)+strlen(newname)+2), "%s/%s",path,newname);
        snprintf(old_fullname, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
        free(path);
    }
    else if( !strncmp(cmd_name,"move",strlen("move")) )
    {
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        old_fullname = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
        snprintf(fullname, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
        snprintf(old_fullname, sizeof(char)*(strlen(oldpath)+strlen(oldname)+2), "%s/%s",oldpath,oldname);
        free(oldpath);
        free(path);
    }
    else
    {
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
        snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
        free(path);
    }
    if( !strncmp(cmd_name,"copyfile",strlen("copyfile")) )
    {
        add_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list);
        free(fullname);
        return 0;
    }


    if( strcmp(cmd_name, "createfile") == 0 )
    {
        snprintf(action, 64, "%s", "createfile");
        action_item *item;

        item = get_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list,index);

        if(item != NULL)
        {
            wd_DEBUG("##### delete copyfile %s ######\n",fullname);
            del_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list);
        }
    }
    else if( strcmp(cmd_name, "cancelcopy") == 0 )
    {
        action_item *item;

        item = get_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list,index);

        if(item != NULL)
        {
            wd_DEBUG("##### delete copyfile %s ######\n",fullname);
            del_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list);
        }
        free(fullname);
        return 0;
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        snprintf(action, 64, "%s", "remove");
        del_download_only_action_item(action,fullname,g_pSyncList[index]->download_only_socket_head);
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        snprintf(action, 64, "%s", "createfolder");
    }
    else if( strncmp(cmd_name, "rename",6) == 0 )
    {
        snprintf(action, 64, "%s", "rename");
        del_download_only_action_item(action,old_fullname,g_pSyncList[index]->download_only_socket_head);
        free(old_fullname);
    }
    else if( strncmp(cmd_name, "move",4) == 0 )
    {
        snprintf(action, 64, "%s", "move");
        del_download_only_action_item(action,old_fullname,g_pSyncList[index]->download_only_socket_head);
    }

    if(g_pSyncList[index]->server_action_list->next != NULL)
    {
        action_item *item;

        item = get_action_item(action,fullname,g_pSyncList[index]->server_action_list,index);

        if(item != NULL)
        {

            wd_DEBUG("##### %s %s by dropbox Server self ######\n",action,fullname);
            del_action_item(action,fullname,g_pSyncList[index]->server_action_list);
            free(fullname);
            return 0;
        }
    }

    if( strcmp(cmd_name, "copyfile") != 0 )
    {
        g_pSyncList[index]->have_local_socket = 1;
    }

    if(strncmp(cmd_name, "rename",6) == 0)
    {
        if(test_if_dir(fullname))
        {
            add_all_download_only_socket_list(cmd_name,fullname,index);
        }
        else
        {
            add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
        }
    }
    else if(strncmp(cmd_name, "move",4) == 0)
    {
        if(test_if_dir(fullname))
        {
            add_all_download_only_socket_list(cmd_name,fullname,index);
        }
        else
        {
            add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
        }
    }
    else if(strcmp(cmd_name, "createfolder") == 0 || strcmp(cmd_name, "dragfolder") == 0)
    {
        add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
        if(!strcmp(cmd_name, "dragfolder"))
            add_all_download_only_dragfolder_socket_list(fullname,index);
    }
    else if( strcmp(cmd_name, "createfile") == 0  || strcmp(cmd_name, "dragfile") == 0 || strcmp(cmd_name, "modify") == 0)
    {
        add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
    }

    free(fullname);
    return 0;


}
void *Socket_Parser()
{

    wd_DEBUG("*******Socket_Parser start********\n");

    queue_entry_t socket_execute;
    int i;
    int status;
    int has_socket = 0;
    int fail_flag;
    struct timeval now;
    struct timespec outtime;
    while(!exit_loop)
    {
        for(i=0;i<asus_cfg.dir_number;i++)
        {
            while(server_sync == 1 && exit_loop ==0)
            {
                usleep(1000*10);
            }
            local_sync=1;

            if(exit_loop)
                break;
#if TOKENFILE
            if(disk_change)
            {
                check_disk_change();
            }

            if(g_pSyncList[i]->sync_disk_exist == 0)
                continue;
#endif
            if(asus_cfg.prule[i]->rule == 1) //Download only
            {
                while(exit_loop == 0)
                {
                    while(g_pSyncList[i]->SocketActionList->head != NULL && exit_loop == 0 && server_sync == 0)
                    {
                        has_socket = 1;
                        socket_execute=g_pSyncList[i]->SocketActionList->head;
                        status = download_only_add_socket_item(socket_execute->cmd_name,i);
                        if(status == 0  || status == SERVER_SPACE_NOT_ENOUGH
                           || status == LOCAL_FILE_LOST)
                        {
                            pthread_mutex_lock(&mutex_socket);
                            socket_execute = queue_dequeue(g_pSyncList[i]->SocketActionList);

#if MEM_POOL_ENABLE
                    mem_free(socket_execute->cmd_name);
                    mem_free(socket_execute->re_cmd);
                    mem_free(socket_execute);
#else
                    if(socket_execute->re_cmd)
                        free(socket_execute->re_cmd);
                    if(socket_execute->cmd_name)
                        free(socket_execute->cmd_name);
                    free(socket_execute);
#endif
                            pthread_mutex_unlock(&mutex_socket);
                        }
                        else
                        {
                            fail_flag = 1;

                            wd_DEBUG("######## socket item fail########\n");

                            break;
                        }
                        usleep(1000*20);
                    }
                    if(fail_flag)
                        break;

                    if(g_pSyncList[i]->copy_file_list->next == NULL)
                    {
                        break;
                    }
                    else
                    {
                        usleep(1000*100);
                    }
                }
                if(g_pSyncList[i]->server_action_list->next != NULL && g_pSyncList[i]->SocketActionList->head == NULL)
                {
                    free_action_item(g_pSyncList[i]->server_action_list);
                    g_pSyncList[i]->server_action_list = create_action_item_head();
                }
                pthread_mutex_lock(&mutex_receve_socket);
                if(g_pSyncList[i]->SocketActionList->head == NULL)
                    g_pSyncList[i]->receve_socket = 0;
                pthread_mutex_unlock(&mutex_receve_socket);
            }
            else
            {
                if(asus_cfg.prule[i]->rule == 2)
                {
#if 0
                    /*
                     fix upload only rule rm sync dir , can not create new sync dir;
                    */

                    if(g_pSyncList[i]->no_local_root)
                    {
                        my_mkdir_r(asus_cfg.prule[i]->path);   //have mountpath
                        send_action(asus_cfg.type,asus_cfg.prule[i]->path);
                        usleep(1000*10);
                        g_pSyncList[i]->no_local_root = 0;
                    }

                    /*
                     fix upload only initial failed,must run intial again();
                    */
                    if(finished_initial)
                    {
                        if( g_pSyncList[i]->init_completed != 1)
                        {
                            sync_initial_again(i);
                        }
                    }

                    /*upload only rule is not server pthread,so unfinished list del is here*/
                    status = do_unfinished(i);
#endif
                }
                while(exit_loop == 0)
                {
                    while(g_pSyncList[i]->SocketActionList->head != NULL && server_sync ==0 && exit_loop ==0)
                    {
                        has_socket = 1;
                        socket_execute=g_pSyncList[i]->SocketActionList->head;

                        wd_DEBUG("######## socket cmd : %s########\n",socket_execute->cmd_name);

                        status=cmd_parser(socket_execute->cmd_name,i,socket_execute->re_cmd);
                        if(status == 0 || status == SERVER_SPACE_NOT_ENOUGH )
                        {
                            wd_DEBUG("del socket item ok?\n");
                            pthread_mutex_lock(&mutex_socket);
                            socket_execute = queue_dequeue(g_pSyncList[i]->SocketActionList);
#if MEM_POOL_ENABLE
                    mem_free(socket_execute->cmd_name);
                    mem_free(socket_execute->re_cmd);
                    mem_free(socket_execute);
#else
                    if(socket_execute->re_cmd)
                        free(socket_execute->re_cmd);
                    if(socket_execute->cmd_name)
                        free(socket_execute->cmd_name);
                    free(socket_execute);
#endif
                            wd_DEBUG("del socket item ok\n");
                            pthread_mutex_unlock(&mutex_socket);
                        }
                        else if(status == LOCAL_FILE_LOST )
                        {
                            wd_DEBUG("del socket item ok?\n");
                            pthread_mutex_lock(&mutex_socket);
                            socket_execute = queue_dequeue_t(g_pSyncList[i]->SocketActionList);
#if MEM_POOL_ENABLE
                    mem_free(socket_execute->cmd_name);
                    mem_free(socket_execute->re_cmd);
                    mem_free(socket_execute);
#else
                    if(socket_execute->re_cmd)
                        free(socket_execute->re_cmd);
                    if(socket_execute->cmd_name)
                        free(socket_execute->cmd_name);
                    free(socket_execute);
#endif
                            wd_DEBUG("del socket item ok\n");
                            pthread_mutex_unlock(&mutex_socket);
                        }
                        else
                        {
                            fail_flag = 1;
                            break;
                        }
                        usleep(1000*20);
                    }
                    if(fail_flag)
                        break;

                    if(g_pSyncList[i]->copy_file_list->next == NULL)
                    {
                        break;
                    }
                    else
                    {
                        usleep(1000*100);
                    }
                }
                if(g_pSyncList[i]->server_action_list->next != NULL && g_pSyncList[i]->SocketActionList->head == NULL)
                {
                    free_action_item(g_pSyncList[i]->server_action_list);
                    g_pSyncList[i]->server_action_list = create_action_item_head();
                }
                pthread_mutex_lock(&mutex_receve_socket);
                if(g_pSyncList[i]->SocketActionList->head == NULL)
                {
                    g_pSyncList[i]->receve_socket = 0;
                }
                pthread_mutex_unlock(&mutex_receve_socket);
            }

        }
        local_sync = 0;
        pthread_mutex_lock(&mutex_socket);
        if(!exit_loop)
        {
            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec + 2;
            outtime.tv_nsec = now.tv_usec * 1000;
            pthread_cond_timedwait(&cond_socket, &mutex_socket, &outtime);
        }
        pthread_mutex_unlock(&mutex_socket);
    }
}
int cmd_parser(char *cmd,int index,char *re_cmd)
{
    if( !strncmp(cmd,"exit",4))
    {
        wd_DEBUG("exit socket\n");

        return 0;
    }

    if(!strncmp(cmd,"rmroot",6))
    {
        g_pSyncList[index]->no_local_root = 1;
        return 0;
    }

    char cmd_name[64]="\0";
    char *path;
    char *temp;
    char filename[256]="\0";
    char *fullname;
    char *fullname_t = NULL;
    char oldname[256]="\0",newname[256]="\0";
    char *oldpath;
    char action[64]="\0";
    char *cmp_name;
    char *mv_newpath;
    char *mv_oldpath;
    char *ch;
    int status;

    ch = cmd;
    int i = 0;

    while(*ch != '\n')
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));
    snprintf(temp, sizeof(char)*(strlen(ch)+1), "%s", ch);
    p = strchr(temp,'\n');
    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",strlen("rename")) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        snprintf(newname, 256, "%s", p1);

        wd_DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

        if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    else if(strncmp(cmd_name, "move",strlen("move")) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,'\n');

        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        snprintf(oldname, 256, "%s", p1);

        wd_DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);


        if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            free(oldpath);
            return 0;
        }
    }
    else if(strcmp(cmd_name, "delete") == 0 || strcmp(cmd_name, "remove") == 0)
    {
        snprintf(filename, 256, "%s", p);
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));

        wd_DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);

        if(filename[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    else
    {
        snprintf(filename, 256, "%s", p);
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
        if(re_cmd != NULL)
            fullname_t = my_str_malloc((size_t)(strlen(re_cmd)+strlen(filename)+2));

        wd_DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);

        if(filename[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }

    free(temp);

    if( !strncmp(cmd_name,"rename",strlen("rename")) )
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        snprintf(cmp_name, sizeof(char)*(strlen(path)+strlen(newname)+2), "%s/%s",path,newname);
    }
    else if( !strcmp(cmd_name,"delete") || !strcmp(cmd_name,"remove"))
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
        snprintf(cmp_name, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
    }
    else
    {
        if(re_cmd == NULL)
        {
            cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
            snprintf(cmp_name, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
        }
        else
        {
            cmp_name = my_str_malloc((size_t)(strlen(re_cmd)+strlen(filename)+2));
            snprintf(cmp_name, sizeof(char)*(strlen(re_cmd)+strlen(filename)+2), "%s%s",re_cmd,filename);
        }
    }

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        snprintf(action, 64, "%s", "createfile");
        action_item *item;

        item = get_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list,index);

        if(item != NULL)
        {

            wd_DEBUG("##### delete copyfile %s ######\n",cmp_name);

            del_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list);
        }
    }
    else if( strcmp(cmd_name, "cancelcopy") == 0 )
    {
        action_item *item;

        item = get_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list,index);

        if(item != NULL)
        {

            wd_DEBUG("##### delete copyfile %s ######\n",cmp_name);

            del_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list);
        }
        free(path);
        free(cmp_name);
        free(fullname);
        return 0;
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        snprintf(action, 64, "%s", "remove");
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        snprintf(action, 64, "%s", "createfolder");
    }
    else if( strncmp(cmd_name, "rename",strlen("rename")) == 0 )
    {
        snprintf(action, 64, "%s", "rename");
    }
#if 1
    if(g_pSyncList[index]->server_action_list->next != NULL)
    {
        action_item *item;

        item = get_action_item(action,cmp_name,g_pSyncList[index]->server_action_list,index);

        if(item != NULL)
        {

            wd_DEBUG("##### %s %s by Dropbox Server self ######\n",action,cmp_name);

            del_action_item(action,cmp_name,g_pSyncList[index]->server_action_list);

            wd_DEBUG("#### del action item success!\n");

            free(path);
            if( strncmp(cmd_name, "rename",strlen("rename")) != 0 )
                free(fullname);
            free(cmp_name);
            return 0;
        }
    }
#endif
    free(cmp_name);

    wd_DEBUG("###### %s is start ######\n",cmd_name);

    if( strcmp(cmd_name, "copyfile") != 0 )
    {
        g_pSyncList[index]->have_local_socket = 1;
    }

    if( strcmp(cmd_name, "createfile") == 0 || strcmp(cmd_name, "dragfile") == 0 )
    {

        snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
        char *serverpath=localpath_to_serverpath(fullname,index);
        if(re_cmd != NULL)
        {
            snprintf(fullname_t, strlen(re_cmd)+strlen(filename)+2, "%s%s",re_cmd,filename);
            status=upload_file(fullname_t,serverpath,0,index);
        }
        else
        {
            status=upload_file(fullname,serverpath,0,index);
        }

        /*third param flase,false=>if server file has exit ,will not be overwrite*/
        if(status == 0)
        {
            char *newlocalname = NULL;
            cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
            time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
            cJSON_Delete(json);
            if(re_cmd == NULL)
                newlocalname=change_local_same_file(fullname,index);
            else
                newlocalname=change_local_same_file(fullname_t,index);
            if(newlocalname)
                ChangeFile_modtime(newlocalname,mtime);
            if(re_cmd)
                free(fullname_t);
            free(fullname);
            free(newlocalname);
            free(serverpath);
        }
        else
        {

            wd_DEBUG("upload %s failed\n",fullname);
            if(re_cmd)
                free(fullname_t);
            free(path);
            free(fullname);
            free(serverpath);
            return status;
        }
    }
    else if( strcmp(cmd_name, "copyfile") == 0 )
    {
        if(re_cmd == NULL)
        {
            snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
            add_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list);
        }
        else
        {
            snprintf(fullname_t, sizeof(char)*(strlen(re_cmd)+strlen(filename)+2), "%s%s",re_cmd,filename);
            add_action_item("copyfile",fullname_t,g_pSyncList[index]->copy_file_list);
        }
        free(fullname);
        if(re_cmd)
            free(fullname_t);
    }
    else if( strcmp(cmd_name, "modify") == 0 )
    {
        time_t mtime_1,mtime_2;

        snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
        char *serverpath=localpath_to_serverpath(fullname,index);
        if(re_cmd != NULL)
            snprintf(fullname_t, sizeof(char)*(strlen(re_cmd)+strlen(filename)+2), "%s%s",re_cmd,filename);

        CloudFile *filetmp;
        filetmp=get_CloudFile_node(g_pSyncList[index]->OldServerRootNode,serverpath,0x2);
        if(filetmp == NULL)
        {
            mtime_1=(time_t)-1;
        }
        else
        {
            mtime_1=filetmp->mtime;
        }
        mtime_2=api_getmtime(serverpath,dofile);
        if(mtime_2 == -1)
        {
            /*only upload */
            /*third param ture,ture=>if server file has exit ,will be overwrite*/
            if(re_cmd == NULL)
                status=upload_file(fullname,serverpath,1,index);
            else
                status=upload_file(fullname_t,serverpath,1,index);
            if(status == 0)
            {
                cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                cJSON_Delete(json);
                if(re_cmd == NULL)
                    ChangeFile_modtime(fullname,mtime);
                else
                    ChangeFile_modtime(fullname_t,mtime);
                if(re_cmd)
                    free(fullname_t);
                free(fullname);
                free(serverpath);
            }
            else
            {

                wd_DEBUG("upload %s failed\n",fullname);

                if(re_cmd)
                    free(fullname_t);
                free(path);
                free(fullname);
                free(serverpath);
                return status;
            }
        }
        else if(mtime_2 != -1 && mtime_2 != -2)
        {
            if(mtime_1 == mtime_2)
            {
                /*third param ture,ture=>if server file has exit ,will be overwrite*/
                if(re_cmd == NULL)
                    status=upload_file(fullname,serverpath,1,index);
                else
                    status=upload_file(fullname_t,serverpath,1,index);
                if(status == 0)
                {
                    cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                    cJSON_Delete(json);
                    if(re_cmd == NULL)
                        ChangeFile_modtime(fullname,mtime);
                    else
                        ChangeFile_modtime(fullname_t,mtime);
                    if(re_cmd)
                        free(fullname_t);
                    free(fullname);
                    free(serverpath);
                }
                else
                {

                    wd_DEBUG("upload %s failed\n",fullname);

                    if(re_cmd)
                        free(fullname_t);
                    free(path);
                    free(fullname);
                    free(serverpath);
                    return status;
                }
            }
            else
            {
                /*rename then upload*/
                /*third param false,false=>if server file has exit ,will not be overwrite*/
                if(re_cmd == NULL)
                    status=upload_file(fullname,serverpath,0,index);
                else
                    status=upload_file(fullname_t,serverpath,0,index);
                if(status == 0)
                {
                    char *newlocalname = NULL;
                    cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                    cJSON_Delete(json);
                    if(re_cmd == NULL)
                        newlocalname=change_local_same_file(fullname,index);
                    else
                        newlocalname=change_local_same_file(fullname_t,index);
                    if(newlocalname)
                        ChangeFile_modtime(newlocalname,mtime);
                    if(re_cmd)
                        free(fullname_t);
                    free(newlocalname);
                    free(fullname);
                    free(serverpath);
                }
                else
                {

                    wd_DEBUG("upload %s failed\n",fullname);

                    if(re_cmd)
                        free(fullname_t);
                    free(path);
                    free(fullname);
                    free(serverpath);
                    return status;
                }
            }
        }
        else
        {
            if(re_cmd)
                free(fullname_t);
            free(path);
            free(fullname);
            free(serverpath);
            return -1;
        }
#if 0
        if(mtime_1 == mtime_2 || mtime_2 == -1 ||asus_cfg.prule[index]->rule == 2)
        {
            /*third param ture,ture=>if server file has exit ,will be overwrite*/
            status=upload_file(fullname,serverpath,1,index);
            if(status == 0)
            {
                time_t mtime=cJSON_printf(dofile(Con(TMP_R,upload_chunk_commit.txt),"modified");
                ChangeFile_modtime(fullname,mtime);
                free(fullname);
                free(serverpath);
            }
            else
            {

                wd_DEBUG("upload %s failed\n",fullname);

                free(path);
                free(fullname);
                free(serverpath);
                return status;
            }
        }
        else if(mtime_1 !=mtime_2 && mtime_2 != -1 && asus_cfg.prule[index]->rule ==0)
        {
            /*third param false,false=>if server file has exit ,will not be overwrite*/
            status=upload_file(fullname,serverpath,0,index);
            if(status == 0)
            {
                char *newlocalname;
                time_t mtime=cJSON_printf(dofile(Con(TMP_R,upload_chunk_commit.txt),"modified");
                newlocalname=change_local_same_file(fullname,index);
                if(newlocalname)
                    ChangeFile_modtime(newlocalname,mtime);
                free(newlocalname);
                free(fullname);
                free(serverpath);
            }
            else
            {

                wd_DEBUG("upload %s failed\n",fullname);

                free(path);
                free(fullname);
                free(serverpath);
                return status;
            }
        }
        else if(mtime_2 == -2) //when the network is not conncet ,the curl resporen -2;
        {
            free(path);
            free(fullname);
            free(serverpath);
            return -1;
        }
#endif
    }
    else if(strcmp(cmd_name, "delete") == 0  || strcmp(cmd_name, "remove") == 0)
    {
        action_item *item;
        item = get_action_item_access("upload",fullname,g_pSyncList[index]->access_failed_list,index);
        if(item != NULL)
        {
            del_action_item("upload",item->path,g_pSyncList[index]->access_failed_list);
        }

        snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
        char *serverpath=localpath_to_serverpath(fullname,index);

        char *serverpath_1=localpath_to_serverpath(path,index);
        if(is_server_exist(serverpath_1,serverpath,index) == 0)
        {
            my_free(serverpath_1);
            my_free(serverpath);
            free(fullname);
            free(path);
            return 0;
        }

        status=api_delete(serverpath,index);

        if(status != 0)
        {
            wd_DEBUG("delete failed\n");

            free(path);
            free(fullname);
            free(serverpath);
            return status;
        }
        free(fullname);
        free(serverpath);
    }
    else if(strncmp(cmd_name, "move",strlen("move")) == 0 || strncmp(cmd_name, "rename",strlen("rename")) == 0)
    {
        if(strncmp(cmd_name, "move",strlen("move")) == 0)
        {
            mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
            mv_oldpath = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
            snprintf(mv_newpath, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
            snprintf(mv_oldpath, sizeof(char)*(strlen(oldpath)+strlen(oldname)+2), "%s/%s",oldpath,oldname);
            free(oldpath);
            if(re_cmd)
            {
                fullname_t = (char *)malloc(sizeof(char)*(strlen(re_cmd)+strlen(oldname)+1));
                memset(fullname_t,'\0',strlen(re_cmd)+strlen(oldname)+1);
                snprintf(fullname_t, sizeof(char)*(strlen(re_cmd)+strlen(oldname)+1), "%s%s",re_cmd,oldname);
            }
        }
        else
        {
            mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
            mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
            snprintf(mv_newpath, sizeof(char)*(strlen(path)+strlen(newname)+2), "%s/%s",path,newname);
            snprintf(mv_oldpath, sizeof(char)*(strlen(path)+strlen(oldname)+2), "%s/%s",path,oldname);
            if(re_cmd)
            {
                fullname_t = (char *)malloc(sizeof(char)*(strlen(re_cmd)+strlen(newname)+1));
                memset(fullname_t,'\0',strlen(re_cmd)+strlen(newname)+1);
                snprintf(fullname_t, sizeof(char)*(strlen(re_cmd)+strlen(newname)+1), "%s%s",re_cmd,newname);
            }
        }
        if(strncmp(cmd_name, "rename",strlen("rename")) == 0)
        {            
                int exist=0;
                char *serverpath=localpath_to_serverpath(mv_newpath,index);
                char *serverpath_old=localpath_to_serverpath(mv_oldpath,index);
                /*if the newer name has exist in server,the server same name will be rename ,than rename the oldname to newer*/
                char *serverpath_1=localpath_to_serverpath(path,index);
                exist=is_server_exist(serverpath_1,serverpath,index);
                wd_DEBUG("exist = %d\n",exist);
                if(exist)
                {
                    char *newname;
                    newname=change_server_same_name(serverpath,index);

                    status = api_move(serverpath,newname,index,0,NULL);


                    if(status == 0)
                    {
                        char *err_msg = write_error_message("server file %s is renamed to %s",serverpath,newname);
                        write_trans_excep_log(serverpath,3,err_msg);
                        free(err_msg);
                        status = api_move(serverpath_old,serverpath,index,1,fullname_t);
                    }
                    free(newname);
                }
                else
                {
                    status = api_move(serverpath_old,serverpath,index,1,fullname_t);
                }
                free(serverpath);
                free(serverpath_old);
                free(serverpath_1);
                if(re_cmd)
                    free(fullname_t);
                if(status != 0)
                {
                    wd_DEBUG("move/rename failed\n");
                    free(mv_oldpath);
                    free(mv_newpath);
                    free(path);
                    return status;
                }
        }
        else  /*action : move*/
        {
            int exist = 0;
            int old_index;
            old_index=get_path_to_index(mv_oldpath);
            /*the move file is in other rules folder or root path*/
            /*index is 0 or 2
              0=>sync
              2=>upload
            */
            if(asus_cfg.prule[old_index]->rule == 1)
            {
                del_download_only_action_item("move",mv_oldpath,g_pSyncList[old_index]->download_only_socket_head);
            }

            char *serverpath=localpath_to_serverpath(mv_newpath,index);
            char *serverpath_old=localpath_to_serverpath(mv_oldpath,index);
            char *serverpath_1=localpath_to_serverpath(path,index);
            exist=is_server_exist(serverpath_1,serverpath,index);
            if(exist)
            {
                char *newname;
                newname=change_server_same_name(serverpath,index);

                status = api_move(serverpath,newname,index,0,NULL);

                if(status == 0)
                {
                    char *err_msg = write_error_message("server file %s is renamed to %s",serverpath,newname);
                    write_trans_excep_log(serverpath,3,err_msg);
                    free(err_msg);
                    status = api_move(serverpath_old,serverpath,index,1,fullname_t);
                }

                free(newname);
            }
            else
            {
                status = api_move(serverpath_old,serverpath,index,1,fullname_t);
            }
            free(serverpath);
            free(serverpath_old);
            free(serverpath_1);
            if(re_cmd)
                free(fullname_t);
            if(status != 0)
            {
                wd_DEBUG("move/rename failed\n");

                free(path);
                free(mv_oldpath);
                free(mv_newpath);
                return status;
            }
        }
        free(mv_oldpath);
        free(mv_newpath);

    }
    else if( strcmp(cmd_name,"dragfolder") == 0)
    {
        char info[512];
        memset(info,0,sizeof(info));
        if(re_cmd == NULL)
        {
            snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
            snprintf(info, 512, "createfolder%s%s%s%s",CMD_SPLIT,path,CMD_SPLIT,filename);
        }
        else
        {
            snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s%s",re_cmd,filename);
            snprintf(info, 512, "createfolder%s%s%s%s",CMD_SPLIT,re_cmd,CMD_SPLIT,filename);
        }


        pthread_mutex_lock(&mutex_socket);
        add_socket_item(info);
        pthread_mutex_unlock(&mutex_socket);
        deal_dragfolder_to_socketlist(fullname,index);
        if(re_cmd)
            free(fullname_t);
        free(fullname);
    }
    else if(strcmp(cmd_name, "createfolder") == 0)
    {
        int exist;

        snprintf(fullname, sizeof(char)*(strlen(path)+strlen(filename)+2), "%s/%s",path,filename);
        char *serverpath=localpath_to_serverpath(fullname,index);
        if(re_cmd != NULL)
            snprintf(fullname_t, sizeof(char)*(strlen(re_cmd)+strlen(filename)+2), "%s%s",re_cmd,filename);

        if(re_cmd == NULL)
            status=api_create_folder(fullname,serverpath);
        else
            status=api_create_folder(fullname_t,serverpath);
        if(status==0)
        {
            exist=parse_create_folder(Con(TMP_R,create_folder.txt));
            if(exist)
            {
#if 0
                char *newname;
                newname=change_server_same_name(serverpath,index);

                status = api_move(serverpath,newname,index,0,NULL);

                free(newname);
                if(status ==0)
                {
                    if(re_cmd == NULL)
                        status=api_create_folder(fullname,serverpath);
                    else
                        status=api_create_folder(fullname_t,serverpath);
                }
#endif
            }
            if(re_cmd)
                free(fullname_t);
            free(serverpath);
            free(fullname);
        }
        else
        {

            wd_DEBUG("createFolder failed status = %d\n",status);

            free(path);
            return status;
        }
    }
    free(path);
    return 0;

}

int deal_dragfolder_to_socketlist(char *dir,int index)
{
    wd_DEBUG("dir = %s\n",dir);

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
            fullname = (char *)malloc(sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2));
            memset(fullname,'\0',strlen(dir)+strlen(ent->d_name)+2);

            snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);
            if(test_if_dir(fullname) == 1)
            {
                snprintf(info, 512, "createfolder%s%s%s%s",CMD_SPLIT,dir,CMD_SPLIT,ent->d_name);
                pthread_mutex_lock(&mutex_socket);
                add_socket_item(info);
                pthread_mutex_unlock(&mutex_socket);
                status = deal_dragfolder_to_socketlist(fullname,index);
            }
            else
            {
                snprintf(info, 512, "createfile%s%s%s%s",CMD_SPLIT,dir,CMD_SPLIT,ent->d_name);
                pthread_mutex_lock(&mutex_socket);
                add_socket_item(info);
                pthread_mutex_unlock(&mutex_socket);
            }
            free(fullname);
        }
        closedir(pDir);
        return 0;
    }
}
int dragfolder_rename(char *dir,int index,time_t server_mtime)
{
    wd_DEBUG("change dir = %s mtime\n",dir);

    int status;
    struct dirent *ent = NULL;
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
            fullname = (char *)malloc(sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2));
            memset(fullname,'\0',strlen(dir)+strlen(ent->d_name)+2);

            snprintf(fullname,sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);
            if(test_if_dir(fullname) == 1)
            {

                status = dragfolder_rename(fullname,index,server_mtime);

            }
            else
            {
                status=ChangeFile_modtime(fullname,server_mtime);
            }
            free(fullname);
        }
        closedir(pDir);
        return 0;
    }
}
int dragfolder_old_dir(char *dir,int index,char *old_dir)
{

    wd_DEBUG("dir = %s\n",dir);

    int status;
    int exist;
    struct dirent *ent = NULL;
    DIR *pDir;
    pDir=opendir(dir);
    if(pDir != NULL)
    {
        status=api_create_folder(dir,old_dir);
        if(status != 0)
        {
            wd_DEBUG("Create %s failed\n",old_dir);

            closedir(pDir);
            return status;
        }
        exist=parse_create_folder(Con(TMP_R,create_folder.txt));
        if(exist)
        {
            cJSON *json = dofile(Con(TMP_R,create_folder.txt));
            int cf_error_flag=cJSON_printf_error(json,"error_summary");
            cJSON_Delete(json);
            if(3 == cf_error_flag)
            {
                char *newname;
                newname=change_server_same_name(old_dir,index);

                status = api_move(old_dir,newname,index,0,NULL);

                free(newname);
                if(status ==0)
                {
                    status=api_create_folder(dir,old_dir);
                }
                if(status != 0)
                {
                    closedir(pDir);
                    return status;
                }
            }
        }

        while((ent=readdir(pDir)) != NULL)
        {
            if(ent->d_name[0] == '.')
                continue;
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;
            char *fullname;
            fullname = (char *)malloc(sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2));
            memset(fullname,'\0',strlen(dir)+strlen(ent->d_name)+2);
            snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);

            char *fullname_r;
            fullname_r = (char *)malloc(sizeof(char)*(strlen(old_dir)+strlen(ent->d_name)+2));
            memset(fullname_r,'\0',strlen(old_dir)+strlen(ent->d_name)+2);
            snprintf(fullname_r, sizeof(char)*(strlen(old_dir)+strlen(ent->d_name)+2), "%s/%s",old_dir,ent->d_name);

            status = check_localpath_is_socket(index,dir,ent->d_name,fullname);
            if(status == 1)
            {
                wd_DEBUG("the %s is socket ,so do nothing\n",fullname);
                free(fullname);
                free(fullname_r);
                continue;
            }

            if(test_if_dir(fullname) == 1)
            {

                status = dragfolder_old_dir(fullname,index,fullname_r);
                if(status != 0)
                {

                    wd_DEBUG("CreateFolder %s failed\n",fullname);

                    free(fullname);
                    free(fullname_r);
                    closedir(pDir);
                    return status;
                }
            }
            else
            {
                char *serverpath_1=localpath_to_serverpath(fullname_r,index);
                status=upload_file(fullname,serverpath_1,1,index);
                if(status == 0)
                {
                    cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                    cJSON_Delete(json);
                    ChangeFile_modtime(fullname,mtime);

                }
                else if(status == SERVER_SPACE_NOT_ENOUGH)
                {

                    wd_DEBUG("upload %s failed,server space is not enough!\n",fullname);

                }
                else if(status == LOCAL_FILE_LOST)
                {

                    wd_DEBUG("upload %s failed,local file lost!\n",fullname);

                }
                else
                {

                    wd_DEBUG("upload %s failed\n",fullname);

                    free(serverpath_1);
                    free(fullname);
                    free(fullname_r);
                    return status;
                }
            }
            free(fullname);
            free(fullname_r);
        }
        closedir(pDir);
        return 0;
    }
}
int dragfolder(char *dir,int index)
{

    wd_DEBUG("dir = %s\n",dir);

    int status;
    int exist;
    struct dirent *ent = NULL;
    DIR *pDir;
    pDir=opendir(dir);
    if(pDir != NULL)
    {
        char *serverpath=localpath_to_serverpath(dir,index);
        status=api_create_folder(dir,serverpath);
        if(status != 0)
        {
            wd_DEBUG("Create %s failed\n",serverpath);

            closedir(pDir);
            return status;
        }
        exist=parse_create_folder(Con(TMP_R,create_folder.txt));
        if(exist)
        {
            cJSON *json = dofile(Con(TMP_R,create_folder.txt));
            int cf_error_flag=cJSON_printf_error(json,"error_summary");
            cJSON_Delete(json);
            if(3 == cf_error_flag)
            {
                char *newname;
                newname=change_server_same_name(serverpath,index);

            status = api_move(serverpath,newname,index,0,NULL);

                free(newname);
                if(status ==0)
                {
                    status=api_create_folder(dir,serverpath);
                }
                if(status != 0)
                {
                    closedir(pDir);
                    return status;
                }
            }
        }
        free(serverpath);
        while((ent=readdir(pDir)) != NULL)
        {
            if(ent->d_name[0] == '.')
                continue;
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;
            char *fullname;
            fullname = (char *)malloc(sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2));
            memset(fullname,'\0',strlen(dir)+strlen(ent->d_name)+2);

            snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);

            /*
             fix :below question
             a.rename local path A,and server is not exist;
             b.create a file 'a' in A/;
             final-->the server will exist 'a' and 'a(1)';
            */
            status = check_localpath_is_socket(index,dir,ent->d_name,fullname);
            if(status == 1)
            {
                wd_DEBUG("the %s is socket ,so do nothing\n",fullname);
                free(fullname);
                continue;
            }
            if(test_if_dir(fullname) == 1)
            {

                status = dragfolder(fullname,index);
                if(status != 0)
                {

                    wd_DEBUG("CreateFolder %s failed\n",fullname);

                    free(fullname);
                    closedir(pDir);
                    return status;
                }
            }
            else
            {
                char *serverpath_1=localpath_to_serverpath(fullname,index);
                status=upload_file(fullname,serverpath_1,1,index);
                if(status == 0)
                {
                    cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
                    cJSON_Delete(json);
                    ChangeFile_modtime(fullname,mtime);

                }
                else if(status == SERVER_SPACE_NOT_ENOUGH)
                {

                    wd_DEBUG("upload %s failed,server space is not enough!\n",fullname);

                }
                else if(status == LOCAL_FILE_LOST)
                {

                    wd_DEBUG("upload %s failed,local file lost!\n",fullname);

                }
                else
                {

                    wd_DEBUG("upload %s failed\n",fullname);

                    free(serverpath_1);
                    free(fullname);
                    return status;
                }
            }
            free(fullname);
        }
        closedir(pDir);
        return 0;
    }
}

int parse_create_folder(char *filename)
{
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp == NULL)
    {
        wd_DEBUG("filename %s is not exit\n",filename);
        return 0;
    }
    char tmp[256]="\0";
    fgets(tmp,256,fp);
    fclose(fp);
#if 0
    if(strstr(tmp,"error") != NULL)
    {
        if(strstr(tmp,"already exists.") != NULL)
        {
            return 1;
        }
    }
#endif
    if(strstr(tmp,"409") != NULL)
    {
        return 1;
    }
    return 0;

}

int get_path_to_index(char *path)
{
    int i;
    char *root_path = NULL;
    char *temp = NULL;
    root_path = my_str_malloc(512);

    temp = my_nstrchr('/',path,4);
    if(temp == NULL)
    {
        snprintf(root_path, sizeof(char)*512, "%s",path);
    }
    else
    {
        snprintf(root_path,strlen(path)-strlen(temp)+1,"%s",path);
    }

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(!strcmp(root_path,asus_cfg.prule[i]->base_path))
            break;
    }

    wd_DEBUG("get_path_to_index root_path = %s\n",root_path);

    free(root_path);

    return i;
}

void sig_handler (int signum)
{
    wd_DEBUG("signal is %d\n",signum);
    switch (signum)
    {
#if TOKENFILE
    case SIGTERM:case SIGUSR2:
        {
            int mountflag = 0;
            if(signum == SIGUSR2)
            {

                wd_DEBUG("signal is SIGUSR2\n");

                FILE *fp;
                fp = fopen("/proc/mounts","r");
                char a[10240];
                memset(a,'\0',sizeof(a));
                fread(a,10240,1,fp);
                fclose(fp);
                if(strstr(a,"/dev/sd"))
                {
                    mountflag = 1;
                }

            }
            if(signum == SIGTERM || mountflag == 0)
            {
                stop_progress = 1;
                exit_loop = 1;

                set_iptables(0);
#ifndef NVRAM_
                char cmd_p[128] = {0};
                snprintf(cmd_p, 128, "sh %s",DropBox_Get_Nvram);
                system(cmd_p);
                sleep(2);
                if(create_webdav_conf_file(&asus_cfg_stop) == -1)
                {
                    wd_DEBUG("create_dropbox_conf_file fail\n");
                    return;
                }

#else
                if(convert_nvram_to_file_mutidir(CONFIG_PATH,&asus_cfg_stop) == -1)
                {
                    wd_DEBUG("convert_nvram_to_file fail\n");
                    write_to_nvram("","db_tokenfile");
                    return;
                }
#endif
                if(asus_cfg_stop.dir_number == 0)
                {
                    char *filename;
                    filename = my_str_malloc(strlen(asus_cfg.prule[0]->base_path)+20+1);
                    snprintf(filename, sizeof(char)*(strlen(asus_cfg.prule[0]->base_path)+20+1), "%s/.dropbox_tokenfile",asus_cfg.prule[0]->base_path);
                    remove(filename);
                    free(filename);

                    remove(g_pSyncList[0]->conflict_file);

#ifdef NVRAM_
                    write_to_nvram("","db_tokenfile");
#else
                    write_to_wd_tokenfile("");
#endif

                }
                else
                {
                    if(asus_cfg_stop.dir_number != asus_cfg.dir_number)
                    {
                        parse_config_mutidir(CONFIG_PATH,&asus_cfg_stop);

                        rewrite_tokenfile_and_nv();
                    }
                }
                sighandler_finished = 1;
                pthread_cond_signal(&cond);
                pthread_cond_signal(&cond_socket);
                pthread_cond_signal(&cond_log);
            }
            else
            {
                disk_change = 1;
                sighandler_finished = 1;
            }
            break;
        }
#else
    case SIGTERM:
        {
            wd_DEBUG("signal is SIGTERM\n");
            stop_progress = 1;
            exit_loop = 1;

            pthread_cond_signal(&cond);
            pthread_cond_signal(&cond_socket);
            pthread_cond_signal(&cond_log);

            break;
        }
#endif
    case SIGPIPE:  // delete user
        wd_DEBUG("signal is SIGPIPE\n");
        signal(SIGPIPE, SIG_IGN);
        break;

    default:
        break;
    }
}

void* sigmgr_thread(){
    sigset_t   waitset;
    int        sig;
    int        rc;
    pthread_t  ppid = pthread_self();

    pthread_detach(ppid);

    sigemptyset(&waitset);
    sigaddset(&waitset,SIGUSR1);
    sigaddset(&waitset,SIGUSR2);
    sigaddset(&waitset,SIGTERM);
    sigaddset(&waitset,SIGPIPE);

    while (1)  {
        rc = sigwait(&waitset, &sig);
        if (rc != -1) {

            wd_DEBUG("sigwait() fetch the signal - %d\n", sig);

            sig_handler(sig);
        } else {
            wd_DEBUG("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
        }
    }
}
void stop_process_clean_up(){
#if TOKENFILE
    unlink("/tmp/notify/usb/dropbox_client");
#endif
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond_socket);
    pthread_cond_destroy(&cond_log);

    if(!access_token_expired)
        unlink(general_log);
}
char *change_local_same_file(char *oldname,int index)
{
    cJSON *json;
    char *newname;
    if (access(Con(TMP_R,upload_chunk_commit.txt),R_OK) ==0)
    {
        json=dofile(Con(TMP_R,upload_chunk_commit.txt));
        if(json)
        {
            char *localpath;
            newname=cJSON_parse_name(json);
            localpath=serverpath_to_localpath(newname,index);
            if(strcmp(newname,oldname)!=0)
            {
#ifndef TEST
                add_action_item("rename",localpath,g_pSyncList[index]->server_action_list);
                rename(oldname,localpath);

                char *err_msg = write_error_message("server has exist %s file ,rename local %s to %s",oldname,oldname,localpath);
                write_trans_excep_log(oldname,3,err_msg);
                free(err_msg);

#endif
                cJSON_Delete(json);
                return localpath;
            }
            else
            {
                cJSON_Delete(json);
                return localpath;
            }
        }
        else
        {
            /*the file contents is error*/
            return NULL;
        }
    }


}

void check_tokenfile()
{
        if(get_tokenfile_info()==-1)
        {
            wd_DEBUG("\nget_tokenfile_info failed\n");
            exit(-1);
        }
        check_config_path(1);
}
int check_link_internet()
{
    struct timeval now;
    struct timespec outtime;
    int link_flag = 0;
    int i;
#if defined NVRAM_
    while(!link_flag && !exit_loop)
    {
#ifndef USE_TCAPI
        char *link_internet = strdup(nvram_safe_get("link_internet"));
#else
        char tmp[MAXLEN_TCAPI_MSG] = {0};
        tcapi_get(WANDUCK, "link_internet", tmp);
        char *link_internet = my_str_malloc(strlen(tmp)+1);
        snprintf(link_internet, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif
        link_flag = atoi(link_internet);
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
        free(link_internet);
    }
#else

    do{
        char cmd_p[128] ={0};
        snprintf(cmd_p, sizeof(char)*128, "sh %s",DropBox_Get_Nvram_Link);
        system(cmd_p);
        sleep(2);

        char nv[64] = {0};
        FILE *fp;
        fp = fopen(TMP_NVRAM_VL,"r");
        fgets(nv,sizeof(nv),fp);
        fclose(fp);

        link_flag = atoi(nv);
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

    }while(!link_flag && !exit_loop);
#endif

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        write_log(S_SYNC,"","",i);
    }
}
#ifdef OAuth1
int do_auth()
{
    int curl_res=0;
    int auth_ok=0;
    int error_time = 0;
    int have_error_log=0;
    int i;
    do
    {
        wd_DEBUG("##########begin auth############\n");

        if(error_time > 5)
        {
            check_link_internet();
            error_time = 0;
        }
        if(exit_loop)
            return auth_ok;
        curl_res = get_request_token();

        if(curl_res == -1)
        {
            error_time++;
            continue;
        }
        curl_res = open_login_page_first();
        if(curl_res == -1)
        {
            error_time++;
            continue;
        }
        curl_res = login_first();
        if(curl_res == -1)
        {
            error_time++;
            continue;
        }
        curl_res = login_second();
        if(curl_res == -1)
        {
            error_time++;
            continue;
        }
        curl_res = login_second_submit();
        if(curl_res == -1)
        {
            error_time++;
            continue;
        }

        curl_res=get_access_token();
        if(curl_res == -1 || curl_res > 0)
        {
            if(curl_res == -1)
            {
                for(i=0;i<asus_cfg.dir_number;i++)
                {
                    write_log(S_ERROR,"Authentication Failed","",i);
                }

                return auth_ok;
            }
            else
                error_time++;
            continue;
        }
        auth_ok=1;
    }while(auth_ok != 1 && exit_loop != 1);
    wd_DEBUG("##########auth over############\n");

    return auth_ok;
}
#endif
int f_exists(const char *path)	// note: anything but a directory
{
        struct stat st;
        return (stat(path, &st) == 0) && (!S_ISDIR(st.st_mode));
}
void clean_sys_data()
{
    if(f_exists("/tmp/smartsync/.logs/dropbox"))
        unlink("/tmp/smartsync/.logs/dropbox");
}

void db_disbale_access_token()
{
    my_free(auth->oauth_token);
    my_free(auth);
}

int main(int args,char *argc[])
{
#ifndef TEST
    clean_sys_data();
    int auth_flag = 0;
    sigset_t bset,oset;
    pthread_t sig_thread;

    curl_global_init(CURL_GLOBAL_ALL);
    sigemptyset(&bset);
    sigaddset(&bset,SIGUSR1);
    sigaddset(&bset,SIGUSR2);
    sigaddset(&bset,SIGTERM);
    sigaddset(&bset,SIGPIPE);

    if( pthread_sigmask(SIG_BLOCK,&bset,&oset) == -1)
        wd_DEBUG("!! Set pthread mask failed\n");

    if( pthread_create(&sig_thread,NULL,(void *)sigmgr_thread,NULL) != 0)
    {
        wd_DEBUG("thread creation failder\n");
        exit(-1);
    }
    init_globar_var();

#if TOKENFILE
    write_notify_file(NOTIFY_PATH,SIGUSR2);
#ifndef NVRAM_
     my_mkdir("/opt/etc/.smartsync");
#ifndef WIN_32
    write_get_nvram_script_va("link_internet");
    write_get_nvram_script();
    char cmd_p[128] ={0};
    snprintf(cmd_p, 128, "sh %s",DropBox_Get_Nvram);
    system(cmd_p);
    sleep(2);
#endif
#else
    create_shell_file();
#endif
#endif

    auth=(Auth *)malloc(sizeof(Auth));
    memset(auth,0,sizeof(auth));


    read_config();
    if(write_rootca() != 0)
        exit(-1);

#ifdef OAuth1
    auth_flag = do_auth();
#else
    auth_flag = 1;
    auth->oauth_token = my_str_malloc(strlen(asus_cfg.pwd)+1);
    snprintf(auth->oauth_token, sizeof(char)*(strlen(asus_cfg.pwd)+1), "%s",asus_cfg.pwd);
#endif

    if(auth_flag)
    {
#if TOKENFILE
        check_tokenfile();
#endif
        if(no_config == 0)
        {
            run();
        }

        pthread_join(sig_thread,NULL);
        stop_process_clean_up();
    }
    db_disbale_access_token();
    curl_global_cleanup();

#else
    upload_file("/tmp/mnt/ASD/My410Sync/aicloud_1.0.0.10-0-gd8a47cb_mipsel.ipk",oauth_url_escape("/aicloud_1.0.0.10-0-gd8a47cb_mipsel.ipk"),0,0);
#endif
}
#ifdef OAuth1
int get_request_token(){

    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *header;
    static const char buf[]="Expect:";
    header=makeAuthorize(1);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
        curl_easy_setopt(curl,CURLOPT_URL,"https://api.dropbox.com/1/oauth/request_token");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        fp=fopen(Con(TMP_R,data_1.txt),"w");
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            wd_DEBUG("get_request_token [%d] failed!\n",res);
            return -1;
        }
    }
    free(header);

    if(parse(Con(TMP_R,data_1.txt),1) == -1)
        return -1;
    return 0;
}

int cgi_init(char *query)
{
    int len, nel;
    char *q, *name, *value;
    /* Parse into individualassignments */
    q = query;
    len = strlen(query);
    nel = 1;
    while (strsep(&q, "&"))
        nel++;
    for (q = query; q< (query + len);) {
        value = name = q;
        /* Skip to next assignment */
        for (q += strlen(q); q < (query +len) && !*q; q++);
        /* Assign variable */
        name = strsep(&value,"=");
        if(strcmp(name,"oauth_token_secret")==0)
            snprintf(auth->oauth_token_secret, 32, "%s", value);
        else if(strcmp(name,"oauth_token")==0)
            snprintf(auth->oauth_token, 32, "%s", value);
        else if(strcmp(name,"uid")==0)
            auth->uid=atoi(value);

    }
    wd_DEBUG("CGI[value] :%s %s %d\n", auth->oauth_token_secret,auth->oauth_token,auth->uid);
    return 0;
}

int parse(char *filename,int flag)
{

    char *p=NULL;
    char p1[56];
    char *p3=NULL;
    char *p4=NULL;
    FILE *fp=fopen(filename,"r");
    char tmp_data[256]="\0";
    fgets(tmp_data,sizeof(tmp_data),fp);
    wd_DEBUG("%s\n",tmp_data);
    if(strstr(tmp_data,"error")==NULL && tmp_data != NULL&&strlen(tmp_data) > 0)
    {
        switch(flag)
        {
        case 1:
            p=strchr(tmp_data,'&');
            memset(p1,'\0',sizeof(p1));
            strncpy(p1,tmp_data,strlen(tmp_data)-strlen(p));
            p3=strchr(p,'=');
            p3++;
            snprintf(auth->tmp_oauth_token, 32, "%s", p3);
            p3=NULL;
            p4=strchr(p1,'=');
            p4++;
            snprintf(auth->tmp_oauth_token_secret,strlen(p4)+1,"%s",p4);
            wd_DEBUG("auth->tmp_oauth_token=%s\nauth->tmp_oauth_token_secret=%s\n",auth->tmp_oauth_token,auth->tmp_oauth_token_secret);
            break;
        case 2:
            cgi_init(tmp_data);
            break;
        default:
            break;
        }
        fclose(fp);
        return 0;
    }
    else
    {
        fclose(fp);
        return -1;
    }


}
#endif
char *makeAuthorize(int flag)
{
    char *header = NULL;
    char http_basic_authentication[] = "Authorization: Bearer ";
    char URI_Query_Parameter[] = "access_token=";
    int header_len ;
    switch(flag)
    {
    case 1:
        header_len = strlen(http_basic_authentication) + strlen(auth->oauth_token) +1;
        header = my_str_malloc(header_len);
        snprintf(header, sizeof(char)*header_len, "%s%s",http_basic_authentication,auth->oauth_token);
        break;
    case 2:
        header_len = strlen(URI_Query_Parameter) + strlen(auth->oauth_token) +1;
        header = my_str_malloc(header_len);
        snprintf(header, sizeof(char)*header_len, "%s%s",URI_Query_Parameter,auth->oauth_token);
        break;
    default:
        break;
    }
    wd_DEBUG("makeAuthorize>%s\n",header);
    return header;
}
#ifdef OAuth1
char *makeAuthorize(int flag)
{
    char *header = NULL;
    header = (char *)malloc(sizeof(char*)*1024);
    char header_signature_method[64];
    char header_timestamp[64];
    char header_nonce[64];
    char *header_signature = NULL;
    char prekey[128];
    long long int sec;

    char query_string[1024];
    char *incode_string = NULL;
    char *sha1_string = NULL;

    snprintf(header_signature_method,64,"%s","PLAINTEXT");
    if(flag==1)
        snprintf(prekey,128,"%s","5vq8jog8wgpx1p0&");
    else if(flag==2)
        snprintf(prekey,128,"%s%s","5vq8jog8wgpx1p0&",auth->tmp_oauth_token_secret);
    else if(flag==3)
    {
#ifndef TEST
        snprintf(prekey,128,"%s&%s","5vq8jog8wgpx1p0",auth->oauth_token_secret);
#else
        snprintf(prekey,128,"%s","5vq8jog8wgpx1p0&efghhkqrt68qta2");
#endif
    }
    else if(flag==4)
    {
#ifndef TEST
        snprintf(prekey,128,"%s&%s","5vq8jog8wgpx1p0",auth->oauth_token_secret);
#else
        snprintf(prekey,128,"%s","5vq8jog8wgpx1p0&efghhkqrt68qta2");
#endif
    }
    sec = time((time_t *)NULL);
    snprintf(header_timestamp,64,"%lld",sec);
    snprintf(header_nonce,64,"%lld",sec);

    header_signature=oauth_sign_plaintext(NULL,prekey);

    if(flag==1)
        snprintf(header,1024,"Authorization:OAuth oauth_consumer_key=\"%s\",oauth_signature_method=\"%s\",oauth_signature=\"%s\",oauth_timestamp=\"%s\",oauth_nonce=\"%s\",oauth_version=\"1.0\""
                 ,"qah4ku73k3qmigj",header_signature_method,header_signature,header_timestamp,header_nonce);
    else if(flag==2)
    snprintf(header,1024,"Authorization:OAuth oauth_consumer_key=\"%s\",oauth_token=\"%s\",oauth_signature_method=\"%s\",oauth_signature=\"%s\",oauth_timestamp=\"%s\",oauth_nonce=\"%s\",oauth_version=\"1.0\""
             ,"qah4ku73k3qmigj",auth->tmp_oauth_token,header_signature_method,header_signature,header_timestamp,header_nonce);
    else if(flag==3)
    {
#ifndef TEST
        snprintf(header,1024,"Authorization:OAuth oauth_consumer_key=\"%s\",oauth_token=\"%s\",oauth_signature_method=\"%s\",oauth_signature=\"%s\",oauth_timestamp=\"%s\",oauth_nonce=\"%s\",oauth_version=\"1.0\""
                 ,"qah4ku73k3qmigj",auth->oauth_token,header_signature_method,header_signature,header_timestamp,header_nonce);
#else
        snprintf(header,1024,"Authorization:OAuth oauth_consumer_key=\"%s\",oauth_token=\"%s\",oauth_signature_method=\"%s\",oauth_signature=\"%s\",oauth_timestamp=\"%s\",oauth_nonce=\"%s\",oauth_version=\"1.0\""
                 ,"qah4ku73k3qmigj","8m9knhps2zgaon9",header_signature_method,header_signature,header_timestamp,header_nonce);
#endif
    }
    else if(flag==4)
    {
#ifndef TEST
        snprintf(header,1024,"oauth_consumer_key=%s&oauth_token=%s&oauth_signature_method=%s&oauth_signature=%s&oauth_timestamp=%s&oauth_nonce=%s&oauth_version=1.0"
                 ,"qah4ku73k3qmigj",auth->oauth_token,header_signature_method,header_signature,header_timestamp,header_nonce);
#else
        snprintf(header,1024,"oauth_consumer_key=%s&oauth_token=%s&oauth_signature_method=%s&oauth_signature=%s&oauth_timestamp=%s&oauth_nonce=%s&oauth_version=1.0"
                 ,"qah4ku73k3qmigj","8m9knhps2zgaon9",header_signature_method,header_signature,header_timestamp,header_nonce);
#endif
    }

    free(header_signature);
    return header;
}
#endif
Browse *browseFolder(char *URL){
    int status;
    int i=0;

    Browse *browse = getb(Browse);
    if( NULL == browse )
    {
        wd_DEBUG("create memery error\n");
        exit(-1);
    }
    memset(browse,0,sizeof(Browse));

    TreeFolderList = (CloudFile *)malloc(sizeof(CloudFile));
    memset(TreeFolderList,0,sizeof(CloudFile));
    TreeFileList = (CloudFile *)malloc(sizeof(CloudFile));
    memset(TreeFileList,0,sizeof(CloudFile));

    TreeFolderList->href = NULL;
    TreeFileList->href = NULL;

    TreeFolderTail = TreeFolderList;
    TreeFileTail = TreeFileList;
    TreeFolderTail->next = NULL;
    TreeFileTail->next = NULL;
    int has_more=0;
    char *cursor = NULL;
    status = api_list_folder(URL,dofile, &has_more, &cursor);
    if(status != 0)
    {
        free_CloudFile_item(TreeFolderList);
        free_CloudFile_item(TreeFileList);
        TreeFolderList = NULL;
        TreeFileList = NULL;
        free(browse);
        if(cursor)
        {
            free(cursor);
            cursor=NULL;
        }
        return NULL;
    }
    else
    {
        while(has_more && cursor)
        {
            has_more=0;
            api_list_continue(dofile, &has_more, &cursor);
        }
    }
    if(cursor)
    {
        free(cursor);
        cursor=NULL;
    }

    browse->filelist = TreeFileList;
    browse->folderlist = TreeFolderList;

    CloudFile *de_foldercurrent,*de_filecurrent;
    de_foldercurrent = TreeFolderList->next;
    de_filecurrent = TreeFileList->next;
    while(de_foldercurrent != NULL){
        ++i;
        de_foldercurrent = de_foldercurrent->next;
    }
    browse->foldernumber = i;
    i = 0;
    while(de_filecurrent != NULL){
        ++i;
        de_filecurrent = de_filecurrent->next;
    }
    browse->filenumber = i;
    return browse;
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

queue_entry_t  queue_dequeue_t (queue_t q)
{
    queue_entry_t first = q->head;

    if (first)
    {
        if(first->re_cmd == NULL)
        {
            q->head = first->next_ptr;
            if (q->head == NULL)
            {
                q->tail = NULL;
            }
            first->next_ptr = NULL;
        }
        else
        {
            if(access(first->re_cmd,0) == 0)
            {
                q->head = first->next_ptr;
                if (q->head == NULL)
                {
                    q->tail = NULL;
                }
                first->next_ptr = NULL;
            }
            else
            {
                if(first->is_first == 0)
                {
                    first->is_first++;
                    return NULL;
                }
                else
                {
                    q->head = first->next_ptr;
                    if (q->head == NULL)
                    {
                        q->tail = NULL;
                    }
                    first->next_ptr = NULL;
                }
            }
        }

    }
    return first;
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

#if MEM_POOL_ENABLE
            mem_free(next->cmd_name);
            mem_free(next->re_cmd);
            mem_free (next);
#else
            if(next->re_cmd)
                free(next->re_cmd);
            free(next->cmd_name);
            free (next);
#endif
        }
        q->head = q->tail = NULL;
        free (q);
    }
}
action_item *get_action_item_access(const char *action,const char *path,action_item *head,int index){

    action_item *p;
    p = head->next;

    while(p != NULL)
    {
        if(asus_cfg.prule[index]->rule == 1)
        {

        }
        else
        {
            if(!strcmp(p->action,action) && !strncmp(p->path,path,strlen(path)))
            {
                return p;
            }
        }
        p = p->next;
    }
    wd_DEBUG("can not find action item_1\n");
    return NULL;
}
action_item *check_action_item(const char *action,const char *oldpath,action_item *head,int index,const char *newpath)
{
    action_item *p;
    p = head->next;

    while(p != NULL)
    {
        char *pTemp_t=(char *)malloc(sizeof(char)*(strlen(p->path)+1+1));
        char *oldpath_t=(char *)malloc(sizeof(char)*(strlen(oldpath)+1+1));
        memset(pTemp_t,'\0',strlen(p->path)+1+1);
        memset(oldpath_t,'\0',strlen(oldpath)+1+1);
        snprintf(pTemp_t, sizeof(char)*(strlen(p->path)+1+1), "%s/",p->path);
        snprintf(oldpath_t, sizeof(char)*(strlen(oldpath)+1+1), "%s/",oldpath);
        char *p_t = NULL;

        if(!strcmp(p->action,action) && (p_t=strstr(pTemp_t,oldpath_t)) != NULL)
        {
            if(strlen(oldpath_t)<strlen(pTemp_t))
            {
                p_t+=strlen(oldpath);
                char *p_tt=(char *)malloc(sizeof(char)*(strlen(p_t)+1));
                memset(p_tt,'\0',strlen(p_t)+1);
                snprintf(p_tt, sizeof(char)*(strlen(p_t)+1), "%s",p_t);
                free(p->path);
                p->path = (char *)malloc(sizeof(char) * (strlen(newpath)+strlen(p_tt) + 1));
                memset(p->path,'\0',strlen(newpath)+strlen(p_tt) + 1);
                snprintf(p->path, sizeof(char) * (strlen(newpath)+strlen(p_tt) + 1), "%s%s",newpath,p_tt);
                free(p_tt);
            }
            else
            {
                free(p->path);
                p->path = (char *)malloc(sizeof(char) * (strlen(newpath) + 1));
                snprintf(p->path, sizeof(char) * (strlen(newpath) + 1), "%s",newpath);
            }
        }

        free(pTemp_t);
        free(oldpath_t);
        p = p->next;
    }
    return NULL;
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
    wd_DEBUG("can not find action item_2\n");
    return NULL;
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
    wd_DEBUG("can not find action item_3\n");
    return 1;
}
int add_action_item(const char *action,const char *path,action_item *head){

    wd_DEBUG("add_action_item,action = %s,path = %s\n",action,path);

    action_item *p1,*p2;

    p1 = head;

    p2 = (action_item *)malloc(sizeof(action_item));
    memset(p2,'\0',sizeof(action_item));
    p2->action = (char *)malloc(sizeof(char)*(strlen(action)+1));
    p2->path = (char *)malloc(sizeof(char)*(strlen(path)+1));
    memset(p2->action,'\0',strlen(action)+1);
    memset(p2->path,'\0',strlen(path)+1);

    snprintf(p2->action, sizeof(char)*(strlen(action)+1), "%s",action);
    snprintf(p2->path, sizeof(char)*(strlen(path)+1), "%s",path);

    while(p1->next != NULL)
        p1 = p1->next;

    p1->next = p2;
    p2->next = NULL;

    wd_DEBUG("add action item OK!\n");

    return 0;
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
        wd_DEBUG("create memory error!\n");
        exit(-1);
    }
    memset(head,'\0',sizeof(action_item));
    head->next = NULL;

    return head;
}
int add_all_download_only_socket_list(char *cmd,const char *dir,int index)
{
    struct dirent* ent = NULL;
    char *fullname;
    int fail_flag = 0;

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {

        wd_DEBUG("opendir %s fail",dir);

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

        wd_DEBUG("opendir %s fail",dir);

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
            add_action_item("createfolder",fullname,g_pSyncList[index]->download_only_socket_head);
            add_all_download_only_dragfolder_socket_list(fullname,index);
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
int do_unfinished(int index){
    if(exit_loop)
    {
        return 0;
    }

    wd_DEBUG("*************do_unfinished*****************\n");

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

                wd_DEBUG("filetmp is NULL\n");

                p1 = p->next;
                del_action_item("download",p->path,g_pSyncList[index]->unfinished_list);
                p = p1;
                continue;
            }
            char *localpath;
            localpath = serverpath_to_localpath(filetmp->href,index);

            wd_DEBUG("localpath = %s\n",localpath);

            if(is_local_space_enough(filetmp,index))
            {
                add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                ret = api_download(localpath,filetmp->href,index);
                if (ret == 0)
                {
                    ChangeFile_modtime(localpath,filetmp->mtime);
                    p1 = p->next;
                    del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    p = p1;
                }
                else
                {

                    wd_DEBUG("download %s failed",filetmp->href);

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
        else
        {
            p = p->next;
        }
    }

    p = g_pSyncList[index]->up_space_not_enough_list->next;
    while(p != NULL)
    {
        p1 = p->next;

        wd_DEBUG("up_space_not_enough_list\n");

        char *serverpath;
        serverpath = localpath_to_serverpath(p->path,index);
        char *path_temp;
        path_temp = my_str_malloc(strlen(p->path)+1);
        snprintf(path_temp, sizeof(char)*(strlen(p->path)+1), "%s",p->path);
        ret = upload_file(p->path,serverpath,1,index);

        wd_DEBUG("########### uploadret = %d\n",ret);

        if(ret == 0)
        {
            cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
            time_t mtime=cJSON_printf(json,"server_modified", NULL, NULL);
            cJSON_Delete(json);
            ChangeFile_modtime(p->path,mtime);

            del_action_item("upload",p->path,g_pSyncList[index]->unfinished_list);
            del_action_item("upload",p->path,g_pSyncList[index]->up_space_not_enough_list);
            p = p1;
        }
        else if(ret == LOCAL_FILE_LOST)
        {
            del_action_item("upload",p->path,g_pSyncList[index]->up_space_not_enough_list);
            p = p1;
        }
        else
        {
            wd_DEBUG("upload %s failed",p->path);

            p = p->next;
        }
        free(serverpath);
        free(path_temp);
    }
    wd_DEBUG("*************do_unfinished ok*****************\n");
    return 0;
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
        ret = api_get_userspace();
        if(ret==0)
        {
            cJSON *json=dofile(Con(TMP_R,data_3.txt));
            cJSON_printf(json,"used", NULL, NULL);
            if(json)
                cJSON_Delete(json);
        }

    }
    long long int totalspace = server_allocated;

    mount_path_length = asus_cfg.prule[index]->base_path_len;

    memset(&log_s,0,LOG_SIZE);

    log_s.status = status;

    fp = fopen(general_log,"w");

    if(fp == NULL)
    {

        wd_DEBUG("open %s error\n",general_log);

        pthread_mutex_unlock(&mutex_log);
        return -1;
    }

    if(log_s.status == S_ERROR)
    {

        wd_DEBUG("******** status is ERROR *******\n");

        snprintf(log_s.error, 512, "%s", message);
        fprintf(fp,"STATUS:%d\nERR_MSG:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,log_s.error,totalspace,server_used,index);
    }
    else if(log_s.status == S_DOWNLOAD)
    {

        wd_DEBUG("******** status is DOWNLOAD *******\n");

        snprintf(log_s.path, 512, "%s", filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,asus_cfg.prule[index]->base_path,log_s.path+mount_path_length,totalspace,server_used,index);
    }
    else if(log_s.status == S_UPLOAD)
    {

        wd_DEBUG("******** status is UPLOAD *******\n");

        snprintf(log_s.path, 512, "%s", filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,asus_cfg.prule[index]->base_path,log_s.path+mount_path_length,totalspace,server_used,index);
    }
    else
    {
        if (log_s.status == S_INITIAL)
            wd_DEBUG("******** other status is INIT *******\n");
        else
            wd_DEBUG("******** other status is SYNC *******\n");

        fprintf(fp,"STATUS:%d\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,totalspace,server_used,index);
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
/*SIG*/
#if TOKENFILE
int write_notify_file(char *path,int signal_num)
{
    FILE *fp;
    char fullname[64];
    memset(fullname,0,sizeof(fullname));

    my_mkdir("/tmp/notify");
    my_mkdir("/tmp/notify/usb");
    snprintf(fullname, 64, "%s/dropbox_client",path);
    fp = fopen(fullname,"w");
    if(NULL == fp)
    {
        wd_DEBUG("open notify %s file fail\n",fullname);
        return -1;
    }
    fprintf(fp,"%d",signal_num);
    fclose(fp);
    return 0;
}
int get_tokenfile_info()
{
    wd_DEBUG("2222\n");
    int i;
    int j = 0;
    struct mounts_info_tag *info = NULL;
    char *tokenfile;
    FILE *fp;
    char buffer[1024];
    char *p;

    wd_DEBUG("12222\n");
    tokenfile_info = tokenfile_info_start;
    wd_DEBUG("42222\n");
    info = (struct mounts_info_tag *)malloc(sizeof(struct mounts_info_tag));
    if(info == NULL)
    {
        wd_DEBUG("obtain memory space fail\n");
        return -1;
    }
    wd_DEBUG("32222\n");
    memset(info,0,sizeof(struct mounts_info_tag));
    memset(buffer,0,1024);

    if(get_mounts_info(info) == -1)
    {
        wd_DEBUG("get mounts info fail\n");
        return -1;
    }
    wd_DEBUG("111\n");
    for(i=0;i<info->num;i++)
    {
        tokenfile = my_str_malloc(strlen(info->paths[i])+20+1);
        snprintf(tokenfile, sizeof(char)*(strlen(info->paths[i])+20+1), "%s/.dropbox_tokenfile",info->paths[i]);
        if(!access(tokenfile,F_OK))
        {
            wd_DEBUG("tokenfile is exist!\n");
            if((fp = fopen(tokenfile,"r"))==NULL)
            {
                fprintf(stderr,"read tokenfile error\n");
                exit(-1);
            }
            while(fgets(buffer,1024,fp)!=NULL)
            {
                if( buffer[ strlen(buffer)-1 ] == '\n' )
                    buffer[ strlen(buffer)-1 ] = '\0';
                p = buffer;
                if(j == 0)
                {
                    if(initial_tokenfile_info_data(&tokenfile_info_tmp) == NULL)
                    {
                        fclose(fp);
                        return -1;
                    }
                    tokenfile_info_tmp->url = my_str_malloc(strlen(p)+1);
                    snprintf(tokenfile_info_tmp->url, sizeof(char)*(strlen(p)+1), "%s",p);
                    tokenfile_info_tmp->mountpath = my_str_malloc(strlen(info->paths[i])+1);
                    snprintf(tokenfile_info_tmp->mountpath, sizeof(char)*(strlen(info->paths[i])+1), "%s",info->paths[i]);
                    j++;
                }
                else
                {
                    tokenfile_info_tmp->folder = my_str_malloc(strlen(p)+1);
                    snprintf(tokenfile_info_tmp->folder, sizeof(char)*(strlen(p)+1), "%s",p);
                    tokenfile_info->next = tokenfile_info_tmp;
                    tokenfile_info = tokenfile_info_tmp;
                    j = 0;
                }
            }
            fclose(fp);
        }
        free(tokenfile);
    }


    for(i=0;i<info->num;++i)
    {
        free(info->paths[i]);
    }
    if(info->paths != NULL)
        free(info->paths);
    free(info);

    return 0;
}
int get_mounts_info(struct mounts_info_tag *info)
{
    wd_DEBUG("get_mounts_info\n");
    int len = 0;
    FILE *fp;
    int i = 0;
    int num = 0;
    char **tmp_mount_list=NULL;
    char **tmp_mount=NULL;

    char buf[len+1];
    memset(buf,'\0',sizeof(buf));
    char a[1024];
    char *p,*q;
    fp = fopen("/proc/mounts","r");
    if(fp)
    {
        while(!feof(fp))
        {
            memset(a,'\0',sizeof(a));
            fscanf(fp,"%[^\n]%*c",a);
            if((strlen(a) != 0)&&((p=strstr(a,"/dev/sd")) != NULL))
            {
                if((q=strstr(p,"/tmp")) != NULL)
                {
                    if((p=strstr(q," ")) != NULL)
                    {

                        len = strlen(q) - strlen(p)+1;


                        tmp_mount = (char **)malloc(sizeof(char *)*(num+1));
                        if(tmp_mount == NULL)
                        {
                            fclose(fp);
                            return -1;
                        }

                        tmp_mount[num] = my_str_malloc(len+1);
                        if(tmp_mount[num] == NULL)
                        {
                            free(tmp_mount);
                            fclose(fp);
                            return -1;
                        }
                        snprintf(tmp_mount[num],len,"%s",q);

                        if(num != 0)
                        {
                            for(i = 0; i < num; ++i)
                                tmp_mount[i] = tmp_mount_list[i];

                            free(tmp_mount_list);
                            tmp_mount_list = tmp_mount;
                        }
                        else
                            tmp_mount_list = tmp_mount;

                        ++num;

                    }
                }
            }
        }
    }
    fclose(fp);

    info->paths = tmp_mount_list;
    info->num = num;
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
int check_config_path(int is_read_config)
{
    wd_DEBUG("check_config_path start\n");
    int i;
    int flag;
    char *nv;
    char *nvp;
    char *new_nv;
    int nv_len;
    int is_path_change = 0;

#ifdef NVRAM_
#ifndef USE_TCAPI
    nv = strdup(nvram_safe_get("db_tokenfile"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "db_tokenfile", tmp);
    nv = my_str_malloc(strlen(tmp)+1);
    snprintf(nv, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif
#else
    FILE *fp;
    fp = fopen(TOKENFILE_RECORD,"r");
    if(fp==NULL)
    {
        nv = my_str_malloc(2);
        snprintf(nv, sizeof(char)*2, "%s", "");
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

    wd_DEBUG("nv_len = %d\n",nv_len);
    wd_DEBUG("nv = %s\n",nv);

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        flag = 0;
        /*tokenfile exist*/
        tokenfile_info_tmp = tokenfile_info_start->next;
        while(tokenfile_info_tmp != NULL)
        {
            if( !strcmp(tokenfile_info_tmp->url,asus_cfg.user) &&
                !strcmp(tokenfile_info_tmp->folder,asus_cfg.prule[i]->rooturl))
            {
                if(strcmp(tokenfile_info_tmp->mountpath,asus_cfg.prule[i]->base_path))
                {
                    memset(asus_cfg.prule[i]->base_path,0,sizeof(asus_cfg.prule[i]->base_path));
                    snprintf(asus_cfg.prule[i]->base_path, 512, "%s",tokenfile_info_tmp->mountpath);
                    memset(asus_cfg.prule[i]->path,0,sizeof(asus_cfg.prule[i]->path));
                    snprintf(asus_cfg.prule[i]->path, 512, "%s%s",tokenfile_info_tmp->mountpath,tokenfile_info_tmp->folder);
                    asus_cfg.prule[i]->base_path_len = strlen(asus_cfg.prule[i]->base_path);
                    is_path_change = 1;
                }
                if(!is_read_config)
                {
                    if(g_pSyncList[i]->sync_disk_exist == 0)
                        is_path_change = 1;   //plug the disk and the mout_path not change
                }
                flag = 1;
                break;
            }
            tokenfile_info_tmp = tokenfile_info_tmp->next;
        }
        wd_DEBUG("asus_cfg.prule[%d]->path = %s\n",i,asus_cfg.prule[i]->path);
        if(!flag)
        {
            nvp = my_str_malloc(strlen(asus_cfg.user)+strlen(asus_cfg.prule[i]->rooturl)+2);
            snprintf(nvp, sizeof(char)*(strlen(asus_cfg.user)+strlen(asus_cfg.prule[i]->rooturl)+2), "%s>%s",asus_cfg.user,asus_cfg.prule[i]->rooturl);

            wd_DEBUG("nvp = %s\n",nvp);

            if(!is_read_config)
            {
                if(g_pSyncList[i]->sync_disk_exist == 1)
                    is_path_change = 2;   //remove the disk and the mout_path not change
            }

            /* "name>folder" is not in tokenfile_record */
            if(strstr(nv,nvp) == NULL)
            {
                if(initial_tokenfile_info_data(&tokenfile_info_tmp) == NULL)
                {
                    return -1;
                }
                tokenfile_info_tmp->url = my_str_malloc(strlen(asus_cfg.user)+1);
                snprintf(tokenfile_info_tmp->url, sizeof(char)*(strlen(asus_cfg.user)+1), "%s",asus_cfg.user);

                tokenfile_info_tmp->mountpath = my_str_malloc(strlen(asus_cfg.prule[i]->base_path)+1);
                snprintf(tokenfile_info_tmp->mountpath, sizeof(char)*(strlen(asus_cfg.prule[i]->base_path)+1), "%s",asus_cfg.prule[i]->base_path);

                tokenfile_info_tmp->folder = my_str_malloc(strlen(asus_cfg.prule[i]->rooturl)+1);
                snprintf(tokenfile_info_tmp->folder, sizeof(char)*(strlen(asus_cfg.prule[i]->rooturl)+1), "%s",asus_cfg.prule[i]->rooturl);

                tokenfile_info->next = tokenfile_info_tmp;
                tokenfile_info = tokenfile_info_tmp;

                write_to_tokenfile(asus_cfg.prule[i]->base_path);

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
                wd_DEBUG("new_nv = %s\n",new_nv);

#ifdef NVRAM_
                write_to_nvram(new_nv,"db_tokenfile");
#else
                write_to_wd_tokenfile(new_nv);
#endif

                free(new_nv);
            }
            free(nvp);
        }
    }
    free(nv);
    return is_path_change;
}
int write_to_tokenfile(char *mpath)
{
    wd_DEBUG("write_to_tokenfile start\n");
    FILE *fp;

    char *filename;
    filename = my_str_malloc(strlen(mpath)+20+1);
    snprintf(filename, sizeof(char)*(strlen(mpath)+20+1), "%s/.dropbox_tokenfile",mpath);
    wd_DEBUG("filename = %s\n",filename);

    int i = 0;
    if( ( fp = fopen(filename,"w") ) == NULL )
    {
        wd_DEBUG("open tokenfile failed!\n");
        return -1;
    }

    tokenfile_info_tmp = tokenfile_info_start->next;
    while(tokenfile_info_tmp != NULL)
    {
        wd_DEBUG("tokenfile_info_tmp->mountpath = %s\n",tokenfile_info_tmp->mountpath);
        if(!strcmp(tokenfile_info_tmp->mountpath,mpath))
        {
            wd_DEBUG("tokenfile_info_tmp->url = %s\n",tokenfile_info_tmp->url);
            if(i == 0)
            {
                fprintf(fp,"%s\n%s",tokenfile_info_tmp->url,tokenfile_info_tmp->folder);
                i=1;
            }
            else
            {
                fprintf(fp,"\n%s\n%s",tokenfile_info_tmp->url,tokenfile_info_tmp->folder);
            }
        }

        tokenfile_info_tmp = tokenfile_info_tmp->next;
    }

    fclose(fp);
    if(!i)
        remove(filename);
    free(filename);

    wd_DEBUG("write_to_tokenfile end\n");
    return 0;
}
#ifdef NVRAM_
int write_to_nvram(char *contents,char *nv_name)
{
    char *command;
    command = my_str_malloc(strlen(contents)+strlen(SHELL_FILE)+strlen(nv_name)+8);
    snprintf(command, sizeof(char)*(strlen(contents)+strlen(SHELL_FILE)+strlen(nv_name)+8), "sh %s \"%s\" %s",SHELL_FILE,contents,nv_name);

    wd_DEBUG("command : [%s]\n",command);

    system(command);
    free(command);

    return 0;
}
#else
int write_to_wd_tokenfile(char *contents)
{
    if(contents == NULL || contents == "")
    {
        unlink(TOKENFILE_RECORD);
        return 0;
    }
    FILE *fp;
    if( ( fp = fopen(TOKENFILE_RECORD,"w") ) == NULL )
    {
        wd_DEBUG("open wd_tokenfile failed!\n");
        return -1;
    }
    fprintf(fp,"%s",contents);
    fclose(fp);
    return 0;
}
#endif
#ifdef NVRAM_
int convert_nvram_to_file_mutidir(char *file,struct asus_config *config)
{

    wd_DEBUG("enter convert_nvram_to_file_mutidir function\n");

    FILE *fp;
    char *nv, *nvp, *b;
    int i;
    char *p;
    char *buffer;
    char *buf;

    fp=fopen(file, "w");

    if (fp==NULL) return -1;
#ifndef USE_TCAPI
    nv = nvp = strdup(nvram_safe_get("cloud_sync"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "cloud_sync", tmp);
    nvp = nv = my_str_malloc(strlen(tmp)+1);
    snprintf(nv, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif

    if(nv)
    {
        while ((b = strsep(&nvp, "<")) != NULL)
        {
            i = 0;
            buf = buffer = strdup(b);

            wd_DEBUG("buffer = %s\n",buffer);

            while((p = strsep(&buffer,">")) != NULL)
            {

                wd_DEBUG("p = %s\n",p);

                if (*p == 0)
                {
                    fprintf(fp,"\n");
                    i++;
                    continue;
                }
                if(i == 0)
                {
                    if(atoi(p) != 3)
                        break;
                    fprintf(fp,"%s",p);
                }
                else
                {
                    fprintf(fp,"\n%s",p);
                }

                if(i == 4)
                    config->dir_number = atoi(p);
                i++;
            }
            free(buf);
        }
        free(nv);
    }

    else
        wd_DEBUG("get nvram fail\n");

    fclose(fp);

    wd_DEBUG("end convert_nvram_to_file_mutidir function\n");

    return 0;
}
#else
int create_webdav_conf_file(struct asus_config *config){


    wd_DEBUG("enter create_dropbox_conf_file function\n");

    FILE *fp;
    char *nv, *nvp, *b;
    int i;
    char *p;
    char *buffer;
    char *buf;

    fp = fopen(TMPCONFIG,"r");
    if (fp==NULL)
    {
        nvp = my_str_malloc(2);
        snprintf(nvp, sizeof(char)*2, "%s", "");
    }
    else
    {
        fseek( fp , 0 , SEEK_END );
        int file_size;
        file_size = ftell( fp );
        fseek(fp , 0 , SEEK_SET);
        nvp =  (char *)malloc( file_size * sizeof( char ) );
        fread(nvp , file_size , sizeof(char) , fp);
        fclose(fp);
    }

    nv = nvp;

    replace_char_in_str(nvp,'\0','\n');

    wd_DEBUG("**********nv = %s\n",nv);


    fp=fopen(CONFIG_PATH, "w");

    if (fp==NULL) return -1;

    config->dir_number = 0;
    if(nv)
    {
        while ((b = strsep(&nvp, "<")) != NULL)
        {
            i = 0;
            buf = buffer = strdup(b);

            wd_DEBUG("buffer = %s\n",buffer);

            while((p = strsep(&buffer,">")) != NULL)
            {

                wd_DEBUG("p = %s\n",p);

                if (*p == 0)
                {
                    fprintf(fp,"\n");
                    i++;
                    continue;
                }
                if(i == 0)
                {
                    if(atoi(p) != 3)
                        break;
                    fprintf(fp,"%s",p);
                }
                else
                {
                    fprintf(fp,"\n%s",p);
                }

                if(i == 4)
                    config->dir_number = atoi(p);
                i++;
            }
            free(buf);

        }

        free(nv);

    }
    else
        wd_DEBUG("get nvram fail\n");

    fclose(fp);
    return 0;

}
#endif
int rewrite_tokenfile_and_nv(){

    int i,j;
    int exist;

    wd_DEBUG("rewrite_tokenfile_and_nv start\n");
    if(asus_cfg.dir_number > asus_cfg_stop.dir_number)
    {
        for(i=0;i<asus_cfg.dir_number;i++)
        {
            exist = 0;
            for(j=0;j<asus_cfg_stop.dir_number;j++)
            {
                if(!strcmp(asus_cfg_stop.prule[j]->rooturl,asus_cfg.prule[i]->rooturl))
                {
                    exist = 1;
                    break;
                }
            }
            if(!exist)
            {
                wd_DEBUG("del form nv\n");
                char *new_nv;
                delete_tokenfile_info(asus_cfg.user,asus_cfg.prule[i]->rooturl);
                new_nv = delete_nvram_contents(asus_cfg.user,asus_cfg.prule[i]->rooturl);
                wd_DEBUG("new_nv = %s\n",new_nv);

                remove(g_pSyncList[i]->conflict_file);

                write_to_tokenfile(asus_cfg.prule[i]->base_path);

#ifdef NVRAM_
                write_to_nvram(new_nv,"db_tokenfile");
#else
                write_to_wd_tokenfile(new_nv);
#endif

                free(new_nv);
            }
        }
    }
    else
    {
        for(i=0;i<asus_cfg_stop.dir_number;i++)
        {
            exist = 0;
            for(j=0;j<asus_cfg.dir_number;j++)
            {
                if(!strcmp(asus_cfg_stop.prule[i]->rooturl,asus_cfg.prule[j]->rooturl))
                {
                    exist = 1;
                    break;
                }
            }
            if(!exist)
            {
                wd_DEBUG("add to nv\n");
                char *new_nv;
                add_tokenfile_info(asus_cfg_stop.user,asus_cfg_stop.prule[i]->rooturl,asus_cfg_stop.prule[i]->base_path);
                new_nv = add_nvram_contents(asus_cfg_stop.user,asus_cfg_stop.prule[i]->rooturl);
                wd_DEBUG("new_nv = %s\n",new_nv);
                wd_DEBUG("base_path = %s\n",asus_cfg_stop.prule[i]->base_path);
                write_to_tokenfile(asus_cfg_stop.prule[i]->base_path);

#ifdef NVRAM_
                write_to_nvram(new_nv,"db_tokenfile");
#else
                write_to_wd_tokenfile(new_nv);
#endif

                free(new_nv);
            }
        }
    }
    return 0;
}
#ifndef NVRAM_
int write_get_nvram_script(){

    FILE *fp;
    char contents[512];
    memset(contents,0,512);
    snprintf(contents, 512, "%s", "#! /bin/sh\nNV=`nvram get cloud_sync`\nif [ ! -f \"/tmp/smartsync/dropbox/config/dropbox_tmpconfig\" ]; then\n   touch /tmp/smartsync/dropbox/config/dropbox_tmpconfig\nfi\necho \"$NV\" >/tmp/smartsync/dropbox/config/dropbox_tmpconfig");

    if(( fp = fopen(DropBox_Get_Nvram,"w"))==NULL)
    {
        fprintf(stderr,"create dropbox_get_nvram file error\n");
        return -1;
    }

    fprintf(fp,"%s",contents);
    fclose(fp);

    return 0;
}
int write_get_nvram_script_va(char *str)
{
    FILE *fp;
    char contents[512];
    memset(contents,0,512);
    snprintf(contents, 512, "#! /bin/sh\nNV=`nvram get %s`\nif [ ! -f \"%s\" ]; then\n   touch %s\nfi\necho \"$NV\" >%s",str,TMP_NVRAM_VL,TMP_NVRAM_VL,TMP_NVRAM_VL);

    if(( fp = fopen(DropBox_Get_Nvram_Link,"w"))==NULL)
    {
        fprintf(stderr,"create dropbox_get_nvram file error\n");
        return -1;
    }

    fprintf(fp,"%s",contents);
    fclose(fp);

    return 0;
}
#endif
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
            tokenfile_info_tmp = NULL;
            break;
        }
        tmp = tokenfile_info_tmp;
        tokenfile_info_tmp = tokenfile_info_tmp->next;
    }

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
    p = nv = strdup(nvram_safe_get("db_tokenfile"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "db_tokenfile", tmp);
    p = nv = my_str_malloc(strlen(tmp)+1);
    snprintf(nv, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif
#else
    FILE *fp;
    fp = fopen(TOKENFILE_RECORD,"r");
    if (fp==NULL)
    {
        nv = my_str_malloc(2);
        snprintf(nv, sizeof(char)*2, "%s", "");
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
        memset(nv,0,sizeof(nv));
        sprintf(nv,"");
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
char *add_nvram_contents(char *url,char *folder){

    char *nv;
    int nv_len;
    char *new_nv;
    char *nvp;

    nvp = my_str_malloc(strlen(url)+strlen(folder)+2);
    snprintf(nvp, sizeof(char)*(strlen(url)+strlen(folder)+2), "%s>%s",url,folder);

    wd_DEBUG("add_nvram_contents     nvp = %s\n",nvp);


#ifdef NVRAM_
#ifndef USE_TCAPI
    nv = strdup(nvram_safe_get("db_tokenfile"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "db_tokenfile", tmp);
    nv = my_str_malloc(strlen(tmp)+1);
    snprintf(nv, sizeof(char)*(strlen(tmp)+1), "%s",tmp);
#endif
#else
    FILE *fp;
    fp = fopen(TOKENFILE_RECORD,"r");
    if (fp==NULL)
    {
        nv = my_str_malloc(2);
        snprintf(nv, sizeof(char)*2, "%s", "");
    }
    else
    {
        fseek( fp , 0 , SEEK_END );
        int file_size;
        file_size = ftell( fp );
        fseek(fp , 0 , SEEK_SET);
        wd_DEBUG("add_nvram_contents     file_size = %d\n",file_size);
        nv = my_str_malloc(file_size+2);
        fscanf(fp,"%[^\n]%*c",nv);
        fclose(fp);
    }
#endif

    wd_DEBUG("add_nvram_contents     nv = %s\n",nv);
    nv_len = strlen(nv);

    if(nv_len)
    {
        new_nv = my_str_malloc(strlen(nv)+strlen(nvp)+2);
        snprintf(new_nv, sizeof(char)*(strlen(nv)+strlen(nvp)+2), "%s<%s",nv,nvp);

    }
    else
    {
        new_nv = my_str_malloc(strlen(nvp)+1);
        snprintf(new_nv, sizeof(char)*(strlen(nvp)+1) ,"%s",nvp);
    }
    wd_DEBUG("add_nvram_contents     new_nv = %s\n",new_nv);
    free(nvp);
    free(nv);
    return new_nv;
}
int check_sync_disk_removed()
{

    wd_DEBUG("check_sync_disk_removed start! \n");


    int ret;

    free_tokenfile_info(tokenfile_info_start);

    if(get_tokenfile_info()==-1)
    {
        wd_DEBUG("\nget_tokenfile_info failed\n");
        exit(-1);
    }

    ret = check_config_path(0);
    return ret;

}
int check_disk_change()
{
    int status = -1;
    disk_change = 0;
    status = check_sync_disk_removed();

    if(status == 2 || status == 1)
    {
        exit_loop = 1;

        sync_disk_removed = 1;
    }

    return 0;
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
int detect_client()
{
    unlink("/tmp/smartsync_app/dropbox_client_start");
    char *dir = "/tmp/smartsync_app";
    int flag = 0;
    if(access(dir,0) != 0)
        flag = 0;
    else
    {
        struct dirent* ent = NULL;
        DIR *pDir;
        pDir=opendir(dir);
        int file_num = 0;
        if(pDir != NULL )
        {
            while (NULL != (ent=readdir(pDir)))
            {
                if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                    continue;
                file_num ++ ;
            }
            closedir(pDir);

            if(file_num > 0)
                flag = 1;
            else
                flag = 0;

        }
        else
            flag = 0;
    }
    if(!flag)
    {
        system("killall -9 inotify &");
    }
}


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
#ifdef NVRAM_
int create_shell_file(){

    FILE *fp;
    char contents[1024];
    memset(contents,0,1024);
#ifndef USE_TCAPI
    snprintf(contents, 1024, "%s", "#! /bin/sh\nnvram set $2=\"$1\"\nnvram commit");
#else
    snprintf(contents, 1024, "%s", "#! /bin/sh\ntcapi set AiCloud_Entry $2 \"$1\"\ntcapi commit AiCloud\ntcapi save");
#endif

    if(( fp = fopen(SHELL_FILE,"w"))==NULL)
    {
        fprintf(stderr,"create shell file error");
        return -1;
    }

    fprintf(fp,"%s",contents);
    fclose(fp);
    return 0;

}
/*process running,return 1;else return 0*/

#endif
#endif

void deal_big_low_conflcit(char *server_conflict_name,char *oldname,char *newname,char *newname_r,int index)
{
    /*get conflict name*/

    char *local_conflcit_name = serverpath_to_localpath(server_conflict_name,index);
    char *g_newname = NULL;
    int is_folder = test_if_dir(local_conflcit_name);

    g_newname = get_confilicted_name_case(local_conflcit_name,is_folder);

    wd_DEBUG("case-conflict=%s\n",g_newname);
    my_free(local_conflcit_name);

    char *localname;
    /*rename local conflict name to newname*/
    if(newname_r == NULL)
    {
        localname = serverpath_to_localpath(newname,index);
    }
    else
        localname = newname_r;

    updata_socket_list(localname,g_newname,index);

    if(access(localname,F_OK) == 0)
    {
        add_action_item("rename",g_newname,g_pSyncList[index]->server_action_list);
        rename(localname,g_newname);
    }

    if(newname_r == NULL)
        my_free(localname);
    /*reback to deal rename a--> bb(1)*/
    char *s_newname = localpath_to_serverpath(g_newname,index);
    my_free(g_newname);
    int status = 0;
    do{
        status = api_move(oldname,s_newname,index,1,NULL);
    }while(status != 0 && !exit_loop);
    my_free(s_newname);
}
