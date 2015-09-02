#ifndef _IPT_CONDITION_H 
#define _IPT_CONDITION_H 
 
#define CONDITION_NAME_LEN 32 
 
struct condition_info { 
    char name[CONDITION_NAME_LEN]; 
    int invert; 
}; 
 
#endif /* _IPT_CONDITION_H */ 
