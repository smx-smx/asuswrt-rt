#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/inotify.h>
#include <signal.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include "event_queue.h"
#include "inotify_utils.h"

//#define inotify_folder "../inotify_file"
#define MYPORT 3678
#define BACKLOG 10 /* max listen num*/
#define PC 0
//#define CONFIG_GER_PATH "/home/gauss/asuswebstorage/etc/Cloud.conf"
//#define CONFIG_GER_PATH "/opt/etc/Cloud.conf"
//#define DEBUG 1
//int keep_running;
//int need_restart_inotify;
//int add_new_watch_item;
int dragfolder_wd;
//int have_from_file;
//int have_from_file_ex;
//int is_modify;
//Folders allfolderlist;     //del by alan
//Paths pathlist;            //del by alan
//char pathlist[][256];
//#if PC
//char *base_path = "/home/gauss/optware_bulid";
//#else
//char base_path[256];
//#endif


//char inotify_path[256];
//char moved_from_file[256];
//char moved_to_file[256];
int ready = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



//int get_all_folders(const char *dirname,Folders *allfolderlist,int offset,int type);
void signal_handler (int signum);
int inotifyStart();
void watch_socket();
void watch_folder();
void cmd_parser(char *path);
void initConfig(char *path);
void initConfig2(char *path);
void my_mkdir_r(char *path);
char  *get_mount_path(char *path , int n);
//long pre_event;
//long pre_pre_event;

#define SHELL_FILE  "/tmp/smartsync/script/write_nvram"

void my_mkdir(char *path)
{
    //char error_message[256];
    if(NULL == opendir(path))
    {
        if(-1 == mkdir(path,0777))
        {
            printf("mkdir %s fail",path);
            //handle_error(S_MKDIR_FAIL,error_message);
            exit(-1);
        }
    }
}

void my_mkdir_r(char *path)
{
    int i,len;
    char str[512];

    strncpy(str,path,512);
    len = strlen(str);
    for(i=0; i < len ; i++)
    {
        if(str[i] == '/' && i != 0)
        {
            str[i] = '\0';
            if(access(str,F_OK) != 0)
            {
                my_mkdir(str);
            }
            str[i] = '/';
        }
    }

    if(len > 0 && access(str,F_OK) != 0)
    {
        my_mkdir(str);
    }

}

int create_shell_file(){

    FILE *fp;
    char contents[256];
    memset(contents,0,256);
    strcpy(contents,"#! /bin/sh\nnvram set $2=\"$1\"\nnvram commit");

    if(( fp = fopen(SHELL_FILE,"w"))==NULL)
    {
        fprintf(stderr,"create shell file error");
        return -1;
    }

    fprintf(fp,"%s",contents);
    fclose(fp);
    return 0;

}

