/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
#ifndef __ENCRYPT_
#define __ENCRYPT_
int calculate_cek( char *s_cookie, unsigned char *cek ) ;
int content_encrypt( char *value, unsigned char *cek, int cek_len, char **output );
int content_decrypt( char *value, unsigned char *cek, int cek_len, unsigned char **output );

#endif
