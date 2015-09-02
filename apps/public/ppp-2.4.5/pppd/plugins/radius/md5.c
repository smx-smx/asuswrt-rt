/*
 * $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/ppp-2.4.5/pppd/plugins/radius/md5.c#1 $
 */
#include "md5.h"

void rc_md5_calc (unsigned char *output, unsigned char *input, unsigned int inlen)
{
	MD5_CTX         context;

	MD5_Init (&context);
	MD5_Update (&context, input, inlen);
	MD5_Final (output, &context);
}
