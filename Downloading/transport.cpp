// Julia Majkowska 290363
#include "transport_ops.h"

using namespace std;


extern int window_start;//numer bajtu na poczatku okna
extern int window_buf_start; // numer na ktorym bajcie bufora jest poczatek okna

extern FILE * plik; 

int sockfd;
extern int len;
struct sockaddr_in server_address;

int receive_datagram(){
    
    struct sockaddr_in  sender;
    socklen_t           sender_len = sizeof(sender);
    u_int8_t            buffer[IP_MAXPACKET+1];
    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sockfd, &descriptors);
    struct timeval tv; tv.tv_sec = 0; tv.tv_usec = 500;
    int ready = select (sockfd+1, &descriptors, NULL, NULL, &tv);
        ssize_t packet_len=0;
        if( ready < 0) {
            fprintf(stderr, "select error: %s\n", strerror(errno));
            return -1;
        }
        if( ready > 0){
            packet_len = recvfrom (sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);
            if (packet_len < 0) {
                fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
                return packet_len;
            }
            char sender_ip_str[20]; 
            inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
            buffer[packet_len] = 0;
            write_to_buffer(buffer);

            return packet_len;    
    }
    return 0;
   
}
int send_datagram(int start, int len){
    
    string msg = "GET "+ to_string(start)+" "+ to_string(len)+"\n";
    char message[20];
    strcpy(message,msg.c_str());
    ssize_t message_len = strlen(message);
    if (sendto(sockfd, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len) {
        fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
        return -1;
    }
    return 1;
}

long long get_time(struct timeval prev_time){
    struct timeval curtime, tval_result; 
    gettimeofday(&curtime, NULL);
    timersub(&curtime, &prev_time, &tval_result);
    return tval_result.tv_sec*1000 + tval_result.tv_usec;
    
}


int get_package(int len){
    request_window(window_start, len);
    const int NUM_SECONDS = 500;//dlugosc rundy
    struct timeval prev_time;
    gettimeofday(&prev_time, NULL);
    
    while(window_start < len-1){    
        int ans = receive_datagram();
        if(ans < 0) return -1;
        long long elapsed = get_time(prev_time);
        if(elapsed>=NUM_SECONDS){
            if(window_start < len-1){
                int ans = request_window(window_start, len);
                if(ans < 0) return -1;
            }
            gettimeofday(&prev_time, NULL);
        }
    }
    return 1;
}



int main(int argc, char* argv[]){
    if(argc <= 3){
        printf("BLAD WEJSCIA\n");
        return EXIT_FAILURE;
    }
    int port = (int) strtol(argv[1], NULL, 10);
    char* nazwa = argv[2];
    len = (int) strtol(argv[3], NULL, 10);
    if(len == 0 || port == 0){
        printf("BLAD WEJSCIA\n");
        return EXIT_FAILURE;
    }
    plik = fopen(nazwa, "w");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "socket error: %s\n", strerror(errno)); 
        return EXIT_FAILURE;
    }

    bzero (&server_address, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(port);
    inet_pton(AF_INET, "156.17.4.30", &server_address.sin_addr);
    
    int ans = get_package(len);
    if (ans < 0) return EXIT_FAILURE;
    close (sockfd);
    return EXIT_SUCCESS;
    

}