#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<iostream>
#include<string>
#include<cstring>
#include<map>
#include"./helper.hpp"
#define BUFFERSIZE 4096
#define REQUEST 0
#define RESPONSE 1
using namespace std;

class Request {
    public:
        string request_message;
        string method;
        string path;
        string version;
        map<string, string> header;
        Request();
        ~Request();
        void parse(const char*);
        void get_headers(string&);
        bool valid_protocol(const string);
};

#endif