#include "../include/helper.hpp"

unsigned int get_port(int argc, const char **argv) {
    unsigned int response = 8228;
    for(int i=0; i<argc; i++) {
        if(!strcmp(argv[i],"-p") && i+1 < argc) {
            stringstream stream(argv[i+1]);
            stream >> response; 
            break;
        }
    }

    return response;
}

Error::Error(const string& msg) {
    this->message = "[ERROR] - " + msg;
}

const char *Error::what() const throw()
{   
    return this->message.c_str();
}