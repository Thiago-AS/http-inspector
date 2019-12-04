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
        }
        close(this->connection);
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
        save_in_cache(new_request, REQUEST);
        create_http_socket(new_request->header["Host"]);
        send_http_request(new_request->request_message);
        //proxy_back();
    } catch (const Error& e) {
        throw;
    } 
}

void Proxy::create_http_socket(const string addr){
    string port = "80";
    struct addrinfo hints = {0}, *serv_addr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    cout << "[INFO] - Host: " + addr + " Port: " + port << endl;

    if (getaddrinfo(addr.c_str(), port.c_str(), &hints, &serv_addr) != 0)
        throw Error("Could not find host address"); 

    if ((this->http_sockfd = socket(serv_addr->ai_family, serv_addr->ai_socktype, serv_addr->ai_protocol)) == 0) 
        throw Error("Socket creation failed"); 

    if (connect(this->http_sockfd,serv_addr->ai_addr, serv_addr->ai_addrlen) < 0 )
        throw Error("Could not connect with the following socket");
}

void Proxy::send_http_request(const string msg){
    cout << msg << endl;
    if ((send(this->http_sockfd, (void*) msg.c_str(), msg.size(), 0)) < 0)
        throw Error("Could send message to remote server");
}

void Proxy::proxy_back() {
    clear_buffer();
    while(recv(this->http_sockfd, this->buffer, BUFFERSIZE, 0) != 0){

        clear_buffer();
    }
}

void Proxy::clear_buffer() {
    bzero(this->buffer, sizeof(this->buffer));
}

void Proxy::save_in_cache(Request *req, int type) {
    ofstream file;
    string file_name = req->path;
    replace(file_name.begin(), file_name.end(), '/', '|');
    if(type == REQUEST) {
        file.open("../cache/requests_" + file_name);
    } else {
        file.open("../cache/response_" + file_name);
    }
    file << this->buffer;
    file.close();
}