void* sigmgr_thread(){
    sigset_t   waitset;
    //siginfo_t  info;
    int        sig;
    int        rc;
    pthread_t  ppid = pthread_self();

    pthread_detach(ppid);

    sigemptyset(&waitset);
    sigaddset(&waitset,SIGTERM);

    while (1)  {
        rc = sigwait(&waitset, &sig);
        if (rc != -1) {
#ifdef MYDEBUG
            printf("sigwait() fetch the signal - %d\n", sig);
#endif
            signal_handler(sig);
        } else {
            printf("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
        }
    }
}

int main(int argc, char *argv[])
{
    exit_loop = 0;

    my_mkdir("/tmp/smartsync");
    my_mkdir("/tmp/smartsync/script");
    create_shell_file();

    //pre_event = -1;
    //pre_pre_event = -1;

    create_file_list = create_list_head();

    allfolderlist = (Folder *)malloc(sizeof(Folder));
    allfolderlist->name = NULL;
    allfolderlist->next = NULL;
    allfolderlist_tail = allfolderlist;
    //create pathlist !!!
    pathlist = (Folder *)malloc(sizeof(Folder));
    pathlist->name = NULL;
    pathlist->next = NULL;
    pathlist_tail = pathlist;

    inotify_fd = open_inotify_fd ();
    pthread_mutex_init(&mutex_allfolderlist, NULL);
    pthread_mutex_init(&mutex_inotify_fd, NULL);
    pthread_mutex_init(&mutex_pathlist, NULL);
    dragfolder_wd = 0;

    sigset_t bset,oset;
    pthread_t sig_thread;
    sigemptyset(&bset);
    sigaddset(&bset,SIGTERM);

    if( pthread_sigmask(SIG_BLOCK,&bset,&oset) == -1)
        printf("!! Set pthread mask failed\n");

    if( pthread_create(&sig_thread,NULL,(void *)sigmgr_thread,NULL) != 0)
    {
        printf("thread creation failder\n");
        exit(-1);
    }

    pthread_t newthid1,newthid2;

    if( pthread_create(&newthid1,NULL,(void *)watch_socket,NULL) != 0)
    {
        printf("thread creation failder\n");
        exit(1);
    }

    if( pthread_create(&newthid2,NULL,(void *)watch_folder,NULL) != 0)
    {
        printf("thread creation failder\n");
        exit(1);
    }
    //sleep(1);

    pthread_join(newthid2,NULL);

    close_inotify_fd (inotify_fd);

    return 0;
}

int inotifyStart()
{
    if (inotify_fd > 0)
    {
        queue_t q;
        q = queue_create (128);

        process_inotify_events (q, inotify_fd);
        //}
        printf ("\nTerminating\n");
        //close_inotify_fd (inotify_fd);
        queue_destroy (q);

    }
    return 0;
}

#if 0
//int get_all_folders(char *dirname,int *num,char **folder_list)
int get_all_folders(const char *dirname,Folders *allfolderlist,int offset,int type)
{
    struct dirent* ent = NULL;
    DIR *pDir;
    char temp_dir[1024];
    int num ;

    //printf("dis %s\n",dirname);

#if 0
    if(offset == 0)
    {
        strcpy(allfolderlist->folderlist[0].name,dirname);

    }
    else if(offset > 0)
        strcpy(allfolderlist->folderlist[offset].name,dirname);
#endif

    if(offset != -1)
    {
        strcpy(allfolderlist->folderlist[offset].name,dirname);
        allfolderlist->number++;
    }



    //allfolderlist->folderlist[0].name

    //if(offset != -1)
    //allfolderlist->number++;

    //allfolderlist.number = 1;

    pDir=opendir(dirname);
    if(pDir != NULL )
    {
        while (NULL != (ent=readdir(pDir)))
        {
            //printf("ent->d_name is %s\n",ent->d_name);

            //if(ent->d_name[0] == '.')
            //continue;
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;

            if( !strcmp(dirname,mount_path) && !strcmp(ent->d_name,"smartcloud") )
            {
                continue;
            }

            if(ent->d_type == DT_DIR)
            {
                num = allfolderlist->number;
                memset(temp_dir,0,sizeof(temp_dir));
                sprintf(temp_dir,"%s/%s",dirname,ent->d_name);

                strcpy(allfolderlist->folderlist[num].name,temp_dir);
                allfolderlist->folderlist[num].type = type;
                //printf("folder name is %s,num is %d\n",temp_dir,num);

                allfolderlist->number++;
                get_all_folders(temp_dir,allfolderlist,-1,type);
            }

        }
        closedir(pDir);
    }
    else
        printf("open %s fail \n",dirname);

    return 0;
}
#endif

int add_all_folders(char *dirname,int type)
{
    struct dirent *ent = NULL;
    DIR *pDir;
    char temp_dir[1024];
    //int num ;
    int wd;

    wd = watch_dir (inotify_fd, dirname, MY_IN_ALL_EVENTS);

    pthread_mutex_lock(&mutex_allfolderlist);
    allfolderlist_tmp = (Folder *)malloc(sizeof(Folder));
    allfolderlist_tmp->type = type;
    allfolderlist_tmp->wd = wd;
    allfolderlist_tmp->name = (char *)malloc(sizeof(char)*(strlen(dirname)+1));
    strcpy(allfolderlist_tmp->name,dirname);
    allfolderlist_tmp->next = NULL;


    allfolderlist_tail->next = allfolderlist_tmp;
    allfolderlist_tail = allfolderlist_tmp;
    pthread_mutex_unlock(&mutex_allfolderlist);

    pDir=opendir(dirname);
    if(pDir != NULL )
    {
        while (NULL != (ent=readdir(pDir)))
        {
            if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
                continue;

            if( !strcmp(dirname,mount_path) && !strcmp(ent->d_name,"smartcloud") )
            {
                continue;
            }

            if(ent->d_type == DT_DIR)
            {
                memset(temp_dir,0,sizeof(temp_dir));
                sprintf(temp_dir,"%s/%s",dirname,ent->d_name);
                add_all_folders(temp_dir,type);

            }
        }
        closedir(pDir);
    }
    else
        printf("open %s fail \n",dirname);

    return wd;
}

/* Signal handler that simply resets a flag to cause termination */
void signal_handler (int signum)
{
    //pthread_mutex_lock(&mutex);
    //keep_running = 0;
    //pthread_mutex_unlock(&mutex);
    if(signum == SIGTERM)
        exit_loop = 1;
    //pthread_exit(0);
}

void watch_socket()
{
    fd_set read_fds;
    fd_set master;
    int fdmax;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_ZERO(&master);

    int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd*/
    int numbytes;
    char buf[MAXDATASIZE];
    int yes = 1;
    int ret;

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

    FD_SET(sockfd,&master);
    fdmax = sockfd;
    sin_size = sizeof(struct sockaddr_in);

    while(!exit_loop) { /* main accept() loop */

        timeout.tv_sec = 0;
        timeout.tv_usec = 100;

        read_fds = master;

        ret = select(fdmax+1,&read_fds,NULL,NULL,&timeout);

        switch(ret)
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

            cmd_parser(buf);
        }

        close(new_fd);
    }
}

