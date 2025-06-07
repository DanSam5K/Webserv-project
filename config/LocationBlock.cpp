// #include "LocationBlock.hpp"

// LocationBlock::LocationBlock() {
//     _GET_allowed = false;
//     _POST_allowed = false;
//     _DELETE_allowed = false;
//     _client_max_body_size = 100000;
//     _root_folder = "/";
//     _index_page = "index.html";
// }

// LocationBlock::LocationBlock(const LocationBlock& other) {
//     (void)other;
// }

// LocationBlock::~LocationBlock() {
// 	// define some cleaning if needed
// }

// LocationBlock&	LocationBlock::operator=(const LocationBlock& other) {
//     (void)other;
// 	return *this;
// }

// // Getters
// std::string		LocationBlock::getURL(void) {
//     return (_url);
// }

// unsigned long	LocationBlock::getClientMaxBodySize(void) {
//     return (_client_max_body_size);
// }

// std::string		LocationBlock::getRootFolder(void) {
//     return (_root_folder);
// }

// std::string		LocationBlock::getIndexPage(void) {
//     return (_index_page);
// }

// bool		    LocationBlock::getLimitExceptGET(void) {
//     return _GET_allowed;
// }

// bool		    LocationBlock::getLimitExceptPOST(void) {
//     return _POST_allowed;
// }

// bool    		LocationBlock::getLimitExceptDELETE(void) {
//     return _DELETE_allowed;
// }

// // Setters
// void                LocationBlock::setURL(std::string URL) {
//     _url = URL;
// }

// void	            LocationBlock::setClientMaxBodySize(unsigned long maxBodySize) {
//     _client_max_body_size = maxBodySize;
// }

// void    		    LocationBlock::setIndexPage(std::string indexPage) {
//     _index_page = indexPage;
// }
// void    		    LocationBlock::setRootFolder(std::string rootFolder) {
//     _root_folder = rootFolder;
// }

// void    		    LocationBlock::setLimitExceptFlag(std::vector<std::string> tokens) {
//     for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
//         if (*it == "GET")
//             _GET_allowed = true;
//         if (*it == "POST")
//             _POST_allowed = true;
//         if (*it == "DELETE")
//             _DELETE_allowed = true;
//     }
// }


#include "LocationBlock.hpp"
#include <iostream> // Optional, if you add debug or output later

LocationBlock::LocationBlock()
    : _pathUri("/"),
      _documentRoot("/"),
      _defaultIndex("index.html"),
      _maxClientBodySize(100000),
      _allowGet(false),
      _allowPost(false),
      _allowDelete(false)
{}

LocationBlock::LocationBlock(const LocationBlock& other) {
    _pathUri = other._pathUri;
    _documentRoot = other._documentRoot;
    _defaultIndex = other._defaultIndex;
    _maxClientBodySize = other._maxClientBodySize;
    _allowGet = other._allowGet;
    _allowPost = other._allowPost;
    _allowDelete = other._allowDelete;
}

LocationBlock::~LocationBlock() {
    // No dynamic allocation, nothing to free
}

LocationBlock& LocationBlock::operator=(const LocationBlock& other) {
    if (this != &other) {
        _pathUri = other._pathUri;
        _documentRoot = other._documentRoot;
        _defaultIndex = other._defaultIndex;
        _maxClientBodySize = other._maxClientBodySize;
        _allowGet = other._allowGet;
        _allowPost = other._allowPost;
        _allowDelete = other._allowDelete;
    }
    return *this;
}

// Getters
std::string LocationBlock::getPathUri() const {
    return _pathUri;
}

std::string LocationBlock::getDocumentRoot() const {
    return _documentRoot;
}

std::string LocationBlock::getDefaultIndex() const {
    return _defaultIndex;
}

unsigned long LocationBlock::getMaxClientBodySize() const {
    return _maxClientBodySize;
}

bool LocationBlock::isGetAllowed() const {
    return _allowGet;
}

bool LocationBlock::isPostAllowed() const {
    return _allowPost;
}

bool LocationBlock::isDeleteAllowed() const {
    return _allowDelete;
}

// Setters
void LocationBlock::setPathUri(const std::string& uri) {
    _pathUri = uri;
}

void LocationBlock::setDocumentRoot(const std::string& rootPath) {
    _documentRoot = rootPath;
}

void LocationBlock::setDefaultIndex(const std::string& indexFile) {
    _defaultIndex = indexFile;
}

void LocationBlock::setMaxClientBodySize(unsigned long maxSize) {
    _maxClientBodySize = maxSize;
}

void LocationBlock::configureAllowedMethods(const std::vector<std::string>& methods) {
    _allowGet = false;
    _allowPost = false;
    _allowDelete = false;

    for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (*it == "GET")
            _allowGet = true;
        else if (*it == "POST")
            _allowPost = true;
        else if (*it == "DELETE")
            _allowDelete = true;
    }
}