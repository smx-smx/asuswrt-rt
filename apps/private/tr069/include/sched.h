/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file sched.h
 */
#ifndef __HEADER_SCHED__
#define __HEADER_SCHED__
#include <time.h>


enum {
    SCHED_PLACE_HOLD,
    SCHED_WAITING_READABLE,
    SCHED_WAITING_WRITABLE,
    SCHED_WAITING_TIMEOUT
};

/*!
 * \struct sched
 * \brief The scheduler to implement a event drived application as a single thread program
 */
struct sched {
    unsigned char type: 2;
    unsigned char need_destroy: 1;

    void *pdata; /*!<The private data of the scheduler, created, operated and destroyed by
                      * the scheduler's callback functions - on_readable, on_writable,
                      * on_timeout and on_destroy. It is transparent to scheduling module.
                      */

    int fd; /*!<The file descriptor is waiting for readable or writable*/
    time_t timeout;

    //MUST NOT free the scheduler in these functions, if it really want to destroy
    //the scheduler, it should set the need_destroy flag instead of free it.
    void ( *on_readable )( struct sched * ); /*!<The callback function to be called if the
                                              * scheduler is SCHED_WAITING_READABLE and the
                                              * file descriptor became readable.
                                              */
    void ( *on_writable )( struct sched * ); /*!<The callback function to be called if the
                                              * scheduler is SCHED_WAITING_WRITABLE and the
                                              * file descriptor became writable.
                                              */
    void ( *on_timeout )( struct sched * ); /*!<The callback function to be called if the
                                              * scheduler is SCHED_WAITING_READABLE,
                                              * SCHED_WAITING_WRITABLE or
                                              * SCHED_WAITING_TIMEOUT and the timer expired.
                                              */
    void ( *on_destroy )( struct sched * ); /*!<The callback function to destroy the pdata*/

    struct sched *next;
#ifdef CODE_DEBUG
    const char *name;
#endif
};

time_t current_time( void );
/*!
 * \brief Add a scheduler into the scheduling list
 * After a scheduler added into the scheduling list, the scheduling module will monitor the
 * file descriptor of the scheduler for readable, writable or timeout.
 *
 * \param s The scheduler to be added into the list
 * \return Always return 0
 */
int add_sched( struct sched *s );

/*!
 * \brief Start the scheduling module
 * The scheduling module is an infinite loop which is the main loop the TRAgent.
 */
void start_sched( void );

#ifdef TR232
/*!
 * \brief Delect schedule for profile of bulkdata
 *
 * \param name The path of profile
 * \return N/A
 *
 */
void del_profile_sched( char *name );
#endif //TR232
#endif //__SCHED_H__
