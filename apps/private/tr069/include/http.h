/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file http.h
 *
 */
#ifndef __HEADER_HTTP__
#define __HEADER_HTTP__

#include "buffer.h"
#include "cookie.h"
#include "connection.h"

typedef enum {
    HTTP_STATE_RECV_HEADER,
    HTTP_STATE_RECV_EXTRA_HEADER,
    HTTP_STATE_RECV_CONTENT,
    HTTP_STATE_RECV_CHUNK_HEADER,
    HTTP_STATE_RECV_CHUNK_DATA
} http_state_t;

typedef enum {
    HTTP_ERROR,
    HTTP_BODY_TOO_LARGE,
    HTTP_NEED_WAITING,
    HTTP_CONTINUE,
    HTTP_COMPLETE
} http_recv_status_t;

typedef enum {
    HTTP_RESPONSE,
    HTTP_REQUEST
} http_msg_type_t;

typedef enum {
    HTTP_BODY_BUFFER,
    HTTP_BODY_FILE,
    HTTP_BODY_NONE
} http_body_type_t;

enum {
    DIGEST_MD5,
    DIGEST_MD5_SESS
};

enum {
    QOP_NONE,
    QOP_AUTH,
    QOP_AUTH_INT
};

struct http {
    union {
        struct {
            char method[6];
            char uri[1025];
            char version[10];
        } request;

        struct {
            char version[10];
            char code[4];
            char phrase[60];
        } response;
    } start_line;

    unsigned char msg_type: 1; //HTTP_REQUEST or HTTP_RESPONSE
    unsigned char sl_flag: 1; //To indicate if or not the start line has been processed
    unsigned char header_count: 6;
    char *headers[32];

    unsigned int bytes_received;
    char authorization[512];

    struct {
        char schema[8];
        char method[16];
        char domain[256];
        char uri[1025];
        char realm[256];
        char nonce[128];
        char opaque[128];
        char cnonce[33];
        unsigned int stale: 1;
        unsigned int algorithm: 1;
        unsigned int qop: 2;
        int nonce_count;
    } chal;

    char req_host[256];
    char req_path[1025];
    unsigned short req_port;

    struct cookie cookie;

    http_state_t state;

    int chunked;
    long int block_len;
    http_body_type_t body_type;
    long int body_size; /*!<Current body size*/
    long int body_limit; /*!<Body limit, 0 or minus number  means no limitation*/
    void *body;

    int inlen;
    char inbuf[512];
};

/*!
 * \brief Retrieve a specific HTTP header
 *
 * \param h The http message
 * \param name The header's name
 *
 * \return The header value if found, or less NULL
 */
char *http_get_header( struct http *h, const char *name );

/*!
 * \brief Receive a HTTP message from a connection
 *
 * \param h The HTTP message
 * \param conn The TCP connection
 */
http_recv_status_t http_recv( struct http *h, struct connection *conn );

/*!
 * \brief Delete all headers of a HTTP message
 *
 * \param h The HTTP message
 */
void del_http_headers( struct http *h );

/*!
 * \brief Destroy a HTTP message - delete all headers, destroy its body and cookies
 *
 * \param h The http message to be destroied
 */
void http_destroy( struct http *h );

/*!
 * \brief Disassemble a string into key-value mode
 *
 * \param s The string's pointer, this function will store the start pointer of the left
 * part into it
 * \param name To store the next argument's name(key)'s start pointer
 * \param value To store the next argument's value's start pointer
 * \param sep The seperator between two argument
 *
 * \return 0 on found one, or less return -1
 *
 * Note: The source string MUST follow the following format:
 * str -> items
 * items ->item | itemSEPitems
 * item ->key | key=value
 * key -> char | charkey
 * value -> char | charvalue
 * SEP -> char
 * char -> (char)(1 ~ 255)
 *
 * There can be any space around seperator
 */
int http_next_arg( char **s, char **name, char **value, char sep );

/*!
 * \brief To generate a authorization fro a HTTP challenge
 *
 * \param h The http request
 * \param username
 * \param password
 * \param method The HTTP method, POST or GET in agent
 * \param uri The uri requested by the HTTP message
 *
 * \return 0 on success, -1 on failure
 */
int http_auth( struct http *h, const char *username, const char *password, const char *method, const char *uri );

/*!
 * \brief Generate a nonce string(a random number encoded in base64)
 *
 * \param buf The buffer to hold the result
 * \param buf_len The length of the buffer
 */
void http_generate_nonce( char *buf, int buf_len );

/*!
 * \brief Update the authorization for each request in the same HTTP session
 *
 * \param h The HTTP message
 * \param username
 * \param password
 *
 * \return 0 on OK, -1 on failure
 */
int http_update_authorization( struct http *h, const char *username, const char *password );

#endif
