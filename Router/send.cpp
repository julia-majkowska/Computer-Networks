
#include "structures.h"
using namespace std;

map< network , route> routing; 
vector<network> neighbours;

//map< network , int> last_round_send;

void write_buff(char* buf, int s) {
    for(int i = 0; i< s; i++) {
    
        printf("%x\n", buf[i]);
    }
    
}

int write_network(in_addr ip, network n, int dist){
    
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

	struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family      = AF_INET;
	server_address.sin_port        = htons(54321);
        printf("%s\n", inet_ntoa(ip));
        server_address.sin_addr = ip;
        //inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

        char message[9];
        ((int*) message)[0] = htonl(n.ip); 
        message[4] = n.mask; 
        char *temp = message + 5;
        *((int*) temp) = htonl(dist);
        write_buff(message, 9);
	ssize_t message_len = 9;
	if (sendto(sockfd, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len) {
		fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
		return 0;		
	}

	close (sockfd);
	return 1;
}

void write_vector(){
    
    for(pair<network, route>  net : routing){
        for (network neigh : neighbours){
                write_network(neigh.broadcast(), net.first, net.second.dist);        
        }
    }
}

void print_vector(){
    printf("Aktualna tablica routingu :\n");
    for(auto a : routing){
        struct in_addr adres; 
        adres.s_addr = a.first.ip;
        char ipstr[20];
        inet_ntop(AF_INET, &adres, ipstr, sizeof(ipstr));
        printf("%s/%" PRIu8" distance %d %s\n", ipstr, a.first.mask, a.second.dist, a.second.via.c_str());
    }
}

void read_ip(){
     u_int8_t mask;
     char c[20];
     char* znak = c;
     *znak = getchar();
     while(*znak!= '/'){
         znak+=1;
         *znak = getchar();
     }
     *znak = 0;
     int ip = inet_addr(c);
     scanf("%" SCNu8, &mask);
     printf("ip to %s a hexy %04x %" PRIu8"\n", c, ip, mask);
     network n(ip, mask);
     char napis[20];
     int dist;
     scanf("%s %d\n", napis, &dist);
     printf("napis %s dist %d\n", napis, dist);
     route r(dist, "direct");
     neighbours.push_back(n);   
     routing[n] = r;
     /*char z;
     z = getchar();
     while(z!= '\n'){
         z = getchar();
     }*/
}

void get_input(){
    int n;
    scanf("%d\n", &n);
    for(int i = 0; i< n; i++) read_ip();
    print_vector();
}

int main()
{
    //read_ip();
    get_input();
    double time_counter = 0;

    clock_t this_time = clock();
    clock_t last_time = this_time;
    const int NUM_SECONDS = 5;
    printf("Gran = %ld\n", NUM_SECONDS * CLOCKS_PER_SEC);

    while(true)
    {
        this_time = clock();

        time_counter += (double)(this_time - last_time);

        last_time = this_time;

        if(time_counter > (double)(NUM_SECONDS * CLOCKS_PER_SEC))
        {
            time_counter -= (double)(NUM_SECONDS * CLOCKS_PER_SEC);
            write_vector();
        }

        //printf("DebugTime = %f\n", time_counter);
    }

    return 0;
    
    
}
	
