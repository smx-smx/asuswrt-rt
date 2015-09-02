//#include <winsock2.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h> 
#include <arpa/inet.h>

unsigned long mask_table[] = {
	0xffffffff,
	0xfffffffe,
	0xfffffffc,
	0xfffffff8,
	0xfffffff0,
	0xffffffe0,
	0xffffffc0,
	0xffffff80,
	0xffffff00,
	0xfffffe00,
	0xfffffc00,
	0xfffff800,
	0xfffff000,
	0xffffe000,
	0xffffc000,
	0xffff8000,
	0xffff0000,
	0xfffe0000,
	0xfffc0000,
	0xfff80000,
	0xfff00000,
	0xffe00000,
	0xffc00000,
	0xff800000,
	0xff000000,
	0xfe000000,
	0xfc000000,
	0xf8000000,
	0xf0000000,
	0xe0000000,
	0xc0000000,
	0x80000000,
};

#define format_netaddr(addr, str) sprintf(str, "%d.%d.%d.%d", ((addr >> 24) & 0xff), ((addr >> 16) & 0xff), ((addr >> 8) & 0xff), ((addr >> 0) & 0xff))

int calc_netmask(char *start, char *end, char *ostart, char *omask)
{
	unsigned long s, e;
	int n, i;

	s = htonl(inet_addr(start));
	e = htonl(inet_addr(end));
	if(s == 0xffffffff || e == 0xffffffff)
	{
		return -1;
	}

	n = e - s;
	
	for(i=0; i<32; i++)
	{
		if(n < (1<<i))
			break;
	}
	
	if ((s & ~mask_table[i]) != 0)
		return -2;

	format_netaddr(s, ostart);
	format_netaddr(mask_table[i], omask);

	return 1<<i;
}
#if 0
int main(int argc, char **argv)
{
	char start[32], mask[32];

	calc_netmask("192.168.1.12", "192.168.1.15", start, mask);
}
#endif
