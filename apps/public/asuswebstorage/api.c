#include <time.h>
#include <unistd.h>
#include <curl/curl.h>
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/xmlreader.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include "api.h"
#include "data.h"
#include "function.h"

#define BUFSIZE	1024*16
#define UP_QUEUE 0
#define STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES         6000
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3

typedef enum time_status
{
    older,
    newer,
    same,
    new_file
} time_status;


typedef enum rename_type_t
{
    createfolder_action,
    createfile_action,
    rename_action,
    move_action
} rename_type ;

typedef struct server_capacity_tag
{
    long long int total;
    long long int used;
} server_capacity;

server_capacity pre_cap;

char *sid = "83379331"; //2015.05.22 update 50001 -> 83379331
char *VERSION = "1.0.0.4";
Servicegateway sergate;
extern Aaa aaa;
extern char username[256];
extern char password[256];
extern queue_t queue_download;
extern queue_t queue_upload;
extern struct sync_item *down_head;
extern struct sync_item *up_head;
extern long long int MySyncFolder;
extern int exit_loop;
extern int error_flag;
extern struct sync_item *from_server_sync_head;
extern sync_item_t up_excep_fail;
extern sync_item_t download_only_socket_head;
extern int upload_only;
double start_time;
extern int server_space_full;
extern int local_space_full;
extern long long int max_upload_filesize;
extern int copying_file_number;
extern struct asus_config cfg;
extern int IsAccountFrozen;
long long int MyRecycleID;
extern int IsSyncError;
extern char token_filename[256];
extern char record_token_file[256];
extern int no_completed;
extern int pre_seq;
extern Hb_SubNode *SyncNode;
extern my_mutex_t wait_server_mutex;
extern api_count_s api_count;

typedef struct cmp_item
{
    time_status status;
    long long id;
    long long size;
}cmp_item_t ;

int is_exist_case_conflicts(char *fullname,char *pre_name);
size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)

{
   if(exit_loop)
        return -1;

  int len ;
  len = fwrite(ptr, size, nmemb, stream);
  return len;

}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    exit(EXIT_FAILURE);
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)

{

   if(exit_loop)
       return -1;

  int len;
  len = fread(ptr, size, nmemb, stream);
  return len;

}

int my_progress_func(char *progress_data,

                     double t, /* dltotal */

                     double d, /* dlnow */

                     double ultotal,

                     double ulnow)

{

#if 0
    int sec;
    double  elapsed = 0;
    elapsed = time(NULL) - start_time;
    sec = (int)elapsed;
    if( sec > 0 )
    {
        if(sec % 5 == 0)
            printf("%s %g / %g (%g %%)\n", progress_data, ulnow, ultotal, ulnow*100.0/ultotal);
    }
#endif

#if 1
    if(exit_loop)
        return -1;



    if(t > 1 && d > 10 && ultotal > 0) // download
        printf("%s %10.0f / %10.0f (%g %%)\n", progress_data, d, t, d*100.0/t);
    else
        printf("%s %10.0f / %10.0f (%g %%)\n", progress_data, ulnow, ultotal, ulnow*100.0/ultotal);

#endif
  return 0;
}

#if 0
struct myprogress {
  double lastruntime;
  CURL *curl;
};

static int progress(void *p,
                    double dltotal, double dlnow,
                    double ultotal, double ulnow)
{
  struct myprogress *myp = (struct myprogress *)p;
  CURL *curl = myp->curl;
  double curtime = 0;

  curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);

  /* under certain circumstances it may be desirable for certain functionality
     to only run every N seconds, in order to do this the transaction time can
     be used */
  if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
    myp->lastruntime = curtime;
    fprintf(stderr, "TOTAL TIME: %f \r\n", curtime);
  }

  if(dltotal < dlnow){
          fprintf(stderr, "read : %f bytes\n", dlnow);
  }else{
          fprintf(stderr, "%% %.2f processed\n", (dlnow/dltotal)*100);
  }

  if(dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
    return 1;
  return 0;
}
#endif

size_t write_data(void *ptr,size_t size,size_t nmemb,void *stream)
{
    if( exit_loop == 1)
        return -1;
    int len = fwrite(ptr,size,nmemb,(FILE *)stream);
    return len;
}

size_t read_data(void *ptr,size_t size,size_t nmemb,void *stream)
{
    return fread(ptr,size,nmemb,stream);
}

void
        parseGateway (xmlDocPtr doc, xmlNodePtr cur,Servicegateway *sg)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                sg->status = atoi((const char *)key);
                if(sg->status != 0)
                {
                    xmlFree(key);
                    return;
                }
            }

            if( !(xmlStrcmp(cur->name, (const xmlChar *)"servicegateway")))
            {
                snprintf(sg->gateway, MINSIZE, "%s", (const char *)key);
            }

            xmlFree(key);
            cur = cur->next;
        }
}

int
        parseGetmysyncfolder(xmlDocPtr doc, xmlNodePtr cur,Getmysyncfolder *gf)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            gf->status = atoi((const char *)key);
            if(gf->status != 0)
            {
                xmlFree(key);
                return -1;
            }
        }
        else if (!(xmlStrcmp(cur->name, (const xmlChar *)"id")))
        {
          gf->id = atoll((const char *)key);
        }


        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseGetPersonalSystemFolder(xmlDocPtr doc, xmlNodePtr cur,Getpersonalsystemfolder *gp)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            gp->status = atoi((const char *)key);
        }
        else if (!(xmlStrcmp(cur->name, (const xmlChar *)"script")))
        {
            snprintf(gp->script, 16, "%s", (const char *)key);
        }
        else if (!(xmlStrcmp(cur->name, (const xmlChar *)"folderid")))
        {
             gp->folderid = atoll((const char *)key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseGetinfo (xmlDocPtr doc, xmlNodePtr cur,Getinfo *gi)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            gi->status = atoi((const char *)key);
            if(gi->status != 0)
            {
                xmlFree(key);
                return -1;
            }
        }
        if( !(xmlStrcmp(cur->name, (const xmlChar *)"account")))
        {
            gi->account = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"email")))
        {
            snprintf(gi->email, NORMALSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"regyear")))
        {
            snprintf(gi->regyear, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"language")))
        {
            snprintf(gi->language, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"activateddate")))
        {
            snprintf(gi->activateddate, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"credentialstate")))
        {
            gi->credentialstate = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"usedbackuppc")))
        {
            gi->usedbackuppc = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"backuppc")))
        {
            parseGetinfo(doc,cur,gi);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"name")))
        {
            snprintf(gi->backuppc.name, NORMALSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"createdtime")))
        {
            snprintf(gi->backuppc.createdtime, NORMALSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"package")))
        {
            parseGetinfo(doc,cur,gi);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
        {
            gi->package.id = atoll((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"display")))
        {
            snprintf(gi->package.display, NORMALSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"capacity")))
        {
            gi->package.capacity = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"uploadbandwidth")))
        {
            gi->package.uploadbandwidth = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"downloadbandwidth")))
        {
            gi->package.downloadbandwidth = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"upload")))
        {
            gi->package.upload = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"download")))
        {
            gi->package.download = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"concurrentsession")))
        {
            gi->package.concurrentsession = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"maxfilesize")))
        {
            gi->package.maxfilesize = atoll((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"sharegroup")))
        {
            gi->package.sharegroup = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"hasencrption")))
        {
            gi->package.hasencrption = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"expire")))
        {
            snprintf(gi->package.expire, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"maxbackuppc")))
        {
            gi->package.maxbackuppc = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"usedcapacity")))
        {
            gi->usedcappacity = atoll((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"freecapacity")))
        {
            gi->freecapacity = atoll((const char *)key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

static int
        parseAaa (xmlDocPtr doc, xmlNodePtr cur,Aaa *aaa)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        /* if value is null and node is no children node,not parse */
        if(key == NULL && cur->children == NULL)
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            aaa->status = atoi((const char *)key);
        }

        if( !(xmlStrcmp(cur->name, (const xmlChar *)"token")))
        {
            snprintf(aaa->token, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"contentrelay")))
        {
            snprintf(aaa->contentrelay, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"filerelay")))
        {
            snprintf(aaa->filerelay, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"inforelay")))
        {
            snprintf(aaa->inforelay, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"jobrelay")))
        {
            snprintf(aaa->jobrelay, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"rssrelay")))
        {
            snprintf(aaa->rssrelay, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"searchrelay")))
        {
            snprintf(aaa->searchrelay, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"webrelay")))
        {
            snprintf(aaa->webrelay, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"package")))
        {
           parseAaa(doc,cur,aaa);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"auxpasswordurl")))
        {
            char *p = oauth_url_unescape((const char *)key,NULL);
            snprintf(aaa->auxpasswordurl, 256, "%s", p);
            free(p);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseInitbinaryupload (xmlDocPtr doc, xmlNodePtr cur,Initbinaryupload *ibu)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            ibu->status = atoi((const char *)key);
        }

        if(ibu->status == 0)
        {
            if( !(xmlStrcmp(cur->name, (const xmlChar *)"transid")))
            {
                snprintf(ibu->transid, NORMALSIZE, "%s", (const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"offset")))
            {
                ibu->offset = atoi((const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"fileid")))
            {
                ibu->fileid = atoll((const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"latestchecksum")))
            {
                snprintf(ibu->latestchecksum, 256, "%s", (const char *)key);
            }
        }
        else
        {
            if( !(xmlStrcmp(cur->name, (const xmlChar *)"logmessage")))
            {
                snprintf(ibu->logmessage, NORMALSIZE, "%s", (const char *)key);
            }
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseResumebinaryupload (xmlDocPtr doc, xmlNodePtr cur,Resumebinaryupload *rbu)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            rbu->status = atoi((const char *)key);
            if(rbu->status != 0)
            {
            	xmlFree(key);
            	return -1;
            }
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseFinishbinaryupload (xmlDocPtr doc, xmlNodePtr cur,Finishbinaryupload *fbu)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            fbu->status = atoi((const char *)key);
            if(fbu->status != 0)
            {
            	xmlFree(key);
            	return -1;
            }
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"fileid")))
        {
            fbu->fileid = atoll((const char *)key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int get_server_item_size(xmlDocPtr doc, xmlNodePtr cur,Browse *browse)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    int folder_size = 0;
    int file_size = 0;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            browse->status = atoi((const char *)key);
            if(browse->status != 0)
            {
                xmlFree(key);
                return -1;
            }
        }

        if( !(xmlStrcmp(cur->name, (const xmlChar *)"folder")))
        {
            folder_size += 1 ;
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"file")))
        {
            file_size += 1 ;
        }
        xmlFree(key);
        cur = cur->next;
    }

    if(file_size > 0)
    {
            browse->filelist = (File **)malloc(sizeof(File *)*file_size);

            if(browse->filelist == NULL)
            {
                handle_error(S_MEMORY_FAIL,"get_server_item_size");
                return -1;
            }
    }

    if(folder_size > 0)
    {
            browse->folderlist = (Folder **)malloc(sizeof(Folder *)*folder_size);

            if(browse->folderlist == NULL)
            {
                handle_error(S_MEMORY_FAIL,"get_server_item_size");
                my_free(browse->filelist);
                return -1;
            }
    }

    return 0;
}


static int
        parseBrowse (xmlDocPtr doc, xmlNodePtr cur,Browse *browse)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    int foldernum = browse->foldernumber-1 ;
    int filenum = browse->filenumber-1;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            browse->status = atoi((const char *)key);
            if(browse->status != 0)
            {
            	xmlFree(key);
                free_server_list(browse);
                return -1;
            }
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
        {
            if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                (browse->folderlist)[foldernum]->id = atoll((const char *)key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                (browse->filelist)[filenum]->id = atoll((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"folder")))
        {
            browse->foldernumber += 1 ;
            int num = browse->foldernumber -1;

            (browse->folderlist)[num] = (Folder *)malloc(sizeof(Folder));
            if((browse->folderlist)[num] == NULL)
            {
                handle_error(S_MEMORY_FAIL,"parse browse");
                free_server_list(browse);
                xmlFree(key);
                return -1;
            }
            memset(browse->folderlist[num],0,sizeof(Folder));
            parseBrowse(doc,cur,browse);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"display")))
        {
                int len = strlen((const char *)key)+1;
                if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                {
                    browse->folderlist[foldernum]->display = calloc(len,sizeof(char));
                    snprintf((browse->folderlist)[foldernum]->display, len*sizeof(char), "%s", (const char *)key);
                }
                else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                {
                    (browse->filelist)[filenum]->display = calloc(len,sizeof(char));
                    snprintf((browse->filelist)[filenum]->display, len*sizeof(char), "%s", (const char *)key);
                }
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"attribute")))
        {
            parseBrowse(doc,cur,browse);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastaccesstime")))
        {
                if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                    snprintf((browse->filelist)[filenum]->attribute.lastaccesstime, 16, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastwritetime")))
        {
                 if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                    snprintf((browse->filelist)[filenum]->attribute.lastwritetime, 16, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"file")))
        {
            browse->filenumber += 1 ;
            int num = browse->filenumber -1;
            (browse->filelist)[num] = (File *)malloc(sizeof(File));
            if((browse->filelist)[num] == NULL)
            {
                handle_error(S_MEMORY_FAIL,"parse browse");
                free_server_list(browse);
                xmlFree(key);
                return -1;
            }
            memset(browse->filelist[num],0,sizeof(File));
            parseBrowse(doc,cur,browse);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"size")))
        {
                (browse->filelist)[filenum]->size = atoll((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"page")))
        {
            parseBrowse(doc,cur,browse);
        }
        if( !(xmlStrcmp(cur->name, (const xmlChar *)"pageno")))
        {
            browse->page.pageno = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"pagesize")))
        {
            browse->page.pagesize = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"totalcount")))
        {
            browse->page.totalcount= atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"hasnextpage")))
        {
            browse->page.hasnextpage = atoi((const char *)key);
        }
        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int find__root_node(const char *buf,const char *key)
{
    int res = 1;
    char *p = NULL;

    p = strstr(buf,key) ;
    if (p == NULL)
    {
        printf("Have no XML content tail!\n");
        res = 0;
    }

    return res;
}

char *get_xml_value(const char *buf,const char *key)
{
    char *start=NULL,*end=NULL,*value = NULL;
    char stag[64]={0},etag[64]={0};
    int len = 0;

    snprintf(stag, 64, "<%s>",key);
    snprintf(etag, 64, "</%s>",key);

    start = strstr(buf,stag);
    end = strstr(buf,etag);

    if(end && start)
    {
        len = strlen(start) - strlen(end)- strlen(stag);
        value = (char *)calloc(len+1,sizeof(char));
        strncpy(value,start+strlen(stag),len);
    }

    return value;
}

int get_item_num(const char *buf,int isfile)
{
    char *nodeStart = NULL;
    char *nodeEnd = NULL;
    int i=0 ;
    const char *p=NULL;
    int len = 0;
    char stag[16]={0},etag[16]={0};

    p = buf;
    if(isfile)
    {
        snprintf(stag, 16, "%s", "<file>");
        snprintf(etag, 16, "%s", "</file>");
    }
    else
    {
        snprintf(stag, 16, "%s", "<folder>");
        snprintf(etag, 16, "%s", "</folder>");
    }

    nodeStart = strstr(p,stag);
    nodeEnd   = strstr(p,etag);

    while(nodeStart && nodeEnd)
    {
        i++;
        len = strlen(etag);
        p = nodeEnd + len;

        nodeStart = strstr(p,stag);
        nodeEnd   = strstr(p,etag);
    }

    return i;
}

