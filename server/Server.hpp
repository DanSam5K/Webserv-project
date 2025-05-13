
#ifndef CLASS_SERVER_HPP
#define CLASS_SERVER_HPP

#include "../config/ConfigParser.hpp"
#include <iostream>
#include <exception>

class Server {
public:
    Server(ConfigParser* configHandler) {
        (void)configHandler;
    } // Constructor
    ~Server() {} // Destructor

    void run(); // Dummy declaration
};

#endif