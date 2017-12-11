#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>
#include <dirent.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ne_request.h>
#include <ne_auth.h>
#include <utime.h>
#include <sys/statvfs.h>
#include <signal.h>
#include <pthread.h>
#include <webdav_base.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <ne_uri.h>
#include <ne_xml.h>
#include <ne_string.h>
#include <ne_session.h>
#include <ne_request.h>
#include <ne_basic.h>
#include "list.h"

pthread_t newthid1,newthid2,newthid3;
pthread_cond_t cond,cond_socket,cond_log;
pthread_mutex_t mutex,mutex_socket,mutex_receve_socket,mutex_log,mutex_checkdisk,mutex_copyfilelist;

#define INOTIFY_PORT 3678

#define NO_PARENT 427

#define LIST_TEST 1


//#define __DEBUG__
//#undef __DEBUG__

#ifdef __DEBUG__
//#define DEBUG(format, ...) printf("FILE:"__FILE__",LINE:%05d:"format"\n",__LINE__,##__VA_ARGS__)
#define DEBUG(format, ...) printf(format,##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

char log_base_path[MAX_LENGTH];
char log_path[MAX_LENGTH];

char general_log[MAX_CONTENT];
#define ERRORLOG "/tmp/smartsync/.logs/webdav_client_errlog"

char base64_auth[256];
long long int DiskAvailable;
long long int DiskAvailableShow;
long long int DiskUsedShow;
long long int PreDiskAvailableShow;
long long int PreDiskUsedShow;

/* log struct */
typedef struct LOG_STRUCT{
    uint8_t  status;
    char  error[512];
    float  progress;
    char path[512];
}Log_struc;

#define S_INITIAL		70
#define S_SYNC			71
#define S_DOWNUP		72
#define S_UPLOAD		73
#define S_DOWNLOAD		74
#define S_STOP			75
#define S_ERROR			76
#define LOG_SIZE                sizeof(struct LOG_STRUCT)

#define LOCAL_SPACE_NOT_ENOUGH          900
#define SERVER_SPACE_NOT_ENOUGH         901
#define LOCAL_FILE_LOST                 902
#define SERVER_FILE_DELETED             903
#define COULD_NOT_CONNECNT_TO_SERVER    904
#define CONNECNTION_TIMED_OUT           905
#define INVALID_ARGUMENT                906
#define PARSE_XML_FAILED                907
#define COULD_NOT_READ_RESPONSE_BODY    908
#define HAVE_LOCAL_SOCKET               909
#define SERVER_ROOT_DELETED             910

int sync_up;
int sync_down;
int download_only;
int upload_only;
int finished_initial;
int del_rule_num;
int local_sync;
int server_sync;
int first_sync;

ne_off_t prog_total;
time_t prog_time;

#ifndef NVRAM_
#ifndef PC
#define CONFIG_PATH "/tmp/smartsync/webdav/config/webdav.conf"
#define TOKENFILE_RECORD "/opt/etc/.smartsync/wd_tokenfile"
#define GET_NVRAM_SCRIPT_1 "/tmp/smartsync/webdav/script/webdav_get_nvram"
#define SH_GET_NVRAM_SCRIPT_1 "sh /tmp/smartsync/webdav/script/webdav_get_nvram"
#define NVRAM_PATH_1 "/tmp/smartsync/webdav/config/webdav_tmpconfig"
#define GET_NVRAM_SCRIPT_2 "/tmp/smartsync/webdav/script/webdav_get_nvram_link"
#define SH_GET_NVRAM_SCRIPT_2 "sh /tmp/smartsync/webdav/script/webdav_get_nvram_link"
#define NVRAM_PATH_2 "/tmp/smartsync/webdav/config/link_internet"
#else
#define CONFIG_PATH "/tmp/Cloud/webdav.conf"
#define TOKENFILE_RECORD "/tmp/wd_tokenfile"
#endif
#else
#define CONFIG_PATH "/tmp/smartsync/webdav/config/webdav.conf"
#define NVRAM_USBINFO "webdav_config_usbinfo"        //save the url-disk-id
#endif

