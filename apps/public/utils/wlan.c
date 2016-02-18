#include <stdio.h>
#include <string.h>
#include "wlan.h"

int check_UseCEforAPAC(const char *ProductName)
{
	//char tmpstr[64];
	int ret = 0;

	//bzero(tmpstr, sizeof(tmpstr));
	//EZgetAttrValue("SysInfo", "Entry", "ProductName", tmpstr);

	if (!strcmp(ProductName, "DSL-AC52U") ||
		!strcmp(ProductName, "DSL-AC56U"))
	{
		ret = 1;
	}
	return ret;
}

int check_SupportDFS(const char *ProductName, const char *BootLoaderVer)
{
	char BLVer_Str[16];
	char *tmp;
	char *tmp2;
	int ret = 0;
	int curBootver = 0;
	// We pass DFS certification of DSL-N66U at Decemer 2013 then use the boot loader version: 2.10 as the anchor to support DFS.
	int supportBootver = 0x2A;

	bzero(BLVer_Str, sizeof(BLVer_Str));
	snprintf(BLVer_Str, sizeof(BLVer_Str) - 1, "%s", BootLoaderVer);
	if (!strcmp(ProductName, "DSL-N66U") ||
		!strcmp(ProductName, "DSL-AC56U") ||
		!strcmp(ProductName, "DSL-AC52U"))
	{
		tmp=strtok(BLVer_Str, ".");
		tmp2=strtok(NULL, ".");
		curBootver = (atoi(tmp) * 0x10) + atoi(tmp2);
		if(curBootver >= supportBootver)
		{
			ret = 1;
		}
	}
	return ret;
}

int check_EnableDFSforAPAC(const char *ProductName, const char *BootLoaderVer)
{
	int ret = 0;

	if (check_UseCEforAPAC(ProductName) && 
		check_SupportDFS(ProductName, BootLoaderVer))
	{
		if (!strcmp(ProductName, "DSL-AC56U") ||
			!strcmp(ProductName, "DSL-AC52U"))
		{
			ret = 1;
		}
	}
	return ret;
}

int check_SupportCountrySelect(const char *ProductName, const char *TerritoryCode)
{
	int ret = 0;

	if (!strncmp(TerritoryCode, "AA", 2))
	{
		if (!strcmp(ProductName, "DSL-AC56U"))
		{
			ret = 1;
		}
	}
	return ret;
}


static channel_list_t channel_list_2g[] = {
	{
		11, 
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
	},
	{
		13,
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
	},
	{
		14,
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
	}
};

int getChannelList2G(int region, channel_list_t *channel)
{
	channel_list_t *list_p;

	if (region < 0 || region > 11)
		return -1;

	if (channel == NULL)
		return -1;

	switch(region)
	{
		case 0: 
			list_p = &channel_list_2g[0];
			break;
			
		case 1: 
			list_p = &channel_list_2g[1];
			break;
			
		case 5: 
			list_p = &channel_list_2g[2];
			break;

		default:
			list_p = &channel_list_2g[2];
			break;
	}

	memcpy(channel, list_p, sizeof(channel_list_t));
	
	return 0;
}


int getCountryRegion2G(const char *countryCode)
{
	if (countryCode == NULL)
	{
		return 5;	// 1-14
	}
	else if((strcasecmp(countryCode, "CA") == 0) || (strcasecmp(countryCode, "CO") == 0) ||
		(strcasecmp(countryCode, "DO") == 0) || (strcasecmp(countryCode, "GT") == 0) ||
		(strcasecmp(countryCode, "MX") == 0) || (strcasecmp(countryCode, "NO") == 0) ||
		(strcasecmp(countryCode, "PA") == 0) || (strcasecmp(countryCode, "PR") == 0) ||
		(strcasecmp(countryCode, "TW") == 0) || (strcasecmp(countryCode, "US") == 0) ||
		(strcasecmp(countryCode, "UZ") == 0) ||
		(strcasecmp(countryCode, "Z1") == 0) || (strcasecmp(countryCode, "Z3") == 0)
		)
	{
		return 0;	// 1-11
	}
	else if (strcasecmp(countryCode, "DB") == 0  || strcasecmp(countryCode, "") == 0)
	{
		return 5;	// 1-14
	}
	else{
		return 1;	// 1-13
	}
}


