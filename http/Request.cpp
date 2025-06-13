// // #include "Request.hpp"
// // // #include "utils.hpp"
// // #include <sstream>
// // #include <algorithm>
// // #include <cstdlib>
// // #include <iostream>
// // #include <vector>
// // #include <string>
// // #include <map>
// // #include <exception>


// // // uitilities functions to be moved to utils.hpp
// // std::vector<std::string> ft_split(const std::string &str, char delim)
// // {
// //     // std::vector<std::string>    result;
// //     // std::stringstream           ss(str);
// //     // std::string                 item;

// //     // while (getline(ss, item, delim))
// //     // {
// //     //     result.push_back(item);
// //     // }
// //     // return result;
// // }

// // // Constructor / Destructor
// // Request::Request() : _raw_request(""), _body_content(""), _method(HTTP_UNDEFINED),
// //                      _bytes_read_from_body(0), _resource_path(""), _has_body(false),
// //                      _content_length(0), _status(HTTP_OK) {}

// // Request::~Request() {}

// // // Setters
// // void Request::setStatusCode(HttpResponseStatus status) {
// //     _status = status;
// // }

// // // Getters
// // std::string Request::getRawRequest() const {
// //     return _raw_request;
// // }

// // std::string Request::getStartLine() const {
// //     return _start_line;
// // }

// // std::string Request::getHeaderSection() const {
// //     return _header_section;
// // }

// // std::string Request::getBody() const {
// //     return _body_content;
// // }

// // HttpMethod Request::getMethod() const {
// //     return _method;
// // }

// // // URI& Request::getURI() {
// // //     return _uri;
// // // }

// // std::map<std::string, std::string> Request::getHeaders() const {
// //     return _header_map;
// // }

// // HttpResponseStatus Request::getStatusCode() const {
// //     return _status;
// // }

// // std::string Request::getResolvedResource() const {
// //     return _resource_path;
// // }

// // // State Checks
// // bool Request::headersParsedCompletely() const {
// //     return _raw_request.find("\r\n\r\n") != std::string::npos;
// // }

// // bool Request::isCompletelyParsed() const {
// //     return headersParsedCompletely() && (!_has_body || _body_content.length() == _content_length);
// // }

// // // Parsing Request
// // int Request::parseRequest(char* buf, int bytes_read, ServerConfiguration* server) {
// //     int cursor = bytes_read;

// //     while (cursor < BUFF_SIZE) {
// //         if (!headersParsedCompletely()) {
// //             _raw_request += buf[cursor];
// //             if (_raw_request.find("\r\n\r\n") != std::string::npos) {
// //                 parseStartLine(server);
// //                 parseHeaders();
// //                 displayRequestInfo();
// //             }
// //             bytes_read++;
// //         } else if (_has_body) {
// //             _body_content += buf[cursor];
// //             _bytes_read_from_body++;
// //             bytes_read++;
// //             if (static_cast<unsigned long>(_bytes_read_from_body) == _content_length) {
// //                 if (_content_length > server->getMaxClientBodySize())
// //                     throw RequestEntityTooLargeException();
// //                 break;
// //             }
// //         }
// //         cursor++;
// //     }
// //     return bytes_read;
// // }

// // // Internal helpers
// // static std::string trimSlashes(const std::string& path) {
// //     size_t first = path.find_first_not_of('/');
// //     if (first == std::string::npos) return "";
// //     size_t last = path.find_last_not_of('/');
// //     return path.substr(first, last - first + 1);
// // }

// // static LocationBlock* findMatchingLocation(std::vector<LocationBlock*>& locations, const std::string& uri_path) {
// //     for (std::vector<LocationBlock*>::iterator it = locations.begin(); it != locations.end(); ++it)
// //         if ((*it)->getPathUri() == uri_path)
// //             return *it;
// //     return NULL;
// // }

// // void Request::parseStartLine(ServerConfiguration* server) {
// //     _start_line = _raw_request.substr(0, _raw_request.find("\r\n"));

// //     std::string methods[] = {"GET", "POST", "DELETE"};
// //     std::size_t pos = _start_line.length();
// //     for (int i = 0; i < 3; ++i) {
// //         std::size_t found = _start_line.find(methods[i]);
// //         if (found != std::string::npos && found < pos)
// //             pos = found;
// //     }
// //     if (pos != _start_line.length())
// //         _start_line = _start_line.substr(pos);

// //     _raw_request = _raw_request.substr(_raw_request.find("\r\n") + 2);
// //     std::vector<std::string> tokens = ft_split(_start_line, ' ');

