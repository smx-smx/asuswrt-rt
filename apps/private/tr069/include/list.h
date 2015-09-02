#ifndef __HEADER_LIST__
#define __HEADER_LIST__

#define LIST_APPEND(type, list, element) do { \
    type *pre; \
    element->next = NULL; \
    for(pre = list; pre && pre->next; pre = pre->next) { \
        /*Noop*/ \
    } \
    if(pre) \
        pre->next = element; \
    else \
        list = element; \
} while(0)

#define LIST_DELETE(type, list, element) do { \
    type *pre; \
    for(pre = list; pre && pre != element && pre->next && pre->next != element; pre = pre->next) { \
        /*Noop*/ \
    } \
    if(pre) { \
        if(pre == element) \
            list = element->next; \
        else if(pre->next == element) \
            pre->next = element->next; \
    } \
} while(0)

#define LIST_ADD(list, element) do { \
    element->next = list; \
    list = element; \
} wile(0)

#define LIST_STRING_SEARCH(list, member, str, output) do { \
    for(output = list; output; output = output->next) { \
        if(strcmp(output->member, str) == 0) \
            break; \
    } \
} while(0)

#define LIST_ISTRING_SEARCH(list, member, str, output) do { \
    for(output = list; output; output = output->next) { \
        if(strcasecmp(output->member, str) == 0) \
            break; \
    } \
} while(0)

#define LIST_NUMBER_SEARCH(list, member, num, output) do { \
    for(output = list; output; output = output->next) { \
        if(output->member == num) \
            break; \
    } \
} while(0)

#endif
