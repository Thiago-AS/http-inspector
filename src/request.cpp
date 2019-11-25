#include "../include/request.hpp"

Request::Request(){
}

Request::~Request(){
}

void Request::parse(const char* response) {
    string str_response(response);
    this->request_message = str_response;
    size_t index = str_response.find("\r\n\r\n"), pos;
    if(index == string::npos)
        throw Error("Invalid request - no end of header");

    pos = str_response.find(' ');
    this->method = str_response.substr(0, pos);
    str_response.erase(0, pos+1); 

    pos = str_response.find(' ');
    this->path = str_response.substr(0, pos);
    str_response.erase(0, pos+1);

    pos = str_response.find('\n');
    this->version = str_response.substr(0, pos);
    str_response.erase(0, pos+1); 

    if(!valid_protocol(this->version))
        throw Error("Invalid request - protocol not supported");
    
    get_headers(str_response);
    cout << this->request_message << endl;
}

void Request::get_headers(string& response) {
    string key, value, delimiter = ": ";
    size_t pos, end_line;
    while ((pos = response.find(delimiter)) != string::npos) {
        key = response.substr(0, pos);
        response.erase(0, pos+delimiter.size()); 
        end_line = response.find("\r\n");
        value = response.substr(0, end_line);
        header[key] = value;
    }
}

bool Request::valid_protocol(const string version) {
    string protocol = version.substr(0, version.find('/'));
    return (protocol == "HTTP") ? true : false;
}
