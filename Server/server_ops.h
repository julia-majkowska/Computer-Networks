// Julia Majkowska 290363
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "sockwrap.h"
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/time.h>
#define BUFFSIZE 10000000	// 10 MB

using namespace std;
const string typy[] = {"txt", "html", "css", "jpg", "jpeg", "png", "pdf"};
const string naglowki_typow[] = {"text/txt", "text/html", "text/css", "image/jpeg", "image/jpeg", "image/png", "application/pdf"};

pair<char*, int> not_implemented_response();

pair<char*, int> forbidden_response();

pair<char*, int> moved_response(string nazwa, string domena, int port);

pair<char*, int> not_found_response();

int is_file(const char* name);
pair<char*, int>  make_response(string katalog, string nazwa, string domena, string typ, int port);
string get_word(char* buff);

bool prefix(const char *pre, const char *str);

string get_typ(string slowo);