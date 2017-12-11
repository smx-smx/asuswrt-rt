#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <ftw.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <setjmp.h>
#include <curl/curl.h>
#include <sys/time.h>
#include "api.h"
#include "function.h"
#include "data.h"
#include "list.h"
#include "mem_pool.h"

#define MAXDATASIZE 1024
#define MYPORT 3567
#define INOTIFY_PORT 3678
#define BACKLOG 100 /* max listen num*/

#define SEM 0

#define UP_FAIL   10
#define DOWN_FAIL 11
#define SOCKET_FAIL 12
#define RETRY_MAX_TIME 1



int stop_sync_server;
int pre_seq = -10;
Servicegateway sergate;
Aaa aaa;
int stop_up = 0;
int stop_down = 0 ;
int local_sync;
int server_sync;
int server_modify;
long long int MySyncFolder;
long long int MyRecycleID;
char username[256];
char password[256];
char base_path[256];
sync_item_t from_server_sync_head = NULL;
sync_item_t down_head = NULL;
sync_item_t up_head = NULL;
sync_item_t up_excep_fail = NULL;
sync_item_t download_only_socket_head = NULL;
sync_item_t download_only_modify_head = NULL;
sync_item_t copy_file_list_head = NULL;
int exit_proc = 0;
pthread_t server_thread,local_thread,socket_thread,download_socket_thread;
int no_config;
int restart_run;
int stop_progress = 0;
int exit_loop = 0;
int error_flag;
int retry_fail_time = 0;
my_mutex_t my_mutex = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
my_mutex_t wait_server_mutex = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
my_mutex_t wait_socket_mutex = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
pthread_mutex_t mutex_socket = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_receve_socket = PTHREAD_MUTEX_INITIALIZER;
int upload_only = 0;
int download_only = 0;
s_tree *s_link = NULL;
Hb_TreeNode *DirRootNode = NULL;
Server_TreeNode  *SRootNode = NULL;
Hb_SubNode *SyncNode;
int server_space_full = 0;
int local_space_full = 0 ;
int has_del_fiel = 0;
int has_socket = 0;
long long int max_upload_filesize;
struct asus_config cfg;
int no_local_root = 0; /* handle rm sync root dir*/
int mysync;
int copying_file_number =0;
int has_local_socket = 0 ;
int is_server_running = 0;
int IsAccountFrozen = 0;
int IsSyncError = 0;
api_count_s api_count;
int loop_max;
int usleep_time;
int IsNetworkUnlink = 0;
queue_t SocketActionList;

char mount_path[NAMESIZE];
char cloud_path[NAMESIZE];
char asus_path[NAMESIZE];
char log_path[NAMESIZE];
char xml_path[NAMESIZE];
char sync_path[NAMESIZE];
char system_log[NAMESIZE];
char general_log[NAMESIZE];
char confilicted_log[NAMESIZE];
char up_item_file[NAMESIZE];
char down_item_file[NAMESIZE];

#if WRITE_DOWNLOAD_TEMP_FILE
char down_item_temp_file[NAMESIZE];
#endif

char up_excep_fail_file[NAMESIZE];
char gateway_xml[NAMESIZE];
char get_user_state_xml[NAMESIZE];
char token_xml[NAMESIZE];
char get_info_xml[NAMESIZE];
char get_sync_folder_xml[NAMESIZE];
char get_personal_system_folder_xml[NAMESIZE];
char browse_folder_xml[NAMESIZE];
char propfind_xml[NAMESIZE];
char create_folder_xml[NAMESIZE];
char rename_xml[NAMESIZE];
char move_xml[NAMESIZE];
char remove_xml[NAMESIZE];
char update_xml[NAMESIZE];
char get_entry_info_xml[NAMESIZE];
char get_change_seq_xml[NAMESIZE];
char init_upload_xml[NAMESIZE];
char resume_upload_xml[NAMESIZE];
char finish_upload_xml[NAMESIZE];
char trans_excep_file[128];
char system_token[256];

int receve_socket;
int is_check_token;
int no_completed;
#ifdef IPKG
int disk_change;
int sync_disk_removed;
char disk_info_filename[128];
char record_token_file[256];
char token_filename[256];
#endif

void *SyncServer();
void *SyncLocal();
void sig_handler (int signum);
int cmd_parser(char *command);
int send_action(int type,char *content);
void init_global_var();
void clean_global_var();
void clean_up();
void run();
int read_config();
void handle_fail_item();
void* sigmgr_thread();

char *get_mount_path(char *path, int n);
void create_parent_folder_r(char *path);

#ifdef IPKG
int write_notify_file(char *path, int signal_num);
#endif


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
            "MIID/jCCAuagAwIBAgIQFaxulBmyeUtB9iepwxgPHzANBgkqhkiG9w0BAQsFADCB\n"
            "mDELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUdlb1RydXN0IEluYy4xOTA3BgNVBAsT\n"
            "MChjKSAyMDA4IEdlb1RydXN0IEluYy4gLSBGb3IgYXV0aG9yaXplZCB1c2Ugb25s\n"
            "eTE2MDQGA1UEAxMtR2VvVHJ1c3QgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhv\n"
            "cml0eSAtIEczMB4XDTA4MDQwMjAwMDAwMFoXDTM3MTIwMTIzNTk1OVowgZgxCzAJ\n"
            "BgNVBAYTAlVTMRYwFAYDVQQKEw1HZW9UcnVzdCBJbmMuMTkwNwYDVQQLEzAoYykg\n"
            "MjAwOCBHZW9UcnVzdCBJbmMuIC0gRm9yIGF1dGhvcml6ZWQgdXNlIG9ubHkxNjA0\n"
            "BgNVBAMTLUdlb1RydXN0IFByaW1hcnkgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkg\n"
            "LSBHMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANziXmJYHTNXOTIz\n"
            "+uvLh4yn1ErdBojqZI4xmKU4kB6Yzy5jK/BGvESyiaHAKAxJcCGVn2TAppMSAmUm\n"
            "hsalifD614SgcK9PGpc/BkTVyetyEH3kMSj7HGHmKAdEc5IiaacDiGydY8hS2pgn\n"
            "5whMcD60yRLBxWeDXTPzAxHsatBT4tG6NmCUgLthY2xbF37fQJQeqw3CIShwiP/W\n"
            "JmxsYAQlTlV+fe+/lEjetx3dcI0FX4ilm/LC7urRQEFtYjgdVgbFA0dRIBn8exAL\n"
            "DmKudlW/X3e+PkkBUz2YJQN2JFodtNuJ6nnltrM7P7pMKEF/BqxqjsHQ9gUdfeZC\n"
            "huOl1UcCAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw\n"
            "HQYDVR0OBBYEFMR5yo6hTgMdHNxr2zFblD4/MH8tMA0GCSqGSIb3DQEBCwUAA4IB\n"
            "AQAtxRPPVoB7eni9n64smefv2t+UXglpp+duaIy9cr5HqQ6XErhK8WTTOd8lNNTB\n"
            "zU6B8A8ExCSzNJbGpqow32hhc9f5joWJ7w5elShKKiePEI4ufIbEAp7aDHdlDkQN\n"
            "kv39sxY2+hENHYwOB4lqKVb3cvTdFZx3NWZXqxNT2I7BQMXXExZacse3aQHEerGD\n"
            "AWh9jUGhlBjBJVz88P6DAod8DQ3PLghcSkANPuyBYeYk28rgDi0Hsj5W3I31QYUH\n"
            "SJsMC8tJP33st/3LjWeJGqvtux6jAAgIFyqCXDFdRootD4abdNlF+9RAsXqqaC2G\n"
            "spki4cErx5z481+oghLrGREt\n"
            "-----END CERTIFICATE-----\n");
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
#if 1
    sigset_t bset,oset;
    pthread_t sig_thread;

    sigemptyset(&bset);
    sigaddset(&bset,SIGUSR1);
    sigaddset(&bset,SIGUSR2);
    sigaddset(&bset,SIGTERM);
    sigaddset(&bset,SIGPIPE);

    if( pthread_sigmask(SIG_BLOCK,&bset,&oset) == -1)
        printf("!! Set pthread mask failed\n");

    if( pthread_create(&sig_thread,NULL,(void *)sigmgr_thread,NULL) != 0)
    {
        printf("thread creation failder\n");
        exit(-1);
    }
