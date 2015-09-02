/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file hex.h
 */
#ifndef __HEADER_HEX__
#define __HEADER_HEX__

/*!
 * \brief Convert binary to hexadecimal string
 *
 * \param bin The binary data to be converted
 * \param bin_len The length of the binary data
 * \param buf The buffer to ouput the result
 * \param buf_len The length of the result buffer
 *
 */
void binary2hexstr( unsigned char *bin, int bin_len, char *buf, int buf_len );

#endif
