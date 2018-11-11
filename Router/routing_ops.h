#include "structures.h"

const int round_wait = 3;
const int round_broadcast = 6;

void delete_indirect(network neigh);

void remove_bad();// usuwanie przeterminownych


int find_and_update_sender(char * sender);


void update_vector( u_int8_t *buffer, char* sender);

void print_vector();

void read_ip();

void get_input();