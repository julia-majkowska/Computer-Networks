// Julia Majkowska 290363
#include "transport_ops.h"

using namespace std;

int window_start= 0;//numer bajtu na poczatku okna
int window_buf_start = 0; // numer na ktorym bajcie bufora jest poczatek okna
FILE * plik; 
int len;

bool wpisany[window_len];
uint8_t window_content[window_len];

void write_to_file(){
    int indeks = window_buf_start;  
    while(wpisany[indeks]){
        int packet_len = min(1000, len - window_start);
        fwrite(&window_content[indeks] , sizeof(char), packet_len, plik);
        wpisany[indeks] = false;
        indeks+=packet_len;
        indeks%=window_len;
        window_start+=packet_len;
    }
    window_buf_start = indeks;
    
}

void write_to_buffer(uint8_t* bufor){
    int header_len = 1;
    bufor += 5;
    int start = 0;
    while(*bufor != ' ') {
        start*=10;
        start+= *bufor -'0';
        bufor++;
        header_len++;
    }
    bufor++;
    if( start < window_start || start >= window_start + window_len) return ;
    int data_len = 0;
    while(*bufor != '\n') {
        data_len*=10;
        data_len+= *bufor -'0';
        bufor++;
        header_len++;
    }
    bufor++;
    if(wpisany[window_buf_start + start - window_start]) return;
    wpisany[window_buf_start + start - window_start] = true;
    for(int i = 0; i<data_len; i++){
        int indeks = (window_buf_start + start - window_start + i)%window_len;
        window_content[indeks] = bufor[i];
    }
    if(start == window_start) write_to_file();
}

int request_window(int window_start, int len){
    
    for(int i = 0; i<window_len && i + window_start < len; i+=maks_len){
        int indeks = (window_buf_start + i)%window_len;
        if(!wpisany[indeks]){
            int ans = send_datagram(i+window_start, min(maks_len, len-window_start-i));
            if (ans < 0) return -1;
        }
    }
    return 1;
}