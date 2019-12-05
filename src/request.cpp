#include "../include/request.hpp"

Request::Request(){
}

Request::~Request(){
}

void Request::parse(const char* response) {
    string str_response(response);
    this->original_message = str_response;
    size_t pos;

    pos = str_response.find(' ');
    this->method = str_response.substr(0, pos);
    str_response.erase(0, pos+1); 

    pos = str_response.find(' ');
    this->path = str_response.substr(0, pos);
    str_response.erase(0, pos+1);

    pos = str_response.find("\r\n");
    this->version = str_response.substr(0, pos);
    str_response.erase(0, pos+2); 

    if(!valid_protocol(this->version))
        throw Error("Invalid request - protocol " + this->version + " not supported");

    if(!valid_method(this->method))
        throw Error("Invalid request - method not supported");
    
    get_headers(str_response);
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

bool Request::valid_method(const string method) {
    return (method == "GET");
}

string Request::build_request() {
    string end_point = this->path.substr(7, this->path.size());
    end_point = end_point.substr(end_point.find('/'), end_point.size());
    string request = this->method + " " + end_point + " " + this->version
    + "\r\nHost: " + this->header["Host"] + "\r\n\r\n\r\n";
    return request;
}
