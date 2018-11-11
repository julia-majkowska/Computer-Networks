// Julia Majkowska 290363
#include <bits/stdc++.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>   // std::cout
#include <string>
#include <sys/time.h>
#include <inttypes.h>
#include <time.h>

const int window_len = 60000;
const int maks_len = 1000;


void write_to_file();

void write_to_buffer(uint8_t* bufor);

int request_window(int window_start, int len);

int send_datagram(int start, int len);