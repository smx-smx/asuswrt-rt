/* crypto/aes/aes.h -*- mode:C; c-file-style: "eay" -*- */
/* ====================================================================
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */

#ifndef __ARITHMETIC_H
#define __ARITHMETIC_H

#include <openssl/ssl.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>
#include <openssl/aes.h>
#include <openssl/hmac.h>

#if 0
#define DLLEXPORT 
typedef char int8_t;
typedef short int int16_t;
typedef int int32_t;

typedef unsigned char u_int8_t;
typedef unsigned short int u_int16_t;
typedef unsigned int u_int32_t;
typedef unsigned long int u_int64_t;

//aes128.h
#define AES_ENCRYPT	1
#define AES_DECRYPT	0

/* Because array size can't be a const in C, the following two are macros.
   Both sizes are in bytes. */
#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16


#ifdef AES_LONG
typedef unsigned long u32;
#else
typedef unsigned int u32;
#endif

typedef unsigned short u16;
typedef unsigned char u8;

# define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
# define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }

/* This should be a hidden type, but EVP requires that the size be known */
struct aes_key_st {
#ifdef AES_LONG
    unsigned long rd_key[4 *(AES_MAXNR + 1)];
#else
    unsigned int rd_key[4 *(AES_MAXNR + 1)];
#endif
    int rounds;
};
typedef struct aes_key_st AES_KEY;

const char *AES_options(void);

int DLLEXPORT AES_set_encrypt_key(const unsigned char *userKey, const int bits,
	AES_KEY *key);

void DLLEXPORT AES_encrypt(const unsigned char *in, unsigned char *out,
	const AES_KEY *key);

int DLLEXPORT AES_set_decrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);
void DLLEXPORT AES_decrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);
//sha.h

#define SHA_DIGEST_LENGTH 20
typedef struct SHA1Context {
    u_int32_t Intermediate_Hash[SHA_DIGEST_LENGTH/4];
    u_int32_t Length_Low;
    u_int32_t Length_High;
    short int Message_Block_Index;
    unsigned char Message_Block[64];

    int Computed;
    int Corrupted;
} SHA_CTX;

int DLLEXPORT SHA1_Init(SHA_CTX *c);
int DLLEXPORT SHA1_Update(SHA_CTX *c, const void *data, unsigned long len);
int DLLEXPORT SHA1_Final(unsigned char *md, SHA_CTX *c);

unsigned char DLLEXPORT *hmac_sha1(const void *key, int key_len, const unsigned char *msg, int msg_len, unsigned char md[SHA_DIGEST_LENGTH], unsigned int *md_len);

#define EVP_sha1() 0
#define HMAC(evp, key, key_len, msg, msg_len, md, md_len) hmac_sha1(key, key_len, msg, msg_len, md, md_len)
//sha256
#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

#define SHA256_HASH_SIZE 32

typedef struct
{
    uint32 total[2];
    uint32 state[8];
    uint8 buffer[64];
}
SHA256_CTX;

void DLLEXPORT SHA256_Init( SHA256_CTX *ctx );
void DLLEXPORT SHA256_Update( SHA256_CTX *ctx, uint8 *input, uint32 length );
void DLLEXPORT SHA256_Final( uint8 digest[32], SHA256_CTX *ctx);

unsigned char DLLEXPORT *hmac_sha256(const void *key, int key_len, const char *msg, int msg_len, unsigned char md[SHA256_HASH_SIZE], unsigned int *md_len);

//md5.h
#define MD5_DIGEST_LENGTH 16
/* MD5 context. */
typedef struct {
    u_int32_t state[4]; /* state (ABCD) */
    u_int32_t count[2]; /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64]; /* input buffer */
} MD5_CTX;

int DLLEXPORT MD5_Init(MD5_CTX *c);
int DLLEXPORT MD5_Update(MD5_CTX *c, const void *data, unsigned long len);
int DLLEXPORT MD5_Final(unsigned char *md, MD5_CTX *c);
#endif
#endif /* !HEADER_AES_H */
