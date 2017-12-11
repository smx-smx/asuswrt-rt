#ifndef __API_H
#define __API_H
#include "data.h"

#define getb(type) (type*)malloc(sizeof(type))
#define NAMESIZE 256

#define UPLOAD 1
#define DOWNLOAD 2
#define UP_EXCEP_FAIL 3

#define SYSTEM_LOG 0
#define my_free(x)  if(x) {free(x);x=NULL;}
#define TREE_NODE_ENABLE 0
#define SERVER_TREE      0
#define MUTI_DIR         0
#define WRITE_DOWNLOAD_TEMP_FILE 0
#define MEM_POOL_ENABLE 1


/*define server error code*/
#define S_PASS                          0
#define S_VER_NOT_SUPPORT               1
#define S_AUTH_FAIL                     2
#define S_RAYLOAD_NOT_VALIDATE          4
#define S_ACCOUNT_AUTH_FAIL             5
#define S_READ_ONLY                     200
#define S_NAME_GER_ERR                  210
#define S_NAME_BLANK                    211
#define S_NAME_ILLEGAL                  212
#define S_NAME_LEN_TOO_LONG             213
#define S_NAME_REPEAT                   214
#define S_MOVE_PARENTID_IS_SAME         215
#define S_MOVE_PARENTID_NOT_EXIST       216
#define S_NEED_ADMIN_HANDLE             217
#define S_DIR_NOT_EXIST                 218
#define S_FILE_NOT_EXIST                219
#define S_FILE_GER_ERR                  220
#define S_FILE_TOO_LARGE                221
#define S_DAY_FLOW_OVER                 222
#define S_PARENT_DIR_NOT_EXIST          223
#define S_USER_NOSPACE                  224
#define S_PARAM_OUT_RANGE               225
#define S_ACCOUNT_FROZEN                226
#define S_FILE_FORMAT_NOT_SUPP          227
#define S_USER_CANCLE_FILE_TRANS        228
#define S_ACCESS_FILE_PWD_ERR           229
#define S_SHARE_GER_ERR                 230
#define S_SHARE_GROUP_OUT_LIMIT         231
#define S_SHARE_NUMOFGROUP_OUT_LIMIT    232
#define S_SRCDIR_DETDIR_NOT_ON_LEVEL    233
#define S_PARENT_NOT_ACCORD             234
#define S_ILLEGAL_OPERATION             235
#define S_FILE_CHECKSUM_NOT_MATCH       250
#define S_TRANS_ID_NOT_EXIST            251
#define S_TRANSID_NOTMATCH_FILEID       252
#define S_ILLEGAL_STATE                 501
#define S_UNAUTH_REMOTE_IP              502
#define S_PREDICATE_NOT_ACCORD          503
#define S_OTP_AUTH_FAIL                 504
#define S_OTP_ID_LOCKED                 505
#define S_OTP_ID_OUT_LIMIT              506
#define S_ILLEGAL_ID                    507
#define S_CAPTCHA_FAIL                  508
#define S_GER_ERR                       999
#define S_LOCAL_SPACE_FULL              2001          // local disk full
#define S_SERVER_SPACE_FULL             2002
#define S_MEMORY_FAIL                   4001
#define S_MKDIR_FAIL                    4002
#define S_UPDATE_ATTR_FAIL              4003
#define S_OPENDIR_FAIL                  4004
#define S_REMOVE_LOCAL_FAIL             4005
#define S_GET_PARENTID_FAIL             4006
#define S_UPLOADFILE_FAIL               4007
#define S_DOWNLOADFILE_FAIL             4008
#define S_CREATEFOLDER_FAIL             4009
#define S_HTTP_ERROR                    4010
#define S_MD5_FAIL                      4011
#define S_ENCODE_BASE64_FAIL            4012
#define S_DECODE_BASE64_FAIL            4013
#define S_URL_ESCAPE_FAIL               4014
#define S_SHA1_FAIL                     4015
#define S_RENAME_FAIL                   4016
#define S_UPLOAD_DELETED                4017
#define S_NETWORK_FAIL                  4018
#define S_ACCOUNT_CLOSE                 4019
#define S_PARSE_XML_FAIL                4020

/* define account */
#define IsAutoAccountBit      (1 << 0)  //0X01
#define NORMAL                (1 << 1)  //0X02
#define COUNTING_DOWN         (1 << 2)  //0X04
#define FROZEN                (1 << 3)  //0X08
#define EXTENSION             (1 << 4)  //0X10
#define CLOSE                 (1 << 5)  //0X20

typedef void (*processing)(double current, double total);


extern char mount_path[NAMESIZE];
extern char cloud_path[NAMESIZE];
extern char asus_path[NAMESIZE];
extern char log_path[NAMESIZE];
extern char xml_path[NAMESIZE];
extern char sync_path[NAMESIZE];
extern char system_log[NAMESIZE];
extern char general_log[NAMESIZE];
extern char confilicted_log[NAMESIZE];
extern char up_item_file[NAMESIZE];
extern char down_item_file[NAMESIZE];

#if WRITE_DOWNLOAD_TEMP_FILE
char down_item_temp_file[NAMESIZE];
#endif