int get_item_list(const char *buf,Browse *br,int isfile)
{
    char *nodeStart = NULL;
    char *nodeEnd = NULL;
    char stag[16]={0},etag[16]={0};
    int index = 0,tag_len=0;
    char *value = NULL;
    int len = 0;
    char *node = NULL;
    char *temp = NULL;
    char *p = NULL;

    if(isfile)
    {
        snprintf(stag, 16, "%s", "<file>");
        snprintf(etag, 16, "%s", "</file>");
    }
    else
    {
        snprintf(stag, 16, "%s", "<folder>");
        snprintf(etag, 16, "%s", "</folder>");
    }

    nodeStart = strstr(buf,stag);
    nodeEnd   = strstr(buf,etag);

    while(nodeStart && nodeEnd)
    {
        tag_len = strlen(stag);
        len = strlen(nodeStart) - strlen(nodeEnd)-tag_len;
        node = (char *)calloc(len+1,sizeof(char));
        strncpy(node,nodeStart+tag_len,len);

        if(isfile)
            br->filelist[index] = (File *)calloc(1,sizeof(File));
        else
            br->folderlist[index] = (Folder *)calloc(1,sizeof(Folder));

        value = get_xml_value(node,"id");
        if(value)
        {
            if(isfile)
                br->filelist[index]->id = atoll(value);
            else
                br->folderlist[index]->id = atoll(value);
            my_free(value);
        }

        value = get_xml_value(node,"display");
        if(value)
        {
            temp = (char *)calloc(strlen(value)+1,sizeof(char));
            snprintf(temp, (strlen(value)+1)*sizeof(char), "%s", value);
            if(isfile)
                br->filelist[index]->display = temp;
            else
                br->folderlist[index]->display = temp;
            my_free(value);
        }

        if(isfile)
        {
            value = get_xml_value(node,"size");
            if(value)
            {
                br->filelist[index]->size = atoll(value);
                my_free(value);
            }

            value = get_xml_value(node,"lastaccesstime");
            if(value)
            {
                snprintf(br->filelist[index]->attribute.lastaccesstime, 16, "%s", value);
                my_free(value);
            }

            value = get_xml_value(node,"lastwritetime");
            if(value)
            {
                snprintf(br->filelist[index]->attribute.lastwritetime, 16, "%s", value);
                my_free(value);
            }

        }

        my_free(node);
        p = nodeEnd + strlen(etag);
        nodeStart = strstr(p,stag);
        nodeEnd = strstr(p,etag);
        index++;
    }

    return 0;
}

int my_parse_browse_xml(const char *filename,Browse *br)
{
    char *value = NULL;
    int foldernum=0,filenum=0;
    char *buf = NULL;
    int len=0;

    LoadFileIntoBuffer(filename,&buf,&len);
    if(NULL == buf)
        return -1;

    if( !find__root_node(buf,"<browse>") || !find__root_node(buf,"</browse>"))
    {
        printf("find <browse> node fail\n");
        my_free(buf);
        return -1;
    }

    value = get_xml_value(buf,"status");
    if(value)
    {
        br->status = atoi(value);
        my_free(value);
        if(br->status != 0)
        {
            my_free(buf);
            return -2;
        }
    }

    foldernum = get_item_num(buf,0);
    if(foldernum > 0)
    {
        br->folderlist = (Folder **)calloc(foldernum,sizeof(Folder *));
        br->foldernumber = foldernum;
        get_item_list(buf,br,0);
    }

    filenum = get_item_num(buf,1);
    if(filenum > 0)
    {
        br->filelist = (File **)calloc(filenum,sizeof(File *));
        br->filenumber = filenum;
        get_item_list(buf,br,1);
    }

    my_free(buf);
    return 0;
}

void
        parsePropfind (xmlDocPtr doc, xmlNodePtr cur,Propfind *pfind)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                pfind->status = atoi((const char *)key);
                if(pfind->status != 0)
                {
                	xmlFree(key);
                    return;
                }
            }

            if( !(xmlStrcmp(cur->name, (const xmlChar *)"isencrypt")))
            {
                pfind->isencrypt = atoi((const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"size")))
            {
                pfind->size = atoll((const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
                pfind->script = atoi((const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"type")))
            {
                snprintf(pfind->type, MINSIZE, "%s", (const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
            {
                pfind->id = atoll((const char *)key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}

void
        parseCreatefolder (xmlDocPtr doc, xmlNodePtr cur,Createfolder *createfolder)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                createfolder->status = atoi((const char *)key);
                if(createfolder->status != 0)
                {
                	xmlFree(key);
                    return;
                }
            }


            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               snprintf(createfolder->scrip, MINSIZE, "%s", (const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
            {
                createfolder->id = atoll((const char *)key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}

void
        parseChangeseq (xmlDocPtr doc, xmlNodePtr cur,Changeseq *changeseq)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                changeseq->status = atoi((const char *)key);
                if(changeseq->status != 0)
                {
                	xmlFree(key);
                    return;
                }
            }

            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               snprintf(changeseq->scrip, MINSIZE, "%s", (const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"changeseq")))
            {
                changeseq->changeseq = atoi((const char *)key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}


static int
        parseGetEntryinfo (xmlDocPtr doc, xmlNodePtr cur,Getentryinfo *ge)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        /* if value is null and node is no children node,not parse */
        if(key == NULL && cur->children == NULL)
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
           ge->status = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
        {
           snprintf(ge->scrip, MINSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isfolder")))
        {
           ge->isfolder = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"display")))
        {
           snprintf(ge->display, NORMALSIZE, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"parent")))
        {
           ge->parent = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"headversion")))
        {
           ge->headversion = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"attribute")))
        {
            parseGetEntryinfo(doc,cur,ge);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastaccesstime")))
        {
             snprintf(ge->attr.lastaccesstime, 16, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastwritetime")))
        {
            snprintf(ge->attr.lastwritetime, 16, "%s", (const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"filesize")))
        {
            ge->filesize = atoll((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"treesize")))
        {
            ge->treesize = atoll((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"ishidden")))
        {
            ge->ishidden = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isinfected")))
        {
            ge->isinfected = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isbackup")))
        {
            ge->isbackup = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isorigdeleted")))
        {
            ge->isorigdeleted = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"ispublic")))
        {
            ge->ispublic = atoi((const char *)key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"createdtime")))
        {
            snprintf(ge->createdtime, MINSIZE, "%s", (const char *)key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

void parseOperateEntry(xmlDocPtr doc, xmlNodePtr cur,Operateentry *oe)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                oe->status = atoi((const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               snprintf(oe->script, MINSIZE, "%s", (const char *)key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}

void parseMoveEntry(xmlDocPtr doc, xmlNodePtr cur,Moveentry *me)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                me->status = atoi((const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               snprintf(me->script, MINSIZE, "%s", (const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"logmessage")))
            {
               snprintf(me->logmessage, NORMALSIZE, "%s", (const char *)key);
            }

            xmlFree(key);
            cur = cur->next;
        }
}

int StrToHex(char *src,int len)
{
    int sum;
    char str[4] = {0};
    if(len == 1)
    {
       snprintf(str, 4, "%s", src);
       sum = atoi(str);
    }
    else if(len == 2)
    {
       strncpy(str,src,1);
       sum = atoi(str)*16;

       memset(str,0,sizeof(str));
       snprintf(str, 4, "%s", src+1);
       sum += atoi(str);
   }

    return sum;
}

void parseGetUserState(xmlDocPtr doc, xmlNodePtr cur,Getuserstate *gu)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                gu->status = atoi((const char *)key);
            }
            else if (!(xmlStrcmp(cur->name, (const xmlChar *)"servicestate")))
            {
               parseGetUserState(doc,cur,gu);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"userstate")))
            {    
                gu->userstate = StrToHex((char *)key,strlen((const char *)key));
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"extensionstarttime")))
            {
               snprintf(gu->extensionstarttime, 32, "%s", (const char *)key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"extensionendtime")))
            {
               snprintf(gu->extensionendtime, 32, "%s", (const char *)key);
            }

            xmlFree(key);
            cur = cur->next;
        }
}

