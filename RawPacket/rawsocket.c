#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_ether.h>//provide strct ethhdr
#include <linux.if_packet.h>
#include <sys/ioctl.h>
#include <errno.h>

static char *ifname = "wlan1";
static unsigned char ether_dest[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static unsigned short ether_protocol = 0xfe;
static unsigned char data[] = "hello world";
static unsigned short data_len = strlen(data);

union ethframe
{
	struct
	{
		struct ethhdr 	header;
		unsigned char 	data[ETH_DATA_LEN];
	}field;
	unsigned char		buffer[ETH_FRAME_LEN];
};

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

char* getIfname(int sockfd)
{
	struct ifreq buffer;
	static char ifname[IFNAMSIZ];
	memset(&buffer, 0x00, sizeof(buffer));
	res = ioctl(sockfd, SIOCGIFNAME, ifname);
	
	if(res < 0)
	{
		perror("ioctl get ifname fail");
		return NULL;
	}
	else
	{
		strncpy(ifname, buffer.ifr_name, IFNAMSIZ);
		return ifname;
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

int initSockaddrLowlayer(struct sockaddr *addr, int ifindex,  unsigned char* mac)
{
	struct sockaddr_ll *addr_ll = (struct sockaddr_ll *)addr;
	
	memset((void *)addr_ll, 0, sizeof(struct sockaddr));
	
	addr_ll->sll_family = PF_PACKET;
	addr_ll->sll_ifindex = ifindex;
	addr_ll->sll_halen = ETH_ALEN;
	memcopy((void *)(addr_ll->sll_addr), (void *)mac, ETH_ALEN);
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr addr_source;
	struct sockaddr addr_dest;
	union ethframe frame_buff;
//	unsigned char data[ETH_FRAME_LEN];
//	int data_len = 0;
//	char *ifname = "wlan1";
//	unsigned char ether_dest[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
//	unsigned short ether_protocol = 0xfe;
	int ifindex;
	
	memset((void *)(&frame_buff), 0, sizeof(union ethframe));
	memset((void *)data, 0, ETH_FRAME_LEN);
	
	sockfd = socket(AF_PACKET, SOCK_RAW, htons(proto));//only for root
	if(sockfd < 0)
	{
		perror("socket fail");
		exit(1);
	}
	
	ifindex = getIndexFromInterface(sockfd, ifname);
	
	
	
	
}
