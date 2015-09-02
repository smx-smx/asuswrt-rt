// written by liuchong the struct Subscrible_Event is used
// to store the subscrible string
//
/*
char * SerialID[] = { 
		"3456a3bc-3da1-553d-21e3-23ace438dc57",
		"3456a3bc-3da1-553d-21e3-23ace438dc58",
		"3456a3bc-3da1-553d-21e3-23ace438dc59"
};
*/


struct Subscrible_Event {
	int      ServerPort;
	char     desc[10];
	char     ServerIp[24];
	char     Subcmd[64];
	char     http_ver[16];
	char     sid[64];
	long     seq;
	struct Subscrible_Event *next;
	struct Subscrible_Event *previous;
};

struct  Thread_Event {
	char   ServerIp[24];
	char   sid[64];
	long   seq;
	char   notifyitem[64];
	char   value[64];

};


