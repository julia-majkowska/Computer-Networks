// Julia Majkowska 290363
#include "server_ops.h"
using namespace std;
pair<char*, int> not_implemented_response(){
    char reply_msg[10000];
    int printed = sprintf (reply_msg, "HTTP/1.1 501 Not Implemented\r\n\r\n\
                                        <!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n\
                                        <html><head>\n\
                                        <title>501 Method Not Implemented</title>\n\
                                        </head><body>\n\
                                        <h1>Method Not Implemented</h1>\n\
                                        </body></html>\n");
    return make_pair(reply_msg, printed);
    
}

pair<char*, int> forbidden_response(){
    char reply_msg[10000];
    int printed = sprintf (reply_msg, "HTTP/1.1 403 Forbidden\r\n\r\n\
                                        <!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n\
                                        <html><head>\n\
                                        <title>403 Forbidden</title>\n\
                                        </head><body>\n\
                                        <h1>403 Forbidden</h1>\n\
                                        <p> The following address does not belong do this domain\n\
                                        </p>\n\
                                        </body></html>\n");
    return make_pair(reply_msg, printed);
    
}

pair<char*, int> moved_response(string nazwa, string domena, int port){//nie skonczone
    char reply_msg[10000];
    nazwa +="index.html";
    int printed = sprintf (reply_msg, "HTTP/1.1 301 Moved Permanently\nLocation: http://%s:%d%s\r\n\r\n"\
                                        /*<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n\
                                        <html><head>\n\
                                        <title>301 Moved Permanently</title>\n\
                                        </head><body>\n\
                                        <h1>301 Moved Permanently</h1>\n\
                                        <p> Did you mean to go to index?\n\
                                        </p>\n\
                                        </body></html>\n"*/, domena.c_str(), port, nazwa.c_str());
    //printf("%s\n", reply_msg);
    return make_pair(reply_msg, printed);
    
}

pair<char*, int> not_found_response(){
    char reply_msg[10000];
    int printed = sprintf (reply_msg, "HTTP/1.1 404 Not Found:\r\n\r\n\
                                        <!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n\
                                        <html><head>\n\
                                        <title>404 Not Found:</title>\n\
                                        </head><body>\n\
                                        <h1>404 Not Found:</h1>\n\
                                        <p> File not found\n\
                                        </p>\n\
                                        </body></html>\n");
    return make_pair(reply_msg, printed);
    
}