int getChannelNumMax2G(int region)
{
	switch(region)
	{
		case 0: return 11;
		case 1: return 13;
		case 5: return 14;
	}
	return 14;
}

static channel_list_t channel_list_5g[] = {
	{
		13, 
		{36, 40, 44, 48, 52, 56, 60, 64, 149, 153, 157, 161, 165}
	},
	{
		19,
		{36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140},
	},
	{
		8,
		{36, 40, 44, 48, 52, 56, 60, 64},
	},
	{
		8,
		{52,  56, 60, 64, 149, 153, 157, 161},
	},
	{
		5,
		{149, 153, 157, 161, 165},
	},
	{
		4,
		{149, 153, 157, 161},
	},
	{
		4,
		{36, 40, 44, 48},
	},
	{
		24,
		{36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165},
	},
	{
		4,
		{52, 56, 60, 64},
	},
	{
		21,
		{36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 132, 136, 140, 149, 153, 157, 161, 165},
	},
	{
		9,
		{36, 40, 44, 48, 149, 153, 157, 161, 165},
	},
	{
		18,
		{36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 149, 153, 157, 161},
	}
};

static skip_channel_list_t channel_list_skip[] = {
	{
		SKIP_TYPE_WEATHER,	//weather radar channel
		{	3,
			{120, 124, 128}, 
		},
	},
	{
		SKIP_TYPE_WEATHER_BW4080,	//weather radar channel
		{	4,
			{116, 120, 124, 128}, 
		},
	},
	{
		SKIP_TYPE_CE_BW40,
		{	1,
			{140}, 
		},
	},
	{
		SKIP_TYPE_CE_BW80,
		{	3,
			{132, 136, 140}, 
		},
	},
	{
		SKIP_TYPE_SGAU_BW4080,	
		{	1,
			{165}, 
		},
	},
};

static void dumplist(channel_list_t *list)
{
	int i;
	char clist[256];
	char ch[4];
	
	bzero(clist, sizeof(clist));
	
	for (i=0; i < list->count; i++)
	{
		if (i != 0)
		{
			strcat(clist, ",");
		}
		sprintf(ch, "%d", list->list[i]);
		strcat(clist, ch);
	}

	printf( "%s", clist);
}

/*
	merge list1 into list2
*/
static int mergeChannelList(channel_list_t *list1, channel_list_t *list2)
{
	channel_list_t result;
	int index_1 = 0;
	int index_2 = 0;
	int index_r = 0;

	if (list1 == NULL || list2 == NULL)
		return -1;

	bzero(&result, sizeof(result));
	while (index_1 < list1->count && index_2 < list2->count)
	{
		if (list1->list[index_1] < list2->list[index_2])
		{
			result.list[index_r] = list1->list[index_1];
			index_1++;
		}
		else
		{
			if (list1->list[index_1] == list2->list[index_2])
				index_1++;

			result.list[index_r] = list2->list[index_2];
			index_2++;
		}
		index_r++;
	}

	while (index_1 < list1->count)
	{
		result.list[index_r] = list1->list[index_1];
		index_1++;
		index_r++;
	}

	while (index_2 < list2->count)
	{
		result.list[index_r] = list2->list[index_2];
		index_2++;
		index_r++;
	}
	result.count = index_r;

	bzero(list2, sizeof(channel_list_t));
	memcpy(list2, &result, sizeof(channel_list_t));

	return 0;
}

