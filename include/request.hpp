#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<iostream>
#include<string>
#include<cstring>
#include"./helper.hpp"
#define BUFFERSIZE 4096
using namespace std;

class Headers {
};

class Request {
    public:
        string method;
        string protocol;
        string host;
        string port;
        string path;
        string version;
        string buf;
        Headers *header;
        Request();
        ~Request();
        bool parse(const char*);
};

#endif