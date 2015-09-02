#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

struct nvram_tuple{
	char* node;
	char* attribute;
	char* value;
	struct nvram_tuple *next;
};

#endif /* _DEFAULTS_H_ */
