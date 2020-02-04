
//defined by country code selection function		
#define COUNTRY_APAC	"AA"
#define COUNTRY_EU	"EU"
#define COUNTRY_AU	"AU"

//Wifi Region code
#define REGION_APAC	"SG"
#define REGION_EU		"DE"
#define REGION_AU		"AU"

typedef enum skip_type {
	SKIP_TYPE_NONE = 0x0,
	SKIP_TYPE_WEATHER = 0x1,
	SKIP_TYPE_WEATHER_BW4080 = 0x2,
	SKIP_TYPE_CE_BW40 = 0x4,
	SKIP_TYPE_CE_BW80 = 0x8,
	SKIP_TYPE_SGAU_BW4080 = 0x10,
	SKIP_TYPE_2G_LOWPOWER = 0x20,
	SKIP_TYPE_5G_LOWPOWER = 0x40,
	SKIP_TYPE_SKIP_5G_BAND3 = 0x80,
}skip_type_t;

typedef enum bwonly_type {
	BW_ONLY_NONE = 0x0,
	BW_ONLY_20MHZ = 0x1,
	BW_ONLY_40MHZ = 0x2,
}bwonly_type_t;

typedef struct channel_list {
	int count;
	int list[32];
} channel_list_t;

typedef struct skip_channel_list {
	skip_type_t type;
	channel_list_t channel;
} skip_channel_list_t;

typedef struct bwonly_channel_list {
	bwonly_type_t type;
	channel_list_t channel;
} bwonly_channel_list_t;

typedef struct region_condition {
	int SupportDFS;
	int UseCEforAPAC;
	int EnableDFSforAPAC;
} region_condition_t; 


int check_UseCEforAPAC(const char *ProductName);
int check_SupportDFS(const char *ProductName, const char *BootLoaderVer);
int check_EnableDFSforAPAC(const char *ProductName, const char *BootLoaderVer);
int check_SupportDFS_by_Region(const char *ProductName, const char *BootLoaderVer, const char *CountryCode);
int check_SupportCountrySelect(const char *ProductName, const char *TerritoryCode);

int getChannelList2G(int region, channel_list_t *channel);
int getCountryRegion2G(const char *countryCode);
int getChannelNumMax2G(int region);

int getChannelList_Skip(skip_type_t type, channel_list_t *list);

int getChannelList5G(int region, channel_list_t *channel, skip_type_t type);
int getCountryRegion5G(const char *countryCode, region_condition_t *condition);
int convert_Channel2str(channel_list_t *channel, const char *delim, char *buf, int buflen);

int get_RegionCode_by_Country(const char *Country, char *buf, int buflen);

int Get_wifi_channel(int band);
bwonly_type_t isBWOnlyChannel(int channel);