#endif


    char error_message[NORMALSIZE];

    memset(error_message,0,sizeof(error_message));

    clean_global_var();

    snprintf(base_path, 256, "%s", "/tmp");

    DIR *dp = opendir(base_path);

    if(NULL == dp)
    {
        printf("open %s dir fail,asuswebstorage exit\n",base_path);
        snprintf(error_message,NORMALSIZE,"open %s dir fail,asuswebstorage exit",base_path);
        handle_error(S_OPENDIR_FAIL,error_message);
        exit(-1);
    }

    closedir(dp);
    init_global_var();
    write_log(S_INITIAL,"","");

#ifdef IPKG
    write_get_nvram_script("cloud_sync",NVRAM_PATH_1,GET_NVRAM_SCRIPT_1);
    write_get_nvram_script("link_internet",NVRAM_PATH_2,GET_NVRAM_SCRIPT_2);
    system(SH_GET_NVRAM_SCRIPT_1);
    enter_sleep_time(1000*2000,NULL);
#endif

#ifdef IPKG
    write_notify_file(NOTIFY_PATH,SIGUSR2);
#endif

    read_config();
    if(write_rootca() != 0)
        exit(-1);

    if(no_config == 0 && !stop_progress)
        run();

    curl_global_cleanup();

#ifdef IPKG
    remove("/tmp/notify/usb/asuswebstorage");
#endif

    printf("stop asuswebstorage end\n");

    return 0;
}

void *SyncServer()
{
    printf("sync server Thread start\n");

    pre_seq = -10 ;
    int cur_seq = 0;
    int sync_fail = 0;

    while(!exit_loop)
    {
        while(local_sync && !exit_loop)
            enter_sleep_time(1000*10,NULL);
        server_sync = 1;

        if(local_sync == 0 && SocketActionList->head == NULL)
        {
#ifdef IPKG
            if(disk_change)
            {
                pthread_mutex_lock(&mutex_socket);
                disk_change = 0;
                pthread_mutex_unlock(&mutex_socket);
                check_disk_change();
            }
#endif

            Changeseq *cs;
            int status;
            cs = getChangeSeq(MySyncFolder);
            if(NULL == cs)
            {
                enter_sleep_time(1000*100,NULL);
                check_network_state();
                printf("########server get seqnumber fail########\n");
                continue;
            }
            if(cs->status == 2)
            {
#if SYSTEM_LOG
               write_system_log("GetChageSeq","Auth Fail");
#endif
                my_free(cs);

                status = getToken(username,password,"",0);

                if(status != 0)
                {
                    if( status != S_NETWORK_FAIL)
                        exit_loop = 1;
                    continue;
                }

            }
            else if(cs->status == 0)
            {
                cur_seq = cs->changeseq;
                if(local_sync == 0)
                    handle_fail_item();

                if( cur_seq != pre_seq || sync_fail == 1 || pre_seq == -10)
                {
                    printf("pre seq_number is %d,cur seq number is %d\n",pre_seq,cur_seq);
                    printf("#### sync server start#####\n");

#if SYSTEM_LOG
                    write_system_log("sync server","start");
#endif
                    is_server_running = 1;
						  if(pre_seq == -10)
                        SyncNode = create_node(MySyncFolder,-10);
                    status = syncServerAllItem(username,MySyncFolder,sync_path);
                    is_server_running = 0 ;                  
                    write_finish_log();
                    print_all_nodes(SyncNode);
                    printf("#### sync server end#####\n");

#if SYSTEM_LOG
                    write_system_log("sync server","end");
#endif
                    if(status == -1)
                    {
                        if(pre_seq == -10)
                            free_node(SyncNode);
                        sync_fail = 1;
                        enter_sleep_time(1000*100,NULL);
                    }
                    else
                    {
                        sync_fail = 0;
                    }

                    print_count_call_api();
                }

                if(sync_fail != 1)
                {
                    pre_seq = cur_seq;
                }
            }
            else
            {
                handle_error(cs->status,"get seqnumber");
            }
            my_free(cs);

        }
        show_finish_log();
        server_sync = 0;
        enter_sleep_time(30,&wait_server_mutex);
    }
    printf("stop asuswebstorage server sync\n");
    stop_up = 1;

    return NULL;
}

int download_only_add_socket_item(Socket_cmd *socket_cmd)
{
    char fullname[512];
    char old_fullname[512];
    char cmd_name[32];
    char action[32];

    memset(cmd_name,0,sizeof(cmd_name));
    memset(fullname,0,sizeof(fullname));
    memset(old_fullname,0,sizeof(old_fullname));
    memset(action,0,sizeof(action));

    snprintf(cmd_name, 32, "%s", socket_cmd->cmd_name);

    if( !strncmp(cmd_name,"copyfile",strlen("copyfile")) )
    {
        snprintf(fullname, 512, "%s/%s",socket_cmd->path,socket_cmd->filename);
        add_sync_item("copyfile",fullname,copy_file_list_head);
        return 0;
    }
    else if( !strncmp(cmd_name,"rename",strlen("rename")) )
    {
        snprintf(fullname,512,"%s/%s",socket_cmd->path,socket_cmd->newname);
        snprintf(old_fullname,512,"%s/%s",socket_cmd->path,socket_cmd->oldname);
    }
    else if( !strncmp(cmd_name,"move",strlen("move")) )
    {
        snprintf(fullname,512,"%s/%s",socket_cmd->path,socket_cmd->oldname);
        snprintf(old_fullname,512,"%s/%s",socket_cmd->oldpath,socket_cmd->oldname);
    }
    else
    {
        snprintf(fullname,512,"%s/%s",socket_cmd->path,socket_cmd->filename);
    }

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        snprintf(action, 32, "%s", "createfile");
        struct sync_item* item;

        item = get_sync_item("copyfile",fullname,copy_file_list_head);

        if(item != NULL)
        {
            del_sync_item("copyfile",fullname,copy_file_list_head);
        }
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        snprintf(action, 32, "%s", "remove");
        del_download_only_sync_item(action,fullname,download_only_socket_head);
        del_download_only_sync_item(action,fullname,download_only_modify_head);
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        snprintf(action, 32, "%s", "createfolder");
    }
    else if( strcmp(cmd_name, "rename") == 0 )
    {
        snprintf(action, 32, "%s", "rename");
        del_download_only_sync_item(action,old_fullname,download_only_socket_head);
        del_download_only_sync_item(action,fullname,download_only_modify_head);
    }
    else if( strcmp(cmd_name, "move") == 0 )
    {
        snprintf(action, 32, "%s", "move");
    	del_download_only_sync_item(action,old_fullname,download_only_socket_head);
        del_download_only_sync_item(action,fullname,download_only_modify_head);
    }
    else if( strcmp(cmd_name, "modify") == 0 )
    {
        snprintf(action, 32, "%s", "modify");
    }

    if(from_server_sync_head->next != NULL)
    {
        sync_item_t item = get_sync_item(action,fullname,from_server_sync_head);

        if(item != NULL)
        {
            del_sync_item(action,fullname,from_server_sync_head);
            return 0;
        }

    }

    if(strcmp(cmd_name, "rename") == 0 || strcmp(cmd_name, "move") == 0)
    {
        if(test_if_dir(fullname))
        {
            add_all_download_only_socket_list(cmd_name,fullname);
        }
        else
        {
            add_sync_item(cmd_name,fullname,download_only_socket_head);
        }
    }
    else if(strcmp(cmd_name, "createfolder") == 0 || strcmp(cmd_name, "dragfolder") == 0)
    {
        add_sync_item(cmd_name,fullname,download_only_socket_head);
        if(!strcmp(cmd_name, "dragfolder"))
            add_all_download_only_dragfolder_socket_list(fullname);
    }
    else if( strcmp(cmd_name, "createfile") == 0  || strcmp(cmd_name, "dragfile") == 0 || strcmp(cmd_name, "modify") == 0)
    {
        add_sync_item(cmd_name,fullname,download_only_socket_head);
    }
    else if( strcmp(cmd_name, "modify") == 0)
    {
        sync_item_t item = get_sync_item(action,fullname,download_only_socket_head);

        if(item == NULL)
        {
            add_sync_item(cmd_name,fullname,download_only_modify_head);
        }
    }

    return 0;
}

