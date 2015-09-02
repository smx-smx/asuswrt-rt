#ifndef __LINUX_BRIDGE_EBT_VLAN_T_H
#define __LINUX_BRIDGE_EBT_VLAN_T_H

#define EBT_VLAN_ID    0x01
#define EBT_VLAN_PRIO  0x02
#define EBT_VLAN_MASK (EBT_VLAN_ID | EBT_VLAN_PRIO)

struct ebt_vlan_t_info
{
	uint16_t id;
	uint8_t prio;
	uint8_t bitmask;
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};
#define EBT_VLAN_TARGET "VLAN"

#endif
