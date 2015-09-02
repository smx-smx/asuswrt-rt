/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file tr_lib.h
 * \brief the integrate related interfaces, data type definition
 */

#ifndef __HEADER_TR_LIB__
#define __HEADER_TR_LIB__

#ifdef __cplusplus
extern "C"
{
#endif

#define TR_LIB_API

#define __ASUS
#undef __V4_2

    //Notification property definition
    enum {
        NOC_OFF,
        NOC_ACTIVE,
        NOC_PASSIVE
    };


    //MOT node type definition
    enum {
        TYPE_STRING,
        TYPE_INT,
        TYPE_UNSIGNED_INT,
        TYPE_BOOLEAN,
        TYPE_DATE_TIME,
        TYPE_BASE64,
        TYPE_NODE, //Our extension, just for interior node, in TR069 protocol, interior
        //called object and leaf node called parameter. In TRAgent, we call
        //both of them as "node", but they have different type property
        TYPE_ANY
    };

#define PROPERTY_LENGTH 256

    //This is the structure that present a node in this simulator.
    //In real world device, the customer just need to define themselves's node structure
    //and implement the callback functions.

    struct node {
        char name[64];
        unsigned int rw: 1;
        unsigned int getc: 1;
        unsigned int noc: 2;
        unsigned int nocc: 4;
        //unsigned int type:4;

        unsigned int nin;
        unsigned int il;

        //The length is limited in the simulator
        char type[32];
        char acl[256];

        int listener_count;
        char **listener_addr;

#ifdef __V4_2
        union {

            struct {
                int ( *get )( int *, int, char **, int *, int * );
                int ( *set )( int *, int, char *, int, int );
            } param;

            struct {
                int ( *add )( int *, int, unsigned int );
                int ( *del )( int *, int, unsigned int );
            } obj;
        } dev;
        //#else
        //     char value[256];
#elif defined __ASUS
	struct {
	    struct handler *cmd;
	    char arg[32];
	    char act[64];
	} dev;
#endif
#ifdef TR098
    //char value[256];
    char value[257];
#endif
#ifdef TR181
        char value[300];
#endif
        struct node *parent;

        struct node *brother;

        struct node *children;
    };

    typedef struct node *node_t;

#ifdef ALIAS
    struct alias_map {
        char uri[256];
        char alias[256];
        struct alias_map *next;
    };

    TR_LIB_API struct alias_map *lib_get_alias_head();
#endif

    /*!
     * \fn lib_download_complete
     * Notify the device that download some file complete
     *
     *  \param type The file type
     *  \param path The path the file was saved
     *  \param cmd_key The command key that CPE MUST echo from CLI
     *
     *  \return 1 means need reboot the device, 0 means OK and do not need to reboot the device, -1 means error
     */
    TR_LIB_API int lib_download_complete( const char *type, const char *path, const char *cmd_key );

    /*!
     * \fn lib_init
     * Initiate the library
     *
     * \param arg The configuration item value of init in tr.conf
     * \return 0 when success, or less return -1
     *
     * \remark if this function return an error, TRAgent will exit abnormally.
     */
    TR_LIB_API int lib_init( const char *arg );

    /*!
     * \fn lib_factory_reset
     * Notify the device to do factoryreset
     *
     * \return always be 0
     * \remark The TRAgent just notify device to do factory reset operation. In this
     * function, device MUST NOT do factory reset immediately. It should just set some
     * flag to indicate it will do it. Because the TRAgent MUST to complete the session
     * with DM server. Once the session end, TRAgent will call the lib_reboot to reboot
     * the device.
     */
    TR_LIB_API int lib_factory_reset( void );

    /*!
     * \fn lib_reboot
     * Reboot the device
     *
     * \return always be 0
     */
    TR_LIB_API int lib_reboot( void );

    /*!
     * \fn lib_resolve_node
     * Resolve the MOT node path to an internal structure(node_t)
     *
     * \param path The MOT node's path, for example "InternetGatewayDevice.A.B.C"
     * \param node The internal presentation of the MOT node
     *
     * \return 0 when success, 1 when the node does not existing, -1 when any error
     */
    TR_LIB_API int lib_resolve_node( const char *path, node_t *node );

    /*!
     * \fn lib_get_property
     * To retrieve a given(by name) property of a given node
     *
     * \param node The node whose property will be retrieved
     * \param name The property name
     * \param prop The buffer to save the property, all property will be transfered
     * as string between TRAgent and the library
     *
     * \return 0 when success, -1 when any error
     */
    TR_LIB_API int lib_get_property( node_t node, const char *name, char prop[PROPERTY_LENGTH] );

    /*!
     * \fn lib_set_property
     * Replace a given property of a given node
     *
     * \param node The node whose property will be replaced
     * \param name The property name
     * \param prop The new property value
     *
     * \return 0 when success, -1 when any error
     */
    TR_LIB_API int lib_set_property( node_t node, const char *name, const char prop[PROPERTY_LENGTH] );

    /*!
     * \fn lib_get_value
     * Retrieve a leaf node's value
     *
     * \param node The node whose value to be retrieved
     * \param value The buffer to save the value's pointer
     *
     * \return 0 when success, -1 when any error
     *
     * \remark This function MUST allocate a block memory from heap to hold the value
     * and save the pointer in the parameter value. Any type data will be transfered
     * in string between TRAgent and callback functions, for example the node type is
     * integer -123, then the callback function should return the value as "-123"
     */
    TR_LIB_API int lib_get_value( node_t node, char **value );

    /*!
     * \fn lib_destroy_value
     * To free the memory allocated by lib_get_value()
     *
     * \param value The memory's pointer
     * \return N/A
     */
    TR_LIB_API void lib_destroy_value( char *value );

    /*!
     * \fn lib_set_value
     * Replace a given leaf node's value
     *
     * \param node The node whose value will be replaced
     * \param value The new value
     *
     * \return 0 when success, -1 when any error
     * \remark As the same as lib_get_value(), any type value will be transfered as string
     * between TRAgent and this callback function
     */
    TR_LIB_API int lib_set_value( node_t node, const char *value );

    /*!
     * \fn lib_ao
     * Add an object instance under
     *
     * \param parent The parent node which the new instance(a sub tree) will be added under
     * \param cin The current instance number, the callback function MUST use it as the
     * new instance's root node name.
     *
     * \return 0 when success, -1 when any error
     */
    TR_LIB_API int lib_ao( node_t parent, int cin, char *alias );

    /*!
     * \fn lib_do
     * Delete an object instance which created by lib_ao
     *
     * \param node The instance sub tree'root node
     * \return 0 when success, -1 when any error
     */
    TR_LIB_API int lib_do( node_t node );

    /*!
     * \fn lib_start_session
     * Notify the library that TRAgent will start a session
     *
     * \return 0 when success, -1 when any error
     * \remark The TRAgent notify the library that it will start a session which does not
     * exactly means that TRAgent will launch a DM session with server, it just to let the
     * library to ready process incoming operations. Note that the TRAgent maybe call
     * this function before it call the lib_end_session, so this function MUST remember
     * the reference count. For example, if the library store all data in a sqlite
     * database, generally, this callback function open the database if the counter is zero
     * and set the count to 1, else just increase the counter by 1. In lib_end_session, it
     * decrease the counter by 1, and then check it if is zero, if yes, then close the
     * sqlite database, or less do nothing.
     *
     */
    TR_LIB_API int lib_start_session( void );

    /*!
     * \fn lib_end_session
     * Notify the library that the session is over
     *
     * \return N/A
     */
    TR_LIB_API int lib_end_session( void );

    /*!
     * \fn lib_node2path
     * Resove the node structure to a string path - converting with lib_resolve_node()
     *
     * \param node The node to be resolved
     *
     * \return The string path of the node.
     *
     * \remark The string MUST be located in static or global scope. The library MUST NOT
     * allocate memory to store the path, or less that will be a memory leak, because
     * the TRAgent will not free it.
     *
     * \remark Do not care about thread safe, the TRAgent just a single thread application
     */
    TR_LIB_API char *lib_node2path( node_t node );


    /*!
     * \fn lib_get_children
     * Get an interior node's children list
     *
     * \param node The parent node
     * \param children The buffer to store the children list
     *
     * \return The children count when success, -1 when any error
     *
     */
    TR_LIB_API int lib_get_children( node_t node, node_t **children );

    TR_LIB_API node_t lib_get_child( node_t parent, char *name );

    /*!
     * \fn lib_destroy_children
     * Destroy the children returned by callback function lib_get_children
     *
     * \param children The children list
     * \return N/A
     */
    TR_LIB_API void lib_destroy_children( node_t *children );

    /*!
     * \fn lib_current_time
     * Get the current system time
     *
     * \return The time in format require by TR069 protocol
     *
     * \remark Customer does not need to reimplement the function, just copy from the
     * simulator, we have test it under linux and windows XP. We define it in the
     * library just hope it more portable.
     */
    TR_LIB_API const char *lib_current_time( void );

    /*!
     * \fn lib_disk_free_space
     * Get the available disk space of the device
     *
     * \return The disk space size in byte
     */
    TR_LIB_API int lib_disk_free_space( const char *type );

    /*!
     * \fn lib_commit_transaction
     * Commit a transaction
     *
     * \return 0 when success, -1 when any error
     *
     * \remark The library does not need to care about atomic operation. TRAgent has
     * implement it. But, think about some device implement the MOT in XML document or
     * some other likely technology, some operations maybe change the XML document,
     * this function is the only chance to write back the MOT to file system. If the
     * device implement the MOT in some other technology for example sqlite database,
     * it does need to do nothing.
     */
    TR_LIB_API int lib_commit_transaction( void );

    /*!
     * \fn lib_start_ip_ping
     * Start the IP Ping diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_ip_ping( void );

    /*!
     * \fn lib_stop_ip_ping
     * Stop the IP Ping diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_stop_ip_ping( void );

    /*!
     * \fn lib_start_trace_route
     * Start the trace route diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_trace_route( void );

    /*!
     * \fn lib_stop_trace_route
     * Stop the trace route Diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_stop_trace_route( void );

    /*!
     * \brief Start the dsl diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_wan_dsl_diagnostics( char * );

    /*!
     * \brief Stop the dsl diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_stop_wan_dsl_diagnostics( char * );

    /*!
     * \brief Start the atm diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_wan_atmf5loopback_diagnostics( char * );

    /*!
     * \brief Stop the atm diagnostics
     *
     * \return N/A
     */

    TR_LIB_API void lib_stop_wan_atmf5loopback_diagnostics( char * );

#ifdef TR157
    /*!
     *  \brief Start the NS lookup diagnostics
     *
     *  \return N/A
     */
    TR_LIB_API void lib_start_nslookup_diagnostics( void );
    /*!
     *  \brief Stop the NS lookup diagnostics
     *
     *  \return N/A
     */
    TR_LIB_API void lib_stop_nslookup_diagnostics( void );
    /*!
     *  \brief Start the self test diagnostics
     *
     *  \return N/A
     */
    TR_LIB_API void lib_start_selftest_diagnostics( void );
    /*!
     *  \brief Stop the self test diagnostics
     *
     *  \return N/A
     */
    TR_LIB_API void lib_stop_selftest_diagnostics( void );
#endif

    TR_LIB_API void lib_get_interface_ip( const char *inter, char *ip, int ip_len );


    enum {
        TRAFFIC_OUTBOUND, //Outgoing traffic
        TRAFFIC_INBOUND //Incomming traffic
    };

    /*!
     * \brief Get an interface total traffic quatity in byte
     *
     * \param interface The interface name
     * \param direction TRAFFIC_OUTBOUND or TRAFFIC_INBOUNT
     *
     * \return The traffic quatity of the interface
     */
    TR_LIB_API unsigned int lib_get_interface_traffic( const char *inter, int direction );


#ifdef TR069_WIB
    /*!
     * \brief Get an EMSK for wib decrypt
     *
     * \param emsk The EMSK value
     */
    TR_LIB_API int lib_get_emsk( char **emsk );
    /*!
     * \brief Get session timer
     * \return session timer value
     */
    TR_LIB_API int lib_get_wib_session_timer();
    /*!
     * \Get WIB Bootstrap server URL
     * \param wib_url Store WIB URL buffer
     * \param len     Store WIB URL buffer len
     */
    TR_LIB_API int lib_get_wib_url( char *wib_url, int len );
#endif //TR069_WIB


#ifdef TR196
    TR_LIB_API int lib_get_parent_node( node_t child, node_t *parent );
    TR_LIB_API int lib_gen_pm_file( const char *path );
#endif
    TR_LIB_API int lib_get_child_node( node_t parent, const char *name, node_t *child );

    TR_LIB_API int lib_du_uninstall( const char *cmdkey, int number, const char *uuid, const char *version, const char *ee_ref );

    /*!
     * \fn lib_du_install
     *
     * \brief Agent notify CPE one DeploymentUnit package downloading completed, now start to
     *        install it. When CPE completes the installation it MUST notify agent the
     *        installation result(failed or successed).
     *
     * \param cmdkey  The CommandKey parameter provided by ACS, CPE MUST echo this parameter
     *                to agent's CLI API.
     * \param number  The serial number of an operation(Install, Update or Uninstall) in
     *                ChangeDUState RPC method, index from 1. CPE MUST echo this parameter
     *                to agent's CLI API.
     * \param uuid    The UUID parameter provided by ACS. Sometimes ACS does not provide it(an
     *                empty string in this API calling), under this situation the CPE MUST
     *                generate one, following the protocol rules. CPE MUST save it to the new
     *                DU instance's UUID parameter.
     * \param url     The URL that agent used to download the DU package. The CPE MUST save it
     *                to the new DU instance's URL parameter(for later update).
     * \param ee_ref  The ExecEnv instance path. If it is not an empty string, CPE MUST only
     *                install this package to the specified ExecEnv for example OSGi.
     * \param fn      The file name of the package. It is not a path, but only a file name. So
     *                CPE MUST decide where the file saved(under the agent's work directory).
     *                When complete the installation, CPE MUST decide to or not to delete the
     *                package. If not, the package will stay there for ever.
     *
     * \return Any, ignored by agent in this version
     *
     * \remark        The CLI api:
     *                    URL: http://127.0.0.1:{CLIPort}/install/du/complete/
     *                    Method: POST
     *                    Arguments: cmdkey
     *                               number
     *                               instance
     *                               fault_code
     *                    Body encodeing: application/x-www-form-urlencoded
     */
    TR_LIB_API int lib_du_install( const char *cmdkey, int number, const char *uuid, const char *url, const char *ee_ref, const char *fn );
    TR_LIB_API int lib_du_update( const char *cmdkey, int number, const char *instance_number, const char *url, const char *fn );

    TR_LIB_API int lib_generate_dynamic_upload_file( const char *name, const char *path );
    TR_LIB_API int lib_remove_dynamic_upload_file( const char *path );

    TR_LIB_API unsigned int lib_schedule_download_random_time( const char *file_type, unsigned int start, unsigned int end );
    TR_LIB_API int lib_schedule_download_confirmation( const char *cmd_key, const char *user_msg, const char *file_type, unsigned int start, unsigned int end );
    TR_LIB_API int lib_cpe_idle();

#ifdef __ASUS
    /*!
     *  \brief Check xml file whether it's valid
     *
     *  \return N/A
     */
    void check_valid_xml_file( char *tr_xml_path );

//#ifdef TR098	/* start of TR098 */
    /*!
     *  \brief Search all nodes that need to do notification
     *
     */
    int search_notification();

    /*!
     *  \brief Upade the value of all nodes
     *
     */
    int update_node_value();
    /*!
     *  \search tree to find which node to update property
     *
     */
    void compare_bak_xml( char *buf, int noc, char *acl, char *value);

//#endif	/* end of TR098 */
#endif

#ifdef TR106
#ifdef ALIAS
#define Alias_Based_Addressing     "Device.ManagementServer.AliasBasedAddressing"
#define Instance_Mode              "Device.ManagementServer.InstanceMode"
#define Auto_Create_Instances      "Device.ManagementServer.AutoCreateInstances"
#endif //ALIAS
#define ACS_URL                    "Device.ManagementServer.URL"
#define SUMMARY                    "Device.DeviceSummary"
#define LANIP                      "Device.LAN.IPAddress"
#define HARDWAREVERSION            "Device.DeviceInfo.HardwareVersion"
#define SOFTWAREVERSION            "Device.DeviceInfo.SoftwareVersion"
#define SERIALNUMBER               "Device.DeviceInfo.SerialNumber"
#define X_WKS_KEY                  "Device.DeviceInfo.X_WKS_Key"
#define PARAMETERKEY                "Device.ManagementServer.ParameterKey"

#define DEVICE_ID_MANUFACTURER     "Device.DeviceInfo.Manufacturer"
#define DEVICE_ID_OUI              "Device.DeviceInfo.ManufacturerOUI"
#define DEVICE_ID_PRODUCT_CLASS    "Device.DeviceInfo.ProductClass"
#define DEVICE_ID_SERIAL_NUMBER    "Device.DeviceInfo.SerialNumber"

#define PERIODIC_INTERVAL          "Device.ManagementServer.PeriodicInformInterval"
#define PERIODIC_INFORM_TIME       "Device.ManagementServer.PeriodicInformTime"
#define PERIODIC_ENABLE            "Device.ManagementServer.PeriodicInformEnable"
#define ACTIVE_NOTIFICATION_THROTTLE "Device.ManagementServer.DefaultActiveNotificationThrottle"
#define CWMP_ENABLE                "Device.ManagementServer.EnableCWMP"
#define SESSION_AUTH_USERNAME      "Device.ManagementServer.Username"
#define SESSION_AUTH_PASSWORD      "Device.ManagementServer.Password"
#define CRS_USERNAME               "Device.ManagementServer.ConnectionRequestUsername"
#define CRS_PASSWORD               "Device.ManagementServer.ConnectionRequestPassword"
#define CRS_URL            "Device.ManagementServer.ConnectionRequestURL"
#define UDP_NOTIFY_INTERVAL        "Device.ManagementServer.UDPConnectionRequestAddressNotificationLimit"
#define STUN_ENABLE                "Device.ManagementServer.STUNEnable"
#define STUN_SERVER_PORT           "Device.ManagementServer.STUNServerPort"
#define STUN_SERVER_ADDRESS        "Device.ManagementServer.STUNServerAddress"
#define STUN_USERNAME              "Device.ManagementServer.STUNUsername"
#define STUN_PASSWORD              "Device.ManagementServer.STUNPassword"
#define STUN_MIN_KEEPALIVE_INTERVAL    "Device.ManagementServer.STUNMinimumKeepAlivePeriod"
#define STUN_MAX_KEEPALIVE_INTERVAL    "Device.ManagementServer.STUNMaximumKeepAlivePeriod"
#define UDP_CRS_ADDRESS            "Device.ManagementServer.UDPConnectionRequestAddress"
#define NAT_DETECTED               "Device.ManagementServer.NATDetected"
#define IP_PING                    "Device.LAN.IPPingDiagnostics.DiagnosticsState"
#define TRACE_ROUTE                "Device.LAN.TraceRouteDiagnostics.DiagnosticsState"
#define ECHO_ENABLE            "Device.UDPEchoConfig.Enable"
#define ECHO_INTERFACE         "Device.UDPEchoConfig.Interface"
#define ECHO_SOURCE_IP_ADDRESS     "Device.UDPEchoConfig.SourceIPAddress"
#define ECHO_UDP_PORT          "Device.UDPEchoConfig.UDPPort"
#define ECHO_PACKETS_RECEIVED      "Device.UDPEchoConfig.PacketsReceived"
#define ECHO_PACKETS_RESPONDED     "Device.UDPEchoConfig.PacketsResponded"
#define ECHO_BYTES_RECEIVED        "Device.UDPEchoConfig.BytesReceived"
#define ECHO_BYTES_RESPONDED       "Device.UDPEchoConfig.BytesResponded"
#define ECHO_TIME_OF_FIRST_PACKET  "Device.UDPEchoConfig.TimeFirstPacketReceived"
#define ECHO_TIME_OF_LAST_PACKET   "Device.UDPEchoConfig.TimeLastPacketReceived"
#define ECHO_PLUS_SUPPORT          "Device.UDPEchoConfig.EchoPlusSupported"
#define ECHO_PLUS_ENABLE           "Device.UDPEchoConfig.EchoPlusEnabled"

#define DD_STATE                   "Device.DownloadDiagnostics.DiagnosticsState"
#define DD_INTERFACE               "Device.DownloadDiagnostics.Interface"
#define DD_URL                     "Device.DownloadDiagnostics.DownloadURL"
#define DD_DSCP                    "Device.DownloadDiagnostics.DSCP"
#define DD_EP                      "Device.DownloadDiagnostics.EthernetPriority"
#define DD_ROM                     "Device.DownloadDiagnostics.ROMTime"
#define DD_BOM                     "Device.DownloadDiagnostics.BOMTime"
#define DD_EOM                     "Device.DownloadDiagnostics.EOMTime"
#define DD_TESTBR                  "Device.DownloadDiagnostics.TestBytesReceived"
#define DD_TOTALBR                 "Device.DownloadDiagnostics.TotalBytesReceived"
#define DD_TCP_REQUEST_TIME        "Device.DownloadDiagnostics.TCPOpenRequestTime"
#define DD_TCP_RESPONSE_TIME       "Device.DownloadDiagnostics.TCPOpenResponseTime"
#define UD_STATE                   "Device.UploadDiagnostics.DiagnosticsState"
#define UD_INTERFACE               "Device.UploadDiagnostics.Interface"
#define UD_URL                     "Device.UploadDiagnostics.UploadURL"
#define UD_DSCP                    "Device.UploadDiagnostics.DSCP"
#define UD_EP                      "Device.UploadDiagnostics.EthernetPriority"
#define UD_ROM                     "Device.UploadDiagnostics.ROMTime"
#define UD_BOM                     "Device.UploadDiagnostics.BOMTime"
#define UD_EOM                     "Device.UploadDiagnostics.EOMTime"
#define UD_TFL                     "Device.UploadDiagnostics.TestFileLength"
#define UD_TBS                     "Device.UploadDiagnostics.TotalBytesSent"
#define UD_TCP_REQUEST_TIME        "Device.UploadDiagnostics.TCPOpenRequestTime"
#define UD_TCP_RESPONSE_TIME       "Device.UploadDiagnostics.TCPOpenResponseTime"
#ifdef TR196
#define UPLOAD_ENABL               "Device.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadEnable"
#define UPLOAD_URL                 "Device.FAPService.1.PerfMgmt.FileMgmt.URL"
#define UPLOAD_USERNAME            "Device.FAPService.1.PerfMgmt.FileMgmt.Username"
#define UPLOAD_PASSWORD            "Device.FAPService.1.PerfMgmt.FileMgmt.Password"
#define UPLOAD_INTERVAL            "Device.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadInterval"
#define UPLOAD_PIT                 "Device.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadTime"
#endif //TR196
#ifdef TR157
#define NS_DIAGNOSTICS              "Device.NSLookupDiagnostics.DiagnosticsState"
#define SELF_DIAGNOSTICS            "Device.SelfTestDiagnostics.DiagnosticsState"
#endif //TR157
#define RETRY_INTERVAL      "Device.ManagementServer.CWMPRetryMinimumWaitInterval"
#define RETRY_MULTI         "Device.ManagementServer.CWMPRetryIntervalMultiplier"

#define SM_EE           "Device.SoftwareModules.ExecEnv"
#define SM_DU           "Device.SoftwareModules.DeploymentUnit"
#define SM_EU           "Device.SoftwareModules.ExecutionUnit"

#define DSL_DIAGNOSTICS            ".WANDSLDiagnostics.LoopDiagnosticsState"
#define ATM_DIAGNOSTICS            ".WANATMF5LoopbackDiagnostics.DiagnosticsState"

#elif TR181
#define ACS_URL                    "Device.ManagementServer.URL"
#define SUMMARY                    "Device.DeviceSummary"
#define LANIP                      "Device.LAN.IPAddress"
#define HARDWAREVERSION            "Device.DeviceInfo.HardwareVersion"
#define SOFTWAREVERSION            "Device.DeviceInfo.SoftwareVersion"
#define SERIALNUMBER               "Device.DeviceInfo.SerialNumber"
#define X_WKS_KEY                  "Device.DeviceInfo.X_WKS_Key"
#define PARAMETERKEY                "Device.ManagementServer.ParameterKey"

#define Alias_Based_Addressing     "Device.ManagementServer.AliasBasedAddressing"
#define Instance_Mode              "Device.ManagementServer.InstanceMode"
#define Auto_Create_Instances      "Device.ManagementServer.AutoCreateInstances"

#define DEVICE_ID_MANUFACTURER     "Device.DeviceInfo.Manufacturer"
#define DEVICE_ID_OUI              "Device.DeviceInfo.ManufacturerOUI"
#define DEVICE_ID_PRODUCT_CLASS    "Device.DeviceInfo.ProductClass"
#define DEVICE_ID_SERIAL_NUMBER    "Device.DeviceInfo.SerialNumber"

#define PERIODIC_INTERVAL          "Device.ManagementServer.PeriodicInformInterval"
#define PERIODIC_INFORM_TIME       "Device.ManagementServer.PeriodicInformTime"
#define PERIODIC_ENABLE            "Device.ManagementServer.PeriodicInformEnable"
#define ACTIVE_NOTIFICATION_THROTTLE "Device.ManagementServer.DefaultActiveNotificationThrottle"
#define CWMP_ENABLE                "Device.ManagementServer.EnableCWMP"
#define SESSION_AUTH_USERNAME      "Device.ManagementServer.Username"
#define SESSION_AUTH_PASSWORD      "Device.ManagementServer.Password"
#define CRS_USERNAME               "Device.ManagementServer.ConnectionRequestUsername"
#define CRS_PASSWORD               "Device.ManagementServer.ConnectionRequestPassword"
#define CRS_URL            "Device.ManagementServer.ConnectionRequestURL"
#define UDP_NOTIFY_INTERVAL        "Device.ManagementServer.UDPConnectionRequestAddressNotificationLimit"
#define STUN_ENABLE                "Device.ManagementServer.STUNEnable"
#define STUN_SERVER_PORT           "Device.ManagementServer.STUNServerPort"
#define STUN_SERVER_ADDRESS        "Device.ManagementServer.STUNServerAddress"
#define STUN_USERNAME              "Device.ManagementServer.STUNUsername"
#define STUN_PASSWORD              "Device.ManagementServer.STUNPassword"
#define STUN_MIN_KEEPALIVE_INTERVAL    "Device.ManagementServer.STUNMinimumKeepAlivePeriod"
#define STUN_MAX_KEEPALIVE_INTERVAL    "Device.ManagementServer.STUNMaximumKeepAlivePeriod"
#define UDP_CRS_ADDRESS            "Device.ManagementServer.UDPConnectionRequestAddress"
#define NAT_DETECTED               "Device.ManagementServer.NATDetected"
#define IP_PING                    "Device.IP.Diagnostics.IPPing.DiagnosticsState"
#define TRACE_ROUTE                "Device.IP.Diagnostics.TraceRoute.DiagnosticsState"
#define ECHO_ENABLE           	   "Device.IP.Diagnostics.UDPEchoConfig.Enable"
#define ECHO_INTERFACE             "Device.IP.Diagnostics.UDPEchoConfig.Interface"
#define ECHO_SOURCE_IP_ADDRESS     "Device.IP.Diagnostics.UDPEchoConfig.SourceIPAddress"
#define ECHO_UDP_PORT              "Device.IP.Diagnostics.UDPEchoConfig.UDPPort"
#define ECHO_PACKETS_RECEIVED      "Device.IP.Diagnostics.UDPEchoConfig.PacketsReceived"
#define ECHO_PACKETS_RESPONDED     "Device.IP.Diagnostics.UDPEchoConfig.PacketsResponded"
#define ECHO_BYTES_RECEIVED        "Device.IP.Diagnostics.UDPEchoConfig.BytesReceived"
#define ECHO_BYTES_RESPONDED       "Device.IP.Diagnostics.UDPEchoConfig.BytesResponded"
#define ECHO_TIME_OF_FIRST_PACKET  "Device.IP.Diagnostics.UDPEchoConfig.TimeFirstPacketReceived"
#define ECHO_TIME_OF_LAST_PACKET   "Device.IP.Diagnostics.UDPEchoConfig.TimeLastPacketReceived"
#define ECHO_PLUS_SUPPORT          "Device.IP.Diagnostics.UDPEchoConfig.EchoPlusSupported"
#define ECHO_PLUS_ENABLE           "Device.IP.Diagnostics.UDPEchoConfig.EchoPlusEnabled"

#define DD_STATE                   "Device.IP.Diagnostics.DownloadDiagnostics.DiagnosticsState"
#define DD_INTERFACE               "Device.IP.Diagnostics.DownloadDiagnostics.Interface"
#define DD_URL                     "Device.IP.Diagnostics.DownloadDiagnostics.DownloadURL"
#define DD_DSCP                    "Device.IP.Diagnostics.DownloadDiagnostics.DSCP"
#define DD_EP                      "Device.IP.Diagnostics.DownloadDiagnostics.EthernetPriority"
#define DD_ROM                     "Device.IP.Diagnostics.DownloadDiagnostics.ROMTime"
#define DD_BOM                     "Device.IP.Diagnostics.DownloadDiagnostics.BOMTime"
#define DD_EOM                     "Device.IP.Diagnostics.DownloadDiagnostics.EOMTime"
#define DD_TESTBR                  "Device.IP.Diagnostics.DownloadDiagnostics.TestBytesReceived"
#define DD_TOTALBR                 "Device.IP.Diagnostics.DownloadDiagnostics.TotalBytesReceived"
#define DD_TCP_REQUEST_TIME        "Device.IP.Diagnostics.DownloadDiagnostics.TCPOpenRequestTime"
#define DD_TCP_RESPONSE_TIME       "Device.IP.Diagnostics.DownloadDiagnostics.TCPOpenResponseTime"
#define UD_STATE                   "Device.IP.Diagnostics.UploadDiagnostics.DiagnosticsState"
#define UD_INTERFACE               "Device.IP.Diagnostics.UploadDiagnostics.Interface"
#define UD_URL                     "Device.IP.Diagnostics.UploadDiagnostics.UploadURL"
#define UD_DSCP                    "Device.IP.Diagnostics.UploadDiagnostics.DSCP"
#define UD_EP                      "Device.IP.Diagnostics.UploadDiagnostics.EthernetPriority"
#define UD_ROM                     "Device.IP.Diagnostics.UploadDiagnostics.ROMTime"
#define UD_BOM                     "Device.IP.Diagnostics.UploadDiagnostics.BOMTime"
#define UD_EOM                     "Device.IP.Diagnostics.UploadDiagnostics.EOMTime"
#define UD_TFL                     "Device.IP.Diagnostics.UploadDiagnostics.TestFileLength"
#define UD_TBS                     "Device.IP.Diagnostics.UploadDiagnostics.TotalBytesSent"
#define UD_TCP_REQUEST_TIME        "Device.IP.Diagnostics.UploadDiagnostics.TCPOpenRequestTime"
#define UD_TCP_RESPONSE_TIME       "Device.IP.Diagnostics.UploadDiagnostics.TCPOpenResponseTime"

#define NS_DIAGNOSTICS              "Device.DNS.Diagnostics.NSLookupDiagnostics.DiagnosticsState"
#define SELF_DIAGNOSTICS            "Device.SelfTestDiagnostics.DiagnosticsState"
#define DSL_DIAGNOSTICS            "Device.DSL.Diagnostics.ADSLLineTest.DiagnosticsState"
#define ATM_DIAGNOSTICS            "Device.ATM.Diagnostics.F5Loopback.DiagnosticsState"
#ifdef TR196
#define UPLOAD_ENABL               "Device.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadEnable"
#define UPLOAD_URL                 "Device.FAPService.1.PerfMgmt.FileMgmt.URL"
#define UPLOAD_USERNAME            "Device.FAPService.1.PerfMgmt.FileMgmt.Username"
#define UPLOAD_PASSWORD            "Device.FAPService.1.PerfMgmt.FileMgmt.Password"
#define UPLOAD_INTERVAL            "Device.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadInterval"
#define UPLOAD_PIT                 "Device.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadTime"
#endif //TR196
#ifdef TR157
#endif //TR157
#define RETRY_INTERVAL      "Device.ManagementServer.CWMPRetryMinimumWaitInterval"
#define RETRY_MULTI         "Device.ManagementServer.CWMPRetryIntervalMultiplier"

#define SM_EE           "Device.SoftwareModules.ExecEnv"
#define SM_DU           "Device.SoftwareModules.DeploymentUnit"
#define SM_EU           "Device.SoftwareModules.ExecutionUnit"

#ifdef __ASUS
/* start for DSL */
#define DSL_LINE_WAN       "Device.DSL.Line.1"
#define DSL_CHANNEL_WAN    "Device.DSL.Channel.1"
/* end for DSL */

/* start for ATM */
#define DEVICE_ATM      "Device.ATM"
#define ATM_LINK        "Device.ATM.Link"
#define ATM_LINK_NUM    "Device.ATM.LinkNumberOfEntries"
/* end for ATM */

/* start for PTM */
#define DEVICE_PTM      "Device.PTM"
#define PTM_LINK        "Device.PTM.Link"
#define PTM_LINK_NUM    "Device.PTM.LinkNumberOfEntries"
/* end for PTM */

/* start for Ethernet */
#define ETH_IF      "Device.Ethernet.Interface"
#define ETH_IF_NUM  "Device.Ethernet.InterfaceNumberOfEntries"
/* end for Ethernet */

/* start for USB */
#define USB_IF      "Device.USB.Interface"
#define USB_IF_NUM  "Device.USB.InterfaceNumberOfEntries"
#define USB_IF_WAN  "Device.USB.Interface.1"
/* end for USB */

/* start for PPP */
#define PPP_IF      "Device.PPP.Interface"
#define PPP_IF_NUM  "Device.PPP.InterfaceNumberOfEntries"
/* end for PPP */

/* start for IP */
#define IP_IF      "Device.IP.Interface"
#define IP_IF_NUM  "Device.IP.InterfaceNumberOfEntries"
/* end for IP */

/* start for DeviceInfo */
#define VENDOR_CONFIG_FILE_NUM		"Device.DeviceInfo.VendorConfigFileNumberOfEntries"
#define VENDOR_CONFIG_FILE		    "Device.DeviceInfo.VendorConfigFile"
/* end for DeviceInfo */

/* start for ManagementServer */
#define MNG_DEVICE_NUM          "Device.ManagementServer.ManageableDeviceNumberOfEntries"
#define MNG_DEVICE              "Device.ManagementServer.ManageableDevice"
/* end for ManagementServer */    

/* start for processor */
#define PROCESSOR_NUM			"Device.DeviceInfo.ProcessorNumberOfEntries"
#define PROCESSOR			    "Device.DeviceInfo.Processor"
/* end for processor */

/* start for wifi */
#define RADIO			    "Device.WiFi"
#define RADIO_2G            "Device.WiFi.Radio.1"
#define RADIO_5G		    "Device.WiFi.Radio.2"
#define WIFI_SSID		    "Device.WiFi.SSID"
#define WIFI_AP			    "Device.WiFi.AccessPoint"
#define TOTALASSOCIATE_2G	"Device.WiFi.AccessPoint.1.AssociatedDevice"
#define TOTALASSOCIATE_5G	"Device.WiFi.AccessPoint.2.AssociatedDevice"
#define TOTALASSOCIATE		"Device.WiFi.AccessPoint."
/* end for wifi */

/* start for IPPingDiagnostics */
#define IP_PING_INTERFACE		    "Device.IP.Diagnostics.IPPing.Interface"
#define IP_PING_TIMEOUT     		"Device.IP.Diagnostics.IPPing.Timeout"
#define IP_PING_SIZE     		    "Device.IP.Diagnostics.IPPing.DataBlockSize"
#define IP_PING_DSCP                "Device.IP.Diagnostics.IPPing.DSCP"
#define IP_PING_HOST			    "Device.IP.Diagnostics.IPPing.Host"
#define IP_PING_CNT     		    "Device.IP.Diagnostics.IPPing.NumberOfRepetitions"
#define IP_PING_SUCCESS_COUNT		"Device.IP.Diagnostics.IPPing.SuccessCount"
#define IP_PING_FAILURE_COUNT		"Device.IP.Diagnostics.IPPing.FailureCount"
#define IP_PING_AVG_RSP_TIME		"Device.IP.Diagnostics.IPPing.AverageResponseTime"
#define IP_PING_MIN_RSP_TIME		"Device.IP.Diagnostics.IPPing.MinimumResponseTime"
#define IP_PING_MAX_RSP_TIME		"Device.IP.Diagnostics.IPPing.MaximumResponseTime"
/* end for IPPingDiagnostics */

/* start for TraceRouteDiagnostics */
#define TRACEROUTE_INTERFACE		"Device.IP.Diagnostics.TraceRoute.Interface"
#define TRACEROUTE_HOST			    "Device.IP.Diagnostics.TraceRoute.Host"
#define TRACEROUTE_CNT     		    "Device.IP.Diagnostics.TraceRoute.NumberOfTries"
#define TRACEROUTE_TIMEOUT     		"Device.IP.Diagnostics.TraceRoute.Timeout"
#define TRACEROUTE_SIZE     		"Device.IP.Diagnostics.TraceRoute.DataBlockSize"
#define TRACEROUTE_DSCP             "Device.IP.Diagnostics.TraceRoute.DSCP"
#define TRACEROUTE_MAX_HOP 		    "Device.IP.Diagnostics.TraceRoute.MaxHopCount"
#define TRACEROUTE_RSP_TIME         "Device.IP.Diagnostics.TraceRoute.ResponseTime"
#define TRACEROUTE_NUM_HOP 		    "Device.IP.Diagnostics.TraceRoute.RouteHopsNumberOfEntries"
#define	TRACEROUTE_ROUTE_HOPS		"Device.IP.Diagnostics.TraceRoute.RouteHops"
/* end for TraceRouteDiagnostics */

#define	DEVICE_IP                       "Device.IP"
#define LEN_OF_IP_INTERFACE_X           "Device.IP.Interface.x"
#define	DEVICE_PPP                      "Device.PPP"
#define	DEVICE_ETHERNET                 "Device.Ethernet"
#define DEVICE_BRIDGING_BRIDGE          "Device.Bridging.Bridge"
#define DEVICE_BRIDGING_BRIDGE_PORT_ONE "Device.Bridging.Bridge.1.Port"
#define DEVICE_NAT			            "Device.NAT"

/* start for GatewayInfo */
#define	DEVICE_GATEWAYINFO		"Device.GatewayInfo"
/* end for GatewayInfo */

/* start for IPV4forwarding */
#define L3_FORWARDING			"Device.Routing.Router.1.IPv4Forwarding"
#define L3_FORWARDING_NUM		"Device.Routing.Router.1.IPv4ForwardingNumberOfEntries"
/* end for IPV4forwarding */

/* start for HOSTS */
#define HOSTS_NUM			    "Device.Hosts.HostNumberOfEntries"
#define HOSTS_HOST			    "Device.Hosts.Host"
/* end for HOSTS */

/* start for DHCPv4 */
#define CLIENT_DHCP			        "Device.DHCPv4"
#define CLIENT_DHCP_1			    "Device.DHCPv4.Client.1"
#define CLIENT_DHCP_2			    "Device.DHCPv4.Client.2"
#define LANHOST_DHCPSTATICADDR_NUM	"Device.DHCPv4.Server.Pool.1.StaticAddressNumberOfEntries"
#define LANHOST_DHCPSTATICADDR      "Device.DHCPv4.Server.Pool.1.StaticAddress"
/* end for DHCPv4 */

/* start for IPV6rd */
#define IPV6RD				"Device.IPv6rd.InterfaceSetting"
/* end for IPV6rd */

#ifdef TR232
#define BULKDATA			    "InternetGatewayDevice.BulkData"
#define BULKDATA_ENABLE			"InternetGatewayDevice.BulkData.Enable"
#define	BULKDATA_PROFILE		"InternetGatewayDevice.BulkData.Profile"
#endif	//TR232
#endif	//__ASUS

#else //end of TR181 for TR098
#define DEVICE_SUMMARY             "InternetGatewayDevice.DeviceSummary"
#define DEVICE_DESCRIPTION             "InternetGatewayDevice.DeviceInfo.Description"
#define ACS_URL                    "InternetGatewayDevice.ManagementServer.URL"
#define SPECVERSION                "InternetGatewayDevice.DeviceInfo.SpecVersion"
#define HARDWAREVERSION            "InternetGatewayDevice.DeviceInfo.HardwareVersion"
#define SOFTWAREVERSION            "InternetGatewayDevice.DeviceInfo.SoftwareVersion"
#define PROVISIONINGCODE           "InternetGatewayDevice.DeviceInfo.ProvisioningCode"
#define SERIALNUMBER               "InternetGatewayDevice.DeviceInfo.SerialNumber"
#define X_WKS_KEY                  "InternetGatewayDevice.DeviceInfo.X_WKS_Key"
#define PARAMETERKEY               "InternetGatewayDevice.ManagementServer.ParameterKey"

#define DEVICE_ID_MANUFACTURER     "InternetGatewayDevice.DeviceInfo.Manufacturer"
#define DEVICE_ID_OUI              "InternetGatewayDevice.DeviceInfo.ManufacturerOUI"
#define DEVICE_ID_PRODUCT_CLASS    "InternetGatewayDevice.DeviceInfo.ProductClass"
#define DEVICE_ID_SERIAL_NUMBER    "InternetGatewayDevice.DeviceInfo.SerialNumber"

#define PERIODIC_INTERVAL          "InternetGatewayDevice.ManagementServer.PeriodicInformInterval"
#define PERIODIC_INFORM_TIME       "InternetGatewayDevice.ManagementServer.PeriodicInformTime"
#define PERIODIC_ENABLE            "InternetGatewayDevice.ManagementServer.PeriodicInformEnable"
#define ACTIVE_NOTIFICATION_THROTTLE "InternetGatewayDevice.ManagementServer.DefaultActiveNotificationThrottle"
#define CWMP_ENABLE                "InternetGatewayDevice.ManagementServer.EnableCWMP"
#define SESSION_AUTH_USERNAME      "InternetGatewayDevice.ManagementServer.Username"
#define SESSION_AUTH_PASSWORD      "InternetGatewayDevice.ManagementServer.Password"
#define CRS_USERNAME               "InternetGatewayDevice.ManagementServer.ConnectionRequestUsername"
#define CRS_PASSWORD               "InternetGatewayDevice.ManagementServer.ConnectionRequestPassword"
#define CRS_URL                "InternetGatewayDevice.ManagementServer.ConnectionRequestURL"
#define UDP_NOTIFY_INTERVAL        "InternetGatewayDevice.ManagementServer.UDPConnectionRequestAddressNotificationLimit"
#define STUN_ENABLE                "InternetGatewayDevice.ManagementServer.STUNEnable"
#define STUN_SERVER_PORT           "InternetGatewayDevice.ManagementServer.STUNServerPort"
#define STUN_SERVER_ADDRESS        "InternetGatewayDevice.ManagementServer.STUNServerAddress"
#define STUN_USERNAME              "InternetGatewayDevice.ManagementServer.STUNUsername"
#define STUN_PASSWORD              "InternetGatewayDevice.ManagementServer.STUNPassword"
#define STUN_MIN_KEEPALIVE_INTERVAL    "InternetGatewayDevice.ManagementServer.STUNMinimumKeepAlivePeriod"
#define STUN_MAX_KEEPALIVE_INTERVAL    "InternetGatewayDevice.ManagementServer.STUNMaximumKeepAlivePeriod"
#define UDP_CRS_ADDRESS            "InternetGatewayDevice.ManagementServer.UDPConnectionRequestAddress"
#define NAT_DETECTED               "InternetGatewayDevice.ManagementServer.NATDetected"
#define IP_PING                    "InternetGatewayDevice.IPPingDiagnostics.DiagnosticsState"
#define TRACE_ROUTE                "InternetGatewayDevice.TraceRouteDiagnostics.DiagnosticsState"
#define ECHO_ENABLE            "InternetGatewayDevice.UDPEchoConfig.Enable"
#define ECHO_INTERFACE         "InternetGatewayDevice.UDPEchoConfig.Interface"
#define ECHO_SOURCE_IP_ADDRESS     "InternetGatewayDevice.UDPEchoConfig.SourceIPAddress"
#define ECHO_UDP_PORT          "InternetGatewayDevice.UDPEchoConfig.UDPPort"
#define ECHO_PACKETS_RECEIVED      "InternetGatewayDevice.UDPEchoConfig.PacketsReceived"
#define ECHO_PACKETS_RESPONDED     "InternetGatewayDevice.UDPEchoConfig.PacketsResponded"
#define ECHO_BYTES_RECEIVED        "InternetGatewayDevice.UDPEchoConfig.BytesReceived"
#define ECHO_BYTES_RESPONDED       "InternetGatewayDevice.UDPEchoConfig.BytesResponded"
#define ECHO_TIME_OF_FIRST_PACKET  "InternetGatewayDevice.UDPEchoConfig.TimeFirstPacketReceived"
#define ECHO_TIME_OF_LAST_PACKET   "InternetGatewayDevice.UDPEchoConfig.TimeLastPacketReceived"
#define DD_STATE                   "InternetGatewayDevice.DownloadDiagnostics.DiagnosticsState"
#define DD_INTERFACE               "InternetGatewayDevice.DownloadDiagnostics.Interface"
#define DD_URL                     "InternetGatewayDevice.DownloadDiagnostics.DownloadURL"
#define DD_DSCP                    "InternetGatewayDevice.DownloadDiagnostics.DSCP"
#define DD_EP                      "InternetGatewayDevice.DownloadDiagnostics.EthernetPriority"
#define DD_ROM                     "InternetGatewayDevice.DownloadDiagnostics.ROMTime"
#define DD_BOM                     "InternetGatewayDevice.DownloadDiagnostics.BOMTime"
#define DD_EOM                     "InternetGatewayDevice.DownloadDiagnostics.EOMTime"
#define DD_TESTBR                  "InternetGatewayDevice.DownloadDiagnostics.TestBytesReceived"
#define DD_TOTALBR                 "InternetGatewayDevice.DownloadDiagnostics.TotalBytesReceived"
#define DD_TCP_REQUEST_TIME        "InternetGatewayDevice.DownloadDiagnostics.TCPOpenRequestTime"
#define DD_TCP_RESPONSE_TIME       "InternetGatewayDevice.DownloadDiagnostics.TCPOpenResponseTime"
#define UD_STATE                   "InternetGatewayDevice.UploadDiagnostics.DiagnosticsState"
#define UD_INTERFACE               "InternetGatewayDevice.UploadDiagnostics.Interface"
#define UD_URL                     "InternetGatewayDevice.UploadDiagnostics.UploadURL"
#define UD_DSCP                    "InternetGatewayDevice.UploadDiagnostics.DSCP"
#define UD_EP                      "InternetGatewayDevice.UploadDiagnostics.EthernetPriority"
#define UD_ROM                     "InternetGatewayDevice.UploadDiagnostics.ROMTime"
#define UD_BOM                     "InternetGatewayDevice.UploadDiagnostics.BOMTime"
#define UD_EOM                     "InternetGatewayDevice.UploadDiagnostics.EOMTime"
#define UD_TFL                     "InternetGatewayDevice.UploadDiagnostics.TestFileLength"
#define UD_TBS                     "InternetGatewayDevice.UploadDiagnostics.TotalBytesSent"
#define UD_TCP_REQUEST_TIME        "InternetGatewayDevice.UploadDiagnostics.TCPOpenRequestTime"
#define UD_TCP_RESPONSE_TIME       "InternetGatewayDevice.UploadDiagnostics.TCPOpenResponseTime"
#define ECHO_PLUS_SUPPORT          "InternetGatewayDevice.UDPEchoConfig.EchoPlusSupported"
#define ECHO_PLUS_ENABLE           "InternetGatewayDevice.UDPEchoConfig.EchoPlusEnabled"

#ifdef __ASUS
/* start for DeviceInfo */
#define VENDOR_CONFIG_FILE_NUM		"InternetGatewayDevice.DeviceInfo.VendorConfigFileNumberOfEntries"
#define VENDOR_CONFIG_FILE		"InternetGatewayDevice.DeviceInfo.VendorConfigFile"
/* end for DeviceInfo */

/* start for ManagementServer */
#define MNG_DEVICE_NUM			"InternetGatewayDevice.ManagementServer.ManageableDeviceNumberOfEntries"
#define MNG_DEVICE			"InternetGatewayDevice.ManagementServer.ManageableDevice"
/* end for ManagementServer */

/* start for Layer3Forwarding */
#define LANHOST_DHCPSTATICADDR_NUM	"InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPStaticAddressNumberOfEntries"
#define LANHOST_DHCPSTATICADDR         	"InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPStaticAddress"
/* end for Layer3Forwarding*/

/* start for Layer3Forwarding */
#define L3_FORWARDING_NUM		"InternetGatewayDevice.Layer3Forwarding.ForwardNumberOfEntries"
#define L3_FORWARDING           	"InternetGatewayDevice.Layer3Forwarding.Forwarding"
/* end for Layer3Forwarding*/

/*start for Wlanforwarding*/
#define LAN_DEVICE              	"InternetGatewayDevice.LANDevice.1"
#define WLAN_CONFIG             	"InternetGatewayDevice.LANDevice.1.WLANConfiguration"
#define WLAN_2G				"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1"
#define WLAN_5G				"InternetGatewayDevice.LANDevice.1.WLANConfiguration.2"
#define WLAN_2G_1           		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.3"
#define WLAN_2G_2           		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.4"
#define WLAN_2G_3           		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.5"
#define WLAN_5G_1           		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.6"
#define WLAN_5G_2           		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.7"
#define WLAN_5G_3           		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.8"
#define TOTALASSOCIATE_2G		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice"
#define TOTALASSOCIATE_5G		"InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice"
#define TOTALASSOCIATE			"InternetGatewayDevice.LANDevice.1.WLANConfiguration."
/*end for Wlanforwarding*/

/* start for IPPingDiagnostics */
#define IP_PING_INTERFACE		"InternetGatewayDevice.IPPingDiagnostics.Interface"
#define IP_PING_HOST			"InternetGatewayDevice.IPPingDiagnostics.Host"
#define IP_PING_CNT     		"InternetGatewayDevice.IPPingDiagnostics.NumberOfRepetitions"
#define IP_PING_TIMEOUT     		"InternetGatewayDevice.IPPingDiagnostics.Timeout"
#define IP_PING_SIZE     		"InternetGatewayDevice.IPPingDiagnostics.DataBlockSize"
#define IP_PING_DSCP            "InternetGatewayDevice.IPPingDiagnostics.DSCP"
#define IP_PING_SUCCESS_COUNT		"InternetGatewayDevice.IPPingDiagnostics.SuccessCount"
#define IP_PING_FAILURE_COUNT		"InternetGatewayDevice.IPPingDiagnostics.FailureCount"
#define IP_PING_AVG_RSP_TIME		"InternetGatewayDevice.IPPingDiagnostics.AverageResponseTime"
#define IP_PING_MIN_RSP_TIME		"InternetGatewayDevice.IPPingDiagnostics.MinimumResponseTime"
#define IP_PING_MAX_RSP_TIME		"InternetGatewayDevice.IPPingDiagnostics.MaximumResponseTime"
/* end for IPPingDiagnostics */

/* start for TraceRouteDiagnostics */
#define TRACEROUTE_INTERFACE		"InternetGatewayDevice.TraceRouteDiagnostics.Interface"
#define TRACEROUTE_HOST			"InternetGatewayDevice.TraceRouteDiagnostics.Host"
#define TRACEROUTE_CNT     		"InternetGatewayDevice.TraceRouteDiagnostics.NumberOfTries"
#define TRACEROUTE_TIMEOUT     		"InternetGatewayDevice.TraceRouteDiagnostics.Timeout"
#define TRACEROUTE_SIZE     		"InternetGatewayDevice.TraceRouteDiagnostics.DataBlockSize"
#define TRACEROUTE_DSCP             "InternetGatewayDevice.TraceRouteDiagnostics.DSCP"    
#define TRACEROUTE_MAX_HOP 		"InternetGatewayDevice.TraceRouteDiagnostics.MaxHopCount"
#define TRACEROUTE_RSP_TIME         "InternetGatewayDevice.TraceRouteDiagnostics.ResponseTime"
#define TRACEROUTE_NUM_HOP 		"InternetGatewayDevice.TraceRouteDiagnostics.RouteHopsNumberOfEntries"
#define	TRACEROUTE_ROUTE_HOPS		"InternetGatewayDevice.TraceRouteDiagnostics.RouteHops"
/* end for TraceRouteDiagnostics */

/* start for WANDevice */
#define IGD_WANDEVICE           "InternetGatewayDevice.WANDevice"
#define IGD_WANDEVICE_NUM       "InternetGatewayDevice.WANDeviceNumberOfEntries"
//#define	IGD_WANDEVICE_ETH		"InternetGatewayDevice.WANDevice.1"
//#define IGD_WANDEVICE_WCD       "InternetGatewayDevice.WANDevice.1.WANConnectionDevice"
/* end for WANDevice */

#ifdef TR232
#define BULKDATA			"InternetGatewayDevice.BulkData"
#define BULKDATA_ENABLE			"InternetGatewayDevice.BulkData.Enable"
#define	BULKDATA_PROFILE		"InternetGatewayDevice.BulkData.Profile"
#endif	//TR232
#endif	//__ASUS

#ifdef TR196
#define UPLOAD_ENABL               "InternetGatewayDevice.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadEnable"
#define UPLOAD_URL                 "InternetGatewayDevice.FAPService.1.PerfMgmt.FileMgmt.URL"
#define UPLOAD_USERNAME            "InternetGatewayDevice.FAPService.1.PerfMgmt.FileMgmt.Username"
#define UPLOAD_PASSWORD            "InternetGatewayDevice.FAPService.1.PerfMgmt.FileMgmt.Password"
#define UPLOAD_INTERVAL            "InternetGatewayDevice.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadInterval"
#define UPLOAD_PIT                 "InternetGatewayDevice.FAPService.1.PerfMgmt.FileMgmt.PeriodicUploadTime"
#endif //TR196
#ifdef TR157
#define NS_DIAGNOSTICS              "InternetGatewayDevice.NSLookupDiagnostics.DiagnosticsState"
#define SELF_DIAGNOSTICS            "InternetGatewayDevice.SelfTestDiagnostics.DiagnosticsState"
#endif //TR157
#define RETRY_INTERVAL      "InternetGatewayDevice.ManagementServer.CWMPRetryMinimumWaitInterval"
#define RETRY_MULTI         "InternetGatewayDevice.ManagementServer.CWMPRetryIntervalMultiplier"

#define SM_EE           "InternetGatewayDevice.SoftwareModules.ExecEnv"
#define SM_DU           "InternetGatewayDevice.SoftwareModules.DeploymentUnit"
#define SM_EU           "InternetGatewayDevice.SoftwareModules.ExecutionUnit"

#define DSL_DIAGNOSTICS            ".WANDSLDiagnostics.LoopDiagnosticsState"
#define ATM_DIAGNOSTICS            ".WANATMF5LoopbackDiagnostics.DiagnosticsState"

#ifdef ALIAS
#define Alias_Based_Addressing     "InternetGatewayDevice.ManagementServer.AliasBasedAddressing"
#define Instance_Mode              "InternetGatewayDevice.ManagementServer.InstanceMode"
#define Auto_Create_Instances      "InternetGatewayDevice.ManagementServer.AutoCreateInstances"
#endif //ALIAS

#endif //TR106

#ifdef __cplusplus
}

#endif

/* ASUS specfic */
#define X_ASUS_OPENVPNCLIENT     "InternetGatewayDevice.X_ASUS_Specific.X_ASUS_Vpn.X_ASUS_VpnClient.X_ASUS_OpenVPNClient"

#define PING_RESULT 	"/tmp/ping_result"
#define TRACEROUTE_RESULT 	"/tmp/traceroute_result"

#define ATM_F5LOOPBACK_DIAGNOSTIC_RESET_PATH    "/proc/tc3162/atm_f5_loopback_diagnostic_reset"
#define ATM_F5LOOPBACK_DIAGNOSTIC_PATH  "/proc/tc3162/atm_f5_loopback_diagnostic"

enum {
    DIAG_COMPLETE = 0,
    DIAG_ERROR_CANNOT_RESOLVE_HOST_NAME,
    DIAG_ERROR_INTERNAL,
    DIAG_NONE,
    DIAG_ERROR_OTHER
};

int update_flag;

#ifdef __ASUS
int sw_mode;    //current mode

#ifdef TR181
int tr_mode;	//lasttime mode

/* record the last time DUT mode */
#define TR_MODE "tr_mode"
#endif
#endif

#endif
