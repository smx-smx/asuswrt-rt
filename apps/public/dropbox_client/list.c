#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#include <unistd.h>
#include"data.h"

extern int exit_loop;
void replace_char_in_str(char *str,char newchar,char oldchar){

    int i;
    int len;
    len = strlen(str);
    for(i=0;i<len;i++)
    {
        if(str[i] == oldchar)
        {
            str[i] = newchar;
        }
    }

}

/*server tree root function*/
Server_TreeNode *create_server_treeroot()
{
    Server_TreeNode *TreeRoot = NULL;
    TreeRoot = (Server_TreeNode *)malloc(sizeof (Server_TreeNode));
    memset(TreeRoot,0,sizeof(Server_TreeNode));
    if(TreeRoot == NULL)
    {
        wd_DEBUG("create memory error!\n");
        exit(-1);
    }
    TreeRoot->level=0;
    TreeRoot->NextBrother = NULL;
    TreeRoot->parenthref = NULL;
    TreeRoot->browse = NULL;
    TreeRoot->Child = NULL;

    return TreeRoot;
}

int browse_to_tree(char *parenthref,Server_TreeNode *node)
{
    Browse *br = NULL;
    int fail_flag = 0;

    Server_TreeNode *tempnode = NULL, *p1 = NULL,*p2 = NULL;
    tempnode = create_server_treeroot();
    tempnode->level = node->level + 1;

    tempnode->parenthref = my_str_malloc((size_t)(strlen(parenthref)+1));

    br = browseFolder(parenthref);
    snprintf(tempnode->parenthref, sizeof(char)*(strlen(parenthref)+1), "%s",parenthref);

    if(NULL == br)
    {
        free_server_tree(tempnode);
        return -1;
    }

    tempnode->browse = br;

    if(node->Child == NULL)
    {
        node->Child = tempnode;
    }
    else
    {
        p2 = node->Child;
        p1 = p2->NextBrother;

        while(p1 != NULL)
        {
            p2 = p1;
            p1 = p1->NextBrother;
        }

        p2->NextBrother = tempnode;
        tempnode->NextBrother = NULL;
    }

    CloudFile *de_foldercurrent;
    de_foldercurrent = br->folderlist->next;
    while(de_foldercurrent != NULL && !exit_loop)
    {
        if(browse_to_tree(de_foldercurrent->href,tempnode) == -1)
        {
            fail_flag = 1;
            break;
        }
        de_foldercurrent = de_foldercurrent->next;
    }

    if(exit_loop)
        fail_flag = 1;

    return (fail_flag == 1) ? -1 : 0 ;

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
        }
        if(p->name != NULL)
        {
            free(p->name);
        }
        free(p);
        p = head;
    }
}
void free_server_tree(Server_TreeNode *node)
{
    if(node != NULL)
    {
        if(node->NextBrother != NULL)
            free_server_tree(node->NextBrother);
        if(node->Child != NULL)
            free_server_tree(node->Child);
        if(node->parenthref != NULL)
        {
            free(node->parenthref);
        }
        if(node->browse != NULL)
        {
            free_CloudFile_item(node->browse->filelist);
            free_CloudFile_item(node->browse->folderlist);
            free(node->browse);
        }
        free(node);
    }
}

void SearchServerTree(Server_TreeNode* treeRoot)
{
    int i;
    for(i=0;i<treeRoot->level;i++)
        printf("-");

    if(treeRoot->browse != NULL)
    {

        CloudFile *de_foldercurrent,*de_filecurrent;
        de_foldercurrent = treeRoot->browse->folderlist->next;
        de_filecurrent = treeRoot->browse->filelist->next;
        while(de_foldercurrent != NULL){
            printf("serverfolder->href = %s\n",de_foldercurrent->href);
            de_foldercurrent = de_foldercurrent->next;
        }
        while(de_filecurrent != NULL){
            printf("serverfile->href = %s,serverfile->mtime=%ld\n",de_filecurrent->href,de_filecurrent->mtime);
            de_filecurrent = de_filecurrent->next;
        }
    }
    else
    {
        printf("treeRoot->browse is null\n");

    }

    if((treeRoot->Child!=NULL))
        SearchServerTree(treeRoot->Child);

    if(treeRoot->NextBrother != NULL)
        SearchServerTree(treeRoot->NextBrother);
}

