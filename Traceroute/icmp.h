//Julia Majkowska 290363
#include <unistd.h>
#include <assert.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <inttypes.h>
#include <time.h>
#include <strings.h>
int sockfd;
int curhops;
struct sockaddr_in sender;
socklen_t sender_len;
u_int8_t buffer[IP_MAXPACKET];
fd_set descriptors;
struct timeval times[3];
//u_int16_t compute_icmp_checksum (const void *buff, int length);
//struct icmphdr make_header*();
int send_icmp(int sockfd, char* ip, int ttl);
//int check_input (char * ip)
int package_receive();

