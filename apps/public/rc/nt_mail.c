/*
	NOTIFICATION_CENTER=y
*/

#include <rc.h>
#include <shared.h>
#include <libnt.h>
#include <nt_actMail_common.h>

#define TM_WRS_MAIL             0x01
#define TRAFFIC_LIMITER_MAIL    0x02

/* debug message */
#define AM_DEBUG                "/tmp/AM_DEBUG"
#define MyDBG(fmt,args...) \
	if(check_if_file_exist(AM_DEBUG) > 0) { \
		dbg("[ALERT MAIL][%s:(%d)]"fmt, __FUNCTION__, __LINE__, ##args); \
	}

#define NC_NODE                 "NTcenter_Entry"

static int value = 0;

void am_setup_email_conf()
{
	FILE *fp;
	char buf[256];
	
	MyDBG("[%s(%d)]start to write conf %s\n", __FUNCTION__, __LINE__, MAIL_CONF);
	
	// email server conf setting
	mkdir_if_none("/etc/email");
	if((fp = fopen(MAIL_CONF, "w")) == NULL){
		_dprintf("fail to open %s\n", MAIL_CONF);
		return;
	}

	if(tcapi_get(NC_NODE, "mIndex", buf) == 0) {
		if(!strcmp(buf, "-1")) {
			_dprintf("[%s(%d)][DEFALUT] Have NOT doing EMAIL configuration.\n", __FUNCTION__, __LINE__);
			goto EMAIL_CONF_ERROR;
		}
	} else 
		goto EMAIL_CONF_ERROR;

	fprintf(fp,"SMTP_SERVER = '%s'\n", SMTPInfo[atoi(buf)].server);
	fprintf(fp,"SMTP_PORT = '%d'\n", SMTPInfo[atoi(buf)].port);
	fprintf(fp,"MY_NAME = 'Administrator'\n");
	if(tcapi_get(NC_NODE, "MY_EMAIL", buf) == 0) {
		fprintf(fp,"MY_EMAIL = '%s'\n", buf);
	} else 
		goto EMAIL_CONF_ERROR;
	
	fprintf(fp,"USE_TLS = 'true'\n");
	fprintf(fp,"SMTP_AUTH = 'LOGIN'\n");
	
	if(tcapi_get(NC_NODE, "SMTP_AUTH_USER", buf) == 0) {
		fprintf(fp,"SMTP_AUTH_USER = '%s'\n", buf);
	} else 
		goto EMAIL_CONF_ERROR;
	
	if(tcapi_get(NC_NODE, "SMTP_AUTH_PASS", buf) == 0) {
		fprintf(fp,"SMTP_AUTH_PASS = '%s'\n", buf);
	} else 
		goto EMAIL_CONF_ERROR;
	
	fclose(fp);
	return;
	
EMAIL_CONF_ERROR:
	if(fp) fclose(fp);
	_dprintf("[%s(%d)] Generate EMAIL CONFIG ERROR.\n", __FUNCTION__, __LINE__);
	return;
}

static void am_setup_email_item(const char *f_name, const char *val)
{
	char buf[128];
	
	snprintf(buf, sizeof(buf), MAIL_ITEM_PATH"/%s", f_name);
	f_write_string(buf, val, 0, 0);
}

void am_setup_email_info()
{
	char mailto[256];
	
	mkdir_if_none(MAIL_ITEM_PATH);
	am_setup_email_item(MODEL_NAME, get_productid()); // model name
	am_setup_email_item(SYSTEM_LANGUAGE, "EN"); // TODO current language
	if(tcapi_get(NC_NODE, "NOTIFY_MAIL_TO", mailto) == 0) {
		if(strcmp(mailto, "")) 
			am_setup_email_item(MAIL_TO, mailto);
	}
}

#ifdef RTCONFIG_BWDPI
static
void am_tm_wrs_mail()
{
	char path[48];
	char str[32];
	memset(path, 0, sizeof(path));

	// AiProtection wrs_vp.txt
	int flag = merge_log(path, sizeof(path));

	MyDBG("flag=%d, path=%s\n", flag, path);

	// send mails && cc events
	if ((flag & 0x01) && strcmp(path, "")) {
		snprintf(str, 32, "0x%x", PROTECTION_CC_EVENT);
		eval("Notify_Event2NC", str, "");
	}

	// send vp events
	if ((flag & 0x02) && strcmp(path, "")) {
		snprintf(str, 32, "0x%x", PROTECTION_VULNERABILITY_EVENT);
		eval("Notify_Event2NC", str, "");
	}
}
#endif

#ifdef RTCONFIG_TRAFFIC_LIMITER
static
void am_traffic_limiter_mail()
{
	char str[32];
	unsigned int flag = traffic_limiter_read_bit("alert");
	unsigned int val = traffic_limiter_read_bit("count"); // have sent
	int send = 0;

	MyDBG("flag=%u, val=%u\n", flag, val);

	if ((flag & 0x1) && !(val & 0x01)) {
		// send alert SMS
		//if(nvram_get_int("modem_sms_limit")) traffic_limiter_sendSMS("alert", 0);
		send = 1;
	}
	if ((flag & 0x2) && !(val & 0x02)) {
		// send alert SMS
		//if(nvram_get_int("modem_sms_limit")) traffic_limiter_sendSMS("alert", 1);
		send = 1;
	}

	// send alert mail
	if (send) {
		snprintf(str, 32, "0x%x", TRAFFICMETER_ALERT_EVENT);
		eval("Notify_Event2NC", str, "");
	}
}
#endif

static
int alert_mail_function_check()
{
	// intial global variable
	value = 0;

	// TrendMicro wrs mail
	if (nvram_get_int("wrs_enable") || nvram_get_int("wrs_cc_enable") || nvram_get_int("wrs_vp_enable"))
		value |= TM_WRS_MAIL;

	// traffic limiter mail
	if (nvram_get_int("tl_enable") && (nvram_get_int("tl0_alert_enable") || nvram_get_int("tl1_alert_enable")))
		value |= TRAFFIC_LIMITER_MAIL;

	// TODO: add new here

	MyDBG("value = %d(0x%x)\n", value, value);

	return value;
}

void alert_mail_service()
{
	// check function enable or not
	if (!alert_mail_function_check())
		return;

#ifdef RTCONFIG_BWDPI
	if (value & TM_WRS_MAIL)
		am_tm_wrs_mail();
#endif

#ifdef RTCONFIG_TRAFFIC_LIMITER
	if (value & TRAFFIC_LIMITER_MAIL)
		am_traffic_limiter_mail();
#endif
	
	// TODO: add new here
}
