#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<iostream>
#include<string>
#include<cstring>
#include<map>
#include"./helper.hpp"
#define BUFFERSIZE 4096
using namespace std;

class Request {
    public:
        string method;
        string protocol;
        string host;
        string port;
        string path;
        string version;
        string buf;
        map<string, string> header;
        Request();
        ~Request();
        void parse(const char*);
        void get_headers(string&);
        bool valid_protocol(const string);
};

#endif