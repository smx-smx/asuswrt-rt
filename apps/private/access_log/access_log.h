#define ACCESSLOG_PATH		"/tmp/access_log_list"		/* temporary file for finished data + unfinished data */
#define ACCESSLOG_END_PATH	"/tmp/access_log_end_list"	/* finished file */
#define ACCESSLOG_UI_PATH	"/tmp/access_log.txt"		/* temporary file for UI display */
#define ACCESSLOG_DIR		"access_log_backup"

/* Access_Log save the maximum number of completed data */
#define MAXAL_FILE_ENTRY	254
/* every AL_ALAM_CLOCK sec trigger Access_Log daemon once */
#define AL_ALAM_CLOCK		15
/* every TRI_NM_CLOCK sec trigger NetwaorkMap daemon once */
#define TRI_NM_CLOCK		(60/AL_ALAM_CLOCK)
/* at least tmie to backup Access Log data */
#define LEAST_AL_BKP		60
/* if more than al_max entries, delete the top 50 data */
#define DEL_TOP_ENTRY		50

#define FAIL -1
#define SUCCESS 0

//#define DEBUG_ACCESS_LOG

#ifdef DEBUG_ACCESS_LOG
#define al_dbg(fmt, args...) do { \
					FILE *fp = fopen("/tmp/access_log.log", "a"); \
					if (fp) { \
						fprintf(fp, fmt, ## args); \
						fclose(fp); \
					} \
				} while (0)
#else
#define al_dbg(args...)	do { } while(0)
#endif