int
        parseDoc1(char *docname,void *obj)
{
    xmlDocPtr doc;
    xmlNodePtr cur;

        doc = xmlParseFile(docname);

    if (doc == NULL ) {
        printf("%s not parsed successfully. \n",docname);
        return -1;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        printf("%s empty document\n",docname);

        xmlFreeDoc(doc);
        return -1;
    }

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"requestservicegateway")))
        {
            parseGateway(doc, cur,(Servicegateway *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"aaa")))
        {
            parseAaa(doc,cur,(Aaa *)obj);
        } 
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"browse")))
        {
            if(get_server_item_size(doc,cur,(Browse *)obj) == -1)
                return -1;
            parseBrowse(doc,cur,(Browse *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"propfind")))
        {
            parsePropfind(doc,cur,(Propfind *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"create")))
        {
            parseCreatefolder(doc,cur,(Createfolder *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getchangeseq")))
        {
            parseChangeseq(doc,cur,(Changeseq *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"initbinaryupload")))
        {
            parseInitbinaryupload(doc,cur,(Initbinaryupload *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"resumebinaryupload")))
        {
            parseResumebinaryupload(doc,cur,(Resumebinaryupload *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"finishbinaryupload")))
        {
            parseFinishbinaryupload(doc,cur,(Finishbinaryupload *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getinfo")))
        {
            parseGetinfo(doc,cur,(Getinfo *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getmysyncfolder")))
        {
            parseGetmysyncfolder(doc,cur,(Getmysyncfolder *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getentryinfo")))
        {
            parseGetEntryinfo(doc,cur,(Getentryinfo *)obj);
        }
        else  if( (!xmlStrcmp(cur->name, (const xmlChar *)"remove") ) ||
              !xmlStrcmp(cur->name, (const xmlChar *)"rename") )
        {
            parseOperateEntry(doc,cur,(Operateentry *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"move")))
        {
            parseMoveEntry(doc,cur,(Moveentry *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getuserstate")))
        {
            parseGetUserState(doc,cur,(Getuserstate *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getpersonalsystemfolder")))
        {
            parseGetPersonalSystemFolder(doc,cur,(Getpersonalsystemfolder *)obj);
        }

        cur = cur->next;
    }
    xmlFreeDoc(doc);

    return 0;
}


int sendBrowseRequest(char *filename,char *url,char *postdata,char *cookie,
                      char *header,struct MemoryStruct *chunk)
{
    FILE *fd;
    CURL *curl;
    CURLcode res = CURLE_FAILED_INIT;
    char cookies[NORMALSIZE];
    char err_message[NORMALSIZE] = {0};
    struct curl_slist *headers = NULL;

    fd = fopen(filename,"w");
    if(NULL == fd)
    {
        printf("open %s file fail\n",filename);
        return -1;
    }

    curl = curl_easy_init();
    curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,err_message);
    curl_easy_setopt(curl,CURLOPT_FAILONERROR,1);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);

    if(cookie != NULL)
    {
        curl_easy_setopt(curl,CURLOPT_COOKIE,cookie);
    }
    else
    {
        snprintf(cookies,NORMALSIZE,"OMNISTORE_VER=1_0; path=/;sid=%s;",sid);
        curl_easy_setopt(curl,CURLOPT_COOKIE,cookies);
    }

    if(header != NULL)
    {
        headers = curl_slist_append(headers,header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headers);
    }
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    if(postdata != NULL)
    {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postdata);
    }
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);

    if(!exit_loop)
        res = curl_easy_perform(curl);
    if( res != CURLE_OK )
    {
        printf("error message is %s \n",err_message);
        if(chunk->memory)
            free(chunk->memory);
    }

    if(header)
        curl_slist_free_all(headers);

    curl_easy_cleanup(curl);

    fclose(fd);

      return res;
}

void print_count_call_api()
{
    printf("**************\n");
    printf("print count call api start\n");
    printf("requestservicegateway=%d\n",api_count.requestservicegateway);
    printf("acquiretoken=%d\n",api_count.acquiretoken);
    printf("getinfo=%d\n",api_count.getinfo);
    printf("getmysyncfolder=%d\n",api_count.getmysyncfolder);
    printf("getpersonalsystemfolder=%d\n",api_count.getpersonalsystemfolder);
    printf("getuserstate=%d\n",api_count.getuserstate);
    printf("browse=%d\n",api_count.browse);
    printf("profind=%d\n",api_count.propfind);
    printf("create=%d\n",api_count.create);
    printf("rename=%d\n",api_count.rename);
    printf("remove=%d\n",api_count.remove);
    printf("move=%d\n",api_count.move);
    printf("getentryinfo=%d\n",api_count.getentryinfo);
    printf("getchangeseq=%d\n",api_count.getchangeseq);
    printf("initbinaryupload=%d\n",api_count.initbinaryupload);
    printf("resumebinaryupload=%d\n",api_count.resumebinaryupload);
    printf("finishbinaryupload=%d\n",api_count.finishbinaryupload);
    printf("directdownload=%d\n",api_count.directdownload);
    printf("**************\n");
}

int count_call_api(const char *url)
{
    if(strstr(url,"requestservicegateway"))
        api_count.requestservicegateway++;
    else if(strstr(url,"acquiretoken"))
        api_count.acquiretoken++;
    else if(strstr(url,"getinfo"))
        api_count.getinfo++;
    else if(strstr(url,"getmysyncfolder"))
        api_count.getmysyncfolder++;
    else if(strstr(url,"getpersonalsystemfolder"))
        api_count.getpersonalsystemfolder++;
    else if(strstr(url,"getuserstate"))
        api_count.getuserstate++;
    else if(strstr(url,"browse"))
        api_count.browse++;
    else if(strstr(url,"propfind"))
        api_count.propfind++;
    else if(strstr(url,"create"))
        api_count.create++;
    else if(strstr(url,"rename"))
        api_count.rename++;
    else if(strstr(url,"remove"))
        api_count.remove++;
    else if(strstr(url,"move"))
        api_count.move++;
    else if(strstr(url,"getentryinfo"))
        api_count.getentryinfo++;
    else if(strstr(url,"getchangeseq"))
        api_count.getchangeseq++;
    else if(strstr(url,"initbinaryupload"))
        api_count.initbinaryupload++;
    else if(strstr(url,"resumebinaryupload"))
        api_count.resumebinaryupload++;
    else if(strstr(url,"finishbinaryupload"))
        api_count.finishbinaryupload++;
    else if(strstr(url,"directdownload"))
        api_count.directdownload++;

    return 0;
}



int sendRequest(char *filename,char *url,char *postdata,char *cookie,char *header)
{
    FILE *fd;
    CURL *curl;
    CURLcode res = CURLE_FAILED_INIT;
    char cookies[NORMALSIZE];
    char err_message[NORMALSIZE] = {0};
    struct curl_slist *headers = NULL;

    fd = fopen(filename,"w");
    if(NULL == fd)
    {
        printf("open %s file fail\n",filename);
        return -1;
    }

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    if(cookie != NULL)
    {
        curl_easy_setopt(curl,CURLOPT_COOKIE,cookie);
    }
    else
    {
        snprintf(cookies,NORMALSIZE,"OMNISTORE_VER=1_0; path=/;sid=%s;v=%s",sid,VERSION);
        curl_easy_setopt(curl,CURLOPT_COOKIE,cookies);
    }

    if(header != NULL)
    {
        headers = curl_slist_append(headers,header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headers);
    }

    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fd);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    if(postdata != NULL)
    {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postdata);
    }

    curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);

    if(!exit_loop)
        res = curl_easy_perform(curl);

    if( res != CURLE_OK )
    {
        printf("error message is %s \n",err_message);
    }
	 else
        count_call_api(url);

    if(header)
        curl_slist_free_all(headers);

    curl_easy_cleanup(curl);

    fclose(fd);

      return res;
}

int getServiceGateway(char *username, char *password,Servicegateway *sg)
{
    int status;
    memset(sg,0,sizeof(Servicegateway));

    char *url = "https://cloudsyncportal01.asuswebstorage.com/member/requestservicegateway/"; //20150602 magic changed
    char postdata[512];

    snprintf(postdata,512,"<requestservicegateway><userid>%s</userid><password>%s</password><language>zh_TW</language><service>1</service><time>2008/1/1</time></requestservicegateway>",username,password);
    status = sendRequest(gateway_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return S_NETWORK_FAIL;
    }

    if( parseDoc1(gateway_xml,sg) == -1)
    {
        return S_NETWORK_FAIL;
    }

   return 0;
}

int obtainGateway(char *user,char *pwd,Servicegateway *sg)
{
    int GetGateOK = 0;
    int status = -1;

    while(GetGateOK != 1 && exit_loop != 1)
    {

        status = getServiceGateway(user,pwd,sg);//fill Service_geteway struct member info

        if(status == S_NETWORK_FAIL)
        {
           enter_sleep_time(1000*500,NULL);
           check_network_state();
           continue;
        }

        status = sg->status;

        if(status == S_AUTH_FAIL)
        {
            write_log(S_ERROR,"Authentication Failed","");
            return -1;
        }

        if( status != 0 )
        {
            handle_error(status,"gateway");
            enter_sleep_time(1000*500,NULL);
            continue;
        }

        GetGateOK = 1;
    }

    return 0;
}

char *makeAuthorize()
{
    char *header = NULL;
    char header_signature_method[64];
    char header_timestamp[64];
    char header_nonce[64];
    char *header_signature = NULL;
    char prekey[128];
    unsigned long int sec;

    char query_string[1024];
    char *incode_string = NULL;
    char *sha1_string = NULL;

    snprintf(header_signature_method,64,"%s","HMAC-SHA1");
    snprintf(prekey,128,"%s","03805FDC4B594FDEA89183D2ADA82586"); //2015.5.22 111C243AC3224439A5C619423B39F7AF -> 03805FDC4B594FDEA89183D2ADA82586

    sec = time((time_t *)NULL);
    snprintf(header_timestamp,64,"%lu",sec);
    snprintf(header_nonce,64,"%lu",sec);

    snprintf( query_string,1024,"nonce=%s&signature_method=%s&timestamp=%s",header_nonce,header_signature_method,header_timestamp);
    incode_string = oauth_url_escape(query_string);
    if(NULL == incode_string)
    {
        handle_error(S_URL_ESCAPE_FAIL,"makeAuthorize");
        return NULL;
    }

    sha1_string = oauth_sign_hmac_sha1(incode_string,prekey);

    if(NULL == sha1_string)
    {
        handle_error(S_SHA1_FAIL,"makeAuthorize");
        my_free(incode_string);
        return NULL;
    }

    header_signature = oauth_url_escape(sha1_string);
    if(NULL == header_signature)
    {
        handle_error(S_URL_ESCAPE_FAIL,"makeAuthorize");
        my_free(incode_string);
        my_free(sha1_string);
        return NULL;
    }

    header = (char *)malloc(sizeof(char*)*1024);

    if(header == NULL)
    {
        my_free(incode_string);
        my_free(sha1_string);
        my_free(header_signature);
        return NULL;
    }

    snprintf(header,1024,"Authorization:signature_method=\"%s\",timestamp=\"%s\",nonce=%s,signature=\"%s\"",header_signature_method,header_timestamp,header_nonce,header_signature);

    my_free(incode_string);
    my_free(sha1_string);
    my_free(header_signature);

    return header;
}



int getToken(char *username, char *password,char *key,int first)
{
    int status;
    char url[256];
    char postdata[512];
    char *header;
    char cookie[512];
    int error_type;
    char msg[512] = {0};
    char filename[512] = {0};
    memset(&aaa,0,sizeof(Aaa));

    header = makeAuthorize();

    snprintf(url,256,"https://%s/member/acquiretoken/",sergate.gateway);
    snprintf(cookie,512,"OMNISTORE_VER=1_0; path=/;sid=%s;",sid);
    snprintf(postdata,512,"<aaa><userid>%s</userid><password>%s</password><time>2008/1/1</time><auxpassword>%s</auxpassword></aaa>",username,password,key);

    status = sendRequest(token_xml,url,postdata,cookie,header);
    my_free(header);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return S_NETWORK_FAIL;
    }


#if 1
    if( parseDoc1(token_xml,&aaa) == -1)
        return S_NETWORK_FAIL;

    if(aaa.status != 0 )
    {
       handle_error(aaa.status,"token");

       status = aaa.status;
       error_type = S_ERROR;

       if(status == S_AUTH_FAIL)
       {
           snprintf(msg, 512, "%s", "Authentication Failed");
       }
       else if(status == S_OTP_AUTH_FAIL)
       {
           if(first)
           {
               if(strlen(cfg.otp_key)>0)
               {
                   if(strlen(cfg.captcha)>0)
                       return status;            // when need otp and captcha no write log 13/11/1
                   else
                        strncpy(msg,"Security Code error.WARNING:If the OTP authentication "
                              "Failures reaches 10 times,OTP will be locked,you must visit the "
                              "official website ,OTP can be used again after unlock it",512);
               }
               else
                   snprintf(msg, 512, "%s", "OTP athentication failed.Please input Security Code");
           }
           else
               snprintf(msg, 512, "%s", "Security Code has expired, please re-enter");

       }
       else if(status == S_OTP_ID_LOCKED)
       {
           strncpy(msg,"OTP Services Credential ID is LOCKED state,you must visit "
                  "the official website can be used again after unlock OTP services",512);
       }
       else if(status == S_CAPTCHA_FAIL)
       {
           error_type = S_NEEDCAPTCHA;
           snprintf(msg, 512, "%s", "Need to enter the CAPTCHA");
           snprintf(filename, 512, "%s", aaa.auxpasswordurl);
       }


       write_log(error_type,msg,filename);

       return aaa.status;
    }
#endif

    return 0;

}

int record_system_token(char *filename,Aaa *aa)
{
    FILE *fp = NULL;
    int len;

    fp = fopen(filename,"wb");
    if(fp == NULL)
    {
        printf("open %s fail\n",filename);
        return -1;
    }

    strncpy(aa->gateway,sergate.gateway,MINSIZE);
    strncpy(aa->user,username,256);
    strncpy(aa->pwd,password,256);
    len = fwrite(aa,sizeof(Aaa),1,fp);
    fclose(fp);

    return 0;
}

int obtainToken(char *user,char *pwd,struct asus_config *cfg,int first)
{
    int error_time = 0;
    int have_error_log = 0;
    char temp_key[8] = {0};
    int  otp_and_captcha = 0;
    int status = -1;

    if(strlen(cfg->captcha) > 0)
    {
        snprintf(temp_key, 8, "%s", cfg->captcha);
        if(strlen(cfg->otp_key) >0)
            otp_and_captcha = 1;
    }
    else if(strlen(cfg->otp_key) > 0)
            snprintf(temp_key, 8, "%s", cfg->otp_key);

    while(exit_loop != 1)
    {
        if(error_time > 5 && have_error_log != 1)
        {
            write_log(S_ERROR,"Can not get server information",""); //modify by gauss 2014/3/10
            have_error_log = 1;
        }

        status = getToken(user,pwd,temp_key,first);

        printf("token status=%d\n",status);

        if(status !=0)
        {
            if(status == S_NETWORK_FAIL)
            {
                error_time++;
                enter_sleep_time(1000*300,NULL);
                check_network_state();
                continue;
            }
            else
            {
#ifdef IPKG
                char fullname[256] = {0};
                snprintf(fullname, 256, "%s/%s",mount_path,token_filename);
                remove(fullname);
                remove(record_token_file);
#endif
                if(otp_and_captcha == 1 && status == S_OTP_AUTH_FAIL)
                {
#ifdef DEBUG
                    printf("need captcha and otp\n");
#endif
                    memset(temp_key,0,sizeof(temp_key));
                    snprintf(temp_key, 8, "%s", cfg->otp_key);
                    otp_and_captcha = 0;
                    continue;
                }

                if(status == S_AUTH_FAIL || status == S_OTP_AUTH_FAIL || status == S_OTP_ID_LOCKED ||
                   status == S_CAPTCHA_FAIL)
                    return -1;
                else // other error code ,such as 999
                {
                    enter_sleep_time(1000*300,NULL);
                    continue;
                }
            }
        }

        record_system_token(system_token,&aaa);
        break;
    }

    return 0;
}

Getinfo *getInfo(char *username,char *server)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char timestamp[MINSIZE];
    int status;
    int sec;
    Getinfo *gi;

    gi = getb(Getinfo);

    if(NULL == gi)
    {
        printf("create dynamic memory fail\n");
        return NULL;
    }

    memset(gi,0,sizeof(Getinfo));

    sec = time((time_t *)NULL);

    snprintf(timestamp,MINSIZE,"%d",sec);
    snprintf(url,NORMALSIZE,"https://%s/member/getinfo/",server);
    snprintf(postdata,MAXSIZE,"<getinfo><token>%s</token><userid>%s</userid><time>2008/1/1</time></getinfo>"
            ,aaa.token,username);

    status = sendRequest(get_info_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gi);
        return NULL;
    }

    if( parseDoc1(get_info_xml,gi) == -1)
    {
        my_free(gi);
        return NULL;
    }
    
    return gi;
}

Getmysyncfolder *getMySyncFolder(char *username)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    int status;
    Getmysyncfolder *gf;
    gf = getb(Getmysyncfolder);

    if(NULL == gf)
        return NULL;

    memset(gf,0,sizeof(Getmysyncfolder));

    snprintf(url,NORMALSIZE,"https://%s/folder/getmysyncfolder/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getmysyncfolder><token>%s</token><userid>%s</userid></getmysyncfolder>"
            ,aaa.token,username);

    status = sendRequest(get_sync_folder_xml,url,postdata,NULL,NULL);
    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gf);
        return NULL;
    }

    if( parseDoc1(get_sync_folder_xml,gf) == -1)
    {
        my_free(gf);
            return NULL;
    }

    
    return gf;
}

long long int obtainSyncRootID(char *user)
{
    long long int id = -1;
    Getmysyncfolder *gf = NULL;

    while(exit_loop != 1)
    {
        gf = getMySyncFolder(user);

        if(NULL == gf)
        {
            enter_sleep_time(1000*300,NULL);
            check_network_state();
            continue;
        }

        if(gf->status == S_AUTH_FAIL)
        {
            my_free(gf);
            if(obtainToken(user,password,&cfg,0) == -1)
                return -1;
            else
                continue;
        }

        if(gf->status != 0)
        {
            handle_error(gf->status,"GetMySyncFolderID Fail");
            my_free(gf);
            enter_sleep_time(1000*300,NULL);
            continue;
        }

        id = gf->id;
        my_free(gf);

        break;
    }

    return id;
}

Getpersonalsystemfolder *getPersonalSystemFolder(char *username,char *filename)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    int status;
    Getpersonalsystemfolder *gp;
    gp = getb(Getpersonalsystemfolder);

    if(NULL == gp)
        return NULL;

    memset(gp,0,sizeof(Getpersonalsystemfolder));

    snprintf(url,NORMALSIZE,"https://%s/folder/getpersonalsystemfolder/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getpersonalsystemfolder><token>%s</token><userid>%s</userid><rawfoldername>%s</rawfoldername></getpersonalsystemfolder>"
            ,aaa.token,username,filename);

    status = sendRequest(get_personal_system_folder_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gp);
        return NULL;
    }

    if( parseDoc1(get_personal_system_folder_xml,gp) == -1)
    {
        my_free(gp);
        return NULL;
    }

    return gp;

}

long long int GetMyRecycleID(char *username,char *filename)
{
    Getpersonalsystemfolder *gp;
    long long int id = -10;

    while(!exit_loop)
    {
        gp = getPersonalSystemFolder(username,filename);

        if(NULL == gp)
        {
            enter_sleep_time(1000*300,NULL);
            check_network_state();
            continue;
        }

        if(gp->status == S_AUTH_FAIL)
        {
            my_free(gp);
            if(obtainToken(username,password,&cfg,0) == -1)
                return -1;
            else
                continue;
        }

        if(gp->status != 0)
        {
            handle_error(gp->status,"getpersonalsystemfolder");
            my_free(gp);
            enter_sleep_time(1000*300,NULL);
            continue;
        }

        id = gp->folderid;
        my_free(gp);

        break;
    }

    return id;
}

Getuserstate *getUserState(char *user,char *server)
{
    Getuserstate *gu;
    int status;
    char url[256] = {0};
    char postdata[256] = {0};
    char cookie[32] = {0};

    gu = getb(Getuserstate);

    if(NULL == gu)
    {
        printf("create dynamic memory fail\n");
        return NULL;
    }

    memset(gu,0,sizeof(Getuserstate));

    snprintf(cookie,32,"OMNISTORE_VER=1_0; path=/;");
    snprintf(url,256,"https://%s/member/getuserstate/",server);
    snprintf(postdata,256,"<getuserstate><userid>%s</userid><serviceid>1</serviceid></getuserstate>",user);

    status = sendRequest(get_user_state_xml,url,postdata,cookie,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gu);
        return NULL;
    }

    if( parseDoc1(get_user_state_xml,gu) == -1)
    {
        my_free(gu);
        return NULL;
    }

    return gu;
}

int CheckUserState(char *user,char *server)
{
    int status;
    Getuserstate *gu;
    int result = 0;

    while(!exit_loop)
    {
      gu = getUserState(user,server);
      if(gu == NULL)
      {
          enter_sleep_time(1000*500,NULL);
          check_network_state();
          continue;
      }

      if(gu->status == S_AUTH_FAIL)
      {
          my_free(gu);
          if(obtainToken(user,password,&cfg,0) == -1)
              return -1;
          else
              continue;
      }

      if(gu->status != 0)
      {
          status = gu->status;
          handle_error(status,"getUserState");
          my_free(gu);
          enter_sleep_time(1000*500,NULL);
          continue;
      }

#ifdef DEBUG
      printf("userstate=%d\n",gu->userstate);
#endif

      if(gu->userstate & FROZEN)   //accout state is FROZEN;
      {
          write_log(S_ERROR,"Your accout is frozen,you can't upload file","");
          result = S_ACCOUNT_FROZEN;
      }
      else if(gu->userstate & CLOSE)
      {
          write_log(S_ERROR,"Your accout is close,please active it","");
          result = S_ACCOUNT_CLOSE;
      }
      my_free(gu);

      break;
    }

    return result;

}

#if 0
/* browse file list by page andt merge list*/
Browse *linkBrowseList(Browse *all,Browse *item)
{
    Browse *temp = NULL;
    File **f1;
    Folder **fd1;
    int i;
    int index,t;

    if(all == NULL || item == NULL)
    {
        printf("pass point is null\n");
        return NULL;
    }

    temp = (Browse *)calloc(1,sizeof(Browse));
    if(temp == NULL)
    {
        printf("realloc browse fail\n");
        return NULL;
    }
    {

        t = all->filenumber + item->filenumber;
        if(t>0)
        {
            f1 = (File **)calloc(t,sizeof(File *));
            if(f1 == NULL)
            {
                printf("realloc browse fail\n");
                my_free(temp);
                return NULL;
            }
            for(i=0;i<all->filenumber;i++)
            {
                f1[i] = all->filelist[i];
            }
            index = all->filenumber;
            for(i=0;i<item->filenumber;i++,index++)
            {
                f1[index] = item->filelist[i];
            }
            temp->filelist = f1;
            temp->filenumber = t;
        }
    }
    {
        t = all->foldernumber + item->foldernumber;
        if(t>0)
        {
            fd1 = (Folder **)calloc(t,sizeof(Folder *));
            if(fd1 == NULL)
            {
                printf("realloc browse fail\n");
                my_free(temp->filelist);
                my_free(temp);
                return NULL;
            }
            for(i=0;i<all->foldernumber;i++)
            {
                fd1[i] = all->folderlist[i];
            }
            index = all->foldernumber;
            for(i=0;i<item->foldernumber;i++,index++)
            {
                fd1[index] = item->folderlist[i];
            }
            temp->folderlist = fd1;
            temp->foldernumber = t;
        }


    }

    return temp;
}

