/* serverpacket.c
 *
 * Construct and send DHCP server packets
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/if.h>

#include "serverpacket.h"
#include "dhcpd.h"
#include "options.h"
#include "common.h"
#include "static_leases.h"
#include "files.h"

#if defined (CWMP) && defined(TR111)
#include <stdlib.h>

extern Tr111_Param_t gTR111_Parm;
#endif

/* send a packet to giaddr using the kernel ip stack */
static int send_packet_to_relay(struct dhcpMessage *payload)
{
	DEBUG(LOG_INFO, "Forwarding packet to relay");

	return kernel_packet(payload, server_config.server, SERVER_PORT,
			payload->giaddr, SERVER_PORT);
}


/* send a packet to a specific arp address and ip address by creating our own ip packet */
static int send_packet_to_client(struct dhcpMessage *payload, int force_broadcast)
{
	uint8_t *chaddr;
	uint32_t ciaddr;

	if (force_broadcast) {
		DEBUG(LOG_INFO, "broadcasting packet to client (NAK)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else if (payload->ciaddr) {
		DEBUG(LOG_INFO, "unicasting packet to client ciaddr");
		ciaddr = payload->ciaddr;
		chaddr = payload->chaddr;
	} else if (ntohs(payload->flags) & BROADCAST_FLAG) {
		DEBUG(LOG_INFO, "broadcasting packet to client (requested)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else {
		DEBUG(LOG_INFO, "unicasting packet to client yiaddr");
		ciaddr = payload->yiaddr;
		chaddr = payload->chaddr;
	}
	return raw_packet(payload, server_config.server, SERVER_PORT,
			ciaddr, CLIENT_PORT, chaddr, server_config.ifindex);
}


/* send a dhcp packet, if force broadcast is set, the packet will be broadcast to the client */
static int send_packet(struct dhcpMessage *payload, int force_broadcast)
{
	int ret;

	if (payload->giaddr)
		ret = send_packet_to_relay(payload);
	else ret = send_packet_to_client(payload, force_broadcast);
	return ret;
}


static void init_packet(struct dhcpMessage *packet, struct dhcpMessage *oldpacket, char type)
{
	init_header(packet, type);
	packet->xid = oldpacket->xid;
	memcpy(packet->chaddr, oldpacket->chaddr, 16);
	packet->flags = oldpacket->flags;
	packet->giaddr = oldpacket->giaddr;
	packet->ciaddr = oldpacket->ciaddr;
	add_simple_option(packet->options, DHCP_SERVER_ID, server_config.server);
#if defined(TCSUPPORT_CT_DHCPC_OPTION125SERVER)
	addVIOptionDebug(packet->options);
#endif

}


/* add in the bootp options */
static void add_bootp_options(struct dhcpMessage *packet)
{
	packet->siaddr = server_config.siaddr;
	if (server_config.sname)
		strncpy(packet->sname, server_config.sname, sizeof(packet->sname) - 1);
	if (server_config.boot_file)
		strncpy(packet->file, server_config.boot_file, sizeof(packet->file) - 1);
}


/* send a DHCP OFFER to a DHCP DISCOVER */
int sendOffer(struct dhcpMessage *oldpacket)
{
	struct dhcpMessage packet;
	struct dhcpOfferedAddr *lease = NULL;
	uint32_t req_align, lease_time_align = server_config.lease;
	uint8_t *req, *lease_time;
	struct option_set *curr;
	struct in_addr addr;

	uint32_t static_lease_ip;
	
	char hostname[HOST_NAME_LEN] = {"N/A"};
	int name_len = 0;
	uint8_t *phostname;
	struct  timespec  curtime;
    int fd;
    struct ifreq ifr;
    uint32_t lanIP;
#if defined(ALIAS_IP)
    uint32_t lanAliasIP;
#endif
	/*use ioctl to get CPE's LAN IP*/
    strcpy(ifr.ifr_name, "br0");
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if( fd < 0 ){
        tcdbg_printf("Failed to open br0\n");
    }
    else{
        if(ioctl(fd, SIOCGIFADDR, &ifr) < 0){
            tcdbg_printf("Failed to ioctl br0\n");
        }
        else{
            lanIP = (((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr).s_addr;
        }
        close(fd);
    }
    
#if defined(ALIAS_IP)
    /*use ioctl to get CPE's Alias LAN IP*/
    strcpy(ifr.ifr_name, "br0:0");
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if( fd < 0 ){
        tcdbg_printf("Failed to open br0:0\n");
    }
    else{
        if(ioctl(fd, SIOCGIFADDR, &ifr) < 0){
            tcdbg_printf("Failed to ioctl br0:0\n");
        }
        else{
            lanAliasIP = (((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr).s_addr;
        }
        close(fd);
    }
#endif
    
	/*Get host name option*/
	
	phostname = get_option(oldpacket, DHCP_HOST_NAME);
	if(phostname){
		memset(hostname, 0 , sizeof(hostname));
		name_len = get_option_len(oldpacket, DHCP_HOST_NAME);
		strncpy(hostname, phostname, name_len >= HOST_NAME_LEN ? (HOST_NAME_LEN - 1) : name_len);		
	}

	init_packet(&packet, oldpacket, DHCPOFFER);

	static_lease_ip = getIpByMac(server_config.static_leases, oldpacket->chaddr);

	/* ADDME: if static, short circuit */
	if(!static_lease_ip)
	{
	/* the client is in our lease/offered table */
	if ((lease = find_lease_by_chaddr(oldpacket->chaddr))) {
		if (!lease_expired(lease)){
			//lease_time_align = lease->expires - time(0);
			clock_gettime(CLOCK_MONOTONIC, &curtime);
			lease_time_align = lease->expires - curtime.tv_sec;
		}
		packet.yiaddr = lease->yiaddr;

	/* Or the client has a requested ip */
	} else if ((req = get_option(oldpacket, DHCP_REQUESTED_IP)) &&

		   /* Don't look here (ugly hackish thing to do) */
		   memcpy(&req_align, req, 4) &&
           
           /*if the requested IP is the same as CPE's LAN IP, 
             don't offer it to LAN PC*/
           (lanIP!=req_align) &&
           #if defined(ALIAS_IP)
           (lanAliasIP!=req_align) &&
           #endif

		   /* and the ip is in the lease range */
		   ntohl(req_align) >= ntohl(server_config.start) &&
		   ntohl(req_align) <= ntohl(server_config.end) &&
		
			!static_lease_ip && !reservedIp(server_config.static_leases, htonl(req_align)) && /* Check that its not a static lease */
			/* and is not already taken/offered */
		   ((!(lease = find_lease_by_yiaddr(req_align)) ||
		
		   /* or its taken, but expired */ /* ADDME: or maybe in here */
		   lease_expired(lease)))) {
				packet.yiaddr = req_align; /* FIXME: oh my, is there a host using this IP? */

			/* otherwise, find a free IP */
	} else {
			/* Is it a static lease? (No, because find_address skips static lease) */
		packet.yiaddr = find_address(0);

		/* try for an expired lease */
		if (!packet.yiaddr) packet.yiaddr = find_address(1);
	}

	if(!packet.yiaddr) {
		LOG(LOG_WARNING, "no IP addresses to give -- OFFER abandoned");
		return -1;
	}

	if (!add_lease(packet.chaddr, packet.yiaddr, server_config.offer_time, hostname)) {
		LOG(LOG_WARNING, "lease pool is full -- OFFER abandoned");
		return -1;
	}

	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);
		if (lease_time_align > server_config.lease)
			lease_time_align = server_config.lease;
	}

	/* Make sure we aren't just using the lease time from the previous offer */
	if (lease_time_align < server_config.min_lease)
		lease_time_align = server_config.lease;
	}
	/* ADDME: end of short circuit */
	else
	{
		/* It is a static lease... use it */
		packet.yiaddr = static_lease_ip;
	}

	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

	curr = server_config.options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

	/*add vendor-specific information */
#if defined (CWMP) && defined(TR111)
	if (check_vi(oldpacket))
		addVIOption(packet.options);
#endif

#ifdef DHCP_PROFILE
	if(gOption240_Param.enable)
		addOPT240Option(packet.options);
#endif

	add_bootp_options(&packet);

	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending OFFER of %s", inet_ntoa(addr));
	return send_packet(&packet, 0);
}


int sendNAK(struct dhcpMessage *oldpacket)
{
	struct dhcpMessage packet;

	init_packet(&packet, oldpacket, DHCPNAK);

#if defined (CWMP) && defined(TR111)
	if (check_vi(oldpacket))
		addVIOption(packet.options);
#endif

#ifdef DHCP_PROFILE
	if(gOption240_Param.enable)
		addOPT240Option(packet.options);
#endif

	DEBUG(LOG_INFO, "sending NAK");
	return send_packet(&packet, 1);
}


int sendACK(struct dhcpMessage *oldpacket, uint32_t yiaddr)
{
	struct dhcpMessage packet;
	struct option_set *curr;
	uint8_t *lease_time;
	uint32_t lease_time_align = server_config.lease;
	struct in_addr addr;
	
	char hostname[HOST_NAME_LEN] = {"N/A"};
	int name_len = 0;
	uint8_t *phostname;

#if defined (CWMP) && defined(TR111)
	/*add client device into manageableDev*/
	if (check_vi(oldpacket))
	{
		oldpacket->yiaddr = yiaddr;
		insertDevice(oldpacket);
	}
#endif

	init_packet(&packet, oldpacket, DHCPACK);
	packet.yiaddr = yiaddr;

	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);
		if (lease_time_align > server_config.lease)
			lease_time_align = server_config.lease;
		else if (lease_time_align < server_config.min_lease)
			lease_time_align = server_config.lease;
	}

	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

	curr = server_config.options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

#if defined (CWMP) && defined(TR111)
	if (check_vi(oldpacket))
		addVIOption(packet.options);
#endif

#ifdef DHCP_PROFILE
	if(gOption240_Param.enable)
		addOPT240Option(packet.options);
#endif

	add_bootp_options(&packet);

	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending ACK to %s", inet_ntoa(addr));

	if (send_packet(&packet, 0) < 0)
		return -1;

	phostname = get_option(oldpacket, DHCP_HOST_NAME);
	if(phostname){
		memset(hostname, 0 , sizeof(hostname));
		name_len = get_option_len(oldpacket, DHCP_HOST_NAME);
		strncpy(hostname, phostname, name_len >= HOST_NAME_LEN ? (HOST_NAME_LEN - 1) : name_len);				
	}
	
	add_lease(packet.chaddr, packet.yiaddr, lease_time_align, hostname);

#ifdef RTCONFIG_TR069
	/* send add host event to tr069 */
	char cmdhost[256] = {0};

	memset(cmdhost, 0, sizeof(cmdhost));
	snprintf(cmdhost, sizeof(cmdhost), "/usr/sbin/sendtocli http://127.0.0.1:1234/hostshost/device \"action=add&ipaddr=%s&hostname=%s\"", inet_ntoa(addr), hostname);
	system(cmdhost);
#endif

	/* after DHCP server assign IP successfully, it need write leases to file*/
	write_leases_enhance(NULL);

	return 0;
}


int send_inform(struct dhcpMessage *oldpacket)
{
	struct dhcpMessage packet;
	struct option_set *curr;

	init_packet(&packet, oldpacket, DHCPACK);

	curr = server_config.options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

	add_bootp_options(&packet);

	return send_packet(&packet, 0);
}
#if defined (CWMP) && defined(TR111)
/* compare two mDev_t . return 0 means equal*/
int devCompare(mDev_t *dev1, mDev_t *dev2)
{
	int result = 1;
	if((dev1 != NULL) &&
		(dev2 != NULL) &&
		/*(memcmp(dev1->clientID, dev2->clientID, CLINETID_LENGTH) == 0) &&*/
		(strcmp(dev1->oui, dev2->oui) == 0) &&
		(strcmp(dev1->sn, dev2->sn) == 0) &&
		(strcmp(dev1->pclass, dev2->pclass) == 0))
		result = 0;
	return result;
}

/* check the vi option */
int check_vi(struct dhcpMessage *packet)
{
	uint8_t *vipos = NULL;
#ifdef TR111_TEST
	uint16_t entNum;
#else
	uint32_t entNum;
#endif
	int viflag = 0;

	if ((vipos = get_option(packet, OPT_VENDOR_INFOR)) != NULL){
	#ifdef TR111_TEST
		memcpy(&entNum, vipos, DSL_FORUM_IANA_NUMBER_LENGTH_TEST);
		LOG(LOG_INFO, "\nEnterprise Number is : %0x", entNum);
		if(entNum != DSL_FORUM_IANA_NUMBER_TEST)
	#else
		memcpy(&entNum, vipos, DSL_FORUM_IANA_NUMBER_LENGTH);
		if(entNum != DSL_FORUM_IANA_NUMBER)
	#endif
		{
			LOG(LOG_INFO, "\nEnterprise Number is error: %0x", entNum);
			return 0;
		}
		viflag = 1;
	}
	return viflag;
}

/* parse mDev_t struct from dhcpMessage */
void parseDev(struct dhcpMessage *packet, mDev_t *dev)
{
	uint8_t* IDpos = get_option(packet, DHCP_CLIENT_ID);
	uint8_t IDlength = CLINETID_LENGTH;
	uint8_t* vipos = get_option(packet, OPT_VENDOR_INFOR);
	uint8_t ouilength = (uint8_t)(*(vipos+6));
	uint8_t snlength = (uint8_t)(*(vipos+8+ouilength));
	uint8_t pclasslength = *(vipos+10+ouilength+snlength);
	if(IDpos)
	{
		IDlength = (uint8_t)(*(IDpos+1));
	}

	if(IDlength > CLINETID_LENGTH)
	{
		IDlength = CLINETID_LENGTH;
	}

	if(IDpos)	
	{
		memcpy(dev->clientID, IDpos+2, sizeof(uint8_t)*IDlength);
	}
	else
	{
		memcpy(&dev->clientID[1], packet->chaddr, CLINETID_LENGTH -1);
	}
	
	memcpy(dev->oui, vipos+7, sizeof(uint8_t)*ouilength);
	memcpy(dev->sn, vipos+9+ouilength, sizeof(uint8_t)*snlength);
	memcpy(dev->pclass, vipos+11+ouilength+snlength, sizeof(uint8_t)*pclasslength);
	dev->next = NULL;
}
void hostindexinit(mDev_t *dev)
{
	int i;
	dev->hostnum =0;
	for(i = 0; i < 8; i++)
	{
		dev->hostindex[i] = 0;
	}
	return;
}
	
void gethostindex(struct dhcpMessage *packet, mDev_t *dev)
{
	unsigned int i;
	int currentLeaseNumer = 0;
	uint32_t tmp_time;

	//hostindexinit(dev);
	for (i = 0; i < gTotalMaxLeases; i++) {
		if (leases[i].yiaddr != 0) {

			/* screw with the time in the struct, for easier writing */
			tmp_time = leases[i].expires;
			
			if (server_config.remaining) {
				if (lease_expired(&(leases[i]))){
					leases[i].expires = 0;
					continue;
				}
			} 
			
			if(packet->yiaddr == leases[i].yiaddr)
			{
				dev->hostnum += 1;
				if((currentLeaseNumer >> 5) >= 8)
					continue;
				dev->hostindex[currentLeaseNumer >> 5] |= (1 << (currentLeaseNumer % 32));
			}
			
			currentLeaseNumer++;		
		}
	}
	return;
}

/* insert device to list */
void insertDevice(struct dhcpMessage *packet)
{
	int exist = 0;

	mDev_t* devp = (mDev_t*)malloc(sizeof(mDev_t));
	if(!devp)
	{		
		LOG(LOG_ERR, "malloc device node failed.");
		return;
	}
	memset(devp, 0, sizeof(mDev_t));
	parseDev(packet, devp);
	#if 0
	exist = findDevice(devp->clientID);
	#else
	//exist = findDevice(devp);
	exist = findDevice(packet, devp);
	#endif
	if (!exist && getDeviceNum() < server_config.max_leases) //prevent attacking
	{
		gTR111_Parm.VITail->next = devp;
		gTR111_Parm.VITail = devp;
		//LOG(LOG_INFO, "Add a device into list.");
#ifdef RTCONFIG_TR069
		char cmd[256] = {0};

		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "/usr/sbin/sendtocli http://127.0.0.1:1234/manageable/device \"action=add&oui=%s&serial=%s&class=%s\"", 
						devp->oui, devp->sn, devp->pclass);
		system(cmd);
#else
		sendCwmpMsg(4); //message type:DEVICE_REINIT
#endif
		writeDevicesFile();
		/* for test */
		//cwmpShowmDev();
	}
	else
	{
		free(devp);
	}
#ifndef RTCONFIG_TR069	
	writeHostIndexFile();
#endif
	return;
}

#if 0
/* find device by id */
int findDevice(uint8_t clientID[])
{
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	while(tmpnode != NULL){
		if (memcmp(tmpnode->clientID, clientID, CLINETID_LENGTH) == 0)
			return 1;
		tmpnode = tmpnode->next;
	}
	return 0;
}
#endif

int findDevice(struct dhcpMessage *packet,mDev_t* pdev)
{
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	while(tmpnode != NULL){
		if (devCompare(tmpnode, pdev) == 0)
		{
			gethostindex(packet, tmpnode);
			return 1;
		}
		tmpnode = tmpnode->next;
	}
	
	hostindexinit(pdev);
	gethostindex(packet, pdev);
	return 0;
}

/* delete device by id */
void deleteDevice(struct dhcpMessage *packet)
{
	//uint8_t* IDpos = get_option(packet, DHCP_SERVER_ID);
	mDev_t* prevnode = gTR111_Parm.VIHeader;
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	while (tmpnode != NULL){
		#if 0
		if (memcmp(tmpnode->clientID, IDpos, CLINETID_LENGTH) == 0)
		#endif
		{
			if(devCompare(tmpnode, gTR111_Parm.VITail) == 0)
			{
				gTR111_Parm.VITail = prevnode;
			}		
			prevnode->next = tmpnode->next;
#ifdef RTCONFIG_TR069
			char cmd[256] = {0};

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "/usr/sbin/sendtocli http://127.0.0.1:1234/manageable/device \"action=del&oui=%s&serial=%s&class=%s\"", 
							tmpnode->oui, tmpnode->sn, tmpnode->pclass);
			system(cmd);
#endif
			free(tmpnode);
			tmpnode = NULL;
			//LOG(LOG_INFO, "Delete a device from list.");
#ifndef RTCONFIG_TR069
			sendCwmpMsg(4); //message type:DEVICE_REINIT
#endif
			writeDevicesFile();
			/* for test */
			//cwmpShowmDev();
			break;
		}
		prevnode = tmpnode;
		tmpnode = tmpnode->next;
	}
#ifndef RTCONFIG_TR069
	writeHostIndexFile();
#endif
}