int trim_sync_path(char *in,char *out)
{
    int len = 0;
    char *p = NULL;
    char prefix[1024] = {0};
    char new_buf[1024] = {0};
    char tail[1024] = {0};

    len = strlen(sync_path);
    p = strstr(in,sync_path);
    if(p != NULL)
    {
        strncpy(prefix,in,strlen(in)-strlen(p));
        snprintf(tail, 1024, "%s", p+len);

        if(p[len] == '\n')
            snprintf(new_buf, 1024, "%s/%s",prefix,tail);
        else
            snprintf(new_buf, 1024, "%s%s",prefix,tail);

        snprintf(out, 1024, "%s", new_buf);
    }
    else
        snprintf(out, 1024, "%s", in);

    return 0;
}

int add_socket_item(char *in)
{
    int len;
    char buf[1024] = {0};
    char move_buf[1024] = {0};

    trim_sync_path(in,buf); // trim sync path

    if(!strncmp(in,"move",4))
    {
       snprintf(move_buf, 1024, "%s", buf);
       memset(buf,0,1024);
       trim_sync_path(move_buf,buf);
    }

    pthread_mutex_lock(&mutex_socket);
    queue_entry_t SocketActionTmp;

#if MEM_POOL_ENABLE
    SocketActionTmp = mem_alloc(8);
#else
    SocketActionTmp = malloc (sizeof (struct queue_entry));
#endif
    if(SocketActionTmp == NULL)
    {
        printf("SocketActionTmp momery not enough\n");
        return -1;
    }
    memset(SocketActionTmp,0,sizeof(struct queue_entry));
    len = strlen(buf)+1;

#if MEM_POOL_ENABLE
    SocketActionTmp->cmd_name = mem_alloc(len);
#else
    SocketActionTmp->cmd_name = (char *)calloc(len,sizeof(char));
#endif
    if(SocketActionTmp->cmd_name == NULL)
    {
        printf("momery not enough\n");
        return -1;
    }
    memset(SocketActionTmp->cmd_name,0,len);
#if MEM_POOL_ENABLE
    sprintf(SocketActionTmp->cmd_name,"%s",buf);
#else
    snprintf(SocketActionTmp->cmd_name, len*sizeof(char), "%s",buf);
#endif


    queue_enqueue(SocketActionTmp,SocketActionList);
    pthread_mutex_unlock(&mutex_socket);

    return 0;
}

void *SyncLocal()
{
    printf("sync local Thread start\n");

    int sockfd;
    int new_fd;
    int numbytes;
    char buf[MAXDATASIZE];
    int yes = 1;

    struct sockaddr_in my_addr; /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    socklen_t sin_size;

    fd_set read_fds;
    fd_set master;
    int fdmax;
    struct timeval timeout;
    int ret;

    FD_ZERO(&read_fds);
    FD_ZERO(&master);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return NULL;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("Server-setsockopt() error lol!");
        return NULL;
    }

    my_addr.sin_family = AF_INET; /* host byte order */
    my_addr.sin_port = htons(MYPORT); /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
    bzero(&(my_addr.sin_zero), sizeof(my_addr.sin_zero)); /* zero the rest of the struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct
                                                         sockaddr))== -1) {
        perror("bind");
        return NULL;
    }
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        return NULL;
    }
    sin_size = sizeof(struct sockaddr_in);    //add by alan

    FD_SET(sockfd,&master);
    fdmax = sockfd;

    while(!exit_loop)
    { /* main accept() loop */
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000*100;

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
                continue;
            }

            if(buf[strlen(buf)] == '\n')
            {
                buf[strlen(buf)] = '\0';
            }

            close(new_fd);  //add by alan

            pthread_mutex_lock(&mutex_receve_socket);
            receve_socket = 1;
            pthread_mutex_unlock(&mutex_receve_socket);
           
            add_socket_item(buf);

        }
    }

    printf("stop asuswebstorage local sync\n");

    close(sockfd);

    stop_down = 1;
    return NULL;
}

void *Socket_Parser(){
    printf("*******Socket_Parser start********\n");
    queue_entry_t socket_execute;
    int status = 0;
    mysync = 1;
    char buf[1024] = {0};

    while(!exit_loop)
    {
        while(server_sync && !exit_loop)
            enter_sleep_time(1000*100,NULL);
        local_sync = 1;
        if(no_local_root == 1)
        {
            my_mkdir(sync_path);
            send_action(1,sync_path);
            if(upload_only)
                mysync = 1;
            else
                pre_seq = -10;
            no_local_root = 0;
            continue;
        }

#ifdef IPKG
        if(disk_change)
        {
            pthread_mutex_lock(&mutex_socket);
            disk_change = 0;
            pthread_mutex_unlock(&mutex_socket);
            check_disk_change();
        }
#endif
        if(upload_only == 1 && mysync == 1)
        {
                printf("upload only mysync start\n");
                status = syncServerAllItem(username,MySyncFolder,sync_path);
                printf("upload only mysync end,status is %d\n",status);
                //write_log(S_SYNC,"","");
                if(status == 0)
                    mysync = 0;
        }

        while(!exit_loop) //if file is copying ,don't exit local sync
        {
            if(upload_only)
            {
                handle_fail_item();
            }

            while(!exit_loop)
            {
                has_socket = 1;
                pthread_mutex_lock(&mutex_socket);
                if(SocketActionList->head == NULL)
                {
                    pthread_mutex_unlock(&mutex_socket);
                     break;
                }
                socket_execute = SocketActionList->head;
                memset(buf,0,sizeof(buf));
                strncpy(buf,socket_execute->cmd_name,1024);
                pthread_mutex_unlock(&mutex_socket);

                if(cmd_parser(buf) == 0)
                {
                    pthread_mutex_lock(&mutex_socket);
                    socket_execute = queue_dequeue(SocketActionList);

#if MEM_POOL_ENABLE
                    mem_free(socket_execute->cmd_name);
                    mem_free(socket_execute);
#else
                    my_free(socket_execute->cmd_name);
                    my_free(socket_execute);
#endif
                    pthread_mutex_unlock(&mutex_socket);
                }
                else
                   printf("######## socket item fail########\n");              
                enter_sleep_time(1000*100,NULL);
            }

            if(copy_file_list_head->next == NULL || exit_loop)
            {
                break;
            }
            enter_sleep_time(1000*100,NULL);
        }

        if(has_socket == 1)
        {
            if(upload_only)
                write_finish_log();
            has_socket = 0;
        }


       if(!is_server_running)
           has_local_socket = 0;

        pthread_mutex_lock(&mutex_receve_socket);
        receve_socket = 0;
        pthread_mutex_unlock(&mutex_receve_socket);

        local_sync = 0;
        enter_sleep_time(5,&wait_socket_mutex);       
    }


    printf("stop asuswebstorage Socket_Parser\n");
    stop_down = 1;
    return NULL;
}

