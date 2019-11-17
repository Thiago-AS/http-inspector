#ifndef PROXY_HPP
#define PROXY_HPP

#include<iostream>
using namespace std;

class Proxy {
    private:
        int port;
    public:
        Proxy(unsigned int);
        ~Proxy();
};

#endif