Browse *GetServerList(char *username,long long int id,int issibiling)
{
    int pageno = 1 ;
    Browse *cur = NULL,*item = NULL,*temp;
    int haspage = 0;

    while(1)
    {
        item = browseFolder(username,id,issibiling,pageno);
        if(item == NULL)
        {
            check_network_state();
            enter_sleep_time(1000*300,NULL);
            continue;
        }

        if(item->status == 2)
        {
            handle_error(item->status,"browsefolder auth fail");
            free_server_list(item);
            free_server_list(cur);
            return NULL;
        }

        if(item->status != 0)
        {
            handle_error(item->status,"browsefolder");
            free_server_list(item);
            enter_sleep_time(1000*300,NULL);
            continue;
        }
        haspage = item->page.hasnextpage;
        if(pageno > 1)
        {
            temp = linkBrowseList(cur,item);
            if(temp == NULL)
            {
                free_server_list(item);
                free_server_list(cur);
                return NULL;
            }
            my_free(item->filelist);
            my_free(item->folderlist);
            my_free(item);
            my_free(cur->filelist);
            my_free(cur->folderlist);
            my_free(cur);
            cur = temp;
        }
        else
        {
           cur = item;
        }
        if(haspage)
            pageno++;
        else
            break;

    }

    if(pageno>1)
        printf("filenum=%d,foldernum=%d\n",cur->filenumber,cur->foldernumber);
    return cur;
}
#endif

long long int get_max_upload_filesize(char *username)
{
    Getinfo *gi = NULL;
    long long int filesize = 0;

    while(!exit_loop)
    {
        gi = getInfo(username,sergate.gateway);

        if(NULL == gi)
        {
            enter_sleep_time(1000*300,NULL);
            check_network_state();
            continue;
        }

        if(gi->status == S_AUTH_FAIL)
        {
            my_free(gi);
            if(obtainToken(username,password,&cfg,0) == -1)
                return -1;
            else
                continue;
        }

        if(gi->status != 0)
        {
            handle_error(gi->status,"getinfo");
            my_free(gi);
            enter_sleep_time(1000*300,NULL);
            continue;
        }

        filesize = gi->package.maxfilesize;
        my_free(gi);

        break;
    }

    return filesize;
}

int record_folder_id(Browse *br,long long int pid)
{
    int i;
    long long int folderid;
    Changeseq *cs = NULL;

    if(pre_seq > 0)
        return 0;

    for(i=0;i<br->foldernumber;i++)
    {
       folderid = br->folderlist[i]->id;
       printf("folderid=%lld\n",folderid);
       while( (cs = getChangeSeq(folderid)) == NULL)
       {
           enter_sleep_time(5,&wait_server_mutex);
       }
       if(cs->status != 0)
       {
          handle_error(cs->status,"get changeseq in record_folder_id");
          my_free(cs);
          return -1;
       }
       else
       {
            insert_node(pid,folderid,cs->changeseq);
            my_free(cs);
       }
    }
    return 0;
}

Browse *browseFolder(char *username,long long int id,int issibiling,int pageno)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char itemofpage[NORMALSIZE];   
    Browse *browse = getb(Browse);

    if( NULL == browse )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(browse,0,sizeof(Browse));

    snprintf(url,NORMALSIZE,"https://%s/folder/browse/",aaa.inforelay);
    snprintf(itemofpage,NORMALSIZE,"<page><pageno>%d</pageno><pagesize>200</pagesize><enable>0</enable></page>",pageno);
    snprintf(postdata,MAXSIZE,"<browse><token>%s</token><language>zh_TW</language><userid>%s</userid><folderid>%lld</folderid>%s<issibiling>%d</issibiling></browse>"
            ,aaa.token,username,id,itemofpage,issibiling);

    status = sendRequest(browse_folder_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(browse);
        return NULL;
    }

      if(my_parse_browse_xml(browse_folder_xml,browse) == -1)
      {
          my_free(browse);
          return NULL;
      }
		 else
    {
        if(!upload_only)
            record_folder_id(browse,id);
        return browse;
    }

      return browse;
}

Propfind *checkEntryExisted(char *userid,long long int parentID,char *filename,char *type)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char entryName[NORMALSIZE];
    int status;
    char *encode = NULL;

    memset(url,0,sizeof(url));
    memset(postdata,0,sizeof(postdata));
    memset(entryName,0,sizeof(entryName));

    Propfind *find = getb(Propfind);

    if( NULL == find )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(find,0,sizeof(Propfind));

    snprintf(entryName, NORMALSIZE, "%s", filename);
    encode = oauth_encode_base64(0,(const unsigned char *)entryName);

    if(NULL == encode)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"checkEntryExisted");
        my_free(find);
        return NULL;
    }

    snprintf(url,NORMALSIZE,"https://%s/find/propfind/",aaa.inforelay);

    snprintf(postdata,MAXSIZE,"<propfind><token>%s</token><scrip></scrip><userid>%s</userid><parent>%lld</parent><find>%s</find><type>%s</type></propfind>"
            ,aaa.token,userid,parentID,encode,type);

    status = sendRequest(propfind_xml,url,postdata,NULL,NULL);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(find);
        return NULL;
    }


    if(parseDoc1(propfind_xml,find) == -1)
    {
        my_free(find);
        return NULL;

    }
    return find;
}

Createfolder *createFolder(char *username,long long int parentID,int isencrpted,char *name)
{
    add_sync_item("create_folder_fail",name,up_excep_fail);

    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char at[MAXSIZE];
    unsigned long int sec;
    char timestamp[MINSIZE];
    char *entryName =  NULL;
    char *encode = NULL;
    Createfolder *cf = getb(Createfolder);

    if( NULL == cf )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(cf,0,sizeof(Createfolder));

    entryName = parse_name_from_path(name);

    if(entryName == NULL)
    {
    	my_free(cf);
        printf("obtain name fail by parse path");
        return NULL;
    }

    encode = oauth_encode_base64(0,(const unsigned char *)entryName);
    my_free(entryName);

    if(NULL == encode)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"createFolder");
        my_free(cf);
        return NULL;
    }

    sec = time((time_t *)NULL);
    snprintf(timestamp,MINSIZE,"%lu",sec);
    snprintf(at,MAXSIZE,"<creationtime>%s</creationtime><lastaccesstime>%s</lastaccesstime><lastwritetime>%s</lastwritetime>"
            ,timestamp,timestamp,timestamp);
    snprintf(url,NORMALSIZE,"https://%s/folder/create/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<create><token>%s</token><userid>%s</userid><parent>%lld</parent><isencrypted>%d</isencrypted><display>%s</display><attribute>%s</attribute></create>"
            ,aaa.token,username,parentID,isencrpted,encode,at);

    status = sendRequest(create_folder_xml,url,postdata,NULL,NULL);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(cf);
        return NULL;
    }

    if(parseDoc1(create_folder_xml,cf) == -1)
    {
        my_free(cf);
        return NULL;

    }
    else
    {
        del_sync_item("create_folder_fail",name,up_excep_fail);
        if(cf->status == 0)
        {
            Changeseq *cs = NULL;
            int seq = 0;
            while( (cs = getChangeSeq(cf->id)) == NULL)
            {
               enter_sleep_time(5,&wait_server_mutex);
            }
            if(cs->status == 0)
                seq = cs->changeseq;
            my_free(cs);
            insert_node(parentID,cf->id,seq);
        }
        return cf;
    }
}

Operateentry *renameEntry(char *username,long long int id,int isencrpted,char *newname,int isfolder)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char type[MINSIZE];
    char *entryName = NULL;
    Operateentry *oe = getb(Operateentry);

    if( NULL == oe )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(oe,0,sizeof(Operateentry));

    if(isfolder)
    {
        snprintf(type, MINSIZE, "%s", "folder");
    }
    else
    {
        snprintf(type, MINSIZE, "%s", "file");
    }

    entryName = oauth_encode_base64(0,(const unsigned char *)newname);

    if(NULL == entryName)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"renameEntry");
        my_free(oe);
        return NULL;
    }

    snprintf(url,NORMALSIZE,"https://%s/%s/rename/",aaa.inforelay,type);
    snprintf(postdata,MAXSIZE,"<rename><token>%s</token><userid>%s</userid><id>%lld</id><isencrypted>%d</isencrypted><display>%s</display></rename>"
            ,aaa.token,username,id,isencrpted,entryName);

    my_free(entryName);

    status = sendRequest(rename_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(oe);
        return NULL;
    }


    if(parseDoc1(rename_xml,oe) == -1)
    {
        my_free(oe);
        return NULL;
    }

    return oe;
}

Moveentry *moveEntry(char *username,long long int id,char *name,long long int parentID,int isfolder,long long int pre_pid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char type[MINSIZE];
    char entryName[NORMALSIZE];
    char *encode = NULL;
    Moveentry *me = getb(Moveentry);

    if( NULL == me )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(me,0,sizeof(Moveentry));

    if(isfolder)
    {
        snprintf(type, MINSIZE, "%s", "folder");
    }
    else
    {
        snprintf(type, MINSIZE, "%s", "file");
    }

    if( NULL == name)
    {
        snprintf(entryName, NORMALSIZE, "%s", name);
        encode = oauth_encode_base64(0,(const unsigned char *)entryName);
        if(NULL == encode)
        {
            handle_error(S_ENCODE_BASE64_FAIL,"moveEntry");
            my_free(me);
            return NULL;
        }
        snprintf(postdata,MAXSIZE,"<move><token>%s</token><userid>%s</userid><id>%lld</id><display>%s</display><parent>%lld</parent></move>"
                ,aaa.token,username,id,encode,parentID);
        my_free(encode);
    }
    else
    {
        snprintf(entryName, NORMALSIZE, "%s", "");
        snprintf(postdata,MAXSIZE,"<move><token>%s</token><userid>%s</userid><id>%lld</id><display>%s</display><parent>%lld</parent></move>"
                ,aaa.token,username,id,entryName,parentID);
    }

    snprintf(url,NORMALSIZE,"https://%s/%s/move/",aaa.inforelay,type);

    status = sendRequest(move_xml,url,postdata,NULL,NULL);


    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(me);
        return NULL;
    }


    if(parseDoc1(move_xml,me) == -1)
    {
        my_free(me);
        return NULL;
    }
    else
    {
        if(isfolder)
        {
            move_node(pre_pid,id,parentID);
        }
        return me;
    }
}

Operateentry *removeEntry(char *username,long long int id,int ischildonly,int isfolder,long long int pid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char type[MINSIZE];
    Operateentry *oe = getb(Operateentry);

    if( NULL == oe )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(oe,0,sizeof(Operateentry));

    if(isfolder)
    {
        snprintf(type, MINSIZE, "%s", "folder");
        snprintf(postdata,MAXSIZE,"<remove><token>%s</token><userid>%s</userid><id>%lld</id><ischildonly>%d</ischildonly></remove>"
                ,aaa.token,username,id,ischildonly);

    }
    else
    {
        snprintf(type, MINSIZE, "%s", "file");
        snprintf(postdata,MAXSIZE,"<remove><token>%s</token><userid>%s</userid><id>%lld</id></remove>"
                ,aaa.token,username,id);
    }

    snprintf(url,NORMALSIZE,"https://%s/%s/remove/",aaa.inforelay,type);

    status = sendRequest(remove_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(oe);
        return NULL;
    }


    if(parseDoc1(remove_xml,oe) == -1)
    {
        my_free(oe);
        return NULL;

    }
    else
    {
        if(isfolder)
            del_node(pid,id);
        return oe;
    }
}

int updateEntryAttribute(char *username,long long int id,long long int parentID,int isencrpted,int isfolder)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char at[MAXSIZE];
    unsigned long int sec;
    char timestamp[MINSIZE];
    char type[MINSIZE];

    if(isfolder)
    {
        snprintf(type, MINSIZE, "%s", "folder");
        snprintf(postdata,MAXSIZE,"<updateattribute><token>%s</token><userid>%s</userid><folder>%lld</folder><parent>%lld</parent><isencrypted>%d</isencrypted><attribute>%s</attribute></updateattribute>"
                ,aaa.token,username,id,parentID,isencrpted,at);
    }
    else
    {
        snprintf(type, MINSIZE, "%s", "file");
        snprintf(postdata,MAXSIZE,"<updateattribute><token>%s</token><userid>%s</userid><folder>%lld</folder><attribute>%s</attribute></updateattribute>"
                ,aaa.token,username,id,at);
    }

    sec = time((time_t *)NULL);
    snprintf(timestamp,MINSIZE,"%lu",sec);
    snprintf(at,MAXSIZE,"<creationtime>%s</creationtime><lastaccesstime>%s</lastaccesstime><lastwritetime>%s</lastwritetime>"
            ,timestamp,timestamp,timestamp);
    snprintf(url,NORMALSIZE,"https://%s/%s/updateattribute/",aaa.inforelay,type);

    status = sendRequest(update_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

Getentryinfo *getEntryInfo(int isfolder,long long int entryid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    Getentryinfo *ge = getb(Getentryinfo);

    if( NULL == ge )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(ge,0,sizeof(Getentryinfo));

    snprintf(url,NORMALSIZE,"https://%s/fsentry/getentryinfo/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getentryinfo><token>%s</token><isfolder>%d</isfolder><entryid>%lld</entryid></getentryinfo>"
            ,aaa.token,isfolder,entryid);

    status = sendRequest(get_entry_info_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(ge);
        return NULL;
    }

    if(parseDoc1(get_entry_info_xml,ge) == -1)
    {
        my_free(ge);
        return NULL;

    }
    return ge;
}

#if 0
int getLatestChangeFiles(char *username,int top,int targetroot,int sortdirection)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/file/getlatestchangefiles/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getlatestchangefiles><userid>%s</userid><token>%s</token><top>%d</top><targetroot>%d</targetroot><sortdirection>%d</sortdirection></getlatestchangefiles>"
            ,username,aaa.token,top,targetroot,sortdirection);

    status = sendRequest(get_change_files_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int getLatestUploads(char *username,int top,int targetroot,int sortdirection)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/file/getlatestuploads/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getlatestuploads><userid>%s</userid><token>%s</token><top>%d</top><targetroot>%d</targetroot><sortdirection>%d</sortdirection></getlatestuploads>"
            ,username,aaa.token,top,targetroot,sortdirection);

    status = sendRequest(get_uploads_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int setEntryMark(int isfolder,long long int entryid,long long int markid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/setentrymark/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<setentrymark><token>%s</token><isfolder>%d</isfolder><entryid>%lld</entryid><markid>%lld</markid></setentrymark>"
            ,aaa.token,isfolder,entryid,markid);

    status = sendRequest(set_mark_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

int getShareCode(char *username,int entryType,long long int entryID,char *password,int actionType)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/getsharecode/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getsharecode><token>%s</token><script></script><userid>%s</userid><entrytype>%d</entrytype><entryid>%lld</entryid><password>%s</password><actiontype>%d</actiontype></getsharecode>"
            ,aaa.token,username,entryType,entryID,password,actionType);

    status = sendRequest(get_share_code_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int deleteShareCode(char *username,int entryType,long long int entryID,char *password)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/deletesharecode/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<deletesharecode><token>%s</token><script></script><userid>%s</userid><entrytype>%d</entrytype><entryid>%lld</entryid><password>%s</password></deletesharecode>"
            ,aaa.token,username,entryType,entryID,password);

    status = sendRequest(del_share_code_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

int getSharedEntries(char *username,int kind,int pagesize,int sortby,int sortdirection,char *firstentrybound)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/getsharedentries/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getsharedentries><token>%s</token><userid>%s</userid><kind>%d</kind><pagesize>%d</pagesize><sortby>%d</sortby><sortdirection>%d</sortdirection><firstentrybound>%s</firstentrybound></getsharedentries>"
            ,aaa.token,username,kind,pagesize,sortby,sortdirection,firstentrybound);

    status = sendRequest(get_share_entry_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

/*getSharedFiles API is nousing*/
#if 0
int getSharedFiles(char *username,int count)
{
    char *infofilename = "../../asuswebstorage/xml/aaa.xml";
    char *getsharedfiles_filename = "../../asuswebstorage/xml/getsharedfiles.xml";

    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    /*obtain token and inforelay*/
    parseDoc(infofilename);

    snprintf(url, NORMALSIZE, "https://%s/fsentry/getsharedfiles/",aaa.inforelay);
    snprintf(postdata, MAXSIZE, "<getsharedfiles><token>%s</token><userid>%s</userid><count>%d</count></getsharedfiles>"
            ,aaa.token,username,count);

    sendRequest(getsharedfiles_filename,url,postdata,NULL,NULL);

    return 0;
}
#endif

int checkPassword(char *username,char *suri)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/checkpassword/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<checkpassword><token>%s</token><userid>%s</userid><suri>%s</suri></checkpassword>"
            ,aaa.token,username,suri);

    status = sendRequest(check_pwd_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int comparePassword(char *username,int isfolder,long long int ffid,char *password)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/comparepassword/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<comparepassword><token>%s</token><userid>%s</userid><isfolder>%d</isfolder><ffid>%lld</ffid><passwd>%s</passwd></comparepassword>"
            ,aaa.token,username,isfolder,ffid,password);

    status = sendRequest(cmp_pwd_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}
#endif

Changeseq *getChangeSeq(long long int folderid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    Changeseq *cs = getb(Changeseq);

    if( NULL == cs )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(cs,0,sizeof(Changeseq));

    snprintf(url,NORMALSIZE,"https://%s/folder/getchangeseq/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getchangeseq><token>%s</token><scrip></scrip><folderid>%lld</folderid></getchangeseq>"
            ,aaa.token,folderid);
    status = sendRequest(get_change_seq_xml,url,postdata,NULL,NULL);
    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(cs);
        return NULL;
    }

    if(parseDoc1(get_change_seq_xml,cs) == -1)
    {
        my_free(cs);
        return NULL;
    }
    return cs;
}