int create_sync_list()
{

#if MEM_POOL_ENABLE
        mem_pool_init();
        printf("######eable mem  pool######\n");
#endif

    local_sync = 0;
    server_sync = 1;
    finished_initial = 0;
    LOCAL_FILE.path = NULL;
    int i;
    int num=asus_cfg.dir_number;
    g_pSyncList = (sync_list **)malloc(sizeof(sync_list *)*num);
    memset(g_pSyncList,0,sizeof(sync_list *)*num);
#if TOKENFILE
    sighandler_finished = 0;
#endif
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        g_pSyncList[i] = (sync_list *)malloc(sizeof(sync_list));
        memset(g_pSyncList[i],0,sizeof(sync_list));

        g_pSyncList[i]->receve_socket = 0;
        g_pSyncList[i]->have_local_socket = 0;
        g_pSyncList[i]->first_sync = 1;
        g_pSyncList[i]->no_local_root = 0;
        g_pSyncList[i]->init_completed = 0;
#if TOKENFILE
        g_pSyncList[i]->sync_disk_exist = 0;
#endif

        g_pSyncList[i]->ServerRootNode = NULL;
        g_pSyncList[i]->OldServerRootNode = NULL;
        g_pSyncList[i]->VeryOldServerRootNode = NULL;
        g_pSyncList[i]->SocketActionList = queue_create();
        g_pSyncList[i]->unfinished_list = create_action_item_head();
        g_pSyncList[i]->up_space_not_enough_list = create_action_item_head();
        g_pSyncList[i]->server_action_list = create_action_item_head();
        g_pSyncList[i]->copy_file_list = create_action_item_head();
        g_pSyncList[i]->access_failed_list = create_action_item_head();
        if(asus_cfg.prule[i]->rule == 1)
        {
            g_pSyncList[i]->download_only_socket_head = create_action_item_head();
        }
        else
        {
            g_pSyncList[i]->download_only_socket_head = NULL;
        }

#if TOKENFILE
        tokenfile_info_tmp = tokenfile_info_start->next;
        while(tokenfile_info_tmp != NULL)
        {
            if(!strcmp(tokenfile_info_tmp->folder,asus_cfg.prule[i]->rooturl) && !strcmp(tokenfile_info_tmp->url,asus_cfg.user))
            {
                g_pSyncList[i]->sync_disk_exist = 1;
                /*
                 fix below bug:
                    rm sync dir,change the sync dir,can not create sync dir
                */
                if(access(asus_cfg.prule[i]->path,F_OK))
                {
                    my_mkdir_r(asus_cfg.prule[i]->path);
                }
                break;
            }
            tokenfile_info_tmp = tokenfile_info_tmp->next;
        }
#endif
    }
}
int initMyLocalFolder(Server_TreeNode *servertreenode,int index)
{
    int res=0;
    if(servertreenode->browse != NULL)
    {
        CloudFile *init_folder=NULL,*init_file=NULL;
        if(servertreenode->browse->foldernumber > 0)
            init_folder=servertreenode->browse->folderlist->next;
        if(servertreenode->browse->filenumber > 0)
            init_file=servertreenode->browse->filelist->next;
        int ret;
        while(init_folder != NULL  && !exit_loop)
        {
            char *createpath;
            createpath = serverpath_to_localpath(init_folder->href,index);
            if(NULL == opendir(createpath))
            {

                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                if(-1 == mkdir(createpath,0777))
                {
                    wd_DEBUG("mkdir %s fail",createpath);
                    return -1;
                }
                else
                {
                    add_action_item("createfolder",createpath,g_pSyncList[index]->server_action_list);
                }
            }
            free(createpath);
            init_folder = init_folder->next;
        }
        while(init_file != NULL && !exit_loop)
        {
            if(is_local_space_enough(init_file,index))
            {
                char *createpath;
                createpath = serverpath_to_localpath(init_file->href,index);
                add_action_item("createfile",createpath,g_pSyncList[index]->server_action_list);
                ret=api_download(createpath,init_file->href,index);
                if(ret == 0)
                {
                    ChangeFile_modtime(createpath,init_file->mtime);
                }
                else
                    return ret;
                free(createpath);
            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                add_action_item("download",init_file->href,g_pSyncList[index]->unfinished_list);
            }
            init_file = init_file->next;
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

    return res;

}
/*changed from serverpath to localpath*/
char *serverpath_to_localpath(char *from_serverpath,int index){
    char *to_localpath;

#ifdef MULTI_PATH
    to_localpath = (char *)malloc(sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+2));
    memset(to_localpath,'\0',sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+2));

    snprintf(to_localpath, sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+2), "%s%s",asus_cfg.prule[index]->base_path,from_serverpath);
