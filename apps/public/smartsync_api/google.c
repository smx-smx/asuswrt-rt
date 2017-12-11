#include "google.h"
#include "data.h"
#include "cJSON.h"


int exit_loop = 0;
char rootid[64] = {0};
int access_token_expired;
char *Clientfp="*";
double start_time;
char delete_id[64] = "\0";


//list.c******************************
int test_if_dir(const char *dir){
    DIR *dp = opendir(dir);

    if(dp == NULL)
        return 0;

    closedir(dp);
    return 1;
}

char *localpath_to_serverpath(char *from_localpath,int index)
{
    DEBUG("localpath_to_server_path start\n");
    char *to_serverpath;
    char *hreftmp;
#ifdef MULTI_PATH
    hreftmp=strstr(from_localpath,asus_cfg.prule[index]->base_path)+asus_cfg.prule[index]->base_path_len;
#else
    DEBUG("33 line\n");
    DEBUG("from_localpath=%s\n",from_localpath);
    DEBUG("asus_cfg.prule[index]->base_path=%s\n",asus_cfg.prule[index]->base_path);
    hreftmp=strstr(from_localpath,asus_cfg.prule[index]->base_path)+asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len;
    DEBUG("35 line\n");
    DEBUG("hreftmp=%p\n",hreftmp);
    DEBUG("hreftmp=%s\n",hreftmp);
#endif
    to_serverpath=(char *)malloc(sizeof(char)*(strlen(hreftmp)+2));
    memset(to_serverpath,0,sizeof(char)*(strlen(hreftmp)+2));
    DEBUG("38 line\n");
    sprintf(to_serverpath,"%s",hreftmp);
    DEBUG("localpath_to_serverpath: %s=>%s\n",from_localpath,to_serverpath);
    //DEBUG("Localpath_to_serverpath to_serverpath = %s\n",to_serverpath);
    return to_serverpath;
}

/*changed from serverpath to localpath*/
char *serverpath_to_localpath(char *from_serverpath,int index){
    char *to_localpath;

    //hreftmp = strstr(from_serverpath,asus_cfg.prule[index]->rootfolder)+asus_cfg.prule[index]->rootfolder_length;
    //hreftmp = oauth_url_unescape(hreftmp,NULL);
#ifdef MULTI_PATH
    to_localpath = (char *)malloc(sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+2));
    memset(to_localpath,'\0',sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+2));

    sprintf(to_localpath,"%s%s",asus_cfg.prule[index]->base_path,from_serverpath);
#else
    to_localpath = (char *)malloc(sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2));
    memset(to_localpath,'\0',sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2));

    sprintf(to_localpath,"%s%s%s",asus_cfg.prule[index]->base_path,asus_cfg.prule[index]->rooturl,from_serverpath);
#endif
    DEBUG("serverpath_to_localpath, to_localpath = %s\n",to_localpath);

    return to_localpath;
}


int path_to_ppath(char *path, char *path1)
{
    char *pointer = NULL;
    pointer = strrchr(path, '/');
    if(strcmp(path, pointer) == 0)
        snprintf(path1, strlen(path) - strlen(pointer) + 2, path);
    else
        snprintf(path1, strlen(path) - strlen(pointer) + 1, path);
    //DEBUG("path = %s,path1 = %s", path, path1);
}


int ChangeFile_modtime(char *filepath,time_t servermodtime){

    DEBUG("**************ChangeFile_modtime**********\n");

    if(access(filepath,F_OK) != 0)
    {
        DEBUG("%s is not exist!\n", filepath);
        return -1;
    }

    //char *localfilepath_tmp;
    //char localfilepath[256];
    struct utimbuf *ub;
    ub = (struct utimbuf *)malloc(sizeof(struct utimbuf));
    //memset(localfilepath,0,sizeof(localfilepath));

    //DEBUG("servermodtime = %lu\n",servermodtime);

    if(servermodtime == -1){

        //DEBUG("ChangeFile_modtime ERROR!\n");

        return -1;
    }

    ub->actime = servermodtime;
    ub->modtime = servermodtime;
    DEBUG("ub->actime=%lu\n",ub->actime);
    DEBUG("ub->actime=%lu\n",ub->modtime);
    //localfilepath_tmp = strstr(filepath,"/RT-N16/");
    //sprintf(localfilepath,"%s%s",base_path,localfilepath_tmp);
    utime(filepath,ub);

    /*struct stat buf;

                if( stat(localfilepath,&buf) == -1)
                {
                        perror("stat:");
                }

                                //unsigned long asec = buf.st_atime;
                                unsigned long msec = buf.st_mtime;
                                //unsigned long csec = buf.st_ctime;

                                //printf("accesstime = %lu\n",asec);
                                //printf("creationtime = %lu\n",csec);
                                printf("lastwritetime = %lu\n",msec);
                                printf("servermodtime = %lu\n",servermodtime);*/

    free(ub);
   // DEBUG("ChangeFile_modtime end!\n");
    return 0;
}

/*server tree root function*/
Server_TreeNode *create_server_treeroot()
{
    DEBUG("create_server_treeroot start\n");
    Server_TreeNode *TreeRoot = NULL;
    TreeRoot = (Server_TreeNode *)malloc(sizeof (Server_TreeNode));
    memset(TreeRoot,0,sizeof(Server_TreeNode));
    if(TreeRoot == NULL)
    {
        DEBUG("create memory error!\n");
        exit(-1);
    }
    TreeRoot->level=0;
    TreeRoot->NextBrother = NULL;
    //TreeRoot->browse = NULL;
    //sprintf(TreeRoot->parenthref,"%s%s/",HOST,ROOTFOLDER);
    TreeRoot->parenthref = NULL;
    TreeRoot->browse = NULL;
    TreeRoot->Child = NULL;
    DEBUG("create_server_treeroot end\n");
    return TreeRoot;
}

void free_cloudfile(CloudFile *head)
{
    DEBUG("free_Cloudfile start!\n");
    CloudFile *p = head;
        if(p->href != NULL)
        {
            printf("free href %s\n",p->href);
            free(p->href);
        }
        if(p->name != NULL)
        {
            printf("free name %s\n",p->name);
            free(p->name);
        }
        //*********************************************
        if(p->id != NULL)
        {
           printf("free id %s\n",p->id);
            free(p->id);
        }

        if(p->mimeType != NULL)
        {
            printf("free mimeType %s\n",p->mimeType);
            free(p->mimeType);
        }

        if(p->parents_id != NULL)
        {
            printf("free parents_id %s\n",p->parents_id);
            free(p->parents_id);
        }
        if(p->exportLinks_document != NULL)
        {
            printf("free exportLinks_document %s\n",p->exportLinks_document);
            free(p->exportLinks_document);
        }

        free(p);

    DEBUG("free_CloudFile end!\n");
}

void free_CloudFile_item(CloudFile *head)
{
    //printf("***************free_CloudFile_item*********************\n");

    CloudFile *p = head;
    while(p != NULL)
    {
        head = head->next;
        if(p->href != NULL)
        {
            //printf("free href %s\n",p->href);
            free(p->href);
        }
        if(p->name != NULL)
        {
            //printf("free name %s\n",p->name);
            free(p->name);
        }
        //*********************************************
        if(p->id != NULL)
        {
           //printf("free id %s\n",p->id);
            free(p->id);
        }
       // if(p->title != NULL)
       // {
            //printf("free CloudFile %s\n",p->href);
            //free(p->title);
       // }
        if(p->mimeType != NULL)
        {
            //printf("free mimeType %s\n",p->mimeType);
            free(p->mimeType);
        }
        /*if(p->modifiedDate != NULL)
        {
            //printf("free CloudFile %s\n",p->href);
            free(p->modifiedDate);
        }*/
        if(p->parents_id != NULL)
        {
            //printf("free parents_id %s\n",p->parents_id);
            free(p->parents_id);
        }

        if(p->exportLinks_document != NULL)
        {
            //printf("free exportLinks_document %s\n",p->exportLinks_document);
            free(p->exportLinks_document);
        }
        //*********************************************
        free(p);
        p = head;
    }
    //DEBUG("free_CloudFile_item end!\n");
}

void free_server_tree(Server_TreeNode *node)
{
    DEBUG("free_server_tree, node=%p\n", node);
    if(node != NULL)
    {
        printf("free tree node, node->parenthref = %p\n", node->parenthref);

        //free_server_list(node->browse);

        if(node->NextBrother != NULL)
        {
            DEBUG("nextbrother!\n");
            free_server_tree(node->NextBrother);
        }
        if(node->Child != NULL)
        {
           DEBUG("child!\n");
            free_server_tree(node->Child);

        }
        if(node->parenthref != NULL)
        {
            DEBUG("node->parenthref!\n");
            free(node->parenthref);
        }
        if(node->folderid != NULL)
        {
            DEBUG("node->folderid!\n");
            free(node->folderid);
        }
        if(node->browse != NULL)
        {
            DEBUG("node->browse!\n");
            free_CloudFile_item(node->browse->filelist);
            free_CloudFile_item(node->browse->folderlist);
            free(node->browse);
        }
        free(node);
    }
    DEBUG("free server tree end!\n");
}

/*
 *if a = 0x1,find in folderlist
 *if a = 0x2,find in filelist
 *if a = 0x3,find in folderlist and filelist
*/
CloudFile *get_CloudFile_node(Server_TreeNode* treeRoot,const char *dofile_href,int a){

    printf("****get_CloudFile_node****dofile_href = %p\n",dofile_href);
    DEBUG("dofile_href=%s\n", dofile_href);
    int href_len = strlen(dofile_href);
    CloudFile *finded_file = NULL;
    if(treeRoot == NULL)
    {
        DEBUG("treeRoot==NULL!\n");
        return NULL;
    }
    //DEBUG("\ntreeRoot->browse = %s", treeRoot->browse);
    if(treeRoot->browse != NULL)
    {
        int int_folder = 0x1;
        int int_file = 0x2;
        CloudFile *de_foldercurrent = NULL;
        CloudFile *de_filecurrent = NULL;
        //DEBUG("treenode->parenthref = %s\n", treeRoot->parenthref);
        printf("111111folder = %d,file = %d\n",treeRoot->browse->foldernumber,treeRoot->browse->filenumber);
        if(treeRoot->browse->foldernumber > 0)
        {
            de_foldercurrent = treeRoot->browse->folderlist->next;
            //DEBUG("de_foldercurrent=%p\n", de_foldercurrent);
            //DEBUG("de_foldercurrent->href = %s\n", de_foldercurrent->href);
            //DEBUG("de_foldercurrent->id = %s\n", de_foldercurrent->id);
        }
        if(treeRoot->browse->filenumber > 0)
            de_filecurrent = treeRoot->browse->filelist->next;

        if((a&int_file) && de_filecurrent != NULL)
        {
            while(de_filecurrent != NULL)
            {
                if(de_filecurrent->href != NULL)
                {
                    printf("de_filecurrent->href = %s\n",de_filecurrent->href);
                    //DEBUG("dofile_href=%s\n", dofile_href);
                    if(!(strncmp(de_filecurrent->href,dofile_href,max(strlen(dofile_href),strlen(de_filecurrent->href)))))
                    {
                        printf("get it,return 2\n");
                        return de_filecurrent;
                    }
                }
                de_filecurrent = de_filecurrent->next;
            }
        }
        if((a&int_folder) && de_foldercurrent != NULL)
        {
            //DEBUG("a&int_folder) && de_foldercurrent != NULL)\n");
            while(de_foldercurrent != NULL)
            {
                //DEBUG("de_foldercurrent != NULL)\n");
                if(de_foldercurrent->href != NULL)
                {
                    printf("de_foldercurrent->href = %s\n",de_foldercurrent->href);
                    if(!(strncmp(de_foldercurrent->href,dofile_href,max(strlen(dofile_href),strlen(de_foldercurrent->href)))))
                    {
                        //DEBUG("return 1\n");
                        return de_foldercurrent;
                    }
                }
                de_foldercurrent = de_foldercurrent->next;
            }
        }
    }

    if((treeRoot->Child!=NULL))
    {
        printf("444444444\n");
        finded_file = get_CloudFile_node(treeRoot->Child,dofile_href,a);
        if(finded_file != NULL)
        {
            printf("444444444 return\n");
            return finded_file;
        }
        //else
            //printf("child not get\n");
    }


    if(treeRoot->NextBrother != NULL)
    {
        //printf("33333333\n");
        finded_file = get_CloudFile_node(treeRoot->NextBrother,dofile_href,a);
        if(finded_file != NULL)
        {
            //printf("33333333 return\n");
            return finded_file;
        }
        //else
            //printf("brother not get\n");
    }
    //printf("##return NULL\n");
    return finded_file;
}

int g_browse_to_tree(char *parenthref, char *id, Server_TreeNode *node, int index)
{
    //printf("browse_to_tree node parenthref is %s\n",parenthref);
    DEBUG("enter browse_to_tree!!!node = %p\n", node);
    Browse *br = NULL;
    int fail_flag = 0;
    //int loop;
    //int i;

    Server_TreeNode *tempnode = NULL, *p1 = NULL,*p2 = NULL;
    tempnode = create_server_treeroot();
    tempnode->level = node->level + 1;
    tempnode->parenthref = my_str_malloc((size_t)(strlen(parenthref)+1));
    tempnode->folderid = my_str_malloc((size_t)(strlen(id)+1));
    br = browseFolder(parenthref, id, index);
    sprintf(tempnode->parenthref,"%s",parenthref);
    sprintf(tempnode->folderid,"%s",id);
    DEBUG("@@@@@@@@@@@@@@@tempnode->parenthref=%s\n", tempnode->parenthref);


    if(NULL == br)
    {
        free_server_tree(tempnode);
        printf("browse folder failed\n");
        return -1;
    }

    tempnode->browse = br;

    if(node->Child == NULL)
    {
        node->Child = tempnode;
        DEBUG("node->Child = tempnode;");
    }
    else
    {
        printf("have child\n");
        p2 = node->Child;
        p1 = p2->NextBrother;

        while(p1 != NULL)
        {
            printf("p1 nextbrother have\n");
            p2 = p1;
            p1 = p1->NextBrother;
        }

        p2->NextBrother = tempnode;
        tempnode->NextBrother = NULL;
        DEBUG("tempnode->nextBrother = tempnode;");
    }

    //printf("browse folder num is %d\n",br->foldernumber);
    CloudFile *de_foldercurrent;
    de_foldercurrent = br->folderlist->next;
    while(de_foldercurrent != NULL && !exit_loop)
    {
        DEBUG("browse_to_tree11\n");
        if(g_browse_to_tree(de_foldercurrent->href, de_foldercurrent->id,tempnode, index) == -1)
        {
            fail_flag = 1;
            break;
        }
        de_foldercurrent = de_foldercurrent->next;
    }
    /*for( i= 0; i <br->foldernumber;i++)
    {
        id = (br->folderlist)[i]->id;

        if(browse_to_tree(username,id,xmlfilename,tempnode) == -1)
        {
            fail_flag = 1;
        }
    }*/
    if(exit_loop)
        fail_flag = 1;
    //free_server_list(br);
    //my_free(br);

    return (fail_flag == 1) ? -1 : 0 ;

}
int upload_serverlist(Server_TreeNode *treenode,Browse *perform_br, char *localfoldertmp,int index, char *newfolderid){
    DEBUG("upload_serverlist start!\n");
    //DEBUG("treenode->folderid = %s\n", treenode->folderid);
    CloudFile *foldertmp = NULL,*foldertmp_1=NULL;
    foldertmp=(CloudFile *)malloc(sizeof(struct node));
    memset(foldertmp,0,sizeof(foldertmp));
    DEBUG("454 line\n");
    foldertmp_1=perform_br->folderlist;
    DEBUG("456 line\n");
    while(foldertmp_1->next != NULL)
    {

        foldertmp_1=foldertmp_1->next;
    }
    DEBUG("462 line\n");
    if(foldertmp_1->next == NULL){
        DEBUG("1111!!!\n");
        char *serverpath;
        DEBUG("localfoldertmp=%p,localfoldertmp=%s\n",localfoldertmp,localfoldertmp);
        serverpath=localpath_to_serverpath(localfoldertmp,index);
        foldertmp->href=(char *)malloc(sizeof(char)*(strlen(serverpath)+1));
        memset(foldertmp->href,'\0',sizeof(char)*(strlen(serverpath)+1));
        strcpy(foldertmp->href,serverpath);
        DEBUG("before,,,newfolderid = %p\n", newfolderid);
        foldertmp->id=(char *)malloc(sizeof(char)*(strlen(newfolderid)+1));
        memset(foldertmp->id,'\0',sizeof(char)*(strlen(newfolderid)+1));
        strcpy(foldertmp->id,newfolderid);
        foldertmp->mimeType=(char *)malloc(128);
        memset(foldertmp->mimeType,'\0',sizeof(foldertmp->mimeType));
        sprintf(foldertmp->mimeType,"application/vnd.google-apps.folder");
        foldertmp->parents_id=(char *)malloc(64);
        memset(foldertmp->parents_id,'\0',sizeof(foldertmp->parents_id));
        strcpy(foldertmp->parents_id, treenode->folderid);
        foldertmp->exportLinks_document = NULL;
        DEBUG("after,,,!!!\n");
        foldertmp->size=0;
        foldertmp->name=parse_name_from_path(foldertmp->href);
        DEBUG("484 line\n");
        //DEBUG("foldertmp->href = %s,name = %s,foldertmp->id=%s\n", foldertmp->href, foldertmp->name,foldertmp->id);
        foldertmp->isFolder=1;
        foldertmp_1->next=foldertmp;
        foldertmp_1=foldertmp;
        foldertmp->next=NULL;

        perform_br->foldernumber += 1;
        DEBUG("492 line\n");
        //DEBUG("%s has %d folder\n", treenode->parenthref, perform_br->foldernumber);
        free(serverpath);

    }

    Browse *br;
    br=getb(Browse);
    memset(br,0,sizeof(br));

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

    br->filelist = TreeFileList;
    br->folderlist = TreeFolderList;
    br->filenumber=0;
    br->foldernumber=0;

    Server_TreeNode *treenodetmp,*p1=NULL,*p2=NULL;
    treenodetmp=create_server_treeroot();
    treenodetmp->parenthref=(char *)malloc(sizeof(char)*(strlen(foldertmp->href)+1));
    treenodetmp->folderid=(char *)malloc(sizeof(char)*(strlen(foldertmp->id)+1));
    treenodetmp->level = treenode->level + 1;
    memset(treenodetmp,'\0',sizeof(treenodetmp));
    strcpy(treenodetmp->parenthref,foldertmp->href);
    strcpy(treenodetmp->folderid,foldertmp->id);


    treenodetmp->browse=br;
    if(treenode->Child == NULL)
    {
        DEBUG("222!!!\n");
        treenode->Child = treenodetmp;
    }
    else
    {
        !!!DEBUG("333\n");
        printf("have child\n");
        p2 = treenode->Child;
        p1 = p2->NextBrother;

        while(p1 != NULL)
        {
            printf("p1 nextbrother have\n");
            p2 = p1;
            p1 = p1->NextBrother;
        }

        p2->NextBrother = treenodetmp;
        treenodetmp->NextBrother = NULL;
    }

}

/*
 *if a = 0x1,find in folderlist
 *if a = 0x2,find in filelist
 *if a = 0x3,find in folderlist and filelist
*/
Server_TreeNode *get_treenode(Server_TreeNode* treeRoot,const char *dofile_href,int index){

    printf("****get_treenode****dofile_href = %s\n",dofile_href);
    int href_len = strlen(dofile_href);
    DEBUG("href_len = %d\n",href_len);
    Server_TreeNode *finded_file = NULL;
    if(treeRoot == NULL)
    {
        DEBUG("treeRoot == NULL\n");
        return NULL;
    }
    DEBUG("3386,treeRoot != NULL\n");
    DEBUG("treeRoot->parenthref = %s\n",treeRoot->parenthref);
    if(treeRoot->parenthref != NULL)
    {
        if(!(strncmp(treeRoot->parenthref,dofile_href,href_len)))
        {
            DEBUG("get_treenode,get it!!!\n");
            return treeRoot;
        }
    }
    DEBUG("3389 line\n");
    if((treeRoot->Child!=NULL))
    {
        //printf("444444444\n");
        finded_file = get_treenode(treeRoot->Child,dofile_href,index);
        if(finded_file != NULL)
        {
            //printf("444444444 return\n");
            return finded_file;
        }
        //else
            //printf("child not get\n");
    }


    if(treeRoot->NextBrother != NULL)
    {
        //printf("33333333\n");
        finded_file = get_treenode(treeRoot->NextBrother,dofile_href,index);
        if(finded_file != NULL)
        {
            //printf("33333333 return\n");
            return finded_file;
        }
        //else
            //printf("brother not get\n");
    }
    printf("get_treenode,return NULL\n");
    return finded_file;
}

int upload_serverlist_(int index,char *fullname_t,char *serverpath,char *newfolderid)
{
    int status = 0;
    DEBUG("!!!!!!upload_serverlist_ start!!!!!!\n");
    char *p_path;
    p_path = (char *)malloc(strlen(serverpath));
    memset(p_path,0, strlen(serverpath));

    char *p = strrchr(serverpath,'/');
    if(strcmp(p, serverpath) == 0)
        strcpy(p_path,"/");
    else
        snprintf(p_path,strlen(serverpath) - strlen(p)+1,serverpath);
    DEBUG("p_path = %s\n", p_path);
    if(!g_pSyncList[index]->ServerRootNode)
    {

        free_server_tree(g_pSyncList[index]->ServerRootNode);
        g_pSyncList[index]->ServerRootNode = NULL;
        g_pSyncList[index]->ServerRootNode = create_server_treeroot();
        //DEBUG("browse_to_tree1\n");
        g_browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode, index);
    }
    /*if(g_pSyncList[index]->ServerRootNode == NULL)
    {
        DEBUG("g_pSyncList[index]->ServerRootNode == NULL\n");

        g_pSyncList[index]->ServerRootNode = create_server_treeroot();
        s_browse_to_tree("/", rootid, g_pSyncList[index]->ServerRootNode, index,6);
    }*/
    DEBUG("p_path,ServerRootNode!=NULL\n");
    Server_TreeNode*treenode = NULL;
    treenode = get_treenode(g_pSyncList[index]->ServerRootNode,p_path,index);
    DEBUG("after get_treenode,treenode = %p\n",treenode);
    free(p_path);
    DEBUG("3441 line\n");
    //DEBUG("treenode->browse = %p\n",treenode->browse);
    DEBUG("fullname_t = %s\n",fullname_t);
    DEBUG("newfolderid = %s\n", newfolderid);
    if (treenode != NULL)
    {
        DEBUG("3444 line\,treenode->browse=%p\n",treenode->browse);
        status = upload_serverlist(treenode,treenode->browse,fullname_t,index,newfolderid);
        DEBUG("upload_serverlist end,status = %d\n", status);
    }
    else
        return -1;
    DEBUG("!!!!!!upload_serverlist_ end!!!!!!\n");
    return 0;
}
//function.c****************************
char *case_conflict_name = "case-conflict";

char *my_str_malloc(size_t len){

    //printf("@len = %d\n",len);

    char *s;
    //printf("my_str_malloc 1\n");
    s = (char *)malloc(sizeof(char)*len);
    //printf("my_str_malloc 2\n");
    if(s == NULL)
    {
        printf("Out of memory.\n");
        exit(1);
    }
    //printf("my_str_malloc 3\n");
    memset(s,'\0',sizeof(char)*len);
    //printf("my_str_malloc return\n");
    return s;
}

/*
 大小写转换函数
 big to low
*/
char *strlwr(char *s)
{
    char *str;
    str = s;
    while(*str != '\0')
    {
        if(*str >= 'A' && *str <= 'Z'){
            *str += 'a'-'A';
        }
        str++;
    }
    return s;
}

int is_number(char *str)
{
    if(str == NULL)
        return -1;

    if(!strcmp(str,"0"))
        return 0;

    int i;
    int len = strlen(str);
    for(i=0;i<len;i++)
    {
        if(!isdigit(str[i]))
            return 0;
    }

    if(i == len && i > 0)
        return 1;

    return 0;
}


