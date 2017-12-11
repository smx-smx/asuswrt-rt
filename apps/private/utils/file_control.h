#ifndef _CONTROL_FILE_H
#define _CONTROL_FILE_H

typedef enum _file_type {
	FILE_NONE = 0,
	FILE_FW,	//firmware
	FILE_SIG,	//DPI signature file
}file_type_t;

int check_rsasign(char *fname, file_type_t type);
#endif

