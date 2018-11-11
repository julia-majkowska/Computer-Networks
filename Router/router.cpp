#include "routing_ops.h"
//#include "structures.h"
using namespace std;

extern map< network , route> routing;
extern vector<network> neighbours;
extern map<network, int> last_round_heard;

extern long long cur_round;

int write_network(int sockfd, struct sockaddr_in server_address,in_addr ip, network neigh, network n, int dist){
       
        //printf("%s\n", inet_ntoa(ip));
        server_address.sin_addr = ip;

        char message[9];
        ((int*) message)[0] = htonl(n.ip); 
        message[4] = n.mask; 
        char *temp = message + 5;
        *((int*) temp) = htonl(dist);
        //write_buff(message, 9);
	ssize_t message_len = 9;
	if (sendto(sockfd, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len) {
                string unr = "unreachable";
                routing[neigh] = route(max_dist, unr.c_str());//jezeli nie moge wyslac to zaznacz brak polaczenia
                delete_indirect(neigh);	
	}

	//close (sockfd);
	return 1;
}




void write_vector(int sockfd, struct sockaddr_in server_address){
    
    for(pair<network, route>  net : routing){
        for (network neigh : neighbours){
                write_network(sockfd, server_address, neigh.broadcast(), neigh, net.first, net.second.dist);        
        }
    }
}



int receive_and_parse(int sockfd) 
{
    struct sockaddr_in  sender;
    socklen_t           sender_len = sizeof(sender);
    u_int8_t            buffer[IP_MAXPACKET+1];
    //select zeby nie czekal dluzej niz ture
    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sockfd, &descriptors);
    struct timeval tv; tv.tv_sec = 1; tv.tv_usec = 0;
    int ready = select (sockfd+1, &descriptors, NULL, NULL, &tv);
    if( ready < 0) {
        fprintf(stderr, "select error: %s\n", strerror(errno));
        return 0;
    }
    if( ready == 0 ) {
        return 1;
    }
    if( ready > 0){
        ssize_t datagram_len = recvfrom (sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
        if (datagram_len < 0) {
            fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
            return 0;
        }

        char sender_ip_str[20]; 
        inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
        //printf ("Received UDP packet from IP address: %s, port: %d\n", sender_ip_str, ntohs(sender.sin_port));

        buffer[datagram_len] = 0;
        //printf ("%ld-byte message: +%s+\n", datagram_len, buffer);
        
        update_vector(buffer, sender_ip_str);
        
        return 1;
    }
    return 1;
}

long long get_time(struct timeval prev_time){
    struct timeval curtime, tval_result; 
    gettimeofday(&curtime, NULL);
    timersub(&curtime, &prev_time, &tval_result);
    return tval_result.tv_sec;
    
}

int main()
{
    get_input();
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "socket error: %s\n", strerror(errno)); 
        return EXIT_FAILURE;
    }
    
    struct sockaddr_in server_address;
    bzero (&server_address, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(54321);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind (sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "bind error: %s\n", strerror(errno)); 
        return EXIT_FAILURE;
    }
   
    const int NUM_SECONDS = 10;//dlugosc rundy
    struct timeval prev_time;
    gettimeofday(&prev_time, NULL);
    for (;;) {
        
        long long elapsed = get_time(prev_time);

        if(elapsed > NUM_SECONDS)
        {
            gettimeofday(&prev_time, NULL);
            print_vector();
            remove_bad();
            //write_vector(sockfd, server_address);
            cur_round++;
        }
        int err = receive_and_parse(sockfd);
        if(err == 0){
            return EXIT_FAILURE;            
        }
        fflush(stdout);
    }

    close (sockfd);
    return EXIT_SUCCESS;
}

