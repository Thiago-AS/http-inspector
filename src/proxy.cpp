#include "../include/proxy.hpp"

Proxy::Proxy(unsigned int port) {
    this->port = port;
    this->address.sin_family = AF_INET; 
	this->address.sin_addr.s_addr = INADDR_ANY; 
	this->address.sin_port = htons( this->port ); 
}

void Proxy::create_socket() {
    int opt_val = 1;
    
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        throw Error("Socket creation failed");
    } 

    if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
        &opt_val, sizeof(int)) == -1)
    {
        throw Error("Failed to set socket options");
    }

    if (bind(this->sockfd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        throw Error("Could not bind the socket to the given address");
    }

    if (listen(this->sockfd, SOMAXCONN) < 0)
    {
        throw Error("Could not listen to the given socket");
    }
}