/* delete device by mac address */
void deleteDeviceByMac(uint8_t *macAddr)
{
	mDev_t* prevnode = gTR111_Parm.VIHeader;
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	while (tmpnode != NULL){
		if (memcmp(&(tmpnode->clientID[1]), macAddr, (CLINETID_LENGTH-1)) == 0){
			if(devCompare(tmpnode, gTR111_Parm.VITail) == 0)
			{
				gTR111_Parm.VITail = prevnode;
			}		
			prevnode->next = tmpnode->next;
#ifdef RTCONFIG_TR069
			char cmd[256] = {0};

			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "/usr/sbin/sendtocli http://127.0.0.1:1234/manageable/device \"action=del&oui=%s&serial=%s&class=%s\"", 
							tmpnode->oui, tmpnode->sn, tmpnode->pclass);
			system(cmd);
#endif
			free(tmpnode);
			tmpnode = NULL;
			//LOG(LOG_INFO, "Delete a device byMac from list.");
			writeDevicesFile();
			/* for test */
			//cwmpShowmDev();
			break;
		}
		prevnode = tmpnode;
		tmpnode = tmpnode->next;
	}
#ifndef RTCONFIG_TR069
	writeHostIndexFile();
#endif
}

/* get length of device list */
unsigned long getDeviceNum(void)
{
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	unsigned long num = 0;
	while (tmpnode != NULL)
	{
		num++;
		tmpnode = tmpnode->next;
	}
	return num;
}