// //     if (tokens.size() != 3)
// //         throw BadRequestException();

// //     if (tokens[0] == "GET") _method = HTTP_GET;
// //     else if (tokens[0] == "POST") _method = HTTP_POST;
// //     else if (tokens[0] == "DELETE") _method = HTTP_DELETE;
// //     else throw BadRequestException();

// //     if ((_method == HTTP_GET && !server->_allowGET) ||
// //         (_method == HTTP_POST && !server->_allowPOST) ||
// //         (_method == HTTP_DELETE && !server->_allowDELETE))
// //         throw UnauthorizedAccessException();

// //     extractURI(tokens[1]);

// //     std::string uri_path = trimSlashes(_uri.getPath());
// //     if (is_subdirectory(server->getRootFolder(), uri_path)) {
// //         if (uri_path == "images")
// //             throw RedirectionException();

// //         LocationBlock* loc = findMatchingLocation(server->getLocationBlocks(), uri_path);
// //         if (!loc)
// //             throw UnauthorizedAccessException();

// //         _resource_path = loc->getRootFolder() + "/" + uri_path + "/" + loc->getIndexPage();
// //     } else if (tokens[1] == "/") {
// //         _resource_path = server->getRootFolder() + _uri.getPath() + server->getIndexPage();
// //     } else if (hasFileExtension(_uri.getPath(), ".php")) {
// //         _resource_path = _uri.getPath();
// //     } else {
// //         _resource_path = server->getRootFolder() + _uri.getPath();
// //     }

// //     if (!ft_is_resource_available(_resource_path) && !hasFileExtension(_uri.getPath(), ".php"))
// //         throw NotFoundException();

// //     if (tokens[2] != "HTTP/1.1")
// //         throw HttpVersionUnsupportedException();

// //     std::cout << "Requested Resource: " << _resource_path << " is available" << std::endl;
// // }

// // void Request::parseHeaders() {
// //     _header_section = _raw_request.substr(0, _raw_request.find("\r\n\r\n"));
// //     std::stringstream ss(_header_section);
// //     std::string line;

// //     while (std::getline(ss, line, '\r')) {
// //         std::stringstream line_stream(line);
// //         std::string key, value;

// //         std::getline(line_stream, key, ':');
// //         std::getline(line_stream, value);

// //         key.erase(key.begin(), std::find_if(key.begin(), key.end(), ft_is_non_whitespace));
// //         key.erase(std::find_if(key.rbegin(), key.rend(), ft_is_non_whitespace).base(), key.end());

// //         value.erase(value.begin(), std::find_if(value.begin(), value.end(), ft_is_non_whitespace));
// //         value.erase(std::find_if(value.rbegin(), value.rend(), ft_is_non_whitespace).base(), value.end());

// //         _header_map.insert(std::make_pair(key, value));
// //     }

// //     std::map<std::string, std::string>::iterator it = _header_map.find("Content-Length");
// //     if (it != _header_map.end()) {
// //         _has_body = true;
// //         _content_length = std::strtoul(it->second.c_str(), NULL, 0);
// //     }
// // }

// // void Request::parseRequestBody() {
// //     _body_content += _raw_request;
// // }

// void Request::extractURI(const std::string& uri_str) {
//     _uri.setPath(uri_str.substr(0, uri_str.find('?')));
//     if (uri_str.find('?') != std::string::npos)
//         _uri.setQuery(uri_str.substr(uri_str.find('?') + 1, uri_str.find('#')));
// }

// // Debug
// void Request::displayRequestInfo() const {
//     std::cout << "\n--- REQUEST PARSED ---\n";
//     std::cout << _start_line << std::endl;
//     std::cout << _header_section << std::endl;
//     std::cout << "--- END OF REQUEST ---\n" << std::endl;
// }

// // Exceptions
// const char* Request::BadRequestException::what() const throw() {
//     return "Bad Request";
// }

// const char* Request::NotFoundException::what() const throw() {
//     return "Not Found";
// }

// const char* Request::HttpVersionUnsupportedException::what() const throw() {
//     return "HTTP Version Not Supported (Only HTTP/1.1 accepted)";
// }

// const char* Request::RequestEntityTooLargeException::what() const throw() {
//     return "Payload Too Large";
// }

// const char* Request::UnauthorizedAccessException::what() const throw() {
//     return "Unauthorized";
// }

// const char* Request::RedirectionException::what() const throw() {
//     return "Redirect Required";
// }