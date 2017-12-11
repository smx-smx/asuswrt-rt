#include <stdint.h>
#include <pthread.h>

#ifndef DATA_H
#define DATA_H

#define MINSIZE 64
#define NORMALSIZE 512
#define MAXSIZE 1024
#define BUFSIZE	1024*16

#define ASUSWEBSTORAGE_SYNCFOLDER "MySyncFolder"
#define ASUSWEBSTORAGE 10

#define S_INITIAL		70
#define S_SYNC			71
#define S_DOWNUP		72
#define S_UPLOAD		73
#define S_DOWNLOAD		74
#define S_STOP			75
#define S_ERROR			76
#define S_NEEDCAPTCHA           77
#define S_AUTHFAIL              78
#define S_OTPFAIL               79

#define LOG_SIZE                sizeof(struct LOG_STRUCT)

#define CONFIG_PATH "/tmp/smartsync/asuswebstorage/config/Cloud.conf"
#define CA_INFO_FILE "/tmp/smartsync/asuswebstorage/cert/GeoTrustPrimaryCertificationAuthority-G3.crt"

#ifdef IPKG
#define NOTIFY_PATH "/tmp/notify/usb"

#define GET_NVRAM_SCRIPT_1 "/tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram"
#define SH_GET_NVRAM_SCRIPT_1 "sh /tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram"
#define NVRAM_PATH_1 "/tmp/smartsync/asuswebstorage/config/asuswebstorage_tmpconfig"

#define GET_NVRAM_SCRIPT_2 "/tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram_link"
#define SH_GET_NVRAM_SCRIPT_2 "sh /tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram_link"
#define NVRAM_PATH_2 "/tmp/smartsync/asuswebstorage/config/link_internet"
#endif

/*servergetway struct*/
typedef struct SERVICEGATEWAY
{
    int status;
    char gateway[MINSIZE];
}Servicegateway;

/*get tokey struct*/
typedef struct PACKAGE
{
    long long id;
    char display[NORMALSIZE];
    int capacity;
    int uploadbandwidth;
}Package;

typedef struct AAA
{
    int status;
    char token[MINSIZE];
    char contentrelay[MINSIZE];
    char filerelay[MINSIZE];
    char inforelay[MINSIZE];
    char jobrelay[MINSIZE];
    char rssrelay[MINSIZE];
    char searchrelay[MINSIZE];
    char webrelay[MINSIZE];
    Package package;
    char auxpasswordurl[256];
    char gateway[MINSIZE];         //for reuse token add
    char user[256];                //for reuse token add
    char pwd[256];                 //for reuse token add
}Aaa;

/*initbinaryupload struct*/
typedef struct INITBINARYUPLOAD
{
    int status;
    char transid[NORMALSIZE];
    int offset;
    char latestchecksum[256];
    char logmessage[NORMALSIZE];
    long long int fileid;
}Initbinaryupload;

/* resume binary upload*/
typedef struct RESUMEBINARYUPLOAD
{
    int status;
}Resumebinaryupload;

/* finish binary upload */
typedef struct FINISHBINARYUPLOAD
{
    int status;
    long long int fileid;
}Finishbinaryupload;

/*browse folder struct*/

typedef struct PARENTFOLDER
{
    char name[NORMALSIZE];
    long long int id;
}Parentfolder;

typedef struct PAGE
{
   int pageno;
   int pagesize;
   int totalcount;
   int hasnextpage;
}Page;

typedef struct ATTRIBUTE
{
    char lastwritetime[16];
}Attribute;

typedef struct FOLDER
{
    long long id;
    char *display;
    int isdeleted;
    int ischangeseq;
}Folder;

typedef struct FILEATTRIBUTE
{
    char lastaccesstime[16];
    char lastwritetime[16];
}Fileattribute;

typedef struct FILE
{
    long long  id;
    char *display;
    Fileattribute attribute;
    long long size;
}File;

typedef struct BROWSE
{
    int status;
    int foldernumber;
    int filenumber;
    Folder **folderlist;
    File   **filelist;
    Page page;
}Browse;

/*struct local folder struct*/
typedef struct LOCALFOLDER
{
  char *name;
}Localfolder;

typedef struct LOCALFILE
{
    char *name;
    Attribute attribute;
    long long size;
}Localfile;

typedef struct LOCAL
{
 int foldernum;
 int filenum;
 Localfolder **folderlist;
 Localfile   **filelist;
}Local;