void sig_handler (int signum)
{
    switch (signum)
    {
    case SIGTERM:
        printf("signal is SIGTERM\n");
        stop_progress = 1;
        exit_loop = 1;
        set_iptables(0);
        pthread_cond_signal(&(wait_server_mutex.cond));
        pthread_cond_signal(&(wait_socket_mutex.cond));
        pthread_cond_signal(&(my_mutex.cond));
#ifdef IPKG
        sync_disk_removed = 0;
#endif
        break;
    case SIGUSR1:  // add user
        printf("signal is SIGUSER1\n");
        exit_loop = 1;
        read_config();
        break;
    case SIGUSR2:  // modify user
        printf("signal is SIGUSR2\n");
#ifdef IPKG
        pthread_mutex_lock(&mutex_socket);
        disk_change = 1;
        pthread_mutex_unlock(&mutex_socket);
#endif
        break;
#if 1
    case SIGPIPE:  // delete user
        printf("signal is SIGPIPE\n");
        signal(SIGPIPE, SIG_IGN);
        break;
#endif
    default:
        printf("signal is %d\n",signum);
        signal(signum,SIG_IGN);
        break;

    }
}

unsigned long stat_file(char *filename)
{
    unsigned long size;
    struct stat filestat;
    if( stat(filename,&filestat) == -1)
    {
        perror("stat:");
        printf("stat file stat error:%s file not exist\n",filename);
        return 0;
    }
    return  filestat.st_size;

    return size;

}

#if 1
int split_socket_cmd(char *cmd_in,Socket_cmd *scmd)
{
#ifdef DEBUG
    printf("socket cmd is %s \n",cmd_in);
#endif

    char cmd[1024];
    char *p;
    const char *split = "\n";
    int i=0;

    memset(cmd,0,sizeof(cmd));
    snprintf(cmd,1024, "%s", cmd_in);

    if(!strncmp(cmd,"rmroot",6))
    {
        no_local_root = 1;
        return 0;
    }

    if(strncmp(cmd,"exit",4) == 0)
    {
        printf("exit socket\n");
        return -1;
    }

    if(strstr(cmd,".asus.td"))
    {
        printf("ignore download temp file\n");
        return -1;
    }

    if(!strchr(cmd,'\n'))
    {
        printf("socket cmd is not math cmd\\npath\\nname format\n");
        return -1;
    }

    p=strtok(cmd,split);

    while(p!=NULL)
    {
        switch (i)
        {
        case 0 :
            if(!strncmp(p,"rename",6) || !strncmp(p,"move",4))
                strncpy(scmd->cmd_name,p,strlen(p)-1);
            else
                snprintf(scmd->cmd_name, 32, "%s", p);
            break;
        case 1:
            if(!strcmp(p,"/"))
                snprintf(scmd->path, 1024, "%s", sync_path);
            else
                snprintf(scmd->path, 1024, "%s%s",sync_path,p);
            break;
        case 2:
            if(!strcmp(scmd->cmd_name,"rename"))
            {
                snprintf(scmd->oldname, NORMALSIZE, "%s", p);
            }
            else if(!strcmp(scmd->cmd_name,"move"))
            {
                if(!strcmp(p,"/"))
                    snprintf(scmd->oldpath, 1024, "%s", sync_path);
                else
                    snprintf(scmd->oldpath, 1024, "%s%s",sync_path,p);
            }
            else
                snprintf(scmd->filename, NORMALSIZE, "%s", p);
            break;
        case 3:
            if(!strcmp(scmd->cmd_name,"rename"))
                snprintf(scmd->newname, NORMALSIZE, "%s", p);
            else if(!strcmp(scmd->cmd_name,"move"))
                snprintf(scmd->oldname, NORMALSIZE, "%s", p);
            break;
        default:
            break;
        }
        i++;
        p=strtok(NULL,split);
    }


    if(scmd->cmd_name[0] == '\0' || scmd->path[0] == '\0')
    {
        printf("socket cmd is not math cmd\\npath\\nname format\n");
        return -1;
    }

    return 0;
}
#endif

int perform_socket_cmd(Socket_cmd *scmd)
{
    char cmd_name[64];
    char cmd_param[512];
    char path[512];
    char temp[512];
    char filename[NORMALSIZE] ={0};
    char fullname[NORMALSIZE];
    char oldname[NORMALSIZE],newname[NORMALSIZE],oldpath[NORMALSIZE];
    char action[64];
    char cmp_name[512];
    long long int parent_ID = -10;
    long long int entry_ID;
    Propfind *find = NULL;
    Createfolder *createfolder = NULL;
    Operateentry *oe = NULL;
    Moveentry *me = NULL;
    struct sync_item *item = NULL;
    int status = -10;
    int res_value = -10;
    int isdelete = 0 ;
    char type[64] = {0};

    memset(cmd_name, 0, sizeof(cmd_name));
    memset(cmd_param, 0, sizeof(cmd_param));
    memset(path,0,sizeof(path));
    memset(temp,0,sizeof(temp));
    memset(oldname,0,sizeof(oldname));
    memset(newname,0,sizeof(newname));
    memset(oldpath,0,sizeof(oldpath));
    memset(fullname,0,sizeof(fullname));
    memset(action,0,sizeof(action));
    memset(cmp_name,0,sizeof(cmp_name));

    snprintf(cmd_name, 64, "%s", scmd->cmd_name);
    snprintf(path, 512, "%s", scmd->path);

    if(!strcmp(cmd_name,"rename"))
    {
        snprintf(oldname, NORMALSIZE, "%s", scmd->oldname);
        snprintf(newname, NORMALSIZE, "%s", scmd->newname);
    }
    else if(!strcmp(cmd_name,"move"))
    {
        snprintf(oldpath, NORMALSIZE, "%s", scmd->oldpath);
        snprintf(oldname, NORMALSIZE, "%s", scmd->oldname);
    }
    else
    {
        snprintf(filename, NORMALSIZE, "%s", scmd->filename);
    }

    //add by alan
    if( strcmp(cmd_name, "copyfile") == 0 )
    {
        //  2013/7/25 change copying way
        snprintf(fullname, NORMALSIZE, "%s/%s",path,filename);
        add_sync_item("copyfile",fullname,copy_file_list_head);

        return 0;
    }

#if 1

    if( !strcmp(cmd_name,"rename") )
        snprintf(cmp_name,512,"%s/%s",path,newname);
    else
        snprintf(cmp_name,512,"%s/%s",path,filename);

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        snprintf(action, 64, "%s", "createfile");
        struct sync_item* item;

        item = get_sync_item("copyfile",cmp_name,copy_file_list_head);

        if(item != NULL)
        {
            del_sync_item("copyfile",cmp_name,copy_file_list_head);
        }
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        snprintf(action, 64, "%s", "remove");
        isdelete = 1;
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        snprintf(action, 64, "%s", "createfolder");
    }
    else if( strcmp(cmd_name, "rename") == 0 )
    {
        snprintf(action, 64, "%s", "rename");
    }

    struct timespec timeout;
    timeout.tv_sec = time(NULL) + 3;
    timeout.tv_nsec = 0;

    if(from_server_sync_head->next != NULL)
    {
    	pthread_mutex_lock(&my_mutex.mutex);

        item = get_sync_item(action,cmp_name,from_server_sync_head);

        if(item != NULL)
        {
#ifdef DEBUG
            printf("##### %s %s by asuswebstorage self ######\n",action,cmp_name);
#endif
            del_sync_item(action,cmp_name,from_server_sync_head);
            pthread_mutex_unlock(&my_mutex.mutex);

            return 0;
        }

        pthread_mutex_unlock(&my_mutex.mutex);
        item = NULL;
    }
    has_local_socket = 1;

    /* first make sure that the token is expired */
    if(is_check_token)
    {
        Getmysyncfolder *gf;
        gf = getMySyncFolder(username);
        if(NULL == gf)
        {
            check_network_state();
            return -1;
        }
        if(gf->status == S_AUTH_FAIL)
        {
            status = getToken(username,password,"",0);

            if(status != 0)
            {
                if( status != S_NETWORK_FAIL)
                    exit_loop = 1;
                my_free(gf);
                return -1;
            }
        }
        my_free(gf);
    }

    parent_ID = getParentID(path);

    if(parent_ID == -1)
    {
        return -1;
    }
    if(parent_ID == -2) //parent dir has del from server
    {
        if(isdelete)
            return 0;

        parent_ID = create_server_folder_r(path);
        if(parent_ID == -1)
            return -1;

    }

