
// #ifndef REQUEST_HPP
// #define REQUEST_HPP

// // #include "URI.hpp"
// // #include "utils.hpp"
// // #include "Config.hpp"

// #include "../config/ServerConfiguration.hpp"
// #include <map>

// # define BUFF_SIZE 2000

// // // Supported HTTP request methods
// typedef enum HttpMethod {
//     HTTP_GET = 0,
//     HTTP_POST,
//     HTTP_DELETE,
//     HTTP_UNDEFINED
// } HttpMethod;

// class Request {
// private:
//     // Raw HTTP request components
//     std::string  _raw_request;       // Complete incoming request buffer
//     std::string  _start_line;        // Example: "GET /index.html HTTP/1.1"
//     std::string  _header_section;    // Concatenated headers
//     std::string  _body_content;      // Request payload (if any)

//     // Parsing state
//     bool         _has_body;
//     unsigned long _content_length;
//     int          _bytes_read_from_body;
//     HttpResponseStatus   _status;

//     // HTTP-level parsed data
//     HttpMethod   _method;

//     // URI parsing and resolution to be implemented
//     // URI          _uri;               // Parsed URI object
//     std::string  _resource_path;     // Resolved path after URI parsing
//     std::map<std::string, std::string> _header_map;

//     // Internal parsing helpers
//     void         parseStartLine(ServerConfiguration* server);
//     void         parseHeaders();
//     void         parseBody();
//     void         extractURI(const std::string& uri_str);

// public:
//     std::string  parse_status; // Optional debug/status info for tracking parsing

//     // Constructor / Destructor
//     Request();
//     virtual ~Request();

//     // Getters
//     std::string                         getRawRequest() const;
//     std::string                         getStartLine() const;
//     std::string                         getHeaderSection() const;
//     std::string                         getBody() const;
//     HttpMethod                          getMethod() const;
//     // URI&                                getURI();
//     std::map<std::string, std::string>  getHeaders() const;
//     HttpResponseStatus                          getStatusCode() const;
//     std::string                         getResolvedResource() const;

//     // Setters
//     void                                setStatusCode(HttpResponseStatus status);

//     // Main processing functions
//     int   parseRequest(char buf[BUFF_SIZE], int byte_count, ServerConfiguration* server);
//     void  displayRequestInfo() const;
//     bool  headersParsedCompletely() const;
//     bool  isCompletelyParsed() const;

//     // HTTP-related exceptions
//     class BadRequestException : public std::exception {
//         public: const char* what() const throw();
//     };
//     class NotFoundException : public std::exception {
//         public: const char* what() const throw();
//     };
//     class HttpVersionUnsupportedException : public std::exception {
//         public: const char* what() const throw();
//     };
//     class RequestEntityTooLargeException : public std::exception {
//         public: const char* what() const throw();
//     };
//     class UnauthorizedAccessException : public std::exception {
//         public: const char* what() const throw();
//     };
//     class RedirectionException : public std::exception {
//         public: const char* what() const throw();
//     };
// };

// #endif // REQUEST_HPP