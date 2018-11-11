#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <bits/stdc++.h>

using namespace std;
const int max_dist = 16;
class network{
public: 
    int ip; 
    u_int8_t mask;
    
    network(int i, u_int8_t m) {
        ip = i; 
        mask = m;
    }
    
    bool operator < ( network n) const{
        return (make_pair(ip, mask)) < (make_pair(n.ip, n.mask));
    }
    
    bool in( int ip2){
        int wart = (ip2^ip); 
        if( mask == 32 ) 
            wart&=-1;
        else wart&= (1<<mask) -1;
        return wart == 0;
    }
    
    in_addr broadcast(){
        
        in_addr c; 
        if(mask == 32)  c.s_addr = ip&(-1);
        else c.s_addr = ip&((1<<(mask))-1);
        //printf("adresy %04x, %04x %04x \n", c.s_addr, ip,  (1<<mask-1) -1);
        return c;
    }
    
};

class route{
public : 
    int dist; 
    string via; //ip adres lub -1 jeśli bezpośrednio
    
    route(){
        dist = max_dist;
        via = "";
    }
    
    route(string via1){
       dist = max_dist;
       via =via1;
    }
    route(int d, string v){
        dist = d; 
        via = v;
    }
    route(int d, char* v){
        dist = d; 
        string str(v);
        via = str;
        
    }
    
};