char *get_confilicted_name_first(const char *fullname,int isfolder)
{
    char *confilicted_name = NULL;
    char prefix_name[NORMALSIZE];
    char suffix_name[256];
    char parse_name[NORMALSIZE];
    char *p = NULL;
    char *p1 = NULL;
    //char *p2 = NULL;
    char seq[8];
    int  num = 0;
    //int have_suf = 0;
    char *filename = NULL;
    char *path;
    int n = 0,j=0;
    //char seq_num[8];
    char con_filename[256];

    memset(prefix_name,0,sizeof(prefix_name));
    memset(suffix_name,0,sizeof(suffix_name));
    memset(parse_name,0,sizeof(parse_name));
    memset(con_filename,0,sizeof(con_filename));

    filename = parse_name_from_path(fullname);

    path = my_str_malloc(strlen(fullname)-strlen(filename)+1);
    strncpy(path,fullname,strlen(fullname)-strlen(filename)-1);

    confilicted_name = (char *)malloc(sizeof(char)*NORMALSIZE);


    if(isfolder)
    {
        strcpy(parse_name,filename);
    }
    else
    {
        p = strrchr(filename,'.');

        //printf("p=%s\n",p);

        if(p && filename[0] != '.')
        {
            strncpy(parse_name,filename,strlen(filename)-strlen(p));
            strcpy(suffix_name,p);
        }
        else
        {
            strcpy(parse_name,filename);
        }
    }

    //printf("parse_name=%s,suffix_name=%s\n",parse_name,suffix_name);

    if(num == 0)
    {
        strcpy(prefix_name,parse_name);
    }

    snprintf(prefix_name,252-strlen(case_conflict_name)-strlen(suffix_name),"%s",prefix_name);
    snprintf(con_filename,256,"%s(%s)%s",prefix_name,case_conflict_name,suffix_name);
    snprintf(confilicted_name,NORMALSIZE,"%s/%s",path,con_filename);

    //printf("------ prefix name is %s,num is %d,suffix name is %s -----\n",prefix_name,num,suffix_name);

    my_free(filename);
    my_free(path);

    return confilicted_name;
}

char *get_confilicted_name_second(const char *fullname,int isfolder,char *prefix_name)
{
    char *confilicted_name = NULL;
    char suffix_name[256];
    char parse_name[NORMALSIZE];
    char *p = NULL;
    char *p1 = NULL;
    //char *p2 = NULL;
    char seq[8];
    int  num = 0;
    //int have_suf = 0;
    char *filename = NULL;
    char *path;
    int n = 0,j=0;
    //char seq_num[8];
    char con_filename[256];
    char cmp_name[128] = {0};

    memset(suffix_name,0,sizeof(suffix_name));
    memset(parse_name,0,sizeof(parse_name));
    memset(con_filename,0,sizeof(con_filename));

    filename = parse_name_from_path(fullname);

    path = my_str_malloc(strlen(fullname)-strlen(filename)+1);
    strncpy(path,fullname,strlen(fullname)-strlen(filename)-1);

    //printf("%s\n",path);
    confilicted_name = (char *)malloc(sizeof(char)*NORMALSIZE);


    if(isfolder)
    {
        strcpy(parse_name,filename);
    }
    else
    {
        p = strrchr(filename,'.');

        //printf("p=%s\n",p);

        if(p && filename[0] != '.')
        {
            strncpy(parse_name,filename,strlen(filename)-strlen(p));
            strcpy(suffix_name,p);
            //have_suf = 1;
        }
        else
        {
            strcpy(parse_name,filename);
        }
    }

    p = NULL;

    p = strrchr(parse_name,'(');

    if(p)
    {
        p1 = strchr(p,')');
        if(p1)
        {
            p++;
            memset(seq,0,sizeof(seq));
            strncpy(seq,p,strlen(p)-strlen(p1));
            if(is_number(seq))
            {
                num = atoi(seq);
                num++;
                //printf("seq is %s,num is %d\n",seq,num);
                n = num;
                while((n=(n/10)))
                {
                    j++;
                }
            }
        }
    }

    //printf("parse_name=%s,suffix_name=%s\n",parse_name,suffix_name);

    if(num == 0)
    {
        sprintf(cmp_name,"(%s(1))",case_conflict_name);

    }
    else
    {
        sprintf(cmp_name,"(%s(%d))",case_conflict_name,num);
    }
    //printf("%s\n",cmp_name);
    //printf("%s\n",prefix_name);
    snprintf(prefix_name,252-strlen(cmp_name)-strlen(suffix_name),"%s",prefix_name);
    //printf("%s\n",prefix_name);
    snprintf(con_filename,256,"%s%s%s",prefix_name,cmp_name,suffix_name);
    //printf("%s\n",con_filename);
    snprintf(confilicted_name,NORMALSIZE,"%s/%s",path,con_filename);
    //printf("%s\n",confilicted_name);

    //printf("------ prefix name is %s,num is %d,suffix name is %s -----\n",prefix_name,num,suffix_name);

    my_free(filename);
    my_free(path);

    return confilicted_name;
}

char *get_prefix_name(const char *fullname,int isfolder)
{
    char *parse_name = NULL;
    char *p = NULL;

    char *filename = NULL;

    filename = parse_name_from_path(fullname);

    p = strrchr(filename,'.');

    if(p && filename[0] != '.' && !isfolder)
    {
        parse_name = my_str_malloc(strlen(filename)-strlen(p)+1);
        strncpy(parse_name,filename,strlen(filename)-strlen(p));
    }
    else
    {
        parse_name = my_str_malloc(strlen(filename)+1);
        strcpy(parse_name,filename);
    }
    my_free(filename);
    return parse_name;

}
char *get_confilicted_name_case(const char *fullname,int is_folder)
{
    char *g_newname = NULL;
    char *tmp_name = malloc(strlen(fullname)+1);
    memset(tmp_name,0,strlen(fullname)+1);
    sprintf(tmp_name,"%s",fullname);

    char *prefix_name = get_prefix_name(tmp_name,is_folder);//a.txt --> a
    //printf("%s\n",prefix_name);
    g_newname = get_confilicted_name_first(tmp_name,is_folder);//a.txt-->a(case-conflict).txt
    //printf("%s\n",g_newname);
    if(access(g_newname,F_OK) == 0)//a(case-conflict).txt-->a(case-conflict(n)).txt
    {
        //printf("111\n");
        my_free(tmp_name);
        tmp_name = malloc(strlen(g_newname)+1);
        memset(tmp_name,0,strlen(g_newname)+1);
        sprintf(tmp_name,"%s",g_newname);
        my_free(g_newname);
        while(!exit_loop)
        {
            g_newname = get_confilicted_name_second(tmp_name,is_folder,prefix_name);
            //printf("confilicted_name=%s\n",confilicted_name);
            if(access(g_newname,F_OK) == 0)
            {
                my_free(tmp_name);
                tmp_name = malloc(strlen(g_newname)+1);
                memset(tmp_name,0,strlen(g_newname)+1);
                sprintf(tmp_name,"%s",g_newname);
                my_free(g_newname);
                //have_same = 1;
            }
            else
                break;
        }
        my_free(tmp_name);
    }else
    {
        //printf("222\n");
        my_free(tmp_name);
    }
    my_free(prefix_name);
    return g_newname;
}

char *get_confilicted_name(const char *fullname,int isfolder)
{
    char *confilicted_name = NULL;
    char prefix_name[NORMALSIZE];
    char suffix_name[256];
    char parse_name[NORMALSIZE];
    char *p = NULL;
    char *p1 = NULL;
    //char *p2 = NULL;
    char seq[8];
    int  num = 0;
    //int have_suf = 0;
    char *filename = NULL;
    char path[512];
    int n = 0,j=0;
    //char seq_num[8];
    char con_filename[256];

    memset(prefix_name,0,sizeof(prefix_name));
    memset(suffix_name,0,sizeof(suffix_name));
    memset(parse_name,0,sizeof(parse_name));
    memset(path,0,sizeof(path));
    memset(con_filename,0,sizeof(con_filename));

    filename = parse_name_from_path(fullname);

    strncpy(path,fullname,strlen(fullname)-strlen(filename)-1);

    confilicted_name = (char *)malloc(sizeof(char)*NORMALSIZE);


    if(isfolder)
    {
        strcpy(parse_name,filename);
    }
    else
    {
        p = strrchr(filename,'.');

        //printf("p=%s\n",p);

        if(p && filename[0] != '.')
        {
            strncpy(parse_name,filename,strlen(filename)-strlen(p));
            strcpy(suffix_name,p);
            //have_suf = 1;
        }
        else
        {
            strcpy(parse_name,filename);
        }
    }

    p = NULL;

    p = strrchr(parse_name,'(');

    if(p)
    {
        p1 = strchr(p,')');
        if(p1)
        {
            p++;
            memset(seq,0,sizeof(seq));
            strncpy(seq,p,strlen(p)-strlen(p1));
            if(is_number(seq))
            {
                num = atoi(seq);
                num++;
                //printf("seq is %s,num is %d\n",seq,num);
                n = num;
                while((n=(n/10)))
                {
                    j++;
                }


                strncpy(prefix_name,parse_name,strlen(parse_name)-strlen(p)-1);
            }
        }
    }

    //printf("parse_name=%s,suffix_name=%s\n",parse_name,suffix_name);

    if(num == 0)
    {
        num = 1;
        strcpy(prefix_name,parse_name);
    }

    snprintf(prefix_name,252-j-strlen(suffix_name),"%s",prefix_name);
    snprintf(con_filename,256,"%s(%d)%s",prefix_name,num,suffix_name);
    snprintf(confilicted_name,NORMALSIZE,"%s/%s",path,con_filename);

    //printf("------ prefix name is %s,num is %d,suffix name is %s -----\n",prefix_name,num,suffix_name);

    my_free(filename);

    return confilicted_name;
}

static void *xmalloc_fatal(size_t size) {
    if (size==0) return NULL;
    fprintf(stderr, "Out of memory.");
    exit(1);
}
void *xcalloc (size_t nmemb, size_t size) {
    void *ptr = calloc (nmemb, size);
    if (ptr == NULL) return xmalloc_fatal(nmemb*size);
    return ptr;
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
            snprintf(&ns[strindex], 4, "%%%02X", in);
            strindex+=3;
            break;
        }
        string++;
    }
    ns[strindex]=0;
    return ns;
}

/*   PLAINTEXT */
char *oauth_sign_plaintext (const char *m, const char *k) {
    return(oauth_url_escape(k));
}

//google.c****************************
void show(queue_entry_t pTemp)
{
    DEBUG(">>>>>>show socketlist>>>>>>>>>>>>>\n");
    while(pTemp!=NULL)
    {
        DEBUG(">>>>>>>>>>>>>>>>>>>>>>>>>\n");
        DEBUG(">>>>%s\n",pTemp->cmd_name);
        DEBUG(">>>>%s\n",pTemp->re_cmd);
        DEBUG("\n");
        pTemp=pTemp->next_ptr;
    }
}

char *parse_name_from_path(const char *path)
{
    char *name;
    char *p;

    p = strrchr(path,'/');

    if( p == NULL)
    {
        //free(name);
        return NULL;
    }

    p++;

    name = (char *)malloc(sizeof(char)*(strlen(p)+2));
    memset(name,0,strlen(p)+2);

    strcpy(name,p);

    return name;
}


int add_action_item(const char *action,const char *path,action_item *head){

    DEBUG("add_action_item,action = %s,path = %s\n",action,path);

    action_item *p1,*p2;

    p1 = head;

    p2 = (action_item *)malloc(sizeof(action_item));
    memset(p2,'\0',sizeof(action_item));
    p2->action = (char *)malloc(sizeof(char)*(strlen(action)+1));
    p2->path = (char *)malloc(sizeof(char)*(strlen(path)+1));
    memset(p2->action,'\0',strlen(action)+1);
    memset(p2->path,'\0',strlen(path)+1);

    sprintf(p2->action,"%s",action);
    sprintf(p2->path,"%s",path);

    while(p1->next != NULL)
        p1 = p1->next;

    p1->next = p2;
    p2->next = NULL;

    DEBUG("add action item OK!\n");

    return 0;
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
    DEBUG("can not find action item_2\n");
    return NULL;
}

