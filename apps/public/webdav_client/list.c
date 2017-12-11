#include <stdio.h>
#include "base.h"

/*server tree root function*/
Server_TreeNode *create_server_treeroot()
{
    Server_TreeNode *TreeRoot = NULL;
    TreeRoot = (Server_TreeNode *)malloc(sizeof (Server_TreeNode));
    memset(TreeRoot,0,sizeof(Server_TreeNode));
    if(TreeRoot == NULL)
    {
        printf("create memory error!\n");
        exit(-1);
    }

    TreeRoot->NextBrother = NULL;
    TreeRoot->parenthref = NULL;
    TreeRoot->browse = NULL;
    TreeRoot->Child = NULL;

    return TreeRoot;
}



int browse_to_tree(char *parenthref,Server_TreeNode *node,int index)
{
    Browse *br = NULL;
    int fail_flag = 0;

    Server_TreeNode *tempnode = NULL, *p1 = NULL,*p2 = NULL;
    tempnode = create_server_treeroot();

    tempnode->parenthref = my_str_malloc((size_t)(strlen(parenthref)+1));

    br = browseFolder(parenthref,index);
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
        char purl[1024];
        snprintf(purl, 1024, "%s%s",asus_cfg.prule[index]->rooturl,de_foldercurrent->href);
        if(browse_to_tree(purl,tempnode,index) == -1)
        {
            fail_flag = 1;
        }
        de_foldercurrent = de_foldercurrent->next;
    }

    if(exit_loop)
        fail_flag = 1;

    return (fail_flag == 1) ? -1 : 0 ;

}



void SearchServerTree(Server_TreeNode* treeRoot)
{
    if(treeRoot->browse != NULL)
    {
        CloudFile *de_foldercurrent,*de_filecurrent;
        de_foldercurrent = treeRoot->browse->folderlist->next;
        de_filecurrent = treeRoot->browse->filelist->next;
        while(de_foldercurrent != NULL){
            de_foldercurrent = de_foldercurrent->next;
        }
        while(de_filecurrent != NULL){
            de_filecurrent = de_filecurrent->next;
        }
    }

    if((treeRoot->Child!=NULL))
        SearchServerTree(treeRoot->Child);

    if(treeRoot->NextBrother != NULL)
        SearchServerTree(treeRoot->NextBrother);
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


void free_server_tree(Server_TreeNode *node)
{
    if(node != NULL)
    {

        if(node->NextBrother != NULL)
        {
            free_server_tree(node->NextBrother);
            node->NextBrother = NULL;
        }

        if(node->Child != NULL)
        {
            free_server_tree(node->Child);
            node->Child = NULL;
        }

        if(node->parenthref != NULL)
        {
            free(node->parenthref);
            node->parenthref = NULL;
        }
        if(node->browse != NULL)
        {
            free_CloudFile_item(node->browse->filelist);
            free_CloudFile_item(node->browse->folderlist);
            free(node->browse);
            node->browse = NULL;
        }
        free(node);
        node = NULL;
    }
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
    char *fullname;

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
        if(ent->d_name[0] == '.')
            continue;
        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;
        if(test_if_download_temp_file(ent->d_name))     //download temp files
            continue;
        if(test_if_aicloud_temp_file(ent->d_name))      //aicloud Uploading files
            continue;

        size_t len;
        len = strlen(path)+strlen(ent->d_name)+2;
        fullname = my_str_malloc(len);
        snprintf(fullname, sizeof(char)*len, "%s/%s",path,ent->d_name);

        if(test_if_dir(fullname) == 1)
        {
            localfloorfoldertmp = (LocalFolder *)malloc(sizeof(LocalFolder));
            memset(localfloorfoldertmp,0,sizeof(localfloorfoldertmp));
            localfloorfoldertmp->path = my_str_malloc((size_t)(strlen(fullname)+1));
            localfloorfoldertmp->name = my_str_malloc(strlen(ent->d_name)+1);

            snprintf(localfloorfoldertmp->name, sizeof(char)*(strlen(ent->d_name)+1), "%s",ent->d_name);
            snprintf(localfloorfoldertmp->path, sizeof(char)*(strlen(fullname)+1), "%s",fullname);

            ++foldernum;

            localfloorfoldertail->next = localfloorfoldertmp;
            localfloorfoldertail = localfloorfoldertmp;
            localfloorfoldertail->next = NULL;
        }
        else
        {
            localfloorfiletmp = (LocalFile *)malloc(sizeof(LocalFile));
            memset(localfloorfiletmp,0,sizeof(localfloorfiletmp));
            localfloorfiletmp->path = my_str_malloc((size_t)(strlen(fullname)+1));
            localfloorfiletmp->name = my_str_malloc(strlen(ent->d_name)+1);

            snprintf(localfloorfiletmp->name, sizeof(char)*(strlen(ent->d_name)+1), "%s",ent->d_name);
            snprintf(localfloorfiletmp->path, sizeof(char)*(strlen(fullname)+1), "%s",fullname);

            ++filenum;

            localfloorfiletail->next = localfloorfiletmp;
            localfloorfiletail = localfloorfiletmp;
            localfloorfiletail->next = NULL;
        }
        free(fullname);
        fullname = NULL;
    }

    local->filelist = localfloorfile;
    local->folderlist = localfloorfolder;

    local->filenumber = filenum;
    local->foldernumber = foldernum;

    closedir(pDir);

    return local;

}

/*free保存单层文件夹信息所用的空间*/
void free_localfloor_node(Local *local)
{
    free_LocalFile_item(local->filelist);
    free_LocalFolder_item(local->folderlist);
    free(local);
}
