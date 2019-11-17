#ifndef PROXY_HPP
#define PROXY_HPP

#include<iostream>
#include<sys/socket.h> 
#include"./helper.hpp"
#include <netinet/in.h> 
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
        void create_socket();
        void loop();
        void handle_request();
};

#endif