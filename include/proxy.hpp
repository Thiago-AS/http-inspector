#ifndef PROXY_HPP
#define PROXY_HPP

#include<iostream>
#include<sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>
#include"./helper.hpp"
#include"./request.hpp"
#define BUFFERSIZE 4096
using namespace std;

class Proxy {
    private:
        unsigned int port;
        int sockfd;
        int connection;
        struct sockaddr_in address;
        char buffer[BUFFERSIZE];
        char *response;
    public:
        Proxy(unsigned int);
        ~Proxy();
        void create_server();
        void loop();
        void handle_request();
        int create_socket(const string, const string);
};

#endif