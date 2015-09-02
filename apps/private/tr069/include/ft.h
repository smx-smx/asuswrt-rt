/*!
 * *************************************************************
 *
 * Copyright(c) 2011, Works Systems, Inc. All rights reserved.
 *
 * This software is supplied under the terms of a license agreement
 * with Works Systems, Inc, and may not be copied nor disclosed except
 * in accordance with the terms of that agreement.
 *
 * *************************************************************
 */

/*
* \fn ft.h
* \brief File transfer module used by Download, Upload and ScheduleDownload
*/

#ifndef __HEADER_FT__
#define __HEADER_FT__

#include <stdio.h>


#include "session.h"
#include "http.h"
#include "connection.h"
#include "war_time.h"

enum {
    FT_TYPE_DOWNLOAD = 0,
    FT_TYPE_UPLOAD,
    FT_TYPE_SCHEDULE_DOWNLOAD,
    FT_TYPE_DIAGNOSTICS_DOWNLOAD,
    FT_TYPE_DIAGNOSTICS_UPLOAD,
    FT_TYPE_AUTONOMOUS_DOWNLOAD,
    FT_TYPE_AUTONOMOUS_UPLOAD,
    FT_TYPE_CDUS_DOWNLOAD
};

#define TASK_IS_DOWNLOAD(t) ((t)->type == FT_TYPE_DOWNLOAD || (t)->type == FT_TYPE_SCHEDULE_DOWNLOAD || (t)->type == FT_TYPE_DIAGNOSTICS_DOWNLOAD || (t)->type == FT_TYPE_AUTONOMOUS_DOWNLOAD || (t)->type == FT_TYPE_CDUS_DOWNLOAD)
#define TASK_IS_DIAGNOSTICS(t) ((t)->type == FT_TYPE_DIAGNOSTICS_DOWNLOAD || (t)->type == FT_TYPE_DIAGNOSTICS_UPLOAD)
#define TASK_NEED_QUEUE(t) ((t)->type == FT_TYPE_DOWNLOAD || (t)->type == FT_TYPE_SCHEDULE_DOWNLOAD (t)->type == FT_TYPE_UPLOAD)


enum {
    FT_RESULT_SUCCESS = 0,
    FT_RESULT_TIMEOUT,
    FT_RESULT_AUTH_FAIL,
    FT_RESULT_CANCELED,
    FT_RESULT_COMMON_FAIL
};

enum {
    FT_STATE_NOT_START = 1,
    FT_STATE_IN_PROGRESS,
    FT_STATE_COMPLETED
};


struct time_window {
    unsigned long start;
    unsigned long end; //0 means unlimit
    unsigned int mode;
    char user_msg[257];
    int max_retries;
};

struct task {
    unsigned int type: 4;
    unsigned int result: 4;
    unsigned int state: 2;
    unsigned int need_queue: 1;
    unsigned int need_persist: 1;
    unsigned int challenged: 1;
    unsigned int next_step: 2;
    unsigned int continue_100: 1;
    unsigned int tc_added: 1; //Add TransferComplete Request
    unsigned int tw_count: 2;
    struct time_window tw[2];
    unsigned int file_size; //0 means unknown
    char cmd_key[33];
    char target_file_name[257];
    char file_path[257];
    char file_type[65];
    char url[257];
    char username[257];
    char password[257];
    char start_time[32];
    char complete_time[32];
    int fault_code;
    FILE *upfile;
    unsigned int upfile_len;
    unsigned int upfile_sent;

#ifdef TR143
    struct { //used for TR143 diagnostics
        char interface[256]; //char interface[32];
        struct timeval rom, bom, eom;
        struct timeval request_time, response_time;
        unsigned int test_file_length;
        unsigned int total_bytes_sent;
        unsigned int already_sent;
        unsigned int test_bytes_received;
        unsigned int total_bytes_received;
        int dscp;
        int ep;
        const char *eod; //Error message on destroy
    } statistics;
#endif

    struct http http;
    struct connection conn;

    int offset;
    struct buffer outbuf;

    struct sched *main; //Do the real file transfer
    struct sched *timer; //Monitor the end time, if expired, make the task failed

    struct task *next;
};

enum {
    TW_MODE_AT_ANY_TIME,
    TW_MODE_IMMEDIATELY,
    TW_MODE_WHEN_IDLE,
    TW_MODE_NEED_CONFIRMATION,
    TW_MODE_CONFIRMATION_NOTIFIED,
    TW_MODE_UONE
};

struct task_config {
    unsigned int dynamic;
    char *name;
    char *path;

    struct task_config *next;
};

int time_window_mode2code( const char *mode );
struct task_config *get_file_type_by_name( unsigned int task_type, const char *file_type );
int ft_trust_target_file_name();
int add_task_config( const char *name, const char *value );
int add_task( struct task *t );
int cancel_task( const char *cmd_key );
int load_task();
void stop_ud();
void stop_dd();
void start_ud();
void start_dd();
int gaqt_body( struct session *ss );
void task_package_applied( const char *cmd_key, int need_reboot, int fault_code, const char *fault_string );
void schedule_download_confirmed( const char *cmd_key );
#endif