int write_log(int status, char *message, char *filename,int index)
{
    if(exit_loop)
    {
        return 0;
    }
    printf("write log status = %d\n",status);
    pthread_mutex_lock(&mutex_log);
    DEBUG("1093\n");
    Log_struc log_s;
    FILE *fp;
    int mount_path_length;
    int ret;
    struct timeval now;
    struct timespec outtime;

    DEBUG("1101 line\n");
    if(status == S_SYNC && exit_loop ==0)
    {
        DEBUG("write_log,status=S_SYNC\n");
        ret = api_accout_info();
        if(ret==0)
        {
            cJSON *json=dofile(Con(TMP_R,data_3.txt));
            cJSON_printf_account_info(json);
            if(json)
                cJSON_Delete(json);
        }

    }
    long long int totalspace = server_quota;

    DEBUG("1116 line\n");
    //mount_path_length = strlen(mount_path);
    mount_path_length = asus_cfg.prule[index]->base_path_len;

    memset(&log_s,0,LOG_SIZE);

    log_s.status = status;
    DEBUG("1124 line\n");
    fp = fopen(general_log,"w");
    DEBUG("1126 line\n");
    if(fp == NULL)
    {

        DEBUG("open %s error\n",general_log);

        pthread_mutex_unlock(&mutex_log);
        return -1;
    }

    if(log_s.status == S_ERROR)
    {

        DEBUG("******** status is ERROR *******\n");

        strcpy(log_s.error,message);
        fprintf(fp,"STATUS:%d\nERR_MSG:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,log_s.error,totalspace,server_normal+server_shared,index);
    }
    else if(log_s.status == S_DOWNLOAD)
    {

        DEBUG("******** status is DOWNLOAD *******\n");

        strcpy(log_s.path,filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,asus_cfg.prule[index]->base_path,log_s.path+mount_path_length,totalspace,server_normal+server_shared,index);
    }
    else if(log_s.status == S_UPLOAD)
    {

        DEBUG("******** status is UPLOAD *******\n");

        strcpy(log_s.path,filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,asus_cfg.prule[index]->base_path,log_s.path+mount_path_length,totalspace,server_normal+server_shared,index);
    }
    else
    {
        //printf("write log status2 = %d\n",status);

        if (log_s.status == S_INITIAL)
            DEBUG("******** other status is INIT *******\n");
        else
            DEBUG("******** other status is SYNC *******\n");

        fprintf(fp,"STATUS:%d\nTOTAL_SPACE:%lld\nUSED_SPACE:%lld\nRULENUM:%d\n",
                log_s.status,totalspace,server_normal+server_shared,index);
        //fprintf(fp,"%d\n",log_s.status);
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
    DEBUG("write_log return 0\n");
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


int parse_create_folder(char *filename)
{
    DEBUG("parse_create_folder!,filename = %s\n", filename);
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp == NULL)
    {
        DEBUG("filename %s is not exit\n",filename);
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
    if(strstr(tmp,"403") != NULL)
    {
        return 1;
    }
    return 0;

}
#if 0
char *change_server_same_name(char *fullname,int index){

    int i = 1;
    int exist;
    char *filename = NULL;
    char *temp_name = NULL;
    //char *temp_suffix = ".asus.td";
    int len = 0;
    char *path;
    char newfilename[512];
    int exit = 1;

    char *fullname_tmp = NULL;
    fullname_tmp = my_str_malloc(strlen(fullname)+1);
    sprintf(fullname_tmp,"%s",fullname);


    filename = parse_name_from_path(fullname_tmp);
    len = strlen(filename);
    //len = 6;

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
        sprintf(newfilename,"%s(%d)",newfilename,i);

        DEBUG("newfilename = %s\n",newfilename);

        i++;

        temp_name = my_str_malloc((size_t)(strlen(path)+strlen(newfilename)+1));
        sprintf(temp_name,"%s%s",path,newfilename);

        //char *serverpath;
        //serverpath = localpath_to_serverpath(temp_name);

        //do{

        DEBUG("temp_name = %s\n",temp_name);


        exist = is_server_exist(path,temp_name,index);
        //}while(exist == -2);

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

char *change_server_same_name(char *fullname,char *URL, int index)
{
    DEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!change_server_same_name start,fullname = %s\n", fullname);
    char *newname;
    char *tmp_name = malloc(strlen(fullname)+1);
    memset(tmp_name,0,strlen(fullname)+1);
    sprintf(tmp_name,"%s",fullname);
    int is_folder = test_if_dir(fullname);
    int exist;
    int len;
    char *filename;
    char *path;

    filename = parse_name_from_path(fullname);
    len = strlen(filename);

    path = my_str_malloc((size_t)(strlen(fullname)-len+1));

    DEBUG("fullname = %s\n",fullname);

    snprintf(path,strlen(fullname)-len+1,"%s",fullname);

    while(!exit_loop)
    {
        newname = get_confilicted_name(tmp_name,is_folder);
        DEBUG("confilicted_name=%s!!!\n",newname);
        DEBUG("is_server_exist2\n");
        exist = is_server_exist(path,newname,URL, index);

        if(exist == 1)
        {
            my_free(tmp_name);
            tmp_name = malloc(strlen(newname)+1);
            memset(tmp_name,0,strlen(newname)+1);
            sprintf(tmp_name,"%s",newname);
            my_free(newname);
            //have_same = 1;
        }
        else
            break;
    }
    my_free(path);
    my_free(filename);
    my_free(tmp_name);
    //DEBUG("6344line,change_server_same_name return\n");
    return newname;
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
    //while(*ch != '@')
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

    strcpy(temp,ch);
    //p = strchr(temp,'@');
    p = strchr(temp,'\n');

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));


    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    filename = (char *)malloc(sizeof(char)*(strlen(p)+2));
    memset(filename,'\0',strlen(p)+2);
    sprintf(filename,"%s",p);
    return filename;
}

/*
 1=>exit;
 0=>not exit;
 path=>serverpath;
 temp_name=>fullname;
 */
int is_server_exist(char *path,char *temp_name,char *URL, int index)
{
    int status;
    FileList_one = (CloudFile *)malloc(sizeof(CloudFile));
    memset(FileList_one,0,sizeof(CloudFile));

    FileList_one->href = NULL;
    FileList_one->name = NULL;

    FileTail_one = FileList_one;
    FileTail_one->next = NULL;
    //DEBUG("g_pSyncList[index]->ServerRootNode = %p\n", g_pSyncList[index]->ServerRootNode);
    //free_server_tree(g_pSyncList[index]->ServerRootNode);
    //g_pSyncList[index]->ServerRootNode = NULL;
    //g_pSyncList[index]->ServerRootNode = create_server_treeroot();
    //browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode);
    DEBUG("@@@@@@@@@@@@@@@@is_server_exist,path = %s, temp_name:%s\n",path, temp_name);
    status=api_metadata_one(path, temp_name,dofile, URL,index);
    //DEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!api_metadata_one end\n");
    if(status == -2)
    {
        free_CloudFile_item(FileList_one);
        DEBUG("@@@@@@@@@@@@@@@@@@is_server_exist (%s)return-2:exist\n", temp_name);
        return -2;
    }
    if(status == -1)
    {
        free_CloudFile_item(FileList_one);
        DEBUG("@@@@@@@@@@@@@@@@@is_server_exist error,return 0\n");
        return -1;
    }
    //free_server_tree(g_pSyncList[index]->ServerRootNode);
    //DEBUG("api_metadata_one end, status = %d\n", status);
    CloudFile *de_filecurrent;
    de_filecurrent=FileList_one->next;
    while(de_filecurrent != NULL)
    {
        if(de_filecurrent->href != NULL)
        {
            DEBUG("de_filecurrent->href:%s\n",de_filecurrent->href);
            DEBUG("temp_name           :%s\n",temp_name);
            if((status=strcmp(de_filecurrent->href,temp_name))==0)
            {
                strcpy(delete_id, de_filecurrent->id);
                free_CloudFile_item(FileList_one);
                DEBUG("@@@@@@@@@@@@@@@@@@is_server_exist (%s)return1:exist\n", temp_name);
                return 1;
            }

        }
        de_filecurrent = de_filecurrent->next;
    }
    free_CloudFile_item(FileList_one);
    DEBUG("@@@@@@@@@@@@@@@@is_server_exist(%s) return0:not exist\n", temp_name);
    return 0;

}

void cjson_change_to_cloudfile(cJSON *q)
{
    //DEBUG("cjson_change_to_cloudfile, q->string=%s\n", cjson_change_to_cloudfile);
    //printf("q->string=%s\n",q->string);
    if(strcmp(q->string,"bytes")==0)
    {
        FolderTmp->size=q->valueint;
        //DEBUG("%s:%lld\n",q->string,FolderTmp->size);
    }
    else if(strcmp(q->string,"path")==0)
    {
        FolderTmp->href=(char *)malloc(sizeof(char)*(strlen(q->valuestring)+1));
        strcpy(FolderTmp->href,q->valuestring);
        FolderTmp->name=parse_name_from_path(FolderTmp->href);
        //DEBUG("@@@@@@@@@@@@@@@@FolderTmp->href,%s:%s\n",q->string,FolderTmp->href);
        //DEBUG("@@@@@@@@@@@@@@@@FolderTmp->name,%s:%s\n",q->string,FolderTmp->name);
    }
    else if(strcmp(q->string,"is_dir")==0)
    {
        FolderTmp->isFolder=q->type;
        //DEBUG("%s:%d\n",q->string,FolderTmp->isFolder);
    }
    else if(strcmp(q->string,"modified")==0)
    {
        strcpy(FolderTmp->tmptime,q->valuestring);
        FolderTmp->mtime=ne_rfc1123_parse(FolderTmp->tmptime);
        //DEBUG("%s:%s\n",q->string,FolderTmp->tmptime);
    }
}
void cjson_change_to_cloudfile2(cJSON *q)
{
    if(strcmp(q->string,"title")==0)
    {
        FolderTmp->href=(char *)malloc(sizeof(char)*(strlen(q->valuestring)+1));
        strcpy(FolderTmp->href,q->valuestring);
        FolderTmp->name=q->valuestring;
        //DEBUG("!FolderTmp->name=%s\n", FolderTmp->name);
    }

}
char *cJSON_parse_name(cJSON *json)
{
    cJSON *q;
    q=json->child;
    while(q!=NULL)
    {
        if(strcmp(q->string,"title")==0)
        {
            DEBUG("title : %s\n",q->valuestring);
            //name=malloc(strlen(q->valuestring)+1);
            //name=parse_name_from_path(q->valuestring);
            //printf("name : %s\n",name);
            return q->valuestring;
        }
        q=q->next;
    }
}

/*char *cJSON_parse_name2(cJSON *json)
{
    char *name;
    char *parents_idd;
    cJSON *item_title, *item_parents, *parents_child, *parents_id;
    item_title = cJSON_GetObjectItem( json , "title");
    if(item_title == NULL){
        printf("fail to get item_title\n");
    }
    if( item_title->type == cJSON_String ){
        name=(char *)malloc(sizeof(char)*(strlen(item_title->valuestring)+1));
        strcpy(name,item_title->valuestring);
    }

    item_parents = cJSON_GetObjectItem( json , "parents");
    if(item_parents == NULL){
        printf("fail to get item_parents\n");
       // continue;
    }
    parents_child=cJSON_GetArrayItem(item_parents,0);
    if(parents_child == NULL){
        printf("fail to get parents_child\n");
    }
    parents_id = cJSON_GetObjectItem( parents_child , "id");
    if(parents_child == NULL){
        printf("fail to get parents_id\n");
    }
    if( parents_id->type == cJSON_String ){
        parents_idd=(char *)malloc(sizeof(char)*(strlen(parents_id->valuestring)+1));
        strcpy(parents_idd,parents_id->valuestring);
        //printf("FolderTmp%d->parents_id=%s\r\n",i, FolderTmp->parents_id);
    }

}*/
time_t cJSON_printf_expires(cJSON *json)
{
    if(json)
    {
        time_t mtime=(time_t)-1;
        cJSON *q;
        q=json->child;
        while(q!=NULL)
        {
            if(strcmp(q->string,"expires") == 0)
            {
                mtime=ne_rfc1123_parse(q->valuestring);
            }
            q=q->next;
        }
        return mtime;
    }
    else
        return (time_t)-1;
}

int cJSON_printf_dir(cJSON *json)
{
    DEBUG("cJSON_printf_dir start\n");
    if(json)
    {
        cJSON *item_mimeType;
            item_mimeType = cJSON_GetObjectItem( json , "trashed");
            if(item_mimeType == NULL){
                printf("fail to get item_mimeType\n");
                return 0;
            }
            if( item_mimeType->type == cJSON_True )
                return 1;
    }
    return 0;
}

cJSON_printf_new_access_token(cJSON *json)
{
    DEBUG("cJSON_printf_new_access_token start!\n");
    if(json)
    {
        DEBUG("json != NULL!\n");
        cJSON *item_token;
        DEBUG("327\n");
            item_token = cJSON_GetObjectItem( json , "access_token");
            DEBUG("329\n");
            if(item_token == NULL){
                DEBUG("fail to get item__token\n");
                return 0;
            }
            if( item_token->type == cJSON_String )
            {
                DEBUG("item_token->valuestring = %s\n", item_token->valuestring);
                strcpy(auth->oauth_token, item_token->valuestring);
                DEBUG("auth->oauth_token = %s\n", auth->oauth_token);
                return 1;
            }
    }
    return 0;
}

time_t cJSON_printf_mtime(cJSON *json)
{
    if(json)
    {
        time_t mtime=(time_t)-1;
        char modtime[32] = {0};
        cJSON *item_mtime;

        item_mtime = cJSON_GetObjectItem( json , "modifiedDate");
        if(item_mtime == NULL){
            printf("fail to get item_title\n");
            return (time_t)-1;
        }
        if( item_mtime->type == cJSON_String ){
            strcpy(modtime,item_mtime->valuestring);
            //printf("modtime=%s\n", modtime);
            mtime =ne_rfc1123_parse2(modtime);
            //printf("mtime = %lu\n", mtime);
            return mtime;
        }
     }
    else
        return (time_t)-1;
}
/*
 FileTail_one is not only file list ,it is also folder list
*/




time_t cJSON_printf(cJSON *json,char *string)
{
    DEBUG("cJSON_printf start!\n");
    cJSON *get_mtime;
    get_mtime = cJSON_GetObjectItem( json , "modifiedDate");
    if(get_mtime == NULL){
        printf("fail to get get_mtime");
        return -1;
    }
    if( get_mtime->type == cJSON_String ){
       // strcpy(mtime,get_mtime->valuestring);
        //printf("rootid=%s\n", mtine);
    }

}

int  cJSON_printf_account_info(cJSON *json)
{
    //DEBUG("cJSON_printf_account_info\n");
    cJSON *item_rootid,*item_total,*item_used,*item_trash;

        item_rootid = cJSON_GetObjectItem( json , "rootFolderId");
        if(item_rootid == NULL){
            printf("fail to get item_rootid");
            return -1;
        }
        if( item_rootid->type == cJSON_String ){
            strcpy(rootid,item_rootid->valuestring);
            printf("#####rootid=%s\n", rootid);
        }

        item_total = cJSON_GetObjectItem( json , "quotaBytesTotal");
        if(item_total == NULL){
            printf("fail to get item_total");
        }
        if( item_total->type == cJSON_String ){
        //DEBUG("item_total->valuestring=%s\n",  item_total->valuestring);
        server_quota = atoll(item_total->valuestring);
            //printf("server_quota=%lld\n", server_quota);
       }

        item_used = cJSON_GetObjectItem( json , "quotaBytesUsed");
        if(item_used == NULL){
            printf("fail to get item_used");
        }
        if( item_used->type == cJSON_String ){
        //DEBUG("item_used->valuestring=%s\n",  item_used->valuestring);
            server_shared = atoll(item_used->valuestring);
           // printf("server_shared=%lld\n", server_shared);
        }

        item_trash = cJSON_GetObjectItem( json , "quotaBytesUsedInTrash");
        if(item_trash == NULL){
            printf("fail to get item_trash");
        }
        //if( item_trash->type == cJSON_String ){
        //DEBUG("item_trash->valuestring=%s\n",  item_trash->valuestring);
            server_normal = atoll(item_trash->valuestring);
            //printf("server_normal=%lld\n", server_normal);
       // }
}

void cjson_to_space(cJSON *q)
{
    //printf("q->string = %s,q->valueint = %d,q->valuedouble = %lf,q->valuelong = %lld\n",q->string,q->valueint,q->valuedouble,q->valuelong);
    if(strcmp(q->string,"shared") == 0)
    {
        server_shared=q->valuelong;
    }
    else if(strcmp(q->string,"quota") == 0)
    {
        server_quota=q->valuelong;
    }
    else if(strcmp(q->string,"normal") == 0)
    {
        server_normal=q->valuelong;
    }
}
cJSON *doit(char *text)
{
    //printf("diit\n");
    char *out;cJSON *json;
    text = strstr(text, "{");
    json=cJSON_Parse(text);
    //DEBUG("cJSON_Parse end,json=%p\n", json);
    if (!json) {DEBUG("Error before: [%s]\n",cJSON_GetErrorPtr());return NULL;}
    else
    {
        return json;
        //cJSON_printf(json);
        //cJSON_Delete(json);
        //printf("%s\n",out);
        //free(out);
    }
}

/* Read a file, parse, render back, etc. */
cJSON *dofile(char *filename)
{
    cJSON *json;
    FILE *f=fopen(filename,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
    char *data=malloc(len+1);fread(data,1,len,f);fclose(f);
    //DEBUG("***dofile,data=%s\n", data);
    json=doit(data);
    free(data);
    if(json)
        return json;
    else
        return NULL;
}

cJSON_printf_insert(cJSON *json, char *newid)
{
    cJSON *item_id;
    if(json)
    {
        item_id = cJSON_GetObjectItem( json , "id");
        if(item_id == NULL){
            printf("fail to get item_id\n");
            return;
        }
        if( item_id->type == cJSON_String ){
            strcpy(newid,item_id->valuestring);
            printf("newid=%s\r\n", newid);
        }
    }
}

int cJSON_printf_one(char *parentref, cJSON *json)
{
    cJSON *json_item;
    cJSON *item_child,*item_id;


    //DEBUG("cJSON_printf!\n");
    if(json)
    {

         json_item = cJSON_GetObjectItem( json , "items");
         if(json_item == NULL){
            printf("fail to get item\n");
            return;
         }

         int iCount = cJSON_GetArraySize(json_item);
         int i ;
         for (i = 0; i < iCount; ++i)
         {
                     FolderTmp_One = (CloudFile *)malloc(sizeof(CloudFile));
                     memset(FolderTmp_One,0,sizeof(CloudFile));
                     FolderTmp_One->href=NULL;
                     FolderTmp_One->name=NULL;
                     FolderTmp_One->mimeType=NULL;
                     FolderTmp_One->id=NULL;
                     //FolderTmp->title=NULL;
                     FolderTmp_One->parents_id=NULL;
                     FolderTmp_One->exportLinks_document=NULL;


                    item_child=cJSON_GetArrayItem(json_item,i);
                    if (item_child == NULL ){
                    printf("fail to get item child\n");
                        continue;
                     }
                    item_id = cJSON_GetObjectItem( item_child , "id");
                    if(item_id == NULL){
                        printf("fail to get item_id\n");
                        continue;
                    }
                    if( item_id->type == cJSON_String ){
                        FolderTmp_One->id=(char *)malloc(sizeof(char)*(strlen(item_id->valuestring)+1));
                        strcpy(FolderTmp_One->id,item_id->valuestring);
                        //printf("1FolderTmp%d->id=%s\r\n",i, FolderTmp_One->id);
                        if(get_metadata(parentref, FolderTmp_One->id, dofile,FolderTmp_One, i) == -1)
                        {
                            free_cloudfile(FolderTmp_One);
                            FolderTmp_One = NULL;
                            return -1;
                        }
                    }

                    if (FolderTmp_One->isTrash)
                    {
                        free_cloudfile(FolderTmp_One);
                        FolderTmp_One = NULL;
                        continue;
                    }

                    if (strcmp(FolderTmp_One->mimeType, "application/vnd.google-apps.folder") == 0)
                    {
                        FolderTmp_One->isFolder = 1;
                        //printf("FolderTmp_One%d->isFolder=%d\n", i, FolderTmp_One->isFolder);
                    }
                    else
                        FolderTmp_One->isFolder = 0;

                    FileTail_one->next = FolderTmp_One;
                    FileTail_one = FolderTmp_One;
                    FileTail_one->next = NULL;
                    //printf("========================\n");
                }
    }
    }

time_t cJSON_batch_printf(char *parentref, cJSON *json,char *string, char *URL, int index)
{
    cJSON *json_item;
    cJSON *item_child,*item_id,*item_title,*item_mimeType,*item_modifiedDate,*item_parents;
    cJSON *parents_child,*parents_id;

    DEBUG("cJSON_batch_printf!\n");
    if(json)
    {

         json_item = cJSON_GetObjectItem( json , "items");
         if(json_item == NULL){
            printf("fail to get item\n");
            return -1;
         }

         int iCount = cJSON_GetArraySize(json_item);
         int i ;
         CloudFile *TreeFileTail1;
         CloudFile *TreeFolderTail1;
         main_batch();
         exit(-1);
    }
    else
        return -1;
}

time_t cJSON_printf2(char *parentref, cJSON *json,char *string, char *URL, int index)
{
    cJSON *json_item;
    cJSON *item_child,*item_id,*item_title,*item_mimeType,*item_modifiedDate,*item_parents;
    cJSON *parents_child,*parents_id;

    DEBUG("cJSON_printf2!\n");
    if(json)
    {

         json_item = cJSON_GetObjectItem( json , "items");
         if(json_item == NULL){
            printf("fail to get item\n");
            return -1;
         }

         int iCount = cJSON_GetArraySize(json_item);
         int i ;
         CloudFile *TreeFileTail1;
         CloudFile *TreeFolderTail1;

         for (i = 0; i < iCount; ++i)
         {
             //DEBUG("**************************************\ni=%d\n", i);
             TreeFileTail1 = TreeFileList->next;
             TreeFolderTail1 = TreeFolderList->next;

                     FolderTmp = (CloudFile *)malloc(sizeof(CloudFile));
                     memset(FolderTmp,0,sizeof(CloudFile));
                     FolderTmp->href=NULL;
                     FolderTmp->name=NULL;
                     FolderTmp->mimeType=NULL;
                     FolderTmp->id=NULL;
                     //FolderTmp->title=NULL;
                     FolderTmp->parents_id=NULL;
                     FolderTmp->exportLinks_document=NULL;


                    item_child=cJSON_GetArrayItem(json_item,i);
                    if (item_child == NULL ){
                    printf("fail to get item child\n");
                        continue;
                     }
                    item_id = cJSON_GetObjectItem( item_child , "id");
                    if(item_id == NULL){
                        printf("fail to get item_id\n");
                        continue;
                    }
                    if( item_id->type == cJSON_String ){
                        FolderTmp->id=(char *)malloc(sizeof(char)*(strlen(item_id->valuestring)+1));
                        strcpy(FolderTmp->id,item_id->valuestring);
                        //printf("2FolderTmp%d->id=%s\r\n",i, FolderTmp->id);
                        if(get_metadata(parentref, FolderTmp->id, dofile,FolderTmp, i) == -1)
                        {
                            DEBUG("get_metadata ERROR!\n");
                            free_cloudfile(FolderTmp);
                            FolderTmp = NULL;
                            return -1;
                        }
                    }
                    if (FolderTmp->isTrash)
                    {
                        free_cloudfile(FolderTmp);
                        FolderTmp = NULL;
                       //printf("========================\n");
                        continue;
                    }
                    if (strcmp(FolderTmp->mimeType, "application/vnd.google-apps.folder") == 0)
                    {
                        FolderTmp->isFolder = 1;
                        while(TreeFolderTail1 != NULL)
                        {
                            if(strcmp(TreeFolderTail1->href, FolderTmp->href) == 0)
                            {
                                //DEBUG("637line\n");
                                char *newname;
                                //DEBUG("!!!FolderTmp->id = %s\n", FolderTmp->id);
                                newname=change_server_same_name(FolderTmp->href,URL, index);
                                //DEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!!640line, newname = %s\n", newname);
                                //DEBUG("@@@FolderTmp->id = %s\n", FolderTmp->id);
                                api_rename(FolderTmp, newname, index, 0, NULL);
                                free(newname);
                            }
                            //DEBUG("643line\n");
                            TreeFolderTail1 = TreeFolderTail1->next;
                        }

                    }
                    else
                        FolderTmp->isFolder = 0;

                    if(FolderTmp->isFolder==0)
                    {
                        //DEBUG("653line\n");
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

                }
    }
    else
        return -1;
}

void get_filetype(char *name, char *filetype)
{
    //DEBUG("get_filetype start\n");
    char *suffix = name;
    //DEBUG("name=%s\n", name);
    //DEBUG("suffix=%s\n", suffix);
    //DEBUG("strlen(suffix)=%d\n", strlen(suffix));
     suffix += strlen(suffix);
    //DEBUG("1!!!\n");
    suffix -= 4;
    //DEBUG("2!!!\n");
    if(strcmp(suffix, ".doc") == 0)
        sprintf(filetype, "application/msword");
    if(strcmp(suffix, ".dcx") == 0)
        sprintf(filetype, "application/x-dcx");
    if(strcmp(suffix, ".jpg") == 0)
        sprintf(filetype, "image/jpeg");
    if(strcmp(suffix, ".ppt") == 0)
        sprintf(filetype, "application/vnd.ms-powerpoint");
    if(strcmp(suffix, ".xml") == 0)
        sprintf(filetype, "text/xml");
    if(strcmp(suffix, ".apk") == 0)
        sprintf(filetype, "application/vnd.android.package-archive");
    if(strcmp(suffix, ".mpg") == 0)
        sprintf(filetype, "video/mpg");
    if(strcmp(suffix, ".mp3") == 0)
        sprintf(filetype, "audio/mp3");
    if(strcmp(suffix, ".png") == 0)
        sprintf(filetype, "image/png");
    if(strcmp(suffix, ".rpm") == 0)
        sprintf(filetype, "audio/x-pn-realaudio-plugin");
    if(strcmp(suffix, ".txt") == 0)
        sprintf(filetype, "text/plain");
    if(strcmp(suffix, ".xls") == 0)
        sprintf(filetype, "application/vnd.ms-excel");
    if(strcmp(suffix, ".trx") == 0)
        sprintf(filetype, "application/octet-stream");


    suffix -=1;
    if(strcmp(suffix, ".jpeg") == 0)
        sprintf(filetype, "image/jpeg");
    if(strchr(name, '.') == NULL)
        sprintf(filetype, "application/octet-stream");

}

//*****************************
#ifdef OAuth1
int open_login_page_first()
{
    char Myurl[MAXSIZE];
    memset(Myurl,0,sizeof(Myurl));

    //const char url[]="https://www.google.com/login?cont=https%3A//www.google.com/1/oauth/authorize%3Foauth_token%3Dss4olgz45siiwdd&signup_tag=oauth&signup_data=177967";
    sprintf(Myurl,"%s","https://www.google.com/login");
    //sprintf(Myurl,"%s%s%s","https://www.google.com/login?cont=https%3A//www.google.com/1/oauth/authorize%3Foauth_token\%3D",auth->tmp_oauth_token,"&signup_tag=oauth&signup_data=177967");
    DEBUG("Myurl:%s\n",Myurl);
    CURL *curl;
    CURLcode res;
    FILE *fp;

    //curl_global_init(CURL_GLOBAL_ALL);
    curl=curl_easy_init();
    if(curl){
        struct curl_slist *headers_l=NULL;
        static const char header1_l[]="Host:www.google.com";
        static const char header2_l[]="Mozilla/5.0 (Windows NT 6.1; rv:17.0) Gecko/20100101 Firefox/17.0";
        static const char header3_l[]="Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        static const char header4_l[]="zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3";
        //static const char header5_l[]="Accept-Encoding:gzip, deflate";
        static const char header6_l[]="Connection:keep-alive";
        //static const char header7_l[]="Referer:https://www.box.net/api/1.0/auth/a26uln89kmbh0h97e0zxl6jffxnef19f";
        //static const char header8_l[]="Content-Type:application/x-www-form-urlencoded";
        //static const char header9_l[]="Content-Length:237";

        headers_l=curl_slist_append(headers_l,header1_l);
        headers_l=curl_slist_append(headers_l,header2_l);
        headers_l=curl_slist_append(headers_l,header3_l);
        headers_l=curl_slist_append(headers_l,header4_l);
        //headers_l=curl_slist_append(headers_l,header5_l);
        headers_l=curl_slist_append(headers_l,header6_l);
        //headers_l=curl_slist_append(headers_l,header7_l);
        //headers_l=curl_slist_append(headers_l,header8_l);
        //headers_l=curl_slist_append(headers_l,header9_l);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        //curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headers_l);

        //curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"/tmp/cookie_open.txt");
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR,Con(TMP_R,cookie_open.txt));

        //#ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //#endif

#ifdef SKIP_HOSTNAME_VERFICATION
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
#endif

        fp=fopen(Con(TMP_R,xmldate1.xml),"w");

        //fp1=fopen("file.txt","w");
        if(fp==NULL){
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers_l);
            //curl_global_cleanup();
            return -1;
        }
        curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,60);
        //curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp1);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        res=curl_easy_perform(curl);
        fclose(fp);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers_l);
        if(res != 0){
            //curl_global_cleanup();
            DEBUG("open_login_page [%d] failed!\n",res);
            return -1;
        }
    }
    else
        DEBUG("url is wrong!!!");

    //curl_global_cleanup();


}
int
        login_first(void){

    CURL *curl;
    CURLcode res;
    FILE *fp;
    char Myurl[MAXSIZE]="\0";
    //char url[]="https://www.box.net/api/1.0/auth/";
    //sprintf(Myurl,"%s%s%s","https://www.google.com/login?cont=https%3A//www.google.com/1/oauth/authorize%3Foauth_token\%3D",auth->tmp_oauth_token,"&signup_tag=oauth&signup_data=177967");
    char *data=parse_login_page();
    //sprintf(Myurl,"%s?%s%s","https://www.google.com/1/oauth/authorize","oauth_token=",auth->tmp_oauth_token);
    sprintf(Myurl,"%s","https://www.google.com/login");
    struct curl_slist *headerlist=NULL;
    static const char buf[]="Expect:";
    //printf("data=%s\n",data);
    //char *data=parse_login_page();

    //curl_global_init(CURL_GLOBAL_ALL);
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,buf);

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        //curl_easy_setopt(curl,CURLOPT_URL,url);
        //curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);

        curl_easy_setopt(curl,CURLOPT_COOKIEFILE,Con(TMP_R,cookie_open.txt));//send first saved cookie
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR,Con(TMP_R,cookie_login.txt));

        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);


//#ifdef SKIP_HOSTNAME_VERFICATION
//        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
//#endif
        fp=fopen(Con(TMP_R,xmldate2.xml),"w");

        if(fp==NULL){
            curl_easy_cleanup(curl);
            curl_slist_free_all(headerlist);
            free(data);
            //curl_global_cleanup();
            return -1;
        }

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();

        if(res != 0){

            free(data);
            wdDEBUG("login_first [%d] failed!\n",res);
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
    char *url="https://www.google.com/1/oauth/authorize";
    sprintf(cont,"%s?oauth_token=%s",url,auth->tmp_oauth_token);
    //sprintf(cont,"%s?oauth_token=%s",url,"s1unnz0qytb4j35");
    memset(buf,'\0',sizeof(buf));
    memset(buff,'\0',sizeof(buff));

    fp=fopen(Con(TMP_R,xmldate1.xml),"r");

    while(!feof(fp)){
        fgets(buf,1024,fp);
        p=strstr(buf,s);
        if(p!=NULL){
            //strcat(buff,p);
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
    //sprintf(data,"t=%s&lhs_type=default&cont=%s&signup_tag=oauth&signup_data=177967&login_email=%s&login_password=%s&%s&%s&%s",
    //        buff,oauth_url_escape(cont),oauth_url_escape(info->usr),oauth_url_escape(info->pwd),"login_submit=1","remember_me=on","login_submit_dummy=Sign+in");
#if 1
    sprintf(data,"t=%s&lhs_type=default&cont=%s&signup_tag=oauth&signup_data=177967&login_email=%s&login_password=%s&%s&%s&%s",
            buff,cont_tmp,usr_tmp,pwd_tmp,"login_submit=1","remember_me=on","login_submit_dummy=Sign+in");
#else
    sprintf(data,"t=%s&cont=%s&signup_tag=oauth&signup_data=177967&display=desktop&login_email=%s&login_password=%s&%s&%s&%s",
            buff,cont_tmp,usr_tmp,pwd_tmp,"login_submit=1","remember_me=on","login_submit_dummy=Sign+in");
#endif
    //printf("m=%s\nbuff=%s\ndata=%s\n",m,buff,data);
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
        //printf("buf=%s\n",buf);
        p=strstr(buf,s);
        if(p!=NULL){
            //strcat(buff,p);
            break;
        }
    }
    fgets(buf,1024,fp);
    //printf("buf=%s\n",buf);
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
            strcpy(cookie,p);
            break;
        case 1:
            break;
        default:
            break;
        }
        i++;
        p=strtok(NULL,m);
    }
    //printf("%c\n",cookie[strlen(buf)+1]);
    if(cookie[strlen(buf)+2] == '\n')
    {
        cookie[strlen(buf)+2] = '\0';
    }
    fclose(fp);
    //printf("cookie=%s\n",cookie);
    //free(cookie);
    return cookie;

}
int login_second()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char Myurl[MAXSIZE]="\0";
    //char url[]="https://www.box.net/api/1.0/auth/";
    //sprintf(Myurl,"%s%s%s","https://www.google.com/login?cont=https%3A//www.google.com/1/oauth/authorize%3Foauth_token\%3D",auth->tmp_oauth_token,"&signup_tag=oauth&signup_data=177967");
    //char *data=parse_login_page();
    char *data=malloc(256);
    memset(data,0,256);
    char *cookie=parse_cookie();
#if 1
    sprintf(data,"t=%s&allow_access=Allow&oauth_token=%s&display&osx_protocol",cookie,auth->tmp_oauth_token);
#else
    sprintf(data,"t=%s&allow_access=Allow&saml_assertion&embedded&osx_protocol&oauth_token=%s&display&oauth_callback&user_id=150377145",cookie,auth->tmp_oauth_token);
#endif
    //sprintf(Myurl,"%s?%s%s","https://www.google.com/1/oauth/authorize","oauth_token=",auth->tmp_oauth_token);
#if 1
    sprintf(Myurl,"%s","https://www.google.com/1/oauth/authorize");
#else
    sprintf(Myurl,"%s","https://www.google.com/1/oauth/authorize_submit");
#endif
    struct curl_slist *headerlist=NULL;
    static const char buf[]="Expect:";
    //printf("data=%s\n",data);
    //char *data=parse_login_page();

    //curl_global_init(CURL_GLOBAL_ALL);
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,buf);

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);get_type
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        //curl_easy_setopt(curl,CURLOPT_URL,url);
        //curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
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
            //curl_global_cleanup();
            return -1;
        }

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res != 0){
            free(data);
            free(cookie);
            DEBUG("login_second [%d] failed!\n",res);
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
        //printf("buf=%s\n",buf);
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


//    FILE *fp = NULL;
//    fp = fopen(Con(TMP_R,xmldate3.xml),"r");
//    char buf[512] ="\0";
//    char *s="name=\"user_id\" value=\"";
//    char *sb="\"";
//    char *p = NULL;
//    char *pt = NULL;
//    char *user_id = NULL;
//    int user_id_length;
//    while(!feof(fp)){
//        fgets(buf,512,fp);
//        printf("buf=%s\n",buf);
//        p=strstr(buf,s);
//        if(p!=NULL){
//            printf("buf=%s\n",buf);
//            p+=strlen(s);
//            pt = strstr(p,sb);
//            user_id_length = strlen(p) - strlen(pt);
//            user_id = malloc(user_id_length+1);
//            memset(user_id,0,user_id_length+1);
//            printf("p=%s\n",p);
//            printf("pt=%s\n",pt);
//            snprintf(user_id,user_id_length+1,"%s",p);
//            break;
//        }
//    }
//    fclose(fp);
    DEBUG("user_id = %s\n",user_id);
    return user_id;
}

int login_second_submit()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char Myurl[MAXSIZE]="\0";
    //char url[]="https://www.box.net/api/1.0/auth/";
    //sprintf(Myurl,"%s%s%s","https://www.google.com/login?cont=https%3A//www.google.com/1/oauth/authorize%3Foauth_token\%3D",auth->tmp_oauth_token,"&signup_tag=oauth&signup_data=177967");
    //char *data=parse_login_page();
    char *data=malloc(256);
    memset(data,0,256);
    char *cookie=parse_cookie();
    char *user_id = pare_login_second_response();
#if 0
    sprintf(data,"t=%s&allow_access=Allow&oauth_token=%s&display&osx_protocol",cookie,auth->tmp_oauth_token);
#else
    sprintf(data,"t=%s&allow_access=Allow&saml_assertion&embedded&osx_protocol&oauth_token=%s&display&oauth_callback&user_id=%s",cookie,auth->tmp_oauth_token,user_id);
#endif
    //sprintf(Myurl,"%s?%s%s","https://www.google.com/1/oauth/authorize","oauth_token=",auth->tmp_oauth_token);
#if 0
    sprintf(Myurl,"%s","https://www.google.com/1/oauth/authorize");
#else
    sprintf(Myurl,"%s","https://www.google.com/1/oauth/authorize_submit");