#ifdef NVRAM_
#ifndef USE_TCAPI
        #include <bcmnvram.h>
    #else
        #define WANDUCK "Wanduck_Common"
        #define AICLOUD "AiCloud_Entry"
        #include "libtcapi.h"
        #include "tcapi.h"
    #endif
#include <shutils.h>
#endif

typedef struct Action_Item
{
    char *action;
    char *path;
    struct Action_Item *next;
} action_item;

struct queue_entry
{
    char *cmd_name;
    struct queue_entry * next_ptr;   /* Pointer to next entry */
};
typedef struct queue_entry * queue_entry_t;

/*struct queue*/
struct queue_struct
{
    struct queue_entry * head;
    struct queue_entry * tail;
};
typedef struct queue_struct *queue_t;

queue_t SocketActionList;
queue_entry_t SocketActionTmp;

/*muti dir read config*/
struct asus_rule
{
    int rule;
    char base_path[512];     //base_path is the sync path
    int base_path_len;
    char base_folder[256];   //base_folder is the sync folder name
    char rooturl[MAX_CONTENT];
    char rootfolder[256];
    char host[128];
    int rootfolder_length;
    char mount_path[MAX_LENGTH];
    char desc[256];
    char capacha[256];
    ne_session *sess;
};

struct asus_config
{
    int type;
    int  enable;
    int dir_number;
    struct asus_rule **prule;
};
struct asus_config asus_cfg,asus_cfg_stop;

typedef struct SYNC_LIST{
    char up_item_file[256];
    char conflict_file[256];
    char temp_path[MAX_LENGTH];
    int sync_disk_exist;
    int sync_up;
    int sync_down;
    int download_only;
    int upload_only;
    int first_sync;
    int receve_socket;
    int have_local_socket;
    int init_completed;
    int no_local_root;
    int server_sync_running;

    action_item *copy_file_list;         //The copy files
    action_item *server_action_list;    //Server变化导致的Socket
    action_item *dragfolder_action_list;   //dragfolder时递归动作造成的Socket重复
    action_item *unfinished_list;
    action_item *download_only_socket_head;
    action_item *up_space_not_enough_list;
    queue_t SocketActionList;
    queue_t SocketActionList_Priority;      //rename0 and move0
    Server_TreeNode *ServerRootNode;
    Server_TreeNode *OldServerRootNode;
    int index;
}sync_list;
sync_list **g_pSyncList;

struct mounts_info_tag
{
    int num;
    char **paths;
};

struct tokenfile_info_tag
{
    char *folder;
    char *url;
    char *mountpath;
    struct tokenfile_info_tag *next;
};
struct tokenfile_info_tag *tokenfile_info,*tokenfile_info_start,*tokenfile_info_tmp;