void writeHostIndexFile(void)
{
	FILE *fp;
	char bufNumber[100] = {0};
	char bufIndex[120] = {0};
	int i = 0;//must be initilaze to 0
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;

	if (!(fp = fopen(DEVICE_HOSTINDEX_INFO_FILE, "w")))
	{
		LOG(LOG_ERR, "Unable to open %s for writing", DEVICE_HOSTINDEX_INFO_FILE);
		return;
	}

	sprintf(bufNumber, "totalCount=%lu\n", getDeviceNum());
	fwrite(bufNumber, strlen(bufNumber), 1, fp);
	while (tmpnode != NULL)
	{
		memset(bufNumber,0,sizeof(bufNumber));
		sprintf(bufNumber, "HostCount%d=%lu\n",i, tmpnode->hostnum);
		fwrite(bufNumber, strlen(bufNumber), 1, fp);
		sprintf(bufIndex, "HostIndex%d=%lu %lu %lu %lu %lu %lu %lu %lu\n",i, tmpnode->hostindex[0],tmpnode->hostindex[1], \
			tmpnode->hostindex[2], tmpnode->hostindex[3], tmpnode->hostindex[4], tmpnode->hostindex[5], tmpnode->hostindex[6], tmpnode->hostindex[7]);
		fwrite(bufIndex, strlen(bufIndex), 1, fp);
		i++;
		tmpnode = tmpnode->next;
	}
	fclose(fp);
	return;
}

