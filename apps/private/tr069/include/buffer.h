/*!
 * \file buffer.h
 * Copyright (C) Arthur Echo(Shuiquan Xie)
 */

#ifndef __HEADER_BUFFER__
#define __HEADER_BUFFER__

#include <stdarg.h>

/*!
 * \brief The dynamic buffer to store a unknown length data
 */
struct buffer {
    int buf_len; /*!< The current buffer length */
    int data_len; /*!< The current data length */
    char *data; /*!< The data buffer*/
};

/*!
 * \brief Destroy a buffer(free the buffer)
 *
 * \param b The buffer
 *
 * \return N/A
 */
void destroy_buffer( struct buffer *b );

/*!
 * \brief Initiate a buffer
 *
 * \param b The buffer
 * \return N/A
 */
void init_buffer( struct buffer *b );

/*!
 * \brief Delete some data from the buffer tail
 *
 * \param b The buffer
 * \param len The length of data to be delete
 * \return N/A
 */
void trim_buffer( struct buffer *b, int len );

/*!
 * \brief Set the data_len to zero
 *
 * \param b The buffer
 * \return N/A
 */
void reset_buffer( struct buffer *b );

/*!
 * Push more data into the buffer
 *
 * \param b The buffer
 * \param fmt The format string of the data argument, just the same as printf
 *
 * \return 0 when success, -1 when any error
 */
int push_buffer( struct buffer *b, const char *fmt, ... );

int vpush_buffer( struct buffer *b, const char *fmt, va_list ap );

/*!
 * Store binary data to buffer
 */
int bpush_buffer( struct buffer *b, const void *data, int len );
#endif
