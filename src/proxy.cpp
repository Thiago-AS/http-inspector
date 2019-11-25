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
        create_http_socket(new_request->header["Host"]);
        send_http_request(new_request->request_message);
    } catch (const Error& e) {
        throw;
    } 
    clear_buffer();
}

void Proxy::create_http_socket(const string addr){
    if ((this->http_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        throw Error("Socket creation failed"); 

    struct hostent *server = gethostbyname(addr.c_str());
    if (server == NULL)
        throw Error("Could not find host address"); 

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);

    if (connect(this->http_sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        throw Error("Could not connect with the following socket");
}

void Proxy::send_http_request(const string msg){
    if ((send(this->http_sockfd, (void*) msg.c_str(), msg.size(), 0)) < 0)
        throw Error("Could send message to remote server");
}

void Proxy::clear_buffer() {
    for(int i=0; i<BUFFERSIZE; i++) {
        this->buffer[i] = 0;
    }
}