int getChannelList_Skip(skip_type_t type, channel_list_t *list)
{
	int i;

	if (list == NULL)
		return -1;

	bzero(list, sizeof(channel_list_t));
	for (i = 0; i<sizeof(channel_list_skip)/sizeof(skip_channel_list_t) ; i++)
	{
		if (channel_list_skip[i].type & type)
		{
			mergeChannelList(&(channel_list_skip[i].channel), list);
		}
	}

	return 0;
}

/*
	remove skip channel from "src_list" according to "type"
	and store to dst_list
*/
static int rmSkipChannel(channel_list_t *src_list, channel_list_t *dst_list, skip_type_t type)
{
	channel_list_t skiplist;
	int i;
	int cindex;
	int skipindex;
	int skipflag;

	if (src_list == NULL || dst_list == NULL)
		return -1;

	getChannelList_Skip(type, &skiplist);

	dst_list->count = src_list->count;
	cindex = 0;
	for (i=0; i<src_list->count; i++)
	{
		skipflag = 0;
		for (skipindex = 0; skipindex < skiplist.count; skipindex++)
		{
			if (src_list->list[i] == skiplist.list[skipindex])
			{
				skipflag = 1;
				break;
			}
		}

		if (skipflag == 0)
		{
			dst_list->list[cindex] = src_list->list[i];
			cindex++;
		}
		else
		{
			dst_list->count--;
		}
	}

	return 0;
}

int getChannelList5G(int region, channel_list_t *channel, skip_type_t type)
{
	channel_list_t *list_p;

	if (region < 0 || region > 11)
		return -1;

	if (channel == NULL)
		return -1;

	list_p = &channel_list_5g[region];
	rmSkipChannel(list_p, channel, type);
	
	return 0;
}

