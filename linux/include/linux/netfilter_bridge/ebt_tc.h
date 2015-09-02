#ifndef __LINUX_BRIDGE_EBT_TC_H_
#define __LINUX_BRIDGE_EBT_TC_H_

struct ebt_tc_info
{
	unsigned char tc;
	int target;
};
#define EBT_TC_TARGET "tc"

#endif