void watch_folder()
{
    while(!exit_loop)
    {
        if(allfolderlist->next != NULL)
        {
#ifdef MYDEBUG
            printf("#########inotifystart watch_folder###########\n");
#endif
            inotifyStart();
        }
		sleep(1);
    }
}

#if 0
int check_path_exist(int type,char *path)
{
    int i;
    int category = -1;

    for(i=0;i<pathlist.number;i++)
    {
        category = pathlist.folderlist[i].type;
        if(category == type)
        {
            if( strcmp(pathlist.folderlist[i].name,path) == 0)
                return 1;
            /*
           else if(strcmp(pathlist.folderlist[i].name,path)!=0 && pathlist.number-1==i)//by added zero
           {
               memset(pathlist.folderlist[pathlist.number].name,0,sizeof(pathlist.folderlist[pathlist.number].name));
               strcpy(pathlist.folderlist[pathlist.number].name,path);
               pathlist.number++;
               return 2;
           }
           */
        }
    }

    return 0;
}
#endif

void cmd_parser(char *content)
{
    //printf("\n path is %s\n",path);

    //pathlist.number++;    //add by alan

    int type;
	char typechar[3];
    char path[256];
    char temp[512];
    int len;
    int path_wd;
    //int i;
    //int num = pathlist.number;
    //const char *split = "@";
    char *p;
    //int status = 0;
    char *m_path = NULL;

    memset(path,0,sizeof(path));
    memset(temp,0,sizeof(temp));
    memset(typechar,0,sizeof(typechar));

    p=strchr(content,'@');
    if(p)
    {
        strcpy(path,p+1);
        snprintf(typechar,strlen(content)-strlen(p)+1,"%s",content);
        type = atoi(typechar);
    }
    else
    {
        printf("socket command error!!\n");
        return;
    }

    /*p=strtok(content,split);
    int j=0;
    while(p!=NULL)
    {
        switch (j)
        {
        case 0 :
            type = atoi(p);
            break;
        case 1:
            strcpy(path,p);
            break;
        default:
            break;
        }

        j++;
        p=strtok(NULL,split);
    }*/

    //printf("inotify type is %d,path is %s\n",type,path);

    len = strlen(path);
    if(path[len - 1] == '/')
        strncpy(temp,path, len - 1 );
    else
        strcpy(temp,path);


    //status = check_path_exist(type,temp);     //del by alan

    if(type == ASUSWEBSTORAGE && strlen(mount_path) == 0)
    {
        m_path = get_mount_path(temp,4);

        if(NULL == m_path)
        {
            printf("get mount path fail from %s\n",temp);
        }
        else
        {
            memset(mount_path,0,sizeof(mount_path));
            strcpy(mount_path,m_path);
            free(m_path);
        }
    }



    /*switch (status)
    {
    case 0:*/ // path is new add


    //pthread_mutex_lock(&mutex_inotify_fd);
    path_wd = add_all_folders(temp,type);
    //pthread_mutex_unlock(&mutex_inotify_fd);
    pthread_mutex_lock(&mutex_pathlist);
    pathlist_tmp = (Folder *)malloc(sizeof(Folder));
    pathlist_tmp->type = type;
    pathlist_tmp->wd = path_wd;
    pathlist_tmp->name = (char *)malloc(sizeof(char)*(strlen(temp)+1));
    strcpy(pathlist_tmp->name,temp);
    pathlist_tmp->next = NULL;

    pathlist_tail->next = pathlist_tmp;
    pathlist_tail = pathlist_tmp;
    pthread_mutex_unlock(&mutex_pathlist);




#if 0
    Folder **folderlist_tmp;
    folderlist_tmp = (Folder **)malloc(sizeof(Folder *)*(num));
    folderlist_tmp[num-1]->type = type;



    for(i = 0;i < num;i++)
    {
        pathlist.folderlist[i] = (Folder *)malloc(sizeof(Folder));

    }


    memset(pathlist.folderlist[num].name,0,sizeof(pathlist.folderlist[num].name));
    strcpy(pathlist.folderlist[num].name,temp);
    //pathlist.folderlist[num].type = type;
    pathlist.number++;
    //add_new_watch_item = 1;
    pthread_mutex_lock(&mutex);
    if(pathlist.number == 1)
        keep_running = 1;
    else
        keep_running = 0;
    pthread_mutex_unlock(&mutex);
    break;
   case 1:    // path has exist;
       break;
   case 2:     //update exist path;
       //add_new_watch_item = 1;
   default:
       break;
   }

