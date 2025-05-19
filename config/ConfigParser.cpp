#include "ConfigParser.hpp"

// void ConfigParser::validateConfig(const std::string& configFilePath) {
//     // Dummy implementation
//     std::cout << "Validating config file: " << configFilePath << std::endl;
// }

// void ConfigParser::printConfig() const {
//     // Dummy implementation
//     std::cout << "Printing configuration..." << std::endl;
// }

ConfigParser::ConfigParser()
    : _clientMaximumBodySize(0), _listeningServerPort("8080") {
    // Constructor implementation
}

ConfigParser::ConfigParser(const ConfigParser& other)
    : _clientMaximumBodySize(other._clientMaximumBodySize),
      _listeningServerPort(other._listeningServerPort),
      _defaultErrorPages(other._defaultErrorPages),
      _serverBlocksFromConfig(other._serverBlocksFromConfig) {
    // Copy constructor implementation
}

ConfigParser::~ConfigParser() {
    // Destructor implementation
    for (size_t i = 0; i < _serverBlocksFromConfig.size(); ++i) {
        delete _serverBlocksFromConfig[i];
    }
    _serverBlocksFromConfig.clear();
}

ConfigParser& ConfigParser::operator=(const ConfigParser& other) {
    if (this != &other) {
        _clientMaximumBodySize = other._clientMaximumBodySize;
        _listeningServerPort = other._listeningServerPort;
        _defaultErrorPages = other._defaultErrorPages;
        _serverBlocksFromConfig = other._serverBlocksFromConfig;
    }
    return *this;
}

std::string trim(const std::string& str) {
    const std::string whitespace = " \t\r\n";
    const auto start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    const auto end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

// Utility function to split a string into tokens based on whitespace
std::vector<std::string> tokenize(const std::string& line) {
    std::istringstream stream(line);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to check if braces are balanced in the configuration file
bool areBracesBalanced(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return false;
    }

    std::stack<char> braceStack;
    std::string line;
    while (std::getline(file, line)) {
        for (char ch : line) {
            if (ch == '{') {
                braceStack.push(ch);
            } else if (ch == '}') {
                if (braceStack.empty()) {
                    return false;
                }
                braceStack.pop();
            }
        }
    }
    return braceStack.empty();
}

// Function to parse a location block
void ConfigParser::validateLocationBlock(std::ifstream& file, const std::vector<std::string>& initialTokens, ServerBlock* serverBlock) {
    if (initialTokens.size() < 2) {
        throw std::runtime_error("Invalid location directive");
    }

    auto locationBlock = new LocationBlock();
    locationBlock->setURL(initialTokens[1]);
    serverBlock->addLocationBlock(locationBlock);

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        if (line == "}") break;

        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        const std::string& directive = tokens[0];
        if (directive == "index") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for index");
            locationBlock->setIndexPage(tokens[1]);
        } else if (directive == "root") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for root");
            locationBlock->setRootFolder(tokens[1]);
        } else if (directive == "limit_except") {
            locationBlock->setLimitExcept(tokens);
        } else if (directive == "client_max_body_size") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for client_max_body_size");
            locationBlock->setClientMaxBodySize(std::stoul(tokens[1]));
        } else {
            throw std::runtime_error("Unknown directive in location block: " + directive);
        }
    }
}

// Function to parse a server block
void ConfigParser::validateServerBlock(std::ifstream& file) {
    auto serverBlock = new ServerBlock();
    _serverBlocks.push_back(serverBlock);

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        if (line == "}") break;

        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        const std::string& directive = tokens[0];
        if (directive == "listen") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for listen");
            serverBlock->setListeningPort(tokens[1]);
        } else if (directive == "server_name") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for server_name");
            serverBlock->setServerName(tokens[1]);
        } else if (directive == "index") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for index");
            serverBlock->setIndexPage(tokens[1]);
        } else if (directive == "root") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for root");
            serverBlock->setRootFolder(tokens[1]);
        } else if (directive == "error_pages") {
            if (tokens.size() < 3) throw std::runtime_error("Missing values for error_pages");
            serverBlock->setErrorPage(tokens[1], tokens[2]);
        } else if (directive == "limit_except") {
            serverBlock->setLimitExcept(tokens);
        } else if (directive == "client_max_body_size") {
            if (tokens.size() < 2) throw std::runtime_error("Missing value for client_max_body_size");
            serverBlock->setClientMaxBodySize(std::stoul(tokens[1]));
        } else if (directive == "location") {
            parseLocationBlock(file, tokens, serverBlock);
        } else {
            throw std::runtime_error("Unknown directive in server block: " + directive);
        }
    }
}

// Function to validate directives in the configuration file
void ConfigParser::validateDirectives(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        const std::string& directive = tokens[0];
        if (directive == "server") {
            parseServerBlock(file);
        } else if (directive == "http") {
            // Skip http block for now
            continue;
        } else {
            throw std::runtime_error("Unknown top-level directive: " + directive);
        }
    }
}

// Function to check for duplicate server names and listening ports
void ConfigParser::checkForDuplicates() {
    std::set<std::string> serverNames;
    std::set<std::string> listeningPorts;

    for (const auto& serverBlock : _serverBlocks) {
        const std::string& name = serverBlock->getServerName();
        const std::string& port = serverBlock->getListeningPort();

        if (!serverNames.insert(name).second) {
            throw std::runtime_error("Duplicate server name: " + name);
        }
        if (!listeningPorts.insert(port).second) {
            throw std::runtime_error("Duplicate listening port: " + port);
        }
    }
}

// Function to parse and validate the entire configuration file
void Config::parseConfigFile(const std::string& filename) {
    if (!areBracesBalanced(filename)) {
        throw std::runtime_error("Unbalanced braces in config file");
    }
    validateDirectives(filename);
    checkForDuplicates();
}


// Getters
unsigned long Config::getClientMaximumBodySize() const {
    return _clientMaxBodySize;
}

const std::string& Config::getListeningServerPort() const {
    return _listeningServerPort;
}

const std::vector<ServerBlock*>& Config::getServerBlocks() const {
    return _serverBlocks;
}