#ifdef DEBUG
    printf("###### %s is start ######\n",cmd_name);
#endif

    if( strcmp(cmd_name, "createfile") == 0 || strcmp(cmd_name, "dragfile") == 0 )
    {
        snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
#if TEST
        sleep(30);
        printf("real upload start\n");
#endif
        status = uploadFile(fullname,parent_ID,NULL,0);

#if TREE_NODE_ENABLE
        modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif

        if(status != 0)
        {
#ifdef DEBUG
            printf("upload %s failed\n",fullname);
#endif
            res_value = handle_upload_fail_code(status,parent_ID,fullname,path);

            return res_value;
        }
    }
    else if( strcmp(cmd_name, "delete") == 0  || strcmp(cmd_name, "remove") == 0 || strcmp(cmd_name, "modify") == 0 ||
             strcmp(cmd_name, "rename") == 0 )
    {
        int isfolder;
        int ischildonly;

        if( strcmp(cmd_name, "rename") == 0 )
            find = checkEntryExisted(username,parent_ID,oldname,"system.unknown");
        else
            find = checkEntryExisted(username,parent_ID,filename,"system.unknown");

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

        entry_ID = find->id;
        snprintf(type, 64, "%s", find->type);
        my_free(find);

        if(strcmp(type,"system.folder") == 0)
        {
            isfolder = 1;
            ischildonly = 0;
        }
        else if(strcmp(type,"system.file") == 0)
        {
            isfolder = 0;
            ischildonly = 0;
        }

        if( strcmp(cmd_name, "rename") == 0 )
        {
            snprintf(fullname,NORMALSIZE,"%s/%s",path,oldname);
            if(test_if_file_up_excep_fail(fullname))
                del_sync_item("up_excep_fail",fullname,up_excep_fail);
            if(strcmp(type,"system.notfound") == 0)
            {
                snprintf(fullname,NORMALSIZE,"%s/%s",path,newname);
                res_value = upload_entry(fullname,parent_ID,path);
                return res_value;
            }
            else
            {
#if TEST
                printf("renameEntry() start\n");
                printf("wait 40 start\n");
                sleep(30);
                printf("rename start\n");
#endif
                oe = renameEntry(username,entry_ID,0,newname,isfolder) ;
#if TREE_NODE_ENABLE
                rename_update_tree(oldname,newname);
#endif

                if(NULL == oe)
                {
                    printf("operate rename failed\n");
                    return -1;
                }

                if( oe->status != 0 )
                {
                    handle_error(oe->status,cmd_name);
                    snprintf(fullname,NORMALSIZE,"%s/%s",path,newname);
                    res_value = handle_rename_fail_code(oe->status,parent_ID,fullname,path,isfolder);
                    my_free(oe);
                    return res_value;
                }

                my_free(oe);

            }

        }
        else if( strcmp(cmd_name, "modify") == 0 )
        {

            if(!strcmp(type,"system.notfound"))
            {
                printf("del item has not exist server\n");
                entry_ID = 0 ;
            }

            snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
            status = uploadFile(fullname,parent_ID,NULL,entry_ID);

#if TREE_NODE_ENABLE
            modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif

            if(status != 0)
            {
                printf("upload %s failed\n",fullname);
                res_value = handle_upload_fail_code(status,parent_ID,fullname,path);
                return res_value;
            }
        }
        else
        {
#if TEST
            printf("wait 40 start\n");
            sleep(30);
            printf("del start\n");
#endif
            snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
            if(test_if_file_up_excep_fail(fullname))
                del_sync_item("up_excep_fail",fullname,up_excep_fail);
            if(!strcmp(type,"system.notfound"))
            {
                printf("del item has not exist server\n");
                return 0;
            }

            oe = removeEntry(username,entry_ID,ischildonly,isfolder,parent_ID);

#if TREE_NODE_ENABLE
            snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
            modify_tree_node(fullname,DirRootNode,DEL_TREE_NODE);
#endif

            if(NULL == oe)
            {
                printf("operate rename failed\n");
                return -1;
            }
            if( oe->status != 0 )
            {
                handle_error(oe->status,cmd_name);
                res_value = handle_delete_fail_code(oe->status);
                my_free(oe);
                return res_value;
            }
            my_free(oe);
        }

    }
    else if(strcmp(cmd_name, "dragfolder") == 0 || strcmp(cmd_name, "createfolder") == 0 )
    {
        snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);

#if TREE_NODE_ENABLE
        modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif

#if TEST
        printf("wait 40 start\n");
        sleep(30);
        printf("create folder start\n");
#endif
        createfolder = createFolder(username,parent_ID,0,fullname);


        if(NULL == createfolder)
            return -1;
        else if( createfolder->status != 0 )
        {
            handle_error(createfolder->status,"createfolder");
            res_value = handle_createfolder_fail_code(createfolder->status,parent_ID,path,fullname);
            my_free(createfolder);
            return res_value;
        }
        else
        {
            entry_ID = createfolder->id;
            my_free(createfolder);
            if(!strcmp(cmd_name,"dragfolder"))
            {
                res_value = sync_all_item(fullname,entry_ID);
                if(res_value == S_UPLOAD_DELETED)
                    return 0;
                else
                    return res_value;
            }

        }
    }
    else if( strcmp(cmd_name, "move") == 0 )
    {
#if TREE_NODE_ENABLE
        char del_name[NORMALSIZE];
        char insert_name[NORMALSIZE];

        memset(del_name,0,sizeof(del_name));
        memset(insert_name,0,sizeof(insert_name));

        snprintf(del_name,NORMALSIZE,"%s/%s",oldpath,oldname);
        snprintf(insert_name,NORMALSIZE,"%s/%s",path,oldname);

        modify_tree_node(del_name,DirRootNode,TREE_DEL_NODE);
        modify_tree_node(insert_name,DirRootNode,ADD_TREE_NODE);

#endif
        long long int pre_parent_ID;
        int isfolder;

        pre_parent_ID = getParentID(oldpath);
        if(pre_parent_ID == -1)
            return -1;
        find = checkEntryExisted(username,pre_parent_ID,oldname,"system.unknown");

        if(NULL == find)
            return -1;

        if( find->status != 0 )
        {
            handle_error(find->status,"propfind");
            my_free(find);
            return -1;
        }
        else
        {
            snprintf(fullname,NORMALSIZE,"%s/%s",oldpath,oldname);
            if(test_if_file_up_excep_fail(fullname) == 1)
                del_sync_item("up_excep_fail",fullname,up_excep_fail);
            if(!strcmp(find->type,"system.notfound"))
            {
                snprintf(fullname,NORMALSIZE,"%s/%s",path,oldname);
                res_value = upload_entry(fullname,parent_ID,path);
                my_free(find);
                return res_value;
            }
            else
            {
                entry_ID = find->id;
                if(strcmp(find->type,"system.folder") == 0)
                {
                    isfolder = 1;
                }
                else if(strcmp(find->type,"system.file") == 0)
                {
                    isfolder = 0;
                }

                my_free(find);

#if TEST
                printf("wait 40 start\n");
                sleep(30);
                printf("move start\n");
#endif
                me = moveEntry(username,entry_ID,oldname,parent_ID,isfolder,pre_parent_ID);

                if(NULL == me)
                {
                    printf("operate move failed\n");
                    return -1;
                }
                else if( me->status != 0 )
                {
                    handle_error(me->status,cmd_name);
                    snprintf(fullname,NORMALSIZE,"%s/%s",path,oldname);
                    res_value = handle_move_fail_code(me->status,path,fullname,parent_ID,oldpath,entry_ID,isfolder);
                    my_free(me);
                    return res_value;
                }

                my_free(me);
            }
        }
    }