/* write devices list to file */
void writeDevicesFile(void)
{
/*
	int fd;
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	char buf[MAX_BUF_SIZE];
	int iByte = 0;
	int i = 0;
	
	memset(buf, 0, sizeof(buf));
	
	iByte += sprintf(buf+iByte, "totalCount = %-8lu\n", getDeviceNum());
	while (tmpnode != NULL){
		iByte += sprintf(buf+iByte, "oui%d    = %-6s\n", i, tmpnode->oui);
		iByte += sprintf(buf+iByte, "sn%d     = %-64s\n", i, tmpnode->sn);
		iByte += sprintf(buf+iByte, "pclass%d = %-64s\n", i, tmpnode->pclass);
		i++;
		tmpnode = tmpnode->next;
	}
	
	fd = open(DEVICE_INFO_FILE, O_RDWR | O_CREAT | O_TRUNC);
	if(fd == -1)
	{
		LOG(LOG_ERR, "error on open file");
		return;
	}
	write(fd, buf, iByte);	
	close(fd);
*/	
	FILE *fp;
	char bufNumber[100] = {0};
	char bufoui[100] = {0};
	char bufsn[100] = {0};
	char bufpclass[100] = {0};
	int i = 0;
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	if (!(fp = fopen(DEVICE_INFO_FILE, "w"))) {
		LOG(LOG_ERR, "Unable to open %s for writing", DEVICE_INFO_FILE);
		return;
	}
#ifdef RTCONFIG_TR069
	char buf[128] = {0};

	while (tmpnode != NULL){
		snprintf(buf, sizeof(buf), "%s,%s,%s,%02X:%02X:%02X:%02X:%02X:%02X\n", tmpnode->oui, tmpnode->sn, tmpnode->pclass,
									tmpnode->clientID[1], tmpnode->clientID[2], tmpnode->clientID[3],
									tmpnode->clientID[4], tmpnode->clientID[5], tmpnode->clientID[6]);	
		fwrite(buf, strlen(buf), 1, fp);
		tmpnode = tmpnode->next;
	}
#else
	sprintf(bufNumber, "totalCount = %-8lu\n", getDeviceNum());
	fwrite(bufNumber, strlen(bufNumber), 1, fp);
	while (tmpnode != NULL){
		sprintf(bufoui, "oui%d    = %-6s\n", i, tmpnode->oui);
		sprintf(bufsn, "sn%d     = %-64s\n", i, tmpnode->sn);
		sprintf(bufpclass, "pclass%d = %-64s\n", i, tmpnode->pclass);
		
		fwrite(bufoui, strlen(bufoui), 1, fp);
		fwrite(bufsn, strlen(bufsn), 1, fp);
		fwrite(bufpclass, strlen(bufpclass), 1, fp);
		i++;
		tmpnode = tmpnode->next;
	}
#endif
	fclose(fp);

}

