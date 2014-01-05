#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_ether.h>//provide strct ethhdr
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include "../rawlib/rawlib.h"

#define MAX_THREAD_NUM 256

struct thread_struct
{
	pthread_t thread_list[MAX_THREAD_NUM];
	int thread_number;
	pthread_mutex_t mutex;
}

static struct thread_struct thread_struct;


int main(int argc, char **argv)
{
	char *ifname = "eth0";
	unsigned short ether_protocol = 0xfe;
	int sockfd = 0;
	int ifindex = 0;
	union ethframe frame_buff;
	unsigned char data_buff[ETH_DATA_LEN] = {0};
	unsigned char * mac_temp = NULL;
	unsigned char * mac_self[ETH_ALEN];// = {0x2e, 0x40, 0x41, 0xbd, 0x6c, 0x0a};
	unsigned char * mac_from[ETH_ALEN] = {0x10, 0xFE, 0xED, 0x18, 0x0B, 0x96};
	unsigned char * mac_get[ETH_ALEN] = {0};
	struct sockaddr addr_self;
	struct sockaddr addr_from;
	struct sockaddr addr_get;
	int addr_get_len = sizeof(addr_get);
	int res;
	
	memset(&frame_buff, 0, sizeof(frame_buff));
	memset(data_buff, 0, sizeof(data_buff));
	memset(&addr_self, 0, sizeof(addr_self));
	memset(&addr_from, 0, sizeof(addr_from));
	memset(&addr_get, 0, sizeof(addr_get));		
	
	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ether_protocol));
	if(sockfd < 0)goto errorhandle;
	
	ifindex = getIndexFromInterface(sockfd, ifname);
	if(ifindex < 0)goto errorhandle;
	
	mac_temp = getMacFromInterface(sockfd, ifname);
	if(mac_temp == NULL)goto errorhandle;
	memcpy(mac_self, mac_temp, ETH_ALEN);
	
	initSockaddrLowlayer(addr_self, ifindex, mac_self);
	
	{	
		memset(thread_struct.thread_list, 0, sizeof(thread_struct.thread_list));
		thread_struct.thread_number = 0;
		pthread_mutex_init(thread_struct.mutex, NULL);
		
		while(1)
		{
			int frame_len_get =0;
			frame_len_get = recvfrom(sockfd, frame_buff, ETH_FRAME_LEN, 0, &addr_get, &addr_get_len);

			if(frame_len_get > 0)
			{
				
	
		
	
			}
			else
			{
				goto errorhandle;
			}
		}
	}
	
	
errorhandle:
	perror(NULL);
	exit(1);
}
