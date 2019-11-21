#include "../include/proxy.hpp"

Proxy::Proxy(unsigned int port) {
    this->port = port;
    this->address.sin_family = AF_INET; 
	this->address.sin_addr.s_addr = INADDR_ANY; 
	this->address.sin_port = htons( this->port ); 
}

Proxy::~Proxy() {

}

void Proxy::create_server() {
    int opt_val = 1;
    
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        throw Error("Socket creation failed"); 

    if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
        &opt_val, sizeof(int)) == -1)
        throw Error("Failed to set socket options");

    if (bind(this->sockfd, (struct sockaddr *)&this->address, 
        sizeof(this->address)) < 0)
        throw Error("Could not bind the socket to the given address");

    if (listen(this->sockfd, SOMAXCONN) < 0)
        throw Error("Could not listen to the given socket");
}

void Proxy::loop() {
    while(true) {
        this->connection = accept(this->sockfd, 
                                    (struct sockaddr *)NULL, NULL);
        
        if(this->connection < 0)
            throw Error("Could not accept the connection");
        
        cout << "[INFO] - Connection established" << endl;

        try {
            handle_request();
        } catch (const Error& e) {
            cout << e.what() << endl;
            close(this->connection);
        }
    } 
}

void Proxy::handle_request() {
    this->response = (char *) malloc(BUFFERSIZE);
    if(this->response == nullptr) 
        throw Error("Memory allocation error");
    
    int byte_recieved = recv(this->connection, this->buffer, 
                                sizeof(this->buffer), 0);
    if(byte_recieved < 0)
        throw Error("Error reading the request");

    strcat(response, buffer);
    Request *new_request = new Request();
    if(new_request == nullptr) 
        throw Error("Memory allocation error");
    
    try {
        new_request->parse(response);
    } catch (const Error& e) {
        throw;
    } 
}

int Proxy::create_socket(const string addr, const string port){
    struct addrinfo *servinfo;

    int socket;
    if ((socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
        throw Error("Socket creation failed");

    //Ponteiro de arquivo para o servidor conectado
    if(connect(socket, servinfo->ai_addr, servinfo->ai_addrlen) < 0){
        cout << "Error in connecting to server!\n" << endl;
    }

    freeaddrinfo(servinfo);
    return socket;
}