#endif
    free(user_id);
    struct curl_slist *headerlist=NULL;
    static const char buf[]="Expect:";
    //printf("data=%s\n",data);
    //char *data=parse_login_page();

    //curl_global_init(CURL_GLOBAL_ALL);
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,buf);

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,Myurl);
        //curl_easy_setopt(curl,CURLOPT_URL,url);
        //curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
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
            //curl_global_cleanup();
            return -1;
        }

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res != 0){
            free(data);
            free(cookie);
            DEBUG("login_second_submit [%d] failed!\n",res);
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
        curl_easy_setopt(curl,CURLOPT_URL,"https://api.google.com/1/oauth/access_token");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);

        fp=fopen(Con(TMP_R,data_2.txt),"w");

        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            DEBUG("get_access_token [%d] failed!\n",res);
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

char *path_to_id(char *path, int index)
{

    //DEBUG("path_to_id, path = %s\n", path);
    CloudFile *filetmp = NULL;
    //DEBUG("before get_CloudFile_node0x1\n");
    //DEBUG("g_pSyncList[index]->ServerRootNode=%p\n", g_pSyncList[index]->ServerRootNode);
    //******************
    if(!g_pSyncList[index]->ServerRootNode)
    {
        //DEBUG("path_to_parents_id--->browse_to_tree\n");
        free_server_tree(g_pSyncList[index]->ServerRootNode);
        g_pSyncList[index]->ServerRootNode = NULL;
        g_pSyncList[index]->ServerRootNode = create_server_treeroot();
        //DEBUG("browse_to_tree1\n");
        g_browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode, index);
    }
    //******************

    filetmp=get_CloudFile_node(g_pSyncList[index]->ServerRootNode,path,0x1);
    //DEBUG("after get_CloudFile_node0x1\n");
    if(filetmp != NULL)
    {
        //DEBUG("path_to_id, filetmp->id = %s\n", filetmp->id);
        return filetmp->id;
    }

    else
    {
        //DEBUG("before get_CloudFile_node0x2\n");
        filetmp = get_CloudFile_node(g_pSyncList[index]->ServerRootNode,path,0x2);
        //DEBUG("after get_CloudFile_node0x2\n");
        if(filetmp != NULL)
        {
            //DEBUG("!path_to_id,filetmp->id = %s\n", filetmp->id);
            return filetmp->id;
        }
    }
    if(filetmp == NULL)
    {
        free_server_tree(g_pSyncList[index]->ServerRootNode);
        g_pSyncList[index]->ServerRootNode = NULL;
        g_pSyncList[index]->ServerRootNode = create_server_treeroot();
        //DEBUG("browse_to_tree2\n");
        g_browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode, index);
        filetmp=get_CloudFile_node(g_pSyncList[index]->ServerRootNode,path,0x1);
        //DEBUG("2after get_CloudFile_node0x1\n");
        if(filetmp != NULL)
        {
            //DEBUG("2path_to_id, filetmp->id = %s\n", filetmp->id);
            return filetmp->id;
        }

        else
        {
            //DEBUG("2before get_CloudFile_node0x2\n");
            filetmp = get_CloudFile_node(g_pSyncList[index]->ServerRootNode,path,0x2);
            //DEBUG("2after get_CloudFile_node0x2\n");
            if(filetmp != NULL)
            {
                //DEBUG("!2path_to_id,filetmp->id = %s\n", filetmp->id);
                return filetmp->id;
            }
        }
    }
    //DEBUG("path_id, fail\n");
}
char *path_to_parents_id(char *path, int index)
{
    //free_server_tree(g_pSyncList[index]->ServerRootNode);
    //g_pSyncList[index]->ServerRootNode = NULL;
   // g_pSyncList[index]->ServerRootNode = create_server_treeroot();
    //browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode);

    DEBUG("path_to_parents_id start!\n");
    CloudFile *filetmp = NULL;
    char *path1;
    //DEBUG("before strrchr,path = %s\n", path);
    path1 = strrchr(path, '/');
    //DEBUG("after strrchr, path1 = %s\n", path1);
    char path3[strlen(path)-strlen(path1) + 2];
    memset(path3, '\0', sizeof(path3));

        snprintf(path3, strlen(path)-strlen(path1) + 1 + 1, path);
        if(strcmp(path3, "/") == 0)
        {
            DEBUG("parent id is %s\n", rootid);
            return rootid;
        }
        else
            snprintf(path3, strlen(path)-strlen(path1) + 1, path);
    /*//DEBUG("path3 = %s\n", path3);
    free_server_tree(g_pSyncList[index]->ServerRootNode);
    g_pSyncList[index]->ServerRootNode = NULL;
    g_pSyncList[index]->ServerRootNode = create_server_treeroot();
    //DEBUG("browse_to_tree3\n");
    browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode, index);*/
    if(!g_pSyncList[index]->ServerRootNode)
    {
        DEBUG("path_to_parents_id--->browse_to_tree\n");
        free_server_tree(g_pSyncList[index]->ServerRootNode);
        g_pSyncList[index]->ServerRootNode = NULL;
        g_pSyncList[index]->ServerRootNode = create_server_treeroot();
        //DEBUG("browse_to_tree1\n");
        g_browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode, index);
    }
        filetmp=get_CloudFile_node(g_pSyncList[index]->ServerRootNode,path3,0x1);
    //DEBUG("get_Cloud_file_node end, filetmp = %p\n", filetmp);
    if(filetmp != NULL)
    {
        //DEBUG("filetmp!!->id = %s\n", filetmp->id);
        return filetmp->id;
    }
    else
        return NULL;

    //******************
    /*if(!g_pSyncList[index]->ServerRootNode)
    {
        DEBUG("path_to_parents_id--->browse_to_tree\n");
        free_server_tree(g_pSyncList[index]->ServerRootNode);
        g_pSyncList[index]->ServerRootNode = NULL;
        g_pSyncList[index]->ServerRootNode = create_server_treeroot();
        browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode);
    }
    //******************
    filetmp=get_CloudFile_node(g_pSyncList[index]->ServerRootNode,path3,0x1);
    DEBUG("get_Cloud_file_node end, filetmp = %p\n", filetmp);
    if(filetmp != NULL)
    {
        DEBUG("filetmp!!->id = %s\n", filetmp->id);
        return filetmp->id;
    }
    else
    {
        free_server_tree(g_pSyncList[index]->ServerRootNode);
        g_pSyncList[index]->ServerRootNode = NULL;
        g_pSyncList[index]->ServerRootNode = create_server_treeroot();
        browse_to_tree("/", rootid,g_pSyncList[index]->ServerRootNode);
        filetmp=get_CloudFile_node(g_pSyncList[index]->ServerRootNode,path3,0x1);
        if(filetmp != NULL)
        {
            DEBUG("filetmp!!->id = %s\n", filetmp->id);
            return filetmp->id;
        }
        else
            return NULL;;
    }*/

}



int dragfolder_rename(char *dir,int index,time_t server_mtime)
{
    DEBUG("change dir = %s mtime\n",dir);

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

            sprintf(fullname,"%s/%s",dir,ent->d_name);
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

#define GMTOFF(t) ((t).tm_gmtoff)

static const char short_months[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* RFC1123: Sun, 06 Nov 1994 08:49:37 GMT */
#define RFC1123_FORMAT "%3s, %02d %3s %4d %02d:%02d:%02d GMT"
//Wed, 11 Nov 2015 06:08:39 +0000
//2015-11-09T12:54:13.980Z

time_t ne_rfc1123_parse(const char *date)
{
    struct tm gmt = {0};
    char wkday[4], mon[4];
    int n;
    time_t result;

    /*  it goes: Sun, 06 Nov 1994 08:49:37 GMT */
    n = sscanf(date, RFC1123_FORMAT,
               wkday, &gmt.tm_mday, mon, &gmt.tm_year, &gmt.tm_hour,
               &gmt.tm_min, &gmt.tm_sec);
    /* Is it portable to check n==7 here? */
    gmt.tm_year -= 1900;
    for (n=0; n<12; n++)
        if (strcmp(mon, short_months[n]) == 0)
            break;
    /* tm_mon comes out as 12 if the month is corrupt, which is desired,
     * since the mktime will then fail */
    gmt.tm_mon = n;
    printf("gmt.tm_year=%d\ngmt.tm_mon=%d\ngmt.tm_mday=%d\ngmt.tm_hour=%d\ngmt.tm_min=%d\ngmt.tm_sec=%d\n",gmt.tm_year,gmt.tm_mon,gmt.tm_mday,gmt.tm_hour,gmt.tm_min,gmt.tm_sec);

    gmt.tm_isdst = -1;
    result = mktime(&gmt);
    printf("1result=%lu,GMTOFF(gmt)=%ld\n",result, GMTOFF(gmt));
    return result + GMTOFF(gmt);
}


#define RFC1123_FORMAT2 "%4d-%02d-%02dT%02d:%02d:%02d GMT"
time_t ne_rfc1123_parse2(const char *date)
{
    //DEBUG("ne_rfc1123_parse2\n");
    struct tm gmt = {0};
    char  mon[4];
    int n;
    time_t result;

    n = sscanf(date, RFC1123_FORMAT2,
                  &gmt.tm_year,&gmt.tm_mon, &gmt.tm_mday, &gmt.tm_hour,
               &gmt.tm_min, &gmt.tm_sec);

    gmt.tm_year -= 1900;
    gmt.tm_mon -= 1;
    //printf("gmt.tm_year=%d\ngmt.tm_mon=%d\ngmt.tm_mday=%d\ngmt.tm_hour=%d\ngmt.tm_min=%d\ngmt.tm_sec=%d\n",gmt.tm_year,gmt.tm_mon,gmt.tm_mday,gmt.tm_hour,gmt.tm_min,gmt.tm_sec);
    gmt.tm_isdst = -1;
    result = mktime(&gmt);
    //printf("2result=%lu,GMTOFF(gmt)=%ld\n",result, GMTOFF(gmt));
    return result + GMTOFF(gmt);
}


api_insert(char *folderid, char *name, char *newid)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;

    DEBUG("api_insert\n");
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);

    char *postdata;
    postdata=(char *)malloc(512);
    memset(postdata,0,512);
    //sprintf(postdata, "\n{\n'title': '%s',\n'parents': [{'kind': 'drive#fileLink',\n'id': '%s'}],\n'mimeType': '%s'\n}", name, folderid, filetype);
    sprintf(postdata, "\n{\n'title': '%s',\n'parents': [{'kind': 'drive#fileLink',\n'id': '%s'}]\n}", name, folderid);
    long long int bodydata_size=strlen(postdata);
    DEBUG("!!bodydata_size=%d\n",bodydata_size);
    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files");

   // free(phref_tmp);
    char *header;
    header=(char *)malloc(sizeof(char)*(128));
    memset(header,0,128);
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);

    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    headerlist=curl_slist_append(headerlist, "Expect: " );
    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        DEBUG("@@@@@api_insert,postdata = %s\n", postdata);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE_LARGE,bodydata_size);
        curl_easy_setopt(curl,CURLOPT_POST,1L);

        fp=fopen(Con(TMP_R,data_insert.txt),"w");
        hd=fopen(Con(TMP_R,data_check_access_token.txt),"w+");
       //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("api_insert,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_insert,res = %d\n",res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res!=0){
            fclose(hd);
            free(header);
            free(myUrl);
            free(postdata);
            //DEBUG("api_insert %s [%d] failed!\n",phref,res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                fclose(hd);
                free(header);
                free(myUrl);
                free(postdata);
                return -1;
            }
            fclose(hd);

        }
    }
    free(myUrl);
    free(header);
    free(postdata);
    cJSON *json;
    json=dofile(Con(TMP_R,data_insert.txt));
    if(json)
    {
        cJSON_printf_insert(json, newid);
        cJSON_Delete(json);
        return 0;
    }
    else
    {
        /*the file contents is error*/
        return -1;
    }

}
int api_accout_info()
{
    DEBUG("***api_accout_info\n");
    CURL *curl;
    CURLcode res;
    FILE *fp, *hd;
    struct curl_slist *headerlist=NULL;
    char *header1;
    header1=(char *)malloc(128);
    memset(header1,0,128);
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);
    headerlist=curl_slist_append(headerlist, header1);
    headerlist=curl_slist_append(headerlist, "Expect: " );

    curl=curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
          //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,"https://www.googleapis.com/drive/v2/about");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        fp=fopen(Con(TMP_R,data_3.txt),"w");
        hd=fopen(Con(TMP_R,data_account_info.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("api_accout_info,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_accout_info,res=%d\n",res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);
            free(header1);
            DEBUG("get server space failed , id [%d] !\n",res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"401")!=NULL)
            {
                //write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                DEBUG("api_account_info, refresh_token\n");
                api_refresh_token();
                fclose(hd);
                free(header1);
                return -1;
            }
            fclose(hd);
            free(header1);
            return 0;
        }
    }
    free(header1);
    return -1;
}
int api_metadata_one(char *parentref, char *phref,cJSON *(*cmd_data)(char *filename), char *URL,int index)
{
    DEBUG("***api_metadata_one***\n");
    //DEBUG("URL = %s\n", URL);
    DEBUG("1586 line\n");
    char *folder_id = NULL;
    if(URL != NULL)
    {
        //DEBUG("URL = %s\n", URL);
        folder_id = URL;
    }

    else
    {
        DEBUG("1596line\n");
        folder_id = path_to_parents_id(phref, index);
        DEBUG("1598 line\n");
        DEBUG("folder_id = %s\n", folder_id);
    }
    DEBUG("1601line\n");
    if(!folder_id)
    {
        DEBUG("parent folder is not exist\n");
        return -2;
    }
    DEBUG("1607line\n");
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    char *header1;
    myUrl=(char *)malloc(256);
    memset(myUrl,0,256);
    header1=(char *)malloc(128);
    memset(header1,0,128);
    DEBUG("1614\n");
    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s/children", folder_id);
    DEBUG("1616\n");
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);

    //curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, header1);

        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);

        fp=fopen(Con(TMP_R,data_one.txt),"w");
        hd=fopen(Con(TMP_R,data_check_access_token.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("api_metadata_one,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_metadata_one,res=%d\n", res);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res!=0){
            fclose(hd);
            free(myUrl);
            free(header1);
            DEBUG("api_metadata_one %s [%d] failed!\n",phref,res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                fclose(hd);
                free(header1);
                free(myUrl);
                return -1;
            }
            fclose(hd);

        }
    }
    free(myUrl);
    free(header1);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_one.txt));
    if(json){
        if(cJSON_printf_one(parentref, json) == -1)
        {
            return -1;
            cJSON_Delete(json);
        }
        cJSON_Delete(json);
        return 0;
    }else
        return -1;
}
int api_metadata_test_dir(char *id,proc_pt cmd_data)
{
    DEBUG("!!!api_metadata_test_dir start!\n");
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    myUrl=(char *)malloc(sizeof(char)*(strlen(id)+128));
    memset(myUrl,0,strlen(id)+128);
    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s", id);
    char *header1;
    header1=(char *)malloc(128);
    memset(header1,0,128);
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, header1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //fp=fopen("/tmp/data_5.txt","w");
        fp=fopen(Con(TMP_R,data_test_dir.txt),"w");
        hd=fopen(Con(TMP_R,data_test_dir_header.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("api_metadata_teat_dir,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_metadata_test_dir,res=%d\n", res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);

        if(res!=0){
            fclose(hd);
            free(myUrl);
            free(header1);
            DEBUG("api_metadata_test [%d] failed!\n",res);
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
                if(strstr(tmp,"404") != NULL)
                {
                    free(myUrl);
                    fclose(hd);
                    free(header1);
                    return -1;
                }
                else if(strstr(tmp,"200 OK") != NULL)
                    break;
            }
            fclose(hd);
        }
    }
    free(myUrl);
    free(header1);

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
int api_metadata_test(char *phref)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *myUrl;
    myUrl=(char *)malloc(sizeof(char)*(strlen(phref)+128));
    memset(myUrl,0,strlen(phref)+128);
    sprintf(myUrl,"%s%s%s","https://api.google.com/1/metadata/google",phref,"?list=true&include_deleted=true");
    char *header;
#ifdef OAuth1
    header=makeAuthorize(3);
#else
    header=makeAuthorize(1);
#endif
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    if(curl){
        //curl_easy_setopt(curl,CURLOPT_URL,"https://api.google.com/1/metadata/google/oauth_plaintext_example/main.py?list=true&rev=120e60305d");
        //curl_easy_setopt(curl,CURLOPT_URL,"https://api.google.com/1/metadata/google/main");
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        fp=fopen(Con(TMP_R,data_test.txt),"w");
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            free(myUrl);
            DEBUG("api_metadata_test [%d] failed!\n",res);
            return -1;
        }
    }
    free(myUrl);
    free(header);
    return 0;
}
int get_metadata(char *parentref, char *id, proc_pt cmd_data,CloudFile *FolderTmp, int i)
{
    DEBUG("enter get_metadata,parentref = %s\n", parentref);
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    char *header1;
    DEBUG("1843line\n");
    myUrl=(char *)malloc(sizeof(char)*(128+128));
    memset(myUrl,0,256);
    header1=(char *)malloc(128);
    memset(header1,0,128);

    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s", id);
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);
    DEBUG("myUrl = %s\nheader1=%s\n",myUrl, header1);

    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, header1);

        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //fp=fopen("/tmp/data_5.txt","w");
        fp=fopen(Con(TMP_R,data_6.txt),"w");
        hd=fopen(Con(TMP_R,data_get_metadata.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("get_metadata,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("get_metadata,res=%d\n", res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res!=0){
            fclose(hd);
            free(header1);
            free(myUrl);
            DEBUG("!!!get_metadata %s [%d] failed!\n",id,res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("tmp:%s\n",tmp);
             fclose(hd);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                free(header1);
                free(myUrl);
                return -1;
            }

            else if(strstr(tmp,"404")!=NULL)
            {
                DEBUG("!!!get_metadata:404, return -1\n");
                free(myUrl);
                free(header1);
                return -1;
            }
        }
    }
    free(myUrl);
    free(header1);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_6.txt));
    if(json)
    {
        cJSON *item_title,*item_mimeType,*item_modifiedDate,*item_parents, *item_filesize;
        cJSON *parents_child,*parents_id,  *exportLinks_child, *exportLinks_document, *labels_child, *labels_trash;

            labels_child = cJSON_GetObjectItem( json , "labels");
            if(labels_child == NULL){
                printf("fail to get labels");
                cJSON_Delete(json);
                return -1;
            }
            labels_trash = cJSON_GetObjectItem( labels_child , "trashed");
            if(labels_trash == NULL){
                printf("fail to get labels_trash\n");
            }
            if( labels_trash->type == cJSON_True ){
                FolderTmp->isTrash=1;
                //DEBUG("FolderTmp->isTrash=%d\n", FolderTmp->isTrash);
                cJSON_Delete(json);
                return 0;
            }
            //DEBUG("!FolderTmp->isTrash=%d\n", FolderTmp->isTrash);

            item_title = cJSON_GetObjectItem( json , "title");
            if(item_title == NULL){
                printf("fail to get item_title\n");
                //continue;
            }
            if( item_title->type == cJSON_String ){
                FolderTmp->name=(char *)malloc(sizeof(char)*(strlen(item_title->valuestring)+1));
                //memset(FolderTmp->name, 0, sizeof(FolderTmp->name));
                strcpy(FolderTmp->name,item_title->valuestring);
                //printf("FolderTmp%d->name=%s\n", i, FolderTmp->name);
            }

            item_mimeType = cJSON_GetObjectItem( json , "mimeType");
            if(item_mimeType == NULL){
                printf("fail to get item_mimeType\n");
                //continue;
            }
            if( item_mimeType->type == cJSON_String ){
                FolderTmp->mimeType=(char *)malloc(sizeof(char)*(strlen(item_mimeType->valuestring)+1));
                FolderTmp->href=(char *)malloc(sizeof(char)*(strlen(item_title->valuestring)+1+strlen(parentref)+1));
                strcpy(FolderTmp->mimeType,item_mimeType->valuestring);
                //printf("FolderTmp%d->mimeType=%s\r\n",i, FolderTmp->mimeType);
            }
            if((!strcmp(FolderTmp->mimeType, "application/vnd.google-apps.folder")) || (!strcmp(FolderTmp->mimeType, "application/vnd.google-apps.document")) || (!strcmp(FolderTmp->mimeType, "application/vnd.google-apps.presentation"))||(!strcmp(FolderTmp->mimeType, "application/vnd.google-apps.spreadsheet"))) ;
            else
            {
                //DEBUG("not folder\n");
                item_filesize = cJSON_GetObjectItem( json , "fileSize");
                if(item_filesize == NULL){
                    printf("fail to get fileSize");
                    //FolderTmp->size = 0;
                }
                if( item_filesize->type == cJSON_String ){
                    char tmpfilesize[64] = {0};
                    strcpy(tmpfilesize,item_filesize->valuestring);
                    FolderTmp->size = atoll(tmpfilesize);
                    //printf("FolderTmp%d->size=%lld\n", i, FolderTmp->size);
                }
            }

            item_modifiedDate = cJSON_GetObjectItem( json, "modifiedDate");
            if(item_modifiedDate == NULL){
                printf("fail to get item_id\n");
                //continue;
            }
            if( item_modifiedDate->type == cJSON_String ){
                strcpy(FolderTmp->tmptime, item_modifiedDate->valuestring);
                FolderTmp->mtime=ne_rfc1123_parse2(FolderTmp->tmptime);
                //printf("FolderTmp%d->tmptime=%s\r\n", i, FolderTmp->tmptime);
                //printf("FolderTmp%d->mtime=%lu\r\n", i, FolderTmp->mtime);
            }
            //get parent id
            item_parents = cJSON_GetObjectItem( json , "parents");
            if(item_parents == NULL){
                printf("fail to get item_parents\n");
            }
            parents_child=cJSON_GetArrayItem(item_parents,0);
            if(parents_child == NULL){
                printf("fail to get parents_child\n");
            }
            parents_id = cJSON_GetObjectItem( parents_child , "id");
            if(parents_child == NULL){
                printf("fail to get parents_id\n");
            }
            if( parents_id->type == cJSON_String ){
                FolderTmp->parents_id=(char *)malloc(sizeof(char)*(strlen(parents_id->valuestring)+1));
                strcpy(FolderTmp->parents_id,parents_id->valuestring);
                //printf("FolderTmp%d->parents_id=%s\r\n",i, FolderTmp->parents_id);
            }

            parents_id = cJSON_GetObjectItem( parents_child , "isRoot");
            if(parents_child == NULL){
                printf("fail to get parents_id\n");
            }
            if( parents_id->type == cJSON_True ){
                FolderTmp->isRoot=1;
                //printf("FolderTmp%d->isRoot=%d\r\n",i, FolderTmp->isRoot);
            }

            if(FolderTmp->isRoot==1)
                {
                sprintf(FolderTmp->href, "/%s", FolderTmp->name);
                printf("!!!!FolderTmp%d->href=%s\n", i, FolderTmp->href);
                }
            else
                {
               sprintf(FolderTmp->href, "%s/%s", parentref, FolderTmp->name);
                printf("FolderTmp%d->href=%s\n", i, FolderTmp->href);
            }
            //DEBUG("cJSON_GetObjectItem( json , exportLinks)\n");
            exportLinks_child = cJSON_GetObjectItem( json , "exportLinks");
            if(exportLinks_child == NULL){
                //printf("fail to get exportLinks_child\n");
                FolderTmp->exportLinks_document = NULL;
            }
            else
            {
                //DEBUG("exportLinks_child\n");
                exportLinks_document = cJSON_GetObjectItem( exportLinks_child , "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
                if(exportLinks_document == NULL)
                {
                    printf("fail to get exportLinks_document\n");
                    exportLinks_document = cJSON_GetObjectItem( exportLinks_child , "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
                    if(exportLinks_document == NULL)
                    {
                        printf("fail to get exportLinks_document1\n");
                        exportLinks_document = cJSON_GetObjectItem( exportLinks_child , "application/vnd.openxmlformats-officedocument.presentationml.presentation");
                        if(exportLinks_document == NULL)
                        {
                            printf("fail to get exportLinks_document2\n");
                            FolderTmp->exportLinks_document = NULL;
                            //printf("FolderTmp%d->exportLinks_document=%s\r\n",i, FolderTmp->exportLinks_document);
                            cJSON_Delete(json);
                            return 0;
                        }
                    }
                }
                if( exportLinks_document->type == cJSON_String ){
                    FolderTmp->exportLinks_document=(char *)malloc(sizeof(char)*(strlen(exportLinks_document->valuestring)+1));
                    strcpy(FolderTmp->exportLinks_document,exportLinks_document->valuestring);
                    printf("FolderTmp%d->exportLinks_document=%s\r\n",i, FolderTmp->exportLinks_document);
                }
            }
        //DEBUG("it is not google file\n");
       // cJSON_printf(json,"items");
        cJSON_Delete(json);
        return 0;
    }
    else
    {
        /*the file contents is error*/
        return -1;
    }
}

int main_batch()
{
    DEBUG("enter main_batch");

    CURLcode res;
    CURL *curl;
    struct curl_slist *headerlist=NULL;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    char *header1;
    DEBUG("1843line\n");
    myUrl=(char *)malloc(sizeof(char)*(128+128));
    memset(myUrl,0,256);
    header1=(char *)malloc(128);
    memset(header1,0,128);

    sprintf(myUrl,"/batch");
    headerlist=curl_slist_append(headerlist, "Host: www.googleapis.com" );
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);
    DEBUG("myUrl = %s\nheader1=%s\n",myUrl, header1);
    curl=curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, "Content-Type: multipart/mixed; boundary=END_OF_PART" );
        headerlist=curl_slist_append(headerlist, header1);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        res=curl_easy_perform(curl);
        //---------------------------------------------------------------------------------
        headerlist=curl_slist_append(headerlist, "--END_OF_PART" );
        headerlist=curl_slist_append(headerlist, "Content-Type: application/http" );
        headerlist=curl_slist_append(headerlist, "Content-ID: 1" );
        headerlist=curl_slist_append(headerlist, "Host: www.googleapis.com" );
        curl_easy_setopt(curl,CURLOPT_URL,"/drive/v2/files/0B4cKfgzt5ltER0NuZmpUaDV5ZW8");

        res=curl_easy_perform(curl);
        //---------------------------------------------------------------------------------
        headerlist=curl_slist_append(headerlist, "--END_OF_PART" );
        headerlist=curl_slist_append(headerlist, "Content-Type: application/http" );
        headerlist=curl_slist_append(headerlist, "Content-ID: 2" );
        headerlist=curl_slist_append(headerlist, "Host: www.googleapis.com" );
        curl_easy_setopt(curl,CURLOPT_URL,"/drive/v2/files/0B4cKfgzt5ltEMjFWSm52bUhlbnc");
        res=curl_easy_perform(curl);
        //---------------------------------------------------------------------------------
        headerlist=curl_slist_append(headerlist, "--END_OF_PART" );
        headerlist=curl_slist_append(headerlist, "Content-Type: application/http" );
        headerlist=curl_slist_append(headerlist, "Content-ID: 3" );
        headerlist=curl_slist_append(headerlist, "Host: www.googleapis.com" );
        curl_easy_setopt(curl,CURLOPT_URL,"/drive/v2/files/0B4cKfgzt5ltELU56YkZ0bW94UWc");
