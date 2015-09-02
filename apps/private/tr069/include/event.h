/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file event.h
 *
 * \brief Maintain the TR069 event list
 */

#ifndef __HEADER_EVENT__
#define __HEADER_EVENT__


typedef enum {
    S_EVENT_BOOTSTRAP = 0,
    S_EVENT_BOOT,
    S_EVENT_PERIODIC,
    S_EVENT_SCHEDULED,
    S_EVENT_VALUE_CHANGE,
    S_EVENT_KICKED,
    S_EVENT_CONNECTION_REQUEST,
    S_EVENT_TRANSFER_COMPLETE,
    S_EVENT_DIAGNOSTICS_COMPLETE,
    S_EVENT_REQUEST_DOWNLOAD,
    S_EVENT_AUTONOMOUS_TRANSFER_COMPLETE,
    S_EVENT_DU_STATE_CHANGE_COMPLETE,
    S_EVENT_AUTONOMOUS_DU_STATE_CHANGE_COMPLETE,
    M_EVENT_REBOOT,
    M_EVENT_SCHEDULEINFORM,
    M_EVENT_DOWNLOAD,
    M_EVENT_SCHEDULE_DOWNLOAD,
    M_EVENT_UPLOAD,
    M_EVENT_CHANGE_DU_STATE,
    X_EVENT_CUSTOMED_1, /*!<There are five reserved events which can be customed by CPE vendor*/
    X_EVENT_CUSTOMED_2,
    X_EVENT_CUSTOMED_3,
    X_EVENT_CUSTOMED_4,
    X_EVENT_CUSTOMED_5,
} event_code_t;


/*!
 * \struct event
 */
struct event {
    //int id;
    int commited; /*!<If ot not the event has been committed to ACS*/
    event_code_t event_code;
    char command_key[33]; /*!<Only for multi event*/
    struct event *next;
};

/*!
 * \brief Set a customed event name, it is a config callback function
 *
 * \param name Not used
 * \param value The config item value
 *
 * \return 0 if OK, or less -1
 */
int set_customed_event( const char *name, const char *value );

/*!
 * \brief Retrieve the next event in the event list
 *
 * \param cur The current event pointer, if it is NULL, this function return return the first event,
 * else it return the (*cur)->next. It also the return value int *cur
 */
struct event *next_event( struct event **cur );

/*!
 * \brief Return the number of the events, which is used for formating the Inform message
 *
 * \param commited If or not count the commited event
 *
 * \return The number of the events
 */
int get_event_count( int commited );

int event_only_value_change( void );
/*!
 * \brief Search a event from the events list
 *
 * \param ec The event code
 * \param ck The command key, NULL means ignore the command key and return the first event whose
 * event code match the \a ec
 *
 * \return The first matched event
 */
struct event *get_event( event_code_t ec, const char *ck );

/*!
 * \brief Convert a event code to string
 *
 * \param code The event code
 * \return The converted string
 */
const char *code2string( event_code_t code );

/*!
 * \brief Convert a string to event code
 *
 * \param str The string to be converted
 * \return The related event code
 */
event_code_t string2code( const char *str );

int any_new_event();
/*!
 * \brief Delete a event from the events list
 * \param e The event to be deleted
 */
void del_event( struct event *e );

/*!
 * \brief Add a single-event into the events list
 *
 * \param ec The event code of the single-event
 * \return 0 on success, -1 on failure
 */
int add_single_event( event_code_t ec );


/*!
 * \brief Add a multi-event into the events list
 *
 * \param ec The event code of the event
 * \param ck The command key of the event
 *
 * \return 0 on success, -1 on failure
 */
int add_multi_event( event_code_t ec, const char *ck );

/*!
 * \brief Load the event from the events file
 *
 * \return 0 on success, -1 on failure
 */
int load_event( void );

/*!
 * \brief To complete event list operation, it will rewrite the events file
 *
 * \return 0 on success, -1 on failure
 */
int complete_delete_event( void );

/*!
 * \brief To complete event list operation, it will write the events list to events file
 * and then create session
 *
 * \return 0 on success, -1 on failure
 */
int complete_add_event( int reboot );

#define IS_SINGLE(e) ((e)->event_code >= S_EVENT_BOOTSTRAP && \
        (e)->event_code <= S_EVENT_AUTONOMOUS_TRANSFER_COMPLETE)
#define IS_MULTI(e) (((e)->event_code >= M_EVENT_REBOOT) && ((e)->event_code <= M_EVENT_UPLOAD))
#define MUST_CROSS_REBOOT(e) (((e)->event_code == S_EVENT_BOOTSTRAP) || \
    ((e)->event_code == S_EVENT_PERIODIC) || \
    ((e)->event_code == S_EVENT_SCHEDULED) || \
    ((e)->event_code == S_EVENT_TRANSFER_COMPLETE) || \
    ((e)->event_code == S_EVENT_AUTONOMOUS_TRANSFER_COMPLETE) || \
    IS_MULTI(e))

#endif
