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
    //TreeRoot->level=0;
    TreeRoot->NextBrother = NULL;
    //TreeRoot->browse = NULL;
    //sprintf(TreeRoot->parenthref,"%s%s/",HOST,ROOTFOLDER);
    TreeRoot->parenthref = NULL;
    TreeRoot->browse = NULL;
    TreeRoot->Child = NULL;

    return TreeRoot;
}



int browse_to_tree(char *parenthref,Server_TreeNode *node,int index)
{
    //DEBUG("browse_to_tree node parenthref is %s\n",parenthref);
    Browse *br = NULL;
    int fail_flag = 0;
    //int loop;
    //int i;  

    Server_TreeNode *tempnode = NULL, *p1 = NULL,*p2 = NULL;
    tempnode = create_server_treeroot();
    //tempnode->level = node->level + 1;

    tempnode->parenthref = my_str_malloc((size_t)(strlen(parenthref)+1));
    //memset(tempnode->parenthref,0,sizeof(tempnode->parenthref));

    br = browseFolder(parenthref,index);
    sprintf(tempnode->parenthref,"%s",parenthref);

    if(NULL == br)
    {
        free_server_tree(tempnode);
        //printf("browse folder failed\n");
        return -1;
    }

    tempnode->browse = br;

    if(node->Child == NULL)
    {
        node->Child = tempnode;
    }
    else
    {
        //printf("have child\n");
        p2 = node->Child;
        p1 = p2->NextBrother;

        while(p1 != NULL)
        {
            //printf("p1 nextbrother have\n");
            p2 = p1;
            p1 = p1->NextBrother;
        }

        p2->NextBrother = tempnode;
        tempnode->NextBrother = NULL;
    }

    //printf("browse folder num is %d\n",br->foldernumber);
    CloudFile *de_foldercurrent;
    de_foldercurrent = br->folderlist->next;
    while(de_foldercurrent != NULL && !exit_loop)
    {
        char purl[1024];
        sprintf(purl,"%s%s",asus_cfg.prule[index]->rooturl,de_foldercurrent->href);
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
    //int i;
    //int j;
    //for(i=0;i<treeRoot->level;i++)
    //    printf("-");
    //memory_used += sizeof(Server_TreeNode);
    //if(NULL != treeRoot->parenthref)
        //memory_used += strlen(treeRoot->parenthref)+1;

    if(treeRoot->browse != NULL)
    {
        //memory_used += sizeof(Browse);

        CloudFile *de_foldercurrent,*de_filecurrent;
        de_foldercurrent = treeRoot->browse->folderlist->next;
        de_filecurrent = treeRoot->browse->filelist->next;
        while(de_foldercurrent != NULL){
            //memory_used += sizeof(CloudFile);
            //memory_used += strlen(de_foldercurrent->href)+1;

            //printf("serverfolder->href = %s\n",de_foldercurrent->href);
            de_foldercurrent = de_foldercurrent->next;
        }
        while(de_filecurrent != NULL){
            //memory_used += sizeof(CloudFile);
            //memory_used += strlen(de_filecurrent->href)+1;
            //memory_used += strlen(de_filecurrent->getcontentlength)+1;

            //printf("serverfile->href = %s,serverfile->modtime = %lu\n",de_filecurrent->href,de_filecurrent->modtime);
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

    //printf("****get_CloudFile_node****dofile_href = %s\n",dofile_href);
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
        //printf("111111folder = %d,file = %d\n",treeRoot->browse->foldernumber,treeRoot->browse->filenumber);
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
                    //printf("de_foldercurrent->href = %s\n",de_foldercurrent->href);
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
                    //printf("de_filecurrent->href = %s\n",de_filecurrent->href);
                    if(!(strncmp(de_filecurrent->href,dofile_href,href_len)))
                    {
                        //printf("get it\n");
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
    //printf("free_server_tree\n");
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
        sprintf(fullname,"%s/%s",path,ent->d_name);

        if(test_if_dir(fullname) == 1)
        {
            localfloorfoldertmp = (LocalFolder *)malloc(sizeof(LocalFolder));
            memset(localfloorfoldertmp,0,sizeof(localfloorfoldertmp));
            localfloorfoldertmp->path = my_str_malloc((size_t)(strlen(fullname)+1));
            localfloorfoldertmp->name = my_str_malloc(strlen(ent->d_name)+1);

            sprintf(localfloorfoldertmp->name,"%s",ent->d_name);
            sprintf(localfloorfoldertmp->path,"%s",fullname);

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

            sprintf(localfloorfiletmp->name,"%s",ent->d_name);
            sprintf(localfloorfiletmp->path,"%s",fullname);

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
    //printf("local->filenumber = %d\nlocal->foldernumber = %d\n",local->filenumber,local->foldernumber);
    free_LocalFile_item(local->filelist);
    free_LocalFolder_item(local->folderlist);
    free(local);
}