//---------------------------------------------------------------------------------
        headerlist=curl_slist_append(headerlist, "--END_OF_PART" );
        fp=fopen(Con(TMP_R,data_6.txt),"w");
        hd=fopen(Con(TMP_R,data_get_metadata.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("main_batch,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("main_batch,res=%d\n", res);
        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(hd);
        curl_slist_free_all(headerlist);
    }
    free(myUrl);
    free(header1);


}

int part_batch(char *parentref, char *id, proc_pt cmd_data,CloudFile *FolderTmp, int i,CURL *curl,struct curl_slist *headerlist)
{
    DEBUG("enter part_batch,parentref = %s\n", parentref);

    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    char *header1;
    DEBUG("1843line\n");
    myUrl=(char *)malloc(sizeof(char)*(128+128));
    memset(myUrl,0,256);
    header1=(char *)malloc(128);
    memset(header1,0,128);
    char header[32] = {0};
    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s", id);
    sprintf(header,"content-id: %d", i);
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);
    DEBUG("myUrl = %s\nheader1=%s\n",myUrl, header1);

    //struct curl_slist *headerlist=NULL;
    //curl=curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "--END_OF_PART" );
        headerlist=curl_slist_append(headerlist, header );
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, "Content-Type: application/json; charset=UTF-8" );
        headerlist=curl_slist_append(headerlist, header1);

        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //fp=fopen("/tmp/data_5.txt","w");
        fp=fopen(Con(TMP_R,data_6.txt),"w");
        hd=fopen(Con(TMP_R,data_get_metadata.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("part_batch,before curl_easy_perform\n");
        //res=curl_easy_perform(curl);
        DEBUG("part_batch,res=%d\n", res);
        //curl_easy_cleanup(curl);
        fclose(fp);
        //curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);
            free(header1);
            free(myUrl);
            DEBUG("!!!part_batch %s [%d] failed!\n",id,res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("part_batch,tmp:%s\n",tmp);
             fclose(hd);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                free(header1);
                free(myUrl);
                return -1;
            }

            else if(strstr(tmp,"404")!=NULL)
            {
                DEBUG("!!!part_batch:404, return -1\n");
                free(myUrl);
                free(header1);
                return -1;
            }
        }
    }
    free(myUrl);
    free(header1);
}

int batch_perform(CURL *curl,struct curl_slist *headerlist)
{
    DEBUG("batch_perform start\n");
    CURLcode res;
    DEBUG("batch_perform,before curl_easy_perform\n");
    res=curl_easy_perform(curl);
    DEBUG("batch_perform,res=%d\n", res);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headerlist);
}

int batch_metadata(char * parentref, char *phref,proc_pt cmd_data, int index)
{
    DEBUG("*************enter batch_metadata*************\n");
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    char *header1;
    header1=(char *)malloc(sizeof(char)*(128));
    myUrl=(char *)malloc(sizeof(char)*(256));
    memset(myUrl,0,256);
    memset(header1,0,128);
    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s/children", phref);
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);
    //curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, header1);

        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //fp=fopen("/tmp/data_5.txt","w");
        fp=fopen(Con(TMP_R,data_5.txt),"w");
        hd=fopen(Con(TMP_R,data_check_access_token.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("batch_metadata,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("res=%d\n", res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res!=0){
            fclose(hd);
            free(header1);
            free(myUrl);
            DEBUG("batch_metadata %s [%d] failed!\n",phref,res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("batch_metadata,tmp=%s\n",tmp);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                fclose(hd);
                free(header1);
                free(myUrl);
                return -1;
            }
            if(strstr(tmp,"404")!=NULL)
            {
                DEBUG("batch_metadata:404 not found\n");
                fclose(hd);
                free(header1);
                free(myUrl);
                return -1;
            }
            fclose(hd);

        }
    }
    free(myUrl);
    free(header1);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_5.txt));
    //DEBUG("now5!\n");
    if(json)
    {
           if(cJSON_batch_printf(parentref, json,"items", phref, index) == -1)
           {
                cJSON_Delete(json);
                return -1;
           }
       // cJSON_printf(json,"items");
        cJSON_Delete(json);
        return 0;
    }
    else
    {
        /*the file contents is error*/
        return -1;
    }
}

int api_metadata(char * parentref, char *phref,proc_pt cmd_data, int index)
{
    DEBUG("*************enter api_metadata*************\n");
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *myUrl;
    char *header1;
    header1=(char *)malloc(sizeof(char)*(128));
    myUrl=(char *)malloc(sizeof(char)*(256));
    memset(myUrl,0,256);
    memset(header1,0,128);
    //DEBUG("get %s metadata\n",phref);
    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s/children", phref);
    DEBUG("@@api_metadata,myUrl = %s\n",myUrl);
    sprintf(header1, "Authorization: Bearer %s", auth->oauth_token);
    DEBUG("2059 line\n");
    //sprintf(myUrl,"%s","https://sp.yostore.net/member/requestservicegateway/");


    //curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, header1);

        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //fp=fopen("/tmp/data_5.txt","w");
        fp=fopen(Con(TMP_R,data_5.txt),"w");
        hd=fopen(Con(TMP_R,data_check_access_token.txt),"w+");
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("api_metadata,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("res=%d\n", res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res!=0){
            fclose(hd);
            free(header1);
            free(myUrl);
            DEBUG("api_metadata %s [%d] failed!\n",phref,res);
            return -1;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("api_metadata,tmp=%s\n",tmp);
            if(strstr(tmp,"401")!=NULL)
            {
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                exit_loop = 1;
                access_token_expired = 1;
                fclose(hd);
                free(header1);
                free(myUrl);
                return -1;
            }
            if(strstr(tmp,"404")!=NULL)
            {
                DEBUG("api_metadata:404 not found\n");
                fclose(hd);
                free(header1);
                free(myUrl);
                return -1;
            }
            fclose(hd);

        }
    }
    free(myUrl);
    free(header1);
    cJSON *json;
    json=cmd_data(Con(TMP_R,data_5.txt));
    //DEBUG("now5!\n");
    if(json)
    {
           if(cJSON_printf2(parentref, json,"items", phref, index) == -1)
           {
                cJSON_Delete(json);
                return -1;
           }
       // cJSON_printf(json,"items");
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
int g_move(char *oldname,char *newname,int index,int is_changed_time,char *newname_r)
{
    DEBUG("**************api_move!*******************\n");
    char id[64] = "\0";
    char parents_id[64] = "\0";
    char *id_tmp = NULL;
    char *parents_id_tmp = NULL;
    id_tmp = path_to_id(oldname, index);
    if(id_tmp != NULL)
    {
        //DEBUG("id_tmp != NULL\n");
        strcpy(id, id_tmp);
    }
    parents_id_tmp = path_to_parents_id(newname, index);
    if(parents_id_tmp != NULL)
        strcpy(parents_id, parents_id_tmp);
    //DEBUG("id = %s, parents_id = %s\n", id, parents_id);

    /*if (strcmp(id, "") == 0)
    {
        DEBUG("create and rename immediataly\n");
        char filetype[32] = {0};
        get_filetype(newname, filetype);
        api_insert(parents_id, newname, id, filetype);
    }*/
    //DEBUG("!id = %s, parents_id = %s\n", id, parents_id);
    char *newname1 = NULL;
    newname1 = strrchr(newname,'/');
    newname1++;
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *header;
    header = (char *)malloc(128);
    memset(header, '\0', sizeof(header));
    char *data;
    hd=fopen(Con(TMP_R,data_hd.txt),"w+");
    char *oldname_tmp=oauth_url_escape(oldname);
    DEBUG("@@@@@@@@@@@@@@@@%s-->%s\n", oldname, oldname_tmp);
    char *newname_tmp=oauth_url_escape(newname);
    DEBUG("@@@@@@@@@@@@@@@@%s-->%s\n", newname, newname_tmp);
    data=(char*)malloc(512);
    memset(data, '\0', sizeof(data));
    char myurl[256] = {0};
    //DEBUG("before set myurl, id=%s\n", id);
    sprintf(myurl, "https://www.googleapis.com/drive/v2/files/%s", id);
    //DEBUG("myurl=%s\n",myurl);
    sprintf(data,"\n{\n'title': '%s',\n'parents': [{\n'id': '%s'\n}]\n}", newname1, parents_id);
     long long int bodydata_size=strlen(data);
    DEBUG("data = %s\n", data);
    free(oldname_tmp);
    free(newname_tmp);

    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist,"Content-Type: application/json");
    headerlist=curl_slist_append(headerlist,"Expect: " );
    if(curl){
        curl_easy_setopt(curl,CURLOPT_NOBODY,1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //DEBUG("before set myurl!\n");
        curl_easy_setopt(curl,CURLOPT_URL, myurl);
        //DEBUG("after set myurl!\n");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE_LARGE,bodydata_size);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl,CURLOPT_POST,1L);

        fp=fopen(Con(TMP_R,data_4.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("g_move,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("g_move, res = %d\n", res);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headerlist);
        fclose(fp);
        if(res!=0){

            fclose(hd);
            free(header);
            free(data);
            DEBUG("rename %s failed,id [%d]!\n",oldname,res);
            return res;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"404")!=NULL)
            {
                if(newname_r == NULL)  //newname_r is local new name
                {
                    char *localpath=serverpath_to_localpath(newname,index);
                    if(test_if_dir(localpath))
                    {

                        DEBUG("it is folder\n");

                        res=dragfolder(localpath,index);
                        if(res != 0)
                        {

                            DEBUG("dragfolder %s failed status = %d\n",localpath,res);
                            //write_system_log("error","uploadfile fail");

                            fclose(hd);
                            free(header);
                            free(data);
                            free(localpath);
                            return res;
                        }
                    }
                    else
                    {

                        DEBUG("it is file\n");

                        DEBUG("upload15\n");
                        res=g_upload_file(localpath,newname,1,index);
                        if(res!=0)
                        {

                            fclose(hd);
                            free(header);
                            free(data);
                            free(localpath);
                            return res;
                        }
                        else
                        {
                            cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                            time_t mtime=cJSON_printf_mtime(json);
                            cJSON_Delete(json);

                            ChangeFile_modtime(localpath,mtime);
                        }
                    }
                    free(localpath);
                }
                else
                {
                    //char *localpath=serverpath_to_localpath(newname_r,index);
                    char *localpath = newname_r;
                    if(test_if_dir(localpath))
                    {

                        DEBUG("it is folder\n");

                        res=dragfolder_old_dir(localpath,index,newname);
                        if(res != 0)
                        {

                            DEBUG("dragfolder %s failed status = %d\n",localpath,res);
                            //write_system_log("error","uploadfile fail");

                            fclose(hd);
                            free(header);
                            free(data);
                            //free(localpath);
                            return res;
                        }
                    }
                    else
                    {
                        DEBUG("it is file\n");
                        DEBUG("upload16\n");
                        res=g_upload_file(localpath,newname,1,index);
                        if(res!=0)
                        {
                            fclose(hd);
                            free(header);
                            free(data);
                            //free(localpath);
                            return res;
                        }
                        else
                        {
                            cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                            time_t mtime=cJSON_printf_mtime(json);
                            cJSON_Delete(json);

                            ChangeFile_modtime(localpath,mtime);
                        }
                    }
                    //free(localpath);
                }

            }
            else if(strstr(tmp,"200")!=NULL)
            {
                if(is_changed_time)
                {
                    char *localpath=serverpath_to_localpath(newname,index);
                    cJSON *json = dofile(Con(TMP_R,data_4.txt));
                    time_t mtime=cJSON_printf_mtime(json);
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
            else if(strstr(tmp,"403")!=NULL)
            {
                //cJSON *json = dofile(Con(TMP_R,data_4.txt));
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
                    free(data);
                    return -1;
                }
                //cJSON_Delete(json);
            }
        }
    }

    fclose(hd);
    DEBUG("rename ok\n");
    free(header);
    free(data);
    return 0;
}

api_rename(CloudFile *FolderTmp,char *newname,int index,int is_changed_time,char *newname_r)
{
    DEBUG("api_rename start\n");
    /*if (strcmp(id, "") == 0)
    {
        DEBUG("create and rename immediataly\n");
        char filetype[32] = {0};
        get_filetype(newname, filetype);
        api_insert(parents_id, newname, id, filetype);
    }*/
    char *newname1 = NULL;
    newname1 = strrchr(newname,'/');
    newname1++;
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    char *header;
    header = (char *)malloc(128);
    memset(header, 0, 128);
    char *data;
    hd=fopen(Con(TMP_R,data_hd.txt),"w+");
    data=(char*)malloc(512);
    memset(data, 0, 512);
    char *myurl;
    myurl = (char *)malloc(256);
    memset(myurl, 0, 256);
    sprintf(myurl, "https://www.googleapis.com/drive/v2/files/%s", FolderTmp->id);
    sprintf(data,"\n{\n'title': '%s'\n}", newname1);
     long long int bodydata_size=strlen(data);
    DEBUG("data = %s\n", data);

    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist,"Content-Type: application/json");
    headerlist=curl_slist_append(headerlist,"Expect: " );
    if(curl){
        curl_easy_setopt(curl,CURLOPT_NOBODY,1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //DEBUG("before set myurl!\n");
        curl_easy_setopt(curl,CURLOPT_URL, myurl);
        //DEBUG("after set myurl!\n");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE_LARGE,bodydata_size);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl,CURLOPT_POST,1L);

        fp=fopen(Con(TMP_R,data_4.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("api_rename,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_rename, res = %d\n", res);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headerlist);
        fclose(fp);
        if(res!=0){

            fclose(hd);
            free(header);
            free(data);
            free(myurl);
            DEBUG("rename %s failed,id [%d]!\n",FolderTmp->name,res);
            return res;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            DEBUG("tmp:%s\n",tmp);
            if(strstr(tmp,"404")!=NULL)
            {
                if(newname_r == NULL)  //newname_r is local new name
                {
                    char *localpath=serverpath_to_localpath(newname,index);
                    if(test_if_dir(localpath))
                    {

                        DEBUG("it is folder\n");

                        res=dragfolder(localpath,index);
                        if(res != 0)
                        {

                            DEBUG("dragfolder %s failed status = %d\n",localpath,res);
                            //write_system_log("error","uploadfile fail");

                            fclose(hd);
                            free(header);
                            free(data);
                            free(localpath);
                            free(myurl);
                            return res;
                        }
                    }
                    else
                    {

                        DEBUG("it is file\n");

                        DEBUG("upload17\n");
                        res=g_upload_file(localpath,newname,1,index);
                        if(res!=0)
                        {

                            fclose(hd);
                            free(header);
                            free(data);
                            free(localpath);
                            free(myurl);
                            return res;
                        }
                        else
                        {
                            cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                            time_t mtime=cJSON_printf_mtime(json);
                            cJSON_Delete(json);

                            ChangeFile_modtime(localpath,mtime);
                        }
                    }
                    free(localpath);
                }
                else
                {
                    //char *localpath=serverpath_to_localpath(newname_r,index);
                    char *localpath = newname_r;
                    if(test_if_dir(localpath))
                    {

                        DEBUG("it is folder\n");

                        res=dragfolder_old_dir(localpath,index,newname);
                        if(res != 0)
                        {

                            DEBUG("dragfolder %s failed status = %d\n",localpath,res);
                            //write_system_log("error","uploadfile fail");

                            fclose(hd);
                            free(header);
                            free(data);
                            //free(localpath);
                            free(myurl);
                            return res;
                        }
                    }
                    else
                    {
                        DEBUG("it is file\n");
                        DEBUG("upload18\n");
                        res=g_upload_file(localpath,newname,1,index);
                        if(res!=0)
                        {
                            fclose(hd);
                            free(header);
                            free(data);
                            free(myurl);
                            //free(localpath);
                            return res;
                        }
                        else
                        {
                            cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                            time_t mtime=cJSON_printf_mtime(json);
                            cJSON_Delete(json);

                            ChangeFile_modtime(localpath,mtime);
                        }
                    }
                    //free(localpath);
                }

            }
            else if(strstr(tmp,"200")!=NULL)
            {
                if(is_changed_time)
                {
                    char *localpath=serverpath_to_localpath(newname,index);
                    cJSON *json = dofile(Con(TMP_R,data_4.txt));
                    time_t mtime=cJSON_printf_mtime(json);
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
            else if(strstr(tmp,"403")!=NULL)
            {
                //cJSON *json = dofile(Con(TMP_R,data_4.txt));
                char *server_conflcit_name=get_server_exist(newname,index);
                printf("server_conflict_name=%s\n",server_conflcit_name);
                if(server_conflcit_name)
                {
                    deal_big_low_conflcit(server_conflcit_name,FolderTmp->name,newname,newname_r,index);
                    free(server_conflcit_name);
                }
                else
                {
                    fclose(hd);
                    free(header);
                    free(data);
                    free(myurl);
                    return -1;
                }
                //cJSON_Delete(json);
            }
        }
    }

    fclose(hd);
    DEBUG("rename ok\n");
    free(header);
    free(data);
    free(myurl);
    return 0;
}
/*
fullname=>local
filename=>server
*/
int g_download(CloudFile *filetmp, char *fullname,char *filename,int index)
{

    DEBUG("****************api_download %s******************\n", filename);
    char *namesize=strrchr(filename, '/');
    namesize ++;
    DEBUG("namesize=%s\nstrlen(namesize)=%d\n", namesize, strlen(namesize));
    if(strlen(namesize) > 248)//加上.asus.td超过255
    {
        DEBUG("filename is too long, return!\n");
            return -1;
    }

    //DEBUG("filetmp->mimeType=%s\n",filetmp->mimeType);
    int flag = 0;
    //DEBUG("before if!\n");
    if ((!(strcmp(filetmp->mimeType, "application/vnd.google-apps.document"))) || (!strcmp(filetmp->mimeType, "application/vnd.google-apps.presentation")) || (!strcmp(filetmp->mimeType, "application/vnd.google-apps.spreadsheet")))
        flag = 1;
    //DEBUG("after if!flag=%d\n", flag);
    if(access(fullname,F_OK) == 0)
    {

        DEBUG("Local has %s\n",fullname);

        unlink(fullname);
        add_action_item("remove",fullname,g_pSyncList[index]->server_action_list);
    }

    char *temp_suffix = ".asus.td";
    char *Localfilename_tmp=(char *)malloc(sizeof(char)*(strlen(fullname)+strlen(temp_suffix)+2));
    memset(Localfilename_tmp,0,strlen(fullname)+strlen(temp_suffix)+2);
    sprintf(Localfilename_tmp,"%s%s",fullname,temp_suffix);
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *hd;
    hd=fopen(Con(TMP_R,api_download_header.txt),"w+");
    //FILE *hd;
    char *myUrl;
    //char *filename_tmp=oauth_url_escape(filename);
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    //DEBUG("before set myurl\n");
    if(flag)
    {
        //DEBUG("filetmp->exportLinks_document=%s", filetmp->exportLinks_document);
        sprintf(myUrl,"%s",filetmp->exportLinks_document);
    }
    else
    {
        //DEBUG("filetmp->id=%s", filetmp->id);
        sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s?alt=media",filetmp->id);
    }
    DEBUG("flag=%d,myurl=%s\n", flag,myUrl);
    //free(filename_tmp);
    char *header;
    header = (char *)malloc(128);
    memset(header, 0, 128);
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Expect: " );
    write_log(S_DOWNLOAD,"",fullname,index);
    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL

       // curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,15);
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
        DEBUG("g_download,before CURLOPT_PROGRESSFUNCTION\n");
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,Clientfp);
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);

        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);

        DEBUG("fopen downloadfile, Localfilename_tmp = %s\n", Localfilename_tmp);
        fp=fopen(Localfilename_tmp,"w");
        //fp=fopen("/tmp/google_download.txt","w+");
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("g_download,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("g_download,after curl_easy_perform,res=%d\n",res);

        curl_easy_cleanup(curl);
        fclose(fp);

        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(hd);

            free(header);
            free(myUrl);
            DEBUG("download %s failed,id:[%d]!\n",filename,res);
            unlink(Localfilename_tmp);
            free(Localfilename_tmp);
            char *error_message=write_error_message("download %s failed",filename);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if(res == 7)
            {
                //write_log(S_ERROR,"Could not connect to server!","",index);
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
            DEBUG("tmp:%s\n",tmp_name);
            fclose(hd);
            if(strstr(tmp_name,"401")!=NULL)
            {
                unlink(Localfilename_tmp);
                free(Localfilename_tmp);
                write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                api_refresh_token();
                DEBUG("api_download,,,,,\n");
                //fclose(hd);
                DEBUG("2806\n");
                free(header);
                DEBUG("2808\n");
                free(myUrl);
                DEBUG("api_download???\n");
                return -1;
            }
            if(strstr(tmp_name,"404")!=NULL)
            {
                unlink(Localfilename_tmp);
            }
            else
            {
                if(rename(Localfilename_tmp,fullname)!=0)
                {
                    unlink(Localfilename_tmp);
                    free(Localfilename_tmp);
                    DEBUG("2823\n");
                    free(header);
                    DEBUG("2826\n");
                    free(myUrl);
                    return -1;
                }
            }
        }
    }
    free(Localfilename_tmp);
    free(header);
    free(myUrl);
    /*if(finished_initial)
        write_log(S_SYNC,"","",index);
    else
        write_log(S_INITIAL,"","",index);*/
    return 0;
}

//int get_file_size(char *filename)
//{
//    int file_len = 0;
//    int fd = 0;

//    fd = open(filename, O_RDONLY);
//    if(fd < 0)
//    {
//        perror("open");
//        /*
//         fix below bug:
//            1.create file a;
//            2.rename a-->b;
//            final open() 'a' failed ,will exit();
//        */
//        //exit(-1);
//        return -1;
//    }

//    file_len = lseek(fd, 0, SEEK_END);
//    //DEBUG("file_len is %d\n",file_len);
//    if(file_len < 0)
//    {
//        perror("lseek");
//        exit(-1);
//    }
//    close(fd);
//    return file_len;
//}

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

int api_upload_put( char *filename,char *serverpath,int flag,int index)
{
    DEBUG("api_upload_put,serverpath = %s\nfilename = %s\nflag = %d\n", serverpath, filename, flag);
    char *file_parents_id;
    char *fileid;
    if(flag)
    {
        //DEBUG("before path_to_parents_id\n");
        file_parents_id = path_to_parents_id(serverpath, index);
        //DEBUG("file_parents_id = %s\n", file_parents_id);
    }
    else
    {
        fileid = path_to_id(serverpath, index);
    }

    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *fp_1;
    FILE *fp_hd;
    struct stat filestat;
    unsigned long int filesize;

    if( stat(filename,&filestat) == -1)
    {
        //perror("stat:");
        DEBUG("servr sapce full stat error:%s file not exist\n",filename);
        return -1;
    }

    filesize = filestat.st_size;

    filesize = filesize / 1024 / 1024;

    fp_1=fopen(filename,"rb");
    if(fp_1 == NULL)
    {
        DEBUG("Local has no %s\n",filename);
        return LOCAL_FILE_LOST;
    }
    //DEBUG("!!!filename=%s\n", filename);
    char *name=strrchr(filename, '/');
    name ++;
    char *newid;
    newid=(char *)malloc(64);
    memset(newid,0, 64);
     //get_filetype(name, filetype);
    if(flag)
    {
        //strcpy(filetype, "application/vnd.google-apps.document");
        api_insert(file_parents_id, name, newid);
    }
    //DEBUG("!!!newid = %s\n", newid);
    long long int size=get_file_size(filename);
    //printf("size=%d,filesize=%lu\n",size,filesize);
    char *header;
    header=(char *)malloc(sizeof(char)*(128));
    memset(header,0,128);
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);

    struct curl_slist *headerlist=NULL;
    char header_l[]="Content-Length: ";
    char header1_l[128]="\0";

    sprintf(header1_l,"%s%d\n",header_l,size);

    header1_l[strlen(header1_l)-1]='\0';
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist, "Expect: " );
    char *myUrl;
    myUrl=(char *)malloc(128);
    memset(myUrl,0,128);
    DEBUG("serverpath = %s\n",serverpath);
    if (flag)
   {
        sprintf(myUrl,"https://www.googleapis.com/upload/drive/v2/files/%s",newid);
         printf("myurl=%s\n", myUrl);
    }
    if (!flag)
    {
        sprintf(myUrl,"https://www.googleapis.com/upload/drive/v2/files/%s",fileid);

    }
    //headerlist=curl_slist_append(headerlist,header);
    //headerlist=curl_slist_append(headerlist,buf);
    //headerlist=curl_slist_append(headerlist,buf);

    if(LOCAL_FILE.path != NULL)
        free(LOCAL_FILE.path);
    LOCAL_FILE.path = (char*)malloc(sizeof(char)*(strlen(filename) + 1));
    sprintf(LOCAL_FILE.path,"%s",filename);
    LOCAL_FILE.index = index;



    if(curl){
        fp=fopen(Con(TMP_R,upload_chunk_commit.txt),"w");
        fp_hd=fopen(Con(TMP_R,upload_header.txt),"w+");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        //CURL_DEBUG;
        //curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_READDATA,fp_1);
        curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);
        //curl_easy_setopt(curl,CURLOPT_PUT,1L);
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,15);
            curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10);//upload time_out
        //curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,(curl_off_t)size);//put Content-Length
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)filestat.st_size);//2016.10.19 tina modify for uploadsize is 0 on mipselbig
        //curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,size);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,size);
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);

            curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
            curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,Clientfp);
        curl_easy_setopt(curl,CURLOPT_READFUNCTION, my_read_func);


            curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
           curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);


        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);
        //start_time = time(NULL);
        DEBUG("api_upload_put,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_upload_put,res=%d\n",res);
        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(fp_1);
        if(res!=0){
            char newid1[64] = {0};
            strcpy(newid1, newid);
            fclose(fp_hd);
            free(header);
            free(newid);
            free(myUrl);
            DEBUG("upload %s failed,id1 is [%d]!\n",filename,res);
            char *error_message=write_error_message("upload %s failed",filename);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if (res == 26 || res == 56)
            {
                DEBUG("upload fail, now delete it!\n");
                while(g_delete(serverpath, index, newid1) !=0);
                DEBUG("upload fail,delete complete!\n");
                return -1;
            }

            if( res == 7 )
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filename,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->unfinished_list);
                    }
                    */
                //write_log(S_ERROR,"Could not connect to server!","",index);
                return COULD_NOT_CONNECNT_TO_SERVER;
            }
            else if( res == 28)
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filename,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->unfinished_list);
                    }
                    */
                //write_log(S_ERROR,"Could not connect to server!","",index);
                return CONNECNTION_TIMED_OUT;
            }
            else if( res == 35 )
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filename,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->unfinished_list);
                    }
                    */
                return INVALID_ARGUMENT;
            }
            else
                return res;
        }
        else
        {

            rewind(fp_hd);
            char tmp_[256]="\0";;
            while(!feof(fp_hd))
            {
                fgets(tmp_,sizeof(tmp_),fp_hd);
                if(strstr(tmp_,"507 Quota Error") != NULL)
                {
                    write_log(S_ERROR,"server space is not enough!","",index);
                    action_item *item;
                    //DEBUG("get_action_item1\n");
                    item = get_action_item("upload",filename,g_pSyncList[index]->up_space_not_enough_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->up_space_not_enough_list);
                    }
                    fclose(fp_hd);
                    free(header);
                    free(newid);
                    free(myUrl);
                    return SERVER_SPACE_NOT_ENOUGH;
                }
                else if(strstr(tmp_,"401")!=NULL)
                {
                    write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                    exit_loop = 1;
                    access_token_expired = 1;
                    fclose(fp_hd);
                    //free(header1);
                    free(myUrl);
                    free(header);
                    free(newid);
                    return -1;
                }
                else if(strstr(tmp_,"404")!=NULL)
                {
                    DEBUG("api_upload_put:404 not found\n");
                    //path_to_parents_id();
                    //api_create_folder();
                    //api_upload_put();
                    char localpath1[1024] = {0};
                    char foldername1[1024] = {0};
                    char newfolderid[32] = {0};
                    path_to_ppath(filename, localpath1);
                    path_to_ppath(serverpath, foldername1);
                    if(g_create_folder(localpath1, foldername1,newfolderid, index, 0)==0)
                    {
                        upload_serverlist_(index,localpath1, foldername1,newfolderid);
                    }
                    DEBUG("api_creater_folder end!!@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
                    fclose(fp_hd);
                    free(header);
                    free(newid);
                    free(myUrl);
                    return -1;
                }
            }
            fclose(fp_hd);
        }
    }
    free(header);
    free(newid);
    free(myUrl);
    return 0;
}
int api_upload_post()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    FILE *fp_1;

    struct stat filestat;
    unsigned long int filesize;

    if( stat("cookie_login.txt",&filestat) == -1)
    {
        //perror("stat:");
        DEBUG("servr sapce full stat error:%s file not exist\n","cookie_login.txt");
        return -1;
    }

    filesize = filestat.st_size;

    filesize = filesize / 1024 / 1024;

    fp_1=fopen("cookie_login.txt","rb+");
    long long int size=get_file_size("cookie_login.txt");
    DEBUG("size=%d,filesize=%lu\n",size,filesize);
    char *header;
    static const char buf[]="Content-Type: test/plain";