#ifdef DEBUG
    printf("###### %s is ending ######\n",cmd_name);
#endif

#if SYSTEM_LOG
    write_system_log(cmd_name,filename);
#endif

    return 0;

#endif
}

int cmd_parser(char *command)
{
    Socket_cmd scmd;
    int res;

    memset(&scmd,0,sizeof(scmd));

    res = split_socket_cmd(command,&scmd);

    if(res == -1)
        return 0;

    if(download_only == 1 )
        res = download_only_add_socket_item(&scmd);
    else
    {
        res = perform_socket_cmd(&scmd);
        if(res != 0)
            is_check_token = 1;
        else
            is_check_token = 0;
    }

    return res;
}

int send_action(int type, char *content)
{
    int sockfd;
    char str[1024] = {0};
    int port;

    if(type == 1)
        port = 3678;
    else if(type == 2)
        port = MYPORT;

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

    snprintf(str, 1024, "0@%s",content);
    if (send(sockfd, str, strlen(str), 0) == -1) {
        perror("send");
        return -1;
    }
    else
    {
        printf("send content is %s\n",str);
    }

    close(sockfd);
    return 0;
}



void clean_global_var()
{
    memset(mount_path,0,sizeof(mount_path));
    memset(base_path, 0, sizeof(base_path));
    memset(cloud_path, 0, sizeof(cloud_path));
    memset(asus_path, 0, sizeof(asus_path));
    memset(log_path, 0, sizeof(log_path));
    memset(xml_path, 0, sizeof(xml_path));
    memset(system_log, 0, sizeof(system_log));
    memset(general_log,0,sizeof(general_log));
    memset(confilicted_log,0,sizeof(confilicted_log));
    memset(gateway_xml, 0, sizeof(gateway_xml));
    memset(token_xml, 0, sizeof(token_xml));
    memset(get_info_xml, 0, sizeof(get_info_xml));
    memset(get_sync_folder_xml, 0, sizeof(get_sync_folder_xml));
    memset(get_personal_system_folder_xml,0,sizeof(get_personal_system_folder_xml));
    memset(browse_folder_xml, 0, sizeof(browse_folder_xml));
    memset(propfind_xml, 0, sizeof(propfind_xml));
    memset(create_folder_xml, 0, sizeof(create_folder_xml));
    memset(rename_xml, 0, sizeof(rename_xml));
    memset(move_xml, 0, sizeof(move_xml));
    memset(remove_xml, 0, sizeof(remove_xml));
    memset(update_xml, 0, sizeof(update_xml));
    memset(get_entry_info_xml, 0, sizeof(get_entry_info_xml));
    memset(get_change_seq_xml, 0, sizeof(get_change_seq_xml));
    memset(init_upload_xml, 0, sizeof(init_upload_xml));
    memset(resume_upload_xml, 0, sizeof(resume_upload_xml));
    memset(finish_upload_xml, 0, sizeof(finish_upload_xml));
    memset(trans_excep_file,0,sizeof(trans_excep_file));
    memset(system_token,0,sizeof(system_token));
}


void init_global_var()
{
    char config_path[256] = {0};
    char script_path[256] = {0};
    char temp_path[256] = {0};
    char cert_path[256] = {0};
    snprintf(cloud_path,NAMESIZE,"%s/%s",base_path,"smartsync");
    snprintf(log_path,NAMESIZE,"%s/%s",cloud_path,".logs");
    snprintf(asus_path,NAMESIZE,"%s/%s",cloud_path,"asuswebstorage");
    snprintf(config_path,256,"%s/config",asus_path);
    snprintf(script_path,256,"%s/script",asus_path);
    snprintf(temp_path,256,"%s/temp",asus_path);
    snprintf(cert_path,256,"%s/cert",asus_path);
    snprintf(xml_path,NAMESIZE,"%s/xml",temp_path);


    snprintf(general_log,NAMESIZE,"%s/asuswebstorage",log_path);
    snprintf(system_log,NAMESIZE,"%s/system.log",log_path);
    snprintf(gateway_xml,NAMESIZE,"%s/%s",xml_path,"gateway.xml");
    snprintf(get_user_state_xml,NAMESIZE,"%s/%s",xml_path,"get_user_state.xml");
    snprintf(token_xml,NAMESIZE,"%s/%s",xml_path,"token.xml");
    snprintf(get_info_xml,NAMESIZE,"%s/%s",xml_path,"get_info.xml");
    snprintf(get_sync_folder_xml,NAMESIZE,"%s/%s",xml_path,"get_sync_folder.xml");
    snprintf(get_personal_system_folder_xml,NAMESIZE,"%s/%s",xml_path,"get_personal_system_folder.xml");
    snprintf(browse_folder_xml,NAMESIZE,"%s/%s",xml_path,"browse_folder.xml");
    snprintf(propfind_xml,NAMESIZE,"%s/%s",xml_path,"propfind.xml");
    snprintf(create_folder_xml,NAMESIZE,"%s/%s",xml_path,"create_folder.xml");
    snprintf(rename_xml,NAMESIZE,"%s/%s",xml_path,"rename.xml");
    snprintf(move_xml,NAMESIZE,"%s/%s",xml_path,"move.xml");
    snprintf(remove_xml,NAMESIZE,"%s/%s",xml_path,"remove.xml");
    snprintf(update_xml,NAMESIZE,"%s/%s",xml_path,"update.xml");
    snprintf(get_entry_info_xml,NAMESIZE,"%s/%s",xml_path,"get_entry_info.xml");
    snprintf(get_change_seq_xml,NAMESIZE,"%s/%s",xml_path,"get_change_seq.xml");
    snprintf(init_upload_xml,NAMESIZE,"%s/%s",xml_path,"init_upload.xml");
    snprintf(resume_upload_xml,NAMESIZE,"%s/%s",xml_path,"resume_upload.xml");
    snprintf(finish_upload_xml,NAMESIZE,"%s/%s",xml_path,"finish_upload.xml");
    snprintf(trans_excep_file,128,"%s/%s",log_path,"asuswebstorage_errlog");

    my_mkdir(cloud_path);
    my_mkdir(log_path);
    my_mkdir(asus_path);
    my_mkdir(config_path);
    my_mkdir(script_path);
    my_mkdir(temp_path);
    my_mkdir(cert_path);
    my_mkdir(xml_path);
    remove(trans_excep_file);
}

void clean_up()
{
    printf("enter clean up\n");
    remove("/tmp/smartsync/.logs/asuswebstorage");
    check_accout_status();
#if 0
    if( !item_empty(up_head))
    {
        printf("write uncomplete upload item to file\n");
        print_sync_item(up_head,UPLOAD);
    }

    if( !item_empty(down_head))
    {
#if WRITE_DOWN_TEMP_FILE
        remove(down_item_temp_file);
#endif
        printf("write uncomplete download item to file\n");
        print_sync_item(down_head,DOWNLOAD);
    }

    if( !item_empty(up_excep_fail))
    {
        printf("write up excep fail item to file\n");
        print_sync_item(up_excep_fail,UP_EXCEP_FAIL);
    }
#endif

    free_sync_item(up_head);
    free_sync_item(down_head);
    free_sync_item(from_server_sync_head);
    free_sync_item(up_excep_fail);
    queue_destroy(SocketActionList);
    free_sync_item(copy_file_list_head);
    if(download_only == 1)
        free_sync_item(download_only_socket_head);

#if TREE_NODE_ENABLE
    write_tree_to_file(tree_log,DirRootNode);
    free_tree_node(DirRootNode);
#endif

#if MEM_POOL_ENABLE
    mem_pool_destroy();
#endif

    printf("clean up end !!!\n");
}

void str_to_lower(char *word)
{
    int i;
    int len=strlen(word);
    for(i=0;i<len;i++)
    {
        if(word[i] >='A' && word[i] <= 'Z')
        {
            word[i] += 32;

        }
    }
}