/*
  if( check_path_exist(temp) == 0 )
   {
      strcpy(pathlist.folderlist[num].name,temp);
      pathlist.number++;
      add_new_watch_item = 1;
   }
   */

printf("$$$$$$$$$$ number is %d,keep_running is %d $$$$$$$\n",pathlist.number,keep_running);
for( i = 0; i <pathlist.number;i++)
    printf("folder[%d] is %s \n",i,pathlist.folderlist[i].name);
#endif
}

#if 0
void initConfig(char *path)
{
    int fd, len, i=0,name_len;
    name_len=strlen(path);
    //printf("name lenth is %d\n",name_len);
    char ch, tmp[256], name[256], content[256];
    memset(tmp, 0, sizeof(tmp));
    memset(name, 0, sizeof(name));
    memset(content, 0, sizeof(content));

    if((fd = open(path, O_RDONLY | O_NONBLOCK)) < 0)
    {
        printf("\nread log error!\n");
    }
    else
    {

        while((len = read(fd, &ch, 1)) > 0)
        {
            if(ch == ':')
            {
                strcpy(name, tmp);
                //printf("name is %s\n",name);
                memset(tmp, 0, sizeof(tmp));
                i = 0;
                continue;
            }
            else if(ch == '\n')
            {
                strcpy(content, tmp);
                //printf("content is [%s] \n",content);
                memset(tmp, 0, sizeof(tmp));
                i = 0;

                if(strcmp(name, "BASE_PATH") == 0)
                {
                    strcpy(base_path, content);
                }
                continue;
            }


            memcpy(tmp+i, &ch, 1);
            i++;
        }
        close(fd);
    }

}

void initConfig2(char *path)
{
    FILE *fp;

    char buffer[256];
    const char *split = ",";
    char *p;

    memset(buffer, '\0', sizeof(buffer));

    if (access(path,0) == 0)
    {
        if(( fp = fopen(path,"rb"))==NULL)
        {
            fprintf(stderr,"read Cloud error");
        }
        while(fgets(buffer,256,fp)!=NULL)
        {
            p=strtok(buffer,split);
            int i=0;
            while(p!=NULL)
            {
                switch (i)
                {
                case 0 :
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    break;
                case 6:
                    //strncpy(base_path,p,strlen(p));
                    strcpy(base_path,p);
                    if( base_path[ strlen(base_path)-1 ] == '\n' )
                        base_path[ strlen(base_path)-1 ] = '\0';
                    break;
                default:
                    break;
                }

                i++;
                p=strtok(NULL,split);
            }


        }

        fclose(fp);
    }

    if(NULL == opendir(base_path))
        mkdir(base_path,0777);
#ifdef MYDEBUG
    printf("base_path is %s\n",base_path);
#endif

}
#endif

char  *get_mount_path(char *path , int n)
{
    int i;
    char *m_path = NULL;
    m_path = (char *)malloc(sizeof(char)*256);
    memset(m_path,0,256);

    //memset(info_dest_path,'\0',sizeof(info_dest_path));

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
        strcpy(m_path,path);
#ifdef MYDEBUG
    printf("mount path is [%s]\n",m_path);
#endif

    //return m_path;
    //strcpy(info_dest_path,new_path);

    return m_path;
}
