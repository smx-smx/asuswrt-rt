#ifndef LIST_H
#define LIST_H

#define ADD_TREE_NODE    1
#define DEL_TREE_NODE    2

#define MAX_LENGTH 128
#define MIN_LENGTH 32
#define MAX_CONTENT 256
#define MAXDATASIZE 1024
#define MINSIZE 64

#define getb(type) (type*)malloc(sizeof(type))

/*server item tree struct and function*/
typedef struct node{
    char *href;
    char *getcontentlength;
    int isFolder;
    time_t modtime;
    int ismodify;           /*Is server file modify. 0 is no modify,1 is modify*/
    struct node *next;
}CloudFile;


typedef struct BROWSE
{
    int foldernumber;
    int filenumber;
    CloudFile *folderlist;
    CloudFile *filelist;
}Browse;


struct ServerTreeNode
{
    char *parenthref;
    Browse *browse;
    struct ServerTreeNode *Child;
    struct ServerTreeNode *NextBrother;
};

typedef struct ServerTreeNode Server_TreeNode;

Server_TreeNode *ServerRootNode;
Server_TreeNode *OldServerRootNode;

Server_TreeNode *create_server_treeroot();
int browse_to_tree(char *parenthref,Server_TreeNode *node,int index);
CloudFile *get_CloudFile_node(Server_TreeNode* treeRoot,const char *dofile_href,int a);
void SearchServerTree(Server_TreeNode* treeRoot);
void free_server_tree(Server_TreeNode *node);


/*Local item struct and function of every floor*/
typedef struct LOCALFOLDER{
    char *path;
    char *name;
    struct LOCALFOLDER *next;
}LocalFolder;

typedef struct LOCALFILE{
    char *path;
    char *name;
    struct LOCALFILE *next;
}LocalFile;

typedef struct LOCAL
{
    int foldernumber;
    int filenumber;
    LocalFolder *folderlist;
    LocalFile *filelist;
}Local;

Local *Find_Floor_Dir(const char *path);
void free_localfloor_node(Local *local);
#endif