#ifdef OAuth1
    header=makeAuthorize(4);
#else
    header=makeAuthorize(2);
#endif
    struct curl_slist *headerlist=NULL;
    char header_l[]="Content-Length: ";
    char header1_l[128]="\0";
    //sprintf(header1_l,"%s%lu\n",header_l,filesize);
    sprintf(header1_l,"%s%d\n",header_l,size);
    DEBUG("%d\n",strlen(header1_l));
    //DEBUG("%c\n",header1_l[strlen(header1_l)-2]);
    header1_l[strlen(header1_l)-1]='\0';
    curl=curl_easy_init();
    //headerlist=curl_slist_append(headerlist,header1_l);
    char myUrl[256]="\0";
    sprintf(myUrl,"%s?%s","https://api-content.google.com/1/files/google/oauth_plaintext_example",header);
    //headerlist=curl_slist_append(headerlist,buf);

    //headerlist=curl_slist_append(headerlist,buf);
    if(curl){
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        //curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
        //curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_READDATA,fp_1);
        //curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);//put
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,15);
        //curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,(curl_off_t)size);//put Content-Length
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,fp_1);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE_LARGE,(curl_off_t)size);
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        fp=fopen("upload.txt","w");
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        res=curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(fp_1);
        if(res!=0){
            DEBUG("delete [%d] failed!\n",res);
            return -1;
        }
    }
    free(header);
}

int my_progress_func(char *clientfp,double t,double d,double ultotal,double ulnow){
    DEBUG("my_progress_func\n");
    printf("%g/%g(%g%%)\n",d,t,d*100.0/t);
#if 1
    int sec;
    double  elapsed = 0;
    elapsed = time(NULL) - start_time;
    sec = (int)elapsed;
    if( sec > 0 )
    {
        //double progress = ulnow*100.0/ultoal;
        if(sec % 5 == 0)
            DEBUG("\r@%s %g / %g (%g %%)", clientfp, ulnow, ultotal, ulnow*100.0/ultotal);
    }
#endif

#if 1
    if(t > 1 && d > 10) // download
        DEBUG("\r@@%s %10.0f / %10.0f (%g %%)", clientfp, d, t, d*100.0/t);
    else//upload
    {
        if(exit_loop==1){
            return -1;
        }
        DEBUG("\r@@@%s %10.0f / %10.0f (%g %%)\n", clientfp, ulnow, ultotal, ulnow*100.0/ultotal);
    }
#endif
    return 0;
}

size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)

{
    DEBUG("***my_write_func \n");
    if(exit_loop==1){
        return -1;
    }
    DEBUG("***download***\n");
    int len ;
    len = fwrite(ptr, size, nmemb, stream);
    //printf("write len is %d\n",len);
    return len;

}

size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    DEBUG("my_read_func\n");
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
            sprintf(LOCAL_FILE.path,"%s",ret);
            free(ret);
        }
    }

    int len;
    len = fread(ptr, size, nmemb, stream);
    //DEBUG("\rread len:%d   path:%s",len,LOCAL_FILE.path);
    return len;
}

