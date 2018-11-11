//Julia Majkowska 290363
#include "icmp.h"
#define _BSD_SOURCE


int mine(void *buf){
    struct iphdr *ip = buf;
    struct icmphdr *icmp = buf+ip->ihl*4;
    if(icmp ->type == 0) return icmp->un.echo.id == getpid();
    if(icmp ->type == 11){
        struct icmphdr* oriicmp= buf+48;
        return oriicmp->un.echo.id == getpid();
    }
    return 0;
}

int get_id(void *buf){
    struct iphdr *ip = buf;
    struct icmphdr *icmp = buf+ip->ihl*4;
    if(icmp ->type == 0) return icmp->un.echo.sequence;
    if(icmp ->type == 11){
        struct icmphdr* oriicmp= buf+48;
        return oriicmp->un.echo.sequence;
    }
    return -1;
}

int dest(void *buf){
    struct iphdr *ip = buf;
    struct icmphdr *icmp = buf+ip->ihl*4;
    if(icmp ->type == 0) return 1;
    return 0;
}

int good(void *buf){
    return mine(buf) && (get_id(buf)/3 == curhops + 1);
}

struct timeval get_time(void *buf){
    struct timeval curtime, tval_result; 
    gettimeofday(&curtime, NULL);
    timersub(&curtime, &times[get_id(buf) %3], &tval_result);
    return tval_result;
    
}

int package_receive(){
    int got = 0, missing = 0;
    char adresy[3][30];
    long long int czas[3] = {0, 0, 0};
    long long tavr = 0;
    int wynik = 1;
    while(got < 3){ 

        struct timeval tv; tv.tv_sec = 1; tv.tv_usec = 0;
        int ready = select (sockfd+1, &descriptors, NULL, NULL, &tv);
        ssize_t packet_len=0;
        if( ready < 0) {
            fprintf(stderr, "select error: %s\n", strerror(errno));
            return 0;
        }
        if( ready == 0 ) {
            missing++;
        }
        if( ready > 0){
            packet_len = recvfrom (sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);
            if (packet_len < 0) {
                fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
                return 0;
            }
                
            if(!mine(buffer) ) continue;
            if(dest(buffer) ) wynik = 2;
            struct timeval t = get_time(buffer);
            czas[got] = t.tv_usec;
            inet_ntop(AF_INET, &(sender.sin_addr), adresy[got], sizeof(adresy[got]));
            tavr += t.tv_usec;

        }
        got++;
            
    }
    
    if(missing >=3 ) printf("***\n");
    else{
        //printf("Received packet from: %s", adresy[0]);
        printf("Received packet from: ");
        if(czas[0] != 0){
            printf("%s ", adresy[0]);
        }
        if(czas[1] != 0 && strcmp(adresy[1], adresy[0])!=0){
            printf("%s ", adresy[1]);
        }
        if(czas[2] != 0 && strcmp(adresy[2], adresy[1])!=0 && strcmp(adresy[2], adresy[0])!=0){
            printf("%s ", adresy[2]);
        }
        if(!missing) printf("\nAvarage time = %lld ms\n", (tavr/3000));
        else printf("\nAvarage time = ???\n");
    }
    return wynik; // 1 jak TE a 2 jak echo response
}



