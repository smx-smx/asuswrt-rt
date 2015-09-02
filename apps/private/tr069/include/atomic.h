/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file atomic.h
 */
#ifndef __HEADER_ATOMIC__
#define __HEADER_ATOMIC__

#include "tr_lib.h"
/*!
 * \brief Start the transaction - to open the journal file
 *
 * \return 0 when success, -1 when any error
 */
int start_transaction( void );

/*!
 * \brief Commit the transaction - remove the journal file
 *
 * \return 0 when success, -1 when any error
 */
int commit_transaction( void );

/*!
 * \brief Rollback the transaction - undo all operation in journal file then destroy it
 *
 * \return Always 0
 * \remark Theoretical, it also can failed when rollback, but we can not handle this, else
 * it will be a infinite loop.
 */
int rollback_transaction( void );

/*!
 * \brief Rebuild the journal from journal file the rollback the un-committed transaction
 *
 * \return Always 0
 *
 * \remark Just call this function when the agent startup. It will check the left journal
 * file and rollback and remove it.
 */
int clear_journals( void );

/*!
 * \brief Recoed the SetParameterAttributes(Notification) journal
 *
 * \param path The node's path
 * \param old_noc The old noc property value
 *
 * \return 0 when success, -1 when any error
 */
int spa_noc_journal( const char *path, const char old_noc[PROPERTY_LENGTH] );

/*!
 * \brief Record the SetParameterAttributes(ACL) journal
 *
 * \param path The node's path
 * \param old_acl The old acl property value
 *
 * \return 0 when success, -1 when any error
 */
int spa_acl_journal( const char *path, const char old_acl[PROPERTY_LENGTH] );

/*!
 * \brief Record the SetParameterAttributes(Next Instance Number) journal - we extend this
 *
 * \param path The node's path
 * \param old_acl The old acl property value
 *
 * \return 0 when success, -1 when any error
 */
int spa_nin_journal( const char *path, const char old_nin[PROPERTY_LENGTH] );

/*!
 * \brief Record the SetParameterValues journal
 *
 * \param path The node's path
 * \param old_value The old value
 *
 * \return 0 when success, -1 when any error
 */
int spv_journal( const char *path, const char *old_value );

/*!
 * \brief Record the AddObject journal
 *
 * \param path The new node's path
 *
 * \return 0 when success, -1 when any error
 */
int ao_journal( const char *path );

/*!
 * \brief Record the DeleteObject journal
 *
 * \param path The node's path that has been deleted
 *
 * \return 0 when success, -1 when any error
 */
int do_journal( const char *path );

#endif
