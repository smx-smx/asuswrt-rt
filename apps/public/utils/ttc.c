/******************************************************************************
*
* For territory code related functions
*
******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "ttc.h"


language_list_t supportLang [] = {
	{
		{"en-us"},
		LANG_TYPE_EN
	},
	{
		{"pt-br"},
		LANG_TYPE_BR
	},
	{
		{"zh-cn"},
		LANG_TYPE_CN
	},
	{
		{"zh-sg"},
		LANG_TYPE_CN
	},
	{
		{"cs-cz"},
		LANG_TYPE_CZ
	},
	{
		{"da-dk"},
		LANG_TYPE_DA
	},
	{
		{"de-at"},
		LANG_TYPE_DE
	},
	{
		{"de-li"},
		LANG_TYPE_DE
	},
	{
		{"de-lu"},
		LANG_TYPE_DE
	},
	{
		{"de-de"},
		LANG_TYPE_DE
	},
	{
		{"de-ch"},
		LANG_TYPE_DE
	},
	{
		{"es-ec"},
		LANG_TYPE_ES
	},
	{
		{"es-py"},
		LANG_TYPE_ES
	},
	{
		{"es-pa"},
		LANG_TYPE_ES
	},
	{
		{"es-ni"},
		LANG_TYPE_ES
	},
	{
		{"es-gt"},
		LANG_TYPE_ES
	},
	{
		{"es-do"},
		LANG_TYPE_ES
	},
	{
		{"es-es"},
		LANG_TYPE_ES
	},
	{
		{"es-hn"},
		LANG_TYPE_ES
	},
	{
		{"es-ve"},
		LANG_TYPE_ES
	},
	{
		{"es-pr"},
		LANG_TYPE_ES
	},
	{
		{"es-ar"},
		LANG_TYPE_ES
	},
	{
		{"es-bo"},
		LANG_TYPE_ES
	},
	{
		{"es-us"},
		LANG_TYPE_ES
	},
	{
		{"es-co"},
		LANG_TYPE_ES
	},
	{
		{"es-cr"},
		LANG_TYPE_ES
	},
	{
		{"es-uy"},
		LANG_TYPE_ES
	},
	{
		{"es-pe"},
		LANG_TYPE_ES
	},
	{
		{"es-cl"},
		LANG_TYPE_ES
	},
	{
		{"es-mx"},
		LANG_TYPE_ES
	},
	{
		{"es-sv"},
		LANG_TYPE_ES
	},
	{
		{"fi-fi"},
		LANG_TYPE_FI
	},
	{
		{"fr-fr"},
		LANG_TYPE_FR
	},
	{
		{"it-it"},
		LANG_TYPE_IT
	},
	{
		{"it-ch"},
		LANG_TYPE_IT
	},
	{
		{"ms-my"},
		LANG_TYPE_MS
	},
	{
		{"ms-bn"},
		LANG_TYPE_MS
	},
	{
		{"nb-no"},
		LANG_TYPE_NO
	},
	{
		{"nn-no"},
		LANG_TYPE_NO
	},
	{
		{"pl-pl"},
		LANG_TYPE_PL
	},
	{
		{"ru-ru"},
		LANG_TYPE_RU
	},
	{
		{"sv-fi"},
		LANG_TYPE_SV
	},
	{
		{"sv-se"},
		LANG_TYPE_SV
	},
	{
		{"th-th"},
		LANG_TYPE_TH
	},
	{
		{"tr-tr"},
		LANG_TYPE_TR
	},
	{
		{"zh-tw"},
		LANG_TYPE_TW
	},
	{
		{"zh-hk"},
		LANG_TYPE_TW
	},
	{
		{"zh-mo"},
		LANG_TYPE_TW
	},
	{
		{"uk-ua"},
		LANG_TYPE_UK
	},
};




/******************************************************************************
*
* ttc: territory code
* str_lang: 'en-us' or 'en', all lower case
*
******************************************************************************/
int getLangType(char *ttc, char * str_lang)
{
	int index = 0;
	int len = 0;
	int langType = LANG_TYPE_NONE;

	if((!str_lang)||(strlen(str_lang) == 0))
	{
		return LANG_TYPE_EN; //default
	}

	//01:compare 'en-us' with supportLang
	len = strlen(str_lang);
	for(index = 0; index < (sizeof(supportLang)/sizeof(supportLang[0])); index++ )
	{
		if(strncmp(supportLang[index].langStr, str_lang, len) == 0)
		{
			langType = supportLang[index].langType;
			break;
		}
	}

	//02:compare 'en' with supportLang
	if(langType == LANG_TYPE_NONE)
	{
		len = 2;
		for(index = 0; index < (sizeof(supportLang)/sizeof(supportLang[0])); index++ )
		{
			if(strncmp(supportLang[index].langStr, str_lang, len) == 0)
			{
				langType = supportLang[index].langType;
				break;
			}
		}
	}

	if(langType == LANG_TYPE_NONE)
	{
		return LANG_TYPE_EN; //default
	}

	if(langType == LANG_TYPE_EN)
	{
		return LANG_TYPE_EN; //to speed up
	}

	if((!ttc) || (strlen(ttc) == 0))
	{
		return langType;
	}

	//03:compare with territory code
	if(strstr(ttc, "UK"))
	{
		switch(langType)
		{
			case LANG_TYPE_EN:
			case LANG_TYPE_DA:
			case LANG_TYPE_FI:
			case LANG_TYPE_NO:
			case LANG_TYPE_SV:
				break;
			default:
				langType = LANG_TYPE_EN;
		}
	}
	else if(strstr(ttc, "EU"))
	{
		switch(langType)
		{
			case LANG_TYPE_EN:
			case LANG_TYPE_BR:
			case LANG_TYPE_CN:
			case LANG_TYPE_CZ:
			case LANG_TYPE_DE:
			case LANG_TYPE_ES:
			case LANG_TYPE_FR:
			case LANG_TYPE_IT:
			case LANG_TYPE_MS:
			case LANG_TYPE_PL:
			case LANG_TYPE_RU:
			case LANG_TYPE_TH:
			case LANG_TYPE_TR:
			case LANG_TYPE_TW:
			case LANG_TYPE_UK:
				break;
			default:
				langType = LANG_TYPE_EN;
		}
	}

	return langType;
}



/******************************************************************************
******************************************************************************/