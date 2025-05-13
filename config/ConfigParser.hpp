#ifndef CLASS_CONFIGPARSER_HPP
#define CLASS_CONFIGPARSER_HPP

#include <string>
#include <iostream>
#include <exception>

class ConfigParser {
public:
    ConfigParser() {} // Default constructor
    ~ConfigParser() {} // Destructor

    void validateConfig(const std::string& configFilePath); // Dummy declaration
    void printConfig() const; // Dummy declaration
};

#endif