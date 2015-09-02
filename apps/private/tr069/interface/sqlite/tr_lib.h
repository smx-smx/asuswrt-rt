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
        TYPE_ANY,
    };

#define PROPERTY_LENGTH 256

#ifdef __V4_2
    union dev_ {
        int ( *get )( int *, int, char **, int *, int * );
        int ( *set )( int *, int, char *, int, int );
        int ( *add )( int *, int, unsigned int );
        int ( *del )( int *, int, unsigned int );
    } ;
#endif
    /*
     * \remark Very important information:
     *
     * The type node_t present the storage of the device's MOT tree node. Device vendor
     * should define it as needness, normally, it is pointer to a structure, for example:
     *
     * struct priv_node {
     *      const char *name;
     *      struct priv_node *parent;
     *      struct priv_node *sibling;
     *      struct priv_node *first_child, *last_child;
     *
     *      struct {
     *          ...;
     *      } property;
     *      struct {
     *          ...;
     *      } value;
     * };
     *
     * typedef struct priv_node * node_t;
     *
     * \note Theoretically, the node_t can be any simple or complex type, but for
     * portabilityp, lease define the node_t as a pointer, in our sqlite sample code
     * we just define it as an integer to represent the index of a database record.
     */
    typedef int node_t;


    /*!
     * \brief Notify the device that download some file complete
     *
     *  \param type The file type
     *  \param path The path the file was saved
     *
     *  \return If or not need reboot the device
     */
    TR_LIB_API int lib_download_complete( const char *type, const char *path );

    /*!
     * \brief Initiate the library
     *
     * \param arg The configuration item value of init in tr.conf
     * \return 0 when success, or less return -1
     *
     * \remark if this function return an error, TRAgent will exit abnormally.
     */
    TR_LIB_API int lib_init( const char *arg );

    /*!
     * \brief Notify the device to do factoryreset
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
     * \brief Reboot the device
     *
     * \return always be 0
     */
    TR_LIB_API int lib_reboot( void );

    /*!
     * \brief Resolve the MOT node path to an internal structure(node_t)
     *
     * \param path The MOT node's path, for example "InternetGatewayDevice.A.B.C"
     * \param node The internal presentation of the MOT node
     *
     * \return 0 when success, 1 when the node does not existing, -1 when any error
     */
    TR_LIB_API int lib_resolve_node( const char *path, node_t *node );

    /*!
     * \brief To retrieve a given(by name) property of a given node
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
     * \brief Replace a given property of a given node
     *
     * \param node The node whose property will be replaced
     * \param name The property name
     * \param prop The new property value
     *
     * \return 0 when success, -1 when any error
     */
    TR_LIB_API int lib_set_property( node_t node, const char *name, const char prop[PROPERTY_LENGTH] );

    /*!
     * \brief Retrieve a leaf node's value
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
     * \brief To free the memory allocated by lib_get_value()
     *
     * \param value The memory's pointer
     * \return N/A
     */
    TR_LIB_API void lib_destroy_value( char *value );

    /*!
     * \brief Replace a given leaf node's value
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
     * \brief Add an object instance under
     *
     * \param parent The parent node which the new instance(a sub tree) will be added under
     * \param cin The current instance number, the callback function MUST use it as the
     * new instance's root node name.
     *
     * \return 0 when success, -1 when any error
     */
    TR_LIB_API int lib_ao( node_t parent, int cin );

    /*!
     * \brief Delete an object instance which created by lib_ao
     *
     * \param node The instance sub tree'root node
     * \return 0 when success, -1 when any error
     */
    TR_LIB_API int lib_do( node_t node );

    /*!
     * \brief Notify the library that TRAgent will start a session
     *
     * \return The reference count of started session, 0 or a minus number means failure
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
     * \brief Notify the library that the session is over
     *
     * \return N/A
     */
    TR_LIB_API int lib_end_session( void );

    /*!
     * \brief Resove the node structure to a string path - converting with lib_resolve_node()
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
     * \brief Get an interior node's children list
     *
     * \param node The parent node
     * \param children The buffer to store the children list
     *c
     * \return The children count when success, -1 when any error
     *
     */
    TR_LIB_API int lib_get_children( node_t node, node_t **children );

    /*!
     * \brief Destroy the children returned by callback function lib_get_children
     *
     * \param children The children list
     * \return N/A
     */
    TR_LIB_API void lib_destroy_children( node_t *children );

    /*!
     * \brief Get the current system time
     *
     * \return The time in format require by TR069 protocol
     *
     * \remark Customer does not need to reimplement the function, just copy from the
     * simulator, we have test it under linux and windows XP. We define it in the
     * library just hope it more portable.
     */
    TR_LIB_API const char *lib_current_time( void );

    /*!
     * \brief Get the available disk space of the device
     *
     * \return The disk space size in byte
     */
    TR_LIB_API int lib_disk_free_space( const char *type );

    /*!
     * \brief Commit a transaction
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
     * \brief Start the IP Ping diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_ip_ping( void );

    /*!
     * \brief Stop the IP Ping diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_stop_ip_ping( void );

    /*!
     * \brief Start the trace route diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_trace_route( void );

    /*!
     * \brief Stop the trace route Diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_stop_trace_route( void );

    /*!
     * \brief Start the dsl diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_dsl_diagnostics( char * );

    /*!
     * \brief Stop the dsl diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_stop_dsl_diagnostics( char * );

    /*!
     * \brief Start the atm diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_start_atm_diagnostics( char * );

    /*!
     * \brief Stop the atm diagnostics
     *
     * \return N/A
     */
    TR_LIB_API void lib_stop_atm_diagnostics( char * );

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


    /*!
     * \brief Get an NIC interface's IP address
     *
     * \param inter The interface name, for example "eth0"
     * \param ip The buffer to put the interface's IP like "1.2.3.4"
     * \param ip_len The buffer length
     */
    TR_LIB_API void lib_get_interface_ip( const char *inter, char *ip, int ip_len );

    enum {
        TRAFFIC_OUTBOUND, //Outgoing traffic
        TRAFFIC_INBOUND //Incomming traffic
    };

    /*!
     * \brief Get an interface total traffic quatity in byte
     *
     * \param inter The interface name
     * \param direction TRAFFIC_OUTBOUND or TRAFFIC_INBOUNT
     *
     * \return The traffic quatity of the interface
     */
    TR_LIB_API unsigned int lib_get_interface_traffic( const char *inter, int direction );



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

#ifdef TR196
    TR_LIB_API int lib_get_parent_node( node_t child, node_t *parent );
    TR_LIB_API int lib_get_child_node( node_t parent, const char *name, node_t *child );
    TR_LIB_API int lib_gen_pm_file( const char *path );
#endif


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

#ifdef TR106
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
#define ECHO_ENABLE            "Device.IP.Diagnostics.UDPEchoConfig.Enable"
#define ECHO_INTERFACE         "Device.IP.Diagnostics.UDPEchoConfig.Interface"
#define ECHO_SOURCE_IP_ADDRESS     "Device.IP.Diagnostics.UDPEchoConfig.SourceIPAddress"
#define ECHO_UDP_PORT          "Device.IP.Diagnostics.UDPEchoConfig.UDPPort"
#define ECHO_PACKETS_RECEIVED      "Device.IP.Diagnostics.UDPEchoConfig.PacketsReceived"
#define ECHO_PACKETS_RESPONDED     "DDevice.IP.Diagnostics.UDPEchoConfig.PacketsResponded"
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


#else //end of TR181 for TR098
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
#endif //TR106

#ifdef __cplusplus
}

#endif

#endif