int api_upload_chunk_put(char *buffer,char *upload_id,unsigned long offset,unsigned long chunk,int index,char *filepath)
{
    DEBUG("api_upload_chunk_put,upload_id: %s\n",upload_id);
    FILE *fp;
    FILE *fp_2;
    fp_2=fopen(Con(TMP_R,swap),"w+");
    char *header;
    header=(char *)malloc(sizeof(char)*(128));
    memset(header,0,128);
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    fwrite(buffer,4000000/10,10,fp_2);
    rewind(fp_2);
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist=NULL;

    curl=curl_easy_init();
    //char myUrl[2046]="\0";
    char *myUrl=(char *)malloc(2046);
    memset(myUrl,0,2046);
    //char range[256] = {0};
    /*if (flag)
   {
        sprintf(myUrl,"https://www.googleapis.com/upload/drive/v2/files/%s",newid);
         printf("myurl=%s\n", myUrl);
    }
    if (!flag)
    {
        sprintf(myUrl,"https://www.googleapis.com/upload/drive/v2/files/%s",fileid);

    }*/
    if( upload_id ==NULL )
        sprintf(myUrl,"%s?%s","https://api-content.google.com/1/chunked_upload",header);
    else
    {
        sprintf(myUrl,"%s?upload_id=%s&offset=%lu&%s","https://api-content.google.com/1/chunked_upload",upload_id,chunk,header);
    }
    DEBUG("myUrl: %s\n",myUrl);
    headerlist=curl_slist_append(headerlist,header);

    if(LOCAL_FILE.path != NULL)
        free(LOCAL_FILE.path);
    LOCAL_FILE.path = (char*)malloc(sizeof(char)*(strlen(filepath) + 1));
    sprintf(LOCAL_FILE.path,"%s",filepath);
    LOCAL_FILE.index = index;

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_READDATA,fp_2);
        curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);
        curl_easy_setopt(curl,CURLOPT_PUT,1L);
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,10);//download time_out
        curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10);//upload time_out
        curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,(curl_off_t)offset);//put Content-Length
        //if( offset > 0)
        //sprintf(range,"0-%lu",offset);
        //curl_easy_setopt(curl, CURLOPT_RANGE,range);
        //curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE,(curl_off_t)0);
        fp=fopen(Con(TMP_R,upload_chunk_1.txt),"w+");
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,Clientfp);

        curl_easy_setopt(curl,CURLOPT_READFUNCTION, my_read_func);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);
        //curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);


        res=curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        fclose(fp_2);
        if(res!=CURLE_OK){
            free(header);
            free(myUrl);
            DEBUG("upload fail,id is %d!\n",res);
            char *error_message=write_error_message("upload %s failed",filepath);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if( res == 7 )
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filepath,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filepath,g_pSyncList[index]->unfinished_list);
                    }
                    */
                return COULD_NOT_CONNECNT_TO_SERVER;
            }
            else if( res == 28)
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filepath,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filepath,g_pSyncList[index]->unfinished_list);
                    }
                    */
                return CONNECNTION_TIMED_OUT;
            }
            else if( res == 35 )
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filepath,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filepath,g_pSyncList[index]->unfinished_list);
                    }*/
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
        DEBUG("open %s fialed\n","upload_chunk_1.txt");
        return NULL;
    }
    char buff[1024]="\0";
    char *p=NULL,*m=NULL;
    char *upload_id;
    upload_id=(char *)malloc(sizeof(char *)*512);
    memset(upload_id,0,512);
    fgets(buff,sizeof(buff),fp);
    p=strstr(buff,"upload_id");
    p=p+strlen("upload_id")+4;
    m=strchr(p,'"');
    snprintf(upload_id,strlen(p)-strlen(m)+1,"%s",p);
    //strncpy(upload_id,p,strlen(p)-strlen(m));
    DEBUG("upload_id : %s,%d\n",upload_id,strlen(upload_id));
    //DEBUG("upload_id : %c\n",upload_id[21]);
    if(upload_id[strlen(upload_id)] == '\0')
    {
        //DEBUG("111\n");
        upload_id[strlen(upload_id)] = '\0';
    }
    fclose(fp);
    //printf("upload_id : %s,%d\n",upload_id,strlen(upload_id));
    //printf("upload_id : %c\n",upload_id[strlen(upload_id)]);
    return upload_id;
}
int api_upload_chunk_commit(char *upload_id,char *filename,int flag,int index)
{
    FILE *fp;
    FILE *fp_hd;
    char *header;
#ifdef OAuth1
    header=makeAuthorize(4);
#else
    header=makeAuthorize(2);
#endif
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist=NULL;

    curl=curl_easy_init();
    char *data=malloc(256);
    memset(data,0,256);
    if(flag)
        sprintf(data,"upload_id=%s",upload_id);
    else
        sprintf(data,"overwrite=false&upload_id=%s",upload_id);
    char myUrl[2046]="\0";
    sprintf(myUrl,"%s%s?upload_id=%s&%s","https://api-content.google.com/1/commit_chunked_upload/google",filename,upload_id,header);
    //headerlist=curl_slist_append(headerlist,header);

    if(curl){
        curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        //curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_READDATA,fp_2);
        //curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,15);
        //curl_easy_setopt(curl,CURLOPT_INFILESIZE_LARGE,(curl_off_t)offset);//put Content-Length
        //if( offset > 0)
        //sprintf(range,"0-%lu",offset);
        //curl_easy_setopt(curl, CURLOPT_RANGE,range);
        //curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE,(curl_off_t)0);
        fp=fopen(Con(TMP_R,upload_chunk_commit.txt),"w");
        //curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

        fp_hd=fopen(Con(TMP_R,upload_header.txt),"w+");
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);

        res=curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        if(res!=0){
            fclose(fp_hd);
            free(header);
            free(data);
            DEBUG("upload %s failed,id2 is [%d]!\n",filename,res);
            char *error_message=write_error_message("upload %s failed",filename);
            write_log(S_ERROR,error_message,"",index);
            free(error_message);
            if( res == 7 )
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filename,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->unfinished_list);
                    }*/

                return COULD_NOT_CONNECNT_TO_SERVER;
            }
            else if( res == 28)
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filename,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->unfinished_list);
                    }
                    */

                return CONNECNTION_TIMED_OUT;
            }
            else if( res == 35 )
            {
                /*
                    action_item *item;
                    item = get_action_item("upload",filename,g_pSyncList[index]->unfinished_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->unfinished_list);
                    }
                    */
                return INVALID_ARGUMENT;
            }
            else
                return res;
        }
        else
        {
            free(header);
            free(data);
            rewind(fp_hd);
            char tmp_[256];
            while(!feof(fp_hd))
            {
                fgets(tmp_,sizeof(tmp_),fp_hd);
                if(strstr(tmp_,"507 Quota Error") != NULL)
                {
                    write_log(S_ERROR,"server space is not enough!","",index);
                    action_item *item;
                    DEBUG("get_action_item2\n");
                    item = get_action_item("upload",filename,g_pSyncList[index]->up_space_not_enough_list,index);
                    if(item == NULL)
                    {
                        add_action_item("upload",filename,g_pSyncList[index]->up_space_not_enough_list);
                    }
                    fclose(fp_hd);
                    return SERVER_SPACE_NOT_ENOUGH;
                }
            }
            fclose(fp_hd);
        }
    }
    return 0;
}
/*
 1=>enough
 0=>not enough
 status=>get accout_info fail
*/
int is_server_enough(char *filename) //1:enough
{
    DEBUG("***is_server_enough\n");
    long long int size_lo=get_file_size(filename);
    if(size_lo == -1)
        return -1;
    int status = 0;
    status=api_accout_info();
    //DEBUG("status=%d\n", status);
    long long int size_server;
    if(status == 0)
    {
        cJSON *json = dofile(Con(TMP_R,data_3.txt));
        cJSON_printf_account_info(json);
        cJSON_Delete(json);
        //printf("%lld %lld %lld\n",server_quota,server_shared,server_normal);
        size_server=server_quota-server_shared;
        size_server-=server_normal;
        //size_server=(server_quota-server_shared-server_normal);
        DEBUG("server free space is %lld\n",size_server);

        if(size_server> size_lo)
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

int add_upload_chunked_info(char *filename,char *upload_id,unsigned long offset,unsigned long chunk,time_t expires)
{
    upload_chunk = (Upload_chunked *)malloc(sizeof(Upload_chunked));
    memset(upload_chunk,0,sizeof(upload_chunk));
    upload_chunk->filename = (char *)malloc(sizeof(char)*(strlen(filename)+1));
    memset(upload_chunk->filename,'\0',strlen(filename)+1);
    sprintf(upload_chunk->filename,"%s",filename);
    upload_chunk->upload_id = (char *)malloc(sizeof(char)*(strlen(upload_id)+1));
    memset(upload_chunk->upload_id,'\0',strlen(upload_id)+1);
    sprintf(upload_chunk->upload_id,"%s",upload_id);
    upload_chunk->chunk = chunk;
    upload_chunk->offset = offset;
    upload_chunk->expires = expires;
    return 1;
}

int api_upload_chunk_continue(char *filename,int index,int flag,char *serverpath,unsigned long size)
{
    DEBUG("api_upload_chunk_continue\n");
    int res=0;
    unsigned long offset=4*1000*1000;//4M chunk
    unsigned long chunk = 0;
    FILE *fp_1;
    char *upload_id = (char *)malloc(sizeof(char)*(strlen(upload_chunk->upload_id)+1));
    time_t expires = upload_chunk->expires;
    memset(upload_id,'\0',strlen(upload_chunk->upload_id)+1);
    sprintf(upload_id,"%s",upload_chunk->upload_id);
    char *buffer = malloc(4000000);
    fp_1=fopen(filename,"r");

    if(fp_1 == NULL)
    {
        DEBUG("open %s failed\n",filename);
        return -1;
    }
    fseek(fp_1,upload_chunk->chunk,SEEK_SET);

    chunk = upload_chunk->chunk;
    while(chunk+offset <= size)
    {
        memset(buffer,0,4000000);
        fread(buffer,sizeof(buffer)/10,10,fp_1);
        res=api_upload_chunk_put(buffer,upload_id,offset,chunk,index,filename);
        if(res != 0)
        {
            free_upload_chunk_info();
            add_upload_chunked_info(filename,upload_id,offset,chunk,expires);

            fclose(fp_1);
            free(upload_id);
            free(buffer);
            return res;
        }
        chunk=chunk+offset;
    }
    offset = size-chunk;
    memset(buffer,0,4000000);
    fread(buffer,sizeof(buffer)/10,10,fp_1);
    res = api_upload_chunk_put(buffer,upload_id,offset,chunk,index,filename);
    if(res != 0)
    {
        free_upload_chunk_info();
        add_upload_chunked_info(filename,upload_id,offset,chunk,expires);
        fclose(fp_1);
        free(buffer);
        free(upload_id);
        return res;
    }
    char *serverpath_encode=oauth_url_escape(serverpath);
    res=api_upload_chunk_commit(upload_id,serverpath_encode,flag,index);
    free(serverpath_encode);
    if(res != 0)
    {
        free_upload_chunk_info();
        fclose(fp_1);
        free(upload_id);
        free(buffer);
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
    DEBUG("api_upload_chunk!!!\n");
    int res=0;
    unsigned long offset=4*1000*1000;//4M chunk
    unsigned long chunk = 0;
    FILE *fp_1;
    char *upload_id;
    fp_1=fopen(filename,"r");

    if(fp_1 == NULL)
    {
        DEBUG("open %s failed\n",filename);
        return -1;
    }
    char *buffer = malloc(4000000);
    memset(buffer,0,4000000);
    fread(buffer,sizeof(buffer)/10,10,fp_1);
    //fwrite(buffer,sizeof(buffer)/10,10,fp_2);
    //rewind(fp_2);
    //printf("buffer : %s\n",buffer);
    start_time = time(NULL);
    res = api_upload_chunk_put(buffer,NULL,offset,0,index,filename);//offset :Content-length;chunk :offset
    if(res != 0)
    {
        fclose(fp_1);
        free(buffer);
        return res;
    }

    /*
    do{
        res=api_upload_chunk_put(buffer,NULL,offset,0);//offset :Content-length;chunk :offset
    }while(res!=0&&res==7||res==35||res==28);
*/
    upload_id = get_upload_id();
    if(upload_id == NULL)
    {
        fclose(fp_1);
        free(buffer);
        return -1;
    }
    cJSON *json = dofile(Con(TMP_R,upload_chunk_1.txt));
    time_t mtime=cJSON_printf_expires(json);
    cJSON_Delete(json);
    DEBUG("filename chunked file expires : %d\n",mtime);
    //exit(1);
    chunk = offset;
    while(chunk+offset <= size)
    {
        memset(buffer,0,4000000);
        fread(buffer,sizeof(buffer)/10,10,fp_1);
        res=api_upload_chunk_put(buffer,upload_id,offset,chunk,index,filename);
        if(res != 0)
        {
            free_upload_chunk_info();
            add_upload_chunked_info(filename,upload_id,offset,chunk,mtime);

            fclose(fp_1);
            free(upload_id);
            free(buffer);
            return res;
        }
        /*
        do{
            res=api_upload_chunk_put(buffer,upload_id,offset,chunk);
        }while(res!=0&&res==7||res==35||res==28);
        */
        chunk=chunk+offset;
    }
    offset=size-chunk;
    memset(buffer,0,4000000);
    fread(buffer,sizeof(buffer)/10,10,fp_1);
    res=api_upload_chunk_put(buffer,upload_id,offset,chunk,index,filename);
    if(res!=0)
    {
        free_upload_chunk_info();
        add_upload_chunked_info(filename,upload_id,offset,chunk,mtime);
        fclose(fp_1);
        free(upload_id);
        free(buffer);
        return res;
    }
    /*
    do{
        res=api_upload_chunk_put(buffer,upload_id,offset,chunk);
    }while(res!=0&&res==7||res==35||res==28);
    */
    char *serverpath_encode=oauth_url_escape(serverpath);
    res=api_upload_chunk_commit(upload_id,serverpath_encode,flag,index);
    free(serverpath_encode);
    if(res!=0)
    {
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


int g_upload_file(char *filename,char *serverpath,int flag,int index)
{


    DEBUG("*****************Upload_file:flag=%d***************\nfilename=%s\nserverpath=%s\n",flag, filename,serverpath);


    if(access(filename,F_OK) != 0)
    {

        DEBUG("Local has no %s\n",filename);
        //add_action_item("upload",filename,g_pSyncList[index]->access_failed_list);
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
            //DEBUG("get_action_item3\n");
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
    DEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!enough!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
#endif
    unsigned long MAX_FILE_SIZE=140*1000*1000;

    int res=0;

    write_log(S_UPLOAD,"",filename,index);

    long long int size = get_file_size(filename);
    if( size == -1)
    {
        if(access(filename,F_OK) != 0)
        {

            DEBUG("Local has no %s\n",filename);
            //add_action_item("upload",filename,g_pSyncList[index]->access_failed_list);
            return LOCAL_FILE_LOST;
        }
    }
    /*if( size >= MAX_FILE_SIZE )
    {
        //vpOkR7xVrTZKrFl05pPSww
        DEBUG("upload_file.size>140000000\n");
        if(upload_chunk != NULL)
        {
            time_t cur_ts = time(NULL);
            time_t expires = upload_chunk->expires;
            if(strcmp(filename,upload_chunk->filename) == 0 || expires == -1 || cur_ts < expires)
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
    }*/
    else
    {
        //char *serverpath_encode=oauth_url_escape(serverpath);
        //DEBUG("api_upload_put!!\n");
        res=api_upload_put(filename,serverpath, flag,index);
        DEBUG("api_upload_put end, res = %d\n", res);
        //free(serverpath_encode);
        if(res!=0)
        {
            return res;
        }
    }
    /*if(finished_initial)
        write_log(S_SYNC,"","",index);
    else
        write_log(S_INITIAL,"","",index);*/
    return 0;
    //fclose(fp_2);

}

int api_refresh_token()
{
    //static int times = 0;
    printf("ci_refresh_token start\n");
    DEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! api_refresh_token!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    /*FILE *fp1, *fp2;
    char clientid[128] = {0};
    char clientsecrets[32] = {0};
    if (fp1 = fopen("/tmp/smartsync/google/temp/clientid", "r") == NULL)
    {
        fprintf(stderr, "fopen()failed:%s\n", strerror(errno));
        return -1;
    }
    if (fread(clientid, sizeof(clientid), 1, fp1)  == 0)
    {
        fprintf(stderr, "fread()failed:%s\n", strerror(errno));
        fclose(fp1);
        return -1;
    }
    if(fp2 = fopen("/tmp/smartsync/google/temp/clientsecrets", "r") == NULL)
    {
        fprintf(stderr, "fopen()failed:%s\n", strerror(errno));
        return -1;
    }
    if (fread(clientsecrets, sizeof(clientsecrets), 1, fp2)  == 0)
    {
        fprintf(stderr, "fread()failed:%s\n", strerror(errno));
        fclose(fp2);
        return -1;
    }
    fclose(fp1);
    fclose(fp2);*/
    DEBUG("4031\n");
    char uri_code[128] = "https://www.googleapis.com/oauth2/v4/token";
    DEBUG("4032\n");
    CURL *curl;
    CURLcode res;
    curl=curl_easy_init();
    char postdata[512] = {0};
    sprintf(postdata, "client_id=757352056609-vljhsi1guf3pnubj7l4b3oiemgfisgov.apps.googleusercontent.com&client_secret=VpzNog5EBwPPohlkTu2gJgdz&refresh_token=%s&grant_type=refresh_token",asus_cfg.user);
    //DEBUG("postdata = %s\n", postdata);
    //char *postdata = "client_id=757352056609-vljhsi1guf3pnubj7l4b3oiemgfisgov.apps.googleusercontent.com&client_secret=VpzNog5EBwPPohlkTu2gJgdz&refresh_token=1/f5rkPPm1KPLrWQZP-uwLqMjYu5UL5BKNfcKFYLdp6EdIgOrJDtdun6zK6XiATCKT&grant_type=refresh_token";
    DEBUG("4041\n");
    long long int bodydata_size=strlen(postdata);
    DEBUG("4043\n");
    struct curl_slist *headers = NULL;
    FILE * fp_hd;
    fp_hd = fopen(Con(TMP_R,refresh_header.txt),"w+");
    if(curl){
        DEBUG("4046\n");
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        //headers=curl_slist_append(headers, "Expect: " );
        headers=curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded" );
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headers);
        curl_easy_setopt(curl,CURLOPT_URL, uri_code);

        //curl_easy_setopt(curl, CURLOPT_USERAGENT, "fuse-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);


        //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

         printf("postdata = ***%s***\n", postdata);
         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
         curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE_LARGE,bodydata_size);
         curl_easy_setopt(curl,CURLOPT_POST,1L);

         FILE *fp;
         fp=fopen(Con(TMP_R,data_refresh_token.txt),"w");

         curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
         curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);
        DEBUG("api_refresh_token,before_curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_refresh_token,res=%d\n",res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headers);
        if(res!=0){
            fclose(fp_hd);
            DEBUG("refresh_token failed \n");
            return res;
        }
        else
        {
            rewind(fp_hd);
            char tmp_[256]="\0";
            fgets(tmp_,sizeof(tmp_),fp_hd);
            DEBUG("api_refresh_token,tmp_=%s\n", tmp_);
            if(strstr(tmp_,"401") != NULL)
            {
                    DEBUG("401,tmp_=%s\n", tmp_);
                    write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                    fclose(fp_hd);
                    return -1;
            }
            else if(strstr(tmp_,"404") != NULL)
            {
                    fclose(fp_hd);
                    return -1;
            }

            fclose(fp_hd);
    }

    cJSON *json;
    json=dofile(Con(TMP_R,data_refresh_token.txt));
    if(json)
    {
        cJSON_printf_new_access_token(json);
        DEBUG("aftercJSON_printf_new_access_token\n");
        cJSON_Delete(json);
        //times++;
        return 0;
    }
    else
    {
        return -1;
    }
}
}

int g_delete(char *herf,int index, char *delete_ids)
{
    DEBUG("*****************api_trash!herf=%s******************\n", herf);
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char myurl[256] = {0};
    sprintf(myurl, "https://www.googleapis.com/drive/v2/files/%s/trash", delete_ids);

    char *header;
    header = (char *)malloc(128);
    memset(header, 0, sizeof(header));
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    //DEBUG("header = %s\n", header);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist, "Expect: " );
    headerlist=curl_slist_append(headerlist,header);
    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,myurl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        fp=fopen(Con(TMP_R,api_delete.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        DEBUG("g_delete,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("g_delete,res = %d\n", res);//断网res=6或7
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            char error_info[100];
            memset(error_info,0,sizeof(error_info));
            sprintf(error_info,"%s%s","delete fail ",herf);
            DEBUG("delete %s failed,id [%d]!\n",herf,res);
            return res;
        }
    }
    free(header);
    return 0;
}

int g_delete2(char *herf,int index)
{
    DEBUG("*****************api_delete1!herf=%s******************\n", herf);
    char id[64] = "\0";
    char *id_tmp = NULL;
    //DEBUG("before path_to_id,herf = %s,index = %d\n", herf, index);
    id_tmp = path_to_id(herf, index);
    //DEBUG("after path_to_id\n");
    if(id_tmp != NULL)
        strcpy(id, id_tmp);
    //DEBUG("id = %s\n", id);
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char myurl[256] = {0};
    sprintf(myurl, "https://www.googleapis.com/drive/v2/files/%s", delete_id);
    //DEBUG("myurl = %s\n", myurl);
    char *herf_tmp=oauth_url_escape(herf);
    char *data=(char *)malloc(sizeof(char)*(strlen(herf_tmp)+64));
    memset(data,0,strlen(herf_tmp)+64);

    sprintf(data,"%s%s","root=google&path=",herf_tmp);
    DEBUG("data = %s\n", data);
    free(herf_tmp);

    char *header;
    header = (char *)malloc(128);
    memset(header, 0, sizeof(header));
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    //DEBUG("header = %s\n", header);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist, "Expect: " );
    headerlist=curl_slist_append(headerlist,header);
    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,myurl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        fp=fopen(Con(TMP_R,api_delete.txt),"w");
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        DEBUG("g_delete,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("g_delete2,res = %d\n", res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        if(res!=0){
            free(header);
            free(data);
            char error_info[100];
            memset(error_info,0,sizeof(error_info));
            sprintf(error_info,"%s%s","delete fail ",herf);
            DEBUG("delete %s failed,id [%d]!\n",herf,res);
            return res;
        }
    }
    free(header);
    free(data);
    return 0;
}
/*
 lcoalpath=>local name
 folderpath=>server path
*/
/*int api_create_folder(char *localpath,char *foldername,char *newfolderid)
{

    wdDEBUG("****************create_server_folder****************\n");
    int flag = 0;


    if(access(localpath,0) != 0)
    {

        DEBUG("Local has no %s\n",localpath);

        return LOCAL_FILE_LOST;
    }
    char *folderid = NULL;



    CURL *curl;
    CURLcode res;
    FILE *fp;

    char *postdata[256] = {0};
    DEBUG("before strrchr, foldername = %s\n", foldername);
    char *name = strrchr(foldername, '/');
    DEBUG("after strrchr, name = %s\n", name);
    name ++;
    DEBUG("after name++, name = %s\n", name);
        DEBUG("treenode isn't NULL!\n");

    //DEBUG("pastdata = %s\n", postdata);
    long long int bodydata_size=strlen(postdata);
    FILE *fp_hd;



    char *foldername_tmp=oauth_url_escape(foldername);
    char *data=(char *)malloc(sizeof(char)*(strlen(foldername_tmp)+32));
    memset(data,0,strlen(foldername_tmp)+32);
    sprintf(data,"%s%s","root=google&path=",foldername_tmp);
    free(foldername_tmp);
    char *header;
    header = (char *)malloc(128);
    memset(header, 0, sizeof(header));
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    struct curl_slist *headerlist=NULL;
    do
    {
        flag = 0;
        headerlist = NULL;
        folderid = path_to_parents_id(foldername, 0);
        sprintf(postdata,"\n{\n'title': '%s',\n'parents': [{'id':'%s'}],\n'mimeType': 'application/vnd.google-apps.folder'\n}", name, folderid);
        DEBUG("before curl_easy_init\n");
        curl=curl_easy_init();
        DEBUG("after curl_easy_init\n");
        headerlist=curl_slist_append(headerlist,header);
        DEBUG("curl_slist_append1\n");
        headerlist=curl_slist_append(headerlist,"Expect: ");
        headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
        if(curl){
                DEBUG("if curl\n");
                fp=fopen(Con(TMP_R,create_folder.txt),"w");
                fp_hd=fopen(Con(TMP_R,create_folder_header.txt),"w+");

                curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
                curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL

                //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(curl,CURLOPT_URL,"https://www.googleapis.com/drive/v2/files");
                CURL_DEBUG;
                curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
                DEBUG("*postdata = %s\n", postdata);
                curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postdata);
                curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE_LARGE,bodydata_size);
                curl_easy_setopt(curl,CURLOPT_POST,1L);
                //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);

                //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
                curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
                curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);

                DEBUG("curl_easy_perform:%s\n", name);
                res=curl_easy_perform(curl);
                fclose(fp);
                //fclose(fp_hd);
                curl_slist_free_all(headerlist);
                if(res!=0){
                    fclose(fp_hd);
                    free(header);
                    free(data);
                    DEBUG("create %s failed,is [%d] !\n",foldername,res);
                    curl_easy_cleanup(curl);
                    return res;
                }
                else
                {

                    rewind(fp_hd);
                    char tmp_[256]="\0";

                    fgets(tmp_,sizeof(tmp_),fp_hd);
                    DEBUG("api_create_folder,tmp_=%s\n", tmp_);
                    if(strstr(tmp_,"401") != NULL)
                    {
                            DEBUG("401,tmp_=%s\n", tmp_);
                            write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                            exit_loop = 1;
                            access_token_expired = 1;
                            fclose(fp_hd);
                            free(header);
                            free(data);
                            curl_easy_cleanup(curl);
                            return -1;
                    }
                    if(strstr(tmp_,"404") != NULL)
                    {
                            DEBUG("404,tmp_=%s\n", tmp_);
                            char path[1024] = {0};
                            char parentfolder[1024] = {0};
                            DEBUG("3897\n");
                            name = strrchr(foldername, '/');
                            DEBUG("name = %s\n", name);
                            strncpy(parentfolder,foldername, strlen(foldername) - strlen(name));
                            DEBUG("parentfolder=%s\n", parentfolder);
                            name = strrchr(localpath, '/');
                            DEBUG("*name = %s\n", name);
                            strncpy(path, localpath, strlen(localpath) - strlen(name));
                            DEBUG("path=%s\n", path);
                            DEBUG("api_creater_folder 404not found,now creater parent folder!\n");
                            int status = 0;
                            status = api_create_folder(path, parentfolder, newfolderid);
                            if(status != 0)
                            {
                                fclose(fp_hd);
                                free(header);
                                free(data);
                                curl_easy_cleanup(curl);
                                return -1;
                            }
                            else
                           {
                                DEBUG("status = %d\n", status);
                                flag = 1;
                            }
                    }
                    fclose(fp_hd);
                }


            }
        DEBUG("after if(curl)\n");
        DEBUG("f@@@lag = %d\n", flag);
         }while(flag == 1);
        DEBUG("%s:breake while\n", name);
        if (curl)
        {
            //deal big or small case sentive problem,will return 403
            if(parse_create_folder(Con(TMP_R,create_folder_header.txt)))
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
                    char newfolderid1[32] = {0};
                    do
                    {
                        DEBUG("create folder1\n");
                        status = api_create_folder(g_newname,s_newname, newfolderid1);
                    }while(status != 0 && !exit_loop);
                    my_free(g_newname);
                    my_free(s_newname);
                }
#endif
            }
        }
    curl_easy_cleanup(curl);
    free(header);
    free(data);

    cJSON *json;
    json=dofile(Con(TMP_R,create_folder.txt));
    if(json)
    {
        cJSON *item_new_folder_id;

            item_new_folder_id = cJSON_GetObjectItem( json , "id");
            if(item_new_folder_id == NULL){
                printf("fail to get item_new_folder_id");
                //continue;
            }
            if( item_new_folder_id->type == cJSON_String ){
                strcpy(newfolderid,item_new_folder_id->valuestring);
                printf("!!!newfolderid:%s\n",  newfolderid);
            }
    }
    cJSON_Delete(json);
    DEBUG("api_creater_folder end,return 0\n");
    return 0;
}
*/
int g_create_folder(char *localpath,char *foldername,char *newfolderid, int index, int i)
{

    DEBUG("****************create_server_folder****************\n");
    //DEBUG("localpath = %s,foldername = %s\n", localpath, foldername);// /tmp/mnt/TINA/cc/dd   /dd  创建文件夹/dd/ee

    if(access(localpath,0) != 0)
    {

        DEBUG("Local has no %s\n",localpath);

        return LOCAL_FILE_LOST;
    }
    char *folderid = NULL;

        folderid = path_to_parents_id(foldername, index);

    CURL *curl;
    CURLcode res;
    FILE *fp;

    char *postdata[256] = {0};
    //DEBUG("before strrchr, foldername = %s\n", foldername);
    char *name = strrchr(foldername, '/');
    //DEBUG("after strrchr, name = %s\n", name);
    name ++;
    //DEBUG("after name++, name = %s\n", name);
        //DEBUG("treenode isn't NULL!\n");
        sprintf(postdata,"\n{\n'title': '%s',\n'parents': [{'id':'%s'}],\n'mimeType': 'application/vnd.google-apps.folder'\n}", name, folderid);
    //DEBUG("pastdata = %s\n", postdata);
    long long int bodydata_size=strlen(postdata);
    FILE *fp_hd;

    char backup_file[1024] = {0};
    char backup_file1[1024] = {0};
    int c;

    //char data[]="root=google&path=/main";
    char *foldername_tmp=oauth_url_escape(foldername);
    char *data=(char *)malloc(sizeof(char)*(strlen(foldername_tmp)+32));
    memset(data,0,strlen(foldername_tmp)+32);
    sprintf(data,"%s%s","root=google&path=",foldername_tmp);
    free(foldername_tmp);
    char *header;
    header = (char *)malloc(128);
    memset(header, 0, sizeof(header));
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();
    headerlist=curl_slist_append(headerlist,header);
    headerlist=curl_slist_append(headerlist,"Expect: ");
    headerlist=curl_slist_append(headerlist, "Content-Type: application/json" );
    if(curl){
        sprintf(backup_file, "create_folder%d.txt", i);
        fp=fopen(Con(TMP_R,backup_file),"w+");
        sprintf(backup_file1, "create_folder_header%d.txt", i);
        fp_hd = fopen(Con(TMP_R,backup_file1),"w+");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl,CURLOPT_URL,"https://www.googleapis.com/drive/v2/files");
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        DEBUG("postdata = %s\n", postdata);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postdata);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE_LARGE,bodydata_size);
        curl_easy_setopt(curl,CURLOPT_POST,1L);
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);

        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,fp_hd);
        DEBUG("g_create_folder,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("g_create_folder,res=%d\n",res);
        curl_easy_cleanup(curl);
        fclose(fp);
        //fclose(fp_hd);
        curl_slist_free_all(headerlist);
        if(res!=0){
            fclose(fp_hd);
            free(header);
            free(data);
            DEBUG("create %s failed,is [%d] !\n",foldername,res);
            return res;
        }
        else
        {
            rewind(fp_hd);

            char tmp_[256]="\0";
            fgets(tmp_,sizeof(tmp_),fp_hd);
            DEBUG("api_create_folder,tmp_=%s\n", tmp_);
            if(strstr(tmp_,"401") != NULL)
            {
                    DEBUG("401,tmp_=%s\n", tmp_);
                    write_log(S_ERROR,"Access token has expired,please re-authenticate!","",0);
                    exit_loop = 1;
                    access_token_expired = 1;
                    fclose(fp_hd);
                    free(header);
                    free(data);
                    //free(header1);
                    //free(myUrl);
                    return -1;
            }
            else if(strstr(tmp_,"404") != NULL)
            {
                    DEBUG("404,tmp_=%s\n", tmp_);
                    i += 1;
                    char localpath1[1024] = {0};
                    char foldername1[1024] = {0};
                    char newfolderid[32] = {0};
                    path_to_ppath(localpath, localpath1);
                    path_to_ppath(foldername, foldername1);
                    if( g_create_folder(localpath1, foldername1,newfolderid, index, i) == 0)
                    {
                        upload_serverlist_(index,localpath1,foldername1,newfolderid);
                    }
                    DEBUG("api_creater_folder endd@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
                    fclose(fp_hd);
                    free(header);
                    free(data);
                    return -1;
            }

            fclose(fp_hd);
            /*deal big or small case sentive problem,will return 403*/
            if(parse_create_folder(Con(TMP_R,backup_file1)))
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
                    char newfolderid[32] = {0};
                    do
                    {
                        DEBUG("create folder1\n");
                        status = g_create_folder(g_newname,s_newname, newfolderid, index, i);
                    }while(status != 0 && !exit_loop);
                    my_free(g_newname);
                    my_free(s_newname);
                }
#endif
            }
        }
    }

    free(header);
    free(data);

    cJSON *json;
    json=dofile(Con(TMP_R,backup_file));
    if(json)
    {
        cJSON *item_new_folder_id;

            item_new_folder_id = cJSON_GetObjectItem( json , "id");
            if(item_new_folder_id == NULL){
                printf("fail to get item_new_folder_id");
                cJSON_Delete(json);
                return -1;
            }
            if( item_new_folder_id->type == cJSON_String ){
                strcpy(newfolderid,item_new_folder_id->valuestring);
                printf("!!!newfolderid%s\n",  newfolderid);
            }
    }
    cJSON_Delete(json);

    return 0;
}