/*show device list info*/
void cwmpShowmDev(void)
{
	mDev_t* tmpnode = gTR111_Parm.VIHeader->next;
	uint32_t num = 0;
	int i;
	num = getDeviceNum();
	LOG(LOG_INFO, "**********show device list start************");
	LOG(LOG_INFO, "num of mdev = %d", num);
	while (tmpnode != NULL)
	{		
		for(i = 0; i<CLINETID_LENGTH; i++)
		{
			LOG(LOG_INFO, "clientID value is 0X%02X.",tmpnode->clientID[i]);
		}
		LOG(LOG_INFO, "oui = %s", tmpnode->oui);
		LOG(LOG_INFO, "sn = %s", tmpnode->sn);
		LOG(LOG_INFO, "pclass = %s", tmpnode->pclass);
		tmpnode = tmpnode->next;
	}
	LOG(LOG_INFO, "**********show device list end************");
}
void dhcpRefresh(void)
{
	char value[10];
	int r_val;
	uint32_t limitTime;
	int attrLimitTime;
	
	unsigned int i;
	unsigned long tmp_time;
	uint8_t macAddr[CLINETID_LENGTH-1] = {0};

	for (i = 0; i < gTotalMaxLeases; i++) {
		if (leases[i].yiaddr != 0) {
			/* screw with the time in the struct, for easier writing */
			tmp_time = leases[i].expires;
			if (dhcp_config.remaining) {
				if (lease_expired(&(leases[i])))
				{
					//LOG(LOG_INFO, "entry[%d] expired", i);
					/* remove dhcp client from ManageableDev_S*/
					memcpy(macAddr, leases[i].chaddr, sizeof(uint8_t)*(CLINETID_LENGTH-1));
					deleteDeviceByMac(macAddr);					
				}
			}
		}
	}

#ifndef RTCONFIG_TR069
	memset(value, 0, sizeof(value));
	
	r_val = tcapi_get("DhcpClientLimit_Entry", "time", value);
	limitTime = strtoul(value, NULL, 10);
	
	r_val = tcapi_get("TR069Attr_SpeAttr", "dhcpClientCount", value);
	attrLimitTime = atoi(value);
	
	if(attrLimitTime & ATTRIBUTE_ACTIVE)
	{
		if(1 == gTR111_Parm.iNotifyFlag)
		{
			if(gTR111_Parm.iTotalNumber != getDeviceNum())
			{
				sendCwmpMsg(2); //messaye type:VALUE_CHANGED
				gTR111_Parm.iTotalNumber = getDeviceNum();
				gTR111_Parm.iNotifyFlag = 0;
			}
		}
		else
		{
			if(gTR111_Parm.iNotifyTimer >= limitTime)
			{
				gTR111_Parm.iNotifyTimer = 0; //reset timer
				if(gTR111_Parm.iTotalNumber != getDeviceNum())
				{
					sendCwmpMsg(2); //messaye type:VALUE_CHANGED
					gTR111_Parm.iTotalNumber = getDeviceNum();
				}
				else
				{
					gTR111_Parm.iNotifyFlag = 1;
				}
			}
			else
			{
				gTR111_Parm.iNotifyTimer++;
			}
		}
	}
#endif
}
#endif

#ifdef RTCONFIG_TR069
void deleteHost(struct dhcpMessage *oldpacket, uint32_t yiaddr)
{
	struct in_addr addr;
	char cmdhost[256] = {0};

	addr.s_addr = yiaddr;
	memset(cmdhost, 0, sizeof(cmdhost));
	snprintf(cmdhost, sizeof(cmdhost), "/usr/sbin/sendtocli http://127.0.0.1:1234/hostshost/device \"action=del&ipaddr=%s\"", inet_ntoa(addr));
	system(cmdhost);	
}
#endif
