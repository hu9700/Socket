#ifndef _RAWLIB_H_
#define _RAWLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_ether.h>//provide strct ethhdr
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <errno.h>

union ethframe
{
	struct
	{
		struct ethhdr 	header;
		unsigned char 	data[ETH_DATA_LEN];
	}field;
	unsigned char		buffer[ETH_FRAME_LEN];
};

int getIndexFromInterface(int sockfd, char *ifname);
unsigned char* getMacFromInterface(int sockfd, char *ifname);
int fillEthframe(union ethframe *frame, char *mac_dest, char *mac_source, unsigned short protocol, void *data, int data_len);
void initSockaddrLowlayer(struct sockaddr *addr, int ifindex,  unsigned char* mac);
#endif