#else
    to_localpath = (char *)malloc(sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2));
    memset(to_localpath,'\0',sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2));

    snprintf(to_localpath, sizeof(char)*(strlen(from_serverpath)+asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len+2), "%s%s%s",asus_cfg.prule[index]->base_path,asus_cfg.prule[index]->rooturl,from_serverpath);
#endif
    wd_DEBUG("serverpath_to_localpath to_localpath = %s\n",to_localpath);

    return to_localpath;
}

char *localpath_to_serverpath(char *from_localpath,int index)
{
    char *to_serverpath;
    char *hreftmp;
#ifdef MULTI_PATH
    hreftmp=strstr(from_localpath,asus_cfg.prule[index]->base_path)+asus_cfg.prule[index]->base_path_len;
#else
    hreftmp=strstr(from_localpath,asus_cfg.prule[index]->base_path)+asus_cfg.prule[index]->base_path_len+asus_cfg.prule[index]->rooturl_len;
#endif
    to_serverpath=(char *)malloc(sizeof(char)*(strlen(hreftmp)+2));
    memset(to_serverpath,0,sizeof(char)*(strlen(hreftmp)+2));
    snprintf(to_serverpath, sizeof(char)*(strlen(hreftmp)+2), "%s",hreftmp);
    wd_DEBUG("from_localpath = %s\n",from_localpath);
    wd_DEBUG("Localpath_to_serverpath to_serverpath = %s\n",to_serverpath);

    return to_serverpath;
}

