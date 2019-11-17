#include "../include/request.hpp"

Request::Request(){
    this->header = nullptr;
}

Request::~Request(){
   delete this->header;
   this->header = nullptr;
}

bool Request::parse(const char* response) {
    string str_response(response);
    string full_url;
    size_t index = str_response.find("\r\n\r\n");
    if(index == string::npos){
        throw Error("Invalid request, no end of header");
    }
    stringstream stream(str_response);
    cout << str_response << endl;
    stream >> this->method; 
    stream >> full_url;
    stream >> this->version;
    cout << endl << this->method << full_url << this->version << endl;
    return true;
}