/*encode file by sha512*/
int do_fp(FILE *f,char *checksum);
void pt(unsigned char *md,char*checksum);
int read(int, void *, unsigned int);
int do_fp(FILE *f,char *checksum)
{
    SHA512_CTX c;
    unsigned char md[SHA512_DIGEST_LENGTH];
    int fd;
    int i;
    unsigned char buf[BUFSIZE];

    fd=fileno(f);
    SHA512_Init(&c);
    for (;;)
    {
        if(exit_loop)
            return -1;
        i=read(fd,buf,BUFSIZE);
        if (i <= 0) break;
        SHA512_Update(&c,buf,(unsigned long)i);
    }
    SHA512_Final(&(md[0]),&c);
    pt(md,checksum);

    return 0;
}

void pt(unsigned char *md, char *checksum)
{
    int i;
    char temp[3];

    for (i=0; i<SHA512_DIGEST_LENGTH; i++)
    {
        memset(temp,0,sizeof(temp));
        snprintf(temp, 3, "%02x",md[i]);
        snprintf(checksum+strlen(checksum), NORMALSIZE-strlen(checksum), "%s", temp);
    }
}

int sha512(char *filename,char *checksum)
{
    FILE *IN;

    IN=fopen(filename,"r");
    if (IN == NULL)
    {
       printf("%s can't open \n",filename);
       return -1;
    }
    if(do_fp(IN,checksum) == -1)
    {
        fclose(IN);
        return -1;
    }
    fclose(IN);

    return 0;
}

int if_server_space_full(char *filename)
{
    struct stat filestat;
    long long int filesize;
    long long int server_free_capacity;

    if( stat(filename,&filestat) == -1)
    {
        printf("servr sapce full stat error:%s file not exist\n",filename);
        return -1;
    }

    filesize = filestat.st_size;

    while( (server_free_capacity = check_server_space(username)) == -1)
    {
        if(exit_loop)
            return -1;
        enter_sleep_time(1000*500,NULL);
        check_network_state();
    }

    if(server_free_capacity > 0)
    {
        if( filesize > server_free_capacity * 1024 *1024)
        {
            handle_error(S_SERVER_SPACE_FULL,"upload");
            return 1;
        }
    }

    return 0;
}

Initbinaryupload  *initBinaryUpload(char *filename,long long int parentID,char *transid,long long int fileID)
{


    int status;
    char url[MAXSIZE];
    char checksum[NORMALSIZE];
    char at[NORMALSIZE];
    char *at_encode = NULL;
    char fileCtime[MINSIZE];
    char fileAtime[MINSIZE];
    char fileMtime[MINSIZE];
    char name_raw[NORMALSIZE];
    char *finalname = NULL;
    Initbinaryupload *ibu = NULL;
    char *encode = NULL;

    memset(checksum,0,sizeof(checksum));

    struct stat filestat;
    long long int filesize;

    if( stat(filename,&filestat) == -1)
    {
        printf("InitBinaryUpload stat error:%s file not exist\n",filename);
        return NULL;
    }

    filesize = filestat.st_size;

    snprintf(fileCtime,MINSIZE,"%ld",filestat.st_ctime);
    snprintf(fileAtime,MINSIZE,"%ld",filestat.st_atime);
    snprintf(fileMtime,MINSIZE,"%ld",filestat.st_mtime);

    char *p = strrchr(filename,'/');

    if(NULL == p)
    {
        return NULL;
    }

    p++;
    snprintf(name_raw, NORMALSIZE, "%s", p);

    encode = oauth_encode_base64(0,(const unsigned char *)name_raw);

    if(NULL == encode)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"initBinaryUpload");
        return NULL;
    }

    finalname = oauth_url_escape(encode);
    my_free(encode);

    if(NULL == finalname)
    {
        handle_error(S_URL_ESCAPE_FAIL,"initBinaryUpload");
        return NULL;
    }

    snprintf(at,NORMALSIZE,"<creationtime>%s</creationtime><lastaccesstime>%s</lastaccesstime><lastwritetime>%s</lastwritetime>"
            ,fileCtime,fileAtime,fileMtime);

    at_encode = oauth_url_escape(at);

    if(NULL == at_encode)
    {
        handle_error(S_URL_ESCAPE_FAIL,"initBinaryUpload");
        my_free(finalname);
        return NULL;
    }

    if(sha512(filename,checksum) == -1)
    {
        printf("sha512 fail\n");
        my_free(finalname);
        my_free(at_encode);
        return NULL;
    }

    if(NULL == transid)
    {
        if(fileID == 0)
        {
            snprintf(url,MAXSIZE,"https://%s/webrelay/initbinaryupload/?tk=%s&pa=%lld&na=%s&at=%s&fs=%lld&sg=%s&sc=&dis=%s"
               ,aaa.webrelay,aaa.token,parentID,finalname,at_encode,filesize,checksum,sid);
        }
        else if(fileID > 0)
        {
            snprintf(url,MAXSIZE,"https://%s/webrelay/initbinaryupload/?tk=%s&pa=%lld&na=%s&at=%s&fi=%lld&fs=%lld&sg=%s&sc=&dis=%s"
               ,aaa.webrelay,aaa.token,parentID,finalname,at_encode,fileID,filesize,checksum,sid);
        }


    }
    else
    {
        if(fileID == 0)
        {
            snprintf(url,MAXSIZE,"https://%s/webrelay/initbinaryupload/?tk=%s&pa=%lld&na=%s&at=%s&fs=%lld&sg=%s&sc=&dis=%s&tx=%s"
                ,aaa.webrelay,aaa.token,parentID,finalname,at_encode,filesize,checksum,sid,transid);
        }
        else if(fileID > 0)
        {
            snprintf(url,MAXSIZE,"https://%s/webrelay/initbinaryupload/?tk=%s&pa=%lld&na=%s&at=%s&fi=%lld&fs=%lld&sg=%s&sc=&dis=%s&tx=%s"
                ,aaa.webrelay,aaa.token,parentID,finalname,at_encode,fileID,filesize,checksum,sid,transid);
        }
    }

    status = sendRequest(init_upload_xml,url,NULL,NULL,NULL);

    my_free(finalname);
    my_free(at_encode);

    if( status != 0 )
    {
        return NULL;
    }


    ibu = getb(Initbinaryupload);

    if( NULL == ibu )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(ibu,0,sizeof(Initbinaryupload));

    if(parseDoc1(init_upload_xml,ibu) == -1)
    {
        my_free(ibu);
        return NULL;
    }
    
   return ibu;

}


Resumebinaryupload *resumeBinaryUpload(char *filename, Initbinaryupload *ibu)
{
    FILE *fd;
    FILE *output;
    CURL *curl;
    CURLcode res;
    char cookies[NORMALSIZE];
    char url[NORMALSIZE];
    struct stat filestat;
    long long int filesize;
    Resumebinaryupload *rbu = NULL;
    char range[128];
    int offset;

    offset = ibu->offset;

    memset(range,0,sizeof(range));


    if( stat(filename,&filestat) == -1)
    {
        printf("Resumebinaryupload stat error:%s file not exist\n",filename);
        return NULL;
    }

    filesize = filestat.st_size;

#ifdef DEBUG
        printf("######filesize is %lld,offset is %d #####\n",filesize,offset);
#endif



#if 1
    snprintf(cookies,NORMALSIZE,"OMNISTORE_VER=1_0; path=/;sid=%s;v=%s",sid,VERSION);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/resumebinaryupload/?tk=%s&tx=%s&dis=%s"
            ,aaa.webrelay,aaa.token,ibu->transid,sid);


    if( NULL==(fd= fopen(resume_upload_xml,"w")) )
    {
        return NULL;
    }


    if( NULL==(output= fopen(filename,"rb")) )
    {
        fclose(fd);
        return NULL;
    }

    if(offset > 0)
        fseek(output,offset,0);

    curl = curl_easy_init();
#if 0
    curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
    curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);
#endif
    curl_easy_setopt(curl,CURLOPT_COOKIE,cookies);
    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fd);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl,CURLOPT_READFUNCTION,my_read_func);
    curl_easy_setopt(curl, CURLOPT_READDATA, output);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)filesize);

    if( offset > 0)
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE,(curl_off_t)offset);
#if 1
    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);

    start_time = time(NULL);
#endif
    res = curl_easy_perform(curl);

    if( res != 0 )
    {
        handle_error(res,"curl");
        fclose(fd);
        fclose(output);
        curl_easy_cleanup(curl);
        return NULL;
    }
    else
        count_call_api(url);

    curl_easy_cleanup(curl);

    fclose(fd);
    fclose(output);

    rbu = getb(Resumebinaryupload);

    if( NULL == rbu )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(rbu,0,sizeof(Resumebinaryupload));

    if(parseDoc1(resume_upload_xml,rbu) == -1)
    {
        my_free(rbu);
        return NULL;
    }
     return rbu;


#endif
}

Finishbinaryupload *finishBinaryUpload(Initbinaryupload *ibu)
{
    int status;
    char url[NORMALSIZE];
    Finishbinaryupload *fbu = NULL;

    if(strlen(ibu->latestchecksum) == 0)
    {
        snprintf(url,NORMALSIZE,"https://%s/webrelay/finishbinaryupload/?tk=%s&tx=%s&dis=%s"
                ,aaa.webrelay,aaa.token,ibu->transid,sid);
    }
    else
    {
        snprintf(url,NORMALSIZE,"https://%s/webrelay/finishbinaryupload/?tk=%s&tx=%s&dis=%s&lsg=%s"
                ,aaa.webrelay,aaa.token,ibu->transid,sid,ibu->latestchecksum);
    }

    status = sendRequest(finish_upload_xml,url,NULL,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return NULL;
    }

    fbu = getb(Finishbinaryupload);

    if( NULL == fbu )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(fbu,0,sizeof(Finishbinaryupload));

    if(parseDoc1(finish_upload_xml,fbu) == -1)
    {
        my_free(fbu);
        return NULL;
    }
    return fbu;
}

int is_local_file_newer(char *filename,long long int parentID,cmp_item_t *cmp,long long int fileID)
{
    /*check file is exist on server*/
    Propfind *find;
    Getentryinfo *getinfo;
    int server_mtime = 0 ;
    int local_mtime = 0;
    int local_atime = 0;
    int local_ctime = 0;
    struct stat filestat;
    char finalname[NORMALSIZE];
    long long int max_filesize;

    memset(&filestat,0,sizeof(struct stat));

    if( stat(filename,&filestat) == -1)
    {
        printf("is_local_file_newer stat error:%s file not exist\n",filename);
        return S_UPLOAD_DELETED;
    }

    max_filesize = max_upload_filesize * 1024 *1024;

    if(filestat.st_size > max_filesize)
    {
        printf("file size is %lld\n",(long long int)filestat.st_size);
        return S_FILE_TOO_LARGE;
    }

    if(fileID > 0) //server auto backup
    {
        cmp->status = new_file;
        return 0;
    }

    local_mtime = (int)filestat.st_mtime;
    local_atime = (int)filestat.st_atime;
    local_ctime = (int)filestat.st_ctime;

    cmp->size = filestat.st_size;

    char *p = strrchr(filename,'/');

    if(p)
    {
        p++;
        snprintf(finalname, NORMALSIZE, "%s", p);
    }


    find = checkEntryExisted(username,parentID,finalname,"system.file");

    if(NULL == find)
    {
        printf("find prop failed\n");
        return -1;
    }

    if( find->status != 0 )
    {
        handle_error(find->status,"propfind");
        my_free(find);
        return -1;
    }

    if( !strcmp(find->type,"system.file") )
    {
        cmp->id = find->id;
        my_free(find);

        getinfo = getEntryInfo(0,cmp->id);

        if(NULL == getinfo)
        {
            printf("get entry info failed\n");
            return -1;
        }
        else if( getinfo->status != 0 )
        {
            handle_error(getinfo->status,"getinfo");
            my_free(getinfo);
            return -1;
        }
        else
        {
             server_mtime = atoi(getinfo->attr.lastwritetime);
             my_free(getinfo);
              if(local_mtime > server_mtime)
                  cmp->status = newer;
              else if(local_mtime < server_mtime)
                  cmp->status = older;
              else
                   cmp->status = same;
         }
    }
    else
    {
        my_free(find);
    }

    if(server_mtime == 0)  //server not existed this file
        cmp->status = new_file;

    return 0;
}

