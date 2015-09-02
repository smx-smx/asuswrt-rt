/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file b64.h
 */
#ifndef __HEADER_B64__
#define __HEADER_B64__
#include <string.h>
/*!
 * \brief Encode data in base64 format
 *
 * \param input The source data
 * \param input_len The source data length
 * \param output The output buffer
 * \param output_len The output buffer length
 *
 * \return 0 when success, -1 whe the outbuf is not enough
 */
int b64_encode( const unsigned char *input, int input_len, char *output, int output_len );

int b64_decode( const char *src, int src_len, unsigned char *dst, int dst_len );

/*!
 * \brief Validate a base64 encoded string
 *
 * \param str The string to be validated
 *
 * \return 1 if the string is a base 64 string, 0 if not
 */
int string_is_b64( const char *str );

#endif
