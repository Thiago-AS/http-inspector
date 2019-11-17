#ifndef HELPER_HPP
#define HELPER_HPP

#include<iostream>
#include<string>
#include<cstring>
#include <sstream>
#include<stdexcept>
using namespace std;

unsigned int get_port(int, const char**);

class Error : public std::exception {
    private:
        string message;
    public:
        explicit Error(const string&);
        const char *what() const throw();
};

#endif