time_t api_getmtime(char *id, char *phref,cJSON *(*cmd_data)(char *filename))
{
    DEBUG("***api_getmtime***\n");
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char *myUrl;
    char *phref_tmp=oauth_url_escape(phref);
    myUrl=(char *)malloc(sizeof(char)*(strlen(phref_tmp)+128));
    memset(myUrl,0,strlen(phref_tmp)+128);
    sprintf(myUrl,"https://www.googleapis.com/drive/v2/files/%s", id);
    free(phref_tmp);
    FILE *hd;

    char *header;
    header=(char *)malloc(128);
    memset(header, 0, 128);
    sprintf(header, "Authorization: Bearer %s", auth->oauth_token);
    struct curl_slist *headerlist=NULL;
    curl=curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "asus-google-drive/0.1");
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // SSL
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        headerlist=curl_slist_append(headerlist, "Expect: " );
        headerlist=curl_slist_append(headerlist, header);

        curl_easy_setopt(curl,CURLOPT_URL,myUrl);
        CURL_DEBUG;
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
        //curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
        //curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
        //fp=fopen("/tmp/data_5.txt","w");
        fp=fopen(Con(TMP_R,mtime.txt),"w");

        //curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
        hd=fopen(Con(TMP_R,api_getmime.txt),"w+");
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,hd);
        DEBUG("api_getmtime,before curl_easy_perform\n");
        res=curl_easy_perform(curl);
        DEBUG("api_getmtime,res=%d\n", res);
        curl_easy_cleanup(curl);
        fclose(fp);
        curl_slist_free_all(headerlist);
        //curl_global_cleanup();
        if(res!=0){
            fclose(hd);
            free(header);
            free(myUrl);
            DEBUG("api_getmtime %s [%d] failed!\n",id,res);
            return -2;
        }
        else
        {
            rewind(hd);
            char tmp[256]="\0";
            fgets(tmp,sizeof(tmp),hd);
            //DEBUG("tmp:%s\n",tmp);
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
            else if(strstr(tmp,"404")!=NULL)
            {
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
    time_t mtime;
    json=cmd_data(Con(TMP_R,mtime.txt));
    mtime=cJSON_printf_mtime(json);
    //DEBUG("api_getmtime,mtime=%lu\n", mtime);
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
int newer_file(char *id, char *localpath,int index,int is_init,int rule){

    char *serverpath;
    serverpath = localpath_to_serverpath(localpath,index);
    //printf("serverpath = %s\n",serverpath);
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
    modtime1 = api_getmtime(id, serverpath,dofile);
    //DEBUG("*****************modtime1 = %lu\n", modtime1);
    free(serverpath);
    if(modtime1 == -1 || modtime1 == -2)
    {

        DEBUG("newer_file Getmodtime failed!\n");

        return -1;
    }

    struct stat buf;

    //printf("!!!localpath = %s\n",localpath);

    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
        return -1;
    }
    modtime2 = buf.st_mtime;
    DEBUG("@@@local>modtime2 = %lu,server>modtime1 = %lu@@@\n",modtime2,modtime1);

    if(is_init)
    {
        if(modtime2 < modtime1)
            return 1;
        else if(modtime2 == modtime1 || modtime2 -1 == modtime1) //FAT32 and umount HD than plugin HD ,it will be local time less then server time 1 sec;
        {
            if(modtime2 -1 == modtime1)
                ChangeFile_modtime(localpath,modtime1);
            return 2;
        }
        else
            return 0;
    }
    else
    {
        if(rule == 1) //download only
        {
            if(modtime2 < modtime1)
            {
                if(modtime2 == old_mtime)
                    return 1;
                else
                    return 0;
            }
            else if(modtime2 == modtime1|| modtime2-1 == modtime1)
                return 2;
            else
            {
                    return 0;
            }
        }
        else
        {
            if(modtime2 < modtime1)
                return 1;
            else if(modtime2 == modtime1 || modtime2-1 == modtime1)
                return 2;
            else
                return 0;
        }
    }


}

int newer_file2(char *id, char *localpath,int index,int is_init,int rule){

    char *serverpath;
    serverpath = localpath_to_serverpath(localpath,index);
    //printf("serverpath = %s\n",serverpath);
    time_t old_mtime;
    if(is_init == 0 && rule == 1)
    {
        CloudFile *filetmp;
        DEBUG("newer_file,get_CloudFile_node\n");
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
    modtime1 = api_getmtime(id, serverpath,dofile);
    //DEBUG("*****************modtime1 = %lu\n", modtime1);
    free(serverpath);
    if(modtime1 == -1 || modtime1 == -2)
    {

        DEBUG("newer_file Getmodtime failed!\n");

        return -1;
    }

    struct stat buf;

    //printf("!!!localpath = %s\n",localpath);

    if( stat(localpath,&buf) == -1)
    {
        perror("stat:");
        return -1;
    }
    modtime2 = buf.st_mtime;
    DEBUG("@@@local>modtime2 = %lu,server>modtime1 = %lu@@@\n",modtime2,modtime1);

    if(is_init)
    {
        if(modtime2 > modtime1)
        {
            DEBUG("is_init,newer_file return 0\n");
            return 0;
        }
        else if(modtime2 == modtime1 || modtime2 +1 == modtime1) //FAT32 and umount HD than plugin HD ,it will be local time less then server time 1 sec;
        {
            if(modtime2 +1 == modtime1)
                ChangeFile_modtime(localpath,modtime1);
            DEBUG("is_init,newer_file return 2\n");
            return 2;
        }
        else
        {
            DEBUG("is_init,newer_file return 1\n");
            return 1;
        }
    }
    else
    {
        if(rule == 1) //download only
        {
            if(modtime2 > modtime1)
            {
                DEBUG("download only,newer_file return 0\n");
                return 0;
            }
            else if(modtime2 == modtime1)
            {
                DEBUG("download only,newer_file return 2\n");
                return 2;
            }
            else
            {
                if(modtime2 == old_mtime)
                {
                    DEBUG("download only...,newer_file return 1\n");
                    return 1;
                }
                else
                {
                    DEBUG("download only...,newer_file return 0\n");
                    return 0;
                }
            }
        }
        else
        {
            if(modtime2 > modtime1)
            {
                DEBUG("newer_file return 0\n");
                return 0;
            }
            else if(modtime2 == modtime1)
            {
                DEBUG("newer_file return 2\n");
                return 2;
            }
            else
            {
                DEBUG("newer_file return 1\n");
                return 1;
            }
        }
    }
}
char *change_local_same_name(char *fullname)
{
    int i = 1;
    char *temp_name = NULL;
    //char *temp_suffix = ".asus.td";
    int len = 0;
    char *path;
    char newfilename[256];

    char *fullname_tmp = NULL;
    fullname_tmp = my_str_malloc(strlen(fullname)+1);
    sprintf(fullname_tmp,"%s",fullname);

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
        sprintf(newfilename,"%s(%d)",newfilename,i);

        DEBUG("newfilename = %s\n",newfilename);

        i++;

        temp_name = my_str_malloc((size_t)(strlen(path)+strlen(newfilename)+1));
        sprintf(temp_name,"%s%s",path,newfilename);

        if(access(temp_name,F_OK) != 0)
            break;
        else
            free(temp_name);
    }

    free(path);
    free(filename);
    return temp_name;
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
    DEBUG("get_server_exist path:%s\n",path);
    status=api_metadata_one(path,temp_name,dofile,NULL,index);
    if(status == -1)
    {
        free_CloudFile_item(FileList_one);
        free(path);
        return NULL;
    }
    char *temp_name_g = my_str_malloc(strlen(temp_name)+1);
    sprintf(temp_name_g,"%s",temp_name);

    CloudFile *de_filecurrent;
    de_filecurrent=FileList_one->next;
    while(de_filecurrent != NULL)
    {
        if(de_filecurrent->href != NULL)
        {
            DEBUG("de_filecurrent->href:%s\n",de_filecurrent->href);
            DEBUG("temp_name           :%s\n",temp_name);
            char *tmp_href = my_str_malloc(strlen(de_filecurrent->href)+1);
            strcpy(tmp_href,de_filecurrent->href);
            if(strcmp(de_filecurrent->href,temp_name) != 0 && (status=strcmp(strlwr(tmp_href),strlwr(temp_name_g)))==0)
            {
                char *conflict_name = my_str_malloc(strlen(de_filecurrent->href)+1);
                strcpy(conflict_name,de_filecurrent->href);
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
//    free_CloudFile_item(FileList_one);
//    return 0;

}


char *search_newpath(char *href,int index)
{
    DEBUG("################search_newpath###########\n");
    DEBUG("href = %s\n",href);
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
            //while(*ch != '@')
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

            strcpy(temp,ch);
            //p = strchr(temp,'@');
            p = strchr(temp,'\n');

            path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));


            if(p!=NULL)
                snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

            p++;
            if(strcmp(cmd_name, "rename0") == 0)
            {
                char *p1 = NULL;

                //p1 = strchr(p,'@');
                p1 = strchr(p,'\n');

                if(p1 != NULL)
                    strncpy(oldname,p,strlen(p)- strlen(p1));

                p1++;

                strcpy(newname,p1);

                //DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

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

                //p1 = strchr(p,'@');
                p1 = strchr(p,'\n');

                oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

                if(p1 != NULL)
                    snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

                p1++;

                strcpy(oldname,p1);

                //DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);


                if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
                {
                    free(temp);
                    free(path);
                    pTemp=pTemp->next_ptr;
                    continue;
                }
            }
            //DEBUG("111\n");
            free(temp);
            //DEBUG("222\n");
            if(strcmp(cmd_name, "move0") == 0)
            {
                mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
                mv_oldpath = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
                sprintf(mv_newpath,"%s/%s",path,oldname);
                sprintf(mv_oldpath,"%s/%s",oldpath,oldname);
                free(oldpath);
            }
            else
            {
                mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
                mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
                sprintf(mv_newpath,"%s/%s",path,newname);
                sprintf(mv_oldpath,"%s/%s",path,oldname);
            }
            //DEBUG("333\n");
            char *pTemp_t=(char *)malloc(sizeof(char)*(strlen(href)+1+1));
            char *oldpath_t=(char *)malloc(sizeof(char)*(strlen(mv_oldpath)+1+1));
            memset(pTemp_t,'\0',strlen(href)+1+1);
            memset(oldpath_t,'\0',strlen(mv_oldpath)+1+1);
            sprintf(pTemp_t,"%s/",href);
            sprintf(oldpath_t,"%s/",mv_oldpath);

            DEBUG("pTemp_t=%s ,oldpath_t=%s\n",pTemp_t ,oldpath_t);
            char *p_t = NULL;
            char *ret_p = NULL;
            if((p_t=strstr(pTemp_t,oldpath_t)) != NULL)
            {
                //DEBUG("666\n");
                p_t=p_t+strlen(oldpath_t);
                DEBUG("p_t=%s\n",p_t);
                ret_p = (char *)malloc(sizeof(char) * (strlen(mv_newpath)+strlen(p_t) + 1 +1));
                memset(ret_p,'\0',strlen(mv_newpath)+strlen(p_t) + 1 +1);
                sprintf(ret_p,"%s/%s",mv_newpath,p_t);
                DEBUG("ret_p = %s\n",ret_p);
                flag_r = 1;
            }
            //DEBUG("444\n");
            free(pTemp_t);
            free(oldpath_t);

            free(path);
            free(mv_oldpath);
            free(mv_newpath);
           // DEBUG("555\n");
            if(1 == flag_r)
            {
                DEBUG("ret_p = %s\n",ret_p);
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
    //while(*ch != '@')
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

    strcpy(temp,ch);
    //p = strchr(temp,'@');
    p = strchr(temp,'\n');

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));


    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",6) == 0)
    {
        char *p1 = NULL;

        //p1 = strchr(p,'@');
        p1 = strchr(p,'\n');

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(newname,p1);

        //DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

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

        //p1 = strchr(p,'@');
        p1 = strchr(p,'\n');

        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        strcpy(oldname,p1);

        //DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);


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
        sprintf(mv_newpath,"%s/%s",path,oldname);
        sprintf(mv_oldpath,"%s/%s",oldpath,oldname);
        free(oldpath);
    }
    else
    {
        mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        sprintf(mv_newpath,"%s/%s",path,newname);
        sprintf(mv_oldpath,"%s/%s",path,oldname);
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
            sprintf(p1,"%s%s",pTemp->re_cmd,oldname);
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
            sprintf(p1,"%s%s",pTemp->re_cmd,oldname);
            sprintf(p2,"%s%s",pTemp->re_cmd,newname);
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
    DEBUG("************************check_localpath_is_socket***********************\n");
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

            sprintf(local_p1,"\n%s\n%s",ParentHerf,Fname);

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
                sprintf(local_p2,"%s%s",pTemp->re_cmd,filename);
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

    //show(g_pSyncList[i]->SocketActionList->head);
}

int check_socket_parser(char *cmd,int index,char *re_cmd,char *str_path,char *so_filename,char *sn_filename)
{
//    if(strstr(cmd,"conflict") != NULL)
//        return 0;
    if( !strncmp(cmd,"exit",4))
    {
        DEBUG("exit socket\n");
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
    //while(*ch != '@')
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

    strcpy(temp,ch);
    //p = strchr(temp,'@');
    p = strchr(temp,'\n');

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));


    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",strlen("rename")) == 0)
    {
        char *p1 = NULL;

        //p1 = strchr(p,'@');
        p1 = strchr(p,'\n');

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(newname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

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

        //p1 = strchr(p,'@');
        p1 = strchr(p,'\n');

        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        strcpy(oldname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);


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
        strcpy(filename,p);

        DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);

        if(filename[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }

    free(temp);

    if( strcmp(cmd_name, "createfile") == 0 || strcmp(cmd_name, "dragfile") == 0 )
    {}
    else if( strcmp(cmd_name, "modify") == 0 )
    {
        if(re_cmd)
        {
            if(strcmp(re_cmd,str_path) == 0 && strcmp(filename,so_filename)==0)
            {
                pthread_mutex_lock(&mutex_socket);
//                my_free(cmd);
//                cmd = my_str_malloc(512);
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
//                my_free(cmd);
//                cmd = my_str_malloc(512);
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
//            my_free(cmd);
//            cmd = my_str_malloc(512);
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
//                    my_free(cmd);
//                    cmd = my_str_malloc(512);
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
                    //my_free(cmd);
                    //cmd = my_str_malloc(512);
                    memset(cmd,0,1024);
                    sprintf(cmd,"%s%s%s%s%s%s%s",cmd_name,CMD_SPLIT,path,CMD_SPLIT,sn_filename,CMD_SPLIT,newname);
                    //printf("cmd=%s\n",cmd);
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
//                    my_free(cmd);
//                    cmd = my_str_malloc(512);
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
//                    my_free(cmd);
//                    cmd = my_str_malloc(512);
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
    sprintf(old_filename,"%s",p);

    p = NULL;
    char *new_filename;
    p = strrchr(new_name,'/');
    p++;
    new_filename = my_str_malloc(strlen(p)+1);
    sprintf(new_filename,"%s",p);

    DEBUG("*****************updata_socket_list***************\n");
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

int dragfolder_old_dir(char *dir,int index,char *old_dir)
{

    DEBUG("dragfolder_old_dir,dir = %s\n",dir);

    int status;
    int exist;
    struct dirent *ent = NULL;
    DIR *pDir;
    pDir=opendir(dir);
            char newfolderid[32] = {0};
    if(pDir != NULL)
    {
        //char *serverpath=localpath_to_serverpath(old_dir,index);
        DEBUG("create folder6\n");
        status=g_create_folder(dir,old_dir, newfolderid, index, 0);
        if(status != 0)
        {

            DEBUG("Create %s failed\n",old_dir);

            //return -1;
            closedir(pDir);
            return status;
        }
        char backup_file3[1024] = {0};
        sprintf(backup_file3, "%screate_folder_header0.txt",TMP_R);
        exist=parse_create_folder(backup_file3);
        if(exist)
        {
            char *newname;
            newname=change_server_same_name(old_dir,NULL, index);
            DEBUG("api_move7!\n");
            status = g_move(old_dir,newname,index,0,NULL);

            free(newname);
            if(status ==0)
            {
                DEBUG("create folder7\n");
                status=g_create_folder(dir,old_dir, newfolderid, index, 0);
            }
            if(status != 0)
            {
                closedir(pDir);
                return status;
            }
        }
        //free(serverpath);
        while((ent=readdir(pDir)) != NULL)
        {
            if(ent->d_name[0] == '.')
                continue;
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;
            char *fullname;
            fullname = (char *)malloc(sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2));
            memset(fullname,'\0',strlen(dir)+strlen(ent->d_name)+2);
            sprintf(fullname,"%s/%s",dir,ent->d_name);

            char *fullname_r;
            fullname_r = (char *)malloc(sizeof(char)*(strlen(old_dir)+strlen(ent->d_name)+2));
            memset(fullname_r,'\0',strlen(old_dir)+strlen(ent->d_name)+2);
            sprintf(fullname_r,"%s/%s",old_dir,ent->d_name);

            status = check_localpath_is_socket(index,dir,ent->d_name,fullname);
            if(status == 1)
            {
                DEBUG("the %s is socket ,so do nothing\n",fullname);
                free(fullname);
                free(fullname_r);
                continue;
            }

            if(test_if_dir(fullname) == 1)
            {

                status = dragfolder_old_dir(fullname,index,fullname_r);
                if(status != 0)
                {

                    DEBUG("CreateFolder %s failed\n",fullname);

                    free(fullname);
                    free(fullname_r);
                    closedir(pDir);
                    return status;
                }
            }
            else
            {
                char *serverpath_1=localpath_to_serverpath(fullname_r,index);
                DEBUG("upload12\n");
                status=g_upload_file(fullname,serverpath_1,1,index);
                if(status == 0)
                {
                    cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf_mtime(json);
                    cJSON_Delete(json);
                    ChangeFile_modtime(fullname,mtime);

                }
                else if(status == SERVER_SPACE_NOT_ENOUGH)
                {

                    DEBUG("upload %s failed,server space is not enough!\n",fullname);

                }
                else if(status == LOCAL_FILE_LOST)
                {

                    DEBUG("upload %s failed,local file lost!\n",fullname);

                }
                else
                {

                    DEBUG("upload %s failed\n",fullname);

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

    DEBUG("dir = %s\n",dir);

    int status;
    int exist;
    struct dirent *ent = NULL;
    DIR *pDir;
    pDir=opendir(dir);
            char newfolderid[32] = {0};
    if(pDir != NULL)
    {
        char *serverpath=localpath_to_serverpath(dir,index);
        DEBUG("create folder8\n");
        status=g_create_folder(dir,serverpath, newfolderid, index, 0);

        //*****************

        //*****************
        if(status != 0)
        {

            DEBUG("Create %s failed\n",serverpath);

            //return -1;
            closedir(pDir);
            return status;
        }

        exist=parse_create_folder(Con(TMP_R,create_folder.txt));
        if(exist)
        {
            char *newname;
            newname=change_server_same_name(serverpath,NULL, index);
            DEBUG("api_move8!\n");
            status = g_move(serverpath,newname,index,0,NULL);

            free(newname);
            if(status ==0)
            {
                DEBUG("create folder9\n");
                status=g_create_folder(dir,serverpath, newfolderid, index, 0);
            }
            if(status != 0)
            {
                closedir(pDir);
                return status;
            }
        }
        upload_serverlist_(index,dir,serverpath, newfolderid);
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

            sprintf(fullname,"%s/%s",dir,ent->d_name);

            /*
             fix :below question
             a.rename local path A,and server is not exist;
             b.create a file 'a' in A/;
             final-->the server will exist 'a' and 'a(1)';
            */
            status = check_localpath_is_socket(index,dir,ent->d_name,fullname);
            if(status == 1)
            {
                DEBUG("the %s is socket ,so do nothing\n",fullname);
                free(fullname);
                continue;
            }
            if(test_if_dir(fullname) == 1)
            {

                status = dragfolder(fullname,index);
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
                char *serverpath_1=localpath_to_serverpath(fullname,index);
                //DEBUG("before upload13, need browse_to_tree\n");
                //********
                free_server_tree(g_pSyncList[index]->ServerRootNode);
                g_pSyncList[index]->ServerRootNode = NULL;
                g_pSyncList[index]->ServerRootNode = create_server_treeroot();
                DEBUG("browse_to_tree10\n");
                status = g_browse_to_tree("/", rootid, g_pSyncList[index]->ServerRootNode, index);
                //********
                DEBUG("upload13\n");
                status=g_upload_file(fullname,serverpath_1,1,index);
                if(status == 0)
                {
                    cJSON *json = dofile(Con(TMP_R,upload_chunk_commit.txt));
                    time_t mtime=cJSON_printf_mtime(json);
                    cJSON_Delete(json);
                    ChangeFile_modtime(fullname,mtime);

                }
                else if(status == SERVER_SPACE_NOT_ENOUGH)
                {

                    DEBUG("upload %s failed,server space is not enough!\n",fullname);

                }
                else if(status == LOCAL_FILE_LOST)
                {

                    DEBUG("upload %s failed,local file lost!\n",fullname);

                }
                else
                {

                    DEBUG("upload %s failed\n",fullname);

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
        sprintf(header,"%s%s",http_basic_authentication,auth->oauth_token);
        break;
    case 2:
        header_len = strlen(URI_Query_Parameter) + strlen(auth->oauth_token) +1;
        header = my_str_malloc(header_len);
        sprintf(header,"%s%s",URI_Query_Parameter,auth->oauth_token);
        break;
    default:
        break;
    }
    //sprintf(header,"%s%s",http_basic_authentication,auth->oauth_token);
    //sprintf(header,"%s%s","access_token=","XDwYx52HJBYAAAAAAAABcY809z2fKmv_xai8IZXzYmZKcIADF9elZ55nmeUNR_m2");
    DEBUG("makeAuthorize>%s\n",header);
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

    //snprintf(header_signature_method,64,"%s","HMAC-SHA1");
    snprintf(header_signature_method,64,"%s","PLAINTEXT");
    if(flag==1)
        snprintf(prekey,128,"%s","5vq8jog8wgpx1p0&");
    else if(flag==2)
        //snprintf(prekey,128,"%s%s","5vq8jog8wgpx1p0&","YXG59mgidir11f6b");
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
    //snprintf(prekey,128,"%s&%s","5vq8jog8wgpx1p0",auth->oauth_token_secret);
    //snprintf(prekey,128,"%s","5vq8jog8wgpx1p0&efghhkqrt68qta2");
    sec = time((time_t *)NULL);
    snprintf(header_timestamp,64,"%lld",sec);
    snprintf(header_nonce,64,"%lld",sec);
    /*
    //hmac_sha1
    snprintf( query_string,1024,"oauth_consumer_key=qah4ku73k3qmigj&oauth_nonce=%s&oauth_signature_method=HMAC-SHA1&oauth_timestamp=%s&oauth_version=1.0",header_nonce,header_timestamp);
    //snprintf( query_string,1024,"POST&https://api.google.com/1/oauth/request_token&oauth_consumer_key",header_nonce,header_signature_method,header_timestamp);
    incode_string = oauth_url_escape(query_string);
    if(NULL == incode_string)
    {
        //handle_error(S_URL_ESCAPE_FAIL,"makeAuthorize");
        return NULL;
    }
    char *httpsUrl="https://api.google.com/1/oauth/request_token";
    char *test_key = NULL;
    test_key=(char *)malloc(sizeof(char *)*(strlen(query_string)+4+4+strlen(httpsUrl)));
    memset(test_key,'\0',sizeof(test_key));
    sprintf(test_key,"POST&%s&%s",httpsUrl,incode_string);
    sha1_string = oauth_sign_hmac_sha1(test_key,prekey);

    if(NULL == sha1_string)
    {
        //handle_error(S_SHA1_FAIL,"makeAuthorize");
        //my_free(incode_string);
        return NULL;
    }

    header_signature = oauth_url_escape(sha1_string);
    if(NULL == header_signature)
    {
       // handle_error(S_URL_ESCAPE_FAIL,"makeAuthorize");
       // my_free(incode_string);
        //my_free(sha1_string);
        return NULL;
    }
*/
    header_signature=oauth_sign_plaintext(NULL,prekey);
    //snprintf(header,1024,"Authorization:signature_method=\"%s\",timestamp=\"%s\",nonce=%s,signature=\"%s\"",header_signature_method,header_timestamp,header_nonce,header_signature);
    if(flag==1)
        snprintf(header,1024,"Authorization:OAuth oauth_consumer_key=\"%s\",oauth_signature_method=\"%s\",oauth_signature=\"%s\",oauth_timestamp=\"%s\",oauth_nonce=\"%s\",oauth_version=\"1.0\""
                 ,"qah4ku73k3qmigj",header_signature_method,header_signature,header_timestamp,header_nonce);
    else if(flag==2)
//        snprintf(header,1024,"Authorization:OAuth oauth_consumer_key=\"%s\",oauth_token=\"%s\",oauth_signature_method=\"%s\",oauth_signature=\"%s\",oauth_timestamp=\"%s\",oauth_nonce=\"%s\",oauth_version=\"1.0\""
 //                ,"qah4ku73k3qmigj","AIZpfQv7qYwuDotM",header_signature_method,header_signature,header_timestamp,header_nonce);
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
    //snprintf(header,1024,"oauth_consumer_key=%s&oauth_token=%s&oauth_signature_method=%s&oauth_signature=%s&oauth_timestamp=%s&oauth_nonce=%s&oauth_version=1.0"
    //,"qah4ku73k3qmigj",auth->oauth_token,header_signature_method,header_signature,header_timestamp,header_nonce);
    //snprintf(header,1024,"oauth_consumer_key=%s&oauth_token=%s&oauth_signature_method=%s&oauth_signature=%s&oauth_timestamp=%s&oauth_nonce=%s&oauth_version=1.0"
    //,"qah4ku73k3qmigj","8m9knhps2zgaon9",header_signature_method,header_signature,header_timestamp,header_nonce);

    // my_free(incode_string);
    //my_free(sha1_string);
    free(header_signature);
    //free(test_key);
    return header;
}
#endif
Browse *browseFolder(char *parentref, char *URL, int index){
    printf("browseFolder start!\n");
    int status;
    int i=0;
    //DEBUG("11587 line\n");
    Browse *browse = getb(Browse);
    //DEBUG("11589 line\n");
    if( NULL == browse )
    {
        DEBUG("create memery error\n");
        exit(-1);
    }
    //DEBUG("11595 line\n");
    memset(browse,0,sizeof(Browse));
    //DEBUG("11597 line\n");
    TreeFolderList = (CloudFile *)malloc(sizeof(CloudFile));
    //DEBUG("11599 line\n");
    memset(TreeFolderList,0,sizeof(CloudFile));
    //DEBUG("11601 line\n");
    TreeFileList = (CloudFile *)malloc(sizeof(CloudFile));
    //DEBUG("11603 line\n");
    memset(TreeFileList,0,sizeof(CloudFile));
    DEBUG("11605 line\n");
    TreeFolderList->href = NULL;
    TreeFileList->href = NULL;
    DEBUG("11608 line\n");
    TreeFolderTail = TreeFolderList;
    DEBUG("11610 line\n");
    TreeFileTail = TreeFileList;
    DEBUG("11612 line\n");
    TreeFolderTail->next = NULL;
    TreeFileTail->next = NULL;
    DEBUG("11615 line\n");
    DEBUG("parentref = %s,URL = %s\n",parentref, URL);
    status = api_metadata(parentref, URL,dofile, index);
    //DEBUG("status! = %d\n", status);
    if(status != 0)
    {
        free_CloudFile_item(TreeFolderList);
        free_CloudFile_item(TreeFileList);
        TreeFolderList = NULL;
        TreeFileList = NULL;
        free(browse);
        printf("get Cloud Info ERROR! \n");
        return NULL;
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
    DEBUG("browseFolder reeturn\n");
    return browse;
}


void deal_big_low_conflcit(char *server_conflict_name,char *oldname,char *newname,char *newname_r,int index)
{
    /*get conflict name*/

    char *local_conflcit_name = serverpath_to_localpath(server_conflict_name,index);
//    char *tmp_name = malloc(strlen(local_conflcit_name)+1);
//    memset(tmp_name,0,strlen(local_conflcit_name)+1);
//    sprintf(tmp_name,"%s",local_conflcit_name);
    char *g_newname = NULL;
    int is_folder = test_if_dir(local_conflcit_name);

    g_newname = get_confilicted_name_case(local_conflcit_name,is_folder);

    DEBUG("case-conflict=%s\n",g_newname);
    my_free(local_conflcit_name);


//    char *prefix_name = get_prefix_name(tmp_name,is_folder);//a.txt --> a

//    g_newname = get_confilicted_name_first(tmp_name,is_folder);//a.txt-->a(case-conflict).txt

//    if(access(g_newname,F_OK) == 0)//a(case-conflict).txt-->a(case-conflict(n)).txt
//    {
//        my_free(tmp_name);
//        tmp_name = malloc(strlen(g_newname)+1);
//        memset(tmp_name,0,strlen(g_newname)+1);
//        sprintf(tmp_name,"%s",g_newname);
//        my_free(g_newname);
//        while(!exit_loop)
//        {
//            g_newname = get_confilicted_name_second(tmp_name,is_folder,prefix_name);
//            //printf("confilicted_name=%s\n",confilicted_name);
//            if(access(g_newname,F_OK) == 0)
//            {
//                my_free(tmp_name);
//                tmp_name = malloc(strlen(g_newname)+1);
//                memset(tmp_name,0,strlen(g_newname)+1);
//                sprintf(tmp_name,"%s",g_newname);
//                my_free(g_newname);
//                //have_same = 1;
//            }
//            else
//                break;
//        }
//        my_free(tmp_name);
//    }else
//    {
//        my_free(tmp_name);
//    }
//    my_free(prefix_name);

//    while(!exit_loop)
//    {
//        g_newname = get_confilicted_name(tmp_name,is_folder);
//        //printf("confilicted_name=%s\n",confilicted_name);
//        if(access(g_newname,F_OK) == 0)
//        {
//            my_free(tmp_name);
//            tmp_name = malloc(strlen(g_newname)+1);
//            memset(tmp_name,0,strlen(g_newname)+1);
//            sprintf(tmp_name,"%s",g_newname);
//            my_free(g_newname);
//            //have_same = 1;
//        }
//        else
//            break;
//    }
//    my_free(tmp_name);

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
        DEBUG("api_move9!\n");
        status = s_move(oldname,s_newname,index,1,NULL,6);
    }while(status != 0 && !exit_loop);
    my_free(s_newname);
}
