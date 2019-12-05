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

/**
 *  @class Proxy
 *  Defines everything needed to run the proxy server
 */
class Proxy {
    private:
        /** Port where the server socket will run */
        unsigned int port;
        /** Server socket id */
        int sockfd;
        /** Server socket connection */
        int connection;
        /** Server socket struct address */
        struct sockaddr_in address;
        /** Buffer to read and send data */
        char buffer[BUFFERSIZE];
        /** Client socket id */
        int http_sockfd;
        /** Cache file name */
        string file_name;
    public:
        /**
         *  Constructor
         *  @param port number
         */
        Proxy(unsigned int);
        ~Proxy();
        /**
         *  Creates server, initializing socket, setting options, binding to port,
         *  and start the listener process
         */
        void create_server();
        /**
         *  Main application loop
         */
        void loop();
        /**
         *  Handle initial client request
         */
        void handle_request();
        /**
         *  Creates http socket to connect with the remote server
         *  @param HostName
         */
        void create_http_socket(const string);
        /**
         *  Send client request to remote server
         *  @param Message to be sent
         *  @param File name to be stored
         */
        void send_http_request(const string, const string);
        /**
         *  Clears the buffer
         */
        void clear_buffer();
        /**
         *  Send the remote server response back to the client
         */
        void proxy_back();
        /**
         *  Save request or response in cache
         *  @param Type of message : REQUEST or RESPONSE
         */
        void save_in_cache(int);
        /**
         *  Print all content in buffer
         */
        void debug_buffer();
        /**
         *  Intercept client request and display options
         */
        void intercept_request();
        /**
         *  Intercept remote server response and display options
         *  @param Request that generated the response
         */
        void intercept_response(Request*);
        /**
         *  Edit data to be sent or received
         *  @param Type of message : REQUEST or RESPONSE
         */
        void edit(int);
        /**
         *  Reload the buffer when data is edited
         *  @param Type of message : REQUEST or RESPONSE
         */
        void reload_buffer(int);
};

#endif