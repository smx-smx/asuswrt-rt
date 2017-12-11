#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include "SMB.h"

extern Config smb_config;
extern sync_list **g_pSyncList;
extern pthread_cond_t cond,cond_socket,cond_log;
extern struct tokenfile_info_tag *tokenfile_info_start;

char *my_malloc(size_t len)
{
        char *s;
        s = (char *)malloc(sizeof(char) * len);
        if(s == NULL)
        {
                return NULL;
        }
        memset(s, '\0', sizeof(char) * len);//2014.10.10 by sherry
        return s;
}

action_item *create_action_item_head()
{
        action_item *head;

        head = (action_item *)malloc(sizeof(action_item));
        if(head == NULL)
        {
                return NULL;
        }
        memset(head, '\0', sizeof(action_item));
        head->next = NULL;

        return head;
}

Server_TreeNode *create_server_treeroot()
{
        Server_TreeNode *TreeRoot = NULL;
        TreeRoot = (Server_TreeNode *)malloc(sizeof (Server_TreeNode));
        memset(TreeRoot, 0, sizeof(Server_TreeNode));
        if(TreeRoot == NULL)
        {
                return NULL;
        }
        TreeRoot->NextBrother = NULL;
        TreeRoot->parenthref = NULL;
        TreeRoot->browse = NULL;
        TreeRoot->Child = NULL;
        return TreeRoot;
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

char *serverpath_to_localpath(char *serverpath, int index)//2014.10.08 by sherry ，未考虑在根目录下进行操作的情况
{
        char *p = serverpath;
        char *localpath;

        if(!strcmp(serverpath,smb_config.multrule[index]->server_root_path))
        {//在根目录下进行操作
            localpath = my_malloc(strlen(smb_config.multrule[index]->client_root_path) + 1);
            if(localpath==NULL)//2014.10.17 by sherry malloc申请内存是否成功
            {
               return NULL;
            }
            snprintf(localpath, sizeof(char)*(strlen(smb_config.multrule[index]->client_root_path) + 1), "%s",smb_config.multrule[index]->client_root_path);
            return localpath;
        }
        else
        {//深目录的情况
            localpath = my_malloc(strlen(serverpath) - strlen(smb_config.multrule[index]->server_root_path)
                                        + strlen(smb_config.multrule[index]->client_root_path) + 1);
            //2014.10.17 by sherry malloc申请内存是否成功
            if(localpath==NULL)
            {
               return NULL;
            }
            p = p + strlen(smb_config.multrule[index]->server_root_path);
            snprintf(localpath, sizeof(char)*(strlen(serverpath) - strlen(smb_config.multrule[index]->server_root_path)
                                        + strlen(smb_config.multrule[index]->client_root_path) + 1),  "%s%s", smb_config.multrule[index]->client_root_path, p);
            return localpath;
       }

}

char *localpath_to_serverpath(char *localpath, int index)//2014.10.08 by sherry ，未考虑在根目录下进行操作的情况
{
        char *p = localpath;
        char *serverpath;

        if(!strcmp(localpath,smb_config.multrule[index]->client_root_path))
        {//在根目录下进行操作
            serverpath = my_malloc(strlen(smb_config.multrule[index]->server_root_path) + 3);
            //2014.10.11 by sherry 删除操作 SMB_del()中 使用了strcat导致内存不足 所以：“+3”
            //2014.10.17 by sherry malloc申请内存是否成功
            if(serverpath==NULL)
            {
                return NULL;
            }
            snprintf(serverpath, sizeof(char)*(strlen(smb_config.multrule[index]->server_root_path) + 3), "%s", smb_config.multrule[index]->server_root_path);
            return serverpath;

        }
        else
        {//深目录的情况
            serverpath = my_malloc(strlen(localpath) - strlen(smb_config.multrule[index]->client_root_path)
                                         + strlen(smb_config.multrule[index]->server_root_path) + 3);
            //2014.10.17 by sherry malloc申请内存是否成功
            if(serverpath==NULL)
            {
                return NULL;
            }
            p = p + strlen(smb_config.multrule[index]->client_root_path);
            snprintf(serverpath, sizeof(char)*(strlen(localpath) - strlen(smb_config.multrule[index]->client_root_path)
                                              + strlen(smb_config.multrule[index]->server_root_path) + 3), "%s%s", smb_config.multrule[index]->server_root_path, p);
            return serverpath;

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

void free_CloudFile_item(CloudFile *head)
{
        CloudFile *p = head;
        while(p != NULL)
        {
                head = head->next;
                if(p->href != NULL)
                {
                        free(p->href);
                        free(p->filename);
                }
                free(p);
                p = head;
        }
}

long long int get_local_freespace(int index)
{
        long long int freespace = 0;
        struct statvfs diskdata;
        if(!statvfs(smb_config.multrule[index]->client_root_path, &diskdata))
        {
                freespace = (long long)diskdata.f_bsize * (long long)diskdata.f_bavail;
                return freespace;
        }
        else
        {
                return 0;
        }
}

int is_local_space_enough(CloudFile *do_file, int index)
{
        long long int freespace;
        freespace = get_local_freespace(index);
        if(freespace <= do_file->size)
                return 0;
        else
                return 1;
}

int add_action_item(const char *action, const char *path, action_item *head)
{
        DEBUG("add_action_item, action = %s, path = %s\n", action, path);

        action_item *p1, *p2;
        p1 = head;
        p2 = (action_item *)malloc(sizeof(action_item));
        memset(p2, '\0', sizeof(action_item));

        p2->action = my_malloc(strlen(action) + 1);
        p2->path =   my_malloc(strlen(path) + 1);
        //2014.10.17 by sherry malloc申请内存是否成功
        if(p2->action==NULL||p2->path==NULL)
        {
            return -1;
        }
        snprintf(p2->action, sizeof(char)*(strlen(action) + 1), "%s", action);
        snprintf(p2->path, sizeof(char)*(strlen(path) + 1),  "%s", path);

        while(p1->next != NULL)
                p1 = p1->next;

        p1->next = p2;
        p2->next = NULL;
        DEBUG("add action item OK!\n");
        return 0;
}

int test_if_download_temp_file(char *filename)
{
        char file_suffix[9];
        char *temp_suffix = my_malloc(9);
        //2014.10.17 by sherry malloc申请内存是否成功
        if(temp_suffix==NULL)
        {
            return -1;
        }
        snprintf(temp_suffix, sizeof(char)*9, "%s", ".asus.td");

        memset(file_suffix, 0, sizeof(file_suffix));
        char *p = filename;
        if(strstr(filename, temp_suffix))
        {
            snprintf(file_suffix, 9, "%s", p + (strlen(filename) - strlen(temp_suffix)));
                if(!strcmp(file_suffix, temp_suffix))
                {
                        free(temp_suffix);
                        return 1;
                }
        }
        free(temp_suffix);
        return 0;
}

int test_if_dir(const char *dir)
{
        DIR *dp = opendir(dir);
        if(dp == NULL)
                return 0;
        closedir(dp);
        return 1;
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

/*free保存单层文件夹信息所用的空间*/
void free_localfloor_node(Local *local)
{
        free_LocalFile_item(local->filelist);
        free_LocalFolder_item(local->folderlist);
        free(local);
}

int is_server_have_localpath(char *path, Server_TreeNode *treenode, int index)
{
        if(treenode == NULL)
                return 0;

        char *localpath;
        int ret = 0;
        int cmp = 1;

        localpath = serverpath_to_localpath(treenode->parenthref, index);

        if((cmp = strcmp(localpath, path)) == 0)
        {
                ret = 1;
                free(localpath);
                return ret;
        }
        else
        {
                free(localpath);
        }

        if(treenode->Child != NULL)
        {
                ret = is_server_have_localpath(path, treenode->Child, index);
                if(ret == 1)
                {
                        return ret;
                }
        }
        if(treenode->NextBrother != NULL)
        {
                ret = is_server_have_localpath(path,treenode->NextBrother, index);
                if(ret == 1)
                {
                        return ret;
                }
        }

        return ret;
}

action_item *get_action_item(const char *action,const char *path,action_item *head,int index)
{
        action_item *p;
        p = head->next;
        while(p != NULL)
        {
                if(smb_config.multrule[index]->rule == 1)
                {
                        if(!strcmp(p->path, path))
                        {
                                return p;
                        }
                }
                else
                {
                        if(!strcmp(p->action, action) && !strcmp(p->path, path))
                        {
                                return p;
                        }
                }
                p = p->next;
        }
        return NULL;
}

int del_action_item(const char *action,const char *path,action_item *head)
{
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

void local_mkdir(char *path)
{
        DIR *dir;
        if(NULL == (dir = opendir(path)))
        {
                if(-1 == mkdir(path, 0777))//创建目录，0777默认模式 最大可能访问权
                {
                        return;
                }
        }
        else
                closedir(dir);
}

void free_action_item(action_item *head)
{
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

Node *queue_dequeue (Node *q)
{
        Node *first = q->next;
        Node *newfirst = q->next->next;
        q->next = newfirst;
        first->next = NULL;
        return first;
}

#ifdef OLEG_ARM
int detect_process(char * process_name)
{
    DEBUG("@@@@@OLEG_ARM\n");
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
    DEBUG("@@@@@I686\n");
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
int detect_process(char *process_name)
{
        FILE *ptr;
        char buff[512] = {0};
        char ps[128] = {0};
        snprintf(ps, 128, "ps | grep -c %s", process_name);
        snprintf(buff, 512, "%s", "ABNORMAL");
        if((ptr = popen(ps, "r")) != NULL)
        {
                while (fgets(buff, 512, ptr) != NULL)
                {
                        if(atoi(buff) > 2)
                        {
                                pclose(ptr);
                                return 1;
                        }
                }
        }
        if(strcmp(buff, "ABNORMAL") == 0)  /*ps command error*/
        {
                return 0;
        }
        pclose(ptr);
        return 0;
}
#endif
#endif


char *my_nstrchr(const char chr,char *str,int n){

        if(n<1)
        {
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
        }while(p2!=NULL && i <= n);

        if(i<n)
        {
                return NULL;
        }

        return p2;
}

int get_path_to_index(char *path)
{
        int i;
        char *root_path = NULL;
        char *temp = NULL;
        root_path = my_malloc(512);
        //2014.10.17 by sherry malloc申请内存是否成功
        if(root_path==NULL)
        {
            return -1;
        }
        temp = my_nstrchr('/', path, 5);
        if(temp == NULL)
        {
                snprintf(root_path, sizeof(char)*512, "%s", path);
        }
        else
        {
                snprintf(root_path, strlen(path) - strlen(temp) + 1, "%s", path);
        }

        for(i = 0; i < smb_config.dir_num; i++)
        {
                if(!strcmp(root_path, smb_config.multrule[i]->client_root_path))
                        break;
        }
        free(root_path);

        return i;
}

char *get_socket_base_path(char *cmd)
{
        char *temp = NULL;
        char *temp1 = NULL;
        char path[1024] = {0};
        char *root_path = NULL;

        if(!strncmp(cmd, "rmroot", 6))
        {
                temp = strchr(cmd, '/');
                root_path = my_malloc(128);
                //2014.10.17 by sherry malloc申请内存是否成功
                if(root_path==NULL)
                    return NULL;
                snprintf(root_path, sizeof(char)*128, "%s", temp);
        }
        else
        {
                temp = strchr(cmd, '/');
                temp1 = strchr(temp, '\n');
                strncpy(path, temp, strlen(temp) - strlen(temp1));
                root_path = my_malloc(128);
                //2014.10.17 by sherry malloc申请内存是否成功
                if(root_path==NULL)
                    return NULL;

                temp = my_nstrchr('/', path, 5);
                if(temp == NULL)
                {
                    snprintf(root_path, sizeof(char)*128, "%s", path);
                }
                else
                {
                        snprintf(root_path, strlen(path) - strlen(temp) + 1, "%s", path);
                }
        }
        return root_path;
}

void del_download_only_action_item(const char *action,const char *path,action_item *head)
{
        action_item *p1, *p2;
        char *cmp_name;
        char *p1_cmp_name;
        p1 = head->next;
        p2 = head;

        cmp_name = my_malloc((size_t)(strlen(path)+2));
        //2014.10.17 by sherry malloc申请内存是否成功
        if(cmp_name==NULL)
            return;
        snprintf(cmp_name, sizeof(char)*(strlen(path)+2), "%s/",path);    //add for delete folder and subfolder in download only socket list

        while(p1 != NULL)
        {
                p1_cmp_name = my_malloc((size_t)(strlen(p1->path)+2));
                //2014.10.17 by sherry malloc申请内存是否成功
                if(p1_cmp_name==NULL)
                    return;
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

                fullname = my_malloc((size_t)(strlen(dir)+strlen(ent->d_name)+2));
                //2014.10.17 by sherry malloc申请内存是否成功
                if(fullname==NULL)
                    return -1;


                snprintf(fullname, sizeof(char)*(strlen(dir)+strlen(ent->d_name)+2), "%s/%s",dir,ent->d_name);

                if( test_if_dir(fullname) == 1)
                {
                        add_action_item("createfolder", fullname, g_pSyncList[index]->dragfolder_action_list);
                        add_action_item("createfolder", fullname, g_pSyncList[index]->download_only_socket_head);
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
                        fullname = my_malloc(len);
                        //2014.10.17 by sherry malloc申请内存是否成功
                        if(fullname==NULL)
                            return;
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
                DEBUG("open %s fail \n",dir);
}

/*
 *if a = 0x1,find in folderlist
 *if a = 0x2,find in filelist
 *if a = 0x3,find in folderlist and filelist
*/
CloudFile *get_CloudFile_node(Server_TreeNode* treeRoot, const char *dofile_href, int a, int index)
{
        DEBUG("****get_CloudFile_node****dofile_href = %s\n",dofile_href);
        int href_len = strlen(dofile_href);
        CloudFile *finded_file = NULL;
        if(treeRoot == NULL)
        {
                return NULL;
        }

        if((treeRoot->Child!=NULL))
        {

                finded_file = get_CloudFile_node(treeRoot->Child,dofile_href,a,index);
                if(finded_file != NULL)
                {
                        return finded_file;
                }
        }

        if(treeRoot->NextBrother != NULL)
        {
                finded_file = get_CloudFile_node(treeRoot->NextBrother,dofile_href,a,index);
                if(finded_file != NULL)
                {
                        return finded_file;
                }
        }

        if(treeRoot->browse != NULL)
        {
                int int_folder = 0x1;
                int int_file = 0x2;
                CloudFile *de_foldercurrent = NULL;
                CloudFile *de_filecurrent = NULL;
                DEBUG("111111folder = %d,file = %d\n",treeRoot->browse->foldernumber,treeRoot->browse->filenumber);
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
                                        DEBUG("de_foldercurrent->href = %s\n",de_foldercurrent->href);
                                        if(!(strncmp(de_foldercurrent->href,dofile_href,href_len)))
                                        { 
                                                return de_foldercurrent;
                                        }
                                }

                                de_foldercurrent = de_foldercurrent->next;
                        }
                }

                //2014.10.15 by sherry
                if((a&int_file) && de_filecurrent != NULL)
                {
                        while(de_filecurrent != NULL)
                        {
                                if(de_filecurrent->href != NULL)
                                {
                                        char *filecurrent_href = NULL;
                                        filecurrent_href = my_malloc(strlen(de_filecurrent->href) + strlen(smb_config.multrule[index]->server_root_path) + 1);
                                        //2014.10.20 by sherry malloc申请内存是否成功
                                        if(filecurrent_href==NULL)
                                        {
                                            return NULL;
                                        }
                                        snprintf(filecurrent_href, sizeof(char)*(strlen(de_filecurrent->href) + strlen(smb_config.multrule[index]->server_root_path) + 1), "%s%s", smb_config.multrule[index]->server_root_path, de_filecurrent->href);
                                        if(!(strncmp(filecurrent_href,dofile_href,href_len)))
                                        {
                                                DEBUG("get it\n");
                                                free(filecurrent_href);
                                                return de_filecurrent;
                                        }
                                        free(filecurrent_href);
                                }
                                de_filecurrent = de_filecurrent->next;
                        }
                }
        }
        return finded_file;
}

char *write_error_message(char *format,...)
{
        char *p = my_malloc(256);
        //2014.10.20 by sherry malloc申请内存是否成功
        if(p==NULL)
            return NULL;
        memset(p, 0, sizeof(p));
        va_list ap;
        va_start(ap, format);
        vsnprintf(p, 256, format, ap);
        va_end(ap);
        return p;
}

int write_conflict_log(char *fullname, int type, char *msg)
{
        FILE *fp = 0;
        char ctype[16] = {0};

        if(type == 1)
            snprintf(ctype, 16, "%s", "Error");
        else if(type == 2)
            snprintf(ctype, 16, "%s", "Info");
        else if(type == 3)
            snprintf(ctype, 16, "%s", "Warning");


        if(access(CONFLICT_DIR, 0) == 0)
                fp = fopen(CONFLICT_DIR, "a");
        else
                fp = fopen(CONFLICT_DIR, "w");


        if(fp == NULL)
        {
                return -1;
        }
        fclose(fp);
        return 0;
}

char *insert_suffix(char *name, char *suffix)
{
        char *new_name, *name1;
        new_name = my_malloc(strlen(name) + strlen(suffix) + 1);
        //2014.10.20 by sherry malloc申请内存是否成功
        if(new_name==NULL)
            return NULL;

        //2014.10.13 by sherry
        char *p;
        p=strrchr(name,'.');

        if(p==NULL){
            snprintf(new_name, sizeof(char)*(strlen(name) + strlen(suffix) + 1), "%s%s", name, suffix);
        }else{
                name1 = my_malloc(strlen(name) - strlen(p) + 1);
                //2014.10.20 by sherry malloc申请内存是否成功
                if(name1==NULL)
                    return NULL;
                snprintf(name1, strlen(name) - strlen(p) + 1, "%s", name);
                snprintf(new_name, sizeof(char)*(strlen(name) + strlen(suffix) + 1), "%s%s%s", name1, suffix, p);
                free(name1);
        }
        return new_name;
}

/** 0:server
  * 1:local
 */
char *change_same_name(char *localpath, int index, int flag)
{
        int i = 1;
        char *filename = NULL;
        char *new_path = NULL;
        char *path = NULL;
        char temp[256] = {0};
        char suffix[6] = {0};

        //2014.10.13 by sherry
        char *p;
        p=strrchr(localpath,'/');
        if(p==NULL)
        {
            return NULL;
        }
        filename = my_malloc(strlen(p) + 1);
        //2014.10.20 by sherry malloc申请内存是否成功
        if(filename==NULL)
            return NULL;
        snprintf(filename, sizeof(char)*(strlen(p) + 1), "%s", p + 1);
        path = my_malloc(strlen(localpath) - strlen(p) + 1);
        //2014.10.20 by sherry malloc申请内存是否成功
        if(path==NULL)
            return NULL;
        snprintf(path, strlen(localpath) - strlen(p) + 1, "%s", localpath);
        int exit = 1;
        while(exit)
        {
                int n = i;
                int j = 0;
                while((n = (n / 10)))
                {
                        j++;
                }
                memset(temp, '\0', sizeof(temp));
                snprintf(temp, 252 - j, "%s", filename);
                snprintf(suffix, 6, "(%d)", i);
                char *new_name = insert_suffix(temp, suffix);
                if(new_name==NULL)
                    return NULL;
                i++;

                new_path = my_malloc(strlen(path) + strlen(new_name) + 2);
                //2014.10.20 by sherry malloc申请内存是否成功
                if(new_path==NULL)
                    return NULL;
                snprintf(new_path, sizeof(char)*(strlen(path) + strlen(new_name) + 2), "%s/%s", path, new_name);

                if(flag == 0)
                        exit = is_server_exist(new_path, index);
                else if(flag == 1){
                        if(access(new_path, F_OK) != 0)
                                exit = 0;
                }

                free(new_name);
        }
        free(path);
        free(filename);
        return new_path;
}

void my_mkdir_r(char *path,int index)
{
        int i, len;
        char str[512];
        char fullname[512];
        char *temp;
        DEBUG("****************my_mkdir_r*******************\n");
        DEBUG("%s\n", path);
        memset(str,0,sizeof(str));

        temp = strstr(path, smb_config.multrule[index]->mount_path);

        len = strlen(smb_config.multrule[index]->mount_path);
        snprintf(str, 512, "%s", temp + len);

        len = strlen(str);
        for(i = 0; i < len; i++)
        {
                if(str[i] == '/' && i != 0)
                {
                        str[i] = '\0';
                        memset(fullname, 0, sizeof(fullname));
                        snprintf(fullname,  512, "%s%s", smb_config.multrule[index]->mount_path, str);
                        if(access(fullname, F_OK) != 0)
                        {
                                DEBUG("%s\n", fullname);
                                local_mkdir(fullname);
                        }
                        str[i] = '/';
                }
        }


        memset(fullname, 0, sizeof(fullname));
        snprintf(fullname, 512, "%s%s", smb_config.multrule[index]->mount_path, str);

        if(len > 0 && access(fullname, F_OK) != 0)
        {
                DEBUG("%s\n", fullname);
                local_mkdir(fullname);
        }
}

long long stat_file(const char *filename)
{
        struct stat filestat;
        if( stat(filename, &filestat) == -1)
        {
                perror("stat:");
                return -1;
        }
        return  filestat.st_size;
}

void replace_char_in_str(char *str,char newchar,char oldchar){

        int i;
        int len;
        len = strlen(str);
        for(i = 0; i < len; i++)
        {
                if(str[i] == oldchar)
                {
                        str[i] = newchar;
                }
        }

}

int create_shell_file()
{
        FILE *fp = NULL;
        char contents[256] = {0};
#ifndef USE_TCAPI
        snprintf(contents, 256, "%s", "#! /bin/sh\n" \
               "nvram set $2=\"$1\"\n" \
               "nvram commit");
#else
        snprintf(contents, 256, "%s", "#! /bin/sh\n" \
               "tcapi set AiCloud_Entry $2 \"$1\"\n" \
               "tcapi commit AiCloud\n" \
               "tcapi save");
#endif
        if(( fp = fopen(SHELL_FILE, "w"))==NULL)
        {
                fprintf(stderr,"create shell file error");
                return -1;
        }

        fprintf(fp, "%s", contents);
        fclose(fp);
        return 0;
}

#ifndef NVRAM_
int write_get_nvram_script(char *name,char *shell_dir,char *val_dir)
{
        FILE *fp = NULL;
        char contents[512] = {0};
        snprintf(contents, 512, "#! /bin/sh\n" \
                "NV=`nvram get %s`\n" \
                "if [ ! -f \"%s\" ]; then\n" \
                "touch %s\n" \
                "fi\n" \
                "echo \"$NV\" >%s", name, val_dir, val_dir, val_dir);

        if(( fp = fopen(shell_dir, "w"))==NULL)
        {
                fprintf(stderr,"create ftpclient_get_nvram file error\n");
                return -1;
        }

        fprintf(fp, "%s", contents);
        fclose(fp);

        return 0;
}
#endif

void clear_global_var(){
        if(initial_tokenfile_info_data(&tokenfile_info_start) == NULL)
        {
                return;
        }
}

int write_notify_file(char *path, int signal_num)
{
        FILE *fp;
        char fullname[64];
        memset(fullname,0,sizeof(fullname));

        local_mkdir("/tmp/notify");
        local_mkdir("/tmp/notify/usb");
        snprintf(fullname, 64, "%s/usbclient", path);
        fp = fopen(fullname, "w");
        if(NULL == fp)
        {
                printf("open notify %s file fail\n", fullname);
                return -1;
        }
        fprintf(fp, "%d", signal_num);
        fclose(fp);
        return 0;
}

void stop_process_clean_up()
{
        unlink("/tmp/notify/usb/usbclient");
        pthread_cond_destroy(&cond);
        pthread_cond_destroy(&cond_socket);
        pthread_cond_destroy(&cond_log);
        unlink(LOG_DIR);
}
