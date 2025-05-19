#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <iostream>
#include <exception>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
// #include "ServerBlock.hpp"
// #include "utils.hpp"

// class ConfigParser {
// public:
//     ConfigParser() {} // Default constructor
//     ~ConfigParser() {} // Destructor

//     void validateConfig(const std::string& configFilePath); // Dummy declaration
//     void printConfig() const; // Dummy declaration
// };
class ConfigParser
{
    private:
        // Maximum size for client request bodies
        unsigned long _clientMaximumBodySize;

        // Listening port for the server
        std::string _listeningServerPort;

        // Default error pages mapped by HTTP status codes
        std::map<int, std::string> _defaultErrorPages;

        // Server blocks parsed from the configuration
        std::vector<ServerBlock*> _serverBlocksFromConfig;

    public:
        // Constructors and Destructor
        ConfigParser();
        ConfigParser(const ConfigParser& other);
        ~ConfigParser();

        // Assignment Operator
        ConfigParser& operator=(const ConfigParser& other);

        // Configuration Parsing Methods
        bool validateConfig(const std::string& configFilePath);
        bool validateDirectives(const std::string& fileName);
        void validateLocationBlock(std::ifstream& fileStream, const std::vector<std::string>& tokens, ServerBlock* serverBlock);
        void checkForDuplicateServerBlocks();
        void displayConfiguration() const;

        // Accessors
        std::vector<ServerBlock*>& getServerBlocks();
        unsigned long getClientMaximumBodySize() const;
        const std::string& getListeningServerPort() const;

        // Exception Classes
        class FileNotFoundException : public std::exception {
        public:
            const char* what() const noexcept override;
        };

        class InvalidDirectiveException : public std::exception {
        public:
            const char* what() const noexcept override;
        };

        class MissingBracketsException : public std::exception {
        public:
            const char* what() const noexcept override;
        };

        class DuplicateServerBlockException : public std::exception {
        public:
            const char* what() const noexcept override;
        };
};

#endif