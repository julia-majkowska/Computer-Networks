// Julia Majkowska 290363
#include "server_ops.h"

using namespace std;

char buffer[BUFFSIZE+1];
int port;
string katalog;

int parse_package(char* buff, int conn_sockfd){
    //printf("%s\n", buff);
    string nazwa; 
    string domena;
    string typ_polaczenia;
    int info = 0;
    while(*buff != 0){
        
        if(prefix("GET", buff)){
            buff+=4;
            nazwa = get_word(buff);
            info++;
        }
        else if(prefix("Host", buff)){
            buff+=6;
            domena = get_word(buff);
            info++;
        }
        else if(prefix("Connection", buff)){
            buff+=12;
            typ_polaczenia = get_word(buff);
            info++;
        }
        
        while(*buff!= '\n' && *buff !=0) buff++;
        buff++;
        
    }
    if( info < 3){
        pair<char*, int>  response = not_implemented_response();
        Send (conn_sockfd, response.first, response.second, 0);
        return 0;
        
    }
    printf("Zapytanie : %s%s%s  - polaczenie typu %s\n", katalog.c_str(),  domena.c_str(), nazwa.c_str(), typ_polaczenia.c_str());
    string typ = get_typ(nazwa);
    pair<char*, int>  response = make_response(katalog, nazwa, domena, typ, port);
    Send (conn_sockfd, response.first, response.second, 0);
    if(typ_polaczenia =="close") return 2;
    return 1;
}

long long get_time(struct timeval prev_time){
    struct timeval curtime, tval_result; 
    gettimeofday(&curtime, NULL);
    timersub(&curtime, &prev_time, &tval_result);
    return tval_result.tv_sec;
    
}


int main(int argc, char* argv[])
{
        char * kat;
        if( argc < 3){
            printf("ZA MALO PARAMETROW\n");
            return EXIT_FAILURE;
        }
        
        port = strtol(argv[1], NULL, 10);
        if(port == 0) {
            printf("BLEDNY NUMER PORTU\n");
            return EXIT_FAILURE;
        }
        kat = argv[2];
        if(is_file(kat) != 0){
            printf("NIE MA TAKIEGO KATALOGU%s\n", kat);
            return EXIT_FAILURE;
        }
        katalog = string(kat);
        katalog+='/';
	// TCP: STREAM zamiast DGRAM 
	int sockfd = Socket(AF_INET, SOCK_STREAM, 0);	

	struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family      = AF_INET;
	server_address.sin_port        = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        Bind (sockfd, &server_address, sizeof(server_address));

	// TCP: rozmiar kolejki na trzymanie zadan polaczenia 
	Listen (sockfd, 64);
        const int NUM_SECONDS = 1;//dlugosc rundy
        struct timeval prev_time;
        gettimeofday(&prev_time, NULL);
        bool active = 0;
        int conn_sockfd;
	while (1) {    

		if(!active) {
                    conn_sockfd = Accept (sockfd, NULL, NULL);
                    active = 1;
                }

		int n = Recv(conn_sockfd, buffer, BUFFSIZE, 0);
                if( n != 0){
                    gettimeofday(&prev_time, NULL);
                    int msglen = n;
                    buffer[msglen] = 0;
                    int t = parse_package(buffer, conn_sockfd);
                    if(t == 2){
                        active = 0;
                        Close (conn_sockfd);
                    }
                }
                else {
                    long long elapsed = get_time(prev_time);

                    if(elapsed > NUM_SECONDS )
                    {
                        active = 0;
                        Close (conn_sockfd);
                    }
                }
                
	}
}
