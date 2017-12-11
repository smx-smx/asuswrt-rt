#ifndef __LIST_H
#define __LIST_H

#define MAX_LENGTH 128
#define MIN_LENGTH 32
#define MAX_CONTENT 256
#define MINSIZE 64

#define MYPORT 3568
#define INOTIFY_PORT 3678
#define MAXDATASIZE 1024
#define BACKLOG 100 /* max listen num*/
#define OUTLEN 256

#define PROGRESSBAR 1
#define FIX 0
#define TELLENC 1

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
#define COULD_NOT_READ_RESPONSE_BODY    908
#define HAVE_LOCAL_SOCKET               909
#define SERVER_ROOT_DELETED             910
#define PERMISSION_DENIED               911
#define UNSUPPORT_ENCODING              912

#ifndef NVRAM_
#define TOKENFILE_RECORD "/opt/etc/.smartsync/ftp_tokenfile"
#endif
#define NOTIFY_PATH "/tmp/notify/usb"

//20140914  by sherry
#ifdef _PC
#define CONFIG_PATH   "/home/eric/sherry/ftpclient_PC/ftp.conf"
#define LIST_DIR      "/home/eric/sherry/ftpclient_PC/temp/list"
#define LIST_ONE_DIR  "/home/eric/sherry/ftpclient_PC/temp/list_one.txt"
#define LIST_SIZE_DIR "/home/eric/sherry/ftpclient_PC/temp/list_size.txt"
#define TIME_DIR      "/home/eric/sherry/ftpclient_PC/temp/time.txt"
#define TIME_ONE_DIR  "/home/eric/sherry/ftpclient_PC/temp/time_one.txt"
#define CURL_HEAD     "/home/eric/sherry/ftpclient_PC/temp/headdata"
#define LOG_DIR       "/home/eric/sherry/ftpclient_PC/.logs/ftpclient"
#define CONFLICT_DIR  "/home/eric/sherry/ftpclient_PC/.logs/ftpclient_errlog"
#else
#define SHELL_FILE    "/tmp/smartsync/ftpclient/script/write_nvram"
#define CONFIG_PATH   "/tmp/smartsync/ftpclient/config/ftp.conf"
#define TMP_CONFIG    "/tmp/smartsync/ftpclient/config/ftp_tmpconfig"
#define GET_NVRAM     "/tmp/smartsync/ftpclient/script/ftpclient_get_nvram"
#define GET_INTERNET  "/tmp/smartsync/ftpclient/script/ftpclient_get_internet"
#define VAL_INTERNET  "/tmp/smartsync/ftpclient/config/ftp_val_internet"
#define LIST_DIR      "/tmp/smartsync/ftpclient/temp/list"
#define STAT_DIR      "/tmp/smartsync/ftpclient/temp/stat.txt"   //2014.11.14 by sherry 加入server空间不足的判断
#define LIST_ONE_DIR  "/tmp/smartsync/ftpclient/temp/list_one.txt"
#define LIST_SIZE_DIR "/tmp/smartsync/ftpclient/temp/list_size.txt"
#define TIME_DIR      "/tmp/smartsync/ftpclient/temp/time.txt"
#define TIME_ONE_DIR  "/tmp/smartsync/ftpclient/temp/time_one.txt"
#define CURL_HEAD     "/tmp/smartsync/ftpclient/temp/headdata"
#define LOG_DIR       "/tmp/smartsync/.logs/ftpclient"
#define CONFLICT_DIR  "/tmp/smartsync/.logs/ftpclient_errlog"
#endif

#define SPACE " "
#define ENTER "\n"
#define PLAN_B 0

#endif // LIST_H