int uploadFile(char *filename,long long int parentID,char *transid,long long int InFileID)
{
#ifdef DEBUG
    printf("#####upload %s is start,parentID=%lld,fileID=%lld#####\n",filename,parentID,InFileID);
#endif
    char action[128];
    char excep_action[128];
    char error_message[512];
    int res;
    int error_code = -10;
    server_space_full = 0;
    memset(action,0,sizeof(action));
    cmp_item_t c_item = {-2,-1,0};
    int check_res;
    int IsExistList = 0;
    long long int fileID = InFileID;
    char pre_name[256] = {0};

    snprintf(excep_action,128,"up_excep_fail");
    snprintf(action,128,"uploadfile,%lld,%s",parentID,transid);
    
    if(IsAccountFrozen)
    {
        check_res = CheckUserState(username,sergate.gateway);

        if(check_res == S_ACCOUNT_FROZEN)
        {
            add_sync_item(excep_action,filename,up_excep_fail);
            return 0;
        }            

        if(check_res == S_ACCOUNT_CLOSE)
        {
           exit_loop = 1;
           return 0;
        }

        if(check_res == S_AUTH_FAIL)
                return 0;

        IsAccountFrozen = 0;
    }

    check_res = is_local_file_newer(filename,parentID,&c_item,fileID);

    if(check_res != 0)
    {
        if(check_res == S_FILE_TOO_LARGE)
        {
           add_sync_item(excep_action,filename,up_excep_fail);
           snprintf(error_message,512,"%s filesize too large,max upload file size is %lldMB",
                    filename,max_upload_filesize);
#ifdef DEBUG
           printf("%s\n",error_message);
#endif
           write_log(S_ERROR,error_message,"");
        }

        return check_res;
    }

#ifdef DEBUG
    printf("compare status is %d\n",c_item.status);
#endif

    switch (c_item.status)
    {
    case older: // server file newer
    case newer: //local file newer         
    case same: //server not exist this file
        res = is_exist_case_conflicts(filename,pre_name);

        if(res == -1)
            return -1;

        if(res)
        {
            res = handle_rename(parentID,filename,createfile_action,NULL,1,pre_name);
            return res;
        }
        else
            fileID = c_item.id;
         break;
    case new_file:
         break;
    default:
         break;
    }

#ifdef DEBUG
    printf("fileID=%lld\n",fileID);
#endif

    res = if_server_space_full(filename);

    if(res == 1)
    {
        write_log(S_ERROR,"server space is not enough","");
        server_space_full = 1;
        return S_SERVER_SPACE_FULL;
    }
    else if(res == -1)
    {
        snprintf(error_message,512,"upload %s fail,file is not exist",filename);
        write_log(S_ERROR,error_message,"");
        return S_UPLOAD_DELETED;
    }

#if UP_QUEUE
    queue_entry_t  entry,entry2;

    entry = (queue_entry_t)malloc(sizeof(struct queue_entry));

    if(NULL == entry)
       return -1;
    entry->type = 2;
    entry->id = parentID;
    strcpy(entry->filename,filename);

    queue_enqueue(entry,queue_upload);
#endif
    write_log(S_UPLOAD,"",filename);
    snprintf(error_message,512,"upload %s fail\n",filename);
    Initbinaryupload *ibu;
    Resumebinaryupload *rbu;
    Finishbinaryupload *fbu;

    add_sync_item(excep_action,filename,up_excep_fail);
    res = add_sync_item(action,filename,up_head);
    if(res == -1)
        IsExistList = 1;

    ibu = initBinaryUpload(filename,parentID,transid,fileID);

    if( NULL == ibu)
    {
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Upload Fail");
        IsSyncError = 1;
        if(upload_only == 1)     //add for upload only mySync
                del_sync_item(excep_action,filename,up_excep_fail);
        return -1;
    }
    else if( ibu->status != 0  )
    {
        error_code = ibu->status;
        handle_error(ibu->status,"initbinaryupload");
        write_log(S_ERROR,error_message,"");

        switch(error_code)
        {
        case S_FILE_TOO_LARGE:
             del_sync_item(action,filename,up_head);
             break;
        case S_NAME_REPEAT:
             break;
        default:
             break;
        }
        
        my_free(ibu);

        return error_code;
    }

    if(ibu->fileid > 0)
    {
    	del_sync_item(excep_action,filename,up_excep_fail);
    	del_sync_item(action,filename,up_head);
#ifdef DEBUG
        printf("upload %s end!!!!\n",filename);
#endif
        my_free(ibu);
        return 0;
    }


    del_sync_item(action,filename,up_head);
    memset(action,0,sizeof(action));
    snprintf(action,128,"uploadfile,%lld,%s",parentID,ibu->transid);
    add_sync_item(action,filename,up_head);

    rbu = resumeBinaryUpload(filename,ibu);

    if( NULL == rbu)
    {
    	my_free(ibu);
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Upload Fail");
        IsSyncError = 1;
        if(upload_only == 1)   //add for upload only mySync
        	del_sync_item(excep_action,filename,up_excep_fail);
    	return -1;
    }

    else if( rbu->status !=0 )
    {
        error_code = rbu->status;
        handle_error(rbu->status,"resumebinaryupload");
        my_free(ibu);
        my_free(rbu);
        write_log(S_ERROR,error_message,"");
        return error_code;
    }

    fbu = finishBinaryUpload(ibu);

    if(NULL == fbu)
    {
    	my_free(ibu);
    	my_free(rbu);
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Upload Fail");
        IsSyncError = 1;
        if(upload_only == 1)    //add for upload only mySync
        	del_sync_item(excep_action,filename,up_excep_fail);
    	return -1;
    }

    else if(  fbu->status != 0 )
    {
        int res_value = fbu->status;
        handle_error(fbu->status,"finishbinaryupload");
        my_free(ibu);
        my_free(rbu);
        my_free(fbu);
        write_log(S_ERROR,error_message,"");
        return res_value;
    }

#if UP_QUEUE
    if(!queue_empty(queue_upload))
    {
        entry2 = queue_dequeue(queue_upload);
        free(entry2);
    }
#endif

    del_sync_item(action,filename,up_head);
    del_sync_item(excep_action,filename,up_excep_fail);

    my_free(ibu);
    my_free(rbu);
    my_free(fbu);

#ifdef DEBUG
    printf("upload %s end!!!!\n",filename);
#endif

    return 0;
}


int update_local_file_attr(Fileattribute *attr,char *filename)
{
   struct utimbuf tbuf;
   int server_mtime;
   int server_atime;

   /* change local file time according to server file */
       server_mtime = atoi(attr->lastwritetime);
       server_atime = atoi(attr->lastaccesstime);
       tbuf.actime = (time_t)server_atime;
       tbuf.modtime = (time_t)server_mtime;
       if(utime(filename,&tbuf) == -1)
       {
           printf("utime %s fail\n",filename);
           return -1;
       }

   return 0;
}

#if WRITE_DOWNLOAD_TEMP_FILE
int write_download_temp_file(const char *action,const char *name)
{
    FILE *fp;
    fp = fopen(down_item_temp_file,"w");
    if(NULL == fp)
    {
        printf("open %s fail",down_item_temp_file);
        return -1;
    }

    fprintf(fp,"%s,%s\n",action,name);
    fclose(fp);

    return 0;
}
#endif

int check_download_path_exist(const char *filename)
{
    char file_path[512];
    char *p = NULL;
    DIR *pDir = NULL;

    memset(file_path,0,sizeof(file_path));

    p = strrchr(filename,'/');

    if(NULL == p )
    {
        printf(" %s path is not exist\n",filename);
        return -1;
    }

    strncpy(file_path,filename,strlen(filename)-strlen(p));

    pDir = opendir(file_path);

    if(NULL == pDir)
    {
        return -1;
    }

    closedir(pDir);
    return 1;
}

char *get_temp_name(char *fullname)
{
    char *temp_name = NULL;
    char *temp_suffix = ".asus.td";
    int len = 0;
    char path[NORMALSIZE];
    char newfilename[NORMALSIZE];

    memset(path,0,sizeof(path));
    memset(newfilename,0,sizeof(newfilename));
    char *filename = NULL;
    filename =  parse_name_from_path(fullname);
    if(filename == NULL)
       return NULL;
    len = strlen(filename);
    if(len > 247)
    {
        strncpy(path,fullname,strlen(fullname)-len-1);
        strncpy(newfilename,filename,247);
        temp_name = (char *)malloc(sizeof(char)*(strlen(path)+strlen("/")+
                     strlen(newfilename)+strlen(temp_suffix)+1));
        memset(temp_name,0,sizeof(temp_name));
        snprintf(temp_name, sizeof(char)*(strlen(path)+strlen("/")+
                                          strlen(newfilename)+strlen(temp_suffix)+1), "%s/%s%s",path,newfilename,temp_suffix);
    }
    else
    {
        temp_name = (char *)malloc(sizeof(char)*(strlen(fullname)+strlen(temp_suffix)+1));
        memset(temp_name,0,sizeof(temp_name));
        snprintf(temp_name, sizeof(char)*(strlen(fullname)+strlen(temp_suffix)+1), "%s%s",fullname,temp_suffix);
    }

    my_free(filename);

    return temp_name;
}

int IsEntryDeletedFromServer(long long int fileID,int isfolder)
{
    Getentryinfo *ginfo = NULL;
    ginfo = getEntryInfo(isfolder,fileID);
    if(ginfo == NULL)
    {
        printf("getEntryInfo fail\n");
        return -1;
    }

    if(ginfo->status != 0)
    {

        if(ginfo->status == S_FILE_NOT_EXIST)
        {
#ifdef DEBUG
            printf("file has del from server\n");
#endif
            my_free(ginfo);
            return 1;
        }
        else
        {
            handle_error(ginfo->status,"getEntryInfo before download");
            my_free(ginfo);
            return -1;
        }
    }
    else
    {
        if(ginfo->parent == MyRecycleID)
        {
#ifdef DEBUG
            printf("file has put to Recycle\n");
#endif
            my_free(ginfo);
            return 1;
        }
        my_free(ginfo);
    }

    return 0;
}

int rename_download_file(char *temp_name,char *fullname,long long int fileID,Fileattribute *attr,int is_modify)
{

    if(access(fullname,0) == 0 && !is_modify)
    {
        char con_name[1024] = {0};
        char new_filename[256] = {0};
        char *name = NULL;
        Operateentry *oe = NULL;
        char tmp_name[1024] = {0};

        strncpy(tmp_name,fullname,1024);

        while(!exit_loop)
        {

            name = get_confilicted_name(tmp_name,0);

            if(name ==  NULL)
            {
                printf("handle_local_confilict_file fail\n");
                return -1;
            }
           if(access(name,F_OK) == 0)
           {
               memset(tmp_name,0,sizeof(tmp_name));
               snprintf(tmp_name,NORMALSIZE,"%s",name);
               my_free(name);
           }
           else
               break;
         }
 
        strncpy(con_name,name,1024);
        my_free(name);

        name = parse_name_from_path(con_name);
        strncpy(new_filename,name,256);
        my_free(name);

        while(!exit_loop)
        {
            oe = renameEntry(username,fileID,0,new_filename,0);
            if(oe == NULL)
            {
                check_network_state();
                continue;
            }
            if(oe->status != 0)
            {
                handle_error(oe->status,"renameEntry");
                my_free(oe);
                return -1;
            }

            my_free(oe);

            if(rename(temp_name,con_name) == -1)
            {
                printf("rename %s to %s fail",temp_name,fullname);
                return -1;
            }

            if( update_local_file_attr(attr,con_name) == -1)
                handle_error(S_UPDATE_ATTR_FAIL,"update file attr");

            break;
        }

    }
    else
    {
        if(rename(temp_name,fullname) == -1)
        {
            printf("rename %s to %s fail",temp_name,fullname);
            return -1;
        }

        if( update_local_file_attr(attr,fullname) == -1)
            handle_error(S_UPDATE_ATTR_FAIL,"update file attr");
    }

    return 0;
}

int downloadFile(long long int fileID,char *filename,long long int size,int ismodify,Fileattribute *attr)
{
#if DOWN_QUEUE
    queue_entry_t  entry,entry2;

    entry = (queue_entry_t)malloc(sizeof(struct queue_entry));

    if(NULL == entry)
        return  -1;
    entry->type = 1;
    entry->id = fileID;
    strcpy(entry->filename,filename);
    entry->size = size;

    queue_enqueue(entry,queue_download);
#endif
    local_space_full = 0;
    char *temp_name = NULL;

#ifdef DEBUG
    printf("download %s is start,fildID is %lld,size is %lld\n",filename,fileID,size);
#endif

    char action[256];
    long http_code = -10;

    memset(action,0,sizeof(action));
    snprintf(action,256,"downloadfile,%lld,%lld",fileID,size);
   
    char url[NORMALSIZE];
    CURL *curl;
    CURLcode res;
    FILE *fd;
    char cookies[NORMALSIZE];
    long long int disk_free_size;
    struct stat filestat;
    int file_exist =  -1;
    long long int local_file_len = -1;
    char error_message[NORMALSIZE];
    int status;
    int IsExistList = 0 ;

    status = IsEntryDeletedFromServer(fileID,0);

    if(status == 1)
        return 0;

    if(status == -1)
        return -1;

    disk_free_size = check_disk_space(sync_path);

    if( disk_free_size <= size )
    {
            handle_error(S_LOCAL_SPACE_FULL,"download");
            write_log(S_ERROR,"usb disk space is not enough","");
            local_space_full = 1;
            return -1;
    }

    temp_name = get_temp_name(filename);

    if(NULL == temp_name)
    {
        handle_error(S_MEMORY_FAIL,"get temp name");
        return -1;
    }

    status = add_sync_item(action,temp_name,down_head);
    if(status == -1)
        IsExistList = 1;

    if( stat(temp_name,&filestat) == -1)
    {
        file_exist = 0 ;
        local_file_len = 0;
    }
    else
    {
        file_exist = 1;
        local_file_len = filestat.st_size;
        if(filestat.st_size == size)
        {
            printf("exist download complete file\n");
            rename_download_file(temp_name,filename,fileID,attr,ismodify);
            del_sync_item(action,temp_name,down_head);
            my_free(temp_name);
            return 0;
        }
    }

#if WRITE_DOWNLOAD_TEMP_FILE
    write_download_temp_file(action,filename);
#endif
    write_log(S_DOWNLOAD,"",filename);

    if(file_exist && ismodify != 1 )
    {
       fd = fopen(temp_name,"ab");
    }
    else
    {
       fd = fopen(temp_name,"wb");
    }


    if(NULL == fd)
    {
        printf("fopen %s fail,dir is not exist??\n",filename);
        my_free(temp_name);
        return -1;
    }

    snprintf(cookies,NORMALSIZE,"OMNISTORE_VER=1_0; path=/;sid=%s;v=%s",sid,VERSION);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/directdownload/?tk=%s&fi=%lld&pv=0&u=&of=&rn=&dis=%s"
            ,aaa.webrelay,aaa.token,fileID,sid);
    curl = curl_easy_init();
#if 0
    curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
    curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);
#endif
    /* resume download file*/
    if(file_exist && ismodify != 1)
    	curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE,(curl_off_t)local_file_len);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "asuswebstorage-client/1.0.0.14.0");
    curl_easy_setopt(curl,CURLOPT_COOKIE,cookies);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, CA_INFO_FILE);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);
    curl_easy_setopt(curl,CURLOPT_READFUNCTION,my_read_func);
    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fd);

    res = curl_easy_perform(curl);

    if( res != 0 )
    {
        snprintf(error_message,NORMALSIZE,"download %s fail",filename);
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Download Fail");
        IsSyncError = 1;
        handle_error(res,"curl");
        fclose(fd);
        curl_easy_cleanup(curl);
        my_free(temp_name);
        return -1;
    }
    else
        count_call_api(url);


    fclose(fd);
#if 1
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_code);

#endif
    curl_easy_cleanup(curl);


#if DOWN_QUEUE
    if(!queue_empty(queue_download))
    {
        entry2 = queue_dequeue(queue_download);
        free(entry2);
    }
#endif

#if WRITE_DOWN_TEMP_FILE
    remove(down_item_temp_file);
#endif
    del_sync_item(action,temp_name,down_head);

#ifdef DEBUG
#endif

#ifdef DEBUG
    printf("download %s is end\n",filename);
#endif

    if(http_code > 300 && http_code < 600)
    {
        printf("download file http code is %ld\n",http_code);
        if(remove(temp_name) == -1)
        {
           printf("remove downlaod excep %s fail \n",temp_name);
        }
    }
    else
    {
        rename_download_file(temp_name,filename,fileID,attr,ismodify);
    }

    my_free(temp_name);
    return 0;
}

