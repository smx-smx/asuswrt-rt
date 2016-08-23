
typedef enum lang_type_s {
	LANG_TYPE_NONE = 0,
	LANG_TYPE_EN = 1,
	LANG_TYPE_BR = 2,
	LANG_TYPE_CN = 3,
	LANG_TYPE_CZ = 4,
	LANG_TYPE_DA = 5,
	LANG_TYPE_DE = 6,
	LANG_TYPE_ES = 7,
	LANG_TYPE_FI = 8,
	LANG_TYPE_FR = 9,
	LANG_TYPE_IT = 10,
	LANG_TYPE_MS = 11,
	LANG_TYPE_NO = 12,
	LANG_TYPE_PL = 13,
	LANG_TYPE_RU = 14,
	LANG_TYPE_SV = 15,
	LANG_TYPE_TH = 16,
	LANG_TYPE_TR = 17,
	LANG_TYPE_TW = 18,
	LANG_TYPE_UK = 19
}lang_type_t;

typedef struct language_list_s {
	char langStr[8];
	lang_type_t langType;
} language_list_t;

extern int getLangType(char *ttc, char *str_lang);

