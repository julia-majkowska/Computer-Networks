//Julia Majkowska 290363
#include "icmp.h"
#define MAXPACKET (65536 - 60 - 8)/* max packet size */
int sequence_id = 0; 
struct timeval times[3];
int cur_probe = 0;
u_int16_t compute_icmp_checksum (const void *buff, int length)
{
    u_int32_t sum;
    const u_int16_t* ptr = buff;
    assert (length % 2 == 0);
    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t)(~(sum + (sum >> 16)));
}

struct icmphdr make_header(){
    
    struct icmphdr icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.un.echo.id = getpid();
    icmp_header.un.echo.sequence = sequence_id++;
    icmp_header.checksum = 0;
    icmp_header.checksum = compute_icmp_checksum (
    (u_int16_t*)&icmp_header, sizeof(icmp_header));
    return icmp_header;
}


int send_icmp(int sockfd, char* ip, int ttl){
    struct icmphdr icmp_header =make_header();
    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &recipient.sin_addr);
    setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
    
    ssize_t bytes_sent = sendto (
        sockfd,
        &icmp_header,
        sizeof(icmp_header),
        0,
        (struct sockaddr*)&recipient,
        sizeof(recipient)
    );
    if(bytes_sent < 0){
        fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
        return 0;
    }
    
    gettimeofday(&times[(sequence_id + 2)%3], NULL) ;

    return 1;
}




