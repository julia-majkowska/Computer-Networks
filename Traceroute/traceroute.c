//Julia Majkowska 290363
#include "icmp.h"


int check_input(char* ip){
    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    return inet_pton(AF_INET, ip, &recipient.sin_addr);
}

int main(int argc, char* argv[])
{
    sender_len = sizeof(sender);
    char* ip = "8.8.8.8";
    if(argc <= 1) {
        fprintf(stderr, "NO INPUT\n"); 
        return EXIT_FAILURE;
    }
    ip = argv[1];
    if(!check_input(ip)){
        fprintf(stderr, "WRONG INPUT\n"); 
        return EXIT_FAILURE;
    }
    printf("%s\n", ip);
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        printf("socket error: %s\n", strerror(errno)); 
        return EXIT_FAILURE;
    }

    for (int i = 1; i< 50; i++) {
        curhops = i;
        FD_ZERO (&descriptors);
        FD_SET (sockfd, &descriptors);
        for(int j = 0; j<3; j++){
            int blad = send_icmp(sockfd, ip , i);
            if(blad == 0) 
                return EXIT_FAILURE;
        }
        printf("Packages with hop = %d : \n", i);
        int blad = package_receive();
        if(blad == 0) 
            return EXIT_FAILURE;
        if(blad == 2) break;
    }
    return EXIT_SUCCESS;
}
