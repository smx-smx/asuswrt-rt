/* md5.h
 * Copyright (C) 2005 Tillmann Werner <tillmann.werner@gmx.de>
 *
 * This file is free software; as a special exception the author gives
 * unlimited permission to copy and/or distribute it, with or without
 * modifications, as long as this notice is preserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This code is based on an OpenSSL-compatible implementation of the RSA
 * Data Security, * Inc. MD5 Message-Digest Algorithm, written by Solar
 * Designer <solar at openwall.com> in 2001, and placed in the public
 * domain. There's absolutely no warranty. See md5.c for more information.
 */

#ifndef __MD5_H
#define __MD5_H

#define MD5_DIGEST_LENGTH 16
#ifdef __ENABLE_SSL__
#include <openssl/md5.h>
#else
/* MD5 context. */
typedef struct {
    u_int32_t state[4]; /* state (ABCD) */
    u_int32_t count[2]; /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64]; /* input buffer */
} MD5_CTX;

int MD5_Init( MD5_CTX *c );
int MD5_Update( MD5_CTX *c, const void *data, unsigned long len );
int MD5_Final( unsigned char *md, MD5_CTX *c );
#endif //__ENABLE_SSL__

#endif //__MD5_H