void run()
{
    error_flag = 0;
    restart_run = 0;
    int status;
    int auth_ok = 0;
    char *pwd = NULL;
    char pword[256];

    snprintf(pword, 256, "%s", password);
    str_to_lower(pword);
    pwd = MD5_string(pword);
    memset(password,0,sizeof(password));
    snprintf(password,256,"%s",pwd);
    my_free(pwd);

    printf("username is %s,pwd is %s,sync path is %s\n",username,password,sync_path);

    memset(&sergate,0,sizeof(Servicegateway));
    memset(&aaa,0,sizeof(Aaa));

    if(obtain_token_from_file(system_token,&aaa) == -1)
    {
        if(obtainGateway(username,password,&sergate) == -1)
            return;

     #ifdef DEBUG
         printf("getServiceGateway ok\n");
     #endif

        if(obtainToken(username,password,&cfg,1) == -1)
            return;

     #ifdef DEBUG
         printf("GetToken ok\n");
     #endif
    }

   auth_ok = 1;

   MySyncFolder = obtainSyncRootID(username);
   if(MySyncFolder == -1)
       return ;

#ifdef DEBUG
    printf("getMySyncFolder ok\n");
#endif

    max_upload_filesize = get_max_upload_filesize(username);
    if(max_upload_filesize == -1)
        return ;

    status = CheckUserState(username,sergate.gateway);
    if(status == -1 || status == S_ACCOUNT_CLOSE)
           return;

    if(status == S_ACCOUNT_FROZEN)
         IsAccountFrozen = 1;

    MyRecycleID = GetMyRecycleID(username,"MyRecycleBin");
    if(MyRecycleID == -1)
        return;

#ifdef DEBUG
    printf("##### max upload filesize is %lldMB ####\n",max_upload_filesize);
#endif

    if(auth_ok == 1)
    {
#if SERVER_TREE
        SRootNode = create_server_treeroot(gf->id);
        browse_to_tree(username,gf->id,browse_folder_xml,SRootNode);
        SearchServerTree(SRootNode);
#endif
        //create mem pool
#if MEM_POOL_ENABLE
        mem_pool_init();
#endif

        from_server_sync_head = create_head();
        up_head = create_head();
        down_head =  create_head();
        up_excep_fail = create_head();
        copy_file_list_head = create_head();
        if(download_only == 1)
        {
            download_only_socket_head = create_head();
            download_only_modify_head = create_head();
        }

        if(NULL == from_server_sync_head || NULL == up_head ||
           NULL == up_excep_fail)
            return;

        if(download_only == 1)
        {
            if(NULL == download_only_socket_head || NULL == download_only_modify_head)
                return;
        }

        printf("MySyncFolder is %d\n",MySyncFolder);

#if WRITE_DOWNLOAD_TEMP_FILE
        if(access(down_item_temp_file,0) == 0)
        {
            parse_trans_item(down_item_temp_file,DOWNLOAD);
            remove(down_item_temp_file);
        }
#endif

#if 0
        if(access(up_item_file,0) == 0)
        {
            parse_trans_item(up_item_file,UPLOAD);

            if(remove(up_item_file) == -1)
            {
                printf("remove %s fail\n",up_item_file);
            }
        }

        if(access(down_item_file,0) == 0)
        {
            parse_trans_item(down_item_file,DOWNLOAD);
            if(remove(down_item_file) == -1)
            {
                printf("remove %s fail\n",down_item_file);
            }
        }

        if(access(up_excep_fail_file,0) == 0)
        {
            init_up_excep_fail(up_excep_fail_file);
        }
#endif

#if TREE_NODE_ENABLE
        if(access(tree_log,0) == 0)
        {
            DirRootNode = read_file_to_tree(tree_log);
            remove(tree_log);
        }
        else
        {
            DirRootNode = create_tree_rootnode(sync_path);
            FindDir(DirRootNode,sync_path);
        }
#endif
        //write_log(S_SYNC,"","");//fix local path is blank can't show server space bug

        send_action(1,sync_path);

        /*add by alan*/
        SocketActionList = queue_create();

        //write_log(S_SYNC,"","");

        if(set_iptables(1))
            exit(-1);

        if( pthread_create(&local_thread,NULL,(void *)SyncLocal,NULL) != 0)
        {
            printf("thread creation failder\n");
            return;
        }

        enter_sleep_time(1000*500,NULL);

            if( pthread_create(&socket_thread,NULL,(void *)Socket_Parser,NULL) != 0)
            {
                printf("thread creation failder\n");
                return;
            }

            enter_sleep_time(1000*500,NULL);

        if(upload_only != 1)
        {
            if( pthread_create(&server_thread,NULL,(void *)SyncServer,NULL) != 0)
            {
                printf("thread creation failder\n");
                return;
            }
            enter_sleep_time(1000*500,NULL);
        }

        pthread_join(local_thread,NULL);
        pthread_join(socket_thread,NULL);
        if(!upload_only)
            pthread_join(server_thread,NULL);
    }

    enter_sleep_time(1000*500,NULL);
    clean_up();

    if(stop_progress != 1)
    {
#ifdef IPKG
        do
        {
            printf("##########enter loop check token file\n");
            if(disk_change)
            {
                sync_disk_removed = check_token_file(&cfg);
                pthread_mutex_lock(&mutex_socket);
                disk_change = 0;
                pthread_mutex_unlock(&mutex_socket);
                if(sync_disk_removed == 0)
                {
                    printf("sync disk exists\n");
                }
                else if(sync_disk_removed == 2)
                {
                    printf("sync disk unmount\n");
                }
            }
            else
            {
                enter_sleep_time(1000*300,NULL);
            }

        }while(sync_disk_removed == 2);
        sync_disk_removed = 0;

        printf("stop_progress is %d \n",stop_progress);
#endif
        if(stop_progress != 1)
        {
            exit_loop = 0;
            if(read_config() != -1)
                run();
        }
    }
}

char  *get_mount_path(char *path , int n)
{
    int i;
    char *m_path = NULL;
    m_path = (char *)malloc(sizeof(char)*NORMALSIZE);
    memset(m_path,0,NORMALSIZE);

    char *new_path = NULL;
    new_path = path;

    for(i= 0;i< n ;i++)
    {
        new_path = strchr(new_path,'/');
        if(new_path == NULL)
            break;
        new_path++;
    }

    if( i > 3)
        strncpy(m_path,path,strlen(path)-strlen(new_path)-1);
    else
        snprintf(m_path, sizeof(char)*NORMALSIZE, "%s", path);

    printf("mount path is [%s]\n",m_path);

    return m_path;
}



