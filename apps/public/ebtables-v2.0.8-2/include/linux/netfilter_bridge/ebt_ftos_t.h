#ifndef __LINUX_BRIDGE_EBT_FTOS_T_H
#define __LINUX_BRIDGE_EBT_FTOS_T_H

struct ebt_ftos_info
{
	unsigned char ftos;
	unsigned char mask;
    int target;
};
#define EBT_FTOS_TARGET "ftos"
      
#endif
