#ifdef RTCONFIG_OPENVPN

#define MAX_OPENVPN_ACCNT	32

typedef struct openvpn_accnt
{
	char name[128];
	char passwd[128];
} openvpn_accnt_t;

typedef struct openvpn_accnt_info
{
	openvpn_accnt_t account[MAX_OPENVPN_ACCNT];
	int count;
} openvpn_accnt_info_t;

extern int get_openvpn_account(openvpn_accnt_info_t *account_info);
#endif


