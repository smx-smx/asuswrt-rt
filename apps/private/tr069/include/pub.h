/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
#ifndef __PPP_H
#define __PPP_H

#define blockSize 16
#define BLOCKSIZE (blockSize*8)

#define ADATA_level1 65280
#define ADATA_level2 4294967296

#ifndef SHA256_BLOCK_SIZE
#define SHA256_BLOCK_SIZE 64
#endif

#ifndef SHA256_HASH_SIZE
#define SHA256_HASH_SIZE 32
#endif

struct point {
    unsigned char *a;
};

unsigned char *hmac_sha256( const void *key, int key_len, const char *msg, int msg_len, unsigned char md[SHA256_HASH_SIZE], unsigned int *md_len );
void hex_print( unsigned char *input, int input_bytes );
void argscat( unsigned char **dec, unsigned char *arg1, int arg1_len, unsigned char *arg2, int arg2_len );
int alen_2_encodelen( int Alen );
void conver_num_2_bytes( unsigned char **p, int num, int bytes );
void destroy_struct( struct point *p, int r );
int CIPH( unsigned char *out, unsigned char *in );
int bits_2_bytes( int bits );
void counter_generation( struct point **p, int m, unsigned char *N, int n_len, unsigned char falg_counter );
void format_payload( unsigned char *P, int p_len, struct point **r, int *row );
void format_associated_data( unsigned char *A, int a_len, struct point **p, int *row );

#endif
