// Julia Majkowska 290363
#include"server_ops.h"
using namespace std;
int is_file(const char* name)
{
    DIR* directory = opendir(name);
    if(directory != NULL)
    {
     closedir(directory);
     return 0;
    }
    if(errno == ENOTDIR)
    {
     return 1;
    }

    return -1;
}

pair<char*, int>  make_response(string katalog, string nazwa, string domena, string typ, int port){
    string fn = katalog + domena + nazwa;
    if(is_file(fn.c_str()) == 0)    return moved_response(nazwa,domena,  port);
    char * filename = (char*)malloc(10000);
    filename = strcpy(filename, fn.c_str());
    char reply_msg[1000000];
    FILE* f = fopen(filename, "rb");
    if( f == NULL) return not_found_response();
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);
    int printed = sprintf (reply_msg, "HTTP/1.1 200 OK\n\
                        Content-Length: %ld\n\
                        Content-Type: %s\r\n\r\n", fsize+1, typ.c_str());
    fread(reply_msg + printed, fsize, 1, f);
    fclose(f);
    free(filename);
    return make_pair(reply_msg, printed + fsize +1 );
}

string get_word(char* buff){
    string slowo = "";
    int i = 0;
    while(*buff != 0 && *buff !='\n' && *buff != ' ' && i<1000 && *buff != ':'){
        slowo += *buff;
        i++;
        buff++;       
    }
    return slowo;
}

bool prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

string get_typ(string slowo){
    const char * buff = slowo.c_str();
    while(*buff != '.') buff++;
    buff++;
    for(int i = 0; i< 7; i++) 
        if(strcmp(typy[i].c_str(), buff) == 0) return naglowki_typow[i];
    
    return "application/octet-stream";
}