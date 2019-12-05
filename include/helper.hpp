#ifndef HELPER_HPP
#define HELPER_HPP

#include<iostream>
#include<string>
#include<cstring>
#include <sstream>
#include<stdexcept>
using namespace std;

unsigned int get_port(int, const char**);

/**
 *  @class Error
 *  Defines a custom error handler to display [ERROR] messages
 */
class Error : public std::exception {
    private:
        /** Message to be displayed */
        string message;
    public:
        /** Constructor */
        explicit Error(const string&);
        /**
         *  Return the error messagem when called
         *  @returns the message to be displayed
         */
        const char *what() const throw();
};

#endif