#ifndef __HEADER_CDUS__
#define __HEADER_CDUS__

int is_validated_uuid( const char *uuid );
int load_cdus( void );
int cdus_process( struct session *ss, char **msg );
void cdus_install_complete( const char *cmd_key, int number, int fault_code, const char *fault_string, const char *instance );
void cdus_update_complete( const char *cmd_key, int number, int fault_code, const char *fault_string );
void cdus_uninstall_complete( const char *cmd_key, int number, int fault_code, const char *fault_string );

#endif
