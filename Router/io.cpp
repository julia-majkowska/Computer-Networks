#include "routing_ops.h"

extern map< network , route> routing;
extern vector<network> neighbours;
extern map<network, int> last_round_heard;
extern map<network, int> neigh_dist;

extern long long cur_round;

void read_ip(){
     scanf("\n");
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
     //printf("ip to %s a hexy %04x %" PRIu8"\n", c, ip, mask);
     network n(ip, mask);
     char napis[20];
     int dist;
     scanf("%s %d", napis, &dist);
     ///printf("napis %s dist %d\n", napis, dist);
     string dir = "direct";
     route r(dist, dir.c_str());
     neighbours.push_back(n); 
     neigh_dist[n] = dist;
     routing[n] = r;
}

void get_input(){
    int n;
    scanf("%d", &n);
    for(int i = 0; i< n; i++) read_ip();
    print_vector();
}

void print_vector(){
    printf("\n\n\n");
    for(auto a : routing){
        struct in_addr adres; 
        adres.s_addr = a.first.ip;
        char ipstr[20];
        inet_ntop(AF_INET, &adres, ipstr, sizeof(ipstr));
        if(a.second.dist < max_dist){
            printf("%s/%" PRIu8" distance %d ", ipstr, a.first.mask, a.second.dist);
            if(a.second.via == "direct")
                printf("connected directly\n");
            else
                printf("via %s\n", a.second.via.c_str());
        }
        else if(a.second.via == "unreachable"){
            printf("%s/%" PRIu8" unreachable connected directly\n", ipstr, a.first.mask);
            
        }
    }
}