#if 0
int getResizedPhoto(long long int pfd,int st,int pv)
{
    int status;
    char url[NORMALSIZE];
    char *encode = NULL;
    char query_string[MAXSIZE];
    char query_raw[NORMALSIZE];
    char header[NORMALSIZE];

    snprintf(header,NORMALSIZE,"Last-Modified:Tue, 17 Nov 2009 07:13:19 GMT,ETag:\"1258441999687\"");
    snprintf(query_raw,NORMALSIZE,"pfd=%lld,st=%d,pv=%d",pfd,st,pv);
    encode = oauth_encode_base64(0,query_raw);

    if(encode == NULL)
   {
       handle_error(S_ENCODE_BASE64_FAIL,"getResizePhoto");
       return -1;
   }

    snprintf(query_string,MAXSIZE,"%s.jpg",encode);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/getresizedphoto/%s/%s?dis=%s&ecd=1"
            ,aaa.webrelay,aaa.token,query_string,sid);

    status = sendRequest(get_resize_photo_xml,url,NULL,NULL,header);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int getFullTextCompanion(long long int fi,int pv,int k)
{
    int status;
    char url[NORMALSIZE];
    char query_raw[NORMALSIZE];
    char *encode = NULL;
    char query_string[MAXSIZE];
    char header[NORMALSIZE];

    /*obtain token and inforelay*/
    snprintf(header,NORMALSIZE,"Last-Modified:Tue, 17 Nov 2009 07:13:19 GMT,ETag:\"1258441999687\"");
    snprintf(query_raw,NORMALSIZE,"fi=%lld,pv=%d,k=%d",fi,pv,k);
    encode = oauth_encode_base64(0,query_raw);

    if(encode == NULL)
   {
       handle_error(S_ENCODE_BASE64_FAIL,"getFullTextCompanion");
       return -1;
   }

   snprintf(query_string,MAXSIZE,"%s.txt",encode);
    snprintf(url,MAXSIZE,"https://%s/webrelay/getfulltextcompanion/%s/%s?dis=%s&ecd=1"
            ,aaa.webrelay,aaa.token,query_string,sid);

    status = sendRequest(get_full_txt_xml,url,NULL,NULL,header);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int getVideoSnapshot(long long int fi,int pv)
{
    int status;
    char url[NORMALSIZE];
    char query_raw[NORMALSIZE];
    char query_string[MAXSIZE];
    char *encode = NULL;

    snprintf(query_raw,NORMALSIZE,"fi=%lld,pv=%d",fi,pv);
    encode = oauth_encode_base64(0,query_raw);

    if(encode == NULL)
   {
       handle_error(S_ENCODE_BASE64_FAIL,"getVideoSnapshot");
       return -1;
   }
    snprintf(query_string,MAXSIZE,"%s.jpg",encode);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/getvideosnapshot/%s/%s?dis=%s&ecd=1"
            ,aaa.webrelay,aaa.token,query_string,sid);


    status = sendRequest(get_video_snapshot_xml,url,NULL,NULL,NULL);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}
#endif

int handle_error(int code,char *type)
{
    printf("code is %d,type is %s\n",code,type);
    char error_message[NORMALSIZE];

    memset(error_message,0,sizeof(error_message));


    switch (code)
    {
    case S_AUTH_FAIL:

        if( strcmp(type,"gateway") == 0)
        {
#ifdef DEBUG
            printf("username is error \n");
            snprintf(error_message, NORMALSIZE, "%s", "username is error");
#endif
        }
        else
        {
#ifdef DEBUG
            printf("auth failed ,please check username and password\n");
            snprintf(error_message, NORMALSIZE, "%s", "auth failed ,please check username and password");
#endif
        }
        break;

    case CURLE_COULDNT_RESOLVE_HOST:
#ifdef DEBUG
        printf("can't resolve host,please check connection \n");
        snprintf(error_message, NORMALSIZE, "%s", "can't resolve host,please check connection");
#endif
        break;
    case CURLE_COULDNT_CONNECT:
#ifdef DEBUG
        printf("can't connect to host,please check connection \n");
        snprintf(error_message, NORMALSIZE, "%s", "can't connect to host,please check connection");
#endif
        break;
    case CURLE_PARTIAL_FILE:
        printf("partial file ,please check transe file size \n");
        break;
    case CURLE_QUOTE_ERROR:
        printf("quote error  \n");
        break;
    case CURLE_HTTP_RETURNED_ERROR:
        printf("http return error code is %d  \n",code);
        break;
    case CURLE_OPERATION_TIMEDOUT:
        printf("connect time out  \n");
        break;
    case S_LOCAL_SPACE_FULL:
#ifdef DEBUG
        printf("local space is not enough \n");
        snprintf(error_message, NORMALSIZE, "%s", "local space is not enough");
#endif
        break;
    case S_SERVER_SPACE_FULL:
#ifdef DEBUG
        printf("server space is not enough \n");
        snprintf(error_message, NORMALSIZE, "%s", "server space is not enough");
#endif
        break;
    case S_MEMORY_FAIL:
#ifdef DEBUG
        printf("create dynamic memory fail \n");
#endif
        snprintf(error_message, NORMALSIZE, "%s", "create dynamic memory fail");
        break;

    case S_MKDIR_FAIL:
#ifdef DEBUG
        printf("create folder error,please check disk can write or dir has exist???");
#endif
        break;

    case S_UPDATE_ATTR_FAIL:
#ifdef DEBUG
        printf("update attr fail\n");
#endif
        snprintf(error_message, NORMALSIZE, "%s", "update file attr fail");
        break;

    case S_OPENDIR_FAIL:
#ifdef DEBUG
        printf("open dir fail\n");
#endif
        snprintf(error_message, NORMALSIZE, "%s", "open dir fail");
        break;

    default:
#ifdef DEBUG
        snprintf(error_message,NORMALSIZE,"code is %d,type is %s",code,type);
#endif
        break;
    }
#if SYSTEM_LOG
    write_system_log("error",error_message);
#endif
    return 0;
}

long long int getParentID(char *path)
{
    if(NULL == path)
        return -1;

    char *cut_path;
    char parse_path[512];
    long long int parentID = -5;
    Propfind *pfind;
    int sync_path_len;
    const char *split = "/";
    char *p2;

    memset(parse_path,0,sizeof(parse_path));
    snprintf(parse_path, 512, "%s", path);

    if( !strcmp(parse_path,sync_path) ) // path is sync root path
    {
        return MySyncFolder;
    }

    sync_path_len = strlen(sync_path);
    cut_path = parse_path;
    cut_path = cut_path + sync_path_len;
    cut_path++ ; // pass first '/'

    p2 = strtok(cut_path,split);
    int j=0;
    while(p2!=NULL)
    {
        if(j == 0)
            parentID = MySyncFolder;

        pfind = checkEntryExisted(username,parentID,p2,"system.folder");

        if(NULL == pfind)
        {
            return -1;
        }

        if(pfind->status != 0)
        {
            my_free(pfind);
            return -1;
        }

        if(strcmp(pfind->type,"system.notfound") == 0)
        {
        	my_free(pfind);
        	return -2;
        }      

        parentID = pfind->id;

        my_free(pfind);
        j++;
        p2 = strtok(NULL,split);
    }

   return parentID;
}

long int check_server_space(char *username)
{
    Getinfo *gi = NULL;
    long long int size;

    gi = getInfo(username,sergate.gateway);

    if(NULL == gi)
        return -1;
    if(gi->status != 0)
    {
        handle_error(gi->status,"getinfo");
        my_free(gi);
        return -1;
    }

    size = gi->freecapacity;

    my_free(gi);
    return size;

}

int get_server_space(char *username,server_capacity *cap)
{
    Getinfo *gi = NULL;

    gi = getInfo(username,sergate.gateway);


    if(NULL == gi)
        return -1;
    if(gi->status != 0)
    {
        handle_error(gi->status,"getinfo");
        my_free(gi);
        return -1;
    }

    cap->total = gi->usedcappacity + gi->freecapacity;
    cap->used = gi->usedcappacity;
    my_free(gi);
    return 0;
}

int write_log(int status, char *message, char *filename)
{
    Log_struc log_s;
    FILE *fp;
    int mount_path_length;
    server_capacity cap;

    if(status == S_SYNC || status == S_UPLOAD)
    {
        if(get_server_space(username,&cap) != -1)
        {
            pre_cap.total = cap.total;
            pre_cap.used = cap.used;
        }
        else
        {
            cap.total = pre_cap.total;
            cap.used = pre_cap.used;
        }
    }

    mount_path_length = strlen(mount_path);

    memset(&log_s,0,LOG_SIZE);

    log_s.status = status;

    fp = fopen(general_log,"w");

    if(fp == NULL)
    {
        printf("open %s error\n",general_log);
        return -1;
    }

    if(log_s.status == S_ERROR)
    {
        error_flag = 1;
        snprintf(log_s.error, 512, "%s", message);
        fprintf(fp,"STATUS:%d\nERR_MSG:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\n",log_s.status,log_s.error,pre_cap.total,pre_cap.used);

    }
    else if(log_s.status == S_NEEDCAPTCHA)
    {
        fprintf(fp,"STATUS:%d\nERR_MSG:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nCAPTCHA_URL:%s\n",
                status,message,pre_cap.total,pre_cap.used,filename);
    }
    else if(log_s.status == S_DOWNLOAD)
    {
        snprintf(log_s.path, 512, "%s", filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\n",
                log_s.status,mount_path,log_s.path+mount_path_length,pre_cap.total,pre_cap.used);
    }
    else if(log_s.status == S_UPLOAD)
    {
        snprintf(log_s.path, 512, "%s", filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\n",log_s.status,mount_path,log_s.path+mount_path_length,cap.total,cap.used);
    }
    else
    {
        if (log_s.status == S_SYNC)
            fprintf(fp,"STATUS:%d\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\n",log_s.status,cap.total,cap.used);
        else
            fprintf(fp,"STATUS:%d\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\n",log_s.status,pre_cap.total,pre_cap.used);

    }

    fclose(fp);
    return 0;
}

int write_finish_log()
{
    if( local_space_full || server_space_full)
        return 0;

    if(IsSyncError)
    {
        write_log(S_ERROR,"Local synchronization is not entirely successful,failure information,please refer to errlog","");
        IsSyncError = 0 ;
    }
    else
        show_finish_log();

    return 0;
}

int write_system_log(char *action,char *message)
{
    struct stat buf;
    FILE *fp;
    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    char *ctime = asctime(timeinfo);

    if( ctime[ strlen(ctime)-1 ] == '\n')
        ctime[ strlen(ctime)-1 ] = '\0';

    if( stat(system_log,&buf) == -1)
    {
        fp = fopen(system_log,"w");
    }
    else
    {
        fp = fopen(system_log,"a");
    }

     if(NULL == fp)
     {
         printf("open %s failed\n",system_log);
         return -1;
     }

     fprintf(fp,"%s asuswebstorage [action]:%s [message]:%s\n",ctime,action,message);
     fclose(fp);

     /*wiret temp file*/
#if 0
     fp = fopen(temp_file,"w");

     if(NULL == fp)
     {
         printf("open %s failed\n",temp_file);
         return -1;
     }

     fprintf(fp,"%s\n",filename);

     fclose(fp);
#endif
     return 0;

}

int write_confilicted_log(char *prename, char *confilicted_name)
{
    struct stat buf;
    FILE *fp;

    if( stat(confilicted_log,&buf) == -1)
    {
        fp = fopen(confilicted_log,"w");
    }
    else
    {
        fp = fopen(confilicted_log,"a");
    }

     if(NULL == fp)
     {
         printf("open %s failed\n",system_log);
         return -1;
     }

     fprintf(fp,"%s is download from server,%s is local file and rename from %s\n",prename,confilicted_name,prename);
     fclose(fp);

     return 0;
}

int write_trans_excep_log(char *fullname,int type,char *msg)
{
    FILE *fp = 0;
    char ctype[16] = {0};

    if(type == 1)
        snprintf(ctype, 16, "%s", "Error");
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

#if 1
int sync_all_item(char *dir,long long int parentID)
{
    struct dirent* ent = NULL;
    Createfolder *cf;
    char fullname[NORMALSIZE];
    int fail_flag = 0;
    char error_message[NORMALSIZE];
    int res_value = -10;

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
       snprintf(error_message,NORMALSIZE,"opendir %s fail",dir);
       handle_error(S_OPENDIR_FAIL,error_message);
       fail_flag = 1;
       return S_UPLOAD_DELETED;
    }

    while (NULL != (ent=readdir(dp)))
    {
        long long int id;
        int status = -10;

        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        memset(fullname,0,sizeof(fullname));
        memset(error_message,0,sizeof(error_message));
        snprintf(fullname,NORMALSIZE,"%s/%s",dir,ent->d_name);

        if( test_if_dir(fullname) == 1)
        {
            cf = createFolder(username,parentID,0,fullname);
            if(NULL == cf)
            {
                printf("sync_all_item function create folder fail\n");
                fail_flag = 1;
                continue;
            }
            else if(cf->status != 0)
            {
                 fail_flag = 1;
                 snprintf(error_message,NORMALSIZE,"createfolder %s fail on server",ent->d_name);
                 handle_error(cf->status,error_message);
                 res_value = handle_createfolder_fail_code(cf->status,parentID,dir,fullname);
                 if(res_value != 0)
                      fail_flag = 1;
                 my_free(cf);
                 continue;
            }
            else if(cf->status == 0)
            {
                id = cf->id;
                my_free(cf);
                sync_all_item(fullname,id);
            }
        }
        else
        {
            status = uploadFile(fullname,parentID,NULL,0);
                if(status != 0)
                {
                   printf("upload %s failed\n",fullname);
                   res_value = handle_upload_fail_code(status,parentID,fullname,dir);
                   if(res_value != 0)
                        fail_flag = 1;
                }
        }
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}


#endif

int sync_all_item_uploadonly(char *dir,long long int parentID)
{
    struct dirent* ent = NULL;
    Createfolder *cf;
    char fullname[NORMALSIZE];
    int fail_flag = 0;
    char error_message[NORMALSIZE];
    int res_value = -10;

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
       snprintf(error_message,NORMALSIZE,"opendir %s fail",dir);
       handle_error(S_OPENDIR_FAIL,error_message);
       fail_flag = 1;
       return S_UPLOAD_DELETED;
    }

    while (NULL != (ent=readdir(dp)))
    {
        long long int id;
        int status = -10;

        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        memset(fullname,0,sizeof(fullname));
        memset(error_message,0,sizeof(error_message));
        snprintf(fullname,NORMALSIZE,"%s/%s",dir,ent->d_name);

        if(upload_only && receve_socket)
            return -1;

        if( test_if_dir(fullname) == 1)
        {
            cf = createFolder(username,parentID,0,fullname);
            if(NULL == cf)
            {
                printf("sync_all_item function create folder fail\n");
                fail_flag = 1;
                continue;
            }
            else if(cf->status != 0)
            {
                 fail_flag = 1;
                 snprintf(error_message,NORMALSIZE,"createfolder %s fail on server",ent->d_name);
                 handle_error(cf->status,error_message);
                 res_value = handle_createfolder_fail_code(cf->status,parentID,dir,fullname);
                 if(res_value != 0)
                      fail_flag = 1;
                 my_free(cf);
                 continue;
            }
            else if(cf->status == 0)
            {
                id = cf->id;
                my_free(cf);
                sync_all_item(fullname,id);
            }
        }
        else
        {
            status = uploadFile(fullname,parentID,NULL,0);
                if(status != 0)
                {
                   printf("upload %s failed\n",fullname);
                   res_value = handle_upload_fail_code(status,parentID,fullname,dir);
                   if(res_value != 0)
                        fail_flag = 1;
                }
        }
    }
    closedir(dp);
    return (fail_flag == 1) ? -1 : 0;
}

int add_all_download_only_socket_list(char *cmd,const char *dir)
{   
    struct dirent* ent = NULL;
    char fullname[NORMALSIZE];
    int fail_flag = 0;
    char error_message[NORMALSIZE];

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
       snprintf(error_message,NORMALSIZE,"opendir %s fail",dir);
       handle_error(S_OPENDIR_FAIL,error_message);
       fail_flag = 1;
       return -1;
    }

    add_sync_item(cmd,dir,download_only_socket_head);

    while (NULL != (ent=readdir(dp)))
    {

        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        memset(fullname,0,sizeof(fullname));
        memset(error_message,0,sizeof(error_message));

        snprintf(fullname,NORMALSIZE,"%s/%s",dir,ent->d_name);

        if( test_if_dir(fullname) == 1)
        {
            add_all_download_only_socket_list("createfolder",fullname);
        }
        else
        {
            add_sync_item("createfile",fullname,download_only_socket_head);
        }
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}

int add_all_download_only_dragfolder_socket_list(const char *dir)
{
    struct dirent* ent = NULL;
    char fullname[NORMALSIZE];
    int fail_flag = 0;
    char error_message[NORMALSIZE];

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
       snprintf(error_message,NORMALSIZE,"opendir %s fail",dir);
       handle_error(S_OPENDIR_FAIL,error_message);
       fail_flag = 1;
       return -1;
    }

    while (NULL != (ent=readdir(dp)))
    {

        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        memset(fullname,0,sizeof(fullname));
        memset(error_message,0,sizeof(error_message));

        snprintf(fullname,NORMALSIZE,"%s/%s",dir,ent->d_name);

        if( test_if_dir(fullname) == 1)
        {
            add_sync_item("createfolder",fullname,download_only_socket_head);
            add_all_download_only_dragfolder_socket_list(fullname);
        }
        else
        {
            add_sync_item("createfile",fullname,download_only_socket_head);
        }
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}

int handle_rename(long long int parentID,char *fullname,int type,char *prepath,
                  int is_case_conflict,char *pre_name)//tina modify
{
    Propfind *find = NULL;
    long long int entryID = -10;
    char *confilicted_name = NULL;
    char *filename = NULL;
    char path[512];
    int isfolder = 0;
    char *newfilename = NULL;
    Operateentry *oe = NULL;
    int res_value = -10;
    char newfullname[512];
    int status = -10;
    char tmp_name[1024] = {0};

    filename = parse_name_from_path(fullname);


    if(NULL == filename)
    {
        handle_error(S_MEMORY_FAIL,"handle_rename parse filename");
        return -1;
    }

    memset(path,0,sizeof(path));
    strncpy(path,fullname,strlen(fullname)-strlen(filename)-1);

    if(test_if_dir(fullname))
        isfolder = 1;
    else
        isfolder = 0;

    strncpy(tmp_name,fullname,1024);
    while(!exit_loop)
    {

        if(is_case_conflict)
            confilicted_name = get_confilicted_name_case(tmp_name,path,pre_name,filename);
        else
            confilicted_name = get_confilicted_name(tmp_name,0);

        if(confilicted_name ==  NULL)
        {
            printf("handle_local_confilict_file fail\n");
            return -1;
        }
       if(access(confilicted_name,F_OK) == 0)
       {
           memset(tmp_name,0,sizeof(tmp_name));
           snprintf(tmp_name,NORMALSIZE,"%s",confilicted_name);
           my_free(confilicted_name);
       }
       else
           break;
     }

    if(upload_only && !is_case_conflict)
    {  
        find = checkEntryExisted(username,parentID,filename,"system.unknown");

        if(NULL == find)
        {
            printf("find prop failed\n");
            my_free(filename);
            my_free(confilicted_name);
            return -1;
        }

        if( find->status != 0 )
        {
            handle_error(find->status,"propfind");
            my_free(filename);
            my_free(confilicted_name);
            my_free(find);
            return -1;
        }

        entryID = find->id;

        if(strcmp(find->type,"system.folder") == 0)
        {
            isfolder = 1;
        }
        else if(strcmp(find->type,"system.file") == 0)
        {
            isfolder = 0;
        }

       newfilename = parse_name_from_path(confilicted_name);
       if(NULL == newfilename)
       {
           handle_error(find->status,"propfind");
           my_free(filename);
           my_free(confilicted_name);
           my_free(find);
           return -1;
       }

       oe = renameEntry(username,entryID,0,newfilename,isfolder) ;
       if(NULL == oe)
       {
           printf("operate rename failed\n");
           handle_error(find->status,"propfind");
           my_free(filename);
           my_free(confilicted_name);
           my_free(find);
           my_free(newfilename);
           return -1;
       }

       if( oe->status != 0 )
       {
           handle_error(oe->status,"renameEntry");
           snprintf(newfullname, 512, "%s/%s",path,newfilename);
           res_value = handle_rename_fail_code(oe->status,parentID,newfullname,path,isfolder);
           if(res_value != 0)
           {
               my_free(filename);
               my_free(confilicted_name);
               my_free(find);
               my_free(newfilename);
               my_free(oe);
               return res_value;
           }
       }
       res_value = upload_entry(fullname,parentID,path);
       my_free(find);
       my_free(newfilename);
       my_free(oe);
    }
    else  //sync or case conflict
    {     
        status = rename(fullname,confilicted_name);

        if(status == -1)
        {
            handle_error(S_RENAME_FAIL," rename name");
            my_free(filename);
            my_free(confilicted_name);
            return -1;
        }
        add_sync_item("rename",confilicted_name,from_server_sync_head);
        res_value = upload_entry(confilicted_name,parentID,path);

    }
    my_free(filename);
    my_free(confilicted_name);

    return res_value;
}

char *strlwr(char *s)
{
    char *str = NULL;
    char *name = (char *)calloc(256,sizeof(char));
    strncpy(name,s,256);
    str = name;

    while(*str != '\0')
    {
        if(*str >= 'A' && *str <= 'Z'){
            *str += 'a'-'A';
        }
        str++;
    }
    return name;
}

int is_exist_case_conflicts(char *fullname,char *pre_name)
{
    struct dirent* ent = NULL;
    DIR *pDir;
    char *name;
    char path[1024] = {0};
    char *p1 = NULL;
    char*p2 = NULL;
    int count = 0;

    name = parse_name_from_path(fullname);

    if(name == NULL)
    {
        printf("parse name fail\n");
        return -1;
    }

    strncpy(path,fullname,strlen(fullname)-strlen(name)-1);
    p1 = strlwr(name);


    pDir=opendir(path);

    if(NULL == pDir)
    {
        printf("open %s fail\n",path);
        my_free(name);
        my_free(p1);
        return -1;
    }

    while (NULL != (ent=readdir(pDir)))
    {
        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,"..") || !strcmp(ent->d_name,name))
            continue;

        p2 = strlwr(ent->d_name);
        if(!strcmp(p1,p2))
        {
           count++;
           snprintf(pre_name, 256, "%s", ent->d_name);
        }
        my_free(p2);
    }

    closedir(pDir);
    my_free(p1)
    my_free(name);

    if(count>0)
        return 1;
    return 0;
}

