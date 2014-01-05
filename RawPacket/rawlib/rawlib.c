#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_ether.h>//provide strct ethhdr
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "rawlib.h"

int getIndexFromInterface(int sockfd, char *ifname)
{
	struct ifreq buffer;
	int ifindex;
	int res;
	memset(&buffer, 0x00, sizeof(buffer));
	strncpy(buffer.ifr_name, ifname, IFNAMSIZ);
	res = ioctl(sockfd, SIOCGIFINDEX, &buffer);

	if(res < 0)
	{
		perror("ioctl get index fail");
		return res;
	}
	else
	{
		ifindex = buffer.ifr_ifindex;
		return ifindex;
	}
}

unsigned char* getMacFromInterface(int sockfd, char *ifname)
{
	struct ifreq buffer;
	static unsigned char mac[ETH_ALEN] = {0};
	int res;
	memset(&buffer, 0x00, sizeof(buffer));
	strncpy(buffer.ifr_name, ifname, IFNAMSIZ);
	res = ioctl(sockfd, SIOCGIFHWADDR, &buffer);

	if(res < 0)
	{
		perror("ioctl get mac fail");
		return NULL;
	}
	else
	{
		memcpy((void *)mac, (void *)(buffer.ifr_hwaddr.sa_data), ETH_ALEN);
		return mac;
	}	
}

int fillEthframe(union ethframe *frame, char *mac_dest, char *mac_source, unsigned short protocol, void *data, int data_len)
{
	if(data_len < ETH_DATA_LEN)
	{
		memcpy((frame->field).header.h_dest, mac_dest, ETH_ALEN);
		memcpy((frame->field).header.h_source, mac_source, ETH_ALEN);
		(frame->field).header.h_proto = htons(protocol);
		memcpy((frame->field).data, data, data_len);
		return data_len;
	}
	else
	{
		printf("data is too long for ethernet frame\n");
		return -1;
	}
}

void initSockaddrLowlayer(struct sockaddr *addr, int ifindex,  unsigned char* mac)
{
	struct sockaddr_ll *addr_ll = (struct sockaddr_ll *)addr;
	
	memset((void *)addr_ll, 0, sizeof(struct sockaddr));
	
	addr_ll->sll_family = PF_PACKET;
	addr_ll->sll_ifindex = ifindex;
	addr_ll->sll_halen = ETH_ALEN;
	memcpy((void *)(addr_ll->sll_addr), (void *)mac, ETH_ALEN);
}
