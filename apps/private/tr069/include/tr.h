/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file tr.h
 */
#ifndef __HEADER_TR__
#define __HEADER_TR__


#include <stdio.h>
#include "war_type.h"
#ifdef ASUSWRT
#include <rtconfig.h>
#endif

/*!
 * \brief load listener from file .register
 */
void load_listener();

/*!
 * \brief Set the maximum number of listeners for loading listeners from file
 * \param max maximum number
 */
void set_max_listener( const int max );

/*!
 * \brief Set the init argument for lib_init()
 * \param name The init directive name - "Init"
 * \param value The init directive's value
 * \return 0 when success, -1 when any error
 */
int set_init_arg( const char *name, const char *value );

/*!
 * \brief Generate a time based random number
 * \return The generated random number
 */
long int tr_random( void );

/*!
 * \brief Translate a digit string into a short integer
 * \param str The digit string
 * \return The transllated short integer
 */
short int tr_atos( const char *str );

/*!
 * \brief Calculate the length of an opened file
 * \param fp The pointer of the opened file
 * \return The length of the file
 */
long int tr_file_len( FILE *fp );

/*!
 * \brief Calculate a file's full name
 * If the file name is a absolute path, the full name is the same as the file name, or
 * it will be a file under the current work directory
 *
 * \param fn The file name
 * \param full_fn The buffer to hold the full file name
 * \param full_len The full file name buffer size
 *
 * \return N/A
 */
void tr_full_name( const char *fn, char *full_fn, int full_len );

size_t tr_fread( void *ptr, size_t size, size_t nmemb, FILE *stream );
size_t tr_fwrite( const void *ptr, size_t size, size_t nmemb, FILE *stream );


/*!
 * \brief A wrapper of the standard function - fopen
 * This function will open a file related to the work directory. For example, if the
 * current work directory is "/home/user/tr/conf" and the file_name is "abc.txt",
 * then this function will open the "/home/user/tr/conf/abc.txt". But if the file_name
 * start with the "/"(say "/some/path/to/abc.txt") which means it is a absolute path,
 * then this function will open "/some/path/to/abc.txt".
 *
 * \param file_name The file name
 * \param mode The mode to open the file in
 *
 * \return The opened file pointer when success, NULL when any error
 */
FILE *tr_fopen( const char *file_name, const char *mode );

/*!
 * \brief Check if or not a file exists
 * As the same as tr_fopen(), the file's path is calculated from the current work
 * directory.
 *
 * \param name The file name
 *
 * \return 1 if exists, or less return 0
 */
int tr_exist( const char *name );

/*!
 * \brief A wrapper of fclose
 * \param fp The opened file pointer
 * \return N/A
 */

void tr_fclose( FILE *fp );

/*!
 * \brief Rename a file
 * \param old_name The old file name
 * \param new_name The new file name
 *
 * \return 0 when success, -1 when any error
 */
int tr_rename( const char *old_name, const char *new_name );

/*!
 * \brief Delete a file
 * \param file_name The file's name
 * \return 0 when success, -1 when any error
 */
int tr_remove( const char *file_name );

/*!
 * \brief Backup a file
 * \param file_name The file's name
 * \return 0 when success, -1 when any error
 */
int tr_backup( const char *file_name );

/*!
 * \brief Create a file
 * \param file_name The file's name
 * \return 0 when success, -1 when any error
 */
int tr_create( const char *file_name );

/*!
 * \brief Delete a file's backup file
 * \param file_name The file's name
 * \return 0 when success, -1 when any error
 */
int tr_remove_backup( const char *file_name );

/*!
 * \brief Rebuild a file from its backup file
 * \param file_name The file's name
 * \return 0 when success, -1 when any error
 */
int tr_restore( const char *file_name );

enum {
    BOOLEAN_FALSE = 0,
    BOOLEAN_TRUE,
    BOOLEAN_ERROR
};

#if defined(RTCONFIG_JFFS2) || defined(RTCONFIG_BRCM_NAND_JFFS2) || defined(TCSUPPORT_ADD_JFFS)
#define JFFS_TR_PATH	"/jffs/tr/"
#else
#define JFFS_TR_PATH	"/yaffs/tr/"
#endif

#define TR069_CLIENT_VERSION "TR069 Client 1.0"
#define FLAG_NEED_FACTORY_RESET JFFS_TR_PATH".FLAG_NEED_FACTORY_RESET"	//#define FLAG_NEED_FACTORY_RESET ".FLAG_NEED_FACTORY_RESET"
#define FLAG_BOOTSTRAP JFFS_TR_PATH".FLAG_BOOTSTRAP"	//#define FLAG_BOOTSTRAP ".FLAG_BOOTSTRAP"
#define FLAG_ACS_URL_CHANGED JFFS_TR_PATH".FLAG_ACS_URL_CHANGED"	//#define FLAG_ACS_URL_CHANGED ".FLAG_ACS_URL_CHANGED"
#define FILE_PATH_LEN   128
#define NODE_PATH_LEN   256
#define PARAMETER_KEY_LEN   32

#ifdef MIN
#undef MIN
#endif
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#ifdef MAX
#undef MAX
#endif
#define MAX(a, b) ((a) > (b) ? (a) : (b))


#endif
