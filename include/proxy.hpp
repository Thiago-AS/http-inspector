#ifndef PROXY_HPP
#define PROXY_HPP

#include <iostream>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "./request.hpp"

#define BUFFERSIZE 4096
using namespace std;

class Proxy {
    private:
        unsigned int port;
        int sockfd;
        int connection;
        struct sockaddr_in address;
        char buffer[BUFFERSIZE];
        int http_sockfd;
        string file_name;
    public:
        Proxy(unsigned int);
        ~Proxy();
        void create_server();
        void loop();
        void handle_request();
        void create_http_socket(const string);
        void send_http_request(const string, const string);
        void clear_buffer();
        void proxy_back();
        void save_in_cache(int);
        void debug_buffer();
        void intercept_request();
        void intercept_response(Request*);
        void edit(int);
        void reload_buffer(int);
};

#endif