int is_local_space_enough(CloudFile *do_file,int index){

    long long int freespace;
    freespace = get_local_freespace(index);

    wd_DEBUG("freespace = %lld,do_file->size = %lld,do_file->href = %s\n",freespace,do_file->size,do_file->href);

    if(freespace <= do_file->size){

        wd_DEBUG("local freespace is not enough!\n");

        return 0;
    }
    else
    {

        wd_DEBUG("local freespace is enough!\n");

        return 1;
    }
}
long long int get_local_freespace(int index){
    /*************unit is B************/

    wd_DEBUG("***********get %s freespace!***********\n",asus_cfg.prule[index]->base_path);

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
int ChangeFile_modtime(char *filepath,time_t servermodtime){

    wd_DEBUG("**************ChangeFile_modtime**********\n");

    if(access(filepath,F_OK) != 0)
    {
        return -1;
    }
    struct utimbuf *ub;
    ub = (struct utimbuf *)malloc(sizeof(struct utimbuf));

    wd_DEBUG("servermodtime = %lu\n",servermodtime);

    if(servermodtime == -1){

        wd_DEBUG("ChangeFile_modtime ERROR!\n");

        return -1;
    }

    ub->actime = servermodtime;
    ub->modtime = servermodtime;

    utime(filepath,ub);

    free(ub);
    return 0;
}


/*获取某一文件夹下的所有文件和文件夹信息*/
Local *Find_Floor_Dir(const char *path)
{
    Local *local;
    int filenum;
    int foldernum;
    LocalFile *localfloorfile;
    LocalFolder *localfloorfolder;
    LocalFile *localfloorfiletmp;
    LocalFolder *localfloorfoldertmp;
    LocalFile *localfloorfiletail;
    LocalFolder *localfloorfoldertail;
    DIR *pDir;
    struct dirent *ent = NULL;

    filenum = 0;
    foldernum = 0;
    local = (Local *)malloc(sizeof(Local));
    memset(local,0,sizeof(Local));
    localfloorfile = (LocalFile *)malloc(sizeof(LocalFile));
    localfloorfolder = (LocalFolder *)malloc(sizeof(LocalFolder));
    memset(localfloorfolder,0,sizeof(localfloorfolder));
    memset(localfloorfile,0,sizeof(localfloorfile));

    localfloorfile->path = NULL;
    localfloorfolder->path = NULL;

    localfloorfiletail = localfloorfile;
    localfloorfoldertail = localfloorfolder;
    localfloorfiletail->next = NULL;
    localfloorfoldertail->next = NULL;

    pDir = opendir(path);

    if(NULL == pDir)
    {
        return NULL;
    }

    while(NULL != (ent = readdir(pDir)))
    {
        /*
         fix :accept the begin of '.' files;
        */
        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        if(test_if_download_temp_file(ent->d_name))     //xxx.asus.td filename will not get
            continue;

        char *fullname;
        size_t len;
        len = strlen(path)+strlen(ent->d_name)+2;
        fullname = my_str_malloc(len);
        snprintf(fullname, sizeof(char)*len, "%s/%s",path,ent->d_name);

        if(test_if_dir(fullname) == 1)
        {
            localfloorfoldertmp = (LocalFolder *)malloc(sizeof(LocalFolder));
            memset(localfloorfoldertmp,0,sizeof(localfloorfoldertmp));
            localfloorfoldertmp->path = my_str_malloc((size_t)(strlen(fullname)+1));

            snprintf(localfloorfoldertmp->name, MAX_CONTENT, "%s",ent->d_name);
            snprintf(localfloorfoldertmp->path, sizeof(char)*(strlen(fullname)+1), "%s",fullname);

            ++foldernum;

            localfloorfoldertail->next = localfloorfoldertmp;
            localfloorfoldertail = localfloorfoldertmp;
            localfloorfoldertail->next = NULL;
        }
        else
        {
            struct stat buf;

            if(stat(fullname,&buf) == -1)
            {
                perror("stat:");
                continue;
            }

            localfloorfiletmp = (LocalFile *)malloc(sizeof(LocalFile));
            memset(localfloorfiletmp,0,sizeof(localfloorfiletmp));
            localfloorfiletmp->path = my_str_malloc((size_t)(strlen(fullname)+1));

            unsigned long asec = buf.st_atime;
            unsigned long msec = buf.st_mtime;
            unsigned long csec = buf.st_ctime;

            snprintf(localfloorfiletmp->creationtime, MINSIZE, "%lu",csec);
            snprintf(localfloorfiletmp->lastaccesstime, MINSIZE, "%lu",asec);
            snprintf(localfloorfiletmp->lastwritetime, MINSIZE, "%lu",msec);

            snprintf(localfloorfiletmp->name, MAX_CONTENT, "%s",ent->d_name);
            snprintf(localfloorfiletmp->path, sizeof(char)*(strlen(fullname)+1), "%s",fullname);

            localfloorfiletmp->size = buf.st_size;

            ++filenum;

            localfloorfiletail->next = localfloorfiletmp;
            localfloorfiletail = localfloorfiletmp;
            localfloorfiletail->next = NULL;
        }
        free(fullname);
    }

    local->filelist = localfloorfile;
    local->folderlist = localfloorfolder;

    local->filenumber = filenum;
    local->foldernumber = foldernum;

    closedir(pDir);

    return local;

}
int test_if_dir(const char *dir){
    DIR *dp = opendir(dir);

    if(dp == NULL)
        return 0;

    closedir(dp);
    return 1;
}

/*free保存单层文件夹信息所用的空间*/
void free_localfloor_node(Local *local)
{
    free_LocalFile_item(local->filelist);
    free_LocalFolder_item(local->folderlist);
    free(local);
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
        }
        free(p);
        p = head;
    }

}
int upload_serverlist(Server_TreeNode *treenode,Browse *perform_br, LocalFolder *localfoldertmp,int index){
    wd_DEBUG("upload_serverlist\n");
    CloudFile *foldertmp = NULL,*foldertmp_1=NULL;
    foldertmp=(CloudFile *)malloc(sizeof(struct node));
    memset(foldertmp,0,sizeof(foldertmp));

    foldertmp_1=perform_br->folderlist;
    while(foldertmp_1->next != NULL)
    {

        foldertmp_1=foldertmp_1->next;
    }
    if(foldertmp_1->next == NULL){
        char *serverpath;
        serverpath=localpath_to_serverpath(localfoldertmp->path,index);
        foldertmp->href=(char *)malloc(sizeof(char)*(strlen(serverpath)+1));
        memset(foldertmp->href,'\0',sizeof(char)*(strlen(serverpath)+1));
        snprintf(foldertmp->href, sizeof(char)*(strlen(serverpath)+1), "%s", serverpath);
        foldertmp->size=0;
        foldertmp->name=parse_name_from_path(foldertmp->href);
        foldertmp->isFolder=1;
        foldertmp_1->next=foldertmp;
        foldertmp_1=foldertmp;
        foldertmp->next=NULL;
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
    treenodetmp->level = treenode->level + 1;
    memset(treenodetmp,'\0',sizeof(treenodetmp));
    snprintf(treenodetmp->parenthref, sizeof(char)*(sizeof(char)*(strlen(foldertmp->href)+1)), "%s", foldertmp->href);

    treenodetmp->browse=br;
    if(treenode->Child == NULL)
    {
        treenode->Child = treenodetmp;
    }
    else
    {
        p2 = treenode->Child;
        p1 = p2->NextBrother;

        while(p1 != NULL)
        {
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
CloudFile *get_CloudFile_node(Server_TreeNode* treeRoot,const char *dofile_href,int a){

    int href_len = strlen(dofile_href);
    CloudFile *finded_file = NULL;
    if(treeRoot == NULL)
    {
        return NULL;
    }
    if(treeRoot->browse != NULL)
    {
        int int_folder = 0x1;
        int int_file = 0x2;
        CloudFile *de_foldercurrent = NULL;
        CloudFile *de_filecurrent = NULL;
        if(treeRoot->browse->foldernumber > 0)
            de_foldercurrent = treeRoot->browse->folderlist->next;
        if(treeRoot->browse->filenumber > 0)
            de_filecurrent = treeRoot->browse->filelist->next;
        if((a&int_folder) && de_foldercurrent != NULL)
        {
            while(de_foldercurrent != NULL)
            {
                if(de_foldercurrent->href != NULL)
                {
                    if(!(strncmp(de_foldercurrent->href,dofile_href,href_len)))
                    {
                        return de_foldercurrent;
                    }
                }
                de_foldercurrent = de_foldercurrent->next;
            }
        }
        if((a&int_file) && de_filecurrent != NULL)
        {
            while(de_filecurrent != NULL)
            {
                if(de_filecurrent->href != NULL)
                {
                    if(!(strncmp(de_filecurrent->href,dofile_href,href_len)))
                    {
                        return de_filecurrent;
                    }
                }
                de_filecurrent = de_filecurrent->next;
            }
        }
    }

    if((treeRoot->Child!=NULL))
    {
        finded_file = get_CloudFile_node(treeRoot->Child,dofile_href,a);
        if(finded_file != NULL)
        {
            return finded_file;
        }
    }


    if(treeRoot->NextBrother != NULL)
    {
        finded_file = get_CloudFile_node(treeRoot->NextBrother,dofile_href,a);
        if(finded_file != NULL)
        {
            return finded_file;
        }
    }
    return finded_file;
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
