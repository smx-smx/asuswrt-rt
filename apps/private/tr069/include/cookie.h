/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file cookie.h
 */
#ifndef __HEADER_COOKIE__
#define __HEADER_COOKIE__

#include "buffer.h"

/*!
 *
 * \struct cookie_item
 * \brief Structure to hold a cookie
 *
 * Not follow the limit rule:
 *    o  At least 4096 bytes per cookie (as measured by the sum of the
 *       length of the cookie's name, value, and attributes).
 *
 *    o  At least 50 cookies per domain.
 *
 *    o  At least 3000 cookies total.
 */
struct cookie_item {
    char name[256];
    char value[256];
    char domain[256];
    char path[256];
    char port[64];

    unsigned int secure: 1;
    unsigned short received_port;

    time_t expire; /*!<Max-Age + receive time*/
    unsigned int version;

    struct cookie_item *next; /*!<All cookies in a HTTP session will be maintained in list*/
};

struct cookie {
    int version;
    int cook_type;
    int count;
    struct buffer cookie_header; /*!<buffer for "Cookie: " header*/
    struct cookie_item *items; /*!<Can set more than one cookie in a http session*/
};

/*!
 * \brief Destroy a session's cookies
 *
 * \param c The cookie to be destroied
 * \return N/A
 */
void cookie_destroy( struct cookie *c );

/*!
 * \brief Generate the cookie HTTP header
 *
 * \param c The cookie
 * \param secure If or not the http session is in a secure connection
 * \param req_host The host of the server
 * \param req_path The request resource path
 * \param req_port The server's port
 *
 * \return N/A
 */
void cookie_header( struct cookie *c, int secure, const char *req_host,
                    const char *req_path, unsigned short req_port );

/*!
 * \brief Add a cookie - Set-Cookie or Set-Cookie2 from server
 *
 * \param c The cookie
 * \param header The Set-Cookie or Set-Cookie2 HTTP header
 * \param req_host The server's host
 * \param req_path The request resource path
 * \param req_port The server's port
 * \param netscape If or not in netscape mode
 * \param cookie_type 0:set-cookie 1:set-cookie2
 * \return 0 when success, -1 when any error
 */
//int add_cookie(struct cookie *c, char *header, const char *req_host,const char *req_path, unsigned short req_port, int netscape);
int add_cookie( struct cookie *c, char *header, const char *req_host, const char *req_path, unsigned short req_port, int netscape, int cookie_type );
#endif
