//#include "api.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*basic function */
#define getb(type) (type*)malloc(sizeof(type))

char *oauth_encode_base64(int size, const unsigned char *src);
int oauth_decode_base64(unsigned char *dest, const char *src);
char *oauth_url_escape(const char *string);
char *oauth_url_unescape(const char *string, size_t *olen);
char *oauth_sign_hmac_sha1 (const char *m, const char *k);
char *oauth_gen_nonce();

char* MD5_string(char *string);


char *oauth_sign_plaintext (const char *m, const char *k);
char *my_str_malloc(size_t len);
void my_mkdir_r(char *path);
void my_mkdir(char *path);
//int create_sync_list();
int test_if_dir_empty(char *dir);
char *my_nstrchr(const char chr,char *str,int n);
