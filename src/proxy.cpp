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
        
        cout << "[PROXY INFO] - Connection established" << endl;

        try {
            handle_request();
        } catch (const Error& e) {
            cout << e.what() << endl;
        }
        close(this->connection);
    } 
}

void Proxy::handle_request() {
    int byte_recieved = recv(this->connection, this->buffer, 
                                sizeof(this->buffer), 0);
    if(byte_recieved < 0)
        throw Error("Error reading the request");

    this->intercept_request();
}

void Proxy::intercept_request() {
    int choice;
    cout << "[PROXY INFO] - Choose what do you want to do with the request:" << endl;
    cout << "1 - Send" << endl;
    cout << "2 - Edit" << endl;
    cout << "3 - Block" << endl;
    cout << "-> ";
    cin >> choice;
    if(choice == 1 || choice == 2) {
        Request *new_request = new Request();
        if(new_request == nullptr) 
            throw Error("Memory allocation error");

        try {
            debug_buffer();
            save_in_cache(REQUEST);
            if(choice == 2) {
                this->edit(REQUEST);
                this->reload_buffer(REQUEST);
            }
            debug_buffer();
            new_request->parse(this->buffer);
            create_http_socket(new_request->header["Host"]);
            send_http_request(new_request->build_request());
            this->intercept_response();
            proxy_back();
            delete new_request;
            clear_buffer();
        } catch (const Error& e) {
            delete new_request;
            clear_buffer();
            throw;
        } 
    }else if(choice == 3){
        cout << "Request Blocked" << endl;
    }else{
        cout << "Option not avaible" << endl;
    }
}

void Proxy::intercept_response() {
    int choice, s_choice;
    cout << "[PROXY INFO] - Choose what do you want to do with the response:" << endl;
    cout << "1 - Send" << endl;
    cout << "2 - Edit" << endl;
    cout << "3 - Spider" << endl;
    cout << "3 - Dump" << endl;
    cout << "-> ";
    cin >> choice;
    if(choice == 1 || choice == 2) {
        try {
            if(choice == 2) this->edit(RESPONSE);
            proxy_back();
        } catch (const Error& e) {
            throw;
        } 
    }else if(choice == 3){
        cout << "[SPIDER INFO] - Input tree size: " << endl;
        cin >> s_choice;
        cout << "[SPIDER INFO] - Running spider" << endl;
    }else{
        cout << "Option not avaible" << endl;
    }
}

void Proxy::edit(int type) {
    ifstream file;
    string cmd;
    if(type == REQUEST)
        cmd = "vim ../cache/request_" + this->file_name;
    else 
        cmd = "vim ../cache/response_" + this->file_name;

    cout << cmd << endl;
    system(cmd.c_str());
}

void Proxy::create_http_socket(const string addr){
    string port = "80";
    struct addrinfo hints = {0}, *serv_addr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    struct timeval tv;
    tv.tv_sec = 1;

    cout << "[INFO] - Host: " + addr + " Port: " + port << endl;

    if (getaddrinfo(addr.c_str(), port.c_str(), &hints, &serv_addr) != 0)
        throw Error("Could not find host address"); 

    if ((this->http_sockfd = socket(serv_addr->ai_family, serv_addr->ai_socktype, serv_addr->ai_protocol)) == 0) 
        throw Error("Socket creation failed"); 

    if (setsockopt(this->http_sockfd, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval)) == -1)
        throw Error("Failed to set socket options");

    if (connect(this->http_sockfd,serv_addr->ai_addr, serv_addr->ai_addrlen) < 0 )
        throw Error("Could not connect with the following socket");
}

void Proxy::send_http_request(const string msg){
    if (send(this->http_sockfd, msg.c_str(), msg.size(), 0) <= 0)
        throw Error("Could send message to remote server");
    
    cout << "[PROXY INFO] - Request sent to original path" << endl;
    cout << msg << endl;

    ssize_t bytes;
    ofstream file;
    file.open("../cache/response_" + this->file_name);
    clear_buffer();
    cout << "[PROXY INFO] - Start of response" << endl;
    while(( bytes = recv(this->http_sockfd, this->buffer, BUFFERSIZE, 0)) > 0){
        cout << "\t" << bytes << " bytes received" << endl;
        file << this->buffer;
        clear_buffer();
    }
    cout << endl << "[PROXY INFO] - End of response" << endl;
    file.close();
}

void Proxy::proxy_back() {
    ifstream file;
    file.open("../cache/response_" + this->file_name);
    string proxy_back_msg((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    if (send(this->connection, proxy_back_msg.c_str(), proxy_back_msg.size(), 0) <= 0)
        throw Error("Could send message to remote server");
}

void Proxy::clear_buffer() {
    bzero(this->buffer, sizeof(this->buffer));
}

void Proxy::save_in_cache(int type) {
    ofstream file;

    string path;
    string str_response(this->buffer);
    str_response.erase(0, str_response.find(' ')+1); 
    path = str_response.substr(0, str_response.find(' '));
    this->file_name = path;

    replace(this->file_name.begin(), this->file_name.end(), '/', '_');
    if(type == REQUEST) {
        file.open("../cache/request_" + this->file_name);
    } else {
        file.open("../cache/response_" + this->file_name);
    }
    file << this->buffer;
    file.close();
}

void Proxy::debug_buffer() {
    cout << "[DEBUG] - Display buffer" << endl;
    for(int i=0; i<BUFFERSIZE; i++) {
        cout << this->buffer[i];
    }
    cout << "[DEBUG] - End display buffer" << endl;
}

void Proxy::reload_buffer(int type) {
    clear_buffer();
    ifstream file;
    if(type == REQUEST) {
        file.open("../cache/request_" + this->file_name);
    } else {
        file.open("../cache/response_" + this->file_name);
    }
    string new_buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    strcpy(this->buffer, new_buffer.c_str());
    file.close();

}
