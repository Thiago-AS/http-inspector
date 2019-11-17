#ifndef PROXY_HPP
#define PROXY_HPP

#include<iostream>
#include<sys/socket.h> 
#include"./helper.hpp"
#include <netinet/in.h> 
using namespace std;

class Proxy {
    private:
        unsigned int port;
        int sockfd;
        struct sockaddr_in address;
    public:
        Proxy(unsigned int);
        ~Proxy();
        void create_socket();
};

#endif