extern char up_excep_fail_file[NAMESIZE];
extern char gateway_xml[NAMESIZE];
extern char get_user_state_xml[NAMESIZE];
extern char token_xml[NAMESIZE];
extern char get_info_xml[NAMESIZE];
extern char get_sync_folder_xml[NAMESIZE];
extern char get_personal_system_folder_xml[NAMESIZE];
extern char browse_folder_xml[NAMESIZE];
extern char propfind_xml[NAMESIZE];
extern char create_folder_xml[NAMESIZE];
extern char rename_xml[NAMESIZE];
extern char move_xml[NAMESIZE];
extern char remove_xml[NAMESIZE];
extern char update_xml[NAMESIZE];
extern char get_entry_info_xml[NAMESIZE];
extern char get_change_seq_xml[NAMESIZE];
extern char init_upload_xml[NAMESIZE];
extern char resume_upload_xml[NAMESIZE];
extern char finish_upload_xml[NAMESIZE];
extern char trans_excep_file[128];
extern char system_token[256];

int if_file_exist(char *filename);
int myParseHTML(char *name);
int parseDoc1(char *docname,void *obj);
void parseDoc(char *docname);
void parseMemory(char *buffer,void *obj);
void streamMyFile(const char *filename);

int getServiceGateway(char *username, char *password,Servicegateway *sg);
int obtainGateway(char *user,char *pwd,Servicegateway *sg);
int getToken(char *username, char *password,char *key,int first);
int obtainToken(char *user,char *pwd,struct asus_config *cfg,int first);

Getinfo *getInfo(char *username,char *server);
Getmysyncfolder *getMySyncFolder(char *username);
long long int obtainSyncRootID(char *user);
Getpersonalsystemfolder *getPersonalSystemFolder(char *username,char *filename);
long long int GetMyRecycleID(char *username,char *filename);
Getuserstate *getUserState(char *user,char *server);
int CheckUserState(char *user,char *server);
long long int get_max_upload_filesize(char *username);

Browse *browseFolder(char *username,long long int id,int issibiling,int pageno);
int my_parse_browse_xml(const char *filename,Browse *br);
Propfind *checkEntryExisted(char *username,long long int parentID,char *filename,char *type);
Createfolder *createFolder(char *username,long long int parentID,int isencrpted,char *name);

Operateentry *renameEntry(char *username,long long int id,int isencrpted,char *newname,int isfolder);
Moveentry *moveEntry(char *username,long long int id,char *name,long long int parentID,int isfolder,long long int pre_pid);
Operateentry *removeEntry(char *username,long long int id,int ischildonly,int isfolder,long long int pid);
int updateEntryAttribute(char *username,long long int id,long long int parentID,int isencrpted,int isfolder);

Getentryinfo *getEntryInfo(int isfolder,long long int entryid);


/*shared files API*/
Changeseq *getChangeSeq(long long int folderid);

/* upload file*/
int sha512(char *filename,char *checksum);
Initbinaryupload  *initBinaryUpload(char *filename,long long int parentID,char *transid,long long int fileID);
Resumebinaryupload *resumeBinaryUpload(char *filename, Initbinaryupload *ibu);
Finishbinaryupload *finishBinaryUpload(Initbinaryupload *ibu);
int uploadFile(char *filename,long long int parentID,char *transid,long long int fileID);

/*download file*/
int update_local_file_attr(Fileattribute *attr,char *filename);
int downloadFile(long long int fileID,char *filename,long long int size,int ismodify,Fileattribute *attr);

int getResizedPhoto(long long int photoFileID,int sizeType,int preview);
int getFullTextCompanion(long long int fileID,int preview,int key);
int getVideoSnapshot(long long int filmID,int preview);

int handle_error(int code,char *type);
long long int getParentID(char *path);
long int check_server_space(char *username);
int write_log(int status,char *message,char *filename);
int write_finish_log();
int write_system_log(char *action,char *name);
int write_confilicted_log(char *prename,char *confilicted_name);
int write_trans_excep_log(char *fullname,int type,char *msg);
int sync_all_item(char *dir,long long int parentID);
int sync_all_item_uploadonly(char *dir,long long int parentID);
int add_all_download_only_socket_list(char *cmd,const char *dir);
int add_all_download_only_dragfolder_socket_list(const char *dir);
int handle_rename(long long int parentID,char *fullname,int type,char *prepath,int is_case_conflict,char *pre_name);
int handle_createfolder_fail_code(int status,long long int parent_ID,char *path,char* fullname);
int handle_upload_fail_code(int status,long long int parent_ID,char* fullname,const char *path);
int handle_delete_fail_code(int status);
int handle_rename_fail_code(int status,long long int parentID,char *fullname,char *path,int isfolder);
int handle_move_fail_code(int status,char *path,char *fullname,long long int parentID,char *prepath,long long int entryID,int isfolder);
long long int create_server_folder_r(const char *path);
int upload_entry(char *fullname,long long int parent_ID,char *path);

int StrToHex(char *src,int len);

int IsEntryDeletedFromServer(long long int fileID,int isfolder);
int obtain_token_from_file(const char *filename,Aaa *aaa);

int count_call_api(const char *url);
void print_count_call_api();
#endif

