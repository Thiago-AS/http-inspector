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

/**
 *  @class Request
 *  Defines the request entitie
 */
class Request {
    public:
        /** Original message */
        string original_message;
        /** Request method */
        string method;
        /** Url path */
        string path;
        /** Protocol Version */
        string version;
        /** Url endpoint */
        string end_point;
        /** Set of headers */
        map<string, string> header;
        /** Constructor */
        Request();
        ~Request();
        /**
         *  Parse the message into the given class fields
         *  @param Message
         */
        void parse(const char*);
        /**
         *  Parse headers into the given class map header
         *  @param Message
         */
        void get_headers(string&);
        /**
         *  Check if protocol is valid
         *  @param version
         *  @return 1 if valid, 0 otherwise
         */
        bool valid_protocol(const string);
        /**
         *  Build final request
         *  @return request to be sent to remote server
         */
        string build_request();
        /**
         *  Check if method is valid
         *  @param method
         *  @return 1 if valid, 0 otherwise
         */
        bool valid_method(const string);
};

#endif