#ifndef _CONFIG_DPI_H
#define _CONFIG_DPI_H

#define AiProtection_Node "AiProtection_Entry"
#define Vram_Node "Vram_Entry"

#define MAX_WEB_RULE 16
#define MAX_APP_RULE MAX_WEB_RULE	//current design

typedef struct _AiProtection_rule
{
	int enable;
	char mac[32];
	char catid1[64];	// Adult
	char catid2[64];	// Instant Message and Communication
	char catid3[64];	// P2P and File Transfer
	char catid4[64];	// Streaming and Entertainment
}AiProtection_rule_t;

typedef struct _AiProtection_rulelist
{
	int enable;
	int count;
	AiProtection_rule_t rule[MAX_WEB_RULE];
}AiProtection_rulelist_t;

typedef struct _AiProtection_conf
{
	int wrs_mals_enable;
	int wrs_cc_enable;
	int wrs_vp_enable;
	AiProtection_rulelist_t wrs_web;
	AiProtection_rulelist_t wrs_app;
	int tm_eula;
} AiProtection_conf_t;

typedef struct _AiProtection_dpi_version
{
	char engine[32];
	char signature[32];
} AiProtection_dpi_version_t;


typedef struct _AiProtection_status
{
	int engine_insert;
	AiProtection_dpi_version_t version;
} AiProtection_status_t;

int get_AiProtection_config(AiProtection_conf_t *config);
int set_AiProtection_config(AiProtection_conf_t *config);
int get_AiProtection_status(AiProtection_status_t *status);
int set_AiProtection_status(AiProtection_status_t *status);
int get_AiProtection_web_maxrule();
int get_AiProtection_app_maxrule();

#define DPI_CONFIG_ENABLE 1
#define DPI_CONFIG_DISABLE 0

#define DPI_STATUS_ENABLE 1
#define DPI_STATUS_DISABLE 0

#endif