int getCountryRegion5G(const char *countryCode, region_condition_t *condition)
{
	if ((!strcasecmp(countryCode, "AE")) ||
		(!strcasecmp(countryCode, "AL")) ||
		(!strcasecmp(countryCode, "AU")) ||
		(!strcasecmp(countryCode, "BH")) ||
		(!strcasecmp(countryCode, "BY")) ||
		(!strcasecmp(countryCode, "CA")) ||
		(!strcasecmp(countryCode, "CL")) ||
		(!strcasecmp(countryCode, "CO")) ||
		(!strcasecmp(countryCode, "CR")) ||
		(!strcasecmp(countryCode, "DO")) ||
		(!strcasecmp(countryCode, "DZ")) ||
		(!strcasecmp(countryCode, "EC")) ||
		(!strcasecmp(countryCode, "GT")) ||
		(!strcasecmp(countryCode, "HK")) ||
		(!strcasecmp(countryCode, "HN")) ||
		(!strcasecmp(countryCode, "IL")) ||
		(!strcasecmp(countryCode, "JO")) ||
		(!strcasecmp(countryCode, "KW")) ||
		(!strcasecmp(countryCode, "KZ")) ||
		(!strcasecmp(countryCode, "LB")) ||
		(!strcasecmp(countryCode, "MA")) ||
		(!strcasecmp(countryCode, "MK")) ||
		(!strcasecmp(countryCode, "MY")) ||
		(!strcasecmp(countryCode, "NZ")) ||
		(!strcasecmp(countryCode, "OM")) ||
		(!strcasecmp(countryCode, "PA")) ||
		(!strcasecmp(countryCode, "PK")) ||
		(!strcasecmp(countryCode, "PR")) ||
		(!strcasecmp(countryCode, "QA")) ||
		(!strcasecmp(countryCode, "SA")) ||
		(!strcasecmp(countryCode, "SV")) ||
		(!strcasecmp(countryCode, "SY")) ||
		(!strcasecmp(countryCode, "TH")) ||
		(!strcasecmp(countryCode, "US")) ||
		(!strcasecmp(countryCode, "VN")) ||
		(!strcasecmp(countryCode, "YE")) ||
		(!strcasecmp(countryCode, "ZW")) ||
#ifdef RTCONFIG_LOCALE2012
		(!strcasecmp(countryCode, "AR")) ||
		(!strcasecmp(countryCode, "UY")) ||
#else
		(!strcasecmp(countryCode, "IN")) ||
		(!strcasecmp(countryCode, "MO")) ||
		(!strcasecmp(countryCode, "MX")) ||
		(!strcasecmp(countryCode, "NO")) ||
		(!strcasecmp(countryCode, "RO")) ||
		(!strcasecmp(countryCode, "RU")) ||
		(!strcasecmp(countryCode, "UA")) ||
#endif
			//for specific power
			(!strcasecmp(countryCode, "Z1"))
	)
	{
		return 10;
	}
	else if ((!strcasecmp(countryCode, "AT")) ||
			(!strcasecmp(countryCode, "BE")) ||
			(!strcasecmp(countryCode, "BG")) ||
			(!strcasecmp(countryCode, "CH")) ||
			(!strcasecmp(countryCode, "CY")) ||
			(!strcasecmp(countryCode, "DK")) ||
			(!strcasecmp(countryCode, "EE")) ||
			(!strcasecmp(countryCode, "ES")) ||
			(!strcasecmp(countryCode, "FI")) ||
			(!strcasecmp(countryCode, "GB")) ||
			(!strcasecmp(countryCode, "GR")) ||
			(!strcasecmp(countryCode, "HU")) ||
			(!strcasecmp(countryCode, "IE")) ||
			(!strcasecmp(countryCode, "IS")) ||
			(!strcasecmp(countryCode, "IT")) ||
			(!strcasecmp(countryCode, "LI")) ||
			(!strcasecmp(countryCode, "LT")) ||
			(!strcasecmp(countryCode, "LU")) ||
			(!strcasecmp(countryCode, "LV")) ||
			(!strcasecmp(countryCode, "NL")) ||
			(!strcasecmp(countryCode, "PL")) ||
			(!strcasecmp(countryCode, "PT")) ||
			(!strcasecmp(countryCode, "SE")) ||
			(!strcasecmp(countryCode, "SI")) ||
			(!strcasecmp(countryCode, "SK")) ||
			(!strcasecmp(countryCode, "UZ")) ||
			(!strcasecmp(countryCode, "ZA")) ||
#ifdef RTCONFIG_LOCALE2012
			(!strcasecmp(countryCode, "AM")) ||
			(!strcasecmp(countryCode, "AZ")) ||
			(!strcasecmp(countryCode, "CZ")) ||
			(!strcasecmp(countryCode, "EG")) ||
			(!strcasecmp(countryCode, "FR")) ||
			(!strcasecmp(countryCode, "GE")) ||
			(!strcasecmp(countryCode, "HR")) ||
			(!strcasecmp(countryCode, "JP")) ||
			(!strcasecmp(countryCode, "KP")) ||
			(!strcasecmp(countryCode, "KR")) ||
			(!strcasecmp(countryCode, "MC")) ||
			(!strcasecmp(countryCode, "NO")) ||
			(!strcasecmp(countryCode, "RO")) ||
			(!strcasecmp(countryCode, "TN")) ||
			(!strcasecmp(countryCode, "TR")) ||
			(!strcasecmp(countryCode, "TT")) ||
#else
			(!strcasecmp(countryCode, "BR")) ||
#endif
			//for specific power
			(!strcasecmp(countryCode, "Z2"))
	)
	{
		return 6;
	}
	else if (!strcasecmp(countryCode, "SG"))
	{
		if(condition && condition->UseCEforAPAC)
		{
			if (condition->EnableDFSforAPAC)
			{
				return 1;
			}
			else
			{
				return 6;
			}
		}
		else
		{
			return 10;
		}
	}
	else if (!strcasecmp(countryCode, "DE"))
	{
		if(condition && condition->SupportDFS)
		{
			return 1;
		}
		else
		{
			return 6;
		}
	}
	else if (
#ifndef RTCONFIG_LOCALE2012
			(!strcasecmp(countryCode, "AM")) ||
			(!strcasecmp(countryCode, "AZ")) ||
			(!strcasecmp(countryCode, "CZ")) ||
			(!strcasecmp(countryCode, "EG")) ||
			(!strcasecmp(countryCode, "FR")) ||
			(!strcasecmp(countryCode, "GE")) ||
			(!strcasecmp(countryCode, "HR")) ||
			(!strcasecmp(countryCode, "MC")) ||
			(!strcasecmp(countryCode, "TN")) ||
			(!strcasecmp(countryCode, "TR")) ||
			(!strcasecmp(countryCode, "TT"))
#else
			(!strcasecmp(countryCode, "IN")) ||
			(!strcasecmp(countryCode, "MX"))
#endif
	)
	{
		return 6;
	}
	else if (
#ifndef RTCONFIG_LOCALE2012
			(!strcasecmp(countryCode, "AR")) ||
#endif
			//for specific power
			(!strcasecmp(countryCode, "Z3"))

	)
	{
		return 3;
	}
	else if ((!strcasecmp(countryCode, "BZ")) ||
			(!strcasecmp(countryCode, "BO")) ||
			(!strcasecmp(countryCode, "BN")) ||
			(!strcasecmp(countryCode, "CN")) ||
			(!strcasecmp(countryCode, "ID")) ||
			(!strcasecmp(countryCode, "IR")) ||
			(!strcasecmp(countryCode, "PE")) ||
			(!strcasecmp(countryCode, "PH")) ||
			(!strcasecmp(countryCode, "TW")) ||
#ifdef RTCONFIG_LOCALE2012
			(!strcasecmp(countryCode, "BR")) ||
			(!strcasecmp(countryCode, "MO")) ||
			(!strcasecmp(countryCode, "VE"))	||
#endif
			//for specific power
			(!strcasecmp(countryCode, "Z4"))
	)
	{
		return 5;
	}
#ifndef RTCONFIG_LOCALE2012
	else if (	(!strcasecmp(countryCode, "KP")) ||
			(!strcasecmp(countryCode, "KR")) ||
			(!strcasecmp(countryCode, "UY")) ||
			(!strcasecmp(countryCode, "VE"))
	)
	{
		return 5;
	}
#else
	else if (!strcasecmp(countryCode, "RU"))
	{
		return 6;
	}
#endif
	else if (!strcasecmp(countryCode, "DB"))
	{
		return 7;
	}
	else if (
#ifndef RTCONFIG_LOCALE2012
			(!strcasecmp(countryCode, "JP"))
#else
			(!strcasecmp(countryCode, "UA"))
#endif
	)
	{
		return 6;
	}
	else
	{
		return 7;
	}
}


int convert_Channel2str(channel_list_t *channel, const char *delim, char *buf, int buflen)
{
	char clist[256];
	char ch[8];
	int i;

	if (channel == NULL || buf == NULL || delim == NULL)
		return -1;

	bzero(clist, sizeof(clist));
	bzero(buf, buflen);
	for (i=0; i < channel->count; i++)
	{
		if (i != 0)
		{
			strcat(clist, delim);
		}
		sprintf(ch, "%d", channel->list[i]);
		strcat(clist, ch);
	}

	snprintf(buf, buflen, "%s", clist);
	return strlen(buf);
}

int get_RegionCode_by_Country(const char *Country, char *buf, int buflen)
{
	if (Country == NULL && buf == NULL)
		return -1;

	bzero(buf, buflen);
	if (!strcmp(Country, "AA"))
	{
		snprintf(buf, buflen, "SG");
	}
	else if (!strcmp(Country, "EU"))
	{
		snprintf(buf, buflen, "DE");
	}
	else if (!strcmp(Country, "AU"))
	{
		snprintf(buf, buflen, "AU");
	}

	return strlen(buf);
}


