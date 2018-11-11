#include "routing_ops.h"

map< network , route> routing;
vector<network> neighbours;
map<network, int> neigh_dist;
map<network, int> last_round_heard;

long long cur_round = 0;

void delete_indirect(network neigh){
    for(pair<network, route>  net : routing){
        network n = net.first;
        int kandydat = inet_addr(net.second.via.c_str());//jezeli pierwszy router na trasie jest w odciętej sieci
        if(neigh.in(kandydat)){
            routing[n] = route(routing[n].via);//ustaw inf
        }
    }
}


void remove_bad(){// usuwanie przeterminownych
    
    for(pair<network, route>  net : routing){
        network n = net.first;
        if(cur_round - last_round_heard[n]  > round_wait ){
            if((routing[n].via !="direct") && (routing[n].via !="unreachable"))
                routing[n] = route(routing[n].via);
            else{
                string unr = "unreachable";
                routing[n] = route(max_dist, unr.c_str());
            }
        }
    }
    
    for (network neigh : neighbours){
        if(routing[neigh].dist >= max_dist){
            routing[neigh].via = "unreachable"; 
        }
        if(routing[neigh].via == "unreachable")
            delete_indirect(neigh);
    }
    
    vector<network> expired;
    
    for(pair<network, route>  net : routing){
        network n = net.first;
        route r = net.second;

        if(cur_round - last_round_heard[n]  > round_broadcast){
            if(r.via != "unreachable" && r.via!="direct") {
                expired.push_back(n);
            }
        }
    }
    
    for(network n : expired){
        routing.erase(n);
    }
            
    
}

int find_and_update_sender(char * sender){
    int ip = inet_addr(sender);
    int res = max_dist;
    for(network neigh : neighbours)
        if(neigh.in(ip)){
            if(routing[neigh].dist >= neigh_dist[neigh]){
                last_round_heard[neigh] = cur_round;
                routing[neigh].via = "direct";
                routing[neigh].dist= neigh_dist[neigh];
                res = routing[neigh].dist;
            }
        }
    return res;
}


void update_vector( u_int8_t *buffer, char* sender){
    //write_buff((char*)buffer, 9);
    int addr = *((int*) buffer);
    addr = ntohl(addr);
    u_int8_t mask= *(buffer + 4);
    void* temp = buffer + 5;
    int d = *((int *)temp);
    d = ntohl(d);
    
    //printf("to %d mask %d dist %d via%s",addr, mask, d, sender);
    network dest(addr, mask);
    d+=find_and_update_sender(sender);
    if(d> max_dist) d = max_dist;
    route cur= routing[dest];
    if((cur. dist > d)||(cur.via == sender)){
        if(d < max_dist) 
            last_round_heard[dest] = cur_round;
        routing[dest] = route(d, sender);
    }
   
    
}