typedef struct LOCALS
{
    Parentfolder parent;
    Local local;
    struct LOCALS *next;
}Locals;

/* find/propfind data struct*/
typedef struct PROPFIND
{
    int status;
    int isencrypt;
    long long size;
    unsigned long script;
    char type[MINSIZE];
    long long id;
}Propfind;

/*get change seq*/
typedef struct CHANGESEQ
{
  int status;
  char scrip[MINSIZE];
  unsigned int changeseq;

}Changeseq;

/*createfolder data struct*/
typedef struct CREATEFOLDER
{
    int status;
    char scrip[MINSIZE];
    long long id;

}Createfolder;

/* get all items struct*/
typedef struct FOLDERNAME
{
    char name[512];
}Foldername;

typedef struct FOLDERS
{
  int number;
  Foldername folderlist[512];
}Folders;

/* get file and folderid*/
typedef struct ITEMID
{
    long long fileID;
    long long parentID;
}ItemID;

/*rename struct*/
typedef struct OPERATEENTRY
{
    int status;
    char script[MINSIZE];
}Operateentry;

/*move etnry*/
typedef struct MOVEENTRY
{
    int status;
    char script[MINSIZE];
    char logmessage[NORMALSIZE];
}Moveentry;

/*get info*/
typedef struct BACKUPPC
{
    char name[NORMALSIZE];
    char createdtime[NORMALSIZE];
}Backuppc;

typedef struct FEATURELIST
{

}Featurelist;

typedef struct INFOPACKAGE
{
    long long id;
    char display[NORMALSIZE];
    int capacity;
    int uploadbandwidth;
    int downloadbandwidth;
    int upload;
    int download;
    int concurrentsession;
    long long int maxfilesize;
    int sharegroup;
    int hasencrption;
    char expire[MINSIZE];
    int maxbackuppc;

}Infopackage;



typedef struct GETINFO
{
   int status;
   int account;
   char email[NORMALSIZE];
   char regyear[MINSIZE];
   char language[MINSIZE];
   char activateddate[MINSIZE];
   int credentialstate;
   int usedbackuppc;
   Backuppc backuppc;
   Infopackage package;
   long long int usedcappacity;
   long long int freecapacity;
}Getinfo;

/*get MySyncFolder data struct*/
typedef struct GETMYSYNCFOLDER
{
    int status;
    long long id;
}Getmysyncfolder;

/*get PersonalSystemFolder data struct*/
typedef struct GETPERSONALSYSTEMFOLDER
{
    int status;
    char script[16];
    long long folderid;
}Getpersonalsystemfolder;

/*upload and download item struct*/
typedef struct TRANSITEM
{
    long long id;
    char name[NORMALSIZE];
    long long int size;
    char transid[64];

}Transitem;

/* log struct */
typedef struct LOG_STRUCT{
        int  status;
        char  error[512];
        float  progress;
        char path[512];
}Log_struc;

/*get entry info*/
typedef struct GETENTRYINFO
{
    int status;
    char scrip[MINSIZE];
    int isfolder;
    char display[NORMALSIZE];
    long long  parent;
    int headversion;
    Fileattribute attr;
    long long filesize;     // for file
    long long treesize; //for folder
    int ishidden;       //for folder
    int isinfected;
    int isbackup;
    int isorigdeleted;
    int ispublic;
    char createdtime[MINSIZE];
}Getentryinfo;

typedef struct my_mutex_tag
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int ready;
}my_mutex_t;


typedef struct SOCKET_CMD
{
    char cmd_name[32];
    char filename[NORMALSIZE];
    char path[1024];
    char oldname[NORMALSIZE]; //rename or move prename
    char newname[NORMALSIZE]; //rename new name
    char oldpath[1024]; //move old path
}Socket_cmd;

/*muti dir read config*/
struct asus_rule
{
    int rule;
    char path[256];
};

struct asus_config
{
    int type;
    char user[256];
    char pwd[256];
    char url[32];
    int  enable;
    char sync_path[256];
    int rule;
    char dir_name[256];
    int sync_disk_exist;
    char captcha[8];
    char otp_key[8];
};

typedef struct USERSTATE
{
    int status;
    int userstate;
    char extensionstarttime[32];
    char extensionendtime[32];
}Getuserstate;

struct MemoryStruct {
  char *memory;
  size_t size;
};

#endif