char *change_local_same_name(char *fullname);
char *change_server_same_name(char *fullname,int index);
char *change_server_same_name_root(char *filename);
int Download(char *dofile_href,int index);
int Upload(char *filepath,int index);
int Delete(char *filepath,int index);
int Mkcol(char *folderpath,int index);
time_t Getmodtime(char *serverhref,int index);
long long int GetContentLength(char *serverhref,int index);
int is_Server_Copying(char *serverhref,int index);
int Move(char *oldpath,char *newpath,int index);
void *xmalloc (size_t size);
char *xstrdup (const char *s);
void *xrealloc (void *ptr, size_t size);
char *oauth_url_escape(const char *string);
char *oauth_url_unescape(const char *string, size_t *olen);
int parse_config(char *path);
int getServerList();
int send_action(int type, char *content);
void *SyncLocal();
void *SyncServer();
int parse_config(char *path);
void init_global_var();
void clear_global_var();
int cmd_parser(char *cmd,int index);
int test_if_dir(const char *dir);
int test_if_dir_empty(char *dir);
int wait_file_copying(char *filename);
long long stat_file(const char *filename);
int createFolder(char *path,int index);
int deal_dragfolder_to_socketlist(char *dir,int index);
int compareServerList(int index);
int compareLocalList(int index);
void read_config();
int my_auth(void *userdata, const char *realm, int attempt,char *username, char *password);
int ChangeFile_modtime(char *filepath,time_t servermodtime,int index);
int compareTime(time_t servermodtime ,char *localpath);
void wd_escape(char *unescapepath,char *escapepath);
int getCloudInfo(char *URL,int (*cmd_data)(char *, int, CloudFile *, CloudFile *, int),CloudFile *TreeFolderTail,CloudFile *TreeFileTail,int index);
int getCloudInfo_one(char *URL,int (*cmd_data)(char *, int, CloudFile *, int),CloudFile *FileTail_one,int index);
void del_all_items(char *dir,int index);
time_t GetFile_modtime(char *localpath);
int is_file_copying(char *filename);
long long int get_local_freespace(int index);
void sig_handler (int signum);
void free_CloudFile_item(CloudFile *head);
void free_LocalFolder_item(LocalFolder *head);
void free_LocalFile_item(LocalFile *head);
int write_log(int status, char *message, char *filename,int index);
int write_error_log(char *message);
int get_all_local_file_item(char *path,LocalFile *head);
int wd_create_session(int j);
int do_unfinished(int index);
int strnicmp(const char *s1, const char *s2, size_t len);
int add_FolderList_item(CloudFile *head,char *item_href);
int queue_empty (queue_t q);
queue_t queue_create ();
void queue_enqueue (queue_entry_t d, queue_t q);
queue_entry_t queue_dequeue (queue_t q);
void queue_destroy (queue_t q);
int is_copying_finished(char *filename);
Browse *browseFolder(char *URL,int index);
int initMyLocalFolder(Server_TreeNode *servertreenode,int index);
char *serverpath_to_localpath(char *from_serverpath,int index);
char *localpath_to_serverpath(char *from_localpath,int index);
int add_action_item(const char *action,const char *path,action_item *head);
int add_all_download_only_socket_list(char *cmd,const char *dir,int index);
void free_action_item(action_item *head);
action_item *get_action_item(const char *action,const char *path,action_item *head,int index);
int del_action_item(const char *action,const char *path,action_item *head);
void del_download_only_action_item(const char *action,const char *path,action_item *head);
int add_all_download_only_dragfolder_socket_list(const char *dir,int index);
action_item *create_action_item_head();
int is_local_space_enough(CloudFile *do_file,int index);
char *my_str_malloc(size_t len);
int test_if_download_temp_file(char *filename);
int test_if_aicloud_temp_file(char *filename);
void my_mkdir(char *path);
void my_mkdir_r(char *path,int index);
int newer_file(char *localpath,int index);
int init_newer_file(char *localpath,int index);
int sync_newer_file(char *localpath,int index);
char *my_nstrchr(const char chr,char *str,int n);
void handle_quit_upload();
int wait_handle_socket(int index);
int moveFolder(char *old_dir,char *new_dir,int index);
int is_server_exist(char *parenthref,char *filepath,int index);
int is_server_exist_with_type(char *filepath);
char *parse_name_from_path(const char *path);
int get_create_threads_state();
int ne_getrouterinfo(int (*cmd_data)(char *, int, int),int index);
struct tokenfile_info_tag *initial_tokenfile_info_data(struct tokenfile_info_tag **token);
int free_tokenfile_info(struct tokenfile_info_tag *head);
int delete_tokenfile_info(char *url,char *folder);
int add_tokenfile_info(char *url,char *folder,char *mpath);
char *delete_nvram_contents(char *url,char *folder);
char *add_nvram_contents(char *url,char *folder);
int write_debug_log(char *str);
int detect_process(char * process_name);
void wd_set_error(char *error,int size,const char *format, ...);
int add_socket_item(char *buf,int pri,int i);
char *search_newpath(char *href,int i);
int write_conflict_log(char *prename, char *conflict_name,int index);
int check_link_internet(int index);
int write_get_nvram_script(char *nvram_name,char *nvram_path,char *script_path);
char *parse_parentfolder_from_path(const char *path);
void create_start_file();
int detect_process_file();
int wd_parsexml(char *xmlBuf,int xmlBufLength,CloudFile *TreeFolderTail,CloudFile *TreeFileTail,int index);
int wd_parsexml_one(char *xmlBuf,int xmlBufLength,CloudFile *FileTail_one,int index);
int wd_parsexml_RouterInfo(char *xmlBuf,int xmlBufLength,int index);