int handle_createfolder_fail_code(int status,long long int parent_ID,char *path,char* fullname)
{
    int res_value = -10;
    long long int ID = parent_ID ;
    char error_message[512];
    char pre_name[256] = {0};

    memset(error_message,0,sizeof(error_message));

    switch(status)
    {
    case S_DIR_NOT_EXIST: //parent dir not exist
        ID = create_server_folder_r(path);
        if(ID<0)
            return -1;
        res_value = upload_entry(fullname,ID,path);
        break;
    case S_NAME_REPEAT:
        res_value = is_exist_case_conflicts(fullname,pre_name);
        if(res_value == -1)
            return -1;
        if(res_value)
            res_value = handle_rename(parent_ID,fullname,createfolder_action,NULL,1,pre_name);
        break;
    default:
        break;
    }

    return res_value;
}

int handle_upload_fail_code(int status,long long int parent_ID,char* fullname,const char *path)
{
    int res_value = 0;
    char up_action[256] = {0};
    long long int ID = -10 ;
    int res_upload = -10;

    switch(status)
    {
    case S_USER_NOSPACE:
        write_log(S_ERROR,"server space is not enough","");
        server_space_full = 1;
        res_value = 0;
        break;
    case S_SERVER_SPACE_FULL:
        snprintf(up_action,256,"uploadfile,%lld,%s",parent_ID,"none");
        add_sync_item(up_action,fullname,up_head);
        add_sync_item("up_excep_fail",fullname,up_excep_fail);
        res_value = 0;
        break;
    case S_UPLOAD_DELETED:
        snprintf(up_action,256,"uploadfile,%lld,%s",parent_ID,"none");
        del_sync_item(up_action,fullname,up_head);
        del_sync_item("up_excep_fail",fullname,up_excep_fail);
        res_value = 0;
        break;
    case S_FILE_TOO_LARGE:
         write_trans_excep_log(fullname,1,"Filesize Too Large");
         res_value = 0;
         no_completed = 1;
         break;
    case S_TRANS_ID_NOT_EXIST:
        res_value = 0 ;
        break;
    case S_DIR_NOT_EXIST:
        ID = create_server_folder_r(path);
        if( -1 == ID )
        {
            res_value = -1;
        }
        else if(ID > 0)
        {
            res_upload = uploadFile(fullname,ID,NULL,0);
            if(res_upload != 0)
            {
                res_value = handle_upload_fail_code(res_upload,ID,fullname,path);
            }
        }
        break;
 case S_NAME_REPEAT:
        res_value = -1;
        break;
   case -1:
        res_value = -1;
        break;
    default:
        break;
    }
    return res_value;
}

int handle_delete_fail_code(int status)
{
    int res_value = 0;

    switch(status)
    {
    case S_FILE_NOT_EXIST:
    case S_DIR_NOT_EXIST:
        res_value = 0;
        break;
    default:
        break;
    }

    return res_value;
}

int handle_rename_fail_code(int status,long long int parentID,char *fullname,char *path,int isfolder)//tina modify
{
    int res_value = 0;
    long long int ID = parentID ;
    char error_message[512];
    char pre_name[256] = {0};

    memset(error_message,0,sizeof(error_message));

    switch(status)
    {
    case S_FILE_NOT_EXIST:
        res_value = upload_entry(fullname,parentID,path);
        break;
    case S_DIR_NOT_EXIST:
        ID = create_server_folder_r(path);
        if(ID < 0)
            return -1;
        res_value = upload_entry(fullname,ID,path);
        break;
    case S_NAME_REPEAT:
        res_value = is_exist_case_conflicts(fullname,pre_name);
        printf("res_value=%d\n",res_value);
        if(res_value == -1)
            return -1;
        res_value = handle_rename(parentID,fullname,rename_action,NULL,res_value,pre_name);
        break;
    default:
        break;
    }
  return res_value;
}

int handle_move_fail_code(int status,char *path,char *fullname,long long int parentID,char *prepath,long long int entryID,int isfolder)
{
    int res_value = 0;
    long long int ID = parentID ;
    char pre_name[256] = {0};
    Operateentry *oe;

    switch(status)
    {
    case S_FILE_NOT_EXIST:
        res_value = upload_entry(fullname,parentID,path);
        break;
    case S_NAME_REPEAT:
        oe = removeEntry(username,entryID,0,isfolder,parentID);
        if(oe == NULL)
            return -1;
        my_free(oe);
        res_value = is_exist_case_conflicts(fullname,pre_name);
        if(res_value == -1)
            return -1;
        res_value = handle_rename(parentID,fullname,rename_action,NULL,res_value,pre_name);
        break;
    case S_DIR_NOT_EXIST:
    case S_ILLEGAL_OPERATION:   //parent dir delete
        ID = create_server_folder_r(path);
        if(ID<0)
            return -1;
        res_value = upload_entry(fullname,ID,path);
        break;
    default:
        break;

    }

    return res_value;

}

long long int create_server_folder_r(const char *path)
{
    if(NULL == path)
        return -1;

    char *cut_path;
    char parse_path[512];
    long long int parentID = -5;
    Propfind *pfind;
    int sync_path_len;
    const char *split = "/";
    char *p2;
    Createfolder *cf = NULL;
    char fullname[512];

    memset(parse_path,0,sizeof(parse_path));
    memset(fullname,0,sizeof(fullname));
    snprintf(parse_path, 512, "%s", path);
    snprintf(fullname, 512, "%s", sync_path);

    if( !strcmp(parse_path,sync_path) ) // path is sync root path
    {
        return MySyncFolder;
    }

    sync_path_len = strlen(sync_path);
    cut_path = parse_path;
    cut_path = cut_path + sync_path_len;
    cut_path++ ; // pass first '/'

    p2 = strtok(cut_path,split);
    int j=0;
    while(p2!=NULL)
    {
        snprintf(fullname+strlen(fullname), 512-strlen(fullname), "%s", "/");
        snprintf(fullname+strlen(fullname), 512-strlen(fullname), "%s", p2);

        if(j == 0)
            parentID = MySyncFolder;

#ifdef DEBUG
#endif

        pfind = checkEntryExisted(username,parentID,p2,"system.folder");

        if(NULL == pfind)
        {
            return -1;
        }
        else if(pfind->status == 0)
        {
            if(strcmp(pfind->type,"system.notfound") == 0)
            {
                    my_free(pfind);
                    cf = createFolder(username,parentID,0,fullname);
                    if(NULL == cf)
                    {
                        printf("cf is null\n");
                        return -1;
                    }
                    else if(cf->status == 0)
                    {
                        parentID = cf->id;
                        my_free(cf);
                    }
                    else
                    {
                        handle_error(cf->status,"createfolder");
                        my_free(cf);
                        return -1;
                    }
            }
            else
            {
                parentID = pfind->id;
                my_free(pfind);
            }
        }
        else
        {
            handle_error(pfind->status,"propfind");
            my_free(pfind);
            return -1;
        }

        j++;
        p2 = strtok(NULL,split);
    }

   return parentID;
}

int upload_entry(char *fullname,long long int parent_ID,char *path)
{
    int status = -10;
    long long int entry_ID = -10;
    Createfolder *cf = NULL;
    int res_value = 0;
#if TREE_NODE_ENABLE
    modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif
    if(test_if_dir(fullname))
    {
        cf = createFolder(username,parent_ID,0,fullname);
        if(NULL == cf)
        {
            printf("createfolder %s fail\n",fullname);
            return -1;
        }
        else if( cf->status != 0 )
        {
            handle_error(cf->status,"propfind");
            res_value = handle_createfolder_fail_code(cf->status,parent_ID,path,fullname);
            my_free(cf);
            return res_value;
        }
        else
        {
            entry_ID = cf->id;
            my_free(cf);
            res_value = sync_all_item(fullname,entry_ID);
            return res_value;
        }
    }
    else //entry is file
    {
        status = uploadFile(fullname,parent_ID,NULL,0);
        
        if(status != 0)
        {
            printf("upload %s failed\n",fullname);
            res_value = handle_upload_fail_code(status,parent_ID,fullname,path);
            return res_value;
        }
    }

    return res_value;
}

int obtain_token_from_file(const char *filename,Aaa *aaa)
{
    FILE *fp = NULL;
    int len;

    if(access(filename,0) != 0)
        return -1;

    fp = fopen(filename,"rb");
    if(fp == NULL)
    {
        printf("open %s fail\n",filename);
        return -1;
    }
    len = fread(aaa,sizeof(Aaa),1,fp);
    fclose(fp);
    if(len <= 0)
        return -1;

    if(strcmp(aaa->user,username) || strcmp(aaa->pwd,password))
        return -1;


    strncpy(sergate.gateway,aaa->gateway,MINSIZE);
    if(obtainSyncRootID(username) == -1)
        return -1;

    return 0;
}