int read_config()
{
    char resume_path[NORMALSIZE];
    upload_only = 0;
    download_only = 0;
    memset(resume_path,0,sizeof(resume_path));

#ifdef IPKG 
    if(create_asuswebstorage_conf_file(CONFIG_PATH) == -1)
    {
        printf("create_asuswebstorage_conf_file fail\n");
        return;
    }
#else
    if(convert_nvram_to_file(CONFIG_PATH) == -1)
    {
        printf("convert_nvram_to_file fail\n");
        return -1;
    }
#endif

    memset(up_item_file, 0, sizeof(up_item_file));
    memset(down_item_file, 0, sizeof(down_item_file));
#if WRITE_DOWNLOAD_TEMP_FILE
    memset(down_item_temp_file, 0, sizeof(down_item_temp_file));
#endif
    memset(up_excep_fail_file,0,sizeof(up_excep_fail_file));
    memset(confilicted_log,0,sizeof(confilicted_log));
    memset(&cfg,0,sizeof(struct asus_config));
    parse_config_new(CONFIG_PATH,&cfg);

    if( strlen(username) == 0 )
    {
#ifdef DEBUG
        printf("username is blank ,please input your username and passwrod\n");
#endif

#if SYSTEM_LOG
        write_system_log("error","username is blank ,please input your username and passwrod");
#endif
        no_config = 1;
    }

    if(no_config)
        return -1;

    if(!no_config)
    {
        parse_sync_path(&cfg,sync_path);
#ifdef IPKG
        memset(record_token_file,0,sizeof(record_token_file));
        memset(token_filename,0,sizeof(token_filename));
        snprintf(token_filename, 256, ".__smartsync_0_%s_%s",username,cfg.dir_name);
        snprintf(record_token_file, 256, "/opt/etc/.smartsync/asuswebstorage_%s",cfg.user);

        int have_log = 0;

        sync_disk_removed = check_token_file(&cfg);

        printf("disk status is %d \n",sync_disk_removed);
        while(sync_disk_removed == 2) //sync disk unmount
        {
            if(!have_log)
            {
                write_log(S_ERROR,"sync disk unmount","");
                printf("write log end\n");
                have_log = 1;
            }
            if(disk_change)
            {
                printf("enter check tokenf file\n");
                pthread_mutex_lock(&mutex_socket);
                disk_change = 0;
                pthread_mutex_unlock(&mutex_socket);
                sync_disk_removed = check_token_file(&cfg);
            }
            else
               //enter_sleep_time(1);
                //usleep(1000*300);
                enter_sleep_time(1000*300,NULL);
        }
#endif
            if(my_mkdir_r(sync_path) == -1)
            {
               printf("create sync_path=%s fail,asuswebstorage exit\n",sync_path);
               write_log(S_ERROR,"Create sync path fail,please check whether the hard disk is read only mode","");
               exit(-1);
            }

            DIR *dir = opendir(mount_path);
            if(NULL == dir)
            {
                printf("open %s fail,asuswebstorage exit\n",mount_path);
                exit(-1);
            }
            closedir(dir);

            snprintf(resume_path,NORMALSIZE,"%s/.smartsync/asuswebstorage",mount_path);
            my_mkdir_r(resume_path);

            snprintf(up_item_file,NAMESIZE,"%s/%s_up_item",resume_path,username);
            snprintf(down_item_file,NAMESIZE,"%s/%s_down_item",resume_path,username);
    #if WRITE_DOWNLOAD_TEMP_FILE
            snprintf(down_item_temp_file, NAMESIZE, "%s/%s_down_temp_item",resume_path,username);
    #endif
            snprintf(up_excep_fail_file,NAMESIZE,"%s/%s_up_excep_fail",resume_path,username);
            snprintf(confilicted_log,NAMESIZE,"%s/confilicted.log",resume_path);
            snprintf(system_token,256,"%s/sys_token",resume_path);

        no_config = 0 ;
        exit_loop = 0;
    }
    return 0;
}


int retry_all_fail_item(int type)
{
    if(upload_only)
        has_socket = 1;
    struct sync_item *point,*p2;
    Transitem *item;
    char content[NORMALSIZE];
    int retry_time = 1;
    int status;
    char path[NORMALSIZE];
    int ok;

    memset(content,0,sizeof(content));
    memset(path,0,sizeof(path));

    if(type == UP_FAIL)
        point = up_head->next;

    while(point != NULL)
    {
        p2 = point;
        point = point->next;

        switch (type)
        {
        case UP_FAIL:
            snprintf(content,NORMALSIZE,"%s,%s",p2->action,p2->name);
            item = parse_trans_item_from_buffer(content,UPLOAD);
            struct stat buf;

            if(NULL == item)
            {
                return -1;
            }

            if(item->id > 0)
            {
                if( stat(item->name,&buf) == -1)
                {
                    printf(" %s file does not exist\n",item->name);
                    memset(content,0,sizeof(content));
                    strncpy(content,p2->name,NORMALSIZE);
                    del_sync_item(p2->action,p2->name,up_head); 
                    del_sync_item("up_excep_fail",content,up_excep_fail);
                    my_free(item);
                    return -1;
                }
#ifdef DEBUG
                printf("##### handle upload fail file is %s,parentid id %lld,transid is %s#####\n",item->name,item->id,item->transid);
#endif
                while(retry_time)
                {
                    //2015/1/9 fix file can't upload when transe id is not exist,set trans_id=NULL
                    status = uploadFile(item->name,item->id,NULL,0);
                    if(status != 0)
                    {
                        char *p = strrchr(item->name,'/');
                        if(p)
                        {
                            strncpy(path,item->name,strlen(item->name)-strlen(p));
                            status = handle_upload_fail_code(status,item->id,item->name,path);
                        }
                    }
                    if(status == 0)
                    {
                        ok = 1;
                        break;
                    }
                    else
                    {
                        retry_time--;
                    }
                }
            }
            else
            {
                del_sync_item(p2->action,p2->name,up_head);
            }
            my_free(item);
            break;
#if 0
        case DOWN_FAIL:
            snprintf(content,NORMALSIZE,"%s,%s",p2->action,p2->name);
            item = parse_trans_item_from_buffer(content,DOWNLOAD);

            if(item->id > 0)
            {
                filename = parse_name_from_path(item->name);

                if(NULL == filename)
                {
                    printf("parse %s filenam fail\n",item->name);
                    my_free(item);
                    return -1;
                }
                strncpy(path,item->name,strlen(item->name)-strlen(filename)-1);
                parentID = getParentID(path);

                if(parentID <= 0)
                {
                    printf("obtain %s parent ID is fai\n",item->name);
                    my_free(filename);
                    my_free(item);
                    return -1;
                }

                find = checkEntryExisted(username,parentID,filename,"system.file");

                if(NULL == find)
                {
                    printf("find prop failed\n");
                    my_free(filename);
                    my_free(item);
                    return -1;
                }
                else if(find->status != 0)
                {
                    handle_error(find->status,"profind");
                    my_free(filename);
                    my_free(item);
                    my_free(find);
                    return -1;
                }
                else if(!strcmp(find->type,"system.notfound"))
                {
                    del_sync_item(p2->action,p2->name,down_head);
                    my_free(filename);
                    my_free(item);
                    my_free(find);
                    return 0;
                }
                else if(!strcmp(find->type,"system.file"))
                {
                    if( find->status != 0 )
                    {
                        handle_error(find->status,"propfind");
                        my_free(find);
                        my_free(item);
                        return -1;
                    }
                    if(!strncmp(find->type,"system.file",sizeof("system.file")))
                    {
                        while(retry_time)
                        {
                            status = downloadFile(item->id,item->name,item->size,0);
                            if(status == 0)
                            {
                                ok = 1;
                                break;
                            }
                            else
                            {
                                printf("##### download %s fail #####\n",item->name);
                                retry_time--;
                            }
                        }
                    }
                }

                my_free(filename);
                my_free(item);
                my_free(find);
            }
            break;
#endif
              case SOCKET_FAIL:
            break;
              default:
            break;
        }
    }
    return 0;
}

void handle_fail_item()
{
#if 0
    if(!item_empty())
    {
#ifdef DEBUG
        printf("######## handle downoad fail########\n");
#endif
        retry_all_fail_item(DOWN_FAIL);
    }
#endif

    if(!item_empty(up_head))
    {
#ifdef DEBUG
        printf("######## handle up item fail########\n");
#endif
        retry_all_fail_item(UP_FAIL);
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
            printf("sigwait() fetch the signal - %d\n", sig);
            sig_handler(sig);
        } else {
            printf("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
        }
    }
}

#ifdef IPKG
int write_notify_file(char *path,int signal_num)
{
    FILE *fp;
    char fullname[64];
    memset(fullname,0,sizeof(fullname));

    my_mkdir("/tmp/notify");
    my_mkdir("/tmp/notify/usb");
    snprintf(fullname, 64, "%s/asuswebstorage",path);
    fp = fopen(fullname,"w");
    if(NULL == fp)
    {
        printf("open notify %s file fail\n",fullname);
        return -1;
    }
    fprintf(fp,"%d",signal_num);
    fclose(fp);
    return 0;
}
#endif
